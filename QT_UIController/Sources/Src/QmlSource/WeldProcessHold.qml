import QtQuick 2.0
import QtQuick.Controls 2.2

Item
{
    GridView
    {
        id: grid
        x: 0
        y: 50
        width: parent.width
        height: parent.height-50
        cellHeight: height/5
        cellWidth: width/2
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickDirection
        model:ModelWeldProcessHold{id:holdmenuitem}
        delegate: WeldModeItem
        {
            paramText:name
            paramTextLocale:name
            height:  labelSize
            width:  grid.cellWidth-80
        }
    }
}
