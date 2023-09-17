import QtQuick 2.0

import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import QtQuick.Controls.Styles 1.4

Item {
    readonly property string qmltextOff: qsTr("off")
    readonly property string qmltextOn:qsTr("on")
    id: customSwitch
    implicitHeight: 30
    implicitWidth: 60
    property string custFontFamily:""
    property int custFontPixelSize:  Style.style7
    property string custColor:"#ffffff"

    //    property  bool isDone: false
    //    property alias myswitch_left_text:myswitch_left_text
    //    property alias myswitch_right_text:myswitch_right_text
    //    property alias custom_switch:custom_switch

    Switch {
        id: switchItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 60
        height: 30
        checked: false
        indicator: Rectangle
        {
            id:indicatorRect
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height : Math.max(bar.height , circle.height)
            width : Math.max(bar.width , circle.width)
            color: customSwitch.custColor
            Rectangle {
                id:bar
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: 50
                height: 20
                radius: parent.height/2
                color: switchItem.checked ? qmlPrimaryColor : "#898d96"
                border.color: switchItem.checked ? qmlPrimaryColor : "#898d96"
                opacity: 0.5
            }
            Rectangle {
                id:circle

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenterOffset: switchItem.checked ? (parent.width / 2 - width /2) : -(parent.width / 2 - width /2)

                z:2
                //                width: Math.sqrt(((30/50)*parent.width)*((30/20)*parent.height))
                //                height:Math.sqrt(((30/50)*parent.width)*((30/20)*parent.height))
                width: 30
                height : 30
                color: switchItem.checked ? qmlPrimaryColor : "#898d96"
                radius:height
                border.color: switchItem.checked ? qmlPrimaryColor : "#898d96"
                opacity: 1.0
            }
        }
    }

    Text {
        id: myswitch_left_text
        //        x: custom_switch.x-(15/105)*parent.width
        //        y: 0.2666666666666667*parent.height

        anchors.verticalCenter:  switchItem.verticalCenter
        anchors.right : switchItem.left
        width: parent.width * 0.2
        text:qmltextOff
        font.family: custFontFamily
        font.pixelSize: custFontPixelSize
        font.capitalization: Font.AllUppercase
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
    }
    Text {
        id: myswitch_right_text
        //        x: custom_switch.width+(15/105)*parent.width
        //        y: 0.2666666666666667*parent.height
        anchors.verticalCenter:  switchItem.verticalCenter
        anchors.left : switchItem.right

        width: parent.width
        height: parent.height/2
        text:qmltextOn
        font.capitalization: Font.AllUppercase
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter

        font.family: custFontFamily
        font.pixelSize: custFontPixelSize
    }
}
