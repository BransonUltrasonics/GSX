/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Event Record

***************************************************************************/
#ifndef EVENTRECORD_H
#define EVENTRECORD_H
#include <QAbstractListModel>
#include "Header/eventlog.h"

//#define DEBUG
#define BUFFER_SIZE 256
#define EVENT_TITLE         "Title"
#define EVENT_TIME          "Time"
#define EVENT_NAME          "Eventtitle"
#define EVENT_USER          "User"
#define EVENT_ID            "ID"
#define EVENT_IDENTIFIER    "Identifier"
#define EVENT_FROM          "From"
#define EVENT_TO            "To"
#define EVENT_PS_SERIAL     "PsSerial"
#define EVENT_ACT_SERIAL    "ActSerial"
#define EVENT_AUX_SERIAL    "AuxSerial"
#define EVENT_COMMENTS      "Comments"

class Eventrecord : public QAbstractListModel
{
    Q_OBJECT
public:
     enum EventData{
        EVENTINDEX = Qt::UserRole+1,
        EVENTTIME,
        EVENTNAME,
        EVENTUSER,
        EVENTID,
        EVENTIDENTIFIER,
        EVENTFROM,
        EVENTTO,
        EVENTPSSERIAL,
        EVENTACTSERIAL,
        EVENTAUXSERIAL,
        EVENTCOMMENTS,
    };

    Eventrecord(QObject *parent = 0);
    virtual ~Eventrecord() {}

    Q_PROPERTY(QString eventDateTime    READ GeteventDateTime   NOTIFY eventDateTimeChanged)
    Q_PROPERTY(QString eventName        READ GeteventName       NOTIFY eventNameChanged)
    Q_PROPERTY(QString eventUser        READ GeteventUser       NOTIFY eventUserChanged)
    Q_PROPERTY(QString eventID          READ GeteventID         NOTIFY eventIDChanged)
    Q_PROPERTY(QString eventIdentifier  READ GeteventIdentifier NOTIFY eventIdentifierChanged)
    Q_PROPERTY(QString eventFrom        READ GeteventFrom       NOTIFY eventFromChanged)
    Q_PROPERTY(QString eventTo          READ GeteventTo         NOTIFY eventToChanged)
    Q_PROPERTY(QString eventPsSerial    READ GeteventPsSerial   NOTIFY eventPsSerialChanged)
    Q_PROPERTY(QString eventActSerial   READ GeteventActSerial  NOTIFY eventActSerialChanged)
    Q_PROPERTY(QString eventAuxSerial   READ GeteventAuxSerial  NOTIFY eventAuxSerialChanged)
    Q_PROPERTY(QString eventUsercmt     READ GeteventUsercmt    NOTIFY eventUsercmtChanged)
    Q_PROPERTY(QString eventHeaderTime  READ GeteventHeaderTime NOTIFY eventHeaderTimeChanged)

    void addevent(EventLog EventInformation);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void getSelectedrowdata(int);
    Q_INVOKABLE void  getEventRecords();

    QString GeteventDateTime();
    QString GeteventName();
    QString GeteventUser();
    QString GeteventID();
    QString GeteventIdentifier();
    QString GeteventFrom();
    QString GeteventTo();
    QString GeteventPsSerial();
    QString GeteventActSerial();
    QString GeteventAuxSerial();
    QString GeteventUsercmt();
    QString GeteventHeaderTime();

signals:
    void eventDateTimeChanged();
    void eventNameChanged();
    void eventUserChanged();
    void eventIDChanged();
    void eventIdentifierChanged();
    void eventFromChanged();
    void eventToChanged();
    void eventPsSerialChanged();
    void eventActSerialChanged();
    void eventAuxSerialChanged();
    void eventUsercmtChanged();
    void eventHeaderTimeChanged();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<EventLog> m_eventList;
    QString eventDateTime;
    QString eventName;
    QString eventUser;
    QString eventID;
    QString eventIdentifier;
    QString eventFrom;
    QString eventTo;
    QString eventPsSerial;
    QString eventActSerial;
    QString eventAuxSerial;
    QString eventHeaderTime;
    QString eventUsercmt;
};

#endif // EVENTRECORD_H
