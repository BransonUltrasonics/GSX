import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import com.branson.menu 1.0

Item {
    implicitWidth: 400
    implicitHeight: 995
    property real gridHeight:1080
    property real parentHeight:1080
    property real rectHeight: 80
    property real rectWidth:400
    property real parentwidth:1920
    property real limitsmenupressureX: 0
    property string pressedColor: qmlPrimaryColor
    property string unpressedColor: "#ECECEC"

    id:root

    /**
      *@breif: To switch title bar text
      **/
    function switchTitleBarText()
    {
    }
    Component.onCompleted: {
        obj_smartStack.smartStackInit()
    }

    ListView
    {
        id: id_toolingGrid
        width: parent.width
        height: parent.height
        currentIndex:0
        clip:true
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection
        onCurrentItemChanged:
        {
            id_toolingGrid.itemAt(currentItem.x,currentItem.y).paramColor="#000000"
        }
        onCurrentIndexChanged: {
            switchTitleBarText()
        }
        spacing: 1
        model: ListModel{
            ListElement{name:qsTr("SMART STACK")}
        }
        delegate:ConfigurationItem
        {
            id:id_toolingGrid_tab0
            paramText:name
            height:  id_toolingGrid.height/10
            width:  id_toolingGrid.width/5

            tab_griditem_mousearea.onClicked:
            {
                id_toolingGrid.currentIndex=index
            }

            Connections
            {
                target:id_toolingGrid
                onCurrentItemChanged:
                {
                    switchColor()
                }
            }
            function switchColor()
            {
                if(index==id_toolingGrid.currentIndex)
                {
                    paramTextColor="#ffffff"
                    paramColor=pressedColor
                }
                if(index!=id_toolingGrid.currentIndex)
                {
                    paramTextColor="#000000"
                    paramColor=unpressedColor
                }
            }
        }
    }
    StackLayout
    {
        id: view
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: id_toolingGrid.currentIndex
        Item
        {
            id: tab0
            SmartStack
            {
                x:(420/1920)*window.width
                y: id_toolingGrid.y
                width:window.width*(1500/1920)
                height:(995/1080)*window.height
            }
        }
    }
}
