import QtQuick 2.5
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3
import Style 1.0

Item {
    readonly property string qmltextSave :  qsTr("SAVE")
    readonly property string qmltextCancel :  qsTr("CANCEL")
    readonly property string qmltextResetDefaults : qsTr("RESET TO DEFAULTS")
    readonly property string qmltextSaveSuccess :  qsTr("Run time features saved successfully, Please restart the machine to take effect the changes")
    readonly property string qmltextScreens :  qsTr("Screens")
    readonly property string qmltextWeldMode :  qsTr("Weld Mode")
    readonly property string qmltextCycleModification :  qsTr("Cycle Modifications")
    readonly property string qmltextLogging :  qsTr("Logging")
    readonly property string qmltextStorage :  qsTr("Storage")
    readonly property string qmltextExtraction :  qsTr("Data Extraction")
    readonly property string qmltextReporting :  qsTr("Reporting")

    id: item1

    Rectangle{
        id: authorityLog
        x: (20/1920)*parent.width - 20
        width: parent.width
        height:(850/1080)*parent.height
        color:"#ECECEC"
        clip: true
        Flickable{
            id: featuresTabFlick
            contentHeight: Window.height + 400
            flickableDirection: Flickable.VerticalFlick
            anchors.fill: parent
            clip: true

            Rectangle {
                id: authorityLogRec1
                anchors.fill: parent
                color: "#ECECEC"//"#F8F9FA"
                clip: true

                ScrollBar.vertical: ScrollBar{}
                Column
                {
                    id:featuresColumn
                    width: parent.width - 10
                    height: parent.height
                    anchors.top:parent.top
                    anchors.topMargin: 10
                    spacing: 10
                    Rectangle
                    {
                        width:parent.width - 10
                        height: parent.height/10
                        x:10
                        CustomGroupBox
                        {
                            id:screensGroupBox
                            title:qmltextScreens
                            lineRecWidth : parent.width - 10
                            y:5

                        }
                        GridView
                        {
                            id:screensList
                            width: parent.width - 15
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: screensGroupBox.bottom
                            anchors.topMargin: 10
                            cellHeight: 50
                            cellWidth: screensList.width/3
                            clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            model:screensModel/*ListModel
                            {
                                ListElement{name :"DASH BOARD"}
                                ListElement{name :"Overlay Graph"}
                                ListElement{name :"Trends Graph"}
                            }*/
                            delegate:
                                CustomCheckBox
                            {
                                id:screenmodeldel
                                checkBoxName :checkBoxname// name
                                checkBoxStatus:isCheckBoxSelected
                                onCheckBoxStatusChanged:
                                {
                                    bransonDataObj.updateScreensCheckboxStatus(index,checkBoxStatus)
                                }
                            }
                            ScrollBar.vertical: ScrollBar
                            {
                                parent: screensList
                                anchors.top: screensList.top
                                anchors.bottom: screensList.bottom
                            }
                        }
                    }
                    Rectangle
                    {
                        width:parent.width - 10
                        height: parent.height/5
                        x:10
                        CustomGroupBox
                        {
                            id:weldmodeGroupBox
                            title:qmltextWeldMode
                            lineRecWidth : parent.width - 20
                            y:5
                        }
                        GridView
                        {
                            id:weldModeList
                            width: parent.width - 15
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: weldmodeGroupBox.bottom
                            anchors.topMargin: 10
                            cellHeight: 50
                            cellWidth: screensList.width/3
                            clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            model:weldModesModel
                            delegate:
                                CustomCheckBox
                            {
                                checkBoxName :checkBoxname// name
                                checkBoxStatus:isCheckBoxSelected
                                onCheckBoxStatusChanged:
                                {
                                    bransonDataObj.updateWeldModeCheckboxStatus(index,checkBoxStatus)
                                }

                            }
                        }
                    }
                    Rectangle
                    {
                        width:parent.width - 10
                        height: parent.height/6.5
                        x:10
                        CustomGroupBox
                        {
                            id:cycleModifyGroupBox
                            title:qmltextCycleModification
                            lineRecWidth : parent.width - 20
                            y:5
                        }
                        GridView
                        {
                            id:cyclemODIFYList
                            width: parent.width - 15
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: cycleModifyGroupBox.bottom
                            anchors.topMargin: 10
                            cellHeight: 50
                            cellWidth: screensList.width/3
                            //clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            // flickableDirection: Flickable.AutoFlickDirection
                            model:cycleModifyModel/*ListModel
                            {
                                ListElement{name :"Pre-trigger"}
                                ListElement{name :"After burst"}
                                ListElement{name :"Energy Brake"}
                                ListElement{name :"Amplitude Step"}
                                ListElement{name :"Force sTEP"}
                            }*/
                            delegate:
                                CustomCheckBox
                            {
                                checkBoxName :checkBoxname// name
                                checkBoxStatus:isCheckBoxSelected
                                onCheckBoxStatusChanged:
                                {
                                    bransonDataObj.updateCycleModifyCheckboxStatus(index,checkBoxStatus)
                                }

                            }
                        }
                    }
                    Rectangle
                    {
                        width:parent.width - 10
                        height: parent.height/9.5
                        x:10
                        CustomGroupBox
                        {
                            id:eventLogGroupBox
                            title:qmltextLogging
                            lineRecWidth : parent.width - 20
                            y:5

                        }
                        GridView
                        {
                            id:eventLogList
                            width: parent.width - 15
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: eventLogGroupBox.bottom
                            anchors.topMargin: 10
                            cellHeight: 50
                            cellWidth: screensList.width/3
                            clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            model:loggingModel/*ListModel
                            {
                                ListElement{name :"Event Log"}
                                ListElement{name :"Alarm Log"}
                                ListElement{name :"Weld Data Log"}
                            }*/
                            delegate:
                                CustomCheckBox
                            {
                                checkBoxName :checkBoxname// name
                                checkBoxStatus:isCheckBoxSelected
                                onCheckBoxStatusChanged:
                                {
                                    bransonDataObj.updateLoggingCheckboxStatus(index,checkBoxStatus)
                                }

                            }
                        }
                    }
//                    Rectangle
//                    {
//                        width:parent.width - 10
//                        height: parent.height/10
//                        x:10
//                        CustomGroupBox
//                        {
//                            id:dataStorageGroupBox
//                            title:qmltextStorage
//                            lineRecWidth : parent.width - 20
//                            y:5

//                        }
//                        GridView
//                        {
//                            id:dataStorageList
//                            width: parent.width - 15
//                            height: parent.height
//                            anchors.left: parent.left
//                            anchors.leftMargin: 10
//                            anchors.top: dataStorageGroupBox.bottom
//                            anchors.topMargin: 10
//                            cellHeight: 50
//                            cellWidth: screensList.width/3
//                            clip: true
//                            boundsBehavior: Flickable.StopAtBounds
//                            flickableDirection: Flickable.AutoFlickDirection
//                            model:storageModel/*ListModel
//                            {
//                                ListElement{name :"External Data Storage"}
//                            }*/
//                            delegate:
//                                CustomCheckBox
//                            {
//                                checkBoxName :checkBoxname// name
//                                checkBoxStatus:isCheckBoxSelected
//                                onCheckBoxStatusChanged:
//                                {
//                                    bransonDataObj.updateStorageCheckboxStatus(index,checkBoxStatus)
//                                }

//                            }
//                        }
//                    }
                    Rectangle
                    {
                        width:parent.width - 10
                        height: parent.height/10
                        x:10
                        CustomGroupBox
                        {
                            id:dataExtractionGroupBox
                            title:qmltextExtraction
                            lineRecWidth : parent.width - 20
                            y:5

                        }
                        GridView
                        {
                            id:dataExtractionList
                            width: parent.width - 15
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: dataExtractionGroupBox.bottom
                            anchors.topMargin: 10
                            cellHeight: 50
                            cellWidth: screensList.width/3
                            clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            model:dataExtractionModel/*ListModel
                            {
                                ListElement{name :"USB"}
                            }*/
                            delegate:
                                CustomCheckBox
                            {
                                checkBoxName :checkBoxname// name
                                checkBoxStatus:isCheckBoxSelected
                                onCheckBoxStatusChanged:
                                {
                                    bransonDataObj.updateExtractionCheckboxStatus(index,checkBoxStatus)
                                }

                            }
                        }
                    }
                    Rectangle
                    {
                        width:parent.width - 10
                        height: parent.height/10
                        x:10
                        CustomGroupBox
                        {
                            id:reportingGroupBox
                            title:qmltextReporting
                            lineRecWidth : parent.width - 20
                            y:5
                        }
                        GridView
                        {
                            id:reporttingList
                            width: parent.width - 15
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: reportingGroupBox.bottom
                            anchors.topMargin: 10
                            cellHeight: 50
                            cellWidth: screensList.width/3
                            clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            model:reportingModel/*ListModel
                            {
                                ListElement{name :"CSV"}
                                ListElement{name :"PDF"}
                            }*/
                            delegate:
                                CustomCheckBox
                            {
                                checkBoxName :checkBoxname// name
                                checkBoxStatus:isCheckBoxSelected
                                onCheckBoxStatusChanged:
                                {
                                    bransonDataObj.updateReportingCheckboxStatus(index,checkBoxStatus)
                                }
                            }
                        }
                    }
                    Rectangle
                    {
                        width:parent.width - 10
                        height: parent.height/15
                        x:10
                        GridView
                        {
                            id:extraList
                            width: parent.width - 15
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: parent.top
                            anchors.topMargin: 15
                            cellHeight: 50
                            cellWidth: screensList.width/3
                            clip: true
                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            model:extraParameterModel/*ListModel
                            {
                                ListElement{name :"Smart Convertor"}
                                ListElement{name :"User IO"}
                                ListElement{name :"IO Diagnostics"}
                                ListElement{name :"Teach Mode"}
                            }*/
                            delegate:
                                CustomCheckBox
                            {
                                checkBoxName :checkBoxname
                                checkBoxStatus:isCheckBoxSelected
                                onCheckBoxStatusChanged:
                                {
                                   bransonDataObj.updateExtraParamCheckboxStatus(index,checkBoxStatus)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    SecondaryButton{
        id:defalutbutton
        text:qmltextResetDefaults
        anchors.top: authorityLog.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 20
        onClicked:
        {
            bransonDataObj.resetDefaultsValues()
        }
    }
    RowLayout{
        id:buttonslayout
        width: parent.width/4
        anchors.top: authorityLog.bottom
        anchors.topMargin: 5
        anchors.right:parent.right
        anchors.rightMargin: 50
        spacing: 10
        SecondaryButton {
            id: cancelRect
            text:qmltextCancel
            onClicked: {
                bransonDataObj.cancelCurrentChanges()
            }
        }
        PrimaryButton {
            id: saveRect
            text: qmltextSave
            onClicked: {
                var res = bransonDataObj.saveRunTimeFeatures()
                if(res == 1)
                {
                    recipe.updateStatusText(qmltextSaveSuccess)
                }
                else
                {
                    recipe.updateStatusText(qsTr("Runtime features save failed"))
                }
            }
        }
    }
}
