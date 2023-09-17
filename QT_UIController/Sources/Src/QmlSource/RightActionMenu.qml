import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import Style 1.0
import com.branson.menu 1.0
import com.branson.eventlogging 1.0

Item {
    readonly property string qmltextBranson :qsTr("Branson")
    readonly property string qmltextLogout : qsTr("Logout")
    readonly property string qmltextOverride : qsTr("Override")
    readonly property string qmltextHornScan : qsTr("HornScan")
    readonly property string qmltextSCANSEEKTEST : qsTr("SCAN/SEEK/TEST")
    readonly property string qmltextScanSeekTest : qsTr("Scan/Seek/Test")
    readonly property string qmltextACTUATORSETUP : qsTr("ACTUATOR SETUP")
    readonly property string qmltextActuatorSetup : qsTr("Actuator Setup")
    readonly property string qmltextACTIONCENTER : qsTr("ACTION CENTER")
    readonly property string qmltextCLEARMEMORY : qsTr("CLEAR MEMORY")
    readonly property string qmltextClearMemory : qsTr("Clear Memory")
    readonly property string qmltextDASHBOARD : qsTr("DASHBOARD")
    readonly property string qmltextNoActiveRecipeMsg : qsTr("Active recipe is not available")
    readonly property string qmltextRunningStatus : qsTr("Status: Running
Next Service: 5/16/17
Lifetime Cycles: 560078")

    id:rightActionCenterItemId
    width: 320
    height: 480
    z:1
    NumberAnimation on x{
        id:rightActionAnimationReturn
        from: rightActionCenterItemId.x;to: parent.width+1+rightmenuoutsideareaID.width
        duration: 250
    }
    property int gapbetweenelements: 55
    property int  gapfromrightside: 70
    property string userNameLevel: ""
    Component.onCompleted:
    {
        rightActionAnimationReturn.restart()
    }

    function init()
    {
        if (window.authorityCheckFlag)
            rectanglelogoutId.visible = false
    }

    function logout()
    {
        isLogout = true
        alarmDetails.uicStatus(false);
        rightActionAnimationReturn.restart()
        loadmain.setSource("Login.qml")

        mainwindow.visible=false
        loaderelement.setSource("System.qml")
        titlebar.text= qmltextDASHBOARD

        //If there is a WS  login, set the WS user as main user for weld cycles and alarms
        if(true == login.IsWebServicesLogged){
            PrimaryData.setUsername(login.WSLoggedUserName)
            recipe.setSelectedRecipeToActive(recipe.getStructureValueByName("recipenumber"))
        }
        else
            horndownObj.startOrStopWeld(0)

        config.setHMILogonStatus(false)
        closeAllPopups()
        titlebarchild.visible=false
        disableMainwindowOpacity.start()
        Eventlogger.eventSend(EventLogging.UI_EVENT_USER_LOGOUT)
        login.logout()
        window.isAppLogIn = false
        sysconfig.bransonKeyGlobalFlag = false
        sysconfig.bransonSecondLevelFlag = false
    }

    Rectangle
    {
        id: rootRectangleId
        anchors.fill: parent
        color: "transparent"
        Canvas{
            id: canvash
            width: parent.height
            height: parent.height
            rotation: -90
            z: 0
            onPaint:
            {
                var ctx = getContext("2d")
                ctx.fillStyle = "#212529"
                {
                    ctx.beginPath()
                    ctx.arc(canvash.width/2,canvash.height, canvash.width,0,Math.PI,1)//original
                    ctx.fill()

                }
            }
            MouseArea
            {
                anchors.fill: parent
            }
        }
        Rectangle {
            id: rectangleiteamId
            x: 80
            width: (parent.width*0.75)
            color: "#212529"
            Text {
                id: username
                width: parent.width/2
                height: 23
                color: "#ffffff"
                font.family: Style.regular.name
                text: userNameLevel
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.right: imageUsername.left
                anchors.rightMargin: 27
                anchors.top: rectangleactioncenterId.bottom
                anchors.topMargin: gapbetweenelements
                font.pixelSize: Style.style3
            }
            Button{
                id:rectangleHornDownId
                width: parent.width/2
                height: 35
                anchors.top: username.bottom
                anchors.topMargin: gapbetweenelements
                anchors.right: parent.right
                anchors.rightMargin: gapfromrightside
                enabled: isPermissionAllowed(Features.WELD_SETUP)  ? true : false
                contentItem: Text{
                    text: qmltextActuatorSetup
                    font.family: Style.regular.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: isPermissionAllowed(Features.WELD_SETUP) ? "#FFFFFF": "#BEC1C3"
                    font.pixelSize: Style.style3
                }
                background: Rectangle {
                    radius: 6
                    color: isPermissionAllowed(Features.WELD_SETUP) ? "#212529" : "#757575"
                    border.color: "#ffffff"
                    border.width: 2
                }
                onClicked:
                {
                    if(isPermissionAllowed(Features.WELD_SETUP)===true)
                    {
                        rightActionAnimationReturn.restart();
                        if((titlebar.text == qmltextACTUATORSETUP)&&(titlebarchild.visible == false))
                        {
                            disableMainwindowOpacity.start()
                            return
                        }

                        clickedTab = "Actuator Setup"
                        leftMenuData_obj.resetLeftMenu()
                        rightActionAnimationReturn.restart();
                        disableMainwindowOpacity.start()
                        horndownObj.fillActiveRecipeForActuator(1)
                        var status = horndownObj.getRecipeNumberForActuator()
                        if(status == "NA")
                            recipe.updateStatusText(qmltextNoActiveRecipeMsg)
                        else
                        {
                            loaderelement.setSource("HornDown.qml")
                            titlebarchild.visible = false;
                            titlebar.text = qmltextACTUATORSETUP;
                        }
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.ToolTip
                    onPressed:
                    {
                        mouse.accepted = false ;
                    }
                }
            }
            Button{
                id:rectangleHornscanId
                x: 110
                width: parent.width/2
                height: 35
                anchors.top: rectangleHornDownId.bottom
                anchors.topMargin: gapbetweenelements
                anchors.right: parent.right
                anchors.rightMargin: gapfromrightside
                enabled: isPermissionAllowed(Features.SCAN_SEEK_TEST) ? true : false
                contentItem: Text{
                    text: qmltextScanSeekTest
                    font.family: Style.regular.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: isPermissionAllowed(Features.SCAN_SEEK_TEST) ? "#FFFFFF": "#BEC1C3"
                    font.pixelSize:  Style.style3
                }
                background: Rectangle {
                    radius: 6
                    color: isPermissionAllowed(Features.SCAN_SEEK_TEST) ? "#212529" : "#757575"
                    border.color: "#ffffff"
                    border.width: 2
                }
                onClicked: {
                    if (isPermissionAllowed(qmltextHornScan))
                    {
                        rightActionAnimationReturn.restart();
                        if((titlebar.text == qmltextSCANSEEKTEST)&&(titlebarchild.visible == false))
                        {
                            disableMainwindowOpacity.start()
                            return
                        }

                        disableMainwindowOpacity.start()
                        leftMenuData_obj.resetLeftMenu()

                        historyDataObj.hornIndSelected = 0
                        hornRecipeObj.init()
                        historyDataObj.refreshHistory()
                        loaderelement.setSource("HornPage.qml");
                        titlebarchild.visible= false;
                        titlebar.text=qmltextSCANSEEKTEST;
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.ToolTip
                    onPressed:
                    {
                        mouse.accepted = false ;
                    }
                }
            }

            Button{
                id:rectanglelogoutId
                width: parent.width/2
                height: 35
                anchors.right: parent.right
                anchors.rightMargin: gapfromrightside
                anchors.top: rectangleClearMemId.bottom
                anchors.topMargin: gapbetweenelements
                contentItem: Text{
                    font.family: Style.regular.name
                    text: qmltextLogout
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#ffffff"
                    font.pixelSize: Style.style3
                }
                background: Rectangle {
                    radius: 6
                    color: "#212529"
                    border.color: "#ffffff"
                    border.width: 2
                }
                onClicked: {
                    logout()
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.ToolTip
                    onPressed:
                    {
                        mouse.accepted = false ;
                    }
                }
                Connections{
                    target: customFilter
                    onLogOut: {
                        if (window.isAppLogIn && (window.authorityCheckFlag == false) && (firmwareUpgradeObj.getUpgradeStatus()==false))
                        {
                            logout()
                        }
                    }
                }
            }
            Button{
                id:rectangleClearMemId
                x: 110
                width: parent.width/2
                height: 35
                anchors.top: rectangleHornscanId.bottom
                anchors.topMargin: gapbetweenelements
                anchors.right: parent.right
                anchors.rightMargin: gapfromrightside
                enabled: isPermissionAllowed(Features.CLEAR_MEMORY) ? true : false
                contentItem: Text{
                    text: qmltextClearMemory
                    font.family: Style.regular.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: isPermissionAllowed(Features.CLEAR_MEMORY) ? "#FFFFFF": "#BEC1C3"
                    font.pixelSize:  Style.style3
                }
                background: Rectangle {
                    radius: 6
                    color: isPermissionAllowed(Features.CLEAR_MEMORY) ? "#212529" : "#757575"
                    border.color: "#ffffff"
                    border.width: 2
                }
                onClicked:
                {
                    rightActionAnimationReturn.restart()
                    disableMainwindowOpacity.start()
                    obj_smartStack.sendClearMemoryReqToSC()
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.ToolTip
                    onPressed:
                    {
                        mouse.accepted = false ;
                    }
                }
            }
            Image {
                id: imageUsername
                width: 30
                height: 30
                anchors.right: parent.right
                anchors.rightMargin:gapfromrightside
                anchors.top: rectangleactioncenterId.bottom
                anchors.topMargin: gapbetweenelements - 5
                source: "qrc:/images/image/UserLogin_White.svg"
            }
            Rectangle
            {
                id: rectangleactioncenterId
                width: 300
                height: 35
                color: "#212529"
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 3
                MouseArea {
                    id: mouseAreaRightactionreturn
                    anchors.fill: parent
                    cursorShape: Qt.ToolTip
                    onClicked: {
                        rightActionAnimationReturn.restart()
                        disableMainwindowOpacity.start()
                    }
                }
                Image {
                    id: image
                    width: 21
                    height: 26
                    anchors.top: parent.top
                    anchors.topMargin: 12
                    anchors.right: parent.right
                    source: "qrc:/images/image/menu_close_arrow_rightMenu.png"
                }
                Text {
                    id: actioncenter
                    width: 94
                    height: 14
                    color: "#ffffff"
                    text: qmltextACTIONCENTER
                    font.family: Style.regular.name
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize:Style.style7
                }
            }
            Image
            {
                id: powerimage
                sourceSize.width: 80
                sourceSize.height: 80
                anchors.top: rectanglelogoutId.bottom
                anchors.topMargin:  gapbetweenelements + 100
                anchors.right: parent.right
                anchors.rightMargin: gapfromrightside
                source: "qrc:/images/image/power.svg"
                MouseArea{
                    anchors.fill: parent
                    onClicked:
                    {
                        //If there is a WS  login, set the WS user as main user for weld cycles and alarms
                        if(true == login.IsWebServicesLogged){
                            PrimaryData.setUsername(login.WSLoggedUserName)
                            recipe.setSelectedRecipeToActive(recipe.getStructureValueByName("recipenumber"))
                        }
                        else
                            horndownObj.startOrStopWeld(0)

                        config.setHMILogonStatus(false)
                        Qt.quit()
                    }
                }
            }

            Text {
                id: statusTextId
                y: 412
                width: 132
                height: 60
                color: "#ffffff"
                text:qmltextRunningStatus
                font.family: Style.regular.name
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 40
                font.pixelSize:Style.style1
                visible: false
            }
        }
    }
    Rectangle{
        id:rightmenuoutsideareaID
        anchors.right: rootRectangleId.left
        anchors.rightMargin: 0
        height: parent.parent.height
        width:rightActionCenterItemId.parent.width-rightActionCenterItemId.width
        color: "transparent"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                rightActionAnimationReturn.restart()
                disableMainwindowOpacity.start()
            }
        }
    }
}

