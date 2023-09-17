#include "ACStateMachine.h"
#include "AC.h"
#include "GpioAPI.h"
#include "ServoReset.h"
extern "C"
{
	#include <customSystemCall.h>
}

#define MAX_RESET_SIGNAL_TIME 75 * CYCLES_PER_MS

/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ServoReset::ServoReset() {
	stateType = SERVO_RESET;
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
ServoReset::~ServoReset() {
	
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
void ServoReset::Enter() {
	loopCount = 0;
	GpioSetVal(SERVO_AMP_RESET, GPIO_VALUE_HIGH);
}

/**************************************************************************//**
* \brief  - State loop code
* \brief  - Keep reset line high until alarm input cleared or 75ms has elapsed
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ServoReset::Loop() {
	UINT16 alarmIn;
	GpioGetVal(SERVO_AMP_ALARM, &alarmIn);
	if(!alarmIn)
	{
		ChangeState(HOMEING);
	} else if(loopCount++ > MAX_RESET_SIGNAL_TIME) //Servo alarm should have cleared by now, go back to alarm state
	{
		Alarm::SetAlarmReason ((UINT32)ERR_SERVO_MOTOR);
		ChangeState(AC_ALARM);
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
void ServoReset::Exit() {
	GpioSetVal(SERVO_AMP_RESET, GPIO_VALUE_LOW);
}
