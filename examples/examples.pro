TEMPLATE = app

QT += qml quick

SOURCES += main.cpp
HEADERS += 

INCLUDEPATH += ../src

LIBPATH += ../src
LIBS += ../src/libsopomygga.a -lmosquitto -lmosquittopp 

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)


