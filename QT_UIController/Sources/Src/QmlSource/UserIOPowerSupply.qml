import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2 as New
import QtQuick.Controls.Styles 1.4
import Style 1.0
import QtQuick.Window 2.2

Item {
    property string digitalInputsText : qsTr("DIGITAL INPUTS")
    property string digitalOutputsText :qsTr("DIGITAL OUTPUTS")
    property string analogInputsText : qsTr("ANALOG INPUTS")
    property string analogOutputsText : qsTr("ANALOG OUTPUTS")
    property string restoreDefaultText : qsTr("RESTORE DEFAULTS")
    property string saveText: qsTr("SAVE")

    Rectangle {
        id:mainrec
        width: parent.width
        height: parent.height*0.92

        Flickable{
            id:flickable
            contentHeight: Window.height + 400
            anchors.fill: parent
            flickableDirection: Flickable.VerticalFlick
            clip: true

            Rectangle
            {
                anchors.fill: parent
                Column
                {
                    id:machinedetailscolumns
                    width: parent.width
                    height: parent.height/1.1
                    spacing: 10

                    Rectangle {
                        width: parent.width
                        height: parent.height - 140

                        Rectangle{
                            id:digitalInputmodelid
                            width: parent.width/2
                            height: parent.height
                            clip:true

                            Rectangle{
                                id:digitalInputheadertext
                                width: parent.width
                                height: 35
                                Text{
                                    text: digitalInputsText
                                    font.pixelSize: Style.style1
                                    font.family: Style.semibold.name
                                    color: "#898D96"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            GridView {
                                id:leftTopListView
                                anchors.top: digitalInputheadertext.top
                                anchors.topMargin: 50
                                width: parent.width
                                height: parent.height
                                cellHeight: 100
                                cellWidth: parent.width
                                clip:true
                                cacheBuffer: 990
                                boundsBehavior: Flickable.StopAtBounds
                                model: digitalInputParamsPS
                                property bool refreshCombo: false
                                property bool refreshCheckBox: false
                                property bool refreshToggle: false
                                property int currentYPos: 0

                                onModelChanged:
                                {
                                    contentY=currentYPos
                                    userIOPSDIObj.clearGarbage()
                                }
                                onMovementStarted:
                                {
                                    cacheBuffer = 0
                                }
                                onMovementEnded:
                                {
                                    cacheBuffer = 990
                                    currentYPos = contentY
                                }
                                delegate: Rectangle
                                {
                                    property int indexOfDelegate: index
                                    id:modelbackground
                                    x:0
                                    width: 0.8*leftTopListView.width
                                    height: 90
                                    color: "#ECECEC"
                                    UserIODigitalDelegate
                                    {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        height: 60
                                        comboBox_userIO.onActivated:
                                        {
                                            leftTopListView.refreshCombo=true
                                            if(leftTopListView.refreshCombo==true)
                                            {
                                                leftTopListView.refreshCombo=false
                                                userIOPSDIObj.updateDigitalInputsModelList(indexOfDelegate, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
                                            }
                                        }
                                        checkBox_UserIO.onPressedChanged:
                                        {
                                            leftTopListView.refreshCheckBox=true
                                        }
                                        checkBox_UserIO.onCheckedChanged:
                                        {
                                            if(leftTopListView.refreshCheckBox==true)
                                            {
                                                leftTopListView.refreshCheckBox=false

                                                if(checkBox_UserIO.checked==false)
                                                {
                                                    userIOPSDIObj.onUnchecked(index,comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked);
                                                }
                                                else
                                                {
                                                    userIOPSDIObj.onChecking(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
                                                }
                                            }
                                        }
                                        userIO_Switch.onPressedChanged:
                                        {
                                            leftTopListView.refreshToggle=true
                                        }
                                        userIO_Switch.onToggled:
                                        {
                                            if(leftTopListView.refreshToggle==true)
                                            {
                                                leftTopListView.refreshToggle=false
                                                userIOPSDIObj.updateDigitalInputsModelList(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle{
                            id:digitalOutputmodelid
                            x:digitalInputmodelid.width
                            width: parent.width/2
                            height: parent.height
                            clip:true
                            Rectangle{
                                id:digitalOutputheadertext
                                width: parent.width
                                height: 35
                                Text{
                                    text: digitalOutputsText
                                    font.pixelSize: Style.style1
                                    font.family: Style.semibold.name
                                    color: "#898D96"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            GridView
                            {
                                id:rightTopListView
                                anchors.top: digitalOutputheadertext.top
                                anchors.topMargin: 50
                                width: parent.width
                                height: parent.height
                                cellHeight: 100
                                cellWidth: parent.width
                                clip:true
                                boundsBehavior: Flickable.StopAtBounds
                                model:digitalOutputParamsPS
                                cacheBuffer: 810
                                property bool refreshCombo: false
                                property bool refreshCheckBox: false
                                property bool refreshToggle: false
                                property int currentYPos: 0

                                onModelChanged:
                                {
                                    contentY = currentYPos
                                    userIOPSDOObj.clearGarbage()
                                }
                                onMovementStarted:
                                {
                                    cacheBuffer = 0
                                }
                                onMovementEnded:
                                {
                                    cacheBuffer = 810
                                    currentYPos=contentY
                                }
                                delegate: Rectangle
                                {
                                    property int indexOfDelegate: index
                                    x:0
                                    width: 0.8*rightTopListView.width
                                    height: 90
                                    color: "#ECECEC"
                                    UserIODigitalDelegate{
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        height: 60
                                        comboBox_userIO.onActivated:
                                        {
                                            rightTopListView.refreshCombo=true
                                            if(rightTopListView.refreshCombo==true)
                                            {
                                                rightTopListView.refreshCombo=false
                                                //  userIOPSObj.saveModelData(index, comboBox_userIO.currentIndex)
                                                userIOPSDOObj.updateDigitalOutputsModelList(indexOfDelegate, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
                                            }
                                        }

                                        checkBox_UserIO.onPressedChanged:
                                        {
                                            rightTopListView.refreshCheckBox=true
                                        }
                                        checkBox_UserIO.onCheckedChanged:
                                        {
                                            if(rightTopListView.refreshCheckBox==true)
                                            {
                                                rightTopListView.refreshCheckBox=false
                                                if(checkBox_UserIO.checked==false)
                                                {
                                                    userIOPSDOObj.onUnchecked(index,comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked);
                                                }
                                                else
                                                {
                                                    userIOPSDOObj.onChecking(index,comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked);
                                                }
                                            }
                                        }
                                        userIO_Switch.onPressedChanged:
                                        {
                                            rightTopListView.refreshToggle=true
                                        }
                                        userIO_Switch.onToggled:
                                        {
                                            if(rightTopListView.refreshToggle==true)
                                            {
                                                rightTopListView.refreshToggle=false
                                                userIOPSDOObj.updateDigitalOutputsModelList(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: parent.height-140
                        visible: false
                        Rectangle
                        {
                            id:analogInputmodelid
                            width: parent.width/2
                            height: parent.height/4
                            Rectangle{
                                id:analogInputheadertext
                                width: parent.width
                                height: 35
                                Text{
                                    text: analogInputsText
                                    font.pixelSize: Style.style1
                                    font.family: Style.semibold.name
                                    color: "#898D96"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            GridView
                            {
                                id:leftBottomListView
                                anchors.top: analogInputheadertext.top
                                anchors.topMargin: 50
                                width: parent.width
                                height: parent.height
                                cellHeight: 100
                                cellWidth: parent.width
                                boundsBehavior: Flickable.StopAtBounds
                                clip:true
                                model:analogInputParamsPS
                                cacheBuffer: 180
                                property bool refreshCombo: false
                                property bool refreshCheckBox: false
                                property bool refreshToggle: false
                                property int currentYPos: 0
                                onModelChanged:
                                {
                                    contentY = currentYPos
                                    userIOPSAIObj.clearGarbage()
                                    cacheBuffer = 180
                                }
                                onMovementEnded:
                                {
                                    currentYPos = contentY
                                }
                                delegate: Rectangle
                                {
                                    property int indexOfDelegate: index
                                    x:0
                                    width: 0.8*leftBottomListView.width
                                    height: 90
                                    color: "#ECECEC"
                                    UserIOAnalogDelegate{
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        height: 60
                                        comboBox_userIO.onActivated:
                                        {
                                            leftBottomListView.refreshCombo=true
                                            if(leftBottomListView.refreshCombo==true)
                                            {
                                                leftBottomListView.refreshCombo=false
                                                userIOPSAIObj.updateAnalogInputsModelList(indexOfDelegate, comboBox_userIO.currentIndex,checkBox_UserIO.checked)
                                            }
                                        }

                                        checkBox_UserIO.onPressedChanged:
                                        {
                                            leftBottomListView.refreshCheckBox=true
                                        }
                                        checkBox_UserIO.onCheckedChanged:
                                        {
                                            if(leftBottomListView.refreshCheckBox==true)
                                            {
                                                leftBottomListView.refreshCheckBox=false
                                                if(checkBox_UserIO.checked==false)
                                                { userIOPSAIObj.onUnchecked(index,comboBox_userIO.currentIndex,checkBox_UserIO.checked);
                                                }
                                                else
                                                {
                                                    userIOPSAIObj.onChecking(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked)

                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Rectangle
                        {
                            id:analogOutputmodelid
                            x:analogInputmodelid.width
                            width: parent.width/2
                            height: parent.height/4
                            Rectangle{
                                id:analogOutputheadertext
                                width: parent.width
                                height: 35
                                Text{
                                    text: analogOutputsText
                                    font.pixelSize: Style.style1
                                    font.family: Style.semibold.name
                                    color: "#898D96"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            GridView
                            {
                                id:rightBottomListView
                                anchors.top: analogOutputheadertext.top
                                anchors.topMargin: 50
                                width: parent.width
                                height: parent.height
                                cellHeight: 100
                                cellWidth: parent.width
                                boundsBehavior: Flickable.StopAtBounds
                                clip:true
                                model:analogOutputParamsPS
                                cacheBuffer: 180
                                property bool refreshCombo: false
                                property bool refreshCheckBox: false
                                property bool refreshToggle: false
                                property int currentYPos:0


                                onModelChanged:
                                {
                                    contentY = currentYPos
                                    cacheBuffer = 180
                                }
                                onMovementEnded:
                                {
                                    currentYPos = contentY
                                }
                                delegate: Rectangle
                                {
                                    property int indexOfDelegate: index
                                    x:0
                                    width: 0.8*leftBottomListView.width
                                    height: 90
                                    color: "#ECECEC"
                                    UserIOAnalogDelegate{
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        height: 60
                                        comboBox_userIO.onActivated:
                                        {
                                            rightBottomListView.refreshCombo=true
                                            if(rightBottomListView.refreshCombo==true)
                                            {
                                                rightBottomListView.refreshCombo=false
                                                userIOPSAOObj.updateAnalogOutputsModelList(indexOfDelegate, comboBox_userIO.currentIndex,checkBox_UserIO.checked)
                                            }
                                        }

                                        checkBox_UserIO.onPressedChanged:
                                        {
                                            rightBottomListView.refreshCheckBox=true
                                        }
                                        checkBox_UserIO.onCheckedChanged:
                                        {
                                            if(rightBottomListView.refreshCheckBox==true)
                                            {
                                                rightBottomListView.refreshCheckBox=false

                                                if(checkBox_UserIO.checked==false)
                                                {

                                                    userIOPSAOObj.updateAnalogOutputsModelList(index, 0,checkBox_UserIO.checked)
                                                }
                                                else
                                                {
                                                    userIOPSAOObj.updateAnalogOutputsModelList(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked)

                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    RowLayout {
        id:buttonsLayout
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 50
        spacing: 20
        PrimaryButton
        {
            id:saveButton
            text:saveText
            onClicked:
            {
                saveButton.enabled = false
                Qt.callLater(callBackBtnClick)
            }
            onPressed: {
                saveButton.buttonColor = Style.grayColor
                saveButton.textColor =  Style.blackColor
            }

            function callBackBtnClick()
            {
                userIOPSObj.setUIAfterSaving()
                userioObj.getPSDataFromUI()
                if(userioObj.saveDataToDB())
                {
                    recipe.updateStatusText(qsTr("Data saved successfully"))
                }
                else
                {
                    recipe.updateStatusText(qsTr("Data saving failed"))

                }
                saveButton.buttonColor = qmlPrimaryColor
                saveButton.textColor = Style.whiteColor
                saveButton.enabled = true
            }

        }
        SecondaryButton
        {
            text:restoreDefaultText
            onClicked:
            {
                Qt.callLater(callBackRestoreDefaultBtn)
            }

            function callBackRestoreDefaultBtn()
            {
                userIOPSDIObj.setDefaultValuesForDigitalInputParams();
                userIOPSAIObj.setDefaultValuesForAnalogInputParams();
                userIOPSDOObj.setDefaultValuesForDigitalOutputParams();
                userIOPSAOObj.setDefaultValuesForAnalogOutputParams();
            }
        }
    }
}
