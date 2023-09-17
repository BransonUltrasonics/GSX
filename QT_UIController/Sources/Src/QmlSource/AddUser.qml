import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Private 1.0
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.eventlogging 1.0

Item
{
    id: addUserItem
    readonly property string qmltextUserName : qsTr("Enter Username")
    readonly property string qmltextUserId : qsTr("User Id")
    readonly property string qmltextEnterPassWord :qsTr("Enter Password")
    readonly property string qmltextPassWord :qsTr("Password")
    readonly property string qmltextEnterConfirmPassword :qsTr("Enter Confirm Password")
    readonly property string qmltextConfirmPassword : qsTr("Confirm Password")
    readonly property var qmltextUserLevelExecutive : qsTr("Executive")
    readonly property var qmltextUserLevelSupervisor : qsTr("Supervisor")
    readonly property var qmltextUserLevelTechnician : qsTr("Technician")
    readonly property var qmltextUserLevelOperator : qsTr("Operator")
    readonly property string qmltextUserLevel : qsTr("User Level")
    readonly property string qmltextRightText :qsTr("Enabled")
    readonly property string qmltextleftText : qsTr("Disabled")
    readonly property string qmltextRightLockedText:  qsTr("Locked")
    readonly property string qmltextLeftUnlockedText: qsTr("Unlocked")
    readonly property string qmltextRightTextNoLanguage :"Enabled"
    readonly property string qmltextleftTextNoLanguage : "Disabled"
    readonly property string qmltextRightLockedTextNoLanguage: "Locked"
    readonly property string qmltextLeftUnlockedTextNoLanguage: "Unlocked"
    readonly property string qmltextStatus : qsTr("Status")
    readonly property string qmltextLock:    qsTr("Is Locked User")
    readonly property string qmltextErrorText : qsTr("")
    readonly property string qmltextCancelButton :qsTr("CANCEL")
    readonly property string qmltextSaveButton :qsTr("SAVE")
    readonly property string qmltexttabButtonText:qsTr("USER LIST")
    readonly property string qmltextUpdatedMsg:qsTr("User updated successfully")
    readonly property string qmltextUserAddedMsg:qsTr("User added successfully")
    readonly property string qmltextCommtReqMsg:qsTr("Comment is required")
    readonly property string qmltextPassWordEmptyMsg:qsTr("Password or Confirm Password is empty")
    readonly property string qmltextPassWordDiffMsg:qsTr("Password and Confirm Password are different")
    readonly property string qmltextValidPasswordMsg:qsTr("Password must be at least 8 characters in length, including 1 uppercase letter, 1 lowercase letter, 1 numeric character, 1 special character")
    readonly property string qmltextUserExistMsg:qsTr("User Id already exist, please enter new User ID")
    readonly property string qmltextUpdate:qsTr("UPDATE")
    readonly property string qmltextUserNameEmptyMsg:qsTr("Username is empty")
    readonly property string qmltextNewUserCreatedMsg:"New user is created"
    readonly property string qmltextUpdatedUserMsg:"Updated existing user"


    property var windowHeight :1080
    property var windowWidth :1920
    property var columnSpace :20
    property var itemPosition: 20
    property var columnHeightLeft: 700
    property var columnHeightRight: 610
    property var gridElementHeight:90
    property var gridElementWidth:830
    property var columnWidth:870
    property var linePositionY: 0
    property real vinternalRectheight:1080-85
    property var comboBoxWidth: 250
    property var comboBoxHeight: 50
    property var sysconfigMainWindowPositionX : 20
    property alias systemConfigButton1: systemConfigButton1
    property alias systemConfigButton2: systemConfigButton2
    property alias userListComoboBox: userListComoboBox
    property bool isStatusUpdated:false
    property bool isLockUpdated: false
    property alias name: userIdLineEdit.text
    property int  selectRowIndex
    property bool isUserErrorExist: false
    property string oldUserid
    property int checkfocus: 1
    property string olduserPassword: ""
    property string userlastChangedPasswordDate: ""
    property DialogPopup dialogPopup

    /**
      *@breif: On save the data will be saved to DB
      **/
    function sendDatatoDB(eventComment)
    {
        if (!isUserErrorExist)
        {
            var userId = userIdLineEdit.text.trim()
            var password = lineEditPassword.text.trim()
            var userLevelText = userListComoboBox.userLevelText.trim()
            if(userListComoboBox.currentIndex == 0)
                userLevelText = "Executive"
            else if(userListComoboBox.currentIndex == 1)
                userLevelText = "Supervisor"
            else if(userListComoboBox.currentIndex == 2)
                userLevelText = "Technician"
            else if(userListComoboBox.currentIndex == 3)
                userLevelText = "Operator"

            var userStatus = ""
            if(isStatusUpdated)
                userStatus = qmltextRightTextNoLanguage
            else
                userStatus = qmltextleftTextNoLanguage

            var userLock = ""
            if(isLockUpdated) //false is unlock
                userLock = qmltextRightLockedTextNoLanguage
            else
                userLock = qmltextLeftUnlockedTextNoLanguage

            var dateTimeFormat =  "yyyy/MM/dd hh:mm:ss"
            var lastModified = Qt.formatDateTime(sysconfig.TitleBarDate,dateTimeFormat).toString()
            var lastChangedPassword
            var oldUserLock
            var numLoginAttempt
            var timeLoginFrom
            var timeLoginTo
            if (customButton1.text == qmltextUpdate)
            {
                if (!isUserErrorExist)
                {
                    var newPassword = ""

                    if (password === "")
                    {
                        newPassword = olduserPassword
                        lastChangedPassword = userlastChangedPasswordDate
                    }
                    else
                    {
                        newPassword = password
                        lastChangedPassword = lastModified
                    }

                    oldUserLock = userModel.get(selectRowIndex).userLock
                    numLoginAttempt = userModel.get(selectRowIndex).userNumLoginAttempt
                    timeLoginFrom = userModel.get(selectRowIndex).userTimeLoginFrom
                    timeLoginTo = userModel.get(selectRowIndex).userTimeLoginTo
                    if(userLock != oldUserLock)
                    {
                        if(userLock == qmltextLeftUnlockedTextNoLanguage)
                            numLoginAttempt = "0"
                    }

                    userModel.updateUserDetails(selectRowIndex, userId, newPassword, userLevelText, userStatus,
                                                lastModified, lastChangedPassword, lastModified, userLock,
                                                numLoginAttempt, timeLoginFrom, timeLoginTo)
                    userListLayout.currentIndex = 0
                    tabButtonText = qmltexttabButtonText
                    Eventlogger.eventSend(EventLogging.UI_EVENT_USER_MODIFIED, eventComment);
                    recipe.updateStatusText(qmltextUpdatedMsg)
                }
            }
            else
            {
                if (!isUserErrorExist)
                {
                    numLoginAttempt = "0"
                    timeLoginFrom = lastModified
                    timeLoginTo = lastModified
                    lastChangedPassword = lastModified
                    userModel.addUserDetails(userId, password, userLevelText, userStatus, lastModified, lastChangedPassword, lastChangedPassword,
                                             userLock, numLoginAttempt, timeLoginFrom, timeLoginTo)
                    userListLayout.currentIndex = 0
                    tabButtonText = qmltexttabButtonText
                    Eventlogger.eventSend(EventLogging.UI_EVENT_USER_CREATED, eventComment);
                    recipe.updateStatusText(qmltextUserAddedMsg)
                }
            }
        }
    }

    /**
      *@breif: Open Dialog popup
      **/
    function createDialogpopup(message)
    {
        var comment = ""
        var component = Qt.createComponent("DialogPopup.qml")
        dialogPopup = component.createObject(mainwindow)
        dialogPopup.open(messagePopup.info_title, message)
        dialogPopup.onSave.connect(function onOkay()
        {
            comment = dialogPopup.quitMessage
            if (comment === "")
            {
                isUserErrorExist = true
                recipe.updateStatusText(qmltextCommtReqMsg)
            }
            else
            {
                isUserErrorExist = false
                sendDatatoDB(comment)
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
      *@breif: Focus change on the selection of textfield
      **/
    function setUserMngKeyValue( keyboardInput )
    {
        if(checkfocus == 1)
        {
            userIdLineEdit.input.text = keyboardInput
        }
        else if(checkfocus == 2)
        {
            lineEditPassword.input.text = keyboardInput
        }
        else if(checkfocus == 3)
        {
            lineEditConfirmPassword.input.text = keyboardInput
        }
    }

    /**
      *@breif: Clears all the textfields in the add user tab
      **/
    function clearText()
    {
        userIdLineEdit.text = ''
        lineEditPassword.text = ''
        lineEditConfirmPassword.text = ''
        userListComoboBox.userLevel = 0
        systemConfigButton1.checked = true
        systemConfigButton2.checked = false //unlocked
        customButton1.text = qmltextSaveButton
        isUserErrorExist = false
        error_text.text = "";
        userIdLineEdit.readOnlyText = false
        userIdLineEdit.setBackgroundColor("#FFFFFF")
        olduserPassword = ""
    }

    /**
      *@breif: Verify both password and confirm password are same or not
      **/
    function passwordCheck(password, confirmPassword)
    {
        if (password === "" || confirmPassword === "")
        {
            isUserErrorExist = true
            recipe.updateStatusText(qmltextPassWordEmptyMsg)
        }
        else
        {
            if (password !== confirmPassword)
            {
                isUserErrorExist = true
                recipe.updateStatusText(qmltextPassWordDiffMsg)
            }
            else
            {
                var regExp =/(?=^.{8,16}$)(?=.*\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%^&amp;*()_+}{&quot;:;'?/&gt;.&lt;,])(?!.*\s).*$/;
                if(regExp.test(password))
                {
                    isUserErrorExist = false
                    error_text.text = "";
                }
                else
                {
                    isUserErrorExist = true
                    recipe.updateStatusText(qmltextValidPasswordMsg)
                }
            }
        }
    }

    /**
      *@breif: Check if user id is already existing or not
      **/
    function checkUserId(userId)
    {
        isUserErrorExist = userModel.checkUserId(userId)

        if (isUserErrorExist)
        {
            recipe.updateStatusText(qmltextUserExistMsg)
        }
    }

    /**
      *@breif: Modify the existing user data
      **/
    function modifyUserData(selectedRowIndex, userId, userPassword, userLevel, userStatus, userLastChangedPassword, userLock)
    {
        isUserErrorExist = false
        error_text.text = "";

        if (userStatus === qmltextRightTextNoLanguage)
        {
            systemConfigButton1.checked = true
        }
        else
        {
            systemConfigButton1.checked = false
        }

        if (userLock === qmltextRightLockedTextNoLanguage)
        {
            systemConfigButton2.checked = true
        }
        else
        {
            systemConfigButton2.checked = false
        }

        customButton1.text = qmltextUpdate
        userListComoboBox.userLevel = userLevel
        selectRowIndex = selectedRowIndex
        userListLayout.currentIndex = 0
        userIdLineEdit.text = userId
        lineEditPassword.text = ""
        lineEditConfirmPassword.text = ""
        olduserPassword = userPassword
        oldUserid = userId
        login.UserOnModifyScreen = userId
        userlastChangedPasswordDate = userLastChangedPassword
        // enable below lines if we want to readonlyText
        if (customButton1.text == qmltextUpdate)
        {
            userIdLineEdit.readOnlyText = true
            userIdLineEdit.setBackgroundColor("#BEC1C3")
        }
    }

    /**
      *@breif: Saving the new user details
      **/
    function onSave()
    {
        var userId = userIdLineEdit.text.trim()
        var password = lineEditPassword.text.trim()
        var confirmPassword = lineEditConfirmPassword.text.trim()
        var userLevelText = userListComoboBox.userLevelText.trim()

        if(userListComoboBox.currentIndex == 0)
            userLevelText = "Executive"
        else if(userListComoboBox.currentIndex == 1)
            userLevelText = "Supervisor"
        else if(userListComoboBox.currentIndex == 2)
            userLevelText = "Technician"
        else if(userListComoboBox.currentIndex == 3)
            userLevelText = "Operator"


        if (userId === "")
        {
            recipe.updateStatusText(qmltextUserNameEmptyMsg)
        }
        else
        {
            if (customButton1.text == qmltextUpdate)
            {
                if (password === "" && confirmPassword === "")
                {
                    password = olduserPassword
                    isUserErrorExist = false
                }
                else
                {
                    passwordCheck(password, confirmPassword)
                }
            }
            else
            {
                passwordCheck(password, confirmPassword)
            }

            if (!isUserErrorExist)
            {
                if (customButton1.text == qmltextSaveButton)
                {
                    checkUserId(userId)
                }

                if (!isUserErrorExist)
                {
                    var message = ""
                    if (customButton1.text == qmltextSaveButton)
                    {
                        message = qmltextNewUserCreatedMsg
                    }
                    else
                    {
                        message = qmltextUpdatedUserMsg
                    }
                    var eventComment = createDialogpopup(message)
                }
            }
        }
    }

    Flickable{
        id: addUserFlick
        width: parent.width
        height: parent.height
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        interactive: Qt.inputMethod.visible ? true : false
        bottomMargin: Qt.inputMethod.visible ? parent.height* (50/100) : 0
        clip: true

        Rectangle
        {
            id:leftPanelRect
            anchors.left: parent.left
            width: parent.width
            height: (columnHeightLeft/windowHeight)*parent.height
            color:"#ECECEC"
            DropShadow{
                anchors.fill: column
                horizontalOffset: 3
                verticalOffset: 3
                color: "#000000"
                opacity: 0.2
                source: column
            }
            Column {
                id: column
                x: parent.width/2-width/2
                y: (itemPosition/columnHeightRight)*parent.height
                width: parent.width
                height: parent.height
                spacing: (columnSpace/columnHeightRight)*height

                SystemConfigMenuItem {
                    x:parent.width/2-width/2

                    id: systemConfigMenuItem1
                    width: (gridElementWidth/columnWidth)*parent.width
                    height: (gridElementHeight/columnHeightLeft)*parent.height

                    LineEdit{
                        id: userIdLineEdit
                        x: 0.5172413793103448*parent.width
                        y: parent.height/2-height/2
                        width: (comboBoxWidth/gridElementWidth)*systemConfigMenuItem3.width
                        height: (comboBoxHeight/gridElementHeight)*systemConfigMenuItem3.height
                        placehodertext: qmltextUserName
                    }

                    Text{
                        x: (7/290)*parent.width; y: 0.2857142857142857*parent.height
                        width: 102.922
                        height: .42*parent.height
                        text:qmltextUserId
                        elide: Text.ElideLeft
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                }

                SystemConfigMenuItem {
                    x:parent.width/2-width/2
                    id: systemConfigMenuItem2
                    width: (gridElementWidth/columnWidth)*parent.width
                    height: (gridElementHeight/columnHeightLeft)*parent.height

                    LineEdit {
                        id: lineEditPassword
                        x: 0.5172413793103448*parent.width
                        y: parent.height/2-height/2
                        width: (comboBoxWidth/gridElementWidth)*systemConfigMenuItem3.width
                        height: (comboBoxHeight/gridElementHeight)*systemConfigMenuItem3.height
                        echoMode: TextInput.Password
                        placehodertext: qmltextEnterPassWord
                        input.onFocusChanged:
                        {
                            var isfocus = input.focus
                            if(isfocus)
                                addUserFlick.contentY = lineEditPassword.y + lineEditPassword.height
                        }
                    }

                    Text{ x: (7/290)*parent.width;
                        y: 0.2857142857142857*parent.height
                        width: .355*parent.width
                        height: .42*parent.height
                        text: qmltextPassWord
                        elide: Text.ElideMiddle
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                }

                SystemConfigMenuItem {
                    x:parent.width/2-width/2
                    id: systemConfigMenuItem44
                    width: (gridElementWidth/columnWidth)*parent.width
                    height: (gridElementHeight/columnHeightLeft)*parent.height

                    LineEdit {
                        id: lineEditConfirmPassword
                        x: 0.5172413793103448*parent.width
                        y: parent.height/2-height/2
                        width: (comboBoxWidth/gridElementWidth)*systemConfigMenuItem3.width
                        height: (comboBoxHeight/gridElementHeight)*systemConfigMenuItem3.height
                        echoMode: TextInput.Password
                        placehodertext: qmltextEnterConfirmPassword
                        input.onFocusChanged:
                        {
                            var isfocus = input.focus
                            if(isfocus)
                                addUserFlick.contentY = lineEditConfirmPassword.y + lineEditConfirmPassword.height
                        }
                    }

                    Text{ x: (7/290)*parent.width;
                        y: 0.2857142857142857*parent.height
                        width: .355*parent.width
                        height: .42*parent.height
                        text: qmltextConfirmPassword
                        elide: Text.ElideMiddle
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                }

                SystemConfigMenuItem {
                    x:parent.width/2-width/2
                    id: systemConfigMenuItem3
                    width: (gridElementWidth/columnWidth)*parent.width
                    height: (gridElementHeight/columnHeightLeft)*parent.height

                    CustomCombobox
                    {
                        id: userListComoboBox
                        x: 0.5172413793103448*parent.width
                        y: parent.height/2-height/2
                        width: (comboBoxWidth/gridElementWidth)*systemConfigMenuItem3.width
                        model: ListModel {
                            id: userLevelModel
                            ListElement { text: qsTr("Executive")     }
                            ListElement { text:  qsTr("Supervisor")    }
                            ListElement { text:  qsTr("Technician")     }
                            ListElement { text: qsTr("Operator")        }
                        }
                    }

                    Text{id:userlevelText
                        x: (7/290)*parent.width;y: 0.2857142857142857*parent.height
                        width: .355*parent.width
                        height: .42*parent.height
                        text: qmltextUserLevel
                        elide: Text.ElideMiddle
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                }

                SystemConfigMenuItem {
                    x:parent.width/2-width/2
                    id: systemConfigMenuItem4
                    width: (gridElementWidth/columnWidth)*parent.width
                    height: (gridElementHeight/columnHeightLeft)*parent.height

                    SystemConfigButton{
                        id: systemConfigButton1
                        x: 0.5172413793103448*parent.width
                        y: parent.height/2-height/2
                        width: Math.sqrt((window.width*32/1920)*(window.height*32/1080))
                        height:  Math.sqrt((window.width*32/1920)*(window.height*32/1080))
                        rightText: qmltextRightText
                        leftText: qmltextleftText
                        checked: false

                        onCheckedChanged:  {
                            isStatusUpdated=checked
                        }
                    }
                    Text{ x: (7/290)*parent.width;y: 0.2857142857142857*parent.height
                        width: .355*parent.width
                        height: .42*parent.height
                        text: qmltextStatus
                        elide: Text.ElideMiddle
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                }

                SystemConfigMenuItem {
                    x:parent.width/2-width/2
                    id: systemConfigMenuItem5
                    width: (gridElementWidth/columnWidth)*parent.width
                    height: (gridElementHeight/columnHeightLeft)*parent.height

                    SystemConfigButton{
                        id: systemConfigButton2
                        x: 0.5172413793103448*parent.width
                        y: parent.height/2-height/2
                        width: Math.sqrt((window.width*32/1920)*(window.height*32/1080))
                        height:  Math.sqrt((window.width*32/1920)*(window.height*32/1080))
                        rightText: qmltextRightLockedText
                        leftText: qmltextLeftUnlockedText
                        checked: false

                        onCheckedChanged:  {
                            isLockUpdated=checked
                        }
                    }
                    Text{ x: (7/290)*parent.width;y: 0.2857142857142857*parent.height
                        width: .355*parent.width
                        height: .42*parent.height
                        text: qmltextLock
                        elide: Text.ElideMiddle
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.regular.name
                        font.pixelSize:  Style.style3
                    }
                }
            }
        }

        Text {
            id: error_text
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: leftPanelRect.bottom
            anchors.topMargin: 15
            color: "#F03E3E"
            text: ""
            wrapMode: Text.WordWrap
            font.family: Style.regular.name
            font.pixelSize:    Style.style1
            visible: isUserErrorExist? true: false
        }

        RowLayout {
            spacing: 20
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: leftPanelRect.bottom
            anchors.topMargin: 20

            SecondaryButton {
                id: customButton
                text: qmltextCancelButton
                onReleased:
                {
                    userListLayout.currentIndex = 0
                    tabButtonText = qmltexttabButtonText
                }
            }
            PrimaryButton {
                id: customButton1
                text:qmltextSaveButton
                onReleased:
                {
                    onSave()
                }
            }
        }
    }
}
