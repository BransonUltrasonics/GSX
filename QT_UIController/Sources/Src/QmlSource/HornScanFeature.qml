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
    readonly property string qmltextPhaseAxis:  qsTr("Phase")
    readonly property string qmltextFreqAxis: qsTr("Frequency")
    readonly property string qmltextAmpAxis:   qsTr("Amplitude")
    readonly property string qmltextCurrentAxis:  qsTr("Rated Current")
    readonly property string qmltextImpedanceAxis:  qsTr("Impedance")
    readonly property string qmltextAmpPlot:  qsTr("ampPlot")
    readonly property string qmltextCurrentPlot: qsTr("currPlot")
    readonly property string qmltextPhasePlot:qsTr("phasePlot")
    readonly property string qmltextPhaseAxisPlot:qsTr("phaseAxis")
    readonly property string qmltextImpedancePlot:qsTr("impedancePlot")
    readonly property string qmltextHornSigReadFailed:qsTr("Horn signature read failed. Check network connection")
    readonly property string qmltextHornScanInProgress:qsTr("Horn Scan is in progress. Another request cannot be processed")
    readonly property string qmltextHornRecipeSaveToSCFailed:qsTr("Horn Recipe save to Supervisory Controller failed")
    readonly property string qmltextHornScanTimeOut:qsTr("Horn Scan Operation Timed Out")
    readonly property string qmltextHornScanFailed:qsTr("Horn Scan Operation Failed")
    readonly property string qmltextHornScanSuccess:qsTr("Horn Scan Operation Success")
    readonly property string qmltextAbortScan: qsTr("ABORT SCAN")
    readonly property string qmltextAbortingReq:qsTr("No Horn Scan in operation. Aborting Request")
    readonly property string qmltextScanCouldNotBeAborted:qsTr("Horn Scan could not be aborted. Check the system status")
    readonly property string qmltextAbortSuccess:qsTr( "Horn Scan aborted Successfully")
    readonly property string qmltextProgressBar:qsTr("progressBar")
    readonly property string qmltextStatus:qsTr("Status")
    readonly property string qmltextHornRecipe:qsTr("Horn Recipe")
    readonly property string qmltextHornRecipeTitleBar:qsTr("HORN RECIPE")
    readonly property string qmltextGraphPlot:qsTr("graphPlot")
    readonly property string qmltextHORNSCAN:qsTr("HORN SCAN")
    readonly property string qmltextFailToSaveHornScanId:qsTr("Failed to save Horn Scan ID. Index not contained in DB")
    readonly property string qmltextScanIdAlreadyPresent:qsTr("Scan ID requested to be saved already present in the Database. Try with a different scan ID")
    readonly property string qmltextFailToSaveHornId:qsTr("Failed to save Horn Scan ID")
    readonly property string qmltextNoHistory:qsTr("No history found")
    readonly property string qmltextDigitalTune: qsTr("Digital Tune")
    readonly property string qmltextFrequency:qsTr("Frequency")
    readonly property string qmltextSAVE:qsTr("SAVE")
    readonly property string qmltextFailToSaveHornRecipeData:qsTr("Failed to save Horn Recipe data. Check for network connection or Recipe parameters check")
    readonly property string qmltextHornRecipeSaveSuccess:qsTr("Horn recipe saved successfully")
    readonly property string qmltextCancel:qsTr("CANCEL")
    readonly property string qmltextResetToDefaults:qsTr("RESET TO DEFAULT")
    readonly property string qmltextHornScan:qsTr("Horn Scan ")
    readonly property string qmltextStartHornScan:qsTr("START HORN SCAN")
    readonly property string qmltextResonantFrequency: qsTr("Resonant Freq ")
    readonly property string qmltextSetDigitalWithHornScan:  qsTr("Set Digital Tune with Horn Scan")
    readonly property string qmltextUnitHz:  qsTr(" Hz")


    id: itemHornScan
    visible: true
    width:  loaderelement.width
    height: loaderelement.height

    property double scaleRatio: 1
    property int tabWindowMargin: 10
    property int dotSpacing: 5
    property int assetSize: 32
    property int layoutSpacing: 10
    property int histItemCount: 1

    property int ampInd: 0
    property int curInd: 1
    property int phaseInd: 2
    property int impedanceInd: 3
    property int freqInd: 4

    property bool ampflag: true
    property bool currentflag: true
    property bool phaseflag: true
    property bool impedanceflag: true

    property var checkedImpeedanceStatus: false

    property double fontSizeScaleFactor : window.height / 800

    property alias historyList: historyListCtrlView

    Component.onCompleted:
    {
        threewayCarouselDots = true
        sysconfig.bransonSecondLevelFlag = false
    }
    Component.onDestruction:
    {
        sysconfig.bransonSecondLevelFlag = false
    }

    Rectangle {
        id: rootRect

        width: parent.width - 4
        height: parent.height - 4
        color: "#ffffff"
        visible: true

        /* Implement the entire screen as a layout for alignment on resizing as a 3 x 3 grid layout */
        GridLayout {
            id: mainLayout
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
                id: tabWindowRect
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
                id: leftWindow

                Layout.row : 1
                Layout. column : 0

                Layout.rowSpan : 3
                Layout.leftMargin: layoutSpacing
                Layout.rightMargin: layoutSpacing
                Layout.topMargin: layoutSpacing
                Layout.bottomMargin: layoutSpacing

                Layout.alignment: Qt.AlignTop

                Layout.maximumWidth: 260 * fontSizeScaleFactor

                implicitHeight: (parent.height - (tabWindowRect.height +  2 * layoutSpacing))
                implicitWidth : parent.width * 0.2
                color : "#ffffff"

                visible: true

                HistoryBasicBar
                {
                    id: histBasicBar
                    width: parent.width
                    height : (progressBarRect1.height + progressBarRect2.height) - layoutSpacing
                    visible: false
                }

                Rectangle {
                    id: historyListCtrlRect

                    property int numRecPerPage:0;

                    anchors.top: histBasicBar.bottom
                    anchors.bottom: parent.bottom
                    anchors.left:leftWindow.left
                    anchors.right:parent.right

                    color:"#f8f9fa"

                    clip: true


                    Component.onCompleted:
                    {
                        numRecPerPage = height / 60 ;

                        historyListCtrlView.indexSelected = historyDataObj.hornIndSelected ;
                    }


                    ListView {
                        id: historyListCtrlView
                        property int indexSelected:0;
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        height:parent.height
                        ScrollBar.vertical: ScrollBar
                        {
                            active: hovered || pressed
                        }

                        model:historyListCtrlModel
                        delegate: HistoryDelegate
                        {
                            Connections
                            {
                                target: historyListCtrlView
                                onCurrentIndexChanged:
                                {
                                    if(historyListCtrlView.currentIndex == index)
                                    {
                                        var titleTextString
                                        historyListCtrlView.indexSelected = index
                                        historyDataObj.hornIndSelected = historyListCtrlView.indexSelected
                                        if(model.modelData.scanName != "")
                                        {
                                            titleTextString = model.modelData.scanName
                                        }
                                        else
                                        {
                                            titleTextString = model.modelData.scanId
                                        }
                                        var retVal = 0 ;

                                        /* Update the horn result titleBar on the right window */
                                        titleRectText.text = getRightTitleString(titleTextString);

                                        /* Signal the Backend function to fetch the horn data and result from the DB */
                                        retVal = historyDataObj.getHornGraphDataFromDb(model.modelData.scanId);

                                        if(retVal == 0)
                                        {
                                            plotGraph();
                                        }
                                        else
                                        {
                                            recipe.updateStatusText(qmltextHornSigReadFailed)
                                        }
                                    }
                                }
                            }
                        }

                        Component.onCompleted:
                        {
                            if(historyListCtrlView.count != 0)
                            {
                                plotGraph();
                                var titleTextString
                                if(historyListCtrlView.model[historyListCtrlView.indexSelected].scanName != "")
                                {
                                    titleTextString = historyListCtrlView.model[historyListCtrlView.indexSelected].scanName
                                }
                                else
                                {
                                    titleTextString = historyListCtrlView.model[historyListCtrlView.indexSelected].scanId
                                }
                                var retVal = 0 ;

                                /* Update the horn result titleBar on the right window */
                                titleRectText.text = getRightTitleString(titleTextString);
                            }
                            positionViewAtIndex(historyListCtrlView.indexSelected , ListView.Center)
                        }
                    }
                }
            }

            Rectangle {
                id : progressBarRect1


                Layout.row : 1
                Layout.column : 1

                Layout.columnSpan : 2

                Layout.alignment: Qt.AlignTop

                implicitWidth : parent.width - ( leftWindow.width + 2 * layoutSpacing )

                implicitHeight : parent.height * 0.075

                visible:true

                PrimaryButton {
                    id: startButton

                    //                    width: Math.max(140*scaleRatio , parent.width * 0.22 - 50 )
                    //                    height: Math.min(40 , ((parent.height) - (  2 * 10)) )

                    //                    font.pixelSize: Style.style1
                    //                    font.family: Style.semibold.name
                    text: qmltextStartHornScan

                    anchors.verticalCenter: parent.verticalCenter

                    anchors.left : parent.left
                    anchors.leftMargin: 2 * 10

                    //primaryButton: true

                    onClicked:
                    {
                        var retVal = 0 ;
                        var hornOpStatus = historyDataObj.getHornOperationStatus() ;

                        /* Send horn recipe to SC */
                        if(true == hornOpStatus)
                        {
                            recipe.updateStatusText(qmltextHornScanInProgress)
                        }
                        else
                        {
                            retVal = hornRecipeObj.sendHornRecipeDataToSc();

                            if(0 == retVal)
                            {
                                /* Horn Recipe Save Failed */
                                recipe.updateStatusText(qmltextHornRecipeSaveToSCFailed)
                                scanAbortTimer.stop();
                            }
                            else
                            {
                                /* Request Start Scan to SC */
                                historyDataObj.performHornScan();

                                statusProgressBar.progressVal = 0.0 ;

                                /* Start horn scan timeout timer */
                                scanAbortTimer.start() ;

                                clearGraphSamples();

                                /* Set the horn Operation progress to true */
                                mainwindow.hornOpInProgress = true ;
                            }
                        }
                    }

                    Timer {
                        id: scanAbortTimer
                        interval: 10 * 1000 // 10s timer interval
                        running: false
                        repeat: false
                        onTriggered: {
                            /* Implement pop up for the horn scan timeout */
                            recipe.updateStatusText(qmltextHornScanTimeOut)
                            /* Reset the horn scan operation in progress backend flag */
                            abortButton.clicked() ;
                        }
                    }

                    Connections
                    {
                        target: historyDataObj

                        onHornScanCompleted:
                        {
                            Eventlogger.eventSend(EventLogging.UI_EVENT_HORN_SCAN_COMPLETED);
                            /* Stop the abort timer */
                            scanAbortTimer.stop();

                            /* Refresh the history data */
                            var retVal = refreshHistory();

                            /* Scan Status - 49 ("1"). Horn Scan Success return status */
                            var scanStatus = 49 ;
                            hornScanPopup.hornPopupClose();
                            mainwindow.hornOpInProgress = false;

                            if(scanStatus != statusVal)
                            {
                                recipe.updateStatusText(qmltextHornScanFailed)
                            }
                            else
                            {
                                recipe.updateStatusText(qmltextHornScanSuccess)
                            }
                            if(0 == retVal)
                            {
                                plotGraph();
                            }
                        }
                    }

                }


                SecondaryButton {
                    id: abortButton
                    text: qmltextAbortScan
                    anchors.verticalCenter: startButton.verticalCenter
                    anchors.left: startButton.right
                    anchors.leftMargin: 2 * 10

                    onClicked: {

                        /* Simple logic will be implemented directly with the Business Logic Communication */
                        var abortStatus = 0 ;
                        var retVal = 0 ;
                        var hornOpStatus = historyDataObj.getHornOperationStatus();

                        if(false == hornOpStatus)
                        {
                            recipe.updateStatusText(qmltextAbortingReq)
                        }
                        else
                        {
                            abortStatus = historyDataObj.abortHornScan();
                            if(1 != abortStatus)
                            {
                                recipe.updateStatusText(qmltextScanCouldNotBeAborted)
                            }
                            else
                            {
                                recipe.updateStatusText(qmltextAbortSuccess)
                                Eventlogger.eventSend(EventLogging.UI_EVENT_HORN_SCAN_ABORTED);
                            }

                            /* Refrresh the history */
                            retVal = refreshHistory();

                            /* Stop the timer */
                            scanAbortTimer.stop();

                            /* Close any pop up opened */
                            hornScanPopup.hornPopupClose();

                            /* Close pop up option */
                            mainwindow.hornOpInProgress = false;
                            if(0 == retVal)
                            {
                                plotGraph();
                            }
                        }
                    }
                }

                Item {
                    id:customProgressBar
                    objectName:qmltextProgressBar
                    property real status:0.5
                    property int numRect:1
                    property int rectHeight:10
                    property int rectWidth:10
                    property int custSpacing:2

                    width:parent.width * 0.52
                    height: Math.min(25 , ((parent.height) - ( 2 * 10)) )

                    anchors.verticalCenter:abortButton.verticalCenter
                    anchors.left:abortButton.right
                    anchors.leftMargin: 40



                    Text {
                        id:progressBarTitle
                        height: parent.height

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left:parent.left

                        verticalAlignment: Text.AlignVCenter

                        font.pixelSize: Style.style1
                        font.family: Style.semibold.name
                        text: qmltextStatus;

                    }

                    HornScanProgressBar {
                        id: statusProgressBar

                        height : parent. height

                        anchors.top:parent.top
                        anchors.left: progressBarTitle.right
                        anchors.leftMargin: 5
                        anchors.right:parent.right

                        progressVal: 0.0

                        Connections {
                            target: historyDataObj

                            onChangeProgress:
                            {
                                //if(statusVal >= 100)
                                //    Eventlogger.eventSend(EventLogging.UI_EVENT_HORN_SCAN_COMPLETED);
                                statusProgressBar.progressVal = statusVal ;
                                scanAbortTimer.restart() ;
                            }
                        }
                    }
                }
            }

            Rectangle {
                id : progressBarRect2


                Layout.row : 2
                Layout.column : 1

                Layout.columnSpan : 2

                Layout.alignment: Qt.AlignTop

                implicitWidth : parent.width - ( leftWindow.width + 2 * layoutSpacing )

                implicitHeight : parent.height * 0.075

                visible:true

                SecondaryButton {
                    id: advancedButton
                    text: qmltextHornRecipe

                    anchors.verticalCenter: parent.verticalCenter

                    anchors.left : parent.left
                    anchors.leftMargin: 2 * 10


                    onClicked: {
                        var hornRecipeParamFlag ;

                        leftWindow.visible = false
                        rightWindow.visible = false
                        progressBarRect1.visible = false
                        progressBarRect2.visible = false
                        graphWindow.visible = false
                        tabWindowRect.visible = false
                        threewayCarouselDots = false

                        hornRecipeObj.getHornDefaultValues()
                        hornRecipeControlWindow.visible = true
                        hornRecipeParamWindow.visible = true
                        titlebarchild.visible=true
                        titlebarchildtext.text=qmltextHornRecipeTitleBar
                        titlbaronclicked="HornPage.qml"
                    }
                }
            }

            Rectangle {
                id : graphWindow

                Layout.row : 3
                Layout.column : 1

                implicitHeight: parent.height - (tabWindowRect.height + progressBarRect1.height + progressBarRect2.height)
                implicitWidth : parent.width - (leftWindow.width + rightWindow.width + 2 * layoutSpacing)

                color:"#ffffff"

                visible: true


                ChartView {
                    id:scanGraphChartItem

                    property int  graphSpacing:10;
                    property real deltaX;
                    property real deltaYPhase;
                    property real deltaYCurrent;
                    property real deltaYImpedance;
                    property real deltaYAmplitude;
                    property real plotStartPointX;
                    property real plotEndPointX;
                    property real plotStartPointY;
                    property real plotEndPointY;

                    property int samplesPlotted:0;
                    property int totalSamples:0;

                    property real stopFreq: -1
                    property real startFreq: -1
                    property real centerFreq: -1

                    property bool hornPlotInProgress: false
                    property bool hornPlotRequested: false

                    property int graphNavigatorPosition:0
                    property real myWidth : 0.5

                    objectName: "graphPlot"
                    animationOptions: ChartView.SeriesAnimations

                    signal getHornSignature(int idVal);

                    x:0
                    y:graphNavigatorImage.height + graphNavigatorLabel.height

                    height: parent.height - (y)
                    width:parent.width


                    plotStartPointX: plotArea.x
                    plotEndPointX: plotArea.x + plotArea.width
                    plotStartPointY: plotArea.y
                    plotEndPointY: plotArea.y + plotArea.height
                    deltaX: plotArea.width / (freqAxis.max - freqAxis.min)
                    deltaYAmplitude: plotArea.height / (ampAxis.max - ampAxis.min)
                    deltaYCurrent: plotArea.height / (currentAxis.max - currentAxis.min)
                    deltaYPhase: plotArea.height / (phaseAxis.max - phaseAxis.min)
                    deltaYImpedance: plotArea.height / (impedanceAxis.max - impedanceAxis.min)

                    localizeNumbers: true

                    Component.onCompleted:
                    {
                        graphNavigatorPosition = freqAxis.min ;
                    }

                    onStartFreqChanged:
                    {
                        freqAxis.min = scanGraphChartItem.startFreq ;
                    }

                    onStopFreqChanged:
                    {
                        freqAxis.max = scanGraphChartItem.stopFreq ;
                    }

                    ValueAxis {
                        id:freqAxis
                        color: "#000000"
                        gridVisible: false
                        labelsVisible: false
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        lineVisible: true
                        minorGridVisible: false
                        titleText: qmltextFrequency + qmltextUnitHz
                        max: 0
                        min: 0
                        tickCount: 11
                    }

                    ValueAxis {
                        id:ampAxis
                        color: "#f03e3e"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#f03e3e"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Amplitude(%)"
                        labelFormat: "%.0f"
                        titleVisible: false
                        max: 125
                        min:0
                        tickCount: 11
                    }
                    ValueAxis {
                        id:ampAxis1
                        color: "#f03e3e"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#f03e3e"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Amplitude(%)"
                        labelFormat: "%.0f"
                        titleVisible: false
                        max: 125
                        min:0
                        tickCount: 11
                    }

                    ValueAxis {
                        id:currentAxis
                        color: "#009587"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#009587"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Current(%)"
                        titleVisible: false
                        max: 125
                        min:0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }
                    ValueAxis {
                        id:currentAxis1
                        color: "#009587"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#009587"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Current(%)"
                        titleVisible: false
                        max: 125
                        min:0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }

                    ValueAxis {
                        id:phaseAxis
                        color: "black"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "black"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Phase(Deg)"
                        titleVisible: false
                        max: 180
                        min: -180
                        tickCount: 11
                        labelFormat: "%.0f"
                    }
                    ValueAxis {
                        id:phaseAxis1
                        color: "black"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "black"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Phase(Deg)"
                        titleVisible: false
                        max: 180
                        min: -180
                        tickCount: 11
                        labelFormat: "%.0f"
                    }

                    ValueAxis {
                        id : impedanceAxis
                        color: "#a82ba8"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#a82ba8"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Impedance(Ohms)"
                        titleVisible: false
                        max: 50
                        min: 0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }
                    ValueAxis {
                        id : impedanceAxis1
                        color: "#a82ba8"
                        gridVisible: false
                        labelsVisible: true
                        labelsFont.family: Style.regular.name
                        labelsFont.pixelSize: Style.style1
                        labelsColor: "#a82ba8"
                        lineVisible: true
                        minorGridVisible: false
                        titleText: "Impedance(Ohms)"
                        titleVisible: false
                        max: 50
                        min: 0
                        tickCount: 11
                        labelFormat: "%.0f"
                    }

                    SplineSeries
                    {
                        id: ampPlot
                        width: scanGraphChartItem.myWidth
                        name: "ampPlot"
                        color: "#f03e3e"
                        axisX: freqAxis
                        axisY: ampAxis
                        visible: true
                        useOpenGL: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: ampPlot1
                        width: scanGraphChartItem.myWidth
                        name: "ampPlot"
                        color: "#f03e3e"
                        axisX: freqAxis
                        axisYRight: ampAxis1
                        visible: false
                        useOpenGL: true
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: currPlot
                        width: scanGraphChartItem.myWidth
                        name: "currPlot"
                        color: "#009587"
                        axisX: freqAxis
                        axisY: currentAxis
                        visible: true
                        useOpenGL: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: currPlot1
                        width: scanGraphChartItem.myWidth
                        name: "currPlot"
                        color: "#009587"
                        axisX: freqAxis
                        axisYRight: currentAxis1
                        visible: false
                        useOpenGL: true
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: phasePlot
                        width: scanGraphChartItem.myWidth
                        name: "phasePlot"
                        color: "#000001"
                        axisX: freqAxis
                        axisY: phaseAxis
                        visible: true
                        useOpenGL: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: phasePlot1
                        width: scanGraphChartItem.myWidth
                        name: "phasePlot"
                        color: "#000001"
                        axisX: freqAxis
                        axisYRight: phaseAxis1
                        visible: false
                        useOpenGL: true
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: impedancePlot
                        width: scanGraphChartItem.myWidth
                        name: "impedancePlot"
                        color: "#a82ba8"
                        axisX: freqAxis
                        axisY: impedanceAxis
                        visible: true
                        useOpenGL: true
                        pointsVisible: true
                    }
                    SplineSeries
                    {
                        id: impedancePlot1
                        width: scanGraphChartItem.myWidth
                        name: "impedancePlot"
                        color: "#a82ba8"
                        axisX: freqAxis
                        axisYRight: impedanceAxis1
                        visible: false
                        useOpenGL: true
                        pointsVisible: true
                    }

                    SplineSeries
                    {
                        id: phase0Plot
                        name: "phaseAxis"
                        axisX: freqAxis
                        axisYRight: phaseAxis
                        XYPoint {x: freqAxis.max; y:0}
                        XYPoint {x: freqAxis.min; y:0}
                        color: "#000000"
                        style: Qt.DashLine
                        useOpenGL: true
                        visible: false
                    }

                    Repeater {
                        id:graphPoints
                        model:resonantPointModel
                        Component
                        {
                            id:graphPointsDelegate
                            Item {
                                id: graphPointItem
                                y: scanGraphChartItem.plotStartPointY - graphPointLabel.height
                                x: scanGraphChartItem.plotStartPointX + ( (scanGraphChartItem.deltaX * (model.modelData.xAxisPos - freqAxis.min)) - graphPointLine.width / 2 )
                                width : graphPointLine.width
                                height : graphPointLine.height + graphPointLabel.height
                                visible: ((model.modelData.xAxisPos !=0) && (model.modelData.xAxisPos >= freqAxis.min) && (model.modelData.xAxisPos <= freqAxis.max))
                                Component.onCompleted:
                                {
                                    if((index == 0) && (model.modelData.xAxisPos >0))
                                        hornresonantFreqValue.freq = model.modelData.xAxisPos
                                }
                                Rectangle
                                {
                                    id: graphPointLine
                                    anchors.bottom: parent.bottom
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    border.width: 1
                                    height: scanGraphChartItem.plotArea.height
                                    width:1
                                    color: model.modelData.custColor
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
                                    color: model.modelData.custColor
                                }
                            }
                        }
                    }

                    Rectangle{
                        id: chartViewArea

                        x: scanGraphChartItem.plotArea.x
                        y: scanGraphChartItem.plotArea.y - ( graphNavigatorImage.height +  graphNavigatorLabel.height + graphSlider.raiseHeight)

                        height : graphNavigator.height + graphNavigatorImage.height + graphNavigatorLabel.height
                        width : scanGraphChartItem.plotArea.width

                        visible: false

                    }
                    Item {
                        id: graphSlider

                        property int raiseHeight: 30
                        property string custColor : qmlPrimaryColor
                        property int assetSize:32

                        property real xAxisVal: freqAxis.min

                        property string freqLabel:freqAxis.min

                        y: scanGraphChartItem.plotStartPointY - ( graphNavigatorImage.height +  graphNavigatorLabel.height + raiseHeight)

                        x: scanGraphChartItem.plotStartPointX + (xAxisVal - freqAxis.min) * scanGraphChartItem.deltaX - width / 2

                        width : graphNavigatorLabel.width

                        height : graphNavigator.height + graphNavigatorImage.height + graphNavigatorLabel.height

                        Rectangle
                        {
                            id: graphNavigator
                            //border.width: 1
                            height: scanGraphChartItem.plotArea.height + parent.raiseHeight
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
                                property int indexClosest2:0

                                anchors.fill: parent

                                cursorShape: Qt.ToolTip

                                hoverEnabled: false

                                onPositionChanged: {

                                    /* Drag position of the slider */
                                    var point = mapToItem(chartViewArea , mouse.x , mouse.y);
                                    var retVal ;
                                    var freqPoint;
                                    var phasePoint = 0;
                                    var ampPoint = 0 ;
                                    var currentPoint = 0 ;


                                    var impedancePoint = 0;

                                    if(point.x < 0 || point.x > scanGraphChartItem.plotArea.width)
                                    {
                                        if(point.x < 0)
                                        {
                                            posInChartArea.x = 0 ;

                                            freqPoint = freqAxis.min ;
                                        }
                                        else
                                        {
                                            posInChartArea.x = scanGraphChartItem.plotArea.width ;

                                            freqPoint = freqAxis.max ;
                                        }
                                    }
                                    else
                                    {

                                        posInChartArea.x = point.x;
                                        posInChartArea.y = point.y;

                                        freqPoint = posInChartArea.x / scanGraphChartItem.deltaX + freqAxis.min;
                                    }


                                    graphSlider.xAxisVal = (freqAxis.min + posInChartArea.x / scanGraphChartItem.deltaX );

                                    retVal = getClosestPoints(parseInt(freqPoint));

                                    if(0 == retVal)
                                    {

                                        var tmpPoint1 ;
                                        var tmpPoint2;

                                        tmpPoint1 = ampPlot.at(indexClosest1);
                                        tmpPoint2 = ampPlot.at(indexClosest2);

                                        ampPoint = tmpPoint1.y + ((parseInt(freqPoint) - tmpPoint1.x) * ((tmpPoint1.y - tmpPoint2.y) / (tmpPoint1.x - tmpPoint2.x)));

                                        tmpPoint1 = currPlot.at(indexClosest1);
                                        tmpPoint2 = currPlot.at(indexClosest2);

                                        currentPoint = tmpPoint1.y + ((parseInt(freqPoint) - tmpPoint1.x) * ((tmpPoint1.y - tmpPoint2.y) / (tmpPoint1.x - tmpPoint2.x)));


                                        tmpPoint1 = phasePlot.at(indexClosest1);
                                        tmpPoint2 = phasePlot.at(indexClosest2);

                                        phasePoint = tmpPoint1.y + ((parseInt(freqPoint) - tmpPoint1.x) * ((tmpPoint1.y - tmpPoint2.y) / (tmpPoint1.x - tmpPoint2.x)));


                                        tmpPoint1 = impedancePlot.at(indexClosest1);
                                        tmpPoint2 = impedancePlot.at(indexClosest2);

                                        impedancePoint = tmpPoint1.y + ((parseInt(freqPoint) - tmpPoint1.x) * ((tmpPoint1.y - tmpPoint2.y) / (tmpPoint1.x - tmpPoint2.x)));
                                    }
                                    else
                                    {
                                        ampPoint = 0;
                                        currentPoint = 0 ;
                                        phasePoint = 0 ;
                                        impedancePoint = 0 ;
                                        freqPoint = 0 ;
                                    }
                                    graphSlider.freqLabel = freqPoint.toFixed(0);
                                    resonantFreqValue.freqPoint = freqPoint.toFixed(0);
                                    hornRecipeObj.updateParameterVal(ampPoint.toFixed(2),currentPoint.toFixed(2),phasePoint.toFixed(2),impedancePoint.toFixed(2))
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
                            text: graphSlider.freqLabel +qmltextUnitHz
                            color: parent.custColor
                        }
                    }
                    Text {
                        id: startFreqLabel
                        y:parent.plotEndPointY + 5
                        x:(parent.plotStartPointX + parent.deltaX * (scanGraphChartItem.startFreq - freqAxis.min)) - 20
                        horizontalAlignment: Text.AlignHCenter
                        font.family : Style.regular.name
                        font.pixelSize: Style.style1
                        text:scanGraphChartItem.startFreq + qmltextUnitHz
                        color: "#000000"
                        visible: (scanGraphChartItem.startFreq < 0 ) ? false : true
                    }
                    Text {
                        id: stopFreqLabel
                        y:parent.plotEndPointY + 5
                        x:(parent.plotStartPointX + parent.deltaX * (scanGraphChartItem.stopFreq - freqAxis.min)) - 20
                        horizontalAlignment: Text.AlignHCenter
                        font.family : Style.regular.name
                        font.pixelSize: Style.style1
                        text: scanGraphChartItem.stopFreq + qmltextUnitHz
                        color: "#000000"
                        visible: (scanGraphChartItem.stopFreq < 0 ) ? false : true
                    }
                    Text {
                        id: centerFreqLabel
                        y:parent.plotEndPointY + 5
                        x:(parent.plotStartPointX + parent.deltaX * (scanGraphChartItem.centerFreq - freqAxis.min)) - 20
                        horizontalAlignment: Text.AlignHCenter
                        font.family : Style.regular.name
                        font.pixelSize: Style.style1
                        text: scanGraphChartItem.centerFreq + qmltextUnitHz
                        color: "#000000"
                        visible: (scanGraphChartItem.centerFreq < 0 ) ? false : true
                    }
                    legend.visible: false
                }
            }
            Rectangle {
                id : rightWindow
                Layout.row : 2
                Layout.column : 2
                implicitHeight : parent.height - (tabWindowRect.height + progressBarRect1.height + layoutSpacing)
                Layout.rowSpan: 2
                Layout.minimumWidth: 340 * fontSizeScaleFactor
                Layout.topMargin: layoutSpacing
                implicitWidth : parent.width * 0.2
                color : "#e9ecef"
                clip: true
                anchors.right:parent.right
                visible: true
                Rectangle {
                    id:rightWindowColumnRect
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
                        id: rightWindowColumn
                        y: -(rightWindowVbar.position * ( rightWindowColumn.height  + 52))
                        spacing: (window.height > 900 ? 2 : (window.height == 900 ? 1.5 : 1)) * layoutSpacing
                        anchors.left: parent.left
                        anchors.right: parent.right
                        Layout.alignment: Qt.AlignTop
                        Layout.minimumWidth: 260 * scaleRatio
                        Rectangle {
                            id: titleRectanngle
                            width: parent.width
                            height: Math.max( 40 * scaleRatio + layoutSpacing , Math.min(50 * scaleRatio , parent.height * 0.15))
                            color : "#e9ecef"
                            TextInput {
                                id: titleRectText
                                x: 0
                                y: 0
                                height: parent.height
                                width: parent.width * 0.6
                                color: "#868e96"
                                font.family : Style.semibold.name
                                wrapMode: TextEdit.Wrap
                                font.pixelSize: Style.style6
                                maximumLength: 15
                                text: qmltextHORNSCAN
                                verticalAlignment: Text.AlignVCenter
                                enabled: mainwindow.hornOpInProgress?false:true
                                validator: RegExpValidator
                                {
                                    regExp: /[A-Za-z0-9]+/
                                }

                                Keys.onReturnPressed:
                                {
                                    /* Send update request to DB */
                                    var updatedString = text;
                                    var updateResponse = -1;
                                    if("" == updatedString)
                                    {
                                        console.error("Please enter a valid string");
                                    }
                                    else
                                    {
                                        text = getRightTitleString(updatedString);
                                        if(historyListCtrlView.count > 0)
                                            updateResponse = historyDataObj.updateHistoryDataId( historyListCtrlView.indexSelected , updatedString);
                                        else
                                            recipe.updateStatusText(qmltextNoHistory)
                                    }
                                    focus = false ;
                                    if(1 != updateResponse)
                                    {
                                        if(historyListCtrlView.model[historyListCtrlView.indexSelected].scanName  == "")
                                            updatedString = historyListCtrlView.model[historyListCtrlView.indexSelected].scanId ;
                                        else
                                            updatedString = historyListCtrlView.model[historyListCtrlView.indexSelected].scanName ;
                                        text = getRightTitleString(updatedString);

                                        /* Implement Popups for horn scan updation failure */
                                        switch(updateResponse)
                                        {
                                        case -1:
                                            recipe.updateStatusText(qmltextFailToSaveHornScanId)
                                            break;
                                        case -4:
                                            recipe.updateStatusText(qmltextScanIdAlreadyPresent)
                                            break ;
                                        default:
                                            recipe.updateStatusText(qmltextFailToSaveHornId)
                                            break;
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                id: hornScanImageRect
                                height: width
                                width : Math.min(parent.width * 0.4 , 40 * scaleRatio)
                                anchors.right : parent.right
                                color: qmlPrimaryColor
                                visible: false
                                Image {
                                    id: hornImage
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
                            id: autoGraphRefreshRect
                            width: parent.width
                            height : Math.min(40 , 0.1 * parent.height)
                            color: "#e9ecef"
                            Text {
                                id:hornresonantFreqValue
                                property int freq:0;
                                text:qmltextResonantFrequency + " : " + freq
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                width : parent.width
                                horizontalAlignment: Text.AlignLeft
                                font.family: Style.semibold.name
                                font.pixelSize: Style.style4
                                color: "#868e96"
                                elide: Text.ElideRight
                            }
                        }
                        Rectangle {
                            id: resonantFreqRect
                            height: Math.max(25 , Math.min ( 45 * scaleRatio , parent.height * 0.1))
                            width:parent.width
                            color: "#898e96"
                            Text {
                                id:resonantFreqTitle
                                height: parent.height
                                width: parent.width * 0.4
                                color: "#ffffff"
                                text: qmltextFrequency
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                verticalAlignment: Text.AlignVCenter
                                font.family: Style.semibold.name
                                font.pixelSize: Style.style1
                                elide: Text.ElideRight
                            }
                            Text {
                                id:resonantFreqValue
                                property int freqPoint:0;
                                height: parent.height
                                width: parent.width * 0.4
                                color: "#ffffff"
                                text: freqPoint + qmltextUnitHz
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
                            id:graphParamView
                            property int paramIndex:1
                            width:parent.width
                            height: parent.height * 0.8
                            boundsBehavior: Flickable.StopAtBounds
                            flickableDirection: Flickable.AutoFlickDirection
                            clip:true
                            cellWidth: graphParamView.width
                            cellHeight: 50
                            model:rightParameterModel
                            delegate: GraphRightParamDelegate {
                                id: graphRightParamDelegate
                                width : graphParamView.cellWidth
                                height: graphParamView.cellHeight

                                comboBox.onCurrentIndexChanged:
                                {
                                    leftAndRightSwitch(index , comboBox.currentIndex , checkBox.checked);
                                }

                                checkBox.onCheckedStateChanged :
                                {
                                    changeGraphAxisVisiblity(index,checkBox.checked);

                                }


                                checkBox.onPressedChanged: {
                                    if(index === 3 && sysconfig.bransonKeyGlobalFlag)
                                    {
                                        if(checkBox.checked == true)
                                        {
                                            checkedImpeedanceStatus = true
                                        }
                                        else
                                        {
                                            checkedImpeedanceStatus = false
                                        }

                                        if(!sysconfig.bransonSecondLevelFlag)
                                        {
                                            checkBox.checked = false
                                            disableMainwindowOpacity.start()
                                            keyVerifyPopup.passwordLineEditText = qmlTextEmpty
                                            keyVerifyPopup.open()
                                            threewayCarouselDots = false
                                            keyVerifyPopup.onOkay.connect(function onOkay()
                                            {
                                                var resp;
                                                resp=sysconfig.compareBransonSecondLevelKey(keyVerifyPopup.passwordLineEditText);
                                                if(resp)
                                                {
                                                    //recipe.updateStatusText("Second Level Key is matched, unlocking the hidden screens")
                                                    sysconfig.bransonSecondLevelFlag = true
                                                    checkBox.checked = true
                                                    checkedImpeedanceStatus = true
                                                }
                                                else
                                                {
                                                    //recipe.updateStatusText("Entered wrong password, Retry again.")
                                                    checkBox.checked = false
                                                    checkedImpeedanceStatus = false

                                                }
                                                keyVerifyPopup.close()
                                                keyVerifyPopup.onOkay.disconnect(onOkay)
                                                threewayCarouselDots = true
                                            })
                                            keyVerifyPopup.onCancel.connect(function onCancel()
                                            {
                                                checkBox.checked = false
                                                checkedImpeedanceStatus = false
                                                keyVerifyPopup.onCancel.disconnect(onCancel)
                                                threewayCarouselDots = true
                                            })
                                        }
                                    }
                                }
                                Component.onCompleted:
                                {
                                    comboBox.currentIndex = model.modelData.isLeftAxis;
                                    checkBox.checked = model.modelData.isCheckBoxSelected;

                                    if(index === 3)
                                    {
                                        if(checkedImpeedanceStatus)
                                        {
                                            checkBox.checked = true
                                        }
                                        else
                                        {
                                            checkBox.checked = false
                                        }

                                        if(sysconfig.bransonKeyGlobalFlag)
                                        {
                                            outerRectVisibility = true
                                        }
                                        else
                                        {
                                            outerRectVisibility = false
                                        }
                                    }
                                }
                            }
                            Component.onCompleted:
                            {
                                hornRecipeObj.setHornDefaultValues()
                            }
                            Component.onDestruction:
                            {
                                hornRecipeObj.getHornDefaultValues()
                            }
                        }
                    }
                }
                ScrollBar {
                    id: rightWindowVbar
                    hoverEnabled: true
                    active:hovered || pressed
                    size: rightWindow.height / (rightWindowColumn.height + 52)
                    anchors.top : parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }

            Rectangle {
                id: hornRecipeParamWindow
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
                    id: grid
                    x: 50
                    y: 50
                    width: parent.width-50
                    height: parent.height-50
                    cellHeight: height/5
                    cellWidth: width/3
                    boundsBehavior: Flickable.StopAtBounds
                    flickableDirection: Flickable.AutoFlickDirection
                    clip: true
                    model: recipeParameterModel
                    property bool refreshSwitch: false
                    delegate: WeldModeItem
                    {
                        paramText:model.modelData.parameterName
                        paramTextLocale:model.modelData.parameterLocaleName
                        paramValue:model.modelData.parameterValue
                        paramUnit:model.modelData.parameterUnit
                        height:  labelSize
                        width:  grid.cellWidth-80
                        isSwitch: model.modelData.parameterSwitch
                        checkStatus:model.modelData.parameterSwitchState
                        toggleSwitch.onPressedChanged:
                        {
                            grid.refreshSwitch = true
                        }
                        toggleSwitch.onCheckedChanged:
                        {
                            if(grid.refreshSwitch == true)
                            {
                                grid.refreshSwitch = false
                                if(toggleSwitch.checked)
                                    hornRecipeObj.modifyParameter("1",paramText)
                                else
                                    hornRecipeObj.modifyParameter("0",paramText)
                            }
                        }
                        control_mode_mouseArea.onClicked:
                        {
                            if(model.modelData.parameterName != qmltextSetDigitalWithHornScan)
                            {
                                steppingNumPad.parameterText = model.modelData.parameterName
                                steppingNumPad.parameterLocaleText = model.modelData.parameterLocaleName
                                steppingNumPad.unitText = model.modelData.parameterUnit
                                steppingNumPad.keyInputText = model.modelData.parameterValue
                                steppingNumPad.checkParameter( model.modelData.parameterName )
                                steppingNumPad.minValue = model.modelData.parameterMinValue
                                steppingNumPad.maxValue = model.modelData.parameterMaxValue
                                numPadStatus = 1
                                steppingNumPad.open()
                                enableMainwindowOpacity.start();
                                steppingNumPad.dialoguerecipeIndex=index;
                                steppingNumPad.editrecipeflag= false
                                steppingNumPad.addrecipeflag= false
                                steppingNumPad.paramAtoZflag= false
                                steppingNumPad.ultrasonicAdvancedFlag=false
                                steppingNumPad.editHornRecipeFlag= true
                                steppingNumPad.afterBurst = false
                                steppingNumPad.preTrigger = false
                            }
                        }
                    }
                    ScrollBar.vertical: ScrollBar
                    {
                        parent: grid
                        anchors.top: grid.top
                        anchors.bottom: grid.bottom
                    }
                }
            }
            Rectangle {
                id: hornRecipeControlWindow
                Layout.row: 2
                Layout.column: 0
                Layout.columnSpan:3
                implicitHeight: parent.height * 0.15
                implicitWidth: parent.width
                Layout.alignment:Qt.AlignTop
                color:"#ffffff"
                visible: false
                PrimaryButton {
                    id: saveButton
                    text: qmltextSAVE
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 2 * layoutSpacing
                    onClicked: {
                        var retVal = 0 ;
                        retVal = hornRecipeObj.saveUserData();
                        if(0 != retVal)
                        {
                            recipe.updateStatusText(qmltextFailToSaveHornRecipeData)
                        }
                        else
                        {
                            recipe.updateStatusText(qmltextHornRecipeSaveSuccess)
                        }
                        statusProgressBar.progressVal = 0.0
                    }
                }

                SecondaryButton {
                    id: cancelButton
                    text:qmltextCancel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: saveButton.left
                    anchors.rightMargin: layoutSpacing
                    onClicked: {
                        hornRecipeObj.cancelUserData();
                        statusProgressBar.progressVal = 0.0
                    }
                }

                SecondaryButton {
                    id: resetButton
                    text: qmltextResetToDefaults
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * layoutSpacing
                    onClicked: {
                        hornRecipeObj.resetHornRecipeData();
                    }
                }
            }
        }
        Text {
            id: text_hornscan
            x:20
            color: "#868e96"
            text: qmltextHORNSCAN
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: Style.style6
            font.family: Style.semibold.name
        }
    }
    property var itemComponent;
    property var objInst;


    /**
    *@breif: Calculating the points in the graph
    **/
    function getClosestPoints(xval)
    {
        var length = ampPlot.count;
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

        point = ampPlot.at(0);
        startFreq = point.x ;
        point = ampPlot.at(1);
        startFreq1 = point.x ;

        point = ampPlot.at(length - 1)
        endFreq = point.x;

        if((xval < startFreq) || (xval > endFreq))
        {
            return -1;
        }

        point = ampPlot.at(2);
        freqStep = point.x - startFreq1 ;

        if(freqStep == 0)
            freqStep=1

        if((xval - startFreq) == 0)
            closestInd = 1
        else
            closestInd = (xval - startFreq) / freqStep;
        closestVal = 0;
        point = ampPlot.at(closestInd);

        {
            graphNavigatorMouseArea.indexClosest1 = closestInd;
            graphNavigatorMouseArea.indexClosest2 = closestInd+1;
        }

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
            titleText = "Horn Scan "+idString;
        }
        else
        {
            titleText = idString;
        }

        return titleText ;
    }

    /**
    *@breif: Function to plot the graph data of the horn scan UI from backend
    **/
    function plotGraph()
    {
        var retVal = 0 ;

        var startFreqValue = 0 ;

        var hornPsFreq = 0 ;
        var plotCenterFreq = 0 ;

        var axisMaxValues ;
        var axisMinValues ;

        /* Clear the graph Samples */
        clearGraphSamples();

        /* Append the points for each of the parameters */
        historyDataObj.retrieveHornSigPoints(scanGraphChartItem.series("ampPlot") , 1);
        historyDataObj.retrieveHornSigPoints(scanGraphChartItem.series("currPlot") , 2);
        historyDataObj.retrieveHornSigPoints(scanGraphChartItem.series("phasePlot") , 3);
        historyDataObj.retrieveHornSigPoints(scanGraphChartItem.series("impedancePlot") , 4);

        /* Updating the start and stop freq */
        var point = ampPlot.at(0);
        var length = ampPlot.count ;
        startFreqValue = point.x ;
        scanGraphChartItem.startFreq = startFreqValue;

        point = ampPlot.at(length - 1);
        scanGraphChartItem.stopFreq = point.x;

        //hornPsFreq = hornRecipeObj.getHornPsFreq();
        plotCenterFreq = determinePlotCenterFreq();
        scanGraphChartItem.centerFreq = plotCenterFreq ;
        /* Initialize the vaues */
        resonantFreqValue.freqPoint = startFreqValue ;

        graphParamView.model[0].parameterval = ampPlot.at(0).y ;
        graphParamView.model[1].parameterval = currPlot.at(0).y;
        graphParamView.model[2].parameterval = phasePlot.at(0).y;
        graphParamView.model[3].parameterval = impedancePlot.at(0).y

        /* Repositioning the graph Slider */
        graphSlider.xAxisVal = startFreqValue ;
        graphSlider.freqLabel = startFreqValue ;

        phase0Plot.append(scanGraphChartItem.startFreq , 0);
        phase0Plot.append(scanGraphChartItem.stopFreq , 0);

        /* Modifying the axis values */
        axisMaxValues = historyDataObj.hornAxisMaxValues();
        axisMinValues = historyDataObj.hornAxisMinValues();

        /* Calibrating the axis values */

        freqAxis.max = axisMaxValues[freqInd] ;
        freqAxis.min = axisMinValues[freqInd] ;

        roundAxisValues(ampInd , (axisMaxValues[ampInd] + 5) , (axisMinValues[ampInd] - 5) , 50000000 , 0);
        roundAxisValues(curInd , (axisMaxValues[curInd] + 5) , (axisMinValues[curInd] - 5) , 50000000 , 0);
        roundAxisValues(impedanceInd , (axisMaxValues[impedanceInd] + 5) , (axisMinValues[impedanceInd] - 5) , 50000000 , 0);
        roundAxisValues(phaseInd , (axisMaxValues[phaseInd] + 5) , (axisMinValues[phaseInd] - 5) , 180 , -180);
    }


    /**
    *@breif: Refresh the history data
    **/
    function refreshHistory()
    {
        var retVal = 0 ;
        var titleTextString = text_hornscan.text ;
        /* Reset the index Selected */
        historyListCtrlView.indexSelected = 0 ;

        historyDataObj.hornIndSelected = historyListCtrlView.indexSelected ;

        /* Refresh the history */
        retVal = historyDataObj.refreshHistory();

        if(0 == retVal)
        {

            titleTextString = historyListCtrlView.model[historyListCtrlView.indexSelected].scanId

        }
        /* Update the horn result titleBar on the right window */
        titleRectText.text = getRightTitleString(titleTextString);

        return retVal ;
    }

    /**
    *@breif: Clear the graph Samples
    **/
    function clearGraphSamples()
    {
        ampPlot.clear();
        ampPlot1.clear();
        phasePlot.clear();
        phasePlot1.clear();
        impedancePlot.clear();
        impedancePlot1.clear();
        currPlot.clear();
        currPlot1.clear();
        phase0Plot.clear();
    }

    /**
    *@breif: Forcefully generate an abort button clicked event to stop the horn scan operation
    **/
    function abortScanFromPopup()
    {
        /* Forcefully generate an abort button clicked event to stop the horn scan operation */
        abortButton.clicked();
    }

    /**
    *@breif: To determine graph center frequency
    **/
    function determinePlotCenterFreq()
    {

        var centerFreqVal = 0 ;
        /* Check if graph in range of 20khz horn */
        //        if(scanGraphChartItem.startFreq >= 19450 && scanGraphChartItem.stopFreq <= 20450)
        //        {
        //            centerFreqVal = 19950;
        //        }
        //        /* Check if graph in range of 30khz horn */
        //        else if(scanGraphChartItem.startFreq >= 29250 && scanGraphChartItem.stopFreq <= 30750)
        //        {
        //            centerFreqVal = 30000;
        //        }
        //        /* Check if graph in range of 40khz horn */
        //        else if(scanGraphChartItem.startFreq >= 38900 && scanGraphChartItem.stopFreq <= 40900)
        //        {
        //            centerFreqVal = 39900;
        //        }
        //        else
        //{
        centerFreqVal = scanGraphChartItem.startFreq + ((scanGraphChartItem.stopFreq - (scanGraphChartItem.startFreq))/2) ;
        //}
        return centerFreqVal ;
    }


    /**
    *@breif: Change axis to left or right based on selected axis and checkboxstatus
    **/
    function graphPropertyVisible(rightParamIndex , dropDownOption , checkState)
    {
        if(true == checkState)
        {
            /* Left Axis Parameters to be modified */
            if(0 == dropDownOption)
            {
                switch(rightParamIndex)
                {
                case 1:
                    ampPlot.visible = true ;
                    ampAxis.visible = true ;

                    ampPlot1.visible = false ;
                    ampAxis1.visible = false ;
                    break ;

                case 2:
                    currPlot.visible = true ;
                    currentAxis.visible = true ;

                    currPlot1.visible = false ;
                    currentAxis1.visible = false ;
                    break ;

                case 3:
                    phasePlot.visible = true ;
                    phase0Plot.visible = true;
                    phaseAxis.visible = true ;

                    phasePlot1.visible = false ;
                    phaseAxis1.visible = false ;
                    break ;

                case 4:
                    impedancePlot.visible = true ;
                    impedanceAxis.visible = true ;

                    impedancePlot1.visible = false ;
                    impedanceAxis1.visible = false ;
                    break ;

                default:
                    break ;
                }
            }
            else
            {
                switch(rightParamIndex)
                {
                case 1:
                    ampPlot.visible = false ;
                    ampAxis.visible = false ;

                    ampPlot1.visible = true ;
                    ampAxis1.visible = true ;
                    break ;

                case 2:
                    currPlot.visible = false ;
                    currentAxis.visible = false ;

                    currPlot1.visible = true ;
                    currentAxis1.visible = true ;
                    break ;

                case 3:
                    phasePlot.visible = false ;
                    phase0Plot.visible = false;
                    phaseAxis.visible = false ;

                    phasePlot1.visible = true ;
                    phaseAxis1.visible = true ;
                    break ;

                case 4:

                    impedancePlot.visible = false ;
                    impedanceAxis.visible = false ;

                    impedancePlot1.visible = true ;
                    impedanceAxis1.visible = true ;
                    break ;

                default:
                    break ;
                }
            }
        }
        else
        {
            switch(rightParamIndex)
            {
            case 1:
                ampPlot.visible = false ;
                ampAxis.visible = false ;

                ampPlot1.visible = false ;
                ampAxis1.visible = false ;
                break ;

            case 2:
                currPlot.visible = false ;
                currentAxis.visible = false ;

                currPlot1.visible = false ;
                currentAxis1.visible = false ;
                break ;

            case 3:
                phasePlot.visible = false ;
                phase0Plot.visible = false;
                phaseAxis.visible = false ;

                phasePlot1.visible = false ;
                phaseAxis1.visible = false ;
                break ;

            case 4:

                impedancePlot.visible = false ;
                impedanceAxis.visible = false ;

                impedancePlot1.visible = false ;
                impedanceAxis1.visible = false ;
                break ;

            default:
                break ;
            }
        }
    }

    /**
    *@breif: Rounding of axis values for proper representation
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
            ampAxis.max = ampAxis1.max = 120
            ampAxis.min  = ampAxis1.min = 0
            break ;

        case curInd:
            currentAxis.max = currentAxis1.max = 120
            currentAxis.min = currentAxis1.min = 0
            break ;

        case phaseInd:
            phaseAxis.max = phaseAxis1.max = 180
            phaseAxis.min = phaseAxis1.min = -180
            break;

        case impedanceInd:
            impedanceAxis.max = impedanceAxis1.max =  tmpAxisMaxVal
            impedanceAxis.min = impedanceAxis1.min = tmpAxisMinVal
            break ;

        case freqInd:
            freqAxis.max = tmpAxisMaxVal
            freqAxis.min = tmpAxisMinVal
            break;
        }
    }

    /**
    *@breif: To switch y-axis to left or right
    **/
    function leftAndRightSwitch(index,currentIndex,isCheckBoxSelected)
    {
        if(index < 0)
            index = 0

        hornRecipeObj.setAxisIndex(index,currentIndex);
        if(index == 0)//Amplitude
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    ampAxis.visible = true
                    ampAxis1.visible = false
                }
                else
                {
                    ampAxis.visible = false
                    ampAxis1.visible = true
                }
            }
            else
            {
                ampAxis.visible = false
                ampAxis1.visible = false
                ampPlot.visible = false
            }
        }
        else if(index == 1)//Current
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    currentAxis.visible = true
                    currentAxis1.visible = false
                }
                else
                {
                    currentAxis.visible = false
                    currentAxis1.visible = true
                }
            }
            else
            {
                currentAxis.visible = false
                currentAxis1.visible = false
                currPlot.visible = false
            }
        }
        else if(index == 2)//Phase
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    phaseAxis.visible = true
                    phaseAxis1.visible = false
                }
                else
                {
                    phaseAxis.visible = false
                    phaseAxis1.visible = true
                }
            }
            else
            {
                phaseAxis.visible = false
                phaseAxis1.visible = false
                phasePlot.visible = false
                phase0Plot.visible = false
            }
        }
        else if(index == 3)//impedance
        {
            if(isCheckBoxSelected)
            {
                if(currentIndex == 0)
                {
                    impedanceAxis.visible = true
                    impedanceAxis1.visible = false
                }
                else
                {
                    impedanceAxis.visible = false
                    impedanceAxis1.visible = true
                }
            }
            else
            {
                impedanceAxis.visible = false
                impedanceAxis1.visible = false
                impedancePlot.visible = false
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

        var flag = hornRecipeObj.getComboIndex(dwIndex)
        switch(dwIndex)
        {
        case ampInd:
            ampPlot.visible = ischecked
            ampAxis.visible = ischecked && (!flag)
            ampAxis1.visible = ischecked && (flag)
            break;
        case curInd:
            currPlot.visible =     ischecked
            currentAxis.visible =  ischecked && (!flag)
            currentAxis1.visible = ischecked && (flag)
            break;
        case phaseInd:
            phasePlot.visible  = ischecked
            phase0Plot.visible = ischecked
            phaseAxis.visible  = ischecked && (!flag)
            phaseAxis1.visible = ischecked && (flag)
            break;
        case impedanceInd:
            impedancePlot.visible = ischecked
            impedanceAxis.visible = ischecked && (!flag)
            impedanceAxis1.visible= ischecked && (flag)
            break;
        default:
            break;
        }
        hornRecipeObj.setCheckboxStatus(dwIndex,ischecked)
    }
    BransonKeyVerifyPopup
    {
        id:keyVerifyPopup
    }

}
