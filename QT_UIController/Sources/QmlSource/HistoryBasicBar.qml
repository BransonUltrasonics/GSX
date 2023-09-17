import QtQuick 2.0
import Style 1.0

Item
{
    property int graphlistIndex:  0
    property real listRectHeight: weldResultGraphImagePanel.height
    property string selectedRecipeNumber: ""
    ListModel
    {
        id:modelHistoryGraphType
        ListElement{name:"qrc:/images/image/double-graph.svg"}
        ListElement{name:"qrc:/images/image/trends.svg"}
        ListElement{name:"qrc:/images/image/table.svg"}
    }

    Rectangle
    {
        id: weldResultGraphImagePanel
        width: parent.width
        height: parent.height
        color: "#eeeeee"
        ListView
        {
            id:graphlist
            width: parent.width
            height: weldResultGraphImagePanel.height
            orientation: ListView.Horizontal
            spacing: 1
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            model:modelHistoryGraphType
            focus: true
            delegate:Rectangle
            {
                id: borderImgRect
                width: graphlist.width/3
                height: graphlist.height
                border.width: 0
                x: graphlist.width/20
                border.color: qmlPrimaryColor
                color:"#00ffffff"
                MouseArea
                {
                    id:borderImgRectMouseArea
                    anchors.fill:parent
                    onClicked:
                    {
                        graphlist.currentIndex=index
                        graphlistIndex = index
                    }
                }
                Connections
                {
                    target:graphlist
                    onCurrentItemChanged:
                    {
                        if(index===graphlist.currentIndex)
                        {
                            borderImgRect.border.width=2
                        }
                        if(index!==graphlist.currentIndex)
                        {
                            borderImgRect.border.width=0
                        }
                    }
                }
                Image
                {
                    id: graphImg
                    width: 40
                    height: 40
                    x: (borderImgRect.width-width)/2
                    y: (borderImgRect.height-height)/2
                    source: name
                }
            }
            onCurrentIndexChanged: 
			{
            }
        }
    }

}
