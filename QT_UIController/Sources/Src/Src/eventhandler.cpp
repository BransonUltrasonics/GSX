/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to log events in the Event log table in UI and some events
    raised by UI will be stored in DB

***************************************************************************/
#include "Header/eventhandler.h"
#include "Header/qtcommon.h"
#include <QDateTime>
#include <QTime>
#include <QDebug>
#include <Header/communicationinterface.h>
#include "Header/informationpanel.h"
#include "Header/configuration.h"

extern CommunicationInterface CIObj;
extern InformationPanel *PrimaryData;

#define EVENTHAND_USER_LOGIN                "USER LOGIN"
#define EVENTHAND_USER_LOGOUT               "USER LOGOUT"
#define EVENTHAND_HORN_SCAN_STARTED         "HORN SCAN STARTED"
#define EVENTHAND_HORN_SCAN_COMPLETED       "HORN SCAN COMPLETED"
#define EVENTHAND_HORN_SCAN_ABORTED         "HORN SCAN ABORTED"
#define EVENTHAND_USER_CREATED              "USER CREATED"
#define EVENTHAND_USER_MODIFIED             "USER MODIFIED"
#define EVENTHAND_AUTHORITY_CHECK_CHANGED   "AUTHORITY CHECK CHANGED"
#define EVENTHAND_RECIPE_CREATED            "RECIPE CREATED"
#define EVENTHAND_RECIPE_MODIFIED           "RECIPE MODIFIED"
#define EVENTHAND_SEEK_PERFORM              "SEEK RUN PERFORMED"
#define EVENTHAND_CYCLE_COUNT_RESETTED      "CYCLE COUNT RESETTED"
#define EVENTHAND_BOARRD_REPLACED           "BOARD REPLACED"
#define EVENTHAND_SYSTEM_CLEANED            "SYSTEM CLEANED"
#define EVENTHAND_SYSYTEM_FACTORY_RESET     "FACTORY RESET"
#define EVENTHAND_RECIPE_VALIDATED          "RECIPE VALIDATED"
#define EVENTHAND_RECIPE_INVALIDATED        "RECIPE INVALIDATED"
#define EVENTHAND_DATE_CHANGED              "SYSTEM DATE/TIME CHANGED"
#define EVENTHAND_PASSWORD_CHANGED          "PASSWORD CHANGED"
#define EVENTHAND_RECIPE_COPIED             "RECIPE COPIED"
#define EVENTHAND_VALIDATED_CHANGE          "RECIPE VALIDATION CHANGE"
#define EVENTHAND_SYSTEM_CALIBRATION        "CALIBRATION COMPLETED"
#define EVENTHAND_SYSTEM_HW_CHANGED 		"SYSTEM HARDWARE CHANGED"
#define EVENTHAND_UIC_SOFTWARE_VERSION      "UIC SOFTWARE VERSION"
#define EVENTHAND_RECIPE_SAVED              "RECIPE SAVED"
#define EVENTHAND_RECIPE_DELETED            "RECIPE DELETED"

/**
 * @brief EventHandler::EventHandler :constructor ,Initializing the EventDescripto
 */
EventHandler::EventHandler()
{
    m_Ps_Serial = "Default123";
    EventDescription();
}

/**
 * @brief EventHandler::eventSend :Get the event Description  from the EventDescriptor
 * @param eventId
 * @return
 */
bool EventHandler::eventSend(int eventId)
{
    m_eventId = EventDescriptor[eventId];
    send();
    return 0;
}

/**
 * @brief EventHandler::eventSend : Get the event Description  from the EventDescriptor
 * @param eventId
 * @param comment
 * @return
 */
bool EventHandler::eventSend(int eventId, QString comment)
{
    m_eventId=EventDescriptor[eventId];
    send(comment);
    return 0;
}

/*future Use
void EventHandler::GetTheSytemDetails(){
    QDateTime systemDateTime=QDateTime::currentDateTime();

    m_time=systemDateTime.time();

    m_date=systemDateTime.date();
    QString dateStr=m_date.toString("dd.MM.yyyy");

}
*/

/**
 * @brief EventHandler::send :sending the event to the SC via sockets
 */
void EventHandler::send()
{
    QString data;
    QString errormsg;

    data = ("'")+PrimaryData->getUsername()+"'"+","+ "'"  + m_eventId + "'"+ "," + "' '" + "," + "'"+m_Ps_Serial+"'";

    //qDebug() << data;

    if(Configuration::getInstance()->getFeatureFlagStatus(EVENT_LOG) == 1)
    {
        CIObj.sendMessage(REQ_SET_EVENT_LOG,RES_SET_EVENT_LOG,data);
        CIObj.recivedMessage(RES_SET_EVENT_LOG,errormsg);

        //qDebug() << errormsg;
    }
}

/**
  * @brief EventHandler::send : sending the event to the SC via sockets
  * @param comment
  */
void EventHandler::send(QString comment){
    QString data;
    QString errormsg;
    m_UserComment=comment;

    //qDebug() << m_UserComment;

    data=("'")+PrimaryData->getUsername()+"'"+","+ "'"  + m_eventId + "'"+ "," +"'"+ m_UserComment +"'"+ "," + "'"+m_Ps_Serial+"'";

    //qDebug() << data;

    CIObj.sendMessage(REQ_SET_EVENT_LOG,RES_SET_EVENT_LOG,data);
    CIObj.recivedMessage(RES_SET_EVENT_LOG,errormsg);

    //qDebug() << errormsg;

}

/**
 * @brief EventHandler::EventDescription : Filing the EventDescriptor with descriptor
 */
void EventHandler::EventDescription()
{

    EventDescriptor[EVENT_USER_LOGIN]                       = EVENTHAND_USER_LOGIN;
    EventDescriptor[EVENT_USER_LOGOUT]                      = EVENTHAND_USER_LOGOUT;
    EventDescriptor[EVENT_HORN_SCAN_COMPLETED]              = EVENTHAND_HORN_SCAN_COMPLETED;
    EventDescriptor[EVENT_HORN_SCAN_ABORTED]                = EVENTHAND_HORN_SCAN_ABORTED;
    EventDescriptor[EVENT_USER_CREATED]                     = EVENTHAND_USER_CREATED;
    EventDescriptor[EVENT_USER_MODIFIED]                    = EVENTHAND_USER_MODIFIED;
    EventDescriptor[EVENT_AUTHORITY_CHECK_CHANGED]          = EVENTHAND_AUTHORITY_CHECK_CHANGED;
    EventDescriptor[EVENT_RECIPE_CREATED]                   = EVENTHAND_RECIPE_CREATED;
    EventDescriptor[EVENT_RECIPE_MODIFIED]                  = EVENTHAND_RECIPE_MODIFIED;
    EventDescriptor[EVENT_SEEK_SCAN_STARTED]                = EVENTHAND_SEEK_PERFORM;
    EventDescriptor[EVENT_CYCLE_COUNTER_RESETTED]           = EVENTHAND_CYCLE_COUNT_RESETTED;
    EventDescriptor[EVENT_SYSTEM_BOARD_REPLACED]            = EVENTHAND_BOARRD_REPLACED ;
    EventDescriptor[EVENT_SYSTEM_CLEANED]                   = EVENTHAND_SYSTEM_CLEANED;
    EventDescriptor[EVENT_SYSYTEM_COMPONENT_FACTORY_RESET]  = EVENTHAND_SYSYTEM_FACTORY_RESET;
    EventDescriptor[EVENT_RECIPE_VALIDATED]                 = EVENTHAND_RECIPE_VALIDATED;
    EventDescriptor[EVENT_RECIPE_INVALIDATED]               = EVENTHAND_RECIPE_INVALIDATED;
    EventDescriptor[EVENT_DATE_CHANGED]                     = EVENTHAND_DATE_CHANGED;
    EventDescriptor[EVENT_PASSWORD_CHANGED]                 = EVENTHAND_PASSWORD_CHANGED;
    EventDescriptor[EVENT_RECIPE_COPIED]                    = EVENTHAND_RECIPE_COPIED;
    EventDescriptor[EVENT_VALIDATED_CHANGE]                 = EVENTHAND_VALIDATED_CHANGE;
    EventDescriptor[EVENT_SYSTEM_CALIBRATION]               = EVENTHAND_SYSTEM_CALIBRATION;
    EventDescriptor[EVENT_SYSTEM_HW_CHANGED]                = EVENTHAND_SYSTEM_HW_CHANGED;
    EventDescriptor[EVENT_UIC_SOFTWARE_VERSION]             = EVENTHAND_UIC_SOFTWARE_VERSION;
    EventDescriptor[EVENT_RECIPE_SAVED]                     = EVENTHAND_RECIPE_SAVED;
    EventDescriptor[EVENT_RECIPE_DELETED]                   = EVENTHAND_RECIPE_DELETED;
}
