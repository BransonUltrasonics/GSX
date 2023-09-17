/*
 * setupLimitNumpad.qml
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
    id: setupLimitNum
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false
    property alias setupLimitNum: setupLimitNum.visible
    readonly property string qmltextEneterValidLimitsMsg : qsTr("Please enter valid limits to continue")
    readonly property string qmlTextInvalidSetup: qsTr("Setup limits values are outside the Suspect/Reject limits! Modify suitably or Turn OFF")
    readonly property string qmlTextInvalidParamSetup: qsTr("Parameter Values are outside the Setup limits! Modify suitably")
    readonly property int minWidth: 420
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
    property alias setupLimitheader: setupLimitheader.text
    /*unitText varaible declared to compare unit*/
    property string unitText: ""
    property alias switchSetup: switchSetupLimitsText.checked
    property alias setupOnOffRec:  setupOnOffRec.visible
    property alias mpText: wpText.visible
    property alias wpText: mpText.visible
    property int setupLimitIndex: 0
    property string  mode : ""
    property string emptyString: ""
    property double setupLowValue
    property double setupHighValue
    property string checkSetupLow: ""
    property string checkSetupHigh: ""
    property bool retValue: false
    property string sampleStr : ""
    property double setUpLow
    property double setUpHigh
    property int recipeIndex: 0
    property bool retStatus
    property bool hornDown:false
    property int retVal: 0
    property string oldMinVal: ""
    property string oldMaxVal: ""
    property int maxLimit:0
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
        }
    }

    /**
    *@breif: To close setupLimit numberpad popup
    **/
    function close()
    {
        /*To clear flag*/
        hornDown = false
        setupLimitNum.visible = false
        disableMainwindowOpacity.start()
        focusedItem = 0
    }

    /**
    *@breif: To open setupLimit numberpad popup
    **/
    function open()
    {
        setupLimitNum.visible = true
        focusedItem = 0
    }

    /**
    *@breif: Validates the entered horndown parameter limits and stores in weld recipe structure
    **/
    function validateLimitParameter()
    {
        /*Here the use of "hornDown" flag is to make sure that below section is used by only Expected Contact Window*/
        if(hornDown == true)
        {
            setUpLow = parseFloat(setupLowTF.text)
            setUpHigh = parseFloat(setupHighTF.text)
            if(((setupLowTF.text != "") && (setupHighTF.text != "")))
            {
                var validate1,validate2
                validate1 = recipe.keypadlimitation("part contact window minus",setupLowTF.text)
                validate2 = recipe.keypadlimitation("part contact window plus",setupHighTF.text)
                if((validate1 == setupLowTF.text) && (validate2 == setupHighTF.text))
                {
                    recipe.setParamaterChangedFlag(1)
                    recipe.setStructureValue("part contact window minus",setupLowTF.text,0)
                    recipe.setStructureValue("part contact window plus",setupHighTF.text,true)
                    horndownObj.updateActuatorList(4)
                    horndownObj.updateHornPositionValues()
                    close()
                }

                else
                {
                    setupLowTF.text = oldMinVal
                    setupHighTF.text = oldMaxVal
                    displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
                }
            }

            else
            {
                setupLowTF.text = oldMinVal
                setupHighTF.text = oldMaxVal
                displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
            }
        }

        else
            unsavedRecipe()
    }

    /**
    *@breif: Validates the entered recipe setup limits and stores in weld recipe structure
    **/
    function unsavedRecipe()
    {
        setUpLow = parseFloat(setupLowTF.text)
        setUpHigh = parseFloat(setupHighTF.text)
        var retStatus = recipe.checkSetupLimits(setUpLow,setUpHigh,switchSetupLimitsText.checked,mode)

        if(retStatus == 1)
        {
            displayQuitPopup(messagePopup.warning_title,qmltextEneterValidLimitsMsg)
        }
        else if(retStatus == 2)
        {
            displayQuitPopup(messagePopup.warning_title,qmlTextInvalidParamSetup)
        }
        else if(retStatus == 3)
        {
            displayQuitPopup(messagePopup.warning_title,qmlTextInvalidSetup)
        }
        else
        {
            var strParamName = "";

            if(mode == "Weld Amplitude" || mode == "Weld Force" || mode ==  "Trigger Force"||
               mode ==  "Hold Force" || mode == "Hold Time" || mode == "Reactivity"  ||
               mode ==  "Force Level")
                strParamName = mode;
            else
                strParamName = "weld mode";

            recipe.setParamaterChangedFlag(3)
            recipe.setStructureValue("setup " + strParamName + " status",switchSetupLimitsText.checked?"1":"0",0)
            recipe.setStructureValue("setup " + strParamName + " low",setupLowTF.text,0)
            recipe.setStructureValue("setup " + strParamName + " high",setupHighTF.text,1)
            close()
        }
    }

    /**
    *@breif: Sets limits values when popup is opened
    **/
    function setLimitValues(a_setupLowVal,a_setupHighVal,setupStatus)
    {
        if(a_setupLowVal == "0" && a_setupHighVal == "0")
        {
            a_setupLowVal = ""
            a_setupHighVal = ""
        }
        switchSetupLimitsText.checked = setupStatus
        if ((switchSetupLimitsText.checked == true) && (a_setupLowVal != emptyString) && (a_setupHighVal != emptyString))
        {
            setupLowTF.text = a_setupLowVal
            setupLowTF.visible = true
            setupHighTF.text = a_setupHighVal
            setupHighTF.visible = true
        }
        else if((switchSetupLimitsText.checked == false) && (a_setupLowVal != emptyString) && (a_setupHighVal != emptyString))
        {
            setupLowTF.text = a_setupLowVal
            setupLimitLowdummyRec.visible = true
            setupLimitHighdummyRec.visible = true
            setupLimitLowRec.visible = false
            setupLimitHighRec.visible = false
            setupLowTF.visible = false
            setupHighTF.text = a_setupHighVal
            setupHighTF.visible = false
        }
        else if((switchSetupLimitsText.checked == false) )
        {
            setupLowTF.visible = false
            setupHighTF.text = emptyString
            setupHighTF.visible = false
            setupHighTF.text = emptyString
        }
    }

    /**
    *@breif: Resets textfields, when  setup switch is off
    **/
    function setValuesWhenSetupOFF()
    {
        setupLimitLowRec.visible = false
        setupLowTF.visible =false
        setupLimitLowdummyRec.visible =true

        setupLimitHighRec.visible =false
        setupHighTF.visible = false
        setupLimitHighdummyRec.visible = true
        focusedItem = 0
    }

    /**
    *@breif: Resets textfields, when setup switch is on
    **/
    function setValuesWhenSetupOn()
    {
        if(setupLowTF.text == "" && setupHighTF.text == "")
        {
            setupLowTF.visible = false
            setupHighTF.visible = false
        }
        else
        {
            setupLowTF.visible =true
            setupHighTF.visible = true
        }
        setupLimitLowRec.visible = true
        setupLimitLowdummyRec.visible = false

        setupLimitHighRec.visible = true
        setupLimitHighdummyRec.visible = false
        focusedItem = 0
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

            case 1:
                setupLowTF.text = qmlTextEmpty
                break
            case 2:
                setupHighTF.text = qmlTextEmpty
                break;
            case 3:
                var digits2 = setupLowTF.text
                digits2 =  digits2.toString().slice(0, -1)
                setupLowTF.text = digits2
                break
            case 4 :
                var digits1 = setupHighTF.text
                digits1 =  digits1.toString().slice(0, -1)
                setupHighTF.text = digits1
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
            case 3:
                setupLowTF.text = emptyString
                break
            case 2:
            case 4:
                setupHighTF.text = emptyString
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
        case 3:
            if(setupLowTF.text.length<7)
                setupLowTF.text += zeroDigit
            break
        case 2:
        case 4:
            if(setupHighTF.text.length<7)
                setupHighTF.text += zeroDigit
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
            setupLowTF.text = qmlTextEmpty
            focusedItem = 3
            break
        case 2:
            setupHighTF.text = qmlTextEmpty
            focusedItem = 4
            break
        }

        /**
        *@breif:To fetch System Type info for
                1.To select regular expression which decides precision and length of value entered.
                2.To determine max length for which value entered i.e either Absolute Distance or Collapse Distance.
        **/
        var regExp

        if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
        {
            if(setupLimitheader.text === qsTr("ABSOLUTE DISTANCE"))
            {
                maxLimit = 3;
                regExp =/^\d{0,3}(\.\d{0,1})?$/;
            }

            else if((setupLimitheader.text === qsTr("COLLAPSE DISTANCE")) ||
                    (setupLimitheader.text === qsTr("EXPECTED CONTACT WINDOW")))
            {
                maxLimit = 2;
                regExp =/^\d{0,2}(\.\d{0,1})?$/;
            }
        }

        else
        {
            if(setupLimitheader.text === qsTr("ABSOLUTE DISTANCE"))
            {
                maxLimit = 3;
                regExp =/^\d{0,3}(\.\d{0,2})?$/;
            }

            else if((setupLimitheader.text === qsTr("COLLAPSE DISTANCE")) ||
                    (setupLimitheader.text === qsTr("EXPECTED CONTACT WINDOW")))
            {
                maxLimit = 2;
                regExp =/^\d{0,2}(\.\d{0,2})?$/;
            }
        }

        /*Common and independent parameters from System Type*/
        if((setupLimitheader.text === qsTr("HOLD TIME"))||
                (setupLimitheader.text === qsTr("TIME")))
        {
            maxLimit = 3;
            regExp =/^\d{0,3}(\.\d{0,2})?$/;
        }

        else if(setupLimitheader.text === qsTr("ENERGY"))
        {
            maxLimit = 5;
            regExp =/^\d{0,5}(\.\d{0,0})?$/;
        }

        else if(setupLimitheader.text === qsTr("SCRUB TIME"))
        {
            maxLimit = 1;
            regExp =/^\d{0,1}(\.\d{0,2})?$/;
        }
        /*limit setup screen force parameter numpad restriction on the basis of system unit */
        else if((setupLimitheader.text === qsTr("HOLD FORCE"))||
               (setupLimitheader.text === qsTr("WELD FORCE")) ||
               (setupLimitheader.text === qsTr("TRIGGER FORCE")))
        {
            if (unitText == "lbs")
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

        else if((setupLimitheader.text !== qsTr("ABSOLUTE DISTANCE")) &&
                (setupLimitheader.text !== qsTr("COLLAPSE DISTANCE")) &&
                (setupLimitheader.text !== qsTr("EXPECTED CONTACT WINDOW")))
        {
            maxLimit = 5;
            if(digit === ".")
                return false

            regExp =/^\d{0,5}?$/;
        }

        switch(focusedItem)
        {
        case 3:
            if (disabled(digit, setupLowTF.text))
            {
                return
            }

            else
            {
                if(regExp.test(setupLowTF.text))
                {
                    if(setupLowTF.text.length !== maxLimit)
                    {
                        setupLowTF.text += digit
                    }

                    else if(setupLowTF.text.length === maxLimit)
                    {
                        if((!isDecimal(setupLowTF.text)) && digit === ".")
                        {
                            setupLowTF.text += digit
                        }

                        else if((isDecimal(setupLowTF.text)) && digit !== ".")
                        {
                            setupLowTF.text += digit
                        }
                    }
                }
            }
            break
        case 4:
            if (disabled(digit, setupHighTF.text))
            {
                return
            }

            else
            {
                if(regExp.test(setupHighTF.text))
                {
                    if(setupHighTF.text.length !== maxLimit)
                    {
                        setupHighTF.text += digit
                    }

                    else if(setupHighTF.text.length === maxLimit)
                    {
                        if((!isDecimal(setupHighTF.text)) && digit === ".")
                        {
                            setupHighTF.text += digit
                        }

                        else if((isDecimal(setupHighTF.text)) && digit !== ".")
                        {
                            setupHighTF.text += digit
                        }
                    }
                }
            }
            break
        }
    }

    /*********************************************************/
    Item {
        id: setupLimitWindow
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
            id: setupLimitheaderRec
            width: setupLimitWindow.width
            height: 30
            color: titleBGColor
            anchors.bottom : roundRect.bottom
            anchors.left : roundRect.left
            anchors.right : roundRect.right
            Text{
                id:setupLimitheader
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text:  qsTr("")
                font.pixelSize: Style.style4
                font.family: Style.semibold.name
                color: titleColor
            }
            Text{
                id:setupLimitheaderCommonText
                visible: !(setupLimitheader.text === qsTr("EXPECTED CONTACT WINDOW"))
                anchors.left: setupLimitheader.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                text:  qsTr(": SETUP LOW & HIGH LIMITS")
                font.pixelSize: Style.style4
                font.family: Style.semibold.name
                color: titleColor
            }
            Image {
                id: setupLimittWindowclose
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
                id: setupLimittWindowCloseMA
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
            width: setupLimitWindow.width
            height: parent.height - roundRect.height
            color: bgColor
            anchors.top : setupLimitheaderRec.bottom

            Rectangle{
                id:setupOnOffRec
                width: parent.width*(70/100)
                height: parent.height*(9/100)
                x: setupLimitWindow.width*(15/100)
                color: bgColor
                Text {
                    id: suspectLimitsText
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(3/10)
                    text: qsTr("SETUP")
                    color:unitTextColor
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style5
                }
                ToogleBar {
                    id: switchSetupLimitsText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: suspectLimitsText.right
                    anchors.leftMargin: 20
                    onCheckedChanged: {
                        if(checked == true )
                        {
                            setValuesWhenSetupOn()
                        }
                        if(checked == false)
                        {
                            setValuesWhenSetupOFF()
                        }
                    }
                }
            }

            Rectangle{
                id: limitRanges
                width: setupLimitWindow.width*(70/100)
                height: 30
                x: setupLimitWindow.width*(15/100)
                anchors.top: setupOnOffRec.bottom
                anchors.topMargin: 5
                Rectangle{
                    id:setupLimitLowdummyRec
                    width: limitRanges.width*(1/3)
                    height: limitRanges.height
                    anchors.left: limitRanges.left
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    visible: true
                }

                Rectangle{
                    id:setupLimitLowRec
                    width: limitRanges.width*(1/3)
                    height: limitRanges.height
                    color: titleBGColor
                    visible: false
                    Text {
                        id:wpText
                        height: 17
                        y: -25
                        x: 40
                        text: qsTr("WP")
                        color: highlightColor
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style3
                        visible: false
                    }
                    TextField{
                        id: setupLowTF
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
                                focusedItem = 1
                                Qt.inputMethod.hide()
                            }
                            onDoubleClicked:
                            {
                                parent.focus = true
                                parent.deselect()
                                focusedItem = 3
                                Qt.inputMethod.hide()
                            }
                        }
                    }
                }
                Rectangle{
                    id: goodRec
                    width: parent.width*(1/3)
                    height: limitRanges.height
                    anchors.centerIn: parent
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    Image {
                        id: goodImg
                        sourceSize.width: 40
                        sourceSize.height: 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/images/image/checkmark_green.svg"
                    }
                }
                Rectangle{
                    id:setupLimitHighdummyRec
                    width: limitRanges.width*(1/3)
                    height: limitRanges.height
                    anchors.left: goodRec.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: lowColor
                    visible: true
                }

                Rectangle{
                    id:setupLimitHighRec
                    width: limitRanges.width*(1/3)
                    height: limitRanges.height
                    anchors.left: goodRec.right
                    color: titleBGColor
                    border.color: titleBGColor
                    visible: false
                    Text {
                        id:mpText
                        height: 17
                        y: -25
                        x: 40
                        text: qsTr("MP")
                        color: highlightColor
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style3
                        visible: false
                    }
                    TextField{
                        id: setupHighTF
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
                                focusedItem = 2
                                Qt.inputMethod.hide()
                            }
                            onDoubleClicked:
                            {
                                parent.focus = true
                                parent.deselect()
                                focusedItem = 4
                                Qt.inputMethod.hide()
                            }
                        }
                    }
                }
            }

            /******************************NUMPAD*******************************************************/
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
                        maxHeight: buttonSizeNumpad
                        maxWidth: buttonSizeNumpad
                        fontSize: Style.style5
                        text: qmltextDigit1

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
                        text: qmltextDigit8
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
                    textColor: titleBGColor
                    anchors.left: numGrid.right
                    anchors.leftMargin: 10
                    anchors.top: numpadRec.top
                    anchors.topMargin: 0
                    width: textdot.width
                    height: textdot.height*3 + 20
                    maxHeight: 60
                    fontSize: Style.style8
                    onClicked:
                    {
                        operatorPressed("backspace")
                    }
                }
                PrimaryIconButton {
                    id: customButton
                    text: qmltextClr
                    width: buttonSizeNumpad
                    anchors.leftMargin: 10
                    anchors.left: numGrid.right
                    anchors.top:deleteButton.bottom
                    anchors.topMargin: 10
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5
//                    visible: false
                    onClicked:
                    {
                        clearPressed("clear")
                    }
                }
                PrimaryIconButton
                {
                    id: zeroButton
                    text: qmltextDigit0
                    width: 60 * 2 + 10
                    anchors.right: customButton.left
                    anchors.rightMargin: 10
                    anchors.top:deleteButton.bottom
                    anchors.topMargin: 10
                    maxHeight: buttonSizeNumpad
                    maxWidth: buttonSizeNumpad
                    fontSize: Style.style5

                }

                SecondaryButton
                {
                    id: set
                    anchors.top: zeroButton.bottom
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: -6
                    text: qmltextCancel
                    fontSize: Style.style5
                    onClicked:
                    {
                        close()
                        disableMainwindowOpacity.start()
                    }
                }

                PrimaryButton
                {
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
