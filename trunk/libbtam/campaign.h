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
#ifndef BTAMCAMPAIGN_H
#define BTAMCAMPAIGN_H

#include <QMetaType>
#include <QString>
#include <QtDBus/QDBusArgument>

/// This macro needs to go within any method or ctor, and you must #include <QtDBus/QDbusMetaType> for it to work
#define REGISTER_CAMPAIGNS\
	qDBusRegisterMetaType<Campaign>();\
	qDBusRegisterMetaType<QList<Campaign> >();\
	qDBusRegisterMetaType<CampaignLogEntry>();\
	qDBusRegisterMetaType<QList<CampaignLogEntry> >();\

class Campaign {
	public:
		Campaign();
		/// Identificador unico de campaña (non-editable)
		QString id;
			
		/// Nombre de la campaña
		QString nombre;
	
		QString fechaInicio;
		QString fechaFin;
			
		/// Indica si la campaña tiene un mensaje explicito o usa un script
		bool mensajeOScript;
			
		/// Si mensajeOScript es true el mensaje explicito, sino el comando a ejecutar
		/// para obtener el mensaje
		QString mensaje;
			
		/// Indica si la campaña esta habilitada
		bool habilitada;
			
		/// Indica si se debe volver a enviar un mensaje a un usuario que ya recibio
		/// otro en esta campaña.
		bool repetir;
			
		/// Cuanto tiempo a de pasar antes de que se le vuelva a enviar el mensaje
		/// a un usuario que ya lo ha recibido en esta campaña (en minutos)
		uint repetirPeriodo;
};
	
QDBusArgument &operator<<(QDBusArgument &argument, const Campaign & campaign);
const QDBusArgument &operator>>(const QDBusArgument &argument,Campaign &campaign);
	
Q_DECLARE_METATYPE(Campaign);

class CampaignLogEntry {
	public:
		CampaignLogEntry();
		QString campaignId;
		QString campaignName;
		QString when;
		QString nivel;
		QString mensaje;
};

QDBusArgument &operator<<(QDBusArgument &argument, const CampaignLogEntry & entry);
const QDBusArgument &operator>>(const QDBusArgument &argument,CampaignLogEntry &entry);


Q_DECLARE_METATYPE(CampaignLogEntry);

// typedef QList<Campaign> QList<Campaign> ;

Q_DECLARE_METATYPE(QList<Campaign>);

// typedef QList<CampaignLogEntry> QList<CampaignLogEntry> ;

Q_DECLARE_METATYPE(QList<CampaignLogEntry>);

#endif
