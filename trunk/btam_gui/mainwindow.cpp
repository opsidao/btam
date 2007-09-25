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


#include "mainwindow.h"
#include "campaignconfig.h"
#include "logwindow.h"

#include <QDebug>
#include <QMessageBox>

#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusReply>

#include <QPainter>

MainWindow::MainWindow ( QWidget* parent, Qt::WFlags fl )
	: QMainWindow ( parent, fl )
{
	setupUi ( this );
	
// 	QTableWidget *activeCampaigns;
	
	activeCampaigns->setItemDelegateForColumn(4,new BooleanItemDelegate(this));
	activeCampaigns->setItemDelegateForColumn(5,new BooleanItemDelegate(this));
		
	
	
	REGISTER_CAMPAIGNS
	remote = new QDBusInterface ("org.opsiland.btam","/org/opsiland/btam","org.opsiland.btam.Daemon");
	if (remote->lastError().isValid()) {
		setEnabled(false);
		QDBusError error = remote->lastError();
		QString msg = "Lo mas probable es que no se este ejecutando el demonio btamd <br><br> Error  : <i><b>"+error.name()+ "</b></i><br>Mensaje: <i><b>"+error.message()+"</b></i>";
		QMessageBox::critical(this,"Imposible conectar al demonio btamd",msg);
	} else {
		QObject::connect(activeCampaigns,SIGNAL(itemSelectionChanged()),
				 this, SLOT(slotSelectedItemChanged()));
		QObject::connect(activeCampaigns,SIGNAL(cellDoubleClicked( int, int )),
				 this, SLOT(slotEditCampaign()));
		
		QDBusReply<QList<Campaign> > reply = remote->call("listCampaigns");
		if(reply.isValid())
			updateCampaignList(reply.value());
	
		QObject::connect(addCampaign,SIGNAL(clicked()),
				 this,SLOT(slotAddCampaign()));
	
		QObject::connect(removeCampaign,SIGNAL(clicked()),
				 this,SLOT(slotRemoveCampaign()));
	
		QObject::connect(editCampaign,SIGNAL(clicked()),
				 this,SLOT(slotEditCampaign()));
		
		QObject::connect(logButton,SIGNAL(clicked()),
				 this,SLOT(slotLogWindow()));
	}
}

MainWindow::~MainWindow()
{
	delete remote;
}

void MainWindow::slotAddCampaign()
{
	QDBusReply<QString> id = remote->call("nextId");
	CampaignConfig config(this,id.value());
	
	if (config.exec() == QDialog::Accepted) {
		QVariant var;
		var.setValue(config.campaign());
		remote->call("addCampaign",var);
		
		QDBusReply<QList<Campaign> > list = remote->call("listCampaigns");
		if(list.isValid())
			updateCampaignList(list.value());
	}
	
}

void MainWindow::slotRemoveCampaign()
{
	Campaign campaign = campaigns[activeCampaigns->currentRow()];
	
	QVariant var;
	var.setValue(campaign);
	remote->call("removeCampaign",var);
	
	QDBusReply<QList<Campaign> > list = remote->call("listCampaigns");
	if(list.isValid())
		updateCampaignList(list.value());
}

void MainWindow::slotEditCampaign()
{
	Campaign campaign = campaigns[activeCampaigns->currentRow()];
	CampaignConfig config(this,campaign);
	
	if (config.exec() == QDialog::Accepted) {
		QVariant var;
		var.setValue(config.campaign());
		remote->call("updateCampaign",var);
		
		QDBusReply<QList<Campaign> > list = remote->call("listCampaigns");
		if(list.isValid())
			updateCampaignList(list.value());

	}	
}

void MainWindow::slotLogWindow()
{
	QDBusReply<QList<CampaignLogEntry> > log = remote->call("campaignsLog");
	if (!log.isValid()) {
		QDBusError error = log.error();
		QString msg = "Lo mas probable es que no se este ejecutando el demonio btamd <br><br> Error  : <i><b>"+error.name()+ "</b></i><br>Mensaje: <i><b>"+error.message()+"</b></i>";
		QMessageBox::critical(this,"Fallo al obtener el log",msg);
		return;
	}


	LogWindow dialog(this,log.value());
	dialog.exec();
}

void MainWindow::slotSelectedItemChanged()
{
// 	qDebug() << "Current row : " << activeCampaigns->currentRow();
	if (activeCampaigns->currentRow()>=0) {
		removeCampaign->setEnabled(true);
		editCampaign->setEnabled(true);
	} else {
		removeCampaign->setEnabled(false);
		editCampaign->setEnabled(false);
	}
}

void MainWindow::updateCampaignList(QList<Campaign>  campaigns)
{
// 	  	QTableWidget *activeCampaigns;
	for (int i = activeCampaigns->rowCount()-1;i >=0;i--) {
		activeCampaigns->removeRow(i);
	}
	foreach (Campaign c, campaigns) {
		int r = activeCampaigns->rowCount();
		activeCampaigns->insertRow(r);
		activeCampaigns->setItem(r,0,new QTableWidgetItem(c.id));
		activeCampaigns->setItem(r,1,new QTableWidgetItem(c.nombre));
		activeCampaigns->setItem(r,2,new QTableWidgetItem(c.fechaInicio));
		activeCampaigns->setItem(r,3,new QTableWidgetItem(c.fechaFin));
		activeCampaigns->setItem(r,4,new QTableWidgetItem(c.habilitada ? "true":"false"));
		activeCampaigns->setItem(r,5,new QTableWidgetItem(c.repetir ? "true":"false"));
	}
	activeCampaigns->resizeColumnsToContents();
	this->campaigns = campaigns;
}

BooleanItemDelegate::BooleanItemDelegate(QObject * parent)
	:QAbstractItemDelegate(parent)
{
}

BooleanItemDelegate::~ BooleanItemDelegate()
{
}

void BooleanItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if(index.isValid())
	{
		QString s= index.data(Qt::DisplayRole).toString();
		QColor color = (s == "true") ? Qt::green : Qt::red;
		
		QRect rect(option.rect.x()+5,option.rect.y()+5,option.rect.width()-10,	option.rect.height()-10);
		painter->setBrush(QBrush(color));
		painter->drawRoundRect(rect,5,5);
	}
}

QSize BooleanItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(index)
	return option.rect.size();
}




