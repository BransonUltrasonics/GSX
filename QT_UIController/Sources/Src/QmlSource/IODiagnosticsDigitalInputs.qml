import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2 as New
import QtQuick.Controls.Styles 1.4
import Style 1.0
Item
{
    height:50
    width:parent.width
    //property alias checkBox_UserIO: check
   // property alias comboBox_userIO: comboBox_userIO

   // property alias userIO_Switch: userIO_Switch

    DropShadow
    {
       anchors.fill:elementDISC

        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.6
        source: elementDISC
    }
    Rectangle
    {
        id:elementDISC
        width:parent.width
        height:parent.height
        Text
        {
            id:title_Pin
            //font.pixelSize: Style.style2
            //font.family: Style.semibold.name
            text:"Title"
            height:20
            color:"#212529"
            verticalAlignment: Text.AlignVCenter
            anchors.left:parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle
        {
//            anchors.top:title_Pin.top
//            anchors.topMargin:0
            id:rootOfDelegate
            height:width

            radius: width/2
            border.width: 1
            anchors.right:parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 170
             anchors.verticalCenter: parent.verticalCenter
//            anchors.right:parent.right
//            anchors.rightMargin: 20
            color:"#ffffff"

            Rectangle
            {
                height:width
                width:.6*parent.width
                radius: width/2
                border.width: 1
                anchors.centerIn: parent
                color:"green"

            }
        }
    }
}
