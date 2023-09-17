import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0
import QtQuick.Controls 2.2

Item {

    readonly property string qmltextWeldMode : "Weld Mode"
    readonly property string qmltextGroundDetect : "Ground Detect"
    readonly property string qmltextScrubTime : "Scrub Time"
    readonly property string qmltextScrubAmp : "Scrub Amplitude"

    property real rectWidth:480.6
    property string paramText: ""
    property string paramTextLocale: ""
    property string paramMinValue: ""
    property string paramMaxValue: ""

    property string paramUnit: ""
    property string paramColor: "#ECECEC"
    property string paramTextColor: "#898d96"
    property string paramBoarderColor: qmlPrimaryColor
    property alias setup_Limit_mouseArea: setup_Limit_mouseArea
    property bool isTimeClick: false
    property bool isEnergyClick: false
    property bool isPeakPower : false
    property bool isAbsDist: false
    property bool isweldEnable: false
    property string modeName: ""
    property string weldmodeStr: ""
    property bool isSwitch: false
    property bool checkStatus: false
    property bool individualCheck: false
    property bool setupParameterVisible: true
    property bool disableMA: false
    property bool isRec: false
    property alias toggleSwitch: toggleSwitch
    property alias toggleSwitchChecked: toggleSwitch.checked

    Component.onCompleted: {
        if((individualCheck == true) && (isSwitch == false))
        {
            limitRanges.visible = true
            paramunit.visible = true
        }
        else
        {
            limitRanges.visible = false
            paramunit.visible = false
        }
        if(paramText == qmltextWeldMode)
        {
            paramText = weldmodeStr
        }

        mainSetupRec.visible = setupParameterVisible
        mainSetupRecDrp.visible = setupParameterVisible
    }

    DropShadow{
        id:mainSetupRecDrp
        anchors.fill: mainSetupRec
        horizontalOffset: 3
        verticalOffset: 3
        color: "#000000"
        opacity: 0.2
        source: mainSetupRec
    }
    Rectangle
    {
        id: mainSetupRec
        radius: 3
        width: parent.width
        height: parent.height
        color:mainSetupRec.pressed?paramColor:paramColor
        border.width: 0
        MouseArea
        {
            id:setup_Limit_mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: "PointingHandCursor"
            visible: (isSwitch == true) || (disableMA == true) ?false:true
        }
        Rectangle
        {
            id:paramStrip
            width: parent.height/5
            height: parent.height
            radius: 3
            color:qmlPrimaryColor
        }
        Text
        {
            id: paramtext
            text: paramTextLocale
            color: paramTextColor
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            anchors.left: paramStrip.right
            anchors.leftMargin: 10
            anchors.top:parent.top
            anchors.topMargin: 5
            font.capitalization: Font.AllUppercase
        }
        Text
        {
            id: paramunit
            text: paramUnit
            color: paramTextColor
            font.family: Style.semibold.name
            font.pixelSize:  Style.style2
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            //visible: true//isRec||isweldEnable
        }
        ToogleBar {
            id: toggleSwitch
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            checked: checkStatus
            visible: isSwitch
        }
//        Rectangle {
//            id: modeDisplayLbl
//            anchors.top: parent.top
//            anchors.topMargin: 17
//            anchors.right: parent.right
//            width : 170
//            visible:isweldEnable
//            Text {
//                id: modeTxt
//                font.family: Style.semibold.name
//                font.pixelSize:  Style.style1
//                color: paramTextColor
//                anchors.centerIn: parent
//                text: weldmodeStr
//                font.capitalization: Font.AllUppercase
//                visible: false
//            }
//        }


        /***********************VALUES DISPLAYING RECT*********************************************/
        Rectangle
        {
            id: limitRanges
            width: 170
            height: 30
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: paramunit.left
            anchors.rightMargin: 5
           // visible: (isRec||isweldEnable) //&& checkStatus
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
