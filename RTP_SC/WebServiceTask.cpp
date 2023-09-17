/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     WebServices Task for communicating with the Webcli in Kernel and to process the requests comes from HTTP/HTTPS clients   

 **********************************************************************************************************/

#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "MainTask.h"
#include "Common.h"
#include "WebServiceTask.h"
#include "versions.h"
#include "version_common.h"
#include "DataBaseConfiguration.h"
#include "SC.h"

#include <time.h>
#include <cmath>
#include "SysConfigEeprom.hpp"

using namespace std;

char RecvMsgBuffer[MAX_SIZE_OF_MSG_LENGTH] 	= { 0x00 };
char UserLevel[USERDATA_LENGTH] 			= { 0x00 };
char m_PrivateKey[PRIVATE_KEY_SIZE + 4]       = { 0x00 }; /* Adding 4 bytes extra for null character*/

/**************************************************************************//**
 * \brief  - Acquires message queue handles for intertask communications
 *			 It is called once the Webservice task is created.
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
WebServiceTask::WebServiceTask(): DB_MSG_Q_ID(0), WEBSERVICE_MSG_Q_ID(0)
{
	CP						= CommonProperty::getInstance(); 
	m_RecipePtr				= Recipe::getInstance(); 
	DB_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[DB_T]);
	WEBSERVICE_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[WEBSERVICE_T]);
	CTRL_MSG_Q_ID			= CP->GetMsgQId(cTaskName[CTRL_T]);
	UI_MSG_Q_ID				= CP->GetMsgQId(cTaskName[UI_T]);
	WEBSERVICEREC_MSG_Q_ID 	= MSG_Q_ID_NULL;
	WEBSERVICEREC_MSG_Q_ID 	= msgQOpen("/WebServiceRec_Task", MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO, OM_CREATE, 0); 
	dwIdleLogoutTime		= 0;
	dwDBIdleLogoutTime		= 30; /* default value for Idle logout time is 30mins */
	dwTimeOut				= WAIT_FOREVER;
	m_AuthenticationKeyObj.GetAuthenticationKey(m_PrivateKey);
	ConnectDB();
}

/**************************************************************************//**
 * \brief  - WebServiceTask destructor
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
WebServiceTask::~WebServiceTask()
{
	CP = NULL;
	CloseDB();
}

/**************************************************************************//**
 * \brief  - Convert the Given date and time string to Days.
 *
 * \param  - const char *date. Input format should be "YYYY/MM/DD HH:mm:SS"
 *
 * \return  - int. Will return the no.of days from Jan 1st of year 0001 onwards
 *
 ******************************************************************************/
int WebServiceTask::ConvertDateToDays(const char *date)
{
	LOGDBG("WEBSERVICE_T : ConvertDateToDays Entry",0,0,0);
	long int retval=0;
	int Year, Mon, Day, hour, min, sec;
	sscanf(date,"%d/%d/%d %d:%d:%d", &Year, &Mon, &Day, &hour, &min, &sec);

	/* Simplified logic to calculate the no.of days from 0001-01-01 to specified date including the leap year */
	if(Mon < 3)
	{
		Year--;
		Mon+= 12;
	}
	retval = 365*Year + Year/4 - Year/100 + Year/400 + (153*Mon - 457)/5 + Day - 306;
	LOGDBG("WEBSERVICE_T : ConvertDateToDays Exit", 0, 0, 0);
	return retval;
}

/**************************************************************************//**
 * \brief  - Gets main task's run status, it is called in HttpWebService_Task()
 *
 * \param  - None
 *
 * \return  - bool TRUE or FALSE to check whether Webservice task is running or not
 *
 ******************************************************************************/
bool WebServiceTask::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}

/**************************************************************************//**
 * \brief  - Web service Task entry point and constructor. 
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
void HttpWebService_Task(void)
{	
	LOGDBG("WEBSERVICE_T : HttpWebService_Task Entry", 0, 0, 0);
	Message 		ProcessMessageBuff;
	WebServiceTask * pWebServiceTask 	= new (nothrow) WebServiceTask();

	if(NULL != pWebServiceTask)
	{
		/* Web Service Task loop and the bIsTaskRun flag enabled when task created */
		while (pWebServiceTask->bIsTaskRunStatus())
		{
			memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
			if(msgQReceive(pWebServiceTask->WEBSERVICE_MSG_Q_ID, RecvMsgBuffer, MAX_SIZE_OF_MSG_LENGTH, pWebServiceTask->dwTimeOut) != ERROR) 
			{
				memcpy(&ProcessMessageBuff, RecvMsgBuffer, sizeof(ProcessMessageBuff));
				pWebServiceTask->dwIdleLogoutTime = 0;
				pWebServiceTask->ProcessCommandMessage(ProcessMessageBuff);	
			}
			else
			{
				pWebServiceTask->dwIdleLogoutTime+= 1;				

				if(pWebServiceTask->dwIdleLogoutTime == pWebServiceTask->dwDBIdleLogoutTime)
				{
					pWebServiceTask->IdleLogoutFromWebServices();
				}
			}
		}	
		delete pWebServiceTask;
	}
	else
	{
		LOGDBG("WEBSERVICE_T : ------------- Allocation failed",0,0,0);
	}

	LOGDBG ("WEBSERVICE_T : ------------- Task Terminated",0,0,0);
	pWebServiceTask = NULL;
	taskDelete(taskIdSelf());
}

/**************************************************************************//**
 * \brief  - Process commands received by the message queue from other tasks
 * 			 It is the entry point for communications with the web service task
 * 			 It is called in HttpWebService_Task()
 *
 * \param  - Message &ProcessMessageBuff
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::ProcessCommandMessage(Message &ProcessMessageBuff)
{
	LOGDBG("WEBSERVICE_T : ProcessCommandMessage Entry", 0, 0, 0);
	Message messageWeb, messageUI;
	WS_STATUS status = WS_SUCCESS;
	char pRecvBuffer[MAX_SIZE_OF_MSG_LENGTH] = { 0x00 };
	bool isProcessReq = false;
	
	if((ProcessMessageBuff.msgID != TO_WEB_SERVICES_TASK_GET_RUNTIME_DETAILS) && (ProcessMessageBuff.msgID != TO_WEB_SERVICES_TASK_GET_ALL_USER_DETAILS_FU) )
	{
		if(!ValidateJson(ProcessMessageBuff))
		{
			messageWeb.msgID = ProcessMessageBuff.msgID;
			sprintf(messageWeb.Buffer, "{%s:%d}", STATUS_CODE, INVALID_JSON_FORMAT);
		}
		else if(!ValidateSid(ProcessMessageBuff))
		{
			messageWeb.msgID = ProcessMessageBuff.msgID;
			sprintf(messageWeb.Buffer, "{%s:%d}", STATUS_CODE, INVALID_SERVICE_DATA);
		}
		/* bIsWebServicesLogged flag is set when user logged into Web services. Check the same flag then return NOT_LOGGED_IN status code  if false */
		else if(!CP->bIsWebServicesLogged && ProcessMessageBuff.msgID != TO_WEB_SERVICES_TASK_LOGIN)
		{
			messageWeb.msgID = ProcessMessageBuff.msgID;
			sprintf(messageWeb.Buffer, "{%s:%d}", STATUS_CODE, NOT_LOGGED_IN);
		}
		else if(CP->bIsWebServicesLogged && ProcessMessageBuff.msgID == TO_WEB_SERVICES_TASK_LOGIN)
		{
			messageWeb.msgID = ProcessMessageBuff.msgID;
			sprintf(messageWeb.Buffer, "{%s:%d}", STATUS_CODE, ALREADY_LOGGED_IN);
		}
		else if(((CP->bIsWebServicesLogged) && (ProcessMessageBuff.msgID != TO_WEB_SERVICES_TASK_LOGIN)) && (!(IsAuthorized(ProcessMessageBuff.msgID))))
		{
			sprintf(messageWeb.Buffer, "{%s:%d}", STATUS_CODE, INVALID_SECURITYLEVEL);
		}
		else
			isProcessReq = true;
	}
	else
		isProcessReq = true;
	
	if(isProcessReq == true)
	{
		semTake (m_db.getSCDBMutex(), WAIT_FOREVER);
		/* Checking the Web services commands based up on the ID's */
		switch(ProcessMessageBuff.msgID)
		{
		/* SystemLogin Command */
		case TO_WEB_SERVICES_TASK_LOGIN: 
			messageWeb.msgID = TO_WEBCLI_KERNEL_LOGIN;
			status = Login(ProcessMessageBuff.Buffer,messageWeb);
			break;

			/* SystemLogout Command  */
		case TO_WEB_SERVICES_TASK_LOGOUT:
			messageWeb.msgID = TO_WEBCLI_KERNEL_LOGOUT;	
			status = Logout(messageWeb);
			break;

			/* GetSoftwareVersion Command  */
		case TO_WEB_SERVICES_TASK_SW_VERSION:
			messageWeb.msgID = TO_WEBCLI_KERNEL_SW_VERSION;
			status = GetSWVersion(messageWeb);
			break;

			/* GetWeldResult Command */
		case TO_WEB_SERVICES_TASK_LAST_WELD:
			messageWeb.msgID = TO_WEBCLI_KERNEL_LAST_WELD;
			status = GetLastWeldResult(messageWeb);
			break;

			/* SetActiveRecipe Command */
		case TO_WEB_SERVICES_TASK_SET_ACTIVE_RECIPE:
			messageWeb.msgID = TO_WEBCLI_KERNEL_SET_ACTIVE_RECIPE;
			status = SetActiveRecipe(ProcessMessageBuff.Buffer,messageWeb);	
			
			if (WS_SUCCESS == status)
			{
				/* Refresh the actual screen on the UI*/
				memset(&messageUI, 0x00, sizeof(messageUI));
				messageUI.msgID = TO_UI_TASK_WEBSERVICE_REFRESH_SCREEN;					
				sprintf(messageUI.Buffer, "%d", SET_ACTIVE_RECIPE);
				SendToMsgQ(UI_MSG_Q_ID,messageUI);
			}
			break;

			/* DeleteRecipe Command */
		case TO_WEB_SERVICES_TASK_DELETE_RECIPE:
			messageWeb.msgID = TO_WEBCLI_KERNEL_DELETE_RECIPE;
			status = DeleteRecipe(ProcessMessageBuff.Buffer,messageWeb);	
			
			if (WS_SUCCESS == status)
			{
				/* Refresh the actual screen on the UI*/
				memset(&messageUI, 0x00, sizeof(messageUI));
				messageUI.msgID = TO_UI_TASK_WEBSERVICE_REFRESH_SCREEN;					
				sprintf(messageUI.Buffer, "%d", DELETE_RECIPE);
				SendToMsgQ(UI_MSG_Q_ID,messageUI);
			}
			break;

			/* SaveCurrentRecipe Command */
		case TO_WEB_SERVICES_TASK_SAVE_RECIPE:
			messageWeb.msgID = TO_WEBCLI_KERNEL_SAVE_RECIPE;
			status = SaveCurrentRecipe(messageWeb);
			
			if (WS_SUCCESS == status)
			{
				/* Refresh the actual screen on the UI*/
				memset(&messageUI, 0x00, sizeof(messageUI));
				messageUI.msgID = TO_UI_TASK_WEBSERVICE_REFRESH_SCREEN;					
				sprintf(messageUI.Buffer, "%d", SAVE_CURRENT_RECIPE);
				SendToMsgQ(UI_MSG_Q_ID,messageUI);
			}
			break;

			/* SetRecipeValue Command */
		case TO_WEB_SERVICES_TASK_SET_RECIPE_VALUE:
			messageWeb.msgID = TO_WEBCLI_KERNEL_SET_RECIPE_VALUE;
			status = SetRecipeValue(ProcessMessageBuff.Buffer,messageWeb);
			
			if (WS_SUCCESS == status)
			{
				/* Refresh the actual screen on the UI*/
				memset(&messageUI, 0x00, sizeof(messageUI));
				messageUI.msgID = TO_UI_TASK_WEBSERVICE_REFRESH_SCREEN;					
				sprintf(messageUI.Buffer, "%d", SET_RECIPE_VALUE);
				SendToMsgQ(UI_MSG_Q_ID,messageUI);
			}
			break;	

			/* GetRecipeValue Command */
		case TO_WEB_SERVICES_TASK_GET_RECIPE_VALUE:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_RECIPE_VALUE;
			status = GetRecipeValue(ProcessMessageBuff.Buffer,messageWeb);
			break;

			/* GetSystemValue Command */
		case TO_WEB_SERVICES_TASK_GET_SYSTEM_VALUE:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_SYSTEM_VALUE;
			status = GetSystemValue(ProcessMessageBuff.Buffer,messageWeb);	
			break;			

			/* GetNumWeldHistory Command */
		case TO_WEB_SERVICES_TASK_GET_NUM_WELD_HISTROY:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_NUM_WELD_HISTROY;
			status = GetNumberOfWeldHistory(messageWeb);	
			break;

			/* GetWeldHistory Command */
		case TO_WEB_SERVICES_TASK_GET_WELD_HISTROY:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_WELD_HISTROY_FINISH;
			status = GetWeldHistory(ProcessMessageBuff.Buffer,messageWeb);
			break;

			/* GetNumAlarms Command */
		case TO_WEB_SERVICES_TASK_GET_NUM_ALARMS:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_NUM_ALARMS;
			status = GetNumberOfAlarms(messageWeb);	
			break;

			/* GetAlarmLog Command */
		case TO_WEB_SERVICES_TASK_GET_ALARM_LOG:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_ALARM_LOG_FINISH;
			status = GetAlarmLog(ProcessMessageBuff.Buffer,messageWeb);
			break;

			/* GetGraphResult Command */
		case TO_WEB_SERVICES_TASK_GET_GRAPH_RESULT:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_GRAPH_RESULT_FINISH;
			status = GetGraphResults(ProcessMessageBuff.Buffer,messageWeb);
			break;

			/* Reading the Runtime configuration details of Web Services */
		case TO_WEB_SERVICES_TASK_GET_RUNTIME_DETAILS:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_RUNTIME_DETAILS;
			status = GetRuntimeDetails(messageWeb);
			break;

			/* Reading all the User details from the DB for the Firmware authentication purpose */
		case TO_WEB_SERVICES_TASK_GET_ALL_USER_DETAILS_FU:
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_ALL_USER_DETAILS_FU;
			status = GetAllUserDetails(messageWeb);
			break;

			/* Normally the default case should never come as we are already checking all the cases on the Kernel side and 
			 ** only valid cases will come here, So error handling is not required.
			 ** If internally any developer sends specifically then this log will help him to validate. */
		default:
			LOGERR("WEBSERVICE_T : ProcessCommandMessage : Unknown command : %d",ProcessMessageBuff.msgID,0,0);
			break;
		}
	}
	semGive(m_db.getSCDBMutex());
	//Disables WS log attempt flag
	CP->bIsWSLogAttempt = false;
	
	if(status != WS_SUCCESS)
		sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,status);

	LOGDBG("WEBSERVICE_T : ProcessCommandMessage Exit",0,0,0);
	/* Send the response to Kernel */
	SendToMsgQ(WEBSERVICEREC_MSG_Q_ID,messageWeb);
}

/**************************************************************************//**
 * 
 * \brief   - Post the encoded message to destination message queue.
 * 			 It is called in Login()
 *
 * \param   - const MSG_Q_ID& MsgQID, Message& msg
 *
 * \return  - STATUS - OK or ERROR to check msgq send is successful or not
 *
 ******************************************************************************/
STATUS WebServiceTask::SendToMsgQ(const MSG_Q_ID &MsgQID, Message& msg)
{
	STATUS ret = OK;

	if(msgQSend(MsgQID,reinterpret_cast<char*>(&msg),sizeof(msg),NO_WAIT,MSG_PRI_NORMAL) != OK)
	{
		LOGERR("WEBSERVICE_T : Message send failed to MSG Q",0,0,0);
		ret = ERROR;
	}

	return ret;
}

/**************************************************************************//**
 * \brief   - Validating the json format of POST data
 *
 * \param   - Message &ProcessMessageBuff
 *
 * \return  - bool
 *
 ******************************************************************************/
bool WebServiceTask::ValidateJson(Message &ProcessMessageBuff)
{
	int dwCurlyBrace = 0, dwSquareBracket = 0, dwDoubleQuotes = 0;
	for(int dwInc = 0;dwInc < strlen(ProcessMessageBuff.Buffer);dwInc++)
	{
		if(ProcessMessageBuff.Buffer[dwInc] == '{')
			dwCurlyBrace++;
		else if(ProcessMessageBuff.Buffer[dwInc] == '"')
			dwDoubleQuotes++;
		else if(ProcessMessageBuff.Buffer[dwInc] == '[')
			dwSquareBracket++;
		else if(ProcessMessageBuff.Buffer[dwInc] == '}')
			dwCurlyBrace--;
		else if(ProcessMessageBuff.Buffer[dwInc] == ']')
			dwSquareBracket--;
	}
	if((dwCurlyBrace != 0) || (dwSquareBracket != 0) || ((dwDoubleQuotes % 2) != 0))
	{
		return  false;
	}
	else 
		return  true;
}

/**************************************************************************//**
 * \brief   - To validate SID in POST data
 *
 * \param   - Message &ProcessMessageBuff
 *
 * \return  - bool.
 *
 ******************************************************************************/
bool WebServiceTask::ValidateSid(Message &ProcessMessageBuff)
{
	char * pSIDcheck = strstr(ProcessMessageBuff.Buffer,SID);
	if((pSIDcheck == NULL) && ProcessMessageBuff.msgID != TO_WEB_SERVICES_TASK_LOGIN)
	{
		return  false;
	}
	else
		return  true;
}

/**************************************************************************//**
 * \brief   - To get current date and time. It is called in Login() 
 * 			  and EventLogWeb()
 *
 * \param   - string date
 *
 * \return  - none.
 *
 ******************************************************************************/
void WebServiceTask::GetDateAndTime(string &DateTime)
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
 * \brief   - Checks the authority of a particular command according to the user level status
 *
 * \param   - int Privilege
 *
 * \return  - bool.
 *
 ******************************************************************************/
bool WebServiceTask::IsAuthorized(int Privilege)
{
	bool bRetVal;
	switch(CP->WSUserLevelStatus)
	{
	case EXECUTIVEUSER:
		bRetVal = true;
		break;
	case SUPERVISORUSER:
	case TECHNICIANUSER:
		if(Privilege == TO_WEB_SERVICES_TASK_DELETE_RECIPE)
			bRetVal = false;
		else
			bRetVal = true;
		break;

	case OPERATORUSER:
		if(Privilege == TO_WEB_SERVICES_TASK_SET_ACTIVE_RECIPE)
		{
			messageDb.msgID = TO_DB_TASK_WS_AUTHORITY_CHECK_OPERATOR;
			strcpy(messageDb.Buffer,"");
			SendToMsgQ(DB_MSG_Q_ID,messageDb);

			/* Receive operator authority option from DB task */
			if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
			{
				memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
			}

			if(messageDb.msgID == TO_WEB_SERVICES_TASK_AUTHORITY_CHECK_OPERATOR)
			{
				if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
				{
					LOGDBG("WEBSERVICE_T : IsAuthorized Exit",0,0,0);
					return false;
				}
				
				if(atoi(messageDb.Buffer) == 0)
					bRetVal = false;
				else 
					bRetVal = true;
			}
			else
				SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		}
		else if((Privilege == TO_WEB_SERVICES_TASK_SAVE_RECIPE) || (Privilege == TO_WEB_SERVICES_TASK_DELETE_RECIPE) || (Privilege == TO_WEB_SERVICES_TASK_SET_RECIPE_VALUE))
			bRetVal = false;
		else
			bRetVal = true;

		break;

	default:
		break;
	}
	return bRetVal;
}

/**************************************************************************//**
 * \brief   - bIsWebServicesLogged Flag will be reset if it is set already.
 * 				 It is called from HttpWebService_Task function
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::IdleLogoutFromWebServices()
{
	Message Message;
	if(CP->bIsWebServicesLogged)
	{
		Logout(Message);
		CP->bIsWSLogAttempt = false;
	}
}

/**************************************************************************//**
 * \brief   - Sends idle logout time req to DB Task and receives response from DB Task 
 * 			  through message queues. It is called in Login function
 *
 * \param   - int &dwDBIdleLogoutTime
 *
 * \return  - WS_STATUS.
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::ReadIdleLogoutTime(int &dwDBIdleLogoutTime)
{
	LOGDBG("WEBSERVICE_T : ReadIdleLogoutTime Entry",0,0,0);
	messageDb.msgID = TO_DB_TASK_WS_GET_IDLE_LOGOUT_TIME;
	strcpy(messageDb.Buffer,"");
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	/* Receive message queue from DB task */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		return SYSTEM_BUSY;
	}

	if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_GET_IDLE_LOGOUT_TIME)
	{
		if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
		{
			LOGDBG("WEBSERVICE_T : ReadIdleLogoutTime Exit",0,0,0);
			return SYSTEM_BUSY;
		}
		if(messageDb.Buffer != NULL)
		{
			dwDBIdleLogoutTime = atoi(messageDb.Buffer);
			return WS_SUCCESS;
		}
		else
			return SYSTEM_BUSY;
	}
	else
	{
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Compare the date and time to check first time login
 *
 * \param   - string LastLogin, string LastChangedPassword
 *
 * \return  - bool
 *
 ******************************************************************************/
bool WebServiceTask::FirstTimeLoginCheck(string StrLastLogin, string StrLastChangedPassword)
{
	LOGDBG("WEBSERVICE_T : FirstTimeLoginCheck Entry",0,0,0);
	if(StrLastChangedPassword == StrLastLogin)
	{
		LOGDBG("WEBSERVICE_T : FirstTimeLoginCheck Exit",0,0,0);
		return true;
	}
	else
	{
		LOGDBG("WEBSERVICE_T : FirstTimeLoginCheck Exit",0,0,0);
		return false;
	}
}

/**************************************************************************//**
 * \brief   - Compare the date and time to check password expiry
 *
 * \param   - string StrCurrentTime, string StrDBTime
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::PasswordExpiryCheck(string StrCurrentTime, string StrDBTime)
{
	LOGDBG("WEBSERVICE_T : PasswordExpiryCheck Entry",0,0,0);
	int dwPasswordExpiryInDays;
	int dwCurrentDateInDays,dwLastPasswordChangeDateInDays;
	messageDb.msgID = TO_DB_TASK_WS_GET_PW_EXPIRY_DAYS;
	strcpy(messageDb.Buffer,"");
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
	/* Receive message queue from DB task */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : PasswordExpiryCheck Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	if(messageDb.msgID == TO_WEB_SERVICES_TASK_GET_PW_EXPIRY_DAYS)
	{
		if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
		{
			LOGDBG("WEBSERVICE_T : PasswordExpiryCheck Exit",0,0,0);
			return SYSTEM_BUSY;
		}
		/* Check message buffer length*/
		if(strlen(messageDb.Buffer) > 0)
			dwPasswordExpiryInDays = atoi(messageDb.Buffer);
		else
		{
			LOGDBG("WEBSERVICE_T : PasswordExpiryCheck Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. 
		 * So, ignoring the current request and redirecting the new Request received to ProcessCommand function */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG(" WEBSERVICE_T : PasswordExpiryCheck Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	dwCurrentDateInDays = ConvertDateToDays(StrCurrentTime.c_str());
	dwLastPasswordChangeDateInDays = ConvertDateToDays(StrDBTime.c_str());

	if((dwCurrentDateInDays - dwLastPasswordChangeDateInDays) > dwPasswordExpiryInDays)
	{
		LOGDBG("WEBSERVICE_T :  PasswordExpiryCheck Exit",0,0,0);
		return PASSWORDEXPIRED;
	}

	LOGDBG("WEBSERVICE_T : PasswordExpiryCheck Exit",0,0,0);
	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief   - Checks Authority level from intf_SysConfig_t structure
 *
 * \param   - None
 *
 * \return  - bool
 *
 ******************************************************************************/
bool WebServiceTask::CheckAuthorityLevel()
{
	intf_SysConfig_t SysConfigInfo;
	CP->GetSysConfContext(SysConfigInfo);

	if(SysConfigInfo.AuthCheckLevel)
		return false;
	else
		return true;
}

/**************************************************************************//**
 * \brief   - Initialize recipe zero and update the local Weld recipe structures
 *
 * \param   - none
 *
 * \return  - WS_STATUS
 * 
 ******************************************************************************/
WS_STATUS WebServiceTask::InitialRecipeConfiguration()
{
//	vector<string> vRecipeParams;
	string 	strActiveWeldRecipe = "", strSRActiveRecipe = "", strBransonOnly = "", strSetupLimits = "";
	DBInterface::GetRecipeZeroStrings(strActiveWeldRecipe,strSRActiveRecipe,strBransonOnly,strSetupLimits);
	string strRecipeNum, strRecipeName, strRecipeData, strVerRecipeNum;
	INT32 dwRecipeNo, dwRecVeripeNum;

	if((!strActiveWeldRecipe.empty()) && (!strSRActiveRecipe.empty()) )
	{
		stringstream ssRecipeData(strActiveWeldRecipe.c_str());
		
		getline (ssRecipeData, strRecipeNum, ',');
		getline (ssRecipeData, strRecipeName, ',');
		getline (ssRecipeData, strVerRecipeNum, ',');

		dwRecipeNo = atoi(strRecipeNum.c_str());
		dwRecVeripeNum = atoi(strVerRecipeNum.c_str());

		if((dwRecipeNo != 0) ||((dwRecipeNo == 0) && (dwRecVeripeNum == 0)))
		{
			messageDb.msgID = TO_DB_TASK_WS_GET_ACTIVE_RECIPE;
			strcpy(messageDb.Buffer,"");
			SendToMsgQ(DB_MSG_Q_ID,messageDb);
		}
		else if((dwRecipeNo == 0) && (dwRecVeripeNum != 0))
		{
			CP->bIsActiveRecipeAvailble = true;
			messageDb.msgID = TO_DB_TASK_WS_SET_ACTIVE_WELD_RECIPE;
			strcpy(messageDb.Buffer,strActiveWeldRecipe.c_str());
			SendToMsgQ(DB_MSG_Q_ID,messageDb);

			messageDb.msgID = TO_DB_TASK_WS_SET_ACTIVE_SR_RECIPE;
			strcpy(messageDb.Buffer,strSRActiveRecipe.c_str());
			SendToMsgQ(DB_MSG_Q_ID,messageDb);
		}		
	}
	else
	{
		messageDb.msgID = TO_DB_TASK_WS_GET_ACTIVE_RECIPE;
		strcpy(messageDb.Buffer,"");
		SendToMsgQ(DB_MSG_Q_ID,messageDb);
	}
	
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	/* Check if received msg id is for active recipe or not */
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_GET_ACTIVE_RECIPE)
	{
		if(strlen(messageDb.Buffer) > 0)
		{
			if(WS_SUCCESS == atoi(messageDb.Buffer)) 
				return WS_SUCCESS;
			else
				return SYSTEM_BUSY;			
		}
		else
			return SYSTEM_BUSY;
	}
	else 
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : Logout Exit",0,0,0);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Send indication to the Control task for start or stop the weld
 *
 * \param   - INT32 dwSubId
 *
 * \return  - none
 * 
 ******************************************************************************/
void WebServiceTask::StartStopWeldIndcation(bool bSubId)
{
	Message message;
	if(bSubId)
		sprintf(message.Buffer,"%c",WS_WELD);
	else
		sprintf(message.Buffer,"%c",WS_NO_WELD);

	message.msgID = TO_CTRL_TASK_SET_NEXT_OPERATION_REQ;
	SendToMsgQ(CTRL_MSG_Q_ID,message);	
}

/**************************************************************************//**
 * \brief   - Divides the ParamID and value form the provided string in the below format
 * 					"ParamId":1,"ParamValue":10 or "ParamId":1 (Here ParamValue will be empty)
 *
 * \param   - char *pchBuffer, int &dwParamId, char *chParamValue
 *
 * \return  - boolean, returns input string is valid or not.
 * 
 ******************************************************************************/
bool WebServiceTask::GetParamDetails(char *pchBuffer, int &dwParamId, char *chParamValue)
{
	if(!pchBuffer || !chParamValue)
		return false;

	bool dwReturnValue = true;

	char* var1 = strstr(pchBuffer,"\"ParamId\":");
	if(var1 == NULL)
		dwReturnValue = false;
	else
	{
		var1 = var1 + strlen("\"ParamId\":");
		dwParamId = atoi(var1);
		if(dwParamId == 0)
			dwReturnValue = false;
	}

	char* var2 = strstr(pchBuffer,"\"ParamValue\":");
	if(var2 == NULL)
	{
		strcpy(chParamValue,"");
	}
	else
	{
		var2 = var2 + strlen("\"ParamValue\":");
		char *temp = var2;
		bool bIsProperVal;
		while(*temp)
		{
			if(isdigit(*temp) || *temp == '.')
				bIsProperVal = true;
			else
			{
				bIsProperVal = false;
				break;
			}
			temp++;
		}
		if(bIsProperVal)
		{
			strcpy(chParamValue,var2);
			if(strlen(chParamValue) == 0)
				dwReturnValue = false;
		}
		else
			dwReturnValue = false;
	}
	return dwReturnValue;
}

/**************************************************************************//**
 * \brief   - Validate the user credentials for login
 * 			  It is called in TO_WEB_SERVICES_TASK_LOGIN Switch case
 *
 * \param   - char * userDetails,Message &messageWeb
 *
 * \return  - WS_STATUS to check the successful login or error conditions
 * 
 ******************************************************************************/
WS_STATUS WebServiceTask::Login(char * userDetails,Message &messageWeb)
{	
	LOGDBG("WEBSERVICE_T : Login Entry",0,0,0);
	char 	*pToken 			= NULL;
	string  strDateTime;
	char 	userStatus[USERDATA_LENGTH] = {0x00};
	char 	loggedUser[USERDATA_LENGTH] = {0x00};
	char 	LastChangedPassword[USERDATA_LENGTH] = {0x00};
	char 	LastLogin[USERDATA_LENGTH] = {0x00};
	char 	dwIndex = 0;
	vector<string> vecUserDetails;
	string 	itemUserDetails;
	string 	strUsername = "";
	string 	strPassword = "";
	string  strPrivateKey = "";
	UINT8	iUserLevel;
	
	char 	userCredentials[USERDATA_LENGTH] = {0x00};
	WS_STATUS status;
	
	//Webservices will wait for state machine to be in ready/pre ready state just if the HMI is logged in
	//and the state machine is not in alarm state.
	if((true == CP->bIsHMILogged) && (SC::stateMachine.GetStateType() != SC_ALARM))
	{
		status = WSWaitForStateMachine();
		if(WS_SUCCESS != status)
			return status;
	}	
	/* If authority level is true then return status code AUTHORITY_CHECK_DISABLED */
	if(CheckAuthorityLevel())
	{
		return AUTHORITY_CHECK_DISABLED;   /* Authority check disabled */
	}

	if(strlen(userDetails) > 0)
	{
		stringstream ssUserDetails (userDetails);
		while(getline(ssUserDetails,itemUserDetails,'\"'))
			vecUserDetails.push_back(itemUserDetails);

		if(vecUserDetails.size() != JSONFORMAT_USERDETAILS_LENGTH)
		{
			LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
			return INVALID_JSON_FORMAT;
		}

		strUsername = std::string(vecUserDetails[USERID_VALUE]);
		strPassword = std::string(vecUserDetails[PASSWORD_VALUE]);
		strPrivateKey = std::string(vecUserDetails[KEY_VALUE]);
		/* Check if user name or password is empty */
		if((strUsername.empty()) || (strPassword.empty()) || (strcmp(m_PrivateKey,strPrivateKey.c_str())))
		{			
			LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
			return WRONGNAME_PASSWORD; /* Wrong user name and password */
		}
		
		/* Send user credentials to DB task to get the user details through message queue */
		messageDb.msgID = TO_DB_TASK_WS_USER_DETAILS;
		strcat(userCredentials,strUsername.c_str());
		strcat(userCredentials,",");
		strcat(userCredentials,strPassword.c_str());
		memcpy(messageDb.Buffer,userCredentials,sizeof(userCredentials));
		SendToMsgQ(DB_MSG_Q_ID,messageDb);

		memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
		/* Receive user details from DB task to validate user credentials through message queue */
		if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
		{
			memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
		}
		else
		{
			LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
			return SYSTEM_BUSY;
		}

		/* Check if received msg id is for user details or not */
		if(messageDb.msgID == TO_WEB_SERVICES_TASK_USER_DETAILS)
		{
			if(strlen(messageDb.Buffer) > 0)
			{
				if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
				{
					LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
					return SYSTEM_BUSY; 	/* System busy*/
				}
				pToken = strtok((char *)messageDb.Buffer, ",");
				while (pToken != NULL)
				{
					switch(dwIndex)
					{
					case USERID:
						strcpy(loggedUser,pToken);
						break;
					case USERSTATUS:
						strcpy(userStatus,pToken);
						break;
					case LASTCHANGEDPASSWORD:
						strcpy(LastChangedPassword,pToken);
						break;
					case LASTLOGIN:
						strcpy(LastLogin,pToken);
						break;
					case USERLEVEL:
						strcpy(UserLevel,pToken);
						
						if(!strcmp(UserLevel, EXECUTIVE))
							iUserLevel = EXECUTIVEUSER;
						else if(!strcmp(UserLevel, SUPERVISOR))
							iUserLevel = SUPERVISORUSER;
						else if(!strcmp(UserLevel, TECHNICIAN))
							iUserLevel = TECHNICIANUSER;
						else if(!strcmp(UserLevel, OPERATOR))
							iUserLevel = OPERATORUSER;
						break;
					default:
						break;
					}
					pToken = strtok(NULL, ",");
					dwIndex++;
				}

				if(strcmp(userStatus, USERSTATUS_ENABLED) == 0)
				{
					if(FirstTimeLoginCheck(LastLogin,LastChangedPassword))
					{
						LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
						return FIRSTTIMELOGIN;
					}

					GetDateAndTime(strDateTime);
					status = PasswordExpiryCheck(strDateTime,LastChangedPassword);
					if(WS_SUCCESS != status)
					{
						LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
						return status;
					}
					
					status = ReadIdleLogoutTime(dwDBIdleLogoutTime);
					if(WS_SUCCESS != status)
					{
						LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
						return status;
					}
					
					//Asks for UI permission to login just if the UI is logged in
					if (true == CP->bIsHMILogged)
					{
						status = WSLoginRequestToUI(loggedUser, iUserLevel);
						if(WS_SUCCESS != status)
							return status;
					}
					else //If the UI is logged out, weld results will be attributed to the WS user
					{
						CP->m_RecipeSC.SetUserName(loggedUser);
					}

					strcpy(strLoggedUser,loggedUser);
					
					/* Raise an Event for web service login in DB_Task */
					messageDb.msgID = TO_DB_TASK_WS_EVENT_LOGIN;
					memset(messageDb.Buffer,0x00,sizeof(messageDb.Buffer));
					snprintf (messageDb.Buffer, sizeof (messageDb.Buffer), "%s", strLoggedUser); 
					SendToMsgQ (DB_MSG_Q_ID,messageDb);

					memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
					/* Receive user details from DB task to validate user credentials through message queue */
					if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
					{
						memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
						
						if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_EVENT_LOGIN)
						{
							if(EVENT_LOG_FAIL == atoi(messageDb.Buffer))
							{
								//Reverts the WS login status from the UI
								if(true == CP->bIsHMILogged)
									WSLogoutNotificationToUI();
								return MEMORY_FULL_ALARM;
							}
						}
						else 
						{
							/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
							SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
							LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
							//Reverts the WS login status from the UI
							if(true == CP->bIsHMILogged)
								WSLogoutNotificationToUI();
							return SYSTEM_BUSY;
						}	
					}
					else
					{
						LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
						//Reverts the WS login status from the UI
						if(true == CP->bIsHMILogged)
							WSLogoutNotificationToUI();
						return SYSTEM_BUSY;
					}
					
					//Set the User IOs if the HMI is logged out
					if(false == CP->bIsHMILogged)
					{
						status = WSSetUserIOInit();
						if(WS_SUCCESS != status)
							return status;
					}
					
					WS_STATUS dwstatus = InitialRecipeConfiguration();
					if(dwstatus)
					{
						LOGDBG("WEBSERVICE_T : Active Recipe Not Available.",0,0,0);
					}
					
				}			
				else 
					return USER_ACCOUNT_DISABLED; /* User is disabled */
			}
			else if(strlen(messageDb.Buffer) == 0)
			{				
				LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
				return WRONGNAME_PASSWORD; 	/* Wrong user name and password */
			}	
			else
			{
				LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
				return SYSTEM_BUSY; 	/* System busy*/
			}
		}
		else 
		{
			/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
			SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
			LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
		return INVALID_SERVICE_DATA;
	}
	
	dwTimeOut = WS_KERNEL_MESSAGE_RESPONSE_TIMEOUT;	
	
	StartStopWeldIndcation(true);
	
	//Update global variables
	CP->WSLoggedUserName = loggedUser;
	CP->WSUserLevelStatus = iUserLevel;
	CP->bIsWebServicesLogged = true;
	
	sprintf(messageWeb.Buffer,"{%s:%d,%s}",STATUS_CODE,WS_SUCCESS,SID);	/* WS_SUCCESSful login */
	
	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief   - Logout.It is called in TO_WEB_SERVICES_TASK_LOGOUT switch case
 *
 * \param   - Message &messageWeb
 *
 * \return  - WS_STATUS to check successful logout or error conditions
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::Logout(Message &messageWeb)
{	
	LOGDBG("WEBSERVICE_T : Logout Entry",0,0,0);
	
	WS_STATUS status;
	
	if(SC::stateMachine.GetStateType() != SC_ALARM)
	{
		status = WSWaitForStateMachine();
		if(WS_SUCCESS != status)
			return status;
	}
		
	if(strlen(strLoggedUser) > 0)
	{
		/* Raise an Event for web service login in DB_Task */
		messageDb.msgID = TO_DB_TASK_WS_EVENT_LOGOUT;
		memset(messageDb.Buffer,0x00,sizeof(messageDb.Buffer));
		snprintf (messageDb.Buffer, sizeof (messageDb.Buffer), "%s", strLoggedUser);
		SendToMsgQ(DB_MSG_Q_ID,messageDb);

		memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
		/* Receive event logout status from DB task through message queue */
		if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
		{
			memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
		}
		else
		{
			LOGDBG("WEBSERVICE_T : Login Exit",0,0,0);
			return SYSTEM_BUSY;
		}

		if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_EVENT_LOGOUT)
		{
			if(EVENT_LOG_FAIL == atoi(messageDb.Buffer)) 
				return MEMORY_FULL_ALARM;
		}
		else 
		{
			/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
			SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
			LOGDBG("WEBSERVICE_T : Logout Exit",0,0,0);
			return SYSTEM_BUSY;
		}
		
		//Notify the UI for a WS logout just if the UI is logged in
		if(true == CP->bIsHMILogged)
		{	
			WSLogoutNotificationToUI();
		}
		else
		{
			CP->m_RecipeSC.SetUserName("N/A");
			CP->bIsActiveRecipeAvailble = false;
			StartStopWeldIndcation(false);
		}
		
		//Update global variables
		CP->WSLoggedUserName = NO_WS_USER;
		CP->WSUserLevelStatus = NO_LEVEL;
		CP->bIsWebServicesLogged = false;
		
		sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,WS_SUCCESS);
		
		LOGDBG("WEBSERVICE_T : Logout Exit",0,0,0);
		return WS_SUCCESS;
	}
	else
	{
		LOGDBG("WEBSERVICE_T : Logout Exit",0,0,0);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Get the various versions from SC,PC and AC
 * 			  It is called in TO_WEB_SERVICES_TASK_SW_VERSION switch case
 *
 * \param   - Message &messageWeb
 *
 * \return  - WS_STATUS to check HMI connection status
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetSWVersion(Message &messageWeb)
{	
	LOGDBG("WEBSERVICE_T : GetSWVersion Entry",0,0,0);
	char FWVersionSC[BUFFER_LENGTH] 			= { 0x00 };
	char FWVersionPC[BUFFER_LENGTH] 			= { 0x00 };
	char FWVersionAC[BUFFER_LENGTH] 			= { 0x00 };
	char FWVersionUI[BUFFER_LENGTH] 			= { 0x00 };
	USBResponseMsg respMsg;
	Message messageUI;
	UINT32 dwLen = 0;

#ifdef MAINLINE_BUGFIX_BUILD
	
	/* Reading SC Version */
	sprintf(FWVersionSC, "%d.%d.%d.%d",
			FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),
			FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
			FWVersion::Get(FW_VERSION_SC, VERSION_BUILD),
			SC_FW_VERSION_AUTOS);
	
	/* Reading PC Version */
	sprintf(FWVersionPC, "%d.%d.%d.%d",
			FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),
			FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
			FWVersion::Get(FW_VERSION_PC, VERSION_BUILD),
			(((FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR) == 2 && FWVersion::Get(FW_VERSION_PC, VERSION_MINOR) == 0) ? 1 : PC_FW_VERSION_AUTOS)));
	
	/* Reading AC Version */
	sprintf(FWVersionAC, "%d.%d.%d.%d",
			FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
			FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
			FWVersion::Get(FW_VERSION_AC, VERSION_BUILD),
			AC_FW_VERSION_AUTOS);
#else
	/* Reading SC Version */
	sprintf(FWVersionSC, "%d.%d.%d",
			FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),
			FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),
			FWVersion::Get(FW_VERSION_SC, VERSION_BUILD));
	/* Reading PC Version */
	sprintf(FWVersionPC, "%d.%d.%d",
			FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),
			FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),
			FWVersion::Get(FW_VERSION_PC, VERSION_BUILD));
	/* Reading AC Version */
	sprintf(FWVersionAC, "%d.%d.%d",
			FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),
			FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),
			FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
#endif

	/* Reading the UI Version from the UIC if it is connected */
	if(!CP->bIsUIConnected)
	{
		/* UI is not connected So making the UIC version as NA */
		sprintf(messageWeb.Buffer,"{%s:%d, \"%s\": \"%s\", \"%s\": \"%s\", \"%s\": \"%s\", \"%s\": \"NA\"}",
				STATUS_CODE,UI_NOT_CONNECTED,SCVERSION,FWVersionSC,PCVERSION,FWVersionPC,ACVERSION,FWVersionAC,UIVERSION);

		LOGDBG("WEBSERVICE_T : GetSWVersion Exit",0,0,0);
		return WS_SUCCESS;
	}
	respMsg.msgID = UIC_WEBSERVICE_UI_VERSION_REQ;
	strcpy(respMsg.Buffer,UIVERSION);
	respMsg.msglen = strlen(respMsg.Buffer);
	dwLen = sizeof(respMsg.msgID) + sizeof(respMsg.msglen) + respMsg.msglen;

	/* Send UI version read request to UIC */
	Interface = CommunicateInterface::getinstance();
	Interface->Send(reinterpret_cast<char*>(&respMsg), dwLen);

	memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
	/*Receive the UI version from UIC*/  
	if (msgQReceive(WEBSERVICE_MSG_Q_ID, RecvMsgBuffer, MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageUI, RecvMsgBuffer, sizeof(messageUI));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : GetSWVersion Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	if(messageUI.msgID == SCBL_WEBSERVICE_UI_VERSION_RES)
	{
		if(strlen(messageUI.Buffer) > 0)
		{
			strcpy(FWVersionUI,messageUI.Buffer);
			sprintf(messageWeb.Buffer,"{%s:%d, \"%s\": \"%s\", \"%s\": \"%s\", \"%s\": \"%s\", \"%s\": \"%s\"}",STATUS_CODE,WS_SUCCESS,SCVERSION,FWVersionSC,PCVERSION,FWVersionPC,ACVERSION,FWVersionAC,UIVERSION,FWVersionUI);
			LOGDBG("WEBSERVICE_T : GetSWVersion Exit",0,0,0);
			return WS_SUCCESS;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : GetSWVersion Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : GetSWVersion Exit",0,0,0);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Get the last weld result from DB
 * 			  It is called in TO_WEB_SERVICES_TASK_LAST_WELD Switch case 
 *
 * \param   - Message &messageWeb
 *
 * \return  - WS_STATUS to display last weld result or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetLastWeldResult(Message &messageWeb)
{
	LOGDBG("WEBSERVICE_T : GetLastWeldResult Entry",0,0,0);
	int dwIndex = 1;
	char tempBuffer[USERDATA_LENGTH];

	/* Send last weld results request id to DB task through message queue */
	memset(messageDb.Buffer,0x00,sizeof(messageDb.Buffer));
	
	// access database from here
	string strDBData;
	string strSelectQuery;

	strSelectQuery = "select * from WeldResultTable ORDER BY ROWID DESC LIMIT 1";
	strDBData = RetryMechanism ((char *)strSelectQuery.c_str());
	snprintf (messageDb.Buffer, sizeof (messageDb.Buffer), "%s", strDBData.c_str());
	messageDb.msgID = TO_WEB_SERVICES_TASK_LAST_WELD_RESULTS;
	

	/* Check received msgId is related to last weld result or not if yes fill the buffer with proper json format and return WS_SUCCESS
	{ Example “StatusCode”:0,”1”:Value*,”2”:Value*,……”28”:Value*}
	 */
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_LAST_WELD_RESULTS)
	{
		if(strlen(messageDb.Buffer) > 0)
		{
			if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
			{
				LOGDBG("WEBSERVICE_T : GetLastWeldResult Exit",0,0,0);
				return SYSTEM_BUSY;
			}

			sprintf(messageWeb.Buffer,"{%s:%d,",STATUS_CODE,WS_SUCCESS);
			vector<string> WeldData;
			string item;
			stringstream sstream (messageDb.Buffer);
			int itr = 0;
			while(getline(sstream,item,','))
				WeldData.push_back(item);

			for(itr = 0;itr < WeldData.size() - 1;itr++ )
			{
				if((itr == WELDRESULT_DATETIME_INDEX) || (itr == WELDRESULT_USERNAME_INDEX) || (itr == WELDRESULT_PARTID_INDEX) || (itr == WELDRESULT_BATCHID_INDEX))
					sprintf(tempBuffer," \"%d\":\"%s\",",dwIndex,WeldData[itr].c_str());
				
				else if(itr==WELDRESULTS_ENERGY)
				{
					char 	value[10] = {0x00};	
					memset(value, 0x00, sizeof(value));
					int WeldEnergyInMillijoules=atoi(WeldData[itr].c_str());
					int WeldEnergyInTenthsofJoules = (WeldEnergyInMillijoules+ 50)/100;
					/*to avoid truncating the result of the division, we must add "divisor / 2 (100/2=50)"
					 * (dividend + (divisor / 2)) / divisor */
					sprintf(value,"%d",WeldEnergyInTenthsofJoules);
					sprintf(tempBuffer,"\"%d\":%s,",dwIndex,value);
					//strJSONFormat.append(tempBuffer);
				}
				else
					sprintf(tempBuffer," \"%d\":%s,",dwIndex,WeldData[itr].c_str());
				strcat(messageWeb.Buffer,tempBuffer);
				dwIndex++;
			}
			/* Concatenating last element of vector */
			sprintf(tempBuffer," \"%d\":%s",dwIndex,WeldData[itr].c_str());
			strcat(messageWeb.Buffer,tempBuffer);
			strcat(messageWeb.Buffer,"}");
			LOGDBG("WEBSERVICE_T : GetLastWeldResult Exit",0,0,0);
			return WS_SUCCESS;
		}
		else if(strlen(messageDb.Buffer) == 0)
		{
			LOGDBG("WEBSERVICE_T : GetLastWeldResult Exit",0,0,0);
			sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,WS_SUCCESS);
			return WS_SUCCESS;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : GetLastWeldResult Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : GetLastWeldResult Exit",0,0,0);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Updates SC weld recipe parameters with latest data
 * 			  It is called in TO_WEB_SERVICES_TASK_SET_ACTIVE_RECIPE switch case
 *
 * \param   - char * pReceivedQuery,Message &messageWeb
 *
 * \return  - WS_STATUS and display Recipe number which is set as active or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::SetActiveRecipe(char * pReceivedQuery,Message &messageWeb)
{
	LOGDBG("WEBSERVICE_T : SetActiveRecipe Entry",0,0,0);
	int dwRecipeNum = 0;
	char* pRecipeNo = strstr(pReceivedQuery,"\"RecipeNo\":");
	if(pRecipeNo != NULL)
	{
		pRecipeNo = pRecipeNo + strlen("\"RecipeNo\":");
		dwRecipeNum = atoi(pRecipeNo);
	}
	else
		return INVALID_JSON_FORMAT;

	/* Send set active recipe message to DB task */
	messageDb.msgID = TO_DB_TASK_WS_SET_ACTIVE_RECIPE;
	sprintf(messageDb.Buffer,"%d",dwRecipeNum);
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	/* Receive message from DB task */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : SetActiveRecipe Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	if(messageDb.msgID == TO_WEB_SERVICES_TASK_SET_AS_ACTIVE_RECIPE)
	{
		/* Check buffer length */
		if(strlen(messageDb.Buffer) > 0)
		{
			if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
			{
				LOGDBG("WEBSERVICE_T : SetActiveRecipe Exit",0,0,0);
				return SYSTEM_BUSY;
			}
			else if(atoi(messageDb.Buffer) == WS_SUCCESS)
			{
				sprintf(messageWeb.Buffer,"{%s:%d,\"RecipeNo\":%d}",STATUS_CODE,WS_SUCCESS,dwRecipeNum);
				return WS_SUCCESS;
			}
			else if(atoi(messageDb.Buffer) == RECIPE_NOT_FOUND)
				return RECIPE_NOT_FOUND;
			else if(atoi(messageDb.Buffer) == ACTIVE_RECIPE_NOT_SAVED)
				return ACTIVE_RECIPE_NOT_SAVED;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : SetActiveRecipe Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : SetActiveRecipe Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	return SYSTEM_BUSY;
}

/**************************************************************************//**
 * \brief   - Deletes the recipe
 * 			  It is called from ProcessCommandMessage function for command TO_WEB_SERVICES_TASK_DELETE_RECIPE
 *
 * \param   - char * pReceivedQuery, Message &messageWeb
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::DeleteRecipe(char * pReceivedQuery, Message &messageWeb)
{
	int dwRecipeNo = 0;
	char* pRecipeNumber = strstr(pReceivedQuery,"\"RecipeNo\":");
	if(pRecipeNumber != NULL)
	{
		pRecipeNumber = pRecipeNumber + strlen("\"RecipeNo\":");
		dwRecipeNo = atoi(pRecipeNumber);
	}
	else
		return INVALID_JSON_FORMAT;

	if(dwRecipeNo == CP->m_RecipeSC.GetRecipeNumber())		
		return DELETE_RECIPE_FAIL_ACTIVE_RECIPE;

	messageDb.msgID = TO_DB_TASK_WS_DELETE_RECIPE;
	sprintf(messageDb.Buffer,"%d",dwRecipeNo);
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	/* Receive message from DB task */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : DeleteRecipe Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	/* Check received msgId is related to delete recipe or not and return proper status code */
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_DELETE_RECIPE)
	{
		/* Check buffer length */
		if(strlen(messageDb.Buffer) > 0)
		{
			if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
			{
				LOGDBG("WEBSERVICE_T : DeleteRecipe Exit",0,0,0);
				return SYSTEM_BUSY;
			}
			else if(WS_SUCCESS == atoi(messageDb.Buffer))
			{
				sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,WS_SUCCESS);
				return WS_SUCCESS;
			}
			else if(RECIPE_NOT_FOUND == atoi(messageDb.Buffer))
				return RECIPE_NOT_FOUND;
			else
			{
				LOGDBG("WEBSERVICE_T : DeleteRecipe Exit",0,0,0);
				return SYSTEM_BUSY;
			}
		}
		else
		{
			LOGDBG("WEBSERVICE_T : DeleteRecipe Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : DeleteRecipe Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	/* TODO:: Need to write code to add a field in Weldresults table and alarms table and have to set the new field value as yes */
	/* Which indicates as the recipe for that result/alarm was deleted. */
}

/**************************************************************************//**
 * \brief   - Saves the recipe
 * 			  It is called in TO_WEB_SERVICES_TASK_SAVE_RECIPE switch case
 *
 * \param   - Message &messageWeb
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::SaveCurrentRecipe(Message &messageWeb)
{
	if(!CP->bIsActiveRecipeAvailble)
		return RECIPE_NOT_FOUND;

	LOGDBG("WEBSERVICE_T : SaveCurrentRecipe Entry",0,0,0);
	messageDb.msgID = TO_DB_TASK_WS_SAVE_RECIPE;
	strcpy(messageDb.Buffer,"");
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	/* Receive message from DB task */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : SaveCurrentRecipe Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	/* Check received msgId is related to save recipe or not and return proper status code */
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_SAVE_RECIPE)
	{
		/* Check buffer length */
		if(strlen(messageDb.Buffer) > 0)
		{
			if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
			{
				LOGDBG("WEBSERVICE_T : SaveCurrentRecipe Exit",0,0,0);
				return SYSTEM_BUSY;
			}

			if(WS_SUCCESS == atoi(messageDb.Buffer))
			{
				sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,WS_SUCCESS);
				return WS_SUCCESS;
			}
			else if(RECIPE_NOT_FOUND == atoi(messageDb.Buffer))
			{
				return RECIPE_NOT_FOUND;
			}
			else
				return SYSTEM_BUSY;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : SaveCurrentRecipe Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : SaveCurrentRecipe Exit",0,0,0);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Gets particular recipe parameter values converted in metric or imperial 
 *          - based on request received from UI. 
 *
 * \param   - int dwParamId, char *chParamValue, char *chParamValueConverted
 *
 * \return  - bool
 *
 ******************************************************************************/
bool WebServiceTask::GetRecipeParamValueConversion(int dwParamId, char *chParamValue, char *chParamValueConverted)
{
	bool bStatus = true;
	intf_SysConfig_t SysConfigInfo;
	int weldModeIndex;
	
	CP->GetSysConfContext(SysConfigInfo);
	
	if(dwParamId == MODE_VALUE)
	{
		m_RecipePtr->GetParamValueByID(WELD_MODE,weldModeIndex);
	}

	if( dwParamId == TRIGGER_FORCE || dwParamId == HOLD_FORCE || dwParamId == FORCE_STEP1 || dwParamId == FORCE_STEP2
		|| dwParamId == FORCE_STEP3 || dwParamId == FORCE_STEP4 || dwParamId == FORCE_STEP5 || dwParamId == FORCE_STEP6
		|| dwParamId == FORCE_STEP7 || dwParamId == FORCE_STEP8 || dwParamId == FORCE_STEP9 || dwParamId == FORCE_STEP10
		||  dwParamId == SUSPECT_ENDWELDFORCE_LOW_VALUE || dwParamId == SUSPECT_ENDWELDFORCE_HIGH_VALUE
		|| dwParamId == REJECT_ENDWELDFORCE_LOW_VALUE || dwParamId == REJECT_ENDWELDFORCE_HIGH_VALUE )
	{
		float originalValue = atof(chParamValue);
		originalValue = originalValue / 10.0;
		if(SysConfigInfo.UnitIs == IMPERIAL)
		{
			float lbsValue = Util->convertNtolbs(originalValue);
			sprintf(chParamValueConverted,"%.1f",lbsValue);
		}
		else
		{
			int newtonValue = (int)round(originalValue);
			sprintf(chParamValueConverted,"%d",newtonValue);
		}
	}
	else if((dwParamId == MODE_VALUE) && (weldModeIndex == WM_ABSOLUTE_DISTANCE || weldModeIndex == WM_COLLAPSE_DISTANCE))
	{
		if(SysConfigInfo.UnitIs == IMPERIAL)
		{
			float millimeterValue = atof(chParamValue);
			float inchValue = Util->convertmillimetretoinches(millimeterValue);
			sprintf(chParamValueConverted,"%.4f",inchValue);
		}
	}
	else if(dwParamId == SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE || dwParamId == SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE
			|| dwParamId == REJECT_ABSOLUTEDISTANCE_LOW_VALUE || dwParamId == REJECT_ABSOLUTEDISTANCE_HIGH_VALUE
			|| dwParamId == SUSPECT_COLLAPSEDISTANCE_LOW_VALUE || dwParamId == SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE
			|| dwParamId == REJECT_COLLAPSEDISTANCE_LOW_VALUE  || dwParamId == REJECT_COLLAPSEDISTANCE_HIGH_VALUE 
			|| dwParamId == PRETRIGGER_DISTANCE )
	{
		if(SysConfigInfo.UnitIs == IMPERIAL)
		{
			float millimeterValue = atof(chParamValue);
			float inchValue = Util->convertmillimetretoinches(millimeterValue);
			sprintf(chParamValueConverted,"%.4f",inchValue);
		}
	}
	else if(dwParamId == DOWN_ACCELERATION || dwParamId == RETURN_ACCELERATION || dwParamId == DOWN_MAX_VELOCITY
			|| dwParamId == RETURN_MAX_VELOCITY || dwParamId == DOWN_DECELERATION || dwParamId == RETURN_DECELERATION)
	{
		float millimeterValue = atof(chParamValue);
		millimeterValue = millimeterValue / 10000.0;
		if(SysConfigInfo.UnitIs == IMPERIAL)
		{
			float inchValue = Util->convertmillimetretoinches(millimeterValue);
			sprintf(chParamValueConverted,"%.4f",inchValue);
		}
		else
		{
			int mmValueInMetric = (int)millimeterValue;
			sprintf(chParamValueConverted,"%d",mmValueInMetric);
		}
	}
	return bStatus;
}

/**************************************************************************//**
 * \brief   - Gets particular recipe parameter values based on request received from WebCli. 
 * 			  It is called in TO_WEB_SERVICES_TASK_GET_NUM_ALARMS switch case
 *
 * \param   - char * pReceivedQuery, Message &messageWeb
 *
 * \return  - WS_STATUS and display number of alarm value or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetRecipeValue(char * pReceivedQuery,Message &messageWeb)
{
	int weldModeIndex;
	LOGDBG("WEBSERVICE_T : GetRecipeValue Entry",0,0,0);

	if(strlen(pReceivedQuery) > WELDBUFFER_LENGTH)
		sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,MEMORY_FULL_ALARM);

	if(!CP->bIsActiveRecipeAvailble)
		return RECIPE_NOT_FOUND;

	vector <int> vecParamIds;
	vector <int>::const_iterator itr;

	int dwParamId = 0;
	WS_STATUS status;
	char tempBuffer[USERDATA_LENGTH];			
	char *pMultipleValues = strstr(pReceivedQuery,"[");

	if(pMultipleValues == NULL)
	{
		char* pParamId = strstr(pReceivedQuery,"\"ParamId\":");
		char* pEnd = strstr(pReceivedQuery,"}");
		
		if((pEnd - pParamId) > WS_MAX_GET_RECIPE_VALUE_PARAM_DATA_LENGTH)
		{
			return INVALID_JSON_FORMAT;
		}
		
		char chParamBuffer[64]={0};
		memcpy(chParamBuffer,pParamId,pEnd - pParamId);
		char chParamValue[8]={0};
		char chParamValueConverted[8]={0};

		if(GetParamDetails(chParamBuffer,dwParamId,chParamValue))
		{
			status = m_RecipePtr->GetParamValue(dwParamId,chParamValue);
			if(status == WS_SUCCESS)
			{
				GetRecipeParamValueConversion(dwParamId,chParamValue,chParamValueConverted);
				sprintf(messageWeb.Buffer,"{%s:%d,\"ParamId\":%d,\"ParamValue\":%s}",STATUS_CODE,WS_SUCCESS,dwParamId,chParamValueConverted);
			}
			else 
				return status;
		}
		else
		{
			return INVALID_JSON_FORMAT;
		}
	}
	else
	{
		/* For multiple params the format is {"Sid":12345,"RecipeParameters":[{"ParamId":177},{"ParamId":178}]} 
		 * Need to separate them and have to get the data form the recipe structure. 
		 * */
		
		// Does the array has the correct name?
		char *pMultipleParams = strstr(pReceivedQuery, GET_MULTIPLE_PARAMS);
		if (pMultipleParams == NULL)
		{
			return INVALID_JSON_FORMAT;
		}
				
		char *pStartIndex = strstr(pMultipleParams,"{");
		if(!pStartIndex)
		{
			return INVALID_JSON_FORMAT;
		}
		
		sprintf(messageWeb.Buffer,"{%s:%d,\"RecipeParameters\":[",STATUS_CODE,WS_SUCCESS);
		while(pStartIndex)
		{
			char *pEndIndex = strstr(pStartIndex,"}");
			if(!pEndIndex)
			{
				return INVALID_JSON_FORMAT;
			}
			if((pEndIndex - pStartIndex) > WS_MAX_GET_RECIPE_VALUE_PARAM_DATA_LENGTH)
			{
				return INVALID_JSON_FORMAT;
			}
			char chParamBuffer[64]={0};
			char chParamValue[8]={0};
			char chParamValueConverted[8]={0};

			/* Ignoring the brackets while copying the data '{' and '}' */
			memcpy(chParamBuffer,pStartIndex+1,pEndIndex-(pStartIndex+1));
			if(!GetParamDetails(chParamBuffer,dwParamId,chParamValue))
			{
				return INVALID_JSON_FORMAT;
			}
			status = m_RecipePtr->GetParamValue(dwParamId,chParamValue);
			if(status != WS_SUCCESS)
				return status;
			GetRecipeParamValueConversion(dwParamId,chParamValue,chParamValueConverted);
			sprintf(tempBuffer,"{\"ParamId\":%d,\"ParamValue\": %s},",dwParamId,chParamValueConverted);

			strcat(messageWeb.Buffer,tempBuffer);
			pStartIndex = strstr(pEndIndex,"{");
		}
		/* Removing the last byte from the string as it will be ','*/
		messageWeb.Buffer[strlen(messageWeb.Buffer)-1] = 0;
		strcat(messageWeb.Buffer,"]}");
	}

	LOGDBG("WEBSERVICE_T : GetRecipeValue Exit",0,0,0);
	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief - Raise an Event. There are few basic remotely generated events
 * 			we can include some will not have all the details but won't leave
 * 			the log in the dark
 *
 * \param   - LOG_EVENT eventID, void *pData
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::EventForWebServices(LOG_EVENT eventID, void *pData)
{
	std::ostringstream ss;
	std::string details;
	
	switch (eventID)
	{
	case RECIPE_MODIFIED:
    	ss << "Identifier:Active Recipe Modified:From:N/A:To:N/A:";
    	details = ss.str();
		DataBaseManager::EnterEventLog (WEB_SERVICES, details);
		break;
		
		
	default:
		break;
	}
}

/**************************************************************************//**
 * \brief   - Set particular recipe parameter value converted on the basis of system Unit
 * 
 * \param   - int dwParamId, char *chParamValue, char *chParamConvertedValue
 *
 * \return  - bool
 *
 ******************************************************************************/
bool WebServiceTask::SetRecipeParamValueConversion (int dwParamId, char *chParamValue, char *chParamConvertedValue)
{
	bool bStatus = true;
	intf_SysConfig_t SysConfigInfo;
	CP->GetSysConfContext(SysConfigInfo);
	int weldModeIndex;
	if(dwParamId == MODE_VALUE)
	{
		m_RecipePtr->GetParamValueByID(WELD_MODE,weldModeIndex);
	}
	if(dwParamId == TRIGGER_FORCE || dwParamId == HOLD_FORCE || dwParamId == FORCE_STEP1 || dwParamId == FORCE_STEP2
       || dwParamId == FORCE_STEP3 || dwParamId == FORCE_STEP4 || dwParamId == FORCE_STEP5 || dwParamId == FORCE_STEP6
       || dwParamId == FORCE_STEP7 || dwParamId == FORCE_STEP8 || dwParamId == FORCE_STEP9 || dwParamId == FORCE_STEP10
       || dwParamId == SUSPECT_ENDWELDFORCE_LOW_VALUE || dwParamId == SUSPECT_ENDWELDFORCE_HIGH_VALUE
       || dwParamId == REJECT_ENDWELDFORCE_LOW_VALUE || dwParamId == REJECT_ENDWELDFORCE_HIGH_VALUE)
    {
    	float actParamValue = atof(chParamValue);
    	if(SysConfigInfo.UnitIs == IMPERIAL)
    	{
    		actParamValue = Util->convertlbstoN(actParamValue);
    	}
    	int forceValueToStoreIndb = actParamValue*10;
    	sprintf(chParamConvertedValue,"%d",forceValueToStoreIndb);
    }
    else if((dwParamId == MODE_VALUE) && (weldModeIndex == WM_ABSOLUTE_DISTANCE || weldModeIndex == WM_COLLAPSE_DISTANCE))
    {
    	if(SysConfigInfo.UnitIs == IMPERIAL)
    	{
    		float actParamValue = atof(chParamValue);
    		float milliMeterValue = Util->convertInchesToMillimetre(actParamValue);
    		sprintf(chParamConvertedValue,"%.3f",milliMeterValue);
    	}
    }
    else if(dwParamId == SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE || dwParamId == SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE
            || dwParamId == REJECT_ABSOLUTEDISTANCE_LOW_VALUE || dwParamId == REJECT_ABSOLUTEDISTANCE_HIGH_VALUE
    		|| dwParamId == SUSPECT_COLLAPSEDISTANCE_LOW_VALUE || dwParamId == SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE
    		|| dwParamId == REJECT_COLLAPSEDISTANCE_LOW_VALUE  || dwParamId == REJECT_COLLAPSEDISTANCE_HIGH_VALUE 
    		|| dwParamId == PRETRIGGER_DISTANCE)
    {
    	if(SysConfigInfo.UnitIs == IMPERIAL)
    	{
    		float actParamValue = atof(chParamValue);
    		float milliMeterValue = Util->convertInchesToMillimetre(actParamValue);
    		sprintf(chParamConvertedValue,"%.3f",milliMeterValue);
    	}
    }
    else if(dwParamId == DOWN_ACCELERATION || dwParamId == RETURN_ACCELERATION || dwParamId == DOWN_MAX_VELOCITY
    	    || dwParamId == RETURN_MAX_VELOCITY || dwParamId == DOWN_DECELERATION || dwParamId == RETURN_DECELERATION)
    {
    	float actParamValue = atof(chParamValue);
    	if(SysConfigInfo.UnitIs == IMPERIAL)
    	{
    		actParamValue = Util->convertInchesToMillimetre(actParamValue);
    	}
    	int valueToStoreIndb = actParamValue * 10000;
    	sprintf(chParamConvertedValue,"%d",valueToStoreIndb);
   }	
	return bStatus;
}

/**************************************************************************//**
 * \brief   - Sets particular recipe parameter value 
 * 			  It is called in TO_WEB_SERVICES_TASK_GET_NUM_ALARMS switch case
 *
 * \param   - char * pReceivedQuery, Message &messageWeb
 *
 * \return  - WS_STATUS and display number of alarm value or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::SetRecipeValue(char * pReceivedQuery,Message &messageWeb)
{
	LOGDBG("WEBSERVICE_T : SetRecipeValue Entry",0,0,0);

	if(strlen(pReceivedQuery) > WELDBUFFER_LENGTH)
		sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,MEMORY_FULL_ALARM);

	if(!CP->bIsActiveRecipeAvailble)
		return RECIPE_NOT_FOUND;

	int dwParamId = 0;
	WS_STATUS status;
	vector <int> vecParamIds;
	vector <string> vecParamValues;
	vector <int>::const_iterator paramIdsItr;
	vector <string>::const_iterator paramValuesItr;
	double dParamValue = 0;
	char reason[WELDBUFFER_LENGTH] = { 0x00 };

	char *pMultipleValues = strstr(pReceivedQuery,"[");

	if(pMultipleValues == NULL)
	{
		/* The format of the buffer will be {"Sid":12345,"ParamId":177,"ParamValue":0.250,"Reason":"xyz"}
		 * From this we need to extract "ParamId":177,"ParamValue":0.250 and send to GetParamDetails function for the details
		 * */
		char* pParamId = strstr(pReceivedQuery,"\"ParamId\":");
		if(!pParamId)
		{
			return INVALID_JSON_FORMAT;
		}
		
		char* pEnd = strstr(pParamId,"\"Reason\":");
		if(!pEnd)
		{
			return INVALID_JSON_FORMAT;
		}

		char* pReasonEnd = strstr(pEnd,"}");
		if(!pReasonEnd)
		{
			return INVALID_JSON_FORMAT;
		}

		/* To remove the , and " from the string in the end*/
		pEnd = pEnd - 1;
		
		if(((pEnd - pParamId) > WS_MAX_SET_RECIPE_VALUE_PARAM_DATA_LENGTH) || ((pReasonEnd - pEnd) > WELDBUFFER_LENGTH))
		{
			return INVALID_JSON_FORMAT;
		}
		
		char chParamBuffer[64]={0};
		char chParamValue[8]={0};
		char chParamConvertedValue[8] = {0};

		memcpy(chParamBuffer,pParamId,pEnd - pParamId);
		/* To add the , and " which was removed above. Need to copy only the reason so  moving the Reason string a head*/
		pEnd = pEnd + strlen("\"Reason\":") + 1;
		char *pReason = strstr(pEnd,"\"");
		pReasonEnd = strstr(pReason+1,"\"");
		if(!pReasonEnd)
		{
			return INVALID_JSON_FORMAT;
		}
		memcpy(reason,pReason+1,pReasonEnd - (pReason + 1));
		
		if(GetParamDetails(chParamBuffer,dwParamId,chParamValue))
		{
			if(strlen(chParamValue) <= 0)
			{
				return INVALID_JSON_FORMAT;
			}
			SetRecipeParamValueConversion(dwParamId, chParamValue, chParamConvertedValue);
			status = m_RecipePtr->SetParamValue(dwParamId,chParamConvertedValue);
			if(status == WS_SUCCESS)
			{
				EventForWebServices (RECIPE_MODIFIED);

				sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,WS_SUCCESS);
				return WS_SUCCESS;
			}
			else 
				return status;
		}
		else
		{
			return INVALID_JSON_FORMAT;
		}
	}
	else
	{
		/* For multiple params the format is {"Sid":12345,"RecipeValues":[{"ParamId":177,"ParamValue":0.250},{"ParamId":18,"ParamValue":0.250}],"Reason":"xyz"}
		 * Need to separate them and have to get the data form the recipe structure. 
		 * */
		
		// Does the array has the correct name?
		char *pMultipleParams = strstr(pReceivedQuery, SET_MULTIPLE_PARAMS);
		if (pMultipleParams == NULL)
		{
			return INVALID_JSON_FORMAT;
		}
		
		char *pStartIndex = strstr(pMultipleParams,"{");
		if(!pStartIndex)
		{
			return INVALID_JSON_FORMAT;
		}
		
		while(pStartIndex)
		{
			char *pEndIndex = strstr(pStartIndex,"}");
			if(!pEndIndex)
			{
				return INVALID_JSON_FORMAT;
			}
			
			if((pEndIndex - pStartIndex) > WS_MAX_SET_RECIPE_VALUE_PARAM_DATA_LENGTH)
			{
				return INVALID_JSON_FORMAT;
			}

			char chParamBuffer[64]={0};
			char chParamValue[8]={0};

			/* Ignoring the brackets while copying the data '{' and '}' */
			memcpy(chParamBuffer,pStartIndex+1,pEndIndex-(pStartIndex+1));
			
			if(!GetParamDetails(chParamBuffer,dwParamId,chParamValue))
			{
				return INVALID_JSON_FORMAT;
			}
			
			vecParamIds.push_back(dwParamId);
			vecParamValues.push_back(chParamValue);
			pStartIndex = strstr(pEndIndex,"{");
		}
		
		char * pReason = strstr(pReceivedQuery,"\"Reason\":");
		
		if(pReason == NULL)
		{
			return INVALID_JSON_FORMAT;
		}
		else
		{
			int dwDataLength = strlen(pReceivedQuery);
			pReason =  pReason + strlen("\"Reason\":");
			pReason = strstr(pReason,"\"");
			char *pReasonEnd = strstr(pReason+1,"\"");
			memcpy(reason,pReason+1,pReasonEnd - (pReason + 1));
		}
		
		if(((!vecParamIds.empty()) && (!vecParamValues.empty())) && (vecParamIds.size() == vecParamValues.size()))
		{
			paramValuesItr = vecParamValues.begin();
			for(paramIdsItr = vecParamIds.begin();paramIdsItr != vecParamIds.end();paramIdsItr++,paramValuesItr++)
			{
				dwParamId = *paramIdsItr;
				char chParamConvertedValue[8] = {0};
				string strParamValue = *paramValuesItr;
				SetRecipeParamValueConversion(dwParamId, (char*)strParamValue.c_str(), chParamConvertedValue);
				status = m_RecipePtr->SetParamValue(dwParamId,chParamConvertedValue);
				if(status != WS_SUCCESS)
					return status;
				
				EventForWebServices (RECIPE_MODIFIED);
			}

			sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,WS_SUCCESS);
		}	
	}

	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief   - Get the system configuration value
 * 			  It is called in TO_WEB_SERVICES_TASK_GET_SYSTEM_VALUE switch case
 *
 * \param   - char * pReceivedQuery,Message &messageWeb
 *
 * \return  - WS_STATUS and display system configuration value or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetSystemValue(char * pReceivedQuery,Message &messageWeb)
{
	LOGDBG("WEBSERVICE_T : GetSystemConfigValue Entry",0,0,0);
	if(strlen(pReceivedQuery) > 0)
	{
		intf_SysConfig_t  sysConfig;
		int dwParamId = 0;
		char* pParamId = strstr(pReceivedQuery,"\"ParamId\":");
		if(pParamId != NULL)
		{
			pParamId = pParamId + strlen("\"ParamId\":");
			dwParamId = atoi(pParamId);
		}		
		else
			return INVALID_JSON_FORMAT;

		/* Getting the system configuration details from Common Property*/
		CP->GetSysConfContext(sysConfig);

		switch(dwParamId)
		{
		case MEMORY_FULL_ACTION:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%d}",STATUS_CODE,WS_SUCCESS,sysConfig.MemFullActionIs);
			break;

		case LANGUAGUE:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%d}",STATUS_CODE,WS_SUCCESS,sysConfig.LanguageIs);
			break;

		case STARTUP_SCREEN:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%d}",STATUS_CODE,WS_SUCCESS,sysConfig.Sscreen);
			break;

		case RECIPE_PREFIX_FOR_BARCODE:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%c}",STATUS_CODE,WS_SUCCESS,sysConfig.PresetRecipeBarcodeStart);
			break;

		case PART_ID_SWITCH_STATUS:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%d}",STATUS_CODE,WS_SUCCESS,sysConfig.bPartIDSwitchStatus);
			break;

		case AUTHORITY_CHECK_LEVEL:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%d}",STATUS_CODE,WS_SUCCESS,sysConfig.AuthCheckLevel);
			break;

		case UNITS:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%d}",STATUS_CODE,WS_SUCCESS,sysConfig.UnitIs);
			break;

		case PS_POWERUP_OPTION:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%d}",STATUS_CODE,WS_SUCCESS,sysConfig.PsPowerUpOptionIs);
			break;

		case MACHINE_NAME:
			sprintf(messageWeb.Buffer,"{%s:%d,\"ParamValue\":%s}",STATUS_CODE,WS_SUCCESS,sysConfig.machineName);
			break;

		default:
			sprintf(messageWeb.Buffer,"{%s:%d}",STATUS_CODE,MISMATCH_PARAMID);
			break;
		}
		LOGDBG("WEBSERVICE_T : GetSystemConfigValue Exit",0,0,0);
		return WS_SUCCESS;
	}
	else
	{
		LOGDBG("WEBSERVICE_T : GetSystemConfigValue Exit",0,0,0);
		return INVALID_SERVICE_DATA;
	}
}

/**************************************************************************//**
 * \brief   - Get the number of weld history from DB
 * 			  It is called in TO_WEB_SERVICES_TASK_NUMBER_OF_WELDHISTORY switch case
 *
 * \param   - Message &messageWeb
 *
 * \return  - WS_STATUS and display number of weld result value or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetNumberOfWeldHistory(Message &messageWeb)
{
	LOGDBG("WEBSERVICE_T : GetNumberOfWeldHistory Entry",0,0,0);
	/* Send message to DB task */
	messageDb.msgID = TO_DB_TASK_WS_NUMBER_OF_WELDHISTORY;
	int dwRecipeNo = CP->m_RecipeSC.GetRecipeNumber();
	sprintf(messageDb.Buffer,"%d",dwRecipeNo);
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
	/* Receive message from DB task */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : GetNumberOfWeldHistory Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	/* Check received msgId is related to number of weld history or not if yes fill the buffer with proper json format and return WS_SUCCESS*/
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_NUMBER_OF_WELDHISTORY)
	{
		if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
		{
			LOGDBG("WEBSERVICE_T : GetNumberOfWeldHistory Exit",0,0,0);
			return SYSTEM_BUSY;
		}
		/* Check buffer length */
		if(strlen(messageDb.Buffer) > 0)
		{
			sprintf(messageWeb.Buffer,"{%s:%d,\"TotalWeldDataPresent\":%s}",STATUS_CODE,WS_SUCCESS,messageDb.Buffer);	
			LOGDBG("WEBSERVICE_T : GetNumberOfWeldHistory Exit",0,0,0);
			return WS_SUCCESS;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : GetNumberOfWeldHistory Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : GetNumberOfWeldHistory Exit",0,0,0);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Get the weld history for particular range from DB
 * 			  It is called in TO_WEB_SERVICES_TASK_GET_NUM_ALARMS switch case
 *
 * \param   - char * pReceivedQuery, Message &messageWeb
 *
 * \return  - WS_STATUS and display weld history or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetWeldHistory(char * pReceivedQuery,Message &messageWeb)
{
	int dwFrom = 0,dwTo = 0,dwRecipeNo;
	char* pFromData = strstr(pReceivedQuery,"\"From\":");
	WS_STATUS status;
	string strJSONFormat;
	
	if(pFromData != NULL)
	{
		pFromData = pFromData + strlen("\"From\":");
		dwFrom = atoi(pFromData);
	}
	else
		return INVALID_JSON_FORMAT;		

	char* pToData = strstr(pReceivedQuery,"\"To\":");
	if(pToData != NULL)
	{
		pToData = pToData + strlen("\"To\":");
		dwTo = atoi(pToData);
	}
	else
		return INVALID_JSON_FORMAT;


	if(dwFrom == 0 && dwTo == 0)
	{
		/* If both are zero then we have to get the latest MAX_GET_WELD_HISTORY results.
		 * for that we have to pass the same to DBManager. DBManager we will check this
		 * case and diff query will be executed.
		 * */
		
		dwFrom = 0;
		dwTo = 0;
	}
	else if((dwFrom < 0) || (dwTo < 0) || (dwFrom > dwTo))
	{
		return INVALID_SERVICE_DATA;
	}

	// get the entire formatted history
	dwRecipeNo = CP->m_RecipeSC.GetRecipeNumber();
	status = FormatWeldHistory (dwFrom, dwTo, dwRecipeNo, strJSONFormat);
	
	if (status != WS_SUCCESS)
		return status;
	
	// Send messages until all is sent
	int dwBufferSize = strJSONFormat.size();
	int dwBytesSent = 0;
	char *chBuffer = (char*)strJSONFormat.c_str();
	
	while(dwBytesSent < dwBufferSize)
	{
		int dwBytesToSend;
		
		if((dwBufferSize - dwBytesSent) > (MAX_SIZE_OF_MSG_BUFF - 1))
		{
			// TO_WEBCLI_KERNEL_GET_WELD_HISTROY_NEXT;
			dwBytesToSend = MAX_SIZE_OF_MSG_BUFF - 1;//1 is subtracted to leave the last element for the end of string.

			memset(messageDb.Buffer,0x00,sizeof(messageDb.Buffer));
			memcpy(messageDb.Buffer, chBuffer + dwBytesSent, dwBytesToSend);
			
			dwBytesSent = dwBytesSent + dwBytesToSend;

			messageDb.msgID = TO_WEBCLI_KERNEL_GET_WELD_HISTROY_NEXT;
			SendToMsgQ(WEBSERVICEREC_MSG_Q_ID,messageDb);

		}
		else
		{
			// TO_WEBCLI_KERNEL_GET_WELD_HISTROY_FINISH;
			dwBytesToSend = dwBufferSize - dwBytesSent;
			
			memset(messageDb.Buffer,0x00,sizeof(messageDb.Buffer));
			memcpy(messageDb.Buffer, chBuffer + dwBytesSent, dwBytesToSend);
			
			dwBytesSent = dwBytesSent + dwBytesToSend;

			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_WELD_HISTROY_FINISH;
			strcpy(messageWeb.Buffer,messageDb.Buffer);
			
			break;
		}
	}

	LOGDBG("WEBSERVICE_T : GetWeldHistory Exit",0,0,0);
	return WS_SUCCESS;
}


/**************************************************************************//**
 * \brief   -  Reads weld history for particular recipe number within a range
 * 			   of cycles from the DB and frames it into JSON format string.
 *
 * \param   - int dwFrom, int dwTo, int dwRecipeNo, string& strJSONFormat
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::FormatWeldHistory (int dwFrom, int dwTo, int dwRecipeNo, string& strJSONFormat)
{
	char strSelectQuery[MSG_SIZE] = {0x00};
	string queryDBData;
	INT32 dwQueryStatus = 0;
	WS_STATUS status;

	if(dwFrom == 0 && dwTo == 0)
	{
		if (dwRecipeNo > 0)
			sprintf(strSelectQuery, "SELECT * FROM WeldResultTable where RecipeNumber = %d ORDER BY ROWID DESC LIMIT %d;",dwRecipeNo, WS_MAX_GET_WELD_HISTORY);
		else
			sprintf(strSelectQuery, "SELECT * FROM WeldResultTable ORDER BY ROWID DESC LIMIT %d;", WS_MAX_GET_WELD_HISTORY);
	}
	else
	{
		if (dwRecipeNo > 0)
			sprintf(strSelectQuery, "SELECT * FROM WeldResultTable where RecipeNumber = %d and CycleCounter between %d and %d ORDER BY ROWID DESC LIMIT %d;",dwRecipeNo,dwFrom,dwTo,WS_MAX_GET_WELD_HISTORY);
		else
			sprintf(strSelectQuery, "SELECT * FROM WeldResultTable ORDER BY ROWID DESC LIMIT %d;", WS_MAX_GET_WELD_HISTORY);
	}
	
	queryDBData = RetryMechanism ((char *)strSelectQuery);
	
	if(DB_QUERY_FAIL == queryDBData)
	{
		LOGERR("DB_T : GetWeldHistory : ErrCode from DB : %d",dwQueryStatus,0,0); 
		return SYSTEM_BUSY;
	}

	/* Framing result into JSON format */
	WeldHistoryJSONFormat (strJSONFormat, queryDBData);
	status = WS_SUCCESS;
	
	return status; 
}


/**************************************************************************//**
 * \brief   -  Prepares JSON Format string for Weld history
 *
 * \param   - string &strJSONFormat,string &strDBData
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::WeldHistoryJSONFormat (string &strJSONFormat,string &strDBData)
{
	char tempBuffer[USERDATA_LENGTH];
	vector <string>vecWeldData;
	string item;
	sprintf(tempBuffer,"{%s:%d,\"WeldData\":[",STATUS_CODE,WS_SUCCESS);
	strJSONFormat.append(tempBuffer);
	intf_SysConfig_t SysConfigInfo;
	
	if(strDBData.size() > 0)
	{
		stringstream sstream (strDBData.c_str());
		int dwEntireDataIndex = 0,dwSingleRecordDataIndex = 0;
		
		while(getline(sstream,item,','))
			vecWeldData.push_back(item);
		
		int dwIndex = 0,dwCounter = 0;
		CP->GetSysConfContext(SysConfigInfo);
		for(dwEntireDataIndex = 0;dwEntireDataIndex < vecWeldData.size()/NUMBEROF_WELDRESULTS_PARAMETERS;dwEntireDataIndex++ )
		{
			int dwNumberInJSONFormat = 1;
			strJSONFormat.append("{");
			dwIndex = dwCounter * NUMBEROF_WELDRESULTS_PARAMETERS;
			
			for(dwSingleRecordDataIndex = 0 ; dwSingleRecordDataIndex < NUMBEROF_WELDHISTORY_PARAMETERS - 1 ; dwSingleRecordDataIndex++)
			{
				//<! validation added here to check the datetime, username, part ID and batch Id field quotes.
				if((dwSingleRecordDataIndex == WELDRESULT_DATETIME_INDEX)
						|| (dwSingleRecordDataIndex == WELDRESULT_USERNAME_INDEX)
						|| (dwSingleRecordDataIndex == WELDRESULT_PARTID_INDEX)
						|| (dwSingleRecordDataIndex == WELDRESULT_BATCHID_INDEX))
				{
				    sprintf(tempBuffer, " \"%d\":\"%s\",",dwNumberInJSONFormat,vecWeldData[dwIndex].c_str());
				    strJSONFormat.append(tempBuffer);
				}
				
				else if(dwSingleRecordDataIndex == WELDRESULTS_ENERGY)
				{
					char value[10] = {0x00};
					memset(value, 0x00, sizeof(value));
					int WeldEnergyInMillijoules=atoi(vecWeldData[dwIndex].c_str());
					int WeldEnergyInTenthsofJoules = (WeldEnergyInMillijoules+ 50)/100;
					/*to avoid truncating the result of the division, we must add "divisor / 2 (100/2=50)"
					 * (dividend + (divisor / 2)) / divisor */
					
					sprintf(value,"%d",WeldEnergyInTenthsofJoules);
					sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,value);
					strJSONFormat.append(tempBuffer);
				}
				else if(dwSingleRecordDataIndex == MAX_WELD_FORCE || dwSingleRecordDataIndex == END_HOLD_FORCE)
				{
					char value[10] = {0x00};
					memset(value, 0x00, sizeof(value));
					int forceValue = atoi(vecWeldData[dwIndex].c_str());
					float tmpConvert =  forceValue /10.0;
					if(SysConfigInfo.UnitIs == IMPERIAL)
					{
						tmpConvert = Util->convertNtolbs(tmpConvert);
						sprintf(value,"%.1f",tmpConvert);
						sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,value);
						strJSONFormat.append(tempBuffer);
					}
					else
					{
						int newtonValue = (int)round(tmpConvert);
						sprintf(value,"%d",newtonValue);
						sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,value);
						strJSONFormat.append(tempBuffer);
					}
				}
				else if(dwSingleRecordDataIndex == WELD_ABSOLUTE || dwSingleRecordDataIndex == TOTAL_ABSOLUTE || dwSingleRecordDataIndex == WELD_COLLAPSE_DISTANCE
				    || dwSingleRecordDataIndex == HOLD_COLLAPSE_DISTANCE || dwSingleRecordDataIndex == TOTAL_COLLAPSE_DISTANCE ||dwSingleRecordDataIndex == TRIGGER_DISTANCE)
				{
					char value[10] = {0x00};
					memset(value, 0x00, sizeof(value));
					float distInMicroMtr = atof(vecWeldData[dwIndex].c_str());
					if(SysConfigInfo.UnitIs == IMPERIAL)
					{
						float tmpConvert = Util->convertMicrometerToInches(distInMicroMtr);
						sprintf(value,"%.4f",tmpConvert);
						sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,value);
						strJSONFormat.append(tempBuffer);
					}
					else
					{
						float tmpConvert = Util->convertMicrometerToMillimeter(distInMicroMtr);
						sprintf(value,"%.3f",tmpConvert);
						sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,value);
						strJSONFormat.append(tempBuffer);
					}
				}
				else if(dwSingleRecordDataIndex == WELD_VELOCITY)
				{
					char value[10] = {0x00};
					memset(value, 0x00, sizeof(value));
					float velInMMPerSec = atof(vecWeldData[dwIndex].c_str());
					if(SysConfigInfo.UnitIs == IMPERIAL)
					{
						float tmpConvert = Util->convertMMPerSecToInchPerSec(velInMMPerSec);
						sprintf(value,"%.4f",tmpConvert);
						sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,value);
						strJSONFormat.append(tempBuffer);
					}
					else
					{
						sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,vecWeldData[dwIndex].c_str());
						strJSONFormat.append(tempBuffer);
					}
				}
				else if(dwSingleRecordDataIndex == WELD_TIME || dwSingleRecordDataIndex == CYCLE_TIME)
				{
					char value[10] = {0x00};
					memset(value, 0x00, sizeof(value));
					float timeInMsec = atof(vecWeldData[dwIndex].c_str());
					float tmpConvert = Util->convertMsecToSec(timeInMsec);
					sprintf(value,"%.3f",tmpConvert);
					sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,value);
					strJSONFormat.append(tempBuffer);
				}
				else
				{
					sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,vecWeldData[dwIndex].c_str());
					strJSONFormat.append(tempBuffer);
				}
				dwNumberInJSONFormat++;
				dwIndex++;
			}
			
			sprintf(tempBuffer,"\"%d\":%s",dwNumberInJSONFormat,vecWeldData[dwIndex].c_str());
			strJSONFormat.append(tempBuffer);

			dwCounter++;
			strJSONFormat.append("}");
			
			if(dwEntireDataIndex < ((vecWeldData.size()/NUMBEROF_WELDRESULTS_PARAMETERS) - 1))
				strJSONFormat.append(",");
		}
	}
	
	strJSONFormat.append("]}");
}


/**************************************************************************//**
 * \brief   - Get the number of alarms from DB
 * 			  It is called in TO_WEB_SERVICES_TASK_GET_NUM_ALARMS switch case
 *
 * \param   - Message &messageWeb
 *
 * \return  - WS_STATUS and display number of alarm value or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetNumberOfAlarms(Message &messageWeb)
{
	LOGDBG("WEBSERVICE_T : GetNumberOfAlarms Entry",0,0,0);
	/* Send number of alarms request id to DB task through message queue */
	messageDb.msgID = TO_DB_TASK_WS_NUMBER_OF_ALARMS;
	strcpy(messageDb.Buffer,""); 
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
	/* Receive number of alarms from DB task through message queue */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : GetNumberOfAlarms Exit",0,0,0);
		return SYSTEM_BUSY;
	}

	/* Check received msgId is related to number of alarms or not if yes fill the buffer with proper json format and return WS_SUCCESS*/
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_NUMBER_OF_ALARMS)
	{
		if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
		{
			LOGDBG("WEBSERVICE_T : GetNumberOfAlarms Exit",0,0,0);
			return SYSTEM_BUSY;
		}
		/* Check buffer length */
		if(strlen(messageDb.Buffer) > 0)
		{
			sprintf(messageWeb.Buffer,"{%s:%d,\"TotalAlarmPresent\":%s}",STATUS_CODE,WS_SUCCESS,messageDb.Buffer);	
			LOGDBG("WEBSERVICE_T : GetNumberOfAlarms Exit",0,0,0);
			return WS_SUCCESS;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : GetNumberOfAlarms Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : GetNumberOfAlarms Exit",0,0,0);
		return SYSTEM_BUSY;
	}
}

/**************************************************************************//**
 * \brief   - Get the alarm log for particular range from DB
 * 			  It is called in TO_WEB_SERVICES_TASK_GET_NUM_ALARMS switch case
 *
 * \param   - char * pReceivedQuery, Message &messageWeb
 *
 * \return  - WS_STATUS and display alarm log or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetAlarmLog(char * pReceivedQuery,Message &messageWeb)
{
	int dwFrom = 0,dwTo = 0;
	char* pFromData = strstr(pReceivedQuery,"\"From\":");
	if(pFromData != NULL)
	{
		pFromData = pFromData + strlen("\"From\":");
		dwFrom = atoi(pFromData);
	}
	else
		return INVALID_JSON_FORMAT;

	char* pToData = strstr(pReceivedQuery,"\"To\":");
	if(pToData != NULL)
	{
		pToData = pToData + strlen("\"To\":");
		dwTo = atoi(pToData);
	}
	else
		return INVALID_JSON_FORMAT;

	
	if(dwFrom == 0 && dwTo == 0)
	{
		/* If both are zero then we have to get the latest MAX_GET_ALARM_HISTORY results.
		 * for that we have to pass the same to DBManager. DBManager we will check this
		 * case and diff query will be executed.
		 * */
		
		dwFrom = 0;
		dwTo = 0;
	}
	else if((dwFrom < 0) || (dwTo < 0) || (dwFrom > dwTo) || ((dwTo - dwFrom) > WS_MAX_GET_ALARM_HISTORY))
	{
		return INVALID_SERVICE_DATA;
	}

	messageDb.msgID = TO_DB_TASK_WS_GET_ALARM_LOG;
	sprintf(messageDb.Buffer,"%d,%d",dwFrom,dwTo);
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	bool bContext = true;
	while(1)
	{
		memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
		if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
		{
			if(bContext)
				memset(&messageDb,0x00,sizeof(messageDb));
				memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
		}
		else
		{
			LOGDBG("WEBSERVICE_T : GetAlarmLog Exit, SYSTEM_BUSY",0,0,0);
			return SYSTEM_BUSY;
		}

		if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_GET_ALARM_LOG_NEXT)
		{
			if(bContext)
			{
				messageDb.msgID = TO_WEBCLI_KERNEL_GET_ALARM_LOG_NEXT;
				SendToMsgQ(WEBSERVICEREC_MSG_Q_ID,messageDb);
			}
		}
		else if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_GET_ALARM_LOG_FINISH)
		{
			if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
			{
				LOGDBG("WEBSERVICE_T : GetAlarmLog Exit",0,0,0);
				return SYSTEM_BUSY;
			}
			if(bContext)
			{
				messageWeb.msgID = TO_WEBCLI_KERNEL_GET_ALARM_LOG_FINISH;
				strcpy(messageWeb.Buffer,messageDb.Buffer);
			}
			break;
		}
		else
		{
			/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message
			 ** New command received from the Web client so canceling the current request.
			 ** So ignoring the rest of the data and later send the new request to process command */
			LOGERR("WEBSERVICE_T : GetAlarmLog Command got canceled and a new command received,",0,0,0);
			bContext = false;
		}
	}
	if(!bContext)
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);

	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief   - 	Get the graph results for corresponding recipe number and
 * 				cycle counter from DB It is called in 
 * 				TO_WEB_SERVICES_TASK_GET_GRAPH_RESULT switch case
 *
 * \param   - char * pReceivedQuery, Message &messageWeb
 *
 * \return  - WS_STATUS and display graph results or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetGraphResults(char * pReceivedQuery,Message &messageWeb)
{
	int dwRecipeNo, dwCycleCounter;
	INT32 dwQueryStatus = 0;
	string strDBData, strJSONFormat;
	string strRecipeNumber="",strCycleCnt="";
	char strSelectQuery[MSG_SIZE] = {0x00};
	char *DelimeterPoint;
	char* pCycleCounterData = strstr(pReceivedQuery,"\"CycleCounter\":");
	char* pRecipeNoData = strstr(pReceivedQuery,"\"RecipeNo\":");
		
	/*	Modified to fix bug 7275 - To handle Wrong Parameter Name in web service request from Postman Utility */ 
	if((pCycleCounterData != NULL) && (pRecipeNoData != NULL))
	{
		//Get the pointers to the CycleCounter and RecipeNo values
		pCycleCounterData = pCycleCounterData + strlen("\"CycleCounter\":");
		pRecipeNoData = pRecipeNoData + strlen("\"RecipeNo\":");
		
		//Get the CycleCounter value
		DelimeterPoint = strstr(pCycleCounterData,",");
		strCycleCnt.append(pCycleCounterData,DelimeterPoint - pCycleCounterData);
		dwCycleCounter = atoi(strCycleCnt.c_str());
		
		//Get the RecipeNo value
		DelimeterPoint = strstr(pRecipeNoData,"}");
		strRecipeNumber.append(pRecipeNoData,DelimeterPoint - pRecipeNoData);
		dwRecipeNo = atoi(strRecipeNumber.c_str());
		
		if ((dwRecipeNo <= 0) || (dwCycleCounter <= 0))
			return INVALID_SERVICE_DATA;
	}
	else
		return INVALID_JSON_FORMAT;

	memset(messageDb.Buffer,0x00,sizeof(messageDb.Buffer));
	sprintf(strSelectQuery, "SELECT * FROM WeldResultSignature where CycleCounter=%d and RecipeNumber=%d;",dwCycleCounter,dwRecipeNo);
	strDBData = RetryMechanism((char *)strSelectQuery);

	//<! Modified for bug Id 8085 to validate cycle counter and recipe no.
	if ((strDBData == "") || (strDBData == DB_QUERY_FAIL))
	{
		LOGERR("DB_T : GetGraphResults : ErrCode from DB : %d",dwQueryStatus,0,0);
		return DATA_NOT_FOUND_IN_SPECIFIEDRANGE;
	}
	
	//Framing result into JSON format
	WeldGraphJSONFormat(strJSONFormat,strDBData,dwCycleCounter,dwRecipeNo);
	
	int dwBufferSize = strJSONFormat.size();
	int dwBytesSent = 0;
	char *chBuffer = (char*)strJSONFormat.c_str();
	
	while(dwBytesSent < dwBufferSize)
	{
		int dwBytesToSend;
		memset(messageDb.Buffer,0x00,sizeof(messageDb.Buffer));
		
		if((dwBufferSize - dwBytesSent) > (MAX_SIZE_OF_MSG_BUFF - 1))
		{
			messageDb.msgID = TO_WEBCLI_KERNEL_GET_GRAPH_RESULT_NEXT;
			dwBytesToSend = MAX_SIZE_OF_MSG_BUFF - 1; //1 is subtracted to leave the last element for the end of string.
			
			memcpy(messageDb.Buffer,chBuffer+dwBytesSent,dwBytesToSend);
			SendToMsgQ(WEBSERVICEREC_MSG_Q_ID,messageDb);
		}
		else
		{
			messageWeb.msgID = TO_WEBCLI_KERNEL_GET_GRAPH_RESULT_FINISH;
			dwBytesToSend = dwBufferSize - dwBytesSent;
			
			memset(messageWeb.Buffer,0x00,sizeof(messageWeb.Buffer));
			memcpy(messageWeb.Buffer,chBuffer+dwBytesSent,dwBytesToSend);
		}

		dwBytesSent = dwBytesSent + dwBytesToSend;
	}

	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief   - Get the Runtime details of Web Services and FIrmware over Ethernet and send them to Kernel
 *
 * \param   - Message &messageWeb
 *
 * \return  - WS_STATUS and display 0 or 1 in the message for 2 params mentioned above in 0,0 format or error codes
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::GetRuntimeDetails(Message &messageWeb)
{
	LOGDBG("WEBSERVICE_T : GetRuntimeDetails Entry",0,0,0);
	/* Send Request to DB task to get the Web Services runtime details from the DB through message queue */
	messageDb.msgID = TO_DB_TASK_WS_GET_RUNTIME_DETAILS;
	strcpy(messageDb.Buffer,""); 
	SendToMsgQ(DB_MSG_Q_ID,messageDb);
	memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
	/* Receive Runtime details of Web Services and firmware over Ethernet */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : GetRuntimeDetails Exit",0,0,0);
		return SYSTEM_BUSY;
	}
	/* Check received msgId is related to Runtime details or not if yes fill the buffer and return WS_SUCCESS*/
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_WS_GET_RUNTIME_DETAILS)
	{
		if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
		{
			LOGDBG("WEBSERVICE_T : GetRuntimeDetails Exit",0,0,0);
			return SYSTEM_BUSY;
		}
		/* Check buffer length */
		if(strlen(messageDb.Buffer) > 0)
		{
			strcpy(messageWeb.Buffer,messageDb.Buffer);
			LOGDBG("WEBSERVICE_T : GetRuntimeDetails Exit",0,0,0);
			return WS_SUCCESS;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : GetRuntimeDetails Exit",0,0,0);
			return SYSTEM_BUSY;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : GetRuntimeDetails Exit",0,0,0);
		return SYSTEM_BUSY;
	}	
}

WS_STATUS WebServiceTask::GetAllUserDetails(Message &messageWeb)
{
	/* Only for this feature case we wont have any error case as the actual validation happens on kernel side the error will be calculated there.
	** For the failures we have to send the EMPTY BUFFER indicating failure to Kernel*/
	LOGDBG("WEBSERVICE_T : GetAllUserDetails Entry",0,0,0);
	
	/* Send number of alarms request id to DB task through message queue */
	messageDb.msgID = TO_DB_TASK_WS_GET_ALL_USER_DETAILS;
	memset(messageWeb.Buffer,0x00,sizeof(messageWeb.Buffer));
	strcpy(messageDb.Buffer,""); 
	SendToMsgQ(DB_MSG_Q_ID,messageDb);

	memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
	/* Receive number of alarms from DB task through message queue */
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&messageDb, RecvMsgBuffer, sizeof(messageDb));
	}
	else
	{
		LOGDBG("WEBSERVICE_T : GetAllUserDetails Exit",0,0,0);
		return WS_SUCCESS;
	}

	/* Check received msgId is related to number of alarms or not if yes fill the buffer with proper json format and return WS_SUCCESS*/
	if(messageDb.msgID == TO_WEB_SERVICES_TASK_GET_ALL_USER_DETAILS)
	{
		if(!strcmp(messageDb.Buffer,DB_QUERY_FAIL))
		{
			LOGDBG("WEBSERVICE_T : GetAllUserDetails Exit",0,0,0);
			return WS_SUCCESS;
		}
		/* Check buffer length */
		if(strlen(messageDb.Buffer) > 0)
		{
			strcpy(messageWeb.Buffer,messageDb.Buffer); 
			LOGDBG("WEBSERVICE_T : GetAllUserDetails Exit",0,0,0);
			return WS_SUCCESS;
		}
		else
		{
			LOGDBG("WEBSERVICE_T : GetAllUserDetails Exit",0,0,0);
			return WS_SUCCESS;
		}
	}
	else
	{
		/* Expected message is not received and it may be another request from web client. So, redirecting the message received to process message */
		SendToMsgQ(WEBSERVICE_MSG_Q_ID,messageDb);
		LOGDBG("WEBSERVICE_T : GetAllUserDetails Exit",0,0,0);
		return WS_SUCCESS;
	}
}


/**************************************************************************//**
 * \brief  - opens DB connection, per WS requests.
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::ConnectDB()
{
	m_db.EstablishDataBaseConnection();
}

/**************************************************************************//**
 * \brief  - closes DB connection, per WS requests.
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::CloseDB()
{
	m_db.Close();
}

/**************************************************************************//**
 * \brief  - Retries database access where implemented, per WS requests.
 *
 * \param  - char* sqlQuery
 *
 * \return  - string retry_response
 *
 ******************************************************************************/
std::string WebServiceTask::RetryMechanism (char* sqlQuery)
{
	int queryStatus 	= 1;
	static std::string retry_response;
	
	retry_response.clear();
	
	retry_response = m_db.Exec(sqlQuery, &queryStatus, RETRY_COUNT_FOR_ONE_FIFTY_MILLISEC_DELAY);
	
	if(SQLITE_OK != queryStatus)
	{
		LOGERR("WebServiceTask::RetryMechanism: query failed ErrCode : %d",queryStatus,0,0);
		retry_response = DB_QUERY_FAIL;
	}
	
	return retry_response;
}

/**************************************************************************//**
 * \brief   -  Prepares JSON Format string for Weld graph results
 *
 * \param   - string &strJSONFormat,string &strDBData,INT32 dwCycleCounter,INT32 dwRecipeNo
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::WeldGraphJSONFormat(string &strJSONFormat,string &strDBData,INT32 dwCycleCounter,INT32 dwRecipeNo)
{
	char tempBuffer[USERDATA_LENGTH];
	vector <string>vecWeldData,VecTime,vecFreq,vecPower,vecCurrent,vecAmp,vecPhase,vecEnergy,vecForce,vecVelocity,vecAD,vecCD;
	string item;
	sprintf(tempBuffer,"{%s:%d,",STATUS_CODE,WS_SUCCESS);
	strJSONFormat.append(tempBuffer);
	int dwEntireDataIndex = 0,dwSingleRecordDataIndex = 0;
	intf_SysConfig_t SysConfigInfo;

	CP->GetSysConfContext(SysConfigInfo);

	if(strDBData.size() > 0)
	{
		stringstream sstream (strDBData.c_str());

		while(getline(sstream,item,','))
			vecWeldData.push_back(item);

		/* Appending RecipeNo,RecipeVerNum and Cycle in JSON format to string from vector data */
		sprintf(tempBuffer,"\"RecipeNo\":%s,",vecWeldData[0].c_str());
		strJSONFormat.append(tempBuffer);
		sprintf(tempBuffer,"\"RecipeVerNum\":%s,",vecWeldData[1].c_str());
		strJSONFormat.append(tempBuffer);
		sprintf(tempBuffer,"\"Cycle\":%s,",vecWeldData[2].c_str());
		strJSONFormat.append(tempBuffer);

		/* Erasing RecipeNo,RecipeVerNum and Cycle from vector data */
		vecWeldData.erase(vecWeldData.begin());
		vecWeldData.erase(vecWeldData.begin());
		vecWeldData.erase(vecWeldData.begin());

		/* Storing respective parametr values from vector data to individual vectors */
		for(dwEntireDataIndex = 0;dwEntireDataIndex<vecWeldData.size();dwEntireDataIndex++)
		{
			if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == TIME_VALUES)
				VecTime.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == FREQUENCY_VALUES)
				vecFreq.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == POWER_VALUES)
				vecPower.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == CURRENT_VALUES)
				vecCurrent.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == AMPLITUDE_VALUES)
				vecAmp.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == ENERGY_VALUES)
				vecPhase.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == PHASE_VALUES)
				vecEnergy.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == FORCE_VALUES)
				vecForce.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == VELOCITY_VALUES)
				vecVelocity.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == ABSDISTANCE_VALUES)
				vecAD.push_back(vecWeldData[dwEntireDataIndex]);
			else if(dwEntireDataIndex % NUMBEROF_WELDGRAPH_PARAMETERS == COLDISTANCE_VALUES)
				vecCD.push_back(vecWeldData[dwEntireDataIndex]);
		}
	}
	else
	{
		sprintf(tempBuffer,"\"RecipeNo\":%d,",dwRecipeNo);
		strJSONFormat.append(tempBuffer);
		sprintf(tempBuffer,"\"RecipeVerNum\":%d,",0);
		strJSONFormat.append(tempBuffer);
		sprintf(tempBuffer,"\"Cycle\":%d,",dwCycleCounter);
		strJSONFormat.append(tempBuffer);
	}
	/* Appending Time,Frequency,Power,Current,Amplitude,Energy,Phase,Force,Velocity,AbsDistance and ColDistance in JSON format to string from individual vectors */
	strJSONFormat.append("\"Time\":[");
	if(VecTime.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<VecTime.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != VecTime.size() - 1)
			{
				char value[10] = {0x00};
				memset(value, 0x00, sizeof(value));
				float timeInMsec = atof(VecTime[dwSingleRecordDataIndex].c_str());
				float timeInSec = Util->convertMsecToSec(timeInMsec);
				sprintf(value,"%.3f",timeInSec);
				sprintf(tempBuffer,"%s,",value);
			}
			else
				sprintf(tempBuffer,"%s",VecTime[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Frequency\":[");
	if(vecFreq.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecFreq.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecFreq.size() - 1)
				sprintf(tempBuffer,"%s,",vecFreq[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecFreq[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Power\":[");
	if(vecPower.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecPower.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecPower.size() - 1)
				sprintf(tempBuffer,"%s,",vecPower[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecPower[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Current\":[");
	if(vecCurrent.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecCurrent.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecCurrent.size() - 1)
				sprintf(tempBuffer,"%s,",vecCurrent[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecCurrent[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Amplitude\":[");
	if(vecAmp.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecAmp.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecAmp.size() - 1)
				sprintf(tempBuffer,"%s,",vecAmp[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecAmp[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Energy\":[");
	if(vecEnergy.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecEnergy.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecEnergy.size() - 1)
				sprintf(tempBuffer,"%s,",vecEnergy[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecEnergy[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Phase\":[");
	if(vecPhase.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecPhase.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecPhase.size() - 1)
				sprintf(tempBuffer,"%s,",vecPhase[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecPhase[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Force\":[");
	if(vecForce.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecForce.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecForce.size() - 1)
			{
				char value[10] = {0x00};
				memset(value, 0x00, sizeof(value));
				float forceIndb = atof(vecForce[dwSingleRecordDataIndex].c_str());
				forceIndb = forceIndb /10.0;
				if(SysConfigInfo.UnitIs == IMPERIAL)
				{
					float tmpConvert = Util->convertNtolbs(forceIndb);
					sprintf(value,"%.1f",tmpConvert);
				}
				else
				{
					int tmpConvert = (int)round(forceIndb);
					sprintf(value,"%d",tmpConvert);
				}
				sprintf(tempBuffer,"%s,",value);
			}
			else
			{
				sprintf(tempBuffer,"%s",vecForce[dwSingleRecordDataIndex].c_str());
			}
			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Velocity\":[");
	if(vecVelocity.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecVelocity.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecVelocity.size() - 1)
			{
				char value[10] = {0x00};
				memset(value, 0x00, sizeof(value));
				float velInMMPerSec = atof(vecVelocity[dwSingleRecordDataIndex].c_str());
				if(SysConfigInfo.UnitIs == IMPERIAL)
				{
					float tmpConvert = Util->convertMMPerSecToInchPerSec(velInMMPerSec);
					sprintf(value,"%.4f",tmpConvert);
					sprintf(tempBuffer,"%s,",value);
				}
				else
				{
					sprintf(tempBuffer,"%s,",vecVelocity[dwSingleRecordDataIndex].c_str());
				}
			}
			else
			{
				sprintf(tempBuffer,"%s",vecVelocity[dwSingleRecordDataIndex].c_str());
			}

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"AbsDistance\":[");
	if(vecAD.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecAD.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecAD.size() - 1)
			{
				char value[10] = {0x00};
				memset(value, 0x00, sizeof(value));
				float distInMicroMtr = atof(vecAD[dwSingleRecordDataIndex].c_str());
				if(SysConfigInfo.UnitIs == IMPERIAL)
				{
					float tmpConvert = Util->convertMicrometerToInches(distInMicroMtr);
					sprintf(value,"%.4f",tmpConvert);
				}
				else
				{
					float tmpConvert = Util->convertMicrometerToMillimeter(distInMicroMtr);
					sprintf(value,"%.3f",tmpConvert);
				}
				sprintf(tempBuffer,"%s,",value);
			}
			else
			{
				sprintf(tempBuffer,"%s",vecAD[dwSingleRecordDataIndex].c_str());
			}
			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"ColDistance\":[");
	if(vecCD.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecCD.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecCD.size() - 1)
			{
				char value[10] = {0x00};
				memset(value, 0x00, sizeof(value));
				float distInMicroMtr = atof(vecCD[dwSingleRecordDataIndex].c_str());
				if(SysConfigInfo.UnitIs == IMPERIAL)
				{
					float tmpConvert = Util->convertMicrometerToInches(distInMicroMtr);
					sprintf(value,"%.4f",tmpConvert);
				}
				else
				{
					float tmpConvert = Util->convertMicrometerToMillimeter(distInMicroMtr);
					sprintf(value,"%.3f",tmpConvert);
				}
				sprintf(tempBuffer,"%s,",value);
			}
			else
			{
				sprintf(tempBuffer,"%s",vecCD[dwSingleRecordDataIndex].c_str());
			}
			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("]}");
}

/**************************************************************************//**
 * \brief   -  Request the UI for a webservices login by sending the actual 
 * 			   username attempting to login and its level.
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
WS_STATUS WebServiceTask::WSLoginRequestToUI(char* UserName, UINT8 iUserLevel)
{
	Message message;
	char chUserNameAndLevel[BUFFER_LENGTH] = {0x00};
	char UserLevel[USERDATA_LENGTH] = {0x00};
	
	sprintf(UserLevel, "%d", iUserLevel);
	
	strcat(chUserNameAndLevel, UserName);
	strcat(chUserNameAndLevel, ",");
	strcat(chUserNameAndLevel, UserLevel);
	strcat(chUserNameAndLevel, ",");
	strcat(chUserNameAndLevel, LOG_IN);
	
	memset(&message, 0x00, sizeof(message));
	message.msgID = TO_UI_TASK_WEBSERVICE_USER_LOGGED_INFO;					
	memcpy(message.Buffer, chUserNameAndLevel, sizeof(chUserNameAndLevel));
	SendToMsgQ(UI_MSG_Q_ID,message);
	
	//Wait for response of the request
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR) 
	{
		memcpy(&message, RecvMsgBuffer, sizeof(message));
		
		if((message.msgID == SCBL_WS_LOGIN_PERMISSION_RES) && (strlen(message.Buffer) > 0))
		{
			//If permission is granted returns WS_SUCCESS
			if(LOGIN_ALLOWED == atoi(message.Buffer))
				return WS_SUCCESS;
			else if(LOGIN_DENIED == atoi(message.Buffer))
				return INVALID_UI_SCREEN;
		}
	}
	
	return SYSTEM_BUSY;
}

/**************************************************************************//**
 * \brief   -  Notifies the UI that there was a webservices logout, by sending
 * 			   "WS_N/A" as the actual WS user and NO_LEVEL (0) as its level.
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void WebServiceTask::WSLogoutNotificationToUI()
{
	Message message;
	char chUserNameAndLevel[BUFFER_LENGTH] = {0x00};
	char chUserLevel[USERDATA_LENGTH] = {0x00};
	
	sprintf(chUserLevel, "%d", NO_LEVEL);
	
	strcat(chUserNameAndLevel, NO_WS_USER);
	strcat(chUserNameAndLevel, ",");
	strcat(chUserNameAndLevel, chUserLevel);
	strcat(chUserNameAndLevel, ",");
	strcat(chUserNameAndLevel, LOG_OUT);
	
	memset(&message, 0x00, sizeof(message));
	message.msgID = TO_UI_TASK_WEBSERVICE_USER_LOGGED_INFO;					
	memcpy(message.Buffer, chUserNameAndLevel, sizeof(chUserNameAndLevel));
	SendToMsgQ(UI_MSG_Q_ID,message);
}

/**************************************************************************//**
* \brief  - Gets the IO Configuration from the DB and sends it to the Control
* 			Task to be stored as the current IO Configuration.
*
* \param  - NONE
*
* \return - WS_STATUS
*
******************************************************************************/
WS_STATUS WebServiceTask::WSSetUserIOInit()
{
	Message message;

	//Get IO from DB
	memset(&message, 0x00, sizeof(message));
	message.msgID = TO_DB_TASK_WS_GET_IO_CONFIGURATION;					
	SendToMsgQ(DB_MSG_Q_ID,message);
	
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR)
	{
		memcpy(&message, RecvMsgBuffer, sizeof(message));
		
		/* Check if received Message ID is for IO Configuration */
		if(TO_WEB_SERVICES_TASK_GET_IO_CONFIGURATION == message.msgID)
		{
			//Set USERIO format to the IO Configuration from DB
			WSSetUserIOFormat(message);
			
			// Send IOConfig to Control Task
			message.msgID = WS_TO_CTRL_TASK_USERIO_READ_REQ;
			SendToMsgQ(CTRL_MSG_Q_ID,message);
			
			if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR)
			{
				memcpy(&message, RecvMsgBuffer, sizeof(message));
				
				if((TO_WEB_SERVICES_TASK_USERIO_READ_RES == message.msgID) && (RESPONSE_SUCCESS == atoi(message.Buffer)))
				{
					return WS_SUCCESS;
				}
				else
					LOGDBG("WEBSERVICE_T : COULD NOT SET IO CONFIGURATION",0,0,0);
			}
			else
				LOGDBG("WEBSERVICE_T : SYSTEM BUSY",0,0,0);
		}
		else
			LOGDBG("WEBSERVICE_T : SYSTEM BUSY",0,0,0);
	}
	else
		LOGDBG("WEBSERVICE_T : COULD NOT GET IO CONFIGURATION",0,0,0);
	
	return SYSTEM_BUSY;
}

/**************************************************************************//**
* \brief  - Gives format to the IO Configuration string from the DB and stores
* 			it on a USERIO type variable.
*
* \param  - IOConfigMessage - This variable contains the IO Configuration
* 							  string from the DB.
*
* \return - NONE
*
******************************************************************************/
void WebServiceTask::WSSetUserIOFormat(Message& IOConfigMessage)
{
	stringstream IOConfigResponse(IOConfigMessage.Buffer);
	string IOConfig;
	int iIOConfig;
	vector<int> IOList;
	USERIO userIO;
	
	// From c-string to IOList
	while ((getline(IOConfigResponse, IOConfig, ',')) && (IO_END > IOList.size()))
	{
		iIOConfig = atoi(IOConfig.c_str());
		IOList.push_back(iIOConfig);
	}

	//Give format to IO from DB
	WSIODataFromDB(userIO, IOList);
	
	char* temp  = reinterpret_cast<char*>(&userIO);
	//Send formated IO to control task
	memset(IOConfigMessage.Buffer, 0x00, sizeof(IOConfigMessage.Buffer));
	memcpy(IOConfigMessage.Buffer, temp, sizeof(userIO));
}

/**************************************************************************//**
* \brief  - Stores all the IOList elements on single lists sorted by IO type.
*
* \param  - &userIO - USERIO type variable reference where the IO Configuration
* 					  contents are going to be stored.
* 		  - IOList - This variable contains the IO Configuration from the DB.
*
* \return - NONE
*
******************************************************************************/
void WebServiceTask::WSIODataFromDB(USERIO &userIO, vector<int> IOList)
{
	vector<int> tempIOList;
    int itr;
    
    if((false == IOList.empty()) && (IOList.size() % USER_IO_DATA == 0))
    {
        //PS Digital Inputs [9]
        for(itr = PS_D_INPUTS_START;itr < PS_D_OUTPUTS_START; itr++)
        {
        	tempIOList.push_back(IOList[itr]);
    	}
    	WSSetUserPSInputs(userIO, tempIOList);
    	tempIOList.clear();
    	
    	//PS Digital Outputs [8]
    	for(itr = PS_D_OUTPUTS_START;itr < PS_A_INPUTS_START; itr++)
    	{
    		tempIOList.push_back(IOList[itr]);
    	}
    	WSSetUserPSOutputs(userIO, tempIOList);
    	tempIOList.clear();
    	
    	//PS Analog Inputs [2]
    	//PS Analog Outpus [2]

    	//AC Digital Inputs [5]
    	for(itr = AC_D_INPUTS_START;itr < AC_D_OUTPUTS_START; itr++)
    	{
    		tempIOList.push_back(IOList[itr]);
    	}
    	WSSetUserACInputs(userIO, tempIOList);
    	tempIOList.clear();

    	//AC Digital Outputs [5]
    	for(itr = AC_D_OUTPUTS_START;itr < IO_END; itr++)
    	{
    		tempIOList.push_back(IOList[itr]);
    	}
    	WSSetUserACOutputs(userIO, tempIOList);
    	tempIOList.clear();
    }
}

/**************************************************************************//**
* \brief  - Stores the PS Inputs on the USERIO type variable.
*
* \param  - &userIO - USERIO type variable reference where the IO Configuration
* 					  contents are going to be stored.
* 		  - PSInputList - This variable contains the PS Input Configuration.
*
* \return - NONE
*
******************************************************************************/
void WebServiceTask::WSSetUserPSInputs(USERIO &userIO, vector<int> PSInputList)
{
	int IOEnable = CHECK_STATUS;
	int IOPin = PIN_FUNCTION;
	int IOActive = ACTIVE_STATUS;

	userIO.PS_USERIO.PsDigitalInput.J116_01.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin] * PSInputList[IOEnable]);
	userIO.PS_USERIO.PsDigitalInput.J116_01.Status = PSInputList[IOActive];

	userIO.PS_USERIO.PsDigitalInput.J116_02.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_02.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalInput.J116_03.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_03.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalInput.J116_04.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_04.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalInput.J116_11.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_11.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalInput.J116_12.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_12.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalInput.J116_13.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_13.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalInput.J116_16.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_16.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalInput.J116_23.PS_EVENT = (PowerSupplyInput)(PSInputList[IOPin+=USER_IO_DATA_OFFSET] * PSInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalInput.J116_23.Status = PSInputList[IOActive+=USER_IO_DATA_OFFSET];
}

/**************************************************************************//**
* \brief  - Stores the PS Outputs on the USERIO type variable.
*
* \param  - &userIO - USERIO type variable reference where the IO Configuration
* 					  contents are going to be stored.
* 		  - PSInputList - This variable contains the PS Output Configuration.
*
* \return - NONE
*
******************************************************************************/
void WebServiceTask::WSSetUserPSOutputs(USERIO &userIO, vector<int> PSOutputList)
{
	int IOEnable = CHECK_STATUS;
	int IOPin = PIN_FUNCTION;
	int IOActive = ACTIVE_STATUS;

	userIO.PS_USERIO.PsDigitalOutput.J116_07.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin] * PSOutputList[IOEnable]);
	userIO.PS_USERIO.PsDigitalOutput.J116_07.Status = PSOutputList[IOActive];

	userIO.PS_USERIO.PsDigitalOutput.J116_08.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin+=USER_IO_DATA_OFFSET] * PSOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalOutput.J116_08.Status = PSOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalOutput.J116_09.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin+=USER_IO_DATA_OFFSET] * PSOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalOutput.J116_09.Status = PSOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalOutput.J116_10.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin+=USER_IO_DATA_OFFSET] * PSOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalOutput.J116_10.Status = PSOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalOutput.J116_19.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin+=USER_IO_DATA_OFFSET] * PSOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalOutput.J116_19.Status = PSOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalOutput.J116_20.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin+=USER_IO_DATA_OFFSET] * PSOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalOutput.J116_20.Status = PSOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalOutput.J116_21.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin+=USER_IO_DATA_OFFSET] * PSOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalOutput.J116_21.Status = PSOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.PS_USERIO.PsDigitalOutput.J116_22.PS_EVENT = (PowerSupplyOutput)(PSOutputList[IOPin+=USER_IO_DATA_OFFSET] * PSOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.PS_USERIO.PsDigitalOutput.J116_22.Status = PSOutputList[IOActive+=USER_IO_DATA_OFFSET];
}

/**************************************************************************//**
* \brief  - Stores the AC Inputs on the USERIO type variable.
*
* \param  - &userIO - USERIO type variable reference where the IO Configuration
* 					  contents are going to be stored.
* 		  - PSInputList - This variable contains the AC Input Configuration.
*
* \return - NONE
*
******************************************************************************/
void WebServiceTask::WSSetUserACInputs(USERIO &userIO, vector<int> ACInputList)
{
	int IOEnable = CHECK_STATUS;
	int IOPin = PIN_FUNCTION;
	int IOActive = ACTIVE_STATUS;

	userIO.AC_USERIO.ACDigitalInput.J710_01.AC_EVENT = (ActuatorInput)(ACInputList[IOPin] * ACInputList[IOEnable]);
	userIO.AC_USERIO.ACDigitalInput.J710_01.Status = ACInputList[IOActive];

	userIO.AC_USERIO.ACDigitalInput.J710_02.AC_EVENT = (ActuatorInput)(ACInputList[IOPin+=USER_IO_DATA_OFFSET] * ACInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalInput.J710_02.Status = ACInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.AC_USERIO.ACDigitalInput.J710_03.AC_EVENT = (ActuatorInput)(ACInputList[IOPin+=USER_IO_DATA_OFFSET] * ACInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalInput.J710_03.Status = ACInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.AC_USERIO.ACDigitalInput.J710_04.AC_EVENT = (ActuatorInput)(ACInputList[IOPin+=USER_IO_DATA_OFFSET] * ACInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalInput.J710_04.Status = ACInputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.AC_USERIO.ACDigitalInput.J710_05.AC_EVENT = (ActuatorInput)(ACInputList[IOPin+=USER_IO_DATA_OFFSET] * ACInputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalInput.J710_05.Status = ACInputList[IOActive+=USER_IO_DATA_OFFSET];
}

/**************************************************************************//**
* \brief  - Stores the AC Outputs on the USERIO type variable.
*
* \param  - &userIO - USERIO type variable reference where the IO Configuration
* 					  contents are going to be stored.
* 		  - PSInputList - This variable contains the AC Output Configuration.
*
* \return - NONE
*
******************************************************************************/
void WebServiceTask::WSSetUserACOutputs(USERIO &userIO, vector<int> ACOutputList)
{
	int IOEnable = CHECK_STATUS;
	int IOPin = PIN_FUNCTION;
	int IOActive = ACTIVE_STATUS;

	userIO.AC_USERIO.ACDigitalOutput.J710_09.AC_EVENT = (ActuatorOutput)(ACOutputList[IOPin] * ACOutputList[IOEnable]);
	userIO.AC_USERIO.ACDigitalOutput.J710_09.Status = ACOutputList[IOActive];

	userIO.AC_USERIO.ACDigitalOutput.J710_10.AC_EVENT = (ActuatorOutput)(ACOutputList[IOPin+=USER_IO_DATA_OFFSET] * ACOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalOutput.J710_10.Status = ACOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.AC_USERIO.ACDigitalOutput.J710_11.AC_EVENT = (ActuatorOutput)(ACOutputList[IOPin+=USER_IO_DATA_OFFSET] * ACOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalOutput.J710_11.Status = ACOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.AC_USERIO.ACDigitalOutput.J710_12.AC_EVENT = (ActuatorOutput)(ACOutputList[IOPin+=USER_IO_DATA_OFFSET] * ACOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalOutput.J710_12.Status = ACOutputList[IOActive+=USER_IO_DATA_OFFSET];

	userIO.AC_USERIO.ACDigitalOutput.J710_13.AC_EVENT = (ActuatorOutput)(ACOutputList[IOPin+=USER_IO_DATA_OFFSET] * ACOutputList[IOEnable+=USER_IO_DATA_OFFSET]);
	userIO.AC_USERIO.ACDigitalOutput.J710_13.Status = ACOutputList[IOActive+=USER_IO_DATA_OFFSET];
}

/**************************************************************************//**
* \brief  - Wait for the state machine to be on Ready state to attempt the
* 			Login or Logout.
*
* \param  - NONE
*
* \return - WS_STATUS
*
******************************************************************************/
WS_STATUS WebServiceTask::WSWaitForStateMachine()
{
	Message ctrlTaskMessage;
	
	//Enables WS Log attempt flag
	CP->bIsWSLogAttempt = true;
	
	//Waits for response 5 seconds, if no response system is busy 
	if(msgQReceive(WEBSERVICE_MSG_Q_ID,RecvMsgBuffer,MAX_SIZE_OF_MSG_LENGTH,WS_MESSAGE_RESPONSE_TIMEOUT) != ERROR)
	{
		memcpy(&ctrlTaskMessage, RecvMsgBuffer, sizeof(ctrlTaskMessage));
		
		if(ctrlTaskMessage.msgID == TO_WEB_SERVICES_TASK_STATE_MACHINE_WAIT)		
			return WS_SUCCESS;
	}
	return SYSTEM_BUSY;
}
