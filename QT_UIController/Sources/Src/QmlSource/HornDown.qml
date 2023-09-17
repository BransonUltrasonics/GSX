import QtQuick 2.0
import Style 1.0
import QtQuick.Controls 2.1
import com.branson.WSrestrictionscreen 1.0

Item {
    id:root
    width:  loaderelement.width
    height: loaderelement.height

    property bool radioButtonSelection: true
    property string qmlRecipeName: qsTr("RECIPES")
    property string qmlTextPosition: qsTr("Ready Position")
    property string qmlTextHornClampDown: qsTr("Horn Clamp")
    readonly property string qmltextEneterValidLimitsMsg : qsTr("Please enter valid limits to continue")
    property string qmlTextStatus1: qsTr("Press start switch for find Part Contact")
    property string qmlTextStatus2: qsTr("Press start switch for Jog")
    property string qmlTextStatus3: qsTr("Press start switch for Horn Down")
    property string qmlTextStatus4: qsTr("Press start switch for Jog Up")
    property string qmlTextStatus5: qsTr("Press start switch for Jog Down")
    property string qmlTextStatus6: qsTr("Do find Part Contact operation first")
    property string qmlTextStatus7: qsTr("Ready offset can not be greater than EPC")
    property string qmlTextsetmyReadyPosition: qsTr("Set My Ready Position")
    property string qmlTextRetract: qsTr("    Retract   ")
    property string qmlTextAdvancedFeature: qsTr("    Advanced Feature   ")
    property string qmlTextActualForce: qsTr("Actual Force              = ")
    property string qmlTextpreContactVelocity: qsTr("Part Contact Velocity  = ")
    property string qmlTextprepartContact: qsTr("Pre-Part Contact        = ")
    property string qmlTextAbsolutePosition : qsTr("Absolute Position      = ")
    property string qmlTextActualForceUnit: " N"
    property string qmlTextpreContactVelocityUnit: " mm/s"
    property string qmlTextprepartContactUnit: " mm"
    property string qmlTextAbsolutePositionUnit : " mm"
    property string qmlTextHornDown: qsTr("Horn Down")
    property string qmlTextFindPartContact: qsTr("Find Part Contact")
    property string qmlTextJog: qsTr("Jog")
    property string qmlTextFixture: qsTr("Stroke Len")
    property string qmlTextMP: qsTr("MP")
    property string qmlTextEPC:qsTr("EPC")
    property string qmlTextPPC: qsTr("PPC")
    property string qmlTextWP: qsTr("WP")
    property string qmlTextReady: qsTr("Ready")
    property string qmlTextHome: qsTr("Home")
    property string qmlTextdistanceUnit: " mm"
    property real homepositionvalue: 0
    property real positionrefrence: 0
    property real readyPositionValue: strokeLength*(2)
    property real wpPositionValue: strokeLength*(2.5)
    property real ppcPositionValue: strokeLength*(3)
    property real epcPositionValue: strokeLength*(3.5)
    property real mpPositionValue: strokeLength*(4)
    property real fixturePositionValue: strokeLenthInPixel
    property real hornpointerPosition: 0
    property real hornPointerDistance: 0
    property real strokeLength: (id_StatusofHorn.height/125)*1.25
    property real strokeLenthInPixel: id_StatusofHorn.height
    property int activeRecipeEditFlag: 0
    property bool radioButton1: true
    property bool radioButton2: false
    property bool radioButton3: false
    property bool findpartcontactstatus: false
    property bool readypositionswitch:false
    property bool horndownclampswitch:false
    property bool horndownswitchpressed: false
    property bool jogswitchpressed: false
    property bool setflagUp: true
    property bool setflagDown: false
    property string qmlAbsolutePositionValue: "0"
    property string qmlPrePartContactValue: "0"
    property string qmlPartContactVelocityValue: "0"
    property string qmlActualForceValue: "0"
    property var clickedIndex;
    property int imageWidth: 35
    property int imageHeight: 35
    property string updatedparamvalue: ""
    property int  stubreadypos: 1250
    property double partcontactval: 0
    property int unitType
    property int responseType: 0
    property CustomNumPad cNumpad
    property bool refreshSwitch: false

    property int dwReadyTextPosition: 0
    property int dwPPCTextPosition: 0
    property int dwWPTextPosition: 0
    property int dwEPCTextPosition: 0
    property int dwMPTextPosition: 0
    property bool bIsTopPossible : true
    property bool isFirstTimePageLoad: true

    readonly property string qmltextExpectedContactWindow:   qsTr("EXPECTED CONTACT WINDOW")

    /** Units **/
    readonly property string qmltextUnitlbs :  " lbs"
    readonly property string qmltextUnitinPerSec :  " in/s"
    readonly property string qmltextUnitIn :  " in"

    readonly property string qmltextColon :  " : "
    readonly property string qmltextStar :  "*"

    property int rightMarginSpace: 90
    property int leftMarginSpace: 90
    property var positionsList: [7]
    property var positionsListSorted: [7]
    property int paramCount: 7
    /**
    *@breif: To set actuator parameters
    **/
    function setActuatorParameter(paramText,paramTextLocale,paramValue,paramUnit,ParameterMinValue,ParameterMaxValue)
    {
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)
        /** Opens the numpad **/
        cNumpad.open(paramText, paramTextLocale,paramValue,paramUnit)
        cNumpad.minDisplay.visible = true
        cNumpad.maxDisplay.visible = true
        cNumpad.minVal = ParameterMinValue
        cNumpad.maxVal = ParameterMaxValue

        /** When save button is clicked **/
        cNumpad.onSave.connect(function okay()
        {
            recipe.setParamaterChangedFlag(1)
            var temp =  cNumpad.keyInputText
            updatedparamvalue = recipe.keypadlimitation(paramText,cNumpad.keyInputText)
            if(temp == updatedparamvalue)
            {
                if(actuatorParamList_Id.clickedIndex == 0)
                {
                    isForceValueChanged = true
                    oldForceValue = updatedparamvalue
                    // recipe.setStructureValue("weld force",updatedparamvalue,true)
                    recipe.setActuatorRecipeValue(updatedparamvalue,true)
                }
                else if(actuatorParamList_Id.clickedIndex == 1)
                {
                    recipe.setStructureValue("ready offset",updatedparamvalue,true)
                    //id_ReadyPosition_switch.checked=true
                    horndownObj.RefreshAllData()
                }
                else if(actuatorParamList_Id.clickedIndex == 2)
                    recipe.setStructureValue("expected part contact", updatedparamvalue, true)
                else if(actuatorParamList_Id.clickedIndex == 3)
                    recipe.setStructureValue("expected contact offset",updatedparamvalue,true)
                horndownObj.updateActuatorList(actuatorParamList_Id.clickedIndex)
                horndownObj.updateHornPositionValues()
                cNumpad.close()
                cNumpad.onSave.disconnect(okay)
            }
            else
            {
                cNumpad.keyInputText = updatedparamvalue
                displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
            }
        })
        /** When cancel button is clicked **/

        cNumpad.onCancel.connect(function cancel()
        {
            cNumpad.close()
            cNumpad.onCancel.disconnect(cancel)
            cNumpad.destroy()
        })

    }

    /**
    *@breif: To set actual actuator data
    **/
    function setActualActuatordata()
    {
        qmlAbsolutePositionValue = horndownObj.absolutePosition
        qmlPrePartContactValue = horndownObj.actualPPC
        qmlPartContactVelocityValue = horndownObj.partContactVelocity
        qmlActualForceValue = horndownObj.actualForce
    }

    function setHornpostionValue()
    {
        var dwTextWidth = 10

        var dwHomePosition = homepositionvalue        
        var dwEndPosition = fixturePositionValue

        var dwReadyPosition         = 0
        var dwReadyPositionStart    = 0
        var dwReadyPositionEnd      = 0

        var dwPPCPosition           = 0
        var dwPPCPositionStart      = 0
        var dwPPCPositionEnd        = 0

        var dwWPPosition            = 0
        var dwWPPositionStart       = 0
        var dwWPPositionEnd         = 0

        var dwEPCPosition           = 0
        var dwEPCPositionStart      = 0
        var dwEPCPositionEnd        = 0

        var dwMPPosition            = 0
        var dwMPPositionStart       = 0
        var dwMPPositionEnd         = 0

        if(isFirstTimePageLoad)
        {
            dwReadyPosition         = horndownObj.toPixel(horndownObj.readyposition)
            dwReadyPositionStart    = horndownObj.toPixel(horndownObj.readyposition) - dwTextWidth
            dwReadyPositionEnd      = horndownObj.toPixel(horndownObj.readyposition) + dwTextWidth

            dwPPCPosition           = horndownObj.toPixel(horndownObj.ppcPosition)
            dwPPCPositionStart      = horndownObj.toPixel(horndownObj.ppcPosition) - dwTextWidth
            dwPPCPositionEnd        = horndownObj.toPixel(horndownObj.ppcPosition) + dwTextWidth

            dwWPPosition            = horndownObj.toPixel(horndownObj.wpPosition)
            dwWPPositionStart       = horndownObj.toPixel(horndownObj.wpPosition) - dwTextWidth
            dwWPPositionEnd         = horndownObj.toPixel(horndownObj.wpPosition) + dwTextWidth

            dwEPCPosition           = horndownObj.toPixel(horndownObj.epcPosition)
            dwEPCPositionStart      = horndownObj.toPixel(horndownObj.epcPosition) - dwTextWidth
            dwEPCPositionEnd        = horndownObj.toPixel(horndownObj.epcPosition) + dwTextWidth

            dwMPPosition            = horndownObj.toPixel(horndownObj.mpPosition)
            dwMPPositionStart       = horndownObj.toPixel(horndownObj.mpPosition) - dwTextWidth
            dwMPPositionEnd         = horndownObj.toPixel(horndownObj.mpPosition) + dwTextWidth

            dwReadyTextPosition = dwReadyPositionStart
            dwPPCTextPosition   = dwPPCPositionStart
            dwWPTextPosition    = dwWPPositionStart
            dwEPCTextPosition   = dwEPCPositionStart
            dwMPTextPosition    = dwMPPositionStart
            isFirstTimePageLoad = false;
        }
        else
        {
            dwReadyPosition         = dwReadyTextPosition + dwTextWidth
            dwReadyPositionStart    = dwReadyTextPosition
            dwReadyPositionEnd      = dwReadyTextPosition + (2*dwTextWidth)

            dwPPCPosition           = dwPPCTextPosition + dwTextWidth
            dwPPCPositionStart      = dwPPCTextPosition
            dwPPCPositionEnd        = dwPPCTextPosition + (2*dwTextWidth)

            dwWPPosition            = dwWPTextPosition + dwTextWidth
            dwWPPositionStart       = dwWPTextPosition
            dwWPPositionEnd         = dwWPTextPosition + (2*dwTextWidth)

            dwEPCPosition           = dwEPCTextPosition + dwTextWidth
            dwEPCPositionStart      = dwEPCTextPosition
            dwEPCPositionEnd        = dwEPCTextPosition + (2*dwTextWidth)

            dwMPPosition            = dwMPTextPosition + dwTextWidth
            dwMPPositionStart       = dwMPTextPosition
            dwMPPositionEnd         = dwMPTextPosition + (2*dwTextWidth)
        }       
        var i = 0
        if(i<paramCount)
        {
            positionsList[0]        = dwReadyPosition
            positionsListSorted[0]  = 1
            positionsList[1]        = dwPPCPosition
            positionsListSorted[1]  = 2
            positionsList[2]        = dwWPPosition
            positionsListSorted[2]  = 3
            positionsList[3]        = dwEPCPosition
            positionsListSorted[3]  = 4
            positionsList[4]        = dwHomePosition
            positionsListSorted[4]  = 5
            positionsList[5]        = dwMPPosition
            positionsListSorted[5]  = 6
            positionsList[6]        = dwEndPosition
            positionsListSorted[6]  = 7
        }

        var firstItr,secondItr,temp
        for(firstItr = 0;firstItr<paramCount;firstItr++)
        {
            for(secondItr = 0;secondItr<paramCount-firstItr-1;secondItr++)
            {
                if(parseFloat(positionsList[secondItr]) > parseFloat(positionsList[secondItr + 1]))
                {
                    temp = positionsList[secondItr];
                    positionsList[secondItr] = positionsList[secondItr+1]
                    positionsList[secondItr+1] = temp

                    temp = positionsListSorted[secondItr];
                    positionsListSorted[secondItr] = positionsListSorted[secondItr+1]
                    positionsListSorted[secondItr+1] = temp
                }
            }
        }

        if((bIsTopPossible == true) && (dwReadyPosition <= dwHomePosition || dwPPCPosition <= dwHomePosition || dwWPPosition <= dwHomePosition || dwEPCPosition <= dwHomePosition || dwMPPosition <= dwHomePosition))
        {
            bIsTopPossible = false
        }
        var dwHomePos = 0,dwReadyPos,dwPPCPos,dwWPPos,dwEPCPos,dwMPPos

        var dwIndex1 = 0,dwIndex2 = 0,dwStart,dwEnd
        if(bIsTopPossible)
        {
            for(dwIndex1 = 0; dwIndex1 < paramCount; dwIndex1++)
            {
                for(dwIndex2 = 0; dwIndex2 < paramCount; dwIndex2++)
                {
                    if(dwIndex2 != dwIndex1 && positionsListSorted[dwIndex1] != 5 && positionsListSorted[dwIndex1] != 7)
                    {
                        dwStart = positionsList[dwIndex1] - dwTextWidth
                        dwEnd = positionsList[dwIndex1] + dwTextWidth
                        if((positionsList[dwIndex2] - dwTextWidth >= dwStart && positionsList[dwIndex2] - dwTextWidth <= dwEnd)||
                                (positionsList[dwIndex2] - dwTextWidth <= dwStart && positionsList[dwIndex2] + dwTextWidth >= dwStart))
                        {
                            movePositionUp(dwIndex1)
                            setHornpostionValue()
                            return
                        }
                    }
                }
            }
        }
        else
        {
            for(dwIndex1 = paramCount-1; dwIndex1 >= 0; dwIndex1--)
            {
                for(dwIndex2 = 0; dwIndex2 < paramCount; dwIndex2++)
                {
                    if(dwIndex2 != dwIndex1 && positionsListSorted[dwIndex1] != 5 && positionsListSorted[dwIndex1] != 7)
                    {
                        dwStart = positionsList[dwIndex1] - dwTextWidth
                        dwEnd = positionsList[dwIndex1] + dwTextWidth
                        if((positionsList[dwIndex2] - dwTextWidth >= dwStart && positionsList[dwIndex2] - dwTextWidth <= dwEnd)||
                                (positionsList[dwIndex2] - dwTextWidth <= dwStart && positionsList[dwIndex2] + dwTextWidth >= dwStart))
                        {
                            movePositionDown(dwIndex1)
                            setHornpostionValue()
                            return
                        }
                    }
                }
            }
        }
    }

    function movePositionUp(dwIndex)
    {
        var dwTextWidth = 10
        switch(positionsListSorted[dwIndex])
        {
        case 1:
            dwReadyTextPosition = dwReadyTextPosition - 5
            positionsList[dwIndex] = dwReadyTextPosition - dwTextWidth
            break
        case 2:
            dwPPCTextPosition = dwPPCTextPosition - 5
            positionsList[dwIndex] = dwPPCTextPosition - dwTextWidth
            break
        case 3:
            dwWPTextPosition = dwWPTextPosition - 5
            positionsList[dwIndex] = dwWPTextPosition - dwTextWidth
            break
        case 4:
            dwEPCTextPosition = dwEPCTextPosition - 5
            positionsList[dwIndex] = dwEPCTextPosition - dwTextWidth
            break
        case 6:
            dwMPTextPosition = dwMPTextPosition - 5
            positionsList[dwIndex] = dwMPTextPosition - dwTextWidth
            break
        }        
    }

    function movePositionDown(dwIndex)
    {
        var dwTextWidth = 10
        switch(positionsListSorted[dwIndex])
        {
        case 1:
            dwReadyTextPosition = dwReadyTextPosition + 5
            positionsList[dwIndex] = dwReadyTextPosition - dwTextWidth
            break
        case 2:
            dwPPCTextPosition = dwPPCTextPosition + 5
            positionsList[dwIndex] = dwPPCTextPosition - dwTextWidth
            break
        case 3:
            dwWPTextPosition = dwWPTextPosition + 5
            positionsList[dwIndex] = dwWPTextPosition - dwTextWidth
            break
        case 4:
            dwEPCTextPosition = dwEPCTextPosition + 5
            positionsList[dwIndex] = dwEPCTextPosition - dwTextWidth
            break
        case 6:
            dwMPTextPosition = dwMPTextPosition + 5
            positionsList[dwIndex] = dwMPTextPosition - dwTextWidth
            break
        }
    }

    function updateHornPositions()
    {
        isFirstTimePageLoad = true
        setHornpostionValue()
    }

    function updateActSetupScreen()
    {
        //update Act Setup  values
        id_Actuator_recipeAsterisk.visible = recipe.isRecipeModified
        id_Actuator_recipeNumber.text = horndownObj.getRecipeNumberForActuator()
        id_Actuator_recipeName.text = horndownObj.getRecipeNameForActuator()
    }

    Connections{
        target: recipe
        onRefreshScreenPopUpSignal: {
            updateActSetupScreen()
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

    Connections
    {
        target: horndownObj
        onUpdateHornPositions :
        {
            canvasAll.resetCanvas()
        }
    }

    Component.onCompleted:
    {
        bIsTopPossible = true
        fromAdvanced = true
        horndownObj.startOrStopWeld(0)
        horndownObj.findPartContactRequestToBL()
        strokeLenthInPixel=loaderelement.height*(0.85)
        horndownObj.strokeLengthInPixel = loaderelement.height*(0.85)
        horndownObj.fillActiveRecipeForActuator(1)
        isActSetupActive = true;
        login.setWSRestrictedScreens(WSRestrictionScreen.GENERAL_RESTRICTED)

        unitType = horndownObj.getUnitsType()
        var expectpartcontact = recipe.getStructureValue(203)
        partcontactval = parseFloat(expectpartcontact)
        if(unitType)
        {
            qmlTextActualForceUnit = qsTr(" lbs")
            qmlTextprepartContactUnit = qsTr(" in")
            qmlTextAbsolutePositionUnit = qsTr(" in")
            qmlTextdistanceUnit = qsTr(" in")
            qmlTextpreContactVelocityUnit = qsTr(" in/s")
        }
        if(advancedHornDownClicked == true)
        {
            radioButton1         = radioButtonFPC
            radioButton2         = radioButtonJOG
            radioButton3         = radioButtonHORNDOWN
            findpartcontactstatus = findpartcontactstatusActSetup
            readypositionswitch   = readypositionswitchActSetup
            horndownclampswitch   = horndownclampswitchActSetup
            horndownswitchpressed = horndownswitchpressedActSetup
            jogswitchpressed      = jogswitchpressedActSetup
            setflagUp             = setflagUpActSetup
            setflagDown           = setflagDownActSetup
            id_ReadyPosition_switch.checked  = readyPosActSetup
            id_HornClampDown_switch.checked = hornClampActSetup
            if(setflagDown)
                changeImageDown()
            if(setflagUp)
                changeImageUp()
            advancedHornDownClicked = false
            if(radioButton1 == true)
            {
                horndownObj.findPartContactRequestToBL()
                id_press_start_switch_text.text=qmlTextStatus1
            }
            else if(radioButton3 == true)
            {
                horndownObj.horndownRequestToBL(horndownclampswitch)
                id_press_start_switch_text.text=qmlTextStatus3
            }
            else if(radioButton2 == true && setflagDown == true)
            {
                horndownObj.jogDownRequestToBL()
                id_press_start_switch_text.text=qmlTextStatus5
            }
            else if(radioButton2 == true && setflagUp == true)
            {
                horndownObj.jogUpRequestToBL()
                id_press_start_switch_text.text=qmlTextStatus4
            }
        }
        else if(partcontactval > 0)
        {
            var oldModes = horndownObj.getHornDownRememberModes()
            var oldModesList = oldModes.split(",")
            radioButton1  = parseInt(oldModesList[0])
            radioButton2  = parseInt(oldModesList[1])
            radioButton3  = parseInt(oldModesList[2])
            findpartcontactstatus = parseInt(oldModesList[3])
            jogswitchpressed = parseInt(oldModesList[4])
            horndownswitchpressed = parseInt(oldModesList[5])
            setflagUp = parseInt(oldModesList[6])
            setflagDown = parseInt(oldModesList[7])
            if(setflagDown)
                changeImageDown()
            if(setflagUp)
                changeImageUp()
            if(radioButton1 == true)
            {
                horndownObj.findPartContactRequestToBL()
                id_press_start_switch_text.text=qmlTextStatus1
            }
            else if(radioButton3 == true)
            {
                horndownObj.horndownRequestToBL(horndownclampswitch)
                id_press_start_switch_text.text=qmlTextStatus3
            }
            else if(radioButton2 == true && setflagDown == true)
            {
                horndownObj.jogDownRequestToBL()
                id_press_start_switch_text.text=qmlTextStatus5
            }
            else if(radioButton2 == true && setflagUp == true)
            {
                horndownObj.jogUpRequestToBL()
                id_press_start_switch_text.text=qmlTextStatus4
            }
            oldForceValue =  recipe.getStructureValue(224)
            recipe.updateAcutatorForceSetting()
        }
        else
        {
            radioButton1 = true
            radioButton2 = false
            radioButton3 = false
            findpartcontactstatus = false
            readypositionswitch = false
            horndownclampswitch = false
            horndownswitchpressed = false
            jogswitchpressed = false
            setflagUp = true
            setflagDown = false
            horndownObj.findPartContactRequestToBL()
            id_press_start_switch_text.text=qmlTextStatus1
            oldForceValue =  recipe.getStructureValue(224)
            recipe.updateAcutatorForceSetting()
        }

        if(partcontactval > 0)
        {
            findpartcontactstatus = true
        }
        id_ReadyPosition_switch.checked = recipe.getStructureValue(234) //true
        readypositionswitch = recipe.getStructureValue(234)

        updateHornPositions()
    }

    Component.onDestruction:
    {
        if(advancedHornDownClicked == false)
        {
            horndownObj.retractRequestToBL()
            if(isForceValueChanged)
                recipe.setActuatorRecipeValue(oldForceValue,false)

            if(!isLogout)
            {
                horndownObj.startOrStopWeld(1)
            }

            isActSetupActive = false;
            login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
        }
        radioButtonFPC = radioButton1
        radioButtonJOG = radioButton2
        radioButtonHORNDOWN = radioButton3
        findpartcontactstatusActSetup = findpartcontactstatus
        horndownswitchpressedActSetup = horndownswitchpressed
        jogswitchpressedActSetup = jogswitchpressed
        setflagUpActSetup = setflagUp
        setflagDownActSetup = setflagDown
        horndownObj.setHornDownRememberModes(radioButton1,radioButton2,radioButton3,findpartcontactstatus,jogswitchpressed,horndownswitchpressed,setflagUp,setflagDown)
    }

    /**
    *@breif: To change image on clicking
    **/
    function changeImageUp()
    {
        /*Image icon update based on System Type*/
        id_Image_Up.source = (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/Jog_Up_Image_ASX.svg" :
                                                                    "qrc:/images/image/Jog_Up_Image.svg"
        id_Image_Down.source="qrc:/images/image/Jog_Down.svg"
    }

    /**
    *@breif: To change image on clicking
    **/
    function changeImageDown()
    {
        /*Image icon update based on System Type*/
        id_Image_Down.source = (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/Jog_Down_Image_ASX.svg" :
                                                                      "qrc:/images/image/Jog_Down_Image.svg"
        id_Image_Up.source="qrc:/images/image/Jog_Up.svg"
    }

    Canvas
    {
        id:canvasAll
        anchors.fill: parent
        z:20
        smooth: true
        onPaint:
        {
            var context = getContext("2d")
            var i = 1;
            var x1
            var y1
            var x2
            var y2
            for(;i<=10;i++)
            {
                switch(i)
                {
                case 1:
                    x1 = id_StatusofHorn.x + id_ReadyHornPosition_text.x + id_ReadyHornPosition_text.width
                    y1 = id_StatusofHorn.y + id_ReadyHornPosition_text.y + id_ReadyHornPosition_text.height/2
                    x2 = id_StatusofHorn.x + id_ReadyPosition.x
                    y2 = id_StatusofHorn.y + id_ReadyPosition.y + id_ReadyPosition.height/2
                    context.strokeStyle = "#8fa350"
                    break
                case 2:
                    x1 = id_StatusofHorn.x + id_WpHornPosition_text.x + id_WpHornPosition_text.width
                    y1 = id_StatusofHorn.y + id_WpHornPosition_text.y + id_WpHornPosition_text.height/2
                    x2 = id_StatusofHorn.x + id_WPPosition.x
                    y2 = id_StatusofHorn.y + id_WPPosition.y + id_WPPosition.height/2
                    context.strokeStyle = "#645287"
                    break
                case 3:
                    x1 = id_StatusofHorn.x + id_PPCHornPosition_text.x + id_PPCHornPosition_text.width
                    y1 = id_StatusofHorn.y + id_PPCHornPosition_text.y + id_PPCHornPosition_text.height/2
                    x2 = id_StatusofHorn.x + id_PPCPosition.x
                    y2 = id_StatusofHorn.y + id_PPCPosition.y + id_PPCPosition.height/2
                    context.strokeStyle = "#d26d19"
                    break
                case 4:
                    x1 = id_StatusofHorn.x + id_EPCHornPosition_text.x + id_EPCHornPosition_text.width
                    y1 = id_StatusofHorn.y + id_EPCHornPosition_text.y + id_EPCHornPosition_text.height/2
                    x2 = id_StatusofHorn.x + id_EPCPosition.x
                    y2 = id_StatusofHorn.y + id_EPCPosition.y + id_EPCPosition.height/2
                    context.strokeStyle = "#d26d19"
                    break
                case 5:
                    x1 = id_StatusofHorn.x + id_MPHornPosition_text.x + id_MPHornPosition_text.width
                    y1 = id_StatusofHorn.y + id_MPHornPosition_text.y + id_MPHornPosition_text.height/2
                    x2 = id_StatusofHorn.x + id_MPPosition.x
                    y2 = id_StatusofHorn.y + id_MPPosition.y + id_MPPosition.height/2
                    context.strokeStyle = "#645287"
                    break
                case 6:
                    x1 = id_StatusofHorn.x + id_ReadyHornPositionVal.x
                    y1 = id_StatusofHorn.y + id_ReadyHornPositionVal.y + id_ReadyHornPositionVal.height/2
                    x2 = id_StatusofHorn.x + id_ReadyPosition.x + id_ReadyPosition.width
                    y2 = id_StatusofHorn.y + id_ReadyPosition.y + id_ReadyPosition.height/2
                    context.strokeStyle = "#8fa350"
                    break
                case 7:
                    x1 = id_StatusofHorn.x + id_WpHornPositionVal.x
                    y1 = id_StatusofHorn.y + id_WpHornPositionVal.y + id_WpHornPositionVal.height/2
                    x2 = id_StatusofHorn.x + id_WPPosition.x + id_WPPosition.width
                    y2 = id_StatusofHorn.y + id_WPPosition.y + id_WPPosition.height/2
                    context.strokeStyle = "#645287"
                    break
                case 8:
                    x1 = id_StatusofHorn.x + id_PPCHornPositionVal.x
                    y1 = id_StatusofHorn.y + id_PPCHornPositionVal.y + id_PPCHornPositionVal.height/2
                    x2 = id_StatusofHorn.x + id_PPCPosition.x + id_PPCPosition.width
                    y2 = id_StatusofHorn.y + id_PPCPosition.y + id_PPCPosition.height/2
                    context.strokeStyle = "#d26d19"
                    break
                case 9:
                    x1 = id_StatusofHorn.x + id_EPCHornPositionVal.x
                    y1 = id_StatusofHorn.y + id_EPCHornPositionVal.y + id_EPCHornPositionVal.height/2
                    x2 = id_StatusofHorn.x + id_EPCPosition.x + id_EPCPosition.width
                    y2 = id_StatusofHorn.y + id_EPCPosition.y + id_EPCPosition.height/2
                    context.strokeStyle = "#d26d19"
                    break
                case 10:
                    x1 = id_StatusofHorn.x + id_MPHornPositionVal.x
                    y1 = id_StatusofHorn.y + id_MPHornPositionVal.y + id_MPHornPositionVal.height/2
                    x2 = id_StatusofHorn.x + id_MPPosition.x + id_MPPosition.width
                    y2 = id_StatusofHorn.y + id_MPPosition.y + id_MPPosition.height/2
                    context.strokeStyle = "#645287"
                    break
                }
                context.beginPath()
                context.lineWidth = 2
                context.moveTo(x1, y1)
                context.lineTo(x2, y2)
                context.stroke()
            }
        }
        function resetCanvas()
        {
            var ctx = getContext("2d");
            ctx.reset();
            canvasAll.requestPaint();
        }
    }
    Rectangle {
        id: id_Horn_Status
        color: "#ffffff"
        width: parent.width*(.4)
        height: parent.height
        anchors.left: parent.left
        Image {
            id: id_Horn_Pointer
            source: "qrc:/images/image/Pointer_Horn_Image.svg"
            anchors.left: id_StatusofHorn.right
            anchors.leftMargin: 0
            anchors.top: id_StatusofHorn.top
            anchors.topMargin: horndownObj.toPixel(horndownObj.hornPointer)-7
            z: 3
        }
        Text {
            id: id_Horn_Pointer_text
            //text: qsTr("100340 um")
            text: (horndownObj.hornPointer) + qmlTextdistanceUnit
            anchors.left: id_StatusofHorn.right
            anchors.leftMargin:12
            anchors.top: id_StatusofHorn.top
            anchors.topMargin: horndownObj.toPixel(horndownObj.hornPointer)-20
            font.family: Style.semibold.name
            font.pixelSize: Style.style4
            z: 3
        }

        Rectangle
        {
            id:id_StatusofHorn
            color: "#d1d1d1"
            width:parent.width*(0.3)
            height:parent.height*(0.85)
            anchors.top: parent.top
            anchors.topMargin: 19
            anchors.centerIn: parent
            border.color: "#8b8b8b"
            border.width: 2
            radius: 5
            Text {
                id: id_HomePosition_text
                text: qmlTextHome
                anchors.right: id_HomePosition.left
                anchors.rightMargin:rightMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: homepositionvalue-10
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }

            Rectangle
            {
                id:id_HomePosition
                color: "#00b0f0"
                width:parent.width*(1.2)
                height:5
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: homepositionvalue
                border.color: "#7fd7f7"
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                radius: 5
            }

            Text {
                id: id_ReadyHornPosition_text
                text: qmlTextReady
                anchors.right: id_ReadyPosition.left
                anchors.rightMargin:rightMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwReadyTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }

            Rectangle
            {
                id:id_ReadyPosition
                color: "#8fa350"
                width:parent.width*(1.2)
                height:5
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: horndownObj.toPixel(horndownObj.readyposition)
                border.color: "#99ab60"
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                radius: 5
            }
            Text {
                id: id_ReadyHornPositionVal
                text: horndownObj.readyposition  + qmlTextdistanceUnit
                anchors.left: id_ReadyPosition.right
                anchors.leftMargin:leftMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwReadyTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }

            Text {
                id: id_WpHornPosition_text
                text: qmlTextWP
                anchors.right: id_WPPosition.left
                anchors.rightMargin:rightMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwWPTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            Rectangle
            {
                id:id_WPPosition
                color: "#645287"
                width:parent.width*(1.2)
                height:5
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: horndownObj.toPixel(horndownObj.wpPosition)
                border.color: "#645287"
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                radius: 5
            }
            Text {
                id: id_WpHornPositionVal
                text: horndownObj.wpPosition  + qmlTextdistanceUnit
                anchors.left: id_WPPosition.right
                anchors.leftMargin:leftMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwWPTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }

            Text {
                id: id_PPCHornPosition_text
                text: qmlTextPPC
                anchors.right: id_PPCPosition.left
                anchors.rightMargin:rightMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwPPCTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }

            Rectangle
            {
                id:id_PPCPosition
                color: "#d26d19"
                width:parent.width*(1.2)
                height:5
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: horndownObj.toPixel(horndownObj.ppcPosition)
                border.color: "#d26d19"
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                radius: 5
            }
            Text {
                id: id_PPCHornPositionVal
                text: horndownObj.ppcPosition + qmlTextdistanceUnit
                anchors.left: id_PPCPosition.right
                anchors.leftMargin:leftMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwPPCTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }

            Text {
                id: id_EPCHornPosition_text
                text: qmlTextEPC
                anchors.right: id_EPCPosition.left
                anchors.rightMargin:rightMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwEPCTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            Rectangle
            {
                id:id_EPCPosition
                color: "#d26d19"
                width:parent.width*(1.2)
                height:5
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: horndownObj.toPixel(horndownObj.epcPosition)
                border.color: "#d26d19"
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                radius: 5
            }
            Text {
                id: id_EPCHornPositionVal
                text: horndownObj.epcPosition + qmlTextdistanceUnit
                anchors.left: id_EPCPosition.right
                anchors.leftMargin:leftMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwEPCTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }

            Text {
                id: id_MPHornPosition_text
                text: qmlTextMP
                anchors.right: id_MPPosition.left
                anchors.rightMargin:rightMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwMPTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            Rectangle
            {
                id:id_MPPosition
                color: "#645287"
                width:parent.width*(1.2)
                height:5
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: horndownObj.toPixel(horndownObj.mpPosition)
                border.color: "#645287"
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                radius: 5
            }
            Text {
                id: id_MPHornPositionVal
                text: horndownObj.mpPosition + qmlTextdistanceUnit
                anchors.left: id_MPPosition.right
                anchors.leftMargin: leftMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: dwMPTextPosition
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            Text {
                id: id_FixtureHornPosition_text
                text: qmlTextFixture
                anchors.right: id_FixturePosition.left
                anchors.rightMargin:60//rightMarginSpace
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: id_StatusofHorn.height-10
                color:"#212529"
                verticalAlignment: Text.AlignVCenter
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            Rectangle
            {
                id:id_FixturePosition
                color: "#7f7f7f"
                width:parent.width*(1.2)
                height:5
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: id_StatusofHorn.height
                border.color: "#7f7f7f"
                anchors.horizontalCenter: parent.horizontalCenter
                border.width: 2
                radius: 5
            }
            Text {
                id: id_strokeLength_text
                text: horndownObj.getStrokeLengthFormConfig() + qmlTextdistanceUnit
                anchors.left: id_StatusofHorn.right
                anchors.leftMargin:20
                anchors.top: id_StatusofHorn.top
                anchors.topMargin: id_StatusofHorn.height-10
                color:"#212529"
                verticalAlignment: Text.AlignVCenter
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
        }
    }

    Rectangle {
        id:id_Actuator_set_up_Main_rect
        color: "#ffffff"
        width: parent.width*(.6)
        height: parent.height*(.82)
        anchors.left: id_Horn_Status.right
        anchors.leftMargin: 40
        anchors.right: parent.right
        anchors.rightMargin: parent.width*(.05)

        Rectangle
        {
            id:id_Actuator_Recipe_info
            width: parent.width*(.4)
            height: parent.height*(0.08)
            anchors.top: id_Actuator_set_up_Main_rect.top
            anchors.topMargin: 40
            Text {
                id: id_Actuator_recipeNumber
                text: horndownObj.getRecipeNumberForActuator()
                anchors.left: parent.left
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
                anchors.leftMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 6
            }
            Text {
                id: id_Actuator_colon
                text: qsTr(" : ")
                anchors.left: id_Actuator_recipeNumber.right
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
                anchors.top: parent.top
                anchors.topMargin: 6
            }
            Text {
                id: id_Actuator_recipeName
                text: horndownObj.getRecipeNameForActuator()
                anchors.left: id_Actuator_colon.right
                color:"#212529"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
                anchors.top: parent.top
                anchors.topMargin: 6
            }
            Text {
                id: id_Actuator_recipeAsterisk
                color: "#212529"
                text:qmltextStar
                anchors.topMargin: 6
                anchors.top: parent.top
                anchors.right: id_Actuator_recipeNumber.left
                anchors.rightMargin: 1
                font.family: Style.semibold.name
                font.bold: true
                font.pixelSize: Style.style5
                verticalAlignment: Text.AlignVCenter
                visible: recipe.isRecipeModified//activeRecipeEditFlag?true:false
            }
        }

        Rectangle{
            id:id_Actuator_set_up_Param
            width: parent.width*(.5)
            height: parent.height
            color: "#ffffff"
            anchors.left: parent.left
            clip: true
            anchors.top: id_Actuator_Recipe_info.bottom

            GridView
            {
                id: actuatorParamList_Id
                x:20
                y:20
                width: parent.width*(.9)
                height: id_Actuator_set_up_Param.height
                anchors.centerIn: parent.Center
                cellHeight: 80
                cellWidth: parent.width*(.9)
                model:setupModel//actuatorparamList
                interactive: false
                property var clickedIndex;
                delegate:WeldModeItem{
                    id: newActuatorparam
                    width:280
                    height:60
                    paramText:model.modelData.ParameterName
                    paramTextLocale:model.modelData.ParameterLocaleName
                    paramValue:model.modelData.ParameterValue
                    paramUnit:model.modelData.ParameterUnit
                    paramStripColor:model.modelData.ParameterStripColor
                    property var split
                    Component.onCompleted: {
                        if(paramText == "Expected Contact Window")
                        {
                            split = paramValue.split(":")
                            paramMinValue = split[0]
                            paramMaxValue = split[1]
                        }
                    }
                    control_mode_mouseArea.onClicked:
                    {
                        /*Index 4 represent Expected Contact Window*/
                        if(index == 4)
                        {
                            setupNumpad.setLimitValues(paramMinValue,paramMaxValue,true)
                            setupNumpad.oldMinVal = paramMinValue
                            setupNumpad.oldMaxVal = paramMaxValue
                            setupNumpad.setupLimitheader = qmltextExpectedContactWindow
                            setupNumpad.setupOnOffRec = false
                            setupNumpad.hornDown = true
                            numPadStatus = 3
                            setupNumpad.mpText = true
                            setupNumpad.wpText = true
                            setupNumpad.open()
                            enableMainwindowOpacity.start()
                            setupNumpad.mode = ParameterName
                        }
                        else
                        {
                            if(index == 1 && partcontactval <= 0)
                            {
                                id_press_start_switch_text.text = qmlTextStatus6
                                return
                            }
                            setActuatorParameter(paramText,paramTextLocale,model.modelData.ParameterValue,paramUnit,model.modelData.ParameterMinValue,model.modelData.ParameterMaxValue)
                            actuatorParamList_Id.clickedIndex=index
                            /*Flag for non Expected Part Contact Window*/
                            setupNumpad.hornDown = false
                        }
                    }
                }
            }
        }
        Rectangle{
            id:id_Actuator_set_up_Carousel
            width: parent.width*(.58)
            height: parent.height
            color: "#ffffff"
            anchors.left: id_Actuator_set_up_Param.right
            anchors.leftMargin: 40
            Rectangle
            {
                id:id_Readyswitch_Main_Rect
                width: parent.width
                height: parent.height*(.2)
                anchors.top: parent.top
                anchors.topMargin: 45
                Text {
                    id: id_ReadyPosition_text
                    width: parent.width*(.2)
                    height: (parent.height)/2
                    text: qmlTextPosition
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    anchors.leftMargin: 15
                }

                ToogleBar{
                    id:id_ReadyPosition_switch
                    anchors.left: id_ReadyPosition_text.right
                    anchors.leftMargin: parent.width*(0.2)
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    onPressedChanged:
                    {
                        refreshSwitch = true
                    }
                    onCheckedChanged:
                    {
                        if(refreshSwitch == true)
                        {
                            recipe.setParamaterChangedFlag(1)
                            refreshSwitch = false
                            if(checked)
                            {
                                readypositionswitch=true
                                recipe.setStructureValue("ready position toggle",1,true)
                            }
                            else
                            {
                                //horndownObj.setMyReadyPositionRequestToBL("0")
                                recipe.setStructureValue("ready position toggle",0,true)
                                readypositionswitch=false
                            }
                        }
                    }
                }

                Text {
                    id: id_HornClampDown_text
                    width: parent.width*(.2)
                    height: (parent.height)/2
                    text: qmlTextHornClampDown
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: parent.left
                    anchors.top: id_ReadyPosition_text.bottom
                    anchors.topMargin: 2
                    anchors.leftMargin: 15
                }

                ToogleBar{
                    id:id_HornClampDown_switch
                    anchors.left: id_HornClampDown_text.right
                    anchors.leftMargin: parent.width*(0.2)
                    anchors.top: id_ReadyPosition_switch.bottom
                    anchors.topMargin: 20
                    enabled: radioButton3
                    onCheckedChanged:
                    {
                        if(checked)
                            horndownclampswitch=true
                        else
                            horndownclampswitch=false
                        horndownObj.horndownRequestToBL(horndownclampswitch)
                    }
                }
            }
            Rectangle
            {
                id:id_RadioItem_Main_Rect
                width: parent.width
                height: parent.height*(.45)

                anchors.top: id_Readyswitch_Main_Rect.bottom
                anchors.topMargin: 2
                ActuatorRadioButton
                {
                    id:id_partContact
                    rbTextdata:qmlTextFindPartContact
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    radioLeft.onClicked:
                    {
                        radioButtonSelection=true
                        radioButton2=false
                        radioButton3=false
                        radioButton1=true
                        jogswitchpressed=false
                        horndownswitchpressed=false
                        horndownClamped=false
                        stubreadypos+=1250
                        horndownObj.resetValues()
                        horndownObj.findPartContactRequestToBL()
                        var expectpartcontact = recipe.getStructureValue(203)
                        partcontactval = parseFloat(expectpartcontact)
                        if(partcontactval > 0)
                        {
                            findpartcontactstatus = true
                        }
                        id_press_start_switch_text.text=qmlTextStatus1
                    }
                    checkedStatus: radioButton1
                }
                ActuatorRadioButton
                {
                    id:id_jog
                    rbTextdata:qmlTextJog
                    anchors.top: id_partContact.bottom
                    anchors.topMargin: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    radioLeft.onClicked:
                    {
                        radioButtonSelection=true
                        radioButton1=false
                        radioButton3=false
                        radioButton2=true
                        jogswitchpressed=true
                        horndownswitchpressed=false
                        horndownClamped=false
                        horndownObj.resetValues()
                        if(setflagUp)
                        {
                            id_press_start_switch_text.text=qmlTextStatus4
                            horndownObj.jogUpRequestToBL()
                        }
                        if(setflagDown)
                        {
                            id_press_start_switch_text.text=qmlTextStatus5
                            horndownObj.jogDownRequestToBL()
                        }
                    }
                    checkedStatus: radioButton2

                }
                Rectangle
                {
                    id:id_image_rect
                    //                border.color: "green"
                    //                border.width: 2
                    width: 70
                    height: 74
                    anchors.left: id_jog.right
                    anchors.leftMargin: 160
                    anchors.top: id_partContact.bottom
                    anchors.topMargin: 35
                    Image {
                        id: id_Image_Up
                        /*Image icon update based on System Type*/
                        source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/Jog_Up_Image_ASX.svg" :
                                                                       "qrc:/images/image/Jog_Up_Image.svg"
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: imageWidth
                        height:imageHeight
                    }
                    MouseArea
                    {
                        id:mouseareaof_UpImage
                        anchors.fill: id_Image_Up
                        onClicked:
                        {
                            setflagDown=false
                            setflagUp=true
                            changeImageUp()
                            if(hornpointerPosition>0)
                            {
                                // hornpointerPosition= hornpointerPosition-2
                                //hornPointerDistance=hornPointerDistance-1.25
                            }
                            horndownObj.resetValues()
                            horndownObj.jogUpRequestToBL()
                            //setActualActuatordata()
                            id_press_start_switch_text.text=qmlTextStatus4
                            //hornpointerPosition=parseFloat(horndownObj.hornPointer)
                        }
                        visible: id_jog.checkedStatus?true:false
                    }

                    Image {
                        id: id_Image_Down
                        source: "qrc:/images/image/Jog_Down.svg"
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: imageWidth
                        height: imageHeight
                    }
                    MouseArea
                    {
                        id:mouseareaof_DownImage
                        anchors.fill: id_Image_Down
                        onClicked:
                        {
                            setflagUp=false
                            setflagDown=true
                            changeImageDown()
                            if(hornpointerPosition < strokeLenthInPixel-1)
                            {
                                //hornpointerPosition= hornpointerPosition+2
                                //hornPointerDistance=hornPointerDistance+1.25
                            }
                            horndownObj.resetValues()
                            horndownObj.jogDownRequestToBL()
                            //setActualActuatordata()
                            id_press_start_switch_text.text=qmlTextStatus5
                            //hornpointerPosition=parseFloat(horndownObj.hornPointer)
                        }
                        visible: id_jog.checkedStatus?true:false
                    }
                }

                ActuatorRadioButton
                {
                    id:id_HornDown
                    rbTextdata:qmlTextHornDown
                    anchors.top: id_jog.bottom
                    anchors.topMargin: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    radioLeft.onClicked:
                    {
                        if(findpartcontactstatus)
                        {
                            radioButtonSelection=true
                            radioButton1=false
                            radioButton2=false
                            radioButton3=true
                            horndownObj.resetValues()
                            horndownObj.horndownRequestToBL(horndownclampswitch)
                            //setActualActuatordata()
                            horndownswitchpressed=true
                            jogswitchpressed=false
                            id_press_start_switch_text.text=qmlTextStatus3
                        }
                        else
                        {
                            radioButtonSelection=true
                            id_press_start_switch_text.text = qmlTextStatus6
                            radioButton1=false
                            radioButton2=false
                            radioButton3=true
                            jogswitchpressed=false
                        }
                    }
                    checkedStatus: radioButton3
                }
            }

            Rectangle
            {
                id:id_textItem_Main_Rect
                width: parent.width
                height: parent.height*(.4)
                anchors.bottom: parent.bottom

                Text {
                    id: id_AbsolutePosition_text
                    height: parent.height*(0.01)
                    text: qmlTextAbsolutePosition
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.top: parent.top
                    anchors.topMargin: 0
                }
                Text {
                    id: id_AbsolutePositionValue_text
                    height: parent.height*(0.01)
                    text: horndownObj.absolutePosition + qmlTextAbsolutePositionUnit
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: id_AbsolutePosition_text.right
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.leftMargin:10
                }

                Text {
                    id: id_PrePartContact_text
                    height: parent.height*(0.01)
                    text: qmlTextprepartContact
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: parent.left
                    anchors.top: id_AbsolutePosition_text.bottom
                    anchors.topMargin: 40
                    anchors.leftMargin: 15
                }
                Text {
                    id: id_PrePartContactValue_text
                    height: parent.height*(0.01)
                    text: horndownObj.actualPPC + qmlTextprepartContactUnit
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: id_PrePartContact_text.right
                    anchors.top: id_AbsolutePosition_text.bottom
                    anchors.topMargin: 40
                    anchors.leftMargin:10
                }

                Text {
                    id: id_Force_text
                    height: parent.height*(0.01)
                    text: qmlTextActualForce
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: parent.left
                    anchors.top: id_PrePartContact_text.bottom
                    anchors.topMargin: 40
                    anchors.leftMargin: 15
                }
                Text {
                    id: id_ForceValue_text
                    height: parent.height*(0.01)
                    text: horndownObj.actualForce + qmlTextActualForceUnit
                    color:"#212529"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style4
                    anchors.left: id_Force_text.right
                    anchors.top: id_PrePartContact_text.bottom
                    anchors.topMargin: 40
                    anchors.leftMargin:10
                }
            }
        }
    }

    Rectangle{
        id:id_control_Button
        color:"#ffffff"
        width:parent.width*(.6)
        height: parent.height*(.2)
        anchors.bottom:parent.bottom
        anchors.left: id_Horn_Status.right
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: parent.width*(.05)
        Row {
            id:id_three_switch_row
            anchors.left:parent.left
            anchors.leftMargin: 16
            width: parent.width
            spacing: parent.width/9
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 15

            PrimaryButton
            {
                id:id_AdvanceFeature
                anchors.top: parent.top
                anchors.topMargin: 5
                text: qmlTextAdvancedFeature
                onClicked:
                {
                    titlebarchild.visible=true
                    titlebarchildtext.text=qsTr("ADVANCED FEATURE")
                    titlbaronclicked="HornDown.qml"
                    fromAdvanced = false
                    loaderelement.setSource("HornDownAdvanced.qml")
                    advancedHornDownClicked = true
                    radioButtonFPC = radioButton1
                    radioButtonJOG = radioButton2
                    radioButtonHORNDOWN = radioButton3
                    findpartcontactstatusActSetup = findpartcontactstatus
                    readypositionswitchActSetup = readypositionswitch
                    horndownclampswitchActSetup = horndownclampswitch
                    horndownswitchpressedActSetup = horndownswitchpressed
                    jogswitchpressedActSetup = jogswitchpressed
                    setflagUpActSetup = setflagUp
                    setflagDownActSetup = setflagDown
                    readyPosActSetup = id_ReadyPosition_switch.checked
                    hornClampActSetup = id_HornClampDown_switch.checked
                }
            }

            PrimaryButton
            {
                id:id_retract
                anchors.top: parent.top
                anchors.topMargin: 5
                text: qmlTextRetract
                onClicked:
                {
                    if(horndownclampswitch)
                    {
                        horndownObj.retractRequestToBL()
                        horndownClamped=false
                        //setActualActuatordata()
                    }
                }
                enabled:(horndownswitchpressed && horndownclampswitch)?true:false
            }

            PrimaryButton
            {
                id:id_SetMyReadyPosition
                anchors.top: parent.top
                anchors.topMargin: 5
                text: qmlTextsetmyReadyPosition
                onClicked:
                {
                    if(partcontactval <= 0)
                    {
                        id_press_start_switch_text.text = qmlTextStatus6
                    }
                    else if(parseFloat(horndownObj.hornPointer) > partcontactval)
                    {
                        id_press_start_switch_text.text = qmlTextStatus7
                    }
                    else
                    {
                        if(readypositionswitch)
                        {
                            horndownObj.setMyReadyPositionRequestToBL(horndownObj.hornPointer)
                            updateHornPositions()
                            canvasAll.resetCanvas()
                            //setActualActuatordata()
                        }
                    }
                }
                enabled: (jogswitchpressed && readypositionswitch)?true:false
            }
        }
        Rectangle
        {
            color: "#ffffff"
            width: parent.width*(.4)
            height: parent.height*(.2)
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.bottom: parent.bottom
            Text {
                id: id_press_start_switch_text
                text: ""
                anchors.centerIn: parent
                color: "#f4a041"
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
                //visible:radioButtonSelection?true:false
                Connections
                {
                    target: horndownObj

                    onUpdateHornDownStatusText:
                    {
                        id_press_start_switch_text.text = strHornDownStatusText
                        var expectpartcontact = recipe.getStructureValue(203)
                        partcontactval = parseFloat(expectpartcontact)
                        if(partcontactval > 0)
                        {
                            findpartcontactstatus = true
                        }
                        responseType = horndownObj.getResponseType()
                        if(((responseType== 7) || (responseType== 8)) && (horndownclampswitch==true))
                        {
                            horndownClamped = true
                        }
                        else
                        {
                            horndownClamped = false
                        }
                    }
                }
            }
        }
    }
}
