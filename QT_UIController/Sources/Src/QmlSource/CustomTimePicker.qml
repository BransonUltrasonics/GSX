import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import Style 1.0

Rectangle {
    id: hourPicker
    color: "#fff"
    clip: true
    width: parent.width/2
    implicitHeight: parent.height
    // anchors.left:parent.left
    // anchors.leftMargin: 500


    property int numberOfItems: 7
    property string fontFamily: Style.regular.name
    property var time: ({hour: 0})
    property  var hour : "12"
    property var minuteHand : "0"
    property var meridianTxt : "AM"
    property bool stopUpdate: false

    property int listviewLeftmargin : 100
    property int timeDisplayRectHeight : 50
    property int listviewsYvalue: -150
    property alias timeLabel: timeLabel
    property alias hourTopLine : hourTopLine
    property alias hourBottomLine : hourBottomLine
    property alias listView : listView
    property alias minuteslistview : minuteslistview
    property alias meridianslistview : meridianslistview

    /**
       *@breif: To set the time
       **/
    function setTime(hours, minutes, meridian)
    {
        listView.currentIndex = parseInt(hours)
        minuteslistview.currentIndex = parseInt(minutes)
        hour = hours
        minuteHand = minutes
        meridianTxt = meridian
    }

    Rectangle {  //Selecting Bars
        id:hourTopLine
        width: parent.width
        height: 1
        y:  280
        color: "#898D96"
    }
    Rectangle {
        id:hourBottomLine
        width: parent.width
        height: 1
        y:330
        color: "#898D96"
    }

    ListView {
        id: listView
        x: listviewLeftmargin
        y: listviewsYvalue
        width: parent.width/2
        height: parent.height *1.75
        property int delegateHeight: height / numberOfItems
        model: 96000
        spacing: 1
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: (height - delegateHeight) / 2
        preferredHighlightEnd: (height + delegateHeight) / 2
        currentIndex: sysconfig.TitleBarDate.getHours()>12?sysconfig.TitleBarDate.getHours()%12:sysconfig.TitleBarDate.getHours()
        delegate: Item {
            id: contentItem
            width: listView.width/2
            height: listView.delegateHeight
            Rectangle {
                anchors.centerIn: parent
                color: "white"
                visible: false
            }
            Text {
                id: innerText
                property int hour: Math.floor(index % 12)
                text: leadingHourZero(hour)
                anchors.centerIn: parent
                font.pixelSize: Style.style5
                font.family: fontFamily
                color: contentItem.ListView.isCurrentItem ? qmlPrimaryColor : "#999"

                transform: [
                    Rotation {
                        origin.x: innerText.width / 2
                        origin.y: innerText.height / 2
                        axis { x: 1; y: 0; z: 50 }
                        angle: {
                            var middle = contentItem.ListView.view.contentY - contentItem.y + contentItem.ListView.view.height/2
                            var calculated = (middle - contentItem.height  ) / contentItem.height
                            if (calculated < -90)
                                return -90
                            else if (calculated > 90)
                                return 90
                            else
                                return calculated
                        }
                    },
                    Scale { //Content Items X & Y Positions
                        origin.x: innerText.width / 2
                        origin.y: innerText.height /6
                        xScale: {
                            // scaled 1 in middle position -> 0 when reaching edges
                            var scaled = (contentItem.y - contentItem.ListView.view.contentY + contentItem.height * 0.5) / contentItem.ListView.view.height * 2
                            if (scaled > 1) scaled = 2 - scaled
                            return Math.max(0, scaled)
                        }
                        yScale: xScale
                    },
                    Translate {
                        y: {
                            var scaled = (contentItem.y - contentItem.ListView.view.contentY +  contentItem.height ) / contentItem.ListView.view.height * 2 /2
                            scaled = Math.max(0, scaled)
                            scaled = 1 - scaled
                            return scaled * scaled * scaled * contentItem.height * 3
                        }
                    }
                ]
            }
        }
        onCurrentIndexChanged: {
            if(stopUpdate==false){
                hour = sysconfig.TitleBarDate.getHours()>12?sysconfig.TitleBarDate.getHours()%12:sysconfig.TitleBarDate.getHours()
                if(isNaN(hour))
                    hour=12;
            }
        }
        onMovementStarted: {
            stopUpdate=true

        }
        Component.onCompleted: {
            // Scrolls to middle of list
            //positionViewAtIndex(model * 0.5 - (listView.numberOfItems > 2 ? 1 : 0), ListView.SnapPosition)
        }
        onMovementEnded: {
            stopUpdate=false
            var item = currentIndex % 12
            hour = Math.floor(item)
            if (hour === 0){
                hour = 12
            }
            console.debug("TIME IS:", hour)
           // hourPicker.time = {hour: hour}*/
            hourPicker.time = leadingHourZero(hour)
        }
    }

    /**
       *@breif: Hide all controls
       **/
    function leadingHourZero(number) {
        var finalString = ('00' + number).slice(-2)
        if (finalString === "00" )
        {
            finalString = "12"
        }
        return finalString
    }

    /**
       *@breif: Hide all controls
       **/
    function leadingMinuteZero(number)
    {
        var finalString = ('00' + number).slice(-2)
        return finalString
    }

    /*********************************MINUTE RECT*********************************/

    property var mintime: ({minute: 0})
    ListView {
        id: minuteslistview
        x: listView.x + listviewLeftmargin
        y: listviewsYvalue
        width: parent.width/2
        height: parent.height * 1.75
        property int delegateHeight: height / numberOfItems
        model: 96000
        spacing: 1
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: (height - delegateHeight) / 2
        preferredHighlightEnd: (height + delegateHeight) / 2
        currentIndex: sysconfig.TitleBarDate.getMinutes()
        onCurrentIndexChanged: {
            if(stopUpdate==false){
                minuteHand = sysconfig.TitleBarDate.getMinutes()

                if(isNaN(minuteHand))
                    minuteHand="0";
            }
        }
        delegate: Item {
            id: minutescontentItem
            width: minuteslistview.width/2
            height: minuteslistview.delegateHeight
            Rectangle {
                anchors.centerIn: parent
                color: "white"
                visible: false
            }
            Text {
                id: minutesinnerText
                property int minute: 1 * (index % 60)
                text:leadingMinuteZero(minute)
                anchors.centerIn: parent
                font.pixelSize: Style.style5
                font.family: fontFamily
                color: minutescontentItem.ListView.isCurrentItem ? qmlPrimaryColor : "#999"
                transform: [
                    Rotation {
                        origin.x: minutesinnerText.width / 2
                        origin.y: minutesinnerText.height / 2
                        axis { x: 1; y: 0; z: 0 }
                        angle: {
                            var middle = minutescontentItem.ListView.view.contentY - minutescontentItem.y + minutescontentItem.ListView.view.height
                            var calculated = (middle - minutescontentItem.height  ) / minutescontentItem.height
                            if (calculated < -90)
                                return -90
                            else if (calculated > 90)
                                return 90
                            else
                                return calculated
                        }
                    },
                    Scale { //Content Items X & Y Positions
                        origin.x: minutesinnerText.width / 2
                        origin.y: minutesinnerText.height /2
                        xScale: {
                            // scaled 1 in middle position -> 0 when reaching edges
                            var scaled = (minutescontentItem.y - minutescontentItem.ListView.view.contentY + minutescontentItem.height * 0.5) / minutescontentItem.ListView.view.height * 2
                            if (scaled > 1) scaled = 2 - scaled
                            return Math.max(0, scaled)
                        }
                        yScale: xScale
                    },
                    Translate {
                        y: {
                            var scaled = (minutescontentItem.y - minutescontentItem.ListView.view.contentY +  minutescontentItem.height ) / minutescontentItem.ListView.view.height * 2 /2
                            scaled = Math.max(0, scaled)
                            scaled = 1 - scaled
                            return scaled * scaled * scaled * minutescontentItem.height * 3
                        }
                    }
                ]
            }
        }

        Component.onCompleted: {
            // Scrolls to middle of list
            // positionViewAtIndex(model * 0.5 - (minuteslistView.numberOfItems > 2 ? 1 : 0), ListView.SnapPosition)
        }
        onMovementStarted: {
            stopUpdate=true

        }
        onMovementEnded: {
            stopUpdate=false
            var item = currentIndex % 12
            var hour = Math.floor(item)
            console.debug("hour IS:", hour)
            minuteHand = 1 * (currentIndex % 60)
            console.debug("min IS:", minuteHand)
        }
    }

    /*********************************MERIDIANS RECT*********************************/

    ListModel {  //Declaring List Model for displaying Meridians
        id: merlistmodel
        ListElement {
            name: qsTr("AM")
        }
        ListElement {
            name: qsTr("PM")
        }
    }
    ListView {
        id: meridianslistview
        x: listView.x + (2*listviewLeftmargin)
        y: listviewsYvalue
        width: parent.width/2
        height: parent.height * 1.75
        property int delegateHeight: height / numberOfItems
        model: merlistmodel
        spacing: 1
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: (height - delegateHeight) / 2
        preferredHighlightEnd: (height + delegateHeight) / 2
        currentIndex:sysconfig.TitleBarDate.getHours()>=12?1:0

        delegate: Item {
            id: meridiancontentItem
            width: meridianslistview.width/2
            height: meridianslistview.delegateHeight
            Rectangle {
                anchors.centerIn: parent
                color: "white"
                visible: false
            }
            Text {
                id: meridiansinnerText
                text: name
                anchors.centerIn: parent
                font.pixelSize: Style.style5
                font.family: fontFamily
                color: meridiancontentItem.ListView.isCurrentItem ? qmlPrimaryColor : "#999"
                transform: [
                    Rotation {
                        origin.x: meridiansinnerText.width / 2
                        origin.y: meridiansinnerText.height / 2
                        axis { x: 1; y: 0; z: 0 }
                        angle: {
                            var middle = meridiancontentItem.ListView.view.contentY - meridiancontentItem.y + meridiancontentItem.ListView.view.height
                            var calculated = (middle - meridiancontentItem.height / 2 ) / meridiancontentItem.height
                            if (calculated < -90)
                                return -90
                            else if (calculated > 90)
                                return 90
                            else
                                return calculated
                        }
                    },
                    Scale { //Content Items X & Y Positions
                        origin.x: meridiansinnerText.width / 2
                        origin.y: meridiansinnerText.height /2
                        xScale: {
                            // scaled 1 in middle position -> 0 when reaching edges
                            var scaled = (meridiancontentItem.y - meridiancontentItem.ListView.view.contentY + meridiancontentItem.height * 0.5) / meridiancontentItem.ListView.view.height * 2
                            if (scaled > 1) scaled = 2 - scaled
                            return Math.max(0, scaled)
                        }
                        yScale: xScale
                    },
                    Translate {
                        y: {
                            var scaled = (meridiancontentItem.y - meridiancontentItem.ListView.view.contentY +  meridiancontentItem.height ) / meridiancontentItem.ListView.view.height * 2 /2
                            scaled = Math.max(0, scaled)
                            scaled = 1 - scaled
                            return scaled * scaled * scaled * meridiancontentItem.height * 3
                        }
                    }
                ]
            }
        }
        onCurrentIndexChanged: {
            if(stopUpdate==false)
                meridianTxt = merdian(meridianslistview.currentIndex)
        }
        Component.onCompleted: {
            // Scrolls to middle of list
            // positionViewAtIndex(model * 0.5 - (meridianslistView.numberOfItems > 2 ? 1 : 0), ListView.SnapPosition)
            meridianTxt = merdian(meridianslistview.currentIndex)
        }
        onMovementStarted: {
            stopUpdate=true

        }
        onMovementEnded: {
            meridianTxt = merdian(meridianslistview.currentIndex)
            stopUpdate=false
        }
    }
    function merdian(name) {
        if(name === 0) {
            meridianTxt = "AM"
        }
        else {
            meridianTxt = "PM"
        }
        return meridianTxt
    }

    /******************************DATE DISPLAY & OK CANCEL ACTIONS*****************************/
    Rectangle{
        id: timedisplayrect
        y:parent.y
        width: parent.width
        height: timeDisplayRectHeight
        color: qmlPrimaryColor
        Label {
            id:timeLabel
            width: parent.width
            height: parent.height
            color: "white"
            font.pixelSize:  Style.style5
            text: leadingHourZero(hour) + ":" + leadingMinuteZero(minuteHand) + " "+ meridianTxt
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.top: parent.top
        }
    }
    /* Rectangle{
        id: okcancelrect
        width: parent.width
        height: 50
        y: parent.y +425
        color: "white"
        PrimaryButton{
            id : okbtn
            text: "OK"
            textColor: qmlPrimaryColor
            anchors.right: parent.right
            anchors.rightMargin : 10
            background: Rectangle {
                color: "#fff"
            }
            onClicked: {
                calenderPopup.close()
                disableMainwindowOpacity.start()
            }
        }
        PrimaryButton{
            id : cancelbtn
            text: "CANCEL"
            textColor: qmlPrimaryColor
            anchors.right: parent.right
            anchors.rightMargin : 120
            background: Rectangle {
                color: "#fff"
            }
            onClicked: {
                calenderPopup.close()
                disableMainwindowOpacity.start()
            }

        }
    }*/
}





