/*
 * AlarmTask.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: DShilonie
 *      
 *      The alarm task will receive messages from the control task, SDO task and possibly the database task, whenever an
 *      of the alarm, saving information in the DB and notifying the UIC. It will do so by using member alarm manager which
 *      encapsulates the management of the alarm event list. 
 *      
 */

#include "CommonProperty.h"
#include  "ControlTask.h"
#include  "SC_Common.h"
#include  "SC.h"
#include  "ConsoleApp.h"
#include "UserInterface.h"
#include  "MainTask.h"
#include "AlarmTask.h"
#include "Commons.h"
#include "Counters.h"

using namespace std;

/**************************************************************************//**
* \brief  - Acquires queue handles for proper intertask communications
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
AlarmTask::AlarmTask(): DB_MSG_Q_ID(0), CTRL_MSG_Q_ID(0), ALARM_MSG_Q_ID(0), SDO_MSG_Q_ID(0), UI_MSG_Q_ID(0) 
{
	CP = CommonProperty::getInstance();
	
	CTRL_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[CTRL_T]);
	DB_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[DB_T]);
	SDO_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[SDO_T]);
	ALARM_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[ALARM_T]);
	UI_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[UI_T]);
	MAINTENANCE_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[MAINTENANCE_T]);
	
	message.msgID = 0;
}

/**************************************************************************//**
* \brief  - Posts the encoded message to the destination message queue of
* 			a given task  
*
* \param  - reference to msgBuffer
* \param  - const reference to msgQID
* \param  - _Vx_ticks_t waitType	
*
* \return  - STATUS - OK or ERROR
*
******************************************************************************/
STATUS AlarmTask::SendToMsgQ (Message& msgBuffer, const MSG_Q_ID &msgQID, _Vx_ticks_t waitType)
{
	STATUS ret = OK;
	if(msgQSend(msgQID, reinterpret_cast<char*>(&msgBuffer), sizeof(msgBuffer), waitType, MSG_PRI_NORMAL) != OK)
	{
		LOGDBG("\nALARM_T : ----------- Error on msgQSend",0,0,0);

		ret = ERROR;
	}
	return ret;
}


/**************************************************************************//**
* \brief  - Gets state of EtherCat master stack state.  
*
* \param  - None
*
* \return  - EC_MASTER_STATE
*
******************************************************************************/
EC_MASTER_STATE AlarmTask::EcatGetMasterStateSysCall ()
{
	INT32 iState = syscall(0, 0, 0, 0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,2));
	return (EC_MASTER_STATE)iState;
}


/**************************************************************************//**
* \brief  - Gets the original alarm ID and type from the bit mask and enters the
* 			alarm into the list. In most cases, notifies the UIC
*
* \param  - UINT16 source
* 			UINT16 id
* 			ALARM_INFO& alarm_info
*
* \return  - ALARM_INFO& alarm_info
*
******************************************************************************/
void AlarmTask::ProcessAlarmID (UINT16 source, UINT16 id, ALARM_INFO& alarm_info)
{
	UINT16 i;
	Message msg;

	msg.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_ALARMS;
	SendToMsgQ (msg, MAINTENANCE_MSG_Q_ID, NO_WAIT);
	
	switch (source)
	{
	case ALARM_PC:
		
		alarm_info.id = 0;
		
		if ((id & (UINT16) ERR_TEMP_OVERLOAD) != NO_ERROR)
		{
			// need to determine type 
			alarm_info.alarmType = TYPE_NOCYCLE_OVERLOAD | OVERLOAD_THERMAL;
			alarm_info.id = alarm_info.alarmType | ALARM_TEMP_OVERLOAD_OVA;

			// need to map type to action
			alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);
			
			// save the alarm event
			if (alarmMgr.SaveAlarmInfo (alarm_info))
			{
				// retrieve and post 
				if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
				{
					SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND,  alarm_info.description, WAIT_FOREVER);
					
					// Send data to the DB task to save in the database as well
					if (alarm_info.flags.LogAlarm)
						SendAlarmDataToDataBase (alarm_info);
				}
			}

			msg.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_OVERLOADS;
			SendToMsgQ (msg, MAINTENANCE_MSG_Q_ID, NO_WAIT);
		}
		if ((id & (UINT16) ERR_CURRENT_OVERLOAD) != NO_ERROR)
		{
			// need to determine type
			ProcessAlarmType (source, alarm_info);
			alarm_info.id = alarm_info.alarmType | ALARM_CURR_OVERLOAD_OVA;

			// need to map type to action
			alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);

			// save the alarm event
			if (alarmMgr.SaveAlarmInfo (alarm_info))
			{
				// retrieve and post 
				if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
				{
					SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND,  alarm_info.description, WAIT_FOREVER);
	
					// Send data to the DB task to save in the database as well
					if (alarm_info.flags.LogAlarm)
						SendAlarmDataToDataBase (alarm_info);
				}
			}
			msg.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_OVERLOADS;
			SendToMsgQ (msg, MAINTENANCE_MSG_Q_ID, NO_WAIT);

		}
		if ((id & (UINT16) ERR_VOLTAGE_OVERLOAD) != NO_ERROR)
		{
			// need to determine type
			ProcessAlarmType (source, alarm_info);
			alarm_info.id = alarm_info.alarmType | ALARM_VOLT_OVERLOAD_OVA;

			// need to map type to action
			alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);

			// save the alarm event
			if (alarmMgr.SaveAlarmInfo (alarm_info))
			{
				// retrieve and post 
				if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
				{
					SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND, alarm_info.description, WAIT_FOREVER);
	
					// Send data to the DB task to save in the database as well
					if (alarm_info.flags.LogAlarm)
						SendAlarmDataToDataBase (alarm_info);
				}
			}
			msg.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_OVERLOADS;
			SendToMsgQ (msg, MAINTENANCE_MSG_Q_ID, NO_WAIT);

		}
		if ((id & (UINT16) ERR_POWER_OVERLOAD) != NO_ERROR)
		{
			// need to determine type
			ProcessAlarmType (source, alarm_info);
			alarm_info.id = alarm_info.alarmType | ALARM_POWER_OVERLOAD_OVA;

			// need to map type to action
			alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);

			// save the alarm event
			if (alarmMgr.SaveAlarmInfo (alarm_info))
			{
				// retrieve and post 
				if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
				{
					SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND,  alarm_info.description, WAIT_FOREVER);
	
					// Send data to the DB task to save in the database as well
					if (alarm_info.flags.LogAlarm)
						SendAlarmDataToDataBase (alarm_info);
				}
			}
			msg.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_OVERLOADS;
			SendToMsgQ (msg, MAINTENANCE_MSG_Q_ID, NO_WAIT);

		}
		if ((id & (UINT16) ERR_DDSFREQ_LOWSEEKLIMIT) != NO_ERROR 
				|| (id & (UINT16) ERR_DDSFREQ_HIGHSEEKLIMIT)  != NO_ERROR)
		{
			// need to determine type
			ProcessAlarmType (source, alarm_info);
			alarm_info.id = alarm_info.alarmType | ALARM_FREQ_OVERLOAD_OVA;

			// need to map type to action
			alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);

			// save the alarm event
			if (alarmMgr.SaveAlarmInfo (alarm_info))
			{
				// retrieve and post 
				if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
				{
					SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND,  alarm_info.description, WAIT_FOREVER);
	
					// Send data to the DB task to save in the database as well
					if (alarm_info.flags.LogAlarm)
						SendAlarmDataToDataBase (alarm_info);
				}
			}
			msg.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_OVERLOADS;
			SendToMsgQ (msg, MAINTENANCE_MSG_Q_ID, NO_WAIT);
		}
		if ((id & (UINT16) ERR_DDSFREQ_LOWWELDLIMIT)  != NO_ERROR
				|| (id & (UINT16) ERR_DDSFREQ_HIGHWELDLIMIT) != NO_ERROR)
		{
			// need to determine type
			ProcessAlarmType (source, alarm_info);
			alarm_info.id = alarm_info.alarmType | ALARM_FREQ_OVERLOAD_OVA;
			
			// need to map type to action
			alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);

			// save the alarm event
			if (alarmMgr.SaveAlarmInfo (alarm_info))
			{
				// retrieve and post 
				if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
				{
					SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND, alarm_info.description, WAIT_FOREVER);
	
					// Send data to the DB task to save in the database as well
					if (alarm_info.flags.LogAlarm)
						SendAlarmDataToDataBase (alarm_info);
				}
			}
			msg.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_OVERLOADS;
			SendToMsgQ (msg, MAINTENANCE_MSG_Q_ID, NO_WAIT);
		}
		if ((id & (UINT16) ERR_15V_LOST) != NO_ERROR)
		{
					alarm_info.id = ALARM_SONICS_SOURCE_LOST_EFA;  
							
					// determine type
					alarm_info.alarmType = alarm_info.id & ALARMTYPE_MASK;
					
					// need to map type to action
					alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);

					// save the alarm event
					if (alarmMgr.SaveAlarmInfo (alarm_info))
					{
						// retrieve and post 
						if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
						{
							SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND, alarm_info.description, WAIT_FOREVER);
			
							// Send data to the DB task to save in the database as well
							if (alarm_info.flags.LogAlarm)
								SendAlarmDataToDataBase (alarm_info);
						}
					}
		}

		// alarm ID unknown
		if (!alarm_info.id)
			ProcessAlarmUnknown (alarm_info);

		// all reported alarms have been processed
		 if (alarmMgr.GetAlarmActionCount (COUNT_RESET_REQ, ALARM_PC) == 0)
			PC_RX->MasterEvents |=  (UINT16) BIT_MASK(CTRL_PC_CLEAR_ALARMS);
		break;
		
	case ALARM_AC:
		
		alarm_info.id = 0;
		
		//check each alarm bit
		for(i=0;i<16;i++) //sizeof(id) didn't work...
		{
			if(id & BIT_MASK(i))
			{
				switch (BIT_MASK(i))
				{
				case ERR_PART_WINDOW:
					alarm_info.id = ALARM_PART_WINDOW_ABORT_NCA;
					break;
				case ERR_SERVO_MOTOR:
					alarm_info.id = ALARM_SERVO_MOTOR_ERROR_NCA;
					break;
				case ERR_RECALIBRATE:
					alarm_info.id = ALARM_RECALIBRATE_ACTUATOR_EFA;
					break;
				case ERR_ELC_UNKNOWN:
					alarm_info.id = ALARM_ELC_UNKNOWN;
					break;
				case ERR_ELC_PB:
					alarm_info.id = ALARM_ELC_PB;
					break;
				case ERR_ELC_24V:
					alarm_info.id = ALARM_ELC_24V;
					break;
				case ERR_ELC_ESTOP:
					alarm_info.id = ALARM_ELC_ESTOP;
					break;
				case ERR_ELC_LE:
					alarm_info.id = ALARM_ELC_LE;
					break;
				case ERR_ELC_SBEAM:
					alarm_info.id = ALARM_ELC_SBEAM;
					break;
				case ERR_ELC_TRS:
					alarm_info.id = ALARM_ELC_TRS;
					break;
				case ERR_ELC_DRIVE:
					alarm_info.id = ALARM_ELC_DRIVE;
					break;
				case ERR_ELC_CM:
					alarm_info.id = ALARM_ELC_CM;
					break;
				case ERR_ELC_LU:
					alarm_info.id = ALARM_ELC_LU;
					break;
				case ERR_ELC_SONICS_EN:
					alarm_info.id = ALARM_ELC_SONICS_EN;
					break;
				}
				// determine type
				alarm_info.alarmType = alarm_info.id & ALARMTYPE_MASK;

				// need to map type to action
				alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);

				// save the alarm event
				if (alarmMgr.SaveAlarmInfo (alarm_info))
				{
					// retrieve and post 
					if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
					{
						SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND,  alarm_info.description, WAIT_FOREVER);

						// Send data to the DB task to save in the database as well
						if (alarm_info.flags.LogAlarm)
							SendAlarmDataToDataBase (alarm_info);
					}
				}
			}
		}

		// alarm ID unknown
		if (!alarm_info.id)
			ProcessAlarmUnknown (alarm_info);

		// all reported alarms have been processed
		 if (alarmMgr.GetAlarmActionCount (COUNT_RESET_REQ, ALARM_AC) == 0)
			AC_RX->ACControlEvent |=  (UINT16) BIT_MASK(EVENT_AC_CLEAR_ALARMS);
		break;
		
	case ALARM_SC:
		
		if (!alarm_info.id)
		{
			// alarm ID unknown
			ProcessAlarmUnknown (alarm_info);
		}
		else
		{
			// save the alarm event
			if (alarmMgr.SaveAlarmInfo (alarm_info))
			{
				// retrieve and post 
				if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
				{
					SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND,  alarm_info.description);
	
					// Send data to the DB task to save in the database as well
					if (alarm_info.flags.LogAlarm)
						SendAlarmDataToDataBase (alarm_info);
				}
			}
		}
		break;
	}
}


/**************************************************************************//**
* \brief  - This method maps a source of alarm and the weld state to an
*			alarm type with sub type if needed. The returned reference will be used
*			to further create the correct alarm ID
*				 
* \param  	- UINT16 source of alarm 
* 			- ALARM_INFO& alarm_info
*
* \return  - ALARM_INFO& alarm_info
*
******************************************************************************/
void AlarmTask::ProcessAlarmType (UINT16 source, ALARM_INFO& alarm_info)
{
	SCSTATES state = alarmMgr.GetAlarmSCState (source);
	ActiveRecipe activeRecipe (CP->getActiveRecipeInstance());
	
	if (source == ALARM_PC)
	{
		alarm_info.alarmType = TYPE_NOCYCLE_OVERLOAD;
		
		if (state == SEEK_SONIC_ON || state == SCAN_SONIC_ON)
		{
			// determine sub type here...
			alarm_info.alarmType |= OVERLOAD_SEEK;
		}
		else if (state == TEST_SONICS_ON)
		{
			// determine sub type here...
			alarm_info.alarmType |= OVERLOAD_TEST;
		}
		else
		{
			alarm_info.alarmType = TYPE_OVERLOAD;
	
			if (state == WAIT_FOR_READY_POSITION)
			{
				// one of these must be true at this point
				if (PC_TX->PCState == SEEK_RUN)
					alarm_info.alarmType |= OVERLOAD_POST_WELD;
				else if (activeRecipe.GetAfterBurst().bIsEnabled)
					alarm_info.alarmType |= OVERLOAD_AFTER_BURST;
			}
			else if (state == WAIT_FOR_TRIGGER)
			{
				alarm_info.alarmType = TYPE_NOCYCLE_OVERLOAD;
				alarm_info.alarmType |= OVERLOAD_PRE_TRIGGER;
			}
			else
			{
				alarm_info.alarmType |= OVERLOAD_WELD;
			}
		}
	}
}


/**************************************************************************//**
* \brief  -   This method  processes the unlikely alarm with with no ID 
*				Those are set to reset required.	
*				 
* \param  	- ALARM_INFO& alarm_info
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessAlarmUnknown (ALARM_INFO& alarm_info)
{
	char description[MAX_SIZE_OF_MSG_BUFF];
	
	// need to map ID to type here...
	alarm_info.alarmType = TYPE_NOCYCLE;

	// need to map type to action
	alarmMgr.GetAlarmActions (alarm_info.alarmType, alarm_info.id, alarm_info.flags);
	alarm_info.flags.ResetRequired = true;

	snprintf(description, MAX_SIZE_OF_MSG_BUFF, "%d%X", alarm_info.flags.ResetRequired, alarm_info.id);

	alarm_info.description = description;

	// save the alarm event
	if (alarmMgr.SaveAlarmInfo (alarm_info))
	{
		SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ALARM_IND,  alarm_info.description, WAIT_FOREVER);
	
		// Send data to the DB task to save in the database as well
		if (alarm_info.flags.LogAlarm)
			SendAlarmDataToDataBase (alarm_info);
	}
}


/**************************************************************************//**
* \brief  - Decodes the received buffer from message queue.  
*
* \param  - pRecvBuffer
* \param  - message
*
* \return  - None
*
******************************************************************************/
void AlarmTask::DeCoder (char *pRecvBuffer, Message& message)
{
	memcpy (&message, pRecvBuffer, sizeof (message));
}


/**************************************************************************//**
* \brief  - Process commands received by the message queue from other tasks
* 			It is the entry point for communications with the alarm task
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessCommandMessage ()
{
	ALARM_INFO alarm_info;
	
	memset(recvMsgBuffer,0x00,sizeof(recvMsgBuffer));
	EC_MASTER_STATE EcMasterState = EcatGetMasterStateSysCall();

	/* check the state of the Ec-Master before reading the MsgQ to send the PDO data*/
	if(ecat_OP == EcMasterState)
	{
		if(msgQReceive (ALARM_MSG_Q_ID, recvMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
		{
			DeCoder(recvMsgBuffer, message);
			
			switch(message.msgID)
			{
			case TO_ALARM_TASK_ALARM_EVENT:
				
				// prepare info here
				alarm_info.source = ((pALARM_INFO) message.Buffer)->source;
				
				// send for further processing
				ProcessAlarmEvent (alarm_info);
				break;
				
			case TO_ALARM_TASK_ESTOP_EVENT:
				ProcessAlarmEstop (alarm_info);
				break;
				
			case TO_ALARM_TASK_ESTOP_RELEASE:
				SendEstopEventUI (TO_UI_TASK_EMERGENCY_STOP_RESET_IND);
				break;
				
			case TO_ALARM_TASK_ESTOP_RESET:
				
				ProcessEstopReset (alarm_info);
				break;

			case TO_ALARM_TASK_ALARM_INFO_SC:
				
				// receives reset alarm info and send for further processing here
				ProcessAlarmInfo (ALARM_SC, alarm_info);
				break;

			case TO_ALARM_TASK_ALARM_INFO_PC:
				
				// receives slave alarm info and send for further processing here
				ProcessAlarmInfo (ALARM_PC, alarm_info);
				break;

			case TO_ALARM_TASK_ALARM_INFO_AC:
				
				// receives slave alarm info and send for further processing here
				ProcessAlarmInfo (ALARM_AC, alarm_info);
				break;

			case TO_ALARM_TASK_RESET_ALARM:
				
				ProcessAlarmReset (alarm_info);
				break;
				
			case TO_ALARM_TASK_ACTIVE_ALARM_REQ:
				
				if (!alarmMgr.GetAllActiveAlarms (message))
					sprintf(message.Buffer,"%d", RESPONSE_FAIL);
				
				SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_ACTIVE_ALARM_REQ);
				break;
				
			case TO_ALARM_TASK_CREATE_DEFAULT_ALARM_CONFIG:
				
				// get default configuration from the alarm manager
				alarmMgr.GetDefaultConfiguration ((pAlarmConfiguration) message.Buffer);
				
				// send a message to the DB task to save in the database as well
				message.msgID = TO_DB_TASK_CREATE_DEFAULT_ALARM_CONFIG;
				SendToMsgQ (message, DB_MSG_Q_ID, NO_WAIT);
				break;

			case TO_ALARM_TASK_CREATE_CURRENT_ALARM_CONFIG:
				
				// save configuration from DB as current in the alarm manager
				alarmMgr.SetCurrentConfiguration ((pAlarmConfiguration) message.Buffer);
				break;
				
			case TO_ALARM_TASK_DEFAULT_ALARM_CONFIG_REQ:
				
				// get default configuration from the alarm manager
				alarmMgr.GetDefaultConfiguration ((pAlarmConfiguration) message.Buffer);
				SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_DEFAULT_CONFIG);
				break;
				
			case TO_ALARM_TASK_ALARM_CONFIG_REQ:
				
				// get current configuration from the alarm manager
				alarmMgr.GetCurrentConfiguration ((pAlarmConfiguration) message.Buffer);
				SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_CONFIG_REQ);
				break;

			case TO_ALARM_TASK_ALARM_CONFIG_SAVE_REQ:
				
				// save new configuration from UIC in the alarm manager
				alarmMgr.SetCurrentConfiguration ((pAlarmConfiguration) message.Buffer);
				
				// send a message to the DB task to save in the database as well
				message.msgID = TO_DB_TASK_ALARM_CONFIG_SAVE_REQ;
				SendToMsgQ (message, DB_MSG_Q_ID, NO_WAIT);
				break;
				
			case TO_ALARM_TASK_ALARM_CONFIG_SAVE:
				
				// send save status to the UI task
				SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_CONFIG_SAVE);
				break;
				
			case TO_ALARM_TASK_CLEAR_ALL:
				
				// auto request to clear out all alarms
				alarmMgr.ClearAllAlarmInfo();
				break;
				
			case TO_ALARM_TASK_UI_CLEAR_ALL_REQ:
				
				sprintf(message.Buffer,"%d", RESPONSE_SUCCESS);
				SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_RESET_ALL_REQ);
				break;

			case TO_ALARM_TASK_UI_CLEAR_ALL:

				// user request to clear out all alarms
				ProcessResetAll (message.msgID);
				break;
				
			case TO_ALARM_TASK_RESET_INPUT:

				// reset signal asserted, attempt to clear all alarms
				ProcessResetInput ();
				break;

			default:
				LOGDBG("\nALARM_T : ------- Unknown message received",0,0,0);
				break;
			}
		}
	}
}


/**************************************************************************//**
* \brief  - Process resulting event entry from from the message queue.
*
* \param  - ALARM_INFO& alarm_info
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessAlarmEvent (ALARM_INFO& alarm_info)
{
	switch (alarm_info.source)
	{
	case ALARM_SC:

		alarm_info.alarmType = ((pALARM_INFO) message.Buffer)->alarmType;
		alarm_info.flags = ((pALARM_INFO) message.Buffer)->flags;
		alarm_info.id = ((pALARM_INFO) message.Buffer)->id;
		alarm_info.priority = AlarmEvent::PRIORITY_HIGH;
		alarm_info.scState = ((pALARM_INFO) message.Buffer)->scState;

		
		message.msgID = TO_DB_TASK_ALARM_DATA_SC;
		SendToMsgQ (message, DB_MSG_Q_ID, NO_WAIT);

		// process based on alarm event ID and type
		ProcessAlarmID (alarm_info.source, alarm_info.id, alarm_info);
		break;
		
	case ALARM_PC:
		
		// request more info before entering the alarm event into the manager
		message.msgID = TO_SDO_TASK_UL_ALARM_DATA_REQ_PC;
		SendToMsgQ (message,SDO_MSG_Q_ID, NO_WAIT);
		break;

	case ALARM_AC:

		// request more info before entering the alarm event into the manager
		message.msgID = TO_SDO_TASK_UL_ALARM_DATA_REQ_AC;
		SendToMsgQ (message,SDO_MSG_Q_ID, NO_WAIT);
		break;

	default:
		LOGDBG("\nALARM_T : ------- ProcessAlarmEvent: Unknown",0,0,0);
		break;
	}
}



/**************************************************************************//**
* \brief  - Process resulting alarm state received from the actuator
* 			in the case of EStop
*
* \param  - ALARM_INFO& alarm_info
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessAlarmEstop(ALARM_INFO& alarm_info)
{
	char description[MAX_SIZE_OF_MSG_BUFF];
	
	alarm_info.source = ((pALARM_INFO) message.Buffer)->source;
	alarm_info.id = ((pALARM_INFO) message.Buffer)->id;
	alarm_info.priority = AlarmEvent::PRIORITY_HIGH;
	alarm_info.scState = ((pALARM_INFO) message.Buffer)->scState;
	
	// determine type
	alarm_info.alarmType = alarm_info.id & ALARMTYPE_MASK;

	// these actions will always be apart of E-Stop
	alarm_info.flags.ResetRequired = 1;
	alarm_info.flags.GeneralAlarm = 1;
	alarm_info.flags.LogAlarm = 0;

	snprintf(description, MAX_SIZE_OF_MSG_BUFF, "%d%X", alarm_info.flags.ResetRequired, alarm_info.id);

	alarm_info.description = description;

	// save the alarm event
	if (alarmMgr.SaveAlarmInfo (alarm_info))
	{
		// retrieve and post 
		if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
		{
			SendEstopEventUI (TO_UI_TASK_EMERGENCY_STOP_ACTIVE_IND);
		}
	}
	
}


/**************************************************************************//**
* \brief  - Process resulting alarm info received from the slaves or fromthe DB
* 			in the case of the SC itself
*
* \param  - UINT16 origin
* 			ALARM_INFO& alarm_info
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessAlarmInfo (UINT16 origin, ALARM_INFO& alarm_info)
{
	switch (origin)
	{

	case ALARM_PC:

		// common for now
		alarm_info.source = ALARM_PC;
		alarm_info.priority = AlarmEvent::PRIORITY_HIGH;
		alarm_info.scState = (UINT16) alarmMgr.GetAlarmSCState (origin);
		
		// point to to alarmLog which was copied from the SDO 
		alarm_info.alarmLog = message.Buffer;
		
		// process based on alarm event ID and type
		ProcessAlarmID (origin, ((PCAlarmData *) message.Buffer)->Overloads, alarm_info);
		break;

	case ALARM_AC:

		alarm_info.source = ALARM_AC;
		alarm_info.priority = AlarmEvent::PRIORITY_HIGH;
		alarm_info.scState = (UINT16) alarmMgr.GetAlarmSCState (origin);
		
		// point to to alarmLog which was copied from the SDO 
		alarm_info.alarmLog = message.Buffer;

		// process based on alarm event ID and type
		ProcessAlarmID (origin, ((ACAlarmData *) message.Buffer)->ACAlarm, alarm_info);
		break;
		
	default:
		LOGDBG("\nALARM_T : ------- ProcessAlarmInfo: Unknown",0,0,0);
		break;
	}
	
}


/**************************************************************************//**
* \brief  - Process reset request received from the UIC
*
* \param  - ALARM_INFO& alarm_info
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessAlarmReset (ALARM_INFO& alarm_info)
{
	bool failed = false;
	
	// get the message ID
	alarm_info.id = strtol (message.Buffer, NULL, 16);
	
	// look for the alarm event
	if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
	{
		// try to clear it
		if (!alarmMgr.ClearAlarmInfo (alarm_info.id))
			failed = true;
	}

	if (failed)
	{
		sprintf(message.Buffer,"%d", RESPONSE_FAIL);
		SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_RESET_RESP);
	}
	else
	{
		sprintf(message.Buffer,"%d", RESPONSE_SUCCESS);
		SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_RESET_RESP);
	}
}



/**************************************************************************//**
* \brief  - Process E-Stop reset request received from the UIC
*
* \param  - ALARM_INFO& alarm_info
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessEstopReset (ALARM_INFO& alarm_info)
{
	// get the message ID
	alarm_info.id = ALARM_ESTOP_NCA;
	
	// look for the alarm event
	if (alarmMgr.GetAlarmInfo (alarm_info.id, alarm_info))
	{
		if (alarmMgr.ClearAlarmInfo (alarm_info.id))
		{
			// successful reset
			SendEstopEventUI (TO_UI_TASK_ESTOP_RESET_RES);
		}
	}
	else
	{
		// must have been reset before
		SendEstopEventUI (TO_UI_TASK_ESTOP_RESET_RES);
	}
}


/**************************************************************************//**
* \brief  - Process reset all request received from the UIC
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessResetAll (UINT32 msgId)
{
	alarmMgr.ClearAllAlarmInfo();

	switch (msgId)
	{
		case TO_ALARM_TASK_UI_CLEAR_ALL:
		
		sprintf(message.Buffer,"%d", RESPONSE_SUCCESS);
		SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_RESET_ALL_RESP);
		break;
		
		default:
			break;
	}
}


/**************************************************************************//**
* \brief  - Process reset all request received from the RESET input signal.
* 			Persistent alarms will not be removed, and alarm state will be
*			maintained.
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void AlarmTask::ProcessResetInput()
{
	if (alarmMgr.ClearAlarmResetInput() == 0)
	{
		sprintf(message.Buffer,"%d", RESPONSE_SUCCESS);
		SendAlarmEventUI (TO_UI_TASK_ALARM_MGR_RESET_ALL_REQ);
	}
}


/**************************************************************************//**
* \brief  - Sends alarm event information to the UIC.
*
* \param  - INT32 msgID
* \param  - char * altMsg - option to alter the already embedded message in the call
* \param  - Vx_ticks_t waitType
*
* \return  - STATUS status
*
******************************************************************************/
STATUS AlarmTask::SendAlarmEventUI (INT32 msgID, char * altMsg, _Vx_ticks_t waitType)
{
	STATUS status;
	Message msg;
	memset(msg.Buffer,0x00,sizeof(msg.Buffer));
	
	if (altMsg)
	{
		memcpy (msg.Buffer, altMsg, sizeof (msg.Buffer));
		msg.msgID = msgID;
		status = SendToMsgQ (msg, UI_MSG_Q_ID, waitType);
	}
	else
	{
		message.msgID = msgID;
		status = SendToMsgQ (message, UI_MSG_Q_ID, waitType);
	}
	
	return status;
}


/**************************************************************************//**
* \brief  - Sends E-Stop event information to the UIC and records this event
* 			in the system event log.
*
* \param  - INT32 msgID - what kind of event to send to the UIC
* 			
* \return  - STATUS
*
******************************************************************************/
STATUS AlarmTask::SendEstopEventUI (UINT32 msgID)
{
	STATUS status;
	Message msg;
	
	switch (msgID)
	{
		case TO_UI_TASK_EMERGENCY_STOP_ACTIVE_IND:

			message.msgID = msgID;
			sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
			status = SendToMsgQ (message, UI_MSG_Q_ID, NO_WAIT);
		
			message.msgID = TO_DB_TASK_EMERGENCY_STOP_ACTIVE_IND;
			status = SendToMsgQ (message, DB_MSG_Q_ID, NO_WAIT);
			break;
			
		case TO_UI_TASK_EMERGENCY_STOP_RESET_IND:
			
			message.msgID = msgID;
			sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
			status = SendToMsgQ (message, UI_MSG_Q_ID, NO_WAIT);
			
			message.msgID = TO_DB_TASK_EMERGENCY_STOP_RESET_IND;
			status = SendToMsgQ (message, DB_MSG_Q_ID, NO_WAIT);
			break;
			
		case TO_UI_TASK_ESTOP_RESET_RES:
			
			message.msgID = msgID;
			sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
			status = SendToMsgQ (message, UI_MSG_Q_ID, NO_WAIT);
			break;
			
		default:
			status = ERROR;
			break;
	}
	
	return status;
}


/**************************************************************************//**
* \brief  - Sends alarm event info to DB task for storage
*
* \param  - ALARM_INFO& alarm_info
*
* \return  - STATUS status
*
******************************************************************************/
STATUS AlarmTask::SendAlarmDataToDataBase (ALARM_INFO& alarm_info)
{
	STATUS status;
	Message msg;
	
	((pALARM_INFO) msg.Buffer)->id = alarm_info.id;
	((pALARM_INFO) msg.Buffer)->source = alarm_info.source;
	((pALARM_INFO) msg.Buffer)->flags = alarm_info.flags;
	((pALARM_INFO) msg.Buffer)->alarmType = alarm_info.alarmType;

	msg.msgID = TO_DB_TASK_ALARM_MGR_ALARM_DATA;
	status = SendToMsgQ (msg, DB_MSG_Q_ID, NO_WAIT);
	
	return status;
}


/**************************************************************************//**
* \brief  - Gets main task's run status
*
* \param  - None
*
* \return  - bool TRUE or FALSE
*
******************************************************************************/
bool AlarmTask::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}


/**************************************************************************//**
* \brief  - AlarmTask destructor
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
AlarmTask::~AlarmTask()
{
	CP = NULL;
}


/**************************************************************************//**
* \brief  - AlarmTask entry point and constructor. 
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void Alarm_Task (void)
{	
	AlarmTask * pAlarmTask 	= new (nothrow) AlarmTask();

	if(NULL != pAlarmTask)
	{
		/* Alarm Task loop and the bIsTaskRun flag enabled when task created */
		while (pAlarmTask->bIsTaskRunStatus())
		{
			pAlarmTask->ProcessCommandMessage();
		}

		delete pAlarmTask;
	}
	else
	{
		LOGDBG("\nALARM_T : ------------- Allocation failed",0,0,0);
	}

	LOGDBG ("ALARM_T : ------------- Task Terminated",0,0,0);

	taskDelete(taskIdSelf());
}

