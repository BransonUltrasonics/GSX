import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import Style 1.0
import com.branson.runtimefeatures 1.0

Item
{

    readonly property string qmlTextcolon : qsTr(" : ")
    readonly property string qmlTextcomma : qsTr(",")

    property alias weldHistoryListView: weldHistoryListView
    property alias weldGraphPlotTimer: weldGraphPlotTimer
    property int graphlistIndex:  0
    property real graphListHeight : weldHistoryListView.height*(1/9)
    property real listRectHeight: weldResultGraphImagePanel.height
    property string selectedRecipeNumber: ""
    property string strweldRecipeID: ""
    property string strweldResultID: ""
    property string strweldResultRowID: ""
    property bool runTimeConfigTrends: config.getFeatureFlagStatus(RuntimeFeatures.UI_TRENDS)
    property bool isHistoryPressed: false
    property string resultSelected: ""
    property int graphPageIndex: 0
    property bool historyReachedBottom: false
    property real historyListCurrPosition:0
    property bool isLocationUpdateRequired: false
    property bool isTableViewSelected: false

    Component.onDestruction:
    {
        recipe.setHistoryToNull()
    }

    /**
    *@breif: Load the graph page based on index whether normal or trends or table graph
    **/
    function loadGraphPage(index)
    {
        var localLoaderElement = loaderelement
        var dwPageIndex = config.getActivePage()
        if(dwPageIndex == 5)
            localLoaderElement = loaderelementAnalysis

        switch(index)
        {
        case 0:
            localLoaderElement.item.loadNormalGraph()
            break;
        case 1:
            localLoaderElement.item.loadTableGraph()
            //localLoaderElement.item.loadTrendsGraph()
            break;
        case 2:
            localLoaderElement.item.loadTableGraph()
            break;
        default:
            break;
        }
        weldGraphPlotTimer.start()
    }

    ListModel
    {
        id:modelHistoryGraphType
        ListElement{name:"qrc:/images/image/double-graph.svg"}
        ListElement{name:"qrc:/images/image/table.svg"}
    }
    ListModel
    {
        id:modelHistoryGraphType2
        ListElement{name:"qrc:/images/image/double-graph.svg"}
        ListElement{name:"qrc:/images/image/table.svg"}
    }
    Timer
    {
        id: weldGraphPlotTimer
        interval: 10
        triggeredOnStart: false
        repeat: false
        onTriggered:
        {
            if(graphPageIndex == 0)
            {
                if(recipe.getAutoGraphRefreshState())
                {
                    weldGraphObj.receiveWeldPoints(strweldRecipeID+qmlTextcomma+strweldResultID+qmlTextcomma+strweldResultRowID);
                    weldGraphObj.receiveWeldGraphData(strweldRecipeID+qmlTextcomma+strweldResultID+qmlTextcomma+strweldResultRowID);
                }
                loaderGraphElement.item.resetPlots()
                loaderGraphElement.item.plotGraph()
            }
            else if(graphPageIndex == 1)
            {
                loaderGraphElement.item.tableRowSelection(weldHistoryListView.currentIndex)
            }
            else if(graphPageIndex == 2)
            {
                loaderGraphElement.item.tableRowSelection(weldHistoryListView.currentIndex)
            }
        }
    }
    DropShadow{
        source: weldResultPanel
        anchors.fill: weldResultPanel
        horizontalOffset: 1
        verticalOffset: 1
        color: "#000000"
        opacity: 0.2
        radius: 3
    }
    Rectangle
    {
        id: weldResultPanel
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height
        color: "#f8f9fa"
        clip: true

        Rectangle
        {
            id: weldResultGraphImagePanel
            width: parent.width
            height: parent.height * (9/100)
            color: "#f8f9fa"
            ListView
            {
                id:graphlist
                width: parent.width
                height: weldResultGraphImagePanel.height
                orientation: ListView.Horizontal
                spacing: 1
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.AutoFlickDirection
                model:runTimeConfigTrends ? modelHistoryGraphType : modelHistoryGraphType2
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
                            if(runTimeConfigTrends == 1)
                            {
                                graphlist.currentIndex=index
                                graphlistIndex = index
                            }
                            else
                            {
                                graphlist.currentIndex=index
                                graphlistIndex = index
                                if(graphlistIndex == 1)
                                {
                                    graphlistIndex = graphlistIndex+1
                                }
                            }
                            loadGraphPage(graphlistIndex)
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

                onCurrentIndexChanged: {

                    if(graphlist.currentIndex == 1)
                    {
                        graphtableview.getGraphViewTableOrder();
                    }
                }
            }
        }
        Rectangle
        {
            id: weldResultHistoryPanel
            anchors.top: weldResultGraphImagePanel.bottom
            anchors.left: parent.left
            width: parent.width
            height: (parent.height * (0.9)) -18
            color: "#f8f9fa"
            ListView
            {
                id: weldHistoryListView
                width: parent.width
                height: weldResultHistoryPanel.height
                spacing: 0
                cacheBuffer: 0
                clip: true
                flickableDirection: Flickable.VerticalFlick
                onContentYChanged:
                {
                    if (contentY >= (contentHeight - weldHistoryListView.height-100))
                    {
                        if(!historyReachedBottom)
                        {
                            historyListCurrPosition = contentY
                            historyReachedBottom = true
							isLocationUpdateRequired = true
                            recipe.updateWeldHistoryList(2)
                        }
                    }
                    else
                    {
                        if(historyReachedBottom)
                            historyReachedBottom = false;
                    }
                }

                model:weldResultsModel
                delegate: Rectangle
                {
                    id:listRect
                    width: parent.width
                    height: weldHistoryListView.height*(1/9)
                    border.color: "#E9ECEF"
                    border.width: 0.5
                    color: "#f8f9fa"
                    Text
                    {
                        id: weldRecipeID
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        color: "#898D96"
                        anchors.left:listRect.left
                        anchors.leftMargin:  10
                        fontSizeMode: Text.HorizontalFit
                        font.pixelSize: Style.style3
                        text:WeldResultNumber + qmlTextcolon
                        font.family: Style.regular.name

                    }
                    Text
                    {
                        id: weldResultID
                        anchors.top: parent.top
                        anchors.topMargin: 8
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
                        anchors.topMargin: 8
                        color: "#898D96"
                        anchors.left:weldResultID.right
                        anchors.leftMargin: 0
                        fontSizeMode: Text.HorizontalFit
                        font.pixelSize: Style.style3
                        text:qmlTextcolon + WeldResultMode
                        font.family: Style.regular.name

                    }
                    Image
                    {
                        id:alaramAlert
                        anchors.top: listRect.top
                        anchors.topMargin: 8
                        anchors.right: listRect.right
                        anchors.rightMargin: 8
                        source: "qrc:/images/image/alaram_alert.svg"
                        visible: AlaramRaised == 1 ? true : false
                    }
                    Text
                    {
                        id: list1Date
                        anchors.top: weldResultID.bottom
                        anchors.topMargin: 5
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
                        anchors.topMargin: 5
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
                            recipe.setIsManualRefreshState(true);
                            weldHistoryListView.currentIndex=index
                            recipe.setLastSelectedResult(weldResultID.text)
                            g_SelectedRecipeNum = WeldResultNumber
                        }
                    }

                    Connections
                    {
                        target: weldHistoryListView
                        onCurrentIndexChanged:
                        {
                            if(index==weldHistoryListView.currentIndex)
                            {
                                g_SelectedRecipeNum=weldRecipeID.text.replace(":","")
                                if(isTableViewSelected)
                                {
                                    recipe.setLastSelectedResult(weldResultID.text)
                                    isTableViewSelected = false
                                }
                                listRect.color = qmlPrimaryColor
                                weldResultID.color = "#ffffff"
                                weldRecipeID.color = "#ffffff"
                                weldResultMode.color = "#ffffff"
                                list1Date.color = "#ffffff"
                                list1Time.color = "#ffffff"
                                weldId = weldResultID.text
                                strweldRecipeID = weldRecipeID.text.replace(":", "")
                                strweldResultID = weldResultID.text.toString()
                                strweldResultRowID = WeldResultRowId
                                weldGraphPlotTimer.start()
                            }
                            else
                            {
                                listRect.color = "#f8f9fa"
                                weldResultID.color = "#898D96"
                                weldRecipeID.color = "#898D96"
                                weldResultMode.color = "#898D96"
                                list1Date.color = "#898D96"
                                list1Time.color = "#898D96"
                                weldId = weldResultID.text
                            }
                        }
                    }
                    Component.onCompleted:
                    {
                        g_SelectedRecipeNum = WeldResultNumber
                        //recipe.setLastSelectedResult(weldHistoryListView.currentSection.toString())
                    }
                }
                onModelChanged:
                {
                    var dwPastIndex = recipe.getPastSelectIndex(recipe.getLastSelectedResult())
                    if(count>0 && !recipe.isManualRefresh)
                    {
                        weldHistoryListView.currentIndex = dwPastIndex;
                    }
                    if(contentHeight > contentY)
                    {
                        if(isLocationUpdateRequired == true)
                        {
                            contentY = historyListCurrPosition
                            isLocationUpdateRequired = false
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {}
                Component.onCompleted:
                {
                    var dwPastIndex = recipe.getPastSelectIndex(recipe.getLastSelectedResult())
                    if(dwPastIndex != weldHistoryListView.currentIndex)
                    {
                        weldHistoryListView.currentIndex = dwPastIndex;
                    }
                }
            }
        }
    }
}
