/*
 * AlarmFunctions.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: DShilonie
 *      
 *      Implements the various characteristics of an individual alarm event that
 *      is detected during a weld, setup process or seek/scan
 
 */
#include  <sys/types.h>
#include  <iostream>

#include "AlarmFunctions.h"
#include "ControlTask.h"
#include "Commons.h"
#include "Logger.h"

using namespace std;


/**************************************************************************//**
* \brief  - create an alarm resource upon occurrence based on: 
*
* \param  - id - ID of alarms
* \param  - type - type of alarms
* \param  - source - alarm origin i.e. SC
* \param  - state - i.e.  active or pending
* \param  - priority - i.e.  high, medium low
* \param  - flags - i.e. bits representing characteristics of the alarm i.e. reset able, 
*
* \return  - none
*
******************************************************************************/
AlarmEvent::AlarmEvent (UINT16 id, UINT16 type, UINT16 source, ALARM_STATE state, ALARM_PRIORITY priority, AlarmActions flags, UINT16 scState, char* description, char * alarmLog)
{
	_type = type;
	_id = id;
	_source = source;
	_priority = priority;
	_flags = flags;
	_state = state;
	_scState = scState;
	
	if (description)
		snprintf (_description, MAX_SIZE_OF_MSG_BUFF, "%s", description);

	if (alarmLog)
	{
		if (_source == ALARM_PC)
			memcpy(_alarmLog, alarmLog, sizeof (PCAlarmData));
		if (_source == ALARM_AC)
			memcpy(_alarmLog, alarmLog, sizeof (ACAlarmData));
	}

#ifdef ALARMMGR_DBG
	printf ("\n%sALARM_T : AlarmEvent entered id:%X source:%d flags: RR%d,LA%d,GA%d,CO%d,CC%d%s\n", KRED, _id, _source, _flags.ResetRequired,
			_flags.LogAlarm, _flags.GeneralAlarm, _flags.CustomAlarmOutput, _flags.CycleCounter, KNRM);
#endif
}

/**************************************************************************//**
* \brief  - relinquish resources upon deletion 
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
AlarmEvent::~AlarmEvent () 
{
#ifdef ALARMMGR_DBG
	printf ("\n%sALARM_T : AlarmEvent removed id:%X source:%d%s\n", KBLU, _id, _source, KNRM);
#endif
}


/**************************************************************************//**
* \brief  - returns the alarm state to the alarm manager
*
* \param  - none
*
*
* \return  - UINT16 alarm state
*
******************************************************************************/
UINT16 AlarmEvent::GetAlarmState ()
{
	return _state;
}


/**************************************************************************//**
* \brief  - returns the alarm source to the alarm manager
*
* \param  - none
*
*
* \return  - UINT16 alarm source
*
******************************************************************************/
UINT16 AlarmEvent::GetAlarmSource()
{
	return _source;
}


/**************************************************************************//**
* \brief  - returns the alarm type to the alarm manager
*
* \param  - none
*
*
* \return  - UINT16 alarm type
*
******************************************************************************/
UINT16 AlarmEvent::GetType()
{
	return _type;
}

/**************************************************************************//**
* \brief  - returns the alarm ID to the alarm manager
*
* \param  - none
*
*
* \return  - UINT16 alarm ID
*
******************************************************************************/
UINT16 AlarmEvent::GetID()
{
	return _id;
}

/**************************************************************************//**
* \brief  - returns the alarm priority to the alarm manager
*
* \param  - none
*
*
* \return  - ALARM_PRIORITY alarm priority
*
******************************************************************************/
AlarmEvent::ALARM_PRIORITY AlarmEvent::GetAlarmPriority()
{
	return _priority;
}


/**************************************************************************//**
* \brief  - returns the alarm flags associated with the alarm  to the alarm manager
*
* \param  - none
*
*
* \return  - UINT16 alarm flags
*
******************************************************************************/
AlarmActions AlarmEvent::GetFlags()
{
	return _flags;					
}

/**************************************************************************//**
* \brief  - returns the log associated with the alarm  to the alarm manager
*
* \param  - none
*
*
* \return  - char * alarmLog
*
******************************************************************************/
char * AlarmEvent::GetLog ()
{
	return _alarmLog;
}


/**************************************************************************//**
* \brief  - copies the log associated with the alarm  to a buffer
*
* \param  - char * alarmLog
*
*
* \return  - None
*
******************************************************************************/
void AlarmEvent::CopyAlarmLog (Message& message)
{
	memcpy(message.Buffer, _alarmLog, sizeof (message.Buffer));
}


/**************************************************************************//**
* \brief  - copies the description associated with the last alarm  to a buffer
*
* \param  - char * alarmDesc
*
*
* \return  - None
*
******************************************************************************/
void AlarmEvent::CopyAlarmDescription (Message& message)
{
	memcpy(message.Buffer, _description, sizeof (message.Buffer));
}


/**************************************************************************//**
* \brief  - returns the description associated with the alarm to the alarm manager
*
* \param  - none
*
*
* \return  - char * alarmDesc
*
******************************************************************************/
char * AlarmEvent::GetAlarmDescription()
{
	return _description;
}


/**************************************************************************//**
* \brief  - sets an alarm's state
*
* \param  - Alarm state to set
*
* \return  - none
*
******************************************************************************/
void AlarmEvent::SetAlarmState (AlarmEvent::ALARM_STATE state)
{
	_state |= BIT_MASK(state);
}

SCSTATES AlarmEvent::GetScState()
{
	return (SCSTATES) _scState;
}

/**************************************************************************//**
* \brief  - clears an alarm's state
*
* \param  - Alarm state to clear
*
* \return  - none
*
******************************************************************************/
void AlarmEvent::ClearAlarmState (AlarmEvent::ALARM_STATE state)
{
	_state &= ~BIT_MASK(state);
}

