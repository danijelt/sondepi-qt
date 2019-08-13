// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings.h"

Settings::Settings(Decoder *dec, QObject *parent) : QObject(parent)
{
    settings = new QSettings();
    decoder = dec;
}

void Settings::loadSettings()
{
    decoder->setFrequency(settings->value("frequency").toInt());
    decoder->setBandwidth(settings->value("bandwidth").toString());
    decoder->setSondeType(settings->value("sonde_type").toString());
}

void Settings::saveSettings()
{
    settings->setValue("frequency", decoder->getFrequency());
    settings->setValue("bandwidth", decoder->getBandwidth());
    settings->setValue("sonde_type", decoder->getSondeType());
    settings->sync();
}
