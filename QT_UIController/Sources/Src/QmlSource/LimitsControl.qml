import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.menu 1.0

Item {

    readonly property string qmltextInvalidMsg : qsTr("Invalid value is entered, Reverted to old value")
    readonly property string qmlTextInvalidSetup: qsTr("Setup/Control limits values are outside the Suspect/Reject limits! Modify suitably or Turn OFF")
    readonly property string qmlTextInvalidControl: qsTr("Control limits values are outside the Suspect/Reject limits! Modify suitably or Turn OFF")
    readonly property string qmlTextInvalidParam: qsTr("Parameter Values are outside the Suspect/Reject limits! Modify suitably")
    readonly property string qmltextEneterValidLimitsMsg: qsTr("Please enter valid limits to continue")
    readonly property string qmltextControl : "Control"
    readonly property string qmltextstate : " state"
    readonly property string qmltextGroundDetectCutOff : "Ground Detect Cutoff"
    readonly property string qmlTextdenied: qsTr("Absolute Distance Cutoff selection is denied until part contact is done.")

    property real gridHeight:870
    property real rectWidth:480.6
    property real parentwidth:1350
    property int modelIndex: 0
    property string paramColor: "#ECECEC"
    property string paramTextColor: "#898d96"
    property string paramBoarderColor: qmlPrimaryColor
    property string title: qmltextControl
    property bool refreshSwitch: false
    property int controlModelIndex: 0
    property string validatedvalue : ""
    property int individualCheckStatus
    property bool retVal: false
    property string oldValue: ""
    property double partcontactval: 0
    property CustomNumPad cNumpad

    function setControlParameters(paramText,paramTextLocale,paramValue,paramUnit)
    {
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)
        cNumpad.open(paramText,paramTextLocale,paramValue,paramUnit)
        cNumpad.toggleSwicthRec = true
        individualCheckStatus = recipe.getStructureValueByName(paramText + qmltextstate)
        cNumpad.toggleSwicthCheck = parseInt(individualCheckStatus)
        if(cNumpad.toggleSwicthCheck == true)
        {
            cNumpad.lineeditReadOnly = false
            cNumpad.lineEdit.setBackgroundColor("#ffffff")
        }
        if(cNumpad.toggleSwicthCheck == false)
        {
            cNumpad.lineeditReadOnly = true
            cNumpad.lineEdit.setBackgroundColor("#898D96")
        }
        /* On clicking Done button in the numpad */
        cNumpad.onSave.connect(function okay()
        {
            var retVal = recipe.checkControlLimits(cNumpad.switchChecked,cNumpad.numpadTitle,cNumpad.keyInputText)
            if(!retVal)
            {
                cNumpad.keyInputText = oldValue
                displayQuitPopup(messagePopup.warning_title, qmltextInvalidMsg)
            }
            else
            {
                recipe.setParamaterChangedFlag(1)
                recipe.setStructureValue(cNumpad.numpadTitle + qmltextstate,cNumpad.switchChecked,0)
                recipe.setStructureValue(cNumpad.numpadTitle,cNumpad.keyInputText)
                cNumpad.close()
                cNumpad.onSave.disconnect(okay)
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

    GridView {
        id: grid
        width: parent.width
        height: parent.height - 50
        y:50
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection
        cellHeight: grid.height/5
        cellWidth: grid.width/2
        clip:true
        model: modelControlLimits
        cacheBuffer: 0
        property bool refreshSwitch: false
        delegate: WeldModeItem
        {
            paramText:model.modelData.ParameterName
            paramTextLocale:model.modelData.ParameterLocaleName
            paramValue:model.modelData.ParameterValue
            paramUnit:model.modelData.ParameterUnit
            isSwitch:model.modelData.ParameterSwitch
            checkStatus:model.modelData.ParameterSwitchState
            height:  labelSize
            width:  grid.cellWidth-80
            Component.onCompleted:
            {
                toggleSwitch.enabled = isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX)
            }
            toggleSwitch.onPressedChanged:
            {
                grid.refreshSwitch = true
            }
            toggleSwitch.onCheckedChanged:
            {
                var currentIndex = index
                if(grid.refreshSwitch == true)
                {
                    recipe.setParamaterChangedFlag(1)
                    grid.refreshSwitch = false
                    var bRetVal = 0
                    if(toggleSwitchChecked)
                        bRetVal = recipe.midPointValidationForAllParams(false,false,false)
					
					if(bRetVal == 2)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
                    }
                    else if(bRetVal == 3)
                    {
                        displayQuitPopup(messagePopup.warning_title,qmlTextInvalidSetup)
                    }
                    if(toggleSwitchChecked)
                        recipe.setStructureValue(paramText,1)
                    else
                        recipe.setStructureValue(paramText,0)
                }
            }
            control_mode_mouseArea.onClicked:
            {
                if(isPermissionAllowed(Features.MODIFY_WELD_PARAMS_WITH_MIN_MAX))
                {
                    if((model.modelData.ParameterName != qmltextControl) ||
                            (model.modelData.ParameterName != qmltextGroundDetectCutOff))
                    {
                        var expectedpartcontact = recipe.getStructureValue(203)
                        partcontactval = parseFloat(expectedpartcontact)
                        if((model.modelData.ParameterName == "Absolute Distance Cutoff") &&(partcontactval <= 0) )
                        {
                            displayQuitPopup(messagePopup.warning_title, qmlTextdenied)
                        }
                        else
                        {
                            controlModelIndex = index
                            oldValue = paramValue
                            setControlParameters(paramText,paramTextLocale,paramValue,paramUnit)
                        }
                    }
                }
            }
        }
        ScrollBar.vertical: ScrollBar {
            parent: grid
            anchors.top: grid.top
            anchors.left: grid.right
            anchors.bottom: grid.bottom
        }
    }
}


