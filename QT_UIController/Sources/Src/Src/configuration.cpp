/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle configuration data

***************************************************************************/

#include<QString>
#include <QSettings>
#include <QQmlContext>
#include <QFile>
#include <QTranslator>
#include <QtWidgets/QApplication>
#include <QDebug>
#include "Header/configuration.h"
#include "Header/communicationinterface.h"
#include "Header/recipe.h"
#include "Header/hornrecipedata.h"
#include "Header/seekrecipedata.h"
#include "Header/ultrasonictest.h"
#include "Header/graphtableview.h"
#include "Header/dashboard.h"
#include "Header/common.h"
Configuration* Configuration::configPtr=NULL;
extern CommunicationInterface CIObj;

extern unsigned int psFreqGlobalVar;
extern Recipe *recipe;
extern QQmlContext* context;
extern HornRecipeData *obj_hornRecipeData;
extern SeekRecipeData *obj_seekRecipeData;
extern UltrasonicTest *obj_ultrasonictest;
extern checkbox *obj_checkbox;
extern int dwCurrentUnit;
extern QApplication *app;
extern QTranslator *app_translator;
extern GraphTableView *graphtableview;
extern DashBoard *dashboard;

/**
 * @brief Configuration::Configuration : Reads/writes IP address and port into config.ini
 *                                       and initializing SC,PC,AC,UIC versions
 */
Configuration::Configuration(): m_settings("config.ini", QSettings::IniFormat)
{
    m_dwActivePage=0;

    m_IPaddr= "192.168.1.100";
    m_Port="1600";
    m_SkipLogin=false;
    m_UserID="ADMIN";

    m_SCFV = "0.0.0";
    m_ACFV = "0.0.0";
    m_PCFV = "0.0.0";
    m_UICV = "0.11.0";
    m_DBV  = "0.0.0";
    m_nStatusDisplayTime = 30;

    auto ipAddress = getConfigValues("ipsettings", "IP");
    if (!ipAddress.isEmpty())
        m_IPaddr = ipAddress;
    else
        setConigValues("ipsettings", "IP", m_IPaddr);

    auto ipPort = getConfigValues("ipsettings", "PORT");
    if (!ipPort.isEmpty())
        m_Port = ipPort;
    else
        setConigValues("ipsettings", "PORT", m_Port);

    QString nTempDisplayTime = getConfigValues("Statusbar", "Displaytime");
    if(!nTempDisplayTime.isEmpty())
        m_nStatusDisplayTime = (nTempDisplayTime.toInt() * 1000);
    else
        setConigValues("Statusbar", "Displaytime", QString::number(m_nStatusDisplayTime));
    qstrListPrivateKeyFilenames.clear();
    qstrListPrivateKeyFilenames.append(NO_PRIVATE_KEY);
}

/**
 * @brief Configuration::~Configuration : Destructor
 */
Configuration::~Configuration()
{
    if(configPtr)
        delete configPtr;
}

/**
 * @brief Configuration::init : Reads runtime flag information, system configuration, stack info
 *                              and branson key details
 */
void Configuration::init()
{
    //readRuntimeFlagInformation();
    //readSysConfigData();
    SysConfigData::getInstance()->getBransonKeyDetails();
}

/**
 * @brief Configuration::getInstance : Singleton class initialization
 * @return
 */
Configuration *Configuration::getInstance(){

    if(!configPtr)
        configPtr=new Configuration();

    return configPtr;
}

/**
 * @brief Configuration::getIP : Returns IP address
 * @return
 */
QString Configuration::getIP()
{
    return m_IPaddr;
}

/**
 * @brief Configuration::getPort : Returns port address
 * @return
 */
QString Configuration::getPort()
{
    return m_Port;
}

/**
 * @brief Configuration::CheckDBVersion : Reads DB version from DB, checks and retuns the version
 * @return
 */
QString Configuration::CheckDBVersion()
{
    int isConnected = CIObj.sendMessage(REQ_GET_DB_VERSION,RES_GET_DB_VERSION,"");
    if(isConnected == 1)
        return "No Connection";
    QString qStrDBVersion="";
    CIObj.recivedMessage(RES_GET_DB_VERSION,qStrDBVersion);
    m_DBV = qStrDBVersion;
    return qStrDBVersion;
}

/**
 * @brief Configuration::readFirmwareVersion : Returns firmware version from BL
 */
void Configuration::readFirmwareVersion()
{
    char sendData[]="";
    CIObj.sendMessage(SCBL_FW_VERSION_REQ,UIC_FW_VERSION_RES,sendData,0);

    char msg[32]={'\0'};
    CIObj.recivedMessage(UIC_FW_VERSION_RES,msg,32);

    QString qstrVer = msg;

    if (!qstrVer.isEmpty())
    {
        QStringList qstrlstVersionList = qstrVer.split(",");
        if(qstrlstVersionList.size()%3 == 0)
        {
            m_SCFV = qstrlstVersionList.at(0);
            m_ACFV = qstrlstVersionList.at(1);
            m_PCFV = qstrlstVersionList.at(2);
        }
        else
        {
            m_SCFV = "0.0.0";
            m_ACFV = "0.0.0";
            m_PCFV = "0.0.0";
        }
    }
}

/**
 * @brief Configuration::getUICVersion : Returns UIC version
 * @return
 */
QString Configuration::getUICVersion()
{
    return m_UICV;
}

/**
 * @brief Configuration::getSCFirmwareVersion : Returns SC firmware version
 * @return
 */
QString Configuration::getSCFirmwareVersion()
{
    return m_SCFV;
}

/**
 * @brief Configuration::getACFirmwareVersion : Returns AC firmware version
 * @return
 */
QString Configuration::getACFirmwareVersion()
{
    return m_ACFV;
}

/**
 * @brief Configuration::getPCFirmwareVersion : Returns PC firmware version
 * @return
 */
QString Configuration::getPCFirmwareVersion()
{
    return m_PCFV;
}

/**
 * @brief Configuration::readSysConfigData : Reads System configuration from BL
 */
bool Configuration::readSysConfigData()
{
    char ch[4]={'\0'};
    m_systemData.bIsExternalPresets=false;
    m_systemData.bIsHornClamp = false;

    m_systemData.PcConfig.PsTypeIs=PS_TYPE_CHD;
    m_systemData.ActConfig.ActuatorTypeIs=ACTUATOR_TYPE_SERVO;
    m_systemData.PcConfig.PSWatt=800;
    m_systemData.LanguageIs=LANGUAGE_ENGLISH;
    m_systemData.PcConfig.ControlLevelIs=CONTROL_LEVEL_GSX_S;
    m_systemData.PcConfig.PSFreq=3;
    m_systemData.UnitIs=UNIT_METRIC;
    m_systemData.Sscreen=START_SCREEN_DASHBOARD;
    m_systemData.PsPowerUpOptionIs=PS_SCAN;
    m_systemData.AuthCheckLevel=AUTH_CHECK_DISABLED;
    m_systemData.MemFullActionIs=MEM_FULL_CONTINUE;
    m_systemData.bResetMem=true;
    m_systemData.bPowerUpMem=true;
    m_systemData.PresetRecipeBarcodeStart = 'R';
    m_systemData.bPartIDSwitchStatus = false;
    m_systemData.ActConfig.MaxStrokelength=125;
    if(getFeatureFlagStatus(DASHBOARD_SCREEN) == 1)
        m_systemData.Sscreen = START_SCREEN_DASHBOARD;
    else
        m_systemData.Sscreen = START_SCREEN_PRODUCTION;

    char *m_data=reinterpret_cast<char*>(&m_systemData);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_SYSCONFIG_READ_REQ,UIC_SYSCONFIG_READ_RES,ch,0);
    bool bReturnVal = CIObj.recivedMessage(UIC_SYSCONFIG_READ_RES,m_data,sizeof(m_systemData));

    switch(m_systemData.PcConfig.PSFreq)
    {
    case 0:
        psFreqGlobalVar = 20000;
        break;
    case 1:
        psFreqGlobalVar = 30000;
        break;
    case 2:
        psFreqGlobalVar = 40000;
        break;
    default:
        psFreqGlobalVar = 20000;
        m_systemData.PcConfig.PSFreq = 0;
    }
    changingUnits();
    m_ScreenActiveValue=m_systemData.Sscreen;
    return bReturnVal;
}
/**
 * @brief Configuration::changingUnits
 */
void Configuration::changingUnits()
{
    int units = m_systemData.UnitIs;
    dwCurrentUnit = m_systemData.UnitIs;
    recipe->setSystemConfigData(units,m_systemData.PcConfig.PSFreq);
    recipe->createAllModels();
    obj_checkbox->addcheckboxValue();
    obj_hornRecipeData->setDefaultSystemValuesForScan(units,psFreqGlobalVar);
    obj_seekRecipeData->setDefaultSystemValuesForSeek(units,psFreqGlobalVar);
    obj_ultrasonictest->setDefaultSystemValuesForTest(units,psFreqGlobalVar);
    graphtableview->initAnalytics();
    dashboard->FillLastWeldResult();
    m_ScreenActiveValue=m_systemData.Sscreen;
}

/**
 * @brief Configuration::readRuntimeFlagInformation : Reads runtime flag information from BL
 */
bool Configuration::readRuntimeFlagInformation()
{
    m_runTimeData.RtFeature1.Multimode_Flag_B0 = 0;
    m_runTimeData.RtFeature1.Time_Flag_B1 = 1;
    m_runTimeData.RtFeature1.Energy_Flag_B2 = 1;
    m_runTimeData.RtFeature1.PeakPower_Flag_B3 = 1;
    m_runTimeData.RtFeature1.CollapseDist_Flag_B4 = 1;
    m_runTimeData.RtFeature1.AbsoluteDist_Flag_B5 = 1;
    m_runTimeData.RtFeature1.GroundDetect_Flag_B6 = 1;
    m_runTimeData.RtFeature1.Continous_Flag_B7 = 0;
    m_runTimeData.RtFeature1.PreTrigger_Flag_B8 = 1;
    m_runTimeData.RtFeature1.AfterBurst_Flag_B9 = 1;
    m_runTimeData.RtFeature1.EnergyBrake_Flag_B10 = 0;
    m_runTimeData.RtFeature1.AmplitudeStep_Flag_B11 = 1;
    m_runTimeData.RtFeature1.ForceStep_Flag_B12 = 1;
    m_runTimeData.RtFeature1.AlarmLog_Flag_B13 = 1;
    m_runTimeData.RtFeature1.EventLog_Flag_B14 = 1;
    m_runTimeData.RtFeature1.WeldDataLog_Flag_B15 = 1;
    m_runTimeData.RtFeature1.Reporting_Flag_B16 = 0;
    m_runTimeData.RtFeature1.CrackedHorn_Flag_B17 = 0;
    m_runTimeData.RtFeature1.USB_Flag_B18 = 1;
    m_runTimeData.RtFeature1.Binary_Flag_B19 = 0;
    m_runTimeData.RtFeature1.WebServices_Flag_B20 = 0;
    m_runTimeData.RtFeature1.OPC_UA_FLAG_B21 = 0;
    m_runTimeData.RtFeature1.UserIO_Flag_B22 = 1;
    m_runTimeData.RtFeature1.ActuatorIO_Flag_B23 = 0;
    m_runTimeData.RtFeature1.CycleCounter_Flag_B24 = 0;
    m_runTimeData.RtFeature1.BatchCounter_Flag_B25 = 0;
    m_runTimeData.RtFeature1.PowerMatchCurve_Flag_B26 = 0;
    m_runTimeData.RtFeature1.SmartConverter_Flag_B27 = 0;
    m_runTimeData.RtFeature1.ControlSpeed_Flag_B28 = 0;
    m_runTimeData.RtFeature1.Sequencing_Flag_B29 = 0;
    m_runTimeData.RtFeature1.AutoConverterCooling_Flag_B30 = 0;
    m_runTimeData.RtFeature1.FieldBus_Flag_B31 = 0;
    m_runTimeData.RtFeature2.DashBoard_Flag_B32 = 1;
    m_runTimeData.RtFeature2.Password_Flag_B33 = 0;
    m_runTimeData.RtFeature2.Mobile_Flag_B34 = 0;
    m_runTimeData.RtFeature2.Overlay_Flag_B35 = 0;
    m_runTimeData.RtFeature2.Trends_Flag_B36 = 1;
    m_runTimeData.RtFeature2.Optitune_Flag_B37 = 0;
    m_runTimeData.RtFeature2.Limits_Flag_B38 = 0;
    m_runTimeData.RtFeature2.HDMode_Flag_B39 = 0;
    m_runTimeData.RtFeature2.TeachMode_Flag_B40 = 0;
    m_runTimeData.RtFeature2.AdvComm_Flag_B41 = 0;
    m_runTimeData.RtFeature2.ExtraDataStorage_Flag_B42 = 0;
    m_runTimeData.RtFeature2.TroubleShoot_Suggestion_Flag_B43 = 0;
    m_runTimeData.RtFeature2.CFR_Flag_B44 = 0;
    m_runTimeData.RtFeature2.HourCounter_Flag_B45 = 0;
    m_runTimeData.RtFeature2.Reporting_Flag_Csv_B46 = 1;
    m_runTimeData.RtFeature2.Reporting_Flag_Pdf_B47 = 0;
    m_runTimeData.RtFeature2.IODiagnostics_Flag_B48 = 0;
    m_runTimeData.RtFeature2.Dynamic_Flag_B49 = 0;
    m_runTimeData.RtFeature2.Reserved_B50 = 0;
    m_runTimeData.RtFeature2.Reserved_B51 = 0;

    char ch[4]={'\0'};
    char *m_data=reinterpret_cast<char*>(&m_runTimeData);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_FEATURERUN_READ_REQ,UIC_FEATURERUN_READ_RES,ch,0);
    bool bReturnVal = CIObj.recivedMessage(UIC_FEATURERUN_READ_RES,m_data,sizeof(m_runTimeData));
    return bReturnVal;
}

/**
 * @brief Configuration::getFeatureFlagStatus : Returns particular runtime flag status
 * @param dwIndex
 * @return
 */
int Configuration::getFeatureFlagStatus(int dwIndex)
{
    int dwRetFlag=0;
    switch(dwIndex)
    {
    case MULTI_MODE:
        dwRetFlag = m_runTimeData.RtFeature1.Multimode_Flag_B0;
        break;
    case MODE_TIME:
        dwRetFlag = m_runTimeData.RtFeature1.Time_Flag_B1;
        break;
    case MODE_ENERGY:
        dwRetFlag = m_runTimeData.RtFeature1.Energy_Flag_B2;
        break;
    case MODE_POWER:
        dwRetFlag = m_runTimeData.RtFeature1.PeakPower_Flag_B3;
        break;
    case MODE_COLLAPSE_DISTANCE:
        dwRetFlag = m_runTimeData.RtFeature1.CollapseDist_Flag_B4;
        break;
    case MODE_ABSOLUTE_DISTANCE:
        dwRetFlag = m_runTimeData.RtFeature1.AbsoluteDist_Flag_B5;
        break;
    case MODE_GROUND_DETECT:
        dwRetFlag = m_runTimeData.RtFeature1.GroundDetect_Flag_B6;
        break;
    case MODE_CONTINOUS:
        dwRetFlag = m_runTimeData.RtFeature1.Continous_Flag_B7;
        break;
    case PRE_TRIGGER:
        dwRetFlag = m_runTimeData.RtFeature1.PreTrigger_Flag_B8;
        break;
    case AFTER_BURST:
        dwRetFlag = m_runTimeData.RtFeature1.AfterBurst_Flag_B9;
        break;
    case ENERGY_BRAKE:
        dwRetFlag = m_runTimeData.RtFeature1.EnergyBrake_Flag_B10;
        break;
    case AMPLITUDE_STEP:
        dwRetFlag = m_runTimeData.RtFeature1.AmplitudeStep_Flag_B11;
        break;
    case FORCE_STEP:
        dwRetFlag = m_runTimeData.RtFeature1.ForceStep_Flag_B12;
        break;
    case ALARM_LOG:
        dwRetFlag = m_runTimeData.RtFeature1.AlarmLog_Flag_B13;
        break;
    case EVENT_LOG:
        dwRetFlag = m_runTimeData.RtFeature1.EventLog_Flag_B14;
        break;
    case WELD_LOG:
        dwRetFlag = m_runTimeData.RtFeature1.WeldDataLog_Flag_B15;
        break;
    case REPORTING:
        dwRetFlag = m_runTimeData.RtFeature1.Reporting_Flag_B16;
        break;
    case CRACKED_HORN:
        dwRetFlag = m_runTimeData.RtFeature1.CrackedHorn_Flag_B17;
        break;
    case USB:
        dwRetFlag = m_runTimeData.RtFeature1.USB_Flag_B18;
        break;
    case BINARY:
        dwRetFlag = m_runTimeData.RtFeature1.Binary_Flag_B19;
        break;
    case WEB_SERVICES:
        dwRetFlag = m_runTimeData.RtFeature1.WebServices_Flag_B20;
        break;
    case OPC_UA:
        dwRetFlag = m_runTimeData.RtFeature1.OPC_UA_FLAG_B21;
        break;
    case USER_IO:
        dwRetFlag = m_runTimeData.RtFeature1.UserIO_Flag_B22;
        break;
    case ACTUATOR_IO:
        dwRetFlag = m_runTimeData.RtFeature1.ActuatorIO_Flag_B23;
        break;
    case CYCLE_COUNTER:
        dwRetFlag = m_runTimeData.RtFeature1.CycleCounter_Flag_B24;
        break;
    case BATCH_COUNTER:
        dwRetFlag = m_runTimeData.RtFeature1.BatchCounter_Flag_B25;
        break;
    case POWER_MATCH_CURVE:
        dwRetFlag = m_runTimeData.RtFeature1.PowerMatchCurve_Flag_B26;
        break;
    case SMART_CONVERTER:
        dwRetFlag = m_runTimeData.RtFeature1.SmartConverter_Flag_B27;
        break;
    case CONTROL_SPEED:
        dwRetFlag = m_runTimeData.RtFeature1.ControlSpeed_Flag_B28;
        break;
    case SEQUENCING:
        dwRetFlag = m_runTimeData.RtFeature1.Sequencing_Flag_B29;
        break;
    case AUTO_CONVERTER_COOLING:
        dwRetFlag = m_runTimeData.RtFeature1.AutoConverterCooling_Flag_B30;
        break;
    case FIELD_BUS:
        dwRetFlag = m_runTimeData.RtFeature1.FieldBus_Flag_B31;
        break;
    case DASHBOARD_SCREEN:
        dwRetFlag = m_runTimeData.RtFeature2.DashBoard_Flag_B32;
        break;
    case PASSWORD_SCREEN:
        dwRetFlag = m_runTimeData.RtFeature2.Password_Flag_B33;
        break;
    case MOBILE:
        dwRetFlag = m_runTimeData.RtFeature2.Mobile_Flag_B34;
        break;
    case OVERLAY:
        dwRetFlag = m_runTimeData.RtFeature2.Overlay_Flag_B35;
        break;
    case TRENDS:
        dwRetFlag = m_runTimeData.RtFeature2.Trends_Flag_B36;
        break;
    case OPTITUNE:
        dwRetFlag = m_runTimeData.RtFeature2.Optitune_Flag_B37;
        break;
    case LIMITS:
        dwRetFlag = m_runTimeData.RtFeature2.Limits_Flag_B38;
        break;
    case HD_MODE:
        dwRetFlag = m_runTimeData.RtFeature2.HDMode_Flag_B39;
        break;
    case TEACH_MODE:
        dwRetFlag = m_runTimeData.RtFeature2.TeachMode_Flag_B40;
        break;
    case ADV_COMM:
        dwRetFlag = m_runTimeData.RtFeature2.AdvComm_Flag_B41;
        break;
    case EXTRA_DATA_STORAGE:
        dwRetFlag = m_runTimeData.RtFeature2.ExtraDataStorage_Flag_B42;
        break;
    case TOUBLE_SHOOT:
        dwRetFlag = m_runTimeData.RtFeature2.TroubleShoot_Suggestion_Flag_B43;
        break;
    case CFR:
        dwRetFlag = m_runTimeData.RtFeature2.CFR_Flag_B44;
        break;
    case HOUR_COUNTER:
        dwRetFlag = m_runTimeData.RtFeature2.HourCounter_Flag_B45;
        break;
    case CSV:
        dwRetFlag = m_runTimeData.RtFeature2.Reporting_Flag_Csv_B46;
        break;
    case PDF:
        dwRetFlag = m_runTimeData.RtFeature2.Reporting_Flag_Pdf_B47;
        break;
    case IODIAG:
        dwRetFlag = m_runTimeData.RtFeature2.IODiagnostics_Flag_B48;
        break;
    case MODE_DYNAMIC:
        dwRetFlag = m_runTimeData.RtFeature2.Dynamic_Flag_B49;
        break;
    case RESERVED_B50:
        dwRetFlag = m_runTimeData.RtFeature2.Reserved_B50;
        break;
    case RESERVED_B51:
        dwRetFlag = m_runTimeData.RtFeature2.Reserved_B51;
        break;

    default:
        dwRetFlag = 0;
        break;
    }
    return dwRetFlag;
}

/**
 * @brief Configuration::setEventTableOrder : Sets event table columns order into confi.ini
 * @param qstrOrder
 */
void Configuration::setEventTableOrder(QString qstrOrder)
{
    qstrOrder.remove(qstrOrder.length()-1,1);
    qstrOrder.insert(0,"\"");
    qstrOrder.insert(qstrOrder.length(),"\"");
    //m_EventTableOrder = "\"" + qstrOrder + "\"";

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("EventTableOrder");
    settings.setValue("ORDER",qstrOrder);
    settings.endGroup();
}

/**
 * @brief Configuration::getEventTableOrder : Returns event table columns order from confi.ini
 * @return
 */
QString Configuration::getEventTableOrder()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("EventTableOrder");
    QString value = settings.value("ORDER").toString();
    settings.endGroup();
    if(value.size()>1)
    {
        value.remove(value.length()-1,1);
        value.remove(0,1);
        return value;
    }
    return "";
}

/**
 * @brief Configuration::setAnalyticsTableOrder : Sets analytics table columns order into confi.ini
 * @param qstrOrder
 */
void Configuration::setAnalyticsTableOrder(QString qstrOrder)
{
    qstrOrder.remove(qstrOrder.length()-1,1);
    qstrOrder.insert(0,"\"");
    qstrOrder.insert(qstrOrder.length(),"\"");
    //m_EventTableOrder = "\"" + qstrOrder + "\"";

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("AnalyticsTableOrder");
    settings.setValue("ORDER",qstrOrder);
    settings.endGroup();
}

/**
 * @brief Configuration::getAnalyticsTableOrder : Returns analytics table columns order from confi.ini
 * @return
 */
QString Configuration::getAnalyticsTableOrder()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("AnalyticsTableOrder");
    QString value = settings.value("ORDER").toString();
    settings.endGroup();
    if(value.size()>1)
    {
        value.remove(value.length()-1,1);
        value.remove(0,1);
        return value;
    }
    return "";
}

/**
 * @brief Configuration::setActivePage : Sets active page index
 * @param dwPageIndex
 */
void Configuration::setActivePage(int dwPageIndex)
{
    m_dwActivePage = dwPageIndex;
}

/**
 * @brief Configuration::getActivePage :  Returns active page index
 * @return
 */
int Configuration::getActivePage()
{
    return m_dwActivePage;
}

/**
 * @brief Configuration::getActiveScreen : Returns which is active screen/ Startup screen
 * @param a_screenIndex
 * @return
 */
int Configuration::getActiveScreen(int a_screenIndex)
{
    switch (a_screenIndex) {
    case 0:
        m_systemData.Sscreen=START_SCREEN_DASHBOARD;
        return START_SCREEN_DASHBOARD;
        break;
    case 1:
        m_systemData.Sscreen=START_SCREEN_PRODUCTION;
        return START_SCREEN_PRODUCTION;
        break;
    case 2:
        m_systemData.Sscreen=START_SCREEN_RECIPES;
        return START_SCREEN_RECIPES;
        break;
    case 3:
        m_systemData.Sscreen=START_SCREEN_ANALYTICS;
        return START_SCREEN_ANALYTICS;
        break;
    default:
        return 0;
        break;
    }
}

/**
 * @brief Configuration::getScreenActiveValue : Returns index of active screen
 * @return
 */
unsigned int Configuration::getScreenActiveValue()
{
    return m_ScreenActiveValue;
}

/**
 * @brief Configuration::setScreenActiveValue : Sets index of active screen
 * @param a_ScreenActiveValue
 */
void Configuration::setScreenActiveValue(unsigned int a_ScreenActiveValue)
{
    this->m_ScreenActiveValue=a_ScreenActiveValue;
}

/**
 * @brief Configuration::setConigValues : Sets config.ini values
 * @param group
 * @param key
 * @param value
 */
void Configuration::setConigValues(const QString &group, const QString &key, const QString &value)
{
    m_settings.beginGroup(group);
    m_settings.setValue(key, value);
    m_settings.sync();
    m_settings.endGroup();
}

/**
 * @brief Configuration::getConfigValues : Returns config.ini values
 * @param group
 * @param key
 * @return
 */
QString Configuration::getConfigValues(const QString &group, const QString &key)
{
    m_settings.beginGroup(group);
    m_settings.sync();
    QString value = m_settings.value(key).toString();
    m_settings.endGroup();
    return  value;
}

/**
 * @brief Configuration::getStatusDisplayTime : Returns status of display time
 * @return
 */
int Configuration::getStatusDisplayTime()
{
    return m_nStatusDisplayTime;
}

/**
 * @brief Configuration::checkWebServices : Web services login status notifcation
 * @param
 * @return int
 */
int Configuration::checkWebServices()
{
    int status = 0;
    char requestMessage[] = "";
    char response[4]={'\0'};
    CIObj.sendMessage(SCBL_WEBSERVICES_LOGIN_REQ ,UIC_WEBSERVICES_LOGIN_RES,requestMessage,0);
    CIObj.recivedMessage(UIC_WEBSERVICES_LOGIN_RES,response,1);

    status = response[0]-48;
    return status;
}

/**
 * @brief Configuration::readPrivateKeyFiles : Read private key files from USB
 * @return int
 */
int Configuration::readPrivateKeyFiles()
{
    int dwReadUSb = 0,nFilesCounter;
    QString qstrReadusbResponse,qstrReadusbRequest;
    qstrListPrivateKeyFilenames.clear();
    /* Send message to BL */
    CIObj.sendMessage(SCBL_PRIVATE_KEY_REQ,UIC_PRIVATE_KEY_RES,qstrReadusbRequest);
    CIObj.recivedMessage(UIC_PRIVATE_KEY_RES,qstrReadusbResponse);

    /* Error case */
    if(qstrReadusbResponse.length() == 1)
    {
        dwReadUSb = qstrReadusbResponse.toInt();
        qstrListPrivateKeyFilenames.append(NO_PRIVATE_KEY);
    }
    else
    {       
        /* Read files from usb and assigns to lists */
        dwReadUSb = qstrReadusbResponse.length();
        if(dwReadUSb > 0)
        {
            QStringList lstAllReceivedFiles = qstrReadusbResponse.split(FILESEPERATOR, QString::SkipEmptyParts);
            for(nFilesCounter=0;nFilesCounter<lstAllReceivedFiles.count();nFilesCounter++)
            {
                qstrListPrivateKeyFilenames.append(lstAllReceivedFiles[nFilesCounter]);
            }
            /* end of all files for loop
             * Sorting of files and reverse order to show the latest file on top */

            qstrListPrivateKeyFilenames.sort();
            std::reverse(qstrListPrivateKeyFilenames.begin(), qstrListPrivateKeyFilenames.end());

            qstrListPrivateKeyFilenames.append(NO_PRIVATE_KEY);
        }
    }
    return dwReadUSb;
}

/**
 * @brief Configuration::getPrivateKeyFilesList : Gets private key files
 * @return  QStringList
 */
QStringList Configuration::getPrivateKeyFilesList()
{
    if(qstrListPrivateKeyFilenames.count() ==0)
        qstrListPrivateKeyFilenames.append(NO_PRIVATE_KEY);
    return qstrListPrivateKeyFilenames;
}

/**
 * @brief Configuration::clearPrivateKeyFilesList : clears the private key files
 */
void Configuration::clearPrivateKeyFilesList()
{
    qstrListPrivateKeyFilenames.clear();
}

/**
 * @brief Configuration::uploadAuthenticationKey : Sends authentication key to SC BL
 * @param index
 * @return int
 */
int Configuration::uploadAuthenticationKey(int index)
{
    int status;
    char *buff;
    char response[4]={'\0'};

    std::string fname = qstrListPrivateKeyFilenames.at(index).toStdString();
    buff = new char [fname.size()+1];
    strcpy( buff, fname.c_str() );

    CIObj.sendMessage(SCBL_PRIVATE_KEY_UPLOAD_REQ,UIC_PRIVATE_KEY_UPLOAD_RES,buff,strlen(buff));
    CIObj.recivedMessage(UIC_PRIVATE_KEY_UPLOAD_RES,response,1);

    status = response[0]-48;
    delete []buff;
    buff = NULL;
    return status;
}

/**
 * @brief Configuration::setHMILogonStatus : Sends HMI logon status to SC BL
 * @param status
 */
void Configuration::setHMILogonStatus(bool status)
{
    char buff;
    if(status == false)
        buff = (char)(NO_HMI);
    else if(status == true)
        buff = (char)(HMI_LOGIN);
    CIObj.sendMessage(SCBL_SET_HMI_LOGIN_REQ,SCBL_SET_HMI_LOGIN_REQ,&buff,1);
}

/**
 * @brief Configuration::convertSecToMsec : Converts seconds to milliseconds
 * @param time
 * @return
 */
double Configuration::convertSecToMsec(double time)
{
    double tmp;
    tmp=time*(double)1000;
    return tmp;
}

/**
 * @brief Configuration::convertMsecToSec : Converts milliseconds to seconds
 * @param time
 * @return
 */
double Configuration::convertMsecToSec(double time)
{
    double tmp;
    tmp=time/(double)1000;
    return tmp;
}

/**
 * @brief Configuration::convertMicrometreToMillimetre : Converts micrometer to millimeter
 * @param length
 * @return
 */
double Configuration::convertMicrometreToMillimetre(double length)
{
    double tmp;
    tmp=length/(double)1000;
    return tmp;
}

/**
 * @brief Configuration::convertMillimetreToMicrometre : Converts millimeter to micrometer
 * @param length
 * @return
 */
double Configuration::convertMillimetreToMicrometre(double length)
{
    double tmp;
    tmp=length*(double)1000;
    return tmp;
}

/**
 * @brief Configuration::convertinchestomicrometre : Converts inches to micrometer
 * @param dbInches
 * @return
 */
double Configuration::convertinchestomicrometre(double dbInches)
{
    double dbMicrometre;
    dbMicrometre = dbInches * (double)25400;
    return dbMicrometre;
}

/**
 * @brief Configuration::convertmicrometretoinches : Converts micrometer to inches
 * @param dbmicrometres
 * @return
 */
double Configuration::convertmicrometretoinches(double dbmicrometres)
{
    double dbInches;
    dbInches = dbmicrometres / (double)25400;
    return dbInches;
}

/**
 * @brief Configuration::convertMillimeterToInches : Converts millimeters to inches
 * @param dbMillimeters
 * @return
 */
double Configuration::convertMillimeterToInches(double dbMillimeters)
{
    double dbInches;
    dbInches = dbMillimeters / (double)25.4;
    return dbInches;
}

/**
 * @brief Configuration::getDatabaseVersion
 * @param
 * @return m_DBV
 */
QString Configuration::getDatabaseVersion()
{
    return m_DBV;
}

/**
 * @brief Configuration::convertInchesToMillimetre : Converts inches to millimeter
 * @param dbInches
 * @return
 */
double Configuration::convertInchesToMillimetre(double dbInches)
{
    double dbMillimetre;
    dbMillimetre = dbInches * (double)25.4;
    return dbMillimetre;
}

void Configuration::setSystemLanguage()
{
    SysConfigData *sysconfig = SysConfigData::getInstance();
    int nCurrentLanguageIndexId = sysconfig->getLanguage();
    int nActualLanguageId = sysconfig->getQTLanguageId(nCurrentLanguageIndexId);

    if (!app_translator->isEmpty())
        app->removeTranslator(app_translator);

    //QLocale::Language lRealLanguage1 = (QLocale::Language)nActualLanguageId;
    //qsTr should not be used to avaoid wrong language  translations
    QString strLanguageFile = app->applicationDirPath() + "/Languages/" +  app->applicationName() + "_" +  QLocale((QLocale::Language)nActualLanguageId).name() + ".qm";
    if( QFile::exists(strLanguageFile) )
    {
        if(app_translator->load(strLanguageFile))
        {
            app->installTranslator(app_translator);
        }
    }
    //recipe->createAllModels();
}

void Configuration::blockAltTab()
{
    RegisterHotKey(NULL,100,MOD_ALT,VK_TAB);
}

void Configuration::allowAltTab()
{
    UnregisterHotKey(NULL, 100);
}
