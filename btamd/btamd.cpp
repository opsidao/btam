/***************************************************************************
 *   Copyright (C) 2007 by Juan González Aguilera   *
 *   juan@aga-system.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "btamd.h"
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutexLocker>
#include <QSettings>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusReply>
#include <QProcess>
#include "log.h"

bool debugEnabled = false;

BtAM::Daemon::Daemon(const QStringList &arguments)
	:bluetoothManager(0), bluetoothAdapter(0),m_nextId(0)
{
	parseArguments(arguments);
	bluetoothManager = new QDBusInterface("org.bluez","/org/bluez","org.bluez.Manager",QDBusConnection::systemBus());
	if (!bluetoothManager->isValid()) {
		qCritical() << "No se ha podido conectar al servicio bluez : "+bluetoothManager->lastError().name() +": " + bluetoothManager->lastError().message();;
		return;
	}
	if (debugEnabled) {
		qDebug() << "Conectado al servicio de bluetooth!";
	}
	QDBusReply<QString> adapter = bluetoothManager->call("DefaultAdapter");
	if (!adapter.isValid()) {
		qCritical() << "No se ha encontrado ningun adaptador bluetooth!!";
		return;
	}
	if (debugEnabled) {
		qDebug() << "Se ha encontrado al menos un adaptador de bluetooth";
	}
	bluetoothAdapter = new QDBusInterface("org.bluez",adapter.value(),"org.bluez.Adapter",QDBusConnection::systemBus());
	if (!bluetoothAdapter->isValid()) {
		qCritical() << "Error al crear el adaptador (DBUS):"+bluetoothAdapter->lastError().name() +": " + bluetoothAdapter->lastError().message();
		return;
	}
	if (debugEnabled) {
		qDebug() << "Usando adaptador : "<< adapter.value();
	}
	
	syncFromDisk();
	
	QObject::connect(bluetoothAdapter,SIGNAL(RemoteDeviceFound(const QString&, uint,short)), this, SLOT(slotRemoteDeviceArrived(const QString&, uint,short)));
	
	bluetoothAdapter->call("SetPeriodicDiscoveryNameResolving",false);
	bluetoothAdapter->call("StartPeriodicDiscovery");
}

BtAM::Daemon::~Daemon()
{
	syncToDisk();
	delete bluetoothManager;
	delete bluetoothAdapter;
}

void BtAM::Daemon::addCampaign(const Campaign & campaign)
{
	QMutexLocker lock(&campaignMutex);
	if (campaign.id == QString::number(m_nextId)) {
		campaigns[campaign.id] = campaign;
		m_nextId++;
		syncToDisk();
	} else 
		qWarning() << "Se intento añadir una campaña con id invalido (esperado : " << nextId() << ", recibido : " << campaign.id << ")";
}

void BtAM::Daemon::removeCampaign(const Campaign & campaign)
{
	QMutexLocker lock(&campaignMutex);
	campaigns.remove(campaign.id);
	syncToDisk();
}

void BtAM::Daemon::updateCampaign(const Campaign & campaign)
{
	//TODO Check campaign id
	QMutexLocker lock(&campaignMutex);
	if (campaigns.contains(campaign.id)) {
		campaigns[campaign.id] = campaign;
		syncToDisk();
	}
}

QList<Campaign> BtAM::Daemon::listCampaigns()
{
	QMutexLocker lock(&campaignMutex);
	return campaigns.values();
}

QList<CampaignLogEntry> BtAM::Daemon::campaignsLog()
{
	QMutexLocker lock(&campaignMutex);
	return Log::getLog();
}

QString BtAM::Daemon::nextId()
{
	QMutexLocker lock(&campaignMutex);
	return QString::number(m_nextId);
}

void BtAM::Daemon::slotRemoteDeviceArrived(const QString & direccion, uint clase, short int rssi)
{
	Q_UNUSED(clase);
	Q_UNUSED(rssi);
	if (debugEnabled) {
		qDebug() << "Remote arrived: " << direccion;
	}
	foreach ( Campaign campaign, campaigns.values()) {
		if(campaign.habilitada)
			aplicar(campaign,direccion);
	}
	syncToDisk();	
}

void BtAM::Daemon::aplicar(Campaign campaign, QString direccion)
{
	QDateTime curr = QDateTime::currentDateTime();
	if (curr > QDateTime::fromString(campaign.fechaInicio) &&
		   curr < QDateTime::fromString(campaign.fechaFin)) 
	{
		if (!ultimosEnvios.contains(campaign.id)) {
			ultimosEnvios[campaign.id] = QMap<QString,QDateTime> ();
		}
		if (!ultimosEnvios[campaign.id].contains(direccion)) {//Aun no hemos visto a este dispositivo en esta campaña
			if (debugEnabled) {
				qDebug() << "Campaña: " << campaign.nombre << ", enviando a " << direccion;
			}
			Log::add(Log::INFO,campaign,"Nuevo receptor (" + direccion + ")");
			enviar(campaign,direccion);
			ultimosEnvios[campaign.id].insert(direccion,curr);
			
		} else if(campaign.repetir) { //Los mensajes se envian otra vez, comprobemos si ha pasado sugiciente tiempo
			QDateTime ultimoEnvio = ultimosEnvios[campaign.id].value(direccion);
			if (ultimoEnvio.addSecs(campaign.repetirPeriodo*60) < curr) { //Reenviamos porque ya ha pasado el tiempo suficiente
				if (debugEnabled) {
					qDebug() << "Campaña: " << campaign.nombre << ", reenviando a " << direccion 
							<< "ultimoEnvio = " << ultimoEnvio << " actual = " << curr;
				}
				Log::add(Log::INFO,campaign,"Receptor conocido (" + direccion + "), reenviando");
				enviar(campaign,direccion);
			} else {
				Log::add(Log::INFO,campaign,"Receptor conocido (" + direccion + "), timeout aun no superado");
				if (debugEnabled) {
 					qDebug() << "No reenviando aun a " << direccion;
				}
			}
		} else {
			if (debugEnabled) {
	 			qDebug() << "Ignorando a " << direccion << ", ya se le envio";
			}
			Log::add(Log::INFO,campaign,"Receptor conocido (" + direccion + ")");			
		}

		syncToDisk();
	} else if (debugEnabled) {
		qDebug()  << "Campaña: " << campaign.nombre << ", fuera de fecha";
	}
}

void BtAM::Daemon::enviar(Campaign campaign, QString direccion)
{
	qDebug() << "Direccion: " << direccion;
	if (direccion != "00:1C:9A:3D:1B:12")
		return;
	
	QProcess * obexftp = new QProcess(this);
	QStringList arg;
	arg << "--nopath" << "--noconn" << "--uuid" << "none" << "--bluetooth" << direccion << "--put";
	
	QString fileName;
	if (campaign.mensajeOScript) { //Es mensaje...
		fileName = QDir::tempPath()+campaign.nombre+".txt";
		QFile file(fileName);
		file.open(QFile::ReadWrite| QFile::Truncate |QFile::Text);
		file.write(campaign.mensaje.toUtf8());
		file.close();
		//FIXME The file should be eventually removed after sending
		//FIXME Possible conflicts in consequtive sents
	} else { //Es script...
		QProcess script;
		script.start(campaign.mensaje);
		script.waitForFinished();
		fileName = script.readAllStandardOutput().trimmed();
	}
	if (fileName.isEmpty()) {
		Log::add(Log::ALERT,campaign,"El nombre del fichero a enviar esta vacio");
		return;
	}
	arg << fileName;
	bluetoothAdapter->call("SetName",campaign.nombre);
	ultimosEnvios[campaign.id].insert(direccion,QDateTime::currentDateTime());
	QObject::connect(obexftp,SIGNAL(finished( int )),new ObexFtpFinisher(obexftp,campaign,direccion),SLOT(finished(int)));
	obexftp->start("obexftp",arg);
}

void BtAM::Daemon::parseArguments(const QStringList & arguments)
{
	if (arguments.size() > 1) {
		for (int i=1; i < arguments.size(); i++) {
			QString arg = arguments[i];
			if (arg == "-debug") {
				qDebug() << "Debug mode enabled";
				debugEnabled = true;
			}
		}
	}
}

void BtAM::Daemon::syncToDisk()
{
	QSettings settings;
	settings.setValue("nextId",m_nextId);
	settings.beginWriteArray("campaigns");
	int i = 0;
	foreach (Campaign campaign,campaigns.values()) {
		settings.setArrayIndex(i++);
		settings.setValue("id",campaign.id);
		settings.setValue("nombre",campaign.nombre);
		settings.setValue("fechaInicio",campaign.fechaInicio);
		settings.setValue("fechaFin",campaign.fechaFin);
		settings.setValue("mensajeOScript",campaign.mensajeOScript);
		settings.setValue("mensaje",campaign.mensaje);
		settings.setValue("habilitada",campaign.habilitada);
		settings.setValue("repetir",campaign.repetir);
		settings.setValue("repetirPeriodo",campaign.repetirPeriodo);
	}
	settings.endArray();
	
	settings.beginGroup("ultimosEnvios");
	i=0;
	foreach ( QString campaign, ultimosEnvios.keys() ) {
		settings.beginWriteArray(campaign);
		foreach ( QString direccion, ultimosEnvios[campaign].keys() ) {
			settings.setArrayIndex(i++);
			settings.setValue("direccion",direccion);
			settings.setValue("datetime",ultimosEnvios[campaign].value(direccion));
		}
		settings.endArray();
	}
	settings.endGroup();
	
	Log::syncToDisk();
	settings.sync();
}

void BtAM::Daemon::syncFromDisk()
{
	QSettings settings;
	m_nextId = settings.value("nextId").toUInt();
	int size = settings.beginReadArray("campaigns");
	for (int i=0; i<size; i++) {
		settings.setArrayIndex(i);
		Campaign c;
		c.id = settings.value("id").toString();
		c.nombre = settings.value("nombre").toString();
		c.fechaInicio = settings.value("fechaInicio").toString();
		c.fechaFin = settings.value("fechaFin").toString();
		c.mensajeOScript = settings.value("mensajeOScript").toBool();
		c.mensaje = settings.value("mensaje").toString();
		c.habilitada = settings.value("habilitada").toBool();
		c.repetir = settings.value("repetir").toBool();
		c.repetirPeriodo = settings.value("repetirPeriodo").toUInt();
		campaigns[c.id] = c;
	}
	settings.endArray();
	
	settings.beginGroup("ultimosEnvios");
	foreach (QString campaign, settings.childGroups() ) {
 		size = settings.beginReadArray(campaign);
		for (int i = 0; i < size; i++) {
			settings.setArrayIndex(i);
			QString direccion = settings.value("direccion").toString();
			QDateTime datetime = settings.value("datetime").toDateTime();
			ultimosEnvios[campaign].insert(direccion,datetime);
		}
		settings.endArray();
	}
	Log::syncFromDisk();
	settings.endGroup();
	
}

/**************** ObexFtpFinisher *************/

BtAM::ObexFtpFinisher::ObexFtpFinisher(QProcess *parent, const Campaign &campaign,const QString &direccion)
	:QObject(parent),proceso(parent),campaign(campaign),direccion(direccion)
{
}

void BtAM::ObexFtpFinisher::finished(int estado)
{
	QString out = proceso.readAllStandardError();
	if (estado == 0) { //Enviado correctamente
		if(debugEnabled)
		{
			qDebug() << "Campaña : " << campaign.id << ", mensaje enviado con exito a " << direccion;
		}
		Log::add(Log::SENT,campaign,out);
	} else { //Falló el envio
		if(debugEnabled)
		{
			qDebug() << "Campaña : " << campaign.id << ", no se pudo enviar el mensaje a " << direccion;
		}
		Log::add(Log::NOTSENT,campaign,out);	
	}
	deleteLater();
}

