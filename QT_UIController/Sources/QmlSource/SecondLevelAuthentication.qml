import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import Style 1.0
import QtQuick.Window 2.2
Rectangle {
    property alias secondLevelAuthenticationPopup: secondLevelAuthenticationPopup
    property alias passwordLineEditText: passwordLineEdit.text
    property alias usernameLineEditText: usernameLineEdit.text
    readonly property string qmltextFailAlarmMsg:qsTr("Failed to login Alarm Configuration, please try again")
    readonly property string qmltextSecondLevelAuth:qsTr("Second Level Authentication")
    readonly property string qmltextPasswrd:qsTr("Password")
     readonly property string qmltextEnterUsername:qsTr("Enter Username")
    readonly property string qmltextEnterPasswrd:qsTr("Enter Password")
    readonly property string qmltextCancel:qsTr("CANCEL")
    readonly property string qmltextOk:qsTr("CONFIRM")

    id:secondLevelAuthenticationPopup
    visible: false
    width: Window.width
    height: Window.height
    x:0
    y:0//-100
    color: "#80000000"
    signal onOkay()
    signal onCancel()

    MouseArea{
        anchors.fill: parent
        onClicked: {
        }
    }

    /**
       *@breif: To close the pop up
       **/
    function close() {
        secondLevelAuthenticationPopup.visible = false
    }

    /**
       *@breif: To open the popup
       **/
    function open() {
        secondLevelAuthenticationPopup.visible = true
    }

    /**
       *@breif:OnClicking Cancel button in the popup
       **/
    function cancelAction()
    {
        secondLevelAuthenticationPopup.close()
        disableMainwindowOpacity.start()
    }
    Flickable{
        id: changePasswordFlick
        width: parent.width
        height: parent.height
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        interactive: Qt.inputMethod.visible ? true : false
        bottomMargin: Qt.inputMethod.visible ? parent.height* (15/100) : 0
        clip: true
        Rectangle{
            id:mainRec
            width:window.width/3.3
            height:window.height/3.5
            anchors.centerIn: parent
            color: "#ffffff"
            border.color: "#ffffff"
            Rectangle {
                id: toprec
                width:  mainRec.width
                height: 27
                radius: 3
                anchors.top:parent.top
                anchors.topMargin: -3
                color: qmlPrimaryColor
            }
            LabelBars{
                id:labelBarId1
                width:  secondLevelAuthenticationPopup.width
                height: 30
                anchors.bottom : toprec.bottom
                anchors.left : toprec.left
                anchors.right : toprec.right
                labelName: qmltextSecondLevelAuth
                Image {
                    id: image
                    sourceSize.width: 30
                    sourceSize.height: 30
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    /*Image icon update based on System Type*/
                    source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/close_ASX.svg" :
                                                                   "qrc:/images/image/close.svg"
                }
                MouseArea {
                    id: mouseArea
                    width: 40
                    height: 40
                    x: image.x
                    y: image.y
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    onClicked:
                    {
                        passwordLineEdit.text = qmlTextEmpty
                        secondLevelAuthenticationPopup.close()
                        disableMainwindowOpacity.start()
                        onCancel()
                    }
                    onEntered:
                    {
                        cursorShape=Qt.PointingHandCursor
                    }
                    z:1
                }
            }
            Rectangle
            {
                id:usernamelabel
                width: parent.width
                height: 70
                anchors.top : labelBarId1.bottom
                anchors.topMargin: 10
                LineEdit{
                    id: usernameLineEdit
                    width: usernamelabel.width*(1/2)
                    height: 40
                    placehodertext:qmltextEnterUsername
                    focus: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    input.onFocusChanged:
                    {
                        var isfocus = input.focus
                        if(isfocus)
                            changePasswordFlick.contentY = usernameLineEdit.y + 75
                    }
                }
            }
            Rectangle
            {
                id:passwordlabel
                width: parent.width
                height: 70
                anchors.top : usernamelabel.bottom
                anchors.topMargin: 5
                LineEdit{
                    id: passwordLineEdit
                    width: passwordlabel.width*(1/2)
                    height: 40
                    echoMode:TextInput.Password
                    placehodertext:qmltextEnterPasswrd
                    focus: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    input.onFocusChanged:
                    {
                        var isfocus = input.focus
                        if(isfocus)
                            changePasswordFlick.contentY = passwordLineEdit.y + 75
                    }
                }
            }
            RowLayout
            {
                spacing: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                SecondaryButton
                {
                    id:cancelButton
                    text: qmltextCancel
                    onClicked:
                    {
                        passwordLineEdit.text = qmlTextEmpty
                        cancelAction()
                        onCancel()
                    }
                }
                PrimaryButton
                {
                    id:okButton
                    text:qmltextOk
                    onClicked:
                    {
                        onOkay()
                        close()
                    }
                }
            }
        }
    }
}

