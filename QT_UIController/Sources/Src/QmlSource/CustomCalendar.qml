import QtQuick 2.0
import QtCharts 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.calendar 1.0
import Style 1.0
Item {
    id:calendarItem
    visible: true

    property  alias dateDisplayText: dateDisplayText.text
    property  alias gridDate:gridDate
    property  var userSelectedDate : ""
    property  var userSelectedDateTOBL: ""
    property  int calendarRectWidth:parent.width/2
    property  int calendarRectHeight:parent.height-70
    property  int dateDisplayRectHeight: 35
    property  string dateStr: ""
    property  string formatDateStr: ""
    property  string systemDate: sysconfig.TitleBarDate
    property  var currentSysDate : ""
    property  int year :2018
    property  int month :1
    property  int date : 1
    property  string jan :qsTr("January")    // 0 //
    property  string feb :qsTr("February")   // 1 //
    property  string mar :qsTr("March")      // 2 //
    property  string apr :qsTr("April")      // 3 //
    property  string may :qsTr("May")        // 4 //
    property  string jun :qsTr("June")       // 5 //
    property  string jul :qsTr("July")       // 6 //
    property  string aug :qsTr("August")     // 7 //
    property  string sep :qsTr("September")  // 8 //
    property  string oct :qsTr("October")    // 9 //
    property  string nov :qsTr("November")   // 10//
    property  string dec :qsTr("December")   // 11//
    property  bool selected: false

    property int currentIndex: 0

    function setCurrentDate()
    {
        systemDate          = sysconfig.TitleBarDate
        var sysDateList     = systemDate.split("T")
        currentSysDate      = sysDateList[0]
        var dateSplitList   = sysDateList[0].split("-")
        year                = dateSplitList[0]
        month               = dateSplitList[1]
        date                = dateSplitList[2]
        setDate(date,month,year)
    }

    /**
       *@breif: To set Date
       **/
    function setDate(day, month, year)
    {
        gridDate.month = month-1
        gridDate.year = year
        var localDate = new Date()
        localDate.setFullYear(year)
        localDate.setMonth(month-1)
        localDate.setDate(day)
        dateDisplayText.text = localDate.toLocaleDateString(locale,"yyyy-MM-dd").toString()
        userSelectedDateTOBL = localDate.toLocaleDateString(locale,"yyyy/MM/dd").toString()
    }

    /**
       *@breif: Open Date rectangle to select date, which is corresponding to the selected month
       **/
    function  openDateRect(index){
        if(index == 0){
            gridDate.month = index
            dateDisplayText.text = jan +" "+year
        }
        else if(index == 1){
            gridDate.month = index
            dateDisplayText.text = feb+" "+year
        }
        else if(index == 2){
            gridDate.month = index
            dateDisplayText.text = mar+" "+year
        }
        else if(index == 3){
            gridDate.month = index
            dateDisplayText.text = apr+" "+year
        }
        else if(index == 4){
            gridDate.month = index
            dateDisplayText.text = may+" "+year
        }
        else if(index == 5){
            gridDate.month = index
            dateDisplayText.text = jun+" "+year
        }
        else if(index == 6){
            gridDate.month = index
            dateDisplayText.text = jul+" "+year
        }
        else if(index == 7){
            gridDate.month = index
            dateDisplayText.text = aug+" "+year
        }
        else if(index == 8){
            gridDate.month = index
            dateDisplayText.text = sep+" "+year
        }
        else if(index == 9){
            gridDate.month = index
            dateDisplayText.text = oct+" "+year
        }
        else if(index == 10){
            gridDate.month = index
            dateDisplayText.text = nov+" "+year
        }
        else{
            gridDate.month = index
            dateDisplayText.text = dec+" "+year
        }

        gridMonthRect.visible = false
        gridDate.visible =  true
        gridYearRect.visible = false
    }

    /**
       *@breif: Opens month rectangle to select month
       **/
    function  openMonthRect(year){
        dateDisplayText.text = year
        gridDate.year = year
        gridMonthRect.visible = true
        gridDate.visible =   false
        gridYearRect.visible = false
    }

    Rectangle{
        width:calendarRectWidth// parent.width
        height:calendarRectHeight//parent.height
        Rectangle{
            width: parent.width
            height:parent.height
            Rectangle{
                id:titleRect
                width: calendarRectWidth+5
                height:dateDisplayRectHeight
                color: qmlPrimaryColor
                Text{
                    id:dateDisplayText
                    width: parent.width
                    height:parent.height
                    text:currentSysDate
                    color: "white"
                    font.pixelSize:  20
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    MouseArea{
                        anchors.fill:dateDisplayText
                        onClicked: {

                            if(gridDate.visible==true)
                            {
                                gridMonthRect.visible = true
                                dateDisplayText.text = year
                                gridDate.visible = false
                                gridYearRect.visible = false
                            }
                            else if(gridMonthRect.visible==true)
                            {
                                gridMonthRect.visible = false
                                gridYearRect.visible = true
                                dateDisplayText.text = year
                                gridDate.visible = false

                            }
                            else {}
                        }
                    }
                }
            }
            Rectangle{
                y: titleRect.height
                width: calendarRectWidth
                height: calendarRectHeight
                clip:true
                border.color: "lightgray"
                border.width: 1

                MonthGrid {

                    id:gridDate
                    width:parent.width
                    height:parent.height
                    spacing: 3
                    visible: true
                    month: month
                    year: year
                    locale: Qt.locale("en_US")

                    delegate:

                        Label  {
                        id:labelId
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: model.day
                        color:"gray"
                        font.pixelSize: Style.style2
                        font.family: Style.semibold.name

                        Rectangle{
                            id:rectBg
                            width: parent.width
                            height: parent.height
                            color: qmlPrimaryColor
                            opacity: 0.7
                            visible: false//currentIndex === index
                            radius: rectBg.width/2
                        }

                        MouseArea {
                            id: clicker
                            anchors {
                                fill: parent
                            }
                            onClicked: {
                                labelId.forceActiveFocus ();
                                userSelectedDate = date.toLocaleDateString(locale,"yyyy-MM-dd")//(locale,"ddd yyyy-MM-dd ")
                                dateDisplayText.text = userSelectedDate
                                userSelectedDateTOBL=date.toLocaleDateString(locale,"yyyy/MM/dd")
                                currentIndex = index
                            }
                        }
                    }
                }

                //month
                Rectangle{
                    id:gridMonthRect
                    width: parent.width
                    height:parent.height
                    visible: false
                    GridView{
                        id: monthGridView
                        width: parent.width
                        height:parent.height-10
                        anchors.top: parent.topR
                        anchors.topMargin: 3
                        model: monthModel
                        cellWidth: parent.width/3-1
                        cellHeight:parent.width/3-1
                        delegate:
                            Rectangle {
                            width: yearGridView.cellWidth
                            height:yearGridView.cellHeight
                            id: monthRect
                            border.color: "lightgray"
                            border.width:1
                            Text {
                                width: parent.width
                                height:parent.height
                                text: model.modelData
                                font.pixelSize: Style.style2
                                font.family: Style.semibold.name
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            MouseArea{
                                id:ma
                                anchors.fill: parent
                                onClicked: {
                                    openDateRect(index)
                                }
                            }
                        }
                    }
                }

                //year
                Rectangle{
                    id:gridYearRect
                    width: parent.width
                    height:parent.height
                    visible: false
                    GridView{
                        id: yearGridView
                        width: parent.width
                        height:parent.height-10
                        cellWidth: parent.width/3-1
                        cellHeight:parent.width/3-1
                        model: yearModel
                        delegate:
                            Rectangle {
                            width: yearGridView.cellWidth
                            height:yearGridView.cellHeight
                            id: yearRect
                            border.color: "lightgray"
                            border.width:1
                            Text {
                                width: parent.width
                                height:parent.height
                                text: model.modelData
                                font.pixelSize: Style.style2
                                font.family: Style.semibold.name
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    year =parseInt(model.modelData)
                                    openMonthRect(year)
                                }
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    customCalendarObj.initializeMonthYearModel()
                }
            }
        }
    }
}
