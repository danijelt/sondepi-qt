// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class Decoder : public QObject
{
private:
    Q_OBJECT
    int frequency = 0;
    QString bandwidth = "";
    QString sonde_type = "";

    void startRtlFm();
    void startSox();
    void startDecoder();
    void killOldProcess();

    QProcess *rtlfm;
    QProcess *sox;
    QProcess *decoder;

    double lat, lon;
    QString packet, serial, time, alt, vH, dir, vV, temp, hum;

    ~Decoder();

public slots:
    void readStandardOutput();

signals:
    void sondePositionChanged(double lat, double lon);
    void detailsUpdated(QString packet, QString serial, QString time, double lat, double lon, QString alt, QString vH, QString dir, QString vV, QString temp, QString hum);

public:
    explicit Decoder(QObject *parent = nullptr);
    Q_INVOKABLE void setFrequency(int freq);
    Q_INVOKABLE int getFrequency();
    Q_INVOKABLE void setBandwidth(QString bw);
    Q_INVOKABLE QString getBandwidth();
    Q_INVOKABLE void setSondeType(QString type);
    Q_INVOKABLE QString getSondeType();
    Q_INVOKABLE void beginDecoding();
};

#endif // DECODER_H
