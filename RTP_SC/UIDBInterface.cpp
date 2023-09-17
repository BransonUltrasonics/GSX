/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     UI Database Interface task for DB related read or write operations  
 
**********************************************************************************************************/

#include "UIDBInterface.h"
#include "CommonProperty.h"
#include  "ControlTask.h"
#include "CommunicateInterface.h"
#include "commons.h"
#include "WeldRecipe.h"
#include "RunTimeFeature.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern "C"
{
	#include "customSystemCall.h"	
}

#define MAX_RETRY_ATTEMPT	10
#define MAX_LOGIN_ATTEPTS	4
#define RECIPE_LIST_ELEMENTS		9
#define RECIPE_LIST_LOC_ACTIVE 		5
#define RECIPE_LIST_LOC_REVISION	8


#define INT 0
#define FLOAT 1

#define RECIPENOINDEX    0
#define RECIPEVERNOINDEX  2
#define TOTALCOLLPASETARGETINDEX  32
#define RECIPENAMEINDEX  1
#define COMPANYNAMEINDEX  115
#define MODE_VALUEINDEX   3
#define SETWELDPARAMNO    124
#define NOOFWELDMODES     7
#define SETUPLIMITPRAMNO  34
#define SETUPLIMITOLDRECIPEINDEX  0
#define SETUPLIMITRECIPEVERNOINDEX  1

string DBInterface::m_WeldRecipeZero;
string DBInterface::m_SuspectRejectLimitsZero;
string DBInterface::m_BransonOnlyZero;
string DBInterface::m_SetupLimitsZero;
string DBInterface::m_StackRecipeZero;


/*{0,0} for param not present in map */
WeldParamDetails WeldParamDetailsValidation[SETWELDPARAMNO]={
{RECIPE_NO,INT},{0,0},{0,0},
{WELD_MODE,INT},{MODE_VALUE,FLOAT},{HOLD_TIME,FLOAT},{TRIGGER_FORCE,INT},{PRETRIGGER_DISTANCE,INT},
{AMPLITUDESTEPSCNT,INT},{AMPLITUDE_STEP_AT,INT},{AMPLITUDE_STEP_VALUE1,INT},{AMPLITUDE_STEP_VALUE2,INT},{AMPLITUDE_STEP_VALUE3,INT},
{AMPLITUDE_STEP_VALUE4,INT},{AMPLITUDE_STEP_VALUE5,INT},{AMPLITUDE_STEP_VALUE6,INT},{AMPLITUDE_STEP_VALUE7,INT},{AMPLITUDE_STEP_VALUE8,INT},
{AMPLITUDE_STEP_VALUE9,INT},{AMPLITUDE_STEP_VALUE10,INT},{AMPLITUDE_STEP1,INT},{AMPLITUDE_STEP2,INT},{AMPLITUDE_STEP3,INT},
{AMPLITUDE_STEP4,INT},{AMPLITUDE_STEP5,INT},{AMPLITUDE_STEP6,INT},{AMPLITUDE_STEP7,INT},{AMPLITUDE_STEP8,INT},
{AMPLITUDE_STEP9,INT},{AMPLITUDE_STEP10,INT},{WELD_FORCE,INT},{FORCE_RAMP_TIME,FLOAT},{0,0},
{HOLD_FORCE,INT},{HOLD_FORCE_RAMP_TIME,INT},{EXPECTED_PART_CONTACT_POSITION,INT},{READY_POSITION,FLOAT},{DOWN_ACCELERATION,FLOAT},
{DOWN_MAX_VELOCITY,INT},{DOWN_DECELERATION,INT},{RETURN_ACCELERATION,INT},{RETURN_MAX_VELOCITY, INT},{RETURN_DECELERATION,INT},
{NUM_FORCE_STEPS,INT},{FORCE_STEP_AT,INT},{FORCE_STEP_VALUE1,INT},{FORCE_STEP_VALUE2,INT},{FORCE_STEP_VALUE3,INT},
{FORCE_STEP_VALUE4,INT},{FORCE_STEP_VALUE5,INT},{FORCE_STEP_VALUE6,INT},{FORCE_STEP_VALUE7,INT},{FORCE_STEP_VALUE8,INT},
{FORCE_STEP_VALUE9,INT},{FORCE_STEP_VALUE10,INT},{FORCE_STEP_RAMP_VALUE1,INT},{FORCE_STEP_RAMP_VALUE2,INT},{FORCE_STEP_RAMP_VALUE3,INT},
{FORCE_STEP_RAMP_VALUE4,INT},{FORCE_STEP_RAMP_VALUE5,INT},{FORCE_STEP_RAMP_VALUE6,INT},{FORCE_STEP_RAMP_VALUE7,INT},{FORCE_STEP_RAMP_VALUE8,INT},
{FORCE_STEP_RAMP_VALUE9,INT},{FORCE_STEP_RAMP_VALUE10,INT},{FORCE_STEP1,INT},{FORCE_STEP2,INT},{FORCE_STEP3,INT},
{FORCE_STEP4,INT},{FORCE_STEP5,INT},{FORCE_STEP6,INT},{FORCE_STEP7,INT},{FORCE_STEP8,INT},
{FORCE_STEP9,INT},{FORCE_STEP10,INT},{WELD_RAMP_TIME,FLOAT},{AFTER_BRUST,INT},{AFTER_BRUST_DELAY,FLOAT},
{AFTER_BRUST_TIME,FLOAT},{AFTER_BRUST_AMPLITUDE,INT},{PRETRIGGER,INT},{AUTO_PRETRIGGER,INT},{DISTANCE_PRETRIGGER,INT},
{PRETRIGGER_AMPLITUDE,INT},{PRETRIGGER_DISTANCE,INT},{ENERGY_BRAKE,INT},{ENERGY_BRAKE_TIME,FLOAT},{ENERGY_BRAKE_AMPLITUDE,INT},
{EXTRA_COOLING,INT },{MAX_WELD_TIMEOUT,FLOAT},{TIMED_SEEK,INT},{PRE_WELD_SEEK,INT},{POST_WELD_SEEK,INT},
{TIMED_SEEK_PERIOD,INT},{PART_CONTACT_WINDOW_MINUS,FLOAT},{PART_CONTACT_WINDOW_PLUS,FLOAT},{PART_CONTACT_WINDOW_OFFSET, FLOAT},{GLOBALCONTROL,INT},
{PEAKPOWERCUTOFF,INT},{PEAKPOWERCUTOFFENABLE,INT},{FREQUENCYLOWCUTOFF,INT},{FREQUENCYLOWCUTOFFENABLE,INT},{ABSOLUTECUTOFF,INT },
{ABSOLUTECUTOFFENABLE,INT },{FREQUENCYHIGHCUTOFF,INT },{FREQUENCYHIGHCUTOFFENABLE,INT},{COLLAPSECUTOFF,FLOAT},{COLLAPSECUTOFFENABLE,INT},
{ENERGYCUTOFF, INT},{ENERGYCUTOFFENABLE,INT},{TIMECUTOFF,FLOAT},{TIMECUTOFFENABLE,INT},{GROUNDDETECTENABLE,INT},
{ISACTIVE,INT},{ISLOCK,INT},{0,0},{ISVALID,INT},{READY_POSITION_TOGGLE,INT},
{WELD_FORCE_CONTROL,INT},{REACTIVITY, INT},{FORCE_LEVEL_ENABLED,INT},{FORCE_LEVEL,INT },{FORCE_LEVEL_TIME,FLOAT},
{SCRUB_AMPLITUDE,INT}

};

WeldParamDetails WeldParamDetailsWeldModeValidation[NOOFWELDMODES]={
{WELD_MODE_TIME,FLOAT},{WELD_MODE_ENERGY,FLOAT},{WELD_MODE_POWER,INT},{WELD_MODE_ABSOLUTE_DISTANCE,INT},{WELD_MODE_COLLAPSE_DISTANCE,FLOAT},
{WELD_MODE_SCRUB_TIME,FLOAT},{WELD_MODE_DENSITY,INT}};

WeldParamDetails WeldParamDetailsSetupLimitValidation[SETUPLIMITPRAMNO]={
{0,0},{0,0},{RECIPE_NO,INT},		
{SETUP_IS_GLOBAL_SETUP,INT},{SETUP_WELD_MODE_STATUS,INT},{SETUP_WELD_MODE_LOW,FLOAT},{SETUP_WELD_MODE_HIGH,FLOAT},{SETUP_WELD_AMP_STATUS,INT},
{SETUP_WELD_AMP_LOW,INT},{SETUP_WELD_AMP_HIGH,INT},{SETUP_WELD_FORCE_STATUS,INT},{SETUP_WELD_FORCE_LOW,INT},{SETUP_WELD_FORCE_HIGH,INT},
{SETUP_TRIGGER_FORCE_STATUS,INT},{SETUP_TRIGGER_FORCE_LOW,INT},{SETUP_TRIGGER_FORCE_HIGH,INT},{SETUP_HOLD_FORCE_STATUS,INT},{SETUP_HOLD_FORCE_LOW,INT},
{SETUP_HOLD_FORCE_HIGH,INT},{SETUP_HOLD_TIME_STATUS,INT},{SETUP_HOLD_TIME_LOW,FLOAT},{SETUP_HOLD_TIME_HIGH,FLOAT},{SETUP_SCRUB_AMP_STATUS,INT},
{SETUP_SCRUB_AMP_LOW,INT},{SETUP_SCRUB_AMP_HIGH,INT},{SETUP_SCRUB_TIME_STATUS,INT},{SETUP_SCRUB_TIME_LOW,FLOAT},{SETUP_SCRUB_TIME_HIGH,FLOAT},
{SETUP_REACTIVITY_STATUS,INT},{SETUP_REACTIVITY_LOW,INT},{SETUP_REACTIVITY_HIGH,INT},{SETUP_FORCE_LEVEL_STATUS,INT},{SETUP_FORCE_LEVEL_LOW,INT},
{SETUP_FORCE_LEVEL_HIGH,INT}};

void UI_DataBase_Task(void)
{   
	char MsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};
	DBInterface *DBInit = new DBInterface();
	Message ReqMessage;
	DBResponseMessage ResMessage;	
	MSG_Q_ID UI_DB_MSG_Q_ID = DBInit->CP->GetMsgQId(cTaskName[UI_DB_T]);
	CommunicateInterface *InterfaceObject = CommunicateInterface::getinstance();
	
	while(DBInit->CP->bIsTaskRun)
	{
		if(msgQReceive(UI_DB_MSG_Q_ID, MsgBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
		{
			DBInit->Decode(MsgBuffer, ReqMessage);
			DBInit->ProcessRequest(ReqMessage, ResMessage);
			
//			printf ("\n%sDBInterface::ProcessRequest: msgID: %d %s\n", KYEL, ReqMessage.msgID, KNRM);
			
			// 4+4 is the size of the msg ID + length of buffer
            int len = 4+4+ResMessage.Buffer.length();
            char *ch = (char*)malloc(len+4);
            memcpy(ch,&(ResMessage.msgID),4);
            int dwBufflen = ResMessage.Buffer.length();
            memcpy(ch+4,&dwBufflen,4);
            memcpy(ch+8,ResMessage.Buffer.c_str(),ResMessage.Buffer.length());
            
			if((ResMessage.msgID > DUMMY) || (ResMessage.msgID < 0))
			{
				LOGCRIT("UI_DB_T : Invalid response message ID: %d",ResMessage.msgID, 0, 0);
			}
			else
			{
            	InterfaceObject->Send(ch,len);
            }
			free(ch);
		}
	}	
	delete DBInit;
	DBInit = NULL;
	taskDelete(taskIdSelf());
}


DBInterface::DBInterface()
{
	CP = CommonProperty::getInstance();
	m_RecipeParamValidation=Recipe::getInstance(); 
	ConnectDB();
	PrepareValidatedRecipezeroFiles();
    LoadRecipeZero();
    CP->UpdateStartUPDigitalTune(m_StackRecipeZero);
}

DBInterface::~DBInterface()
{
	CloseDB();
}

void DBInterface::GetDateAndTime(string &DateTime)
{
	RTCtimerStr SysTime;
	char temp[TIMESTAMP] = {0x00};
	GetRTC(&SysTime);
	
	SysTime.tm_year = 1900 + SysTime.tm_year;
	SysTime.tm_mon  = 1 + SysTime.tm_mon;
	
	sprintf(temp,"%d/%02d/%02d %02d:%02d:%02d",SysTime.tm_year,SysTime.tm_mon,SysTime.tm_mday,SysTime.tm_hour,SysTime.tm_min,SysTime.tm_sec);

	DateTime = temp;
}

void DBInterface::ConnectDB()
{
	m_db.EstablishDataBaseConnection();
#ifdef DEBUG
	LOGDBG("DB Connection opened",0,0,0);
#endif	
}

void DBInterface::CloseDB()
{
	m_db.Close();
#ifdef DEBUG
	LOGDBG("DB Connection closed",0,0,0);
#endif
}

void DBInterface::ProcessRequest(Message& requestBuffer, DBResponseMessage& OutData)
{
	Message detailsBuffer;
	string details;
    string rec1;
    string sParamId;
    char cParamId[100];
	stringstream request;
    stringstream data;
    vector<string> vec1;
    vector<string> vec2;
    size_t pos = 0;
    bool logged = false;
    vector<string> vRecipeList;
    UINT32 numOfRecipes;

	bool bNewRecipe = false;
	INT32 queryCode;
	string query;

    enum {
    	req_user = 1,
    	req_password = 3,
    	req_userlevel = 5,
    	req_userpriv = 7,
    	req_userstatus = 9,
    };
    enum {
    	rec_user = 0,
    	rec_password,
    	rec_userlevel,
    	rec_userstatus,
    	rec_userpriv,
    };
	semTake (m_db.getSCDBMutex(), WAIT_FOREVER);
    switch(requestBuffer.msgID)
    {
    case TO_DB_TASK_DB_CONNECTION_CLOSE_REQ:				/* Do not remove this code */
    	CloseDB();
    	EraseRecipeZero();
    	break;
    	
    case TO_DB_TASK_DB_CONNECTION_OPEN_REQ:
    	ConnectDB();
    	break;
    
    case REQ_WELD_RECIPE_LIST:
        OutData.msgID = RES_WELD_RECIPE_LIST;
        OutData.Buffer = GetWeldRecipeList(requestBuffer.Buffer);
        
        // get a recipe list
        CP->StringToTokens(OutData.Buffer.c_str(),vRecipeList,',');
        numOfRecipes = vRecipeList.size() / RECIPE_LIST_ELEMENTS;
        
        // get last recipe revision of an unsaved active recipe
        for (int grp = 0; grp < numOfRecipes; grp++)
        {
        	if (vRecipeList[(grp * RECIPE_LIST_ELEMENTS) + RECIPE_LIST_LOC_ACTIVE] == "1")
        	{
        		m_RevisionNum = atoi (vRecipeList[(grp * RECIPE_LIST_ELEMENTS) + RECIPE_LIST_LOC_REVISION].c_str());
        		break;
        	}
        }
//        printf ("\n%sDBInterface::ProcessRequest: REQ_WELD_RECIPE_LIST: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_WELD_RECIPE_PARAM:
        OutData.msgID = RES_GET_WELD_RECIPE_PARAM;
        OutData.Buffer = GetWeldRecipeParams(requestBuffer.Buffer);
        m_IsGetWeldRecipeParam = true;
        
        // save recipe
        request.str (OutData.Buffer);
        m_GetWeldRecipeParam.clear();
        for (pos = 0; pos < ACTIVE_RECIPE_END; pos++)
        {
        	getline (request, details,',');
        	m_GetWeldRecipeParam.push_back (details);
        }
        
        // only get recipe revision if the recipe was saved
        if (m_GetWeldRecipeParam[SC_RECIPE_NUMBER-1].compare("0") != 0)
        	m_RevisionNum = atoi (m_GetWeldRecipeParam[SC_RECIPE_REV_NUMBER-1].c_str());
        
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_WELD_RECIPE_PARAM: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_SET_WELD_RECIPE_PARAM:
    	
        // get what to save
    	request.str (requestBuffer.Buffer);
    	
    	vec1.clear();
		while (getline (request, details,','))
			vec1.push_back (details);
		
		// get what's current
		request.clear();
		request.str (GetWeldRecipeParams ("0"));
		
		vec2.clear();
		while (getline (request, details,','))
			vec2.push_back (details);

        OutData.msgID = RES_SET_WELD_RECIPE_PARAM;
        OutData.Buffer = SetWeldRecipeParams(requestBuffer.Buffer);
        
        // save the last update
        m_SetWeldRecipeParam. clear();
        m_SetWeldRecipeParam = vec1;
		
		if (vec1[SC_RECIPE_NUMBER] == "0")
		{
			if (vec1[SC_RECIPE_REV_NUMBER] != "0")
			{
				for (pos = SC_WELD_MODE; pos < vec1.size() && pos < vec2.size(); pos++)
				{
					// check for changes and (ignore reported changes to company name and validation status)
					if ((vec1[pos] != vec2[pos-1]) && (pos != SC_COMPANY_NAME && pos != SC_IS_VALIDATE))
					{
						snprintf (cParamId, sizeof (cParamId), "%d", pos);
						sParamId = cParamId;
						details = "Identifier:" + sParamId + ":From:" + vec2[pos-1] + ":" + "To:" + vec1[pos] + ":";
						DataBaseManager::EnterEventLog  (RECIPE_MODIFIED, details);
						WeldRecipeStatus::SetRecipeStatus (UNSAVED);
						logged = true;
					}
				}
				
				if (logged)
				{
					semGive(m_db.getSCDBMutex());
					return;
				}
			}
		}
		
		if (m_IsGetWeldRecipeParam)
		{
			if (!m_GetWeldRecipeParam.empty() and !m_SetWeldRecipeParam.empty())
			{
				if (atoi (m_GetWeldRecipeParam[0].c_str()) > 0  and atoi (m_SetWeldRecipeParam[SC_RECIPE_NUMBER].c_str()) > 0
						and (atoi (m_GetWeldRecipeParam[0].c_str()) != atoi (m_SetWeldRecipeParam[SC_RECIPE_NUMBER].c_str())))
				{
					details = "Identifier:N/A:From:" + m_GetWeldRecipeParam[0] + ":" + "To:" + m_SetWeldRecipeParam[SC_RECIPE_NUMBER] + ":";
					DataBaseManager::EnterEventLog  (RECIPE_COPIED, details);
				}
			}
			m_IsGetWeldRecipeParam = false;
		}
		
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_WELD_RECIPE_PARAM: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_ACTIVE_RECIPE:
        OutData.msgID = RES_GET_ACTIVE_RECIPE;
        OutData.Buffer = GetActiveRecipe(requestBuffer.Buffer);
        WeldRecipeStatus::SetRecipeStatus (GetActiveRecipeStatus());
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_ACTIVE_RECIPE: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_PRODUCTION_WELD_RESULT:
        OutData.msgID = RES_GET_PRODUCTION_WELD_RESULT;
        OutData.Buffer = GetProductionWeldResult(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_PRODUCTION_WELD_RESULT: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_LAST_WELD_RESULT:
        OutData.msgID = RES_GET_LAST_WELD_RESULT;
        OutData.Buffer = GetLastWeldResult(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_LAST_WELD_RESULT: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_LAST_WELD_PEAKPOWER:
        OutData.msgID = RES_GET_LAST_WELD_PEAKPOWER;
        OutData.Buffer = GetLastWeldPeakPower(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_LAST_WELD_PEAKPOWER: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_ALARM_LOG:
        OutData.msgID = RES_GET_ALARM_LOG;
        OutData.Buffer = GetAlarmLog(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_LAST_WELD_PEAKPOWER: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_DEFAULT_HORN_RECIPE:
        OutData.msgID = RES_GET_DEFAULT_HORN_RECIPE;
        OutData.Buffer = GetDafaultHornScanRecipe(requestBuffer.Buffer);
        break;
    case REQ_GET_HORN_SIGNATURE_DATA:
        OutData.msgID = RES_GET_HORN_SIGNATURE_DATA;
        OutData.Buffer = GetHornSignatureData(requestBuffer.Buffer);
        break;
    case REQ_GET_HORN_SCAN_HISTORY:
        OutData.msgID = RES_GET_HORN_SCAN_HISTORY;
        OutData.Buffer = GetHornScanHistory();
        break;
    case REQ_GET_HORN_SCAN_RESULTS:
        OutData.msgID = RES_GET_HORN_SCAN_RESULTS;
        OutData.Buffer = GetHornScanResult(requestBuffer.Buffer);
        break;
    case REQ_SET_HORN_SCAN_ID:
        OutData.msgID = RES_SET_HORN_SCAN_ID;
        OutData.Buffer = SetHornScanID(requestBuffer.Buffer);
        break;
    case REQ_GET_RECIPE_INFO:
        OutData.msgID = RES_GET_RECIPE_INFO;
        OutData.Buffer = GetRecipeInfo(requestBuffer.Buffer);
        break;
    case REQ_GET_RECIPE_SETUP_INFO:
        OutData.msgID = RES_GET_RECIPE_SETUP_INFO;
        OutData.Buffer = GetRecipeSetupInfo(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_RECIPE_SETUP_INFO: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_SET_RECIPE_SETUP_INFO:
        OutData.msgID = RES_SET_RECIPE_SETUP_INFO;
        OutData.Buffer = SetRecipeSetupInfo(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_RECIPE_SETUP_INFO: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
	case REQ_SET_WELD_RECIPE_SETUP_INFO:
        OutData.msgID = RES_SET_WELD_RECIPE_SETUP_INFO;
        OutData.Buffer = SetWeldRecipeSetupInfo(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_WELD_RECIPE_SETUP_INFO: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_USER_LOGIN:
        OutData.msgID = RES_USER_LOGIN;
        OutData.Buffer = UserLogin(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_USER_LOGIN: requestBuffer: %s OutData: %s %s\n", KYEL, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_ALL_USERS:
        OutData.msgID = RES_GET_ALL_USERS;
        OutData.Buffer = GetAllUsers();
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_ALL_USERS: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        
        // save all users
        m_AllUsers = OutData.Buffer;
        break;
    case REQ_SET_USER_INFO:
        OutData.msgID = RES_SET_USER_INFO;
        OutData.Buffer = SetUserInfo(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_USER_INFO: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);

        // break up the request
        request.str (requestBuffer.Buffer);
		while (getline (request, details,'\''))
			vec1.push_back (details);
                
        // look for that user
		pos = m_AllUsers.find (vec1[req_user]);
		
        if (pos != string::npos)
        {
        	// extract the record
        	rec1.assign (m_AllUsers, pos, string::npos);
        	data.str (rec1);
        	
            // break up the data
    		while (getline (data, details,','))
    			vec2.push_back (details);
    		
    		// compare
    		if (vec1[req_user] != vec2[rec_user])
    			details = "Identifier:" + vec2[rec_user] + " User" + ":" + "From:" +  vec2[rec_user] + ":" + "To:" +  vec1[req_user] + ":";
    		
    		else if (vec1[req_password] != vec2[rec_password])
    			details = "Identifier:" + vec2[rec_user] + " Password" + ":" + "From:" +  "N/A" + ":" + "To:" +  "N/A" + ":";
    			
    		else if (vec1[req_userlevel] != vec2[rec_userlevel])
    			details = "Identifier:" + vec2[rec_user] + " Level" + ":" + "From:" +  vec2[rec_userlevel] + ":" + "To:" +  vec1[req_userlevel] + ":";

    		else if (vec1[req_userpriv] != vec2[rec_userpriv])
    			details = "Identifier:" + vec2[rec_user] + " Privilege" + ":" + "From:" +  vec2[rec_userpriv] + ":" + "To:" +  vec1[req_userpriv] + ":";

    		else if (vec1[req_userstatus] != vec2[rec_userstatus])
    			details = "Identifier:" + vec2[rec_user] + " Status" + ":" + "From:" +  vec2[rec_userstatus] + ":" + "To:" +  vec1[req_userstatus] + ":";
    		else
    			details.clear();

    		// send it to the event log
            if (!details.empty())
            	DataBaseManager::EnterEventLog (USER_MODIFIED, details);
        }
        else
		{
            // send it to the event log
        	details = "Identifier:" + vec1[req_user] + ":" + "From: N/A" +  ":" + "To: N/A:";
        	DataBaseManager::EnterEventLog (USER_CREATED, details);
		}
        
        break;
    case REQ_SET_ACTIVE_RECIPE:
        OutData.msgID = RES_SET_ACTIVE_RECIPE;
        OutData.Buffer = SetActiveRecipe(requestBuffer.Buffer);
        WeldRecipeStatus::SetRecipeStatus (GetActiveRecipeStatus());
        break;
    case REQ_GET_EVENTS_LOG:
        OutData.msgID = RES_GET_EVENTS_LOG;
        OutData.Buffer = GetEventLogs();
        break;
    case REQ_SET_EVENT_LOG:
        OutData.msgID = RES_SET_EVENT_LOG;
        OutData.Buffer = SetEventLogs(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_EVENT_LOG: requestBuffer: %s OutData: %s %s\n", KYEL, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_GET_WELD_SIGNATURE_DATA:
    	
        OutData.msgID = RES_GET_WELD_SIGNATURE_DATA;
        if (CP->ind_CollectGraphData==true)
        {
           OutData.Buffer = GetWeldSignature(requestBuffer.Buffer);
        }
        break;
    case REQ_GET_WELD_TWH_DATA:
        if (CP->ind_CollectGraphData==true)
        {
        	OutData.msgID = RES_GET_WELD_TWH_DATA;
        	OutData.Buffer = GetWeldTWH(requestBuffer.Buffer);
        }
        break;
    case REQ_GET_WELD_HISTORY_DATA:
        OutData.msgID = RES_GET_WELD_HISTORY_DATA;
        OutData.Buffer = GetWeldHistory(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_WELD//_HISTORY_DATA %s\n", KYEL, KNRM);
        break;
    case REQ_GET_LAST_WELD_HISTORY_DATA:
        OutData.msgID = RES_GET_LAST_WELD_HISTORY_DATA;
        OutData.Buffer = GetLastWeldHistory(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_LAST_WELD_HISTORY_DATA %s\n", KYEL, KNRM);
        break;
    case REQ_GET_NEXT_WELD_HISTORY_DATA:
        OutData.msgID = RES_GET_NEXT_WELD_HISTORY_DATA;
        OutData.Buffer = GetNextWeldHistory(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_NEXT_WELD_HISTORY_DATA %s\n", KYEL, KNRM);
        break;
    case REQ_GET_ALL_AUTHORITY_LEVEL:
        OutData.msgID = RES_GET_ALL_AUTHORITY_LEVEL;
        OutData.Buffer = GetAuthorityLevel(requestBuffer.Buffer);
        break;
    case REQ_UPDATE_ALL_AUTHORITY_LEVEL:
        OutData.msgID = RES_UPDATE_ALL_AUTHORITY_LEVEL;
        OutData.Buffer = UpdateAuthorityLevel(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_UPDATE_ALL_AUTHORITY_LEVEL: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_USER_PASSWORD:
        OutData.msgID = RES_GET_USER_PASSWORD;
        OutData.Buffer = GetUserPassword(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_USER_PASSWORD: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_SET_USER_PASSWORD:
        OutData.msgID = RES_SET_USER_PASSWORD;
        OutData.Buffer = SetUserPassword(requestBuffer.Buffer);
        break;
    case REQ_GET_USER_DETAILS:
        OutData.msgID = RES_GET_USER_DETAILS;
        OutData.Buffer = GetUserDetails(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_USER_DETAILS: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_SET_GLOBAL_SETTINGS:
        OutData.msgID = RES_SET_GLOBAL_SETTINGS;
        OutData.Buffer = SetGlobalSettings(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_GLOBAL_SETTINGS: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_GET_GLOBAL_SETTINGS:
        OutData.msgID = RES_GET_GLOBAL_SETTINGS;
        OutData.Buffer = GetGlobalSettings(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_GLOBAL_SETTINGS: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_SET_HORN_RECIPE_DATA :
        OutData.msgID = RES_SET_HORN_RECIPE_DATA ;
        OutData.Buffer = SetHornRecipeData(requestBuffer.Buffer);
        break;
	case REQ_GET_SEEK_SCAN_HISTORY :
        OutData.msgID = RES_GET_SEEK_SCAN_HISTORY ;
        OutData.Buffer = SetSeekScanHistory();
        break;
    case REQ_GET_DEFAULT_SEEK_RECIPE :
        OutData.msgID = RES_GET_DEFAULT_SEEK_RECIPE ;
        OutData.Buffer = GetSeekRecipe(requestBuffer.Buffer);
        break;
    case REQ_SET_SEEK_RECIPE_DATA :
        OutData.msgID = RES_SET_SEEK_RECIPE_DATA ;
        OutData.Buffer = SetSeekRecipe(requestBuffer.Buffer);
        break;
    case REQ_GET_SEEK_SIGNATURE_DATA :
        OutData.msgID = RES_GET_SEEK_SIGNATURE_DATA ;
        OutData.Buffer = GetSeekSignatureData();
        break;
        
    case REQ_SET_STACK_RECIPE_DATA :

		// break up the request for new stack (request: 20000,20001,1,0,0)
		request.str (requestBuffer.Buffer);
		while (getline (request, details,','))
			vec2.push_back (details);
		
		query = "select DigitialTune, InternalOffsetFlag, InternalFreqOffset, EndOfWeldStore from StackRecipeTable where RecipeNumber = "+ vec2[0] + " LIMIT 1;";
		if ((m_db.Exec((char*)query.c_str(), &queryCode).empty()) && queryCode == SQLITE_OK)
		{
			bNewRecipe = true;
	        m_RecipeNumber = vec2[0];
			details = "Identifier:Recipe " + m_RecipeNumber + ":From:N/A:To:N/A:";
			DataBaseManager::EnterEventLog  (RECIPE_CREATED, details);
		}
		
		if (!m_RecipeNumber.empty())
		{
			data.str (GetStackRecipe (m_Save ? m_RecipeNumber : "0"));
			
			if (!data.str().empty())
			{
				// break up the data from old stack (OutData: 20001,1,0,0)
				while (getline (data, details,','))
					vec1.push_back (details);
	
		    	m_Save = true;

		    	// compare DigitialTune,InternalOffsetFlag,InternalFreqOffset,EndofWeldStore and enter event
				if (vec1[0] != vec2[1] && !bNewRecipe)
				{
					snprintf (detailsBuffer.Buffer, sizeof (detailsBuffer.Buffer), "Identifier:%d:From:%s:To:%s:", SC_DIGITAL_TUNE, vec1[0].c_str(), vec2[1].c_str());
					details = detailsBuffer.Buffer;
					m_Save = false;
					DataBaseManager::EnterEventLog  (RECIPE_MODIFIED, details);
				}
				if (vec1[1] != vec2[2] && !bNewRecipe)
				{
					snprintf (detailsBuffer.Buffer, sizeof (detailsBuffer.Buffer), "Identifier:%d:From:%s:To:%s:", SC_OFFSET_FLAG, vec1[1].c_str(), vec2[2].c_str());
					details = detailsBuffer.Buffer;
					m_Save = false;
					DataBaseManager::EnterEventLog  (RECIPE_MODIFIED, details);
				}
				if (vec1[2] != vec2[3] && !bNewRecipe)
				{
					snprintf (detailsBuffer.Buffer, sizeof (detailsBuffer.Buffer), "Identifier:%d:From:%s:To:%s:", SC_FREQUENCY_OFFSET, vec1[2].c_str(), vec2[3].c_str());
					details = detailsBuffer.Buffer;
					m_Save = false;
					DataBaseManager::EnterEventLog  (RECIPE_MODIFIED, details);
				}
				if (vec1[3] != vec2[4] && !bNewRecipe)
				{
					snprintf (detailsBuffer.Buffer, sizeof (detailsBuffer.Buffer), "Identifier:%d:From:%s:To:%s:", SC_END_OF_WELD, vec1[3].c_str(), vec2[4].c_str());
					details = detailsBuffer.Buffer;
					m_Save = false;
					DataBaseManager::EnterEventLog  (RECIPE_MODIFIED, details);
				}
			}
		}
		else
			m_Save = true;

        OutData.msgID = RES_SET_STACK_RECIPE_DATA ;
        OutData.Buffer = SetStackRecipe (requestBuffer.Buffer, m_Save);
        
        if (m_Save == true && !bNewRecipe)
        {
        	details = "Identifier:Recipe " + m_db.Exec("select RecipeNumber from WeldRecipeTable where IsActive = 1 and RecipeNumber>0;") + ":From:N/A:To:N/A:";
        	DataBaseManager::EnterEventLog (RECIPE_SAVED, details);
			WeldRecipeStatus::SetRecipeStatus (SAVED);
        }
        
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_STACK_RECIPE_DATA: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
        
    case REQ_GET_STACK_RECIPE_DATA :
        OutData.msgID = RES_GET_STACK_RECIPE_DATA ;
        m_RecipeNumber = requestBuffer.Buffer;
        OutData.Buffer = GetStackRecipe(m_Save ? requestBuffer.Buffer : "0");
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_STACK_RECIPE_DATA: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_SET_SUSPECT_REJECT_LIMITS_INFO :
        OutData.msgID = RES_SET_SUSPECT_REJECT_LIMITS_INFO ;
        OutData.Buffer = SetSuspectRejectLimits(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_SUSPECT_REJECT_LIMITS_INFO: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_GET_SUSPECT_REJECT_LIMITS_INFO :
        OutData.msgID = RES_GET_SUSPECT_REJECT_LIMITS_INFO ;
        OutData.Buffer = GetSuspectRejectLimits(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_SUSPECT_REJECT_LIMITS_INFO: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_GET_TEST_MODE_DATA :
        OutData.msgID = RES_GET_TEST_MODE_DATA ;
        OutData.Buffer = GetTestModeGraphData(requestBuffer.Buffer);
        break;
    case REQ_GET_TEST_RECIPE_DATA:
        OutData.msgID = RES_GET_TEST_RECIPE_DATA ;
        OutData.Buffer = GetTestModeRecipeData(requestBuffer.Buffer);
        break;
    case REQ_SET_TEST_RECIPE_DATA:
        OutData.msgID = RES_SET_TEST_RECIPE_DATA ;
        OutData.Buffer = SetTestModeRecipeData(requestBuffer.Buffer);
        break;
    case REQ_GET_TEST_PROGRESS_INFO:
        OutData.msgID = RES_GET_TEST_PROGRESS_INFO ;
        OutData.Buffer = GetTestModeResultData(requestBuffer.Buffer);
        break;
    case REQ_GET_DB_VERSION:
        OutData.msgID = RES_GET_DB_VERSION ;
        OutData.Buffer = GetDBVersion();
        break;
	case REQ_RESONANT_FREQUENCY:
        OutData.msgID = RES_RESONANT_FREQUENCY ;
        OutData.Buffer = GetResonantFrequency(requestBuffer.Buffer);
        break;
    case REQ_GET_ADD_RECIPENUMBER:
        OutData.msgID = RES_SET_ADD_RECIPENUMBER;
        OutData.Buffer = GetRecipeNumberFromDbForAddRecipe();
        
        m_IsGetWeldRecipeParam = false;
        m_lastRecipeNum = OutData.Buffer;
//        printf ("\n%sDBInterface::ProcessRequest: REQ_GET_ADD_RECIPENUMBER: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_GET_COPY_RECIPENUMBER:
        OutData.msgID = RES_SET_COPY_RECIPENUMBER;
        OutData.Buffer = GetRecipeNumberFromDbForCopyRecipe(requestBuffer.Buffer);
        break;
    case REQ_SET_WELD_RECIPE_PARAM_FOR_NEW_RECIPE:
        OutData.msgID = RES_SET_WELD_RECIPE_PARAM_FOR_NEW_RECIPE;
        OutData.Buffer = SetWeldRecipeParamsForNewRecipe(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_WELD_RECIPE_PARAM_FOR_NEW_RECIPE: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_SET_SUSPECT_REJECT_LIMITS_INFO_FOR_UPDATED:
        OutData.msgID = RES_SET_SUSPECT_REJECT_LIMITS_INFO_FOR_UPDATED;
        OutData.Buffer = SetSuspectRejectForNewRecipe(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_SUSPECT_REJECT_LIMITS_INFO_FOR_UPDATED: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_SET_RECIPE_SETUP_INFO_FOR_UPDATED:
        OutData.msgID = RES_SET_RECIPE_SETUP_INFO_FOR_UPDATED;
        OutData.Buffer = SetRecipeSetupInfoForNewRecipe(requestBuffer.Buffer);
        break;
    case REQ_GET_ACTUATOR_SETUP_PARAMS:
        OutData.msgID = RES_GET_ACTUATOR_SETUP_PARAMS;
        OutData.Buffer = GetActuatorSetupParams();
        break;
    case REQ_SET_ACTUATOR_SETUP_PARAMS:
        OutData.msgID = RES_SET_ACTUATOR_SETUP_PARAMS;
        OutData.Buffer = SetActuatorSetupParams(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_ACTUATOR_SETUP_PARAMS: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_GET_ANALYTICS_ALARM:
        OutData.msgID = RES_GET_ANALYTICS_ALARM;
        OutData.Buffer = GetAnalyticsAlarm(requestBuffer.Buffer);
        break;
    case REQ_SET_ANALYTICS_ALARM:
        OutData.msgID = RES_SET_ANALYTICS_ALARM;
        OutData.Buffer = setAnalyticsAlarm(requestBuffer.Buffer);
        break;
    case REQ_GET_TRENDSGRAPH_SIGNATURE_DATA:
        OutData.msgID = RES_GET_TRENDSGRAPH_SIGNATURE_DATA;
        OutData.Buffer = GetTrendsGraphSignatureData(requestBuffer.Buffer);
        break;
    case REQ_GET_SUSPECT_REJECT_LIMITS_TRENDSGRAPH:
        OutData.msgID = RES_GET_SUSPECT_REJECT_LIMITS_TRENDSGRAPH;
        OutData.Buffer = GetSuspectRejectLimitsForTrendsGraph(requestBuffer.Buffer);
        break;
    case REQ_SET_BRANSONONLY_PARAMS:
        OutData.msgID = RES_SET_BRANSONONLY_PARAMS;
        OutData.Buffer = setBransonOnlyParams(requestBuffer.Buffer);
        break;
    case REQ_GET_BRANSONONLY_PARAMS:
        OutData.msgID = RES_GET_BRANSONONLY_PARAMS;
        OutData.Buffer = getBransonOnlyParams(requestBuffer.Buffer);
        break;
    case REQ_SET_BRANSONONLY_PARAMS_UPDATED:
        OutData.msgID = RES_SET_BRANSONONLY_PARAMS_UPDATED;
        OutData.Buffer = setBransonOnlyParamsUpdated(requestBuffer.Buffer);
        break;
    case REQ_GET_ALL_WELD_RESULT_DATA:
        OutData.msgID = RES_GET_ALL_WELD_RESULT_DATA;
        OutData.Buffer = getAllWeldResultData(requestBuffer.Buffer);
        break;
    case REQ_GET_LAST_ALL_WELD_RESULT_DATA:
        OutData.msgID = RES_GET_LAST_ALL_WELD_RESULT_DATA;
        OutData.Buffer = getLastAllWeldResultData(requestBuffer.Buffer);
        break;
    case  REQ_SET_PRODUCT_DATE_PARAM:
        OutData.msgID = RES_SET_PRODUCT_DATE_PARAM;
        OutData.Buffer = GetAnalyticsProductionOverView(requestBuffer.Buffer);
        break;
	case REQ_GET_ACTIVE_RECIPE_NUMBER:
        OutData.msgID = RES_GET_ACTIVE_RECIPE_NUMBER;
        OutData.Buffer = GetActiveRecipeNumber(requestBuffer.Buffer);
        break;
    case REQ_GET_BATCH_COUNT:
        OutData.msgID = RES_GET_BATCH_COUNT;
        OutData.Buffer = GetBatchCount(requestBuffer.Buffer);
        break;
    case REQ_RESET_BATCH_COUNTER:
        OutData.msgID = RES_RESET_BATCH_COUNTER;
        OutData.Buffer = SetBatchCounter(requestBuffer.Buffer);
        break;
    case REQ_GET_USERIO_DATA:
        OutData.msgID = RES_GET_USERIO_DATA;
        OutData.Buffer = GetUserIO(requestBuffer.Buffer);
        break;
    case REQ_SET_USERIO_DATA:
        OutData.msgID = RES_SET_USERIO_DATA;
        OutData.Buffer = SetUserIO(requestBuffer.Buffer);
        break;
    case REQ_GET_SETUP_LIMITS:
        OutData.msgID = RES_GET_SETUP_LIMITS;
        OutData.Buffer = GetSetupLimits(requestBuffer.Buffer);
        break;
    case REQ_SET_SETUP_LIMITS:
        OutData.msgID = RES_SET_SETUP_LIMITS;
        OutData.Buffer = SetSetupLimits(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_SETUP_LIMITS: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_SET_RECIPE_VALIDATE:
        OutData.msgID = RES_SET_RECIPE_VALIDATE;
        OutData.Buffer = SetRecipeValidate(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_RECIPE_VALIDATE: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        break;
    case REQ_GET_ACTIVE_RECIPE_FOR_ACTUATOR:
        OutData.msgID = RES_GET_ACTIVE_RECIPE_FOR_ACTUATOR;
        OutData.Buffer = GetActiveRecipeForActuatorSetUp();
        break;
    case REQ_SET_RECIPE_BLOB:
        OutData.msgID = RES_SET_RECIPE_BLOB;
        OutData.Buffer = SetRecipeToBLOB(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SET_RECIPE_BLOB: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(),KNRM);
        
        break;
    case REQ_GET_RECIPE_BLOB:
        OutData.msgID = RES_GET_RECIPE_BLOB;
        OutData.Buffer = GetRecipeToBLOB(requestBuffer.Buffer);
        break;
    case REQ_SET_WELDFORCE_FOR_ACTUATOR:
        OutData.msgID = RES_SET_WELDFORCE_FOR_ACTUATOR;
        OutData.Buffer = SetWeldForceForActuatorSetUp(requestBuffer.Buffer);
        break;
    case REQ_SET_RECIPE_UNVALIDATE:
        OutData.msgID = RES_SET_RECIPE_UNVALIDATE;
        OutData.Buffer = SetRecipeUnValidate(requestBuffer.Buffer);
        break;
    case REQ_FACTORY_RESET:
        OutData.msgID = RES_FACTORY_RESET;
        OutData.Buffer = SetFactoryReset(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_FACTORY_RESET: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_UPDATE_SYSTEM_DATA:
        OutData.msgID = RES_UPDATE_SYSTEM_DATA;
        OutData.Buffer = UpdateSystemData(requestBuffer.Buffer);
        break;
    case REQ_UPDATE_USER_LOGIN:
        OutData.msgID = RES_UPDATE_USER_LOGIN;
        OutData.Buffer = UpdateUserLogin(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_UPDATE_USER_LOGIN: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_SAVE_WEBSERVICES_DATA:
        OutData.msgID = RES_SAVE_WEBSERVICES_DATA;
        OutData.Buffer = SaveWebServicesStatus(requestBuffer.Buffer);
//        printf ("\n%sDBInterface::ProcessRequest: REQ_SAVE_WEBSERVICES_DATA: requestBuffer: %s OutData: %s %s\n", KCYN, requestBuffer.Buffer, OutData.Buffer.c_str(), KNRM);
        break;
    case REQ_GET_WEBSERVICES_DATA:
        OutData.msgID = RES_GET_WEBSERVICES_DATA;
        OutData.Buffer = GetServicesStatus();
        break;
    case REQ_GET_WEBSERVICES_USER_AND_LEVEL:
        OutData.msgID = RES_GET_WEBSERVICES_USER_AND_LEVEL;
        OutData.Buffer = GetWSUserAndLevel();
    	break;
    case 0:
        OutData.msgID = 0;
        OutData.Buffer = "";
        break;
    case REQ_DELETE_RECIPE:
        OutData.msgID = RES_DELETE_RECIPE;
        OutData.Buffer = DeleteRecipeHMI(requestBuffer.Buffer);
    	break;       
    default:
        OutData.msgID = requestBuffer.msgID;
        OutData.Buffer = "1";
        break;
    };
    semGive(m_db.getSCDBMutex());
}

// UI Cases
std::string DBInterface::GetWeldRecipeList(std::string strData)
{
    std::string strQuery = "SELECT RecipeNumber,WeldRecipeName,CompanyName,WeldMode,ModeValue,IsActive,IsLocked,IsValidate,MAX(WeldRecipeVerNumber) as id FROM WeldRecipeTable where RecipeNumber>0 AND weldmode IN (" + strData + ") GROUP BY RecipeNumber;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetWeldRecipeParams(std::string strData)
{
	if("0" == strData)
	{
		return GetRecipeZero();
	}
	else
	{
		std::string strQuery = "select * from weldrecipeTable,SuspectRejectLimits,BransonOnlyParameters,SetupLimits where weldrecipeTable.recipenumber = "+ strData +" and SuspectRejectLimits.recipenumber = "+ strData +" and BransonOnlyParameters.recipenumber = "+ strData +" and SetupLimits.recipenumber = "+ strData +";";
		return m_db.Exec((char*)strQuery.c_str());
	}
}


bool DBInterface:: ValidateSuspectRejectLimitsForASX(std::string strData)
{
	
	
}

/**************************************************************************//**
 * \brief  - Validate SetupLimits specific parameters of recipe
 *
 * \param  - string :strData ( recipe parameter string)
 * 		     
 * \return - result (0 or 1)
 *
 ******************************************************************************/
bool DBInterface::ValidateSetupLimit(std::string strData)
{
	bool status=true;
    int cnt=0;
    vector<string> vRecipeParams;
    int pIndex=0;
    
    /* the format of setuplimit recipe parameter string received from HMI is like this
       0,0,1,0,'0:10:30000','0:10:100','0:500:25000',..... 
       but for the Ease of parsing we have to make it comma separated string.
	   so we need to remove "'", and replace  ":"  by ","    from the  string
	   after this the format of string should be like this 0,0,1,0,0,10,30000,0,10,100,0,500,25000,..... */
    
	strData.erase(remove(strData.begin(), strData.end(), '\''), strData.end()); //remove "'"  from Param string
	std::replace( strData.begin(), strData.end(), ':', ',');  					//replace  ":"  by ","    from Param string
    cnt=CP->StringToTokens(strData.c_str(),vRecipeParams,',');

    for(pIndex=0;(pIndex<vRecipeParams.size());pIndex++)
    {  
    	/*Recipeoldnumber,Recipever number are skipped from validation*/
    	if((pIndex==SETUPLIMITOLDRECIPEINDEX) || (pIndex==SETUPLIMITRECIPEVERNOINDEX))
    	{
    		continue;
    	}
    	else if(!ValidateRecipeParam(vRecipeParams[pIndex],WeldParamDetailsSetupLimitValidation[pIndex].recipePramMapID ,WeldParamDetailsSetupLimitValidation[pIndex].dataType))
		{
			status=false;
			break;
		}
    
    }
    return status;
}

/**************************************************************************//**
 * \brief  - Validate the Decimal Digit Numbers of a float value Upto decimalNumLimit .
 *
 * \param  - float: chInputParamValue ( value)
 * 		   - int:   decimalNumLimit   
 *
 * \return - result (0 or 1)
 *
 ******************************************************************************/
bool DBInterface:: ValidateDecimalDigitNumbers(float chInputParamValue, int decimalNumLimit)
{
	bool status=true;
	char chInputParamValueArray[10]="";
	stringstream strInputParamValue; 
	string strWholeDecimal, strDecimalDigits;
	int decimalPoints=0;

	sprintf(chInputParamValueArray, "%f", chInputParamValue);
	strInputParamValue.str(chInputParamValueArray);
	getline(strInputParamValue, strWholeDecimal, '.');
	getline(strInputParamValue, strDecimalDigits);

	decimalPoints = strDecimalDigits.size();	
	for (int pos = decimalNumLimit;pos < (decimalPoints);pos++)
	{
		if(strDecimalDigits[pos]!='0')
		{
			status=false;
			break;
		}
	}
	return status;
}

/**************************************************************************//**
 * \brief  - check if the string is alphanumeric or not
 *
 * \param  - string :strData ( recipe parameter string)
 *
 * \return - result (0 or 1)
 *
 ******************************************************************************/
bool DBInterface::  IsAlphaNumeric(const std::string str)
{
	 for(int i=0;i<str.length();i++)
	 {
	      if(!isalnum(str[i]))
	      {
	      return false;
	      }	 
	 }
	 return true;
}

/**************************************************************************//**
 * \brief  - Validate Name parameter of recipe for empty strings,length of name string checks and check if string is alphanumeric .
 *
 * \param  - string :strData ( recipe parameter string)
 *
 * \return - result (0 or 1)
 *
 ******************************************************************************/
bool DBInterface:: ValidateRecipeParamName(std::string strData)
{
	bool status =true;
	strData.erase(remove(strData.begin(), strData.end(), '\''), strData.end()); //the name string consist of ' at the start and end ex: 'NewRecipe'
	if(strData.empty())
	{ 
		return status; 
	}
	
	if(strData.length()> 20)
	{ 
		status =false;	
	}
	else if(!(IsAlphaNumeric(strData)))
	{
		status =false;		 
	}
	 
	 return status;	
	
}
/**************************************************************************//**
 * \brief  - Validate parameter of recipe for empty strings and checks against min/max values
 *
 * \param  -string :strData    ( recipe parameter string),
 * 			int paramId:		The Paramid present in Map for respective parameter,
 * 			int paramDataType:	Prameter data type
 *
 * \return - result (0 or 1)
 *
 ******************************************************************************/
bool DBInterface:: ValidateRecipeParam(std::string strData,int paramId ,int paramDataType)
{
	int dwParamValue=0;
	bool status =false;
	int dwMinValue=0;
	int dwMaxValue=0;
	float dwParamFloatValue=0;
    
	if(strData.empty())
	{ 
		return status; 
	}
	dwParamValue=atoi(strData.c_str());	
	
	if((paramDataType==FLOAT) && (dwParamValue!=0) )
	{
		dwParamFloatValue=dwParamValue/1000.0;		
		if(!ValidateDecimalDigitNumbers(dwParamFloatValue,3))
		{
			status =false;
			return status; 
		}
	}	
	dwMinValue = m_RecipeParamValidation->GetMinValue(paramId);
	dwMaxValue = m_RecipeParamValidation->GetMaxValue(paramId);
	if((dwParamValue >= dwMinValue) && (dwParamValue <= dwMaxValue))
	{
		status =true;
	}else
	{
		status =false;
	}
	return status;
}

/**************************************************************************//**
 * \brief  - Validate SetWeldRecipe specific parameters of recipe
 *
 * \param  - string :strData ( recipe parameter string)
 *
 * \return - result (0 or 1)
 *
 ******************************************************************************/
bool DBInterface:: ValidateRecipeForSetWeldRecipeParams(std::string strData)
{
	bool status = true;
	int numForceVal = 0;
	int weldModeVal = 0;
	float distVal = 0;
	int forceVal=0;
    int cnt=0;int i=0;
    vector<string> vRecipeParams;
    int dwParamValue=0;
    int pIndex=0;
    int dwmodeType=0;
    cnt=CP->StringToTokens(strData.c_str(),vRecipeParams,',');
 
    for(pIndex=0;(pIndex<vRecipeParams.size());pIndex++)
    {  
    	/*recipever number, TotalCollapseTarget are skipped from validation*/
    	if((pIndex==RECIPEVERNOINDEX) || (pIndex==TOTALCOLLPASETARGETINDEX))
    	{
    		continue;
    	}
    	else if((pIndex==COMPANYNAMEINDEX) ||(pIndex== RECIPENAMEINDEX) )
    	{
    		if(!(ValidateRecipeParamName(vRecipeParams[pIndex])))
    		{
    			status =false;
    		  	break;
    		}
    		else
    		{
    			status =true;
    		}	 		
    	} 	    	   	    	
    	else if((WeldParamDetailsValidation[pIndex].recipePramMapID ==MODE_VALUE))
    	{
     		dwmodeType=atoi(vRecipeParams[MODE_VALUEINDEX].c_str()); 		
    		if(!ValidateRecipeParam(vRecipeParams[pIndex],WeldParamDetailsWeldModeValidation[dwmodeType-1].recipePramMapID ,WeldParamDetailsWeldModeValidation[dwmodeType-1].dataType))
    		{
    			status=false;
    			break;
    		}						
    	}
    	else 	
    	{	    	
    		if(!ValidateRecipeParam(vRecipeParams[pIndex],WeldParamDetailsValidation[pIndex].recipePramMapID ,WeldParamDetailsValidation[pIndex].dataType))
    		{
    			status=false;
    			break;
    		}    	
    	}    	
    } 
	return status;
}

std::string DBInterface::SetWeldRecipeParams(std::string strData)
{
	std::string strQuery,strOldRecipeNumber,strNewRecipeNumber,strRecipeData;
	char *temp = (char*)strData.c_str();
	char *RecipeNamePoint = strstr(temp,",");
	char revisionNumStr[10];
	vector<string> vRecipeParams;
	
	if(!RecipeNamePoint)
		return "";
	
	strOldRecipeNumber.append(temp,RecipeNamePoint - temp);
	strRecipeData.append(RecipeNamePoint+1);
	temp = (char*)strRecipeData.c_str();
	RecipeNamePoint = strstr(temp,",");
	
	if(!RecipeNamePoint)
		return "";
	
	strNewRecipeNumber.append(temp,RecipeNamePoint - temp);
	/*validate SetWeldRecipeParams*/
//    if(!ValidateRecipeForSetWeldRecipeParams(strRecipeData))
//     {
//	     AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_NOT_VALID_NCA);
//	     return "";
//	 }
	if("0" == strNewRecipeNumber)
	{
		m_WeldRecipeZero = strRecipeData;
		WriteRecipeZeroToFile();
		
		return "";
	}
	else
	{

		// handle revision number (intercept)
		CP->StringToTokens(strRecipeData.c_str(),vRecipeParams,',');
		
		// is the revision = 0 then newly created recipe
		if (atoi (vRecipeParams[SC_RECIPE_REV_NUMBER-1].c_str()) == 0)
		{
			m_RevisionNum = atoi (vRecipeParams[SC_RECIPE_REV_NUMBER-1].c_str());
		}
		else
		{
			
			m_RevisionNum += 1;
			snprintf (revisionNumStr, sizeof (revisionNumStr), "%d", m_RevisionNum);
			
			vRecipeParams[SC_RECIPE_REV_NUMBER-1] = revisionNumStr;
			
			DataBaseManager::EventLogRecipeRevision (vRecipeParams[SC_RECIPE_REV_NUMBER-1]);
			
			CP->TokensToString(strRecipeData,vRecipeParams,',');
		}

		strQuery="delete from WeldRecipeTable where RecipeNumber=" + strOldRecipeNumber + ";";
		RetryMechanism((char*)strQuery.c_str());

		if(strNewRecipeNumber.compare(strOldRecipeNumber) != 0)
		{
			strQuery="delete from WeldRecipeTable where RecipeNumber=" + strNewRecipeNumber + ";";
			RetryMechanism((char*)strQuery.c_str());
		}

		strQuery="Insert into WeldRecipeTable values(" + strRecipeData + ");";
		m_WeldRecipeZero = strRecipeData;
		WriteRecipeZeroToFile();
		
		return RetryMechanism ((char*)strQuery.c_str());
	}
}

std::string DBInterface::SetWeldRecipeParamsForNewRecipe(std::string strData)
{
	std::string strQuery,strRecipeNumber;
	char *temp = (char*)strData.c_str();
	char *RecipeNumberPoint = strstr(temp,",");
	if(!RecipeNumberPoint)
		return "";
	strRecipeNumber.append(temp,RecipeNumberPoint - temp);

	//if system is ASX
	if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
	{
        if(!ValidateRecipeForSetWeldRecipeParams(strData))
        {
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_NOT_VALID_NCA);
			return "";
		}
	}
	
	if("0" == strRecipeNumber)
	{
		m_WeldRecipeZero = strData;
		WriteRecipeZeroToFile();
		
		return "";
	}
	else
	{
		strQuery="delete from WeldRecipeTable where RecipeNumber=" + strRecipeNumber + ";";
		m_db.Exec((char*)strQuery.c_str());
		
		int firstIndex=strData.find(',');
		strData=strData.substr(firstIndex+1,strData.length());
		
		strQuery="Insert into WeldRecipeTable values(" + strData + ");";
		return RetryMechanism ((char*)strQuery.c_str());
	}
}


std::string DBInterface::SetSuspectRejectForNewRecipe(std::string strData)
{
	std::string strQuery,strRecipeNumber;
	char *temp = (char*)strData.c_str();
	char *RecipeNumberPoint = strstr(temp,",");
	if(!RecipeNumberPoint)
		return "";
	strRecipeNumber.append(temp,RecipeNumberPoint - temp);

	if("0" == strRecipeNumber)
	{
		m_SuspectRejectLimitsZero = strData;
		return "";
	}
	else
	{
		strQuery="delete from SuspectRejectLimits where RecipeNumber=" + strRecipeNumber + ";";
		m_db.Exec((char*)strQuery.c_str());
		int firstIndex=strData.find(',');
		strData=strData.substr(firstIndex+1,strData.length());
		strQuery="Insert into SuspectRejectLimits values(" + strData + ");";
		return RetryMechanism ((char*)strQuery.c_str());
	}
}

std::string DBInterface::SetRecipeSetupInfoForNewRecipe(std::string strData)
{
    std::string strQuery,strRecipeNumber;
    char *temp = (char*)strData.c_str();
    char *RecipeNumberPoint = strstr(temp,",");
    if(!RecipeNumberPoint)
        return "";
    strRecipeNumber.append(temp,RecipeNumberPoint - temp);

    strQuery="delete from ProductionSetupTable where RecipeNumber=" + strRecipeNumber + ";";
    m_db.Exec((char*)strQuery.c_str());
    int firstIndex=strData.find(',');
    strData=strData.substr(firstIndex+1,strData.length());
    strQuery="Insert into ProductionSetupTable values(" + strData + ");";
    return RetryMechanism ((char*)strQuery.c_str());
}

std::string DBInterface::GetActuatorSetupParams()
{
    std::string strQuery = "select PartContactWindowMinus,PartContactWindowPlus,PartContactWindowOffset FROM WeldRecipeTable where RecipeNumber>0 AND IsActive = 1";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::SetActuatorSetupParams(std::string strData)
{
    std::string partContactWindowMinus = "";
    std::string partContactWindowPlus = "";
    std::string partContactWindowOffset = "";
    char * dup = NULL;
  	char * token = NULL;
  	
	dup = strdup(strData.c_str());
	
	if(!dup)
	{
		return "";
	}
	
	token = strtok(dup, ",");
	
	if(!token)
	{
		free(dup);
		return "";
	}
	
    if (token)
    {
        partContactWindowMinus = std::string(token);
        token = strtok(NULL, ",");
        if(!token)
        {
        	free(dup);
            return "";
        }
        partContactWindowPlus = std::string(token);
        token = strtok(NULL, ",");
        
       if(!token)
	   {
    	   free(dup);
		   return "";
	   }
        
        partContactWindowOffset = std::string(token);
    }

    free(dup);

    std::string strQuery="UPDATE WeldRecipeTable SET PartContactWindowMinus ='" +partContactWindowMinus +"',PartContactWindowPlus  = '" +partContactWindowPlus +"', PartContactWindowOffset = '" +partContactWindowOffset +"' where IsActive = 1 AND RecipeNumber>0;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetAnalyticsAlarm(std::string strData)
{
    std::string strQuery = "Select RecipeNumber,AlarmID,AlarmType,DateTime,CycleCounter,RecipeVerNumber,PcAssembly,AcAssembly,UserName from AlarmLogs ORDER BY rowId DESC LIMIT 100;";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::setAnalyticsAlarm(std::string strData)
{
    std::string alaramStatus = "";
    std::string recipeNo = "";

    char *temp = (char*)strData.c_str();
    char *DelimeterPoint = strstr(temp,",");
    if(!DelimeterPoint)
        return "";
    recipeNo.append(temp,DelimeterPoint - temp);
    alaramStatus.append(DelimeterPoint+1);

    std::string strQuery="UPDATE AlarmLogs SET AlarmStatus ='" +alaramStatus +"' where RecipeNumber='" + recipeNo + "';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::setBransonOnlyParams(std::string strData)
{
	std::string strQuery,strBransonOnly,strRecipeData,strNewRecipeNumber;
	char *temp = (char*)strData.c_str();
	char *DelimeterPoint = strstr(temp,",");
	if(!DelimeterPoint)
		return "";
	strBransonOnly.append(temp,DelimeterPoint - temp);
	strRecipeData.append(DelimeterPoint+1);
	temp = (char*)strRecipeData.c_str();
	DelimeterPoint = strstr(temp,",");
	if(!DelimeterPoint)
		return "";
	strNewRecipeNumber.append(temp,DelimeterPoint - temp);

	if("0" == strNewRecipeNumber)
	{
		m_BransonOnlyZero = strRecipeData;
		return "";
	}
	else
	{
		strQuery="delete from BransonOnlyParameters where RecipeNumber=" + strBransonOnly + ";";
		RetryMechanism ((char*)strQuery.c_str());

		if(strNewRecipeNumber.compare(strBransonOnly) != 0)
		{
			strQuery="delete from BransonOnlyParameters where RecipeNumber=" + strNewRecipeNumber + ";";
			RetryMechanism ((char*)strQuery.c_str());
		}
		strQuery="Insert into BransonOnlyParameters values(" + strRecipeData + ");";
		return RetryMechanism ((char*)strQuery.c_str());
	}
}

std::string DBInterface::getBransonOnlyParams(std::string strData)
{
	if("0" == strData)
	{
		return m_BransonOnlyZero;
	}
	else
	{
		std::string strQuery = "select * from BransonOnlyParameters where RecipeNumber = (" + strData + ");";
		return m_db.Exec((char*)strQuery.c_str());
	}
}

std::string DBInterface::setBransonOnlyParamsUpdated(std::string strData)
{
	std::string strQuery,strRecipeNumber;
	char *temp = (char*)strData.c_str();
	char *RecipeNumberPoint = strstr(temp,",");
	if(!RecipeNumberPoint)
		return "";
	strRecipeNumber.append(temp,RecipeNumberPoint - temp);

	if("0" == strRecipeNumber)
	{
		m_BransonOnlyZero = strData;
		return "";
	}
	else
	{
		strQuery="delete from BransonOnlyParameters where RecipeNumber=" + strRecipeNumber + ";";
		m_db.Exec((char*)strQuery.c_str());

		int firstIndex=strData.find(',');
		strData=strData.substr(firstIndex+1,strData.length());
		strQuery="Insert into BransonOnlyParameters values(" + strData + ");";
		return RetryMechanism ((char*)strQuery.c_str());
	}
}


std::string DBInterface::GetActiveRecipe(std::string strData)
{
	std::string strQuery = "Select RecipeNumber,WeldRecipeName,CompanyName,WeldMode,ModeValue,IsActive,IsLocked,IsValidate from WeldRecipeTable where IsActive=1 and RecipeNumber>0 and WeldMode IN (" + strData + ");";
    return m_db.Exec((char*)strQuery.c_str());
}


/**************************************************************************//**
* \brief   - Logs any suspect/reject limits changes to the event log base on:
 *  0
 *  1
 *  1
 *  0
 * '0:0:0:10:30000:0:0:0:10:30000'
 * '0:0:0:1:120000:0:0:0:1:0'
 * '0:0:0:1:4000:0:0:0:1:4000'
 * '0:0:0:3000:125000:0:0:0:3000:125000'
 * '0:0:0:10:25000:0:0:0:10:25000'
 * '0:0:0:3000:125000:0:0:0:3000:125000'
 * '0:0:0:50:2500:0:0:0:50:2500'
 * '0:0:0:1:500:0:0:0:1:500'
*
* \param   - string limitsData
*
* \return  - STATUS
*
******************************************************************************/
STATUS DBInterface::LogSuspectRejectLimits (std::string limitsData)
{
	enum {
		_recipeNum,
		_recipeVersion,
		_globalSuspect,
		_globalReject,
		_timeLimits,
		_energyLimits,
		_powerLimits,
		_absoluteLimits,
		_collapseLimits,
		_triggerLimits,
		_forceLimits,
		_frequencyLimits
	};
	enum {
		_suspectEnable,
		_suspectLowEnable,
		_suspectHighEnable,
		_suspectLowVal,
		_suspectHighVal,
		_rejectEnable,
		_rejectLowEnable,
		_rejectHighEnable,
		_rejectLowVal,
		_rejectHighVal,
	};
	
	vector<string> 	vLimits;
	vector<string> 	vRecipeZero;
	vector<string> 	vRecipeValues;
	vector<string> 	vZeroValues;
	stringstream 	ssLimits;
	stringstream 	ssRecipeZero;
	stringstream	ssRecipeValues;
	stringstream	ssZeroValues;
	string 			details;
	
	char			buffer[MSG_SIZE];
	int 			pos;
	
    // breakup incoming limits
	ssLimits.str (limitsData);
	
	if (ssLimits.str().empty())
		return ERROR;
	
	// build
	while (getline (ssLimits, details,','))
		vLimits.push_back (details);
	
	// clean
	for (pos = _timeLimits; pos < vLimits.size(); pos++)
		{vLimits[pos].erase (vLimits[pos].begin()); vLimits[pos].erase (vLimits[pos].end()-1);};

    // breakup recipe 0 limits
	ssRecipeZero.str (m_SuspectRejectLimitsZero);
	
	if (ssRecipeZero.str().empty())
		return ERROR;
	
	// build
	while (getline (ssRecipeZero, details,','))
		vRecipeZero.push_back (details);

	// clean
	for (pos = _timeLimits; pos < vRecipeZero.size(); pos++)
		{vRecipeZero[pos].erase (vRecipeZero[pos].begin()); vRecipeZero[pos].erase (vRecipeZero[pos].end()-1);};


	// --- check globals enables
	if (vLimits[_globalSuspect] != vRecipeZero[_globalSuspect])
	{
		// send it to the event log
		snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_GLOBAL_ENABLE, vRecipeZero[_globalSuspect].c_str(), vLimits[_globalSuspect].c_str());
		details = buffer;
		DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
	}
	if (vLimits[_globalReject] != vRecipeZero[_globalReject])
	{
		// send it to the event log
		snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_GLOBAL_ENABLE, vRecipeZero[_globalReject].c_str(), vLimits[_globalReject].c_str());
		details = buffer;
		DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
	}
	
	// --- check values
	if (vLimits[_timeLimits] != vRecipeZero[_timeLimits])
	{
		ssRecipeValues << vLimits[_timeLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _timeLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_timeLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _timeLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_TIME_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_TIME_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_TIME_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_TIME_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_TIME_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_TIME_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
		
	if (vLimits[_energyLimits] != vRecipeZero[_energyLimits])
	{
		ssRecipeValues << vLimits[_energyLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _energyLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_energyLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _energyLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_ENERGY_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_ENERGY_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_ENERGY_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_ENERGY_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_ENERGY_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_ENERGY_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}

	if (vLimits[_powerLimits] != vRecipeZero[_powerLimits])
	{
		ssRecipeValues << vLimits[_powerLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _powerLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_powerLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _powerLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_POWER_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_POWER_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_POWER_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_POWER_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_POWER_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_POWER_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_absoluteLimits] != vRecipeZero[_absoluteLimits])
	{
		ssRecipeValues << vLimits[_absoluteLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _absoluteLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_absoluteLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _absoluteLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_ABSOLUTE_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_ABSOLUTE_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_ABSOLUTE_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_ABSOLUTE_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_ABSOLUTE_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_ABSOLUTE_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_collapseLimits] != vRecipeZero[_collapseLimits])
	{
		ssRecipeValues << vLimits[_collapseLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _collapseLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_collapseLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _collapseLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_COLLAPSE_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_COLLAPSE_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_COLLAPSE_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_COLLAPSE_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_COLLAPSE_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_COLLAPSE_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_triggerLimits] != vRecipeZero[_triggerLimits])
	{
		ssRecipeValues << vLimits[_triggerLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _triggerLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_triggerLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _triggerLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_TRIGGER_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_TRIGGER_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_TRIGGER_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_TRIGGER_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_TRIGGER_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_TRIGGER_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_forceLimits] != vRecipeZero[_forceLimits])
	{
		ssRecipeValues << vLimits[_forceLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _forceLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_forceLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _forceLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_END_FORCE_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_END_FORCE_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_END_FORCE_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_END_FORCE_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_END_FORCE_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_END_FORCE_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}

	if (vLimits[_frequencyLimits] != vRecipeZero[_frequencyLimits])
	{
		ssRecipeValues << vLimits[_frequencyLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssRecipeValues: _frequencyLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_frequencyLimits]; 
//		printf ("\n%sDBInterface::LogSuspectRejectLimits: ssZeroValues: _frequencyLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_suspectEnable] != vZeroValues[_suspectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_FREQUENCY_ENABLE, vZeroValues[_suspectEnable].c_str(), vRecipeValues[_suspectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectLowVal] != vZeroValues[_suspectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_FREQUENCY_LOW, vZeroValues[_suspectLowVal].c_str(), vRecipeValues[_suspectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_suspectHighVal] != vZeroValues[_suspectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SUSPECT_FREQUENCY_HIGH, vZeroValues[_suspectHighVal].c_str(), vRecipeValues[_suspectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectEnable] != vZeroValues[_rejectEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_FREQUENCY_ENABLE, vZeroValues[_rejectEnable].c_str(), vRecipeValues[_rejectEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectLowVal] != vZeroValues[_rejectLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_FREQUENCY_LOW, vZeroValues[_rejectLowVal].c_str(), vRecipeValues[_rejectLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_rejectHighVal] != vZeroValues[_rejectHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_REJECT_FREQUENCY_HIGH, vZeroValues[_rejectHighVal].c_str(), vRecipeValues[_rejectHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}

	return OK;
}


/**************************************************************************//**
* \brief   - Logs any setup based limits changes to the event log base on:
* recipe number, recipe version, enable, Time, Amplitude, Weld Force,
* Trigger Force, Hold Force, Hold Time and scrub time, scrub amplitude,
* reactivity and force level
* 0,
* 1,
* 1,
* '1:10:30000'
* ,'0:10:100',
* '0:25:2500',
* '0:5:50',
* '0:25:2500',
* '0:10:30000',
* '0:1:100',
* '0:1:500',
* '0:0:100'
* '0:0:100'
*
* \param   - string limitsData
*
* \return  - STATUS
*
******************************************************************************/
STATUS DBInterface::LogSetupLimits (std::string limitsData)
{
	enum {
		_recipeNum,
		_recipeVersion,
		_globalSetupEnable,
		_modeLimits,
		_amplitueLimits,
		_forceLimits,
		_triggerLimits,
		_holdForceLimits,
		_holdTimeLimits,
		_scrubAmpLimits,
		_scrubTimeLimits,
		_reactivityLimits,
		_forceLevelLimits,
	};
	enum {
		_localSetupEnable,
		_setupLowVal,
		_setupHighVal,
	};
	
	vector<string> 	vLimits;
	vector<string> 	vRecipeZero;
	vector<string> 	vRecipeValues;
	vector<string> 	vZeroValues;
	stringstream 	ssLimits;
	stringstream 	ssRecipeZero;
	stringstream	ssRecipeValues;
	stringstream	ssZeroValues;
	string 			details;
	
	char			buffer[MSG_SIZE];
	int 			pos;
	
    // breakup incoming limits
	ssLimits.str (limitsData);
	
	if (ssLimits.str().empty())
		return ERROR;
	
	// build
	while (getline (ssLimits, details,','))
		vLimits.push_back (details);
	
	// clean
	for (pos = _modeLimits; pos < vLimits.size(); pos++)
		{vLimits[pos].erase (vLimits[pos].begin()); vLimits[pos].erase (vLimits[pos].end()-1);};

    // breakup recipe 0 limits
	ssRecipeZero.str (m_SetupLimitsZero);
	
	if (ssRecipeZero.str().empty())
		return ERROR;
	
	// build
	while (getline (ssRecipeZero, details,','))
		vRecipeZero.push_back (details);

	// clean
	for (pos = _modeLimits; pos < vRecipeZero.size(); pos++)
		{vRecipeZero[pos].erase (vRecipeZero[pos].begin()); vRecipeZero[pos].erase (vRecipeZero[pos].end()-1);};
	
	// --- check globals enables
	if (vLimits[_globalSetupEnable] != vRecipeZero[_globalSetupEnable])
	{
		// send it to the event log
		snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_GLOBAL_ENABLE, vRecipeZero[_globalSetupEnable].c_str(), vLimits[_globalSetupEnable].c_str());
		details = buffer;
		DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
	}

	// --- check values
	if (vLimits[_modeLimits] != vRecipeZero[_modeLimits])
	{
		ssRecipeValues << vLimits[_modeLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _modeLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_modeLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _modeLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_MODE_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_MODE_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_MODE_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_amplitueLimits] != vRecipeZero[_amplitueLimits])
	{
		ssRecipeValues << vLimits[_amplitueLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _amplitueLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_amplitueLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _amplitueLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_AMPLITUDE_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_AMPLITUDE_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_AMPLITUDE_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}

	if (vLimits[_forceLimits] != vRecipeZero[_forceLimits])
	{
		ssRecipeValues << vLimits[_forceLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _forceLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_forceLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _forceLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_WELD_FORCE_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_WELD_FORCE_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_WELD_FORCE_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_triggerLimits] != vRecipeZero[_triggerLimits])
	{
		ssRecipeValues << vLimits[_triggerLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _triggerLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_triggerLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _triggerLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_TRIGGER_FORCE_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_TRIGGER_FORCE_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_TRIGGER_FORCE_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}

	if (vLimits[_holdForceLimits] != vRecipeZero[_holdForceLimits])
	{
		ssRecipeValues << vLimits[_holdForceLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _holdForceLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_holdForceLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _holdForceLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_HOLD_FORCE_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_HOLD_FORCE_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_HOLD_FORCE_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}

	if (vLimits[_holdTimeLimits] != vRecipeZero[_holdTimeLimits])
	{
		ssRecipeValues << vLimits[_holdTimeLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _holdTimeLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_holdTimeLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _holdTimeLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_HOLD_TIME_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_HOLD_TIME_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_HOLD_TIME_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_scrubAmpLimits] != vRecipeZero[_scrubAmpLimits])
	{
		ssRecipeValues << vLimits[_scrubAmpLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _scrubAmpLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_scrubAmpLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _scrubAmpLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_SCRUB_AMPLITUDE_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SCRUB_AMPLITUDE_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SCRUB_AMPLITUDE_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}

	if (vLimits[_scrubTimeLimits] != vRecipeZero[_scrubTimeLimits])
	{
		ssRecipeValues << vLimits[_scrubTimeLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssRecipeValues: _scrubTimeLimits: %s %s\n", KCYN, ssRecipeValues.str().c_str(),KNRM);
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_scrubTimeLimits]; 
//		printf ("\n%sDBInterface::LogSetupLimits: ssZeroValues: _scrubTimeLimits: %s %s\n", KCYN, ssZeroValues.str().c_str(),KNRM);
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_SCRUB_TIME_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SCRUB_TIME_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SCRUB_TIME_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_reactivityLimits] != vRecipeZero[_reactivityLimits])
	{
		ssRecipeValues << vLimits[_reactivityLimits]; 
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_reactivityLimits]; 
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_REACTIVITY_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_REACTIVITY_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_REACTIVITY_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	if (vLimits[_forceLevelLimits] != vRecipeZero[_forceLevelLimits])
	{
		ssRecipeValues << vLimits[_forceLevelLimits]; 
		while (getline (ssRecipeValues, details,':'))
			vRecipeValues.push_back (details);
		
		ssZeroValues << vRecipeZero[_forceLevelLimits]; 
		while (getline (ssZeroValues, details,':'))
			vZeroValues.push_back (details);
		
		if (vRecipeValues[_localSetupEnable] != vZeroValues[_localSetupEnable])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_FORCE_LEVEL_ENABLE, vZeroValues[_localSetupEnable].c_str(), vRecipeValues[_localSetupEnable].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupLowVal] != vZeroValues[_setupLowVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_FORCE_LEVEL_LOW, vZeroValues[_setupLowVal].c_str(), vRecipeValues[_setupLowVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
		if (vRecipeValues[_setupHighVal] != vZeroValues[_setupHighVal])
		{
			// send it to the event log
			snprintf (buffer, sizeof (buffer), "Identifier:%d:From:%s:To:%s:", SC_SETUP_FORCE_LEVEL_HIGH, vZeroValues[_setupHighVal].c_str(), vRecipeValues[_setupHighVal].c_str());
			details = buffer;
			DataBaseManager::EnterEventLog (RECIPE_MODIFIED, details);
		}
	}
	
	return OK;
}

/**************************************************************************//**
* \brief   - Gets the IsValidate field from the active recipe and returns the RECIPE_STATUS
*
* \param   - None
*
* \return  - RECIPE_STATUS of either SAVED or VALIDATED (would not be called when there are unasaved changes)
*
******************************************************************************/
RECIPE_STATUS DBInterface::GetActiveRecipeStatus()
{
    std::string strQuery = "Select IsValidate from WeldRecipeTable where IsActive=1 and RecipeNumber>0;";
    if(0 == (m_db.Exec((char*)strQuery.c_str()).compare("1")))
    	return VALIDATED;
    return SAVED;
}


std::string DBInterface::GetActiveRecipeForActuatorSetUp()
{
    std::string strQuery = "Select RecipeNumber,WeldRecipeName,WeldForce from WeldRecipeTable where IsActive=1 and RecipeNumber>0;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetProductionWeldResult(std::string strData)
{
    std::string strQuery = "Select Good,PartsPerMin,Reject,Suspect,BatchCountLimit,BatchCounter,CycleCount,BatchSetupEnable from ProductionSetupTable where RecipeNumber=" + strData +";";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetLastWeldResult(std::string strData)
{
	std::string strQuery;
	
    if (!strData.empty())
    	strQuery = "Select WeldTime,MaxWeldForce,WeldEnergy,EndHoldForce,WeldCollapseDistance,TotalAbsolute from WeldResultTable where RecipeNumber = " + strData +" ORDER BY ROWID DESC LIMIT 1;";
    else
    	strQuery = "Select WeldTime,MaxWeldForce,WeldEnergy,EndHoldForce,WeldCollapseDistance,TotalAbsolute from WeldResultTable ORDER BY ROWID DESC LIMIT 1;";
    
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetLastWeldPeakPower(std::string strData)
{
    std::string strQuery = "Select WeldPeakPower from WeldResultTable where RecipeNumber = " + strData +" ORDER BY ROWID DESC LIMIT 1;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetAlarmLog(std::string strData)
{
    std::string strQuery = "Select AlarmID,AlarmType,DateTime from AlarmLogs where RecipeNumber =" + strData +" ORDER BY DateTime DESC LIMIT 8;";
	return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetDafaultHornScanRecipe(std::string strData)
{ 
    std::string strQuery = "select StepFrequency,maxAmplitude,maxCurrent,setDigTuneWithHornScan,StartFrequency,StopFrequency,TimeDelay from HornRecipeTable where Psfreq='"+ strData +"'";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetHornSignatureData(std::string strData)
{
    //std::string strQuery = "select frequency,current,amplitude,phase from HornSignatureTable where ScanId='"+ strData +"'";
	std::string strQuery = "select GraphData from HornSignatureTable where ScanId='"+ strData +"'";	
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetHornScanHistory()
{
    std::string strQuery="select ScanName,ScanID,DateTime from HornResultTable ORDER BY ScanID DESC";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetHornScanResult(std::string strData)
{
    std::string strQuery="select mainParallelResonantFreq,secParallelResonantFreq,thirdParallelResonantFreq,"
                         "firstSeriesResonantFreq,secSeriesResonantFreq,thirdSeriesResonantFreq from HornResultTable where ScanID='" + strData + "'";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetHornScanID(std::string strData)
{
    std::string OldID,NewID,strResult;
    char *temp = (char*)strData.c_str();
    char *DelimeterPoint = strstr(temp,",");
    if(!DelimeterPoint)
        return "";
    OldID.append(temp,DelimeterPoint - temp);
    NewID.append(DelimeterPoint+1);

    std::string strQuery="UPDATE HornResultTable SET ScanName='" + NewID +"' where ScanID='" + OldID + "';";
    m_db.Exec((char*)strQuery.c_str());
    strResult = "1," + strData;
    return strResult;
	
}

std::string DBInterface::GetRecipeInfo(std::string strData)
{
    std::string strQuery="select ProductionSetupTable.RecipeDescription,ProductionSetupTable.ProductionInstruction,WeldRecipeTable.CompanyName,WeldRecipeTable.WeldMode,WeldRecipeTable.ModeValue from ProductionSetupTable, WeldRecipeTable where ProductionSetupTable.RecipeNumber='" + strData + "' and WeldRecipeTable.RecipeNumber = '"+ strData + "';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetRecipeSetupInfo(std::string strData)
{
    std::string strQuery="select CycleCount,ProductionInstruction,RecipeDescription,BatchCountLimit,BatchID,BatchSetupEnable,BatchAlarmEnable from ProductionSetupTable where RecipeNumber=" + strData  + ";";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetRecipeSetupInfo(std::string strData)
{
    std::string recipeNum = "";
    std::string cycleCount = "";
    std::string productionIns = "";
    std::string recipeDesc = "";
    std::string batchCount = "";
    std::string batchID = "";
    std::string batchSetupEnable = "";
    std::string alarmCountEnable = "";
    std::string queryData;
    std::string from, to, details;

    char * dup = NULL;
	char * token = NULL;
     	
   	dup = strdup(strData.c_str());
   	
   	if(!dup)
   	{
   		return "";
   	}
   	
   	token = strtok(dup, ",");
   	
   	if(!token)
   	{
   		free(dup);
   		return "";
   	}

    if (token)
    {
        recipeNum = std::string(token);
        token = strtok(NULL, ",");
     	if(!token)
       	{
       		free(dup);
       		return "";
       	}
        cycleCount = std::string(token);
        token = strtok(NULL, ",");
     	if(!token)
       	{
       		free(dup);
       		return "";
       	}
        productionIns = std::string(token);
        token = strtok(NULL, ",");
     	if(!token)
       	{
       		free(dup);
       		return "";
       	}
        recipeDesc = std::string(token);
        token = strtok(NULL, ",");
        if(!token)
        {
            free (dup);
        	return "";
        }
        batchCount = std::string(token);
        token = strtok(NULL, ",");
     	if(!token)
       	{
       		free(dup);
       		return "";
       	}
        batchID = std::string(token);
        token = strtok(NULL, ",");
     	if(!token)
       	{
       		free(dup);
       		return "";
       	}
        batchSetupEnable = std::string(token);
        token = strtok(NULL, ",");
     	if(!token)
       	{
       		free(dup);
       		return "";
       	}
        alarmCountEnable = std::string(token);
    }
    free(dup);
    std::string strQuery = "update ProductionSetupTable SET CycleCount ="+ cycleCount +", ProductionInstruction = "+ productionIns+", RecipeDescription = "+recipeDesc+", BatchCountLimit ="+ batchCount+", BatchID = "+batchID+", BatchSetupEnable = "+batchSetupEnable+",BatchAlarmEnable = "+alarmCountEnable+" where RecipeNumber='" + recipeNum +"';";
    
    if (cycleCount == "0")
    {
		// cycle count reset
		queryData = "SELECT WeldRecipeName FROM WeldRecipeTable WHERE RecipeNumber=" + recipeNum;
		queryData = m_db.Exec((char*)queryData.c_str()).c_str();
		details = "Identifier:" + queryData + " " + recipeNum + ":";
		
		queryData = "SELECT CycleCount FROM ProductionSetupTable WHERE RecipeNumber=" + recipeNum;
		queryData = m_db.Exec((char*)queryData.c_str()).c_str();
		from = "From:" + queryData + ":";
		to = "To:0:";
		
		details.append(from).append(to);
		DataBaseManager::EnterEventLog (CYCLE_COUNT_RESET, details);
		
		ControlTask::SetCycleCounter(0);
		CommonProperty::weldResult.Set(WeldResults::CYCLE_COUNTER, 0);
    }
    
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetSuspectRejectLimits(std::string strData)
{
	std::string strQuery,strSuspectRejectLimits,strRecipeData,strNewRecipeNumber;
	char *temp = (char*)strData.c_str();
	char *DelimeterPoint = strstr(temp,",");
	if(!DelimeterPoint)
		return "";
	strSuspectRejectLimits.append(temp,DelimeterPoint - temp);
	strRecipeData.append(DelimeterPoint+1);

	temp = (char*)strRecipeData.c_str();
	DelimeterPoint = strstr(temp,",");
	if(!DelimeterPoint)
		return "";
	strNewRecipeNumber.append(temp,DelimeterPoint - temp);

	//if system is ASX
	if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
	{
        if(! ValidateSuspectRejectLimitsForASX(strData))
        {
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_NOT_VALID_NCA);
			return "";
		}
	}		
	
	if("0" == strNewRecipeNumber)
	{
		if (m_SuspectRejectLimitsZero.compare (strRecipeData) != 0)
			LogSuspectRejectLimits (strRecipeData);
		
		m_SuspectRejectLimitsZero = strRecipeData;
		return "";
	}
	else
	{
		strQuery="delete from SuspectRejectLimits where RecipeNumber=" + strSuspectRejectLimits + ";";
		RetryMechanism ((char*)strQuery.c_str());

		if(strNewRecipeNumber.compare(strSuspectRejectLimits) != 0)
		{
			strQuery="delete from SuspectRejectLimits where RecipeNumber=" + strNewRecipeNumber + ";";
			RetryMechanism ((char*)strQuery.c_str());
		}
		
		// makes sure there is m_SuspectRejectLimitsZero first time ever
		if (m_SuspectRejectLimitsZero.empty())
			m_SuspectRejectLimitsZero = strRecipeData;

		strQuery="Insert into SuspectRejectLimits values(" + strRecipeData + ");";
		return RetryMechanism ((char*)strQuery.c_str());
	}
}

std::string DBInterface::GetSuspectRejectLimits(std::string strData)
{
	if("0" == strData)
	{
		return m_SuspectRejectLimitsZero;
	}
	else
	{
		std::string strQuery = "select RecipeNumber, WeldRecipeVerNumber, GlobalSuspect, GlobalReject, Time, Energy,PeakPower,AbsoluteDistance, CollapseDistance, TriggerDistance,EndWeldForce,Frequency from SuspectRejectLimits where RecipeNumber = " + strData + ";";
		return m_db.Exec((char*)strQuery.c_str());
	}
}

std::string DBInterface::SetWeldRecipeSetupInfo(std::string strData)
{
    std::string strQuery,strRecipeNumber,strRecipeData,strNewRecipeNumber;
    char *temp = (char*)strData.c_str();
    char *RecipeNumberPoint = strstr(temp,",");
    if(!RecipeNumberPoint)
        return "";
    strRecipeNumber.append(temp,RecipeNumberPoint - temp);
    strRecipeData.append(RecipeNumberPoint+1);
    temp = (char*)strRecipeData.c_str();
    RecipeNumberPoint = strstr(temp,",");
    if(!RecipeNumberPoint)
        return "";
    strNewRecipeNumber.append(temp,RecipeNumberPoint - temp);

    strQuery="delete from ProductionSetupTable where RecipeNumber=" + strRecipeNumber + ";";
    RetryMechanism ((char*)strQuery.c_str());

    if(strNewRecipeNumber.compare(strRecipeNumber) != 0)
    {
        strQuery="delete from ProductionSetupTable where RecipeNumber=" + strNewRecipeNumber + ";";
        RetryMechanism ((char*)strQuery.c_str());
    }

    strQuery="Insert into ProductionSetupTable values(" + strRecipeData + ");";
    return RetryMechanism ((char*)strQuery.c_str());
}

std::string DBInterface::UserLogin(std::string strData)
{
    std::string strQuery="select UserID from UserProfilesTable where Status='ENABLED' AND User='"+ strData +"';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetAllUsers()
{
	std::string strQuery="select UserID,Password,UserLevel,UserStatus,OperatorPrivileges,DateTime,lastChangedPassword,lastLogin,isLocked,NumLoginAttempt,TimeLoginFrom, TimeLoginTo from UserProfiles ORDER BY UserID ASC";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetUserInfo(std::string strData)
{
    std::string strQuery,strUserID;
    char *temp = (char*)strData.c_str();
    char *UserIDPoint = strstr(temp,",");
    if(!UserIDPoint)
        return "";
    strUserID.append(temp,UserIDPoint - temp);

    strQuery="delete from UserProfiles where UserID=" + strUserID + ";";
    m_db.Exec((char*)strQuery.c_str());

    strQuery="Insert into UserProfiles values(" + strData + ");";
    return RetryMechanism ((char*)strQuery.c_str());
}

std::string DBInterface::SetActiveRecipe(std::string strData)
{
	std::string retVal;
    std::string strQuery="update WeldRecipeTable SET IsActive=0";
    std::string strActiveWeldRecipe, strProperStringFormatToStoreInDB;
	vector<string> vRecipeParams;
    m_db.Exec((char*)strQuery.c_str());
    
    strQuery="update WeldRecipeTable SET IsActive=1 where RecipeNumber='" + strData + "';";
    retVal = m_db.Exec((char*)strQuery.c_str());
    
    //update recipe 0 with the newly activated recipe
    //weld recipe 0
    strQuery = "select * from WeldRecipeTable where RecipeNumber=" + strData+";"; 
    strActiveWeldRecipe = m_db.Exec((char*)strQuery.c_str());
    
	/* Proper string format to update in recipe zero and store in DB again  for active recipe parameters */
	CP->StringToTokens(strActiveWeldRecipe.c_str(),vRecipeParams,',');
	strProperStringFormatToStoreInDB.append("'");
	strProperStringFormatToStoreInDB.append(vRecipeParams[WR_DB_WELD_RECIPE_NAME].c_str());
	strProperStringFormatToStoreInDB.append("'");
	vRecipeParams[WR_DB_WELD_RECIPE_NAME] = strProperStringFormatToStoreInDB;
	
	strProperStringFormatToStoreInDB.clear();
	strProperStringFormatToStoreInDB.append("''");
	vRecipeParams[WR_DB_COMPANY_NAME] = strProperStringFormatToStoreInDB;
	CP->TokensToString(strActiveWeldRecipe,vRecipeParams,',');
    
    m_WeldRecipeZero = strActiveWeldRecipe;
    
    //stack recipe 0
    strQuery = "select * from StackRecipeTable where RecipeNumber=" + strData+";"; 
    m_StackRecipeZero = m_db.Exec((char*)strQuery.c_str());
    
    //suspect reject limits 0
    m_SuspectRejectLimitsZero = "0,0,";
    strQuery = "select GlobalSuspect from SuspectRejectLimits where RecipeNumber=" + strData+";"; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append(",");
    strQuery = "select GlobalReject from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append(",'");
    strQuery = "select Time from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("','");
    strQuery = "select Energy from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("','");
    strQuery = "select PeakPower from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("','");
    strQuery = "select AbsoluteDistance from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("','");
    strQuery = "select CollapseDistance from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("','");
    strQuery = "select TriggerDistance from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("','");
    strQuery = "select EndWeldForce from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("','");
    strQuery = "select Frequency from SuspectRejectLimits where RecipeNumber=" + strData; 
    m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SuspectRejectLimitsZero.append("'");
    
    //setup limits 0
    m_SetupLimitsZero = "0,0,";
    strQuery = "select GlobalSetup from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append(",'");
    strQuery = "select WeldMode from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select WeldAmplitude from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select WeldForce from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select TriggerForce from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select HoldForce from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select HoldTime from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select ScrubAmplitude from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select ScrubTime from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select Reactivity from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("','");
    strQuery = "select ForceLevel from SetupLimits where RecipeNumber=" + strData; 
    m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    m_SetupLimitsZero.append("'");
    
    WriteRecipeZeroToFile();
    
    return retVal;
}

std::string DBInterface::GetEventLogs()
{
    std::string getQuery = "select EventNum,EventName,DateTime,PSSerialNo,ACSerialNo,AUXSerialNo,UserID,EventID,Identifier,Old,New,Comment from EventLogTable ORDER BY EventNum DESC LIMIT 100;";
    std::string returnQuery = m_db.Exec((char*) getQuery.c_str());
    
    return returnQuery;
}


/**************************************************************************//**
* \brief   - Channels down event logs sets from the UI for further processing
*
* \param   - string 	strData
*
* \return  - string		response
*
******************************************************************************/
std::string DBInterface::SetEventLogs(std::string strData)
{
	std::string response;
    EventLogComment (strData);
    return response;
}


/**************************************************************************//**
* \brief   - Extracts user log comments associated with a specific requests
* 			 to be logged in the event log and sends it to the DB task
* 			 for further processing.
* 			 i.e. 	Comment: eventData: 'N/A','USER CREATED','Hello World','Default123'
* 			
* \param   - string& 	eventData
*
* \return  - STATUS		status
*
******************************************************************************/
STATUS DBInterface::EventLogComment (string& eventData)
{
	STATUS status = OK;
	Message message;
	string comment, eventName, userName;
	stringstream request;
	MSG_Q_ID DB_MSG_Q_ID = CP->GetMsgQId(cTaskName[DB_T]);
	message.msgID = TO_DB_TASK_LOG_EVENT_COMMENT;
	size_t pos;
	
	// clean
	for (;;)	
	{
		pos = eventData.find ('\n');
		if (pos != string::npos)
			eventData.replace (pos, 1, " ");
		else
			break;
	}
	
	// get the event name and comment
	request.str (eventData);
	getline (request, userName,'\'');
	getline (request, userName,'\'');
	getline (request, eventName,'\'');
	getline (request, eventName,'\'');
	getline (request, comment,'\'');
	getline (request, comment,'\'');
	snprintf (message.Buffer, sizeof(message.Buffer), "%s,%s,%s", userName.c_str(), eventName.c_str(), comment.c_str());

	// send it to the event log
	status = SendToMsgQ (message, DB_MSG_Q_ID, NO_WAIT);
	return status;
}


std::string DBInterface::GetWeldSignature(std::string strData)
{
    std::string strQuery,strRecipeNumber="",strCycleCnt="", strRecordNumber="";
    char *temp = (char*)strData.c_str();
    char *DelimeterPoint = strstr(temp,",");
    if(!DelimeterPoint)
        return "";
    strRecipeNumber.append(temp,DelimeterPoint - temp);
    temp = DelimeterPoint + 1;
    DelimeterPoint = strstr(temp, ",");
    if(!DelimeterPoint)
    	return "";
    strCycleCnt.append(temp, DelimeterPoint - temp);
    strRecordNumber.append(DelimeterPoint + 1);
    strQuery="select GraphData from WeldResultSignature where RecipeNumber = " + strRecipeNumber + " and CycleCounter = " + strCycleCnt + " and RecordNumber = " + strRecordNumber +";";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetWeldTWH(std::string strData)
{
    if(strData.length() == 0)
        return "";
    std::string strQuery,strRecipeNumber="",strCycleCnt="", strRowId="";
    char *temp = (char*)strData.c_str();
    char *DelimeterPoint = strstr(temp,",");
    if(!DelimeterPoint)
        return "";
    strRecipeNumber.append(temp,DelimeterPoint - temp);
    temp = DelimeterPoint + 1;
    DelimeterPoint = strstr(temp, ",");
    if(!DelimeterPoint)
    	return "";
    strCycleCnt.append(temp, DelimeterPoint - temp);
    strRowId.append(DelimeterPoint+1);
    strQuery="select TriggerPoint,WeldSonicPoint,HoldPoint from WeldResultTable where RecipeNumber=" + strRecipeNumber + " AND CycleCounter=" + strCycleCnt + " AND rowid=" + strRowId +";";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetWeldHistory(std::string strData)
{
    bool isRecordsPerRecipe = true;
    std::string strQuery = "";
    std::string recipeNumber="",LastCycleCounter="";
    char *temp = (char*)strData.c_str();
    char *cycleCountertemp = strstr(temp,",");
    if(!cycleCountertemp)
        return "";
    recipeNumber.append(temp,cycleCountertemp - temp);
    LastCycleCounter.append(temp+(cycleCountertemp-temp+1));

    // fetch the weld result data for specified recipe number
    if (!recipeNumber.empty())
    {
        strQuery = "select *, rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " ORDER BY ROWID DESC LIMIT 25;";
    }
    else
    {
        // fetch weld result data for all the recipes
        strQuery = "select *, rowid from WeldResultTable ORDER BY ROWID DESC LIMIT 25;";
    }

    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetLastWeldHistory(std::string strData)
{
	bool isRecordsPerRecipe = true;
    std::string recipeNumber,LastCycleCounter;
    char *temp = (char*)strData.c_str();
    char *cycleCountertemp = strstr(temp,",");
    
    if(!cycleCountertemp)
        return "";
    recipeNumber.append(temp,cycleCountertemp - temp);
    LastCycleCounter.append(temp+(cycleCountertemp-temp+1));
    char *temp1 = (char*)LastCycleCounter.c_str();
    
    temp = strstr(temp1,",");
    stringstream ssData(LastCycleCounter.c_str());
    string temp3;
    getline (ssData, temp3, ',');
    getline (ssData, temp3);
	    
    if(temp)
    {
        if(recipeNumber == "")
            isRecordsPerRecipe = false;
        recipeNumber.clear();
        recipeNumber.append(temp1,temp-temp1);
        LastCycleCounter.clear();
        LastCycleCounter.append(temp3);
    }
    
    std::string strQuery = "";

    // fetch the weld result data for specified recipe number
    if (isRecordsPerRecipe)
    {
        strQuery = "select *, rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and ROWID>(select IFNULL((select rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and cyclecounter = "+ LastCycleCounter +" ORDER BY ROWID DESC LIMIT 1),0)) ORDER BY ROWID DESC;";
    }
    else
    {
        // fetch weld result data for all the recipes
        strQuery = "select *, rowid from WeldResultTable where ROWID>(select IFNULL((select rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and cyclecounter = "+ LastCycleCounter +" ORDER BY ROWID DESC LIMIT 1),0)) ORDER BY ROWID DESC;";
    }
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetNextWeldHistory(std::string strData)
{
    bool isRecordsPerRecipe = true;
    std::string recipeNumber="",LastCycleCounter="";
    char *temp = (char*)strData.c_str();
    char *cycleCountertemp = strstr(temp,",");
    if(!cycleCountertemp)
        return "";
    recipeNumber.append(temp,cycleCountertemp - temp);
    LastCycleCounter.append(temp+(cycleCountertemp-temp+1));
    char *temp1 = (char*)LastCycleCounter.c_str();
    temp = strstr(temp1,",");
    if(temp)
    {
        if(recipeNumber == "")
            isRecordsPerRecipe = false;
        
		stringstream ssDataRecipeData(LastCycleCounter.c_str());
		string strTemp;
		getline (ssDataRecipeData, recipeNumber, ',');
		getline (ssDataRecipeData, strTemp);
        
        LastCycleCounter.assign(strTemp);
    }

    std::string strQuery = "";

    // fetch the weld result data for specified recipe number
    if (isRecordsPerRecipe)
    {
        strQuery = "select *, rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and ROWID<(select IFNULL((select rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and cyclecounter = "+ LastCycleCounter +" ORDER BY ROWID DESC LIMIT 1),0)) ORDER BY ROWID DESC LIMIT 25;";
    }
    else
    {
        // fetch weld result data for all the recipes
        strQuery = "select *, rowid from WeldResultTable where ROWID<(select IFNULL((select rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and cyclecounter = "+ LastCycleCounter +" ORDER BY ROWID DESC LIMIT 1),0)) ORDER BY ROWID DESC LIMIT 25;";
    }
    std::cout<<strQuery<<std::endl;

    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetAuthorityLevel(std::string strData)
{
    std::string strQuery="select * from OperatorAuthorityOptions;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::UpdateAuthorityLevel(std::string strData)
{
    std::string strQuery="UPDATE OperatorAuthorityOptions SET " + strData +";";
    return m_db.Exec((char*)strQuery.c_str());

}

std::string DBInterface::GetUserPassword(std::string strData)
{
    std::string strQuery="select Password from UserProfiles where UserID='" + strData +"';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetHornRecipeData(std::string strData)
{
    std::string strQuery,strUserID,strRetString;
    char *temp = (char*)strData.c_str();
    char *UserIDPoint = strstr(temp,",");
    if(!UserIDPoint)
        return "";
    strUserID.append(temp,UserIDPoint - temp);

    strQuery="delete from HornRecipeTable where Psfreq=" + strUserID + ";";
    m_db.Exec((char*)strQuery.c_str());

    strQuery="Insert into HornRecipeTable values(" + strData + ");";
    RetryMechanism ((char*)strQuery.c_str());

    strRetString = "1";
    return strRetString ;

}
std::string DBInterface::SetUserPassword(std::string strData)
{
    std::string strUserID="",strPassword="",strLastChangedPassword="";

	char * dup = NULL;
	char * token = NULL;
	
	dup = strdup(strData.c_str());
	
	if(!dup)
	{
		return "";
	}
	
	token = strtok(dup, ",");
	
	if(!token)
	{
		free(dup);
		return "";
	}

    if (token)
    {
        strUserID = std::string(token);
        token = strtok(NULL, ",");
        if(!token)
		{
			free(dup);
			return "";
		}

        strPassword = std::string(token);
        token = strtok(NULL, ",");
        if(!token)
		{
			free(dup);
			return "";
		}

        strLastChangedPassword = std::string(token);
    }

	free(dup);

    std::string strQuery="update UserProfiles SET Password ='" + strPassword +"', lastChangedPassword = '"+strLastChangedPassword+"' where UserID='" + strUserID +"';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetUserDetails(std::string strData)
{
	std::string strUserID="",strPassword="",strQuery="",queryRet="", NumLoginqueryRet="";
	std::string strLoginTime="", strSendtoUI="";
	char *LoginInfo[3];
    char *ptr;
    char delim[] = ",";
    UINT16 i=0;
	UINT16 NumAttempt;
	char *char_array;
	tm FromTime, ToTime, diff;
    		
    char *temp = (char*)strData.c_str();
    char *userIDPoint = strstr(temp,",");
    if(!userIDPoint)
        return "";
    strUserID.append(temp,userIDPoint - temp);
    strPassword.append(temp+(userIDPoint-temp+1));
    
    strQuery="select isLocked from UserProfiles where UserId = '"+ strUserID +"';";
    queryRet=m_db.Exec((char*)strQuery.c_str());
      
    if (queryRet=="Unlocked")
    {
        strQuery="select UserId, UserLevel, UserStatus, DateTime, lastChangedPassword,lastLogin from UserProfiles where UserId = '"+ strUserID +"' and Password = '"+ strPassword +"';";
        queryRet=m_db.Exec((char*)strQuery.c_str());
        GetDateAndTime(strLoginTime);
      
        if (queryRet=="")
        {
        	strQuery="select NumLoginAttempt, TimeLoginFrom, TimeLoginTo from UserProfiles where UserId = '"+ strUserID +"';";
			queryRet=m_db.Exec((char*)strQuery.c_str());
		
			char_array=(char*)(queryRet.c_str()); 			 
			ptr= strtok(char_array, delim);
	
			while (ptr != NULL)
			{
				LoginInfo[i++] = ptr;
				ptr = strtok(NULL, delim);
			}
			NumAttempt=atoi(LoginInfo[0])+1;
					
			sscanf(LoginInfo[1], "%d/%d/%d %d:%d:%d", &FromTime.tm_year, &FromTime.tm_mon, &FromTime.tm_mday, &FromTime.tm_hour, &FromTime.tm_min, &FromTime.tm_sec);
	
			if (NumAttempt==1)
			{
				strQuery="update UserProfiles SET NumLoginAttempt ='" + CP->ConvertIntToString(NumAttempt)+"', TimeLoginFrom = '"+strLoginTime+"', TimeLoginTo = '"+strLoginTime+"' where UserID='" + strUserID +"';";
			}
			else if ((NumAttempt > 1) && (NumAttempt < MAX_LOGIN_ATTEPTS))
			{
				strQuery="update UserProfiles SET NumLoginAttempt ='" + CP->ConvertIntToString(NumAttempt)+"', TimeLoginTo = '"+strLoginTime+"' where UserID='" + strUserID +"';";
			}
			else if (NumAttempt >= MAX_LOGIN_ATTEPTS)
			{
				sscanf((char*)(strLoginTime.c_str()), "%d/%d/%d %d:%d:%d", &ToTime.tm_year, &ToTime.tm_mon, &ToTime.tm_mday, &ToTime.tm_hour, &ToTime.tm_min, &ToTime.tm_sec);
				getTimedifferenct(FromTime, ToTime, &diff);
	
				if (diff.tm_min>=10)
				{
					strQuery="update UserProfiles SET isLocked='Locked', NumLoginAttempt ='" + CP->ConvertIntToString(NumAttempt)+"', TimeLoginTo = '"+strLoginTime+"' where UserID='" + strUserID +"';";
					strSendtoUI="Locked";
				}
				else
				{
					strSendtoUI = CP->ConvertIntToString(NumAttempt);
					strQuery="update UserProfiles SET NumLoginAttempt ='" + CP->ConvertIntToString(NumAttempt)+"', TimeLoginTo = '"+strLoginTime+"' where UserID='" + strUserID +"';";
				}
			}
			m_db.Exec((char*)strQuery.c_str());
         }
         else
  	     {
        	 strQuery="update UserProfiles SET isLocked='Unlocked', NumLoginAttempt ='0', TimeLoginFrom = '"+strLoginTime+"', TimeLoginTo = '"+strLoginTime+"' where UserID='" + strUserID +"';";
        	 m_db.Exec((char*)strQuery.c_str());
        	 strSendtoUI=queryRet;
  	     }
     }
     else
     {
    	 strSendtoUI="Locked";
     }
  	 return strSendtoUI;
}

std::string DBInterface::SetGlobalSettings(std::string strData)
{
    std::string strQuery="UPDATE GlobalSettings SET " + strData;
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetGlobalSettings(std::string strData)
{
    std::string strQuery="select * from GlobalSettings";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetSeekScanHistory()
{
    std::string strQuery="select EndFrequency,DateTime from SeekResults";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetSeekRecipe(std::string strData)
{
    std::string strQuery = "select SeekTime,SeekRampTime,SeekFreqOffset,SeekMemClearBeforeSeek,SeekAmplitudeLoopC1,SeekAmplitudeLoopC2,SeekFreqLimitHigh,SeekFreqLimitLow,SeekPhaseloopI,SeekPhaseloopCF from SeekRecipeTable where Psfreq='"+ strData +"'";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetSeekRecipe(std::string strData)
{
    std::string strQuery,strUserID,strRetString;
    char *temp = (char*)strData.c_str();
    char *UserIDPoint = strstr(temp,",");
    if(!UserIDPoint)
        return "";
    strUserID.append(temp,UserIDPoint - temp);

    strQuery="delete from SeekRecipeTable where Psfreq=" + strUserID + ";";
    m_db.Exec((char*)strQuery.c_str());

    strQuery="Insert into SeekRecipeTable values(" + strData + ");";
    RetryMechanism ((char*)strQuery.c_str());

    strRetString = "1";
    return strRetString ;
}


/**************************************************************************//**
* \brief   - reading the stack recipe from database.
*
* \param   - string strData  // the data input from UI
*
* \return  - the query from select statement
*
******************************************************************************/
std::string DBInterface::GetStackRecipe(std::string strData)
{
	std::string qstrQuery="";
	vector<string> vStrdata;
	std::string m_stackRecipeRead;
	std:string strQuery, strReturnQuery;
	UINT16 RecordCnt=0;
	 INT32 errorCode;
	
    strQuery="select count(*) from StackRecipeTable;";
	strReturnQuery=m_db.Exec((char*)strQuery.c_str(), &errorCode);

	
	RecordCnt=atoi(strReturnQuery.c_str());

	if(("0" == strData)|| (RecordCnt==0))
	{
	    vStrdata = split (m_StackRecipeZero, ",");
	    m_stackRecipeRead.append(vStrdata.at(1));
	    m_stackRecipeRead.append(",");
	    m_stackRecipeRead.append(vStrdata.at(2));
	    m_stackRecipeRead.append(",");
	    m_stackRecipeRead.append(vStrdata.at(3));
	    m_stackRecipeRead.append(",");
	    m_stackRecipeRead.append(vStrdata.at(4));
	    return m_stackRecipeRead;
	}
	else
	{
		qstrQuery = "select DigitialTune, InternalOffsetFlag, InternalFreqOffset, EndOfWeldStore from StackRecipeTable where RecipeNumber = "+ strData + " LIMIT 1;";
		
	    return m_db.Exec((char*)qstrQuery.c_str());
	}
	
}

/**************************************************************************//**
* \brief   - insert the stack recipe to the database.
*
* \param   - string strData  // the data input from UI
*
* \return  - status string
*
******************************************************************************/

std::string DBInterface::SetStackRecipe (std::string strData, bool save)
{
	    std::string strQuery,strRecipeNumber="", strRecipeData="", strReturnQuery="";
	    std::string retString="0";
	    INT16 RecordCnt=0;
	    INT32 errorCode;
		char *temp = (char*)strData.c_str();
		char *DelimeterPoint = strstr(temp,",");
		bool bNewRecipe = false;
		
		if(!DelimeterPoint)
			return "";
		
		strRecipeNumber.append(temp,DelimeterPoint - temp);

		strQuery = "select DigitialTune, InternalOffsetFlag, InternalFreqOffset, EndOfWeldStore from StackRecipeTable where RecipeNumber = "+ strRecipeNumber + " LIMIT 1;";
		if ((m_db.Exec((char*)strQuery.c_str(), &errorCode).empty()) && errorCode == SQLITE_OK)
			bNewRecipe = true;
			
		vector<string> vStrdata = split (strData, ",");

		// force recipe number to zero, so changes do not go to the DB
		if (!save && !bNewRecipe)
			strRecipeNumber = "0";		
		
		if("0" == strRecipeNumber)
		{
			m_StackRecipeZero = strData;
			WriteRecipeZeroToFile();
		}
		else
		{
			m_StackRecipeZero= strData;
		    WriteRecipeZeroToFile();
		    
		    strQuery="select count(*) from StackRecipeTable where RecipeNumber=" + strRecipeNumber + ";";
			strReturnQuery=m_db.Exec((char*)strQuery.c_str(), &errorCode);
			
			RecordCnt=atoi(strReturnQuery.c_str());
			if(RecordCnt>=1)
			{
				strQuery="UPDATE StackRecipeTable SET DigitialTune ='" +vStrdata.at(1) +"',InternalOffsetFlag  = '" +vStrdata.at(2)  +"', InternalFreqOffset= '" +vStrdata.at(3)+"', EndOfWeldStore= '"+vStrdata.at(4)  +"' where RecipeNumber="+strRecipeNumber+";";
				m_db.Exec((char*)strQuery.c_str(), &errorCode);
				
				if(SQLITE_OK != errorCode)
					LOGERR("Unable to insert data to StackRecipeTable",0,0,0);
			}
			else if (RecordCnt==0)
			{
				strQuery="Insert into StackRecipeTable values(" + strData + ");";
              	strReturnQuery=m_db.Exec((char*)strQuery.c_str(),  &errorCode);
			  	
              	if(SQLITE_OK != errorCode)
		   			LOGERR("Unable to insert data to StackRecipeTable",0,0,0);
			}

		}

    retString = "1";
    return retString ;
}

std::string DBInterface::GetSeekSignatureData()
{
    std::string strQuery = "select time,amplitude,frequency,power,current,phase from SeekSignature ORDER BY time ASC;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetTestModeGraphData(std::string strData)
{
    std::string strQuery = "select time,amplitude,frequency,power,current,phase from TestSignature ORDER BY time ASC;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetTestModeRecipeData(std::string strData)
{
    std::string strQuery = "select TestAmplitude, AmplitudeLoopC1 ,AmplitudeLoopC2, PhaseLoopI, PhaseLoopCF ,FrequencyHigh ,FrequencyLow from TestRecipe where PsFreq="+strData;
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::SetTestModeRecipeData(std::string strData)
{
    std::string strQuery,strUserID,strRetString;
    char *temp = (char*)strData.c_str();
    char *UserIDPoint = strstr(temp,",");
    if(!UserIDPoint)
        return "";
    strUserID.append(temp,UserIDPoint - temp);

    strQuery="delete from TestRecipe where PsFreq=" + strUserID + ";";
    m_db.Exec((char*)strQuery.c_str());

    strQuery="Insert into TestRecipe values(" + strData + ");";
    m_db.Exec((char*)strQuery.c_str());

    strRetString = "1";
    return strRetString ;
}

std::string DBInterface::GetTestModeResultData(std::string strData)
{
    std::string strQuery = "select frequency,memory,amplitude,power from TestResult";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetDBVersion()
{
    std::string strQuery = "select VersionNo from DBVersion";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetResonantFrequency(std::string strData)
{
    std::string strQuery = "select EndFrequency from SeekResults";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetRecipeNumberFromDbForAddRecipe()
{
    std::string strQuery = "SELECT MAX(RecipeNumber) FROM WeldRecipeTable;";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetRecipeNumberFromDbForCopyRecipe(std::string strData)
{
    std::string strQuery = "select * from WeldRecipeTable where RecipeNumber='" + strData + "';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetTrendsGraphSignatureData(std::string strData)
{
    std::string strQuery="";
    if (!strData.empty())
    {
        strQuery = "select MaxWeldForce,EndHoldForce,TotalAbsolute,WeldTime,WeldEnergy,WeldPeakPower,StartFrequency,FrequencyChange,CycleCounter,AlarmFlag from WeldResultTable  WHERE RecipeNumber = "+strData+" ORDER BY DateTime ; ";
    }
    else
    {
        strQuery = "select MaxWeldForce,EndHoldForce,TotalAbsolute,WeldTime,WeldEnergy,WeldPeakPower,StartFrequency,FrequencyChange,CycleCounter,AlarmFlag from WeldResultTable ORDER BY DateTime ;";
    }
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetSuspectRejectLimitsForTrendsGraph(std::string strData)
{
    std::string strQuery = "select Time,Energy,PeakPower,AbsoluteDistance,Frequency,EndWeldForce from SuspectRejectLimits WHERE RecipeNumber = "+strData+"; ";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::GetUserIO(std::string strData)
{
    std::string strQuery = "select PinFunction,ActiveStatus,CheckStatus from UserIO";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetUserIO(std::string strData)
{
    std::string strQuery,strRetString;

    if(strData.empty())
        return "0";
    strQuery="delete from UserIO ;";

    //printf("%s",strData.c_str());
    m_db.Exec((char*)strQuery.c_str());

    strQuery="Insert into UserIO(Id,PinName,PinFunction,ActiveStatus,CheckStatus) values" + strData + ";";
    //printf("%s",strData.c_str());
    m_db.Exec((char*)strQuery.c_str());

    strRetString = "1";
    return strRetString ;
}

std::string DBInterface::getAllWeldResultData(std::string strData)
{
    std::string strQuery = "";

    // fetch the weld result data for specified recipe number
    if (!strData.empty())
    {
        strQuery = "select MaxWeldForce, EndHoldForce, TotalAbsolute, CycleCounter, DateTime, WeldTime, WeldEnergy, WeldPeakPower, StartFrequency, FrequencyChange, WeldCollapseDistance, UserName, WeldMode, StackSerialNo, PartID, BatchID from WeldResultTable WHERE RecipeNumber = "+strData+" ORDER BY ROWID DESC;";
    }
    else
    {
        // fetch weld result data for all the recipes
        strQuery = "select MaxWeldForce, EndHoldForce, TotalAbsolute, CycleCounter, DateTime, WeldTime, WeldEnergy, WeldPeakPower, StartFrequency, FrequencyChange, WeldCollapseDistance, UserName, WeldMode, StackSerialNo, PartID, BatchID from WeldResultTable ORDER BY ROWID DESC;";
    }

    //std::cout << strQuery << std::endl;
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::getLastAllWeldResultData(std::string strData)
{
	bool isRecordsPerRecipe = true;
	bool isLastCycleCounterAvailable = true;
    std::string recipeNumber="",LastCycleCounter="";
    char *temp = (char*)strData.c_str();
    char *cycleCountertemp = strstr(temp,",");
    if(!cycleCountertemp)
    {
    	if(strData.empty())
    		isRecordsPerRecipe = false;
    	
    	recipeNumber.append(temp,cycleCountertemp - temp);
    	LastCycleCounter.append("0");
    }
    else
    {
    	recipeNumber.append(temp,cycleCountertemp - temp);
    	LastCycleCounter.append(temp+(cycleCountertemp-temp+1));
    }
    
    std::string strQuery = "";

    // fetch the weld result data for specified recipe number
    if (isRecordsPerRecipe)
    {
        strQuery = "select MaxWeldForce, EndHoldForce, TotalAbsolute, CycleCounter, DateTime, WeldTime, WeldEnergy, WeldPeakPower, StartFrequency, FrequencyChange, WeldCollapseDistance, UserName, WeldMode, StackSerialNo, PartID, BatchID from WeldResultTable WHERE RecipeNumber = "+recipeNumber+" and ROWID>(select IFNULL((select rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and cyclecounter = "+ LastCycleCounter +"),0)) ORDER BY ROWID DESC;";
    }
    else
    {
        // fetch weld result data for all the recipes
        strQuery = "select MaxWeldForce, EndHoldForce, TotalAbsolute, CycleCounter, DateTime, WeldTime, WeldEnergy, WeldPeakPower, StartFrequency, FrequencyChange, WeldCollapseDistance, UserName, WeldMode, StackSerialNo, PartID, BatchID from WeldResultTable where ROWID>(select IFNULL((select rowid from WeldResultTable where RecipeNumber= " + recipeNumber + " and cyclecounter = "+ LastCycleCounter +"),0)) ORDER BY ROWID DESC;";
    }

    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::GetAnalyticsProductionOverView(std::string strData)
{
    std::string fromDate="",toDate="";
    char *temp = (char*)strData.c_str();
    char *datetemp = strstr(temp,",");
    if(!datetemp)
        return "";
    fromDate.append(temp,datetemp - temp);
    toDate.append(temp+(datetemp-temp+1));
    std::string strQuery = "SELECT date(DateTime),COUNT(DateTime) AS AvgAlarmCount FROM AlarmLogs where DateTime between '"+fromDate+"' and '"+toDate+"' GROUP BY DateTime HAVING   COUNT(DateTime) > 1;";
   // std::cout << strQuery << std::endl;
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::GetActiveRecipeNumber(std::string strData)
{
    std::string strQuery = "select RecipeNumber,WeldRecipeName from WeldRecipeTable where IsActive = 1 and RecipeNumber>0;";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::GetBatchCount(std::string strData)
{
    std::string strQuery = "select BatchCountLimit,BatchCounter from ProductionSetupTable where RecipeNumber = '" + strData + "';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetBatchCounter(std::string strData)
{
    std::string strQuery = "UPDATE ProductionSetupTable SET BatchCounter = 0 where RecipeNumber = '" + strData + "';";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetSetupLimits(std::string strData)
{
	std::string strQuery,strSetupLimits,strRecipeData,strNewRecipeNumber;
	char *temp = (char*)strData.c_str();
	char *DelimeterPoint = strstr(temp,",");
	if(!DelimeterPoint)
		return "";
	strSetupLimits.append(temp,DelimeterPoint - temp);
	strRecipeData.append(DelimeterPoint+1);

	temp = (char*)strRecipeData.c_str();
	DelimeterPoint = strstr(temp,",");
	if(!DelimeterPoint)
		return "";
	strNewRecipeNumber.append(temp,DelimeterPoint - temp);
	
//    if(!ValidateSetupLimit(strData))
//    {
//	    AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_NOT_VALID_NCA);
//		return "";
//	}
    
	if("0" == strNewRecipeNumber)
	{
		if (m_SetupLimitsZero.compare (strRecipeData) != 0)
			LogSetupLimits (strRecipeData);

		m_SetupLimitsZero = strRecipeData;
		return "";
	}
	else
	{
		strQuery = "delete from SetupLimits where RecipeNumber=" + strSetupLimits + ";";
		RetryMechanism ((char*)strQuery.c_str());

		if(strNewRecipeNumber.compare(strSetupLimits) != 0)
		{
			strQuery = "delete from SetupLimits where RecipeNumber=" + strNewRecipeNumber + ";";
			RetryMechanism ((char*)strQuery.c_str());
		}
		
		// makes sure there is m_SetupLimitsZero first time ever
		if (m_SetupLimitsZero.empty())
			m_SetupLimitsZero = strRecipeData;

		strQuery="Insert into SetupLimits values(" + strRecipeData + ");";
		return RetryMechanism ((char*)strQuery.c_str());
	}
}
std::string DBInterface::GetSetupLimits(std::string strData)
{
	if("0" == strData)
	{
		return m_SetupLimitsZero;
	}
	else
	{
		std::string strQuery = "select * from SetupLimits where RecipeNumber = " + strData + ";";
		return m_db.Exec((char*)strQuery.c_str());
	}
}
std::string DBInterface::SetRecipeValidate(std::string strData)
{
	string details, queryData, recipeName, validStat;
	char recipeNumber[10];
	
	// record recipe name, number and validation
	queryData = "SELECT WeldRecipeName FROM WeldRecipeTable WHERE RecipeNumber=" + strData;
	recipeName = m_db.Exec((char*)queryData.c_str()).c_str();
	snprintf (recipeNumber, sizeof(recipeNumber), "%s", strData.c_str());
	queryData = "SELECT IsValidate FROM WeldRecipeTable WHERE RecipeNumber=" + strData;
	validStat = m_db.Exec((char*)queryData.c_str()).c_str();
    validStat = validStat == "1" ? "Valid" : "Invalid";
	
	// send it to the event log
	details = "Identifier:" + recipeName + " " + recipeNumber + ":From:" + validStat + ":To:Valid:";
	DataBaseManager::EnterEventLog (RECIPE_VALIDATION, details);
	WeldRecipeStatus::SetRecipeStatus (VALIDATED);
    
    std::string strQuery="update WeldRecipeTable SET IsValidate=1 where RecipeNumber=" + strData + ";";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::SetRecipeUnValidate(std::string strData)
{
	string details, queryData, recipeName, validStat;
	char recipeNumber[10];

	// record recipe name, number and validation
	queryData = "SELECT WeldRecipeName FROM WeldRecipeTable WHERE RecipeNumber=" + strData;
	recipeName = m_db.Exec((char*)queryData.c_str()).c_str();
	snprintf (recipeNumber, sizeof(recipeNumber), "%s", strData.c_str());
	queryData = "SELECT IsValidate FROM WeldRecipeTable WHERE RecipeNumber=" + strData;
	validStat = m_db.Exec((char*)queryData.c_str()).c_str();
    validStat = validStat == "1" ? "Valid" : "Invalid";

	// send it to the event log
	details = "Identifier:" + recipeName + " " + recipeNumber + ":From:" + validStat + ":To:Invalid:";
	DataBaseManager::EnterEventLog (RECIPE_VALIDATION, details);
	
	std::string strQuery="update WeldRecipeTable SET IsValidate=0 where RecipeNumber=" + strData + ";";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::SetRecipeToBLOB(std::string strData)
{
    std::string strQuery="insert into WeldRecipe values(" + strData + "');";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::GetRecipeToBLOB(std::string strData)
{
    std::string strQuery="select WeldRecipeStructure from WeldRecipe where RecipeNumber=" + strData + ";";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::SetWeldForceForActuatorSetUp(std::string strdata)
{
    std::string strQuery="update WeldRecipeTable SET WeldForce="+strdata+" where RecipeNumber>0 and IsActive=1";
    return m_db.Exec((char*)strQuery.c_str());
}
std::string DBInterface::UpdateSystemData(std::string strData)
{
    std::string strQuery="";
    strQuery = "delete from Report " ;
    m_db.Exec((char*)strQuery.c_str());
    strQuery="insert into Report (Type,Frequency,Period,TimeOrCycles,  StorageOptions,OutputFormat,ScheduledFrequency,ScheduledTime, Results, Recipes,Graph ,Overloads,CycleModified,Warnings ,Suspect ,Reject ,NoCycle,HardwareFailure ,NonCycleOverloads,SysSettings,Events) values(" + strData + ");";
    return m_db.Exec((char*)strQuery.c_str());
}

std::string DBInterface::SetFactoryReset(std::string strData)
{
    std::string tempData, details;
    tempData = "90,30,1";
    std::string strQuery = "";

    EraseRecipeZero();
    
    // Deleting the existing records
    strQuery.append("delete from AlarmConfiguration;");
    strQuery.append("delete from AlarmLogs;");
    strQuery.append("delete from BransonOnlyParameters;");
    strQuery.append("delete from Calibration;");
    strQuery.append("delete from EventLogTable;");
    strQuery.append("delete from GlobalSettings;");
    strQuery.append("delete from HornRecipeTable;");
    strQuery.append("delete from HornResultTable;");
    strQuery.append("delete from HornSignatureTable;");
    strQuery.append("delete from OperatorAuthorityOptions;");
    strQuery.append("delete from ProductionSetupTable;");
    strQuery.append("delete from SeekRecipeTable;");
    strQuery.append("delete from SeekResults;");
    strQuery.append("delete from SeekSignature;");
    strQuery.append("delete from SetupLimits;");
    strQuery.append("delete from StackRecipeTable;");
    strQuery.append("delete from SuspectRejectLimits;");
    strQuery.append("delete from TestRecipe;");
    strQuery.append("delete from TestResult;");
    strQuery.append("delete from TestSignature;");
    strQuery.append("delete from UserIO;");
    strQuery.append("delete from UserProfiles;");
    strQuery.append("delete from WeldRecipe;");
    strQuery.append("delete from WeldRecipeTable;");
    strQuery.append("delete from WeldResultSignature;");
    strQuery.append("delete from WeldResultTable;");
    m_db.Exec((char*)strQuery.c_str());

    //updating the upincrement sequence tables
    strQuery = "";
    strQuery.append("UPDATE SQLITE_SEQUENCE SET SEQ=0 WHERE NAME='EventLogTable';");
    strQuery.append("UPDATE SQLITE_SEQUENCE SET SEQ=0 WHERE NAME='HornResultTable';");
    m_db.Exec((char*)strQuery.c_str());

    //inserting defalut values into respective tables
    strQuery = "";
    strQuery.append("insert into AlarmConfiguration values('Overload',0,1,1,0,1);");
    strQuery.append("insert into AlarmConfiguration values('CycleModified',0,1,1,0,1);");
    strQuery.append("insert into AlarmConfiguration values('Warning',0,1,1,0,1);");
    strQuery.append("insert into AlarmConfiguration values('ProcessSuspect',0,1,1,1,1);");
    strQuery.append("insert into AlarmConfiguration values('ProcessReject',0,1,1,0,1);");
    strQuery.append("insert into AlarmConfiguration values('NoCycle',0,1,1,1,0);");
    strQuery.append("insert into AlarmConfiguration values('HardwareFailure',0,1,1,0,0);");
    strQuery.append("insert into AlarmConfiguration values('NoCycleOverload',0,1,1,1,0);");
    m_db.Exec((char*)strQuery.c_str());
    strQuery = "insert into GlobalSettings values(90,30,0);";
    m_db.Exec((char*)strQuery.c_str());
    strQuery.clear();
    strQuery.append("insert into HornRecipeTable values(20000,1,10,10,1,19450,20450,10);");
    strQuery.append("insert into HornRecipeTable values(30000,1,10,10,1,29250,30750,10);");
    strQuery.append("insert into HornRecipeTable values(40000,1,10,10,1,38900,40900,10);");
    m_db.Exec((char*)strQuery.c_str());
    strQuery.clear();
    strQuery.append("insert into OperatorAuthorityOptions values(0,0,0,0,0,0,0);");
    strQuery.append("insert into SeekRecipeTable values(20000,500,80,0,0,205,370,500,500,140,200);");
    strQuery.append("insert into SeekRecipeTable values(30000,500,80,0,0,205,370,750,750,100,200);");
    strQuery.append("insert into SeekRecipeTable values(40000,500,80,0,0,205,370,1000,1000,60,200);");
    m_db.Exec((char*)strQuery.c_str());
    strQuery.clear();
    strQuery.append("insert into UserIO values(1,'J17-01',1,1,1);");
    strQuery.append("insert into UserIO values(2,'J17-02',2,1,1);");
    strQuery.append("insert into UserIO values(3,'J17-03',3,1,1);");
    strQuery.append("insert into UserIO values(4,'J17-04',0,1,0);");
    strQuery.append("insert into UserIO values(5,'J17-11',4,1,0);");
    strQuery.append("insert into UserIO values(6,'J17-12',6,1,1);");
    strQuery.append("insert into UserIO values(7,'J17-13',0,1,0);");
    strQuery.append("insert into UserIO values(8,'J17-16',0,1,0);");
    strQuery.append("insert into UserIO values(9,'J17-23',0,1,0);");
    strQuery.append("insert into UserIO values(10,'J17-07',1,1,1);");
    strQuery.append("insert into UserIO values(11,'J17-08',2,1,1);");
    strQuery.append("insert into UserIO values(12,'J17-09',3,1,1);");
    strQuery.append("insert into UserIO values(13,'J17-10',4,1,1);");
    strQuery.append("insert into UserIO values(14,'J17-19',10,1,1);");
    strQuery.append("insert into UserIO values(15,'J17-20',5,1,1);");
    strQuery.append("insert into UserIO values(16,'J17-21',9,1,1);");
    strQuery.append("insert into UserIO values(17,'J17-22',8,1,1);");
    strQuery.append("insert into UserIO values(18,'J17-17',1,0,1);");
    strQuery.append("insert into UserIO values(19,'J17-18',2,0,1);");
    strQuery.append("insert into UserIO values(20,'J17-24',1,0,1);");
    strQuery.append("insert into UserIO values(21,'J17-25',2,0,1);");
    strQuery.append("insert into UserIO values(22,'J58-01',0,1,0);");
    strQuery.append("insert into UserIO values(23,'J58-02',1,1,0);");
    strQuery.append("insert into UserIO values(24,'J58-03',3,1,1);");
    strQuery.append("insert into UserIO values(25,'J58-09',4,1,1);");
    strQuery.append("insert into UserIO values(26,'J58-10',0,1,0);");
    strQuery.append("insert into UserIO values(27,'J58-05',1,1,1);");
    strQuery.append("insert into UserIO values(28,'J58-06',2,1,1);");
    strQuery.append("insert into UserIO values(29,'J58-07',3,1,1);");
    strQuery.append("insert into UserIO values(30,'J58-13',0,1,0);");
    strQuery.append("insert into UserIO values(31,'J58-14',0,1,0);");
    m_db.Exec((char*)strQuery.c_str());
    
    strQuery.clear();
    strQuery.append("insert into TestRecipe values(20000,205,370,140,200,500,500,50);");
    strQuery.append("insert into TestRecipe values(30000,205,370,100,200,750,750,50);");
    strQuery.append("insert into TestRecipe values(40000,205,370,60,200,1000,1000,50);");
    m_db.Exec((char*)strQuery.c_str());
    
    strQuery.clear();
    strQuery.append("insert into UserProfiles values('ADMIN','123456Aa#','Executive','1111','Enabled','2018/01/01 00:00:01','2018/01/01 00:00:01','2018/01/01 00:00:01','Unlocked','0','2018/01/01 00:00:01','2018/01/01 00:00:01');");
        
	details = "Identifier:N/A:From:N/A:To:N/A:";
	DataBaseManager::EnterEventLog (SYSTEM_FACTORY_RESET, details);
    
    return m_db.Exec((char*)strQuery.c_str());
}

/**************************************************************************//**
* \brief  - Decodes the received buffer from the message queue into a
* 			structured message.
*
* \param  - char* buff
* \param  - Message& ReqMessage
*
* \return  - None
*
******************************************************************************/
void DBInterface::Decode(char* buff, Message& ReqMessage)
{
	memcpy(&ReqMessage.msgID,buff,sizeof(ReqMessage.msgID));
	strcpy(ReqMessage.Buffer,buff+4);
}

/**
 * @brief DBInterface::UpdateUserLogin
 * @param strData
 * @return
 */
std::string DBInterface::UpdateUserLogin(std::string strData)
{
    std::string strUserID = "";
    std::string lastLoginDate = "";

    char * dup = strdup(strData.c_str());
    char * token = strtok(dup, ",");

    if (token)
    {
        strUserID = std::string(token);
        token = strtok(NULL, ",");
        lastLoginDate = std::string(token);
    }

    free(dup);

    std::string strQuery="update UserProfiles SET lastLogin ='" + lastLoginDate +"' where UserID='" + strUserID +"';";
    return m_db.Exec((char*)strQuery.c_str());
}

/**************************************************************************//**
* \brief   - Save web services data into DB
*
* \param   - std::string strData
*
* \return  - std::string
*
******************************************************************************/
std::string DBInterface::SaveWebServicesStatus(std::string strData)
{
	Eeprom epclass;
	stringstream switches, old_switches;
	string web_enable, foe_enable, old_web_enable, old_foe_enable, details;
	char chData[WEBSERVICE_FOE_ENABLE_SIZE+1]={0};
	
	// read old values
	epclass.Read(chData,WEBSERVICE_FOE_ENABLE_SIZE,WEBSERVICE_FOE_ENABLE_OFFSET);
	old_switches.str(chData); 
	getline (old_switches, old_web_enable, ',');
	getline (old_switches, old_foe_enable);
	
	epclass.Write((char *)strData.c_str(),WEBSERVICE_FOE_ENABLE_SIZE,WEBSERVICE_FOE_ENABLE_OFFSET);
	
	switches.str(strData.c_str());
	getline (switches, web_enable, ',');
	getline (switches, foe_enable);

	if (web_enable != old_web_enable)
	{
		old_web_enable = strcmp (old_web_enable.c_str(), "1") ? "OFF": "ON";
		web_enable = strcmp (web_enable.c_str(), "1") ? "OFF": "ON";
		
		details = "Identifier:Web Services Enable:From:" + old_web_enable + ":To:" + web_enable + ":";
		DataBaseManager::EnterEventLog  (WEB_SERVICES, details);
	}
	if (foe_enable != old_foe_enable)
	{
		old_foe_enable = strcmp (old_foe_enable.c_str(), "1") ? "OFF": "ON";
		foe_enable = strcmp (foe_enable.c_str(), "1") ? "OFF": "ON";
		
		details = "Identifier:Upgrade Over Ethernet Enable:From:" + old_foe_enable + ":To:" + foe_enable + ":";
		DataBaseManager::EnterEventLog  (WEB_SERVICES, details);
	}

	return "";
}

/**************************************************************************//**
* \brief   - Return web services data from DB
*
* \param   - None
*
* \return  - std::string
*
******************************************************************************/
std::string DBInterface::GetServicesStatus()
{
	Eeprom epclass;
	char chData[WEBSERVICE_FOE_ENABLE_SIZE+1]={0};

	epclass.Read(chData,WEBSERVICE_FOE_ENABLE_SIZE,WEBSERVICE_FOE_ENABLE_OFFSET);
	return chData;
}

/**************************************************************************//**
* \brief   - Return webservices actual user logged and its level, if nobody is logged
* into the webservices return NO_WS_USER and NO_LEVEL
*
* \param   - None
*
* \return  - std::string
*
******************************************************************************/
std::string DBInterface::GetWSUserAndLevel()
{
	char chUserNameAndLevel[BUFFER_LENGTH] = {0x00};
	char chUserLevel[USERDATA_LENGTH] = {0x00};
	char chLogStatus[USERDATA_LENGTH] = {0x00};
	
	sprintf(chUserLevel, "%d", CP->WSUserLevelStatus);
	sprintf(chLogStatus, "%d", CP->bIsWebServicesLogged);
	
	strcat(chUserNameAndLevel, CP->WSLoggedUserName.c_str());
	strcat(chUserNameAndLevel, ",");
	strcat(chUserNameAndLevel, chUserLevel);
	strcat(chUserNameAndLevel, ",");
	strcat(chUserNameAndLevel, chLogStatus);
	
	return chUserNameAndLevel;
}

/**************************************************************************//**
* \brief   - Returns the recipe zero context.
*
* \param   - None
*
* \return  - string m_RecipeZero
*
******************************************************************************/
string DBInterface::GetRecipeZero()
{
	string recipeData = "";

	if((!m_WeldRecipeZero.empty()) && (!m_SuspectRejectLimitsZero.empty()) 
			&& (!m_BransonOnlyZero.empty()) && (!m_SetupLimitsZero.empty()))
	{
		recipeData.append(m_WeldRecipeZero);
		recipeData.append(",");
		recipeData.append(m_SuspectRejectLimitsZero);
		recipeData.append(",");
		recipeData.append(m_BransonOnlyZero);
		recipeData.append(",");
		recipeData.append(m_SetupLimitsZero);

		if(m_WeldRecipeZero.at(0) != '0')
			WeldRecipeStatus::SetRecipeStatus (SAVED);
	}
	else
		recipeData = m_WeldRecipeZero;
	
	return recipeData;
}

/**************************************************************************//**
* \brief   - Reads recipe zero from file and loads into m_RecipeZero.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DBInterface::LoadRecipeZero()
{
	string recipeZero[5] = {"", "", "", "", ""};
	string temp;
	fstream fp;
	UINT8 idx = 0;

	/* Read from file */
	if(CP->IsFileExist(RECIPE_ZERO_PATH))
	{
		fp.open(RECIPE_ZERO_PATH, ios::in);

		if(fp.is_open())
		{
			while(getline(fp,temp))
			{
				if(idx < 5)
				{
					recipeZero[idx] = temp;
				}
				idx++;
			}
			
			fp.close();
			
			// make sure all sections of the file were read
			if (idx < 4)
			{
				LOGERR("UI_DB_T : Recipe zero file missing records",0,0,0);
				WriteDefaultRecipeZero();
				m_RecipeZerodb.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
			}
			else
			{
				m_WeldRecipeZero 			= recipeZero[0];
				m_SuspectRejectLimitsZero 	= recipeZero[1];
				m_BransonOnlyZero			= recipeZero[2];
				m_SetupLimitsZero			= recipeZero[3];
				m_StackRecipeZero			= recipeZero[4];
			}
		}
		else
		{
			LOGERR("UI_DB_T : Recipe zero file open failed",0,0,0);
		}
	}
	else
	{
	   WriteDefaultRecipeZero();
	   m_RecipeZerodb.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );

	}
}

/**************************************************************************//**
* \brief   - Writes the current recipe zero into a file. If a record
* 			is empty or contains unrecognized characters then the default
* 			record for that section will be saved instead.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DBInterface::WriteRecipeZeroToFile()
{
	string recipeData;
	fstream fp;
	vector<string> vRecordParams;
	bool bNewRecipe = false;
	unsigned short recipeZeroCrc ;bool insertResult;
	// if new, get recipe number
	SetDefaultRecipeZero();
	CP->StringToTokens(m_WeldRecipeZero.c_str(),vRecordParams,',');
	if (vRecordParams[WR_DB_WELD_RECIPE_VERSION_NUMBER] == "0")
	{
		bNewRecipe = true;
	}
	
	if (m_WeldRecipeZero.empty() || !IsValidRecipeRecord (m_WeldRecipeZero))
	{
		if (!bNewRecipe)
		{
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_WELD_CORRUPTED_WRA);
		}
		
		m_WeldRecipeZero = m_DefaultWeldRecipeZero;
	}
	
	if (m_SuspectRejectLimitsZero.empty() || !IsValidRecipeRecord (m_SuspectRejectLimitsZero))
	{
		if (!bNewRecipe)
		{
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_LIMITS_CORRUPTED_WRA);
		}
		
		m_SuspectRejectLimitsZero = m_DefaultSuspectRejectLimitsZero;
	}
	
	if (m_BransonOnlyZero.empty() || !IsValidRecipeRecord (m_BransonOnlyZero))
	{
		m_BransonOnlyZero = m_DefaultBransonOnlyZero;
	}
	
	if (m_SetupLimitsZero.empty() || !IsValidRecipeRecord (m_SetupLimitsZero))
	{
		if (!bNewRecipe)
		{
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_SETUP_CORRUPTED_WRA);
		}

		m_SetupLimitsZero = m_DefaultSetupLimitsZero;
	}
	
	if (m_StackRecipeZero.empty() || !IsValidRecipeRecord (m_StackRecipeZero))
	{
		if (!bNewRecipe)
		{
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RECIPE_STACK_CORRUPTED_WRA);
		}

		m_StackRecipeZero = m_DefaultStackRecipeZero;
	}

	recipeData.append(m_WeldRecipeZero);
	recipeData.append("\n");
	recipeData.append(m_SuspectRejectLimitsZero);
	recipeData.append("\n");
	recipeData.append(m_BransonOnlyZero);
	recipeData.append("\n");
	recipeData.append(m_SetupLimitsZero);
	recipeData.append("\n");
	recipeData.append(m_StackRecipeZero);
	recipeData.append("\n");
	
	fp.open(RECIPE_ZERO_PATH, ios::out);
	if(fp.is_open())
	{
		fp << recipeData;
		fp.close();
		recipeZeroCrc= m_RecipeZerodb.CalCrcRecipeZeroFile(0,RECIPE_ZERO_PATH);
		insertResult=m_RecipeZerodb.InsertCrcInRecipeZeroFile(recipeZeroCrc);
		if(insertResult)
		{
			m_RecipeZerodb.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
		}
		else
		{
			LOGERR("UI_DB_T : InsertCrcInRecipeZeroFile failed in WriteRecipeZeroToFile() ",0,0,0);
		}
	}
	else
	{
		LOGERR("Error on writing into recipe zero file. File open failed",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Checks recipe zero records to contains only recognized
* 			characters, with some exceptions. 
*
* \param   - string recipeRecord
*
* \return  - bool bValid
*
******************************************************************************/
bool DBInterface::IsValidRecipeRecord (string recipeRecord)
{
	bool bValid = false;
	UINT32 vIdx = 0;
	string::iterator it = recipeRecord.begin();
	vector<string> vRecordParams;
	
	// specific record here
	if (m_WeldRecipeZero == recipeRecord)
	{
		// break the record
		CP->StringToTokens(recipeRecord.c_str(),vRecordParams,',');
		
		// scan the record
		for (vIdx = 0; vIdx < vRecordParams.size(); ++vIdx)
		{
			// exclude record sections here
			if (vIdx != 1)
			{
				// check if recognized
				for (it = vRecordParams[vIdx].begin(); it != vRecordParams[vIdx].end(); ++it)
				{
					if ((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z') || (*it >= '0' && *it <= '9') || (*it == ',') || (*it == '\'') || (*it == ':'))
					{
						bValid = true;	
					}
					else
					{
						bValid = false;
						break;
					}
				}

				if (!bValid)
					break;
			}
		}
	}
	else
	{
		// check if recognized
		for (it = recipeRecord.begin(); it != recipeRecord.end(); ++it)
		{
			if ((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z') || (*it >= '0' && *it <= '9') || (*it == ',') || (*it == '\'') || (*it == ':'))
			{
				bValid = true;	
			}
			else
			{
				bValid = false;
				break;
			}
		}
	}
	
	return bValid;
}

/**************************************************************************//**
* \brief   - Erases recipe zero data from file on factory reset.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DBInterface::EraseRecipeZero()
{
	fstream fp;
	
	m_WeldRecipeZero.clear();
	m_SuspectRejectLimitsZero.clear();
	m_BransonOnlyZero.clear();
	m_SetupLimitsZero.clear();
	m_StackRecipeZero.clear();
	
	if(CP->IsFileExist(RECIPE_ZERO_PATH))
	{
		fp.open(RECIPE_ZERO_PATH, fstream::in | fstream::out | fstream::trunc);
		if(fp.is_open())
		{	
			fp.close();
		}
		else
		{
			LOGERR("UI_DB_T : Error on truncating recipe zero file",0,0,0);
		}
	}
	else
	{
		LOGERR("UI_T : Truncate - Recipe zero file does not exists",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - The function re-tries the same query if the query 
* 			 failed to execute.
*      		
* \param   - char *sqlQuery
*
* \return  - string retry_response
*
******************************************************************************/
std::string DBInterface::RetryMechanism (char* sqlQuery)
{
	UINT8 reTryCounter 	= 0;
	int queryStatus 	= 1;
	static std::string retry_response;
	
	retry_response.clear();
	
	retry_response = m_db.Exec(sqlQuery, &queryStatus, RETRY_COUNT_FOR_ONE_FIFTY_MILLISEC_DELAY);
	
	if (queryStatus != SQLITE_OK)
	{
		retry_response = "RESPONSE_FAIL";
		LOGERR("DBInterface::RetryMechanism: Executing query failed with ErrCode : %d",queryStatus, 0, 0);
	}
	
	return retry_response;
}

/**************************************************************************//**
* \brief  - Posts the encoded message to the destination message queue of
* 			a given task  
*
* \param  - reference to msgBuffer
* \param  - const reference to msgQID
* \param  - _Vx_ticks_t waitType	
*
* \return  - STATUS - OK or ERROR
*
******************************************************************************/
STATUS DBInterface::SendToMsgQ (Message& msgBuffer, const MSG_Q_ID &msgQID, _Vx_ticks_t waitType)
{
	STATUS status = OK;
	if(msgQSend(msgQID, reinterpret_cast<char*>(&msgBuffer), sizeof(msgBuffer), waitType, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("\nUI_DB_T : ----------- Error on msgQSend",0,0,0);

		status = ERROR;
	}
	return status;
}

/**************************************************************************//**
* \brief   - calculate the time difference
*      		
* \param   - struct tm FromTime
*            struct tm ToTime
*            struct tm *diff
*
*
******************************************************************************/
void DBInterface::getTimedifferenct(struct tm FromTime, struct tm ToTime, struct tm *diff)
{
    if (ToTime.tm_sec > FromTime.tm_sec)
    {
	   --FromTime.tm_min;
	   FromTime.tm_sec += 60;
    }
		
	diff->tm_sec = FromTime.tm_sec- ToTime.tm_sec;
	if (ToTime.tm_min > FromTime.tm_min)
	{
	  --FromTime.tm_hour;
	  FromTime.tm_min += 60;
	}
		
	diff->tm_min = FromTime.tm_min - ToTime.tm_min;
	diff->tm_hour = FromTime.tm_hour - ToTime.tm_hour;

}


/**************************************************************************//**
 * \brief   -split (string s, string delimiter) 
*
* \param   - string s  the string needs to parse
*          - string delimiter  the delimiter
*
 * \return  - the vector with parsed data
*
******************************************************************************/
vector<string> DBInterface::split (string s, string delimiter) 
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}



/**************************************************************************//**
* \brief   -writes the default recipe zero when the recipe zero text file is not in the EMMC
*
* \param   - none
*     
*
* \return  - none
*
******************************************************************************/
void DBInterface::SetDefaultRecipeZero()
{
	std::string DefaultWeldRecipeZero= "";
	std::string m_DefaultSusRejLimits="0,0,0,0,'0:0:0:10:30000:0:0:0:10:30000','0:0:0:1:120000:0:0:0:1:120000','0:0:0:1:4000:0:0:0:1:4000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:10:25000:0:0:0:10:25000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:50:2500:0:0:0:50:2500',";
	std::string m_DefaultSusRejLimits20KHz="'0:0:0:1:500:0:0:0:1:500'";
	std::string m_DefaultSusRejLimits30KHz="'0:0:0:1:750:0:0:0:1:750'";
	std::string m_DefaultSusRejLimits40KHz="'0:0:0:1:1000:0:0:0:1:1000'";
	std::string DefSusRejLimitsRecipeZero="";
	std::string m_DefaultBransononly="0,0,100,300,450,450,500,500,500,1000,0,0,0,500,800,1000,200,5,0,0,20000,20000,20000,1000,100,5,22,5,100,450,500,500,500,1000,80,";
	std::string m_Default20KHz="19950";
	std::string m_Default30KHz="30000";
	std::string m_Default40KHz="39900";
	std::string DefaultBransononlyRecipeZero="";
	std::string DefaultSetupLimitsRecipeZero="";
	std::string m_defaultStack20KHz="0,19950,0,0,0";
	std::string m_defaultStack30KHz="0,30000,0,0,0";
	std::string m_defaultStack40KHz="0,39900,0,0,0";
	std::string DefaultStackRecipeZero="";
	string recipeData;
	fstream fp;
	
	//if system is ASX
	if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
	{
		if(CP->powerupRecipe.Frequency == KHz_30 || CP->powerupRecipe.Frequency == KHz_40 )
		{
			DefaultSetupLimitsRecipeZero="0,0,0,'0:10:30000','0:10:100','0:22:2000','0:22:2000','0:22:2000','0:10:30000','0:1:100','0:1:500','0:0:100','0:0:100'";
			/*Actuator Setup Advanced Feature Parameter Default value changed to support imperial unit and to get correct value on UI when create first recipe after RecipeZero delete for system Freq. 30 or 40 of type ASX */
			DefaultWeldRecipeZero= "0,'NewRecipe',3,1,10,10,22,3000,1,1,10,10,10,10,10,10,10,10,10,10,100,100,100,100,100,100,100,100,100,100,50,100,0,50,0,13415,5000,5000000,500000,5000000,5000000,500000,5000000,1,1,10,10,10,10,10,10,10,10,10,10,100,100,100,100,100,100,100,100,100,100,50,50,50,50,50,50,50,50,50,50,80,0,100,100,100,0,0,1,100,3000,0,20,3,0,6000,0,0,0,1,2000,2000,10,0,400,0,500,0,38410,0,500,0,25000,0,120000,0,30000,0,0,1,0,'NewCompany',0,1,3,100,0,100,0,100";
		}
		else if(CP->powerupRecipe.Frequency == KHz_20)
		{
			DefaultSetupLimitsRecipeZero="0,0,0,'0:10:30000','0:10:100','0:44:2000','0:44:2000','0:44:2000','0:10:30000','0:1:100','0:1:500','0:0:100','0:0:100'";
			/*Actuator Setup Advanced Feature Parameter Default value changed to support imperial unit and to get correct value on UI when create first recipe after RecipeZero delete for system Freq. 20 of type ASX */
			DefaultWeldRecipeZero= "0,'NewRecipe',3,1,10,10,44,3000,1,1,10,10,10,10,10,10,10,10,10,10,100,100,100,100,100,100,100,100,100,100,50,100,0,50,0,13415,5000,5000000,500000,5000000,5000000,500000,5000000,1,1,10,10,10,10,10,10,10,10,10,10,100,100,100,100,100,100,100,100,100,100,50,50,50,50,50,50,50,50,50,50,80,0,100,100,100,0,0,1,100,3000,0,20,3,0,6000,0,0,0,1,2000,2000,10,0,400,0,500,0,38410,0,500,0,25000,0,120000,0,30000,0,0,1,0,'NewCompany',0,1,3,100,0,100,0,100";
		}
	}
	else
	{
		DefaultSetupLimitsRecipeZero="0,0,0,'0:10:30000','0:10:100','0:25:2500','0:5:50','0:25:2500','0:10:30000','0:1:100','0:1:500','0:0:100','0:0:100'";
		/*Actuator Setup Advanced Feature Parameter Default value changed to support imperial unit and to get correct value on UI when create first recipe after RecipeZero delete*/	
		DefaultWeldRecipeZero= "0,'NewRecipe',3,1,10,10,25,3000,1,1,10,10,10,10,10,10,10,10,10,10,100,100,100,100,100,100,100,100,100,100,50,100,0,50,0,13415,5000,5000000,500000,5000000,5000000,500000,5000000,1,1,10,10,10,10,10,10,10,10,10,10,100,100,100,100,100,100,100,100,100,100,50,50,50,50,50,50,50,50,50,50,80,0,100,100,100,0,0,1,100,3000,0,20,3,0,6000,0,0,0,1,2000,2000,10,0,400,0,500,0,38410,0,500,0,25000,0,120000,0,30000,0,0,1,0,'NewCompany',0,1,3,100,0,100,0,100";
	}

	switch(CP->powerupRecipe.Frequency)
	{
	    case FREQ_20KHZ:
	    	//if system is ASX
	    	if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
	    	{
	    		m_DefaultSusRejLimits="0,0,0,0,'0:0:0:10:30000:0:0:0:10:30000','0:0:0:1:120000:0:0:0:1:120000','0:0:0:1:4000:0:0:0:1:4000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:10:25000:0:0:0:10:25000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:44:2000:0:0:0:44:2000',";	
	    	}
	    	DefSusRejLimitsRecipeZero.append(m_DefaultSusRejLimits);		
	    	DefSusRejLimitsRecipeZero.append(m_DefaultSusRejLimits20KHz);
	    	DefaultBransononlyRecipeZero.append(m_DefaultBransononly);
	    	DefaultBransononlyRecipeZero.append(m_Default20KHz);
			DefaultStackRecipeZero=m_defaultStack20KHz;
		  break;
	    case FREQ_30KHZ:
	    	//if system is ASX
	    	if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
	    	{
	    		m_DefaultSusRejLimits="0,0,0,0,'0:0:0:10:30000:0:0:0:10:30000','0:0:0:1:45000:0:0:0:1:45000','0:0:0:1:1500:0:0:0:1:1500','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:10:25000:0:0:0:10:25000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:22:2000:0:0:0:22:2000',";
	    	}
	    	else
	    	{
	    		m_DefaultSusRejLimits="0,0,0,0,'0:0:0:10:30000:0:0:0:10:30000','0:0:0:1:45000:0:0:0:1:45000','0:0:0:1:1500:0:0:0:1:1500','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:10:25000:0:0:0:10:25000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:50:2500:0:0:0:50:2500',";	
	    	}
	    	DefSusRejLimitsRecipeZero.append(m_DefaultSusRejLimits);		
	    	DefSusRejLimitsRecipeZero.append(m_DefaultSusRejLimits30KHz);	
	      	DefaultBransononlyRecipeZero.append(m_DefaultBransononly);
	    	DefaultBransononlyRecipeZero.append(m_Default30KHz);
			DefaultStackRecipeZero=m_defaultStack30KHz;
		   break;		
		case FREQ_40KHZ:
	    	//if system is ASX
	    	if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
	    	{
	    		m_DefaultSusRejLimits="0,0,0,0,'0:0:0:10:30000:0:0:0:10:30000','0:0:0:1:24000:0:0:0:1:24000','0:0:0:1:800:0:0:0:1:800','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:10:25000:0:0:0:10:25000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:22:2000:0:0:0:22:2000',";	
	    	}
	    	else
	    	{
	    		m_DefaultSusRejLimits="0,0,0,0,'0:0:0:10:30000:0:0:0:10:30000','0:0:0:1:24000:0:0:0:1:24000','0:0:0:1:800:0:0:0:1:800','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:10:25000:0:0:0:10:25000','0:0:0:3000:125000:0:0:0:3000:125000','0:0:0:50:2500:0:0:0:50:2500',";
	    	}
			DefSusRejLimitsRecipeZero.append(m_DefaultSusRejLimits);		
			DefSusRejLimitsRecipeZero.append(m_DefaultSusRejLimits40KHz);		
		  	DefaultBransononlyRecipeZero.append(m_DefaultBransononly);
			DefaultBransononlyRecipeZero.append(m_Default40KHz);
			DefaultStackRecipeZero=m_defaultStack40KHz;
		  break;
		default:
			LOGERR("DBInterface: Undefined system",0, 0, 0);
			break;
	  }
	
	m_DefaultWeldRecipeZero=DefaultWeldRecipeZero;
	m_DefaultSuspectRejectLimitsZero=DefSusRejLimitsRecipeZero;
	m_DefaultBransonOnlyZero=DefaultBransononlyRecipeZero;
	m_DefaultSetupLimitsZero=DefaultSetupLimitsRecipeZero;
	m_DefaultStackRecipeZero=DefaultStackRecipeZero;
	
}

/**************************************************************************//**
 * \brief   -writes the default recipe zero when the recipe zero text file is not in the EMMC
*
* \param   - none
*     
*
 * \return  - none
*
******************************************************************************/
void DBInterface::WriteDefaultRecipeZero()
{
	string recipeData;
	fstream fp;
	unsigned short recipeZeroCrc ;bool insertResult;
	SetDefaultRecipeZero();
	
	recipeData.append(m_DefaultWeldRecipeZero);
	recipeData.append("\n");
	recipeData.append( m_DefaultSuspectRejectLimitsZero);
	recipeData.append("\n");
	recipeData.append(m_DefaultBransonOnlyZero);
	recipeData.append("\n");
	recipeData.append(m_DefaultSetupLimitsZero);
	recipeData.append("\n");
	recipeData.append(m_DefaultStackRecipeZero);
	recipeData.append("\n");
	
	fp.open(RECIPE_ZERO_PATH, ios::out);

	if(fp.is_open())
	{
		fp << recipeData;
		fp.close();
		m_WeldRecipeZero 			= m_DefaultWeldRecipeZero;
		m_SuspectRejectLimitsZero 	= m_DefaultSuspectRejectLimitsZero;
		m_BransonOnlyZero			= m_DefaultBransonOnlyZero;
		m_SetupLimitsZero			= m_DefaultSetupLimitsZero;
		m_StackRecipeZero			= m_DefaultStackRecipeZero;
		
		recipeZeroCrc= m_RecipeZerodb.CalCrcRecipeZeroFile(0,RECIPE_ZERO_PATH);
		insertResult=m_RecipeZerodb.InsertCrcInRecipeZeroFile(recipeZeroCrc);
		if(!insertResult)
		{
			LOGERR("UI_DB_T : InsertCrcInRecipeZeroFile failed in WriteDefaultRecipeZero() ",0,0,0);  
		}
	}
	else
	{
		LOGERR("Error on writing into recipe zero file. File open failed",0,0,0);
	}
}


/**************************************************************************//**
* \brief   - Returns the recipe zero context to web services.
*
* \param   - std::string &ActiveWeldRecipe, std::string &SRActiveRecipe ,
* 				std::string &BransonOnly, std::string &SetupLimits
*
* \return  - None
*
******************************************************************************/
void DBInterface::GetRecipeZeroStrings(std::string &ActiveWeldRecipe, std::string &SRActiveRecipe, std::string &BransonOnly, std::string &SetupLimits)
{

	if((!m_WeldRecipeZero.empty()) && (!m_SuspectRejectLimitsZero.empty()) &&
			(!m_BransonOnlyZero.empty()) && (!m_SetupLimitsZero.empty()))
	{
		ActiveWeldRecipe = m_WeldRecipeZero;
		SRActiveRecipe = m_SuspectRejectLimitsZero;
		BransonOnly = m_BransonOnlyZero;
		SetupLimits = m_SetupLimitsZero;
	}
}


/**************************************************************************//**
 * \brief   - Sets the recipe zero context to web services.
 *
 * \param   - std::string ActiveWeldRecipe, std::string SRActiveRecipe,
 * 			std::string BransonOnly, std::string SetupLimits
 *
 * \return  - None
 *
 ******************************************************************************/
void DBInterface::SetRecipeZero(std::string ActiveWeldRecipe, std::string SRActiveRecipe, std::string BransonOnly, std::string SetupLimits)
{
	if((!ActiveWeldRecipe.empty()) && (!SRActiveRecipe.empty()) &&
			(!BransonOnly.empty()) && (!SetupLimits.empty()))
	{
		m_WeldRecipeZero = ActiveWeldRecipe;
		m_SuspectRejectLimitsZero = SRActiveRecipe;
		m_BransonOnlyZero = BransonOnly;
		m_SetupLimitsZero = SetupLimits ;
	}
}

/**************************************************************************//**
 * \brief   - To delete the recipe based on recipe number from HMI 
 *
 * \param   - Recipe number as string strData  . 
 *
 * \return  - Error code string
 *
 ******************************************************************************/
std::string DBInterface::DeleteRecipeHMI(std::string strData)
{
	INT32 recipeNum=0;
	char SelectQuery[MSG_SIZE] = {0x00};
	string strDBData, details;
	INT32 dwQueryStatus = 0;
    string maxrecipe;
    INT32 maxrecipeNum=0;
    
    recipeNum = atoi(strData.c_str());
    
    maxrecipe=GetRecipeNumberFromDbForAddRecipe();
    maxrecipeNum=atoi(maxrecipe.c_str());
    
    if((recipeNum < 0) || (recipeNum > maxrecipeNum) || (recipeNum > RECIPE_ID_MAX_RANGE))
    {
    	return CP->ConvertIntToString(DELETE_RECIPE_INVALID_NUMBER);
    }

	if(recipeNum  == CP->m_RecipeSC.GetRecipeNumber())
    {		
		return CP->ConvertIntToString(DELETE_RECIPE_ACTIVE_RECIPE);
    }
	/* Read recipe number from DB to check received recipe is available or not */
	sprintf(SelectQuery,"select RecipeNumber from WeldRecipeTable where RecipeNumber = %d;",recipeNum);
	strDBData = m_db.Exec(SelectQuery, &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		return CP->ConvertIntToString(DELETE_RECIPE_NOT_PRESENT);
	}

	if(!strDBData.empty() && (recipeNum == atoi(strDBData.c_str())))
	{
		sprintf(SelectQuery,"delete from WeldRecipeTable where RecipeNumber = %d;",recipeNum);
		strDBData = m_db.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
        {
		    return CP->ConvertIntToString(DELETE_RECIPE_FAILED);
        }

		sprintf(SelectQuery,"delete from SuspectRejectLimits where RecipeNumber = %d;",recipeNum);
		strDBData = m_db.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
        {
		    return CP->ConvertIntToString(DELETE_RECIPE_FAILED);
        }

		sprintf(SelectQuery,"delete from BransonOnlyParameters where RecipeNumber = %d;",recipeNum);
		strDBData = m_db.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
        {
		    return CP->ConvertIntToString(DELETE_RECIPE_FAILED);
        }

		sprintf(SelectQuery,"delete from SetupLimits where RecipeNumber = %d;",recipeNum);
		strDBData = m_db.Exec(SelectQuery,&dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
        {
		     return CP->ConvertIntToString(DELETE_RECIPE_FAILED);
        }
		
 		sprintf(SelectQuery,"delete from ProductionSetupTable where RecipeNumber = %d;",recipeNum);
		strDBData = m_db.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
        {
		     return CP->ConvertIntToString(DELETE_RECIPE_FAILED);
        }
                
 		sprintf(SelectQuery,"delete from WeldRecipe where RecipeNumber = %d;",recipeNum);
		strDBData = m_db.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
        {
		     return CP->ConvertIntToString(DELETE_RECIPE_FAILED);
        }
        
 		sprintf(SelectQuery,"delete from StackRecipeTable where RecipeNumber = %d;",recipeNum);
		strDBData = m_db.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
        {
		     return CP->ConvertIntToString(DELETE_RECIPE_FAILED);
        }
	}
	else
	{
		return CP->ConvertIntToString(DELETE_RECIPE_NOT_PRESENT);
	}

	details = "Identifier:Recipe " + CP->ConvertIntToString(recipeNum) + ":From:N/A:To:N/A:";
	DataBaseManager::EnterEventLog  (RECIPE_DELETED, details);
	return CP->ConvertIntToString(DELETE_RECIPE_SUCCESS);
}


/**************************************************************************//**
* 
* \brief   - Copy active recipe data from database to Recipe Zero file .
*
* \param   - None.
*			
* \return  - None.
*
******************************************************************************/
void DBInterface::UpdateRecipeZeroWithActiveRecipe()
{
	std::string retVal;std::string strData;std::string strQuery;
    std::string strActiveWeldRecipe, strProperStringFormatToStoreInDB;
	vector<string> vRecipeParams;
	INT8 recipeNo=0;
	
    strQuery="select RecipeNumber from WeldRecipeTable where IsActive=1";
    strData= m_db.Exec((char*)strQuery.c_str());
    recipeNo= atoi(strData.c_str());
    if(recipeNo)
    {
    	//update recipe 0 with the newly activated recipe
    	//weld recipe 0
    	strQuery = "select * from WeldRecipeTable where RecipeNumber=" + strData+";"; 
    	strActiveWeldRecipe = m_db.Exec((char*)strQuery.c_str());
    	/* Proper string format to update in recipe zero and store in DB again  for active recipe parameters */
    	CP->StringToTokens(strActiveWeldRecipe.c_str(),vRecipeParams,',');
    	strProperStringFormatToStoreInDB.append("'");
    	strProperStringFormatToStoreInDB.append(vRecipeParams[WR_DB_WELD_RECIPE_NAME].c_str());
    	strProperStringFormatToStoreInDB.append("'");
    	vRecipeParams[WR_DB_WELD_RECIPE_NAME] = strProperStringFormatToStoreInDB;
	
    	strProperStringFormatToStoreInDB.clear();
    	strProperStringFormatToStoreInDB.append("''");
    	vRecipeParams[WR_DB_COMPANY_NAME] = strProperStringFormatToStoreInDB;
    	CP->TokensToString(strActiveWeldRecipe,vRecipeParams,',');
    
    	m_WeldRecipeZero = strActiveWeldRecipe;
    
    	//stack recipe 0
    	strQuery = "select * from StackRecipeTable where RecipeNumber=" + strData+";"; 
    	m_StackRecipeZero = m_db.Exec((char*)strQuery.c_str());
    
    	//suspect reject limits 0
    	m_SuspectRejectLimitsZero = "0,0,";
    	strQuery = "select GlobalSuspect from SuspectRejectLimits where RecipeNumber=" + strData+";"; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append(",");
    	strQuery = "select GlobalReject from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append(",'");
    	strQuery = "select Time from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("','");
    	strQuery = "select Energy from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("','");
    	strQuery = "select PeakPower from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("','");
    	strQuery = "select AbsoluteDistance from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("','");
    	strQuery = "select CollapseDistance from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("','");
    	strQuery = "select TriggerDistance from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("','");
    	strQuery = "select EndWeldForce from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("','");
    	strQuery = "select Frequency from SuspectRejectLimits where RecipeNumber=" + strData; 
    	m_SuspectRejectLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SuspectRejectLimitsZero.append("'");
    
    	//setup limits 0
    	m_SetupLimitsZero = "0,0,";
    	strQuery = "select GlobalSetup from SetupLimits where RecipeNumber=" + strData; 
    	m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SetupLimitsZero.append(",'");
    	strQuery = "select WeldMode from SetupLimits where RecipeNumber=" + strData; 
    	m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
    	m_SetupLimitsZero.append("','");
    	strQuery = "select WeldAmplitude from SetupLimits where RecipeNumber=" + strData; 
    	m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select WeldForce from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select TriggerForce from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select HoldForce from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select HoldTime from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select ScrubAmplitude from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select ScrubTime from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select Reactivity from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("','");
		strQuery = "select ForceLevel from SetupLimits where RecipeNumber=" + strData; 
		m_SetupLimitsZero.append(m_db.Exec((char*)strQuery.c_str()));
		m_SetupLimitsZero.append("'");
    }
    else
    {
    	/*in case active recipe data not found copy default recipe zero data*/
    	SetDefaultRecipeZero();
    	m_WeldRecipeZero 			= m_DefaultWeldRecipeZero;
    	m_SuspectRejectLimitsZero 	= m_DefaultSuspectRejectLimitsZero;
    	m_BransonOnlyZero			= m_DefaultBransonOnlyZero;
    	m_SetupLimitsZero			= m_DefaultSetupLimitsZero;
    	m_StackRecipeZero			= m_DefaultStackRecipeZero;	
    }
    WriteRecipeZeroToFile();

}
/**************************************************************************//**
 * \brief   - Prepare validated RecipeZero.txt,RecipeZeroBkup.txt files
 *			  in following cases after power up:
			  1:main recipe zero is existed and backup recipe zero file is not existed.
			  2:main recipe zero is not existed and backup recipe zero file is  existed.
			  3:main recipe zero is existed and backup recipe zero file is existed.
			  4:both files are not existed.
 * \param   - None . 
 *
 * \return  - None
 *
 ******************************************************************************/

void DBInterface::PrepareValidatedRecipezeroFiles(void)
{
	unsigned short recipeZeroCrc ;bool insertResult;
	bool isMainRecipeValidate;bool isBkupRecipeValidate;

	if((CP->IsFileExist(RECIPE_ZERO_PATH)) && (!(CP->IsFileExist(RECIPE_ZERO_BKUP_PATH))))
	{
		if(m_RecipeZerodb.ValidateRecipeZeroFile(RECIPE_ZERO_PATH))
		{
			m_RecipeZerodb.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
		}
		else
		{
			UpdateRecipeZeroWithActiveRecipe();
		}

	}else if((CP->IsFileExist(RECIPE_ZERO_PATH)) && ((CP->IsFileExist(RECIPE_ZERO_BKUP_PATH))))
	{
		isMainRecipeValidate=m_RecipeZerodb.ValidateRecipeZeroFile(RECIPE_ZERO_PATH);
		isBkupRecipeValidate=m_RecipeZerodb.ValidateRecipeZeroFile(RECIPE_ZERO_BKUP_PATH);
		
		if(isMainRecipeValidate && isBkupRecipeValidate)
		{
		}else if(isMainRecipeValidate && !(isBkupRecipeValidate))
		{
			m_RecipeZerodb.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );	
		}else if(!(isMainRecipeValidate) && isBkupRecipeValidate)
		{
			m_RecipeZerodb.CopyFile(RECIPE_ZERO_BKUP_PATH ,RECIPE_ZERO_PATH);
		}else
		{
			UpdateRecipeZeroWithActiveRecipe();		
		}
		
	}else if(!(CP->IsFileExist(RECIPE_ZERO_PATH)) && ((CP->IsFileExist(RECIPE_ZERO_BKUP_PATH))))
	{
		if(m_RecipeZerodb.ValidateRecipeZeroFile(RECIPE_ZERO_BKUP_PATH))
		{
			m_RecipeZerodb.CopyFile( RECIPE_ZERO_BKUP_PATH,RECIPE_ZERO_PATH);
		}else
		{
			UpdateRecipeZeroWithActiveRecipe();	
		}
		
	}else
	{
		 WriteDefaultRecipeZero();
		 m_RecipeZerodb.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );		
	}

}











