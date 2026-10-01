#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "platform/windows/PhotoshopWindowManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    cleanflow::PhotoshopWindowManager photoshopManager;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("photoshopManager", &photoshopManager);
    engine.loadFromModule("CleanFlow", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
