/*
 * PreReady.cpp
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#include "Common.h"
#include "PreReady.h"
#include "SeekSonicOn.h"
#include "ControlTask.h"
#include "MainTask.h"
#include "AlarmManager.h"
#include "ReportGeneration.h"
#include "UIDBInterface.h"

static SystemConfiguration sysConf;


/**************************************************************************//**
* \brief   - PreReady Class constructor
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

PreReady::PreReady()
{
	stateType = PREREADY;
}

/**************************************************************************//**
* \brief   - PreReady Class destructor
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

PreReady::~PreReady()
{

}

/**************************************************************************//**
* \brief   - This method will be executed when entering the PreReady state
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

void PreReady::Enter()
{
	bOneTimeSend = false;
	pbActiveTimeout = PB_ACTIVE_TIME_OUT_IN_US/ECAT_CYCLE_TIME_IN_US;
	partPresentActiveTimeout = PP_ACTIVE_TIME_OUT_IN_US/ECAT_CYCLE_TIME_IN_US;

#ifdef DEBUG
	 printf ("\nEnter PreReady\n");
#endif
		
	sysConf.SysConfigCache();

	if (AlarmMgr::IsAlarmState())
	{
		ChangeState(SC_ALARM);
	}
	

	ResetCycleTimeCounter();
}

/**************************************************************************//**
* \brief   - This method will be executed every 250us while waiting for the
*                       both PC and AC in the ready state.
*                       This function is using multiple returns to resolve digital inputs
*                       lines status as quick and in line. The purpose is to resolve to a fault
*                       thus showing the return statements rather than them being hidden
*                       in a sub-routine call. This gives faster resolution and execution and simplifies
*                       the flow of the code.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/

void PreReady::Loop()
{	
	PsPowerUpOptions   		PsPowerUpOptionIsSeek;
    static bool             PsSeekDone = false;
    
	if((AC_TX->ACInputs & SS1MASK) || (AC_TX->ACInputs & SS2MASK))
	{
		pbActiveTimeout = pbActiveTimeout-1;
		
		if (pbActiveTimeout == 0)
			ProcessAlarm (ALARM_SC, ALARM_START_SWITCH_CLOSED_EFA);
	}
	else if (DigitalIOMgr::ScanForInputFunction (DigitalIO::INPUT_PART_PRESENT) &&
			DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PART_PRESENT) == DigitalIO::INPUT_ASSERTED)
	{
		partPresentActiveTimeout = partPresentActiveTimeout - 1;
		if (partPresentActiveTimeout == 0)
			ProcessAlarm (ALARM_SC, ALARM_PART_PRESENT_ACTIVE_EFA);
		
		DigitalIOMgr::ReadInputs ();
	}
	else
	{
		if(AC_TX->ACState == AC_ESTOP)
		{
			ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
		}
		else if(AC_TX->ACState == AC_ALARM)
		{
			ProcessAlarm (ALARM_AC);
		}
		else if(PC_TX->PCState == PC_ALARM)
		{
			ProcessAlarm (ALARM_PC);
		}
		else if((AC_TX->ACState == AC_READY) && (PC_TX->PCState == PC_READY))
		{
			PsPowerUpOptionIsSeek = sysConf.SysConfigGetCache().PsPowerUpOptionIs; 

			if((PsPowerUpOptionIsSeek == PS_SEEK) && (PsSeekDone == false))
			{
				ControlTask::SetOperationType(SEEK);
//				ChangeState(SEEK_SONIC_ON);
				//seek_done   //only powerup, not for each weld
				PsSeekDone = true;
			}
			else
			{
				// Note: this call keeps a status that can be checked later
				DigitalIOMgr::ReadInputs ();
				
				if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
				{
					// ignore INPUT_DISABLE process input chain here
	
					if (AC_TX->ACInputs & GRDDETMASK)
					{
						ProcessAlarm (ALARM_SC, ALARM_GND_DETECT_EFA);
						return;
					}
	
					if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED
							|| DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_REJECT_CONFIRM) == DigitalIO::INPUT_ASSERTED
							|| DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_HORN_SEEK) == DigitalIO::INPUT_ASSERTED
							|| DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_AC_HOME) == DigitalIO::INPUT_ASSERTED
							|| DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_AC_READY) == DigitalIO::INPUT_ASSERTED
							|| ((DigitalIOMgr::ScanForInputFunction (DigitalIO::INPUT_PART_PRESENT) &&
									DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PART_PRESENT) != DigitalIO::INPUT_ASSERTED)
									&& (ControlTask::GetOperationType()  == WELD || ControlTask::GetOperationType()  == FIND_PART_CONTACT_OP)))
					{
						return;
					}
				}

				ProcessLoop ();
			}
		}
	}
}

/**************************************************************************//**
* \brief   - This method will be use to compartmentalize repeating loop code 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

void PreReady::ProcessLoop ()
{
	
#ifdef DEBUG
	LOGDBG("MasterState-PC	:	%u\n MasterState-AC	:	%u Amplitudechange:%u \n",PC_RX->MasterState,AC_RX->SCState,PC_RX->Amplitudechange);
#endif

	// reset output for cycle RUN 
	if (ControlTask::GetOperationType() == NO_OPERATION)
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_RUN, DigitalIO::LINE_LOW);
	else if (!ReportGeneration::GetReportGenerationFlag())
		return;
	else
	{
		if((PC_TX->PC_StatusEvent & BIT_MASK(STATUS_POWER_UP_RECIPE))==BIT_MASK(STATUS_POWER_UP_RECIPE))
		{
			PC_RX->MasterEvents &=~BIT_MASK(CTRL_POWER_UP_RECIPE);
		}
		if (!CP->ind_ULGraphData)
		{
			// PPM control loop
			return;
		}
		else if(PC_RX->MasterEvents &BIT_MASK(CTRL_POWER_UP_RECIPE))
		{ //wait for PC to process it
			return;
		}
		else if (CP->bIsWSLogAttempt)
		{//Waits until the Web Services Login or Logout routine finishes
			if(false == bOneTimeSend)
			{
				bOneTimeSend = true;
				memset(message.Buffer,0x00,sizeof(message.Buffer));
				message.msgID = TO_WEB_SERVICES_TASK_STATE_MACHINE_WAIT;
				msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[WEBSERVICE_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			}
			return;
		}
		else if (CP->req_OptimizeDB)
		{
			// optimize and send results
			CP->req_OptimizeDB = false;
			memset(message.Buffer,0x00,sizeof(message.Buffer));
			message.msgID = TO_UI_TASK_OPTIMIZE_DATABASE_RESP;
			snprintf (message.Buffer, sizeof (message.Buffer), DataBaseManager::OptimizeDB().c_str());
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[UI_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
		}
		else
		{
			ChangeState(SC_READY);
		}
	}
}


/**************************************************************************//**
* \brief   - This method will be executed when exiting the PreReady state 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/

void PreReady::Exit()
{
	CommonProperty::weldResult.ResetWeldResult();
	
#ifdef DEBUG
	LOGDBG("Exit PreReady",0,0,0);
#endif
}



