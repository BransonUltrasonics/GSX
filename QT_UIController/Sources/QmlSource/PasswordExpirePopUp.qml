import QtQuick 2.0
import QtQuick.Controls 2.1
import Style 1.0
import com.branson.menu 1.0

Rectangle {
    readonly property string qmltextHeading :  qsTr("Your password has expired please change your password")
    readonly property string qmltextHeadingForFirstTimeUser:qsTr("First time login. Please reset your User login password")
    id: passwordPopup
    width: parent.width
    height: parent.height
    visible: true
    color: "#ECECEC"    
    Rectangle
    {
        color: qmlPrimaryColor
        width: parent.width
        height: 40
        Text {
            id: title_change_password           
            color: "#FFFFFF"
            text:reqLoginExpriyPwd?qmltextHeading: qmltextHeadingForFirstTimeUser
            anchors.centerIn: parent
            font.pixelSize: Style.style6
            font.family:Style.semibold.name
        }
    }
    Rectangle
    {
        x: 50
        anchors.top:parent.top
        anchors.topMargin: 40
        width: parent.width-100
        height: parent.height-100
        ChangePassword {
            id:expirechangepassword
            z:1
            anchors.fill: parent
        }
    }
}
