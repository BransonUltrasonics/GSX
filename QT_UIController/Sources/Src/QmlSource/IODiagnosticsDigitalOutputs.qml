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
        Text {
            id: rbTex0
            anchors.right:userIO_Switch.left
            anchors.rightMargin: 10
            y: parent.height/2-height/2
            width: (61/32)*userIO_Switch.width
            height: userIO_Switch.height*.6
            text: qsTr("0 V")
            wrapMode: Text.WordWrap
            font.capitalization: Font.AllUppercase
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
           // color: ActiveStatus===false&&IsEnabled?qmlPrimaryColor:"#E9ECEF"
        }
        ToogleBar {
            id: userIO_Switch

            anchors.right:rbTex1.left
            anchors.rightMargin: 20
            y: parent.height/2-height/2
            width:20
            height:20
//            checked:ActiveStatus&&IsEnabled
//            enabled:IsChecked&&IsEnabled


        }
        Text {
            id: rbTex1
            anchors.right:parent.right
            anchors.rightMargin: 10
            y: parent.height/2-height/2
            width: (61/32)*userIO_Switch.width
            height: userIO_Switch.height*.6
            text: qsTr("24 V")
            wrapMode: Text.WordWrap
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
          //  color: ActiveStatus===true&&IsEnabled?qmlPrimaryColor:"#E9ECEF"
            font.capitalization: Font.AllUppercase
        }
    }
}
