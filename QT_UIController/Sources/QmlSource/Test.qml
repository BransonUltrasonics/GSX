import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4 as OldStyle
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Window 2.1
import QtCharts 2.1
import CustomValidator 1.0
import QtQuick.Layouts 1.3
import Style 1.0

Item {

    readonly property string qmltextStartTest:qsTr("Start Test")
    readonly property string qmltextRunning: qsTr("Running")
    readonly property string qmltextReceipeSendFailed: qsTr("Test recipe sending failed")
    readonly property string qmltextSuccess: qsTr("Success")
    readonly property string qmltextFailed: qsTr("Failed")
    readonly property string qmltextTestRecipe: qsTr("Test Recipe")
    readonly property string qmltextTestRecipeTitleBar: qsTr("TEST RECIPE")
    readonly property string qmltextTestGraphChatItem: qsTr("testgraphChartItem")
    readonly property string qmltextUltrasonictest: qsTr("ULTRASONICS TEST")

    id: testModeId
    FontLoader {id: light; source: "qrc:/Fonts/OpenSans-Light.ttf"}
    FontLoader {id: regular; source: "qrc:/Fonts/OpenSans-Regular.ttf"}
    FontLoader {id: semibold; source: "qrc:/Fonts/OpenSans-Semibold.ttf"}
    property bool  response:false
    // white back ground color
    readonly property color whiteBackgroundColor: "#FFFFFF"

    /**
     * left margin spacing: 20
     * right margin spacing: 20
     */
    readonly property int leftMarginLength: 20
    readonly property int rightMarginLength: 10
    readonly property int topMarginLength: 20
    readonly property int bottomMarginLength: 20
    property double fontSizeScaleFactor : window.height / 800
    property var numOfAxis: 0
    property var numOfGraph: 0
    property var  responseForGraph: -1
    property var xAxisUnit: " s"
    property int isPageLoaded: 0

    // left rectangle with in percentage: 30%
    readonly property int leftRectangleWidthInPercentage: 30

    // left top rectangle width size in percentage: 35%
    readonly property int leftTopRectangleWidthInPercentage: 35

    // space between panels
    readonly property int spaceBetweenPanels: 20
    property alias graphNavigatorLabel: graphNavigatorLabel
    property alias testgraphSlider: testgraphSlider
    property alias testgraphNavigatorImage:testgraphNavigatorImage
    property alias testgraphChartItem: testgraphChartItem

//    function createListview()
//    {
//        var listViewComponent = Qt.createComponent("TestModeProgressBarListview.qml")
//        var listViewObject = listViewComponent.createObject(window)
//    }

    Component.onCompleted:
    {
        testThreeway.visible = true
    }
    FourCarouselDots
    {
        z:1
        id:testThreeway
        width:30
        height: 30
        x:parent.width/2
        anchors.top: parent.top
        anchors.topMargin: 10
        Component.onCompleted:
        {
            selected= 3
        }

        mar1.onClicked:
        {
            selected=1
            historyDataObj.hornIndSelected = 0
            hornRecipeObj.init()
            historyDataObj.refreshHistory()
            loaderelement.setSource("HornPage.qml");
            titlebarchild.visible = false
        }
        mar2.onClicked:
        {
            selected=2
            seekRecipeObj.init()
            seekhistoryDataObj.seekIndSelected = 0
            seekhistoryDataObj.refreshSeekHistory()
            loaderelement.setSource("SeekScanFeature.qml");
            titlebarchild.visible = false
        }
        mar3.onClicked:
        {
            //            ultrasonictest.getTestModeAdvancedDataFromDB()
            loaderelement.setSource("Test.qml");
            //            ultrasonictest.setCheckBoxModelData()
            //            selected=3;
            titlebarchild.visible = false
        }
    }

    Rectangle {
        id: testModeRectangle
        width: parent.width
        height: parent.height
        //color: whiteBackgroundColor

        anchors
        {
            top: parent.top
            topMargin: topMarginLength
            bottom: parent.bottom
            bottomMargin: bottomMarginLength
            left: parent.left
            leftMargin: leftMarginLength
            right: parent.right
            rightMargin: rightMarginLength
        }

        Rectangle{
            id: rectangleLeft
            width: window.width * 0.1
            height: parent.height
            Rectangle {
                id: rectangleLeftTop
                width: parent.width
                height: parent.height * (3/10)
                anchors.top:rectangleLeft.top
                anchors.topMargin:parent.height * (25/600)
                //utltrasonic test text old position
                Column
                {
                    spacing:40
                    anchors.centerIn: parent.Center
                    Rectangle {
                        id: customButton_startTest
                        width: 140
                        height: 40
                        color: "#ECECEC"
                        radius: 3
                        Layout.alignment: Qt.AlignCenter
                        layer.enabled: customButton_startTestMA.pressed ? false : true
                        layer.effect: DropShadow{
                            source: customButton_startTest
                            anchors.fill: customButton_startTest
                            horizontalOffset: customButton_startTestMA.pressed ? 0 : 3
                            verticalOffset: customButton_startTestMA.pressed ? 0 : 3
                            color: "#80000000"
                            opacity: 0.2
                            samples: 10
                        }
                        Text {
                            text: qmltextStartTest
                            font{
                                family: Style.semibold.name
                                pixelSize: Style.style2
                                capitalization: Font.AllUppercase
                            }
                            color: "#212529"
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        MouseArea{
                            id:customButton_startTestMA
                            anchors.fill: parent
                            preventStealing: true
                            onPressed:
                            {
                                ultrasonictest.StatusText=qmltextRunning
                                status.text=qmltextRunning
                                response=ultrasonictest.sendTestRecipeDataToSC()
                                if(response)
                                {
                                    ultrasonictest.startTest()
                                }
                                else
                                {
                                    recipe.updateStatusText(qmltextReceipeSendFailed)
                                }
                            }
                            onReleased:
                            {
                                if(ultrasonictest.sendTestResponseToSC())
                                {
                                    ultrasonictest.StatusText="Success"
                                    status.text=qmltextSuccess
                                    recipe.updateStatusText(" ")
                                    responseForGraph=-1;
                                    responseForGraph=graph.getSignatureData()
                                    if(responseForGraph === 0)
                                    {
                                        testgraphChartItem.testplotGraph()
                                        testgraphChartItem.deltaX = testgraphChartItem.plotArea.width / (testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].max -
                                                                                      testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min)
                                        testgraphChartItem.resetCheckBoxValues()
                                    }
                                }
                                else
                                {
                                    ultrasonictest.StatusText=qmltextFailed
                                    status.text=qmltextFailed
                                }
                            }
                        }
                    }
                    SecondaryButton
                    {
                        id: advancedButton
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredHeight:  40
                        textColor: "#212529"
                        font.family: Style.semibold.name
                        text: qmltextTestRecipe
                        onClicked:
                        {
                            ultrasonictest.getTestDefaultValues()
                            ultrasonictest.createTestRecipe()
                            testModeRectangle.visible=false;
                            testThreeway.visible = false
                            id_testadvanced.visible=true
                            titlebarchild.visible=true
                            titlebarchildtext.text=qmltextTestRecipeTitleBar
                            titlbaronclicked="Test.qml"
                        }
                    }
                }
            } //Rectangle left top

        } // Rectangle left

        Rectangle
        {
            id: rectangleRight
            width: window.width*0.60
            height: parent.height - mainStatusbarHeight
            anchors
            {
                left: rectangleLeft.right
                leftMargin: spaceBetweenPanels
                top:rectangleLeft.top
            }


            Rectangle
            {
                id: rectangleRightTop
                color: "#ECECEC"
                height: window.height * (22/100)
                anchors
                {
                    left: parent.left
                    right: parent.right
                    top:parent.top
                    topMargin:parent.height * (25/600)
                }

                ListView
                {
                    id:progressBarRect
                    width: parent.width
                    height: parent.height
                    spacing: rectangleRightTop.height*.11
                    clip:true
                    model: testProgressbarModel
                    anchors
                    {
                        top: parent.top
                        topMargin: rectangleRightTop.height*.075
                        bottom: parent.bottom
                        bottomMargin: rectangleRightTop.height*.075
                        left: parent.left
                        leftMargin: leftMarginLength
                        right: parent.right
                        rightMargin: rightMarginLength
                    }
                    ScrollBar.vertical: ScrollBar
                    {
                        parent: progressBarRect.parent
                        anchors.top: progressBarRect.top
                        anchors.left: progressBarRect.right
                        anchors.bottom: progressBarRect.bottom
                    }
                    delegate: TestModeProgressBarDelegate
                    {
                        height:rectangleRightTop.height*.12
                    }
                }
            } //rectangleRightTop

            Rectangle
            {
                id : rectangleRightDown
                height: parent.height - (rectangleRightTop.height + spaceBetweenPanels)
                anchors
                {
                    top: rectangleRightTop.bottom
                    topMargin: spaceBetweenPanels
                    left: parent.left
                    right: parent.right
                }

                //chartview

                ChartView
                {
                    id:testgraphChartItem
                    property real plotStartPointX;
                    property real plotEndPointX;
                    property real plotStartPointY;
                    property real plotEndPointY;
                    plotStartPointX: plotArea.x
                    plotEndPointX: plotArea.x + plotArea.width
                    plotStartPointY: plotArea.y
                    plotEndPointY: plotArea.y + plotArea.height
                    animationOptions: ChartView.SeriesAnimations
                    property real deltaX;
                    signal getHornSignature(int idVal);
                    legend.visible: false
                    x:0
                    y:testgraphNavigatorImage.height + graphNavigatorLabel.height
//                    deltaX: plotArea.width / (axesList[axesList.length-1].max - axesList[axesList.length-1].min)
                    deltaX: 1.0
                    height: parent.height - (y)
                    width:parent.width
                    property var axesList : []
                    property var graphList : []
                    objectName: qmltextTestGraphChatItem
                    title: ""
                    anchors.fill: parent

                    // Define x-axis to be used with the series instead of default one

                    Component.onCompleted:
                    {

                        responseForGraph=-1;
                        graph.clearLists()
                        graph.addAxesAndSplines();
                        clear()
                        addAxes()
                        addSeries()

                        if(graph.getSignatureData() === 0)
                        {
                            testplotGraph()
                            deltaX = plotArea.width / (axesList[axesList.length-1].max - axesList[axesList.length-1].min)
                            if(graph.XAxisValue!=0)
                            {
                                testgraphSlider.xAxisVal=graph.XAxisValue
                                graphNavigatorLabel.text=graph.GraphTime
                                testgraphSlider.timeLabel = graph.GraphTime+xAxisUnit;
                            }
                        }
                    }

                    function addAxes()
                    {
                        for(var index=0;index<graph.noOfGraphs()+1;index++)
                        {
                            var sprite=Qt.createQmlObject('import QtQuick 2.0; import QtCharts 2.0; ValueAxis {titleText:" "}', testgraphChartItem);
                            sprite.titleText = graph.nameAtAxis(index)
                            sprite.titleVisible = false
                            sprite.minorGridVisible=false
                            sprite.color=graph.colorAtAxis(index)
                            sprite.tickCount=5
                            sprite.gridVisible= false
                            sprite.lineVisible= true
                            axesList.push(sprite);
                        }
                    }

                    function clear(){

                        for(var index=0;index<axesList.length;index++)
                        {
                            axesList[index].destroy();
                        }

                        for(var index=0;index<graphList.length;index++)
                        {
                            graphList[index].destroy();
                        }

                        axesList=[]
                        graphList=[]
                    }

                    function addSeries()
                    {
                        for(var index=0;index<graphList.length;index++)
                        {
                            graphList.pop()
                        }
                        for(var index=0;index<graph.noOfGraphs();index++)
                        {
                            var series=createSeries(ChartView.SeriesTypeSpline,"test",axesList[axesList.length-1],axesList[index])
                            series.color=graph.colorAtGraph(index)
                            series.name=graph.nameAtGraph(index)
                            series=graph.copy(series);
                            graphList.push(series);
                        }

                    }

                    function testplotGraph()
                    {
                        var retVal = 0 ;
                        var startFreqValue = 0 ;
                        var hornPsFreq = 0 ;
                        for(var index=0;index<graphList.length;index++)
                        {
                            graph.replaceSample(index);

                            if(graph.XAxisValue==0)
                            {
                                if(index!=graphList.length-1)
                                {
                                    graphParamViewTest.model[index].parameterval = graph.getMin(graphList[index]).y.toFixed(0)
                                }
                                else
                                {
                                    graphParamViewTest.model[index].parameterval = graph.getMin(graphList[index]).y.toFixed(4)
                                }
                            }
                        }

                        for(var index=0;index<axesList.length-1;index++)
                        {
                            var strName = graph.nameAtGraph(index)
                            if(strName == "PhasePlot")
                                graph.setMin_Max(axesList[index],-180,180)
                            else if(strName == "AmpPlot")
                                graph.setMin_Max(axesList[index],0,120)
                            else if(strName == "CurrentPlot")
                                graph.setMin_Max(axesList[index],0,120)
                            else
                                graph.setMin_Max(axesList[index],graph.getMinPoint(index).y,graph.getMaxPoint(index).y)
                            if(index==0)
                            {
                                axesList[index].labelFormat="%d"
                            }
                        }
                        if(axesList.length-1>=0)
                        {
                            axesList[axesList.length-1].min=graph.getMin(graphList[0]).x
                            axesList[axesList.length-1].max=graph.getMax(graphList[0]).x
                            axesList[axesList.length-1].labelFormat="%.5f"
                            testgraphSlider.xAxisVal = axesList[axesList.length-1].min ;
                            testgraphSlider.timeLabel = (axesList[axesList.length-1].min).toFixed(5);
                            graphNavigatorLabel.text=testgraphSlider.timeLabel +xAxisUnit
                        }
                    }
                    function resetCheckBoxValues()
                    {
                        var timePoint = testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min
                        var retVal = testgetClosestPoints(timePoint);

                        if(0 == retVal)
                        {
                            for(var index=0;index<testgraphChartItem.graphList.length;index++){

                                if(testgraphNavigatorMouseArea.indexClosest1.toFixed(0)>=0&&testgraphNavigatorMouseArea.indexClosest1.toFixed(0)<testgraphChartItem.graphList[index].count-1&&testgraphNavigatorMouseArea.indexClosest2.toFixed(0)>=0&&testgraphNavigatorMouseArea.indexClosest2.toFixed(0)<testgraphChartItem.graphList[index].count-1)
                                {
                                    var tmpPoint1=graph.getAtIndex(testgraphChartItem.graphList[index],testgraphNavigatorMouseArea.indexClosest1.toFixed(0))
                                    var tmpPoint2=graph.getAtIndex(testgraphChartItem.graphList[index],testgraphNavigatorMouseArea.indexClosest2.toFixed(0))
                                    var graphPoint=tmpPoint1.y + ((timePoint - tmpPoint1.x) * ((tmpPoint1.y - tmpPoint2.y) / (tmpPoint1.x - tmpPoint2.x)));
                                    if(index!=testgraphChartItem.graphList.length-1)
                                    {
                                        graphParamViewTest.model[index].parameterval = graphPoint.toFixed(0)
                                    }
                                    else
                                    {
                                        graphParamViewTest.model[index].parameterval = graphPoint.toFixed(4)

                                    }
                                }
                            }
                        }
                    }
                    Rectangle
                    {
                        id: testchartViewArea
                        x: testgraphChartItem.plotArea.x
                        y: testgraphChartItem.plotArea.y - ( testgraphNavigatorImage.height +  graphNavigatorLabel.height + testgraphSlider.raiseHeight)
                        height : testgraphNavigator.height + testgraphNavigatorImage.height + graphNavigatorLabel.height
                        width : testgraphChartItem.plotArea.width
                        visible: false
                    }
                    Item
                    {
                        id: testgraphSlider

                        property int raiseHeight: 0
                        property string custColor : qmlPrimaryColor
                        property int assetSize:32
                        property real xAxisVal:testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min ;
                        property string timeLabel:"0"

                        y: testgraphChartItem.plotStartPointY - ( testgraphNavigatorImage.height +  graphNavigatorLabel.height + raiseHeight)
                        x: testgraphChartItem.plotStartPointX + (xAxisVal - testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min) * testgraphChartItem.deltaX - width / 2
                        width : graphNavigatorLabel.width
                        height : testgraphNavigator.height + testgraphNavigatorImage.height + graphNavigatorLabel.height

                        Rectangle
                        {
                            id: testgraphNavigator
                            //border.width: 1
                            height: testgraphChartItem.plotArea.height + parent.raiseHeight
                            width:1
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: parent.custColor
                        }

                        Rectangle {
                            id: testgraphNavigatorImage
                            height: parent.assetSize * fontSizeScaleFactor
                            width : parent.assetSize * fontSizeScaleFactor
                            radius: width / 2
                            color: parent.custColor
                            anchors.bottom: testgraphNavigator.top
                            anchors.horizontalCenter: parent.horizontalCenter

                            MouseArea
                            {
                                id: testgraphNavigatorMouseArea

                                property point posInChartArea
                                property int indexClosest1:0
                                property int indexClosest2:0

                                anchors.fill: parent
                                cursorShape: Qt.ToolTip
                                hoverEnabled: false
                                onPositionChanged:
                                {

                                    var point = mapToItem(testchartViewArea , mouse.x , mouse.y);
                                    var retVal ;
                                    var timePoint;
                                    var phasePoint = 0.0;
                                    var ampPoint = 0 ;
                                    var currentPoint = 0 ;
                                    var impedancePoint = 0;

                                    if(point.x < 0 || point.x > testgraphChartItem.plotArea.width)
                                    {
                                        if(point.x < 0)
                                        {
                                            posInChartArea.x = 0 ;
                                            timePoint = testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min
                                        }
                                        else
                                        {
                                            posInChartArea.x = testgraphChartItem.plotArea.width ;
                                            timePoint = testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].max
                                            testgraphSlider.xAxisVal = (testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min
                                                                        + posInChartArea.x / testgraphChartItem.deltaX );
                                            retVal = testgetClosestPoints(timePoint.toFixed(5));
                                            if(0 == retVal)
                                            {
                                                for(var index=0;index<testgraphChartItem.graphList.length;index++)
                                                {
                                                    if(index!=testgraphChartItem.graphList.length-1)
                                                    {
                                                        graphParamViewTest.model[index].parameterval = graph.getMax(testgraphChartItem.graphList[index]).y
                                                    }
                                                    else
                                                    {
                                                        graphParamViewTest.model[index].parameterval = (graph.getMax(testgraphChartItem.graphList[index]).y).toFixed(4)
                                                    }
                                                }
                                                graph.XAxisValue=testgraphSlider.xAxisVal;
                                                testgraphSlider.timeLabel = (timePoint).toFixed(5);
                                                graph.GraphTime=(timePoint).toFixed(5);
                                                graphNavigatorLabel.text=graph.GraphTime +xAxisUnit
                                                return
                                            }
                                        }
                                    }
                                    else
                                    {
                                        posInChartArea.x = point.x;
                                        posInChartArea.y = point.y;
                                        timePoint = posInChartArea.x / testgraphChartItem.deltaX + testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min
                                    }

                                    testgraphSlider.xAxisVal = (testgraphChartItem.axesList[testgraphChartItem.axesList.length-1].min
                                                                + posInChartArea.x / testgraphChartItem.deltaX );
                                    retVal = testgetClosestPoints(timePoint);

                                    if(0 == retVal){

                                        for(var index=0;index<testgraphChartItem.graphList.length;index++)
                                        {
                                            if(indexClosest1.toFixed(0)>=0&&indexClosest1.toFixed(0)<=testgraphChartItem.graphList[index].count-1&&
                                                    indexClosest2.toFixed(0)>=0&&indexClosest2.toFixed(0)<=testgraphChartItem.graphList[index].count-1)
                                            {
                                                if(point.x > testgraphChartItem.plotArea.width )
                                                {
                                                    if(index!=testgraphChartItem.graphList.length-1)
                                                    {
                                                        graphParamViewTest.model[index].parameterval = graph.getMax(testgraphChartItem.graphList[index]).y
                                                    }
                                                    else
                                                    {
                                                        graphParamViewTest.model[index].parameterval = graph.getMax(testgraphChartItem.graphList[index]).y
                                                    }
                                                }

                                                else
                                                {
                                                    var tmpPoint1=graph.getAtIndex(testgraphChartItem.graphList[index],indexClosest1.toFixed(0))
                                                    var tmpPoint2=graph.getAtIndex(testgraphChartItem.graphList[index],indexClosest2.toFixed(0))
                                                    var graphPoint=tmpPoint1.y + ((timePoint - tmpPoint1.x) * ((tmpPoint1.y - tmpPoint2.y) / (tmpPoint1.x - tmpPoint2.x)));

                                                    if(index!=testgraphChartItem.graphList.length-1)
                                                    {
                                                        graphParamViewTest.model[index].parameterval = graphPoint.toFixed(0)
                                                    }

                                                    else
                                                    {
                                                        graphParamViewTest.model[index].parameterval = graphPoint.toFixed(4)
                                                    }
                                                }
                                            }
                                        }

                                        graph.XAxisValue=testgraphSlider.xAxisVal;
                                        testgraphSlider.timeLabel = (timePoint).toFixed(5);
                                        graph.GraphTime=(timePoint).toFixed(5);
                                        graphNavigatorLabel.text=graph.GraphTime +xAxisUnit
                                    }
                                }
                            }
                        }

                        Text
                        {
                            id: graphNavigatorLabel
                            anchors.bottom: testgraphNavigatorImage.top
                            anchors.bottomMargin: 1
                            height :  font.pixelSize + 6
                            width: font.pixelSize * 12
                            anchors.horizontalCenter: testgraphSlider.horizontalCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family : Style.semibold.name
                            font.pixelSize: Style.style4
                            text: testgraphSlider.timeLabel +xAxisUnit
                            color: parent.custColor
                        }

                    }
                    function graphPropertyVisible(rightParamIndex , dropDownOption , checkState)
                    {
                        if(rightParamIndex < 0 || dropDownOption < 0 || checkState < 0 || axesList.length === 0)
                        {
                            return
                        }
                        ultrasonictest.setAxisIndex(rightParamIndex,dropDownOption)
                        ultrasonictest.setCheckboxStatus(rightParamIndex,checkState)

                        if(dropDownOption === 1)
                        {
                            axesList[rightParamIndex].destroy()
                            axesList[rightParamIndex] = Qt.createQmlObject('import QtQuick 2.0; import QtCharts 2.0; ValueAxis { titleText: " " }', testgraphChartItem);
                            axesList[rightParamIndex].titleText = graph.nameAtAxis(rightParamIndex)
                            axesList[rightParamIndex].titleVisible = false
                            graphList[rightParamIndex].axisYRight=axesList[rightParamIndex]
                            graphList[rightParamIndex].axisX=axesList[graphList.length]

                        }
                        if(dropDownOption === 0)
                        {
                            axesList[rightParamIndex].destroy()
                            axesList[rightParamIndex]=Qt.createQmlObject('import QtQuick 2.0; import QtCharts 2.0; ValueAxis { titleText: " " }', testgraphChartItem);
                            axesList[rightParamIndex].titleText = graph.nameAtAxis(rightParamIndex)
                            axesList[rightParamIndex].titleVisible = false
                            graphList[rightParamIndex].axisY=axesList[rightParamIndex]
                            graphList[rightParamIndex].axisX=axesList[graphList.length]

                        }
                        graph.setVisibility(graphList[rightParamIndex],axesList[rightParamIndex],checkState)

                        if(responseForGraph === 0)
                        {
                            var strName = graph.nameAtGraph(rightParamIndex)
                            if(strName === "PhasePlot")
                                graph.setMin_Max(axesList[rightParamIndex],-180,180)
                            else if(strName === "AmpPlot")
                                graph.setMin_Max(axesList[rightParamIndex],0,120)
                            else if(strName === "CurrentPlot")
                                graph.setMin_Max(axesList[rightParamIndex],0,120)
                            else
                                graph.setMin_Max(axesList[rightParamIndex],graph.getMinPoint(rightParamIndex).y,graph.getMaxPoint(rightParamIndex).y)
                        }
                        if(rightParamIndex === 0)
                        {
                            axesList[rightParamIndex].labelFormat="%d"
                        }
                        axesList[rightParamIndex].gridVisible= false
                        axesList[rightParamIndex].minorGridVisible= false
                        axesList[rightParamIndex].minorTickCount=0
                        axesList[axesList.length-1].tickCount =5
                        axesList[rightParamIndex].color=graph.colorAtAxis(rightParamIndex)
                        axesList[rightParamIndex].labelsColor=graph.colorAtAxis(rightParamIndex)
                    }
                }//chartview end
            }

            // rectangleRightDown
        } // rectangle right

        Rectangle
        {
            id:rightRect
            color: "#ECECEC"
            width: window.width * 0.26
            height: (parent.height - (rectangleLeftTop.height + spaceBetweenPanels) ) - mainStatusbarHeight
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.topMargin: parent.height/3
            Text {
                id: status
                color: "#868e96"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                font.pixelSize: Style.style6
                font.family: Style.semibold.name
                text: ultrasonictest.StatusText;

            }
            GridView
            {
                id:graphParamViewTest
                property int paramIndex:1
                width:parent.width
                height: parent.height * 0.5
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.AutoFlickDirection
                clip:true
                cellWidth: graphParamViewTest.width
                cellHeight: 50
                anchors.right: parent.right
                anchors.rightMargin: (10/500)*parent.width
                anchors.left: parent.left
                anchors.leftMargin: (10/500)*parent.width
                anchors.bottom: parent.bottom
                anchors.bottomMargin: (20/680)*parent.height
                anchors.top: parent.top
                anchors.topMargin: (70/680 )*parent.height
                model:testCheckboxModel
                property var refreshCombo: false
                property var refreshCheckBox: false
                delegate:  GraphRightParamDelegate
                {
                    id: graphRightParamDelegate
                    width : graphParamViewTest.width
                    height: 50
                    comboBox.onActivated:
                    {
                        graphParamViewTest.refreshCombo=true
                    }

                    checkBox.onPressedChanged:
                    {
                        graphParamViewTest.refreshCheckBox=true
                    }

                    comboBox.onCurrentIndexChanged:
                    {
                        testgraphChartItem.graphPropertyVisible(index , comboBox.currentIndex , checkBox.checked);

                        if(graphParamViewTest.refreshCombo==true)
                        {
                            graphParamViewTest.refreshCombo=false
                            ultrasonictest.setCheckBoxModelData()
                        }

                    }

                    checkBox.onCheckedStateChanged :
                    {
                        testgraphChartItem.graphPropertyVisible(index , comboBox.currentIndex , checkBox.checked);

                        if(graphParamViewTest.refreshCheckBox==true)
                        {
                            graphParamViewTest.refreshCheckBox=false
                            ultrasonictest.setCheckBoxModelData()
                        }
                    }

                    Component.onCompleted:
                    {
                        graphParamIndex = index
                    }
                }
                Component.onCompleted:
                {
                    ultrasonictest.setTestDefaultValues()
                }
                Component.onDestruction:
                {
                    ultrasonictest.getTestDefaultValues()
                }

            }

        }
    }

    Text
    {
        id: text_UltraSonicTest
        x:20//rectangleLeftTop.x+rectangleLeftTop.width/2-width/2
        y:testThreeway.y+testThreeway.height/2-height/2
        color: "#868e96"
        text: qmltextUltrasonictest
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: Style.style6
        font.family: Style.semibold.name
    }
    function testgetClosestPoints(xval)
    {
        var length = testgraphChartItem.graphList[0].count;
        var point ;
        var startFreq = 0;
        var endFreq = 0;
        var freqStep = 0 ;
        var closestVal = 9999 ;
        var closestInd = 0;
        var tmpVal = 0 ;

        if(0 == length)
        {
            return -1 ;
        }
        startFreq=graph.getMin(testgraphChartItem.graphList[0]).x
        endFreq=graph.getMax(testgraphChartItem.graphList[0]).x

        if((xval < startFreq) || (xval > endFreq))
        {
            return -1;
        }

        var point0 = graph.getAtIndex(testgraphChartItem.graphList[0],1)
        freqStep = point0.x - startFreq ;
        closestInd = (xval - startFreq) / freqStep;
        closestVal = 0;
        point0 = graph.getAtIndex(testgraphChartItem.graphList[0],closestInd);

        {
            testgraphNavigatorMouseArea.indexClosest1 = closestInd;
            testgraphNavigatorMouseArea.indexClosest2 = closestInd+1;
        }
        return 0;
    }

    TestAdvanced
    {
        id:id_testadvanced
        width: parent.width
        height: parent.height
        Component.onCompleted:
        {
            visible=false
        }
    }
}
