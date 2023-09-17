
#include "CalibrationFirstPoint.h"
#include "SC_Common.h"
#include "MainTask.h"
#include "ControlTask.h"
#include "IOManager.h"
#include "AlarmManager.h"
#include "Counters.h"

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
CalibrationFirstPoint::CalibrationFirstPoint()
{
	stateType = CALIBRATION_FIRST_POINT;
}


/**************************************************************************//**
* \brief  - destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
CalibrationFirstPoint::~CalibrationFirstPoint()
{

}


/**************************************************************************//**
* \brief  - This method will be executed when entering the CalibrationFirstPoint state
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void CalibrationFirstPoint::Enter()
{
#ifndef DEBUG
	printf("Enter CalibrationFirstPoint\n");	
#endif
	onPart=false;
	Counters::incrementCounter(LIFETIME_ACTUATIONS);
}


/**************************************************************************//**
* \brief  - This method will be executed every 250us while waiting for the
* \breif  - User to enter the external force reading
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void CalibrationFirstPoint::Loop()
{	

		
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
*
* \return  - none
*
******************************************************************************/
void CalibrationFirstPoint::ProcessLoop ()
{
	if(!onPart)
	{
		if(AC_TX->ACStatusEvent & BIT_MASK(PART_CONTACT_FOUND))
		{
			onPart=true;
			ControlTask::TriggerConsoleTask();
		} else if(((AC_TX->ACInputs & SS1MASK) | (AC_TX->ACInputs & SS2MASK)) != BOTHSTARTSWITCHMASK)
		{
			ProcessAlarm (ALARM_SC, ALARM_START_SWITCH_LOST_EFA);
		}
	} else
	{
		switch(ControlTask::GetCalibrationStep())
		{
		case 1:
			break;
		case 2:
			ChangeState(CALIBRATION_SECOND_POINT);
			break;
		default:
			ChangeState(WAIT_FOR_READY_POSITION);
			break;
		}
	}

}



/**************************************************************************//**
* \brief  - This method will be executed when exiting the CalibrationFirstPoint state
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void CalibrationFirstPoint::Exit()
{
#ifndef DEBUG
	printf("Exit CalibrationFirstPoint\n");	
#endif
}


