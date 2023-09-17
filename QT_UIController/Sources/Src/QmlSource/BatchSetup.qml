import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import com.branson.menu 1.0
import QtQuick.Window 2.2
import Style 1.0

Item {
    readonly property string qmltextInvalidValueMsg : qsTr("Invalid value is entered, Reverted to old value")
    readonly property string qmltextBatchCounterResetSuccess: qsTr("Batch counter reset is successful")
    readonly property string qmltextBatchCounterResetFail: qsTr("Failed to reset batch counter")
    readonly property string qmltextInvalidBatchIdMsg : qsTr("Batch Id cannot be empty")
    readonly property string qmltextBatchSetup : qsTr("Batch Setup")
    readonly property string qmltextCountwithAlarms : qsTr("Count with Alarms")
    readonly property string qmltextBatchCount : qsTr("Batch Count")
    readonly property string qmltextBatchId : qsTr("Batch ID")
    readonly property string qmltextDone : qsTr("DONE")
    readonly property string qmltextCancel : qsTr("CANCEL")
    readonly property string qmltextProductionSetup : qsTr("PRODUCTION SETUP")
    readonly property string qmltextResetBatchCounter :qsTr("RESET BATCH COUNTER")

    property bool isBatchCountSwitchUpdated:false
    property bool isCountWithAlarmUpdated:false
    property int rootHeight:995
    property int recipeNamePanelHeight:50
    property int rootPanelsHeight: 945
    property int leftRightMargin : 20
    property int mainPanelY: 80
    property int mainPanelHeight: 720
    property int batchCountLabelWidth: 150
    property int batchCountLabelHeight: 50
    property int leftPanelWidth : 900
    property int leftPanelHeight: 720
    property string temp_BatchCount: ""
    property bool isBatchCountUpdated: false
    property int validateVal
    property int returnValFromSc
    property CustomNumPad cNumpad

    /**
    *@breif: Save the values to the batch setup structure
    **/
    function saveValuesToStructure()
    {
        recipeSetupInfo.setBatchCount(textEditBatchCountValue.text)
        recipeSetupInfo.setBatchSetupEnable(switchBatchSetup.checked)
        recipeSetupInfo.setCountWithAlarmEnable(switchCountWithAlarms.checked)
        if(batchIdText.text == "")
        {
            recipe.updateStatusText(qmltextInvalidBatchIdMsg)
        }
        else
        {
            recipeSetupInfo.setBatchId(batchIdText.text)
            loaderelement.setSource("RecipeProductionSetUp.qml",{"width":1920,"height":995})
            titlebarchildtext.text=qmltextProductionSetup
        }
    }
    Component.onCompleted:
    {
        switchBatchSetup.checked = recipeSetupInfo.getBatchSetupEnable()
        switchCountWithAlarms.checked = recipeSetupInfo.getCountWithAlarmEnable()
        textEditBatchCountValue.text = recipeSetupInfo.getBatchCount()
        batchIdText.text = recipeSetupInfo.getBatchId()
        if(switchBatchSetup.checked == false)
        {
            batchSetupSwitchDisabled()
        }
        else if(switchBatchSetup.checked == true)
        {
            batchSetupSwitchEnabled()
        }
        if(isPermissionAllowed(Features.BATCH_SETUP) == false)
        {
            batchSetupSwitchDisabled()
            switchBatchSetup.enabled = false
        }
        else
        {
            switchBatchSetup.enabled = true
        }
        if(isPermissionAllowed(Features.RESET_BATCH_COUNTER) == true)
        {
            btnResetBatchCount.enabled =  true
        }
        else
        {
            btnResetBatchCount.enabled = false
        }
        if(isPermissionAllowed(Features.RESET_BATCH_COUNTER) == true)
        {
            btnResetBatchCount.buttonColor = qmlPrimaryColor
        }
        else
        {
            btnResetBatchCount.buttonColor = "#757575"
        }
    }

    /**
    *@breif: When batch setup is disabled disable all the corresponding field
    **/
    function batchSetupSwitchDisabled()
    {
        textEditBatchCountValue.enabled = false
        textEditBatchCountValue.setBackgroundColor("#757575")
        textEditBatchCountValue.setTextColor("#BEC1C3")
        batchIdText.enabled = false
        batchIdText.setBackgroundColor("#757575")
        batchIdText.setTextColor("#BEC1C3")
        switchCountWithAlarms.checked = false
        switchCountWithAlarms.enabled = false
    }

    /**
    *@breif: When batch setup is enabled, enable all the corresponding field
    **/
    function batchSetupSwitchEnabled()
    {
        textEditBatchCountValue.enabled = true
        textEditBatchCountValue.setBackgroundColor("#ffffff")
        textEditBatchCountValue.setTextColor(qmlPrimaryColor)
        batchIdText.setBackgroundColor("#ffffff")
        batchIdText.setTextColor(qmlPrimaryColor)
        batchIdText.enabled = true
        switchCountWithAlarms.enabled = true
    }

    Rectangle {
        id: rectangle
        y: (recipeNamePanelHeight/rootHeight)*parent.height
        width: parent.width
        height: (rootPanelsHeight/rootHeight)*parent.height
        color: "#ffffff"
        Flickable{
            id:batchSetupFlick
            width: parent.width
            height: parent.height
            flickableDirection: Flickable.VerticalFlick
            anchors.fill: parent
            interactive: Qt.inputMethod.visible ? true : false
            bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
            clip: true

            DropShadow{
                anchors.fill: mainPanel
                horizontalOffset: 3
                verticalOffset: 3
                color: "#000000"
                opacity: 0.2
                source: mainPanel
            }
            Rectangle {
                id: mainPanel
                y: (mainPanelY/rootPanelsHeight)*parent.height
                width:parent.width
                height: (mainPanelHeight/rootPanelsHeight)*parent.height
                anchors.left: parent.left
                anchors.leftMargin: leftRightMargin
                anchors.right:  parent.right
                anchors.rightMargin:  leftRightMargin
                color:"#80ECECEC"
                border.width: 0

                Text {
                    id: textBatchSetup
                    x:0.1*parent.width
                    y:0.05*parent.width
                    width: (batchCountLabelWidth/leftPanelWidth)*parent.width
                    height: (batchCountLabelHeight/leftPanelHeight)*parent.height
                    text: qmltextBatchSetup
                    color:"#868e96"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                }//Text Batch setup

                Text {
                    id: textCountWithAlarms
                    x:0.1*parent.width
                    y:0.1*parent.width
                    width: (batchCountLabelWidth/leftPanelWidth)*parent.width
                    height: (batchCountLabelHeight/leftPanelHeight)*parent.height
                    text: qmltextCountwithAlarms
                    color:"#868e96"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                }//Text Count with alarms

                Text {
                    id: textBatchCount
                    x:0.1*parent.width
                    y:0.15*parent.width
                    width: (batchCountLabelWidth/leftPanelWidth)*parent.width
                    height: (batchCountLabelHeight/leftPanelHeight)*parent.height
                    text: qmltextBatchCount
                    color:"#868e96"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                }
                Text {
                    id: textBatchId
                    x:0.1*parent.width
                    y:0.20*parent.width
                    width: (batchCountLabelWidth/leftPanelWidth)*parent.width
                    height: (batchCountLabelHeight/leftPanelHeight)*parent.height
                    text: qmltextBatchId
                    color:"#868e96"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                }
                ToogleBar
                {
                    id: switchBatchSetup
                    y: textBatchSetup.y
                    anchors.left:textBatchSetup.right
                    anchors.leftMargin:textBatchSetup.width/4
                    width:(70/900)*parent.width
                    height: textBatchSetup.height
                    checked: false
                    onCheckedChanged:
                    {
                        if(checked == false)
                        {
                            batchSetupSwitchDisabled()
                        }
                        else if(checked == true)
                        {
                            batchSetupSwitchEnabled()
                        }
                    }
                }
                ToogleBar {
                    id: switchCountWithAlarms
                    y: textCountWithAlarms.y
                    anchors.left:textCountWithAlarms.right
                    anchors.leftMargin:textCountWithAlarms.width/4
                    width:(70/900)*parent.width
                    height: textCountWithAlarms.height
                    checked: false
                }
                Rectangle{
                    id:rectBatchCountText
                    y:textBatchCount.y
                    anchors.left:textBatchCount.right
                    anchors.leftMargin:textBatchCount.width/4
                    width: (120/900)*parent.width
                    height: textBatchCount.height
                    border.color: qmlPrimaryColor
                    border.width: 1
                    CustomTextField {
                        id: textEditBatchCountValue
                        width: 140
                        height: 25
                        anchors.fill: parent
                        onClick:
                        {
                            var component = Qt.createComponent("CustomNumPad.qml")
                            cNumpad = component.createObject(window)
                            cNumpad.open(textBatchCount.text, textBatchCount.text, textEditBatchCountValue.text, "")
                            /* On clicking Done button in the numpad */
                            cNumpad.onSave.connect(function okay()
                            {
                                validateVal = parseInt(cNumpad.keyInputText)
                                if((validateVal > 0) && (validateVal <= 100000))
                                {
                                    textEditBatchCountValue.text = cNumpad.keyInputText
                                    cNumpad.close()
                                    cNumpad.onSave.disconnect(okay)
                                    cNumpad.destroy()
                                }
                                else
                                {
                                    cNumpad.keyInputText = textEditBatchCountValue.text
                                    displayQuitPopup(messagePopup.warning_title,qmltextInvalidValueMsg)
                                }
                            })
                            /* On clicking Cancel button in the numpad */
                            cNumpad.onCancel.connect(function cancel()
                            {
                                cNumpad.close()
                                cNumpad.onCancel.disconnect(cancel)
                                cNumpad.destroy()
                            })
                        }
                    }
                }
                LineEdit{
                    id: batchIdText
                    y: textBatchId.y
                    anchors.left: textBatchId.right
                    anchors.leftMargin: textBatchId.width/4
                    width: parent.width*(4/10)
                    height: textBatchId.height
                    maximumLength: 50
                    validator: RegExpValidator {
                        regExp: /^[a-zA-Z0-9_]*$/
                    }
                    onFocusChanged:
                    {
                        var isfocus = batchIdText.focus
                        if(isfocus)
                            batchSetupFlick.contentY = batchIdText.y + batchIdText.height
                    }
                }

                PrimaryButton {
                    id: btnDone
                    anchors.right:mainPanel.right
                    anchors.rightMargin: btnDone.width/2
                    anchors.bottom:  mainPanel.bottom
                    anchors.bottomMargin: btnDone.width/2
                    text: qmltextDone
                    onClicked: {
                        saveValuesToStructure()                        
                    }
                }//Done Button

                SecondaryButton {
                    id: btnCancel
                    anchors.right: btnDone.left
                    anchors.rightMargin:30
                    anchors.bottom:   mainPanel.bottom
                    anchors.bottomMargin: btnDone.width/2
                    text:qmltextCancel
                    onClicked: {
                        loaderelement.setSource("RecipeProductionSetUp.qml",{"width":1920,"height":995})
                        titlebarchildtext.text=qmltextProductionSetup
                    }
                }//Cancel Button

                PrimaryButton {
                    id: btnResetBatchCount
                    anchors.right: btnCancel.left
                    anchors.rightMargin:30
                    anchors.bottom:   mainPanel.bottom
                    anchors.bottomMargin: btnDone.width/2
                    text: qmltextResetBatchCounter
                    enabled: isPermissionAllowed(Features.RESET_BATCH_COUNTER) ? true : false
                    buttonColor:isPermissionAllowed(Features.RESET_BATCH_COUNTER) ? qmlPrimaryColor : "#757575"
                    onClicked: {
                        returnValFromSc = recipeSetupInfo.sendResetBatchCounterReqToSC()
                        if(returnValFromSc == 1)
                        {
                            recipe.updateStatusText(qmltextBatchCounterResetSuccess)
                        }
                        else
                        {
                            recipe.updateStatusText(qmltextBatchCounterResetFail)
                        }
                    }
                }//Reset Batch Count Button
            }
        }
    }
}
