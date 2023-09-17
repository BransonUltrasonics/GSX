
#include "HornDown.h"
#include "SC_Common.h"
#include "MainTask.h"
#include "ControlTask.h"
#include "IOManager.h"
#include "AlarmManager.h"
#include "Counters.h"

/**************************************************************************//**
* 
* \brief   - HornDown Class constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
HornDown::HornDown()
{
	stateType = HORN_DOWN;
}

/**************************************************************************//**
* 
* \brief   - HornDown Class destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
HornDown::~HornDown()
{
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering the
* 			 HornDown state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HornDown::Enter()
{
#ifdef DEBUG
	LOGDBG("Enter HornDown",0,0,0);
#endif
	
	m_TimerCount = 0;
	onPart=false;
	Counters::incrementCounter(LIFETIME_ACTUATIONS);
}

/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us while waiting for the
* 			 HORN_RETRACT command from UIC to go back to homing position.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HornDown::Loop()
{
	if(AC_ALARM == AC_TX->ACState)
	{
		ProcessAlarm (ALARM_AC);
	}
	else
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
		}
		ProcessLoop ();
	}
}

/**************************************************************************//**
* \brief  - This method will be use to compartmentalize repeating loop code
*
* \param  - none
*
* \return  -    None 
*
******************************************************************************/
void HornDown::ProcessLoop ()
{
	if(0 == (++m_TimerCount) % INTERVAL_INDICATION)
	{
		ControlTask::SetActuatorSetupData(ACTUAL_FORCE,AC_TX->ActualForce);
		ControlTask::SetActuatorSetupData(ACTUAL_DISTANCE,AC_TX->ActualDistance);
		ControlTask::m_bIsIndicateUIC = true;								/* Sends actual distance to UIC */
	}
	if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	else if(HORN_RETRACT == ControlTask::GetOperationType() || 
		(onPart && (HORN_DOWN_CLAMP_OFF == ControlTask::GetOperationType()) && 
				(((AC_TX->ACInputs & SS1MASK) | (AC_TX->ACInputs & SS2MASK)) != BOTHSTARTSWITCHMASK)))
	{
		ChangeState(WAIT_FOR_READY_POSITION);
	}
	else if(!onPart)
	{
		if(AC_TX->ACStatusEvent & BIT_MASK(PART_CONTACT_FOUND))
		{
			onPart=true;
			ControlTask::TriggerConsoleTask();
		} else if(((AC_TX->ACInputs & SS1MASK) | (AC_TX->ACInputs & SS2MASK)) != BOTHSTARTSWITCHMASK)
		{
			ChangeState(WAIT_FOR_READY_POSITION);;
		}
	}
}


/**************************************************************************//**
* \brief  - This method will be executed when exiting the HornDown state
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void HornDown::Exit()
{
#ifdef DEBUG
	LOGDBG("Exit HownDown",0,0,0);
#endif
}
