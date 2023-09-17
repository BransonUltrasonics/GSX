import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Style 1.0
Item {
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#757575"
    property string unpressedTabBkColor: "#FFFFFF"
    property string tabTextColor: "#757575"
    property string tabTextBkColor: "#FFFFFF"
    property int recipeIndex1: recipeIndex1
	Component.onDestruction:
    {
		horndownObj.startOrStopWeld(1)
    }

    TabBar
    {
        id: tabbar
        width: parent.width
        height: (49/780)*parent.height
        Layout.fillWidth: true
        spacing: 0
//        anchors.left: grid.right
//        anchors.leftMargin: 40
        font.family: Style.semibold.name
        font.pixelSize: Style.style2
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
        TabButton
        {
            id:tabbtn0
            height: parent.height
            hoverEnabled: true
            width: 200

            contentItem: Text
            {
                id:textSC
                text:qsTr("Power Supply I/O")
                font: tabbar.font
                opacity: 1
                color: unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                opacity: 1
                id:textBkGdSC
                x:0
                y:textSC.height*(38/40)+8
                implicitWidth: parent.width
                implicitHeight: 2
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(0)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }
        TabButton {
            id:tabbtn1
            width: 200
            height: parent.height
            contentItem: Text
            {
                id:textAC
                text:qsTr("Actuator I/O")
                font: tabbar.font
                color:  unpressedColor
                horizontalAlignment: Text.Center
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                visible: true
                opacity: 1
            }
            background: Rectangle
            {
                opacity: 1
                color:unpressedTabBkColor
            }
            Rectangle
            {
                id: textBkGdAC
                y:textAC.height*(38/40)+8
                implicitWidth:parent.width
                implicitHeight:2
                opacity: 1
            }
            onHoveredChanged:
            {
                if(hovered)
                {
                    changeTabElementColors(1)
                }
                else
                {
                    changeTabElementColors(tabbar.currentIndex)
                }
            }
        }




    }
    function changeTabElementColors(index)
    {
        if(index==0 || tabbar.currentIndex==0)
        {
            textSC.color = pressedColor
            textBkGdSC.color = pressedColor
        }
        else
        {
            textSC.color = unpressedColor
            textBkGdSC.color = unpressedTabBkColor
        }

        if(index==1 || tabbar.currentIndex==1)
        {
            textAC.color = pressedColor
            textBkGdAC.color = pressedColor
        }
        else
        {
            textAC.color = unpressedColor
            textBkGdAC.color = unpressedTabBkColor
        }


    }
    Component.onCompleted:
    {
        tabbar.currentIndex=0
        textSC.color=pressedColor
        textBkGdSC.color = pressedColor

		horndownObj.startOrStopWeld(0)
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
            IOSC
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
            UserIOAC
            {
                x:0
                y:tabbar.y+tabbar.height
                width:window.width*(1500/1920)
                height:(890/1080)*window.height-40
            }
        }

    }
}
