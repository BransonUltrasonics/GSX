import QtQuick 2.6
import Qt.labs.calendar 1.0
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import Style 1.0
import com.branson.firmwareupgradedefs 1.0

Popup{

    readonly property string qmltextFIRMWARE_UPGRADE_USB_TEXT : qsTr("READ USB")
    readonly property string qmltextFIRMWARE_UPGRADE_AC_TEXT : qsTr("Actuator Controller")
    readonly property string qmltextFIRMWARE_UPGRADE_PC_TEXT : qsTr("Power Controller")
    readonly property string qmltextFIRMWARE_UPGRADE_SC_TEXT : qsTr("Supervisory Controller")
    readonly property string qmltextFIRMWARE_UPGRADE_UPGRADE_TEXT : qsTr("UPGRADE")
    readonly property string qmltextFIRMWARE_UPGRADE_TITLE : qsTr("SOFTWARE UPGRADE")
    readonly property string qmltextFIRMWARE_UPDATE_READING_TEXT: qsTr("Requesting File list")
    readonly property string qmltextFIRMWARE_UPDATE_READUSB_TEXT0: qsTr("Request timed out")
    readonly property string qmltextFIRMWARE_UPDATE_READUSB_TEXT1: qsTr("Plugin the USB device and retry")
    readonly property string qmltextFIRMWARE_UPDATE_READUSB_TEXT2: qsTr("No upgrade files(.bin/check file naming convention) found")
    readonly property string qmltextFIRMWARE_UPDATE_READUSB_TEXT3: qsTr("Populated the upgrade file list. Click on drop down for options")

    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_TEXT1: qsTr("None to upgrade")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_TEXT2: qsTr("Blank is not supported")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_TEXT3: qsTr("Upgrade in progress")

    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_TEXT4: qsTr("Upgrade failure, Please reboot the system")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_TEXT5: qsTr("Upgrade is successful, Please reboot the system")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_TEXT6: qsTr("Upgrade failure, One of the selected file size is zero bytes")

    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_Progress1:qsTr("Upgrade request sent")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_Progress2:qsTr("File transfer in progress")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_Progress3:qsTr("Erasing the Flash")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_Progress4:qsTr("Flash writing in progress")
    readonly property string qmltextFIRMWARE_UPDATE_UPGRADE_Progress5:qsTr("Flash writing Completed")
    readonly property string qmlTextUpgradeNow: qsTr("UPGRADE NOW")
    readonly property string qmlTextUpgradeLater: qsTr("UPGRADE LATER")
    property string  strModuleText: qsTr("")
    property int checkUsbOrEther: 0
    property color disabledcolor : "gray"
    property color enabledcolor  : qmlPrimaryColor
    property bool  bUpgrade: false

    /**
    *@breif: To Set StatusBar Text
    **/
    function setStatusbarText(strStatusText)
    {
        id_firmwareupgrade_statustext.text = strStatusText;
    }

    /**
    *@breif: Get ProgressBar Text
    **/
    function getProgressText(currentprogress)
    {
        switch(currentprogress)
        {
        case 1:
            return qmltextFIRMWARE_UPDATE_UPGRADE_Progress1+".."
            break;
        case 2:
            return qmltextFIRMWARE_UPDATE_UPGRADE_Progress2+".."
            break;
        case 3:
            return qmltextFIRMWARE_UPDATE_UPGRADE_Progress3+".."
            break;
        case 4:
            return qmltextFIRMWARE_UPDATE_UPGRADE_Progress4+".."
            break;
        case 5:
            return qmltextFIRMWARE_UPDATE_UPGRADE_Progress5
            break;
        }
    }

    /**
    *@breif: Upgrade over Ethernet
    **/
    function upgradeOverEther()
    {
        id_firmwareupgrade_usbrow_readusb.buttonColor = disabledcolor
        id_firmwareupgrade_usbrow_readusb.enabled = false
        id_firmwareupgrade_saverow.visible = false
        buttonsRowForOverEther.visible = true

        id_firmwareupgrade_ac_filelist.enabled = false
        id_firmwareupgrade_pc_filelist.enabled = false
        id_firmwareupgrade_sc_filelist.enabled = false
        checkUsbOrEther = 1
    }

    /**
    *@breif: Upgrade over USB
    **/
    function upgradeOverUsb()
    {
        id_firmwareupgrade_usbrow_readusb.buttonColor = enabledcolor
        id_firmwareupgrade_usbrow_readusb.enabled = true
        id_firmwareupgrade_saverow.visible = true
        buttonsRowForOverEther.visible = false

        id_firmwareupgrade_ac_filelist.enabled = true
        id_firmwareupgrade_pc_filelist.enabled = true
        id_firmwareupgrade_sc_filelist.enabled = true
        checkUsbOrEther = 0
    }

    id:id_firmwareupgrade_main
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    width :(( window.width * 65 ) /100)
    height: ((window.height * 55 )/100)
    x: 50
    y:50
    padding:0

    background: Rectangle {
        color: "#ECECEC"
    }

    Rectangle{
        id:id_firmwareupgrade_title //Popup title bar
        width: parent.width
        anchors.left: parent.Left
        anchors.top: parent.Top
        height: 40
        color: qmlPrimaryColor
        Text {
            id: id_title
            text: qmltextFIRMWARE_UPGRADE_TITLE
            font.family: Style.regular.name
            font.pixelSize:  Style.style3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color:"#FFFFFF"
            font.bold: true
        }
        Image {
            id: closeImage1
            sourceSize.width: parent.width*(10/100)
            sourceSize.height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 5
            /*Image icon update based on System Type*/
            source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/close_ASX.svg" :
                                                                   "qrc:/images/image/close.svg"

        }
        MouseArea{
            id: closeImagefw
            width: parent.width*(10/100)
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            x: closeImage1.x
            y: closeImage1.y
            onClicked:
            {
                if(bUpgrade == false)
                {
                    id_firmwareupgrade_main.close()
                    disableMainwindowOpacity.start()
                }
                else
                {
                    //id_firmwareupgrade_statustext.text= setStatusbarText(qsTr("Can't close. Upgrade in progress"))
                }
            }
        }

    }
    Rectangle {
        id:id_firmwareupgrade_content
        width:parent.width
        height: parent.height - id_firmwareupgrade_title.height
        anchors.top: id_firmwareupgrade_title.bottom
        color : "#ECECEC"
        Rectangle {
            id: id_firmwareupgrade_content_main_content
            width:parent.width
            height: parent.height
            RowLayout {
                id: id_firmwareupgrade_usbrow
                width: 100
                height: 100
                anchors.right: parent.right
                anchors.rightMargin: 50
                anchors.top: parent.top
                anchors.topMargin: 10
                layoutDirection: Qt.RightToLeft

                PrimaryButton {
                    id: id_firmwareupgrade_usbrow_readusb
                    text: qmltextFIRMWARE_UPGRADE_USB_TEXT
                    onClicked:
                    {
                        // filling the comobox after reading the USB
                        setStatusbarText(qmltextFIRMWARE_UPDATE_READING_TEXT+"..")
                        id_firmwareupgrade_saverow_save.enabled = true
                        var readusbresponse = firmwareUpgradeObject.readUSB()


                        ///AC File List
                        acfilelistmodel.clear();
                        var acfilelist = firmwareUpgradeObject.getfileslist(Firmwareupgradetypes.ACTUATOR_CONTROLLER)
                        for(var acindex=0;acindex<acfilelist.length;acindex++)
                        {
                            acfilelistmodel.append({"text":acfilelist[acindex]})
                        }

                        if(id_firmwareupgrade_ac_filelist.count >0)
                            id_firmwareupgrade_ac_filelist.currentIndex=0;

                        //PC File list
                        pcfilelistmodel.clear();
                        var pcfilelist = firmwareUpgradeObject.getfileslist(Firmwareupgradetypes.POWER_CONTROLLER)
                        for(var pcindex=0;pcindex<pcfilelist.length;pcindex++)
                        {
                            pcfilelistmodel.append({"text":pcfilelist[pcindex]})
                        }

                        if(id_firmwareupgrade_pc_filelist.count >0)
                            id_firmwareupgrade_pc_filelist.currentIndex=0;


                        ///SC File List
                        scfilelistmodel.clear();
                        var scfilelist = firmwareUpgradeObject.getfileslist(Firmwareupgradetypes.SUPERVISORY_CONTROLLER)
                        for(var scindex=0;scindex<scfilelist.length;scindex++)
                        {
                            scfilelistmodel.append({"text":scfilelist[scindex]})
                        }

                        if(id_firmwareupgrade_sc_filelist.count >0)
                            id_firmwareupgrade_sc_filelist.currentIndex=0;

                        switch(readusbresponse)
                        {
                            /* Request Time out */
                        case 0:
                            setStatusbarText( qmltextFIRMWARE_UPDATE_READUSB_TEXT0)
                            //disabledcolor
                            id_firmwareupgrade_saverow_save.buttonColor = disabledcolor
                            id_firmwareupgrade_saverow_save.enabled = false
                            break;
                            /* usb not plugged in*/
                        case 1:
                            setStatusbarText( qmltextFIRMWARE_UPDATE_READUSB_TEXT1)
                            //disabledcolor
                            id_firmwareupgrade_saverow_save.buttonColor = disabledcolor
                            id_firmwareupgrade_saverow_save.enabled = false
                            break;
                            /* No files found in USB */
                        case 2:
                            setStatusbarText( qmltextFIRMWARE_UPDATE_READUSB_TEXT2)
                            //disabledcolor
                            id_firmwareupgrade_saverow_save.buttonColor = disabledcolor
                            id_firmwareupgrade_saverow_save.enabled = false
                            break;
                            /* successful files found */
                        default:
                            setStatusbarText(qmltextFIRMWARE_UPDATE_READUSB_TEXT3)
                            //disabledcolor
                            id_firmwareupgrade_saverow_save.buttonColor = enabledcolor
                            id_firmwareupgrade_saverow_save.enabled = true
                            break;

                        }
                    }
                }
            }

            GridLayout {
                id: id_firmwareupgrade_grid
                //x: 15
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: id_firmwareupgrade_usbrow.bottom
                anchors.right: parent.right
                anchors.rightMargin: 10
                rows: 3
                columns: 3
                columnSpacing: 40
                rowSpacing: 15
                Text {
                    id: id_firmwareupgrade_ac_text
                    text: qmltextFIRMWARE_UPGRADE_AC_TEXT
                    horizontalAlignment: Text.AlignHCenter
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                    color: "#868e96"
                }
                CustomCombobox {
                    id: id_firmwareupgrade_ac_filelist
                    comboboxpopupheight:120
                    model : ListModel {
                        id:acfilelistmodel
                    }
                    Component.onCompleted: {
                        if(id_firmwareupgrade_ac_filelist.count >0)
                            id_firmwareupgrade_ac_filelist.currentIndex=0;
                    }
                }
                ProgressBar {
                    id: id_firmwareupgrade_ac_progressbar
                    maximumValue:5
                }

                Text {
                    id: id_firmwareupgrade_pc_text
                    text: qmltextFIRMWARE_UPGRADE_PC_TEXT
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                    color: "#868e96"
                }

                CustomCombobox {
                    id: id_firmwareupgrade_pc_filelist
                    comboboxpopupheight:120
                    model : ListModel {
                        id:pcfilelistmodel
                    }
                    Component.onCompleted:
                    {
                        if(id_firmwareupgrade_pc_filelist.count >0)
                            id_firmwareupgrade_pc_filelist.currentIndex=0;
                    }
                }

                ProgressBar {
                    id: id_firmwareupgrade_pc_progressbar
                    maximumValue:5
                }

                Text {
                    id: id_firmwareupgrade_sc_text
                    text: qmltextFIRMWARE_UPGRADE_SC_TEXT
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                    color: "#868e96"
                }
                CustomCombobox {
                    id: id_firmwareupgrade_sc_filelist
                    comboboxpopupheight:120
                    model : ListModel {
                        id:scfilelistmodel
                    }
                    Component.onCompleted: {
                        if(id_firmwareupgrade_sc_filelist.count >0)
                            id_firmwareupgrade_sc_filelist.currentIndex=0;
                    }

                }
                ProgressBar {
                    id: id_firmwareupgrade_sc_progressbar
                    maximumValue:5
                }
            }
            RowLayout
            {
                id:buttonsRowForOverEther
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                anchors.right: parent.right
                anchors.rightMargin: 50
                spacing: 30
                visible: false
                PrimaryButton {
                    id:popupPrimButtonId
                    text: qmlTextUpgradeNow
                    visible: true
                    onClicked: {
                        if((id_firmwareupgrade_ac_filelist.currentText == "None") &&
                                (id_firmwareupgrade_pc_filelist.currentText == "None") &&
                                (id_firmwareupgrade_sc_filelist.currentText == "None"))
                        {
                            setStatusbarText(qmltextFIRMWARE_UPDATE_UPGRADE_TEXT1)
                        }
                        else if ((id_firmwareupgrade_ac_filelist.count <=0) ||
                                 (id_firmwareupgrade_pc_filelist.count <=0) ||
                                 (id_firmwareupgrade_sc_filelist.count<=0))
                        {
                            setStatusbarText(qmltextFIRMWARE_UPDATE_UPGRADE_TEXT2)
                        }
                        else
                        {
                            setStatusbarText( qmltextFIRMWARE_UPDATE_UPGRADE_TEXT3+"...")
                            bUpgrade = true
                            firmwareupgradeInitated = true
                            firmwareUpgradeObject.upgradeFirmwares(id_firmwareupgrade_sc_filelist.currentIndex,id_firmwareupgrade_ac_filelist.currentIndex,id_firmwareupgrade_pc_filelist.currentIndex,checkUsbOrEther);
                            popupPrimButtonId.buttonColor = disabledcolor
                            popupPrimButtonId.enabled = false
                            popupPrimButton2Id.buttonColor = disabledcolor
                            popupPrimButton2Id.enabled = false

                        }
                    }
                }
                PrimaryButton {
                    id:popupPrimButton2Id
                    text: qmlTextUpgradeLater
                    visible: true
                    onClicked: {
                        upgradeImage.visible = true
                        disableMainwindowOpacity.start()
                        id_firmwareupgrade_main.close()
                    }
                }
            }
            RowLayout {
                id: id_firmwareupgrade_saverow
                anchors.right: parent.right
                anchors.rightMargin: 50
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                layoutDirection: Qt.RightToLeft
                spacing: 30
                PrimaryButton {
                    id: id_firmwareupgrade_saverow_save
                    text: qmltextFIRMWARE_UPGRADE_UPGRADE_TEXT
                    onClicked: {

                        if(true == login.IsWebServicesLogged)
                        {
                            messagePopup.open(messagePopup.warning_title,qmltextWebServicesLoggedIn,messagePopup.without_button)
                            id_firmwareupgrade_main.close()
                            disableMainwindowOpacity.start()
                        }
                        else
                        {
                            if((id_firmwareupgrade_ac_filelist.currentText == "None") &&
                                    (id_firmwareupgrade_pc_filelist.currentText == "None") &&
                                    (id_firmwareupgrade_sc_filelist.currentText == "None"))
                            {
                                setStatusbarText(qmltextFIRMWARE_UPDATE_UPGRADE_TEXT1)
                                //disabledcolor
                                id_firmwareupgrade_saverow_save.buttonColor = disabledcolor
                                id_firmwareupgrade_saverow_save.enabled = false
                            }
                            else if ((id_firmwareupgrade_ac_filelist.count <=0) ||
                                     (id_firmwareupgrade_pc_filelist.count <=0) ||
                                     (id_firmwareupgrade_sc_filelist.count<=0))
                            {
                                setStatusbarText(qmltextFIRMWARE_UPDATE_UPGRADE_TEXT2)
                                //disabledcolor
                                id_firmwareupgrade_saverow_save.buttonColor = disabledcolor
                                id_firmwareupgrade_saverow_save.enabled = false
                            }
                            else
                            {
                                setStatusbarText( qmltextFIRMWARE_UPDATE_UPGRADE_TEXT3+"...")
                                bUpgrade = true
                                firmwareupgradeInitated = true
                                var firmwarereturnsstr = firmwareUpgradeObject.upgradeFirmwares(id_firmwareupgrade_sc_filelist.currentIndex,id_firmwareupgrade_ac_filelist.currentIndex,id_firmwareupgrade_pc_filelist.currentIndex,checkUsbOrEther);

                                //disabledcolor
                                id_firmwareupgrade_saverow_save.buttonColor = disabledcolor
                                id_firmwareupgrade_usbrow_readusb.buttonColor = disabledcolor
                                id_firmwareupgrade_saverow_save.enabled = false
                                id_firmwareupgrade_usbrow_readusb.enabled = false
                                //disable combobox as well
                                id_firmwareupgrade_ac_filelist.enabled = false
                                id_firmwareupgrade_pc_filelist.enabled = false
                                id_firmwareupgrade_sc_filelist.enabled = false
                            }
                        }
                    } //end of clicked
                }

                Component.onCompleted: {
                    id_firmwareupgrade_saverow_save.buttonColor = enabledcolor
                    id_firmwareupgrade_usbrow_readusb.buttonColor = enabledcolor

                }
            }
            RowLayout {
                id: id_firmwareupgrade_statusrow
                width: id_firmwareupgrade_content.width //100
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: id_firmwareupgrade_main.horizontalCenter
                anchors.bottomMargin: 1
                Text {
                    color: "#868e96"
                    id:id_firmwareupgrade_statustext
                    font.family: Style.regular.name
                    font.pixelSize:  Style.style3
                    horizontalAlignment:Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    Connections {
                        target: firmwareUpgradeObj
                        onUpgradeCompletionStatus:
                        {
                            switch(statusVal)
                            {
                            case 0:
                                setStatusbarText( id_firmwareupgrade_statustext.text + qsTr(" \n") + qmltextFIRMWARE_UPDATE_UPGRADE_TEXT4)
                                firmwareupgradestatusmessage = id_firmwareupgrade_statustext.text + qsTr(" \n") + qmltextFIRMWARE_UPDATE_UPGRADE_TEXT4
                                bUpgrade = false
                                break;
                            case 1:
                                setStatusbarText( qmltextFIRMWARE_UPDATE_UPGRADE_TEXT5)
                                firmwareupgradestatusmessage = qmltextFIRMWARE_UPDATE_UPGRADE_TEXT5
                                bUpgrade = false
                                break;
                            case 3:
                                setStatusbarText( qmltextFIRMWARE_UPDATE_UPGRADE_TEXT6)
                                firmwareupgradestatusmessage = qmltextFIRMWARE_UPDATE_UPGRADE_TEXT6
                                bUpgrade = false
                                break;
                            }
                            //disable combobox as well
                            if(checkUsbOrEther == 0)
                            {
                                id_firmwareupgrade_ac_filelist.enabled = true
                                id_firmwareupgrade_pc_filelist.enabled = true
                                id_firmwareupgrade_sc_filelist.enabled = true
                                id_firmwareupgrade_saverow_save.buttonColor = enabledcolor
                                id_firmwareupgrade_usbrow_readusb.buttonColor = enabledcolor
                                id_firmwareupgrade_saverow_save.enabled = true
                                id_firmwareupgrade_usbrow_readusb.enabled = true
                            }
                            else if(checkUsbOrEther == 1)
                            {
                                popupPrimButtonId.buttonColor = enabledcolor
                                popupPrimButtonId.enabled = true
                                popupPrimButton2Id.buttonColor = enabledcolor
                                popupPrimButton2Id.enabled = true
                            }
                        }
                        onUpgradeProgressDetail:
                        {
                            if(nProgress > 0 )
                            {

                                switch(nModule)
                                {
                                case Firmwareupgradetypes.ACTUATOR_CONTROLLER:
                                    //case 1:
                                    id_firmwareupgrade_ac_progressbar.value = nProgress
                                    setStatusbarText(id_firmwareupgrade_ac_text.text +  qsTr(" : ") + getProgressText(nProgress))
                                    strModuleText = id_firmwareupgrade_ac_text.text
                                    break;
                                case Firmwareupgradetypes.POWER_CONTROLLER:
                                    //case 2:
                                    id_firmwareupgrade_pc_progressbar.value = nProgress
                                    setStatusbarText(id_firmwareupgrade_pc_text.text +  qsTr(" : ") + getProgressText(nProgress))
                                    strModuleText = id_firmwareupgrade_pc_text.text
                                    break;
                                case Firmwareupgradetypes.SUPERVISORY_CONTROLLER:
                                    //case 3:
                                    id_firmwareupgrade_sc_progressbar.value = nProgress
                                    setStatusbarText(id_firmwareupgrade_sc_text.text +  qsTr(" : ") + getProgressText(nProgress))
                                    strModuleText = id_firmwareupgrade_sc_text.text
                                    break;
                                }
                            }
                            if(nProgress == 0)
                            {
                                switch(nModule)
                                {
                                case Firmwareupgradetypes.ACTUATOR_CONTROLLER:
                                    id_firmwareupgrade_ac_progressbar.value = nProgress
                                    break;
                                case Firmwareupgradetypes.POWER_CONTROLLER:
                                    id_firmwareupgrade_pc_progressbar.value = nProgress
                                    break;
                                case Firmwareupgradetypes.SUPERVISORY_CONTROLLER:
                                    id_firmwareupgrade_sc_progressbar.value = nProgress
                                    break;
                                }
                            }
                        }
                    }

                }
            }
        } //end of upgrade rect
    }
    onClosed:
    {
        acfilelistmodel.clear();
        pcfilelistmodel.clear();
        scfilelistmodel.clear();
        id_firmwareupgrade_usbrow_readusb.enabled=true
        id_firmwareupgrade_saverow_save.enabled = true
        id_firmwareupgrade_saverow_save.buttonColor = enabledcolor
        id_firmwareupgrade_usbrow_readusb.buttonColor = enabledcolor
        setStatusbarText(qsTr(""))
        if(checkUsbOrEther == 0)
            firmwareUpgradeObject.clearfilelist();
    }
    onOpened:
    {
        acfilelistmodel.clear();
        pcfilelistmodel.clear();
        scfilelistmodel.clear()

        //AC Combobox
        var acfilelist = firmwareUpgradeObject.getfileslist(Firmwareupgradetypes.ACTUATOR_CONTROLLER)
        for(var acindex=0;acindex<acfilelist.length;acindex++)
        {
            acfilelistmodel.append({"text":acfilelist[acindex]})
        }

        if(id_firmwareupgrade_ac_filelist.count >0)
            id_firmwareupgrade_ac_filelist.currentIndex=0;
        ///PC comobobx box
        var pcfilelist = firmwareUpgradeObject.getfileslist(Firmwareupgradetypes.POWER_CONTROLLER)
        for(var pcindex=0;pcindex<pcfilelist.length;pcindex++)
        {
            pcfilelistmodel.append({"text":pcfilelist[pcindex]})
        }

        if(id_firmwareupgrade_pc_filelist.count >0)
            id_firmwareupgrade_pc_filelist.currentIndex=0;


        var scfilelist = firmwareUpgradeObject.getfileslist(Firmwareupgradetypes.SUPERVISORY_CONTROLLER)
        for(var scindex=0;scindex<scfilelist.length;scindex++)
        {
            scfilelistmodel.append({"text":scfilelist[scindex]})
        }

        if(id_firmwareupgrade_sc_filelist.count >0)
            id_firmwareupgrade_sc_filelist.currentIndex=0;

        //disabledcolor
        id_firmwareupgrade_saverow_save.buttonColor = disabledcolor
        id_firmwareupgrade_saverow_save.enabled = false
    }
}
