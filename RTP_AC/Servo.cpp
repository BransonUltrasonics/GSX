/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "Servo.h"
#include "AC.h"
#include "GpioAPI.h"
#include <math.h>
#include "Utils.h"
extern "C"
{
	#include <customSystemCall.h>
}


/**************************************************************************//**
* \brief  - Default constructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Servo::Servo() : m_SpeedMultiplier(HIGH_SPEED_MULTIPLIER){
#ifdef FORCECONSTANT_FILE
	forceConsts.LoadValuesFromFile();
#endif
	forceConsts.LoadValuesFromEeprom();
	expectedOvershoot = DEFAULT_EXPECTED_OVERSHOOT;

}

/**************************************************************************//**
* \brief  - Default destructor
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
Servo::~Servo() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* \brief  - Initializes the servo
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Servo::servoInit() {
	channel = UPCHANNEL;
	EhrPwmSetTbClk(PWMSS_TBCLK_DIVIDER, UPCHANNEL);
	EhrPwmSetTbClk(PWMSS_TBCLK_DIVIDER, DOWNCHANNEL);
	EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, UPCHANNEL);
	EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, DOWNCHANNEL);
	EhrPwmSetTbAQConfB(0, UPCHANNEL);
	EhrPwmSetTbAQConfA(0, UPCHANNEL);
	EhrPwmSetTbAQConfB(0, DOWNCHANNEL);
	EhrPwmSetTbAQConfA(0, DOWNCHANNEL);
	EhrPwmSetTbPWMCfg(2000, 0, UPCHANNEL);
	EhrPwmSetTbPWMCfg(2000, 0, DOWNCHANNEL);
	servoInterruptMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
	servoTaskMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
	
	ACStateMachine::moveDone = true;
}


/**************************************************************************//**
* \brief  - Sets the servo to move with a trapezoidal motion profile
*
* \param  - newTarget - The target position of the move in µm
* \param  - acceleration - The acceleration of the trapezoidal move in µm/s²
* \param  - deceleration - The deceleration of the trapezoidal move µm/s²
* \param  - maxSpeed - The maximum speed of the trapezoidal move µm/s²
* \param  - applyForce - control force after move is complete or not
*
* \return  - none
*
******************************************************************************/
void Servo::setTarget(UINT32 newTarget, UINT32 acceleration,
		UINT32 deceleration, UINT32 maxSpeed, bool applyForce) 
{
	semTake(servoInterruptMutex, WAIT_FOREVER);
	semTake(servoTaskMutex, WAIT_FOREVER);
	if (ACStateMachine::moveDone && ((newTarget > (EqepGetPosCount(ROTARY)/ENCODER_RES) + MAX_SERVO_ERROR) 
			|| (newTarget < (EqepGetPosCount(ROTARY)/ENCODER_RES) - MAX_SERVO_ERROR))) 
	{
		forceControl = false;//stop controlling force from previous move until new parameters are set
		moving = false;
		ACStateMachine::moveDone = false;
		moveStart = (EqepGetPosCount(ROTARY)/ENCODER_RES);
		demandPos = moveStart;
		target = newTarget;
		accel = acceleration * RECIPE_UNIT_SCALING;
		decel = deceleration * RECIPE_UNIT_SCALING;
		accelCount = 0;
		decelCount = 0;
		topSpeed = maxSpeed * RECIPE_UNIT_SCALING * SPEED_SCALE_PERCENT / 100;
		instSpeed = MINIMUM_SPEED;
		if (target < moveStart) 
		{
			if(moveStart - target > expectedOvershoot)
				target += expectedOvershoot;
			channel = UPCHANNEL;
			if(applyForce)
			{
				GpioSetVal(SV1_REQ, GPIO_VALUE_HIGH);
			} else
			{
				GpioSetVal(SV1_REQ, GPIO_VALUE_LOW);
			}
			EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, DOWNCHANNEL);
			//calculate the position at which the max speed would be reached and at which to begin slowing down
			// based upon the kinematic equation d = 1/2  * v² / a
			startDecel = (UINT32)(target + ((UINT64)topSpeed * topSpeed / 2 / decel) + (topSpeed / PERIOD_CALCULATIONS_PER_SECOND));
			accelDone = (UINT32)(moveStart - ((UINT64)topSpeed * topSpeed / 2 / accel) - (topSpeed / PERIOD_CALCULATIONS_PER_SECOND));
			// if max speed is never reached make it a triangular motion scaled by ratio of accel to decel
			if (accelDone < startDecel || accelDone < target || startDecel > moveStart) 
			{
				accelDone = moveStart
						- ((moveStart - target) * decel / (accel + decel));
				startDecel = accelDone;
				triangularAccelerating = true;
			} else
				triangularAccelerating = false;
		} else 
		{
			if(target - moveStart > expectedOvershoot)
							target -= expectedOvershoot;
			channel = DOWNCHANNEL;
			GpioSetVal(SV1_REQ, GPIO_VALUE_HIGH);
			EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, UPCHANNEL);
			//calculate the position at which the max speed would be reached and at which to begin slowing down
			// based upon the kinematic equation d = 1/2 * v² / a 
			startDecel = (UINT32)(target - ((UINT64)topSpeed * topSpeed / 2 / decel) - (topSpeed / PERIOD_CALCULATIONS_PER_SECOND));
			accelDone = (UINT32)(moveStart + ((UINT64)topSpeed * topSpeed / 2 / accel) + (topSpeed / PERIOD_CALCULATIONS_PER_SECOND));
			// if max speed is never reached make it a triangular motion scaled by ratio of accel to decel
			if (accelDone > startDecel || accelDone > target || startDecel < moveStart) {
				accelDone = moveStart
						+ ((target - moveStart) * decel / (accel + decel));
				startDecel = accelDone;
				triangularAccelerating = true;
			} else
				triangularAccelerating = false;
		}

		EhrPwmSetTbPWMCfg((UINT32) START_PERIOD, 0, channel);
		forceControl=applyForce;
		errorSum=0;
		moving = true;
		/*If EhrPwmSetTbAQConfA is toggled driver alarm comes out, further test need to be done.*/
//		EhrPwmSetTbAQConfA(TOGGLE_ON_PERIOD, channel);
		EhrPwmSetTbAQConfB(TOGGLE_ON_PERIOD, channel);

	} else if (newTarget != target ) 
	{
		//TODO change target mid move
		if (channel == DOWNCHANNEL && newTarget < ((EqepGetPosCount(ROTARY)/ENCODER_RES))) 
		{
			servoStop();
		}
	}
	
	semGive(servoTaskMutex);
	semGive(servoInterruptMutex);
}


/**************************************************************************//**
* \brief  - Alters the active motion profile to begin decelerating to a stop
*
* \param  - applyForce - true stops trapezoidal motion but still switches to force control mode
*
*
* \return  - none
*
******************************************************************************/
void Servo::servoStop(bool applyForce) {
	semTake(servoInterruptMutex, WAIT_FOREVER);
	semTake(servoTaskMutex, WAIT_FOREVER);
	
	ACStateMachine::moveDone = true;
	EhrPwmSetTbAQConfB(0, DOWNCHANNEL);
	EhrPwmSetTbAQConfA(0, DOWNCHANNEL);
	EhrPwmSetTbAQConfB(0, UPCHANNEL);
	EhrPwmSetTbAQConfA(0, UPCHANNEL);
	
	forceControl = applyForce;
	if(!applyForce)
		GpioSetVal(SV1_REQ, GPIO_VALUE_LOW);
	
	semGive(servoTaskMutex);
	semGive(servoInterruptMutex);
}


/**************************************************************************//**
* \brief  - Calculates the current speed required based upon the motion profile and sets the PWM period accordingly
*
* \brief  - This is called in the time domain every 100µs, but the start and stop points are in the position domain
*
* \brief  - This works like a midpoint riemann sum, but shifted from midpoint to 2/3 point for improved performance
*
* \brief  - Also performs force control after trapezoidal move is completed
* 
* \breif  - Essentially uses closed loop PID control of force to determine whether to send command pulses, how fast, and in which direction
*  
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Servo::calculatePeriod() {
	semTake(servoTaskMutex, WAIT_FOREVER);
	INT32 actualCompression;
	INT32 error, PIDout;
	if (channel == UPCHANNEL)
	{
		if ((EqepGetPosCount(ROTARY)/ENCODER_RES) < target && (!ACStateMachine::moveDone)){
			ACStateMachine::moveDone = true;
//			printf("\nTarget Exceeded!\n");
		}
		if(!ACStateMachine::moveDone)
		{
			demandPos -= instSpeed / (float)PERIOD_CALCULATIONS_PER_SECOND;
			moving = true;
			if(demandPos <= target)
			{
				ACStateMachine::moveDone = true;
			} else
			{
				moving = true;
				if (demandPos >= accelDone) 
				{
					accelCount ++;
					instSpeed = (accel * accelCount / PERIOD_CALCULATIONS_PER_SECOND) + (accel / PERIOD_CALCULATIONS_PER_SECOND  * 2 / 3);
				} else if (demandPos <= startDecel) 
				{
					//top speed target is never met in triangular moves. 
					//Redefine top speed as speed at which the change is made from accelerating to decelerating so it can be used to calculate the speed when decelerating
					if(triangularAccelerating)
					{
						topSpeed = instSpeed;
						triangularAccelerating = false;
					}
					decelCount ++;
					instSpeed = topSpeed - (decel * decelCount / PERIOD_CALCULATIONS_PER_SECOND) - (decel / PERIOD_CALCULATIONS_PER_SECOND  * 2 / 3);
				} else 
				{
					instSpeed = topSpeed;
				}
			}
		}
		else if (forceControl)
		{
			actualCompression = EqepGetPosCount(ROTARY) - EqepGetPosCount(LINEAR);
			error = targetCompression - actualCompression;
			if(error > ROUNDED_ERROR || error < (-1) * ROUNDED_ERROR)
			{
				errorSum += error;
				errorSum = _CHECKMAX(MIN_ERROR_SUM, errorSum);
				errorSum = _CHECKMIN(MAX_ERROR_SUM, errorSum);
			}
			PIDout = round((error * P_CONST) + (errorSum * I_CONST));
			//Proportional + Integral control to determine if change of direction is required
			if (PIDout > ROUNDED_ERROR)
			{
				EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, channel);
				channel = DOWNCHANNEL;
			}
			//proportional bang-bang control of moving or not
			moving = (actualCompression > (INT32)(targetCompression + ROUNDED_ERROR) && (EqepGetPosCount(ROTARY)/ENCODER_RES) > moveStart);
			if(moving)
			{
				//proportional control of pulse rate
				instSpeed = MINIMUM_SPEED - (m_SpeedMultiplier * error);//minus because error is negative
			} 
		}
		else
		{
			moving = false;
		}

	} else { //down
		if ((EqepGetPosCount(ROTARY)/ENCODER_RES) > target && (!ACStateMachine::moveDone))
		{

			ACStateMachine::moveDone = true;
			//				printf("\nTarget Exceeded!\n");
		}
		if (!ACStateMachine::moveDone) {
			demandPos += instSpeed / (float)PERIOD_CALCULATIONS_PER_SECOND;
			moving = true;
			if(demandPos >= target)
			{
				ACStateMachine::moveDone = true;
			} else
			{
				moving = true;
				if (demandPos <= accelDone) {
					accelCount++;
					instSpeed =
							(accel * accelCount / PERIOD_CALCULATIONS_PER_SECOND)
							+ (accel / PERIOD_CALCULATIONS_PER_SECOND * 2/ 3);
				}else if (forceControl && (EqepGetPosCount(ROTARY)
						- EqepGetPosCount(LINEAR)) > (MAX_PART_CONTACT_COMPRESSION * ENCODER_RES)){
					servoStop(forceControl);
				} else if (demandPos >= startDecel) {
					//top speed target is never met in triangular moves. 
					//Redefine top speed as speed at which the change is made from accelerating to decelerating so it can be used to calculate the speed when decelerating
					if(triangularAccelerating)
					{
						topSpeed = instSpeed;
						triangularAccelerating = false;
					}
					decelCount++;
					instSpeed = topSpeed - (decel * decelCount / PERIOD_CALCULATIONS_PER_SECOND) - (decel / PERIOD_CALCULATIONS_PER_SECOND  * 2 / 3);
				} else 
				{
					instSpeed = topSpeed;
				}
			}
		}
		else if (forceControl)
		{
			actualCompression = EqepGetPosCount(ROTARY)
					- EqepGetPosCount(LINEAR);
			error = targetCompression - actualCompression;
			if(error > ROUNDED_ERROR || error < (-1) * ROUNDED_ERROR)
			{
				errorSum += error;
				errorSum = _CHECKMAX(MIN_ERROR_SUM, errorSum);
				errorSum = _CHECKMIN(MAX_ERROR_SUM, errorSum);
			}
			PIDout = round((error * P_CONST) + (errorSum * I_CONST));
			//Proportional + Integral control to determine if change of direction is required
			if (PIDout < (-1) * ROUNDED_ERROR) {
				EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, channel);
				channel = UPCHANNEL;
			}
			//proportional bang-bang control of moving or not
			moving = (actualCompression < (INT32)(targetCompression - ROUNDED_ERROR) && (EqepGetPosCount(ROTARY)/ENCODER_RES) < FULL_STROKE);
			if (moving) {
				//proportional control of pulse rate
				instSpeed = MINIMUM_SPEED +  (m_SpeedMultiplier * error);
				
			}
		}
		else
		{
			moving = false;
		}
		
	}
	
	if (instSpeed < MINIMUM_SPEED) instSpeed = MINIMUM_SPEED;
	if (instSpeed > topSpeed) instSpeed = topSpeed;
	
	period = (TICS_PER_SECOND / instSpeed / PULSES_PER_MICRON) - 1;
	
	EhrPwmSetTbPWMCfg((UINT32) period, 0, channel);
	if(moving)
	{
		//Clears opposite channel before toggling actual channel
		if(channel == DOWNCHANNEL)
			EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, UPCHANNEL);
		else
			EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, DOWNCHANNEL);
		
		/*If EhrPwmSetTbAQConfA is toggled driver alarm comes out, further test need to be done.*/
//		EhrPwmSetTbAQConfA(TOGGLE_ON_PERIOD, channel);
		EhrPwmSetTbAQConfB(TOGGLE_ON_PERIOD, channel);
	} else
	{
	//Clears both channels when the moving is done 
		EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, DOWNCHANNEL);
		EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL, UPCHANNEL);
		
		EhrPwmSetTbAQConfB(0, DOWNCHANNEL);
		EhrPwmSetTbAQConfA(0, DOWNCHANNEL);
		EhrPwmSetTbAQConfB(0, UPCHANNEL);
		EhrPwmSetTbAQConfA(0, UPCHANNEL);
	}

	semGive(servoTaskMutex);
}


/**************************************************************************//**
* \brief  - Calculates the current force
*
* \param  - none
*
*
* \return  - current force based upon spring compression
*
******************************************************************************/
UINT16 Servo::CalculateForce()
{
	INT32 calc;
	if ((EqepGetPosCount(LINEAR) / ENCODER_RES) < MIN_STROKE)
	{
		calc = 0;
	} else
	{
		calc = round(((EqepGetPosCount(ROTARY) - EqepGetPosCount(LINEAR)) 
			/ ENCODER_RES * AC::servo.forceConsts.GetForceConstant(1)) + (AC::servo.forceConsts.GetForceConstant(0)));
	}
	if(0 > calc)  
		calc=0;
	if(MAX_FORCE < calc)
		calc = MAX_FORCE;
	return (UINT16)calc;
}



/**************************************************************************//**
* \brief  - Sets targetCompression
*
* \param  - compression - new target value
*
*
* \return  - none
*
******************************************************************************/
void Servo::setTargetCompression(INT32 compression)
{
	if(targetCompression != compression * ENCODER_RES)
	{
		errorSum = 0;
		targetCompression = compression * ENCODER_RES;
	}
}


/**************************************************************************//**
* \brief  - sets forceControl
*
* \param  - onOff - new value
*
*
* \return  - none
*
******************************************************************************/
void Servo::setForceControl(bool onOff)
{
	if(onOff)
	{//make sure the ISR is registered and SV1_REQ on if turning on force control
		GpioSetVal(SV1_REQ, GPIO_VALUE_HIGH);
	} 
	else if(ACStateMachine::moveDone) //actively running force control
	{
		if (moving) //update counter for final pulse since the interrupt for that pulse has been disabled
		{
			EhrPwmSetTbAQConfB(0, DOWNCHANNEL);
			EhrPwmSetTbAQConfA(0, DOWNCHANNEL);
			EhrPwmSetTbAQConfB(0, UPCHANNEL);
			EhrPwmSetTbAQConfA(0, UPCHANNEL);
			moving=false;
		}
	}
	forceControl = onOff;
}


/**************************************************************************//**
* \brief  - gets forceControl
*
* \param  - none
*
*
* \return  - forceControl
*
******************************************************************************/
bool Servo::getForceControl()
{
	return forceControl;
}


/**************************************************************************//**
* \brief  - Toggles the up channel output if it is the active channel and no move is active
* 
* \brief  - If a move is active causes it to stop
* 
* \brief  - If active channel is down, switches it to up
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Servo::toggleUp() 
{
	forceControl=false;
	if(false == ACStateMachine::moveDone)
	{
		servoStop();
	} else if (DOWNCHANNEL == channel)
	{
		EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, channel);
		channel = UPCHANNEL;
	} else if((EqepGetPosCount(ROTARY)/ENCODER_RES) > ROUNDED_ERROR)
	{
		EhrPwmSetAQSFRC(AQSFRC_TOGGLE_CHANNEL_NOW, channel);
	}
}


/**************************************************************************//**
* \brief  - Toggles the down channel output if it is the active channel and no move is active
* 
* \brief  - If a move is active causes it to stop
* 
* \brief  - If active channel is up, switches it to down
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void Servo::toggleDown() 
{
	forceControl=false;
	if(false == ACStateMachine::moveDone)
	{
		servoStop();
	} else if (UPCHANNEL == channel)
	{
		EhrPwmSetAQSFRC(AQSFRC_CLEAR_CHANNEL_NOW, channel);
		channel = DOWNCHANNEL;
	} else if ((EqepGetPosCount(ROTARY)/ENCODER_RES) < (FULL_STROKE - ROUNDED_ERROR))
	{
		EhrPwmSetAQSFRC(AQSFRC_TOGGLE_CHANNEL_NOW, channel);
	}
}

/**************************************************************************//**
* \brief  - returns proportional gain factor based on user selection. 
*
* \param  - none
*
*
* \return - float m_SpeedMultiplier
*
******************************************************************************/
void Servo::setSpeedMultiplier(UINT16 a_WeldForceControl)
{
	switch((WELDFORCECONTROL)a_WeldForceControl)
	{
		case CONTROL_LOW:
			m_SpeedMultiplier = LOW_SPEED_MULTIPLIER;
			break;
	
		case CONTROL_MEDIUM:
			m_SpeedMultiplier = MEDIUM_SPEED_MULTIPLIER;
			break;
	
		default:
			m_SpeedMultiplier = HIGH_SPEED_MULTIPLIER;
			break;
	}
}

/**************************************************************************//**
* \brief  - Gets the stack location depending of where we are supposed to be
*
* \param  - bool bStackReturnedFlag: Determines if the stack was returned to
* 									 home position because error, this way the
* 									 function will expect it to be at home.
*
* \return  - STACK_POSITION iStackPosition: Contains the stack position
* 
******************************************************************************/
STACK_POSITION Servo::WhereIsStack(bool bStackReturnedFlag)
{	
	STACK_POSITION iStackPosition = NOT_HOMED;
	
	if(ACStateMachine::homed)
	{
		if((ObjectDictionary::AcWeldRecipe.ReadyPositionToggle) && (!bStackReturnedFlag)) //Where we are supposed to be
		{
			if(EqepGetPosCount(ROTARY)/ENCODER_RES > (INT32)((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset) + (MAX_SERVO_ERROR/10)))
			{
				iStackPosition = BELLOW_READY;
			}
			else if(EqepGetPosCount(ROTARY)/ENCODER_RES < (INT32)((ObjectDictionary::AcWeldRecipe.ExpectedPartContactPosition - ObjectDictionary::AcWeldRecipe.ReadyOffset) - (MAX_SERVO_ERROR)))
			{
				iStackPosition = ABOVE_READY;
			}
			else
			{
				iStackPosition = READY;
			}
		}
		else 
		{
			if(EqepGetPosCount(ROTARY) > MAX_SERVO_ERROR)
			{
				iStackPosition = BELLOW_HOME;
			}
			else
			{
				iStackPosition = HOME;
			}
		}
	}
	
	return iStackPosition;
}
