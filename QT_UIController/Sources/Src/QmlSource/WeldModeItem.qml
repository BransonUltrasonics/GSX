import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0

Item {

    property real rectWidth:480.6
    property alias control_mode_mouseArea: control_mode_mouseArea
    property alias rectangle1: rectangle1
    property string paramText: ""
    property string paramTextLocale: ""
    property string paramValue: ""
    property string paramUnit: ""
    property string paramColor: "#ECECEC"
    property string paramTextColor: "#898d96"
    property string paramBoarderColor: qmlPrimaryColor
    property string paramStripColor: qmlPrimaryColor
    property string doubleLeftText: ""
    property string doubleRightText: ""
    property int paramBoarderWidth: 0
    property bool isSwitch: false
    property bool isDoubleSwitch: false
    property bool isTripleButton: false
    property bool checkStatus: false
    property bool refreshDoubleSwitch: false
    property bool refreshTripleSwitch: false
    property alias toggleSwitch: toggleSwitch
    property alias toggleSwitchChecked: toggleSwitch.checked
    property alias toggleDoubleSwitch: toggleDoubleSwitch
    property alias toggleDoubleSwitchChecked: toggleDoubleSwitch.checked
    property alias switchInside: toggleDoubleSwitch.switchID
    property alias tripleRadioButtons:tripleRadioButtons
    property string paramMinValue: "2"
    property string paramMaxValue: "7"

    Component.onCompleted: {
        if(paramText.toLowerCase() == "expected contact window")
            limitRanges.visible = true
        else
            limitRanges.visible = false
    }

    DropShadow{
        source: rectangle1
        anchors.fill: rectangle1
        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.2
        radius: 3
    }
    Rectangle
    {
        id: rectangle1
        x: 0
        y: 0
        radius: 3
        width: parent.width
        height: parent.height
        color:rectangle1.pressed?paramColor:paramColor
        border.color: paramBoarderColor
        border.width: paramBoarderWidth
        MouseArea
        {
            id:control_mode_mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: "PointingHandCursor"
            visible: isSwitch | isTripleButton ?false:true
        }
        Rectangle
        {
            id:paramStrip
            width: parent.height/5
            height: parent.height
            radius: 3
            color:paramStripColor
        }

        Text
        {
            id: paramtext
            text: qsTr(paramTextLocale)
            color: paramTextColor
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            anchors.left: paramStrip.right
            anchors.leftMargin: 10
            anchors.top:parent.top
            anchors.topMargin: 5
            font.capitalization: Font.AllUppercase
            visible: isDoubleSwitch?false:true
        }
        Text
        {
            id: paramvalue
            text: qsTr(paramValue)
            color: paramTextColor
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            anchors.right:paramunit.left
            anchors.rightMargin: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            visible: isSwitch| isTripleButton?false:true
        }
        Text
        {
            id: paramunit
            text: qsTr(paramUnit)
            color: paramTextColor
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            visible: isSwitch?false:true
        }

        ToogleBar {
            id: toggleSwitch
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            checked: checkStatus
            visible: isSwitch&&!isDoubleSwitch
        }

        DoubleSideToogleBar
        {
            id: toggleDoubleSwitch
            leftText: qsTr(doubleLeftText)
            rightText: qsTr(doubleRightText)
            checked: checkStatus
            visible: isDoubleSwitch

            anchors.left: parent.left
            anchors.leftMargin: 100
            anchors.verticalCenter: parent.verticalCenter
            switchID.onPressedChanged:
            {
                refreshDoubleSwitch = true
            }
            switchID.onCheckedChanged:
            {
                if(refreshDoubleSwitch == true)
                {
                    recipe.setParamaterChangedFlag(0)
                    refreshDoubleSwitch = false
                    if(toggleDoubleSwitchChecked)
                        recipe.setStructureValue(paramText,1)
                    else
                        recipe.setStructureValue(paramText,0)
                }
            }
        }
        TripleRadioButtons
        {
            id:tripleRadioButtons
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            visible: isTripleButton
            customRadioBtnLow.onPressedChanged:
            {
                refreshTripleSwitch = true
            }
            customRadioBtnLow.onCheckedChanged:
            {
                if(refreshTripleSwitch == true)
                {
                    refreshTripleSwitch = false
                    if(customRadioBtnLow.checked == true)
                    {
                        customRadioBtnLowStatus = true
                        customRadioBtnMedStatus = false
                        customRadioBtnHighStatus = false
                        recipe.setStructureValue(paramText,1)
                    }
                    else
                        customRadioBtnLowStatus = false
                }
            }
            customRadioBtnMed.onPressedChanged:
            {
                refreshTripleSwitch = true
            }
            customRadioBtnMed.onCheckedChanged:
            {
                if(refreshTripleSwitch == true)
                {
                    refreshTripleSwitch = false
                    if(customRadioBtnMed.checked == true)
                    {
                        customRadioBtnLowStatus = false
                        customRadioBtnMedStatus = true
                        customRadioBtnHighStatus = false
                        recipe.setStructureValue(paramText,2)
                    }
                    else
                        customRadioBtnMedStatus = false
                }
            }
            customRadioBtnHigh.onPressedChanged:
            {
                refreshTripleSwitch = true
            }
            customRadioBtnHigh.onCheckedChanged:
            {
                if(refreshTripleSwitch == true)
                {
                    refreshTripleSwitch = false
                    if(customRadioBtnHigh.checked == true)
                    {
                        customRadioBtnLowStatus = false
                        customRadioBtnMedStatus = false
                        customRadioBtnHighStatus = true
                        recipe.setStructureValue(paramText,3)
                    }
                    else
                        customRadioBtnHighStatus = false
                }
            }
        }

        Rectangle{
            id: limitRanges
            width: 170
            height: 30
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: paramunit.left
            anchors.rightMargin: 5
            visible: false
            Rectangle{
                id:setupLimitLowRec
                width: limitRanges.width*(1/3)
                height: limitRanges.height
                anchors.verticalCenter: parent.verticalCenter
                color: qmlPrimaryColor
                Text {
                    id: lowValLbl
                    text: paramMinValue
                    color: "white"
                    anchors.centerIn: setupLimitLowRec
                    font.pixelSize:  Style.style1
                }
            }
            Rectangle{
                id:setupLimitHighRec
                width: limitRanges.width*(1/3)
                height: limitRanges.height
                anchors.left: goodRec.right
                anchors.verticalCenter: parent.verticalCenter
                color: qmlPrimaryColor
                Text {
                    id: highValLbl
                    text: paramMaxValue
                    color: "white"
                    anchors.centerIn: setupLimitHighRec
                    font.pixelSize:  Style.style1
                }
            }
            Rectangle{
                id: goodRec
                width: parent.width*(1/3)
                height: limitRanges.height
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                color: "#D9E8FA"
                Image {
                    id: goodImg
                    sourceSize.width: 25//parent.width
                    sourceSize.height: 25//parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/image/checkmark_green.svg"
                }
            }
        }
    }
}
