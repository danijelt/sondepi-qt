// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.VirtualKeyboard 2.1

Page {
    id: networkForm
    title: qsTr("Network Configuration")
    padding: 10

    GridLayout {
        columns: 2

        Label {
            id: ssidLabel
            text: "SSID"
        }

        TextField {
            Layout.preferredWidth: networkForm.width - x - 2*networkForm.padding
            id: ssidInput
            text: {
                return network.getSSID();
            }
        }

        Label {
            id: securityLabel
            text: "Security"
        }

        ComboBox {
            Layout.preferredWidth: networkForm.width - x - 2*networkForm.padding
            id: security
            textRole: "key"
            currentIndex: {
                var sec = network.getSecurity();
                if (sec !== "") {
                    for (var i = 0; i < security.model.count; i++) {
                        if (security.model.get(i).value === sec)
                            return i;
                    }
                }
            }

            model: ListModel {
                ListElement { key: "None or WEP"; value: "NONE" }
                ListElement { key: "WPA/WPA2 PSK"; value: "WPA-PSK" }
            }
        }

        Label {
            id: passLabel
            text: "Passphrase"
        }

        TextField {
            Layout.preferredWidth: networkForm.width - x - 2*networkForm.padding
            id: passInput
            text: {
                return network.getPassphrase();
            }
        }

        Label {
            id: ethIpLabel
            text: "Ethernet IP"
        }

        Label {
            id: ethIpText
            text: {
                network.getEthIp();
            }
        }

        Label {
            id: wlanIpLabel
            text: "WLAN IP"
        }

        Label {
            id: wlanIpText
            text: {
                network.getWlanIp();
            }
        }

        Button {
            text: "Apply"

            onClicked: {
                network.setSSID(ssidInput.text);
                network.setSecurity(security.model.get(security.currentIndex).value);
                network.setPassphrase(passInput.text);
                network.applySettings();
                settings.saveSettings();
            }
        }
    }

    Connections {
        target: network
    }

    Connections {
        target: settings
    }

    Timer {
            interval: 1000;
            running: true;
            repeat: true;
            onTriggered: {
                ethIpText.text = network.getEthIp();
                wlanIpText.text = network.getWlanIp();
            }
        }
}
