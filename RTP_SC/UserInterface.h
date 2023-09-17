/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#define SEND_POWERUP_RECIPE_TO_SLAVES

#include "Common.h"
#include "CommonProperty.h"
#include "WeldRecipe.h"
#include "CommunicateInterface.h"
#include "SysConfigEeprom.hpp"
#include "DataBaseManager.h"

#define VALID_STATE 	1
#define CONTINUE_CMD 	1
#define BRASON_KEY_SIZE	8


//#define UI_RESPONSE_DEBUG

#define SYSTEM_READY_IND_NOT_SENT	-1
#define BUS_SCAN_TIMEOUT 			10
#define FIRST_LEVEL_KEY		"BRNBuiltMe"
#define SECOND_LEVEL_KEY	"BMXBuiltMe"

/* firmware upgrade eeprom variables */

#define   FW_UPGRADE_SUCCESS         0x11
#define   FW_UPGRADE_RESET			 0x00

#define   FPGA_VERSION_DIGITS		 4

typedef struct Calibration
{	
	UINT8	CommandId;
	UINT32  FirstReading;
	UINT32  SecondReading;
}Calibration;

enum BUS_SCAN_STATUS
{
	SCAN_IN_PROGRESS = 2,
	SCAN_FAILED
}BUS_SCAN_STATUS;


enum CALIBRATION_CMD
{
	START_REQ = 1,
	NEXT_REQ,
	FINISH_REQ,
	ABORT_REQ
}CALIBRATION_CMD;

enum POWER_ON_SEQ
{
	FW_VERSION = 1,
	POWER_UP_RECIPE,
	POWER_ON_RECIPE_READ,
	CALIBRATION_TIME_STAMP
}POWER_ON_SEQ;


/*Changed by MOUNIKA*/
typedef struct 
{
	INT32	psLifeCounter;
	INT32   actuatorlifecounter;
	INT32   generalAlarmCounter;
	INT32   overloadAlarmCounter;
	INT32	actuatorOverloads;
	INT32	actuatorStrokeLength;
	UINT16  psFrequency;
	INT32   psWatt;
	UINT8   calibrationStatus;
	UINT8   psType;
	UINT8   actuatorType;	
	char    modelName[SYSINFO_SIZE];
	char    version_SC[SYSINFO_SIZE];
	char    version_PC[SYSINFO_SIZE];
	char    version_AC[SYSINFO_SIZE];
	char 	pc_FPGA_Version[SYSINFO_SIZE];
	char    psAssemblyNumber[SYSINFO_SIZE];
	char    actuatorAssembly[SYSINFO_SIZE];
	char    stackAssembly[SYSINFO_SIZE];
	char    psMacID[MAC_ADDR_SIZE];
	char    psIP[SYSINFO_SIZE];
	char    dateTime[2*SYSINFO_SIZE];
	UINT16  crc_SC;
	UINT16  crc_PC;
	UINT16  crc_AC;
}SystemInformation;

class UserInterface
{	
public:
	UserInterface();
	~UserInterface();

	bool			bIsTaskRunStatus();
	MSG_Q_ID		GetUIMsgQID() const;
	void			TriggerConsoleTask();
	EC_MASTER_STATE EcatGetMasterStateSysCall();
	void			PowerOnRequest(UINT8 typeOfReq);
	void			ProcessMessage(Message& message);	    		
	void			GetDateAndTime(string &DateTime);
	void			SdoResponseStatus(char *RespMsg);
	void			DeCoder(char *pRecvBuffer, Message& ProcessBuffer);
	STATUS			NotifyResponseToClient(Message& message, UINT32 msgLen);
	UINT32 			EcatGetBusScanTimeoutSysCall();
	void			RaiseBusScanFailureAlarm();
	void 			UpdateFirmwareUpgradeEvent(char *, char *, char *);
	
private:
	void			UpdateActiveRecipe();
	void			SetPCWeldRecipeWithMinValue();
	void			SetPCWeldRecipeWithMaxValue();
	void			SetACWeldRecipeWithMinValue();
	void			SetACWeldRecipeWithMaxValue();
	void			CalibrationHandling(Message& data);
	bool			GetActiveAlarmData(char *pAlarmData);
	void			SendMessageToSDOTask(Message& message);
	void			SendMessageToControlTask(Message& message);
	bool			ValidateRecipe(WeldRecipeAC& recipe);
	bool			ValidateRecipe(WeldRecipePC& recipe);
	//bool			ValidateRecipe(ScanRecipe& recipe);
	void			UpdateSeekRecipe(Message& a_seekRecipe);
	void			UpdateScanRecipe(Message& a_scanRecipe);
	void			UpdateTestRecipe(Message& a_testRecipe);
	UINT16			GetSystemModel(char *psystemModelData);
	
	STATUS 			SetSystemModel(char *pRunTimeData);
	bool 			FactoryReset();
	void 			GetDefaultRTConfig(char *pRunTimeData);

    void 			UpdateRecipeSC(Message& activeRecipe);
    void            UpdateRecipePcAc(Message& activeRecipe);
	UINT16 			AccessSystemInformation(char *pSysInfData);
	INT32			ActiveAlarmData(char *pAlarmData);
	UINT8			ResponseMessage(UINT8 respMsg, char *pMsgBuffer);
	UINT16			AccessIPConfiguration(UINT8 accessType, char *pIPConfigData);
	UINT16 			AccessSystemConfiguration(UINT8 accessType, char *pSysConfigData);
	void			LogSystemConfiguration (intf_SysConfig_t& _systemConfig_old, intf_SysConfig_t& _systemConfig_new);
	UINT16			AccessDateAndTime(UINT8 accessType, char *pRtcConfigData);
	void			GetUserIO(Message& data);
	void			UpdateAlarmPendingList();
	bool			ValidateAlarmReq(char *alarmData);
	void 			SendMessageToDBTask(Message& message);
	STATUS 			SendToMsgQ(Message& message,MSG_Q_ID msgQID, INT32 priority = MSG_PRI_NORMAL);
	UINT8           UpdateStackRecipe(Message& stackRecipe);
	void 			DecodeBarcodeData(Message &barcodeMessage);
	void			CompareRecipeNumber(INT32 recipeNumber);
	void			AccessBransonKey(Message& message);
	void			PrintRecipe(WeldRecipePC& weldRecipePC);
	void			PrintRecipe(WeldRecipeAC& weldRecipeAC);
	void            PrintRecipe(WeldRecipeAcCompatibleWithAcBoard& weldRecipeAC);
	void			ProcessDeleteReportRes(Message &message);
	void			ClearMemoryOffset();
	void			UpdateCalibrationTimeStampToAC(string &CalTimeStamp);
	bool            ActuatorSetupAdvFeatureParamConv();
    std::string 	GetFPGAVersion();
	
private:
    MSG_Q_ID		CTRL_MSG_Q_ID, SDO_MSG_Q_ID, UI_MSG_Q_ID, DB_MSG_Q_ID,UIDB_MSG_Q_ID;
    bool			bIsPowerOnSeek, bIsFirstReadDone,bIsSecondReadDone,m_bIsActiveRecipe,bIsPendingRecipe;
    UINT8 			ErrCode;
    UINT32			m_alarmID;
    TASK_ID			Console_TID;
	WeldRecipePC   	RecipePC, WeldMinValPC, WeldMaxValPC;
	WeldRecipeAC   	RecipeAC, WeldMinValAC, WeldMaxValAC;
	WeldRecipeAcCompatibleWithAcBoard RecipeAcCompaWithAcBoard;
	WeldRecipeSC	ActiveRecipeSC;
	Calibration		m_CalibrationInput;
	CalibrationReadings m_ReadingSDO;
	char			*m_ActiveAlarm;
	ActiveRecipe 	m_SC;
	PcWeldRecipe 	m_PC;
	AcWeldRecipe 	m_AC;
	CommonProperty	*CP;
	USERIO  		m_UserIO;			
	CommunicateInterface *Interface;
	intf_SysConfig_t SysConfig;
	PowerUpData		PowerUpRecipe;
	DBFileHandler m_objDBFileHandler;
	
	SystemConfiguration sysConf;
	intf_SysConfig_t 	sysConfig;	
	char 				m_pBarcodeData[BARCODE_DATA_SIZE];
	string CalTimeStamp;
	char cal_temp_data[TIMESTAMP];
};

#endif /* USERINTERFACE_H_ */
