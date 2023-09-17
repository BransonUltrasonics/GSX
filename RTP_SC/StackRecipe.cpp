/*
 * StackRecipe.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: lzhu
 */
#include "StackRecipe.h"
#include "DataBaseManager.h"
#include  "versions.h"

using namespace std;

SCAN_RESULT_IDX HornScanResults::scanResultIdx;


/**************************************************************************//**
* \brief  - Sets parameters at start up (needed for static initialization)
*
* \param  - All stack recipe parameters to initialize
*
*
* \return  - None
*
******************************************************************************/
StackRecipe::StackRecipe()
{
	ScStackRecipe.DigitalTune=0;
	ScStackRecipe.DigitalTuneMemory=0;
	ScStackRecipe.HornScanPassed=false;
	ScStackRecipe.InternalOffset=0;
	ScStackRecipe.MemOffset=0;
	ScStackRecipe.MemoryClear=false;
	ScStackRecipe.SystemMidBand=0;
	ScStackRecipe.ClearMemoryAtPowerUp=false;
	ScStackRecipe.ClearMemoryWithReset=false;
}

/**************************************************************************//**
* \brief  - Memory release of object 
*
* \param  - None
*
*
* \return  - None
*
******************************************************************************/
StackRecipe::~StackRecipe()
{
}

/**************************************************************************//**
* \brief  - Gets any parameter from the stack recipe.
*
* \param  - idx - index of specific stack recipe to get
*
*
* \return  - Specific result's value
*
******************************************************************************/
UINT32 StackRecipe::GetData(Digital_Tune_IDX idx)
{
	UINT32 result;
	
	switch (idx)
	{
	case StackRecipe::DIGITAL_TUNE:
		result = ScStackRecipe.DigitalTune;
		break;
	case StackRecipe::MEMORY_OFFSET:
		result = ScStackRecipe.MemOffset;
		break;
	case StackRecipe::MEMORY_CLEAR:
		result = ScStackRecipe.MemoryClear;
		break;
	case StackRecipe::HORN_SCAN_PASSED:
		result = ScStackRecipe.HornScanPassed;
		break;
	case StackRecipe::MID_BAND:
	    result = ScStackRecipe.SystemMidBand;
		break;
	case StackRecipe::CLEARMEMORY_WITH_RESET:
		result=ScStackRecipe.ClearMemoryWithReset;
		break;
	case StackRecipe::CLEARMEMORY_AT_POWERUP:
		result=ScStackRecipe.ClearMemoryAtPowerUp;
		break;
	case StackRecipe::INTERNAL_OFFSET_FLAG:
		result=ScStackRecipe.InternalOffsetFlag;
		break;
	case StackRecipe::INTERNAL_FREQ_OFFSET:
		result=ScStackRecipe.InternalFreqOffset;
		break;
	case StackRecipe::MEMORY:
		result=ScStackRecipe.Memory;
		break;
	case StackRecipe::END_OF_WELD_STORE:
		result=ScStackRecipe.EndofWeldStore;
		break;
	  
	default:
		result = 0;
		break;
	}
	
	return result;
}


/**************************************************************************//**
* \brief  - Saves a specific data from the stack recipe .
*
* \param  - idx - 	index of specific data to save
* \param  - data - 	value to set specific data to
*
*
* \return  - None
*
******************************************************************************/
void StackRecipe::SetData (Digital_Tune_IDX idx, UINT32 data)
{
	switch (idx)
	{
	case StackRecipe::DIGITAL_TUNE:
		ScStackRecipe.DigitalTune = data;
		break;
	case StackRecipe::MEMORY_OFFSET:
		ScStackRecipe.MemOffset = data;
		break;
	case StackRecipe::MEMORY_CLEAR:
		ScStackRecipe.MemoryClear = data;
		break;
	case StackRecipe::HORN_SCAN_PASSED:
		ScStackRecipe.HornScanPassed=data;
		break;
	case StackRecipe::MID_BAND:
		ScStackRecipe.SystemMidBand=data;
		break;
	case StackRecipe::CLEARMEMORY_WITH_RESET:
		ScStackRecipe.ClearMemoryWithReset=data;
		break;
	case StackRecipe::CLEARMEMORY_AT_POWERUP:
		ScStackRecipe.ClearMemoryAtPowerUp=data;
		break;
	case StackRecipe::INTERNAL_OFFSET_FLAG:
		ScStackRecipe.InternalOffsetFlag=data;
		break;
	case StackRecipe::INTERNAL_FREQ_OFFSET:
		ScStackRecipe.InternalFreqOffset=data;
		break;
	case StackRecipe::MEMORY:
		ScStackRecipe.Memory=data;
		break;
	case StackRecipe::END_OF_WELD_STORE:
		ScStackRecipe.EndofWeldStore=data;
		break;
	    
	}
}






