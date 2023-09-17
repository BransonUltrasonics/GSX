import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0

Item {
    property string parameterName: ""
    property string parameterValue: ""
    property string parameterUnit: ""
    readonly property string qmltextSpace :" "

    DropShadow{
        anchors.fill: lastWeldParam1
        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.2
        source: lastWeldParam1
    }
    Rectangle {
        id: lastWeldParam1
        width: parent.width
        height: parent.height
        color: "#e9ecef"
        radius: 3
        border.color: "#e9ecef"
        border.width: 1
        Rectangle {
            id: lastWeldParam1Rect
            width: 9
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0
            radius: 3
            color: qmlSecondaryColor
            border.color: qmlSecondaryColor
            Rectangle{
                id: paramRec
                width: 7
                height: parent.height
                color: qmlSecondaryColor
                border.color: qmlSecondaryColor
                anchors.left: lastWeldParam1Rect.left
                anchors.leftMargin: 3
                radius: 0
            }
        }
        Text {
            id: lastWeldParam1Txt1
            width: 85
            anchors.verticalCenter: parent.verticalCenter
            anchors.left:lastWeldParam1Rect.right
            anchors.leftMargin: 10
            color: "#898D96"
            text: parameterName
            wrapMode: Text.WordWrap
            font.family: Style.semibold.name
            font.pixelSize:  Style.style1
        }
        Text {
            id: lastWeldParam1Txt2
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: lastWeldParam1Txt3.left
            color: qmlSecondaryColor
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.semibold.name
            font.pixelSize:  Style.style1
            text : parameterValue + qmltextSpace
        }
        Text {
            id: lastWeldParam1Txt3
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: lastWeldParam1.right
            anchors.rightMargin: 10
            color: "#898D96"
            horizontalAlignment: Text.AlignHCenter
            font.family: Style.semibold.name
            font.pixelSize:  Style.style1
            text : parameterUnit
        }
    }
}
