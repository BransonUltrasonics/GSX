/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Analytics Alarm from DB

***************************************************************************/
#ifndef ANALYTICSALARM_H
#define ANALYTICSALARM_H
#include <QObject>
#include <QDate>
#include <QTime>
#include "Header/communicationinterface.h"
#include "Header/configuration.h"

#define ANALY_ALARM_NEW             QObject::tr("Unresolved")
#define ANALY_ALARM_RESET           QObject::tr("Resolved")
#define ANALY_ALARM_TYPE_1          "1"
#define ANALY_SEPERATOR             ","
#define ANALY_EMPTY_STRING          ""
#define ANALY_PART_ICON_IMG         "qrc:/images/image/part-icon.svg"
#define ANALY_MACHINE_IMG           "qrc:/images/image/machine.svg"
#define ANALY_NOUSERNAME_DBVERSION    "1"

class AnalyticAlarm: public QObject
{
    Q_OBJECT
public:
    QList<QObject*> m_analyticsList;
    QStringList m_compareAlarmIdList;
    QList<QObject*> m_analyticsListDummy;

    Q_PROPERTY(QString RecipeNo READ GetRecipeNo NOTIFY OnRecipeNoChanged )
    Q_PROPERTY(QString AlarmId READ GetAlarmId NOTIFY OnAlarmIdChanged )
    Q_PROPERTY(QString ErrorType READ GetErrorType NOTIFY OnErrorTypeChanged)
    Q_PROPERTY(QString AlarmType READ GetAlarmType NOTIFY OnAlarmTypeChanged)
    Q_PROPERTY(QString AlarmTime READ GetAlarmTime NOTIFY OnAlarmTimeChanged)
    Q_PROPERTY(QString AlarmDate READ GetAlarmDate NOTIFY OnAlarmDateChanged)
    Q_PROPERTY(QString AlarmStatus READ GetAlarmStatus NOTIFY OnAlarmStatusChanged)
    Q_PROPERTY(QString CycleCounter READ GetCycleCounter NOTIFY OnCycleCounterChanged)
    Q_PROPERTY(QString RecipeVerNo READ GetRecipeVerNo NOTIFY OnRecipeVerNoChanged)
    Q_PROPERTY(QString AlarmDescription READ GetAlarmDescription NOTIFY OnAlarmDescriptionChanged)
    Q_PROPERTY(QString AlarmName READ GetAlarmName NOTIFY OnAlarmNameChanged)
    Q_PROPERTY(QString Username READ GetUserName NOTIFY OnUserNameChanged)
    Q_PROPERTY(int Reset READ GetReset NOTIFY OnResetChanged)

    explicit AnalyticAlarm(QObject *parent = 0);
    AnalyticAlarm(QString recipeN0, QString alarmId, QString alarmType, QTime alarmTime,
                  QDate alarmDate, QString alarmStatus, QString cycleCounter, QString recipeVerNo,
                  QString errorType, QString alarmDesc, QString alarmName, QString userName, int reset);

    void analyticsAlarmLogs(QStringList);
    void updateAlarmsList(QString alarmLog);
    void errorNameforAlarm(int alarmId);
    QString GetAnalyticsAlarmdetails(QString);
    QString setAnalyticsAlarmdetails(QString receiveStatus);
    QString getAnalyticsResultsFilterDataFromDB(QString param);
    Q_INVOKABLE void updateAnalyticsAlarmsList();

signals:
public:
    QString GetRecipeNo() const;
    QString GetAlarmId() const;
    QString GetAlarmType() const;
    QString GetAlarmTime();
    QString GetAlarmDate() ;
    QString GetAlarmStatus();
    QString GetAlarmDescription();
    QString GetAlarmName();
    QString GetErrorType();
    QString GetCycleCounter();
    QString GetRecipeVerNo();
    QString GetUserName();
    int GetReset();
    Q_INVOKABLE void setAlarmStatus(QString recipeNo,QString alarmstatus);
   // Q_INVOKABLE void updateAnalyticsAlarmFilterData(QString param);

signals:
    void OnRecipeNoChanged();
    void OnAlarmIdChanged();
    void OnAlarmTypeChanged();
    void OnAlarmTimeChanged();
    void OnAlarmDateChanged();
    void OnAlarmStatusChanged();
    void OnCycleCounterChanged();
    void OnRecipeVerNoChanged();
    void OnErrorTypeChanged();
    void OnAlarmDescriptionChanged();
    void OnAlarmNameChanged();
    void OnResetChanged();
    void OnUserNameChanged();

private:    
    QString m_recipeN0;
    QString m_alarmId;
    QString m_alarmType;
    QTime m_alarmTime;
    QDate m_alarmDate;
    QString m_alarmStatus;
    QString m_cycleCounter;
    QString m_recipeVerNo;
    QString m_errorType;
    QString m_alarmDescription;
    QString m_alarmName;
    QString m_userName;
    int m_reset;
    int m_expectedBufferSize;
    QString recipeNo;
    QString alarmId;
    QString alarmType;
    QString errorType;
    QString alarmStatus;
    QString cycleCounter;
    QString recipeVerNo;
    QString pcAssembly;
    QString acAssembly;
    QStringList firstList;
    QTime alarmbufTime;
    QDate alarmbufDate;
    QString userName;
    Configuration *config;

    void clearList();
    void clearAlaramList();

};
#endif // ANALYTICSALARM_H
