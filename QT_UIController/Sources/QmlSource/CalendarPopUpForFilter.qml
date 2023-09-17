import QtQuick 2.6
import Qt.labs.calendar 1.0
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Popup{
    readonly property string qmltextOk :  qsTr("OK")
    readonly property string qmltextCancel :  qsTr("CANCEL")
    readonly property string qmltexTimeDate: qsTr("Time and Date")
    readonly property string qmltexSuccess: qsTr("Successfully set")
    readonly property string qmltexFailed: qsTr("Failed")
    readonly property string qmltexSpace: qsTr(" ")

    property alias timePicker: timePicker
    property alias okbtn: okbtn
    property alias cancelbtn: cancelbtn
    property alias calendar: calendar
    property int okCancelbtnHeight: 30
    property int okCancelbtnWidth: 80
    property int okCancelRectHeight: 45
    signal updateCalender()

    property  string jan :"January"    // 0 //
    property  string feb :"February"   // 1 //
    property  string mar :"March"      // 2 //
    property  string apr :"April"      // 3 //
    property  string may :"May"        // 4 //
    property  string jun :"June"       // 5 //
    property  string jul :"July"       // 6 //
    property  string aug :"August"     // 7 //
    property  string sep :"September"  // 8 //
    property  string oct :"Octomber"   // 9  //
    property  string nov :"November"   // 10 //
    property  string dec :"December"    //11 //


    id:calenderPopup
    modal: true
    focus: true
    dim: true
    closePolicy: Popup.NoAutoClose

    /**
       *@breif: To set full date
       **/
    function setFullDate(fullDate)
    {
        var selectedDateSplitList = fullDate.split("-")
        var selectedYear = parseInt(selectedDateSplitList[0])
        var selectedMonth = parseInt(selectedDateSplitList[1])
        var selectedDateTime = selectedDateSplitList[2]
        var selectedDateTimeSplitList = selectedDateTime.split(qmltexSpace)
        var selectedDay = selectedDateTimeSplitList[0]
        var selectedMaridain = selectedDateTimeSplitList[2]
        var splitHourMinList = selectedDateTimeSplitList[1].split(":")
        var selectedHours = splitHourMinList[0]
        var selectedMinutes = splitHourMinList[1]

        setDateTime(selectedDay, selectedMonth, selectedYear, selectedHours, selectedMinutes, selectedMaridain)
    }

    /**
       *@breif: To set date and time
       **/
    function setDateTime(day, month, year, hours, minutes, meridian)
    {
        calendar.setDate(day, month, year)
        timePicker.setTime(hours, minutes, meridian)
        setDateDisplayText(month, year)
    }

    /**
       *@breif: To set date display text based on selcted month
       **/
    function setDateDisplayText(monthIndex, selectedYear){

        if(monthIndex === 1){
            calendar.dateDisplayText = jan +qmltexSpace+selectedYear
        }
        else if(monthIndex === 2){
            calendar.dateDisplayText = feb+qmltexSpace+selectedYear
        }
        else if(monthIndex === 3){
            calendar.dateDisplayText= mar+qmltexSpace+selectedYear
        }
        else if(monthIndex === 4){
            calendar.dateDisplayText = apr+qmltexSpace+selectedYear
        }
        else if(monthIndex === 5){
            calendar.dateDisplayText = may+qmltexSpace+selectedYear
        }
        else if(monthIndex === 6){
            calendar.dateDisplayText = jun+qmltexSpace+selectedYear
        }
        else if(monthIndex === 7){
            calendar.dateDisplayText = jul+qmltexSpace+selectedYear
        }
        else if(monthIndex === 8){
            calendar.dateDisplayText = aug+qmltexSpace+selectedYear
        }
        else if(monthIndex === 9){
            calendar.dateDisplayText = sep+qmltexSpace+selectedYear
        }
        else if(monthIndex === 10){
            calendar.dateDisplayText = oct+qmltexSpace+selectedYear
        }
        else if(monthIndex === 11){
            calendar.dateDisplayText = nov+qmltexSpace+selectedYear
        }
        else if(monthIndex === 12){
            calendar.dateDisplayText = dec+qmltexSpace+selectedYear
        }
        else{}
    }

    background: Rectangle{
        color: "transparent"
    }

    Rectangle{
        id:mainRectId
        width: 400//window.width/3
        height:250// window.height/3
        color: "#F8F9FA"
        clip:true

        CustomCalendar
        {
            id: calendar
            height: parent.height- okcancelrect.height
            dateDisplayRectHeight: 30
        }

        CustomTimePicker
        {
            id:timePicker
            x:(parent.width/2)
            height: parent.height- okcancelrect.height
            listviewLeftmargin: 45
            listView.currentIndex: 0
            meridianslistview.currentIndex: 0
            minuteslistview.currentIndex: 0
            hourTopLine.y :100
            hourBottomLine.y : 130
            listviewsYvalue : -75
            timeDisplayRectHeight : 30
            Component.onCompleted: {
                listView.delegateHeight = 40
                minuteslistview.delegateHeight = 40
                meridianslistview.delegateHeight = 40
            }
        }
        Rectangle
        {
            id: okcancelrect
            width: parent.width
            height: okCancelRectHeight
            y: calendar.height
            color: "#F8F9FA"
            PrimaryButton{
                id : okbtn
                text: qmltextOk
                width: okCancelbtnWidth
                height: okCancelbtnHeight
                textColor: "#fff"
                anchors.right: parent.right
                anchors.rightMargin : 10
                anchors.bottomMargin: 5
                anchors.bottom: parent.bottom
                onClicked:
                {
                    updateCalender()
                    calenderPopup.close()
                    disableMainwindowOpacity.start()
                }
            }
            SecondaryButton{
                id : cancelbtn
                text:qmltextCancel
                width: okCancelbtnWidth
                height: okCancelbtnHeight
                textColor: qmlPrimaryColor
                anchors.right: parent.right
                anchors.rightMargin : okbtn.width+20
                anchors.bottomMargin: 5
                anchors.bottom: parent.bottom
                onClicked:
                {
                    calenderPopup.close()
                    disableMainwindowOpacity.start()
                }
            }
        }
    }
}
