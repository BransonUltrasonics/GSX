import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Universal 2.0
import Style 1.0
import com.branson.runtimefeatures 1.0

Rectangle{
    id: steppingNumpopup
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false

    readonly property string qmltextInvalidValueMsg : qsTr("Invalid value is entered, Reverted to old value")
    readonly property string qmltextSuspectRejectMsg : qsTr("Suspect/Reject values are incorrect")
    readonly property string qmltextDuplicateValueMsg : qsTr("Duplicate value entered,Do you want to override")
    readonly property string qmltextInvalidValueCorrectMsg : qsTr("Invalid value is entered, Enter correct value again")
    readonly property string qmltextWeldAmplitudeLowerCase : "weld amplitude"
    readonly property string qmltextForceStep1: "weld force"
    readonly property string qmltextForceStepRampValue1 : "force step ramp value 1"

    //step
    readonly property string qmltextStepAt :qsTr("Step @")
    readonly property string qmltextStepAtTime : qsTr( "Step@Time")
    readonly property string qmltextStepAtEnergy : qsTr("Step@Energy")
    readonly property string qmltextStepAtPKpower : qsTr("Step@Pk Power")
    readonly property string qmltextStepAtCollapse : qsTr("Step@Collapse")
    readonly property string qmltextStepAtAbsolute : qsTr("Step@Absolute")

    readonly property string qmltextTime : "Time"
    readonly property string qmltextEnergy : "Energy"
    readonly property string qmltextPowerPower : "Peak Power"
    readonly property string qmltextAbsoluteDistance : "Absolute Distance"
    readonly property string qmltextCollapseDistance : "Collapse Distance"

    //uints
    readonly property string qmltextUnitSecUpperCase :  "S"
    readonly property string qmltextUnitSecLowerCase :  "s"
    readonly property string qmltextUnitWattUpperCase :  "W"
    readonly property string qmltextUnitWattLowerCase :  "w"
    readonly property string qmltextUnitMillimeter :  "mm"
    readonly property string qmltextUnitIn :  "in"

    readonly property string qmltextnumamplitudesteps :  "numamplitudesteps"
    readonly property string qmltextnumforcesteps :  "numforcesteps"
    readonly property string qmltextamplitudestepat :  "amplitudestepat"
    readonly property string qmltextAmplitudeLowerCase :  "amplitude "
    readonly property string qmltextAmplitudeUpperCase :  qsTr("Amplitude ")
    readonly property string qmltextAmplitudeStepValue :  "amplitude step value "
    readonly property string qmltextforcestepat :  "forcestepat"
    readonly property string qmltextForceStep :  "force step "
    readonly property string qmltextForceStepValue :  "force step value "
    readonly property string qmltextForceStepRampValue :  "force step ramp value "
    readonly property string qmltextStepValue :  qsTr("Step Value")
    readonly property string qmltextStepRamp : qsTr("Step Ramp")
    readonly property string qmltextStepping :qsTr("STEPPING ")
    readonly property string qmltextMin :qsTr("Min : ")
    readonly property string qmltextMax :qsTr("Max : ")
    readonly property string qmltextStackRecipe :qsTr("Stack Recipe")
    readonly property string qmltextBackspace:"backspace"
    readonly property string qmltextClear:"clear"

    //symbols
    readonly property string qmltextHyphen:"-"
    readonly property string qmltextPlus: "+"
    readonly property string qmltextDot:  "."
    readonly property string qmltextBackSpaceSymbol: "⌫"
    readonly property string qmltextClr:  qsTr("Clr")

    //digits on keypad
    readonly property string qmltextDigit1:  "1"
    readonly property string qmltextDigit2:  "2"
    readonly property string qmltextDigit3:  "3"
    readonly property string qmltextDigit4:  "4"
    readonly property string qmltextDigit5:  "5"
    readonly property string qmltextDigit6:  "6"
    readonly property string qmltextDigit7:  "7"
    readonly property string qmltextDigit8:  "8"
    readonly property string qmltextDigit9:  "9"
    readonly property string qmltextDigit0:  "0"

    readonly property string qmltextCancel:  qsTr("CANCEL")
    readonly property string qmltextDone:  qsTr("DONE")

    readonly property int minWidth: 320
    readonly property int maxWidth: 610
    readonly property int minHeight: 460
    readonly property int stepRectangleWidth: 220
    readonly property int stepRectangleHeightOffset: 210
    readonly property int forceRampTimeRectangleWidth: 140
    readonly property int forceRampTimeRectangleHeight: 100
    readonly property int customTextFieldWidth: 210
    readonly property int customTextFieldHeight:40
    readonly property int minXpos: 55
    readonly property int maxXpos: 350
    readonly property int minWidthNumpad: 280
    readonly property int minHeightNumpad: 280
    readonly property int buttonSizeNumpad: 60
    readonly property int numpadXpos: 85
    readonly property int numpadYpos: 40
    property int dialogueModuleIndex:0
    property int dialoguerecipeIndex: 0
    property int focusedItem: 1
    property int ypos: 90
    property int yposFinal
    property int count : 2
    property int loopvar : 2
    property int selectedStep: 1
    property int dwAmpVal: 0
    property int dwAmpStepVal: 0
    property int dwAmpTotalStep: 0
    property int dwAmpStepat: 0
    property int totalRecs : 1
    property int stepAtExSignal: 5
    property int lineEditYPosForStepping: 60
    property int lineEditYPosForNoStepping: 45
    property int recipemofifyFlag: 0

    property alias steppingNumpopup: steppingNumpopup
    property alias lineEditSimple: lineedit
    property alias keyInputText: lineedit.text
    property string parameterText: ""
    property string parameterLocaleText: ""
    property alias unitText: unit.text
    property alias textFieldValidator: lineedit.customTFValidator
    property alias steppingRecVisibility: stepRec.visible
    property alias parameterRecVisibility:parameterRec.visible
    property alias numpadRecXPos: numpadRec.x
    property alias numpadRecYPos: numpadRec.y
    property alias mainRecwidth:mainamprec.width
    property alias numsRec:numsRec.visible
    property alias small1color: small1.color
    property alias nocolor: no.color

    property bool switchStatus: false
    property bool editrecipeflag: false
    property bool addrecipeflag: false
    property bool paramAtoZflag: false
    property bool ultrasonicAdvancedFlag: false
    property bool afterBurst: false
    property bool preTrigger: false
    property bool bransonOnlyFlag: false
    property bool energyBrake: false
    property bool continuosReport: false
    property bool editHornRecipeFlag: false
    property bool editSeekRecipeFlag: false
    property bool isValidateReq: true
    property bool retVal: false
    property bool editStackRecipeFlag: false
    property bool notRestore: false
    property bool refreshSwitch: false

    property var dynamicObject : []
    property var stepAmpValues: [10]
    property var stepValues: [10]
    property var rampTime: [10]

    property string parameterClicked : ""
    property string amplitudeParameter_1: "WELD AMPLITUDE"
    property string amplitudeParameter_2: "Weld Amplitude"

    property string forceParameter_1: "WELD FORCE"
    property string forceParameter_2: "Weld Force"
    property string recipeNumber: "RecipeNumber"
    property string minValue: qmlTextEmpty
    property string maxValue:qmlTextEmpty
    property string paramNameToPasslineedit: ""
    property string paramNameToPasslineeditstepvalue: ""
    property real recipeNumberForParamAtoZ: 0

    signal valueNumpad()
    signal modifiedRecipe(var flagdata);
    signal validateRecipeCopy(var copiedRecipeNumber)
    property var valid1 : RegExpValidator
    {
        regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
    }

    property var valid2 : RegExpValidator
    {
        regExp:  /^((?:[0-9]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-9]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
    }

    readonly property string unitTextColor: "#898D96"
    readonly property string bgColor: "#F8F9FA"
    readonly property string titleBGColor: qmlPrimaryColor
    readonly property string titleColor: "#FFFFFF"

    property real stepSize: 10
    MouseArea{
        anchors.fill: parent
        onClicked: {
        }
    }

    /**
    *@breif: Open the stepping numpad
    **/
    function open()
    {
        steppingNumpopup.visible = true
        focusedItem = 0

    }

    /**
    *@breif: Close the stepping numpad
    **/
    function close()
    {
        parameterLocaleText = qmlTextEmpty
        parameterText = qmlTextEmpty
        keyInputText = qmlTextEmpty
        unitText = qmlTextEmpty
        steppingNumpopup.visible = false
        focusedItem = 0
    }

    /**
    *@breif: When External signal is selected then clears all the steps
    **/
    function deleteStepsAtEXSignal()
    {
        if(totalRecs>=2)
        {
            count = totalRecs+1
            loopvar = totalRecs+1
            var loopCnt = totalRecs
            for( var i=0;i<loopCnt;i++)
            {
                if(totalRecs > 2)
                {
                    deleteDynamicSteps()
                    setDefaultValues(i)
                }
            }
        }
        count = 3
        loopvar = 3
        incMA.enabled = false
        decMA.enabled = false
        disableParameters()
    }

    /**
    *@breif: Enables the selected step
    **/
    function enableStepAtChanged()
    {
        incMA.enabled = true
        decMA.enabled = true
        lineeditstepvalue.text = stepValues[1].toString()
        lineeditstepvalue.readOnly = false
        lineeditRampTime.text = rampTime[1].toString()
        lineeditstepvalue.setBackgroundColor(titleColor)
        lineeditRampTime.readOnly = false
        lineeditRampTime.setBackgroundColor(titleColor)
        lineedit.setBackgroundColor(titleColor)
        lineedit.text = stepAmpValues[1].toString()
        lineedit.readOnly = false
    }

    /**
    *@breif: Sets default values
    **/
    function setDefaultValues(dwIndex)
    {
        var stepAmpValue
        if(notRestore == false)
        {
            if(parameterClicked == amplitudeParameter_1 | parameterClicked == amplitudeParameter_2)
            {
                stepAmpValue = recipe.getDefaultValue(qmltextWeldAmplitudeLowerCase)
            }
            else if(parameterClicked == forceParameter_1 | parameterClicked == forceParameter_2)
            {
                stepAmpValue = recipe.getDefaultValue(qmltextForceStep1)
            }
        }
        var stepValue =qmlTextEmpty
        var forceRampTime = recipe.getDefaultValue(qmltextForceStepRampValue1)
        if(dropdownCustom.currentIndex == -1)
        {
            dropdownCustom.currentIndex = 0;
            return
        }
        var dwStepAtIndex = dropdownCustom.textAt(dropdownCustom.currentIndex)
        if(dwStepAtIndex == qmltextStepAtTime)
        {
            stepValue=recipe.getDefaultValue(qmltextTime)
        }
        else if(dwStepAtIndex ==qmltextStepAtEnergy)
        {
            stepValue=recipe.getDefaultValue(qmltextEnergy)
        }
        else if(dwStepAtIndex == qmltextStepAtPKpower)
        {
            stepValue=recipe.getDefaultValue(qmltextPowerPower)
        }
        else if(dwStepAtIndex == qmltextStepAtCollapse)
        {
            stepValue=recipe.getDefaultValue(qmltextCollapseDistance)
        }
        else if(dwStepAtIndex == qmltextStepAtAbsolute)
        {
            stepValue=recipe.getDefaultValue(qmltextAbsoluteDistance)
        }

        if(dwIndex>=0)
        {
            if(notRestore == false && dwIndex>0)
                stepAmpValues[dwIndex] = stepAmpValue
            stepValues[dwIndex] = stepValue
            rampTime[dwIndex] = forceRampTime
        }
        else
        {
            for(var index = 0 ; index < 10 ; index++ )
            {
                //                if(index)
                if(notRestore == false && index>0)
                    stepAmpValues[index] = stepAmpValue
                stepValues[index] = stepValue
                rampTime[index] = forceRampTime
            }
        }
        if(notRestore == true)
            notRestore = false
    }

    function setStructureWithDefaultValues()
    {
        if( parameterText == amplitudeParameter_1 | parameterText == amplitudeParameter_2 )
        {
            for(var i=0;i<10;i++)
            {
                if(i==0)
                    recipe.setStructureValue(qmltextWeldAmplitudeLowerCase,stepAmpValues[i],0)
                else
                    recipe.setStructureValue(qmltextAmplitudeLowerCase+(i+1),stepAmpValues[i],0)
                recipe.setStructureValue(qmltextAmplitudeStepValue+(i+1),stepValues[i],0)
            }
        }
        if( parameterText == forceParameter_1 | parameterText == forceParameter_2 )
        {
            for(i=0;i<10;i++)
            {
                if(i==0)
                    recipe.setStructureValue(qmltextForceStep1,stepAmpValues[i],0)
                else
                    recipe.setStructureValue(qmltextForceStep+(i+1),stepAmpValues[i],0)
                recipe.setStructureValue(qmltextForceStepValue+(i+1),stepValues[i],0)
                recipe.setStructureValue(qmltextForceStepRampValue+(i+1),rampTime[i],0)
            }
        }
    }

    function changeStepAt()
    {
        if( parameterText == amplitudeParameter_1 | parameterText == amplitudeParameter_2 )
        {
            recipe.setStructureValue(qmltextamplitudestepat,qmlTextEmpty+(dropdownCustom.modeIndex+1),0)
        }
        if( parameterText == forceParameter_1 | parameterText == forceParameter_2 )
        {
            recipe.setStructureValue(qmltextforcestepat,qmlTextEmpty+(dropdownCustom.modeIndex+1),0)
        }
    }

    /**
    *@breif: Validate the values with corresponding values and if they are correct then saves data
    **/
    function validateSteps(stepNumber)
    {
        if(isValidateReq == false)
            return 0
        var returnVal = 0
        var keyValue = qmlTextEmpty
        var validatedvalue=qmlTextEmpty
        if( parameterText == amplitudeParameter_1 | parameterText == amplitudeParameter_2 )
        {
            if(stepNumber == 0 )
                paramNameToPasslineedit = qmltextWeldAmplitudeLowerCase
            else
            {
                paramNameToPasslineedit = qmltextAmplitudeLowerCase+stepNumber
                paramNameToPasslineeditstepvalue = qmltextAmplitudeStepValue+stepNumber
            }
        }
        if( parameterText == forceParameter_1 | parameterText == forceParameter_2 )
        {
            if(stepNumber == 0 )
                paramNameToPasslineedit = qmltextForceStep1
            else
            {
                paramNameToPasslineedit = qmltextForceStep+stepNumber
                paramNameToPasslineeditstepvalue = qmltextForceStepValue+stepNumber
            }
        }

        if(lineedit.readOnly == false)
        {
            keyValue = stepAmpValues[stepNumber-1].toString()
            validatedvalue=recipe.keypadlimitation(parameterText,keyValue)
            if(keyValue != validatedvalue)
            {
                validatedvalue = recipe.getStructureValueByName(paramNameToPasslineedit)
                stepAmpValues[stepNumber-1] = validatedvalue
                lineedit.text = validatedvalue
                returnVal=1
            }
        }

        if(lineeditRampTime.readOnly == false)
        {
            keyValue = rampTime[stepNumber-1].toString()
            validatedvalue=recipe.keypadlimitation(qmltextForceStepRampValue1,keyValue)
            if(keyValue != validatedvalue)
            {
                validatedvalue = recipe.getStructureValueByName(qmltextForceStepRampValue+stepNumber)
                rampTime[stepNumber-1] = validatedvalue
                lineeditRampTime.text = validatedvalue
                returnVal=1
            }
        }
        if(lineeditstepvalue.readOnly == false)
        {
            keyValue = stepValues[stepNumber-1].toString()
            if(stepNumber == 1)
                validatedvalue = keyValue
            else
            {
                var dwStepAtIndex = dropdownCustom.currentIndex
                /*Change in Step@ dropdown list options based on System Type*/
                if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
                {
                    switch(dwStepAtIndex)
                    {
                    case -1:
                    case 0:
                        validatedvalue=recipe.keypadlimitation(qmltextTime,keyValue)
                        break;
                    case 1:
                        validatedvalue=recipe.keypadlimitation(qmltextEnergy,keyValue)
                        break;
                    case 2:
                        validatedvalue=recipe.keypadlimitation(qmltextAbsoluteDistance,keyValue)
                        break;
                    case 3:
                        validatedvalue=recipe.keypadlimitation(qmltextCollapseDistance,keyValue)
                        break;
                    }
                }

                else
                {
                    switch(dwStepAtIndex)
                    {
                    case -1:
                    case 0:
                        validatedvalue=recipe.keypadlimitation(qmltextTime,keyValue)
                        break;
                    case 1:
                        validatedvalue=recipe.keypadlimitation(qmltextEnergy,keyValue)
                        break;
                    case 2:
                        validatedvalue=recipe.keypadlimitation(qmltextPowerPower,keyValue)
                        break;
                    case 3:
                        validatedvalue=recipe.keypadlimitation(qmltextAbsoluteDistance,keyValue)
                        break;
                    case 4:
                        validatedvalue=recipe.keypadlimitation(qmltextCollapseDistance,keyValue)
                        break;
                    }
                }
            }

            if(keyValue != validatedvalue)
            {
                validatedvalue = recipe.getStructureValueByName(paramNameToPasslineeditstepvalue)
                stepValues[stepNumber-1] = validatedvalue
                lineeditstepvalue.text = validatedvalue
                returnVal=1
            }
        }
        return returnVal
    }

    /**
    *@breif: On clicking DONE
    **/
    function onClickAction( RecValue)
    {
        var validateStatus = validateSteps(selectedStep)

        if(validateStatus == 1)
        {
            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
            small1.color=titleColor
            no.color=unitTextColor
        }
        else
        {
            selectedStep = RecValue
            if( RecValue == 2)
            {
                dropdownCustom.enabled = true
                dropdownCustom.setBackgroundColor(titleColor)
                if(dropdownCustom.currentIndex == stepAtExSignal)
                {
                    deleteStepsAtEXSignal()
                }
                else
                {
                    enableStepAtChanged()
                }
                small1.color=titleColor
                no.color=unitTextColor
                changeSelection(2)
            }
            else if ( RecValue == 1 )
            {
                lineedit.readOnly = false
                dropdownCustom.enabled = false
                dropdownCustom.setBackgroundColor("#BEC1C3")
                lineeditstepvalue.readOnly = true
                lineeditstepvalue.text = qmlTextEmpty
                lineeditstepvalue.setBackgroundColor("#BEC1C3")
                lineeditRampTime.readOnly = true
                lineeditRampTime.setBackgroundColor("#BEC1C3")
                lineeditRampTime.text = qmlTextEmpty
                lineedit.text = stepAmpValues[0].toString()
                lineedit.setBackgroundColor(titleColor)
                changeSelection(1)
            }
            else
            {
                dropdownCustom.enabled  = false
                dropdownCustom.setBackgroundColor("#BEC1C3")
                lineeditstepvalue.readOnly = false
                lineeditstepvalue.setBackgroundColor(titleColor)
                lineeditRampTime.readOnly = false
                lineeditRampTime.setBackgroundColor(titleColor)

                switch( RecValue )
                {
                case 3 :
                    lineedit.text = stepAmpValues[2].toString()
                    lineeditstepvalue.text = stepValues[2].toString()
                    lineeditRampTime.text = rampTime[2].toString()
                    small1.color=titleColor
                    no.color=unitTextColor
                    changeSelection(3)
                    break
                case 4:
                    lineedit.text = stepAmpValues[3].toString()
                    lineeditstepvalue.text = stepValues[3].toString()
                    lineeditRampTime.text = rampTime[3].toString()
                    small1.color=titleColor
                    no.color=unitTextColor
                    changeSelection(4)
                    break
                case 5:
                    lineedit.text = stepAmpValues[4].toString()
                    lineeditstepvalue.text = stepValues[4].toString()
                    lineeditRampTime.text = rampTime[4].toString()
                    small1.color=titleColor
                    no.color=unitTextColor
                    changeSelection(5)
                    break
                case 6:
                    lineedit.text = stepAmpValues[5].toString()
                    lineeditstepvalue.text = stepValues[5].toString()
                    lineeditRampTime.text = rampTime[5].toString()
                    small1.color=titleColor
                    no.color=unitTextColor
                    changeSelection(6)
                    break
                case 7:
                    lineedit.text = stepAmpValues[6].toString()
                    lineeditstepvalue.text = stepValues[6].toString()
                    lineeditRampTime.text = rampTime[6].toString()
                    small1.color=titleColor
                    no.color=unitTextColor
                    changeSelection(7)
                    break
                case 8:
                    lineedit.text = stepAmpValues[7].toString()
                    lineeditstepvalue.text = stepValues[7].toString()
                    lineeditRampTime.text = rampTime[7].toString()
                    small1.color=titleColor
                    no.color=unitTextColor
                    changeSelection(8)
                    break
                case 9:
                    lineedit.text = stepAmpValues[8].toString()
                    lineeditstepvalue.text = stepValues[8].toString()
                    lineeditRampTime.text = rampTime[8].toString()
                    small1.color=titleColor
                    no.color=unitTextColor
                    changeSelection(9)
                    break
                case 10:
                    lineedit.text = stepAmpValues[9].toString()
                    lineeditstepvalue.text = stepValues[9].toString()
                    lineeditRampTime.text = rampTime[9].toString()
                    changeSelection(10)
                    break
                }
            }
        }
    }

    /**
    *@breif: When Step@ index is changed
    **/
    function changeSelection(selectedIndex)
    {
        if(selectedIndex ==1)
        {
            small1.color=titleBGColor
            no.color=titleColor
        }
        else
        {
            small1.color=titleColor
            no.color=unitTextColor
        }

        for(var i=2;i<=totalRecs;i++)
        {
            if(selectedIndex==i)
            {
                dynamicObject[i].color = titleBGColor
                dynamicObject[i].textcolor = titleColor
            }
            else
            {
                dynamicObject[i].color = titleColor
                dynamicObject[i].textcolor = unitTextColor
            }
        }
    }

    /**
    *@breif: When numpad is open and checks the parameter name and display corresponding field
    **/
    function checkParameter(parameter)
    {
        //incMA.enabled = false
        //decMA.enabled = false
        if(unitText ==  qmltextUnitSecUpperCase || unitText == qmltextUnitSecLowerCase)
        {
            //textFieldValidator =  valid1
        }

        parameterClicked = parameter
        setDefaultValues(-1)
        //dropdownCustom.modeIndex=0
        if(parameter == amplitudeParameter_1 | parameter == amplitudeParameter_2 | parameter == forceParameter_1 | parameter == forceParameter_2)
        {
            if((parameter == amplitudeParameter_1 | parameter == amplitudeParameter_2) & (config.getFeatureFlagStatus(RuntimeFeatures.UI_AMPLITUDE_STEP) != 1))
            {
                noStepping()
                return
            }

            if((parameter == forceParameter_1 | parameter == forceParameter_2) & (config.getFeatureFlagStatus(RuntimeFeatures.UI_FORCE_STEP) != 1))
            {
                noStepping()
                return
            }

            stepRec.visible = true
            switchItemStepping.checked=true
            switchItemStepping.checked=false
            numsRec.visible = true
            var temptotalRecs=1
            if(parameter == amplitudeParameter_1 | parameter == amplitudeParameter_2)
            {
                temptotalRecs=parseInt(recipe.getStructureValueByName(qmltextnumamplitudesteps))
            }
            else
            {
                temptotalRecs=parseInt(recipe.getStructureValueByName(qmltextnumforcesteps))
            }
            if(temptotalRecs > 1)
            {
                switchItemStepping.checked = true
            }
            else
            {
                switchItemStepping.checked = false
            }
            if(parameter == amplitudeParameter_1 | parameter == amplitudeParameter_2)
            {
                forceRampTime.visible = 0
                dropdownCustom.isDefultValuesReq = false
                dropdownCustom.modeIndex=parseInt(recipe.getStructureValueByName(qmltextamplitudestepat))-1
                dropdownCustom.currentIndex = parseInt(recipe.getStructureValueByName(qmltextamplitudestepat))-1
                dropdownCustom.isDefultValuesReq = true
                var index = 23;
                for(var i=0;i<10;i++)
                {
                    if(i==0)
                        stepAmpValues[i]= recipe.getStructureValueByName(qmltextWeldAmplitudeLowerCase)
                    else
                        stepAmpValues[i]= recipe.getStructureValueByName(qmltextAmplitudeLowerCase+(i+1))
                    stepValues[i]= recipe.getStructureValueByName(qmltextAmplitudeStepValue+(i+1))
                }
            }
            else
            {
                forceRampTime.visible = 1
                dropdownCustom.isDefultValuesReq = false
                dropdownCustom.modeIndex=parseInt(recipe.getStructureValueByName(qmltextforcestepat))-1
                dropdownCustom.currentIndex=parseInt(recipe.getStructureValueByName(qmltextforcestepat))-1
                dropdownCustom.isDefultValuesReq = true
                var forceStepindex = 212;
                var forceStepValindex = 13;
                for(i=0;i<10;i++)
                {
                    if(i==0)
                        stepAmpValues[i]= recipe.getStructureValueByName(qmltextForceStep1)
                    else
                        stepAmpValues[i]= recipe.getStructureValueByName(qmltextForceStep+(i+1))
                    stepValues[i]= recipe.getStructureValueByName(qmltextForceStepValue+(i+1))
                    rampTime[i]= recipe.getStructureValueByName(qmltextForceStepRampValue+(i+1))
                }
            }
            if(totalRecs>temptotalRecs)
            {
                count = totalRecs+1
                loopvar = totalRecs+1
                var loopCnt = totalRecs-temptotalRecs
                for(i=0;i<loopCnt;i++)
                {
                    deleteDynamicSteps()
                }
            }
            else
            {
                count = totalRecs+1
                loopvar = totalRecs+1
                loopCnt = temptotalRecs-totalRecs
                for(i=0;i<loopCnt;i++)
                {
                    isValidateReq = false
                    createDynamicSteps()
                    isValidateReq = true
                }
            }

            if ( switchItemStepping.checked == true)
            {
                numpadRec.y = lineEditYPosForStepping
                if( (mainamprec.width == maxWidth) || ( mainamprec.width == minWidth ))
                {
                    parameterRec.visible = true
                    mainamprec.width= maxWidth
                    numpadRec.x = maxXpos
                    numsRec.visible = true

                    while((smallinc.y + 30 * 1.5) > (mainamprec.height - 30 * 1.5))
                    {
                        mainamprec.height += 30 * 1.5
                    }

                }
            }
            else
            {
                mainamprec.width =  minWidth
                numpadRec.x = minXpos
                numpadRec.y = lineEditYPosForStepping
                numsRec.visible = false
                parameterRec.visible = false
                forceRampTime.visible = false
            }
            onClickAction( 1 )
        }
        else
        {
            noStepping()
        }
    }

    /**
    *@breif: When steeping not there for the selected parameter name
    **/
    function noStepping()
    {
        stepRec.visible = false
        parameterRec.visible = false
        numsRec.visible = false
        forceRampTime.visible = false
        numpadRec.y = lineEditYPosForNoStepping
        mainamprec.width =  minWidth
        mainamprec.height = minHeight
        numpadRec.x = minXpos
    }

    /**
    *@breif: Ir parameter is  not weld force or weld amplitude then it disable some of teh fields
    **/
    function disableParameters()
    {

        lineedit.readOnly = true
        lineedit.text = qmlTextEmpty
        lineedit.setBackgroundColor("#BEC1C3")
        lineeditstepvalue.readOnly = true
        lineeditstepvalue.text = qmlTextEmpty
        lineeditstepvalue.setBackgroundColor("#BEC1C3")
        lineeditRampTime.readOnly = true
        lineeditRampTime.setBackgroundColor("#BEC1C3")
        lineeditRampTime.text = qmlTextEmpty
    }

    /**
    *@breif: Disables some of the parameter name
    **/
    function disabled(operator)
    {
        if (keyInputText == qmlTextEmpty && !((operator >= "0" && operator <= "9") || operator == "."))
            return true
        else if (operator == "." && keyInputText.toString().search(/\./) != -1)
        {
            return true
        }
        else
        {
            return false
        }
    }

    /**
    *@breif: When backspace is pressed
    **/
    function operatorPressed(operator)
    {
        if (disabled(operator))
        {
            return
        }

        if (operator == qmltextBackspace)
        {
            switch(focusedItem)
            {
            case 0:
                lineedit.text = qmlTextEmpty
                break;
            case 1 :
                var digits4 = lineedit.text
                digits4 =  digits4.toString().slice(0, -1)
                lineedit.text = digits4
                break
            case 2:
                var digits2 = lineeditstepvalue.text
                digits2 =  digits2.toString().slice(0, -1)
                lineeditstepvalue.text = digits2
                break
            case 3:
                var digits3 = lineeditRampTime.text
                digits3 = digits3.toString().slice(0, -1)
                lineeditRampTime.text = digits3
                break
            }
        }
    }

    /**
    *@breif: When clr is pressed
    **/
    function clearPressed( clearop )
    {
        if (clearop == qmltextClear)
        {
            switch(focusedItem)
            {
            case 1 :
                lineedit.text = qmlTextEmpty
                break
            case 2:
                lineeditstepvalue.text = qmlTextEmpty
                break
            case 3:
                lineeditRampTime.text = qmlTextEmpty
                break
            }
        }
    }

    /**
    *@breif: When zero is pressed
    **/
    function zeroPressed( zeroDigit )
    {
        switch(focusedItem)
        {
        case 1:
            if(lineedit.readOnly==false && lineedit.text.length<7)
                lineedit.text += zeroDigit
            break
        case 2:
            if(lineeditstepvalue.readOnly==false && lineeditstepvalue.text.length<7)
                lineeditstepvalue.text += zeroDigit
            break
        case 3:
            if(lineeditRampTime.readOnly==false && lineeditRampTime.text.length<5)
                lineeditRampTime.text += zeroDigit
            break
        }
    }

    /**
    *@breif: Valiadte the data based  on the unit and save into the structure if it is correct
    **/
    function isValidData(textInput)
    {
        var validateSec = /^\d{0,6}?$/;
        if(unitText == qmltextUnitSecLowerCase || unitText ==  qmltextUnitSecUpperCase)
        {
            validateSec = /^\d{0,2}(\.\d{0,3})?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == qmltextUnitMillimeter|| unitText == "mm/s2" || unitText == "mm/s")
        {
            if(unitText == qmltextUnitMillimeter)
            {
                /*For Collapse Distance, Absolute Distance, Pretrigger Distance and Ready Offset precision handling
                  while data being entered*/
                if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
                {
                    if(dialog.text === qsTr("ABSOLUTE DISTANCE") || dialog.text === qsTr("PRETRIGGER DISTANCE")
                            || dialog.text === qsTr("READY OFFSET"))
                    {
                        validateSec =/^\d{0,3}(\.\d{0,2})?$/;
                    }

                    else
                    {
                        validateSec =/^\d{0,2}(\.\d{0,2})?$/;
                    }
                }

                else
                {
                    if(dialog.text === qsTr("ABSOLUTE DISTANCE") || dialog.text === qsTr("PRETRIGGER DISTANCE")
                            || dialog.text === qsTr("READY OFFSET"))
                    {
                        validateSec =/^\d{0,3}(\.\d{0,3})?$/;
                    }

                    else
                    {
                        validateSec =/^\d{0,2}(\.\d{0,3})?$/;
                    }
                }
            }
            else
            {
                validateSec = /^\d{0,3}(\.\d{0,2})?$/;
            }
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == qmltextUnitIn || unitText == "in/s2" || unitText == "in/s")
        {
            validateSec = /^\d{0,2}(\.\d{0,4})?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == qmltextUnitWattLowerCase || unitText == qmltextUnitWattUpperCase)
        {
            validateSec = /^\d{0,4}?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == "J"|| unitText =="j")
        {
            validateSec = /^\d{0,6}(\.\d{0,1})?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == "%")
        {
            validateSec = /^\d{0,3}?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        /*recipe screen force parameter numpad restriction on the basis of system unit */
        else if(unitText == "lbs" || unitText =="Lbs" || unitText =="LBS")
        {
            validateSec = /^\d{0,3}(\.\d{0,1})?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else
        {
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
    }

    /**
    *@breif: When digit is pressed
    **/
    function digitPressed(digit)
    {
        if (disabled(digit))
        {
            return
        }

        if(focusedItem == 0)
        {
            lineedit.text = qmlTextEmpty
            focusedItem = 1
        }

        switch(focusedItem)
        {
        case 1:
            var textInput = lineedit.text
            textInput += digit
            if(isValidData(textInput) == 1)
                lineedit.text = textInput

            break
        case 2:
            if(lineeditstepvalue.readOnly==false && lineeditstepvalue.text.length<8)
                lineeditstepvalue.text += digit
            break
        case 3:
            if(lineeditRampTime.readOnly==false && lineeditRampTime.text.length<6)
                lineeditRampTime.text += digit
            break
        }
    }

    /**
    *@breif: Creates dynamic steps on clicking "+"
    **/
    function createDynamicSteps()
    {
        var validateStatus=validateSteps(selectedStep)
        if(validateStatus == 1)
        {
            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
            return;
        }
        //lineedit.text = qmlTextEmpty
        var component;
        /*Stepping adjustment*/
        if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
        {
            if( parameterText == forceParameter_1 | parameterText == forceParameter_2 )
            {
                stepSize = 3
            }
            else
            {
                stepSize = 10
            }
        }

        else
            stepSize = 10

        if( totalRecs <= (stepSize-1))
        {
            while ( loopvar == count )
            {
                component = Qt.createComponent("DynamicRec.qml");
                dynamicObject[count] = component.createObject(numsRec, {"anchors.horizontalCenter": numsRec.horizontalCenter,
                                                                        "y": ypos,
                                                                        "width" : 30 * 1.5,
                                                                        "height" : 30 * 1.5,
                                                                        });

                dynamicObject[count].textvalue = count
                dynamicObject[count].fontvalue = Style.style5
                ypos = ypos + 30 * 1.5
                yposFinal = ypos
                smallinc.y = ypos
                if((yposFinal + 30 * 1.5) > (minHeight - 30 * 1.5))
                {
                    mainamprec.height += 30 * 1.5
                }

                if( count == 2)
                {
                    dropdownCustom.enabled = true
                    dropdownCustom.setBackgroundColor(titleColor)
                    lineeditstepvalue.readOnly = false
                    lineeditstepvalue.setBackgroundColor(titleColor)
                    lineeditRampTime.readOnly = false
                    lineeditRampTime.setBackgroundColor(titleColor)
                }
                else
                {
                    dropdownCustom.enabled  = false
                    dropdownCustom.setBackgroundColor("#BEC1C3")
                    lineeditstepvalue.readOnly = false
                    lineeditstepvalue.setBackgroundColor(titleColor)
                    lineeditRampTime.readOnly = false
                    lineeditRampTime.setBackgroundColor(titleColor)
                }
                if( loopvar == count )
                {
                    totalRecs++
                    isValidateReq = false
                    onClickAction( totalRecs )
                    isValidateReq = true
                    count = count+1
                    loopvar = loopvar+1
                    break
                }
            }
        }
    }

    /**
    *@breif: Delete dynamic steps on clicking "-"
    **/
    function deleteDynamicSteps()
    {
        if( totalRecs >= 2)
        {
            for( var i = count ; i >=2 ; i-- )
            {
                dynamicObject[i-1].destroy()
                setDefaultValues(i-1)
                ypos = ypos - 30 * 1.5
                smallinc.y = ypos
                if((smallinc.y + 30 * 1.5) > (minHeight - 30 * 1.5))
                {
                    mainamprec.height -= 30 * 1.5
                }
                else
                {
                    mainamprec.height = minHeight
                }

                if( (count-1) == 3)
                {
                    dropdownCustom.enabled  = true
                    dropdownCustom.setBackgroundColor(titleColor)
                    lineeditstepvalue.readOnly = false
                    lineeditstepvalue.setBackgroundColor(titleColor)
                    lineeditRampTime.readOnly = false
                    lineeditRampTime.setBackgroundColor(titleColor)
                }
                else
                {
                    dropdownCustom.enabled = false
                    dropdownCustom.setBackgroundColor("#BEC1C3")
                    lineeditstepvalue.readOnly = false
                    lineeditstepvalue.setBackgroundColor(titleColor)
                    lineeditRampTime.readOnly = false
                    lineeditRampTime.setBackgroundColor(titleColor)
                }
                if( (count-1) == 2)
                {
                    lineeditstepvalue.readOnly = true
                    lineeditstepvalue.setBackgroundColor("#BEC1C3")
                    lineeditRampTime.readOnly = true
                    lineeditRampTime.setBackgroundColor("#BEC1C3")
                }

                if( loopvar == count )
                {
                    totalRecs--
                    setDefaultValues(totalRecs)
                    isValidateReq = false
                    onClickAction( totalRecs)
                    isValidateReq = true
                    count = count-1
                    loopvar = loopvar-1
                    break
                }
            }
        }
    }

    Component.onCompleted:
    {
        setDefaultValues(-1)
    }

    Rectangle {
        id: mainamprec
        width: minWidth
        height: minHeight
        x: window.width/2 - width/2
        y:window.height/2 - height/2
        color: bgColor
        Rectangle{
            id: amplitudeTextRect
            width: parent.width
            height: 30
            anchors.top:parent.top
            anchors.topMargin: -30
            radius: 0
            color: titleBGColor
            Text {
                id: dialog
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: parameterLocaleText
                color: titleColor
                font.family: Style.regular.name
                font.pixelSize: Style.style4
                anchors.verticalCenter: parent.verticalCenter
                font.capitalization: Font.AllUppercase
            }
            Image {
                id: closeImage
                sourceSize.width: parent.width*(10/100)
                sourceSize.height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5
                /*Image icon update based on System Type*/
                source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/close_ASX.svg" :
                                                                   "qrc:/images/image/close.svg"
            }
            MouseArea{
                id: closeImageMA
                width: parent.width*(10/100)
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5
                onClicked: {
                    if( switchStatus == true)
                    {
                        numpadRec.x = maxXpos
                        parameterRec.visible = true
                        if( mainamprec.width == maxWidth )
                        {
                            mainamprec.width = maxWidth
                        }
                        else if ( mainamprec.width == minWidth )
                        {
                            mainamprec.width = minWidth
                        }
                    }
                    else
                    {
                        numpadRec.x = minXpos
                        parameterRec.visible = false
                    }
                    small1.color=titleBGColor
                    no.color=titleColor
                    setAllFlagsToFalse()
                    steppingNumpopup.close()
                    disableMainwindowOpacity.start()
                }
            }
        }
        Rectangle{
            id:stepRec
            width: parent.width*(30/100)
            height: parent.height*(9/100)
            anchors.top: amplitudeTextRect.bottom
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            color:bgColor
            visible: true
            Text {
                id: steppingText
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: qmltextStepping
                color: unitTextColor
                anchors.verticalCenter: parent.verticalCenter
                font.family: Style.semibold.name
                font.pixelSize: Style.style5
            }
            ToogleBar {
                id: switchItemStepping
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width/2-width/2
                y: parent.height / 2 - height / 2

                anchors.left: steppingText.right
                anchors.leftMargin: 30

                Component.onCompleted: {
                    parameterRec.visible = false
                    numpadRec.x = minXpos
                }
                onPressedChanged:
                {
                    refreshSwitch = true
                }
                onCheckedChanged:
                {
                    if(refreshSwitch == false)
                        return;
                    refreshSwitch = false;
                    if(checked == true)
                    {
                        switchStatus = true
                        parameterRec.visible = true
                        numpadRec.x = maxXpos
                        mainamprec.width = steppingNumpopup.maxWidth
                        decMA.enabled = true
                        incMA.enabled = true
                        numsRec.visible = true
                        lineeditstepvalue.text = qmlTextEmpty
                        lineeditstepvalue.readOnly = true
                        lineeditstepvalue.setBackgroundColor("#BEC1C3")
                        stepAmpValues[0] = lineedit.text
                        if( parameterClicked == forceParameter_1 | parameterClicked == forceParameter_2)
                        {
                            forceRampTime.visible = true
                        }
                        else
                        {
                            forceRampTime.visible = false
                        }
                    }
                    else if(checked == false)
                    {
                        switchStatus = false
                        parameterRec.visible = false
                        forceRampTime.visible = false
                        numpadRec.x = minXpos
                        mainamprec.width = minWidth
                        lineeditstepvalue.readOnly = true
                        lineeditstepvalue.setBackgroundColor("#BEC1C3")
                        if(totalRecs>=2)
                        {
                            count = totalRecs+1
                            loopvar = totalRecs+1
                            var loopCnt = totalRecs
                            for( var i=0;i<loopCnt;i++)
                            {
                                deleteDynamicSteps()
                                dropdownCustom.currentIndex = 0
                                setDefaultValues(i)
                                setStructureWithDefaultValues()
                            }
                            lineedit.text = stepAmpValues[0]
                        }
                        decMA.enabled = false
                        incMA.enabled = false
                        numsRec.visible = false
                    }
                }
            }
        }

        Rectangle{
            id: numsRec
            property int fontSize: Style.style5
            width: parent.width*(10/100)
            height: mainamprec.height - stepRec.height - amplitudeTextRect.height
            anchors.top: stepRec.bottom
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            color: bgColor
            Rectangle{
                id:smalldec
                width: 30 * 1.5
                height: /*parent.height*(5/100)//*/30 * 1.5
                anchors.horizontalCenter:  parent.horizontalCenter
                anchors.top:numsRec.top
                anchors.topMargin: 2
                border.color: "#BEC1C3"
                color: "#ECECEC"
                Text
                {
                    id: dec
                    text: qmltextHyphen
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: Style.regular.name
                    font.pixelSize: numsRec.fontSize
                    color: unitTextColor
                }
                MouseArea {
                    id:decMA
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: false
                    onClicked:
                    {
                        deleteDynamicSteps()
                    }
                }
            }
            Rectangle{
                id:small1
                width: 30 * 1.5
                height: 30 * 1.5
                anchors.horizontalCenter:  parent.horizontalCenter
                anchors.top:smalldec.bottom
                anchors.topMargin: 0
                border.color: "#BEC1C3"
                color: titleBGColor
                Text
                {
                    id:no
                    text:qmltextDigit1
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: Style.regular.name
                    font.pixelSize: numsRec.fontSize
                    color: titleColor
                }
                MouseArea{
                    id : small1MA
                    width: 30 * 1.5
                    height: 30 * 1.5
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        small1.color=titleBGColor
                        no.color=titleColor
                        onClickAction(no.text)
                    }
                }
            }
            Rectangle{
                id:smallinc
                width: 30 * 1.5
                height: /*parent.height*(5/100)//*/30 * 1.5
                anchors.horizontalCenter:  parent.horizontalCenter
                y:ypos
                border.color: "#BEC1C3"
                color: "#ECECEC"
                Text
                {
                    id: inc
                    text:qmltextPlus
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: Style.regular.name
                    font.pixelSize: numsRec.fontSize
                    color: unitTextColor
                }
                MouseArea {
                    id:incMA
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: false
                    onClicked:
                    {
                        createDynamicSteps()
                    }
                }
            }
        }

        Rectangle{
            id: parameterRec
            width: stepRectangleWidth
            height: mainamprec.height - stepRec.height - amplitudeTextRect.height - stepRectangleHeightOffset
            anchors.top: stepRec.bottom
            anchors.topMargin: 95
            anchors.left: numsRec.right
            anchors.leftMargin: 10
            border.color: bgColor
            color: bgColor
            visible: true

            Text {
                id: step
                anchors.left: parent.left
                anchors.leftMargin: 5
                text: qmltextStepAt
                color: unitTextColor
                anchors.top: parent.top
                anchors.topMargin: 0
                font.family: Style.regular.name
                font.pixelSize: Style.style5
            }
            ComboBoxCustomNumpad{
                id: dropdownCustom
                width: customTextFieldWidth
                height: customTextFieldHeight
                anchors.top:step.bottom
                anchors.topMargin: 3
                anchors.left: parent.left
                anchors.leftMargin: 5//parent.width*(27/100)
                fontSize: Style.style5
                Component.onCompleted: {
                    setBackgroundColor("#BEC1C3")
                }
            }
            Text {
                id: stepvalue
                anchors.left: parent.left
                anchors.leftMargin: 5//parent.width*(27/100)
                text:qmltextStepValue
                color: unitTextColor
                anchors.top: step.bottom
                anchors.topMargin: 55
                font.family: Style.regular.name
                font.pixelSize: Style.style5
            }

            CustomTextField
            {
                id: lineeditstepvalue
                width: customTextFieldWidth
                height: customTextFieldHeight
                anchors.top:stepvalue.bottom
                anchors.topMargin: 3
                anchors.left: parent.left
                anchors.leftMargin: 5
                fontSize: Style.style5
                Component.onCompleted: {
                    setBackgroundColor("#BEC1C3")
                    readOnlyMode = true
                }
                validator: RegExpValidator
                {
                    regExp:  /^\d{0,6}?$/
                }
                onTextChanged: {
                    switch(selectedStep)
                    {
                    case 1:
                        stepValues[0] = lineeditstepvalue.text
                        break
                    case 2:
                        stepValues[1] = lineeditstepvalue.text
                        break
                    case 3:
                        stepValues[2] = lineeditstepvalue.text
                        break
                    case 4:
                        stepValues[3] = lineeditstepvalue.text
                        break
                    case 5:
                        stepValues[4] = lineeditstepvalue.text
                        break
                    case 6:
                        stepValues[5] = lineeditstepvalue.text
                        break
                    case 7:
                        stepValues[6] = lineeditstepvalue.text
                        break
                    case 8:
                        stepValues[7] = lineeditstepvalue.text
                        break
                    case 9:
                        stepValues[8] = lineeditstepvalue.text
                        break
                    case 10:
                        stepValues[9] = lineeditstepvalue.text
                        break
                    }
                }
                onFocusChanged:
                {
                    focusedItem = 2
                    //activeFocusOnPress ? true : false
                    //                    if(activeFocusOnPress)
                    //                    {
                    //                        focusedItem = 2
                    //                    }
                }
            }
        }
        Rectangle{
            id: forceRampTime
            width: forceRampTimeRectangleWidth
            height: forceRampTimeRectangleHeight
            anchors.top: parameterRec.bottom
            anchors.topMargin: -10
            anchors.left: parameterRec.left
            anchors.leftMargin: 5
            border.color: bgColor
            color: bgColor
            visible: false
            Text {
                id: rampTimevalue
                anchors.left: parent.left
                anchors.leftMargin: 0
                text: qmltextStepRamp
                color: unitTextColor
                anchors.topMargin: 5
                anchors.top: parent.top
                font.family: Style.regular.name
                font.pixelSize: Style.style1
            }
            CustomTextField
            {
                id: lineeditRampTime
                width: customTextFieldWidth
                height: customTextFieldHeight
                anchors.top:rampTimevalue.bottom
                anchors.topMargin: 3
                anchors.left: parent.left
                anchors.leftMargin: 0
                validator: RegExpValidator
                {
                    regExp:  /^\d{0,6}?$/
                }
                Component.onCompleted: {
                    setBackgroundColor("#BEC1C3")
                    readOnlyMode = true
                }
                onTextChanged: {
                    switch(selectedStep)
                    {
                    case 1:
                        rampTime[0] = lineeditRampTime.text
                        break
                    case 2:
                        rampTime[1] = lineeditRampTime.text
                        break
                    case 3:
                        rampTime[2] = lineeditRampTime.text
                        break
                    case 4:
                        rampTime[3] = lineeditRampTime.text
                        break
                    case 5:
                        rampTime[4] = lineeditRampTime.text
                        break
                    case 6:
                        rampTime[5] = lineeditRampTime.text
                        break
                    case 7:
                        rampTime[6] = lineeditRampTime.text
                        break
                    case 8:
                        rampTime[7] = lineeditRampTime.text
                        break
                    case 9:
                        rampTime[8] = lineeditRampTime.text
                        break
                    case 10:
                        rampTime[9] = lineeditRampTime.text
                        break
                    }
                }
                onFocusChanged:
                {
                    focusedItem = 3
                }
            }
        }

        Rectangle{
            id:numpadRec
            x: numpadXpos
            y: numpadYpos
            width: mainamprec.width - parameterRec.width - numsRec.width - 80
            height: parent.height - stepRec.height - 50 - amplitudeTextRect.height
            color: bgColor
            border.color: bgColor
            CustomTextField
            {
                id: lineedit
                width: customTextFieldWidth - 20
                height: customTextFieldHeight
                anchors.top:parent.top
                anchors.topMargin: -20
                anchors.left: parent.left
                anchors.leftMargin: 12
                inputMethodHints:Qt.ImhFormattedNumbersOnly
                onlyForNumPad: true
                fontSize: Style.style5
                Component.onCompleted:
                {
                    setBackgroundColor(titleColor)
                }
                onTextChanged:
                {
                    Qt.inputMethod.hide();
                    switch(selectedStep)
                    {
                    case 1:
                        stepAmpValues[0] = lineedit.text
                        break
                    case 2:
                        stepAmpValues[1] = lineedit.text
                        break
                    case 3:
                        stepAmpValues[2] = lineedit.text
                        break
                    case 4:
                        stepAmpValues[3] = lineedit.text
                        break
                    case 5:
                        stepAmpValues[4] = lineedit.text
                        break
                    case 6:
                        stepAmpValues[5] = lineedit.text
                        break
                    case 7:
                        stepAmpValues[6] = lineedit.text
                        break
                    case 8:
                        stepAmpValues[7] = lineedit.text
                        break
                    case 9:
                        stepAmpValues[8] = lineedit.text
                        break
                    case 10:
                        stepAmpValues[9] = lineedit.text
                        break
                    }
                }
                onSingleClick:
                {
                    focusedItem = 0
                }
                onDoubleClick:
                {
                    focusedItem = 1
                }

                Text {
                    id: unit
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    text: qsTr("%")
                    color: unitTextColor
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Style.regular.name
                    font.pixelSize: Style.style3
                }
            }
            Text{
                id:minDisplay
                x:lineedit.x
                text:qmltextMin + minValue
                color: titleBGColor
                font.family: Style.regular.name
                font.pixelSize: Style.style3
                anchors.top:lineedit.bottom
                anchors.topMargin: 5/*
                anchors.left:parent.left
                anchors.leftMargin: 12*/
            }
            Text{
                id:maxDisplay
                text:qmltextMax + maxValue
                x: (lineedit.x + (lineedit.width*60/100))
                color: titleBGColor
                font.family: Style.regular.name
                font.pixelSize: Style.style3
                anchors.top:lineedit.bottom
                anchors.topMargin: 5/*
                anchors.left:parent.right
                anchors.leftMargin: 30*/
            }

            Grid {
                id:numGrid
                columns: 3
                columnSpacing: 10
                rowSpacing: 10
                anchors.top: lineedit.bottom
                anchors.topMargin: 38
                anchors.left:parent.left
                anchors.leftMargin: -30
                width: minWidthNumpad - buttonSizeNumpad - 20
                height: minHeightNumpad

                PrimaryIconButton
                {
                    id:text7
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text:qmltextDigit1

                }

                PrimaryIconButton
                {
                    id:text8
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit2
                }

                PrimaryIconButton
                {
                    id:text9
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit3
                }

                PrimaryIconButton
                {
                    id:text4
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit4
                }

                PrimaryIconButton
                {
                    id:text5
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit5
                }

                PrimaryIconButton
                {
                    id:text6
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit6
                }

                PrimaryIconButton
                {
                    id:text1
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit7
                }

                PrimaryIconButton
                {
                    id:text2
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text:qmltextDigit8
                }

                PrimaryIconButton
                {
                    id:text3
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit9
                }

                PrimaryIconButton
                {
                    id:textdot
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDot
                }
            }
            SecondaryButton {
                id: deleteButton
                text: qmltextBackSpaceSymbol
                fontSize: Style.style8
                anchors.left: numGrid.right
                anchors.leftMargin: 10
                anchors.top: lineedit.bottom
                anchors.topMargin: 38
                width: textdot.width
                height: textdot.height*3 + 20
                textColor: titleBGColor
                onClicked: {
                    operatorPressed(qmltextBackspace)
                }
            }
            PrimaryIconButton {
                id: customButton
                maxHeight: buttonSizeNumpad
                maxWidth: buttonSizeNumpad
                fontSize: Style.style5
                name: qmltextClr
                anchors.left: deleteButton.left
                anchors.top:deleteButton.bottom
                anchors.topMargin: 10
                onClicked: {
                    clearPressed(qmltextClear)
                }
            }
            PrimaryIconButton {
                id: zeroButton
                text:qmltextDigit0
                maxHeight: buttonSizeNumpad
                maxWidth: buttonSizeNumpad
                fontSize: Style.style5
                width: buttonSizeNumpad * 2 + 10
                anchors.right: customButton.left
                anchors.rightMargin: 10
                anchors.top:deleteButton.bottom
                anchors.topMargin: 10
            }
            SecondaryButton {
                id: set
                anchors.top: zeroButton.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: -42
                fontSize: Style.style5
                text: qmltextCancel
                onClicked: {
                    if( switchStatus == true)
                    {
                        numpadRec.x = maxXpos
                        parameterRec.visible = true
                        if( mainamprec.width == steppingNumPad.maxWidth )
                        {
                            mainamprec.width = steppingNumPad.maxWidth
                        }
                        else if ( mainamprec.width == steppingNumPad.minWidth )
                        {
                            mainamprec.width = steppingNumPad.minWidth //mainamprec.width+150
                        }
                    }
                    else
                    {
                        parameterRec.visible = false
                        numpadRec.x = minXpos
                    }
                    setAllFlagsToFalse()
                    steppingNumpopup.close()
                    disableMainwindowOpacity.start()
                }
            }
            PrimaryButton {
                id: setandcontinue
                anchors.top: zeroButton.bottom
                anchors.topMargin: 20
                anchors.left: set.right
                anchors.leftMargin: 12
                fontSize: Style.style5
                text: qmltextDone

                onClicked: {
                    validateRecipeCopy(keyInputText)
                    var validatedvalue=qmlTextEmpty
                    var dwResult = 0
                    if(ultrasonicAdvancedFlag)
                    {
                        validatedvalue = ultrasonictest.rangeCheck(keyInputText, parameterText)
                        if(validatedvalue != keyInputText)
                        {
                            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
                            keyInputText = validatedvalue
                        }
                        else
                        {
                            ultrasonictest.modifyParameter(keyInputText, parameterText);
                            setAllFlagsToFalse()
                            steppingNumpopup.close()
                            disableMainwindowOpacity.start()
                        }
                        return
                    }
                    if(editHornRecipeFlag)
                    {
                        validatedvalue = hornRecipeObj.rangeCheck(keyInputText, parameterText)
                        if(validatedvalue != keyInputText)
                        {
                            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueCorrectMsg)
                            keyInputText = validatedvalue
                        }
                        else
                        {
                            hornRecipeObj.modifyParameter(keyInputText , parameterText);
                            setAllFlagsToFalse()
                            steppingNumpopup.close()
                            disableMainwindowOpacity.start()
                        }
                        return
                    }
                    if(editSeekRecipeFlag)
                    {
                        validatedvalue = seekRecipeObj.rangeCheck(keyInputText , parameterText)
                        if(validatedvalue != keyInputText)
                        {
                            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
                            keyInputText = validatedvalue
                        }
                        else
                        {
                            seekRecipeObj.modifyParameter(keyInputText , parameterText);
                            setAllFlagsToFalse()
                            steppingNumpopup.close()
                            disableMainwindowOpacity.start()
                        }
                        return
                    }
                    if(editStackRecipeFlag)
                    {
                        validatedvalue = StackRecipeObj.rangeCheck(keyInputText , dialoguerecipeIndex)
                        if(validatedvalue != 0)
                        {
                            displayQuitPopup(qmltextStackRecipe, qmltextInvalidValueMsg)
                            //keyInputText = validatedvalue
                        }
                        else
                        {
                            StackRecipeObj.modifyParameter(keyInputText, dialoguerecipeIndex);
                            recipe.setParamaterChangedFlag(1);
                            recipe.setStructureValue("","");
                            setAllFlagsToFalse()
                            steppingNumpopup.close()
                            disableMainwindowOpacity.start()
                        }
                        return
                    }
                    validatedvalue=keyInputText
                    if( !editStackRecipeFlag && !ultrasonicAdvancedFlag && !editHornRecipeFlag && !editSeekRecipeFlag)
                    {
                        var showText = parameterText
                        if(showText == "abs distance")
                            showText = "absolute distance"
                        if(showText == "col distance")
                            showText = "collapse distance"

                        validatedvalue = recipe.paramRangeValidation(parameterText,keyInputText)//recipe.keypadlimitation(parameterText,keyInputText)
                    }
                    if(validatedvalue != keyInputText)
                    {
                        displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
                        keyInputText = validatedvalue
                        return;
                    }
                    if( parameterText == amplitudeParameter_1 | parameterText == amplitudeParameter_2 | parameterText == forceParameter_1 | parameterText == forceParameter_2)
                    {
                        var validateStatus = validateSteps(selectedStep)

                        if(validateStatus == 1)
                        {
                            displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
                            return;
                        }
                    }

                    selectedStep=1
                    if( parameterText == amplitudeParameter_1 | parameterText == amplitudeParameter_2 )
                    {
                        keyInputText = stepAmpValues[0]
                        recipe.setStructureValue(qmltextamplitudestepat,qmlTextEmpty+(dropdownCustom.modeIndex+1),0)
                        recipe.setStructureValue(qmltextnumamplitudesteps,qmlTextEmpty+(totalRecs),0)
                        for(var i=0;i<10;i++)
                        {
                            if(i==0)
                                recipe.setStructureValue(qmltextWeldAmplitudeLowerCase,stepAmpValues[i],0)
                            else
                                recipe.setStructureValue(qmltextAmplitudeLowerCase+(i+1),stepAmpValues[i],0)
                            recipe.setStructureValue(qmltextAmplitudeStepValue+(i+1),stepValues[i],0)
                        }
                    }
                    if( parameterText == forceParameter_1 | parameterText == forceParameter_2 )
                    {
                        keyInputText = stepAmpValues[0]
                        recipe.setStructureValue(qmltextforcestepat,qmlTextEmpty+(dropdownCustom.modeIndex+1),0)
                        recipe.setStructureValue(qmltextnumforcesteps,qmlTextEmpty+(totalRecs),0)
                        for(i=0;i<10;i++)
                        {
                            if(i==0)
                                recipe.setStructureValue(qmltextForceStep1,stepAmpValues[i],0)
                            else
                                recipe.setStructureValue(qmltextForceStep+(i+1),stepAmpValues[i],0)
                            recipe.setStructureValue(qmltextForceStepValue+(i+1),stepValues[i],0)
                            recipe.setStructureValue(qmltextForceStepRampValue+(i+1),rampTime[i],0)
                        }
                    }
                    validatedvalue=keyInputText
                    if(validatedvalue == keyInputText)
                    {
                        if(bransonOnlyFlag)
                            recipe.setParamaterChangedFlag(4)
                        else if(editrecipeflag || paramAtoZflag || afterBurst || preTrigger || energyBrake || addrecipeflag)
                            recipe.setParamaterChangedFlag(1)
                        recipe.setStructureValue(parameterText,keyInputText)
                        setAllFlagsToFalse()
                        steppingNumpopup.close()
                        disableMainwindowOpacity.start()
                    }
                    else
                    {
                        setAllFlagsToFalse()
                        disableMainwindowOpacity.start()
                        displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
                        keyInputText = validatedvalue
                    }
                }
            }
        }
    }
    function setAllFlagsToFalse()
    {
        editrecipeflag = false
        addrecipeflag = false
        paramAtoZflag = false
        editHornRecipeFlag = false
        editSeekRecipeFlag = false
        ultrasonicAdvancedFlag=false
        afterBurst = false
        preTrigger = false
        editStackRecipeFlag = false
        bransonOnlyFlag = false
        energyBrake = false
        continuosReport = false
    }
}
