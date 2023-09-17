/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Object Dictionary function definitions
 
**********************************************************************************************************/


/* Header files */
#include "ACOD.h"

/* Function definitions */
/*		
***************************************************************************************************

*      Function    :   ACOD
*      Description :   AC Object Dictionary Class Constructor to allocate memory for R/W Operation
*      Input       :   No argument
*      Output      :   No return type
*      Called by   :   ACOD :: ACOD

***************************************************************************************************
*/

ACOD :: ACOD()
{
	/* Allocate buffer memory for Real time data and Non real time data*/
	
	//To store the Read data in a buffer
	bufferRt = (USHORT MBXMEM*)malloc(ObjSizeRt);
	
	//To store the Weld Recipe data in a buffer
	bufferWrecipe = (USHORT MBXMEM*)malloc(ObjSizeWrecipe);	
	
	//To store the Calibration data in a buffer
	bufferCalibration = (USHORT MBXMEM*)malloc(ObjSizeCalibration);		
	/*To store the Firmware data in to a buffer*/
	bufferFwUpgrade = (USHORT MBXMEM*)malloc(ObjSizeFWUPGRDInfo);
	if (bufferFwUpgrade == NULL)
	{
		#ifdef ERROR_PRINT
			LOG("\nbufferFwUpgrade is NULL!!!\n");					
		#endif	
	}
}



/*		
***************************************************************************************************

*      Function    :   ACOD
*      Description :   AC Object Dictionary Class destructor to destroy the object
*      Input       :   No argument
*      Output      :   No return type
*      Called by   :   ACOD :: ~ACOD

***************************************************************************************************
*/

ACOD :: ~ACOD()
{
	// Free the allocated buffer memory of Real time and Non-Real time data
	free(bufferRt);
	free(bufferWrecipe);
	free(bufferCalibration);
	free(bufferFwUpgrade);
}


/*		
*************************************************************************************

*      Function    :   GetODDataProp
*      Description :   Get OD data properties like Offset and Object Size
*      Input       :   No argument
*      Output      :   No return type
*      Called by   :   ACOD :: GetODDataProp

*************************************************************************************
*/
void ACOD :: GetODDataProp()
{		
	UINT16 subIndex, arrIndex, nextArrIndex;
	
	pObjEntryRt = obj_getobjecthandleSc(TX_RX_PDO_INDEX);					//Get the pointer to the Real time data in OD
	if(pObjEntryRt == NULL)
	{
		LOG("Failed to get the pointer to the Real time data OD!!!\n");
		exit(FAILURE);
	}
	
	//Get the pointer to the Weld Recipe data in OD	
	pObjEntryWrecipe = obj_getobjecthandleSc(WELD_RECIPE_INDEX);
	if(pObjEntryWrecipe == NULL)
	{
		LOG("Failed to get the pointer to the Weld Recipe OD!!!\n");
		exit(FAILURE);
	}
	
	//Get the pointer to the Weld Result data in OD
	pObjEntryDiagData = obj_getobjecthandleSc(DIAGNOSTIC_DATA_INDEX);		
	if(pObjEntryDiagData == NULL)
	{
		LOG("Failed to get the pointer to the Diagnostic Data OD!!!\n");
		exit(FAILURE);
	}
	
	//Get the pointer to the firmware info data in OD
	pObjEntryFWInfo = obj_getobjecthandleSc(FIRMWARE_INFO_INDEX);
	if(pObjEntryFWInfo == NULL)
	{
		LOG("Failed to get the pointer to the Firmware Info Data OD!!!\n");
		exit(FAILURE);
	}
	
	//Get the pointer to the caliberation data in OD
	pObjEntryCalibration = obj_getobjecthandleSc(CALIBRATION_READINGS_INDEX);
	if(pObjEntryCalibration == NULL)
	{
		LOG("Failed to get the pointer to the Calibration Readings OD!!!\n");
		exit(FAILURE);
	}		
	
	pObjEntryFWUpgrade = obj_getobjecthandleSc( FIRMWARE_UPGRADE_INDEX );
	if (pObjEntryFWUpgrade == NULL)
	{
		LOG("Failed to get the pointer to the Firmware Upgrade Readings OD!!!\n");				
		exit(FAILURE);	
	}
	//To find the Length of the Real Time Object for RW operation
	ObjSizeRt = obj_getobjectlengthSc(TX_RX_PDO_INDEX, SUB_INDEX, pObjEntryRt, COMPLETE_ACCESS);			
	ObjSizeWrecipe = obj_getobjectlengthSc(WELD_RECIPE_INDEX, SUB_INDEX, pObjEntryWrecipe, COMPLETE_ACCESS);	
	ObjSizeDiagData = obj_getobjectlengthSc(DIAGNOSTIC_DATA_INDEX, SUB_INDEX, pObjEntryDiagData, COMPLETE_ACCESS);	
	ObjSizeFWInfo = obj_getobjectlengthSc(FIRMWARE_INFO_INDEX, SUB_INDEX, pObjEntryFWInfo, COMPLETE_ACCESS);	
	ObjSizeCalibration = obj_getobjectlengthSc(CALIBRATION_READINGS_INDEX, SUB_INDEX, pObjEntryCalibration, COMPLETE_ACCESS);
	ObjSizeFWUPGRDInfo = obj_getobjectlengthSc(FIRMWARE_UPGRADE_INDEX, SUB_INDEX,pObjEntryFWUpgrade, COMPLETE_ACCESS);
		
	//subIndex is Sub index of entries and arrIndex is array index to store the offset
	//Gets the offset of every TxPdo entry and converts it into bytes
	for (subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_TXPDO_ENTRY; subIndex++, arrIndex++)
	{		
		offsetRt[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntryRt) / 8);								
	}
	
	//Gets the offset of every Weld Recipe entry and converts it into bytes
	for (subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_WELD_RECIPE_ENTRY; subIndex++, arrIndex++)			
	{		
		offsetWrecipe[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntryWrecipe) / 8);		 					
	}
		
	//Gets the offset of every Diagnostic Data entry and converts it into bytes
	for (subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_DIAGNOSTIC_DATA_ENTRY; subIndex++, arrIndex++)			
	{
		offsetDiagData[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntryDiagData) / 8);		
	}	
		
	//Gets the offset of every firmware info entry and converts it into bytes
	for (subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_FW_INFO_ENTRY; subIndex++, arrIndex++)			
	{		
		offsetFWInfo[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntryFWInfo) / 8);							
	}	
	for (subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_CALIBRATION_ENTRY; subIndex++, arrIndex++)			
	{
		//Gets the offset of every calibration entry and converts it into bytes
		offsetCalibration[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntryCalibration) / 8);							
	}	
	/* Offset calculation of 8 weld result parameters for OBJ Write operation*/
	for (subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_FWUPGRD_INFO_ENTRY;
			subIndex++, arrIndex++)	
	{
		/*Divide by 8 to convert bits to bytes*/
		offsetFWUpgrade[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntryFWUpgrade) / 8);		
		
	}
	// Calculate the Sizes of TxPdo for Object Write operation
	nextArrIndex = sizeof(char);
	for (arrIndex = 0; arrIndex < NUM_OF_TXPDO_ENTRY; arrIndex++, nextArrIndex++)			
	{
		if(arrIndex == (NUM_OF_TXPDO_ENTRY - 1))
		{
			//Since the Offset entry after last data member will not be calculated, directly size of last TxPdo is taken
			TxPdoSizes[arrIndex] = sizeof(ObjectDictionary::tpdo.ACInputs);		
		}
		else
		{
			TxPdoSizes[arrIndex] = offsetRt[nextArrIndex] - offsetRt[arrIndex];
		}
	}
	
	
	// Calculate the Sizes of Diagnostic Data for Object Write operation
	nextArrIndex = sizeof(char);
	for (arrIndex = 0; arrIndex < NUM_OF_DIAGNOSTIC_DATA_ENTRY; arrIndex++, nextArrIndex++)			
	{
		if(arrIndex == (NUM_OF_DIAGNOSTIC_DATA_ENTRY - 1))
		{
			//Since the Offset entry after last data member will not be calculated, directly size of last Weld Result is taken
			DiagDataSizes[arrIndex] = sizeof(UINT16);
		}
		else
		{
			DiagDataSizes[arrIndex] = offsetDiagData[nextArrIndex] - offsetDiagData[arrIndex];			
		}
	}
			
	
	// Calculate the Sizes of Firmware Information for object Write operation
	nextArrIndex = sizeof(char);
	for (arrIndex = 0; arrIndex < NUM_OF_FW_INFO_ENTRY; arrIndex++, nextArrIndex++)			
	{
		if(arrIndex == (NUM_OF_FW_INFO_ENTRY - 1))
		{
			//Since the Offset entry after last data member will not be calculated, directly size of last Weld Result is taken
			FWInfoSizes[arrIndex] = sizeof(UINT32);		
		}
		else
		{
			FWInfoSizes[arrIndex] = offsetFWInfo[nextArrIndex] - offsetFWInfo[arrIndex];			
		}
	}
	nextArrIndex = sizeof(char);
	for (arrIndex = 0; arrIndex < NUM_OF_FW_INFO_ENTRY; arrIndex++, nextArrIndex++)			
	{
		if(arrIndex == (NUM_OF_FW_INFO_ENTRY - 1))
		{
			//Since the Offset entry after last data member will not be calculated, directly size of last Weld Result is taken
			FWInfoSizes[arrIndex] = sizeof(UINT32);		
		}
		else
		{
			FWInfoSizes[arrIndex] = offsetFWInfo[nextArrIndex] - offsetFWInfo[arrIndex];			
		}
	}
	
}
