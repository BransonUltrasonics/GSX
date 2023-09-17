import QtQuick 2.0
import QtQuick.Controls 2.2
import Style 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.2
import com.branson.scstate 1.0

Item {
    readonly property string qmltextChoose :qsTr("Choose Option")
    readonly property string qmltextSelectOutput :qsTr("Select output file type to generate report.")
    readonly property string qmltextDeleteDBRecords: qsTr("Delete DB Records")
    readonly property string qmltextReportNotGenerated:qsTr("Report is not generated")
    readonly property string qmlTextGenerateButton: qsTr("Generate")
    readonly property string qmltextOverrideRecipeMsg : qsTr("This operation will delete both result and graph data. Do you want to continue?")

    readonly property string qmlTextWeldResults: qsTr("Weld Results")
    readonly property string qmlTextGraphData: qsTr("Graph Data")
    readonly property string qmlTextOutput: qsTr("Output")
    readonly property string qmlTextCSV: qsTr("CSV")
    readonly property string qmlTextPDF: qsTr("PDF")

    property var selectedoutputIndex
    property int scStatus: 0
    function stopProgress()
    {
        generateReportProgBar.visible = false
    }

    function sendGenerateReqToSC()
    {
        scStatus = splash.getSCStatus()
        if(scStatus == ScStateEnums.READY || scStatus == ScStateEnums.ALARM)
        {
            titlebarmousearea.enabled = false
            isReportinProgress=true
            generateReportProgBar.visible = true
            sysconfig.generateReport(comboBox_ActuatorType.currentIndex,selectedoutputIndex,deleteDBRecords.checkBoxStatus)
        }
        else
            recipe.updateStatusText(qmltextReportNotGenerated)
    }

    Text
    {
        id:choosetext
        x:100
        y:60
        width:200
        height: 60
        verticalAlignment: Text.AlignVCenter
        text:qmltextChoose+"            :"
        font.pixelSize: Style.style6
        font.family: Style.regular.name
    }
    CustomCombobox {
        id: comboBox_ActuatorType
        width :200
        height: 60
        y:60
        anchors.left: choosetext.right
        anchors.leftMargin: 30
        model: ListModel {
            id: datafilterstypes
            ListElement { text: qsTr("Weld Results") }
            ListElement { text: qsTr("Graph Data") }
        }
    }
    CustomCheckBox
    {
        id: deleteDBRecords
        y:75
        checkBoxStatus: false
        checkBoxName: qmltextDeleteDBRecords
        anchors.left: comboBox_ActuatorType.right
        anchors.leftMargin: 30
        width: 140
        height: 50
    }

    Text
    {
        id:stroragetext
        x:100
        y:60
        width:200
        height: 60
        verticalAlignment: Text.AlignVCenter
        anchors.top:choosetext.bottom
        anchors.topMargin: 50
        text:qmlTextOutput+"                           :"
        font.pixelSize: Style.style6
        font.family: Style.regular.name
    }
    GridView {
        id: storagetype
        width :350
        height: 60
        anchors.top:comboBox_ActuatorType.bottom
        anchors.topMargin: 65
        anchors.left: stroragetext.right
        anchors.leftMargin: 30
        ExclusiveGroup { id: group }
        model: ListModel {
            id: storagekind
            ListElement { name: qsTr("CSV") }
            //ListElement { name: qmlTextPDF }
        }
        delegate: CustomRadioButton
        {
            id:customRadioBtn
            buttonGroup: group
            radiobuttontext:name
            Component.onCompleted:
            {
                storagetype.currentIndex = 0
            }
            onClicked:
            {
                selectedoutputIndex = index
            }
        }
    }
    ProgressBar{
        id:generateReportProgBar
        maximumValue: 5
        width: 400
        height: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: storagetype.bottom
        anchors.topMargin: 65
        visible: false
        value: 5
        indeterminate: true
    }

    PrimaryButton{
        id:generatebutton
        width: 200
        height: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 70
        anchors.right: parent.right
        anchors.rightMargin: 150
        text:qmlTextGenerateButton
        onClicked:
        {
            if((selectedoutputIndex === 0) || (selectedoutputIndex === 1))
            {
                scStatus = splash.getSCStatus()
                if((comboBox_ActuatorType.currentIndex == 0) && (deleteDBRecords.checkBoxStatus == true))
                {
                    messagePopup.open(messagePopup.info_title, qmltextOverrideRecipeMsg, messagePopup.ok_and_cancel_button)
                    messagePopup.onOkay.connect(function onOkay()
                    {
                        sendGenerateReqToSC()
                        messagePopup.close()
                        messagePopup.onOkay.disconnect(onOkay)
                    })
                    messagePopup.onCancel.connect(function onCancel(){
                        messagePopup.close()
                        messagePopup.onCancel.disconnect(onCancel)
                    })
                }
                else
                    sendGenerateReqToSC()
            }
            else
            {
                recipe.updateStatusText(qmltextSelectOutput)
                return;
            }
        }
    }
}
