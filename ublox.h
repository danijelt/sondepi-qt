// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UBLOX_H
#define UBLOX_H

#include <QObject>
#include <QtPositioning/QNmeaPositionInfoSource>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Ublox : public QObject
{

private:
    Q_OBJECT
    QNmeaPositionInfoSource *ublox_source;
    QSerialPort serialPort;

public:
    explicit Ublox(QObject *parent = nullptr);

signals:
    void positionChanged(double lat, double lon, double heading);

private slots:
    void printPosition(QGeoPositionInfo update);
};

#endif // UBLOX_H
