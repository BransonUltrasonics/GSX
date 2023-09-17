import QtQuick 2.0
import QtGraphicalEffects 1.0
import Style 1.0

Item
{

//    DropShadow{
//        anchors.fill: listRect
//        horizontalOffset: 1
//        verticalOffset: 1
//        color: "#60000000"
//        opacity: 0.2
//        source: listRect
//    }

    Rectangle
    {
        id:listRect
        width: parent.width
        height: parent.height
        color: "#F8F9FA"
        Text
        {
            id: weldRecipeID
            anchors.top: parent.top
            color: "#898D96"
            anchors.left:listRect.left
            anchors.leftMargin:  10
            fontSizeMode: Text.HorizontalFit
            font.pixelSize: Style.style3
            text:WeldResultNumber + qsTr(" : ")
            font.family: Style.regular.name

        }
        Text
        {
            id: weldResultID
            anchors.top: parent.top
            color: "#898D96"
            anchors.left:weldRecipeID.right
            anchors.leftMargin:  0
            fontSizeMode: Text.HorizontalFit
            font.pixelSize: Style.style3
            text:WeldResultID
            font.family: Style.regular.name

        }
        Text
        {
            id: weldResultMode
            anchors.top: parent.top
            color: "#898D96"
            anchors.left:weldResultID.right
            anchors.leftMargin: 0
            fontSizeMode: Text.HorizontalFit
            font.pixelSize: Style.style3
            text:qsTr(" : ") + WeldResultMode
            font.family: Style.regular.name

        }
        Text
        {
            id: list1Date
            anchors.top: weldResultID.bottom
            color: "#898D96"
            anchors.left:listRect.left
            anchors.leftMargin:  10
            text:WeldResultDate
            font.pixelSize: Style.style1
            font.family: Style.regular.name
        }
        Text
        {
            id: list1Time
            anchors.top: weldResultID.bottom
            anchors.right:listRect.right
            anchors.rightMargin: 10
            color: "#898D96"
            text:WeldResultTime
            font.pixelSize: Style.style1
            font.family: Style.regular.name
        }
        MouseArea
        {
            id:tab_griditem_mousearea
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked:
            {
                weldHistoryListView.currentIndex=index
            }
        }
    }
}
