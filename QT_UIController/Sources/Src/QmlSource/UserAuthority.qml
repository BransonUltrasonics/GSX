import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import Style 1.0
import com.branson.helperenums 1.0
import com.branson.WSrestrictionscreen 1.0

Item
{
    readonly property string qmltextOperatorAuthorityOption :qsTr("OPERATORS AUTHORITY OPTIONS")
    readonly property string qmltextGlobalUserSetting :qsTr("GLOBAL USER SETTINGS")
    readonly property string qmltextWebServicesLoggedIn :qsTr("Please logout from the Web Services before accessing this screen")
    id: userAuthority

    property alias barDefaultIndex: bar.currentIndex


    TabBar {
        id: bar
        width: parent.width
        anchors.left: parent.left
        height:50
        currentIndex: 0
        anchors.top: parent.top

        TabButton {
            height: parent.height
            contentItem: Text {
                text:qmltextOperatorAuthorityOption
                opacity: enabled ? 1.0 : 1
                color:  bar.currentIndex===0 ? qmlPrimaryColor : "#757575"
                anchors.leftMargin: (160/480)*parent.width
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height/*parent.height*/
                opacity: enabled ? 1 : 1
                color:"#FFFFFF"
                border.width: 0
                radius: 0
                Rectangle{
                    x:(20/130)*parent.width
                    y:parent.height-(2/40)*parent.height
                    width:parent.width-x
                    height:2
                    color:bar.currentIndex===0?qmlPrimaryColor:"#00ffffff"
                }
            }
        }

        TabButton {
            height: parent.height
            contentItem: Text {
                text: qmltextGlobalUserSetting
                opacity: enabled ? 1.0 : 1
                color:  bar.currentIndex===1 ? qmlPrimaryColor : "#757575"
                anchors.leftMargin: (160/480)*parent.width
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent,width
                implicitHeight: (40/40)*parent.height
                opacity: enabled ? 1 : 1
                color:"#FFFFFF"
                border.width: 0
                radius: 0
                Rectangle{

                    y:parent.height-(2/40)*parent.height
                    width: parent.width-(20/130)*parent.width
                    height:2
                    color:bar.currentIndex===1?qmlPrimaryColor:"#00ffffff"
                }
            }
        }

        onCurrentIndexChanged: {

            if (indexGlobalUserSettings === bar.currentIndex)
            {
                globalSettings.globalSettingsInit()
                login.setWSRestrictedScreens(WSRestrictionScreen.NO_RESTRICTED)
            }
            else if(indexOperatorAuthorityOptions === bar.currentIndex)
            {
                login.setWSRestrictedScreens(WSRestrictionScreen.USER_OPERATOR_AUTHORITY)

                if(HelperEnums.OPERATOR == login.WSUserLevelStatus)
                {
                    messagePopup.open(messagePopup.warning_title,qmltextWebServicesLoggedIn,messagePopup.without_button)
                    setCurrentIndex(indexGlobalUserSettings)
                }
            }
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

        AuthorityOptionList{
            id:authorityOPtion
            height:1080
        }

        GlobalSettings{
            id: globalSettings
            height:1080
        }
    }
}
