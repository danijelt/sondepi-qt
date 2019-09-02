// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QNetworkInterface>

class Network : public QObject
{
private:
    Q_OBJECT
    QNetworkInterface intf_eth, intf_wlan;
    QNetworkAddressEntry ip_eth, ip_wlan;
    bool status_eth, status_wlan;
    QString ssid, security, passphrase;
public:
    explicit Network(QObject *parent = nullptr);
    Q_INVOKABLE QString getEthIp();
    Q_INVOKABLE QString getWlanIp();
    Q_INVOKABLE bool getEthStatus();
    Q_INVOKABLE bool getWlanStatus();
    Q_INVOKABLE void applySettings();
    Q_INVOKABLE void setSSID(QString name);
    Q_INVOKABLE QString getSSID();
    Q_INVOKABLE void setSecurity(QString sec);
    Q_INVOKABLE QString getSecurity();
    Q_INVOKABLE void setPassphrase(QString pass);
    Q_INVOKABLE QString getPassphrase();
};

#endif // NETWORK_H
