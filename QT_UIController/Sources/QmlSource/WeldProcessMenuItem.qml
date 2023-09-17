import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0
Item {

    property real rectWidth:480.6
    property alias tab_griditem: tab_griditem
    property string paramText: ""
    property string paramColor: "#ffffff"
    property string paramTextColor: "#000000"
    TabButton {
        id: tab_griditem
        x: 0
        y: 0
        width: parent.width
        height: parent.height        
        font.family: Style.semibold.name
        font.pixelSize:  Style.style2

       contentItem:  Text {
            y: parent.height/2-height/2
            width: parent.width-x
            height: 480
            text: paramText
            anchors.leftMargin: (40/480)*parent.width
            anchors.left: parent.left
            font: tab_griditem.font
            opacity: 1
            color: paramTextColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        leftPadding: 2
        topPadding: 6
        background: Rectangle{
            //border.width: 1
            color:tab_griditem.pressed?paramColor:paramColor

        }

    }
}
