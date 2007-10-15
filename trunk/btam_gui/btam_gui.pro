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

FORMS += campaigndialog.ui \
logwindow.ui \
mainwindow.ui

INCLUDEPATH += ../libbtam

INSTALLS += target

target.path = /usr/bin

LIBS += ../libbtam/liblibbtam.a

TARGETDEPS += ../libbtam/liblibbtam.a

