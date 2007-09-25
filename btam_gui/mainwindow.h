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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDBus/QDBusInterface>
#include "ui_mainwindow.h"
#include <campaign.h>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

	public:
		MainWindow ( QWidget* parent = 0, Qt::WFlags fl = 0 );
		~MainWindow();
	private slots:
		void slotAddCampaign();
		void slotRemoveCampaign();
		void slotEditCampaign();
		void slotLogWindow();
		void slotSelectedItemChanged();
	private:
		void updateCampaignList(QList<Campaign> campaigns);
		QDBusInterface *remote;
		QList<Campaign> campaigns;
};

class BooleanItemDelegate : public QAbstractItemDelegate
{
	Q_OBJECT
	public:
		BooleanItemDelegate(QObject *parent);
		~BooleanItemDelegate();
		void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif

