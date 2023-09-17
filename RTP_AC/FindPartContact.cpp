#include <stdio.h>
#include "FindPartContact.h"
#include "ACStateMachine.h"
#include "Servo.h"
#include "AC.h"
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
FindPartContact::FindPartContact() {	
	stateType = FIND_PART_CONTACT;
	bFirstPartContact = false;
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
FindPartContact::~FindPartContact() {
}


/**************************************************************************//**
* \brief  - State entry code
* \brief  - Start slow full stroke move
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void FindPartContact::Enter() {
	start = EqepGetPosCount(LINEAR);
	AC::servo.setTarget(MAX_PART_CONTACT,DEFAULT_ACCELERATION,DEFAULT_ACCELERATION,SEARCH_SPEED,false);
	top=0;
	bottom=0;
	ACStateMachine::partContactFound = FALSE;
	
	//The expected overshoot is set to 0 after the first part contact is made to reset the correction calculated for
	//the first actuator setup, this only has to be done once as in further contacts/welds the overshoot is recalculated.
	if(bFirstPartContact) 
	{
		AC::servo.expectedOvershoot = 0;
		bFirstPartContact = false;
	}
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - Monitor for part contact. Stop servo when contact detected.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void FindPartContact::Loop() 
{
	INT32 topLast, bottomLast, springCompression;
	if (ACStateMachine::alarm) {
		ChangeState(AC_ALARM);

	} else if (ObjectDictionary::rpdo.SC_State != WAIT_FOR_PART_CONTACT) {
		ChangeState(RETURN_STROKE);
	} else {

		
		topLast = top;
		bottomLast = bottom;
	
		top = EqepGetPosCount(ROTARY);
	
		bottom = EqepGetPosCount(LINEAR);
		springCompression = top - bottom;
		//Look for part contact
		if(!ACStateMachine::partContactFound){
			if(((top - topLast) > (PART_CONTACT_RATIO * (bottom - bottomLast)) && bottom > (start + (ACCEL_WINDOW* ENCODER_RES))) ||
					(springCompression > (MAX_PART_CONTACT_COMPRESSION * ENCODER_RES))) {
				ACStateMachine::partContactFound = TRUE;
				ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(PART_CONTACT_FOUND); //tell SC we found it
				AC::servo.servoStop();
				ACStateMachine::foundPartContactPos = bottom / ENCODER_RES;
			}
			//TODO: what if move done and still no contact?
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
void FindPartContact::Exit() {
	ACStateMachine::partContactFound = false;
	ObjectDictionary::tpdo.ACStatusEvent &=~ BIT_MASK(PART_CONTACT_FOUND); //clear part contact flag;
	
	if (ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition == 0)
		bFirstPartContact = true;

	
}
