import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import com.branson.menu 1.0
import QtQuick.Window 2.2
import com.branson.helperenums 1.0
import com.branson.WSrestrictionscreen 1.0

Item {
    readonly property string qmltextUserLevelExecutive : "Executive"
    readonly property string qmltextUserLevelSupervisor : "Supervisor"
    readonly property string qmltextPermissionDeniedMsg : qsTr("Permission denied")
    readonly property string qmltextNoLoginUser : "N/A"
    readonly property string qmltextWebServicesLoggedIn :qsTr("Please logout from the Web Services before accessing this screen")

    readonly property int indexOperatorAuthorityOptions : 0
    readonly property int indexGlobalUserSettings : 1

    implicitWidth: 400
    implicitHeight: 995
    property real gridHeight:1080
    property real parentHeight:1080
    property real rectHeight: 80
    property real rectWidth:400
    property real parentwidth:1920
    property real limitsmenupressureX: 0
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#ECECEC"
    property alias alarmConfigurationvisibility: alarmConfiguration.visible

    property int gridViewCurrentIndex: 0
    id:root

    Component.onDestruction: {
        if(false == isActSetupActive)
            login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
        login.UserOnModifyScreen = ""
    }

    /**
       *@breif: When the current index is changed
       **/
    function switchTitleBarText(cIndex )
    {
        var userLevel = login.getUserLevel();
        Qt.inputMethod.hide();
        var currentIndex = cIndex
        login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
        if(0== currentIndex)
        {
            systemConfigMenuPageID.text_line_edit_branson.text=""
            sysconfig.createSystemConfigGeneralParamList()
        }
        else if(1 === currentIndex)
        {
            if (isPermissionAllowed(Features.USER_LIST))
            {
                userMangementObj.handleUserList()
                usrMgrMain.userlisthighlightfirstrow()
                login.setWSRestrictedScreens(WSRestrictionScreen.USER_MANAGEMENT)
            }
            else
            {
                usrMgrMain.visible = false
            }
        }
        else if(2 == currentIndex)
        {
            userioObj.userIOInit()
        }
        else if(3 === currentIndex)
        {
            if (authorityModel)
            {
                authorityLevelManagement.handleAuthorityOption()
                qmlGlobalSettingsObject.handleGlobalSettings()
                if(HelperEnums.OPERATOR == login.WSUserLevelStatus)
                    authorityOPtion.barDefaultIndex = indexGlobalUserSettings
                else
                    authorityOPtion.barDefaultIndex = indexOperatorAuthorityOptions
            }
            login.setWSRestrictedScreens(WSRestrictionScreen.USER_OPERATOR_AUTHORITY)
        }
        else if(4 === currentIndex)
        {
            if (login.m_username == qmltextNoLoginUser)
            {
                displayQuitPopup(messagePopup.warning_title, qmltextPermissionDeniedMsg)
                changePasswordId.visible = false
            }
            else
            {
                changePasswordId.temp_UserID = login.m_username
                m_NewPassword.setUserID(changePasswordId.temp_UserID)
                m_NewPassword.getPasswordFromDB()
                login.setWSRestrictedScreens(WSRestrictionScreen.CHANGE_PASSWORD)
            }
        }
        else if(5 == currentIndex)
        {
            var userLevel = login.getUserLevel();
            if(userLevel === qmltextUserLevelExecutive || userLevel === qmltextUserLevelSupervisor)
            {
                alarmconfig.readAlarmconfigDetails()
            }
            else
            {
                displayQuitPopup(messagePopup.warning_title, qmltextPermissionDeniedMsg)
            }
        }
        else if(9 == currentIndex)
        {
            if(!sysconfig.bransonSecondLevelFlag)
            {
                disableMainwindowOpacity.start()
                keyVerifyPopup.passwordLineEditText = ""
                keyVerifyPopup.open()
                keyVerifyPopup.onOkay.connect(function onOkay()
                {
                    var resp;
                    resp=sysconfig.compareBransonSecondLevelKey(keyVerifyPopup.passwordLineEditText);
                    if(resp)
                    {
                        recipe.updateStatusText(qsTr("Second Level Key is matched, unlocking the hidden screens"))
                        sysconfig.bransonSecondLevelFlag = true
                        bransonDataObj.readRunTimeFeatureData()
                        sysconfig.createBransonSystemConfigList()
                    }
                    keyVerifyPopup.close()
                    keyVerifyPopup.onOkay.disconnect(onOkay)
                })
            }
        }
    }
    CustomListview
    {
        id: grid
        width: parent.width
        height: parent.height
        clip: true
        dynamicModel: systemConfigModel
        itemWidth:  grid.width/5
        itemHeight:  grid.height/10
        setCurrentIndex: authorityOptionModel.getDefaultIndex()
        onCustomCurrentIndexChanged:
        {
            gridViewCurrentIndex = cIndex
            switchTitleBarText(cIndex)
            sysconfig.bransonSecondLevelFlag = false
        }
    }
    Component.onCompleted:
    {
        sysconfig.bransonSecondLevelFlag = false
    }

    StackLayout
    {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: grid.customListviewCurrentIndex

        Item
        {
            id: tab0
            property real limitpressureX
            limitpressureX:(grid.width-(20/parentwidth)*grid.width)/4 +(20/1920)*window.width
            SystemConfigMenu
            {
                id: systemConfigMenuPageID
                x:(420/1920)*window.width
                y: grid.y
                width:window.width*(1500/1920)
                height:(995/1080)*window.height
            }
        }
        Item
        {
            id: tab1
            property real limitpressureX
            limitpressureX:(grid.width-(20/parentwidth)*grid.width)/4 +(20/1920)*window.width
            UserManagementMain
            {
                id: usrMgrMain
                x:(420/1920)*window.width
                y: grid.y
                width:window.width*(1450/1920)
                height:(995/1080)*window.height
            }
        }
        Item{
            id:userIO
            UserIOTabs
            {
                x:(400/1920)*window.width
                y: grid.y
                width:window.width*(1500/1920)
                height:(999/1080)*window.height

            }
        }
        Item
        {
            id: tab2
            property real limitpressureX
            limitpressureX:(grid.width-(20/parentwidth)*grid.width)/4 +(20/1920)*window.width

            UserAuthority
            {
                id: authorityOPtion
                x:(420/1920)*window.width
                y: grid.y
                width:window.width*(1450/1920)
                height:(995/1080)*window.height
                barDefaultIndex: 0
            }
        }

        Item
        {
            ChangePassword
            {
                id:changePasswordId
                x:(400/1920)*window.width
                y: grid.y
                width:window.width*(1520/1920)
                height:(995/1080)*window.height
                visible: true
            }
        }
        Item
        {
            AlarmConfiguration
            {
                id:alarmConfiguration
                x:(400/1920)*window.width
                y: grid.y
                width:window.width*(1520/1920)
                height:(995/1080)*window.height
            }
        }
        Item {}
        Item {}
        Item {}
        Item {
            Branson
            {
                x:(420/1920)*window.width
                y: grid.y
                width:window.width*(1500/1920)
                height:(995/1080)*window.height
                visible:sysconfig.bransonSecondLevelFlag
            }
        }
    }


    /**
       *@breif: To set the Textfield
       **/
    function setTextFieldValue( inputText )
    {
        changePasswordId.setTextFieldValue(inputText)
    }

    /**
       *@breif: To set usermanagement key value
       **/
    function setUserMngKeyValue( inputText )
    {
        usrMgrMain.setUserMngKeyValue(inputText)
    }
    BransonKeyVerifyPopup
    {
        id:keyVerifyPopup
    }
    ExecVerifyPopup
    {
        id:execkeyVerifyPopup
    }

}
