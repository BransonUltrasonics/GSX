import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Window 2.1
import QtCharts 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import CustomValidator 1.0
import Style 1.0
import com.branson.eventlogging 1.0

Item {
    readonly property string qmltextSeekRun :qsTr("SEEK RUN")
    readonly property string qmltextAbortRequest :qsTr("Seek scan is in progress. Aborting request.")
    readonly property string qmltextSeekRecipeSaveToSCFailed :qsTr("Seek recipe save to supervisory controller failed.")
    readonly property string qmltextSeekOperationFailed :qsTr("Seek run operation failed")
    readonly property string qmltextSeekOperationSuccess :qsTr("Seek run operation successful")
    readonly property string qmltextSeekRecipe :qsTr("Seek Recipe")
    readonly property string qmltextSEEKRECIPE:qsTr("SEEK RECIPE")
    readonly property string qmltextNoRecipeFoundInDB :qsTr("No seek recipe found in database. Save a copy before performing seek scan")
    readonly property string qmltextgraphPlotseek :qsTr("graphPlotseek")
    readonly property string qmltextTimeAxis :qsTr("Time(s)")
    readonly property string qmltextAmplitudeAxis : qsTr("Amplitude(%)")
    readonly property string qmltextCurrentAxis : qsTr("Current(%)")
    readonly property string qmltextPhaseAxis :qsTr("Phase(Deg)")
    readonly property string qmltextFrequencyAxis :qsTr("Frequency(Hz)")
    readonly property string qmltextPowerAxis :qsTr("Power(W)")
    readonly property string qmltextseekAmpPlot : qsTr("seekAmpPlot")
    readonly property string qmltextseekCurrPlot :qsTr("seekCurrPlot")
    readonly property string qmltextseekPhasePlot : qsTr("seekPhasePlot")
    readonly property string qmltextseekFreqPlot:qsTr("seekFreqPlot")
    readonly property string qmltextseekPowerPlot : qsTr("seekPowerPlot")
    readonly property string qmltextseekphaseAxis : qsTr("seekphaseAxis")
    readonly property string qmltextTimeUnit :qsTr(" s")
    readonly property string qmltextTime :qsTr("Time")
    readonly property string qmltextSAVE :qsTr("SAVE")
    readonly property string qmltextFailToSaveSeekRecipe :qsTr("Failed to save Seek Recipe data. Check for network connection or Recipe parameters")
    readonly property string qmltextSeekRecipeSaveSuccess :qsTr("Seek recipe saved successfully")
    readonly property string qmltextCANCEL :qsTr("CANCEL")
    readonly property string qmltextRESETTODEFAULT : qsTr("RESET TO DEFAULT")
    readonly property string qmltextResonantFreq :qsTr("Resonant Freq ")
    readonly property string qmltextSEEK :qsTr("SEEK")
    readonly property string qmltextGraphAutoRefresh: qsTr("Auto Graph Refresh")
    readonly property string qmltextMemoryClearBeforeSeek: qsTr("Memory Clear Before Seek")

    id: itemSeekScan
    visible: true
    width:  loaderelement.width
    height: loaderelement.height
    //y: 350
    property double scaleRatio: 1
    property int tabWindowMargin: 10
    property int dotSpacing: 5
    property int assetSize: 32
    property int layoutSpacing: 10
    property int histItemCount: 1

    property int startFreqInd:0
    property int stopFreqInd:1
    property int freqStepInd:2
    property int timeDelayInd:3
    property int maxAmpInd:4
    property int maxCurrentInd:5
    property int hornScanTimeoutInd:6

    property double fontSizeScaleFactor : window.height / 800
    property alias seekhistoryList: seekhistoryListCtrlView

    property int ampInd: 0
    property int freqInd: 1
    property int powerInd: 2
    property int curInd: 3
    property int phaseInd: 4
    property int timeInd: 5

    /**
    *@breif: Before plotting graph update the seek result titleBar on the right window
    **/
    function beforePlotGraph()
    {
        if(seekhistoryListCtrlView.count > 0)
        {
            seekPlotGraph();

            var titleTextString = seekhistoryListCtrlView.model[seekhistoryListCtrlView.indexSelected].scanId
            var retVal = 0 ;

            /* Update the seek result titleBar on the right window */
            seektitleRectText.text = getRightTitleString(titleTextString);
        }
    }

    Component.onCompleted:
    {
        seekThreeway.visible = true
    }
    FourCarouselDots
    {
        z:1
        id:seekThreeway
        width:30
        height: 30
        x:parent.width/2
        anchors.top: parent.top
        anchors.topMargin: 10
        Component.onCompleted:
        {
            selected= 2
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
            //            selected=2
            //            seekRecipeObj.init()
            //            seekhistoryDataObj.seekIndSelected = 0
            //            seekhistoryDataObj.refreshSeekHistory()
            loaderelement.setSource("SeekScanFeature.qml");
            titlebarchild.visible = false
        }
        mar3.onClicked:
        {
//            ultrasonictest.getTestModeAdvancedDataFromDB()
            ultrasonictest.init()
            loaderelement.setSource("Test.qml");
            ultrasonictest.setCheckBoxModelData()
            selected=3;
            titlebarchild.visible = false
        }
    }

    Rectangle {
        id: seekrootRect
        x: 2
        width: parent.width - 4
        height: parent.height - 4
        color: "#ffffff"
        visible: true

        /* Implement the entire screen as a layout for alignment on resizing as a 3 x 3 seekgrid layout */
        GridLayout {
            id: seekmainLayout
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
            columns: 3
            rows: 4
            columnSpacing: 0
            rowSpacing: 0

            Rectangle {
                id: seektabWindowRect
                Layout.row : 0
                Layout.column : 0

                Layout.columnSpan : 3

                implicitHeight: parent.height * 0.05
                implicitWidth: parent.width


                Layout.alignment: Qt.AlignTop
                Layout.minimumHeight: 2 * tabWindowMargin + assetSize
                Layout.minimumWidth: 2 * tabWindowMargin + 2 * assetSize + dotSpacing

                visible:true
            }

            Rectangle{
                id: seekleftWindow

                Layout.row : 1
                Layout. column : 0

                Layout.rowSpan : 3
                Layout.leftMargin: layoutSpacing
                Layout.rightMargin: layoutSpacing
                Layout.topMargin: layoutSpacing
                Layout.bottomMargin: layoutSpacing

                Layout.alignment: Qt.AlignTop

                Layout.maximumWidth: 260 * fontSizeScaleFactor

                implicitHeight: (parent.height - (seektabWindowRect.height +  2 * layoutSpacing))
                implicitWidth : parent.width * 0.2
                color : "#ffffff"

                visible: true

                HistoryBasicBar
                {
                    id: seekhistBasicBar
                    width: parent.width
                    height : (2*seekprogressBarRect1.height ) - layoutSpacing
                    visible: false
                }

                Rectangle {
                    id: seekhistoryListCtrlRect

                    property int numRecPerPage:0;

                    anchors.top: seekhistBasicBar.bottom
                    anchors.bottom: parent.bottom
                    anchors.left:seekleftWindow.left
                    anchors.right:parent.right

                    color:"#f8f9fa"

                    clip: true


                    Component.onCompleted:
                    {
                        numRecPerPage = height / 60 ;
                        seekhistoryListCtrlView.indexSelected = seekhistoryDataObj.seekIndSelected ;
                    }


                    ListView {
                        id: seekhistoryListCtrlView

                        property int indexSelected:0;

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        height:parent.height
                        model:seekhistoryListCtrlModel
                        delegate: SeekHistoryDelegate {
                            Connections
                            {
                                target: seekhistoryListCtrlView
                                onCurrentIndexChanged:
                                {
                                    if(seekhistoryListCtrlView.currentIndex == index)
                                    {
                                        seekhistoryListCtrlView.indexSelected = index
                                        seekhistoryDataObj.seekIndSelected = seekhistoryListCtrlView.indexSelected
                                        var titleTextString = model.modelData.scanId
                                        var retVal = 0 ;

                                        /* Update the seek result titleBar on the right window */
                                        seektitleRectText.text = getRightTitleString(titleTextString);

                                        /* Signal the Backend function to fetch the horn data and result from the DB */
                                        retVal = seekhistoryDataObj.getSeekGraphDataFromDb(model.modelData.scanId);

                                        if(retVal == 0)
                                        {
                                            seekPlotGraph();
                                        }
                                    }
                                }
                            }
                        }

                        Component.onCompleted:
                        {
                            beforePlotGraph()
                        }
                    }
                }
            }

            Rectangle {
                id : seekprogressBarRect1


                Layout.row : 1
                Layout.column : 1

                Layout.columnSpan : 2

                Layout.alignment: Qt.AlignTop

                implicitWidth : parent.width - ( seekleftWindow.width + 2 * layoutSpacing )

                implicitHeight : parent.height * 0.075

                visible:true

                PrimaryButton {
                    id: seekstartButton
                    text:qmltextSeekRun
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left : parent.left
                    anchors.leftMargin: 2 * 10

                    onClicked:
                    {
                        var retVal = 0 ;
                        var hornOpStatus = seekhistoryDataObj.getSeekOperationStatus();

                        /* Send horn recipe to SC */
                        if(true == hornOpStatus)
                        {
                            recipe.updateStatusText(qmltextAbortRequest)
                        }
                        else
                        {
                            Eventlogger.eventSend(EventLogging.UI_EVENT_SEEK_SCAN_STARTED);
                            retVal = seekRecipeObj.sendSeekRecipeDataToSc();

                            if(0 == retVal)
                            {
                                /* Horn Recipe Save Failed */
                                recipe.updateStatusText(qmltextSeekRecipeSaveToSCFailed)
                            }
                            else
                            {
                                /* Request Start Scan to SC */
                                seekhistoryDataObj.performSeekScan();
                                //statusProgressBar.progressVal = 0.0 ;
                                clearGraphSamples();
                            }
                        }
                    }

                    Connections
                    {
                        target: seekhistoryDataObj

                        onSeekScanCompleted:
                        {
                            if(49 != statusVal)
                            {
                                recipe.updateStatusText(qmltextSeekOperationFailed)
                            }
                            else
                            {
                                recipe.updateStatusText(qmltextSeekOperationSuccess)
                            }

                            /* Refresh the history data */
                            var retVal = seekrefreshHistory();

                            if(0 == retVal)
                            {
                                seekPlotGraph();
                            }
                        }
                    }
                }

                SecondaryButton {
                    id: seekadvancedButton
                    text: qmltextSeekRecipe
                    visible: true
                    anchors.verticalCenter: seekstartButton.verticalCenter
                    anchors.left: seekstartButton.right
                    anchors.leftMargin: 2 * 10
                    onClicked: {
                        var seekRecipeParamFlag ;

                        seekleftWindow.visible = false
                        seekrightWindow.visible = false
                        seekprogressBarRect1.visible = false
                        seekgraphWindow.visible = false
                        seektabWindowRect.visible = false
                        seekThreeway.visible = false

                        seekRecipeObj.getSeekDefaultValues()
                        seekRecipeControlWindow.visible = true
                        seekRecipeParamWindow.visible = true
                        titlebarchild.visible=true
                        titlebarchildtext.text=qmltextSEEKRECIPE
                        titlbaronclicked="SeekScanFeature.qml"

                        /* In case the seek recipe Param flag is modified, then there is no copy of seek recipe in Database */
                        //                        seekRecipeParamFlag = seekRecipeObj.getParameterChangedFlag();

                        //                        if(true == seekRecipeParamFlag)
                        //                        {
                        //                            recipe.updateStatusText(qmltextNoRecipeFoundInDB)
                        //                        }
                    }
                }
            }

            Rectangle {
                id : seekgraphWindow

                Layout.row : 3
                Layout.column : 1

                implicitHeight: parent.height - (seektabWindowRect.height + seekprogressBarRect1.height*2)
                implicitWidth : parent.width - (seekleftWindow.width + seekrightWindow.width + 2 * layoutSpacing)

                color:"#ffffff"

                visible: true


                ChartView {
                    id:seekgraphChartItem

                    property int  graphSpacing:10;
                    property real deltaX;
                    property real deltaYAmplitude;
                    property real deltaYFrequency;
                    property real deltaYPower;
                    property real deltaYCurrent;
                    property real deltaYPhase;

                    property real plotStartPointX;
                    property real plotEndPointX;
                    property real plotStartPointY;
                    property real plotEndPointY;

                    property int samplesPlotted:0;
                    property int totalSamples:0;

                    property real stopFreq: 4
                    property real startFreq: 0
                    property real centerFreq: 2

                    property bool hornPlotInProgress: false
                    property bool hornPlotRequested: false

                    property real lineWidth : 0.5

                    property int seekgraphNavigatorPosition:0

                    objectName: qsTr("graphPlotseek")
                    animationOptions: ChartView.SeriesAnimations

                    //signal getHornSignature(int idVal);

                    x:0
                    y:seekgraphNavigatorImage.height + seekgraphNavigatorLabel.height

                    height: parent.height - (y)
                    width:parent.width


                    plotStartPointX: plotArea.x
                    plotEndPointX: plotArea.x + plotArea.width
                    plotStartPointY: plotArea.y
                    plotEndPointY: plotArea.y + plotArea.height
                    deltaX: plotArea.width / (seekTimeAxis.max - seekTimeAxis.min)
                    deltaYAmplitude: plotArea.height / (seekAmpAxis.max - seekAmpAxis.min)
                    deltaYFrequency: plotArea.height / (seekFreqAxis.max - seekFreqAxis.min)
                    deltaYPower: plotArea.height / (seekPowerAxis.max - seekPowerAxis.min)
                    deltaYCurrent: plotArea.height / (seekCurrAxis.max - seekCurrAxis.min)
                    deltaYPhase: plotArea.height / (seekPhaseAxis.max - seekPhaseAxis.min)

                    localizeNumbers: true

                    Component.onCompleted:
                    {
                        seekgraphNavigatorPosition = seekTimeAxis.min ;
                    }

                    onStartFreqChanged:
                    {
                        seekTimeAxis.min = seekgraphChartItem.startFreq ;
                        seekphase0Plot.removePoints(0 , 1);
                        seekphase0Plot.insert(0, seekTimeAxis.min , 0);
                    }

                    onStopFreqChanged:
                    {
                        seekTimeAxis.max = seekgraphChartItem.stopFreq ;
                        seekphase0Plot.removePoints(1 , 1);
                        seekphase0Plot.insert(1 , seekTimeAxis.max , 0)
                    }

                    ValueAxis {
                        id:seekTimeAxis
                        color: "#000000"
                        gridVisible: false
                        labelsVisible: false
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextTimeAxis
                        titleVisible: false
                        labelFormat: "%.5f"
                        max: 4
                        min: 0
                        tickCount: 5
                    }

                    ValueAxis {
                        id:seekAmpAxis
                        color: "#f03e3e"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#f03e3e"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextAmplitudeAxis
                        labelFormat: "%.0f"
                        titleVisible: false
                        max: 20
                        min:0
                        tickCount: 11
                    }
                    ValueAxis {
                        id:seekAmpAxis1
                        color: "#f03e3e"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#f03e3e"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextAmplitudeAxis
                        labelFormat: "%.0f"
                        titleVisible: false
                        max: 20
                        min:0
                        tickCount: 11
                    }

                    ValueAxis {
                        id:seekCurrAxis
                        color: "#009587"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#009587"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextCurrentAxis
                        titleVisible: false
                        max: 10
                        min:0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }
                    ValueAxis {
                        id:seekCurrAxis1
                        color: "#009587"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#009587"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextCurrentAxis
                        titleVisible: false
                        max: 10
                        min:0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }

                    ValueAxis {
                        id:seekPhaseAxis
                        color: "black"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "black"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextPhaseAxis
                        titleVisible: false
                        max: 180
                        min: -180
                        tickCount: 11
                        labelFormat: "%.0f"
                    }
                    ValueAxis {
                        id:seekPhaseAxis1
                        color: "black"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "black"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextPhaseAxis
                        titleVisible: false
                        max: 180
                        min: -180
                        tickCount: 11
                        labelFormat: "%.0f"
                    }

                    ValueAxis {
                        id:seekFreqAxis
                        color: "#4285f4"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#4285f4"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextFrequencyAxis
                        titleVisible: false
                        max: 20000
                        min: 0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }
                    ValueAxis {
                        id:seekFreqAxis1
                        color: "#4285f4"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#4285f4"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextFrequencyAxis
                        titleVisible: false
                        max: 20000
                        min: 0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }

                    ValueAxis {
                        id:seekPowerAxis
                        color: "#a82ba8"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#a82ba8"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextPowerAxis
                        titleVisible: false
                        labelFormat: "%.0f"
                        max: 10
                        min:0
                        tickCount: 11
                    }
                    ValueAxis {
                        id:seekPowerAxis1
                        color: "#a82ba8"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#a82ba8"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextPowerAxis
                        titleVisible: false
                        labelFormat: "%.0f"
                        max: 10
                        min:0
                        tickCount: 11
                    }

                    SplineSeries
                    {
                        id: seekAmpPlot
                        width: seekgraphChartItem.lineWidth
                        color: "#f03e3e"
                        name: qmltextseekAmpPlot
                        axisX: seekTimeAxis
                        axisY: seekAmpAxis
                        useOpenGL: true
                        visible: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: seekAmpPlot1
                        width: seekgraphChartItem.lineWidth
                        color: "#f03e3e"
                        name: qmltextseekAmpPlot
                        axisX: seekTimeAxis
                        axisYRight: seekAmpAxis1
                        useOpenGL: true
                        visible: false
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: seekCurrPlot
                        width: seekgraphChartItem.lineWidth
                        color: "#009587"
                        name: qmltextseekCurrPlot
                        axisX: seekTimeAxis
                        axisY: seekCurrAxis
                        useOpenGL: true
                        visible: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: seekCurrPlot1
                        width: seekgraphChartItem.lineWidth
                        color: "#009587"
                        name: qmltextseekCurrPlot
                        axisX: seekTimeAxis
                        axisYRight: seekCurrAxis1
                        useOpenGL: true
                        visible: false
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: seekPhasePlot
                        width: seekgraphChartItem.lineWidth
                        name: qmltextseekPhasePlot
                        axisX: seekTimeAxis
                        axisY: seekPhaseAxis
                        color: "#000001"
                        useOpenGL: true
                        visible: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: seekPhasePlot1
                        width: seekgraphChartItem.lineWidth
                        name: qmltextseekPhasePlot
                        axisX: seekTimeAxis
                        axisYRight: seekPhaseAxis1
                        color: "#000001"
                        useOpenGL: true
                        visible: false
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: seekFreqPlot
                        width: seekgraphChartItem.lineWidth
                        name: qmltextseekFreqPlot
                        axisX: seekTimeAxis
                        axisY: seekFreqAxis
                        color: "#4285f4"
                        useOpenGL: true
                        visible: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: seekFreqPlot1
                        width: seekgraphChartItem.lineWidth
                        name: qmltextseekFreqPlot
                        axisX: seekTimeAxis
                        axisYRight: seekFreqAxis1
                        color: "#4285f4"
                        useOpenGL: true
                        visible: false
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: seekPowerPlot
                        width: seekgraphChartItem.lineWidth
                        name: qmltextseekPowerPlot
                        axisX: seekTimeAxis
                        axisY: seekPowerAxis
                        color: "#a82ba8"
                        useOpenGL: true
                        visible: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: seekPowerPlot1
                        width: seekgraphChartItem.lineWidth
                        name: qmltextseekPowerPlot
                        axisX: seekTimeAxis
                        axisYRight: seekPowerAxis1
                        color: "#a82ba8"
                        useOpenGL: true
                        visible: false
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: seekphase0Plot
                        name:qmltextseekphaseAxis
                        axisX: seekTimeAxis
                        axisYRight: seekPhaseAxis
                        width: seekgraphChartItem.lineWidth
                        XYPoint {x: seekTimeAxis.max; y:0}
                        XYPoint {x: seekTimeAxis.min; y:0}
                        color: "#000000"
                        style: Qt.DashLine
                        useOpenGL: true
                        visible: true
                        pointsVisible: true
                    }

                    Rectangle{
                        id: seekchartViewArea

                        x: seekgraphChartItem.plotArea.x
                        y: seekgraphChartItem.plotArea.y - ( seekgraphNavigatorImage.height +  seekgraphNavigatorLabel.height + seekgraphSlider.raiseHeight)

                        height : seekgraphNavigator.height + seekgraphNavigatorImage.height + seekgraphNavigatorLabel.height
                        width : seekgraphChartItem.plotArea.width

                        visible: false

                    }
                    Item {
                        id: seekgraphSlider

                        property int raiseHeight: 30
                        property string custColor : qmlPrimaryColor
                        property int assetSize:32

                        property real xAxisVal: seekTimeAxis.min

                        property string freqLabel:seekTimeAxis.min

                        y: seekgraphChartItem.plotStartPointY - ( seekgraphNavigatorImage.height +  seekgraphNavigatorLabel.height + raiseHeight)

                        x: seekgraphChartItem.plotStartPointX + (xAxisVal - seekTimeAxis.min) * seekgraphChartItem.deltaX - width / 2

                        width : seekgraphNavigatorLabel.width

                        height : seekgraphNavigator.height + seekgraphNavigatorImage.height + seekgraphNavigatorLabel.height

                        Rectangle
                        {
                            id: seekgraphNavigator

                            // border.width: 1
                            height: seekgraphChartItem.plotArea.height + parent.raiseHeight
                            width:1

                            anchors.bottom: parent.bottom

                            anchors.horizontalCenter: parent.horizontalCenter

                            color: parent.custColor
                        }

                        Rectangle {
                            id: seekgraphNavigatorImage

                            height: parent.assetSize * fontSizeScaleFactor
                            width : parent.assetSize * fontSizeScaleFactor

                            radius: width / 2

                            color: parent.custColor

                            anchors.bottom: seekgraphNavigator.top

                            anchors.horizontalCenter: parent.horizontalCenter

                            MouseArea {
                                id: seekgraphNavigatorMouseArea

                                property point posInChartArea
                                property int indexClosest1:0
                                property int indexClosest2:0

                                anchors.fill: parent

                                cursorShape: Qt.ToolTip

                                hoverEnabled: false

                                onPositionChanged: {

                                    /* Drag position of the slider */
                                    var point = mapToItem(seekchartViewArea , mouse.x , mouse.y);
                                    var retVal ;
                                    var timePoint;
                                    var ampPoint = 0.0;
                                    var freqPoint = 0;
                                    var powerPoint = 0;
                                    var currPoint = 0 ;
                                    var phasePoint = 0 ;

                                    if(point.x < 0 || point.x > seekgraphChartItem.plotArea.width)
                                    {
                                        if(point.x < 0)
                                        {
                                            posInChartArea.x = 0 ;
                                            timePoint = seekTimeAxis.min ;
                                        }
                                        else
                                        {
                                            posInChartArea.x = seekgraphChartItem.plotArea.width ;
                                            timePoint = seekTimeAxis.max ;
                                        }
                                    }
                                    else
                                    {
                                        posInChartArea.x = point.x;
                                        posInChartArea.y = point.y;

                                        timePoint = posInChartArea.x / seekgraphChartItem.deltaX + seekTimeAxis.min;
                                    }


                                    seekgraphSlider.xAxisVal = (seekTimeAxis.min + posInChartArea.x / seekgraphChartItem.deltaX );

                                    retVal = getClosestPoints(timePoint.toFixed(5));

                                    if(-1 == retVal)
                                    {
                                        timePoint = 0;
                                        ampPoint = 0;
                                        freqPoint = 0 ;
                                        powerPoint = 0 ;
                                        currPoint = 0 ;
                                        phasePoint = 0 ;
                                    }
                                    else
                                    {
                                        var tmpPoint = seekAmpPlot.at(indexClosest1);
                                        ampPoint = tmpPoint.y;
                                        timePoint = tmpPoint.x;

                                        tmpPoint = seekFreqPlot.at(indexClosest1);
                                        freqPoint = tmpPoint.y;

                                        tmpPoint = seekPowerPlot.at(indexClosest1);
                                        powerPoint = tmpPoint.y;

                                        tmpPoint = seekCurrPlot.at(indexClosest1);
                                        currPoint = tmpPoint.y;

                                        tmpPoint = seekPhasePlot.at(indexClosest1);
                                        phasePoint = tmpPoint.y;
                                    }
                                    seekgraphSlider.freqLabel = timePoint.toFixed(5);
                                    seekresonantFreqValue.freqPoint = timePoint.toFixed(5);
                                    seekRecipeObj.setParamValues(ampPoint,freqPoint,powerPoint,currPoint,phasePoint);
                                }
                            }
                        }

                        Text {
                            id: seekgraphNavigatorLabel

                            anchors.bottom: seekgraphNavigatorImage.top
                            anchors.bottomMargin: 5

                            height :  font.pixelSize + 10
                            width: font.pixelSize * 12

                            anchors.horizontalCenter: seekgraphSlider.horizontalCenter

                            horizontalAlignment: Text.AlignHCenter
                            font.family : Style.semibold.name
                            font.pixelSize: Style.style4
                            text: seekgraphSlider.freqLabel +qmltextTimeUnit

                            color: parent.custColor
                        }

                    }

                    Text {
                        id: seekstartFreqLabel
                        y:parent.plotEndPointY + 5
                        x:(parent.plotStartPointX + parent.deltaX * (seekgraphChartItem.startFreq - seekTimeAxis.min)) - 20
                        horizontalAlignment: Text.AlignHCenter
                        font.family : Style.regular.name
                        font.pixelSize: Style.style1
                        text: seekgraphChartItem.startFreq + qmltextTimeUnit
                        color: "#000000"
                    }


                    Text {
                        id: seekstopFreqLabel

                        y:parent.plotEndPointY + 5

                        x:(parent.plotStartPointX + parent.deltaX * (seekgraphChartItem.stopFreq - seekTimeAxis.min)) - 20

                        horizontalAlignment: Text.AlignHCenter
                        font.family : Style.regular.name
                        font.pixelSize: Style.style1
                        text: seekgraphChartItem.stopFreq + qmltextTimeUnit
                        color: "#000000"
                    }


                    Text {
                        id: seekcenterFreqLabel

                        y:parent.plotEndPointY + 5

                        x:(parent.plotStartPointX + parent.deltaX * (seekgraphChartItem.centerFreq - seekTimeAxis.min)) - 20

                        horizontalAlignment: Text.AlignHCenter
                        font.family : Style.regular.name
                        font.pixelSize: Style.style1
                        text: seekgraphChartItem.centerFreq + qmltextTimeUnit
                        color: "#000000"
                        visible: false
                    }

                    legend.visible: false
                }
            }

            Rectangle {
                id : seekrightWindow

                Layout.row : 2
                Layout.column : 2
                implicitHeight : parent.height - (seektabWindowRect.height + seekprogressBarRect1.height + layoutSpacing)
                Layout.rowSpan: 2

                Layout.minimumWidth: 340 * fontSizeScaleFactor

                Layout.topMargin: layoutSpacing

                implicitWidth : parent.width * 0.2
                color : "#e9ecef"
                clip: true

                anchors.right:parent.right

                visible: true

                Rectangle {
                    id:seekrightWindowColumnRect

                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        top: parent.top

                        leftMargin: 2 * layoutSpacing
                        rightMargin: 2 * layoutSpacing
                        topMargin: 2 * layoutSpacing
                        bottomMargin: 2 * layoutSpacing
                    }

                    color: parent.color

                    Column {
                        id: seekrightWindowColumn


                        y: -(seekrightWindowVbar.position * ( seekrightWindowColumn.height  + 52))
                        spacing: (window.height > 900 ? 2 : (window.height == 900 ? 1.5 : 1)) * layoutSpacing


                        anchors.left: parent.left
                        anchors.right: parent.right
                        Layout.alignment: Qt.AlignTop

                        Layout.minimumWidth: 260 * scaleRatio



                        Rectangle {
                            id: seektitleRectanngle

                            width: parent.width
                            height: Math.max( 40 * scaleRatio + layoutSpacing , Math.min(50 * scaleRatio , parent.height * 0.15))
                            color : "#e9ecef"


                            Text {
                                id: seektitleRectText
                                x: 0
                                y: 0
                                height: parent.height
                                width: parent.width * 0.6
                                color: "#868e96"
                                font.family : Style.semibold.name
                                wrapMode: TextEdit.WordWrap
                                font.pixelSize: Style.style4
                                text: qmltextResonantFreq

                            }

                            Rectangle {
                                id: seekScanImageRect

                                height: width
                                width : Math.min(parent.width * 0.4 , 40 * scaleRatio)
                                anchors.right : parent.right
                                color: qmlPrimaryColor
                                visible: false
                                Image {
                                    id: seekImage
                                    x: 0
                                    y: 0
                                    width: Math.min ( parent.width , parent.height ) - 2 * 5
                                    height: Math.min ( parent.width , parent.height ) - 2 * 5
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/images/image/delta.svg" + qmlPrimaryColor
                                }
                            }
                        }
                        Rectangle {
                            id: seekresonantFreqRect
                            height: Math.max(25 , Math.min ( 45 * scaleRatio , parent.height * 0.1))
                            width:parent.width

                            color: "#898e96"

                            Text {
                                id:seekresonantFreqTitle
                                height: parent.height
                                width: parent.width * 0.4
                                color: "#ffffff"
                                text: qmltextTime
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                verticalAlignment: Text.AlignVCenter
                                font.family: Style.semibold.name
                                font.pixelSize: Style.style1
                                elide: Text.ElideRight

                            }

                            Text {
                                id:seekresonantFreqValue

                                property var freqPoint:0;
                                height: parent.height
                                width: parent.width * 0.4
                                color: "#ffffff"
                                text: freqPoint + qmltextTimeUnit
                                anchors.right: parent.right
                                anchors.rightMargin: layoutSpacing
                                verticalAlignment: Text.AlignVCenter
                                font.family: Style.semibold.name
                                font.pixelSize: Style.style2

                                horizontalAlignment: Text.AlignRight

                                elide : Text.ElideRight

                            }
                        }



                        GridView
                        {
                            id:graphParamViewSeek

                            property int paramIndex:1

                            width:parent.width
                            height: parent.height * 0.8

                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            clip:true
                            cellWidth: graphParamViewSeek.width
                            cellHeight: 50
                            model:seekScanCheckBoxModel

                            delegate:
                                GraphRightParamDelegate {
                                id: seekGraphRightParamDelegate
                                width : graphParamViewSeek.cellWidth
                                height: graphParamViewSeek.cellHeight

                                comboBox.onCurrentIndexChanged:
                                {
                                    leftAndRightSwitch(index,comboBox.currentIndex,checkBox.checked)
                                }

                                checkBox.onCheckedStateChanged :
                                {
                                    changeGraphAxisVisiblity(index,checkBox.checked)
                                }
                                Component.onCompleted:
                                {
                                    comboBox.currentIndex = model.modelData.isLeftAxis;
                                    checkBox.checked = model.modelData.isCheckBoxSelected;
                                }
                            }
                            Component.onCompleted:
                            {
                                seekRecipeObj.setSeekDefaultValues()
                            }
                            Component.onDestruction:
                            {
                                seekRecipeObj.getSeekDefaultValues()
                            }
                        }
                    }
                }

                ScrollBar {
                    id: seekrightWindowVbar
                    hoverEnabled: true
                    active:hovered || pressed

                    size: seekrightWindow.height / (seekrightWindowColumn.height + 52)
                    anchors.top : parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }

            }

            Rectangle {
                id: seekRecipeParamWindow

                Layout.row:1
                Layout.column:0
                Layout.columnSpan:3

                implicitHeight: parent.height * 0.85

                implicitWidth: parent.width
                Layout.alignment: Qt.AlignTop

                visible: false

                color: "#ffffff"

                GridView
                {
                    id: seekgrid
                    x: 50
                    y: 50
                    width: parent.width-50
                    height: parent.height-50

                    cellHeight: height/5
                    cellWidth: width/3
                    boundsBehavior: Flickable.StopAtBounds
                    flickableDirection: Flickable.AutoFlickDirection
                    clip: true

                    model: seekrecipeParameterModel
                    property bool refreshSwitch: false
                    delegate: WeldModeItem
                    {
                        paramText:model.modelData.parameterName
                        paramTextLocale:model.modelData.parameterLocaleName
                        paramValue:model.modelData.parameterValue
                        paramUnit:model.modelData.parameterUnit
                        height:  seekgrid.cellHeight-40
                        width:  seekgrid.cellWidth-80
                        isSwitch: model.modelData.parameterSwitch
                        checkStatus:model.modelData.parameterSwitchState
                        toggleSwitch.onPressedChanged:
                        {
                            seekgrid.refreshSwitch = true
                        }
                        toggleSwitch.onCheckedChanged:
                        {
                            if(seekgrid.refreshSwitch == true)
                            {
                                seekgrid.refreshSwitch = false
                                if(toggleSwitch.checked)
                                    seekRecipeObj.modifyParameter("1",paramText)
                                else
                                    seekRecipeObj.modifyParameter("0",paramText)
                            }
                        }
                        control_mode_mouseArea.onClicked:
                        {
                            if(model.modelData.parameterName != qmltextMemoryClearBeforeSeek)
                            {
                                steppingNumPad.parameterText = model.modelData.parameterName
                                steppingNumPad.parameterLocaleText = model.modelData.parameterLocaleName
                                steppingNumPad.unitText = model.modelData.parameterUnit
                                steppingNumPad.keyInputText = model.modelData.parameterValue
                                steppingNumPad.minValue = model.modelData.parameterMinValue
                                steppingNumPad.maxValue = model.modelData.parameterMaxValue
                                steppingNumPad.checkParameter( model.modelData.parameterName )
                                numPadStatus = 1
                                steppingNumPad.open()
                                enableMainwindowOpacity.start();
                                steppingNumPad.dialoguerecipeIndex=index;
                                steppingNumPad.editrecipeflag= false
                                steppingNumPad.addrecipeflag= false
                                steppingNumPad.paramAtoZflag= false
                                steppingNumPad.editHornRecipeFlag= false
                                steppingNumPad.editSeekRecipeFlag= true
                                steppingNumPad.ultrasonicAdvancedFlag=false
                                steppingNumPad.afterBurst = false
                                steppingNumPad.preTrigger = false
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar
                    {
                        parent: seekgrid
                        anchors.top: seekgrid.top
                        anchors.bottom: seekgrid.bottom
                    }
                }
            }

            Rectangle {
                id: seekRecipeControlWindow

                Layout.row: 3
                Layout.column: 0
                Layout.columnSpan:3

                implicitHeight: parent.height * 0.15
                implicitWidth: parent.width
                Layout.alignment:Qt.AlignTop

                color:"#ffffff"

                visible: false

                PrimaryButton {
                    id: seeksaveButton
                    //                    width:  Math.max(140*scaleRatio , parent.width * 0.2 )
                    //                    height: Math.min(40 , (parent.height - ( 2 * 10)) )
                    text: qmltextSAVE
                    visible: true
                    anchors.verticalCenter: parent.verticalCenter

                    anchors.right: parent.right
                    anchors.rightMargin: 2 * layoutSpacing
                    onClicked: {
                        var retVal = 0 ;

                        var paramChangeFlag = seekRecipeObj.getParameterChangedFlag() ;

                        if(true == paramChangeFlag)
                        {
                            retVal = seekRecipeObj.saveUserData();

                            if(0 != retVal)
                            {
                                recipe.updateStatusText(qmltextFailToSaveSeekRecipe)
                            }
                            else
                            {
                                recipe.updateStatusText(qmltextSeekRecipeSaveSuccess)
                            }
                        }
                        //statusProgressBar.progressVal = 0.0
                    }
                }

                SecondaryButton {
                    id: seekcancelButton
                    text: qmltextCANCEL
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: seeksaveButton.left
                    anchors.rightMargin: layoutSpacing
                    onClicked: {
                        seekRecipeObj.cancelUserData();
                        //statusProgressBar.progressVal = 0.0
                    }
                }

                SecondaryButton {
                    id: seekresetButton
                    text: qmltextRESETTODEFAULT
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * layoutSpacing

                    onClicked: {
                        seekRecipeObj.resetSeekRecipeData();
                    }
                }
            }
        }
        Text {
            id: text_seek
            x:20
            // width: 605

            y:seekThreeway.y+seekThreeway.height/2-height/2
            // height: 71
            color: "#868e96"
            text: qmltextSEEK

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.pixelSize: Style.style6
            font.family: Style.semibold.name
        }
    }

    property var itemComponent;
    property var objInst;

    /**
    *@breif: To get the closest points
    **/
    function getClosestPoints(xval)
    {
        var length = seekAmpPlot.count;
        var point ;
        var startFreq = 0;
        var startFreq1 = 0;
        var endFreq = 0;
        var freqStep = 0 ;
        var closestVal = 9999 ;
        var closestInd = 0;

        var tmpVal = 0 ;

        if(0 == length)
        {
            return -1 ;
        }

        point = seekAmpPlot.at(0);
        startFreq = point.x ;
        point = seekAmpPlot.at(1);
        startFreq1 = point.x ;

        point = seekAmpPlot.at(length - 1)
        endFreq = point.x;

        if((xval < startFreq) || (xval > endFreq))
        {
            return -1;
        }

        point = seekAmpPlot.at(2);
        freqStep = point.x - startFreq1 ;
        if(freqStep==0)
            freqStep=1
        if((xval - startFreq) == 0)
            closestInd = 1
        else
            closestInd = (xval - startFreq) / freqStep;
        closestVal = 0;
        point = seekAmpPlot.at(closestInd);
        seekgraphNavigatorMouseArea.indexClosest1 = closestInd;
        seekgraphNavigatorMouseArea.indexClosest2 = closestInd+1;
        return 0;
    }


    /**
    *@breif: Update the horn result titleBar on the right window
    **/
    function getRightTitleString(idString)
    {
        var titleText = "";

        var numRegExp = /(^-?\d\d*\.\d*$)|(^-?\d\d*$)|(^-?\.\d\d*$)/

        var status = numRegExp.test(idString);

        if(true == status)
        {
            titleText = qmltextResonantFreq + idString;
        }
        else
        {
            titleText = idString;
        }

        return titleText ;
    }

    /* Function to plot the graph data of the Seek Scan UI from backend */

    /**
    *@breif: To plot the seek graph
    **/
    function seekPlotGraph()
    {
        var retVal = 0 ;

        var startFreqValue = 0 ;

        var hornPsFreq = 0 ;
        var axisMaxValues ;
        var axisMinValues ;

        /* Clear the graph Samples */
        clearGraphSamples();

        /* Append the points for each of the parameters */
        seekhistoryDataObj.retrieveSeekSigPoints(seekgraphChartItem.series(qmltextseekAmpPlot) , 1);
        seekhistoryDataObj.retrieveSeekSigPoints(seekgraphChartItem.series(qmltextseekFreqPlot) , 2);
        seekhistoryDataObj.retrieveSeekSigPoints(seekgraphChartItem.series(qmltextseekPowerPlot) , 3);
        seekhistoryDataObj.retrieveSeekSigPoints(seekgraphChartItem.series(qmltextseekCurrPlot) , 4);
        seekhistoryDataObj.retrieveSeekSigPoints(seekgraphChartItem.series(qmltextseekPhasePlot) , 5);

        /* Updating the start and stop freq */
        var point = seekAmpPlot.at(0);
        var length = seekAmpPlot.count ;
        if(length == 0)
        {
            seekgraphChartItem.startFreq = 0;
            seekgraphChartItem.stopFreq = 4
            startFreqValue=0;
        }
        else
        {
            startFreqValue = point.x ;
            seekgraphChartItem.startFreq = startFreqValue;

            point = seekAmpPlot.at(length - 1);
            seekgraphChartItem.stopFreq = point.x;
        }
        hornPsFreq = seekRecipeObj.getSeekPsFreq();

        /* Initialize the vaues */
        seekresonantFreqValue.freqPoint = startFreqValue

        /* Repositioning the graph Slider */
        seekgraphSlider.xAxisVal = startFreqValue ;
        seekgraphSlider.freqLabel = startFreqValue ;
        seekRecipeObj.setParamValues(0,0,0,0,0);

        /* Modifying the axis values */
        axisMaxValues = seekhistoryDataObj.seekAxisMaxValues();
        axisMinValues = seekhistoryDataObj.seekAxisMinValues();

        /* Calibrating the axis values */

        //seekTimeAxis.min = axisMinValues[timeInd] ;
        //seekTimeAxis.max = axisMaxValues[timeInd] ;

        roundAxisValues(ampInd , (axisMaxValues[ampInd] + 5) , (axisMinValues[ampInd] - 5) , 50000000 , 0);
        roundAxisValues(freqInd , (axisMaxValues[freqInd] + 5) , (axisMinValues[freqInd] - 5) , 50000000 , 0);
        roundAxisValues(powerInd , (axisMaxValues[powerInd] + 5) , (axisMinValues[powerInd] - 5) , 50000000 , 0);
        roundAxisValues(curInd , (axisMaxValues[curInd] + 5) , (axisMinValues[curInd] - 5) , 50000000 , 0);
        roundAxisValues(phaseInd , (axisMaxValues[phaseInd] + 5) , (axisMinValues[phaseInd] - 5) , 50000000 , 0);

    }

    /**
    *@breif: To roundup the axis values
    **/
    function roundAxisValues(index , axisMaxVal , axisMinVal , paramMax , paramMin)
    {
        var tmpAxisMaxVal;
        var tmpAxisMinVal;

        var diffVal ;
        var reminder;
        var retVal = 0 ;

        tmpAxisMaxVal = axisMaxVal ;
        tmpAxisMinVal = axisMinVal ;

        /* Calculate the reminder value */
        diffVal = tmpAxisMaxVal - tmpAxisMinVal;

        /* Tick count for an axis = 10. Calculate the reminder as modulo */
        reminder = (diffVal % 10)

        /* Recalibrate the Min and Maximum axis val to ensure it indicates rounded values for tick axis 10 */
        if(reminder < 5)
        {
            /* The sign factor considered when subtracting reminder */
            /* Subtract from the absolute value and multiply by the sign */
            tmpAxisMinVal = (Math.abs(tmpAxisMinVal) - reminder) *(tmpAxisMinVal / Math.abs(tmpAxisMinVal))
        }
        else
        {
            tmpAxisMaxVal += (10 - reminder) ;
        }

        /* Check if values are greater or lesser than maximum values for the axis and recalibrate data */
        if((tmpAxisMaxVal > paramMax) || (tmpAxisMinVal < paramMin))
        {
            if(tmpAxisMaxVal > paramMax)
            {
                reminder = tmpAxisMaxVal - paramMax ;
                tmpAxisMaxVal = paramMax ;

                tmpAxisMinVal = (Math.abs(tmpAxisMinVal) - (reminder % 10)) *(tmpAxisMinVal / Math.abs(tmpAxisMinVal))

                Math.max(tmpAxisMinVal , paramMin)
            }
            else
            {
                reminder = paramMin - tmpAxisMinVal ;
                tmpAxisMinVal = paramMin ;

                tmpAxisMaxVal = tmpAxisMaxVal + (reminder % 10)

                Math.min(tmpAxisMaxVal , paramMax)
            }
        }

        /* Update the axis ticks */
        switch(index)
        {
        case ampInd:
            seekAmpAxis.max = seekAmpAxis1.max = 20
            seekAmpAxis.min  = seekAmpAxis1.min = 0
            break ;
        case freqInd:
            seekFreqAxis.max = seekFreqAxis1.max = tmpAxisMaxVal
            seekFreqAxis.min = seekFreqAxis1.min = tmpAxisMinVal
            break ;
        case powerInd:
            seekPowerAxis.max = seekPowerAxis1.max = 10
            seekPowerAxis.min = seekPowerAxis1.min = 0
            break;
        case curInd:
            seekCurrAxis.max = seekCurrAxis1.max =  10
            seekCurrAxis.min = seekCurrAxis1.min = 0
            break ;
        case phaseInd:
            seekPhaseAxis.max = seekPhaseAxis1.max = 180
            seekPhaseAxis.min = seekPhaseAxis1.min = -180
            break;
        }
    }

    /**
    *@breif: Refreshes the history data
    **/
    function seekrefreshHistory()
    {
        var retVal = 0 ;
        var titleTextString = "" ;
        /* Reset the index Selected */
        seekhistoryListCtrlView.indexSelected = 0 ;

        seekhistoryDataObj.seekIndSelected = seekhistoryListCtrlView.indexSelected ;

        /* Refresh the history */
        retVal = seekhistoryDataObj.refreshSeekHistory();

        if(0 == retVal)
        {
            titleTextString = seekhistoryListCtrlView.model[seekhistoryListCtrlView.indexSelected].scanId
        }

        /* Update the horn result titleBar on the right window */
        seektitleRectText.text = getRightTitleString(titleTextString);

        return retVal ;
    }

    /**
    *@breif:To clear all graph samples
    **/
    function clearGraphSamples()
    {
        seekAmpPlot.clear();
        seekAmpPlot1.clear();
        seekFreqPlot.clear();
        seekFreqPlot1.clear();
        seekPowerPlot.clear();
        seekPowerPlot1.clear();
        seekCurrPlot.clear();
        seekCurrPlot1.clear();
        seekPhasePlot.clear();
        seekPhasePlot1.clear();
    }

    /**
    *@breif: To switch between right and left axis
    **/
    function leftAndRightSwitch(index,currentIndex,isCheckBoxSelected)
    {
        if(index < 0)
            index = 0

        seekRecipeObj.setAxisIndex(index,currentIndex);
        if(index == 0)//Amplitude
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    seekAmpAxis1.visible =false
                    seekAmpAxis.visible =true
                }
                else
                {
                    seekAmpAxis1.visible =true
                    seekAmpAxis.visible =false
                }
            }
            else
            {
                seekAmpAxis1.visible =false
                seekAmpAxis.visible =false
                seekAmpPlot.visible = false
            }
        }
        else if(index == 1)//Freq
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    seekFreqAxis1.visible =false
                    seekFreqAxis.visible =true
                }
                else
                {
                    seekFreqAxis1.visible =true
                    seekFreqAxis.visible =false
                }
            }
            else
            {
                seekFreqAxis1.visible =false
                seekFreqAxis.visible =false
                seekFreqPlot.visible = false
            }
        }
        else if(index == 2)//Power
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    seekPowerAxis1.visible =false
                    seekPowerAxis.visible =true
                }
                else
                {
                    seekPowerAxis1.visible  = true
                    seekPowerAxis.visible   = false
                }
            }
            else
            {
                seekPowerAxis1.visible  = false
                seekPowerAxis.visible   = false
                seekPowerPlot.visible   = false
            }
        }
        else if(index == 3)//Current
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    seekCurrAxis1.visible =false
                    seekCurrAxis.visible =true
                }
                else
                {
                    seekCurrAxis1.visible =true
                    seekCurrAxis.visible =false
                }
            }
            else
            {
                seekCurrAxis1.visible =false
                seekCurrAxis.visible =false
                seekCurrPlot.visible = false
            }
        }
        else if(index == 4)//Phase
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    seekPhaseAxis1.visible =false
                    seekPhaseAxis.visible =true
                }
                else
                {
                    seekPhaseAxis1.visible =true
                    seekPhaseAxis.visible =false
                }
            }
            else
            {
                seekPhaseAxis1.visible =false
                seekPhaseAxis.visible =false
                seekPhasePlot.visible = false
                seekphase0Plot.visible = false;
            }
        }
    }

    /**
    *@breif: Change graph axis to left or right based on selection
    **/
    function changeGraphAxisVisiblity(dwIndex,ischecked)
    {
        if(dwIndex < 0)
            dwIndex = 0

        var flag = seekRecipeObj.getComboIndex(dwIndex)
        switch(dwIndex)
        {
        case 0:
            seekAmpPlot.visible = ischecked
            seekAmpAxis.visible = ischecked && (!flag)
            seekAmpAxis1.visible = ischecked && (flag)
            break;
        case 1:
            seekFreqPlot.visible = ischecked
            seekFreqAxis.visible = ischecked && (!flag)
            seekFreqAxis1.visible = ischecked && (flag)
            break;
        case 2:
            seekPowerPlot.visible = ischecked
            seekPowerAxis.visible = ischecked && (!flag)
            seekPowerAxis1.visible = ischecked && (flag)
            break;
        case 3:
            seekCurrPlot.visible = ischecked
            seekCurrAxis.visible = ischecked && (!flag)
            seekCurrAxis1.visible = ischecked && (flag)
            break;
        case 4:
            seekPhasePlot.visible = ischecked
            seekphase0Plot.visible = ischecked
            seekPhaseAxis.visible = ischecked && (!flag)
            seekPhaseAxis1.visible = ischecked && (flag)
            break;

        default:

            break;
        }
        seekRecipeObj.setCheckboxStatus(dwIndex,ischecked)
    }
}


