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
#ifndef BTAMDAEMON_H
#define BTAMDAEMON_H

#include <QMap>
#include <QObject>

#include "campaign.h"
#include <QMutex>
#include <QPair>
#include <QtDBus/QDBusInterface>
#include <QProcess>


#include "log.h"
namespace BtAM {
	class ObexFtpFinisher : public QObject
	{
		Q_OBJECT
		public:
			ObexFtpFinisher(QProcess *parent, const Campaign &campaign,const QString &direccion);
		public slots:
			void finished(int estado);
		private:
			QProcess proceso;
			Campaign campaign;
			QString direccion;
	};
	class Daemon: public QObject
	{
		Q_OBJECT
		public:
			Daemon(const QStringList &arguments);
			~Daemon();

			void addCampaign(const Campaign & campaign);
			void removeCampaign(const Campaign & campaign);
			void updateCampaign(const Campaign & campaign);
			QList<Campaign> listCampaigns();
			QList<CampaignLogEntry> campaignsLog();
			QString nextId();
		private slots:
			void slotRemoteDeviceArrived(const QString &direccion, uint clase, short rssi);
		private:
			void parseArguments(const QStringList &arguments);
			void syncToDisk();
			void syncFromDisk();
			
			void aplicar(Campaign campaign, QString direccion);
			void enviar(Campaign campaign,QString direccion);
			
			QDBusInterface *bluetoothManager;
			QDBusInterface *bluetoothAdapter;
			
			QMutex campaignMutex;
			///Asocia a un identificador de campaña una campaña
			QMap<QString,Campaign> campaigns;
			///Asocia a un identificador de campaña un mapa con parejas direccion/fecha
			QMap<QString,QMap<QString,QDateTime> >  ultimosEnvios;

			uint m_nextId;
	};
}

#endif
