/**********************************************************************************************************

       Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     It contains Real time(PDO) and Non Real time(SDO) data processing functions
 
**********************************************************************************************************/

#include"PCObjDictionary.h"
#include "DynamicSystemCall.h"
#include "Common.h"
#include "versions.h"
#include "PowerUpRecipe.h"
#include "PC.h"
#include "TimerInterval.h"

using namespace std;

extern PcRealtimeData *pRtData;

/******************************************************************************
*      Function    :   TxPDO::sendTxPDOToOD
*      Description :   This function is responsible for sending TxPDO Object Dictionary data to SC.
*      Input       :   void
*      Output      :   void
*      Called by   :   PCControlTask::PCControlTask()
*******************************************************************************/
void TxPDO::sendTxPDOToOD(void)
{	
	static UINT8 PreviousSlaveState = 10;	// for printing only when state change purpose 
	// Send TxPdo to OBject dictionary shared Data memory
	
		pRtData->Amplitude_2 = PC_TxPdo.Amplitude;
		pRtData->Phase 		 = PC_TxPdo.Phase ;
		pRtData->Power		 = PC_TxPdo.Power;
		pRtData->Frequency 	 = PC_TxPdo.Frequency ;
		pRtData->Current 	 = PC_TxPdo.Current ;
		pRtData->PCStatus 	 = PC_TxPdo.PC_StatusEvent;
		pRtData->PCstate 	 = PC_TxPdo.PCState;
	
	
	static UINT8 CurrentState, PrevState=0;	
	if(PreviousSlaveState != PC_TxPdo.PCState)
	{
		#ifdef PRINT
			LOG("\n");
			LOG("---------------------The TxPDO entries are-----------------------\n");
			LOG("   Amplitude 		 =  %d\n",PC_TxPdo.Amplitude);
			LOG("   Phase     		 =  %d\n",PC_TxPdo.Phase);
			LOG("   Power    		 =  %d\n",PC_TxPdo.Power);
			LOG("   Frequency 		 =  %d\n",PC_TxPdo.Frequency);
			LOG("   Current   		 =  %d\n",PC_TxPdo.Current);
			LOG("   PCevent   		 =  %d\n",PC_TxPdo.PC_StatusEvent);
			LOG("   PCstate   		 =  %d\n",PC_TxPdo.PCState);			
			LOG("-------------------Ended TxPDO-----------------------------------\n");
		#endif
	}
	PreviousSlaveState=PC_TxPdo.PCState;
	CurrentState=PC_TxPdo.PCState;
	switch(CurrentState)
	{
	case WELD_RUN:
		if(CurrentState!=PrevState)
		{
			WeldRecipe wrcp;
		}
		break;
		
	case SEEK_RUN:
		if(CurrentState!=PrevState)
		{
			SeekRecipe srcp;			
			PowerUpData powerUpData;
			powerUpData.ReadPowerUpDataFromOD();
		}
		break;
	}
	PrevState=CurrentState;
}


/******************************************************************************
*      Function    :   recieveRxPDOFromO
*      Description :   This function is responsible for receiving RxPDO Object Dictionary data from SC.
*      Input       :   void
*      Output      :   void
*      Called by   :   PCControlTask::PCControlTask()
*******************************************************************************/
void RxPDO::recieveRxPDOFromOD()
{	
	PC_RxPdo.Amplitudechange = pRtData->Amplitude_1;
	PC_RxPdo.MasterEvents = pRtData->MasterEvents;
	PC_RxPdo.MasterState = pRtData->MasterState;
	PC_RxPdo.Frequency = pRtData->Frequency_1;
	
	PCStateMachine::masterState = (SCSTATES)PC_RxPdo.MasterState;
}

/******************************************************************************
*      Function    :   processRxPDO
*      Description :   This function is responsible for invoking State Machine by sending the RxPDO
*                      MasterState value.
*      Input       :   void
*      Output      :   void
*      Called by   :   PCControlTask::PCControlTask()
*******************************************************************************/
void RxPDO::processRxPDO()
{
	PC::stateMachine.RunStateMachine();
}



/******************************************************************************
*      Function    :   TxPDO::sendTxPDOToOD
*      Description :   This function will display WeldRecipe Object Dictionary
*      Input       :   void
*      Output      :   void
*      Called by   :   WeldRun::Enter()
*******************************************************************************/
void WeldRecipe::ReadWeldRecipeFromOD()
{
	OBJCONST TOBJECT OBJMEM * pObjEntry;
	memset(&PCWeldRecipe,0,sizeof(WeldRecipeParam));
	UINT32 WeldRecipeODLength=0;
	UINT8 Status;
	UINT8 SubIndex=0;
	pObjEntry = obj_getobjecthandleSc(INDEX_WELD_RECIPE);	
	if(pObjEntry == NULL)
	{
		LOG("   Failed to get the pointer to Weld Recipe OBJ\n");
		exit(-1);
	}
	WeldRecipeODLength = obj_getobjectlengthSc( INDEX_WELD_RECIPE, SubIndex, pObjEntry, COMPLETE_ACCESS);
	Status= obj_readSc( INDEX_WELD_RECIPE, SubIndex, WeldRecipeODLength, pObjEntry, (UINT16 MBXMEM *) Buffer, COMPLETE_ACCESS );
	UINT8* pBuffer = (UINT8 *)Buffer;
	if(Status==SUCCESS)
	{					
		memcpy(&PCWeldRecipe,pBuffer,sizeof(WeldRecipeParam));
				
		#ifdef PRINT
		LOG("   StartFrequency         = %d\n",PCWeldRecipe.StartFrequency);
		LOG("   memoffset              = %d\n", PCWeldRecipe.MemoryOffset);
		LOG("   DigitalTune            = %d\n", PCWeldRecipe.DigitalTune);
		LOG("   FrequencyOffset        = %d\n", PCWeldRecipe.FrequencyOffset);
		
		LOG("--------------------Weld Recipe----------------------------------\n");
		            LOG("   AmplitudeLoopC1        = %d\n",PCWeldRecipe.AmplitudeLoopC1);
					LOG("   AmplitudeLoopC2        = %d\n",PCWeldRecipe.AmplitudeLoopC2);
					LOG("   PhaseLoop              = %d\n",PCWeldRecipe.PhaseLoop);
					LOG("   PhaseLoopCF            = %d,\n",PCWeldRecipe.PhaseLoopCF);
					LOG("   FrequencyLow           = %d,\n",PCWeldRecipe.FrequencyLow);
					LOG("   FrequencyHigh          = %d,\n",PCWeldRecipe.FrequencyHigh);
					LOG("   PhaseLimitTime         = %d,\n",PCWeldRecipe.PhaseLimitTime);
					LOG("   PhaseLimit             = %d,\n",PCWeldRecipe.PhaseLimit);
					LOG("   ControlMode            = %d,\n",PCWeldRecipe.ControlMode);
					LOG("   BlindtimeSeek          = %d,\n",PCWeldRecipe.BlindtimeSeek);
					LOG("   Blind time weld        = %d,\n",PCWeldRecipe.Blindtimeweld);
					LOG("   DDSSwitchtime          = %d,\n",PCWeldRecipe.DDSSwitchtime);
					LOG("   Parameter7             = %d,\n",PCWeldRecipe.Parameter7);
					LOG("   Parameter8             = %d,\n",PCWeldRecipe.Parameter8);
					LOG("   Parameter9             = %d,\n",PCWeldRecipe.Parameter9);
					LOG("   Parameter10            = %d,\n",PCWeldRecipe.Parameter10);
					LOG("   Parameter11            = %d,\n",PCWeldRecipe.Parameter11);
					LOG("   Parameter12            = %d,\n",PCWeldRecipe.Parameter12);
					LOG("   Parameter13            = %d,\n",PCWeldRecipe.Parameter13);
					LOG("   Parameter14            = %d,\n",PCWeldRecipe.Parameter14);
					LOG("   Parameter15            = %d,\n",PCWeldRecipe.Parameter15);
					LOG("   F_LimitTime            = %d,\n",PCWeldRecipe.F_LimitTime);
					LOG("   AmpProportionalGain    = %d,\n",PCWeldRecipe.AmpProportionalGain);
					LOG("   AmpIntegralGain        = %d,\n",PCWeldRecipe.AmpIntegralGain);
					LOG("   PhaseIntegralGain      = %d,\n",PCWeldRecipe.PhaseIntegralGain);
					LOG("   PhaseProportionalGain  = %d\n",PCWeldRecipe.PhaseProportionalGain);
					LOG("   FrequencyWindowSize    = %d\n",PCWeldRecipe.FrequencyWindowSize);
					LOG("   RFofPhasecontrolloop   = %d\n",PCWeldRecipe.RFofPhasecontrolloop);
					LOG("   PIFrequencyLow         = %d\n",PCWeldRecipe.PIFrequencyLow);
					LOG("   PIFrequencyHigh        = %d\n",PCWeldRecipe.PIFrequencyHigh);
					LOG("   PIPhaseLimitTime       = %d\n",PCWeldRecipe.PIPhaseLimitTime);
					LOG("   PIPhaseLimit           = %d\n",PCWeldRecipe.PIPhaseLimit);
					LOG("   WELDRamptime           = %d\n",PCWeldRecipe.WELDRamptime);
					LOG("   StartFrequency         = %d\n",PCWeldRecipe.StartFrequency);
			LOG("-----------------------------------------------------------------\n");
		#endif
	}
	else
	{
		#ifdef ERROR_PRINT
			LOG("Error reading WeldRecipe from Object Dictionary\n");
		#endif
	}
	memset(Buffer,'\0',GLOBAL_BUFFER_SIZE);	
}

/******************************************************************************
*      Function    :   TxPDO::sendTxPDOToOD
*      Description :   This function reads and displays SeekRecipe from SeekRecipe class.
*      Input       :   void
*      Output      :   void
*      Called by   :   SeekRun::Enter()
*******************************************************************************/
void SeekRecipe::ReadSeekRecipeFromOD()
{
	OBJCONST TOBJECT OBJMEM *pObjEntry;
	UINT32 SeekRecipeODLength;
	UINT8 Status;
	UINT8 SubIndex=0;
	memset(&PCSeekRecipe,0,sizeof(SeekRecipeParam));
	pObjEntry = obj_getobjecthandleSc(INDEX_SEEK_RECIPE);
	if(pObjEntry == NULL)
	{
		LOG("   Failed to get the pointer to Seek Recipe OBJ\n");
		exit(-1);
	}
	SeekRecipeODLength = obj_getobjectlengthSc(INDEX_SEEK_RECIPE, SubIndex, pObjEntry, COMPLETE_ACCESS);
	Status= obj_readSc( INDEX_SEEK_RECIPE, SubIndex, SeekRecipeODLength, pObjEntry, (UINT16 MBXMEM *) Buffer, COMPLETE_ACCESS );
	UINT8* pBuffer = (UINT8 *)Buffer;
	if(Status==SUCCESS)
	{	
		memcpy(&PCSeekRecipe,pBuffer,sizeof(SeekRecipeParam));
		#ifdef PRINT
		LOG("--------------------Seek Recipe----------------------------------\n");
		LOG("   SeekAmplitudeLoopC1   = %d\n",PCSeekRecipe.SeekAmplitudeLoopC1);
		LOG("   SeekAmplitudeLoopC2   = %d\n",PCSeekRecipe.SeekAmplitudeLoopC2);
		LOG("   SeekFreqLimitHigh 	 = %d\n",PCSeekRecipe.SeekFreqLimitHigh);
		LOG("   SeekFreqLimitLow  	 = %d\n",PCSeekRecipe.SeekFreqLimitLow);
		LOG("   SeekPhaseloopI    	 = %d\n",PCSeekRecipe.SeekPhaseloopI);
		LOG("   SeekPhaseloopCF     	 = %d\n",PCSeekRecipe.SeekPhaseloopCF);
		LOG("   SeekRampTime     	 = %d\n",PCSeekRecipe.SeekRampTime);
		LOG("   SeekFreqOffset     	 = %d\n",PCSeekRecipe.SeekFreqOffset);
		LOG("-----------------------------------------------------------------\n");
		#endif
	}
	else
	{
		#ifdef ERROR_PRINT
		LOG("Error:%d, Reading from Seek Recipe\n", Status);
		#endif
	}
	memset(Buffer,'\0',GLOBAL_BUFFER_SIZE);	
}
	



/*
**********************************************************************************

*      Function    :   	VersionInfoSDO
*      Description :   	Get the props to the firmware info data in OD
*      Input       :   	No arguments
*      Output      :   	None
*      Called by   :   	PCControlTask

**********************************************************************************
*/
VersionInfoSDO::VersionInfoSDO (void)
{

}


/*
**********************************************************************************

*      Function    :   	WriteFirmwareInfoToOD
*      Description :   	Writes firmware info data to OD that is stored locally
*						in response to master request
*      Input       :   	No arguments
*      Output      :   	INT16 - SUCCESS if successful, FAILURE otherwise.
*      Called by   :   	PCControlTask

**********************************************************************************
*/
INT16 VersionInfoSDO::WriteFirmwareInfoToOD ()
{	

	OBJCONST TOBJECT OBJMEM * pObjEntry;
	pObjEntry = obj_getobjecthandleSc( FIRMWARE_INFO_INDEX );//FIRMWARE_INFO_INDEX
	UINT16 CurrentOffset,LastOffset=0;
	UINT16 offset[NUM_OF_FW_INFO_ENTRY];
			
	for (UINT16 subIndex = 1, arrIndex = 0; subIndex <= NUM_OF_FW_INFO_ENTRY; subIndex++, arrIndex++)	// Offset calculation of 8 weld result parameters for OBJ Write operation
	{
		offset[arrIndex] = (obj_getentryoffsetSc(subIndex, pObjEntry) / 8);		// Divide by 8 to convert bits to bytes				
	}	
	
	/* Changed By MOUNIKA*/
	UINT32 arr[NUM_OF_FW_INFO_ENTRY] = {FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),FWVersion::Get(FW_VERSION_PC, VERSION_BUILD),FWCrc::Get(FW_CRC_PC),FWInfo::Get(FW_INFO_PC)};
	UINT8 sizes[NUM_OF_FW_INFO_ENTRY] = {2, 2, 2, 2, 4};	
	obj_writeSc( FIRMWARE_INFO_INDEX, arr, offset, sizes, NUM_OF_FW_INFO_ENTRY);//FIRMWARE_INFO_INDEX
	
	return SUCCESS;
}

/******************************************************************************
*      Function    :   TxPDO::sendTxPDOToOD
*      Description :   This function reads and displays TestRecipe
*      Input       :   void
*      Output      :   void
*      Called by   :   WeldRun::Enter()
*******************************************************************************/
void TestRecipe::ReadTestRecipeFromOD()
{
	
	OBJCONST TOBJECT OBJMEM *pObjEntry;
	UINT32 TestRecipeODLength;
	UINT8 Status;
	UINT8 SubIndex=0;
	memset(&PCTestRecipe,0,sizeof(WeldRecipeParam));
	pObjEntry = obj_getobjecthandleSc(INDEX_TEST_RECIPE);
	
	
	if(pObjEntry == NULL)
	{
		LOG("   Failed to get the pointer to Test Recipe OBJ\n");
		exit(-1);
	}


	TestRecipeODLength = obj_getobjectlengthSc(INDEX_TEST_RECIPE, SubIndex, pObjEntry, COMPLETE_ACCESS);
	Status= obj_readSc( INDEX_TEST_RECIPE, SubIndex, TestRecipeODLength, pObjEntry, (UINT16 MBXMEM *) Buffer, COMPLETE_ACCESS );
	UINT8* pBuffer = (UINT8 *)Buffer;
	

	if(Status==SUCCESS)
	{	
		memcpy(&PCTestRecipe,pBuffer,sizeof(WeldRecipeParam));
#ifdef PRINT
				LOG("   StartFrequency         = %d\n",PCTestRecipe.StartFrequency);
				LOG("   memoffset              = %d\n", PCTestRecipe.MemoryOffset);
				LOG("   DigitalTune            = %d\n", PCTestRecipe.DigitalTune);
				LOG("   FrequencyOffset        = %d\n", PCTestRecipe.FrequencyOffset);
				
				LOG("--------------------Weld Recipe----------------------------------\n");
				LOG("   AmplitudeLoopC1        = %d\n",PCTestRecipe.AmplitudeLoopC1);
				LOG("   AmplitudeLoopC2        = %d\n",PCTestRecipe.AmplitudeLoopC2);
				LOG("   PhaseLoop              = %d\n",PCTestRecipe.PhaseLoop);
				LOG("   PhaseLoopCF            = %d,\n",PCTestRecipe.PhaseLoopCF);
				LOG("   FrequencyLow           = %d,\n",PCTestRecipe.FrequencyLow);
				LOG("   FrequencyHigh          = %d,\n",PCTestRecipe.FrequencyHigh);
				LOG("   PhaseLimitTime         = %d,\n",PCTestRecipe.PhaseLimitTime);
				LOG("   PhaseLimit             = %d,\n",PCTestRecipe.PhaseLimit);
				LOG("   ControlMode            = %d,\n",PCTestRecipe.ControlMode);
				LOG("   BlindtimeSeek          = %d,\n",PCTestRecipe.BlindtimeSeek);
				LOG("   Blind time weld        = %d,\n",PCTestRecipe.Blindtimeweld);
				LOG("   DDSSwitchtime          = %d,\n",PCTestRecipe.DDSSwitchtime);
				LOG("   Parameter7             = %d,\n",PCTestRecipe.Parameter7);
				LOG("   Parameter8             = %d,\n",PCTestRecipe.Parameter8);
				LOG("   Parameter9             = %d,\n",PCTestRecipe.Parameter9);
				LOG("   Parameter10            = %d,\n",PCTestRecipe.Parameter10);
				LOG("   Parameter11            = %d,\n",PCTestRecipe.Parameter11);
				LOG("   Parameter12            = %d,\n",PCTestRecipe.Parameter12);
				LOG("   Parameter13            = %d,\n",PCTestRecipe.Parameter13);
				LOG("   Parameter14            = %d,\n",PCTestRecipe.Parameter14);
				LOG("   Parameter15            = %d,\n",PCTestRecipe.Parameter15);
				LOG("   F_LimitTime            = %d,\n",PCTestRecipe.F_LimitTime);
				LOG("   AmpProportionalGain    = %d,\n",PCTestRecipe.AmpProportionalGain);
				LOG("   AmpIntegralGain        = %d,\n",PCTestRecipe.AmpIntegralGain);
				LOG("   PhaseIntegralGain      = %d,\n",PCTestRecipe.PhaseIntegralGain);
				LOG("   PhaseProportionalGain  = %d\n",PCTestRecipe.PhaseProportionalGain);
				LOG("   FrequencyWindowSize    = %d\n",PCTestRecipe.FrequencyWindowSize);
				LOG("   RFofPhasecontrolloop   = %d\n",PCTestRecipe.RFofPhasecontrolloop);
				LOG("   PIFrequencyLow         = %d\n",PCTestRecipe.PIFrequencyLow);
				LOG("   PIFrequencyHigh        = %d\n",PCTestRecipe.PIFrequencyHigh);
				LOG("   PIPhaseLimitTime       = %d\n",PCTestRecipe.PIPhaseLimitTime);
				LOG("   PIPhaseLimit           = %d\n",PCTestRecipe.PIPhaseLimit);
				LOG("   WELDRamptime           = %d\n",PCTestRecipe.WELDRamptime);
				LOG("   StartFrequency         = %d\n",PCTestRecipe.StartFrequency);
				LOG("-----------------------------------------------------------------\n");
#endif
	}
	else
	{
		#ifdef ERROR_PRINT
		LOG("Error:%d, Reading from Test Recipe\n", Status);
		#endif
	}
	memset(Buffer,'\0',GLOBAL_BUFFER_SIZE);	
	
}




