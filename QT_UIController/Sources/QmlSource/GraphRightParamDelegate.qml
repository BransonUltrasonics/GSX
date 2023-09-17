import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Style 1.0
import QtGraphicalEffects 1.0

Rectangle
{
    id: outerRect

    property int custSpacing : 10 ;
    property alias comboBox: comboId
    property alias outerRectVisibility: outerRect.visible
    property alias checkBox: check
    property int graphParamIndex:0
    property string graphParamVal: (true == checkBox.checked) ? model.modelData.parameterval : 0
    property string leftSeriesName: ""
    property string rightSeriesName: ""

    width: parent.width*.25
    height: parent.height
    color:"#eeeeee"

    Rectangle
    {
        id:comboboxId

        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left

        width: parent.width*.35
        color:"#eeeeee"

        CustomCombobox
        {
            id:comboId
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            currentIndex:isLeftAxis
            model: ListModel {
                id: _Language
                ListElement { text: qsTr("Left Axis")}
                ListElement { text: qsTr("Right Axis")}
            }
        }
    }

    Rectangle
    {
        id:rect
        //anchors.verticalCenter: parent.verticalCenter
        anchors.left: comboboxId.right
        color:parent.color
        anchors.leftMargin: custSpacing / 2
        width: height
        //height: parent.height
        anchors.top:parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 7
        anchors.bottomMargin: 7

        Rectangle {
            id: checkBoxRect

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            width : parent.width
            height: parent.height

            color: model.modelData.checkBoxcolor

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

                checked:isCheckBoxSelected
                visible: true
                style: CheckBoxStyle{
                    indicator:  Rectangle {
                        id:comborect0
                        implicitWidth: check.width
                        implicitHeight: check.height
                        radius: 3
                        color:model.modelData.checkBoxcolor
                        Image {
                            width:check.width
                            height: check.height
                            source: "qrc:/images/image/checkmark.svg"
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            visible: (check.checkedState === Qt.Checked)
                        }
                    }
                }
            }
        }
    }
    Text {
        id:id_paramName
        anchors.left:rect.right
        anchors.leftMargin: custSpacing / 2

        anchors.verticalCenter: parent.verticalCenter

        height : parent.height - custSpacing

        width: parent.width * 0.3

        font.pixelSize: Style.style1
        font.family: Style.regular.name

        //elide: Text.ElideRight
        wrapMode :Text.WordWrap
        horizontalAlignment: Text.AlignLeft

        verticalAlignment: Text.AlignVCenter

        text: model.modelData.checkBoxname

        color: model.modelData.checkBoxcolor
    }

    Text {
        id:id_paramValue

        anchors.right: parent.right
        anchors.rightMargin: 10

        anchors.verticalCenter: parent.verticalCenter

        color: model.modelData.checkBoxcolor

        height: parent.height - custSpacing

        width: parent.width - (rect.width + comboId.width + id_paramName.width + custSpacing)

        text:graphParamVal + " " + model.modelData.checkBoxunit
        font.pixelSize: Style.style2
        font.family: Style.semibold.name

        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter

        //  elide: Text.ElideLeft
    }
}
