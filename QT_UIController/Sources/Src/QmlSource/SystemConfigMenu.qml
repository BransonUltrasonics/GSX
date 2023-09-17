import QtQuick 2.6
import QtQuick.Controls 1.4 as OldControls
import QtQuick.Controls 2.1
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import Style 1.0
import QtQuick.Window 2.0
import com.branson.eventlogging 1.0
import com.branson.menu 1.0

Item {

    id:item1
    readonly property string qmltextPSType:  qsTr("PS Type")
    readonly property string qmltextActuatorType:  qsTr("Actuator Type")
    readonly property string qmltextMemoryFullAction:  qsTr("Memory Full Action")
    readonly property string qmltextAuthorityCheck: qsTr("Authority Check")
    readonly property string qmltextLanguage:  qsTr("Language")
    readonly property string qmltextControlLevel:  qsTr("Control Level")
    readonly property string qmltextPsFreq:  qsTr("PS Frequency")
    readonly property string qmltextUnits:  qsTr("Units")
    readonly property string qmltextStartScreen:  qsTr("Start Screen")
    readonly property string qmltextPSPowerOnOption:  qsTr("PS Power On Option")
    readonly property string qmltextMachineName:  qsTr("Machine Name")
    readonly property string qmltextStrokeLength:  qsTr("Stroke Length")
    readonly property string qmltextPSWatt:  qsTr("PS Watt")
    readonly property string qmltextBarcodeRecall:  qsTr("Barcode recall recipe prefix")
    readonly property string qmltextCancel:  qsTr("CANCEL")
    readonly property string qmltextKey:  qsTr("Key")
    readonly property string qmltextVerify:  qsTr("VERIFY")
    readonly property string qmltextInvalidPsWatt:  qsTr("PS Watt value entered is invalid")
    readonly property string qmltextInvalidStrokeLength:  qsTr("Stroke Length value entered is invalid")
    readonly property string qmltextRecipeRecallShouldNotBeBlank:  qsTr("Recipe recall prefix should not be blank")
    readonly property string qmltextMachineNameShouldNotBeBlank:  qsTr("Machine name should not be blank")
    readonly property string qmltextUnknownError:  qsTr("Unknown Error while Saving")
    readonly property string qmltextMachine:  qsTr("Machine: ")
    readonly property string qmltextSavedSuccess:  qsTr("Saved successfully")
    readonly property string qmltextSave:qsTr("SAVE")
    readonly property string qmlTextAuthorityCheckChanged: "Authority check is modified"
    readonly property string qmltextCommtReqMsg:qsTr("Comment is required")

    property alias text_line_edit_branson: text_line_edit_branson

    property int windowHeight :1080
    property int windowWidth :1920
    property int columnSpace :20
    property int itemPosition: 20
    property int columnHeightLeft: 900
    property int columnHeightRight: 900
    property int gridElementHeight:90
    property int gridElementWidth:830
    property int columnWidth:870
    property int linePositionY: 0
    property real vinternalRectheight:1080-85
    property int comboBoxWidth: 286
    property int comboBoxHeight: 50
    property int sysconfigMainWindowPositionX : 20
    property int btnMargin:10
    property int res:0
    property var userLevel: window.userLevelId
    property bool userLevelCheckForBransonKey: false
    property bool isTextSelected: false
    property bool isAuthCheckChanged: false
    /*Global declaration for Simplified Chinese Language ID*/
    property int simplifiedChineseLanguageID: 5
    property DialogPopup dialogPopup
    Flickable{
        id: verticalFlick
        width: parent.width
        height: parent.height
        flickableDirection: Flickable.VerticalFlick
        interactive: Qt.inputMethod.visible ? true : false
        bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
        clip: true

        DropShadow{
            anchors.fill: rect1
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.2
            source: rect1
        }
        Rectangle{
            id:rect1
            width: parent.width - 30
            height: parent.height*0.7
            color:"#ECECEC"
            Rectangle {
                id: centerRect
                anchors.centerIn: parent
                width: 20
                height: parent.height
                color: "#ffffff"
                Rectangle {
                    id: centerLine
                    anchors.centerIn: parent
                    width: 1
                    height: parent.height
                    color: "#000000"
                }
            }
            GridView
            {
                id: grid_view
                width: parent.width
                height: parent.height-20
                anchors.left:rect1.left
                anchors.leftMargin: 30
                anchors.top:rect1.top
                anchors.topMargin: 10
                cellHeight: 70
                cellWidth: width/2
                flickableDirection: Flickable.AutoFlickDirection
                clip: true
                cacheBuffer :0
                model: systemConfigGeneralParams
                property bool refreshSwitch: false
                delegate: SystemConfigGeneralItem
                {
                    paramText:model.modelData.ParameterName
                    isToggleSwitch:model.modelData.ParameterSwitch
                    isLineEdit:model.modelData.ParameterTextEdit
                    isComboBox:model.modelData.ParameterComboBox
                    height:  grid_view.cellHeight - 10
                    width:  grid_view.cellWidth-60

                    toggleSwitch.onPressedChanged:
                    {
                        grid_view.refreshSwitch = true
                    }
                    toggleSwitch.onCheckedChanged:
                    {
                        if(grid_view.refreshSwitch == true)
                        {
                            grid_view.refreshSwitch = false
                            sysconfig.updateToggleSwitchState(paramText,toggleSwitch.checked)
                        }
                    }

                    combo_box.onPressedChanged:
                    {
                        grid_view.refreshSwitch = true
                    }
                    combo_box.onCurrentIndexChanged:
                    {
                        if(grid_view.refreshSwitch == true)
                        {
                            grid_view.refreshSwitch = false
                            sysconfig.updateComboBoxState(paramText,combo_box.currentIndex)
                            if(paramText == qmltextAuthorityCheck)
                            {
                                isAuthCheckChanged = true
                            }
                        }
                    }
                    text_line_edit.onTextChanged:
                    {
                        if(isTextSelected)
                            sysconfig.updateLineEditState(paramText,text_line_edit.text)
                    }
                    text_line_edit.onFocusChanged:
                    {
                        var isfocus = text_line_edit.focus
                        if(isfocus)
                        {
                            isTextSelected = true
                            verticalFlick.contentY = text_line_edit.height*5 +text_line_edit.y
                        }
                    }
                    Component.onCompleted:
                    {
                        if(isToggleSwitch)
                        {
                            checkStatus=model.modelData.ParameterSwitchState
                        }
                        if(isComboBox)
                        {
                            combo_box.model=paramText==qmltextPSType ?psTypeModel:paramText==qmltextActuatorType ?actuatorTypeModel:paramText==qmltextMemoryFullAction?
                                                                                                                       memoryFullActionModel:paramText==qmltextAuthorityCheck?authorityCheckModel:paramText==qmltextLanguage?
                                                                                                                                                                                   languageModel:paramText==qmltextControlLevel?controlLevelModel:paramText==qmltextUnits?unitsModel:paramText==qmltextStartScreen?startScreenModel:paramText==qmltextPSPowerOnOption?psPowerOnOptionModel:qmlTextEmpty
                            if(paramText == qmltextAuthorityCheck)
                            {
                                combo_box.enabled = isPermissionAllowed(Features.SET_AUTHORITY_CHECK_TO_YES)  ? true : false
                                combo_box.comboboxColor = isPermissionAllowed(Features.SET_AUTHORITY_CHECK_TO_YES)  ? "#ECECEC" : "#757575"
                                combo_box.textColor = isPermissionAllowed(Features.SET_AUTHORITY_CHECK_TO_YES)  ? "#757575" : "#BEC1C3"
                            }

                            if(paramText == qmltextMemoryFullAction)
                            {
                                combo_box.enabled =       isPermissionAllowed(Features.SET_MEMORY_FULL_ACTION_TO_YES) ? true : false
                                combo_box.comboboxColor = isPermissionAllowed(Features.SET_MEMORY_FULL_ACTION_TO_YES) ? "#ECECEC" : "#757575"
                                combo_box.textColor =     isPermissionAllowed(Features.SET_MEMORY_FULL_ACTION_TO_YES) ? "#757575" : "#BEC1C3"
                            }

                            /*To display disabled Languages combobox with default langauge selected (Simplified Chinese)*/
                            if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
                            {
                                combo_box.currentIndex = (paramText === qmltextLanguage) ? simplifiedChineseLanguageID : model.modelData.ComboBoxModelName
                                if(paramText === qmltextLanguage)
                                {
                                    sysconfig.updateComboBoxState(paramText,combo_box.currentIndex)
                                    combo_box.enabled = false
                                    combo_box.dropDownImg.visible = false
                                    combo_box.comboboxColor = "#757575"
                                    combo_box.textColor = "#BEC1C3"
                                }
                            }

                            else
                            {
                                combo_box.currentIndex = model.modelData.ComboBoxModelName
                            }
                        }
                        if(isLineEdit)
                        {
                            text_line_edit.text = model.modelData.ParamTextEditName

                            if(paramText == qmltextMachineName)
                            {
                                text_line_edit.maximumLength=31
                            }
                            else if(paramText==qmltextStrokeLength)
                            {
                                text_line_edit.maximumLength=3
                                text_line_edit.validator= validationCheckerStroke

                            }
                            else if(paramText==qmltextPSWatt)
                            {
                                text_line_edit.validator= validationChecker
                            }
                            else if(paramText==qmltextBarcodeRecall)
                            {
                                param_text.wrapMode = Text.WrapAtWordBoundaryOrAnywhere
                                param_text.width = param_text.width /2
                                text_line_edit.maximumLength = 1
                            }
                            else{}
                        }

                    }
                }
                RegExpValidator
                {
                    id: validationChecker
                    regExp: new RegExp (/^((?:4[0-9][0-9]|[5-9][0-9]{2}|[1-3][0-9]{3}|4000))$/) //RegExp('/^-?[0-9]+(\.[0-9]{1,' + numberOfDecimals + '})?$/') // doesn't work, can enter unlimited amount of decimals. Also when field is empty, I cannot type
                }
                RegExpValidator
                {
                    id: validationCheckerStroke
                    regExp: new RegExp (/^(([0]?\d\d?|1[0-2][0-5]|12[0-5]))$/) //RegExp('/^-?[0-9]+(\.[0-9]{1,' + numberOfDecimals + '})?$/') // doesn't work, can enter unlimited amount of decimals. Also when field is empty, I cannot type
                }
                ScrollBar.vertical: ScrollBar
                {
                    parent: grid_view
                    anchors.top: grid_view.top
                    anchors.bottom: grid_view.bottom
                }
            }
        }
        Rectangle{
            id:rect2
            width: parent.width
            height:0.1*parent.height//saveRect.height
            anchors.top: rect1.bottom
            anchors.topMargin: 10
            PrimaryButton {
                id: saveRect
                y: rect2.height/4
                anchors.right: parent.right
                anchors.rightMargin: saveRect.width/2
                text: qmltextSave
                onClicked: {
                    if(isAuthCheckChanged)
                        createDialogpopup(qmlTextAuthorityCheckChanged)
                    else
                        saveValues()
                }
            }
            SecondaryButton {
                id: cancelRect
                y: saveRect.y
                anchors.right:parent.right
                anchors.rightMargin:saveRect.width+saveRect.width/2+ btnMargin
                text: qmltextCancel
                onClicked: {
                    config.readSysConfigData()
                    sysconfig.createSystemConfigGeneralParamList()
                }
            }
        }
        DropShadow{
            id:keyrecDropshadow
            anchors.fill: keyRec
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.2
            source: keyRec
        }
        Rectangle{
            id:keyRec
            width: parent.width - 30
            height: parent.height/11//parent.height
            color:"#ECECEC"
            anchors.top:rect2.bottom
            SystemConfigMenuItem {
                id: bransonkeyId
                width: parent.width/2.5
                height: parent.height - 20
                x:20
                y:10
                Text{
                    id:param_textid
                    text:qmltextKey
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideMiddle
                    wrapMode: Text.WordWrap
                    font.family: Style.regular.name
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize:  Style.style3
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top:parent.top
                    anchors.topMargin: 5
                }
                LineEdit
                {
                    id: text_line_edit_branson
                    anchors.right: parent.right
                    width: 220
                    height: 40
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    maximumLength: 10
                    echoMode: TextField.Password
                    onTextChanged:
                    {
                        sysconfig.updateLineEditState(qmltextKey,text_line_edit_branson.text)
                    }
                    onFocusChanged:
                    {
                        var isfocus = text_line_edit_branson.focus
                        if(isfocus)
                        {
                            verticalFlick.contentY = text_line_edit_branson.height*11 +text_line_edit_branson.y
                        }
                    }
                }
            }
            PrimaryButton
            {
                text:qmltextVerify
                anchors.left: bransonkeyId.right
                anchors.leftMargin: 30
                anchors.centerIn: parent
                onClicked:
                {
                    checkBransonKeyEdited()
                }
            }
        }

    }
    Component.onCompleted:
    {
        text_line_edit_branson.text=""//sysconfig.getNameOfTextEdit(qmltextKey);
        if((userLevel === 1) || (!sysconfig.getAuthorityCheck()))
        {
            keyRec.visible = true
            keyrecDropshadow.visible = true
            userLevelCheckForBransonKey=true
        }
        else
        {
            keyRec.visible = false
            keyrecDropshadow.visible = false
            userLevelCheckForBransonKey=false
        }
    }


    //Resetting Boolean values and saving temp values in sysconfig C++ variables
    function saveValues()
    {
        res= sysconfig.saveSysConfigData();
        if(res==1)
        {
            saveResponse()
            config.changingUnits()
        }
        else if(res==3)
        {
            recipe.updateStatusText(qmltextRecipeRecallShouldNotBeBlank)
        }
        else if(res==4)
        {
            recipe.updateStatusText(qmltextMachineNameShouldNotBeBlank)
        }
        else
        {
            recipe.updateStatusText(qmltextUnknownError)
        }
    }

    function checkBransonKeyEdited()
    {
        var resp=0;
        var val=text_line_edit_branson.text
        resp=sysconfig.compareBransonKey(val);
        if(resp === 1)
        {
            if(userLevelCheckForBransonKey && val !== qmlTextEmpty && !sysconfig.bransonKeyGlobalFlag )
            {
                //display popup
                disableMainwindowOpacity.start()
                keyVerifyPopup.passwordLineEditText = qmlTextEmpty
                keyVerifyPopup.open()
                keyVerifyPopup.onOkay.connect(function onOkay()
                {
                    resp=sysconfig.compareBransonKey(keyVerifyPopup.passwordLineEditText);
                    if(resp==1)
                    {
                        recipe.updateStatusText(qsTr("Key is matched, unlocking the hidden screens"))
                        sysconfig.bransonKeyGlobalFlag = true
                        authorityOptionModel.updateList(window.userLevelId)
                        sysconfig.updateLineEditState(qmltextKey,keyVerifyPopup.passwordLineEditText)
                    }
                    else
                    {
                        sysconfig.updateLineEditState(qmltextKey,qmlTextEmpty)
                        text_line_edit_branson.text = qmlTextEmpty
                        sysconfig.createSystemConfigGeneralParamList()
                        return
                    }
                    keyVerifyPopup.close()
                    keyVerifyPopup.onOkay.disconnect(onOkay)
                })
                keyVerifyPopup.onCancel.connect(function onCancel()
                {
                    text_line_edit_branson.text = qmlTextEmpty
                    keyVerifyPopup.onCancel.disconnect(onCancel)
                })
            }
        }
        else if(resp == 0)
        {
            recipe.updateStatusText(qsTr("Entered wrong password, Retry again."))
            text_line_edit_branson.text = qmlTextEmpty
        }
        else if(resp == 2)
        {
            recipe.updateStatusText(qsTr("Key is empty"))
        }
    }
    function saveResponse()
    {
        machineName.text = qmltextMachine + sysconfig.getMachineName();
        recipe.updateStatusText(qmltextSavedSuccess)
        //loaderelement.setSource("Configuration.qml",{"width":(400/1920)*window.width,"height":window.height})
    }
    function createDialogpopup(message)
    {
        var comment = qmlTextEmpty
        var component = Qt.createComponent("DialogPopup.qml")
        dialogPopup = component.createObject(mainwindow)
        dialogPopup.open(messagePopup.info_title, message)
        dialogPopup.onSave.connect(function onOkay()
        {
            comment = dialogPopup.quitMessage
            if (comment === qmlTextEmpty)
            {
                recipe.updateStatusText(qmltextCommtReqMsg)
            }
            else
            {
                //Resetting Boolean values and saving temp values in sysconfig C++ variables
                res= sysconfig.saveSysConfigData();
                if(res==1)
                {
                    machineName.text = qmltextMachine + sysconfig.getMachineName();
                    recipe.updateStatusText(qmltextSavedSuccess)
                    sysconfig.setAuthorityCheck()
                    Eventlogger.eventSend(EventLogging.UI_EVENT_AUTHORITY_CHECK_CHANGED,comment);
                    isAuthCheckChanged = false
                    config.changingUnits()
                }
                else if(res==0)
                {
                    recipe.updateStatusText(qmltextInvalidPsWatt)
                }
                else if(res==2)
                {
                    recipe.updateStatusText(qmltextInvalidStrokeLength)
                }
                else if(res==3)
                {
                    recipe.updateStatusText(qmltextRecipeRecallShouldNotBeBlank)
                }
                else if(res==4)
                {
                    recipe.updateStatusText(qmltextMachineNameShouldNotBeBlank)
                }
                else
                {
                    recipe.updateStatusText(qmltextUnknownError)
                }
            }
            dialogPopup.close()
            dialogPopup.onSave.disconnect(onOkay)
            dialogPopup.destroy()
        })
        dialogPopup.onCancel.connect(function onCancel()
        {
            dialogPopup.close()
            dialogPopup.onCancel.disconnect(onCancel)
            dialogPopup.destroy()
        })
    }
}
