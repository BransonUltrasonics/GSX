import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Style 1.0

Item {
    id: item1
    width: 1280
    height: 750
    readonly property string qmltextInvalidValueMsg : qsTr("Invalid value is entered, Reverted to old value")
    readonly property string qmltextEmptyMsg : qsTr(" Cannot be empty")
    readonly property string qmltextHistory :qsTr("HISTORY")
    property string recipeNameassign:qsTr("First")
    property int clickedRecipeIndex: 0
    property string recipeNumber: ""
    property int indexofLoader: 0
    readonly property string qmlTextEmpty: qsTr("")

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
    readonly property string qmltextAllWeld: qsTr("All Weld")
    readonly property string qmltextSuspect: qsTr("Suspect")
    readonly property string qmltextReject: qsTr("Reject")
    readonly property string qmltextWeldsWithAlarms: qsTr("Welds with Alarms")
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

    property  int validateVal : 1
    property  string fetchResult: "1"
    property  string fromDateBL: "0"
    property  string toDateBL: "0"
    property  string fromCycleBL: "0"
    property  string toCycleBL: "0"
    property bool  allWeldStatus : false
    property bool  suspectStatus : false
    property bool  rejectStatus : false
    property bool  weldsWithAlarmsStatus : false
    property bool  cycleModifiedStatus : false
    property bool closeFilterFlag:false
    property string pressedColor: qmlPrimaryColor
    property CustomNumPad cNumpad

    Component.onCompleted:
    {
//        if(indexofLoader == 1)
//        {
//            analyticsTabBar.currentIndex = 1
//            analyticTabBar.tabbtn1Text.color=pressedColor
//            analyticTabBar.tab1BkRect.color = pressedColor
//        }

        if(indexofLoader == 0)
            analyticsTabBar.currentIndex = 0

        loadNormalGraph()
        weldResultHistory.isHistoryPressed = true
        if(weldResultHistory.graphPageIndex == 0)
            loaderGraphElement.item.weldGraphLoadTimer.start()
        weldResultHistory.weldGraphPlotTimer.start()
    }
    TabBar
    {
        id: bar
        width: (18/100)*parent.width
        height: (40/item1.height)*parent.height
        anchors.top: parent.top
        anchors.topMargin: 0

        TabButton {
            height: parent.height
            contentItem: Text {
                text: qmltextHistory
                font.family: Style.semibold.name
                font.pixelSize: Style.style2
                opacity: enabled ? 1.0 : 1
                color:  bar.currentIndex===1 ? qmlPrimaryColor : "#757575"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height
                opacity: enabled ? 1 : 1
                color:"#f8f9fa"
                border.width: 0
                radius: 0
                Rectangle{
                    y:parent.height-(2/40)*parent.height
                    width:parent.width-(20/130)*parent.width
                    height:2
                    color:bar.currentIndex===1?qmlPrimaryColor:"#00ffffff"
                }
            }
        }
    }
    StackLayout {
        id: lab_stacklayout
        z:1
        width:(18/100)*parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: bar.bottom
        anchors.topMargin: 0
        currentIndex: bar.currentIndex

        RecipeResultHistory
        {
            id: weldResultHistory
            x: 0
            y: 2
            width:(260/1280)*item1.width
            height:item1.height
            selectedRecipeNumber: qmlTextEmpty
        }
    }

    Loader
    {
        id:loaderGraphElement
        x:bar.x+bar.width
        width: (82/100)*parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: rectangle4.bottom
    }
    function loadNormalGraph()
    {
        weldResultHistory.graphPageIndex = 0
        loaderGraphElement.setSource("GraphRightParameter.qml",{"width":parent.width,"height":parent.height})
    }

    function loadTrendsGraph()
    {
        weldResultHistory.graphPageIndex = 1
        loaderGraphElement.setSource("TrendsGraph.qml",{"width":parent.width,"height":parent.height})
    }

    function loadTableGraph()
    {
        weldResultHistory.graphPageIndex = 2
        loaderGraphElement.setSource("AnalyticsResultsTable.qml",
                                     {   "width":parent.width-20,
                                         "height":parent.height-20,
                                         "tableHeaderH": weldResultHistory.listRectHeight,
                                         "tableRowH": weldResultHistory.graphListHeight})
        //recipe.updateTableGraphModel()
    }
    /*
    StackLayout{
        x:bar.x+bar.width
        id: analyticsStackLayout
        width: (82/100)*parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: rectangle4.bottom

        currentIndex: weldResultHistory.graphlistIndex

        Item
        {
            GraphRightParameter
            {
                id: graphRightParameter
                width: parent.width
                height:parent.height
            }
        }
        Item
        {
			TrendsGraph
			{
                      id: trendsgraph
                      width: parent.width
                      height:parent.height
                  }
        }
        Item
        {
            AnalyticsResultsTable
            {
                id: art
                width: parent.width - 20
                height:parent.height - 20
                tableHeaderH: weldResultHistory.listRectHeight
                tableRowH: weldResultHistory.graphListHeight
            }
        }
    }
    */
    Rectangle {
        id: rectangle4
        width: (82/100)*parent.width
        height: (42/item1.height)*parent.height
        color: "#F8F9FA"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: (18/100)*parent.width
        anchors.left: parent.left
        //border.color: "#f8f9fa"

        DropShadow{
            source: rectangle5
            anchors.fill: rectangle5
            horizontalOffset: 1
            verticalOffset: 1
            color: "#000000"
            opacity: 0.2
            radius: 3
            visible: false
        }
        Rectangle {
            id: rectangle5
            width: (60/1020)*parent.width
            height: (35/42)*parent.height
            color: "#e9ecef"
            border.color: "#e9ecef"
            radius: 0
            anchors.bottomMargin: (4/42)*parent.height
            anchors.bottom: parent.bottom
            anchors.topMargin: (3/42)*parent.height
            anchors.top: parent.top
            anchors.rightMargin: (8/1020)*parent.width
            anchors.right: parent.right
            anchors.leftMargin: (976/1020)*parent.width
            anchors.left: parent.left
            visible: false
            layer.enabled: true
            layer.effect: DropShadow
            {
                //transparentBorder:true
                horizontalOffset:0.5
                verticalOffset:0.5
                opacity:0.2
                radius:3
                color:"#000000"
                samples:7
            }

            Image {
                id: filterimage
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                anchors.fill: parent
                source: "qrc:/images/image/filter.svg"
                visible: false
            }
            MouseArea {
                id: filtermouseArea
                anchors.fill: parent
                onClicked:
                {
//                    if(filterRectVisible == false)
//                    {
//                        filterBGRect.visible = true
//                        filteranim.start()
//                        filterRectVisible = true
//                    }
//                    else {
//                        filteranim.stop()
//                        filterBGRect.visible = false
//                        filterRectVisible = false
//                    }
                }
            }
        }
    }

    Rectangle {
        id: filterBGRect
        visible: false
        y:rectangle4.y-filterBGRect.height
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
            to:rectangle4.y+40
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
                                    fromDateId.text = calenderPopupForFilter1.calendar.userSelectedDate +" "+calenderPopupForFilter1.timePicker.timeLabel.text //calenderPopupForFilter.timePicker.listView.currentIndex+":"+calenderPopupForFilter.timePicker.minuteslistview.currentIndex+" "+calenderPopupForFilter.timePicker.meridianslistview.currentIndex//
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
                        anchors.fill:fromCycleImg
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
                CustomCheckBox {
                    id:weldsWithAlarmsCheckBox
                    checkBoxStatus: false
                    enabled: true
                    checkBoxName: qmltextWeldsWithAlarms
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

    function applyChanges()
    {
        var filterData=qmlTextEmpty
        if((fromDateId.text == qmlTextEmpty && toDateId.text == qmlTextEmpty) && (fromCycleId.text == qmlTextEmpty && toCycleId.text == qmlTextEmpty))
        {
            allWeldStatus = allWeldsCheckBox.checkBoxStatus
            suspectStatus = suspectCheckBox.checkBoxStatus
            rejectStatus = rejectCheckBox.checkBoxStatus
            cycleModifiedStatus = cycleModifiedCheckBox.checkBoxStatus
            weldsWithAlarmsStatus = weldsWithAlarmsCheckBox.checkBoxStatus
            closeFilterFlag = true
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
                weldsWithAlarmsStatus = weldsWithAlarmsCheckBox.checkBoxStatus
                closeFilterFlag = true
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
                weldsWithAlarmsStatus = weldsWithAlarmsCheckBox.checkBoxStatus
                closeFilterFlag = true
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
        }

        recipe.updateAnalyticsResultsFilterData(filterData)

    }

    function displayWarningPopup(msg){
        displayQuitPopup(messagePopup.warning_title,msg+qmltextEmptyMsg)
    }

    function resetAllFields()
    {
        toDateId.text = qmlTextEmpty
        fromDateId.text = qmlTextEmpty
        toCycleId.text = qmlTextEmpty
        fromCycleId.text = qmlTextEmpty
        fetchResult = "1"
        fromCycleBL="0"
        toCycleBL="0"
        fromDateBL="0"
        toDateBL="0"
        cycleModifiedCheckBox.checkBoxStatus = false
        allWeldsCheckBox.checkBoxStatus = false
        suspectCheckBox.checkBoxStatus = false
        rejectCheckBox.checkBoxStatus = false
        weldsWithAlarmsCheckBox.checkBoxStatus = false

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
    }

    function openNumPadPopUp(title,titleLocale,index,value)
    {
        var component = Qt.createComponent("CustomNumPad.qml")
        cNumpad = component.createObject(window)
        cNumpad.open(title,titleLocale, value, qmlTextEmpty)
        /* On clicking Done button in the numpad */
        cNumpad.onSave.connect(function okay()
        {
            validateVal = parseInt(cNumpad.keyInputText)
            if((validateVal > 0) && (validateVal < 10000))
            {
                if(index===1)
                {
                    fromCycleId.text = parseInt(cNumpad.keyInputText)
                }
                else if(index === 2)
                {
                    toCycleId.text = parseInt(cNumpad.keyInputText)
                }
                else{}
            }
            else
            {
                displayQuitPopup(messagePopup.warning_title,qmltextInvalidValueMsg)
            }
            cNumpad.close();
            cNumpad.save.disconnect(okay)
        })
        cNumpad.cancel.connect(function pressCancel()
        {
            cNumpad.close()
            cNumpad.cancel.disconnect(pressCancel)
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
