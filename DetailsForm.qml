// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Page {
    id: detailsForm
    title: qsTr("Details")
    padding: 10

    Label {
        text: qsTr("Information from last received sonde packet goes here.")
        anchors.centerIn: parent
    }

    GridLayout {
        columns: 2

        Label {
            id: freqLabel
            text: "Frequency"
        }

        Label {
            id: freqText
            text: "0 MHz"
        }

        Label {
            id: coordLabel
            text: "Coordinates"
        }

        Label {
            id: coordText
            text: "0.0000 0.0000"
        }

        Label {
            id: altitudeLabel
            text: "Altitude"
        }

        Label {
            id: altitudeText
            text: "0 m"
        }
    }
}
