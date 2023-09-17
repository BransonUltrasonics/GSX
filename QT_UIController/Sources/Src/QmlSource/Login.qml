/*
 * Login.qml
 *
 *  Created on: Aug 31, 2017
 */

import QtQuick 2.0
import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.errorcodes 1.0
import com.branson.helperenums 1.0
import com.branson.openedscreen 1.0
import com.branson.scstate 1.0
import com.branson.eventlogging 1.0
import com.branson.firmwareupgradedefs 1.0

Item {
    readonly property string qmltextMachine:    qsTr("Machine: ")
    readonly property string qmltextDASHBOARD:  qsTr("DASHBOARD")
    readonly property string qmltextPRODUCTION: qsTr("PRODUCTION")
    readonly property string qmltextRECIPES:    qsTr("RECIPES")
    readonly property string qmltextANALYTICS:  qsTr("ANALYTICS")
    readonly property string qmltextWrongVersionOfDBRequiredVer:        qsTr("Database version mismatch found. Required version: ")
    readonly property string qmltextWrongVersionOfDBExixtingVer:        qsTr(", Existing version is: ")
    readonly property string qmltextDBVersion:          "8"
    readonly property string qmltextDefaultDBVersion:   "1.0.0"
    readonly property string qmltextNoServerConn:                       qsTr("Internal communication failure. Attempting to recover")
    readonly property string qmltextEnterCorrectUserNameandPassword:    qsTr("Please Enter correct Username and Password")
    readonly property string qmltextPasswordExpired:                    qsTr("Your password is expired, Please contact your Executive")
    readonly property string qmltextUserDiabled:                        qsTr("User disabled, Please contact your Executive")
    readonly property string qmltextInvalidUserOrPassword:              qsTr("Invalid UserName or Password")
    readonly property string qmltextUserLocked:                         qsTr("User locked, Please contact your Executive")
    readonly property string qmltextHELLO:      qsTr("HELLO")
    readonly property string qmltextEnterUserNameAndPassword:           qsTr("Enter username and password")
    readonly property string qmltextUserName:   qsTr("Username")
    readonly property string qmltextPassWord:   qsTr("Password")
    readonly property string qmltextLOGIN:      qsTr("LOGIN")
    readonly property string qmltextFirstLogin:                         qsTr("First time login. Please reset your User login password")
    readonly property string qmltextNoConnection:                       "No Connection"
    readonly property string qmlTextEmpty:      qsTr("")
    readonly property string qmlTextLoginSuccess:   "Success"
    readonly property string qmlTextLoginFailed:    "Failed"
    readonly property string qmlTextLoginLocked:    "Locked"
    readonly property string qmltextWebServicesLogged:qsTr("Another user is logged in using Webservices, Please logout in Webservices to Login here.")
    readonly property string qmltextCorruptionRecipeMessage: qsTr("Some data has been returned to default values.")
    id: loginId
    property alias loginmain: loginId
    property alias userNameTextInputText: userNameTextInput.text
    property alias passWordTextInputText: passWordTextInput.text
    property int checkfocus: 1
    property bool isLoginEventReq: true
    property int daysLeft: 0
    property int reqIdfromSc: 0
    property int scStatus: 0
    property int noOfAlarms: 0
    property string textcolor: "#E9ECEF"

    /**
    *@breif: To initialise dashboard
    **/
    function initDashboard(userLevel)
    {
        recipe.updateStatusText("")
        messagePopup.close()
        alarmDetails.uicStatus(true);
        alarmDetails.alarmPopupStatus(false)
        scStatus = splash.getSCStatus()
        alarmNotify.clearAlarmNotificationList()
        if(scStatus == ScStateEnums.ALARM)
        {
            noOfAlarms = alarmNotify.getAlarmDetails()
            alarmNumber.text = noOfAlarms.toString()
        }
        mainwindow.visible = true
        if(isFirstTime == true)
        {
            config.init()
            StackRecipeObj.init()
        }
        leftMenuData_obj.resetLeftMenu()
        if(recipe.readRecipeFromDB("0") != 0)
            recipe.updateStatusText(qmltextCorruptionRecipeMessage)
        machineName.text = qmltextMachine + sysconfig.getMachineName()

        userPrivilageObj.init(userLevel)
        switch(config.getScreenActiveValue())
        {
        case 0:
            leftMenuData_obj.resetParent(qmltextDASHBOARD)
            loadDashBoardScreen()
            break;
        case 1:
            leftMenuData_obj.resetParent(qmltextPRODUCTION)
            loadProductionScreen()
            break;
        case 2:
            leftMenuData_obj.resetParent(qmltextRECIPES)
            loadRecipeScreen()
            break;
        case 3:
            leftMenuData_obj.resetParent(qmltextANALYTICS)
            loadAnalyticsScreen()
            break;
        default:
            break;
        }
        if(isFirstTime == true)
        {
            isFirstTime = false
            userioObj.userIOInit()
            userioObj.sendIndicationToSCToReadDBData()
        }

        isLogout = false
        mainwindow.visible=1
        titlebarchild.visible= false
        if(isLoginEventReq == true)
        {
            login.updateUserLogin()
            Eventlogger.eventSend(EventLogging.UI_EVENT_USER_LOGIN, qmlTextLoginSuccess);
        }
        config.setHMILogonStatus(true)
    }

    /**
    *@breif: To load dashboard Screen
    **/
    function loadDashBoardScreen()
    {
        sendActiveRecipeToBL()
        loadmain.setSource(null)
        loaderelement.setSource("Dashboard.qml")
    }

    /**
    *@breif: To load production screen
    **/
    function loadProductionScreen()
    {
        sendActiveRecipeToBL()
        titlebar.text=qmltextPRODUCTION
        production.productionInit();
        loaderelement.setSource("ProductionWindow.qml")
    }


    /**
    *@breif: To load recipe screen
    **/
    function loadRecipeScreen()
    {
        sendActiveRecipeToBL()
        recipe.loadRecipeCards()
        loaderelement.setSource("RecipeListControl.qml")
        titlebar.text=qmltextRECIPES
    }

    /**
    *@breif: To load analytics screen
    **/
    function loadAnalyticsScreen()
    {
        sendActiveRecipeToBL()
        titlebar.text=qmltextANALYTICS
        loaderelement.setSource("Analytics.qml")
    }

    /**
    *@breif: To send active recipe to DB
    **/
    function sendActiveRecipeToBL()
    {
        dashboard.FillLastWeldResult()
        dashboard.init();
        var isActiveRecipe = dashboard.GetIsActiveValue();
        enteredRecipeName = "NA"
        if(true === isActiveRecipe)
        {
            var recipeNumber = dashboard.GetRecipeNumber();
            recipe.setSelectedRecipeToActive(recipeNumber)
            enteredRecipeName = dashboard.GetRecipeName()
        }
    }


    /**
    *@breif: Based on checkfocus filling Keyboard input to corresponding textfields
    **/
    function sendUsername( keyboardInput )
    {
        if(checkfocus == 1)
        {
            userNameTextInput.text = keyboardInput
        }
        else if( checkfocus == 2 )
        {
            passWordTextInput.text = keyboardInput
        }
    }

    /**
    *@breif: When login button is clicked
    **/
    function onClickLogin()
    {
        if(dbVersion == "")
            dbVersion = config.CheckDBVersion()
        if(dbVersion == qmltextNoConnection)
        {
            displayQuitPopup(messagePopup.error_title, qmltextNoServerConn)
            messagePopup.displayPopupClose = 1
            return
        }
        else if((dbVersion != qmltextDBVersion) && (dbVersion != qmltextDefaultDBVersion))
        {
            displayQuitPopup(messagePopup.error_title, qmltextWrongVersionOfDBRequiredVer+qmltextDBVersion+qmltextWrongVersionOfDBExixtingVer+dbVersion)
            return
        }
        sysconfig.getTimeAndDateFromSC()
        sysconfig.startTimerThread()
        mainwindow.hourTimer.start()
        userName = userNameTextInput.text
        var password = passWordTextInput.text

        //keyboard.close()
        disableMainwindowOpacity.start()
        PrimaryData.setUsername(userNameTextInput.text)

        if (userName === qmlTextEmpty || password === qmlTextEmpty)
        {
            displayQuitPopup(messagePopup.error_title, qmltextEnterCorrectUserNameandPassword)
        }
        else
        {
            var dwRet = login.loginValidate(userName, password)

            if(dwRet === ErrorCode.SUCESS)
            {
                var lastChangedPassword = login.lastChangedPassword

                daysLeft = login.validatePasswordExpiry()

                if (daysLeft >= qmlGlobalSettingsObject.passwordExpiryInDays)
                {
                    reqLoginExpriyPwd = true
                    loadmain.setSource("PasswordExpirePopUp.qml")
                }
                else if(isLoginEventReq == true && login.isFirstLogin())
                {
                    firstTimeLogin = true
                    loadmain.setSource("PasswordExpirePopUp.qml")
                }
                else
                {
                    loadinitialView()
                }
            }
            else if(dwRet === ErrorCode.ERROR_USER_LOCKED)
            {
                messagePopup.open(messagePopup.error_title, qmltextUserLocked, messagePopup.without_button)
                if(isLoginEventReq == true)
                {
                    Eventlogger.eventSend(EventLogging.UI_EVENT_USER_LOGIN, qmlTextLoginLocked);
                }
            }
            else{
                messagePopup.open(messagePopup.error_title, qmltextInvalidUserOrPassword, messagePopup.without_button)
                passWordTextInputText = qmlTextEmpty
                if(isLoginEventReq == true)
                {
                    Eventlogger.eventSend(EventLogging.UI_EVENT_USER_LOGIN, qmlTextLoginFailed);
                }
            }
        }
    }
    Keys.onEnterPressed:  {
        onClickLogin()
    }

    Keys.onReturnPressed: {
        onClickLogin()
    }

/**
  @brief loading initival view after success response
 **/
    function loadinitialView()
    {
        var userStatus = login.userStatus
        if (userStatus === "Enabled")
        {
            loadmain.setSource("NULL.qml")
            var userLevel = login.getUserLevel()
            if (userLevel == "Executive")
                window.userLevelId = HelperEnums.EXECUTIVE
            else if (userLevel == "Supervisor")
                window.userLevelId = HelperEnums.SUPERVISOR
            else if (userLevel == "Technician")
                window.userLevelId = HelperEnums.TECHNICIAN
            else if (userLevel == "Operator")
                window.userLevelId = HelperEnums.OPERATOR

            initDashboard(window.userLevelId)
            window.isAppLogIn = true
            mainwindow.usernameTextTitleBar =userName
            authorityOptionModel.authorityOptionModelInit()
            authorityOptionModel.updateList(window.userLevelId)
            recipe.updateRecipeMenuList(window.userLevelId)
            login.getWSUserAndLevelLogged()
            horndownObj.startOrStopWeld(1)
        }
        else
        {
            messagePopup.open(messagePopup.error_title, qmltextUserDiabled, messagePopup.without_button)
        }
    }
    /**
    *@breif:Set logout time
    **/
    function setLogoutTimer()
    {
        var timeOutSetInMin = parseInt(qmlGlobalSettingsObject.idealLogoffTime)
        var timeOutInMilliseconds = 60000 * timeOutSetInMin;
        customFilter.setTimeOut(timeOutInMilliseconds);
    }


    Component.onCompleted:
    {

        // send the current version to the supervisor
        machineDetails.getmachineDetails();
        var strCommentUIVersion = modelversionlist["1"]["ParameterContent"];
        Eventlogger.eventSend(EventLogging.UI_EVENT_UIC_SOFTWARE_VERSION,strCommentUIVersion);

        config.setActivePage(8)
        if(dbVersion == "")
            dbVersion = config.CheckDBVersion()
        if(dbVersion == qmltextNoConnection)
        {
            displayQuitPopup(messagePopup.error_title, qmltextNoServerConn)
            messagePopup.displayPopupClose = 1
            return
        }
        else if(dbVersion != qmltextDBVersion)
        {
            displayQuitPopup(messagePopup.error_title, qmltextWrongVersionOfDBRequiredVer+qmltextDBVersion+qmltextWrongVersionOfDBExixtingVer+dbVersion)
            return
        }
        qmlGlobalSettingsObject.handleGlobalSettings()
        setLogoutTimer()
        var status = login.getAuthorityCheckStatus()
        if(status == 1)
        {
            login.setAuthorityCheckStatus(0)
            var grant = sysconfig.globalCheckFlag ?false: sysconfig.getAuthorityCheck()
            if (grant)
            {
                window.authorityCheckFlag = false
            }
            else
            {
                sysconfig.getTimeAndDateFromSC()
                sysconfig.startTimerThread()
                mainwindow.hourTimer.start()
                isLoginEventReq=false
                userNameTextInputText = qmlTextEmpty
                login.setUserDetails("N/A")
                PrimaryData.setUsername("N/A")
                window.userLevelId = 1
                initDashboard(window.userLevelId)
                authorityOptionModel.authorityOptionModelInit()
                authorityOptionModel.updateList(window.userLevelId)
                recipe.updateRecipeMenuList(window.userLevelId)
                window.authorityCheckFlag = true
                login.getWSUserAndLevelLogged()
                horndownObj.startOrStopWeld(1)
            }
        }
    }

    Rectangle{
        id: loginRecOutside
        width: parent.width
        height: parent.height

        Flickable{
            id: loginFlick
            width: parent.width
            height: parent.height
            flickableDirection: Flickable.VerticalFlick
            anchors.fill: parent
            contentHeight: loginRec.height
            interactive: Qt.inputMethod.visible ? true : false
            bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
            clip: true
            Rectangle{
                id: loginRec
                width: parent.width
                height: parent.height
                color: "#E9ECEF"
                //border.color: "#757575"
                Image
                {
                    id: backgroundImage
                    anchors.fill: parent
                    source: "qrc:/images/image/loginbackground.png"
                }
                Text {
                    id: loginText1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: parent.height*(6/100)
                    color: textcolor
                    text: qmltextHELLO
                    font.family: Style.regular.name
                    font.pixelSize: Style.style8
                }
                Text {
                    id: loginText2
                    anchors.horizontalCenterOffset: 1
                    anchors.topMargin: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: loginText1.bottom
                    color: textcolor
                    text: qmltextEnterUserNameAndPassword
                    font.family: Style.regular.name
                    font.pixelSize: Style.style7
                }
                Image {
                    id: loginUserImg
                    sourceSize.width: parent.width*(5/100)
                    sourceSize.height: parent.height*(5/100)
                    anchors.leftMargin: parent.width*(25/100)
                    anchors.topMargin: parent.height*(8/100)
                    anchors.left: parent.left
                    anchors.top: loginText2.bottom
                    source: "qrc:/images/image/UserLogin_White.svg"
                }
                Text {
                    id: loginUserName
                    anchors.left: loginUserImg.right
                    anchors.top: loginText2.bottom
                    anchors.leftMargin: parent.width*(0.5/100)
                    anchors.topMargin: parent.height*(8/100)
                    color: textcolor
                    text: qmltextUserName
                    font.family: Style.regular.name
                    font.pixelSize: Style.style7
                }
                DropShadow {
                    anchors.fill: loginUserNameRect
                    source: loginUserNameRect
                    horizontalOffset:  3
                    verticalOffset: 3
                    color:   "#80000000"
                    opacity: 1
                    samples: 20
                }
                Rectangle {
                    id: loginUserNameRect
                    width: parent.width*(47/100)
                    height: parent.height*(8.3/100)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(25/100)
                    anchors.top: loginUserName.bottom
                    anchors.topMargin: parent.height*(0.5/100)
                    color: "transparent"
                    border.color: qmlPrimaryColor
                    border.width: 1
                    LineEdit {
                        id: userNameTextInput
                        width: parent.width - 3
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize:  Style.style5
                        font.family: Style.semibold.name
                        anchors.centerIn: parent
                        selectByMouse: true
                        property string placeholderText: qsTr("Enter UserName")
                        Text
                        {
                            text: userNameTextInput.placeholderText
                            color: textcolor
                            visible: !userNameTextInput.text
                            anchors.verticalCenter:  parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 15
                            font.pixelSize: Style.style3
                            font.family: Style.regular.name
                        }
                        style: TextFieldStyle
                        {
                            textColor: textcolor
                            background: Rectangle
                            {
                                color: "transparent"
                            }
                        }
                    }
                }
                Text {
                    id: userNameErrorText
                    anchors.leftMargin: parent.width*(25/100)
                    anchors.top: loginUserNameRect.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    font.family: Style.regular.name
                    font.pixelSize: Style.style5
                    color: "#f03e3e"
                }

                Image {
                    id: loginPwdImg
                    sourceSize.width: parent.width*(4.5/100)
                    sourceSize.height: parent.height*(4/100)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(25/100)
                    anchors.top: loginUserNameRect.bottom
                    anchors.topMargin: parent.height*(8/100)
                    source: "qrc:/images/image/lock_White.svg"
                }

                Text {
                    id: loginPwdTxt
                    anchors.top: loginUserNameRect.bottom
                    anchors.topMargin: parent.height*(7/100)
                    anchors.left: loginPwdImg.right
                    anchors.leftMargin: parent.width*(0.5/100)
                    color: textcolor
                    text:qmltextPassWord
                    font.family: Style.regular.name
                    font.pixelSize: Style.style7
                }
                DropShadow {
                    anchors.fill: loginPwdNameRect
                    source: loginPwdNameRect
                    horizontalOffset:  3
                    verticalOffset: 3
                    color:   "#80000000"
                    opacity: 1
                    samples: 20
                }
                Rectangle {
                    id: loginPwdNameRect
                    width: parent.width*(47/100)
                    height: parent.height*(8.3/100)
                    anchors.top: loginPwdTxt.bottom
                    anchors.topMargin: parent.height*(0.5/100)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(25/100)
                    color: "transparent"
                    border.color: qmlPrimaryColor
                    border.width: 1
                    LineEdit {
                        id: passWordTextInput
                        width: parent.width
                        height: parent.height
                        anchors.centerIn: parent
                        font.pixelSize: Style.style5
                        font.family: Style.semibold.name
                        echoMode: TextInput.Password
                        selectByMouse: true
                        property string placeholderText: qsTr("Enter Password")
                        Text {
                            text: passWordTextInput.placeholderText
                            color: textcolor
                            visible: !passWordTextInput.text
                            anchors.verticalCenter:  parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 15
                            font.pixelSize: Style.style3
                            font.family: Style.regular.name
                        }
                        style: TextFieldStyle
                        {
                            textColor: textcolor
                            background: Rectangle
                            {
                                color: "transparent"
                            }
                        }
                        onActiveFocusChanged:
                        {
                            if (passWordTextInput.focus)
                            {
                                loginFlick.contentY = 160
                            }
                        }
                    }
                }
                Button {
                    id: button
                    width: parent.width*(23/100)
                    height: parent.height*(10.3/100)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*(37/100)
                    anchors.top: loginPwdNameRect.bottom
                    anchors.topMargin: parent.height*(6/100)
                    font.bold: true
                    font.family: Style.regular.name
                    font.pixelSize: Style.style5
                    text: qmltextLOGIN
                    highlighted: true
                    background: Rectangle
                    {
                        color: "#10000000"
                        radius: 5
                        border.width: 1
                        border.color: "#10000000"
                        opacity: 0.9
                        layer.enabled: true
                        layer.effect: DropShadow
                        {
                            horizontalOffset: 3
                            verticalOffset: 3
                            color: "#50000000"
                            opacity: 0.5
                            samples: 10
                        }
                    }
                    onClicked: {
                        //loginFlick.contentY = 500
                        onClickLogin()
                    }
                }

                Text {
                    id: errortext
                    anchors.leftMargin: parent.width*(25/100)
                    anchors.top: button.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    font.family: Style.regular.name
                    font.pixelSize: Style.style5
                    color: "#f03e3e"
                }
            }
        }
    }
}
