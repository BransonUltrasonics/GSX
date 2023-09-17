/*
 * WaitForTrigger.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#include "WaitForTrigger.h"
#include "SC_Common.h"
#include "ControlTask.h"
#include "IOManager.h"
#include "MainTask.h"
#include "Commons.h"
#include "AlarmManager.h"
#include "ACVelocity.h"
#include "Counters.h"

/**************************************************************************//**
* 
* \brief   - WaitForTrigger Class constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WaitForTrigger::WaitForTrigger()
{
	stateType = WAIT_FOR_TRIGGER;
}

/**************************************************************************//**
* 
* \brief   - WaitForTrigger Class destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WaitForTrigger::~WaitForTrigger()
{
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering the WaitForTrigger state .
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Enter()
{
#ifdef DEBUG
	LOGDBG("Enter WaitForTrigger",0,0,0);
#endif
	WaitForTriggerTimeCounter	= 0;
	NewMaxHoldForce				= 0;
	PretriggerActiveFlag		= false;
	CP->m_TimeCounter			= 0;
	CP->ind_ULGraphData 		= false;

	ACVelocity::ClearVelocityBuffer();
	
	message.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_ACTUATIONS;
	SendToMsgQ (CP->GetMsgQId(cTaskName[MAINTENANCE_T]),message);
	
	SDO.TriggerDistance = CP->m_RecipeSC.GetTriggerDistance();
	SDO.TriggerForce 	= CP->m_RecipeSC.GetTriggerForce();
	m_WeldAmplitude		= CP->m_RecipeSC.InitialAmplitude();
	
	/* UPdate the trigger from UI recipe */
	preTrigger 			= CP->m_RecipeSC.GetPreTrigger();	
	PretriggerFlag 		= preTrigger.bIsEnabled;
#ifdef PRETRIGGER_DEBUG
	LOGDBG("PretriggerFlag: %d",PretriggerFlag,0,0);
#endif
	
	if(true == preTrigger.bIsAutoEnabled)
	{
		PretriggerType 	= preTrigger.bIsAutoEnabled;	
	}
	else if(true == preTrigger.bIsDistanceEnabled)
	{
		PretriggerType 	= 2;		
	}	
	PretriggerAmplitude = preTrigger.TriggerAmplitude;
	PretriggerDistance 	= preTrigger.PreTriggerDistance;
	
	if(CP->m_RecipeSC.IsCoolingValveEnabled())
	{
		AC_RX->ACOutputs |= (UINT16)BIT_MASK(CONVERTER_COOLING);
	}
	
	// set output cycle RUN 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_RUN, DigitalIO::LINE_HIGH);

	// set first weld cycle for the alarm manager 
	AlarmMgr::FirstWeldCycle();

	// reset output cycle OK
	AlarmMgr::ClearCycleOK();
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_CYCLE_OK, DigitalIO::LINE_LOW);
	
	// reset outputs process limits
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_REJECT, DigitalIO::LINE_LOW);
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_ALARM_SUSPECT, DigitalIO::LINE_LOW);

	#ifdef PRETRIGGER_DEBUG
	LOGDBG("PretriggerType: %d",PretriggerType,0,0);
	LOGDBG("PretriggerAmplitude: %d",PretriggerAmplitude,0,0);
	LOGDBG("PretriggerDistance: %d",PretriggerDistance,0,0);
#endif
}

/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us while waiting for the trigger.
* 			 both PC and AC in the ready state.
* 			 This function is using multiple returns to resolve digital inputs
* 			 lines status as quick and in line. The purpose is to resolve to a fault
* 			 thus showing the return statements rather than them being hidden
* 			 in a sub-routine call. This gives faster resolution and execution 
* 			 and simplifies the flow of the code.
* 			 
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Loop()
{
	ACVelocity::CalculateVelocity();
	
	if(WELD == ControlTask::GetOperationType() and (WaitForTriggerTimeCounter % 4 == 0))
	{
		CP->m_TimeCounter =	CP->m_TimeCounter + 1;			/* static counter for keep track of total weld time */
		
		if (CP->ind_CollectGraphData==true)
			CaptureWeldData();		/* Accumulate weld signature data */
	}
	WaitForTriggerTimeCounter = WaitForTriggerTimeCounter + 1;
	
	if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	else if(AC_TX->ACState == AC_ALARM)
	{
		ProcessAlarm (ALARM_AC);
	}
	else if(PC_TX->PCState == PC_ALARM)
	{
		ProcessAlarm (ALARM_PC);
	}
	else
	{	
		if(((AC_TX->ACInputs & SS1MASK) | (AC_TX->ACInputs & SS2MASK)) == BOTHSTARTSWITCHMASK)
		{  
			if(true == PretriggerFlag) /* Pre-trigger ON */
			{
					if((AC_TX->ACStatusEvent & BIT_MASK(PART_CONTACT_FOUND)) && (PretriggerActiveFlag==false))  //Checks if part contact found before pretrigger
					{
						ProcessAlarm (ALARM_SC, ALARM_PART_CONTACT_PRETRIGGER_NCA);
					}
					else
					{
				switch(PretriggerType)
				{
					case 0: /* None */
						break;
						
					case 1: /* Auto */
						if(false == PretriggerActiveFlag)
						{
#ifdef PRETRIGGER_DEBUG
							LOGDBG("In Auto trigger type",0,0,0);
#endif
							CheckSonicsON();
							PC_RX->Amplitudechange = PretriggerAmplitude; /* set the pre-trigger amplitude*/
							PC_RX->MasterEvents |= (UINT16) BIT_MASK(CTRL_PRETRIGGER_ENABLE); /* add the event to trigger the sonics */
							PretriggerActiveFlag = true;
						}					    	
						break;
						
					case 2: /* Distance */
						if((AC_TX->ActualDistance >= PretriggerDistance) && (false == PretriggerActiveFlag))
						{
#ifdef PRETRIGGER_DEBUG
							LOGDBG("In Distance trigger type",0,0,0);
#endif
							CheckSonicsON();
							PC_RX->Amplitudechange = PretriggerAmplitude; /* set the pretrigger amplitude */
							PC_RX->MasterEvents |= (UINT16) BIT_MASK(CTRL_PRETRIGGER_ENABLE); /* add the event to trigger the sonics */
#ifdef PRETRIGGER_DEBUG
							LOGDBG("Pre-trigger Distance Send Event by SC",0,0,0);
#endif
							PretriggerActiveFlag = true;
						}
						break;
					}
				}
			}
				if(CP->m_RecipeSC.GetWeldMode() != ABSMODE)
				{		
					if(CP->m_RecipeSC.IsGlobalCutOffLimitCheckEnabled() && CP->m_RecipeSC.IsCutoffLimitCheckEnabled(ABSOLUTE_DISTANCE_CUTOFF))	
					{
						if(CP->m_RecipeSC.ValidateCutoffLimit(ABSOLUTE_DISTANCE_CUTOFF,AC_TX->ActualDistance))
						{
							ProcessAlarm (ALARM_SC, ALARM_ABSOLUTE_DISTANCE_NCA);
							return;
						}
					}
				}
				else if(AC_TX->ActualDistance >= CP->m_RecipeSC.GetModeValue())
				{
					ProcessAlarm (ALARM_SC, ALARM_ABSOLUTE_DISTANCE_NCA);
					return;
				}
				
			if (AC_TX->ActualForce >= SDO.TriggerForce)
			{	
				if(false == PretriggerActiveFlag)
				{
					CheckSonicsON();
				}
				
				PC_RX->Amplitudechange = m_WeldAmplitude; /* set the amplitude */
				
				message.msgID = TO_MAINTENANCE_TASK_CNTRS_INC_WELDS;
				SendToMsgQ (CP->GetMsgQId (cTaskName[MAINTENANCE_T]),message);

				ChangeState(WELD_SONIC_ON);
			}
			else
			{
				DigitalIOMgr::ReadInputs ();
	
				if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
				{
					/* ignore INPUT_DISABLE process input chain here */
						if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
						{
							ProcessAlarm (ALARM_SC, ALARM_CYCLE_ABORT_NCA);
							return;
						}
					
						if (DigitalIOMgr::ScanForInputFunction (DigitalIO::INPUT_PART_PRESENT) &&
										DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PART_PRESENT) != DigitalIO::INPUT_ASSERTED)
						{
							ProcessAlarm (ALARM_SC, ALARM_PART_PRESENT_LOST_NCA);
							return;
						}

						if (AC_TX->ACInputs & GRDDETMASK)
						{
							ProcessAlarm (ALARM_SC, ALARM_GND_DETECT_EFA);
							return;
						}
						
						if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_SONIC_DISABLE) == DigitalIO::INPUT_ASSERTED)
							AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_DISABLE_WRA);
				}
			}
		}
		else
		{
			ProcessAlarm (ALARM_SC, ALARM_START_SWITCH_LOST_EFA);
		}
	}
}

/*******************************************************************************
*
*\brief   - 	Check before enter sonic's mode 
*      
*\param   -   	None
*
*\return  -   	void
*   
********************************************************************************/
void WaitForTrigger::CheckSonicsON()
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
		PC_RX->Amplitudechange = m_WeldAmplitude;		
		PC_RX->MasterEvents &=  (UINT16) ~BIT_MASK(CTRL_PC_SONIC_DISABLE);
	}
}


/**************************************************************************//**
* 
* \brief   - Overrides the base class function to handle weld signature data
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::ProcessAlarm (UINT8 origin, UINT16 alarmID)
{	
	/* Truncates accumulated signature data, Case of alarm raised without even meeting trigger point during weld */
	if(WELD == ControlTask::GetOperationType())
	{
		deque<WeldSignature>::iterator startPoint = (CP->m_qWeldSignature.end() - CP->m_TimeCounter); 
		deque<WeldSignature>::iterator endPoint   = CP->m_qWeldSignature.end();
		CP->m_qWeldSignature.erase(startPoint,endPoint);
	}
	
	AlarmMgr::EnterAlarmEvent (origin, alarmID);
 	ChangeState(SC_ALARM); 
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
void WaitForTrigger::CaptureWeldData()
{
	WeldSignature weldSignature;

	weldSignature.AbsoluteDistance 	= AC_TX->ActualDistance;
	weldSignature.Amplitude			= PC_TX->Amplitude;
	weldSignature.CollapseDistance	= 0;
	weldSignature.Current			= PC_TX->Current/100;
	weldSignature.Energy			= 0;
	weldSignature.Force				= AC_TX->ActualForce;
	
	if (CP->stackRecipe.GetData(StackRecipe::END_OF_WELD_STORE) == 1)
		weldSignature.Frequency			= PC_TX->Frequency/10; //This is the last frequency sent from the PC
	else
		weldSignature.Frequency			= CP->stackRecipe.GetData(StackRecipe::MEMORY)/10;
		
	weldSignature.Phase				= 0;
	weldSignature.Power				= PC_TX->Power;
	weldSignature.Time				= CP->m_TimeCounter;
	if(0 > ACVelocity::GetVelocity()) //TODO: remove this check. This is a workaround because velocity is being graphed as unsigned
	{
		weldSignature.Velocity 	= 0;
	} else
	{
		weldSignature.Velocity 	= ACVelocity::GetVelocity();
	}

	CP->m_qWeldSignature.push_back(weldSignature);
}


/**************************************************************************//**
* 
* \brief   - This method will be executed when exiting the WaitForTrigger state .
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WaitForTrigger::Exit()
{
#ifdef DEBUG
	LOGDBG("Exit WaitForTrigger",0,0,0);
#endif
}


