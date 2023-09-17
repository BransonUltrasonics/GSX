/*
 * SeekResults.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: lzhu
 */

#include "DataBaseManager.h"
#include  "versions.h"
#include "SeekResults.h"

/**************************************************************************//**
* \brief  - Sets parameters at start up (needed for static initialization)
*
* \param  - All results parameters to initialize
*
*
* \return  - None
*
******************************************************************************/
SeekResults::SeekResults () 
{
	results.PeakPower = 0;
	results.FrequencyChange = 0;
	results.StartFrequency = 0;
	results.EndFrequency = 0;
	results.RecipeNumber = 0;
	
	#ifdef DEBUG
		logMsg("DB_T : -----------Constructor called of SeekResult------------\n",0,0,0,0,0,0);
	#endif
}


/**************************************************************************//**
* \brief  - Gets any parameter from the seek results.
*
* \param  - idx - index of specific result to get
*
*
* \return  - Specific result's value
*
******************************************************************************/
UINT32 SeekResults::Get (SEEK_RESULT_IDX idx)
{
	UINT32 result;
	
	switch (idx)
	{
	case SeekResults::START_FREQ:
		result = 		results.StartFrequency;
		break;
	case SeekResults::END_FREQ:
		result = 	results.EndFrequency;
		break;
	case SeekResults::FREQ_CHANGE:
		result = 	results.FrequencyChange;
		break;
	case SeekResults::PEAK_POWER:
		result = 	results.PeakPower;
		break;
	case SeekResults::RECIPE_NUM:
		result = 	results.RecipeNumber;
		break;
	default:
		result = 0;
		break;
	}
	
	return result;
}


/**************************************************************************//**
* \brief  - Saves a specific result from the seek results.
*
* \param  - idx - 	index of specific result to save
* \param  - data - 	value to set specific result to
*
*
* \return  - None
*
******************************************************************************/
void SeekResults::Set (SEEK_RESULT_IDX idx, UINT32 data)
{
	switch (idx)
	{
	case SeekResults::START_FREQ:
		results.StartFrequency = data;
		break;
	case SeekResults::END_FREQ:
		results.EndFrequency = data;
		break;
	case SeekResults::FREQ_CHANGE:
		results.FrequencyChange = data;
		break;
	case SeekResults::PEAK_POWER:
		results.PeakPower = data;
		break;
	case SeekResults::RECIPE_NUM:
		results.RecipeNumber = data;
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Get member values and send into DB for storing. 
*
* \param   - None.
*
* \return  - SeekResult.
******************************************************************************/
SeekResult SeekResults::GetSeekData()
{	
#ifdef PRINT
	printf("------------- SeekResults (GetData) -------------\n"); 
	printf("PeakPower(W)               :  %d\n", results.PeakPower);
	printf("FrequencyChange(10s of Hz) :  %d\n", results.FrequencyChange);
	printf("StartFrequency(10s of Hz)  :  %d\n", results.StartFrequency);
	printf("EndFrequency(10s of Hz)    :  %d\n", results.EndFrequency);
	printf("-------------------------------------------------\n\n");
#endif	
	
	return results;
}

/**************************************************************************//**
* \brief   - Memory release of object.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
SeekResults::~SeekResults()
{
	#ifdef DEBUG
		printf("---------Destructor called of SeekResult------------\n");
	#endif
}




