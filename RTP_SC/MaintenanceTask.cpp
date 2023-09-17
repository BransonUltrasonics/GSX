/*
  * MaintenanceTask.cpp
 *
 *  Created on: May 26, 2020
 *      Author: DShilonie
 *      
 *      The maintenance task will receive messages from tasks that need low priority operations i.e lifetime counters,
 *      lengthy memory access, long calculations, etc. Typical users are the control task, SDO task, alarm task and others.
 *      
 */

#include  "ControlTask.h"
#include  "MainTask.h"
#include "Counters.h"
#include "MaintenanceTask.h"

using namespace std;

/**************************************************************************//**
* \brief  - May acquires queue handles for proper intertask communications
* 			and initialize various elements.
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
MaintenanceTask::MaintenanceTask(): DB_MSG_Q_ID(0), CTRL_MSG_Q_ID(0), ALARM_MSG_Q_ID(0), SDO_MSG_Q_ID(0), UI_MSG_Q_ID(0) 
{
	CP						= CommonProperty::getInstance(); 	
	CTRL_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[CTRL_T]);
	DB_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[DB_T]);
	SDO_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[SDO_T]);
	ALARM_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[ALARM_T]);
	UI_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[UI_T]);
	MAINTENANCE_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[MAINTENANCE_T]);
	
	message.msgID = 0;
}


/**************************************************************************//**
* \brief  - Process commands received by the message queue from other tasks
* 			It is the entry point for communications with the this task
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void MaintenanceTask::ProcessCommandMessage ()
{
	memset(recvMsgBuffer,0x00,sizeof(recvMsgBuffer));
	EC_MASTER_STATE EcMasterState = EcatGetMasterStateSysCall();

	/* check the state of the Ec-Master before reading the MsgQ to send the PDO data*/
	if(ecat_OP == EcMasterState)
	{
		if(msgQReceive (MAINTENANCE_MSG_Q_ID, recvMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
		{
			memcpy (&message, recvMsgBuffer, sizeof (message));
			
			switch(message.msgID)
			{
			case TO_MAINTENANCE_TASK_CNTRS_INC_ACTUATIONS:

				Counters::incrementCounter(LIFETIME_ACTUATIONS);
				break;
			case TO_MAINTENANCE_TASK_CNTRS_INC_WELDS:

				Counters::incrementCounter(LIFETIME_WELDS);
				break;
			case TO_MAINTENANCE_TASK_CNTRS_INC_ALARMS:

				Counters::incrementCounter(LIFETIME_ALARMS);
				break;
			case TO_MAINTENANCE_TASK_CNTRS_INC_OVERLOADS:

				Counters::incrementCounter(LIFETIME_OVERLOADS);
				break;
				

			default:
				LOGDBG("\nMAINTENANCE_T : ------- Unknown message received",0,0,0);
				break;
			}
		}
	}
}

/**************************************************************************//**
* \brief  - Gets main task's run status
*
* \param  - None
*
* \return  - bool TRUE or FALSE
*
******************************************************************************/
bool MaintenanceTask::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}


/**************************************************************************//**
* \brief  - MaintenanceTask destructor
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
MaintenanceTask::~MaintenanceTask()
{
	CP = NULL;
}


/**************************************************************************//**
* \brief  - MaintenanceTask entry point and constructor. 
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void Maintenance_Task (void)
{	
	MaintenanceTask * pMaintenanceTask 	= new (nothrow) MaintenanceTask();

	if(NULL != pMaintenanceTask)
	{
		/* Maintenance Task loop if the main's bIsTaskRun flag is enabled */
		while (pMaintenanceTask->bIsTaskRunStatus())
		{
			pMaintenanceTask->ProcessCommandMessage();
		}

		delete pMaintenanceTask;
	}
	else
	{
		LOGERR("\nMAINTENANCE_T : ------------- Allocation failed",0,0,0);
	}

	LOGDBG ("MAINTENANCE_T : ------------- Task Terminated",0,0,0);

	taskDelete(taskIdSelf());
}


/**************************************************************************//**
* 
* \brief   - Returns the state of the EtherCat master stack.
*
* \param   - None.
*
* \return  - EC_MASTER_STATE
*
******************************************************************************/
EC_MASTER_STATE MaintenanceTask::EcatGetMasterStateSysCall()
{
	INT32 iState = syscall(0, 0, 0, 0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,2));
	return (EC_MASTER_STATE)iState;
}
