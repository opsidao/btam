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



#include <campaign.h>

#include <QDateTime>
#include <QSettings>

#include "log.h"
QList<CampaignLogEntry> Log::log = QList<CampaignLogEntry>();
void Log::add(Level level, const Campaign &campaign, const QString & message)
{
	CampaignLogEntry entry;
	entry.when = QDateTime::currentDateTime().toString();
	entry.nivel = fromLevel(level);
	entry.campaignId = campaign.id;
	entry.campaignName = campaign.nombre;
	entry.mensaje = message;
	log << entry;
}

QString Log::fromLevel(Level level)
{
	QString out;
	switch(level)
	{
		case SENT:
			out = "SENT";
			break;
		case NOTSENT:
			out = "NOTSENT";
			break;
		case ALERT:
			out = "ALERT";
			break;
		case INFO:
			out = "INFO";
			break;
		default:
			Q_ASSERT(false);
	}
	return out;
}

void Log::syncToDisk()
{
	QSettings settings;
	settings.beginWriteArray("log");
	for (int i = 0; i < log.size();i++) {
		settings.setArrayIndex(i);
		settings.setValue("campaignId",log[i].campaignId);
		settings.setValue("campaignName",log[i].campaignName);
		settings.setValue("when",log[i].when);
		settings.setValue("nivel",log[i].nivel);
		settings.setValue("mensaje",log[i].mensaje);
	}
	settings.endArray();
}

void Log::syncFromDisk()
{
	QSettings settings;
	int size = settings.beginReadArray("log");
	for (int i = 0; i < size;i++) {
		settings.setArrayIndex(i);
		CampaignLogEntry entry;
		entry.campaignId = settings.value("campaignId").toString();
		entry.campaignName = settings.value("campaignName").toString();
		entry.when = settings.value("when").toString();
		entry.nivel = settings.value("nivel").toString();
		entry.mensaje = settings.value("mensaje").toString();
		log << entry;
	}
	settings.endArray();
}

QList< CampaignLogEntry > Log::getLog()
{
	return log;
}




