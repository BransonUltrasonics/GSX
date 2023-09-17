import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0

Item {
    id:delegateproduction
    property string innerRecColor: ""
    property string innerRecBorderColor: ""
    property string paramName: ""
    property string paramValue: ""
    property alias goodText: goodNo
    DropShadow{
        anchors.fill: good
        horizontalOffset: 3
        verticalOffset: 3
        color: "#000000"
        opacity: 0.2
        source: good
    }
    Rectangle {
        id: good
        width: parent.width
        height: parent.height
        color: "#e9ecef"
        radius: 3
        border.color: "#e9ecef"
        Rectangle {
            id: cornerRect
            width: 9
            height: parent.height
            color: innerRecColor
            border.color: innerRecBorderColor
            radius: 3
            anchors.left: good.left
            anchors.leftMargin: 0
            Rectangle{
                id: paramRec
                width: 7
                height: parent.height
                color: innerRecColor
                border.color: innerRecBorderColor
                anchors.left: cornerRect.left
                anchors.leftMargin: 3
                radius: 0
            }
        }
        Text {
            id: goodText
            anchors.left: cornerRect.right
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            horizontalAlignment: Text.AlignHCenter
            color: "#898D96"
            text: paramName
            font.family: Style.regular.name
            font.pixelSize: Style.style5
        }
        Text {
            id: goodNo
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 10
            //font.pixelSize: Style.style9
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Text.AlignHCenter
            color: "#898D96"
            text: paramValue
            font.family: Style.light.name
        }
    }
}
