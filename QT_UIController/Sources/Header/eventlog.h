/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Event Log

***************************************************************************/
#ifndef EVENTLOG_H
#define EVENTLOG_H
#include <QObject>
#include <QDate>
#include <QTime>

class EventLog
{
public:
    QString m_eventIndex;
    QString m_eventTime;
    QString m_eventName;
    QString m_eventUser;
    QString m_eventID;
    QString m_eventIdentifier;
    QString m_eventFrom;
    QString m_eventTo;
    QString m_eventPsSerial;  //PS SN
    QString m_eventActSerial; //Act SN
    QString m_eventAuxSerial; //AUX SN
    QString m_eventComments;

    EventLog(const QString& eventIndex, const QString& eventTime, const QString& eventName,
             const QString& eventUser, const QString& eventPSSerial, const QString& eventActSerial);
    EventLog(const QString& eventIndex, const QString& eventTime, const QString& eventName,
             const QString& eventUser, const QString& eventID, const QString& eventIdentifier,
             const QString& eventFrom, const QString& eventTo, const QString& eventPSSerial,
             const QString& eventActSerial, const QString& eventAuxSerial, const QString& eventComments);

    QString GeteventIndex() const;
    QString GeteventTime() const;
    QString GeteventName() const;
    QString GeteventUser() const;
    QString GeteventID() const;
    QString GeteventIdentifier() const;
    QString GeteventFrom() const;
    QString GeteventTo() const;
    QString GeteventPsSerial() const;
    QString GeteventActSerial() const;
    QString GeteventAuxSerial() const;
    QString GeteventComments() const;

};
#endif // EVENTLOG_H
