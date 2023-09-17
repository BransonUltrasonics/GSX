import QtQuick 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.eventlogging 1.0

Item {
    id: item1
    implicitHeight: 995
    implicitWidth: 1520

    readonly property string qmltextOk :  qsTr("OKAY")
    readonly property string qmltextCancel :  qsTr("CANCEL")
    readonly property string qmltextPasswordConfirmPassWrdDiff: qsTr("Password and confirm password are different")
    readonly property string qmltextCurrentNewPasswordDiff: qsTr("Current Password and New Password should be different")
    readonly property string qmltextValidationMsg :  qsTr("Password must be at least 8 characters in length, including 1 uppercase letter, 1 lowercase letter, 1 numeric character, 1 special character")
    readonly property string qmltextPasswrdChangedSuccessMsg :  "Password changed successfully"
    readonly property string qmltextPasswrdEmpty:  qsTr("Password cannot be empty")
    readonly property string qmltextWrngPasswrd :  qsTr("Wrong Password")

    property string cancelButtonLabel: qsTr("CANCEL")
    property string confirmPasswordLabel: qsTr("Confirm Password")
    property string changePasswordLabel: qsTr("Change Your Password")
    property string currentPasswordLabel: qsTr("Current Password")
    property string newPasswordLabel: qsTr("New Password")
    property string userIDLabel: qsTr("User ID")

    /* Colors */
    property string panelColor: "#ECECEC"
    property string selectingTextColor: "#ffffff"
    property string titleRectColor: "#E9ECEF"
    property string userIDValueColor: qmlPrimaryColor

    /* Margins */
    property int columnSpacing:   20
    property int columnElementheight : 60
    property int currentPassword_labelRightMargin:100
    property int columnWidth: 785
    property int confirmPassword_labelRightMargin:160
    property int labelLeftMargin:20
    property int newPassword_labelRightMargin:160
    property int panelHeight: 440
    property int rootWidth: 1520
    property int textFieldRightMargin: 40
    property int textFieldLeftMargin: 340
    property int textFieldHeight: 30

    property int checkfocus: 1

    property bool isUserIDUpdated:false
    property string temp_UserID: qmlTextEmpty
    property bool isCurrentPasswordUpdated:false
    property string temp_CurrentPassword: qmlTextEmpty
    property bool isNewPasswordUpdated:false
    property string temp_NewPassword: qmlTextEmpty
    property bool isConfirmNewPasswordUpdated:false
    property string temp_ConfirmNewPassword: qmlTextEmpty
    property string errorText: qmlTextEmpty
    property alias text_currentPassword: text_currentPassword
    readonly property string qmlTextEmpty: qsTr("")
    property DialogPopup dialogPopup

    /**
       *@breif: Update text field  value based on the focus
       **/
    function setTextFieldValue( keyboardInput )
    {
        if(checkfocus == 1)
        {
            temp_CurrentPassword = keyboardInput
            text_currentPassword.text = keyboardInput
        }
        else if( checkfocus == 2 )
        {
            temp_NewPassword = keyboardInput
            text_newPassword.text = keyboardInput
        }
        else if( checkfocus == 3 )
        {
            temp_ConfirmNewPassword = keyboardInput
            text_confirmPassword.text = keyboardInput
        }
    }

    /**
       *@breif: To validate password and confirm password
       **/
    function passwordCheck(password, confirmPassword,currentPassword)
    {
        var passwordCheckStatus = false

        if (password !== confirmPassword)
        {
            errorText = qmltextPasswordConfirmPassWrdDiff;
        }
        else if (password == currentPassword)
        {
            errorText = qmltextCurrentNewPasswordDiff;
        }
        else
        {
            //var regExp = /^(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,16}$/;
            var regExp =/(?=^.{8,16}$)(?=.*\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%^&amp;*()_+}{&quot;:;'?/&gt;.&lt;,])(?!.*\s).*$/;
            if(regExp.test(password))
            {
                errorInPassword.text = qmlTextEmpty;
                passwordCheckStatus = true
            }
            else
            {
                errorText = qmltextValidationMsg;
            }
        }

        return passwordCheckStatus
    }

    /**
      *@breif: Open Dialog popup
      **/
    function createDialogpopup(message)
    {
        var comment = qmlTextEmpty
        var component = Qt.createComponent("DialogPopup.qml")
        dialogPopup = component.createObject(window)
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
                m_NewPassword.setNewPassword(temp_NewPassword)
                clearTextFields()
                errorInPassword.color="#8BC24A"
                errorText=qmltextPasswrdChangedSuccessMsg
                Eventlogger.eventSend(EventLogging.UI_EVENT_PASSWORD_CHANGED, comment);
                if(reqLoginExpriyPwd == true)
                {
                    reqLoginExpriyPwd = false
                    mainwindow.visible=false
                    loaderelement.setSource("Login.qml")
                    loaderelement.item.loadinitialView()
                    recipe.updateStatusText(qsTr("Password changed successfully."))
                }
                if(login.isFirstLogin())
                {
                    firstTimeLogin = false
                    mainwindow.visible=false
                    loaderelement.setSource("Login.qml")
                    loaderelement.item.loadinitialView()
                    recipe.updateStatusText(qsTr("Password changed successfully."))
                }
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

    /**
       *@breif: save the user details
       **/
    function onSave()
    {
        if(temp_CurrentPassword!=qmlTextEmpty&&m_NewPassword.isUserDataValid(temp_UserID,temp_CurrentPassword))
        {
            errorText=qmlTextEmpty
            if  (temp_NewPassword!=qmlTextEmpty||temp_ConfirmNewPassword!=qmlTextEmpty)
            {
                if (passwordCheck(temp_NewPassword,temp_ConfirmNewPassword,temp_CurrentPassword))
                {

					createDialogpopup(qmltextPasswrdChangedSuccessMsg)
                }
            }
            else if(temp_NewPassword==qmlTextEmpty && temp_ConfirmNewPassword==qmlTextEmpty)
            {

                errorInPassword.color="#f03e3e"
                errorText=qmltextPasswrdEmpty
            }
        }
        else
        {
            errorInPassword.color="#f03e3e"
            errorText=qmltextWrngPasswrd
        }
        errorInPassword.text= errorText.toString()
        saveButton.enabled = true
    }

    /**
       *@breif: To clear all the text fields
       **/
    function clearTextFields()
    {
        temp_CurrentPassword=qmlTextEmpty
        temp_NewPassword=qmlTextEmpty
        temp_ConfirmNewPassword=qmlTextEmpty
        text_currentPassword.text=temp_CurrentPassword
        text_newPassword.text=temp_NewPassword
        text_confirmPassword.text=temp_ConfirmNewPassword
        errorInPassword.color="#f03e3e"
    }

    Component.onCompleted:
    {
        if(reqLoginExpriyPwd || login.isFirstLogin())
        {
            temp_UserID = login.m_username
            m_NewPassword.setUserID(temp_UserID)
            m_NewPassword.getPasswordFromDB()
        }
    }

    Rectangle {
        id: title_rect
        width: parent.width
        height: columnElementheight
        color: titleRectColor
        Text {
            id: title_change_password
            x:parent.width/2-width/2
            height: parent.height
            color: userIDValueColor
            text: changePasswordLabel
            font.pixelSize: Style.style7
            anchors.centerIn: title_rect
            font.family:Style.semibold.name
        }
    }

    Flickable{
        id: changePasswordFlick
        width: parent.width
        height: parent.height
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.top: title_rect.bottom
        anchors.topMargin: 100
        interactive: Qt.inputMethod.visible ? true : false
        bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
        clip: true

        Rectangle
        {
            id: changePasswordRect
            width: parent.width * (50/100)
            height: parent.height * (80/100)
            x: parent.width/2-width/2

            Rectangle
            {
                id:userid_Rect
                width: parent.width
                height: 60
                color:qmlPrimaryColor
                Text
                {
                    id: userId_text
                    x:parent.width/2-width/2
                    height: parent.height
                    color: "#ffffff"
                    text: temp_UserID
                    font.pixelSize: Style.style6
                    verticalAlignment:  Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    Component.onCompleted:
                    {
                        //temp_UserID=text
                        isUserIDUpdated=true
                    }

                    onTextChanged:
                    {
                        if(isUserIDUpdated)
                        {
                            temp_UserID=text
                        }
                    }
                    font.family:Style.semibold.name
                }
            }

            Rectangle
            {
                id:leftPanelRect
                width: parent.width
                height: parent.height * (40/100)
                color:panelColor
                anchors.topMargin: 0
                anchors.top: userid_Rect.bottom
                DropShadow
                {
                    anchors.fill: column
                    horizontalOffset: 3
                    verticalOffset: 3
                    color: "#000000"
                    opacity: 0.2
                    source: column
                }
                Column
                {
                    id:column
                    width:parent.width-2*column.spacing
                    height:parent.height-2*column.spacing

                    spacing:(columnSpacing/panelHeight)*parent.height
                    y:(parent.height-height)/2
                    x:parent.width/2-width/2

                    ChangePasswordItem{
                        id: changePasswordItem1

                        width:parent.width
                        height:(column.height-(2*column.spacing))/3

                        Text
                        {
                            y: parent.height/2-height/2
                            text:currentPasswordLabel
                            height: (textFieldHeight/columnElementheight)*parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: (labelLeftMargin/(columnWidth))*parent.width
                            anchors.right: text_currentPassword.left
                            anchors.rightMargin: (currentPassword_labelRightMargin/(columnWidth))*parent.width
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.regular.name
                            font.pixelSize:  Style.style5
                        }

                        LineEdit
                        {
                            id: text_currentPassword
                            y: parent.height/2-height/2
                            height: (textFieldHeight/columnElementheight)*parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: (textFieldLeftMargin/columnWidth)*parent.width
                            anchors.rightMargin: (textFieldRightMargin/columnWidth)*parent.width
                            anchors.right: parent.right
                            font.pixelSize: Style.style3
                            echoMode: TextField.Password
                            selectByMouse: true
                            verticalAlignment:TextField.AlignVCenter
                            Component.onCompleted:
                            {
                                text= qmlTextEmpty
                                temp_CurrentPassword==text
                                isCurrentPasswordUpdated=true
                            }
                            onTextChanged:
                            {
                                if(isCurrentPasswordUpdated)
                                {
                                    temp_CurrentPassword=text
                                }
                            }
                            onFocusChanged:
                            {
                                var isfocus = text_currentPassword.focus
                                if(isfocus)
                                    changePasswordFlick.contentY = text_currentPassword.y + text_currentPassword.height
                            }
                        }
                    }

                    ChangePasswordItem{
                        id: changePasswordItem2

                        width:parent.width
                        height:(column.height-(2*column.spacing))/3

                        Text
                        {
                            y: parent.height/2-height/2
                            text: newPasswordLabel
                            height: (textFieldHeight/columnElementheight)*parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: (labelLeftMargin/columnWidth)*parent.width
                            anchors.right: text_newPassword.left
                            anchors.rightMargin: (newPassword_labelRightMargin/columnWidth)*parent.width
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.regular.name
                            font.pixelSize:  Style.style5
                        }

                        LineEdit
                        {
                            id: text_newPassword
                            y: parent.height/2-height/2
                            height: (textFieldHeight/columnElementheight)*parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: (textFieldLeftMargin/columnWidth)*parent.width
                            anchors.rightMargin: (textFieldRightMargin/columnWidth)*parent.width
                            font.pixelSize: Style.style3
                            anchors.right: parent.right
                            verticalAlignment:TextField.AlignVCenter
                            echoMode: TextField.Password
                            selectByMouse: true
                            Component.onCompleted:
                            {
                                text= qmlTextEmpty
                                temp_NewPassword==text
                                isNewPasswordUpdated=true
                            }

                            onTextChanged:
                            {

                                if(isNewPasswordUpdated)
                                {
                                    temp_NewPassword=text
                                }
                            }
                            onFocusChanged:
                            {
                                var isfocus = text_newPassword.focus
                                if(isfocus)
                                    changePasswordFlick.contentY = text_newPassword.y + text_newPassword.height
                            }
                        }
                    }

                    ChangePasswordItem{
                        id: changePasswordItem3

                        width:parent.width
                        height:(column.height-(2*column.spacing))/3

                        Text
                        {
                            y: parent.height/2-height/2
                            height: (textFieldHeight/columnElementheight)*parent.height
                            text: confirmPasswordLabel
                            anchors.left: parent.left
                            anchors.leftMargin: (labelLeftMargin/columnWidth)*parent.width
                            anchors.right: text_confirmPassword.left
                            anchors.rightMargin: (confirmPassword_labelRightMargin/columnWidth)*parent.width
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.regular.name
                            font.pixelSize:  Style.style5
                        }

                        LineEdit
                        {
                            id: text_confirmPassword
                            y: parent.height/2-height/2
                            height: (textFieldHeight/columnElementheight)*parent.height
                            text: temp_ConfirmNewPassword
                            anchors.left: parent.left
                            anchors.leftMargin: (textFieldLeftMargin/columnWidth)*parent.width
                            anchors.rightMargin: (textFieldRightMargin/columnWidth)*parent.width
                            anchors.right: parent.right
                            font.pixelSize: Style.style3
                            verticalAlignment:TextField.AlignVCenter
                            echoMode: TextInput.Password
                            selectByMouse: true
                            Component.onCompleted:
                            {
                                text= qmlTextEmpty
                                temp_ConfirmNewPassword==text
                                isConfirmNewPasswordUpdated=true
                            }
                            onTextChanged:
                            {
                                if(isConfirmNewPasswordUpdated)
                                {
                                    temp_ConfirmNewPassword=text
                                }
                            }
                            onFocusChanged:
                            {
                                var isfocus = text_confirmPassword.focus
                                if(isfocus)
                                    changePasswordFlick.contentY = text_confirmPassword.y + 3*text_confirmPassword.height
                            }
                        }
                    }
                }
            }
            Rectangle
            {
                id: butttonsRect
                anchors.top: leftPanelRect.bottom
                anchors.right: leftPanelRect.right
                anchors.left: leftPanelRect.left
                anchors.topMargin: 20
                RowLayout {
                    spacing: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    SecondaryButton
                    {
                        id: cancelButton
                        text: qmltextCancel
                        onClicked: {
                            clearTextFields()
                            errorInPassword.text=qmlTextEmpty
                            if(reqLoginExpriyPwd == true || firstTimeLogin == true)
                            {
                                callLogoutFunction()
                            }

                        }
                    }

                    PrimaryButton
                    {
                        id: saveButton
                        text:qmltextOk
                        onClicked:
                        {
                            saveButton.enabled = false
                            Qt.callLater(onSave)
                        }
                    }
                }
            }

            Text
            {
                id: errorInPassword
                x: leftPanelRect.x
                width: (280/rootWidth)*parent.width
                anchors.topMargin: 50
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                anchors.top: butttonsRect.bottom
                font.pixelSize: Style.style1
                font.family: Style.regular.name
                color:"#f03e3e"
            }
        }
    }
}
