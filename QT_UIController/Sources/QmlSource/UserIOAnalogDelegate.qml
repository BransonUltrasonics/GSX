import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2 as New
import QtQuick.Controls.Styles 1.4
import Style 1.0
Item{
    height:60
    width:parent.width
    property alias checkBox_UserIO: check
    property alias comboBox_userIO: comboBox_userIO
    property var currIndex: 0

    DropShadow{
        width: rootOfDelegate.width
        height: rootOfDelegate.height
        x:rootOfDelegate.x
        y:rootOfDelegate.y
        horizontalOffset: 3
        verticalOffset: 3
        color: "#000000"
        opacity: 0.2
        source: rootOfDelegate
    }
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
        height:60
        width:parent.width

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
                checked:model.modelData.IsChecked&&IsEnabled
                visible: true
                style: CheckBoxStyle{
                    indicator:  Rectangle {
                        id:comborect0
                        implicitWidth: check.width
                        implicitHeight: check.height
                        radius: 3

                        color:"#E9ECEF"
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
            anchors.right:parent.right
            anchors.rightMargin: 80
            y: parent.height/2-height/2
            textRole: "ParamName"
            model: ComboBoxList
            comboboxpopupheight: 120
            currentIndex: CurrentIndexCombobox
            enabled:IsChecked&&IsEnabled
            opacity:IsChecked==false&&IsEnabled? 0.3 : 1.0
        }
    }
}
