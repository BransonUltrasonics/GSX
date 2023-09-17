import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Style 1.0
import com.branson.runtimefeatures 1.0

Item {
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#757575"
    property string unpressedTabBkColor: "#FFFFFF"
    property string tabTextColor: "#757575"
    property string tabTextBkColor: "#FFFFFF"
    property int recipeIndex1: recipeIndex1

    readonly property string qmltextPowersupplyIo :qsTr("Power Supply I/O")
    readonly property string qmltextActuatorIo :qsTr("Actuator I/O")
    property bool runTimeConfigActuator: config.getFeatureFlagStatus(RuntimeFeatures.UI_ACTUATOR_IO)

    TabBar {
        id: tabbar
        width: parent.width
        anchors.left: parent.left
        height:50
        currentIndex: 0
        anchors.top: parent.top

        TabButton {
            height: parent.height
            contentItem: Text {
                text:qmltextPowersupplyIo
                opacity: enabled ? 1.0 : 1
                color:  tabbar.currentIndex==0 ? qmlPrimaryColor : "#757575"
                anchors.leftMargin:(100/480)*parent.width
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                font.family: Style.semibold.name
                font.pixelSize: Style.style4
            }
            background: Rectangle {
                implicitWidth: (130/130)*parent.width
                implicitHeight: (40/40)*parent.height
                opacity: enabled ? 1 : 1
                color:"#FFFFFF"
                border.width: 0
                radius: 0
                Rectangle{
                    x:(10/130)*parent.width
                    y:parent.height-(2/40)*parent.height
                    width:parent.width/2
                    height:2
                    color:tabbar.currentIndex==0?qmlPrimaryColor:"#00ffffff"
                }
            }
        }

        TabButton {
            height: parent.height
            visible: runTimeConfigActuator
            contentItem: Text {
                text: qmltextActuatorIo
                opacity: enabled ? 1.0 : 1
                color:  tabbar.currentIndex==1 ? qmlPrimaryColor : "#757575"
                anchors.leftMargin:(100/480)*parent.width
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
                    x:(10/130)*parent.width
                    width: parent.width/2
                    height:2
                    color:tabbar.currentIndex==1?qmlPrimaryColor:"#00ffffff"
                }
            }
        }

        onCurrentIndexChanged:
        {
            if(currentIndex==0)
            {
                userIOPSObj.refreshModel()
            }
            if(currentIndex==1)
            {
                userIOACObj.refreshModel()
            }
        }
    }

    StackLayout
    {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: tabbar.currentIndex
        x:0
        Item
        {
            id: tab0

            UserIOPowerSupply
            {
                x:0
                y: tabbar.y+tabbar.height
                width:window.width*(1500/1920)
                height:(890/1080)*window.height-40

            }
        }
        Item
        {
            id: tab1
            UserIOActuator
            {
                x:0
                y:tabbar.y+tabbar.height
                width:window.width*(1500/1920)
                height:(890/1080)*window.height-40
            }
        }

    }
}
