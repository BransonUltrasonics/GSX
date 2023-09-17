import QtQuick 2.6
import QtQuick.Controls 1.4 as OldControls
import QtQuick.Controls 2.1
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import Style 1.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0

Item {
    id:mainItem
    readonly property string qmltextSavedSuccessfullMsg : qsTr("Branson System Configuration parameters saved successfully, Please restart the machine to take effect the changes")
    readonly property string qmltextInvalidPsWatt:  qsTr("PS Power Level value entered is invalid")
    readonly property string qmltextInvalidStrokeLength:  qsTr("Stroke Length value entered is invalid")
    readonly property string qmltextUnknownError:  qsTr("Unknown Error while Saving")
    readonly property string qmltextCancel : qsTr("CANCEL")
    readonly property string qmltextSave : qsTr("SAVE")
    readonly property string qmltextSCHeader : qsTr("Supervisory Controller")
    readonly property string qmltextPCHeader : qsTr("Power Controller")
    readonly property string qmltextACHeader : qsTr("Actuator Controller")
    property string qmltextPsType: qsTr("PS Type")
    property bool isTextSelected: false

    /*Global declararion for ASX default language update*/
    property string qmltextLanguage: qsTr("Language")
    property int simplifiedChineseLanguageID: 5

    Component.onCompleted:
    {
        sysconfig.createBransonSystemConfigList()
    }

    Flickable{
        id: verticalFlick
        width: parent.width
        height: 0.7*parent.height
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
            height: parent.height//parent.height
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

            Text {
                id: scHeaderText
                width: (parent.width/2)-40
                height:20
                anchors.top: rect1.top
                anchors.topMargin:  7
                anchors.left:rect1.left
                anchors.leftMargin: 30
                font.pixelSize: Style.style3
                font.family: Style.regular.name
                text: qmltextSCHeader
                wrapMode: Text.Wrap
                color:  qmlPrimaryColor
            }
            ListView
            {
                id: grid_view
                width: (parent.width/2)-40
                height: parent.height-20
                anchors.left:rect1.left
                anchors.leftMargin: 30
                anchors.top:scHeaderText.bottom
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.AutoFlickDirection
                clip: true
                spacing: 10
                cacheBuffer :0
                model:bransonSCSystemMenuModel
                property bool scRefreshSwitch: false
                delegate: SystemConfigGeneralItem
                {
                    id:delRec
                    width:grid_view.width - 40
                    height:grid_view.height/10
                    paramText:model.modelData.ParameterName
                    isLineEdit:model.modelData.ParameterTextEdit
                    isComboBox:model.modelData.ParameterComboBox
                    isSSDSATA:model.modelData.ssdToggleSwitch
                    property bool refreshSwitch: false
                    toggleSwitchForSATA.onPressedChanged:
                    {
                        grid_view.scRefreshSwitch = true
                    }
                    toggleSwitchForSATA.onCheckedChanged:
                    {
                        if(grid_view.scRefreshSwitch == true)
                        {
                            grid_view.scRefreshSwitch = false
                            if(toggleSwitchForSATA.checked == 0)
                            {
                                text_line_edit.setBackgroundColor("#757575")
                                text_line_edit.enabled = false
                            }
                            else
                            {
                                text_line_edit.setBackgroundColor("#ffffff")
                                text_line_edit.enabled = true
                            }
                            sysconfig.updateToggleSwitchState(paramText, toggleSwitchForSATA.checked)
                        }
                    }
                    combo_box.onPressedChanged:
                    {
                        grid_view.scRefreshSwitch = true
                    }
                    combo_box.onCurrentIndexChanged:
                    {
                        if(grid_view.scRefreshSwitch == true)
                        {
                            grid_view.scRefreshSwitch = false
                            sysconfig.updateComboBoxState(paramText,combo_box.currentIndex)
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
                            verticalFlick.contentY = text_line_edit.height +text_line_edit.y
                        }
                    }
                    Component.onCompleted:
                    {
                        if(isComboBox)
                        {
                            combo_box.model=paramText == qsTr("System Type")?systemTypeModel:""
                            combo_box.currentIndex = model.modelData.ComboBoxModelName
                        }
                        if(isLineEdit)
                        {
                            if((paramText === qsTr("SC Serial Number")) || (paramText === qsTr("Aux Box Serial Number")))
                            {
                                text_line_edit.maximumLength = 11
                            }
                            text_line_edit.text = model.modelData.ParamTextEditName
                        }
                        if(paramText == qsTr("SSD SATA"))
                        {
                            text_line_edit.maximumLength = 31
                            textForSATA.visible = true
                            toggleSwitchForSATA.checked = model.modelData.ParameterSwitchState
                            if(toggleSwitchForSATA.checked == 0)
                            {
                                text_line_edit.setBackgroundColor("#757575")
                                text_line_edit.enabled = false
                            }
                            else
                            {
                                text_line_edit.setBackgroundColor("#ffffff")
                                text_line_edit.enabled = true
                            }
                        }
                    }
                }
            }
            Text {
                id: pcHeaderText
                width: (parent.width/2)-70
                height:20
                anchors.right:rect1.right
                anchors.rightMargin: 30
                anchors.top: rect1.top
                anchors.topMargin:  7
                font.pixelSize: Style.style3
                font.family: Style.regular.name
                text: qmltextPCHeader
                wrapMode: Text.Wrap
                color:  qmlPrimaryColor
            }

            ListView
            {
                id:pclistView
                width: (parent.width/2)-70
                height: (parent.height/2)-10
                anchors.right:rect1.right
                anchors.rightMargin: 30
                anchors.top:pcHeaderText.bottom
                spacing: 10
                model: bransonPCSystemMenuModel
                property bool pcRefreshSwitch: false
                delegate: SystemConfigGeneralItem
                {
                    id:delPCRec
                    width:grid_view.width - 40
                    height:grid_view.height/10
                    paramText:model.modelData.ParameterName
                    isLineEdit:model.modelData.ParameterTextEdit
                    isComboBox:model.modelData.ParameterComboBox
                    combo_box.onPressedChanged:
                    {
                        pclistView.pcRefreshSwitch = true
                    }
                    combo_box.onCurrentIndexChanged:
                    {
                        if(pclistView.pcRefreshSwitch == true)
                        {
                            pclistView.pcRefreshSwitch = false
                            sysconfig.updateComboBoxState(paramText,combo_box.currentIndex)
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
                            verticalFlick.contentY = text_line_edit.height +text_line_edit.y
                        }
                    }
                    Component.onCompleted:
                    {
                        if(isComboBox)
                        {
                            combo_box.model=paramText==qmltextPsType ?psTypeModel:paramText==qsTr("PS Frequency (Hz)")?psFrequencyModel:""
                            combo_box.currentIndex = model.modelData.ComboBoxModelName
                        }
                        if(isLineEdit)
                        {
                            if(paramText === qsTr("PC Serial Number"))
                            {
                                text_line_edit.maximumLength = 11
                            }
                            else if(paramText==qsTr("PS Power Level (W)"))
                            {
                                text_line_edit.validator= validationChecker
                            }
                            text_line_edit.text = model.modelData.ParamTextEditName
                        }
                    }
                    RegExpValidator
                    {
                        id: validationChecker
                        regExp: new RegExp (/^((?:4[0-9][0-9]|[5-9][0-9]{2}|[1-3][0-9]{3}|4000))$/)
                    }
                }

            }

            Text {
                id: acHeaderText
                width: (parent.width/2)-70
                height:20
                anchors.right:rect1.right
                anchors.rightMargin: 30
                anchors.top: pclistView.bottom
                anchors.topMargin:  7
                font.pixelSize: Style.style3
                font.family: Style.regular.name
                text: qmltextACHeader
                wrapMode: Text.Wrap
                color:  qmlPrimaryColor
            }

            ListView
            {
                id:aclistView
                width: (parent.width/2)-70
                height: (parent.height/2)-10
                anchors.right:rect1.right
                anchors.rightMargin: 30
                anchors.top:acHeaderText.bottom
                spacing: 10
                model: bransonACSystemMenuModel
                property bool acRefreshSwitch: false
                delegate: SystemConfigGeneralItem
                {
                    id:delACRec
                    width:grid_view.width - 40
                    height:grid_view.height/10
                    paramText:model.modelData.ParameterName
                    isLineEdit:model.modelData.ParameterTextEdit
                    isComboBox:model.modelData.ParameterComboBox
                    combo_box.onPressedChanged:
                    {
                        aclistView.acRefreshSwitch = true
                    }
                    combo_box.onCurrentIndexChanged:
                    {
                        if(aclistView.acRefreshSwitch == true)
                        {
                            aclistView.acRefreshSwitch = false
                            sysconfig.updateComboBoxState(paramText,combo_box.currentIndex)
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
                        if(isComboBox)
                        {
                            combo_box.model=paramText == qsTr("Actuator Type")?actuatorTypeModel:""
                            combo_box.currentIndex = model.modelData.ComboBoxModelName
                        }
                        if(isLineEdit)
                        {
                            if((index == 0))
                            {
                                text_line_edit.maximumLength = 11
                            }
                            else if(index == 2)
                            {
                                text_line_edit.validator = validationCheckerStroke
                            }

                            text_line_edit.text = model.modelData.ParamTextEditName
                        }
                    }
                    RegExpValidator
                    {
                        id: validationCheckerStroke
                        regExp: new RegExp (/^(([0]?\d\d?|1[0-2][0-5]|12[0-5]))$/) //RegExp('/^-?[0-9]+(\.[0-9]{1,' + numberOfDecimals + '})?$/') // doesn't work, can enter unlimited amount of decimals. Also when field is empty, I cannot type
                    }
                }
            }
        }
    }
    RowLayout{
        id:buttonslayout
        width: parent.width/4
        height:80
        anchors.top: verticalFlick.bottom
        anchors.topMargin: 40
        anchors.right:parent.right
        anchors.rightMargin: 50
        spacing: 10
        SecondaryButton {
            id: cancelRect
            text: qmltextCancel
            onClicked:
            {
                config.readSysConfigData()
                sysconfig.createBransonSystemConfigList()
            }
        }
        PrimaryButton {
            id: saveRect
            text: qmltextSave
            onClicked:
            {
                var res;

                /*Below section deliberately updates the language configuration setting to Simplified Chinese when
                  System Type ASX is selected*/
                 if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
                 {
                     sysconfig.updateComboBoxState(qmltextLanguage,simplifiedChineseLanguageID)
                 }

                res= sysconfig.saveSysConfigData();
                if(res==1)
                {
                    recipe.updateStatusText(qmltextSavedSuccessfullMsg)
                }
                else if(res==0)
                {
                    recipe.updateStatusText(qmltextInvalidPsWatt)
                }
                else if(res==2)
                {
                    recipe.updateStatusText(qmltextInvalidStrokeLength)
                }
                else
                {
                    recipe.updateStatusText(qmltextUnknownError)
                }
            }
        }
    }
}

