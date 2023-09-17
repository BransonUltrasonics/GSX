import QtQuick 2.1
import QtQuick.Controls 2.1
import Style 1.0

Item {

    property real rectWidth:400
    property real rectHeight:80
    property alias tab_griditem: tab_griditem
    property alias name: name
    property alias tab_griditem_mousearea: tab_griditem_mousearea
    property string paramText: ""
    property string paramColor: "#ffffff"
    property string paramTextColor: "#212529"
    property var textLeftMargin: 60

    Rectangle {
        id: tab_griditem

        width: parent.width
        height: parent.height
        enabled: model.state
        color:tab_griditem.pressed?paramColor:paramColor
        Text
        {
            id: name
            y: parent.height/2-height/2
            width: parent.width-x
            text: model.name
            anchors.leftMargin: (textLeftMargin/rectWidth)*parent.width
            anchors.left: parent.left
            opacity: enabled ? 1.0 : 1
            color: paramTextColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            font.family: Style.regular.name
            font.pixelSize: Style.style5
        }
        MouseArea
        {
            id:tab_griditem_mousearea
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
        }

        Component.onCompleted: {
            if (!tab_griditem.enabled)
            {
                tab_griditem.color = "#757575"
                name.color = "#BEC1C3"
            }
        }
    }
}
