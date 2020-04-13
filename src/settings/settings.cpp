// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings.h"

/**
 * @brief Settings::Settings
 * @param dec
 * @param net
 * @param parent
 * Get decoder and network objects for getting and setting data
 */
Settings::Settings(Decoder *dec, Network *net, QObject *parent) : QObject(parent)
{
    settings = new QSettings();
    decoder = dec;
    network = net;
}

/**
 * @brief Settings::loadSettings
 * Load settings from the file (on application start)
 */
void Settings::loadSettings()
{
    qDebug() << "Loading settings";
    decoder->setFrequency(settings->value("frequency").toInt());
    decoder->setBandwidth(settings->value("bandwidth").toString());
    decoder->setSondeType(settings->value("sonde_type").toString());

    network->setSSID(settings->value("ssid").toString());
    network->setSecurity(settings->value("security").toString());
    network->setPassphrase(settings->value("passphrase").toString());
}

/**
 * @brief Settings::saveSettings
 * Save settings to the file
 * Called when updating decoder or network settings
 */
void Settings::saveSettings()
{
    qDebug() << "Saving settings";
    settings->setValue("frequency", decoder->getFrequency());
    settings->setValue("bandwidth", decoder->getBandwidth());
    settings->setValue("sonde_type", decoder->getSondeType());

    settings->setValue("ssid", network->getSSID());
    settings->setValue("security", network->getSecurity());
    settings->setValue("passphrase", network->getPassphrase());

    settings->sync();
}
