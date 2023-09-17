import QtQuick 2.0
import Style 1.0

Rectangle {
    property int textvalue: 1
    property var  textcolor: "#898D96"
    property int fontvalue: Style.style1
    readonly property int minWidth: 30 * 1.5
    readonly property int minHeight: 30 * 1.5

    id:small1
    width: minWidth
    height: minHeight
    border.color: "#BEC1C3"
    border.width: 1
    Text
    {
        id:no
        text: textvalue
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: Style.regular.name
        font.pixelSize: fontvalue
        color: textcolor
    }
    MouseArea{
        id: small1MA
        width: parent.width
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            steppingNumpopup.onClickAction( textvalue)
        }
    }
}

