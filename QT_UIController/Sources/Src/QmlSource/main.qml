import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.0
import Style 1.0
import com.branson.menu 1.0
import com.branson.openedscreen 1.0
import com.branson.scstate 1.0

Window {
    id: window
    readonly property string qmltextExecutive : qsTr("Executive")
    readonly property string qmltextSupervisor : qsTr("Supervisor")
    readonly property string qmltextTechnician : qsTr("Technician")
    readonly property string qmltextOperator : qsTr("Operator")
    readonly property string qmltextDASHBOARD:qsTr("DASHBOARD")
    readonly property string qmltextContinue:qsTr("CONTINUE")
    readonly property string qmltextAbort:qsTr("ABORT")
    readonly property string qmltextReportGenerateSuccess:qsTr("Report is generated successfully")
    readonly property string qmltextReportNotGeneratedDueToInvalidSc:qsTr("Report is not generated due to SC is in invalid state")
    readonly property string qmltextReportNotGeneratedDueToUsbStorage:qsTr("Report is not generated due to USB storage is not present")
    readonly property string qmltextReportNotGeneratedDueToUsbFull:qsTr("Report is not generated due to USB storage full")
    readonly property string qmltextReportNotGeneratedDueToUsbWritingIncomplete:qsTr("Report is not generated due to USB reporting writing incomplete")
    readonly property string qmltextReportNotGenerated:qsTr("Report is not generated")
    readonly property string qmltextReportNotGeneratedDueToMountErr:qsTr("USB is not mounted")
    readonly property string qmltextReportNotGeneratedToUsbInsufficient:qsTr("Report is not generated due to USB insufficient memory")
    readonly property string qmltextDataBaseAlmostFull:qsTr("Data Base memory is almost full")
    readonly property string qmltextNoMemory:qsTr("No memory space in Data Base, Weld is blocked")
	readonly property string qmltextDBRecordNotFound:qsTr("DB Record is not found")
    readonly property string qmltextBackup:qsTr("BACKUP DB")
    readonly property string qmltextDBEmpty: qsTr("DB is empty")
    readonly property string qmltextDataNotComplete: qsTr("Data is not complete")
    readonly property string qmltextStackChangeConfirmation: qsTr("Stack change confirmation")
    readonly property string qmlTextNo: qsTr("NO")
    readonly property string qmlTextYes: qsTr("YES")
    readonly property string qmlTextSpace: " "
    readonly property string qmlTextEmpty: ""
    readonly property string qmltextNoServerConn:qsTr("Internal communication failure. Attempting to recover")
    readonly property string qmlTextRecipeNameInvalid: qsTr("Recipe name cannot be empty")
    property bool isLogout: false
    property string clickedTab: ""

    /*System Type argument and return type, qml macro definition*/
    property string qmlTextSystemType:qsTr("System Type")
    property real qmlSystemType: 0
    property real qmlSystemTypeGSX_E1: 0
    property real qmlSystemTypeASX: 1
    property bool isReportinProgress: false
    property bool g_reportgenerationinprogress: false
    property bool g_vacummgenerationinprogress: false
    property int numPadStatus: 0
    property var weldId: 0
    property alias window: window
    property var sprite:[]
    property var addrecipe:null
    property var titlbaronclicked: null
    property alias loaderelement: loaderelement
    property alias loadmain: loadmain
    property alias enableMainwindowOpacity: enableMainwindowOpacity
    property alias disableMainwindowOpacity: disableMainwindowOpacity
    property alias loaderelementwidth: loaderelement.width
    property alias loaderelementheight: loaderelement.height
    property alias mainWindow: mainwindow
    property bool alarmPopupOpenStatus: false
    property bool firmwareupgradeInitated: false
    property string firmwareupgradestatusmessage : qmlTextEmpty
    property bool horndownClamped: false
    property int labelSize: 65
    visible: true
    width: Screen.width-15
    height:Screen.height - 25
    property alias steppingNumPad: steppingNumPad
    property alias recipeInformationPopup: recipeInformationPopup
    property alias hornScanPopup: hornScanPopUpWindow
    property int userLevelId
    property bool authorityCheckFlag: false
    property string errorLabel: qsTr("Error")
    property string errorMessage: qsTr("Permission denied")
    property bool isAppLogIn: false
    property int mainStatusbarHeight: 25
    property int  g_SelectedRecipeNum :0
    visibility: Window.Minimized

    /* Variables to remember previous HornDown modes */
    property bool radioButtonFPC : false
    property bool radioButtonJOG : false
    property bool radioButtonHORNDOWN : false
    property bool advancedHornDownClicked: false
    property bool findpartcontactstatusActSetup: false
    property bool readypositionswitchActSetup:false
    property bool horndownclampswitchActSetup:false
    property bool horndownswitchpressedActSetup: false
    property bool jogswitchpressedActSetup: false
    property bool setflagUpActSetup: false
    property bool setflagDownActSetup: false
    property bool readyPosActSetup: false
    property bool hornClampActSetup: false
    property bool  fromAdvanced: false
    property alias mainStatusBarText : main_Statusbar_text.text
    property alias mainStatusBar : main_Statusbar_text
    property int g_CurrentWeldMode: 0
    property bool selectSingleMode: true
    property bool reqLoginExpriyPwd: false
    property bool firstTimeLogin: false
    property string userName: ""
 	property string oldForceValue: ""
    property bool isForceValueChanged: false
    property bool isFirstTime: true
    property string dbVersion: ""
    property string enteredRecipeName: ""
    property bool isDBDeleted: false
    property bool isActSetupActive: false
    /*Global color properties*/
    property color qmlPrimaryColor
    property color qmlSecondaryColor
    Component.onCompleted:
    {
        window.visibility = Window.FullScreen
        config.blockAltTab()
        /*Request configuration info from SC*/
        config.readSysConfigData()
        qmlSystemType = sysconfig.getCurrentNameOfComboBox(qmlTextSystemType)
        switch(qmlSystemType)
        {
        case qmlSystemTypeGSX_E1:

            qmlPrimaryColor = "#68A4EB"
            qmlSecondaryColor = "#5283BF"
            break

        case qmlSystemTypeASX:

            qmlPrimaryColor = "#00AA7E"
            qmlSecondaryColor = "#004B8D"
            break

        default:
            break
        }
    }

    Component.onDestruction:
    {
        config.allowAltTab()
    }

    function setFirstTimeFlag()
    {
        isFirstTime = true
    }

    /**
    *@breif: To display busy indicator
    **/
    function displayBusyIndicator(busyIndicatorRectId, busyMessage)
    {
        busyIndicatorRectId.visible = true
        busyIndicatorRectId.running = true
        busyIndicatorRectId.message =  busyMessage
        busyIndicatorRectId.busyIndicatorRectIdArea = true
    }

    /**
    *@breif: To quit busy indicator
    **/
    function quitBusyIndicator(busyIndicatorRectId)
    {
        busyIndicatorRectId.running = true.visible = false
        busyIndicatorRectId.running = false
        busyIndicatorRectId.busyIndicatorRectIdArea = false
    }

    /**
    *@breif: To display message popup
    **/
    function displayQuitPopup(labelName, errorMessage)
    {
        messagePopup.open(qsTr(labelName), qsTr(errorMessage), messagePopup.without_button)
        messagePopup.displayPopupClose = 0
    }

    /**
    *@breif: Checking the user level and enabling is permisssion enabled for the respective user level
    **/
    function isPermissionAllowed(permissionItem)
    {
        var permission = userPrivilageObj.isPermissionAllowed(window.userLevelId, permissionItem)
        //if (!permission)
        //    displayQuitPopup(errorLabel, errorMessage)

        return permission
    }

    /**
    *@breif: To get user level
    **/
    function getUserLevel()
    {
        var level
        level = window.userLevelId
        switch(level)
        {
        case 1: return "Executive"

        case 2: return "Supervisor"

        case 3: return "Technician"

        case 4: return "Operator"

        }
    }

    /**
    *@breif: To open full screen alarm popup
    **/
    function raiseAlarmPopup()
    {
        alarmPopup.alarmNumberInPopup = alarmDetails.getAlarmId()
        alarmPopup.alarmID = alarmDetails.getAlarmId() + " : " + alarmDetails.getErrorName()
        alarmPopup.alarmDate = alarmDetails.getAlarmDate()
        alarmPopup.alarmTime = alarmDetails.getAlarmTime()
        alarmPopup.resetStatus = alarmDetails.getReset()
        alarmPopup.errorDes = alarmDetails.getErrorDesc()
        alarmPopup.customResetButton.enabled = isPermissionAllowed(Features.RESET_ALARAMS) ? true : false
        alarmPopup.customResetButton.buttonColor = isPermissionAllowed(Features.RESET_ALARAMS) ? qmlPrimaryColor : "#757575"

        if((alarmPopup.alarmNumberInPopup == "71A") || (alarmPopup.alarmNumberInPopup == "71a"))
        {
            alarmPopup.toProductionSetupButton.visible = true
            alarmPopup.clearBacthCounterButton.visible = true
            alarmPopup.customNotifyButton.visible = true
            alarmPopup.customResetButton.visible = true
            alarmPopup.customResetButton2.visible = false
            alarmPopup.clearBacthCounterButton.enabled = isPermissionAllowed(Features.RESET_BATCH_COUNTER) ? true : false
            alarmPopup.clearBacthCounterButton.buttonColor = isPermissionAllowed(Features.RESET_BATCH_COUNTER) ? qmlPrimaryColor : "#757575"

        }
        else if(alarmPopup.alarmNumberInPopup === "423")
        {
            alarmPopup.customResetButton2.visible = true
            alarmPopup.customNotifyButton.visible = false
            alarmPopup.customResetButton.visible = true
            alarmPopup.customResetButton.text = "REMIND LATER"
            alarmPopup.toProductionSetupButton.visible = false
            alarmPopup.clearBacthCounterButton.visible = false
        }
        else if(alarmPopup.alarmNumberInPopup === "629")
        {
            alarmPopup.customResetButton2.visible = true
            alarmPopup.customResetButton.visible = false
            alarmPopup.customNotifyButton.visible = false
            alarmPopup.toProductionSetupButton.visible = false
            alarmPopup.clearBacthCounterButton.visible = false
        }
        else if(alarmPopup.alarmNumberInPopup === "631")
        {
            alarmPopup.customResetButton2.visible = false
            alarmPopup.customResetButton.visible = false
            alarmPopup.customNotifyButton.visible = false
            alarmPopup.toProductionSetupButton.visible = false
            alarmPopup.clearBacthCounterButton.visible = false
        }
        else if(alarmPopup.alarmNumberInPopup === "63e" || alarmPopup.alarmNumberInPopup === "63E")
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
        alarmNumber.text = alarmNotify.getNoOfAlarms()

    }

    /**
    *@breif: Getting count before login howmany alarms raised
    **/
    function alarmRaisedWithoutLogin()
    {
        alarmNumber.text = alarmNotify.getNoOfAlarms()
    }

    /**
    *@breif: Update the production results
    **/
    function updateProductionResults()
    {
        loaderelement.item.updateResults()
    }

    /**
    *@breif: Update the production power status
    **/
    function updateProductionPowerStatus()
    {
        loaderelement.item.updateProductionPowerResults()
    }

    function updateHornPositions()
    {
        loaderelement.item.updateHornPositions()
    }

    /**
    *@breif: To open emergency popup
    **/
    function emergencyStart()
    {
        emergencyPopup.open()
        enableMainwindowOpacity.start()
        emergencyPopup.resetbuttonvisibility = false
    }

    /**
    *@breif: To close emergency popup
    **/
    function emergencyStop()
    {        
        emergencyPopup.resetbuttonvisibility = true
    }

    /**
    *@breif: To close alarm notification popup
    **/
    function alarmNotificationPopup()
    {
        alarmNumber.text = alarmNotify.getNoOfAlarms()
        alarmNotificationPopup.close()
        alarmPopup.resetAllAlarmsFromSC()
    }

    function showServerFailPopup()
    {
        displayQuitPopup(messagePopup.error_title, qmltextNoServerConn)
        messagePopup.displayPopupClose = 1
    }
    /**
    *@breif: Verifing stack change 
    **/
    function stackChangeConfirmation()
    {
        messagePopup.openSplPurpose(" ",qmltextStackChangeConfirmation,messagePopup.ok_and_cancel_button,qmlTextYes,qmlTextNo)
        messagePopup.onOkay.connect(function onOkay()
        {
            obj_smartStack.copyNewStackToCurrentStack()
            messagePopup.onOkay.disconnect(onOkay)
        })
        messagePopup.onCancel.connect(function onCancel()
        {
            obj_smartStack.clearNewStackInfo()
            messagePopup.onCancel.disconnect(onCancel)
        })
    }

    property FirmWareEthernet fwPopup

    /**
    *@breif: To open software upgrade popup
    **/
    function upgradePopupOpen()
    {
        upgradeImage.visible = true
        if(window.isAppLogIn == false)
            return;
        var component = Qt.createComponent("FirmWareEthernet.qml")
        fwPopup = component.createObject(window)
        fwPopup.open()
        fwPopup.onUpgradeNow.connect(function upgradeNow()
        {
            fwPopup.close()
            enableMainwindowOpacity.start()
            id_fwupgrade_pop.open()
            id_fwupgrade_pop.upgradeOverEther()
            upgradeImage.visible = false
            fwPopup.onUpgradeNow.disconnect(upgradeNow)
            fwPopup.destroy()
        })
        fwPopup.onUpgradeLater.connect(function upgradeLater()
        {
            disableMainwindowOpacity.start()
            fwPopup.close()
            upgradeImage.visible = true
            fwPopup.onUpgradeLater.disconnect(upgradeLater)
            fwPopup.destroy()
        })
    }

	/**
    *@breif: Displaying report generation response popup
    **/
    function vacuumReportPopup()
    {
        titlebarmousearea.enabled = true
        var resData;
        g_vacummgenerationinprogress = false;
        resData = sysconfig.getVacuumRes();
        recipe.updateStatusText(resData);
    }

    /**
    *@breif: Displaying vacuum generation response popup
    **/
    function generateReportPopup()
    {
        titlebarmousearea.enabled = true
        var resData;
        isReportinProgress = false
        g_reportgenerationinprogress = false
        resData = sysconfig.getReportRes()
        if(resData === 0)
        {
            recipe.updateStatusText(qmltextReportNotGeneratedDueToInvalidSc)
        }
        else if(resData === 1)
        {
            recipe.updateStatusText(qmltextReportGenerateSuccess);
        }
        else if(resData === 16)
        {
            recipe.updateStatusText(qmltextReportNotGeneratedDueToUsbStorage)
        }
        else if(resData === 17)
        {
            recipe.updateStatusText(qmltextReportNotGeneratedDueToUsbFull)
        }
        else if(resData === 18)
        {
            var reportStatus;
            messagePopup.openSplPurpose(messagePopup.info_title,qmltextReportNotGeneratedToUsbInsufficient,messagePopup.ok_and_cancel_button,qmltextContinue,qmltextAbort)
            messagePopup.onOkay.connect(function onOkay()
            {
                reportStatus = 1
                sysconfig.sendReportAction(reportStatus)
                messagePopup.onOkay.disconnect(onOkay)
            })
            messagePopup.onCancel.connect(function onCancel()
            {
                reportStatus = 0
                sysconfig.sendReportAction(reportStatus)
                messagePopup.onCancel.disconnect(onCancel)
            })
        }
        else if(resData == 19)
        {
            recipe.updateStatusText(qmltextReportNotGeneratedDueToMountErr)
        }
        else if(resData === 32)
        {
            recipe.updateStatusText(qmltextDBRecordNotFound)
        }
        else if(resData == 33)
        {
            recipe.updateStatusText(qmltextDataNotComplete)
        }
        else if(resData == 34)
        {
            recipe.updateStatusText(qmltextDBEmpty)
        }
        else
            recipe.updateStatusText(qmltextReportNotGenerated)
    }

    function dbMemoryStatusPopUp()
    {
        var memoryStatus,per;
        memoryStatus = sysconfig.getDbmemoryStatus()
        if(memoryStatus !== 5)
        {
            if(memoryStatus === 1)
            {
                per = "80%";
            }
            else if(memoryStatus === 2)
            {
                per = "85%";
            }
            else if(memoryStatus === 3)
            {
                per = "90%";
            }else if(memoryStatus === 4)
            {
                per = "95%";
            }
            messagePopup.openSplPurpose(messagePopup.warning_title,qmltextDataBaseAlmostFull + per,messagePopup.primButtonBoth,"BACKUP DB","REMIND LATER");
            messagePopup.onOkay.connect(function onOkay()
            {
                loaderelement.setSource("Data.qml",{"width":(400/1920)*window.width,"height":window.height})
                titlebarchild.visible=true
                titlebarchildtext.text="DATA"
                titlbaronclicked="System.qml"
            })
        }
        else if(memoryStatus === 5)
        {
            messagePopup.openSplPurpose(messagePopup.warning_title,qmltextNoMemory,messagePopup.ok_button,qmltextBackup);
            messagePopup.onOkay.connect(function onOkay()
            {
                loaderelement.setSource("Data.qml",{"width":(400/1920)*window.width,"height":window.height})
                titlebarchild.visible=true
                titlebarchildtext.text="DATA"
                titlbaronclicked="System.qml"
            })
        }
        else
            return;
    }

    /**
    *@breif: To display barcode status
    **/
    function displaybarcodeStatus(statustext)
    {
        mainWindow.mainStatusBar.visible = true;
        mainWindow.mainStatusBarText = statustext
    }

    /**
    *@breif: Call logout function
    **/
    function callLogoutFunction()
    {
        closeAllPopups()
        alarmDetails.uicStatus(false);
        loadmain.setSource("Login.qml")

        mainwindow.visible=false
        loaderelement.setSource("System.qml")
        titlebar.text= qmltextDASHBOARD

        config.setActivePage(8)
        titlebarchild.visible=false
        disableMainwindowOpacity.start()
        //Eventlogger.eventSend(EventLogging.UI_EVENT_USER_LOGOUT)
        window.isAppLogIn = false
        login.logout()
    }

    function closeServerFailPopup()
    {
        messagePopup.close()
        messagePopup.displayPopupClose = 0
        dbVersion = ""
        if(!sysconfig.getAuthorityCheck())
        {
            horndownObj.startOrStopWeld(1)
            alarmDetails.uicStatus(true);
            alarmDetails.alarmPopupStatus(false)
            var scStatus = splash.getSCStatus();
            alarmNotify.clearAlarmNotificationList()
            if(scStatus == ScStateEnums.ALARM )
            {
                var noOfAlarms = alarmNotify.getAlarmDetails()
                alarmNumber.text = noOfAlarms
            }
        }
    }

    function closeAllPopups()
    {
        steppingNumPad.close()
        alarmNotificationPopup.close()
        recipeInformationPopup.close()
        suspectReject.close()
        setupNumpad.close()
        numpopup.close()
        alarmPopup.close()
        messagePopup.close()
        copyingDBPopUp.close()
    }

    function setUserIOToDefaultValues()
    {
        userIOPSDIObj.setDefaultValuesForDigitalInputParams();
        userIOPSAIObj.setDefaultValuesForAnalogInputParams();
        userIOPSDOObj.setDefaultValuesForDigitalOutputParams();
        userIOPSAOObj.setDefaultValuesForAnalogOutputParams();
        userIOACDIObj.setDefaultValuesForDigitalInputParams();
        userIOACDOObj.setDefaultValuesForDigitalOutputParams();

        userIOPSObj.setUIAfterSaving()
        userioObj.getPSDataFromUI()

        userIOACObj.setUIAfterSaving()
        userioObj.getACDataFromUI()
        userioObj.saveDataToDB()
    }

    function copyingDBPopUpRes()
    {
        copyingDBPopUp.close()
        var resData = sysconfig.getCopyDBRes()
        if(resData == 1)
        {
            recipe.updateStatusText(qsTr("DB file copied successfully to USB storage"))
            return
        }
        recipe.updateStatusText(qsTr("DB file copy to USB storage failed"))
    }

    Timer
    {
        id:id_main_statusdisplaytimer
        interval: config.getStatusDisplayTime()
        triggeredOnStart: false
        onTriggered:
        {
            mainStatusBarText = qmlTextEmpty
        }
    }

    Loader{
        id:loadmain
        x:0
        y:0
        width: parent.width
        height: parent.height
        source: Qt.resolvedUrl("SplashScreen.qml")
    }

    SteppigNumPad
    {
        id:steppingNumPad
        z:1
    }
    FirmwareUpgrade
    {
        id:id_fwupgrade_pop
        x: window.width / 2 - width / 2
        y : window.height/2 - height/2
    }
    AlarmPopup
    {
        id: alarmPopup
        z:3
    }
    AlarmNotificationPopup{
        id: alarmNotificationPopup
        z:1
        alarmlistView{
            id:alarmlistView
        }
    }

    //    Keyboardold
    //    {
    //        id: keyboard
    //        visible: false
    //        width: window.width
    //        height: window.height/2
    //        x:0
    //        y:window.height/2
    //    }

    RecipeInformation
    {
        id:recipeInformationPopup
        x:window.width*(15/100)
        y:window.height*(15/100)
    }
    HornScanPopup
    {
        id:hornScanPopUpWindow
        x: window.width / 2 - width / 2
        y : window.height/2 - height/2
    }

    SuspectRejectNumpad
    {
        id: suspectReject
        z:1
    }
    SetUpLimitNumpad{
        id: setupNumpad
        z:1
    }
    EmergencyStopPopup
    {
        id:emergencyPopup
        z:1
    }

    MainWindow
    {
        id:mainwindow
        property bool hornOpInProgress: historyDataObj.hornOpStatus
        x:0
        y:0
        height: parent.height - 35
        width: parent.width
        visible:false
        titlebarchild.visible: false
        titlebar.text: qmltextDASHBOARD
        titlebar{
            id: titlebar
            text:qmltextDASHBOARD
        }
        titlebarchild{
            id:titlebarchild
        }
        alarmNumber
        {
            id:alarmNumber
        }
        machineName
        {
            id:machineName
        }
        upgradeImage
        {
            id:upgradeImage
        }
        titlebarchildtext{
            id:titlebarchildtext

        }
        titlebarmousearea{
            id:titlebarmousearea
            onClicked: {

            }
        }
        texttimetitelbar
        {
            id:texttimetitelbar
        }

        Connections
        {
            target: recipe

            onUpdateStatusText:
            {
                if(config.getStatusDisplayTime() > 0)
                    id_main_statusdisplaytimer.restart()
                mainStatusBar.visible = true
                mainStatusBarText = strStatusText
            }
            onRefreshPage:
            {
                switch(dwIndex)
                {
                case OpenedScreenEnums.DASHBORAD:
                    dashboard.init();
                    loaderelement.setSource("Dashboard.qml")
                    break;
                case OpenedScreenEnums.PRODUTCION:
                    production.productionInit();
                    loaderelement.setSource("ProductionWindow.qml")
                    break;
                case OpenedScreenEnums.RECIPE:
                    recipe.loadRecipeCards()
                    loaderelement.setSource("RecipeListControl.qml")
                    break;
                }
            }
        }
        Connections
        {
            target: production

            onUpdatePartStatus:
            {
                mainStatusBar.visible = true
                mainStatusBarText = strPartStatusText
            }
        }
        Rectangle
        {
            id:main_Statusbar
            height: mainStatusbarHeight
            width: parent.width
            anchors.top: parent.bottom
            color:"#212529"
            Text
            {
                id:main_Statusbar_text
                width:parent.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                text:qmlTextEmpty
                font.pixelSize: Style.style1
                font.family: Style.regular.name
                color: "#ffffff"
            }
        }
        Rectangle
        {
            id:rectangle
            anchors.fill: parent
            color:qmlSecondaryColor
            z:-1
            OpacityAnimator on opacity
            {
                id:enableMainwindowOpacity
                from: 0;
                to: 0.5;
                duration: 250
                target:rectangle
                onRunningChanged: {
                    if(enableMainwindowOpacity.running)
                    {
                        rectangle.z=1
                    }
                }
            }
            OpacityAnimator on opacity{
                id:disableMainwindowOpacity
                from: 0.5;
                to: 0;
                duration: 250
                target: rectangle
                onRunningChanged: {
                    if(disableMainwindowOpacity.running)
                    {
                        rectangle.z=-1
                    }
                    else
                    {
                        rectangle.z=-1
                    }
                }
            }
            opacity:0
        }
        Loader{
            id:loaderelement
            x:0
            y:90
            width: parent.width
            height: parent.height-90
        }
    }

    /*CustomBusyIndicator{
        id:busyIndicatorRectId
    }*/

    CustomNumPad {
        id:numpopup
    }

    MessagePopup
    {
        id:messagePopup
        z:5
        displayPopupClose : 0
    }

    ProgressBarMessagePopUp
    {
        id:copyingDBPopUp
    }

    InputPanel
    {
        id: inputPanel
        visible: active
        z: 2
        y: active ? parent.height - inputPanel.height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        Component.onCompleted: {
            VirtualKeyboardSettings.styleName = "test_sas"
            VirtualKeyboardSettings.locale = sysconfig.getLanguageCode()
        }
    }

    SecondLevelAuthentication
    {
        id:secondLevelAuthentication
    }
}

