import QtQuick 2.8
import QtCharts 2.1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Style 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0



Item {
    id: trendsGraph
    //        width: 1020
    //        height: 750
    width:parent.width
    height: parent.height
    readonly property string qmltextChooseParameter : qsTr("CHOOSE PARAMETER")
    readonly property string qmltextChooseParameterDes :qsTr("Select parameters to graph")
    readonly property string qmltextDone :qsTr("DONE")

    //axis counts
    property int xAxisCount:0//trendsGraphQmlObj.getXCount()
    property int yAxisCount: 0//trendsGraphQmlObj.tickCountY


    //parameter flags
    property int maxWeldForce_flag:0
    property int maxHoldForce_flag:0
    property int totalAbsolute_flag:0
    property int partContactPosition_flag:0
    property int weldTime_flag:0
    property int weldEnergy_flag:0
    property int weldPeakPower_flag:0
    property int startFrequency_flag:0
    property int frequencyChange_flag:0
    property int amplitude_flag:0
    //parameter limits
    property int limit_1:0

    property int time_suspect_limit_low:0
    property int time_suspect_limit_high:0
    property int time_reject_limit_low:0
    property int time_reject_limit_high: 0

    property int energy_suspect_limit_low:0
    property int energy_suspect_limit_high:0
    property int energy_reject_limit_low:0
    property int energy_reject_limit_high:0

    property int absolutdist_suspect_limit_low:0
    property int absolutdist_suspect_limit_high:0
    property int absolutdist_reject_limit_low:0
    property int absolutdist_reject_limit_high:0

    property int peakpower_suspect_limit_low:0
    property int peakpower_suspect_limit_high:0
    property int peakpower_reject_limit_low:0
    property int peakpower_reject_limit_high:0

    property int frequency_suspect_limit_low:0
    property int frequency_suspect_limit_high:0
    property int frequency_reject_limit_low:0
    property int frequency_reject_limit_high:0

    property int maxweldforce_suspect_limit_low:0
    property int maxweldforce_suspect_limit_high:0
    property int maxweldforce_reject_limit_low:0
    property int maxweldforce_reject_limit_high:0

    //    delegate chart height
    property int delegateChartHeight:   80
    property var temp
    property int chartViewVisibleWidth:   500
    property  bool bvisible : false

    property int tempModel:1

    property var comp
    property  var rect1
    property  var rectTemp2
    property bool firstTimeRun:false

    property int count:0//for testing circle

    /**
      *@breif: plot trendsgraph for selected recipe
      **/
    function plotTrendsGraph(recipeNumber)
    {
        trendsGraphQmlObj.trendsGraphData(recipeNumber)
        xAxisCount=trendsGraphQmlObj.getXCount()
        yAxisCount=trendsGraphQmlObj.tickCountY
        refreshGraph(recipeNumber)
    }
    /**
      *@breif: refresh and plot trendsgraph for selected recipe
      **/
    function refreshGraph(recipeNumber)
    {
        trendsGraphQmlObj.trendsGraphData(recipeNumber)
        xAxisCount=trendsGraphQmlObj.getXCount()
        yAxisCount=trendsGraphQmlObj.tickCountY
    }

    /**
      *@breif: scale format to display Y axis values
      **/
    function getScaleFormat(maxNum)
    {
        if(maxNum>=100)
            return "%06.0f"
        else if(maxNum<=9)
            return "%.4f"
        else if(maxNum>99999)
            return "%.0f"
        else
            return "%.3f"
    }

    Component.onCompleted:
    {
        trendsGraphLoadTimer.start()

    }
    Rectangle {
        id: trendGraphParentRect
        width:parent.width
        height: parent.height

        //Top scale Rectangle
        Rectangle {
            id:rectTopScale
            width:parent.width * (80/100)
            height: parent.height * (12/100)
            clip:true

            //Top Scale Chart List
            ListView {
                id: topScaleHorizontalList
                width: parent.width;
                height: parent.height
                anchors.bottom: rectTopScale.bottom
                anchors.bottomMargin: 2
                orientation: ListView.Horizontal
                anchors.left: parent.left
                anchors.leftMargin: 55
                model:1
                clip:true

                delegate:

                    ChartView {
                    id:topScaleChart
                    width:xAxisCount*100
                    height:2*parent.height
                    antialiasing: true
                    animationOptions: ChartView.NoAnimation
                    legend.visible:false
                    anchors.margins: 0
                    Canvas {
                        id: canvas
                        width:parent.width/2
                        height:parent.width/2

                        onPaint: {
                            var context = getContext('2d');
                            context.rect(65,30, 50,50);
                            context.fillStyle="white"
                            context.fill();
                        }
                    }
                    ValueAxis {
                        id: scaleAxisXTop
                        min:0
                        max:xAxisCount
                        tickCount: xAxisCount+1
                        labelFormat: "%i"
                        titleVisible: false
                        gridVisible: false
                        labelsColor:"#757575"
                        lineVisible:false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                    }
                    ValueAxis {
                        id: scaleAxisYTop
                        min:0
                        max: 1
                        tickCount: 1
                        titleVisible: false
                        gridVisible: false
                        labelsVisible: false
                        labelFormat: "%06.0f"
                        color: "white"
                    }
                    ScatterSeries {
                        id: scatterSeriesTop
                        axisXTop: scaleAxisXTop
                        axisY: scaleAxisYTop
                        color: "#898D96"

                    }

                    //alarm circles
                    Repeater
                    {
                        id:canvasCircleRepeater
                        model:cycleCounterModel

                        Item {
                            id:itemCanvas
                            anchors.fill: topScaleChart

                            Canvas {
                                id: id_canvas_progress
                                anchors.fill: parent
                                contextType: "2d"
                                antialiasing: true
                                visible:model.modelData.alarmFlag// trendsGraphQmlObj.getAlarmFlag(index)//
                                renderStrategy: Canvas.Threaded
                                renderTarget: Canvas.FramebufferObject
                                onPaint: {
                                    // Circle Part
                                    var context = getContext('2d');
                                    context.strokeStyle = "red"
                                    context.lineWidth = 1;
                                    context.beginPath();
                                    var cnt=trendsGraphQmlObj.getXCount();
                                    var temp=0
                                    if(cnt>0)
                                        temp=((cnt*100)-73-35)/cnt
                                    var localTemp=index+1
                                    var tempRes=temp

                                    context.arc(localTemp*tempRes+70, 42, 15,0, 15, false)//(id_rec1.height / 2, id_rec1.height / 2, id_rec1.height / 3, index * Math.PI * 2 * (360 / id_rec1.p_Blocks / 360) + Math.PI * 2 * 0.003, index * Math.PI * 2 * (360 / id_rec1.p_Blocks / 360) + Math.PI * 2 * (360 / id_rec1.p_Blocks / 360), false);
                                    context.stroke();

                                }
                            }
                        }
                    }
                }// top chartview end
                Component.onCompleted:
                {
                    topScaleHorizontalList.positionViewAtEnd()
                }

                ScrollBar.horizontal: hbar
            }
        }


        //
        Rectangle {
            id:rectTopScaleEmpty
            width:parent.width * (20/100)
            height: parent.height * (12/100)
            anchors.right: parent.right
        }

        //Graph display chart and right parameter
        Rectangle{
            id: chartViewRect
            width: parent.width
            height: parent.height * (88/100)
            anchors.top: rectTopScale.bottom

            //vertical list with graph and parameter
            ListView {
                id: verticalList
                width: parent.width
                height: parent.height
                spacing: 0
                clip:true
                orientation: ListView.Vertical
                model: trendsModel
                property var activeIndex : 0
                property var lastContentX : 0

                delegate:
                    Rectangle{
                    id:rect
                    width:parent.width
                    height: delegateChartHeight
                    clip: true
                    Rectangle{
                        id:rect1
                        width: parent.width * (80/100)
                        height:parent.height
                        //chartview for showing Y axis at left side
                        ChartView{
                            id:sideChartView
                            width:100
                            y:-45
                            x:10
                            height:parent.height+76
                            antialiasing: true
                            animationOptions: ChartView.SeriesAnimations
                            legend.visible:false
                            visible: false

                            ValueAxis {
                                id: sideScaleAxisX
                                min: 0
                                max:5
                                tickCount: 1
                                labelFormat: "%.0f"
                                titleVisible: false
                                lineVisible: false
                                labelsVisible: false
                                labelsColor:"black"
                                gridVisible: false
                            }
                            ValueAxis {
                                id: sideScaleAxisY
                                min: 0
                                max: 1.0
                                tickCount: 6
                                labelFormat: "%.1f"//"%.0f"
                                titleVisible: true
                                gridVisible: false
                                labelsVisible: true
                            }

                            ScatterSeries {
                                id: scatterSeriesSide
                                axisXTop:   sideScaleAxisX
                                axisY: sideScaleAxisY
                                color: "#898D96"
                            }
                        }

                        //horizonatal list with chartview
                        ListView {
                            id: horizontalList
                            width: parent.width;
                            height: parent.height;
                            spacing: 0
                            orientation: ListView.Horizontal
                            model:1
                            clip:true
                            anchors.left: parent.left
                            anchors.leftMargin: 55
                            property var verticalIndex : index
                            property var activeIndex_H : 0
                            property var lastContentX_H : 0

                            delegate:

                                ChartView {
                                id:time_chart
                                y:-45
                                width:xAxisCount*100
                                height:parent.height+76
                                antialiasing: true
                                animationOptions: ChartView.NoAnimation
                                legend.visible:false

                                //orange rect top
                                Rectangle
                                {
                                    id:rect_orange_top
                                    anchors.top:parent.top
                                    anchors.topMargin: 45
                                    anchors.left: parent.left
                                    anchors.leftMargin:70
                                    width: time_chart.width-105
                                    height:(delegateChartHeight+100)/yAxisCount
                                    color: "#F03E3E"
                                    visible: false
                                    opacity: 0.4
                                }
                                //orange rect bottom
                                Rectangle
                                {
                                    id:rect_orange_bottom
                                    anchors.left: parent.left
                                    anchors.leftMargin: rect_orange_top.x
                                    width: rect_orange_top.width
                                    height:rect_orange_top.height
                                    color: "#F03E3E"
                                    visible: false
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 35
                                    opacity: 0.4
                                }
                                //yellow rect top
                                Rectangle
                                {
                                    id:rect_yellow_top
                                    anchors.left: parent.left
                                    anchors.leftMargin: rect_orange_top.x
                                    anchors.top: rect_orange_top.bottom
                                    width: rect_orange_top.width
                                    height:rect_orange_top.height
                                    visible: false
                                    color: "#F59F00"
                                    opacity: 0.4
                                }
                                //yellow rect bottom
                                Rectangle
                                {
                                    id:rect_yellow_bottom
                                    anchors.left: parent.left
                                    anchors.leftMargin:rect_orange_top.x
                                    width: rect_orange_top.width
                                    height:rect_orange_top.height
                                    color: "#F59F00"
                                    visible: false
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 35+rect_orange_bottom.height
                                    opacity: 0.4
                                }
                                ValueAxis {
                                    id: scaleAxisX
                                    min: 0
                                    max:xAxisCount
                                    labelsVisible: false
                                    tickCount:xAxisCount+1
                                    labelFormat: "%i"
                                    titleVisible: false
                                    gridVisible: true
                                }
                                ValueAxis {
                                    id: scaleAxisY
                                    min: 0
                                    max: 1
                                    tickCount: yAxisCount
                                    titleVisible: false
                                    gridVisible: false
                                    labelsVisible: false
                                    //labelFormat: ("%s")
                                    labelFormat: getScaleFormat(max)
                                    labelsColor: "#757575"
                                }
                                ScatterSeries {
                                    id: scatterSeries
                                    axisXTop:   scaleAxisX
                                    axisY: scaleAxisY
                                    color: "#898D96"
                                    markerSize: 10
                                    useOpenGL: false
                                }
                                ScatterSeries {
                                    id: scatterSeriesRedPoint
                                    markerSize:10
                                    color:"#F03E3E"
                                    useOpenGL: false
                                }
                                ScatterSeries {
                                    id: scatterSeriesYellowPoint
                                    markerSize:10
                                    color:"#F59F00"
                                    useOpenGL: false
                                }
                                LineSeries {
                                    id: lineSeries
                                    axisXTop:   scaleAxisX
                                    axisY: scaleAxisY
                                    color: qmlPrimaryColor
                                    useOpenGL: true
                                }


                                Component.onCompleted:
                                {

                                    if(horizontalList.verticalIndex===0){

                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        maxweldforce_suspect_limit_low=trendsGraphQmlObj.maxWeldForceSuspectLimitLow//10
                                        maxweldforce_suspect_limit_high=trendsGraphQmlObj.maxWeldForceSuspectLimitHigh//80
                                        maxweldforce_reject_limit_low=trendsGraphQmlObj.maxWeldForceRejectLimitLow//20
                                        maxweldforce_reject_limit_high=trendsGraphQmlObj.maxWeldForceRejectLimitHigh//70

                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)

                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)
                                            if((maxweldforce_suspect_limit_high==0) && (maxweldforce_suspect_limit_low==0) && (maxweldforce_reject_limit_low==0)&& (maxweldforce_reject_limit_high==0))
                                            {
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                            else
                                            {
                                                if(point.y<maxweldforce_suspect_limit_low && point.y>maxweldforce_reject_limit_low || point.y>maxweldforce_suspect_limit_high && point.y<maxweldforce_reject_limit_high )
                                                {
                                                    maxWeldForce_flag=1;
                                                    scatterSeriesYellowPoint.append(point.x, point.y);
                                                }
                                                if(point.y<=maxweldforce_reject_limit_low || point.y>=maxweldforce_reject_limit_high)
                                                {
                                                    maxWeldForce_flag=1;
                                                    scatterSeriesRedPoint.append(point.x, point.y);
                                                }
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===1)
                                    {
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)
                                            scatterSeries.append(point.x, point.y);
                                            lineSeries.append(point.x, point.y);
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===2){
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        absolutdist_suspect_limit_low=trendsGraphQmlObj.absoluteDistSuspectLimitLow
                                        absolutdist_suspect_limit_high=trendsGraphQmlObj.absoluteDistSuspectLimitHigh
                                        absolutdist_reject_limit_low=trendsGraphQmlObj.absoluteDistRejectLimitLow
                                        absolutdist_reject_limit_high=trendsGraphQmlObj.absoluteDistRejectLimitHigh

                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)
                                            if((absolutdist_suspect_limit_high==0) && (absolutdist_suspect_limit_low==0) && (absolutdist_reject_limit_low==0)&& (absolutdist_reject_limit_high==0))
                                            {
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                            else
                                            {
                                                if( point.y<absolutdist_suspect_limit_low &&  point.y>absolutdist_reject_limit_low || point.y>absolutdist_suspect_limit_high && point.y<absolutdist_reject_limit_high )
                                                {
                                                    totalAbsolute_flag=1;
                                                    scatterSeriesYellowPoint.append(point.x, point.y);
                                                }
                                                if( point.y<=absolutdist_reject_limit_low || point.y>=absolutdist_reject_limit_high)
                                                {
                                                    totalAbsolute_flag=1;
                                                    scatterSeriesRedPoint.append(point.x, point.y);
                                                }
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===3)
                                    {
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)
                                            scatterSeries.append(point.x, point.y);
                                            lineSeries.append(point.x, point.y);
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===4){
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        time_suspect_limit_low=trendsGraphQmlObj.timeSuspectLimitLow
                                        time_suspect_limit_high=trendsGraphQmlObj.timeSuspectLimitHigh
                                        time_reject_limit_low=trendsGraphQmlObj.timeRejectLimitLow
                                        time_reject_limit_high=trendsGraphQmlObj.timeRejectLimitHigh

                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)
                                            if((time_suspect_limit_high==0) && (time_suspect_limit_low==0) && (time_reject_limit_low==0)&& (time_reject_limit_high==0))
                                            {
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                            else
                                            {
                                                if( point.y<time_suspect_limit_low && point.y>time_reject_limit_low || point.y>time_suspect_limit_high && point.y<time_reject_limit_high)
                                                {
                                                    weldTime_flag=1;
                                                    scatterSeriesYellowPoint.append(point.x, point.y);
                                                }
                                                if( point.y<=time_reject_limit_low || point.y>=time_reject_limit_high)
                                                {
                                                    weldTime_flag=1;
                                                    scatterSeriesRedPoint.append(point.x, point.y);
                                                }
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===5){
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        energy_suspect_limit_low=trendsGraphQmlObj.energySuspectLimitLow
                                        energy_suspect_limit_high=trendsGraphQmlObj.energySuspectLimitHigh
                                        energy_reject_limit_low=trendsGraphQmlObj.energyRejectLimitLow
                                        energy_reject_limit_high=trendsGraphQmlObj.energyRejectLimitHigh

                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)

                                            if((maxweldforce_suspect_limit_high==0) && (maxweldforce_suspect_limit_low==0) && (maxweldforce_reject_limit_low==0)&& (maxweldforce_reject_limit_high==0))
                                            {
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                            else
                                            {
                                                if( point.y<energy_suspect_limit_low && point.y>energy_reject_limit_low || point.y>energy_suspect_limit_high && point.y<energy_reject_limit_high)
                                                {
                                                    weldEnergy_flag=1;
                                                    scatterSeriesYellowPoint.append(point.x, point.y);
                                                }
                                                if( point.y<=energy_reject_limit_low || point.y>=energy_reject_limit_high)
                                                {
                                                    weldEnergy_flag=1;
                                                    scatterSeriesRedPoint.append(point.x, point.y);
                                                }
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===6){
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        peakpower_suspect_limit_low=trendsGraphQmlObj.peakPowerSuspectLimitLow
                                        peakpower_suspect_limit_high=trendsGraphQmlObj.peakPowerSuspectLimitHigh
                                        peakpower_reject_limit_low=trendsGraphQmlObj.peakPowerRejectLimitLow
                                        peakpower_reject_limit_high=trendsGraphQmlObj.peakPowerRejectLimitHigh

                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)

                                            if((peakpower_suspect_limit_high==0) && (peakpower_suspect_limit_low==0) && (peakpower_reject_limit_low==0)&& (peakpower_reject_limit_high==0))
                                            {
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                            else
                                            {
                                                if( point.y<peakpower_suspect_limit_low && point.y>peakpower_reject_limit_low || point.y>peakpower_suspect_limit_high && point.y<peakpower_reject_limit_high)
                                                {
                                                    weldPeakPower_flag=1;
                                                    scatterSeriesYellowPoint.append(point.x, point.y);
                                                }
                                                if(point.y<=peakpower_reject_limit_low || point.y>=peakpower_reject_limit_high)
                                                {
                                                    weldPeakPower_flag=1;
                                                    scatterSeriesRedPoint.append(point.x, point.y);
                                                }
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===7){
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        frequency_suspect_limit_low=trendsGraphQmlObj.frequencySuspectLimitLow
                                        frequency_suspect_limit_high=trendsGraphQmlObj.frequencySuspectLimitHigh
                                        frequency_reject_limit_low=trendsGraphQmlObj.frequencyRejectLimitLow
                                        frequency_reject_limit_high=trendsGraphQmlObj.frequencyRejectLimitHigh

                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)

                                            if((frequency_suspect_limit_high==0) && (frequency_suspect_limit_low==0) && (frequency_reject_limit_low==0)&& (frequency_reject_limit_high==0))
                                            {
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                            else
                                            {
                                                if( point.y<frequency_suspect_limit_low && point.y>frequency_reject_limit_low || point.y>frequency_suspect_limit_high && point.y<frequency_reject_limit_high)
                                                {
                                                    startFrequency_flag=1;
                                                    scatterSeriesYellowPoint.append(point.x, point.y);
                                                }
                                                if(point.y<=frequency_reject_limit_low || point.y>=frequency_reject_limit_high)
                                                {
                                                    startFrequency_flag=1;
                                                    scatterSeriesRedPoint.append(point.x, point.y);
                                                }
                                                scatterSeries.append(point.x, point.y);
                                                lineSeries.append(point.x, point.y);
                                            }
                                        }
                                    }
                                    else if(horizontalList.verticalIndex===8){
                                        scaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        scaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.max=trendsGraphQmlObj.getYAxisMaxValues(horizontalList.verticalIndex)
                                        sideScaleAxisY.min=trendsGraphQmlObj.getYAxisMinValues(horizontalList.verticalIndex)
                                        for (var j=0; j<trendsGraphQmlObj.getXCount(); j++)
                                        {
                                            var point = trendsGraphQmlObj.getGraphValueByIndex(j,horizontalList.verticalIndex)
                                            scatterSeries.append(point.x, point.y);
                                            lineSeries.append(point.x, point.y);
                                        }
                                    }
                                    else{}

                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked:  {
                                        if(horizontalList.verticalIndex===0){
                                            if(maxWeldForce_flag===1){
                                                if(rect.height===delegateChartHeight){
                                                    suspect_limit_high.text=qsTr(" "+maxweldforce_suspect_limit_high)
                                                    suspect_limit_low.text=qsTr(" "+maxweldforce_suspect_limit_low)
                                                    reject_limit_high.text=qsTr(" "+maxweldforce_reject_limit_high)
                                                    reject_limit_low.text=qsTr(" "+maxweldforce_reject_limit_low)
                                                    zoomInGraph();
                                                }
                                                else {
                                                    zoomOutGraph();
                                                }
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===1){
                                            if(maxHoldForce_flag===1){
                                                if(rect.height===delegateChartHeight){
                                                    zoomInGraph();
                                                }
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===2){
                                            if(totalAbsolute_flag===1){
                                                if(rect.height===delegateChartHeight){
                                                    suspect_limit_high.text=qsTr(" "+absolutdist_suspect_limit_high)
                                                    suspect_limit_low.text=qsTr(" "+absolutdist_suspect_limit_low)
                                                    reject_limit_high.text=qsTr(" "+absolutdist_reject_limit_high)
                                                    reject_limit_low.text=qsTr(" "+absolutdist_reject_limit_low)
                                                    zoomInGraph();
                                                }
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===3){
                                            if(partContactPosition_flag===1){
                                                if(rect.height===delegateChartHeight)
                                                    zoomInGraph();
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===4){
                                            if(weldTime_flag===1){
                                                if(rect.height===delegateChartHeight){
                                                    suspect_limit_high.text=qsTr(" "+time_suspect_limit_high)
                                                    suspect_limit_low.text=qsTr(" "+time_suspect_limit_low)
                                                    reject_limit_high.text=qsTr(" "+time_reject_limit_high)
                                                    reject_limit_low.text=qsTr(" "+time_reject_limit_low)
                                                    zoomInGraph();
                                                }
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===5){
                                            if(weldEnergy_flag===1){
                                                if(rect.height===delegateChartHeight){
                                                    suspect_limit_high.text=qsTr(" "+energy_suspect_limit_high)
                                                    suspect_limit_low.text=qsTr(" "+energy_suspect_limit_low)
                                                    reject_limit_high.text=qsTr(" "+energy_reject_limit_high)
                                                    reject_limit_low.text=qsTr(" "+energy_reject_limit_low)

                                                    zoomInGraph();
                                                }
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===6){
                                            if(weldPeakPower_flag===1){
                                                if(rect.height===delegateChartHeight){
                                                    suspect_limit_high.text=qsTr(" "+peakpower_suspect_limit_high)
                                                    suspect_limit_low.text=qsTr(" "+peakpower_suspect_limit_low)
                                                    reject_limit_high.text=qsTr(" "+peakpower_reject_limit_high)
                                                    reject_limit_low.text=qsTr(" "+peakpower_reject_limit_low)
                                                    zoomInGraph();
                                                }
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===7){
                                            if(startFrequency_flag===1){
                                                if(rect.height===delegateChartHeight){
                                                    suspect_limit_high.text=qsTr(" "+frequency_suspect_limit_high)
                                                    suspect_limit_low.text=qsTr(" "+frequency_suspect_limit_low)
                                                    reject_limit_high.text=qsTr(" "+frequency_reject_limit_high)
                                                    reject_limit_low.text=qsTr(" "+frequency_reject_limit_low)
                                                    zoomInGraph();
                                                }
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===8){
                                            if(frequencyChange_flag===1){
                                                if(rect.height===delegateChartHeight)
                                                    zoomInGraph();
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else if(horizontalList.verticalIndex===9){
                                            if(amplitude_flag===1){
                                                if(rect.height===delegateChartHeight)
                                                    zoomInGraph();
                                                else
                                                    zoomOutGraph();
                                            }
                                        }
                                        else{}
                                    }
                                }

                                /**
                                  *@breif: zoom In graph on click
                                  **/
                                function zoomInGraph(){
                                    scatterSeriesRedPoint.visible=false;
                                    scatterSeriesYellowPoint.visible=false;
                                    rect.height=rect.height+100;


                                    rect_orange_top.visible=true;
                                    rect_orange_bottom.visible=true;
                                    rect_yellow_top.visible=true;
                                    rect_yellow_bottom.visible=true;
                                    //scaleAxisY.labelsVisible=true;
                                    suspect_limit_high_rect.visible=true;
                                    reject_limit_high_rect.visible=true;
                                    suspect_limit_low_rect.visible=true;
                                    reject_limit_low_rect.visible=true;
                                    sideChartView.visible=true
                                }

                                /**
                                  *@breif: zoom out graph on click
                                  **/
                                function zoomOutGraph(){
                                    scatterSeriesRedPoint.visible=true;
                                    scatterSeriesYellowPoint.visible=true;
                                    rect.height=rect.height-100;
                                    rect_orange_top.visible=false;
                                    rect_orange_bottom.visible=false;
                                    rect_yellow_top.visible=false;
                                    rect_yellow_bottom.visible=false;
                                    scaleAxisY.labelsVisible=false;
                                    suspect_limit_high_rect.visible=false;
                                    reject_limit_high_rect.visible=false;
                                    suspect_limit_low_rect.visible=false;
                                    reject_limit_low_rect.visible=false;
                                    sideChartView.visible=false
                                }
                            }

                            onContentXChanged:
                            {
                                if(model.index === verticalList.activeIndex)
                                {
                                    verticalList.lastContentX = contentX
                                }
                            }
                            onMovementStarted:
                            {
                                verticalList.activeIndex = model.index
                            }
                            Component.onCompleted:
                            {
                                horizontalList.positionViewAtEnd()
                            }

                            ScrollBar.horizontal: hbar
                        }
                    }

                    //right side parameters
                    Rectangle{
                        id:rect2
                        width:parent.width * (20/100)
                        height:parent.height
                        anchors.right: parent.right
                        border.color: "#cbcbcb"
                        Text
                        {
                            id:paramtext
                            width: parent.width
                            height: parent.height
                            text:model.modelData.parameterName
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: "#757575"
                        }
                        Rectangle{
                            id:reject_limit_high_rect
                            width:parent.width
                            height:(delegateChartHeight+100)/yAxisCount
                            anchors.top:parent.top
                            anchors.topMargin: 1
                            anchors.left: parent.left
                            anchors.leftMargin:  1
                            x:1
                            visible: false

                            Text
                            {
                                id:reject_limit_high
                                x:0
                                width: parent.width
                                height:parent.height
                                text: qsTr("____"+limit_1)
                                color: "#757575"
                                verticalAlignment: Text.AlignBottom

                            }
                        }
                        Rectangle{
                            id:suspect_limit_high_rect
                            width:parent.width
                            height:(delegateChartHeight+100)/yAxisCount
                            anchors.left: parent.left
                            anchors.leftMargin:  1
                            y: reject_limit_high.height
                            visible: false
                            Text
                            {
                                id:suspect_limit_high
                                width: parent.width
                                height:parent.height
                                text:  qsTr("____"+limit_1)
                                verticalAlignment: Text.AlignBottom
                                color: "#757575"
                            }
                        }
                        Rectangle{
                            id:suspect_limit_low_rect
                            width:parent.width
                            height:(delegateChartHeight+100)/yAxisCount
                            anchors.left: parent.left
                            anchors.leftMargin:  1
                            anchors.bottom: reject_limit_low_rect.top
                            visible: false
                            Text
                            {
                                id:suspect_limit_low
                                width: parent.width
                                height:parent.height
                                text:  qsTr("____"+limit_1)
                                verticalAlignment: Text.AlignTop
                                color: "#757575"
                            }
                        }
                        Rectangle{
                            id:reject_limit_low_rect
                            width:parent.width
                            height:(delegateChartHeight+100)/yAxisCount
                            anchors.left: parent.left
                            anchors.leftMargin:  1
                            anchors.bottom:  parent.bottom
                            anchors.bottomMargin:   8
                            y:suspect_limit_high.height+reject_limit_high.height+suspect_limit_low.height+60
                            visible: false
                            Text
                            {
                                id: reject_limit_low
                                width: parent.width
                                height:parent.height
                                text: qsTr("____"+limit_1)
                                verticalAlignment: Text.AlignTop
                                color: "#757575"
                            }
                        }
                    }
                }
            }
        }

        ScrollBar {
            id: hbar
            height: parent.height
            anchors.right: parent.right
            policy: ScrollBar.AlwaysOn
        }
    }

    Timer
    {
        id: trendsGraphLoadTimer
        interval: 10
        triggeredOnStart: false
        repeat: false
        onTriggered:
        {

            var dwPageIndex = config.getActivePage()
            if(dwPageIndex == 5)
            {
                trendsGraphQmlObj.trendsGraphData(g_SelectedRecipeNum)//selectedRecipeNum)

            }
        }
    }
}

