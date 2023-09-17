/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Control task of SC for different kinds of Weld/Seek operations and OD communication with slaves
 
**********************************************************************************************************/

#include  "ControlTask.h"
#include  "SCStateMachine.h"
#include  "SC_Common.h"
#include  "SC.h"
#include  "WaitForReadyPosition.h"
#include  "ConsoleApp.h"
#include  "MainTask.h"
#include "AlarmManager.h"
#include "ScDgtInput.h"
#include "ScDgtOutput.h"
#include "Eeprom.h"
#include "Counters.h"
#include "McSPI.h"
#include "UIDBInterface.h"
#include  "Utils.h"
#include  "FieldBus/FieldBus.h"
extern "C"
{
#include <customSystemCall.h>
}

using namespace std;


/* Static member variables are initialized */
OPERATION 	ControlTask::OpType				= NO_OPERATION;
OPERATION	ControlTask::PrevOpType			= NO_OPERATION;
OPERATION	ControlTask::nextOpType			= NO_OPERATION;
UINT8		ControlTask::calibrationStep 	= 0;
TASK_ID		ControlTask::Console_TID		= 0;
bool   		ControlTask::bIsResultConfig	= false;
bool		ControlTask::m_bIsLimitExceed	= false;

UINT8    	ControlTask::WeldOpFlag 		= FALSE;
UINT8 		ControlTask::PostWeldDoneFlag 	= FALSE;
UINT8 		ControlTask::PreWeldDoneFlag 	= FALSE;
UINT8 		ControlTask::emmcWearLevel		= EMMC_WEAR_LEVEL_LOW;

bool		ControlTask::m_bIsIndicateUIC 	= false;
bool		ControlTask::m_bIsHornClampOn	= false;
UINT32		ControlTask::m_CycleCounter		= 0;		/* Will be assigned with latest counter from DB on power up */
ActuatorSetup	ControlTask::m_SetupData;
UINT32		ControlTask::m_PartsPerMinCounter = 0;
bool		ControlTask::m_bIsWeldAllowed = 0;

// default initialization of the inputs and outputs here
static DigitalIO::DIGITAL_FUNCTION _digitalInputs[DIGITAL_IO_MAX_INPUTS] = {
		
		// SC inputs
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},		
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},		
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},		
		{DigitalIO::INPUT_PSWD_OVERRIDE, DigitalIO::STATE_ACTIVE_HIGH, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY}, 
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		
		// actuator inputs
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},		
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR}, 
		{DigitalIO::INPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
};

static DigitalIO::DIGITAL_FUNCTION _digitalOutputs[DIGITAL_IO_MAX_OUTPUTS] = {
		
		// SC outputs
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::OUTPUT_PSWD_OVERRIDE, DigitalIO::STATE_ACTIVE_HIGH, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_POWERSUPPLY},
		
		// AC outputs
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
		{DigitalIO::OUTPUT_FUNCTION_NONE, DigitalIO::STATE_NONE, DigitalIO::SOURCE_ACTUATOR},
};

/**************************************************************************//**
* \brief   - Initialize and assign the message queue of DB and CTRL.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
ControlTask::ControlTask(): DB_MSG_Q_ID(0), CTRL_MSG_Q_ID(0),UI_DB_MSG_Q_ID(0),
							events(0),m_Setup(0)
{
	CP				= CommonProperty::getInstance(); 	
	CTRL_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[CTRL_T]);
	SDO_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[SDO_T]);
	UI_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[UI_T]);
	DB_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[DB_T]);
	ALARM_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[ALARM_T]);
	UI_DB_MSG_Q_ID	= CP->GetMsgQId(cTaskName[UI_DB_T]);
	WEBSERVICE_MSG_Q_ID	= CP->GetMsgQId(cTaskName[WEBSERVICE_T]);
	
	Console_TID		= CP->GetTaskId(cTaskName[CONSOLE_T]);	
	MAIN_TID 		= CP->GetTaskId(cTaskName[MAIN_T]);
	bIsPowerup	    = true;
	
	// program the IO manager with defaults
	digitalIOMgr.SetInputs (_digitalInputs);
	digitalIOMgr.SetOutputs (_digitalOutputs);

	//Initializing Task Id's
	IO_Image_TID= CP->GetTaskId(cTaskName[IO_IMAGE_HANDLER_T]);
	Mail_Box_TID= CP->GetTaskId(cTaskName[MAIL_BOX_HANDLER_T]);	
}

/**************************************************************************//**
* \brief   - Post the encoded message to destination message queue.
*
* \param   - Message& MsgBuffer, MSG_Q_ID - Destination queue.
* 
* \return  - STATUS - OK or ERROR
*
******************************************************************************/
STATUS ControlTask::SendToMsgQ(Message& msgBuffer, const MSG_Q_ID &msgQID)
{
	STATUS ret = OK;
	if(msgQSend(msgQID, reinterpret_cast<char*>(&msgBuffer), sizeof(msgBuffer), NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("Ctrl_T : -----------Error on msgQSend----------",0,0,0);
		ret = ERROR;
	}
	return ret;
}

/**************************************************************************//**
* \brief   - Decodes the received buffer from message queue.
*
* \param   - char *recvBuffer, message& MsgID
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::DeCoder(char *recvBuffer, Message& msgDecoder)
{
	memcpy(&msgDecoder,recvBuffer,sizeof(msgDecoder));
	m_Setup = msgDecoder.Buffer[0];
}

/**************************************************************************//**
* \brief   - Gets state of EtherCat master stack state.
*
* \param   - None.
* 
* \return  - EC_MASTER_STATE (init, pre-op, safe-op or operational).
*
******************************************************************************/
EC_MASTER_STATE ControlTask::EcatGetMasterStateSysCall()
{
	INT32 iState = syscall(0, 0, 0, 0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,2));
	return (EC_MASTER_STATE)iState;
}

/**************************************************************************//**
* \brief   - Process commands(WELD,SEEK,SETUP, SCAN or ALARM) received from MsgQ.
*
* \param   - None.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::ProcessCommandMessage()
{
	memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
	EC_MASTER_STATE EcMasterState = EcatGetMasterStateSysCall();

	/* check the state of the Ec-Master before reading the MsgQ to send the PDO data*/
	if(ecat_OP == EcMasterState)
	{
		if(msgQReceive(CTRL_MSG_Q_ID, RecvMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, NO_WAIT) != ERROR)
		{
			#ifdef DEBUG
				LOGDBG("CTRL_T : ------------Message has received------------",0,0,0);
			#endif

			DeCoder(RecvMsgBuffer,message);
			
			SCSTATES state = SC::stateMachine.GetStateType ();
			
			switch(message.msgID)
			{
				case TO_CTRL_TASK_WELD_CMD:
					LOGDBG("CTRL_T : -------Weld command received-------",0,0,0);
					OpType 		= WELD;				/* To identify what operation (weld) selected chosen in consoleApp or UIC */
					PrevOpType 	= WELD;
					break;
	
				case TO_CTRL_TASK_SEEK_CMD:
					LOGDBG("CTRL_T : -------Seek command received-------",0,0,0);
					if(SC_READY == state || PREREADY == state)
					{
						OpType 		= SEEK;			/* To identify what operation (seek) selected chosen in consoleApp or UIC */
						PrevOpType 	= SEEK;
					}
					else
					{
						message.msgID = TO_UI_TASK_SEEK_RES;
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					break;
	
				case TO_CTRL_TASK_SETUP_CMD:
					LOGDBG("CTRL_T : -------Setup command received------- : %d",m_Setup,0,0);
					if(SC_READY == state || PREREADY == state)
					{ 
						OpType 		= (OPERATION)m_Setup;					/* To identify what operation (setup) selected chosen in consoleApp or UIC */
						PrevOpType 	= (OPERATION)m_Setup;
					}
					else if((HORN_DOWN == state) && (HORN_RETRACT == m_Setup))
					{
						OpType 		= (OPERATION)m_Setup;					/* To identify what operation (setup) selected chosen in consoleApp or UIC */
						PrevOpType 	= (OPERATION)m_Setup;
					}
					else
					{ 
						UpdateActuatorSetup(m_Setup,RESPONSE_FAIL);
					}
					
					if(HORN_RETRACT != m_Setup)
					{
						memset(message.Buffer,0x00,sizeof(message.Buffer));
						message.Buffer[0] = m_Setup;
						DecideNextOperationType(message.Buffer,state);
					}
					break;

				case TO_CTRL_TASK_SET_NEXT_OPERATION_REQ:	
					LOGDBG("CTRL_T : -------Next operation request received------- ",0,0,0);					
					DecideNextOperationType(message.Buffer,state);
					break;
					
				case TO_CTRL_TASK_RESET_ALARM_CMD:
					LOGDBG("CTRL_T : -------RESET ALARM CMD received -------",0,0,0);
					if(SC_ALARM == state || HORN_CLAMP_ALARM == state)
					{
						OpType = RESET_ALARM;			/* To identify what operation (alarm reset) selected chosen in consoleApp or UIC */
						bIsAlarmReset 	= false;
						bIsAlarmData 	= true;			/* will cause a response to the UIC */
					}
					else
					{
						message.msgID = TO_UI_TASK_ALARM_RESET_RESP;
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					break;
	
				case TO_CTRL_TASK_HORN_SCAN_CMD:
					LOGDBG("CTRL_T : -------Scan command received-------",0,0,0);
					if(SC_READY == state || PREREADY == state)
					{
						OpType 		= SCAN;		/* To identify what operation (scan) selected chosen in consoleApp or UIC */
						PrevOpType 	= SCAN;
					}
					else
					{
						message.msgID = TO_UI_TASK_HORN_SCAN_RES;
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					break;
	
				case TO_CTRL_TASK_HORN_SCAN_ABORT_CMD:
					LOGDBG("CTRL_T : -------Scan abort command received-------",0,0,0);
					if(SCAN_SONIC_ON == state)
					{
						OpType = ABORT;				/* To identify what operation (scan) selected chosen in consoleApp or UIC */ 
					}
					else
					{
						message.msgID = TO_UI_TASK_SCAN_ABORT_CMD_RESP;
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					break;
	
				case TO_CTRL_TASK_SEEK_RECIPE_CONFIGURED:
					LOGDBG("CTRL_T : -------Seek Recipe Configured-------",0,0,0);
#ifdef POWERUP_SEEK_DISABLE
					OpType 		= WELD;						
					PrevOpType 	= WELD;
#else
					OpType 		= SEEK;						/* To identify what operation (seek) selected chosen in consoleApp or UIC */
					PrevOpType 	= SEEK;
#endif
					break;
					
				case TO_CTRL_TASK_SC_STATE_REQ:
					LOGDBG("CTRL_T : -------SC state request-------",0,0,0);
					sprintf(message.Buffer,"%d",AC_RX->SCState);
					message.msgID = TO_UI_TASK_SC_STATE_RES;
					SendToMsgQ(message,UI_MSG_Q_ID);
					break;
					
				case TO_CTRL_TASK_TEST_CMD_REQ:					
					LOGDBG("CTRL_T : -------Test command received-------",0,0,0);
					if(SC_READY == state || PREREADY == state)
					{						
						OpType 		= TEST;			/* To identify what operation (seek) selected chosen in consoleApp or UIC */
						PrevOpType 	= TEST;
					}
					else
					{						
						message.msgID = TO_UI_TASK_TEST_CMD_RES;
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					break;

				case TO_CTRL_TASK_TEST_ABORT_CMD_REQ:					
					LOGDBG("CTRL_T : -------Test abort command received-------",0,0,0);
					if(TEST_SONICS_ON == state)
					{
						OpType = ABORT_TEST;				/* To identify what operation (scan) selected chosen in consoleApp or UIC */ 
					}
					else
					{
						message.msgID = TO_UI_TASK_TEST_ABORT_CMD_RES;
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					break;
	
				case TO_CTRL_TASK_CALIBRATION_CMD_REQ:
					LOGDBG("CTRL_T : -------Calibration command received-------",0,0,0);
					if((SC_READY == state) || (CALIBRATION_FIRST_POINT == state) || (CALIBRATION_SECOND_POINT == state) || (CALIBRATION_FINISH == calibrationStep) || (CALIBRATION_ABORT == calibrationStep)) 
					{						
						OpType 		= CALIBRATION;		/* To identify what operation (Calibration) selected chosen in consoleApp or UIC */
						PrevOpType 	= CALIBRATION;
						message.msgID = TO_UI_TASK_CALIBRATION_CMD_RES;
						sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					else
					{						
						message.msgID = TO_UI_TASK_CALIBRATION_CMD_RES;
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);
						SendToMsgQ(message,UI_MSG_Q_ID);
					}
					break;
					
					
				case UI_TO_CTRL_TASK_USERIO_READ_REQ:

					// Initialize the I/O manager here
					memcpy (&message, RecvMsgBuffer, sizeof (message));
					
					if (digitalIOMgr.ReadConfig (message, _digitalInputs, _digitalOutputs))
					{
						digitalIOMgr.SetInputs (_digitalInputs);
						digitalIOMgr.SetOutputs (_digitalOutputs);
						digitalIOMgr.RefreshOutputs();

						sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
					}
					else
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);

					message.msgID = TO_UI_TASK_USERIO_READ_RES;
					SendToMsgQ(message,UI_MSG_Q_ID);
					break;
					
				case WS_TO_CTRL_TASK_USERIO_READ_REQ:
					//This code is a duplication of UI_TO_CTRL_TASK_USERIO_READ_REQ it was made to avoid response collisions between UI and WS
					
					//Initialize the I/O manager from Web Services login
					memcpy (&message, RecvMsgBuffer, sizeof (message));
					
					if (digitalIOMgr.ReadConfig (message, _digitalInputs, _digitalOutputs))
					{
						digitalIOMgr.SetInputs (_digitalInputs);
						digitalIOMgr.SetOutputs (_digitalOutputs);
						digitalIOMgr.RefreshOutputs();

						sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
					}
					else
						sprintf(message.Buffer,"%d",RESPONSE_FAIL);

					message.msgID = TO_WEB_SERVICES_TASK_USERIO_READ_RES;
					SendToMsgQ(message,WEBSERVICE_MSG_Q_ID);
					break;
					
				case TO_CTRL_TASK_EMMC_WEAR_LEVEL_REQ:
					
					LOGDBG("\nCTRL_T : TO_CTRL_TASK_EMMC_WEAR_LEVEL_REQ\n",0,0,0);
					
					sprintf(message.Buffer,"%d", GetEmmcWearLevel());
					message.msgID = TO_UI_TASK_EMMC_WEAR_LEVEL_RESP;
					SendToMsgQ(message,UI_MSG_Q_ID);
					break;
					
				case TO_CTRL_TASK_UL_WELD_SIGNATURE:
					CP->ind_ULGraphData = true;
					break;
					
				case TO_CTRL_TASK_OPTIMIZE_DATABASE_REQ:
					CP->req_OptimizeDB = true;
					break;

				case TO_CTRL_TASK_BATCH_COUNT_EXCEED_CMD:
					LOGDBG("CTRL_T : -------BATCHCOUNT Exceed command received-------",0,0,0);
					ResetLimitExceedAlarm(true);
					OpType 		= WELD;			
					PrevOpType 	= WELD;
					break;
				
				case TO_CTRL_TASK_BATCH_COUNT_RESET_CMD_REQ:
					LOGDBG("CTRL_T : -------BATCHCOUNT Reset command received-------",0,0,0);
					OpType 		= WELD;			
					PrevOpType 	= WELD;
					ResetLimitExceedAlarm(false);
					break;
					
				default:
					LOGERR("CTRL : ----------Unknown Message ID------------- : %d",message.msgID,0,0);
					break;
			}
		}
	}
	else if((FirmwareUpgrade::bIsFirmwareUpgrade == true) && (ecat_INIT == EcMasterState))
	{
		taskClose(taskIdSelf());
	}
	else if(POWER_UP != SC::stateMachine.GetStateType())
	{
		AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_INTERNAL_COMM_ECAT_EFA);
	}
}

/**************************************************************************//**
* \brief   - Process State machine and Message received from MsgQ. 
*
* \param   - None.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::StateMachineHandler()
{	
	static UINT16 sem_selector = 0;
	static UINT16 loop_selector = 0;
	
	/* Event getting generated from Aux-Clock timer function on every 250us */
	if(eventReceive(SHUTDOWN_EVENT | CTRL_250us,EVENTS_WAIT_ANY,WAIT_FOREVER, &events) != ERROR)
	{
		if(events & SHUTDOWN_EVENT)
		{
			startTime = TimeStamp();
		
			if((WAIT_FOR_READY_POSITION == SC::stateMachine.GetStateType()) || (CP->ShutdownCriticalDataFlag))
			{
				if((WELD == ControlTask::GetOperationType()) ||
				((SC_ALARM == SC::stateMachine.GetStateType()) && (WELD == ControlTask::GetPrevOperationType())))
				{
					/* copy Part ID to weld result structure */ 
					StoreWeldResultToEEPROM();
					CP->ShutdownCriticalDataFlag = 0;  // Used for Shutdown event Weld result saving logic
				}
			}

			if(ERROR == eventSend(MAIN_TID,SHUTDOWN_EVENT))			/* Exit rtpMain */
			{
				LOGERR("CTRL_T : Event send failed on application exit",0,0,0);
			}
			
			message.msgID = SHUTDOWN_APPLICATION;			/* Exit all business tasks */
			SendToMsgQ(message,DB_MSG_Q_ID);
			SendToMsgQ(message,UI_DB_MSG_Q_ID);
			SendToMsgQ(message,UI_MSG_Q_ID);
			SendToMsgQ(message,SDO_MSG_Q_ID);
			
			endTime = TimeStamp();
			printf("CTRL_T Shutdown triggered. Total Time Elapsed in Shutdown Event: %f\n",(endTime-startTime));

		}			
		else if(events & CTRL_250us)
		{
			// timing digital IO inputs and outputs
			loop_selector++;

			if (loop_selector % 2)
			{
				if (sem_selector % 2)
					McSPI::ScDgtInOutSemGive (DGTOUT_T);
				else
					McSPI::ScDgtInOutSemGive (DGTIN_T);

				sem_selector++;
			}
			
			//Trigger IOImage Handler Task every 1 millisecond.
			if (loop_selector % 4 == 0)
			{
				//Send event only when Fieldbus is initialized successfully and the task is running.
				if ((FieldBus::GetFieldBusInitialised() == true) && (!taskIdVerify(CP->GetTaskId(cTaskName[IO_IMAGE_HANDLER_T]))))
				{	
					if(eventSend(CP->GetTaskId(cTaskName[IO_IMAGE_HANDLER_T]), IO_IMAGE_HANDLE_EVENT) != OK)
					{
						LOGERR("CTRL_T :Event send to IOImage_Handler_Task failed",0,0,0);
					}		
				}
			}
			//Trigger Mailbox Handler Task every 20 millisecond.
			if (loop_selector % 80 == 0)
			{
				//Send event only when Fieldbus is initialized successfully and the task is running.
				if ((FieldBus::GetFieldBusInitialised() == true) && (!taskIdVerify(CP->GetTaskId(cTaskName[MAIL_BOX_HANDLER_T]))))
				{
					//<! For testing purpose the MailBox task has been masked.
					if(eventSend(CP->GetTaskId(cTaskName[MAIL_BOX_HANDLER_T]), MAILBOX_HANDLE_EVENT) != OK)
					{
						LOGERR("CTRL_T :Event send to Mailbox_Handler_Task failed",0,0,0);
					}					
				}
			}
			
			switch(OpType)
			{
				case WELD:
				if(!ControlTask::bIsResultConfig)
				{
					SC::stateMachine.RunStateMachine();
				}
				else
				{	
					/* Status of weld will be updated */
					UpdateWeldStatus();
					if(CP->ind_CollectGraphData == true)
					{
						SendDataToDataBaseQueue(WELD_SIGNATURE);
					}
					SendDataToDataBaseQueue(WELD_RESULT);
					
					if(TRUE == (ControlTask::GetPostWeldDoneFlag()))
					{
						OpType = SEEK;
					}
					else
					{
						SwitchToNextOp();
					}
					
					CheckNewActiveRecipeAvailability();
					ControlTask::SetResultData(false);
				}
				break;
	
				case SEEK:
					if(!ControlTask::bIsResultConfig)
					{
						SC::stateMachine.RunStateMachine();
					}
					else
					{	
						SendDataToDataBaseQueue(SEEK_RESULT);
						if((TRUE == (ControlTask::GetWeldOpFlag())) && (FALSE == (ControlTask::GetPostWeldDoneFlag())))
						{
							OpType = WELD;
						}
						else
						{
							SwitchToNextOp();
						}
						ControlTask::SetResultData(false);
					}
					break;
	
				case FIND_PART_CONTACT_OP:
				case JOG_UP_OP:
				case JOG_DOWN_OP:
				case JOG_READY_POSITION:
				case HORN_DOWN_CLAMP_ON:
				case HORN_DOWN_CLAMP_OFF:
				case HORN_RETRACT:
					if(!ControlTask::bIsResultConfig)
					{
						SC::stateMachine.RunStateMachine();
						if(ControlTask::m_bIsIndicateUIC)
						{
							ControlTask::m_bIsIndicateUIC = false;
							UpdateActuatorSetup((UINT8)OpType,RESPONSE_SUCCESS);
						}
					}
					else
					{	
						ControlTask::SetResultData(false);					
						UpdateActuatorSetup((UINT8)OpType,RESPONSE_SUCCESS);
						message.msgID = TO_SDO_TASK_WELD_RECIPE_AC;
						SendToMsgQ(message,SDO_MSG_Q_ID);		// Updated ExpectedPartContact in WeldRecipe and SDO DOWNLOAD(Weld recipe to AC)
						SwitchToNextOp();
					}
					break;
								
				case CALIBRATION:
					if(!bIsResultConfig)
					{
						SC::stateMachine.RunStateMachine();
					}
					else
					{	
						SetResultData(false);
						SwitchToNextOp();
						calibrationStep = 0;
					}
					break;
		
				case ALARM:
				case RESET_ALL_ALARM:
				case RESET_ALARM:
					SC::stateMachine.RunStateMachine();
					if(ControlTask::bIsResultConfig)
					{
						ControlTask::SetResultData(false);
						CP->m_ScAlarmObj.SetAlarmFlag(true);
						
						PrevOperationHandler(PrevOpType);
						
						ControlTask::TriggerConsoleTask();
					}
					else if(bIsAlarmData)
					{
						bIsAlarmData = false;
						CP->m_ScAlarmObj.SetAlarmFlag(false);
						SwitchToNextOp();
						ConsoleApp::bIsScanStart = false;		/* To change user option from abort to normal mode */
					}
					break;
				
				case SCAN:
				case ABORT:
					if(!ControlTask::bIsResultConfig)
					{
						SC::stateMachine.RunStateMachine();
					}
					else
					{	
						if(ControlTask::OpType != ABORT)
						{
							message.msgID = TO_DB_TASK_SCAN_SIGN_DATA;
							SendToMsgQ(message,DB_MSG_Q_ID);
							message.msgID = TO_DB_TASK_HORN_SCAN_RESULT;
							SendToMsgQ(message,DB_MSG_Q_ID);
							message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
							SendToMsgQ(message,SDO_MSG_Q_ID);
						}
						else if(ControlTask::OpType == ABORT)
						{
							message.msgID = TO_UI_TASK_SCAN_ABORT_CMD_RESP;	/* Notify UI task for abort request */
							sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);	
							SendToMsgQ(message,UI_MSG_Q_ID);
							
						}
						
						ControlTask::SetResultData(false);
						ConsoleApp::bIsScanStart = false;		/* To change user option from abort to normal mode */
						SwitchToNextOp();		
					}
					break;
				
				case TEST:
				case ABORT_TEST:
					if(!ControlTask::bIsResultConfig)
					{
						SC::stateMachine.RunStateMachine();
					}
					else
					{					
						ControlTask::SetResultData(false);
						
						message.msgID = TO_DB_TASK_CTRL_TEST_SIGN_DATA;		 
						SendToMsgQ(message, DB_MSG_Q_ID);
						message.msgID = TO_DB_TASK_TEST_RESULT;
						SendToMsgQ(message, DB_MSG_Q_ID);
						SwitchToNextOp();
					}
					break;

				default:
					SC::stateMachine.RunStateMachine();
					break;
			}
		}
	}
	else
	{
				LOGERR("CTRL_T-------event receive error-------",0,0,0);
	}

}

/**************************************************************************//**
* \brief   - Alarm Handler for different types of alarms 
*
* \param   - OPERATION - prevOpType.
*
* \return  - None
*
******************************************************************************/
void ControlTask::PrevOperationHandler(OPERATION prevOpType)
{
	switch(prevOpType)
	{
		case WELD:
			/* Weld signature and result data will be stored into DB only when (T)trigger point met */
			if(CommonProperty::weldResult.Get(WeldResults::TRIGGER_TIME) > 0)
			{
				/* Status of weld will be updated */
				UpdateWeldStatus();
				if(CP->ind_CollectGraphData == true)
				{
					SendDataToDataBaseQueue(WELD_SIGNATURE);	/* Signature must me called first */
				}
				SendDataToDataBaseQueue(WELD_RESULT);
			}
			else
			{
				message.msgID = TO_UI_TASK_UL_WELD_RESULT;
				sprintf(message.Buffer,"%d",RESPONSE_FAIL);
				SendToMsgQ(message,UI_MSG_Q_ID);
			}
			CheckNewActiveRecipeAvailability();
			break;
	
		case SEEK:
			message.msgID = TO_UI_TASK_SEEK_RES;
			sprintf(message.Buffer,"%d",RESPONSE_FAIL);
			SendToMsgQ(message,UI_MSG_Q_ID);
			break;
	
		case SCAN:
			message.msgID = TO_UI_TASK_HORN_SCAN_RES;
			sprintf(message.Buffer,"%d",RESPONSE_FAIL);
			SendToMsgQ(message,UI_MSG_Q_ID);
			break;
	
		case FIND_PART_CONTACT_OP:
		case HORN_DOWN_CLAMP_ON:
		case HORN_DOWN_CLAMP_OFF:
			UpdateActuatorSetup((UINT8)prevOpType,RESPONSE_FAIL);
			break;
			
		case AC_TO_HOME:
		case AC_TO_READY:
			break;
		default:
			break;
	}
}

/**************************************************************************//**
* \brief   - Returns the current operation (Weld or Seek) type 
*
* \param   - None.
* 
* \return  - OPERATION - OpType - Type Of Operation received.
* 			(weld, seek, setup or scan).
*
******************************************************************************/
OPERATION ControlTask::GetOperationType()
{
	return OpType;	
}

/**************************************************************************//**
* \brief   - Returns the previous operation(Weld or Seek) performed on or before
*  			 alarm. 
*
* \param   - None.
* 
* \return  - OPERATION - PrevOpType - Type Of Operation performed before alarm.
* 			(weld, seek, setup or scan).
*
******************************************************************************/
OPERATION ControlTask::GetPrevOperationType()
{
	return PrevOpType;	
}


/**************************************************************************//**
* \brief   - Returns the next operation to be performed 
*
* \param   - None
* 
* \return  - OPERATION - nextOpType
*
******************************************************************************/
OPERATION ControlTask::GetNextOperationType()
{
	return nextOpType;
}

/**************************************************************************//**
* \brief   - Get the selected PostWeldDoneFlag from consoleApp. 
*
* \param   - None.
* 
* \return  - PostWeldDoneFlag
*
******************************************************************************/
UINT8 ControlTask::GetPostWeldDoneFlag()
{
	return PostWeldDoneFlag;	
}

/**************************************************************************//**
* \brief   - Get the selected PreWeldDoneFlag from consoleApp. 
*
* \param   - None.
* 
* \return  - PreWeldDoneFlag
*
******************************************************************************/
UINT8 ControlTask::GetPreWeldDoneFlag()
{
	return PreWeldDoneFlag;	
}

/**************************************************************************//**
* \brief   - Get the selected WeldOpFlag from consoleApp. 
*
* \param   - None.
* 
* \return  - WeldOpFlag
*
******************************************************************************/
UINT8 ControlTask::GetWeldOpFlag()
{
	return WeldOpFlag;	
}

/**************************************************************************//**
* \brief   - Sets the selected operation(Weld or Seek) from UIC.  
*
* \param   - OPERATION - typeOfOp - weld, seek. setup, alarm or horn scan. 
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::SetOperationType(OPERATION typeOfOp)
{
	OpType = typeOfOp;
}

/**************************************************************************//**
* \brief   - Set the PostWeldDoneFlag from consoleApp.  
*
* \param   - UINT8 - TempPostWeldDoneFlag - TRUE/FALSE. 
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::SetPostWeldDoneFlag(UINT8 TempPostWeldDoneFlag)
{
	PostWeldDoneFlag = TempPostWeldDoneFlag;
}

/**************************************************************************//**
* \brief   - Set the PreWeldDoneFlag from consoleApp.  
*
* \param   - UINT8 - TempPreWeldDoneFlag - TRUE/FALSE. 
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::SetPreWeldDoneFlag(UINT8 TempPreWeldDoneFlag)
{
	PreWeldDoneFlag = TempPreWeldDoneFlag;
}

/**************************************************************************//**
* \brief   - Set the WeldOpFlag from consoleApp.  
*
* \param   - UINT8 - WeldOpFlag - TRUE/FALSE.  
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::SetWeldOpFlag(UINT8 TempWeldOpFlag)
{
	WeldOpFlag = TempWeldOpFlag;
}

/**************************************************************************//**
* \brief  - Gets the counter of calibration steps
*
* \param  - none
*
*
* \return  - calibrationStep
*
******************************************************************************/
UINT8 ControlTask::GetCalibrationStep()
{
	return calibrationStep;	
}

/**************************************************************************//**
* \brief  - Sets the calibration step
*
* \param  - step
*
*
* \return  - calibrationStep
*
******************************************************************************/
void ControlTask::SetCalibrationStep(UINT8 step)
{
	calibrationStep = step;	
}

/**************************************************************************//**
* \brief   - Set the result flag as true to collect results.
* 			 (weld, seek, setup or scan). 
*
* \param   - bool - true or false.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::SetResultData(bool bVal)
{
	bIsResultConfig = bVal;
}

/**************************************************************************//**
* \brief   - Returns status about limit validation alarm status such as 
* 			 Batch counter exceed, storage space of DB,etc..
* 
* \param   - None
*
* \return  - bool - TRUE or FALSE
*
******************************************************************************/
bool ControlTask::GetLimitExceedAlarm()
{
	return m_bIsLimitExceed;
}

/**************************************************************************//**
* \brief   - Resets batch count exceeded alarm
* 
* \param   - bool - TRUE or FALSE
*
* \return  - None
*
******************************************************************************/
void ControlTask::ResetLimitExceedAlarm(bool bIsVal)
{
	m_bIsLimitExceed = bIsVal;
}

/**************************************************************************//**
* \brief   - Updates actuator setup response with output data 
*
* \param   - UINT8 respType, UINT8 respMsg
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::UpdateActuatorSetup(UINT8 respType, UINT8 respMsg)
{
	message.msgID = TO_UI_TASK_SETUP_RESP;
	ControlTask::m_SetupData.ResponseType 	= respType;	
	ControlTask::m_SetupData.ResponseStatus = respMsg;	
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	memcpy(message.Buffer,&m_SetupData,sizeof(m_SetupData));
	SendToMsgQ(message,UI_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Sets actuator setup data  
*
* \param   - ACTUATOR_SETUP typeofData, UINT16 data.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::SetActuatorSetupData(ACTUATOR_SETUP typeofData, UINT32 data)
{
	switch(typeofData)
	{
		case ABS_POSITION:
			m_SetupData.AbsolutePosition 		= data;
			break;
	
		case PRE_PART_CONTACT:
			m_SetupData.PrePartContact 			= data;
			break;
	
		case VELOCITY:
			m_SetupData.PrePartContactVelocity 	= data;
			break;
	
		case EXP_PART_CONTACT:			
			m_SetupData.ExpectedPartContact 	= data;			
			break;
	
		case ACTUAL_FORCE:
			m_SetupData.ActualForce 			= data;
			break;
	
		case ACTUAL_DISTANCE:
			m_SetupData.ActualDistance 			= data;
			break;
	
		default:
			break;
	}
}

/**************************************************************************//**
* \brief   - To interrupt the ConsoleApp for user menu options. 
*
* \param   - None.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::TriggerConsoleTask()
{
	if(eventSend(Console_TID,CON_EVENT) != OK)
	{
		LOGERR("CTRL_T : ----------Event send to consoleApp_Task failed--------",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Sends the result(weld or seek) to DBQ. 
* 
* \param   - UINT8	- typeOfResult
*
* \return  - None
*
******************************************************************************/
void ControlTask::SendDataToDataBaseQueue(UINT8 typeOfResult)
{
	if(WELD_RESULT == typeOfResult)
	{
		WeldResult 	results;
		memset(message.Buffer,0x00,sizeof(message.Buffer));
		
		results = CommonProperty::weldResult.GetWeldData();
		message.msgID = TO_DB_TASK_UL_WELD_RESULT;
		memcpy(message.Buffer, &results, sizeof(results));
		SendToMsgQ(message,DB_MSG_Q_ID);
		if (CP->stackRecipe.GetData(StackRecipe::END_OF_WELD_STORE)==1)
		{
			message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
			SendToMsgQ(message,SDO_MSG_Q_ID);		/* Updated StartFrequency in WeldRecipe and SDO DOWNLOAD(Weld recipe to PC) */
		}
	}
	else if(WELD_SIGNATURE == typeOfResult)
	{
		UINT32 totalRecord 	= CP->m_TimeCounter;
		UINT32 recipeNum 	= CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM); 
		UINT32 recipeRevNum	= CommonProperty::weldResult.Get(WeldResults::RECIPE_REV_NUM);
		UINT32 cycleCounter	= CommonProperty::weldResult.Get(WeldResults::CYCLE_COUNTER);
		UINT8 index		   	= 0;
		memset(message.Buffer,0x00,sizeof(message.Buffer));
		
		message.msgID = TO_DB_TASK_WELD_SIGN_DATA;
		memcpy(message.Buffer, &totalRecord, sizeof(totalRecord));
		index = index + sizeof(totalRecord);
		memcpy(message.Buffer + index, &recipeNum, sizeof(recipeNum));
		index = index + sizeof(recipeNum);
		memcpy(message.Buffer + index, &recipeRevNum, sizeof(recipeRevNum));
		index = index + sizeof(recipeRevNum);
		memcpy(message.Buffer + index, &cycleCounter, sizeof(cycleCounter));
		SendToMsgQ(message,DB_MSG_Q_ID);
	}
	else if(SEEK_RESULT == typeOfResult)
	{
		message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
		SendToMsgQ(message,SDO_MSG_Q_ID);		/* Updated StartFrequency in WeldRecipe and SDO DOWNLOAD(Weld recipe to PC) */
		message.msgID = TO_DB_TASK_UL_SEEK_RESULT;
		SendToMsgQ(message,DB_MSG_Q_ID);
	}
}

/**************************************************************************//**
* \brief   - Checks new active recipe received from UIC, If received then
* 			 Update received as active recipe for next weld.
* 
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ControlTask::CheckNewActiveRecipeAvailability()
{
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	message.msgID = TO_UI_TASK_CHECK_NEW_ACTIVE_RECIPE;
	SendToMsgQ(message,UI_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Assigns weld cycle counter with latest with respective recipe.
* 
* \param   - const UINT32 cycleCounter
*
* \return  - None
*
******************************************************************************/
void ControlTask::SetCycleCounter(const UINT32 cycleCounter)
{
	m_CycleCounter = cycleCounter;
}

/**************************************************************************//**
* \brief   - Updates cycle counter when weld cycle met trigger point.
* 
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ControlTask::UpdateCycleCounter()
{
	m_CycleCounter = m_CycleCounter + 1;
	CommonProperty::weldResult.Set(WeldResults::CYCLE_COUNTER, m_CycleCounter);
}

/**************************************************************************//**
* \brief   - Get the task life time status.
* 
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool ControlTask::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}

/**************************************************************************//**
* \brief   - Prepares next operation after finishing current operation
* 			 Repeats actuator setup operations or switches to weld for all others
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ControlTask::SwitchToNextOp()
{
	/* When m_bIsWeldAllowed and m_bIsReleased flag is true then next operation type will be WELD */
	if(m_bIsWeldAllowed)
	{
		OpType 		= WELD;
		PrevOpType 	= WELD;
	}
	/* When UIC not logged-in or user moved to (actuator or IO or report generation) screen then weld will be blocked and requested operation
	 * UIC will be selected or NO_OPERATION operation will be applied for next 
	 */
	else if(!m_bIsWeldAllowed)	
	{
		OpType 		= nextOpType;
		PrevOpType 	= nextOpType;
	}
}

/**************************************************************************//**
* \brief   - Identifies received operation request from UIC and decides on
* 			 Which operation has to perform next. 
*
* \param   - char *pRecv_Msg, SCSTATES state
*
* \return  - None
*
******************************************************************************/
void ControlTask::DecideNextOperationType(char *pRecv_Msg, SCSTATES state)
{
	UINT8 opType = 254;
	
	memcpy(&opType, pRecv_Msg, sizeof(opType));
	
	if((PREREADY == state) || (READY == state))
	{
		OpType		= (OPERATION)opType; 	/* When system is in pre_ready or ready state, Requested operation will be allowed immediately */ 
		PrevOpType	= (OPERATION)opType; 				
	}
	
	/* Next operation type will be set and will be used post completion of current operation */
	switch(opType)
	{
		case NO_OPERATION:
			LOGDBG("Request from UIC or UIC closed hence blocking the weld operation",0,0,0);
			m_bIsWeldAllowed = false;
			nextOpType 		 = NO_OPERATION;
			
			/* When UIC closed or logout from actuator screen then retract the horn to home position */
			if(((HORN_DOWN_CLAMP_ON == PrevOpType) || (HORN_DOWN_CLAMP_OFF == PrevOpType)) && (SC_ALARM != OpType)) 
			{	
				OpType 		= HORN_RETRACT;
				PrevOpType	= HORN_RETRACT;
			}
			break;
	
		case WELD:
			LOGDBG("Request from UIC for switching to weld operation",0,0,0);
			m_bIsWeldAllowed = true;
			nextOpType 		 = WELD;
			break;
	
		case FIND_PART_CONTACT_OP:
		case JOG_UP_OP:
		case JOG_DOWN_OP:
		case JOG_READY_POSITION:
		case HORN_DOWN_CLAMP_ON:
		case HORN_DOWN_CLAMP_OFF:
			LOGDBG("Request from UIC for switching to %d operation",opType,0,0);
			m_bIsWeldAllowed = false;
			nextOpType 		 = (OPERATION)opType;
			break;
	
		case HORN_RETRACT:
			LOGDBG("Request from UIC for abort actuator setup operation and switch to weld operation",0,0,0);
			m_bIsWeldAllowed = true;
			if((SC_ALARM != state) && ((PREREADY != state) || (READY != state)))
			{	
				/* Horn retracts to home position and next operation will be selected as weld when user goes out of actuator screen */
				OpType		= HORN_RETRACT;  
				PrevOpType	= HORN_RETRACT;
			}
			nextOpType 		= WELD;
			break;
	
		default:
			/* When Invalid operation type received from UIC then will change it NO_OPERATION operation type */
			LOGERR("CTRL_T : Received operation from UIC invalid : %d",opType,0,0);
			m_bIsWeldAllowed = false;	
			nextOpType 		 = NO_OPERATION;
			if(SC_ALARM != state)
			{
				OpType		= NO_OPERATION; 
				PrevOpType	= NO_OPERATION; 
			}
			break;
	}
}

/**************************************************************************//**
* \brief   - Responsible to update the status of weld (GOOD, SUSPECT, REJECT).  
*
* \param   - None.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::UpdateWeldStatus()
{
	if(bRejectStatus)
	{
		/* The flag has been set in alarm::Enter and used in weld results to identify weld resulted in alarm */
		CommonProperty::weldResult.Set(WeldResults::IS_ALARM,true);
		CommonProperty::weldResult.Set(WeldResults::WELD_STATUS,REJECTED);
		bRejectStatus  = false;
		bSuspectStatus = false;
	}
	else if(bSuspectStatus)
	{
		/* The flag has been set in alarm::Enter and used in weld results to identify weld resulted in alarm */
		CommonProperty::weldResult.Set(WeldResults::IS_ALARM,true);
		CommonProperty::weldResult.Set(WeldResults::WELD_STATUS,SUSPECT);
		bSuspectStatus = false;
	}
	else
	{
		CommonProperty::weldResult.Set(WeldResults::WELD_STATUS,GOOD);
		UpdatePartsPerMinCounter();
	}
}

/**************************************************************************//**
* \brief   - Store Weld Results(Critical data) into EEPROM when shutdown happens 
*
* \param   - None.
* 
* \return  - STATUS - OK - FAILURE
*
******************************************************************************/
STATUS ControlTask::StoreWeldResultToEEPROM()
{
	INT8 status 			 = OK;
	ssize_t criticalDatasize = 0;
	string insertQuery 		 = "";
	WeldResult w;
	Eeprom epclass;
	ShutdownCriticalData criticalData = {0x00};

	/*Updating Critical Parameter Structure*/
	GetRTC(&criticalData.SysTime); //updating RTC Time
	
	criticalData.WeldResultFlag = CRITICAL_DATA_STATUS_FLAG;	
	memcpy(&criticalData.Results, &CP->ShutdownWeldResult, sizeof(criticalData.Results));
	
	criticalDatasize = sizeof(criticalData) + (strlen(criticalData.Results.PartID)+1) - (sizeof(criticalData.Results.PartID));
	
	printf("################################	Writing Critical Data into EEPROM	##############################\n");
	
	/*Writing Critical data into EEPROM at the offset of 0x400*/ /*BARCODE_DATA_SIZE*/
	status = epclass.Write((char *)&criticalData, criticalDatasize, (ssize_t)SHUTDOWN_EEPROM_OFFSET);
	
	if(ERROR==status)
	{
		LOGCRIT("CTRL_T : -------------Error in Writing Critical Data-----------",0,0,0);
	}
	
	return status;
}


UINT8 ControlTask::GetEmmcWearLevel ()
{
	return emmcWearLevel;
}


/**************************************************************************//**
* \brief   - Reads the wear level register from the eMMC. If the wear level
* 			 is high, this method will raise an alarm. 
*
* \param   - None
* 
* \return  - bool read
*
******************************************************************************/
bool ControlTask::CheckEmmcWearLevel ()
{
	bool read = true;
	EST_LIFE eMMCwear;
	
	// call custom system call here...
	read = EmmcExtCsdDecode(&eMMCwear);
	
	if (read)
	{
		emmcWearLevel =  _CHECKMIN(eMMCwear.LifeTimeEstB * 10,EMMC_WEAR_LEVEL_MAX);
		LOGDBG ("\nCTRL_T : CheckEmmcWearLevel: emmcWearLevel = %d\n",emmcWearLevel,0,0);
		
		if (emmcWearLevel >= EMMC_WEAR_LEVEL_ALARM)
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_INTERNAL_STORAGE_FAILURE_EFA);
	}
	
	return read;
}


/**************************************************************************//**
* \brief   - Returns the number of good welds count to UIC to keep track of
* 			 parts per minute status. Will be sent as part of heart beat. 
*
* \param   - None.
* 
* \return  - m_PartsPerMinCounter
*
******************************************************************************/
UINT32 ControlTask::GetPartsPerMinCounter()
{
	return m_PartsPerMinCounter;
}


/**************************************************************************//**
* \brief   - Returns whether or not welding is allowed based on various
*			 condition.
*
* \param   - None
* 
* \return  - bool m_bIsWeldAllowed
*
******************************************************************************/
bool ControlTask::IsWeldAllowed()
{
	return m_bIsWeldAllowed; 
}

/**************************************************************************//**
* \brief   - Sets previous operation  
*
* \param   - OPERATION typeOfOp 
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::SetPrevOperationType(OPERATION typeofOp)
{
	PrevOpType = typeofOp;
}

/**************************************************************************//**
* \brief   - Updates the counter for every good weld. 
*
* \param   - None.
* 
* \return  - None.
*
******************************************************************************/
void ControlTask::UpdatePartsPerMinCounter()
{
	m_PartsPerMinCounter = m_PartsPerMinCounter + 1;
}

/**************************************************************************//**
* \brief   - De-Initialize allocated stack memory of ControlTask.  
*
* \param   - None.
* 
* \return  - None.
*
******************************************************************************/
ControlTask::~ControlTask()
{
	CP = NULL;
}

/**************************************************************************//**
* \brief   - Controls state machine functionalities(TASK ROUTINE).  
*
* \param   - None.
* 
* \return  - None.
*
******************************************************************************/
void Control_Task(void)
{	
	bIsAlarmReset		= false;
	PC_RX->MasterState 	= 0x00;
	AC_RX->SCState 	 	= 0x00;
	ControlTask *ScCtrl = new(nothrow) ControlTask();

	if(NULL != ScCtrl)
	{
		/* Control Task loop and the bIsTaskRun flag enabled when task created */
		while(ScCtrl->bIsTaskRunStatus())
		{
			ScCtrl->ProcessCommandMessage();
			ScCtrl->StateMachineHandler();
		}

		delete ScCtrl;
	}
	else
	{
		LOGCRIT("CTRL_T : -------------Memory allocation failed-----------",0,0,0);
	}

	ScCtrl = NULL;
	LOGDBG("-------------ControlTask Terminated-----------",0,0,0);

}
