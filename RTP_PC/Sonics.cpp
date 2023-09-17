/*
 * Sonics.cpp
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#include <stdio.h>
#include "Sonics.h"
#include "FPGA.h"
#include "ADC.h"
#include "PCObjDictionary.h"

#include "PCMain.h"
static LogInfo*       InfoLog           = NULL;


UINT32 Sonics::F_Factor;
INT32 Sonics::IntegralFactor;
INT32 Sonics::PhaseValueI;
INT32 Sonics::IntegralValue;

UINT64 Sonics::FinalAmplitude;
UINT16 Sonics::WeldError;
UINT32 Sonics::Midband;
INT32 Sonics::LastAmpDifference;
INT32 Sonics::PWMLastValue;
INT32 Sonics::CurrentPowerFactor; 
INT32 Sonics::PhaseI;
INT32 Sonics::AmplitudeGain;
INT32 Sonics::AmplitudeIntegral;	
UINT8 Sonics::SonicsConfigDoneFlag ;	
UINT32 Sonics::PCSampleTimeStep;
UINT32 Sonics::BlindTime;
UINT16 Sonics::BlindTimeCounter;
UINT8 Sonics::PhaseShift;

/**************************************************************************//**
* \brief  - Sonics constructor
*
* \param  - none
*
*
* \return - none
*
******************************************************************************/
Sonics::Sonics()
{
	
	//Constructor
	PhaseShift = 8;
}


/**************************************************************************//**
* \brief  - Sonics Destructor
*
* \param  - none
*
* \return - none
*
******************************************************************************/
Sonics::~Sonics()
{
	//Destructor
}


/**************************************************************************//**
* \brief  - Sonics Enter
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void Sonics::Enter()
{
    //INT32 Memoryoffset=0;
	
	//if (WeldRecipeObj.PCWeldRecipe.MemoryOffset>10000)
	//{
	 // Memoryoffset=0;
	//}
	//else
	//{
	// Memoryoffset=WeldRecipeObj.PCWeldRecipe.MemoryOffset;
	//}
	//printf("se--%d\n", WeldRecipeObj.PCWeldRecipe.StartFrequency);

	//Enter code
}


/**************************************************************************//**
* \brief  - Sonics Exit
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void Sonics::Exit()
{
	//printf("sx--%d\n", WeldRecipeObj.PCWeldRecipe.StartFrequency);
	//Exit code
}

/**************************************************************************//**
* \brief  - Sonics Loop
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void Sonics::Loop()
{
	//Loop code
}



/**************************************************************************//**
* \brief  - Sonics Control
* 			Here the functionality of the loop for amplitude and frequency 
* 			is  provided.
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void Sonics::Control()
{
	AmplitudeControl();
	
	if(BlindTimeCounter >= BlindTime)
	{
		FrequencyControl();
	}
	else
	{
		BlindTimeCounter++;
	}
}


/**************************************************************************//**
* \brief  - Sonics AmplitudeControl
* 			The amplitude control is implemented here according to the 
* 			control parameters.
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void Sonics::AmplitudeControl()
{
		INT32 AmpDifference;
		UINT32 TempPWMVal;
		UINT32 TempAmp;
		
		TempAmp = ADC::GetAmplitude();
		//LOG("   amp =  %d\n",TempAmp);
		AmpDifference = (INT32)(FinalAmplitude - TempAmp);
		TempPWMVal  = PWMLastValue + ((AmplitudeGain * AmpDifference)) - ((AmplitudeIntegral * LastAmpDifference));
		
		PWMLastValue = TempPWMVal;
		LastAmpDifference = AmpDifference;

		 if(PWMLastValue <= MIN_PWMAMP_VALUE)
		     PWMLastValue = MIN_PWMAMP_VALUE;

		 if(PWMLastValue > MAX_PWMAMP_VALUE)
		      PWMLastValue = MAX_PWMAMP_VALUE;

		 TempPWMVal =(UINT32) PWMLastValue >> BITS_ON_BYTE;
		 
		
		 FPGA::SetPwmVal((UINT16)TempPWMVal);
		 
}


/**************************************************************************//**
* \brief  - Sonics FrequencyControl
* 			The frequency control is implemented here according to the 
* 			control parameters.
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void Sonics::FrequencyControl()
{
	UINT32 TempFreqVal = FPGA::GetFrequency();
	
	INT32 TempPhase = ADC::GetPhase();
	
	if ((FPGA::GetGPI() & FPGA_SIGN) == 0 )
	{
		TempPhase = (-TempPhase);
	}

	if (CurrentPowerFactor == 0)
	{
		CurrentPowerFactor = 1;
	}
	if (PhaseI == 0)
	{
		PhaseI = 1;
	}
		
	PhaseValueI = TempPhase * (((INT32)(ADC::GetDCC()) / CurrentPowerFactor) + 1);
		
	PhaseValueI = (PhaseValueI << PhaseShift );
	PhaseValueI /= PhaseI;
	

	IntegralFactor = IntegralFactor  - PhaseValueI;
	//IntegralFactor = IntegralFactor/PhaseValueI;
	IntegralValue = (IntegralFactor >> BITS_ON_BYTE );
	
	//printf("start Frequency=%d\n", WeldRecipeObj.PCWeldRecipe.StartFrequency);

	//TempFreqVal = IntegralValue + WeldRecipeObj.PCWeldRecipe.StartFrequency;
	
	if(PC_TxPdo.PCState == SEEK_RUN)
	{
		if ((PCStateMachine::masterState==TEST_SONICS_ON))
		{
			TempFreqVal=IntegralValue+TestRecipeObj.PCTestRecipe.DigitalTune+SeekRecipobj.PCSeekRecipe.SeekFreqOffset;	
		}
		else
			TempFreqVal=IntegralValue+WeldRecipeObj.PCWeldRecipe.DigitalTune+SeekRecipobj.PCSeekRecipe.SeekFreqOffset;	
	}
	else if(PC_TxPdo.PCState == WELD_RUN)
	{
		if ((PCStateMachine::masterState==TEST_SONICS_ON))
		{
			TempFreqVal=IntegralValue+TestRecipeObj.PCTestRecipe.DigitalTune+TestRecipeObj.PCTestRecipe.FrequencyOffset;	
		}
		else if((PCStateMachine::masterState == WAIT_FOR_READY_POSITION) &&	(PC_TxPdo.PC_StatusEvent & BIT_MASK(STATUS_AFTERBURST_ENABLE)) == BIT_MASK(STATUS_AFTERBURST_ENABLE))	
		{
			TempFreqVal=IntegralValue+WeldRecipeObj.PCWeldRecipe.DigitalTune;
		}
		else
		{
			TempFreqVal=IntegralValue+WeldRecipeObj.PCWeldRecipe.DigitalTune+WeldRecipeObj.PCWeldRecipe.FrequencyOffset;
		}
	}
	
	FPGA::SetFrequency(TempFreqVal);
		
}

/**************************************************************************//**
* \brief  - Sonics SetDefaultFreqAndFFactor
* 			Function updates the Midband and FFactor according to the
* 	  		frequency of PowerSupply.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void Sonics::SetDefaultFreqAndFFactor()
{

	switch(PSFrequency)
		{

			case FREQ_30KHZ:
				Midband = F30KHZMIDBAND;
				F_Factor = F30KHZF_FACTOR;
				break;
			case FREQ_40KHZ:
				Midband = F40KHZMIDBAND;
				F_Factor = F40KHZF_FACTOR;
				break;
			case FREQ_20KHZ:
			default:
				Midband = F20KHZMIDBAND;
				F_Factor = F20KHZF_FACTOR;
				break;
		}
}


/**************************************************************************//**
* \brief  - GetF_Factor
* 			Gets current Frequency factor.
*
* \param  - None
*
* \return - F_Factor: Current frequency factor
*
******************************************************************************/
UINT32 Sonics::GetF_Factor()
{
	return (F_Factor);
}


/**************************************************************************//**
* \brief  - Get_SonicsConfigDoneFlag
* 			Gets SonicsConfigDoneFlag value.
*
* \param  - None
*
* \return - SonicsConfigDoneFlag
*
******************************************************************************/

UINT8  Sonics::Get_SonicsConfigDoneFlag()
{
	return (SonicsConfigDoneFlag);
}
  
void Sonics::initIntergalfactor()
{
	if ((PCStateMachine::masterState==TEST_SONICS_ON))
		IntegralFactor=TestRecipeObj.PCTestRecipe.MemoryOffset<<8;
	else
		IntegralFactor=WeldRecipeObj.PCWeldRecipe.MemoryOffset<<8;
}
