/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "DownStroke.h"
#include "ACStateMachine.h"
#include "Common.h"
#include "AC.h"
#include "Servo.h"
#include <math.h>
#include "Commons.h"

extern "C"
{
	#include <customSystemCall.h>
}

#define MAX_FRICTIONAL_OFFSET -100 //Compression reading for minimum force may be negative due to frictional forces increasing with the stroke length

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
DownStroke::DownStroke() {	
	stateType = DOWN_STROKE;
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
DownStroke::~DownStroke() {
}

/**************************************************************************//**
* \brief  - State entry code
* \brief  - Start move to expected part contact position
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void DownStroke::Enter() {
	INT32 minTriggerCompression, maxForceCompression;
	
	start=EqepGetPosCount(LINEAR);
	if((start / ENCODER_RES) > FULL_STROKE)//in case of negative encoder reading
		start=0;

	if (0 == AC::servo.forceConsts.GetForceConstant(1))//prevent divide by 0
	{
		Alarm::SetAlarmReason ((UINT32)ERR_RECALIBRATE);
		ChangeState(AC_ALARM);
	} else
	{
		minTriggerCompression = round(((MIN_TRIGGER_FORCE - (AC::servo.forceConsts.GetForceConstant(0))) / (AC::servo.forceConsts.GetForceConstant(1))));
		maxForceCompression = round(((MAX_FORCE - (AC::servo.forceConsts.GetForceConstant(0))) / (AC::servo.forceConsts.GetForceConstant(1))));
		if(MAX_FRICTIONAL_OFFSET >= minTriggerCompression || MAX_COMPRESSION <= maxForceCompression) //if the minimum trigger force or maximum force is not a possible compression value then the calibration is invalid
		{
			Alarm::SetAlarmReason ((UINT32)ERR_RECALIBRATE);
			ChangeState(AC_ALARM);
		} else
		{
			if(ObjectDictionary::AcWeldRecipe.DownAcceleration > 0 && ObjectDictionary::AcWeldRecipe.DownDeceleration > 0 && ObjectDictionary::AcWeldRecipe.DownMaxVelocity > 0)
			{
				AC::servo.setTarget((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ExpectedPartContactOffset),ObjectDictionary::AcWeldRecipe.DownAcceleration,
						ObjectDictionary::AcWeldRecipe.DownDeceleration,ObjectDictionary::AcWeldRecipe.DownMaxVelocity, true);
			} else
			{
				AC::servo.setTarget((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ExpectedPartContactOffset),DEFAULT_ACCELERATION,DEFAULT_ACCELERATION,DEFAULT_SPEED,true);	
			}

			if(0==ObjectDictionary::AcWeldRecipe.ForceRampTime)
			{
				AC::servo.setTargetCompression(round(((ObjectDictionary::AcWeldRecipe.ForceStepForce[0] - (AC::servo.forceConsts.GetForceConstant(0))) / (AC::servo.forceConsts.GetForceConstant(1)))));
			}
			else
			{
				AC::servo.setTargetCompression(minTriggerCompression);
			}
			ACStateMachine::partContactFound = FALSE;
			top = EqepGetPosCount(ROTARY);
			bottom = EqepGetPosCount(LINEAR);
		}
	}
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - monitor for early contact and wait for move to expected part contact position to finish
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
	
void DownStroke::Loop() {
	INT32 topLast, bottomLast, springCompression, targetPos; 
	UINT16 targetForce;
	topLast = top;
	bottomLast = bottom;
	top = EqepGetPosCount(ROTARY);
	bottom = EqepGetPosCount(LINEAR);
	springCompression = top - bottom;
	if((bottom / ENCODER_RES) > (ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.PartContactWindowMinus)
			&& (bottom / ENCODER_RES) < FULL_STROKE)
	{
		ObjectDictionary::tpdo.ActualForce = AC::servo.CalculateForce();
	} else
	{
		ObjectDictionary::tpdo.ActualForce = 0;
	}
	if (ACStateMachine::alarm) {
		ChangeState(AC_ALARM);
	} else if (SC_ALARM == ObjectDictionary::rpdo.SC_State || WAIT_FOR_READY_POSITION == ObjectDictionary::rpdo.SC_State) {
		AC::servo.servoStop();
		ChangeState(RETURN_STROKE); //if Supervisor has an alarm and actuator doesn't, return to a safe position (e.g. start switch lost)
	} else if(ObjectDictionary::rpdo.SC_State == HOLD) {		
		ChangeState(FORCE_CONTROL);
	} else
	{
		//Look for part contact
		if(!ACStateMachine::partContactFound)
		{
			//look for the change in rotary encoder to be at least double the change in the linear encoder
			//and a minimum compression amount to ensure contact has been made
			if(((top - topLast) > (PART_CONTACT_RATIO * (bottom - bottomLast)) && bottom > (start + (ACCEL_WINDOW * ENCODER_RES)))
								|| (springCompression > (MAX_PART_CONTACT_COMPRESSION * ENCODER_RES)))
			{
				ACStateMachine::partContactFound = TRUE;
				ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(PART_CONTACT_FOUND); //tell SC we found it 
				
				//printf("bottom: %d PartContactWindowMinus:%d\n", bottom, ObjectDictionary::AcWeldRecipe.PartContactWindowMinus);
				if (bottom < ((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.PartContactWindowMinus) * ENCODER_RES))
				{
					AC::servo.servoStop(false);
					Alarm::SetAlarmReason ((UINT32)ERR_PART_WINDOW);
					ChangeState(AC_ALARM);
				} else
				{
					AC::servo.servoStop(true);
					ChangeState(FORCE_CONTROL);
				}
			}
			else if (ACStateMachine::moveDone)
			{
				ChangeState(FORCE_CONTROL);
			}	
		} 
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
void DownStroke::Exit() {

	

}
