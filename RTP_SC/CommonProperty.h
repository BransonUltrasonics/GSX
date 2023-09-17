/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef COMMONPROPERTY_H_
#define COMMONPROPERTY_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>

#include "Common.h"
#include "SeekResults.h"
#include "WeldResults.h"
#include "StackRecipe.h"
#include "TestResults.h"
#include "HornScanResults.h"
#include "AlarmData.h"
#include "ActiveRecipe.h"
#include "ControlTaskCommon.h"
#include "BarcodeReader.h"
#include "SysConfigEeprom.hpp"
#include "SeekRecipe.h"
#include "HornScanRecipe.h"
#include "TestRecipe.h"

#define TOTAL_NUM_OF_TASK 			17
#define NO_WS_USER					"WS_N/A"
#define NO_WS_LEVEL					0

#define GETPACKET_TIMEOUT		10
#define PUTPACKET_TIMEOUT		10
#define TRANSFERPACKET_TIMEOUT	10

#define STATUS_MESSAGES_LENGTH 100

using namespace std;

/* enum for Task Indices of Task names  */
enum taskIndex
{
	CTRL_T 		= 0,
	SDO_T,
	UI_T,
	DB_T,
	UI_DB_T,
	ALARM_T,
	BARCODE_READER_T,
	DGTIN_T,
	DGTOUT_T,
	WEBSERVICE_T,
	MAINTENANCE_T,    
	IO_IMAGE_HANDLER_T,
	MAIL_BOX_HANDLER_T,
	CONSOLE_T,
	MAIN_T,
	SOCKSERV_T,
	FWUPGRADE_T
};

/* Constant array for task names  */
const char* cTaskName[TOTAL_NUM_OF_TASK];


typedef enum CIRCULAR_DB_LIMIT  
{
	WELD_SIGNATURE_LIM_CB	= 1,
	WELD_RESULT_LIM_CB,
	SCAN_SIGNATURE_LIM_CB,
	SCAN_RESULT_LIM_CB,
	ALARMS_LIM_CB,
	EVENTS_LIM_CB,
	RESET_ALL_LIM_CB
}CIRCULAR_DB_LIMIT;

typedef enum CIRCULAR_DB_MEM  
{
	WELD_MEM_CB = 1,
	SCAN_MEM_CB,
	DISK_MEM_FULL_CB,
	ALARM_MEM_CB,
	EVENT_MEM_CB,
	RESET_MEM_FULL_CB
}CIRCULAR_DB_MEM;



typedef struct StorageFull
{
	bool			m_IsWeldMemStorageFull;
	bool			m_IsScanMemStorageFull;
	bool 			m_IsDiskStorageFull;
	bool			m_IsAlarmMemStorageFull;
	bool			m_IsEventMemStorageFull;
}StorageFull;




class CommonProperty
{
public:						/* Public member functions */
	static CommonProperty*	getInstance();
	static void 			DeleteInstance();
	void					CopyRecipe(UINT32 RECIPE);
	void					StoreRecipe(UINT32 RECIPE);
	TASK_ID					GetTaskId(string TaskName);
	MSG_Q_ID				GetMsgQId(string TaskName);
	bool					IsFileExist(string fileName);
	void					DisplayRecipe(UINT32 TypeOfRecipe);
	void					SetTaskId(string TaskName, TASK_ID tid);
	void					SetMsgQId(string TaskName, MSG_Q_ID mid);
	void					ReadWriteRecipe(UINT32 RECIPE, UINT32 OP);
	void					SendToMsgQ(Message& DestMsg, MSG_Q_ID& MsgQID);

	ActiveRecipe&			getActiveRecipeInstance();
	PcWeldRecipe&			getPcWeldRecipeInstance();
	AcWeldRecipe&			getAcWeldRecipeInstance();
	void					UpdateActiveRecipe(const ActiveRecipe& srcObj);
	void					UpdatePcWeldRecipe(const PcWeldRecipe& srcObj);
	void					UpdateAcWeldRecipe(const AcWeldRecipe& srcObj);

	bool 					IsMemStorageFull(CIRCULAR_DB_MEM idx) const;
	void 					SetMemStorageFullStatus(CIRCULAR_DB_MEM idx, bool storageStatus = false);
	void 					GetSysConfContext(intf_SysConfig_t  &sysConfig);
	void					SetSysConfContext(intf_SysConfig_t  *pSysConfig);
	bool 					getIsTimedSeekEnabledForEvent();
	void 					setIsTimedSeekEnabledForEvent(bool bTimedSeekEvent);
	int 					StringToTokens(const char *pData, vector<string>&vTokens, char delimiter);
	int 					TokensToString(string &strData, vector<string>&vTokens, char delimiter);
	std::string  			ConvertIntToString(UINT16 iNumber);
public:						/* Public member data */
	map<string,INT32> 		recipe,VersionSC_minor,VersionSC_build,VersionSC_major,TimerIntervalRecipe;
	map<string,INT32>		dispFlag,dispFlagPowerVersionSC_major,dispFlagScanRes,dispFlagTmrInterval;
	bool					bIsTaskRun;
	PowerUpData				powerupRecipe;

	// static members for global access
	static WeldResults 		weldResult;
	static SeekResults		seekResult;
	StackRecipe             stackRecipe;
	HornScanResults			ScanResult;
	TestResults				testResult;
	ScanSignature			*Signature;
	SeekSignature			*seekSignature;
	UINT32					NoOfSamples;		
	UINT32					TotalTime;
	TestSignature			*testSignature;
	UINT32                  DistanceStart; 
	UINT8                   MemClearBeforeSeek;
	UINT8                   SetDigTuneWithHornScan;
	void                    UpdateStartUPDigitalTune(string strStackRecipe);
	void                    ClearMemoryFrequency();
	void                    UpdateMemoryOffset();
	void                    SetDigitalTune(INT32 DigitalTune);
	void                    SetFrequencyOffset(INT32 FreqOffset);
	void 					SetSystemFrequency();
	std::vector<UINT16>     SuspectLimitAlarmList;
	std::vector<UINT16>     RejectLimitAlarmList;
	
	AlarmDataSC				m_ScAlarmObj;
	AlarmDataPC				m_PcAlarmObj;
	AlarmDataAC				m_AcAlarmObj;
	
	ActiveRecipe			m_RecipeSC;
	PcWeldRecipe			m_RecipePC;
	AcWeldRecipe			m_RecipeAC;
	
	SeekRecipe				m_SeekRecipeObj;
	HornScanRecipe			m_ScanRecipeObj;
	TestRecipe				m_TestRecipeObj;
	
	deque<WeldSignature>	m_qWeldSignature;
	UINT32					m_TimeCounter;
	BarcodeReader 			barcodeReader;
	
	/*Members defined for Shutdown feature*/
	UINT8					ShutdownCriticalDataFlag;
	WeldResult				ShutdownWeldResult;
	bool                    ind_CollectGraphData;   
	bool                    ind_ULGraphData;
	bool					ind_RTC_LowBatt;
	bool                    req_OptimizeDB;   
	
	bool					bIsHMILogged;
	bool					bIsWebServicesLogged;
	bool					bIsUIConnected;
	bool 					bIsActiveRecipeAvailble;
	bool					bIsWSLogAttempt;
	UINT8					WSUserLevelStatus;
	string					WSLoggedUserName;
private:					/* Private member functions */
	CommonProperty();
	CommonProperty(const CommonProperty& );
	CommonProperty& operator=(const CommonProperty& );
	~CommonProperty();
		
private:					/* Private member data */
	map<string,MSG_Q_ID>	MsgQMap;
	map<string,TASK_ID>		TaskIdMap;
	string 					RecipeBuffer;
	static CommonProperty	*m_CPObj;	
	StorageFull				storageFull;
	intf_SysConfig_t		m_SysConfig;
	
	/*Member defined for Timed seek event log*/ 
	bool 					bIsTimedSeekEnabledForEvent;
};

#endif /* COMMONPROPERTY_H_ */
