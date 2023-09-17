import QtQuick 2.6
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import Style 1.0

Button {
    readonly property string qmltextPrimary: qsTr("PRIMARY")
    id: primaryButton
    text: qmltextPrimary

    property int minWidth: 140
    property int maxHeight: 40
    property int radiusWidth: 3
    property color buttonColor: qmlPrimaryColor
    property color textColor : "#FFFFFF"
    property color shadowColor: "#80000000"
    property int fontSize: Style.style2

    contentItem: Text {
        text: qsTr(primaryButton.text)
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
        color: buttonColor
        radius: radiusWidth
        layer.enabled: primaryButton.pressed ? false : true
        layer.effect: DropShadow {
            horizontalOffset: primaryButton.pressed ? 0 : 3
            verticalOffset: primaryButton.pressed ? 0 : 3
            color:  shadowColor
            opacity: 0.2
            samples: 10
        }
    }
}

