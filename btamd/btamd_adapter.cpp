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
#include <QDebug>

#include "btamd_adapter.h"
#include "btamd.h"
#include <QtDBus/QDBusMetaType>

BtAM::DaemonAdapter::DaemonAdapter(BtAM::Daemon *parent)
	: QDBusAbstractAdaptor(parent),parent(parent)
{
	REGISTER_CAMPAIGNS
}

void BtAM::DaemonAdapter::addCampaign(const Campaign & campaign)
{
	qDebug() << "Nueva campaña" << campaign.id;
// 	qDebug() << "Id     :" << campaign.id;
// 	qDebug() << "Nombre :" << campaign.nombre;
//  	qDebug() << "Inicio :" << campaign.fechaInicio;
//  	qDebug() << "Fin    :" << campaign.fechaFin;
// 	qDebug() << "M/S    :" << campaign.mensajeOScript;
// 	qDebug() << "Mensaje:" << campaign.mensaje;
// 	qDebug() << "Enabled:" << campaign.habilitada;
// 	qDebug() << "Repite :" << campaign.repetir;
// 	qDebug() << "Cada   :" << campaign.repetirPeriodo << " minutos";
	parent->addCampaign(campaign);	
}

void BtAM::DaemonAdapter::removeCampaign(const Campaign & campaign)
{
	qDebug() << "Remove campaign" << campaign.id;
	parent->removeCampaign(campaign);
}

void BtAM::DaemonAdapter::updateCampaign(const Campaign & campaign)
{
	qDebug() << "Update campaign" << campaign.id;
	parent->updateCampaign(campaign);
}

QList<Campaign> BtAM::DaemonAdapter::listCampaigns()
{
	return parent->listCampaigns();
}

QList<CampaignLogEntry> BtAM::DaemonAdapter::campaignsLog()
{
	return parent->campaignsLog();
}

QString BtAM::DaemonAdapter::nextId()
{
	return parent->nextId();
}
