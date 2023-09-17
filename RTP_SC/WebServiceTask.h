/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ***************************************************************************/

#ifndef WEBSERVICETASK_H_
#define WEBSERVICETASK_H_

#include <map>
#include "Common.h"
#include "CommonProperty.h"
#include "SC_Common.h"
#include "SQLiteDB.h" 
#include "SysConfigEeprom.hpp"
#include "CommunicateInterface.h"
#include "UIDBInterface.h"
#include "Recipe.h"
#include "AuthenticationKey.h"
#include "Utils.h"

#define USERSTATUS_ENABLED				"Enabled"
#define USERSTATUS_DISABLED				"Disabled"
#define PART_ID_INDEX					23
#define JSONFORMAT_USERDETAILS_LENGTH	13
#define USER_STATUS                		2
#define BUFFER_LENGTH				 	100
#define	VERSIONBUFFER_LENGTH       		512
#define WELDBUFFER_LENGTH          		512
#define EMPTYTOKEN                  	"EmptyToken"
#define SCVERSION                  		"Supervisory Controller"
#define PCVERSION                  		"Power Controller"
#define ACVERSION                  		"Actuator Controller"
#define UIVERSION                  		"HMI Controller"
#define EXECUTIVE						"Executive"
#define SUPERVISOR						"Supervisor"
#define TECHNICIAN						"Technician"
#define OPERATOR						"Operator"
#define SID                       		"\"Sid\":12345"
#define SET_MULTIPLE_PARAMS				"\"RecipeValues\":["
#define GET_MULTIPLE_PARAMS				"\"RecipeParameters\":["
#define LOG_IN   						"1"
#define LOG_OUT  						"0"

#define LOGIN_ALLOWED  1
#define LOGIN_DENIED   0

#define WS_MAX_GET_RECIPE_VALUE_PARAM_DATA_LENGTH		16 		// Length of paramid string plus id length*/
#define WS_MAX_SET_RECIPE_VALUE_PARAM_DATA_LENGTH		36 		// Length of paramid string plus id length and for value also*/
#define WS_MAX_GET_WELD_HISTORY							50 		// cycles
#define WS_MAX_GET_ALARM_HISTORY						50 		// alarms
#define WS_MESSAGE_RESPONSE_TIMEOUT						5*60 	// 5Seconds
#define WS_KERNEL_MESSAGE_RESPONSE_TIMEOUT				60*60 	// 60 Seconds

#define USER_IO_DATA					3
#define USER_IO_DATA_OFFSET				3

#define PS_D_INPUTS_START				0
#define PS_D_OUTPUTS_START				27
#define PS_A_INPUTS_START				51
#define PS_A_OUTPUTS_START				59
#define AC_D_INPUTS_START				63
#define AC_D_OUTPUTS_START				78
#define IO_END							93

/*indexes of Weld History header*/
#define MAX_WELD_FORCE  				6
#define END_HOLD_FORCE                  7
#define WELD_ABSOLUTE                   8
#define TOTAL_ABSOLUTE                  9
#define WELD_COLLAPSE_DISTANCE          10
#define HOLD_COLLAPSE_DISTANCE          11
#define TOTAL_COLLAPSE_DISTANCE         12
#define TRIGGER_DISTANCE                13
#define WELD_VELOCITY                   14
#define WELD_TIME                       15
#define WELDRESULTS_ENERGY				16
#define CYCLE_TIME                      20

// Indexes of the Fields in the user details.
typedef enum
{
	USERID = 0,
	USERLEVEL,
	USERSTATUS,
	DATETIME,
	LASTCHANGEDPASSWORD,
	LASTLOGIN
}DBUSERDETAILS;

// Indexes of the User Level Status
typedef enum
{
	NO_LEVEL = 0,
	EXECUTIVEUSER,
	SUPERVISORUSER,
	TECHNICIANUSER,
	OPERATORUSER
}UserAccessLevel;

//Indexes for system configuration
typedef enum
{
	MEMORY_FULL_ACTION = 1,
	LANGUAGUE,
	STARTUP_SCREEN,
	RECIPE_PREFIX_FOR_BARCODE,
	PART_ID_SWITCH_STATUS,
	AUTHORITY_CHECK_LEVEL,
	UNITS,
	PS_POWERUP_OPTION,
	MACHINE_NAME
}WS_SysConfig;

//Start or stop weld enums
typedef enum
{
	WS_NO_WELD = 0,
	WS_WELD
}STARTSTOP_WELD;

enum
{
	SET_ACTIVE_RECIPE = 1,
	SAVE_CURRENT_RECIPE,
	SET_RECIPE_VALUE,
	DELETE_RECIPE
};

enum
{
	PIN_FUNCTION,
	ACTIVE_STATUS,
	CHECK_STATUS
};

// WebservicesTask Class
class WebServiceTask
{
public:
	WebServiceTask();
	~WebServiceTask();

	void 		WebService_Task(); 
	void		ProcessCommandMessage		(Message& message);
	void 		IdleLogoutFromWebServices	();
	bool		bIsTaskRunStatus			();	
	STATUS 		SendToMsgQ					(const MSG_Q_ID& MsgQID, Message& msg);
	WS_STATUS 	ReadIdleLogoutTime			(int &dwDBIdleLogoutTime);

	//Command Functions
	WS_STATUS	Login				(char * userDetails,Message &messageWeb);
	WS_STATUS	Logout				(Message &messageWeb);	
	WS_STATUS	GetSWVersion		(Message &messageWeb);
	WS_STATUS	GetLastWeldResult	(Message &messageWeb);
	WS_STATUS	SetActiveRecipe		(char * RecipeNum,Message &messageWeb);
	WS_STATUS	DeleteRecipe		(char * RecipeNum,Message &messageWeb);
	WS_STATUS	SaveCurrentRecipe	(Message &messageWeb);
	WS_STATUS	GetRecipeValue		(char * receivedQuery, Message &messageWeb);
	WS_STATUS	SetRecipeValue		(char * receivedQuery, Message &messageWeb);
	WS_STATUS	GetSystemValue		(char * ParamId,Message &messageWeb);
	
	WS_STATUS	GetNumberOfWeldHistory	(Message &messageWeb);
	WS_STATUS	GetWeldHistory			(char * receivedQuery,Message &messageWeb);
	WS_STATUS	GetGraphResults			(char * receivedQuery, Message &messageWeb);
	WS_STATUS	GetNumberOfAlarms		(Message &messageWeb);		
	WS_STATUS 	GetAlarmLog				(char * receivedQuery,Message &messageWeb);
	WS_STATUS 	GetRuntimeDetails		(Message &messageWeb);
	WS_STATUS 	GetAllUserDetails		(Message &messageWeb);
	
	MSG_Q_ID 	DB_MSG_Q_ID,WEBSERVICE_MSG_Q_ID,WEBSERVICEREC_MSG_Q_ID,CTRL_MSG_Q_ID,UI_MSG_Q_ID;
	int			dwIdleLogoutTime;
	int			dwDBIdleLogoutTime;
	int 		dwTimeOut;

private:
	void 					GetDateAndTime				(std::string &DateTime);
	WS_STATUS				InitialRecipeConfiguration	();
	void 					StartStopWeldIndcation		(bool dwSubId);
	WS_STATUS				PasswordExpiryCheck			(string CurrentTime, string DBTime);
	void 					EventForWebServices			(LOG_EVENT eventID, void *pData = NULL);
	int 					ConvertDateToDays			(const char *date);
	bool					FirstTimeLoginCheck			(string CurrentTime, string DBTime);
	bool 					CheckAuthorityLevel			();
	bool					IsAuthorized				(int Privilege);	
	bool					ValidateJson				(Message &ProcessMessageBuff);
	bool					ValidateSid					(Message &ProcessMessageBuff);
	bool 					GetParamDetails				(char *pchBuffer, int &dwParamId, char *chParamValue);
	
	WS_STATUS				FormatWeldHistory 			(int dwFrom, int dwTo, int dwRecipeNo, string& history);
	void 					WeldHistoryJSONFormat 		(string &strJSONFormat,string &strDBData);
	string 					RetryMechanism 				(char* sqlQuery);
	
	void 					ConnectDB();
	void 					CloseDB();
	void		 			WeldGraphJSONFormat			(string &strJSONFormat,string &strDBData,INT32 cycleCounter,INT32 recipeNo);
	
	WS_STATUS 				WSLoginRequestToUI			(char* UserName, UINT8 iUserLevel);
	void					WSLogoutNotificationToUI	();
	WS_STATUS				WSSetUserIOInit				();
	void					WSSetUserIOFormat			(Message& message);
	
	void					WSSetUserPSInputs			(USERIO &userIO, vector<int> PSInputList);
	void					WSSetUserPSOutputs			(USERIO &userIO, vector<int> PSOutputList);
	void					WSSetUserACInputs			(USERIO &userIO, vector<int> ACInputList);
	void					WSSetUserACOutputs			(USERIO &userIO, vector<int> ACOutputList);
	
	void 					WSIODataFromDB				(USERIO &userIO, vector<int> IOList);
	WS_STATUS				WSWaitForStateMachine		();
	bool                    GetRecipeParamValueConversion     (int dwParamID, char *chParamValue, char * chParamConvertedValue);
	bool                    SetRecipeParamValueConversion     (int dwParamID, char *chParamValue, char *chParamValueConverted);

	char 					strLoggedUser[USERDATA_LENGTH];
	Message 				messageDb;
	CommonProperty 			*CP;
	CommunicateInterface 	*Interface;
	WeldRecipe 				m_WeldRecipe;
	Recipe 					*m_RecipePtr;
	AuthenticationKey 		m_AuthenticationKeyObj;
    SQLiteDB 				m_db;
    Utils                   *Util;
};

#endif /* WEBSERVICETASK_H_ */
