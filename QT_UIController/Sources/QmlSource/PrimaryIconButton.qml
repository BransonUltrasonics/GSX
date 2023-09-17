import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Style 1.0

Button{
    id: primaryIcon
    property int maxWidth: 40
    property int maxHeight: 40
    property int radiusWidth: 3
    property int fontSize: Style.style2
    property bool operator: false

    property string selectedColor: "#ECECEC"
    property string buttonColor: qmlPrimaryColor
    property string textColor: "#FFFFFF"
    property string shadowColor: "#80000000"
    property alias name: primaryIcon.text

    contentItem: Text {
        text: primaryIcon.text
        color: textColor
        font{
            family: Style.semibold.name
            pixelSize: fontSize
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    background: Rectangle {
        id:primaryRect
        color: buttonColor
        implicitWidth: maxWidth
        implicitHeight: maxHeight
        border.width:  1
        border.color: buttonColor
        radius: radiusWidth
        layer.enabled: primaryIcon.pressed ? false: true
        layer.effect: DropShadow{
            anchors.fill: primaryRect
            horizontalOffset: primaryIcon.pressed ? 0 : 3
            verticalOffset: primaryIcon.pressed ? 0 : 3
            color: shadowColor
            opacity: 0.2
            source: primaryRect
        }
    }
    onClicked: {
        if (operator)
        {
            if(numPadStatus == 1)
            {
                steppingNumpopup.operatorPressed(name)
            }
            else if(numPadStatus == 2)
            {
                suspectReject.operatorPressed(name)
            }
            else if(numPadStatus == 3)
            {
                setupNumpad.operatorPressed(name)
            }
        }
        else
        {
            if(numPadStatus == 1)
            {
                steppingNumpopup.digitPressed(name)
            }
            else if(numPadStatus == 2)
            {
                suspectReject.digitPressed(name)
            }
            else if(numPadStatus == 3)
            {
                setupNumpad.digitPressed(name)
            }
        }
    }
}
