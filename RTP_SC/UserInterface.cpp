/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     UI Task for communicating with the console or QT UI   
 
**********************************************************************************************************/

#include "UserInterface.h"
#include "versions.h"
#include "RunTimeFeature.h"
#include "IPConfig.h"
#include "ControlTask.h"
#include <string.h>
#include <usrFsLib.h>
#include "MainTask.h"
#include "AlarmManager.h"
#include "Counters.h"
#include "FirmwareUpgrade.h"
#include "version_common.h"
#include "UIDBInterface.h"
#include "SysConfigEeprom.hpp"

extern "C"
{
	#include "customSystemCall.h"	
}

using namespace std;

/**************************************************************************//**
* 
* \brief   - Constructor - initialize the data members of UserInterface.
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
UserInterface::UserInterface() : CTRL_MSG_Q_ID(0), SDO_MSG_Q_ID(0), UI_MSG_Q_ID(0), DB_MSG_Q_ID(0),bIsPowerOnSeek(true),
								 bIsFirstReadDone(false),bIsSecondReadDone(false),m_bIsActiveRecipe(false),bIsPendingRecipe(false)
{
	CP				= CommonProperty::getInstance();
	CTRL_MSG_Q_ID	= CP->GetMsgQId(cTaskName[CTRL_T]);
	SDO_MSG_Q_ID	= CP->GetMsgQId(cTaskName[SDO_T]);
	UI_MSG_Q_ID		= CP->GetMsgQId(cTaskName[UI_T]);
	DB_MSG_Q_ID		= CP->GetMsgQId(cTaskName[DB_T]);
	UIDB_MSG_Q_ID	= CP->GetMsgQId(cTaskName[UI_DB_T]);
	Console_TID		= CP->GetTaskId(cTaskName[CONSOLE_T]);
	
	Interface 		= CommunicateInterface::getinstance();
	
}

/**************************************************************************//**
* 
* \brief   - Assigning maximum recipe(PC) values to structure.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::SetPCWeldRecipeWithMaxValue()
{
	WeldMaxValPC.AmplitudeLoopC1 	= PC_MAX_AmplitudeLoopC1;
	WeldMaxValPC.AmplitudeLoopC2 	= PC_MAX_AmplitudeLoopC2;
	WeldMaxValPC.PhaseLoop 			= PC_MAX_PhaseLoop;
	WeldMaxValPC.PhaseLoopCF 		= PC_MAX_PhaseLoopCF;

	switch(uiFrequencyInput)
	{
	case FREQ_20KHZ:
		WeldMaxValPC.FrequencyLow 	= PC_MAX_20KFrequencyLow;
		WeldMaxValPC.FrequencyHigh 	= PC_MAX_20KFrequencyHigh;
		WeldMaxValPC.PIFrequencyLow = PC_MAX_20KPIFrequencyLow;
		WeldMaxValPC.PIFrequencyHigh= PC_MAX_20KPIFrequencyHigh;
		break;
	case FREQ_30KHZ:
		WeldMaxValPC.FrequencyLow 	= PC_MAX_30KFrequencyLow;
		WeldMaxValPC.FrequencyHigh 	= PC_MAX_30KFrequencyHigh;
		WeldMaxValPC.PIFrequencyLow = PC_MAX_30KPIFrequencyLow;
		WeldMaxValPC.PIFrequencyHigh= PC_MAX_30KPIFrequencyHigh;
		break;
	case FREQ_40KHZ:
		WeldMaxValPC.FrequencyLow 	= PC_MAX_40KFrequencyLow;
		WeldMaxValPC.FrequencyHigh 	= PC_MAX_40KFrequencyHigh;
		WeldMaxValPC.PIFrequencyLow = PC_MAX_40KPIFrequencyLow;
		WeldMaxValPC.PIFrequencyHigh= PC_MAX_40KPIFrequencyHigh;
	    break;
    }

	WeldMaxValPC.PhaseLimitTime 	= PC_MAX_PhaseLimitTime;
	WeldMaxValPC.PhaseLimit 		= PC_MAX_PhaseLimit;
	WeldMaxValPC.ControlMode 		= PC_MAX_ControlMode;
	WeldMaxValPC.BlindtimeSeek 		= PC_MAX_BlindtimeSeek;
	WeldMaxValPC.Blindtimeweld		= PC_MAX_Blindtimeweld;
	WeldMaxValPC.DDSSwitchtime		= PC_MAX_DDSSwitchtime;
	WeldMaxValPC.Parameter7 		= PC_MAX_Parameter7;
	WeldMaxValPC.Parameter8 		= PC_MAX_Parameter8;
	WeldMaxValPC.Parameter9 		= PC_MAX_Parameter9;
	WeldMaxValPC.Parameter10		= PC_MAX_Parameter10;
	WeldMaxValPC.Parameter11 		= PC_MAX_Parameter11;
	WeldMaxValPC.Parameter12 		= PC_MAX_Parameter12;
	WeldMaxValPC.Parameter13 		= PC_MAX_Parameter13;
	WeldMaxValPC.Parameter14		= PC_MAX_Parameter14;
	WeldMaxValPC.Parameter15 		= PC_MAX_Parameter15;
	WeldMaxValPC.FLimitTime 		= PC_MAX_FLimitTime;
	WeldMaxValPC.AmpProportionalGain= PC_MAX_AmpProportionalGain;
	WeldMaxValPC.AmpIntegralGain 	= PC_MAX_AmpIntegralGain;
	WeldMaxValPC.FrequencyWindowSize= PC_MAX_FrequencyWindowSize;
	WeldMaxValPC.PIPhaseLimitTime 	= PC_MAX_PIPhaseLimitTime;
	WeldMaxValPC.PIPhaseLimit 		= PC_MAX_PIPhaseLimit;
	WeldMaxValPC.PFofPhasecontrolloop= PC_MAX_PFofPhasecontrolloop;
	WeldMaxValPC.PhaseProportionalGain= PC_MAX_PhaseProportionalGain;
	WeldMaxValPC.WeldRampTime	    = PC_MAX_WeldRampTime;
}

/**************************************************************************//**
* 
* \brief   - Assigning minimum recipe(PC) values to structure.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::SetPCWeldRecipeWithMinValue()
{	         
	WeldMinValPC.AmplitudeLoopC1 	= PC_MIN_AmplitudeLoopC1;
	WeldMinValPC.AmplitudeLoopC2 	= PC_MIN_AmplitudeLoopC2;
	WeldMinValPC.PhaseLoop 			= PC_MIN_PhaseLoop;
	WeldMinValPC.PhaseLoopCF 		= PC_MIN_PhaseLoopCF;
	
	switch(uiFrequencyInput)
	{
	case FREQ_20KHZ:
		WeldMinValPC.FrequencyLow 	= PC_MIN_20KFrequencyLow;
		WeldMinValPC.FrequencyHigh 	= PC_MIN_20KFrequencyHigh;
		break;
	case FREQ_30KHZ:
		WeldMinValPC.FrequencyLow 	= PC_MIN_30KFrequencyLow;
		WeldMinValPC.FrequencyHigh 	= PC_MIN_30KFrequencyHigh;
		break;
	case FREQ_40KHZ:
		WeldMinValPC.FrequencyLow 	= PC_MIN_40KFrequencyLow;
		WeldMinValPC.FrequencyHigh 	= PC_MIN_40KFrequencyHigh;
		break;
	}

	WeldMinValPC.PhaseLimitTime 	= PC_MIN_PhaseLimitTime;
	WeldMinValPC.PhaseLimit 		= PC_MIN_PhaseLimit;
	WeldMinValPC.ControlMode 		= PC_MIN_ControlMode;
	WeldMinValPC.BlindtimeSeek 		= PC_MIN_BlindtimeSeek;
	WeldMinValPC.Blindtimeweld 		= PC_MIN_Blindtimeweld;
	WeldMinValPC.DDSSwitchtime 		= PC_MIN_DDSSwitchtime;
	WeldMinValPC.Parameter7 		= PC_MIN_Parameter7;
	WeldMinValPC.Parameter8 		= PC_MIN_Parameter8;
	WeldMinValPC.Parameter9 		= PC_MIN_Parameter9;
	WeldMinValPC.Parameter10 		= PC_MIN_Parameter10;
	WeldMinValPC.Parameter11 		= PC_MIN_Parameter11;
	WeldMinValPC.Parameter12 		= PC_MIN_Parameter12;
	WeldMinValPC.Parameter13 		= PC_MIN_Parameter13;
	WeldMinValPC.Parameter14 		= PC_MIN_Parameter14;
	WeldMinValPC.Parameter15		= PC_MIN_Parameter15;
	WeldMinValPC.FLimitTime 		= PC_MIN_FLimitTime;
	WeldMinValPC.AmpProportionalGain= PC_MIN_AmpProportionalGain;
	WeldMinValPC.AmpIntegralGain 	= PC_MIN_AmpIntegralGain;
	WeldMinValPC.FrequencyWindowSize= PC_MIN_FrequencyWindowSize;
	WeldMinValPC.PIFrequencyLow 	= PC_MIN_PIFrequencyLow;
	WeldMinValPC.PIFrequencyHigh 	= PC_MIN_PIFrequencyHigh;
	WeldMinValPC.PIPhaseLimitTime 	= PC_MIN_PIPhaseLimitTime;
	WeldMinValPC.PIPhaseLimit 		= PC_MIN_PIPhaseLimit;
	WeldMinValPC.PFofPhasecontrolloop= PC_MIN_PFofPhasecontrolloop;
	WeldMinValPC.PhaseProportionalGain= PC_MIN_PhaseProportionalGain;
	WeldMinValPC.WeldRampTime       = PC_MIN_WeldRampTime;
}

/**************************************************************************//**
* 
* \brief   - Assigning maximum recipe(AC) values to structure.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
 void UserInterface::SetACWeldRecipeWithMaxValue()
 {
	WeldMaxValAC.WeldForce 			= AC_MAX_WeldForce;
	WeldMaxValAC.ForceRampTime 		= AC_MAX_ForceRampTime;
	WeldMaxValAC.HoldMode 			= AC_MAX_HoldMode;
	WeldMaxValAC.TotalCollapseTarget= AC_MAX_TotalCollapseTarget;
	WeldMaxValAC.HoldForce 			= AC_MAX_HoldForce;
	WeldMaxValAC.HoldForceRampTime 	= AC_MAX_HoldForceRampTime;
	WeldMaxValAC.ReadyPosition 		= AC_MAX_ReadyPosition;
	WeldMaxValAC.DownAcceleration 	= AC_MAX_DownAcceleration;
	WeldMaxValAC.DownMaxVelocity 	= AC_MAX_DownMaxVelocity;
	WeldMaxValAC.DownDeceleration 	= AC_MAX_DownDeceleration;
	WeldMaxValAC.ReturnAcceleration = AC_MAX_ReturnAcceleration;
	WeldMaxValAC.ReturnMaxVelocity 	= AC_MAX_ReturnMaxVelocity;
	WeldMaxValAC.ReturnDeceleration = AC_MAX_ReturnDeceleration;
	WeldMinValAC.PartContactWindowMinus = AC_MAX_PartContactWindowMinus;
	WeldMinValAC.PartContactWindowPlus  = AC_MAX_PartContactWindowPlus;
	WeldMinValAC.ExpectedPartContactOffset = AC_MAX_ExcepectedPartContactOffset;
	WeldMaxValAC.NumForceSteps 		= AC_MAX_NumForceSteps;
	WeldMaxValAC.ForceStepForce[0] 	= AC_MAX_ForceStepForce;
	WeldMaxValAC.ForceStepRampTime[0] = AC_MAX_ForceStepRampTime;
	WeldMaxValAC.ExpectedPartContactPosition = AC_MAX_ExpectedPartContactPosition;
}

 /**************************************************************************//**
 * 
 * \brief   - Assigning minimum recipe(AC) values to structure.
 *
 * \param   - None.
 *
 * \return  - None.
 *
 ******************************************************************************/
void UserInterface::SetACWeldRecipeWithMinValue()
{
	WeldMinValAC.WeldForce 			= AC_MIN_WeldForce;
	WeldMinValAC.ForceRampTime 		= AC_MIN_ForceRampTime;
	WeldMinValAC.HoldMode 			= AC_MIN_HoldMode;
	WeldMinValAC.TotalCollapseTarget= AC_MIN_TotalCollapseTarget;
	WeldMinValAC.HoldForce 			= AC_MIN_HoldForce ;
	WeldMinValAC.HoldForceRampTime 	= AC_MIN_HoldForceRampTime;
	WeldMinValAC.ReadyPosition  	= AC_MIN_ReadyPosition;
	WeldMinValAC.DownAcceleration 	= AC_MIN_DownAcceleration;
	WeldMinValAC.DownMaxVelocity 	= AC_MIN_DownMaxVelocity;
	WeldMinValAC.DownDeceleration 	= AC_MIN_DownDeceleration;
	WeldMinValAC.ReturnAcceleration = AC_MIN_ReturnAcceleration;
	WeldMinValAC.ReturnMaxVelocity 	= AC_MIN_ReturnMaxVelocity;
	WeldMinValAC.ReturnDeceleration = AC_MIN_ReturnDeceleration;
	WeldMinValAC.PartContactWindowMinus = AC_MIN_PartContactWindowMinus;
	WeldMinValAC.PartContactWindowPlus  = AC_MIN_PartContactWindowPlus;
	WeldMinValAC.ExpectedPartContactOffset = AC_MIN_ExcepectedPartContactOffset;
	WeldMinValAC.NumForceSteps 		= AC_MIN_NumForceSteps;
	WeldMinValAC.ForceStepForce[0] 	= AC_MIN_ForceStepForce;
	WeldMinValAC.ForceStepRampTime[0] = AC_MIN_ForceStepRampTime;
	WeldMinValAC.ExpectedPartContactPosition = AC_MIN_ExpectedPartContactPosition;
}

/**************************************************************************//**
* 
* \brief   - Validation of PC weld recipe parameters.
*
* \param   - WeldRecipePC&.
*
* \return  - bool.
*
******************************************************************************/
bool UserInterface::ValidateRecipe(WeldRecipePC& Recipe)
{
	bool bIsValidation = false;

	if(Recipe.AmplitudeLoopC1 < WeldMinValPC.AmplitudeLoopC1)
	{
		printf("UI_T : PC_MIN validation failed - AmplitudeLoopC1 : %d\n",Recipe.AmplitudeLoopC1);
	}
	else if(Recipe.AmplitudeLoopC2 < WeldMinValPC.AmplitudeLoopC2)
	{
		printf("UI_T : PC_MIN validation failed - AmplitudeLoopC2 : %d\n",Recipe.AmplitudeLoopC2);
	}
	else if(Recipe.PhaseLoop < WeldMinValPC.PhaseLoop)
	{
		printf("UI_T : PC_MIN validation failed - PhaseLoop : %d\n",Recipe.PhaseLoop);
	}
	else if(Recipe.PhaseLoopCF < WeldMinValPC.PhaseLoopCF)
	{
		printf("UI_T : PC_MIN validation failed - PhaseLoopCF : %d\n",Recipe.PhaseLoopCF);
	}
	else if(Recipe.FrequencyLow < WeldMinValPC.FrequencyLow)
	{
		printf("UI_T : PC_MIN validation failed - FrequencyLow : %d\n",Recipe.FrequencyLow);
	}
	else if(Recipe.FrequencyHigh < WeldMinValPC.FrequencyHigh)
	{
		printf("UI_T : PC_MIN validation failed - FrequencyHigh : %d\n",Recipe.FrequencyHigh);
	}
	else if(Recipe.PhaseLimitTime < WeldMinValPC.PhaseLimitTime)
	{
		printf("UI_T : PC_MIN validation failed - PhaseLimitTime : %d\n",Recipe.PhaseLimitTime);
	}
	else if(Recipe.PhaseLimit < WeldMinValPC.PhaseLimit)
	{
		printf("UI_T : PC_MIN validation failed - PhaseLimit : %d\n",Recipe.PhaseLimit);
	}
	else if(Recipe.ControlMode < WeldMinValPC.ControlMode)
	{
		printf("UI_T : PC_MIN validation failed - ControlMode : %d\n",Recipe.ControlMode);
	}
	else if(Recipe.BlindtimeSeek < WeldMinValPC.BlindtimeSeek)
	{
		printf("UI_T : PC_MIN validation failed - BlindtimeSeek : %d\n",Recipe.BlindtimeSeek);
	}
	else if(Recipe.Blindtimeweld < WeldMinValPC.Blindtimeweld)
	{
		printf("UI_T : PC_MIN validation failed - Blindtimeweld : %d\n",Recipe.Blindtimeweld);
	}
	else if(Recipe.DDSSwitchtime < WeldMinValPC.DDSSwitchtime)
	{
		printf("UI_T : PC_MIN validation failed - DDSSwitchtime : %d\n",Recipe.DDSSwitchtime);
	}
	else if(Recipe.WeldRampTime < WeldMinValPC.WeldRampTime)
	{
		printf("UI_T : PC_MIN validation failed - WeldRampTime : %d\n",Recipe.WeldRampTime);
	}
	else if(Recipe.Parameter7 < WeldMinValPC.Parameter7)
	{
		printf("UI_T : PC_MIN validation failed - Parameter7 : %d\n",Recipe.Parameter7);
	}
	else if(Recipe.Parameter8 < WeldMinValPC.Parameter8)
	{
		printf("UI_T : PC_MIN validation failed - Parameter8 : %d\n",Recipe.Parameter8);
	}
	else if(Recipe.Parameter9 < WeldMinValPC.Parameter9)
	{
		printf("UI_T : PC_MIN validation failed - Parameter9 : %d\n",Recipe.Parameter9);
	}
	else if(Recipe.Parameter10 < WeldMinValPC.Parameter10)
	{
		printf("UI_T : PC_MIN validation failed - Parameter10 : %d\n",Recipe.Parameter10);
	}
	else if(Recipe.Parameter11 < WeldMinValPC.Parameter11)
	{
		printf("UI_T : PC_MIN validation failed - Parameter11 : %d\n",Recipe.Parameter11);
	}
	else if(Recipe.Parameter12 < WeldMinValPC.Parameter12)
	{
		printf("UI_T : PC_MIN validation failed - Parameter12 : %d\n",Recipe.Parameter12);
	}
	else if(Recipe.Parameter13 < WeldMinValPC.Parameter13)
	{
		printf("UI_T : PC_MIN validation failed - Parameter13 : %d\n",Recipe.Parameter13);
	}
	else if(Recipe.Parameter14 < WeldMinValPC.Parameter14)
	{
		printf("UI_T : PC_MIN validation failed - Parameter14 : %d\n",Recipe.Parameter14);
	}
	else if(Recipe.Parameter15 < WeldMinValPC.Parameter15)
	{
		printf("UI_T : PC_MIN validation failed - Parameter15 : %d\n",Recipe.Parameter15);
	}
	else if(Recipe.FLimitTime < WeldMinValPC.FLimitTime)
	{
		printf("UI_T : PC_MIN validation failed - FLimitTime : %d\n",Recipe.FLimitTime);
	}
	else if(Recipe.AmpProportionalGain < WeldMinValPC.AmpProportionalGain)
	{
		printf("UI_T : PC_MIN validation failed - AmpProportionalGain : %d\n",Recipe.AmpProportionalGain);
	}
	else if(Recipe.AmpIntegralGain < WeldMinValPC.AmpIntegralGain)
	{
		printf("UI_T : PC_MIN validation failed - AmpIntegralGain : %d\n",Recipe.AmpIntegralGain);
	}
	else if(Recipe.PhaseProportionalGain < WeldMinValPC.PhaseProportionalGain)
	{
		printf("UI_T : PC_MIN validation failed - PhaseProportionalGain : %d\n",Recipe.PhaseProportionalGain);
	}
	else if(Recipe.FrequencyWindowSize < WeldMinValPC.FrequencyWindowSize)
	{
		printf("UI_T : PC_MIN validation failed - FrequencyWindowSize : %d\n",Recipe.FrequencyWindowSize);
	}
	else if(Recipe.PFofPhasecontrolloop < WeldMinValPC.PFofPhasecontrolloop)
	{
		printf("UI_T : PC_MIN validation failed - PFofPhasecontrolloop : %d\n",Recipe.PFofPhasecontrolloop);
	}
	else if(Recipe.PIPhaseLimitTime < WeldMinValPC.PIPhaseLimitTime)
	{
		printf("UI_T : PC_MIN validation failed - PIPhaseLimitTime : %d\n",Recipe.PIPhaseLimit);
	}
	else if(Recipe.PIPhaseLimit < WeldMinValPC.PIPhaseLimit)
	{
		printf("UI_T : PC_MIN validation failed - PIPhaseLimit : %d\n",Recipe.PIPhaseLimit);
	}
	else
	{
		//printf("UI_T : PC_MIN validation success\n",0);
	}

	/* RECIPE PC_MAX validation starts */
	if(Recipe.AmplitudeLoopC1 > WeldMaxValPC.AmplitudeLoopC1)
	{
		printf("UI_T : PC_MAX validation failed - AmplitudeLoopC1 : %d\n",Recipe.AmplitudeLoopC1);
	}
	else if(Recipe.AmplitudeLoopC2 > WeldMaxValPC.AmplitudeLoopC2)
	{
		printf("UI_T : PC_MAX validation failed - AmplitudeLoopC2 : %d\n",Recipe.AmplitudeLoopC2);
	}
	else if(Recipe.PhaseLoop > WeldMaxValPC.PhaseLoop)
	{
		printf("UI_T : PC_MAX validation failed - PhaseLoop : %d\n",Recipe.PhaseLoop);
	}
	else if(Recipe.PhaseLoopCF > WeldMaxValPC.PhaseLoopCF)
	{
		printf("UI_T : PC_MAX validation failed - PhaseLoopCF : %d\n",Recipe.PhaseLoopCF);
	}
	else if(Recipe.FrequencyLow > WeldMaxValPC.FrequencyLow)
	{
		printf("UI_T : PC_MAX validation failed - FrequencyLow : %d\n",Recipe.FrequencyLow);
	}
	else if(Recipe.FrequencyHigh > WeldMaxValPC.FrequencyHigh)
	{
		printf("UI_T : PC_MAX validation failed - FrequencyHigh : %d\n",Recipe.FrequencyHigh);
	}
	else if(Recipe.PhaseLimitTime > WeldMaxValPC.PhaseLimitTime)
	{
		printf("UI_T : PC_MAX validation failed - PhaseLimitTime : %d\n",Recipe.PhaseLimitTime);
	}
	else if(Recipe.PhaseLimit > WeldMaxValPC.PhaseLimit)
	{
		printf("UI_T : PC_MAX validation failed - PhaseLimit : %d\n",Recipe.PhaseLimit);
	}
	else if(Recipe.ControlMode > WeldMaxValPC.ControlMode)
	{
		printf("UI_T : PC_MAX validation failed - ControlMode : %d\n",Recipe.ControlMode);
	}
	else if(Recipe.BlindtimeSeek > WeldMaxValPC.BlindtimeSeek)
	{
		printf("UI_T : PC_MAX validation failed - BlindtimeSeek : %d\n",Recipe.BlindtimeSeek);
	}
	else if(Recipe.Blindtimeweld > WeldMaxValPC.Blindtimeweld)
	{
		printf("UI_T : PC_MAX validation failed - Blindtimeweld : %d\n",Recipe.Blindtimeweld);
	}
	else if(Recipe.DDSSwitchtime > WeldMaxValPC.DDSSwitchtime)
	{
		printf("UI_T : PC_MAX validation failed - DDSSwitchtime : %d\n",Recipe.DDSSwitchtime);
	}
	else if(Recipe.WeldRampTime > WeldMaxValPC.WeldRampTime)
	{
		printf("UI_T : PC_MAX validation failed - WeldRampTime : %d\n",Recipe.WeldRampTime);
	}
	else if(Recipe.Parameter7 > WeldMaxValPC.Parameter7)
	{
		printf("UI_T : PC_MAX validation failed - Parameter7 : %d\n",Recipe.Parameter7);
	}
	else if(Recipe.Parameter8 > WeldMaxValPC.Parameter8)
	{
		printf("UI_T : PC_MAX validation failed - Parameter8 : %d\n",Recipe.Parameter8);
	}
	else if(Recipe.Parameter9 > WeldMaxValPC.Parameter9)
	{
		printf("UI_T : PC_MAX validation failed - Parameter9 : %d\n",Recipe.Parameter9);
	}
	else if(Recipe.Parameter10 > WeldMaxValPC.Parameter10)
	{
		printf("UI_T : PC_MAX validation failed - Parameter10 : %d\n",Recipe.Parameter10);
	}
	else if(Recipe.Parameter11 > WeldMaxValPC.Parameter11)
	{
		printf("UI_T : PC_MAX validation failed - Parameter11 : %d\n",Recipe.Parameter11);
	}
	else if(Recipe.Parameter12 > WeldMaxValPC.Parameter12)
	{
		printf("UI_T : PC_MAX validation failed - Parameter12 : %d\n",Recipe.Parameter12);
	}
	else if(Recipe.Parameter13 > WeldMaxValPC.Parameter13)
	{
		printf("UI_T : PC_MAX validation failed - Parameter13 : %d\n",Recipe.Parameter13);
	}
	else if(Recipe.Parameter14 > WeldMaxValPC.Parameter14)
	{
		printf("UI_T : PC_MAX validation failed - Parameter14 : %d\n",Recipe.Parameter14);
	}
	else if(Recipe.Parameter15 > WeldMaxValPC.Parameter15)
	{
		printf("UI_T : PC_MAX validation failed - Parameter15 : %d\n",Recipe.Parameter15);
	}
	else if(Recipe.FLimitTime > WeldMaxValPC.FLimitTime)
	{
		printf("UI_T : PC_MAX validation failed - FLimitTime : %d\n",Recipe.FLimitTime);
	}
	else if(Recipe.AmpProportionalGain > WeldMaxValPC.AmpProportionalGain)
	{
		printf("UI_T : PC_MAX validation failed - AmpProportionalGain : %d\n",Recipe.AmpProportionalGain);
	}
	else if(Recipe.AmpIntegralGain > WeldMaxValPC.AmpIntegralGain)
	{
		printf("UI_T : PC_MAX validation failed - AmpIntegralGain : %d\n",Recipe.AmpIntegralGain);
	}
	else if(Recipe.PhaseProportionalGain > WeldMaxValPC.PhaseProportionalGain)
	{
		printf("UI_T : PC_MAX validation failed - PhaseProportionalGain : %d\n",Recipe.PhaseProportionalGain);
	}
	else if(Recipe.FrequencyWindowSize > WeldMaxValPC.FrequencyWindowSize)
	{
		printf("UI_T : PC_MAX validation failed - FrequencyWindowSize : %d\n",Recipe.FrequencyWindowSize);
	}
	else if(Recipe.PFofPhasecontrolloop > WeldMaxValPC.PFofPhasecontrolloop)
	{
		printf("UI_T : PC_MAX validation failed - PFofPhasecontrolloop : %d\n",Recipe.PFofPhasecontrolloop);
	}
	else if(Recipe.PIPhaseLimitTime > WeldMaxValPC.PIPhaseLimitTime)
	{
		printf("UI_T : PC_MAX validation failed - PIPhaseLimitTime : %d\n",Recipe.PIPhaseLimit);
	}
	else if(Recipe.PIPhaseLimit > WeldMaxValPC.PIPhaseLimit)
	{
		printf("UI_T : PC_MAX validation failed - PIPhaseLimit : %d\n",Recipe.PIPhaseLimit);
	}
	else
	{
		//printf("UI_T : PC_MAX validation success\n");
		bIsValidation = true ;
	}

	return bIsValidation;
}

/**************************************************************************//**
* 
* \brief   - Validation of AC weld recipe parameters.
*
* \param   -  WeldRecipeAC&
*
* \return  - bool.
*
******************************************************************************/
bool UserInterface::ValidateRecipe(WeldRecipeAC& Recipe)
{	
	bool bIsValidation = false;

	if(Recipe.WeldForce > WeldMaxValAC.WeldForce)
	{
		printf("UI_T : AC_MAX validation failed - WeldForce : %d\n",Recipe.WeldForce);
	}
	else if(Recipe.ForceRampTime > WeldMaxValAC.ForceRampTime)
	{
		printf("UI_T : AC_MAX validation failed - ForceRampTime : %d\n",Recipe.ForceRampTime);
	}
	else if(Recipe.TotalCollapseTarget > WeldMaxValAC.TotalCollapseTarget)
	{
		printf("UI_T : AC_MAX validation failed - TotalCollapseTarget : %d\n",Recipe.TotalCollapseTarget);
	}
	else if(Recipe.HoldForce > WeldMaxValAC.HoldForce)
	{
		printf("UI_T : AC_MAX validation failed - HoldForce : %d\n",Recipe.HoldForce);
	}
	else if(Recipe.HoldForceRampTime > WeldMaxValAC.HoldForceRampTime)
	{
		printf("UI_T : AC_MAX validation failed - HoldForceRampTime : %d\n",Recipe.HoldForceRampTime);
	}
	else if(Recipe.ExpectedPartContactPosition > WeldMaxValAC.ExpectedPartContactPosition)
	{
		printf("UI_T : AC_MAX validation failed - ExpectedPartContactPosition : %d\n",Recipe.ExpectedPartContactPosition);
	}
	else if(Recipe.ReadyPosition > WeldMaxValAC.ReadyPosition)
	{
		printf("UI_T : AC_MAX validation failed - ReadyPosition : %d\n",Recipe.ReadyPosition);
	}
	else if(Recipe.DownAcceleration > WeldMaxValAC.DownAcceleration)
	{
		printf("UI_T : AC_MAX validation failed - DownAcceleration : %d\n",Recipe.DownAcceleration);
	}
	else if(Recipe.DownMaxVelocity > WeldMaxValAC.DownMaxVelocity)
	{
		printf("UI_T : AC_MAX validation failed - DownMaxVelocity : %d\n",Recipe.DownMaxVelocity);
	}
	else if(Recipe.DownDeceleration > WeldMaxValAC.DownDeceleration)
	{
		printf("UI_T : AC_MAX validation failed - DownDeceleration : %d\n",Recipe.DownDeceleration);
	}
	else if(Recipe.ReturnAcceleration > WeldMaxValAC.ReturnAcceleration)
	{
		printf("UI_T : AC_MAX validation failed - ReturnAcceleration : %d\n",Recipe.ReturnAcceleration);
	}
	else if(Recipe.ReturnMaxVelocity > WeldMaxValAC.ReturnMaxVelocity)
	{
		printf("UI_T : AC_MAX validation failed - ReturnMaxVelocity : %d\n",Recipe.ReturnMaxVelocity);
	}
	else if(Recipe.ReturnDeceleration > WeldMaxValAC.ReturnDeceleration)
	{
		printf("UI_T : AC_MAX validation failed - ReturnDeceleration : %d\n",Recipe.ReturnDeceleration);
	}
	else if(Recipe.NumForceSteps > WeldMaxValAC.NumForceSteps)
	{
		printf("UI_T : AC_MAX validation failed - NumForceSteps : %d\n",Recipe.NumForceSteps);
	}
	else if(Recipe.ForceStepForce[0] > WeldMaxValAC.ForceStepForce[0])
	{
		printf("UI_T : AC_MAX validation failed - ForceStepForce : %d\n",Recipe.ForceStepForce[0]);
	}
	else if(Recipe.ForceStepRampTime[0] > WeldMaxValAC.ForceStepRampTime[0])
	{
		printf("UI_T : AC_MAX validation failed - ForceStepRampTime : %d\n",Recipe.ForceStepRampTime[0]);
	}
	else
	{
		//printf("UI_T : AC_MAX validation success\n");
	}

	/* Recipe AC_MIN validation starts */
	if(Recipe.WeldForce > WeldMaxValAC.WeldForce)
	{
		printf("UI_T : AC_MAX validation failed - WeldForce : %d\n",Recipe.WeldForce);
	}
	else if(Recipe.ForceRampTime < WeldMinValAC.ForceRampTime)
	{
		printf("UI_T : AC_MIN validation failed - ForceRampTime : %d\n",Recipe.ForceRampTime);
	}
	else if(Recipe.TotalCollapseTarget < WeldMinValAC.TotalCollapseTarget)
	{
		printf("UI_T : AC_MIN validation failed - TotalCollapseTarget : %d\n",Recipe.TotalCollapseTarget);
	}
	else if(Recipe.HoldForce < WeldMinValAC.HoldForce)
	{
		printf("UI_T : AC_MIN validation failed - HoldForce : %d\n",Recipe.HoldForce);
	}
	else if(Recipe.HoldForceRampTime < WeldMinValAC.HoldForceRampTime)
	{
		printf("UI_T : AC_MIN validation failed - HoldForceRampTime : %d\n",Recipe.HoldForceRampTime);
	}
	else if(Recipe.ExpectedPartContactPosition < WeldMinValAC.ExpectedPartContactPosition)
	{
		printf("UI_T : AC_MIN validation failed - ExpectedPartContactPosition : %d\n",Recipe.ExpectedPartContactPosition);
	}
	else if(Recipe.ReadyPosition < WeldMinValAC.ReadyPosition)
	{
		printf("UI_T : AC_MIN validation failed - ReadyPosition : %d\n",Recipe.ReadyPosition);
	}
	else if(Recipe.DownAcceleration < WeldMinValAC.DownAcceleration)
	{
		printf("UI_T : AC_MIN validation failed - DownAcceleration : %d\n",Recipe.DownAcceleration);
	}
	else if(Recipe.DownMaxVelocity < WeldMinValAC.DownMaxVelocity)
	{
		printf("UI_T : AC_MIN validation failed - DownMaxVelocity : %d\n",Recipe.DownMaxVelocity);
	}
	else if(Recipe.DownDeceleration < WeldMinValAC.DownDeceleration)
	{
		printf("UI_T : AC_MIN validation failed - DownDeceleration : %d\n",Recipe.DownDeceleration);
	}
	else if(Recipe.ReturnAcceleration < WeldMinValAC.ReturnAcceleration)
	{
		printf("UI_T : AC_MIN validation failed - ReturnAcceleration : %d\n",Recipe.ReturnAcceleration);
	}
	else if(Recipe.ReturnMaxVelocity < WeldMinValAC.ReturnMaxVelocity)
	{
		printf("UI_T : AC_MIN validation failed - ReturnMaxVelocity : %d\n",Recipe.ReturnMaxVelocity);
	}
	else if(Recipe.ReturnDeceleration < WeldMinValAC.ReturnDeceleration)
	{
		printf("UI_T : AC_MIN validation failed - ReturnDeceleration : %d\n",Recipe.ReturnDeceleration);
	}
	else if(Recipe.NumForceSteps < WeldMinValAC.NumForceSteps)
	{
		printf("UI_T : AC_MIN validation failed - NumForceSteps : %d\n",Recipe.NumForceSteps);
	}
	else if(Recipe.ForceStepForce[0] < WeldMinValAC.ForceStepForce[0])
	{
		printf("UI_T : AC_MIN validation failed - ForceStepForce : %d\n",Recipe.ForceStepForce[0]);
	}
	else if(Recipe.ForceStepRampTime[0] < WeldMinValAC.ForceStepRampTime[0])
	{
		printf("UI_T : AC_MIN validation failed - ForceStepRampTime : %d\n",Recipe.ForceStepRampTime[0]);
	}
	else
	{
		//printf("UI_T : AC_MIN validation success\n");
		bIsValidation = true;
	}

	return bIsValidation;
}

/*************************************************************************//**
* \brief   -  Sends CMD(weld, seek, Setup and Scan) to Control task.
*
* \param   - Message& message.
*
* \return  - None.
*
******************************************************************************/
 void UserInterface::SendMessageToControlTask(Message& message)
{	
	 bool bIsIdValid = true;

	 switch(message.msgID)
	 {
		 case TO_UI_TASK_WELD_CMD:
			 message.msgID = TO_CTRL_TASK_WELD_CMD;
			 break;
	
		 case TO_UI_TASK_SEEK_CMD:
			 message.msgID = TO_CTRL_TASK_SEEK_CMD;
			 break;
	
		 case TO_UI_TASK_SETUP_CMD:
			 message.msgID = TO_CTRL_TASK_SETUP_CMD;
			 break;

		 case TO_UI_TASK_SET_NEXT_OPERATION_REQ:
			 message.msgID = TO_CTRL_TASK_SET_NEXT_OPERATION_REQ;
			 break;

		 case TO_UI_TASK_HORN_SCAN_CMD:
			 message.msgID = TO_CTRL_TASK_HORN_SCAN_CMD;
			 break;
	
		 case TO_UI_TASK_HORN_SCAN_ABORT_CMD:
			 message.msgID = TO_CTRL_TASK_HORN_SCAN_ABORT_CMD;
			 break;
	
		 case TO_UI_TASK_RESET_ALARM_CMD:
			 message.msgID = TO_CTRL_TASK_RESET_ALARM_CMD;
			 break;
	
		 case TO_UI_TASK_ALARM_RESET_ALL_REQ:
			 message.msgID = TO_CTRL_TASK_ALARM_RESET_ALL_REQ;
			 break;
	
		 case TO_UI_TASK_SC_STATE_REQ:
			 message.msgID = TO_CTRL_TASK_SC_STATE_REQ;
			 break;
	
		 case TO_UI_TASK_WELD_RECIPE_CONFIGURED:
			 message.msgID = TO_CTRL_TASK_WELD_RECIPE_CONFIGURED;
			 break;
	
		 case TO_UI_TASK_SEEK_RECIPE_CONFIGURED:
			 message.msgID = TO_CTRL_TASK_SEEK_RECIPE_CONFIGURED;
			 break;
	
		 case TO_UI_TASK_TEST_CMD_REQ:
			 message.msgID = TO_CTRL_TASK_TEST_CMD_REQ;
			 break;
	
		 case TO_UI_TASK_TEST_ABORT_CMD_REQ:
			 message.msgID = TO_CTRL_TASK_TEST_ABORT_CMD_REQ;
			 break;
	
		 case TO_UI_TASK_CALIBRATION_CMD_REQ:
			 message.msgID = TO_CTRL_TASK_CALIBRATION_CMD_REQ;
			 break;
	
		 case TO_UI_TASK_BATCH_COUNT_RESET_REQ:
			 message.msgID = TO_CTRL_TASK_BATCH_COUNT_RESET_CMD_REQ;
			 break;
	
		 case TO_UI_TASK_BATCH_COUNT_EXCEED_CMD:
			 message.msgID = TO_CTRL_TASK_BATCH_COUNT_EXCEED_CMD;
			 break;
			 
		 case TO_UI_TASK_EMMC_WEAR_LEVEL_REQ:
			 LOGDBG("\nUI_T : TO_UI_TASK_EMMC_WEAR_LEVEL_REQ\n",0,0,0);
			 message.msgID = TO_CTRL_TASK_EMMC_WEAR_LEVEL_REQ;
			 break;

		 case TO_UI_TASK_OPTIMIZE_DATABASE_REQ:
			 message.msgID = TO_CTRL_TASK_OPTIMIZE_DATABASE_REQ;
			 break;
		 default:
			 bIsIdValid = false;
			 LOGDBG("UI_T : trying to send Invalid Msg id to Control Task : %d",message.msgID,0,0); 
			 break;
	 }

	 if(bIsIdValid)
	 {
		 if(msgQSend(CTRL_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		 {
			 LOGERR("UI_T : Message send failed to Ctrl_Task",0,0,0);
		 }
	 }
 }
 
 /**************************************************************************//**
 * 
 * \brief   - Prepare message and sends to SDO_Task message queue.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
 void UserInterface::SendMessageToSDOTask(Message& message)
 {
	 bool bIsIdValid = true;
	 
	 switch(message.msgID)
	 {
		 case TO_UI_TASK_WELD_RECIPE:
			 message.msgID = TO_SDO_TASK_WELD_RECIPE;
			 break;

		 case TO_UI_TASK_SEEK_RECIPE:
			 message.msgID = TO_SDO_TASK_SEEK_RECIPE;
			 break;
			
		case TO_UI_TASK_VERSION_REQUEST:
			message.msgID = TO_SDO_TASK_VERSION_REQUEST;
			break;
			
		case TO_CALIBRATION_DATA_REQUEST:
			message.msgID = TO_SDO_CALIBRATION_DATA_REQUEST;
			break;
			
		 case TO_UI_TASK_POWER_UP_RECIPE:
			 message.msgID = TO_SDO_TASK_POWER_UP_RECIPE;
			 break;
			
		case TO_UI_TASK_HORN_SCAN_RECIPE:
			message.msgID = TO_SDO_TASK_HORN_SCAN_RECIPE;
			break;

		case TO_SDO_TASK_UL_WELD_REQUEST:
			message.msgID = TO_SDO_TASK_UL_WELD_REQUEST;
			break;
			
		case TO_UI_TASK_TMR_INTVL_DATA_PC:   
			message.msgID = TO_SDO_TASK_TMR_INTVL_DATA_PC;
			break;
			
		case TO_UI_TASK_TEST_RECIPE_REQ:
			message.msgID = TO_SDO_TASK_TEST_RECIPE_REQ;
			break;
			
		case TO_UI_TASK_CALIBRATION_READ_REQ:
			message.msgID = TO_SDO_TASK_CALIBRATION_READ_REQ;
			break;

		default:
			bIsIdValid	= false;
			LOGDBG("UI_T : Invalid msgId to send to SDOTask msgQ : %d",message.msgID,0,0);
			
			break;
	 }
	 
	if(bIsIdValid)
	{
		if(msgQSend(SDO_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			LOGERR("UI_T : Message send failed to SDO_T Q",0,0,0);
		}
	 }
 }
 

 /*************************************************************************//**
  * \brief   - Prepare message and sends to DB_Task message queue.
  *
  * \param   - struct Message&.
  *
  * \return  - None.
  *
  ******************************************************************************/
void UserInterface::SendMessageToDBTask(Message& message)
{
	 bool bIsIdValid = true;
	 char *pBuffer	 = NULL;
	 
	 switch(message.msgID)
	 {
		 case TO_UI_TASK_ALARM_CONFIG_SAVE_REQ:
			 message.msgID = TO_DB_TASK_ALARM_CONFIG_SAVE_REQ;
			 break;
		 case TO_UI_TASK_GEN_SINGLE_REPORT_REQ:
		 	 message.msgID = TO_DB_TASK_GEN_SINGLE_REPORT_REQ;
		 	 break;
			 
		 case TO_UI_TASK_GEN_REPORT_ACTION_IND:
			 message.msgID = TO_DB_TASK_GEN_REPORT_ACTION_IND;
			 break;
		 	 
		 case TO_UI_TASK_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_RES:
			 memset(&message, 0x00, sizeof(message));
			 message.msgID = TO_DB_TASK_BARCODE_RECIPE_RECALL_REQ;
			 memcpy(message.Buffer, m_pBarcodeData, sizeof(m_pBarcodeData));
			 break;
			 
		 case TO_UI_TASK_BARCODE_RECIPE_RECALL_RSP:
			 memset(&message, 0x00, sizeof(message));			 
			 message.msgID = TO_DB_TASK_BARCODE_RECIPE_RECALL_RSP;		 
			 break;
			 
		 case TO_UI_TASK_BATCH_COUNT_RESET_REQ :
			 message.msgID = TO_DB_TASK_BATCH_COUNT_RESET_REQ;
			 break;
			 
		 case TO_UI_TASK_SYS_CONFIG_WRITE_REQ:			 			
			 message.msgID = TO_DB_TASK_MEMORY_FULL_STORAGE_CHECK;		 
			 break;

		 	 
		default:
			bIsIdValid	= false;
			LOGDBG("UI_T : Invalid msgId to send to DBTask msgQ : %d", message.msgID, 0, 0);
			break;
	 }
	 
	if(bIsIdValid)
	{
		pBuffer = reinterpret_cast<char*>(&message);
		if(msgQSend(DB_MSG_Q_ID, pBuffer, sizeof(message), NO_WAIT, MSG_PRI_NORMAL) != OK)
		{
			LOGDBG("UI_T : Message send failed to DB_T Q", 0, 0, 0);
		}
	 }
}
 
/*************************************************************************//**
 * \brief   - Process the received message from UI_MSG_Q.
 *
 * \param   - struct Message&.
 *
 * \return  - None.
 *
 ******************************************************************************/
void UserInterface::ProcessMessage(Message& message)
{
	UINT32 msgLen = 0;
	UINT8 status;
	INT32 socketStatus = 0;
	string details;
	char signal;
	
	switch(message.msgID)
	{
		case TO_UI_TASK_TEST_CMD_REQ:	
			SendMessageToControlTask(message);
			break;

		case TO_UI_TASK_TEST_ABORT_CMD_REQ:
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_WELD_CMD:
			SendMessageToControlTask(message);
			break;
		
		case TO_UI_TASK_SEEK_CMD:
			SendMessageToControlTask(message);
			break;
		
		case TO_UI_TASK_SETUP_CMD:
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_SET_NEXT_OPERATION_REQ:
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_HORN_SCAN_CMD:
			SendMessageToControlTask(message);
			break;
		
		case TO_UI_TASK_HORN_SCAN_ABORT_CMD:
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_EMMC_WEAR_LEVEL_REQ:
			SendMessageToControlTask(message);
			break;

		case TO_UI_TASK_OPTIMIZE_DATABASE_REQ:
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_RESET_ALARM_CMD:
			message.msgID = TO_ALARM_TASK_RESET_ALARM;
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			break;

		case TO_UI_TASK_CLEAR_ALL:		                                               
			// send it to the alarm task
			message.msgID = TO_ALARM_TASK_UI_CLEAR_ALL;
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			break;

		case TO_UI_TASK_DEFAULT_ALARM_CONFIG_REQ:
			// send it to the alarm task
			message.msgID = TO_ALARM_TASK_DEFAULT_ALARM_CONFIG_REQ;
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			break;

		case TO_UI_TASK_ALARM_MGR_DEFAULT_CONFIG:
			message.msgID = UIC_DEFAULT_ALARM_CONFIG_RSP;	
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_ALARM_CONFIG_READ_REQ:
			// send it to the alarm task
			message.msgID = TO_ALARM_TASK_ALARM_CONFIG_REQ;
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			break;

		case TO_UI_TASK_ALARM_MGR_CONFIG_REQ:
			message.msgID = UIC_ALARM_CONFIG_READ_RSP;	
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_ALARM_CONFIG_SAVE_REQ:
			
			// send it to the alarm task
			message.msgID = TO_ALARM_TASK_ALARM_CONFIG_SAVE_REQ;
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			break;
			
		case TO_UI_TASK_ALARM_MGR_CONFIG_SAVE:
			message.msgID = UIC_ALARM_CONFIG_SAVE_RSP;
			if(message.Buffer[0] != '0')
			{
#ifndef DEBUG_PRINT		
				LOGWARN("----------- UI_T: Alarm configuration write to DB Failed!!! ------------",0,0,0);
#endif
				memset(message.Buffer, 0x00, sizeof(message.Buffer));
				sprintf(message.Buffer,"%d",RESPONSE_FAIL);
			}
			else
			{
				memset(message.Buffer,0x00,sizeof(message.Buffer));
				sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
			}						
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_CALIBRATION_CMD_REQ:
			CalibrationHandling(message);
			break;
		
		case TO_UI_TASK_SC_STATE_REQ:
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_TEST_RECIPE_REQ:			 		 
			UpdateTestRecipe(message);			 			
			break;
			
		case TO_UI_TASK_SEEK_RECIPE:
			UpdateSeekRecipe(message);
			break;
			
		case TO_UI_TASK_STACK_RECIPE:
			UpdateStackRecipe(message);
			break;
			
		case TO_UI_TASK_VERSION_REQUEST:
			 SendMessageToSDOTask(message);
			 break;
			 
		case TO_CALIBRATION_DATA_REQUEST:
			 SendMessageToSDOTask(message);
			break;
			 
		case TO_UI_TASK_TMR_INTVL_DATA_PC: 
			SendMessageToSDOTask(message);
			break;
			
		case TO_UI_TASK_POWER_UP_RECIPE:
			SendMessageToSDOTask(message);
			break;
			
		case TO_UI_TASK_WELD_RECIPE_SC:
			UpdateRecipeSC(message);
			break;
			
		case TO_UI_TASK_WELD_RECIPE:
            UpdateRecipePcAc(message);
			break;
			
		case TO_UI_TASK_RECIPE_UPDATE_STOP_REQ:
			m_bIsActiveRecipe = true;	/* Stop updating active weld recipe - System is performing weld */
#ifdef UI_RESPONSE_DEBUG
			LOGDBG("UI_T : -----------Stop updating weld recipe-----------",0,0,0);
#endif
			break;

		case TO_UI_TASK_HORN_SCAN_RECIPE:
			UpdateScanRecipe(message);
			break;

		case TO_UI_TASK_WELD_RECIPE_RESP_PC:
#ifdef UI_RESPONSE_DEBUG
			LOGDBG("UI_T : -----------Weld Recipe configured in PC-----------",0,0,0);
#endif
			SdoResponseStatus(message.Buffer);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;

		case TO_UI_TASK_WELD_RECIPE_RESP_AC:
#ifdef UI_RESPONSE_DEBUG
			LOGDBG("UI_T : -----------Weld Recipe configured in AC-----------",0,0,0);
#endif
			SdoResponseStatus(message.Buffer);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;

		case TO_UI_TASK_SEEK_RECIPE_CONFIGURED:
			LOGDBG("UI_T : ------------Seek Recipe Configured----------",0,0,0);
			if(bIsPowerOnSeek)
			{
				SendMessageToControlTask(message);
				bIsPowerOnSeek = false;
			}
			message.msgID = UIC_SEEK_RECIPE_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
	
		case TO_UI_TASK_DOWNLOAD_POWERUP_RESP:
			LOGDBG("UI_T : ------------PowerUp Recipe Configured----------",0,0,0);
			PC_RX->MasterEvents |= BIT_MASK(CTRL_POWER_UP_RECIPE);

			break;
		case TO_UI_TASK_HORN_SCAN_RECIPE_RESP:
			LOGDBG("UI_T : ------------HorScan Recipe Configured----------",0,0,0);
			SdoResponseStatus(message.Buffer);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_HORN_SCAN_RESULT:
		case TO_UI_TASK_HORN_SCAN_RES:
			LOGDBG("UI_T : ------------HorScan Request Response----------",0,0,0);
			message.msgID = TO_UI_TASK_HORN_SCAN_RESULT;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_SCAN_ABORT_CMD_RESP:
			LOGDBG("UI_T : ------------HorScan Abort Done----------",0,0,0);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_VERSION_REQUEST_RESP_AC:
		{
			LOGDBG("UI_T : ------------ AC Version Info Saved ----------",0,0,0);
			// storing AC version info locally, it can be retrieved by the UI
			FWVersion::Set(FW_VERSION_AC, VERSION_MAJOR, ((pFirmwareVersion) message.Buffer)->FwMajor);
			FWVersion::Set(FW_VERSION_AC, VERSION_MINOR, ((pFirmwareVersion) message.Buffer)->FwMinor);
			FWVersion::Set(FW_VERSION_AC, VERSION_BUILD, ((pFirmwareVersion) message.Buffer)->FwBuild);
			FWCrc::Set(FW_CRC_AC, ((pFirmwareVersion) message.Buffer)->FwCrc);
			FWInfo::Set(FW_INFO_AC, ((pFirmwareVersion) message.Buffer)->FwInfo);
			
			DataBaseManager::EnterEventLogSWVersion (FW_INFO_AC);
		}
		break;

		case TO_UI_TASK_VERSION_REQUEST_RESP_PC:
		{
			LOGDBG("UI_T : ------------ PC Version Info Saved ----------",0,0,0);
			// storing PC version info locally, it can be retrieved by the UI
			FWVersion::Set(FW_VERSION_PC, VERSION_MAJOR, ((pFirmwareVersion) message.Buffer)->FwMajor);
			FWVersion::Set(FW_VERSION_PC, VERSION_MINOR, ((pFirmwareVersion) message.Buffer)->FwMinor);
			FWVersion::Set(FW_VERSION_PC, VERSION_BUILD, ((pFirmwareVersion) message.Buffer)->FwBuild);
			FWCrc::Set(FW_CRC_PC, ((pFirmwareVersion) message.Buffer)->FwCrc);
			FWInfo::Set(FW_INFO_PC, ((pFirmwareVersion) message.Buffer)->FwInfo);
			
			DataBaseManager::EnterEventLogSWVersion (FW_INFO_PC);
		}
		break;
		case TO_UI_CALIBRATION_DATA_REQUEST_RESP:
		{
			if(((pCalibrationReadings) message.Buffer)->FirstReading == 0)
			{
				CalTimeStamp = DEFAULT_RTC_TIME;
				LOGDBG("Sytem is not calibrated, perform calibration!",0,0,0);
			}
			else
			{
				sprintf(cal_temp_data,"%d/%02d/%02d %02d:%02d:%02d",((pCalibrationReadings) message.Buffer)->tm_year,((pCalibrationReadings) message.Buffer)->tm_mon,((pCalibrationReadings) message.Buffer)->tm_mday,
						((pCalibrationReadings) message.Buffer)->tm_hour,((pCalibrationReadings) message.Buffer)->tm_min,((pCalibrationReadings) message.Buffer)->tm_sec);
				CalTimeStamp = cal_temp_data;
			}
		}
		break;
			
		case TO_UI_TASK_TMR_INTVL_RESP_PC:		
			LOGDBG("UI_T : ------------Timer Interval Recipe Configured----------",0,0,0);
			PC_RX->MasterEvents |= BIT_MASK(CTRL_TMR_INTERVAL);
		    break;
			
		case TO_UI_TASK_CALIBRATION_READ_RES:
			LOGDBG("UI_T : ------------Calibration Done----------",0,0,0);
			message.msgID = TO_UI_TASK_CALIBRATION_CMD_REQ;
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_CALIBRATION_CMD_RES:
			LOGDBG("UI_T : ------------Calibration request response----------",0,0,0);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_PROGRESS_BAR_STATUS:
			NotifyResponseToClient(message,sizeof(ErrCode));
			break;
			
		case TO_UI_TASK_CHECK_NEW_ACTIVE_RECIPE:
			/*message.msgID = TO_UI_TASK_CHECK_NEW_ACTIVE_RECIPE;
			socketStatus = NotifyResponseToClient(message, msgLen);
			// If sending socket is failed or broken then blocks the subsequent welds
			if(ERROR == socketStatus)		
			{
				memset(message.Buffer,0x00,sizeof(message.Buffer));
				message.msgID = TO_UI_TASK_SET_NEXT_OPERATION_REQ;
				SendMessageToControlTask(message);				
			}*/
			UpdateActiveRecipe();
			break;
			
		case TO_UI_TASK_UL_WELD_RESULT:
			message.msgID = TO_UI_TASK_UL_WELD_RESULT;
			msgLen = strlen(message.Buffer);

			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_UL_SEEK_RESULT:
		case TO_UI_TASK_SEEK_RES:
			LOGDBG("UI_T : ------------Seek Request Response---------- : %d",message.msgID,0,0);
			message.msgID = UIC_SEEK_CMD_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_TEST_RECIPE_RES:			 
			LOGDBG("UI_T : ------------Test Recipe Configured----------",0,0,0);
			SdoResponseStatus(message.Buffer);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;

		case TO_UI_TASK_TEST_CMD_RES:			 
			LOGDBG("UI_T : ------------Test Request Response----------",0,0,0);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_TEST_ABORT_CMD_RES:
			LOGDBG("UI_T : ------------Test Abort Done----------",0,0,0);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_TEST_PROGRESS_IND:	
			LOGDBG("UI_T : ------------Test Progress Data to UIC----------",0,0,0);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_SYSREADY_IND_REQ:
			message.msgID = UIC_UW_SYSREADY_IND;						
			msgLen = ResponseMessage(RESPONSE_SUCCESS,message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_ALARM_MGR_ALARM_IND:
			message.msgID = UIC_ALARM_DATA_INDICATION;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_ACTIVE_ALARM_REQ:
			// send it to the alarm task
			message.msgID = TO_ALARM_TASK_ACTIVE_ALARM_REQ;
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			break;
			
		case TO_UI_TASK_ALARM_MGR_ACTIVE_ALARM_REQ:
			
			message.msgID = UIC_ACTIVE_ALARM_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_ALARM_MGR_RESET_RESP:
			
			message.msgID = UIC_ALARM_RESET_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_OPTIMIZE_DATABASE_RESP:
			

			message.msgID = UIC_OPTIMIZE_DATABASE_RESP;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_ALARM_MGR_RESET_ALL_RESP:
			
			message.msgID = UIC_ALARM_RESET_ALL_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_ALARM_MGR_RESET_ALL_REQ:
			
			message.msgID = UIC_ALARM_CLEAR_REQ;
			msgLen = strlen(message.Buffer);
			
			STATUS socketResponse = NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_SYSINFO_READ_REQ:
			message.msgID = UIC_SYSINFO_READ_RES;
			memset(message.Buffer, 0x00, sizeof(message.Buffer));
			msgLen = AccessSystemInformation(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_SETUP_RESP:
			message.msgID = UIC_SETUP_CMD_RES;
			msgLen = sizeof(ActuatorSetup);
			NotifyResponseToClient(message, msgLen);
#ifdef UI_RESPONSE_DEBUG
			{
				ActuatorSetup setup;				
				memcpy(&setup,message.Buffer,sizeof(ActuatorSetup));
				LOGDBG("Length of actuator response msg : %d",strlen(message.Buffer),0,0);
				LOGDBG("RespType	: %d",setup.ResponseType,0,0);
				LOGDBG("RespStatus	: %d",setup.ResponseStatus,0,0);
				LOGDBG("EPC			: %d",setup.ExpectedPartContact,0,0);
				LOGDBG("Force		: %d",setup.ActualForce,0,0);
				LOGDBG("Distance	: %d",setup.ActualDistance,0,0);
			}
#endif
			break;
			
		case TO_UI_TASK_SC_STATE_RES:
			message.msgID = UIC_SC_STATE_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_FEATURERUN_READ_REQ:
			message.msgID  = UIC_FEATURERUN_READ_RES;
			msgLen = GetSystemModel(message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;

		case TO_UI_TASK_SYS_CONFIG_READ_REQ:
			memset(message.Buffer,0x00,sizeof(message.Buffer));
			message.msgID  = UIC_SYSCONFIG_READ_RES;
			msgLen = AccessSystemConfiguration(READ,message.Buffer);
			memcpy(&SysConfig,message.Buffer,sizeof(SysConfig));
			PowerOnRequest(POWER_UP_RECIPE);			
			NotifyResponseToClient(message,msgLen);
			break;

		case TO_UI_TASK_SYS_CONFIG_WRITE_REQ:
			AccessSystemConfiguration(WRITE,message.Buffer);
			sysConf.SysConfigGet((char *)&sysConfig);
			message.msgID = UIC_SYSCONFIG_WRITE_RES;
			msgLen = ResponseMessage(RESPONSE_SUCCESS,message.Buffer);
			memcpy(&SysConfig,&sysConfig,sizeof(sysConfig));			
			PowerOnRequest(POWER_UP_RECIPE);
			NotifyResponseToClient(message,msgLen);
			message.msgID = TO_UI_TASK_SYS_CONFIG_WRITE_REQ;	
			SendMessageToDBTask(message);
			break;
			
		case TO_UI_TASK_EMERGENCY_STOP_ACTIVE_IND:
			message.msgID = UIC_EMERGENCY_STOP_ACTIVE_IND;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;
			
		case TO_UI_TASK_EMERGENCY_STOP_RESET_IND:
			message.msgID = UIC_EMERGENCY_STOP_RESET_IND;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;
			
		case TO_UI_TASK_ESTOP_RESET_REQ:
			// send it to the alarm task
			message.msgID = TO_ALARM_TASK_ESTOP_RESET;
			msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[ALARM_T]), reinterpret_cast<char*>(&message), sizeof (message), NO_WAIT,MSG_PRI_NORMAL);
			break;
			
		case TO_UI_TASK_ESTOP_RESET_RES:
			message.msgID = UIC_EMERGENCY_STOP_RESET_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;
			
		case TO_UI_TASK_IP_CONFIGURATION_READ_REQ:
			memset(message.Buffer,0x00,sizeof(message.Buffer));
			message.msgID = UIC_IP_CONFIGURATION_READ_RES;
			msgLen = AccessIPConfiguration(READ,message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;
			
		case TO_UI_TASK_IP_CONFIGURATION_WRITE_REQ:
			message.msgID = UIC_IP_CONFIGURATION_WRITE_RES;
			status = AccessIPConfiguration(WRITE,message.Buffer);
			msgLen = ResponseMessage(status,message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;	
			
		case TO_UI_TASK_DATETIME_READ_REQ:
			memset(message.Buffer, 0x00, sizeof(message.Buffer));
			msgLen = AccessDateAndTime(READ,message.Buffer);
			message.msgID = UIC_SYS_DATETIME_READ_RES;
			NotifyResponseToClient(message,msgLen);
			break;
			
		case TO_UI_TASK_DATETIME_WRITE_REQ:
			message.msgID = UIC_SYS_DATETIME_WRITE_RES;
			status=AccessDateAndTime(WRITE,message.Buffer);
			msgLen = ResponseMessage(status,message.Buffer);
			NotifyResponseToClient(message,msgLen);
			break;
			
		case TO_UI_TASK_GEN_SINGLE_REPORT_REQ:
			message.msgID=TO_UI_TASK_GEN_SINGLE_REPORT_REQ;
			SendMessageToDBTask(message);
			break;	
			
		case TO_UI_TASK_GEN_SINGLE_REPORT_RES:
			message.msgID = SCBL_GEN_SINGLE_REPORT_RSP;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_GEN_REPORT_ACTION_IND:
			message.msgID=TO_UI_TASK_GEN_REPORT_ACTION_IND;
			memcpy(&status, message.Buffer, sizeof(status));
			if(CONTINUE_CMD == status)
			{
				SendMessageToDBTask(message);			
			}
			
			break;
			
		case TO_UI_GEN_DB_REPORT_REMIND_LATER_REQ:
			message.msgID=SCBL_GEN_REPORT_REMIND_LATER_RSP;
			sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_DB_FILE_COPY_TO_USB_REQ:
			message.msgID = UIC_DB_FILE_COPY_TO_USB_RES;
			if(m_objDBFileHandler.CopyDBFile(EMMC_TO_USB))
			{
				msgLen = ResponseMessage(RESPONSE_SUCCESS,message.Buffer);
			}
			else
			{
				msgLen = ResponseMessage(RESPONSE_FAIL,message.Buffer);
			}
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_DB_FILE_DELETE_FROM_USB_REQ:
			message.msgID = TO_DB_TASK_DB_CONNECTION_CLOSE_REQ;
			SendToMsgQ(message,UIDB_MSG_Q_ID);
			SendToMsgQ(message,DB_MSG_Q_ID);
			break;
			
		case TO_DB_TASK_DB_CONNECTION_CLOSE_RES:
			memset(message.Buffer,0x00,sizeof(message.Buffer));
			if(m_objDBFileHandler.DeleteDBFileFromEMMC())
			{
				// record DB deletion signal in EEPROM
				signal = EVENTLOG_DBDELETED_SIG_ACTIVE;
				DataBaseManager::EnterEventLogNVM (SYSTEM_DB_DELETED, &signal);
				
				message.msgID = UIC_DB_FILE_DELETE_FROM_USB_RES;
				msgLen = ResponseMessage(RESPONSE_SUCCESS,message.Buffer);
				NotifyResponseToClient(message, msgLen);
			}
			else
			{
				message.msgID = UIC_DB_FILE_DELETE_FROM_USB_RES;
				msgLen = ResponseMessage(RESPONSE_FAIL,message.Buffer);
				NotifyResponseToClient(message, msgLen);
			}
			break;
			
		case TO_DB_TASK_DB_CONNECTION_OPEN_RES:
			message.msgID = UIC_DB_FILE_DELETE_FROM_USB_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_BARCODE_DATA:
			DecodeBarcodeData(message);
			break;

		case TO_UI_TASK_BARCODE_RECIPE_RECALL_REQ:									
			message.msgID = UIC_BARCODE_RECIPE_RECALL_REQ;		/* Notify UIC the active recipe ID read from the barcode for the UIC to update the same on the UI */
			msgLen = strlen(message.Buffer);			
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_BARCODE_SCANNER_STATUS_IND:	
			message.msgID = UIC_BARCODE_SCANNER_STATUS_IND;			/* Notify UIC the Barcode scanner connect status */
			msgLen = strlen(message.Buffer);			
			NotifyResponseToClient(message, msgLen);			
			break;

		/* Response from UIC the authentication for recipe recall */
		case TO_UI_TASK_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_RES:						
			/* If the User has permission, do a recipe recall from the barcode */			
			if(message.Buffer[0] == '1')
			{	
				SendMessageToDBTask(message);	
			}
			else
			{
				LOGWARN("----------- UI_T: User doesn't have permission to do recipe recall!!! ------------",0,0,0);
			}						
			break;			

			/* Response from UIC to make the recipe active or not  */
		case TO_UI_TASK_BARCODE_RECIPE_RECALL_RSP:											
			if(message.Buffer[0] == '1')
			{				
				SendMessageToDBTask(message);	
			}
			else
			{						
				LOGWARN("----------- UI_T: Recipe Recall Response from UI - Fail!!! ------------",0,0,0);
			}						
			break;			
			
		case TO_UI_TASK_BARCODE_ERROR_IND:						
			message.msgID = UIC_BARCODE_ERROR_IND;			/* Notify UIC the Barcode recall failure indication */
			msgLen = strlen(message.Buffer);			
			NotifyResponseToClient(message, msgLen);			
			break;
			
		case TO_UI_TASK_BATCH_COUNT_RESET_REQ:
			SendMessageToControlTask(message);
			message.msgID=TO_UI_TASK_BATCH_COUNT_RESET_REQ;
			SendMessageToDBTask(message);
			break;
			
		case TO_UI_TASK_BATCH_COUNT_RESET_RSP:
			message.msgID=UIC_BATCH_COUNT_RESET_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
		
		case TO_UI_TASK_BATCH_COUNT_EXCEED_CMD:
			#ifdef UI_RESPONSE_DEBUG
				LOGDBG("UI_T : ------------BATCH COUNT EXCEED CMD TO CTRL TASK----------",0,0,0);
			#endif
			SendMessageToControlTask(message);
			break;
			
		case TO_UI_TASK_USERIO_READ_REQ :
			
			message.msgID = UI_TO_CTRL_TASK_USERIO_READ_REQ;
			
			GetUserIO(message);
			
			if(msgQSend (CTRL_MSG_Q_ID, reinterpret_cast<char*> (&message), sizeof(message), NO_WAIT,MSG_PRI_NORMAL) != OK)
				LOGERR("UI_T : Message send failed to Ctrl_Task",0,0,0);
			
			break;
			
		case TO_UI_TASK_USERIO_READ_RES:
			message.msgID =UIC_USERIO_READ_RES;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_UI_TASK_EMMC_WEAR_LEVEL_RESP:
			
			message.msgID = UIC_EMMC_WEAR_LEVEL_RESP;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;

		case TO_TASK_WELD_PROGRESS_IND:
			message.msgID = UIC_WELD_PROGRESS_IND;
		 	msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
		 	break;	
			
		case TO_UI_TASK_BRANSON_KEY_REQ:
			LOGDBG("UI_T : Branson key request received",0,0,0);
			AccessBransonKey(message);
			break;
			
		case TO_UI_TASK_FEATURERUN_WRITE_REQ:
			LOGDBG("UI_T : Run time feature write request received",0,0,0);
			status = SetSystemModel(message.Buffer);
			memset(message.Buffer, 0x00, sizeof(message.Buffer));
			if(status == TRUE)
			{				
				msgLen = ResponseMessage(RESPONSE_SUCCESS, message.Buffer);	
			}
			else
			{				
				msgLen = ResponseMessage(RESPONSE_FAIL, message.Buffer);
			}
			message.msgID = UIC_FEATURERUN_WRITE_RES;
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_DEFAULT_FEATURERUN_REQ:	
			LOGDBG("UI_T : Default Run time configuration read request received",0,0,0);
			memset(&message, 0x00, sizeof(message));
			message.msgID = UIC_DEFAULT_FEATURERUN_RES;
			GetDefaultRTConfig(message.Buffer);			
			NotifyResponseToClient(message, FEATUREFLAG_SIZE);
			break;

			
		case TO_UI_TASK_FACTORY_RESET_REQ:
			LOGDBG("UI_T : Factory Reset request received!!!",0,0,0);			
			status = FactoryReset();
			memset(&message, 0x00, sizeof(message));
			if(status == TRUE)
			{				
				msgLen = ResponseMessage(RESPONSE_SUCCESS, message.Buffer);	
			}
			else
			{				
				msgLen = ResponseMessage(RESPONSE_FAIL, message.Buffer);
			}
			message.msgID = UIC_FACTORY_RESET_RSP;					
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_REPORT_DELETE_RSP:			
			ProcessDeleteReportRes(message);		
			break;
	 	
		case TO_UI_TASK_ALARM_DATA_INDICATION:					
			message.msgID = UIC_ALARM_DATA_INDICATION;	
			msgLen = strlen(message.Buffer);			
			while(NotifyResponseToClient(message, msgLen) == SYSTEM_READY_IND_NOT_SENT);			
			break;
		
		case TO_UI_TASK_WELD_DATA_DELETE_IND:
			message.msgID = TO_UIC_WELD_DATA_DELETE_IND;	
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_MEMORY_OFFSET_RESET_REQ:
			ClearMemoryOffset();
			break;
			
		/* TnM Feature end */
		case TO_UI_TASK_FW_UPGRADE_ETHER_IND:
			LOGDBG("UI_T : FW_UPGRADE_ETHER_NOTIFY_ID : %d",message.msgID ,0,0);
			LOGDBG("UI_T : FW_UPGRADE_ETHER_NOTIFY_DATA : %s",(_Vx_usr_arg_t )message.Buffer ,0,0);
			message.msgID = UIC_FW_UPGRADE_ETHER_IND;
			msgLen = strlen(message.Buffer);
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_WEBSERVICE_USER_LOGGED_INFO:									
			message.msgID = UIC_WEBSERVICE_USER_LOGGED_INFO;		/* Notify UIC that there was a WebSrvices Login or Logout */
			msgLen = strlen(message.Buffer);			
			NotifyResponseToClient(message, msgLen);
			break;
			
		case TO_UI_TASK_WEBSERVICE_REFRESH_SCREEN:									
			message.msgID = UIC_WEBSERVICE_REFRESH_SCREEN;		/* Tell the UI to refresh the active screen */
			msgLen = strlen(message.Buffer);			
			NotifyResponseToClient(message, msgLen);
			break;
						
		case SHUTDOWN_APPLICATION:
			break;
			
		default:
			LOGERR("UI_T : --------Unknown Message ID----------- : ",message.msgID,0,0);
			break;
	}
}

/*************************************************************************//**
* \brief   -  Decode Barcode data 
*
* \param   -  Message &message - Bar code message
*
* \return  -  None
*
******************************************************************************/
void UserInterface::DecodeBarcodeData(Message &message)
{
	UINT32 msgLen = 0;
	
	/* Copy only the bar code data and not the Message ID, hence ignoring first four bytes */
	memset(m_pBarcodeData, 0x00, sizeof(m_pBarcodeData));					
	memcpy(&m_pBarcodeData, message.Buffer, sizeof(m_pBarcodeData));
	
	LOGDBG("\n\t\t Decoded Barcode:%s\n", *m_pBarcodeData, 0, 0);
	
	sysConf.SysConfigGet((char *)&sysConfig);
	memset(&message, 0x00, sizeof(message));
	
	/* Recipe Bar code */	
	if(m_pBarcodeData[0] == sysConfig.PresetRecipeBarcodeStart)			
	{								
		/* Request UIC the authentication for recipe recall */
		message.msgID = UIC_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_REQ;		
		sprintf(message.Buffer,"%d",1);
		msgLen = strlen(message.Buffer);			
		NotifyResponseToClient(message, msgLen);			
	}
	else	/* Part ID Bar code */			
	{
		if(strlen(m_pBarcodeData) > 1)									/* If the part id size is greater than 1, it is a valid part id. */				
		{
			CP->weldResult.SetPartID(m_pBarcodeData);				
			CP->weldResult.SetPartIDScanStatus(true);					/* Part ID scan done enabled for weld reference */
			/* Notify UIC the Part ID picked up from the Bar code */														
			memcpy(message.Buffer, m_pBarcodeData, sizeof(m_pBarcodeData));							
			message.msgID = UIC_BARCODE_PART_ID_IND;					/* Notify UIC the Part ID read from the bar code */
			msgLen = strlen(message.Buffer);			
			NotifyResponseToClient(message, msgLen);			
		}
		else
		{
			/* Notify UIC that the scanned part id is invalid */														
			sprintf(message.Buffer,"%d",INVALID_PART_ID);
			message.msgID = UIC_BARCODE_ERROR_IND;			
			msgLen = strlen(message.Buffer);			
			NotifyResponseToClient(message, msgLen);			
		}
	}			
}

/*************************************************************************//**
* \brief   -  System configuration read and write. 
*
* \param   -  UINT8 accessType, char *pBuffer
*
* \return  -  UINT16
*
******************************************************************************/
UINT16 UserInterface::AccessSystemConfiguration(UINT8 accessType, char *pSysConfigData)
{
	SystemConfiguration SysConfig;
	char _systemConfig_read[MAX_SIZE_OF_MSG_BUFF];
	intf_SysConfig_t _systemConfig_old;
	intf_SysConfig_t _systemConfig_new;
	
	if(READ == accessType)
	{
		SysConfig.SysConfigGet(pSysConfigData);
	}
	else if(WRITE == accessType)
	{
		SysConfig.SysConfigGet(_systemConfig_read);
		memcpy (&_systemConfig_old, _systemConfig_read, sizeof (_systemConfig_old));
		SysConfig.SysConfigSet(pSysConfigData);
		memcpy (&_systemConfig_new, pSysConfigData, sizeof (_systemConfig_new));
		
		LogSystemConfiguration (_systemConfig_old, _systemConfig_new);
	}
	else
	{
		/* Feature implementation */
	}
	CP->SetSysConfContext((intf_SysConfig_t  *)pSysConfigData);
	return sizeof(intf_SysConfig_t);
}

/**************************************************************************//**
* \brief   - Read and write IP configuration of CPSW port 2 
*
* \param   - UINT8 accessType, char *pIPConfigData
*
* \return  - UINT16 length of byte read or written
*
******************************************************************************/
UINT16 UserInterface::AccessIPConfiguration(UINT8 accessType, char *pIPConfigData)
{
	IPConfig IPConfObj;
	UINT16 response = 0;

	
	if(READ == accessType)
	{
		IPConfObj.GetIPParameters(pIPConfigData);

		response = IPCONF_PARAM_LENGTH;
	}
	else if(WRITE == accessType)
	{
		response = IPConfObj.SetIPParameters(pIPConfigData);
	}
	
	return response;
}

/**************************************************************************//**
* \brief   - Get and Set RTC date and time.
*
* \param   - UINT8 accessType, char *pRtcConfigData
*
* \return  - UINT16 length of byte read or written
*
******************************************************************************/
UINT16 UserInterface::AccessDateAndTime(UINT8 accessType, char *pRtcConfigData)
{
	string 	TimeStamp, details, from, to;
	RTCtimerStr SysTime={0x00};
	UINT16 response = 0;
	
	if(READ == accessType)
	{
		GetDateAndTime(TimeStamp);
		sprintf(pRtcConfigData, "%s", (char *)TimeStamp.c_str()) ; 
		response = TimeStamp.length();
	}
	else if(WRITE == accessType)
	{
		// send it to the event log
		GetDateAndTime (from);
		to = pRtcConfigData;
		details = "Identifier;N/A;From;" + from + ";To;" + to + ";";
		DataBaseManager::EnterEventLog (SYSTEM_TIME_CHANGED, details);
		
		sscanf(pRtcConfigData,"%d/%d/%d %d:%d:%d",&SysTime.tm_year,&SysTime.tm_mon,&SysTime.tm_mday,&SysTime.tm_hour,&SysTime.tm_min,&SysTime.tm_sec);
		SysTime.tm_year = SysTime.tm_year-1900;
		SysTime.tm_mon = SysTime.tm_mon-1;
		CP->ind_RTC_LowBatt = false;
		response = SetRTC(SysTime);
	}
	
	return response;
}

/**************************************************************************//**
* \brief   - System information details collected and sends to UIC 
*
* \param   - char *pSysInfData
*
* \return  - UINT16 length of byte read or written
*
******************************************************************************/
UINT16 UserInterface::AccessSystemInformation(char *pSysInfData)
{
	RunTime RunTimeModel;
	SystemConfiguration SysConfigObj;
	intf_SysConfig_t sysConfigData;
	SystemInformation sysInfObj;
	bool UbootVersionFlag;
	FirmwareUpgrade Fwupdate;
	Eeprom eeprom;
	char macaddr [6];
	char macAddr[20]={0x00};

	memset(&sysInfObj, 0x00, sizeof(sysInfObj));
	/*HardCoded Values*/
	sysInfObj.psLifeCounter			= Counters::getCounter(LIFETIME_WELDS);
	sysInfObj.overloadAlarmCounter  = Counters::getCounter(LIFETIME_OVERLOADS);
	sysInfObj.actuatorlifecounter	= Counters::getCounter(LIFETIME_ACTUATIONS);
	sysInfObj.generalAlarmCounter	= Counters::getCounter(LIFETIME_ALARMS);
	sysInfObj.actuatorOverloads		= 0;
	sysInfObj.calibrationStatus		= 0;
	
	/*System Configuration Values*/
	memset(&sysConfigData, 0x00, sizeof(intf_SysConfig_t));
	SysConfigObj.SysConfigGet((char *)&sysConfigData);
	
	sysInfObj.psFrequency			= sysConfigData.PcConfig.PSFreq;
	sysInfObj.psWatt				= sysConfigData.PcConfig.PSWatt;
	sysInfObj.psType				= sysConfigData.PcConfig.PsTypeIs;
	sysInfObj.actuatorType			= sysConfigData.ActConfig.ActuatorTypeIs;
	sysInfObj.actuatorStrokeLength 	= sysConfigData.ActConfig.MaxStrokelength;
		
	SysConfigObj.GetSystemTypeName(sysInfObj.modelName);

#ifdef MAINLINE_BUGFIX_BUILD
	sprintf(sysInfObj.version_SC,"%d.%d.%d.%d", FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),
												FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
												FWVersion::Get(FW_VERSION_SC, VERSION_BUILD),
												SC_FW_VERSION_AUTOS);

	sprintf(sysInfObj.version_PC,"%d.%d.%d.%d",	FWVersion::Get(FW_VERSION_PC,VERSION_MAJOR),
												FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
												FWVersion::Get(FW_VERSION_PC, VERSION_BUILD),
												((FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR) == 2 && FWVersion::Get(FW_VERSION_PC, VERSION_MINOR) == 0) ? 1 : PC_FW_VERSION_AUTOS));

	sprintf(sysInfObj.version_AC,"%d.%d.%d.%d",	FWVersion::Get(FW_VERSION_AC,VERSION_MAJOR),
												FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
												FWVersion::Get(FW_VERSION_AC, VERSION_BUILD),
												AC_FW_VERSION_AUTOS);
	
#else
	sprintf(sysInfObj.version_SC,"%d.%d.%d",FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR), 
											FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_SC, VERSION_BUILD));

	sprintf(sysInfObj.version_PC,"%d.%d.%d",FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),
											FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_PC, VERSION_BUILD));

	sprintf(sysInfObj.version_AC,"%d.%d.%d",FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
											FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
											FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
#endif	
	
	sprintf(sysInfObj.pc_FPGA_Version,"%s", GetFPGAVersion().c_str());
	
	/*HardCoded Values*/
	sprintf(sysInfObj.psAssemblyNumber, "%s", "xxxxxxxxxx");
	sprintf(sysInfObj.actuatorAssembly, "%s", "xxxxxxxxxx");
	sprintf(sysInfObj.stackAssembly, "%s", "xxxxxxxxxx");
	
	
	/* read the MAC address for CPSW1 */
	eeprom.Read((char *) macaddr, EEPROM_MAC_ADDRESS_SIZE, EEPROM_MAC_ADDRESS_OFFSET);
	snprintf(macAddr, sizeof (macAddr), "%02X:%02X:%02X:%02X:%02X:%02X",
			macaddr[0],
			macaddr[1],
			macaddr[2],
			macaddr[3],
			macaddr[4],
			macaddr[5]);

	sprintf(sysInfObj.psMacID, "%s", macAddr);
	sprintf(sysInfObj.psIP, "%s", "192.168.1.100");
	
	/*Getting Date and Time Format*/
	//GetDateAndTime(TimeStamp);
	sprintf(sysInfObj.dateTime, "%s", (char *)CalTimeStamp.c_str());
	
	/*Changed by MOUNIKA*/
	UbootVersionFlag = Fwupdate.IsNewBootloaderForCRC();
	
	if(UbootVersionFlag)
	{
		sysInfObj.crc_SC	= FWCrc::Get(FW_CRC_SC);
		sysInfObj.crc_PC	= FWCrc::Get(FW_CRC_PC);
		sysInfObj.crc_AC	= FWCrc::Get(FW_CRC_AC);
	}
	else
	{
		sysInfObj.crc_SC	= 0;
		sysInfObj.crc_PC	= 0;
		sysInfObj.crc_AC	= 0;
	}
	
	memcpy(pSysInfData, (char *)&sysInfObj, sizeof(sysInfObj));
	UpdateFirmwareUpgradeEvent(sysInfObj.version_SC, sysInfObj.version_PC, sysInfObj.version_AC);
	
	return sizeof(sysInfObj);
}

/**************************************************************************//**
* 
* \brief   - Updates firmware upgrade event.
*
* \param   - char *fwVersionSC, char *fwVersionPC, char *fwVersionAC
*
* \return  - None.
*
******************************************************************************/
void UserInterface::UpdateFirmwareUpgradeEvent(char *fwVersionSC, char *fwVersionPC, char *fwVersionAC)
{
	char fwUpgradeFlag= 0x00;
	Eeprom eepromObj;
	char prevFwVersionSC[SYSINFO_SIZE] = { 0x00 };
	char prevFwVersionPC[SYSINFO_SIZE] = { 0x00 };
	char prevFwVersionAC[SYSINFO_SIZE] = { 0x00 };
	Message message;
	
	eepromObj.Read((char *)&fwUpgradeFlag,sizeof(fwUpgradeFlag), FW_FLAG_EEPROM_OFFSET);
	
	if ( fwUpgradeFlag == FW_UPGRADE_SUCCESS )
	{
		/* Write Upgrade Success Flag to EEPROM */
		fwUpgradeFlag = FW_UPGRADE_RESET;
		eepromObj.Write((char *) &fwUpgradeFlag, sizeof(fwUpgradeFlag), FW_FLAG_EEPROM_OFFSET); 
		eepromObj.Read((char *) prevFwVersionSC, SYSINFO_SIZE, SC_FW_VER_EEPROM_OFFSET);
		eepromObj.Read((char *) prevFwVersionPC, SYSINFO_SIZE, PC_FW_VER_EEPROM_OFFSET);
		eepromObj.Read((char *) prevFwVersionAC, SYSINFO_SIZE, AC_FW_VER_EEPROM_OFFSET); 
		
		if (( strcmp(prevFwVersionSC,fwVersionSC) != 0 ) || (strcmp(prevFwVersionPC,fwVersionPC) != 0) 
																			|| (strcmp(prevFwVersionAC,fwVersionAC) != 0))
		{
		}		
	}	
}

/**************************************************************************//**
* 
* \brief   - Updates the seek recipe details, Which is received from QT-UI.
*
* \param   - Message& a_seekRecipe.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::UpdateSeekRecipe(Message& a_seekRecipe)
{
	seekRecipe seek_Recipe;
	
	memcpy(&seek_Recipe,a_seekRecipe.Buffer,sizeof(seek_Recipe));
	CP->m_SeekRecipeObj.UpdateSeekRecipe(seek_Recipe);
	LOGDBG("UI_T : seekTime : %d",seek_Recipe.SeekTime,0,0);
	
	seek_Recipe.SeekFreqLimitHigh 	= seek_Recipe.SeekFreqLimitHigh * 10;
	seek_Recipe.SeekFreqLimitLow  	= seek_Recipe.SeekFreqLimitLow * 10;
	CP->MemClearBeforeSeek			= seek_Recipe.SeekMemClearBeforeSeek;
	
	memset(a_seekRecipe.Buffer, 0x00, sizeof(a_seekRecipe.Buffer));
	memcpy(a_seekRecipe.Buffer, &seek_Recipe, (sizeof(seek_Recipe) - sizeof(seek_Recipe.SeekTime)));
	
	SendMessageToSDOTask(a_seekRecipe);
}

/**************************************************************************//**
* \brief   - Updates scan recipe details which is received from QT-UI.
*
* \param   - Message& a_scanRecipe
*
* \return  - None
*
******************************************************************************/
void UserInterface::UpdateScanRecipe(Message& a_scanRecipe)
{
	ScanRecipe scanRecipe;
	
	memcpy(&scanRecipe,a_scanRecipe.Buffer,sizeof(scanRecipe));
	CP->m_ScanRecipeObj.UpdateScanRecipe(scanRecipe);
	SendMessageToSDOTask(a_scanRecipe);
}

/**************************************************************************//**
* \brief   - Updates test recipe into class members
*
* \param   - Message& a_testRecipe
*
* \return  - None.
*
******************************************************************************/
void UserInterface::UpdateTestRecipe(Message& a_testRecipe)
{
	testRecipe test_Recipe;
	WeldRecipePC recipe;
		
	memcpy(&test_Recipe, a_testRecipe.Buffer, sizeof(test_Recipe));
	CP->m_TestRecipeObj.UpdateTestRecipe(test_Recipe);
	CP->m_RecipePC.GetWeldRecipe(recipe);
	
	recipe.AmplitudeLoopC1 	= CP->m_TestRecipeObj.GetAmplitudeLoopC1();
	recipe.AmplitudeLoopC2 	= CP->m_TestRecipeObj.GetAmplitudeLoopC2();
	recipe.PhaseLoop	   	= CP->m_TestRecipeObj.GetPhaseLoopI();
	recipe.PhaseLoopCF	   	= CP->m_TestRecipeObj.GetPhaseLoopCF();
	recipe.FrequencyLow	   	= CP->m_TestRecipeObj.GetFrequencyLow();
	recipe.FrequencyHigh   	= CP->m_TestRecipeObj.GetFrequencyHigh();
	recipe.PhaseLimitTime	= TEST_PHASELIMITTIME;
	recipe.PhaseLimit		= TEST_PHASELIMIT;
	
	memset(a_testRecipe.Buffer, 0x00, sizeof(a_testRecipe.Buffer));
	memcpy(a_testRecipe.Buffer, &recipe, sizeof(recipe));
	SendMessageToSDOTask(a_testRecipe);
}

/**************************************************************************//**
* 
* \brief   - Updates the seek recipe details, Which is received from QT-UI.
*
* \param   - Message& seekRecipe.
*
* \return  - UINT8 - status.
*
******************************************************************************/
UINT8 UserInterface::UpdateStackRecipe(Message& stackRecipe)
{
	UINT8  status 				= RESPONSE_SUCCESS;
	INT8   idx 					= 0;
	INT32  EndofWeldStore		= 0;
	INT32  InternalOffsetFlag	= 0;
	INT32  DigitalTune 			= 0; 
	INT32  InternalFreqOffset	= 0;
	
	memcpy(&DigitalTune,stackRecipe.Buffer,sizeof(DigitalTune));
    idx += sizeof(DigitalTune);
    LOGDBG("digitaltune = %d", DigitalTune,0,0);
    
    memcpy(&InternalOffsetFlag, stackRecipe.Buffer+idx, sizeof(InternalOffsetFlag));
    idx += sizeof(InternalOffsetFlag);
    LOGDBG("InternalOffsetFlag = %d",InternalOffsetFlag,0,0);
    
    memcpy(&InternalFreqOffset, stackRecipe.Buffer+idx, sizeof(InternalFreqOffset));
    idx += sizeof(InternalFreqOffset);
    LOGDBG("int=%d",InternalFreqOffset,0,0);
    
    memcpy(&EndofWeldStore, stackRecipe.Buffer+idx, sizeof(EndofWeldStore));
    idx += sizeof(EndofWeldStore);
    LOGDBG("end = %d",EndofWeldStore,0,0);
	LOGDBG("Digital Tune in the system = %d",CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE),0,0);
	CP->SetDigitalTune(DigitalTune*10);
	CP->stackRecipe.SetData(StackRecipe::DIGITAL_TUNE, (DigitalTune*10));
	CP->stackRecipe.SetData(StackRecipe::INTERNAL_OFFSET_FLAG, InternalOffsetFlag);
	CP->stackRecipe.SetData(StackRecipe::INTERNAL_FREQ_OFFSET, InternalFreqOffset);
	CP->stackRecipe.SetData(StackRecipe::END_OF_WELD_STORE, EndofWeldStore);

	LOGDBG("Digital Tune after update=%d",CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE),0,0);
	
	CP->recipe["MemoryOffset"] = 0;
	CP->recipe["StartFrequency"] = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
	
	PcWeldRecipe *m_RecipePC = new(nothrow) PcWeldRecipe(CP->getPcWeldRecipeInstance());
	if(NULL != m_RecipePC)
	{
			m_RecipePC->UpdateDigitalTune(CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));	/* Update Digital Tune*/
			m_RecipePC->UpdateMemoryOffset(0);	/* Updates MemoryOffset*/
			m_RecipePC->UpdateResonantFreq(CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));	/* Update StartFrequency*/
			//m_RecipePC->UpdateFrequencyOffset(CP->stackRecipe.GetData(StackRecipe::INTERNAL_FREQ_OFFSET)); /*Update Frequency Offset*/
		    CP->UpdatePcWeldRecipe(*m_RecipePC);				/* Updates the weld recipe PC instance */
			delete m_RecipePC;
			m_RecipePC = NULL;
	}
	else
	{
		status = RESPONSE_FAIL;
		LOGERR("Stack recipe : memory allocation failed",0,0,0);
	}
	
	return status;
}

/**************************************************************************//**
* 
* \brief   - Updates the active recipe details, Which is received from QT-UI. 
*
* \param   - Message& activeRecipe - Data of SC
*
* \return  - None.
*
******************************************************************************/
void UserInterface::UpdateRecipeSC(Message& activeRecipe)
{
	RunTime RT;
	RT.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);
	
#ifdef DEBUG
	LOGDBG("AMPLITUDE FLAG 	:%d",RT.ModelFeature.RtFeature1.AmplitudeStep_Flag_B11,0,0);
	LOGDBG("FORCE FLAG  	:%d",RT.ModelFeature.RtFeature1.ForceStep_Flag_B12,0,0);
#endif
	
		/* Copies all the recipe data into SC structure, Which has been sent by consoleApp or UIC */
		memcpy(&ActiveRecipeSC,activeRecipe.Buffer,sizeof(WeldRecipeSC));

		RunTime::SetModeValue(ActiveRecipeSC.WeldMode);

		if(DISABLE == RT.ModelFeature.RtFeature1.AmplitudeStep_Flag_B11)
		{
			LOGERR("UI_T : -------AMPLITUDE STEP DISABLED-------",0,0,0);
			ActiveRecipeSC.NumAmplitudeSteps = 1;
		}
		
		if(DISABLE == RT.ModelFeature.RtFeature1.ForceStep_Flag_B12)
		{
			LOGERR("UI_T : -------FORCE STEP DISABLED-------",0,0,0);
			ActiveRecipeSC.NumForceSteps = 1;
		}

		/* Update all active recipes of SC with latest changes by user in QT-UI */
	m_SC.SetActiveRecipe(ActiveRecipeSC);

#ifndef DEBUG
	m_SC.RecipeInformation();
	m_SC.DisplayAmplitudeStep();
	m_SC.DisplayForceStep();
	m_SC.DisplaySuspectLimit();
	m_SC.DisplayRejectLimit();
	m_SC.DisplayAfterBurst();
	m_SC.DisplayPreTrigger();
	m_SC.DisplayEnergyBrake();
	m_SC.DisplaycutoffLimit();
#endif
		
	if(!m_bIsActiveRecipe)
	{	
		CompareRecipeNumber(ActiveRecipeSC.RecipeNumber);
		/* Set the active recipe details to store along with results */
		CommonProperty::weldResult.Set(WeldResults::WELD_MODE, ActiveRecipeSC.WeldMode);
		CommonProperty::weldResult.Set(WeldResults::RECIPE_NUM, ActiveRecipeSC.RecipeNumber);
		CommonProperty::weldResult.Set(WeldResults::RECIPE_REV_NUM, ActiveRecipeSC.RecipeRevNumber);
		CommonProperty::weldResult.SetUserName(ActiveRecipeSC.User);
		CommonProperty::weldResult.Set(WeldResults::STACK_SERIAL_NUM, ActiveRecipeSC.StackSerialNo);

		CP->UpdateActiveRecipe(m_SC);
	}
	else
	{
		bIsPendingRecipe = true;	/* Whenever new recipe received from UIC and system performing weld flag sets */ 
	}
}

/**************************************************************************//**
* 
* \brief   - convert Actuator Setup Advanced Feature parameter to Uint16 byte before it sent to slave
*
* \param   - None
*
* \return  - bStatus
*
******************************************************************************/
bool UserInterface::ActuatorSetupAdvFeatureParamConv()
{
	bool bStatus = true;
	WeldRecipeAC recipe;
	AcWeldRecipe *AC = new AcWeldRecipe(CP->getAcWeldRecipeInstance());
	if(NULL != AC)
	{
		AC->GetWeldRecipe(recipe);
		/* During part contact, Actuator setUp Advanced Feature Parameter value get copied to RecipeAcCompaWithAcBoard 
		 * Structure varaible which is compatible with AC board */
		RecipeAcCompaWithAcBoard.WeldForce = recipe.WeldForce;
		RecipeAcCompaWithAcBoard.ForceRampTime = recipe.ForceRampTime;
		RecipeAcCompaWithAcBoard.HoldMode = recipe.HoldMode;
		RecipeAcCompaWithAcBoard.TotalCollapseTarget = recipe.TotalCollapseTarget;
		RecipeAcCompaWithAcBoard.HoldForce = recipe.HoldForce;
		RecipeAcCompaWithAcBoard.HoldForceRampTime = recipe.HoldForceRampTime;
		RecipeAcCompaWithAcBoard.ExpectedPartContactPosition = recipe.ExpectedPartContactPosition;
		RecipeAcCompaWithAcBoard.ReadyPosition = recipe.ReadyPosition;

		RecipeAcCompaWithAcBoard.DownAcceleration = recipe.DownAcceleration / 10000;
		RecipeAcCompaWithAcBoard.DownMaxVelocity = recipe.DownMaxVelocity / 10000;
		RecipeAcCompaWithAcBoard.DownDeceleration = recipe.DownDeceleration / 10000;
		RecipeAcCompaWithAcBoard.ReturnAcceleration = recipe.ReturnAcceleration / 10000;
		RecipeAcCompaWithAcBoard.ReturnMaxVelocity = recipe.ReturnMaxVelocity / 10000;
		RecipeAcCompaWithAcBoard.ReturnDeceleration = recipe.ReturnDeceleration /10000;

		RecipeAcCompaWithAcBoard.ExpectedPartContactOffset = recipe.ExpectedPartContactOffset;
		RecipeAcCompaWithAcBoard.PartContactWindowMinus = recipe.PartContactWindowMinus;
		RecipeAcCompaWithAcBoard.PartContactWindowPlus = recipe.PartContactWindowPlus;
		RecipeAcCompaWithAcBoard.NumForceSteps = recipe.NumForceSteps;
		RecipeAcCompaWithAcBoard.ReadyPositionToggle = recipe.ReadyPositionToggle;
		RecipeAcCompaWithAcBoard.WeldForceControl = recipe.WeldForceControl;
		copy(recipe.ForceStepRampTime, recipe.ForceStepRampTime + (sizeof(recipe.ForceStepRampTime)/sizeof(recipe.ForceStepRampTime[0])), RecipeAcCompaWithAcBoard.ForceStepRampTime);
    	copy(recipe.ForceStepForce, recipe.ForceStepForce + (sizeof(recipe.ForceStepForce)/sizeof(recipe.ForceStepForce[0])), RecipeAcCompaWithAcBoard.ForceStepForce);
    	
    	bStatus = true;
	}
	else
	{
		bStatus = false;
	}
}

/****************************************************************************
* \brief   - Updates active recipe of PC, AC and forwards the same to slaves.
*
* \param   - Message& activeRecipe - Data of PC and AC
*
* \return  - void
*
******************************************************************************/
void UserInterface::UpdateRecipePcAc(Message& activeRecipe)
{
	UINT32 msgLen = 0;

	m_PC = CP->getPcWeldRecipeInstance();
	m_AC = CP->getAcWeldRecipeInstance();

	memcpy(&RecipePC,activeRecipe.Buffer,sizeof(WeldRecipePC));		/* Copies all the recipe data into PC, AC structure, Which has been sent by consoleApp */
	memcpy(&RecipeAC,activeRecipe.Buffer + sizeof(WeldRecipePC),sizeof(WeldRecipeAC));

	RecipePC.StartFrequency  = m_PC.GetResonantFreq();	/* Overriding calculated resonant frequency from previous seek or scan or weld operations with UIC value */
	RecipePC.DigitalTune 	 = m_PC.GetDigitalTune();	/* Overriding calculated digital tune from previous seek or scan operations with UIC value */
	RecipePC.MemoryOffset 	 = m_PC.GetMemoryOffset();	/* TBD */
	RecipePC.FrequencyOffset = m_PC.GetFrequencyOffset();

#ifdef DEBUG
	LOGDBG("RecipePC.StartFrequency : %d",RecipePC.StartFrequency,0,0);
	LOGDBG("RecipePC.DigitalTune 	: %d",RecipePC.DigitalTune,0,0);
	LOGDBG("RecipePC.MemoryOffset 	: %d",RecipePC.MemoryOffset,0,0);
	LOGDBG("RecipePC.FrequencyOffset: %d",RecipePC.FrequencyOffset,0,0);
#endif

	/* RecipeAC.ExpectedPartContactPosition = m_AC.GetPartContactPos(); Latest part contact position updated for new recipe(from UIC) before send to AC */

#ifdef IMPLEMENT	/* Need to remove from here */
	SetPCWeldRecipeWithMinValue();	/* Assign default values to recipe struct object for validation */
	SetPCWeldRecipeWithMaxValue();
	SetACWeldRecipeWithMinValue();
	SetACWeldRecipeWithMaxValue();
#endif

	/* if((ValidateRecipe(RecipePC)) && (ValidateRecipe(RecipeAC))) */
	if(1)
	{
		m_PC.SetWeldRecipe(RecipePC);
		m_AC.SetWeldRecipe(RecipeAC);

		if(!m_bIsActiveRecipe)	/* Update and send active recipe to slaves only when system is not performing weld operation */
		{
			CP->UpdatePcWeldRecipe(m_PC);	/* Update all active recipes of PC and AC with latest changes by user in UIC */
			CP->UpdateAcWeldRecipe(m_AC);

			memset(activeRecipe.Buffer,0x00,sizeof(activeRecipe.Buffer));
			memcpy(&activeRecipe.Buffer[0],&RecipePC,sizeof(RecipePC));
			ActuatorSetupAdvFeatureParamConv();
			memcpy(&activeRecipe.Buffer[sizeof(RecipePC)],&RecipeAcCompaWithAcBoard,sizeof(RecipeAcCompaWithAcBoard));
			SendMessageToSDOTask(activeRecipe);		/* Send PC and AC active weld recipes to slaves with help of EcSDO task */
		}
		else
		{
			bIsPendingRecipe = true;
			/* Response to UIC for updated weld recipe received */
			memset(activeRecipe.Buffer,0x00,sizeof(activeRecipe.Buffer));
			activeRecipe.msgID = TO_UI_TASK_WELD_RECIPE_RESP_PC;
			msgLen = ResponseMessage(RESPONSE_SUCCESS,activeRecipe.Buffer);
			NotifyResponseToClient(activeRecipe,msgLen);

			memset(activeRecipe.Buffer,0x00,sizeof(activeRecipe.Buffer));
			activeRecipe.msgID = TO_UI_TASK_WELD_RECIPE_RESP_AC;
			msgLen = ResponseMessage(RESPONSE_SUCCESS,activeRecipe.Buffer);
			NotifyResponseToClient(activeRecipe,msgLen);
		}
	}
	else 
	{
		LOGWARN("Cannot send weld recipe to slaves",0,0,0);
	}

#ifdef DEBUG				
	PrintRecipe(RecipePC);
	PrintRecipe(RecipeAC);
	PrintRecipe(RecipeAcCompaWithAcBoard);
#endif /* DEBUG */
}

/**************************************************************************//**
* \brief   - Update active weld recipe which is received from UIC. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void UserInterface::UpdateActiveRecipe()
{
	Message	activeRecipe;
	
	if((m_bIsActiveRecipe) && (bIsPendingRecipe))
	{
		bIsPendingRecipe 	= false;
		
		CompareRecipeNumber(ActiveRecipeSC.RecipeNumber);
		
		/* Set the active recipe details to store along with results */
		CommonProperty::weldResult.Set(WeldResults::WELD_MODE, ActiveRecipeSC.WeldMode);
		CommonProperty::weldResult.Set(WeldResults::RECIPE_NUM, ActiveRecipeSC.RecipeNumber);
		CommonProperty::weldResult.Set(WeldResults::RECIPE_REV_NUM, ActiveRecipeSC.RecipeRevNumber);
		CommonProperty::weldResult.SetUserName(ActiveRecipeSC.User);
		CommonProperty::weldResult.Set(WeldResults::STACK_SERIAL_NUM, ActiveRecipeSC.StackSerialNo);

		m_PC.UpdateResonantFreq(CP->m_RecipePC.GetResonantFreq());
		m_PC.UpdateMemoryOffset(CP->m_RecipePC.GetMemoryOffset());
		m_PC.UpdateDigitalTune(CP->m_RecipePC.GetDigitalTune());
		m_PC.UpdateFrequencyOffset(CP->m_RecipePC.GetFrequencyOffset());
		m_AC.UpdatePartContactPos(CP->m_RecipeAC.GetPartContactPos());
		
		CP->UpdateActiveRecipe(m_SC);
		CP->UpdatePcWeldRecipe(m_PC);	/* Update all active recipes of PC and AC with latest changes by user in UIC */
		CP->UpdateAcWeldRecipe(m_AC);

		activeRecipe.msgID = TO_UI_TASK_WELD_RECIPE;
		memcpy(&activeRecipe.Buffer[0],&RecipePC,sizeof(RecipePC));
		ActuatorSetupAdvFeatureParamConv();
		memcpy(&activeRecipe.Buffer[sizeof(RecipePC)],&RecipeAcCompaWithAcBoard,sizeof(RecipeAcCompaWithAcBoard));
		SendMessageToSDOTask(activeRecipe);		/* Send PC and AC active weld recipes to slaves with help of EcSDO task */
		
#ifdef DEBUG				
	PrintRecipe(RecipePC);
	PrintRecipe(RecipeAC);
	PrintRecipe(RecipeAcCompaWithAcBoard);
#endif /* DEBUG */
	}
#ifdef UI_RESPONSE_DEBUG
	else
	{
		LOGDBG("UI_T : No new active recipe received from UIC",0,0,0);
	}
#endif
	m_bIsActiveRecipe 	= false;
}

/**************************************************************************//**
* \brief   - Compares new recipe received from UIC with current recipe 
* 			 based on recipe number and if true post message to DB task-Q.  
*
* \param   - INT32 recipeNumber
*
* \return  - None.
*
******************************************************************************/
void UserInterface::CompareRecipeNumber(INT32 recipeNumber)
{
	/* Checks previous recipe number and new recipe number received from UIC and post message to DB task to update the cycle counter and batch counter */ 
	if(recipeNumber != CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM))
	{
		Message send;
		send.msgID = TO_DB_TASK_NEW_RECIPE_RECEIVED; 
		SendToMsgQ(send,DB_MSG_Q_ID, MSG_PRI_URGENT);
	}
	else
	{
		LOGDBG("UI_T : There is no change in recipe number",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Reads system model based on runtime configuration 
*
* \param   - char *pSystemModelData
*
* \return  - UINT16 length of byte read
*
******************************************************************************/
UINT16 UserInterface::GetSystemModel(char *pSystemModelData)
{
	RunTime RTF;
	
	RTF.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);
	memcpy(pSystemModelData,&RTF.ModelFeature,FEATUREFLAG_SIZE);
	return FEATUREFLAG_SIZE;
}

/**************************************************************************//**
* \brief   - Set Run time configuration from UI 
*
* \param   - char *pRunTimeData
*
* \return  - None.
*
******************************************************************************/
STATUS UserInterface :: SetSystemModel(char *pRunTimeData)
{		
	RunTime runTime;
	return runTime.FeaturesFlagWrite(pRunTimeData);
}


/**************************************************************************//**
* \brief   - Factory reset actions: Set default configuration for Alarm, System and 
* 			Run time.  
*
* \param   - Message& message.
*
* \return  - bool.
*
******************************************************************************/
bool UserInterface :: FactoryReset()
{	
	SystemConfiguration sysConf;		//TODO: Sys info and continuous report disable pending.
	bool success = false;
	if(sysConf.FactoryResetEeprom() == 0)
	{
		LOGDBG("Configuration Factory Reset is successful", 0, 0, 0);
		success = true;
	}
	return success;
}


/**************************************************************************//**
* \brief   - Get Default Run time configuration. 
*
* \param   - char *pRunTimeData
*
* \return  - None.
*
******************************************************************************/
void UserInterface :: GetDefaultRTConfig(char *pRunTimeData)
{		
	RunTime runTime;
	return runTime.GetDefaultModelConf(pRunTimeData);
}

/*************************************************************************//**
* \brief   -  returns the active alarm data when UIC requests   
*
* \param   -  char *pData
*
* \return  -  bool - ret - available or not
*
******************************************************************************/
bool UserInterface::GetActiveAlarmData(char *pAlarmData)
{
	bool ret = false;
	if(m_ActiveAlarm != NULL)
	{
		strcpy(pAlarmData, m_ActiveAlarm);
		ret = true;
	}
	return ret;
}

/*************************************************************************//**
* \brief   	-  	Logs any changes made to the system configuration
* 				when it is saved    
*
* \param   	-  	intf_SysConfig_t& systemConfig_old
* \param 	- 	intf_SysConfig_t& systemConfig_new
*
* \return  	-  	None
*
******************************************************************************/
void UserInterface::LogSystemConfiguration (intf_SysConfig_t& systemConfig_old, intf_SysConfig_t& systemConfig_new)
{
	string details, from, to;
	vector<string> events;
	char	buffer[10];
	UINT16	idx;
	
	// check for all changes here...
	if (systemConfig_old.AuthCheckLevel != systemConfig_new.AuthCheckLevel)
	{
		from = systemConfig_old.AuthCheckLevel == FULL ? "FULL:" : "DISABLED:";
		to = systemConfig_new.AuthCheckLevel == FULL ? "FULL:" : "DISABLED:";
		details = "identifier:AUTHORITY CHECK:From:" + from + "To:" + to;
		events.push_back (details);
	}
	
	if (strcmp (systemConfig_old.machineName, systemConfig_new.machineName) != 0)
	{
		from = systemConfig_old.machineName;
		from +=  ":";
		to = systemConfig_new.machineName;
		to +=  ":";
		details = "Identifier:MACHINE NAME:From:" + from + "To:" + to;
		events.push_back (details);
	}
	
	if (strcmp (systemConfig_old.AC_SerialNumber, systemConfig_new.AC_SerialNumber) != 0)
	{
		from = systemConfig_old.AC_SerialNumber;
		from +=  ":";
		to = systemConfig_new.AC_SerialNumber;
		to +=  ":";
		details = "Identifier:ACTUATOR SERIAL:From:" + from + "To:" + to;
		events.push_back (details);
	}
	
	if (systemConfig_old.ActConfig.ActuatorTypeIs != systemConfig_new.ActConfig.ActuatorTypeIs)
	{
		from = systemConfig_old.ActConfig.ActuatorTypeIs == 0 ? "SERVO:" : "PNEUMATIC:";
		to = systemConfig_new.ActConfig.ActuatorTypeIs == 0 ? "SERVO:" : "PNEUMATIC:";
		details = "Identifier:ACTUATOR TYPE:From:" + from + "To:" + to;
		events.push_back (details);
	}

	if (systemConfig_old.ActConfig.ControlLevelIs != systemConfig_new.ActConfig.ControlLevelIs)
	{
			if (systemConfig_old.ActConfig.ControlLevelIs == GSX_E) from = "GSX E:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == P1_PNEUMATIC) from = "P1 PNEUMATIC:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == P2_PNEUMATIC) from = "P2 PNEUMATIC:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == I_IW) from = "I IW:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == MERCURY) from = "MERCURY:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == SATURNP1) from = "SATURNP 1:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == SATURNP2) from = "SATURNP 2:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == MARS) from = "MARS:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == JUNO) from = "JUNO:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == VENUS) from = "VENUS:";
			else if (systemConfig_old.ActConfig.ControlLevelIs == VENUSHH) from = "VENUS HH:";
			else from = "ASX:";

			if (systemConfig_new.ActConfig.ControlLevelIs == GSX_E) to ="GSX E:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == P1_PNEUMATIC) to ="P1 PNEUMATIC:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == P2_PNEUMATIC) to ="P2 PNEUMATIC:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == I_IW) to ="I IW:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == MERCURY) to ="MERCURY:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == SATURNP1) to ="SATURNP 1:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == SATURNP2) to ="SATURNP 2:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == MARS) to ="MARS:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == JUNO) to ="JUNO:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == VENUS) to ="VENUS:";
			else if (systemConfig_new.ActConfig.ControlLevelIs == VENUSHH) from = "VENUS HH:";
			else to ="ASX:";

			details = "Identifier:ACTUATOR CONTROL:From:" + from + "To:" + to;
			events.push_back (details);
	}
	
	if (systemConfig_old.ActConfig.MaxStrokelength != systemConfig_new.ActConfig.MaxStrokelength)
	{
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_old.ActConfig.MaxStrokelength);
		from = buffer;
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_new.ActConfig.MaxStrokelength);
		to = buffer;
		details = "Identifier:ACTUATOR STROKE:From:" + from + "To:" + to;
		events.push_back (details);

	}

	if (strcmp (systemConfig_old.AuxBoxSerialNumber, systemConfig_new.AuxBoxSerialNumber) != 0)
	{
		from = systemConfig_old.AuxBoxSerialNumber;
		from +=  ":";
		to = systemConfig_new.AuxBoxSerialNumber;
		to +=  ":";
		details = "Identifier:AUXILIARY SERIAL:From:" + from + "To:" + to;
		events.push_back (details);
	}
	
	if (systemConfig_old.LanguageIs != systemConfig_new.LanguageIs)
	{
		if (systemConfig_old.LanguageIs == ENGLISH) from = "ENGLISH:";
		else if (systemConfig_old.LanguageIs == FRENCH) from = "FRENCH:";
		else if (systemConfig_old.LanguageIs == GERMAN) from = "GERMAN:";
		else if (systemConfig_old.LanguageIs == SPANISH) from = "SPANISH:";
		else if (systemConfig_old.LanguageIs == KOREAN) from = "KOREAN:";
		else if (systemConfig_old.LanguageIs == TRADITIONAL_CHINESE) from = "TRADITIONAL_CHINESE:";
		else if (systemConfig_old.LanguageIs == SIMPLIFIED_CHINESE) from = "SIMPLIFIED_CHINESE:";
		else if (systemConfig_old.LanguageIs == ITALIAN) from = "ITALIAN:";
		else if (systemConfig_old.LanguageIs == JAPANESE) from = "JAPANESE:";
		else if (systemConfig_old.LanguageIs == SLOVAK) from = "SLOVAK:";
		else from = "DANISH:";

		if (systemConfig_new.LanguageIs == ENGLISH) to ="ENGLISH:";
		else if (systemConfig_new.LanguageIs == FRENCH) to ="FRENCH:";
		else if (systemConfig_new.LanguageIs == GERMAN) to ="GERMAN:";
		else if (systemConfig_new.LanguageIs == SPANISH) to ="SPANISH:";
		else if (systemConfig_new.LanguageIs == KOREAN) to ="KOREAN:";
		else if (systemConfig_new.LanguageIs == TRADITIONAL_CHINESE) to ="TRADITIONAL_CHINESE:";
		else if (systemConfig_new.LanguageIs == SIMPLIFIED_CHINESE) to ="SIMPLIFIED_CHINESE:";
		else if (systemConfig_new.LanguageIs == ITALIAN) to ="ITALIAN:";
		else if (systemConfig_new.LanguageIs == JAPANESE) to = "JAPANESE:";
		else if (systemConfig_new.LanguageIs == SLOVAK) to = "SLOVAK:";
		else to ="DANISH:";

		details = "Identifier:LANGUAGE CHANGE:From:" + from + "To:" + to;
		events.push_back (details);

	}
	
	if (systemConfig_old.MemFullActionIs != systemConfig_new.MemFullActionIs)
	{
		from = systemConfig_old.MemFullActionIs == MEM_FULL_STOP ? "STOP:" : "CONTINUE:";
		to = systemConfig_new.MemFullActionIs == MEM_FULL_STOP ? "STOP:" : "CONTINUE:";
		details = "identifier:MEMORY FULL ACTION:From:" + from + "To:" + to;
		events.push_back (details);

	}

	if (strcmp (systemConfig_old.PC_SerialNumber, systemConfig_new.PC_SerialNumber) != 0)
	{
		from = systemConfig_old.PC_SerialNumber;
		from +=  ":";
		to = systemConfig_new.PC_SerialNumber;
		to +=  ":";
		details = "Identifier:POWER SUPPLY SERIAL:From:" + from + "To:" + to;
		events.push_back (details);

	}
	
	if (systemConfig_old.PcConfig.ControlLevelIs != systemConfig_new.PcConfig.ControlLevelIs)
	{
		if (systemConfig_old.PcConfig.ControlLevelIs == GSX_E) from = "GSX E:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == P1_PNEUMATIC) from = "P1 PNEUMATIC:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == P2_PNEUMATIC) from = "P2 PNEUMATIC:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == I_IW) from = "I IW:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == MERCURY) from = "MERCURY:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == SATURNP1) from = "SATURNP 1:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == SATURNP2) from = "SATURNP 2:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == MARS) from = "MARS:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == JUNO) from = "JUNO:";
		else if (systemConfig_old.PcConfig.ControlLevelIs == VENUS) from = "VENUS:";
		else from = "VENUS HH:";

		if (systemConfig_new.PcConfig.ControlLevelIs == GSX_E) to ="GSX E:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == P1_PNEUMATIC) to ="P1 PNEUMATIC:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == P2_PNEUMATIC) to ="P2 PNEUMATIC:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == I_IW) to ="I IW:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == MERCURY) to ="MERCURY:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == SATURNP1) to ="SATURNP 1:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == SATURNP2) to ="SATURNP 2:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == MARS) to ="MARS:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == JUNO) to ="JUNO:";
		else if (systemConfig_new.PcConfig.ControlLevelIs == VENUS) to ="VENUS:";
		else to ="VENUS HH:";

		details = "Identifier:POWER SUPPLY CONTROL:From:" + from + "To:" + to;
		events.push_back (details);
	}

	if (systemConfig_old.PcConfig.PSFreq != systemConfig_new.PcConfig.PSFreq)
	{
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_old.PcConfig.PSFreq);
		from = buffer;
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_new.PcConfig.PSFreq);
		to = buffer;
		details = "Identifier:POWER SUPPLY FREQUECNY:From:" + from + "To:" + to;
		events.push_back (details);

	}
	
	if (systemConfig_old.PcConfig.PSWatt != systemConfig_new.PcConfig.PSWatt)
	{
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_old.PcConfig.PSWatt);
		from = buffer;
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_new.PcConfig.PSWatt);
		to = buffer;
		details = "Identifier:POWER SUPPLY WATTS:From:" + from + "To:" + to;
		events.push_back (details);
	}
	
	if (systemConfig_old.PcConfig.PsTypeIs != systemConfig_new.PcConfig.PsTypeIs)
	{
		// only standard type is supported
	}
	
	if (systemConfig_old.PresetRecipeBarcodeStart != systemConfig_new.PresetRecipeBarcodeStart)
	{
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_old.PresetRecipeBarcodeStart);
		from = buffer;
		snprintf(buffer, sizeof(buffer),"%d:", systemConfig_new.PresetRecipeBarcodeStart);
		to = buffer;
		details = "Identifier:PRESET BARCODE:From:" + from + "To:" + to;
		events.push_back (details);
	}	
	
	if (systemConfig_old.PsPowerUpOptionIs != systemConfig_new.PsPowerUpOptionIs)
	{
		if (systemConfig_old.PsPowerUpOptionIs == PS_SEEK) from = "PS SEEK:";
		else if (systemConfig_old.PsPowerUpOptionIs == PS_SCAN) from = "PS SACN:";
		else from = "PS NONE:";

		if (systemConfig_new.PsPowerUpOptionIs == PS_SEEK) to = "PS SEEK:";
		else if (systemConfig_new.PsPowerUpOptionIs == PS_SCAN) to = "PS SACN:";
		else to = "PS NONE:";

		details = "Identifier:POWER SUPPLY OPTION:From:" + from + "To:" + to;
		events.push_back (details);

	}

	if (strcmp (systemConfig_old.SC_SerialNumber, systemConfig_new.SC_SerialNumber) != 0)
	{
		from = systemConfig_old.SC_SerialNumber;
		from +=  ":";
		to = systemConfig_new.SC_SerialNumber;
		to +=  ":";
		details = "Identifier:SUPERVISORY SERIAL:From:" + from + "To:" + to;
		events.push_back (details);

	}
	
	if (systemConfig_old.Sscreen != systemConfig_new.Sscreen)
	{
		if (systemConfig_old.Sscreen == DASHBOARD) from = "DASHBOARD:";
		else if (systemConfig_old.Sscreen == PRODUCTION) from = "PRODUCTION:";
		else if (systemConfig_old.Sscreen == RECIPES) from = "RECIPES:";
		else from = "ANALYTICS:";

		if (systemConfig_new.Sscreen == DASHBOARD) to = "DASHBOARD:";
		else if (systemConfig_new.Sscreen == PRODUCTION) to = "PRODUCTION:";
		else if (systemConfig_new.Sscreen == RECIPES) to = "RECIPES:";
		else to = "ANALYTICS:";
		
		details = "Identifier:SCREEN CHANGE:From:" + from + "To:" + to;
		events.push_back (details);
	}

	if (systemConfig_old.UnitIs != systemConfig_new.UnitIs)
	{
		if (systemConfig_old.UnitIs == METRIC) from = "METRIC:";
		else from = "IMPERIAL:";

		if (systemConfig_new.UnitIs == METRIC) to = "METRIC:";
		else to = "IMPERIAL:";

		details = "Identifier:UNITS CHANGE:From:" + from + "To:" + to;
		events.push_back (details);
	}

	if (systemConfig_old.bIsExternalPresets != systemConfig_new.bIsExternalPresets)
	{
		from = systemConfig_old.bIsExternalPresets ? "YES:" : "NO:";
		to = systemConfig_new.bIsExternalPresets ? "YES:" : "NO:";

		details = "Identifier:EXTERNAL PRESET:From:" + from + "To:" + to;
		events.push_back (details);
	}
	
	
	if (systemConfig_old.bIsHornClamp != systemConfig_new.bIsHornClamp)
	{
		from = systemConfig_old.bIsHornClamp ? "YES:" : "NO:";
		to = systemConfig_new.bIsHornClamp ? "YES:" : "NO:";

		details = "Identifier:HORN CLAMP:From:" + from + "To:" + to;
		events.push_back (details);
	}

	if (systemConfig_old.bPartIDSwitchStatus != systemConfig_new.bPartIDSwitchStatus)
	{
		from = systemConfig_old.bPartIDSwitchStatus ? "ON:" : "OFF:";
		to = systemConfig_new.bPartIDSwitchStatus ? "ON:" : "OFF:";

		details = "Identifier:PART ID SWITCH:From:" + from + "To:" + to;
		events.push_back (details);
	}

	if (systemConfig_old.bPowerUpMem != systemConfig_new.bPowerUpMem)
	{
		from = systemConfig_old.bPowerUpMem ? "ON:" : "OFF:";
		to = systemConfig_new.bPowerUpMem ? "ON:" : "OFF:";

		details = "Identifier:POWERUP MEMORY:From:" + from + "To:" + to;
		events.push_back (details);
	}

	if (systemConfig_old.bResetMem != systemConfig_new.bResetMem)
	{
		from = systemConfig_old.bResetMem ? "YES:" : "NO:";
		to = systemConfig_new.bResetMem ? "YES:" : "NO:";

		details = "Identifier:RESET MEMORY:From:" + from + "To:" + to;
		events.push_back (details);
	}

	
	if (systemConfig_old.systemType != systemConfig_new.systemType)
	{
		if (systemConfig_old.systemType == 	GSX_E_SYSTEM ) from = "GSX E1:";
		else if(systemConfig_old.systemType == 	ASX_SYSTEM ) from = "ASX:";
		else from = "UNKNOWN:";

		if (systemConfig_new.systemType == 	GSX_E_SYSTEM ) to = "GSX E1:";
		else if(systemConfig_new.systemType == 	ASX_SYSTEM ) to = "ASX:";
		else to = "UNKNOWN:";
		
		details = "Identifier:SYSTEM TYPE:From:" + from + "To:" + to;
		events.push_back (details);
	}
	
	// send them to the event log
	for (idx = 0; idx < events.size(); idx++)
		DataBaseManager::EnterEventLog (SYSTEM_CONFIGURATION, events[idx]);
}


/*************************************************************************//**
* \brief   - Gets the Calibration time stamp 
*
* \param   - 
*
* \return  - void
*
******************************************************************************/
void UserInterface::UpdateCalibrationTimeStampToAC(string &CalTimeStamp)
{
	RTCtimerStr SysTime;
	strcpy(cal_temp_data, CalTimeStamp.c_str()); 
	sscanf(cal_temp_data,"%d/%02d/%02d %02d:%02d:%02d",&SysTime.tm_year,&SysTime.tm_mon,
			&SysTime.tm_mday,&SysTime.tm_hour,&SysTime.tm_min,&SysTime.tm_sec);
	
	m_ReadingSDO.tm_hour  = SysTime.tm_hour;
	m_ReadingSDO.tm_isdst = SysTime.tm_isdst;
	m_ReadingSDO.tm_mday  = SysTime.tm_mday;
	m_ReadingSDO.tm_min   = SysTime.tm_min;
	m_ReadingSDO.tm_mon	  = SysTime.tm_mon;	
	m_ReadingSDO.tm_sec   = SysTime.tm_sec;
	m_ReadingSDO.tm_wday  = SysTime.tm_wday;
	m_ReadingSDO.tm_yday  = SysTime.tm_yday;
	m_ReadingSDO.tm_year  = SysTime.tm_year; 
}

/*************************************************************************//**
* \brief   - Gets the input from UIC and handle 
*
* \param   - Message& data
*
* \return  - void
*
******************************************************************************/
void UserInterface::CalibrationHandling(Message& data)
{
	UINT32 msgLen = 0;
	
	memcpy(&m_CalibrationInput,data.Buffer,sizeof(m_CalibrationInput));
	ControlTask::SetCalibrationStep(m_CalibrationInput.CommandId);
	
	if(START_REQ == m_CalibrationInput.CommandId)
	{
		bIsFirstReadDone = true;
		LOGDBG("UI_T : Calibration START Cmd",0,0,0);
		data.msgID = TO_UI_TASK_CALIBRATION_CMD_REQ;
		SendMessageToControlTask(data);
	}
	else if(NEXT_REQ == m_CalibrationInput.CommandId)
	{
		bIsSecondReadDone = true;
		LOGDBG("UI_T : Calibration NEXT Cmd",0,0,0);
		m_ReadingSDO.FirstReading = m_CalibrationInput.FirstReading;
		data.msgID = TO_UI_TASK_CALIBRATION_CMD_REQ;
		SendMessageToControlTask(data);
		cout << "UI_T : First reading value : " << m_ReadingSDO.FirstReading << endl;
	}
	else if(FINISH_REQ == m_CalibrationInput.CommandId)
	{
		LOGDBG("UI_T : Calibration FINISH Cmd",0,0,0);
		m_ReadingSDO.SecondReading = m_CalibrationInput.SecondReading;
		GetDateAndTime(CalTimeStamp);
		UpdateCalibrationTimeStampToAC(CalTimeStamp);
		LOGDBG("UI_T : First reading value : %d",m_ReadingSDO.FirstReading,0,0);
		LOGDBG("UI_T : Second reading value : %d",m_ReadingSDO.SecondReading,0,0);
		
		memset(data.Buffer,0x00,sizeof(data.Buffer));
		data.msgID = TO_SDO_TASK_CALIBRATION_READ_REQ;					/* Sending read data to SDO task for sending to AC */
		memcpy(data.Buffer,&m_ReadingSDO,sizeof(m_ReadingSDO));
		SendMessageToSDOTask(data);
		
		memset(data.Buffer,0x00,sizeof(data.Buffer));
		data.msgID = TO_DB_TASK_CALIBRATION_ABORT_CMD_REQ;				/* Insert into DB */
		memcpy(data.Buffer,&m_CalibrationInput,sizeof(m_CalibrationInput));
		SendToMsgQ(data,DB_MSG_Q_ID);		
	}
	else if(ABORT_REQ == m_CalibrationInput.CommandId)
	{
		LOGDBG("UI_T : Calibration ABORT Cmd",0,0,0);
		
		if((!bIsSecondReadDone) && (bIsFirstReadDone))
		 {
				data.msgID = TO_UI_TASK_CALIBRATION_CMD_REQ;
		 		SendMessageToControlTask(data);		/* On abort, Triggers to second calibration and waits for complete */
		 }
		 else
		 {
		 		bIsSecondReadDone = false;
				bIsFirstReadDone = false;
		 }
		
		data.msgID = TO_DB_TASK_CALIBRATION_ABORT_CMD_REQ;				/* Read previous values from DB */
		SendToMsgQ(data,DB_MSG_Q_ID);
		
		memset(data.Buffer,0x00,sizeof(data.Buffer));
		data.msgID = UIC_CALIBRATION_CMD_RES;
		msgLen = ResponseMessage(RESPONSE_SUCCESS,data.Buffer);
		NotifyResponseToClient(data,msgLen);
	}
}

/*************************************************************************//**
* \brief   -  On power up, request for FW version data of slaves. 
*
* \param   -  UINT8 typeOfReq
*
* \return  -  None
*
******************************************************************************/
void UserInterface::PowerOnRequest(UINT8 typeOfReq)
{
	Message powerOnReq;
	powerOnReq.msgID = 0;
	memset(powerOnReq.Buffer, 0x00, sizeof(powerOnReq.Buffer));

	if(FW_VERSION == typeOfReq)
	{
		powerOnReq.msgID = TO_UI_TASK_VERSION_REQUEST;
		SendMessageToSDOTask(powerOnReq);
	}
	else if(POWER_UP_RECIPE == typeOfReq)
	{
		powerOnReq.msgID = TO_UI_TASK_POWER_UP_RECIPE;
		
		PowerUpRecipe.Power	= SysConfig.PcConfig.PSWatt;
		
		if(KHz_20 == SysConfig.PcConfig.PSFreq)
		{
			PowerUpRecipe.Frequency = FREQ_20KHz;
		}
		else if(KHz_30 == SysConfig.PcConfig.PSFreq)
		{
			PowerUpRecipe.Frequency = FREQ_30KHz;
		}
		else if(KHz_40 == SysConfig.PcConfig.PSFreq)
		{
			PowerUpRecipe.Frequency = FREQ_40KHz;
		}
		PowerUpRecipe.CheckSum	 = 123;
		PowerUpRecipe.SerialNo   = 1;
		PowerUpRecipe.SystemType = 1;
		LOGDBG("-----PowerUpRecipe.Power=%d  PowerUpRecipe.Frequency=%d  SysConfig.PcConfig.PSFreq=%d------",PowerUpRecipe.Power,PowerUpRecipe.Frequency,SysConfig.PcConfig.PSFreq); 
		
		memcpy(powerOnReq.Buffer,&PowerUpRecipe,sizeof(PowerUpRecipe));
		SendMessageToSDOTask(powerOnReq);
	}
	else if(POWER_ON_RECIPE_READ == typeOfReq)
	{
		powerOnReq.msgID = TO_DB_TASK_READ_RECIPE;
		SendToMsgQ(powerOnReq,DB_MSG_Q_ID); 
	}
	else if(CALIBRATION_TIME_STAMP == typeOfReq)
	{
		powerOnReq.msgID = TO_CALIBRATION_DATA_REQUEST;
		SendMessageToSDOTask(powerOnReq);
	}
}

/*************************************************************************//**
* \brief   -  Gets RTC date and time.
*
* \param   -  string DateTime 
*
* \return  -  None
*
******************************************************************************/
void UserInterface::GetDateAndTime(string &DateTime)
{
	RTCtimerStr SysTime;
	char temp[TIMESTAMP] = {0x00};
	GetRTC(&SysTime);
	
	SysTime.tm_year = 1900 + SysTime.tm_year;
	SysTime.tm_mon  = 1 + SysTime.tm_mon;
	sprintf(temp,"%d/%02d/%02d %02d:%02d:%02d",SysTime.tm_year,SysTime.tm_mon,SysTime.tm_mday,SysTime.tm_hour,SysTime.tm_min,SysTime.tm_sec);
	DateTime = temp;
}

/**************************************************************************//**
* 
* \brief   - Gets constant stored Branson key and send it to UIC. 
*
* \param   - Message& message.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::AccessBransonKey(Message& message)
{
	UINT32 msgLen = 0;
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	
	message.msgID = UIC_BRANSON_KEY_RES;
	sprintf(message.Buffer,"%s,%s",FIRST_LEVEL_KEY,SECOND_LEVEL_KEY);
	msgLen = strlen(message.Buffer);
	NotifyResponseToClient(message, msgLen);
	
#ifdef UI_RESPONSE_DEBUG
	printf("UI_T : BransonKey  	: %s\n",message.Buffer);
	printf("UI_T : Key Len 		: %d",msgLen);
#endif /* UI_RESPONSE_DEBUG */
}

/**************************************************************************//**
* 
* \brief   - Decodes the received message from UI_MSG_Q . 
*
* \param   - char *pRecvBuffer, Message& destBuffer.
*
* \return  - struct Message.
*
******************************************************************************/
void UserInterface::DeCoder(char *pRecvBuffer, Message& message) 
{
	memcpy(&message, pRecvBuffer, sizeof(message));
}

/**************************************************************************//**
* 
* \brief   - Interrupt the consoleApp for user menu options.  
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::TriggerConsoleTask()
{
	if(eventSend(Console_TID,CON_EVENT) != OK)
	{
		LOGERR("UI_T : Event failed to invoke console task",0,0,0);
	}
}

/**************************************************************************//**
* 
* \brief   - Retrieve the Control task's message queue ID.  
*
* \param   - None.
*
* \return  - MSG_Q_ID.
*
******************************************************************************/
MSG_Q_ID UserInterface::GetUIMsgQID() const
{
	return UI_MSG_Q_ID;
}

/**************************************************************************//**
* 
* \brief   - Get the task life time status.  
*
* \param   - None.
*
* \return  - bool.
*
******************************************************************************/
bool UserInterface::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}

/**************************************************************************//**
* 
* \brief   - Get the response status of SDO DOWNLOAD REQ. 
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::SdoResponseStatus(char *RespMsg)
{
	UINT8 pos  = 0;
	memcpy(&ErrCode, RespMsg + pos, sizeof(ErrCode));
	sprintf(RespMsg,"%d",ErrCode);
}

/*************************************************************************//**
* \brief   - Prepare response message with ANSCII string format for UIC
*
* \param   - UINT8 respMsg (RESPONSE_SUCCESS or RESPONSE_FAIL)
*
* \return  - UINT8 length of respMsg
*
******************************************************************************/
UINT8 UserInterface::ResponseMessage(UINT8 respMsg, char *pMsgBuffer)
{
	sprintf(pMsgBuffer, "%u", respMsg);
	return strlen(pMsgBuffer);
}

/**************************************************************************//**
* 
* \brief   - Get the response status of message sent to client over socket.
*
* \param   - Message& message, UINT16 buffLen.
*
* \return  - STATUS (fail -1 or success >=0)
*
******************************************************************************/
STATUS UserInterface::NotifyResponseToClient(Message& message, UINT32 buffLen)
{
	Client_Message sendBuffer;
	UINT32 iLen = 0;
	STATUS socketSendResponse = 0;
	memset(sendBuffer.Buffer,0x00,sizeof(sendBuffer.Buffer));
	
	sendBuffer.msgID 	= message.msgID;
	sendBuffer.msglen 	= buffLen;
	
	if(sendBuffer.msglen < (MAX_SIZE_OF_MSG_LENGTH - 8))
	{
		memcpy(sendBuffer.Buffer,message.Buffer,sendBuffer.msglen);
		iLen = sizeof(sendBuffer.msgID)+sizeof(sendBuffer.msglen)+sendBuffer.msglen;
		
		if(iLen >= MAX_SIZE_OF_MSG_LENGTH)
		{
			LOGCRIT("UI_T : Invalid message length : %d, msg_ID : %d",iLen, sendBuffer.msgID, 0);
		}
		else
		{
			socketSendResponse = Interface->Send(reinterpret_cast<char*>(&sendBuffer),iLen);
//			printf ("\n%sUInterface::NotifyResponseToClient: MsgID %d  ReqLen: %d SentLen: %d %s\n", (iLen == socketSendResponse ? KMAG:KRED), sendBuffer.msgID, iLen, socketSendResponse, KNRM);
		}
	}
	else
	{
		LOGCRIT("UI_T : Invalid message limit : %d, msg_ID : %d",sendBuffer.msglen, sendBuffer.msgID, 0);
	}
	
	/* Debug logs to ensure right FW Version is sent to UIC */
#ifdef UI_RESPONSE_DEBUG
	LOGDBG("UI_T : MSG ID : %d\n",sendBuffer.msgID,0,0);
	LOGDBG("UI_T : MSG LEN : %d\n",sendBuffer.msglen,0,0);
	
	if((message.msgID == UIC_ALARM_DATA_INDICATION) || (message.msgID == UIC_ACTIVE_ALARM_RES))
	{
		printf("UI_T : MSG alarm data: %s\n",sendBuffer.Buffer);
	}
	else if(message.msgID == UIC_SC_STATE_RES)
	{
		printf("UI_T : MSG SC state status: %s\n",sendBuffer.Buffer);
	}
	else if(message.msgID == UIC_TEST_PROGRESS_IND)
	{
		printf("UI_T : MSG Test signature data : %s\n",sendBuffer.Buffer);
	}
	
	else if( (message.msgID == UIC_DEFAULT_ALARM_CONFIG_RSP) || (message.msgID == UIC_ALARM_CONFIG_READ_RSP))
	{
		printf("UI_T : MSG Alarm Configuration: %s\n", sendBuffer.Buffer);
	}
	/*
	else if(message.msgID == UIC_WELD_PROGRESS_IND)
	{
		printf("UI_T : MSG Weld data -- Power : %s\n",sendBuffer.Buffer);
	}
	*/
	else
	{
		printf("UI_T : MSG : %d\n",sendBuffer.Buffer[0]);
	}
#endif
	return socketSendResponse;
}

/**************************************************************************//**
* 
* \brief   - Gets state of EtherCat master stack state.
*
* \param   - None.
*
* \return  - EC_MASTER_STATE (init, pre-op, safe-op or operational).
*
******************************************************************************/
EC_MASTER_STATE UserInterface::EcatGetMasterStateSysCall()
{
	INT32 iState = syscall(0, 0, 0, 0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,2));
	return (EC_MASTER_STATE)iState;
}

/**************************************************************************//**
* 
* \brief   - Gets number of retry count for Bus scan
*
* \param   - None.
*
* \return  - Bus scan timeout count
*
******************************************************************************/
UINT32 UserInterface :: EcatGetBusScanTimeoutSysCall()
{
	return syscall(0, 0, 0, 0 , 0, 0, 0, 0, SYSCALL_NUMBER(3,5));	 
}

/**************************************************************************//**
* 
* \brief   - Raise Bus scan failure hardware alarm.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface :: RaiseBusScanFailureAlarm()		//TODO: ALARM CODE
{
	Message  message;
	AlarmMgr alarmMgr;
	UINT8 	 resetReq;
	LOGDBG("Raising Bus scan failure alarm!!!", 0, 0, 0);
	((pALARM_INFO) message.Buffer)->alarmType = ALARM_BUS_SCAN_FAILURE_EFA & ALARMTYPE_MASK;
	((pALARM_INFO) message.Buffer)->source = ALARM_SC;
	((pALARM_INFO) message.Buffer)->id = ALARM_BUS_SCAN_FAILURE_EFA;
	((pALARM_INFO) message.Buffer)->priority = AlarmEvent::PRIORITY_HIGH;
	alarmMgr.GetAlarmActions(((pALARM_INFO) message.Buffer)->alarmType, ALARM_BUS_SCAN_FAILURE_EFA, ((pALARM_INFO) message.Buffer)->flags);	
	resetReq = ((pALARM_INFO) message.Buffer)->flags.ResetRequired;
		
	message.msgID = TO_DB_TASK_ALARM_DATA_SC;
	SendToMsgQ(message, DB_MSG_Q_ID);	
	message.msgID = TO_DB_TASK_ALARM_MGR_ALARM_DATA;
	/* Send data to the DB task to save in the database as well */
	if (((pALARM_INFO) message.Buffer)->flags.LogAlarm)
	{
		SendToMsgQ(message, DB_MSG_Q_ID);
	}
 
	memset(&message, 0x00, sizeof(message));
	message.msgID = UIC_ALARM_DATA_INDICATION;
	snprintf(message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d%X", resetReq, ALARM_BUS_SCAN_FAILURE_EFA);			
	NotifyResponseToClient(message, strlen(message.Buffer));
	}

			
/**************************************************************************//**
* 
* \brief   - Process the Delete DB response to set Weld/Scan flag
*
* \param   - Message &message
*
* \return  - None
*
******************************************************************************/
void UserInterface :: ProcessDeleteReportRes(Message &message)	
{
	bool	bIsIdValid = false;
	DeleteReport delRep;
	memcpy(&delRep, message.Buffer, sizeof(DeleteReport));
	switch(delRep.reportID)
	{
	case WELD_REP:
		if(delRep.status == 0)
		{			
			CP->SetMemStorageFullStatus(WELD_MEM_CB, false);
			CP->SetMemStorageFullStatus(DISK_MEM_FULL_CB, false);
			bIsIdValid = true;
		}
		break;
		
	case SCAN_REP:
		if(delRep.status == 0)
		{			
			CP->SetMemStorageFullStatus(SCAN_MEM_CB, false);
			CP->SetMemStorageFullStatus(DISK_MEM_FULL_CB, false);
			bIsIdValid = true;
		}
		break;
		
	default:		
		break;
	}	
		
	if(bIsIdValid)
	{
		CP->SetMemStorageFullStatus(ALARM_MEM_CB, false);
		CP->SetMemStorageFullStatus(EVENT_MEM_CB, false);
		memset(&message, 0x00, sizeof(Message));
		message.msgID = TO_CTRL_TASK_RESET_ALARM_CMD;	

		 if(msgQSend(CTRL_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		 {
			 LOGERR("UI_T : Message send failed to Ctrl_Task",0,0,0);
		 }
	}
}

/**************************************************************************//**
* 
* \brief   - Clears the memory offset of stack recipe on request base.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface::ClearMemoryOffset()
{
	UINT32 msgLen = 0, DigitalTuneValue = 0, NewStartFrequency = 0;
	Message message;
	
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	DigitalTuneValue = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
	
	//Reset MemoryOffset value
	CP->stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, 0);	
	CP->recipe["MemoryOffset"] = 0;	
	
	//Get NewStartFrequency value
	if (1 == CP->stackRecipe.GetData(StackRecipe::INTERNAL_OFFSET_FLAG))
	{
		 NewStartFrequency = DigitalTuneValue + CP->stackRecipe.GetData(StackRecipe::INTERNAL_FREQ_OFFSET);
	}
	else
	{
		 NewStartFrequency = DigitalTuneValue;
	}
	CP->recipe["StartFrequency"] = NewStartFrequency;	
	
	// As MemoryOffset=0 now, set Memory value to DigitalTune value
	CP->stackRecipe.SetData(StackRecipe::MEMORY, DigitalTuneValue);		
	
	// Update the above parameters in SDO Task to the active recipe and send the updated recipe to PC
	message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;
	SendToMsgQ(message, SDO_MSG_Q_ID);
	
	// Signaling UIC without waiting for SDO response message.
	message.msgID = UIC_MEMORY_OFFSET_RESET_RES;
	msgLen = ResponseMessage(RESPONSE_SUCCESS, message.Buffer);
	NotifyResponseToClient(message, msgLen);
}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
UserInterface::~UserInterface()
{
	CP = NULL;
}

/**************************************************************************//**
* 
* \brief   - Processing the User data.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UserInterface_Task()
{
	Message			ProcessBuffer;
	char			MsgQBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};	
	STATUS 			socketSendResp = SYSTEM_READY_IND_NOT_SENT;
	bool alarmRaised = false;
	UserInterface	*UI			= new(nothrow) UserInterface();
	
	if(NULL != UI)
	{
		MSG_Q_ID		UI_MSG_Q_ID	= UI->GetUIMsgQID();

		UI->PowerOnRequest(FW_VERSION);
		UI->PowerOnRequest(POWER_ON_RECIPE_READ);
		UI->PowerOnRequest(CALIBRATION_TIME_STAMP);
		Counters::loadCounters();

		/* UserInterface Task loop and the bIsTaskRun flag enabled when task created */
		while(UI->bIsTaskRunStatus())
		{
			EC_MASTER_STATE EcMasterState = UI->EcatGetMasterStateSysCall();
			/* check the state of the Ec Master before reading the MsgQ to send the SDO down load/Up load req	*/
			if(ecat_OP == EcMasterState)
			{
				if(msgQReceive(UI_MSG_Q_ID,MsgQBuffer,sizeof(MsgQBuffer),WAIT_FOREVER) != ERROR) 
				{
					UI->DeCoder(MsgQBuffer,ProcessBuffer);
					UI->ProcessMessage(ProcessBuffer);
				}
			}
			else
			{
				ProcessBuffer.msgID 	= UIC_UW_SYSREADY_IND;			
				//sprintf(ProcessBuffer.Buffer, "%d", RESPONSE_FAIL);
				//UI->NotifyResponseToClient(ProcessBuffer, sizeof(ProcessBuffer.Buffer[0]));
				taskDelay(ONE_SEC_DELAY);
				if(socketSendResp == SYSTEM_READY_IND_NOT_SENT)
				{
					sprintf(ProcessBuffer.Buffer, "%d", SCAN_IN_PROGRESS);
					socketSendResp = UI->NotifyResponseToClient(ProcessBuffer, sizeof(ProcessBuffer.Buffer[0]));					
				}
				else if( UI->EcatGetBusScanTimeoutSysCall() >= BUS_SCAN_TIMEOUT )
				{						
					/* Raise a Hardware failure alarm */
					if(false == alarmRaised)
					{
						sprintf(ProcessBuffer.Buffer, "%d", SCAN_FAILED);
						UI->NotifyResponseToClient(ProcessBuffer, sizeof(ProcessBuffer.Buffer[0]));
						alarmRaised = true;
						UI->RaiseBusScanFailureAlarm();						
					}									
				}				
			}
		}
		
		delete UI;
	}
	else
	{
		LOGCRIT("UI_T : ----------------Memory allocation failed----------------",0,0,0);
	}
	UI = NULL;	
	LOGDBG("UI_T : ----------------UserInterfaceTask Suspended----------------",0,0,0);	
	taskSuspend(taskIdSelf());
}



STATUS UserInterface::SendToMsgQ(Message& message, MSG_Q_ID msgQID, INT32 priority)
{
	STATUS retCode = OK;

	if(msgQSend(msgQID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,priority) != OK)
	{
		LOGERR("UI_T : Message send failed to MSG Q",0,0,0);
		retCode = ERROR;
	}
	
	return retCode;
}
/**************************************************************************//**
* \brief   - UserIO read
* 			 
* \param   - Message& message, MSG_Q_ID msgQID
*
* \return  - None
*
******************************************************************************/

void UserInterface::GetUserIO(Message& data)
{

	memcpy(&m_UserIO,data.Buffer,sizeof(USERIO));
	
//	LOGDBG("UI_T : ----------------POWERSUPPLY I/O READ----------------",0,0,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_01  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_01.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_01.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_02  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_02.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_02.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_03  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_03.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_03.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_04  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_04.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_04.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_11  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_11.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_11.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_12  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_12.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_12.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_13  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_13.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_13.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_16  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_16.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_16.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_23  - %d,%d",m_UserIO.PS_USERIO.PsDigitalInput.J116_23.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalInput.J116_23.Status,0);
//
//	
//	LOGDBG("m_UserIO.PS_USERIO.PsDigitalInput.J116_07  - %d ,%d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_07.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_07.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDgitalOutput.J116_08  - %d, %d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_08.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_08.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDgitalOutput.J116_09  - %d, %d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_09.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_09.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDgitalOutput.J116_10  - %d, %d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_10.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_10.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDgitalOutput.J116_19  - %d, %d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_19.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_19.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDgitalOutput.J116_20  - %d, %d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_20.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_20.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDgitalOutput.J116_21  - %d, %d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_21.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_21.Status,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsDgitalOutput.J116_22  - %d, %d",m_UserIO.PS_USERIO.PsDigitalOutput.J116_22.PS_EVENT,m_UserIO.PS_USERIO.PsDigitalOutput.J116_22.Status,0);
//	
//	LOGDBG("m_UserIO.PS_USERIO.PsAnalogInput.J116_17  - %d ",m_UserIO.PS_USERIO.PsAnalogInput.J116_17,0,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsAnalogInput.J116_18  - %d",m_UserIO.PS_USERIO.PsAnalogInput.J116_18,0,0);
//	
//	LOGDBG("m_UserIO.PS_USERIO.PsAnalogOutput.J116_24  - %d",m_UserIO.PS_USERIO.PsAnalogOutput.J116_24,0,0);
//	LOGDBG("m_UserIO.PS_USERIO.PsAnalogOutput.J116_25  - %d",m_UserIO.PS_USERIO.PsAnalogOutput.J116_25,0,0);
//		
//	LOGDBG("UI_T : ----------------ACTUATOR I/O READ----------------",0,0,0);
	
//	printf("m_UserIO.AC_USERIO.ACDigitalInput.J710_01  - %d, %d\n",m_UserIO.AC_USERIO.ACDigitalInput.J710_01.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalInput.J710_01.Status);
//	printf("m_UserIO.AC_USERIO.ACDigitalInput.J710_02  - %d, %d\n",m_UserIO.AC_USERIO.ACDigitalInput.J710_02.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalInput.J710_02.Status);
//	printf("m_UserIO.AC_USERIO.ACDigitalInput.J710_03  - %d, %d\n",m_UserIO.AC_USERIO.ACDigitalInput.J710_03.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalInput.J710_03.Status);
//	printf("m_UserIO.AC_USERIO.ACDigitalInput.J710_04  - %d,%d\n",m_UserIO.AC_USERIO.ACDigitalInput.J710_04.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalInput.J710_04.Status);	
//	printf("m_UserIO.AC_USERIO.ACDigitalInput.J710_05  - %d ,%d\n",m_UserIO.AC_USERIO.ACDigitalInput.J710_05.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalInput.J710_05.Status);

//	LOGDBG("m_UserIO.AC_USERIO.ACDgitalOutput.J710_06  - %d , %d",m_UserIO.AC_USERIO.ACDigitalOutput.J710_06.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalOutput.J710_06.Status,0);
//	LOGDBG("m_UserIO.AC_USERIO.ACDgitalOutput.J710_07  - %d, %d",m_UserIO.AC_USERIO.ACDigitalOutput.J710_07.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalOutput.J710_07.Status,0);
//	LOGDBG("m_UserIO.AC_USERIO.ACDgitalOutput.J710_08  - %d, %d",m_UserIO.AC_USERIO.ACDigitalOutput.J710_08.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalOutput.J710_08.Status,0);
//	LOGDBG("m_UserIO.AC_USERIO.ACDgitalOutput.J710_09  - %d, %d",m_UserIO.AC_USERIO.ACDigitalOutput.J710_09.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalOutput.J710_09.Status,0);
//	LOGDBG("m_UserIO.AC_USERIO.ACDgitalOutput.J710_10  - %d, %d",m_UserIO.AC_USERIO.ACDigitalOutput.J710_10.AC_EVENT,m_UserIO.AC_USERIO.ACDigitalOutput.J710_10.Status,0);
//	
//	data.msgID = TO_UI_TASK_USERIO_READ_RES;
//	sprintf(data.Buffer,"%d",RESPONSE_SUCCESS);
//	SendToMsgQ(data,UI_MSG_Q_ID);	 	
}

/**************************************************************************//**
* \brief   - Prints the WeldRecipeAC parameters
*
* \param   - WeldRecipeAC& weldRecipeAC
*
* \return  - None
*
******************************************************************************/
void UserInterface::PrintRecipe(WeldRecipePC& weldRecipePC)
{
	printf("UI_T : ---------PC WELD RECIPE-----------\n");
	printf("AmplitudeLoopC1		: %d\n",weldRecipePC.AmplitudeLoopC1);
	printf("AmplitudeLoopC2 		: %d\n",weldRecipePC.AmplitudeLoopC2);
	printf("PhaseLoop			: %d\n",weldRecipePC.PhaseLoop);
	printf("PhaseLoopCF			: %d\n",weldRecipePC.PhaseLoopCF);
	printf("FrequencyLow		: %d\n",weldRecipePC.FrequencyLow);
	printf("FrequencyHigh		: %d\n",weldRecipePC.FrequencyHigh);
	printf("PhaseLimitTime 		: %d\n",weldRecipePC.PhaseLimitTime);
	printf("PhaseLimit		: %d\n",weldRecipePC.PhaseLimit);
	printf("ControlMode		: %d\n",weldRecipePC.ControlMode);
	printf("BlindtimeSeek		: %d\n",weldRecipePC.BlindtimeSeek);
	printf("BlindtimeWeld		: %d\n",weldRecipePC.Blindtimeweld);
	printf("DDSSwitchtime		: %d\n",weldRecipePC.DDSSwitchtime);
	printf("WeldRampTime		: %d\n",weldRecipePC.WeldRampTime);
	printf("Parameter7		: %d\n",weldRecipePC.Parameter7);
	printf("Parameter8		: %d\n",weldRecipePC.Parameter8);
	printf("Parameter9		: %d\n",weldRecipePC.Parameter9);
	printf("Parameter10		: %d\n",weldRecipePC.Parameter10);
	printf("Parameter11		: %d\n",weldRecipePC.Parameter11);
	printf("Parameter12		: %d\n",weldRecipePC.Parameter12);
	printf("Parameter13		: %d\n",weldRecipePC.Parameter13);
	printf("Parameter14		: %d\n",weldRecipePC.Parameter14);
	printf("Parameter15		: %d\n",weldRecipePC.Parameter15);
	printf("F_LimitTime		: %d\n",weldRecipePC.FLimitTime);
	printf("AmpProportionalGain	: %d\n",weldRecipePC.AmpProportionalGain);
	printf("AmpIntegralGain		: %d\n",weldRecipePC.AmpIntegralGain);
	printf("PhaseProportionalGain	: %d\n",weldRecipePC.PhaseProportionalGain);
	printf("FrequencyWindowSize	: %d\n",weldRecipePC.FrequencyWindowSize);
	printf("PFofPhasecontrolloop	: %d\n",weldRecipePC.PFofPhasecontrolloop);
	printf("PIFrequencyLow		: %d\n",weldRecipePC.PIFrequencyLow);
	printf("PIFrequencyHigh		: %d\n",weldRecipePC.PIFrequencyHigh);
	printf("PIPhaseLimitTime		: %d\n",weldRecipePC.PIPhaseLimitTime);
	printf("PIPhaseLimit		: %d\n",weldRecipePC.PIPhaseLimit);
	printf("StartFrequency		: %d\n",weldRecipePC.StartFrequency);
	printf("Memory Offset       : %d\n",weldRecipePC.MemoryOffset);
	printf("Digital Tune       : %d\n",weldRecipePC.DigitalTune);
	printf("Frequency Offset       : %d\n",weldRecipePC.FrequencyOffset);
}

/**************************************************************************//**
* \brief   - Prints the WeldRecipeAcCompatibleWithAcBoard Parameters
*
* \param   - WeldRecipeAC& weldRecipeAC
*
* \return  - None
*
******************************************************************************/
void UserInterface::PrintRecipe(WeldRecipeAcCompatibleWithAcBoard& weldRecipeAC)
{
	printf("UI_T : ---------AC WELD RECIPE-----------\n");
	printf("WeldForce		: %d\n",weldRecipeAC.WeldForce);
	printf("ForceRampTime		: %d\n",weldRecipeAC.ForceRampTime);
	printf("HoldMode			: %d\n",weldRecipeAC.HoldMode);
	printf("TotalCollapseTarget	: %d\n",weldRecipeAC.TotalCollapseTarget);
	printf("HoldForce		: %d\n",weldRecipeAC.HoldForce);
	printf("HoldForceRampTime	: %d\n",weldRecipeAC.HoldForceRampTime);
	printf("ExpectedPartContactPosition : %d\n",weldRecipeAC.ExpectedPartContactPosition);
	printf("ReadyPosition		: %d\n",weldRecipeAC.ReadyPosition);
	printf("DownAcceleration		: %d\n",weldRecipeAC.DownAcceleration);
	printf("DownMaxVelocity		: %d\n",weldRecipeAC.DownMaxVelocity);
	printf("DownDeceleration		: %d\n",weldRecipeAC.DownDeceleration);
	printf("ReturnAcceleration	: %d\n",weldRecipeAC.ReturnAcceleration);
	printf("ReturnMaxVelocity	: %d\n",weldRecipeAC.ReturnMaxVelocity);
	printf("ReturnDeceleration	: %d\n",weldRecipeAC.ReturnDeceleration);
	printf("ExpectedPartContactOffset: %d\n",weldRecipeAC.ExpectedPartContactOffset);
	printf("PartContactWindowMinus: %d\n",weldRecipeAC.PartContactWindowMinus);
	printf("PartContactWindowPlus: %d\n",weldRecipeAC.PartContactWindowPlus);
	printf("NumForceSteps 		: %d\n",weldRecipeAC.NumForceSteps);
	
	for(int idx = 0; idx < weldRecipeAC.NumForceSteps; idx++)
	{
		printf("ForceStepForce[%d]	: %d\n",idx,weldRecipeAC.ForceStepForce[idx]);
		printf("ForceStepRampTime[%d] 	: %d\n",idx,weldRecipeAC.ForceStepRampTime[idx]);
	}
}

/**************************************************************************//**
* \brief   - Prints the WeldRecipePC parameters
*
* \param   - WeldRecipePC& weldRecipePC
*
* \return  - None
*
******************************************************************************/
void UserInterface::PrintRecipe(WeldRecipeAC& weldRecipeAC)
{
	printf("UI_T : ---------AC WELD RECIPE-----------\n");
	printf("WeldForce		: %d\n",weldRecipeAC.WeldForce);
	printf("ForceRampTime		: %d\n",weldRecipeAC.ForceRampTime);
	printf("HoldMode			: %d\n",weldRecipeAC.HoldMode);
	printf("TotalCollapseTarget	: %d\n",weldRecipeAC.TotalCollapseTarget);
	printf("HoldForce		: %d\n",weldRecipeAC.HoldForce);
	printf("HoldForceRampTime	: %d\n",weldRecipeAC.HoldForceRampTime);
	printf("ExpectedPartContactPosition : %d\n",weldRecipeAC.ExpectedPartContactPosition);
	printf("ReadyPosition		: %d\n",weldRecipeAC.ReadyPosition);
	printf("DownAcceleration		: %d\n",weldRecipeAC.DownAcceleration);
	printf("DownMaxVelocity		: %d\n",weldRecipeAC.DownMaxVelocity);
	printf("DownDeceleration		: %d\n",weldRecipeAC.DownDeceleration);
	printf("ReturnAcceleration	: %d\n",weldRecipeAC.ReturnAcceleration);
	printf("ReturnMaxVelocity	: %d\n",weldRecipeAC.ReturnMaxVelocity);
	printf("ReturnDeceleration	: %d\n",weldRecipeAC.ReturnDeceleration);
	printf("ExpectedPartContactOffset: %d\n",weldRecipeAC.ExpectedPartContactOffset);
	printf("PartContactWindowMinus: %d\n",weldRecipeAC.PartContactWindowMinus);
	printf("PartContactWindowPlus: %d\n",weldRecipeAC.PartContactWindowPlus);
	printf("NumForceSteps 		: %d\n",weldRecipeAC.NumForceSteps);
	
	for(int idx = 0; idx < weldRecipeAC.NumForceSteps; idx++)
	{
		printf("ForceStepForce[%d]	: %d\n",idx,weldRecipeAC.ForceStepForce[idx]);
		printf("ForceStepRampTime[%d] 	: %d\n",idx,weldRecipeAC.ForceStepRampTime[idx]);
	}
}

/**************************************************************************//**
* \brief   - Gets the FPGA version as a 32 bit integer (8 bits per version digit) 
* 			 and process it into a string.
*
* \param   - None
*
* \return  - string FPGAVersionStr: FPGA version as a string.
*
******************************************************************************/
std::string UserInterface::GetFPGAVersion()
{
    UINT32 FPGAVersionTemp = 0;
    std::string FPGAVersionStr;
    char FPGAVersionSplit[sizeof(FPGAVersionTemp) + 1] = {0};  //+1 for the end of string character
	
	//Get FPGA version
	FPGAVersionTemp = FWInfo::Get(FW_INFO_PC);
	
	//Cast the integer value into a C string type 
	std::copy(static_cast<const char*> (static_cast<const void*>(&FPGAVersionTemp)), static_cast<const char*> (static_cast<const void*>(&FPGAVersionTemp)) + sizeof(FPGAVersionTemp), FPGAVersionSplit);
	
	//Reverse the array excluding the end of string character
	reverse(FPGAVersionSplit, FPGAVersionSplit + sizeof(FPGAVersionSplit) - 1);
	
	for(int i = 0; i < FPGA_VERSION_DIGITS; i++)
	{
		FPGAVersionStr.append(CP->ConvertIntToString(FPGAVersionSplit[i]));
		
	    if(i < 3)
	    {
	    	FPGAVersionStr.append(".");
	    }
	}
	
	return FPGAVersionStr;
}
