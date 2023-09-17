import QtQuick 2.0
import Style 1.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2
Item {
    property string parameterName: ""
    property string paramColor: "#ECECEC"
    property string paramTextColor: "#898D96"
    property bool isTextField: false
    DropShadow{
        source: rectangle1
        anchors.fill: rectangle1
        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.2
        radius: 3
    }
    Rectangle{
        id:rectangle1
        width: parent.width
        height: parent.height
        radius: 3
        color:paramColor
        Text {
            id: paramName
            text: parameterName
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: Style.style2
            font.family: Style.semibold.name
            color: paramTextColor
        }
        LineEdit {
            id: userNameTextInput
            width: parent.width
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0// 13
            //anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: Style.style2
            font.family: Style.semibold.name
            visible: false
        }
    }
}
