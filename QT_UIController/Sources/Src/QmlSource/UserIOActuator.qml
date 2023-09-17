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
    property string restoreDefaultText: qsTr("RESTORE DEFAULTS")
    property string saveText: qsTr("SAVE")

    Rectangle
    {
        id:mainrec
        width: parent.width
        height: parent.height*0.92

        Flickable
        {
            id:flickable
            contentHeight: Window.height + 100
            anchors.fill: parent
            flickableDirection: Flickable.VerticalFlick
            bottomMargin: Qt.inputMethod.visible ? parent.height* (5/100) : 0
            clip: true
            Rectangle
            {
                anchors.fill: parent
                Column
                {
                    id:machinedetailscolumns
                    width: parent.width
                    height: parent.height/1.3
                    spacing: 10

                    Rectangle {
                        width: parent.width
                        height: parent.height

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
                                height: parent.height*1.1
                                cellHeight: 100
                                cellWidth: parent.width
                                clip:true
                                cacheBuffer: 990
                                boundsBehavior: Flickable.StopAtBounds
                                model: digitalInputParamsAC
                                property bool refreshCombo: false
                                property bool refreshCheckBox: false
                                property bool refreshToggle: false
                                property int currentYPos:0
                                onModelChanged:
                                {
                                    contentY=currentYPos
                                    userIOACDIObj.clearGarbage()
                                    cacheBuffer=450
                                }
                                onMovementEnded:
                                {
                                    currentYPos=contentY
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
                                                userIOACDIObj.updateDigitalInputsModelList(indexOfDelegate, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
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
                                                { userIOACDIObj.onUnchecked(index,comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked);
                                                }
                                                else
                                                {
                                                    userIOACDIObj.onChecking(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
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
                                                userIOACDIObj.updateDigitalInputsModelList(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
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
                                model:digitalOutputParamsAC
                                cacheBuffer: 810
                                property bool refreshCombo: false
                                property bool refreshCheckBox: false
                                property bool refreshToggle: false
                                property int currentYPos:0

                                onModelChanged:
                                {
                                    contentY=currentYPos
                                }
                                onMovementStarted:
                                {
                                    cacheBuffer= 0
                                }
                                onMovementEnded:
                                {
                                    cacheBuffer=810
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
                                                userIOACDOObj.updateDigitalOutputsModelList(indexOfDelegate, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
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
                                                userIOACDOObj.updateDigitalOutputsModelList(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
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
                                                userIOACDOObj.updateDigitalOutputsModelList(index, comboBox_userIO.currentIndex,checkBox_UserIO.checked,userIO_Switch.checked)
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
                Qt.callLater(callBackBtn)
            }

            onPressed: {
                saveButton.buttonColor = Style.grayColor
                saveButton.textColor =  Style.blackColor
            }

            function callBackBtn()
            {
                userIOACObj.setUIAfterSaving()
                userioObj.getACDataFromUI()
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
        SecondaryButton   //the button is gray
        {
            text:restoreDefaultText
            onClicked:
            {
                Qt.callLater(callBackRestoreDefaultBtn)
            }

            function callBackRestoreDefaultBtn()
            {
                userIOACDIObj.setDefaultValuesForDigitalInputParams();
                userIOACDOObj.setDefaultValuesForDigitalOutputParams();
            }

        }
    }



}
