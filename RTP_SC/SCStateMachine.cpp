/*
 * SCStateMachine.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#include "SCStateMachine.h"
#include "MainTask.h"
#include "IOManager.h"

/************************************************************
 *   Check for all corrupt nodes and add these to the        *
 *   Bad Data list. Any clear nodes found are added to the   *
 *   Free Data list.                                         *
 *************************************************************/
SCStateMachine::SCStateMachine()
{
	currentState = & PowerupObj;
	currentState->Enter();
	SlaveTimeoutCounter=SLAVE_TIME_OUT_IN_US/ECAT_CYCLE_TIME_IN_US;
}

SCStateMachine::~SCStateMachine()
{
}


void SCStateMachine::RunStateMachine()
{  
	
	// clear event for next alarm
	if(PC_TX->PCState != PC_ALARM)
		PC_RX->MasterEvents &=  (UINT16) ~BIT_MASK(CTRL_PC_CLEAR_ALARMS);
	if(AC_TX->ACState != AC_ALARM)
		AC_RX->ACControlEvent &=  (UINT8) ~BIT_MASK(EVENT_AC_CLEAR_ALARMS);
	
	// checks if to set OUTPUT_ULTRASONIC
	CheckSonicActiveOutput ();
	
	// checks if to set OUTPUT_AC_READY
	CheckReadyPositionOutput ();

	// checks if to set OUTPUT_AC_HOME
	CheckHomePositionOutput ();

	currentState->Loop();
}


void SCStateMachine::ChangeState(SCSTATES newState)
{
	currentState->Exit();
	PC_RX->MasterState = newState;
	AC_RX->SCState = newState;
	
	// assess alarm state effect on exiting state
	if (newState == SC_ALARM)
	{
		// add cases here
		switch (GetStateType())
		{
		case CALIBRATION_FIRST_POINT:
		case CALIBRATION_SECOND_POINT:
			EventLog (CALIBRATION_COMPLETED, RESPONSE_FAIL);			
			break;
		}
	}
	
	switch (newState)
	{
	case PREREADY:
		currentState = &PreReadyObj;
		break;
	case SC_READY:
		currentState = &ReadyObj;
		break;
	case START_SWITCH:
		currentState = &StartSwitchObj;
		break;
	case WAIT_FOR_TRIGGER:
		currentState = &WaitForTriggerObj;
		break;
	case WELD_SONIC_ON:
		currentState = &WeldSonicOnobj;
		break;
	case HOLD:
		currentState = &Holdobj;
		break;	
	case WAIT_FOR_READY_POSITION:
		currentState = &WaitForReadyPosobj;
	    break;
	case SEEK_SONIC_ON:
		currentState = &SeekSonicOnobj;
		break;
	case WAIT_FOR_PART_CONTACT:
		currentState = &WaitForPartContactobj;
		break;
	case SCAN_SONIC_ON:
		currentState = &ScanSonicOnObj;
		break;
	case SC_ALARM:
		currentState = &Alarmobj;
		 break;
	case CALIBRATION_FIRST_POINT:
		currentState = &CalibrationFirstPointObj;
		break;
	case CALIBRATION_SECOND_POINT:
		currentState = &CalibrationSecondPointObj;
		break;
	case TEST_SONICS_ON:
		currentState = &TestSonicOnObj;
		break;
	case HORN_DOWN:
		currentState = &HornDownObj;
		break;
	case JOG_DOWN:
		currentState = &JogDownObj;
		break;
	case JOG_UP:
		currentState = &JogUpObj;
		break;
	case HORN_CLAMP_ALARM:
		currentState = &HornClampAlarmObj;
		break;
	case AC_TO_HOME:
		currentState = &AcToHomeObj;
		break;
	case AC_TO_READY:
		currentState = &AcToReadyObj;
		break;
	default:
		break;
	}
	
	currentState->Enter();
}


SCSTATES SCStateMachine::GetStateType ()
{
	return currentState->GetStateType();
}

/**************************************************************************//**
* \brief  - Returns total weld cycle time.
*
* \param  - None
*
* \return - UINT32 - Cycle time counter
*
******************************************************************************/
UINT32 SCStateMachine::CycleCounter()
{
	UINT32 uiCycleCounter;
	
	uiCycleCounter = WaitForTriggerObj.WaitForTriggerTimeCounter
	                 + WeldSonicOnobj.SonicsOnCounter + Holdobj.HoldTimeCounter + WaitForReadyPosobj.WaitForReadyPosTimeCounter;
	
	return uiCycleCounter;
}

/**************************************************************************//**
* \brief  - Resets all the state's counter for next cycle.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void SCStateMachine::ResetCycleTimeCounter()
{
	StartSwitchObj.uiStartSwitchCounter 			= 0;
	WaitForTriggerObj.WaitForTriggerTimeCounter 	= 0;
	WeldSonicOnobj.SonicsOnCounter					= 0;
	Holdobj.HoldTimeCounter							= 0;
	WaitForReadyPosobj.WaitForReadyPosTimeCounter 	= 0;
}


/**************************************************************************//**
* \brief  - Asserts the sonics active output, one time, whenever amplitude
			is detected. Then clears it when amplitude is no longer detected.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void SCStateMachine::CheckSonicActiveOutput ()
{
	static bool output_high = false;
	static bool output_low = false;
	
	// assert output for sonics active
	if (PC_TX->Amplitude > 0)
	{
		if (!output_high)
			DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ULTRASONIC, DigitalIO::LINE_HIGH);
		
		output_high = true;
		output_low = false;
	}
	else
	{
		if (!output_low)
			DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ULTRASONIC, DigitalIO::LINE_LOW);
		
		output_low = true;
		output_high = false;
	}
}


/**************************************************************************//**
* \brief  - Asserts the AC home output, one time, whenever HOME_POS
* 			bit is active. Then clears it when is no longer active.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void SCStateMachine::CheckHomePositionOutput()
{
	// assert output for AC home
	if (AC_TX->ACStatusEvent & BIT_MASK(HOME_POS))
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_AC_HOME, DigitalIO::LINE_HIGH);
	else
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_AC_HOME, DigitalIO::LINE_LOW);
}

/**************************************************************************//**
* \brief  - Asserts the AC ready output, one time, whenever there it is
* 			in ready state and clears it when in any other state.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void SCStateMachine::CheckReadyPositionOutput()
{
	// assert output for AC ready
	if (AC_TX->ACStatusEvent & BIT_MASK(READY_POS))
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_AC_READY, DigitalIO::LINE_HIGH);
	else
		DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_AC_READY, DigitalIO::LINE_LOW);
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
STATUS SCStateMachine::EventLog (LOG_EVENT eventID, UINT32 prevVal, UINT32 newVal)
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
