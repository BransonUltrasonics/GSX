import QtQuick 2.0
import Style 1.0
import com.branson.WSrestrictionscreen 1.0

Item
{
    property int layoutSpacing: 10
    property string validatedvalue : ""
    readonly property string qmltextInvalidMsg : qsTr("Invalid value is entered, Reverted to old value")
    property string qmlRecipeName: qsTr("RECIPES")
    property CustomNumPad cNumpad
    id:horndownAdvanced_ID
    Component.onDestruction:
    {
        if(isForceValueChanged && advancedHornDownClicked)
        {
            recipe.setActuatorRecipeValue(oldForceValue,false)
        }
        advancedHornDownClicked = false
        if(fromAdvanced == false)
        {
            horndownObj.retractRequestToBL()
            if(!isLogout)
                horndownObj.startOrStopWeld(1)

            isActSetupActive = false;
            login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
        }
    }

    function updateAdvActSetUpScreen()
    {
        //update Adv Act Setup  values
    }

    Connections{
        target: recipe
        onRefreshScreenPopUpSignal: {
            updateAdvActSetUpScreen()
            if(1 == typeOfChangeIndex)
            {
                messagePopup.open(messagePopup.warning_title,popUpString,messagePopup.ok_button)
                messagePopup.displayPopupClose = 1
                messagePopup.onOkay.connect(function onOkay()
                {
                    recipe.loadRecipeCards()
                    loaderelement.setSource("RecipeListControl.qml")
                    leftMenuData_obj.resetParent(qmlRecipeName)
                    titlebar.text=qmlRecipeName
                    titlebarchild.visible= false
                    mainWindow.leftaction.recipeColor="#0099ff"
                    mainWindow.leftaction.recipeImage="qrc:/images/image/OnclickRecipes.svg"
                    messagePopup.close()
                    messagePopup.onOkay.disconnect(onOkay)
                })
            }
            else if (0 === messagePopup.displayPopupClose)
                messagePopup.open(messagePopup.warning_title,popUpString,messagePopup.without_button)
        }
    }

    Rectangle
    {
        id:id_Actuator_Advanced_Param
        width: parent.width
        height: parent.height
        color: "#ffffff"
        anchors.left: parent.left
        clip: true
        GridView
        {
            id: advancedParamList
            x:50
            y:50
            width: parent.width-50
            height: parent.height-50
            anchors.centerIn: parent.Center
            cellHeight: height/5
            cellWidth: width/3
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            model:setupAdvancedModel
            interactive: false
            property var clickedIndex;
            delegate:WeldModeItem{
                id: advanacedparam
                height:  labelSize
                width:  advancedParamList.cellWidth-80
                paramText:ParameterName
                paramTextLocale:ParameterLocaleName
                paramValue:ParameterValue
                paramUnit:ParameterUnit
                control_mode_mouseArea.onClicked:
                {
                    saveHornDownAdvValue(paramText,paramTextLocale,paramValue,paramUnit,ParameterMinValue,ParameterMaxValue)

                }
            }
        }
        SecondaryButton
        {
            id:id_resetToDefault
            anchors.left: parent.left
            anchors.leftMargin: 2 * layoutSpacing
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2 * layoutSpacing
            text: qsTr("RESET TO DEFAULT")
            onClicked: {
                recipe.hornDownRestoreDefaultAdvParameters()
            }
        }

        PrimaryButton
        {
            id:id_save
            anchors.right: parent.right
            anchors.rightMargin: 2 * layoutSpacing
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2 * layoutSpacing
            text: qsTr("SAVE")
            enabled: false
            visible: false
        }

        SecondaryButton
        {
            id:id_cancel
            anchors.right: id_save.left
            anchors.rightMargin: layoutSpacing
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2 * layoutSpacing
            text: qsTr("CANCEL")
            enabled: false
            visible: false
        }
    }

    /**
    *@breif: To save horndown values
    **/
    function saveHornDownAdvValue(paramText,paramTextLocale,paramValue,paramUnit,ParameterMinValue,ParameterMaxValue)
    {
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)
        cNumpad.open(paramText,paramTextLocale,paramValue,paramUnit)
        cNumpad.minDisplay.visible = true
        cNumpad.maxDisplay.visible = true
        cNumpad.minVal = ParameterMinValue
        cNumpad.maxVal = ParameterMaxValue

        /* On clicking Done button in the numpad */
        cNumpad.onSave.connect(function okay()
        {
            validatedvalue = recipe.keypadlimitation(cNumpad.numpadTitle,cNumpad.keyInputText)
            if(validatedvalue != cNumpad.keyInputText)
            {
                cNumpad.keyInputText = validatedvalue;
                displayQuitPopup(messagePopup.warning_title, qmltextInvalidMsg)
            }
            else
            {
                recipe.setParamaterChangedFlag(1)
                recipe.setStructureValue(cNumpad.numpadTitle,cNumpad.keyInputText,true)
                if(isForceValueChanged)
                    recipe.setActuatorRecipeValue(oldForceValue,true)
                cNumpad.save.disconnect(okay)
                cNumpad.close()
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
