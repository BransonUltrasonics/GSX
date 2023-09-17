/*
 * WeldSonicOn.cpp
 *
 *  Created on: May 31, 2017
 *      Author: lzhu
 */

#include "WeldSonicOn.h"
#include "ControlTask.h"
#include "IOManager.h"
#include "MainTask.h"
#include "Commons.h"
#include "AlarmManager.h"
#include "ACVelocity.h"

#ifdef SONICS_LED_GPIO_TEST
extern "C"
{
#include <customSystemCall.h>
}
#endif 

#define GPIO_DIR_INPUT          (0)
#define GPIO_DIR_OUTPUT         (1)

#define GPIO_VALUE_LOW          (0)
#define GPIO_VALUE_HIGH         (1)
#define GPIO_6_7                0xA7


/**************************************************************************//**
* 
* \brief   - WeldSonicOn Class constructor. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldSonicOn::WeldSonicOn()
{
	stateType = WELD_SONIC_ON;
}

/**************************************************************************//**
* 
* \brief   - WeldSonicOn Class destructor. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldSonicOn::~WeldSonicOn()
{
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering the Sonic on state. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::Enter()
{
	INT32 Freqoffset;
	sonicsDelayCounter		= 0;
	TriggerLostCounter		= 0;
	SonicsOnCounter			= 0;
	NewMaxWeldForce 		= 0;
	AccumulatedEnergy		= 0;
	CommonProperty::weldResult.Set(WeldResults::TRIGGER_DISTANCE, AC_TX->ActualDistance);
	EnergyinMilliJoules 	= 0;
	currentForceStep 		= 0;
	StepCounter 			= 1;			/* Skip the first amplitude step */
	m_NumAmplitudeSteps 	= 0;
	DistanceStart			= AC_TX->ActualDistance;
	CP->DistanceStart 		= DistanceStart;
	
	/* Update Density parameters from UI recipe */
	DynamicParams 			= CP->m_RecipeSC.GetDynamic();
	Reactivity				= DynamicParams.Reactivity;
	ReactivityCounter		= 0;
	ReactivityFinishFlag	= false;
	
	ForceLevelEnabled 		= DynamicParams.IsForceLevelEnabled;
	ForceLevelTime			= DynamicParams.ForceLevelTime;
	ForceLevelFinishFlag	= !DynamicParams.IsForceLevelEnabled;
	ForceLevel				= DynamicParams.ForceLevel;
	ForceLevelCounter		= 0;
	
	if(CP->m_RecipeSC.GetNumberOfForceSteps() > 1){
		WeldForce			= CP->m_RecipeAC.GetForceStepForce(CP->m_RecipeSC.GetNumberOfForceSteps() - 1);
	}else{
		WeldForce			= CP->m_RecipeAC.GetWeldForce();
	}
	
	m_MaxWeldTimeoutCount 	= CP->m_RecipeSC.GetMaxWeldTimeout();
	m_MaxWeldTimeoutCount=((m_MaxWeldTimeoutCount) *(1000/ECAT_CYCLE_TIME_IN_US));
	
	m_NumAmplitudeSteps 	= CP->m_RecipeSC.GetAmplitudeSteps();
	m_AmplitudeStepAt		= CP->m_RecipeSC.GetAmplitudeStepAt();
	PC_RX->Amplitudechange 	= CP->m_RecipeSC.InitialAmplitude();
	ScrubAmplitude			= CP->m_RecipeSC.GetScrubAmplitude();
	m_UI_MSG_Q_ID			= CP->GetMsgQId(cTaskName[UI_T]);
	
	if (CP->stackRecipe.GetData(StackRecipe::INTERNAL_OFFSET_FLAG)==1)
	{
#ifdef DEBUG
		LOGDBG("flag is 1",0,0,0);
#endif
		CP->recipe["FrequencyOffset"]=CP->stackRecipe.GetData(StackRecipe::INTERNAL_FREQ_OFFSET);
	}
	else
	{
		Freqoffset=0;
	}	
	CP->SetFrequencyOffset(Freqoffset);
	
#ifdef DEBUG
	printf("WeldSonicOn : NumOfAmpSteps :  %u :  AmpStepAt : %u :\n",m_NumAmplitudeSteps,m_AmplitudeStepAt);
#endif

	numForceSteps 			= CP->m_RecipeSC.GetNumberOfForceSteps();
	forceStepAt 			= (STEPPINGMODE)CP->m_RecipeSC.GetForceStepAt();
#ifdef DEBUG
	printf("WeldSonicOn : NumOfForceSteps :  %u, forceStepAt : %d\n",numForceSteps,forceStepAt);
#endif

	if(numForceSteps > 1)
	{
		if(forceStepAt == TIMEMODE) //convert to cycles
		{
			for(currentForceStep = 0;currentForceStep < numForceSteps;currentForceStep++)
			{
				forceStepAtValue[currentForceStep] = (CP->m_RecipeSC.GetForceStepAtValue(currentForceStep) * (1000/ECAT_CYCLE_TIME_IN_US));
			}
		}
		else  /* For other step at modes */ 
		{
			for(currentForceStep = 0;currentForceStep < numForceSteps;currentForceStep++)
			{
				forceStepAtValue[currentForceStep] = CP->m_RecipeSC.GetForceStepAtValue(currentForceStep);
			}
		}
	}
	currentForceStep = 1;
	
	modes 				= CP->m_RecipeSC.GetWeldMode();
	modeValue 			= CP->m_RecipeSC.GetModeValue();
#ifdef DEBUG
	LOGDBG("Current Weld Mode : %d, Weld Value %d", modes,modeValue,0);
#endif

	if(modes == TIMEMODE)
	{
		uiWeldSonicOnCounter = (modeValue*(1000/ECAT_CYCLE_TIME_IN_US));
#ifdef DEBUG
		LOGDBG("WELD BY  TIME: %d ms", modeValue,0,0);
#endif
	}
	else if (modes==ENERGYMODE)
	{
		EnergyNeededinMilliJoules =  modeValue;
#ifdef DEBUG
		LOGDBG("WELD BY  Energy: %d J", modeValue,0,0);
#endif
	}
	else if(modes == GRDDETECTMODE)
	{
		ScrubTime = (modeValue*(1000/ECAT_CYCLE_TIME_IN_US));
		ScrubTimeCounter=0;
#ifdef DEBUG
		LOGDBG("WELD BY  GRDDETECTMODE : SCRUBTIME: %d ms", modeValue,0,0);
#endif
	}
	else if(modes == ABSMODE)
	{
#ifdef DEBUG
		LOGDBG("WELD BY  ABSOLUTE DISTANCE : %d um", modeValue,0,0);
#endif
	}
	else if(modes == COLLAPSEMODE)
	{
#ifdef DEBUG
		LOGDBG("WELD BY  COLLAPSE DISTANCE : %d um", modeValue,0,0);
#endif
	}
	else if(modes == PEAKPOWERMODE)
	{
#ifdef DEBUG
		LOGDBG("WELD BY  PEAK POWER: %d W", modeValue,0,0);
#endif
	}
	else if(modes == DYNAMICMODE)
	{
		//The velocity's unit is micro meters per second
		//The range goes from 0µm/s (100%) to 50,000µm/s (0%)
		//Below is the linear function that represents the velocity in percentage.
		modeValue = (50000 - (500*modeValue));
		
		//The reactivity's unit is micro seconds
		//The range goes from 5,000µs (100%) to 300,000µs (0%)
		//Below is the linear function that represents the Time range in percentage.
		Reactivity = ((300000 - (2950*Reactivity))/ECAT_CYCLE_TIME_IN_US);
		
		ForceLevel = ((ForceLevel*WeldForce)/100);
		ForceLevelTime = (ForceLevelTime*(1000/ECAT_CYCLE_TIME_IN_US));
#ifdef DEBUG
		LOGDBG("WELD BY  DYNAMIC: %d W", modeValue,0,0);
#endif
	}

#ifdef SONICS_LED_GPIO_TEST
	TurnonLed();
#endif
	
	/* Weld cycle started and cycle counter will be incremented */
	ControlTask::UpdateCycleCounter();			
	CommonProperty::weldResult.Set(WeldResults::WELD_RECIPE_STATUS, WeldRecipeStatus::GetRecipeStatus());
	CommonProperty::weldResult.Set(WeldResults::PEAK_POWER, 0);
	CommonProperty::weldResult.Set(WeldResults::TRIGGER_TIME, CP->m_TimeCounter);		/* Trigger Time Point - For Weld Graph */
	
	/* check eMMC wear level */
	if ((CommonProperty::weldResult.Get (WeldResults::CYCLE_COUNTER) % EMMC_WEAR_CYCLE_CHECK) == 0)
		ControlTask::CheckEmmcWearLevel ();

	/* Keeps only 100ms signature data before trigger  */ 
	if(CP->m_qWeldSignature.size() > WELD_DATA_POINTS_BAND_MS)
	{	
		deque<WeldSignature>::iterator startPoint = CP->m_qWeldSignature.begin(); 
		deque<WeldSignature>::iterator endPoint   = (CP->m_qWeldSignature.end() - WELD_DATA_POINTS_BAND_MS);
		CP->m_qWeldSignature.erase(startPoint,endPoint);
	}
	
	// set output for weld active 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_WELD_ACTIVE, DigitalIO::LINE_HIGH);
}

/**************************************************************************//**
* 
* 
* \brief   - This method will be executed every 250us while waiting for the. 
* 			 weld time to expire for the Time mode,
* 			 energy reach specified weld energy for the energy mode,
* 			 Weld distance reach specified distance for absolute mode,
* 			 Collapse distance reach specified distance for the collapse mode,
* 			 scrub time to expire for the ground detect mode,
* 			 Power reach specified peak power level for the Peak Power mode.
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
void WeldSonicOn::Loop()
{
	ACVelocity::CalculateVelocity();
	// Note: this call keeps a status that can be checked later
	DigitalIOMgr::ReadInputs ();

	if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
	{
		// ignore INPUT_DISABLE process input chain here
		if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_CYCLE_ABORT) == DigitalIO::INPUT_ASSERTED)
		{
			// ignore INPUT_DISABLE process input chain here
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
				if (modes != GRDDETECTMODE)
				{
					// ground detect cutoff triggered while in other modes
					 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(GROUND_DETECT_CUTOFF))
						 AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_GND_DETECT_CUTOFF_WRA);
					
					// continue to HOLD immediately
					StateTrans();
					return;
				}
			}
			
			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_SONIC_DISABLE) == DigitalIO::INPUT_ASSERTED)
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_DISABLE_WRA);
		}
	}
	
	ProcessLoop ();
}

/**************************************************************************//**
* 
* \brief   - Overrides the base class function to check for horn clamp on alarm
*
* \param   - UINT8.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::ProcessAlarm (UINT8 origin, UINT16 alarmID)
{
	AlarmMgr::EnterAlarmEvent (origin, alarmID);
	if(sysConfigData.bIsHornClamp)
	{
		ChangeState(HORN_CLAMP_ALARM);
	} else
	{
		ChangeState(SC_ALARM);
	}
	
	/* Alarm raised after trigger point during weld will be counted as rejected weld */
	bRejectStatus = true;
}

/**************************************************************************//**
* 
* \brief   - This method will be use to compartmentalize repeating loop code. 
*
* \param   - cmdID - command to process.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::ProcessLoop ()
{
	bool weld_done = false;
	bool weld_max_timeout = false;
	
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
		/* Alarm raised after trigger point during weld will be counted as rejected weld */
		bRejectStatus = true;
		AlarmMgr::EnterAlarmEvent (ALARM_PC);
		StopFrequency=PC_TX->Frequency;
		if(sysConfigData.bIsHornClamp)
		{
			ChangeState(HORN_CLAMP_ALARM);
		} else
		{
			ChangeState(HOLD);
		}
	}  
	else if(PC_TX->PCState == WELD_RUN)
	{
		if(AC_TX->ActualForce < MIN_TRIGGER_FORCE)
		{
			if(TriggerLostCounter++ == (1000/ECAT_CYCLE_TIME_IN_US))
			{
				//warning type alarm does not cause reject
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TRIGGER_LOST_WRA);
			}
		}
		else
		{
			TriggerLostCounter = 0;
		}
		
		
		AccumulatedEnergy 	= AccumulatedEnergy + PC_TX->Power;
		EnergyinMilliJoules = ((AccumulatedEnergy*ECAT_CYCLE_TIME_IN_US)/1000);

		if (SonicsOnCounter % 4 == 0)
		{
			CP->m_TimeCounter =  CP->m_TimeCounter + 1;				/* static counter for keep track of total weld time */
			
			if (CP->ind_CollectGraphData==true)
				CaptureWeldData();			/* Accumulate data into weld signature queue on every 250 us */
		}
		
		SonicsOnCounter = SonicsOnCounter + 1;

		/* max time out implementation. Except time mode */
		if((m_MaxWeldTimeoutCount != 0) && (SonicsOnCounter >= m_MaxWeldTimeoutCount) && (modes !=  TIMEMODE))
		{
			// enter alarm here, continue with the cycle
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_MAX_CMA);
			weld_max_timeout = true;
			weld_done = true;
			bRejectStatus = true; /* When max time out alarm raised, Weld status will be updated as Rejected in production table */
		}
		
		if(SonicsOnCounter == 1)
		{
			CommonProperty::weldResult.Set(WeldResults::START_FREQ, PC_TX->Frequency);
		}
				   
		if(CommonProperty::weldResult.Get(WeldResults::PEAK_POWER) < PC_TX->Power)
		{
			CommonProperty::weldResult.Set(WeldResults::PEAK_POWER, PC_TX->Power);	
		}
		
		if(0 == SonicsOnCounter % TIMER_INTERVAL_250_MS)	/* Present power value will be notified to UIC on every 250 milli-seconds*/
		{
			SendMsgToUIMsgQ(PC_TX->Power);
		}    
		
		if(NewMaxWeldForce<AC_TX->ActualForce)
		{
		   NewMaxWeldForce=AC_TX->ActualForce;
		}
		
		if (!weld_max_timeout)
		{
			INT32 ActualCollapseDistance;
			switch(modes)
			{
			case TIMEMODE:
				if(SonicsOnCounter >= uiWeldSonicOnCounter) 
				{
					weld_done = true;
				}
				break;
			case ENERGYMODE:
				if(EnergyinMilliJoules >= EnergyNeededinMilliJoules || PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE)) 
				{
					weld_done = true;
				}
				break;
			case PEAKPOWERMODE:
				if(PC_TX->Power >= modeValue || PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE)) 
				{
					weld_done = true;
				}
				break;
			case GRDDETECTMODE:
				if (PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE))
				{
					weld_done = true;
				}
				else if(0 == ScrubTimeCounter) 
				{
					if (AC_TX->ACInputs & GRDDETMASK)
					{
						PC_RX->Amplitudechange = ScrubAmplitude;
						ScrubTimeCounter++;
					}
				}
				else
				{
					ScrubTimeCounter++;
					if(ScrubTimeCounter >= ScrubTime)
					{
						weld_done = true;
					}
				}
				break;
			case COLLAPSEMODE:
				ActualCollapseDistance = (INT32) (AC_TX->ActualDistance - DistanceStart);
				if(ActualCollapseDistance >= modeValue  || PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE)) 
				{
					weld_done = true;
				}
				break;
			case ABSMODE:
				if(AC_TX->ActualDistance >= modeValue  || PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE)) 
				{
					weld_done = true;
				}
				break;
			case DYNAMICMODE:											
				if(PC_RX->MasterEvents & BIT_MASK(CTRL_PC_SONIC_DISABLE))
				{
					weld_done = true;
				} else{
					if(ACVelocity::GetVelocity() <= modeValue)
					{
						ReactivityCounter++;
						if(ReactivityCounter >= Reactivity)
							ReactivityFinishFlag = true;
					} else
					{
						ReactivityCounter = 0;
						ReactivityFinishFlag = false;
					}
				
					if(true == ForceLevelEnabled){
						if(AC_TX->ActualForce >= ForceLevel)
						{
							ForceLevelCounter++;
							if (ForceLevelCounter >= ForceLevelTime)
								ForceLevelFinishFlag = true;
						}else	
						{
							ForceLevelCounter = 0;	
							ForceLevelFinishFlag = false;
						}
					}
				
					if (ReactivityFinishFlag && ForceLevelFinishFlag)
						weld_done = true;
				}
				
				break;
			default:
			LOGDBG("Error! Unknown weld mode!",0,0,0);
				weld_done = true;
				break;
			}
		}
		
		if(true != weld_done) 
		{
			if(AC_RX->ACControlEvent & BIT_MASK(EVENT_STEP_FORCE))
			{
				AC_RX->ACControlEvent &= ~BIT_MASK(EVENT_STEP_FORCE);
			}
			else if(currentForceStep < numForceSteps)
			{
				CheckForForceStep();
			}
		
			if(StepCounter < m_NumAmplitudeSteps)
			{
				AmplitudeStepping();
			}	
			
			if (CutoffLimitsValidation())
		    {	
				/* Alarm raised after trigger point during weld will be counted as rejected weld */
				bRejectStatus = true;
				ChangeState(HOLD);
			}
		}
		else 
		{
			if (CP->stackRecipe.GetData(StackRecipe::END_OF_WELD_STORE)==1)
			{
				INT32 MemoryOffset=0;
				INT32 EndFrequency=PC_TX->Frequency;
				if (PC_TX->PCState!=PC_ALARM)
				{
					MemoryOffset = EndFrequency - CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
					CP->stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, MemoryOffset);
					CP->recipe["MemoryOffset"]=MemoryOffset;		// this gets sent to PC in an SDO
					CP->stackRecipe.SetData(StackRecipe::MEMORY, EndFrequency);
				}
				// in case of alarm, we need to start the next weld at the same place as this weld started.
				// this is because an overload can cause the frequency to change drastically therefore we 
				// don't want to start the next weld at a bad frequency.
				else 
				{
					EndFrequency = CP->stackRecipe.GetData(StackRecipe::MEMORY);
				}
				
				CP->recipe["StartFrequency"] = EndFrequency;		
				CP->m_RecipePC.UpdateResonantFreq(EndFrequency);    // this call updates the PC recipe which gets sent to PC in an SDO		
			}
			StateTrans();
		}
	}
	else //PC hasn't turned on sonics
	{
		if(++sonicsDelayCounter > MAX_SONICS_DELAY)
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_DELAY_TIMEOUT_NCA);
	}
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when exiting the WeldSonicOn state . 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::Exit()
{
	SendMsgToUIMsgQ(CommonProperty::weldResult.Get(WeldResults::PEAK_POWER));			/* Final power value will be notified to UIC */
	
	FrequencyChange = StopFrequency - CommonProperty::weldResult.Get(WeldResults::START_FREQ);
	CommonProperty::weldResult.Set(WeldResults::FREQ_CHANGE, FrequencyChange);
	CommonProperty::weldResult.Set(WeldResults::TOTAL_ENERGY, (UINT32)(EnergyinMilliJoules));
	CommonProperty::weldResult.Set(WeldResults::MAX_WELD_FORCE, (UINT32) NewMaxWeldForce);
	CommonProperty::weldResult.Set(WeldResults::WELD_TIME, SonicsOnCounter*ECAT_CYCLE_TIME_IN_US);
	CommonProperty::weldResult.Set(WeldResults::WELD_COLLAPSE_DIST, AC_TX->ActualDistance-DistanceStart);
	CommonProperty::weldResult.Set(WeldResults::WELD_ABSOLUTE, AC_TX->ActualDistance);
	CommonProperty::weldResult.Set(WeldResults::WELD_SONIC_TIME, CP->m_TimeCounter);		/* SonicOnEnd Time Point - For Weld Graph */	
	AC_RX->ACControlEvent &= 0x0F;
	
	// reset output for weld active 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_WELD_ACTIVE, DigitalIO::LINE_LOW);


#ifdef DEBUG
	LOGDBG("WeldTime : %d, Counter : %d",modeValue,Counter,0);
#endif

#ifdef SONICS_LED_GPIO_TEST
	TurnoffLed();
#endif

#ifdef PRINT
#endif

}

/**************************************************************************//**
* 
* \brief   - This method will be executed when WeldSonic ready to be
* 			  transacted to hold state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::StateTrans()
{	
  if(AC_RX->ACControlEvent & BIT_MASK(EVENT_STEP_FORCE))
  {
  	AC_RX->ACControlEvent &= ~BIT_MASK(EVENT_STEP_FORCE);
  }
  StopFrequency=PC_TX->Frequency;
  ChangeState(HOLD);
}

/**************************************************************************//**
* 
* \brief   - Determine if it is time to move to the next force step.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::CheckForForceStep()
{	
	AC_RX->ACControlEvent &= ~BIT_MASK(EVENT_STEP_FORCE);
	switch(forceStepAt) 
	{
		case STEPBYTIME:
			if(SonicsOnCounter >= forceStepAtValue[currentForceStep]) 
			{
				AC_RX->ACControlEvent |= BIT_MASK(EVENT_STEP_FORCE);
#ifdef DEBUG
				printf("WeldSonicOn : ForceStep : %d Counter:%d \n",currentForceStep, Counter);
#endif
				currentForceStep++;
			}
			break;
			
		case STEPBYENERGY:
			if(EnergyinMilliJoules >= forceStepAtValue[currentForceStep]) 
			{
					AC_RX->ACControlEvent |= BIT_MASK(EVENT_STEP_FORCE);
#ifdef DEBUG
				printf("WeldSonicOn : ForceStep : %d EnergyinMilliJoules:%d \n",currentForceStep, EnergyinMilliJoules);
#endif
				currentForceStep++;
			}
			break;
			
		case STEPBYPOWER:
				if(PC_TX->Power >= forceStepAtValue[currentForceStep]) 
				{
					AC_RX->ACControlEvent |= BIT_MASK(EVENT_STEP_FORCE);
					currentForceStep++;
				}
				break;
				
		case STEPBYCOLLAPSEDISTANCE:
				//(INT32) Cast is needed because sometimes collapse value throws negative values.
				if((AC_TX->ActualDistance > DistanceStart) && ((INT32)(AC_TX->ActualDistance - DistanceStart) >= (INT32)forceStepAtValue[currentForceStep])) 
				{
					AC_RX->ACControlEvent |= BIT_MASK(EVENT_STEP_FORCE);
					currentForceStep++;
				}
				break;
				
		case STEPBYABSOLUTEDISTANCE:
				if(AC_TX->ActualDistance >= forceStepAtValue[currentForceStep]) 
				{
					AC_RX->ACControlEvent |= BIT_MASK(EVENT_STEP_FORCE);
					currentForceStep++;
				}
				break;
				
		default:
			LOGDBG("Error! Unknown Force Step Mode!",0,0,0);
			numForceSteps=0;
			break;
	}
	AC_RX->ACControlEvent &= 0x0F;
	AC_RX->ACControlEvent |= currentForceStep << 4;
}

/**************************************************************************//**
* \Method Name :   	AmplitudeStepping() 
* 
* \brief  - this function performance the weld with different Apmplitude settings
*
* \param  -  none
*
* \return  - none
*
******************************************************************************/
void WeldSonicOn::AmplitudeStepping()
{
	switch(m_AmplitudeStepAt)
	{
		case STEPBYTIME:
		{     
			if(StepCounter < m_NumAmplitudeSteps)
			{
				if(SonicsOnCounter >= ((CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter)*1000)/ECAT_CYCLE_TIME_IN_US))
				{    
					PC_RX->Amplitudechange = CP->m_RecipeSC.GetAmplitudeStep(StepCounter);
#ifdef DEBUG
					printf("WeldSonicOn : StepCounter : %u\n", StepCounter);
					printf("WeldSonicOn : stepvalue : %u\n", CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter));
					printf("WeldSonicOn : step %u\n", CP->m_RecipeSC.GetAmplitudeStep(StepCounter));
#endif
					StepCounter = StepCounter + 1;
				}
			}
		}
		break;
	
		case STEPBYENERGY:
		{
			if(StepCounter < m_NumAmplitudeSteps)
			{
				if(EnergyinMilliJoules >= CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter))
				{
		   			PC_RX->Amplitudechange = CP->m_RecipeSC.GetAmplitudeStep(StepCounter);
#ifdef DEBUG
					printf("WeldSonicOn : StepCounter : %u\n", StepCounter);
					printf("WeldSonicOn : stepvalue : %u\n", CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter));
					printf("WeldSonicOn : step %u\n", CP->m_RecipeSC.GetAmplitudeStep(StepCounter));
#endif
					StepCounter = StepCounter + 1;
				}  	
			}
		}
		break;
		
		case STEPBYPOWER:
		{
			if(StepCounter < m_NumAmplitudeSteps)
			{
				if(PC_TX->Power >= CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter))
				{
					PC_RX->Amplitudechange = CP->m_RecipeSC.GetAmplitudeStep(StepCounter);
#ifdef DEBUG
					printf("WeldSonicOn : StepCounter : %u\n", StepCounter);
					printf("WeldSonicOn : stepvalue : %u\n", CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter));
					printf("WeldSonicOn : step %u\n", CP->m_RecipeSC.GetAmplitudeStep(StepCounter));
#endif
					StepCounter = StepCounter + 1;
				}  
	
			}
		}
		break;
		
		case STEPBYCOLLAPSEDISTANCE:   
		{
			if(StepCounter < m_NumAmplitudeSteps)
			{
				//(INT32) Cast is needed because sometimes collapse value throws negative values.
				if(((INT32)(AC_TX->ActualDistance-DistanceStart)) >= ((INT32)CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter)))
				{ 
					PC_RX->Amplitudechange = CP->m_RecipeSC.GetAmplitudeStep(StepCounter);
#ifdef DEBUG
					printf("WeldSonicOn : StepCounter : %u\n", StepCounter);
					printf("WeldSonicOn : stepvalue : %u\n", CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter));
					printf("WeldSonicOn : step %u\n", CP->m_RecipeSC.GetAmplitudeStep(StepCounter));
#endif
					StepCounter = StepCounter + 1;
				}  
			}
		}
		break;
		
		case STEPBYABSOLUTEDISTANCE:
		{
			if(StepCounter < m_NumAmplitudeSteps)
			{
				if(AC_TX->ActualDistance >= CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter))
				{
					PC_RX->Amplitudechange = CP->m_RecipeSC.GetAmplitudeStep(StepCounter);
#ifdef DEBUG
					printf("WeldSonicOn : StepCounter : %u\n", StepCounter);
					printf("WeldSonicOn : stepvalue : %u\n", CP->m_RecipeSC.GetAmplitudeStepAtValue(StepCounter));
					printf("WeldSonicOn : step %u\n", CP->m_RecipeSC.GetAmplitudeStep(StepCounter));
#endif
					StepCounter = StepCounter + 1;
				}  
			}
		}
		break;
		
		default:
			LOGDBG("The selection is unknown. %d",CP->m_RecipeSC.GetAmplitudeStepAt(),0,0);
			break;	
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
void WeldSonicOn::CaptureWeldData()
{
	WeldSignature weldSignature;
	
	weldSignature.AbsoluteDistance 	= AC_TX->ActualDistance;
	weldSignature.Amplitude			= PC_TX->Amplitude;
	weldSignature.CollapseDistance	= AC_TX->ActualDistance - DistanceStart;
	weldSignature.Current			= PC_TX->Current/100;
	weldSignature.Energy			= EnergyinMilliJoules;
	weldSignature.Force				= AC_TX->ActualForce;
	weldSignature.Frequency			= PC_TX->Frequency/10;
	weldSignature.Phase				= PC_TX->Phase/10;
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
* \brief   -  	Power progress bar indication to UIC  
*
* \param   -	UINT16 power_data
*
* \return  -    None 
*
******************************************************************************/
void WeldSonicOn::SendMsgToUIMsgQ(UINT16 power_data)
{
	Message msg;
	
	msg.msgID = TO_TASK_WELD_PROGRESS_IND;
	sprintf(msg.Buffer,"%d",power_data);
	
	if(msgQSend(m_UI_MSG_Q_ID,reinterpret_cast<char*>(&msg), sizeof(msg),NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("-----------Error on msgQSend----------",0,0,0);
	}
}

/**************************************************************************//**
* \brief  - Cutoff limits validation 
*
* \param  - None
*
* \return - None
* 
******************************************************************************/
//bool WeldSonicOn::CutoffLimitsValidation()
//{
//	 CommonProperty *CP = CommonProperty::getInstance();
//	 //ActiveRecipe *limitObj = new(nothrow) ActiveRecipe(CP->getActiveRecipeInstance());	
//	 
//	 bool bIsValid = false;
//
//	 if(NULL != limitObj)
//	 {
//		// if(limitObj->IsGlobalCutOffLimitCheckEnabled())		/* Check global reject limit enabled or not */
//		 if(CP->m_RecipeSC.IsGlobalCutOffLimitCheckEnabled())	
//	     {
//			 
//			 if (limitObj->IsCutoffLimitCheckEnabled(PEAK_POWER_CUTOFF))
//			 {
//				 if (limitObj->ValidateCutoffLimit(PEAK_POWER_CUTOFF, PC_TX->Power))
//				 {
//					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_PEAK_POWER_CUTOFF_WRA);
//					//printf("--------------------------alarm=%x\n",ALARM_PEAK_POWER_CUTOFF_WRA);
//					bIsValid=true;
//				 } 
//			 }	
//
//			 if (limitObj->IsCutoffLimitCheckEnabled(COLLAPSE_DISTANCE_CUTOFF))
//			 {
//				 if (limitObj->ValidateCutoffLimit(COLLAPSE_DISTANCE_CUTOFF, (AC_TX->ActualDistance - DistanceStart)))
//				 {
//					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_COLLAPSE_CUTOFF_WRA);
//					//printf("------------------------------alarm=%x\n", ALARM_COLLAPSE_CUTOFF_WRA);
//					bIsValid=true;
//				 } 
//			 }
//
//			 if (limitObj->IsCutoffLimitCheckEnabled(ABSOLUTE_DISTANCE_CUTOFF))
//			 {
//		
//				 if (limitObj->ValidateCutoffLimit(ABSOLUTE_DISTANCE_CUTOFF, AC_TX->ActualDistance))
//				 {
//					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ABSOLUTE_CUTOFF_WRA );
//					//printf("------------------------------alarm=%x\n", ALARM_ABSOLUTE_CUTOFF_WRA);
//					bIsValid=true;
//				 } 
//			 }	
//
//			 if (limitObj->IsCutoffLimitCheckEnabled(FREQUENCY_LOW_CUTOFF))
//			 {
//				 if (limitObj->ValidateCutoffLimit(FREQUENCY_LOW_CUTOFF, PC_TX->Frequency))
//				 {
//					 AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_LOW_CUTOFF_WRA);
//					 //printf("------------------------------alarm=%x\n", ALARM_FREQUENCY_LOW_CUTOFF_WRA);
//					 bIsValid=true;
//				 } 
//			 }
//
//			 if (limitObj->IsCutoffLimitCheckEnabled(FREQUENCY_HIGH_CUTOFF))
//			 {
//				 if (limitObj->ValidateCutoffLimit(FREQUENCY_HIGH_CUTOFF, PC_TX->Frequency))
//				 {
//					 AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_HIGH_CUTOFF_WRA);
//					 //printf("------------------------------alarm=%x\n", ALARM_FREQUENCY_HIGH_CUTOFF_WRA);
//					 bIsValid=true;
//				 } 
//			 }
//			
//			 if (limitObj->IsCutoffLimitCheckEnabled(ENERGY_CUTOFF))
//			 {
//				 if (limitObj->ValidateCutoffLimit(ENERGY_CUTOFF, EnergyinMilliJoules/1000))
//				 {
//					 AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ENERGY_CUTOFF_WRA);
//					 //printf("------------------------------alarm=%x\n", ALARM_ENERGY_CUTOFF_WRA);
//					 bIsValid=true;
//				 } 
//			 }
//			
//			 if (limitObj->IsCutoffLimitCheckEnabled(TIME_CUTOFF))
//			 {
//				 if (limitObj->ValidateCutoffLimit(TIME_CUTOFF, (Counter*ECAT_CYCLE_TIME_IN_US)/1000))
//				 {
//					 AlarmMgr::EnterAlarmEvent(ALARM_SC, ALARM_TIME_CUTOFF_WRA);
//					 //printf("------------------------------alarm=%x\n", ALARM_TIME_CUTOFF_WRA);
//					 bIsValid=true;
//				 }
//			 }
//			 
//			 
//			 if (limitObj->IsCutoffLimitCheckEnabled(GROUND_DETECT_CUTOFF))
//			 {
//				if ((AC_TX->ACInputs & GRDDETMASK)==GRDDETMASK)
//			     {
//			        AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_GND_DETECT_CUTOFF_WRA);
//					//printf("------------------------------alarm=%x\n", ALARM_ABSOLUTE_CUTOFF_WRA);
//			        bIsValid=true;
//			     } 
//			 }
//			  
//	     }
//		delete limitObj;
//		limitObj = NULL;		 
//	 }
//  return bIsValid;
//}

bool WeldSonicOn::CutoffLimitsValidation()
{
	 CommonProperty *CP = CommonProperty::getInstance();
	 
	 bool bIsValid = false;

		 if(CP->m_RecipeSC.IsGlobalCutOffLimitCheckEnabled())	
	     {
			 
			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(PEAK_POWER_CUTOFF))
			 {
				 if (CP->m_RecipeSC.ValidateCutoffLimit(PEAK_POWER_CUTOFF, PC_TX->Power))
				 {
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_PEAK_POWER_CUTOFF_WRA);
					//printf("--------------------------alarm=%x\n",ALARM_PEAK_POWER_CUTOFF_WRA);
					bIsValid=true;
				 } 
			 }	

			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(COLLAPSE_DISTANCE_CUTOFF))
			 {
				 if (CP->m_RecipeSC.ValidateCutoffLimit(COLLAPSE_DISTANCE_CUTOFF, (AC_TX->ActualDistance - DistanceStart)))
				 {
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_COLLAPSE_CUTOFF_WRA);
					//printf("------------------------------alarm=%x\n", ALARM_COLLAPSE_CUTOFF_WRA);
					bIsValid=true;
				 } 
			 }

			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(ABSOLUTE_DISTANCE_CUTOFF))
			 {
		
				 if (CP->m_RecipeSC.ValidateCutoffLimit(ABSOLUTE_DISTANCE_CUTOFF, AC_TX->ActualDistance))
				 {
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ABSOLUTE_CUTOFF_WRA );
					//printf("------------------------------alarm=%x\n", ALARM_ABSOLUTE_CUTOFF_WRA);
					bIsValid=true;
				 } 
			 }	

			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(FREQUENCY_LOW_CUTOFF))
			 {
				 if (CP->m_RecipeSC.ValidateCutoffLimit(FREQUENCY_LOW_CUTOFF, PC_TX->Frequency))
				 {
					 AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_LOW_CUTOFF_WRA);
					 //printf("------------------------------alarm=%x\n", ALARM_FREQUENCY_LOW_CUTOFF_WRA);
					 bIsValid=true;
				 } 
			 }

			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(FREQUENCY_HIGH_CUTOFF))
			 {
				 if (CP->m_RecipeSC.ValidateCutoffLimit(FREQUENCY_HIGH_CUTOFF, PC_TX->Frequency))
				 {
					 AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_HIGH_CUTOFF_WRA);
					 //printf("------------------------------alarm=%x\n", ALARM_FREQUENCY_HIGH_CUTOFF_WRA);
					 bIsValid=true;
				 } 
			 }
			
			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(ENERGY_CUTOFF))
			 {
				 if (CP->m_RecipeSC.ValidateCutoffLimit(ENERGY_CUTOFF, EnergyinMilliJoules))
				 {
					 AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ENERGY_CUTOFF_WRA);
					 //printf("------------------------------alarm=%x\n", ALARM_ENERGY_CUTOFF_WRA);
					 bIsValid=true;
				 } 
			 }
			
			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(TIME_CUTOFF))
			 {
				 if (CP->m_RecipeSC.ValidateCutoffLimit(TIME_CUTOFF, (SonicsOnCounter*ECAT_CYCLE_TIME_IN_US)/1000))
				 {
					 AlarmMgr::EnterAlarmEvent(ALARM_SC, ALARM_TIME_CUTOFF_WRA);
					 //printf("------------------------------alarm=%x\n", ALARM_TIME_CUTOFF_WRA);
					 bIsValid=true;
				 }
			 }
			 
			 
			 if (CP->m_RecipeSC.IsCutoffLimitCheckEnabled(GROUND_DETECT_CUTOFF))
			 {
				if ((AC_TX->ACInputs & GRDDETMASK)==GRDDETMASK)
			     {
			        AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_GND_DETECT_CUTOFF_WRA);
					//printf("------------------------------alarm=%x\n", ALARM_ABSOLUTE_CUTOFF_WRA);
			        bIsValid=true;
			     } 
			 }
	 }
  return bIsValid;
}
#ifdef SONICS_LED_GPIO_TEST
/*******************************************************************************
*      Method Name :   	TurnonLed()
*      Description :   	This method is responsible for enable GPIO_6_7                    
*      param       :   	void
*      return      :   	void
*   
********************************************************************************/
void WeldSonicOn::TurnonLed()
{
#ifdef SC_DKM		
	vxbGpioAlloc (GPIO_6_7);
		       
	if(GPIO_DIR_INPUT == vxbGpioGetDir(GPIO_6_7))
	{ 
	   vxbGpioSetDir(GPIO_6_7,GPIO_DIR_OUTPUT);
	}
		        
		    
	if (vxbGpioGetValue(GPIO_6_7)==GPIO_VALUE_HIGH)
	{
	   vxbGpioSetValue(GPIO_6_7,GPIO_VALUE_LOW);
	}
    else
	{
	   vxbGpioSetValue(GPIO_6_7,GPIO_VALUE_HIGH);
	}
#else //SC_RTP
	GpioAlloc (GPIO_6_7);
	if(GPIO_DIR_INPUT == GpioGetDir(GPIO_6_7))
	{
		GpioSetDir(GPIO_6_7,GPIO_DIR_OUTPUT);
	}
	
	if (GpioGetValue(GPIO_6_7)==GPIO_VALUE_HIGH)
	{
		GpioSetValue(GPIO_6_7,GPIO_VALUE_LOW);
	}
	else
	{
		GpioSetValue(GPIO_6_7,GPIO_VALUE_HIGH);
	}
#endif //SC_RTP
}

/**************************************************************************//**
* 
* \brief   - This method is responsible for disable GPIO_6_7  
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldSonicOn::TurnoffLed()
{
#ifdef SC_DKM		
	if (vxbGpioGetValue(GPIO_6_7)==GPIO_VALUE_HIGH)
	{
	  vxbGpioSetValue(GPIO_6_7,GPIO_VALUE_LOW);
	}
	else
	{
	  vxbGpioSetValue(GPIO_6_7,GPIO_VALUE_HIGH);
	}
	
	(void)vxbGpioFree (GPIO_6_7);
#else //SC_RTP
	if (GpioGetValue(GPIO_6_7)==GPIO_VALUE_HIGH)
	{
		GpioSetValue(GPIO_6_7,GPIO_VALUE_LOW);
	}
	else
	{
		GpioSetValue(GPIO_6_7,GPIO_VALUE_HIGH);
	}
	
	(void)GpioFree (GPIO_6_7);
#endif //SC_RTP
}
#endif  //SONICS_LED_GPIO_TEST
