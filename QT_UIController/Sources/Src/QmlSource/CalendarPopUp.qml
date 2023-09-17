import QtQuick 2.6
import Qt.labs.calendar 1.0
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.branson.eventlogging 1.0

Popup
{
    property alias calender: calendar
    readonly property string qmltextOk :  qsTr("OK")
    readonly property string qmltextCancel :  qsTr("CANCEL")
    readonly property string qmltexTimeDate: qsTr("Time and Date")
    readonly property string qmltexSuccess: qsTr("Successfully date and time has been modified")
    readonly property string qmltexFailed: qsTr("Failed to update date/time")
    readonly property string qmltexSpace: " "

    readonly property string qmltextDateTimeChangedSuccessMsg : qsTr("Date & Time changed successfully")
    readonly property string qmltextCommtReqMsg: qsTr("Comment is required")
    property DialogPopup dialogPopup

    property int formatTime: 0
    property int formatTimeConvert: 0

    property int returnValueFromBL
    id:calenderPopup
    modal: true
    focus: true
    dim: true
    closePolicy: Popup.CloseOnPressOutsideParent
    property alias calenderPopup: calenderPopup
    width : 1000
    height: 510
    x: (window.width-1000)/2
    y: (window.height-510)/2
    background: Rectangle{
        color: "transparent"
    }

    function setDateTime(date,time)
    {
        var splitHourMinList = time.split(":")
        var selectedHours = splitHourMinList[0]
        var selectedMinutesAndMeridian = splitHourMinList[1].split(" ")
        var selectedminutes = selectedMinutesAndMeridian[0]
        var selectedmeridian = selectedMinutesAndMeridian[1]
        if(selectedmeridian=== "AM")
        {
            if(selectedHours == 12)
            {
                selectedHours = selectedHours - 12
                selectedHours = "0"+selectedHours
            }
        }

        else if(selectedmeridian == "PM")
        {
            formatTime = selectedHours
            if(formatTime == 12)
            {
                formatTimeConvert = formatTime
            }
            else
                formatTimeConvert = formatTime + 12
            selectedHours = formatTimeConvert
        }

        var formatedDate = date + " "+selectedHours+":"+selectedminutes //+":"+ "00"
        return formatedDate
    }


    function createDialogpopup(message)
    {
        var comment = ""
        var component = Qt.createComponent("DialogPopup.qml")
        dialogPopup = component.createObject(window)
        dialogPopup.open(messagePopup.info_title, message)
        dialogPopup.onSave.connect(function onOkay()
        {
            comment = dialogPopup.quitMessage
            if (comment === "")
            {
                recipe.updateStatusText(qmltextCommtReqMsg)
            }
            else
            {
                sysconfig.getTimeAndDateFromSC()
                recipe.updateStatusText(qmltexSuccess)
                Eventlogger.eventSend(EventLogging.UI_EVENT_DATE_CHANGED, comment);
            }

            dialogPopup.close()
            dialogPopup.onSave.disconnect(onOkay)
            dialogPopup.destroy()
        })
        dialogPopup.onCancel.connect(function onCancel()
        {
            dialogPopup.close()
            dialogPopup.onCancel.disconnect(onCancel)
            dialogPopup.destroy()
        })
    }


    Rectangle{
        id:mainRectId
        width: parent.width //+475
        height: parent.height +58
        x: parent.x + 14.5
        color: "#F8F9FA"
        CustomCalendar
        {
            id: calendar
            width: parent.width/2
            height: parent.height- okcancelrect.height
            dateDisplayRectHeight: 50
        }

        CustomTimePicker
        {
            id:timePicker
            x:(parent.width/2)
            y:parent.y
            height: parent.height- okcancelrect.height
        }
        Rectangle
        {
            id: okcancelrect
            width: parent.width
            height: 50
            y: calendar.height
            color: "#F8F9FA"
            PrimaryButton{
                id : okbtn
                text: qmltextOk
                textColor: "#fff"
                anchors.right: parent.right
                anchors.rightMargin : 10
                anchors.top : parent.top
                anchors.topMargin: 5

                onClicked:
                {
                    var dateTime = calendar.userSelectedDateTOBL+qmltexSpace+timePicker.timeLabel.text
                    dateTime = setDateTime(calendar.userSelectedDateTOBL,timePicker.timeLabel.text)
                    calenderPopup.close()
                    sysconfig.startTimerThread()
                    mainwindow.hourTimer.restart()
                    disableMainwindowOpacity.start()
                    returnValueFromBL =  sysconfig.sendDateAndTimeToSC(dateTime)
                    if(returnValueFromBL == 1)
                    {
                        createDialogpopup(qmltextDateTimeChangedSuccessMsg)
                    }
                    else
                    {
                        recipe.updateStatusText(qmltexFailed)
                    }
                }
            }
            SecondaryButton{
                id : cancelbtn
                text:qmltextCancel
                textColor: qmlPrimaryColor
                anchors.right: parent.right
                anchors.rightMargin : 180
                anchors.top :  parent.top
                anchors.topMargin: 5
                onClicked:
                {
                    sysconfig.getTimeAndDateFromSC()
                    sysconfig.startTimerThread()
                    hourTimer.restart()
                    calenderPopup.close()
                    disableMainwindowOpacity.start()
                }
            }
        }
    }
}







