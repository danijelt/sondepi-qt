// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.VirtualKeyboard 2.1

Page {
    id: configForm
    title: qsTr("Configuration")
    padding: 10

    GridLayout {
        columns: 2

        Label {
            id: freqLabel
            text: "Frequency (MHz)"
        }

        TextField {
            Layout.preferredWidth: configForm.width - x - 2*configForm.padding
            id: freqInput
            // Limit input to the digits only
            inputMethodHints: Qt.ImhDigitsOnly
            // Requires 6 digits, 3 decimal places
            inputMask: "999.999"
            // Get frequency from the decoder
            text: {
                var frequency = (decoder.getFrequency()/1000/1000).toFixed(3);
                if (frequency > 0) {
                    return frequency;
                }
            }
        }

        Label {
            id: bandwidthLabel
            text: "Receiver bandwidth"
        }

        ComboBox {
            Layout.preferredWidth: configForm.width - x - 2*configForm.padding
            id: bandwidth
            // Key is passed to the decoder and used as a parameter in the decoder chain
            textRole: "key"
            // Get bandwidth from the decoder
            currentIndex: {
                var bw = decoder.getBandwidth();
                if (bw !== "") {
                    for (var i = 0; i < bandwidth.model.count; i++) {
                        if (bandwidth.model.get(i).value === bw)
                            return i;
                    }
                }
            }

            // List of currently supported bandwidths, experimentally determined as optimal
            model: ListModel {
                ListElement { key: "6 kHz (RS41)"; value: "6k" }
                ListElement { key: "10 kHz"; value: "10k" }
                ListElement { key: "15 kHz"; value: "15k" }
                ListElement { key: "24 kHz (M10)"; value: "24k" }
            }
        }

        Label {
            id: modelLabel
            text: "Sonde model"
        }

        ComboBox {
            Layout.preferredWidth: configForm.width - x - 2*configForm.padding
            id: sondeType
            // Key is passed to the decoder and used as the decoder executable name
            textRole: "key"
            // Get sonde type from the decoder
            currentIndex: {
                var sonde = decoder.getSondeType();
                if (sonde !== "") {
                    for (var i = 0; i < sondeType.model.count; i++) {
                        if (sondeType.model.get(i).value === sonde)
                            return i;
                    }
                }
            }

            // Currently tested and supported sondes
            model: ListModel {
                ListElement { key: "Vaisala RS41"; value: "rs41" }
                ListElement { key: "Meteomodem M10"; value: "m10" }
            }
        }

        // Apply and save the settings and begin decoding
        Button {
            text: "Apply and run"

            onClicked: {
                decoder.setFrequency(freqInput.text*1000*1000);
                decoder.setBandwidth(bandwidth.model.get(bandwidth.currentIndex).value);
                decoder.setSondeType(sondeType.model.get(sondeType.currentIndex).value);
                decoder.beginDecoding();
                settings.saveSettings();
                stackView.pop();
            }
        }
    }

    // Connect to the decoder object
    Connections {
        target: decoder
    }

    // Connect to the settings object
    Connections {
        target: settings
    }
}
