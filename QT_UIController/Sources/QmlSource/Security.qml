import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import Style 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import Qt.labs.platform 1.0
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import com.branson.runtimefeatures 1.0
import com.branson.errorcodes 1.0

Item {
    readonly property string qmltextWebServices: qsTr("Enable Web Service Communication")
    readonly property string qmltextFOE: qsTr("Enable Software Upgrade Over Ethernet")
    readonly property string qmltextPrivateKey: qsTr("Read USB")
    readonly property string qmltextSavePrivateKey: qsTr("Save")
    readonly property string invalidKey: qsTr("Invalid Key")
    readonly property string qmlTextSettings: qsTr("Settings")
    readonly property string qmlTextKey: qsTr("Authentication Key")
    readonly property string qmlTextFailed: qsTr("Authentication key upload failed due to wrong key length or invalid characters")
    readonly property string qmlTextSuccess: qsTr("Authentication key uploaded successfully ")
    readonly property string qmltextWebServicesSecurity: qsTr("Web Services Security")
    readonly property string qmltextInvalidUserOrPassword:qsTr("Invalid UserName or Password")
    readonly property string qmltextPasswordExpired: qsTr("Your password is expired")
    readonly property string qmlTextRequsetTimedOut: qsTr("Request timed out")
    readonly property string qmlTextUSBNotConnected: qsTr("Plugin the USB device and retry")
    readonly property string qmlTextNoFiles: qsTr("No private key files (.keys) found. Please make sure the key files are in the root directory of USB device and try again")
    readonly property string qmlTextFilesList: qsTr("Populated the private key file list. Click on drop down for options")
    readonly property string qmlTextNoneToUpload: qsTr("None to upload")
    readonly property string qmlTextNoBlankSupport: qsTr("Blank is not supported")
    readonly property string qmlTextFileOpenError: qsTr("Authentication Key Upload Failed::Unable to open private key file")
    readonly property string qmlTextNoKey: qsTr("Authentication Key Upload Failed::Private key is not available in file")
    readonly property string qmlTextFileReadError: qsTr("Authentication Key Upload Failed::Unable to read private key file")
    readonly property string qmlTextInvalidKeyFormat: qsTr("Authentication Key Upload Failed::Invalid private key Format")
    readonly property string qmlTextInvalidKey: qsTr("Authentication Key Upload Failed::Invalid private key")
    readonly property string qmlTextKeySuccess: qsTr("Authentication Key Uploaded Successfully")
    readonly property string qmlTextKeyFail: qsTr("Authentication Key Upload Failed")
    readonly property string qmltextCancel:qsTr("CANCEL")
    readonly property string qmltextSave:qsTr("SAVE")
    readonly property string qmltextSaveSuccess:qsTr("Security settings saved successfully. Please restart")
    property bool refreshSwitch: false
    property int topMargin: 50
    property int daysLeft: 0
    property string  disabledcolor: "#757575"
    property string enabledcolor  : qmlPrimaryColor
    property string none: "None"
    id:dataid

    Component.onCompleted:
    {
        bransonDataObj.getWebServiceOptions()
        firstElement_Switch.checked = bransonDataObj.getWebServiceStatus()
        secondElement_Switch.checked = bransonDataObj.getFOEStatus()

        privatekeyfilelistmodel.clear()
        config.clearPrivateKeyFilesList()
        var privatekeyfilelist = config.getPrivateKeyFilesList()
        for(var privatekeyindex = 0;privatekeyindex < privatekeyfilelist.length;privatekeyindex++)
        {
            privatekeyfilelistmodel.append({"text":privatekeyfilelist[privatekeyindex]})
        }

        if(id_privateKey_filelist.count > 0)
            id_privateKey_filelist.currentIndex=0;
    }

    Rectangle
    {
        id: main_TitleRect
        width: parent.width
        height: 60
        color: "#ECECEC"
        anchors.topMargin: 0
        Text
        {
            id: id_TitleTxt
            text: qmlTextSettings
            color: qmlPrimaryColor
            font.bold: true
            font.pixelSize: Style.style7
            font.family:  Style.semibold.name
            anchors.centerIn: main_TitleRect
        }
    }
    Rectangle{
        id:mainRect
        width: parent.width - 30
        height: parent.height - (topMargin + 400)
        anchors.left: parent.left
        anchors.top: main_TitleRect.bottom
        anchors.topMargin: 5
        color:"#ECECEC"

        Rectangle {
            id: centerRect
            anchors.centerIn: parent
            width: 20
            height: parent.height
            color: "#ffffff"
            Rectangle {
                id: centerLine
                anchors.centerIn: parent
                width: 1
                height: parent.height
                color: "#000000"
            }
        }

        DropShadow
        {
            id: firstElement_DropShadow
            source: firstElement_Rect
            anchors.fill: firstElement_Rect
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.5
            radius: 3
        }
        Rectangle
        {
            id: firstElement_Rect
            anchors.top: mainRect.top
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 15
            width: (parent.width - 80)/2
            height: 60
            color: "#ffffff"
            Text
            {
                id: firstElement_Text
                text:qmltextWebServices
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideMiddle
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize: Style.style3
            }
            ToogleBar
            {
                id: firstElement_Switch
                width: (70/900)*parent.width
                height: 40
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
            }
        }

        DropShadow
        {
            id: secondElement_DropShadow
            source: secondElement_Rect
            anchors.fill: secondElement_Rect
            horizontalOffset: 3
            verticalOffset: 3
            color: "#000000"
            opacity: 0.5
            radius: 3
        }
        Rectangle
        {
            id: secondElement_Rect
            anchors.top: firstElement_Rect.bottom
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 15
            width: (parent.width - 80)/2
            height: 60
            color: "#ffffff"
            Text
            {
                id: secondElement_Text
                text:qmltextFOE
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideMiddle
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize: Style.style3
            }
            ToogleBar
            {
                id: secondElement_Switch
                width: (70/900)*parent.width
                height: 40
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
            }
        }

        Flickable{
            id: ipFlick
            width: parent.width
            height: parent.height
            flickableDirection: Flickable.VerticalFlick
            anchors.fill: parent
            interactive: Qt.inputMethod.visible ? true : false
            bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
            clip: true
            Rectangle
            {
                id: id_parent_Column
                color:"#ECECEC"
                width: (parent.width - 50)/2
                height: parent.height * (42/100)
                anchors.top: mainRect.top
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: (parent.width/2) + 15
                DropShadow  {
                    anchors.fill: id_MainCol
                    horizontalOffset: 3
                    verticalOffset: 3
                    color: "#000000"
                    opacity: 0.5
                    source: id_MainCol
                    radius: 3
                }
                Column
                {
                    id: id_MainCol
                    spacing: 20
                    width: id_parent_Column.width - 15
                    y: 20
                    leftPadding: 10
                    Repeater
                    {
                        model: [qsTr("IP Address :"), qsTr("Subnet Mask :"),qsTr("Gateway :")]

                        Rectangle
                        {
                            property alias ipAddressText: id_IP_Config_txtInput.text
                            id: id_handler
                            width: id_MainCol.width
                            height: 60

                            Text
                            {
                                id: id_IP_Config_Lbl
                                width: id_handler.width/2
                                height: id_handler.height
                                elide: Text.ElideMiddle
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: 20
                                font.family: Style.regular.name
                                font.pixelSize: Style.style3
                                text: modelData

                            }

                            Rectangle
                            {
                                id:id_IP_Config_txtInput_Rect
                                width: id_handler.width/2 - 10
                                height: id_handler.height
                                anchors.left: id_IP_Config_Lbl.right

                                LineEdit
                                {
                                    id: id_IP_Config_txtInput
                                    width: id_IP_Config_txtInput_Rect.width - 10
                                    height: id_IP_Config_txtInput_Rect.height - 10
                                    focus: false
                                    selectByMouse: true
                                    font.pixelSize: Style.style3
                                    font.family:  Style.semibold.name
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    inputMethodHints: Qt.ImhDigitsOnly
                                    anchors.top: id_IP_Config_txtInput_Rect.top
                                    anchors.topMargin: 5
                                    anchors.left: id_IP_Config_txtInput_Rect.left
                                    anchors.leftMargin: 5
                                    validator: RegExpValidator
                                    {
                                        regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                                        //regExp: /^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
                                    }
                                    onTextChanged:
                                    {
                                        ipConfigObj.saveChangedValue(ipAddressText,index)
                                    }
                                    Component.onCompleted:
                                    {
                                        ipConfigObj.readIPv4Details()

                                        if(index == 0)
                                            id_IP_Config_txtInput.text = ipConfigObj.getIP()
                                        else if(index == 1)
                                            id_IP_Config_txtInput.text = ipConfigObj.getSubnetMask()
                                        else
                                            id_IP_Config_txtInput.text = ipConfigObj.getGatewayAddr()
                                    }
                                    onActiveFocusChanged:
                                    {
                                        if (id_IP_Config_txtInput.focus)
                                        {
                                            console.log("\nindex::"+id_IP_Config_txtInput.focus)
                                            ipFlick.contentY = id_IP_Config_txtInput.height*index + id_IP_Config_txtInput.y
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: bottom_TitleRect
        width: parent.width
        height: 60
        color: "#ECECEC"
        anchors.top: mainRect.bottom
        anchors.topMargin: 30
        Text
        {
            id: bottom_TitleTxt
            text: qmlTextKey
            color: qmlPrimaryColor
            font.bold: true
            font.pixelSize: Style.style7
            font.family:  Style.semibold.name
            anchors.centerIn: bottom_TitleRect
        }
    }
    Rectangle
    {
        id:belowRect
        width: mainRect.width
        height: 100
        anchors.left: parent.left
        anchors.top: bottom_TitleRect.bottom
        anchors.topMargin: 5
        color:"#ECECEC"
        DropShadow{
            id:privateKeyDropshadow
            source: recPrivateKey
            anchors.fill: recPrivateKey
            horizontalOffset: 1
            verticalOffset: 1
            color: "#000000"
            opacity: 0.5
            radius: 3
        }

        Rectangle
        {
            id: recPrivateKey
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: belowRect.top
            anchors.topMargin: 10
            width:parent.width*0.50
            height: 60
            color:"#FFFFFF"
            PrimaryButton
            {
                id: uploadAuthenticationKey
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                text: qmltextPrivateKey
                width: 200
                onClicked:
                {
                    secondLevelAuthentication.open()
                    disableMainwindowOpacity.start()
                    secondLevelAuthentication.passwordLineEditText = ""
                    secondLevelAuthentication.usernameLineEditText = ""
                    secondLevelAuthentication.open()
                    secondLevelAuthentication.onOkay.connect(function onOkay()
                    {
                        var dwRet = login.loginValidate(secondLevelAuthentication.usernameLineEditText, secondLevelAuthentication.passwordLineEditText)

                        if(dwRet === ErrorCode.SUCESS)
                        {
                            var lastChangedPassword = login.lastChangedPassword

                            daysLeft = login.validatePasswordExpiry()

                            if (secondLevelAuthentication.usernameLineEditText != "ADMIN" && daysLeft >= qmlGlobalSettingsObject.passwordExpiryInDays)
                            {
                                recipe.updateStatusText(qmltextPasswordExpired)
                            }
                            else
                            {
                                var readusbresponse = config.readPrivateKeyFiles()
                                privatekeyfilelistmodel.clear()

                                var privatekeyfilelist = config.getPrivateKeyFilesList()
                                for(var privatekeyindex = 0;privatekeyindex < privatekeyfilelist.length;privatekeyindex++)
                                {
                                    privatekeyfilelistmodel.append({"text":privatekeyfilelist[privatekeyindex]})
                                }

                                if(id_privateKey_filelist.count >0)
                                    id_privateKey_filelist.currentIndex=0;

                                switch(readusbresponse)
                                {
                                case 0:
                                    /* Request Time out */
                                    recipe.updateStatusText(qmlTextRequsetTimedOut)
                                    break;

                                case 1:
                                    /* usb not plugged in*/
                                    recipe.updateStatusText(qmlTextUSBNotConnected)
                                    break;

                                case 2:
                                    /* No files found in USB */
                                    recipe.updateStatusText(qmlTextNoFiles)
                                    break;

                                default:
                                    /* successful files found */
                                    recipe.updateStatusText(qmlTextFilesList)
                                    break;
                                }
                            }
                        }
                        else
                        {
                            recipe.updateStatusText(qmltextInvalidUserOrPassword)
                            secondLevelAuthentication.passwordLineEditText = ""
                            secondLevelAuthentication.usernameLineEditText = ""
                        }
                        secondLevelAuthentication.close()
                        secondLevelAuthentication.onOkay.disconnect(onOkay)
                    })
                }
            }
            CustomCombobox {
                id: id_privateKey_filelist
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: uploadAuthenticationKey.right
                anchors.leftMargin: 30
                anchors.right: recPrivateKey.right
                anchors.rightMargin: 20
                comboboxpopupheight:120
                model : ListModel {
                    id:privatekeyfilelistmodel
                }
                Component.onCompleted: {
                    if(id_privateKey_filelist.count >0)
                    {
                        id_privateKey_filelist.currentIndex=0;
                    }

                }
            }
        }
    }
    Row
    {
        id: settings_Cancel_layout
        width: parent.width
        height: 60
        anchors.bottom :parent.bottom
        anchors.bottomMargin: 100
        leftPadding: parent.width - 320
        spacing: 20

        SecondaryButton
        {
            id:  id_cancelBtn
            text: qmltextCancel
            onClicked:
            {

            }
        }
        PrimaryButton
        {
            id: id_OkBtn
            text: qmltextSave
            onClicked:
            {
                var readusbresponse = 5
                /* Saving the Key Info*/
                if(id_privateKey_filelist.currentText == none || id_privateKey_filelist.count <= 0)
                {
                    readusbresponse = 0
                }
                else
                {
                    /* send to BL and update  the status */
                    readusbresponse = config.uploadAuthenticationKey(id_privateKey_filelist.currentIndex)
                    switch(readusbresponse)
                    {
                    case 3:
                        /* Private key is not available in file */
                        recipe.updateStatusText(qmlTextNoKey)
                        break;
                    case 4:
                        /* Unable to open private key file */
                        recipe.updateStatusText(qmlTextFileOpenError)
                        break;
                    case 5:
                        /* Unable to read private key file */
                        recipe.updateStatusText(qmlTextFileReadError)
                        break;
                    case 6:
                        /* Invalid private key Format */
                        recipe.updateStatusText(qmlTextInvalidKeyFormat)
                        break;
                    case 7:
                        /* Invalid private key */
                        recipe.updateStatusText(qmlTextInvalidKey)
                        break;
                    case 0 :
                        /* successful files found */
                        break;
                    default:
                        recipe.updateStatusText(qmlTextKeyFail)
                        break;
                    }
                }
                // If Key upload is success then only we have save the others else show fail in status
                if(readusbresponse == 0)
                {
                    /* Saving the Switch Details*/
                    bransonDataObj.saveWebServiceOptions(firstElement_Switch.checked,secondElement_Switch.checked)

                    /* Saving the IP configuration Details*/
                    ipConfigObj.saveIPv4Details()
                    recipe.updateStatusText(qmltextSaveSuccess)
                }
            }
        }
    }
}
