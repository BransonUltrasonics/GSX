/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Console application for the Ultrasonic Weld machine
 
**********************************************************************************************************/


#include "ConsoleApp.h"
#include "SysConfigEeprom.hpp"
#include <sstream>
#include "MainTask.h"
#include "TestSonicsOn.h"
#include <usrFsLib.h>
#include  "DataBaseManager.h"
#include "version_common.h"

//#define  AWR_DBG
using namespace std;

bool ConsoleApp::bIsScanStart = false;
bool ConsoleApp::bIsTestStart = false;

/**************************************************************************//**
* \brief   - Initialize the member data with default values.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
ConsoleApp::ConsoleApp() : 	UI_MSG_Q_ID(0), iChoice(0), triggerDistance(10), triggerForce(10), m_weldmode(TIMEMODE),m_modeValue(100),
							seektime(1),inputPercentageWeldAmplitude(10),bIsSetup(false),bIsValidOp(false),
							bIsPowerUp(true),PowerupSetup(true),bIsRecipeOp(false)
{
	CP = CommonProperty::getInstance();	
	UI_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[UI_T]);
	SDO_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[SDO_T]);	
	DB_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[DB_T]);
	Main_TID		= CP->GetTaskId(cTaskName[MAIN_T]);
	CTRL_TID		= CP->GetTaskId(cTaskName[CTRL_T]);
}

/**************************************************************************//**
* \brief   - Verifies particular recipe is customizable from console by user
*
* \param   - None
*
* \return  - INT32 - iChoice - User selected input from  given menu
*
*****************************************************************************/
INT32 ConsoleApp::IsUserEdit()
{
	INT32 iChoice = 0;
	cout << "To modify, please choose EDIT" << endl;
	while((iChoice != EDIT) && (iChoice != EXIT_INIT))
	{
		cout << "1.EDIT\n2.EXIT" << endl;
		iChoice = ConsoleRead();

		if((iChoice != EDIT) && (iChoice != EXIT_INIT))
		{
			cout << endl << "Invalid request" << iChoice <<  endl;
		}
	}
	cout << endl;
	return iChoice;
}

/**************************************************************************//**
* \brief   - Checks the recipe parameters are configurable from console
*
* \param   - UINT32 TypeOfRecipe, string Key[index]
*
* \return  - INT32 - IRet(1) - Is recipe parameter configured to edit
*
*****************************************************************************/
INT32 ConsoleApp::IsUserMode(UINT32 TypeOfRecipe, string Key)
{
	INT32 iRet = 0;
	if(WELD_RECIPE == TypeOfRecipe)
	{
		iRet = CP->dispFlag[Key];
	}
	else if(TIMER_INTERVAL_RECIPE == TypeOfRecipe)		
	{
		iRet = CP->dispFlagTmrInterval[Key];
	}
	return iRet;
}

/**************************************************************************//**
* \brief   - Shows options to reset the alarm condition
*
* \param   - None
*
* \return  - None
*
*****************************************************************************/
void ConsoleApp::ResetAlarm()
{
	cout << "Select the operation" << endl;
	cout << "1.RESET ALARM" << endl;
	if(WELD_OP == iCmd)
	{
		cout << "2.UPDATE WELD RECIPE" << endl;
	}
	else if(SEEK_OP == iCmd)
	{
		cout << "2.UPDATE SEEK RECIPE" << endl;
	}
	else if(HORN_SCAN == iCmd)
	{
		cout << "2.UPDATE SCAN RECIPE" << endl;
	}

	iChoice = ConsoleRead();

	memset(message.Buffer,0x00,sizeof(message.Buffer));
	if(ALARM_OP == iChoice)
	{
		/* Prepare message with alarm reset CMD */
		message.msgID = TO_UI_TASK_CLEAR_ALL;
		this->SendMsgToUITask(UI_MSG_Q_ID,message);
	}
	else if(UPDATE_WELD_RECIPE_INIT == iChoice)
	{
		/*Accepts the updated recipe values and save */
		switch(iCmd)
		{
			case WELD_OP:
				this->GetInputFromUser(WELD_RECIPE);
				this->SaveWeldRecipe(WELD_RECIPE);
				break;
				
			case SEEK_OP:
				this->GetInputFromUser(SEEK_RECIPE);
				this->SaveWeldRecipe(SEEK_RECIPE);
				break;
				
			case HORN_SCAN:
				this->GetInputFromUser(SCAN_RECIPE);
				this->SaveWeldRecipe(SCAN_RECIPE);
				break;
				
			default:
				break;
		}
		message.msgID = TO_UI_TASK_CLEAR_ALL;
		this->SendMsgToUITask(UI_MSG_Q_ID,message);
		bIsRecipeOp = true;
		bIsValidOp 	= true;
		iCmd = 0;
	}
	else
	{
		cout << "Invalid selection" << endl;
		bIsValidOp	= true;
	}
}

/**************************************************************************//**
* \brief   - Capture the user input to display in console terminal.
*
* \param   - None
*
* \return  - IRet - Returns captured integer value.
*
*****************************************************************************/
INT32 ConsoleApp::ConsoleRead()
{
	char chInput;
	char temp[10] = {0x00};
	INT32 iRet 	 = 0;
	INT32 Digit  = 0;

	while(cin.get(chInput))
	{
		if(chInput =='\n')
		{
			break;
		}
		else
		{
			cout << chInput;			
			temp[0] = chInput;
			Digit = atoi(temp);
			iRet  = (iRet*10)+Digit; 
		}				
	}
	cout << endl;
	return iRet;	
}

/**************************************************************************//**
* \brief   - Sends the initial power up and default recipe to UI task.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::OnSysPowerUp()
{
	INT32 recipeID;
	UINT32 status;
	
	/* On PowerUp Block for Freq and Power values from user */
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	SetPowerUpRecipe();			/* Assign read recipe values into PowerUpData structure */
	message.msgID = TO_UI_TASK_POWER_UP_RECIPE;
	memcpy(&message.Buffer[0], &Console_PowerUpRecipe, sizeof(Console_PowerUpRecipe));
	SendMsgToUITask(UI_MSG_Q_ID,message);
		
	// assign default weld recipe values into console weld structures
	PullActiveRecipeFromCP ();
	
	// attempt to pull last active recipe number from DB
	recipeID = ObtainActiveRecipeNumber();
	
	if (recipeID > 0)
	{
		// pull last active weld recipe from DB, assign values into console weld structures
		status = PullActiveRecipeFromDB (recipeID);
		
#ifdef AWR_DBG
	printf ("\nConsoleApp::PullActiveRecipeFromDB:  -------------------> status 6: %d\n",status);
#endif
		
		if (status != OK)
		{
			// revert to default weld recipe values into console weld structures
			PullActiveRecipeFromCP ();
		}
		
		CloseDB();
	}
	
	// Prepare the message with Weld recipe
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	message.msgID = TO_UI_TASK_WELD_RECIPE_SC;

	// load SC recipe as active
	cout << "ConsoleApp : Size of SC : " << sizeof(Console_WeldRecipeSC) << " PC : " << sizeof(Console_WeldRecipePC) << " AC : " << sizeof(Console_WeldRecipeAC) << endl;
	memcpy(&message.Buffer[0],&Console_WeldRecipeSC, sizeof(Console_WeldRecipeSC));
	SendMsgToUITask(UI_MSG_Q_ID,message);

	// load PC/AC recipe as active
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	message.msgID = TO_UI_TASK_WELD_RECIPE;
	
	memcpy(message.Buffer,&Console_WeldRecipePC, sizeof(Console_WeldRecipePC));
	memcpy(message.Buffer + sizeof(Console_WeldRecipePC),&Console_WeldRecipeAC, sizeof(Console_WeldRecipeAC));
	SendMsgToUITask(UI_MSG_Q_ID,message);
}

/**************************************************************************//**
* \brief   - Load the weld recipe with latest saved recipe on power up.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetWeldRecipe()
{	
	SetScWeldRecipe();
	SetPcWeldRecipe();
	SetAcWeldRecipe();
	
	SetSuspectLimits();
	SetRejectLimits();
}

/**************************************************************************//**
* \brief   - Set the SC weld recipe with latest saved recipe on power up.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetScWeldRecipe()
{
	/* Default or latest modified recipe from consoleApp - Which is stored into WeldRecipe.txt or UpdateWeldRecipe.txt file in SD card */
	Console_WeldRecipe.SCData.WeldMode				= CP->recipe["WeldMode"];
	Console_WeldRecipe.SCData.ModeValue				= CP->recipe["ModeValue"];
	Console_WeldRecipe.SCData.HoldTime				= CP->recipe["HoldTime"];
	Console_WeldRecipe.SCData.TriggerDistance		= CP->recipe["TriggerDistance"];
	Console_WeldRecipe.SCData.TriggerForce			= CP->recipe["TriggerForce"];
	Console_WeldRecipe.SCData.MaxWeldTimeout		= 0;							/* Indicates the feature is disabled */ 
	Console_WeldRecipe.SCData.bIsCoolingValveEnabled = true; 	
	Console_WeldRecipe.SCData.bIsTimedSeekEnabled	= CP->recipe["bIsTimedSeekEnabled"];
	Console_WeldRecipe.SCData.bIsPreWeldSeekEnabled	= CP->recipe["bIsPreWeldSeekEnabled"];
	Console_WeldRecipe.SCData.bIsPostWeldSeekEnabled= CP->recipe["bIsPostWeldSeekEnabled"];;
	Console_WeldRecipe.SCData.TimedSeekPeriod		 =CP->recipe["TimedSeekPeriod"];
	Console_WeldRecipe.SCData.RecipeNumber			 = 0;
	Console_WeldRecipe.SCData.RecipeRevNumber		 = 0;
	strcpy(Console_WeldRecipe.SCData.User,"ConsoleUser");
	Console_WeldRecipe.SCData.StackSerialNo			 = 0;
		
	Console_WeldRecipe.SCData.AfterBurst.bIsEnabled       = CP->recipe["AfterburstFlag"];
	Console_WeldRecipe.SCData.AfterBurst.BurstDelay       = CP->recipe["AfterburstDelay"];
	Console_WeldRecipe.SCData.AfterBurst.BurstAmplitude   = CP->recipe["AfterburstAmplitude"];
	Console_WeldRecipe.SCData.AfterBurst.BurstTime        = CP->recipe["AfterburstTime"];
	
//	Console_WeldRecipe.SCData.PreWeldSeek	= 	CP->recipe["PreWeldSeek"];
//	Console_WeldRecipe.SCData.PostWeldSeek 	= 	CP->recipe["PostWeldSeek"];
//	
//	Console_WeldRecipe.SCData.TimedSeek		= 	CP->recipe["TimedSeek"];
//	Console_WeldRecipe.SCData.TimedSeekTime = 	CP->recipe["TimedSeekTime"];
	
	/* Local reference purpose - Only for consoleApp context */
	triggerDistance 							= CP->recipe["TriggerDistance"];
	triggerForce 								= CP->recipe["TriggerForce"];
	holdtime									= CP->recipe["HoldTime"];
	m_weldmode  								= (WELDMODES)CP->recipe["WeldMode"];
	m_modeValue  								= CP->recipe["ModeValue"];

	SetForceStep();
	SetAmplitudeStep();
	SetAfterBurst();
	SetPreTrigger();
	SetCutoffLimits();
	SetSuspectLimits();
	SetRejectLimits();
	SetEnergyBrake();
}

/**************************************************************************//**
* \brief   - Set the PC weld recipe with latest saved recipe on power up.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetPcWeldRecipe()
{
	switch(Console_PowerUpRecipe.Frequency)
	{
	case FREQ_20KHZ:
		Console_WeldRecipe.PCData.AmplitudeLoopC1 	= AMPLOOPC1_20KDEFAULT;
		Console_WeldRecipe.PCData.AmplitudeLoopC2 	= AMPLOOPC2_20KDEFAULT;
		Console_WeldRecipe.PCData.PhaseLoop			= PLOOP_20KDEFAULT;
		Console_WeldRecipe.PCData.PhaseLoopCF 		= PLOOPCF_20KDEFAULT;
		Console_WeldRecipe.PCData.FrequencyLow 		= FREQENCYLOW_20KDEFAULT;
		Console_WeldRecipe.PCData.FrequencyHigh		= FREQENCYHIGH_20KDEFAULT;
		Console_WeldRecipe.PCData.StartFrequency	= START_FREQUENCY20KDEFAULT;
		Console_WeldRecipe.PCData.DigitalTune		= START_FREQUENCY20KDEFAULT;
		break;
	case FREQ_30KHZ:
		Console_WeldRecipe.PCData.AmplitudeLoopC1 	= AMPLOOPC1_30KDEFAULT;
		Console_WeldRecipe.PCData.AmplitudeLoopC2 	= AMPLOOPC2_30KDEFAULT;
		Console_WeldRecipe.PCData.PhaseLoop			= PLOOP_30KDEFAULT;
		Console_WeldRecipe.PCData.PhaseLoopCF 		= PLOOPCF_30KDEFAULT;
		Console_WeldRecipe.PCData.FrequencyLow 		= FREQENCYLOW_30KDEFAULT;
		Console_WeldRecipe.PCData.FrequencyHigh		= FREQENCYHIGH_30KDEFAULT;
		Console_WeldRecipe.PCData.StartFrequency	= START_FREQUENCY30KDEFAULT;
		Console_WeldRecipe.PCData.DigitalTune		= START_FREQUENCY30KDEFAULT;
		break;
	case FREQ_40KHZ:
		Console_WeldRecipe.PCData.AmplitudeLoopC1 	= AMPLOOPC1_40KDEFAULT;
		Console_WeldRecipe.PCData.AmplitudeLoopC2 	= AMPLOOPC2_40KDEFAULT;
		Console_WeldRecipe.PCData.PhaseLoop			= PLOOP_40KDEFAULT;
		Console_WeldRecipe.PCData.PhaseLoopCF 		= PLOOPCF_40KDEFAULT;
		Console_WeldRecipe.PCData.FrequencyLow 		= FREQENCYLOW_40KDEFAULT;
		Console_WeldRecipe.PCData.FrequencyHigh		= FREQENCYHIGH_40KDEFAULT;
		Console_WeldRecipe.PCData.StartFrequency	= START_FREQUENCY40KDEFAULT;
		Console_WeldRecipe.PCData.DigitalTune		= START_FREQUENCY40KDEFAULT;
		break;
	default:
		cout << "ConsoleApp : Wrong system frequency" << endl;
		break;
	}

	Console_WeldRecipe.PCData.PhaseLimitTime	= CP->recipe["PhaseLimitTime"];
	Console_WeldRecipe.PCData.PhaseLimit		= CP->recipe["PhaseLimit"];
	Console_WeldRecipe.PCData.ControlMode		= CP->recipe["ControlMode"];
	Console_WeldRecipe.PCData.BlindtimeSeek		= CP->recipe["BlindtimeSeek"];
	Console_WeldRecipe.PCData.Blindtimeweld		= CP->recipe["BlindtimeWeld"];
	Console_WeldRecipe.PCData.DDSSwitchtime		= CP->recipe["DDSSwitchtime"];
	inputPercentageWeldAmplitude				= CP->recipe["WeldAmplitude"];
	Console_WeldRecipe.PCData.Parameter7		= CP->recipe["Parameter7"];
	Console_WeldRecipe.PCData.Parameter8		= CP->recipe["Parameter8"];
	Console_WeldRecipe.PCData.Parameter9		= CP->recipe["Parameter9"];
	Console_WeldRecipe.PCData.Parameter10		= CP->recipe["Parameter10"];
	Console_WeldRecipe.PCData.Parameter11		= CP->recipe["Parameter11"];
	Console_WeldRecipe.PCData.Parameter12		= CP->recipe["Parameter12"];
	Console_WeldRecipe.PCData.Parameter13		= CP->recipe["Parameter13"];
	Console_WeldRecipe.PCData.Parameter14		= CP->recipe["Parameter14"];
	Console_WeldRecipe.PCData.Parameter15		= CP->recipe["Parameter15"];
	Console_WeldRecipe.PCData.FLimitTime		= CP->recipe["FLimitTime"];
	Console_WeldRecipe.PCData.AmpProportionalGain = CP->recipe["AmpProportionalGain"];
	Console_WeldRecipe.PCData.AmpIntegralGain	= CP->recipe["AmpIntegralGain"];
	Console_WeldRecipe.PCData.PhaseProportionalGain = CP->recipe["PhaseProportionalGain"];
	Console_WeldRecipe.PCData.FrequencyWindowSize 	= CP->recipe["FrequencyWindowSize"];
	Console_WeldRecipe.PCData.PFofPhasecontrolloop 	= CP->recipe["PFofPhasecontrolloop"];
	Console_WeldRecipe.PCData.PIFrequencyLow 		= CP->recipe["PIFrequencyLow"];
	Console_WeldRecipe.PCData.PIFrequencyHigh		= CP->recipe["PIFrequencyHigh"];
	Console_WeldRecipe.PCData.PIPhaseLimitTime		= CP->recipe["PIPhaseLimitTime"];
	Console_WeldRecipe.PCData.PIPhaseLimit			= CP->recipe["PIPhaseLimit"];
	Console_WeldRecipe.PCData.WeldRampTime			= CP->recipe["WRampTime"];
	Console_WeldRecipe.PCData.MemoryOffset			= 0;
}

/**************************************************************************//**
* \brief   - Set the AC weld recipe with latest saved recipe on power up.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetAcWeldRecipe()
{
	Console_WeldRecipe.ACData.WeldForce				= CP->recipe["WeldForce"];
	Console_WeldRecipe.ACData.ForceRampTime			= CP->recipe["ForceRampTime"];
	Console_WeldRecipe.ACData.HoldMode				= CP->recipe["HoldMode"];
	Console_WeldRecipe.ACData.HoldForce				= CP->recipe["HoldForce"];
	Console_WeldRecipe.ACData.HoldForceRampTime		= CP->recipe["HoldForceRampTime"];
	Console_WeldRecipe.ACData.ExpectedPartContactPosition = CP->recipe["ExpectedPartContactPosition"];
	Console_WeldRecipe.ACData.TotalCollapseTarget 	= CP->recipe["TotalCollapseTarget"];
	Console_WeldRecipe.ACData.ReadyPosition			= CP->recipe["ReadyPosition"];
	
	/* On power up, Actuator setup Advanced Feature parameters (Acceleration, Velocity etc.) default value for recipe zero
       shall be multiplied with 10000 to support imperial conversion  */
	Console_WeldRecipe.ACData.DownAcceleration		= CP->recipe["DownAcceleration"] * 10000;
	Console_WeldRecipe.ACData.DownMaxVelocity		= CP->recipe["DownMaxVelocity"] * 10000;
	Console_WeldRecipe.ACData.DownDeceleration		= CP->recipe["DownDeceleration"] * 10000;
	Console_WeldRecipe.ACData.ReturnAcceleration	= CP->recipe["ReturnAcceleration"] * 10000;
	Console_WeldRecipe.ACData.ReturnMaxVelocity		= CP->recipe["ReturnMaxVelocity"] * 10000;
	Console_WeldRecipe.ACData.ReturnDeceleration	= CP->recipe["ReturnDeceleration"] * 10000;
	
	Console_WeldRecipe.ACData.ExpectedPartContactOffset = CP->recipe["ExpectedPartContactOffset"];
	Console_WeldRecipe.ACData.PartContactWindowMinus= CP->recipe["PartContactWindowMinus"];
	Console_WeldRecipe.ACData.PartContactWindowPlus = CP->recipe["PartContactWindowPlus"];
	Console_WeldRecipe.ACData.NumForceSteps			= CP->recipe["NumForceSteps"];
	Console_WeldRecipe.ACData.ForceStepForce[0]		= CP->recipe["ForceStep1Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[0]	= CP->recipe["ForceStep1RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[1]		= CP->recipe["ForceStep2Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[1]	= CP->recipe["ForceStep2RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[2]		= CP->recipe["ForceStep3Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[2]	= CP->recipe["ForceStep3RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[3]		= CP->recipe["ForceStep4Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[3]	= CP->recipe["ForceStep4RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[4]		= CP->recipe["ForceStep5Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[4]	= CP->recipe["ForceStep5RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[5]		= CP->recipe["ForceStep6Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[5]	= CP->recipe["ForceStep6RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[6]		= CP->recipe["ForceStep7Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[6]	= CP->recipe["ForceStep7RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[7]		= CP->recipe["ForceStep8Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[7]	= CP->recipe["ForceStep8RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[8]		= CP->recipe["ForceStep9Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[8]	= CP->recipe["ForceStep9RampTime"];
	Console_WeldRecipe.ACData.ForceStepForce[9]		= CP->recipe["ForceStep10Force"];
	Console_WeldRecipe.ACData.ForceStepRampTime[9]	= CP->recipe["ForceStep10RampTime"];
	Console_WeldRecipe.ACData.ReadyPositionToggle	= 1;
	triggerDistance 							= CP->recipe["TriggerDistance"];
}

/**************************************************************************//**
* \brief   - Set the force steps with latest saved recipe on power up.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetForceStep()
{
	Console_WeldRecipe.SCData.NumForceSteps		  	= CP->recipe["NumForceSteps"];
	Console_WeldRecipe.SCData.ForceStepAt		  	= CP->recipe["ForceStepAt"];
	Console_WeldRecipe.SCData.ForceStepAtValue[0] 	= CP->recipe["ForceStep1AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[1] 	= CP->recipe["ForceStep2AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[2] 	= CP->recipe["ForceStep3AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[3] 	= CP->recipe["ForceStep4AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[4] 	= CP->recipe["ForceStep5AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[5] 	= CP->recipe["ForceStep6AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[6] 	= CP->recipe["ForceStep7AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[7] 	= CP->recipe["ForceStep8AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[8] 	= CP->recipe["ForceStep9AtValue"];
	Console_WeldRecipe.SCData.ForceStepAtValue[9] 	= CP->recipe["ForceStep10AtValue"];
}

/**************************************************************************//**
* \brief   - Set the amplitude steps with latest saved recipe on power up.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetAmplitudeStep()
{
	Console_WeldRecipe.SCData.NumAmplitudeSteps 	= 1;
	Console_WeldRecipe.SCData.AmplitudeStepAt		= STEPBYTIME;
	Console_WeldRecipe.SCData.AmplitudeStepValue[0] = CP->recipe["WRampTime"];
	Console_WeldRecipe.SCData.AmplitudeStepValue[1]	= 200;
	Console_WeldRecipe.SCData.AmplitudeStepValue[2]	= 300;
	Console_WeldRecipe.SCData.AmplitudeStepValue[3]	= 400;
	Console_WeldRecipe.SCData.AmplitudeStepValue[4]	= 500;
	Console_WeldRecipe.SCData.AmplitudeStepValue[5]	= 600;
	Console_WeldRecipe.SCData.AmplitudeStepValue[6]	= 700;
	Console_WeldRecipe.SCData.AmplitudeStepValue[7]	= 800;
	Console_WeldRecipe.SCData.AmplitudeStepValue[8]	= 900;
	Console_WeldRecipe.SCData.AmplitudeStepValue[9]	= 1000;
	Console_WeldRecipe.SCData.AmplitudeStep[0]		= 90;
	Console_WeldRecipe.SCData.AmplitudeStep[1]		= 10;
	Console_WeldRecipe.SCData.AmplitudeStep[2]		= 20;
	Console_WeldRecipe.SCData.AmplitudeStep[3]		= 30;
	Console_WeldRecipe.SCData.AmplitudeStep[4]		= 40;
	Console_WeldRecipe.SCData.AmplitudeStep[5]		= 50;
	Console_WeldRecipe.SCData.AmplitudeStep[6]		= 60;
	Console_WeldRecipe.SCData.AmplitudeStep[7]		= 70;
	Console_WeldRecipe.SCData.AmplitudeStep[8]		= 80;
	Console_WeldRecipe.SCData.AmplitudeStep[9]		= 90;
}

/**************************************************************************//**
* \brief   - Load the after burst recipe 
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetAfterBurst()
{
	Console_WeldRecipe.SCData.AfterBurst.bIsEnabled		= true;
	Console_WeldRecipe.SCData.AfterBurst.BurstAmplitude = 50;
	Console_WeldRecipe.SCData.AfterBurst.BurstDelay		= 100;
	Console_WeldRecipe.SCData.AfterBurst.BurstTime		= 100;
}

/**************************************************************************//**
* \brief   - Load the pre trigger recipe .
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetPreTrigger()
{
	Console_WeldRecipe.SCData.PreTrigger.bIsAutoEnabled		= true;
	Console_WeldRecipe.SCData.PreTrigger.bIsEnabled			= true;
	Console_WeldRecipe.SCData.PreTrigger.TriggerAmplitude 	= 50;
	Console_WeldRecipe.SCData.PreTrigger.bIsDistanceEnabled	= true;
	Console_WeldRecipe.SCData.PreTrigger.PreTriggerDistance = 5000;
}

/**************************************************************************//**
* \brief   - Load the energy brake recipe 
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetEnergyBrake()
{
	Console_WeldRecipe.SCData.EnergyBrake.bIsEnabled			= true;
	Console_WeldRecipe.SCData.EnergyBrake.EnergyBrakeAmplitude	= 50;
	Console_WeldRecipe.SCData.EnergyBrake.EnergyBrakeTime		= 100;
}

/**************************************************************************//**
* \brief   - Set the default suspect limits recipe
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetSuspectLimits()
{
	Console_WeldRecipe.SCData.bIsSuspectLimitEnabled			= false;
	Console_WeldRecipe.SCData.SuspectLimit.Time.bIsLowEnabled	= true;
	Console_WeldRecipe.SCData.SuspectLimit.Time.bIsHighEnabled	= true;
	Console_WeldRecipe.SCData.SuspectLimit.Time.LowLimit= 10000;//32;//10000;//32;
	Console_WeldRecipe.SCData.SuspectLimit.Time.HighLimit=100000;//480;//100000;//  480;
//
	Console_WeldRecipe.SCData.SuspectLimit.Energy.bIsLowEnabled = true;
	Console_WeldRecipe.SCData.SuspectLimit.Energy.bIsHighEnabled = true;
	Console_WeldRecipe.SCData.SuspectLimit.Energy.LowLimit= 10000;//21;//10000;//21;
	Console_WeldRecipe.SCData.SuspectLimit.Energy.HighLimit=100000;//27;//100000; //27;

	Console_WeldRecipe.SCData.SuspectLimit.PeakPower.bIsLowEnabled = true;
	Console_WeldRecipe.SCData.SuspectLimit.PeakPower.bIsHighEnabled = true;
	Console_WeldRecipe.SCData.SuspectLimit.PeakPower.LowLimit= 10000;//91;//10000;//91;
	Console_WeldRecipe.SCData.SuspectLimit.PeakPower.HighLimit=100000;//94;///100000; //94;

	Console_WeldRecipe.SCData.SuspectLimit.CollapseDistance.bIsLowEnabled	 = true;
	Console_WeldRecipe.SCData.SuspectLimit.CollapseDistance.bIsHighEnabled	 = true;
	Console_WeldRecipe.SCData.SuspectLimit.CollapseDistance.LowLimit=10000;//31;//10000; //31;
	Console_WeldRecipe.SCData.SuspectLimit.CollapseDistance.HighLimit=100000;//32; //100000;// 32;

	Console_WeldRecipe.SCData.SuspectLimit.AbsoluteDistance.bIsLowEnabled	= true;
	Console_WeldRecipe.SCData.SuspectLimit.AbsoluteDistance.bIsHighEnabled	= true;
	Console_WeldRecipe.SCData.SuspectLimit.AbsoluteDistance.LowLimit= 10000;//25010;
	Console_WeldRecipe.SCData.SuspectLimit.AbsoluteDistance.HighLimit=100000;//25799;

	Console_WeldRecipe.SCData.SuspectLimit.TriggerDistance.bIsLowEnabled	= true;
	Console_WeldRecipe.SCData.SuspectLimit.TriggerDistance.bIsHighEnabled	= true;
	Console_WeldRecipe.SCData.SuspectLimit.TriggerDistance.LowLimit		=10000; //25011;
	Console_WeldRecipe.SCData.SuspectLimit.TriggerDistance.HighLimit	= 100000;//25700;

	Console_WeldRecipe.SCData.SuspectLimit.WeldForce.bIsLowEnabled 		= true;
	Console_WeldRecipe.SCData.SuspectLimit.WeldForce.bIsHighEnabled 	= true;
	Console_WeldRecipe.SCData.SuspectLimit.WeldForce.LowLimit			= 10000;//5;
	Console_WeldRecipe.SCData.SuspectLimit.WeldForce.HighLimit	  		= 100000;//8;
	   
	Console_WeldRecipe.SCData.SuspectLimit.Frequency.bIsLowEnabled		= true;
	Console_WeldRecipe.SCData.SuspectLimit.Frequency.bIsHighEnabled		= true;
	Console_WeldRecipe.SCData.SuspectLimit.Frequency.LowLimit			= 10000;//19990;
	Console_WeldRecipe.SCData.SuspectLimit.Frequency.HighLimit	  		= 100000;//20300;
}
 
/**************************************************************************//**
* \brief   - Set the default reject limits recipe
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetRejectLimits()
{
	Console_WeldRecipe.SCData.bIsRejectLimitEnabled					= true;
	Console_WeldRecipe.SCData.RejectLimit.Time.bIsLowEnabled		= true;
	Console_WeldRecipe.SCData.RejectLimit.Time.bIsHighEnabled 		= true ;//true;
	Console_WeldRecipe.SCData.RejectLimit.Time.LowLimit		  		= 10000;//30;   //10000;
	Console_WeldRecipe.SCData.RejectLimit.Time.HighLimit	  		= 100000;//499;   //100000;

	Console_WeldRecipe.SCData.RejectLimit.Energy.bIsLowEnabled 		= true;
	Console_WeldRecipe.SCData.RejectLimit.Energy.bIsHighEnabled 	= true;
	Console_WeldRecipe.SCData.RejectLimit.Energy.LowLimit			= 10000;//19;     //10000;
	Console_WeldRecipe.SCData.RejectLimit.Energy.HighLimit	    	= 100000;//28;     //100000;

	Console_WeldRecipe.SCData.RejectLimit.PeakPower.bIsLowEnabled 	= true;
	Console_WeldRecipe.SCData.RejectLimit.PeakPower.bIsHighEnabled 	= true;
	Console_WeldRecipe.SCData.RejectLimit.PeakPower.LowLimit	   	= 10000;//80;    //10000
	Console_WeldRecipe.SCData.RejectLimit.PeakPower.HighLimit	   	= 100000;//95;    //100000

	Console_WeldRecipe.SCData.RejectLimit.CollapseDistance.bIsLowEnabled= true;
	Console_WeldRecipe.SCData.RejectLimit.CollapseDistance.bIsHighEnabled= true;
	Console_WeldRecipe.SCData.RejectLimit.CollapseDistance.LowLimit	= 10000; //29;  //10000;
	Console_WeldRecipe.SCData.RejectLimit.CollapseDistance.HighLimit= 100000;//34;  //100000;

	Console_WeldRecipe.SCData.RejectLimit.AbsoluteDistance.bIsLowEnabled= true;
	Console_WeldRecipe.SCData.RejectLimit.AbsoluteDistance.bIsHighEnabled= true;
	Console_WeldRecipe.SCData.RejectLimit.AbsoluteDistance.LowLimit	= 10000;//25000;
	Console_WeldRecipe.SCData.RejectLimit.AbsoluteDistance.HighLimit= 100000;//25800;

	Console_WeldRecipe.SCData.RejectLimit.TriggerDistance.bIsLowEnabled= true;
	Console_WeldRecipe.SCData.RejectLimit.TriggerDistance.bIsHighEnabled= true;
	Console_WeldRecipe.SCData.RejectLimit.TriggerDistance.LowLimit	= 10000;//25012;
	Console_WeldRecipe.SCData.RejectLimit.TriggerDistance.HighLimit	= 100000;//25768;

	Console_WeldRecipe.SCData.RejectLimit.WeldForce.bIsLowEnabled 	= true;
	Console_WeldRecipe.SCData.RejectLimit.WeldForce.bIsHighEnabled	= true;
	Console_WeldRecipe.SCData.RejectLimit.WeldForce.LowLimit		= 10000;//4;
	Console_WeldRecipe.SCData.RejectLimit.WeldForce.HighLimit	  	= 100000;//9;	
	
	Console_WeldRecipe.SCData.RejectLimit.Frequency.bIsLowEnabled		= true;
	Console_WeldRecipe.SCData.RejectLimit.Frequency.bIsHighEnabled		= true;
	Console_WeldRecipe.SCData.RejectLimit.Frequency.LowLimit		= 10000;//19989;
	Console_WeldRecipe.SCData.RejectLimit.Frequency.HighLimit	  	= 100000;//20379;
}

/**************************************************************************//**
* \brief   - Set the default cutoff limits recipe
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetCutoffLimits()
{
	Console_WeldRecipe.SCData.bIsCutoffLimitEnabled					= true;
	
	Console_WeldRecipe.SCData.CutoffLimit.AbsoluteDistanceCutoff.bIsEnabled	= true;
	Console_WeldRecipe.SCData.CutoffLimit.AbsoluteDistanceCutoff.Value		= 5000;//25529;
	
	Console_WeldRecipe.SCData.CutoffLimit.CollapseDistanceCutoff.bIsEnabled	= true;  
	Console_WeldRecipe.SCData.CutoffLimit.CollapseDistanceCutoff.Value		= 100;
	
	//Console_WeldRecipe.SCData.CutoffLimit.FrequencyLowCutoff.bIsEnabled	= true;
	Console_WeldRecipe.SCData.CutoffLimit.FrequencyHighCutoff.bIsEnabled    = true;
	Console_WeldRecipe.SCData.CutoffLimit.FrequencyHighCutoff.Value			=15000;
	
	Console_WeldRecipe.SCData.CutoffLimit.FrequencyLowCutoff.bIsEnabled		= true;
	Console_WeldRecipe.SCData.CutoffLimit.FrequencyLowCutoff.Value			=10000;
	
	Console_WeldRecipe.SCData.CutoffLimit.PeakPowerCutoff.bIsEnabled	= true;
	Console_WeldRecipe.SCData.CutoffLimit.PeakPowerCutoff.Value		=10000;
	
	Console_WeldRecipe.SCData.CutoffLimit.TimeCutoff.bIsEnabled		= true;
	Console_WeldRecipe.SCData.CutoffLimit.TimeCutoff.Value			=10000;
	
	Console_WeldRecipe.SCData.CutoffLimit.EnergyCutoff.bIsEnabled		= true;   
	Console_WeldRecipe.SCData.CutoffLimit.EnergyCutoff.Value			= 10000;
	
	Console_WeldRecipe.SCData.CutoffLimit.GroundDetectCutoff			= true;
}

/**************************************************************************//**
* \brief   - Set the version control info and send to slaves.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetVersionInfo()
{
}

/**************************************************************************//**
* \brief   - Set the timer interval for the AuxClock of slaves..
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetTimerInterval()		
{
	Console_TimerInterval.TmrIntervalPC = CP->TimerIntervalRecipe["TmrIntervalPC"];
	TempTmrIntervalPC = Console_TimerInterval.TmrIntervalPC;
	GetInputFromUser(TIMER_INTERVAL_RECIPE);
	SaveWeldRecipe(TIMER_INTERVAL_RECIPE);
	SendMsgToUITask(UI_MSG_Q_ID,message);
}

/**************************************************************************//**
* \brief   - Load the power up recipe with latest saved recipe.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetPowerUpRecipe()
{
	Console_PowerUpRecipe.CheckSum			= CP->powerupRecipe.CheckSum;
	Console_PowerUpRecipe.Frequency			= CP->powerupRecipe.Frequency;
	Console_PowerUpRecipe.Power				= CP->powerupRecipe.Power;
	Console_PowerUpRecipe.SerialNo			= CP->powerupRecipe.SerialNo;
	Console_PowerUpRecipe.SystemType		= CP->powerupRecipe.SystemType;

	TempFreq  = Console_PowerUpRecipe.Frequency;
	TempPower = Console_PowerUpRecipe.Power;
	//GetInputFromUser(POWERUP_RECIPE);
	//SaveWeldRecipe(POWERUP_RECIPE);
}

/**************************************************************************//**
* \brief   - Save the customized recipe in file.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SaveWeldRecipe(UINT32 TypeOfRecipe)
{
	UINT32 iChoice 		= 0;
	bool bIsValidRecipe = true;
	
	/* Loops until select correct option from below menu */
	while(1)
	{
		cout << "Need to submit the changes..?" << endl;
		cout << "1.Submit" << endl;
		cout << "2.Cancel" << endl;

		iChoice = ConsoleRead();

		if(SAVE == iChoice)
		{         
			switch(TypeOfRecipe)
			{
			case WELD_RECIPE:
				CP->ReadWriteRecipe(WELD_RECIPE,WRITE);			/* Write updated recipe value into UpdatedWeldRecipe.txt file */
				message.msgID = TO_UI_TASK_WELD_RECIPE_SC;
				memcpy(&message.Buffer[0],&Console_WeldRecipeSC, sizeof(Console_WeldRecipeSC));
				SendMsgToUITask(UI_MSG_Q_ID,message);
				
				message.msgID = TO_UI_TASK_WELD_RECIPE;
				memset(message.Buffer,0x00,sizeof(message.Buffer));
				memcpy(message.Buffer,&Console_WeldRecipePC, sizeof(Console_WeldRecipePC));
				memcpy(message.Buffer + sizeof(Console_WeldRecipePC),&Console_WeldRecipeAC, sizeof(Console_WeldRecipeAC));
				break;

			case POWERUP_RECIPE:
				CP->ReadWriteRecipe(POWERUP_RECIPE,WRITE);		/* Write updated recipe value into PowerUpRecipe.txt file */
				message.msgID = TO_UI_TASK_POWER_UP_RECIPE;
				memcpy(&message.Buffer[0],&Console_PowerUpRecipe, sizeof(Console_PowerUpRecipe));
				break;
			case TIMER_INTERVAL_RECIPE:	
				CP->ReadWriteRecipe(TIMER_INTERVAL_RECIPE,WRITE);		/* Write updated recipe value into TimerInterval.txt file */
				message.msgID = TO_UI_TASK_TMR_INTVL_DATA_PC;
				memcpy (&message.Buffer[0], &Console_TimerInterval, sizeof(Console_TimerInterval));
				break;
				
			default:
				bIsValidRecipe = false;
				break;
			}

			if(bIsValidRecipe)
			{
				SendMsgToUITask(UI_MSG_Q_ID,message);
			}
			break;
		}
		else if(CANCEL == iChoice)
		{
			if(TypeOfRecipe == WELD_RECIPE)
			{
				triggerDistance = triggerDistance_temp;
				triggerForce 	= triggerForce_temp;
				m_weldmode 		= m_weldmode_temp;
				m_modeValue 	= m_modeValue_temp;
				inputPercentageWeldAmplitude = inputPercentageWeldAmplitude_temp;
				Console_WeldRecipePC = Console_WeldRecipe.PCData;
				Console_WeldRecipeAC = Console_WeldRecipe.ACData;					
			}	
			if(TypeOfRecipe == POWERUP_RECIPE)
			{	
				Console_PowerUpRecipe.Frequency = TempFreq;
				Console_PowerUpRecipe.Power = TempPower;
			}
			if(TypeOfRecipe == TIMER_INTERVAL_RECIPE)		
			{
				Console_TimerInterval.TmrIntervalPC = TempTmrIntervalPC;
			}
			break;
		}
		else
		{	
			cout << "Invalid Input....!!" << endl;
		}
	}
}

/**************************************************************************//**
* \brief   - Allow user to customize the weld recipe from consoleApp.
*
* \param   - UINT32 - TypeOfRecipe (weld, seek, scan or power-up).
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::GetInputFromUser(UINT32 TypeOfRecipe)
{
	UINT16 itterator;
	stringstream tempStream;
	string tempString;
	bool ValidVal = false;
	/* Edit the weld recipe from console if the configuration flag is enabled in UpdatedWeldRecipe.txt file */
	if(WELD_RECIPE == TypeOfRecipe)
	{
		cout << "Weld recipe configuration" << endl;
		
		/* Only SC reference weld recipe parameters */
		if(IsUserMode(WELD_RECIPE,"WeldMode"))
		{
			cout << "Weld Mode		: 	"<< m_weldmode << endl;
			m_weldmode_temp = m_weldmode;
			if(IsUserEdit() == EDIT)
			{
				cout << "Weld Mode : TIMEMODE=1, ENERGYMODE=2,  PEAKPOWERMODE=3" << endl << "GRDDETECTMODE=4, ABSMODE=5, COLLAPSEMODE=6" << endl;
				m_weldmode = (WELDMODES)ConsoleRead();
				CP->recipe["WeldMode"] = m_weldmode;
				Console_WeldRecipeSC.WeldMode = m_weldmode;
			}
		}
		if((m_weldmode < TIMEMODE) || (m_weldmode > COLLAPSEMODE))
		{
			cout << "*****Invalid Weld Mode - defaulting to TIME MODE weld *****"<< endl;
			m_weldmode = TIMEMODE;
		}
		if(IsUserMode(WELD_RECIPE,"ModeValue"))
		{
			switch(m_weldmode)
			{
				case TIMEMODE:
					cout << "Entered Time - Mode Value	(ms)	: 	"<< m_modeValue << endl;
					break;
				case ENERGYMODE:
					cout << "Entered Energy - Mode Value (J)		: 	"<< m_modeValue << endl;
					break;
				case PEAKPOWERMODE:
					cout << "Entered PeakPower - Mode Value	(W)	: 	"<< m_modeValue << endl;
					break;
				case GRDDETECTMODE:
					cout << "Entered ScrubTime - Mode Value(ms)	: 	"<< m_modeValue << endl;
					break;
				case ABSMODE:
					cout << "Entered Absolute Distance - Mode Value	(um)	: 	"<< m_modeValue << endl;
					break;
				case COLLAPSEMODE:
					cout << "Entered Collapse Distance - Mode Value	(um)	: 	"<< m_modeValue << endl;
					break;
			}
			
			m_modeValue_temp = m_modeValue;
			if(IsUserEdit() == EDIT)
			{
				cout << "New modeValue :" << endl;
				m_modeValue = (UINT32)ConsoleRead();
				CP->recipe["ModeValue"] = m_modeValue;
				Console_WeldRecipeSC.ModeValue = m_modeValue;
			}
		}
		if(IsUserMode(WELD_RECIPE,"HoldTime"))
		{
			cout << "HoldTime(ms)		:	" << Console_WeldRecipeSC.HoldTime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" HoldTime(ms):";
				holdtime = (UINT16) ConsoleRead();
				CP->recipe["HoldTime"] = holdtime;
				Console_WeldRecipeSC.HoldTime = holdtime;
			}
		}
		if(IsUserMode(WELD_RECIPE,"MaxWeldTimeout"))
		{
			cout << "MaxWeldTimeout(ms)		:	" << Console_WeldRecipeSC.MaxWeldTimeout << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" MaxWeldTimeout(ms):";
				Console_WeldRecipeSC.MaxWeldTimeout = ConsoleRead();
				CP->recipe["MaxWeldTimeout"] = Console_WeldRecipeSC.MaxWeldTimeout;
			}
		}
		if(IsUserMode(WELD_RECIPE,"CoolingValve"))
		{
			cout << "IsCoolingValveEnabled		:	" << Console_WeldRecipeSC.bIsCoolingValveEnabled << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" CoolingValveEnable(true/false) :";
				Console_WeldRecipeSC.bIsCoolingValveEnabled 	= ConsoleRead();
				CP->recipe["bIsCoolingValveEnabled"] 		= Console_WeldRecipeSC.bIsCoolingValveEnabled;
			}
		}
		
		/* AC WELD RECIPE */
		if(IsUserMode(WELD_RECIPE,"WeldForce"))
		{
			cout << "Weld Force(N)		:	" << Console_WeldRecipeAC.WeldForce << endl;
			if(IsUserEdit() == EDIT)
			{

				cout << "Weld Force (N): ";
				Console_WeldRecipeAC.WeldForce = (UINT16) ConsoleRead();
				Console_WeldRecipeAC.ForceStepForce[1] = Console_WeldRecipeAC.WeldForce;
				CP->recipe["WeldForce"] = Console_WeldRecipeAC.WeldForce;
			}
		}
		if(IsUserMode(WELD_RECIPE,"ForceRampTime"))
		{
			cout << "Weld Force Ramp Time(ms)		:	" << Console_WeldRecipeAC.ForceRampTime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Weld Force Ramp Time(ms): ";
				Console_WeldRecipeAC.ForceRampTime = (UINT16) ConsoleRead();
				Console_WeldRecipeAC.ForceStepRampTime[1] = Console_WeldRecipeAC.ForceRampTime;
				CP->recipe["ForceRampTime"] = Console_WeldRecipeAC.ForceRampTime;
			}
		}
		if(IsUserMode(WELD_RECIPE,"HoldMode"))
		{
			printf("HoldMode		:	%u\n",Console_WeldRecipeAC.HoldMode);
			if(IsUserEdit() == EDIT)
			{
				cout<<"HoldMode:";
				Console_WeldRecipeAC.HoldMode = (UINT8) ConsoleRead();
				CP->recipe["HoldMode"] = Console_WeldRecipeAC.HoldMode;
			}
		}
		if(IsUserMode(WELD_RECIPE,"TotalCollapseTarget"))
		{
			cout << "TotalCollapseTarget		:	" << Console_WeldRecipeAC.TotalCollapseTarget << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" TotalCollapseTarget :";
				Console_WeldRecipeAC.TotalCollapseTarget = (UINT16) ConsoleRead();
				CP->recipe["TotalCollapseTarget"] = Console_WeldRecipeAC.TotalCollapseTarget;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"HoldForce"))
		{
			cout << "Hold Force(N)		: 	"<< Console_WeldRecipeAC.HoldForce << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" Hold Force(N):";
				Console_WeldRecipeAC.HoldForce = (UINT16) ConsoleRead();
				CP->recipe["HoldForce"] = Console_WeldRecipeAC.HoldForce;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"HoldForceRampTime"))
		{
			cout << "Hold Force Ramp Time(ms)	: 	"<< Console_WeldRecipeAC.HoldForceRampTime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" Hold Force Ramp Time(ms):";
				Console_WeldRecipeAC.HoldForceRampTime = (UINT16) ConsoleRead();
				CP->recipe["HoldForceRampTime"] = Console_WeldRecipeAC.HoldForceRampTime;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"ExpectedPartContactPosition"))
		{
			cout << "ExpectedPartContactPosition (um)	: 	"<< Console_WeldRecipeAC.ExpectedPartContactPosition << endl;		
		}
		
		if(IsUserMode(WELD_RECIPE,"ReadyPosition"))
		{
			cout << "ReadyPosition(um)		: 	"<< Console_WeldRecipeAC.ReadyPosition << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" ReadyPosition(um):";
				Console_WeldRecipeAC.ReadyPosition = (UINT32) ConsoleRead();
				CP->recipe["ReadyPosition"]	= Console_WeldRecipeAC.ReadyPosition;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"DownAcceleration"))
		{
			cout << "DownAcceleration	: 	"<< Console_WeldRecipeAC.DownAcceleration << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<< "DownAcceleration(mm/s):";
				Console_WeldRecipeAC.DownAcceleration = (UINT32) ConsoleRead();
				CP->recipe["DownAcceleration"] = Console_WeldRecipeAC.DownAcceleration;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"DownMaxVelocity"))
		{
			cout << "DownMaxVelocity		: 	"<< Console_WeldRecipeAC.DownMaxVelocity << endl;
			if(IsUserEdit() == EDIT)
			{
				cout << "DownMaxVelocity(mm/s):";
				Console_WeldRecipeAC.DownMaxVelocity = (UINT16) ConsoleRead();
				CP->recipe["DownMaxVelocity"] = Console_WeldRecipeAC.DownMaxVelocity;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"DownDeceleration"))
		{
			cout << "DownDeceleration(mm/s)	: 	"<< Console_WeldRecipeAC.DownDeceleration << endl;
			if(IsUserEdit() == EDIT)
			{
				cout << "DownDeceleration(mm/s):";
				Console_WeldRecipeAC.DownDeceleration = (UINT16) ConsoleRead();
				CP->recipe["DownDeceleration"] = Console_WeldRecipeAC.DownDeceleration;
			}
		}
		if(IsUserMode(WELD_RECIPE,"ReturnAcceleration"))
		{
			cout << "ReturnAcceleration(mm/s)	: 	"<< Console_WeldRecipeAC.ReturnAcceleration << endl;
			if(IsUserEdit() == EDIT)
			{
				cout << "ReturnAcceleration(mm/s):";
				Console_WeldRecipeAC.ReturnAcceleration = (UINT16) ConsoleRead();
				CP->recipe["ReturnAcceleration"] = Console_WeldRecipeAC.ReturnAcceleration;
			}
		}
		if(IsUserMode(WELD_RECIPE,"ReturnMaxVelocity"))
		{
			cout << "ReturnMaxVelocity(mm/s)	: 	"<< Console_WeldRecipeAC.ReturnMaxVelocity << endl;
			if(IsUserEdit() == EDIT)
			{
				cout << "ReturnMaxVelocity(mm/s):";
				Console_WeldRecipeAC.ReturnMaxVelocity = (UINT16) ConsoleRead();
				CP->recipe["ReturnMaxVelocity"] = Console_WeldRecipeAC.ReturnMaxVelocity;
			}
		}
		if(IsUserMode(WELD_RECIPE,"ReturnDeceleration"))
		{
			cout << "ReturnDeceleration(mm/s)	: 	"<< Console_WeldRecipeAC.ReturnDeceleration << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" ReturnDeceleration(mm/s):";
				Console_WeldRecipeAC.ReturnDeceleration = (UINT16) ConsoleRead();
				CP->recipe["ReturnDeceleration"] = Console_WeldRecipeAC.ReturnDeceleration;
			}
		}
		if(IsUserMode(WELD_RECIPE,"ExpectedPartContactOffset"))
		{
			cout << "ExpectedPartContactOffset	: 	"<< Console_WeldRecipeAC.ExpectedPartContactOffset << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" ExpectedPartContactOffset:";
				Console_WeldRecipeAC.ExpectedPartContactOffset = (UINT16) ConsoleRead();
				CP->recipe["ExpectedPartContactOffset"] = Console_WeldRecipeAC.ExpectedPartContactOffset;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PartContactWindowMinus"))
		{
			cout << "PartContactWindowMinus	: 	"<< Console_WeldRecipeAC.PartContactWindowMinus << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" PartContactWindowMinus:";
				Console_WeldRecipeAC.PartContactWindowMinus = (UINT16) ConsoleRead();
				CP->recipe["PartContactWindowMinus"] = Console_WeldRecipeAC.PartContactWindowMinus;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PartContactWindowPlus"))
		{
			cout << "PartContactWindowPlus	: 	"<< Console_WeldRecipeAC.PartContactWindowPlus << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<" PartContactWindowPlus:";
				Console_WeldRecipeAC.PartContactWindowPlus = (UINT16) ConsoleRead();
				CP->recipe["PartContactWindowPlus"] = Console_WeldRecipeAC.PartContactWindowPlus;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"NumForceSteps"))
		{
			printf("NumForceSteps		: 	%u\n",Console_WeldRecipeAC.NumForceSteps);
			if(IsUserEdit() == EDIT)
			{
				cout << "NumForceSteps :";
				Console_WeldRecipeAC.NumForceSteps = (UINT8) ConsoleRead();
				CP->recipe["NumForceSteps"] = Console_WeldRecipeAC.NumForceSteps;
			}
		}
		
		if(Console_WeldRecipeAC.NumForceSteps > 1)
		{
			if(IsUserMode(WELD_RECIPE,"ForceStepAt"))
			{
				cout << "ForceStepAt		: 	"<< (WELDMODES)Console_WeldRecipeSC.ForceStepAt << endl;
				if(IsUserEdit() == EDIT)
				{
					cout << "ForceStepAt Mode : TIMEMODE=1, ENERGYMODE=2,  PEAKPOWERMODE=3" << endl << "ABSMODE=4, COLLAPSEMODE=5" << endl;
					Console_WeldRecipeSC.ForceStepAt = (STEPPINGMODE)ConsoleRead();
					if((Console_WeldRecipeSC.ForceStepAt < TIMEMODE) || (Console_WeldRecipeSC.ForceStepAt > COLLAPSEMODE) 
							|| (GRDDETECTMODE == Console_WeldRecipeSC.ForceStepAt))
					{
						cout << "*****Invalid Step Mode - defaulting to TIME MODE step *****"<< endl;
						Console_WeldRecipeSC.ForceStepAt = TIMEMODE;
					}
					CP->recipe["ForceStepAt"] = Console_WeldRecipeSC.ForceStepAt;
				}
				switch(Console_WeldRecipeSC.ForceStepAt)
				{
					case STEPBYTIME:
						cout << "Entered Time - Force Step At Value Units: (ms) " << endl;
						break;
					case STEPBYENERGY:
						cout << "Entered Energy - Force Step At Value Units: (J)	" << endl;
						break;
					case STEPBYPOWER:
						cout << "Entered PeakPower - Force Step At Value Units: (W) "<< endl;
						break;
					case STEPBYCOLLAPSEDISTANCE:
						cout << "Entered Collapse Distance - Force Step At Value Units: (um) " << endl;
						break;
					case STEPBYABSOLUTEDISTANCE:
						cout << "Entered Absolute Distance - Force Step At Value Units: (um) " << endl;
						break;
				}
			}
		}
		
		for(itterator=2;itterator<(Console_WeldRecipeAC.NumForceSteps+1);itterator++)
		{
			tempStream.str("");
			tempStream << "ForceStep" << itterator << "AtValue";
			tempString = tempStream.str();
			if(IsUserMode(WELD_RECIPE,tempString))
			{
				cout << tempString << "		: 	"<< Console_WeldRecipeSC.ForceStepAtValue[(itterator-1)] << endl;
				if(IsUserEdit() == EDIT)
				{
					cout << tempString << " :";
					Console_WeldRecipeSC.ForceStepAtValue[(itterator-1)] = (UINT16) ConsoleRead();
					CP->recipe[tempString] = Console_WeldRecipeSC.ForceStepAtValue[(itterator-1)];
				}
			}
			tempStream.str("");
			tempStream << "ForceStep" << itterator << "Force";
			tempString = tempStream.str();
			if(IsUserMode(WELD_RECIPE,tempString))
			{
				cout << tempString << "		: 	"<< Console_WeldRecipeAC.ForceStepForce[(itterator-1)] << endl;
				if(IsUserEdit() == EDIT)
				{
					cout << tempString << " :";
					Console_WeldRecipeAC.ForceStepForce[(itterator-1)] = (UINT16) ConsoleRead();
					CP->recipe[tempString] = Console_WeldRecipeAC.ForceStepForce[(itterator-1)];
				}
			}
			tempStream.str("");
			tempStream << "ForceStep" << itterator << "RampTime";
			tempString = tempStream.str();
			if(IsUserMode(WELD_RECIPE,tempString))
			{
				cout <<  tempString << "	: 	"<< Console_WeldRecipeAC.ForceStepRampTime[(itterator-1)] << endl;
				if(IsUserEdit() == EDIT)
				{
					cout << tempString << " :";
					Console_WeldRecipeAC.ForceStepRampTime[(itterator-1)] = (UINT16) ConsoleRead();
					CP->recipe[tempString] = Console_WeldRecipeAC.ForceStepRampTime[(itterator-1)];
				}
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"TriggerDistance"))
		{
			cout << "TriggerDistance(D)		:	" << Console_WeldRecipeSC.TriggerDistance << endl;
			triggerDistance_temp = Console_WeldRecipeSC.TriggerDistance;
			if(IsUserEdit() == EDIT)
			{
				cout << "Trigger Distance (N): ";
				Console_WeldRecipeSC.TriggerDistance	= (INT32) ConsoleRead();
				CP->recipe["TriggerDistance"]			= Console_WeldRecipeSC.TriggerDistance;
			}
		}
		if(IsUserMode(WELD_RECIPE,"TriggerForce"))
		{
			cout << "Trigger Force(N)		:	" << Console_WeldRecipeSC.TriggerForce << endl;
			triggerForce_temp = Console_WeldRecipeSC.TriggerForce;
			if(IsUserEdit() == EDIT)
			{
				cout << "Trigger Force (N): ";
				Console_WeldRecipeSC.TriggerForce 	= (INT32) ConsoleRead();
				CP->recipe["TriggerForce"]			= Console_WeldRecipeSC.TriggerForce;
			}
		}
		cout << endl;

		/* PC Weld Recipe */
		if(IsUserMode(WELD_RECIPE,"WRampTime"))
		{
			cout <<"Weld Ramp Time(ms)	:	"<< Console_WeldRecipePC.WeldRampTime << endl;
			if(IsUserEdit()	==	EDIT)
			{
				cout << "Weld Ramp Time(ms)	:	";
				Console_WeldRecipePC.WeldRampTime = (INT32)ConsoleRead();
				CP->recipe["WRampTime"]	=	Console_WeldRecipePC.WeldRampTime;
			}
		}
		if(IsUserMode(WELD_RECIPE,"AmplitudeLoopC1"))
		{
			cout << "AmplitudeLoopC1		: 	"<< Console_WeldRecipePC.AmplitudeLoopC1 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout << "Amplitude Loop C1 : ";
				Console_WeldRecipePC.AmplitudeLoopC1 = (INT32)ConsoleRead();
				CP->recipe["AmplitudeLoopC1"] = Console_WeldRecipePC.AmplitudeLoopC1;
			}
		}
		if(IsUserMode(WELD_RECIPE,"AmplitudeLoopC2"))
		{
			cout << "AmplitudeLoopC2		: 	"<< Console_WeldRecipePC.AmplitudeLoopC2 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Amplitude Loop C2 :";
				Console_WeldRecipePC.AmplitudeLoopC2 = (INT32)ConsoleRead();
				CP->recipe["AmplitudeLoopC2"] = Console_WeldRecipePC.AmplitudeLoopC2;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PhaseLoop"))
		{
			cout << "PhaseLoop		: 	"<< Console_WeldRecipePC.PhaseLoop << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Phase Loop (50):";
				Console_WeldRecipePC.PhaseLoop = (INT32)ConsoleRead();
				CP->recipe["PhaseLoop"] = Console_WeldRecipePC.PhaseLoop;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PhaseLoopCF"))
		{
			cout << "PhaseLoopCF		: 	"<< Console_WeldRecipePC.PhaseLoopCF << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Phase Loop CF :";
				Console_WeldRecipePC.PhaseLoopCF = (INT32)ConsoleRead();
				CP->recipe["PhaseLoopCF"] = Console_WeldRecipePC.PhaseLoopCF;
			}
		}
		if(IsUserMode(WELD_RECIPE,"FrequencyLow"))
		{
			cout << "FrequencyLow		: 	"<< Console_WeldRecipePC.FrequencyLow << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Frequency Low :";
				Console_WeldRecipePC.FrequencyLow = (INT32)ConsoleRead();
				CP->recipe["FrequencyLow"] = Console_WeldRecipePC.FrequencyLow;
			}
		}
		if(IsUserMode(WELD_RECIPE,"FrequencyHigh"))
		{
			cout << "FrequencyHigh		: 	"<< Console_WeldRecipePC.FrequencyHigh << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Frequency High :";
				Console_WeldRecipePC.FrequencyHigh = (INT32)ConsoleRead();
				CP->recipe["FrequencyHigh"] = Console_WeldRecipePC.FrequencyHigh;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PhaseLimitTime"))
		{
			cout << "PhaseLimitTime		: 	"<< Console_WeldRecipePC.PhaseLimitTime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Phase Limit Time :";
				Console_WeldRecipePC.PhaseLimitTime = (INT32)ConsoleRead();
				CP->recipe["PhaseLimitTime"] = Console_WeldRecipePC.PhaseLimitTime;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PhaseLimit"))
		{
			cout << "PhaseLimit		:	"<< Console_WeldRecipePC.PhaseLimit << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Phase Limit :";
				Console_WeldRecipePC.PhaseLimit = (INT32)ConsoleRead();
				CP->recipe["PhaseLimit"] = Console_WeldRecipePC.PhaseLimit;
			}
		}
		if(IsUserMode(WELD_RECIPE,"ControlMode"))
		{
			cout << "ControlMode		: 	"<< Console_WeldRecipePC.ControlMode << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Control Mode:";
				Console_WeldRecipePC.ControlMode = (INT32)ConsoleRead();
				CP->recipe["ControlMode"] = Console_WeldRecipePC.ControlMode;
			}
		}
		if(IsUserMode(WELD_RECIPE,"BlindtimeSeek"))
		{
			cout << "BlindtimeSeek		: 	"<< Console_WeldRecipePC.BlindtimeSeek << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"BlindtimeSeek :";
				Console_WeldRecipePC.BlindtimeSeek = (INT32)ConsoleRead();
				CP->recipe["BlindtimeSeek"] = Console_WeldRecipePC.BlindtimeSeek;
			}
		}
		if(IsUserMode(WELD_RECIPE,"BlindtimeWeld"))
		{
			cout << "BlindtimeWeld		: 	"<< Console_WeldRecipePC.Blindtimeweld << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"BlindtimeWeld :";
				Console_WeldRecipePC.Blindtimeweld = (INT32)ConsoleRead();
				CP->recipe["BlindtimeWeld"] = Console_WeldRecipePC.Blindtimeweld;
			}
		}
		if(IsUserMode(WELD_RECIPE,"DDSSwitchtime"))
		{
			cout << "DDS Switch time		: 	"<< Console_WeldRecipePC.DDSSwitchtime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"DDS Switch time :";
				Console_WeldRecipePC.DDSSwitchtime = (INT32)ConsoleRead();
				CP->recipe["DDSSwitchtime"] = Console_WeldRecipePC.DDSSwitchtime;
			}
		}
		if(IsUserMode(WELD_RECIPE,"WeldAmplitude"))
		{
			cout << "Weld Amplitude		: 	"<< inputPercentageWeldAmplitude << endl;
			inputPercentageWeldAmplitude_temp = inputPercentageWeldAmplitude;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Weld Amplitude :";
				inputPercentageWeldAmplitude = (INT16)ConsoleRead();
				CP->recipe["WeldAmplitude"] = inputPercentageWeldAmplitude; 
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter7"))
		{
			cout << "Parameter7		: 	"<< Console_WeldRecipePC.Parameter7 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter7 :";
				Console_WeldRecipePC.Parameter7 = (INT32)ConsoleRead();
				CP->recipe["Parameter7"] = Console_WeldRecipePC.Parameter7;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter8"))
		{
			cout << "Parameter8		: 	"<< Console_WeldRecipePC.Parameter8 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter8 :";
				Console_WeldRecipePC.Parameter8 = (INT32)ConsoleRead();
				CP->recipe["Parameter8"] = Console_WeldRecipePC.Parameter8;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter9"))
		{
			cout << "Parameter9		: 	"<< Console_WeldRecipePC.Parameter9 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter9 :";
				Console_WeldRecipePC.Parameter9 = (INT32)ConsoleRead();
				CP->recipe["Parameter9"] = Console_WeldRecipePC.Parameter9;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter10"))
		{
			cout << "Parameter10		: 	"<< Console_WeldRecipePC.Parameter10 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter10 :";
				Console_WeldRecipePC.Parameter10 = (INT32)ConsoleRead();
				CP->recipe["Parameter10"] = Console_WeldRecipePC.Parameter10;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter11"))
		{
			cout << "Parameter11		: 	"<< Console_WeldRecipePC.Parameter11 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter11 :";
				Console_WeldRecipePC.Parameter11 = (INT32)ConsoleRead();
				CP->recipe["Parameter11"] = Console_WeldRecipePC.Parameter11;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter12"))
		{
			cout << "Parameter12		: 	"<< Console_WeldRecipePC.Parameter12 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter12 :";
				Console_WeldRecipePC.Parameter12 = (INT32)ConsoleRead();
				CP->recipe["Parameter12"] = Console_WeldRecipePC.Parameter12;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter13"))
		{
			cout << "Parameter13		: 	"<< Console_WeldRecipePC.Parameter13 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter13 :";
				Console_WeldRecipePC.Parameter13 = (INT32)ConsoleRead();
				CP->recipe["Parameter13"] = Console_WeldRecipePC.Parameter13;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter14"))
		{
			cout << "Parameter14		: 	"<< Console_WeldRecipePC.Parameter14 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter14 :";
				Console_WeldRecipePC.Parameter14 = (INT32)ConsoleRead();
				CP->recipe["Parameter14"] = Console_WeldRecipePC.Parameter14;
			}
		}
		if(IsUserMode(WELD_RECIPE,"Parameter15"))
		{
			cout << "Parameter15		: 	"<< Console_WeldRecipePC.Parameter15 << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Parameter15 :";
				Console_WeldRecipePC.Parameter15 = (INT32)ConsoleRead();
				CP->recipe["Parameter15"] = Console_WeldRecipePC.Parameter15;
			}
		}
		if(IsUserMode(WELD_RECIPE,"FLimitTime"))
		{
			cout << "FLimitTime		: 	"<< Console_WeldRecipePC.FLimitTime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"F-Limit Time :";
				Console_WeldRecipePC.FLimitTime = (INT32)ConsoleRead();
				CP->recipe["FLimitTime"] = Console_WeldRecipePC.FLimitTime;
			}
		}
		if(IsUserMode(WELD_RECIPE,"AmpProportionalGain"))
		{
			cout << "AmpProportionalGain	: 	"<< Console_WeldRecipePC.AmpProportionalGain << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Proportional Gain :";
				Console_WeldRecipePC.AmpProportionalGain = (INT32)ConsoleRead();
				CP->recipe["AmpProportionalGain"] = Console_WeldRecipePC.AmpProportionalGain;
			}
		}
		if(IsUserMode(WELD_RECIPE,"AmpIntegralGain"))
		{
			cout << "AmpIntegralGain		: 	"<< Console_WeldRecipePC.AmpIntegralGain << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Amp Integral Gain :";
				Console_WeldRecipePC.AmpIntegralGain = (INT32)ConsoleRead();
				CP->recipe["AmpIntegralGain"] = Console_WeldRecipePC.AmpIntegralGain;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PhaseProportionalGain"))
		{
			cout << "PhaseProportionalGain	: 	"<< Console_WeldRecipePC.PhaseProportionalGain << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Phase Proportional Gain :";
				Console_WeldRecipePC.PhaseProportionalGain = (INT32)ConsoleRead();
				CP->recipe["PhaseProportionalGain"] = Console_WeldRecipePC.PhaseProportionalGain;
			}
		}
		if(IsUserMode(WELD_RECIPE,"FrequencyWindowSize"))
		{
			cout << "FrequencyWindowSize	: 	"<< Console_WeldRecipePC.FrequencyWindowSize << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Frequency Window Size :";
				Console_WeldRecipePC.FrequencyWindowSize = (INT32)ConsoleRead();
				CP->recipe["FrequencyWindowSize"] = Console_WeldRecipePC.FrequencyWindowSize;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PFofPhasecontrolloop"))
		{
			cout << "PFofPhasecontrolloop	: 	"<< Console_WeldRecipePC.PFofPhasecontrolloop << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"PF of Phase control loop :";
				Console_WeldRecipePC.PFofPhasecontrolloop = (INT32)ConsoleRead();
				CP->recipe["PFofPhasecontrolloop"] = Console_WeldRecipePC.PFofPhasecontrolloop;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PIFrequencyLow"))
		{
			cout << "PIFrequencyLow		: 	"<< Console_WeldRecipePC.PIFrequencyLow << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"PI Frequency Low :";
				Console_WeldRecipePC.PIFrequencyLow = (INT32)ConsoleRead();
				CP->recipe["PIFrequencyLow"] = Console_WeldRecipePC.PIFrequencyLow;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PIFrequencyHigh"))
		{
			cout << "PIFrequencyHigh		: 	"<< Console_WeldRecipePC.PIFrequencyHigh << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"PI Frequency High :";
				Console_WeldRecipePC.PIFrequencyHigh = (INT32)ConsoleRead();
				CP->recipe["PIFrequencyHigh"] = Console_WeldRecipePC.PIFrequencyHigh;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PIPhaseLimitTime"))
		{
			cout << "PIPhaseLimitTime	: 	"<< Console_WeldRecipePC.PIPhaseLimitTime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"PI Phase Limit Time :";
				Console_WeldRecipePC.PIPhaseLimitTime = (INT32)ConsoleRead();
				CP->recipe["PIPhaseLimitTime"] = Console_WeldRecipePC.PIPhaseLimitTime;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PIPhaseLimit"))
		{
			cout << "PIPhaseLimit		: 	"<< Console_WeldRecipePC.PIPhaseLimit << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"PI Phase Limit :";
				Console_WeldRecipePC.PIPhaseLimit = (INT32)ConsoleRead();
				CP->recipe["PIPhaseLimit"] = Console_WeldRecipePC.PIPhaseLimit;
			}
		}
/*		if(IsUserMode(WELD_RECIPE,"PretriggerFlag"))
		{
			cout << "Pretrigger Flag		: 	"<< Console_WeldRecipeSC.PreTrigger.bIsEnabled << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Pretrigger Flag :";
				Console_WeldRecipeSC.PreTrigger.bIsEnabled = (INT16)ConsoleRead();
				CP->recipe["PretriggerFlag"] = Console_WeldRecipeSC.PreTrigger.bIsEnabled;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PretriggerType"))
		{
			cout << "Pretrigger Type		: 	"<< Console_WeldRecipeSC.PreTrigger.bIsAutoEnabled << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Pretrigger Type :";
				Console_WeldRecipeSC.PreTrigger.bIsAutoEnabled = (INT32)ConsoleRead();
				CP->recipe["PretriggerType"] = Console_WeldRecipeSC.PreTrigger.bIsAutoEnabled;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PretriggerAmplitude"))
		{
			cout << "Pretrigger Amplitude		: 	"<< Console_WeldRecipeSC.PreTrigger.TriggerAmplitude << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Pretrigger Amplitude :";
				Console_WeldRecipeSC.PreTrigger.TriggerAmplitude = (INT32)ConsoleRead();
				CP->recipe["PretriggerAmplitude"] = Console_WeldRecipeSC.PreTrigger.TriggerAmplitude;
			}
		}
		if(IsUserMode(WELD_RECIPE,"PretriggerDistance"))
		{
			cout << "Pretrigger Distance		: 	"<< Console_WeldRecipeSC.PretriggerDistance << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Pretrigger Distance :";
				Console_WeldRecipeSC.PretriggerDistance = (INT32)ConsoleRead();
				CP->recipe["PretriggerDistance"] = Console_WeldRecipeSC.PretriggerDistance;
			}
		}
		if(IsUserMode(WELD_RECIPE,"AfterburstFlag"))
		{
			cout << "Afterburst Flag		: 	"<< Console_WeldRecipeSC.AfterburstFlag << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Afterburst Flag :";
				Console_WeldRecipeSC.AfterburstFlag = (UINT16)ConsoleRead();
				CP->recipe["AfterburstFlag"] = Console_WeldRecipeSC.AfterburstFlag;
			}
		}		
		if(IsUserMode(WELD_RECIPE,"AfterburstDelay"))
		{
			cout << "Afterburst Delay(ms)		: 	"<< Console_WeldRecipeSC.AfterburstDelay << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Afterburst Delay :";
				Console_WeldRecipeSC.AfterburstDelay = (INT32)ConsoleRead();
				CP->recipe["AfterburstDelay"] = Console_WeldRecipeSC.AfterburstDelay;
			}
		}		
		if(IsUserMode(WELD_RECIPE,"AfterburstAmplitude"))
		{
			cout << "Afterburst Amplitude		: 	"<< Console_WeldRecipeSC.AfterburstAmplitude << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Afterburst Amplitude :";
				Console_WeldRecipeSC.AfterburstAmplitude = (UINT16)ConsoleRead();
				CP->recipe["AfterburstAmplitude"] = Console_WeldRecipeSC.AfterburstAmplitude;
			}
		}		
		if(IsUserMode(WELD_RECIPE,"AfterburstTime"))
		{
			cout << "Afterburst Time(ms)		: 	"<< Console_WeldRecipeSC.AfterburstTime << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"Afterburst Time :";
				Console_WeldRecipeSC.AfterburstTime = (INT32)ConsoleRead();
				CP->recipe["AfterburstTime"] = Console_WeldRecipeSC.AfterburstTime;
			}
		}	
*/		
		
		if(IsUserMode(WELD_RECIPE,"PreWeldSeek"))
		{
			cout << "PreWeldSeek		: 	" << Console_WeldRecipeSC.bIsPreWeldSeekEnabled << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"PreWeldSeek : ";
				Console_WeldRecipeSC.bIsPreWeldSeekEnabled = (INT32)ConsoleRead();
				CP->recipe["bIsPreWeldSeekEnabled"] = Console_WeldRecipeSC.bIsPreWeldSeekEnabled;
			}
		}
		
		
		if(IsUserMode(WELD_RECIPE,"bIsPostWeldSeekEnabled"))
		{
			cout << "PostWeldSeek		: 	"<< Console_WeldRecipeSC.bIsPostWeldSeekEnabled << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"PostWeldSeek :";
				Console_WeldRecipeSC.bIsPostWeldSeekEnabled = (INT32)ConsoleRead();
				CP->recipe["bIsPostWeldSeekEnabled"] = Console_WeldRecipeSC.bIsPostWeldSeekEnabled;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"TimedSeekPeriod"))
		{
			cout << "TimedSeekTime		: 	"<< Console_WeldRecipeSC.TimedSeekPeriod << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"TimedSeekTime :";
				Console_WeldRecipeSC.TimedSeekPeriod = (INT32)ConsoleRead();
				CP->recipe["TimedSeekPeriod"] = Console_WeldRecipeSC.TimedSeekPeriod;
			}
		}
		
		if(IsUserMode(WELD_RECIPE,"bIsTimedSeekEnabled"))
		{
			cout << "TimedSeek		: 	" << Console_WeldRecipeSC.bIsTimedSeekEnabled << endl;
			if(IsUserEdit() == EDIT)
			{
				cout<<"TimedSeek : ";
				Console_WeldRecipeSC.bIsTimedSeekEnabled = (INT32)ConsoleRead();
				CP->recipe["bIsTimedSeekEnabled"] = Console_WeldRecipeSC.bIsTimedSeekEnabled;
			}
		}
		if(IsUserMode(WELD_RECIPE,"StartFrequency"))
		{
			cout << "StartFrequency		: 	"<< Console_WeldRecipePC.StartFrequency << endl;
		}
	}
	else if(TIMER_INTERVAL_RECIPE == TypeOfRecipe)		
	{
		ValidVal = false;
		cout << "Timer Interval recipe configuration" << endl;
		if(IsUserMode(TIMER_INTERVAL_RECIPE,"TmrIntervalPC"))
		{
			ValidVal = false;
			cout << "Timer Interval PC(us)		: 	"<< Console_TimerInterval.TmrIntervalPC << endl;
			if(IsUserEdit() == EDIT)
			{
				while(!ValidVal)
				{
					cout<<"Timer Interval PC(us) :";
					Console_TimerInterval.TmrIntervalPC = (INT32)ConsoleRead();
					if(Console_TimerInterval.TmrIntervalPC < PC_MIN_TIMER_INTERVAL)
					{
						cout<<"\nInvalid Value-Lower than min value("<<PC_MIN_TIMER_INTERVAL<<")\n";
						Console_TimerInterval.TmrIntervalPC = TempTmrIntervalPC;
					}
					else if(Console_TimerInterval.TmrIntervalPC > PC_MAX_TIMER_INTERVAL)
					{
						cout<<"\nInvalid Value-Upper than max value("<<PC_MAX_TIMER_INTERVAL<<")\n";
						Console_TimerInterval.TmrIntervalPC = TempTmrIntervalPC;
					}
					else
					{
						ValidVal = true;
						CP->TimerIntervalRecipe["TmrIntervalPC"] = Console_TimerInterval.TmrIntervalPC;
					}
				}
			}

		}
	}
	else if(TEST_FUNC == TypeOfRecipe)
	{

		cout << "*******Test Function******* \n\n\n" << endl;
			
		cout << "PRESS ANY KEY TO STOP TEST \n\n\n" << endl;
		
		cout << "*************************** \n\n\n" << endl;
		
		char dummy;
		cin.get(dummy);
		iCmd = STOP_TEST;
		message.msgID = TO_UI_TASK_TEST_ABORT_CMD_REQ;
		this->SendMsgToUITask(UI_MSG_Q_ID,message);
		

		
	}
	else 
	{
		/* Yet to add in future */
	}
	cout << endl;
}

/**************************************************************************//**
* \brief   - Send event to enable the consoleApp.
*
* \param   - TASK_ID& tid, INT32 event type.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SendEvent(TASK_ID& tid, INT32 event)
{
	if(eventSend(tid,event) != OK)
	{
		cout << "Console_T : Event failed to invoke console task" << endl;
	}
}

/**************************************************************************//**
* \brief   - Sends CMD and Recipes to UI task message queue.
*
* \param   - MSG_Q_ID , Message&.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SendMsgToUITask(MSG_Q_ID &MSG_ID, Message &Msg)
{
	char *pMsgStr = NULL;
	pMsgStr = reinterpret_cast<char*>(&message);
	if(msgQSend(MSG_ID,pMsgStr,sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
	{
		cout << "Error in sending message to UI task" << endl;
	}
	pMsgStr = NULL;	
}

/**************************************************************************//**
* \brief   - Provides available functionality options to user.
*
* \param   - None
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::ProcessUserOptions()
{
	INT32 iTimeout		= 0;
	char *pMsgStr 		= NULL;
	UINT32 iEvents		= 0;
	
	if(eventReceive(CON_EVENT,EVENTS_WAIT_ANY,WAIT_FOREVER, &iEvents) != ERROR)
	{
		/* Sends the default recipes of PowerUp, Weld and Seek to UI task only at system power on */
		if(bIsPowerUp)
		{
			if(bIsAlarmReset)
			{
				iCmd = 0;
				ResetAlarm();
			}
			else
			{
				iCmd = SEEK_OP;
				//OnSysPowerUp();
				bIsPowerUp = false;
			}			
		}
		/* User menu for alarm reset whenever alarm raised */
		else if(bIsAlarmReset)
		{
#ifdef CONSOLE_ALARM_RESET
			ResetAlarm();
#endif
		}
		/* User menu for setup at every power on - Mandatory option to move further available options*/
		else if((!bIsPowerUp) && (bIsSetup))
		{
			taskDelay(TWO_SEC_DELAY);
			cout << "Select the operation" << endl;
			cout << "1.SETUP" << endl;
			cout << "2.EXIT" << endl;
			iChoice = ConsoleRead();			

			memset(message.Buffer,0x00,sizeof(message.Buffer));
			if(SETUP_INIT == iChoice)
			{
				/* Prepare message with SETUP CMD */
				iCmd = 0;
				message.msgID = TO_UI_TASK_SETUP_CMD;
				memset(message.Buffer,0x00,sizeof(message.Buffer));
				message.Buffer[0] = 3;			/* PART_CONTACT - Refer control task .h file for defined enum */
				SendMsgToUITask(UI_MSG_Q_ID,message);
			}
			else if(EXIT_INIT == iChoice)
			{
				/* EXIT from application */
				this->SendEvent(Main_TID,SHUTDOWN_EVENT);
			}
			else
			{
				cout << "Invalid selection" << endl;
				bIsValidOp 		  = true;
			}		
		}	
		/* Only on horn scan process - Ask user for abort or not */
		else if((!bIsPowerUp) && (!bIsSetup) && (ConsoleApp::bIsScanStart))
		{
			cout << "Do you wish to abort the scan?" << endl;
			cout << "1.SCAN ABORT" << endl;
			iChoice = ConsoleRead();
			
			if(iChoice == 1)
			{
				message.msgID = TO_UI_TASK_HORN_SCAN_ABORT_CMD;
				this->SendMsgToUITask(UI_MSG_Q_ID,message);
				bIsScanStart = false;
			}
		}
		/* User menu for MORE options available on ultra sonic weld system on event receive */
		else if((!bIsPowerUp) && (!bIsSetup))
		{
			taskDelay(TWO_SEC_DELAY);
			memset(message.Buffer,0x00,sizeof(message.Buffer));
			/* Update WeldRecipe with ExpectedPartContactPosition received from Setup */
			///Console_WeldRecipe.ACData.ExpectedPartContactPosition = CP->recipe["ExpectedPartContactPosition"];
			Console_WeldRecipeAC.ExpectedPartContactPosition = CP->recipe["ExpectedPartContactPosition"];
			Console_WeldRecipe.ACData.ExpectedPartContactPosition = Console_WeldRecipeAC.ExpectedPartContactPosition;
			
			/* Update WeldRecipe with StartFrequency value received from SEEK */
			Console_WeldRecipePC.StartFrequency = CP->recipe["StartFrequency"];
			Console_WeldRecipe.PCData.StartFrequency = Console_WeldRecipePC.StartFrequency;
			
			Console_WeldRecipePC.DigitalTune = CP->recipe["DigitalTune"];
			Console_WeldRecipe.PCData.DigitalTune = Console_WeldRecipePC.DigitalTune;

//#ifdef DEBUG
			cout << "StartFrequency: " << Console_WeldRecipePC.StartFrequency ;
			cout << "...DigTune" << Console_WeldRecipe.PCData.DigitalTune;
			cout << "...ExpectedPartContactPosition" << Console_WeldRecipeAC.ExpectedPartContactPosition << endl;
//#endif //DEBUG			
			
			cout << "Select the operation" << endl;
			cout << " 1. WELD" << endl;
			cout << " 2. SEEK" << endl;
			cout << " 3. UPDATE WELD RECIPE" << endl;
			cout << " 4. Find Part Contact" << endl;
			cout << " 5. Horn Down" << endl;
			cout << " 6. Jog Up" << endl;
			cout << " 7. Jog Down" << endl;
			cout << " 8. Retract" << endl;
			cout << " 9. SCAN START" << endl;
			cout << "10. SYSTEM CONFIGURATION" << endl;
			cout << "11. FWINFO" << endl;
			cout << "12. CALIBRATION" << endl;
			cout << "13. PC TIMER INTERVAL"<<endl;
			cout << "14. RUN TEST"<<endl;
			cout << "15. COPY DB: EMMC -> USB"<<endl;
			cout << "16. Delete DB: FROM EMMC"<<endl;
			cout << "17. Check DB size" << endl;
			cout << "18. Optimize DB" << endl;
			cout << "19. SHUTDOWN" << endl;
						
			iChoice = ConsoleRead();
			
			switch(iChoice)
			{
				case WELD_OP:
					/* Prepare message with WELD CMD */
					iCmd = WELD_OP;
					message.msgID = TO_UI_TASK_WELD_CMD;
					SendMsgToUITask(UI_MSG_Q_ID,message);
					break;
				
				case SEEK_OP:
					/* Prepare message with SEEK CMD */
					iCmd = SEEK_OP;
					message.msgID = TO_UI_TASK_SEEK_CMD;
					SendMsgToUITask(UI_MSG_Q_ID,message);
					break;
				
				case UPDATE_WELD_RECIPE:
					/*Accepts the updated weld recipe values and save */
					GetInputFromUser(WELD_RECIPE);
					SaveWeldRecipe(WELD_RECIPE);
					bIsRecipeOp = true;
					bIsValidOp 	= true;
					break;
				
				case OP_FIND_PART:
					/* Prepare message with SETUP CMD */
					iCmd = 0;
					message.msgID = TO_UI_TASK_SETUP_CMD;
					memset(message.Buffer,0x00,sizeof(message.Buffer));
					message.Buffer[0] = 3;			/* PART_CONTACT - Refer control task .h file for defined enum */
					SendMsgToUITask(UI_MSG_Q_ID,message);
					break;
				
				case RETRACT:
					/* Prepare message with SETUP CMD */
					iCmd = 0;
					message.msgID = TO_UI_TASK_SETUP_CMD;
					memset(message.Buffer,0x00,sizeof(message.Buffer));
					message.Buffer[0] = 9;			/* PART_CONTACT - Refer control task .h file for defined enum */
					SendMsgToUITask(UI_MSG_Q_ID,message);
					break;
				
				case OP_HORN_DOWN:
					/* Prepare message with SETUP CMD */
					iCmd = 0;
					message.msgID = TO_UI_TASK_SETUP_CMD;
					memset(message.Buffer,0x00,sizeof(message.Buffer));
					message.Buffer[0] = 7;			/* PART_CONTACT - Refer control task .h file for defined enum */
					SendMsgToUITask(UI_MSG_Q_ID,message);
					break;
				
				case OP_JOG_UP:
					/* Prepare message with SETUP CMD */
					iCmd = 0;
					message.msgID = TO_UI_TASK_SETUP_CMD;
					memset(message.Buffer,0x00,sizeof(message.Buffer));
					message.Buffer[0] = 4;			/* PART_CONTACT - Refer control task .h file for defined enum */
					SendMsgToUITask(UI_MSG_Q_ID,message);
					break;
				
				case OP_JOG_DOWN:
					/* Prepare message with SETUP CMD */
					iCmd = 0;
					message.msgID = TO_UI_TASK_SETUP_CMD;
					memset(message.Buffer,0x00,sizeof(message.Buffer));
					message.Buffer[0] = 5;			/* PART_CONTACT - Refer control task .h file for defined enum */
					SendMsgToUITask(UI_MSG_Q_ID,message);
					break;

				/*L1A features */
				case HORN_SCAN:
					/* Prepare message with HORN SCAN CMD */
					iCmd = HORN_SCAN;
					message.msgID = TO_UI_TASK_HORN_SCAN_CMD;	
					SendMsgToUITask(UI_MSG_Q_ID,message);
					bIsScanStart = true;
					break;

				case SYS_CONF:
					cout << "Display System Configuration\n"<< endl;
					DisplaySystemConfiguration();
					/* Provide provision to user for editing the frequency and amplitude */
					bIsValidOp 	= true;
					break;

				case FW_INFO:
					// previously attained versions
					printf ("---------- FIRMWARE INFO ----------\n");
					printf ("SC Major: %d\n",FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR));
					printf ("SC Minor: %d\n",FWVersion::Get(FW_VERSION_SC, VERSION_MINOR));
					printf ("SC Build: %d\n",FWVersion::Get(FW_VERSION_SC, VERSION_BUILD));
#ifdef MAINLINE_BUGFIX_BUILD
					printf ("SC DefectFixBuild: %d\n", SC_FW_VERSION_AUTOS);
#endif
					printf ("AC Major: %d\n",FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR));
					printf ("AC Minor: %d\n",FWVersion::Get(FW_VERSION_AC, VERSION_MINOR));
					printf ("AC Build: %d\n",FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
					printf ("PC Major: %d\n",FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR));
					printf ("PC Minor: %d\n",FWVersion::Get(FW_VERSION_PC, VERSION_MINOR));
					printf ("PC Build: %d\n",FWVersion::Get(FW_VERSION_PC, VERSION_BUILD));						
					bIsValidOp 	= true;
					break;
				
				case CALIBRATION_OP:
					/*Run calibration routine */
					CalibrationRoutine();
					break;
				case TMR_INTERVAL_PC_OP:
					SetTimerInterval();
					bIsValidOp 	= true;
					bIsRecipeOp = true;
					break;
				
				case COPY_DB_EMMC_USB:
					iCmd = COPY_DB_EMMC_USB;
					message.msgID = TO_UI_TASK_DB_FILE_COPY_TO_USB_REQ;
					SendMsgToUITask(UI_MSG_Q_ID,message);
					bIsValidOp 	= true;
					bIsRecipeOp = true;
					break;
									
				case DELETE_DB_EMMC:
					iCmd = DELETE_DB_EMMC;
					message.msgID = TO_UI_TASK_DB_FILE_DELETE_FROM_USB_REQ;
					SendMsgToUITask(UI_MSG_Q_ID,message);
					bIsValidOp 	= true;
					bIsRecipeOp = true;
					break;
					
				case RUN_TEST: 
					iCmd = RUN_TEST;
					message.msgID = TO_UI_TASK_TEST_CMD_REQ;
					this->SendMsgToUITask(UI_MSG_Q_ID,message);					
					this->GetInputFromUser(TEST_FUNC);
					bIsRecipeOp = true;
					bIsValidOp 	= true;
					break;
					
				case CHECK_DB_SIZE:
					iCmd = CHECK_DB_SIZE;
					ll(EMMC_DISK_PATH);
					bIsValidOp 	= true;
					bIsRecipeOp = true;
					break;
					
				case OPTIMIZE_DB:
					iCmd = OPTIMIZE_DB;
					OptimizeDB ();
					bIsValidOp 	= true;
					break;

				case SHUTDOWN:
					LOGDBG("ConsoleApp_T : Exit from consoleApp",0,0,0);
					SendEvent(CTRL_TID,SHUTDOWN_EVENT);
					break;
			
				default:
					cout << "Invalid Input.Insert the valid input" << endl;
					bIsValidOp 		 = true;
					break;
			}
		}
		else
		{
			/* Future enhancement */ 
		}
		
		if(bIsValidOp)
		{
			if(bIsRecipeOp)
			{
				taskDelay(ONE_SEC_DELAY);
				bIsRecipeOp = false;
			}
			bIsValidOp 	= false;
			SendEvent(Console_TID,CON_EVENT);
		}
	}
	else
	{
		cout << "ConsoleApp_T : Event receive failed" << endl;
	}
}

/**************************************************************************//**
* \brief   - Get the task life time status.
*
* \param   - None
*
* \return  - bool.
*
*****************************************************************************/
bool ConsoleApp::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}


/**************************************************************************//**
* \brief   - Assign Console_Task id with DS.
*
* \param   - string Task name, TASK_ID TID.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::SetTaskId(string T_Name, TASK_ID TID)
{
	Console_TID = TID;
	CP->SetTaskId(T_Name,TID);
}

/**************************************************************************//**
* \brief   - Cleans the memory related to ConsoleApp class.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
ConsoleApp::~ConsoleApp()
{
	CP = NULL;
}

/**************************************************************************//**
* \brief   - Taking the user input.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp_Task()
{	
	ConsoleApp *CA = new ConsoleApp();
	TASK_ID tid = taskIdSelf();
	
	CA->SetTaskId(cTaskName[CONSOLE_T], tid);

	CA->OnSysPowerUp();
	
	/* Responsible for displaying the user menu options based on system sequence */  
	while(CA->bIsTaskRunStatus())
	{
		CA->ProcessUserOptions();
	}
	
	delete CA;
	CA = NULL;
	LOGDBG("ConsoleApp_T: ----------------ConsoleApp Task Terminated----------------",0,0,0);
	taskDelete(tid);
}

/**************************************************************************//**
* \brief   - DisplaySystemConfiguration.
*
* \param   - None.
*
* \return  - None.
*
*****************************************************************************/
void ConsoleApp::DisplaySystemConfiguration() 
{
	SystemConfiguration SysConfig;
	intf_SysConfig_t SysConfigInfo;
	char buffer[512] = {0x00};
	
	SysConfig.SysConfigGet(buffer);
	memcpy(&SysConfigInfo,buffer,sizeof(SysConfigInfo));
	Console_PowerUpRecipe.Power 	= SysConfigInfo.PcConfig.PSWatt;
	Console_PowerUpRecipe.Frequency = SysConfigInfo.PcConfig.PSFreq;
}


/**************************************************************************//**
* \brief  - Method for calibration which propmts user for force readings
* \brief  - Coordinates with control task to proced to next step after each user input
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void ConsoleApp::CalibrationRoutine()
{
	message.msgID = TO_UI_TASK_CALIBRATION_CMD_REQ;
	SendMsgToUITask(UI_MSG_Q_ID,message);
	taskDelay(ONE_SEC_DELAY);
	cout << "Place external force gauge under the horn and hold start switches until force is applied" << endl;
	
	taskDelay(ONE_SEC_DELAY);
	cout << "Enter first force reading (N): ";
	Console_CalibrationReadings.FirstReading = (UINT32)ConsoleRead();
	message.msgID = TO_UI_TASK_CALIBRATION_CMD_REQ;
	SendMsgToUITask(UI_MSG_Q_ID,message);
	
	taskDelay(ONE_SEC_DELAY);
	cout << "Enter second force reading (N): ";
	Console_CalibrationReadings.SecondReading = (UINT32)ConsoleRead();
	message.msgID = TO_UI_TASK_CALIBRATION_CMD_RES;
	memcpy(&message.Buffer[0],&Console_CalibrationReadings, sizeof(Console_CalibrationReadings));
	SendMsgToUITask(UI_MSG_Q_ID,message);

}

// *************** active weld recipe from DB

/**************************************************************************//**
* \brief  - Connects to a DB (default opened DB)
*
* \param  - none
*
*
* \return  - UINT32 ErrCode
*
******************************************************************************/
UINT32 ConsoleApp::ConnectDB()
{
	UINT32 ErrCode;
	
	ErrCode = m_DbConn.EstablishDataBaseConnection();

	if(ErrCode != SQLITE_OK)
		LOGERR("Error DB Connection not opened",0,0,0);
	
	return ErrCode;
}


/**************************************************************************//**
* \brief  - Disconnects from a DB (default opened DB)
*
* \param  - None
*
* \return  - None
*
******************************************************************************/
void ConsoleApp::CloseDB()
{
	m_DbConn.Close();
#ifdef DEBUG
	LOGDBG("DB Connection closed",0,0,0);
#endif

}

/**************************************************************************//**
* \brief  - Attempts to pull last active recipe number from DB
*
* \param  - none
*
* \return  - INT32 recipeNum
*
******************************************************************************/
INT32 ConsoleApp::ObtainActiveRecipeNumber ()
{
	INT32 recipeNum = -1;
	
	if (ConnectDB() == SQLITE_OK)
	{
		std::string query = "Select RecipeNumber,WeldRecipeName,CompanyName,WeldMode,ModeValue,IsActive,IsLocked,IsValidate from WeldRecipeTable where IsActive=1 and RecipeNumber>0 and WeldMode IN (1,2,3,4,5,6);";
		std::string recipeStr = m_DbConn.Exec((char*) query.c_str());
		
		if (strlen(recipeStr.c_str()) > 0)
		{
			UINT16 offset = recipeStr.find(",", 0);
			
			if (offset > 0)
				recipeNum = atoi (recipeStr.substr(0, offset).c_str());
		}

#ifdef AWR_DBG
			printf ("\nConsoleApp::ObtainActiveRecipeNumber:  -------------------> recipeNumber: %d\n",recipeNum);
#endif
		
	}
    
    return recipeNum;
}


/**************************************************************************//**
* \brief  - Pull last active weld recipe from DB, assign values into
* 			console weld structures
*
* \param  - INT32 activeRecipeID
*
* \return  - INT32 status
*
******************************************************************************/
INT32 ConsoleApp::PullActiveRecipeFromDB (INT32 activeRecipeID)
{
	INT32 	status = OK;
	char 	query[MSG_SIZE] = {0x00};	
	string 	activeWeldRecipe, SRActiveRecipe;
	
	// Read the SC, AC & PC active recipe data from DB
	sprintf(query, "SELECT * FROM WeldRecipeTable WHERE RecipeNumber = %d;",activeRecipeID);	
	
	activeWeldRecipe = m_DbConn.Exec (query, &status);
	
	if(SQLITE_OK != status)
	{					
		LOGERR("CON_T : Get active recipe : Err Code from DB : %d", status, 0, 0);
	}	
	else
	{	
#ifdef AWR_DBG
	printf ("\nConsoleApp::PullActiveRecipeFromDB:  -------------------> status 0: %d\n",status);
#endif

		// Read the suspect reject active recipe data from DB
		memset(query, 0x00, sizeof(query));
		sprintf(query, "SELECT * FROM SuspectRejectLimits WHERE RecipeNumber = %d;",activeRecipeID);
		
		SRActiveRecipe = m_DbConn.Exec (query, &status);
		
		if(SQLITE_OK != status)
		{					
			LOGERR("CON_T : Get suspect reject active recipe : Err Code from DB : %d", status, 0, 0);					
		}	
		else
		{
			
#ifdef AWR_DBG
	printf ("\nConsoleApp::PullActiveRecipeFromDB:  -------------------> status 1: %d\n",status);
#endif
			
			// Copy out the active recipe
			if( (strlen (activeWeldRecipe.c_str()) > 0) && (strlen (SRActiveRecipe.c_str()) > 0))
			{
				status = CopyActiveRecipe (activeWeldRecipe);
				
				if(status != OK)
				{
					LOGERR("----------- CON_T : Failed to copy active recipe -----------", 0, 0, 0);	
				}
				else
				{
#ifdef AWR_DBG
	printf ("\nConsoleApp::PullActiveRecipeFromDB:  -------------------> status 2: %d\n",status);
#endif
					status = CopySRActiveRecipe (Console_WeldRecipeSC, SRActiveRecipe);
					
					if(status != OK)
					{
						LOGERR("----------- CON_T : Failed to copy suspect reject active recipe -----------", 0, 0, 0);	
					}
					else
					{
						
#ifdef AWR_DBG
	printf ("\nConsoleApp::PullActiveRecipeFromDB:  -------------------> status 3: %d\n",status);
#endif
						// and finally load the stack recipe
						status = PullStackRecipeFromDB ();
						
						if(status != OK)
						{
							LOGERR("----------- CON_T : Failed to copy stack recipe -----------", 0, 0, 0);	
						}
					}
				}
			}
		}	
	}	
	
	return status;
}


/**************************************************************************//**
* \brief  - Pulls last stack recipe from DB, assigns values to context
*
* \param  - None
*
* \return - INT32 status
*
******************************************************************************/
INT32 ConsoleApp::PullStackRecipeFromDB ()
{
	INT32  	endofWeldStore		= 0;
	INT32  	internalOffsetFlag	= 0;
	INT32  	digitalTune 		= 0; 
	INT32  	internalFreqOffset	= 0;
	INT32  	status 				= ERROR;
	string 	psFreq, qstrQuery, stackRecipe;
	std::string m_DefaultStackRecipe="";
	std::string ReturnQuery="";
	
	switch(CP->powerupRecipe.Frequency)
	{
		 case FREQ_20KHZ:
	        m_DefaultStackRecipe="19950,0,0,0";
	        break;
	     case FREQ_30KHZ:
	    	 m_DefaultStackRecipe="30000,0,0,0";
	    	break;
		case FREQ_40KHZ:
			m_DefaultStackRecipe="39900,0,0,0";
			break;
		default:
			LOGERR("ConsoleApp: Undefined system",0, 0, 0);
			break;
	
	}
	
	qstrQuery = "select count(*) from StackRecipeTable  where RecipeNumber =(select RecipeNumber from WeldRecipeTable where IsActive=1);";
	ReturnQuery=m_DbConn.Exec ((char*) qstrQuery.c_str(), &status);
	
	if (atoi(ReturnQuery.c_str())==1)
    {
	  qstrQuery = "select DigitialTune,InternalOffsetFlag,InternalFreqOffset,EndofWeldStore from StackRecipeTable  where RecipeNumber =(select RecipeNumber from WeldRecipeTable where IsActive=1);";
    
      // get the recipe
       stackRecipe = m_DbConn.Exec ((char*) qstrQuery.c_str(), &status);
    }
	else
	{
	  stackRecipe = m_DefaultStackRecipe;
	}
    
	if(SQLITE_OK == status)
	{						
#ifdef AWR_DBG
	printf ("\nConsoleApp::PullActiveRecipeFromDB:  -------------------> status 4: %d\n",status);
#endif
	
		if(strlen(stackRecipe.c_str()) > 0)
		{
			// parse it
			if (OK == ParseStackRecipe (stackRecipe, endofWeldStore, internalOffsetFlag, digitalTune, internalFreqOffset))
			{
				// save it
				if (OK == LoadStackRecipe (endofWeldStore, internalOffsetFlag, digitalTune, internalFreqOffset))
				{
#ifdef AWR_DBG
	printf ("\nConsoleApp::PullActiveRecipeFromDB:  -------------------> status 5: %d\n",status);
#endif
					status = OK;
				}
			}
		}
	}
	
	return status;
}


/**************************************************************************//**
* \brief  - Parses last stack recipe string from DB, assigns values to the
* 			various members 
*
* \param  - string& stackRecipe
* \param  - INT32& 	endofWeldStore
* \param  - INT32& 	internalOffsetFlag
* \param  - INT32& 	digitalTune
* \param  - INT32& 	internalFreqOffset
*
* \return - INT32 status
*
******************************************************************************/
INT32 ConsoleApp::ParseStackRecipe(string& stackRecipe, INT32& endofWeldStore,
		INT32& internalOffsetFlag, INT32& digitalTune, INT32& internalFreqOffset)
{
	UINT16 offset1, offset2, offset3, offset4;
	INT32 status = ERROR;
	
	// possible query results: stackdRecipe: 19985,0,0,0
	
    // get settings
    offset1 = stackRecipe.find(",", 0);
    
	if (offset1)
	{
		digitalTune = atoi (stackRecipe.substr(0, offset1).c_str());
	    offset2 = stackRecipe.find(",", offset1);
	    
	    if (offset2)
	    {
	    	internalOffsetFlag = atoi (stackRecipe.substr(offset1+1, offset2).c_str());
	    	
	    	offset3 = stackRecipe.find(",", offset2);
	
	    	if (offset3)
		    {
		    	internalFreqOffset = atoi (stackRecipe.substr(offset2+1, offset3).c_str());
		    	
	    		offset4 = stackRecipe.size();
		    	
	    		endofWeldStore = atoi (stackRecipe.substr(offset3+1, offset4).c_str());
	    		
#ifdef AWR_DBG
	    		printf ("\nConsoleApp::ParseStackRecipe:  digitalTune: %d internalOffsetFlag: %d internalFreqOffset: %d endofWeldStore: %d\n"
	    				,digitalTune,internalOffsetFlag,internalFreqOffset,endofWeldStore);
#endif
		    	status = OK;
		    }
	    }
	}
	
	return status;
}

/**************************************************************************//**
* \brief  - Run optimization on the DB by using the vacuum query.
* 			The free-list displays while the operation takes place.
*			The optimization can take anywhere between 2-60
* 			seconds therefore should not be performed while cycling.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
void ConsoleApp::OptimizeDB ()
{
	string 	DbQuery, freeList;
	INT32 	queryStatus = SQLITE_ERROR;	
	float startTime;
	float endTime;
	float elapsedTime;
		
	if (ConnectDB() == SQLITE_OK)
	{
		DbQuery.clear();
		DbQuery = "PRAGMA freelist_count;";
		freeList = m_DbConn.Exec((char *)DbQuery.c_str(), &queryStatus);
		
		if(SQLITE_OK == queryStatus)
		{			
			printf ("\n%sDataBase free records count: %s Optimizing...%s\n", KNRM, freeList.c_str(), KNRM);
			
			startTime = TimeStamp();
			m_DbConn.DBVacuum();
			endTime = TimeStamp();
			elapsedTime = endTime-startTime;
			printf ("\n%s %s records optimized in %f seconds. %s\n\n", KGRN, freeList.c_str(), elapsedTime,KNRM);
		}
		else
		{
			printf ("\n\n%sRecords optimization failed.%s\n\n", KRED, KNRM);
		}
		
		CloseDB();
	}
	else
		printf ("\n\n%sRecords optimization failed.%s\n\n", KRED, KNRM);
}

/**************************************************************************//**
* \brief  - Loads last stack recipe members to the active recipe and notifies
* 			the power supply controller
* 			
* \param  - INT32 	endofWeldStore
* \param  - INT32 	internalOffsetFlag
* \param  - INT32 	digitalTune
* \param  - INT32 	internalFreqOffset
*
* \return - INT32 status
*
******************************************************************************/
INT32 ConsoleApp::LoadStackRecipe(INT32 endofWeldStore,
		INT32 internalOffsetFlag, INT32 digitalTune, INT32 internalFreqOffset)
{
	INT32 status = ERROR;
	
	// save in stack recipe
	CP->stackRecipe.SetData (StackRecipe::DIGITAL_TUNE, (digitalTune*10));
	CP->stackRecipe.SetData (StackRecipe::INTERNAL_OFFSET_FLAG, internalOffsetFlag);
	CP->stackRecipe.SetData (StackRecipe::INTERNAL_FREQ_OFFSET, internalFreqOffset);
	CP->stackRecipe.SetData (StackRecipe::END_OF_WELD_STORE, endofWeldStore);
	
	// update common part
	CP->recipe["MemoryOffset"] = 0;
	CP->recipe["StartFrequency"] = CP->stackRecipe.GetData (StackRecipe::DIGITAL_TUNE);
	CP->recipe["DigitalTune"] = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
	CP->recipe["FrequencyOffset"] = CP->stackRecipe.GetData(StackRecipe::INTERNAL_FREQ_OFFSET);
	
	// send it off to the PC
    CP->SetDigitalTune (CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));
			
	status = OK;
	return status;
}


/**************************************************************************//**
* \brief   - Copies active recipe to SC object
*
* \param   - string& activeRecipe 
*
* \return  - UINT32 status
*
******************************************************************************/
INT32 ConsoleApp::CopyActiveRecipe (string& activeRecipe)
{
	string token;	
	UINT16 start = 0, tokenNum = 0;
	INT32 status = ERROR;
	INT32 found;
	
	while(tokenNum < (NUM_OF_ACTIVE_RECIPE_DB_DATA - 1))	
	{
		found = activeRecipe.find(",", start);
		
		if (found >= 0) 									 
		{  													
			// Each substring here is the token
			token = activeRecipe.substr(start, found);  			
			tokenNum++;		
			start = found + 1;
			switch (tokenNum) 
			{			
			case SC_RECIPE_NUMBER:
				Console_WeldRecipeSC.RecipeNumber = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC RecipeNumber: " << Console_WeldRecipeSC.RecipeNumber << "\n";
#endif	
				break;

			case SC_RECIPE_NAME:
				break;

			case SC_RECIPE_REV_NUMBER:				
				Console_WeldRecipeSC.RecipeRevNumber = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC RecipeRevNumber: " << Console_WeldRecipeSC.RecipeRevNumber << "\n";
#endif	
				break;

			case SC_WELD_MODE:				
				Console_WeldRecipeSC.WeldMode = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC WeldMode: " << Console_WeldRecipeSC.WeldMode << "\n";
#endif		
				break;

			case SC_MODE_VALUE:				
				Console_WeldRecipeSC.ModeValue = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC ModeValue: " << Console_WeldRecipeSC.ModeValue << "\n";
#endif	
				break;

			case SC_HOLD_TIME:				
				Console_WeldRecipeSC.HoldTime = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC HoldTime: " << Console_WeldRecipeSC.HoldTime << "\n";
#endif		
				break;

			case SC_TRIGGER_FORCE:				
				Console_WeldRecipeSC.TriggerForce = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC TriggerForce: " << Console_WeldRecipeSC.TriggerForce << "\n";
#endif	
				break;

			case SC_TRIGGER_DISTANCE:				
				Console_WeldRecipeSC.TriggerDistance = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC TriggerDistance: " << Console_WeldRecipeSC.TriggerDistance << "\n";
#endif	
				break;

			case SC_NUM_AMP_STEPS:				
				Console_WeldRecipeSC.NumAmplitudeSteps = atoi(token.c_str());			
#ifdef CONSOLE_DBG									
				printf("SC NumAmplitudeSteps: %u\n", Console_WeldRecipeSC.NumAmplitudeSteps);
#endif	
				break;

			case SC_AMP_STEP_AT:				
				Console_WeldRecipeSC.AmplitudeStepAt = atoi(token.c_str());		
#ifdef CONSOLE_DBG									
				printf("SC AmplitudeStepAt: %u\n", Console_WeldRecipeSC.AmplitudeStepAt);
#endif
				break;

			case SC_AMP_STEP_VALUE_0:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_ZERO] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[0]: " << Console_WeldRecipeSC.AmplitudeStepValue[INDEX_ZERO] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_1:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_ONE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[1]: " << Console_WeldRecipeSC.AmplitudeStepValue[INDEX_ONE] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_2:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_TWO] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[2]: " << Console_WeldRecipeSC.AmplitudeStepValue[2] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_3:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_THREE] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[3]: " << Console_WeldRecipeSC.AmplitudeStepValue[3] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_4:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_FOUR] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[4]: " << Console_WeldRecipeSC.AmplitudeStepValue[4] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_5:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_FIVE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[5]: " << Console_WeldRecipeSC.AmplitudeStepValue[5] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_6:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_SIX] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[6]: " << Console_WeldRecipeSC.AmplitudeStepValue[6] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_7:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_SEVEN] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[7]: " << Console_WeldRecipeSC.AmplitudeStepValue[7] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_8:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_EIGHT] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[8]: " << Console_WeldRecipeSC.AmplitudeStepValue[8] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_9:				
				Console_WeldRecipeSC.AmplitudeStepValue[INDEX_NINE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStepValue[9]: " << Console_WeldRecipeSC.AmplitudeStepValue[9] << "\n";
#endif
				break;

			case SC_AMP_STEP_0:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_ZERO] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[0]: " << Console_WeldRecipeSC.AmplitudeStep[0] << "\n";
#endif
				break;

			case SC_AMP_STEP_1:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_ONE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[1]: " << Console_WeldRecipeSC.AmplitudeStep[1] << "\n";
#endif
				break;

			case SC_AMP_STEP_2:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_TWO] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[2]: " << Console_WeldRecipeSC.AmplitudeStep[2] << "\n";
#endif
				break;

			case SC_AMP_STEP_3:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_THREE] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[3]: " << Console_WeldRecipeSC.AmplitudeStep[3] << "\n";
#endif
				break;

			case SC_AMP_STEP_4:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_FOUR] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[4]: " << Console_WeldRecipeSC.AmplitudeStep[4] << "\n";
#endif
				break;

			case SC_AMP_STEP_5:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_FIVE] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[5]: " << Console_WeldRecipeSC.AmplitudeStep[5] << "\n";
#endif
				break;

			case SC_AMP_STEP_6:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_SIX] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[6]: " << Console_WeldRecipeSC.AmplitudeStep[6] << "\n";
#endif
				break;

			case SC_AMP_STEP_7:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_SEVEN] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[7]: " << Console_WeldRecipeSC.AmplitudeStep[7] << "\n";
#endif
				break;

			case SC_AMP_STEP_8:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_EIGHT] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[8]: " << Console_WeldRecipeSC.AmplitudeStep[8] << "\n";
#endif
				break;

			case SC_AMP_STEP_9:				
				Console_WeldRecipeSC.AmplitudeStep[INDEX_NINE] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AmplitudeStep[9]: " << Console_WeldRecipeSC.AmplitudeStep[9] << "\n";
#endif
				break;

			case AC_WELD_FORCE:				
				Console_WeldRecipeAC.WeldForce = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC WeldForce: " << Console_WeldRecipeAC.WeldForce << "\n";
#endif
				break;

			case AC_FORCE_RAMP_TIME:				
				Console_WeldRecipeAC.ForceRampTime = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC ForceRampTime: " << Console_WeldRecipeAC.ForceRampTime << "\n";
#endif
				break;

			case AC_TOTAL_COLLAPSE_TARGET:				
				Console_WeldRecipeAC.TotalCollapseTarget = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC TotalCollapseTarget: " << Console_WeldRecipeAC.TotalCollapseTarget << "\n";
#endif
				break;

			case AC_HOLD_FORCE:				
				Console_WeldRecipeAC.HoldForce = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC HoldForce: " << Console_WeldRecipeAC.HoldForce << "\n";
#endif
				break;

			case AC_HOLD_FORCE_RAMP_TIME:				
				Console_WeldRecipeAC.HoldForceRampTime = atoi(token.c_str());		
#ifdef CONSOLE_DBG					
				cout << "AC HoldForceRampTime: " << Console_WeldRecipeAC.HoldForceRampTime << "\n";
#endif
				break;

			case AC_EXPECTED_PC_POSITION:				
				Console_WeldRecipeAC.ExpectedPartContactPosition = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ExpectedPartContactPosition: " << Console_WeldRecipeAC.ExpectedPartContactPosition << "\n";
#endif
				break;

			case AC_READY_POSITION:				
				Console_WeldRecipeAC.ReadyPosition = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ReadyPosition: " << Console_WeldRecipeAC.ReadyPosition << "\n";
#endif
				break;

			case AC_DOWN_ACCELERATION:				
				Console_WeldRecipeAC.DownAcceleration = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC DownAcceleration: " << Console_WeldRecipeAC.DownAcceleration << "\n";
#endif
				break;

			case AC_DOWN_MAX_VELOCITY:				
				Console_WeldRecipeAC.DownMaxVelocity = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC DownMaxVelocity: " << Console_WeldRecipeAC.DownMaxVelocity << "\n";
#endif
				break;

			case AC_DOWN_DECELERATION:				
				Console_WeldRecipeAC.DownDeceleration = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC DownDeceleration: " << Console_WeldRecipeAC.DownDeceleration << "\n";
#endif
				break;

			case AC_RETURN_ACCELERATION:				
				Console_WeldRecipeAC.ReturnAcceleration = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ReturnAcceleration: " << Console_WeldRecipeAC.ReturnAcceleration << "\n";
#endif
				break;

			case AC_RETURN_MAX_VELOCITY:				
				Console_WeldRecipeAC.ReturnMaxVelocity = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC ReturnMaxVelocity: " << Console_WeldRecipeAC.ReturnMaxVelocity << "\n";
#endif
				break;

			case AC_RETURN_DECELERATION:				
				Console_WeldRecipeAC.ReturnDeceleration = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ReturnDeceleration: " << Console_WeldRecipeAC.ReturnDeceleration << "\n";
#endif
				break;

			case SC_NUM_FORCE_STEPS:				
				Console_WeldRecipeSC.NumForceSteps = atoi(token.c_str());				
#ifdef CONSOLE_DBG									
				printf("SC NumForceSteps: %u\n", Console_WeldRecipeSC.NumForceSteps);
#endif
				break;

			case SC_FORCE_STEP_AT:				
				Console_WeldRecipeSC.ForceStepAt = atoi(token.c_str());	
#ifdef CONSOLE_DBG									
				printf("SC ForceStepAt: %u\n", Console_WeldRecipeSC.ForceStepAt);
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_0:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_ZERO] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[0]: " << Console_WeldRecipeSC.ForceStepAtValue[0] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_1:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_ONE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[1]: " << Console_WeldRecipeSC.ForceStepAtValue[1] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_2:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_TWO] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[2]: " << Console_WeldRecipeSC.ForceStepAtValue[2] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_3:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_THREE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[3]: " << Console_WeldRecipeSC.ForceStepAtValue[3] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_4:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_FOUR] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[4]: " << Console_WeldRecipeSC.ForceStepAtValue[4] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_5:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_FIVE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[5]: " << Console_WeldRecipeSC.ForceStepAtValue[5] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_6:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_SIX] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[6]: " << Console_WeldRecipeSC.ForceStepAtValue[6] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_7:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_SEVEN] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[7]: " << Console_WeldRecipeSC.ForceStepAtValue[7] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_8:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_EIGHT] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[8]: " << Console_WeldRecipeSC.ForceStepAtValue[8] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_9:				
				Console_WeldRecipeSC.ForceStepAtValue[INDEX_NINE] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC ForceStepAtValue[9]: " << Console_WeldRecipeSC.ForceStepAtValue[9] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_0:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_ZERO] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[0]: " << Console_WeldRecipeAC.ForceStepRampTime[0] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_1:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_ONE] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[1]: " << Console_WeldRecipeAC.ForceStepRampTime[1] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_2:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_TWO] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[2]: " << Console_WeldRecipeAC.ForceStepRampTime[2] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_3:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_THREE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[3]: " << Console_WeldRecipeAC.ForceStepRampTime[3] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_4:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_FOUR] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[4]: " << Console_WeldRecipeAC.ForceStepRampTime[4] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_5:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_FIVE] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[5]: " << Console_WeldRecipeAC.ForceStepRampTime[5] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_6:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_SIX] = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[6]: " << Console_WeldRecipeAC.ForceStepRampTime[6] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_7:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_SEVEN] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[7]: " << Console_WeldRecipeAC.ForceStepRampTime[7] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_8:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_EIGHT] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[8]: " << Console_WeldRecipeAC.ForceStepRampTime[8] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_9:				
				Console_WeldRecipeAC.ForceStepRampTime[INDEX_NINE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepRampTime[9]: " << Console_WeldRecipeAC.ForceStepRampTime[9] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_0:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_ZERO] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[0]: " << Console_WeldRecipeAC.ForceStepForce[0] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_1:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_ONE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[1]: " << Console_WeldRecipeAC.ForceStepForce[1] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_2:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_TWO] = atoi(token.c_str());		
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[2]: " << Console_WeldRecipeAC.ForceStepForce[2] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_3:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_THREE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[3]: " << Console_WeldRecipeAC.ForceStepForce[3] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_4:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_FOUR] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[4]: " << Console_WeldRecipeAC.ForceStepForce[4] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_5:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_FIVE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[5]: " << Console_WeldRecipeAC.ForceStepForce[5] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_6:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_SIX] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[6]: " << Console_WeldRecipeAC.ForceStepForce[6] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_7:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_SEVEN] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[7]: " << Console_WeldRecipeAC.ForceStepForce[7] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_8:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_EIGHT] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[8]: " << Console_WeldRecipeAC.ForceStepForce[8] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_9:				
				Console_WeldRecipeAC.ForceStepForce[INDEX_NINE] = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC ForceStepForce[9]: " << Console_WeldRecipeAC.ForceStepForce[9] << "\n";
#endif
				break;

			case PC_WELD_RAMP_TIME:				
				Console_WeldRecipePC.WeldRampTime = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "PC WeldRampTime: " << Console_WeldRecipePC.WeldRampTime << "\n";
#endif
				break;

			case SC_AFTER_BURST_ENABLED:				
				Console_WeldRecipeSC.AfterBurst.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG									
				printf("SC AfterBurst.bIsEnabled: %u\n", Console_WeldRecipeSC.AfterBurst.bIsEnabled);
#endif
				break;

			case SC_AFTER_BURST_DELAY:				
				Console_WeldRecipeSC.AfterBurst.BurstDelay = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AfterBurst.BurstDelay: " << Console_WeldRecipeSC.AfterBurst.BurstDelay << "\n";
#endif
				break;

			case SC_AFTER_BURST_TIME:				
				Console_WeldRecipeSC.AfterBurst.BurstTime = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC AfterBurst.BurstTime: " << Console_WeldRecipeSC.AfterBurst.BurstTime << "\n";
#endif								
				break;

			case SC_AFTER_BURST_AMP:				
				Console_WeldRecipeSC.AfterBurst.BurstAmplitude = atoi(token.c_str());
#ifdef CONSOLE_DBG									
				printf("SC AfterBurst.BurstAmplitude: %u\n", Console_WeldRecipeSC.AfterBurst.BurstAmplitude);
#endif				
				break;

			case SC_PRETRIGGER_ENABLED:				
				Console_WeldRecipeSC.PreTrigger.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG									
				printf("SC PreTrigger.bIsEnabled: %u\n", Console_WeldRecipeSC.PreTrigger.bIsEnabled);
#endif
				break;
																				
			case SC_PRETRIGGER_AUTO_ENABLED:					
				Console_WeldRecipeSC.PreTrigger.bIsAutoEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG									
				printf("SC PreTrigger.bIsAutoEnabled: %u\n", Console_WeldRecipeSC.PreTrigger.bIsAutoEnabled);
#endif
				break;

			case SC_PRETRIGGER_DISTANCE_ENABLED:					
				Console_WeldRecipeSC.PreTrigger.bIsDistanceEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG									
				printf("SC PreTrigger.bIsDistanceEnabled: %u\n", Console_WeldRecipeSC.PreTrigger.bIsDistanceEnabled);
#endif
				break;								
											
			case SC_PRETRIGGER_AMP:				
				Console_WeldRecipeSC.PreTrigger.TriggerAmplitude = atoi(token.c_str());
#ifdef CONSOLE_DBG									
				printf("SC PreTrigger.TriggerAmplitude: %u\n", Console_WeldRecipeSC.PreTrigger.TriggerAmplitude);
#endif
				break;

			case SC_PRETRIGGER_DISTANCE:				
				Console_WeldRecipeSC.PreTrigger.PreTriggerDistance = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC PreTrigger.PreTriggerDistance: " << Console_WeldRecipeSC.PreTrigger.PreTriggerDistance << "\n";
#endif		
				break;
																							
			case SC_ENERGY_BRAKE_ENABLED:
				Console_WeldRecipeSC.EnergyBrake.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC EnergyBrake.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ENERGY_BRAKE_TIME:
				Console_WeldRecipeSC.EnergyBrake.EnergyBrakeTime = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC EnergyBrake.Time: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ENERGY_BRAKE_AMP:
				Console_WeldRecipeSC.EnergyBrake.EnergyBrakeAmplitude = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC EnergyBrake.Amplitude: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_COOLING_VALVE:				
				Console_WeldRecipeSC.bIsCoolingValveEnabled = atoi(token.c_str());	
#ifdef CONSOLE_DBG									
				printf("SC bIsCoolingVavleEnabled: %u\n", Console_WeldRecipeSC.bIsCoolingValveEnabled);
#endif
				break;

			case SC_MAX_WELD_TIMEOUT:				
				Console_WeldRecipeSC.MaxWeldTimeout = atoi(token.c_str());	
#ifdef CONSOLE_DBG					
				cout << "SC MaxWeldTimeout: " << Console_WeldRecipeSC.MaxWeldTimeout << "\n";
#endif
				break;
				
			case SC_TIMED_SEEK_ENABLED:
				Console_WeldRecipeSC.bIsTimedSeekEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC TimedSeek: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_PRE_WELD_SEEK_ENABLED:
				Console_WeldRecipeSC.bIsPreWeldSeekEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC PreWeldSeek: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_POST_WELD_SEEK_ENABLED:
				Console_WeldRecipeSC.bIsPostWeldSeekEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC PostWeldSeek: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_TIMED_SEEK_PERIOD:
				Console_WeldRecipeSC.TimedSeekPeriod = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC TimedSeekPeriod: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case AC_PC_WINDOW_MINUS:
				Console_WeldRecipeAC.PartContactWindowMinus = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC PartContactWindowMinus: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case AC_PC_WINDOW_PLUS:
				Console_WeldRecipeAC.PartContactWindowPlus = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC PartContactWindowPlus: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case AC_PC_WINDOW_OFFSET:
				Console_WeldRecipeAC.ExpectedPartContactOffset = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "AC PartContactWindowOffset: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_GLOBAL_CONTROL:
#ifdef CONSOLE_DBG					
				cout << "SC GlobalControl: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_PEAK_POWER_CUTOFF:
				Console_WeldRecipeSC.CutoffLimit.PeakPowerCutoff.Value = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC PeakPowerCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_PEAK_POWER_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.PeakPowerCutoff.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC PeakPowerCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_LOW_CUTOFF:
				Console_WeldRecipeSC.CutoffLimit.FrequencyLowCutoff.Value = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC FrequencyLowCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_LOW_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.FrequencyLowCutoff.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC FrequencyLowCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ABSOLUTE_CUTOFF:
				Console_WeldRecipeSC.CutoffLimit.AbsoluteDistanceCutoff.Value = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AbsoluteCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ABSOLUTE_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.AbsoluteDistanceCutoff.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC AbsoluteCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_HIGH_CUTOFF:
				Console_WeldRecipeSC.CutoffLimit.FrequencyHighCutoff.Value = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC FrequencyHighCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_HIGH_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.FrequencyHighCutoff.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC FrequencyHighCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_COLLAPSE_CUTOFF:
				Console_WeldRecipeSC.CutoffLimit.CollapseDistanceCutoff.Value = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC CollapseCutoff: " << atoi(token.c_str())<< "\n";
#endif		
				break;

			case SC_COLLAPSE_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.CollapseDistanceCutoff.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC CollapseCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
								
			case SC_ENERGY_CUTOFF:
				Console_WeldRecipeSC.CutoffLimit.EnergyCutoff.Value = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC EnergyCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_ENERGY_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.EnergyCutoff.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC EnergyCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_TIME_CUTOFF:
				Console_WeldRecipeSC.CutoffLimit.TimeCutoff.Value = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC TimeCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_TIME_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.TimeCutoff.bIsEnabled = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC TimeCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
			
			case SC_GROUND_DETECT_CUTOFF_ENABLE:
				Console_WeldRecipeSC.CutoffLimit.GroundDetectCutoff = atoi(token.c_str());
#ifdef CONSOLE_DBG					
				cout << "SC GroundDetectCutoffEnable: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case AC_READY_POS_TOGGLE:
				Console_WeldRecipeAC.ReadyPositionToggle = atoi(token.c_str());
				
#ifdef CONSOLE_DBG					
				cout << "AC ReadyPositionToggle: " << atoi(token.c_str()) << "\n";
#endif		
				break;


			}	// end of switch
		}
		else 
		{ 
			// not found anymore, break out of loop
			LOGDBG("CON_T: Could not find one of the recipe string, breaks out of while loop\n", 0, 0, 0);				
			break;													
		}
		
	}	// end of while
	
	if((tokenNum + 1) == NUM_OF_ACTIVE_RECIPE_DB_DATA)
	{
		// may need to get the last string portion
		token = activeRecipe.substr(start, activeRecipe.length());				
		status = OK;
	}
	else
	{
		LOGERR("CON_T: -------------- Failed to get all the recipe data!!! --------------\n", 0, 0, 0);
		status = ERROR;
	}
	
	return status;	

}


/**************************************************************************//**
* \brief   - Assigns default weld recipe values into console weld structures
* 			 from the CP (common part)
*
* \param   - None 
*
* \return  - None
*
******************************************************************************/
void ConsoleApp::PullActiveRecipeFromCP()
{
	SetWeldRecipe();
	
	// Copy over locally
	Console_WeldRecipeSC = Console_WeldRecipe.SCData;
	Console_WeldRecipePC = Console_WeldRecipe.PCData;
	Console_WeldRecipeAC = Console_WeldRecipe.ACData;
}


/**************************************************************************//**
* \brief   - Copies Suspect Reject active recipe to SC object
*
* \param   - WeldRecipeSC& ScWeldRecipe, string& SRActiveRecipe 
*
* \return  - UINT32 status
*
******************************************************************************/
INT32 ConsoleApp::CopySRActiveRecipe (WeldRecipeSC& ScWeldRecipe, string& SRActiveRecipe)
{
	UINT16 weldRecipeIdx;										
	vector<UINT32> SRWeldRecipe;							
	string SusRejRecipe, token;								
	stringstream SRActiveRecipeSS(SRActiveRecipe);			// Parse it from the string	
	UINT32 tokenNum = 0, recipeValue = 0;
	INT32 status = ERROR;
	
	SRWeldRecipe.clear();	

	while( getline(SRActiveRecipeSS, SusRejRecipe, ',') )
	{
		stringstream SusRejRecipeSS(SusRejRecipe);
		while( getline(SusRejRecipeSS, token, ':') )
		{
			tokenNum++;
			recipeValue = atoi(token.c_str());
			SRWeldRecipe.push_back(recipeValue);		
		}
	}

	// Write the data to the Weld Recipe structure
	for (weldRecipeIdx = 0; weldRecipeIdx < SRWeldRecipe.size(); weldRecipeIdx++)
	{
#ifdef CONSOLE_DBG	
		cout << "Case" << weldRecipeIdx << ": " << SRWeldRecipe.at(weldRecipeIdx) << "\n";		
#endif		
		switch(weldRecipeIdx)		
		{
		case SC_SR_RECIPE_NUMBER:					
			break;

		case SC_SR_RECIPE_REV_NUMBER:			
			break;

		case SC_SUSPECT_LIMIT_ENABLED:
			ScWeldRecipe.bIsSuspectLimitEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_ENABLED:
			ScWeldRecipe.bIsRejectLimitEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_SUSPECT_LIMIT_TIME_ENABLED:
			ScWeldRecipe.SuspectLimit.Time.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;
			
		case SC_SUSPECT_LIMIT_TIME_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.Time.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;
			
		case SC_SUSPECT_LIMIT_TIME_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.Time.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;
			
		case SC_SUSPECT_LIMIT_TIME_LOW:
			ScWeldRecipe.SuspectLimit.Time.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_TIME_HIGH:
			ScWeldRecipe.SuspectLimit.Time.HighLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_TIME_ENABLED:
			ScWeldRecipe.RejectLimit.Time.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;
			
		case SC_REJECT_LIMIT_TIME_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.Time.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_TIME_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.Time.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;


		case SC_REJECT_LIMIT_TIME_LOW:
			ScWeldRecipe.RejectLimit.Time.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_TIME_HIGH:
			ScWeldRecipe.RejectLimit.Time.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_ENERGY_ENABLED:
			ScWeldRecipe.SuspectLimit.Energy.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_ENERGY_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.Energy.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;
			
		case SC_SUSPECT_LIMIT_ENERGY_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.Energy.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_ENERGY_LOW:
			ScWeldRecipe.SuspectLimit.Energy.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_ENERGY_HIGH:
			ScWeldRecipe.SuspectLimit.Energy.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_ENERGY_ENABLED:
			ScWeldRecipe.RejectLimit.Energy.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_ENERGY_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.Energy.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_ENERGY_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.Energy.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_ENERGY_LOW:
			ScWeldRecipe.RejectLimit.Energy.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_ENERGY_HIGH:
			ScWeldRecipe.RejectLimit.Energy.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_PEAK_POWER_ENABLED:
			ScWeldRecipe.SuspectLimit.PeakPower.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_PEAK_POWER_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.PeakPower.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_PEAK_POWER_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.PeakPower.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_PEAK_POWER_LOW:
			ScWeldRecipe.SuspectLimit.PeakPower.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_PEAK_POWER_HIGH:
			ScWeldRecipe.SuspectLimit.PeakPower.HighLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_PEAK_POWER_ENABLED:
			ScWeldRecipe.RejectLimit.PeakPower.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_PEAK_POWER_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.PeakPower.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_PEAK_POWER_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.PeakPower.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_PEAK_POWER_LOW:
			ScWeldRecipe.RejectLimit.PeakPower.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_PEAK_POWER_HIGH:
			ScWeldRecipe.RejectLimit.PeakPower.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_ENABLED:
			ScWeldRecipe.SuspectLimit.AbsoluteDistance.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.AbsoluteDistance.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.AbsoluteDistance.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_LOW:
			ScWeldRecipe.SuspectLimit.AbsoluteDistance.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_HIGH:
			ScWeldRecipe.SuspectLimit.AbsoluteDistance.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_ENABLED:
			ScWeldRecipe.RejectLimit.AbsoluteDistance.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.AbsoluteDistance.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.AbsoluteDistance.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_LOW:
			ScWeldRecipe.RejectLimit.AbsoluteDistance.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_HIGH:
			ScWeldRecipe.RejectLimit.AbsoluteDistance.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_ENABLED:
			ScWeldRecipe.SuspectLimit.CollapseDistance.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.CollapseDistance.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.CollapseDistance.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_LOW:
			ScWeldRecipe.SuspectLimit.CollapseDistance.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_HIGH:
			ScWeldRecipe.SuspectLimit.CollapseDistance.HighLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_ENABLED:
			ScWeldRecipe.RejectLimit.CollapseDistance.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.CollapseDistance.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.CollapseDistance.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_LOW:
			ScWeldRecipe.RejectLimit.CollapseDistance.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_HIGH:
			ScWeldRecipe.RejectLimit.CollapseDistance.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_ENABLED:
			ScWeldRecipe.SuspectLimit.TriggerDistance.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.TriggerDistance.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.TriggerDistance.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_LOW:
			ScWeldRecipe.SuspectLimit.TriggerDistance.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_HIGH:
			ScWeldRecipe.SuspectLimit.TriggerDistance.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_ENABLED:
			ScWeldRecipe.RejectLimit.TriggerDistance.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.TriggerDistance.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.TriggerDistance.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_LOW:
			ScWeldRecipe.RejectLimit.TriggerDistance.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_HIGH:
			ScWeldRecipe.RejectLimit.TriggerDistance.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_WELD_FORCE_ENABLED:
			ScWeldRecipe.SuspectLimit.WeldForce.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_WELD_FORCE_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.WeldForce.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_WELD_FORCE_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.WeldForce.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_WELD_FORCE_LOW:
			ScWeldRecipe.SuspectLimit.WeldForce.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_WELD_FORCE_HIGH:
			ScWeldRecipe.SuspectLimit.WeldForce.HighLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_WELD_FORCE_ENABLED:
			ScWeldRecipe.RejectLimit.WeldForce.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_WELD_FORCE_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.WeldForce.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_WELD_FORCE_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.WeldForce.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_WELD_FORCE_LOW:
			ScWeldRecipe.RejectLimit.WeldForce.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_WELD_FORCE_HIGH:
			ScWeldRecipe.RejectLimit.WeldForce.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_FREQUENCY_ENABLED:
			ScWeldRecipe.SuspectLimit.Frequency.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_FREQUENCY_LOW_ENABLED:
			ScWeldRecipe.SuspectLimit.Frequency.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_SUSPECT_LIMIT_FREQUENCY_HIGH_ENABLED:
			ScWeldRecipe.SuspectLimit.Frequency.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);			
			break;

			
		case SC_SUSPECT_LIMIT_FREQUENCY_LOW:
			ScWeldRecipe.SuspectLimit.Frequency.LowLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_SUSPECT_LIMIT_FREQUENCY_HIGH:
			ScWeldRecipe.SuspectLimit.Frequency.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;

		case SC_REJECT_LIMIT_FREQUENCY_ENABLED:
			ScWeldRecipe.RejectLimit.Frequency.bIsEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_FREQUENCY_LOW_ENABLED:
			ScWeldRecipe.RejectLimit.Frequency.bIsLowEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_FREQUENCY_HIGH_ENABLED:
			ScWeldRecipe.RejectLimit.Frequency.bIsHighEnabled = SRWeldRecipe.at(weldRecipeIdx);	
			break;

		case SC_REJECT_LIMIT_FREQUENCY_LOW:
			ScWeldRecipe.RejectLimit.Frequency.LowLimit = SRWeldRecipe.at(weldRecipeIdx);			
			break;

		case SC_REJECT_LIMIT_FREQUENCY_HIGH:
			ScWeldRecipe.RejectLimit.Frequency.HighLimit = SRWeldRecipe.at(weldRecipeIdx);
			break;						
		}
	}		
#ifdef CONSOLE_DBG	
	cout << endl;		
#endif	

	if(SRWeldRecipe.size() == NUM_OF_SUS_REJ_RECIPE_DB_DATA)	
	{
		status = OK;
		LOGDBG("CON_T : Suspect Reject active recipe read from DB", 0, 0, 0);		
	}
	else
	{
		status = ERROR;
		LOGERR("CON_T : Failed to read the Suspect Reject active recipe from DB", 0, 0, 0);		
	}

	return status;			
}

