// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtLocation 5.12
import QtPositioning 5.12

Page {
    title: qsTr("Map")
    property string trackingMode: "none";
    property string mapType: "standard";

    Image {
        id: sondeMarkerIcon
        source: "qrc:/marker-red.png"
        sourceSize.width:32
        sourceSize.height: 32
    }

    Image {
        id: userMarkerIcon
        source: "qrc:/marker-yellow.png"
        sourceSize.width:32
        sourceSize.height: 32
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: osmMap
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(0, 0)
        zoomLevel: 1
        maximumZoomLevel: 21

        gesture.acceptedGestures: MapGestureArea.PinchGesture | MapGestureArea.PanGesture

        MapQuickItem {
            id: userMarker
            anchorPoint.x: userMarkerIcon.width/2
            anchorPoint.y: userMarkerIcon.height
            sourceItem: userMarkerIcon
        }

        MapQuickItem {
            id: sondeMarker
            anchorPoint.x: sondeMarkerIcon.width/2
            anchorPoint.y: sondeMarkerIcon.height
            sourceItem: sondeMarkerIcon
        }
    }

    GridLayout {
        anchors.top: parent.top
        anchors.right: parent.right
        columns: 1

        Button {
            id: distanceLabel
            text: "Distance X m"
            onClicked: {
                console.log(userMarker.coordinate.distanceTo(sondeMarker.coordinate))
            }
        }
    }

    GridLayout {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        columns: 1

        Button {
            Layout.preferredWidth: 150
            id: mapTypeButton
            text:  "Standard map"
            onClicked: {
                if (mapType === "standard") {
                    osmMap.activeMapType = osmMap.supportedMapTypes[5];
                    mapType = "hiking";
                    mapTypeButton.text = "Hiking map";
                } else if (mapType === "hiking") {
                    osmMap.activeMapType = osmMap.supportedMapTypes[0];
                    mapType = "standard";
                    mapTypeButton.text = "Standard map";
                }
            }
        }

        Button {
            Layout.preferredWidth: 150
            id: trackingModeButton
            text:  "No tracking"

            onClicked: {
                switch(trackingMode) {
                case "none":
                    if (userMarker.coordinate != "") {
                        osmMap.center = userMarker.coordinate;
                        osmMap.zoomLevel = 19;
                    }
                    trackingMode = "user";
                    trackingModeButton.text = "Tracking user";
                    break;
                case "user":
                    if (sondeMarker.coordinate != "") {
                        osmMap.center = sondeMarker.coordinate;
                        osmMap.zoomLevel = 19;
                    }
                    trackingMode = "sonde";
                    trackingModeButton.text = "Tracking sonde";
                    break;
                case "sonde":
                    trackingMode = "none";
                    trackingModeButton.text = "No tracking";
                    break;
                default:
                    break;
                }
            }
        }
    }

    Connections {
        target: ublox;

        onPositionChanged: {
            userMarker.coordinate = QtPositioning.coordinate(lat, lon);
            if (trackingMode === "user") {
                osmMap.center = userMarker.coordinate;
            }
            console.log(userMarker.coordinate.distanceTo(sondeMarker.coordinate))
        }
    }

    Connections {
        target: decoder

        onSondePositionChanged: {
            sondeMarker.coordinate = QtPositioning.coordinate(lat, lon);
            if (trackingMode === "sonde") {
                osmMap.center = sondeMarker.coordinate;
            }
        }
    }
}
