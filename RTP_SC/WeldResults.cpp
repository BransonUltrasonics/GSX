/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

/*------------------------------ DESCRIPTION --------------------------------

     	 Implemented to organize the weld result related data.
     
----------------------------------------------------------------------------*/

#include "WeldResults.h"
#include "DataBaseManager.h"
#include "versions.h"
#include "DataBaseConfiguration.h"

using namespace std;

/**************************************************************************//**
* \brief  - Sets parameters at start up (needed for static initialization)
*
* \param  - All results parameters to initialize
*
* \return  - None
*
******************************************************************************/
WeldResults::WeldResults () : m_PartIDScanStatus(false)
{
	results.MaxWeldForce 		= 0;
	results.EndHoldForce 		= 0;
	results.TotalAbsolute 		= 0;
	results.Velocity            = 0;
	results.TriggerDistance     = 0;
	results.CycleCounter 		= 0;
	results.RecipeNumber 		= 0;
	results.RecipeRevNumber		= 0;
	results.WeldTime 			= 0;
	results.CycleTime			= 0;
	results.TotalEnergy 		= 0;
	results.PeakPower 			= 0;
	results.StartFrequency 		= 0;
	results.FrequencyChange 	= 0;
	results.WeldCollapseDistance= 0;
	results.HoldCollapseDistance= 0;
	results.TriggerTime 		= 0;
	results.WeldSonicTime 		= 0;
	results.HoldTime 			= 0;
	results.bIsAlarmflag		= false;
	results.WeldStatus			= 0;
	results.WeldMode			= 0;
	results.StackSerialNo		= 0;
	
	SetPartID(DEFAULT_PART_ID);
	strcpy(results.UserName,"0");
}	

/**************************************************************************//**
* 
* \brief  - Gets any parameter from the weld results.
*
* \param  - idx - index of specific result to get
*
* \return  - Specific result's value
*
******************************************************************************/
UINT32 WeldResults::Get (WELD_RESULT_IDX idx)
{
	UINT32 result = 0;
	
	switch (idx)
	{
	case WeldResults::MAX_WELD_FORCE:
		result = 		results.MaxWeldForce;
		break;
	case WeldResults::END_HOLD_FORCE:
		result = 	results.EndHoldForce;
		break;
	case WeldResults::WELD_ABSOLUTE:
		result = 	results.WeldAbsolute;
		break;
	case WeldResults::TOTAL_ABSOLUTE:
		result = 	results.TotalAbsolute;
		break;
	case WeldResults::CYCLE_COUNTER:
		result = 	results.CycleCounter;
		break;
	case WeldResults::RECIPE_NUM:
		result = 	results.RecipeNumber;
		break;
	case WeldResults::WELD_TIME:
		result = 	results.WeldTime;
		break;
	case WeldResults::CYCLE_TIME:
		result = 	results.CycleTime;
		break;
	case WeldResults::TOTAL_ENERGY:
		result = 	results.TotalEnergy;
		break;
	case WeldResults::PEAK_POWER:
		result = 	results.PeakPower;
		break;
	case WeldResults::START_FREQ:
		result = 	results.StartFrequency;
		break;
	case WeldResults::FREQ_CHANGE:
		result = 	results.FrequencyChange;
		break;
	case WeldResults::WELD_COLLAPSE_DIST:
		result =	results.WeldCollapseDistance;
		break;
	case WeldResults::HOLD_COLLAPSE_DIST:
		result =	results.HoldCollapseDistance;
		break;
	case WeldResults::TOTAL_COLLAPSE_DIST:
		result =	results.TotalCollapseDistance;
		break;
	case WeldResults::TRIGGER_TIME:
		result =	results.TriggerTime;
		break;
	case WeldResults::WELD_SONIC_TIME:
		result =	results.WeldSonicTime;
		break;
	case WeldResults::HOLD_TIME:
		result =	results.HoldTime;
		break;
	case WeldResults::TRIGGER_DISTANCE:
	    result =    results.TriggerDistance;
	    break;
	case WeldResults::VELOCITY:
		result =    results.Velocity;
		break;
	case WeldResults::IS_ALARM:
		result = 	results.bIsAlarmflag;
		break;
	case RECIPE_REV_NUM:
		result = 	results.RecipeRevNumber;
		break;
	case WELD_MODE:
		result = 	results.WeldMode;
		break;
	case WELD_STATUS:
		result = 	results.WeldStatus;
		break;
	case STACK_SERIAL_NUM:
		result =	results.StackSerialNo;
		break;
	case WeldResults::WELD_RECIPE_STATUS:
		result = 	results.recipeStatus;
		break;
	default:
		LOGERR("WeldResult : Invalid parameter index passed : %d",idx,0,0);
		break;
	}
	
	return result;
}

/**************************************************************************//**
* \brief  - Saves a specific result from the weld results.
*
* \param  - idx - 	index of specific result to save
* \param  - data - 	value to set specific result to
*
* \return  - None
*
******************************************************************************/
void WeldResults::Set (WELD_RESULT_IDX idx, UINT32 data)
{
	switch (idx)
	{
	case WeldResults::MAX_WELD_FORCE:
		results.MaxWeldForce  			= data;
		break;
	case WeldResults::END_HOLD_FORCE:
		results.EndHoldForce  			= data;
		break;
	case WeldResults::WELD_ABSOLUTE:
		results.WeldAbsolute  			= data;
		break;
	case WeldResults::TOTAL_ABSOLUTE:
		results.TotalAbsolute  			= data;
		break;
	case WeldResults::CYCLE_COUNTER:
		results.CycleCounter  			= data;
		break;
	case WeldResults::RECIPE_NUM:
		results.RecipeNumber  			= data;
		break;
	case WeldResults::WELD_TIME:
		results.WeldTime  				= data;
		break;
	case WeldResults::CYCLE_TIME:
		results.CycleTime  				= data;
		break;
	case WeldResults::TOTAL_ENERGY:
		results.TotalEnergy  			= data;
		break;
	case WeldResults::PEAK_POWER:
		results.PeakPower  				= data;
		break;
	case WeldResults::START_FREQ:
		results.StartFrequency  		= data;
		break;
	case WeldResults::FREQ_CHANGE:
		results.FrequencyChange  		= data;
		break;
	case WeldResults::WELD_COLLAPSE_DIST:
		results.WeldCollapseDistance 	= data;	
		break;
	case WeldResults::HOLD_COLLAPSE_DIST:
		results.HoldCollapseDistance 	= data;
		break;
	case WeldResults::TOTAL_COLLAPSE_DIST:
		results.TotalCollapseDistance	= data;
		break;
	case WeldResults::TRIGGER_TIME:
		results.TriggerTime 			= data;
		break;
	case WeldResults::WELD_SONIC_TIME:
		results.WeldSonicTime 			= data;
		break;
	case WeldResults::HOLD_TIME:
		results.HoldTime 				= data;
		break;
	case WeldResults::TRIGGER_DISTANCE:
	    results.TriggerDistance 		= data;
	    break;
	case WeldResults::VELOCITY:
		results.Velocity 				= data;
		break;
	case WeldResults::IS_ALARM:
		results.bIsAlarmflag 			= data;
		break;
	case RECIPE_REV_NUM:
		results.RecipeRevNumber			= data;
		break;
	case WELD_MODE:
		results.WeldMode				= data;
		break;
	case WELD_STATUS:
		results.WeldStatus				= data;
		break;
	case STACK_SERIAL_NUM:
		results.StackSerialNo			= data;
		break;
	case WeldResults::WELD_RECIPE_STATUS:
		results.recipeStatus 			= data;
		break;
	default:
		LOGERR("WeldResult : Invalid parameter index passed : %d",idx,0,0);
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Clears the previous weld results. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void WeldResults::ResetWeldResult()
{
	results.MaxWeldForce 			= 0;
	results.EndHoldForce 			= 0;
	results.WeldAbsolute 			= 0;
	results.TotalAbsolute 			= 0;
	results.WeldCollapseDistance	= 0;
	results.HoldCollapseDistance	= 0;
	results.TotalCollapseDistance	= 0;
	results.WeldTime 				= 0;
	results.CycleTime 				= 0;
	results.TotalEnergy 			= 0;
	results.PeakPower 				= 0;
	results.StartFrequency 			= 0;
	results.FrequencyChange 		= 0;
	results.TriggerTime 			= 0;
	results.WeldSonicTime 			= 0;
	results.HoldTime 				= 0;
	results.TriggerDistance			= 0;
	results.Velocity				= 0;
	results.bIsAlarmflag			= 0;
	results.WeldStatus				= 0;
}

/**************************************************************************//**
* 
* \brief   - Set the active recipe user name. 
*
* \param   - char* name
*
* \return  - None.
*
******************************************************************************/
void WeldResults::SetUserName(char* name)
{
	strncpy(results.UserName, name, sizeof(results.UserName));
}

/**************************************************************************//**
* 
* \brief   - Returns the weld status.
*
* \param   - UINT8	 - weldStatus(IN), 
* \param   - string& - weldStatusName(Good,Rejected,ThroughPut,Suspect)(OUT)
* \param   - UINT8   - status(OUT)
* 
* \return  - None.
*
******************************************************************************/
void WeldResults::GetWeldStatus(UINT8 weldStatus, string& weldStatusName,UINT8& status)
{
	switch(weldStatus)
	{
		case GOOD:
			weldStatusName = COLUMN_NAME_GOOD;
			status = 1;
			break;
			
		case THROUGHPUT:
			weldStatusName = COLUMN_NAME_THROUGHPUT;
			status = 1;
			break;
			
		case SUSPECT:
			weldStatusName = COLUMN_NAME_SUSPECT;
			status = 1;
			break;
			
		case REJECTED:
			weldStatusName = COLUMN_NAME_REJECT;
			status = 1;
			break;
			
		default:
			status = 0;
			break;
	}
}

/**************************************************************************//**
* 
* \brief   - Returns current user name
*
* \param   - None
*
* \return  - string User name.
*
******************************************************************************/
string WeldResults::GetUserName()
{
	return (const char*)results.UserName;
}

/**************************************************************************//**
* 
* \brief   - Get the member data of WeldResults. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResult WeldResults::GetWeldData()
{
#ifdef DEBUG
	printf("------------- WeldResults -------------\n");
	printf("CycleCounter            :   %6d \n", results.CycleCounter); 
	printf("WeldRecipeNumber        :   %6d \n", results.RecipeNumber);
	printf("StartFrequency(Hz)      :   %6d (Hz) \n", results.StartFrequency);
	printf("FrequencyChange(Hz)     :   %6d (Hz) \n", results.FrequencyChange);	
	printf("WeldTime(ms)            :   %6d (ms) \n", results.WeldTime/1000);
	printf("CycleTime(ms)           :   %6d (ms) \n", results.CycleTime/1000);
	printf("TotalEnergy(J)          :   %6d (J) \n", results.TotalEnergy); 
	printf("WeldPeakPower(W)        :   %6d (W) \n", results.PeakPower);
	printf("MaxWeldForce(N)         :   %6d (N) \n", results.MaxWeldForce);
	printf("MaxHoldForce(N)         :   %6d (N) \n", results.MaxHoldForce);
	printf("EndHoldForce(N)         :   %6d (N) \n", results.EndHoldForce);
	printf("WeldAbsolute(um)        :   %6d (um) \n", results.WeldAbsolute);
	printf("TotalAbsolute(um)       :   %6d (um) \n", results.TotalAbsolute);
	printf("WeldCollapseDistance(um):   %6d (um) \n", results.WeldCollapseDistance);
	printf("HoldCollapseDistance(um):   %6d (um) \n", results.HoldCollapseDistance);
	printf("TotalCollapseDistance(um):   %6d (um) \n", results.TotalCollapseDistance);
	printf("PartContactDistance(um) :   %6d (um) \n", results.PartContactDistance);
	printf("TriggerDistance(um)     :   %6d (um) \n", results.TriggerDistance);
	printf("TriggerTime(ms) 	:   %f (ms) \n", (float)results.TriggerTime/4);
	printf("WeldSonicTime(ms) 	:   %f (ms) \n", (float)results.WeldSonicTime/4);
	printf("HoldTime(ms) 		:   %f (ms) \n", (float)results.HoldTime/4);
	printf("PartID 			:   %s\n", results.PartID);
	printf("-------------------------------------\n");
#endif

	return results;
}


/**************************************************************************//**
* 
* \brief   - Memory release of object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
WeldResults::~WeldResults()
{
}

/**************************************************************************//**
* \brief   - Get Part ID Scan Status
*
* \param   - None
*
* \return  - m_PartIDScanStatus - True(Scanned)/False(Not Scanned)
*
******************************************************************************/
bool WeldResults::GetPartIDScanStatus()
{
	return m_PartIDScanStatus;
}

/**************************************************************************//**
* \brief   - Set Part ID Scan Status
*
* \param   - bool m_PartIDScanStatus
*
* \return  - None
*
******************************************************************************/
void WeldResults::SetPartIDScanStatus(bool PartIDScanStatus)
{
	m_PartIDScanStatus = PartIDScanStatus;
}

/**************************************************************************//**
* \brief   - Get Part ID
*
* \param   - char *bcPartId - Bar code read Part ID to be updated
*
* \return  - None
*
******************************************************************************/
void WeldResults::GetPartID(char *bcPartId)
{	
	memcpy(bcPartId, results.PartID, sizeof(results.PartID));
}

/**************************************************************************//**
* \brief   - Set Part ID 
*
* \param   - char *bcPartId - Part ID read from the bar code
*
* \return  - None
*
******************************************************************************/
void WeldResults::SetPartID(const char *bcPartId)
{		
	memcpy(results.PartID, bcPartId, sizeof(results.PartID));
}
