import QtQuick 2.0
import Style 1.0

Rectangle   {
    id:labelBarId
    width: parent.width
    height: 30
    color: qmlPrimaryColor
    property alias labelName: name.text

    Text {
        id: name
        color: "#FFFFFF"
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: Style.style3
        font.family: Style.regular.name
    }
}
