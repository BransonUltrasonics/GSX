/*
 * StartSwitch.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#include "StartSwitch.h"
#include  "SCStateMachine.h"
#include  "SC_Common.h"
#include  "SC.h"
#include  "ControlTask.h"
#include "IOManager.h"
#include "AlarmManager.h"
#include "CommonProperty.h"
#include "MainTask.h"

/**************************************************************************//**
* 
* \brief   - StartSwitch Class constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
StartSwitch::StartSwitch()
{
	stateType = START_SWITCH;
}

/**************************************************************************//**
* 
* \brief   - StartSwitch Class destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
StartSwitch::~StartSwitch()
{
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering the StartSwitch state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::Enter()
{
	Message message;
	
#ifdef DEBUG
	LOGDBG("enter Start SWITCH",0,0,0);
#endif
	Stagg_Time			= STAGGER_TIME_IN_US/ECAT_CYCLE_TIME_IN_US;
	Stagg_Time_Counter	= 0;
	NewMaxWeldForce		= 0;
	uiStartSwitchCounter= 0;
	m_PartContactPos	= CP->m_RecipeAC.GetPartContactPos();
	
	// at this point no alarms, request UIC to clear its alarms
	message.msgID = TO_ALARM_TASK_UI_CLEAR_ALL_REQ;
	msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);

}

/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us while waiting for 
* 			 both start switches signal
* 			 This function is using multiple returns to resolve digital inputs
* 			 lines status as quick and in line. The purpose is to resolve to a fault
* 			 thus showing the return statements rather than them being hidden
* 			 in a sub-routine call. This gives faster resolution and execution and simplifies
* 			 the flow of the code. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::Loop()
{	
	uiStartSwitchCounter 	= uiStartSwitchCounter+1;
	Stagg_Time_Counter 		= Stagg_Time_Counter+1;
	
	
	if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	
	if((WELD == (ControlTask::GetOperationType())) && (m_PartContactPos <= 0))
	{
		ProcessAlarm (ALARM_SC, ALARM_ABSOLUTE_DISTANCE_NCA);
	}
	else if(AC_TX->ACState == AC_ALARM)
	{
		ProcessAlarm (ALARM_AC);
	}
	else
	{
		if (Stagg_Time_Counter<Stagg_Time)
		{
			if(((AC_TX->ACInputs & SS1MASK) | (AC_TX->ACInputs & SS2MASK)) == BOTHSTARTSWITCHMASK)
			{
				// Note: this call keeps a status that can be checked later
				DigitalIOMgr::ReadInputs ();
	
				if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
				{
					// ignore INPUT_DISABLE process input chain here
					
					if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
					{
						ProcessAlarm (ALARM_SC, ALARM_CYCLE_ABORT_NCA);
						return;
					}
					
					if (DigitalIOMgr::ScanForInputFunction (DigitalIO::INPUT_PART_PRESENT) &&
									DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PART_PRESENT) != DigitalIO::INPUT_ASSERTED)
					{
						ProcessAlarm (ALARM_SC, ALARM_PART_PRESENT_LOST_NCA);
						return;
					}

					if (AC_TX->ACInputs & GRDDETMASK)
					{
						ProcessAlarm (ALARM_SC, ALARM_GND_DETECT_EFA);
						return;
					}
				}
						
				ProcessLoop (ControlTask::GetOperationType());
			 }
			else
			 {
				 /* BothStartSwitch is NOT true yet, continue loop in StartSwitch */
			 }
			
		}
		else if(Stagg_Time_Counter>=Stagg_Time)
		{
			ProcessAlarm (ALARM_SC, ALARM_START_SWITCH_TIME_EFA);
		}
	}
}


/**************************************************************************//**
* 
* \brief   - This method will be use to compartmentalize repeating loop code.
*
* \param   - cmdID - command to process.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::ProcessLoop (INT32 cmdID)
{
	switch (cmdID)
	{
	case FIND_PART_CONTACT_OP:
	 	ChangeState(WAIT_FOR_PART_CONTACT);
		break;

	case CALIBRATION:
		ChangeState(CALIBRATION_FIRST_POINT);
		break;

	case HORN_DOWN_CLAMP_ON:
	case HORN_DOWN_CLAMP_OFF:
		ChangeState(HORN_DOWN);
		break;
		
	case JOG_DOWN_OP:
		ChangeState(JOG_DOWN);
		break;
		
	case JOG_UP_OP:
		ChangeState(JOG_UP);
		break;
		
	case AC_GO_HOME:
		ChangeState(AC_TO_HOME);
		break;
	case AC_GO_READY:
		ChangeState(AC_TO_READY);
		break;

	default:
		if(TRUE == (CP->m_RecipeSC.IsPreWeldSeekEnabled()))
		{
			ControlTask::SetOperationType(SEEK);
  	 		ChangeState(SEEK_SONIC_ON);
		}
  	 	else
  	 	{
  	 		ChangeState(WAIT_FOR_TRIGGER);
  	 	}
  	 	break;
	}
}


/**************************************************************************//**
* 
* \brief   - This method will be executed when exiting the StartSwitch state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void StartSwitch::Exit()
{
#ifdef DEBUG
	LOGDBG("Exit Start SWITCH",0,0,0);
#endif
}



