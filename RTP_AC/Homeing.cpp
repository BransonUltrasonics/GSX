/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "FindPartContact.h"
#include "ACStateMachine.h"
#include "Servo.h"
#include "AC.h"
#include "GpioAPI.h"
#include "AcControlTask.h"
extern "C"
{
	#include <customSystemCall.h>
}

/*
 * HOMING ROUTINE:
 * If Switch is ON at power up:

1.	Move down until switch is off
2.	Continue down for 1 mm
3.	Reverse direction until switch is on
4.	Continue up for 1 mm
5.	Reverse direction until switch is off and call that zero

If Switch is OFF at power up:

1.	Move up until switch is on
2.	Continue up for 1 mm
3.	Reverse direction until switch is off and call that zero
 * 
 */

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Homeing::Homeing() {
	stateType = HOMEING;
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
Homeing::~Homeing() {
	// TODO Auto-generated destructor stub
}


/**************************************************************************//**
* \brief  - State entry code
* \brief  - If starting on home sensor, go down off it first to ensure we are at the  
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Homeing::Enter() {
	UINT16 ULS;
	GpioGetVal(ULS_SIG, &ULS); 
	overshoot = 0;
	settleTime = 0;
	
	AC::servo.servoStop(false);
	GpioSetVal(SV1_REQ, GPIO_VALUE_LOW);
	
	//If starting on home sensor, go down off it first to ensure homed at a consistent spot
	if(ULS) {
		direction = DOWNCHANNEL;
		goBackUp = true;
		EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, UPCHANNEL);
	} else {
		direction = UPCHANNEL;
		goBackUp = false;
		EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, DOWNCHANNEL);
	}
}

/**************************************************************************//**
* \brief  - State loop code
* \brief  - Move one pulse at a time to ensure accurate home position
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Homeing::Loop() {
	UINT16 ULS;
	GpioGetVal(ULS_SIG, &ULS); 
	
	if (direction == UPCHANNEL) { 	
		if(ULS) {
			if(++overshoot > 1000)
			{
				direction = DOWNCHANNEL;
				EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, UPCHANNEL);
			} else 
			{
				EhrPwmSetAQSFRC(AQSFRC_TOGGLE_CHANNEL_NOW, direction);
			}
		} else 
		{
			EhrPwmSetAQSFRC(AQSFRC_TOGGLE_CHANNEL_NOW, direction);
		}
	} else { 
		if(ULS) {
			
			EhrPwmSetAQSFRC(AQSFRC_TOGGLE_CHANNEL_NOW, direction);
		} else if(goBackUp){
			if(++overshoot > 1000)
			{
				overshoot = 0;
				goBackUp = false;
				direction = UPCHANNEL;
				EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, DOWNCHANNEL);
			} else
			{
				EhrPwmSetAQSFRC(AQSFRC_TOGGLE_CHANNEL_NOW, direction);
			}
		} else
		{
			if(++settleTime > 1000)
				ChangeState(AC_READY);
		}
	}
	
}

/**************************************************************************//**
* \brief  - State exit code
* \brief  - Zero the encoders and reinitialize the servo
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Homeing::Exit() {
	//Get weld recipe to be able to accurately assert home and ready outputs
	AcControlTask::ReadWeldRecipeFromOD();
	
	AC::servo.servoInit();

	EqepSetMaxCount(LINEAR,0);//zero encoder readings
	EqepSetMaxCount(ROTARY,0);
	ACStateMachine::homed = TRUE;
}
