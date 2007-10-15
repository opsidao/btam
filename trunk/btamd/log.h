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
#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QList>

class Campaign;
class CampaignLogEntry;
/**
	@author Juan González Aguilera <juan@aga-system.com>
 */
class Log
{	
	public:
		enum Level {SENT,NOTSENT,ALERT,INFO};
		static void add(Level level, const Campaign &campaign, const QString &message);
		static QList<CampaignLogEntry> getLog();
		
		static void syncToDisk();
		static void syncFromDisk();

	private:
		static QString fromLevel(Level level);
				
		static QList<CampaignLogEntry> log;
};

#endif
