/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Reads Alarms from DB and updates in the UI

***************************************************************************/

#include <QQmlContext>
#include "Header/alarmfromsc.h"
#include "Header/alarmnotification.h"
#include "Header/analyticsalarm.h"
#include "Header/communicationinterface.h"
#include "Header/localizeheader.h"
#include "Header/recipe.h"

extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern AlarmFromSC *alarmDetails;
extern AlarmNotification *alarmNotify;

#define ANALY_ALARM_NEW  QObject::tr("Unresolved")
#define ANALY_ALARM_RESET  QObject::tr("Resolved")

/**
 * @brief AnalyticAlarm::GetAnalyticsAlarmdetails : Returns alarm details from DB
 * @param : QString qstrRecipeName
 * @return : QString
 */
QString AnalyticAlarm::GetAnalyticsAlarmdetails(QString qstrRecipeName)
{
    QString a_analyticsAlarmLogBuffer = ANALY_EMPTY_STRING;

    /* Send messgae to DB */
    CIObj.sendMessage(REQ_GET_ANALYTICS_ALARM,RES_GET_ANALYTICS_ALARM,qstrRecipeName);
    CIObj.recivedMessage(RES_GET_ANALYTICS_ALARM,a_analyticsAlarmLogBuffer);

    return a_analyticsAlarmLogBuffer;
}

/**
 * @brief AnalyticAlarm::setAnalyticsAlarmdetails : Sets alarm status to DB
 * @param : QString recipeNo
 * @param : QString alarmStatus
 * @return : QString
 */
QString AnalyticAlarm::setAnalyticsAlarmdetails(QString receiveStatus)
{
    QString a_analyticsAlarmLogBuffer = ANALY_EMPTY_STRING;

    /* Send messgae to DB */
    CIObj.sendMessage(REQ_SET_ANALYTICS_ALARM, RES_SET_ANALYTICS_ALARM,receiveStatus);
    CIObj.recivedMessage(RES_SET_ANALYTICS_ALARM,a_analyticsAlarmLogBuffer);

    return a_analyticsAlarmLogBuffer;
}

/**
 * @brief AnalyticAlarm::clearList : Clears analytics model list
 */
void AnalyticAlarm::clearList()
{
    if (!m_analyticsList.empty())
    {
        auto tempList = m_analyticsList;
        m_analyticsList.clear();

        /* Updating UI analytics model*/
        context->setContextProperty("analyticModel", QVariant::fromValue(m_analyticsList));
        qDeleteAll(tempList);
        tempList.clear();
    }
}

/**
 * @brief AnalyticAlarm::GetRecipeNo : Returns recipe number
 * @return : QString
 */
QString AnalyticAlarm::GetRecipeNo() const
{
    return m_recipeN0;
}

/**
 * @brief AnalyticAlarm::GetAlarmId : Returns alarm ID
 * @return : QString
 */
QString AnalyticAlarm::GetAlarmId() const
{
    return m_alarmId;
}

/**
 * @brief AnalyticAlarm::GetAlarmType : Returns alarm type
 * @return : QString
 */
QString AnalyticAlarm::GetAlarmType() const
{
    return m_alarmType;
}

/**
 * @brief AnalyticAlarm::GetAlarmTime : Returns alarm time
 * @return : QString
 */
QString AnalyticAlarm::GetAlarmTime()
{
    return m_alarmTime.toString("hh:mm:ss");
}

/**
 * @brief AnalyticAlarm::GetAlarmDate : Returns alarm date
 * @return : QString
 */
QString AnalyticAlarm::GetAlarmDate()
{
    return m_alarmDate.toString(Qt::ISODate);
}

/**
 * @brief AnalyticAlarm::GetAlarmStatus : Returns alarm status
 * @return : QString
 */
QString AnalyticAlarm::GetAlarmStatus()
{
    return m_alarmStatus;
}

/**
 * @brief AnalyticAlarm::GetAlarmDescription : Returns alarm description
 * @return : QString
 */
QString AnalyticAlarm::GetAlarmDescription()
{
    return m_alarmDescription;
}

/**
 * @brief AnalyticAlarm::GetAlarmName
 * @return
 */
QString AnalyticAlarm::GetAlarmName()
{
    return m_alarmName;
}

/**
 * @brief AnalyticAlarm::GetErrorType : Returns alarm type
 * @return : QString
 */
QString AnalyticAlarm::GetErrorType()
{
    return m_errorType;
}

/**
 * @brief AnalyticAlarm::GetCycleCounter : Returns cycle counter
 * @return : QString
 */
QString AnalyticAlarm::GetCycleCounter()
{
    return m_cycleCounter;
}

/**
 * @brief AnalyticAlarm::GetRecipeVerNo : Returns recipe version number
 * @return : QString
 */
QString AnalyticAlarm::GetRecipeVerNo()
{
    return m_recipeVerNo;
}

/**
 * @brief AnalyticAlarm::GetUserName : Return Username
 * @return : QString
 */
QString AnalyticAlarm::GetUserName()
{
    return m_userName;
}

/**
 * @brief AnalyticAlarm::setAlarmStatus : Sets alarm status for particular recipe
 * @param : QString recipeNo
 * @param : QString alarmstatus
 */
void AnalyticAlarm::setAlarmStatus(QString recipeNo,QString alarmstatus)
{
    QString sendstr = recipeNo + ANALY_SEPERATOR + alarmstatus;
    setAnalyticsAlarmdetails(sendstr);
}

/**
 * @brief AnalyticAlarm::GetReset : Returns alarm reset status
 * @return : int
 */
int AnalyticAlarm::GetReset()
{
    return m_reset;
}

/**
 * @brief AnalyticAlarm::AnalyticAlarm : Constructor
 * @param : QObject parent
 */
AnalyticAlarm::AnalyticAlarm(QObject *parent) : QObject(parent)
{
    config = Configuration::getInstance();
    //Get the Database version and if it´s greater than version 2 it expects
    //a buffer size of 9, otherwise (if Database version is 1) it expects 8.
    if(config->getDatabaseVersion() >= 2){
        m_expectedBufferSize = 9;
    }
    else{
        m_expectedBufferSize = 8;
    }
    context->setContextProperty("analyticModel", QVariant::fromValue(m_analyticsList));
}

/**
 * @brief AnalyticAlarm::AnalyticAlarm :  Initializes the alarms model list
 * @param : QString recipeN0
 * @param : QString alarmId
 * @param : QString alarmType
 * @param : QTime alarmTime
 * @param : Qdate alarmDate
 * @param : QString alarmStatus
 * @param : QString cycleCounter
 * @param : QString recipeVerNo
 * @param : QString errorType
 * @param : QString alarmDesc
 * @param : QString alarmName
 * @param : int reset
 */

AnalyticAlarm::AnalyticAlarm(QString recipeN0, QString alarmId, QString alarmType,
                             QTime alarmTime, QDate alarmDate, QString alarmStatus,
                             QString cycleCounter, QString recipeVerNo, 
                             QString errorType, QString alarmDesc,
                             QString alarmName, QString userName, int reset)
    : m_recipeN0(recipeN0)
    ,m_alarmId(alarmId)
    ,m_alarmType(alarmType)
    ,m_alarmTime(alarmTime)
    ,m_alarmDate(alarmDate)
    ,m_alarmStatus(alarmStatus)
    ,m_cycleCounter(cycleCounter)
    ,m_recipeVerNo(recipeVerNo)
    ,m_errorType(errorType)
    ,m_alarmDescription(alarmDesc)
    ,m_alarmName(alarmName)
    ,m_userName(userName)
    ,m_reset(reset)
{
    config = Configuration::getInstance();
    //Get the Database version and if it´s greater than version 2 it expects
    //a buffer size of 9, otherwise (if Database version is 1) it expects 8.
    if(config->getDatabaseVersion() >= 2){
        m_expectedBufferSize = 9;
    }
    else{
        m_expectedBufferSize = 8;
    }
    context->setContextProperty("analyticModel", QVariant::fromValue(m_analyticsList));
}

/**
 * @brief AnalyticAlarm::updateAnalyticsAlarmsList : Updates alarms list read from DB
 */
void AnalyticAlarm::updateAnalyticsAlarmsList()
{
    QString alarmResult = GetAnalyticsAlarmdetails(ANALY_EMPTY_STRING);
    if(!alarmResult.isEmpty())
    {
        updateAlarmsList(alarmResult);
    }
}

/**
 * @brief AnalyticAlarm::analyticsAlarmLogs : Updates alarm details in the list and updates in UI
 * @param : QStringList m_alarmBuffer
 */
void AnalyticAlarm::analyticsAlarmLogs (QStringList m_alarmBuffer)
{
    QString compareAlarmId = alarmDetails->getAlarmId();
    QRegExp separator(" ");
    int dwIndex,size=0;
    size = m_alarmBuffer.size();
    m_compareAlarmIdList.append(compareAlarmId);
    for (dwIndex = 0;dwIndex < size;)
    {
        recipeNo = m_alarmBuffer[dwIndex];
        alarmId = m_alarmBuffer[dwIndex+1];
        alarmType = m_alarmBuffer[dwIndex+2];
        errorType = m_alarmBuffer[dwIndex+2];
        cycleCounter = m_alarmBuffer[dwIndex+4];
        recipeVerNo =m_alarmBuffer[dwIndex+5];
        //If the Database version is 1, just ignore the 9th element (userName)
        if(!(config->getDatabaseVersion() == ANALY_NOUSERNAME_DBVERSION))
            userName = m_alarmBuffer[dwIndex+8];
        QStringList firstList = m_alarmBuffer[dwIndex+3].split(separator);
        alarmbufTime = QTime::fromString(firstList.takeLast(),"HH:mm:ss");
        alarmbufDate = QDate::fromString(firstList.takeFirst(),Qt::ISODate);
        m_alarmDescription = alarmDetails->errorDescforAlarm(alarmId);
        m_alarmName = alarmDetails->errorNameforAlarm(alarmId);

        if (alarmType == ANALY_ALARM_TYPE_1)
        {
            alarmType = ANALY_PART_ICON_IMG;
            errorType =  errorType;
        }
        else
        {
            alarmType = ANALY_MACHINE_IMG;
            errorType =  errorType;
        }

        m_reset = 0;
        alarmStatus = ANALY_ALARM_RESET;

        for(int j=0;j<alarmNotify->alarmNotificationList.size();j++)
        {
            AlarmNotification *alarmnotificationTemp = dynamic_cast<AlarmNotification*>(alarmNotify->alarmNotificationList.at(j));
            QString tempdata=alarmnotificationTemp->m_alarmName;
            if(alarmId == tempdata)
            {
                m_reset = 1;
                alarmStatus = ANALY_ALARM_NEW;
                clearAlaramList();
            }
        }
        m_analyticsList.append(new AnalyticAlarm(recipeNo, alarmId, alarmType, alarmbufTime,
                                                 alarmbufDate, alarmStatus, cycleCounter, recipeVerNo,
                                                 errorType, m_alarmDescription,
                                                 m_alarmName, userName, m_reset));

        dwIndex = dwIndex + m_expectedBufferSize;
    }

    if(m_analyticsList.size())
    {
        /* Updating UI analytics model*/
        context->setContextProperty("analyticModel", QVariant::fromValue(m_analyticsList));
    }
}

/**
 * @brief AnalyticAlarm::clearAlaramList : Clears the alarms in the list
 */
void AnalyticAlarm::clearAlaramList()
{
    auto tempList = m_compareAlarmIdList;
    m_compareAlarmIdList.clear();
    tempList.clear();
}

/**
 * @brief AnalyticAlarm::updateAlarmsList : Updates alarm list
 * @param alarmLog
 */
void AnalyticAlarm::updateAlarmsList(QString alarmLog)
{
    QStringList alarmBuffer = alarmLog.split(ANALY_SEPERATOR);
    clearList();
    if((alarmBuffer.size() % m_expectedBufferSize) == 0)
    {
        analyticsAlarmLogs(alarmBuffer);
    }
}
