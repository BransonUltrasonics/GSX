import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Controls 2.1
import Style 1.0

Item
{
    id:temp
    property int rectWidth: 50
    property int rectHeight: 20
    property int maxWidth: 30
    property int maxHeight: 30
    property string offColor: "#BEC1C3"
    property string onColor: qmlPrimaryColor
    property string textColor: "#868e96"
    property string barOnColor: qmlSecondaryColor
    property alias switchID: switchID
    property alias checked: switchID.checked
    property alias leftText: switch_left_text.text
    property alias rightText: switch_right_text.text

    height: maxHeight
    Switch
    {
        id: switchID
        implicitWidth: 60
        implicitHeight: maxHeight
        checked: false
        indicator: Rectangle
        {
            id:bar
            x: parent.width/2-width/2
            y: parent.height / 2 - height / 2
            width: rectWidth
            height: rectHeight
            radius: parent.height/2
            color: textColor//switchID.checked ? barOnColor : textColor
            border.color: textColor// switchID.checked ? barOnColor : textColor

            Rectangle {
                id:circle
                x: switchID.checked ? parent.width - width : 0
                y: parent.height/2-height/2
                z:2
                width: maxWidth
                height : maxHeight
                color: onColor
                radius:height
                border.color: onColor
                opacity: 1.0
            }
        }
    }

    Text
    {
        id: switch_left_text
        anchors.verticalCenter:  switchID.verticalCenter
        anchors.right : switchID.left
        anchors.rightMargin: 10
        width: parent.width * 0.2
        text: ""
        font.family: Style.semibold.name
        font.pixelSize: Style.style2
        font.capitalization: Font.AllUppercase
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        color: "#898d96"
    }

    Text
    {
        id: switch_right_text
        anchors.verticalCenter:  switchID.verticalCenter
        anchors.left : switchID.right
        anchors.leftMargin: 10
        width: parent.width
        height: parent.height/2
        text: ""
        font.capitalization: Font.AllUppercase
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.family: Style.semibold.name
        font.pixelSize: Style.style2
        color: "#898d96"
    }
}
