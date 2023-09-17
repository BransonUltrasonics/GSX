/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Common property class functions for Real time and Non real time data communication
 
**********************************************************************************************************/


#include "CommonProperty.h"
#include  "versions.h"
#include <fstream>

/* static initializations */
CommonProperty* CommonProperty::m_CPObj	= NULL;
WeldResults CommonProperty::weldResult;
SeekResults CommonProperty::seekResult;

/**************************************************************************//**
* \brief   - Constructor - Read default recipe and load into MAP.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
CommonProperty::CommonProperty()
{
	#ifdef DEBUG
		logMsg("---------CommonProperty constructor called---------",0,0,0,0,0,0);
	#endif

	TimerIntervalRecipe["TmrIntervalPC"] = 0;	
	dispFlagTmrInterval = TimerIntervalRecipe;	
	ReadWriteRecipe(TIMER_INTERVAL_RECIPE,READ);		
	
	recipe["AmplitudeLoopC1"]		=	0;
	recipe["AmplitudeLoopC2"]		=	0;
	recipe["PhaseLoop"]				=	0;
	recipe["PhaseLoopCF"]			=	0;
	recipe["FrequencyLow"]			=	0;
	recipe["FrequencyHigh"]			=	0;
	recipe["PhaseLimitTime"]		=	0;
	recipe["PhaseLimit"]			=	0;
	recipe["ControlMode"]			=	0;
	recipe["BlindtimeSeek"]			=	0;
	recipe["BlindtimeWeld"]			=	0;
	recipe["DDSSwitchtime"]			=	0;
	recipe["Parameter7"]			=	0;
	recipe["Parameter8"]			=	0;
	recipe["Parameter9"]			=	0;
	recipe["Parameter10"]			=	0;
	recipe["Parameter11"]			=	0;
	recipe["Parameter12"]			=	0;
	recipe["Parameter13"]			=	0;
	recipe["Parameter14"]			=	0;
	recipe["Parameter15"]			=	0;
	recipe["FLimitTime"]			=	0;
	recipe["AmpProportionalGain"]	=	0;
	recipe["AmpIntegralGain"]		=	0;
	recipe["PhaseProportionalGain"]	=	0;
	recipe["FrequencyWindowSize"]	=	0;
	recipe["PFofPhasecontrolloop"]	=	0;
	recipe["PIFrequencyLow"]		=	0;
	recipe["PIFrequencyHigh"]		=	0;
	recipe["PIPhaseLimitTime"]		=	0;
	recipe["PIPhaseLimit"]			=	0;
	recipe["WeldAmplitude"]			=	0;
	recipe["WeldMode"]				=   0;	
	recipe["ModeValue"]				=   0;
	recipe["WRampTime"]				= 	0;
	recipe["StartFrequency"]		= 	0;
	recipe["MemoryOffset"]		    = 	0;	
	recipe["MaxWeldTimeout"]		= 	0;
	recipe["CoolingValve"]			= 	0;
	recipe["DigitalTune"]		    = 	0;
	recipe["FrequencyOffset"]		= 	0;
	
	recipe["WeldForce"]				=	0;
	recipe["ForceRampTime"]			=	0;
	recipe["HoldMode"]				=	0;
	recipe["TotalCollapseTarget"] 	= 	0;
	recipe["HoldForce"]				=	0;
	recipe["HoldForceRampTime"]		=	0;	
	recipe["ExpectedPartContactPosition"]	=	0;
	recipe["ReadyPosition"]			=	0;
	recipe["DownAcceleration"]		=	0;
	recipe["DownMaxVelocity"]		=	0;
	recipe["DownDeceleration"]		=	0;
	recipe["ReturnAcceleration"]	=	0;
	recipe["ReturnMaxVelocity"]		=	0;
	recipe["ReturnDeceleration"]	=	0;
	recipe["ExpectedPartContactOffset"] = 0;
	recipe["PartContactWindowMinus"]= 	0;
	recipe["PartContactWindowPlus"] = 	0;
	recipe["NumForceSteps"]			=	0;
	recipe["ForceStepAt"]			=	0;
	recipe["ForceStep1AtValue"]		=	0;
	recipe["ForceStep1Force"]		=	0;
	recipe["ForceStep1RampTime"]	=	0;
	recipe["ForceStep2AtValue"]		=	0;
	recipe["ForceStep2Force"]		=	0;
	recipe["ForceStep2RampTime"]	=	0;
	recipe["ForceStep3AtValue"]		=	0;
	recipe["ForceStep3Force"]		=	0;
	recipe["ForceStep3RampTime"]	=	0;
	recipe["ForceStep4AtValue"]		=	0;
	recipe["ForceStep4Force"]		=	0;
	recipe["ForceStep4RampTime"]	=	0;
	recipe["ForceStep5AtValue"]		=	0;
	recipe["ForceStep5Force"]		=	0;
	recipe["ForceStep5RampTime"]	=	0;
	recipe["ForceStep6AtValue"]		=	0;
	recipe["ForceStep6Force"]		=	0;
	recipe["ForceStep6RampTime"]	=	0;
	recipe["ForceStep7AtValue"]		=	0;
	recipe["ForceStep7Force"]		=	0;
	recipe["ForceStep7RampTime"]	=	0;
	recipe["ForceStep8AtValue"]		=	0;
	recipe["ForceStep8Force"]		=	0;
	recipe["ForceStep8RampTime"]	=	0;
	recipe["ForceStep9AtValue"]		=	0;
	recipe["ForceStep9Force"]		=	0;
	recipe["ForceStep9RampTime"]	=	0;
	recipe["ForceStep10AtValue"]	=	0;
	recipe["ForceStep10Force"]		=	0;
	recipe["ForceStep10RampTime"]	=	0;
	recipe["TriggerDistance"]		=	0;
	recipe["TriggerForce"]			=	0;
	recipe["HoldTime"]				=	0;
	recipe["PretriggerFlag"]		=	0;
	recipe["PretriggerType"]		=	0;
	recipe["PretriggerAmplitude"]	=	0;
	recipe["PretriggerDistance"]	=	0;
	
	recipe["AfterburstFlag"]		=	0;
	recipe["AfterburstDelay"]		=	0;
	recipe["AfterburstAmplitude"]	=	0;
	recipe["AfterburstTime"]		=	0;
	
	recipe["Reactivity"]			=	0;
	recipe["ForceLevelEnable"]		= 	0;
	recipe["ForceLevel"]			=	0;
	recipe["ForceLevelTime"]		=	0;
	
	recipe["ScrubAmplitude"]		=	0;
		
	dispFlag = recipe;
	ReadWriteRecipe(WELD_RECIPE,READ);		/* Reading the default weld recipe from file */

	/* get own fw versions to display */
	VersionSC_major["SC Major"]	=	FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR);
	VersionSC_minor["SC Minor"]	=	FWVersion::Get(FW_VERSION_SC, VERSION_MINOR);
	VersionSC_build["SC Build"]	=	FWVersion::Get(FW_VERSION_SC, VERSION_BUILD);
	//DisplayRecipe(VERSION_INFO);
	/*Critical Data Status member variable*/
	ShutdownCriticalDataFlag = 0;
	
	storageFull.m_IsWeldMemStorageFull = false;	
	storageFull.m_IsScanMemStorageFull = false;	
	storageFull.m_IsDiskStorageFull = false;
	storageFull.m_IsAlarmMemStorageFull = false;
	storageFull.m_IsEventMemStorageFull = false;
	
	bIsTimedSeekEnabledForEvent = false;
	ind_CollectGraphData = false;
	ind_ULGraphData = true;
	ind_RTC_LowBatt = false;
	
	req_OptimizeDB = false;
	
	bIsHMILogged 				= false;
	bIsWebServicesLogged 		= false;
	bIsUIConnected				= false;
	bIsActiveRecipeAvailble		= false;
	bIsWSLogAttempt				= false;
	
	WSUserLevelStatus			= NO_WS_LEVEL;
	WSLoggedUserName			= NO_WS_USER;
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  -    CommonProperty* Object 
*
******************************************************************************/
CommonProperty* CommonProperty::getInstance()
{
	return m_CPObj ? m_CPObj : (m_CPObj = new CommonProperty());
}

/**************************************************************************//**
* \brief   - Checks the system frequency and update the recipes.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::SetSystemFrequency()
{
	switch(powerupRecipe.Frequency)
	{
		case FREQ_20KHZ:
			m_RecipePC.UpdateResonantFreq(START_FREQUENCY20KDEFAULT);
			break;
	
		case FREQ_30KHZ:
			m_RecipePC.UpdateResonantFreq(START_FREQUENCY30KDEFAULT);
			break;
	
		case FREQ_40KHZ:
			m_RecipePC.UpdateResonantFreq(START_FREQUENCY40KDEFAULT);
			break;
	
		default:
			LOGERR("CommonProperty : wrong system frequency type = %d : Setting default system as 20KHz",0,0,0);
			m_RecipePC.UpdateResonantFreq(START_FREQUENCY20KDEFAULT);
	}
}

/**************************************************************************//**
* \brief   - Copies default(read from file) recipe into MAP object.
*
* \param   - UINT32 TypeOfRecipe.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::CopyRecipe(UINT32 TypeOfRecipe)
{
	if(WELD_RECIPE == TypeOfRecipe)
	{
		map<string,int>::iterator it		= recipe.begin();
		map<string,int>::iterator it_flag 	= dispFlag.begin();
		for(;it != recipe.end();it++,it_flag++)
		{
			INT32 firstPos = 0, endPos = 0, valPos = 0;			

			firstPos = RecipeBuffer.find(it->first,firstPos) + ((it->first).length() + 1);
			if(firstPos != -1)
			{
				valPos = RecipeBuffer.find(" ",firstPos);
				if(valPos != -1)
				{
					recipe[it->first] = atoi(RecipeBuffer.substr(firstPos,valPos-firstPos).c_str());				
					endPos = RecipeBuffer.find("\n",valPos);
					if(endPos != -1)
					{
						valPos++;
						dispFlag[it_flag->first] = atoi(RecipeBuffer.substr(valPos,endPos-valPos).c_str());
					}
					else
					{
						//logMsg("Weld : endPos not found\n",0,0,0,0,0,0);
					}
				}
				else
				{
					//logMsg("Weld : ValPos not found\n",0,0,0,0,0,0);
				}
			}
			else
			{
				//logMsg("Weld : firstPos not found\n",0,0,0,0,0,0);
			}
		} /* For loop end */
	}
	else if(TIMER_INTERVAL_RECIPE == TypeOfRecipe)	
	{
		map<string,int>::iterator it 		= TimerIntervalRecipe.begin();
		map<string,int>::iterator it_flag 	= dispFlagTmrInterval.begin();
		for(;it != TimerIntervalRecipe.end();it++,it_flag++)
		{
			INT32 firstPos = 0, endPos = 0, valPos = 0;
			firstPos = RecipeBuffer.find(it->first,firstPos) + ((it->first).length() + 1);
			if(firstPos != -1)
			{
				valPos = RecipeBuffer.find(" ",firstPos);
				if(valPos != -1)
				{
					TimerIntervalRecipe[it->first] = atoi(RecipeBuffer.substr(firstPos,valPos-firstPos).c_str());
					endPos = RecipeBuffer.find("\n",valPos);
					if(endPos != -1)
					{
						valPos++;
						dispFlagTmrInterval[it_flag->first] = atoi(RecipeBuffer.substr(valPos,endPos-valPos).c_str());
					}
					else
					{
						//logMsg("TimerInterval : endPos not found\n",0,0,0,0,0,0);
					}
				}
				else
				{
					//logMsg("TimerInterval : ValPos not found\n",0,0,0,0,0,0);
				}
			}
			else
			{
				//logMsg("TimerInterval : firstPos not found\n",0,0,0,0,0,0);
			}
		} /* For loop end */
	}
	else
	{
		LOGERR("WRONG OPERATION ON RECIPE READ FROM FILE",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Updated value of recipes are stored into MAP object.
*
* \param   - UINT32 TypeOfRecipe.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::StoreRecipe(UINT32 TypeOfRecipe)
{
	char temp[100] = {0x00};
	bool bIsValid  = true;
	map<string,int>::iterator it;
	map<string,int>::iterator it_flag; 
	map<string,int>::iterator it_end;
	
	if(WELD_RECIPE == TypeOfRecipe)
	{
		it 		= recipe.begin();
		it_flag = dispFlag.begin();
		it_end 	= recipe.end();
	}
	else if(TIMER_INTERVAL_RECIPE == TypeOfRecipe)	
	{
		it = TimerIntervalRecipe.begin();
		it_flag = dispFlagTmrInterval.begin();
		it_end = TimerIntervalRecipe.end();
	}
	else if(VERSION_INFO == TypeOfRecipe)
	{
		bIsValid = false;
		cout << "---------- VERSION ----------" << endl;
		map<string,int>::iterator it = VersionSC_major.begin();

		for(;it != VersionSC_major.end();it++)
		{
			cout << it->first << " :" << it->second;
		}
		cout << endl;

		it = VersionSC_minor.begin();
		for(;it != VersionSC_minor.end();it++)
		{
			cout << it->first << " :" << it->second;
		}
		cout <<  endl;
		
		it = VersionSC_build.begin();
		for(;it != VersionSC_build.end();it++)
		{
			cout << it->first << " :" << it->second;
		}
		
		cout << endl << endl;
	}
	else
	{
		bIsValid = false;
		//logMsg("StoreRecipe : Wrong operation\n",0,0,0,0,0,0);
	}
	
	if(bIsValid)
	{
		for(;it != it_end;it++,it_flag++)
		{
			string key 		= it->first;
			UINT32 value 	= it->second;
			UINT32 dispflag = it_flag->second; 
			sprintf((char *)temp,"%s %d %d\n",(char *)key.c_str(), value, dispflag);
			RecipeBuffer = RecipeBuffer + temp;
			key.clear();
		}
	}
}

/**************************************************************************//**
* \brief   - Display the updated recipe values.
*
* \param   - UINT32 - TypeOfRecipe (weld, seek, scan or power up).
*
* \return  - None
*
******************************************************************************/
void CommonProperty::DisplayRecipe(UINT32 TypeOfRecipe)
{
	bool bIsValid	= true;
	map<string,int>::iterator it;
	map<string,int>::iterator it_flag; 
	map<string,int>::iterator it_end;
	
	if(WELD_RECIPE == TypeOfRecipe)
	{
		it 			= recipe.begin();
		it_flag 	= dispFlag.begin();
		it_end 		= recipe.end();
		cout << "---------WELD RECIPE---------" << endl;
	}
	else if(TIMER_INTERVAL_RECIPE == TypeOfRecipe) 
	{
				it 			= TimerIntervalRecipe.begin();
				it_flag 	= dispFlagTmrInterval.begin();
				it_end 		= TimerIntervalRecipe.end();
				cout << "---------TIMER INTERVAL RECIPE---------" << endl;
	}
	else
	{
		bIsValid = false;
		//logMsg("DispRecipe : Wrong operation\n",0,0,0,0,0,0);
	}
	
	if(bIsValid)
	{
		for(;it != it_end;it++,it_flag++)
		{
			cout << it->first << " : " << it->second << " : " << it_flag->second << endl;
		}
	}
}

/**************************************************************************//**
* \brief   - Checks for specific file available or not.
*
* \param   - string filename - recipe file name.
*
* \return  - bool - on available return true
*
******************************************************************************/
bool CommonProperty::IsFileExist(string fileName)
{
	ifstream infile(fileName.c_str());
    return infile.good();
}

/**************************************************************************//**
* \brief   - Read and Write operation performed on recipe file.
*
* \param   - UINT32 TypeOfRecipe, UINT32 OP.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::ReadWriteRecipe(UINT32 TypeOfRecipe, UINT32 OP)
{
	fstream fp;
	bool bIsValidRecipe = true;
	RecipeBuffer.clear();
	if(WELD_RECIPE == TypeOfRecipe)
	{
		if(READ == OP)
		{
			if(IsFileExist(UPDATED_WELD_RECIPE_PATH))
			{
				fp.open(UPDATED_WELD_RECIPE_PATH, ios::in);
			}
			else
			{
				fp.open(DEFAULT_WELD_RECIPE_PATH, ios::in);
			}
		}
		else if(WRITE == OP)
		{
			fp.open(UPDATED_WELD_RECIPE_PATH, ios::out);
		}
		else
		{
			LOGERR("Invalid operation on weld recipe",0,0,0);
		}
	}
	else if(TIMER_INTERVAL_RECIPE == TypeOfRecipe)		
	{
		if(READ == OP)
		{
			fp.open(TIMER_INTERVAL_RECIPE_PATH, ios::in);
		}
		else if(WRITE == OP)
		{
			fp.open(TIMER_INTERVAL_RECIPE_PATH, ios::out);
		}
		else
		{
			LOGERR("Invalid operation on power up data recipe\n",0,0,0);
		}
	}
	else
	{
		bIsValidRecipe = false;
		LOGERR("Wrong recipe type",0,0,0);
	}
	
	/* READ and WRITE OPERATION ON FILE */
	if(bIsValidRecipe)
	{
		if(READ == OP)
		{		
			if(fp.is_open())
			{
				string temp;
				while(getline(fp,temp))
				{
					RecipeBuffer = RecipeBuffer + temp + "\n";
					temp.clear();
				}
				fp.close();
				CopyRecipe(TypeOfRecipe);
			}
			else
			{
				LOGERR("Failed to open updated recipe file for read",0,0,0);
			}
		}
		else if(WRITE == OP)
		{
			StoreRecipe(TypeOfRecipe);

			if(fp.is_open())
			{
				fp << RecipeBuffer;
				fp.close();
			}
			else
			{
				LOGERR("Failed to open updated recipe file for write",0,0,0);
			}
		}
	}
}

/**************************************************************************//**
* 
* \brief  - Sets the Digital Tune frequency at start up
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void CommonProperty::UpdateStartUPDigitalTune(string strStackRecipe)
{
	UINT32 Memory, Midband = 0;
	stringstream ssStackRecipeZero;
	std::string strStackRecipeParam;
	vector<string> vStackParams;
	Message message;

	switch(powerupRecipe.Frequency)
	{
		case FREQ_20KHZ:
			Midband = F20KHZMIDBAND;			
			break;
	
		case FREQ_30KHZ:
			Midband = F30KHZMIDBAND;			
			break;
	
		case FREQ_40KHZ:
			Midband = F40KHZMIDBAND;			
			break;
	}
	
	//Get the Stack Recipe zero
	ssStackRecipeZero.str(strStackRecipe);
	
	if (!ssStackRecipeZero.str().empty())
	{
		//break up the data from stack recipe (Eg. OutData: 20001,1,0,0)
		while (getline (ssStackRecipeZero, strStackRecipeParam,','))
			vStackParams.push_back (strStackRecipeParam);
		
	    stackRecipe.SetData(StackRecipe::DIGITAL_TUNE, atoi(vStackParams[1].c_str())*10);
	    stackRecipe.SetData(StackRecipe::INTERNAL_OFFSET_FLAG, atoi(vStackParams[2].c_str()));
	    stackRecipe.SetData(StackRecipe::INTERNAL_FREQ_OFFSET, atoi(vStackParams[3].c_str()));
	    stackRecipe.SetData(StackRecipe::END_OF_WELD_STORE, atoi(vStackParams[4].c_str()));
	}
	else
	{
	    stackRecipe.SetData(StackRecipe::DIGITAL_TUNE, Midband);
	    stackRecipe.SetData(StackRecipe::INTERNAL_OFFSET_FLAG, 0);
	    stackRecipe.SetData(StackRecipe::INTERNAL_FREQ_OFFSET, 0);
	    stackRecipe.SetData(StackRecipe::END_OF_WELD_STORE, 0);
	}
    
    stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, 0);
	
	Memory = stackRecipe.GetData(StackRecipe::DIGITAL_TUNE); //Digital Tune + 0 (MemoryOffset)
	
	stackRecipe.SetData(StackRecipe::MEMORY, Memory);
	
	recipe["DigitalTune"] = stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
	m_RecipePC.UpdateDigitalTune(stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));
	
	recipe["MemoryOffset"] = 0;
	m_RecipePC.UpdateMemoryOffset(0);
	
	recipe["StartFrequency"] = Memory;       
	m_RecipePC.UpdateResonantFreq(Memory);
	
	message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
	msgQSend(GetMsgQId(cTaskName[SDO_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);

}


/**************************************************************************//**
* \brief   - returns object weld recipe of SC.
*
* \param   - None.
*
* \return  - ActiveRecipe& - m_ActiveRecpObj
*
******************************************************************************/
ActiveRecipe& CommonProperty::getActiveRecipeInstance()
{
	return m_RecipeSC;
}

/**************************************************************************//**
* \brief   - returns object weld recipe of PC.
*
* \param   - None.
*
* \return  - PcWeldRecipe& - m_PcWeldRecpObj
*
******************************************************************************/
PcWeldRecipe& CommonProperty::getPcWeldRecipeInstance()
{
	return m_RecipePC;
}

/**************************************************************************//**
* \brief   - returns object weld recipe of AC.
*
* \param   - None.
*
* \return  - AcWeldRecipe& - m_AcWeldRecpObj
*
******************************************************************************/
AcWeldRecipe& CommonProperty::getAcWeldRecipeInstance()
{
	return m_RecipeAC;
}

/**************************************************************************//**
* \brief   - Keep update weld recipe of SC.
*
* \param   - const ActiveRecipe& srcObj.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::UpdateActiveRecipe(const ActiveRecipe& srcObj)
{
	m_RecipeSC = srcObj;
}

/**************************************************************************//**
* \brief   - Keep update weld recipe of PC.
*
* \param   - const PcWeldRecipe& srcObj.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::UpdatePcWeldRecipe(const PcWeldRecipe& srcObj)
{
	m_RecipePC = srcObj;
}

/**************************************************************************//**
* \brief   - Keep update weld recipe of AC.
*
* \param   - const AcWeldRecipe& srcObj.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::UpdateAcWeldRecipe(const AcWeldRecipe& srcObj)
{
	m_RecipeAC = srcObj;
}

/**************************************************************************//**
* \brief   - Returns the particular task's message queue ID.
*
* \param   - string TaskName.
*
* \return  - MSG_Q_ID.
*
******************************************************************************/
MSG_Q_ID CommonProperty::GetMsgQId(string TaskName)
{
	return MsgQMap.find(TaskName)->second;
}

/**************************************************************************//**
* \brief   - Store all the message queues id created for business logic.
*
* \param   - string TaskName, MSG_Q_ID.
*
* \return  - MSG_Q_ID.
*
******************************************************************************/
void CommonProperty::SetMsgQId(string TaskName, MSG_Q_ID MID)
{
	MsgQMap.insert ( pair<string,MSG_Q_ID>(TaskName,MID));
}

/**************************************************************************//**
* \brief   - Returns the particular task ID.
*
* \param   - string TaskName.
*
* \return  - TASK_ID.
*
******************************************************************************/
TASK_ID CommonProperty::GetTaskId(string TaskName)
{
	return TaskIdMap.find(TaskName)->second;
}

/**************************************************************************//**
* \brief   - Store all the tasks id created for business logic.
*
* \param   - string TaskName, TASK_ID.
*
* \return  - TASK_ID.
*
******************************************************************************/
void CommonProperty::SetTaskId(string TaskName, TASK_ID TID)
{
	TaskIdMap.insert ( pair<string,TASK_ID>(TaskName,TID) );
}

/**************************************************************************//** 
* \brief  - To clear the memory offset frequency 
*
* \param  - None.
*
* \return  - None
*
******************************************************************************/
void CommonProperty::ClearMemoryFrequency()
{
	unsigned char	pData[MAX_SIZE_OF_MSG_LENGTH];
	Message 		message;
	
	stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, 0);
	recipe["StartFrequency"] 	= stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
	recipe["MemoryOffset"] 		= 0;
	stackRecipe.SetData(StackRecipe::MEMORY, stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));
	
	message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
	msgQSend(GetMsgQId(cTaskName[SDO_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);

}

/**************************************************************************//** 
* \brief   - To Update the memory offset frequency 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void CommonProperty::UpdateMemoryOffset()
{
	unsigned char	pData[MAX_SIZE_OF_MSG_LENGTH];
	Message 		message;
	
	//stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, 0);
	recipe["MemoryOffset"]= stackRecipe.GetData(StackRecipe::MEMORY_OFFSET);

	message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
	msgQSend(GetMsgQId(cTaskName[SDO_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
}

/**************************************************************************//** 
* \brief   - To Set the digital tune frequency. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void CommonProperty::SetDigitalTune(INT32 DigitalTune)
{
	unsigned char	pData[MAX_SIZE_OF_MSG_LENGTH];
	Message 		message;
	
	//stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, 0);
	recipe["DigitalTune"]= DigitalTune;
	LOGDBG("digitaltune = %d",recipe["DigitalTune"],0,0);
	
	message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
	msgQSend(GetMsgQId(cTaskName[SDO_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
}

/**************************************************************************//** 
* \brief   - To Set the frequency offset. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void CommonProperty::SetFrequencyOffset(INT32 FreqOffset)
{
	unsigned char	pData[MAX_SIZE_OF_MSG_LENGTH];
	Message 		message;
	
	//stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, 0);
	recipe["FrequencyOffset"]= FreqOffset;
#ifdef DEBUG
	LOGDBG("digi = %d", recipe["FrequencyOffset"],0,0);
#endif
	
	message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
	msgQSend(GetMsgQId(cTaskName[SDO_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
}

/**************************************************************************//**
* 
* \brief  - Get memory storage full status
*
* \param  - CIRCULAR_DB_MEM idx
*
* \return  - True/false(full or not)
*
******************************************************************************/
bool CommonProperty :: IsMemStorageFull(CIRCULAR_DB_MEM idx) const
{
	bool storageStatus = true;
	
	switch (idx)
	{
	case WELD_MEM_CB:
		storageStatus = storageFull.m_IsWeldMemStorageFull;
		break;
	case SCAN_MEM_CB:
		storageStatus = storageFull.m_IsScanMemStorageFull;
		break;
	case DISK_MEM_FULL_CB:
		storageStatus = storageFull.m_IsDiskStorageFull;
		break;
	case ALARM_MEM_CB:
		 storageStatus = storageFull.m_IsAlarmMemStorageFull;
		break;
	case EVENT_MEM_CB:
		 storageStatus = storageFull.m_IsEventMemStorageFull;
		break;	

	default:
		LOGDBG("DB_T : Invalid ID : %d",idx,0,0); 
		break;
	}	
	return storageStatus;
}

/**************************************************************************//**
* 
* \brief  - Set memory storage full status
*
* \param  - CIRCULAR_DB_MEM idx bool storageStatus
*
* \return  - None
*
******************************************************************************/
void CommonProperty :: SetMemStorageFullStatus(CIRCULAR_DB_MEM idx, bool storageStatus)
{
	switch (idx)
	{
	case WELD_MEM_CB:
		storageFull.m_IsWeldMemStorageFull = storageStatus;
		break;
	case SCAN_MEM_CB:
		storageFull.m_IsScanMemStorageFull = storageStatus;
		break;	
	case DISK_MEM_FULL_CB:
		storageFull.m_IsDiskStorageFull = storageStatus;
		break;
	case ALARM_MEM_CB:
		storageFull.m_IsAlarmMemStorageFull = storageStatus;
		break;
	case EVENT_MEM_CB:
		storageFull.m_IsEventMemStorageFull = storageStatus;
		break;	
	case RESET_MEM_FULL_CB:
		storageFull.m_IsWeldMemStorageFull = false;
		storageFull.m_IsScanMemStorageFull = false;
		storageFull.m_IsDiskStorageFull = false;
		storageFull.m_IsAlarmMemStorageFull = false;
		storageFull.m_IsEventMemStorageFull = false;
		break;	
		
	default:
		LOGDBG("DB_T : Invalid ID : %d",idx,0,0); 
		break;
	}	
}


/**************************************************************************//**
* \brief   - Return the system configuration data
*
* \param   - intf_SysConfig_t  &sysConfig
*
* \return  - None.
*
******************************************************************************/
void CommonProperty :: GetSysConfContext(intf_SysConfig_t  &sysConfig)
{	
	memcpy(&sysConfig, &m_SysConfig, sizeof(intf_SysConfig_t));
}

/**************************************************************************//**
* \brief   - Set the system configuration data to context
*
* \param   - intf_SysConfig_t  *pSysConfig
*
* \return  - None.
*
******************************************************************************/
void CommonProperty :: SetSysConfContext(intf_SysConfig_t  *pSysConfig)
{		
	memcpy(&m_SysConfig, pSysConfig, sizeof(intf_SysConfig_t));
}

/**************************************************************************//**
* \brief   - Returns whether timed seek is enable or not for event log.
*
* \param   - None.
*
* \return  - bool.
*
******************************************************************************/
bool CommonProperty :: getIsTimedSeekEnabledForEvent()
{
	return bIsTimedSeekEnabledForEvent;
}

/**************************************************************************//**
* \brief   - Sets timed seek for event log.
*
* \param   - bool.
*
* \return  - None.
*
******************************************************************************/
void CommonProperty :: setIsTimedSeekEnabledForEvent(bool bTimedSeekEvent )
{
	bIsTimedSeekEnabledForEvent = bTimedSeekEvent;
}

/**************************************************************************//**
 * \brief  - Tokenise the string with given delimiter and stores it in vector
 * 
 * \param  - const char *pData, vector<string>&vTokens,char delimiter
 *
 * \return  - int
 *
 ******************************************************************************/
int CommonProperty::StringToTokens(const char *pData, vector<string>&vTokens,char delimiter)
{
	stringstream sstream (pData);
	string strToken;
	int dwTokeCnt=0;

	while(getline(sstream,strToken,delimiter))
	{
		dwTokeCnt++;
		vTokens.push_back(strToken);
	}

	return dwTokeCnt;
}

/**************************************************************************//**
 * \brief  - vector data will be stored in string with given delimiter
 * 
 * \param  - string &strData, vector<string>&vTokens,char delimiter
 *
 * \return  - int
 *
 ******************************************************************************/
int CommonProperty::TokensToString(string &strData, vector<string>&vTokens, char delimiter)
{
	stringstream sstream (strData);
	string strToken;
	int dwTokeCnt = 0,dwIndex = 0;
	dwTokeCnt = vTokens.size();
	strData.clear();
	
	/* Inserting upto last but one elements */
	for(dwIndex = 0;dwIndex < dwTokeCnt-1;dwIndex++)
	{
		strData.append(vTokens[dwIndex].c_str());
		strData.append(1,delimiter);
	}
	/* Inserting the last element */
	strData.append(vTokens[dwIndex].c_str());

	return dwTokeCnt;
}

/**************************************************************************//**
* \brief   - Converts an integer to string
*      		
* \param   - UINT16 iNumber
*
* \return  - string
*
******************************************************************************/
std::string CommonProperty::ConvertIntToString(UINT16 iNumber)
{
	string Result;          // string which will contain the result

	ostringstream convert;   // stream used for the conversion
	convert << iNumber;      // insert the textual representation of 'Number' in the characters in the stream

	Result = convert.str();
	return Result;
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void CommonProperty::DeleteInstance()
{
	delete m_CPObj;
}

/**************************************************************************//**
* \brief   - Release the memory related with object.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
CommonProperty::~CommonProperty()
{
	m_CPObj = NULL;
}
