import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQml.Models 2.2
Item {
    id: limitsMenuItem
    property real gridHeight:870
    property real rectWidth:480.6
    property real parentwidth:1510
    property real limitsmenupressureX: 0
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#ECECEC"
    Component.onCompleted:
    {
    }
    Rectangle {
        id: flickable
        anchors.fill: parent
        color: "#F8F9FA"
        ListView {
            id: grid
            width: parent.width-50
            height: parent.height-50
            currentIndex:0
            clip: true
            spacing: 1
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            onCurrentItemChanged:
            {
                //grid.itemAt(currentItem.x,currentItem.y).paramColor="#000000"
            }
            model: modelLimitsMenu
            delegate:LimitMenuItem
            {
                id:tab_grid
                paramText:model.modelData.ParameterName
                paramLocaleText: model.modelData.ParameterLocaleName
                height: grid.height/10
                width: grid.width/4
                tab_griditem_mousearea.onClicked:
                {
                    grid.currentIndex=index
                }
                Connections
                {
                    target:grid
                    onCurrentItemChanged:
                    {
                        switchColor()
                    }
                }

                /**
                *@breif: To Switch color based on the index
                **/
                function switchColor()
                {
                    if(index===grid.currentIndex)
                    {
                        paramTextColor="#ffffff"
                        paramColor=pressedColor
                    }
                    if(index!==grid.currentIndex)
                    {
                        paramTextColor="#000000"
                        paramColor=unpressedColor
                    }
                }
            }
            Component.onCompleted:
            {
                grid.currentIndex = 0
            }
        }
    }
    StackLayout {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: grid.currentIndex
        Item {
            id: tab0
            LimitsSetup
            {
//                x:(grid.width-(20/parentwidth)*grid.width)/4 +(20/1920)*window.width
//                y: (20/780)*window.height
//                width:window.width*(1350/1920)-x
//                height:(870/780)*window.height
                x: grid.width*(1/3)
                y: 0
                width:grid.width*(2/3)
                height:limitsMenuItem.height
            }
        }
        Item {
            id: tab1
            LimitsControl
            {
//                x:(grid.width-(20/parentwidth)*grid.width)/4 +(20/1920)*window.width
//                y: (20/780)*window.height
//                width:window.width*(1350/1920)-x
//                height:(870/780)*window.height
                x: grid.width*(1/3)
                y: 0
                width:grid.width*(2/3)
                height:limitsMenuItem.height
            }
        }
        Item {
            id: tab2
            LimitsSuspectAndReject
            {
                x: grid.width*(1/3)
                y: 0
                width:grid.width*(2/3)
                height:limitsMenuItem.height
            }
        }
    }
}
