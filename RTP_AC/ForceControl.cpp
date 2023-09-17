#include "ForceControl.h"
#include "ACStateMachine.h"
#include "Servo.h"
#include "AC.h"
#include <math.h>
#include "Commons.h"

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
ForceControl::ForceControl() {	
	stateType = FORCE_CONTROL;
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
ForceControl::~ForceControl() {
}


/**************************************************************************//**
* \brief  - State entry code
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ForceControl::Enter() {
	if(1 >= ObjectDictionary::AcWeldRecipe.NumForceSteps)
	{
		forceStep = START;
	} else
	{
		forceStep = STEP1;
	}	
	
	if ( GetStepRampTime() == 0 )
	{
		rampDone = true;
	} else
	{
		rampDone = false;
		rampCounter = 0;
		rampStartForce = 0;
	}
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - Determine the appropriate force to apply based upon the weld recipe parameters such as steps and ramp time
* \brief  - Make the servo move to the current linear encoder reading plus the appropriate spring compression distance for the desired force
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ForceControl::Loop() {	
	
	if (true == ACStateMachine::alarm) {
		ChangeState(AC_ALARM);
	} else if (ObjectDictionary::rpdo.SC_State != WELD_SONIC_ON && ObjectDictionary::rpdo.SC_State != HOLD && 
			ObjectDictionary::rpdo.SC_State != HORN_CLAMP_ALARM && ObjectDictionary::rpdo.SC_State != HORN_DOWN && ObjectDictionary::rpdo.SC_State != WAIT_FOR_TRIGGER) 
	{				
		//Tell the servo to stop and wait a cycle to let it do so before starting return stroke
		if(AC::servo.getForceControl())
		{
			AC::servo.servoStop(false);
			ObjectDictionary::tpdo.ActualForce = AC::servo.CalculateForce();
		}
		else
		{
			ChangeState(RETURN_STROKE);
		}
	} else {
		INT32 topLast, bottomLast, springCompression;
		topLast = top;
		bottomLast = bottom;
		top = EqepGetPosCount(ROTARY);
		bottom = EqepGetPosCount(LINEAR);
		springCompression = top - bottom;
		
		//Look for part contact
		if(!ACStateMachine::partContactFound)
		{
			if(((top - topLast) > (PART_CONTACT_RATIO * (bottom - bottomLast)) && bottom > (ACCEL_WINDOW * ENCODER_RES))||(springCompression > (MAX_PART_CONTACT_COMPRESSION * ENCODER_RES))) 
			{
				ACStateMachine::partContactFound = TRUE;
				ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(PART_CONTACT_FOUND); //tell SC we found it
			}
		}
		
		//calculate current force
		ObjectDictionary::tpdo.ActualForce = AC::servo.CalculateForce();

		//see if master switched to hold
		if(forceStep != HOLD_FORCE && ObjectDictionary::rpdo.SC_State == HOLD)
		{
			forceStep = HOLD_FORCE;
			if ( GetStepRampTime() == 0 )
				{
					rampDone = true;
				} else
				{
					rampDone = false;
					rampCounter = 0;
					rampStartForce = ObjectDictionary::tpdo.ActualForce;
				}
		}
		
		LookForForceStep();

		ControlForce();
		
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
void ForceControl::Exit() {
}


/**************************************************************************//**
* \brief  - Get the force parameter for the current step
*
* \param  - none
*
*
* \return  - The current step's target force 
*
******************************************************************************/
UINT16 ForceControl::GetStepForce() {
	UINT16 value;
	switch (forceStep)
	{
		case START:
			value = ObjectDictionary::AcWeldRecipe.WeldForce;
			break;
		case HOLD_FORCE:
			value = ObjectDictionary::AcWeldRecipe.HoldForce;
			break;
		case STEP1:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[0];
			break;
		case STEP2:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[1];
			break;
		case STEP3:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[2];
			break;
		case STEP4:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[3];
			break;
		case STEP5:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[4];
			break;
		case STEP6:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[5];
			break;
		case STEP7:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[6];
			break;
		case STEP8:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[7];
			break;
		case STEP9:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[8];
			break;
		case STEP10:
			value = ObjectDictionary::AcWeldRecipe.ForceStepForce[9];
			break;
		default:
			value=0;
	}
	return value;
}


/**************************************************************************//**
* \brief  - Get the force ramp time parameter for the current step
*
* \param  - none
*
*
* \return  - The current step's force ramp time
*
******************************************************************************/
UINT16 ForceControl::GetStepRampTime() {
	UINT16 value;
	switch (forceStep)
	{
		case START:
			value = ObjectDictionary::AcWeldRecipe.ForceRampTime;
			break;
		case HOLD_FORCE:
			value = ObjectDictionary::AcWeldRecipe.HoldForceRampTime;
			break;
		case STEP1:
			value = ObjectDictionary::AcWeldRecipe.ForceRampTime;
			break;
		case STEP2:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[1];
			break;
		case STEP3:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[2];
			break;
		case STEP4:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[3];
			break;
		case STEP5:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[4];
			break;
		case STEP6:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[5];
			break;
		case STEP7:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[6];
			break;
		case STEP8:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[7];
			break;
		case STEP9:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[8];
			break;
		case STEP10:
			value = ObjectDictionary::AcWeldRecipe.ForceStepRampTime[9];
			break;
		default:
					value=0;
	}
	return value;
}


/**************************************************************************//**
* \brief  - look for force step command
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ForceControl::LookForForceStep() {
	if(forceStep != HOLD_FORCE && 1 < ObjectDictionary::AcWeldRecipe.NumForceSteps)
	{
		if(forceStep < ((ObjectDictionary::rpdo.AC_ControlEvent & 0xF0) >> 4))
		{
			forceStep = (ObjectDictionary::rpdo.AC_ControlEvent & 0xF0) >> 4;
			if ( GetStepRampTime() == 0 )
			{
				rampDone = true;
			} else
			{
				rampDone = false;
				rampCounter = 0;
				rampStartForce = ObjectDictionary::tpdo.ActualForce;
			}
		}
	}
}


/**************************************************************************//**
* \brief  - Calculate the target force and request to servo to move to compress the springs and apply that force
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ForceControl::ControlForce() {
	INT32 targetCompress; 
	UINT16 targetForce;
	//calculate target force
	if(++rampCounter == GetStepRampTime() * CYCLES_PER_MS)
	{
		rampDone=true;
	}
	if (rampDone)
	{
		targetForce = GetStepForce();
	} else 
	{
		targetForce = rampStartForce + ((GetStepForce() - rampStartForce) * rampCounter / (GetStepRampTime() * CYCLES_PER_MS));
	}
	
	if(0 != AC::servo.forceConsts.GetForceConstant(1))
	{
		targetCompress = round(((targetForce - (AC::servo.forceConsts.GetForceConstant(0))) / (AC::servo.forceConsts.GetForceConstant(1))));
	} else
	{
		//TODO: error case
	}
	
	if (((bottom > ((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition + ObjectDictionary::AcWeldRecipe.PartContactWindowPlus) * ENCODER_RES))
			&& (ObjectDictionary::rpdo.SC_State == WAIT_FOR_TRIGGER || ObjectDictionary::rpdo.SC_State == HORN_DOWN)) 
			|| ((bottom < ((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.PartContactWindowMinus) * ENCODER_RES)) && 
					ACStateMachine::partContactFound)) 
	{
		AC::servo.servoStop();
		Alarm::SetAlarmReason ((UINT32)ERR_PART_WINDOW);
		ChangeState(AC_ALARM);
	}
	else
	{
		AC::servo.setTargetCompression(targetCompress);
	}
}
