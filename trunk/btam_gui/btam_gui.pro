TEMPLATE = app

SOURCES += main.cpp \
campaignconfig.cpp \
logwindow.cpp \
mainwindow.cpp \

HEADERS += campaignconfig.h \
logwindow.h \
mainwindow.h \

CONFIG += qt \
	warn_on \
	qdbus

QT += core ui

INCLUDEPATH += ../libbtam

LIBS += ../libbtam/liblibbtam.a

TARGETDEPS += ../libbtam/liblibbtam.a
FORMS += campaigndialog.ui \
logwindow.ui \
mainwindow.ui
