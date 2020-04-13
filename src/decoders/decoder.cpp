// SPDX-License-Identifier: GPL-3.0-or-later

#include "decoder.h"

/**
 * @brief Decoder::Decoder
 * @param parent
 * Create new instances of QProcess class and kill old decoder process
 */
Decoder::Decoder(QObject *parent) : QObject(parent)
{
    rtlfm = new QProcess(this);
    sox = new QProcess(this);
    decoder = new QProcess(this);

    this->killOldProcess();
}

/**
 * @brief Decoder::~Decoder
 * Delete all QProcesses and kill decoder process
 */
Decoder::~Decoder() {
    delete rtlfm;
    delete sox;
    delete decoder;
    this->killOldProcess();
}

/**
 * @brief Decoder::killOldProcess
 * Kill old decoder process
 * Reinitialize QProcesses, use pkill to kill rtl_fm
 */
void Decoder::killOldProcess() {
    QProcess *killOldRtlSdr = new QProcess(this);
    QString kill_program = "pkill";
    QStringList kill_params;
    kill_params << "-9" << "rtl_fm";
    killOldRtlSdr->execute(kill_program, kill_params);
    delete rtlfm;
    delete sox;
    delete decoder;
    rtlfm = new QProcess(this);
    sox = new QProcess(this);
    decoder = new QProcess(this);
}

/**
 * @brief Decoder::setFrequency
 * @param freq
 * Frequency setter
 */
void Decoder::setFrequency(int freq) {
    frequency = freq;
}

/**
 * @brief Decoder::getFrequency
 * @return int frequency
 * Frequency getter
 */
int Decoder::getFrequency() {
    return frequency;
}

/**
 * @brief Decoder::setBandwidth
 * @param bw
 * Bandwidth setter
 */
void Decoder::setBandwidth(QString bw) {
    bandwidth = bw;
}

/**
 * @brief Decoder::getBandwidth
 * @return QString bandwidth
 * Bandwidth getter
 */
QString Decoder::getBandwidth() {
    return bandwidth;
}

/**
 * @brief Decoder::setSondeType
 * @param type
 * Sonde type setter
 */
void Decoder::setSondeType(QString type) {
    sonde_type = type;
}

/**
 * @brief Decoder::getSondeType
 * @return QString sonde_type
 * Sonde type getter
 */
QString Decoder::getSondeType() {
    return sonde_type;
}

/**
 * @brief Decoder::startRtlFm
 * Start rtl_fm process with requested frequency and bandwidth
 */
void Decoder::startRtlFm() {
    qDebug() << "Starting rtl_fm: frequency: " << frequency << ", bandwidth: " << bandwidth;

    QString rtlfm_program = "rtl_fm";
    QStringList rtlfm_params;
    rtlfm_params << "-M" << "fm" << "-f" << QString::number(frequency) << "-s" << bandwidth;
    // pipe output to sox
    rtlfm->setStandardOutputProcess(sox);
    rtlfm->start(rtlfm_program, rtlfm_params);
}

/**
 * @brief Decoder::startSox
 * Start sox process with requested bandwidth
 */
void Decoder::startSox() {
    qDebug() << "Staring sox";

    QString sox_program = "sox";
    QStringList sox_params;
    sox_params << "--buffer" << "1024" << "-t" << "raw" << "-r" << bandwidth << "-es" << "-b16" << "-c1" << "-V1" << "-" << "-t" << "wav" << "-r" << "48k" << "-";
    // pipe output to decoder
    sox->setStandardOutputProcess(decoder);
    sox->start(sox_program, sox_params);
}

/**
 * @brief Decoder::startDecoder
 * Start decoder for the requested sonde type
 */
void Decoder::startDecoder() {
    qDebug() << "Starting decoder: " << sonde_type;

    QString decoder_program = sonde_type;
    QStringList decoder_params;
    // --ptu and -vvv parameters enable printing temperature and humidity
    decoder_params << "--ptu" << "-vvv";
    decoder->start(decoder_program, decoder_params);
}

/**
 * @brief Decoder::beginDecoding
 * Begin the whole decoding process
 * Kills old decoder, starts new, connects the signal to process the output
 */
void Decoder::beginDecoding() {
    if (frequency > 0 && sonde_type != "") {
        this->killOldProcess();
        startRtlFm();
        startSox();
        startDecoder();

        decoder->setProcessChannelMode(QProcess::ForwardedChannels);
        qDebug() << "Connecting decoder signal";
        connect(decoder, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    } else {
        qDebug() << "Frequency and sonde type must be set!";
    }
}

/**
 * @brief Decoder::readStandardOutput
 * Parse decoder output
 * Position data, temperature and humidity are the same for every sonde
 * Serial and time are formatted differently for RS41 and M10
 * M10 decoder doesn't have the packet number
 */
void Decoder::readStandardOutput() {
    QString processOutput;
    while (decoder->canReadLine()) {
       processOutput = QString::fromLocal8Bit(decoder->readLine());
    }

    if (sonde_type == "rs41") {
        // packet number
        QRegularExpression re_packet("\\[\\s*([0-9]+)");
        QRegularExpressionMatch match_packet = re_packet.match(processOutput);
        if (match_packet.hasMatch()) {
            packet = match_packet.captured(1);
        }

        // sonde serial
        QRegularExpression re_serial("\\(([A-Z0-9]+)");
        QRegularExpressionMatch match_serial = re_serial.match(processOutput);
        if (match_serial.hasMatch()) {
            serial = match_serial.captured(1);
        }

        // date and time (from GPS)
        QRegularExpression re_time("[0-9]{4}-[0-9]{2}-[0-9]{2}\\s[0-9]{2}:[0-9]{2}:[0-9]{2}");
        QRegularExpressionMatch match_time = re_time.match(processOutput);
        if (match_time.hasMatch()) {
            time = match_time.captured(0);
        }
    } else if (sonde_type == "m10") {
        // M10 doesn't have the packet number
        packet = "-";

        // sonde serial
        QRegularExpression re_serial("SN:\\s+([0-9\\s]+)");
        QRegularExpressionMatch match_serial = re_serial.match(processOutput);
        if (match_serial.hasMatch()) {
            serial = match_serial.captured(1);
        }

        // date and time (from GPS)
        QRegularExpression re_time("[0-9]{4}-[0-9]{2}-[0-9]{2}\\s\\([0-9]{2}:[0-9]{2}:[0-9]{2}\\)");
        QRegularExpressionMatch match_time = re_time.match(processOutput);
        if (match_time.hasMatch()) {
            time = match_time.captured(0);
        }
    }

    // latitude
    QRegularExpression re_lat("lat:\\s+([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_lat = re_lat.match(processOutput);
    if (match_lat.hasMatch()) {
        lat = match_lat.captured(1).toDouble();
    }

    // longitude
    QRegularExpression re_lon("lon:\\s+([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_lon = re_lon.match(processOutput);
    if (match_lon.hasMatch()) {
        lon = match_lon.captured(1).toDouble();
    }

    // altitude
    QRegularExpression re_alt("alt:\\s+([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_alt = re_alt.match(processOutput);
    if (match_alt.hasMatch()) {
        alt = match_alt.captured(1);
    }

    // horizontal speed
    QRegularExpression re_vh("vH:\\s+([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_vh = re_vh.match(processOutput);
    if (match_vh.hasMatch()) {
        vH = match_vh.captured(1);
    }

    // azimuth
    QRegularExpression re_dir("D:\\s+([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_dir = re_dir.match(processOutput);
    if (match_dir.hasMatch()) {
        dir = match_dir.captured(1);
    }

    // vertical speed
    QRegularExpression re_vv("vV:\\s+([\\-\\+]?([0-9]*\\.[0-9]+|[0-9]+))");
    QRegularExpressionMatch match_vv = re_vv.match(processOutput);
    if (match_vv.hasMatch()) {
        vV = match_vv.captured(1);
    }

    // temperature
    QRegularExpression re_temp("T=([\\-\\+]?([0-9\\.]*))");
    QRegularExpressionMatch match_temp = re_temp.match(processOutput);
    if (match_temp.hasMatch()) {
        temp = match_temp.captured(1);
    }

    // humidity
    QRegularExpression re_hum("RH=([0-9\\.]*)");
    QRegularExpressionMatch match_hum = re_hum.match(processOutput);
    if (match_hum.hasMatch()) {
        hum = match_hum.captured(1);
    }

    // emit signals for MapForm and DetailsForm
    emit sondePositionChanged(lat, lon);
    emit detailsUpdated(packet, serial, time, lat, lon, alt, vH, dir, vV, temp, hum);
}
