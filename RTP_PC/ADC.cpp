/*************************************************************************** 

       Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************
 * ADC.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: hasanchez
 */


#include "SPI.h"
#include <iostream>
#include <vxWorks.h>        //for vxworks main
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <sys\types.h>
#include <taskLib.h>        
#include"PCObjDictionary.h"
#include "DynamicSystemCall.h"
#include "ADC.h"
#include "Common.h"

extern PcRealtimeData *pRtData;
using namespace std;

//Values Actually loaded from the
 ADC_DATA  ADC::ADC_Data;
UINT8  ADC::ReInitRequestFlag;
// ADC values. These values are scaled 
UINT32 ADC::Amplitude;
UINT32 ADC::Power;
UINT32 ADC::DCC;
UINT32 ADC::Phase;
UINT32 ADC::Temperature;


/**************************************************************************//**
*  \brief - This is the constructor of ADC class.
*
*  \param -  None.
*
*  \return -  None.
******************************************************************************/
ADC::ADC()
{
/*
 test 12
 */
}



/**************************************************************************//**
*  \brief - This is the destructor of ADC class.
*
*   \param -  None.
*
*   \return - None.
******************************************************************************/
ADC::~ADC()
{
	
}

/**************************************************************************//**
*  \brief - Send the configuration to the ADC via SPI according the configuration required.
*
*   \param -  None.
*
*   \return -  None.
******************************************************************************/
void ADC::SetADCSequence()
{
	
	UINT16 ADC_Config = ADC_CONFIG;
	UINT16  StrNull=0;
	
	
	SPI::SingleTransfer(ADC_SPI_CHANNEL, (UINT8 *)&ADC_Config, (UINT8 *)ADC_Data.DataADC[0], ADC_LEN);
	SPI::SingleTransfer(ADC_SPI_CHANNEL, (UINT8 *)&StrNull, (UINT8 *)ADC_Data.DataADC[0], ADC_LEN);  //Dummy string	
}


/**************************************************************************//**
*  \brief - Once configured the ADC it is in charge of reading the channels available.
*
*   \param -  None.
*
*   \return -  None.
******************************************************************************/
void ADC::ReadADC()
{
	
	UINT16  StrNull=0;
	
	for(UINT16  Indx = 0; Indx < CHANNELS_AVAILABLE; Indx++)
	{
		SPI::SingleTransfer(ADC_SPI_CHANNEL, (UINT8 *)&StrNull, (UINT8 *)&ADC_Data.DataADC[Indx], ADC_LEN);
		
		//LOG("   Temp =  %x\n",ADC_Data.SingleParam.Temperature);
		ADC_Data.DataADC[Indx] = ADC_RIGHT_SHIFT_BITS(ADC_Data.DataADC[Indx]);
		
	}
	
}


/**************************************************************************//**
*  \brief - Scales ADC readings for the format of 10v-100%.
*
*   \param -  none.
*
*   \return - none.
******************************************************************************/
void ADC::ScaleADCValues()
{
	Amplitude	=	(UINT32)((ADC_Data.SingleParam.Amplitude * ADC_NUM_FACTOR )/ADC_DENOM_FACTOR);
	Power		=	(UINT32)((ADC_Data.SingleParam.Power * ADC_NUM_FACTOR )/ADC_DENOM_FACTOR);
	DCC			=	(UINT32)((ADC_Data.SingleParam.DCC * ADC_NUM_FACTOR )/ADC_DENOM_FACTOR);
	Phase 		=	ADC_Data.SingleParam.Phase;
    Temperature = ADC_Data.SingleParam.Temperature;
}




/**************************************************************************//**
* \brief  -	Read Amplitude through a method of the class ADC
* 
*  \param - 	none.
*
*  \return  -  Raw Amplitude.
******************************************************************************/
UINT32 	ADC::GetAmplitude ()
{
	return (Amplitude);
}


/**************************************************************************//**
* \brief  -	Read Power through a method of the class ADC
* 
*  \param - 	none.
*
*  \return  -  Raw Power.
******************************************************************************/
UINT32 	ADC::GetPower ()
{
	return (Power);
}


/**************************************************************************//**
* \brief  -	Read DCC through a method of the class ADC
* 
*  \param - 	none.
*
*  \return  -  Raw DCC.
******************************************************************************/
UINT32 	ADC::GetDCC ()
{
	return (DCC);
}


/**************************************************************************//**
* \brief  -	Read Phase through a method of the class ADC
* 
*  \param - 	none.
*
*  \return  -  Phase in counts.
******************************************************************************/
UINT32 ADC::GetPhase()
{
	return (Phase);
}


/**************************************************************************//**
* \brief  -	Read the amplitude in terms of percentage
* 
*  \param - 	none.
*
*  \return  -  Amplitude in %.
******************************************************************************/
UINT32 	ADC::GetAmpPercent()
{
	return( Amplitude * SCALING_FACTOR / ADC11BITVALUE);
}


/**************************************************************************//** 
* \brief  -	Read the DCC in terms of percentage
* 
*  \param - 	none.
*
*  \return  -  Current in %.
******************************************************************************/
UINT32 	ADC::GetDCCPercent()
{
	UINT32 Current;
		
    Current=((DCC * SCALING_FACTOR*100) / ADC11BITVALUE);
    
	return Current;
}

/**************************************************************************//**
* \brief  -	Read the total Power
* 
* \param - 	none.
*
* \return  -  Power in watts.
******************************************************************************/
UINT32 	ADC::GetTotalPwr ()
{
	return (Power * PSPower / ADC11BITVALUE);
}

/**************************************************************************//**
*  \brief  - 	Read Phase in degrees units
* 
*  \param -  	none.
*
*  \return  - 	Returns the phase converted in degrees.
*******************************************************************************/
UINT16 ADC::GetPhaseInDeg()
{
	UINT32 TempPhase;
	TempPhase = (Phase * MAX_PHASE_DEGREES)/ ADC11BITVALUE;
	return((UINT16)TempPhase);
}
/**************************************************************************//**
* \brief  - This function reads the current temperature converted in celsius degrees,
*
* \param  - none
*
*
* \return  - Temperature in °C
* 
******************************************************************************/
UINT32 ADC::GetTemperatureInDegC()
{
	UINT32 TempTemperature;
	
	TempTemperature = Temperature * SCALING_FACTOR / ADC11BITVALUE;
	return(TempTemperature);
}

/**************************************************************************//**
* \brief  -   This function is for setting if the ADC sequence of initialization is requested
*
* \param  - UINT8 ReqFlag 
*
*
* \return  - none
*
******************************************************************************/
 void ADC::SetADCReInitRequest(UINT8 ReqFlag)
 {
	 ReInitRequestFlag = ReqFlag;
 }
 
 /**************************************************************************//**
 * \brief  - This function returns the request flag to reinitialize the ADC. It is called in CommFPGATask
 *
 * \param  - none
 *
 *
 * \return  - UINT8 ReInitRequestFlag
 *
 ******************************************************************************/
 UINT8   ADC::IsADCReInitRequested()
 {
	 return ReInitRequestFlag;
 }
