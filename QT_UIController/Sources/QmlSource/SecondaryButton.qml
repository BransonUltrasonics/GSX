import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import Style 1.0

Button {
    readonly property string qmltextSecondary : qsTr("SECONDARY")

    id: secondaryButton

    text:qmltextSecondary
    property int minWidth: 140
    property int maxHeight: 40
    property int radiusWidth: 3
    property int fontSize: Style.style2
    property color buttonColor: "#ECECEC"
    property color textColor : "#212529"
    property color shadowColor: "#80000000"

    contentItem: Text {
        text: secondaryButton.text
        font{
            family: Style.semibold.name
            pixelSize: fontSize
            capitalization: Font.AllUppercase
        }
        color: textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        id: buttonRect
        implicitWidth: minWidth
        implicitHeight: maxHeight
        color: secondaryButton.buttonColor
        radius: radiusWidth
        layer.enabled: secondaryButton.pressed ? false : true
        layer.effect: DropShadow{
            source: buttonRect
            anchors.fill: buttonRect
            horizontalOffset: secondaryButton.pressed ? 0 : 3
            verticalOffset: secondaryButton.pressed ? 0 : 3
            color: shadowColor
            opacity: 0.2
            samples: 10
        }
    }
}
