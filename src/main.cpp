#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqmlregistration.h>
#include <iostream>
#include "OpenGL3DViewport.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // Set application information
    app.setApplicationName("SURGAR Manual Registration Simulator V2");
    app.setApplicationVersion("2.0.0");
    app.setOrganizationName("Research Lab");
    
    std::cout << "SURGAR Manual Registration Simulator V2 - Starting..." << std::endl;
    
    // Register the 3D viewport component
    qmlRegisterType<OpenGL3DViewport>("SURGAR.Components", 1, 0, "OpenGL3DViewport");
    
    // Initialize QML engine
    QQmlApplicationEngine engine;
    
    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/src/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);
    
    if (engine.rootObjects().isEmpty()) {
        std::cerr << "Failed to load QML interface" << std::endl;
        return -1;
    }

    std::cout << "SURGAR Interface loaded successfully!" << std::endl;
    std::cout << "3D OpenGL Backend integrated!" << std::endl;
    std::cout << "Demo ready for presentation!" << std::endl;
    
    return app.exec();
}
