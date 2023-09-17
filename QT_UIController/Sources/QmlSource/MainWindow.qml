import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick 2.7
import Style 1.0
import com.branson.menu 1.0

Item {
    readonly property string qmltextMachine :qsTr("Machine: ")
    readonly property string qmltextNotifications :qsTr("NOTIFICATIONS")
    readonly property string qmltextWeldDataMsg :qsTr("Weld data extraction is in progress, Please wait until completion")
    readonly property string qmltextHornClampMsg :qsTr("The Actuator must be retracted before leaving the screen. Do you want to retract now?")

    id: item1
    clip: false
    property alias titlebar: titlebar
    property alias  titlebarchild: titlebarchild
    property alias titlebarmousearea: titlebarmousearea
    property alias texttimetitelbar: texttimetitelbar
    property string usernameTextTitleBar: ""
    property alias  titlebarchildtext: titlebarchildtext
    property alias leftaction: leftaction
    property alias alarmNumber: alarmNumber
    property alias machineName: machineText
    property alias hourTimer: hourTimer
    property alias upgradeImage: upgradeImage
    property string alarmId: ""
    Component.onCompleted:
    {
    }
    Component.onDestruction:
    {
        config.setHMILogonStatus(false)

        //If there is a WS  login, set the WS user as main user for weld cycles and alarms
        if(true == login.IsWebServicesLogged){
            PrimaryData.setUsername(login.WSLoggedUserName)
            recipe.setSelectedRecipeToActive(recipe.getStructureValueByName("recipenumber"))
        }
    }

    Rectangle {
        id: root
        color: "#ffffff"
        anchors.fill: parent
        clip: false

        Rectangle {
            id: header
            height: 25 //can not keep variable value
            color: "#212529"
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            Text {
                id: machineText
                color: "#ffffff"
                text: qmltextMachine + sysconfig.getMachineName()
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 16
                font.pixelSize: Style.style1
                font.family: Style.regular.name
            }
            Image {
                id: upgradeImage
                sourceSize.width: 30
                sourceSize.height: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: image2.left
                anchors.rightMargin:250
                source: "qrc:/images/image/Upgrade.svg"
                visible: false
                MouseArea{
                    id:upgradeImageMA
                    anchors.fill: parent
                    cursorShape:Qt.PointingHandCursor
                    onClicked:
                    {
                        upgradePopupOpen()
                    }
                }
            }

            Text {
                id: textUsernametitlebar
                x: 294
                color: "#ffffff"
                text: usernameTextTitleBar=="N/A"?"":usernameTextTitleBar
                anchors.verticalCenterOffset: 2
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Style.style1
                font.family: Style.regular.name
            }

            Image {
                id: image2
                sourceSize.width: 30
                sourceSize.height: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: textUsernametitlebar.left
                anchors.rightMargin: 6
                source: "qrc:/images/image/UserLogin_White.svg"
            }
            Rectangle{
                id: alarmNotificationRec
                width: 50
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: image2.right
                anchors.leftMargin: 270
                color: "#212529"
                border.color: "#212529"

                Text {
                    id: alarmNotificationText
                    text: qmltextNotifications
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    color: "#ffffff"
                    font.pixelSize: Style.style1
                    font.family: Style.regular.name
                }
                Image {
                    id: alarmNotificationImg
                    width: 30
                    height: 20
                    source: "qrc:/images/image/Ring_blue.png"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: alarmNotificationText.right
                    anchors.leftMargin: 5
                    Text {
                        id: alarmNumber
                        text:qsTr("0")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#ffffff"
                        font.pixelSize: Style.style1
                        font.family: Style.regular.name
                    }
                }

            }
            MouseArea {
                id: alarmNotificationMA
                width: 135
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: image2.right
                anchors.leftMargin: 270
                onClicked:
                {
                    alarmNotificationPopup.customResetButton.buttonColor = qmlPrimaryColor
                    alarmNotificationPopup.customResetButton.enabled = true

                    if(alarmNumber.text != "0")
                    {
                        if(alarmNumber.text == "1")
                        {
                            alarmNotificationPopup.parentNotifyRecHeight = 170
                        }
                        else if(alarmNumber.text == "2")
                        {
                            alarmNotificationPopup.parentNotifyRecHeight = 240
                        }
                        else if(alarmNumber.text == "3")
                        {
                            alarmNotificationPopup.parentNotifyRecHeight = 320
                        }
                        else
                        {
                            alarmNotificationPopup.parentNotifyRecHeight = 400
                        }
                        alarmNotificationPopup.open()
                        alarmId = alarmNotify.getAlarmIdAtIndex(0)
                        console.log("alarmId ",alarmId)
                        if(alarmId == "63e" || alarmId == "63E")
                        {
                            alarmNotificationPopup.customResetButton.buttonColor = "#757575"
                            alarmNotificationPopup.customResetButton.enabled = false
                        }
                    }
                }
            }

            Timer
            {
                id:hourTimer
                interval: 1000*3600     /*one hour*/
                repeat: true
                onTriggered:
                {
                    sysconfig.stopTimerThread()
                    sysconfig.getTimeAndDateFromSC()
                    //texttimetitelbar.text = Qt.formatTime(sysconfig.TitleBarDate,"hh:mm AP").toString()
                    sysconfig.startTimerThread()
                }
            }

            Text {
                id: texttimetitelbar
                //x: 571
                anchors.verticalCenter: parent.verticalCenter
                width: 53
                height: 14
                color: "#ffffff"
                text:Qt.formatTime(sysconfig.TitleBarDate,"hh:mm AP").toString()
                anchors.right: parent.right
                anchors.rightMargin: 35
                font.pixelSize: Style.style1
                font.family: Style.regular.name
                MouseArea
                {
                    id: timePopup
                    anchors.fill: parent
                    onClicked:
                    {
                        if(sysconfig.getAuthorityCheck() == true)
                        {
                            if(isPermissionAllowed(Features.SYSTEM_CLOCK) == false)
                            {
                                displayQuitPopup(errorLabel, errorMessage)
                                return
                            }
                        }
                        //recipe.updateWeldHistory()
                        //production.updateProductionScreenResults()
                        //loaderelement.item.updateResults()
                        //ProductionWindow.updateResults()
                        //alarmDetails.loadAlarmPopup("063e")
                        sysconfig.stopTimerThread()
                        hourTimer.stop()
                        enableMainwindowOpacity.start()
                        color : "transparent"
                        calenderPopup.calender.setCurrentDate()
                        calenderPopup.open()
                        calenderPopup.visible = true
                    }
                }
            }
        }
        CalendarPopUp
        {
            id:calenderPopup
        }
        DropShadow{
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.20
            source: title
            anchors.fill: title
        }

        Rectangle
        {
            id: title
            height: 60
            color: qmlPrimaryColor
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: header.bottom

            MouseArea
            {
                id: mouseArea
                //anchors.fill: parent
                anchors.left: title.left
                anchors.top: title.top
                anchors.bottom: title.bottom
                anchors.right: image.right
                hoverEnabled: true
                onReleased:
                {
					Qt.inputMethod.hide();
//                    checkChangeStackProgress()
                    if(isReportinProgress)
                    {
                        messagePopup.open(messagePopup.warning_title,qmltextWeldDataMsg,messagePopup.without_button)
                        return
                    }
                    if(firmwareupgradeInitated || isDBDeleted)
                    {
                        messagePopup.open(messagePopup.warning_title,firmwareupgradestatusmessage,messagePopup.without_button)
                        return
                    }
                    if(horndownClamped)
                    {
                        messagePopup.open(messagePopup.warning_title,qmltextHornClampMsg,messagePopup.ok_and_cancel_button)
                        messagePopup.onOkay.connect(function okay()
                        {
                            horndownClamped = false
                            horndownObj.retractRequestToBL()
                        })
                        messagePopup.onCancel.connect(function cancel()
                        {
                            horndownClamped = true
                        })
                        return
                    }
                    if(enteredRecipeName == "")
                    {
                        recipe.updateStatusText(qmlTextRecipeNameInvalid)
                        return
                    }

                    if(false == mainwindow.hornOpInProgress)
                    {
                        leftActionAnimation.restart()
                        enableMainwindowOpacity.start()
                    }
                    else
                    {
                        hornScanPopup.hornPopupOpen();
                    }
                }
                onEntered:
                {
                    cursorShape=Qt.PointingHandCursor
                }
            }

            Image
            {
                id: image
                width: 30
                anchors.leftMargin: 20
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/image/hamburger-menu.svg"
            }

            Image {
                id: image1
                x: 582
                sourceSize.width: 40
                anchors.rightMargin: 20
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/image/Action-center-new.svg"

                MouseArea {
                    id: mouseArea1
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape=Qt.PointingHandCursor
                    }
                    onReleased:
                    {
						Qt.inputMethod.hide();
//                        checkChangeStackProgress()
                        if(isReportinProgress)
                        {
                            messagePopup.open(messagePopup.warning_title,qmltextWeldDataMsg,messagePopup.without_button)
                            return
                        }
                        if(firmwareupgradeInitated || isDBDeleted)
                        {
                            messagePopup.open(messagePopup.warning_title,firmwareupgradestatusmessage,messagePopup.without_button)
                            return
                        }
                        if(horndownClamped)
                        {
                            messagePopup.open(messagePopup.warning_title,qmltextHornClampMsg,messagePopup.ok_and_cancel_button)
                            messagePopup.onOkay.connect(function okay()
                            {
                                horndownClamped = false
                                horndownObj.retractRequestToBL()
                            })
                            messagePopup.onCancel.connect(function cancel()
                            {
                                horndownClamped = true
                            })
                            return
                        }
                        if(enteredRecipeName == "")
                        {
                            recipe.updateStatusText(qmlTextRecipeNameInvalid)
                            return
                        }
                        if(false == mainwindow.hornOpInProgress)
                        {
                            rightaction.init()
                            rightaction.userNameLevel = usernameTextTitleBar == ""?"N/A":usernameTextTitleBar + " / " + getUserLevel()
                            rightActionAnimation.restart()
                            enableMainwindowOpacity.start()
                        }
                        else
                        {
                            hornScanPopup.hornPopupOpen();
                        }

                    }
                }
            }

            Rectangle{
                id:titlebarrectangle
                x: 65
                width: titlebar.text.length*14+14
                height: titlebarchild.height
                anchors.top: title.top
                anchors.topMargin: 0
                color: titlebarchild.visible ? qmlSecondaryColor:  qmlPrimaryColor
                MouseArea{
                    id:titlebarmousearea
                    anchors.fill:parent
                    enabled: titlebarchild.visible ? 1:0
                    cursorShape:titlebarchild.visible ? Qt.ToolTip:Qt.ArrowCursor//hoverEnabled: titlebarmousearea.enabled//titlebarchild.visible ? true:false


                    onClicked: {
                        Qt.inputMethod.hide();
                        if(isReportinProgress)
                        {
                            messagePopup.open(messagePopup.warning_title,qmltextWeldDataMsg,messagePopup.without_button)
                            return
                        }
                        if(firmwareupgradeInitated || isDBDeleted)
                        {
                            messagePopup.open(messagePopup.warning_title,firmwareupgradestatusmessage,messagePopup.without_button)
                            return
                        }
                        if(enteredRecipeName == "")
                        {
                            recipe.updateStatusText(qmlTextRecipeNameInvalid)
                            return
                        }
                        if(titlbaronclicked=="Dashboard.qml")
                        {
                            dashboard.init()
                            loaderelement.setSource("Dashboard.qml")
                        }
                        else if(titlbaronclicked=="RecipeListControl.qml")
                        {
                            recipe.loadRecipeCards()
                            loaderelement.setSource("RecipeListControl.qml")
                        }
                        else if(titlbaronclicked=="SeekScanFeature.qml")
                        {
                            seekRecipeObj.init()
                            seekhistoryDataObj.seekIndSelected = 0
                            seekhistoryDataObj.refreshSeekHistory()
                            loaderelement.setSource("SeekScanFeature.qml");
                        }
                        else if(titlbaronclicked=="HornScanFeature.qml")
                        {
                            historyDataObj.hornIndSelected = 0
                            hornRecipeObj.init()
                            historyDataObj.refreshHistory()
                            loaderelement.setSource("HornScanFeature.qml");
                        }
                        else if(titlbaronclicked=="Test.qml")
                        {
                            loaderelement.setSource("Test.qml");
                            ultrasonictest.setCheckBoxModelData()
                        }
                        else if(titlbaronclicked == "HornDown.qml")
                        {
                            fromAdvanced = true
                            loaderelement.setSource("HornDown.qml")
                        }
                        else
                        {
                            loaderelement.setSource(titlbaronclicked);
                        }
                        titlebarchild.visible=false
                    }
                }
            }
            Text {
                id: titlebar
                x: 79
                width: titlebar.text.length*14//titlebar.text.length*7 //43
                height: 14
                color: "#ffffff"
                text: qsTr("")
                font.pixelSize: Style.style5
                font.family: Style.semibold.name
                renderType: Text.NativeRendering
                wrapMode: Text.NoWrap
                //anchors.fill: parent
                anchors.top: title.top
                anchors.topMargin: 20

            }
            Rectangle
            {
                id:titlebarchild
                y: 0
                width: 150
                height: 60//43
                color: qmlPrimaryColor
                anchors.left: titlebar.right
                anchors.leftMargin: 0
                Canvas{
                    id:canvasline
                    width:10
                    height:parent.height
                    onPaint:
                    {
                        var Line = getContext("2d")
                        // setup the stroke
                        //Line.strokeStyle = "red"
                        Line.fillStyle = qmlSecondaryColor
                        // create a path
                        Line.beginPath()
                        Line.moveTo(0,0)
                        Line.lineTo(0,60)//40
                        Line.lineTo(10,30)//(10,20)
                        Line.lineTo(0,0)
                        // stroke path
                        //Line.stroke()
                        Line.fill()
                    }

                }
                Text{

                    id:titlebarchildtext

                    x: 30//40//46
                    y: 20
                    width: 79
                    height: 13
                    color: "#ffffff"
                    text: qsTr("")
                    font.pixelSize: Style.style5
                    font.family: Style.regular.name
                }
            }
        }

        Rectangle{
            id:parentviewwindow
            x: 0
            y: 90
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.top: title.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }
    }


    LeftActionMenu{
        id:leftaction
        width: parent.width/2
        height:parent.height
        y:0
        x:-(2*leftaction.width)
        z:2
        //x:-(*leftaction.width)-1000
        NumberAnimation on x{
            id:leftActionAnimation
            //running: mouseArea.pressed
            from:leftaction.x;to:0
            duration: 250

        }
    }

    RightActionMenu {
        id:rightaction
        width: parent.width/2
        height: parent.height
        x:parent.width+(rightaction.width)
        y:0
        z:2

        NumberAnimation on x{
            id:rightActionAnimation

            //running: mouseArea1.pressed
            from: rightaction.x ;to: rightaction.width
            duration: 250 //duration is in milisecond
        }
    }
}
