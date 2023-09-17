import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Style 1.0
Item {

    property real rectWidth:480.6
    property alias tab_griditem: rectangle1
    property string paramText: ""
    property string paramTextLocale: ""
    property string suspectLow: ""
    property string suspectHigh: ""
    property string rejectLow: ""
    property string rejectHigh: ""
    property bool rejectStatus: false
    property bool suspectStatus: false
    property bool suspectLowStatus: false
    property bool suspectHighStatus: false
    property bool rejectLowStatus: false
    property bool rejectHighStatus: false
    property string paramColor: "#ECECEC"
    property string paramRecColor: "#ECECEC"
    property string paramTextColor: "#898d96"
    property string paramBoarderColor: qmlPrimaryColor
    property alias suspect_Reject_mouseArea: suspect_Reject_mouseArea
    property string suspectOffColor: "#D9E8FA"    
    property bool isSwitch: false
    property bool checkStatus: false
    property alias toggleSwitch: toggleSwitch
    property alias toggleSwitchChecked: toggleSwitch.checked
    property int paramBoarderWidth: 0

    Component.onCompleted:
    {
        if(suspectStatus == false && rejectStatus == false )
        {
            suspectLow = ""
            suspectHigh = ""
            suspectLowRec.visible = false
            suspectHighRec.visible = false
            goodRec.visible = false
            rejectHigh = ""
            rejectLow = ""
            rejectLowRec.visible = false
            rejectHighRec.visible = false
        }
        else
        {
            if (suspectStatus == false)
            {
                suspectLow = ""
                suspectHigh = ""
                suspectHighRec.color = suspectOffColor
                suspectLowRec.color = suspectOffColor
                suspectHighRec.border.color = suspectOffColor
                suspectLowRec.border.color = suspectOffColor
            }
            if (rejectStatus == false)
            {
                rejectHigh = ""
                rejectLow = ""
                rejectHighRec.color = paramRecColor
                rejectLowRec.color = paramRecColor
                rejectHighRec.border.color = paramRecColor
                rejectLowRec.border.color = paramRecColor
            }
        }
        if(suspectLowStatus == 0)
            suspectLow = ""
        if(suspectHighStatus == 0)
            suspectHigh = ""
        if(rejectLowStatus == 0)
            rejectLow = ""
        if(rejectHighStatus == 0)
            rejectHigh = ""
    }

    DropShadow{
        anchors.fill: rectangle1
        horizontalOffset: 3
        verticalOffset: 3
        color: "#000000"
        opacity: 0.2
        source: rectangle1
    }
    Rectangle
    {
        id: rectangle1
        x: 0
        y: 0
        radius: 3
        width: parent.width
        height: parent.height
        color:rectangle1.pressed?paramRecColor:paramRecColor
        border.color: paramBoarderColor
        border.width: paramBoarderWidth
        MouseArea
        {
            id:suspect_Reject_mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: "PointingHandCursor"
            visible:  isSwitch?false:true
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
        ToogleBar {
            id: toggleSwitch
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            checked: checkStatus
            visible: isSwitch
        }
        Rectangle{
            id: limitRanges
            width: 250
            height: 30
            anchors.bottom:  rectangle1.bottom
            anchors.bottomMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 10
            color: paramRecColor
            border.color: paramRecColor
            visible: isSwitch?false:true
            Rectangle{
                id:rejectLowRec
                width: limitRanges.width*(1/5)
                height: limitRanges.height
                x:0
                y:0
                color: qmlSecondaryColor
                border.color: qmlSecondaryColor
                Text {
                    id:rejectLowText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: rejectLow
                    wrapMode: Text.WordWrap
                    color: "#ffffff"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style2
                }
            }
            Rectangle{
                id:suspectLowRec
                width: limitRanges.width*(1/5)
                height: limitRanges.height
                anchors.left: rejectLowRec.right
                anchors.verticalCenter: parent.verticalCenter
                color: qmlPrimaryColor
                border.color: qmlPrimaryColor
                Text {
                    id:suspectLowText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: suspectLow
                    wrapMode: Text.WordWrap
                    color: "#ffffff"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style2
                }
            }
            Rectangle{
                id: goodRec
                width: limitRanges.width*(1/5)
                height: limitRanges.height
                anchors.left: suspectLowRec.right
                anchors.verticalCenter: parent.verticalCenter
                color: "#D9E8FA"
                border.color: "#D9E8FA"
                Image {
                    id: goodImg
                    sourceSize.width: parent.width
                    sourceSize.height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/image/checkmark_green.svg"
                }
            }
            Rectangle{
                id:suspectHighRec
                width: limitRanges.width*(1/5)
                height: limitRanges.height
                anchors.left: goodRec.right
                anchors.verticalCenter: parent.verticalCenter
                color: qmlPrimaryColor
                border.color: qmlPrimaryColor
                Text {
                    id:suspectHighText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: suspectHigh
                    wrapMode: Text.WordWrap
                    color: "#ffffff"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style2
                }

            }
            Rectangle{
                id:rejectHighRec
                width: limitRanges.width*(1/5)
                height: limitRanges.height
                anchors.left: suspectHighRec.right
                anchors.verticalCenter: parent.verticalCenter
                color: qmlSecondaryColor
                border.color: qmlSecondaryColor
                Text {
                    id:rejectHighText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: rejectHigh
                    wrapMode: Text.WordWrap
                    color: "#ffffff"
                    font.family: Style.semibold.name
                    font.pixelSize: Style.style2
                }
            }
        }
    }
}
