// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings.h"

Settings::Settings(Decoder *dec, Network *net, QObject *parent) : QObject(parent)
{
    settings = new QSettings();
    decoder = dec;
    network = net;
}

void Settings::loadSettings()
{
    decoder->setFrequency(settings->value("frequency").toInt());
    decoder->setBandwidth(settings->value("bandwidth").toString());
    decoder->setSondeType(settings->value("sonde_type").toString());

    network->setSSID(settings->value("ssid").toString());
    network->setSecurity(settings->value("security").toString());
    network->setPassphrase(settings->value("passphrase").toString());
}

void Settings::saveSettings()
{
    settings->setValue("frequency", decoder->getFrequency());
    settings->setValue("bandwidth", decoder->getBandwidth());
    settings->setValue("sonde_type", decoder->getSondeType());

    settings->setValue("ssid", network->getSSID());
    settings->setValue("security", network->getSecurity());
    settings->setValue("passphrase", network->getPassphrase());

    settings->sync();
}
