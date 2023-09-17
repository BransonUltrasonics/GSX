
#include "ACStateMachine.h"
#include "Servo.h"
#include "AC.h"
#include <math.h>
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
JoggingUp::JoggingUp() {
	stateType = JOGGING_UP;
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
JoggingUp::~JoggingUp() {

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
void JoggingUp::Enter() {
	
}

/**************************************************************************//**
* \brief  - State loop code
* \brief  - Move one pulse at a time
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void JoggingUp::Loop() {
	ObjectDictionary::tpdo.ActualForce = AC::servo.CalculateForce();
	if (true == ACStateMachine::alarm) 
	{
		ChangeState(AC_ALARM);
	} else if (JOG_UP != ObjectDictionary::rpdo.SC_State) {				

		ChangeState(AC_READY);
	}else
	{
		AC::servo.toggleUp();	
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
void JoggingUp::Exit() {

}
