TEMPLATE = app

SOURCES += main.cpp \
    btamd.cpp \
    btamd_adapter.cpp \
    log.cpp
HEADERS += btamd.h \
    btamd_adapter.h \
    log.h
CONFIG += qt \
    warn_on \
    qdbus
QT += core

INCLUDEPATH += ../libbtam

LIBS += ../libbtam/liblibbtam.a

TARGETDEPS += ../libbtam/liblibbtam.a

INSTALLS += target \
service

target.path = /usr/bin/

service.files += btamd.service
service.path = /usr/share/dbus-1/services/

