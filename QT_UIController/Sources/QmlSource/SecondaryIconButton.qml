import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Style 1.0

Button{
    id: secondaryIcon
    property int maxWidth: 40
    property int maxHeight: 40
    property int radiusWidth: 3
    property bool operator: false

    property string selectedColor: "#FFFFFF"
    property string buttonColor: "#ECECEC"
    property string textColor: qmlPrimaryColor
    property string shadowColor: "#80000000"

    contentItem: Text {
        text: secondaryIcon.text
        color: textColor
        font{
            family: Style.semibold.name
            pixelSize: Style.style2
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    background: Rectangle {
        id:secondaryRect
        color: buttonColor
        implicitWidth: maxWidth
        implicitHeight: maxHeight
        border.width:  1
        border.color: buttonColor
        radius: radiusWidth
        layer.enabled: secondaryIcon.pressed ? false :true
        layer.effect: DropShadow{
            anchors.fill: secondaryRect
            horizontalOffset: secondaryIcon.pressed ? 0 : 3
            verticalOffset: secondaryIcon.pressed ? 0 : 3
            color: shadowColor
            opacity: 0.2
            source: secondaryRect
        }
}
    onClicked: {
        if (operator)
        {
            if(numPadStatus == 1)
            {
                steppingNumpopup.operatorPressed(name)
            }
            else
            {
                suspectRecject.operatorPressed(name)
            }
        }
        else
        {
            if(numPadStatus == 1)
            {
                steppingNumpopup.digitPressed(name)
            }
            else
            {
                suspectRecject.digitPressed(name)
            }
        }
    }
}
