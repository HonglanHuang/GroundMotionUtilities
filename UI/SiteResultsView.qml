import QtQuick 2.4
import QtLocation 5.9
import QtPositioning 5.6
import QtQuick.Controls 2.2
import org.designsafe.ci.simcenter 1.0

MapItemView {
    model: siteResultsModel
    delegate: MapQuickItem
    {
        zoomLevel:11
        visible: (gmApp.mode === GMWidget.Results)
        sourceItem: Rectangle {
            width: 40 * model.display.pgaResult.mean
            height:width
            radius: width/2
            visible: true
            anchors.centerIn: parent
            color: getcolor(model.display.pgaResult.mean)

            border.color: "black"
            border.width: 0.5
            z: 50
            MouseArea
            {
                property bool hovered: false
                anchors.centerIn: parent
                anchors.fill: parent
                ToolTip.text:  String(qsTr("Site %1\nLocation: (%2, %3)\nMean PGA:%4\nSelected Record Id:%5\nScale Factor:%6\nDouble click for details.."))
                                    .arg(index)
                                    .arg(model.display.location.latitude)
                                    .arg(model.display.location.longitude)
                                    .arg(model.display.pgaResult.mean).arg(model.display.recordSelection.recordId)
                                    .arg(model.display.recordSelection.scaleFactor)
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                onEntered: hovered = true
                onExited: hovered = false
                hoverEnabled: true
                onDoubleClicked: {
                    resultsWidget.setResult(index, model.display);
                    resultsWidget.show()
                }
            }

        }

        coordinate
        {
            latitude: model.display.location.latitude
            longitude: model.display.location.longitude
        }
        z:50
        function getcolor(value) {
                if (value < 0.1)
                    return Qt.rgba(0, 1, 1-value/0.1)
                else if(value >= 0.1 && value < 0.25)
                    return Qt.rgba((value-0.1)/0.15, 1, 0)
                else if(value >= 0.25 && value < 0.5)
                    return Qt.rgba(1, (0.5-value)/0.25, 0)
                else
                    return Qt.rgba(Math.max(1.0 - (value-0.5), 0.5), 0, 0)
             }
    }

}
