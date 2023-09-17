import QtQuick 2.0
import QtQuick 2.5
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import Style 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.2

Item {
    readonly property string qmltextShowing :qsTr("Showing")
    readonly property string qmltextFilterText:qsTr("ALARMS")
    readonly property string qmtextReset: qsTr("RESET")
    readonly property string qmtextApply: qsTr("APPLY")
    readonly property string qmltextToDate: qsTr("To Date/Time")
    readonly property string qmltextFromDate: qsTr("From Date/Time")
    readonly property string qmltextOr: qsTr("OR")
    readonly property string qmltextToCycle: qsTr("To Cycle")
    readonly property string qmltextFromCycle: qsTr("From Cycle")
    readonly property string qmltextCycleModify: qsTr("Cycle Modified")
    readonly property string qmltextAllWeld: qsTr("All Alarm")
    readonly property string qmltextSuspect: qsTr("Suspect")
    readonly property string qmltextReject: qsTr("Reject")
    readonly property string qmltextInvalidValueMsg: qsTr("Invalid value is entered. Please Check")
    readonly property string qmltextEmptyMsg: qsTr(" ")

    property bool filterRectVisible: false
    property int  textLabelWidthForFilter: 120
    property int  textLabelHeightForFilter: 40
    property int  lineEditWidthForFilter: 180
    property int  lineEditHeightForFilter: 40
    property int  dateImgWidthForFilter: 35
    property int  dateImgHeightForFilter: 35
    property int  numpadImgWidthForFilter: 35
    property int  numpadImgHeightForFilter: 35
    property int customCheckboxHeight :50
    property int customCheckboxWidth :140
    property int customOrTextWidth :40
    property int formatTime: 0
    property int formatTimeConvert: 0

    property  int validateVal : 1
    property  string fetchResult: "1"
    property  string fromDateBL: "0"
    property  string toDateBL: "0"
    property  string fromCycleBL: "0"
    property  string toCycleBL: "0"
    property string formatedDate: "0"
    property bool  allWeldStatus : false
    property bool  suspectStatus : false
    property bool  rejectStatus : false
    property bool  weldsWithAlarmsStatus : false
    property bool  cycleModifiedStatus : false
    property bool closeFilterFlag:false

    property bool collapased: false
    property int  currentIndex : 0
    Rectangle{
        id:alarmRec
        width: parent.width
        height: parent.height - mainStatusbarHeight
        y:15
        color: "#ffffff"
        border.color: "#ffffff"
        border.width: 1
        Rectangle {
            id: filterRec
            width: parent.width
            height: 40
            anchors.top:parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            color: "#ECECEC"
            border.color: "#ECECEC"
            border.width: 2
            Image {
                id: filterImage
                width: 35
                height: 35
                anchors.rightMargin: 25
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/image/filter.svg"
                visible: false
            }
            Text {
                id: filtertext1
                x: 10
                y: 19
                text: qmltextShowing
                font.pixelSize: Style.style1
                font.family: Style.semibold.name
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: filtertext2
                x: 84
                y: 19
                text: qmltextFilterText
                font.pixelSize: Style.style1
                font.family: Style.regular.name
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea {
                id: filtermouseArea
                anchors.fill: parent
                width: filterRec.width
                height: filterRec.height
//                onClicked: {
//                    if(filterRectVisible == false) {
//                        filterBGRect.visible = true
//                        filteranim.start()
//                        filterRectVisible = true
//                    }
//                    else {
//                        filteranim.stop()
//                        filterBGRect.visible = false
//                        filterRectVisible = false
//                    }
//                }
            }
        }

        Rectangle{
            id:alarmListRec
            width: parent.width
            height: parent.height - 80
            anchors.top:filterRec.bottom
            anchors.topMargin: 0
            color: "#ECECEC"
            clip: true
            ListView {
                id: alarmlistView1
                x:alarmListRec.x
                width: parent.width
                height: parent.height
                spacing: 10
                anchors.left: alarmListRec.left
                anchors.leftMargin: 10
                anchors.top:alarmListRec.top
                anchors.bottom: alarmListRec.bottom
                anchors.bottomMargin: 10
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                model:analyticModel
                focus: true
                headerPositioning: ListView.OverlayHeader
                cacheBuffer: 0
                header: AlarmTitleDelegate
                {
                    id: titlesRec
                    z:2
                    width: parent.width*(98.5/100)
                    height: filterRec.height
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                }
                delegate: AlarmListDelegate{
                    id:alarmListDelegateRect
                    width: parent.width*(98.5/100)
                    height: 60
                    resetStatus: model.modelData.Reset
                    MouseArea {
                        id:alarmdelMA
                        width: parent.width
                        height: 60
                        onClicked: {
                            if(index == currentIndex) {

                                if(collapased == false) {
                                    collapased = true
                                    visiblityRect = true
                                    alarmListDelegateRect.height = 260
                                    animateRect.start()
                                }
                                else {
                                    collapased = false
                                    visiblityRect = false
                                    alarmListDelegateRect.height = 60
                                    animateRect.stop()
                                }

                            } else {

                                if(alarmListDelegateRect.height == 260) {
                                    animateRect.stop()
                                    collapased = false
                                    visiblityRect = false
                                    alarmListDelegateRect.height = 60
                                } else {
                                    collapased = true
                                    visiblityRect = true
                                    alarmListDelegateRect.height = 260
                                    animateRect.start()
                                }

                            }
                            currentIndex = index
                        }
                    }
                }
                ScrollBar.horizontal: ScrollBar{}
                ScrollBar.vertical: ScrollBar {}
            }
        }

        Rectangle {
            id: filterBGRect
            visible: false
            y:filterRec.y-filterBGRect.height
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            height: parent.height*0.7
            width: parent.width
            color: "#ECECEC"
            border.color: "grey"
            border.width: 0.5
            z:2

            NumberAnimation on y{
                id:filteranim
                from:0
                to:filterRec.y+80
                duration: 500
            }
            Rectangle {
                id:daterectId
                anchors.top:parent.top
                anchors.topMargin: 20
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.horizontalCenter:  parent.horizontalCenter
                width: parent.width*(0.6)
                color: "#ECECEC"

                //selecting fromDate and toDate
                Row{
                    id:row1
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    spacing:5
                    Text {
                        id:blankText
                        verticalAlignment: Text.AlignVCenter
                        height: textLabelHeightForFilter
                        width: customOrTextWidth
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                    Text {
                        id: fromDateLabel
                        text: qmltextFromDate
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        height: textLabelHeightForFilter
                        width: textLabelWidthForFilter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                    LineEdit {
                        id:fromDateId
                        height: lineEditHeightForFilter
                        width: lineEditWidthForFilter
                        enabled: false
                        maximumLength: 20
                        Component.onCompleted:  {
                            text = calenderPopupForFilter1.calendar.dateDisplayText+" "+calenderPopupForFilter1.timePicker.timeLabel.text
                        }
                        onTextChanged: {
                            row2.opacity = 0.5
                            fromCycleImg.enabled =false
                            toCycleImg.enabled =false
                        }

                    }
                    Image {
                        id:fromDateImg
                        source: "qrc:/images/image/date-time.svg"
                        fillMode: Image.PreserveAspectFit
                        width: dateImgWidthForFilter
                        height: dateImgHeightForFilter

                        MouseArea
                        {
                            anchors.fill:fromDateImg
                            onClicked: {
                                fetchResult = "1"
                                openCalenderPopUp1(fromDateId.text)
                                calenderPopupForFilter1.updateCalender.connect(function saveText()
                                {
                                    if(calenderPopupForFilter1.calendar.userSelectedDate==qmlTextEmpty){
                                        fromDateId.text =calenderPopupForFilter1.calendar.currentSysDate+" "+calenderPopupForFilter1.timePicker.timeLabel.text
                                    }
                                    else{
                                        fromDateId.text = calenderPopupForFilter1.calendar.userSelectedDate +" "+calenderPopupForFilter1.timePicker.timeLabel.text
                                    }
                                    calenderPopupForFilter1.updateCalender.disconnect(saveText)
                                    calenderPopupForFilter1.close()
                                    disableMainwindowOpacity.start()
                                })
                            }
                        }
                    }

                    Text {
                        id: toDateLabel
                        text: qmltextToDate
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        height: textLabelHeightForFilter
                        width: textLabelWidthForFilter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                    LineEdit {
                        id:toDateId
                        height: lineEditHeightForFilter
                        width: lineEditWidthForFilter
                        enabled: false
                        maximumLength: 20
                        Component.onCompleted:  {
                            text = calenderPopupForFilter2.calendar.dateDisplayText+" "+calenderPopupForFilter2.timePicker.timeLabel.text
                        }
                        onTextChanged: {
                            row2.opacity = 0.5
                            fromCycleImg.enabled =false
                            toCycleImg.enabled =false
                        }
                    }
                    Image {
                        id:toDateImg
                        source: "qrc:/images/image/date-time.svg"
                        fillMode: Image.PreserveAspectFit
                        width: dateImgWidthForFilter
                        height: dateImgHeightForFilter
                        MouseArea
                        {

                            anchors.fill:toDateImg
                            onClicked: {
                                fetchResult = "1"
                                openCalenderPopUp2(toDateId.text)
                                calenderPopupForFilter2.updateCalender.connect(function saveText()
                                {
                                    if(calenderPopupForFilter2.calendar.userSelectedDate==qmlTextEmpty)
                                        toDateId.text =calenderPopupForFilter2.calendar.currentSysDate+" "+calenderPopupForFilter2.timePicker.timeLabel.text
                                    else
                                        toDateId.text = calenderPopupForFilter2.calendar.userSelectedDate +" "+calenderPopupForFilter2.timePicker.timeLabel.text

                                    calenderPopupForFilter2.updateCalender.disconnect(saveText)
                                    calenderPopupForFilter2.close()
                                    disableMainwindowOpacity.start()
                                })
                            }
                        }
                    }
                }
                //selecting fromCycle and toCycle
                Row{
                    id:row2
                    anchors.top: parent.top
                    anchors.topMargin: 80
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    spacing:5

                    Text {
                        id: orText
                        text: qmltextOr
                        verticalAlignment: Text.AlignVCenter
                        height: textLabelHeightForFilter
                        width: customOrTextWidth
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                    Text {
                        id: fromCycleLabel
                        text: qmltextFromCycle
                        verticalAlignment: Text.AlignVCenter
                        height: textLabelHeightForFilter
                        width: textLabelWidthForFilter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                    LineEdit {
                        id:fromCycleId
                        height: lineEditHeightForFilter
                        width: lineEditWidthForFilter
                        enabled: false
                        onTextChanged: {
                            row1.opacity = 0.5
                            fromDateImg.enabled = false
                            toDateImg.enabled = false
                        }
                    }
                    Image {
                        id:fromCycleImg
                        source: "qrc:/images/image/custom-numpad.svg"
                        fillMode: Image.PreserveAspectFit
                        width: numpadImgWidthForFilter
                        height: numpadImgHeightForFilter
                        MouseArea
                        {
                            id:ma
                            width: parent.width
                            height:parent.height
                            onClicked: {
                                fetchResult = "2"
                                openNumPadPopUp("From Cycle",fromCycleLabel.text,1,fromCycleId.text)
                            }
                        }
                    }

                    Text {
                        id: toCycleLabel
                        text: qmltextToCycle
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        height: textLabelHeightForFilter
                        width: textLabelWidthForFilter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                    LineEdit {
                        id:toCycleId
                        height: lineEditHeightForFilter
                        width: lineEditWidthForFilter
                        enabled: false
                        onTextChanged: {
                            row1.opacity = 0.5
                            fromDateImg.enabled = false
                            toDateImg.enabled = false
                        }
                    }
                    Image {
                        id:toCycleImg
                        source: "qrc:/images/image/custom-numpad.svg"
                        fillMode: Image.PreserveAspectFit
                        width: numpadImgWidthForFilter
                        height: numpadImgHeightForFilter
                        MouseArea
                        {
                            anchors.fill:toCycleImg
                            onClicked: {
                                fetchResult = "2"
                                openNumPadPopUp("To Cycle",toCycleLabel.text,2,toCycleId.text)
                            }
                        }
                    }

                }

                Canvas {
                    id:drawingCanvas
                    anchors.top:row2.bottom
                    anchors.topMargin:5
                    anchors.left: daterectId.left
                    width: daterectId.width
                    height: 50
                    onPaint: {
                        // Get drawing context
                        var context = getContext("2d");

                        // Draw a line
                        context.beginPath();
                        context.lineWidth = 1;
                        context.moveTo(5,50)
                        context.lineTo(drawingCanvas.width-10,50)
                        context.strokeStyle = "black"
                        context.stroke();
                    }
                }
                Row{
                    id:checkBoxRow1
                    anchors.top: drawingCanvas.bottom
                    anchors.topMargin: 30
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    spacing:50
                    visible: false
                    CustomCheckBox {
                        id:cycleModifiedCheckBox
                        checkBoxStatus: false
                        enabled: true
                        checkBoxName: qmltextCycleModify
                        width: customCheckboxWidth
                        height: customCheckboxHeight
                    }

                    CustomCheckBox {
                        id:allWeldsCheckBox
                        checkBoxStatus: false
                        enabled: true
                        checkBoxName: qmltextAllWeld
                        width: customCheckboxWidth
                        height: customCheckboxHeight
                    }

                }
                Row{
                    id:checkBoxRow2
                    anchors.top:checkBoxRow1.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    spacing:50
                    visible: false
                    CustomCheckBox {
                        id:suspectCheckBox
                        checkBoxStatus: false
                        enabled: true
                        checkBoxName:qmltextSuspect
                        width: customCheckboxWidth
                        height: customCheckboxHeight
                    }
                    CustomCheckBox {
                        id:rejectCheckBox
                        checkBoxStatus: false
                        enabled: true
                        checkBoxName:qmltextReject
                        width: customCheckboxWidth
                        height: customCheckboxHeight
                    }

                }
                SecondaryButton {
                    id:resetFilterId
                    anchors.top:drawingCanvas.bottom
                    anchors.topMargin: 20
                    anchors.left: drawingCanvas.right
                    anchors.leftMargin: 20
                    text:qmtextReset
                    onClicked: {
                        //clear all fields
                        resetAllFields()
                    }
                }
                PrimaryButton {
                    id:applyFilterId
                    anchors.top: resetFilterId.bottom
                    anchors.topMargin: 20
                    anchors.leftMargin: 20
                    anchors.left:  drawingCanvas.right
                    text:qmtextApply
                    onClicked: {
                        applyChanges()
                    }
                }
                CalendarPopUpForFilter
                {
                    id:calenderPopupForFilter1
                    x: (window.width-1000)/2
                    y: (window.height)/7
                }
                CalendarPopUpForFilter
                {
                    id:calenderPopupForFilter2
                    x: (window.width-1000)/2
                    y: (window.height)/7
                }
            }
        }

    }

    /**
    *@breif: To set full date
    **/
    function setFullDate(fullDate)
    {
        var selectedDateSplitList = fullDate.split("-")
        var selectedYear = parseInt(selectedDateSplitList[0])
        // var selectedMonth = parseInt(selectedDateSplitList[1])
        var selectedMonth = selectedDateSplitList[1]
        var selectedDateTime = selectedDateSplitList[2]
        var selectedDateTimeSplitList = selectedDateTime.split(" ")
        var selectedDay = selectedDateTimeSplitList[0]
        var selectedMaridain = selectedDateTimeSplitList[2]
        var splitHourMinList = selectedDateTimeSplitList[1].split(":")
        var selectedHours = splitHourMinList[0]
        var selectedMinutes = splitHourMinList[1]

        return setDateTime(selectedDay, selectedMonth, selectedYear, selectedHours, selectedMinutes, selectedMaridain)
    }

    /**
    *@breif: To set date and time
    **/
    function setDateTime(day, month, year, hours, minutes, meridian)
    {
        if(meridian=== "AM")
        {
            if(hours == 12)
            {
                hours = hours - 12
                hours = "0"+hours
            }
        }

        else if(meridian == "PM")
        {
            formatTime = hours
            formatTimeConvert = formatTime + 12
            hours = formatTimeConvert
        }

        formatedDate = year+"/"+month+"/"+day+" "+hours+":"+minutes+":"+ "00"
        return formatedDate
    }

    /**
    *@breif: Hide all controls
    **/
    function applyChanges()
    {
        var filterData=qmlTextEmpty
        if((fromDateId.text == qmlTextEmpty && toDateId.text == qmlTextEmpty) && (fromCycleId.text == qmlTextEmpty && toCycleId.text == qmlTextEmpty))
        {
            allWeldStatus = allWeldsCheckBox.checkBoxStatus
            suspectStatus = suspectCheckBox.checkBoxStatus
            rejectStatus = rejectCheckBox.checkBoxStatus
            cycleModifiedStatus = cycleModifiedCheckBox.checkBoxStatus

            closeFilterFlag = true

            if(fromDateId.text != qmlTextEmpty)
            {
                fromDateBL = setFullDate(fromDateBL)
            }
            if(toDateId.text != qmlTextEmpty)
            {
                toDateBL = setFullDate(toDateBL)
            }

            filterData =   fetchResult + ","+fromDateBL+","+toDateBL+","+fromCycleBL+","+toCycleBL+","+allWeldStatus+","+suspectStatus+","+rejectStatus+","+cycleModifiedStatus+","+weldsWithAlarmsStatus

        }
        else if(fromDateId.text == qmlTextEmpty && toDateId.text == qmlTextEmpty)
        {
            if(fromCycleId.text!=qmlTextEmpty && toCycleId.text!=qmlTextEmpty)
            {
                fromCycleBL = fromCycleId.text
                toCycleBL = toCycleId.text
                allWeldStatus = allWeldsCheckBox.checkBoxStatus
                suspectStatus = suspectCheckBox.checkBoxStatus
                rejectStatus = rejectCheckBox.checkBoxStatus
                cycleModifiedStatus = cycleModifiedCheckBox.checkBoxStatus

                closeFilterFlag = true

                if(fromDateId.text != qmlTextEmpty)
                {
                    fromDateBL = setFullDate(fromDateBL)
                }
                if(toDateId.text != qmlTextEmpty)
                {
                    toDateBL = setFullDate(toDateBL)
                }

                filterData =   fetchResult + ","+fromDateBL+","+toDateBL+","+fromCycleBL+","+toCycleBL+","+allWeldStatus+","+suspectStatus+","+rejectStatus+","+cycleModifiedStatus+","+weldsWithAlarmsStatus
            }
            else{
                if(fromCycleId.text==qmlTextEmpty)
                    displayWarningPopup(fromCycleLabel.text)
                else
                    displayWarningPopup(toCycleLabel.text)
            }
        }
        else if(fromCycleId.text == qmlTextEmpty && toCycleId.text == qmlTextEmpty)
        {
            if(fromDateId.text!=qmlTextEmpty && toDateId.text!=qmlTextEmpty)
            {
                fromDateBL = fromDateId.text
                toDateBL = toDateId.text
                allWeldStatus = allWeldsCheckBox.checkBoxStatus
                suspectStatus = suspectCheckBox.checkBoxStatus
                rejectStatus = rejectCheckBox.checkBoxStatus
                cycleModifiedStatus = cycleModifiedCheckBox.checkBoxStatus

                closeFilterFlag = true

                if(fromDateId.text != qmlTextEmpty)
                {
                    fromDateBL = setFullDate(fromDateBL)
                }
                if(toDateId.text != qmlTextEmpty)
                {
                    toDateBL = setFullDate(toDateBL)
                }

                filterData =   fetchResult + ","+fromDateBL+","+toDateBL+","+fromCycleBL+","+toCycleBL+","+allWeldStatus+","+suspectStatus+","+rejectStatus+","+cycleModifiedStatus+","+weldsWithAlarmsStatus
            }
            else{
                if(fromDateId.text==qmlTextEmpty)
                    displayWarningPopup(fromDateLabel.text)
                else
                    displayWarningPopup(toDateLabel.text)
            }
        }
        else{ }

        if(closeFilterFlag)
        {
            filteranim.stop()
            filterBGRect.visible = false
            filterRectVisible = false
            closeFilterFlag = false
            //  alarmdelMA.enabled = true
        }
       // analyticalarm.updateAnalyticsAlarmFilterData(filterData)
    }

    function displayWarningPopup(msg){
         recipe.updateStatusText(msg+qmltextEmptyMsg)
    }

    function resetAllFields()
    {
        toDateId.text = qmlTextEmpty
        fromDateId.text = qmlTextEmpty
        toCycleId.text = qmlTextEmpty
        fromCycleId.text = qmlTextEmpty
        fetchResult = "1"
        fromCycleBL=qmlTextEmpty
        toCycleBL="0"
        fromDateBL="0"
        toDateBL="0"
        cycleModifiedCheckBox.checkBoxStatus = false
        allWeldsCheckBox.checkBoxStatus = false
        suspectCheckBox.checkBoxStatus = false
        rejectCheckBox.checkBoxStatus = false


        row2.opacity = 1.0
        row1.opacity = 1.0
        fromCycleImg.enabled = true
        toCycleImg.enabled = true
        fromDateImg.enabled = true
        toDateImg.enabled = true

        calenderPopupForFilter1.timePicker.listView.currentIndex =  sysconfig.TitleBarDate.getHours()>12?sysconfig.TitleBarDate.getHours()%12:sysconfig.TitleBarDate.getHours()
        calenderPopupForFilter1.timePicker.minuteslistview.currentIndex =sysconfig.TitleBarDate.getMinutes()
        calenderPopupForFilter1.timePicker.meridianslistview.currentIndex =sysconfig.TitleBarDate.getHours()>=12?1:0

        calenderPopupForFilter2.timePicker.listView.currentIndex =  sysconfig.TitleBarDate.getHours()>12?sysconfig.TitleBarDate.getHours()%12:sysconfig.TitleBarDate.getHours()
        calenderPopupForFilter2.timePicker.minuteslistview.currentIndex =sysconfig.TitleBarDate.getMinutes()
        calenderPopupForFilter2.timePicker.meridianslistview.currentIndex =sysconfig.TitleBarDate.getHours()>=12?1:0

        analyticalarm.updateAnalyticsAlarmsList()

    }
	property CustomNumPad cNumpad
    function openNumPadPopUp(title,titleLocale,index,value)
    {
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)
        cNumpad.open(title,titleLocale, value, qmlTextEmpty)
        /* On clicking Done button in the numpad */
        cNumpad.onSave.connect(function okay()
        {
            validateVal = parseInt(cNumpad.keyInputText)
            if((validateVal >= 0) && (validateVal <= 100000))
            {
                if(index===1)
                {
                    fromCycleId.text = parseInt(cNumpad.keyInputText)
                }
                else if(index === 2)
                {
                    toCycleId.text = parseInt(cNumpad.keyInputText)
                }
                cNumpad.close()
                cNumpad.onSave.disconnect(okay)
                cNumpad.destroy()
            }
            else
            {
                displayQuitPopup(messagePopup.warning_title, qmltextInvalidValueMsg)
            }
        })
        /* On clicking Cancel button in the numpad */
        cNumpad.onCancel.connect(function cancel()
        {
            cNumpad.close()
            cNumpad.onCancel.disconnect(cancel)
            cNumpad.destroy()
        })
    }

    function openCalenderPopUp1(str)
    {
        if(str === qmlTextEmpty){
            str = calenderPopupForFilter1.calendar.currentSysDate+" "+calenderPopupForFilter1.timePicker.timeLabel.text
            splitDate(1,str)
        }
        else{
            splitDate(1,str)
        }
        calenderPopupForFilter1.open()
        enableMainwindowOpacity.start()
    }

    function openCalenderPopUp2(str)
    {
        if(str === qmlTextEmpty){
            str = calenderPopupForFilter2.calendar.currentSysDate+" "+calenderPopupForFilter2.timePicker.timeLabel.text
            splitDate(2,str)
        }
        else{
            splitDate(2,str)
        }
        calenderPopupForFilter2.open()
        enableMainwindowOpacity.start()
    }

    function splitDate(ind,fullDate){
        if(ind===1)
        {
            calenderPopupForFilter1.setFullDate(fullDate)
        }
        else if(ind===2)
        {
            calenderPopupForFilter2.setFullDate(fullDate)
        }
        else{}
    }


}
