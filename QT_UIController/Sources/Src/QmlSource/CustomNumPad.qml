import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Universal 2.0
import Style 1.0

Rectangle{
    id: numpadPopupId
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false

    readonly property string qmltextTitle:qsTr("Title")
    readonly property string qmltextControlLimit:qsTr("CONTROL LIMIT")
    readonly property string qmltextBackspace:"backspace"

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

    //symbols
    readonly property string qmltextDot:  "."
    readonly property string qmltextBackSpaceSymbol: "⌫"
    readonly property string qmltextClr:  qsTr("Clr")

    //units
    readonly property string qmltextUnitSecUpperCase :  "S"
    readonly property string qmltextUnitSecLowerCase :  "s"
    readonly property string qmltextUnitWattUpperCase :  "W"
    readonly property string qmltextUnitWattLowerCase :  "w"
    readonly property string qmltextUnitMillimeter :  "mm"
    readonly property string qmltextUnitPercentage :  "%"
    readonly property string qmltextUnitIn :  "in"

    readonly property string qmltextIPV4 :  qsTr("IPV4")
    readonly property string qmltextIPV6 :  qsTr("IPV6")

    readonly property string qmltextCancel:  qsTr("CANCEL")
    readonly property string qmltextOkay:  qsTr("OKAY")
    readonly property string qmltextMin :qsTr("Min : ")
    readonly property string qmltextMax :qsTr("Max : ")
    property string unitText: qmlTextEmpty
    readonly property string unitTextColor: "#898D96"
    readonly property string bgColor: "#F8F9FA"
    readonly property string titleBGColor: qmlPrimaryColor
    readonly property string titleColor: "#FFFFFF"
    readonly property int minWidth: 330
    readonly property int minHeight: 500
    readonly property int minWidthNumpad: 280
    readonly property int minHeightNumpad: 280
    readonly property int buttonSizeNumpad: 60
    property alias keyInputText: lineedit.text
    property string numpadTitle: ""
    property string numpadTitleLocale: ""
    property alias toggleSwicthRec: toggleSwicthInnerRec.visible
    property alias toggleSwicthCheck: toggleSwicth.checked
    property alias lineeditReadOnly: lineedit.readOnly
    property alias lineEdit: lineedit
    property alias minDisplay: minDisplay
    property alias maxDisplay: maxDisplay
    property int switchChecked

    property string minVal: ""
    property string maxVal: ""
    property bool refreshSwitch: false
    property int focusedItem: 0
    signal save()
    signal cancel()
    property var valid1: RegExpValidator
    {
        regExp:  /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/
    }
    property alias textFieldValidator: lineedit.validator

    MouseArea{
        anchors.fill: parent
        onClicked: {
        }
    }

    /**
       *@breif: To open the numpad
       **/
    function open(title,titleLocale, value, unit)
    {
        numPadStatus=0
        numpadTitle = title
        numpadTitleLocale = titleLocale
        keyInputText = value
        unitText = unit
        numpadPopupId.visible = true
        toggleSwicthInnerRec.visible = false
        lineedit.readOnly = false
        lineedit.setBackgroundColor("#ffffff")
        focusedItem = 0
    }

    /**
       *@breif: To close the numpad
       **/
    function close()
    {
        numpadTitle = qmlTextEmpty
		numpadTitleLocale = qmlTextEmpty
        keyInputText = qmlTextEmpty
        unitText = qmlTextEmpty
        numpadPopupId.visible = false
        focusedItem = 0
    }

    /**
       *@breif: To disable "0" to "9" and "." based on the parameter and its unit
       **/
    function disabled(operator)
    {
        if (keyInputText == qmlTextEmpty && !((operator >= qmltextDigit0 && operator <= qmltextDigit9) || operator === qmltextDot))
            return true


        else if (operator === qmltextDot && keyInputText.toString().search(/\./) != -1)
        {
            return true
        }
        else
        {
            return false
        }
    }

    /**
       *@breif: When clear is pressed
       **/
    function clearPressed()
    {
        if(lineedit.readOnly == false)
            lineedit.text = qmlTextEmpty
    }

    /**
       *@breif: When backspace is pressed in the numpad
       **/
    function operatorPressed (operator)
    {
        if(lineedit.readOnly == false)
        {
            if (operator === qmltextBackspace)
            {
                if(focusedItem == 0)
                    lineedit.text = qmlTextEmpty
                else
                {
                    var digitsClear = lineedit.text
                    digitsClear =  digitsClear.toString().slice(0,-1)
                    lineedit.text = digitsClear
                }
            }
        }
    }

    /**
       *@breif: When backspace is pressed in the numpad
       **/
    function digitPressed(digit)
    {
        if(lineedit.readOnly == false)
        {
            if(focusedItem == 0)
            {
                lineedit.text = qmlTextEmpty
                focusedItem = 1
            }

            if(focusedItem == 1)
            {
                var textInput = lineedit.text
                textInput += digit
                if(isValidData(textInput) == 1)
                    lineedit.text = textInput
            }
        }
    }

    /**
       *@breif: Validate the data based on the unit
       **/
    function isValidData(textInput)
    {
        var validateSec = /^\d{0,6}?$/;
        if(unitText == qmltextUnitSecLowerCase || unitText == qmltextUnitSecUpperCase)
        {
            validateSec = /^\d{0,2}(\.\d{0,3})?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == qmltextUnitMillimeter|| unitText == "mm/s2" || unitText == "mm/s")
        {
            if(unitText == "mm/s2") // use 5 digit for mm/s2 input
            {
                validateSec =/^\d{0,5}?$/;
            }
            else
            {
                /*For Collapse Distance Cutoff, Absolute Distance Cutoff, Expected Part Contact, Ready Offset,
                  Down Max Velocity and Return Max Velocity precision handling while data being entered*/

                if(sysconfig.getCurrentNameOfComboBox(qmlTextSystemType) === qmlSystemTypeASX)
                {
                    if(titleBarText.text === qsTr("ABSOLUTE DISTANCE CUTOFF") || titleBarText.text === qsTr("EXPECTED PART CONTACT")
                            || titleBarText.text === qsTr("READY OFFSET"))
                    {
                        validateSec =/^\d{0,3}(\.\d{0,2})?$/;
                    }

                    else if(titleBarText.text === qsTr("DOWN MAX VELOCITY") || titleBarText.text === qsTr("RETURN MAX VELOCITY"))
                    {
                        validateSec =/^\d{0,3}?$/;
                    }

                    else
                    {
                        validateSec =/^\d{0,2}(\.\d{0,2})?$/;
                    }
                }

                else
                {
                    if(titleBarText.text === qsTr("ABSOLUTE DISTANCE CUTOFF") || titleBarText.text === qsTr("EXPECTED PART CONTACT")
                            || titleBarText.text === qsTr("READY OFFSET"))
                    {
                        validateSec =/^\d{0,3}(\.\d{0,3})?$/;
                    }

                    else if(titleBarText.text === qsTr("DOWN MAX VELOCITY") || titleBarText.text === qsTr("RETURN MAX VELOCITY"))
                    {
                        validateSec =/^\d{0,3}?$/;
                    }

                    else
                    {
                        validateSec =/^\d{0,2}(\.\d{0,3})?$/;
                    }
                }
            }

            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == "in")
        {
            validateSec = /^\d{0,2}(\.\d{0,4})?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        /* use 3 digit with 2 decimal for in/s2 and in/s input */
        else if(unitText == "in/s2" || unitText == "in/s")
        {
            validateSec = /^\d{0,3}(\.\d{0,4})?$/;
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
        /*ActuatorSetUp screen force parameter numpad restriction on the basis of system unit */
        else if(unitText == "lbs"|| unitText =="LBS" ||unitText == "Lbs")
        {
            validateSec = /^\d{0,3}(\.\d{0,1})?$/;
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
        else if(unitText == qmltextUnitPercentage)
        {
            validateSec = /^\d{0,3}?$/;
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
        else if(unitText == qmltextIPV4 || unitText == qmltextIPV6)
        {
            return 1
        }
        else
        {
            if (validateSec.test(textInput))
                return 1
            else
                return 0
        }
    }

    Item {
        id: mainNumPad
        width: minWidth
        height: minHeight
        anchors.centerIn: parent
       // color: bgColor
        Rectangle {
            id: roundRect
            width:  parent.width
            height: 36
            radius: 5
            color: titleBGColor
        }

        Rectangle{
            id: title
            width: parent.width
            height: 30
            color: titleBGColor
            anchors.bottom : roundRect.bottom
            anchors.left : roundRect.left
            anchors.right : roundRect.right
            Text {
                id: titleBarText
                anchors.left: parent.left
                anchors.leftMargin: 20
                text:numpadTitleLocale
                color: titleColor
                font.family: Style.regular.name
                font.pixelSize: Style.style4
                anchors.verticalCenter: parent.verticalCenter
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
                    close()
                }
            }
        }

        Rectangle{
            id: toggleSwicthRec
            width: mainNumPad.width
            height: 35
            anchors.top : title.bottom
            color: bgColor
            Rectangle{
                id:  toggleSwicthInnerRec
                width: 0.9*parent.width
                height: 35
                anchors.fill: parent
                color: bgColor

                Text
                {
                    id: toggleSwicthText
                    anchors.left: parent.left
                    anchors.leftMargin: toggleSwicthRec.width*0.2//10
                    color: unitTextColor
                    text: qmltextControlLimit
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style5
                }
                ToogleBar
                {
                    id:toggleSwicth
                    anchors.left: toggleSwicthText.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    checked: false
                    onCheckedChanged:
                    {
                        if(checked == true)
                        {
                            lineedit.readOnly = false
                            lineedit.setBackgroundColor("#ffffff")
                            switchChecked = 1
                            focusedItem = 0
                        }
                        else if(checked == false)
                        {
                            lineedit.readOnly = true
                            lineedit.setBackgroundColor(unitTextColor)
                            switchChecked = 0
                            focusedItem = 0
                        }
                    }
                }
            }
        }

        Rectangle{
            id:numpadRec
            color: bgColor
            anchors.top : toggleSwicthRec.bottom
            anchors.left : title.left
            width: mainNumPad.width
            height: parent.height - roundRect.height - toggleSwicthRec.height

            CustomTextField
            {
                id: lineedit
                width: 190
                height: 40
                anchors.top:parent.top
                anchors.topMargin: 2
                anchors.left: parent.left
                anchors.leftMargin: lineedit.width/2-25
                readOnly: false
                onlyForNumPad: true
                fontSize: Style.style5
                Component.onCompleted: {
                    lineedit.bgColor = "#FFFFFF"
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
                    color: unitTextColor
                    text: unitText
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: Style.regular.name
                    font.pixelSize: Style.style3
                }
            }
            Text
            {
                id:minDisplay
                x:lineedit.x
                text:qmltextMin + minVal
                color: titleBGColor
                font.family: Style.regular.name
                font.pixelSize: Style.style3
                anchors.top:lineedit.bottom
                anchors.topMargin: 5
                visible: false
            }
            Text
            {
                id:maxDisplay
                text:qmltextMax + maxVal
                x: (lineedit.x + (lineedit.width*60/100))
                color: titleBGColor
                font.family: Style.regular.name
                font.pixelSize: Style.style3
                anchors.top:lineedit.bottom
                anchors.topMargin: 5
                visible: false
            }
            Grid {
                id:numGrid
                columns: 3
                columnSpacing: 10
                rowSpacing: 10
                anchors.top: lineedit.bottom
                anchors.topMargin: 38
                anchors.horizontalCenter: lineedit.horizontalCenter
                width: minWidthNumpad - 10
                height: minHeightNumpad

                PrimaryIconButton
                {
                    id:iconButton1
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit1
                    onReleased: {
                        digitPressed (iconButton1.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton2
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit2
                    onReleased: {
                        digitPressed (iconButton2.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton3
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit3
                    onReleased: {
                        digitPressed (iconButton3.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton4
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit4
                    onReleased: {
                        digitPressed (iconButton4.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton5
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit5
                    onReleased: {
                        digitPressed (iconButton5.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton6
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text:qmltextDigit6
                    onReleased: {
                        digitPressed (iconButton6.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton7
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit7
                    onReleased: {
                        digitPressed (iconButton7.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton8
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit8
                    onReleased: {
                        digitPressed (iconButton8.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButton9
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDigit9
                    onReleased: {
                        digitPressed (iconButton9.text)
                    }
                }

                PrimaryIconButton
                {
                    id:iconButtonDot
                    maxWidth: buttonSizeNumpad
                    maxHeight: buttonSizeNumpad
                    fontSize: Style.style5
                    text: qmltextDot
                    onReleased: {
                        digitPressed (iconButtonDot.text)
                    }
                }
            }
            SecondaryButton {
                id: deleteButton
                text:  qmltextBackSpaceSymbol
                anchors.left: numGrid.right
                anchors.leftMargin: -buttonSizeNumpad
                anchors.top: lineedit.bottom
                anchors.topMargin: 38
                width: iconButtonDot.width
                height: iconButtonDot.height*3 + 20
                textColor: titleBGColor
                fontSize: Style.style8

                onReleased: {
                    // keyPressed(lineedit.text)

                    operatorPressed(qmltextBackspace)
                }
            }
            PrimaryIconButton {
                id: customButton
                text: qmltextClr
                anchors.left: deleteButton.left
                anchors.top:deleteButton.bottom
                anchors.topMargin: 10
                maxWidth: buttonSizeNumpad
                maxHeight: buttonSizeNumpad
                fontSize: Style.style5
                onReleased: {
                    clearPressed()
                }
            }
            PrimaryIconButton {
                id: zeroButton
                text: qmltextDigit0
                maxHeight: buttonSizeNumpad
                maxWidth: buttonSizeNumpad
                fontSize: Style.style5
                width: buttonSizeNumpad * 2 + 10
                anchors.rightMargin: 10
                anchors.right: customButton.left
                anchors.top:deleteButton.bottom
                anchors.topMargin: 10
                onReleased: {
                    digitPressed (zeroButton.text)
                }
            }

            SecondaryButton {
                id: set
                anchors.top: zeroButton.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 15
                text: qmltextCancel
                fontSize: Style.style5
                onClicked: {
                    cancel()
                }
            }
            PrimaryButton {
                id: setandcontinue
                anchors.top: zeroButton.bottom
                anchors.topMargin: 20
                anchors.left: set.right
                anchors.leftMargin: 15
                text: qmltextOkay
                fontSize: Style.style5
                onClicked: {
                    save()
                }
            }
        }
    }
}
