// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include "../decoders/decoder.h"
#include "../network/network.h"


class Settings : public QObject
{
private:
    Q_OBJECT
    QSettings *settings;
    Decoder *decoder;
    Network *network;
public:
    explicit Settings(Decoder *dec, Network *net, QObject *parent = nullptr);
    Q_INVOKABLE void loadSettings();
    Q_INVOKABLE void saveSettings();
};

#endif // SETTINGS_H
