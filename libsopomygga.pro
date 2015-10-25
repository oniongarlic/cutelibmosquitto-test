TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    sopomygga.cpp

LIBS += -lmosquittopp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    sopomygga.h

