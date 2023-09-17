import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0
Item {

    property real rectWidth:480.6
    property alias tab_griditem: tab_griditem
    property string paramText: ""
    property string paramLocaleText: ""
    property string paramColor: "#ffffff"
    property string paramTextColor: "#000000"
    property alias tab_griditem_mousearea: tab_griditem_mousearea
    property int textLeftMargin: 60
    Rectangle {
        id: tab_griditem
        width: parent.width
        height: parent.height
        color:tab_griditem.pressed?paramColor:paramColor
        Text
        {
            y: parent.height/2-height/2
            width: parent.width-x
            text: paramLocaleText
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
    }
}
