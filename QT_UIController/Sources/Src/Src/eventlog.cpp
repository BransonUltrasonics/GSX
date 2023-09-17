/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Event Log parameters

***************************************************************************/
#include "Header/eventlog.h"
#include <QQuickView>
#include <QQmlContext>
/**
 * @brief EventLog::EventLog : Initialization of class members and memory for object
 * @param eventIndex
 * @param eventTime
 * @param eventName
 * @param eventUser
 * @param eventPSSerial
 * @param eventActSerial
 */
EventLog::EventLog(const QString& eventIndex, const  QString& eventTime, const QString& eventName,
                   const QString& eventUser, const QString& eventPSSerial, const  QString& eventActSerial)
    :m_eventIndex(eventIndex)
    ,m_eventTime(eventTime)
    ,m_eventName(eventName)
    ,m_eventUser(eventUser)
    ,m_eventPsSerial(eventPSSerial)
    ,m_eventActSerial(eventActSerial)
{

}

/**
 * @brief EventLog::EventLog : Initialization of class members and memory for object
 * @param eventIndex
 * @param eventTime
 * @param eventName
 * @param eventUser
 * @param eventID
 * @param eventIdentifier
 * @param eventFrom
 * @param eventTo
 * @param eventPSSerial
 * @param eventActSerial
 * @param eventAuxSerial
 * @param eventComments
 */
EventLog::EventLog(const QString &eventIndex, const QString &eventTime, const QString &eventName,
                   const QString &eventUser, const QString &eventID, const QString &eventIdentifier,
                   const QString &eventFrom, const QString &eventTo, const QString &eventPsSerial,
                   const QString &eventActSerial, const QString &eventAuxSerial, const QString &eventComments)
    :m_eventIndex(eventIndex)
    ,m_eventTime(eventTime)
    ,m_eventName(eventName)
    ,m_eventUser(eventUser)
    ,m_eventID(eventID)
    ,m_eventIdentifier(eventIdentifier)
    ,m_eventFrom(eventFrom)
    ,m_eventTo(eventTo)
    ,m_eventPsSerial(eventPsSerial)
    ,m_eventActSerial(eventActSerial)
    ,m_eventAuxSerial(eventAuxSerial)
    ,m_eventComments(eventComments)
{

}

/**
 * @brief EventLog::GeteventIndex : Returns event Index
 * @return
 */
QString EventLog::GeteventIndex() const
{
    return m_eventIndex;
}

/**
 * @brief EventLog::GeteventTime : Returns event time
 * @return
 */
QString EventLog::GeteventTime() const
{
    return m_eventTime;
}

/**
 * @brief EventLog::GeteventName : Returns event Name
 * @return
 */
QString EventLog::GeteventName() const
{
    return m_eventName;
}

/**
 * @brief EventLog::GeteventUser : Returns event user
 * @return
 */
QString EventLog::GeteventUser() const
{
    return m_eventUser;
}

/**
 * @brief EventLog::GeteventID : Returns event ID
 * @return
 */
QString EventLog::GeteventID() const
{
    return m_eventID;
}

/**
 * @brief EventLog::GeteventIdentifier : Returns event Identifier
 * @return
 */
QString EventLog::GeteventIdentifier() const
{
    return m_eventIdentifier;
}

/**
 * @brief EventLog::GeteventFrom : Returns event From
 * @return
 */
QString EventLog::GeteventFrom() const
{
    return m_eventFrom;
}

/**
 * @brief EventLog::GeteventTo : Returns event To
 * @return
 */
QString EventLog::GeteventTo() const
{
    return m_eventTo;
}

/**
 * @brief EventLog::GeteventPsSerial : Returns event PS Serial No.
 * @return
 */
QString EventLog::GeteventPsSerial() const
{
    return m_eventPsSerial;
}

/**
 * @brief EventLog::GeteventActSerial : Returns event ACT Serial No.
 * @return
 */
QString EventLog::GeteventActSerial() const
{
    return m_eventActSerial;
}

/**
 * @brief EventLog::GeteventAuxSerial : Returns event AUX Serial No.
 * @return
 */
QString EventLog::GeteventAuxSerial() const
{
    return m_eventAuxSerial;
}

/**
 * @brief EventLog::GeteventComments : Returns event comments
 * @return
 */
QString EventLog::GeteventComments() const
{
    return m_eventComments;
}
