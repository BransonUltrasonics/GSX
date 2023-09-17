/*
 * WeldRun.cpp
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#include "WeldRun.h"
#include <iostream>
#include <stdio.h>
#include "PC.h"
#include "FPGA.h"
#include "CommFPGATask.h"
#include "ADC.h"
#include <msgQLib.h>
#include"PCObjDictionary.h"
#include "Commons.h"
#include "SeekRun.h"


using namespace std;


UINT8 WeldRun::RampDownFlag;
UINT64 WeldRun::WeldRampTimer;
UINT32 WeldRun::WeldAmplitudeLimit;
UINT8 WeldRun::WeldRampDoneFlag;
UINT32 WeldRun::TempHDMode;
INT32 WeldRun::PhaseCount;
UINT32 WeldRun::WeldAmplitudeLimitPrev;
UINT32 WeldRun::ddsFrequencyHighWeld;
UINT32 WeldRun::ddsFrequencyLowWeld;
INT32 WeldRun::WeldRamptime;
#ifdef DEBUG_CURRENT_OVERLOAD
//Amplitude data for upto 1second weld ramp (either individual step or cumulative if stepping is enabled)
UINT64 WeldRun::FinalAmplitudedata[4000];
UINT32 WeldRun::WeldAmplitudeLimitdata[4000];
UINT32 WeldRun::i;
#endif //DEBUG_CURRENT_OVERLOAD

extern MSG_Q_ID StubMid;
INT16 StubCounterAlarm, WeldCounter;


/**************************************************************************//**
* \brief  - constructor
*
* \param  - none
*
*
* \return - none
*
******************************************************************************/
WeldRun::WeldRun()
{

}


/**************************************************************************//**
* \brief  - Destructor
*
* \param  - none
*
*
* \return - none
*
******************************************************************************/
WeldRun::~WeldRun()
{
	//Destructor
}



/**************************************************************************//**
* \brief  - WeldRun Enter
*			This function is called before preparing the Weld.The parameters 
*			for the loop are initialized from recipe structure.
*
* \param  - none
*
*
* \return - none
*
******************************************************************************/
void WeldRun::Enter()
{
	 printf ("\nEnter WeldRun\n");

    #ifdef DEBUG
	LOG("WeldRun Enter\n");
    #endif	

	RampDownFlag = FALSE;
	
	//A phase shift of 10 for 20Khz systems was set so the frequency corrects faster
	if(PowerUpData::GetPSFrequency() == FREQ_20KHZ)
		PhaseShift = 10;
	else
		PhaseShift = 8;
	
	if (PCStateMachine::masterState == TEST_SONICS_ON)
	{
		// check for sonics disable event. if set, this will skip the setup, FPGA start, ramping etc.
		// and should wait out this state. 
		if ((PC_RxPdo.MasterEvents & (UINT16) 1 << CTRL_PC_SONIC_DISABLE) == 0)
		{
			PhaseI = TestRecipeObj.PCTestRecipe.PhaseLoop;
			AmplitudeGain = (TestRecipeObj.PCTestRecipe.AmplitudeLoopC1<<BITS_ON_SHORT_WORD)/AMPLOOP_C1FACTOR;
			AmplitudeIntegral = (TestRecipeObj.PCTestRecipe.AmplitudeLoopC2<<BITS_ON_SHORT_WORD)/AMPLOOP_C2FACTOR;
			CurrentPowerFactor =TestRecipeObj.PCTestRecipe.PhaseLoopCF;
			
			//Blind time for Frequency control
			BlindTime = TestRecipeObj.PCTestRecipe.Blindtimeweld;
			BlindTimeCounter = 0;
			
			WeldRampDoneFlag = FALSE;
			
			ddsFrequencyHighWeld= Midband + TestRecipeObj.PCTestRecipe.FrequencyHigh;
			ddsFrequencyLowWeld = Midband - TestRecipeObj.PCTestRecipe.FrequencyLow;		
			
			LOG("WeldRun Enter AC %d\n", PC_RxPdo.Amplitudechange);
			WeldAmplitudeLimit = (PC_RxPdo.Amplitudechange * ADC11BITVALUE) / SCALING_FACTOR;	
			WeldAmplitudeLimitPrev = WeldAmplitudeLimit;
			WeldRamptime = CONV_MILLI_MICROSEC(TestRecipeObj.PCTestRecipe.WELDRamptime);
			FinalAmplitude=0;
			WeldRampTimer=0;
			SonicsConfigDoneFlag = TRUE;
			
			PhaseCount = 0;		
			PCSampleTimeStep = TimerInterval::GetTimerInterval();
		}
	}
	else
	{
		WeldRecipeObj.ReadWeldRecipeFromOD();
		
		FPGA::SetFrequency(WeldRecipeObj.PCWeldRecipe.StartFrequency);
		
		// check for sonics disable event. if set, this will skip the setup, FPGA start, ramping etc.
		// and should wait out this state. 
		if ((PC_RxPdo.MasterEvents & (UINT16) 1 << CTRL_PC_SONIC_DISABLE) == 0)
		{
				#ifdef DEBUG
					LOG("Sonic ON\n");
				#endif	
				
			PhaseI = WeldRecipeObj.PCWeldRecipe.PhaseLoop;
			AmplitudeGain = (WeldRecipeObj.PCWeldRecipe.AmplitudeLoopC1<<BITS_ON_SHORT_WORD)/AMPLOOP_C1FACTOR;
			AmplitudeIntegral = (WeldRecipeObj.PCWeldRecipe.AmplitudeLoopC2<<BITS_ON_SHORT_WORD)/AMPLOOP_C2FACTOR;
			CurrentPowerFactor = WeldRecipeObj.PCWeldRecipe.PhaseLoopCF;
			
			//Blind time for Frequency control
			BlindTime = WeldRecipeObj.PCWeldRecipe.Blindtimeweld;
			BlindTimeCounter = 0;
			
			WeldRampDoneFlag = FALSE;
			FPGA::SetSonicCommand(START);
			FinalAmplitude = 0;
			PWMLastValue = 0;
			LastAmpDifference = 0;
			WeldRampTimer = 0;
			WeldError = NO_ERROR;
			
			ddsFrequencyHighWeld= Midband + WeldRecipeObj.PCWeldRecipe.FrequencyHigh;
			ddsFrequencyLowWeld = Midband - WeldRecipeObj.PCWeldRecipe.FrequencyLow;		
			WeldAmplitudeLimit = (PC_RxPdo.Amplitudechange * ADC11BITVALUE) / SCALING_FACTOR;	
			WeldAmplitudeLimitPrev = WeldAmplitudeLimit;
			WeldRamptime = CONV_MILLI_MICROSEC(WeldRecipeObj.PCWeldRecipe.WELDRamptime);
			FPGA::SetCtrlHD(FPGA_HDCTRL_SONICON);
			TempHDMode = FPGA::GetParameterHD(INX_HD_MODE);
			PhaseCount = 0;
			SonicsConfigDoneFlag = TRUE;
			PC_TxPdo.Frequency = FPGA::GetFrequency();
			PCSampleTimeStep = TimerInterval::GetTimerInterval();
#ifdef DEBUG_CURRENT_OVERLOAD
			i=0;
			memset(FinalAmplitudedata,0, sizeof(FinalAmplitudedata));
			memset(WeldAmplitudeLimitdata,0, sizeof(WeldAmplitudeLimitdata));
#endif //DEBUG_CURRENT_OVERLOAD
		}
		else
		{
				#ifdef DEBUG
					LOG("Sonic OFF\n");
				#endif
		}
		
	}
	initIntergalfactor();
}



/**************************************************************************//**
* \brief  - WeldRun Exit
*			The PWM amplitude is reset to zero and sonics is stopped. 
*
* \param  - none
*
* \return - none
*
******************************************************************************/

void WeldRun::Exit()
{
	SonicsConfigDoneFlag = FALSE; 
	WeldCounter++;
	StubCounterAlarm = 0;
	FPGA::SetSonicCommand(STOP);
	
	//printf("wx--%d\n", WeldRecipeObj.PCWeldRecipe.StartFrequency);
	
	LOG("WeldRun Exit\n");
}



/**************************************************************************//**
* \brief  - WeldRun Loop
*			Here amplitude keeps a constant value if the ramp has been done.
*			The Loop for frequency and amplitude are run through.The welding 
*			cycle is stopped if there is an weld error or Run Input is no 
*			longer available.
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void WeldRun::Loop()
{
	static UINT16 Counter = 0;
	char stubRecvBuffer[100];

	//Loop code
	
    if((WeldError != NO_ERROR) || ( Alarm::GetAlarmFlag() == TRUE) || (PCStateMachine::alarm==TRUE))
	{
    	PC_TxPdo.PCState = PC_ALARM;
		ChangeState(PC_ALARM);
	    Counter = 0;			  
	
#ifdef DEBUG_CURRENT_OVERLOAD
    	LOG("Alarm at pdo %d...Dumping FinalAmplitude and WeldAmplitude @ Alarm\n", i);
    	for(UINT32 index=0; index <= i; index++)
		{
    		LOG("[%d]:%lld, [%d]:%d\n",index,FinalAmplitudedata[index],index,WeldAmplitudeLimitdata[index]);
		}
#endif //DEBUG_CURRENT_OVERLOAD
	}
		  
		  
    if(WeldError == NO_ERROR)
    {
    	if (
    		(PCStateMachine::masterState == HOLD) || 
			(PCStateMachine::masterState == PREREADY) ||
			(PCStateMachine::masterState == SC_ALARM) ||
			(
				(PCStateMachine::masterState == WAIT_FOR_READY_POSITION) &&
				
				(PC_RxPdo.MasterEvents & BIT_MASK(CTRL_AFTERBURST_ENABLE)) != BIT_MASK(CTRL_AFTERBURST_ENABLE)
			)
		)
		{
            #ifdef DEBUG
				LOG("\n Afterburst OFF Received Event by SC \n");
			#endif
			PC_TxPdo.PCState = PC_READY;
			ChangeState(PC_READY);
			Counter = 0;		 		
		}
		else if (
					(PCStateMachine::masterState != WELD_SONIC_ON) && 
					(PCStateMachine::masterState != TEST_SONICS_ON) && 
					(
							(PCStateMachine::masterState != WAIT_FOR_TRIGGER) && 
							(PC_TxPdo.PC_StatusEvent & BIT_MASK(STATUS_PRETRIGGER_ENABLE)) == BIT_MASK(STATUS_PRETRIGGER_ENABLE)
					) && 
					(
							(PCStateMachine::masterState != WAIT_FOR_READY_POSITION) && 
							(PC_TxPdo.PC_StatusEvent & BIT_MASK(STATUS_AFTERBURST_ENABLE)) == BIT_MASK(STATUS_AFTERBURST_ENABLE)
					) 
					
				) 		
		{			  	
			PC_TxPdo.PCState = PC_ALARM;
			ChangeState(PC_ALARM);
			Counter = 0;	
		}					  
	}		
}

/**************************************************************************//**
* \brief  - WeldRamp
*			Once the parameters have been registered .The ramp is started based 
*			upon WeldTimer.In each run, the nominal amplitude is calculated 
*			and regulated.
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void WeldRun::WeldRamp()
{
	
	if(RampDownFlag == TRUE){
		WeldRampTimer -= PCSampleTimeStep;
		FinalAmplitude = ((WeldRampTimer * ADC11BITVALUE) / WeldRamptime);  
		// Check if final amplitude is reached
		if(FinalAmplitude <= WeldAmplitudeLimit){
			WeldRampDoneFlag = TRUE;
			FinalAmplitude = WeldAmplitudeLimit;
		}
	}
	else{
			WeldRampTimer += PCSampleTimeStep;
			FinalAmplitude =((WeldRampTimer * ADC11BITVALUE) / WeldRamptime);
			
			   // Check if final amplitude is reached
			if(FinalAmplitude >= WeldAmplitudeLimit){
				WeldRampDoneFlag = TRUE;
				FinalAmplitude = WeldAmplitudeLimit;
			}
#ifdef DEBUG_CURRENT_OVERLOAD
			else //store amplitude data during ramp up for debugging
			{
				FinalAmplitudedata[i] = FinalAmplitude;
				WeldAmplitudeLimitdata[i] =	WeldAmplitudeLimit;
				i++;
			}
#endif //DEBUG_CURRENT_OVERLOAD
	}
	  
}


/**************************************************************************//**
* \brief  - CalculateRampStep
*			This function compares target amplitude with set amplitude. 
*			If set amplitude changes during weld process, then Amplitude 
*			ramping shall be done. If set amplitude is greater than target 
*			amplitude, then ramp up process shall be done. In the same way if 
*			set amplitude is less than target amplitude, then ramp down process 
*			shall be done..
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void WeldRun::CalculateRampStep(void)
{
	if(WeldRamptime > 0){
		
		if(FinalAmplitude > WeldAmplitudeLimit)
			RampDownFlag = TRUE;
		else
			RampDownFlag = FALSE;

		WeldRampTimer = ((FinalAmplitude * (WeldRamptime)) / ADC11BITVALUE) + PCSampleTimeStep;
	}
}


/**************************************************************************//**
* \brief  - CheckWeldLimits
*			Check the limits of weld 
*			
* \param  - none
*
*
* \return - INT16 RetVal: Error code
*
******************************************************************************/
INT16 WeldRun::CheckWeldLimits()
{
	INT16 RetVal=NO_ERROR;
	INT32 TempPhase;
	if (PCStateMachine::masterState == WELD_SONIC_ON)
	{
		TempPhase = (INT32)ADC::GetPhase();
	
		if (SGN(TempPhase) > WeldRecipeObj.PCWeldRecipe.PhaseLimit)
			PhaseCount += CONVMICRO_MILLI(PCSampleTimeStep);
		else
			PhaseCount = 0;
	
		if (PhaseCount > WeldRecipeObj.PCWeldRecipe.PhaseLimitTime)
		{
			Alarm::SetAlarmReason (ERR_PHASELIMIT_TIMEERROR);
			RetVal = ERR_WELDERROR;
		}
	
		if (FPGA::GetFrequency() > ddsFrequencyHighWeld)
		{
			Alarm::SetAlarmReason (ERR_DDSFREQ_HIGHWELDLIMIT);
			RetVal = ERR_WELDERROR;
		}
			
		if (FPGA::GetFrequency() < ddsFrequencyLowWeld)
		{
			Alarm::SetAlarmReason (ERR_DDSFREQ_LOWWELDLIMIT);
			RetVal = ERR_WELDERROR;
		}
	}
	else if (PCStateMachine::masterState == TEST_SONICS_ON)
	{
		TempPhase = (INT32)ADC::GetPhase();
		
		if (SGN(TempPhase) > TestRecipeObj.PCTestRecipe.PhaseLimit)
			PhaseCount += CONVMICRO_MILLI(PCSampleTimeStep);
		else
			PhaseCount = 0;
		if (PhaseCount > TestRecipeObj.PCTestRecipe.PhaseLimitTime)
		{
			Alarm::SetAlarmReason (ERR_PHASELIMIT_TIMEERROR);
			RetVal = ERR_WELDERROR;
		}
	
		if (FPGA::GetFrequency() > ddsFrequencyHighWeld)
		{
			Alarm::SetAlarmReason (ERR_DDSFREQ_HIGHWELDLIMIT);
			RetVal = ERR_WELDERROR;
		}
			
		if (FPGA::GetFrequency() < ddsFrequencyLowWeld)
		{
			Alarm::SetAlarmReason (ERR_DDSFREQ_LOWWELDLIMIT);
			RetVal = ERR_WELDERROR;
		}
	}
	return RetVal;
}

/**************************************************************************//**
* \brief  - This function executes weld ramp and control function for amplitude 
* 			and frequency. It is called by CommFPGATask 
* 
*			
* \param  - none
*
*
* \return - none
*
******************************************************************************/
void WeldRun::WeldControlLoop()
{
#ifdef DEBUG_CURRENT_OVERLOAD
	if(PC_RxPdo.Amplitudechange > MAX_AMPLITUDE_PERCENTAGE)
	{
		LOG("WeldRun::WeldControlLoop - Incorrect Amplitude from SC: %d - Raising Alarm with ID=3\n", (PC_RxPdo.Amplitudechange));
		Alarm::SetAlarmReason(ERR_CURRENT_OVERLOAD | ERR_WELDERROR);
		return;
	}
#endif //DEBUG_CURRENT_OVERLOAD
	
	WeldAmplitudeLimit = (PC_RxPdo.Amplitudechange * ADC11BITVALUE) / SCALING_FACTOR;
	if(WeldRampDoneFlag == FALSE)
	{
		WeldRamp();
	}
	Control();   //To control amplitude and frequency
	WeldError = CheckWeldLimits();
	if(WeldAmplitudeLimitPrev != WeldAmplitudeLimit)
	{
		WeldAmplitudeLimitPrev = WeldAmplitudeLimit;	
		WeldRampDoneFlag = FALSE;
		CalculateRampStep();
	}
}


