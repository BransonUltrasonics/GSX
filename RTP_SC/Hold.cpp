/*
 * Hold.cpp
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#include "Hold.h"
#include "WeldSonicOn.h"
#include "ControlTask.h"
#include "IOManager.h"
#include "MainTask.h"
#include "AlarmManager.h"
#include "ACVelocity.h"

/**************************************************************************//**
* \brief    - Hold Class constructor
*
* \param    - None
*
* \return  - None
*
******************************************************************************/
Hold::Hold()
{
	stateType = HOLD;
}

/**************************************************************************//**
* \brief    - Hold Class destructor
*
* \param    - None
*
* \return   - None
*
******************************************************************************/
Hold::~Hold()
{

}

/**************************************************************************//**
* \brief   - This method will be executed when entering the Hold state
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void Hold::Enter()
{
#ifdef DEBUG
	LOGDBG("enter Hold",0,0,0);
#endif
	
	HoldTimeCounter	= 0;
	NewMaxHoldForce	= 0;
	NewEndHoldForce	= 0;
	HoldTime 	 	= ((CP->m_RecipeSC.GetHoldTime()*1000)/ECAT_CYCLE_TIME_IN_US);
	weldMode		= CP->m_RecipeSC.GetWeldMode();	/* Get weld mode, used if GROUND DETECT is configured */
	
	/*Used for Shutdown event Weld result saving logic*/
	CP->ShutdownCriticalDataFlag = 0;  /* Resetting Critical Data flag */
	
	// set output for hold active 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_HOLD_ACTIVE, DigitalIO::LINE_HIGH);
	
#ifdef DEBUG
	LOGDBG("HOLD TIME: %d ms ; HoldTimeCounter : %d", CP->m_RecipeSC.GetHoldTime(),HoldTime,0);
#endif
}

/**************************************************************************//**
* \brief   - This method will be executed every 250us while waiting for 
*            the hold time to expire
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void Hold::Loop()
{
	ACVelocity::CalculateVelocity();

	if(WELD == ControlTask::GetOperationType()  and (HoldTimeCounter % 4 == 0))
	{
		CP->m_TimeCounter = CP->m_TimeCounter + 1;		/* static counter for keep track of total weld time */
		
		if (CP->ind_CollectGraphData==true)
			CaptureWeldData();			/* Accumulate data into weld signature queue on every 250 us */
	}

	HoldTimeCounter = HoldTimeCounter + 1;
	if(AC_TX->ACState == AC_ESTOP)
	{
		ProcessAlarm (ALARM_AC, ALARM_ESTOP_NCA);
	}
	else if(AC_TX->ACState == AC_ALARM)
    {
		ProcessAlarm (ALARM_AC);
    }
	
	else if (sysConfigData.bIsHornClamp && PC_TX->PCState == PC_ALARM) 
	{
		ProcessAlarm (ALARM_PC);
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
			
			if (DigitalIOMgr::ScanForInputFunction (DigitalIO::INPUT_PART_PRESENT) &&
							DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PART_PRESENT) != DigitalIO::INPUT_ASSERTED)
			{
				ProcessAlarm (ALARM_SC, ALARM_PART_PRESENT_LOST_NCA);
				return;
			}

			if ((AC_TX->ACInputs & GRDDETMASK) && weldMode != GRDDETECTMODE
					&& CP->m_RecipeSC.IsCutoffLimitCheckEnabled (GROUND_DETECT_CUTOFF)) 
			{
				// ground detect cutoff triggered while in other modes
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_GND_DETECT_CUTOFF_WRA);
			}

			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_SONIC_DISABLE) == DigitalIO::INPUT_ASSERTED)
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_DISABLE_WRA);
		}
		
		ProcessLoop ();
	}
}

/**************************************************************************//**
* \brief   - This method will be use to compartmentalize repeating loop code
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void Hold::ProcessLoop ()
{	
	if (HoldTimeCounter < HoldTime)
	{
	   if (NewMaxHoldForce < AC_TX->ActualForce)
	   {
		 NewMaxHoldForce = AC_TX->ActualForce;
	   }
	}
	else  
	{
		if ((DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_DISABLE) != DigitalIO::INPUT_ASSERTED)
				&&(DigitalIOMgr::ScanForInputFunction (DigitalIO::INPUT_HOLD_DELAY)
				&& DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_HOLD_DELAY) != DigitalIO::INPUT_ASSERTED))
		{
			// keep holding if hold delay input is configured but hasn't been asserted yet
			if (NewMaxHoldForce < AC_TX->ActualForce)
				NewMaxHoldForce = AC_TX->ActualForce;
		}
		else
		{
	#ifdef DEBUG
			LOGDBG("holdtime = %d in us", HoldTimeCounter*ECAT_CYCLE_TIME_IN_US,0,0);
	#endif
			
			NewEndHoldForce = AC_TX->ActualForce;
			
						
			
			CommonProperty::weldResult.Set(WeldResults::TOTAL_ABSOLUTE, AC_TX->ActualDistance);
			CommonProperty::weldResult.Set(WeldResults::END_HOLD_FORCE, (UINT32) NewEndHoldForce);
			CommonProperty::weldResult.Set(WeldResults::TOTAL_COLLAPSE_DIST, AC_TX->ActualDistance - DistanceStart);
			CommonProperty::weldResult.Set(WeldResults::HOLD_COLLAPSE_DIST, CommonProperty::weldResult.Get(WeldResults::TOTAL_COLLAPSE_DIST) - CommonProperty::weldResult.Get(WeldResults::WELD_COLLAPSE_DIST));
			LimitsValidation();
			
			ChangeState(WAIT_FOR_READY_POSITION);
		}
	}
}

/**************************************************************************//**
* \brief   - Overrides the base class function to check for horn clamp on alarm 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void Hold::ProcessAlarm (UINT8 origin, UINT16 alarmID)
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
* \brief   -  	Captures weld signature data at every 250 us and insert into
* 				std::deque called m_qWeldSignature.
*
* \param   -	None
*
* \return  -    None 
*
******************************************************************************/
void Hold::CaptureWeldData()
{
	WeldSignature weldSignature;

	weldSignature.AbsoluteDistance 	= AC_TX->ActualDistance;
	weldSignature.Amplitude			= PC_TX->Amplitude;
	weldSignature.CollapseDistance	= AC_TX->ActualDistance - DistanceStart;	/* DistanceStart - Assigned in WeldSonicOn::Enter() with ActualDistance */
	weldSignature.Current			= PC_TX->Current/100;
	weldSignature.Energy			= 0;
	weldSignature.Force				= AC_TX->ActualForce;
	weldSignature.Frequency			= PC_TX->Frequency/10;
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
* \brief   - This method will be executed when exiting the hold state 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void Hold::Exit()
{
	CommonProperty::weldResult.Set(WeldResults::HOLD_TIME, CP->m_TimeCounter);		/* HoldEnd Point - For Weld Graph */
		
	/*Used for Shutdown event Weld result saving logic*/
	CP->ShutdownCriticalDataFlag = 1;  //Setting Critical Data flag 
	CP->ShutdownWeldResult = CommonProperty::weldResult.GetWeldData();	
	
	// set output for PB RELEASE 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_PB_RELEASE, DigitalIO::LINE_HIGH);
	
	// reset output for hold active 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_HOLD_ACTIVE, DigitalIO::LINE_LOW);

#ifdef DEBUG
	LOGDBG("Exit hold",0,0,0);
#endif
}

/**************************************************************************//**
* \brief  - Reject and reject limits validation 
*
* \param  - None
*
* \return - None
* 
******************************************************************************/
void Hold::LimitsValidation()
{
	//Global enable check for Reject Limit
	if(CP->m_RecipeSC.IsGlobalLimitCheckEnabled(REJECT_LIMIT))
	{
		//Individual enable check for Reject Time
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_TIME_EN))
		{
			//Enable check for Reject Time Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, TIME_LIMIT_ML))
			{
				if (CP->m_RecipeSC.ValidateRejectLimit(TIME_LIMIT_ML,(CP->weldResult.Get(WeldResults::WELD_TIME)/1000)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TIME_LIMIT_MLR_PRA);
			}
			//Enable check for Reject Time High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, TIME_LIMIT_PL))
			{ 
				if(CP->m_RecipeSC.ValidateRejectLimit(TIME_LIMIT_PL,(CP->weldResult.Get(WeldResults::WELD_TIME)/1000)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TIME_LIMIT_PLR_PRA);
			}
		}
		//Individual enable check for Reject Energy
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_ENERGY_EN))
		{
			//Enable check for Reject Energy Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, ENERGY_LIMIT_ML))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(ENERGY_LIMIT_ML, CP->weldResult.Get(WeldResults::TOTAL_ENERGY)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ENERGY_LIMIT_MLR_PRA); 
			}
			//Enable check for Reject Energy High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, ENERGY_LIMIT_PL))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(ENERGY_LIMIT_PL, CP->weldResult.Get(WeldResults::TOTAL_ENERGY)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ENERGY_LIMIT_PLR_PRA); 
			}
		}
		//Individual enable check for Reject Peak Power
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_PEAKPOWER_EN))
		{
			//Enable check for Reject Peak Power Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, PEAKPOWER_LIMIT_ML))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(PEAKPOWER_LIMIT_ML, CP->weldResult.Get(WeldResults::PEAK_POWER)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_PEAKPOWER_LIMIT_MLR_PRA); 
			}
			//Enable check for Reject Peak Power High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, PEAKPOWER_LIMIT_PL))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(PEAKPOWER_LIMIT_PL, CP->weldResult.Get(WeldResults::PEAK_POWER)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_PEAKPOWER_LIMIT_PLR_PRA); 
			}
		}
		//Individual enable check for Reject Absolute Distance
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_ABSOLUTE_EN))
		{
			//Enable check for Reject Absolute Distance Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, ABSOLUTE_LIMIT_ML))
			{  
				if (CP->m_RecipeSC.ValidateRejectLimit(ABSOLUTE_LIMIT_ML, CP->weldResult.Get(WeldResults::TOTAL_ABSOLUTE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ABSOLUTEDIST_LIMIT_MLR_PRA); 
			} 
			//Enable check for Reject Absolute Distance High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, ABSOLUTE_LIMIT_PL))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(ABSOLUTE_LIMIT_PL, CP->weldResult.Get(WeldResults::TOTAL_ABSOLUTE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ABSOLUTEDIST_LIMIT_PLR_PRA); 
			} 
		}
		//Individual enable check for Reject Collapse Distance
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_COLLAPSE_EN))
		{
			//Enable check for Reject Collapse Distance Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, COLLAPSE_LIMIT_ML))
			{
				if (CP->m_RecipeSC.ValidateRejectLimit(COLLAPSE_LIMIT_ML, CP->weldResult.Get(WeldResults::TOTAL_COLLAPSE_DIST)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_COLLAPSEDIST_LIMIT_MLR_PRA); 
			}
			//Enable check for Reject Collapse Distance High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, COLLAPSE_LIMIT_PL))
			{
				if (CP->m_RecipeSC.ValidateRejectLimit(COLLAPSE_LIMIT_PL, CP->weldResult.Get(WeldResults::TOTAL_COLLAPSE_DIST)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_COLLAPSEDIST_LIMIT_PLR_PRA);
			}
		}
		//Individual enable check for Reject Trigger Distance
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_TRIGGER_EN))
		{
			//Enable check for Reject Trigger Distance Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, TRIGGER_LIMIT_ML))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(TRIGGER_LIMIT_ML, CP->DistanceStart))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TRIGGERDIST_LIMIT_MLR_PRA);
			}
			//Enable check for Reject Trigger Distance High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, TRIGGER_LIMIT_PL))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(TRIGGER_LIMIT_PL, CP->DistanceStart))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TIRGGERDIST_LIMIT_PLR_PRA);
			} 
		}
		//Individual enable check for Reject Weld Force
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_WELDFORCE_EN))
		{
			//Enable check for Reject Weld Force Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, WELDFORCE_LIMIT_ML))
			{ 
				if(CP->m_RecipeSC.ValidateRejectLimit(WELDFORCE_LIMIT_ML, CP->weldResult.Get(WeldResults::MAX_WELD_FORCE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELDFORCE_LIMIT_MLR_PRA);
			}
			//Enable check for Reject Weld Force High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, WELDFORCE_LIMIT_PL))
			{ 	
				if (CP->m_RecipeSC.ValidateRejectLimit(WELDFORCE_LIMIT_PL, CP->weldResult.Get(WeldResults::MAX_WELD_FORCE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELDFORCE_LIMIT_PLR_PRA);
			} 
		}
		//Individual enable check for Reject Frequency
		if(CP->m_RecipeSC.IsSRCheckEnabled(REJECT_LIMIT, SR_FREQUENCY_EN))
		{
			//Enable check for Reject Frequency Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, FREQUENCY_LIMIT_ML))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(FREQUENCY_LIMIT_ML, CP->weldResult.Get(WeldResults::START_FREQ)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_LIMIT_MLR_PRA);
			} 
			//Enable check for Reject Frequency High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(REJECT_LIMIT, FREQUENCY_LIMIT_PL))
			{ 
				if (CP->m_RecipeSC.ValidateRejectLimit(FREQUENCY_LIMIT_PL, (CP->weldResult.Get(WeldResults::START_FREQ)/10)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_LIMIT_PLR_PRA);
			}
		}
	}
	//Global enable check for Suspect Limit
	if(CP->m_RecipeSC.IsGlobalLimitCheckEnabled(SUSPECT_LIMIT))
	{
		//Individual enable check for Suspect Time
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_TIME_EN))
		{
			//Enable check for Suspect Time Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, TIME_LIMIT_ML))
			{ 
				if (CP->m_RecipeSC.ValidateSuspectLimit(TIME_LIMIT_ML,(CP->weldResult.Get(WeldResults::WELD_TIME)/1000)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TIME_LIMIT_MLS_PSA);
			} 
			//Enable check for Suspect Time High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, TIME_LIMIT_PL))
			{ 			 
				if (CP->m_RecipeSC.ValidateSuspectLimit(TIME_LIMIT_PL,(CP->weldResult.Get(WeldResults::WELD_TIME)/1000)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TIME_LIMIT_PLS_PSA);
			}
		}
		//Individual enable check for Suspect Energy
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_ENERGY_EN))
		{
			//Enable check for Suspect Energy Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, ENERGY_LIMIT_ML))
			{ 	
				if (CP->m_RecipeSC.ValidateSuspectLimit(ENERGY_LIMIT_ML, CP->weldResult.Get(WeldResults::TOTAL_ENERGY)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ENERGY_LIMIT_MLS_PSA );
			}
			//Enable check for Suspect Energy High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, ENERGY_LIMIT_PL))
			{ 	
				if (CP->m_RecipeSC.ValidateSuspectLimit(ENERGY_LIMIT_PL, CP->weldResult.Get(WeldResults::TOTAL_ENERGY)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ENERGY_LIMIT_PLS_PSA);
			}
		}
		//Individual enable check for Suspect Peak Power
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_PEAKPOWER_EN))
		{
			//Enable check for Suspect Peak Power Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, PEAKPOWER_LIMIT_ML))
			{ 	
				if (CP->m_RecipeSC.ValidateSuspectLimit(PEAKPOWER_LIMIT_ML, CP->weldResult.Get(WeldResults::PEAK_POWER)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_PEAKPOWER_LIMIT_MLS_PSA );
			}
			//Enable check for Suspect Peak Power High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, PEAKPOWER_LIMIT_PL))
			{ 	
				if (CP->m_RecipeSC.ValidateSuspectLimit(PEAKPOWER_LIMIT_PL, CP->weldResult.Get(WeldResults::PEAK_POWER)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_PEAKPOWER_LIMIT_PLS_PSA);
			}
		}
		//Individual enable check for Suspect Absolute Distance
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_ABSOLUTE_EN))
		{
			//Enable check for Suspect Absolute Distance Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, ABSOLUTE_LIMIT_ML))
			{ 	
				if (CP->m_RecipeSC.ValidateSuspectLimit(ABSOLUTE_LIMIT_ML, CP->weldResult.Get(WeldResults::TOTAL_ABSOLUTE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ABSOLUTEDIST_LIMIT_MLS_PSA);
			}
			//Enable check for Suspect Absolute Distance High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, ABSOLUTE_LIMIT_PL))
			{ 	
				if (CP->m_RecipeSC.ValidateSuspectLimit(ABSOLUTE_LIMIT_PL, CP->weldResult.Get(WeldResults::TOTAL_ABSOLUTE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ABSOLUTEDIST_LIMIT_PLS_PSA );
			}
		}
		//Individual enable check for Suspect Collapse Distance
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_COLLAPSE_EN))
		{
			//Enable check for Suspect Collapse Distance Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, COLLAPSE_LIMIT_ML))
			{ 
				if (CP->m_RecipeSC.ValidateSuspectLimit(COLLAPSE_LIMIT_ML, CP->weldResult.Get(WeldResults::TOTAL_COLLAPSE_DIST)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_COLLAPSEDIST_LIMIT_MLS_PSA);
			}
			//Enable check for Suspect Collapse Distance High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, COLLAPSE_LIMIT_PL))
			{ 
				if (CP->m_RecipeSC.ValidateSuspectLimit(COLLAPSE_LIMIT_PL, CP->weldResult.Get(WeldResults::TOTAL_COLLAPSE_DIST)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_COLLAPSEDIST_LIMIT_PLS_PSA);
			}
		}
		//Individual enable check for Suspect Trigger Distance
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_TRIGGER_EN))
		{
			//Enable check for Suspect Trigger Distance Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, TRIGGER_LIMIT_ML))
			{ 				 
				if ( CP->m_RecipeSC.ValidateSuspectLimit(TRIGGER_LIMIT_ML, CP->DistanceStart))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TRIGGERDIST_LIMIT_MLS_PSA);
			}
			//Enable check for Suspect Trigger Distance High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, TRIGGER_LIMIT_PL))
			{ 
				if (CP->m_RecipeSC.ValidateSuspectLimit(TRIGGER_LIMIT_PL, CP->DistanceStart)) 
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_TIRGGERDIST_LIMIT_PLS_PSA);
			}
		}
		//Individual enable check for Suspect Weld Force
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_WELDFORCE_EN))
		{
			//Enable check for Suspect Weld Force Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, WELDFORCE_LIMIT_ML))
			{ 
				if (CP->m_RecipeSC.ValidateSuspectLimit(WELDFORCE_LIMIT_ML, CP->weldResult.Get(WeldResults::MAX_WELD_FORCE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELDFORCE_LIMIT_MLS_PSA);
			}
			//Enable check for Suspect Weld Force High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, WELDFORCE_LIMIT_PL))
			{			 
				if (CP->m_RecipeSC.ValidateSuspectLimit(WELDFORCE_LIMIT_PL, CP->weldResult.Get(WeldResults::MAX_WELD_FORCE)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELDFORCE_LIMIT_PLS_PSA);
			}
		}
		//Individual enable check for Suspect Frequency
		if(CP->m_RecipeSC.IsSRCheckEnabled(SUSPECT_LIMIT, SR_FREQUENCY_EN))
		{
			//Enable check for Suspect Frequency Low
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, FREQUENCY_LIMIT_ML))
			{				 
				if (CP->m_RecipeSC.ValidateSuspectLimit(FREQUENCY_LIMIT_ML, CP->weldResult.Get(WeldResults::START_FREQ)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_LIMIT_MLS_PSA);
			}
			//Enable check for Suspect Frequency High
			if (CP->m_RecipeSC.IsLimitCheckEnabled(SUSPECT_LIMIT, FREQUENCY_LIMIT_PL))
			{
				if (CP->m_RecipeSC.ValidateSuspectLimit(FREQUENCY_LIMIT_PL, CP->weldResult.Get(WeldResults::START_FREQ)))
					AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_FREQUENCY_LIMIT_PLS_PSA ); 
			}
		}
	}  
}
