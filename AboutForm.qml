// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: aboutForm
    title: qsTr("About")
    padding: 10

    Label {
        text: qsTr(
                  "SondePi by Danijel Tudek (9A5DTL)\n\n" +
                  "SondePi is licensed under GPL version 3.\n" +
                  "SondePi uses Qt library licensed under LGPL or GPL version 3.\n\n" +
                  "Map markers are licensed under CC BY-NC 4.0, source: http://www.pngall.com/map-marker-png"
        )
    }
}
