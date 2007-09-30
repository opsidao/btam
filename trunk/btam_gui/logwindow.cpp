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

#include <QDebug>
#include "logwindow.h"

LogWindow::LogWindow(QWidget* parent,QList<CampaignLogEntry> log)
	: QDialog( parent)
{
	setupUi(this);
	QMap<QString,QString> campaigns;
	
// 	QTableWidget *logTable;	
	logTable->setRowCount(log.size());
	int i = 0;
	foreach (CampaignLogEntry logEntry, log ) {
		if (!campaigns.contains(logEntry.campaignId)) {
			campaigns[logEntry.campaignId] = logEntry.campaignName;
		}
// 		qDebug() << "Row: " << i << " Id: " << logEntry.campaignName;
		logTable->setItem(i,0,new QTableWidgetItem(logEntry.nivel));
		logTable->setItem(i,1,new QTableWidgetItem(logEntry.campaignId+" - "+logEntry.campaignName));
		logTable->setItem(i,2,new QTableWidgetItem(logEntry.when));		
		logTable->setItem(i,3,new QTableWidgetItem(logEntry.mensaje));
		i++;
	}
	logTable->setSortingEnabled(true);
	logTable->sortByColumn(2,Qt::DescendingOrder);
	logTable->resizeColumnsToContents();
	
// 	QComboBox *filtroCampaign;
	foreach (QString id, campaigns.keys()) {
		filtroCampaign->addItem(id+"\t"+ campaigns[id]);
	}
	QObject::connect(filtroCampaign,SIGNAL(activated( int )),
					 this,SLOT(filtrarCampaign(int)));
}

LogWindow::~LogWindow()
{
}

void LogWindow::filtrarCampaign(int index)
{
// 	QComboBox *filtroCampaign;
// 	QTableWidget *logTable;
	QString filtro = filtroCampaign->itemText(index);
	if (filtro == "Todas") {
		filtro = "";
	} else {
		filtro = filtro.split("\t")[0];
	}
	logTable->setSortingEnabled(false);
	for (int i = 0; i < logTable->rowCount();i++) {
		if (filtro.isEmpty()) {
			logTable->setRowHidden(i,false);
		} else if (logTable->item(i,1)->text().split(" - ")[0] == filtro) {
			logTable->setRowHidden(i,false);
		} else {
			logTable->setRowHidden(i,true);
		}
	}
	logTable->setSortingEnabled(true);
}


