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
    readonly property string qmltextSystemConfig :qsTr("SYSTEM CONFIGURATION")
    readonly property string qmltextFeatures :qsTr("FEATURES")
    readonly property string qmltextService: qsTr("SERVICE")
    id: bransontabs

    Component.onCompleted:
    {
        if(sysconfig.bransonSecondLevelFlag)
        {
            sysconfig.createBransonSystemConfigList()
        }
    }

    TabBar {
        id: tabbar
        width: parent.width
        anchors.left: parent.left
        height:50
        currentIndex: 0
        anchors.top: parent.top
        font.family: Style.semibold.name
        font.pixelSize: Style.style2
        Layout.fillWidth: true

        onCurrentIndexChanged: {

            if(currentIndex == 2)
            {
                sysconfig.createServiceRightModel()
            }
        }

        TabButton
        {
            id:tabbtn0
            height: parent.height
            hoverEnabled: true
            contentItem: Text
            {
                id:textWeldMode
                text:qmltextSystemConfig
                font: tabbar.font
                opacity: 1
                color: tabbar.currentIndex===0 ? qmlPrimaryColor : "#757575"
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height/*parent.height*/
                opacity: enabled ? 1 : 1
                color:"#FFFFFF"
                border.width: 0
                radius: 0
                Rectangle{
                    y:parent.height-(2/40)*parent.height
                    width:parent.width
                    height:2
                    color:tabbar.currentIndex===0?qmlPrimaryColor:"#00ffffff"
                }
            }

        }
        TabButton {
            id:tabbtn1
            height: parent.height
            hoverEnabled: true
            contentItem: Text
            {
                id:textWeldProcess
                text:qmltextFeatures
                font: tabbar.font
                color: tabbar.currentIndex===1 ? qmlPrimaryColor : "#757575"
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                opacity: 1
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height/*parent.height*/
                opacity: enabled ? 1 : 1
                color:"#FFFFFF"
                border.width: 0
                radius: 0
                Rectangle{
                    y:parent.height-(2/40)*parent.height
                    width:parent.width
                    height:2
                    color:tabbar.currentIndex===1?qmlPrimaryColor:"#00ffffff"
                }
            }

        }
        TabButton {
            id:tabbtn2
            height: parent.height
            hoverEnabled: true
            contentItem: Text
            {
                id:textParamAZ
                text:qmltextService
                font: tabbar.font
                color:  tabbar.currentIndex===2 ? qmlPrimaryColor : "#757575"
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                opacity: 1
            }

            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height/*parent.height*/
                opacity: enabled ? 1 : 1
                color:"#FFFFFF"
                border.width: 0
                radius: 0
                Rectangle{
                    y:parent.height-(2/40)*parent.height
                    width:parent.width
                    height:2
                    color:tabbar.currentIndex===2?qmlPrimaryColor:"#00ffffff"
                }
            }
        }
    }

    StackLayout {
        id: layout
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: tabbar.bottom
        anchors.topMargin: 0
        currentIndex: tabbar.currentIndex

        BransonSystemMenu{
            id:systremMenu
            x:(420/1920)*window.width
            width:window.width*(1500/1920)
            height:500
        }
        Features
        {
            id:featuresMenu
            x:(420/1920)*window.width
            width:window.width*(1500/1920)
            height:500
        }
        Service
        {
            id:serviceTab
            x:(420/1920)*window.width
            width:window.width*(1500/1920)
            height:500
        }
    }
}
