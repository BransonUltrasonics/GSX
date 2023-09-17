/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Handels single report and continous report wizard UI

***************************************************************************/
#include "Header/datawizard.h"
#include "Header/datafilter.h"
#include "Header/alarmconfiguration.h"
#include "Header/communicationinterface.h"
#include "Header/qtcommon.h"

extern AlarmConfiguration alarmconfig;

//Enable the below macro for continous reports.
//#define CONTINOUS
//#define SINGLE 1
extern CommunicationInterface CIObj;
extern QQmlContext* context;

/**
 * @brief DataWizard::DataWizard : Constructor
 * @param parent
 */
DataWizard::DataWizard(QObject *parent) : QObject(parent)
{
    QStringList strList;

    m_OutputsList.clear();
    m_OutputType = DataWizardOutputEnums::DATAWIZARD_OUTPUT_CSV;

    //    strList.clear();
    //    strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_RESULTS);
    //    strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_RECIPES);
    //    strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_GRAPH);

    //#ifdef CONTINOUS
    //m_OutputsList.append(new DataFilter("Format",{"PDF","CSV"})); //"XLS","RAW","XML/XHTML"
    //    m_OutputsList.append(new DataFilter("Scheduling",{"test","test","tes1t"}));
    //    m_OutputsList.append(new DataFilter("Recipients",{"Username1","Username2","Username3","Username4","Username5"}));
    //#else
    //m_OutputsList.append(new DataFilter("Format",{"CSV","PDF"}));
    // m_OutputsList.append(new DataFilter("Scheduling",{}));
    // m_OutputsList.append(new DataFilter("Recipients",{}));

    //#endif

    m_SelectedReportType = DataWizardEnums::DATAWIZARD_REPORT_SINGLE;
    m_SelectedFrequencyType = DataWizardEnums::DATAWIZARD_FREQUENCY_CYCLE;
    m_SelectedFrequencyPeriodType = DATAWIZARD_FREQUENCY_TIME_PERIOD_MONTHLY;

    m_DataWizardStrings.clear();
    m_DataWizardStrings.insert(DATAWIZARD_REPORT_CONTINOUS,DATAWIZARD_REPORT_CONTINOUS_STR);
    m_DataWizardStrings.insert(DATAWIZARD_REPORT_SINGLE,DATAWIZARD_REPORT_SINGLE_STR);
    m_DataWizardStrings.insert(DATAWIZARD_FREQUENCY_TIME,DATAWIZARD_FREQUENCY_TIME_STR);
    m_DataWizardStrings.insert(DATAWIZARD_FREQUENCY_CYCLE,DATAWIZARD_FREQUENCY_CYCLE_STR);
    //    m_DataWizardStrings.insert(DATAWIZARD_FREQUENCY_SHIFT,DATAWIZARD_FREQUENCY_SHIFT_STR);
    m_DataWizardStrings.insert(DATAWIZARD_FREQUENCY_TIME_PERIOD_DAILY,DATAWIZARD_FREQUENCY_TIME_PERIOD_DAILY_STR);
    m_DataWizardStrings.insert(DATAWIZARD_FREQUENCY_TIME_PERIOD_MONTHLY,DATAWIZARD_FREQUENCY_TIME_PERIOD_MONTHLY_STR);
    m_DataWizardStrings.insert(DATAWIZARD_FREQUENCY_TIME_PERIOD_WEEKLY,DATAWIZARD_FREQUENCY_TIME_PERIOD_WEEKLY_STR);

    //frequencyperiodmodel
    m_FrequencyPeriods.clear();
    m_FrequencyPeriods.append(DATAWIZARD_FREQUENCY_TIME_PERIOD_DAILY_STR);
    m_FrequencyPeriods.append(DATAWIZARD_FREQUENCY_TIME_PERIOD_WEEKLY_STR);
    m_FrequencyPeriods.append(DATAWIZARD_FREQUENCY_TIME_PERIOD_MONTHLY_STR);

    context->setContextProperty("frequencyperiodmodel", QVariant :: fromValue(m_FrequencyPeriods));

    m_FrequencyPeriods_Time_Weekly.clear();
    m_FrequencyPeriods_Time_Weekly.append("Sunday");
    m_FrequencyPeriods_Time_Weekly.append("Monday");
    m_FrequencyPeriods_Time_Weekly.append("Tuesday");
    m_FrequencyPeriods_Time_Weekly.append("Wednesday");
    m_FrequencyPeriods_Time_Weekly.append("Thursday");
    m_FrequencyPeriods_Time_Weekly.append("Friday");
    m_FrequencyPeriods_Time_Weekly.append("Saturday");
    context->setContextProperty("frequencyperiodmodeltimeweeks", QVariant :: fromValue(m_FrequencyPeriods_Time_Weekly));

    m_FrequencyPeriods_Time_Monthly.clear();
    for(int i=1;i<=31;i++)
        m_FrequencyPeriods_Time_Monthly.append(QString::number(i));
    context->setContextProperty("frequencyperiodmodeltimemonthly", QVariant :: fromValue(m_FrequencyPeriods_Time_Monthly));


    m_StorageList.clear();
    m_StorageList.append(DATAWIZARD_STORAGE_USB_STR);

    context->setContextProperty("storagemodellist", QVariant::fromValue(m_StorageList));



    m_FrequencyPeriodDate = QDateTime::currentDateTime();
    int FilterCount;
    m_DataFiltersList.clear();
    for(int nFilter =DataWizardDataFilters::DATAWIZARD_DATAFILTER_WELD_DATA ; nFilter <= DataWizardDataFilters::DATAWIZARD_DATAFILTER_EVENT_DATA;nFilter++)
    {
        QString strFilterName = getFilterName(nFilter);

        int v = rand() % strFilterName.length() + 1;
        Q_UNUSED(v);

        strList.clear();
       // strList.append("None");
        if(nFilter == DataWizardDataFilters::DATAWIZARD_DATAFILTER_WELD_DATA)
        {
            FilterCount = 0;
            strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_RESULTS);
           // strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_RECIPES);
            strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_GRAPH);

            //            strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_RECIPES);
            //            strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_PARAMETER);
            //            strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_OTHER);


        }
        else if(nFilter == DataWizardDataFilters::DATAWIZARD_DATAFILTER_ALARM_DATA)
        {
            FilterCount = 1 ;

            strList.append(getAlarmRows());

        } /*else if(nFilter == DataWizardDataFilters::DATAWIZARD_DATAFILTER_USER_DATA){
            strList.clear();
            FilterCount = 2 ;
            strList.append("User Data1");
              strList.append("User Data2");
                strList.append("User Data3");
                  strList.append("User Data4");*/
        else if(nFilter == DataWizardDataFilters::DATAWIZARD_DATAFILTER_SYSTEMSETTINGS_DATA)
        {
            FilterCount = 3 ;
            strList.append("Sys Settings");
            //              strList.append("Sys Data2");
            //                strList.append("Sys Data3");
            //                  strList.append("Sys Data4");
        }
        else if(nFilter == DataWizardDataFilters::DATAWIZARD_DATAFILTER_EVENT_DATA)
        {
            FilterCount = 4 ;
            strList.append("Events");

        }
        else{

        }
        m_DataFiltersList.append(new DataFilter(strFilterName,strList,FilterCount));
    }
    outputRefresh();
    context->setContextProperty("datafiltermodel", QVariant::fromValue(m_DataFiltersList));

    getSelectedForSingleReport();
    context->setContextProperty("datafiltermodelsr", QVariant::fromValue(m_DataFiltersList));

    createModelforDataFilter();
}

/**
 * @brief DataWizard::createModelforDataFilter : Creates model for datafilter and updates in UI
 */
void DataWizard::createModelforDataFilter()
{

    m_weldDataList.clear();
    m_alarmDataList.clear();
    m_SystemDataList.clear();
    m_eventDataList.clear();

    defaultDatafilterValues();

    m_weldDataList.append(new DataFilter(DATA_WIZARD_DATAFILTER_WELDDATA_RESULTS,weldDataFilter.resultExport,weldDataFilter.resultDelete));
    m_weldDataList.append(new DataFilter(DATA_WIZARD_DATAFILTER_WELDDATA_RECIPES,weldDataFilter.recipeExport,weldDataFilter.recipeDelete));
    m_weldDataList.append(new DataFilter(DATA_WIZARD_DATAFILTER_WELDDATA_GRAPH,weldDataFilter.graphExport,weldDataFilter.graphDelete));
    context->setContextProperty("weldDatamodel", QVariant::fromValue(m_weldDataList));


    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_OVERLOADS,alarmDataFilter.overloadExport,alarmDataFilter.overloadDelete));
    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_CYCLE_MODIFIED,alarmDataFilter.cyclemodifyExport,alarmDataFilter.cyclemodifyDelete));
    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_WARNINGS,alarmDataFilter.warningExport,alarmDataFilter.warningDelete));
    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_SUSPECT,alarmDataFilter.suspectExport,alarmDataFilter.suspectDelete));
    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_REJECT,alarmDataFilter.rejectExport,alarmDataFilter.rejectDelete));
    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_NO_CYCLE,alarmDataFilter.nocycleExport,alarmDataFilter.nocycleDelete));
    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_HARDWARE_FAILURE,alarmDataFilter.hardwareFailExport,alarmDataFilter.hardwareFailDelete));
    m_alarmDataList.append(new DataFilter(ALARM_CONFIG_NO_CYCLE_OVERLOADS,alarmDataFilter.nocycleOverloadExport,alarmDataFilter.nocycleOverloadDelete));

    context->setContextProperty("alarmDatamodel", QVariant::fromValue(m_alarmDataList));


    m_SystemDataList.append(new DataFilter("Sys Settings",systemFilter.systemExport,systemFilter.systemDelete));
    context->setContextProperty("systemModel",QVariant::fromValue(m_SystemDataList));

    m_eventDataList.append(new DataFilter("Events",eventFilter.eventExport,eventFilter.eventDelete));
    context->setContextProperty("eventModel",QVariant::fromValue(m_eventDataList));

}

void DataWizard::storeDataFilters(QString dataFilterName,int currentIndex,bool exportdata, bool deletedata)
{
    if(dataFilterName == WELD_DATA)
    {
        switch (currentIndex) {
        case 0:
            weldDataFilter.resultExport = exportdata;
            weldDataFilter.resultDelete = deletedata;
            break;
        case 1:
            weldDataFilter.recipeExport = exportdata;
            weldDataFilter.recipeDelete = deletedata;
            break;
        case 2:
            weldDataFilter.graphExport = exportdata;
            weldDataFilter.graphDelete = deletedata;
            break;
        default:
            break;
        }

    }
    else if (dataFilterName == ALARM_DATA)
    {
        switch (currentIndex) {
        case 0:
            alarmDataFilter.overloadExport = exportdata;
            alarmDataFilter.overloadDelete = deletedata;
            break;
        case 1:
            alarmDataFilter.cyclemodifyExport = exportdata;
            alarmDataFilter.cyclemodifyDelete = deletedata;
            break;
        case 2:
            alarmDataFilter.warningExport = exportdata;
            alarmDataFilter.warningDelete = deletedata;
            break;
        case 3:
            alarmDataFilter.suspectExport = exportdata;
            alarmDataFilter.suspectDelete = deletedata;
            break;
        case 4:
            alarmDataFilter.rejectExport = exportdata;
            alarmDataFilter.rejectDelete = deletedata;
            break;
        case 5:
            alarmDataFilter.nocycleExport = exportdata;
            alarmDataFilter.nocycleDelete = deletedata;
            break;
        case 6:
            alarmDataFilter.hardwareFailExport = exportdata;
            alarmDataFilter.hardwareFailDelete = deletedata;
            break;
        case 7:
            alarmDataFilter.nocycleOverloadExport = exportdata;
            alarmDataFilter.nocycleOverloadDelete = deletedata;
            break;
        default:
            break;
        }
    }
    else if (dataFilterName == SYSTEM_DATA)
    {
        switch (currentIndex) {
        case 0:
            systemFilter.systemExport = exportdata;
            systemFilter.systemDelete = deletedata;
            break;
        default:
            break;
        }

    }
    else if (dataFilterName == EVENT_DATA)
    {
        switch (currentIndex) {
        case 0:
            eventFilter.eventExport = exportdata;
            eventFilter.eventDelete = deletedata;
            break;
        default:
            break;
        }
    }

}

void DataWizard::defaultDatafilterValues()
{
    int exportdata = 0;
    int deletedata = 0;

    weldDataFilter.resultExport = exportdata;
    weldDataFilter.resultDelete = deletedata;

    weldDataFilter.recipeExport = exportdata;
    weldDataFilter.recipeDelete = deletedata;

    weldDataFilter.graphExport = exportdata;
    weldDataFilter.graphDelete = deletedata;

    alarmDataFilter.overloadExport = exportdata;
    alarmDataFilter.overloadDelete = deletedata;

    alarmDataFilter.cyclemodifyExport = exportdata;
    alarmDataFilter.cyclemodifyDelete = deletedata;

    alarmDataFilter.warningExport = exportdata;
    alarmDataFilter.warningDelete = deletedata;

    alarmDataFilter.suspectExport = exportdata;
    alarmDataFilter.suspectDelete = deletedata;

    alarmDataFilter.rejectExport = exportdata;
    alarmDataFilter.rejectDelete = deletedata;

    alarmDataFilter.nocycleExport = exportdata;
    alarmDataFilter.nocycleDelete = deletedata;

    alarmDataFilter.hardwareFailExport = exportdata;
    alarmDataFilter.hardwareFailDelete = deletedata;

    alarmDataFilter.nocycleOverloadExport = exportdata;
    alarmDataFilter.nocycleOverloadDelete = deletedata;

    systemFilter.systemExport = exportdata;
    systemFilter.systemDelete = deletedata;

    eventFilter.eventExport = exportdata;
    eventFilter.eventDelete = deletedata;

}

void DataWizard::storeReportSummary(QString data)
{
    QString updateData=data+"," ;
    QString signatureMsgLimits ="";
    updateData.append("'"+QString::number(weldDataFilter.resultExport)+"|"+QString::number(weldDataFilter.resultDelete)+"',");
    updateData.append("'"+QString::number(weldDataFilter.recipeExport)+"|"+QString::number(weldDataFilter.recipeDelete)+"',");
    updateData.append("'"+QString::number(weldDataFilter.graphExport)+"|"+QString::number(weldDataFilter.graphDelete)+"',");

    updateData.append("'"+QString::number(alarmDataFilter.overloadExport)+"|"+QString::number(alarmDataFilter.overloadDelete)+"',");
    updateData.append("'"+QString::number(alarmDataFilter.cyclemodifyExport)+"|"+QString::number(alarmDataFilter.cyclemodifyDelete)+"',");
    updateData.append("'"+QString::number(alarmDataFilter.warningExport)+"|"+QString::number(alarmDataFilter.warningDelete)+"',");
    updateData.append("'"+QString::number(alarmDataFilter.suspectExport)+"|"+QString::number(alarmDataFilter.suspectDelete)+"',");
    updateData.append("'"+QString::number(alarmDataFilter.rejectExport)+"|"+QString::number(alarmDataFilter.rejectDelete)+"',");
    updateData.append("'"+QString::number(alarmDataFilter.nocycleExport)+"|"+QString::number(alarmDataFilter.nocycleDelete)+"',");
    updateData.append("'"+QString::number(alarmDataFilter.hardwareFailExport)+"|"+QString::number(alarmDataFilter.hardwareFailDelete)+"',");
    updateData.append("'"+QString::number(alarmDataFilter.nocycleOverloadExport)+"|"+QString::number(alarmDataFilter.nocycleOverloadDelete)+"',");

    updateData.append("'"+QString::number(systemFilter.systemExport)+"|"+QString::number(systemFilter.systemDelete)+"',");

    updateData.append("'"+QString::number(eventFilter.eventExport)+"|"+QString::number(eventFilter.eventDelete)+"'");

   // int retVal = OPERATION_SUCCESS ;
    /* Send request message for the  signature data */
    CIObj.sendMessage(REQ_UPDATE_SYSTEM_DATA, RES_UPDATE_SYSTEM_DATA, updateData);

    /* Receive response message of the  signature data */
    CIObj.recivedMessage(RES_UPDATE_SYSTEM_DATA , signatureMsgLimits);


}

/**
 * @brief DataWizard::outputRefresh : Refreshes model for output
 */
void DataWizard::outputRefresh()
{
    m_OutputsList.clear();
    if(m_SelectedReportType == DataWizardEnums::DATAWIZARD_REPORT_SINGLE)
        m_OutputsList.append(new DataFilter(DATAWIZARD_SUMMARY_FORMAT,{"CSV", "PDF"})); //"XLS","RAW","XML/XHTML"
    if(m_SelectedReportType == DataWizardEnums::DATAWIZARD_REPORT_CONTINOUS)
    {
        m_OutputsList.append(new DataFilter(DATAWIZARD_SUMMARY_FORMAT,{"CSV","PDF"})); //"XLS","RAW","XML/XHTML"
        m_OutputsList.append(new DataFilter("Scheduling",{"test","test","tes1t"}));
    }
    context->setContextProperty("outputmodellist", QVariant::fromValue(m_OutputsList));
}

void DataWizard::setOutputType(int currentOuputIndex, int currentSubOutputIndex)
{
    Q_UNUSED(currentOuputIndex)
    m_OutputType = currentSubOutputIndex;
}

/**
 * @brief DataWizard::getAlarmRows : Returns alarm config coloumns
 * @return
 */
QStringList DataWizard::getAlarmRows()
{
    QStringList alarmDataList;
    alarmDataList.append(ALARM_CONFIG_OVERLOADS);
    alarmDataList.append(ALARM_CONFIG_CYCLE_MODIFIED);
    alarmDataList.append(ALARM_CONFIG_WARNINGS);
    alarmDataList.append(ALARM_CONFIG_SUSPECT);
    alarmDataList.append(ALARM_CONFIG_REJECT);
    alarmDataList.append(ALARM_CONFIG_NO_CYCLE);
    alarmDataList.append(ALARM_CONFIG_HARDWARE_FAILURE);
    alarmDataList.append(ALARM_CONFIG_NO_CYCLE_OVERLOADS);
    return alarmDataList;
}

/**
 * @brief DataWizard::getSelectedForSingleReport : Updates UI according to single report when clicked on single report in UI
 */
void DataWizard::getSelectedForSingleReport()
{
    QStringList strList;
    QString strFilterName;
    m_DataFiltersList.clear();
    for(int nFilter = DataWizardDataFilters::DATAWIZARD_DATAFILTER_WELD_DATA; nFilter <= DataWizardDataFilters::DATAWIZARD_DATAFILTER_CSV_DELETE_DB; nFilter++)
    {
        strFilterName = getFilterName(nFilter);
        strList.clear();
        switch (nFilter)
        {
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_WELD_DATA:
        {
            strList.append(DATA_WIZARD_DATAFILTER_WELDDATA_RESULTS);
            m_DataFiltersList.append(new DataFilter(strFilterName,strList));
        }
            break;
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_ALARM_DATA:
            if(m_OutputType == DataWizardOutputEnums::DATAWIZARD_OUTPUT_PDF)
            {
                strList.append(DATAWIZARD_DATAFILTER_ALARM_DATA_STR);
                m_DataFiltersList.append(new DataFilter(strFilterName, strList));
            }
            break;
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_SYSTEMSETTINGS_DATA:
            if(m_OutputType == DataWizardOutputEnums::DATAWIZARD_OUTPUT_PDF)
            {
                strList.append(DATAWIZARD_DATAFILTER_SYSTEMSETTINGS_DATA_STR);
                m_DataFiltersList.append(new DataFilter(strFilterName, strList));
            }
            break;
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_EVENT_DATA:
            if(m_OutputType == DataWizardOutputEnums::DATAWIZARD_OUTPUT_PDF)
            {
                strList.append(DATAWIZARD_DATAFILTER_EVENT_DATA_STR);
                m_DataFiltersList.append(new DataFilter(strFilterName, strList));
            }
            break;
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_USER_DATA:
            if(m_OutputType == DataWizardOutputEnums::DATAWIZARD_OUTPUT_PDF)
            {
                strList.append(DATAWIZARD_DATAFILTER_USER_DATA_STR);
                m_DataFiltersList.append(new DataFilter(strFilterName, strList));
            }
            break;
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_RECIPE_DATA:
            if(m_OutputType == DataWizardOutputEnums::DATAWIZARD_OUTPUT_PDF)
            {
                strList.append(DATAWIZARD_DATAFILTER_RECIPE_DATA_STR);
                m_DataFiltersList.append(new DataFilter(strFilterName, strList));
            }
            break;
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_WELD_GRAPH_DATA:
            if(m_OutputType == DataWizardOutputEnums::DATAWIZARD_OUTPUT_CSV)
            {
                strList.append(DATA_WIZARD_DATAFILTER_WELD_GRAPH_DATA);
                m_DataFiltersList.append(new DataFilter(strFilterName, strList));
            }
            break;
        case DataWizardDataFilters::DATAWIZARD_DATAFILTER_CSV_DELETE_DB:
            if(m_OutputType == DataWizardOutputEnums::DATAWIZARD_OUTPUT_CSV)
            {
                strList.append(DATA_WIZARD_DATAFILTER_DELETE_DB_RECORDS);
                m_DataFiltersList.append(new DataFilter(strFilterName, strList));
            }
            break;
        default:
            break;
        }

    }

    context->setContextProperty("datafiltermodelsr", QVariant::fromValue(m_DataFiltersList));
}

/**
 * @brief DataWizard::setSelectedReportType : Sets selected report type
 * @param SelectedReportType
 */
void DataWizard::setSelectedReportType(DataWizardEnums SelectedReportType)
{
    m_SelectedReportType = SelectedReportType;
    emit selectedReportTypeChanged();
}

/**
 * @brief DataWizard::getSelectedReportType : Returns selected report type
 * @return
 */
DataWizard::DataWizardEnums DataWizard::getSelectedReportType()
{
    return m_SelectedReportType;
}

/**
 * @brief DataWizard::getSelectedIndexString : Returns selected index
 * @param CurrentReportType
 * @return
 */
QString DataWizard::getSelectedIndexString(DataWizard::DataWizardEnums CurrentReportType)
{
    return m_DataWizardStrings.value(CurrentReportType);
}

/**
 * @brief DataWizard::setSelectedFrequencyType : Sets selected frequency
 * @param SelectedFrequencyType
 */
void DataWizard::setSelectedFrequencyType(DataWizardEnums SelectedFrequencyType)
{
    m_SelectedFrequencyType = SelectedFrequencyType;
    emit selectedFrequencyTypeChanged();
}

/**
 * @brief DataWizard::getSelectedFrequencyType : Returns selected frequency type
 * @return
 */
DataWizard::DataWizardEnums DataWizard::getSelectedFrequencyType()
{
    return m_SelectedFrequencyType;
}

/**
 * @brief DataWizard::getCurrentStatusofValues : Prints info about all selected parametres in single or continuous report
 */
void DataWizard::getCurrentStatusofValues()
{
}

/**
 * @brief DataWizard::getSelectedStringIndex : Returns selected stirng index
 * @param strSelectedString
 * @return
 */
int DataWizard::getSelectedStringIndex(QString strSelectedString)
{
    if(m_DataWizardStrings.values().contains(strSelectedString))
        return m_DataWizardStrings.key(strSelectedString);
    else
        return -1;
}

/**
 * @brief DataWizard::setSelectedFrequencyPeriodType : Sets selected frequency period type
 * @param SelectedFrequencyPeriodType
 */
void DataWizard::setSelectedFrequencyPeriodType(DataWizardEnums SelectedFrequencyPeriodType)
{
    m_SelectedFrequencyPeriodType = SelectedFrequencyPeriodType;
    emit selectedFrequencyPeriodTypeChanged();
}

/**
 * @brief DataWizard::getSelectedFrequencyPeriodType : Returns selected frequency period
 * @return
 */
DataWizard::DataWizardEnums DataWizard::getSelectedFrequencyPeriodType()
{
    return m_SelectedFrequencyPeriodType;
}

/**
 * @brief DataWizard::getSelectedFrequencyPeriodDate : Returns selected frequency period date
 * @return
 */
QDateTime DataWizard::getSelectedFrequencyPeriodDate()
{
    return m_FrequencyPeriodDate;
}

/**
 * @brief DataWizard::setSelectedFrequencyPeriodDate : Sets selected frequency period date
 * @param currenttime
 */
void DataWizard::setSelectedFrequencyPeriodDate(QDateTime currenttime)
{
    if(currenttime.isValid())
    {
        m_FrequencyPeriodDate = currenttime;
        emit selectedFrequencyPeriodDateChanged();
    }
}

/**
 * @brief DataWizard::getSelectedFrequencyPeriodTime : Returns selected frequency period time
 * @return
 */
QString DataWizard::getSelectedFrequencyPeriodTime()
{
    return m_FrequencyPeriodsTime;
}

/**
 * @brief DataWizard::setSelectedFrequencyPeriodTime : Sets selected frequency period time
 * @param currentTime
 */
void DataWizard::setSelectedFrequencyPeriodTime(QString currentTime)
{
    m_FrequencyPeriodsTime = currentTime;
    emit selectedFrequencyPeriodTimeChanged();

}

/**
 * @brief DataWizard::getSelectedOutputTime : Returns selected output time
 * @return
 */
QString DataWizard::getSelectedOutputTime()
{
    return m_OutputTime;
}

/**
 * @brief DataWizard::setSelectedOutputTime : Sets selected output time
 * @param str
 */
void DataWizard::setSelectedOutputTime(QString str)
{
    m_OutputTime = str;
    emit selectedOutputTimeChanged();
}

/**
 * @brief DataWizard::getSelectedCycles : Returns selected cycles
 * @return
 */
int DataWizard::getSelectedCycles()
{
    return m_Cycles;
}

/**
 * @brief DataWizard::setSelectedCycles : Sets selected cycles
 * @param CurrentCycle
 */
void DataWizard::setSelectedCycles(int CurrentCycle)
{
    m_Cycles = CurrentCycle;
    emit selectedCyclesChanged();

}

/**
 * @brief DataWizard::getCyclesText : Returns selected cycles text
 * @return
 */
QString DataWizard::getCyclesText()
{
    return m_NoOfcycles;
}

/**
 * @brief DataWizard::setCyclesText : Sets selected cycles text
 * @param cycles
 */
void DataWizard::setCyclesText(QString cycles)
{
    if(cycles != m_NoOfcycles)
        m_NoOfcycles = cycles;
    emit cyclesTextChanged();
}

/**
 * @brief DataWizard::getFrequencyDailyTime : Returns frequency daily time
 * @return
 */
QString DataWizard::getFrequencyDailyTime()
{
    return m_FrequencyDailyTime;
}

/**
 * @brief DataWizard::setFrequencyDailyTime : Sets frequency daily time
 * @param cycles
 */
void DataWizard::setFrequencyDailyTime(QString cycles)
{
    if(cycles != m_FrequencyDailyTime)
        m_FrequencyDailyTime = cycles;
    emit frequencyDailyTimeChanged();
}


////shifts
//QString DataWizard::getSelectedShifts()
//{
//    int count=0;
//    for(int i=0;i<3;i++)
//    {
//        if(shifts[i] == 1)
//            count++;
//    }
//    return QString::number(count);
//}
//void DataWizard::setSelectedShifts(int index,bool selected)
//{
//    if(selected == true)
//        shifts[index] = 1;
//    else
//        shifts[index] = 0;
//    emit selectedShiftsChanged();

//}

/**
 * @brief DataWizard::getcysclesContinousReport : Returns cycles continous report
 * @return
 */
QString DataWizard::getcysclesContinousReport()
{
    return cysclesContinousReport;
}

/**
 * @brief DataWizard::setcysclesContinousReport : Sets cycles continous report
 * @param str
 */
void DataWizard::setcysclesContinousReport(QString str){
    cysclesContinousReport = str;
}

/**
 * @brief DataWizard::getFilterName : Returns filter name
 * @param nIndex
 * @return
 */
QString DataWizard::getFilterName(int nIndex)
{
    QString strFilterName;
    switch(nIndex)
    {
    case DATAWIZARD_DATAFILTER_WELD_DATA:
        strFilterName=DATAWIZARD_DATAFILTER_WELD_DATA_STR;
        break;
    case DATAWIZARD_DATAFILTER_ALARM_DATA:
        strFilterName=DATAWIZARD_DATAFILTER_ALARM_DATA_STR;
        break;
    case DATAWIZARD_DATAFILTER_SYSTEMSETTINGS_DATA:
        strFilterName=DATAWIZARD_DATAFILTER_SYSTEMSETTINGS_DATA_STR;
        break;
    case DATAWIZARD_DATAFILTER_EVENT_DATA:
        strFilterName=DATAWIZARD_DATAFILTER_EVENT_DATA_STR;
        break;
    case DATAWIZARD_DATAFILTER_USER_DATA:
        strFilterName=DATAWIZARD_DATAFILTER_USER_DATA_STR;
        break;
    case DATAWIZARD_DATAFILTER_RECIPE_DATA:
        strFilterName = DATAWIZARD_DATAFILTER_RECIPE_DATA_STR;
        break;
    case DATAWIZARD_DATAFILTER_WELD_GRAPH_DATA:
        strFilterName = DATA_WIZARD_DATAFILTER_WELD_SIGNATURE;
        break;
    case DATAWIZARD_DATAFILTER_CSV_DELETE_DB:
        strFilterName = DATA_WIZARD_DATAFILTER_OTHER_OPTION;
        break;
    default:
        break;
    }
    return strFilterName;
}
