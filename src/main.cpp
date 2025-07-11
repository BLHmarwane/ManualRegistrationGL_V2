#include <QtQml/qqmlregistration.h>

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>

#include "OpenGL3DViewport.hpp"

int main(int argc, char* argv[]) {
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

    // Try different paths to find the QML files
    QStringList possiblePaths = {
        "src/qml/main.qml",                           // From build directory
        "../src/qml/main.qml",                        // From build subdirectory
        "../../src/qml/main.qml",                     // From deeper build structure
        QDir::currentPath() + "/src/qml/main.qml",    // Absolute from current
        QDir::currentPath() + "/../src/qml/main.qml"  // Absolute from build dir
    };

    bool loaded = false;
    for (const QString& path : possiblePaths) {
        QUrl url = QUrl::fromLocalFile(QDir::currentPath() + "/" + path);
        if (QFile::exists(path)) {
            std::cout << "Loading QML from: " << path.toStdString() << std::endl;

            QObject::connect(
                &engine, &QQmlApplicationEngine::objectCreated, &app,
                [url](QObject* obj, const QUrl& objUrl) {
                    if (!obj && url == objUrl) {
                        QCoreApplication::exit(-1);
                    }
                },
                Qt::QueuedConnection);

            engine.load(url);

            if (!engine.rootObjects().isEmpty()) {
                loaded = true;
                break;
            }
        }
    }

    if (!loaded) {
        std::cerr << "Failed to load QML interface!" << std::endl;
        std::cerr << "Make sure you're running from the project root or build directory"
                  << std::endl;
        std::cerr << "Current working directory: " << QDir::currentPath().toStdString()
                  << std::endl;
        return -1;
    }

    std::cout << "SURGAR Interface loaded successfully!" << std::endl;
    std::cout << "Dual Model 3D OpenGL Backend integrated!" << std::endl;
    std::cout << "Research system ready!" << std::endl;

    return app.exec();
}