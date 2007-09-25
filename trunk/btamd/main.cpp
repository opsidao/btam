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

#include <QCoreApplication>
#include <QDebug>
#include <QMetaType>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>

#include "btamd.h"
#include "btamd_adapter.h"

#include <syslog.h>


extern "C" {
	void logMessage(int priority, const char *msg)
	{
		openlog("btamd",LOG_PID,LOG_USER);
		syslog(priority,msg);
		closelog();
	}
}


void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			fprintf(stderr, "%s\n", msg);
			logMessage(LOG_DEBUG,msg);
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s\n", msg);
			logMessage(LOG_WARNING,msg);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n", msg);
			logMessage(LOG_CRIT,msg);
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", msg);
			logMessage(LOG_EMERG,msg);
			abort();
	}
}
int main(int argc, char** argv)
{
	qInstallMsgHandler(myMessageOutput);
	
	QCoreApplication app(argc,argv);
	QCoreApplication::setOrganizationName("opsiland");
	QCoreApplication::setOrganizationDomain("opsiland.info");
	QCoreApplication::setApplicationName("Bluetooth Advertising Manager");
	
	BtAM::Daemon daemon(app.arguments());
	new BtAM::DaemonAdapter(&daemon);
	QDBusConnection conn = QDBusConnection::sessionBus();	
	conn.registerService("org.opsiland.btam");
	if (!conn.registerObject("/org/opsiland/btam",&daemon,QDBusConnection::ExportAdaptors)) {
		qDebug() << conn.lastError().message() << endl;
	} else {
		qDebug() << "Demonio Bluetooth Advertising Manager en ejecucion!" << endl;
	}
	return app.exec();
}
