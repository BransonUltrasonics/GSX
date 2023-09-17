
#include "JogDown.h"
#include "ControlTask.h"
#include "IOManager.h"
#include "Commons.h"
#include "AlarmManager.h"
#include "MainTask.h"

/*******************************************************************************
*      Method Name :   	
*      Description :   	JogDown Class constructor
*      param       :   	
*      return      :   	
*   
********************************************************************************/
JogDown::JogDown()
{
	stateType = JOG_DOWN;
}

/*******************************************************************************
*      Method Name :   	
*      Description :   	JogDown Class destructor
*      param       :   	
*      return      :   	
*   
********************************************************************************/
JogDown::~JogDown()
{
}

/*******************************************************************************
*      Method Name :   	Enter()
*      Description :   	This method will be executed when entering the JogDown state 
*      param       :   	void
*      return      :   	void
*   
********************************************************************************/
void JogDown::Enter()
{
	m_TimerCount = 0;
}

/*******************************************************************************
*      Method Name :   	Loop()
*      Description :   	This method will be executed every 250us while Actuator Jogging down. 
*      param       :   	void
*      return      :   	void
*   
********************************************************************************/
void JogDown::Loop()
{
	if(AC_TX->ACState == AC_ALARM)
	{
		ProcessAlarm (ALARM_AC);
	}
	else if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	else if(((AC_TX->ACInputs & SS1MASK) | (AC_TX->ACInputs & SS2MASK)) == BOTHSTARTSWITCHMASK)
	{  
		//TODO: add any required user IO checks here

		if(0 == (++m_TimerCount) % INTERVAL_INDICATION)
		{
			ControlTask::SetActuatorSetupData(ACTUAL_FORCE,AC_TX->ActualForce);
			ControlTask::SetActuatorSetupData(ACTUAL_DISTANCE,AC_TX->ActualDistance);
			ControlTask::m_bIsIndicateUIC = true;								/* Sends actual distance to UIC */
		}
	}
	else
	{
		ChangeState(PREREADY);
	}
}


/*******************************************************************************
*      Method Name :   	Exit()
*      Description :   	This method will be executed when exiting the JogDown state 
*      param       :   	void
*      return      :   	void
*   
********************************************************************************/
void JogDown::Exit()
{
	ControlTask::SetActuatorSetupData(ACTUAL_FORCE,AC_TX->ActualForce);
	ControlTask::SetActuatorSetupData(ACTUAL_DISTANCE,AC_TX->ActualDistance);
	ControlTask::m_bIsIndicateUIC = true;	
}


