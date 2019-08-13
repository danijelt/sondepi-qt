// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ublox.h"
#include "decoder.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("SondePi");
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Ublox *ublox = new Ublox();
    Decoder *decoder = new Decoder();
    Settings *settings = new Settings(decoder);

    settings->loadSettings();

    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("ublox", ublox);
    ctx->setContextProperty("decoder", decoder);
    ctx->setContextProperty("settings", settings);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
