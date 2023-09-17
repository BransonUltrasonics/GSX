import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0
Item {
    property string typeOfImg: ""
    property string alarmTime: ""
    property string alarmError: ""
    DropShadow{
        anchors.fill: listRect
        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.2
        source: listRect
    }

    Rectangle{
        id:listRect
        width: parent.width
        height: parent.height
        Image {
            id: list1img
            width: parent.width*(7/100)
            height: parent.height*(45/100)
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.width*(3/100)
            source: typeOfImg
        }
        Text {
            id: list1Time
            anchors.right: parent.right
            anchors.rightMargin:  parent.width*(3/100)
            anchors.verticalCenter: parent.verticalCenter
            color: "#898D96"
            text:alarmTime
            font.pixelSize: Style.style1
            font.family: Style.regular.name
        }
        Text {
            id: list1Error
            anchors.left: list1img.right
            anchors.leftMargin: parent.width*(9.55/100)
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#898D96"
            fontSizeMode: Text.HorizontalFit
            font.pixelSize: Style.style5
            text:alarmError
            font.family: Style.regular.name
        }
    }
}
