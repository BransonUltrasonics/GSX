import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import Style 1.0
import QtQuick.Controls.Styles 1.4
Item {
    readonly property string qmltextTestRecipeSaveSuccess: qsTr("Test recipe saved successfully")
    readonly property string qmltextTestRecipeFail: qsTr("Failed to save Test Recipe. Check for network connection or Recipe parameters check")
    readonly property string qmltextCancel: qsTr("CANCEL")
    readonly property string qmltextReset: qsTr("RESET")
    readonly property string qmltextSave: qsTr("SAVE")

    width:parent.width
    height: parent.height
    property bool retVal : false
    property int layoutSpacing: 10
    Rectangle {
        id: testRecipeParamWindow
        Layout.row:1
        Layout.column:0
        Layout.columnSpan:3
        width:parent.width
        height: parent.height * 0.85
        implicitHeight: parent.height * 0.85
        implicitWidth: parent.width
        Layout.alignment: Qt.AlignTop
        color: "#ffffff"

        GridView
        {
            id: grid
            x: 50
            y: 50
            width: parent.width-50
            height: parent.height-50

            cellHeight: height/5
            cellWidth: width/3
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            clip: true
            model: testModeAdvancedData
            delegate: WeldModeItem
            {
                paramText:model.modelData.parameterName
                paramTextLocale:model.modelData.parameterLocaleName
                paramValue:model.modelData.parameterValue
                paramUnit:model.modelData.parameterUnit
                height: labelSize
                width:  grid.cellWidth-80
                control_mode_mouseArea.onClicked:
                {
                    steppingNumPad.parameterText = model.modelData.parameterName
                    steppingNumPad.parameterLocaleText = model.modelData.parameterLocaleName
                    steppingNumPad.unitText = model.modelData.parameterUnit
                    steppingNumPad.keyInputText = model.modelData.parameterValue
                    steppingNumPad.minValue = model.modelData.parameterMinValue
                    steppingNumPad.maxValue = model.modelData.parameterMaxValue
                    steppingNumPad.checkParameter( model.modelData.parameterName )
                    numPadStatus = 1
                    steppingNumPad.dialoguerecipeIndex=index;
                    steppingNumPad.ultrasonicAdvancedFlag=true
                    steppingNumPad.preTrigger = false
                    steppingNumPad.open()
                    enableMainwindowOpacity.start();
                }
            }
            ScrollBar.vertical: ScrollBar
            {
                parent: grid
                anchors.top: grid.top
                anchors.bottom: grid.bottom
            }
        }
    }
    Rectangle {
        id: testRecipeControlWindow
        Layout.row: 2
        Layout.column: 0
        Layout.columnSpan:3
        anchors.top:testRecipeParamWindow.bottom
        anchors.topMargin: 0
        anchors.left: testRecipeParamWindow.left
        anchors.leftMargin: 0
        anchors.right: testRecipeParamWindow.right
        anchors.rightMargin: 0
        implicitHeight: parent.height * 0.15
        implicitWidth: parent.width
        color:"#ffffff"
        visible: true
        PrimaryButton {
            id: saveButton
            text: qsTr("SAVE")
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 2 * layoutSpacing
            onClicked: {

                retVal = ultrasonictest.saveTestRecipeData()

                if(retVal == true)
                {
                    recipe.updateStatusText(qmltextTestRecipeSaveSuccess)
                }
                else
                {
                    recipe.updateStatusText(qmltextTestRecipeFail)
                }
            }
        }

        SecondaryButton {
            id: cancelButton
            text: qmltextCancel
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: saveButton.left
            anchors.rightMargin: layoutSpacing
            onClicked: {
                ultrasonictest.cancelTestRecipeData()
            }
        }

        SecondaryButton {
            id: resetButton
            text: qmltextReset
            visible: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 2 * layoutSpacing
            onClicked:
            {
                ultrasonictest.resetTestRecipeData();
            }
        }
    }
}
