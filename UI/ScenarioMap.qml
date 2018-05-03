import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.2

Rectangle
{
    id:scenarioMap
    visible: true
    anchors.fill: parent
    border.color: "black"
    border.width: 2

    Plugin {
        id: mapPlugin
        name: "osm" //"mapbox"// , "esri", ...
        // specify plugin parameters if necessary
         PluginParameter { name: "osm.mapping.providersrepository.disabled";value: true}
         PluginParameter { name: "osm.mapping.highdpi_tiles";value: true}
         PluginParameter { name: "osm.mapping.host";value: "https://tiles.basemaps.cartocdn.com/rastertiles/voyager/"}
         PluginParameter { name: "osm.useragent"; value: "Simcenter" }
//         PluginParameter { name: "mapbox.access_token"; value: "pk.eyJ1IjoiZWw3YWRkYWQiLCJhIjoiY2pnaW5ra2xyMDJkaDJwbmI5aWdnN3hoMCJ9.PbM-__DXZNSVuqTYnjIdrQ" }


    }

    Map {
        id:map
        plugin: mapPlugin
        anchors.fill: parent
        width: 800
        height: 800
        zoomLevel: 11
        center: QtPositioning.coordinate(37.9358, -122.3477) // Richmond, CA
        activeMapType: supportedMapTypes[supportedMapTypes.length -1]

        Rectangle
        {
            id: mapBorder
            visible: true
            anchors.fill: parent
            border.color: "black"
            border.width: 2
            color: "transparent"
        }

        SiteMarker
        {
            id:siteMarker
        }

        PointRuptureMarker
        {
            id:ruptureMarker
        }
    }
}
