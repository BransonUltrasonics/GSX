import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0
import com.branson.menu 1.0

Rectangle
{
    readonly property string qmltextErrDesHeader :   qsTr("ERROR DESCRIPTION")
    readonly property string qmltextAlarm :   qsTr("ALARM")
    readonly property string qmltextErrDes :   qsTr("Clear all alarms before performing SETUP/SCAN/SEEK/WELD operation")
    readonly property string qmltextResetTxt :   qsTr("RESET")
    readonly property string qmltextHideTxt :   qsTr("HIDE")
    readonly property string qmltextResetSucess:    qsTr("Reset successful")
    readonly property string qmltextFailedReset:    qsTr("Failed to reset")
    readonly property string qmltextResetBatchCountSucess : qsTr("Batch counter reset successful")
    readonly property string qmltextResetBatchCountFail : qsTr("Failed to reset batch counter")
    readonly property string qmltextSystem : qsTr("SYSTEM")
    readonly property string qmltextSingleReport : qsTr("SINGLE REPORT")
    readonly property string qmltextProductionSetup : qsTr("TO PRODUCTION SETUP")
    readonly property string qmltextProductionSetUp : qsTr("PRODUCTION SETUP")
    readonly property string qmltextRecipes : qsTr("RECIPES")
    readonly property string qmltextClearBatchCounter : qsTr("CLEAR BATCH COUNTER")
    readonly property string qmltextBackUpDb : qsTr("BACKUP DB")

    id:alarmPopupWin
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false
    property alias alarmID: cycleNo.text
    property alias alarmDate: dateText.text
    property alias alarmTime: timeText.text
    property int resetStatus: 0
    property alias errorDes: errorDes.text
    property int returnValueFromSc
    property int success: 1
    property int alarmNotificationIndex: 0
    property alias customResetButton: customResetButton
    property alias customResetButton2: customResetButton2
    property alias customNotifyButton: customNotifyButton
    property alias toProductionSetupButton: toProductionSetupButton
    property alias clearBacthCounterButton: clearBacthCounterButton
    property string recipeInfo: ""
    property string alarmNumberInPopup: ""

    MouseArea{
        anchors.fill: parent
        onClicked: {
        }
    }

    /**
      *@breif: Opens the alarm popup
      **/
    function open()
    {
        alarmPopupWin.visible = true
    }

    /**
      *@breif: closes the alarm popup
      **/
    function close()
    {
        alarmPopupWin.visible = false
    }

    /**
    *@breif: Alarm Popup open
    **/
    function alarmPopupOpened()
    {
        if(alarmPopupOpenStatus == true)
        {
            alarmPopupOpenStatus = false
            alarmNotify.updateAlarmList(alarmNotificationIndex)
            alarmNumber.text = alarmNotify.getNoOfAlarms()
        }
    }

    /**
    *@breif: To reset all the alarm from SC
    **/
    function resetAllAlarmsFromSC()
    {
        if(cycleNo.text === "423" || cycleNo.text === "629")
        {
            alarmDetails.dbFullRemindReq()
        }
        else
        {
            alarmNotify.clearAlarmNotificationList()
            alarmNumber.text = alarmNotify.getNoOfAlarms()
            recipe.updateStatusText(qmltextResetSucess)
            alarmPopupWin.close()
        }
        alarmDetails.alarmPopupStatus(false)
        alarmNumber.text = alarmNotify.getNoOfAlarms()
        disableMainwindowOpacity.start()
    }

    /**
    *@breif: To reset the alarm
    **/
    function resetAlarm()
    {
        if(cycleNo.text === "423" || cycleNo.text === "629")
        {
            alarmDetails.dbFullRemindReq()
        }
        else
        {
            if(resetStatus == 1)
            {
                returnValueFromSc = alarmDetails.sendAlarmToSCToReset(cycleNo.text)
                if(returnValueFromSc == 1)
                {
                    alarmPopupOpened()
                    recipe.updateStatusText(qmltextResetSucess)
                    alarmPopupWin.close()
                }
                else
                {
                    recipe.updateStatusText("")
                    displayQuitPopup(messagePopup.error_title,qmltextFailedReset)
                }
            }
            else
            {
                alarmPopupOpened()
                recipe.updateStatusText(qmltextResetSucess)
                alarmPopupWin.close()
            }
        }
        alarmDetails.alarmPopupStatus(false)
        alarmNumber.text = alarmNotify.getNoOfAlarms()
        disableMainwindowOpacity.start()
    }

    /**
    *@breif: To set Alarm details like date, time, alarmid, description
    **/
    function setAlarmDetails(typeOfError,errorTime,errorDate,resetStatusFromNotify,index)
    {
        alarmNumberInPopup = typeOfError
        cycleNo.text = typeOfError + " : " + alarmDetails.errorNameforAlarm(alarmNumberInPopup)
        dateText.text = errorDate
        timeText.text = errorTime
        resetStatus = resetStatusFromNotify
        alarmNotificationIndex = index
        errorDes.text = alarmDetails.errorDescforAlarm(alarmNumberInPopup)
    }

    /**
    *@breif: To fill alarm notification list to the title bar
    **/
    function sendAlarmToNotificationList()
    {
        if(alarmPopupOpenStatus == true)
        {
            alarmPopupOpenStatus = false
        }
        else
        {
            alarmNotify.fillNotificationList(alarmNumberInPopup, timeText.text, dateText.text,resetStatus)
        }
        alarmDetails.alarmPopupStatus(false)
        alarmPopupWin.close()
        alarmNumber.text = alarmNotify.getNoOfAlarms()
        disableMainwindowOpacity.start()
    }

    Rectangle {
        id: alarmPopupRec
        width: window.width
        height: window.height
        x:0
        y:0
        color: "#ECECEC"
        border.color: "#ECECEC"
        Rectangle {
            id: alarmTitleRec
            width: parent.width
            height: 200
            color: qmlSecondaryColor
            border.color: qmlSecondaryColor
            Rectangle {
                id: alarmTitleImgRec
                width: 200
                height: 200
                color: "#000000"
                border.color: "#000000"
                Image {
                    id: image
                    width: 150
                    height: 150
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/images/image/part_icon_white.png"
                }
            }
            Text {
                id: titleText
                text: qmltextAlarm
                anchors.left: alarmTitleImgRec.right
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Style.style8
                font.family: Style.regular.name
                color: "#ffffff"
            }
        }

        Text {
            id: cycleNo
            text: qmlTextEmpty
            anchors.left: parent.left
            anchors.leftMargin: 200
            anchors.top:alarmTitleRec.bottom
            anchors.topMargin: 100
            font.pixelSize: Style.style7
            font.family: Style.regular.name
        }

        Text {
            id: errorDesTitle
            anchors.left: parent.left
            anchors.leftMargin: 200
            anchors.top:cycleNo.bottom
            anchors.topMargin: 40
            text: qmltextErrDesHeader
            font.pixelSize: Style.style3
            font.family: Style.regular.name
        }

        Text {
            id: errorDes
            width: 450
            anchors.left: parent.left
            anchors.leftMargin: 200
            anchors.top: errorDesTitle.bottom
            anchors.topMargin: 15
            text: qmltextErrDes
            font.pixelSize: Style.style3
            font.family: Style.regular.name
            wrapMode: Text.WordWrap
        }
        Rectangle{
            id:userRectangle
            width: 100
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 200
            anchors.top:errorDes.bottom
            anchors.topMargin: 50
            color: "#ECECEC"
            border.color: "#ECECEC"
            Image {
                id: userImg
                x:0
                sourceSize.width: 30
                sourceSize.height: 30
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/image/UserLogin.svg"
            }
            Text {
                id: userName
                anchors.left: userImg.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: PrimaryData.m_userName
                font.pixelSize: Style.style3
                font.family: Style.regular.name
            }
        }
        Rectangle{
            id:dateRectangle
            width: 100
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 200
            anchors.top:userRectangle.bottom
            anchors.topMargin: 15
            color: "#ECECEC"
            border.color: "#ECECEC"
            Image {
                id: dateImg
                x:0
                sourceSize.width: 30
                sourceSize.height: 30
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/image/date-time.svg"

            }
            Text {
                id: dateText
                anchors.left: dateImg.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: ""
                font.pixelSize: Style.style3
                font.family: Style.regular.name
            }
        }
        Rectangle{
            id:timeRectangle
            width: 100
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 200
            anchors.top:dateRectangle.bottom
            anchors.topMargin: 15
            color: "#ECECEC"
            border.color: "#ECECEC"
            Image {
                id: timeImg
                x:0
                anchors.verticalCenter: parent.verticalCenter
                sourceSize.width: 30
                sourceSize.height: 30
                source: "qrc:/images/image/Time.png"
            }
            Text {
                id: timeText
                anchors.left: timeImg.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: ""
                font.pixelSize: Style.style3
                font.family: Style.regular.name
            }
        }
        PrimaryButton {
            id: customResetButton
            text: qmltextResetTxt
            anchors.top:cycleNo.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 170
            visible: false
            enabled : isPermissionAllowed(Features.RESET_ALARAMS) ? true : false
            buttonColor : isPermissionAllowed(Features.RESET_ALARAMS) ? qmlPrimaryColor : "#757575"
            onClicked:
            {
               resetAlarm()
            }
        }
        PrimaryButton {
            id: customResetButton2
            text:qmltextBackUpDb
            anchors.top:customResetButton.bottom
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 170
            visible: false
            onClicked:
            {
                alarmPopup.close()
                alarmDetails.alarmPopupStatus(false)
                alarmDetails.dbFullRemindReq()
                loaderelement.setSource("SingleReport.qml",{"width":(400/1920)*window.width,"height":window.height})
                titlebar.text=qmltextSystem
                titlebarchild.visible=true
                titlebarchildtext.text=qmltextSingleReport
                titlbaronclicked="System.qml"
            }
        }
        SecondaryButton {
            id: customNotifyButton
            text:qmltextHideTxt
            anchors.top:customResetButton.bottom
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 170
            visible: false
            onClicked:
            {
                sendAlarmToNotificationList()
            }
        }
        PrimaryButton{
            id:toProductionSetupButton
            anchors.right: parent.right
            anchors.rightMargin: 170
            anchors.top:timeRectangle.bottom
            anchors.topMargin: 5
            visible: false
            text: qmltextProductionSetup
            onClicked:
            {
                var recipeParams
                recipeInfo = recipeSetupInfo.getActiveRecipeNumberForProduction()
                recipeParams = recipeInfo.split(",")
                recipeSetupInfo.recipeSetupInfoInit(recipeParams[0],recipeParams[1])
                loaderelement.setSource("RecipeProductionSetUp.qml",{"width":1920,"height":995})
                titlebarchildtext.text = qmltextProductionSetUp
                leftMenuData_obj.resetParent(qmltextRecipes)
                titlebar.text=qmltextRecipes
                titlebarchild.visible=true
                titlbaronclicked="RecipeListControl.qml"
                sendAlarmToNotificationList()
            }
        }
        PrimaryButton{
            id:clearBacthCounterButton
            anchors.right:toProductionSetupButton.left
            anchors.rightMargin: 30
            anchors.top:timeRectangle.bottom
            anchors.topMargin: 5
            visible: false
            text: qmltextClearBatchCounter
            onClicked: {
                var recipeParams
                recipeInfo = recipeSetupInfo.getActiveRecipeNumberForProduction()
                recipeParams = recipeInfo.split(",")
                recipeSetupInfo.recipeSetupInfoInit(recipeParams[0],recipeParams[1])
                returnValueFromSc = recipeSetupInfo.sendResetBatchCounterReqToSC()
                if(returnValueFromSc == 1)
                {
                    alarmPopupOpened()
                    displayQuitPopup(messagePopup.info_title,qmltextResetBatchCountSucess)
                }
                else
                {
                    displayQuitPopup(messagePopup.error_title,qmltextResetBatchCountFail)
                }
                sendAlarmToNotificationList()
            }
        }
    }
}

