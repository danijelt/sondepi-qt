// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtLocation 5.12
import QtPositioning 5.12

Page {
    title: qsTr("Map")
    // Tracking mode and map type variables
    property string trackingMode: "none";
    property string mapType: "standard";

    // Red sonde icon
    Image {
        id: sondeMarkerIcon
        source: "qrc:/marker-red.png"
        sourceSize.width:32
        sourceSize.height: 32
    }

    // Yellow user icon
    Image {
        id: userMarkerIcon
        source: "qrc:/marker-yellow.png"
        sourceSize.width:32
        sourceSize.height: 32
    }

    // OpenStreetMap plugin
    Plugin {
        id: mapPlugin
        name: "osm"
    }

    // Map area
    Map {
        id: osmMap
        anchors.fill: parent
        plugin: mapPlugin
        // Center to 0,0 by default, zoom level 1, maximum zoom level 21
        center: QtPositioning.coordinate(0, 0)
        zoomLevel: 1
        maximumZoomLevel: 21

        // Limit gestures to pan and zoom
        gesture.acceptedGestures: MapGestureArea.PinchGesture | MapGestureArea.PanGesture

        // Set anchor points for the icon to bottom center
        MapQuickItem {
            id: sondeMarker
            anchorPoint.x: sondeMarkerIcon.width/2
            anchorPoint.y: sondeMarkerIcon.height
            sourceItem: sondeMarkerIcon
        }

        MapQuickItem {
            id: userMarker
            anchorPoint.x: userMarkerIcon.width/2
            anchorPoint.y: userMarkerIcon.height
            sourceItem: userMarkerIcon
        }
    }

    GridLayout {
        anchors.top: parent.top
        anchors.right: parent.right
        columns: 1

        // Distance between the sonde and the user
        Button {
            id: distanceLabel
            text: {
                return (userMarker.coordinate.distanceTo(sondeMarker.coordinate)).toFixed(1) + " m"
            }
        }
    }

    GridLayout {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        columns: 1

        // Switch between standard OSM and hiking (topographical) map
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

        // Tracking mode: none, sonde in the center, user in the center
        // Zoom to level 18 when enabling tracking
        Button {
            Layout.preferredWidth: 150
            id: trackingModeButton
            text:  "No tracking"

            onClicked: {
                switch(trackingMode) {
                case "none":
                    if (userMarker.coordinate != "") {
                        osmMap.center = userMarker.coordinate;
                        osmMap.zoomLevel = 18;
                    }
                    osmMap.bearing = 0;
                    trackingMode = "user";
                    trackingModeButton.text = "Tracking user";
                    break;
                case "user":
                    if (sondeMarker.coordinate != "") {
                        osmMap.center = sondeMarker.coordinate;
                        osmMap.zoomLevel = 18;
                    }
                    osmMap.bearing = 0;
                    trackingMode = "sonde";
                    trackingModeButton.text = "Tracking sonde";
                    break;
                case "sonde":
                    osmMap.bearing = 0;
                    trackingMode = "none";
                    trackingModeButton.text = "No tracking";
                    break;
                default:
                    break;
                }
            }
        }
    }

    // Connect to the decoder object, move sonde icon to the coordinates received from the decoder
    Connections {
        target: decoder

        onSondePositionChanged: {
            sondeMarker.coordinate = QtPositioning.coordinate(lat, lon);
            // Center the map to the new coordinates if sonde tracking is enabled
            if (trackingMode === "sonde") {
                osmMap.center = sondeMarker.coordinate;
            }
            // Calculate distance between sonde and the user
            distanceLabel.text = (userMarker.coordinate.distanceTo(sondeMarker.coordinate)).toFixed(1) + " m"
        }
    }

    // Connect to the ublox object, move user icon to the coordinates received from the decoder
    Connections {
        target: ublox;

        onPositionChanged: {
            userMarker.coordinate = QtPositioning.coordinate(lat, lon);
            // Center the map to the new coordinates if user tracking is enabled
            // Rotate the map to the direction the user is moving in
            if (trackingMode === "user") {
                osmMap.center = userMarker.coordinate;
                if (!isNaN(dir)) {
                    osmMap.bearing = dir;
                }
            }
            // Calculate distance between sonde and the user
            distanceLabel.text = (userMarker.coordinate.distanceTo(sondeMarker.coordinate)).toFixed(1) + " m"
        }
    }
}
