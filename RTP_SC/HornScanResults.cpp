/*
 * HornScanResults.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: lzhu
 */
#include "HornScanResults.h"
#include "DataBaseManager.h"
#include  "versions.h"
/*
******************************************************************************

*      Function    :   	HornScanResults
*      Description :   	Object construction and initialize of members
*      Input       :   	void
*      Output      :   	void
*      Called by   :  	Creation of Object 

*******************************************************************************
*/
HornScanResults::HornScanResults()
{
	scanResult.FirstSeriesFrequency 		= 0;
	scanResult.MainParallelFrequency 		= 0;
	scanResult.SecondaryParallelFrequency 	= 0;
	scanResult.SecondarySeriesFrequency 	= 0;
	scanResult.TertiaryParallelFrequency 	= 0;
	scanResult.TertiarySeriesFrequency 		= 0;
}

/**************************************************************************//**
* 
* \brief   - Get HornScanResults values to store into DB.
*
* \param   - ENUM type.
*
* \return  - UINT32 - Frequency.
******************************************************************************/
UINT32 HornScanResults::GetResonantFrequency(SCAN_RESULT_IDX idx)
{
	UINT32 Frequency = 0;
	switch (idx)
	{
		case SECOND_PARALLEL_FREQ:
			Frequency = scanResult.SecondaryParallelFrequency;
			break;
			
		case MAIN_PARALLEL_FREQ:
			Frequency = scanResult.MainParallelFrequency;
			break;
			
		case TERT_PARALLEL_FREQ:
			Frequency = scanResult.TertiaryParallelFrequency;
			break;
			
		case FIRST_SERIES_FREQ:
			Frequency = scanResult.FirstSeriesFrequency;
			break;
			
		case SECOND_SERIES_FREQ:
			Frequency = scanResult.SecondarySeriesFrequency;
			break;
			
		case TERT_SERIES_FREQ:
			Frequency = scanResult.TertiarySeriesFrequency;
			break;
			
		default:
			break;
	}
#ifdef DEBUG
	printf("idx : %d, Frequency : %d\n",idx,Frequency);
#endif
	return Frequency;
}

/**************************************************************************//**
* 
* \brief   - Assign HornScanResults values to member data.
*
* \param   - ENUM type, UINT32 Frequency.
*
* \return  - None.
******************************************************************************/
void HornScanResults::SetResonantFrequency(SCAN_RESULT_IDX idx, UINT32 Frequency)
{
#ifdef DEBUG
	printf("idx : %d, Frequency : %d\n",idx,Frequency);
#endif
	switch (idx)
	{
		case SECOND_PARALLEL_FREQ:
			scanResult.SecondaryParallelFrequency 	= (UINT16)Frequency;
			break;
	
		case MAIN_PARALLEL_FREQ:
			scanResult.MainParallelFrequency 		= Frequency;
			break;
	
		case TERT_PARALLEL_FREQ:
			scanResult.TertiaryParallelFrequency 	= Frequency;
			break;
	
		case FIRST_SERIES_FREQ:
			scanResult.FirstSeriesFrequency 		= Frequency;
			break;
	
		case SECOND_SERIES_FREQ:
			scanResult.SecondarySeriesFrequency 	= Frequency;
			break;
	
		case TERT_SERIES_FREQ:
			scanResult.TertiarySeriesFrequency 		= Frequency;
			break;
	
		default:
			break;
	}
}

/**************************************************************************//**
* 
* \brief   - Reset HornScanResult values post each horn scan operation.
*
* \param   - None
*
* \return  - None
******************************************************************************/
void HornScanResults::ResetHornScanResult()
{
	scanResult.FirstSeriesFrequency 		= 0;
	scanResult.MainParallelFrequency 		= 0;
	scanResult.SecondaryParallelFrequency 	= 0;
	scanResult.SecondarySeriesFrequency 	= 0;
	scanResult.TertiaryParallelFrequency 	= 0;
	scanResult.TertiarySeriesFrequency 		= 0;
}

/**************************************************************************//**
* 
* \brief   - Memory release of object.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
HornScanResults::~HornScanResults()
{
}


