/**************************************************************************

       Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

System Configuration

***************************************************************************/
#include <QQmlContext>
#include <QObject>
#include "Header/configuration.h"
#include "Header/sysconfigdata.h"
#include "Header/systemconfiggeneral.h"
#include "Header/setupparameter.h"
#include "Header/checkbox.h"
#include "Header/eventhandler.h"
#include "Header/datawizard.h"

extern QObject * object;
extern  EventHandler Eventlogger;
extern CommunicationInterface CIObj;
SysConfigData* SysConfigData::sysConfigPtr = nullptr;

/**
 * @brief SysConfigData::generateReport : Sending structure to BL
 * @param index
 * @param outputFormat
 */
void SysConfigData::generateReport(int index, int subIndex, int outputFormat,int deleteID)
{
    welddataextraction dataextraction;
    if(outputFormat == DataWizard::DATAWIZARD_OUTPUT_CSV)
    {
        if(subIndex == 1)
        {
            dataextraction.reportId = DataWizard::DATAWIZARD_DATAFILTER_RESULT_AND_SIGNATURE;
        }
        else
        {
            if(index == 0)
                dataextraction.reportId = DataWizard::DATAWIZARD_DATAFILTER_WELD_DATA;
            else
                dataextraction.reportId = DataWizard::DATAWIZARD_DATAFILTER_WELD_GRAPH_DATA;
        }
    }
    else {
        dataextraction.reportId = static_cast<UINT8>(index);
    }
    dataextraction.outputFormat = static_cast<UINT8>(outputFormat);
    dataextraction.deleteID = static_cast<UINT8>(deleteID);
    strcpy(dataextraction.storageType, "USB") ;

    char *data = reinterpret_cast<char*>(&dataextraction);
    CIObj.sendMessage(SCBL_GEN_SINGLE_REPORT_REQ, SCBL_GEN_SINGLE_REPORT_RSP, data, sizeof(dataextraction));
}

void SysConfigData::startVacuum()
{
    char data[] = "";
    CIObj.sendMessage(SCBL_OPTIMIZE_DATABASE_REQ, UIC_OPTIMIZE_DATABASE_RESP, data, 0);
}

/**
 * @brief SysConfigData::sendReportAction : Indication to BL
 * @param status
 */
void SysConfigData::sendReportAction(QString status)
{
    CIObj.sendMessage(SCBL_GEN_REPORT_ACTION_IND,SCBL_GEN_REPORT_ACTION_IND,status);
}

/**
 * @brief SysConfigData::getReportRes
 * @return
 */
int SysConfigData::getReportRes() const
{
    return m_reportRes;
}

/**
 * @brief SysConfigData::getVacuumRes
 * @return
 */
QString SysConfigData::getVacuumRes() const
{
    return m_vacuumRes;
}

/**
 * @brief SysConfigData::setReportRes
 * @param reportRes
 */
void SysConfigData::setReportRes(int reportRes)
{
    if(m_reportRes != reportRes)
    {
        m_reportRes = reportRes;
        emit responseResChaned();
    }
}

/**
 * @brief SysConfigData::setVacuumRes
 * @param reportRes
 */
void SysConfigData::setVacuumRes(QString reportRes)
{
    if(m_vacuumRes != reportRes)
    {
        m_vacuumRes = reportRes;
        emit vacuumResChaned();
    }
}

/**
 * @brief SysConfigData::getBransonKeyGlobalFlag
 * @return
 */
bool SysConfigData::getBransonKeyGlobalFlag()
{
    return m_bransonKeyGlobalFlag;
}

/**
 * @brief SysConfigData::setBransonKeyGlobalFlag
 * @param flag
 */
void SysConfigData::setBransonKeyGlobalFlag(bool flag)
{
    if(m_bransonKeyGlobalFlag != flag)
    {
        m_bransonKeyGlobalFlag = flag;
        emit bransonKeyGlobalFlagChanged();
    }
}

/**
 * @brief SysConfigData::getBransonSecondLevelKeyFlag
 * @return
 */
bool SysConfigData::getBransonSecondLevelKeyFlag() const
{
    return m_bransonSecondLevelKeyFlag;
}

/**
 * @brief SysConfigData::setBransonSecondLevelKeyFlag : Checking second level key and notify
 * @param bransonSecondLevelKeyFlag
 */
void SysConfigData::setBransonSecondLevelKeyFlag(bool bransonSecondLevelKeyFlag)
{
    if(m_bransonSecondLevelKeyFlag != bransonSecondLevelKeyFlag)
    {
        m_bransonSecondLevelKeyFlag = bransonSecondLevelKeyFlag;
        emit bransonSecondLevelFlagChanged();
    }
}

/**
 * @brief SysConfigData::getDbmemoryStatus
 * @return
 */
int SysConfigData::getDbmemoryStatus() const
{
    return m_dbmemoryStatus;
}

/**
 * @brief SysConfigData::setDbmemoryStatus
 * @param dbmemoryStatus
 */
void SysConfigData::setDbmemoryStatus(int dbmemoryStatus)
{
    if(m_dbmemoryStatus != dbmemoryStatus)
    {
        m_dbmemoryStatus = dbmemoryStatus;
        emit dbStatusChanged();
    }
}

/**
 * @brief SysConfigData::SysConfigData : This Constructor send request to Server to retrive system configuration data.
 * @param parent :  used to initialize parent constructor which needs QObject *
 */
SysConfigData::SysConfigData(QObject *parent) : QObject(parent)
{
    //    m_NewPassword.setUserID("ADMIN");
    m_PartContactDist = "";
    m_WaitingStatus="";
    //setDefaultValues();
    m_bransonKeyGlobalFlag = false;
    m_bransonSecondLevelKeyFlag = false;
    m_GlobalCheckFlag = false;
    serviceParameterObj.boardReplaced = false;
    serviceParameterObj.systemCleaned = false;
    QObject::connect(&CIObj , &CommunicationInterface::copyDBRes , this ,&SysConfigData :: copyDBToUSBRes);
}

/**
 * @brief SysConfigData::Init
 */
void SysConfigData::Init()
{
    //context->setContextProperty("m_NewPassword",QVariant::fromValue(&m_NewPassword));
    setSysConfigData();

    if (1)
    {
       m_EMMCUsage = ReadEMMCWareUsage();
    }
}

/**
 * @brief SysConfigData::getSystemConfigGeneralParam
 * @return
 */
QString SysConfigData::getSystemConfigGeneralParam()
{
    QString systemConfigGeneralParam;

    systemConfigGeneralParam =  SYS_CONFIG_MEMORY_FULL_ACTION + "," +
            SYS_CONFIG_AUTHORITY_CHECK + "," +
            SYS_CONFIG_LANGUAGE + "," +
//            SYS_CONFIG_UNITS+ "," +
            SYS_CONFIG_START_SCREEN+ "," +
            SYS_CONFIG_PS_POWER_ON_OPTION+ "," +
            SYS_CONFIG_BARCODE_RECIPE_PREFIX+ "," +
            SYS_CONFIG_MACHINE_NAME+ "," +
            //SYS_CONFIG_HORN_CLAMP+ "," +
            //SYS_CONFIG_RESET_MEMORY+ "," +
            //SYS_CONFIG_POWERUP_MEMORY+ "," +
            SYS_CONFIG_PARTID_STATUS;//+ "," +
            //SYS_CONFIG_EXTRA_COOLING;//+ "," +
            //SYS_CONFIG_EXTERNAL_PRESETS;

    return systemConfigGeneralParam;
}

/**
 * @brief SysConfigData::createSystemConfigGeneralParamList
 */
void SysConfigData::createSystemConfigGeneralParamList()
{
    initializeComboxModelsName();

    context->setContextProperty("systemConfigGeneralParams", QVariant::fromValue(m_listdummy));

    for(int element=0; element <m_listSysConfigGen.size() ; element++)
    {
        SystemConfigGeneral *ptrSysConfigGenParam = dynamic_cast<SystemConfigGeneral*>(m_listSysConfigGen.at(element));
        delete ptrSysConfigGenParam;
        ptrSysConfigGenParam = NULL;
    }
    m_listSysConfigGen.clear();
    QString sysConfigGenParams = getSystemConfigGeneralParam();
    QStringList sysConfigGenParamsList =sysConfigGenParams.split(",");
    for(int dwIndex=0;dwIndex<sysConfigGenParamsList.size();dwIndex++)
    {
        SystemConfigGeneral *ptrSys = new SystemConfigGeneral;
        ptrSys->m_ParameterName = sysConfigGenParamsList.at(dwIndex);

        if((ptrSys->m_ParameterName == SYS_CONFIG_EXTRA_COOLING)|| (ptrSys->m_ParameterName == SYS_CONFIG_EXTERNAL_PRESETS)|| (ptrSys->m_ParameterName == SYS_CONFIG_HORN_CLAMP) || (ptrSys->m_ParameterName == SYS_CONFIG_PARTID_STATUS) || (ptrSys->m_ParameterName == SYS_CONFIG_RESET_MEMORY) || (ptrSys->m_ParameterName == SYS_CONFIG_POWERUP_MEMORY))
        {
            ptrSys->m_ParameterSwitch = true;
            ptrSys->m_ParameterSwitchState=getStatusOfSwitch(ptrSys->m_ParameterName);
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_ParameterComboBox = false;
        }
        else if((ptrSys->m_ParameterName ==SYS_CONFIG_MEMORY_FULL_ACTION)  ||
                (ptrSys->m_ParameterName ==SYS_CONFIG_AUTHORITY_CHECK) ||
                (ptrSys->m_ParameterName == SYS_CONFIG_LANGUAGE) ||
                (ptrSys->m_ParameterName == SYS_CONFIG_UNITS) ||
                (ptrSys->m_ParameterName ==SYS_CONFIG_START_SCREEN) ||
                (ptrSys->m_ParameterName == SYS_CONFIG_PS_POWER_ON_OPTION))
        {
            ptrSys->m_ParameterComboBox = true;
            ptrSys->m_ComboBoxModelName=getCurrentNameOfComboBox(ptrSys->m_ParameterName);
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_ParameterSwitch = false;
        }
        else if((ptrSys->m_ParameterName == SYS_CONFIG_MACHINE_NAME)||
                (ptrSys->m_ParameterName == SYS_CONFIG_BARCODE_RECIPE_PREFIX))
        {
            ptrSys->m_ParameterTextEdit= true;
            ptrSys->m_ParamTextEditName=getNameOfTextEdit(ptrSys->m_ParameterName);
            ptrSys->m_ParameterSwitch= false;
            ptrSys->m_ParameterComboBox = false;
        }
        else
        {
            ptrSys->m_ParameterSwitch = false;
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_ParameterComboBox = false;
        }
        m_listSysConfigGen.append(ptrSys);
    }
    context->setContextProperty("systemConfigGeneralParams", QVariant::fromValue(m_listSysConfigGen));
}

/**
 * @brief SysConfigData::getBransonSystemConfigList
 * @return
 */
QString SysConfigData::getSCBransonSystemConfigList()
{
    QString paramNames = SYS_SC_SERIAL_NUMBER + "," +
            SYS_AUX_BOX_SERIAL_NUMBER + "," +
            SYS_CONFIG_SYSTEM_TYPE;
//            SYS_CONFIG_SYSTEM_TYPE + "," +
//            SYS_CONFIG_SSD_SATA;
    return paramNames;

}
/**
 * @brief SysConfigData::createBransonSystemConfigList
 */
void SysConfigData::createBransonSystemConfigList()
{
    initializeComboxModelsName();

    context->setContextProperty("bransonSCSystemMenuModel", QVariant::fromValue(m_dummyBransonSystemMenuList));

    for(int element=0; element <m_SCBransonSystemMenuList.size() ; element++)
    {
        SystemConfigGeneral *ptrSysConfigGenParam = dynamic_cast<SystemConfigGeneral*>(m_SCBransonSystemMenuList.at(element));
        delete ptrSysConfigGenParam;
        ptrSysConfigGenParam = NULL;
    }
    m_SCBransonSystemMenuList.clear();
    QString sysConfigGenParams = getSCBransonSystemConfigList();
    QStringList bransonSysMenuList = sysConfigGenParams.split(",");
    for(int dwIndex=0;dwIndex<bransonSysMenuList.size();dwIndex++)
    {
        SystemConfigGeneral *ptrSys = new SystemConfigGeneral;
        ptrSys->m_ParameterName = bransonSysMenuList.at(dwIndex);

        if(ptrSys->m_ParameterName == SYS_CONFIG_SYSTEM_TYPE)
        {
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_ParameterComboBox = true;
            ptrSys->m_ParameterSwitch = false;
            ptrSys->m_ssdToggleSwitch = false;

            ptrSys->m_ComboBoxModelName=getCurrentNameOfComboBox(ptrSys->m_ParameterName);
        }
        else if((ptrSys->m_ParameterName == SYS_SC_SERIAL_NUMBER)||(ptrSys->m_ParameterName == SYS_AUX_BOX_SERIAL_NUMBER))
        {
            ptrSys->m_ParameterTextEdit= true;
            ptrSys->m_ParameterComboBox = false;
            ptrSys->m_ParameterSwitch= false;
            ptrSys->m_ssdToggleSwitch = false;

            ptrSys->m_ParamTextEditName=getNameOfTextEdit(ptrSys->m_ParameterName);
        }
        else if((ptrSys->m_ParameterName == SYS_CONFIG_SSD_SATA))
        {
            ptrSys->m_ParameterTextEdit= true;
            ptrSys->m_ParameterComboBox = false;
            ptrSys->m_ParameterSwitch = true;
            ptrSys->m_ssdToggleSwitch = true;
            ptrSys->m_ParamTextEditName=getNameOfTextEdit(ptrSys->m_ParameterName);
            ptrSys->m_ParameterSwitchState=getStatusOfSwitch(ptrSys->m_ParameterName);
            qDebug()<<"getstatus of switch"<<getStatusOfSwitch(ptrSys->m_ParameterName);
        }
        m_SCBransonSystemMenuList.append(ptrSys);
    }
    context->setContextProperty("bransonSCSystemMenuModel", QVariant::fromValue(m_SCBransonSystemMenuList));
    createBransonACSystemConfigList();
    createBransonPCSystemConfigList();
}

QString SysConfigData::getPCBransonSystemConfigList()
{
    QString paramNames = SYS_PC_SERIAL_NUMBER + "," +
            SYS_CONFIG_PS_POWER_LEVEL + "," +
            SYS_CONFIG_PS_FREQUENCY + "," +
            SYS_CONFIG_PS_TYPE;
    return paramNames;
}

void SysConfigData:: createBransonPCSystemConfigList()
{

    context->setContextProperty("bransonPCSystemMenuModel", QVariant::fromValue(m_dummyBransonSystemMenuList));

    for(int element=0; element <m_PCBransonSystemMenuList.size() ; element++)
    {
        SystemConfigGeneral *ptrSysConfigGenParam = dynamic_cast<SystemConfigGeneral*>(m_PCBransonSystemMenuList.at(element));
        delete ptrSysConfigGenParam;
        ptrSysConfigGenParam = NULL;
    }
    m_PCBransonSystemMenuList.clear();
    QString sysConfigGenParams = getPCBransonSystemConfigList();
    QStringList bransonSysMenuList =sysConfigGenParams.split(",");
    for(int dwIndex=0;dwIndex<bransonSysMenuList.size();dwIndex++)
    {
        SystemConfigGeneral *ptrSys = new SystemConfigGeneral;
        ptrSys->m_ParameterName = bransonSysMenuList.at(dwIndex);

        if((ptrSys->m_ParameterName == SYS_CONFIG_PS_TYPE)|| (ptrSys->m_ParameterName == SYS_CONFIG_PS_FREQUENCY))
        {
            ptrSys->m_ParameterComboBox = true;
            ptrSys->m_ComboBoxModelName=getCurrentNameOfComboBox(ptrSys->m_ParameterName);
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_ParameterSwitch = false;
        }
        else if((ptrSys->m_ParameterName ==SYS_PC_SERIAL_NUMBER)||(ptrSys->m_ParameterName ==SYS_CONFIG_PS_POWER_LEVEL))
        {
            ptrSys->m_ParameterTextEdit= true;
            ptrSys->m_ParamTextEditName=getNameOfTextEdit(ptrSys->m_ParameterName);
            ptrSys->m_ParameterSwitch= false;
            ptrSys->m_ParameterComboBox = false;
        }
        m_PCBransonSystemMenuList.append(ptrSys);
    }
    context->setContextProperty("bransonPCSystemMenuModel", QVariant::fromValue(m_PCBransonSystemMenuList));

}
QString SysConfigData::getACBransonSystemConfigList()
{
    QString paramNames = SYS_AC_SERIAL_NUMBER + "," +
            SYS_CONFIG_ACTUATOR_TYPE + "," +
            SYS_CONFIG_STROKE_LENGTH ;
    return paramNames;
}

void SysConfigData:: createBransonACSystemConfigList()
{

    context->setContextProperty("bransonACSystemMenuModel", QVariant::fromValue(m_dummyBransonSystemMenuList));

    for(int element=0; element <m_ACBransonSystemMenuList.size() ; element++)
    {
        SystemConfigGeneral *ptrSysConfigGenParam = dynamic_cast<SystemConfigGeneral*>(m_ACBransonSystemMenuList.at(element));
        delete ptrSysConfigGenParam;
        ptrSysConfigGenParam = NULL;
    }
    m_ACBransonSystemMenuList.clear();
    QString sysConfigGenParams = getACBransonSystemConfigList();
    QStringList bransonSysMenuList =sysConfigGenParams.split(",");
    for(int dwIndex=0;dwIndex<bransonSysMenuList.size();dwIndex++)
    {
        SystemConfigGeneral *ptrSys = new SystemConfigGeneral;
        ptrSys->m_ParameterName = bransonSysMenuList.at(dwIndex);

        if((ptrSys->m_ParameterName == SYS_CONFIG_ACTUATOR_TYPE))
        {
            ptrSys->m_ParameterComboBox = true;
            ptrSys->m_ComboBoxModelName=getCurrentNameOfComboBox(ptrSys->m_ParameterName);
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_ParameterSwitch = false;
        }
        else if((ptrSys->m_ParameterName ==SYS_AC_SERIAL_NUMBER)||(ptrSys->m_ParameterName ==SYS_CONFIG_STROKE_LENGTH))
        {
            ptrSys->m_ParameterTextEdit= true;
            ptrSys->m_ParamTextEditName=getNameOfTextEdit(ptrSys->m_ParameterName);
            ptrSys->m_ParameterSwitch= false;
            ptrSys->m_ParameterComboBox = false;
        }
        m_ACBransonSystemMenuList.append(ptrSys);
    }
    context->setContextProperty("bransonACSystemMenuModel", QVariant::fromValue(m_ACBransonSystemMenuList));

}

/**
 * @brief SysConfigData::initializeComboxModelsName
 * @return
 */
void SysConfigData :: initializeComboxModelsName()
{
    QString parameterMsg ;
    QStringList a_parameterMsgList;

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_STANDARD);
    context->setContextProperty("psTypeModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_SERVO);
    context->setContextProperty("actuatorTypeModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_STOP);
    a_parameterMsgList.append(S_CONTINUE);
    context->setContextProperty("memoryFullActionModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_DISABLED);
    a_parameterMsgList.append(S_FULL);
    context->setContextProperty("authorityCheckModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_GSX_E1);
    /*System Type ComboBox model names*/
    a_parameterMsgList.append(S_ASX);
    context->setContextProperty("systemTypeModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    parameterMsg="";
    for(int langindex=0;langindex<=LANGUAGE_DANISH;langindex++)
    {
        int actuallanguageid=getQTLanguageId(langindex);
        QString languagestringInEnglish = getQTLanguageName(actuallanguageid);
        QString nativelanguagestring = "";

        if (actuallanguageid == 31)
        {
            nativelanguagestring = languagestringInEnglish;
        }
        else
        {
            nativelanguagestring=getQTNativeLanguageName(actuallanguageid);
        }

        if(langindex<=LANGUAGE_DANISH+1)
        {
            parameterMsg.append(nativelanguagestring);
            parameterMsg.append(",");
        }
    }
    parameterMsg.remove(parameterMsg.size()-1,1);
    a_parameterMsgList = parameterMsg.split(',');
    context->setContextProperty("languageModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append("20000");
    a_parameterMsgList.append("30000");
    a_parameterMsgList.append("40000");
    context->setContextProperty("psFrequencyModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_METRIC);
    a_parameterMsgList.append(S_IMPERIAL);
    context->setContextProperty("unitsModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_DASHBOARD);
    a_parameterMsgList.append(S_PRODUCTION);
    a_parameterMsgList.append(S_RECIPE);
    a_parameterMsgList.append(S_ANALYTICS);
    context->setContextProperty("startScreenModel", QVariant::fromValue(a_parameterMsgList));

    a_parameterMsgList.clear();
    a_parameterMsgList.append(S_SEEK);
    context->setContextProperty("psPowerOnOptionModel", QVariant::fromValue(a_parameterMsgList));
}

/**
 * @brief SysConfigData::getStatusOfSwitch
 * @param QString paramText
 */
bool SysConfigData::getStatusOfSwitch(QString paramText)
{
    bool status=false;

    if(paramText== SYS_CONFIG_EXTERNAL_PRESETS)
    {
        status= Configuration::getInstance()->m_systemData.bIsExternalPresets;
    }
    else if(paramText== SYS_CONFIG_HORN_CLAMP)
    {
        status=Configuration::getInstance()->m_systemData.bIsHornClamp;
    }
    else if (paramText == SYS_CONFIG_PARTID_STATUS)
    {
        status= Configuration::getInstance()->m_systemData.bPartIDSwitchStatus;
    }
    else if (paramText == SYS_CONFIG_RESET_MEMORY)
    {
        status= Configuration::getInstance()->m_systemData.bResetMem;
    }
    else if (paramText == SYS_CONFIG_POWERUP_MEMORY)
    {
        status= Configuration::getInstance()->m_systemData.bPowerUpMem;
    }
    else
    {
        status=false;
    }
    return status;
}

/**
 * @brief SysConfigData::getNameOfTextEdit
 * @param QString paramText
 */
QString SysConfigData::getNameOfTextEdit(QString paramText)
{
    QString currentName="";
    if(paramText== SYS_CONFIG_BARCODE_RECIPE_PREFIX)
    {
        QString strBarcodePrefix=QChar::fromLatin1(Configuration::getInstance()->m_systemData.PresetRecipeBarcodeStart);
        currentName = strBarcodePrefix;
    }
    else if(paramText== SYS_CONFIG_MACHINE_NAME)
    {
        currentName= QString::fromStdString(Configuration::getInstance()->m_systemData.chMachineName);
    }
    else if(paramText== SYS_CONFIG_BRANSON_KEY)
    {
        currentName=  tempBransonKeyStrObj.FirstLevelKey;
    }
    else if(paramText == SYS_SC_SERIAL_NUMBER)
    {
        currentName=QString::fromStdString(Configuration::getInstance()->m_systemData.SC_SerialNumber);
    }
    else if(paramText == SYS_AUX_BOX_SERIAL_NUMBER)
    {
        currentName=QString::fromStdString(Configuration::getInstance()->m_systemData.AuxBoxSerialNumber);
    }
    else if(paramText == SYS_PC_SERIAL_NUMBER)
    {
        currentName=QString::fromStdString(Configuration::getInstance()->m_systemData.PC_SerialNumber);
    }
    else if(paramText == SYS_CONFIG_PS_POWER_LEVEL)
    {
        currentName=QString::number(Configuration::getInstance()->m_systemData.PcConfig.PSWatt);
    }
    else if(paramText== SYS_AC_SERIAL_NUMBER)
    {
        currentName= QString::fromStdString(Configuration::getInstance()->m_systemData.AC_SerialNumber);
    }
    else if(paramText== SYS_CONFIG_STROKE_LENGTH)
    {
        currentName= QString::number(Configuration::getInstance()->m_systemData.ActConfig.MaxStrokelength);
    }
    else
    {
        currentName="";
    }
    return currentName;
}

/**
 * @brief SysConfigData::getCurrentNameOfComboBox
 * @return int currentState
 */
int SysConfigData::getCurrentNameOfComboBox(QString paramText)
{
    int currentState = 0;
    if(paramText== SYS_CONFIG_MEMORY_FULL_ACTION)
    {
        currentState= (Configuration::getInstance()->m_systemData.MemFullActionIs);
    }
    else if(paramText== SYS_CONFIG_AUTHORITY_CHECK)
    {
        currentState= Configuration::getInstance()->m_systemData.AuthCheckLevel;
    }
    else if(paramText== SYS_CONFIG_LANGUAGE)
    {
        currentState= Configuration::getInstance()->m_systemData.LanguageIs;
    }
    else if(paramText== SYS_CONFIG_UNITS)
    {
        currentState= Configuration::getInstance()->m_systemData.UnitIs;
    }
    else if(paramText== SYS_CONFIG_START_SCREEN)
    {
        currentState= Configuration::getInstance()->m_systemData.Sscreen;
    }
    else if(paramText== SYS_CONFIG_PS_POWER_ON_OPTION)
    {
        currentState= Configuration::getInstance()->m_systemData.PsPowerUpOptionIs;
    }
    else if(paramText == SYS_CONFIG_SYSTEM_TYPE)
    {
        /*To update System Type Combo Box current index, based on saved config data.*/

        switch(Configuration::getInstance()->m_systemData.systemType)
        {
        case SYSTEM_TYPE_GSX_E1:
            currentState = 0;
            break;

        case SYSTEM_TYPE_ASX:
            currentState = 1;
            break;

        default:
            break;
        }
    }
    else if(paramText== SYS_CONFIG_PS_FREQUENCY)
    {
        currentState= Configuration::getInstance()->m_systemData.PcConfig.PSFreq;
    }
    else if(paramText== SYS_CONFIG_PS_TYPE)
    {
        currentState=(Configuration::getInstance()->m_systemData.PcConfig.PsTypeIs);
    }
    else if(paramText== SYS_CONFIG_ACTUATOR_TYPE)
    {
        currentState= (Configuration::getInstance()->m_systemData.ActConfig.ActuatorTypeIs);
    }
    else
    {
        currentState=0;
    }
    return currentState;
}

/**
 * @brief SysConfigData::updateToggleSwitchState
 * @param QString paramText,bool toggleState
 */
void SysConfigData::updateToggleSwitchState(QString paramText,bool toggleState)
{
    if(paramText==SYS_CONFIG_HORN_CLAMP)
    {
        Configuration::getInstance()->m_systemData.bIsHornClamp=toggleState;
    }
    else if(paramText==SYS_CONFIG_RESET_MEMORY)
    {
        Configuration::getInstance()->m_systemData.bResetMem = toggleState;
    }
    else if(paramText==SYS_CONFIG_POWERUP_MEMORY)
    {
        Configuration::getInstance()->m_systemData.bPowerUpMem = toggleState;
    }
    else if(paramText == SYS_CONFIG_PARTID_STATUS)
    {
        Configuration::getInstance()->m_systemData.bPartIDSwitchStatus = toggleState;
    }
    else if(paramText==SYS_CONFIG_EXTERNAL_PRESETS)
    {
        Configuration::getInstance()->m_systemData.bIsExternalPresets=toggleState;
    }
    else{}
}

/**
 * @brief SysConfigData::updateComboBoxState
 * @param QString paramText,QString currentState
 */
void SysConfigData::updateComboBoxState(QString paramText,int currentState)
{
    if(paramText==SYS_CONFIG_MEMORY_FULL_ACTION)
    {
        Configuration::getInstance()->m_systemData.MemFullActionIs=(MemFullAction)currentState;
    }
    else if(paramText==SYS_CONFIG_AUTHORITY_CHECK)
    {
        Configuration::getInstance()->m_systemData.AuthCheckLevel=(AuthorityCheck)currentState;
    }
    else if(paramText==SYS_CONFIG_LANGUAGE)
    {
        Configuration::getInstance()->m_systemData.LanguageIs=(App_Language)currentState;
        QString strLanguageValue = QString::number(currentState);
        Configuration::getInstance()->setConigValues("AppLanguage","CurrentLanguage",strLanguageValue);
    }
    else if(paramText==SYS_CONFIG_UNITS)
    {
        Configuration::getInstance()->m_systemData.UnitIs=(Units)currentState;
    }
    else if(paramText==SYS_CONFIG_START_SCREEN)
    {
        Configuration::getInstance()->m_systemData.Sscreen=(StartScreen)currentState;
    }
    else if(paramText==SYS_CONFIG_PS_POWER_ON_OPTION)
    {
        Configuration::getInstance()->m_systemData.PsPowerUpOptionIs=(PsPowerUpOptions)currentState;
    }
    else if(paramText==SYS_CONFIG_SYSTEM_TYPE)
    {
        /*To update config data based on System Type selected. Decision is taken based on selected index
          i.e. currentState, fetched from screen*/
        switch(currentState)
        {
        case 0:
            Configuration::getInstance()->m_systemData.systemType = SYSTEM_TYPE_GSX_E1;
            break;

        case 1:
            Configuration::getInstance()->m_systemData.systemType = SYSTEM_TYPE_ASX;
            break;

        default:
            break;
        }
    }
    else if(paramText==SYS_CONFIG_PS_FREQUENCY)
    {
        Configuration::getInstance()->m_systemData.PcConfig.PSFreq = currentState;
    }
    else if(paramText==SYS_CONFIG_PS_TYPE)
    {
        Configuration::getInstance()->m_systemData.PcConfig.PsTypeIs=(PSType)currentState;
    }
    else if(paramText==SYS_CONFIG_ACTUATOR_TYPE)
    {
        Configuration::getInstance()->m_systemData.ActConfig.ActuatorTypeIs = (ActuatorType)currentState;
    }
    else{}
}

/**
 * @brief SysConfigData::updateLineEditState
 * @param QString paramText,QString currentText
 */
void SysConfigData::updateLineEditState(QString paramText,QString currentText)
{
    if(paramText == SYS_CONFIG_BARCODE_RECIPE_PREFIX)
    {
        if(currentText.length()> 0 )
            memcpy(&Configuration::getInstance()->m_systemData.PresetRecipeBarcodeStart,currentText.data(),sizeof(char));
    }
    else if(paramText==SYS_CONFIG_MACHINE_NAME)
    {
        strcpy(Configuration::getInstance()->m_systemData.chMachineName,currentText.toStdString().c_str());
    }
    else if(paramText==SYS_CONFIG_BRANSON_KEY)
    {
        tempBransonKeyStrObj.FirstLevelKey = currentText;
    }
    else if(paramText == SYS_SC_SERIAL_NUMBER)
    {
        strcpy(Configuration::getInstance()->m_systemData.SC_SerialNumber,currentText.toStdString().c_str());
    }
    else if(paramText == SYS_AUX_BOX_SERIAL_NUMBER)
    {
        strcpy(Configuration::getInstance()->m_systemData.AuxBoxSerialNumber,currentText.toStdString().c_str());
    }
    else if(paramText == SYS_PC_SERIAL_NUMBER)
    {
        strcpy(Configuration::getInstance()->m_systemData.PC_SerialNumber,currentText.toStdString().c_str());
    }
    else if((paramText==SYS_CONFIG_PS_POWER_LEVEL))
    {
        Configuration::getInstance()->m_systemData.PcConfig.PSWatt = currentText.toInt();
    }
    else if(paramText == SYS_AC_SERIAL_NUMBER)
    {
        strcpy(Configuration::getInstance()->m_systemData.AC_SerialNumber,currentText.toStdString().c_str());
    }
    else if(paramText == SYS_CONFIG_STROKE_LENGTH)
    {
        Configuration::getInstance()->m_systemData.ActConfig.MaxStrokelength =  currentText.toInt();
    }
    else{}
}

/**
 * @brief SysConfigData::compareBransonKey
 * @param QString paramText
 */
int SysConfigData::compareBransonKey(QString paramText)
{
    qDebug()<<"key"<<paramText<< "  " << bransonKeyStrObj.FirstLevelKey;
    if(paramText == "")
        return 2;
    if(paramText == bransonKeyStrObj.FirstLevelKey)
        return 1;
    else
        return 0;
}

/**
 * @brief SysConfigData::compareBransonSecondLevelKey
 * @param keyValue
 * @return
 */
int SysConfigData::compareBransonSecondLevelKey(QString keyValue)
{
    if(keyValue == bransonKeyStrObj.SecondLevelKey)
        return 1;
    else
        return 0;
}

/**
 * @brief SysConfigData::getBransonKeyDetails
 */
void SysConfigData::getBransonKeyDetails()
{
    char data[1];

    CIObj.sendMessage(SCBL_BRANSON_KEY_REQ, UIC_BRANSON_KEY_RES, data, sizeof(data));

    QString a_bufferData;
    CIObj.recivedMessage(UIC_BRANSON_KEY_RES, a_bufferData);
    
    QStringList a_bransonDataList =a_bufferData.split(",");
    if(a_bransonDataList.size() == 2)
    {
        bransonKeyStrObj.FirstLevelKey = a_bransonDataList.at(0);
        bransonKeyStrObj.SecondLevelKey = a_bransonDataList.at(1);
    }
    else
    {
        qDebug()<<"Branson Key not found making it empty";
        bransonKeyStrObj.FirstLevelKey = "Sample!@#";
        bransonKeyStrObj.SecondLevelKey = "Sample!@#";
    }
}

/**
 * @brief SysConfigData::getLanguage
 * @return
 */
int SysConfigData::getLanguage()
{
    QString strDefLanguage = Configuration::getInstance()->getConfigValues("AppLanguage","CurrentLanguage");
    int nLanguageIndex = strDefLanguage.toInt();
    //retrived Languge Index should match to our existing range of lanugages otherwise it will be set to english
    if(nLanguageIndex >= LANGUAGE_ENGLISH && nLanguageIndex <= LANGUAGE_DANISH)
        Configuration::getInstance()->m_systemData.LanguageIs =(App_Language) nLanguageIndex;
    else
        Configuration::getInstance()->m_systemData.LanguageIs =LANGUAGE_ENGLISH;
    return Configuration::getInstance()->m_systemData.LanguageIs;
}

/**
 * @brief SysConfigData::getLanguageCode
 * @return
 */
QString SysConfigData::getLanguageCode()
{
    int nCurrentLanguageIndexId = getLanguage();
    int nActualLanguageId = getQTLanguageId(nCurrentLanguageIndexId);
    QString langCode = QLocale((QLocale::Language)nActualLanguageId).name();
    return langCode;
}

/**
 * @brief SysConfigData::setLanguage
 * @param a_language
 */
void SysConfigData::setLanguage(int a_language)
{
        Configuration::getInstance()->m_systemData.LanguageIs=(App_Language)a_language;
        QString strLanguageValue = QString::number(a_language);
        //Storing in config.ini
        Configuration::getInstance()->setConigValues("AppLanguage","CurrentLanguage",strLanguageValue);
        emit LanguageChanged();
}

/**
 * @brief SysConfigData::setSysConfigData
 */
void SysConfigData::setSysConfigData()
{
    //m_systemData=Configuration::getInstance()->m_systemData;
}

/**
 * @brief SysConfigData::saveSysConfigData : Checking the config data, Send data to BL and retun the values
 * @return
 */
int SysConfigData::saveSysConfigData()
{
    char *m_data=reinterpret_cast<char*>(&Configuration::getInstance()->m_systemData);
    char ch[8]={'\0'};

    if(Configuration::getInstance()->m_systemData.PcConfig.PSWatt < 400)
    {
        return 0 ;
    }
    else if(Configuration::getInstance()->m_systemData.ActConfig.MaxStrokelength <= 0)
    {
        return 2 ;
    }
    else if(Configuration::getInstance()->m_systemData.PresetRecipeBarcodeStart == ' ')
    {
        return 3;
    }
    else if(QString::fromStdString(Configuration::getInstance()->m_systemData.chMachineName) == "")
    {
        return 4;
    }
    else
    {
        CIObj.sendMessage(SCBL_SYSCONFIG_WRITE_REQ,UIC_SYSCONFIG_WRITE_RES,m_data,sizeof(Configuration::getInstance()->m_systemData));
        CIObj.recivedMessage(UIC_SYSCONFIG_WRITE_RES,ch,7);
        Configuration::getInstance()->setSystemLanguage();
        return 1;
    }
}

/**
 * @brief SysConfigData::setDefaultValues : Default values to Assign and displayed in UI
 */
void SysConfigData::setDefaultValues()
{
    Configuration::getInstance()->m_systemData.bIsExternalPresets=false;
    Configuration::getInstance()->m_systemData.bIsHornClamp=false;
    Configuration::getInstance()->m_systemData.PcConfig.PsTypeIs=PS_TYPE_DHD;
    Configuration::getInstance()->m_systemData.ActConfig.ActuatorTypeIs=ACTUATOR_TYPE_SERVO;

    Configuration::getInstance()->m_systemData.PcConfig.PSWatt=800;//3000;//;
    Configuration::getInstance()->m_systemData.LanguageIs=LANGUAGE_ENGLISH;
    Configuration::getInstance()->m_systemData.ActConfig.ControlLevelIs=CONTROL_LEVEL_GSX_S;
    Configuration::getInstance()->m_systemData.PcConfig.PSFreq=0;
    Configuration::getInstance()->m_systemData.UnitIs=UNIT_METRIC;
    Configuration::getInstance()->m_systemData.Sscreen=START_SCREEN_DASHBOARD;
    Configuration::getInstance()->m_systemData.PsPowerUpOptionIs=PS_NONE;
    Configuration::getInstance()->m_systemData.AuthCheckLevel=AUTH_CHECK_DISABLED;
    Configuration::getInstance()->m_systemData.MemFullActionIs=MEM_FULL_CONTINUE;
    Configuration::getInstance()->m_systemData.bResetMem=true;
    Configuration::getInstance()->m_systemData.bPowerUpMem=true;
    Configuration::getInstance()->m_systemData.PresetRecipeBarcodeStart = 'R';
    Configuration::getInstance()->m_systemData.bPartIDSwitchStatus = false;
    Configuration::getInstance()->m_systemData.ActConfig.MaxStrokelength = 125;
    //strcpy(  Configuration::getInstance()->m_systemData.bransonKey,Configuration::getInstance()->m_bransonKeyValue.toStdString().c_str());
}

SysConfigData *SysConfigData::getInstance()
{
    if(sysConfigPtr == nullptr)
    {
        sysConfigPtr = new SysConfigData();
    }
    return sysConfigPtr;
}

/**
 * @brief SysConfigData::sendSetupReq : System configutration updated model display in UI
 */
void SysConfigData::sendSetupReq()
{
    m_WaitingStatus="Device is ready for setup, Please press palm button...";
    context->setContextProperty("sysconfig",this);
    char requestMessage[] = "";

    CIObj.sendMessage(SCBL_SETUP_READ_REQ,SCBL_SETUP_RES, requestMessage, 0);
}

/**
 * @brief SysConfigData::recvSetupRes
 * @return
 */
QString SysConfigData::recvSetupRes()
{
    return m_PartContactDist;
}

/**
 * @brief SysConfigData::setWaitingStatus
 * @return
 */
QString SysConfigData::setWaitingStatus()
{
    return m_WaitingStatus;
}

/**
 * @brief SysConfigData::setSetupModel : Setting the setup parameter range and save in model data
 */
void SysConfigData::setSetupModel()
{
    context->setContextProperty("setupModel", QVariant::fromValue(m_dummySetupData));
    for(int element=0; element <m_SetUpData.size() ; element++)
    {
        SetupParameter *ptrSetupParameter = dynamic_cast<SetupParameter*>(m_SetUpData.at(element));
        delete ptrSetupParameter;
        ptrSetupParameter = NULL;
    }
    m_SetUpData.clear();

    m_SetUpData.append(new SetupParameter("Part Contact Window Minus","xx.xx","mm"));
    m_SetUpData.append(new SetupParameter("Part Contact Window Plus","xx.xx","mm"));
    m_SetUpData.append(new SetupParameter("Part Contact Window Offset","xx.xx","mm"));
    context->setContextProperty("setupModel",QVariant::fromValue(m_SetUpData));
}

/**
 * @brief SysConfigData::setPartContactDistance
 * @param a_PartContactDistance
 */
void SysConfigData::setPartContactDistance(QString a_PartContactDistance)
{
    double temp = a_PartContactDistance.toDouble();
    double temp1 = temp/1000;
    m_PartContactDist.setNum(temp1,'f',3);
    if(temp > 0)
        m_WaitingStatus="Setup Successful";
    else
        m_WaitingStatus="Setup Failed";

    context->setContextProperty("sysconfig",this);
}

/**
 * @brief SysConfigData::setMachineName : Seting the updated machine name
 * @param a_MachineName
 */
void SysConfigData::setMachineName(QString a_MachineName)
{
    strcpy(Configuration::getInstance()->m_systemData.chMachineName,a_MachineName.toStdString().c_str());
}

/**
 * @brief SysConfigData::getMachineName : Get the Updated Machine name
 * @return
 */
QString SysConfigData::getMachineName()
{
    return QString::fromStdString(Configuration::getInstance()->m_systemData.chMachineName);
}

/**
 * @brief SysConfigData::getAuthorityCheck
 * @return
 */
int SysConfigData::getAuthorityCheck()
{
    return Configuration::getInstance()->m_systemData.AuthCheckLevel;
}

/**
 * @brief SysConfigData::setAuthorityCheck
 */
void SysConfigData::setAuthorityCheck()
{
    QString strAuthorityCheck = QString::number(Configuration::getInstance()->m_systemData.AuthCheckLevel);
    Configuration::getInstance()->setConigValues("AuthorityCheck","AuthorityCheckStatus",strAuthorityCheck);
}

/**
 * @brief SysConfigData::getTimeAndDateFromSC : Read the data and time following format
 */
void SysConfigData::getTimeAndDateFromSC()
{
    QString dateAndTime="";//"2012/07/06";//+" "+"19:30:00";

    //    dateAndTime.append(" ");
    //    dateAndTime.append("19:30:00");
    CIObj.sendMessage(SCBL_SYS_DATETIME_READ_REQ, UIC_SYS_DATETIME_READ_RES   , "");
    CIObj.recivedMessage(UIC_SYS_DATETIME_READ_RES   , dateAndTime);
    QRegularExpression regExp("^\\d\\d\\d\\d/\\d\\d/\\d\\d \\d\\d:\\d\\d:\\d\\d$");
    QRegularExpressionMatch match = regExp.match(dateAndTime);

    if( match.hasMatch())
    {
        QStringList query=dateAndTime.split(" ");
        QString date=query.at(0);
        QString time=query.at(1);

        m_TitleBarDate.setDate(QDate::fromString(date,"yyyy/MM/dd"));
        m_TitleBarDate.setTime(QTime::fromString(time,"hh:mm:ss"));
        emit DateAndTimeChanged();
    }
   
}

/**
 * @brief SysConfigData::incrementTimeBySec : Increment time by seconds
 */
void SysConfigData::incrementTimeBySec()
{
    QDateTime titleBarDate = m_TitleBarDate.addSecs(1);
    qSwap(titleBarDate,m_TitleBarDate);
    emit DateAndTimeChanged();
}

/**
 * @brief SysConfigData::setDateAndTime
 * @param a_dateAndTime
 */
void SysConfigData::setDateAndTime(QDateTime a_dateAndTime)
{
    if(a_dateAndTime!=m_TitleBarDate){
        m_TitleBarDate=a_dateAndTime;
        emit DateAndTimeChanged();
    }
}

/**
 * @brief SysConfigData::getDateAndTime
 * @return
 */
QDateTime SysConfigData::getDateAndTime()
{
    return m_TitleBarDate;
}

/**
 * @brief SysConfigData::convertDateAndTimeToString : Convert the date fromat
 * @return
 */
QString SysConfigData::convertDateAndTimeToString()
{
    QDate date=m_TitleBarDate.date();
    QString datestring=date.toString("yyyy/MM/dd");

    QTime time=m_TitleBarDate.time();
    QString timestring=time.toString("hh:mm:ss");

    QString strToSend=datestring+" "+timestring;
    return strToSend;
}

/**
 * @brief SysConfigData::sendDateAndTimeToSC : Send the convert date format to BL
 * @param dateTime
 * @return
 */
bool SysConfigData::sendDateAndTimeToSC(QString dateTime)
{
    CIObj.sendMessage(SCBL_SYS_DATETIME_WRITE_REQ,UIC_SYS_DATETIME_WRITE_RES,dateTime);
    QString result="";
    CIObj.recivedMessage(UIC_SYS_DATETIME_WRITE_RES,result);
    if(result=='1')
    {
        return true;
    }
    return false;
}

/**
 * @brief SysConfigData::generateReportResPopup
 * @param res
 * @return
 */
void SysConfigData::generateReportResPopup(QString res)
{
    int resData;
    resData = res.toInt();
    setReportRes(resData);
    QMetaObject::invokeMethod(object,"generateReportPopup");
}

/**
 * @brief SysConfigData::vacuumReportResPopup
 * @param res
 * @return
 */
void SysConfigData::vacuumReportResPopup(QString res)
{
    setVacuumRes(res);
    QMetaObject::invokeMethod(object,"vacuumReportPopup");
}

/**
 * @brief SysConfigData::dBMemoryStatusPopup
 * @param res
 * @return
 */
void SysConfigData::dBMemoryStatusPopup(QString res)
{
    int resData;
    resData = res.toInt();
    setDbmemoryStatus(resData);
    QMetaObject::invokeMethod(object,"dbMemoryStatusPopUp");
}

/**
 * @brief SysConfigData::getGlobalCheckFlag
 * @return
 */
bool SysConfigData::getGlobalCheckFlag() const
{
    return m_GlobalCheckFlag;
}

/**
 * @brief SysConfigData::setGlobalCheckFlag
 * @param GlobalCheckFlag
 */
void SysConfigData::setGlobalCheckFlag(bool GlobalCheckFlag)
{
    if(GlobalCheckFlag != m_GlobalCheckFlag)
    {
        m_GlobalCheckFlag = GlobalCheckFlag;
        globalCheckFlagChanged();
    }
}

/**
 * @brief SysConfigData::getServiceSwitchStatus : Get the service type status and saved
 * @param paramName
 * @return
 */
bool SysConfigData::getServiceSwitchStatus(QString paramName)
{
    bool status=false;

    if(paramName == SYS_SERVICE_BOARD_REPLACED)
    {
        status=serviceParameterObj.boardReplaced;
    }
    else if(paramName== SYS_SERVICE_SYSTEM_CLEANED)
    {
        status= serviceParameterObj.systemCleaned;
    }
    else
    {
        status=false;
    }
    return status;
}

/**
 * @brief SysConfigData::setServiceSwitchStatus : set the service type status
 * @param index
 * @param status
 */
void SysConfigData::setServiceSwitchStatus(int index, bool status)
{
    switch(index)
    {
    case 4:
        serviceParameterObj.boardReplaced = status;
        break;
    case 5:
        serviceParameterObj.systemCleaned = status;
        break;
    default:
        break;
    }
}

/**
 * @brief SysConfigData::resetServiceSwitchStatus : Reset to default values
 */
void SysConfigData::resetServiceSwitchStatus()
{
    serviceParameterObj.boardReplaced = false;
    serviceParameterObj.systemCleaned = false;

    createServiceRightModel();
}

/**
 * @brief SysConfigData::saveServiceData
 * @param serviceNoteText
 */
void SysConfigData::saveServiceData(QString serviceNoteText)
{
    if(serviceParameterObj.boardReplaced)
    {
        Eventlogger.eventSend(12, serviceNoteText);
    }
    if(serviceParameterObj.systemCleaned)
    {
        Eventlogger.eventSend(13, serviceNoteText);
    }
    resetServiceSwitchStatus();
}

/**
 * @brief SysConfigData::getServiceRightModel
 * @return
 */
QStringList SysConfigData::getServiceRightModel()
{
    QStringList rightParams;
    rightParams.append(SYS_SERVICE_FACTORY_RESET);
    rightParams.append(SYS_SERVICE_DELETE_DB);
    rightParams.append(SYS_SERVICE_COPY_DB);
    rightParams.append(SYS_SERVICE_EMMC_WEAR_LEVEL);
  //  rightParams.append(SYS_SERVICE_LIFE_TIME_ALARMS);
  //  rightParams.append(SYS_SERVICE_LIFE_TIME_WELDS);
    rightParams.append(SYS_SERVICE_BOARD_REPLACED);
    rightParams.append(SYS_SERVICE_SYSTEM_CLEANED);
    rightParams.append(SYS_SERVICE_SERVICE_NOTES);
    return rightParams;
}

/**
 * @brief SysConfigData::createServiceRightModel : Model is created from saved values in particular list
 */
void SysConfigData::createServiceRightModel()
{
    context->setContextProperty("serviceRightParamModel", QVariant::fromValue(m_serviceRightParamListdummy));

    for(int element=0; element <m_serviceRightParamList.size() ; element++)
    {
        SystemConfigGeneral *ptrSysConfigGenParam = dynamic_cast<SystemConfigGeneral*>(m_serviceRightParamList.at(element));
        delete ptrSysConfigGenParam;
        ptrSysConfigGenParam = NULL;
    }
    m_serviceRightParamList.clear();
    QStringList serviceRightParamsList = getServiceRightModel();
    for(int dwIndex=0;dwIndex<serviceRightParamsList.size();dwIndex++)
    {
        SystemConfigGeneral *ptrSys = new SystemConfigGeneral;
        ptrSys->m_ParameterName = serviceRightParamsList.at(dwIndex);

        if((ptrSys->m_ParameterName == SYS_SERVICE_BOARD_REPLACED)|| (ptrSys->m_ParameterName == SYS_SERVICE_SYSTEM_CLEANED))
        {
            ptrSys->m_ParameterSwitch = true;
            ptrSys->m_ParameterSwitchState=getServiceSwitchStatus(ptrSys->m_ParameterName);
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_text = false;
            ptrSys->m_PrimaryButton = false;
        }
        else if((ptrSys->m_ParameterName ==SYS_SERVICE_SERVICE_NOTES))
        {
            ptrSys->m_ParameterTextEdit= true;
            ptrSys->m_ParamTextEditName=getNameOfTextEdit(ptrSys->m_ParameterName);
            ptrSys->m_ParameterSwitch= false;
            ptrSys->m_text = false;
            ptrSys->m_PrimaryButton = false;
        }
       // else if((ptrSys->m_ParameterName ==SYS_SERVICE_LIFE_TIME_ALARMS) || (ptrSys->m_ParameterName ==SYS_SERVICE_LIFE_TIME_WELDS))
        else if (ptrSys->m_ParameterName==SYS_SERVICE_EMMC_WEAR_LEVEL)
        {
            ptrSys->m_ParameterTextEdit= false;
            //ptrSys->m_ParamTextEditName=getNameOfTextEdit(ptrSys->m_ParameterName);
            ptrSys->m_ParameterSwitch= false;
            ptrSys->m_text = true;
            ptrSys->m_PrimaryButton = false;
        }
        else if((ptrSys->m_ParameterName ==SYS_SERVICE_FACTORY_RESET) || (ptrSys->m_ParameterName == SYS_SERVICE_DELETE_DB) || (ptrSys->m_ParameterName == SYS_SERVICE_COPY_DB))
        {
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_ParamButtonText=(ptrSys->m_ParameterName ==SYS_SERVICE_FACTORY_RESET)?"RESET":(ptrSys->m_ParameterName == SYS_SERVICE_DELETE_DB) ? "DELETE":(ptrSys->m_ParameterName == SYS_SERVICE_COPY_DB)?"COPY":" ";
            ptrSys->m_ParameterSwitch= false;
            ptrSys->m_text = false;
            ptrSys->m_PrimaryButton = true;
        }
        else
        {
            ptrSys->m_ParameterSwitch = false;
            ptrSys->m_ParameterTextEdit= false;
            ptrSys->m_text = false;
            ptrSys->m_PrimaryButton = false;
        }
        m_serviceRightParamList.append(ptrSys);
    }
    context->setContextProperty("serviceRightParamModel", QVariant::fromValue(m_serviceRightParamList));

}

/**
 * @brief SysConfigData::createServiceNotesList : Write values from the textfield should display in list
 * @param servicenoteText
 */
void SysConfigData::createServiceNotesList(QString servicenoteText)
{
    if(!servicenoteText.isEmpty())
    {
        QFile serviceTextFile("servicetext.txt");
        serviceTextFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        if(!serviceTextFile.isOpen())
        {
            qDebug() << "- Error, unable to open" << "outputFilename" << "for output";
            return;
        }
        QTextStream outStream(&serviceTextFile);
        outStream << servicenoteText << " ^`~\n";
        serviceTextFile.close();
        //return 0;
        SystemConfigGeneral *temp = new SystemConfigGeneral;
        temp->m_ParameterName = servicenoteText;

        m_serviceNotes.append(temp);

        context->setContextProperty("serviceNotesModel",QVariant::fromValue(m_serviceNotes));
    }
}

/**
 * @brief SysConfigData::updateServiceNotesFromFile : Saving the Text file format in your local path
 */
void SysConfigData::updateServiceNotesFromFile()
{
    QFile serviceTextFile("servicetext.txt");
    serviceTextFile.open(QIODevice::ReadOnly);
    if(!serviceTextFile.isOpen())
    {
        qDebug() << "- Error, unable to open" << "outputFilename" << "for output";
        return;
    }
    QTextStream inStream(&serviceTextFile);
    QStringList line;
    QString text = inStream.readLine();
    QString delimiterChar = "~";

    while (!inStream.atEnd())
    {
        if(text.endsWith(delimiterChar))
        {
            line.append( validatingDelimiter(text));
            text = inStream.readLine();
        }
        else
        {
            text.append("\n");
            text.append(inStream.readLine());
        }
    }
    line.append( validatingDelimiter(text));
    serviceTextFile.close();
    m_serviceNotes.clear();
    for(int dwIndex=0;dwIndex<line.size();dwIndex++)
    {
        SystemConfigGeneral *ptrSys = new SystemConfigGeneral;
        ptrSys->m_ParameterName = line.at(dwIndex);
        m_serviceNotes.append(ptrSys);
    }

    context->setContextProperty("serviceNotesModel",QVariant::fromValue(m_serviceNotes));
}

/**
 * @brief SysConfigData::validatingDelimiter : Validating the Textfield with regular expression
 * @param text
 * @return
 */
QString SysConfigData :: validatingDelimiter(QString text)
{
    QStringList tempList;
    QString line;
    tempList = text.split(" ");
    for(int dwIndex = 0; dwIndex <= tempList.size(); dwIndex++)
    {
        if(tempList[dwIndex] == "^`~")
            break;
        else
        {
            line.append(tempList[dwIndex]);
            line.append(" ");
        }
    }
    return line;
}

/**
 * @brief SysConfigData::getQTLanguageId
 * @param currentIndex
 * @return
 */
int SysConfigData::getQTLanguageId(int currentIndex)
{
    App_Language appLangIndex = (App_Language)currentIndex;
    int nQTLangId = QLocale::English;
    switch(appLangIndex)
    {
    case LANGUAGE_ENGLISH:
        nQTLangId = QLocale::English;
        break;
    case LANGUAGE_FRENCH:
        nQTLangId = QLocale::French;
        break;
    case LANGUAGE_GERMAN:
        nQTLangId = QLocale::German;
        break;
    case LANGUAGE_SPANISH:
        nQTLangId = QLocale::Spanish;
        break;
    case LANGUAGE_KOREAN:
        nQTLangId = QLocale::Korean;
        break;
//    case LANGUAGE_TRADITIONAL_CHINESE:
//        //nQTLangId = QLocale::Chinese;
//        nQTLangId = QLocale(QLocale::Chinese, QLocale::China).language();
//        break;
    case LANGUAGE_SIMPLIFIED_CHINESE:
        //nQTLangId = QLocale::Chinese;
        nQTLangId = QLocale(QLocale::Chinese, QLocale::Taiwan).language();
        break;
    case LANGUAGE_ITALIAN:
        nQTLangId = QLocale::Italian;
        break;
    case LANGUAGE_JAPANESE:
        nQTLangId = QLocale::Japanese;
        break;
    case LANGUAGE_SLOVAK:
        nQTLangId = QLocale::Slovak;
        break;
    case LANGUAGE_DANISH:
        nQTLangId = QLocale::Danish;
        break;
    }

    return nQTLangId;
}

/**
 * @brief SysConfigData::getQTLanguageName
 * @param nLanguageIndex
 * @return
 */
QString SysConfigData::getQTLanguageName(int nLanguageIndex)
{
    //where nLanguageIndex is comobbox Index, nRealLanguageId is real constant for the selected language
    QLocale::Language nRealLanguageId = (QLocale::Language)nLanguageIndex;
    return QLocale::languageToString(nRealLanguageId);
}

/**
 * @brief SysConfigData::getQTNativeLanguageName
 * @param lRealLanguageId
 * @return
 */
QString SysConfigData::getQTNativeLanguageName(int lRealLanguageId)
{
    QLocale::Language lRealLanguageId1 = (QLocale::Language)lRealLanguageId;

    return QLocale(lRealLanguageId1).nativeLanguageName();
}

/**
 * @brief SysConfigData::deleteDB
 * @return
 */
bool SysConfigData::deleteDB()
{
    CIObj.sendMessage(REQ_DELETE_DB,RES_DELETE_DB,convertDateAndTimeToString());
    QString result="";
    CIObj.recivedMessage(RES_DELETE_DB,result);
    if(result=='1')
    {
        return true;
    }
    return false;
}

/**
 * @brief SysConfigData::copyDB : Copy the DB request send to BL
 * @return
 */
void SysConfigData::copyDB()
{
    CIObj.sendMessage(REQ_COPY_DB,RES_COPY_DB,convertDateAndTimeToString());
}

/**
 * @brief SysConfigData::setPresetBarcode_RecallRecipe : Present Barcode Recipe Data information
 * @param cPresetBarcodeRecallRecipe
 */
void SysConfigData::setPresetBarcode_RecallRecipe(QString cPresetBarcodeRecallRecipe)
{
    //Configuration::getInstance()->m_systemData.PresetRecipeBarcodeStart=cPresetBarcodeRecallRecipe[1];
    memcpy(&Configuration::getInstance()->m_systemData.PresetRecipeBarcodeStart,cPresetBarcodeRecallRecipe.data(),sizeof(char));
}

/**
 * @brief SysConfigData::getPresetBarcode_RecallRecipe
 * @return
 */
QString SysConfigData::getPresetBarcode_RecallRecipe()
{
    QString l_PresetRecipeBarcodeStart=QChar::fromLatin1(Configuration::getInstance()->m_systemData.PresetRecipeBarcodeStart);
    return  l_PresetRecipeBarcodeStart;
}

/**
 * @brief SysConfigData::setPartIdScanRequired : Set PartId scan Request
 * @param bPartIdScanRequired
 */
void SysConfigData::setPartIdScanRequired(bool bPartIdScanRequired)
{
    Configuration::getInstance()->m_systemData.bPartIDSwitchStatus = bPartIdScanRequired;
}

/**
 * @brief SysConfigData::getPartIdScanRequired : Get PartID scan Request
 * @return
 */
bool SysConfigData::getPartIdScanRequired()
{
    bool bPartIdScanRequired = Configuration::getInstance()->m_systemData.bPartIDSwitchStatus;

    return  bPartIdScanRequired;
}

/**
 * @brief SysConfigData::sendfactoryQuery : Send factory reset Query to DB
 */
bool SysConfigData::sendfactoryQuery()
{
    QString qstrTemp;
    char m_saveResult;
    char ch[4]={'\0'};

    /*Removing servicetext.txt file during factory reset*/
    if( remove( "servicetext.txt" ) != 0 )
    {
        qDebug() << "- Error, unable to delete";
    }
    else
    {
        qDebug() << "servicetext File is deleted";
    }

    CIObj.sendMessage(SCBL_FACTORY_RESET_REQ, UIC_FACTORY_RESET_RSP,"");
    CIObj.recivedMessage(UIC_FACTORY_RESET_RSP,ch,1);

    CIObj.sendMessage(REQ_FACTORY_RESET, RES_FACTORY_RESET,"");
    CIObj.recivedMessage(RES_FACTORY_RESET,qstrTemp);

    memcpy(&m_saveResult,ch,1);

    if(m_saveResult == '1')
    {
        return true;
    }
    return false;
}

/**
 * @brief SysConfigData::copyDBToUSBRes
 * @param res
 */
void SysConfigData::copyDBToUSBRes(QString res)
{
    int resData;
    resData = res.toInt();
    setCopyDBRes(resData);
    QMetaObject::invokeMethod(object,"copyingDBPopUpRes");
}

/**
 * @brief SysConfigData::getCopyDBRes
 * @return
 */
int SysConfigData::getCopyDBRes() const
{
    return m_copyDBRes;
}

/**
 * @brief SysConfigData::setCopyDBRes
 * @param copyDBRes
 */
void SysConfigData::setCopyDBRes(int copyDBRes)
{
    m_copyDBRes = copyDBRes;
}


/**
 * @brief SysConfigData::stopTimerThread
 */
void SysConfigData::stopTimerThread()
{
    dateTimeTimer.stopTimer();
}

/**
 * @brief SysConfigData::startTimerThread
 */
void SysConfigData::startTimerThread()
{
    dateTimeTimer.startTimer();
}

/**
 * @brief SysConfigData::ReadEMMCWareUsage
 */
QString SysConfigData::ReadEMMCWareUsage()
{
    qDebug()<<"enter here";
    CIObj.sendMessage(SCBL_EMMC_WARE_LEVEL_REQ,UIC_EMMC_WARE_LEVEL_RESP,convertDateAndTimeToString());
    QString result="";
    qDebug()<<"in here";
    CIObj.recivedMessage(UIC_EMMC_WARE_LEVEL_RESP,result);
    qDebug()<<"getting the number:"<<result;

    return result;
}

/**
 * @brief SysConfigData::getEMMCUsage()
 */
QString SysConfigData::getEMMCUsage()
{
    return m_EMMCUsage;
}


/**
 * @brief SysConfigData::updateEMMCUsage(QString EMMCUsageValue)
 * @param EMMCUsageValue
 */
void SysConfigData::updateEMMCUsage(QString EMMCUsageValue)
{
    if (m_EMMCUsage != EMMCUsageValue)
    {
        m_EMMCUsage = EMMCUsageValue;
        emit emmcUsageChanged();
    }
}
/**
 * @brief DateAndTime::DateAndTime
 */
DateAndTime::DateAndTime()
{
}

/**
 * @brief DateAndTime::~DateAndTime
 */
DateAndTime::~DateAndTime()
{
    terminate();
}

/**
 * @brief DateAndTime::stratDateAndTimeThread
 */
void DateAndTime::startTimer()
{
    start();
}

/**
 * @brief DateAndTime::stopDateAndTimeThread
 */
void DateAndTime::stopTimer()
{
    terminate();
}

/**
 * @brief DateAndTime::run thread
 */
void DateAndTime::run()
{
    while(1)
    {
        SysConfigData::getInstance()->incrementTimeBySec();
        Sleep(1000);
    }
}
