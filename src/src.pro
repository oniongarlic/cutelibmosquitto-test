TEMPLATE = lib

QT += qml quick

CONFIG += staticlib

TARGET = sopomygga

LIBS += -lmosquitto -lmosquittopp

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

SOURCES += sopomygga.cpp
HEADERS += sopomygga.h

