#include "GoingToPosition.h"

#include "ACStateMachine.h"
#include "AC.h"
#include "Servo.h"
extern "C"
{
	#include <customSystemCall.h>
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
GoingToPosition::GoingToPosition() {	
	stateType = GOING_TO_POSITION;
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
GoingToPosition::~GoingToPosition() {
}


/**************************************************************************//**
* \brief  - State entry code
* \brief  - Start move to commanded position
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void GoingToPosition::Enter() {
	start=EqepGetPosCount(LINEAR);
	top = EqepGetPosCount(ROTARY);
	bottom = start;
	/* Determine position to go to */
	if(ObjectDictionary::rpdo.SC_State == AC_TO_READY  
			&& ObjectDictionary::AcWeldRecipe.ReadyOffset <= ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition
			&& ObjectDictionary::AcWeldRecipe.ReadyPositionToggle && MIN_STROKE <= ObjectDictionary::AcWeldRecipe.ReadyOffset) //go to ready
	{
		//Ready position in weld recipe is an offset from part contact, convert it to an absolute position
		targetPosition = (ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset);
	}
	else //go home
	{
		targetPosition = 0;
	}
	
	AC::servo.setTarget(targetPosition, DEFAULT_ACCELERATION, DEFAULT_ACCELERATION, DEFAULT_SPEED, false); //Go to position motion parameters undefined - use defaults
	
	ACStateMachine::partContactFound = FALSE;
	ObjectDictionary::tpdo.ACStatusEvent &=~ BIT_MASK(PART_CONTACT_FOUND); //clear part contact flag
	motionDelay = MOTION_DELAY;
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - Wait for move to commanded position and make sure it gets to the right place
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void GoingToPosition::Loop() {	
	INT32 offset, topLast, bottomLast, springCompression;
	if(ObjectDictionary::rpdo.SC_State == PREREADY || ObjectDictionary::rpdo.SC_State == SC_READY) 		//Change on Master State
	{
		AC::servo.servoStop(false);
		ChangeState(AC_READY); 		
	}else if (ObjectDictionary::rpdo.SC_State == SC_ALARM) //Master alarm such as start switch lost. Retract to safe position
	{
		AC::servo.servoStop(false);
		ChangeState(RETURN_STROKE); 		
	} else if (ACStateMachine::moveDone) 
	{
		if (0==(--motionDelay))
		{
			offset = (EqepGetPosCount(ROTARY)/ENCODER_RES) - targetPosition;
			//Make sure we returned to ready position successfully
			if(offset > (MAX_SERVO_ERROR / 10) || offset < -(MAX_SERVO_ERROR / 10)) //within a fraction of the max error because we gave the system time to catch up
			{
				Alarm::SetAlarmReason ((UINT32)ERR_SERVO_MOTOR);
				ACStateMachine::homed = false;
				ChangeState(AC_ALARM);
			} 
			else
			{
				ChangeState(AC_READY);
			}
		}
	}else if (targetPosition > (start / ENCODER_RES)) //If moving down check to make sure we don't hit anything
	{
		topLast = top;
		bottomLast = bottom;
		top = EqepGetPosCount(ROTARY);
		bottom = EqepGetPosCount(LINEAR);
		springCompression = top - bottom;
		if(((top - topLast) > (PART_CONTACT_RATIO * (bottom - bottomLast)) && bottom > (start + (ACCEL_WINDOW * ENCODER_RES)))
										|| (springCompression > (MAX_PART_CONTACT_COMPRESSION * ENCODER_RES)))
		{
			AC::servo.servoStop(false);
			Alarm::SetAlarmReason ((UINT32)ERR_PART_WINDOW);
			ChangeState(AC_ALARM);
		}
	} else if ( ObjectDictionary::tpdo.ActualForce > 0 ) //going up, calculate force if it was non-zero to start
	{
		springCompression = EqepGetPosCount(ROTARY) - EqepGetPosCount(LINEAR);
		ObjectDictionary::tpdo.ActualForce = AC::servo.CalculateForce();
	}
}


/**************************************************************************//**
* \brief  - State exit code
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void GoingToPosition::Exit() {
	
}
