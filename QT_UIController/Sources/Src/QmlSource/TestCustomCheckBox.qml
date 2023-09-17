import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Item {
    property alias checkBoxName: checkBoxText.text
    property alias checkBoxStatus: graphcheckbox.checked
    property alias checkBoxText: checkBoxText
    property  alias graphcheckbox: graphcheckbox
    property var checkBoxColor : "#E9ECEF"
    property alias checkBoxTextPixelSize: checkBoxText.font.pixelSize
    Text {
        id: checkBoxText
        anchors.left: graphcheckbox.right
        anchors.leftMargin: 15
        anchors.topMargin: 20
        anchors.bottomMargin:  20
        font.pixelSize: 16
        text: qsTr("CheckBox")
        color: graphcheckbox.checked ? qmlPrimaryColor : "#868e96"
    }

    CheckBox{
        id:graphcheckbox

        checked: false
        

        style: CheckBoxStyle{
            indicator:  Rectangle {
                id:comborect0
                implicitWidth: 30
                implicitHeight: 30
                radius: 3
                //border.color: control.activeFocus ? "darkblue" : "gray"
                //border.width: 1
                color:checkBoxColor

                Image {
                    id:checkMarkImage
                    x: comborect0.x
                    y: comborect0.y
                    width:parent.width
                    height: parent.height
                    source: "qrc:/images/image/checkmark.svg"
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    visible: graphcheckbox.checked==true
                }

                ColorOverlay {
                    anchors.fill: checkMarkImage
                    source: checkMarkImage
                    color: "#ffffff"  // make image like it lays under red glass
                    visible: graphcheckbox.checked==true
                }
            }
        }
    }
}
