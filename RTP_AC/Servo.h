/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SERVO_H_
#define SERVO_H_
#include "ACStateMachine.h"
#include <vxworks.h>

///#include <types.h>
///#include <vxbEhrPwmLib.h>
#include "ForceConstants.h"

#define MOTION_DELAY 200 //wait this many cycles after commanded motion finishes to allow the system to settle
#define TICS_PER_SECOND 10000000
#define PERIOD_CALCULATIONS_PER_SECOND 10000
#define PULSES_PER_MICRON 2
#define UPCHANNEL 4
#define DOWNCHANNEL 3
#define MILI_TO_MICRO 1000
#define SPEED_SCALE_PERCENT 110 //used to adjust for minor discrepency in PWM frequency
#define MINIMUM_SPEED 1000
//#define ERROR_SPEED_MULTIPLIER 5.0 / PULSES_PER_MICRON
#define LOW_SPEED_MULTIPLIER 	(3.0 / PULSES_PER_MICRON)
#define MEDIUM_SPEED_MULTIPLIER (6.0 / PULSES_PER_MICRON)
#define HIGH_SPEED_MULTIPLIER 	(10.0 / PULSES_PER_MICRON)
#define START_PERIOD ((TICS_PER_SECOND / MINIMUM_SPEED / PULSES_PER_MICRON) - 1)
#define MIN_ERROR_SUM -1000000000
#define MAX_ERROR_SUM 1000000000
#define RECIPE_UNIT_SCALING 1000
#define MOTION_DELAY_TIME 10 //ms
#define DEFAULT_EXPECTED_OVERSHOOT 100

typedef enum {
	NOT_HOMED,
	HOME,
	BELLOW_HOME,
	ABOVE_READY,
	READY,
	BELLOW_READY
}STACK_POSITION;

//used to set the channel to an idle state so pulses can be read from the other channel
#define AQSFRC_CLEAR_CHANNEL 0x35 

//same as above but immediate instead of on next PWM cycle
#define AQSFRC_CLEAR_CHANNEL_NOW 0xF5 

//toggle the channel output on next PWM cycle
#define AQSFRC_TOGGLE_CHANNEL 0x3F 

//same as above but immediate instead of on next PWM cycle
#define AQSFRC_TOGGLE_CHANNEL_NOW 0xFF 

//free run toggle every cycle
#define TOGGLE_ON_PERIOD 0x000C

#define TBCNT_EQ_ZERO    			0
#define ETPS_INTPRD_DISABLE 		0
#define ETPS_INTPRD_GEN_FIRST_EVT 	1
#define ETPS_INTPRD_GEN_SECOND_EVT 	2
#define ETPS_INTPRD_GEN_THIRD_EVT 	3
#define PWMSS_TBCLK_DIVIDER			10

#define ROUNDED_ERROR ENCODER_RES*PULSES_PER_MICRON*3 //3µm for about +/-1N

#define P_CONST 1.0
#define I_CONST 0.05

/* PROPORTIONAL GAIN FACTOR - LOW, MEDIUM or HIGH_SPEED_MULTIPLIER */
typedef
	enum {CONTROL_LOW = 1, CONTROL_MEDIUM, CONTROL_HIGH}WELDFORCECONTROL;

SEM_ID servoTaskSem;
class Servo {
public:
	Servo();
	virtual ~Servo();
	void servoInit();
	void setTarget(UINT32 newTarget, UINT32 acceleration, UINT32 deceleration, UINT32 maxSpeed, bool applyForce);
	void setForceControl(bool onOff);
	bool getForceControl();
	void setTargetCompression(INT32 compression);
	void servoStop(bool applyforce = false);	
	void calculatePeriod();
	UINT16 CalculateForce();
	void toggleUp();
	void toggleDown();
	ForceConstants forceConsts;	
	void setSpeedMultiplier(UINT16 a_WeldForceControl);
	INT32 expectedOvershoot;
	STACK_POSITION WhereIsStack(bool bStackReturnedFlag);
	
private:
	bool targetUpdated, moveComplete, up, forceControl, moving, triangularAccelerating;
	INT32 target, moveStart, startDecel, accelDone;
	float demandPos;
	INT32 period; 
	UINT32 accel, decel, topSpeed, accelCount, decelCount, instSpeed;
	INT32 errorSum, targetCompression;
	UINT8 channel;
	SEM_ID servoTaskMutex, servoInterruptMutex;
	float m_SpeedMultiplier;
};

#endif /* SERVO_H_ */
