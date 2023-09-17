import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Style 1.0
import QtQuick.Window 2.2

Item
{
    property int recipePramGridViewCellWidth: 300
    property int recipePramGridViewCellHeight: 70
	/**
      *@breif: To add columns to the tableview
      **/
    function addColumns()
    {

        var gridviewIndex=0;
        var noOfColumns=graphTableView.columnCount;
        while(noOfColumns){

            graphTableView.removeColumn(0);
            noOfColumns--;
        }
        var size=graphtableview.getEditRecipeHistoryListSize();

        for(var checkedIndex=0;checkedIndex<size;checkedIndex++)
        {
            var roletemp  = graphtableview.getTextFromModelAtIndexInEditRecipeHistoryList(graphtableview.getEditRecipeHistoryTableIndex(checkedIndex));
            var temp=tablecolumn.createObject(graphTableView, { role: roletemp, title: roletemp, width:200});
            graphTableView.addColumn(temp);
        }
    }

    Rectangle
    {
        id:tableViewRect
        anchors.fill: parent
        color: "#F8F9FA"
        Component
        {
            id: tablecolumn
            TableViewColumn
            {
            }
        }
        TableView
        {
            id: graphTableView
            width:parent.width
            height:parent.height

            property int currentColumn: 0
            property int selectedRow: 2

            ScrollBar.horizontal:ScrollBar{}

            model: t_editRecipemodel
            style: TableViewStyle
            {
                headerDelegate: Rectangle
                {
                    id:upperrectangle
                    height: 50
                    width:textItem.font.pixelSize*(textItem.text.length)
                    color: "#ECECEC"
                    TextMetrics
                    {
                        id: textMetrics
                        elide: Text.ElideRight
                        elideWidth: 80
                        text: styleData.value
                    }
                    Text
                    {
                        id: textItem
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: textMetrics.text
                        width:textItem.font.pixelSize*(textItem.text.length)
                        color: "#757575"
                        font.pixelSize: Style.style3
                        font.bold: true
                        renderType: Text.NativeRendering
                        font.family: Style.semibold.name

                    }
                    Rectangle
                    {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 1
                        anchors.topMargin: 1
                    }
                }
                rowDelegate: Item
                {
                    height: 40

                    Rectangle
                    {
                        id: rectangle1
                        color: styleData.selected ? qmlPrimaryColor : "#FFFFFF"
                        anchors.fill: parent
                        border.width: 0.5
                        border.color: "#E9ECEF"
                    }
                }

                itemDelegate: Item
                {
                    height: 40
                    Text
                    {
                        id: folderText1
                        text: graphtableview.getTableElementValue(model.modelData.Value,graphtableview.getEditRecipeHistoryTableIndex(styleData.column)+1)
                        fontSizeMode: Text.Fit
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: Style.semibold.name
                        font.pixelSize:  Style.style2
                        color: styleData.selected ? "#FFFFFF" :"#898D96"
                    }
                }
            }
            Component.onCompleted: {
                addColumns();
            }
        }
        DropShadow
        {
            anchors.fill: deltaRectangle
            source: deltaRectangle
            horizontalOffset:1
            verticalOffset:0.5
            opacity:0.5
            radius:3
            color:"#000000"
            samples:7
        }
        Rectangle {
            id: deltaRectangle
            color: "#e9ecef"
            border.color: "#e9ecef"
            radius: 0
            width: 40
            height: 40
            anchors.left: graphTableView.right
            anchors.leftMargin: -50
            anchors.top:graphTableView.top
            anchors.topMargin: 5

            Image {
                id: delta
                sourceSize.width: 35
                sourceSize.height: 35
                width:35
                height:width
                x:parent.width/2-width/2
                y:parent.height/2-height/2
                source: "qrc:/images/image/delta.svg"
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {
                        checkbox_rect_param_selection.open()
                        enableMainwindowOpacity.start()
                    }
                }
                ColorOverlay {
                    anchors.fill: delta
                    source: delta
                    color: qmlPrimaryColor
                }
            }
        }

        Popup{
            id:checkbox_rect_param_selection
            x: 30
            property int tempWidth: Screen.width/recipePramGridViewCellWidth
            width: tempWidth*recipePramGridViewCellWidth
            height: parent.height-20

            background: Rectangle
            {
                width:checkbox_rect_param_selection.width
                height:checkbox_rect_param_selection.height

                color:"#E9ECEF"
                border.width: 0
                border.color:"#000000"
                radius:3
                GridView {
                    id:gridview3
                    //x: 20
                    //y: 100
                    anchors.top:choose_param_description.bottom
                    //anchors.fill: parent
                    anchors.left:parent.left
                    width:parent.width
                    height:parent.height-90
                    boundsBehavior: Flickable.StopAtBounds
                    flickableDirection: Flickable.AutoFlickDirection
                    clip:true
                    cellWidth: recipePramGridViewCellWidth
                    cellHeight: recipePramGridViewCellHeight
                    model: m_editRecipeModel
                    delegate:Item {
                        id: paramCheckBox
                        width:gridview3.cellWidth
                        height:gridview3.cellHeight

                        property bool checkBoxEnabled: true
                        property bool checkBoxChecked: false
                        Component.onCompleted:
                        {
                            if(graphtableview.isIndexInEditRecipeHistoryList(index))
                            {
                                checkBoxChecked=true;
                            }
                        }
                        onCheckBoxCheckedChanged:
                        {
                            graphcheckbox.checkBoxStatus = (checkBoxChecked == true)
                        }

                        Rectangle {
                            id: rectangle_checkBox
                            width: parent.width; height: (50/200)*parent.height
                            color: "#E9ECEF"


                            CustomCheckBox{
                                id:graphcheckbox
                                width: gridview3.cellWidth
                                height: gridview3.cellHeight
                                checkBoxStatus: (checkBoxChecked == true)
                                enabled: (checkBoxEnabled == true)
                                checkBoxName: qsTr(model.modelData.name)

                                onCheckBoxStatusChanged:
                                {
                                    //ToDo
                                    if(checkBoxStatus)
                                    {
                                        graphtableview.addEditRecipeHistoryColumns(index);
                                    }
                                    if(!checkBoxStatus)
                                    {
                                        graphtableview.removeEditRecipeHistoryColumn(index);
                                    }
                                }
                            }
                        }
                    }
                    ScrollBar.vertical: ScrollBar{}

                }
                Rectangle
                {
                    id:title_choose_param
                    height: 45
                    x:parent.x
                    y:parent.y
                    width:parent.width
                    color:qmlPrimaryColor
                    Text {
                        id: choose_param
                        height: 45
                        text: qsTr("Choose Parameter")
                        y:parent.height/2-height/2
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: Style.style4
                        font.family: Style.semibold.name
                        color:"#868E96"
                    }
                }
                Text {
                    id: choose_param_description
                    anchors.top:title_choose_param.bottom
                    height: 45
                    text: qsTr("Select parameters to graph")
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: Style.style2
                    font.family: Style.regular.name
                    color: "#868E96"
                }

                PrimaryButton {
                    id: button
                    width: 50
                    height: 30
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    text: qsTr("DONE")
                    onClicked: {
                        disableMainwindowOpacity.start()
                        checkbox_rect_param_selection.close()
                        addColumns();
                    }
                }
            }
        }
    }
}
