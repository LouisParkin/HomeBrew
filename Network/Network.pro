#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T11:30:03
#
#-------------------------------------------------

QT       += network xml xmlpatterns

QT       -= gui

TARGET = Network
TEMPLATE = lib
CONFIG += staticlib

SOURCES += network.cpp

HEADERS += network.h \
    socket_types.h \
    connection_states.h \
    network_typedefinitions.h \
    network_defs.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
