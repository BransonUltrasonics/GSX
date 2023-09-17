import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0
import QtQuick.Controls.Universal 2.0
import com.branson.eventlogging 1.0
import QtQuick.Controls 2.1

Item {
    id:item1

    property string paramText: ""
    property bool isToggleSwitch: false
    property bool isComboBox: false
    property bool isLineEdit: false
    property bool  checkStatus: false
    property bool isButton: false
    property string lineEditName : ""
    property bool isServiceNote: false
    property bool isText: false
    property bool isSSDSATA: false

    property alias toggleSwitch: toggleSwitch
    property alias text_line_edit: text_line_edit
    property alias serviceNoteTextArea: serviceNoteTextArea
    property alias combo_box: combo_box
    property alias toggleSwitchChecked: toggleSwitch.checked
    property alias param_text:param_text
    property alias textArea_serviceNote: textArea_serviceNote
    property alias delegateDropshadow: delegateDropshadow
    property alias toggleSwitchForSATA: toggleSwitchForSATA
    property alias textForSATA: textForSATA
    property alias paramButton: resetButton

    property int restCount: 0
    property bool response: false

    readonly property string qmltextFactoryResetMsg : "Factory Reset is completed"
    //readonly property string qmltextReset : qsTr("RESET")
    readonly property string qmltextToConfirmResetMsg: qsTr("If reset is done then entire data in the DB will be erased. Do you want to continue?")

    readonly property string qmltextResetDone: qsTr("Factory Reset is done. Restart the system")
    readonly property string qmltextResetNotSuccess: qsTr("Factory Reset is not successful.")
    readonly property string qmltextFactoryResetFail: qsTr("Factory Reset failed, The user name or password is incorrect")
    readonly property string qmltextDBDeleteSuccess: qsTr("DB file deleted successfully, Restart the system or upgrade the system")
    readonly property string qmltextIncorretLogin: qsTr("DB file deletion failed, The user name or password is incorrect")
    readonly property string qmltextDBDeleteFail: qsTr("DB file deletion failed")
    readonly property string qmltextCopyDBSuccess: qsTr("DB file copied successfully to USB storage")
    readonly property string qmltextCopyDBFail:qsTr("DB file copy to USB storage failed")

    readonly property string qmltextFactoryReset: qsTr("Factory Reset")
    readonly property string qmltextCopyDB: qsTr("Copy DB to USB")
    readonly property string qmltextDeleteDB: qsTr("Delete DB")
    readonly property string qmltextloginFail: qsTr("The user name or password is incorrect")
    readonly property string qmltextFactoryResetloginSuccess: qsTr("Factory Reset sucessfully, Please restart the system")
    readonly property string qmltextDeleteDatabaseSuccess: qsTr("Database deleted sucessfully, Please upgrade or restart the system")

    property DialogPopup dialogPopup

    /**
    *@breif: Clears all the tables in the DB and default values will be inserted into DB
    **/
    function restartSystem()
    {
        if(response == 1)
        {
            messagePopup.open(messagePopup.info_title ,qmltextResetDone, messagePopup.without_button)
            enableMainwindowOpacity.start()
            messagePopup.displayPopupClose = 1
        }
        else
        {
            recipe.updateStatusText(qmltextResetNotSuccess)
        }
    }

    function createDialogpopup(message)
    {
        var comment = qmlTextEmpty
        var component = Qt.createComponent("DialogPopup.qml")
        dialogPopup = component.createObject(mainwindow)
        dialogPopup.open(messagePopup.info_title, message)
        dialogPopup.onSave.connect(function onOkay()
        {
            comment = dialogPopup.quitMessage
            if (comment === qmlTextEmpty)
            {
                recipe.updateStatusText(qmltextCommtReqMsg)
            }
            else
            {
                Eventlogger.eventSend(EventLogging.UI_EVENT_SYSTEM_COMPONENT_FACTORY_RESET, comment);
                response = sysconfig.sendfactoryQuery()
                Eventlogger.eventSend(EventLogging.UI_EVENT_SYSTEM_COMPONENT_FACTORY_RESET, comment);
                userIOPSDIObj.setDefaultValuesForDigitalInputParams();
                userIOPSAIObj.setDefaultValuesForAnalogInputParams();
                userIOPSDOObj.setDefaultValuesForDigitalOutputParams();
                userIOPSAOObj.setDefaultValuesForAnalogOutputParams();
                restartTimer.start()
            }
            dialogPopup.close()
            dialogPopup.onSave.disconnect(onOkay)
            dialogPopup.destroy()
        })
        dialogPopup.onCancel.connect(function onCancel()
        {
            dialogPopup.close()
            dialogPopup.onCancel.disconnect(onCancel)
            dialogPopup.destroy()
        })
    }

    Timer
    {
        id: restartTimer
        interval: 500
        triggeredOnStart: false
        repeat: false
        onTriggered:
        {
           restartSystem()
        }
    }

    Timer
    {
        id: restartTimerForDialogPopup
        interval: 100
        triggeredOnStart: false
        repeat: false
        onTriggered:
        {
           createDialogpopup(qmltextFactoryResetMsg)
        }
    }

    function callFactoryReset()
    {
        messagePopup.open(messagePopup.info_title ,qmltextToConfirmResetMsg, messagePopup.ok_and_cancel_button)
        messagePopup.onOkay.connect(function onOkay()
        {
            if(recipe.isRuntimeFeatureEnabledForCFR())
            {
                restartTimerForDialogPopup.restart()
            }
            else
            {
                Eventlogger.eventSend(EventLogging.UI_EVENT_SYSTEM_COMPONENT_FACTORY_RESET);
                response = sysconfig.sendfactoryQuery()
                Eventlogger.eventSend(EventLogging.UI_EVENT_SYSTEM_COMPONENT_FACTORY_RESET);
                userIOPSDIObj.setDefaultValuesForDigitalInputParams();
                userIOPSAIObj.setDefaultValuesForAnalogInputParams();
                userIOPSDOObj.setDefaultValuesForDigitalOutputParams();
                userIOPSAOObj.setDefaultValuesForAnalogOutputParams();
                restartTimer.start()
            }
            messagePopup.close()
            messagePopup.onOkay.disconnect(onOkay)
        })
        messagePopup.onCancel.connect(function onCancel()
        {
            messagePopup.close()
            messagePopup.onCancel.disconnect(onCancel)
        })

        //restartSystem()
        userIOPSDIObj.setDefaultValuesForDigitalInputParams();
        userIOPSAIObj.setDefaultValuesForAnalogInputParams();
        userIOPSDOObj.setDefaultValuesForDigitalOutputParams();
        userIOPSAOObj.setDefaultValuesForAnalogOutputParams();
    }

    Timer
    {
        id: restartTimerFormessagePopup
        interval: 100
        triggeredOnStart: false
        repeat: false
        onTriggered:
        {
            messagePopup.open(messagePopup.info_title ,qmltextDBDeleteSuccess, messagePopup.without_button)
            enableMainwindowOpacity.start()
            messagePopup.displayPopupClose = 1
        }
    }

    /**
       *@breif: On click verify the executive log in,  if the log in sucessful, then do the factory reset
       **/
    function callExecutiveFactoryReset()
    {
        execkeyVerifyPopup.passwordLineEditText = ""
        execkeyVerifyPopup.useridLineEditText = ""
        execkeyVerifyPopup.open()
        execkeyVerifyPopup.onOkay.connect(function onOkay()
        {
            var password=execkeyVerifyPopup.passwordLineEditText;
            var userName=execkeyVerifyPopup.useridLineEditText;

            if (password === qmlTextEmpty)
            {
                 recipe.updateStatusText(qmltextloginFail)
                 factoryresetMessagebox();
            }
            else
            {
               var dwRet = login.loginValidate(userName, password);

                if(dwRet==0)
               {
                 if (login.getUserLevel()=="Executive")
                 {
                     callFactoryReset()
                     recipe.updateStatusText(qmltextFactoryResetloginSuccess)
                 }
                 else
                 {
                     recipe.updateStatusText(qmltextloginFail)
                     factoryresetMessagebox();
                 }

               }
               else
               {
                  recipe.updateStatusText(qmltextloginFail)
                  factoryresetMessagebox();
               }
            }
            execkeyVerifyPopup.close()
            execkeyVerifyPopup.onOkay.disconnect(onOkay)
        })

        execkeyVerifyPopup.onCancel.connect(function onCancel()
        {
            text_line_edit_branson.text = qmlTextEmpty
            execkeyVerifyPopup.onCancel.disconnect(onCancel)
        })

    }

    /**
       *@breif: On click verify the exerctive login, if the login is correct, then delete the DB
       **/
    function callExecutiveDeleteDB()
    {
        execkeyVerifyPopup.passwordLineEditText = ""
        execkeyVerifyPopup.useridLineEditText = ""
        execkeyVerifyPopup.open()
        execkeyVerifyPopup.onOkay.connect(function onOkay()
        {
            var password=execkeyVerifyPopup.passwordLineEditText;
            var userName=execkeyVerifyPopup.useridLineEditText;
            var loginType="E";


            if (password === qmlTextEmpty)
            {
                 recipe.updateStatusText(qmltextloginFail)
                 deleteDBMessagebox();
            }
            else
            {
               var dwRet = login.loginValidate(userName, password);
               if (dwRet==0)
               {
                  if (login.getUserLevel()=="Executive")
                  {
                  deleteDB();
                  recipe.updateStatusText(qmltextDeleteDatabaseSuccess)
                  }
                  else
                  {
                    recipe.updateStatusText(qmltextloginFail)
                    deleteDBMessagebox();
                  }

               }
               else
               {
                  recipe.updateStatusText(qmltextloginFail)
                  deleteDBMessagebox();
               }
            }
            execkeyVerifyPopup.close()
            execkeyVerifyPopup.onOkay.disconnect(onOkay)
        })

        execkeyVerifyPopup.onCancel.connect(function onCancel()
        {
            text_line_edit_branson.text = qmlTextEmpty
            execkeyVerifyPopup.onCancel.disconnect(onCancel)
        })

    }

    function deleteDB()
    {
        var flag = sysconfig.deleteDB()
        if(flag)
        {
            isDBDeleted = true
            messagePopup.openSplPurpose(messagePopup.info_title,qmltextDBDeleteSuccess,messagePopup.ok_and_cancel_button,"Restart","Upgrade")
            messagePopup.onOkay.connect(function onOkay()
            {
                restartTimerFormessagePopup.restart()
                messagePopup.onOkay.disconnect(onOkay)
            })
            messagePopup.onCancel.connect(function onCancel()
            {
                firmwareupgradestatusmessage = "Please Upgrade or Restart the system"
                service.loadSystemInfo()
                messagePopup.onOkay.disconnect(onOkay)
            })

        }
        else
            recipe.updateStatusText(qmltextDBDeleteFail)
    }

    function copyDBToUSB()
    {
        sysconfig.copyDB()
        copyingDBPopUp.open()
    }

    /**
       *@breif: pop up for incorrect login for delete DB
       **/
    function deleteDBMessagebox()
    {
        messagePopup.openSplPurpose(messagePopup.info_title,qmltextIncorretLogin,messagePopup.ok_button,"OK")
        messagePopup.onOkay.connect(function onOkay()
        {
            messagePopup.onOkay.disconnect(onOkay)
        })
    }


    /**
       *@breif: pop up for incorrect login for factory reset
       **/
    function factoryresetMessagebox()
    {
        messagePopup.openSplPurpose(messagePopup.info_title,qmltextFactoryResetFail,messagePopup.ok_button,"OK")
        messagePopup.onOkay.connect(function onOkay()
        {
            messagePopup.onOkay.disconnect(onOkay)
        })
    }

    DropShadow{
        id:delegateDropshadow
        source: rect1
        anchors.fill: rect1
        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.5
        radius: 3
    }
    Rectangle{
        id:rect1
        y:10
        width: parent.width
        height: parent.height
        color: "#ECECEC"
        radius: 3

        SystemConfigMenuItem {
            id: systemConfigMenuItem1
            width: parent.width
            height: parent.height

            DropShadow{
                anchors.fill: combo_box
                horizontalOffset: 3
                verticalOffset: 3
                color: "#000000"
                opacity: 0.2
                source: combo_box
            }

            Text{
                id:param_text
                text:paramText
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize:  Style.style3
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top:parent.top
                anchors.topMargin: 5
            }
            ToogleBar {
                id: toggleSwitchForSATA
                anchors.right: textForSATA.left
                anchors.rightMargin:   20
                anchors.topMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                visible: isSSDSATA
            }
            Text {
                id: textForSATA
                width: 35
                height: 40
                text:"S/N"
                visible: false
                anchors.right: text_line_edit.left
                anchors.rightMargin :10
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize:  Style.style2
                anchors.top:parent.top
                anchors.topMargin: 5
            }

            CustomCombobox {
                id: combo_box
                anchors.right: parent.right
                anchors.rightMargin:10
                anchors.verticalCenter: parent.verticalCenter
                width: 220
                height: 40
                visible: isComboBox
                dropDownImg.visible: isComboBox
                dropDownText.visible: isComboBox
            }
            Text {
                id: textEntry
                width: 220
                height: 40
                text:sysconfig.emmcUsage
                visible: isText
                anchors.right: parent.right
                anchors.rightMargin:50
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize:  Style.style3
                anchors.top:parent.top
                anchors.topMargin: 5
            }
            PrimaryButton
            {
                id: resetButton
                anchors.right: parent.right
                width: 220
                height: 40
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                visible: isButton
                onClicked:
                {
                    if(paramText == qmltextFactoryReset)
                    {
                        callExecutiveFactoryReset();
                    }
                    else if(paramText == qmltextDeleteDB)
                    {
                        callExecutiveDeleteDB();
                    }
                    else if(paramText == qmltextCopyDB)
                    {
                        copyDBToUSB()
                    }
                }
            }
            ToogleBar {
                id: toggleSwitch
                anchors.right: parent.right
                anchors.rightMargin: 50
                anchors.topMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                checked: checkStatus
                visible: isToggleSwitch
            }

            LineEdit
            {
                id: text_line_edit
                anchors.right: parent.right
                width: 220
                height: 40
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                visible: isLineEdit
            }
            Rectangle
            {
                id:serviceNoteTextArea
                width: 220
                height: 40
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                border.color: qmlPrimaryColor
                border.width: 1
                visible: isServiceNote
                Flickable {
                    id: flickable2
                    anchors.fill: parent
                    TextArea.flickable:TextArea {
                        id: textArea_serviceNote
                        focus: false
                        color:qmlPrimaryColor
                        wrapMode: Text.WordWrap
                        font.family: Style.regular.name
                        font.pixelSize: Style.style3
                        background:Rectangle {
                            anchors.fill: parent
                            border.color:qmlPrimaryColor
                        }
                    }

                    ScrollBar.vertical: ScrollBar { }
                }
            }
        }
    }
}
