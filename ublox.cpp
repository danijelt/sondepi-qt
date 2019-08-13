// SPDX-License-Identifier: GPL-3.0-or-later

#include "ublox.h"

Ublox::Ublox(QObject *parent) : QObject(parent)
{
    qDebug() << "Initializing NMEA source";
    try {
        qDebug() << "Initializing serial port";
        serialPort.setPortName("ttyAMA0");
        serialPort.setBaudRate(9600);

        qDebug() << "Configuring NMEA source";
        ublox_source = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode);
        ublox_source->setDevice(&serialPort);
        ublox_source->startUpdates();

        qDebug() << "Connecting signal";
        connect(ublox_source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(printPosition(QGeoPositionInfo)));

    } catch (...) {
        qDebug() << "An exception has occured!";
    }
}

void Ublox::printPosition(QGeoPositionInfo location)
{
    emit positionChanged(location.coordinate().latitude(), location.coordinate().longitude(), location.attribute(QGeoPositionInfo::Direction));
}
