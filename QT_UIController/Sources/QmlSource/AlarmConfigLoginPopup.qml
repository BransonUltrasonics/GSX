import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import Style 1.0
import QtQuick.Window 2.2
Rectangle {
    property alias alarmconfigpopup: alarmconfigpopup
    readonly property string qmltextFailAlarmMsg:qsTr("Failed to log Alarm Configuration, please try again")
    readonly property string qmltextAlarmConfig:qsTr("Alarm Configuration Login")
    readonly property string qmltextPasswrd:qsTr("Password")
    readonly property string qmltextEnterPasswrd:qsTr("Enter Password")
    readonly property string qmltextCancel:qsTr("CANCEL")
    readonly property string qmltextOk:qsTr("OK")

    id:alarmconfigpopup
    visible: false
    width: Window.width
    height: Window.height
    x:0
    y:-100
    color: "#80000000"
    MouseArea{
        anchors.fill: parent
        onClicked: {
        }
    }
    function okAction()
    {
        if(passwordLineEdit.text == "123")
        {
            disableMainwindowOpacity.start()
            alarmconfigpopup.close()
            alarmConfigurationvisibility = true
            alarmconfig.readAlarmconfigDetails()
        }
        else
        {
            messagePopup.open(messagePopup.info_title,qmltextFailAlarmMsg, messagePopup.ok_and_cancel_button)
            messagePopup.onOkay.connect(function onOkay(){
                messagePopup.close()
                passwordLineEdit.text = ""
            })
            messagePopup.onCancel.connect(function onCancel(){
                messagePopup.close()
                alarmconfigpopup.close()
                alarmconfigloginstatus = true
                passwordLineEdit.text = ""

            })
        }
    }
    function close() {
        alarmconfigpopup.visible = false
    }
    function open() {
        alarmconfigpopup.visible = true
    }

    function cancelAction()
    {
        alarmconfig.visible = false
        alarmconfigloginstatus = true
        alarmconfigpopup.close()
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
            height:window.height/4.5
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
                width:  alarmconfigpopup.width
                height: 30
                anchors.bottom : toprec.bottom
                anchors.left : toprec.left
                anchors.right : toprec.right
                labelName: qmltextAlarmConfig
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
                    onClicked: {
                        passwordLineEdit.text = ""
                        alarmconfigpopup.close()
                        disableMainwindowOpacity.start()
                        alarmconfigloginstatus = true
                    }
                    onEntered: {
                        cursorShape=Qt.PointingHandCursor
                    }
                    z:1
                }
            }
            Rectangle{
                id:passwordlabel
                width: parent.width
                height: 70
                anchors.top : labelBarId1.bottom
                anchors.topMargin: 10
                Text {
                    id: text1
                    text: qmltextPasswrd+":"
                    width: parent.width/2.5
                    anchors.left: parent.left
                    anchors.leftMargin: 60
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Style.style6
                }
                LineEdit{
                    id: passwordLineEdit
                    width: passwordlabel.width*(1/3)
                    height: 40
                    echoMode:TextInput.Password
                    placehodertext:qmltextEnterPasswrd
                    anchors.left: text1.right
                    anchors.leftMargin: 20
                    focus: false
                    anchors.verticalCenter: parent.verticalCenter
                    input.onFocusChanged:
                    {
                        var isfocus = input.focus
                        if(isfocus)
                            changePasswordFlick.contentY = passwordLineEdit.y + 75
                    }
                }
            }
            RowLayout{
                spacing: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                SecondaryButton{
                    id:okbutton
                    text: qmltextCancel
                    onClicked:
                    {
                        passwordLineEdit.text = ""
                        cancelAction()
                    }
                }
                PrimaryButton{
                    id:cancelbutton
                    text:qmltextOk
                    onClicked:
                    {
                        okAction()
                    }
                }
            }
        }
    }
}

