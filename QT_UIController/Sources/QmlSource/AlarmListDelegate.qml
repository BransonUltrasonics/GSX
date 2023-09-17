import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0

Item {
    id:listRectDelegateMA
    property alias glowEffect: effect
    property alias listRectDelegateMA: listRectDelegateMA
    property alias visiblityRect: dropDownRect.visible
    readonly property string qmltextPartError : qsTr("Part Error")
    readonly property string qmltextABSCutOff :qsTr("ABS Cutoff")
    readonly property string qmltextlistModel :qsTr("####")
    readonly property string qmltextRecipeName :qsTr("Recipe Name or Number")
    readonly property string qmltextlistStatus :qsTr("New")
    readonly property string qmltextDescription :qsTr("Description")
    readonly property string qmltextReset :qsTr("RESET")
    readonly property string qmltextunResolvedMsg :qsTr("Failed to reset")
    readonly property string qmltextResults :qsTr("RESULTS")
    readonly property string qmltextUserID: qsTr("Username")
    readonly property string qmltextAlarmType: qsTr("Alarm Type")
    readonly property string qmltextPartID: qsTr("Part#")
    readonly property string qmltextNoAlarmDesc : "N/A"


    property string userIdStr: mainwindow.usernameTextTitleBar
    property int  resetStatus: 0
    property alias animateRect: alarmanimId
    property int returnValueFromBL
    width: parent.width
    height: parent.height
    RectangularGlow {
        id: effect
        anchors.fill: listRect
        glowRadius: 3
        spread: 0.2
        color: "#000000"
        opacity: 0.2
        height: listRect.height
        width: listRect.width
        cornerRadius: listRect.radius + glowRadius
    }
    Rectangle{
        id:listRect
        anchors.top:parent.top
        anchors.topMargin: 10
        width: parent.width
        height: parent.height      
        color: "#FFFFFF"
        Image {
            id: list1img
            sourceSize.width: 40
            sourceSize.height: 40
            anchors.top:parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            source: model.modelData.AlarmType
        }
        Text {
            id: list1Error
            anchors.left:parent.left
            anchors.leftMargin: 60
            width: parent.width/9
            height: 60
            color: "#898D96"
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            text: model.modelData.AlarmName
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            id: list1AlarmID
            anchors.left: parent.left
            anchors.leftMargin: parent.width/6.0
            height: 60
            width: parent.width/9
            color: "#898D96"
            text: model.modelData.AlarmId
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: list1RecipeName
            anchors.left: parent.left
            anchors.leftMargin: parent.width/3.8
            color: "#898D96"
            width: parent.width/9
            text: model.modelData.RecipeNo
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            height: 60
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            id: recipeVer
            color: "#898D96"
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            text:model.modelData.RecipeVerNo
            anchors.left: parent.left
            anchors.leftMargin:parent.width/2.8
            height: 60
            verticalAlignment: Text.AlignVCenter
            width: parent.width/9
        }
        Text {
            id: list1Part
            anchors.left: parent.left
            anchors.leftMargin: parent.width/2.1
            height: 60
            verticalAlignment: Text.AlignVCenter
            color: "#898D96"
            text: model.modelData.CycleCounter
            width: parent.width/9
            font.pixelSize: Style.style1
            font.family: Style.regular.name
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
        }

        Text {
            id: list1Time
            anchors.left: parent.left
            anchors.leftMargin: parent.width/1.7
            height: 60
            verticalAlignment: Text.AlignVCenter
            color: "#898D96"
            text: model.modelData.AlarmDate+" "+model.modelData.AlarmTime
            width: parent.width/9
            font.pixelSize: Style.style1
            font.family: Style.regular.name
        }

        Rectangle {
            id:dropDownRect
            visible: false
            anchors.left:parent.left
            y:parent.y-parent.height
            clip:true
            anchors.right: parent.right
            height: 200
            NumberAnimation on y{
                id:alarmanimId
                from: 0
                to: 50
                duration: 400
            }
            RectangularGlow {
                id:innerGlowRect
                anchors.top :dropDownRect.top
                anchors.topMargin: 10
                glowRadius: 1
                spread: 0.1
                color: "#000000"
                opacity: 0.2
                height: 0.3
                width: listRect.width
                cornerRadius: listRect.radius + glowRadius
            }

            MouseArea
            {
                id:innerRect
                anchors.fill: parent
            }
            Image {
                id: list2img
                sourceSize.width: 80
                sourceSize.height: 80
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top:parent.top
                anchors.topMargin: 20
                source: model.modelData.AlarmType
            }
            Text {
                id: alarmDescriptionTitleId
                anchors.left: parent.left
                anchors.leftMargin: 150
                anchors.top:parent.top
                anchors.topMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                fontSizeMode: Text.HorizontalFit
                font.pixelSize: Style.style1
                font.family: Style.semibold.name
                text: qmltextDescription
            }
            Text {
                id: alarmIDTitleDesc
                anchors.left: alarmDescriptionTitleId.left
                anchors.leftMargin: 350
                anchors.top:parent.top
                anchors.topMargin: 20
                text: qmltextUserID
                fontSizeMode: Text.HorizontalFit
                font.pixelSize: Style.style1
                font.family: Style.semibold.name
            }
            Text {
                id: alarmTypeTitleDesc
                anchors.left: alarmIDTitleDesc.left
                anchors.leftMargin: 250
                anchors.top:parent.top
                anchors.topMargin: 20
                text: qmltextAlarmType
                fontSizeMode: Text.HorizontalFit
                font.pixelSize: Style.style1
                font.family: Style.semibold.name
            }
            Text {
                id: alarmDescriptionId
                anchors.left: alarmDescriptionTitleId.left
                anchors.top:parent.top
                anchors.topMargin: 60
                color: "#898D96"
                width: 300
                wrapMode:Text.WordWrap
                text: model.modelData.AlarmDescription
                font.pixelSize: Style.style1
                font.family: Style.regular.name
            }
            Text {
                id: alarmIDDesc
                anchors.left: alarmDescriptionId.left
                anchors.leftMargin: 350
                anchors.top:parent.top
                anchors.topMargin: 60
                color: "#898D96"
                text: (model.modelData.Username === "")?qmltextNoAlarmDesc:model.modelData.Username
                fontSizeMode: Text.HorizontalFit
                font.pixelSize: Style.style1
                font.family: Style.regular.name
            }
            Text {
                id: errorTypeDesc
                anchors.left: alarmIDDesc.left
                anchors.leftMargin: 250
                anchors.top:parent.top
                anchors.topMargin: 60
                color: "#898D96"
                text: model.modelData.ErrorType
                fontSizeMode: Text.HorizontalFit
                font.pixelSize: Style.style1
                font.family: Style.regular.name
            }
            PrimaryButton {
                id:resetButtonId
                anchors.right: parent.right
                anchors.rightMargin: 40
                anchors.top:parent.top
                anchors.topMargin: 20
                text:qmltextReset
                visible:(model.modelData.Reset == 1) ? true : false
                width: 100
                height: 32
                onClicked: {
                    returnValueFromBL = alarmDetails.sendAlarmToSCToReset(model.modelData.AlarmId)
                    if(returnValueFromBL == 1)
                    {
                        alarmNotify.updateAlarmListFromAnalyticsAlarm(model.modelData.AlarmId)
                        alarmNumber.text = alarmNotify.getNoOfAlarms()
                        analyticalarm.updateAnalyticsAlarmsList()
                    }
                    else
                    {
                        messagePopup.open(messagePopup.error_title,qmltextunResolvedMsg, messagePopup.ok_button)
                    }
                }
            }
            SecondaryButton {
                id:notifyButtonId
                anchors.right: parent.right
                anchors.rightMargin: 40
                anchors.top:resetButtonId.bottom
                anchors.topMargin: 12
                text:qmltextResults
                visible:true
                width: 100
                height: 32
                onClicked: {
                    loaderelementAnalysis.setSource("AnalyticsResults.qml",{indexofLoader:0})
                }
            }
        }
    }
}
