/**
 * Alarm.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#include "Alarm.h"
#include  "SCStateMachine.h"
#include  "SC_Common.h"
#include  "SC.h"
#include  "ControlTask.h"
#include  "MainTask.h"
#include "AlarmManager.h"

static 	Message message;
static bool reset_asserted;
static bool alarms_reset;


/**************************************************************************//**
* \brief   - Alarm Class constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
Alarm::Alarm()
{
	stateType = SC_ALARM;
}

/**************************************************************************//**
* \brief   - Alarm Class destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
Alarm::~Alarm()
{
}

/**************************************************************************//**
* \brief   - This method will be executed when entering the Alarm state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Enter()
{

#ifndef DEBUG
	printf("\nEnter alarm\n");	
#endif

	if(WELD == ControlTask::GetOperationType())
	{
		CommonProperty::weldResult.Set(WeldResults::CYCLE_TIME, (getCycleCounter() * ECAT_CYCLE_TIME_IN_US));
	}
	
	ControlTask::SetOperationType(ALARM);
	ControlTask::SetResultData(true);
	bIsAlarmReset = true;
	reset_asserted = false;
	alarms_reset = false;
	CP->ind_ULGraphData	= true;
	moveDoneDelay = AC_ACTIVE_MOVE_DELAY_IN_US/ECAT_CYCLE_TIME_IN_US;

	memset(message.Buffer,0x00,sizeof(message.Buffer));
	message.msgID = 0;
	
	//reset cooling valve on alarm condition
	AC_RX->ACOutputs &= (UINT8)(~BIT_MASK(CONVERTER_COOLING));
	
	// reset output for cycle RUN 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_RUN, DigitalIO::LINE_LOW);
}

/**************************************************************************//**
* \brief   - This method will be executed every 250us while waiting for the.
* 			 reset alarm signal
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Loop()
{
	if(ControlTask::GetOperationType() == RESET_ALARM)	
	{
		ChangeState(PREREADY);
	}
	else
	{	
		// enter alarms into the system
		if(AC_TX->ACState == AC_ESTOP)
		{
			AlarmMgr::EnterAlarmEvent (ALARM_AC, ALARM_ESTOP_NCA);
		}
		else
		{
			AlarmMgr::ExitPersistAlarmEvent (ALARM_AC, ALARM_ESTOP_NCA);
		}
		if(AC_TX->ACState == AC_ALARM)
		{
			AlarmMgr::EnterAlarmEvent (ALARM_AC);
		}
		else
		{
			AlarmMgr::ExitAlarmEvent (ALARM_AC);
		}
		
		if(PC_TX->PCState == PC_ALARM)
		{
			AlarmMgr::EnterAlarmEvent(ALARM_PC);
		}
		else
		{
			AlarmMgr::ExitAlarmEvent (ALARM_PC);
		}
		
		if (AlarmMgr::IsAlarmState())
		{
			// Note: this call keeps a status that can be checked later
			DigitalIOMgr::ReadInputs ();
			
			// ignore INPUT_DISABLE process input chain here
			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
			{
				if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_ALARM_RESET) == DigitalIO::INPUT_ASSERTED)
				{
					if (!reset_asserted)
					{
						// send it to the alarm task
						message.msgID = TO_ALARM_TASK_RESET_INPUT;
						msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
						reset_asserted = true;
					}
				}
				else
				{
					reset_asserted = false;
					
					if (AC_TX->ACInputs & GRDDETMASK)
					{
						AlarmMgr::EnterAlarmEvent(ALARM_SC, ALARM_GND_DETECT_EFA);
					}
					if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
					{
						AlarmMgr::EnterAlarmEvent(ALARM_SC, ALARM_CYCLE_ABORT_NCA);
					}
				}
			}
		}
		else
		{
			// Horn STOP bit from AC or half a second has passed since move done
			moveDoneDelay--;
			
			if (!alarms_reset && ((AC_TX->ACStatusEvent & BIT_MASK(PB_RELEASE_SAFE)) || moveDoneDelay == 0))
			{
				// set output for PB RELEASE 
				DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_PB_RELEASE, DigitalIO::LINE_HIGH);

				// let SC leave alarm state
				message.msgID = TO_CTRL_TASK_RESET_ALARM_CMD;
				msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[CTRL_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
				alarms_reset = true;
			}
		}
	}
}


/**************************************************************************//**
* \brief   - This method will be executed when exiting the Alarm state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Alarm::Exit()
{	
	PC_RX->MasterEvents &=  (UINT16) ~BIT_MASK(CTRL_PC_CLEAR_ALARMS);
	AC_RX->ACControlEvent &=  (UINT8) ~BIT_MASK(EVENT_AC_CLEAR_ALARMS);
	
#ifndef DEBUG
	printf("\nExit alarm\n");	
#endif
}
