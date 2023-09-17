#include "ReturnStroke.h"
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
ReturnStroke::ReturnStroke() {	
	stateType = RETURN_STROKE;
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
ReturnStroke::~ReturnStroke() {
}


/**************************************************************************//**
* \brief  - State entry code
* \brief  - Start move to ready position
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ReturnStroke::Enter() {
	/* Check for ready position option enabled or not */
	if(ObjectDictionary::AcWeldRecipe.ReadyPositionToggle && MIN_STROKE <= ObjectDictionary::AcWeldRecipe.ReadyOffset 
			&& ObjectDictionary::AcWeldRecipe.ReadyOffset <= ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition
			&& (ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset) < (EqepGetPosCount(ROTARY)/ENCODER_RES))
	{
		//Ready position in weld recipe is an offset from part contact, convert it to an absolute position
		readyPosition = (ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset);
	}
	else
	{
		readyPosition = 0;
	}
	
	if(ObjectDictionary::AcWeldRecipe.ReturnAcceleration > 0 && ObjectDictionary::AcWeldRecipe.ReturnDeceleration > 0 && ObjectDictionary::AcWeldRecipe.ReturnMaxVelocity > 0)
	{
		AC::servo.setTarget(readyPosition, ObjectDictionary::AcWeldRecipe.ReturnAcceleration,
			ObjectDictionary::AcWeldRecipe.ReturnDeceleration,ObjectDictionary::AcWeldRecipe.ReturnMaxVelocity, false);
		
	} else
	{
		AC::servo.setTarget(0,DEFAULT_ACCELERATION,DEFAULT_ACCELERATION,DEFAULT_SPEED,false);
	}
	
	ACStateMachine::partContactFound = FALSE;
	ObjectDictionary::tpdo.ACStatusEvent &=~ BIT_MASK(PART_CONTACT_FOUND); //clear part contact flag
	motionDelay = MOTION_DELAY;
	tooSoon=8;
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - Wait for move back to ready position
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ReturnStroke::Loop() {	
	INT32 offset;
//	AC::servo.handleInterupt();	
	if (ACStateMachine::alarm) {
		ChangeState(AC_ALARM);
	}else if (tooSoon>0) {
		tooSoon--; // If moveDone is seen in the first few ms this likely is from the previous move having not finished yet when entering this state. Resend the move command.
		if (ACStateMachine::moveDone)
		{
			if(ObjectDictionary::AcWeldRecipe.ReturnAcceleration > 0 && ObjectDictionary::AcWeldRecipe.ReturnDeceleration > 0 && ObjectDictionary::AcWeldRecipe.ReturnMaxVelocity > 0)
			{
				if(0<readyPosition && readyPosition < ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - MIN_STROKE)
				{
					AC::servo.setTarget(readyPosition, ObjectDictionary::AcWeldRecipe.ReturnAcceleration,
							ObjectDictionary::AcWeldRecipe.ReturnDeceleration,ObjectDictionary::AcWeldRecipe.ReturnMaxVelocity, false);
				} else
				{
					AC::servo.setTarget(0,ObjectDictionary::AcWeldRecipe.ReturnAcceleration,
							ObjectDictionary::AcWeldRecipe.ReturnDeceleration,ObjectDictionary::AcWeldRecipe.ReturnMaxVelocity, false);
				}
			} else
			{
				AC::servo.setTarget(0,DEFAULT_ACCELERATION,DEFAULT_ACCELERATION,DEFAULT_SPEED,false);
			}
		}
	} else if (ACStateMachine::moveDone) {
		if (0 < motionDelay)
			motionDelay--;
		else
		{
			offset = (EqepGetPosCount(ROTARY)/ENCODER_RES) - readyPosition;
			//Make sure we returned to ready position successfully and update expected overshoot based on actual overshoot of the return stroke
			if ( (offset < MIN_STROKE) && (offset > (0 - MIN_STROKE)))
			{
				AC::servo.expectedOvershoot -= offset;
				ChangeState(AC_READY);
			}
			else
			{
				Alarm::SetAlarmReason ((UINT32)ERR_SERVO_MOTOR);
				ACStateMachine::homed = false;
				ChangeState(AC_ALARM);
				return;
			}
		}
	} else if ( ObjectDictionary::tpdo.ActualForce > 0 ){

		INT32 springCompression = EqepGetPosCount(ROTARY) - EqepGetPosCount(LINEAR);
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
void ReturnStroke::Exit() {
	
}
