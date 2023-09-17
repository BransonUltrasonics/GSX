/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to process the alarms received from BL to UI
    (Notifications tab)

***************************************************************************/

#include <QQmlContext>
#include <QQuickView>
#include <string>
#include "Header/alarmnotification.h"
#include "Header/communicationinterface.h"
#include "Header/sysconfigdata.h"
#include "Header/recipe.h"

extern QObject * object;
extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern Recipe *recipe;
/**
 * @brief AlarmNotification::AlarmNotification : Constructor
 * @param : QObject parent
 */
AlarmNotification::AlarmNotification(QObject *parent) : QObject(parent)
{
    m_noOfAlarms=0;

    /* Connecting a signal clearNonResetAlarms from CommunicationInterface.cpp to clearNonResetNotificationlist slot */
    QObject::connect(&CIObj , &CommunicationInterface::clearNonResetAlarms , this ,&AlarmNotification :: clearNonResetNotificationlist );
}

/**
 * @brief AlarmNotification::alarmInit : Initializes the alarms model list
 */
void AlarmNotification::alarmInit()
{
    /* Updating UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationList));
}

/**
 * @brief AlarmNotification::fillNotificationList : Fills the alarms model list from full
 *                                                  screen alarm popup when clicked on hide
 * @param : QString a_AlarmID
 * @param : QString a_AlarmTime
 * @param : QString a_AlarmDate
 * @param : QString a_resetStatus
 */
void AlarmNotification::fillNotificationList(QString a_AlarmID, QString a_AlarmTime, QString a_AlarmDate,int a_resetStatus)
{
    /* Refreshing UI alarm notification model */
    if(checkBusScanAlarm(a_AlarmID))
    {
        context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationListdummy));
        m_alarmName = a_AlarmID;
        m_alarmTime = a_AlarmTime;
        m_alarmDate = a_AlarmDate;
        m_resetStatus = a_resetStatus;
        alarmNotificationList.append(new AlarmNotification(m_alarmName,m_alarmTime,m_alarmDate,m_resetStatus));
        m_noOfAlarms = alarmNotificationList.size();
        /* Updating UI alarm notification model */
        context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationList));
    }

}
/**
 * @brief AlarmNotification::storeAlarmDetails : Stores alarms received from BL before login
 * @param : QString a_alarmData
 * @return : int
 */
int AlarmNotification::storeAlarmDetails(QString a_alarmData)
{
    QString alarmId,tmpId;
    int m_NoOfAlarms;
    char reset;
    QStringList alarmList ;
    SysConfigData *sysconfig = SysConfigData::getInstance();

    /* Refreshing UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationListdummy));

    /* Process if alarm data is not empty */
    if( !a_alarmData.isEmpty() && a_alarmData != "0")
    {
        alarmList =  a_alarmData.split(",");
        for(int dwIndex = 0; dwIndex < alarmList.size(); dwIndex++)
        {
            tmpId = alarmList[dwIndex];
            reset = QString(tmpId).toStdString()[0];
            alarmId.clear();
            for(int dwIndex1 = 1; dwIndex1 < tmpId.count(); dwIndex1++)
            {
                alarmId.append(tmpId[dwIndex1]);
            }
            if(alarmId == "700")
            {
                CIObj.raiseEmergencyStop();
            }
            else if(checkBusScanAlarm(alarmId))
            {
                m_resetStatus = reset - 48;
                m_alarmName = alarmId;
                m_alarmDate = sysconfig->getDateAndTime().toString("yyyy/MM/dd");
                m_alarmTime = sysconfig->getDateAndTime().toString("hh:mm:ss");
                alarmNotificationList.append(new AlarmNotification(m_alarmName,m_alarmTime,m_alarmDate,m_resetStatus));
            }
        }
    }

    /* Updating UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationList));
    m_NoOfAlarms = alarmNotificationList.size();
    m_noOfAlarms = m_NoOfAlarms;
    return m_NoOfAlarms;
}

/**
 * @brief AlarmNotification::clearAlarmNotificationList : Clears all the alarms in notifications
 */
void AlarmNotification::clearAlarmNotificationList()
{
    /* Refreshing UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationListdummy));
    if(alarmNotificationList.size() >= 1)
    {
        for(int element=0; element < alarmNotificationList.size() ; element++)
        {
            AlarmNotification *alarmnotificationTemp = dynamic_cast<AlarmNotification*>(alarmNotificationList.at(element));
            delete alarmnotificationTemp;
            alarmnotificationTemp = NULL ;
        }
    }
    alarmNotificationList.clear();
    m_noOfAlarms = alarmNotificationList.size();

    /* Updating UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationList));
}
/**
 * @brief AlarmNotification::getAlarmIdAtIndex
 * @param a_index
 * @return
 */
QString AlarmNotification::getAlarmIdAtIndex(int a_index)
{
    QString a_alarmId;
    if(alarmNotificationList.size() != 0)
    {
        AlarmNotification *alarmnotificationTemp = dynamic_cast<AlarmNotification*>(alarmNotificationList.at(a_index));
        a_alarmId = alarmnotificationTemp->m_alarmName;
    }
    return a_alarmId;
}
/**
 * @brief AlarmNotification::updateAlarmList : Updates or deletes the particular alarm from notification list when,
 *                                             clicks on Reset in full screen alarm popup
 * @param : int a_index
 */
void AlarmNotification::updateAlarmList(int a_index)
{
    /* Refreshing UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationListdummy));
    if(a_index < alarmNotificationList.size() )
    {
        AlarmNotification *alarmnotificationTemp = dynamic_cast<AlarmNotification*>(alarmNotificationList.at(a_index));
        delete alarmnotificationTemp;
        alarmnotificationTemp = NULL ;
        alarmNotificationList.removeAt(a_index);
    }
    m_noOfAlarms = alarmNotificationList.size();

    /* Updating UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationList));
}

/**
 * @brief AlarmNotification::updateAlarmListFromAnalyticsAlarm : Updates or deletes the particular alarm from notification list when,
 *                                                               clicks on Reset in analytics alarms
 * @param : QString a_id
 */
void AlarmNotification::updateAlarmListFromAnalyticsAlarm(QString a_id)
{
    int dwListSize = alarmNotificationList.size();

    /* Refreshing UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationListdummy));
    for(int dwIndex = 0; dwIndex < dwListSize ;dwIndex++ )
    {
        AlarmNotification *alarmnotificationTemp = dynamic_cast<AlarmNotification*>(alarmNotificationList.at(dwIndex));
        if(alarmnotificationTemp->m_alarmName == a_id)
        {
            delete alarmnotificationTemp;
            alarmnotificationTemp = NULL ;
            alarmNotificationList.removeAt(dwIndex);
            break;
        }
        dwListSize = alarmNotificationList.size();
    }
    m_noOfAlarms = alarmNotificationList.size();

    /* Updating UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationList));
}

/**
 * @brief AlarmNotification::clearNonResetNotificationPopuplist : Clears non resettable alarms in notifications
 */
void AlarmNotification::clearNonResetNotificationPopuplist()
{
    /* Refreshing UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationListdummy));
    if(alarmNotificationList.size() >= 1)
    {
        for(int element=0; element < alarmNotificationList.size() ; )
        {
            AlarmNotification *alarmNotifyObj = dynamic_cast<AlarmNotification*>(alarmNotificationList.at(element));
            if(alarmNotifyObj->m_resetStatus == 0)
            {
                delete alarmNotifyObj;
                alarmNotifyObj = NULL ;
                alarmNotificationList.removeAt(element);
            }
            else
                element++;
        }
    }
    m_noOfAlarms = alarmNotificationList.size();

    /* Updating UI alarm notification model */
    context->setContextProperty("alarmNotificationListModel", QVariant::fromValue(alarmNotificationList));
}

/**
 * @brief AlarmNotification::getAlarmDetails : Gets active alarms from BL and stores in notifications before login
 * @return : int
 */
int AlarmNotification::getAlarmDetails()
{
    QString alarmData="";
    char buff[] = "";
    int m_NoOfAlarms=0;

    /* Send request to BL */
    CIObj.sendMessage(SCBL_ACTIVE_ALARM_REQ ,UIC_ACTIVE_ALARM_RES,buff,0);
    char scStatus[1024];
    CIObj.recivedMessage(UIC_ACTIVE_ALARM_RES,scStatus,sizeof(scStatus));

    alarmData.append(scStatus);
    if(alarmData!="")
    {
        m_NoOfAlarms = storeAlarmDetails(alarmData);
    }
    return m_NoOfAlarms;
}

/**
 * @brief AlarmNotification::clearNonResetNotificationlist : Clears non reset alarms and updates in UI. When
 *                                                           signal received from BL
 */
void AlarmNotification::clearNonResetNotificationlist()
{
    clearNonResetNotificationPopuplist();
    //To reset "E-stop RESET" pop up message
    emit clearEstopResetPopUpSignal();

    /* Invokes a qml function alarmNotificationPopup */
    QMetaObject::invokeMethod(object,"alarmNotificationPopup");
}

/**
 * @brief AlarmNotification::AlarmNotification : Initialization of class members and memory for object
 * @param : QString m_AlarmNameFromDB
 * @param : QString m_AlarmTimeFromDB
 * @param : QString m_AlarmDateFromDB
 * @param : int m_reset
 * @param : QObject parent
 */
AlarmNotification::AlarmNotification(const QString &m_AlarmNameFromDB,const QString &m_AlarmTimeFromDB,const QString &m_AlarmDateFromDB,const int &m_reset,QObject *parent)
    : QObject(parent), m_alarmName(m_AlarmNameFromDB), m_alarmTime(m_AlarmTimeFromDB),m_alarmDate(m_AlarmDateFromDB),m_resetStatus(m_reset)
{

}

/**
 * @brief AlarmNotification::getNoOfAlarms : Returns number of alarms received
 * @return : int
 */
int AlarmNotification::getNoOfAlarms()
{
    return  m_noOfAlarms;
}

/**
 * @brief AlarmNotification::GetAlarmNameFromDB : Returns alarm name read from DB
 * @return : QString
 */
QString AlarmNotification::GetAlarmNameFromDB() const
{
    return m_alarmName;
}

/**
 * @brief AlarmNotification::SetAlarmNameFromDB : Sets alarm name read from DB
 * @param : QString a_AlarmName
 */
void AlarmNotification::SetAlarmNameFromDB(QString a_AlarmName)
{
    if(m_alarmName != a_AlarmName)
    {
        m_alarmName = a_AlarmName;
        emit OnAlarmNameFromDBChanged();
    }
}

/**
 * @brief AlarmNotification::GetAlarmTimeFromDB : Returns alarm time read from DB
 * @return  :QString
 */
QString AlarmNotification::GetAlarmTimeFromDB()
{
    return m_alarmTime;
}

/**
 * @brief AlarmNotification::SetAlarmTimeFromDB : Sets alarm time read from DB
 * @param : QString a_AlarmTime
 */
void AlarmNotification::SetAlarmTimeFromDB(QString a_AlarmTime)
{
    if(m_alarmTime != a_AlarmTime)
    {
        m_alarmTime = a_AlarmTime;
        emit OnAlarmTimeFromDBChanged();
    }
}

/**
 * @brief AlarmNotification::GetAlarmDateFromDB : Returns alarm date read from DB
 * @return : QString
 */
QString AlarmNotification::GetAlarmDateFromDB()
{
    return m_alarmDate;
}

/**
 * @brief AlarmNotification::SetAlarmDateFromDB : Sets alarm date read from DB
 * @param : QString a_AlarmDate
 */
void AlarmNotification::SetAlarmDateFromDB(QString a_AlarmDate)
{
    if(m_alarmDate != a_AlarmDate)
    {
        m_alarmDate = a_AlarmDate;
        emit OnAlarmDateFromDBChanged();
    }
}

/**
 * @brief AlarmNotification::getReset : Returns alarm reset status
 * @return : int
 */
int AlarmNotification::getReset()
{
    return m_resetStatus;
}

/**
 * @brief AlarmNotification::setReset : Sets alarm reset status
 * @param : int a_reset
 */
void AlarmNotification::setReset(int a_reset)
{
    if(m_resetStatus != a_reset)
    {
        m_resetStatus = a_reset;
        emit OnResetChanged();
    }
}
/**
 * @brief AlarmNotification::checkBusScanAlarmInNotificationList
 * @param a_alarmData
 * @return
 */
bool AlarmNotification::checkBusScanAlarmInNotificationList(QString a_alarmData)
{
    QString alarmId,tmpId;
    QStringList alarmList ;

    if( !a_alarmData.isEmpty() && a_alarmData != "0")
    {
        alarmList =  a_alarmData.split(",");
        tmpId = alarmList[0];
        alarmId.clear();
        for(int dwIndex1 = 1; dwIndex1 < tmpId.count();dwIndex1++)
        {
            alarmId.append(tmpId[dwIndex1]);
        }
        return checkBusScanAlarm(alarmId);
    }
    return false;
}

/**
 * @brief AlarmNotification::checkBusScanAlarm
 * @param a_AlarmID
 * @return
 */
bool AlarmNotification::checkBusScanAlarm(QString a_AlarmID)
{
    bool bNotDuplicateAlarm = true;
    if(a_AlarmID == "63e" || a_AlarmID == "63E")
    {
        for(int dwIndex = 0; dwIndex < alarmNotificationList.size();dwIndex++)
        {
            AlarmNotification *alarmnotificationTemp = dynamic_cast<AlarmNotification*>(alarmNotificationList.at(dwIndex));
            if((alarmnotificationTemp->m_alarmName == "63e") || (alarmnotificationTemp->m_alarmName == "63E"))
            {
                recipe->updateStatusText("Internal communication failure. System Halted. Power OFF and Power ON the system");
                bNotDuplicateAlarm = false;
            }
        }
        if(bNotDuplicateAlarm)
            recipe->updateStatusText("Internal communication failure. Attempting to recover");
    }
    return bNotDuplicateAlarm;
}
