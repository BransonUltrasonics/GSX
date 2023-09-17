import QtQuick 2.0
import QtQuick.Layouts 1.3
import Style 1.0
import com.branson.datawizard 1.0
import com.branson.wizardsteps 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4

Item {
    readonly property int  reportgenDelay : 1000
    readonly property string qmlCollectdatatitle : qsTr("Collect Data By")
    readonly property string qmlViewReportTitle : qsTr("View report")
    readonly property string qmlTextSpace : " "
    readonly property string qmlTextSaveReport : qsTr("Save Report")
    readonly property string qmlTextPeriod :qsTr("Period :")
    readonly property string qmlTextFrequency : qsTr( "Frequency :")
    readonly property string qmlTextColon : " :"
    readonly property string qmlTextReportCollectionTime : qsTr("Report Collection: Time")
    readonly property string qmlTextCycles : qsTr("CYCLES")
    readonly property string qmlTextWeldData : qsTr("Weld Data")
    readonly property string qmlTextAlarmData : qsTr("Alarm Data")
    readonly property string qmlTextSystemSettings : qsTr("System Settings")
    readonly property string qmlTextEventData : qsTr("Event Data")
    readonly property string qmlTextUserData: qsTr("User Data")
    readonly property string qmlTextRecipeData: qsTr("Recipe Data")
    readonly property string qmlTextExport : qsTr("Export")
    readonly property string qmlTextDelete :qsTr("Delete")
    readonly property string qmlTextFreq : qsTr("Frequency")
    readonly property string qmlTextGenerateReport: qsTr("Generate Report")
    readonly property string qmltextDaily: qsTr("Daily")
    readonly property string qmltextCycleVal: qsTr("Cycles")
    readonly property string qmltextDeleteDBRecords: qsTr("Delete DB Records")
    readonly property string qmlStringReportType: qsTr("Report Type")
    readonly property string qmlStringDataFilter: qsTr("Data filter")
    readonly property string qmlStringStorageType: qsTr("Storage Type")
    readonly property string qmlStringOutput: qsTr("Output")
    readonly property string qmlStringFrequencyType: qsTr("Frequency Type")
    readonly property string qmlStringFrequencyPeriodType: qsTr("Frequency Period Type")
    readonly property string qmlStringTimeIs: qsTr("Time is")
    readonly property string qmlStringFrequencyCycles: qsTr("Frequency Cycles")
    readonly property string qmlTextReportRunning :qsTr("Report is running. Please wait...")
    readonly property string qmlTextVacuumRunning :qsTr("Optimization is running. This may take up to a minute. Do not turn system off.")
    readonly property string qmlTextReportWait :qsTr("Please wait...")
    readonly property string qmlTextWeldResult: qsTr("Weld Results")
    readonly property string qmlTextWeldGraphDataText: qsTr("Weld Graph Data")


    property bool bContinousReportItems : false //please enabled for continous report
    property bool reportgenerationinprogress: false
    property int qmlstartRange:1
    property int qmlEndRange:10000
    property bool brectvisible: true
    property bool brectvisible_sr: false
    property bool bOutputrectvisible:true
    property string upimage: "qrc:/images/image/baseline_arrow_right_black_36dp.png"
    property string downimage: "qrc:/images/image/baseline_arrow_drop_down_black_36dp.png"
    property string currentstateimg
    property string currentoutputimg
    readonly property string  qmlNextText:qsTr("Next")
    readonly property string qmlBackText: qsTr("Back")
    property color disabledcolor : "gray"
    property color enabledcolor  : qmlPrimaryColor
    property var itemspacing: 50
    property var labeltopmargin:5
    property variant items: [qsTr("Type"), qsTr("Output"), qsTr("Storage Options"), qsTr("Data Filters"), qsTr("Summary")]
    property variant singleReportItems: [qsTr("Results"), qsTr("Recipes"), qsTr("Graph")]

    property string subPeriodName: ""
    property double borderwidth: 1.1
    property string bordercolor : "#000000"
    property string circlecolor : "white"
    property string activecirclecolor : qmlPrimaryColor
    property int currentFrequencyIndex : 0
    property int currentWizardStep: DataWizardStep.DATAWIZARD_STEP_TYPE

    property int nFrequencyType : 0
    property int nFrequencyType_scheduling : 0
    property string strsummary :""
    property int nCurrentDataFilter: 0
    property int nCurrentSubDataFilter: 0
    property int nCurrentSubSubDataFilter: 0
    property int nCurrentDeleteDBCheckBoxStatus: 0
    property int nCurrentStorageIndex: 0
    property int nCurrentOutputIndex: 0
    property int nCurrentSubOutputIndex: 0
    property int rect_summary_visibility: 0
    property int id_frequency_time_weekly_visibilty: 0
    property int id_scheduling_time_weekly_visibilty: 0
    property int rect_scheduling: 0
    property int filterCount : 0
    property int nCurrentSubDataFilterIndex: 0
    property CustomNumPad cNumpad
    property int radioButtonWidth: 35
    property int radioButtonHeigth: 35

    //text field property
    property string textFieldColor: qmlPrimaryColor
    property color grayRectColor: "#f6f6f6"

    property int primaryButtonText: 170
    //custom box in frequency
    property int customBoxWidth: 160
    property int customBoxPopupHeight: 230

    //related to datafilter
    property int  dataFilterHeightWhenMin : 15
    property int  dataFilterHeightWhenMax : 120

    //related to store summary in db
    property string summary : ""

    //Continuous Report storage
    property string outputFormatString: ""
    property string freqPeriod: ""
    property string freqTime :""
    property string freqMonthWeekDaily: ""
    property string outputSchedulefreq: ""
    property string outputScheduleMonthlyWeeklyDaily: ""

    //sigle data filter check box Status
    property bool weldResultCheckStatus: false
    property bool weldGraphCheckStatus: false
    property bool weldResultDBCheckStatus: false
    property bool weldGraphDBCheckStatus: false
    property string reportTypeInfo: ""
    property string reportStorageInfo: ""
    property int weldResultAndGraph: 0


    //Message box to
    property MessagePopup mPopup
    property string qmlDeleteDBInfoText: ""


    QtObject {
        id: theme
        property font textFont: Qt.font({
                                            family: Style.regular.name,
                                            //            weight: Font.Black,
                                            pixelSize:  Style.style3,
                                            verticalAlignment: Text.AlignVCenter,
                                            elide: Text.ElideMiddle ,
                                            wrapMode: Text.WordWrap
                                        })
    }

    /**
    *@breif: Output details visibility based on whether it is single or continuous report
    **/
    function getOutputVisibleDetails(index)
    {
        var bVisible
        if(DataReportType.DATAWIZARD_REPORT_SINGLE == datawizardobject.getSelectedReportType() )
        {
            bVisible = (index == 0)
            if(index == 1)
                bVisible = false
        }
        else if(DataReportType.DATAWIZARD_REPORT_CONTINOUS == datawizardobject.getSelectedReportType() ){
            bVisible = true
        }
        return bVisible
    }

    /**
    *@breif: To show summary for the selected items
    **/
    function selectedDetails(item, number)
    {

        if (datawizardobject.getSelectedFrequencyType() == DataReportType.DATAWIZARD_FREQUENCY_TIME)
            nFrequencyType = 0;
        if (datawizardobject.getSelectedFrequencyType() == DataReportType.DATAWIZARD_FREQUENCY_CYCLE)
            nFrequencyType = 1;

        strsummary = qmlStringReportType + "      : " + datawizardobject.getSelectedIndexString(datawizardobject.getSelectedReportType());
        reportTypeInfo = strsummary

        if(DataReportType.DATAWIZARD_REPORT_CONTINOUS == datawizardobject.getSelectedReportType())
        {
            strsummary+= qmlStringFrequencyType + " : " + " : "+datawizardobject.getSelectedIndexString(datawizardobject.getSelectedFrequencyType());
            strsummary+= "\n";

            var selfreqtype = datawizardobject.getSelectedFrequencyType();
            var frequencyType = datawizardobject.getSelectedFrequencyPeriodType();
            switch(selfreqtype)
            {
            case 2:
                strsummary+= qmlStringFrequencyPeriodType + " : " + datawizardobject.getSelectedFrequencyPeriodType() + " : " + datawizardobject.getSelectedIndexString(datawizardobject.getSelectedFrequencyPeriodType())
                strsummary+= "\n";
                strsummary+= qmlStringTimeIs + " : " + outputTimeSelect.timePicker.timeLabel.text;
                strsummary+= "\n";
                break;
            case 3:
                strsummary+= qmlStringFrequencyCycles + " : " + datawizardobject.getSelectedCycles();
                strsummary+= "\n";
                break;
            }
        }

        if(DataReportType.DATAWIZARD_REPORT_SINGLE == datawizardobject.getSelectedReportType())
        {
            strsummary+="\n" + qmlStringDataFilter + " : " + datafiltermodelsr[nCurrentDataFilter].strFilterName
            strsummary+=" - "+ datafiltermodelsr[nCurrentDataFilter].strFilterList[nCurrentSubDataFilter]
        }
        else
        {
            // strsummary+="Data Sub filter 4  : "+datafiltermodel[nCurrentDataFilter].strFilterList[nCurrentSubDataFilter]
        }


        reportStorageInfo =  "\n" + qmlStringStorageType + "    : " + storagemodellist[nCurrentStorageIndex]
                       +"\n" + qmlStringOutput + " : " + outputmodellist[nCurrentOutputIndex].strFilterName
                       +  " - " + outputmodellist[nCurrentOutputIndex].strFilterList[nCurrentSubOutputIndex];

        strsummary += reportStorageInfo

        summaryDeleteDBDetails()

    }


    /**
    *@breif: To get the summary infomation in Summary step when generate CSV report.
    **/
    function summaryTheDetailed()
    {
        //The weld result and weld graph data are all selected.
        if((weldResultCheckStatus === true) && (weldGraphCheckStatus === true))
        {
            var tempFilterType = ""
            if(nCurrentDataFilter === 0) /* add the weld graph data info*/
            {
                tempFilterType = "\n" + qmlStringDataFilter + " : " + datafiltermodelsr[1].strFilterName
                tempFilterType += " - "+ datafiltermodelsr[1].strFilterList[nCurrentSubDataFilter]
                return reportTypeInfo + tempFilterType + reportStorageInfo
            }
            else if(nCurrentDataFilter === 1) /* add the weld result info*/
            {
                tempFilterType = "\n" + qmlStringDataFilter + " : " + datafiltermodelsr[0].strFilterName
                tempFilterType += " - "+ datafiltermodelsr[0].strFilterList[nCurrentSubDataFilter]
                return reportTypeInfo + tempFilterType + reportStorageInfo
            }
        }
        return ""
    }


    /**
    *@breif: To get the delete DB records infomation after clicking the genereate report button.
    **/
    function summaryDeleteDBDetails()
    {
        if(outputFormatString === "CSV")
        {
            if((weldResultCheckStatus === true) && (weldGraphCheckStatus === true) && (nCurrentDeleteDBCheckBoxStatus === 1))
            {
                qmlDeleteDBInfoText = "Delete " + datafiltermodelsr[0].strFilterList[nCurrentSubDataFilter]
                        +" and " + datafiltermodelsr[1].strFilterList[nCurrentSubDataFilter] + " DB records"
            }
            else if(weldResultCheckStatus === true && nCurrentDeleteDBCheckBoxStatus === 1) /*weld result and its delete records are checked*/
            {
                qmlDeleteDBInfoText = "Delete " + datafiltermodelsr[0].strFilterList[nCurrentSubDataFilter] + " DB records"
            }
            else if(weldGraphCheckStatus === true && nCurrentDeleteDBCheckBoxStatus === 1) /*weld graph data and its delete records are checked*/
            {
                qmlDeleteDBInfoText = "Delete " + datafiltermodelsr[1].strFilterList[nCurrentSubDataFilter] + " DB records"
            }
            else
            {
                qmlDeleteDBInfoText = ""
            }
        }
        else   /*the report format is PDF */
        {
            if(nCurrentDeleteDBCheckBoxStatus === 1) /*there exists a delete DB records checked */
            {
                qmlDeleteDBInfoText = "Delete "
                qmlDeleteDBInfoText += datafiltermodelsr[nCurrentDataFilter].strFilterList[nCurrentSubDataFilter]
                qmlDeleteDBInfoText += " DB records"
            }
            else
            {
                qmlDeleteDBInfoText = ""
            }
        }

    }


    /**
    *@breif: To show the delete message box.
    **/
    function showDeleteDBInfoMessage()
    {
        var component = Qt.createComponent("MessagePopup.qml")
        mPopup = component.createObject(window)
        //the prompt box is popped up
        mPopup.open(mPopup.warning_title,qmlDeleteDBInfoText, mPopup.ok_and_cancel_button)
        //click the Done button
        mPopup.onOkay.connect(function okay()
        {
            mPopup.close()
            mPopup.onOkay.disconnect(okay)
            mPopup.destroy()
            id_reportgen_content.generateReports()
        })

        //click the Cancel button
        mPopup.onCancel.connect(function cancel()
        {
            mPopup.close()
            mPopup.onCancel.disconnect(cancel)
            mPopup.destroy()
        })
    }


    Rectangle
    { //Start of points in Wizard steps
        id:id_ReportPoints
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 30
        anchors.top : parent.top
        Text {
            id:testtext
            //topPadding: 10
            font.letterSpacing: -1
            text:qmlTextSpace
        }

        //Type Point in Wizard Step
        Rectangle
        {

            id: id_reportgen_type
            enabled : !g_reportgenerationinprogress
            anchors.leftMargin: itemspacing
            width: radioButtonWidth
            height: radioButtonHeigth
            color: (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_TYPE)?activecirclecolor:circlecolor
            border.width: borderwidth
            border.color: bordercolor
            radius: height/2

            MouseArea
            {  id:id_reportgen_type_pos
                anchors.fill:parent
                onClicked:
                {
                    currentWizardStep=DataWizardStep.DATAWIZARD_STEP_TYPE
                    selectedDetails(items[0],currentWizardStep)
                }
            }
            Text {
                anchors.top: parent.bottom
                anchors.topMargin: labeltopmargin
                anchors.horizontalCenter: parent.horizontalCenter
                text:items[0]
                font.bold : (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_TYPE)?true:false
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize:  Style.style2
            }
        }

        //Output Point in Wizard Step
        Rectangle
        {
            id: id_reportgen_output
            enabled : !g_reportgenerationinprogress
            anchors.leftMargin: itemspacing
            anchors.left: id_reportgen_type.right
            width: radioButtonWidth
            height: radioButtonHeigth
            color: (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_OUTPUT)?activecirclecolor:circlecolor//qmlPrimaryColor
            border.width: borderwidth
            border.color: bordercolor
            radius: height/2

            MouseArea
            {  id:id_reportgen_output_pos
                anchors.fill:parent
                onClicked:
                {
                    currentWizardStep=DataWizardStep.DATAWIZARD_STEP_OUTPUT
                    selectedDetails(items[1], currentWizardStep)
                    // id_report_Frequency_period_scheduling.currentIndex = 0
                    datawizardobject.outputRefresh()
                }
            }
            Text
            {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.bottom
                anchors.topMargin: labeltopmargin
                text:items[1]
                font.bold : (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_OUTPUT)?true:false
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize:  Style.style2
            }
        }

        //Storage Point in Wizard Step "Not using currentlyn so made visible false"
        Rectangle
        {
            id: id_reportgen_storage
            enabled : !g_reportgenerationinprogress
            anchors.leftMargin: itemspacing
            anchors.left: id_reportgen_output.right
            visible: true
            width: radioButtonWidth
            height: radioButtonHeigth
            color: (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_STORAGEOPTIONS)?activecirclecolor:circlecolor//qmlPrimaryColor
            border.width: borderwidth
            border.color: bordercolor
            radius: height/2
            MouseArea
            {  id:id_reportgen_storage_pos
                anchors.fill:parent
                onClicked:
                {
                    currentWizardStep=DataWizardStep.DATAWIZARD_STEP_STORAGEOPTIONS
                    selectedDetails(items[2],currentWizardStep)
                }
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.bottom
                anchors.topMargin: labeltopmargin
                width: 20
                text:items[2]
                font.bold : (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_STORAGEOPTIONS)?true:false
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize:  Style.style2
            }
        }

        //DataFilter Point in Wizard Step
        Rectangle
        {
            id: id_reportgen_datafilter
            enabled : !g_reportgenerationinprogress
            anchors.leftMargin: itemspacing
            anchors.left: id_reportgen_storage.right
            width: radioButtonWidth
            height: radioButtonHeigth
            color: (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_DATAFILTER)?activecirclecolor:circlecolor//qmlPrimaryColor
            border.width: borderwidth
            border.color: bordercolor
            radius: height/2

            MouseArea
            {  id:id_reportgen_datafilter_pos
                anchors.fill:parent
                onClicked:
                {
                    currentWizardStep= DataWizardStep.DATAWIZARD_STEP_DATAFILTER
                    selectedDetails(items[3],currentWizardStep)
                }
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.bottom
                width: 20
                anchors.topMargin: labeltopmargin
                text:items[3]
                font.bold : (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_DATAFILTER)?true:false
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize:  Style.style2
            }
        }

        //summary in wizard step
        Rectangle
        {
            id: id_reportgen_summary
            enabled : !g_reportgenerationinprogress
            anchors.leftMargin: itemspacing
            anchors.left: id_reportgen_datafilter.right
            width: radioButtonWidth
            height: radioButtonHeigth
            color: (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_SUMMARY)?activecirclecolor:circlecolor//qmlPrimaryColor
            border.width: borderwidth
            border.color: bordercolor
            radius: height/2

            MouseArea
            {  id:id_reportgen_summary_pos
                anchors.fill:parent
                onClicked:
                {
                    currentWizardStep=DataWizardStep.DATAWIZARD_STEP_SUMMARY
                    selectedDetails(items[4],currentWizardStep)
                    rect_summary_visibility = 0
                }
            }
            Text
            {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.bottom
                anchors.topMargin: labeltopmargin
                text:items[4]
                font.bold : (currentWizardStep==DataWizardStep.DATAWIZARD_STEP_SUMMARY)?true:false
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                font.family: Style.regular.name
                font.pixelSize:  Style.style2
            }
        }

        Component.onCompleted:
        {

            //if all points are loaded what are the default actions
            currentWizardStep = DataWizardStep.DATAWIZARD_STEP_TYPE
            selectedDetails(items[0],currentWizardStep)
            var widths=  (id_reportgen_type.width + id_reportgen_datafilter.width + id_reportgen_storage.width + id_reportgen_output.width + itemspacing+ itemspacing-10)
            var avgwidth = (testtext.paintedWidth / testtext.text.length);
            var totchars = (widths / avgwidth);
            testtext.anchors.verticalCenter = id_reportgen_type.verticalCenter
            for(var i=0;i<totchars;i++)
                testtext.text+="-";
        }
    } //End of Points of Wizard steps

    //waiting report
    LoaderPopup
    {
        id: id_report_loader
        onLoaderOpen: {
           g_reportgenerationinprogress = true;
           enableMainwindowOpacity.start();
        }

        onLoaderClose: {
           disableMainwindowOpacity.start();
        }

        onLoaderRunning: {
           if(!g_reportgenerationinprogress) id_report_loader.close();
        }
    }

    //waiting vacuum
    LoaderPopup
    {
        id: id_vacuum_loader
        onLoaderOpen: {
           g_vacummgenerationinprogress = true;
           enableMainwindowOpacity.start();
        }

        onLoaderClose: {
           disableMainwindowOpacity.start();
        }

        onLoaderRunning: {
           if(!g_vacummgenerationinprogress) id_vacuum_loader.close();
        }
    }


    Rectangle
    {
        id:id_reportgen_content
        width:parent.width - 20
        height: (parent.height * 65 ) /100
        anchors.left: parent.left
        anchors.top: id_ReportPoints.bottom
        anchors.topMargin: 80
        anchors.leftMargin: (parent.width ) / 4
        StackLayout
        {
            id: layout
            anchors.fill: parent
            currentIndex: (currentWizardStep - DataWizardStep.DATAWIZARD_STEP_TYPE) // stack layout index is zero based , so subtracing by 1
            //content portion of type step
            Rectangle
            {
                id:reptype
                anchors.left: parent.left
                CustomGroupBox
                {
                    id:id_report_type_content

                    Rectangle
                    {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 50
                        PrimaryButton
                        {
                            buttonColor : (id_report_continous.enabled ==true)?enabledcolor:disabledcolor
                            id:id_report_continous
                            enabled:false
                            visible: false
                            width: primaryButtonText
                            text:datawizardobject.getSelectedIndexString(DataReportType.DATAWIZARD_REPORT_CONTINOUS)
                            onClicked:
                            {
                                datawizardobject.setSelectedReportType(DataReportType.DATAWIZARD_REPORT_CONTINOUS)
                                currentWizardStep = DataWizardStep.DATAWIZARD_STEP_OUTPUT
                                bContinousReportItems = true
                                id_reportgen_summary_viewreport.text = qmlTextSaveReport

                            }
                        }

                        Column {
                            spacing: 20

                            PrimaryButton
                            {
                                id:id_report_single
                                width: primaryButtonText
                                text:datawizardobject.getSelectedIndexString(DataReportType.DATAWIZARD_REPORT_SINGLE)
                                onClicked:
                                {
                                    datawizardobject.setSelectedReportType(DataReportType.DATAWIZARD_REPORT_SINGLE)
                                    currentWizardStep = DataWizardStep.DATAWIZARD_STEP_OUTPUT
                                    bContinousReportItems = false
                                }
                            }

                            PrimaryButton
                            {
                                id:id_vacuum_button
                                width: id_vacuum_image.sourceSize.width
                                text: ""
                                Image
                                {
                                    id:id_vacuum_image
                                    sourceSize.width: 50
                                    sourceSize.height: 50
                                    anchors.left: parent.left
                                    anchors.verticalCenter: parent.verticalCenter
                                    /*Image icon update based on System Type*/
                                    source: (qmlSystemType === qmlSystemTypeASX) ? "qrc:/images/image/Vacuum_ASX.png" :
                                                                                   "qrc:/images/image/Vacuum.png"
                                }
                                onClicked:
                                {
                                    recipe.updateStatusText(qmlTextVacuumRunning);
                                    sysconfig.startVacuum();
                                    id_vacuum_loader.open();
                                }
                            }

                        }
                    }
                    //Tracking the current selection via events if any
                    Connections
                    {
                        target: datawizardobject
                        onSelectedReportTypeChanged:
                        {
                            datawizardobject.getCurrentStatusofValues();
                        }
                        onSelectedFrequencyTypeChanged:
                        {
                            datawizardobject.getCurrentStatusofValues();
                        }
                        onSelectedFrequencyPeriodDateChanged:
                        {
                            datawizardobject.getSelectedFrequencyPeriodDate();
                        }
                        onSelectedFrequencyPeriodTypeChanged:
                        {
                            datawizardobject.getSelectedFrequencyPeriodType();
                        }
                        onSelectedCyclesChanged:
                        {
                            datawizardobject.getSelectedCycles();
                        }

                    }
                }
            }

            //output content
            Rectangle
            {
                id:id_report_output_content
                anchors.left: parent.left
                CustomGroupBox
                {
                    ListView
                    {
                        id:reporttableoutputview
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        model:outputmodellist
                        spacing:50
                        height: id_reportgen_content.height
                        delegate: Rectangle
                        {
                            id:outputsingleitem
                            height: 50
                            Rectangle {
                                visible: getOutputVisibleDetails(reporttableoutputview.currentIndex)
                                Text
                                {
                                    id:modelOutputText
                                    text:modelData.strFilterName
                                    font : theme.textFont
                                    MouseArea
                                    {
                                        width:parent.width
                                        height: parent.height
                                        onClicked:
                                        {
                                            if(index == 1)
                                            {
                                                rect_scheduling = !rect_scheduling
                                                outputsingleitem.height =  (bOutputrectvisible == true) ?50:10
                                                currentoutputimg.source = (rect_scheduling == 1) ?downimage:upimage
                                            }
                                            else
                                            {
                                                bOutputrectvisible = !bOutputrectvisible
                                                rect_scheduling = 0
                                                dynamictableoutputdata.visible = bOutputrectvisible
                                                outputsingleitem.height = (bOutputrectvisible == true) ?50:10
                                                currentoutputimg.source = (bOutputrectvisible == true) ?downimage:upimage
                                                nCurrentOutputIndex = index
                                            }
                                        }

                                    }
                                }
                                Image
                                {
                                    id:currentoutputimg
                                    anchors.left: modelOutputText.right
                                    anchors.top:modelOutputText.top
                                    anchors.rightMargin: 10
                                    source: downimage
                                    width:30
                                    height:30
                                    MouseArea
                                    {
                                        width:parent.width
                                        height: parent.height
                                        onClicked:
                                        {
                                            if(index == 1)
                                            {
                                                rect_scheduling = !rect_scheduling
                                                outputsingleitem.height = (bOutputrectvisible == true) ?50:10
                                                currentoutputimg.source = (rect_scheduling == 1) ?downimage:upimage
                                            }
                                            else
                                            {
                                                bOutputrectvisible = !bOutputrectvisible
                                                rect_scheduling = 0
                                                dynamictableoutputdata.visible = bOutputrectvisible
                                                currentoutputimg.source = (bOutputrectvisible == true) ?downimage:upimage
                                                nCurrentOutputIndex = index
                                            }
                                            selectedDetails(items[2], currentWizardStep)
                                        }
                                    }

                                }
                            }
                            Rectangle {
                                id:dynamictableoutputdata
                                visible: true
                                anchors.top: outputsingleitem.top
                                anchors.topMargin: 30
                                anchors.bottomMargin: 20
                                anchors.right: parent.right
                                anchors.left: parent.left
                                Row{
                                    spacing: 50
                                    ExclusiveGroup {id : group_output}
                                    Repeater
                                    {
                                        id:id_report_output_subcontent_repeater
                                        model:modelData.strFilterList

                                        CustomRadioButton
                                        {
                                            id:id_report_outputsubcontentlist
                                            text:modelData
                                            checked:((index == nCurrentSubOutputIndex)?true:false)
                                            //enabled: modelData == "PDF" ? false : true
                                            exclusiveGroup: group_output
                                            onClicked:
                                            {
                                                nCurrentSubOutputIndex = index
                                                outputFormatString=outputmodellist[nCurrentOutputIndex].strFilterList[nCurrentSubOutputIndex];
                                                selectedDetails(items[2],currentWizardStep)
                                                datawizardobject.setOutputType(nCurrentOutputIndex, nCurrentSubOutputIndex)
                                                datawizardobject.getSelectedForSingleReport()

                                            }
                                        }
                                    }
                                }
                                Component.onCompleted:
                                {
//                                    nCurrentSubOutputIndex = index;
                                    outputFormatString = outputmodellist[nCurrentOutputIndex].strFilterList[nCurrentSubOutputIndex]

                                }
                            }
                        }
                    }
                    Component.onCompleted:
                    {
                        nCurrentOutputIndex =0;
                    }
                    Rectangle{
                        id:rect_scheduling_gray
                        visible:  (rect_scheduling == 1)
                        anchors.top: parent.top
                        anchors.topMargin: 130
//                        width: id_report_output_content.width/2+id_frequency_time_weekly_scheduling.width
                        width:  id_report_output_content.width/2
                        height: id_report_output_content.height/8 //rect_freq.height/8
                        color: "#e6e6e6"
                        Text
                        {
                            id:subPeriodText_scheduling
                            visible: (rect_scheduling == 1)
                            anchors.left: parent.left
                            anchors.leftMargin: 3
                            anchors.verticalCenter: parent.verticalCenter
                            text:qmlTextFrequency
                            font : theme.textFont
                        }
                        Rectangle{
                            anchors.left: subPeriodText_scheduling.right
                            anchors.leftMargin: 10
                            anchors.top:parent.top
                            anchors.topMargin: parent.height/5
                        }
                    }
                }
                CalendarPopUpForFilter
                {
                    id:outputTimeSelect
                }
            }

            //content of storage
            Rectangle
            {
                id:id_report_storage_content
                anchors.left: parent.left
                CustomGroupBox
                {
                    Grid
                    {
                        rows:5
                        columns: 10
                        columnSpacing: 20
                        rowSpacing: 20
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        anchors.leftMargin: 0
                        anchors.left: parent.left
                        Repeater
                        {
                            id:storagerack
                            model:storagemodellist

                            CustomRadioButton
                            {
                                id:storagetype
                                height: 20
                                text:modelData
                                checked: ((index ==nCurrentStorageIndex)?true:false)
                                onClicked:
                                {
                                    nCurrentStorageIndex = index
                                    selectedDetails(items[3],currentWizardStep)

                                }
                            }

                        }

                    }
                    Component.onCompleted:
                    {
                        //Storage selection
                        storagerack.itemAt(nCurrentStorageIndex).checked = true

                    }

                }

            }

            //data filter content for Continuos report
            Rectangle
            {
                id:id_report_datafilter_content
                anchors.left: parent.left

                Rectangle{
                    visible: (bContinousReportItems == true)
                    anchors.fill: parent
                    color: "#ECECEC"
                    CustomGroupBox
                    {
                        anchors.fill: parent
                        Grid
                        {
                            id:reporttableview_cr
                            width:parent.width
                            height: parent.height
                            columns: 4
                            spacing: 2
                            clip: true
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: 20
                            anchors.left: parent.left
                            Rectangle
                            {
                                id:id_weldDataTitleRect
                                width: parent.width/4
                                height: parent.height
                                color: grayRectColor
                                Text
                                {
                                    id:weldData
                                    text:qmlTextWeldData
                                    width: 140
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    id:exportwelddata
                                    anchors.left: weldData.right
                                    text: qmlTextExport
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    anchors.left: exportwelddata.right
                                    anchors.leftMargin:  20
                                    text: qmlTextDelete
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }

                                GridView
                                {
                                    id:weldDataGridView
                                    anchors.top: parent.top
                                    anchors.topMargin: 50
                                    width: parent.width
                                    height: parent.height
                                    model: weldDatamodel
                                    cellWidth:parent.width
                                    cellHeight:50

                                    delegate:  Row
                                    {
                                        spacing:10
                                        Text {
                                            id: weldsubDelegateTitle
                                            text:model.modelData.strSubTitleName
                                            width:160
                                            font {
                                                family: Style.regular.name
                                                pixelSize: Style.style2
                                            }
                                        }
                                        CheckBox
                                        {
                                            id:weldDataexportCheckBox
                                            checked: false
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(weldData.text,index,weldDataexportCheckBox.checked,weldDataDeleteCheckBox.checked)
                                            }
                                        }
                                        CheckBox
                                        {
                                            id:weldDataDeleteCheckBox
                                            checked: false
                                            visible: weldDataexportCheckBox.checked ? true : false
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(weldData.text,index,weldDataexportCheckBox.checked,weldDataDeleteCheckBox.checked)
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                id:id_AlarmDataTitleRect
                                width: parent.width/4
                                height: parent.height
                                color:grayRectColor
                                Text
                                {
                                    id:alarmData
                                    text: qmlTextAlarmData
                                    width: 140
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    id:exportalarmData
                                    anchors.left: alarmData.right
                                    text: qmlTextExport
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    anchors.left: exportalarmData.right
                                    anchors.leftMargin: 20
                                    text: qmlTextDelete
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                GridView
                                {
                                    anchors.top: parent.top
                                    anchors.topMargin: 50
                                    width: parent.width
                                    height: parent.height
                                    model: alarmDatamodel
                                    cellWidth:parent.width
                                    cellHeight: 50
                                    delegate:  Row
                                    {
                                        spacing:10
                                        Text {
                                            id: alarmsubDelegateTitle
                                            text: model.modelData.strSubTitleName
                                            width:160
                                            font {
                                                family: Style.regular.name
                                                pixelSize: Style.style2
                                            }
                                        }
                                        CheckBox
                                        {
                                            id:alarmDataExportCheckBox
                                            checked: false
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(alarmData.text,index,alarmDataExportCheckBox.checked,alarmDataDeleteCheckBox.checked)
                                            }
                                        }
                                        CheckBox
                                        {
                                            id:alarmDataDeleteCheckBox
                                            visible: alarmDataExportCheckBox.checked ? true : false
                                            checked: false
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(alarmData.text,index,alarmDataExportCheckBox.checked,alarmDataDeleteCheckBox.checked)
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                width: parent.width/4
                                height: parent.height
                                color: grayRectColor

                                Text
                                {
                                    id:systemsett
                                    text: qmlTextSystemSettings
                                    width: 140
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    id:exportsystemsett
                                    anchors.left: systemsett.right
                                    text: qmlTextExport
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    anchors.left: exportsystemsett.right
                                    anchors.leftMargin: 20
                                    text: qmlTextDelete
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }

                                GridView
                                {
                                    anchors.top: parent.top
                                    anchors.topMargin: 50
                                    width: parent.width
                                    height: parent.height
                                    model: systemModel
                                    cellWidth:parent.width
                                    cellHeight: 50
                                    delegate:  Row
                                    {
                                        spacing:10
                                        Text {
                                            id:systemSettingTitle
                                            text: model.modelData.strSubTitleName
                                            width:160
                                            font {
                                                family: Style.regular.name
                                                pixelSize: Style.style2
                                            }
                                        }
                                        CheckBox
                                        {
                                            checked: false
                                            id:systemSettingExportCheckBox
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(systemsett.text,index,systemSettingExportCheckBox.checked,systemSettingDeleteCheckBox.checked)
                                            }
                                        }
                                        CheckBox
                                        {
                                            id:systemSettingDeleteCheckBox
                                            visible: systemSettingExportCheckBox.checked ? true : false
                                            checked: false
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(systemsett.text,index,systemSettingExportCheckBox.checked,systemSettingDeleteCheckBox.checked)
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                width: parent.width/4
                                height: parent.height
                                color: grayRectColor
                                Text
                                {
                                    id:eventData
                                    text: qmlTextEventData
                                    width: 140
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    id:exporteventData
                                    anchors.left: eventData.right
                                    text: qmlTextExport
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }
                                Text
                                {
                                    anchors.left: exporteventData.right
                                    anchors.leftMargin:  20
                                    text: qmlTextDelete
                                    width: 50
                                    font {
                                        family: Style.semibold.name
                                        pixelSize: Style.style4
                                    }
                                }

                                GridView
                                {
                                    anchors.top: parent.top
                                    anchors.topMargin: 50
                                    width: parent.width
                                    height: parent.height
                                    model: eventModel
                                    cellWidth:parent.width
                                    cellHeight: 50
                                    delegate:  Row
                                    {
                                        spacing:10
                                        Text {
                                            id:eventDataTitle
                                            width:160
                                            text: model.modelData.strSubTitleName
                                            font {
                                                family: Style.regular.name
                                                pixelSize: Style.style2
                                            }
                                        }
                                        CheckBox
                                        {
                                            id:eventDataExportCheckBox
                                            checked: false
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(eventData.text,index,eventDataExportCheckBox.checked,eventDataDeleteCheckBox.checked)
                                            }
                                        }
                                        CheckBox
                                        {
                                            id:eventDataDeleteCheckBox
                                            visible: eventDataExportCheckBox.checked ? true : false
                                            checked: false
                                            width: 50
                                            height: 50
                                            onClicked:
                                            {
                                                datawizardobject.storeDataFilters(eventData.text,index,eventDataExportCheckBox.checked,eventDataDeleteCheckBox.checked)
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //data filter content for SingleReport
                Rectangle
                {
                    id:id_report_datafilter_content_sr
                    visible: (bContinousReportItems == false)
                    anchors.fill: parent
                    CustomGroupBox
                    {
                        id:custom_group_box_id_sr
                        Column
                        {
                            id:reporttableview_sr
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: 20
                            anchors.left: parent.left
                            spacing:20
                            height: id_reportgen_content.height
                            ExclusiveGroup{id : group1}
                            Repeater
                            {
                                model: datafiltermodelsr
                                delegate: Rectangle
                                {
                                    id:singleitem_sr
                                    height:60
                                    width:800
                                    Rectangle
                                    {
                                        height: 20
                                        Text
                                        {
                                            id:modelText_sr
                                            text:modelData.strFilterName
                                            font : theme.textFont
                                            MouseArea
                                            {
                                                width:parent.width
                                                height: parent.height
                                                onClicked:
                                                {
                                                    brectvisible = !brectvisible
                                                    dynamictabledata_sr.visible = brectvisible
                                                    singleitem_sr.height = (brectvisible == true) ? 60:10
                                                    currentimg_sr.source = (brectvisible == true) ? downimage:upimage
                                                    nCurrentDataFilter = index
                                                    selectedDetails(items[2], currentWizardStep)
                                                }
                                            }
                                        }
                                        Image
                                        {
                                            id:currentimg_sr
                                            anchors.left: modelText_sr.right
                                            anchors.top:modelText_sr.top
                                            anchors.rightMargin: 10
                                            width:30
                                            height:30
                                            source: downimage
                                            MouseArea
                                            {
                                                width:parent.width
                                                height: parent.height
                                                onClicked:
                                                {
                                                    brectvisible = !brectvisible
                                                    dynamictabledata_sr.visible = brectvisible
                                                    singleitem_sr.height = (brectvisible == true) ?60:10
                                                    currentimg_sr.source = (brectvisible == true) ?downimage:upimage
                                                    nCurrentDataFilter = index
                                                    selectedDetails(items[2],currentWizardStep)
                                                }
                                            }
                                        }
                                    }
                                    //sub_sub_headings(ex:recipes*_welddata_datafilter)
                                    Rectangle {
                                        id:dynamictabledata_sr
                                        visible: true
                                        anchors.top: singleitem_sr.top
                                        anchors.topMargin: 30
                                        anchors.bottomMargin: 20
                                        border.width:1
                                        width: 600
                                        anchors.right: parent.right
                                        anchors.left: parent.left
                                        Row{
                                            id : row_selection_sr
                                            spacing: 100
                                            Text
                                            {
                                                id:dummmytest_sr
                                                visible:false
                                                enabled:false
                                                text:modelData.strFilterName
                                            }

                                            CustomRadioButton
                                            {
                                                id:id_report_subcontentlist_sr1
                                                text:modelData.strFilterList[0]
                                                width:150
                                                visible: (outputFormatString === "PDF")
                                                exclusiveGroup: group1
                                                checked: false
                                                onCheckedChanged:
                                                {
                                                    if(index == 0)
                                                    {
                                                        if(id_report_subcontentlist_sr1.checked == false)
                                                        {
                                                            nCurrentSubDataFilter = 0
                                                        }
                                                    }
                                                    if(id_report_subcontentlist_sr1.checked == false)
                                                        deleteDBRecords.checkBoxStatus = false
                                                }
                                                onClicked:
                                                {
                                                    nCurrentDataFilter = index;
                                                    selectedDetails(items[2],currentWizardStep)
                                                }
                                            }



                                            //the component is only for CSV format
                                            CustomCheckBox{
                                                id: weldDataGraph
                                                checkBoxName: modelData.strFilterList[0]
                                                width: 150
                                                height: 50
                                                visible: (outputFormatString === "CSV")
                                                checkBoxStatus: false
                                                onCheckBoxStatusChanged:{                                                   
                                                    if(checkBoxName === qmlTextWeldResult) /* the status of weld result check box is changed */
                                                    {
                                                        if(checkBoxStatus === false) /* the box is unchecked */
                                                        {
                                                            // it needs to judge if there is other check box checked, if not, the box will not be unchecked.
                                                            if(weldGraphCheckStatus === false)
                                                            {
                                                                weldResultCheckStatus = true
                                                                checkBoxStatus = true
                                                                nCurrentDataFilter = index;
                                                            }
                                                            else /* if the weld graph box is checked, the weld result box will be unchecked.  */
                                                            {
                                                                weldResultCheckStatus = false
                                                                checkBoxStatus = false
                                                                 nCurrentDataFilter = 1
                                                            }
                                                        }
                                                        else{   /*  the weld result box is checked. */
                                                            weldResultCheckStatus = true
                                                            nCurrentDataFilter = index;
                                                        }

                                                    }
                                                    else if(checkBoxName === qmlTextWeldGraphDataText) /* the status of the weld graph data box is changed  */
                                                    {

                                                        if(checkBoxStatus === false) /* the weld graph data box is unchecked. */
                                                        {
                                                            if(weldResultCheckStatus === false) /* the weld graph data is not checked */
                                                            {
                                                                checkBoxStatus = true
                                                                weldGraphCheckStatus = true
                                                                nCurrentDataFilter = index;
                                                            }
                                                            else
                                                            {
                                                                weldGraphCheckStatus = false
                                                                checkBoxStatus = false
                                                                weldGraphDBCheckStatus = false
                                                                nCurrentDataFilter = 0
                                                            }
                                                        }
                                                        else  /* the weld graph data box is checked. */
                                                        {
                                                            weldGraphCheckStatus = true
                                                            checkBoxStatus = true
                                                            nCurrentDataFilter = index;

                                                        }
                                                    }
                                                    else if(checkBoxName === qmltextDeleteDBRecords)
                                                    {
                                                        //
                                                        if(checkBoxStatus === true)
                                                        {
                                                            nCurrentDeleteDBCheckBoxStatus = 1
                                                        }
                                                        else
                                                        {
                                                            nCurrentDeleteDBCheckBoxStatus = 0
                                                        }                                                       
                                                    }


                                                    if(weldResultCheckStatus === true && weldGraphCheckStatus === true)
                                                    {
                                                        weldResultAndGraph = 1
                                                    }
                                                    else
                                                    {
                                                        weldResultAndGraph = 0
                                                    }

                                                    selectedDetails(items[2],currentWizardStep)
                                                    if(checkBoxName === qmltextDeleteDBRecords)
                                                    {
                                                        summaryDeleteDBDetails()
                                                    }
                                                }

                                            }

                                            CustomCheckBox
                                            {
                                                id: deleteDBRecords
                                                checkBoxStatus: false
                                                checkBoxName: qmltextDeleteDBRecords
                                                width: 140
                                                height: 50
                                                visible: {
                                                    if(qmlTextSystemSettings == modelData.strFilterName)
                                                        return false
                                                    else if(qmlTextUserData == modelData.strFilterName)
                                                        return false
                                                    else if(qmlTextRecipeData == modelData.strFilterName)
                                                        return false
                                                    else
                                                    {
                                                        if(outputFormatString === "PDF")
                                                        {
                                                            return id_report_subcontentlist_sr1.checked
                                                        }
                                                        else if(outputFormatString === "CSV")
                                                        {
                                                            return false
                                                        }

                                                    }

                                                }
                                                onCheckBoxStatusChanged:
                                                {
                                                    if(deleteDBRecords.checkBoxStatus == true)
                                                    {
                                                        nCurrentDeleteDBCheckBoxStatus = 1
                                                        if(outputFormatString === "CSV")
                                                        {
                                                            if(index === 0) /*Weld Result Data*/
                                                            {
                                                                weldResultDBCheckStatus = true
                                                            }
                                                            else if(index === 1)/*Weld graph Data*/
                                                            {
                                                                weldGraphDBCheckStatus = true
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        nCurrentDeleteDBCheckBoxStatus = 0
                                                        if(outputFormatString === "CSV")
                                                        {
                                                            if(index === 0) /*Weld Result Data*/
                                                            {
                                                                weldResultDBCheckStatus = false
                                                            }
                                                            else if(index === 1)/*Weld graph Data*/
                                                            {
                                                                weldGraphDBCheckStatus = false
                                                            }
                                                        }
                                                    }

                                                    summaryDeleteDBDetails()
                                                }
                                            }
                                        }
                                        Component.onCompleted:
                                        {
                                            //Sub fileds of data filter
                                            nCurrentSubDataFilter = 0;
                                            nCurrentDataFilter = 0; 
                                            nCurrentDeleteDBCheckBoxStatus = 0
                                            if(index == 0)
                                            {
                                                if(outputFormatString === "PDF")
                                                {
                                                    id_report_subcontentlist_sr1.checked = true
                                                }
                                                else
                                                {
                                                    weldDataGraph.checkBoxStatus = true
                                                    weldResultCheckStatus = true
                                                    weldGraphCheckStatus = false
                                                    weldResultAndGraph = 0
                                                }

                                            }

                                            else
                                            {
                                                if(outputFormatString === "PDF")
                                                {
                                                    id_report_subcontentlist_sr1.checked = false
                                                }
                                                else
                                                {
                                                    weldDataGraph.checkBoxStatus = false
                                                    weldGraphCheckStatus = false
                                                }
                                            }

                                        }
                                    }

                                    Component.onCompleted:
                                    {
                                        nCurrentDataFilter = index
                                    }
                                }
                            }
                        }
//                        Component.onCompleted:
//                        {
//                            //data filter List view on completion
//                            nCurrentDataFilter = currentWizardStep;
//                        }
                    }
                }
            }

            //summary content
            Rectangle
            {
                id : rect_summary
                anchors.left: parent.left
                StackLayout{
                    id: stackLayout1
                    CustomGroupBox
                    {
                        id:id_report_summary_content
                        Rectangle{
                            anchors.top: parent.top
                            anchors.topMargin: 20
                            id : rect_summary_content
                            width: Math.max(id_report_summary_textcontent.width, id_report_summary_textcontent1.width)//id_report_summary_textcontent.width+10
                            height: id_report_summary_textcontent.height+10
                            color : "transparent"
                            border.width: 2
                            border.color: "black"
                        }
                        Text
                        {
                            id:id_report_summary_textcontent
                            anchors.top: parent.top
                            anchors.topMargin: 20
                            text: strsummary
                            font.bold: true
                            font.pointSize: 13;
                            color: "#868e96"
                            //padding: 20
                        }

                    }              

                    CustomGroupBox
                    {
                        id:id_report_summary_content1
                        visible: ((weldResultCheckStatus === true) && (weldGraphCheckStatus === true) && (outputFormatString === "CSV"))
                        Rectangle{
                            id : rect_summary_content1
                            anchors.top: parent.top
                            anchors.topMargin: 150
                            width: Math.max(id_report_summary_textcontent.width, id_report_summary_textcontent1.width)//id_report_summary_textcontent1.width+10
                            height: id_report_summary_textcontent1.height+10
                            color : "transparent"
                            border.width: 2
                            border.color: "black"
                        }
                        Text
                        {
                            id:id_report_summary_textcontent1
                            anchors.top: rect_summary_content1.top
                            text: {
                                var tempSummary = ""
                                tempSummary = summaryTheDetailed()
                                return tempSummary
                            }
                            font.bold: true
                            font.pointSize: 13;
                            color: "#868e96"
                        }

                    }

                }



                PrimaryButton
                {
                    id:id_reportgen_summary_viewreport
                    anchors.top: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: 270
                    text: qmlTextGenerateReport
                    onClicked:
                    {
                        if(nCurrentDeleteDBCheckBoxStatus === 1)
                        {
                            showDeleteDBInfoMessage()
                        }
                        else
                        {
                            id_reportgen_content.generateReports()
                        }

                }

            }


        }
    }

        /**
        *@breif: To generate the summary information.
        **/
        function generateReports()
        {
            if( id_reportgen_summary_viewreport.text==qmlTextSaveReport){
                //insert to db
                var outputTime=""

                if(nFrequencyType == 0)
                {
                    //frequency selected time
                    if(freqPeriod==qmltextDaily)
                        freqMonthWeekDaily=""
                    freqTime = frequencyTimeSelect.timePicker.timeLabel.text+"|"+freqMonthWeekDaily

                }
                else
                {
                    freqPeriod = qmltextCycleVal
                    freqTime = datawizardobject.cyclesText  //cyclecount value

                }
                if(outputSchedulefreq==qmltextDaily)
                    outputScheduleMonthlyWeeklyDaily=""
                outputTime =  outputTimeSelect.timePicker.timeLabel.text+"|"+outputScheduleMonthlyWeeklyDaily

                summary="'"+(datawizardobject.getSelectedReportType()+1)+"','"+(nFrequencyType+1)+"','"+freqPeriod+"','"+freqTime+"','"
                        +storagemodellist[nCurrentStorageIndex]+"','"+outputFormatString+"','"+outputSchedulefreq+"','"+outputTime+"'";
                datawizardobject.storeReportSummary(summary);
            }
            //id_reportgen_wait_popup.open();
            id_report_loader.open();
            recipe.updateStatusText(qmlTextReportRunning);
            sysconfig.generateReport(nCurrentDataFilter, weldResultAndGraph, nCurrentSubOutputIndex, nCurrentDeleteDBCheckBoxStatus)
        }

    }



    Rectangle
    {
        id:statusbtn
        anchors.top:id_reportgen_content.bottom
        width: id_reportgen_content.width
        visible:(((layout.currentIndex > 0) && (layout.currentIndex < (items.length)))?true:false)
        anchors.rightMargin: 10
        border.width:1
        PrimaryButton
        {
            id:id_reportgennextbtn
            anchors.right: parent.right
            visible: (((layout.currentIndex > 0) && (layout.currentIndex < (items.length - 1)))?true:false)
            text:qmlNextText
            onClicked:
            {
                if(currentWizardStep <= DataWizardStep.DATAWIZARD_STEP_DATAFILTER)
                {
                    currentWizardStep++
                }
                if(currentWizardStep == DataWizardStep.DATAWIZARD_STEP_OUTPUT)
                {
                    selectedDetails(items[1], currentWizardStep)
                    datawizardobject.outputRefresh()
                }
            }
        }


        //the back button
        PrimaryButton{
            id: id_reportGenBackBtn
            anchors.right: id_reportgennextbtn.left
            anchors.rightMargin: 20
            anchors.top: id_reportgennextbtn.top
            visible: statusbtn.visible
            text: qmlBackText
            onClicked: {
                if(currentWizardStep >= DataWizardStep.DATAWIZARD_STEP_OUTPUT)
                {
                    currentWizardStep--
                }

                if(currentWizardStep === DataWizardStep.DATAWIZARD_STEP_OUTPUT)
                {
                    selectedDetails(items[1], currentWizardStep)
                    datawizardobject.outputRefresh()
                }
            }
        }





    }
}

