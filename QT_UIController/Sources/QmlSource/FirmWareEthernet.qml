import QtQuick 2.0
import Style 1.0
import QtQuick.Layouts 1.3

Rectangle{
    readonly property string qmltextSoftwareUpgradeConfirmation: qsTr("New software is ready to be installed. Do you want to upgrade?")
    readonly property string qmlTextUpgradeNow: qsTr("UPGRADE NOW")
    readonly property string qmlTextUpgradeLater: qsTr("UPGRADE LATER")
    readonly property string qmltextFIRMWARE_UPGRADE_TITLE : qsTr("SOFTWARE UPGRADE")

    id: upgradePopup
    width: parent.width
    height: parent.height
    color: "#80000000"
    visible: false
    readonly property string bgColor: qmlPrimaryColor
    readonly property string titleColor: "#FFFFFF"

    signal upgradeNow()
    signal upgradeLater()
    MouseArea{
        id: upgradePopupMa
        anchors.fill: parent
    }

    /**
    *@breif: Close FirmWareEthernet popup
    **/
    function close()
    {
        upgradePopup.visible = false
    }

    /**
    *@breif: Open FirmWareEthernet popup
    **/
    function open()
    {
        upgradePopup.visible = true
    }


    Rectangle{
        id: dialog
        width:  parent.width/2
        height: parent.height/5
        anchors.centerIn: parent
        color: titleColor
        radius: 3
        Rectangle{
            id:id_firmwareupgrade_title
            width: parent.width
            anchors.left: parent.Left
            anchors.top: parent.Top
            height: 40
            color: qmlPrimaryColor

            Text{
                id: dialogDesc
                text: qmltextFIRMWARE_UPGRADE_TITLE
                font.family: Style.semibold.name
                font.pixelSize:  Style.style3
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color:"#FFFFFF"
            }
        }
        Text{
            id: contentMesg
            text: qmltextSoftwareUpgradeConfirmation
            font.family: Style.semibold.name
            font.pixelSize:  Style.style3
            anchors.top:id_firmwareupgrade_title.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            color:"#868e96"
        }
        RowLayout
        {
            id:buttonsRow
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 30
            spacing: 30
            PrimaryButton {
                id:popupPrimButtonId
                text: qmlTextUpgradeNow
                visible: true
                onClicked: {
                    upgradeNow()
                }
            }
            PrimaryButton {
                id:popupPrimButton2Id
                text: qmlTextUpgradeLater
                visible: true
                onClicked: {
                    upgradeLater()
                }
            }
        }
    }
}
