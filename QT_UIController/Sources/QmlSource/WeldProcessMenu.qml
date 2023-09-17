import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQml.Models 2.2
Item {
    id:weldProcessMenu
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#ECECEC"
    property int selectedIndex: 0

    /**
      *@breif: Setting currentindex based on the parameter
      **/
    function setCurrentIndex(paramtext)
    {
        if(paramtext == "Pretrigger")
            selectedIndex = 0
        else if(paramtext == "Energy Brake")
            selectedIndex = 1
        else if(paramtext == "Afterburst")
            selectedIndex = 2
        else if(paramtext == "Stack Recipe")
            selectedIndex = 3
    }

    Rectangle {
        anchors.fill: parent
        color: "#F8F9FA"
        ListView
        {
            id: grid
            x: 0
            y: 0
            width: parent.width-50
            height: parent.height-50
            currentIndex:0
            clip:true
            spacing: 1
            model: modelWeldProcessMenu
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            delegate:LimitMenuItem
            {
                id:tab_grid
                paramText: model.modelData.ParameterName
                paramLocaleText: model.modelData.ParameterLocaleName
                height: grid.height/10
                width: grid.width/4
                tab_griditem_mousearea.onClicked:
                {
                    grid.currentIndex=index
                    setCurrentIndex(paramText)
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
                  *@breif: To change the current index color
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
                grid.currentIndex=0
                setCurrentIndex(grid.currentItem.paramText)
            }
        }
    }

    StackLayout {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: selectedIndex

        Item {
            id: tab0
            WeldProcessPreTrigger
            {
                x: grid.width*(1/3)
                y: 0
                width:grid.width*(2/3)
                height:weldProcessMenu.height
            }
        }
        Item {
            id: tab1
            WeldProcessEnergyBreak
            {
                x: grid.width*(1/3)
                y: 0
                width:grid.width*(2/3)
                height:weldProcessMenu.height
            }
        }
        Item {
            id: tab2
            WeldProcessAfterBrust
            {
                x: grid.width*(1/3)
                y: 0
                width:grid.width*(2/3)
                height:weldProcessMenu.height
            }
        }
        Item{
            id: tab3
            WeldProcessStackRecipe
            {
                x: grid.width*(1/3)
                y: 0
                width:grid.width*(2/3)
                height:weldProcessMenu.height
            }
        }
    }
}
