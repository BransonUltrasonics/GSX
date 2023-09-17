
#include "CalibrationSecondPoint.h"
#include "SC_Common.h"
#include "MainTask.h"
#include "ControlTask.h"
#include "IOManager.h"
#include "AlarmManager.h"

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
CalibrationSecondPoint::CalibrationSecondPoint()
{
	stateType = CALIBRATION_SECOND_POINT;
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
CalibrationSecondPoint::~CalibrationSecondPoint()
{

}


/**************************************************************************//**
* \brief  - This method will be executed when entering the CalibrationSecondPoint state
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void CalibrationSecondPoint::Enter()
{
#ifndef DEBUG
	printf("Enter CalibrationSecondPoint\n");	
#endif
	
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
void CalibrationSecondPoint::Loop()
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
void CalibrationSecondPoint::ProcessLoop ()
{
	switch(ControlTask::GetCalibrationStep())
	{
	case 2:
		break;
	default:
		
		// partially assume that calibration succeeded
		EventLog (CALIBRATION_COMPLETED, RESPONSE_SUCCESS);
		
		ChangeState(WAIT_FOR_READY_POSITION);
		break;
	}
}


/**************************************************************************//**
* \brief  - This method will be executed when exiting the CalibrationSecondPoint state
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void CalibrationSecondPoint::Exit()
{
#ifndef DEBUG
	printf("Exit CalibrationSecondPoint\n");	
#endif
}


/**************************************************************************//**
* \brief   - Prepares log information for specific requests to be logged in
* 			the event log, and sends it to the DB task for further processing
*
* \param   - LOG_EVENT 	eventID
* \param   - UINT32 prevVal
* \param   - UINT32 newVal
*
* \return  - STATUS status
*
******************************************************************************/
STATUS CalibrationSecondPoint::EventLog (LOG_EVENT eventID, UINT32 prevVal, UINT32 newVal)
{
	STATUS _status = OK;
	string pass_fail (prevVal == RESPONSE_SUCCESS ? "PASS" : "FAIL");
	string details;
	
	// add cases here
	switch (eventID)
	{
	case CALIBRATION_COMPLETED:
		details = "Identifier:" + pass_fail + ":From:N/A:To:N/A:";
		break;
	default:
		_status = ERROR;
		break;
	}
	
	if (_status != ERROR)
		_status = DataBaseManager::EnterEventLog (eventID, details);

	return _status;
}
