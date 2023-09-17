import QtQuick 2.0
import Style 1.0
import QtGraphicalEffects 1.0

Item {
    id: alarmNotificationPopupdelegate
    property alias list_MA: listMA
    property alias alarmNotificationPopupdelegate: alarmNotificationPopupdelegate
    property string typeOfError: "Alarm 1"
    property string errorTime: "10:05"
    property string errorDate: "01/15/2018"
    property int resetStatus: 0
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
        Text {
            id: list1Error
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top:parent.top
            anchors.topMargin: 10
            color: "#898D96"
            fontSizeMode: Text.HorizontalFit
            font.pixelSize: Style.style5
            text: typeOfError
            font.family: Style.regular.name
        }
        Text {
            id: list1Time
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            color: "#898D96"
            text: errorTime
            font.pixelSize: Style.style1
            font.family: Style.regular.name
        }
        Text {
            id: list1Date
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.left: list1Time.right
            anchors.leftMargin: 5
            color: "#898D96"
            text: errorDate
            font.pixelSize: Style.style1
            font.family: Style.regular.name
        }
        MouseArea{
            id:listMA
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: "PointingHandCursor"
        }
    }
}
