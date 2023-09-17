/*
 * SeekRun.cpp
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#include "SeekRun.h"
#include <stdio.h>
#include "PC.h"
#include "FPGA.h"
#include"PCObjDictionary.h"
#include "Commons.h"

#include "PCMain.h"
static LogInfo*       InfoLog           = NULL;

UINT8 SeekRun::SeekRampDone;
UINT32 SeekRun::SeekRunTimeCounter;
UINT32 SeekRun::SeekTimer;
UINT32 SeekRun::ddsFrequencyHighSeek;
UINT32 SeekRun::ddsFrequencyLowSeek;
INT32 SeekRun::SeekStartFreqOffset;

/**************************************************************************//**
* \brief  - SeekRun constructor
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
SeekRun::SeekRun()
{
	//Constructor
}


/**************************************************************************//**
* \brief  - SeekRun Destructor
*
* \param  - none
*
* \return  - none
*
******************************************************************************/
SeekRun::~SeekRun()
{
	//Destructor
}


/**************************************************************************//**
* \brief  - SeekRun::Enter
*			This function prepares Seek cycle before seek is started.
* \param  - none
*
* \return - none
*
******************************************************************************/
void SeekRun::Enter()
{
	LOG("SeekRun Enter\n");
	//Enter code
	
	//A phase shift of 10 for 20Khz systems was set so the frequency corrects faster
	if(PowerUpData::GetPSFrequency() == FREQ_20KHZ)
		PhaseShift = 10;
	else
		PhaseShift = 8;
	
	if (PCStateMachine::masterState == TEST_SONICS_ON) 
	{
		TestRecipeObj.ReadTestRecipeFromOD();
	}	
	
	switch(PowerUpData::GetPSFrequency())
	{
	case FREQ_40KHZ:
		FPGA::SetFrequency(F40KHZMIDBAND);
		LOG("   StartFrequency         = %d\n", F40KHZMIDBAND);
		break;
	case FREQ_30KHZ:
		FPGA::SetFrequency(F30KHZMIDBAND);
		LOG("   StartFrequency         = %d\n", F30KHZMIDBAND);
		break;
	case FREQ_20KHZ:
	default:
		FPGA::SetFrequency(F20KHZMIDBAND);
		LOG("   StartFrequency         = %d\n", F20KHZMIDBAND);
	}
	
	
#ifdef PRINT	
	LOG("   StartFrequency         = %d\n", WeldRecipeObj.PCWeldRecipe.StartFrequency);
	LOG("   memoffset              = %d\n", WeldRecipeObj.PCWeldRecipe.MemoryOffset);
	LOG("   DigitalTune            = %d\n", WeldRecipeObj.PCWeldRecipe.DigitalTune);
	LOG("   FrequencyOffset        = %d\n", WeldRecipeObj.PCWeldRecipe.FrequencyOffset);
#endif
	
	//Initialize PID constants from preset
    AmplitudeGain = (SeekRecipobj.PCSeekRecipe.SeekAmplitudeLoopC1 << BITS_ON_SHORT_WORD)/AMPLOOP_C1FACTOR;
    AmplitudeIntegral = (SeekRecipobj.PCSeekRecipe.SeekAmplitudeLoopC2 << BITS_ON_SHORT_WORD)/AMPLOOP_C2FACTOR;
    CurrentPowerFactor = SeekRecipobj.PCSeekRecipe.SeekPhaseloopCF;
    PhaseI = SeekRecipobj.PCSeekRecipe.SeekPhaseloopI;
    SeekRecipobj.PCSeekRecipe.SeekRampTime=CONV_MILLI_MICROSEC(SeekRecipobj.PCSeekRecipe.SeekRampTime);
    FinalAmplitude = 0;
    
    //Blind time for Frequency control
	BlindTime = WeldRecipeObj.PCWeldRecipe.BlindtimeSeek;
	BlindTimeCounter = 0;
    
    /* (Set the permitted range Seek) */
	ddsFrequencyHighSeek = Midband + SeekRecipobj.PCSeekRecipe.SeekFreqLimitHigh;
	ddsFrequencyLowSeek = Midband - SeekRecipobj.PCSeekRecipe.SeekFreqLimitLow;
       
    PWMLastValue = 0;
	LastAmpDifference = 0;
	SeekTimer = 0;
	FPGA::SetSonicCommand(START);
	FPGA::SetCtrlHD(FPGA_HDCTRL_SEEK);
	FPGA::SetParameterHD(INX_HD_MODE,0);
	SeekRampDone = FALSE;
	WeldError = NO_ERROR;
	PC_TxPdo.Frequency = FPGA::GetFrequency();
	SonicsConfigDoneFlag = TRUE; 
	PCSampleTimeStep = TimerInterval::GetTimerInterval();
	initIntergalfactor();
	cout << "In seek run enter\n";
}


/**************************************************************************//**
* \brief  - SeekRun::Exit
*			In this function ,Seek is stopped.The function store_seek_memo is 
*			called to store frequency information in memory.		

* \param  - none
*
* \return - none
*
******************************************************************************/
void SeekRun::Exit()
{
	//Exit code
	LOG("SeekRun::Exit\n");

	if (PCStateMachine::masterState != TEST_SONICS_ON || 
		(PC_RxPdo.MasterEvents & BIT_MASK(CTRL_ESTOP)) == BIT_MASK(CTRL_ESTOP))
	{
		SonicsConfigDoneFlag = FALSE;
		PC_TxPdo.Frequency = FPGA::GetFrequency();
		FPGA::SetSonicCommand(STOP);
	}
	
}



/**************************************************************************//**
* \brief  - SeekRun::Loop
*			In this function , after the parameters have been registered the 
*			amplitude will go through the ramp function. In each run, the 
*			nominal amplitude is calculated  and regulated. Once the amplitude 
*			is reached, it will keep the amplitude and wait until supervisor 
*			goes to pre-ready.		
*   		
* \param  - none
*
* \return - none
*
******************************************************************************/
void SeekRun::Loop()
{
	//Loop code

	static UINT16 Counter = 0;
	
	if(WeldError == NO_ERROR)
	{
		//LOG("   amp =  %d\n",Counter2);
		if (PCStateMachine::masterState == PREREADY || PCStateMachine::masterState == WAIT_FOR_TRIGGER )
		{		
			PC_TxPdo.PCState=PC_READY;
			ChangeState(PC_READY);
			Counter = 0;
		}
		else if (PCStateMachine::masterState == TEST_SONICS_ON) 
		{
			if((PC_RxPdo.MasterEvents & BIT_MASK(CTRL_WELDTEST)) == BIT_MASK(CTRL_WELDTEST)) 
			{
				cout<< "EVENT_WELDTEST\n";
				PC_TxPdo.PCState = WELD_RUN;
				ChangeState(WELD_RUN);
				Counter = 0;			  
			}
		}
		else if ((PCStateMachine::masterState != SEEK_SONIC_ON) && (PCStateMachine::masterState != TEST_SONICS_ON))
		{
			PC_TxPdo.PCState = PC_ALARM;
			ChangeState(PC_ALARM);
			Counter = 0;			  
		}
		
	}
	if((WeldError != NO_ERROR) || (Alarm::GetAlarmFlag() == TRUE) || (PCStateMachine::alarm==TRUE))
	{
		PC_TxPdo.PCState = PC_ALARM;
		ChangeState(PC_ALARM);
		Counter = 0;
	}
}


/**************************************************************************//**
* \brief  - SeekRun::SeekRamp
*			In this function, ramp is done when the amplitude reaches the limit 
*			required at certain ramp time.
*			
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void SeekRun::SeekRamp()
{
	
	FinalAmplitude = (SeekTimer * ADC11BITVALUE) / SeekRecipobj.PCSeekRecipe.SeekRampTime;
	SeekTimer += PCSampleTimeStep;	

	//SeekAmplitudeLimit is set to 10%
	UINT64 SeekAmplitudeLimit = (SEEKAMPP_DEFAULT * ADC11BITVALUE) / SCALING_FACTOR;
	if (FinalAmplitude > SeekAmplitudeLimit)	
    {
		FinalAmplitude = SeekAmplitudeLimit;
		SeekRampDone = TRUE;
	}
}


/**************************************************************************//**
* \brief  - SeekRun::CheckSeekLimits
*			Check the Low and High limits of the seek 
*			
* \param  - none
*
*
* \return  - INT16 RetVal: Error code
*
******************************************************************************/
INT16 SeekRun::CheckSeekLimits()
{
	INT16 RetVal=NO_ERROR;
	
	if (FPGA::GetFrequency() > ddsFrequencyHighSeek)
		{
			Alarm::SetAlarmReason (ERR_DDSFREQ_HIGHSEEKLIMIT);
			RetVal = ERR_WELDERROR;
					
		}
		else if (FPGA::GetFrequency() < ddsFrequencyLowSeek)
		{
			Alarm::SetAlarmReason (ERR_DDSFREQ_LOWSEEKLIMIT);	
			RetVal = ERR_WELDERROR;
						
		}
		else
		{
			RetVal = NO_ERROR;
		}
	
	return RetVal;
}


/**************************************************************************//**
* \brief  - This function executes seek ramp and control function for amplitude 
* and frequency. It is called by CommFPGATask 
* 
*			
* \param  - none
*
*
* \return - none
*
******************************************************************************/
void SeekRun::SeekControlLoop()
{
	if(SeekRampDone == FALSE)
	{
		SeekRamp();
	}
	Control();
	WeldError = CheckSeekLimits();
}



