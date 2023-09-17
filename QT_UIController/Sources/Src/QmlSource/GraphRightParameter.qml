import QtQuick 2.0
//import QtCharts 2.1
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.1 as NewControl
import QtQml 2.2
import Style 1.0
import QtQuick 2.7
import QtQml.StateMachine 1.0
import com.branson.openedscreen 1.0

import myCustomChartView 19.1
import myCustomXYSeries 19.1
import myCustomValueAxis 19.1

Item {
    readonly property string qmltextTimeAxis: qsTr("Time(s)")
    readonly property string qmltextPhaseAxis:  "Phase(Deg)"
    readonly property string qmltextEnergyAxis:   "Energy"
    readonly property string qmltextFreqAxis: "Freq(Hz)"
    readonly property string qmltextAmpAxis:  "Amp(%)"
    readonly property string qmltextCurrentAxis:  "Current(%)"
    readonly property string qmltextPowerAxis:  "Power(%)"
    readonly property string qmltextForceAxis: "Force(N))"
    readonly property string qmltextVelocityAxis:  "Velocity(mm/s))"
    readonly property string qmltextAbsoluteDistanceAxis:  "Absolute Distance(mm))"
    readonly property string qmltextCollapseDistanceAxis:  "Collapse Distance(mm))"
    readonly property string qmltextEnergyPlot:  "energyPlot"
    readonly property string qmltextAmpPlot:  "ampPlot"
    readonly property string qmltextAmpPlot1:   "ampPlot1"
    readonly property string qmltextCurrentPlot:"currPlot"
    readonly property string qmltextPowerPlot:"powerPlot"
    readonly property string qmltextPhasePlot:"phasePlot"
    readonly property string qmltextForcePlot:"forcePlot"
    readonly property string qmltextVelocityPlot:"velocityPlot"
    readonly property string qmltextAbsoluteDistancePlot:"absoluteDistancePlot"
    readonly property string qmltextCollapseDistancePlot:"collapseDistancePlot"
    readonly property string qmltextFreqPlot: "freqPlot"
    readonly property string qmltextPhase0Plot:"phase0Plot"
    readonly property string qmltextSecUnit:("s")
    readonly property string qmltextDisplayandStoreGraph: qsTr("Display and Store Graph")

    readonly property int timeAxisDefaultMaximumValue: 4
    readonly property int timeAxisDefaultMinimumValue: 0
    readonly property int phaseAxisDefaultMaximumValue: 180
    readonly property int phaseAxisDefaultMinimumValue: -180
    readonly property int energyAxisDefaultMaximumValue: 50
    readonly property int energyAxisDefaultMinimumValue: 0
    readonly property int freqAxisDefaultMaximumValue: 20450
    readonly property int freqAxisDefaultMinimumValue: 19450
    readonly property int ampAxisDefaultMaximumValue: 120
    readonly property int ampAxisDefaultMinimumValue: 0
    readonly property int currentAxisDefaultMaximumValue: 30
    readonly property int currentAxisDefaultMinimumValue: 0
    readonly property int powerAxisDefaultMaximumValue: 10
    readonly property int powerAxisDefaultMinimumValue: 0
    readonly property int forceAxisDefaultMaximumValue: 120
    readonly property int forceAxisDefaultMinimumValue: 0
    readonly property int velocityAxisDefaultMaximumValue: 120
    readonly property int velocityAxisDefaultMinimumValue: 0
    readonly property int absoluteDistAxisDefaultMaximumValue: 120
    readonly property int absoluteDistAxisDefaultMinimumValue: 0
    readonly property int collapseDistAxisDefaultMaximumValue: 5
    readonly property int collapseDistAxisDefaultMinimumValue: -5

    property bool isRedRectChecked : false
    property bool isGreenRectChecked : false
    property bool isBlueRectChecked : false
    property bool isYellowRectChecked : false
    property bool ampflag: true
    property bool currentflag: true
    property bool powerflag: true
    property bool freqflag: true
    property bool enegyflag: true
    property bool phaseflag: true
    property bool forceflag: true
    property bool velocityflag: true
    property bool absdistanceflag: true
    property bool collapseDisflag: true
    property int fontSizeScaleFactor: width / 800

    /* Indices for the check box and drop box functioning */
    property int ampInd : 0
    property int powerInd: 1
    property int freqInd: 2
    property int forceInd: 3
    property int collapseDistInd : 4
    property int absoluteDistInd: 5
    property int velocityInd: 6
    property int currInd: 7
    property int phaseInd: 8
    property int energyInd: 9
    property int timeInd: 10

    /* Indices for plotting the graph from the backend */
    property int ampPlotInd: 0
    property int powerPlotInd: 1
    property int freqPlotInd: 2
    property int forcePlotInd: 3
    property int collapseDistPlotInd : 4
    property int absoluteDistPlotInd: 5
    property int velocityPlotInd: 6
    property int currPlotInd: 7
    property int phasePlotInd: 8
    property int energyPlotInd: 9
    property int timePlotInd: 10

    property bool isAmpLoaded: false
    property bool isPowerLoaded: false
    property bool isFreqLoaded: false
    property bool isForceLoaded: false
    property bool isCDLoaded: false
    property bool isADLoaded: false
    property bool isVelLoaded: false
    property bool isCurrLoaded: false
    property bool isPhaseLoaded: false
    property bool isEnergyLoaded: false

    property bool isPageLoaded: false
    width: 1020
    height: 750
    id: graphRightParameter

    property alias weldGraphLoadTimer: weldGraphLoadTimer

    /**
    *@breif: Reset plots
    **/
    function resetPlots()
    {
        isAmpLoaded     = false
        isPowerLoaded   = false
        isFreqLoaded    = false
        isForceLoaded   = false
        isCDLoaded      = false
        isADLoaded      = false
        isVelLoaded     = false
        isCurrLoaded    = false
        isPhaseLoaded   = false
        isEnergyLoaded  = false
    }

    Component.onCompleted:
    {
        isPageLoaded = true
        var activePage = config.getActivePage()
        if(activePage != 5)
            config.setActivePage(4)
        //weldGraphLoadTimer.start()
        recipe.displayandStoreGraphData();
    }

    ListModel {
        id: weldmodel
        ListElement { name: qsTr("Weld"); value:"";imageSource:"qrc:/images/image/delta.svg"}
    }

    ListModel {
        id: controlmodel
        ListElement { name: qsTr("Time"); value:"0.000"; unit: "s"}

    }

    Rectangle{
        id:graphRec
        width: (70/100)*parent.width
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0

        CustomChartView {
            id:customChartView
            antialiasing:true
            property real plotStartPointY:plotArea.y
            property real plotStartPointX: plotArea.x

            property real deltaX: plotArea.width / (timeAxis.max - timeAxis.min)

            property real myWidth : 0.5

            anchors.fill: parent

            CustomValueAxis {
                id:timeAxis
                color: "#000000"
                gridVisible: false
                labelsVisible: true
                labelsColor: "#000000"
                lineVisible: true
                titleText: qmltextTimeAxis
                titleVisible: true
                decimals: 2
                max: timeAxisDefaultMaximumValue
                min: timeAxisDefaultMinimumValue
                tickCount: 5
            }

            CustomValueAxis {
                id:phaseAxis
                color: "black"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "black"
                lineVisible: true
                decimals: 0
                titleText: qmltextPhaseAxis
                titleVisible: false
                max: phaseAxisDefaultMaximumValue
                min: phaseAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:phaseAxis1
                color: "black"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "black"
                lineVisible:true
                decimals: 0
                titleText: qmltextPhaseAxis
                titleVisible: false
                max: phaseAxisDefaultMaximumValue
                min: phaseAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id : energyAxis
                color: "#f4b400"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#f4b400"
                lineVisible: true
                decimals: 0
                titleText: qmltextEnergyAxis
                titleVisible: false
                max: energyAxisDefaultMaximumValue
                min: energyAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id : energyAxis1
                color: "#f4b400"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#f4b400"
                lineVisible: true
                decimals: 0
                titleText: qmltextEnergyAxis
                titleVisible: false
                max: energyAxisDefaultMaximumValue
                min: energyAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id : freqAxis
                color: "#4285f4"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#4285f4"
                lineVisible: true
                decimals: 0
                titleText: qmltextFreqAxis
                titleVisible: false
                max: freqAxisDefaultMaximumValue
                min: freqAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id : freqAxis1
                color: "#4285f4"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#4285f4"
                lineVisible: true
                decimals: 0
                titleText: qmltextFreqAxis
                titleVisible: false
                max: freqAxisDefaultMaximumValue
                min: freqAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:ampAxis
                color: "#f03e3e"
                visible: true
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#f03e3e"
                lineVisible: true
                decimals: 0
                titleText:qmltextAmpAxis
                titleVisible: false
                max: ampAxisDefaultMaximumValue
                min: ampAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:ampAxis1
                color: "#f03e3e"
                visible: true
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#f03e3e"
                lineVisible: true
                decimals: 0
                titleText:qmltextAmpAxis
                titleVisible: false
                max: ampAxisDefaultMaximumValue
                min: ampAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:currentAxis
                color: "#009587"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#009587"
                lineVisible: true
                decimals: 0
                titleText: qmltextCurrentAxis
                titleVisible: false
                max: currentAxisDefaultMaximumValue
                min: currentAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:currentAxis1
                color: "#009587"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#009587"
                lineVisible: true
                decimals: 0
                titleText: qmltextCurrentAxis
                titleVisible: false
                max: currentAxisDefaultMaximumValue
                min: currentAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:powerAxis
                color: "#a82ba8"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#a82ba8"
                lineVisible: true
                decimals: 0
                titleText: qmltextPowerAxis
                titleVisible: false
                max: powerAxisDefaultMaximumValue
                min: powerAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:powerAxis1
                color: "#a82ba8"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#a82ba8"
                lineVisible: true
                decimals: 0
                titleText: qmltextPowerAxis
                titleVisible: false
                max: powerAxisDefaultMaximumValue
                min: powerAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:forceAxis
                color: "#bf6000"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#bf6000"
                lineVisible: true
                decimals: 0
                titleText: qmltextForceAxis
                titleVisible: false
                max: forceAxisDefaultMaximumValue
                min: forceAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:forceAxis1
                color: "#bf6000"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#bf6000"
                lineVisible: true
                decimals: 0
                titleText: qmltextForceAxis
                titleVisible: false
                max: forceAxisDefaultMaximumValue
                min: forceAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:velocityAxis
                color: "#524b03"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#524b03"
                lineVisible: true
                decimals: 0
                titleText: qmltextVelocityAxis
                titleVisible: false
                max: velocityAxisDefaultMaximumValue
                min: velocityAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:velocityAxis1
                color: "#524b03"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#524b03"
                lineVisible: true
                decimals: 0
                titleText:qmltextVelocityAxis
                titleVisible: false
                max: velocityAxisDefaultMaximumValue
                min: velocityAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:absoluteDistAxis
                color: "#8BC24A"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#8BC24A"
                lineVisible: true
                decimals: 0
                titleText: qmltextAbsoluteDistanceAxis
                titleVisible: false
                max: absoluteDistAxisDefaultMaximumValue
                min: absoluteDistAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:absoluteDistAxis1
                color: "#8BC24A"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#8BC24A"
                lineVisible: true
                decimals: 0
                titleText: qmltextAbsoluteDistanceAxis
                titleVisible: false
                max: absoluteDistAxisDefaultMaximumValue
                min: absoluteDistAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:collapseDistAxis
                color: "#ff80c0"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#ff80c0"
                lineVisible: true
                decimals: 0
                titleText: qmltextCollapseDistanceAxis
                titleVisible: false
                max: collapseDistAxisDefaultMaximumValue
                min: collapseDistAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomValueAxis {
                id:collapseDistAxis1
                color: "#ff80c0"
                gridVisible: false
                labelsVisible: true
                labelsFont.family: Style.regular.name
                labelsFont.pixelSize: Style.style1
                labelsColor: "#ff80c0"
                lineVisible: true
                decimals: 0
                titleText: qmltextCollapseDistanceAxis
                titleVisible: false
                max: collapseDistAxisDefaultMaximumValue
                min: collapseDistAxisDefaultMinimumValue
                tickCount: 11
            }

            CustomXYSeries{
                id: energyPlot
                width: customChartView.myWidth
                name: qmltextEnergyPlot
                axisX: timeAxis
                axisY: energyAxis
                color:"#f4b400"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: energyPlot1
                width: customChartView.myWidth
                name: qmltextEnergyPlot
                axisX: timeAxis
                axisYRight: energyAxis1
                color:"#f4b400"
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: ampPlot
                width: customChartView.myWidth
                color: "#f03e3e"
                name: qmltextAmpPlot
                axisX: timeAxis
                axisY: ampAxis
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: ampPlot1
                width: customChartView.myWidth
                color: "#f03e3e"
                name: qmltextAmpPlot1
                axisX: timeAxis
                axisYRight:ampAxis1
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: currPlot
                width: customChartView.myWidth
                color: "#009587"
                name: qmltextCurrentPlot
                axisX: timeAxis
                axisY: currentAxis
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: currPlot1
                width: customChartView.myWidth
                color: "#009587"
                name: qmltextCurrentPlot
                axisX: timeAxis
                axisYRight: currentAxis1
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: powerPlot
                width: customChartView.myWidth
                name: qmltextPowerPlot
                axisX: timeAxis
                axisY: powerAxis
                color:"#a82ba8"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: powerPlot1
                width: customChartView.myWidth
                name: qmltextPowerPlot
                axisX: timeAxis
                axisYRight: powerAxis1
                color:"#a82ba8"
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: phasePlot
                width: customChartView.myWidth
                name: qmltextPhasePlot
                axisX: timeAxis
                axisY: phaseAxis
                color:"#000001"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: phasePlot1
                width: customChartView.myWidth
                name: qmltextPhasePlot
                axisX: timeAxis
                axisYRight: phaseAxis1
                color:"#000001"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: forcePlot
                width: customChartView.myWidth
                name: qmltextForcePlot
                axisX: timeAxis
                axisY: forceAxis
                color:"#bf6000"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: forcePlot1
                width: customChartView.myWidth
                name: qmltextForcePlot
                axisX: timeAxis
                axisYRight: forceAxis1
                color:"#bf6000"
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: velocityPlot
                width: customChartView.myWidth
                name: qmltextVelocityPlot
                axisX: timeAxis
                axisY: velocityAxis
                color:"#524b03"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: velocityPlot1
                width: customChartView.myWidth
                name:qmltextVelocityPlot
                axisX: timeAxis
                axisYRight: velocityAxis1
                color:"#524b03"
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: absoluteDistancePlot
                width: customChartView.myWidth
                name: qmltextAbsoluteDistancePlot
                axisX: timeAxis
                axisY: absoluteDistAxis
                color:"#8BC24A"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: absoluteDistancePlot1
                width: customChartView.myWidth
                name: qmltextAbsoluteDistancePlot
                axisX: timeAxis
                axisYRight: absoluteDistAxis1
                color:"#8BC24A"
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: collapseDistancePlot
                width: customChartView.myWidth
                name: qmltextCollapseDistancePlot
                axisX: timeAxis
                axisY: collapseDistAxis
                color:"#ff80c0"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: collapseDistancePlot1
                width: customChartView.myWidth
                name: qmltextCollapseDistancePlot
                axisX: timeAxis
                axisYRight: collapseDistAxis1
                color:"#ff80c0"
                visible: false
                pointsVisible: true
            }

            CustomXYSeries{
                id: freqPlot
                width: customChartView.myWidth
                name: qmltextFreqPlot
                axisX: timeAxis
                axisY: freqAxis
                color:"#4285f4"
                visible: true
                pointsVisible: true
            }

            CustomXYSeries{
                id: freqPlot1
                width: customChartView.myWidth
                name: qmltextFreqPlot
                axisX: timeAxis
                axisYRight: freqAxis1
                color:"#4285f4"
                visible: false
                pointsVisible: true
            }

            Repeater {
                id:graphPoints
                model:weldPointModel
                Component
                {
                    id:graphPointsDelegate
                    Item
                    {
                        property double xAxisPosition: model.modelData.xAxisPos - weldGraphObj.getTriggerPoint()
                        id: graphPointItem
                        y: customChartView.plotStartPointY - graphPointLabel.height
                        x: customChartView.plotStartPointX + ( (customChartView.deltaX * (xAxisPosition - timeAxis.min)) - graphPointLine.width / 2 )
                        width : graphPointLine.width
                        height : graphPointLine.height + graphPointLabel.height
                        visible: ((xAxisPosition >= timeAxis.min) && (xAxisPosition <= timeAxis.max))
                        Rectangle
                        {
                            id: graphPointLine
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            border.width: 1
                            height: customChartView.plotArea.height
                            width:1
                            color: "black"
                        }
                        Text {
                            id: graphPointLabel
                            anchors.bottom: graphPointLine.top
                            anchors.bottomMargin: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: font.pixelSize + 10
                            width : 40
                            horizontalAlignment: Text.AlignHCenter
                            font.family : Style.semibold.name
                            font.pixelSize: Style.style4
                            text: model.modelData.labelName
                            color: "black"
                        }
                    }
                }
            }

            Rectangle
            {
                id: chartViewArea
                x: customChartView.plotArea.x
                y: customChartView.plotArea.y - ( graphNavigatorImage.height +  graphNavigatorLabel.height + graphSlider.raiseHeight)
                height : graphNavigator.height + graphNavigatorImage.height + graphNavigatorLabel.height
                width : customChartView.plotArea.width
                visible: false
            }

            Item {
                id: graphSlider

                property int raiseHeight: 30
                property string custColor : qmlPrimaryColor
                property int assetSize:32
                property real xAxisVal: timeAxis.min
                property string timeLabel:timeAxis.min
                property bool timeDecimalValFlag:true

                y: customChartView.plotStartPointY - ( graphNavigatorImage.height +  graphNavigatorLabel.height + raiseHeight)
                x: customChartView.plotStartPointX + (xAxisVal - timeAxis.min) * customChartView.deltaX - width / 2
                width : graphNavigatorLabel.width
                height : graphNavigator.height + graphNavigatorImage.height + graphNavigatorLabel.height
                Rectangle
                {
                    id: graphNavigator
                    height: customChartView.plotArea.height + parent.raiseHeight
                    width:1
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: parent.custColor
                }

                Rectangle {
                    id: graphNavigatorImage
                    height: parent.assetSize * fontSizeScaleFactor
                    width : parent.assetSize * fontSizeScaleFactor
                    radius: width / 2
                    color: parent.custColor
                    anchors.bottom: graphNavigator.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    MouseArea {
                        id: graphNavigatorMouseArea
                        property point posInChartArea
                        property int indexClosest1:0
                        anchors.fill: parent
                        cursorShape: Qt.ToolTip
                        hoverEnabled: false
                        onPositionChanged: {
                            /* Drag position of the slider */
                            var point = mapToItem(chartViewArea , mouse.x , mouse.y);
                            var retVal ;
                            var timeVal;
                            var tmpPoint,timePoint,powerPoint,freqPoint, phasePoint, currPoint, energyPoint, ampPoint, forcePoint, velocityPoint, absoluteDistPoint, collapseDistPoint;

                            if(point.x < 0 || point.x >= customChartView.plotArea.width)
                            {
                                if(point.x < 0)
                                {
                                    posInChartArea.x = 0 ;

                                    timeVal = timeAxis.min ;
                                }
                                else
                                {
                                    posInChartArea.x = customChartView.plotArea.width ;

                                    timeVal = timeAxis.max ;
                                }
                            }
                            else
                            {
                                posInChartArea.x = point.x;
                                posInChartArea.y = point.y;

                                timeVal = posInChartArea.x / customChartView.deltaX + timeAxis.min;
                            }

                            graphSlider.xAxisVal = timeVal;

                            /* calculating the points in the graph */
                            retVal = getClosestPoints(timeVal);
                            console.log("retVal " + retVal);
                            if(-1 == retVal )
                            {
                                tmpPoint = 0;
                                powerPoint = 0 ;
                                freqPoint = 0;
                                phasePoint = 0 ;
                                currPoint = 0 ;
                                ampPoint = 0 ;
                                energyPoint = 0;
                                forcePoint = 0 ;
                                velocityPoint = 0 ;
                                absoluteDistPoint = 0 ;
                                collapseDistPoint = 0 ;
                            }
                            else
                            {
                                if (powerPlot1.visible==true)
                                {
                                   tmpPoint = powerPlot1.at(indexClosest1);
                                   powerPoint = tmpPoint.y;
                                }
                                else
                                {
                                    tmpPoint = powerPlot.at(indexClosest1);
                                    powerPoint = tmpPoint.y;
                                }

                                if (freqPlot1.visible==true)
                                {
                                   tmpPoint = freqPlot1.at(indexClosest1);
                                   freqPoint = tmpPoint.y;
                                }
                                else
                                {
                                   tmpPoint = freqPlot.at(indexClosest1);
                                   freqPoint = tmpPoint.y;
                                }

                                if (ampPlot1.visible==true)
                                {
                                  tmpPoint = ampPlot1.at(indexClosest1);
                                  ampPoint = tmpPoint.y;
                                }
                                else
                                {
                                  tmpPoint = ampPlot.at(indexClosest1);
                                  ampPoint = tmpPoint.y;
                                }

                                if (forcePlot1.visible==true)
                                {
                                  tmpPoint = forcePlot1.at(indexClosest1);
                                  forcePoint = tmpPoint.y
                                }
                                else
                                {
                                  tmpPoint = forcePlot.at(indexClosest1);
                                  forcePoint = tmpPoint.y
                                }

                                if (collapseDistancePlot1.visible ==true)
                                {
                                  tmpPoint = collapseDistancePlot1.at(indexClosest1);
                                  collapseDistPoint = tmpPoint.y ;
                                }
                                else
                                {
                                  tmpPoint = collapseDistancePlot.at(indexClosest1);
                                  collapseDistPoint = tmpPoint.y ;
                                }

                                if (absoluteDistancePlot1.visible == true)
                                {
                                  tmpPoint = absoluteDistancePlot1.at(indexClosest1);
                                  absoluteDistPoint = tmpPoint.y ;
                                }
                                else
                                {
                                   tmpPoint = absoluteDistancePlot.at(indexClosest1);
                                   absoluteDistPoint = tmpPoint.y ;
                                }

                                if (velocityPlot1.visible==true)
                                {
                                    tmpPoint = velocityPlot1.at(indexClosest1);
                                    velocityPoint = tmpPoint.y ;
                                }
                                else
                                {
                                    tmpPoint = velocityPlot.at(indexClosest1);
                                    velocityPoint = tmpPoint.y ;
                                }

                                if (currPlot1.visible==true)
                                {
                                    tmpPoint = currPlot1.at(indexClosest1);
                                    currPoint = tmpPoint.y;
                                 }
                                 else
                                 {
                                     tmpPoint = currPlot.at(indexClosest1);
                                     currPoint = tmpPoint.y;
                                 }

                                 if (phasePlot1.visible==true)
                                 {
                                     tmpPoint = phasePlot1.at(indexClosest1);
                                     phasePoint = tmpPoint.y;
                                 }
                                 else
                                 {
                                     tmpPoint = phasePlot.at(indexClosest1);
                                     phasePoint = tmpPoint.y;
                                  }

                                  if (energyPlot1.visible==true)
                                  {
                                     tmpPoint = energyPlot1.at(indexClosest1);
                                     energyPoint = tmpPoint.y ;
                                   }
                                   else
                                   {
                                     tmpPoint = energyPlot.at(indexClosest1);
                                     energyPoint = tmpPoint.y ;
                                   }
        
                                checkboxObj.updateParameterVal(ampPoint,currPoint,powerPoint,freqPoint,energyPoint,phasePoint,forcePoint,velocityPoint,absoluteDistPoint,collapseDistPoint)
                                gridview2.contentItem.children[0].paramVal = graphNavigatorLabel.text;
                            }
                        }
                    }
                }

                Text {
                    id: graphNavigatorLabel
                    anchors.bottom: graphNavigatorImage.top
                    anchors.bottomMargin: 5
                    height :  font.pixelSize + 10
                    width: font.pixelSize * 12
                    anchors.horizontalCenter: graphSlider.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family : Style.semibold.name
                    font.pixelSize: Style.style4
                    text: (graphSlider.xAxisVal<=0) ? "0" + " "+qmltextSecUnit : graphSlider.xAxisVal.toFixed(5) + " " + qmltextSecUnit
                    color: parent.custColor
                }

                Component.onCompleted: {

                    console.log("parent.x "+parent.x);
                    console.log("graphSlider.x "+graphSlider.x);
                    console.log("graphSlider.xAxisVal "+graphSlider.xAxisVal);

                }

            }
            //legend.visible: false
        }
    }
    Rectangle
    {
        id: enableGraphScreenOpacity
        color: "#eeeeee"
        opacity: 0.5
        anchors.fill: graphRec
        MouseArea
        {
            anchors.fill: parent
        }
    }

    Rectangle {
        id: rectangle
        width: (30/100)*parent.width
        color: "#eeeeee"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: graphRec.right

        GridView {
            id:gridview1
            height:30
            anchors.topMargin: 10
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: (10/260)*parent.width
            anchors.leftMargin: (10/260)*parent.width
            interactive: false
            cellWidth: width; cellHeight: height/4
            focus: true
            model: weldmodel
            delegate: Item {
                width:parent.width ; height: parent.height
                Rectangle {
                    id: weldmodelrectangle
                    width: parent.width; height: (50/50)*parent.height
                    color: "#eeeeee"
                    Text
                    {
                        id:autoRefreshText
                        text:qmltextDisplayandStoreGraph
                        color: "#757575"
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        font.family : Style.semibold.name
                        font.pixelSize: Style.style2
                    }

                    ToogleBar
                    {
                        id: graphAutoRefresh
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        font.family : Style.semibold.name
                        font.pixelSize: Style.style2
                        checked: recipe.autoGraphRefreshState
                        onCheckedChanged:
                        {
                            //clearGraph();
                            recipe.setAutoGraphRefreshState(checked);
                            recipe.displayandStoreGraphData();
                            if(checked == false)
                            {
                                weldGraphObj.clearGraph();
                                clearGraph()
                                loaderGraphElement.item.resetPlots()
                                loaderGraphElement.item.plotGraph()

                            }
                            updateGraph(checked);
                        }

                        Component.onCompleted:
                        {
                            updateGraph(checked)
                        }
                    }
                }
            }
        }

        GridView {
            id:gridview2
            height: 50
            anchors.top: gridview1.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: (10/260)*parent.width
            anchors.leftMargin: (10/260)*parent.width
            interactive: false
            cellWidth: width;
            cellHeight: (50/50)*height
            focus: true
            model: controlmodel
            delegate: Item {
                id:timeValItem

                property string paramVal: value

                width:parent.width
                height: parent.height
                Rectangle {
                    id: legendcontrolrectangle
                    width: parent.width;
                    height:(50/50)*parent.height
                    color: "#898d96"
                    Text
                    {
                        y: parent.height/2-height/2
                        text: name
                        color: "#ffffff"
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style2
                    }
                    Text
                    {
                        y: parent.height/2-height/2
                        color: "#ffffff"
                        text: timeValItem.paramVal+" "
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        font.family: Style.semibold.name
                        font.pixelSize: Style.style2
                    }
                }
            }
        }

        GridView
        {
            id:gridview3
            property int paramIndex:1
            anchors.top: gridview2.bottom
            anchors.topMargin:2
            anchors.bottom: rectangle.bottom
            anchors.bottomMargin: 20
            width:gridview2.width+10
            anchors.left:gridview2.left
            anchors.leftMargin: 0
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickDirection
            clip:true
            cellWidth: gridview3.width
            cellHeight: 50
            model:checkboxlistModel
            delegate:GraphRightParamDelegate {
                id: graphRightParamDelegate

                width : gridview3.cellWidth
                height: gridview3.cellHeight

                comboBox.onCurrentIndexChanged:
                {
                    leftAndRightSwitch(index , comboBox.currentIndex , checkBox.checked);
                }

                checkBox.onCheckedStateChanged :
                {
                    changeGraphAxisVisiblity(index,checkBox.checked);
                }
            }
            NewControl.ScrollBar.vertical: NewControl.ScrollBar{}
            Component.onCompleted:
            {
                checkboxObj.setDefaultValues()
            }
            Component.onDestruction:
            {
                checkboxObj.getDefaultValues()
            }
        }
    }
    Timer
    {
        id: weldGraphLoadTimer
        interval: 10
        triggeredOnStart: false
        repeat: false
        onTriggered:
        {
            weldGraphObj.clearGraph()
            clearGraph()
            recipe.setIsManualRefreshState(true);
            recipe.updateWeldHistoryList()
        }
    }
    /**
    *@breif: Calculating the points in the graph
    **/
    function getClosestPoints(xval)
    {
        graphNavigatorMouseArea.indexClosest1 = weldGraphObj.GetClosestIndex(xval)
        return;
        var length = weldGraphObj.getCount();
        var point ;
        var startXPoint = 0;
        var startXPoint1 = 0;
        var endXPoint = 0;
        var xAxisStep = 0.000 ;

        var closestInd = 0;
        var closestVal = 0.000;

        var tmpVal = 0 ;

        if(0 == length)
        {
            return -1 ;
        }

        point = powerPlot.at(0);
        startXPoint = point.x ;
        point = powerPlot.at(1);
        startXPoint1 = point.x ;

        point = powerPlot.at(length - 1)
        endXPoint = point.x;

        if((xval < startXPoint) || (xval > endXPoint))
        {
            return -1;
        }

        point = powerPlot.at(2);
        xAxisStep = point.x - startXPoint1 ;
        if(xAxisStep == 0)
            xAxisStep=1

        closestVal = (xval - startXPoint);
        if(closestVal == 0)
            closestInd=1
        else
            closestInd = closestVal / xAxisStep;

        point = powerPlot.at(closestInd);

        if((xval - point.x) > (xAxisStep/2))
        {
            graphNavigatorMouseArea.indexClosest1 = ((closestVal/xAxisStep) + 1);
        }
        else
        {
            graphNavigatorMouseArea.indexClosest1 = closestVal/xAxisStep;
        }

        return 0;
    }

    /**
    *@breif: Checking if the decimal flag is to be reset
    **/
    function calDecimalFlag()
    {
        var xAxisStep = 0;
        var point1 = 0;
        var point2 = 0;

       // point1 = powerPlot.at(0);
       // point2 = powerPlot.at(1);

        xAxisStep = point2.x - point1.x;

        //if(Math.abs(xAxisStep) < 1)
       // {
       //     return true;
       // }
       // else
       // {
            return false;
        //}
    }

    /**
    *@breif: Clear graph
    **/
    function clearGraph()
    {
        timeAxis.min=0;
        timeAxis.max=4;
        /*
        freqPlot.clear()
        freqPlot1.clear()
        powerPlot.clear()
        currPlot.clear()
        ampPlot.clear()
        phasePlot.clear()
        energyPlot.clear()
        forcePlot.clear()
        velocityPlot.clear()
        collapseDistancePlot.clear()
        absoluteDistancePlot.clear()
        */
    }

    function updateGraph(checked)
    {
        enableGraphScreenOpacity.visible = !checked
        graphSlider.visible = checked
    }

    /**
    *@breif: Plots the graph
    **/
    function plotGraph()
    {
        if(!recipe.isManualRefresh)
            return;
        clearGraph()
        var tmpPoint,timePoint , powerPoint, freqPoint, phasePoint, currPoint, energyPoint, ampPoint, forcePoint, velocityPoint, absoluteDistPoint, collapseDistPoint;

        /* Update the Min and Max values */
        var axisMinValues = weldGraphObj.getAxisMinParameters();
        var axisMaxValues = weldGraphObj.getAxisMaxParameters();

        timeAxis.min          = axisMinValues[timePlotInd]+0;
        freqAxis1.min         = freqAxis.min            = axisMinValues[freqPlotInd] - 100;
        phaseAxis1.min        = phaseAxis.min           = -180
        ampAxis1.min          = ampAxis.min             = 0
        currentAxis1.min      = currentAxis.min         = 0
        powerAxis1.min        = powerAxis.min           = Math.max(0 , axisMinValues[powerPlotInd]);
        energyAxis1.min       = energyAxis.min          = Math.max(0 , (axisMinValues[energyPlotInd] - 2));
        forceAxis1.min        = forceAxis.min           = Math.max(0 , (axisMinValues[forcePlotInd] - 2));
        velocityAxis1.min     = velocityAxis.min        = Math.max(0 , (axisMinValues[velocityPlotInd] - 2));
        absoluteDistAxis1.min = absoluteDistAxis.min    = Math.max(0 , (axisMinValues[absoluteDistPlotInd] - 2));
        collapseDistAxis1.min = collapseDistAxis.min    = axisMinValues[collapseDistPlotInd];

        timeAxis.max            =                         axisMaxValues[timePlotInd]+0;
        freqAxis1.max           = freqAxis.max          = axisMaxValues[freqPlotInd] + 100;
        phaseAxis1.max          = phaseAxis.max         = 180
        ampAxis1.max            = ampAxis.max           = 120
        currentAxis1.max        = currentAxis.max       = 120
        powerAxis1.max          = powerAxis.max         = axisMaxValues[powerPlotInd] + 1;
        energyAxis1.max         = energyAxis.max        = axisMaxValues[energyPlotInd] + 1;
        forceAxis1.max          = forceAxis.max         = axisMaxValues[forcePlotInd] + 1;
        velocityAxis1.max       = velocityAxis.max      = axisMaxValues[velocityPlotInd] + 1;
        absoluteDistAxis1.max   = absoluteDistAxis.max  = axisMaxValues[absoluteDistPlotInd] + 1;
        collapseDistAxis1.max   =  collapseDistAxis.max = axisMaxValues[collapseDistPlotInd] ;

        /* Rounding of axis values for proper representation */
        freqAxis1.max           = freqAxis.max          = roundAxisValues(freqAxis.max , freqAxis.min);
        phaseAxis1.max          = phaseAxis.max         = roundAxisValues(phaseAxis.max , phaseAxis.min);
        ampAxis1.max            = ampAxis.max           = roundAxisValues(ampAxis.max , ampAxis.min);
        currentAxis1.max        = currentAxis.max       = roundAxisValues(currentAxis.max , currentAxis.min);
        powerAxis1.max          = powerAxis.max         = roundAxisValues(powerAxis.max , powerAxis.min);
        energyAxis1.max         = energyAxis.max        = roundAxisValues(energyAxis.max , energyAxis.min);
        forceAxis1.max          = forceAxis.max         = roundAxisValues(forceAxis.max , forceAxis.min);
        velocityAxis1.max       = velocityAxis.max      = roundAxisValues(velocityAxis.max , velocityAxis.min);
        absoluteDistAxis1.max   = absoluteDistAxis.max  = roundAxisValues(absoluteDistAxis.max , absoluteDistAxis.min);
        //reset the collapse max and min
        axisCompensationValue(collapseDistAxis.max , collapseDistAxis.min)


        var flag = checkboxObj.getCheckBoxState(freqPlotInd)
        var isLeft =checkboxObj.getComboIndex(freqPlotInd)
        if(flag)
        {
            isFreqLoaded = true
            if(isLeft==0)
            {
               weldGraphObj.appendSamples(freqPlot , freqPlotInd);
            }
            else
            {
               weldGraphObj.appendSamples(freqPlot1, freqPlotInd);
            }
        }

        flag = checkboxObj.getCheckBoxState(powerPlotInd)
         isLeft = checkboxObj.getComboIndex(powerPlotInd)
         if(flag)
         {
             isPowerLoaded = true;
             if(isLeft == 0)
            {
                 weldGraphObj.appendSamples(customChartView.series("powerPlot") , powerPlotInd);
            }
             else
            {
                 weldGraphObj.appendSamples(powerPlot1, powerPlotInd);
         }
        }

         flag = checkboxObj.getCheckBoxState(currPlotInd)
         isLeft = checkboxObj.getComboIndex(currPlotInd)
         if(flag)
         {
             isCurrLoaded = true
             if(isLeft == 0)
            {
                weldGraphObj.appendSamples(currPlot, currPlotInd);
            }
             else
            {
                 weldGraphObj.appendSamples(currPlot1, currPlotInd);
         }
        }

         flag = checkboxObj.getCheckBoxState(ampPlotInd)
         isLeft = checkboxObj.getComboIndex(ampPlotInd)
         if(flag)
         {
             isAmpLoaded = true
             if(isLeft == 0)
            {
                weldGraphObj.appendSamples(ampPlot, ampPlotInd);
            }
             else
            {
                 weldGraphObj.appendSamples(ampPlot1 , ampPlotInd);
            }
         }

         flag = checkboxObj.getCheckBoxState(phasePlotInd)
         isLeft = checkboxObj.getCheckBoxState(phasePlotInd)
         if(flag)
         {
             isPhaseLoaded = true
             if(isLeft == 0)
            {
               weldGraphObj.appendSamples(phasePlot, phasePlotInd);
            }
             else
            {
                 weldGraphObj.appendSamples(phasePlot1, phasePlotInd);
         }
        }

         flag = checkboxObj.getCheckBoxState(energyPlotInd)
         isLeft = checkboxObj.getComboIndex(energyPlotInd)
         if(flag)
         {
             isEnergyLoaded = true
             if(isLeft == 0)
            {
               weldGraphObj.appendSamples(energyPlot, energyPlotInd);
            }
            else
            {
                 weldGraphObj.appendSamples(energyPlot1, energyPlotInd);
            }
        }

         flag = checkboxObj.getCheckBoxState(forcePlotInd)
         isLeft = checkboxObj.getComboIndex(forcePlotInd)
         if(flag)
         {
             isForceLoaded = true
             if(isLeft == 0)
            {
               weldGraphObj.appendSamples(forcePlot, forcePlotInd);
            }
             else
            {
                 weldGraphObj.appendSamples(forcePlot1, forcePlotInd);
            }
        }

         flag = checkboxObj.getCheckBoxState(velocityPlotInd)
         isLeft = checkboxObj.getComboIndex(velocityPlotInd)
         if(flag)
         {
             isVelLoaded = true
             if(isLeft == 0)
            {
              weldGraphObj.appendSamples(velocityPlot, velocityPlotInd);
            }
            else
            {
              weldGraphObj.appendSamples(velocityPlot1, velocityPlotInd);
            }
        }

        flag = checkboxObj.getCheckBoxState(absoluteDistPlotInd);
        isLeft = checkboxObj.getComboIndex(absoluteDistPlotInd);
        if(flag)
        {
            isADLoaded = true
            if(isLeft==0)
            {
              weldGraphObj.appendSamples(absoluteDistancePlot, absoluteDistPlotInd);
            }
            else
            {
               weldGraphObj.appendSamples(absoluteDistancePlot1, absoluteDistPlotInd);
            }
        }

         flag = checkboxObj.getCheckBoxState(collapseDistPlotInd)
         isLeft = checkboxObj.getComboIndex(collapseDistPlotInd)
         if(flag)
         {
             isCDLoaded = true
            if (isLeft==0)
            {
                 weldGraphObj.appendSamples(customChartView.series("collapseDistancePlot"),collapseDistPlotInd);
            }
             else
            {
                 weldGraphObj.appendSamples(collapseDistancePlot1, collapseDistPlotInd);
            }
        }

/*

        tmpPoint = powerPlot.at(0);
        powerPoint = tmpPoint.y;

        timePoint = tmpPoint.x;

        tmpPoint = freqPlot.at(0);
        freqPoint = tmpPoint.y;

        tmpPoint = ampPlot.at(0);
        ampPoint = tmpPoint.y;

        tmpPoint = currPlot.at(0);
        currPoint = tmpPoint.y;

        tmpPoint = phasePlot.at(0);
        phasePoint = tmpPoint.y;

        tmpPoint = energyPlot.at(0);
        energyPoint = tmpPoint.y ;

        tmpPoint = forcePlot.at(0);
        forcePoint = tmpPoint.y ;

        tmpPoint = velocityPlot.at(0);
        velocityPoint = tmpPoint.y ;

        tmpPoint = absoluteDistancePlot.at(0);
        absoluteDistPoint = tmpPoint.y ;

        tmpPoint = collapseDistancePlot.at(0);
        collapseDistPoint = tmpPoint.y ;

        graphSlider.timeLabel = timePoint.toFixed(5);
        gridview2.contentItem.children[0].paramVal = timePoint.toFixed(5);

        checkboxObj.updateParameterVal(ampPoint,currPoint,powerPoint,freqPoint,energyPoint,phasePoint,forcePoint,velocityPoint,absoluteDistPoint,collapseDistPoint)
    */
    }

    /**
    *@breif: Rounding of axis values for proper representation
    **/
    function roundAxisValues( axisMaximumVal , axisMinimumVal)
    {
        var ind = 0 ;
        var tmpValues;
        var reminder;
        var retVal = 0 ;

        tmpValues = axisMaximumVal - axisMinimumVal;
        reminder = (tmpValues % 10)

        if(reminder != 0 )
        {
            retVal = axisMaximumVal + (10 - reminder);
        }
        else
        {
            retVal = axisMaximumVal ;
        }

        return retVal ;
    }


    /**
    *@breif: recauclate the collapse maximum and minimum value
    **/
    function axisCompensationValue(axisMaximumVal , axisMinimumVal)
    {
        var ind = 0 ;
        var tmpValues;
        var reminder;
        var retVal = 0 ;

        tmpValues = axisMaximumVal - axisMinimumVal;

        if(tmpValues < 1)
        {
            var tmpValuesPlus100 = 0
            var minExpandVal = 0
            var maxExpandVal = 0

            if(tmpValues < 0.1) /* resolution = 0.01 */
            {
                tmpValuesPlus100 = 10 - (tmpValues * 100)
            }
            else if(tmpValues < 0.5) /* resolution = 0.05 */
            {
                tmpValuesPlus100 = 50 - (tmpValues * 100)
            }
            else  /* resolution = 0.1 */
            {
                tmpValuesPlus100 = 100 - (tmpValues * 100)
            }

            minExpandVal = (tmpValuesPlus100 / 2) * 0.01
            maxExpandVal = (tmpValuesPlus100 - (tmpValuesPlus100 / 2)) * 0.01
            collapseDistAxis1.min = collapseDistAxis.min    = (axisMinimumVal - minExpandVal).toFixed(2);
            collapseDistAxis1.max = collapseDistAxis.max    = (axisMaximumVal + maxExpandVal).toFixed(2);
            collapseDistAxis.decimals = 2
            collapseDistAxis1.decimals = 2
        }
        else if(tmpValues < 10)  /* //if the difference of min and max is less then 10, the resolution accurate to 1 decimal*/
        {
            collapseDistAxis1.min = collapseDistAxis.min    = Math.floor(axisMinimumVal);
            collapseDistAxis1.max = collapseDistAxis.max    = Math.ceil(axisMaximumVal);
            collapseDistAxis1.decimals =  collapseDistAxis.decimals = 1
        }
        else
        {
            tmpValues = Math.ceil(axisMaximumVal) - Math.floor(axisMinimumVal)
            //if the difference is more than 10, the maximum and minimum will be expand to the multiple of 10.
            reminder = (tmpValues % 10)
            if(reminder !== 0)
            {
                var collapseCompensation =  10 - reminder
                var compensationAverg = collapseCompensation / 2
                collapseDistAxis1.min = collapseDistAxis.min = Math.floor(axisMinimumVal) - compensationAverg;
                collapseDistAxis1.max = collapseDistAxis.max = Math.ceil(axisMaximumVal) + compensationAverg + collapseCompensation % 2;
                collapseDistAxis1.decimals = collapseDistAxis.decimals = 0
            }
            else
            {
                collapseDistAxis1.min = collapseDistAxis.min = Math.floor(axisMinimumVal)
                collapseDistAxis1.max = collapseDistAxis.max = Math.ceil(axisMaximumVal)
                collapseDistAxis1.decimals = collapseDistAxis.decimals = 0
            }
        }
    }





    /**
    *@breif: To switch y-axis to left or right
    **/
    function leftAndRightSwitch(index,currentIndex,isCheckBoxSelected)
    {
        if(index < 0)
            index = 0

        checkboxObj.setAxisIndex(index,currentIndex);
        if(index == ampPlotInd)
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                     ampPlot.visible = true
                     ampPlot1.visible = false
                }
                else
                {
                     ampPlot.visible = false
                     ampPlot1.visible = true
                }
            }
            else
            {
               
                ampPlot.visible = false
                ampPlot1.visible = false
            }
        }
        else if(index == currPlotInd)
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {   //Left
                     currPlot.visible = true;
                     currPlot1.visible = false;
                }
                else
                {   //Right
                     currPlot.visible = false;
                     currPlot1.visible = true;
                }
            }
            else
            {
                 currPlot.visible = false;
                 currPlot1.visible = false;
             }
         }
         else if(index === powerPlotInd)
         {
             if(isCheckBoxSelected)
             {
                 if(currentIndex === 0)
                 {
                     powerPlot.visible = true
                     powerPlot1.visible = false
                 }
                 else
                 {
                     powerPlot.visible = false
                     powerPlot1.visible = true
                 }
             }
             else
             {
                 powerPlot.visible = false
                 powerPlot1.visible = false
             }
         }
         else if(index === freqPlotInd)
         {
             if(isCheckBoxSelected)
             {
                 if(currentIndex === 0)
                 {
                     freqPlot.visible = true
                     freqPlot1.visible = false
                 }
                 else
                 {
                     freqPlot.visible = false
                     freqPlot1.visible = true
                 }
             }
             else
             {
                 freqPlot.visible = false
                 freqPlot1.visible = false
             }
         }
         else if(index === energyPlotInd)
         {
             if(isCheckBoxSelected)
             {
                 if(currentIndex === 0)
                 {
                     energyPlot.visible = true;
                     energyPlot1.visible = false;
                 }
                 else
                 {
                     energyPlot.visible = false;
                     energyPlot1.visible = true;
                 }
             }
             else
             {
                 energyPlot.visible = false;
                 energyPlot1.visible = false;
             }
         }
         else if(index === phasePlotInd)
         {
             if(isCheckBoxSelected)
             {
//                 phase0Plot.visible = true;
                 if(currentIndex === 0)
                 {
                     phasePlot.visible = true;
                     phasePlot1.visible = false;

                 }
                 else
                 {
                     phasePlot.visible = false;
                     phasePlot1.visible = true;
                 }
             }
             else
             {
                 phasePlot.visible = false;
                // phase0Plot.visible = false;
                 phasePlot1.visible = false;
             }
         }
         else if(index === forcePlotInd)
         {
             if(isCheckBoxSelected)
             {
                 if(currentIndex === 0)
                 {
                     forcePlot.visible  = true
                     forcePlot1.visible = false
                 }
                 else
                 {
                     forcePlot.visible  = false
                     forcePlot1.visible = true
                 }
             }
             else
             {
                 forcePlot.visible  = false
                 forcePlot1.visible = false
             }
         }
         else if(index === velocityPlotInd)
         {
             if(isCheckBoxSelected)
             {
                 if(currentIndex === 0)
                 {
                     velocityPlot.visible = true;
                     velocityPlot1.visible = false;

                 }
                 else
                 {
                     velocityPlot.visible = false;
                     velocityPlot1.visible = true;
                 }
             }
             else
             {
                 velocityPlot.visible = false;
                 velocityPlot1.visible = false;
             }
         }
         else if(index === absoluteDistPlotInd)
         {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0 )
                {
                   absoluteDistancePlot.visible = true;
                   absoluteDistancePlot1.visible = false;
                }
                else
                {
                     absoluteDistancePlot.visible = false;
                     absoluteDistancePlot1.visible = true;
               }
            }
            else
            {
                 absoluteDistancePlot.visible = false;
                 absoluteDistancePlot1.visible = false;
            }
         }
         else if(index === collapseDistPlotInd)
         {
             if(isCheckBoxSelected)
             {
                 if(currentIndex == 0)
                 {
                     collapseDistancePlot.visible = true
                     collapseDistancePlot1.visible = false
                 }
                 else
                 {
                     collapseDistancePlot.visible = false
                     collapseDistancePlot1.visible = true
                 }
             }
             else
             {
                 collapseDistancePlot.visible = false
                 collapseDistancePlot1.visible = false
             }
         }
     }

    /**
    *@breif: To change graph axis visibility
    **/
    function changeGraphAxisVisiblity(dwIndex,ischecked)
    {
        if(dwIndex < 0)
            dwIndex = 0

         var flag = checkboxObj.getComboIndex(dwIndex)
         switch(dwIndex)
         {
         case ampPlotInd:
             ampPlot.visible = ischecked && (!flag)
             ampPlot1.visible = ischecked && (flag)

             break;

         case powerPlotInd:
             powerPlot.visible = ischecked && (!flag)
             powerPlot1.visible = ischecked && (flag)

             break;

         case freqPlotInd:
             freqPlot.visible = ischecked && (!flag)
             freqPlot1.visible = ischecked && (flag)

             break;

         case forcePlotInd:
             forcePlot.visible = ischecked && (!flag)
             forcePlot1.visible= ischecked && (flag)

             break;

         case collapseDistPlotInd:
             collapseDistancePlot.visible = ischecked && (!flag)
             collapseDistancePlot1.visible = ischecked && (flag)

             break;

         case absoluteDistPlotInd:

             absoluteDistancePlot.visible = ischecked && (!flag)
             absoluteDistancePlot1.visible = ischecked && (flag)

             break;

         case velocityPlotInd:

             velocityPlot.visible = ischecked && (!flag)
             velocityPlot1.visible= ischecked && (flag)

             break;

         case currPlotInd:

             currPlot.visible =  ischecked && (!flag)
             currPlot1.visible = ischecked && (flag)

             break;

         case phasePlotInd:

             phasePlot.visible = ischecked
             phasePlot.visible  = ischecked && (!flag)
             phasePlot1.visible = ischecked && (flag)

             break;

         case energyPlotInd:

             energyPlot.visible = ischecked && (!flag)
             energyPlot1.visible= ischecked && (flag)

             break;

        default:
            break;
        }
        checkboxObj.setCheckboxStatus(dwIndex,ischecked)
    }
}
