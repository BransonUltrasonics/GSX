/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Handels brason tabs

***************************************************************************/
#include "Header/bransondata.h"
#include "Header/configuration.h"
#include "Header/checkbox.h"
#include "Header/communicationinterface.h"

extern CommunicationInterface CIObj;

#define PARAM_NAME_TIME                      QObject::tr("Time")
#define PARAM_NAME_ENERGY                    QObject::tr("Energy")
#define PARAM_NAME_PEAK_POWER                QObject::tr("Peak Power")
#define PARAM_NAME_COLLAPSE_DISTANCE         QObject::tr("Collapse Distance")
#define PARAM_NAME_GROUND_DETECT             QObject::tr("Ground Detect")
#define PARAM_NAME_ABSOLUTE_DISTANCE         QObject::tr("Absolute Distance")
#define PARAM_NAME_DYNAMIC                   QObject::tr("Dynamic")
#define PARAM_NAME_CONTINUOUS                QObject::tr("Continuous")
#define PARAM_NAME_MULTI_MODE                QObject::tr("Multi Mode")
#define PARAM_NAME_DASHBOARD                 QObject::tr("DashBoard")
#define PARAM_NAME_OVERLAY                   QObject::tr("Overlay Graph")
#define PARAM_NAME_TRENDS                    QObject::tr("Trends Graph")
#define PARAM_NAME_PRE_TRIGGER               QObject::tr("Pretrigger")
#define PARAM_NAME_AFTER_BURST               QObject::tr("Afterburst")
#define PARAM_NAME_ENERGY_BRAKE              QObject::tr("Energy Brake")
#define PARAM_NAME_AMPLITUDE_STEP            QObject::tr("Amplitude Step")
#define PARAM_NAME_FORCE_STEP                QObject::tr("Force Step")
#define PARAM_NAME_EVENT_LOG                 QObject::tr("Event Log")
#define PARAM_NAME_ALARM_LOG                 QObject::tr("Alarm Log")
#define PARAM_NAME_WELD_DATA_LOG             QObject::tr("Weld Data Log")
#define PARAM_NAME_EXTERNAL_DATA_STORAGE     QObject::tr("External Data Storage")
#define PARAM_NAME_USB                       QObject::tr("USB")
#define PARAM_NAME_PDF                       QObject::tr("PDF")
#define PARAM_NAME_CSV                       QObject::tr("CSV")
#define PARAM_NAME_SMART_CONVERTOR           QObject::tr("Smart Convertor")
#define PARAM_NAME_USER_IO                   QObject::tr("User IO")
#define PARAM_NAME_IO_DIAGNOSTICS            QObject::tr("IO Diagnostics")
#define PARAM_NAME_TEACH_MODE                QObject::tr("Teach Mode")
#define PARAM_NAME_WEB_SERVICES              QObject::tr("Web Services")
extern Configuration *config;

/**
 * @brief BransonData::BransonData : Constructor
 * @param parent
 */
BransonData::BransonData(QObject *parent) : QObject(parent)
{
    savedStatus = 0;
    context->setContextProperty("screensModel", QVariant::fromValue(m_ParamListDummy));
    context->setContextProperty("weldModesModel", QVariant::fromValue(m_ParamListDummy));
    context->setContextProperty("cycleModifyModel", QVariant::fromValue(m_ParamListDummy));
    context->setContextProperty("loggingModel", QVariant::fromValue(m_ParamListDummy));
    context->setContextProperty("storageModel", QVariant::fromValue(m_ParamListDummy));
    context->setContextProperty("dataExtractionModel", QVariant::fromValue(m_ParamListDummy));
    context->setContextProperty("reportingModel", QVariant::fromValue(m_ParamListDummy));
    context->setContextProperty("extraParameterModel", QVariant::fromValue(m_ParamListDummy));

}

/**
 * @brief BransonData::~BransonData : Destructor
 */
BransonData::~BransonData()
{
}

/**
 * @brief SysConfigData::createBransonFeaturesModel : Creates branson features models
 */
void BransonData::createBransonFeaturesModel()
{
    createScreenModel();
    createWeldModeModel();
    createCycleModifyModel();
    createLoggingModel();
    createDataStorageModel();
    createDataExtractionModel();
    createReportingModel();
    createExtraParamModel();
}

/**
 * @brief BransonData::createScreenModel :  Creates Screens Model and updates in UI
 */
void BransonData::createScreenModel()
{
    /* Refreshing UI screens model */
    context->setContextProperty("screensModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_ScreensList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_ScreensList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_ScreensList.clear();
    QStringList screensList ;
    screensList.append(PARAM_NAME_DASHBOARD);
    //screensList.append(PARAM_NAME_OVERLAY);
    screensList.append(PARAM_NAME_TRENDS);

    for(int dwIndex=0;dwIndex<screensList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;
        ptrcheckBox->m_name = screensList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(screensList.at(dwIndex));
        m_ScreensList.append(ptrcheckBox);
    }
    /* Updating UI screens model */
    context->setContextProperty("screensModel", QVariant::fromValue(m_ScreensList));
}

/**
 * @brief BransonData::createWeldModeModel : Creates weld mode models and updates in UI
 */
void BransonData::createWeldModeModel()
{
    /* Refreshing UI weld modes model */
    context->setContextProperty("weldModesModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_WeldModeList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_WeldModeList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_WeldModeList.clear();
    QStringList weldModesList ;

    /*To handle availability of Weld Mode options in Branson Screen based on System Type*/
    if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        weldModesList.append(PARAM_NAME_TIME);
        weldModesList.append(PARAM_NAME_ENERGY);
        weldModesList.append(PARAM_NAME_COLLAPSE_DISTANCE);
        weldModesList.append(PARAM_NAME_ABSOLUTE_DISTANCE);
    }

    else
    {
        weldModesList.append(PARAM_NAME_TIME);
        weldModesList.append(PARAM_NAME_ENERGY);
        weldModesList.append(PARAM_NAME_PEAK_POWER);
        weldModesList.append(PARAM_NAME_COLLAPSE_DISTANCE);
        weldModesList.append(PARAM_NAME_ABSOLUTE_DISTANCE);
        weldModesList.append(PARAM_NAME_GROUND_DETECT);
        weldModesList.append(PARAM_NAME_DYNAMIC);
        //weldModesList.append(PARAM_NAME_CONTINUOUS);
        //weldModesList.append(PARAM_NAME_MULTI_MODE);
    }

    for(int dwIndex=0;dwIndex<weldModesList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;

        ptrcheckBox->m_name = weldModesList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(weldModesList.at(dwIndex));
        m_WeldModeList.append(ptrcheckBox);
    }

    /* Refreshing UI weld modes model */
    context->setContextProperty("weldModesModel", QVariant::fromValue(m_WeldModeList));
}

/**
 * @brief BransonData::createCycleModifyModel : Creates weld modecycle modify models and updates in UI
 */
void BransonData::createCycleModifyModel()
{
    /* Refreshing UI cycle modify model */
    context->setContextProperty("cycleModifyModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_CycleModifyList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_CycleModifyList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_CycleModifyList.clear();
    QStringList cycleModifyList;
    cycleModifyList.append(PARAM_NAME_PRE_TRIGGER);
    cycleModifyList.append(PARAM_NAME_AFTER_BURST);
    cycleModifyList.append(PARAM_NAME_ENERGY_BRAKE);
    cycleModifyList.append(PARAM_NAME_AMPLITUDE_STEP);
    cycleModifyList.append(PARAM_NAME_FORCE_STEP);

    for(int dwIndex=0;dwIndex<cycleModifyList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;
        ptrcheckBox->m_name = cycleModifyList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(cycleModifyList.at(dwIndex));
        m_CycleModifyList.append(ptrcheckBox);
    }

    /* Refreshing UI cycle modify model */
    context->setContextProperty("cycleModifyModel", QVariant::fromValue(m_CycleModifyList));
}

/**
 * @brief BransonData::createLoggingModel  : Creates logging models and updates in UI
 */
void BransonData::createLoggingModel()
{
    /* Refreshing UI logging model */
    context->setContextProperty("loggingModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_LoggingList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_LoggingList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_LoggingList.clear();
    QStringList loggingStrList;
    loggingStrList.append(PARAM_NAME_EVENT_LOG);
    loggingStrList.append(PARAM_NAME_ALARM_LOG);
    loggingStrList.append(PARAM_NAME_WELD_DATA_LOG);

    for(int dwIndex=0;dwIndex<loggingStrList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;

        ptrcheckBox->m_name = loggingStrList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(loggingStrList.at(dwIndex));
        m_LoggingList.append(ptrcheckBox);
    }

    /* Refreshing UI logging model */
    context->setContextProperty("loggingModel", QVariant::fromValue(m_LoggingList));
}

/**
 * @brief BransonData::createDataStorageModel : Creates storage models and updates in UI
 */
void BransonData::createDataStorageModel()
{
    /* Refreshing UI storage model */
    context->setContextProperty("storageModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_StorageList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_StorageList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_StorageList.clear();
    QStringList storageStrList;
    storageStrList.append(PARAM_NAME_EXTERNAL_DATA_STORAGE);
    for(int dwIndex=0;dwIndex<storageStrList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;

        ptrcheckBox->m_name = storageStrList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(storageStrList.at(dwIndex));
        m_StorageList.append(ptrcheckBox);
    }

    /* Refreshing UI storage model */
    context->setContextProperty("storageModel", QVariant::fromValue(m_StorageList));
}

/**
 * @brief BransonData::createDataExtractionModel : Creates data extraction models and updates in UI
 */
void BransonData::createDataExtractionModel()
{
    /* Refreshing UI data extraction model */
    context->setContextProperty("dataExtractionModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_DataExtractList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_DataExtractList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_DataExtractList.clear();
    QStringList dataExtractionList;
    dataExtractionList.append(PARAM_NAME_USB);

    for(int dwIndex=0;dwIndex<dataExtractionList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;

        ptrcheckBox->m_name = dataExtractionList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(dataExtractionList.at(dwIndex));
        m_DataExtractList.append(ptrcheckBox);
    }

    /* Refreshing UI data extraction model */
    context->setContextProperty("dataExtractionModel", QVariant::fromValue(m_DataExtractList));
}

/**
 * @brief BransonData::createReportingModel : Creates reporting models and updates in UI
 */
void BransonData::createReportingModel()
{
    /* Refreshing UI reporting model */
    context->setContextProperty("reportingModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_ReportingList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_ReportingList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_ReportingList.clear();
    QStringList reportingStrList;
    reportingStrList.append(PARAM_NAME_CSV);
    //reportingStrList.append(PARAM_NAME_PDF);

    for(int dwIndex=0;dwIndex<reportingStrList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;
        ptrcheckBox->m_name = reportingStrList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(reportingStrList.at(dwIndex));
        m_ReportingList.append(ptrcheckBox);
    }

    /* Refreshing UI reporting model */
    context->setContextProperty("reportingModel", QVariant::fromValue(m_ReportingList));
}

/**
 * @brief BransonData::createExtraParamModel : Creates extra parameter models and updates in UI
 */
void BransonData::createExtraParamModel()
{
    /* Refreshing UI extra parameter model */
    context->setContextProperty("extraParameterModel", QVariant::fromValue(m_ParamListDummy));

    for(int element=0; element <m_ParamList.size() ; element++)
    {
        checkbox *ptrCheckbox = dynamic_cast<checkbox*>(m_ParamList.at(element));
        delete ptrCheckbox;
        ptrCheckbox = NULL;
    }

    m_ParamList.clear();
    QStringList paramStrList;
    //paramStrList.append(PARAM_NAME_SMART_CONVERTOR);
    paramStrList.append(PARAM_NAME_USER_IO);
    paramStrList.append(PARAM_NAME_WEB_SERVICES);
    //paramStrList.append(PARAM_NAME_IO_DIAGNOSTICS);
    //paramStrList.append(PARAM_NAME_TEACH_MODE);

    for(int dwIndex=0;dwIndex<paramStrList.size();dwIndex++)
    {
        checkbox *ptrcheckBox = new checkbox;
        ptrcheckBox->m_name = paramStrList.at(dwIndex);
        ptrcheckBox->m_isCheckBoxSelected = getCheckBoxStatus(paramStrList.at(dwIndex));
        m_ParamList.append(ptrcheckBox);
    }

    /* Refreshing UI extra parameter model */
    context->setContextProperty("extraParameterModel", QVariant::fromValue(m_ParamList));
}

/**
 * @brief BransonData::getCheckBoxStatus : Returns check box state
 * @param : QString paramName
 * @return : bool
 */
bool BransonData::getCheckBoxStatus(QString paramName)
{
    int BitFieldValue;
    bool checkBoxStatus;
    if(paramName == PARAM_NAME_DASHBOARD)
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.DashBoard_Flag_B32;

    else if(paramName == PARAM_NAME_OVERLAY)
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.Overlay_Flag_B35;

    else if(paramName == PARAM_NAME_TRENDS )
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.Trends_Flag_B36;

    else if(paramName == PARAM_NAME_TIME )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.Time_Flag_B1;

    else if(paramName == PARAM_NAME_ENERGY )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.Energy_Flag_B2;

    else if(paramName == PARAM_NAME_PEAK_POWER )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.PeakPower_Flag_B3;

    else if(paramName == PARAM_NAME_COLLAPSE_DISTANCE )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.CollapseDist_Flag_B4;

    else if(paramName == PARAM_NAME_GROUND_DETECT )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.GroundDetect_Flag_B6;

    else if(paramName == PARAM_NAME_ABSOLUTE_DISTANCE )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.AbsoluteDist_Flag_B5;

    else if(paramName == PARAM_NAME_CONTINUOUS )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.Continous_Flag_B7;

    else if(paramName == PARAM_NAME_MULTI_MODE )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.Multimode_Flag_B0;

    else if(paramName == PARAM_NAME_PRE_TRIGGER )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.PreTrigger_Flag_B8;

    else if(paramName == PARAM_NAME_AFTER_BURST )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.AfterBurst_Flag_B9;

    else if(paramName == PARAM_NAME_ENERGY_BRAKE )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.EnergyBrake_Flag_B10;

    else if(paramName == PARAM_NAME_AMPLITUDE_STEP )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.AmplitudeStep_Flag_B11;

    else if(paramName == PARAM_NAME_FORCE_STEP )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.ForceStep_Flag_B12;

    else if(paramName == PARAM_NAME_EVENT_LOG )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.EventLog_Flag_B14;

    else if(paramName == PARAM_NAME_ALARM_LOG )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.AlarmLog_Flag_B13;

    else if(paramName == PARAM_NAME_WELD_DATA_LOG )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.WeldDataLog_Flag_B15;

    else if(paramName == PARAM_NAME_EXTERNAL_DATA_STORAGE )
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.ExtraDataStorage_Flag_B42;

    else if(paramName == PARAM_NAME_USB )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.USB_Flag_B18;

    else if(paramName == PARAM_NAME_PDF )
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.Reporting_Flag_Pdf_B47;

    else if(paramName == PARAM_NAME_CSV )
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.Reporting_Flag_Csv_B46;

    else if(paramName == PARAM_NAME_SMART_CONVERTOR )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.SmartConverter_Flag_B27;

    else if(paramName == PARAM_NAME_USER_IO )
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.UserIO_Flag_B22;

    else if(paramName == PARAM_NAME_IO_DIAGNOSTICS )
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.IODiagnostics_Flag_B48;

    else if(paramName == PARAM_NAME_TEACH_MODE )
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.TeachMode_Flag_B40;

    else if(paramName == PARAM_NAME_WEB_SERVICES)
        BitFieldValue = m_runTimeFeatureStr.RtFeature1.WebServices_Flag_B20;

    else if(paramName == PARAM_NAME_DYNAMIC)
        BitFieldValue = m_runTimeFeatureStr.RtFeature2.Dynamic_Flag_B49;
    else
        BitFieldValue = 0;

    if (BitFieldValue != 1)
        checkBoxStatus = 0;
    else
        checkBoxStatus = BitFieldValue;

    return checkBoxStatus;
}

/**
 * @brief BransonData::updateScreensCheckboxStatus : Update screens checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateScreensCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
        m_runTimeFeatureStr.RtFeature2.DashBoard_Flag_B32 =  status;
        break;
//    case 1:
//        m_runTimeFeatureStr.RtFeature2.Overlay_Flag_B35 =  status;
//        break;
    case 1:
        m_runTimeFeatureStr.RtFeature2.Trends_Flag_B36 =  status;
        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::updateWeldModeCheckboxStatus : Update weld mode checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateWeldModeCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
        m_runTimeFeatureStr.RtFeature1.Time_Flag_B1 =  status;
        break;
    case 1:
        m_runTimeFeatureStr.RtFeature1.Energy_Flag_B2 =  status;
        break;
    case 2:
        m_runTimeFeatureStr.RtFeature1.PeakPower_Flag_B3 =  status;
        break;
    case 3:
        m_runTimeFeatureStr.RtFeature1.CollapseDist_Flag_B4 =  status;
        break;
    case 4:
        m_runTimeFeatureStr.RtFeature1.AbsoluteDist_Flag_B5 =  status;
        break;
    case 5:
        m_runTimeFeatureStr.RtFeature1.GroundDetect_Flag_B6 =  status;
        break;
    case 6:
        m_runTimeFeatureStr.RtFeature2.Dynamic_Flag_B49 =  status;
        break;
//    case 6:
//        m_runTimeFeatureStr.RtFeature1.Continous_Flag_B7 =  status;
//        break;
//    case 7:
//        m_runTimeFeatureStr.RtFeature1.Multimode_Flag_B0 =  status;
//        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::updateCycleModifyCheckboxStatus : Update cycle modify checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateCycleModifyCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
        m_runTimeFeatureStr.RtFeature1.PreTrigger_Flag_B8 =  status;
        break;
    case 1:
        m_runTimeFeatureStr.RtFeature1.AfterBurst_Flag_B9 =  status;
        break;
    case 2:
        m_runTimeFeatureStr.RtFeature1.EnergyBrake_Flag_B10 =  status;
        break;
    case 3:
        m_runTimeFeatureStr.RtFeature1.AmplitudeStep_Flag_B11 =  status;
        break;
    case 4:
        m_runTimeFeatureStr.RtFeature1.ForceStep_Flag_B12 =  status;
        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::updateLoggingCheckboxStatus : Update logging checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateLoggingCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
        m_runTimeFeatureStr.RtFeature1.EventLog_Flag_B14 =  status;
        break;
    case 1:
        m_runTimeFeatureStr.RtFeature1.AlarmLog_Flag_B13 =  status;
        break;
    case 2:
        m_runTimeFeatureStr.RtFeature1.WeldDataLog_Flag_B15 =  status;
        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::updateStorageCheckboxStatus : Update storage checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateStorageCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
        m_runTimeFeatureStr.RtFeature2.ExtraDataStorage_Flag_B42 =  status;
        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::updateExtractionCheckboxStatus : Update extraction checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateExtractionCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
        m_runTimeFeatureStr.RtFeature1.USB_Flag_B18 =  status;
        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::updateReportingCheckboxStatus : Update reporting checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateReportingCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
        m_runTimeFeatureStr.RtFeature2.Reporting_Flag_Csv_B46 =  status;
        break;
//    case 1:
//        m_runTimeFeatureStr.RtFeature2.Reporting_Flag_Pdf_B47 =  status;
//        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::updateExtraParamCheckboxStatus : Update extra param checkbox state in structure
 * @param : int index
 * @param : bool status
 */
void BransonData::updateExtraParamCheckboxStatus(int index, bool status)
{
    switch (index)
    {
    case 0:
//        m_runTimeFeatureStr.RtFeature1.SmartConverter_Flag_B27 =  status;
//        break;
        m_runTimeFeatureStr.RtFeature1.UserIO_Flag_B22 =  status;
        break;
    case 1:
        m_runTimeFeatureStr.RtFeature1.WebServices_Flag_B20 =  status;
        break;
//    case 2:
//        m_runTimeFeatureStr.RtFeature2.IODiagnostics_Flag_B48 =  status;
//        break;
//    case 3:
//        m_runTimeFeatureStr.RtFeature2.TeachMode_Flag_B40  =  status;
//        break;
    default:
        break;
    }
}

/**
 * @brief BransonData::saveRunTimeFeatures
 * @return
 */
bool BransonData::saveRunTimeFeatures()
{
    savedStatus = 1;
    cancelRTF = m_runTimeFeatureStr;
    char ch[4]={'\0'};
    char m_saveResult;

    char *data = reinterpret_cast<char*>(&m_runTimeFeatureStr);
    CIObj.sendMessage(SCBL_FEATURERUN_WRITE_REQ,UIC_FEATURERUN_WRITE_RES,data,sizeof(m_runTimeFeatureStr));
    CIObj.recivedMessage(UIC_FEATURERUN_WRITE_RES,ch,1);

    createBransonFeaturesModel();

    memcpy(&m_saveResult,ch,1);
    if(m_saveResult == '1')
        return true;
    return false;

}

/**
 * @brief BransonData::cancelCurrentChanges : Cancells all the current chanegs and sets to default
 */
void BransonData::cancelCurrentChanges()
{
    //m_runTimeFeatureStr = config->m_runTimeData;
    if(savedStatus)
    {
        m_runTimeFeatureStr = cancelRTF;
    }
    else
    {
        m_runTimeFeatureStr = tempRTFStrObj;
    }
    createBransonFeaturesModel();
}

/**
 * @brief BransonData::resetDefaultsValues : Resets to default when clicked on reset
 */
void BransonData::resetDefaultsValues()
{
    char ch[4]={'\0'};

    char *m_data=reinterpret_cast<char*>(&m_runTimeFeatureStr);
    CIObj.sendMessage(SCBL_DEFAULT_FEATURERUN_REQ,UIC_DEFAULT_FEATURERUN_RES,ch,0);
    CIObj.recivedMessage(UIC_DEFAULT_FEATURERUN_RES,m_data,sizeof(m_runTimeFeatureStr));


    createBransonFeaturesModel();
}

/**
 * @brief BransonData::readRunTimeFeatureData
 */
void BransonData:: readRunTimeFeatureData()
{
    char ch[4]={'\0'};
    char *m_data=reinterpret_cast<char*>(&m_runTimeFeatureStr);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_FEATURERUN_READ_REQ,UIC_FEATURERUN_READ_RES,ch,0);
    CIObj.recivedMessage(UIC_FEATURERUN_READ_RES,m_data,sizeof(m_runTimeFeatureStr));

    tempRTFStrObj = m_runTimeFeatureStr;
    createBransonFeaturesModel();
}

/**
 * @brief BransonData::saveWebServiceOptions
 * @param a_webServiceStaus
 * @param a_foeStatus
 */
void BransonData::saveWebServiceOptions(bool a_webServiceStaus,bool a_foeStatus)
{
    QString qstrStoreBuff;
    qstrStoreBuff.append(QString::number(a_webServiceStaus));
    qstrStoreBuff.append(",");
    qstrStoreBuff.append(QString::number(a_foeStatus));
    QString qstrRecipeBuffer = "";
    CIObj.sendMessage(REQ_SAVE_WEBSERVICES_DATA,RES_SAVE_WEBSERVICES_DATA,qstrStoreBuff);
    CIObj.recivedMessage(RES_SAVE_WEBSERVICES_DATA,qstrRecipeBuffer);
}

/**
 * @brief BransonData::getWebServiceOptions
 */
void BransonData::getWebServiceOptions()
{
    QString qstrReceived = "";
    QStringList qstrListReceived;
    CIObj.sendMessage(REQ_GET_WEBSERVICES_DATA,RES_GET_WEBSERVICES_DATA,"");
    CIObj.recivedMessage(RES_GET_WEBSERVICES_DATA,qstrReceived);
    qstrListReceived = qstrReceived.split(",");
    if(qstrListReceived.size() == 2)
    {
        bIsWebServices = qstrListReceived[0].toInt();
        bIsFOE = qstrListReceived[1].toInt();
    }
    else
    {
        bIsWebServices = false;
        bIsFOE = false;
    }
}

/**
 * @brief BransonData::getWebServiceStatus : Returns web service status
 * @return bool
 */
bool BransonData::getWebServiceStatus()
{
    return bIsWebServices;
}

/**
 * @brief BransonData::getFOEStatus : Returns FOE status
 * @return bool
 */
bool BransonData::getFOEStatus()
{
    return bIsFOE;
}
