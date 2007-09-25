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
 *   auint with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "campaign.h"
#include <QDateTime>
#include <QDebug>

/*****************************************************/
/******************** Campaign ***********************/
/*****************************************************/
Campaign::Campaign()
	:mensajeOScript(false),habilitada(false),repetir(false),repetirPeriodo(0)
{
}
QDBusArgument &operator<<(QDBusArgument &argument, const Campaign & campaign)
{
	argument.beginStructure();
	argument << campaign.id 
			<< campaign.repetirPeriodo
			<< campaign.repetir
			<< campaign.nombre
			<< campaign.fechaInicio
			<< campaign.fechaFin
			<< campaign.mensajeOScript
			<< campaign.mensaje
			<< campaign.habilitada;
	argument.endStructure();
	return argument;
}

 // Retrieve the MyStructure data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &argument,Campaign &campaign)
{
	argument.beginStructure();
	argument >> campaign.id 
			>> campaign.repetirPeriodo
			>> campaign.repetir
			>> campaign.nombre
			>> campaign.fechaInicio
			>> campaign.fechaFin
			>> campaign.mensajeOScript
			>> campaign.mensaje
			>> campaign.habilitada;
	argument.endStructure();

	return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const CampaignLogEntry & entry)
{
	argument.beginStructure();
	argument << entry.campaignId 
			<< entry.campaignName
			<< entry.when
			<< entry.mensaje
			<< entry.titulo;
	argument.endStructure();
	return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument,CampaignLogEntry &entry)
{
	argument.beginStructure();
	argument >> entry.campaignId 
			>> entry.campaignName
			>> entry.when
			>> entry.mensaje
			>> entry.titulo;
	argument.endStructure();
	return argument;
}

CampaignLogEntry::CampaignLogEntry()
{
}


