/*
 * WaitForReadyPosition.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#include "WaitForReadyPosition.h"
#include  "ControlTask.h"
#include "IOManager.h"
#include "MainTask.h"
#include "AlarmManager.h"
#include "ACVelocity.h"

/**************************************************************************//**
* 
* \brief   - WaitForReadyPosition Class constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WaitForReadyPosition::WaitForReadyPosition()
{
	stateType = WAIT_FOR_READY_POSITION;
}

/**************************************************************************//**
* 
* \brief   - WaitForReadyPosition Class destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WaitForReadyPosition::~WaitForReadyPosition(){}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering the 
* 			  WaitForReadyPosition state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForReadyPosition::Enter()
{
#ifdef DEBUG
	LOGDBG("Enter WaitForReadyPosition",0,0,0);
#endif
	WaitForReadyPosTimeCounter = 0;
	
	PostWeldSeek = FALSE;
	AfterburstFlag = FALSE;
	if(ControlTask::GetWeldOpFlag() == TRUE)
	{
		AfterburstSetup();
		PostWeldSeek = CP->m_RecipeSC.IsPostWeldSeekEnabled();
	}
	
#ifdef SHUTDOWN_DEBUG
	/*Useful to shutdown testing - power off the system*/
	printf("\n################################### WaitForReadyPosition::Enter : ##############################################\n");
#endif
}

/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us while waiting for the
* 			 AC in the ready state
* 			 both PC and AC in the ready state.
* 			 This function is using multiple returns to resolve digital inputs
* 			 lines status as quick and in line. The purpose is to resolve to a fault
* 			 thus showing the return statements rather than them being hidden
* 			 in a sub-routine call. This gives faster resolution and execution and simplifies
* 			 the flow of the code.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void  WaitForReadyPosition::Loop()
{
	ACVelocity::CalculateVelocity();
	
	/* Accumulates only 100 milliseconds signature data after hold */
	if(WELD == ControlTask::GetOperationType() and (WaitForReadyPosTimeCounter % 4 == 0) and (WaitForReadyPosTimeCounter <= WELD_DATA_POINTS_BAND_US))
	{
		CP->m_TimeCounter = CP->m_TimeCounter + 1;		/* static counter for keep track of total weld time */
		
		if (CP->ind_CollectGraphData==true)
			CaptureWeldData();
	}
	else if((HORN_RETRACT == ControlTask::GetOperationType()) || (HORN_DOWN_CLAMP_OFF == ControlTask::GetOperationType()))
	{
		if(0 == (WaitForReadyPosTimeCounter % INTERVAL_INDICATION))
		{
			UpdateHornDown();
		}
	}

	WaitForReadyPosTimeCounter = WaitForReadyPosTimeCounter + 1;
	
	if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	else if(AC_TX->ACState == AC_ALARM)
	{
		ProcessAlarm (ALARM_AC);
	}
	else
	{
		// Note: this call keeps a status that can be checked later
		DigitalIOMgr::ReadInputs ();

		if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
		{
			// ignore INPUT_DISABLE process input chain here
			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
			{
				ProcessAlarm (ALARM_SC, ALARM_CYCLE_ABORT_NCA);
				return;
			}

			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_SONIC_DISABLE) == DigitalIO::INPUT_ASSERTED)
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_DISABLE_WRA);
		}

		ProcessLoop(ControlTask::GetOperationType());
	}
}

/**************************************************************************//**
* 
* \brief   - This method will be use to compartmentalize repeating 
* 			  loop code.
*
* \param   - cmdID - command to process.
*
* \return  - None.
*
******************************************************************************/
void WaitForReadyPosition::ProcessLoop (INT32 cmdID)
{	
	if(TRUE == AfterburstFlag)
	{
		switch(AfterburstState)
		{
			case AFTERBURST_SONICS_DELAY:
				AfterburstSonicsDelay();
				break;

			case AFTERBURST_SONICS_ON:
				AfterburstSonicsON();
				break;

			case AFTERBURST_SONICS_TIME:
				AfterburstSonicsTime();
				break;

			case AFTERBURST_SONICS_OFF:
				AfterburstSonicsOFF();
				break;

			case AFTERBURST_FINISH:
				AfterburstFinish();
				break;

			default:
				break;
		}
	}
	else if(AC_TX->ACState == AC_READY)
	{
		if(PC_TX->PCState == PC_ALARM)
		{
			AlarmMgr::EnterAlarmEvent(ALARM_PC);
		}

		if(TRUE == PostWeldSeek) 
		{
			ControlTask::SetResultData(true);	/* To collect weld/setup results */
			ControlTask::SetPostWeldDoneFlag(TRUE);
			ChangeState (SEEK_SONIC_ON);
			return;
		}
		else
		{
			ControlTask::SetResultData(true);	/* To collect weld/setup results */
			if (AlarmMgr::IsAlarmState())
				ChangeState(SC_ALARM);
			else
				ChangeState(PREREADY);
			return;
		}
	}
	else if(WaitForReadyPosTimeCounter >= (WAIT_FOR_RDY_POS_TIME_IN_US/ECAT_CYCLE_TIME_IN_US))
	{
		ProcessAlarm (ALARM_SC, ALARM_READY_POSITION_TIMEOUT_NCA);
		return;
	}	
}

void WaitForReadyPosition::AfterburstSetup()
{
	AfterburstTimeCounter = 0;
	AfterburstDelayCounter = 0;
		
	AfterburstFlag      = CP->m_RecipeSC.GetAfterBurst().bIsEnabled;
	AfterburstAmplitude = CP->m_RecipeSC.GetAfterBurst().BurstAmplitude;
	AfterburstTime      = CP->m_RecipeSC.GetAfterBurst().BurstTime;
	AfterburstDelay     = CP->m_RecipeSC.GetAfterBurst().BurstDelay;
		
	if(AfterburstTime>0)
	{
		AfterburstTime=((AfterburstTime*1000)/ECAT_CYCLE_TIME_IN_US);
		if(AfterburstTime > MAX_AFTERBURST_TIME_COUNTER)
			AfterburstTime = MAX_AFTERBURST_TIME_COUNTER;
	}
	else
	{
		AfterburstTime=0;
	}
	
	if(AfterburstDelay>0)
	{
		AfterburstDelay=((AfterburstDelay*1000)/ECAT_CYCLE_TIME_IN_US);
		if(AfterburstDelay > MAX_AFTERBURST_DELAY_COUNTER)			
			AfterburstDelay = MAX_AFTERBURST_DELAY_COUNTER;			
	}
	else
	{
		AfterburstDelay=0;
	}	
	
	LOGDBG("WFR: AfterBurst.bIsEnabled    	: %d\n",AfterburstFlag,0,0);
	LOGDBG("WFR: AfterBurst.BurstAmplitude	: %d\n",AfterburstAmplitude,0,0);
	LOGDBG("WFR: AfterBurst.BurstDelay    	: %d\n",AfterburstDelay,0,0);
	LOGDBG("WFR: AfterBurst.BurstTime     	: %d\n",AfterburstTime,0,0);
		
	AfterburstState = AFTERBURST_SONICS_DELAY;
}

void WaitForReadyPosition::AfterburstSonicsDelay()
{
	AfterburstDelayCounter = AfterburstDelayCounter + 1;
	if(AfterburstDelayCounter >= AfterburstDelay)
	{
		#ifdef DEBUG
			printf("\n Afterburst Delay Exit \n");
		#endif
		AfterburstState = AFTERBURST_SONICS_ON;
	}	
}

void WaitForReadyPosition::AfterburstSonicsON()
{
	//TOD: Enable correctly for IO use case
	CheckSonicsON();
	PC_RX->Amplitudechange = AfterburstAmplitude; // set the afterburst amplitude
	PC_RX->MasterEvents |= (UINT16) BIT_MASK(CTRL_AFTERBURST_ENABLE);
	#ifdef DEBUG
		printf("\n Afterburst ON Send to PC \n");
	#endif
	
		AfterburstState = AFTERBURST_SONICS_TIME;
}

void WaitForReadyPosition::AfterburstSonicsTime()
{
	AfterburstTimeCounter = AfterburstTimeCounter + 1;
	if(AfterburstTimeCounter>=AfterburstTime)
	{
		#ifdef DEBUG
			printf("\n Afterburst OFF Send to PC \n");
		#endif
		AfterburstState = AFTERBURST_SONICS_OFF;
	}
}

void WaitForReadyPosition::AfterburstSonicsOFF()
{
	PC_RX->MasterEvents &= ~(UINT16) BIT_MASK(CTRL_AFTERBURST_ENABLE);
	AfterburstState = AFTERBURST_FINISH;

}

void WaitForReadyPosition::AfterburstFinish()
{
	AfterburstFlag = FALSE;
}

/*******************************************************************************
*      Method Name :   	CheckSonicsON
*      Description :   	Check before enter sonic's mode 
*      param       :   	void
*      return      :   	void
*   
********************************************************************************/
void WaitForReadyPosition::CheckSonicsON()
{
	if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED
		&& 
		DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_SONIC_DISABLE) == DigitalIO::INPUT_ASSERTED)
	{
		PC_RX->Amplitudechange = 0;
		PC_RX->MasterEvents |=  (UINT16) BIT_MASK(CTRL_PC_SONIC_DISABLE);
	}
	else
	{
		PC_RX->Amplitudechange = 0;		
		PC_RX->MasterEvents &=  (UINT16) ~BIT_MASK(CTRL_PC_SONIC_DISABLE);
	}	
}


/**************************************************************************//**
* \brief   -  	Captures weld signature data at every 250 us and insert into
* 				std::deque called m_qWeldSignature.
*
* \param   -	None
*
* \return  -    None 
*
******************************************************************************/
void WaitForReadyPosition::CaptureWeldData()
{
	WeldSignature weldSignature;

	weldSignature.AbsoluteDistance 	= AC_TX->ActualDistance;
	weldSignature.Amplitude			= PC_TX->Amplitude;
	weldSignature.CollapseDistance	= 0;
	weldSignature.Current			= PC_TX->Current/100;
	weldSignature.Energy			= 0;
	weldSignature.Force				= AC_TX->ActualForce;
	weldSignature.Frequency			= PC_TX->Frequency/10;
	weldSignature.Phase				= 0;
	weldSignature.Power				= PC_TX->Power;
	weldSignature.Time				= CP->m_TimeCounter;
	if(0 > ACVelocity::GetVelocity()) //TODO: remove this check. This is a workaround because velocity is being graphed as unsigned
	{
		weldSignature.Velocity 	=  (-1) * ACVelocity::GetVelocity();
	} else
	{
		weldSignature.Velocity 	= ACVelocity::GetVelocity();
	}
	
	CP->m_qWeldSignature.push_back(weldSignature);
}

/**************************************************************************//**
* \brief   -  	Captures horn actuator data at every 500 ms and sends to UIC 
* 				
* \param   -	None
*
* \return  -    None 
*
******************************************************************************/
void WaitForReadyPosition::UpdateHornDown()
{
	ControlTask::SetActuatorSetupData(ACTUAL_FORCE,AC_TX->ActualForce);
	ControlTask::SetActuatorSetupData(ACTUAL_DISTANCE,AC_TX->ActualDistance);
	ControlTask::m_bIsIndicateUIC = true;								/* Sends actual distance to UIC */
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when exiting the WaitForReadyPosition state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForReadyPosition::Exit()
{
#ifdef DEBUG
	LOGDBG("Exit WaitForReadyPosition",0,0,0);
#endif
	
	if(WELD == ControlTask::GetOperationType())
	{
		CommonProperty::weldResult.Set(WeldResults::CYCLE_TIME, (getCycleCounter() * ECAT_CYCLE_TIME_IN_US));
		CP->ShutdownWeldResult.CycleTime = CommonProperty::weldResult.GetWeldData().CycleTime;
#ifdef SHUTDOWN_DEBUG
		printf("\n################################### WaitForReadyPosition::Exit : Weld Completed ##############################################\n");
#endif	
	}

	if((HORN_RETRACT == ControlTask::GetOperationType()) || (HORN_DOWN_CLAMP_OFF == ControlTask::GetOperationType()))
	{
		UpdateHornDown();
	}
	
}

