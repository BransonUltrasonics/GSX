/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Event Record

***************************************************************************/
#include "Header/eventrecord.h"
#include "Header/eventlog.h"
#include <QQmlContext>
#include <Header/communicationinterface.h>
#include <QQmlContext>

extern QQmlContext* context;

extern CommunicationInterface CIObj;
extern Eventrecord *record1;
Eventrecord record2;


/**
 * @brief Eventrecord::Eventrecord : constructor ,Initializing the EventRecord
 * @param parent
 */
Eventrecord::Eventrecord(QObject *parent)
    : QAbstractListModel(parent)
{
}

/**
 * @brief Eventrecord::addevent : Adding the event description as table
 * @param EventInformation
 */
void Eventrecord::addevent(EventLog EventInformation)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_eventList<<EventInformation;
    endInsertRows();
}

/**
 * @brief Eventrecord::rowCount : Function overriding for the MOC compiler
 * @param parent
 * @return
 */
int Eventrecord::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_eventList.count();
}

/**
 * @brief Eventrecord::data : adding the data into table
 * @param index
 * @param role
 * @return
 */
QVariant Eventrecord::data(const QModelIndex &index, int role) const
{

    if(index.row()<0 || index.row()>=m_eventList.count())
        return QVariant();

    const EventLog &eventlogobject=m_eventList[index.row()];

    QVariant tmpData;
    switch (role)
    {
    case EVENTINDEX:
        tmpData = eventlogobject.GeteventIndex();
        break;
    case EVENTTIME:
        tmpData = eventlogobject.GeteventTime();
        break;
    case EVENTNAME:
        tmpData = eventlogobject.GeteventName();
        break;
    case EVENTUSER:
        tmpData = eventlogobject.GeteventUser();
        break;
    case EVENTID:
        tmpData = eventlogobject.GeteventID();
        break;
    case EVENTIDENTIFIER:
        tmpData = eventlogobject.GeteventIdentifier();
        break;
    case EVENTFROM:
        tmpData = eventlogobject.GeteventFrom();
        break;
    case EVENTTO:
        tmpData = eventlogobject.GeteventTo();
        break;
    case EVENTPSSERIAL:
        tmpData = eventlogobject.GeteventPsSerial();
        break;
    case EVENTACTSERIAL:
        tmpData = eventlogobject.GeteventActSerial();
        break;
    case EVENTAUXSERIAL:
        tmpData = eventlogobject.GeteventAuxSerial();
        break;
    case EVENTCOMMENTS:
        tmpData = eventlogobject.GeteventComments();
        break;
    default:
        tmpData = QVariant();
        break;
    }
    return tmpData;
}

/**
 * @brief Eventrecord::roleNames : Defining the table header
 * @return
 */
QHash<int, QByteArray> Eventrecord::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[EVENTINDEX]       = EVENT_TITLE;
    roles[EVENTTIME]        = EVENT_TIME;
    roles[EVENTNAME]        = EVENT_NAME;
    roles[EVENTUSER]        = EVENT_USER;
    roles[EVENTID]          = EVENT_ID;
    roles[EVENTIDENTIFIER]  = EVENT_IDENTIFIER;
    roles[EVENTFROM]        = EVENT_FROM;
    roles[EVENTTO]          = EVENT_TO;
    roles[EVENTPSSERIAL]    = EVENT_PS_SERIAL;
    roles[EVENTACTSERIAL]   = EVENT_ACT_SERIAL;
    roles[EVENTAUXSERIAL]   = EVENT_AUX_SERIAL;
    roles[EVENTCOMMENTS]    = EVENT_COMMENTS;

    return roles;
}

/**
 * @brief Eventrecord::getSelectedrowdata : On Row selection giving the data in to the EventLogPopUp
 * @param index
 */
void Eventrecord::getSelectedrowdata(int index)
{
    record2.eventDateTime   = m_eventList[index].m_eventTime;
    record2.eventName       = m_eventList[index].m_eventName;
    record2.eventUser       = m_eventList[index].m_eventUser;
    record2.eventID         = m_eventList[index].m_eventID;
    record2.eventIdentifier = m_eventList[index].m_eventIdentifier;
    record2.eventFrom       = m_eventList[index].m_eventFrom;
    record2.eventTo         = m_eventList[index].m_eventTo;
    record2.eventPsSerial   = m_eventList[index].m_eventPsSerial;
    record2.eventActSerial  = m_eventList[index].m_eventActSerial;
    record2.eventAuxSerial  = m_eventList[index].m_eventAuxSerial;
    record2.eventUsercmt    = m_eventList[index].m_eventComments;

    record2.eventHeaderTime  = m_eventList[0].m_eventTime;

    context -> setContextProperty("userList1Model2" , &record2);
}

/**
 * @brief Eventrecord::getEventRecords : On the load of event log screen getting the data from the database
 */
void  Eventrecord::getEventRecords()
{
    QString a_activeRecipeBuffer;
    QString errormsg;
    int index=0;
    int size=0;

    m_eventList.clear();
    CIObj.sendMessage(REQ_GET_EVENTS_LOG,RES_GET_EVENTS_LOG,errormsg);
    CIObj.recivedMessage(RES_GET_EVENTS_LOG,a_activeRecipeBuffer);
//	/**********************************Test Data*********************************/
//    a_activeRecipeBuffer = "3,RECIPE SAVED*,2019/10/16 10:45:06,DEF12345,DEF1234,DEF12346,N/A,14,N/A,0,1,Hello world,"
//                           "2,RECIPE CREATED*,2019/10/16 10:45:02,DEF12345,DEF1234,DEF12346,ConsoleUser,13, ,0,1,Hello world,"
//                           "1,SEEK*,2019/10/16 10:43:49,DEF12345,DEF1234,DEF12346,ConsoleUser,10,Recipe 1000,0,20004,Hello world";
//	/********************************************************************************/
    QStringList m_activeBuffer= a_activeRecipeBuffer.split(",");

    size = m_activeBuffer.size();
    index = size/12;
    if((size%12)==0 && a_activeRecipeBuffer != "")
    {
        for(int integer = 0; integer<size ; integer=integer+12)
        {
            //addevent(EventLog(QString::number(index),           //eventIndex
              addevent(EventLog(m_activeBuffer.at(0+integer),   //eventEvent#
                              m_activeBuffer.at(2+integer),     //eventTime
                              m_activeBuffer.at(1+integer),     //eventName
                              m_activeBuffer.at(6+integer),     //eventUser
                              m_activeBuffer.at(7+integer),     //eventID
                              m_activeBuffer.at(8+integer),     //eventIdentifier
                              m_activeBuffer.at(9+integer),     //eventFrom
                              m_activeBuffer.at(10+integer),     //eventTo
                              m_activeBuffer.at(3+integer),     //eventData1
                              m_activeBuffer.at(4+integer),     //eventData2
                              m_activeBuffer.at(5+integer),     //eventData3
                              m_activeBuffer.at(11+integer)));  //eventComments
            index--;
        }
        context -> setContextProperty("userList1Model" , record1);
    }

    a_activeRecipeBuffer.clear();
}

/**
 * @brief Eventrecord::GeteventUser : Returns event user
 * @return
 */
QString Eventrecord::GeteventUser()
{
    return eventUser;
}

/**
 * @brief Eventrecord::GeteventID : Returns event ID
 * @return
 */
QString Eventrecord::GeteventID()
{
    return eventID;
}

/**
 * @brief Eventrecord::GeteventIdentifier : Returns event Identifier
 * @return
 */
QString Eventrecord::GeteventIdentifier()
{
    return eventIdentifier;
}

/**
 * @brief Eventrecord::GeteventFrom : Returns event From
 * @return
 */
QString Eventrecord::GeteventFrom()
{
    return eventFrom;
}

/**
 * @brief Eventrecord::GeteventTo : Returns event To
 * @return
 */
QString Eventrecord::GeteventTo()
{
    return eventTo;
}

/**
 * @brief Eventrecord::GeteventName : Returns event name
 * @return
 */
QString Eventrecord::GeteventName()
{
    return eventName;
}

/**
 * @brief Eventrecord::GeteventDateTime : Returns event date and time
 * @return
 */
QString Eventrecord::GeteventDateTime()
{
    return eventDateTime;
}

/**
 * @brief Eventrecord::GeteventPsSerial : Returns event ps serial
 * @return
 */
QString Eventrecord::GeteventPsSerial()
{
    return eventPsSerial;
}

/**
 * @brief Eventrecord::GeteventActSerial : Returns event Act serial
 * @return
 */
QString Eventrecord::GeteventActSerial()
{
    return eventActSerial;
}

/**
 * @brief Eventrecord::GeteventAuxSerial : Returns event Aux serial
 * @return
 */
QString Eventrecord::GeteventAuxSerial()
{
    return eventAuxSerial;
}

/**
 * @brief Eventrecord::GeteventHeaderTime : Returns event header time
 * @return
 */
QString Eventrecord::GeteventHeaderTime()
{
    return eventHeaderTime;
}

/**
 * @brief Eventrecord::GeteventUsercmt : Returns event user comment
 * @return
 */
QString Eventrecord::GeteventUsercmt()
{
    return eventUsercmt;
}
