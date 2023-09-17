import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0
import com.branson.menu 1.0

Rectangle {

    readonly property string qmltextHeaderText :   qsTr("NOTIFICATIONS")
    readonly property string qmltextResetFailed : qsTr("Failed to Reset")
    readonly property string qmltextAlarmId71A :   "71A"
    readonly property string qmltextResetAll :   qsTr("RESET ALL")

    id:alarmNotificationPopupWindow
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false
    property int returnValueFromSc: 0
    property int alarmNo: 0
    property alias alarmlistView: alarmlistView1
    property string typeOfErrorLocal
    property int success: 1
    property alias parentNotifyRecHeight: parentNotifyRec.height
    property alias customResetButton: customResetButton
    /**
      *@breif: Opens the alarm popup
      **/
    function open()
    {
        alarmNotificationPopupWindow.visible = true
        customResetButton.enabled = isPermissionAllowed(Features.RESET_ALARAMS) ? true : false
        customResetButton.buttonColor = isPermissionAllowed(Features.RESET_ALARAMS) ? qmlPrimaryColor : "#757575"
    }

    /**
      *@breif: closes the alarm popup
      **/
    function close()
    {
        alarmNotificationPopupWindow.visible = false
    }

    Rectangle{
        id: parentNotifyRec
        width:  window.width*(50/100)
        height: 170
        x: window.width*(25/100)
        y: window.height*(35/100)
        color: "#ECECEC"
        border.color: "#ECECEC"
        Rectangle{
            id: cornerRec
            width:  window.width*(50/100)
            height: 10
            color: qmlPrimaryColor
            border.color: qmlPrimaryColor
            radius: 3
            anchors.top:parent.top
            anchors.topMargin: -3
        }

        Rectangle{
            id: headerRec
            width:  window.width*(50/100)
            height: 30
            x:0
            color: qmlPrimaryColor
            border.color: qmlPrimaryColor
            anchors.top:parent.top
            anchors.topMargin: 0
            Text {
                id: headerText
                text: qmltextHeaderText
                anchors.left: parent.left
                anchors.leftMargin: 10
                color: "#ffffff"
                font.pixelSize: Style.style3
                font.family: Style.regular.name
                anchors.verticalCenter: parent.verticalCenter
            }
            Image {
                id: image
                sourceSize.width: 30
                sourceSize.height: 30
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                /*Image icon update based on System Type*/
                source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/close_ASX.svg" :
                                                                   "qrc:/images/image/close.svg"
            }
            MouseArea {
                id: mouseArea
                width: 40
                height: 40
                x: image.x
                y: image.y
                anchors.right: parent.right
                anchors.rightMargin: 10
                onClicked: {
                    alarmNotificationPopupWindow.close()
                    disableMainwindowOpacity.start()
                }
                onEntered: {
                    cursorShape=Qt.PointingHandCursor
                }
                z:1
            }
        }

        Rectangle {
            id: alarmNotifyPopupRec
            width: window.width*(50/100)
            height: parentNotifyRec.height - 80
            color: "#ECECEC"
            border.color: "#ECECEC"
            anchors.top:headerRec.bottom
            anchors.topMargin: 0
            ListView {
                id: alarmlistView1
                width: parent.width
                height: alarmNotifyPopupRec.height - 15
                anchors.left: alarmNotifyPopupRec.left
                anchors.leftMargin:10
                anchors.top : alarmNotifyPopupRec.top
                anchors.topMargin: 10
                spacing: 10
                model: alarmNotificationListModel
                clip: true
                cacheBuffer: 0
                delegate: AlarmNotificationListDelegate
                {
                    width: alarmlistView1.width - 20
                    height: 70
                    typeOfError: model.modelData.m_AlarmNameFromDB
                    errorTime: model.modelData.m_AlarmTimeFromDB
                    errorDate: model.modelData.m_AlarmDateFromDB
                    resetStatus: model.modelData.m_Reset
                    list_MA.onClicked:{
                        alarmlistView1.currentIndex = index
                        alarmPopupOpenStatus = true
                        alarmPopup.setAlarmDetails(model.modelData.m_AlarmNameFromDB,errorTime,errorDate,resetStatus,alarmlistView1.currentIndex)
                        alarmPopup.customResetButton.enabled = isPermissionAllowed(Features.RESET_ALARAMS) ? true : false
                        alarmPopup.customResetButton.buttonColor = isPermissionAllowed(Features.RESET_ALARAMS) ? qmlPrimaryColor : "#757575"
                        if(typeOfError == qmltextAlarmId71A || typeOfError == "71a")
                        {
                            alarmPopup.toProductionSetupButton.visible = true
                            alarmPopup.clearBacthCounterButton.visible = true
                            alarmPopup.customNotifyButton.visible = true
                            alarmPopup.customResetButton.visible = true
                            alarmPopup.customResetButton2.visible = false
                            alarmPopup.clearBacthCounterButton.enabled = isPermissionAllowed(Features.RESET_BATCH_COUNTER) ? true : false
                            alarmPopup.clearBacthCounterButton.buttonColor = isPermissionAllowed(Features.RESET_BATCH_COUNTER) ? qmlPrimaryColor : "#757575"
                        }
                        else if(typeOfError == "423")
                        {
                            alarmPopup.customResetButton2.visible = true
                            alarmPopup.customNotifyButton.visible = false
                            alarmPopup.customResetButton.visible = true
                            alarmPopup.customResetButton.text = "REMIND LATER"
                            alarmPopup.toProductionSetupButton.visible = false
                            alarmPopup.clearBacthCounterButton.visible = false
                        }
                        else if(typeOfError == "629")
                        {
                            alarmPopup.customResetButton2.visible = true
                            alarmPopup.customResetButton.visible = false
                            alarmPopup.customNotifyButton.visible = false
                            alarmPopup.toProductionSetupButton.visible = false
                            alarmPopup.clearBacthCounterButton.visible = false
                        }
                        else if(typeOfError == "631")
                        {
                            alarmPopup.customResetButton2.visible = false
                            alarmPopup.customResetButton.visible = false
                            alarmPopup.customNotifyButton.visible = false
                            alarmPopup.toProductionSetupButton.visible = false
                            alarmPopup.clearBacthCounterButton.visible = false
                        }
                        else if(typeOfError === "63e" || typeOfError === "63E")
                        {
                            alarmPopup.customResetButton2.visible = false
                            alarmPopup.customResetButton.visible = true
                            alarmPopup.customNotifyButton.visible = true
                            alarmPopup.customResetButton.enabled = false
                            alarmPopup.customResetButton.buttonColor = "#757575"
                            alarmPopup.toProductionSetupButton.visible = false
                            alarmPopup.clearBacthCounterButton.visible = false
                        }
                        else
                        {
                            alarmPopup.customResetButton2.visible = false
                            alarmPopup.customResetButton.visible = true
                            alarmPopup.customNotifyButton.visible = true

                            alarmPopup.toProductionSetupButton.visible = false
                            alarmPopup.clearBacthCounterButton.visible = false
                        }
                        alarmDetails.alarmPopupStatus(true)
                        alarmPopup.open()
                        alarmNotificationPopupWindow.close()
                    }
                }
                ScrollBar.vertical: ScrollBar{}
            }
        }

        Rectangle{
            id: resetAllRec
            width: window.width*(50/100)
            height: 50
            anchors.top: alarmNotifyPopupRec.bottom
            color: "#ECECEC"
            border.color: "#ECECEC"
            PrimaryButton{
                id: customResetButton
                text: qmltextResetAll
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 20
                onClicked:
                {
                    returnValueFromSc = alarmDetails.sendResetAllRequestToSC()
                    if(returnValueFromSc == 1)
                    {
                        alarmNotify.clearAlarmNotificationList()
                        alarmNotificationPopupWindow.close()
                        alarmNumber.text = alarmNotify.getNoOfAlarms()
                    }
                    else
                    {
                        displayQuitPopup(messagePopup.error_title,qmltextResetFailed)
                    }
                    analyticalarm.updateAnalyticsAlarmsList()
                }
            }
        }
    }
}
