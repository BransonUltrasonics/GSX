/*
 * SuspectRejectNumpad.qml
 *
 *  Created on: FEB 05, 2018
 *
 */
import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Universal 2.0
import Style 1.0

Rectangle {
    id: suspectRejectNum
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false
    readonly property string qmltextEneterValidLimitsMsg : qsTr("Please enter valid limits to continue")
    readonly property string qmltextEneterValidRangeLimitsMsg : qsTr("Please enter limits within range to continue")
    readonly property string qmltextEneterValidLimitsSetupMsg : qsTr("Setup/Control limits for this parameter is turning Off as the values entered are not maching with them.")
    readonly property string qmltextDecimalNotAcceptMsg : qsTr("Decimal is not acceptable for this parameter. Please enter valid limits to continue")
    property alias suspectRejectNum: suspectRejectNum.visible
    readonly property int minWidth: 430
    readonly property int minHeight: 500
    readonly property int minWidthNumpad: 280
    readonly property int minHeightNumpad: 280
    readonly property int buttonSizeNumpad: 60
    readonly property string unitTextColor: "#898D96"
    readonly property string bgColor: "#F8F9FA"
    readonly property string titleBGColor: qmlPrimaryColor
    readonly property string titleColor: "#FFFFFF"
    readonly property string highlightColor: "#757575"
    readonly property string lowColor: "#D9E8FA"
    readonly property string highColor: qmlSecondaryColor

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
    readonly property string qmltextDot:     "."
    readonly property string qmltextBackSpaceSymbol: "⌫"
    readonly property string qmltextClr:  qsTr("Clr")

    readonly property string qmltextCancel:  qsTr("CANCEL")
    readonly property string qmltextDone:  qsTr("DONE")

    property int focusedItem: 0
    property alias suspectRejectheader: suspectRejectheader.text
    property alias switchReject: switchRejectLimitsText.checked
    property alias switchSuspect: switchSuspectLimitsText.checked
    property int suspectRejectIndex: 0
    property string mode : ""
    property string emptyString: ""
    property double suspectLowValue:0
    property double suspectHighValue:0
    property double rejectLowValue:0
    property double rejectHighValue:0
    property string checkSL: ""
    property string checkSH: ""
    property string checkRL: ""
    property string checkRH: ""
    /*unitText varaible declared to compare system unit*/
    property string unitText: ""
    property bool retValue: false
    signal modifiedRecipe(var flagdata);

    property int recipeIndex: 0
    property int maxLimit:0

    MouseArea{
        anchors.fill: parent
        onClicked: {
        }
    }

    /**
    *@breif: To Close Suspect & Reject Popup
    **/
    function close()
    {
        suspectRejectNum.visible = false
    }

    /**
    *@breif: To open Suspect & Reject Popup
    **/
    function open() {
        suspectRejectNum.visible = true
    }

    /**
    *@breif: To Validate limits on clicking OKAY
    **/
    function validateLimitParameter()
    {
        var lowValue = 0
        var highValue = 0

        if(suspectLow.text > 0)
            lowValue = suspectLowValue
        else if(rejectLowValue > 0)
            lowValue = rejectLowValue
        else
            lowValue = 0

        if(suspectHighValue > 0)
            highValue = suspectHighValue
        else if(rejectHighValue > 0)
            highValue = rejectHighValue
        else
            highValue = 0

        if(suspectLow.text == "")
            suspectLowValue = -1
        else
            suspectLowValue = parseFloat(suspectLow.text)

        if(suspectHigh.text == "")
            suspectHighValue = -1
        else
            suspectHighValue = parseFloat(suspectHigh.text)

        if(rejectLow.text == "")
            rejectLowValue = -1
        else
            rejectLowValue = parseFloat(rejectLow.text)

        if(rejectHigh.text == "")
            rejectHighValue = -1
        else
            rejectHighValue = parseFloat(rejectHigh.text)

        var i
        if ((mode == "Peak Power") || (mode == "Frequency") || (mode == "End Weld Force" && unitText == "N"))
        {
            checkSL = suspectLow.text
            checkSH = suspectHigh.text
            checkRL = rejectLow.text
            checkRH = rejectHigh.text
            if ((checkSL.indexOf(".") != -1) || (checkSH.indexOf(".") != -1) || (checkRL.indexOf(".") != -1) || (checkRH.indexOf(".") != -1) )
            {
                displayQuitPopup(messagePopup.warning_title,qmltextDecimalNotAcceptMsg)
            }
            else
            {
                validateLimits()
            }
        }
        else
        {
            validateLimits()
        }
    }

    /**
    *@breif: To Validate limits on clicking OKAY
    **/
    function validateLimits()
    {
        var retVal = recipe.validateSuspectRejectValues(switchSuspectLimitsText.checked,
                                                        suspectLow.text,
                                                        suspectHigh.text,
                                                        switchRejectLimitsText.checked,
                                                        rejectLow.text,
                                                        rejectHigh.text,
                                                        mode)

        if(retVal == 4)
        {
            displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsSetupMsg)
            unsavedRecipe()
        }
        else if(retVal == 5)
            displayQuitPopup(messagePopup.warning_title,qmltextEneterValidRangeLimitsMsg)
        else if(retVal != 0)
            displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
        else
            unsavedRecipe()

        return;
    }

    /**
    *@breif: To save unsaved recipe limits
    **/
    function unsavedRecipe()
    {
        recipe.setParamaterChangedFlag(2)
        recipe.setStructureValue("suspect "+mode,switchSuspectLimitsText.checked?"1":"0",0)
        recipe.setStructureValue("suspect "+mode+" low",suspectLow.text==""?"0":"1",0)
        recipe.setStructureValue("suspect "+mode+" low value",suspectLow.text,0)
        recipe.setStructureValue("suspect "+mode+" high",suspectHigh.text==""?"0":"1",0)
        recipe.setStructureValue("suspect "+mode+" high value",suspectHigh.text,0)

        recipe.setStructureValue("reject "+mode,switchRejectLimitsText.checked?"1":"0",0)
        recipe.setStructureValue("reject "+mode+" low",rejectLow.text==""?"0":"1",0)
        recipe.setStructureValue("reject "+mode+" low value",rejectLow.text,0)
        recipe.setStructureValue("reject "+mode+" high",rejectHigh.text==""?"0":"1",0)
        recipe.setStructureValue("reject "+mode+" high value",rejectHigh.text)

        close()
    }

    /**
    *@breif: To set the limit values
    **/
    function setLimitValues(a_suspectLowVal,a_suspectHighVal,a_rejectLowVal,a_rejectHighVal,
                            suspectStatus,rejectStatus,suspectLowStatus,suspectHighStatus,
                            rejectLowStatus,rejectHighStatus)
    {
        switchRejectLimitsText.checked = rejectStatus
        switchSuspectLimitsText.checked = suspectStatus

        if (rejectLowStatus == true)
        {
            rejectLow.text = a_rejectLowVal
            rejectLow.visible = true
            rejectLowText.y = -20
            rejectLowText.color = highlightColor           
        }
        else
        {
            rejectLowRecMA.enabled = true
            rejectLowText.y = rejectLowRec.height/2 - rejectLowText.height/2
            rejectLow.visible = false
            rejectLowText.color = "#ffffff"
            rejectLow.text = emptyString
        }

        if((rejectHighStatus == true))
        {
            rejectHigh.text = a_rejectHighVal
            rejectHigh.visible = true
            rejectHighText.y = -20
            rejectHighText.color = highlightColor
        }
        else
        {
            rejectHighRecMA.enabled = true
            rejectHighText.y = rejectHighRec.height/2 - rejectHighText.height/2
            rejectHigh.visible = false
            rejectHigh.text = emptyString
            rejectHighText.color = "#ffffff"
        }

        if(suspectLowStatus == true)
        {
            suspectLow.text = a_suspectLowVal
            suspectLow.visible = true
            suspectLowText.y = -20
            suspectLowText.color = highlightColor            
        }
        else
        {
            suspectLowRecMA.enabled = true
            suspectLowText.y = suspectHighRec.height/2 - suspectHighText.height/2
            suspectLow.visible = false
            suspectLowText.color = "#ffffff"
            suspectLow.text = emptyString
        }

        if(suspectHighStatus == true)
        {
            suspectHigh.text = a_suspectHighVal
            suspectHigh.visible = true
            suspectHighText.y = -20
            suspectHighText.color = highlightColor
        }
        else
        {
            suspectHighRecMA.enabled = true
            suspectHighText.y = suspectHighRec.height/2 - suspectHighText.height/2
            suspectHigh.visible = false
            suspectHighText.color = "#ffffff"
            suspectHigh.text = emptyString
        }
    }

    /**
    *@breif: To set the values when reject if off
    **/
    function setValuesWhenRejectOFF()
    {
        rejectLowdummyRec.visible = true
        rejectHighdummyRec.visible = true
        rejectLowRec.visible = false
        rejectHighRec.visible = false
        rejectLowText.visible = false
        rejectHighText.visible = false
        rejectLow.visible = false
        rejectHigh.visible = false
    }
    /**
    *@breif: To set the values when reject if on
    **/
    function setValuesWhenRejectOn()
    {
        if(rejectLow.text == "" && rejectHigh.text == "")
        {
            rejectLowText.y = rejectLowRec.height/2 - rejectLowText.height/2
            rejectHighText.y = rejectHighRec.height/2 - rejectHighText.height/2
            rejectLowText.color = "#ffffff"
            rejectHighText.color =  "#ffffff"
            rejectLowRecMA.enabled = true
            rejectHighRecMA.enabled = true
            rejectLow.visible = false
            rejectHigh.visible = false
            rejectLowText.visible = true
            rejectHighText.visible = true
        }
        else
        {
            rejectLowText.y = -20
            rejectHighText.y = -20
            rejectLowText.color = highlightColor
            rejectHighText.color =  highlightColor
            rejectLowRecMA.enabled = false
            rejectHighRecMA.enabled = false
            rejectLow.visible = true
            rejectHigh.visible = true
            rejectLowText.visible = false
            rejectHighText.visible = false
        }

        rejectLowdummyRec.visible = false
        rejectHighdummyRec.visible = false
        rejectLowRec.visible = true
        rejectHighRec.visible = true
        rejectLowText.visible = true
        rejectHighText.visible = true
    }
    /**
    *@breif: To set the values when suspect if off
    **/
    function setValuesWhenSuspectOFF()
    {
        suspectLowdummyRec.visible = true
        suspectHighdummyRec.visible = true
        suspectLowRec.visible = false
        suspectHighRec.visible = false
        suspectLowText.visible = false
        suspectHighText.visible = false
        suspectLow.visible = false
        suspectHigh.visible = false
    }

    /**
    *@breif: To set the values when suspect if on
    **/function setValuesWhenSuspectOn()
    {
        if(suspectLow.text == "" && suspectHigh.text == "")
        {
            suspectLowText.y = suspectLowRec.height/2 - suspectLowText.height/2
            suspectHighText.y = suspectHighRec.height/2 - suspectHighText.height/2
            suspectLowText.color = "#ffffff"
            suspectHighText.color =  "#ffffff"
            suspectLowRecMA.enabled = true
            suspectHighRecMA.enabled = true
            suspectLow.visible = false
            suspectHigh.visible = false
            suspectLowText.visible = true
            suspectHighText.visible = true
        }
        else
        {
            suspectLowText.y = -20
            suspectHighText.y = -20
            suspectLowText.color = highlightColor
            suspectHighText.color =  highlightColor
            suspectLowRecMA.enabled = false
            suspectHighRecMA.enabled = false
            suspectLow.visible = true
            suspectHigh.visible = true
            suspectLowText.visible = false
            suspectHighText.visible = false
        }

        suspectLowdummyRec.visible = false
        suspectHighdummyRec.visible = false
        suspectLowRec.visible = true
        suspectHighRec.visible = true
        suspectLowText.visible = true
        suspectHighText.visible = true
    }
    /**
    *@breif: To disable "0" to "9" and "." based on the parameter and its unit
    **/
    function disabled(operator,enteredtext)
    {
        if (enteredtext == emptyString && !((operator >= "0" && operator <= "9") || operator == "."))
        {
            return true
        }
        else if (operator == "." && enteredtext.toString().search(/\./) != -1)
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
        if (operator == "backspace")
        {
            switch(focusedItem)
            {
            case 1 :
                rejectLow.text = qmlTextEmpty
                break
            case 2:
                suspectLow.text = qmlTextEmpty
                break
            case 3:
                suspectHigh.text = qmlTextEmpty
                break
            case 4:
                rejectHigh.text = qmlTextEmpty
                break
            case 5:
                var digits1 = rejectLow.text
                digits1 =  digits1.toString().slice(0, -1)
                rejectLow.text = digits1
                break
            case 6:
                var digits2 = suspectLow.text
                digits2 =  digits2.toString().slice(0, -1)
                suspectLow.text = digits2
                break
            case 7:
                var digits3 = suspectHigh.text
                digits3 = digits3.toString().slice(0, -1)
                suspectHigh.text = digits3
                break
            case 8:
                var digits4 = rejectHigh.text
                digits4 = digits4.toString().slice(0, -1)
                rejectHigh.text = digits4
                break
            }
        }
    }

    /**
    *@breif: When clear is pressed
    **/
    function clearPressed( clearop )
    {
        if (clearop == "clear")
        {
            switch(focusedItem)
            {
            case 1:
            case 5:
                rejectLow.text = emptyString
                break
            case 2:
            case 6:
                suspectLow.text = emptyString
                break
            case 3:
            case 7:
                suspectHigh.text = emptyString
                break
            case 4:
            case 8:
                rejectHigh.text = emptyString
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
        case 5:
            if(rejectLow.text.length<7)
                rejectLow.text += zeroDigit
            break
        case 2:
        case 6:
            if(suspectLow.text.length<7)
                suspectLow.text += zeroDigit
            break
        case 3:
        case 7:
            if(suspectHigh.text.length<7)
                suspectHigh.text += zeroDigit
            break
        case 4:
        case 8:
            if(rejectHigh.text.length<7)
                rejectHigh.text += zeroDigit
            break
        }
    }

    /**
    *@breif:To detect presence of decimal in value
    **/
    function isDecimal(myText)
    {
        if(myText.toString().search(/\./) !== -1)
            return true
        else
            return false
    }

    /**
    *@breif:When digit is pressed
    **/
    function digitPressed(digit)
    {
        switch(focusedItem)
        {
        case 1:
            rejectLow.text = qmlTextEmpty
            focusedItem = 5
            break;
        case 2:
            suspectLow.text = qmlTextEmpty
            focusedItem = 6
            break;
        case 3:
            suspectHigh.text = qmlTextEmpty
            focusedItem = 7
            break;
        case 4:
            rejectHigh.text = qmlTextEmpty
            focusedItem = 8
            break;
        }

        /**
        *@breif:To fetch System Type info for
                1.To select regular expression which decides precision and length of value entered.
                2.To determine max length for which value entered i.e either Absolute Distance or Collapse Distance.
        **/
        var regExp

        if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
         {
             if(suspectRejectheader.text === qsTr("Absolute Distance"))
             {
                 maxLimit = 3;
                 regExp =/^\d{0,3}(\.\d{0,1})?$/;
             }

             else if(suspectRejectheader.text === qsTr("Collapse Distance"))
             {
                 maxLimit = 2;
                 regExp =/^\d{0,2}(\.\d{0,1})?$/;
             }
         }

         else
         {
             if(suspectRejectheader.text === qsTr("Absolute Distance"))
             {
                 maxLimit = 3;
                 regExp =/^\d{0,3}(\.\d{0,2})?$/;
             }

             else if(suspectRejectheader.text === qsTr("Collapse Distance"))
             {
                 maxLimit = 2;
                 regExp =/^\d{0,2}(\.\d{0,2})?$/;
             }
         }

        /*Common and independent parameters from System Type*/
        if(suspectRejectheader.text === qsTr("Time"))
        {
            maxLimit = 3;
            regExp =/^\d{0,3}(\.\d{0,2})?$/;
        }

        else if(suspectRejectheader.text === qsTr("Energy"))
        {
            maxLimit = 5;
            regExp =/^\d{0,5}(\.\d{0,0})?$/;
        }
        /*limit suspect reject screen force parameter numpad restriction on the basis of system unit */
        else if(suspectRejectheader.text === qsTr("END WELD FORCE"))
        {
            if (unitText == "lbs" || unitText == "Lbs" || unitText == "LBS")
            {
                maxLimit = 3;
                regExp =/^\d{0,3}(\.\d{0,0})?$/;
            }
            else
            {
                maxLimit = 5;
                if(digit === ".")
                    return false
                regExp =/^\d{0,5}?$/;
            }
        }

        else if((suspectRejectheader.text !== qsTr("Absolute Distance")) &&
                (suspectRejectheader.text !== qsTr("Collapse Distance")))
        {
            maxLimit = 5;
            if(digit===".")
                return false
            regExp =/^\d{0,5}?$/;
        }

        switch(focusedItem)
        {
        case 5:
            if (disabled(digit, rejectLow.text))
            {
                return
            }

            else
            {
                if(regExp.test(rejectLow.text))
                {
                    if(rejectLow.text.length !== maxLimit)
                    {
                        rejectLow.text += digit
                    }

                    else if(rejectLow.text.length === maxLimit)
                    {
                        if((!isDecimal(rejectLow.text)) && digit === ".")
                        {
                            rejectLow.text += digit
                        }

                        else if((isDecimal(rejectLow.text)) && digit !== ".")
                        {
                            rejectLow.text += digit
                        }
                    }
                }
            }
            break

        case 6:
            if (disabled(digit, suspectLow.text))
            {
                return
            }

            else
            {
                if(regExp.test(suspectLow.text))
                {
                    if(suspectLow.text.length !== maxLimit)
                    {
                        suspectLow.text += digit
                    }

                    else if(suspectLow.text.length === maxLimit)
                    {
                        if((!isDecimal(suspectLow.text)) && digit === ".")
                        {
                            suspectLow.text += digit
                        }

                        else if((isDecimal(suspectLow.text)) && digit !== ".")
                        {
                            suspectLow.text += digit
                        }
                    }
                }

            }
            break

        case 7:
            if (disabled(digit, suspectHigh.text))
            {
                return
            }

            else
            {
                if(regExp.test(suspectHigh.text))
                {
                    if(suspectHigh.text.length !== maxLimit)
                    {
                        suspectHigh.text += digit
                    }

                    else if(suspectHigh.text.length === maxLimit)
                    {
                        if((!isDecimal(suspectHigh.text)) && digit === ".")
                        {
                            suspectHigh.text += digit
                        }

                        else if((isDecimal(suspectHigh.text)) && digit !== ".")
                        {
                            suspectHigh.text += digit
                        }
                    }
                }
            }
            break

        case 8:
            if (disabled(digit, rejectHigh.text))
            {
                return
            }

            else
            {
                if(regExp.test(rejectHigh.text))
                {
                    if(rejectHigh.text.length !== maxLimit)
                    {
                        rejectHigh.text += digit
                    }

                    else if(rejectHigh.text.length === maxLimit)
                    {
                        if((!isDecimal(rejectHigh.text)) && digit === ".")
                        {
                            rejectHigh.text += digit
                        }

                        else if((isDecimal(rejectHigh.text)) && digit !== ".")
                        {
                            rejectHigh.text += digit
                        }
                    }
                }
            }
            break
        }
    }


    Item {
        id: suspectRejectWindow
        width: minWidth
        height: minHeight
        anchors.centerIn: parent

        Rectangle {
            id: roundRect
            width:  parent.width
            height: 36
            radius: 5
            color: titleBGColor
        }

        Rectangle{
            id: suspectRejectheaderRec
            width: suspectRejectWindow.width
            height: 30
            color: titleBGColor
            anchors.bottom : roundRect.bottom
            anchors.left : roundRect.left
            anchors.right : roundRect.right
            Text{
                id:suspectRejectheader
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text:  qsTr("")
                font.pixelSize: Style.style4
                font.family: Style.semibold.name
                color: titleColor
            }
            Text{
                id:suspectRejectheaderCommonText
                anchors.left: suspectRejectheader.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                text:  qsTr(": SUSPECT & REJECT LIMITS")
                font.pixelSize: Style.style4
                font.family: Style.semibold.name
                color: titleColor
            }
            Image {
                id: suspectRejectWindowclose
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
                id: suspectRejectWindowCloseMA
                width: parent.width*(10/100)
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5
                onClicked: {
                    close()
                }
            }
        }

        Rectangle{
            id:numpadRecId
            width: suspectRejectWindow.width
            height: parent.height - roundRect.height
            color: bgColor
            anchors.top : suspectRejectheaderRec.bottom

            Rectangle{
                id:suspectRejectStepRec
                width: parent.width
                height: parent.height*(9/100)
                color: bgColor
                Text {
                    id: suspectLimitsText
                    anchors.left: parent.left
                    anchors.leftMargin: 50
                    text: qsTr("SUSPECT")
                    color:unitTextColor
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style5
                }
                ToogleBar {
                    id: switchSuspectLimitsText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: suspectLimitsText.right
                    anchors.leftMargin: 20
                    onCheckedChanged: {
                        if(checked == true )
                        {
                            setValuesWhenSuspectOn()
                        }

                        if(checked == false)
                        {
                            setValuesWhenSuspectOFF()
                        }
                    }

                }
                Text {
                    id:  rejectLimitsText
                    anchors.right:  switchRejectLimitsText.left
                    anchors.rightMargin: 20
                    text: qsTr("REJECT")
                    color:unitTextColor
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style5
                }
                ToogleBar {
                    id: switchRejectLimitsText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin:  50
                    onCheckedChanged: {
                        if(checked == true )
                        {
                            setValuesWhenRejectOn()
                        }
                        if(checked == false)
                        {
                            setValuesWhenRejectOFF()
                        }
                    }
                }
            }

            Rectangle{
                id: limitRanges
                width: suspectRejectWindow.width*(95/100)
                height: 30
                x: suspectRejectWindow.width*(2.5/100)
                anchors.top: suspectRejectStepRec.bottom
                anchors.topMargin: 20
                color: bgColor
                Rectangle{
                    id:rejectLowdummyRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    visible: true
                }
                Rectangle{
                    id:rejectLowRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    color: highColor
                    border.color: highColor
                    visible: false
                    Text {
                        id:rejectLowText
                        x: 10
                        height: 17
                        text: qsTr("Reject Low")
                        wrapMode: Text.WordWrap
                        color: titleColor
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style0
                        Component.onCompleted: {
                            rejectLowRecMA.enabled = true
                            rejectLowanim.stop()
                        }
                        NumberAnimation on y{
                            id:rejectLowanim
                            from: rejectLowRec.height/2 - rejectLowText.height/2 ;to: -20
                            duration: 250
                        }
                    }
                    MouseArea{
                        id:rejectLowRecMA
                        width: rejectLowRec.width
                        height: rejectLowRec.height
                        onClicked:
                        {
                            rejectLowanim.start()
                            rejectLowRecMA.enabled = false
                            rejectLowText.color = highlightColor
                            rejectLow.visible = true
                            focusedItem = 1
                        }
                        onEntered:
                        {
                            cursorShape=Qt.PointingHandCursor
                        }
                    }
                    TextField{
                        id: rejectLow
                        width: parent.width
                        height: parent.height
                        selectByMouse: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: Style.regular.name
                        font.pixelSize: Style.style3
                        visible: false
                        color: titleColor
                        text:qsTr("")
                        background: Rectangle
                        {
                            anchors.fill: parent
                            border.color: highColor
                            color: highColor
                        }
                        focus: false
                        MouseArea {
                            anchors.fill: parent
                            onClicked:
                            {
                                parent.focus = true;
                                parent.selectAll()
                                focusedItem = 1
                                Qt.inputMethod.hide()
                            }
                            onDoubleClicked:
                            {
                                parent.focus = true
                                parent.deselect()
                                focusedItem = 5
                                Qt.inputMethod.hide()
                            }
                        }
                    }
                }
                Rectangle{
                    id:suspectLowdummyRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.left: rejectLowRec.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    visible: true
                }

                Rectangle{
                    id:suspectLowRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.left: rejectLowRec.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: titleBGColor
                    visible: false
                    Text {
                        id:suspectLowText
                        x: 5
                        height: 17
                        text: qsTr("Suspect Low")
                        wrapMode: Text.WordWrap
                        color: titleColor
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style0
                        Component.onCompleted:
                        {
                            suspectLowRecMA.enabled = true
                            suspectLowanim.stop()
                        }
                        NumberAnimation on y
                        {
                            id:suspectLowanim
                            from: suspectLowRec.height/2 - suspectLowText.height/2 ;to: -20
                            duration: 250
                        }
                    }
                    MouseArea{
                        id:suspectLowRecMA
                        width: suspectLowRec.width
                        height: suspectLowRec.height
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked:
                        {
                            suspectLowanim.start()
                            suspectLowRecMA.enabled = false
                            suspectLowText.color = highlightColor
                            suspectLow.visible = true
                            focusedItem = 2
                        }
                        onEntered:
                        {
                            cursorShape=Qt.PointingHandCursor
                        }
                    }
                    TextField{
                        id: suspectLow
                        width: parent.width
                        height: parent.height
                        selectByMouse: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: Style.regular.name
                        font.pixelSize: Style.style3
                        visible: false
                        color: titleColor
                        text:qsTr("")
                        background: Rectangle {
                            anchors.fill: parent
                            border.color: titleBGColor
                            color: titleBGColor
                        }
                        focus: false
                        MouseArea {
                            anchors.fill: parent
                            onClicked:
                            {
                                parent.focus = true;
                                parent.selectAll()
                                focusedItem = 2
                                Qt.inputMethod.hide()
                            }
                            onDoubleClicked:
                            {
                                parent.focus = true
                                parent.deselect()
                                focusedItem = 6
                                Qt.inputMethod.hide()
                            }
                        }
                    }
                }
                Rectangle{
                    id: goodRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.left: suspectLowRec.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    Image {
                        id: goodImg
                        sourceSize.width: 40//parent.width
                        sourceSize.height: 40//parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/images/image/checkmark_green.svg"
                    }
                }
                Rectangle{
                    id:suspectHighdummyRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.left: goodRec.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    visible: true
                }

                Rectangle{
                    id:suspectHighRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.left: goodRec.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: titleBGColor
                    border.color: titleBGColor
                    visible: false
                    Text {
                        id:suspectHighText
                        x: 5
                        height: 17
                        text: qsTr("Suspect High")
                        wrapMode: Text.WordWrap
                        color: titleColor
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style0
                        Component.onCompleted:
                        {
                            suspectHighRecMA.enabled = true
                            suspectHighanim.stop()
                        }
                        NumberAnimation on y
                        {
                            id:suspectHighanim
                            from: suspectHighRec.height/2 - suspectHighText.height/2 ;to: -20
                            duration: 250
                        }
                    }
                    MouseArea{
                        id:suspectHighRecMA
                        width: suspectHighRec.width
                        height: suspectHighRec.height
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked:
                        {
                            suspectHighanim.start()
                            suspectHighRecMA.enabled = false
                            suspectHighText.color = highlightColor
                            suspectHigh.visible = true
                            focusedItem = 3
                        }
                        onEntered:
                        {
                            cursorShape=Qt.PointingHandCursor
                        }
                    }
                    TextField{
                        id: suspectHigh
                        width: parent.width
                        height: parent.height
                        selectByMouse: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: Style.regular.name
                        font.pixelSize: Style.style3
                        visible: false
                        text:qsTr("")
                        color: titleColor
                        background: Rectangle {
                            anchors.fill: parent
                            border.color: titleBGColor
                            color: titleBGColor
                        }
                        focus: false
                        MouseArea {
                            anchors.fill: parent
                            onClicked:
                            {
                                parent.focus = true;
                                parent.selectAll()
                                focusedItem = 3
                                Qt.inputMethod.hide()
                            }
                            onDoubleClicked:
                            {
                                parent.focus = true
                                parent.deselect()
                                focusedItem = 7
                                Qt.inputMethod.hide()
                            }
                        }
                    }
                }
                Rectangle{
                    id:rejectHighdummyRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    anchors.left: suspectHighdummyRec.right
                    visible: true
                }
                Rectangle{
                    id:rejectHighRec
                    width: limitRanges.width*(1/5)
                    height: limitRanges.height
                    anchors.left: suspectHighRec.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: highColor
                    visible: false
                    Text {
                        id:rejectHighText
                        x: 10
                        height: 17
                        text: qsTr("Reject High")
                        wrapMode: Text.WordWrap
                        color: titleColor
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style0
                        Component.onCompleted:
                        {
                            rejectHighRecMA.enabled = true
                            rejectHighanim.stop()
                        }
                        NumberAnimation on y
                        {
                            id:rejectHighanim
                            from: rejectHighRec.height/2 - rejectHighText.height/2 ;to: -20
                            duration: 250
                        }
                    }
                    MouseArea{
                        id:rejectHighRecMA
                        width: rejectHighRec.width
                        height: rejectHighRec.height
                        onClicked:
                        {
                            rejectHighanim.start()
                            rejectHighRecMA.enabled = false
                            rejectHighText.color = highlightColor
                            rejectHigh.visible = true
                            focusedItem = 4
                        }
                        onEntered:
                        {
                            cursorShape=Qt.PointingHandCursor
                        }
                    }

                    TextField{
                        id: rejectHigh
                        width: parent.width
                        height: parent.height
                        selectByMouse: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: Style.regular.name
                        font.pixelSize: Style.style3
                        visible: false
                        color: titleColor
                        text:qsTr("")
                        background: Rectangle {
                            anchors.fill: parent
                            border.color: highColor
                            color: highColor
                        }
                        focus: false
                        MouseArea {
                            anchors.fill: parent
                            onClicked:
                            {
                                parent.focus = true;
                                parent.selectAll()
                                focusedItem = 4
                                Qt.inputMethod.hide()
                            }
                            onDoubleClicked:
                            {
                                parent.focus = true
                                parent.deselect()
                                focusedItem = 8
                                Qt.inputMethod.hide()
                            }
                        }
                    }
                }
            }

            Rectangle{
                id:numpadRec
                width: minWidthNumpad
                height: minWidthNumpad
                color: bgColor
                anchors.top:limitRanges.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Grid {
                    id:numGrid
                    columns: 3
                    columnSpacing: 10
                    rowSpacing: 10
                    anchors.top: numpadRec.top
                    anchors.topMargin: 0
                    anchors.left:parent.left
                    anchors.leftMargin: 0
                    width: parent.width - buttonSizeNumpad - 20
                    height: parent.height

                    PrimaryIconButton
                    {
                        id:text7
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit1
                    }

                    PrimaryIconButton
                    {
                        id:text8
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit2
                    }

                    PrimaryIconButton
                    {
                        id:text9
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit3
                    }

                    PrimaryIconButton
                    {
                        id:text4
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit4
                    }

                    PrimaryIconButton
                    {
                        id:text5
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit5
                    }

                    PrimaryIconButton
                    {
                        id:text6
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit6
                    }

                    PrimaryIconButton
                    {
                        id:text1
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit7
                    }

                    PrimaryIconButton
                    {
                        id:text2
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit8
                    }

                    PrimaryIconButton
                    {
                        id:text3
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit9
                    }

                    PrimaryIconButton
                    {
                        id:textdot
                        maxWidth: buttonSizeNumpad
                        maxHeight: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDot
                    }
                }
                SecondaryButton {
                    id: deleteButton
                    text: qmltextBackSpaceSymbol
                    textColor: titleBGColor
                    anchors.left: numGrid.right
                    anchors.top: numpadRec.top
                    anchors.topMargin: 0
                    maxHeight: buttonSizeNumpad
                    width: textdot.width//42
                    height: textdot.height*3 + 20//141
                    anchors.leftMargin: 10
                    fontSize: Style.style8
                    onClicked:
                    {
                        operatorPressed("backspace")
                    }
                }
                PrimaryIconButton {
                    id: customButton
                    text: qmltextClr
                    anchors.leftMargin: 10
                    anchors.left: numGrid.right
                    anchors.top:deleteButton.bottom
                    anchors.topMargin: 10
                    visible: true
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    onClicked:
                    {
                        clearPressed("clear")
                    }
                }
                PrimaryIconButton {
                    id: zeroButton
                    text: qmltextDigit0
                    width: buttonSizeNumpad * 2 + 10
                    anchors.right: customButton.left
                    anchors.rightMargin: 10
                    anchors.top:deleteButton.bottom
                    anchors.topMargin: 10
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                }

                SecondaryButton {
                    id: set
                    anchors.top: zeroButton.bottom
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: -6
                    fontSize: Style.style5
                    text: qmltextCancel

                    onClicked:
                    {
                        close()
                        disableMainwindowOpacity.start()
                    }
                }

                PrimaryButton {
                    id: setandcontinue
                    anchors.top: zeroButton.bottom
                    anchors.topMargin: 20
                    anchors.left: set.right
                    anchors.leftMargin: 12
                    text: qmltextDone
                    fontSize: Style.style5
                    onClicked:
                    {
                        validateLimitParameter()
                    }
                }
            }
        }
    }
}
