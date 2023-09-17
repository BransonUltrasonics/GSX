/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Event Handler

***************************************************************************/
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <QString>
#include <QMap>
#include <QTime>
#include <QDate>
#include <QObject>

//#define DEBUG
class EventHandler:public QObject
{
    Q_OBJECT
public:
    EventHandler();
    bool Init();
    Q_INVOKABLE bool eventSend(int);
    Q_INVOKABLE bool eventSend(int,QString);

protected:
    void send();
    void send(QString);

private:
    //void GetTheSytemDetails();
    QMap<int,QString> EventDescriptor;
    QTime m_time;
    QDate m_date;
    QString m_username="ADMIN";
    QString m_Ps_Serial;
    QString m_eventId;
    QString m_UserComment;

    enum Event{
        EVENT_USER_LOGIN=1,
        EVENT_USER_LOGOUT,
        EVENT_HORN_SCAN_COMPLETED,
        EVENT_HORN_SCAN_ABORTED,
        EVENT_USER_CREATED,
        EVENT_USER_MODIFIED,
        EVENT_AUTHORITY_CHECK_CHANGED,
        EVENT_RECIPE_CREATED,
        EVENT_RECIPE_MODIFIED,
        EVENT_SEEK_SCAN_STARTED,
        EVENT_CYCLE_COUNTER_RESETTED,
        EVENT_SYSTEM_BOARD_REPLACED,
        EVENT_SYSTEM_CLEANED,
        EVENT_SYSYTEM_COMPONENT_FACTORY_RESET,
        EVENT_RECIPE_VALIDATED,
        EVENT_RECIPE_INVALIDATED,
        EVENT_DATE_CHANGED,
        EVENT_PASSWORD_CHANGED,
        EVENT_RECIPE_COPIED,
        EVENT_VALIDATED_CHANGE,
        EVENT_SYSTEM_CALIBRATION,
        EVENT_SYSTEM_HW_CHANGED,
        EVENT_UIC_SOFTWARE_VERSION,
        EVENT_RECIPE_SAVED,
        EVENT_RECIPE_DELETED = 26
    };
    void EventDescription();
};

#endif // EVENTHANDLER_H
