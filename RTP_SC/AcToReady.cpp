/*
 * AcToReady.cpp
 *
 *  Created on: Apr 2, 2019
 *      Author: DShilonie
 *      
 *      This state commences when in ready state with a pending actuator movement operation
 *      while INPUT_AC_READY signal is detected. It will then run its course waiting for the
 *      actuator to finish its travel to the ready position. 
 */

#include "Common.h"
#include "AcToReady.h"
#include "IOManager.h"
#include "ControlTask.h"
#include "AlarmManager.h"
#include <stdio.h>
#include <iostream>

/**************************************************************************//**
* 
* \brief   - Class constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
AcToReady::AcToReady()
{
	stateType = AC_TO_READY;
}

/**************************************************************************//**
* 
* \brief   - Class destructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
AcToReady::~AcToReady()
{
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering this state.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void AcToReady::Enter()
{

	
#ifndef DEBUG
	 printf ("\nEnter AcToReady\n");
#endif
}


/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us. It will monitor the actuator
* 			movement until the sequence is complete.  
* 			   
* \param   - None
*
* \return  - None
*
******************************************************************************/
void AcToReady::Loop()
{
	DigitalIOMgr::ReadInputs ();
	
	if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	else if(AC_TX->ACState == AC_ALARM)
	{
		ProcessAlarm (ALARM_AC);
	}
	else
	{
		if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
		{
			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
			{
				ProcessAlarm (ALARM_SC, ALARM_CYCLE_ABORT_NCA);
				return;
			}
		}
		
		// wait until the sequence is complete
		if (AC_TX->ACStatusEvent & BIT_MASK(READY_POS))
		{
			ControlTask::SetOperationType (ControlTask::GetPrevOperationType());
			ChangeState(PREREADY);
		}
	}
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when exiting this state.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void AcToReady::Exit()
{

	
#ifndef DEBUG
	 printf ("\nExit AcToReady\n");
#endif
}


/**************************************************************************//**
* \brief   - Overrides the base function to restore operation types and 
			prepare for the clearing of these alarms
*
* \param   - UINT8 origin, UINT16 alarmID
*
* \return  - None
*
******************************************************************************/
void AcToReady::ProcessAlarm (UINT8 origin, UINT16 alarmID)
{
	// clean up
	ControlTask::SetOperationType (ControlTask::GetPrevOperationType());
	ControlTask::SetPrevOperationType(AC_GO_READY);
	
	AlarmMgr::EnterAlarmEvent (origin, alarmID);
 	ChangeState(SC_ALARM); 
}





