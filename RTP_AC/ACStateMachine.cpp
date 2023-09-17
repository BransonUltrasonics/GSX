/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "ACStateMachine.h"
#include "AC.h"
#include "GpioAPI.h"
#include "Commons.h"

extern "C"
{
	#include <customSystemCall.h>
}
#include "AcCommon.h"
//To build RTP_AC for PACC board, define this macro in Properties->BuildProperties->Defines of RTP_AC WindRiver project
//#define PACC

//initialize static variables
bool ACStateMachine::Estop = false;
bool ACStateMachine::alarm = false;
bool ACStateMachine::moveDone = true;		
bool ACStateMachine::partContactFound = false;
bool ACStateMachine::homed = false;
UINT32 ACStateMachine::foundPartContactPos = 0;


#define GrdDet 0x04
#define SERVO_ALARM_DELAY 200

void ACStateMachine::PrintMasterState() {
	#ifdef DEBUG
	switch (ObjectDictionary::rpdo.SC_State) {
	case PREREADY:
		LOG("Master State: Pre-Ready\n");		
		break;
	case SC_READY:
		LOG("Master State: Ready\n");			
		break;
	case START_SWITCH:
		LOG("Master State: Start Switch\n");				
		break;
	case WAIT_FOR_TRIGGER:
		LOG("Master State: Wait For Trigger\n");				
		break;
	case WELD_SONIC_ON:
		LOG("Master State: Weld Sonics On\n");				
		break;
	case HOLD:
		LOG("Master State: Hold\n");				
		break;
	case WAIT_FOR_READY_POSITION:
		LOG("Master State: Wait for Ready Position\n");				
		break;
	case SC_ALARM:
		LOG("Master State: SC Alarm\n");				
		break;
	case WAIT_FOR_PART_CONTACT:
		LOG("Master State: Wait for Part Contact\n");				
		break;
	case SEEK_SONIC_ON:
		LOG("Master State: Seek Sonic ON\n");				
		break;
	default:
		LOG("Master State: # %i \n", ObjectDictionary::rpdo.SC_State);				
		break;
	}
#endif	//PRINT
}


void ACStateMachine::PrintCurrentState() {
	#ifdef DEBUG
	switch (ObjectDictionary::tpdo.ACState) {
	case AC_READY:
		LOG("AC State: Ready\n");				
		break;
	case DOWN_STROKE:
		LOG("AC State: DownStroke\n");					
		break;
	case FORCE_CONTROL:
		LOG("AC State: ForceControl\n");					
		break;
	case RETURN_STROKE:
		LOG("AC State: ReturnStroke\n");					
		break;
	case AC_ALARM:
		LOG("AC State: AcAlarm\n");				
		break;
	case FIND_PART_CONTACT:
		LOG("AC State: FindPartContact\n");				
		break;
	case HOMEING:
		LOG("AC State: Homeing\n");				
		break;
	default:
		LOG("State # %i \n", ObjectDictionary::tpdo.ACState);					
		break;
	}
#endif	//PRINT
}


/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ACStateMachine::ACStateMachine() {	
#ifndef PACC
	GpioInit();
#endif
	currentState = &ready;
	ObjectDictionary::tpdo.ACState = currentState->stateType;
	currentState->Enter();
	unknownFaultCount = 0;
	servoAlarmCount = 0;
}


/**************************************************************************//**
* \brief  - Default destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ACStateMachine::~ACStateMachine() {
}


/**************************************************************************//**
* \brief  - State machine execution. Runs the loop function of the current state
* \breif  - Also does any action required once per cycle regardless of current state
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ACStateMachine::RunStateMachine() {	
	if(EqepGetPosCount(LINEAR) < 0)
	{
		ObjectDictionary::tpdo.ActualDistance = 0;
	} else
	{
		ObjectDictionary::tpdo.ActualDistance = EqepGetPosCount(LINEAR)/ENCODER_RES;
	}
#ifdef PACC
	ObjectDictionary::tpdo.ACInputs = GetDigitalIn();
	digOut = open( "/tpic-led/0", O_RDWR, 0777);
	write( digOut, (char *)&ObjectDictionary::rpdo.AC_Outputs, 1);
	close(digOut);
#else
	if(digOut != (AC_OUTPUT_MASK & ObjectDictionary::rpdo.AC_Outputs)) 
	{
		digOut = (AC_OUTPUT_MASK & ObjectDictionary::rpdo.AC_Outputs);
		AcDgtOutput::AcDgtSetDoutValue(digOut);
	}
	UINT16 PB1Temp, PB2Temp, eStop1Temp, eStop2Temp, GndDetTemp, ELCcode, ELCtemp, servoAlarmIn;
	UINT8 DINval;
	GpioSetVal(LATCH_OE, GPIO_VALUE_LOW);
	GpioGetVal(PB1_SIG, &PB1Temp); 
	GpioGetVal(PB2_SIG, &PB2Temp); 
	GpioGetVal(ESTOP1_SIG, &eStop1Temp); 
	GpioGetVal(ESTOP2_SIG, &eStop2Temp);
	GpioGetVal(GND_DET_SIG, &GndDetTemp);
	GpioGetVal(SERVO_AMP_ALARM, &servoAlarmIn);
	if(servoAlarmIn)
		servoAlarmCount++;
	else
		servoAlarmCount = 0;
	DINval = AcGetDinValue();
	ObjectDictionary::tpdo.ACInputs = PB1Temp;
	ObjectDictionary::tpdo.ACInputs |= PB2Temp << 1;
	ObjectDictionary::tpdo.ACInputs |= DINval << 3;
	GpioGetVal(ENLOG_ALARM0, &ELCtemp);
	ELCcode = ELCtemp;
	GpioGetVal(ENLOG_ALARM1, &ELCtemp);
	ELCcode |= ELCtemp << 1;
	GpioGetVal(ENLOG_ALARM2, &ELCtemp);
	ELCcode |= ELCtemp << 2;
	GpioGetVal(ENLOG_ALARM3, &ELCtemp);
	ELCcode |= ELCtemp << 3;
	
	// PDO notify
	if(ACStateMachine::homed)
	{
		if (ObjectDictionary::AcWeldRecipe.ReadyPositionToggle)
		{
			if(EqepGetPosCount(ROTARY)/ENCODER_RES > (INT32)((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset) + (MAX_SERVO_ERROR/10)))
				ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(READY_POS);
			else if(EqepGetPosCount(ROTARY)/ENCODER_RES < (INT32)(((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset) - (MAX_SERVO_ERROR))))
				ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(READY_POS);
			else
				ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(READY_POS);
		}
		else
		{
			if (EqepGetPosCount(ROTARY) < MAX_SERVO_ERROR) //within acceptable tolerance of home position
				ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(READY_POS);
			else
				ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(READY_POS);
		}
		
		if (EqepGetPosCount(ROTARY) < MAX_SERVO_ERROR) //within acceptable tolerance of home position
			ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(HOME_POS);
		else
			ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(HOME_POS);
	}
	else
	{
		ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(READY_POS);
		ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(HOME_POS);
	}

	
	//LOG("GroundDet: GndDetTemp=%d\n", GndDetTemp);
	if (GndDetTemp==0)
	{
		ObjectDictionary::tpdo.ACInputs|=GrdDet;
		//LOG("ground detect\n");	
		
	}
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_HIGH);
	GpioSetVal(SV3_EN, ((ObjectDictionary::rpdo.AC_Outputs & (UINT16)BIT_MASK(CONVERTER_COOLING)) >> CONVERTER_COOLING));
	GpioSetVal(LATCH_CLK, GPIO_VALUE_HIGH);
	
	/* Check for E-Stop:
	 * Estop1 and 2 are a differential pair: estop1 && ! estop2 means the button is not pressed,
	 * estop2 && ! estop1 means it is pressed, and any other combination is an error
	 */
	if((eStop2Temp || (! eStop1Temp)))
	{
		Estop = true;
		
		//TODO: differentiate between estop pressed and estop error
		if(currentState->stateType != AC_ESTOP)
		{
			ACStateMachine::homed = false;
			ChangeState(AC_ESTOP);
		}
	} else
	{
		Estop = false;
		//check for servo alarm
		if(servoAlarmCount > SERVO_ALARM_DELAY)
		{ 
			ACStateMachine::homed = false;
			AC::servo.servoStop();
			if(currentState->stateType != AC_ALARM && currentState->stateType != SERVO_RESET && currentState->stateType != AC_ESTOP 
					&& ((currentState->stateType != HOMEING) || (servoAlarmIn)))
			{
				Alarm::SetAlarmReason ((UINT32)ERR_SERVO_MOTOR);
				ChangeState(AC_ALARM);
			}
		}
		//if PDO timeout when horn is down, bring it back up
		else if(ObjectDictionary::rpdo.SC_State == SC_UNREACHABLE &&
				currentState->stateType != AC_ALARM && currentState->stateType != HOMEING &&
				currentState->stateType != AC_READY && currentState->stateType != RETURN_STROKE)
		{
			AC::servo.servoStop();
			ChangeState(RETURN_STROKE);
			//no point going to alarm, as SC won't see it until it has been resolved
		}
	}
#endif
	if(0xF != ELCcode) //EN fault
	{
		ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(EN_FAULT);
		if(AC_ALARM != currentState->stateType)
		{
			ACStateMachine::homed = false;
			switch (ELCcode)
			{
			case 0x1://PBFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_PB);
				ChangeState(AC_ALARM);
				break;
			case 0x2://UFAIL24Fault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_24V);
				ChangeState(AC_ALARM);
				break;
			case 0x3://EStopFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_ESTOP);
				ChangeState(AC_ALARM);
				break;
			case 0x4://LEFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_LE);
				ChangeState(AC_ALARM);
				break;
			case 0x5://SBeamFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_SBEAM);
				ChangeState(AC_ALARM);
				break;
			case 0x6://TRSFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_TRS);
				ChangeState(AC_ALARM);
				break;
			case 0x7://DriveFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_DRIVE);
				ChangeState(AC_ALARM);
				break;
			case 0x8://CMFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_CM);
				ChangeState(AC_ALARM);
				break;
			case 0x9://LUFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_LU);
				ChangeState(AC_ALARM);
				break;
			case 0xA://SonicsEnFault
				Alarm::SetAlarmReason ((UINT32)ERR_ELC_SONICS_EN);
				ChangeState(AC_ALARM);
				break;
			default://unknown fault
				if(unknownFaultCount++ > 4)//Give ELC time for the 2 processors to stabilize to the same alarm code
				{
					Alarm::SetAlarmReason ((UINT32)ERR_ELC_UNKNOWN);
					ChangeState(AC_ALARM);
				}
			}
		}
	}
	else //no EN fault
	{
		ObjectDictionary::tpdo.ACStatusEvent &=~ BIT_MASK(EN_FAULT);
		unknownFaultCount=0;
	}
	currentState->Loop();
	ObjectDictionary::tpdo.ACState = currentState->stateType;
	GpioSetVal(LATCH_CLK, GPIO_VALUE_LOW);
}

/**************************************************************************//**
* \brief  - Transitions to a new state
* \breif  - Calls the exit code of the old state and the entry code of the new state.
*
* \param  - newState - The state to which to transition
*
*
* \return  - none
*
******************************************************************************/
void ACStateMachine::ChangeState(ACSTATES newState) {
	currentState->Exit();
	switch (newState) {
	case AC_READY:
		currentState = &ready;
		break;
	case DOWN_STROKE:
		currentState = &downStroke;
		break;
	case FORCE_CONTROL:
		currentState = &forceControl;
		break;
	case RETURN_STROKE:
		currentState = &returnStroke;
		break;		
	case AC_ALARM:
		currentState = &acAlarm;
		break;		
	case FORCE_CALIBRATION:
		currentState = &forceCalibration;
		break;
	case FIND_PART_CONTACT:
		currentState = &findPartContact;
		break;
	case HOMEING:
		currentState = &homeing;
		break;
	case JOGGING_UP:
		currentState = &joggingUp;
		break;
	case JOGGING_DOWN:
		currentState = &joggingDown;
		break;
	case AC_ESTOP:
		currentState = &estop;
		break;
	case GOING_TO_POSITION:
		currentState = &goingToPosition;
		break;
	case SERVO_RESET:
		currentState = &servoReset;
		break;
	default:
		break;
	}
	ObjectDictionary::tpdo.ACState = newState;
	currentState->Enter();
}

bool ACStateMachine::GetEstop()
{
	return Estop;
}

