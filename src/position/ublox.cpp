// SPDX-License-Identifier: GPL-3.0-or-later

#include "ublox.h"

/**
 * @brief Ublox::Ublox
 * @param parent
 * All initialization is done in the constructor, further interaction is not required
 */
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

        qDebug() << "Connecting position signal";
        connect(ublox_source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(printPosition(QGeoPositionInfo)));

    } catch (...) {
        qDebug() << "An exception has occured!";
    }
}

/**
 * @brief Ublox::printPosition
 * @param location
 * Callback for positionUpdated signal
 * Emits positionChanged signal for the frontend
 */
void Ublox::printPosition(QGeoPositionInfo location)
{
    emit positionChanged(location.coordinate().latitude(), location.coordinate().longitude(), location.attribute(QGeoPositionInfo::Direction));
}
