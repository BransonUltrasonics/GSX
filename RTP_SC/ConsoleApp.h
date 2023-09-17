/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef CONSOLEAPP_H_
#define CONSOLEAPP_H_

#include "Common.h"
#include "WeldRecipe.h"
#include "CommonProperty.h"
#include "versions.h"
#include <sstream>
#include <istream>
#include "SQLiteDB.h"



/* User menu options available in ConsoleApp */
enum USER_MENU
{
	/* More user options */
	WELD_OP					=	1,
	SEEK_OP,
	UPDATE_WELD_RECIPE,
	OP_FIND_PART,
	OP_HORN_DOWN,
	OP_JOG_UP,
	OP_JOG_DOWN,
	RETRACT,
	HORN_SCAN,
	SYS_CONF,
	FW_INFO,
	CALIBRATION_OP,
	TMR_INTERVAL_PC_OP,
	RUN_TEST,
	COPY_DB_EMMC_USB, /* Copy DB from emmc -> USB */
	DELETE_DB_EMMC, 	 /* Delete DB from emmc			 */
	CHECK_DB_SIZE,
	OPTIMIZE_DB,
	SHUTDOWN,
	
	STOP_TEST,
	TEST_FUNC,

	/* Power on setup */
	SETUP_INIT				=	1,
	EXIT_INIT,

	/* For alarm screen */
	ALARM_OP				=	1,
	UPDATE_WELD_RECIPE_INIT,

	/* Update recipe menu screen */
	SAVE					=	1,
	EDIT					=	1,
	CANCEL					=	2
};

TASK_ID 	CReadtid;

class ConsoleApp
{
public:
	ConsoleApp();
	~ConsoleApp();
	
	bool				bIsTaskRunStatus();
	void				ProcessUserOptions();
	void 				SendEvent(TASK_ID& tid, INT32 event);
	void 				SetTaskId(string T_Name, TASK_ID TID);
	void 				OnSysPowerUp();
	
public:
	static bool			bIsScanStart;
	static bool			bIsTestStart;	

private:
	INT32 				IsUserEdit();
	INT32 				ConsoleRead();
	void				ResetAlarm();
	void 				SetWeldRecipe();
	void 				SetVersionInfo();
	void 				SetTimerInterval(); 	
	void 				SetPowerUpRecipe();
	void				DisplaySystemConfiguration();
	void 				SaveWeldRecipe(UINT32 TypeOfRecipe);
	void 				GetInputFromUser(UINT32 TypeOfRecipe);
	INT32				IsUserMode(UINT32 TypeOfRecipe, string key);
	void 				SendMsgToUITask(MSG_Q_ID &MSG_ID, Message &Msg);
		
private:
	void				SetScWeldRecipe();
	void				SetPcWeldRecipe();
	void				SetAcWeldRecipe();
	
	void				SetForceStep();
	void				SetAmplitudeStep();
	void				SetAfterBurst();
	void				SetPreTrigger();
	void				SetControlLimits();
	void				SetRejectLimits();
	void				SetSuspectLimits();
	void				CalibrationRoutine();
	void                SetCutoffLimits();
	void				SetEnergyBrake();
	void				OptimizeDB();				
	
	// *************** active weld recipe from DB
	UINT32 		activeRecipeID;
	SQLiteDB	m_DbConn;
	UINT32		ConnectDB					();
    void 		CloseDB						();
    INT32		ObtainActiveRecipeNumber 	();	
    INT32 		PullActiveRecipeFromDB 		(INT32 activeRecipeID);
    INT32 		PullStackRecipeFromDB 		();
    void 		PullActiveRecipeFromCP 		();
    INT32 		CopyActiveRecipe			(string& activeRecipe);
    INT32 		CopySRActiveRecipe			(WeldRecipeSC& ScWeldRecipe, string& SRActiveRecipe);
    INT32 		ParseStackRecipe			(string& stackRecipe, INT32& endofWeldStore, INT32& internalOffsetFlag, INT32& digitalTune, INT32& internalFreqOffset);
    INT32 		LoadStackRecipe				(INT32 endofWeldStore, INT32 internalOffsetFlag, INT32 digitalTune, INT32 internalFreqOffset);
		
private:
	MSG_Q_ID			UI_MSG_Q_ID,SDO_MSG_Q_ID,DB_MSG_Q_ID;
	TASK_ID				Console_TID,Main_TID,CTRL_TID;
	INT32 				iChoice,iCmd;
	INT32				triggerDistance,triggerForce;				
	INT32 				seektime,holdtime;
	INT16 				inputPercentageWeldAmplitude; 	
	bool 				bIsSetup,bIsValidOp,bIsRecipeOp,bIsPowerUp,PowerupSetup;
	INT32 				triggerDistance_temp;
	INT32 				triggerForce_temp;
	WELDMODES			m_weldmode,m_weldmode_temp;
	INT16 				inputPercentageWeldAmplitude_temp;
	UINT32				m_modeValue,m_modeValue_temp;
	UINT32 				TempFreq;
	UINT32 				TempPower;	 
	UINT32				TempTmrIntervalPC;	
	Message 			message;
	
	// weld recipes
	WeldRecipeSC		Console_WeldRecipeSC;
	WeldRecipePC		Console_WeldRecipePC;
	WeldRecipeAC 		Console_WeldRecipeAC;
	WeldRecipe	 		Console_WeldRecipe;
	
	PowerUpData			Console_PowerUpRecipe;
	CalibrationReadings Console_CalibrationReadings;
	FirmwareVersionACPC Console_VersionInfo;
	TimerInterval 		Console_TimerInterval;	
	
	CommonProperty		*CP;
};


#endif /* CONSOLEAPP_H_ */
