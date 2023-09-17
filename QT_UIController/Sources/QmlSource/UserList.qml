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
    //User Levels
    readonly property string qmltextUserLevelExecutive :  qsTr("Executive")
    readonly property string qmltextUserLevelSupervisor : qsTr("Supervisor")
    readonly property string qmltextUserLevelOperator :   qsTr("Operator")
    readonly property string qmltextUserLevelTechnician : qsTr("Technician")

    readonly property string qmltextUserIdRole : qsTr("userId")
    readonly property string qmltextUserIdTitle : qsTr("User Id")
    readonly property string qmltextUserLevelRole : qsTr("userLevel")
    readonly property string qmltextUserLevelTitle : qsTr("User Level")
    readonly property string qmltextUserStatusRole : qsTr("userStatus")
    readonly property string qmltextUserStatusTitle : qsTr("User Status")
    readonly property string qmltextLastModified : qsTr("lastModified")
    readonly property string qmltextDateTme: qsTr("Date & Time")
    readonly property string qmltextAddUser : qsTr("ADD USER")
    readonly property string qmltextModifyUser : qsTr("MODIFY USER")
    readonly property string qmlTextUserLimitCrossed: qsTr("User profile creation limit crossed. Manage user profiles with 'MODIFY USER' option to change the password or User level.")
    readonly property string qmltextWebServicesLoggedIn :qsTr("The user you are trying to modify is logged into the Web Services, please logout from the Web Services before accessing this screen")

    property alias userTableViewId: userTableView

    function addUserJs()
    {
        addUserId.clearText()
        userListLayout.currentIndex = 1
    }

    function modifyUserJs()
    {
        var selectedRowIndex = userTableView.currentRow
        var userId = userModel.get(selectedRowIndex).userId
        var userLevel =  userModel.get(selectedRowIndex).userLevel.trim()
        var userStatus =  userModel.get(selectedRowIndex).userStatus
        var userPassword =  userModel.get(selectedRowIndex).userPassword
        var userLastChangedPassword =  userModel.get(selectedRowIndex).lastChangedPassword
        var userlocked = userModel.get(selectedRowIndex).userLock

        var userLevelIndex = 0
        if (userLevel === "Executive")
            userLevelIndex = 0
        else if (userLevel === "Supervisor")
            userLevelIndex = 1
        else if (userLevel === "Technician")
            userLevelIndex = 2
        else if (userLevel === "Operator")
            userLevelIndex = 3

        addUserId.modifyUserData(selectedRowIndex, userId, userPassword, userLevelIndex, userStatus, userLastChangedPassword, userlocked)
        userListLayout.currentIndex = 1
    }

    function highlightfirstrow()
    {
        if(userModel.rowCount() > 0)
        {
            userTableView.selection.clear()
            userTableView.currentRow = 0
            userTableView.selection.select(0)
        }
    }

    Rectangle {
        id: userListViewRectangle
        width: parent.width
        height: (700/1080)*parent.height
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 0
        color: "#ffffff"
        border.color: "#989898"
        clip: true

        TableView {
            id: userTableView
            width: parent.width
            height: parent.height
            anchors.rightMargin: 1
            anchors.fill: parent
            anchors.topMargin: 1
            activeFocusOnTab: true
            frameVisible: true
            backgroundVisible: true
            currentRow: -1
            headerVisible: true

            TableViewColumn {
                role: "userId"
                title: qsTr("User Id")
                width: 150
            }
            TableViewColumn {
                role: "userLevel"
                title: qsTr("User Level")
                width: 150
            }
            TableViewColumn {
                role: "userStatus"
                title: qsTr("User Status")
                width: 180
            }
            TableViewColumn {
                role: "userLock"
                title: qsTr("Is Locked User")
                width: 180
            }

            TableViewColumn {
                role: "lastModified"
                title: qsTr("Date & Time")
                width: 200
            }
            model: userModel

            style: TableViewStyle {
                headerDelegate: Rectangle {
                    height: 50
                    width: textItem.implicitWidth
                    color: "#ECECEC"
                    Text {
                        id: textItem
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: styleData.value
                        elide: Text.ElideRight
                        color: "#757575"
                        font.family: Style.semibold.name
                        font.pixelSize:  Style.style4
                        renderType: Text.NativeRendering
                    }
                    Rectangle {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 1
                        anchors.topMargin: 1
                        width: 1
                        color: "#ccc"
                    }
                }

                rowDelegate: Item {
                    height: 40

                    Rectangle
                    {
                        id: rectangle1
                        color: styleData.selected ? qmlPrimaryColor : "#FFFFFF"
                        anchors.fill: parent
                        border.width: 0.5
                        border.color: "#E9ECEF"
                    }
                }

                itemDelegate: Item
                {
                    height: 40
                    Text
                    {
                        id: folderText1
                        text: styleData.value
                        fontSizeMode: Text.Fit
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: Style.semibold.name
                        font.pixelSize:  Style.style2
                        color: styleData.selected ? "#FFFFFF" :"#898D96"
                    }
                }
            }
        }
    }
    RowLayout {
        spacing: 20
        anchors.right: parent.right
        anchors.top:userListViewRectangle.bottom
        anchors.topMargin: 20

        PrimaryButton {
            id: addUserButton
            text: qmltextAddUser
            onClicked:
            {
                if(userModel.rowCount() < 1000)
                {
                    tabButtonText =qmltextAddUser
                    addUserJs()
                }
                else
                {
                    messagePopup.open(messagePopup.warning_title ,qmlTextUserLimitCrossed, messagePopup.without_button)
                }
            }
        }

        PrimaryButton {
            id: modifyUserButton
            text: qmltextModifyUser
            onClicked: {
                if(login.WSLoggedUserName !== userModel.get(userTableView.currentRow).userId)
                {
                    tabButtonText = qmltextModifyUser
                    modifyUserJs()
                }
                else
                {
                    messagePopup.open(messagePopup.warning_title,qmltextWebServicesLoggedIn,messagePopup.without_button)
                }
            }
        }
    }
}
