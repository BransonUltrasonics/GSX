/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to set and get Alarm configuration settings from BL and updates in UI
    and vice versa

***************************************************************************/
#include <Header/alarmconfiguration.h>
#include "Header/communicationinterface.h"

extern QQmlContext* context;
extern CommunicationInterface CIObj;
/**
 * @brief AlarmConfiguration::AlarmConfiguration : Initialization of class members and memory for object
 * @param : QObject parent
 */
AlarmConfiguration::AlarmConfiguration(QObject *parent) : QObject(parent)
{
    createalarmcolumns();
    createAlarmRows();
    saveStatus = false;

    /* Refreshing UI alarmconfig model*/
    context->setContextProperty("alarmconfig", this);
    context->setContextProperty("modelCheckBoxStatus", QVariant::fromValue(m_statuslistdummy));
}

/**
 * @brief AlarmConfiguration::~AlarmConfiguration : Destructor
 */
AlarmConfiguration::~AlarmConfiguration()
{
    AlarmConfiguration* temp;
    int ind = 0 ;

    for(ind = 0 ; ind < m_statuslist.length() ; ind++)
    {
        temp = dynamic_cast<AlarmConfiguration*>(m_statuslist.at(ind));
        delete temp ;
        temp = NULL ;
    }

    m_statuslist.clear();
}

/**
 * @brief AlarmConfiguration::getAlarmcolums : Returns the alarm options
 * @return : QString
 */
QString AlarmConfiguration::getAlarmcolums()
{
    QString columnnames;
   columnnames = ALARM_CONFIG_RESET_REQUIRED+","+
                ALARM_CONFIG_LOG_ALARM+","+
                ALARM_CONFIG_GENERAL_ALARM;//+","+
//                ALARM_CONFIG_CUSTOM_ALARM_REQUIRED+","+
//                ALARM_CONFIG_CYCLE_COUNTER;
    return columnnames;
}

/**
 * @brief AlarmConfiguration::createalarmcolumns : Helps to send the alarm options to UI
 */
void AlarmConfiguration::createalarmcolumns()
{
    /* Refreshing UI alarm config columns model*/
    context->setContextProperty("modelAlarmconfigcolumns", QVariant::fromValue(m_columnslistdummy));
    m_columnslist.clear();
    QString alarmcolumn =  getAlarmcolums();
    QStringList columnlist = alarmcolumn.split(",");

    if(columnlist.size()>1)
    {
        for(int dwIndex=0;dwIndex<columnlist.size();dwIndex++)
        {
            Alarmconfigcolumn* alarmcolumnList = new Alarmconfigcolumn;
            alarmcolumnList->m_ParameterName=columnlist[dwIndex];
            m_columnslist.append(alarmcolumnList);
        }
    }

    /* Updating UI alarm config columns model*/
    context->setContextProperty("modelAlarmconfigcolumns", QVariant::fromValue(m_columnslist));
}

/**
 * @brief AlarmConfiguration::getAlarmRows : Returns the alarm parameter names
 * @return : QString
 */
QString AlarmConfiguration::getAlarmRows()
{
    QString rowstring;
    rowstring = ALARM_CONFIG_OVERLOADS+","+
            ALARM_CONFIG_CYCLE_MODIFIED+","+
            ALARM_CONFIG_WARNINGS+","+
            ALARM_CONFIG_SUSPECT+","+
            ALARM_CONFIG_REJECT+","+
            ALARM_CONFIG_NO_CYCLE+","+
            ALARM_CONFIG_HARDWARE_FAILURE+","+
            ALARM_CONFIG_NO_CYCLE_OVERLOADS;
    return rowstring;
}

/**
 * @brief AlarmConfiguration::createAlarmRows : Helps to send the alarm parameters to UI
 */
void AlarmConfiguration::createAlarmRows()
{
    /* Refreshing UI alarm config rows model*/
    context->setContextProperty("modelAlarmconfigrows", QVariant::fromValue(m_rowslistdummy));
    m_rowslist.clear();
    QString alarmrows=getAlarmRows();
    QStringList rowlist = alarmrows.split(",");

    if(rowlist.size()>1)
    {
        for(int dwIndex=0;dwIndex<rowlist.size();dwIndex++)
        {
            Alarmconfigcolumn* alarmRowList = new Alarmconfigcolumn;
            alarmRowList->m_RowName=rowlist[dwIndex];
            m_rowslist.append(alarmRowList);
        }
    }

    /* Updating UI alarm config rows model*/
    context->setContextProperty("modelAlarmconfigrows", QVariant::fromValue(m_rowslist));
}

/**
 * @brief AlarmConfiguration::saveAlarmconfigDetails : Saves alarm configuration details into structure
 * @param : int index
 * @param : bool checkboxstatus1
 */
void AlarmConfiguration::saveAlarmconfigDetails(int index, bool checkboxstatus1)
{
    int checkboxstatus = checkboxstatus1;
    switch (index)
    {
    case OverloadsAlarmActions_ResetRequired:
        datatoBl.OverloadsAlarmActions.ResetRequired = checkboxstatus;
        break;
    case OverloadsAlarmActions_LogAlarm:
        datatoBl.OverloadsAlarmActions.LogAlarm = checkboxstatus;
        break;
    case OverloadsAlarmActions_GeneralAlarm:
        datatoBl.OverloadsAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 3:
//        datatoBl.OverloadsAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 4:
//        datatoBl.OverloadsAlarmActions.CycleCounter = checkboxstatus;
//        break;
    case CycleModifiedAlarmActions_ResetRequired:
        datatoBl.CycleModifiedAlarmActions.ResetRequired = checkboxstatus;
        break;
    case CycleModifiedAlarmActions_LogAlarm:
        datatoBl.CycleModifiedAlarmActions.LogAlarm = checkboxstatus;
        break;
    case CycleModifiedAlarmActions_GeneralAlarm:
        datatoBl.CycleModifiedAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 8:
//        datatoBl.CycleModifiedAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 9:
//        datatoBl.CycleModifiedAlarmActions.CycleCounter = checkboxstatus;
//        break;
    case WarningsAlarmActions_ResetRequired:
        datatoBl.WarningsAlarmActions.ResetRequired = checkboxstatus;
        break;
    case WarningsAlarmActions_LogAlarm:
        datatoBl.WarningsAlarmActions.LogAlarm = checkboxstatus;
        break;
    case WarningsAlarmActions_GeneralAlarm:
        datatoBl.WarningsAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 13:
//        datatoBl.WarningsAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 14:
//        datatoBl.WarningsAlarmActions.CycleCounter = checkboxstatus;
//        break;
    case SuspectAlarmActions_ResetRequired:
        datatoBl.SuspectAlarmActions.ResetRequired = checkboxstatus;
        break;
    case SuspectAlarmActions_LogAlarm:
        datatoBl.SuspectAlarmActions.LogAlarm = checkboxstatus;
        break;
    case SuspectAlarmActions_GeneralAlarm:
        datatoBl.SuspectAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 18:
//        datatoBl.SuspectAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 19:
//        datatoBl.SuspectAlarmActions.CycleCounter = checkboxstatus;
//        break;
    case RejectAlarmActions_ResetRequired:
        datatoBl.RejectAlarmActions.ResetRequired = checkboxstatus;
        break;
    case RejectAlarmActions_LogAlarm:
        datatoBl.RejectAlarmActions.LogAlarm = checkboxstatus;
        break;
    case RejectAlarmActions_GeneralAlarm:
        datatoBl.RejectAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 23:
//        datatoBl.RejectAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 24:
//        datatoBl.RejectAlarmActions.CycleCounter = checkboxstatus;
//        break;
    case NoCycleAlarmActions_ResetRequired:
        datatoBl.NoCycleAlarmActions.ResetRequired = checkboxstatus;
        break;
    case NoCycleAlarmActions_LogAlarm:
        datatoBl.NoCycleAlarmActions.LogAlarm = checkboxstatus;
        break;
    case NoCycleAlarmActions_GeneralAlarm:
        datatoBl.NoCycleAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 28:
//        datatoBl.NoCycleAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 29:
//        datatoBl.NoCycleAlarmActions.CycleCounter = checkboxstatus;
//        break;
    case HardwareFailureAlarmActions_ResetRequired:
        datatoBl.HardwareFailureAlarmActions.ResetRequired = checkboxstatus;
        break;
    case HardwareFailureAlarmActions_LogAlarm:
        datatoBl.HardwareFailureAlarmActions.LogAlarm = checkboxstatus;
        break;
    case HardwareFailureAlarmActions_GeneralAlarm:
        datatoBl.HardwareFailureAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 33:
//        datatoBl.HardwareFailureAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 34:
//        datatoBl.HardwareFailureAlarmActions.CycleCounter = checkboxstatus;
//        break;
    case NonCycleOverloadsAlarmActions_ResetRequired:
        datatoBl.NonCycleOverloadsAlarmActions.ResetRequired = checkboxstatus;
        break;
    case NonCycleOverloadsAlarmActions_LogAlarm:
        datatoBl.NonCycleOverloadsAlarmActions.LogAlarm = checkboxstatus;
        break;
    case NonCycleOverloadsAlarmActions_GeneralAlarm:
        datatoBl.NonCycleOverloadsAlarmActions.GeneralAlarm = checkboxstatus;
        break;
//    case 38:
//        datatoBl.NonCycleOverloadsAlarmActions.CustomAlarmOutput = checkboxstatus;
//        break;
//    case 39:
//        datatoBl.NonCycleOverloadsAlarmActions.CycleCounter = checkboxstatus;
//        break;
    default:
        break;
    }
}

/**
 * @brief AlarmConfiguration::saveToBL : Sends alarm configuration structure to BL
 * @return
 */
bool AlarmConfiguration::saveToBL()
{    
    char ch[4]={'\0'};
    char *data = reinterpret_cast<char*>(&datatoBl);
    saveStatus = true;
    readstructure = datatoBl;

    /* Send message to BL */
    CIObj.sendMessage(SCBL_ALARM_CONFIG_SAVE_REQ,UIC_ALARM_CONFIG_SAVE_RSP,data,sizeof(datatoBl));
    CIObj.recivedMessage(UIC_ALARM_CONFIG_SAVE_RSP,ch,1);
    memcpy(&m_saveResult,ch,1);

    if(m_saveResult == '1')
    {
        return true;
    }
    return false;
}

/**
 * @brief AlarmConfiguration::resetDefaultValues : Restores alarm configuration structure to default
 */
void AlarmConfiguration::resetDefaultValues()
{
    char data[1];
    AlarmManagementData temp;
    char *m_data = reinterpret_cast<char*>(&temp);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_DEFAULT_ALARM_CONFIG_REQ, UIC_DEFAULT_ALARM_CONFIG_RSP, data, sizeof(data));
    CIObj.recivedMessage(UIC_DEFAULT_ALARM_CONFIG_RSP, m_data ,sizeof(datatoBl));

    datatoBl = temp;
    structureCopy(datatoBl);
}

/**
 * @brief AlarmConfiguration::readAlarmconfigDetails : Reads alarm configuration structure from BL
 */
void AlarmConfiguration::readAlarmconfigDetails()
{
    char data[4]={'\0'};
    AlarmManagementData temp;
    char *m_data = reinterpret_cast<char*>(&temp);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_ALARM_CONFIG_READ_REQ, UIC_ALARM_CONFIG_READ_RSP, data, 1);
    CIObj.recivedMessage(UIC_ALARM_CONFIG_READ_RSP, m_data ,sizeof(datatoBl));

    datatoBl = temp;
    tempStructure = temp;
    structureCopy(datatoBl);
}

/**
 * @brief AlarmConfiguration::cancelCurrentDetails : Cancels alarm configuration changes and stores previous values
 */
void AlarmConfiguration::cancelCurrentDetails()
{
    if(saveStatus)
    {
        datatoBl = readstructure;
    }
    else
    {
        datatoBl = tempStructure;
    }
    structureCopy(datatoBl);
}

/**
 * @brief AlarmConfiguration::structureCopy : copies current structure to temp and updates in UI
 * @param datatoBl
 */
void AlarmConfiguration::structureCopy(AlarmManagementData datatoBl )
{
    QList<bool> tempStatusList;
    m_statuslist.clear();
    tempStatusList.clear();

    /* Refreshing UI alarm check box status model*/
    context->setContextProperty("modelCheckBoxStatus", QVariant::fromValue(m_statuslistdummy));

    tempStatusList.append(datatoBl.OverloadsAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.OverloadsAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.OverloadsAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.OverloadsAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.OverloadsAlarmActions.CycleCounter);
    tempStatusList.append(datatoBl.CycleModifiedAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.CycleModifiedAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.CycleModifiedAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.CycleModifiedAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.CycleModifiedAlarmActions.CycleCounter);
    tempStatusList.append(datatoBl.WarningsAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.WarningsAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.WarningsAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.WarningsAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.WarningsAlarmActions.CycleCounter);
    tempStatusList.append(datatoBl.SuspectAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.SuspectAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.SuspectAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.SuspectAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.SuspectAlarmActions.CycleCounter);
    tempStatusList.append(datatoBl.RejectAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.RejectAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.RejectAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.RejectAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.RejectAlarmActions.CycleCounter);
    tempStatusList.append(datatoBl.NoCycleAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.NoCycleAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.NoCycleAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.NoCycleAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.NoCycleAlarmActions.CycleCounter);
    tempStatusList.append(datatoBl.HardwareFailureAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.HardwareFailureAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.HardwareFailureAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.HardwareFailureAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.HardwareFailureAlarmActions.CycleCounter);
    tempStatusList.append(datatoBl.NonCycleOverloadsAlarmActions.ResetRequired);
    tempStatusList.append(datatoBl.NonCycleOverloadsAlarmActions.LogAlarm);
    tempStatusList.append(datatoBl.NonCycleOverloadsAlarmActions.GeneralAlarm);
//    tempStatusList.append(datatoBl.NonCycleOverloadsAlarmActions.CustomAlarmOutput);
//    tempStatusList.append(datatoBl.NonCycleOverloadsAlarmActions.CycleCounter);

    for(int dwIndex=0;dwIndex<tempStatusList.size();dwIndex++)
    {
        Alarmconfigcolumn* alarmchekboxList = new Alarmconfigcolumn;
        alarmchekboxList->m_checkboxstatus=tempStatusList[dwIndex];
        m_statuslist.append(alarmchekboxList);
    }

    /* Updating UI alarm check box status model*/
    context->setContextProperty("modelCheckBoxStatus", QVariant::fromValue(m_statuslist));
}
