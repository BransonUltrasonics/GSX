/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "Ready.h"
#include "ACStateMachine.h"
#include <stdio.h>
#include "AC.h"
extern "C"
{
	#include <customSystemCall.h>
}
#include "AcControlTask.h"

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Ready::Ready() 
{	
	stateType = AC_READY;
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
Ready::~Ready() 
{

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
void Ready::Enter() 
{
	ObjectDictionary::tpdo.ActualForce = 0;
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - Wait for supervisor to say do something
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Ready::Loop() 
{
	if (ACStateMachine::alarm) 
	{
		ChangeState(AC_ALARM);
	} else if (!ACStateMachine::homed) 
	{				
		ChangeState(HOMEING);
	}
	else
	{
		//Keep recipe up to date so that the ready position output is accurate in case the ready position changes
		AcControlTask::ReadWeldRecipeFromOD();
		switch(ObjectDictionary::rpdo.SC_State)
		{
		case WAIT_FOR_TRIGGER: //intentional fall-through
		case HORN_DOWN:
			ChangeState(DOWN_STROKE);
			break;
		case WAIT_FOR_PART_CONTACT:
			ChangeState(FIND_PART_CONTACT);
			break;
		case CALIBRATION_FIRST_POINT:
			ChangeState(FORCE_CALIBRATION);
			break;
		case JOG_UP:
			ChangeState(JOGGING_UP);
			break;
		case JOG_DOWN:
			ChangeState(JOGGING_DOWN);
			break;
		case AC_TO_HOME: //intentional fall-through
		case AC_TO_READY:
			ChangeState(GOING_TO_POSITION);
			break;
		default:
			break;
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
void Ready::Exit() 
{
	AC::servo.setSpeedMultiplier(ObjectDictionary::AcWeldRecipe.WeldForceControl);

	// clear ActuatorPBRelease signal
	ObjectDictionary::tpdo.ACStatusEvent &= ~BIT_MASK(PB_RELEASE_SAFE);
}
