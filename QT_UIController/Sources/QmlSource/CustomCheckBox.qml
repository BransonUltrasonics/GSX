import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import Style 1.0

Item {
    readonly property string qmltextCheckBox : qsTr("CheckBox")

    property alias checkBoxName: checkBoxText.text
    property alias checkBoxStatus: checkBoxId.checked
    property alias checkBoxTextVisible: checkBoxText.visible

    Text {
        id: checkBoxText
        anchors.left: checkBoxId.right
        anchors.leftMargin: 15
        anchors.topMargin: 20
        anchors.bottomMargin:  20
        font.pixelSize: Style.style3
        font.family: Style.regular.name
        text: qmltextCheckBox
        wrapMode: Text.Wrap
        color: checkBoxId.checked ? qmlPrimaryColor : "#868E96"
    }

    CheckBox{
        id:checkBoxId
        checked: false
        style: CheckBoxStyle{
            indicator:  Rectangle {
                id:comborect
                implicitWidth: 35
                implicitHeight: 35
                radius: 3
                color:"#E9ECEF"
                layer.enabled: true
                layer.effect: DropShadow{
                    anchors.fill: comborect
                    horizontalOffset: 0
                    verticalOffset: 1
                    color: "#60000000"
                    opacity: 0.2
                    source: comborect
                }

                Image {
                    id:checkMarkImage
                    width:parent.width
                    height: parent.height
                    source: "qrc:/images/image/checkmark.svg"
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    visible: checkBoxId.checked==true
                }

                ColorOverlay {
                    anchors.fill: checkMarkImage
                    source: checkMarkImage
                    color: qmlPrimaryColor  // make image like it lays under red glass
                    visible: checkBoxId.checked==true
                }
            }
        }
    }
}
