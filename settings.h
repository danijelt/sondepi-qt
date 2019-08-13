// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include "decoder.h"


class Settings : public QObject
{
private:
    Q_OBJECT
    QSettings *settings;
    Decoder *decoder;
public:
    explicit Settings(Decoder *dec, QObject *parent = nullptr);
    Q_INVOKABLE void loadSettings();
    Q_INVOKABLE void saveSettings();
};

#endif // SETTINGS_H
