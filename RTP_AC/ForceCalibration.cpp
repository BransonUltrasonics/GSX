#include <stdio.h>
#include "ForceCalibration.h"
#include "ACStateMachine.h"
#include "Servo.h"
#include "AC.h"
#include "AcControlTask.h"
extern "C"
{
	#include <customSystemCall.h>
}

#define FIRST_POINT_COMPRESSION 250 //0.25mm
#define SECOND_POINT_COMPRESSION 2500 //2.5mm


/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
ForceCalibration::ForceCalibration() {	
	stateType = FORCE_CALIBRATION;
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
ForceCalibration::~ForceCalibration() {
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
void ForceCalibration::Enter() {
	AC::servo.setTarget(FULL_STROKE,DEFAULT_ACCELERATION,DEFAULT_ACCELERATION,SEARCH_SPEED,true);
	AC::servo.setTargetCompression(FIRST_POINT_COMPRESSION);
	top=0;
	bottom=0;
	start = EqepGetPosCount(LINEAR);
	ACStateMachine::partContactFound = FALSE;
	ObjectDictionary::tpdo.ACStatusEvent &=~ BIT_MASK(PART_CONTACT_FOUND);
	
#ifdef DEBUG
	printf("Enter ForceCalibration\n");	
#endif
}


/**************************************************************************//**
* \brief  - State loop code
* \brief  - Monitor for part contact. Stop servo when contact detected.
* \brief  - Drive the servo to specific spring compression distances for calibration
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ForceCalibration::Loop() 
{
	INT32 topLast, bottomLast, springCompression;
	if (ACStateMachine::alarm) {
		ChangeState(AC_ALARM);

	} else if ((ObjectDictionary::rpdo.SC_State != CALIBRATION_FIRST_POINT) 
			&& (ObjectDictionary::rpdo.SC_State != CALIBRATION_SECOND_POINT)) {
		AC::servo.servoStop();
		ChangeState(RETURN_STROKE);
	} else {

		
		topLast = top;
		bottomLast = bottom;
	
		top = EqepGetPosCount(ROTARY);
	
		bottom = EqepGetPosCount(LINEAR);
		springCompression = top - bottom;
		//Look for part contact
		if(!ACStateMachine::partContactFound){
			if(((top - topLast) > (PART_CONTACT_RATIO * (bottom - bottomLast)) && bottom > (start + (ACCEL_WINDOW * ENCODER_RES))) ||
					(springCompression > (MAX_PART_CONTACT_COMPRESSION * ENCODER_RES))) {
				ACStateMachine::partContactFound = TRUE;
				ObjectDictionary::tpdo.ACStatusEvent |= BIT_MASK(PART_CONTACT_FOUND); //tell SC we found it
				AC::servo.servoStop(true);
			}
		} else if(ObjectDictionary::rpdo.SC_State == CALIBRATION_SECOND_POINT){
			AC::servo.setTargetCompression(SECOND_POINT_COMPRESSION);
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
void ForceCalibration::Exit() {
	AcControlTask acCtl; //TODO: clean this object up to allow static calls instead of making local instances of it
	ACStateMachine::partContactFound = false;
	ObjectDictionary::tpdo.ACStatusEvent &=~ BIT_MASK(PART_CONTACT_FOUND);
	if (SC_ALARM != ObjectDictionary::rpdo.SC_State)
	{
		acCtl.ReadCalibrationReadingsFromOD();
		AC::servo.forceConsts.CalculateTwoPointCalibration(FIRST_POINT_COMPRESSION, ObjectDictionary::calReadings.FirstReading,
				SECOND_POINT_COMPRESSION, ObjectDictionary::calReadings.SecondReading);
	}
}
