import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import QtQuick.Controls.Styles 1.4
import Style 1.0


Item {

    readonly property string qmltextOn:    qsTr("on")
    readonly property string qmltextOff:  qsTr("off")

    id: item1
    property alias myswitch_left_text:myswitch_left_text
    property alias myswitch_right_text:myswitch_right_text
    property alias custom_switch:custom_switch

    Switch {
        id: custom_switch
        x: myswitch_left_text.width
        y: parent.height/2-height/2
        width: (80/116)*parent.width
        height: parent.height
        checked: false

        indicator: Rectangle {
            id:bar

            x: parent.width/2-width/2
            y: parent.height / 2 - height / 2
            width: Math.sqrt(((50/80)*parent.width)*(parent.height))
            height:  .4*width
            radius: parent.height/2
            color: custom_switch.checked ? "#8068a4eb" : "#ECECEC"
            border.color: custom_switch.checked ? "#8068a4eb" : "#ECECEC"

            Rectangle {
                id:circle
                x: custom_switch.checked ? parent.width - width : 0
                y: parent.height/2-height/2
                width: Math.sqrt(((30/50)*parent.width)*((30/20)*parent.height))
                height:Math.sqrt(((30/50)*parent.width)*((30/20)*parent.height))
                color: custom_switch.checked ? qmlPrimaryColor : "#E9ECEF"
                radius:height
                border.color: custom_switch.checked ? qmlPrimaryColor : "#E9ECEF"
            }
        }
    }

    Text {
        id: myswitch_right_text
        x: custom_switch.x+custom_switch.width
        y: 0.2666666666666667*parent.height//0.2666666666666667 is ratio according to parent height
        width: (60/116)*parent.width
        height: parent.height/2
        text:qmltextOn
        font.capitalization: Font.AllUppercase
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.family: Style.semibold.name
        font.pixelSize:  Style.style2

		color:"#868e96"
    }
    Text {
        id: myswitch_left_text
        x: 0
        y: 0.2666666666666667*parent.height
        width: (60/116)*parent.width
        height: parent.height/2
        text: qmltextOff
        font.capitalization: Font.AllUppercase
        font.family: Style.semibold.name
        font.pixelSize:  Style.style2

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        color:"#868e96"
    }
}
