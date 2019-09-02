// SPDX-License-Identifier: GPL-3.0-or-later

#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{
}

QString Network::getEthIp()
{
    intf_eth = QNetworkInterface::interfaceFromName("eth0");
    if (!intf_eth.addressEntries().isEmpty())
        return intf_eth.addressEntries().first().ip().toString();
    else
        return "Interface down";
}

QString Network::getWlanIp()
{
    intf_wlan = QNetworkInterface::interfaceFromName("wlan0");
    if (!intf_wlan.addressEntries().isEmpty())
        return intf_wlan.addressEntries().first().ip().toString();
    else
        return "Interface down";
}

bool Network::getEthStatus()
{
    return intf_eth.IsUp;
}

bool Network::getWlanStatus()
{
    return intf_wlan.IsUp;
}

void Network::applySettings()
{
    QFile wpa_supplicant("/etc/wpa_supplicant.conf");
    if (!wpa_supplicant.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&wpa_supplicant);
    out << "ctrl_interface=/var/run/wpa_supplicant" << "\n";
    out << "network={" << "\n";
    out << "  ssid=\"" << getSSID() << "\"" << "\n";
    out << "  key_mgmt=" << getSecurity() << "\n";
    if(getSecurity() == "NONE") {
        out << "  wep_key0=\"" << getPassphrase() << "\"" << "\n";
    } else {
        out << "  psk=\"" << getPassphrase() << "\"" << "\n";
    }
    out << "}" << "\n";

    wpa_supplicant.close();

    QProcess *reloadWpaSupplicant = new QProcess(this);
    QString wpacli_program = "wpa_cli";
    QStringList wpacli_params;
    wpacli_params << "reconfigure";
    reloadWpaSupplicant->execute(wpacli_program, wpacli_params);
    delete reloadWpaSupplicant;
}

void Network::setSSID(QString name)
{
    ssid = name;
}

QString Network::getSSID()
{
    return ssid;
}

void Network::setSecurity(QString sec)
{
    security = sec;
}

QString Network::getSecurity()
{
    return security;
}

void Network::setPassphrase(QString pass)
{
    passphrase = pass;
}

QString Network::getPassphrase()
{
    return passphrase;
}
