/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef UIDBINTERFACE_H_
#define UIDBINTERFACE_H_

#include "SasSocket.h"
#include "Common.h"
#include "SQLiteDB.h"
#include "CommonProperty.h"
#include "DataBaseManager.h"
#include "RecipeZeroValidate.h"
#include "Recipe.h"
#include <ctime>

	enum CommunicationIDs
{
    REQ_WELD_RECIPE_LIST = 201,
    RES_WELD_RECIPE_LIST,

    REQ_GET_WELD_RECIPE_PARAM,
    RES_GET_WELD_RECIPE_PARAM,

    REQ_SET_WELD_RECIPE_PARAM,
    RES_SET_WELD_RECIPE_PARAM,

    REQ_GET_ACTIVE_RECIPE,
    RES_GET_ACTIVE_RECIPE,

    REQ_GET_PRODUCTION_WELD_RESULT,
    RES_GET_PRODUCTION_WELD_RESULT,

    REQ_GET_LAST_WELD_RESULT,
    RES_GET_LAST_WELD_RESULT,

    REQ_GET_LAST_WELD_PEAKPOWER,
    RES_GET_LAST_WELD_PEAKPOWER,

    REQ_GET_ALARM_LOG,
    RES_GET_ALARM_LOG,

    REQ_GET_DEFAULT_HORN_RECIPE,
    RES_GET_DEFAULT_HORN_RECIPE,

    REQ_GET_HORN_SIGNATURE_DATA,
    RES_GET_HORN_SIGNATURE_DATA,

    REQ_GET_HORN_SCAN_RESULTS,
    RES_GET_HORN_SCAN_RESULTS,

    REQ_GET_HORN_SCAN_HISTORY,
    RES_GET_HORN_SCAN_HISTORY,

    REQ_SET_HORN_SCAN_ID,
    RES_SET_HORN_SCAN_ID,

    REQ_GET_RECIPE_INFO,
    RES_GET_RECIPE_INFO,

    REQ_GET_RECIPE_SETUP_INFO,
    RES_GET_RECIPE_SETUP_INFO,

    REQ_SET_RECIPE_SETUP_INFO,
    RES_SET_RECIPE_SETUP_INFO,

    REQ_SET_WELD_RECIPE_SETUP_INFO,
    RES_SET_WELD_RECIPE_SETUP_INFO,

    REQ_USER_LOGIN,
    RES_USER_LOGIN,

    REQ_GET_ALL_USERS,
    RES_GET_ALL_USERS,

    REQ_SET_USER_INFO,
    RES_SET_USER_INFO,

    REQ_SET_ACTIVE_RECIPE,
    RES_SET_ACTIVE_RECIPE,

    REQ_GET_EVENTS_LOG,
    RES_GET_EVENTS_LOG,

    REQ_SET_EVENT_LOG,
    RES_SET_EVENT_LOG,

    REQ_GET_WELD_SIGNATURE_DATA,
    RES_GET_WELD_SIGNATURE_DATA,

    REQ_GET_WELD_TWH_DATA,
    RES_GET_WELD_TWH_DATA,

    REQ_GET_WELD_HISTORY_DATA,
    RES_GET_WELD_HISTORY_DATA,

    REQ_GET_LAST_WELD_HISTORY_DATA,
    RES_GET_LAST_WELD_HISTORY_DATA,

    REQ_GET_NEXT_WELD_HISTORY_DATA,
    RES_GET_NEXT_WELD_HISTORY_DATA,

    REQ_UPDATE_WELD_HISTORY_ID,
    RES_UPDATE_WELD_HISTORY_ID,

    REQ_GET_ALL_AUTHORITY_LEVEL,
    RES_GET_ALL_AUTHORITY_LEVEL,

    REQ_UPDATE_ALL_AUTHORITY_LEVEL,
    RES_UPDATE_ALL_AUTHORITY_LEVEL,

    REQ_GET_USER_PASSWORD,
    RES_GET_USER_PASSWORD,

    REQ_SET_USER_PASSWORD,
    RES_SET_USER_PASSWORD,

    REQ_GET_USER_DETAILS,
    RES_GET_USER_DETAILS,

    REQ_SET_GLOBAL_SETTINGS,
    RES_SET_GLOBAL_SETTINGS,

    REQ_GET_GLOBAL_SETTINGS,
    RES_GET_GLOBAL_SETTINGS,

    REQ_SET_HORN_RECIPE_DATA,
    RES_SET_HORN_RECIPE_DATA,

    REQ_SET_SEEK_SCAN_ID,
    RES_SET_SEEK_SCAN_ID,

    REQ_GET_SEEK_SCAN_HISTORY,
    RES_GET_SEEK_SCAN_HISTORY,

    REQ_GET_DEFAULT_SEEK_RECIPE,
    RES_GET_DEFAULT_SEEK_RECIPE,

    REQ_SET_SEEK_RECIPE_DATA,
    RES_SET_SEEK_RECIPE_DATA,

    REQ_GET_SEEK_SCAN_RESULTS,
    RES_GET_SEEK_SCAN_RESULTS,

    REQ_GET_SEEK_SIGNATURE_DATA,
    RES_GET_SEEK_SIGNATURE_DATA,

    REQ_SET_SUSPECT_REJECT_LIMITS_INFO,
    RES_SET_SUSPECT_REJECT_LIMITS_INFO,

    REQ_GET_SUSPECT_REJECT_LIMITS_INFO,
    RES_GET_SUSPECT_REJECT_LIMITS_INFO,
   
 	REQ_GET_TEST_MODE_DATA,
    RES_GET_TEST_MODE_DATA,

    REQ_GET_TEST_RECIPE_DATA,
    RES_GET_TEST_RECIPE_DATA,

    REQ_SET_TEST_RECIPE_DATA,
    RES_SET_TEST_RECIPE_DATA,

    REQ_GET_TEST_PROGRESS_INFO,
    RES_GET_TEST_PROGRESS_INFO,

    REQ_GET_DB_VERSION,
    RES_GET_DB_VERSION,

	REQ_GET_ADD_RECIPENUMBER,
    RES_SET_ADD_RECIPENUMBER,

	REQ_GET_COPY_RECIPENUMBER,
    RES_SET_COPY_RECIPENUMBER,

    REQ_SET_WELD_RECIPE_PARAM_FOR_NEW_RECIPE,
    RES_SET_WELD_RECIPE_PARAM_FOR_NEW_RECIPE,

    REQ_RESONANT_FREQUENCY,
    RES_RESONANT_FREQUENCY,

    REQ_SET_SUSPECT_REJECT_LIMITS_INFO_FOR_UPDATED,
    RES_SET_SUSPECT_REJECT_LIMITS_INFO_FOR_UPDATED,

    REQ_SET_RECIPE_SETUP_INFO_FOR_UPDATED,
    RES_SET_RECIPE_SETUP_INFO_FOR_UPDATED,

    REQ_GET_ACTUATOR_SETUP_PARAMS,
    RES_GET_ACTUATOR_SETUP_PARAMS,

    REQ_SET_ACTUATOR_SETUP_PARAMS,
    RES_SET_ACTUATOR_SETUP_PARAMS,

    REQ_GET_ANALYTICS_ALARM,
    RES_GET_ANALYTICS_ALARM,

    REQ_SET_ANALYTICS_ALARM,
    RES_SET_ANALYTICS_ALARM,

    REQ_GET_STACK_RECIPE_DATA,
    RES_GET_STACK_RECIPE_DATA,

    REQ_SET_STACK_RECIPE_DATA,
    RES_SET_STACK_RECIPE_DATA,

    REQ_SET_BRANSONONLY_PARAMS,
    RES_SET_BRANSONONLY_PARAMS,

    REQ_GET_BRANSONONLY_PARAMS,
    RES_GET_BRANSONONLY_PARAMS,

    REQ_SET_BRANSONONLY_PARAMS_UPDATED,
    RES_SET_BRANSONONLY_PARAMS_UPDATED,

    REQ_GET_ACTIVE_RECIPE_NUMBER,
    RES_GET_ACTIVE_RECIPE_NUMBER,

    REQ_GET_BATCH_COUNT,
    RES_GET_BATCH_COUNT,

    REQ_RESET_BATCH_COUNTER,
    RES_RESET_BATCH_COUNTER,

    REQ_GET_ALL_WELD_RESULT_DATA,
    RES_GET_ALL_WELD_RESULT_DATA,

    REQ_GET_LAST_ALL_WELD_RESULT_DATA,
    RES_GET_LAST_ALL_WELD_RESULT_DATA,

    REQ_GET_TRENDSGRAPH_SIGNATURE_DATA,
    RES_GET_TRENDSGRAPH_SIGNATURE_DATA,

    REQ_GET_SUSPECT_REJECT_LIMITS_TRENDSGRAPH,
    RES_GET_SUSPECT_REJECT_LIMITS_TRENDSGRAPH,

    REQ_SET_PRODUCT_DATE_PARAM,
    RES_SET_PRODUCT_DATE_PARAM,

    REQ_GET_USERIO_DATA,
    RES_GET_USERIO_DATA,

    REQ_SET_USERIO_DATA,
    RES_SET_USERIO_DATA,

    REQ_GET_SETUP_LIMITS,
    RES_GET_SETUP_LIMITS,

    REQ_SET_SETUP_LIMITS,
    RES_SET_SETUP_LIMITS,

    REQ_GET_ACTIVE_RECIPE_FOR_ACTUATOR,
    RES_GET_ACTIVE_RECIPE_FOR_ACTUATOR,

    REQ_SET_RECIPE_VALIDATE,
    RES_SET_RECIPE_VALIDATE,

    REQ_SET_RECIPE_BLOB,
    RES_SET_RECIPE_BLOB,

    REQ_GET_RECIPE_BLOB,
    RES_GET_RECIPE_BLOB,

    REQ_SET_WELDFORCE_FOR_ACTUATOR,
    RES_SET_WELDFORCE_FOR_ACTUATOR,

    REQ_SET_RECIPE_UNVALIDATE,
    RES_SET_RECIPE_UNVALIDATE,

    REQ_FACTORY_RESET,
    RES_FACTORY_RESET,

    REQ_UPDATE_SYSTEM_DATA,
    RES_UPDATE_SYSTEM_DATA,

    REQ_UPDATE_USER_LOGIN,
    RES_UPDATE_USER_LOGIN,
	
	SCBL_EMMC_WEAR_LEVEL_REQ,
	UIC_EMMC_WEAR_LEVEL_RESP,
	
	REQ_STOP_COLLECT_GRAPH_DATA,
	RES_STOP_COLLECT_GRAPH_DATA,
	
	SCBL_OPTIMIZE_DATABASE_REQ,
    UIC_OPTIMIZE_DATABASE_RESP,

    REQ_SAVE_WEBSERVICES_DATA,
    RES_SAVE_WEBSERVICES_DATA,

    REQ_GET_WEBSERVICES_DATA,
    RES_GET_WEBSERVICES_DATA,
	
    REQ_GET_WEBSERVICES_USER_AND_LEVEL,
    RES_GET_WEBSERVICES_USER_AND_LEVEL,
	
    REQ_DELETE_RECIPE,
    RES_DELETE_RECIPE,

	DUMMY
};

typedef enum 
{
	DELETE_RECIPE_SUCCESS=0,
	DELETE_RECIPE_INVALID_NUMBER,
	DELETE_RECIPE_ACTIVE_RECIPE,
	DELETE_RECIPE_FAILED,
	DELETE_RECIPE_NOT_PRESENT
}DELETE_RECIPE_RESPONSE;

typedef struct
{
    int msgID;
    std::string Buffer;
}DBResponseMessage;

typedef struct
{
    int recipePramMapID;
    int dataType;
}WeldParamDetails;

class DBInterface
{

public:
    DBInterface();
    ~DBInterface();
 
    void run();
    void ProcessRequest(Message& requestBuffer, DBResponseMessage& OutData);
    void Decode(char *buff, Message& ReqMessage);

    static void GetRecipeZeroStrings(std::string &ActiveWeldRecipe, std::string &SRActiveRecipe, std::string &BransonOnly, std::string &SetupLimits);
    static void SetRecipeZero(std::string ActiveWeldRecipe, std::string SRActiveRecipe, std::string BransonOnly, std::string SetupLimits);
    SasSocket m_socket;
    SQLiteDB m_db;
    CommonProperty *CP;

    static std::string 	m_WeldRecipeZero;
    static std::string 	m_SuspectRejectLimitsZero;
    static std::string 	m_BransonOnlyZero;
    static std::string 	m_SetupLimitsZero;
    static std::string 	m_StackRecipeZero;
    
private:
    void 		ConnectDB();
    void 		CloseDB();

    std::string GetRecipeZero();
    void 		LoadRecipeZero();
    void 		EraseRecipeZero();
    void 		WriteDefaultRecipeZero();
    void 		WriteRecipeZeroToFile();
    bool 		IsValidRecipeRecord (string recipeRecord);
    
	void 		GetDateAndTime(std::string &DateTime);
    STATUS 		EventLogComment (string& eventData);
    std::string GetWeldRecipeList(std::string strData);
    std::string GetWeldRecipeParams(std::string strData);
    bool 		ValidateSuspectRejectLimitsForASX(std::string strData);
    bool 		ValidateSetupLimit(std::string strData);
    bool        ValidateDecimalDigitNumbers(float chInputParamValue, int decimalNumLimit);
    bool   		ValidateRecipeForSetWeldRecipeParams(std::string strData);
    bool 		ValidateRecipeParam(std::string strData,int paramId ,int paramDataType);
    bool        ValidateRecipeParamName(std::string strData);
    bool 	    IsAlphaNumeric(const std::string str);
    std::string SetWeldRecipeParams(std::string strData);
    std::string setBransonOnlyParams(std::string strData);
    std::string getBransonOnlyParams(std::string strData);
    std::string setBransonOnlyParamsUpdated(std::string strData);
    std::string GetActiveRecipe(std::string strData);
    std::string GetProductionWeldResult(std::string strData);
    std::string GetLastWeldResult(std::string strData);
    std::string GetLastWeldPeakPower(std::string strData);
    std::string GetAlarmLog(std::string strData);
    std::string GetDafaultHornScanRecipe(std::string strData);
    std::string GetHornSignatureData(std::string strData);
    std::string GetHornScanHistory();
    std::string GetHornScanResult(std::string strData);
    std::string SetHornScanID(std::string strData);
    std::string GetRecipeInfo(std::string strData);
    std::string GetRecipeSetupInfo(std::string strData);
    std::string SetRecipeSetupInfo(std::string strData);
    std::string SetSuspectRejectLimits(std::string strData);
    std::string GetSuspectRejectLimits(std::string strData);
    std::string SetWeldRecipeSetupInfo(std::string strData);
    std::string UserLogin(std::string strData);
    std::string GetAllUsers();
    std::string SetUserInfo(std::string strData);
    std::string SetActiveRecipe(std::string strData);
    std::string GetEventLogs();
    std::string SetEventLogs(std::string strData);
    std::string GetWeldSignature(std::string strData);
    std::string GetWeldTWH(std::string strData);
    std::string GetWeldHistory(std::string strData);
    std::string GetLastWeldHistory(std::string strData);
    std::string GetNextWeldHistory(std::string strData);
    std::string GetAuthorityLevel(std::string strData);
    std::string UpdateAuthorityLevel(std::string strData);
    std::string GetUserPassword(std::string strData);
    std::string SetUserPassword(std::string strData);
    std::string GetUserDetails(std::string strData);
    std::string SetHornRecipeData(std::string strData);
    std::string SetGlobalSettings(std::string strData);
    std::string GetGlobalSettings(std::string strData);
    std::string SetSeekScanHistory();
    std::string GetSeekRecipe(std::string strData);
    std::string SetSeekRecipe(std::string strData);
    std::string GetSeekSignatureData();
    std::string GetTestModeGraphData(std::string strData);
    std::string GetTestModeRecipeData(std::string strData);
    std::string SetTestModeRecipeData(std::string strData);
    std::string GetTestModeResultData(std::string strData);
    std::string GetDBVersion();
    std::string GetResonantFrequency(std::string strData);
    std::string GetRecipeNumberFromDbForAddRecipe();
    std::string GetRecipeNumberFromDbForCopyRecipe(std::string strData);
    std::string SetWeldRecipeParamsForNewRecipe(std::string strData);
    std::string SetSuspectRejectForNewRecipe(std::string strData);
    std::string SetRecipeSetupInfoForNewRecipe(std::string strData);
    std::string GetActuatorSetupParams();
    std::string SetActuatorSetupParams(std::string);
    std::string GetAnalyticsAlarm(std::string strData);
    std::string setAnalyticsAlarm(std::string strData);
    std::string GetStackRecipe(std::string strData);
    std::string SetStackRecipe(std::string strData, bool save);
    std::string GetTrendsGraphSignatureData(std::string strData);
    std::string GetSuspectRejectLimitsForTrendsGraph(std::string strData);
    std::string getAllWeldResultData(std::string strData);
    std::string getLastAllWeldResultData(std::string strData);
    std::string GetAnalyticsProductionOverView(std::string strData);
	std::string GetActiveRecipeNumber(std::string strData);
    std::string GetBatchCount(std::string strData);
    std::string SetBatchCounter(std::string strData);
    std::string GetUserIO(std::string strData);
    std::string SetUserIO(std::string strData);
    std::string SetSetupLimits(std::string strData);
    std::string GetSetupLimits(std::string strData);
    std::string SetRecipeValidate(std::string strData);
    std::string SetRecipeUnValidate(std::string strData);
    std::string GetActiveRecipeForActuatorSetUp();
    std::string SetRecipeToBLOB(std::string strData);
    std::string GetRecipeToBLOB(std::string strData);
    std::string SetWeldForceForActuatorSetUp(std::string strData);
    std::string UpdateSystemData(std::string strData);
    std::string SetFactoryReset(std::string strData);
    std::string UpdateUserLogin(std::string strData);
    void 		getTimedifferenct(struct tm FromTime, struct tm ToTime, struct tm *diff);
    STATUS 		LogSuspectRejectLimits (std::string limitsData);
    STATUS 		LogSetupLimits (std::string limitsData);
    vector<string>split (string s, string delimiter);
	
    std::string SaveWebServicesStatus	(std::string strData);
    std::string GetServicesStatus		();
    std::string GetWSUserAndLevel 		();
    void 		SetDefaultRecipeZero	();
    std::string DeleteRecipeHMI(std::string strData); 
private:
    std::string	RetryMechanism			(char *sqlQuery);
    STATUS 		SendToMsgQ 				(Message& msgBuffer, const MSG_Q_ID &msgQID, _Vx_ticks_t waitType);    
    RECIPE_STATUS GetActiveRecipeStatus	();
    
    void		 	UpdateRecipeZeroWithActiveRecipe();   
    void           	PrepareValidatedRecipezeroFiles();
    std::string 	m_DefaultWeldRecipeZero;
	std::string 	m_DefaultSuspectRejectLimitsZero;
	std::string 	m_DefaultBransonOnlyZero;
	std::string 	m_DefaultSetupLimitsZero;
	std::string 	m_DefaultStackRecipeZero;
	 
    std::string 		m_AllUsers;
    std::string			m_lastRecipeNum;
    vector<string>		m_GetWeldRecipeParam;
    vector<string>		m_SetWeldRecipeParam;
    std::string			m_RecipeNumber;
    bool 				m_IsGetWeldRecipeParam;
    bool 				m_IsGetAddRecipeNumber;
    bool 				m_Save;
	UINT32 				m_RevisionNum;
	RecipeZeroValidate  m_RecipeZerodb;
	Recipe 				*m_RecipeParamValidation;

};

#endif /* UIDBINTERFACE_H_ */
