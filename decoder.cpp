// SPDX-License-Identifier: GPL-3.0-or-later

#include "decoder.h"

Decoder::Decoder(QObject *parent) : QObject(parent)
{
    rtlfm = new QProcess(this);
    sox = new QProcess(this);
    decoder = new QProcess(this);

    this->killRtlFm();
}

Decoder::~Decoder() {
    decoder->terminate();
    sox->terminate();
    rtlfm->terminate();
    this->killRtlFm();
}

void Decoder::killRtlFm() {
    QProcess *killOldRtlSdr = new QProcess(this);
    QString kill_program = "pkill";
    QStringList kill_params;
    kill_params << "-9" << "rtl_fm";
    killOldRtlSdr->execute(kill_program, kill_params);
}

void Decoder::setFrequency(int freq) {
    frequency = freq;
}

int Decoder::getFrequency() {
    return frequency;
}

void Decoder::setBandwidth(QString bw) {
    bandwidth = bw;
}

QString Decoder::getBandwidth() {
    return bandwidth;
}

void Decoder::setSondeType(QString type) {
    sonde_type = type;
}

QString Decoder::getSondeType() {
    return sonde_type;
}

void Decoder::startRtlFm() {
    qDebug() << "Starting rtl_fm: frequency: " << frequency << ", bandwidth: " << bandwidth;

    this->killRtlFm();

    QString rtlfm_program = "rtl_fm";
    QStringList rtlfm_params;
    rtlfm_params << "-M" << "fm" << "-f" << QString::number(frequency) << "-s" << bandwidth;

    qDebug() << rtlfm_params;

    rtlfm->setStandardOutputProcess(sox);
    rtlfm->start(rtlfm_program, rtlfm_params);
}

void Decoder::startSox() {
    qDebug() << "Staring sox";

    QString sox_program = "sox";
    QStringList sox_params;
    sox_params << "-t" << "raw" << "-r" << bandwidth << "-es" << "-b16" << "-c1" << "-V1" << "-" << "-t" << "wav" << "-r" << "48k" << "-";

    qDebug() << sox_params;

    sox->setStandardOutputProcess(decoder);
    sox->start(sox_program, sox_params);
}

void Decoder::startDecoder() {
    qDebug() << "Starting decoder: " << sonde_type;

    QString decoder_program = sonde_type;
    QStringList decoder_params; // no parameters required at the moment

    decoder->start(decoder_program, decoder_params);
}

void Decoder::beginDecoding() {
    if (frequency > 0 && sonde_type != "") {
        this->killRtlFm();
        startRtlFm();
        startSox();
        startDecoder();

        decoder->setProcessChannelMode(QProcess::ForwardedChannels);
        connect(decoder, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    } else {
        qDebug() << "Frequency and sonde type must be set!";
    }
}

void Decoder::readStandardOutput() {
    QString processOutput;
    while (decoder->canReadLine()) {
       processOutput = QString::fromLocal8Bit(decoder->readLine());
    }

    qDebug() << "Output was " << processOutput;

    double lat_f = 0.0, lon_f = 0.0;

    QRegularExpression re_lat("lat\\:\\s([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_lat = re_lat.match(processOutput);
    if (match_lat.hasMatch()) {
        QString lat = match_lat.captured(1);
        qDebug() << "lat == " << lat;
        lat_f = lat.toDouble();
    }

    QRegularExpression re_lon("lon\\:\\s([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_lon = re_lon.match(processOutput);
    if (match_lon.hasMatch()) {
        QString lon = match_lon.captured(1);
        qDebug() << "lon == " << lon;
        lon_f = lon.toDouble();
    }

    emit sondePositionChanged(lat_f, lon_f);

}
