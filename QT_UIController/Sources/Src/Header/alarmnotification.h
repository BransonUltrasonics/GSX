/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Alarm Management

***************************************************************************/

#ifndef ALARMNOTIFICATION_H
#define ALARMNOTIFICATION_H
#include <QObject>
#include <QQmlListProperty>
#include <QTime>
#include <QDate>
class AlarmNotification : public QObject
{
    Q_OBJECT
public:
    QString m_alarmName;
    QString m_alarmTime;
    QString m_alarmDate;
    int m_resetStatus;
    int m_noOfAlarms;
    QList<QObject*> alarmNotificationList;
    QList<QObject*> alarmNotificationListdummy;

    AlarmNotification(const QString &m_AlarmNameFromDB, const QString &m_AlarmTimeFromDB, const QString &m_AlarmDateFromDB,const int &m_reset, QObject *parent=0);
    explicit AlarmNotification(QObject *parent = 0);

    Q_PROPERTY(int m_Reset READ getReset WRITE setReset NOTIFY OnResetChanged)
    Q_PROPERTY(QString m_AlarmNameFromDB READ GetAlarmNameFromDB WRITE SetAlarmNameFromDB NOTIFY OnAlarmNameFromDBChanged)
    Q_PROPERTY(QString m_AlarmTimeFromDB READ GetAlarmTimeFromDB WRITE SetAlarmTimeFromDB NOTIFY OnAlarmTimeFromDBChanged)
    Q_PROPERTY(QString m_AlarmDateFromDB READ GetAlarmDateFromDB WRITE SetAlarmDateFromDB NOTIFY OnAlarmDateFromDBChanged)

signals:
    Q_INVOKABLE void clearEstopResetPopUpSignal();
public:
    Q_INVOKABLE void alarmInit();
    Q_INVOKABLE void updateAlarmList(int a_Index);
    Q_INVOKABLE void updateAlarmListFromAnalyticsAlarm(QString a_id);
    Q_INVOKABLE QString getAlarmIdAtIndex(int a_index);
    Q_INVOKABLE void fillNotificationList(QString a_AlarmID, QString a_AlarmTime, QString a_AlarmDate,int a_reset);
    Q_INVOKABLE void clearAlarmNotificationList();
    Q_INVOKABLE int getReset();
    Q_INVOKABLE int getAlarmDetails();
    Q_INVOKABLE int getNoOfAlarms();
    QString GetAlarmNameFromDB() const;
    QString GetAlarmTimeFromDB() ;
    QString GetAlarmDateFromDB() ;
    void clearNonResetNotificationPopuplist();
    int storeAlarmDetails(QString a_alarmData);
    bool checkBusScanAlarm(QString a_AlarmID);
    bool checkBusScanAlarmInNotificationList(QString a_alarmData);

public slots:
    void SetAlarmNameFromDB(QString a_AlarmName);
    void SetAlarmTimeFromDB(QString a_AlarmTime);
    void SetAlarmDateFromDB(QString a_AlarmDate);
    void setReset(int a_reset);
    void clearNonResetNotificationlist();

signals:
    void OnAlarmNameFromDBChanged();
    void OnAlarmTimeFromDBChanged();
    void OnAlarmDateFromDBChanged();
    void OnResetChanged();
};
#endif // ALARMNOTIFICATION_H
