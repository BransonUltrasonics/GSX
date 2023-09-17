/*
 * AlarmManager.cpp
 *
 *  Created on: Mar 15, 2018
 *      Author: DShilonie
 */

#include  <iostream>
#include  <fstream>
#include  "IOManager.h"
#include  "AlarmManager.h"
#include  "Utils.h"
#include  "Commons.h"
#include  "CommonProperty.h"
#include  "MainTask.h"
#include  "SC.h"
#include "EthernetIP.h"
using namespace std;

// static initialization
bool AlarmMgr::_alarmAc = false;
bool AlarmMgr::_alarmPc = false;
bool AlarmMgr::_alarmSc = false;
bool AlarmMgr::_alarmProcessingPc = false;
bool AlarmMgr::_alarmProcessingAc = false;
bool AlarmMgr::_alarmProcessingSc = false;

bool AlarmMgr::_persistEstopAc = false;
bool AlarmMgr::_persistAc = false;
bool AlarmMgr::_persistPc = false;
bool AlarmMgr::_persistSc = false;
bool AlarmMgr::_firstWeldCycle = false;

SCSTATES AlarmMgr::_alarmPcSCState = PREREADY;
SCSTATES AlarmMgr::_alarmAcSCState = PREREADY;
UINT16 AlarmMgr::_weldAlarmCount = 0;

AlarmConfiguration	AlarmMgr::_configurations[2] = {0};
LIST AlarmMgr::_alarms = {{0,0}, 0};


/**************************************************************************//**
* \brief  - construct the alarm event node that contains list pointers and a 
* 			pointer to the actual alarm event where all the data is stored  
*
* \param  - UINT16	id,
* \param  - UINT16	type
* \param  - UINT16 	source
* \param  - ALARM_PRIORITY priority
* \param  - AlarmActions flags
* \param  - char * description 
* \param  - char * alarmLog 
*
* \return  - none
*
******************************************************************************/
AlarmMgr::ALARM_EVENT::ALARM_EVENT (UINT16	id, 
		UINT16	type,
		UINT16 	source,
		AlarmEvent::ALARM_PRIORITY priority,
		AlarmActions flags,
		UINT16	scState,
		char * description,
		char * alarmLog)

{
	alarmNode.next = 0;
	alarmNode.previous = 0;
	pAlarmEvent  = new AlarmEvent (id, type, source, AlarmEvent::STATE_ACTIVE, priority, flags, scState, description, alarmLog);
}


/**************************************************************************//**
* \brief  - Deletes the alarm event node that contains a pointer
*			to the actual alarm event and frees up the memory
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
AlarmMgr::ALARM_EVENT::~ALARM_EVENT ()
{
	if (pAlarmEvent)
	{
		delete pAlarmEvent;
	}
}

/**************************************************************************//**
* \brief  - construct resources upon start of alarm task 
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
AlarmMgr::AlarmMgr() 
{
	lstInit (&_alarms);
	_alarmMgrStatus = 0;
	
	SetDefaultConfiguration ();
	
#ifdef ALARMMGR_DBG
	printf("\nALARM_T : Alarm Manager Created\n");
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
AlarmMgr::~AlarmMgr()
{
	ClearAllAlarmInfo();
}


/**************************************************************************//**
* \brief  -  returns alarm count of all or of a certain action required  
*
* \param  - ALARM_ACTION_COUNT action 
*
* \return  - UINT16 count
*
******************************************************************************/
UINT16 AlarmMgr::GetAlarmActionCount (ALARM_ACTION_COUNT action, UINT16 source)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	UINT16	count = 0;
	
	if (action == COUNT_ALL)
	{
		count = (uint16_t) lstCount(&_alarms);
	}
	else
	{
		for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
		{
			pNode = lstNth (&_alarms, alarm);
			
			if (pNode)
			{
				pEvent = (pALARM_EVENT) pNode;
				
				switch (action)
				{
				case COUNT_RESET_REQ:
					if (source == NO_SOURCE)
					{
						if (pEvent->pAlarmEvent->GetFlags().ResetRequired)
							count++;
					}
					else if (pEvent->pAlarmEvent->GetAlarmSource() == source)
					{
						 if (pEvent->pAlarmEvent->GetFlags().ResetRequired)
							count++;
					}
					break;
					
				case COUNT_LOG_ALARM:
					if (source == NO_SOURCE)
					{
						if (pEvent->pAlarmEvent->GetFlags().LogAlarm)
							count++;
					}
					else if (pEvent->pAlarmEvent->GetAlarmSource() == source)
					{
						 if (pEvent->pAlarmEvent->GetFlags().LogAlarm)
							count++;
					}
					break;
					
				case COUNT_GENERAL_OUTPUT:
					if (source == NO_SOURCE)
					{
						if (pEvent->pAlarmEvent->GetFlags().GeneralAlarm)
							count++;
					}
					else if (pEvent->pAlarmEvent->GetAlarmSource() == source)
					{
						 if (pEvent->pAlarmEvent->GetFlags().GeneralAlarm)
							count++;
					}
					break;
					
				case COUNT_CUSTOM_OUTPUT:
					if (source == NO_SOURCE)
					{
						if (pEvent->pAlarmEvent->GetFlags().CustomAlarmOutput)
							count++;
					}
					else if (pEvent->pAlarmEvent->GetAlarmSource() == source)
					{
						 if (pEvent->pAlarmEvent->GetFlags().CustomAlarmOutput)
							count++;
					}
					break;
					
				case COUNT_CYCLE_COUNT:
					if (source == NO_SOURCE)
					{
						if (pEvent->pAlarmEvent->GetFlags().CycleCounter)
							count++;
					}
					else if (pEvent->pAlarmEvent->GetAlarmSource() == source)
					{
						 if (pEvent->pAlarmEvent->GetFlags().CycleCounter)
							count++;
					}
					break;
					
				default:
					break;
				}
			}
		}
	}
	
	return count;
}


/**************************************************************************//**
* \brief  - returns true if alarms have occurred during a weld.
*
* \param  - None 
*
* \return  - boolean - cycleOK
*
******************************************************************************/
bool AlarmMgr::IsCycleOK ()
{
	bool cycleOK;
	
	if (_weldAlarmCount)
		cycleOK = false;
	else
	{
		if (_firstWeldCycle)
			cycleOK = true;
		else
			cycleOK = false;
	}
	
	return cycleOK;
}


/**************************************************************************//**
* \brief  - clears the alarm count for those that have occurred during
*			a weld.
*
* \param  - None 
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::ClearCycleOK()
{
	_weldAlarmCount = 0;
}


/**************************************************************************//**
* \brief  - returns alarm count of all or of a certain source that are
* 			reset required if requested, or all if not.
*
* \param  - UINT16 source 
*
* \return  -UINT16 count
*
******************************************************************************/
UINT16 AlarmMgr::GetAlarmCount (UINT16 source)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	UINT16	count = 0;
	
	if (source == NO_SOURCE)
	{
		count = (uint16_t) lstCount(&_alarms);
	}
	else
	{
		for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
		{
			pNode = lstNth (&_alarms, alarm);
			
			if (pNode)
			{
				pEvent = (pALARM_EVENT) pNode;
				
				if (pEvent->pAlarmEvent->GetAlarmSource() == source)
				{
					count++;
				}
			}
		}
	}
	
	return count;
}


/**************************************************************************//**
* \brief  -  Saves the alarm info event into  by adding it to the alarm list 
*
* \param  - ALARM_INFO& alarmInfo
*
* \return  - bool added
*
******************************************************************************/
bool AlarmMgr::SaveAlarmInfo (ALARM_INFO& alarmInfo)
{
	static char description[MAX_SIZE_OF_MSG_BUFF];
	NODE * pNode;
	pALARM_EVENT pEvent;
	bool added = false;
	
	_CHECKRETURN(alarmInfo.source > NO_SOURCE && alarmInfo.source < MAX_SOURCE,added)
	
	if (!IsAlarmInfo (alarmInfo.id))
	{
		// check if this is a persistent alarm
		IsPersistentAlarm (alarmInfo);
		
		snprintf(description, MAX_SIZE_OF_MSG_BUFF, "%d%X", alarmInfo.flags.ResetRequired, alarmInfo.id);
		alarmInfo.description = description;
		
		pEvent = new ALARM_EVENT (alarmInfo.id, alarmInfo.alarmType, alarmInfo.source, alarmInfo.priority, alarmInfo.flags, alarmInfo.scState, alarmInfo.description, alarmInfo.alarmLog);
		
		if (pEvent)
		{
			lstAdd(&_alarms, &pEvent->alarmNode);
			added = true;
			
			switch (alarmInfo.source)
			{
				case ALARM_PC:
	
					if (GetAlarmActionCount (COUNT_RESET_REQ, ALARM_PC))
						_alarmPc = true;

					if (GetPersistentCount (ALARM_PC))
						_persistPc = true;
					break;
					
				case ALARM_AC:
					
					if (GetAlarmActionCount (COUNT_RESET_REQ, ALARM_AC))
						_alarmAc = true;

					if (GetPersistentCount (ALARM_AC))
						_persistAc = true;
					break;
					
				case ALARM_SC:
					
					if (GetAlarmActionCount (COUNT_RESET_REQ, ALARM_SC))
						_alarmSc = true;
					
					if (GetPersistentCount (ALARM_SC))
						_persistSc = true;
	
					_alarmProcessingSc = false;
					break;
			}
		}
	}
	
	if (added)
	{
		// check for special flags at insertion
		if (alarmInfo.flags.GeneralAlarm)
		{
			// assert output for general alarm 
			DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_GENERAL, DigitalIO::LINE_HIGH);
		}

		// check SC state at insertion
		if (alarmInfo.scState == WAIT_FOR_TRIGGER
				|| alarmInfo.scState == WELD_SONIC_ON
				|| alarmInfo.scState == HOLD
				|| alarmInfo.scState == WAIT_FOR_READY_POSITION)
		{
			_weldAlarmCount++;
		}
		
		// check for special types at insertion
		switch(alarmInfo.alarmType & ALARMTYPE_MASK)
		{		
			case TYPE_PROCESS_LIMIT:
				
				// assert output for part reject/suspect alarm 
				if (alarmInfo.id < ALARM_PROCESS_LIMIT_PSA_START)
					DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_REJECT, DigitalIO::LINE_HIGH);
				else
					DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_SUSPECT, DigitalIO::LINE_HIGH);
				break;
				
			default:
				break;
		}


#ifdef ALARMMGR_DBG
			printf ("\nALARM_T : count: %d alarm(s)\n", GetAlarmCount ());
#endif

	}
	
	return added;
}

/**************************************************************************//**
* \brief  - Returns an alarm info by searching the alarm list for the alarm ID  
*
* \param - UINT16 alarmID
* \param - ALARM_INFO& alarmInfo
*
* \return  - bool found
*
******************************************************************************/
bool AlarmMgr::GetAlarmInfo (UINT16 alarmID, ALARM_INFO& alarmInfo)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	bool found = false;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			
			if (pEvent->pAlarmEvent->GetID() == alarmID)
			{
				alarmInfo.id = pEvent->pAlarmEvent->GetID();
				alarmInfo.source = pEvent->pAlarmEvent->GetAlarmSource();
				alarmInfo.alarmType = pEvent->pAlarmEvent->GetType();
				alarmInfo.priority = pEvent->pAlarmEvent->GetAlarmPriority();
				alarmInfo.flags = pEvent->pAlarmEvent->GetFlags();
				alarmInfo.alarmLog = pEvent->pAlarmEvent->GetLog();
				alarmInfo.description = pEvent->pAlarmEvent->GetAlarmDescription();
			
				found = true;
				break;
			}
		}
	}
	
	return found;
}


/**************************************************************************//**
* \brief  - Searches the alarm list for an alarm ID  
*
* \param - UINT16 alarmID
*
* \return  - bool found
*
******************************************************************************/
bool AlarmMgr::IsAlarmInfo (UINT16 alarmID)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	bool found = false;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			
			if (pEvent->pAlarmEvent->GetID() == alarmID)
			{
				found = true;
				break;
			}
		}
	}
	
	return found;
}

/**************************************************************************//**
* \brief  - Removes an alarm info by searching the alarm list for the
*			given alarm ID, if the alarm exists and is not persistent  
*
* \param - UINT16 alarmID
*
* \return  - bool success
*
******************************************************************************/
bool AlarmMgr::ClearAlarmInfo (UINT16 alarmID)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	bool deleted = false;
	UINT16 source, type, id;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);

		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			
			if (pEvent->pAlarmEvent->GetID() == alarmID)
			{
				// check for persistent alarms
				if (CheckPersistentAlarm (alarmID))
					break;
				
				// extract critical alarm info
				source = pEvent->pAlarmEvent->GetAlarmSource();
				type = pEvent->pAlarmEvent->GetType();
				id = pEvent->pAlarmEvent->GetID();
					
				// then detach
				lstDelete (&_alarms, pNode);

				// now count
				switch (source)
				{
				case ALARM_PC:

					if (GetPersistentCount (ALARM_PC) == 0)
						_persistPc = false;

					if (GetAlarmActionCount (COUNT_RESET_REQ, ALARM_PC) == 0)
					{
						_alarmPc = false;
						
						PC_RX->MasterEvents |=  (UINT16) BIT_MASK(CTRL_PC_CLEAR_ALARMS);
					}
					
					break;
					
				case ALARM_AC:
					
					if (GetPersistentCount (ALARM_AC) == 0)
						_persistAc = false;

					if (GetAlarmActionCount (COUNT_RESET_REQ, ALARM_AC) == 0)
					{
						_alarmAc = false;
						
						AC_RX->ACControlEvent |=  (UINT16) BIT_MASK(EVENT_AC_CLEAR_ALARMS);
					}
					break;
					
				case ALARM_SC:
					
					if (GetPersistentCount (ALARM_SC) == 0)
						_persistSc = false;

					if (GetAlarmActionCount (COUNT_RESET_REQ, ALARM_SC) == 0)
						_alarmSc = false;
					
					break;
				}

				#ifdef ALARMMGR_DBG
					printf ("\nALARM_T : alarm count: %d alarm(s)\n", GetAlarmCount ());
				#endif
				
				delete pEvent;
				deleted = true;
				break;
			}
		}
	}
	
	// check for any actions at removal
	if (GetAlarmActionCount (COUNT_GENERAL_OUTPUT) == 0)
	{
		// lower output for general alarm 
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_GENERAL, DigitalIO::LINE_LOW);
	}
	
	// check for special types at removal
	if (deleted)
	{
		switch(type & ALARMTYPE_MASK)
		{		
			case TYPE_PROCESS_LIMIT:
				
				// clear output for part reject/suspect alarm 
				if (id < ALARM_PROCESS_LIMIT_PSA_START)
					DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_REJECT, DigitalIO::LINE_LOW);
				else
					DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_SUSPECT, DigitalIO::LINE_LOW);
				break;
				
			default:
				break;
		}
	}

	return deleted;
}


/**************************************************************************//**
* \brief  - Checks an alarm if it is still persisting by the alarm ID and 
* 			associated condition  
*
* \param - UINT16 alarmID
*
* \return  - bool persist
*
******************************************************************************/
bool AlarmMgr::CheckPersistentAlarm (UINT16 alarmID)
{
	bool persist = false;
	CommonProperty *CP = CommonProperty::getInstance();
	
	switch (alarmID)
	{
	case ALARM_ESTOP_NCA:
		if(AC_TX->ACState == AC_ESTOP)
			persist = true;
		break;
		
	case ALARM_SONICS_SOURCE_LOST_EFA:
		if(PC_TX->PC_StatusEvent & BIT_MASK(STATUS_SOURCE_LOST))
			persist = true;
		break;
		
	case ALARM_INTERNAL_COMM_ENET:
		if (ControlTask::GetNextOperationType() == NO_OPERATION)
			persist = true;
		break;
		
	case ALARM_INTERNAL_COMM_ECAT_EFA:
		if(ecat_OP != ControlTask::EcatGetMasterStateSysCall())
			persist =true;
		break;

	case ALARM_ELC_UNKNOWN:
	case ALARM_ELC_PB:
	case ALARM_ELC_24V:
	case ALARM_ELC_ESTOP:
	case ALARM_ELC_LE:
	case ALARM_ELC_SBEAM:
	case ALARM_ELC_TRS:
	case ALARM_ELC_DRIVE:
	case ALARM_ELC_CM:
	case ALARM_ELC_LU:
	case ALARM_ELC_SONICS_EN:
		if(AC_TX->ACStatusEvent & BIT_MASK(EN_FAULT))
			persist = true;
		break;
		
	case ALARM_ALARM_LOG_FULL_EFA:
		persist = CP->IsMemStorageFull(ALARM_MEM_CB);
		break;
	case ALARM_EVENT_LOG_FULL_EFA:
		persist = CP->IsMemStorageFull(EVENT_MEM_CB);
		break;
	case ALARM_WELD_RESULT_FULL_EFA:
	case ALARM_WELD_GRAPH_FULL_EFA:
		persist = CP->IsMemStorageFull(WELD_MEM_CB);
		break;
	case ALARM_HORN_SCAN_FULL_EFA:
		persist = CP->IsMemStorageFull(SCAN_MEM_CB);
		break;
	}
		
	return persist;
}

/**************************************************************************//**
* \brief  - Marks an alarm as persistent by the alarm ID  
*
* \param - ALARM_INFO& alarmInfo
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::IsPersistentAlarm (ALARM_INFO& alarmInfo)
{
	switch (alarmInfo.id)
	{
	case ALARM_ESTOP_NCA:
	case ALARM_SONICS_SOURCE_LOST_EFA:
	case ALARM_ELC_UNKNOWN:
	case ALARM_ELC_PB:
	case ALARM_ELC_24V:
	case ALARM_ELC_ESTOP:
	case ALARM_ELC_LE:
	case ALARM_ELC_SBEAM:
	case ALARM_ELC_TRS:
	case ALARM_ELC_DRIVE:
	case ALARM_ELC_CM:
	case ALARM_ELC_LU:
	case ALARM_ELC_SONICS_EN:
	case ALARM_INTERNAL_COMM_ENET:
	case ALARM_INTERNAL_COMM_ECAT_EFA:	
	case ALARM_ALARM_LOG_FULL_EFA:
	case ALARM_EVENT_LOG_FULL_EFA:
	case ALARM_WELD_RESULT_FULL_EFA:
	case ALARM_WELD_GRAPH_FULL_EFA:
	case ALARM_HORN_SCAN_FULL_EFA:
		alarmInfo.flags.Persistent = true;
		alarmInfo.flags.ResetRequired = true;
		break;
	}
}


/**************************************************************************//**
* \brief  - Clears all alarms info by pulling the alarm event from the list
* 			and then free all allocated resources.  
*
* \param - None
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::ClearAllAlarmInfo()
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	int alarms = lstCount(&_alarms);
	
	for (; alarms > 0; alarms--)
	{
		pNode = lstGet(&_alarms);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			delete pEvent;
		}
	}
	
	PC_RX->MasterEvents |=  (UINT16) BIT_MASK(CTRL_PC_CLEAR_ALARMS);
	AC_RX->ACControlEvent |=  (UINT16) BIT_MASK(EVENT_AC_CLEAR_ALARMS);

	_alarmPc = false;
	_alarmAc = false;
	_alarmSc = false;
	
	_persistAc = false;
	_persistPc = false;
	_persistSc = false;

	_alarmProcessingSc = false;
	
	// clear related outputs 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_GENERAL, DigitalIO::LINE_LOW);
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_REJECT, DigitalIO::LINE_LOW);
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_SUSPECT, DigitalIO::LINE_LOW);

#ifdef ALARMMGR_DBG
	printf ("\nALARM_T : alarm count: %d alarm(s)\n", GetAlarmCount ());
#endif
}

/**************************************************************************//**
* \brief  - Sets a given alarm's state in the list by ID  
*
* \param  	- UINT16 alarmID
* \param 	- ALARM_STATE stateBit
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::SetAlarmState(UINT16 alarmID, AlarmEvent::ALARM_STATE stateBit)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			
			if (pEvent->pAlarmEvent->GetID () == alarmID)
			{
				pEvent->pAlarmEvent->SetAlarmState(stateBit);
				break;
			}
		}
	}
}


/**************************************************************************//**
* \brief  - Returns a given alarm's state in the list by ID  
*
* \param  	- UINT16 alarmID
*
* \return  - UINT16 state
*
******************************************************************************/
UINT16 AlarmMgr::GetAlarmState(UINT16 alarmID)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	UINT16 state = 0;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			
			if (pEvent->pAlarmEvent->GetID () == alarmID)
			{
				state = pEvent->pAlarmEvent->GetAlarmState();
				break;
			}
		}
	}
	
	return state;
}


/**************************************************************************//**
* \brief  - Clears a given alarm's state in the list by ID  
*
* \param  	- UINT16 alarmID
* \param 	- ALARM_STATE stateBit
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::ClearAlarmState(UINT16 alarmID, AlarmEvent::ALARM_STATE stateBit)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			
			if (pEvent->pAlarmEvent->GetID () == alarmID)
			{
				pEvent->pAlarmEvent->ClearAlarmState(stateBit);
				break;
			}
		}
	}
}


/**************************************************************************//**
* \brief  - Provides preliminary entry point for an alarm event to be entered 
* 			into the alarm manager.  
*
* \param - UINT8 origin
* \param  - UINT8 alarm origin
*
* \return  - bool success TRUE or FALSE
*
******************************************************************************/
bool AlarmMgr::EnterAlarmEvent (UINT8 origin, UINT16 alarmID)
{
	Message message;
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	message.msgID	= TO_ALARM_TASK_ALARM_EVENT;
	bool success = false;

	((pALARM_INFO) message.Buffer)->source = origin;
	((pALARM_INFO) message.Buffer)->id = alarmID;
	((pALARM_INFO) message.Buffer)->scState = (UINT16) SC::stateMachine.GetStateType();

	switch ((ALARM_FROM) origin)
	{
	case ALARM_SC:

		if (!IsAlarmInfo (alarmID))
		{
			// need to map ID to type
			((pALARM_INFO) message.Buffer)->alarmType = alarmID & ALARMTYPE_MASK;
	
			// need to map type to action
			GetActions (((pALARM_INFO) message.Buffer)->alarmType, alarmID, ((pALARM_INFO) message.Buffer)->flags);
			
			if (msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL) == OK)
			{
				success = true;
				_alarmProcessingSc = true;
				
#ifdef ALARMMGR_DBG
				printf ("\nALARM_T : EnterAlarmEvent: ALARM_SC\n");
#endif
			}
		}
		break;

	case ALARM_PC:
		
		if (!_alarmProcessingPc)
		{
			if (msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL) == OK)
			{
				success = true;
				_alarmProcessingPc = true;
				_alarmPcSCState = SC::stateMachine.GetStateType ();

#ifdef ALARMMGR_DBG
				printf ("\nALARM_T : EnterAlarmEvent: ALARM_PC\n");
#endif
			}
		}
		break;
	
	case ALARM_AC:

		// special case of E-Stop
		if (alarmID == ALARM_ESTOP_NCA)
		{
			if (!_persistEstopAc)
			{
				PC_RX->MasterEvents |= BIT_MASK(CTRL_ESTOP);
				
				message.msgID	= TO_ALARM_TASK_ESTOP_EVENT;
				
				if (msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL) == OK)
				{
					success = true;
					_persistEstopAc = true;
					_alarmAcSCState = SC::stateMachine.GetStateType ();
					
					EthernetIP  *FbPtr = EthernetIP::Getinstance();
					FbPtr->SetOutputToSWIOImage (SW1, ES,LINE_HIGH_EIP);
#ifdef ALARMMGR_DBG
		printf ("\nALARM_T : EnterAlarmEvent: ESTOP_AC\n");
#endif
				}
			}
		}
		else if (!alarmID)
		{
			// check persistent state here...
						
			if (!_alarmProcessingAc)
			{
				if (msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL) == OK)
				{
					success = true;
					_alarmProcessingAc = true;
					_alarmAcSCState = SC::stateMachine.GetStateType ();
	
#ifdef ALARMMGR_DBG
		printf ("\nALARM_T : EnterAlarmEvent: ALARM_AC\n");
#endif
				}
			}
		}

		break;
		
	default:

		LOGDBG("\nALARM_T : ------- EnterAlarmEvent: Unknown source",0,0,0);
		break;

	}
	
	return success;
}


/**************************************************************************//**
* \brief  - Provides preliminary entry point for an alarm source to be marked  
* 			as alarm free  
*
* \param - UINT8 origin
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::ExitAlarmEvent (UINT8 origin)
{
	switch ((ALARM_FROM) origin)
	{
	case ALARM_SC:
		_alarmProcessingSc = false;
		break;
	case ALARM_AC:
		_alarmProcessingAc = false;
		break;
	case ALARM_PC:
		_alarmProcessingPc = false;
		break;
	}
}


/**************************************************************************//**
* \brief  - Provides preliminary entry point for an alarm source to be marked  
* 			as persistent alarm free  
*
* \param 	- UINT8 origin
* \param  	- UINT16 alarmID
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::ExitPersistAlarmEvent(UINT8 origin, UINT16 alarmID)
{
	Message message;
	
	switch ((ALARM_FROM) origin)
	{
	case ALARM_SC:
		break;
		
	case ALARM_AC:
		if(alarmID ==  ALARM_ESTOP_NCA)
		{
			if (_persistEstopAc)
			{
				memset(message.Buffer,0x00,sizeof(message.Buffer));
				message.msgID	= TO_ALARM_TASK_ESTOP_RELEASE;
				msgQSend (CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			}

			PC_RX->MasterEvents &=  (UINT16) ~BIT_MASK(CTRL_ESTOP);
			_persistEstopAc = false;
			EthernetIP  *FbPtr = EthernetIP::Getinstance();
			FbPtr->SetOutputToSWIOImage (SW1, ES,LINE_LOW_EIP);
			
		}
		break;

	case ALARM_PC:
		break;
	}
}


/**************************************************************************//**
* \brief  - Provides preliminary entry point for determining if all alarm
* 			sources are alarm free  
*
* \param 	- None
*
* \return  - bool alarm_state 
*
******************************************************************************/
bool AlarmMgr::IsAlarmState()
{
	bool alarm_state = true;
	
	// check persistent states here...
	if (!_alarmProcessingSc && !_alarmProcessingAc && !_alarmProcessingPc
			&& !_alarmSc && !_alarmAc && !_alarmPc && !_persistEstopAc && !_persistAc && !_persistPc && !_persistSc)
		alarm_state = false;
	
	return alarm_state;
}

/**************************************************************************//**
* \brief  - returns the last alarm event's description
*
* \param  - Message& message
*
* \return  - bool found
*
******************************************************************************/
bool AlarmMgr::GetLastAlarm(Message& message)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	bool found = false;

	pNode = lstFirst(&_alarms);
			
	if (pNode)
	{
		pEvent = (pALARM_EVENT) pNode;
		pEvent->pAlarmEvent->CopyAlarmDescription (message);
		found = true;
	}
	
	return found;
}


/**************************************************************************//**
* \brief  - returns all comma separated active alarms descriptions in
* 			a string 
*
* \param  - Message& message
*
* \return  - bool found
*
******************************************************************************/
bool AlarmMgr::GetAllActiveAlarms (Message& message)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	string active_alarms;
	bool found = false;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			active_alarms.append ((const char *) pEvent->pAlarmEvent->GetAlarmDescription());
			active_alarms.append (",");
			found = true;
		}
	}
	
	if(found)
	{
		active_alarms.erase (active_alarms.end () - 1);
		memset (message.Buffer, 0x00, MAX_SIZE_OF_MSG_BUFF);
		snprintf (message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%s", active_alarms.c_str());
	}
	
	return found;
}



/**************************************************************************//**
* \brief   - Gets enabled or disabled alarm actions reference associated
* 			with for a given type as they were configured (private)
*
* \param   	- UINT16 alarmType
* \param	- AlarmActions& alarmAction
*
* \return  - AlarmActions& alarmAction
*
******************************************************************************/
void AlarmMgr::GetActions (UINT16 alarmType, UINT16 alarmId, AlarmActions& alarmAction)
{
	
	switch(alarmType & ALARMTYPE_MASK)
	{		
		case TYPE_OVERLOAD:			
			alarmAction = _configurations[1].OverloadAlarmActions;			
			break;
	
		case TYPE_CYCLE_MODIFIED:			
			alarmAction = _configurations[1].CycleModifiedAlarmActions;
			break;
	
		case TYPE_WARNING:			
			alarmAction = _configurations[1].WarningAlarmActions;
			break;
	
		case TYPE_PROCESS_LIMIT:
			if (alarmId > ALARM_PROCESS_LIMIT_PSA_START)
			{
				alarmAction = _configurations[1].SuspectAlarmActions;
			}
			else
			{
				alarmAction = _configurations[1].RejectAlarmActions;
			}
			break;
	
		case TYPE_NOCYCLE:			
			alarmAction = _configurations[1].NoCycleAlarmActions;
			break;
	
		case TYPE_HARDWARE_FAILURE:			
			alarmAction = _configurations[1].HardwareFailureAlarmActions;
			break;
	
		case TYPE_NOCYCLE_OVERLOAD:			
			alarmAction = _configurations[1].NoCycleOverloadAlarmActions;
			break;
			
		case TYPE_EN_FAULT:
			alarmAction.LogAlarm = true; 
			alarmAction.CustomAlarmOutput = true;
			alarmAction.CycleCounter = true;
			alarmAction.GeneralAlarm = true;
			alarmAction.Persistent = true;
			alarmAction.ResetRequired = true;
			break;
	
		default :
			break;
	}	
}


/**************************************************************************//**
* \brief   - Gets enabled or disabled alarm actions reference associated
* 			with for a given type as they were configured (Public)
*
* \param   	- UINT16 alarmType
* \param	- AlarmActions& alarmAction
*
* \return  - AlarmActions& alarmAction
*
******************************************************************************/
void AlarmMgr::GetAlarmActions(UINT16 alarmType, UINT16 alarmId, AlarmActions& alarmAction)
{
	GetActions (alarmType, alarmId, alarmAction);
}


/**************************************************************************//**
* \brief   - Returns the SC state for a given alarm source
*
* \param   	- UINT16 source
*
* \return  - SCSTATES state
*
******************************************************************************/
SCSTATES AlarmMgr::GetAlarmSCState(UINT16 source)
{
	SCSTATES state = SC_ALARM;
	
	switch (source)
	{
	case ALARM_PC:
		state = _alarmPcSCState;
		break;
	case ALARM_AC:
		state = _alarmAcSCState;
		break;
	}
	
	return state;
}


/**************************************************************************//**
* \brief   - Sets a flag to note first ever weld cycle for the alarm manager
*
* \param   	- None
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::FirstWeldCycle()
{
	_firstWeldCycle = true;
}


/**************************************************************************//**
* \brief   - Returns the persistent alarm count for a given source
*			or a total for all sources
*
* \param   	- UINT16 source
*
* \return  - UINT16	count
*
******************************************************************************/
UINT16 AlarmMgr::GetPersistentCount(UINT16 source)
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	UINT16	count = 0;
	
	for (int alarm = 1; alarm <= lstCount(&_alarms); alarm++)
	{
		pNode = lstNth (&_alarms, alarm);
		
		if (pNode)
		{
			pEvent = (pALARM_EVENT) pNode;
			
			if (source == NO_SOURCE)
			{
				if (pEvent->pAlarmEvent->GetFlags().Persistent)
					count++;
			}
			else if (pEvent->pAlarmEvent->GetAlarmSource() == source)
			{
				if (pEvent->pAlarmEvent->GetFlags().Persistent)
					count++;
			}
		}
	}
	
	return count;
}


/**************************************************************************//**
* \brief  - Attempts to reset all alarms after receiving the RESET input
*			signal using moving alarm index and diminishing count logic. 
*			Persistent alarms can not be removed, and alarm state will be
*			maintained.
*
* \param  - None
*
* \return  - int count
*
******************************************************************************/
int AlarmMgr::ClearAlarmResetInput()
{
	NODE * pNode;
	pALARM_EVENT pEvent;
	int alarm = 1;
	int	count = 0;
	
	if (lstCount(&_alarms))
	{
		for (;;)
		{
			pNode = lstNth (&_alarms, alarm);
		
			if (pNode)
			{
				pEvent = (pALARM_EVENT) pNode;
	
				if(ClearAlarmInfo (pEvent->pAlarmEvent->GetID()))
				{
					// alarm cleared
					count = lstCount(&_alarms);
					
					if (!count)
						break;
					
					if (count < alarm)
						break;
				}
				else
				{
					// alarm could not be cleared
					count = lstCount(&_alarms);
					
					if (count > alarm)
					{
						alarm++;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	
	return count;
}

/**************************************************************************//**
* \brief   - Sets the default alarm action configuration for the various
* 			alarm types
*
* \param   	- None
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::SetDefaultConfiguration()
{
	_configurations[0].OverloadAlarmActions.ResetRequired			= 	false;
	_configurations[0].OverloadAlarmActions.LogAlarm				= 	true;
	_configurations[0].OverloadAlarmActions.GeneralAlarm			= 	true;
	_configurations[0].OverloadAlarmActions.CustomAlarmOutput		= 	false;
	_configurations[0].OverloadAlarmActions.CycleCounter			= 	true;

	_configurations[0].CycleModifiedAlarmActions.ResetRequired 		= 	false;
	_configurations[0].CycleModifiedAlarmActions.LogAlarm 			= 	true;
	_configurations[0].CycleModifiedAlarmActions.GeneralAlarm 		= 	true;
	_configurations[0].CycleModifiedAlarmActions.CustomAlarmOutput 	= 	false;
	_configurations[0].CycleModifiedAlarmActions.CycleCounter 		= 	true;

	_configurations[0].WarningAlarmActions.ResetRequired			= 	false;
	_configurations[0].WarningAlarmActions.LogAlarm					= 	true;
	_configurations[0].WarningAlarmActions.GeneralAlarm				= 	true;
	_configurations[0].WarningAlarmActions.CustomAlarmOutput		= 	false;
	_configurations[0].WarningAlarmActions.CycleCounter				= 	true;	

	_configurations[0].SuspectAlarmActions.ResetRequired 			= 	false;
	_configurations[0].SuspectAlarmActions.LogAlarm 				= 	true;
	_configurations[0].SuspectAlarmActions.GeneralAlarm 			= 	true;
	_configurations[0].SuspectAlarmActions.CustomAlarmOutput 		= 	true;
	_configurations[0].SuspectAlarmActions.CycleCounter 			= 	true;

	_configurations[0].RejectAlarmActions.ResetRequired 			= 	false;		
	_configurations[0].RejectAlarmActions.LogAlarm 					= 	true;
	_configurations[0].RejectAlarmActions.GeneralAlarm 				= 	true;
	_configurations[0].RejectAlarmActions.CustomAlarmOutput 		= 	false;
	_configurations[0].RejectAlarmActions.CycleCounter 				= 	true;

	_configurations[0].NoCycleAlarmActions.ResetRequired			= 	false;
	_configurations[0].NoCycleAlarmActions.LogAlarm					= 	true;
	_configurations[0].NoCycleAlarmActions.GeneralAlarm				= 	true;
	_configurations[0].NoCycleAlarmActions.CustomAlarmOutput		= 	true;
	_configurations[0].NoCycleAlarmActions.CycleCounter				= 	false;

	_configurations[0].HardwareFailureAlarmActions.ResetRequired	= 	false;
	_configurations[0].HardwareFailureAlarmActions.LogAlarm			= 	true;
	_configurations[0].HardwareFailureAlarmActions.GeneralAlarm		= 	true;
	_configurations[0].HardwareFailureAlarmActions.CustomAlarmOutput= 	false;
	_configurations[0].HardwareFailureAlarmActions.CycleCounter		= 	false;

	_configurations[0].NoCycleOverloadAlarmActions.ResetRequired	= 	false;		
	_configurations[0].NoCycleOverloadAlarmActions.LogAlarm			= 	true;
	_configurations[0].NoCycleOverloadAlarmActions.GeneralAlarm		= 	true;
	_configurations[0].NoCycleOverloadAlarmActions.CustomAlarmOutput= 	true;
	_configurations[0].NoCycleOverloadAlarmActions.CycleCounter		= 	false;
	
	memcpy(&_configurations[1], &_configurations[0], sizeof(AlarmConfiguration));
}


/**************************************************************************//**
* \brief   - Returns the default alarm action configuration of the various
* 			alarm types
*
* \param   	- pAlarmConfiguration defaultAlarmConfig
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::GetDefaultConfiguration (pAlarmConfiguration defaultAlarmConfig)
{
	memcpy(defaultAlarmConfig, &_configurations[0], sizeof(AlarmConfiguration));	
}


/**************************************************************************//**
* \brief   - Returns the current alarm action configuration of the various
* 			alarm types
*
* \param   	- pAlarmConfiguration defaultAlarmConfig
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::GetCurrentConfiguration (pAlarmConfiguration alarmConfig)
{
	memcpy(alarmConfig, &_configurations[1], sizeof(AlarmConfiguration));	
}


/**************************************************************************//**
* \brief   - Sets the alarm action configuration for the various
* 			alarm types
*
* \param   	- pAlarmConfiguration defaultAlarmConfig
*
* \return  - None
*
******************************************************************************/
void AlarmMgr::SetCurrentConfiguration(pAlarmConfiguration alarmConfig)
{
	memcpy(&_configurations[1], alarmConfig, sizeof(AlarmConfiguration));	
}

