// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "position/ublox.h"
#include "decoders/decoder.h"
#include "settings/settings.h"
#include "network/network.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // organization name needed for configuration directory
    QCoreApplication::setOrganizationName("SondePi");
    // US locale for the keyboard (decimal point instead of comma, for easier parsing)
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // instantiate the classes
    Ublox *ublox = new Ublox();
    Decoder *decoder = new Decoder();
    Network *network = new Network();
    Settings *settings = new Settings(decoder, network);

    // load settings
    settings->loadSettings();

    // add objects to the root context for interaction with QML
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("ublox", ublox);
    ctx->setContextProperty("decoder", decoder);
    ctx->setContextProperty("network", network);
    ctx->setContextProperty("settings", settings);

    // load main.qml
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // start main loop
    return app.exec();
}
