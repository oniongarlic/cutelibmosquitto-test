#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "sopomygga.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<SopoMygga>("org.tal.sopomygga", 1, 0, "MQTT");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

