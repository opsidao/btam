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

#ifndef CAMPAIGNCONFIG_H
#define CAMPAIGNCONFIG_H

#include <campaign.h>
#include <QDialog>
#include <QSortFilterProxyModel>
#include "ui_campaigndialog.h"

class CampaignConfig : public QDialog, private Ui::Dialog
{
	Q_OBJECT

	public:
		CampaignConfig(QWidget* parent, Campaign c);
		CampaignConfig(QWidget* parent, QString id);
		~CampaignConfig();
		Campaign campaign();
	private:
		void init();
	private slots:
		void slotTipoMensajeCambiado();
		void slotSelecccionarScript();
};
class ExecutableFilter : public QSortFilterProxyModel
{
	public:
		bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;
};



#endif

