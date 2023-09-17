import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import Style 1.0

Item {
    id:root
    readonly property string qmltextChooseParameter : qsTr("CHOOSE PARAMETER")
    readonly property string qmltextChooseParameterDes :qsTr("Select parameters to graph")
    readonly property string qmltextDone :qsTr("DONE")
    FontLoader {id: light; source: "qrc:/Fonts/OpenSans-Light.ttf"}
    FontLoader {id: regular; source: "qrc:/Fonts/OpenSans-Regular.ttf"}
    FontLoader {id: semibold; source: "qrc:/Fonts/OpenSans-Semibold.ttf"}
    property real tableHeaderH: 0
    property real tableRowH: 0
    property int windowWidth: 0
    property int windowHeight: 0
    property alias graphTableView: graphTableView
    property bool g_Primary: false
    property bool tableModelChanged: false
    property bool firstTimePageLoaded: true

    /**
    *@breif:To set the selected row index
    **/
    function tableRowSelection(index)
    {
        graphTableView.selection.deselect(0,graphTableView.rowCount-1)
        graphTableView.selection.select(index)
        graphTableView.positionViewAtRow(index,ListView.Visible)
    }

    Component.onCompleted:
    {
        windowWidth=window.width
        windowHeight=window.height
        graphTableView.selection.select(0)
        tableModelChanged: false
    }
    Component
    {
        id: tablecolumn
        TableViewColumn
        {
            width: (255/1920)*windowWidth
        }
    }
    Timer
    {
        id: tableRefresh
        triggeredOnStart: false
        interval: 10
        onTriggered:
        {
            var index = graphTableView.currentRow
            recipe.updateTableGridViewModel()
            tableRowSelection(index)
        }
    }
    TableView
    {
        id: graphTableView
        width:parent.width
        height:parent.height
        flickableItem.flickableDirection : Flickable.HorizontalAndVerticalFlick
        anchors
        {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        property int currentColumn: 0
        property int selectedRow: 2

        model: tableGridViewmodel
        style: TableViewStyle
        {
            headerDelegate: Rectangle
            {
                height: tableHeaderH
                width: textItem.implicitWidth
                color: "#ECECEC"
                Connections
                {
                    target: styleData
                    onPressedChanged:
                    {
                        if(g_Primary)
                        {
                            if(styleData.pressed == false)
                            {
                                var strHeaderOrder=qmlTextEmpty;
                                for(var i=0;i <graphTableView.columnCount;i++)
                                {
                                    strHeaderOrder = strHeaderOrder + graphTableView.getColumn(i).title + ","
                                }
                                graphtableview.updateOrderIndex(strHeaderOrder)
                                config.setAnalyticsTableOrder(strHeaderOrder)
                                tableRefresh.start()
                            }
                        }
                        g_Primary =!g_Primary;
                    }
                }
                Text
                {
                    id: textItem
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: styleData.value
                    elide: Text.ElideRight
                    color: "#757575"
                    font.family: Style.semibold.name
                    font.pixelSize:  Style.style4
                    renderType: Text.NativeRendering
                }
                Rectangle
                {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 1
                    anchors.topMargin: 1
                    width: 1
                    color: "#ccc"
                }
            }
            rowDelegate: Item
            {
                height: tableRowH
                Rectangle
                {
                    id: rectangle1
                    color: styleData.selected ? qmlPrimaryColor : "#FFFFFF"
                    anchors.fill: parent
                    border.width: 1
                    border.color: "#E9ECEF"
                }
            }

            itemDelegate: Item
            {
                height: tableRowH
                Text
                {
                    id: folderText1
                    fontSizeMode: Text.Fit
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text:graphtableview.getTableElementValue(styleData.value.Value, graphtableview.getTableIndex(styleData.column))
                    color: styleData.selected ? "#FFFFFF" :"#898D96"
                    font.family: Style.semibold.name
                    font.pixelSize:  Style.style2
                }
            }
        }

        Component.onCompleted:
        {
            addColumns();
        }
        onModelChanged:
        {
            addColumns();
            tableModelChanged = true
        }
        onCurrentRowChanged:
        {
            if(graphTableView.rowCount <=0)
                return

            var index = graphTableView.currentRow
            if(!tableModelChanged || firstTimePageLoaded)
            {
                weldResultHistory.isTableViewSelected = true
                weldResultHistory.weldHistoryListView.currentIndex = index
                firstTimePageLoaded = false;
                tableModelChanged = false;
            }
            else
            {
                tableModelChanged = false;
                graphTableView.selection.deselect(0,graphTableView.rowCount-1)
                graphTableView.selection.select(weldResultHistory.weldHistoryListView.currentIndex)
            }
        }
        //ScrollBar.horizontal: ScrollBar{}
        //ScrollBar.vertical: ScrollBar{}
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
        width:(43/1920)*windowWidth
        height:width
        anchors.right: graphTableView.right
        anchors.rightMargin: 20
        anchors.top: graphTableView.top
        anchors.topMargin: 10

        Image {
            id: delta
            sourceSize.width: parent.width*(90/100)
            sourceSize.height: width
            anchors.bottom: parent.bottom
            anchors.bottomMargin:(10/100)*parent.height
            anchors.top:parent.top
            anchors.topMargin: (10/100)*parent.height
            anchors.left: parent.left
            anchors.leftMargin: (12/100)*parent.width
            anchors.right: parent.right
            anchors.rightMargin: (12/100)*parent.width
            source: "qrc:/images/image/delta.svg"
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    checkbox_rect_param_selection.open()
                    enableMainwindowOpacity.start()
                }
            }
            ColorOverlay
            {
                anchors.fill: delta
                source: delta
                color: qmlPrimaryColor
            }
        }
    }

    Popup
    {
        id:checkbox_rect_param_selection
        width:parent.width-(300/1920)*windowWidth
        height:parent.height-(200/1080)*windowHeight
        modal: true
        focus: true
        dim: true
        closePolicy: Popup.NoAutoClose

        background: Rectangle
        {
            width:checkbox_rect_param_selection.width
            height:checkbox_rect_param_selection.height
            color:"#ffffff"
            border.width: 0
            border.color:"#000000"
            radius:3
            GridView
            {
                id:gridview3
                width: parent.width - 20
                height: parent.height * (7/10)
                anchors.topMargin: 30
                anchors.top: title_choose_param.bottom
                anchors.left: parent.left
                anchors.leftMargin: 20
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.AutoFlickDirection
                clip:true
                cellWidth: gridview3.width/3
                cellHeight: gridview3.height/5
                model: deltaParametersModel
                delegate:Item
                {
                    id: paramCheckBox
                    width:gridview3.cellWidth - 10
                    height:gridview3.cellHeight//-10//(10/1080)*windowHeight
                    property bool checkBoxEnabled: true
                    property bool checkBoxChecked: false
                    Component.onCompleted:
                    {
                        if(graphtableview.isIndexInList(index))
                        {
                            checkBoxChecked=true;
                        }
                    }
                    onCheckBoxCheckedChanged:
                    {
                        graphcheckbox.checkBoxStatus = (checkBoxChecked == true)
                    }

                    Rectangle {
                        id: rectangle4
                        width: gridview3.cellWidth - 40
                        height: 50
                        color: "#ffffff"

                        CustomCheckBox
                        {
                            id:graphcheckbox
                            width: (34/220)*parent.width
                            height: (30/50)*parent.height
                            anchors.rightMargin: (176/220)*parent.width
                            anchors.leftMargin: (0/220)*parent.width
                            anchors.bottomMargin: (10/50)*parent.height
                            anchors.topMargin: (10/50)*parent.height
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            anchors.left: parent.left
                            checkBoxStatus: (checkBoxChecked == true)
                            enabled: (checkBoxEnabled == true)
                            checkBoxName: qsTr(model.modelData.name)

                            onCheckBoxStatusChanged:
                            {
                                //ToDo
                                listviewText.text = ""
                                if(checkBoxStatus)
                                {
                                    graphtableview.addColumns(index);
                                }
                                if(!checkBoxStatus)
                                {
                                    graphtableview.removeColumn(index);
                                }
                            }
                        }
                    }
                }
                ScrollBar.vertical: ScrollBar
                {
                    parent: gridview3
                    anchors.top: gridview3.top
                    anchors.bottom: gridview3.bottom
                }
            }

            Rectangle{
                id:title_choose_param
                height:(44/1080)*windowHeight
                x:parent.x
                width:parent.width
                color:qmlPrimaryColor
                Text {
                    id: choose_param
                    height: (34/1080)*window.height
                    text: qmltextChooseParameter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.Top
                    font.family: Style.regular.name
                    font.pixelSize: Style.style3
                    color:"#ffffff"
                }
                Image
                {
                    id: image
                    sourceSize.width: 30
                    sourceSize.height: 30
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    /*Image icon update based on System Type*/
                    source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/close_ASX.svg" :
                                                                   "qrc:/images/image/close.svg"
                }
                MouseArea
                {
                    id: mouseArea
                    width: 40
                    height: 40
                    x: image.x
                    y: image.y
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    onClicked:
                    {
                        checkbox_rect_param_selection.close()
                        disableMainwindowOpacity.start()
                    }
                    z:1
                }
            }

            Text
            {
                id: listviewText
                width: 300
                height: 30
                text:""
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.left: parent.left
                anchors.leftMargin:50
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: Style.style1
                font.family: Style.semibold.name
                color: qmlSecondaryColor
            }
            PrimaryButton
            {
                id: button
                width:(140/1920)*windowWidth
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 50
                text: qmltextDone
                onClicked:
                {
                    var listsize = graphtableview.getListSize();;
                    if(listsize == 0)
                    {
                        listviewText.text = "No parameter is choosen, please select atleast one parameter to continue"
                    }
                    else
                    {
                        disableMainwindowOpacity.start()
                        checkbox_rect_param_selection.close()
                        addColumns();
                    }
                }
            }
        }
    }

    /**
    *@breif: To add columns in the table
    **/
    function addColumns()
    {

        var gridviewIndex=0;
        var noOfColumns=graphTableView.columnCount;
        while(noOfColumns)
        {
            graphTableView.removeColumn(0);
            noOfColumns--;
        }
        var size=graphtableview.getListSize();

        for(var checkedIndex=0;checkedIndex<size;checkedIndex++)
        {
            var role  = graphtableview.getTextFromModelAtIndex(graphtableview.getTableIndex(checkedIndex));
            var temp=tablecolumn.createObject(graphTableView, { "role": role, "title": role});
            graphTableView.addColumn(temp);
        }
        graphtableview.setGraphViewTableOrder(qmlTextEmpty)
    }
}
