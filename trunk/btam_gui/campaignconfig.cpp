/***************************************************************************
 *   Copyright (C) 2007 by Juan Gonz�lez Aguilera   *
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


#include "campaignconfig.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>

CampaignConfig::CampaignConfig(QWidget* parent,Campaign c)
	: QDialog( parent)
{
	init();	
		
	identificador->setText(c.id);
	nombre->setText(c.nombre);
	
	QDateTime dt = QDateTime::fromString(c.fechaInicio);
	startDate->setSelectedDate(dt.date());
	startTime->setTime(dt.time());
	
	dt = QDateTime::fromString(c.fechaFin);
	endDate->setSelectedDate(dt.date());
	endTime->setTime(dt.time());
	
	if (c.mensajeOScript) {
		esMensaje->setChecked(true);
		mensaje->setText(c.mensaje);
	} else {
		esScript->setChecked(true);
		pathMensaje->setText(c.mensaje);
	}
		
	habilitada->setChecked(c.habilitada);
	reenviar->setChecked(c.repetir);
	
	reenviarDelay->setValue(c.repetirPeriodo);
	QLineEdit *nombre;
	nombre->setFocus();
	
}

CampaignConfig::CampaignConfig(QWidget * parent, QString id)
	: QDialog( parent)
{
	init();
	
	identificador->setText(id);
}


CampaignConfig::~CampaignConfig()
{
}

Campaign CampaignConfig::campaign()
{
	Campaign out;
	out.id = identificador->text();
	out.nombre = nombre->text();
	out.fechaInicio = QDateTime(startDate->selectedDate(),startTime->time()).toString();
	out.fechaFin = QDateTime(endDate->selectedDate(),endTime->time()).toString();
	out.mensajeOScript = esMensaje->isChecked();
	out.mensaje = esMensaje->isChecked() ? mensaje->toPlainText() : pathMensaje->text();
	out.habilitada = habilitada->isChecked();
	out.repetir = reenviar->isChecked();
	out.repetirPeriodo = out.repetir ? reenviarDelay->value() : 0;
	
	if (esMensaje->isChecked()) {
		mensaje->setEnabled(true);
		pathMensaje->setEnabled(false);
		findMsgPathButton->setEnabled(false);
	} else {
		mensaje->setEnabled(false);
		pathMensaje->setEnabled(true);
		findMsgPathButton->setEnabled(true);
	}
	return out;
}

void CampaignConfig::init()
{
	setupUi(this);
	QObject::connect(findMsgPathButton,SIGNAL(clicked()),
					 this,SLOT(slotSelecccionarScript()));
	QObject::connect(esMensaje,SIGNAL(toggled(bool)),
					 this,SLOT(slotTipoMensajeCambiado()));
}

void CampaignConfig::slotTipoMensajeCambiado()
{
	if (esMensaje->isChecked()) {
		mensaje->setEnabled(true);
		pathMensaje->setEnabled(false);
		findMsgPathButton->setEnabled(false);
	} else {
		mensaje->setEnabled(false);
		pathMensaje->setEnabled(true);
		findMsgPathButton->setEnabled(true);
	}
}

void CampaignConfig::slotSelecccionarScript()
{
	QFileDialog dialog(this,"Seleccionar script...");
	dialog.setFilter("Todos los ejecutables (*)");
	dialog.setProxyModel(new ExecutableFilter);
	dialog.setFileMode(QFileDialog::ExistingFile);
	if (dialog.exec() == QDialog::Accepted) {
		QStringList selectedFiles = dialog.selectedFiles();
		Q_ASSERT(selectedFiles.size()==1);
		pathMensaje->setText(selectedFiles[0]);
	}
}

bool ExecutableFilter::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{	
	QModelIndex index = source_parent;
	QString path = index.child(source_row,0).data(Qt::DisplayRole).toString();
	do	{
		path = index.data(Qt::DisplayRole).toString() + "/" + path;
		index = index.parent();
	} while(index.isValid()); //FIXME El path termina empezando por //
	QFile file(path);
	Q_ASSERT(file.exists());
	return (file.permissions() & QFile::ExeUser)!=0;
}






