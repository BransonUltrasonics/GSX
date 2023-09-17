/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     It Contains Alarm state functionalities
 
**********************************************************************************************************/


/* Header files */
#include "Alarm.h"
#include "AC.h"
#include "DynamicSystemCall.h"
#include "Commons.h"
#include "GpioAPI.h"
extern "C"
{
	#include <customSystemCall.h>
}

UINT32 Alarm::AlarmsReasonMask = 0;

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/

Alarm::Alarm()
{
	stateType = AC_ALARM;
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

Alarm::~Alarm()
{
	
}

/**************************************************************************//**
* \brief  - State entry code
* \brief  - Enters alarm state
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/

void Alarm::Enter()
{
	printf("\n AC Alarm Enter\n");
	
	OBJCONST TOBJECT OBJMEM * pObjEntry;
	pObjEntry = obj_getobjecthandleSc(INDEX_AC_ALARM);
	UINT8 size;
	UINT16 offset;
	size = sizeof(AlarmsReasonMask);
	offset = obj_getentryoffsetSc(1, pObjEntry) / 8;
	obj_writeSc(INDEX_AC_ALARM, &AlarmsReasonMask, &offset, &size, NUM_OF_ALARM_ENTRY);
	
	ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(PART_CONTACT_FOUND); //clear part contact flag
	
	ReadyPosition = ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset; 
	AC::servo.expectedOvershoot = 0;
	motionDelay = MOTION_DELAY;
	bStackReturnedFlag = false;
	bDoNotSetTarget = false;
	target = 0;
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - monitor for SC Preready and move to AC Ready
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Alarm::Loop()
{
	UINT16 servoAlarmIn;
	
	//If not an alarm which prevents the servo from moving, go back home or to ready position if it is enabled and actuator is currently bellow it
	//TODO: replace ACStateMachine::alarm with something from a new class to interface with the servo amp
	if ((!ACStateMachine::alarm) && (ACStateMachine::homed)) 
	{ 
		if ((AC::servo.WhereIsStack(bStackReturnedFlag) != READY) && (AC::servo.WhereIsStack(bStackReturnedFlag) != HOME)) //Just when the move is done and the horn is neither on Ready or Home position sets a target
		{
			if((ObjectDictionary::AcWeldRecipe.ReadyOffset >= MIN_STROKE) && (ObjectDictionary::AcWeldRecipe.ReadyOffset < (ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - MIN_STROKE)))
			{
				if(ACStateMachine::moveDone)
				{
					if (0 < motionDelay)
						motionDelay--; //Waits 50ms for the stack to settle its movement
					else
					{
						ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(PB_RELEASE_SAFE);
						//Set the target depending where we are and where we are supposed to be
						switch(AC::servo.WhereIsStack(bStackReturnedFlag))
						{
							case BELLOW_READY:
								target = ReadyPosition;
							break;
		
							case ABOVE_READY: //Intentional fall-through
								bStackReturnedFlag = true;
							case BELLOW_HOME:
								target = 0;
							break;
							
							default:
								bDoNotSetTarget = true;
							break;
						}
						
						if(((EqepGetPosCount(ROTARY)/ENCODER_RES) > (INT32)target) && (!bDoNotSetTarget)) //Only set targets lower than the actual position
						{
							AC::servo.setTarget(target,
												ObjectDictionary::AcWeldRecipe.ReturnAcceleration,
												ObjectDictionary::AcWeldRecipe.ReturnDeceleration,
												ObjectDictionary::AcWeldRecipe.ReturnMaxVelocity,
												false);
							motionDelay = MOTION_DELAY;
						}
					}
				}
			}
			else
			{
			//TODO Set alarm of data out of the specified parameters
			AC::servo.setTarget(0,
								ObjectDictionary::AcWeldRecipe.ReturnAcceleration,
								ObjectDictionary::AcWeldRecipe.ReturnDeceleration,
								ObjectDictionary::AcWeldRecipe.ReturnMaxVelocity,
								false);
			}
		}
	}
	
	if (ObjectDictionary::rpdo.AC_ControlEvent & (UINT16)BIT_MASK(EVENT_AC_CLEAR_ALARMS))
	{
		GpioGetVal(SERVO_AMP_ALARM, &servoAlarmIn);
		if(servoAlarmIn)
		{
			printf("\n AC Alarm Loop : ChangeState SERVO_RESET\n");
			ChangeState(SERVO_RESET);
		} else if(!ACStateMachine::homed)
		{
			printf("\n AC Alarm Loop : ChangeState HOMEING\n");
			ChangeState(HOMEING);
		}else if(((AC::servo.WhereIsStack(bStackReturnedFlag) == READY) || (AC::servo.WhereIsStack(bStackReturnedFlag) == HOME)) && (ACStateMachine::moveDone))// Horn is either on Ready or home position
		{
			printf("\n AC Alarm Loop : ChangeState AC_READY\n");
			ChangeState(AC_READY);
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

void Alarm::Exit()
{
	printf("\n AC Alarm Exit \n");
	
	ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(PB_RELEASE_SAFE);
	
	if (!ACStateMachine::GetEstop())
	{
		AlarmsReasonMask = ERR_NONE;
		ACStateMachine::alarm = false;
	}
	
	AC::servo.expectedOvershoot = DEFAULT_EXPECTED_OVERSHOOT;
}

/**************************************************************************//**
* \brief  - Update the mask with the TempErrReason Given.
*
* \param  - TempErrReason: New alarm to be set on the Mask
*
*
* \return  - none.
*
******************************************************************************/
void Alarm::SetAlarmReason(UINT32 TempAlarmReason)
{
	AlarmsReasonMask |= TempAlarmReason;

}


/**************************************************************************//**
* \brief  - Clears the mask with the TempErrReason Given.
*
* \param  - TempErrReason: New alarm to be cleared on the Mask
*
*
* \return  - None
*
******************************************************************************/
void Alarm::ClearAlarmReason(UINT32 TempAlarmReason)
{
	AlarmsReasonMask &= ~TempAlarmReason;
}


/**************************************************************************//**
* \brief  - Gets the current mask with all the TempErrReason Given.
*
* \param  - None
*
*
* \return  - UINT32 
*
******************************************************************************/
UINT32 Alarm::GetAlarmMask()
{
	return AlarmsReasonMask;
}
