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
#ifndef BTAMDAEMONADAPTER_H
#define BTAMDAEMONADAPTER_H

#include <QtDBus/QDBusAbstractAdaptor>

#include "campaign.h"

namespace BtAM {
	class Daemon;
	class DaemonAdapter : public QDBusAbstractAdaptor
	{
		Q_OBJECT
		Q_CLASSINFO("D-Bus Interface", "org.opsiland.btam.Daemon")
		public:
			DaemonAdapter(Daemon *parent);
		public slots:
			void addCampaign(const Campaign & campaign);
			void removeCampaign(const Campaign & campaign);
			void updateCampaign(const Campaign & campaign);
			QList<Campaign> listCampaigns();
			QList<CampaignLogEntry> campaignsLog();
			QString nextId();
		private:
			Daemon *parent;
	};
}

#endif
