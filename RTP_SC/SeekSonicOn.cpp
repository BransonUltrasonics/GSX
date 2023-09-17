/**
 * SeekSonicOn.cpp
 *
 *  Created on: May 31, 2017
 *      Author: lzhu
 */

#include "Common.h"
#include "SeekSonicOn.h"
#include "ControlTask.h"
#include "MainTask.h"
#include "AlarmManager.h"

/**************************************************************************//**
* 
* \brief   - SeekSonicon Class constructor.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
SeekSonicOn::SeekSonicOn()
{
	stateType = SEEK_SONIC_ON;
}

/**************************************************************************//**
* 
* \brief   - SeekSonicOn Class destructor.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
SeekSonicOn::~SeekSonicOn()
{
}

/**************************************************************************//**
* 
* \brief   - This method will be executed when entering the SeekSonicOn state .
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void SeekSonicOn::Enter()
{
#ifdef DEBUG
	LOGDBG("Enter seeksonic on",0,0,0);
#endif

	sonicsDelayCounter		= 0;
	SeekTimeCounter			= 0;
	Index 					= 0;
	EndFrequency			= 0;
	MemOffset				= 0;
	UINT32 SeekTimeRecipe 	= CP->m_SeekRecipeObj.GetSeekTime();
	SeekTime 				= SeekTimeRecipe*(1000/ECAT_CYCLE_TIME_IN_US);
	
	LOGDBG("SeekSonic : SeekTime : %d",SeekTime,0,0);
	
	PC_RX->Amplitudechange 	= CP->m_RecipeSC.InitialAmplitude();
#ifdef DEBUG
	LOGDBG("PC_RX->Amplitudechange : %d",PC_RX->Amplitudechange,0,0);
#endif
	
	if(NULL != CP->seekSignature)
	{
		delete [] (CP->seekSignature);
		CP->seekSignature = NULL;
	}
	CP->seekSignature = new SeekSignature[SeekTime+1];  /*SIZE_SEEKBUFFER*/
	CP->NoOfSamples = SeekTime;
   
   LOGDBG("digital tune--------=%d", CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE),0,0);
   LOGDBG("the memory offset before seek reset = %d",CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET),0,0);
   LOGDBG("=========value of mem %d", CP->MemClearBeforeSeek,0,0);
   
    if (CP->MemClearBeforeSeek == 1)
    {
    	 CP->ClearMemoryFrequency();    
    }
    
    LOGDBG("the memory offset after seek reset = %d", CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET),0,0);

    // set output for SEEK 
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_HORN_SEEK, DigitalIO::LINE_HIGH);
}

/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us while waiting for the.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
void SeekSonicOn::Loop()
{
		
	if(AC_TX->ACState == AC_ESTOP)
	{
		AlarmMgr::EnterAlarmEvent (ALARM_AC, ALARM_ESTOP_NCA);
		ChangeState(SC_ALARM);
	}
	else if(PC_TX->PCState == PC_ALARM)
	{
		AlarmMgr::EnterAlarmEvent (ALARM_PC);
		ChangeState(SC_ALARM);
		
		if(NULL != CP->seekSignature)
		{
			delete [] (CP->seekSignature);
			CP->seekSignature = NULL;
		}
	}
	else
	{
		if(PC_TX->PCState == SEEK_RUN)
		{ 
			SeekTimeCounter = SeekTimeCounter+1;
		
			if(CP->seekSignature != NULL)
			{
				CP->seekSignature[Index].Amplitude 	= PC_TX->Amplitude;
				CP->seekSignature[Index].Power	 	= PC_TX->Power;
				CP->seekSignature[Index].Current 	= PC_TX->Current/100;
				CP->seekSignature[Index].Frequency 	= PC_TX->Frequency/10;
				CP->seekSignature[Index].Phase 	 	= PC_TX->Phase/10;
				CP->seekSignature[Index].Time 	 	= (float)SeekTimeCounter/4;
				if(SeekTime >= Index)
					Index++;
			}
			else
			{
				//TODO handle if CP->seekSignature is NULL for any reason
			}
			
			if(SeekTimeCounter < SeekTime)
			{	   
				if (SeekTimeCounter == 1)
				{
					CommonProperty::seekResult.Set(SeekResults::START_FREQ, PC_TX->Frequency);
				}

				if (CommonProperty::seekResult.Get(SeekResults::PEAK_POWER) < PC_TX->Power)
				{
					CommonProperty::seekResult.Set(SeekResults::PEAK_POWER, PC_TX->Power);	
				}   
			}
			else if(SeekTimeCounter >= SeekTime)
			{
				ControlTask::SetResultData(true);	/* Flag set for collecting seek result */
				if(TRUE == (ControlTask::GetWeldOpFlag()))
				{
					if((TRUE == (CP->m_RecipeSC.IsPostWeldSeekEnabled())) && (TRUE == (ControlTask::GetPostWeldDoneFlag())))
					{
						ChangeState(PREREADY);
						return;
					}
					else
					{
						if(TRUE == (CP->m_RecipeSC.IsPreWeldSeekEnabled()))
						{							
							ChangeState(WAIT_FOR_TRIGGER);
							return;
						}
						else
						{
							ChangeState(PREREADY);
							return;
						}
					}
					
				}
				else
				{
					ChangeState(PREREADY);
					return;
				}

			}
		}
		else //PC hasn't turned on sonics
		{
			if(++sonicsDelayCounter > MAX_SONICS_DELAY)
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_SONICS_DELAY_TIMEOUT_NCA);
		}
	}
}

/**************************************************************************//**
* 
* \brief  - This method will be called when exiting the SeekSonicOn state 
*
* \param  - None.
*
* \return  - None
*
******************************************************************************/
void SeekSonicOn::Exit()
{
	INT32 FrequencyChange;
	UINT32 NewStartFrequency;
	
	CP->seekResult.Set(SeekResults::END_FREQ, PC_TX->Frequency);
	// send it to the event log
	EventLog (SEEK_PERFORM, CP->seekResult.Get(SeekResults::START_FREQ), CP->seekResult.Get(SeekResults::END_FREQ));
	
	UpdateMemOffSet();
	
	FrequencyChange = CP->seekResult.Get(SeekResults::START_FREQ)- CommonProperty::seekResult.Get(SeekResults::END_FREQ);

	CP->seekResult.Set(SeekResults::FREQ_CHANGE, FrequencyChange);
	
	if (CP->stackRecipe.GetData(StackRecipe::INTERNAL_OFFSET_FLAG)==1)
    {
		NewStartFrequency = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)+CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET)+CP->stackRecipe.GetData(StackRecipe::INTERNAL_FREQ_OFFSET);
    }
	else
	{
		NewStartFrequency = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)+CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET);
	}
	
	LOGDBG("new start frequency in seek = %d", NewStartFrequency,0,0);
	CP->recipe["StartFrequency"] = NewStartFrequency;
	
	// reset output for SEEK
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_HORN_SEEK, DigitalIO::LINE_LOW);

#ifdef DEBUG
	LOGDBG("Exit Seek Sonic",0,0,0);
#endif
}

/**************************************************************************//**
* 
* \brief   - This function updates the MemOffSet for the system. 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void SeekSonicOn::UpdateMemOffSet()
{
	UINT16 Count;   
	UINT32 Memory;
	
  	EndFrequency= PC_TX->Frequency;
#ifdef DEBUG
	LOGDBG("EndFrequency = %d", EndFrequency,0,0);
	LOGDBG("Digital tune = %d", CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE),0,0);
#endif	

	if (PC_TX->PCState!=PC_ALARM)
	{
		MemOffset = EndFrequency - (CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));  
	}
	else
	{
		MemOffset = 0;
	}
	
	CP->stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, MemOffset);
	Memory = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE) + MemOffset;
	LOGDBG("--------------this is memoryvalue = %d, = %d", Memory, MemOffset,0);
	CP->stackRecipe.SetData(StackRecipe::MEMORY, Memory);
	CP->recipe["MemoryOffset"] = MemOffset;
	CP->m_RecipePC.UpdateMemoryOffset(MemOffset);
	CP->m_RecipePC.UpdateResonantFreq(EndFrequency);

#ifdef DEBUG
	LOGDBG("MemOffset = %d", MemOffset,0,0);
#endif
}

/**************************************************************************//**
* \brief   - Prepares log information for specific requests to be logged in
* 			the event log, and sends it to the DB task for further processing
*
* \param   - LOG_EVENT 	eventID
* \param   - UINT32 prevVal
* \param   - UINT32 newVal
*
* \return  - STATUS status
*
******************************************************************************/
STATUS SeekSonicOn::EventLog (LOG_EVENT eventID, UINT32 prevVal, UINT32 newVal)
{
	Message _message;
	string details;
	STATUS _status = OK;

	// add cases here
	switch (eventID)
	{
	case SEEK_PERFORM:
		if (CP->getActiveRecipeInstance().GetRecipeNumber() == 0)
			snprintf(_message.Buffer, sizeof(_message.Buffer),"Identifier:N/A:From:%d:To:%d:" , prevVal/10, newVal/10);
		else
			snprintf(_message.Buffer, sizeof(_message.Buffer),"Identifier:Recipe %d:From:%d:To:%d:" , CP->getActiveRecipeInstance().GetRecipeNumber(), prevVal/10, newVal/10);
		details = _message.Buffer;
		break;
	default:
		_status = ERROR;
		break;
	}

	if (_status != ERROR)
		_status = DataBaseManager::EnterEventLog (eventID, details);
	
	return _status;
}
