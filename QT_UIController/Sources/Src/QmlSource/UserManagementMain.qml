import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import Style 1.0
Item
{
    readonly property string qmltextUserList: qsTr("USER LIST")
    readonly property int indexUserList: 0

    id:userMgr

    property alias addUserId: addUserItem
    property alias userlistqml: userlistId
    property alias barDefaultIndex: bar.currentIndex
    property alias tabButtonText: barText.text

    /**
      *@breif: To highlight the first row in the User Management
      **/
    function userlisthighlightfirstrow()
    {
        userlistId.highlightfirstrow()
    }

    /**
      *@breif: To set User Management key value
      **/
    function setUserMngKeyValue( keyboardInput )
    {
        addUserItem.setUserMngKeyValue(keyboardInput)
    }
    Rectangle
    {
        id: bar
        width: parent.width
        anchors.left: parent.left
        height:50
        property int currentIndex: 0
        anchors.top: parent.top
        anchors.topMargin: 15
        Text {
            id: barText
            text:qmltextUserList
            opacity: enabled ? 1.0 : 1
            color:  bar.currentIndex===0 ? qmlPrimaryColor : "#757575"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
            font.family: Style.semibold.name
            font.pixelSize: Style.style4
        }

        Rectangle {
            height: 2
            width: parent.width
            anchors.bottom:  bar.bottom
            anchors.bottomMargin: 10
            color: qmlPrimaryColor
        }
    }

    StackLayout {
        id: layout
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: bar.bottom
        anchors.topMargin: 0
        currentIndex: bar.currentIndex

        Item
        {
            StackLayout {
                id: userListLayout
                width: parent.width
                currentIndex: 0

                onCurrentIndexChanged: {
                    if (indexUserList == userListLayout.currentIndex)
                        login.UserOnModifyScreen = ""
                }

                UserList{
                    id: userlistId
                    height:(995/1080)*window.height
                    width:window.width*(1500/1920)
                }

                AddUser
                {
                    id: addUserItem
                    width:window.width*(1500/1920)
                    height:(995/1080)*window.height
                }
            }
        }
    }
}
