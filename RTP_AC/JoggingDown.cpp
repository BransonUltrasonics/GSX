
#include "ACStateMachine.h"
#include "Servo.h"
#include "AC.h"
#include <math.h>
#include "GpioAPI.h"
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
JoggingDown::JoggingDown() {
	stateType = JOGGING_DOWN;
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
JoggingDown::~JoggingDown() {
	
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
void JoggingDown::Enter() {
	GpioSetVal(SV1_REQ, GPIO_VALUE_HIGH);
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
void JoggingDown::Loop() {
	ObjectDictionary::tpdo.ActualForce = AC::servo.CalculateForce();
	if (true == ACStateMachine::alarm) 
	{
		ChangeState(AC_ALARM);
	} else if (JOG_DOWN != ObjectDictionary::rpdo.SC_State) {				

		ChangeState(AC_READY);
	}else if (ObjectDictionary::tpdo.ActualForce < ObjectDictionary::AcWeldRecipe.ForceStepForce[0])
	{
		AC::servo.toggleDown();	
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
void JoggingDown::Exit() {
	GpioSetVal(SV1_REQ, GPIO_VALUE_LOW);
}
