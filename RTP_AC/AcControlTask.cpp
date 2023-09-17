/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     It contains Real time(PDO) and Non Real time(SDO) data processing functions
 
**********************************************************************************************************/



/* Header files and Macros */
#include "AcControlTask.h"
#include "versions.h"
#include "Eeprom.h"

/* Function definitions */

/*
******************************************************************************

*      Function    :   AcControlTask
*      Description :   Control Task Class Constructor to construct object
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcControlTask :: AcControlTask()

*******************************************************************************
*/

AcControlTask :: AcControlTask()		//Default Constructor
{
	#ifdef DEBUG				
		LOG("------------------------- Default Constructor ------------------------\n\n");			
	#endif	
	// To ensure Prints come only when there is a state change
	masterChanged = RESET;	
}



/*
******************************************************************************

*      Function    :   ~AcControlTask
*      Description :   Control Task Class Destructor to destroy object
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcControlTask :: ~AcControlTask()

*******************************************************************************
*/

AcControlTask :: ~AcControlTask()		//Default Destructor
{
	#ifdef DEBUG			
		LOG("-------------------------- Default Destructor ------------------------\n");
	#endif
}



/*
**********************************************************************************

*      Function    :   ReadRtFromOD
*      Description :   Read OD data
*      Input       :   No arguments
*      Output      :   INT16 - Successful OD Read returns 0
*      Called by   :   AcControlTask :: ReadRtFromOD()

**********************************************************************************
*/

INT16 AcControlTask :: ReadRtFromOD()		//Read Real Time data
{							
	// Read RxPdo Data from Shared Memory(Object Dictionary)
	ObjectDictionary::rpdo.SC_State =  	pRtData->ScState;
	ObjectDictionary::rpdo.AC_ControlEvent = pRtData->AcControlEvent;
	ObjectDictionary::rpdo.AC_Outputs =  	pRtData->AcOutputs;
				
	#ifdef DEBUG	
		static ULONG prev_SC_State;		//Variable for comparison with previous value and print only when Master changes
		if ( (prev_SC_State != ObjectDictionary::rpdo.SC_State) && (ObjectDictionary::rpdo.SC_State <= NUM_OF_SC_STATES) )		//Print only when the Master changes its state within appropriate range			
		{
			masterChanged = SET;						
			AC :: stateMachine.PrintMasterState();
			LOG("------------------- Reading From Object Dictionary -------------------\n");				
			LOG("ScState: 				0x%X\n", ObjectDictionary::rpdo.SC_State);				
			LOG("AcControlevent: 			0x%X\n", ObjectDictionary::rpdo.AC_ControlEvent);				
			LOG("AcOutput: 				0x%X\n", ObjectDictionary::rpdo.AC_Outputs);				
			LOG("---------------------- Object Read Successfully ----------------------\n\n");								
		}
		prev_SC_State = ObjectDictionary::rpdo.SC_State;
	#endif	//PRINT						
	return SUCCESS;
}



/*
**********************************************************************************

*      Function    :   WriteRtToOD
*      Description :   Write OD data
*      Input       :   No arguments
*      Output      :   INT16 - Successful OD Write returns 0
*      Called by   :   AcControlTask :: WriteRtToOD()

**********************************************************************************
*/

INT16 AcControlTask :: WriteRtToOD()		//Write Real Time data
{			
	// Write TxPdo Data to Shared Memory(Object Dictionary)
	pRtData->ActualForce = ObjectDictionary::tpdo.ActualForce;
	pRtData->ActualDistance = ObjectDictionary::tpdo.ActualDistance;
	pRtData->AcStatusEvent = ObjectDictionary::tpdo.ACStatusEvent;
	pRtData->AcState = ObjectDictionary::tpdo.ACState;
	pRtData->AcInputs = ObjectDictionary::tpdo.ACInputs;
		
	#ifdef DEBUG
		if (masterChanged == SET)		
		{									
			AC :: stateMachine.PrintCurrentState();
			LOG("-------------------- Writing to Object Dictionary --------------------\n");			
			LOG("-------------- TxPdo data written to the Local Object is -------------\n");			
			LOG("ActualForce:				0x%X\n", ObjectDictionary::tpdo.ActualForce);			
			LOG("ActualDistance:				0x%X\n", ObjectDictionary::tpdo.ActualDistance);			
			LOG("AcStatusEvent: 				0x%X\n", ObjectDictionary::tpdo.ACStatusEvent);			
			LOG("AcState: 				0x%X\n", ObjectDictionary::tpdo.ACState);			
			LOG("AcInputs: 				0x%X\n", ObjectDictionary::tpdo.ACInputs);			
			LOG("---------------------- Object Write Successful -----------------------\n\n\n");						
			masterChanged = RESET;
		}
	#endif	//PRINT
	return SUCCESS;
}




/*
**********************************************************************************

*      Function    :   ReadWeldRecipeFromOD
*      Description :   Read Weld Recipe data from OD
*      Input       :   No arguments
*      Output      :   INT16 - Successful 'Weld Recipe' Read returns 0
*      Called by   :   AcControlTask :: ReadWeldRecipeFromOD()

**********************************************************************************
*/

INT16 AcControlTask :: ReadWeldRecipeFromOD()		
{	
	INT8 status, errorStatus;						//"R/W" Success status and function return status
	memset (ACObjDict :: acOD.bufferWrecipe, 0, ACObjDict :: acOD.ObjSizeWrecipe);	//Clear buffer content for a fresh read in subsequent cycles	
	status = obj_readSc(WELD_RECIPE_INDEX, SUB_INDEX, ACObjDict :: acOD.ObjSizeWrecipe, ACObjDict :: acOD.pObjEntryWrecipe, (USHORT MBXMEM *)ACObjDict :: acOD.bufferWrecipe, COMPLETE_ACCESS);	//Read the Weld Recipe data to buffer		
	if (status == SUCCESS)
	{					
		memcpy(&ObjectDictionary::AcWeldRecipe, ACObjDict :: acOD.bufferWrecipe, sizeof(ObjectDictionary::AcWeldRecipe));
		#ifdef DEBUG
			LOG("------------- Reading Weld Recipe From Object Dictionary -------------\n");						
			ACLog :: printInfo.printWRecipe(ObjectDictionary :: AcWeldRecipe);
			LOG("------------------- Weld Recipe Read Successfully -------------------\n\n\n");			
		#endif	//PRINT	
		errorStatus = SUCCESS;
	}
	else
	{
		#ifdef ERROR_PRINT
			LOG("\nFailure in Object Read!!!\n");					
		#endif				
		errorStatus = FAILURE;
	}		
	return errorStatus;	
}



/*
**********************************************************************************

*      Function    :   ProcessEvent
*      Description :   Process the received event
*      Input       :   No argument
*      Output      :   No return type
*      Called by   :   AcControlTask :: ProcessEvent()

**********************************************************************************
*/

void AcControlTask :: ProcessEvent()		 
{					
	if(this->ReadRtFromOD() == FAILURE)
	{
		exit(FAILURE);
	}
	
	//Process the RxPdo data received from the Master(SC)
	AC :: stateMachine.RunStateMachine();		
	
	if(this->WriteRtToOD() == FAILURE)
	{
		exit(FAILURE);
	}	
}

/*
**********************************************************************************

*      Function    :   	WriteFirmwareInfoToOD
*      Description :   	Writes firmware info data to OD that is stored locally
*						in response to master request
*      Input       :   	No arguments
*      Output      :   	INT16 - SUCCESS if successful, FAILURE otherwise.
*      Called by   :   	AcControlTask

**********************************************************************************
*/
INT16 AcControlTask :: WriteFirmwareInfoToOD ()
{					
	/*Changed By MOUNIKA*/
	// Write the info data to Object Dictionary
	UINT32 arrFWInfo[NUM_OF_FW_INFO_ENTRY] = {FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
										FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
										FWVersion::Get(FW_VERSION_AC, VERSION_BUILD),
										FWCrc::Get(FW_CRC_AC),
										0};
	// Writing the firmware data to the OD using ESS API
	obj_writeSc(FIRMWARE_INFO_INDEX, arrFWInfo, ACObjDict :: acOD.offsetFWInfo, ACObjDict :: acOD.FWInfoSizes, NUM_OF_FW_INFO_ENTRY);
						
	#ifdef DEBUG	
		LOG("-------------- Firmware info copied to Object Dictionary --------------\n");		
		LOG("Firmware Major: %d\n", FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR));
		LOG("Firmware Minor: %d\n", FWVersion::Get(FW_VERSION_AC, VERSION_MINOR));
		LOG("Firmware Build: %d\n", FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
	#endif		

	return SUCCESS;
}

/***********************************************************************************
*      Function    :   	WriteCalibrationTimeStampFromEepromToOD
*      Description :   	Writes calibration data to OD that is stored locally
*						in response to master request
*      Input       :   	No arguments
*      Output      :   	INT16 - SUCCESS if successful, FAILURE otherwise.
*      Called by   :   	AcControlTask

***********************************************************************************/
INT16 AcControlTask :: WriteCalibrationTimeStampFromEepromToOD ()
{
	Eeprom epclass;
	char eeprom_sig;
	
	CalibrationReadings calReadings;
	UINT8 CalibrationSizes[NUM_OF_CALIBRATION_ENTRY] = {4,4,2,2,2,2,2,2,2,2,2};
	
	if(ERROR == epclass.Read(&eeprom_sig,CALIBRATIONTIME_IDENTIFIER_SIZE,CALIBRATIONTIME_BASE))
	{
		LOG("Reading calibration data from eeprom failed\n");
		return FAILURE;	
	}
	if (eeprom_sig != CALIBRATIONTIME_SIG)            /*Checks Eeprom is empty*/
	{
		eeprom_sig = CALIBRATIONTIME_SIG;
		epclass.Write(&eeprom_sig,CALIBRATIONTIME_IDENTIFIER_SIZE,CALIBRATIONTIME_BASE);
		calReadings.FirstReading = 0;
		calReadings.SecondReading = 0;
		epclass.Write((char *)&calReadings,sizeof(calReadings),CALIBRATIONTIME_ADDR);
	}
	
	if(ERROR == epclass.Read((char *)&calReadings,sizeof(calReadings),CALIBRATIONTIME_ADDR))
	{
		LOG("Reading calibration data from eeprom failed\n");
		return FAILURE;
	}	
	// Write the info data to Object Dictionary
	UINT32 arrCalibration[NUM_OF_CALIBRATION_ENTRY] = {calReadings.FirstReading,calReadings.SecondReading,calReadings.tm_sec,calReadings.tm_min,
												calReadings.tm_hour,calReadings.tm_mday,calReadings.tm_mon,calReadings.tm_year,calReadings.tm_wday,
												calReadings.tm_yday,calReadings.tm_isdst};
	
	// Writing the Calibration data to the OD using ESS API
	if(!obj_writeSc(CALIBRATION_READINGS_INDEX, arrCalibration, ACObjDict :: acOD.offsetCalibration, CalibrationSizes, NUM_OF_CALIBRATION_ENTRY))
	{	
		return SUCCESS;
	}
	else
	{
		LOG("Writing calibration data to OD failed\n");
		return FAILURE;
	}
}

/**************************************************************************//**
* \brief  - Coppies the calibration timestamp form the OD to eeprom
*
* \param  - CalibrationReadings
*
*
* \return  - SUCCESS if successful, FAILURE otherwise.
*
******************************************************************************/
INT16 AcControlTask :: WriteCalibrationTimeStampToEeprom(CalibrationReadings calReadings)
{
	Eeprom epclass;
	if(epclass.Write((char *)&calReadings,sizeof(calReadings),CALIBRATIONTIME_ADDR))
		return SUCCESS;
	else
		return FAILURE;
}
/**************************************************************************//**
* \brief  - Coppies the calibration readings object form the OD to ObjectDictionary::calReadings
*
* \param  - none
*
*
* \return  - SUCCESS if successful, FAILURE otherwise.
*
******************************************************************************/
INT16 AcControlTask :: ReadCalibrationReadingsFromOD()		
{	
	INT8 status, errorStatus;						//"R/W" Success status and function return status
	memset (ACObjDict :: acOD.bufferCalibration, 0, ACObjDict :: acOD.ObjSizeCalibration);	//Clear buffer content for a fresh read in subsequent cycles	
	status = obj_readSc(CALIBRATION_READINGS_INDEX, SUB_INDEX, ACObjDict :: acOD.ObjSizeCalibration, ACObjDict :: acOD.pObjEntryCalibration, (USHORT MBXMEM *)ACObjDict :: acOD.bufferCalibration, COMPLETE_ACCESS);	//Read the Weld Recipe data to buffer		
	if (status == SUCCESS)
	{					
		memcpy(&ObjectDictionary::calReadings, ACObjDict :: acOD.bufferCalibration, sizeof(ObjectDictionary::calReadings));
		
		if(FAILURE == WriteCalibrationTimeStampToEeprom(ObjectDictionary::calReadings))
		{
			errorStatus = FAILURE;
			LOG("Write calibration data to eeprom failed\n");
		} else
		{

#ifdef PRINT
			LOG("------------- Reading Calibration Readings From Object Dictionary -------------\n");	
			cout << "First Reading: " << ObjectDictionary::calReadings.FirstReading << endl;
			cout << "Second Reading: " << ObjectDictionary::calReadings.SecondReading << endl;
			LOG("------------------- Calibration Readings Read Successfully -------------------\n\n\n");			
#endif	//PRINT	
			errorStatus = SUCCESS;
		}
	}
	else
	{
		#ifdef ERROR_PRINT
			LOG("\nFailure in Object Read!!!\n");					
		#endif				
		errorStatus = FAILURE;
	}		
	return errorStatus;	
}
