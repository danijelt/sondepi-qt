// SPDX-License-Identifier: GPL-3.0-or-later

#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{
}

/**
 * @brief Network::getEthIp
 * @return QString eth0_ip
 * Get IP address of the eth0 interface
 */
QString Network::getEthIp()
{
    intf_eth = QNetworkInterface::interfaceFromName("eth0");
    if (!intf_eth.addressEntries().isEmpty())
        return intf_eth.addressEntries().first().ip().toString();
    else
        return "Interface down";
}

/**
 * @brief Network::getWlanIp
 * @return QString wlan0_ip
 * Get IP address of the wlan0 interface
 */
QString Network::getWlanIp()
{
    intf_wlan = QNetworkInterface::interfaceFromName("wlan0");
    if (!intf_wlan.addressEntries().isEmpty())
        return intf_wlan.addressEntries().first().ip().toString();
    else
        return "Interface down";
}

/**
 * @brief Network::applySettings
 * Apply network configuration
 * Write parameters to /etc/wpa_supplicant.conf and reload it
 */
void Network::applySettings()
{
    qDebug() << "Applying network settings";
    QFile wpa_supplicant("/etc/wpa_supplicant.conf");
    if (!wpa_supplicant.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // Passphrase uses different variable depending on the security level
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
    qDebug() << "wpa_supplicant.conf written";

    // call "wpa_cli reconfigure"
    QProcess *reloadWpaSupplicant = new QProcess(this);
    QString wpacli_program = "wpa_cli";
    QStringList wpacli_params;
    wpacli_params << "reconfigure";
    reloadWpaSupplicant->execute(wpacli_program, wpacli_params);
    delete reloadWpaSupplicant;
    qDebug() << "wpa_supplicant reloaded";
}

/**
 * @brief Network::setSSID
 * @param name
 * SSID setter
 */
void Network::setSSID(QString name)
{
    ssid = name;
}

/**
 * @brief Network::getSSID
 * @return QString ssid
 * SSID getter
 */
QString Network::getSSID()
{
    return ssid;
}

/**
 * @brief Network::setSecurity
 * @param sec
 * Network security setter
 */
void Network::setSecurity(QString sec)
{
    security = sec;
}

/**
 * @brief Network::getSecurity
 * @return QString security
 * Network security getter
 */
QString Network::getSecurity()
{
    return security;
}

/**
 * @brief Network::setPassphrase
 * @param pass
 * Passphrase setter
 */
void Network::setPassphrase(QString pass)
{
    passphrase = pass;
}

/**
 * @brief Network::getPassphrase
 * @return QString passphrase
 * Passphrase getter
 */
QString Network::getPassphrase()
{
    return passphrase;
}
