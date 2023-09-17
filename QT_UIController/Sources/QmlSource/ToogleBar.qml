import QtQuick 2.0
import QtQuick 2.6
import QtQuick.Controls 2.1

Switch {
    property int rectWidth: 50
    property int rectHeight: 20
    property int maxWidth: 30
    property int maxHeight: 30
    property string offColor: "#BEC1C3"
    property string onColor: qmlPrimaryColor
    property string textColor: "#868e96"
    property string barOnColor: qmlSecondaryColor

    property alias innerRect: circle
    property alias outerRect: bar

    id: switchID
    implicitWidth: 60
    implicitHeight: maxHeight
    checked: false
       indicator: Rectangle {
        id:bar
        x: parent.width/2-width/2
        y: parent.height / 2 - height / 2
        width: rectWidth
        height: rectHeight
        radius: parent.height/2
        color: switchID.checked ? barOnColor : textColor
        border.color: switchID.checked ? barOnColor : textColor
        Rectangle {
            id:circle
            x: switchID.checked ? parent.width - width : 0
            y: parent.height/2-height/2
            z:2
            width: maxWidth
            height : maxHeight
            color: switchID.checked ? onColor : offColor
            radius:height
            border.color: switchID.checked ? onColor : offColor
            opacity: 1.0
        }
    }
}
