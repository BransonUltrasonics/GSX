import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2 as New
import QtQuick.Controls.Styles 1.4
import Style 1.0
Item{
    height:parent.height
    width:parent.width
    property alias checkBox_UserIO: check
    property alias comboBox_userIO: comboBox_userIO

    property alias userIO_Switch: userIO_Switch
    property alias rootOfDelegate: rootOfDelegate
    property string checkBox_UserIOColor:"#E9ECEF"
    property int checkBox_UserIORadius: 3

    Text{
        id:title_Pin
        font.pixelSize: Style.style2
        font.family: Style.semibold.name
        text:Title
        height:20
        color:"#212529"
        verticalAlignment: Text.AlignBottom
        anchors.left:parent.left
        anchors.leftMargin: 20
        anchors.right:parent.right
        anchors.rightMargin: 20
    }
    Rectangle{
        anchors.top:title_Pin.bottom
        anchors.topMargin: 1
        id:rootOfDelegate
        height:parent.height
        width:parent.width
        color:"#ffffff"

        Rectangle {
            id: checkBoxRect
            anchors.left: rootOfDelegate.left
            anchors.leftMargin: 40
            border.color:"#ececec"
            border.width: 2
            anchors.top:parent.top
            anchors.topMargin:  parent.height/2-height/2
            width :30
            height: 30
            layer.enabled: true
            layer.effect: DropShadow
            {
                horizontalOffset:1
                verticalOffset:2
                opacity:.2
                radius:3
                color:"#898d96"
                samples:7
            }

            CheckBox
            {
                id:check
                width:checkBoxRect.width
                height:checkBoxRect.height

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: IsEnabled
                checked:IsChecked&&IsEnabled
                visible: true
                style: CheckBoxStyle{
                    indicator:  Rectangle {
                        id:comborect0
                        implicitWidth: check.width
                        implicitHeight: check.height
                        radius: checkBox_UserIORadius
                        color: checkBox_UserIOColor
                        Image {
                            id:checkMarkImage
                            width:check.width
                            height: check.height
                            source: "qrc:/images/image/checkmark.svg"
                            ColorOverlay{

                                anchors.fill: checkMarkImage
                                source: checkMarkImage
                                color: qmlPrimaryColor
                            }
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            visible: (check.checkedState == Qt.Checked)
                        }
                    }
                }
            }
        }

        //combobox
        CustomCombobox {
            id: comboBox_userIO
            anchors.left:checkBoxRect.right
            anchors.leftMargin: 20
            anchors.right:rbTex0.left
            anchors.rightMargin: 20
            y: parent.height/2-height/2
            comboboxpopupheight: 120
            textRole: "ParamName"
            model:ComboBoxList
            currentIndex: CurrentIndexCombobox
            enabled: IsChecked&&IsEnabled
            opacity:IsChecked==false&&IsEnabled? 0.3 : 1.0
            z:2
        }
        //switch
        Text {
            id: rbTex0
            anchors.right:userIO_Switch.left
            anchors.rightMargin: 10
            y: parent.height/2-height/2
            text: qsTr("0 V")
            wrapMode: Text.WordWrap
            font.capitalization: Font.AllUppercase
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            color: ActiveStatus===false&&IsEnabled?qmlPrimaryColor:"#E9ECEF"
        }
        ToogleBar {
            id: userIO_Switch
            anchors.right:parent.right
            anchors.rightMargin: 40
            y: parent.height/2-height/2
            checked:ActiveStatus&&IsEnabled
            enabled:IsChecked&&IsEnabled
            outerRect.opacity: IsChecked===false&&IsEnabled? 0.3:1.0
            innerRect.opacity: IsChecked===false&&IsEnabled? 0.3:1.0

        }
        Text {
            id: rbTex1
            anchors.right:parent.right
            anchors.rightMargin: 5
            y: parent.height/2-height/2
            text: qsTr("24 V")
            wrapMode: Text.WordWrap
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            color: ActiveStatus===true&&IsEnabled?qmlPrimaryColor:"#E9ECEF"
            font.capitalization: Font.AllUppercase
        }
    }
}
