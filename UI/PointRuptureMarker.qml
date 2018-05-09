import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.2
import QtQml 2.2

MapQuickItem {

    anchorPoint.x: 0
    anchorPoint.y: 0

    sourceItem: Rectangle {
        id:ruptureMarkerRect
        width:40
        height:40
        radius: width/2
        visible: true
        color: "transparent"
        opacity:0.5
        anchors.centerIn: parent
        border.color: "black"
        border.width: 2
        z: 100
        MouseArea
        {
            property bool hovered: false
            anchors.fill: parent
            anchors.centerIn: parent

            ToolTip.visible: hovered

            ToolTip.text:  String(qsTr("Point Source Rupture\nLatitude: %1\nLongitude: %2")).arg(rupture.location.latitude).arg(rupture.location.longitude)
            ToolTip.timeout: 5000


            onEntered: hovered = true
            onExited: hovered = false
            hoverEnabled: true

            drag.target: ruptureMarkerRect
            cursorShape: pressed ? "ClosedHandCursor": "OpenHandCursor";


            onPositionChanged: {

                 if(drag.active)
                 {
                     var newCoordinate = map.toCoordinate(mapToItem(map, mouse.x, mouse.y), false);
                     rupture.location.setLatitude(newCoordinate.latitude)
                     rupture.location.setLongitude(newCoordinate.longitude)
                 }
            }
        }


        Rectangle {
            width:25
            height:width
            radius: width/2
            visible: true
            color: "transparent"
            opacity:1.0
            anchors.centerIn: parent
            border.color: "black"
            border.width: 2
            z: 100
        }

        Rectangle {
                width:10
                height:width
                radius: width/2
                visible: true
                color: "transparent"
                opacity:1.0
                anchors.centerIn: parent
                border.color: "black"
                border.width: 2
                z: 100
        }

    }


    coordinate {
        latitude: rupture.location.latitude
        longitude: rupture.location.longitude
    }

    zoomLevel: 11
    z:100


}
