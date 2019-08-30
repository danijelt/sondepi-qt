// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Page {
    id: detailsForm
    title: qsTr("Details")
    padding: 10

    GridLayout {
        columns: 2

        Label {
            id: freqLabel
            text: "Frequency"
        }

        Label {
            id: freqText
            text: "0"
        }

        Label {
            id: packetLabel
            text: "Packet number"
        }

        Label {
            id: packetText
            text: "0"
        }

        Label {
            id: serialLabel
            text: "Serial number"
        }

        Label {
            id: serialText
            text: "0"
        }

        Label {
            id: timeLabel
            text: "Time"
        }

        Label {
            id: timeText
            text: "0000-00-00 00:00:00"
        }

        Label {
            id: coordLabel
            text: "Coordinates"
        }

        Label {
            id: coordText
            text: "0.0000, 0.0000"
        }

        Label {
            id: altitudeLabel
            text: "Altitude"
        }

        Label {
            id: altitudeText
            text: "0 m"
        }

        Label {
            id: vhLabel
            text: "Horizontal speed"
        }

        Label {
            id: vhText
            text: "0.0 m/s"
        }

        Label {
            id: directionLabel
            text: "Direction"
        }

        Label {
            id: directionText
            text: "0 °"
        }

        Label {
            id: vvLabel
            text: "Vertical speed"
        }

        Label {
            id: vvText
            text: "0.0 m/s"
        }

        Label {
            id: temperatureLabel
            text: "Temperature"
        }

        Label {
            id: temperatureText
            text: "- °C"
        }

        Label {
            id: humidityLabel
            text: "Humidity"
        }

        Label {
            id: humidityText
            text: "- %"
        }
    }

    Connections {
        target: decoder

        onDetailsUpdated: {
            freqText.text = (decoder.getFrequency()/1000/1000).toFixed(3) + " MHz";
            packetText.text = packet;
            serialText.text = serial;
            timeText.text = time;
            coordText.text = lat + ", " + lon;
            altitudeText.text = alt + " m";
            vhText.text = vH + " m/s";
            directionText.text = dir + " °";
            vvText.text = vV + " m/s";
            temperatureText.text = temp + " °C";
            humidityText.text = hum + "%";

        }
    }
}
