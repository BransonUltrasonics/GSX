import QtQuick 2.5
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3
import Style 1.0

Item {
    readonly property string qmltextSave :  qsTr("SAVE")
    readonly property string qmltextSaveSuccess :  qsTr("Authority options saved successfully")

    id: item1
    property var windowHeight :1080
    property var windowWidth :1920
    property var columnSpace :25
    property var itemPosition: 20
    property var columnHeightLeft: 700
    property var columnHeightRight: 650
    property var gridElementHeight:100
    property var gridElementWidth:870
    property var columnWidth:870
    property var linePositionY: 0
    property real vinternalRectheight:1080-85
    property var comboBoxWidth: 286
    property var comboBoxHeight: 50
    property var sysconfigMainWindowPositionX : 20

    property var authorityOptionUpdateListId: []
    property var authorityOptionUpdateListStatus: []

    /**
    *@breif: Hide all controls
    **/
    function checkBoxStatusUpdate(permissionId, name,Title, status)
    {
        authorityModel.updateAuthority(permissionId, name,Title, status)
    }

    /**
    *@breif: Save authority option list and send authority status to DB
    **/
    function onSave()
    {
        authorityModel.sendAuthorityStatusToDB()
        userPrivilageObj.init(window.userLevelId)
        recipe.updateStatusText(qmltextSaveSuccess)

        customButton1.buttonColor = qmlPrimaryColor
        customButton1.textColor = Style.whiteColor
        customButton1.enabled = true
    }

    Rectangle{
        id: authorityLog
        x: (sysconfigMainWindowPositionX/windowWidth)*parent.width
        width: parent.width
        height: (columnHeightLeft/windowHeight)*parent.height
        color:"#ECECEC"
        clip: true

        Rectangle {
            id: authorityLogRec1
            width: parent.width*(95/100)
            height: parent.height*(95/100)
            anchors.left: parent.left
            anchors.leftMargin: parent.width*(2/100)
            anchors.right: parent.right
            anchors.rightMargin: parent.width*(2/100)
            anchors.top: parent.top
            anchors.topMargin: parent.height*(4/100)
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height*(4/100)
            color: "#F8F9FA"
            border.color: "#F8F9FA"
            clip: true

            ListView {
                id: authoritylistView1
                width: parent.width
                height: parent.height
                spacing: 2
                model: authorityModel
                delegate: SystemConfigMenuItem {
                    width: parent.width - 10
                    height: authoritylistView1.height*(1.8/10)

                     CustomCheckBox{
                         id: checkBoxAuthorityOption
                         x: (7/290)*parent.width;y: 0.2857142857142857*parent.height
                         width: .355*parent.width
                         height: .42*parent.height
                         checkBoxName: Title
                         checkBoxStatus: grant
                         onCheckBoxStatusChanged: {
                             checkBoxStatusUpdate(permissionId, module, Title, checkBoxAuthorityOption.checkBoxStatus)
                         }
                     }
                 }


                ScrollBar.vertical: ScrollBar{}
            }

            Component{
                id:authorityList
                SystemConfigMenuItem {
                    width: (gridElementWidth/columnWidth)*parent.width
                    height: (gridElementHeight/columnHeightRight)*parent.height

                    CustomCheckBox{
                        id: checkBoxAuthorityOption
                        x: (7/290)*parent.width;y: 0.2857142857142857*parent.height
                        width: .355*parent.width
                        height: .42*parent.height
                        checkBoxName: Title
                        checkBoxStatus: grant
                        onCheckBoxStatusChanged: {
                            checkBoxStatusUpdate(permissionId, module, Title, checkBoxAuthorityOption.checkBoxStatus)
                        }
                    }
                }
            }
        }
    }

    PrimaryButton {
            id: customButton1
            text:qmltextSave
            anchors.top: authorityLog.bottom
            anchors.topMargin: 40
            anchors.rightMargin: parent.width/2
            spacing: 30
            anchors.right: parent.right
            onClicked: {
                customButton1.enabled = false
                Qt.callLater(onSave)
            }

            onPressed: {
                customButton1.buttonColor = Style.grayColor
                customButton1.textColor =  Style.blackColor
            }
        }
}
