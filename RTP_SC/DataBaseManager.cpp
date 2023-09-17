/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Data base manager functions to store the Weld/Seek results to Database
 
**********************************************************************************************************/

#include  "DataBaseManager.h"
#include  "RunTimeFeature.h"
#include  "ReportGeneration.h"
#include  <sys/stat.h>
#include  "commons.h"
#include  "ControlTask.h"
#include  "AlarmManager.h"
#include  "DataBaseConfiguration.h"
#include  "SysConfigEeprom.hpp"
#include  "versions.h"
#include  "Utils.h"
#include "UIDBInterface.h"
#include "version_common.h"
#include "WebServiceTask.h"
#include <VendorSpecificInfo.h>

extern "C"
{
	#include "customSystemCall.h"	
}

#include <sstream>
#include <vector>
#include <bitset>
using namespace std;

// static initialization
string DataBaseManager::m_EventUserName = "N/A";
string DataBaseManager::recipeRevisionNum = "N/A";

/**************************************************************************//**
* \brief   - Initialize and assign the message queue of DB Q
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
DataBaseManager::DataBaseManager() : DB_MSG_Q_ID(0), UI_MSG_Q_ID(0)
{
	CP = CommonProperty::getInstance();
		
	/* Load the message Q ids from CommonProperty class MsgQMap object.	*/	
	DB_MSG_Q_ID = CP->GetMsgQId(cTaskName[DB_T]);
	UI_MSG_Q_ID = CP->GetMsgQId(cTaskName[UI_T]);
	ALARM_MSG_Q_ID = CP->GetMsgQId(cTaskName[ALARM_T]);
	SDO_MSG_Q_ID = CP->GetMsgQId(cTaskName[SDO_T]);
	CTRL_MSG_Q_ID = CP->GetMsgQId(cTaskName[CTRL_T]);
	WEBSERVICE_MSG_Q_ID	= CP->GetMsgQId(cTaskName[WEBSERVICE_T]);

	m_EventNameMap[ESTOP_PRESS] 				= "EMERGENCY STOP PRESSED";
	m_EventNameMap[ESTOP_RELEASE] 				= "EMERGENCY STOP RELEASED";
	m_EventNameMap[USER_LOGIN] 					= "USER LOGIN";
	m_EventNameMap[USER_LOGOUT] 				= "USER LOGOUT";

	m_EventNameMap[USER_LOGIN_WEB]				= "REMOTE USER LOGIN";
	m_EventNameMap[USER_LOGOUT_WEB]				= "REMOTE USER LOGOUT";
	m_EventNameMap[WEB_SERVICES]				= "WEB SERVICES";

	m_EventNameMap[USER_CREATED] 				= "USER CREATED";
	m_EventNameMap[USER_MODIFIED] 				= "USER MODIFIED";
	
	m_EventNameMap[RECIPE_CREATED] 				= "RECIPE CREATED";
	m_EventNameMap[RECIPE_SAVED] 				= "RECIPE SAVED";
	m_EventNameMap[RECIPE_COPIED] 				= "RECIPE COPIED";
	m_EventNameMap[RECIPE_MODIFIED] 			= "RECIPE PARAMETER MODIFIED";
	m_EventNameMap[RECIPE_VALIDATION] 			= "RECIPE VALIDATION CHANGE";
	m_EventNameMap[RECIPE_DELETED] 				= "RECIPE DELETED";
	
	m_EventNameMap[HORN_SCAN_COMPLETED] 		= "HORN SCAN";
	m_EventNameMap[SEEK_PERFORM] 				= "SEEK";
	m_EventNameMap[CYCLE_COUNT_RESET] 			= "RECIPE CYCLE COUNT RESET";
	m_EventNameMap[SYSTEM_HW_CHANGED] 			= "SYSTEM HARDWARE CHANGED";
	m_EventNameMap[SYSTEM_FACTORY_RESET] 		= "FACTORY RESET";
	m_EventNameMap[SYSTEM_TIME_CHANGED]			= "SYSTEM DATE/TIME CHANGED";
	m_EventNameMap[SYSTEM_CONFIGURATION] 		= "SYSTEM CONFIGURATION MODIFIED";
	m_EventNameMap[PART_CONTACT_PERFORM] 		= "FIND PART CONTACT PERFORMED";
	m_EventNameMap[CALIBRATION_COMPLETED] 		= "CALIBRATION COMPLETED";
	m_EventNameMap[SYSTEM_DB_DELETED] 			= "DATABASE DELETED";	
	m_EventNameMap[SYSTEM_LOG_DELETED] 			= "LOG DELETED";
	m_EventNameMap[SYSTEM_BATT_LOW] 			= "SYSTEM BATTERY LOW";
	m_EventNameMap[SYSTEM_SW_VER_CHANGE]		= "SOFTWARE VERSION CHANGE";
	
	m_EventLogNameMap[WELD_RESULTS_LOG_ID]		= "Weld Results";
	m_EventLogNameMap[WELD_SIGNATURE_LOG_ID]	= "Weld Signature";
	m_EventLogNameMap[ALARM_LOG_LOG_ID]			= "Alarm Log";
	m_EventLogNameMap[EVENT_LOG_LOG_ID]			= "Event Log";
	
	m_RecipeParamMap[SC_RECIPE_NUMBER] = "Recipe Number";
	m_RecipeParamMap[SC_RECIPE_NAME] = "Recipe Name";
	m_RecipeParamMap[SC_RECIPE_REV_NUMBER] = "Recipe Revision Number";
	m_RecipeParamMap[SC_WELD_MODE] = "Weld Mode";
	m_RecipeParamMap[SC_MODE_VALUE] = "Mode Value";
	m_RecipeParamMap[SC_HOLD_TIME] = "Hold Time";
	m_RecipeParamMap[SC_TRIGGER_FORCE] = "Trigger Force";
	m_RecipeParamMap[SC_TRIGGER_DISTANCE]	= "Trigger Distance";
	m_RecipeParamMap[SC_NUM_AMP_STEPS] = "Amplitude Steps";
	m_RecipeParamMap[SC_AMP_STEP_AT] = "Amplitude Step At";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_0] = "Amplitude Step Value 1";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_1] = "Amplitude Step Value 2";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_2] = "Amplitude Step Value 3";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_3] = "Amplitude Step Value 4";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_4] = "Amplitude Step Value 5";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_5] = "Amplitude Step Value 6";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_6] = "Amplitude Step Value 7";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_7] = "Amplitude Step Value 8";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_8] = "Amplitude Step Value 9";
	m_RecipeParamMap[SC_AMP_STEP_VALUE_9] = "Amplitude Step Value 10";
	m_RecipeParamMap[SC_AMP_STEP_0] = "Amplitude Step 1";
	m_RecipeParamMap[SC_AMP_STEP_1] = "Amplitude Step 2";
	m_RecipeParamMap[SC_AMP_STEP_2] = "Amplitude Step 3";
	m_RecipeParamMap[SC_AMP_STEP_3] = "Amplitude Step 4";
	m_RecipeParamMap[SC_AMP_STEP_4] = "Amplitude Step 5";
	m_RecipeParamMap[SC_AMP_STEP_5] = "Amplitude Step 6";
	m_RecipeParamMap[SC_AMP_STEP_6] = "Amplitude Step 7";
	m_RecipeParamMap[SC_AMP_STEP_7] = "Amplitude Step 8";
	m_RecipeParamMap[SC_AMP_STEP_8] = "Amplitude Step 9";
	m_RecipeParamMap[SC_AMP_STEP_9] = "Amplitude Step 10";
	m_RecipeParamMap[AC_WELD_FORCE] = "Weld Force";
	m_RecipeParamMap[AC_FORCE_RAMP_TIME] = "Force Weld TIme";
	m_RecipeParamMap[AC_TOTAL_COLLAPSE_TARGET] = "Total Collapse Target";
	m_RecipeParamMap[AC_HOLD_FORCE] = "Hold Force";
	m_RecipeParamMap[AC_HOLD_FORCE_RAMP_TIME] = "Hold Force Ramp Time";
	m_RecipeParamMap[AC_EXPECTED_PC_POSITION] = "Expected PC Position";
	m_RecipeParamMap[AC_READY_POSITION] = "Ready Position Offset";
	m_RecipeParamMap[AC_DOWN_ACCELERATION] = "Down Acceleration";
	m_RecipeParamMap[AC_DOWN_MAX_VELOCITY] = "Max Velocity";
	m_RecipeParamMap[AC_DOWN_DECELERATION] = "Down Deceleration";
	m_RecipeParamMap[AC_RETURN_ACCELERATION] = "Return Acceleration";
	m_RecipeParamMap[AC_RETURN_MAX_VELOCITY] = "Return Max Velocity";
	m_RecipeParamMap[AC_RETURN_DECELERATION] = "Return Deceleration";
	m_RecipeParamMap[SC_NUM_FORCE_STEPS] = "Force Steps";
	m_RecipeParamMap[SC_FORCE_STEP_AT] = "Force Step At";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_0] = "Force Step Value 1";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_1] = "Force Step Value 2";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_2] = "Force Step Value 3";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_3] = "Force Step Value 4";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_4] = "Force Step Value 5";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_5] = "Force Step Value 6";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_6] = "Force Step Value 7";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_7] = "Force Step Value 8";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_8] = "Force Step Value 9";
	m_RecipeParamMap[SC_FORCE_STEP_AT_VALUE_9] = "Force Step Value 10";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_0] = "Force Step Ramp Time 1";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_1] = "Force Step Ramp Time 2";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_2] = "Force Step Ramp Time 3";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_3] = "Force Step Ramp Time 4";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_4] = "Force Step Ramp Time 5";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_5] = "Force Step Ramp Time 6";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_6] = "Force Step Ramp Time 7";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_7] = "Force Step Ramp Time 8";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_8] = "Force Step Ramp Time 9";
	m_RecipeParamMap[AC_FORCE_STEP_RAMP_TIME_9] = "Force Step Ramp Time 10";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_0] = "Force Step 1";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_1] = "Force Step 2";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_2] = "Force Step 3";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_3] = "Force Step 4";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_4] = "Force Step 5";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_5] = "Force Step 6";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_6] = "Force Step 7";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_7] = "Force Step 8";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_8] = "Force Step 9";
	m_RecipeParamMap[AC_FORCE_STEP_FORCE_9] = "Force Step 10";
	m_RecipeParamMap[PC_WELD_RAMP_TIME] = "PC Weld Ramp Time";
	m_RecipeParamMap[SC_AFTER_BURST_ENABLED] = "After Burst Enable";
	m_RecipeParamMap[SC_AFTER_BURST_DELAY] = "After Burst Delay";
	m_RecipeParamMap[SC_AFTER_BURST_TIME] = "After Burst Time";
	m_RecipeParamMap[SC_AFTER_BURST_AMP] = "After Burst Amplitude";
	m_RecipeParamMap[SC_PRETRIGGER_ENABLED] = "Pretrigger Enable";
	m_RecipeParamMap[SC_PRETRIGGER_AUTO_ENABLED] = "Pretrigger Auto Enable";
	m_RecipeParamMap[SC_PRETRIGGER_DISTANCE_ENABLED] = "Pretrigger Distance Enable";
	m_RecipeParamMap[SC_PRETRIGGER_AMP] = "Pretrigger Amplitude";
	m_RecipeParamMap[SC_PRETRIGGER_DISTANCE] = "Pretrigger Distance";
	m_RecipeParamMap[SC_ENERGY_BRAKE_ENABLED] = "Energy Brake Enable";
	m_RecipeParamMap[SC_ENERGY_BRAKE_TIME] = "Energy Brake Time";
	m_RecipeParamMap[SC_ENERGY_BRAKE_AMP] = "Energy Brake Amplitude";
	m_RecipeParamMap[SC_COOLING_VALVE] = "Cooling valve";
	m_RecipeParamMap[SC_MAX_WELD_TIMEOUT] = "Max Weld Timeout";
	m_RecipeParamMap[SC_TIMED_SEEK_ENABLED] = "Timed Seek Enable";
	m_RecipeParamMap[SC_PRE_WELD_SEEK_ENABLED] = "Pre Weld Seek Enable";
	m_RecipeParamMap[SC_POST_WELD_SEEK_ENABLED] = "Post Weld Seek Enable";
	m_RecipeParamMap[SC_TIMED_SEEK_PERIOD] = "Timed Seek Period";
	m_RecipeParamMap[AC_PC_WINDOW_MINUS] = "Window Minus ";
	m_RecipeParamMap[AC_PC_WINDOW_PLUS] = "Window Plus ";
	m_RecipeParamMap[AC_PC_WINDOW_OFFSET] = "Window Offset ";
	m_RecipeParamMap[SC_GLOBAL_CONTROL] = "Global Control Enable";
	m_RecipeParamMap[SC_PEAK_POWER_CUTOFF] = "Peak Power Cut-off";
	m_RecipeParamMap[SC_PEAK_POWER_CUTOFF_ENABLE] = "Peak Power Cut-off Enable";
	m_RecipeParamMap[SC_FREQUENCY_LOW_CUTOFF] = "Frequency Low Cut-off";
	m_RecipeParamMap[SC_FREQUENCY_LOW_CUTOFF_ENABLE] = "Frequency Low Cut-off Enable";
	m_RecipeParamMap[SC_ABSOLUTE_CUTOFF] = "Absolute Distance Cut-off";
	m_RecipeParamMap[SC_ABSOLUTE_CUTOFF_ENABLE] = "Absolute Distance Cut-off Enable";
	m_RecipeParamMap[SC_FREQUENCY_HIGH_CUTOFF] = "Frequency High Cut-off";
	m_RecipeParamMap[SC_FREQUENCY_HIGH_CUTOFF_ENABLE] = "Frequency High Cut-off Enable";
	m_RecipeParamMap[SC_COLLAPSE_CUTOFF] = "Collapse Cut-off";
	m_RecipeParamMap[SC_COLLAPSE_CUTOFF_ENABLE] = "Collapse Cut-off Enable";
	m_RecipeParamMap[SC_ENERGY_CUTOFF] = "Energy Cut-off";
	m_RecipeParamMap[SC_ENERGY_CUTOFF_ENABLE] = "Energy Cut-off Enable";
	m_RecipeParamMap[SC_TIME_CUTOFF] = "Time Cut-off";
	m_RecipeParamMap[SC_TIME_CUTOFF_ENABLE] = "Time Cut-off Enable";
	m_RecipeParamMap[SC_GROUND_DETECT_CUTOFF_ENABLE] = "Ground Detect Cut-off Enable";
	m_RecipeParamMap[SC_IS_ACTIVE] = "Active";
	m_RecipeParamMap[SC_IS_LOCKED] = "Locked";
	m_RecipeParamMap[SC_IS_VALIDATE] = "Validated";
	m_RecipeParamMap[SC_COMPANY_NAME] = "Company Name";
	m_RecipeParamMap[AC_READY_POS_TOGGLE] = "Ready Position Toggle";
	m_RecipeParamMap[AC_WELD_FORCE_CONTROL] = "Weld Force Control";
	m_RecipeParamMap[SC_REACTIVITY] = "Reactivity";
	m_RecipeParamMap[SC_FORCE_LEVEL_ENABLE] = "Force Level Enable";
	m_RecipeParamMap[SC_FORCE_LEVEL] = "Force Level";
	m_RecipeParamMap[SC_FORCE_LEVEL_TIME] = "Force Level Time";
	m_RecipeParamMap[SC_SCRUB_AMPLITUDE] = "Scrub Amplitude";
	
	m_RecipeParamMap[SC_DIGITAL_TUNE] 		= "Digital Tune";
	m_RecipeParamMap[SC_OFFSET_FLAG] 		= "Offset Flag";
	m_RecipeParamMap[SC_FREQUENCY_OFFSET] 	= "Frequency Offset";
	m_RecipeParamMap[SC_END_OF_WELD] 		= "End Of Weld Store";
	
	m_RecipeModeMap[MODE_TIME] 			= "Time Mode";
	m_RecipeModeMap[MODE_ENERGY] 		= "Energy Mode";
	m_RecipeModeMap[MODE_POWER] 		= "Peak Power Mode";
	m_RecipeModeMap[MODE_GROUND_DETECT] = "Ground Detect Mode";
	m_RecipeModeMap[MODE_ABSOLUTE] 		= "Absolute Distance Mode";
	m_RecipeModeMap[MODE_COLLAPSE] 		= "Collapse Mode";
	m_RecipeModeMap[MODE_DYNAMIC]		= "Dynamic Mode";
	
	m_StepTypeMap[STEP_TIME] 		= "Time";
	m_StepTypeMap[STEP_ENERGY] 		= "Energy";
	m_StepTypeMap[STEP_POWER] 		= "Peak Power";
	m_StepTypeMap[STEP_ABSOLUTE] 	= "Absolute Distance";
	m_StepTypeMap[STEP_COLLAPSE] 	= "Collapse";
	m_StepTypeMap[STEP_EXTERNAL] 	= "External";

	m_RecipeParamMap[SC_SUSPECT_GLOBAL_ENABLE] = "Global Suspect Enable";
	m_RecipeParamMap[SC_REJECT_GLOBAL_ENABLE] = "Global Reject Enable";

	m_RecipeParamMap[SC_SUSPECT_TIME_ENABLE] = "Time Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_TIME_LOW] = "Time Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_TIME_HIGH] = "Time Suspect High";
	m_RecipeParamMap[SC_REJECT_TIME_ENABLE] = "Time Reject Enable";
	m_RecipeParamMap[SC_REJECT_TIME_LOW] = "Time Reject Low";
	m_RecipeParamMap[SC_REJECT_TIME_HIGH] = "Time Reject High";

	m_RecipeParamMap[SC_SUSPECT_ENERGY_ENABLE] = "Energy Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_ENERGY_LOW] = "Energy Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_ENERGY_HIGH] = "Energy Suspect High";
	m_RecipeParamMap[SC_REJECT_ENERGY_ENABLE] = "Energy Reject Enable";
	m_RecipeParamMap[SC_REJECT_ENERGY_LOW] = "Energy Reject Low";
	m_RecipeParamMap[SC_REJECT_ENERGY_HIGH] = "Energy Reject High";

	m_RecipeParamMap[SC_SUSPECT_POWER_ENABLE] = "Power Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_POWER_LOW] = "Power Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_POWER_HIGH] = "Power Suspect High";
	m_RecipeParamMap[SC_REJECT_POWER_ENABLE] = "Power Reject Enable";
	m_RecipeParamMap[SC_REJECT_POWER_LOW] = "Power Reject Low";
	m_RecipeParamMap[SC_REJECT_POWER_HIGH] = "Power Reject High";
		
	m_RecipeParamMap[SC_SUSPECT_ABSOLUTE_ENABLE] = "Absolute Distance Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_ABSOLUTE_LOW] = "Absolute Distance Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_ABSOLUTE_HIGH] = "Absolute Distance Suspect High";
	m_RecipeParamMap[SC_REJECT_ABSOLUTE_ENABLE] = "Absolute Distance Reject Enable";
	m_RecipeParamMap[SC_REJECT_ABSOLUTE_LOW] = "Absolute Distance Reject Low";
	m_RecipeParamMap[SC_REJECT_ABSOLUTE_HIGH] = "Absolute Distance Reject High";

	m_RecipeParamMap[SC_SUSPECT_COLLAPSE_ENABLE] = "Collapse Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_COLLAPSE_LOW] = "Collapse Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_COLLAPSE_HIGH] = "Collapse Suspect High";
	m_RecipeParamMap[SC_REJECT_COLLAPSE_ENABLE] = "Collapse Reject Enable";
	m_RecipeParamMap[SC_REJECT_COLLAPSE_LOW] = "Collapse Reject Low";
	m_RecipeParamMap[SC_REJECT_COLLAPSE_HIGH] = "Collapse Reject High";

	m_RecipeParamMap[SC_SUSPECT_TRIGGER_ENABLE] = "Trigger Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_TRIGGER_LOW] = "Trigger Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_TRIGGER_HIGH] = "Trigger Suspect High";
	m_RecipeParamMap[SC_REJECT_TRIGGER_ENABLE] = "Trigger Reject Enable";
	m_RecipeParamMap[SC_REJECT_TRIGGER_LOW] = "Trigger Reject Low";
	m_RecipeParamMap[SC_REJECT_TRIGGER_HIGH] = "Trigger Reject High";

	m_RecipeParamMap[SC_SUSPECT_END_FORCE_ENABLE] = "End Force Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_END_FORCE_LOW] = "End Force Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_END_FORCE_HIGH] = "End Force Suspect High";
	m_RecipeParamMap[SC_REJECT_END_FORCE_ENABLE] = "End Force Reject Enable";
	m_RecipeParamMap[SC_REJECT_END_FORCE_LOW] = "End Force Reject Low";
	m_RecipeParamMap[SC_REJECT_END_FORCE_HIGH] = "End Force Reject High";
	
	m_RecipeParamMap[SC_SUSPECT_FREQUENCY_ENABLE] = "Frequency Suspect Enable";
	m_RecipeParamMap[SC_SUSPECT_FREQUENCY_LOW] = "Frequency Suspect Low";
	m_RecipeParamMap[SC_SUSPECT_FREQUENCY_HIGH] = "Frequency Suspect High";
	m_RecipeParamMap[SC_REJECT_FREQUENCY_ENABLE] = "Frequency Reject Enable";
	m_RecipeParamMap[SC_REJECT_FREQUENCY_LOW] = "Frequency Reject Low";
	m_RecipeParamMap[SC_REJECT_FREQUENCY_HIGH] = "Frequency Reject High";
	
	m_RecipeParamMap[SC_SETUP_GLOBAL_ENABLE] = "Global Setup Enable";
	m_RecipeParamMap[SC_SETUP_MODE_ENABLE] = "Setup Weld Mode Enable";
	m_RecipeParamMap[SC_SETUP_AMPLITUDE_ENABLE] = "Setup Amplitude Enable";
	m_RecipeParamMap[SC_SETUP_WELD_FORCE_ENABLE] = "Setup Weld Force Enable";
	m_RecipeParamMap[SC_SETUP_TRIGGER_FORCE_ENABLE] = "Setup Trigger Force Enable";
	m_RecipeParamMap[SC_SETUP_HOLD_FORCE_ENABLE] = "Setup Hold Force Enable";
	m_RecipeParamMap[SC_SETUP_HOLD_TIME_ENABLE] = "Setup Hold Time Enable";
	m_RecipeParamMap[SC_SETUP_SCRUB_AMPLITUDE_ENABLE] = "Setup Scrub Amplitude Enable";
	m_RecipeParamMap[SC_SETUP_SCRUB_TIME_ENABLE] = "Setup Scrub Time Enable";
	m_RecipeParamMap[SC_SETUP_REACTIVITY_ENABLE] = "Setup Reactivity Enable";
	m_RecipeParamMap[SC_SETUP_FORCE_LEVEL_ENABLE] = "Setup Force Level Enable";

	m_RecipeParamMap[SC_SETUP_MODE_LOW] = "Setup Weld Mode Low";
	m_RecipeParamMap[SC_SETUP_MODE_HIGH] = "Setup Weld Mode High";
	m_RecipeParamMap[SC_SETUP_AMPLITUDE_LOW] = "Setup Amplitude Low";
	m_RecipeParamMap[SC_SETUP_AMPLITUDE_HIGH] = "Setup Amplitude High";
	m_RecipeParamMap[SC_SETUP_WELD_FORCE_LOW] = "Setup Weld Force Low";
	m_RecipeParamMap[SC_SETUP_WELD_FORCE_HIGH] = "Setup Weld Force High";
	m_RecipeParamMap[SC_SETUP_TRIGGER_FORCE_LOW] = "Setup Trigger Force Low";
	m_RecipeParamMap[SC_SETUP_TRIGGER_FORCE_HIGH] = "Setup Trigger Force High";
	m_RecipeParamMap[SC_SETUP_HOLD_FORCE_LOW] = "Setup Hold Force Low";
	m_RecipeParamMap[SC_SETUP_HOLD_FORCE_HIGH] = "Setup Hold Force High";
	m_RecipeParamMap[SC_SETUP_HOLD_TIME_LOW] = "Setup Hold Time Low";
	m_RecipeParamMap[SC_SETUP_HOLD_TIME_HIGH] = "Setup Hold Time High";
	m_RecipeParamMap[SC_SCRUB_AMPLITUDE_LOW] = "Scrub Amplitude Low";
	m_RecipeParamMap[SC_SCRUB_AMPLITUDE_HIGH] = "Scrub Amplitude High";
	m_RecipeParamMap[SC_SCRUB_TIME_LOW] = "Scrub Time Low";
	m_RecipeParamMap[SC_SCRUB_TIME_HIGH] = "Scrub Time High";
	
	m_RecipeParamMap[SC_SETUP_REACTIVITY_LOW] = "Reactivity Low";
	m_RecipeParamMap[SC_SETUP_REACTIVITY_HIGH] = "Reactivity High";
	m_RecipeParamMap[SC_SETUP_FORCE_LEVEL_LOW] = "Force Level Low";
	m_RecipeParamMap[SC_SETUP_FORCE_LEVEL_HIGH] = "Force Level High";
}

/**************************************************************************//**
* \brief   - Gets the DB task message queue's ID.
*
* \param   - None
*
* \return  - MSG_Q_ID
*
******************************************************************************/
MSG_Q_ID DataBaseManager::GetDBMsgQID() const
{
	return DB_MSG_Q_ID;
}

/**************************************************************************//**
* \brief   - Get the task life time status.
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool DataBaseManager::bIsTaskRunStatus()
{
	return CP->bIsTaskRun;
}

/**************************************************************************//**
* \brief   - Gets RTC date and time.
*
* \param   - string DateTime 
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetDateAndTime(string &DateTime)
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
* \brief   - Establish the DB connection
*
* \param   - None.
*
* \return  - STATUS
*
******************************************************************************/
STATUS DataBaseManager::ConnectDB()
{
	return m_DbConn.EstablishDataBaseConnection();
#ifdef DEBUG
	LOGDBG("DB Connection opened",0,0,0);
#endif
}

/**************************************************************************//**
* \brief   - Close the DB connection and release memory associated with DB
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::CloseDB()
{
	m_DbConn.Close();
#ifdef DEBUG
	LOGDBG("DB Connection closed",0,0,0);
#endif
}

/**************************************************************************//**
* \brief   - Writing into DB( RESULT ID into signature table)
*
* \param   - INT32 recipeNum
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ReadCycleCounter(INT32 recipeNum)
{
	char SelectQuery[MSG_SIZE] = {0x00};
	string Counter;
	INT32 queryStatus = 0;
	
	sprintf(SelectQuery,"SELECT CycleCount FROM ProductionSetupTable WHERE RecipeNumber = %d;",recipeNum);
	Counter = m_DbConn.Exec(SelectQuery,&queryStatus);
	
	if(SQLITE_OK != queryStatus)
	{
		LOGERR("DB_T : GetResultID : ErrCode from DB : %d, recipeNum : %d",queryStatus,recipeNum,0); 
	}
		
#ifdef DEBUG
	LOGDBG("DB_T : Counter : %d",Counter,0,0);
#endif
	
	UpdateCycleCounter(atoi(Counter.c_str()));
}

/**************************************************************************//**
* \brief   - Reads unique id of results (Horn scan)
*
* \param   - None
*
* \return  - UINT32 - Scan ID
*
******************************************************************************/
UINT32 DataBaseManager::GetScanID()
{
	char selectQuery[MSG_SIZE] = {0x00};
	string scanID;
	INT32 queryStatus = 0;

	sprintf(selectQuery,"SELECT ScanID FROM HornResultTable ORDER BY ScanID DESC LIMIT 1;");
	scanID = m_DbConn.Exec(selectQuery,&queryStatus);

	if(SQLITE_OK != queryStatus)
	{
		LOGERR("DB_T : GetResultID : ErrCode from DB : %d",queryStatus,0,0); 
	}

#ifdef DEBUG
	LOGDBG("DB_T : scanID : %d",scanID,0,0);
#endif
	return atoi(scanID.c_str());
}

/**************************************************************************//**
* \brief   - Writing into DB (HORN SCAN SIGNATURE)
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::StoreHornScanSignature()
{
	char insertQuery[MSG_SIZE] = {0x00};
	string bulkInsert,blobData,referenceData;	
	INT32 queryStatus = 0;
	UINT32 	scanID	  = 0;

	if(CP->Signature != NULL)
	{		
		scanID = GetScanID();
		
		if(0 == scanID)
		{
			scanID = 1;		/* Initial Scan */
		}
		else
		{
			scanID++;
		}
		
		LOGDBG("DB_T : Horn scan signature insert query start : ID : %d",scanID,0,0); 
		for(INT32 idx = 0; idx < CP->NoOfSamples; idx++)
		{
			memset(insertQuery,0x00,sizeof(insertQuery));

			sprintf(insertQuery,"%d,%d,%d,%f,", CP->Signature[idx].Frequency, CP->Signature[idx].Amplitude,CP->Signature[idx].Current, CP->Signature[idx].Phase);
			blobData.append((const char *)insertQuery);
		}

		if(!blobData.empty())
		{
			blobData.erase(blobData.size() - 1);	/* Removing last comma from blob data */
		}

		memset(insertQuery, 0x00, sizeof(insertQuery));
		sprintf(insertQuery,"%d",scanID);
		referenceData.append((const char *)insertQuery);
		bulkInsert = "INSERT INTO HornSignatureTable(ScanID,GraphData) VALUES("+ referenceData + ",'" + blobData + "');";

		queryStatus = m_DbConn.BulkTransaction((char *)bulkInsert.c_str());
				
		LOGDBG("DB_T : Horn scan signature insert query done : ErrCode : %d",queryStatus,0,0);

		blobData.clear();
		referenceData.clear();

		bulkInsert.clear();
		delete [] CP->Signature;
		CP->Signature = NULL;
		CP->NoOfSamples = 0;

	}
}

/**************************************************************************//**
* \brief   - Writing into DB( SCAN RESULT)
*
* \param   - None
*
* \return  - UINT8 success or fail (DB Write)
*
******************************************************************************/
UINT8 DataBaseManager::StoreScanResult()
{
	char 	insertQuery[MSG_SIZE] = {0x00};
	UINT8 	status  = RESPONSE_SUCCESS;
	INT32	ErrCode = 0;
	string 	dateTime;
	GetDateAndTime(dateTime);
	
	sprintf(insertQuery,"INSERT INTO HornResultTable(ScanName,DateTime,mainParallelResonantFreq,secParallelResonantFreq,thirdParallelResonantFreq,firstSeriesResonantFreq,secSeriesResonantFreq,thirdSeriesResonantFreq) VALUES('','%s',%d,%d,%d,%d,%d,%d)",
	(char *)dateTime.c_str(),
	CP->ScanResult.GetResonantFrequency(HornScanResults::scanResultIdx = MAIN_PARALLEL_FREQ),
	CP->ScanResult.GetResonantFrequency(HornScanResults::scanResultIdx = SECOND_PARALLEL_FREQ),
	CP->ScanResult.GetResonantFrequency(HornScanResults::scanResultIdx = TERT_PARALLEL_FREQ),
	CP->ScanResult.GetResonantFrequency(HornScanResults::scanResultIdx = FIRST_SERIES_FREQ),
	CP->ScanResult.GetResonantFrequency(HornScanResults::scanResultIdx = SECOND_SERIES_FREQ),
	CP->ScanResult.GetResonantFrequency(HornScanResults::scanResultIdx = TERT_SERIES_FREQ));
	
	ErrCode = m_DbConn.SingleTransaction(insertQuery);
	if(SQLITE_OK != ErrCode)
	{
		status = RESPONSE_FAIL;
		LOGERR("StoreScanResult : Writing horn scan result failed with ErrCode : %d",ErrCode,0,0);
	}
	
	CP->ScanResult.ResetHornScanResult();
	return status;
}

/**************************************************************************//**
* \brief   - Updates calculated(Horn Scan) digital tune in stack recipe table.
*
* \param   - None
*
* \return  - UINT8 success or fail (DB Write)
*
******************************************************************************/
UINT8 DataBaseManager::UpdateDigitalTune()
{
	UINT8 	status 					= RESPONSE_SUCCESS;
	INT32	ErrCode					= 0;
	UINT32  digitalTune	 			= CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE);
	vector<string> vStackParams;

    std::string strQuery = "select RecipeNumber from WeldRecipeTable where IsActive = 1 and RecipeNumber>0;";
	
    string recipeNum = m_DbConn.Exec((char*)strQuery.c_str()).c_str();
    
    string updateQuery  = "select RecipeNumber,DigitialTune,InternalOffsetFlag,InternalFreqOffset,EndOfWeldStore from StackRecipeTable where RecipeNumber = "+ recipeNum + ";";
    
    string stackEntry = m_DbConn.Exec((char*)updateQuery.c_str());
    
    if (stackEntry.empty())
    {
    	LOGERR("UpdateDigitalTune : Select query failed with ErrCode : %d",ErrCode,0,0);    	
    	return RESPONSE_FAIL;
    }

    // update DB record
    CP->StringToTokens(stackEntry.c_str(),vStackParams,',');
	vStackParams[1] = CP->ConvertIntToString(digitalTune/10);
	CP->TokensToString(stackEntry,vStackParams,',');
	
    // delete DB record
	updateQuery.clear();
	updateQuery = "delete from StackRecipeTable where RecipeNumber=" + recipeNum + ";";
	m_DbConn.Exec((char*)updateQuery.c_str());
	
    // insert DB record
	updateQuery.clear();
	updateQuery = "Insert into StackRecipeTable values(" + stackEntry + ");";
	m_DbConn.Exec((char*)updateQuery.c_str(), &ErrCode);
	
	// update recipe zero record
	vStackParams.clear();
    CP->StringToTokens(DBInterface::m_StackRecipeZero.c_str(),vStackParams,',');
	vStackParams[1] = CP->ConvertIntToString(digitalTune/10);
	CP->TokensToString(DBInterface::m_StackRecipeZero,vStackParams,',');

    if(SQLITE_OK != ErrCode)
	{
		status = RESPONSE_FAIL;
		LOGERR("UpdateDigitalTune : Update query failed with ErrCode : %d",ErrCode,0,0);
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Writing into DB (SEEK SIGNATURE)
*
* \param   - None
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
UINT8 DataBaseManager::StoreSeekSignature()
{
	char insertQuery[MSG_SIZE] = {0x00};
	string bulkInsert;
	UINT8 status = RESPONSE_FAIL;
	INT32 queryStatus = 0;
	
	if(CP->seekSignature != NULL)
	{
		LOGDBG("DB_T : Seek signature insert query start : %d",CP->NoOfSamples,0,0);
		
		for(INT32 idx = 0; idx < CP->NoOfSamples; idx++)
		{
			memset(insertQuery,0x00,sizeof(insertQuery));

			sprintf(insertQuery,"INSERT INTO SeekSignature(Time,Amplitude,Frequency,Power,Current,Phase) VALUES(%f,%d,%d,%d,%d,%d);",
					CP->seekSignature[idx].Time,CP->seekSignature[idx].Amplitude,CP->seekSignature[idx].Frequency,CP->seekSignature[idx].Power,
					CP->seekSignature[idx].Current,CP->seekSignature[idx].Phase);

			bulkInsert.append((const char *)insertQuery);
		}
		
		queryStatus = m_DbConn.BulkTransaction((char *)bulkInsert.c_str());
		
		if(SQLITE_OK == queryStatus)
		{
			status = RESPONSE_SUCCESS;
		}
		
		LOGDBG("DB_T : Seek signature insert query done : ErrCode : %d",queryStatus,0,0);

		bulkInsert.clear();
		delete [] CP->seekSignature;
		CP->seekSignature = NULL;
		CP->NoOfSamples = 0;		
	}
	else
	{
		LOGERR("DB_T : Seek signature empty",0,0,0);
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Gets seek result data from SeekResults class and write into DB
*
* \param   - void
*
* \return  - UINT8 - status (Success or Failure)
*
******************************************************************************/
UINT8 DataBaseManager::StoreSeekResult()
{
	char 	insertQuery[MSG_SIZE] = {0x00};
	UINT8 	status = RESPONSE_SUCCESS;
	INT32 	ErrCode = 0;
	SeekResult results;
	string dateTime;
	
	GetDateAndTime(dateTime);
	results = CommonProperty::seekResult.GetSeekData();
	
	sprintf(insertQuery,"INSERT INTO SeekResults(PeakPower,FrequencyChange,StartFrequency,EndFrequency,DateTime) VALUES(%d,%d,%d,%d,'%s');",
			results.PeakPower,(results.FrequencyChange/10),(results.StartFrequency/10), (results.EndFrequency/10), dateTime.c_str());
	
	ErrCode = m_DbConn.SingleTransaction(insertQuery);
	if(SQLITE_OK != ErrCode)
	{
		status = RESPONSE_FAIL;
		LOGERR("StoreSeekResult : Seek result writing into DB failed with ErrCode : %d",ErrCode,0,0);
	}
		
	return status;
}

/**************************************************************************//**
* \brief   - Clears the previous seek records
*
* \param   - string - tableName
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::DeleteHistory(string tableName)
{
	char deleteQuery[MSG_SIZE] = {0x00};
	INT32 queryStatus = 0;

	sprintf(deleteQuery,"DELETE FROM %s;",tableName.c_str());
	queryStatus = m_DbConn.BulkTransaction(deleteQuery);
	if(SQLITE_OK != queryStatus)
	{
		LOGERR("DB_T : Deleting history data : ErrCode : %d",queryStatus,0,0);  
	}
}

/**************************************************************************//**
* \brief   - Writing into DB( TEST SIGNATURE)
*      		
* \param   - None
*
* \return  - UINT8 -status of query exec
*
******************************************************************************/
UINT8 DataBaseManager::StoreTestSignature()
{
	char insertQuery[MSG_SIZE] = {0x00};
	string bulkInsert;
	string dateTime;
	INT32 queryStatus 	= 0;
	UINT8 status 		= RESPONSE_FAIL;
			
	if(CP->testSignature != NULL)
	{		
		LOGDBG("DB_T: Test signature insert query start",0,0,0);
		for(INT32 idx = 0; idx < CP->NoOfSamples; idx++)
		{
			memset(insertQuery,0x00,sizeof(insertQuery));
			
			sprintf(insertQuery,"INSERT INTO TestSignature(Time,Frequency,Amplitude,Power,Current,Phase) VALUES(%f,%d,%d,%d,%d,%f);",
					CP->testSignature[idx].Time,CP->testSignature[idx].Frequency,CP->testSignature[idx].Amplitude,
					CP->testSignature[idx].Power,CP->testSignature[idx].Current,CP->testSignature[idx].Phase);
			
			bulkInsert.append((const char *)insertQuery);
		}
		
		queryStatus = m_DbConn.BulkTransaction((char *)bulkInsert.c_str());		
		if(SQLITE_OK == queryStatus)
		{
			status = RESPONSE_SUCCESS;
		}
		LOGDBG("DB_T: Test signature insert query done : ErrCode : %d",queryStatus,0,0);
		
		bulkInsert.clear();
		delete [] CP->testSignature;
		CP->testSignature = NULL;
		CP->NoOfSamples = 0;				
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Writing into DB( TETS RESULT)
*      		
* \param   - None
*
* \return  - UINT8 -status of query exec
*
******************************************************************************/
UINT8 DataBaseManager::StoreTestResult()
{
	char insertQuery[MSG_SIZE] = {0x00};
	UINT8 status = RESPONSE_SUCCESS;
	INT32 ErrCode = 0;
	string dateTime;
	
	GetDateAndTime(dateTime);
	
	sprintf(insertQuery,"INSERT INTO TestResult(DateTime,TestTime,TestPeakPower,ResonantFreq,FrequencyChange) VALUES('%s',%d,%d,%d,%d)",
			dateTime.c_str(),
	CP->testResult.Get(TestResults::testResultIdx = TEST_TIME),		
	CP->testResult.Get(TestResults::testResultIdx = TEST_PEAK_POWER),
	CP->testResult.Get(TestResults::testResultIdx = RESONANT_FREQ),
	CP->testResult.Get(TestResults::testResultIdx = FREQUENCY_CHANGE));
	
	ErrCode = m_DbConn.SingleTransaction(insertQuery); 
	if(SQLITE_OK != ErrCode)
	{
		status = RESPONSE_FAIL;
		LOGERR("StoreTestResult : Test result writing into DB failed with ErrCode : %d",ErrCode,0,0);
	}
	
	return status;	
}

/**************************************************************************//**
* \brief   - Writing WeldSignature into DB
*
* \param   - char *pRecv_Buffer
*
* \return  - UINT8 - status of query exec
*
******************************************************************************/
UINT8 DataBaseManager::StoreWeldSignature(char *pRecv_Buffer)
{
	char 	insertQuery[MSG_SIZE]	= {0x00};
	UINT32 	recipeNumber			= 0;
	UINT32 	recipeRevNumber			= 0;
	UINT32	cycleCounter			= 0;
	UINT32  recordNumber			= 0;
	INT32  	queryStatus 			= 0;
	UINT32	totalRecord				= 0;
	UINT32	weldSignatureIndex		= 0;
	UINT8  	status = RESPONSE_FAIL, index = sizeof(MSG_ID);
	string 	bulkInsert,blobData,referenceData;
	
	memcpy(&totalRecord, pRecv_Buffer + index, sizeof(totalRecord));			/* Accumulated total signature count for particular weld cycle */
	index = index + sizeof(totalRecord);
	memcpy(&recipeNumber, pRecv_Buffer + index, sizeof(recipeNumber));		/* Recipe number for particular weld cycle */
	index = index + sizeof(recipeNumber);
	memcpy(&recipeRevNumber, pRecv_Buffer + index, sizeof(recipeRevNumber));	/* Recipe revision number for particular weld cycle */
	index = index + sizeof(recipeRevNumber);
	memcpy(&cycleCounter, pRecv_Buffer + index, sizeof(cycleCounter));		/* cycle counter for particular weld cycle */
	
	if( GetRowIDFromWeldResultTable(recordNumber) == true)
	{
		if(CP->m_qWeldSignature.size() > 0)
		{	
	#ifdef DEBUG
			LOGDBG("DB_T : Weld signature insert query start : ID : %d, TotalCount : %d",cycleCounter,totalRecord,0);
	#endif			
			totalRecord = totalRecord - CP->m_qWeldSignature.front().Time;
			
			while((!CP->m_qWeldSignature.empty()) && totalRecord > 1)		/* Loops till n-1 record */
			{
				
				memset(insertQuery,0x00,sizeof(insertQuery));

				sprintf(insertQuery,"%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,",
						(float)CP->m_qWeldSignature.front().Time,CP->m_qWeldSignature.front().Frequency,
						CP->m_qWeldSignature.front().Power,CP->m_qWeldSignature.front().Current,CP->m_qWeldSignature.front().Amplitude,
						CP->m_qWeldSignature.front().Phase,CP->m_qWeldSignature.front().Energy,CP->m_qWeldSignature.front().Force,
						CP->m_qWeldSignature.front().Velocity,CP->m_qWeldSignature.front().AbsoluteDistance,CP->m_qWeldSignature.front().CollapseDistance);

				blobData.append((const char *)insertQuery);
				CP->m_qWeldSignature.pop_front();
				
				totalRecord = totalRecord- 1;
			}
			if((!CP->m_qWeldSignature.empty()) &&  totalRecord == 1)		/* To get the n-th record */
			{
				memset(insertQuery,0x00,sizeof(insertQuery));

				sprintf(insertQuery,"%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
						(float)CP->m_qWeldSignature.front().Time,CP->m_qWeldSignature.front().Frequency,
						CP->m_qWeldSignature.front().Power,CP->m_qWeldSignature.front().Current,CP->m_qWeldSignature.front().Amplitude,
						CP->m_qWeldSignature.front().Phase,CP->m_qWeldSignature.front().Energy,CP->m_qWeldSignature.front().Force,
						CP->m_qWeldSignature.front().Velocity,CP->m_qWeldSignature.front().AbsoluteDistance,CP->m_qWeldSignature.front().CollapseDistance);

				blobData.append((const char *)insertQuery);
				CP->m_qWeldSignature.pop_front();
				
				totalRecord = totalRecord- 1;
			}
			
			memset(insertQuery,0x00,sizeof(insertQuery));
			sprintf(insertQuery,"%d,%d,%d,%d",recipeNumber,recipeRevNumber,cycleCounter,recordNumber);
			referenceData.append((const char *)insertQuery);
			bulkInsert = "INSERT INTO WeldResultSignature(RecipeNumber,WeldRecipeVerNumber,CycleCounter,RecordNumber,GraphData) VALUES("+ referenceData + ",'" + blobData + "');";
			queryStatus = m_DbConn.BulkTransaction((char *)bulkInsert.c_str());
			if(SQLITE_OK == queryStatus)
			{
				status = RESPONSE_SUCCESS;
			}
			else
			{
				LOGERR("DB_T : Weld signature failed with an ErrCode : %d, recipeNumber : %d, cycleCounter %d",queryStatus,recipeNumber,cycleCounter);
				RetryMechanism((char *)bulkInsert.c_str(),BULK_TRANSACTION,status,cycleCounter);
			}
	#ifdef DEBUG				
			LOGDBG("DB_T : Weld signature signature insert query done : ErrCode : %d",queryStatus,0,0);
	#endif		
			bulkInsert.clear();
			referenceData.clear();
			blobData.clear();
			
		}
	}
	else
	{
		LOGERR("DB_T : GetRowIDFromWeldResultTable failed for recipeNumber : %d, recipeRevNumber: %d, cycleCounter: %d",recipeNumber, recipeRevNumber, cycleCounter);	
	}
		
	return status;
}


/**************************************************************************//**
* \brief   - Get RowID From WeldResultTable
*
* \param   - recordNumber, RowID is returned in recordNumber
*
* \return  - bool - true on success, false on failure
*
******************************************************************************/
bool DataBaseManager::GetRowIDFromWeldResultTable(UINT32 &recordNumber)
{
	bool bResult = false;
	string strSelectRowCountQuery = "SELECT COUNT(*) from WeldResultTable;";
	string strSelectMaxRowIDQuery = "SELECT MAX(RowID) from WeldResultTable;";
	UINT32 RowID = 0;
	string strRowID = "";
	INT32 ErrCode = 0;
	
	//Check if at least 1 row is present in table
	strRowID = m_DbConn.Exec((char*)strSelectRowCountQuery.c_str(), &ErrCode);
	if(SQLITE_OK != ErrCode)
	{
		LOGERR("DB_T : Error on SELECT COUNT(*) from WeldResultTable : %d",ErrCode,0,0);
	}
	else
	{
		RowID = atoi(strRowID.c_str());
		if(RowID != 0)
		{
			//Rows present in table, so find out max RowID.
			strRowID = "";
			RowID = 0;
			strRowID = m_DbConn.Exec((char*)strSelectMaxRowIDQuery.c_str(), &ErrCode);
			
			if(SQLITE_OK != ErrCode)
			{
				LOGERR("DB_T : Error on SELECT MAX(RowID) from WeldResultTable : %d",ErrCode,0,0);
			}
			else
			{
				bResult = true;
				RowID = atoi(strRowID.c_str());
				//Increment RowID because WeldResultTable will be inserted with a new row after WeldResultSignature Table insert operation.
				RowID = RowID + 1;
			}
		}
		//No row present in table, so return RowID as 1 for first record.
		else
		{
			bResult = true;
			RowID = 1;
		}
	}
	recordNumber = RowID;
	return bResult;
}

/**************************************************************************//**
* \brief   - Writing WeldResult into DB
*      		
* \param   - char *pRecv_Buffer
*
* \return  - UINT8 -status of query exec
*
******************************************************************************/
UINT8 DataBaseManager::StoreWeldResult(char *pRecv_Buffer)
{
	char 	 insertQuery[DB_QUERY_SIZE] = {0x00};
	INT32		weldMode				= 0;
	INT32		recipeRevNumber			= 0;
	UINT8  		status 					= RESPONSE_FAIL;;
	WeldResult 	results;
	string 		dateTime,userName,bulkInsert;
	INT32		ErrCode					= 0;
	string	 	batchData				= "";
	char 		*token;
	char 		BatchID[BATCHID_SIZE]	= {0x00};
	char 	bcPartId[BARCODE_DATA_SIZE]	= {0x00};
	Message message;
	VendorSpecificInfo * VSInfo = VendorSpecificInfo::Getinstance();
	GetDateAndTime(dateTime);
	
	/* Copies enter weld results of particular cycle */
	memcpy(&results, pRecv_Buffer + sizeof(MSG_ID), sizeof(results));
	CommonProperty::weldResult.GetPartID(bcPartId);
	
	/* Writing Batch ID to Weld result table */
	sprintf(insertQuery,"SELECT BatchSetupEnable,BatchID FROM ProductionSetupTable WHERE RecipeNumber = %d;",results.RecipeNumber);
#ifdef BATCH_DEBUG
	printf("DB_T : BatchID Query : %s\n",insertQuery);
#endif
	batchData = m_DbConn.Exec(insertQuery, &ErrCode);
	
#ifdef BATCH_DEBUG
	cout << "DB_T : Batch setup data : " << batchData << endl;
#endif
	
	if(SQLITE_OK != ErrCode)
	{
		LOGERR("DB_T : Error on select BATCH ID from ProductionSetupTable : %d",ErrCode,0,0);
	}
	else
	{	
		if((batchData.length()) > 0)
		{
			token = strtok((char *)batchData.c_str(), ",");
			m_batchSetupEnable = atoi(token);
			if(m_batchSetupEnable)
			{
				token = strtok(NULL, ",");
				memcpy(&BatchID,token,sizeof(BatchID));
			}
			else
			{
				strcpy(BatchID,"NA");
			}
		}
		else
		{
			strcpy(BatchID,"NA");
		}
	}
	
	sprintf(insertQuery, "INSERT INTO WeldResultTable(RecipeNumber,WeldRecipeVerNumber,DateTime,StackSerialNo,CycleCounter,WeldMode,MaxWeldForce,EndHoldForce,WeldAbsolute,TotalAbsolute,WeldCollapseDistance,HoldCollapseDistance,TotalCollapseDistance,TriggerDistance,Velocity,WeldTime,WeldEnergy,WeldPeakPower,StartFrequency,FrequencyChange,CycleTime,Username,PartID,BatchID,TriggerPoint,WeldSonicPoint,HoldPoint,AlarmFlag,RecipeStatus) VALUES(%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s',%f,%f,%f,%d,%d);",
			results.RecipeNumber, results.RecipeRevNumber, dateTime.c_str(), results.StackSerialNo, results.CycleCounter, 
			results.WeldMode, results.MaxWeldForce, results.EndHoldForce, results.WeldAbsolute, results.TotalAbsolute, 
			results.WeldCollapseDistance, results.HoldCollapseDistance, results.TotalCollapseDistance, results.TriggerDistance,
			results.Velocity, results.WeldTime/CONVERSION_SECONDS, results.TotalEnergy, results.PeakPower,
			(results.StartFrequency/CONVERSION_KHZ), (results.FrequencyChange/CONVERSION_KHZ), results.CycleTime/CONVERSION_SECONDS, results.UserName, 
			bcPartId, BatchID, (float)results.TriggerTime/CONVERSION_CYCLE, (float)results.WeldSonicTime/CONVERSION_CYCLE,
			(float)results.HoldTime/CONVERSION_CYCLE, results.bIsAlarmflag, results.recipeStatus);
	
	VSInfo->UpdateLastWeldResultsData(results,BatchID,dateTime);

#ifdef DEBUG
	LOGDBG("DB_T : Weld result query length : %d",strlen(insertQuery),0,0);
#endif /* DEBUG */
	
	ErrCode = m_DbConn.SingleTransaction(insertQuery);
	if(SQLITE_OK != ErrCode)
	{
		LOGERR("StoreWeldResult : Weld result writing into DB failed with ErrCode : %d, CycleCounter %d",ErrCode,results.CycleCounter,0);
		RetryMechanism(insertQuery,SINGLE_TRANSACTION,status,results.CycleCounter);
	}
	else
	{
		status = RESPONSE_SUCCESS;
	}
	
	// PPM control loop
	message.msgID = TO_CTRL_TASK_UL_WELD_SIGNATURE;
	SendMsgToMsgQ(message,CTRL_MSG_Q_ID);

	if((SQLITE_OK == ErrCode) || (RESPONSE_SUCCESS == status))
	{
		ProductionDataAnalysis(results.RecipeNumber, results.WeldStatus, results.CycleCounter);
		BatchSetupValidation(results.RecipeNumber, results.bIsAlarmflag);
	}
	
	return status;	
}

/**************************************************************************//**
* \brief   - Update the production step table based on analysis of
*      		 weld results.
*      		
* \param   - INT32 recipeNumber, UINT8 weldStatus, UINT32 cycleCounter
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ProductionDataAnalysis(INT32 recipeNumber, UINT8 weldStatus, UINT32 cycleCounter)
{
	char 	queryData[MSG_SIZE] = {0x00};
	UINT8	status				= RESPONSE_FAIL;
	INT32	ErrCode				= 0;
	UINT32 	count 				= 0;
	string 	cycleCount, weldStatusName = "";
	
	CommonProperty::weldResult.GetWeldStatus(weldStatus,weldStatusName,status);
	
	if(0 != status)
	{		
		sprintf(queryData,"UPDATE ProductionSetupTable SET %s = %s + 1, CycleCount = %d WHERE RecipeNumber = %d;",
				weldStatusName.c_str(),weldStatusName.c_str(), cycleCounter, recipeNumber);
	}
	else
	{
		LOGERR("DB_T : Error : Wrong weld status(Good,PartsPerMin,Suspect,Reject) : %d, recipeNumber : %d",weldStatus,recipeNumber,cycleCounter);
		sprintf(queryData,"UPDATE ProductionSetupTable SET CycleCount = %d WHERE RecipeNumber = %d;", cycleCounter, recipeNumber);
	}

#ifdef DEBUG
	printf("queryData : %s\n",queryData);
#endif
	ErrCode = m_DbConn.BulkTransaction(queryData);
	if(SQLITE_OK != ErrCode)
	{
		LOGERR("DB_T : Error on updating production data with ErrCode : %d, recipeNumber : %d",ErrCode,recipeNumber,cycleCounter);
		RetryMechanism(queryData,BULK_TRANSACTION,status,cycleCounter);
	}
}

/**************************************************************************//**
* \brief   - Resets all production settings for new recipe such as weld cycle
* 			 counter, batch setup counter and alarm.
*      		
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ResetProductionSetting()
{
	UINT32 recipeNumber = CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM);
	LOGDBG("DB_T : Read cycle/batch counter from DB for recipe : %d",recipeNumber,0,0);
	ReadCycleCounter(recipeNumber);
	BatchSetupValidation(recipeNumber, false, RESET_PRODUCTION);
}

/**************************************************************************//**
* \brief   - The function re-tries twice the same sql query if the query 
* 			 failed to execute in it's previous attempt.
*      		
* \param   - char *sqlQuery, UINT8 typeOfTrans, UINT8& status, UINT32 cycleCounter
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::RetryMechanism(char *sqlQuery, UINT8 typeOfTrans, UINT8& status, UINT32 cycleCounter)
{
	INT32 queryStatus 	= 1;
	
	if(BULK_TRANSACTION == typeOfTrans)
	{
		queryStatus = m_DbConn.BulkTransaction(sqlQuery, RETRY_COUNT_FOR_FIVE_SEC_DELAY);
	}
	else
	{
		queryStatus = m_DbConn.SingleTransaction(sqlQuery, RETRY_COUNT_FOR_FIVE_SEC_DELAY);
	}
		
	if (queryStatus != SQLITE_OK)
	{
		LOGERR("DB_T : Executing query failed with ErrCode : %d, cycleCounter %d",queryStatus,cycleCounter,0);
	}
	else
	{
		status = RESPONSE_SUCCESS;
	}
}

/**************************************************************************//**
* \brief   - Reading current alarm configuration from DB.
*      		
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ReadAlarmConfFromDB()
{
	INT32 queryStatus = 0;
	string 	alarmConfiguration;
	char 	SelectQuery[MSG_SIZE] = {0x00};	
	vector<bool> alarmConfVector;
	int alarmIndex;
	INT8 alarmCounter = 0;
	INT8 alarmSetCounter = 0;		
	bitset<ALARM_DATA_SIZE_BITS> alarmConfbitSet;			//8 Byte alarm data.
	Message message;
	
	sprintf(SelectQuery, "SELECT ResetRequired, LogAlarm, GeneralAlarm, CustomAlarmOutput, CycleCounter FROM AlarmConfiguration;");	
	alarmConfiguration = m_DbConn.Exec(SelectQuery, &queryStatus);
	
	if(SQLITE_OK != queryStatus)
	{		
		LOGERR("DB_T : GetAlarmConfiguration : ErrCode from DB : %d",queryStatus,0,0);		//TODO: Use logging level 
	}	
	
	// DB Alarm data Write when nothing is written
	if(strlen(alarmConfiguration.c_str()) <= 0)
	{
		// configuration is empty ask alarm manager for the default
		message.msgID = TO_ALARM_TASK_CREATE_DEFAULT_ALARM_CONFIG;
	
		// send it to the alarm task
		SendMsgToMsgQ(message,ALARM_MSG_Q_ID);
	}	
	else
	{
		// Read it from the DB if the alarm configuration is already present */	
		alarmConfVector.clear();
		stringstream alarmConfSS(alarmConfiguration);		//Parse it from the string
		
		while (alarmConfSS >> alarmIndex)			
		{
			alarmConfVector.push_back(alarmIndex);
			if (alarmConfSS.peek() == ',')
			{
				alarmConfSS.ignore();
			}				
		}
		
		for (alarmIndex = 0, alarmCounter = 1; alarmIndex < alarmConfVector.size(); alarmIndex++, alarmCounter++)
		{
			if( (alarmConfVector.at(alarmIndex) & 1) == 1 )
			{			
				alarmConfbitSet.set(alarmSetCounter);
			}
			else
			{			
				alarmConfbitSet.reset(alarmSetCounter);
			}
			if(alarmCounter % ALARM_ACTION_SIZE_BITS == 0)
			{				
				alarmConfbitSet.reset(++alarmSetCounter);
				alarmConfbitSet.reset(++alarmSetCounter);
				alarmConfbitSet.reset(++alarmSetCounter);				
			}
			++alarmSetCounter;			
		}
		
		// set this as the current alarm configuration here
		message.msgID = TO_ALARM_TASK_CREATE_CURRENT_ALARM_CONFIG;
		memcpy (message.Buffer, &alarmConfbitSet, sizeof (AlarmConfiguration)); 
		
		// send it to the alarm task
		SendMsgToMsgQ(message,ALARM_MSG_Q_ID);
		
#ifdef DEBUG
		LOGDBG("DB_T : Read Alarm configuration from DB", 0, 0, 0);
#endif
	}	
}

/**************************************************************************//**
* \brief   - Writing current alarm configuration to DB.
*      		
* \param   - pAlarmConfiguration pAlarmConf
*
* \return  - STATUS
*
******************************************************************************/
STATUS DataBaseManager::WriteAlarmConfigToDB (pAlarmConfiguration pAlarmConf)
{
	INT32 queryStatus = 0;
	AlarmConfiguration alarmConf;	
	char 	InsertQuery[MSG_SIZE] = {0x00};	
	string 	bulkInsert;	
	Message message;
	
	memcpy(&alarmConf, pAlarmConf, sizeof(alarmConf));
		
	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"Overload", alarmConf.OverloadAlarmActions.ResetRequired,alarmConf.OverloadAlarmActions.LogAlarm,
			alarmConf.OverloadAlarmActions.GeneralAlarm,alarmConf.OverloadAlarmActions.CustomAlarmOutput,
			alarmConf.OverloadAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);

	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"CycleModified", alarmConf.CycleModifiedAlarmActions.ResetRequired,alarmConf.CycleModifiedAlarmActions.LogAlarm,
			alarmConf.CycleModifiedAlarmActions.GeneralAlarm,alarmConf.CycleModifiedAlarmActions.CustomAlarmOutput,
			alarmConf.CycleModifiedAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);

	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"Warning", alarmConf.WarningAlarmActions.ResetRequired,alarmConf.WarningAlarmActions.LogAlarm,
			alarmConf.WarningAlarmActions.GeneralAlarm,alarmConf.WarningAlarmActions.CustomAlarmOutput,
			alarmConf.WarningAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);

	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"ProcessSuspect", alarmConf.SuspectAlarmActions.ResetRequired,alarmConf.SuspectAlarmActions.LogAlarm,
			alarmConf.SuspectAlarmActions.GeneralAlarm,alarmConf.SuspectAlarmActions.CustomAlarmOutput,
			alarmConf.SuspectAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);

	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"ProcessReject", alarmConf.RejectAlarmActions.ResetRequired,alarmConf.RejectAlarmActions.LogAlarm,
			alarmConf.RejectAlarmActions.GeneralAlarm,alarmConf.RejectAlarmActions.CustomAlarmOutput,
			alarmConf.RejectAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);

	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"NoCycle", alarmConf.NoCycleAlarmActions.ResetRequired,alarmConf.NoCycleAlarmActions.LogAlarm,
			alarmConf.NoCycleAlarmActions.GeneralAlarm,alarmConf.NoCycleAlarmActions.CustomAlarmOutput,
			alarmConf.NoCycleAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);

	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"HardwareFailure", alarmConf.HardwareFailureAlarmActions.ResetRequired,alarmConf.HardwareFailureAlarmActions.LogAlarm,
			alarmConf.HardwareFailureAlarmActions.GeneralAlarm,alarmConf.HardwareFailureAlarmActions.CustomAlarmOutput,
			alarmConf.HardwareFailureAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);

	memset(InsertQuery,0x00,sizeof(InsertQuery));
	sprintf(InsertQuery,"INSERT INTO AlarmConfiguration(AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter) VALUES('%s',%d,%d,%d,%d,%d);",
			"NoCycleOverload", alarmConf.NoCycleOverloadAlarmActions.ResetRequired,alarmConf.NoCycleOverloadAlarmActions.LogAlarm,
			alarmConf.NoCycleOverloadAlarmActions.GeneralAlarm,alarmConf.NoCycleOverloadAlarmActions.CustomAlarmOutput,
			alarmConf.NoCycleOverloadAlarmActions.CycleCounter);
	bulkInsert.append((const char *)InsertQuery);
	
	queryStatus = m_DbConn.BulkTransaction((char *)bulkInsert.c_str());
	if(SQLITE_OK != queryStatus)	
	{
		LOGERR("DB_T : Alarm configuration insert query done : ErrCode : %d", queryStatus, 0, 0);
	}
#ifdef DEBUG
	LOGDBG("DB_T : Wrote Alarm configuration to DB", 0, 0, 0);
#endif
	return queryStatus;
}


/**************************************************************************//**
* \brief   - Gets the name of event from event map.
*
* \param   - LOG_EVENT eventID
*
* \return  - string - Event name
*
******************************************************************************/
string DataBaseManager::GetEventName (LOG_EVENT eventID)
{
	return m_EventNameMap.find(eventID)->second;
}


/**************************************************************************//**
* \brief   - Global entry point for returning current login information
* 			 for specific requests to be logged in the event log
*
* \return  - string m_EventUserName
* 
******************************************************************************/
string DataBaseManager::EventLogUserName()
{
	return m_EventUserName;
}


/**************************************************************************//**
* \brief   - Global entry point for recipe revision number to be logged
* 			in the event log with a recipe save event (intercept)
*
* \param   - string revisionNum
* 
* \return  - None
* 
******************************************************************************/
void DataBaseManager::EventLogRecipeRevision (string revisionNum)
{
	recipeRevisionNum = revisionNum;
}

/**************************************************************************//**
* \brief   - Global entry point for the preparation of log information
* 			 of specific requests to be logged in the event log, and the sending
* 			 to the DB task for further processing
*
* \param   - FW_INFO_IDX 	fwIndex
* \param   - void * 		info
*
* \return  - STATUS		status
* 
******************************************************************************/
STATUS DataBaseManager::EnterEventLogSWVersion (FW_INFO_IDX fwIndex, void* info)
{
	Message message;
	message.msgID = TO_DB_TASK_LOG_EVENT;
	char current_version[SYSINFO_SIZE] = { 0x00 };
	char prev_version[SYSINFO_SIZE] = { 0x00 };
	char fwUpgradeFlag= 0x00;
	Eeprom eeprom;
	STATUS status = OK;
	
	switch (fwIndex)
	{
	case FW_INFO_SC:

#ifdef MAINLINE_BUGFIX_BUILD
	snprintf(current_version, sizeof (current_version), "%d.%d.%d.%d",
			FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),FWVersion::Get(FW_VERSION_SC, VERSION_BUILD), SC_FW_VERSION_AUTOS);
#else
	snprintf(current_version, sizeof (current_version), "%d.%d.%d",FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),FWVersion::Get(FW_VERSION_SC, VERSION_BUILD));
#endif
		
		eeprom.Read ((char *) prev_version, SYSINFO_SIZE, SC_FW_VER_EEPROM_OFFSET);
		
		if (strcmp (current_version, prev_version))
		{
			snprintf (message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d,Identifier:Supervisory Controller:From:%s:To:%s:", SYSTEM_SW_VER_CHANGE, 
					CheckValidVersion (prev_version) ? prev_version : "N/A", current_version);
	
			// send it to the event log
			status = msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DB_T]), reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL);
			
			// save new version
			eeprom.Write ((char *) current_version, SYSINFO_SIZE, SC_FW_VER_EEPROM_OFFSET);
		}
		break;

	case FW_INFO_AC:

#ifdef MAINLINE_BUGFIX_BUILD
		snprintf(current_version, sizeof (current_version), "%d.%d.%d.%d",
				FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),FWVersion::Get(FW_VERSION_AC, VERSION_BUILD), AC_FW_VERSION_AUTOS);
#else
		snprintf(current_version, sizeof (current_version), "%d.%d.%d",FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),FWVersion::Get(FW_VERSION_AC, VERSION_BUILD));
#endif
		
		eeprom.Read ((char *) prev_version, SYSINFO_SIZE, AC_FW_VER_EEPROM_OFFSET);
		
		if (strcmp (current_version, prev_version))
		{
			snprintf (message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d,Identifier:Actuator Controller:From:%s:To:%s:", SYSTEM_SW_VER_CHANGE, 
					CheckValidVersion (prev_version) ? prev_version : "N/A", current_version);

			// send it to the event log
			status = msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DB_T]), reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL);
			
			// save new version
			eeprom.Write ((char *) current_version, SYSINFO_SIZE, AC_FW_VER_EEPROM_OFFSET);
		}
		break;
	
	case FW_INFO_PC:
		
#ifdef MAINLINE_BUGFIX_BUILD
		snprintf(current_version, sizeof (current_version), "%d.%d.%d.%d",
				FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),FWVersion::Get(FW_VERSION_PC, VERSION_BUILD), 
				(FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR) == 2 && FWVersion::Get(FW_VERSION_PC, VERSION_MINOR) == 0) ? 1 : PC_FW_VERSION_AUTOS);
#else
		snprintf(current_version, sizeof (current_version), "%d.%d.%d",FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),FWVersion::Get(FW_VERSION_PC, VERSION_BUILD));
#endif		
		eeprom.Read ((char *) prev_version, SYSINFO_SIZE, PC_FW_VER_EEPROM_OFFSET);
		
		if (strcmp (current_version, prev_version) and status == OK)
		{
			snprintf (message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d,Identifier:Power Controller:From:%s:To:%s:", SYSTEM_SW_VER_CHANGE, 
					CheckValidVersion (prev_version) ? prev_version : "N/A", current_version);
			
			// send it to the event log
			status = msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DB_T]), reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL);
			
			// save new version
			eeprom.Write ((char *) current_version, SYSINFO_SIZE, PC_FW_VER_EEPROM_OFFSET);
		}
		break;
		
	case FW_INFO_UIC:
		
		if ((string *) info)
		{
			snprintf(current_version, sizeof (current_version), "%s", ((string *) info)->c_str());
			eeprom.Read ((char *) prev_version, SYSINFO_SIZE, UIC_FW_VER_EEPROM_OFFSET);
			
			if (strcmp (current_version, prev_version) and status == OK)
			{
				snprintf (message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d,Identifier:HMI Controller:From:%s:To:%s:", SYSTEM_SW_VER_CHANGE, 
						CheckValidVersion (prev_version) ? prev_version : "N/A", current_version);
				
				// send it to the event log
				status = msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DB_T]), reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL);
				
				// save new version
				eeprom.Write ((char *) current_version, SYSINFO_SIZE, UIC_FW_VER_EEPROM_OFFSET);
			}
		}
		break;

	default:
		status = ERROR;
		break;
	}
	
	return status;
}


/**************************************************************************//**
* \brief   - Global entry point for the preparation of log information
* 			 of specific requests to be logged in the event log, and the sending
* 			 to the DB task for further processing
*
* \param   - LOG_EVENT 	eventID
* \param   - string& 	details
*
* \return  - STATUS		status
* 
******************************************************************************/
STATUS DataBaseManager::EnterEventLog(LOG_EVENT eventID, string& details)
{
	Message message;
	message.msgID = TO_DB_TASK_LOG_EVENT;

	snprintf (message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d,%s", eventID, details.c_str());
	
	// send it to the event log
	return msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DB_T]), reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL);
}


/**************************************************************************//**
* \brief   - Global entry point for writing event log non-volatile signals
* 			 of specific requests to be logged in the event log after
* 			 power up 
*
* \param   - LOG_EVENT 	eventID
* \param   - void* 		data
*
* \return  - ssize_t	status
* 
******************************************************************************/
ssize_t DataBaseManager::EnterEventLogNVM (LOG_EVENT eventID, void* data)
{
	Eeprom _epclass;
	ssize_t _status = 0;
	char username[EVENTLOG_USERNAME_SIZE];
	
	switch (eventID)
	{
	case SYSTEM_DB_DELETED:

		if (data)
		{
			_status = _epclass.Write ((char *) data, EVENTLOG_DBDELETED_SIG_SIZE, EVENTLOG_BASE + EVENTLOG_DBDELETED_SIG_OFFSET);
			
			if (*((char *)data) == EVENTLOG_DBDELETED_SIG_ACTIVE)
			{	
				snprintf (username, sizeof (username), "%s", DataBaseManager::EventLogUserName().c_str());
				_status = _epclass.Write (username , EVENTLOG_USERNAME_SIZE, EVENTLOG_BASE + EVENTLOG_USERNAME_OFFSET);
			}
		}
		break;
		
	case USER_LOGIN:
		
		snprintf (username, sizeof (username), "%s", DataBaseManager::EventLogUserName().c_str());
		_status = _epclass.Write (username , EVENTLOG_USERNAME_SIZE, EVENTLOG_BASE + EVENTLOG_USERNAME_OFFSET);
		break;

	default:
		break;
	}

	return _status;
}


/**************************************************************************//**
* \brief   - Global entry point for reading event log non-volatile signals
* 			 of specific requests to be logged in the event log after
* 			 power up 
*
* \param   - LOG_EVENT 	eventID
* \param   - void* 		data
*
* \return  - ssize_t	status
* 
******************************************************************************/
ssize_t DataBaseManager::GetEventLogNVM (LOG_EVENT eventID, void* data)
{
	Eeprom _epclass;
	ssize_t _status = 0;

	switch (eventID)
	{
	case SYSTEM_DB_DELETED:
		if (data)
			_status = _epclass.Read ((char *) data, EVENTLOG_DBDELETED_SIG_SIZE, EVENTLOG_BASE + EVENTLOG_DBDELETED_SIG_OFFSET);
		break;
		
	case USER_LOGIN:
		
		if (data)
			_status = _epclass.Read ((char *) data, EVENTLOG_USERNAME_SIZE, EVENTLOG_BASE + EVENTLOG_USERNAME_OFFSET);
		break;
		
	default:
		break;
	}

	return _status;
}


/**************************************************************************//**
* \brief   - Global entry point for the preparation of deletion of log
*			 information of specific logs, and the sending to the DB task
*			 for further processing
*
* \param   - EVENT_LOGS logID
*
* \return  - STATUS		status
* 
******************************************************************************/
STATUS DataBaseManager::EnterEventLogDeleteLog (EVENT_LOGS logID)
{
	Message message;
	message.msgID = TO_DB_TASK_LOG_EVENT;

	snprintf (message.Buffer, MAX_SIZE_OF_MSG_BUFF, "%d,Identifier:%d:From:N/A:To:N/A:", SYSTEM_LOG_DELETED, logID);
	
	// send it to the event log
	return msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DB_T]), reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL);
}


/**************************************************************************//**
* \brief   - 	Records all BL related events into DB.
*
* \param   	- 	LOG_EVENT eventID
* \param 	-	string* details
*
* \return  	- 	bool	status
*
******************************************************************************/
bool DataBaseManager::EventLog (LOG_EVENT eventID, string* details)
{
	enum {
		IDENTIFIR,
		IDENTIFIR_VAL,
		FROM,
		FROM_VAL,
		TO,
		TO_VAL
	};
	
	string 	dateTime, eventName, comment, PSserialNo, ACserialNo, AUXserialNo, sDetails;
    vector<string> vDetails;
    stringstream request;
    EVENT_LOGS logID;
    ACTIVE_RECIPE paramID;
	char username[EVENTLOG_USERNAME_SIZE];
    static SystemConfiguration sysConf;
    
    char 	insertQuery[MSG_SIZE*2] = {0x00};
	INT32 	ErrCode = 0;
	bool	status = false;

	if (!sysConf.IsSysConfigCached())
		sysConf.SysConfigCache();
	
	if (details)
		sDetails = *details;
	else
		sDetails = "Identifier:N/A:From:N/A:To:N/A:";
		
	request.str (sDetails);
	
	// break the details, system time is delimited differently.
	while (getline (request, sDetails, (eventID == SYSTEM_TIME_CHANGED ? ';' : ':')))
		vDetails.push_back (sDetails);
	
	eventName = GetEventName(eventID);
	GetDateAndTime (dateTime);
	
	switch (eventID)
	{
	case USER_LOGIN:
		
		m_EventUserName = vDetails[IDENTIFIR_VAL];
		vDetails[IDENTIFIR_VAL] = m_EventCommentMap.find(eventID)->second;
		break;
	
	case SYSTEM_CONFIGURATION:
		
		sysConf.SysConfigCache();
		
		break;
		
	case SYSTEM_DB_DELETED:
		
		GetEventLogNVM (USER_LOGIN, username);
		vDetails[IDENTIFIR_VAL] = username;
		break;
		
	case SYSTEM_LOG_DELETED:

		logID = (EVENT_LOGS) atoi (vDetails[IDENTIFIR_VAL].c_str());
		vDetails[IDENTIFIR_VAL] = m_EventLogNameMap.find(logID)->second;
		break;
		
	case SYSTEM_SW_VER_CHANGE:
		
		GetEventLogNVM (USER_LOGIN, username);
		comment = username;
		break;
	
	case RECIPE_MODIFIED:
		
		paramID = (ACTIVE_RECIPE) atoi (vDetails[IDENTIFIR_VAL].c_str());
		
		vDetails[IDENTIFIR_VAL] = (paramID < SC_RECIPE_NUMBER || paramID > ACTIVE_RECIPE_END) ? "N/A" : m_RecipeParamMap.find(paramID)->second;
		
		// special cases
		if (paramID == SC_WELD_MODE)
			{vDetails[FROM_VAL] = m_RecipeModeMap.find ((WELD_MODES) atoi (vDetails[FROM_VAL].c_str()))->second; 
				vDetails[TO_VAL] = m_RecipeModeMap.find ((WELD_MODES) atoi (vDetails[TO_VAL].c_str()))->second;}

		if (paramID == SC_AMP_STEP_AT || paramID == SC_FORCE_STEP_AT)
			{vDetails[FROM_VAL] = m_StepTypeMap.find ((STEP_TYPE) atoi (vDetails[FROM_VAL].c_str()))->second; 
				vDetails[TO_VAL] = m_StepTypeMap.find ((STEP_TYPE) atoi (vDetails[TO_VAL].c_str()))->second;}
		
		if (vDetails[IDENTIFIR_VAL].find ("Enable") != string::npos)
			{vDetails[FROM_VAL] = vDetails[FROM_VAL] == "1" ? "ON" : "OFF"; vDetails[TO_VAL] = vDetails[TO_VAL] == "1" ? "ON" : "OFF";}
		
		// note the active recipe
		comment = "Recipe " + m_DbConn.Exec("select RecipeNumber from WeldRecipeTable where IsActive=1;");
		break;
		
	case HORN_SCAN_COMPLETED:
	case USER_CREATED:
	case USER_MODIFIED:
	case USER_LOGOUT:
	case USER_LOGOUT_WEB:
	case USER_LOGIN_WEB:
	case RECIPE_CREATED:
	case RECIPE_COPIED:
	case RECIPE_SAVED:
	case RECIPE_VALIDATION:
	case SEEK_PERFORM:
	case CYCLE_COUNT_RESET:
	case PART_CONTACT_PERFORM:
	case CALIBRATION_COMPLETED:
	case SYSTEM_FACTORY_RESET:
	case SYSTEM_TIME_CHANGED:
	case SYSTEM_BATT_LOW:
	case SYSTEM_HW_CHANGED:
	case WEB_SERVICES:
	case RECIPE_DELETED:	
		break;
	}
	
	PSserialNo = sysConf.SysConfigGetCache().SC_SerialNumber; 
	ACserialNo = sysConf.SysConfigGetCache().AC_SerialNumber; 
	AUXserialNo = sysConf.SysConfigGetCache().AuxBoxSerialNumber; 
	
	snprintf(insertQuery, sizeof(insertQuery), "INSERT INTO EventLogTable(EventName,DateTime,PSSerialNo,ACSerialNo,AUXSerialNo,UserID,EventID,Identifier,Old,New,Comment) VALUES('%s','%s','%s','%s','%s','%s',%d,'%s','%s','%s','%s');",
			eventName.c_str(), dateTime.c_str(), PSserialNo.c_str(), ACserialNo.c_str(), AUXserialNo.c_str(), m_EventUserName.c_str(), eventID, vDetails[IDENTIFIR_VAL].c_str(), vDetails[FROM_VAL].c_str(), vDetails[TO_VAL].c_str(), comment.c_str() );

	ErrCode = m_DbConn.SingleTransaction(insertQuery);
//	printf ("\n%sDataBaseManager::EventLog: %s ErrCode: %d %s\n", KBLU, insertQuery,ErrCode,KNRM);

	if(SQLITE_OK == ErrCode)
	{
		status = true;
	}
	else
		LOGERR("DB_T : Error on updating event log data with ErrCode : %d",ErrCode,0,0);
	
	if (eventID == USER_LOGOUT || eventID == USER_LOGOUT_WEB)
		m_EventUserName = "N/A";
		
	if (eventID == USER_LOGIN_WEB) 
		m_EventUserName = vDetails[IDENTIFIR_VAL];
	
	ProcessTableCapacity(ALARM_LOG_TABLE);
	
	return status;
}


/**************************************************************************//**
* \brief   	- 	Inserts related comments of deprecated event names into DB
* 				after the fact
*
* \param 	-	string& evantName
* \param 	-	string& details
*
* \return  	- 	bool	status
*
******************************************************************************/
bool DataBaseManager::EventLogComment(string& userName, string& eventName, string& comment)
{
    char			updateQuery[MSG_SIZE] = {0x00};
	string 			maxEventNum, details, _comment;
	stringstream 	data;
	INT32 			ErrCode = 0;
	bool			status = false;
	map<LOG_EVENT,string>::iterator it = m_EventNameMap.begin();
	
	for (;it != m_EventNameMap.end();it++)
	{
		if (it->second == eventName)
		{
			// special cases
			if (eventName == GetEventName (USER_LOGOUT))
			{
				details = "Identifier:N/A:From:N/A:To:N/A:";
				EventLog (USER_LOGOUT, &details);
			}
			else if (eventName == GetEventName (USER_LOGIN))
			{
				details = "Identifier:" + userName + ":From:N/A:To:N/A:";
				m_EventCommentMap[USER_LOGIN] = comment.c_str();
				EventLog (USER_LOGIN, &details);
			}
			else if (eventName == GetEventName (SYSTEM_HW_CHANGED))
			{
				details = "Identifier:Board Replaced:From:N/A:To:N/A:";
				EventLog (SYSTEM_HW_CHANGED, &details);
			}
			else if (eventName == GetEventName (RECIPE_SAVED))
			{
				// recall updated recipe revision (intercept)
				comment = "Revision " + recipeRevisionNum;
			}
		
//			printf ("\n%sDataBaseManager::EventLogComment: eventName: %s comment: %s %s\n", KBLU, eventName.c_str(), comment.c_str(), KNRM);

			maxEventNum = m_DbConn.Exec ((char*) "SELECT MAX(EventNum) FROM EventLogTable;");
			snprintf (updateQuery, sizeof(updateQuery), "UPDATE EventLogTable SET Comment = '%s' WHERE EventNum = %s;", comment.c_str(), maxEventNum.c_str());
			ErrCode = m_DbConn.SingleTransaction (updateQuery);
		
			if(SQLITE_OK == ErrCode)
			{
				status = true;
			}
			else
				LOGERR("DB_T : Error on updating event log data with ErrCode : %d",ErrCode,0,0);
			
			break;
		}
	}

	// catch deprecated event names and comments
	if (!comment.empty() && it == m_EventNameMap.end())
	{
//		printf ("\n%sDataBaseManager::EventLogComment: deprecated eventName: %s comment: %s %s\n", KRED, eventName.c_str(), comment.c_str(), KNRM);
		
		if (eventName == "AUTHORITY CHECK CHANGED")
		{
			//Select latest "EventNum" row from "EventLogTable" table for which Identifier was set to 'AUTHORITY CHECK' 
			//so that the "comment" gets logged to appropriate row in "EventLogTable".
			snprintf (updateQuery, sizeof(updateQuery), "UPDATE EventLogTable SET Comment = '%s' WHERE EventNum = (SELECT EventNum FROM EventLogTable WHERE Identifier = 'AUTHORITY CHECK' ORDER BY EventNum DESC LIMIT 1);", comment.c_str());
			
			ErrCode = m_DbConn.SingleTransaction (updateQuery);
		
			if(SQLITE_OK == ErrCode)
			{
				status = true;
			}
			else
				LOGERR("DB_T : Error on updating event log data with ErrCode : %d",ErrCode,0,0);
		}
		else if (eventName == "PASSWORD CHANGED")
		{
			details = "Identifier:" + userName + " First Time Password Change" + ":From:N/A:To:N/A:";
        	EventLog (USER_MODIFIED, &details);
	
			maxEventNum = m_DbConn.Exec ((char*) "SELECT MAX(EventNum) FROM EventLogTable;");
			snprintf (updateQuery, sizeof(updateQuery), "UPDATE EventLogTable SET Comment = '%s' WHERE EventNum = %s;", comment.c_str(), maxEventNum.c_str());
			ErrCode = m_DbConn.SingleTransaction (updateQuery);
		
			if(SQLITE_OK == ErrCode)
			{
				status = true;
			}
			else
				LOGERR("DB_T : Error on updating event log data with ErrCode : %d",ErrCode,0,0);
		}
		else if (eventName == "UIC SOFTWARE VERSION")
		{
			EnterEventLogSWVersion (FW_INFO_UIC, &comment);
		}
	}

	return status;
}

/**************************************************************************//**
* \brief   - Writes the calibration readings into DB.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::CalibrationData()
{
	char query[MSG_SIZE] = {0x00},QueryData[CALIBRATION_READINGS],index=0;
	char *token;
	INT32 queryStatus = 0;
	string data,dateTime;
	Message message;
	CalibrationReadings readingSDO;
	Calibration		writeData;

	memcpy(&writeData,ProcessBuffer.Buffer,sizeof(writeData));

	if(ABORT_REQ != writeData.CommandId)
	{
		DeleteHistory(TABLE_CALIBRATION);
	}
	
	if(FINISH_REQ == writeData.CommandId)
	{
		GetDateAndTime(dateTime);
		sprintf(query,"INSERT INTO Calibration(FirstReading,SecondReading,DateTime) VALUES(%d,%d,'%s');",
				writeData.FirstReading,writeData.SecondReading,(char *)dateTime.c_str());
		
		m_DbConn.Exec(query, &queryStatus); 
		if(SQLITE_OK != queryStatus)
		{
			cout << "DB_T : CalibrationData : INSERT QUERY STATUS ----ErrCode from DB : " << queryStatus << endl;
		}
	}
	else if(ABORT_REQ == writeData.CommandId)
	{
		sprintf(query,"SELECT FirstReading, SecondReading FROM Calibration;");
		data = m_DbConn.Exec(query, &queryStatus);

		token = strtok((char *)data.c_str(), ",");

		if(SQLITE_OK != queryStatus)
		{
			LOGERR("DB_T : CalibrationData : ErrCode from DB : %d",queryStatus,0,0);
		}
		else
		{
			while (token != NULL)
			{
				printf("DB_T : Calibration Data ON ABORT *************** %s\n", token);
				if(index < CALIBRATION_READINGS)
				{
					QueryData[index]=atoi(token);
				}
				token = strtok(NULL, ",");
				index++;
			}
		}
		readingSDO.FirstReading=QueryData[0];
		readingSDO.SecondReading=QueryData[1];

		message.msgID = TO_SDO_TASK_CALIBRATION_READ_REQ;	
		memcpy(message.Buffer,&readingSDO,sizeof(readingSDO));
		SendMsgToMsgQ(message,SDO_MSG_Q_ID);
	}
}

/**************************************************************************//**
* \brief   - Read Active Recipe from DB.
*      		
* \param   - INT32 ActiveRecipeID,bool bIsRecipeNum
*
* \return  - UINT8
*
******************************************************************************/
UINT8 DataBaseManager::ReadActiveRecipeFromDB(INT32 ActiveRecipeID,bool bIsRecipeNum)	
{
	#ifdef DEBUG_PRINT		
		cout << "----------- DB_T: Read Active Recipe from DB ------------" << endl;
	#endif						
	INT32 queryStatus = 0;	
	string 	strActiveWeldRecipe = "", strSRActiveRecipe = "", strBransonOnlyActive ="", strSetupLimitsActive ="";
	char 	SelectQuery[MSG_SIZE] = {0x00};	
	UINT8 dwWeldRecipeStatus = 1, dwSRStatus = 1, dwReadRecipeStatus = 1;
			
	/* Read the SC, AC & PC active recipe data from DB */
	if(bIsRecipeNum)
		sprintf(SelectQuery, "SELECT * FROM WeldRecipeTable WHERE  RecipeNumber>0 AND IsActive = 1");
	else
		sprintf(SelectQuery, "SELECT * FROM WeldRecipeTable WHERE RecipeNumber = %d;",ActiveRecipeID);	

	strActiveWeldRecipe = m_DbConn.Exec(SelectQuery, &queryStatus);
	if(SQLITE_OK != queryStatus)
	{					
		LOGERR("DB_T : Get Active Recipe : ErrCode from DB : %d", queryStatus, 0, 0);			
	}	

	/* Copy the Active recipe to the context */
	if( (strlen(strActiveWeldRecipe.c_str()) > 0) )
	{
		CP->bIsActiveRecipeAvailble = true;
		dwWeldRecipeStatus = CopyActiveRecipe(ActiveRecipeSC, ActiveRecipeAC, ActiveRecipePC, strActiveWeldRecipe);
		if(dwWeldRecipeStatus != 0)
		{
			LOGERR("----------- DB_T : Failed to copy Active Recipe!!! -----------", 0, 0, 0);	
		}
	}
	else
	{
		CP->bIsActiveRecipeAvailble = false;
		LOGERR("----------- DB_T : Active Recipe data is empty in the Database!!! -----------", 0, 0, 0);		
	}

	/* Read the suspect reject active recipe data from DB */
	memset(SelectQuery, 0x00, sizeof(SelectQuery));
	if(bIsRecipeNum)
		ActiveRecipeID = CP->m_RecipeSC.GetRecipeNumber();

	sprintf(SelectQuery, "SELECT * FROM SuspectRejectLimits WHERE RecipeNumber = %d;",ActiveRecipeID);		
	strSRActiveRecipe = m_DbConn.Exec(SelectQuery, &queryStatus);
	if(SQLITE_OK != queryStatus)
	{					
		LOGERR("DB_T : Get Suspect Reject active recipe : ErrCode from DB : %d", queryStatus, 0, 0);					
	}		

	/* Copy the SR Active recipe to the context */
	if((strlen(strSRActiveRecipe.c_str()) > 0) )
	{
		dwSRStatus = CopySRActiveRecipe(ActiveRecipeSC, strSRActiveRecipe);
		if(dwSRStatus != 0)
		{
			LOGERR("----------- DB_T : Failed to copy Suspect Reject Active Recipe!!! -----------", 0, 0, 0);	
		}
	}
	else
	{
		LOGERR("----------- DB_T : Active Recipe data is empty in the Database!!! -----------", 0, 0, 0);		
	}

	/* If Active Recipe is copied successfully, returns 0 */
	if( (dwWeldRecipeStatus == 0) && (dwSRStatus == 0) ) 		
	{
		dwReadRecipeStatus = 0;
	}
	else
	{
		dwReadRecipeStatus = 1;
	}

	sprintf(SelectQuery, "SELECT * FROM BransonOnlyParameters WHERE RecipeNumber = %d;",ActiveRecipeID);		
	strBransonOnlyActive = m_DbConn.Exec(SelectQuery, &queryStatus);
	if(SQLITE_OK != queryStatus)
	{					
		LOGERR("DB_T : Get BransonOnlyParameters recipe : ErrCode from DB : %d", queryStatus, 0, 0);					
	}	
	
	sprintf(SelectQuery, "SELECT * FROM SetupLimits WHERE RecipeNumber = %d;",ActiveRecipeID);		
	strSetupLimitsActive = m_DbConn.Exec(SelectQuery, &queryStatus);
	if(SQLITE_OK != queryStatus)
	{					
		LOGERR("DB_T : Get SetupLimits recipe : ErrCode from DB : %d", queryStatus, 0, 0);					
	}	

	if(bIsRecipeNum && (strActiveWeldRecipe.size() > 0) && (strSRActiveRecipe.size() > 0) && 
			(strBransonOnlyActive.size() > 0) && (strSetupLimitsActive.size() > 0))
	{		
		vector<string> vRecipeParams,vSuspectRejectParams,vSetupLimits;
		string strProperStringFormatToStoreInDB;
		
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
		
		/* Proper string format to update in recipe zero and store in DB again  for suspect reject parameters */
		CP->StringToTokens(strSRActiveRecipe.c_str(),vSuspectRejectParams,',');
		
		for(int itr = SR_TIME;itr <= SR_FREQUENCY;itr++)
		{
			strProperStringFormatToStoreInDB.clear();
			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSuspectRejectParams[itr].c_str());
			strProperStringFormatToStoreInDB.append("'");
			vSuspectRejectParams[itr] = strProperStringFormatToStoreInDB;
		}
	
		CP->TokensToString(strSRActiveRecipe,vSuspectRejectParams,',');
		
		/* Proper string format to update in recipe zero and store in DB again  for Setup Limits parameters */
		CP->StringToTokens(strSetupLimitsActive.c_str(),vSetupLimits,',');

		for(int itr = SETUP_WELD_MODE; itr <= SETUP_FORCE_LEVEL; itr++)
		{
			strProperStringFormatToStoreInDB.clear();
			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSetupLimits[itr].c_str());
			strProperStringFormatToStoreInDB.append("'");
			vSetupLimits[itr] = strProperStringFormatToStoreInDB;
		}
		
		CP->TokensToString(strSetupLimitsActive,vSetupLimits,',');
		
		/* Setting the recipe zero */
		DBInterface::SetRecipeZero(strActiveWeldRecipe,strSRActiveRecipe,strBransonOnlyActive,strSetupLimitsActive);
	}

	return dwReadRecipeStatus;
}

/**************************************************************************//**
* \brief   - Copies active recipe to SC object
*
* \param   - WeldRecipeSC &ScWeldRecipe, WeldRecipeAC &AcWeldRecipe, WeldRecipePC &PcWeldRecipe, string &activeRecipe 
*
* \return  - UINT8 status - success or failure.
*
******************************************************************************/
UINT8 DataBaseManager::CopyActiveRecipe(WeldRecipeSC &ScWeldRecipe, WeldRecipeAC &acWeldRecipe, WeldRecipePC &pcWeldRecipe, string &activeRecipe)
{
	string token;
	stringstream ssActiveRecipe(activeRecipe);
	UINT16 found, start = 0, tokenNum = 0;
	UINT8 status = 1;	
	Message message;
	ActiveRecipe recipeSC = CP->getActiveRecipeInstance();
	PcWeldRecipe recipePC = CP->getPcWeldRecipeInstance();
	AcWeldRecipe recipeAC = CP->getAcWeldRecipeInstance();
	
	recipeSC.GetActiveRecipe(ScWeldRecipe);
	recipePC.GetWeldRecipe(pcWeldRecipe);
	recipeAC.GetWeldRecipe(acWeldRecipe);
	
	pcWeldRecipe.FrequencyLow = pcWeldRecipe.FrequencyLow / 10;
	pcWeldRecipe.FrequencyHigh = pcWeldRecipe.FrequencyHigh / 10;

	while(getline(ssActiveRecipe,token,','))	
	{													
		tokenNum++;
		if (tokenNum <= NUM_OF_ACTIVE_RECIPE_DB_DATA)
		{
			switch (tokenNum) 
			{			
			case SC_RECIPE_NUMBER:
				ScWeldRecipe.RecipeNumber = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC RecipeNumber: " << ScWeldRecipe.RecipeNumber << "\n";
#endif	
				break;

			case SC_RECIPE_NAME:
				break;

			case SC_RECIPE_REV_NUMBER:				

				ScWeldRecipe.RecipeRevNumber = atoi(token.c_str());	

				if(ScWeldRecipe.RecipeNumber == 0)
				{
					/* If we receive recipe zero then the actual recipe number will be in recipe version number
					 So replacing the version number in Recipe number and reading the version number from the DB and storing in version number*/
					ScWeldRecipe.RecipeNumber = ScWeldRecipe.RecipeRevNumber;
					/* Getting version number and update the same in local context */
					char SelectQuery[MSG_SIZE] = {0x00};
					INT32 queryStatus = 0;
					string strVersionNumber="0";
					sprintf(SelectQuery,"SELECT WeldRecipeVerNumber FROM WeldRecipeTable WHERE RecipeNumber = %d;",ScWeldRecipe.RecipeNumber);
					strVersionNumber = m_DbConn.Exec(SelectQuery, &queryStatus);
					if(SQLITE_OK != queryStatus)
					{
						LOGERR("DB_T : GetResultID : ErrCode from DB : %d, recipeNum : %d",queryStatus,ScWeldRecipe.RecipeNumber,0); 
					}
					ScWeldRecipe.RecipeRevNumber = atoi(strVersionNumber.c_str());
				}
#ifdef DEBUG_PRINT					
				cout << "SC RecipeRevNumber: " << ScWeldRecipe.RecipeRevNumber << "\n";
#endif	
				break;

			case SC_WELD_MODE:				
				ScWeldRecipe.WeldMode = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC WeldMode: " << ScWeldRecipe.WeldMode << "\n";
#endif		
				break;

			case SC_MODE_VALUE:				
				ScWeldRecipe.ModeValue = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC ModeValue: " << ScWeldRecipe.ModeValue << "\n";
#endif	
				break;

			case SC_HOLD_TIME:				
				ScWeldRecipe.HoldTime = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC HoldTime: " << ScWeldRecipe.HoldTime << "\n";
#endif		
				break;

			case SC_TRIGGER_FORCE:				
				ScWeldRecipe.TriggerForce = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC TriggerForce: " << ScWeldRecipe.TriggerForce << "\n";
#endif	
				break;

			case SC_TRIGGER_DISTANCE:				
				ScWeldRecipe.TriggerDistance = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC TriggerDistance: " << ScWeldRecipe.TriggerDistance << "\n";
#endif	
				break;

			case SC_NUM_AMP_STEPS:				
				ScWeldRecipe.NumAmplitudeSteps = atoi(token.c_str());			
#ifdef DEBUG_PRINT									
				printf("SC NumAmplitudeSteps: %u\n", ScWeldRecipe.NumAmplitudeSteps);
#endif	
				break;

			case SC_AMP_STEP_AT:				
				ScWeldRecipe.AmplitudeStepAt = atoi(token.c_str());		
#ifdef DEBUG_PRINT									
				printf("SC AmplitudeStepAt: %u\n", ScWeldRecipe.AmplitudeStepAt);
#endif
				break;

			case SC_AMP_STEP_VALUE_0:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_ZERO] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[0]: " << ScWeldRecipe.AmplitudeStepValue[INDEX_ZERO] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_1:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_ONE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[1]: " << ScWeldRecipe.AmplitudeStepValue[INDEX_ONE] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_2:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_TWO] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[2]: " << ScWeldRecipe.AmplitudeStepValue[2] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_3:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_THREE] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[3]: " << ScWeldRecipe.AmplitudeStepValue[3] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_4:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_FOUR] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[4]: " << ScWeldRecipe.AmplitudeStepValue[4] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_5:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_FIVE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[5]: " << ScWeldRecipe.AmplitudeStepValue[5] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_6:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_SIX] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[6]: " << ScWeldRecipe.AmplitudeStepValue[6] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_7:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_SEVEN] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[7]: " << ScWeldRecipe.AmplitudeStepValue[7] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_8:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_EIGHT] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[8]: " << ScWeldRecipe.AmplitudeStepValue[8] << "\n";
#endif
				break;

			case SC_AMP_STEP_VALUE_9:				
				ScWeldRecipe.AmplitudeStepValue[INDEX_NINE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStepValue[9]: " << ScWeldRecipe.AmplitudeStepValue[9] << "\n";
#endif
				break;

			case SC_AMP_STEP_0:				
				ScWeldRecipe.AmplitudeStep[INDEX_ZERO] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[0]: " << ScWeldRecipe.AmplitudeStep[0] << "\n";
#endif
				break;

			case SC_AMP_STEP_1:				
				ScWeldRecipe.AmplitudeStep[INDEX_ONE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[1]: " << ScWeldRecipe.AmplitudeStep[1] << "\n";
#endif
				break;

			case SC_AMP_STEP_2:				
				ScWeldRecipe.AmplitudeStep[INDEX_TWO] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[2]: " << ScWeldRecipe.AmplitudeStep[2] << "\n";
#endif
				break;

			case SC_AMP_STEP_3:				
				ScWeldRecipe.AmplitudeStep[INDEX_THREE] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[3]: " << ScWeldRecipe.AmplitudeStep[3] << "\n";
#endif
				break;

			case SC_AMP_STEP_4:				
				ScWeldRecipe.AmplitudeStep[INDEX_FOUR] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[4]: " << ScWeldRecipe.AmplitudeStep[4] << "\n";
#endif
				break;

			case SC_AMP_STEP_5:				
				ScWeldRecipe.AmplitudeStep[INDEX_FIVE] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[5]: " << ScWeldRecipe.AmplitudeStep[5] << "\n";
#endif
				break;

			case SC_AMP_STEP_6:				
				ScWeldRecipe.AmplitudeStep[INDEX_SIX] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[6]: " << ScWeldRecipe.AmplitudeStep[6] << "\n";
#endif
				break;

			case SC_AMP_STEP_7:				
				ScWeldRecipe.AmplitudeStep[INDEX_SEVEN] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[7]: " << ScWeldRecipe.AmplitudeStep[7] << "\n";
#endif
				break;

			case SC_AMP_STEP_8:				
				ScWeldRecipe.AmplitudeStep[INDEX_EIGHT] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[8]: " << ScWeldRecipe.AmplitudeStep[8] << "\n";
#endif
				break;

			case SC_AMP_STEP_9:				
				ScWeldRecipe.AmplitudeStep[INDEX_NINE] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AmplitudeStep[9]: " << ScWeldRecipe.AmplitudeStep[9] << "\n";
#endif
				break;

			case AC_WELD_FORCE:				
				acWeldRecipe.ForceStepForce[INDEX_ZERO] = atoi(token.c_str());
				acWeldRecipe.WeldForce = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC WeldForce: " << acWeldRecipe.WeldForce << "\n";
#endif
				break;

			case AC_FORCE_RAMP_TIME:				
				acWeldRecipe.ForceRampTime = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC ForceRampTime: " << acWeldRecipe.ForceRampTime << "\n";
#endif
				break;

			case AC_TOTAL_COLLAPSE_TARGET:				
				acWeldRecipe.TotalCollapseTarget = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC TotalCollapseTarget: " << acWeldRecipe.TotalCollapseTarget << "\n";
#endif
				break;

			case AC_HOLD_FORCE:				
				acWeldRecipe.HoldForce = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC HoldForce: " << acWeldRecipe.HoldForce << "\n";
#endif
				break;

			case AC_HOLD_FORCE_RAMP_TIME:				
				acWeldRecipe.HoldForceRampTime = atoi(token.c_str());		
#ifdef DEBUG_PRINT					
				cout << "AC HoldForceRampTime: " << acWeldRecipe.HoldForceRampTime << "\n";
#endif
				break;

			case AC_EXPECTED_PC_POSITION:				
				acWeldRecipe.ExpectedPartContactPosition = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ExpectedPartContactPosition: " << acWeldRecipe.ExpectedPartContactPosition << "\n";
#endif
				break;

			case AC_READY_POSITION:				
				acWeldRecipe.ReadyPosition = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ReadyPosition: " << acWeldRecipe.ReadyPosition << "\n";
#endif
				break;

			case AC_DOWN_ACCELERATION:				
				acWeldRecipe.DownAcceleration = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC DownAcceleration: " << acWeldRecipe.DownAcceleration << "\n";
#endif
				break;

			case AC_DOWN_MAX_VELOCITY:				
				acWeldRecipe.DownMaxVelocity = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC DownMaxVelocity: " << acWeldRecipe.DownMaxVelocity << "\n";
#endif
				break;

			case AC_DOWN_DECELERATION:				
				acWeldRecipe.DownDeceleration = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC DownDeceleration: " << acWeldRecipe.DownDeceleration << "\n";
#endif
				break;

			case AC_RETURN_ACCELERATION:				
				acWeldRecipe.ReturnAcceleration = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ReturnAcceleration: " << acWeldRecipe.ReturnAcceleration << "\n";
#endif
				break;

			case AC_RETURN_MAX_VELOCITY:				
				acWeldRecipe.ReturnMaxVelocity = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC ReturnMaxVelocity: " << acWeldRecipe.ReturnMaxVelocity << "\n";
#endif
				break;

			case AC_RETURN_DECELERATION:				
				acWeldRecipe.ReturnDeceleration = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ReturnDeceleration: " << acWeldRecipe.ReturnDeceleration << "\n";
#endif
				break;

			case SC_NUM_FORCE_STEPS:				
				ScWeldRecipe.NumForceSteps = atoi(token.c_str());				
#ifdef DEBUG_PRINT									
				printf("SC NumForceSteps: %u\n", ScWeldRecipe.NumForceSteps);
#endif
				break;

			case SC_FORCE_STEP_AT:				
				ScWeldRecipe.ForceStepAt = atoi(token.c_str());	
#ifdef DEBUG_PRINT									
				printf("SC ForceStepAt: %u\n", ScWeldRecipe.ForceStepAt);
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_0:				
				ScWeldRecipe.ForceStepAtValue[INDEX_ZERO] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[0]: " << ScWeldRecipe.ForceStepAtValue[0] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_1:				
				ScWeldRecipe.ForceStepAtValue[INDEX_ONE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[1]: " << ScWeldRecipe.ForceStepAtValue[1] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_2:				
				ScWeldRecipe.ForceStepAtValue[INDEX_TWO] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[2]: " << ScWeldRecipe.ForceStepAtValue[2] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_3:				
				ScWeldRecipe.ForceStepAtValue[INDEX_THREE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[3]: " << ScWeldRecipe.ForceStepAtValue[3] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_4:				
				ScWeldRecipe.ForceStepAtValue[INDEX_FOUR] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[4]: " << ScWeldRecipe.ForceStepAtValue[4] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_5:				
				ScWeldRecipe.ForceStepAtValue[INDEX_FIVE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[5]: " << ScWeldRecipe.ForceStepAtValue[5] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_6:				
				ScWeldRecipe.ForceStepAtValue[INDEX_SIX] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[6]: " << ScWeldRecipe.ForceStepAtValue[6] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_7:				
				ScWeldRecipe.ForceStepAtValue[INDEX_SEVEN] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[7]: " << ScWeldRecipe.ForceStepAtValue[7] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_8:				
				ScWeldRecipe.ForceStepAtValue[INDEX_EIGHT] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[8]: " << ScWeldRecipe.ForceStepAtValue[8] << "\n";
#endif
				break;

			case SC_FORCE_STEP_AT_VALUE_9:				
				ScWeldRecipe.ForceStepAtValue[INDEX_NINE] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC ForceStepAtValue[9]: " << ScWeldRecipe.ForceStepAtValue[9] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_0:				
				acWeldRecipe.ForceStepRampTime[INDEX_ZERO] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[0]: " << acWeldRecipe.ForceStepRampTime[0] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_1:				
				acWeldRecipe.ForceStepRampTime[INDEX_ONE] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[1]: " << acWeldRecipe.ForceStepRampTime[1] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_2:				
				acWeldRecipe.ForceStepRampTime[INDEX_TWO] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[2]: " << acWeldRecipe.ForceStepRampTime[2] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_3:				
				acWeldRecipe.ForceStepRampTime[INDEX_THREE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[3]: " << acWeldRecipe.ForceStepRampTime[3] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_4:				
				acWeldRecipe.ForceStepRampTime[INDEX_FOUR] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[4]: " << acWeldRecipe.ForceStepRampTime[4] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_5:				
				acWeldRecipe.ForceStepRampTime[INDEX_FIVE] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[5]: " << acWeldRecipe.ForceStepRampTime[5] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_6:				
				acWeldRecipe.ForceStepRampTime[INDEX_SIX] = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[6]: " << acWeldRecipe.ForceStepRampTime[6] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_7:				
				acWeldRecipe.ForceStepRampTime[INDEX_SEVEN] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[7]: " << acWeldRecipe.ForceStepRampTime[7] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_8:				
				acWeldRecipe.ForceStepRampTime[INDEX_EIGHT] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[8]: " << acWeldRecipe.ForceStepRampTime[8] << "\n";
#endif
				break;

			case AC_FORCE_STEP_RAMP_TIME_9:				
				acWeldRecipe.ForceStepRampTime[INDEX_NINE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepRampTime[9]: " << acWeldRecipe.ForceStepRampTime[9] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_0:				
				acWeldRecipe.ForceStepForce[INDEX_ZERO] = atoi(token.c_str());
				acWeldRecipe.WeldForce = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[0]: " << acWeldRecipe.ForceStepForce[0] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_1:				
				acWeldRecipe.ForceStepForce[INDEX_ONE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[1]: " << acWeldRecipe.ForceStepForce[1] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_2:				
				acWeldRecipe.ForceStepForce[INDEX_TWO] = atoi(token.c_str());		
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[2]: " << acWeldRecipe.ForceStepForce[2] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_3:				
				acWeldRecipe.ForceStepForce[INDEX_THREE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[3]: " << acWeldRecipe.ForceStepForce[3] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_4:				
				acWeldRecipe.ForceStepForce[INDEX_FOUR] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[4]: " << acWeldRecipe.ForceStepForce[4] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_5:				
				acWeldRecipe.ForceStepForce[INDEX_FIVE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[5]: " << acWeldRecipe.ForceStepForce[5] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_6:				
				acWeldRecipe.ForceStepForce[INDEX_SIX] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[6]: " << acWeldRecipe.ForceStepForce[6] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_7:				
				acWeldRecipe.ForceStepForce[INDEX_SEVEN] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[7]: " << acWeldRecipe.ForceStepForce[7] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_8:				
				acWeldRecipe.ForceStepForce[INDEX_EIGHT] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[8]: " << acWeldRecipe.ForceStepForce[8] << "\n";
#endif
				break;

			case AC_FORCE_STEP_FORCE_9:				
				acWeldRecipe.ForceStepForce[INDEX_NINE] = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC ForceStepForce[9]: " << acWeldRecipe.ForceStepForce[9] << "\n";
#endif
				break;

			case PC_WELD_RAMP_TIME:				
				pcWeldRecipe.WeldRampTime = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "PC WeldRampTime: " << pcWeldRecipe.WeldRampTime << "\n";
#endif
				break;

			case SC_AFTER_BURST_ENABLED:				
				ScWeldRecipe.AfterBurst.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT									
				printf("SC AfterBurst.bIsEnabled: %u\n", ScWeldRecipe.AfterBurst.bIsEnabled);
#endif
				break;

			case SC_AFTER_BURST_DELAY:				
				ScWeldRecipe.AfterBurst.BurstDelay = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AfterBurst.BurstDelay: " << ScWeldRecipe.AfterBurst.BurstDelay << "\n";
#endif
				break;

			case SC_AFTER_BURST_TIME:				
				ScWeldRecipe.AfterBurst.BurstTime = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC AfterBurst.BurstTime: " << ScWeldRecipe.AfterBurst.BurstTime << "\n";
#endif								
				break;

			case SC_AFTER_BURST_AMP:				
				ScWeldRecipe.AfterBurst.BurstAmplitude = atoi(token.c_str());
#ifdef DEBUG_PRINT									
				printf("SC AfterBurst.BurstAmplitude: %u\n", ScWeldRecipe.AfterBurst.BurstAmplitude);
#endif				
				break;

			case SC_PRETRIGGER_ENABLED:				
				ScWeldRecipe.PreTrigger.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT									
				printf("SC PreTrigger.bIsEnabled: %u\n", ScWeldRecipe.PreTrigger.bIsEnabled);
#endif
				break;
																				
			case SC_PRETRIGGER_AUTO_ENABLED:					
				ScWeldRecipe.PreTrigger.bIsAutoEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT									
				printf("SC PreTrigger.bIsAutoEnabled: %u\n", ScWeldRecipe.PreTrigger.bIsAutoEnabled);
#endif
				break;

			case SC_PRETRIGGER_DISTANCE_ENABLED:					
				ScWeldRecipe.PreTrigger.bIsDistanceEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT									
				printf("SC PreTrigger.bIsDistanceEnabled: %u\n", ScWeldRecipe.PreTrigger.bIsDistanceEnabled);
#endif
				break;								
											
			case SC_PRETRIGGER_AMP:				
				ScWeldRecipe.PreTrigger.TriggerAmplitude = atoi(token.c_str());
#ifdef DEBUG_PRINT									
				printf("SC PreTrigger.TriggerAmplitude: %u\n", ScWeldRecipe.PreTrigger.TriggerAmplitude);
#endif
				break;

			case SC_PRETRIGGER_DISTANCE:				
				ScWeldRecipe.PreTrigger.PreTriggerDistance = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC PreTrigger.PreTriggerDistance: " << ScWeldRecipe.PreTrigger.PreTriggerDistance << "\n";
#endif		
				break;
																							
			case SC_ENERGY_BRAKE_ENABLED:
				ScWeldRecipe.EnergyBrake.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC EnergyBrake.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ENERGY_BRAKE_TIME:
				ScWeldRecipe.EnergyBrake.EnergyBrakeTime = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC EnergyBrake.Time: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ENERGY_BRAKE_AMP:
				ScWeldRecipe.EnergyBrake.EnergyBrakeAmplitude = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC EnergyBrake.Amplitude: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_COOLING_VALVE:				
				ScWeldRecipe.bIsCoolingValveEnabled = atoi(token.c_str());	
#ifdef DEBUG_PRINT									
				printf("SC bIsCoolingVavleEnabled: %u\n", ScWeldRecipe.bIsCoolingValveEnabled);
#endif
				break;

			case SC_MAX_WELD_TIMEOUT:				
				ScWeldRecipe.MaxWeldTimeout = atoi(token.c_str());	
#ifdef DEBUG_PRINT					
				cout << "SC MaxWeldTimeout: " << ScWeldRecipe.MaxWeldTimeout << "\n";
#endif
				break;
				
			case SC_TIMED_SEEK_ENABLED:
				ScWeldRecipe.bIsTimedSeekEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC TimedSeek: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_PRE_WELD_SEEK_ENABLED:
				ScWeldRecipe.bIsPreWeldSeekEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC PreWeldSeek: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_POST_WELD_SEEK_ENABLED:
				ScWeldRecipe.bIsPostWeldSeekEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC PostWeldSeek: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_TIMED_SEEK_PERIOD:
				ScWeldRecipe.TimedSeekPeriod = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC TimedSeekPeriod: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case AC_PC_WINDOW_MINUS:
				acWeldRecipe.PartContactWindowMinus = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC PartContactWindowMinus: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case AC_PC_WINDOW_PLUS:
				acWeldRecipe.PartContactWindowPlus = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC PartContactWindowPlus: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case AC_PC_WINDOW_OFFSET:
				acWeldRecipe.ExpectedPartContactOffset = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "AC PartContactWindowOffset: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_GLOBAL_CONTROL:
#ifdef DEBUG_PRINT					
				cout << "SC GlobalControl: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_PEAK_POWER_CUTOFF:
				ScWeldRecipe.CutoffLimit.PeakPowerCutoff.Value = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC PeakPowerCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_PEAK_POWER_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.PeakPowerCutoff.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC PeakPowerCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_LOW_CUTOFF:
				ScWeldRecipe.CutoffLimit.FrequencyLowCutoff.Value = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC FrequencyLowCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_LOW_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.FrequencyLowCutoff.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC FrequencyLowCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ABSOLUTE_CUTOFF:
				ScWeldRecipe.CutoffLimit.AbsoluteDistanceCutoff.Value = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AbsoluteCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;
				
			case SC_ABSOLUTE_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.AbsoluteDistanceCutoff.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC AbsoluteCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_HIGH_CUTOFF:
				ScWeldRecipe.CutoffLimit.FrequencyHighCutoff.Value = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC FrequencyHighCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_FREQUENCY_HIGH_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.FrequencyHighCutoff.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC FrequencyHighCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_COLLAPSE_CUTOFF:
				ScWeldRecipe.CutoffLimit.CollapseDistanceCutoff.Value = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC CollapseCutoff: " << atoi(token.c_str())<< "\n";
#endif		
				break;

			case SC_COLLAPSE_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.CollapseDistanceCutoff.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC CollapseCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
								
			case SC_ENERGY_CUTOFF:
				ScWeldRecipe.CutoffLimit.EnergyCutoff.Value = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC EnergyCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_ENERGY_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.EnergyCutoff.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC EnergyCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_TIME_CUTOFF:
				ScWeldRecipe.CutoffLimit.TimeCutoff.Value = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC TimeCutoff: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			case SC_TIME_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.TimeCutoff.bIsEnabled = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC TimeCutoff.bIsEnabled: " << atoi(token.c_str()) << "\n";
#endif		
				break;
			
			case SC_GROUND_DETECT_CUTOFF_ENABLE:
				ScWeldRecipe.CutoffLimit.GroundDetectCutoff = atoi(token.c_str());
#ifdef DEBUG_PRINT					
				cout << "SC GroundDetectCutoffEnable: " << atoi(token.c_str()) << "\n";
#endif		
				break;

			/* The next 4 parameters are specific to UI only, hence not taken for SC BL */
			/*UI PARAMS BEGING*/
			case SC_IS_ACTIVE:
			case SC_IS_LOCKED:
			case SC_COMPANY_NAME:
			case SC_IS_VALIDATE:
			/*UI PARAMS END*/
			case AC_READY_POS_TOGGLE:
				acWeldRecipe.ReadyPositionToggle = atoi(token.c_str());
				break;
			case AC_WELD_FORCE_CONTROL:
				acWeldRecipe.WeldForceControl = atoi(token.c_str());
				break;
			case SC_REACTIVITY:
				ScWeldRecipe.DynamicModeParam.Reactivity = atoi(token.c_str());
				break;
			case SC_FORCE_LEVEL_ENABLE:
				ScWeldRecipe.DynamicModeParam.IsForceLevelEnabled = atoi(token.c_str());
				break;
			case SC_FORCE_LEVEL:
				ScWeldRecipe.DynamicModeParam.ForceLevel = atoi(token.c_str());
				break;
			case SC_FORCE_LEVEL_TIME:
				ScWeldRecipe.DynamicModeParam.ForceLevelTime = atoi(token.c_str());
				break;
			case SC_SCRUB_AMPLITUDE:
				ScWeldRecipe.ScrubAmplitude = atoi(token.c_str());
				break;
			default:
				LOGDBG("DB_T: Recipe parameter not found\n", 0, 0, 0);		
				break;
				
			}		/* end of switch */
		}			/* end of if */
		else 
		{ 
			LOGDBG("DB_T: Could not find one of the recipe parameters, breaks out of while loop\n", 0, 0, 0);				
			break;
		}  														
	}		/* end of while */
	
	//Check if all the parameters were updated with values
	if(NUM_OF_ACTIVE_RECIPE_DB_DATA == tokenNum)
	{	
		/* Send SC Structure to User Interface to make it active*/
		message.msgID = TO_UI_TASK_WELD_RECIPE_SC;
		memset(message.Buffer,0x00,sizeof(message.Buffer));
		memcpy(message.Buffer,&ScWeldRecipe,sizeof(ScWeldRecipe));

		if(msgQSend(UI_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			LOGERR("Recipe : Message send failed to MSG Q",0,0,0);
		}	
		
		/* Send PC + AC Structure to User Interface to make it active */
		message.msgID = TO_UI_TASK_WELD_RECIPE;
		memset(message.Buffer,0x00,sizeof(message.Buffer));
		memcpy(&message.Buffer[0],&pcWeldRecipe,sizeof(pcWeldRecipe));
		memcpy(&message.Buffer[sizeof(pcWeldRecipe)],&acWeldRecipe,sizeof(acWeldRecipe));

		if(msgQSend(UI_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			LOGERR("Recipe : Message send failed to MSG Q",0,0,0);
		}
		//1ms delay so the messages get time to be processed 
		MsDelay(1);
		
		status = 0;
	}
	else
	{
		LOGERR("DB_T: -------------- Failed to get all the recipe data!!! --------------\n", 0, 0, 0);
		status = 1;
	}

	return status;	
}

/**************************************************************************//**
* \brief   - Copies Suspect Reject active recipe to SC object
*
* \param   - WeldRecipeSC &ScWeldRecipe, string &SRActiveRecipe 
*
* \return  - UINT8 status - success or failure.
*
******************************************************************************/
UINT8 DataBaseManager::CopySRActiveRecipe(WeldRecipeSC &ScWeldRecipe, string &SRActiveRecipe)
{																	
	UINT16 weldRecipeIdx;										
	vector<UINT32> SRWeldRecipe;							
	string SusRejRecipe, token;								
	stringstream SRActiveRecipeSS(SRActiveRecipe);			/* Parse it from the string */	
	UINT32 tokenNum = 0, recipeValue = 0;
	UINT8 status = 1;
	SRWeldRecipe.clear();	
	Message message;
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

	/* Write the data to the Weld Recipe structure. */
	for (weldRecipeIdx = 0; weldRecipeIdx < SRWeldRecipe.size(); weldRecipeIdx++)
	{
#ifdef DEBUG_PRINT	
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
#ifdef DEBUG_PRINT	
	cout << endl;		
#endif	

	if(SRWeldRecipe.size() == NUM_OF_SUS_REJ_RECIPE_DB_DATA)	
	{
		status = 0;
		LOGDBG("DB_T : Suspect Reject active recipe read from DB", 0, 0, 0);		
	}
	else
	{
		status = 1;
		LOGERR("DB_T : Failed to read the Suspect Reject active recipe from DB", 0, 0, 0);		
	}

	/* Send SC Structure to User Interface to make it active*/
	message.msgID = TO_UI_TASK_WELD_RECIPE_SC;
	memset(message.Buffer,0x00,sizeof(message.Buffer));
	memcpy(message.Buffer,&ScWeldRecipe,sizeof(ScWeldRecipe));

	if(msgQSend(UI_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
	{
		LOGERR("Recipe : Message send failed to MSG Q",0,0,0);
	}	
	
	return status;			
}	


/**************************************************************************//**
* \brief   - Set the active recipe read from DB to the BL Current context
*
* \param   - None 
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::SetActiveRecipe()
{		
	ActiveRecipe SC = CP->getActiveRecipeInstance();
	PcWeldRecipe PC = CP->getPcWeldRecipeInstance();
	AcWeldRecipe AC = CP->getAcWeldRecipeInstance();

	SC.SetActiveRecipe(ActiveRecipeSC);			
	CP->UpdateActiveRecipe(SC);

	PC.SetWeldRecipe(ActiveRecipePC);
	CP->UpdatePcWeldRecipe(PC);	

	AC.SetWeldRecipe(ActiveRecipeAC);
	CP->UpdateAcWeldRecipe(AC);
}

/**************************************************************************//**
* \brief   - Checks seek recipe availability in DB. If not then writes default 
* 			 values into SeekRecipeTable in DB based on the system frequency.
* 			 
* \param   - None 
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ConfigureDefaultSeekRecipe()
{
	INT32 	ErrCode 		= 0;
	char 	query[MSG_SIZE] = {0x00};
	string 	insertQuery;
	string 	rowCount;
	
	sprintf(query,"SELECT COUNT(1) FROM SeekRecipeTable;");
	rowCount = m_DbConn.Exec(query, &ErrCode);
	if(SQLITE_OK == ErrCode)
	{
		if(0 == atoi(rowCount.c_str()))
		{
			memset(query,0x00,sizeof(query));
			sprintf(query,"INSERT INTO SeekRecipeTable(psFreq,SeekTime,SeekRampTime,SeekFreqOffset,SeekMemClearBeforeSeek,SeekAmplitudeLoopC1,SeekAmplitudeLoopC2,SeekFreqLimitHigh,SeekFreqLimitLow,SeekPhaseloopI,SeekPhaseloopCF) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);",
					20000,SEEK_TIME_DEFAULT,SEEK_RAMPTIME_DEFAULT,SEEK_FREQUENCY_OFFSET_20KDEFAULT,0,AMPLOOPC1_20KDEFAULT,AMPLOOPC2_20KDEFAULT,
					FREQENCYHIGH_20KDEFAULT,FREQENCYLOW_20KDEFAULT,PLOOP_20KDEFAULT,PLOOPCF_20KDEFAULT);

			insertQuery.append((const char*)query);	/* 20 KHz recipe */

			memset(query,0x00,sizeof(query));
			sprintf(query,"INSERT INTO SeekRecipeTable(psFreq,SeekTime,SeekRampTime,SeekFreqOffset,SeekMemClearBeforeSeek,SeekAmplitudeLoopC1,SeekAmplitudeLoopC2,SeekFreqLimitHigh,SeekFreqLimitLow,SeekPhaseloopI,SeekPhaseloopCF) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);",
					30000,SEEK_TIME_DEFAULT,SEEK_RAMPTIME_DEFAULT,SEEK_FREQUENCY_OFFSET_30KDEFAULT,0,AMPLOOPC1_30KDEFAULT,AMPLOOPC2_30KDEFAULT,
					FREQENCYHIGH_30KDEFAULT,FREQENCYLOW_30KDEFAULT,PLOOP_30KDEFAULT,PLOOPCF_30KDEFAULT);

			insertQuery.append((const char*)query);	/* 30 KHz recipe */

			memset(query,0x00,sizeof(query));
			sprintf(query,"INSERT INTO SeekRecipeTable(psFreq,SeekTime,SeekRampTime,SeekFreqOffset,SeekMemClearBeforeSeek,SeekAmplitudeLoopC1,SeekAmplitudeLoopC2,SeekFreqLimitHigh,SeekFreqLimitLow,SeekPhaseloopI,SeekPhaseloopCF) VALUES(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);",
					40000,SEEK_TIME_DEFAULT,SEEK_RAMPTIME_DEFAULT,SEEK_FREQUENCY_OFFSET_40KDEFAULT,0,AMPLOOPC1_40KDEFAULT,AMPLOOPC2_40KDEFAULT,
					FREQENCYHIGH_40KDEFAULT,FREQENCYLOW_40KDEFAULT,PLOOP_40KDEFAULT,PLOOPCF_40KDEFAULT);

			insertQuery.append((const char*)query);	/* 40 KHz recipe */
			
			ErrCode = m_DbConn.BulkTransaction((char *)insertQuery.c_str());
			if(SQLITE_OK != ErrCode)
			{
				LOGERR("DB_T : Configure default recipe failed with Write::ErrCode : %d",ErrCode,0,0);
			}
		}
		else
		{
			LOGDBG("DB_T : Recipe already available, Need not to configure with default recipe",0,0,0);
		}
	}
	else
	{
		LOGERR("DB_T : Configure default recipe failed with Read::ErrCode : %d",ErrCode,0,0);
	}
}

/**************************************************************************//**
* \brief   - Reads seek recipe from data base
*
* \param   - None 
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ReadSeekRecipe()
{
	vector<INT32> 	recipe;
	seekRecipe		seek_Recipe;
	UINT8 			status 		= 0;
	/* If we are adding extra column to read in below selectQuery then please adjust this count as per no.of column */
	UINT8 			columnCount = SEEK_RECIPE_COLUMN_COUNT;
	UINT16 			freqType	= 0;
	UINT8 			idx			= 0;
	char selectQuery[MSG_SIZE] 	= {0x00};
	
	/* Configures default recipe based on system frequency type before read DB - If DB has configured recipe then will get over write by read 
	 * recipe from DB */
	switch(CP->powerupRecipe.Frequency)
	{
		case FREQ_20KHZ:
			seek_Recipe.SeekAmplitudeLoopC1 	= AMPLOOPC1_20KDEFAULT;
			seek_Recipe.SeekAmplitudeLoopC2		= AMPLOOPC2_20KDEFAULT;
			seek_Recipe.SeekFreqLimitHigh		= FREQENCYHIGH_20KDEFAULT;
			seek_Recipe.SeekFreqLimitLow		= FREQENCYLOW_20KDEFAULT;
			seek_Recipe.SeekPhaseloopI			= PLOOP_20KDEFAULT;
			seek_Recipe.SeekPhaseloopCF			= PLOOPCF_20KDEFAULT;
			seek_Recipe.SeekRampTime			= SEEK_RAMPTIME_DEFAULT;
			seek_Recipe.SeekFreqOffset			= SEEK_FREQUENCY_OFFSET_20KDEFAULT;
			seek_Recipe.SeekMemClearBeforeSeek  = SEEK_MEMORY_CLEAR;
			seek_Recipe.SeekTime				= SEEK_TIME_DEFAULT;
			freqType							= FREQ_20KHZ * 1000; 
			break;
	
		case FREQ_30KHZ:
			seek_Recipe.SeekAmplitudeLoopC1 	= AMPLOOPC1_30KDEFAULT;
			seek_Recipe.SeekAmplitudeLoopC2		= AMPLOOPC2_30KDEFAULT;
			seek_Recipe.SeekFreqLimitHigh		= FREQENCYHIGH_30KDEFAULT;
			seek_Recipe.SeekFreqLimitLow		= FREQENCYLOW_30KDEFAULT;
			seek_Recipe.SeekPhaseloopI			= PLOOP_30KDEFAULT;
			seek_Recipe.SeekPhaseloopCF			= PLOOPCF_30KDEFAULT;
			seek_Recipe.SeekRampTime			= SEEK_RAMPTIME_DEFAULT;
			seek_Recipe.SeekFreqOffset			= SEEK_FREQUENCY_OFFSET_30KDEFAULT;
			seek_Recipe.SeekMemClearBeforeSeek  = SEEK_MEMORY_CLEAR;
			seek_Recipe.SeekTime				= SEEK_TIME_DEFAULT;
			freqType							= FREQ_30KHZ * 1000;
			break;
	
		case FREQ_40KHZ:
			seek_Recipe.SeekAmplitudeLoopC1 	= AMPLOOPC1_40KDEFAULT;
			seek_Recipe.SeekAmplitudeLoopC2		= AMPLOOPC2_40KDEFAULT;
			seek_Recipe.SeekFreqLimitHigh		= FREQENCYHIGH_40KDEFAULT;
			seek_Recipe.SeekFreqLimitLow		= FREQENCYLOW_40KDEFAULT;
			seek_Recipe.SeekPhaseloopI			= PLOOP_40KDEFAULT;
			seek_Recipe.SeekPhaseloopCF			= PLOOPCF_40KDEFAULT;
			seek_Recipe.SeekRampTime			= SEEK_RAMPTIME_DEFAULT;
			seek_Recipe.SeekFreqOffset			= SEEK_FREQUENCY_OFFSET_40KDEFAULT;
			seek_Recipe.SeekMemClearBeforeSeek  = SEEK_MEMORY_CLEAR;
			seek_Recipe.SeekTime				= SEEK_TIME_DEFAULT;
			freqType							= FREQ_40KHZ * 1000;
			break;

		/* Default case never comes, Because RTP_SC main will ensure any of one system frequency type */
		default:
			seek_Recipe.SeekAmplitudeLoopC1 	= AMPLOOPC1_20KDEFAULT;
			seek_Recipe.SeekAmplitudeLoopC2		= AMPLOOPC2_20KDEFAULT;
			seek_Recipe.SeekFreqLimitHigh		= FREQENCYHIGH_20KDEFAULT;
			seek_Recipe.SeekFreqLimitLow		= FREQENCYLOW_20KDEFAULT;
			seek_Recipe.SeekPhaseloopI			= PLOOP_20KDEFAULT;
			seek_Recipe.SeekPhaseloopCF			= PLOOPCF_20KDEFAULT;
			seek_Recipe.SeekRampTime			= SEEK_RAMPTIME_DEFAULT;
			seek_Recipe.SeekFreqOffset			= SEEK_FREQUENCY_OFFSET_20KDEFAULT;
			seek_Recipe.SeekMemClearBeforeSeek  = SEEK_MEMORY_CLEAR;
			seek_Recipe.SeekTime				= SEEK_TIME_DEFAULT;
			freqType							= FREQ_20KHZ * 1000;
			LOGERR("Invalid system configuration on configuring seek recipe for power up seek operation",0,0,0);
			break;
	}
	
	sprintf(selectQuery,"SELECT SeekTime,SeekRampTime,SeekFreqOffset,SeekAmplitudeLoopC1,SeekAmplitudeLoopC2,SeekFreqLimitHigh,SeekFreqLimitLow,SeekPhaseloopI,SeekPhaseloopCF,SeekMemClearBeforeSeek FROM SeekRecipeTable WHERE psFreq = %d LIMIT 1;",freqType);
	
	status = m_DbConn.ReadRecipe((const char*)selectQuery,columnCount,recipe);
	
	if(0 != status)
	{
		seek_Recipe.SeekTime				= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekRampTime 			= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekFreqOffset 			= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekAmplitudeLoopC1 	= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekAmplitudeLoopC2 	= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekFreqLimitHigh 		= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekFreqLimitLow 		= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekPhaseloopI 			= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekPhaseloopCF 		= recipe[idx];
		idx = idx + 1;
		seek_Recipe.SeekMemClearBeforeSeek	= recipe[idx];
	}
	else
	{
		LOGERR("Seek recipe read from DB failed hence assigned default values",0,0,0);
	}
	
	ProcessBuffer.msgID = TO_UI_TASK_SEEK_RECIPE;
	memset(ProcessBuffer.Buffer,0x00,sizeof(ProcessBuffer.Buffer));
	memcpy(ProcessBuffer.Buffer,&seek_Recipe,sizeof(seek_Recipe));
	SendMsgToMsgQ(ProcessBuffer,UI_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Reads scan recipe from data base
*
* \param   - None 
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ReadScanRecipe()
{
	vector<INT32> 	recipe;
	ScanRecipe		scan_Recipe;
	UINT8 			status 		= 0;
	/* If we are adding extra column to read in below selectQuery then please adjust this count as per no.of column */
	UINT8 			columnCount = SCAN_RECIPE_COLUMN_COUNT;
	UINT16 			freqType	= 0;
	UINT8 			idx			= 0;
	char selectQuery[MSG_SIZE] 	= {0x00};
	
	/* Configures default recipe based on system frequency type before read DB - If DB has configured recipe then will get over write by read 
	 * recipe from DB */
	scan_Recipe.FrequencyStep 			= SCAN_FREQUENCY_STEP_DEFAULT;
	scan_Recipe.MaxAmplitude  			= SCAN_MAX_AMPLITUDE_DEFAULT;
	scan_Recipe.MaxCurrent				= SCAN_MAX_CURRENT_DEFAULT;
	scan_Recipe.TimeDelay				= SCAN_TIME_DELAY_DEFAULT;
	scan_Recipe.SetDigTuneWithHornScan	= SCAN_DIGITALTUNE_DEFAULT;
	switch(CP->powerupRecipe.Frequency)
	{
		case FREQ_20KHZ:
			scan_Recipe.FrequencyStart 	= START_FREQUENCY_20KDEFAULT;
			scan_Recipe.FrequencyStop	= STOP_FREQUENCY_20KDEFAULT;
			freqType					= FREQ_20KHZ * 1000;
			break;
	
		case FREQ_30KHZ:
			scan_Recipe.FrequencyStart 	= START_FREQUENCY_30KDEFAULT;
			scan_Recipe.FrequencyStop	= STOP_FREQUENCY_30KDEFAULT;
			freqType					= FREQ_30KHZ * 1000;
			break;
	
		case FREQ_40KHZ:
			scan_Recipe.FrequencyStart 	= START_FREQUENCY_40KDEFAULT;
			scan_Recipe.FrequencyStop	= STOP_FREQUENCY_40KDEFAULT;
			freqType					= FREQ_40KHZ * 1000;
			break;

		/* Default case never comes, Because RTP_SC main will ensure any of one system frequency type */
		default:
			scan_Recipe.FrequencyStart 	= START_FREQUENCY_20KDEFAULT;
			scan_Recipe.FrequencyStop	= STOP_FREQUENCY_20KDEFAULT;
			freqType					= FREQ_20KHZ * 1000;
			LOGERR("Invalid system configuration on configuring scan recipe for power up scan operation",0,0,0);
			break;
	}
	
	sprintf(selectQuery,"SELECT StepFrequency,maxAmplitude,maxCurrent,SetDigTuneWithHornScan,StartFrequency,StopFrequency,TimeDelay FROM HornRecipeTable WHERE Psfreq = %d LIMIT 1;",freqType);
	
	status = m_DbConn.ReadRecipe((const char*)selectQuery,columnCount,recipe);
	
	if(0 != status)
	{
		scan_Recipe.FrequencyStep			= recipe[idx];
		idx = idx + 1;
		scan_Recipe.MaxAmplitude 			= recipe[idx];
		idx = idx + 1;
		scan_Recipe.MaxCurrent	 			= recipe[idx];
		idx = idx + 1;
		scan_Recipe.SetDigTuneWithHornScan 	= recipe[idx];
		idx = idx + 1;
		scan_Recipe.FrequencyStart		 	= recipe[idx];
		idx = idx + 1;
		scan_Recipe.FrequencyStop	 		= recipe[idx];
		idx = idx + 1;
		scan_Recipe.TimeDelay		 		= recipe[idx];
	}
	else
	{
		LOGERR("Scan recipe read from DB failed hence assigned default values",0,0,0);
	}
	
	ProcessBuffer.msgID = TO_UI_TASK_HORN_SCAN_RECIPE;
	memset(ProcessBuffer.Buffer,0x00,sizeof(ProcessBuffer.Buffer));
	memcpy(ProcessBuffer.Buffer,&scan_Recipe,sizeof(scan_Recipe));
	SendMsgToMsgQ(ProcessBuffer,UI_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Reads test recipe from data base
*
* \param   - None 
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ReadTestRecipe()
{
	vector<INT32> 	recipe;
	testRecipe		test_Recipe;
	UINT8 			status 		= 0;
	/* If we are adding extra column to read in below selectQuery then please adjust this count as per no.of column */
	UINT8 			columnCount = TEST_RECIPE_COLUMN_COUNT;
	UINT16 			freqType	= 0;
	UINT8 			idx			= 0;
	char selectQuery[MSG_SIZE] 	= {0x00};
	
	/* Configures default recipe based on system frequency type before read DB - If DB has configured recipe then will get over write by read 
	 * recipe from DB */
	switch(CP->powerupRecipe.Frequency)
	{
		case FREQ_20KHZ:
			test_Recipe.AmplitudeLoopC1 = AMPLOOPC1_20KDEFAULT;
			test_Recipe.AmplitudeLoopC2	= AMPLOOPC2_20KDEFAULT;
			test_Recipe.FrequencyHigh	= FREQENCYHIGH_20KDEFAULT;
			test_Recipe.FrequencyLow	= FREQENCYLOW_20KDEFAULT;
			test_Recipe.PhaseLoopI		= PLOOP_20KDEFAULT;
			test_Recipe.PhaseLoopCF		= PLOOPCF_20KDEFAULT;
			test_Recipe.TestAmplitude	= TEST_AMPLITUDE_DEFAULT;
			freqType					= FREQ_20KHZ * 1000; 
			break;
	
		case FREQ_30KHZ:
			test_Recipe.AmplitudeLoopC1 = AMPLOOPC1_30KDEFAULT;
			test_Recipe.AmplitudeLoopC2	= AMPLOOPC2_30KDEFAULT;
			test_Recipe.FrequencyHigh	= FREQENCYHIGH_30KDEFAULT;
			test_Recipe.FrequencyLow	= FREQENCYLOW_30KDEFAULT;
			test_Recipe.PhaseLoopI		= PLOOP_30KDEFAULT;
			test_Recipe.PhaseLoopCF		= PLOOPCF_30KDEFAULT;
			test_Recipe.TestAmplitude	= TEST_AMPLITUDE_DEFAULT;
			freqType					= FREQ_30KHZ * 1000;
			break;
	
		case FREQ_40KHZ:
			test_Recipe.AmplitudeLoopC1 = AMPLOOPC1_40KDEFAULT;
			test_Recipe.AmplitudeLoopC2	= AMPLOOPC2_40KDEFAULT;
			test_Recipe.FrequencyHigh	= FREQENCYHIGH_40KDEFAULT;
			test_Recipe.FrequencyLow	= FREQENCYLOW_40KDEFAULT;
			test_Recipe.PhaseLoopI		= PLOOP_40KDEFAULT;
			test_Recipe.PhaseLoopCF		= PLOOPCF_40KDEFAULT;
			test_Recipe.TestAmplitude	= TEST_AMPLITUDE_DEFAULT; 
			freqType					= FREQ_40KHZ * 1000;
			break;

		/* Default case never comes, Because RTP_SC main will ensure any of one system frequency type */
		default:
			test_Recipe.AmplitudeLoopC1 = AMPLOOPC1_20KDEFAULT;
			test_Recipe.AmplitudeLoopC2	= AMPLOOPC2_20KDEFAULT;
			test_Recipe.FrequencyHigh	= FREQENCYHIGH_20KDEFAULT;
			test_Recipe.FrequencyLow	= FREQENCYLOW_20KDEFAULT;
			test_Recipe.PhaseLoopI		= PLOOP_20KDEFAULT;
			test_Recipe.PhaseLoopCF		= PLOOPCF_20KDEFAULT;
			test_Recipe.TestAmplitude	= TEST_AMPLITUDE_DEFAULT;
			freqType					= FREQ_20KHZ * 1000;
			LOGERR("Invalid system configuration on configuring test recipe for power up seek operation",0,0,0);
			break;
	}
	
	sprintf(selectQuery,"SELECT AmplitudeLoopC1,AmplitudeLoopC2,PhaseLoopI,PhaseLoopCF,FrequencyHigh,FrequencyLow,TestAmplitude FROM %s WHERE psFreq = %d LIMIT 1;",TABLE_TEST_RECIPE,freqType);
	
	status = m_DbConn.ReadRecipe((const char*)selectQuery,columnCount,recipe);
	
	if(0 != status)
	{
		test_Recipe.AmplitudeLoopC1	= recipe[idx];
		idx = idx + 1;
		test_Recipe.AmplitudeLoopC2 = recipe[idx];
		idx = idx + 1;
		test_Recipe.PhaseLoopI 		= recipe[idx];
		idx = idx + 1;
		test_Recipe.PhaseLoopCF		= recipe[idx];
		idx = idx + 1;
		test_Recipe.FrequencyHigh 	= recipe[idx];
		idx = idx + 1;
		test_Recipe.FrequencyLow 	= recipe[idx];
		idx = idx + 1;
		test_Recipe.TestAmplitude 	= recipe[idx];
	}
	else
	{
		LOGERR("Test recipe read from DB failed hence assigned default values",0,0,0);
	}
	
	ProcessBuffer.msgID = TO_UI_TASK_TEST_RECIPE_REQ;
	memset(ProcessBuffer.Buffer,0x00,sizeof(ProcessBuffer.Buffer));
	memcpy(ProcessBuffer.Buffer,&test_Recipe,sizeof(test_Recipe));
	SendMsgToMsgQ(ProcessBuffer,UI_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Decodes the data sent by other tasks
*
* \param   - char *Recv_Buffer, UINT32 MsgLen 
*
* \return  - bool
*
******************************************************************************/
bool DataBaseManager::DeCoder(char *Recv_Buffer, UINT32 MsgLen)
{
	bool status;
	/* Decodes the received message from other tasks such as UI, etc...*/
	UINT32 pos = 0;
	
	if(pos < MsgLen)
	{
		memcpy(&MSG_ID,Recv_Buffer,sizeof(MSG_ID));
		status = true;
	}
	else
	{		
		status = false;
	}
	
	memcpy(&ProcessBuffer, Recv_Buffer, sizeof(ProcessBuffer));
	
	return status;
}

/**************************************************************************//**
* \brief   - Sends message to destination task's message queue.
*
* \param   - Message& msg, MSG_Q_ID MsgQId
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::SendMsgToMsgQ(Message& msg, MSG_Q_ID& MsgQId)
{
	if(msgQSend(MsgQId, reinterpret_cast<char*>(&msg), sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGWARN("DB_T : -----------Error on msgQSend----------",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Identifies the requests(store or retrieve weld history) and
*			 process it.
* \param   - char *Recv_Buffer
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ProcessMessage(char *Recv_Buffer)
{
	Message message;
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	Report_Message reportMessage;
	UINT8 status = RESPONSE_SUCCESS, memoryStatusCmd=0;
	INT32 msgLength;
	ReportGeneration* pRepGen = NULL;
	RunTime RT;
	stringstream logData (((Message *) Recv_Buffer)->Buffer);
	string details, eventId, eventName, userName, comment;
	string ActiveRecipe,strReasonForSetRecipeValue;

	RT.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);		/* Needs to place in different place , Read should happen only when model changed */
	char bcRecipeID[sizeof(m_barcodeRecipeID)] = {0x00};
	
//	printf ("\n%sDataBaseManager::ProcessMessage: MSG_ID: %d msgCount: %d %s\n", KBLU, MSG_ID, msgQNumMsgs(GetDBMsgQID()), KNRM);
	
	switch(MSG_ID)
	{	
		case TO_DB_TASK_WELD_SIGN_DATA:
			
			if(ENABLE == RT.ModelFeature.RtFeature1.WeldDataLog_Flag_B15)
			{
				StoreWeldSignature(Recv_Buffer);
			  	ProcessTableCapacity(WELD_SIGNATURE_TABLE);
			}
			else
			{
				LOGERR("DB_T : -------WELD DATA LOG DISABLED-------",0,0,0);
			}
			
			break;
			
		case TO_DB_TASK_UL_WELD_RESULT:
			
			if(ENABLE == RT.ModelFeature.RtFeature1.WeldDataLog_Flag_B15)
			{
#ifdef PERFORMANCE_MEASURE
				m_startTime = TimeStamp();
#endif
				status = StoreWeldResult(Recv_Buffer);      /* Weld Result to be saved first */
				CP->ShutdownCriticalDataFlag = 0;			/* Reset the flag on critical data inserted into DB */
				ProcessTableCapacity(WELD_RESULT_TABLE);
#ifdef PERFORMANCE_MEASURE
				m_endTime = TimeStamp();
				PerformanceMeasureLog();
#endif
			}
			else
			{
				LOGERR("DB_T : -------WELD DATA LOG DISABLED-------",0,0,0);
			}
			
			message.msgID = TO_UI_TASK_UL_WELD_RESULT;
			sprintf(message.Buffer,"%d",status);
			SendMsgToMsgQ(message,UI_MSG_Q_ID);
			break;
	
		case TO_DB_TASK_UL_SEEK_RESULT:
			DeleteHistory(TABLE_SEEK_SIGNATURE);
			status = StoreSeekSignature();
			DeleteHistory(TABLE_SEEK_RESULT);
			StoreSeekResult();
			message.msgID = TO_UI_TASK_UL_SEEK_RESULT;
			sprintf(message.Buffer,"%d",status);
			SendMsgToMsgQ(message,UI_MSG_Q_ID);
			break;
	
		case TO_DB_TASK_SCAN_SIGN_DATA:
			StoreHornScanSignature();
			break;
	
		case TO_DB_TASK_HORN_SCAN_RESULT:
			if(1 == CP->m_ScanRecipeObj.GetDigitalTuneStatus())
			{
				UpdateDigitalTune();			/* Latest digital tune will be updated in stack recipe table */
			}
			status = StoreScanResult();
			ProcessTableCapacity(HORN_SCAN_TABLE);
			message.msgID = TO_UI_TASK_HORN_SCAN_RESULT;
			sprintf(message.Buffer,"%d",status);
			SendMsgToMsgQ(message,UI_MSG_Q_ID);
			break;
	
		case TO_DB_TASK_CTRL_TEST_SIGN_DATA:
			DeleteHistory(TABLE_TEST_SIGNATURE);
			StoreTestSignature();
			break;
	
		case TO_DB_TASK_TEST_RESULT:		
			DeleteHistory(TABLE_TEST_RESULT);
			status = StoreTestResult();
			message.msgID = TO_UI_TASK_TEST_ABORT_CMD_RES;
			sprintf(message.Buffer,"%d",status);
			SendMsgToMsgQ(message, UI_MSG_Q_ID);
			break;

		case TO_DB_TASK_ALARM_DATA_SC:
			StoreAlarmLogs(ALARM_DATA_SC, Recv_Buffer);
			break;
			
		case TO_DB_TASK_UL_ALARM_DATA_PC:
			StoreAlarmLogs(ALARM_DATA_PC, Recv_Buffer);
			break;

		case TO_DB_TASK_UL_ALARM_DATA_AC:
			StoreAlarmLogs(ALARM_DATA_AC, Recv_Buffer);
			break;
			
		case TO_DB_TASK_PASSWORD_OVERRIDE:
			
			bool override;
			memcpy (&message, Recv_Buffer, sizeof (message));
			memcpy (&override, message.Buffer, sizeof (override));
			StorePswdOverride (override);
			break;
			
		case TO_DB_TASK_ALARM_MGR_ALARM_DATA:
			StoreAlarmLogs(ALARM_DATA_ALARM_MGR, Recv_Buffer);	
			ProcessTableCapacity(ALARM_LOG_TABLE);
			break;

		case TO_DB_TASK_EMERGENCY_STOP_ACTIVE_IND:
			EventLog(ESTOP_PRESS);
			break;
			
		case TO_DB_TASK_EMERGENCY_STOP_RESET_IND:
			EventLog(ESTOP_RELEASE);
			break;
			
		case TO_DB_TASK_ALARM_CONFIG_RW_REQ:
			ReadAlarmConfFromDB();
			break;
			
		case TO_DB_TASK_CREATE_DEFAULT_ALARM_CONFIG:		
			
			memcpy (&message, Recv_Buffer, sizeof (message));
			WriteAlarmConfigToDB (((pAlarmConfiguration) message.Buffer));	
			break;
			
		case TO_DB_TASK_ALARM_CONFIG_SAVE_REQ:							

			DeleteHistory(TABLE_ALARM_CONFIGURATION);			
			status = WriteAlarmConfigToDB((AlarmConfiguration *)ProcessBuffer.Buffer);
			
			memset(ProcessBuffer.Buffer, 0x00, sizeof(ProcessBuffer.Buffer));
			
			memset(message.Buffer, 0x00, sizeof(message.Buffer));			
			sprintf(message.Buffer,"%d",status);

			message.msgID = TO_ALARM_TASK_ALARM_CONFIG_SAVE;
			SendMsgToMsgQ (message,ALARM_MSG_Q_ID);
			
			break;
			
		case TO_DB_TASK_CALIBRATION_ABORT_CMD_REQ:
			 CalibrationData();
			 break;
			 
		case TO_DB_TASK_DB_CONNECTION_CLOSE_REQ:
			CloseDB();
			message.msgID = TO_DB_TASK_DB_CONNECTION_CLOSE_RES;
			sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
			SendMsgToMsgQ(message, UI_MSG_Q_ID);
			break;
			
		case TO_DB_TASK_DB_CONNECTION_OPEN_REQ:
			ConnectDB();
			message.msgID = TO_DB_TASK_DB_CONNECTION_OPEN_RES;
			sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
			SendMsgToMsgQ(message, UI_MSG_Q_ID);
			break;
			
		case TO_DB_TASK_BARCODE_RECIPE_RECALL_REQ:			
			memcpy(m_pBarcodeData, ProcessBuffer.Buffer, sizeof(m_pBarcodeData));
			/* Copy only the barcode recipe Id and not the prefix */
			strncpy(bcRecipeID, &m_pBarcodeData[1], sizeof(bcRecipeID));
			m_barcodeRecipeID = atoi(bcRecipeID);
			LOGDBG("DB_T: Recipe ID scanned from Barcode is: %d\n", m_barcodeRecipeID, 0, 0);
			if( (m_barcodeRecipeID >= RECIPE_ID_MIN_RANGE) &&  (m_barcodeRecipeID <= RECIPE_ID_MAX_RANGE) )			
			{
				status = ReadActiveRecipeFromDB(m_barcodeRecipeID);									
				if(status == 0)
				{
					/* Notify UIC the Active Recipe ID picked up from the Barcode */
					memset(&message, 0x00, sizeof(message));
					message.msgID = TO_UI_TASK_BARCODE_RECIPE_RECALL_REQ;					
					memcpy(message.Buffer, &m_barcodeRecipeID, sizeof(m_barcodeRecipeID));																		 						
				}
				else
				{
					LOGERR("DB_T: Failed to read the Active recipe from DB: %d\n", m_barcodeRecipeID, 0, 0);
					memset(&message, 0x00, sizeof(message));
					message.msgID = TO_UI_TASK_BARCODE_ERROR_IND;  	
					sprintf(message.Buffer,"%d",RECIPE_RECALL_FAIL);				
				}
			}
			else
			{
				LOGERR("DB_T: Recipe ID is beyond the range: %d\n", m_barcodeRecipeID, 0, 0);
				memset(&message, 0x00, sizeof(message));
				message.msgID = TO_UI_TASK_BARCODE_ERROR_IND;  	
				sprintf(message.Buffer,"%d",RECIPE_ID_BEYOND_RANGE);			
			}
			SendMsgToMsgQ(message, UI_MSG_Q_ID);									 		
			break;
			
		case TO_DB_TASK_BARCODE_RECIPE_RECALL_RSP:
			/* If the Recipe ID sent is made active on UI successfully, set it as active recipe in BL */			
			LOGDBG("----------- DB_T: Updating Active recipe ------------\n", 0, 0, 0);			
			SetActiveRecipe();			
			/* Updated Weld recipe read from DB based on the Barcode Recipe ID to be sent to AC and PC */
			message.msgID = TO_SDO_TASK_WELD_RECIPE_AC;					
			SendMsgToMsgQ(message, SDO_MSG_Q_ID);			
			message.msgID = TO_SDO_TASK_WELD_RECIPE_PC;					
			SendMsgToMsgQ(message, SDO_MSG_Q_ID);
			memset(&message, 0x00, sizeof(message));
			break;
			
		case TO_DB_TASK_GEN_SINGLE_REPORT_REQ:
			memset(reportMessage.reportBuff, 0x00, sizeof(reportMessage.reportBuff));
			memcpy((char *)&reportMessage, (char *)ProcessBuffer.Buffer, sizeof(reportMessage));
			
			//LOGDBG("DB_T : Report Generation Request Parameters: Report ID=%d, Storage format = %d, Report Storage=%s",reportMessage.reportID, reportMessage.storageFormat, (_Vx_usr_arg_t)reportMessage.reportBuff);
			
			if(NULL != pRepGen)
			{
				LOGERR("DB_T : Weld Data generation in progress. Aborting the request", 0, 0, 0);
				status = RESPONSE_FAIL ;
			}
			else
			{
				switch(reportMessage.storageFormat)
				{
					case CSV_FORMAT:
					{
						if(reportMessage.reportID ==  WELDRESULT_AND_WELDSIGNATURE_TYPE)
						{
							//Generate both weld result and weld signature/graph reports
							for(int nReportNumber = 0; nReportNumber <= 1; nReportNumber++) 
							{
								switch(nReportNumber)
								{
									case 0:
									{
										reportMessage.reportID = WELDRESULT_TYPE;
									}
									break;
														
									case 1:
									{
										reportMessage.reportID = WELDSIGNATURE_TYPE;
									}
									break;									
								}//end of switch case
													
								pRepGen = new (nothrow) CSVFormat(reportMessage);
								if(NULL != pRepGen)
								{
									status = pRepGen->GenerateReport();
									delete pRepGen;
									pRepGen = NULL;
								}
								else
								{
									LOGERR("DB_T : Memory allocation failed",0,0,0);
									status = RESPONSE_FAIL;	
									//break from for loop
									break;
								}
													
								if(status != REPORT_GENERATE_SUCCESS)
								{
									//break from for loop if report generation unsuccessful even in case of first report.
									LOGERR("DB_T : CSV Report generation failed",0,0,0);
									break;
								}
							}//End of for loop
						}
						else
						{
							//Generate single report
							if(reportMessage.reportID == WELDRESULT_TYPE || reportMessage.reportID == WELDSIGNATURE_TYPE)
							{
								pRepGen  = new (nothrow) CSVFormat(reportMessage);
													
								if(NULL != pRepGen)
								{
									status = pRepGen->GenerateReport();
									delete pRepGen;
									pRepGen = NULL;
								}
								else
								{
									LOGERR("DB_T : Memory allocation failed",0,0,0);
									status = RESPONSE_FAIL;	
								}
							}
							else
							{
								LOGERR("DB_T : Incorrect report type received from user",0,0,0);
								status = RESPONSE_FAIL;	
							}
						}
					}
					break;
																			
					case PDF_FORMAT:
					{
						pRepGen = new (nothrow) PDFReportGeneration(reportMessage, &m_DbConn);
						if(NULL != pRepGen)
						{
							status = pRepGen->GenerateReport();
							delete pRepGen;
							pRepGen = NULL;		
						}
						else
						{
							LOGERR("DB_T : Memory allocation failed",0,0,0);
							status = RESPONSE_FAIL;	
						}
					}
					break;
										
					default: 
					{
						LOGERR("DB_T : Incorrect format received from user",0,0,0);
						status = RESPONSE_FAIL;			
					}
					break;
				}								
			}
			
			LOGDBG("DB_T : -------WELD DATA REPORT STATUS: %d-------",status,0,0);
			message.msgID = TO_UI_TASK_GEN_SINGLE_REPORT_RES;
			sprintf(message.Buffer,"%d",status);
			SendMsgToMsgQ(message, UI_MSG_Q_ID);
			break;
			
		case TO_DB_TASK_GEN_REPORT_ACTION_IND:
			memcpy(&memoryStatusCmd, (char *)ProcessBuffer.Buffer, sizeof(memoryStatusCmd));
			
			Report_Message tmpReportMessage ;

			
			LOGDBG("DB_T : -------Continue Command Request ID: %d-------",memoryStatusCmd,0,0);

			/* Retrieve the stored report data values from the Class. */
			tmpReportMessage = pRepGen->GetReportData();
	
			switch(tmpReportMessage.storageFormat)
			{
			case CSV_FORMAT:
				pRepGen  = new CSVFormat(tmpReportMessage);
				break;
				
			case PDF_FORMAT:
#ifdef PDF_GENERATION
				pRepGen = new PDFReportGeneration(tmpReportMessage);
#else
				LOGERR("DB_T : Implementation hidden for PDF generation",0,0,0);
				status = RESPONSE_FAIL ;
#endif /* PDF_GENERATION */
				break;
				
			default: 
				LOGERR("DB_T : Incorrect format received from user",0,0,0);
				status = RESPONSE_FAIL ;
				break;
			}
			
			if(RESPONSE_SUCCESS == status)
			{
				/* We generate the report here if the format received from user is as expected. Else the error fail code is sent and pointer is deleted */
				status = pRepGen -> GenerateReport(memoryStatusCmd);
			}				
			delete pRepGen ;
			pRepGen = NULL ;

			LOGDBG("DB_T : -------WELD DATA REPORT STATUS: %d-------",status,0,0);
			
			message.msgID = TO_UI_TASK_GEN_SINGLE_REPORT_RES;
			sprintf(message.Buffer,"%d",status);
			SendMsgToMsgQ(message, UI_MSG_Q_ID);
			break;
			
		case TO_DB_TASK_BATCH_COUNT_RESET_REQ:
			ResetBatchCount();
			break;
			
		case TO_DB_TASK_NEW_RECIPE_RECEIVED:			/* Read cycle counter and batch setup configuration for new recipe */
			ResetProductionSetting();
			break;
			
		case TO_DB_TASK_READ_RECIPE:
			ReadSeekRecipe();
			ReadScanRecipe();
			ReadTestRecipe();
			break;
			
		case TO_DB_TASK_LOG_EVENT:

			// get the event ID and log data
			getline (logData, eventId, ',');
			getline (logData, details);
			
			// log the event
			EventLog ((LOG_EVENT) atoi (eventId.c_str()), &details);
			break;

		case TO_DB_TASK_LOG_EVENT_COMMENT:
			
			// get the user name, event name and associated comment
			getline (logData, userName, ',');
			getline (logData, eventName, ',');
			getline (logData, comment);

			// log the event comment
			EventLogComment (userName, eventName, comment);
			break;
			
		case SHUTDOWN_APPLICATION:
			break;
			
		case TO_DB_TASK_MEMORY_FULL_STORAGE_CHECK:
			CheckCapacitiesForNewConfig();
			break;
			
		case TO_DB_TASK_WS_USER_DETAILS:
			memcpy (&message, Recv_Buffer, sizeof (message));
			GetUserDetails(message);
			break;
			
		case TO_DB_TASK_WS_EVENT_LOGIN:
			
			memcpy (&message, Recv_Buffer, sizeof (message));
			userName = message.Buffer;
			details = "Identifier:" + userName + ":From:N/A:To:N/A:";
			status = (UINT8) EventLog (USER_LOGIN_WEB, &details);
			
			message.msgID = TO_WEB_SERVICES_TASK_WS_EVENT_LOGIN;
			sprintf(message.Buffer,"%d", status == 1 ? 0 : 1);
			SendMsgToMsgQ(message,WEBSERVICE_MSG_Q_ID);
			break;

		case TO_DB_TASK_WS_LAST_WELD_RESULTS:
			GetlastWeldResult();
			break;

		case TO_DB_TASK_WS_NUMBER_OF_ALARMS:
			GetNumberOfAlarms();
			break;
			
		case TO_DB_TASK_WS_GET_PW_EXPIRY_DAYS:
			GetPasswordExpiryInDays();
			break;
			
		case TO_DB_TASK_WS_EVENT_LOGOUT:

			memcpy (&message, Recv_Buffer, sizeof (message));
			userName = message.Buffer;
			details = "Identifier:" + userName + ":From:N/A:To:N/A:";
			status = (UINT8) EventLog (USER_LOGOUT_WEB, &details);

			message.msgID = TO_WEB_SERVICES_TASK_WS_EVENT_LOGOUT;
			sprintf(message.Buffer,"%d", status == 1 ? 0 : 1);
			SendMsgToMsgQ(message,WEBSERVICE_MSG_Q_ID);
			break;

		case TO_DB_TASK_WS_GET_IDLE_LOGOUT_TIME:
			GetIdleLogoutTime();
			break;

		case TO_DB_TASK_WS_NUMBER_OF_WELDHISTORY:
			memcpy (&message, Recv_Buffer, sizeof (message));
			GetNumberOfWeldHistory(message);
			break;

		case TO_DB_TASK_WS_AUTHORITY_CHECK_OPERATOR:
			GetOperatorAuthorityPrivileges();
			break;

		case TO_DB_TASK_WS_SET_ACTIVE_RECIPE:
			memcpy (&message, Recv_Buffer, sizeof (message));
			SetActiveRecipe(message);
			break;

		case TO_DB_TASK_WS_GET_ACTIVE_RECIPE:
			status = ReadActiveRecipeFromDB(1,true);
			message.msgID = TO_WEB_SERVICES_TASK_GET_ACTIVE_RECIPE;
			if(status == 1)
			{
				sprintf(message.Buffer,"%d",SYSTEM_BUSY);
			}
			else
			{
				sprintf(message.Buffer,"%d",WS_SUCCESS);
			}
			SendMsgToMsgQ(message,WEBSERVICE_MSG_Q_ID);
			break;

		case TO_DB_TASK_WS_SET_ACTIVE_WELD_RECIPE:
			memcpy (&message, Recv_Buffer, sizeof (message));
			ActiveRecipe = message.Buffer;
			status = CopyActiveRecipe(ActiveRecipeSC, ActiveRecipeAC, ActiveRecipePC, ActiveRecipe);
			message.msgID = TO_WEB_SERVICES_TASK_GET_ACTIVE_RECIPE;
			if(status == 1)
			{				
				sprintf(message.Buffer,"%d",SYSTEM_BUSY);
			}
			else
			{
				sprintf(message.Buffer,"%d",WS_SUCCESS);
			}
			SendMsgToMsgQ(message,WEBSERVICE_MSG_Q_ID);
			break;

		case TO_DB_TASK_WS_SET_ACTIVE_SR_RECIPE:
			memcpy (&message, Recv_Buffer, sizeof (message));
			ActiveRecipe = message.Buffer;
			status = CopySRActiveRecipe(ActiveRecipeSC, ActiveRecipe);
			message.msgID = TO_WEB_SERVICES_TASK_GET_ACTIVE_RECIPE;
			if(status == 1)
			{				
				sprintf(message.Buffer,"%d",SYSTEM_BUSY);
			}
			else
			{
				sprintf(message.Buffer,"%d",WS_SUCCESS);
			}
			SendMsgToMsgQ(message,WEBSERVICE_MSG_Q_ID);
			break;

		case TO_DB_TASK_WS_DELETE_RECIPE:
			memcpy (&message, Recv_Buffer, sizeof (message));
			DeleteRecipe(message);
			break;

		case TO_DB_TASK_WS_SAVE_RECIPE:
			SaveRecipeZeroToDB();
			break;

		case TO_DB_TASK_WS_GET_ALARM_LOG:
			memcpy (&message, Recv_Buffer, sizeof (message));
			GetAlarmLog(message);
			break;

		case TO_DB_TASK_WS_GET_WELD_HISTORY:
			memcpy (&message, Recv_Buffer, sizeof (message));
			GetWeldHistory(message);
			break;

		case TO_DB_TASK_WS_GET_GRAPH_RESULTS:
			memcpy (&message, Recv_Buffer, sizeof (message));
			GetGraphResult(message);
			break;
			
		case TO_DB_TASK_WS_EVENT_SET_RECIPE_VALUE:
			// not in use
			break;
			
		case TO_DB_TASK_WS_GET_RUNTIME_DETAILS:
			GetRuntimeDetailsOfWebservices();
			break;
			
		case TO_DB_TASK_WS_GET_ALL_USER_DETAILS:
			GetAllUserDetails();
			break;
			
		case TO_DB_TASK_WS_GET_IO_CONFIGURATION:
			memset(&message, 0x00, sizeof (message));
			message.msgID = TO_WEB_SERVICES_TASK_GET_IO_CONFIGURATION;
			GetUserIOConfig(message);
			SendMsgToMsgQ(message,WEBSERVICE_MSG_Q_ID);
			break;
			
		default :
			LOGWARN("DB_T : ---------Unknown Message Received--------------",MSG_ID,0,0);
			break;
	}
}

/**************************************************************************//**
* \brief   - Get the alarm data and store into DB, Then sends response to UI task
*
* \param   - UINT8 machineId
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::StoreAlarmLogs(UINT8 machineId, char * Recv_Buffer)
{
	string queryData,dateTime;
	UINT8 status  = 0;
	INT32 ErrCode = 0;
	Message message;
	
	GetDateAndTime(dateTime);
	
	if(ALARM_DATA_SC == machineId)
	{
		// take the alarm info from the original message
		memcpy (&message, Recv_Buffer, sizeof (message));

		CP->m_ScAlarmObj.SetAlarmData (((pALARM_INFO) message.Buffer)->id, ((pALARM_INFO) message.Buffer)->alarmType, ((pALARM_INFO) message.Buffer)->flags);

	}
	else if(ALARM_DATA_PC == machineId)
	{
		memcpy (message.Buffer, CP->m_PcAlarmObj.GetAlarmLog (), sizeof (PCAlarmData));
		message.msgID = TO_ALARM_TASK_ALARM_INFO_PC;
		
		// send it to the alarm task
		SendMsgToMsgQ (message, ALARM_MSG_Q_ID);
	}
	else if(ALARM_DATA_AC == machineId)
	{
		memcpy (message.Buffer, CP->m_AcAlarmObj.GetAlarmLog (), sizeof (ACAlarmData));
		message.msgID = TO_ALARM_TASK_ALARM_INFO_AC;

		// send it to the alarm task
		SendMsgToMsgQ (message, ALARM_MSG_Q_ID);
	}
	else if(ALARM_DATA_ALARM_MGR == machineId)
	{
		// take latest alarm data from the alarm manager
		memcpy (&message, Recv_Buffer, sizeof (message));

		// save to database here
		if (((pALARM_INFO) message.Buffer)->source == ALARM_PC)
			CP->m_PcAlarmObj.GetAlarmData (dateTime, queryData, message.Buffer);
		else if (((pALARM_INFO) message.Buffer)->source == ALARM_AC)
			CP->m_AcAlarmObj.GetAlarmData (dateTime, queryData, message.Buffer);
		else if (((pALARM_INFO) message.Buffer)->source == ALARM_SC)
			CP->m_ScAlarmObj.GetAlarmData(dateTime, queryData);
		else
			LOGERR("DB_T : Invalid alarm source : %d",((pALARM_INFO) message.Buffer)->source,0,0);

		ErrCode = m_DbConn.SingleTransaction((char *)queryData.c_str()); 
		if(0 != ErrCode)
		{
			LOGERR("DB_T : Error on writing alarm data with ErrCode : %d",ErrCode,0,0);
		}
	}
	else
	{
		LOGERR("DB_T : Invalid alarm machine ID : %d",machineId,0,0);
	}

#ifdef DEBUG
	cout << "DB_T : Alarm Data Insert : " << queryData << endl;
#endif	
}


/**************************************************************************//**
* \brief   - Selects from GlobalSettings table to read, modify and write the
* 			new override flag, in this table, that was passed on to here from the
* 			power-up state after the override detect sequence was completed.
*
* \param   - bool override
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::StorePswdOverride (bool override)
{
	UINT16 expiry, idle, offset1, offset2;
	string read, query;
	char write[200];
	
	// possible query results: BInterface::GetGlobalSettings: str: 90,20,0
	
    // read settings
	query = "select * from GlobalSettings";
	read = m_DbConn.Exec((char*) query.c_str());

#ifdef OVERRIDE_DBG
	printf ("\nDataBaseManager::StorePswdOverride: read: %s\n", read.c_str());
#endif
    
    // modify settings
    offset1 = read.find(",", 0);
    
	if (offset1)
	{
		expiry = atoi (read.substr(0, offset1).c_str());
		
	    offset2 = read.find(",", offset1);
	    
	    if (offset2)
	    {
	    	idle = atoi (read.substr(offset1+1, offset2).c_str());
	    	
	    	// write settings
	    	snprintf (write, sizeof (write), "UPDATE GlobalSettings SET PASSWORD_EXPIRY_IN_DAYS = %d , IDEAL_TIME_LOGOGG_IN_MIN = %d , PASSWORD_OVERRIDE = %d", expiry, idle, override);
    		
	    	m_DbConn.Exec (write);

#ifdef OVERRIDE_DBG
	    	printf ("\nDataBaseManager::StorePswdOverride: write: %s\n", write);
#endif
	    	
	    }
	}
}


/**************************************************************************//**
* \brief   - Sends indication to synchronize the weld data between UI and DB
*
* \param   - string weldData - RecipeNum, RevNum and CycleCounter of deleted
* 			 record.
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::SynchronizeWeldData(string weldData)
{
	Message msg;
	memset(&msg.Buffer,0x00,sizeof(msg.Buffer));
	
	msg.msgID = TO_UI_TASK_WELD_DATA_DELETE_IND;
	strncpy(msg.Buffer, weldData.c_str(), (sizeof(msg.Buffer)-1));
	SendMsgToMsgQ(msg, UI_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Updates cycle counter after every weld.
*
* \param   - UINT32 cycleCounter.
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::UpdateCycleCounter(UINT32 cycleCounter)
{
	/* Assigning cycle counter to control task when active recipe changes or during power on */ 
	ControlTask::SetCycleCounter(cycleCounter);
	CommonProperty::weldResult.Set(WeldResults::CYCLE_COUNTER,cycleCounter);
}

/**************************************************************************//**
* \brief   - Gets Recipe Information
*
* \param   - UINT32 recipeNumber, UINT32  weldRecipeVerNumber, INT32 weldMode
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetRecipeTableInformation(INT32 recipeNumber, INT32  &weldRecipeVerNumber, INT32 &weldMode)
{
	char   SelectQuery[MSG_SIZE]    = {0x00};
	INT32  queryStatus      = 0;
	INT8   colIndex			= 0;
	size_t colPos			= 0;
	size_t firstPos         = 0;
	string ActiveWeldRecipe = "";
	string WeldRecipeVer    = "";
	string WeldMode 		= "";
	
	/* Read the SC, AC & PC active recipe data from DB */
	sprintf(SelectQuery, "SELECT WeldRecipeVerNumber,WeldMode FROM WeldRecipeTable WHERE RecipeNumber = %d;",recipeNumber);
	
	ActiveWeldRecipe = m_DbConn.Exec(SelectQuery, &queryStatus);
	
	if(queryStatus != 0)
	{					
		LOGDBG("DB_T : GetRecipeTableInformation : ErrCode from DB : %d", queryStatus, 0, 0);			
	}	
	else
	{
		/*Extract the Recipe version number and Weld mode from Weld Signature*/
		colPos=ActiveWeldRecipe.find(DB_DELIMITER, colPos+1);
		
		WeldRecipeVer = ActiveWeldRecipe.substr(firstPos, colPos-firstPos);
		
		firstPos = colPos+1;
		WeldMode = ActiveWeldRecipe.substr(firstPos, strlen(ActiveWeldRecipe.c_str())-firstPos);

		if(!WeldRecipeVer.empty() && !WeldMode.empty())
		{
			weldRecipeVerNumber = atoi(WeldRecipeVer.c_str());
			weldMode			= atoi(WeldMode.c_str());
		}
		else
		{
			LOGDBG("DB_T : GetRecipeTableInformation : Weld Count and Weld Recipe Ver number Read Error", 0, 0, 0);	
		}
		
	}	
}

/**************************************************************************//**
* \brief   - Gets Batch ID from DB
*
* \param   - UINT32  - recipeNumber
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetBatchID(INT32 recipeNumber, string &BatchID)
{
	char   SelectQuery[MSG_SIZE]    = {0x00};
	INT32  queryStatus      = 0;
	string batchId			= "";
	
	sprintf(SelectQuery,"SELECT BatchID FROM ProductionSetupTable WHERE RecipeNumber = %d;",recipeNumber);
	
	batchId=m_DbConn.Exec(SelectQuery, &queryStatus);
	
	if(queryStatus != 0)
	{					
		LOGDBG("DB_T : GetBatchID : ErrCode from DB : %d", queryStatus, 0, 0);			
	}
	else
	{
		BatchID = batchId;
	}
}

/**************************************************************************//**
* \brief   - Checks critical data during Power On.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::CheckCriticalData()
{
	INT8 status 						= OK;
	INT32 queryStatus 					= 0;
	INT16 noOfRecords 					= 0;
	ssize_t criticalDatasize 			= 0;
	char SelectQuery[DB_QUERY_SIZE] 	= {0x00};
	char timeBuffer[TIMESTAMP] 			= {0x00};
	ShutdownCriticalData criticalData 	= {0x00};
	WeldResult results 					= {0x00};
	string DataTime 					= "";
	string BatchID 						= "";
	string buffer 						= "";
	Eeprom epclass;

	criticalDatasize = sizeof(criticalData);
	status = epclass.Read((char *)&criticalData, criticalDatasize, (ssize_t)SHUTDOWN_EEPROM_OFFSET);

	LOGERR("CRITICAL_DATA_STATUS_FLAG = 0x%x\n", criticalData.WeldResultFlag, 0, 0);

	if(ERROR == status)
	{
		LOGERR("DB_T : CheckCriticalData: -------------Error in Reading Critical Data-----------",0,0,0);
	}
	else if(CRITICAL_DATA_STATUS_FLAG == criticalData.WeldResultFlag)
	{
		/*Copying Weld Results*/
		results = criticalData.Results;

		/* Query DB for Recipe # and Cycle count already exists? */
		sprintf(SelectQuery,"SELECT COUNT(1) FROM WeldResultTable WHERE RecipeNumber = %d AND CycleCounter = %d;",
				results.RecipeNumber, results.CycleCounter);

		buffer = m_DbConn.Exec(SelectQuery, &queryStatus);
		noOfRecords = atoi(buffer.c_str());

		if(SQLITE_OK == queryStatus)
		{
			if(0 == noOfRecords)
			{
				criticalData.SysTime.tm_year = 1900 + criticalData.SysTime.tm_year;
				criticalData.SysTime.tm_mon  = 1 + criticalData.SysTime.tm_mon;
				sprintf(timeBuffer,"%d/%02d/%02d %02d:%02d:%02d",criticalData.SysTime.tm_year,criticalData.SysTime.tm_mon,criticalData.SysTime.tm_mday,criticalData.SysTime.tm_hour,criticalData.SysTime.tm_min,criticalData.SysTime.tm_sec);

				/*Reading Date and Time*/
				DataTime = timeBuffer;

				/*Updating Weld Status(Good, Reject Suspect) in Production Table*/
				ProductionDataAnalysis(results.RecipeNumber, results.WeldStatus, results.CycleCounter);

				/*Reading System configuration from EEPROM*/
				//sysConf.SysComponentGet((char *)&systemComponent);

				/*Reading Batch ID*/
				GetBatchID(results.RecipeNumber, BatchID);

				/*Reading Stack Serial Number from EEPROM at the offset of 301*/
				//sysConf.GetSmartStackInfo((char *)&smartStackInf);
				//sprintf(stackSerialNum, "%d",smartStackInf.StackSerialNum);

#ifdef SHUTDOWN_DEBUG
				printf("------------- WeldResults -------------\n");
				printf("CycleCounter            :   %6d \n", results.CycleCounter); 
				printf("WeldRecipeNumber        :   %6d \n", results.RecipeNumber);
				printf("StartFrequency(Hz)      :   %6d (Hz) \n", results.StartFrequency);
				printf("FrequencyChange(Hz)     :   %6d (Hz) \n", results.FrequencyChange);	
				printf("WeldTime(ms)            :   %6d (ms) \n", results.WeldTime/1000);
				printf("CycleTime(ms) 		:  %d (ms) \n", results.CycleTime/CONVERSION_SECONDS);	
				printf("TotalEnergy(J)          :   %6d (J) \n", results.TotalEnergy); 
				printf("WeldPeakPower(W)        :   %6d (W) \n", results.PeakPower);
				printf("MaxWeldForce(N)         :   %6d (N) \n", results.MaxWeldForce);
				printf("EndHoldForce(N)         :   %6d (N) \n", results.EndHoldForce);
				printf("WeldAbsolute(um)        :   %6d (um) \n", results.WeldAbsolute);
				printf("TotalAbsolute(um)       :   %6d (um) \n", results.TotalAbsolute);
				printf("WeldCollapseDistance(um):   %6d (um) \n", results.WeldCollapseDistance);
				printf("HoldCollapseDistance(um):   %6d (um) \n", results.HoldCollapseDistance);
				printf("TotalCollapseDistance(um):   %6d (um) \n", results.TotalCollapseDistance);
				printf("TriggerDistance(um) 	:   %6d (um) \n", results.TriggerDistance);
				printf("TriggerTime(ms) 	:   %f (ms) \n", (float)results.TriggerTime/4);
				printf("WeldSonicTime(ms) 	:   %f (ms) \n", (float)results.WeldSonicTime/4);			
				printf("HoldTime(ms) 		:   %f (ms) \n", (float)results.HoldTime/4);
				printf("Velocity(um/s) 		:   %d (um/s) \n",results.Velocity);
				printf("Alarm Flag			:   %d \n",results.bIsAlarmflag);
				printf("PartID 			:   %s\n", results.PartID);
				printf("-------------------------------------\n");
				//printf("PowerSupplyAssembly		   :   %s\n",systemComponent.powerSupplyComponent.PowerSupplyAssembly);
				//printf("ActuatorAssembly		   :   %s\n",systemComponent.actuatorComponent.ActuatorAssembly);
				//printf("StackAssembly			   :   %s\n",systemComponent.stackComponent.StackAssembly);

				cout <<"TimeStamp				   :  "<<DataTime<<endl;
				cout <<"BatchID				   	   :  "<<BatchID<<endl;
#endif							
				memset(SelectQuery, 0x00, sizeof(SelectQuery));
				sprintf(SelectQuery, "INSERT INTO WeldResultTable(RecipeNumber,WeldRecipeVerNumber,DateTime,StackSerialNo,CycleCounter,WeldMode,MaxWeldForce,"
						"EndHoldForce,WeldAbsolute,TotalAbsolute,WeldCollapseDistance,HoldCollapseDistance,TotalCollapseDistance,TriggerDistance,Velocity,WeldTime,WeldEnergy,"
						"WeldPeakPower,StartFrequency,FrequencyChange,CycleTime,Username,PartID,BatchID,TriggerPoint,WeldSonicPoint,HoldPoint,AlarmFlag) VALUES(%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s',%f,%f,%f,%d);",
						results.RecipeNumber, results.RecipeRevNumber, DataTime.c_str(), results.StackSerialNo,
						results.CycleCounter, results.WeldMode,results.MaxWeldForce, results.EndHoldForce, results.WeldAbsolute,
						results.TotalAbsolute, results.WeldCollapseDistance, results.HoldCollapseDistance,
						results.TotalCollapseDistance, results.TriggerDistance, results.Velocity, results.WeldTime/CONVERSION_SECONDS,
						results.TotalEnergy, results.PeakPower,(results.StartFrequency/CONVERSION_KHZ), (results.FrequencyChange/CONVERSION_KHZ), 
						results.CycleTime/CONVERSION_SECONDS, results.UserName, results.PartID, BatchID.c_str(), (float)results.TriggerTime/CONVERSION_CYCLE, 
						(float)results.WeldSonicTime/CONVERSION_CYCLE, (float)results.HoldTime/CONVERSION_CYCLE, results.bIsAlarmflag);

				/*Storing Critical Data into DB on Power On*/
				queryStatus = m_DbConn.SingleTransaction(SelectQuery);
				if(SQLITE_OK != queryStatus)
				{
					LOGERR("DB_T : Critical data weld result insert query failed with ErrCode : %d for CycleCounter : %d", queryStatus, results.CycleCounter,0);
				}
				else
				{
					LOGDBG("DB_T : CheckCriticalData: -------------Successfully saved the Critical Data into DB-----------",0,0,0);
				}
			}
			else /* if - result already present in weld result table */
			{
				/* Query DB for Recipe # and Cycle count already exists? */
				sprintf(SelectQuery,"SELECT COUNT(1) FROM ProductionSetupTable WHERE RecipeNumber = %d AND CycleCount = %d;",
						results.RecipeNumber, results.CycleCounter);
				
				buffer = m_DbConn.Exec(SelectQuery, &queryStatus);
				noOfRecords = atoi(buffer.c_str());

				if(SQLITE_OK == queryStatus)
				{
					/* Updates the production details if not updated before power shutdown */
					if(0 == noOfRecords)
					{
						ProductionDataAnalysis(results.RecipeNumber, results.WeldStatus, results.CycleCounter);
					}
					else
					{
						LOGDBG("DB_T : CheckCriticalData: ------------Weld Result already present in DB. Clearing EEPROM -----------",0,0,0);
					}
				}
				else
				{
					LOGERR("DB_T : Production data select query failed with ErrCode : %d",queryStatus,0,0);
				}
			}
		}
		else
		{
			LOGERR("DB_T : Weld result critical data select query failed with ErrCode : %d",queryStatus,0,0);
		}

		/* Clear the critical data stored in EEPROM */
		memset(&criticalData, 0x00, sizeof(criticalData));
		status = epclass.Write((char *)&criticalData, criticalDatasize, (ssize_t)SHUTDOWN_EEPROM_OFFSET);
		if(ERROR == status)
		{
			LOGERR("DB_T : Error on clearing critical data section in EEPROM",0,0,0);
		}
	}
	else
	{
		LOGDBG("DB_T : No critical data available",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - De-Initialize allocated stack memory of DataBaseManager
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
DataBaseManager::~DataBaseManager()
{
	CP = NULL;
}

/**************************************************************************//**
* \brief   - Managing the weld history data
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBase_Task(void)
{
	char MsgBuffer[MAX_SIZE_OF_MSG_LENGTH] = {0x00};
	DataBaseManager *DBM = new(nothrow) DataBaseManager();
	char signal;
	string details;
	STATUS DBStatus = OK;

#if DEBUG
	LOGDBG("DB_T : ----------------DataBaseTask Started----------------",0,0,0);
#endif

	if(NULL != DBM)
	{
		DBStatus = DBM->ConnectDB();			/* Getting SQLite database connection object */
		//DBM->ConfigureRecipe();		/* Configures default recipes for seek, scan and test */
		
		/*This Function is used to check the critical data saved during shutdown*/
		DBM->CheckCriticalData();

		//Make sure the DB version is up to date
		DBM->VersionCheck();
		
		// read event log related signals from EEPROM
		if (DBStatus == OK)
		{
			// DB deleted event
			DBM->DataBaseManager::GetEventLogNVM (SYSTEM_DB_DELETED, &signal);
			
			if (signal == EVENTLOG_DBDELETED_SIG_ACTIVE)
			{
				// send it to the event log
				details = "Identifier:N/A:From:N/A:To:N/A:";
				DBM->DataBaseManager::EnterEventLog (SYSTEM_DB_DELETED, details);
				signal = EVENTLOG_DBDELETED_SIG_INACTIVE;
				DBM->DataBaseManager::EnterEventLogNVM (SYSTEM_DB_DELETED, &signal);
			}
			
			// software update event
			DBM->DataBaseManager::EnterEventLogSWVersion (FW_INFO_SC);
		}
		
		/* DataBase Task loop and the bIsTaskRun flag enabled when task created */
		while(DBM->bIsTaskRunStatus())
		{
			if(msgQReceive(DBM->GetDBMsgQID(), MsgBuffer, MAX_SIZE_OF_MSG_LENGTH, WAIT_FOREVER) != ERROR)
			{
#ifdef DEBUG
				LOGDBG("DB_T : ------------Message has received------------",0,0,0);
#endif
				if(!DBM->DeCoder(MsgBuffer,strlen(MsgBuffer)))
				{
					LOGERR("DB_T : ------------Message decode has failed------------",0,0,0);
				}
				else
				{
					DBM->ProcessMessage(MsgBuffer);
				}			
			}
			else
			{
				LOGERR("DB_T : ------------Message received failed------------",0,0,0);
			}
		}

		DBM->CloseDB();			/* Close the SQLite database connection object */
		delete DBM;
	}
	else
	{
		LOGCRIT("DB_T: ----------------Memory allocation failed----------------",0,0,0);
	}

	DBM = NULL;
	LOGDBG("DB_T: ----------------DataBaseTask Suspended----------------",0,0,0);
	taskSuspend(taskIdSelf());
}

/**************************************************************************//**
* \brief   - Validates the batch setup based on configured settings. On reaching
* 			 maximum limit, Sets required flag to block subsequent welds. 
*
* \param   - INT32 				- recipeNumber 
* \param   - bool 				- bIsAlarmWeld
* \param   - PRODUCTION_SETTING - mode
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::BatchSetupValidation(INT32 recipeNumber, bool bIsAlarmWeld, PRODUCTION_SETTING mode)
{
	char 	sqlQuery[MSG_SIZE] = {0x00},index=0;
	UINT32	QueryData[4];
	UINT8	status			= RESPONSE_FAIL;
	UINT32 	batchCounter	= 0;
	INT32 	queryStatus		= 0;
	UINT32  count			= 0;
	char 	*token 			= NULL;
	string 	batchData;
	Message message;

	batchData.clear();
	sprintf(sqlQuery,"SELECT BatchSetupEnable, BatchAlarmEnable, BatchCountLimit, BatchCounter FROM ProductionSetupTable WHERE RecipeNumber=%d;",recipeNumber);
	batchData = m_DbConn.Exec(sqlQuery, &queryStatus);
	if(SQLITE_OK != queryStatus)
	{		
		LOGERR("DB_T : BatchSetupValidation ErrCode from DB after SELECT query: %d",queryStatus,0,0);		
	}
	else
	{
#ifdef BATCH_DEBUG
		/* LOGDBG("DB_T: Length of batch count query : %d",batchData.length(),0,0); */
#endif	/* BATCH_DEBUG */
		if(batchData.length() > 0)
		{
			token = strtok((char *)batchData.c_str(), ",");
			while (token != NULL)
			{
				if(index < 4)
				{
					QueryData[index]=atoi(token);
				}
				token = strtok(NULL, ",");
				index++;
			}
			m_batchSetupEnable		= QueryData[0];
			m_batchAlarmEnable		= QueryData[1];
			m_batchCountLimit		= QueryData[2];
			batchCounter			= QueryData[3];
			batchCounter++;		/* Increment from previous counter */

#ifdef BATCH_DEBUG	
			LOGDBG("DB_T : Batch bIsEnable 			: %d",m_batchSetupEnable,0,0);
			LOGDBG("DB_T : Batch bIsAlarmEnable		: %d",m_batchAlarmEnable,0,0);
			LOGDBG("DB_T : Batch Count Limit		: %d",m_batchCountLimit,0,0);
			LOGDBG("DB_T : Batch Counter			: %d",batchCounter,0,0);
#endif	/* BATCH_DEBUG */
			if(m_batchSetupEnable)
			{
				if((!m_batchAlarmEnable) && (bIsAlarmWeld) && (UPDATE_PRODUCTION == mode))
				{
#ifdef BATCH_DEBUG
					LOGDBG("DB_T : ------------Batch setup alarm flag disabled and alarm cycle------------",0,0,0);
#endif	/* BATCH_DEBUG */
				}
				else 
				{
					if(UPDATE_PRODUCTION == mode)
					{
						sprintf(sqlQuery,"UPDATE ProductionSetupTable SET  BatchCounter = BatchCounter + 1 WHERE RecipeNumber = %d;",recipeNumber);
						queryStatus = m_DbConn.BulkTransaction(sqlQuery);
						if(SQLITE_OK != queryStatus)
						{
							LOGERR("DB_T : Error on updating Batch Count data with ErrCode: %d",queryStatus,0,0);
							RetryMechanism(sqlQuery,BULK_TRANSACTION,status);
						}
					}
					else		/* Condition enters when recipe changed and batch setup is enabled */
					{
						ControlTask::ResetLimitExceedAlarm(false);
						CP->m_ScAlarmObj.SetBatchCountAlarmFlag(false);
						batchCounter = batchCounter - 1;
					}
					
					if(batchCounter >= m_batchCountLimit)			/* Blocks the subsequent welds */
					{
						message.msgID = TO_UI_TASK_BATCH_COUNT_EXCEED_CMD;
						sprintf(message.Buffer,"%d",batchCounter);
						SendMsgToMsgQ(message,UI_MSG_Q_ID);
						CP->m_ScAlarmObj.SetBatchCountAlarmFlag(true);
					}
#ifdef BATCH_DEBUG
					memset(SelectQuery,0x00,sizeof(SelectQuery));
					/* TO CHECK BATCH COUNT UPDATED OR NOT */
					sprintf(sqlQuery,"SELECT BatchCounter FROM ProductionSetupTable WHERE RecipeNumber =%d;",recipeNumber);
					batchData = m_DbConn.Exec(sqlQuery, queryStatus);
					count 	  = atoi(batchData.c_str());
					LOGDBG("DB_T : Batch Counter after UPDATE In DB ---------------------------%d",count,0,0);
					memset(SelectQuery,0x00,sizeof(sqlQuery));
					if(SQLITE_OK != queryStatus)
					{
						LOGERR("DB_T : Error on selecting Batch Count data : %d",queryStatus,0,0);
					}
#endif /* BATCH_DEBUG */
				}
			}
			else
			{
				ControlTask::ResetLimitExceedAlarm(false);
				CP->m_ScAlarmObj.SetBatchCountAlarmFlag(false);
#ifdef BATCH_DEBUG
				LOGDBG("DB_T : Batch setup disabled for recipe number : %d",recipeNumber,0,0);
#endif
			}
		}
		else
		{
			ControlTask::ResetLimitExceedAlarm(false);
			CP->m_ScAlarmObj.SetBatchCountAlarmFlag(false);
#ifdef BATCH_DEBUG
			LOGDBG("DB_T : Batch setup data not available for recipe number : %d",recipeNumber,0,0);
#endif
		}
	}
}

/**************************************************************************//**
* \brief   - Resets the Batch Count for given recipeNumber in DB.
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::ResetBatchCount()
{
	char 	queryData[MSG_SIZE] = {0x00};
	INT32 recipeNumber = 0;
	INT32  queryStatus = 0;
	Message message;
	
	recipeNumber = CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM);
	sprintf(queryData,"UPDATE ProductionSetupTable SET BatchCounter = 0 WHERE RecipeNumber = %d;",recipeNumber);
	m_DbConn.Exec(queryData, &queryStatus);
	
	if(SQLITE_OK != queryStatus)
	{
		LOGERR("DB_T : Error on Resetting Batch Count data : %d",queryStatus,0,0);
		message.msgID = TO_UI_TASK_BATCH_COUNT_RESET_RSP;
		sprintf(message.Buffer,"%d",RESPONSE_FAIL);
		SendMsgToMsgQ(message,UI_MSG_Q_ID);	 	 	
	}
	else
	{
		m_batchSetupEnable = 0;
		m_batchAlarmEnable = 0;
		m_batchCountLimit  = 0;
		CP->m_ScAlarmObj.SetBatchCountAlarmFlag(false);
		LOGDBG("DB_T : Batch counter reseted successfully",0,0,0);
		message.msgID = TO_UI_TASK_BATCH_COUNT_RESET_RSP;
		sprintf(message.Buffer,"%d",RESPONSE_SUCCESS);
		SendMsgToMsgQ(message,UI_MSG_Q_ID);	 	 	
	}
}


#ifdef PERFORMANCE_MEASURE
void DataBaseManager::PerformanceMeasureLog()
{
	char query[MSG_SIZE] = {0x00};

	sprintf(query,"INSERT INTO PerformanceMeasure(RecipeNumber,CycleCounter,StartTime,EndTime,TimeDiffInMS,TimeDiffInSec,IntervalPeriodInMS) VALUES(%d,%d,%f,%f,%f,%f,%d);",
			CommonProperty::weldResult.Get(WeldResults::RECIPE_NUM), m_cycleCounter, m_startTime, m_endTime,
			(m_endTime - m_startTime) * 1000, (m_endTime - m_startTime), 0);
	m_DbConn.BulkTransaction(query);
}
#endif

			
/**************************************************************************//**
* \brief   -  Return the current message queue message ID(Log type)
*
* \param   - None
*
* \return  - Message ID(Log type)
*
******************************************************************************/
INT32 DataBaseManager::GetMessageID() const		
{
	return MSG_ID;
}


/**************************************************************************//**
* \brief   -  Checks whether a stored component SW version is valid and does
*			  not contain unrecognized information
*
* \param   - char * swVersion
*
* \return  - bool valid
*
******************************************************************************/
bool DataBaseManager::CheckValidVersion (char* swVersion)
{
#ifdef MAINLINE_BUGFIX_BUILD
	int major, minor, build, autos;
	bool valid = false;
	
	sscanf( swVersion, "%d.%d.%d.%d", &major, &minor, &build, &autos );
	
	if (major != 0xff)
		if (minor != 0xff)
			if (build != 0xff)
				if (autos != 0xff)
					valid = true;
				else
				{
					sprintf( swVersion, "%d.%d.%d", major, minor, build);
					valid = true;
				}
					
#else
	int major, minor, build;
	bool valid = false;
	
	sscanf( swVersion, "%d.%d.%d", &major, &minor, &build );
	
	if (major != 0xff)
		if (minor != 0xff)
			if (build != 0xff)
				valid = true;
	
#endif
	
	return valid;
}


/**************************************************************************//**
* \brief   -  Reads user details from DB and sends to web service task
* 				through message queue
*
* \param   - Message  &message
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetUserDetails(Message &message)
{
	char 	username[USERDATA_LENGTH] = {0x00};
	char 	password[USERDATA_LENGTH] = {0x00};
	Message messageWeb;
	string strSelectQuery;
	string strDBData;
	INT32 dwQueryStatus = 0 , dwIndex = 0;
	char * token = NULL;
	
	if(strlen(message.Buffer) > 0)
	{
		token = strtok((char *)message.Buffer, ",");
		while (token != NULL)
		{
			if(dwIndex == 0)
			{
				strcpy(username,token);
			}
			else if(dwIndex == 1)
			{
				strcpy(password,token);
			}
			token = strtok(NULL, ",");
			dwIndex++;
		}
	}

	strSelectQuery = "SELECT UserId, UserLevel, UserStatus, DateTime, lastChangedPassword,lastLogin from UserProfiles WHERE UserId = '"+ std::string(username) + "'and Password = '"+ std::string(password) +"';";
	strDBData = m_DbConn.Exec((char *)strSelectQuery.c_str(), &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetUserDetails : ErrCode from DB : %d",dwQueryStatus,0,0);
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);;
	}
	else
		strcpy(messageWeb.Buffer,strDBData.c_str());
	/* Send user details to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_USER_DETAILS;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   -  Reads last weld result from DB and sends to web service task
* 			  through message queues 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetlastWeldResult()
{
	string strDBData;
	INT32 dwQueryStatus = 0;
	string strSelectQuery;
	Message WeldResultBuffer;

	strSelectQuery = "select * from WeldResultTable ORDER BY ROWID DESC LIMIT 1";
	strDBData = m_DbConn.Exec((char *)strSelectQuery.c_str(), &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetLastWeldResult : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(WeldResultBuffer.Buffer,DB_QUERY_FAIL);
	}
	else
		strcpy(WeldResultBuffer.Buffer,strDBData.c_str());

	/* Send last weld results to WEBSERVICE_T */
	WeldResultBuffer.msgID = TO_WEB_SERVICES_TASK_LAST_WELD_RESULTS;
	SendMsgToMsgQ(WeldResultBuffer,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   -  Reads numbers of alarm from DB and sends to web service task
* 				through message queues 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetNumberOfAlarms()
{
	string strSelectQuery;
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;

	strSelectQuery = "SELECT count(*) FROM AlarmLogs;";
	strDBData = m_DbConn.Exec((char *)strSelectQuery.c_str(), &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetNumberOfAlarms : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
	}
	else
		strcpy(messageWeb.Buffer,strDBData.c_str());


	/* Send number of alarms to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_NUMBER_OF_ALARMS;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   -  Reads password expiry in days from DB and sends to web service task
* 				through message queues 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetPasswordExpiryInDays()
{
	string strSelectQuery;
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;

	strSelectQuery = "SELECT PASSWORD_EXPIRY_IN_DAYS FROM GlobalSettings;";
	strDBData = m_DbConn.Exec((char *)strSelectQuery.c_str(), &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetPasswordExpiryInDays : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
	}
	else
		strcpy(messageWeb.Buffer,strDBData.c_str());

	/* Send password expiry in days to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_GET_PW_EXPIRY_DAYS;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}
/**************************************************************************//**
 * \brief   -  Reads numbers of weld history from DB and sends to web service task
 * 				through message queues 
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::GetNumberOfWeldHistory(Message &message)
{
	char strSelectQuery[MSG_SIZE] = {0x00};
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;
	int dwRecipeNumber = atoi(message.Buffer);
	
	sprintf(strSelectQuery, "SELECT count(*) FROM WeldResultTable where RecipeNumber = %d;",dwRecipeNumber);
	strDBData = m_DbConn.Exec(strSelectQuery,&dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetNumberOfWeldHistory : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
	}
	else
		strcpy(messageWeb.Buffer,strDBData.c_str());

	/* Send message to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_NUMBER_OF_WELDHISTORY;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
 * \brief   -  Reads idle log out time from DB and sends to web service task
 * 				through message queues 
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::GetIdleLogoutTime()
{
	string strSelectQuery;
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;

	strSelectQuery = "SELECT IDEAL_TIME_LOGOGG_IN_MIN FROM GlobalSettings;";
	strDBData = m_DbConn.Exec((char *)strSelectQuery.c_str(), &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetIdleLogoutTime : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
	}
	else
		strcpy(messageWeb.Buffer,strDBData.c_str());

	/* Send Idle log out time to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_IDLE_LOGOUT_TIME;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
 * \brief   -  Reads numbers of operator authority check value for set active recipe
 *  			  from DB and sends to web service task through message queues 
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::GetOperatorAuthorityPrivileges()
{
	string strSelectQuery;
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;

	strSelectQuery = "SELECT Option3 from OperatorAuthorityOptions;";
	strDBData = m_DbConn.Exec((char *)strSelectQuery.c_str(), &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetOperatorAuthorityPrivileges : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
	}
	else
		strcpy(messageWeb.Buffer,strDBData.c_str());

	/* Send message to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_AUTHORITY_CHECK_OPERATOR;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
 * \brief   -  To set active recipe based on recipe number 
 *
 * \param   - Message &message
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::SetActiveRecipe(Message &message)
{
	int dwRecipeNumberToMakeActive;
	dwRecipeNumberToMakeActive = atoi(message.Buffer);

	char SelectQuery[MSG_SIZE] = {0x00};
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;
	string strVerRecipeNum,strRecipeData,strRecipeNum, strRecipeName;
	INT32	dwRecVeripeNum,dwRecipeNumber,dwStatus;
	string 	strActiveWeldRecipe = "", strSRActiveRecipe = "",strBransonOnly = "",strSetupLimits = "";

	messageWeb.msgID = TO_WEB_SERVICES_TASK_SET_AS_ACTIVE_RECIPE;
	/* Read recipe number from DB to check received recipe is available or not */
	sprintf(SelectQuery,"select RecipeNumber from WeldRecipeTable where RecipeNumber = %d;",dwRecipeNumberToMakeActive);
	strDBData = m_DbConn.Exec(SelectQuery,&dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : SetActiveRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
		/* Send message to WEBSERVICE_T */
		SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
		return;
	}	

	memset(messageWeb.Buffer,0x00,sizeof(messageWeb.Buffer));
	if(!strDBData.empty() && (dwRecipeNumberToMakeActive == atoi(strDBData.c_str())))
	{
		DBInterface::GetRecipeZeroStrings(strActiveWeldRecipe,strSRActiveRecipe,strBransonOnly,strSetupLimits);
		if((!strActiveWeldRecipe.empty()) && (!strSRActiveRecipe.empty()) && 
				(!strBransonOnly.empty()) && (!strSetupLimits.empty()))
		{
			stringstream ssRecipeData(strActiveWeldRecipe.c_str());
			getline (ssRecipeData, strRecipeNum, ',');
			getline (ssRecipeData, strRecipeName, ',');
			getline (ssRecipeData, strVerRecipeNum, ',');

			dwRecipeNumber = atoi(strRecipeNum.c_str());
			dwRecVeripeNum = atoi(strVerRecipeNum.c_str());

			if(dwRecipeNumber == 0 && dwRecVeripeNum != 0)
			{
				sprintf(messageWeb.Buffer,"%d",ACTIVE_RECIPE_NOT_SAVED);
			}
			else
			{
				/* set recipe as active in DB and update local strtucture and also recipe zero with latest active recipe */
				sprintf(SelectQuery,"update WeldRecipeTable SET IsActive=0;");
				strDBData = m_DbConn.Exec(SelectQuery,&dwQueryStatus);
				if(SQLITE_OK != dwQueryStatus)
				{
					LOGERR("DB_T : SetActiveRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
					strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
					/* Send message to WEBSERVICE_T */
					SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
					return;
				}

				sprintf(SelectQuery,"update WeldRecipeTable SET IsActive=1 where RecipeNumber = %d;",dwRecipeNumberToMakeActive);
				strDBData = m_DbConn.Exec(SelectQuery,&dwQueryStatus);
				if(SQLITE_OK != dwQueryStatus)
				{
					LOGERR("DB_T : SetActiveRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
					strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
					/* Send message to WEBSERVICE_T */					
					SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
					return;
				}
				
				//true was added as a parameter to the ReadActiveRecipeFromDB() function 
				//so the new active recipe gets saved on the recipe zero structure
				dwStatus = ReadActiveRecipeFromDB(dwRecipeNumberToMakeActive,true);
				if(dwStatus == 1)
				{
					sprintf(messageWeb.Buffer,"%d",SYSTEM_BUSY);
				}
				else
				{
					sprintf(messageWeb.Buffer,"%d",WS_SUCCESS);
				}
			}
		}
		/* Normally this case never happnes, Untill Developer manually make the variables empty
		 * So for that case sending the error as SYSTEM_BUSY for internal communication Error 
		 * */
		else 
		{
			sprintf(messageWeb.Buffer,"%d",SYSTEM_BUSY);
		}
	}
	else
		sprintf(messageWeb.Buffer,"%d",RECIPE_NOT_FOUND);

	/* Send message to WEBSERVICE_T */
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
 * \brief   -  To delete the recipe based on recipe number 
 *
 * \param   - Message &message
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::DeleteRecipe(Message &message)
{
	int recipeNum;
	recipeNum = atoi(message.Buffer);

	char SelectQuery[MSG_SIZE] = {0x00};
	string strDBData, details;
	INT32 dwQueryStatus = 0;
	Message messageWeb;

	messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_DELETE_RECIPE;
	/* Read recipe number from DB to check received recipe is available or not */
	sprintf(SelectQuery,"select RecipeNumber from WeldRecipeTable where RecipeNumber = %d;",recipeNum);
	strDBData = m_DbConn.Exec(SelectQuery, &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : DeleteRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
		/* Send message to WEBSERVICE_T */
		SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
		return;
	}

	if(!strDBData.empty() && (recipeNum == atoi(strDBData.c_str())))
	{
		sprintf(SelectQuery,"delete from WeldRecipeTable where RecipeNumber = %d;",recipeNum);
		strDBData = m_DbConn.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
		{
			LOGERR("DB_T : DeleteRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
			strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
			/* Send message to WEBSERVICE_T */
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
			return;
		}

		sprintf(SelectQuery,"delete from SuspectRejectLimits where RecipeNumber = %d;",recipeNum);
		strDBData = m_DbConn.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
		{
			LOGERR("DB_T : DeleteRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
			strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
			/* Send message to WEBSERVICE_T */
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
			return;
		}

		sprintf(SelectQuery,"delete from BransonOnlyParameters where RecipeNumber = %d;",recipeNum);
		strDBData = m_DbConn.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
		{
			LOGERR("DB_T : DeleteRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
			strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
			/* Send message to WEBSERVICE_T */
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
			return;
		}

		sprintf(SelectQuery,"delete from SetupLimits where RecipeNumber = %d;",recipeNum);
		strDBData = m_DbConn.Exec(SelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
		{
			LOGERR("DB_T : DeleteRecipe : ErrCode from DB : %d",dwQueryStatus,0,0); 
			strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
			/* Send message to WEBSERVICE_T */
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
			return;
		}

		details = "Identifier:Recipe " + CP->ConvertIntToString(recipeNum) + " Deleted:From:N/A:To:N/A:";
		EventLog (WEB_SERVICES, &details);
		sprintf(messageWeb.Buffer,"%d",WS_SUCCESS);
	}
	else
		sprintf(messageWeb.Buffer,"%d",RECIPE_NOT_FOUND);

	/* Send message to WEBSERVICE_T */
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
 * \brief   -  To save the recipe, if recipe zero available 
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::SaveRecipeZeroToDB()
{
	char DeleteQuery[MSG_SIZE] = {0x00};
	string 	strActiveWeldRecipe = "", strSRActiveRecipe = "",strBransonOnly = "",strSetupLimits = "";	
	string strSelectQuery, strDBData, strRecipeNum, strVerRecipeNum, strRecipeData, strRecipeName, details;
	INT32 dwQueryStatus = 0, dwRecipeNum, dwRecipeVerNum,dwStatus;
	Message messageWeb;

	messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_SAVE_RECIPE;

	/* Get recipe zero */
	DBInterface::GetRecipeZeroStrings(strActiveWeldRecipe,strSRActiveRecipe,strBransonOnly,strSetupLimits);
	if((!strActiveWeldRecipe.empty()) && (!strSRActiveRecipe.empty()) && 
			(!strBransonOnly.empty()) && (!strSetupLimits.empty()))
	{
		/* Extracting recipe number and version number to check weather recipe zero is having valid recipe */
		stringstream ssRecipeData(strActiveWeldRecipe.c_str());
		
		getline (ssRecipeData, strRecipeNum, ',');
		getline (ssRecipeData, strRecipeName, ',');
		getline (ssRecipeData, strVerRecipeNum, ',');
		getline (ssRecipeData, strRecipeData);
		
		dwRecipeNum = atoi(strRecipeNum.c_str());
		dwRecipeVerNum = atoi(strVerRecipeNum.c_str());


		if(dwRecipeNum == 0 && dwRecipeVerNum != 0)
		{
			dwRecipeNum = dwRecipeVerNum;
			strRecipeNum = strVerRecipeNum;
			dwRecipeVerNum = CP->m_RecipeSC.GetRecipeRevNumber()+1;
			stringstream ss;
			ss << dwRecipeVerNum;
			strVerRecipeNum = ss.str();

			/* Preaparing new string for active weld recipe to store in DB */
			strActiveWeldRecipe.clear();
			strActiveWeldRecipe.append(strRecipeNum);
			strActiveWeldRecipe.append(",");
			strActiveWeldRecipe.append(strRecipeName);
			strActiveWeldRecipe.append(",");
			strActiveWeldRecipe.append(strVerRecipeNum);
			strActiveWeldRecipe.append(",");
			strActiveWeldRecipe.append(strRecipeData);

			/* Deleting available WeldRecipeTable and inserting again with new data */
			sprintf(DeleteQuery,"delete from WeldRecipeTable where RecipeNumber = %d;",dwRecipeNum);
			strDBData = m_DbConn.Exec(DeleteQuery, &dwQueryStatus);		
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB WeldRecipeTable delete: ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}

			strSelectQuery = "Insert into WeldRecipeTable values(" + strActiveWeldRecipe + ");";
			strDBData = m_DbConn.Exec((char*)strSelectQuery.c_str(), &dwQueryStatus);
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB WeldRecipeTable Insert: ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}

			/* Preaparing new string for active suspect reject recipe to store in DB */
			stringstream ssSRRecipeData(strSRActiveRecipe.c_str());
			
			getline (ssSRRecipeData, strRecipeData, ',');
			getline (ssSRRecipeData, strRecipeData, ',');
			getline (ssSRRecipeData, strRecipeData);
			
			strSRActiveRecipe.clear();
			strSRActiveRecipe.append(strRecipeNum);
			strSRActiveRecipe.append(",");
			strSRActiveRecipe.append(strVerRecipeNum);
			strSRActiveRecipe.append(",");
			strSRActiveRecipe.append(strRecipeData);

			/* Deleting available SuspectRejectLimits and inserting again with new data */
			sprintf(DeleteQuery,"delete from SuspectRejectLimits where RecipeNumber = %d;",dwRecipeNum);
			strDBData = m_DbConn.Exec(DeleteQuery, &dwQueryStatus);	
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB SuspectRejectLimits delete: ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}

			strSelectQuery = "Insert into SuspectRejectLimits values(" + strSRActiveRecipe + ");";
			strDBData = m_DbConn.Exec((char*)strSelectQuery.c_str(), &dwQueryStatus);
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB SuspectRejectLimits Insert: ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}

			/* Preaparing new string for active strBransonOnlyParameters recipe to store in DB */
			stringstream ssBransonOnlyRecipeData(strBransonOnly.c_str());
			
			getline (ssBransonOnlyRecipeData, strRecipeData, ',');
			getline (ssBransonOnlyRecipeData, strRecipeData, ',');
			getline (ssBransonOnlyRecipeData, strRecipeData);

			strBransonOnly.clear();
			strBransonOnly.append(strRecipeNum);
			strBransonOnly.append(",");
			strBransonOnly.append(strVerRecipeNum);
			strBransonOnly.append(",");
			strBransonOnly.append(strRecipeData);

			/* Deleting available BransonOnlyParameters and inserting again with new data */
			sprintf(DeleteQuery,"delete from BransonOnlyParameters where RecipeNumber = %d;",dwRecipeNum);
			strDBData = m_DbConn.Exec(DeleteQuery, &dwQueryStatus);		
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB BransonOnlyParameters delete: ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}

			strSelectQuery = "Insert into BransonOnlyParameters values(" + strBransonOnly + ");";
			strDBData = m_DbConn.Exec((char*)strSelectQuery.c_str(), &dwQueryStatus);
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB BransonOnlyParameters Insert : ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}

			/* Preaparing new string for active SetupLimits recipe to store in DB */
			stringstream ssSetUpRecipeData(strSetupLimits.c_str());
			
			getline (ssSetUpRecipeData, strRecipeData, ',');
			getline (ssSetUpRecipeData, strRecipeData, ',');
			getline (ssSetUpRecipeData, strRecipeData);

			strSetupLimits.clear();
			strSetupLimits.append(strRecipeNum);
			strSetupLimits.append(",");
			strSetupLimits.append(strVerRecipeNum);
			strSetupLimits.append(",");
			strSetupLimits.append(strRecipeData);

			/* Deleting available SetupLimits and inserting again with new data */
			sprintf(DeleteQuery,"delete from SetupLimits where RecipeNumber = %d;",dwRecipeNum);
			strDBData = m_DbConn.Exec(DeleteQuery, &dwQueryStatus);		
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB SetupLimits delete: ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}

			strSelectQuery = "Insert into SetupLimits values(" + strSetupLimits + ");";
			strDBData = m_DbConn.Exec((char*)strSelectQuery.c_str(), &dwQueryStatus);
			if(SQLITE_OK != dwQueryStatus)
			{
				LOGERR("DB_T : SaveRecipeZeroToDB SetupLimits Insert : ErrCode from DB : %d",dwQueryStatus,0,0);
				strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
				/* Send message to WEBSERVICE_T */
				SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
				return;
			}		
		}
	}
	dwStatus = ReadActiveRecipeFromDB(1,true);
	if(dwStatus == 1)
	{
		sprintf(messageWeb.Buffer,"%d",SYSTEM_BUSY);
	}
	else
	{
    	details = "Identifier:Recipe " + m_DbConn.Exec("select RecipeNumber from WeldRecipeTable where IsActive = 1 and RecipeNumber>0;") + " Saved:From:N/A:To:N/A:";
    	DataBaseManager::EnterEventLog (WEB_SERVICES, details);

		sprintf(messageWeb.Buffer,"%d",WS_SUCCESS);
	}

	/* Send message to WEBSERVICE_T */
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
 * \brief   -  Prepares JSON Format string for Alarm data. Output to
 * 				match the PDF output. 
 *				1: Date/TIme
 *				2: Recipe #
 *				3. Recipe ver
 *				4. alarm ID
 *				5. user ID
 *				6. cycle counter
 *
 * \param   - string &strJSONFormat,string &strDBData
 *
 * \return  - None
 *
 ******************************************************************************/

void DataBaseManager::AlarmLogsJSONFormat(string &strJSONFormat,string &strDBData)
{
	char tempBuffer[USERDATA_LENGTH];
	vector <string>vecAlarmLogs;
	string item;
	sprintf(tempBuffer,"{%s:%d,\"AlarmData\":[",STATUS_CODE,WS_SUCCESS);
	strJSONFormat.append(tempBuffer);
	if(strDBData.size() > 0)
	{
		stringstream sstream (strDBData.c_str());
		int dwEntireDataIndex = 0,dwSingleRecordDataIndex = 0;
		while(getline(sstream,item,','))
			vecAlarmLogs.push_back(item);

		int dwIndex = 0,dwCounter = 0;
		for(dwEntireDataIndex = 0;dwEntireDataIndex < vecAlarmLogs.size()/NUMBEROF_ALARMLOG_PARAMETERS;dwEntireDataIndex++ )
		{
			int dwNumberInJSONFormat = 1;
			strJSONFormat.append("{");
			dwIndex = dwCounter*NUMBEROF_ALARMLOG_PARAMETERS;
			
			for(dwSingleRecordDataIndex = 0 ; dwSingleRecordDataIndex < NUMBEROF_ALARMLOG_PARAMETERS - 1 ; dwSingleRecordDataIndex++)
			{
				if(((dwIndex % NUMBEROF_ALARMLOG_PARAMETERS) == ALARMLOG_ALARM_ID) || ((dwIndex % NUMBEROF_ALARMLOG_PARAMETERS) == ALARMLOG_USERNAME) ||
						((dwIndex % NUMBEROF_ALARMLOG_PARAMETERS) == ALARMLOG_DATETIME))
					snprintf(tempBuffer, USERDATA_LENGTH, "\"%d\":\"%s\",",dwNumberInJSONFormat,vecAlarmLogs[dwIndex].c_str());
				else
					snprintf(tempBuffer, USERDATA_LENGTH, "\"%d\":%s,",dwNumberInJSONFormat,vecAlarmLogs[dwIndex].c_str());
				
				strJSONFormat.append(tempBuffer);
				dwNumberInJSONFormat++;
				dwIndex++;
			}
			snprintf(tempBuffer, USERDATA_LENGTH, "\"%d\":%s",dwNumberInJSONFormat,vecAlarmLogs[dwIndex].c_str());
			strJSONFormat.append(tempBuffer);

			dwCounter++;
			strJSONFormat.append("}");
			if(dwEntireDataIndex != (vecAlarmLogs.size()/NUMBEROF_ALARMLOG_PARAMETERS) - 1)
				strJSONFormat.append(",");
		}
	}
	strJSONFormat.append("]}");
}

/**************************************************************************//**
 * \brief   -  Reads alarm data between particular ROWID's from DB and Sends to 
 * 				Webservice task through message queue
 *
 * \param   - Message &message
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::GetAlarmLog(Message &message)
{
	char strSelectQuery[MSG_SIZE] = {0x00};
	string strDBData,strFrom,strTo,strJSONFormat;
	INT32 dwQueryStatus = 0, dwFrom = 0,dwTo = 0;
	Message messageWeb;
	char *pTemp = message.Buffer;
	char *pData;

	if(pTemp != NULL)
	{
		pData = strstr(pTemp,",");

		strFrom.append(pTemp,pData - pTemp);
		dwFrom = atoi(strFrom.c_str());
		strTo = pData + 1;
		dwTo = atoi(strTo.c_str());

		if(dwFrom == 0 && dwTo == 0)
		{
			sprintf(strSelectQuery, "SELECT DateTime,RecipeNumber,RecipeVerNumber,AlarmID,UserName,CycleCounter FROM AlarmLogs ORDER BY ROWID DESC LIMIT %d;", WS_MAX_GET_ALARM_HISTORY);
		}
		else
		{
			sprintf(strSelectQuery, "SELECT DateTime,RecipeNumber,RecipeVerNumber,AlarmID,UserName,CycleCounter FROM AlarmLogs where ROWID between %d and %d;",dwFrom,dwTo);
		}
		
		strDBData = m_DbConn.Exec((char *)strSelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
		{
			LOGERR("DB_T : GetAlarmLog : ErrCode from DB : %d",dwQueryStatus,0,0); 
			messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_ALARM_LOG_FINISH;
			strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
			return;
		}

		/* Framing result into JSON format */
		AlarmLogsJSONFormat(strJSONFormat,strDBData);

		/* Send message to WEBSERVICE_T */
		int dwBufferSize = strJSONFormat.size();
		int dwBytesSent = 0;
		char *chBuffer = (char*)strJSONFormat.c_str();
		
		while(dwBytesSent < dwBufferSize)
		{
			int dwBytesToSend;
			if((dwBufferSize - dwBytesSent) > (MAX_SIZE_OF_MSG_BUFF - 1))
			{
				messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_ALARM_LOG_NEXT;
				dwBytesToSend = MAX_SIZE_OF_MSG_BUFF - 1; //1 is subtracted to leave the last element for the end of string.
			}
			else
			{
				messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_ALARM_LOG_FINISH;
				dwBytesToSend = dwBufferSize - dwBytesSent;
			}

			memset(messageWeb.Buffer,0x00,sizeof(messageWeb.Buffer));
			memcpy(messageWeb.Buffer,chBuffer+dwBytesSent,dwBytesToSend);
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);

			dwBytesSent = dwBytesSent + dwBytesToSend;
		}
	}
	else
	{
		messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_ALARM_LOG_FINISH;
		sprintf(messageWeb.Buffer,"%d",INVALID_JSON_FORMAT);
		SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
	}
}

/**************************************************************************//**
 * \brief   -  Prepares JSON Format string for Weld history
 *
 * \param   - string &strJSONFormat,string &strDBData
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::WeldHistoryJSONFormat(string &strJSONFormat,string &strDBData)
{
	char tempBuffer[USERDATA_LENGTH];
	vector <string>vecWeldData;
	string item;
	sprintf(tempBuffer,"{%s:%d,\"WeldData\":[",STATUS_CODE,WS_SUCCESS);
	strJSONFormat.append(tempBuffer);
	if(strDBData.size() > 0)
	{
		stringstream sstream (strDBData.c_str());
		int dwEntireDataIndex = 0,dwSingleRecordDataIndex = 0;
		while(getline(sstream,item,','))
			vecWeldData.push_back(item);

		int dwIndex = 0,dwCounter = 0;
		for(dwEntireDataIndex = 0;dwEntireDataIndex < vecWeldData.size()/NUMBEROF_WELDHISTORY_PARAMETERS;dwEntireDataIndex++ )
		{
			int dwNumberInJSONFormat = 1;
			strJSONFormat.append("{");
			dwIndex = dwCounter * NUMBEROF_WELDHISTORY_PARAMETERS;
			for(dwSingleRecordDataIndex = 0 ; dwSingleRecordDataIndex < NUMBEROF_WELDHISTORY_PARAMETERS - 1 ; dwSingleRecordDataIndex++)
			{
				sprintf(tempBuffer,"\"%d\":%s,",dwNumberInJSONFormat,vecWeldData[dwIndex].c_str());
				strJSONFormat.append(tempBuffer);
				dwNumberInJSONFormat++;
				dwIndex++;
			}
			sprintf(tempBuffer,"\"%d\":%s",dwNumberInJSONFormat,vecWeldData[dwIndex].c_str());
			strJSONFormat.append(tempBuffer);

			dwCounter++;
			strJSONFormat.append("}");
			if(dwEntireDataIndex != (vecWeldData.size()/NUMBEROF_WELDHISTORY_PARAMETERS) - 1)
				strJSONFormat.append(",");
		}
	}
	strJSONFormat.append("]}");		
}

/**************************************************************************//**
 * \brief   -  Reads weld history for particular recipe number and between cycle 
 * 			   counter from DB and Sends to	Webservice task through message queue
 *
 * \param   - Message &message
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::GetWeldHistory(Message &message)
{
	char strSelectQuery[MSG_SIZE] = {0x00};
	string strDBData,strFrom,strTo,strMsgData,strRecipeNum,strJSONFormat;
	INT32 dwQueryStatus = 0;
	Message messageWeb;
	char *pTemp = message.Buffer;
	char *pData;
	int dwFrom,dwTo,dwRecipeNumber;

	if(pTemp != NULL && sizeof(pTemp) > 0)
	{
		pData = strstr(pTemp,",");
		strFrom.append(pTemp,pData - pTemp);
		dwFrom = atoi(strFrom.c_str());
		strMsgData.append(pData+1);
		pTemp = (char*)strMsgData.c_str();

		pData = strstr(pTemp,",");
		strTo.append(pTemp,pData - pTemp);
		dwTo = atoi(strTo.c_str());

		strRecipeNum.append(pData + 1);
		dwRecipeNumber = atoi(strRecipeNum.c_str());

		if(dwFrom == 0 && dwTo == 0)
		{
			if (dwRecipeNumber > 0)
				sprintf(strSelectQuery, "SELECT * FROM WeldResultTable where RecipeNumber = %d ORDER BY ROWID DESC LIMIT %d;",dwRecipeNumber, WS_MAX_GET_WELD_HISTORY);
			else
				sprintf(strSelectQuery, "SELECT * FROM WeldResultTable ORDER BY ROWID DESC LIMIT %d;", WS_MAX_GET_WELD_HISTORY);
		}
		else
		{
			if (dwRecipeNumber > 0)
				sprintf(strSelectQuery, "SELECT * FROM WeldResultTable where RecipeNumber = %d and CycleCounter between %d and %d;",dwRecipeNumber,dwFrom,dwTo);
			else
				sprintf(strSelectQuery, "SELECT * FROM WeldResultTable ORDER BY ROWID DESC LIMIT %d;", WS_MAX_GET_WELD_HISTORY);
		}
		
		strDBData = m_DbConn.Exec((char *)strSelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
		{
			LOGERR("DB_T : GetWeldHistory : ErrCode from DB : %d",dwQueryStatus,0,0); 
			messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_WELD_HISTORY_FINISH;
			strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
			return;
		}

		/* Framing result into JSON format */
		WeldHistoryJSONFormat(strJSONFormat,strDBData);

		/* Send message to WEBSERVICE_T */
		int dwBufferSize = strJSONFormat.size();
		int dwBytesSent = 0;
		char *chBuffer = (char*)strJSONFormat.c_str();
		while(dwBytesSent < dwBufferSize)
		{
			int dwBytesToSend;
			if((dwBufferSize - dwBytesSent) > MAX_SIZE_OF_MSG_BUFF)
			{
				messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_WELD_HISTORY_NEXT;
				dwBytesToSend = MAX_SIZE_OF_MSG_BUFF;
			}
			else
			{
				messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_WELD_HISTORY_FINISH;
				dwBytesToSend = dwBufferSize - dwBytesSent;
			}
			memset(messageWeb.Buffer,0x00,sizeof(messageWeb.Buffer));
			memcpy(messageWeb.Buffer,chBuffer+dwBytesSent,dwBytesToSend);
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);

			dwBytesSent = dwBytesSent + dwBytesToSend;
		}
	}
	else
	{
		messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_WELD_HISTORY_FINISH;
		sprintf(messageWeb.Buffer,"%d",INVALID_JSON_FORMAT);
		SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
	}
}

/**************************************************************************//**
 * \brief   -  Prepares JSON Format string for Weld graph results
 *
 * \param   - string &strJSONFormat,string &strDBData,INT32 dwCycleCounter,INT32 dwRecipeNo
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::WeldGraphJSONFormat(string &strJSONFormat,string &strDBData,INT32 dwCycleCounter,INT32 dwRecipeNo)
{
	char tempBuffer[USERDATA_LENGTH];
	vector <string>vecWeldData,VecTime,vecFreq,vecPower,vecCurrent,vecAmp,vecPhase,vecEnergy,vecForce,vecVelocity,vecAD,vecCD;
	string item;
	sprintf(tempBuffer,"{%s:%d,",STATUS_CODE,WS_SUCCESS);
	strJSONFormat.append(tempBuffer);
	int dwEntireDataIndex = 0,dwSingleRecordDataIndex = 0;
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
				sprintf(tempBuffer,"%s,",VecTime[dwSingleRecordDataIndex].c_str());
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
				sprintf(tempBuffer,"%s,",vecForce[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecForce[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"Velocity\":[");
	if(vecVelocity.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecVelocity.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecVelocity.size() - 1)
				sprintf(tempBuffer,"%s,",vecVelocity[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecVelocity[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"AbsDistance\":[");
	if(vecAD.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecAD.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecAD.size() - 1)
				sprintf(tempBuffer,"%s,",vecAD[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecAD[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("],\"ColDistance\":[");
	if(vecCD.size() > 0)
	{
		for(dwSingleRecordDataIndex = 0;dwSingleRecordDataIndex<vecCD.size();dwSingleRecordDataIndex++)
		{
			if(dwSingleRecordDataIndex != vecCD.size() - 1)
				sprintf(tempBuffer,"%s,",vecCD[dwSingleRecordDataIndex].c_str());
			else
				sprintf(tempBuffer,"%s",vecCD[dwSingleRecordDataIndex].c_str());

			strJSONFormat.append(tempBuffer);
		}
	}
	strJSONFormat.append("]}");
}

/**************************************************************************//**
 * \brief   -  Reads weld Graph results for particular recipe number and cycle 
 * 			   counter from DB and Sends to	Webservice task through message queue
 *
 * \param   - Message &message
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::GetGraphResult(Message &message)
{
	char strSelectQuery[MSG_SIZE] = {0x00};
	string strDBData,strCycleCounter, strRecipeNo,strJSONFormat;
	INT32 dwQueryStatus = 0;
	Message messageWeb;
	char *pTemp = message.Buffer;
	char *pData;
	int dwCycleCounter,dwRecipeNo;

	if(pTemp != NULL && sizeof(pTemp) > 0)
	{
		pData = strstr(pTemp,",");

		strCycleCounter.append(pTemp,pData - pTemp);
		dwCycleCounter = atoi(strCycleCounter.c_str());
		strRecipeNo = pData + 1;
		dwRecipeNo = atoi(strRecipeNo.c_str());

		sprintf(strSelectQuery, "SELECT * FROM WeldResultSignature where CycleCounter=%d and RecipeNumber=%d;",dwCycleCounter,dwRecipeNo);	
		strDBData = m_DbConn.Exec((char *)strSelectQuery, &dwQueryStatus);
		if(SQLITE_OK != dwQueryStatus)
		{
			LOGERR("DB_T : GetGraphResult : ErrCode from DB : %d",dwQueryStatus,0,0); 
			messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_GRAPH_RESULTS_FINISH;
			strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
			return;
		}

		/* Framing result into JSON format */
		WeldGraphJSONFormat(strJSONFormat,strDBData,dwCycleCounter,dwRecipeNo);

		/* Send message to WEBSERVICE_T */
		int dwBufferSize = strJSONFormat.size();
		int dwBytesSent = 0;
		char *chBuffer = (char*)strJSONFormat.c_str();
		while(dwBytesSent < dwBufferSize)
		{
			int dwBytesToSend;
			if((dwBufferSize - dwBytesSent) > MAX_SIZE_OF_MSG_BUFF)
			{
				messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_GRAPH_RESULTS_NEXT;
				dwBytesToSend = MAX_SIZE_OF_MSG_BUFF;
			}
			else
			{
				messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_GRAPH_RESULTS_FINISH;
				dwBytesToSend = dwBufferSize - dwBytesSent;
			}

			memset(messageWeb.Buffer,0x00,sizeof(messageWeb.Buffer));
			memcpy(messageWeb.Buffer,chBuffer+dwBytesSent,dwBytesToSend);
			SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);

			dwBytesSent = dwBytesSent + dwBytesToSend;
		}
	}
	else
	{
		messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_GRAPH_RESULTS_FINISH;
		sprintf(messageWeb.Buffer,"%d",INVALID_JSON_FORMAT);
		SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
	}
}

/**************************************************************************//**
 * \brief   -  Reads Runtime details from the DB and return them to Web Service task
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void DataBaseManager::GetRuntimeDetailsOfWebservices()
{
	string strSelectQuery;
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;
	RunTime RT;

	RT.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);

	if(DISABLE == RT.ModelFeature.RtFeature1.WebServices_Flag_B20)
	{
		/*Indicating both Web Services and Firmware upgrade features are disable */
		sprintf(messageWeb.Buffer,"0,0");
	}
	else
	{
		Eeprom epclass;
		char chData[WEBSERVICE_FOE_ENABLE_SIZE+1]={0};

		epclass.Read(chData,WEBSERVICE_FOE_ENABLE_SIZE,WEBSERVICE_FOE_ENABLE_OFFSET);

		strcpy(messageWeb.Buffer,chData);
	}
	/* Send message to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_WS_GET_RUNTIME_DETAILS;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   -  Reads all the user details in the DB and sent to Kernel for the validation purpose
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetAllUserDetails()
{
	string strSelectQuery;
	string strDBData;
	INT32 dwQueryStatus = 0;
	Message messageWeb;

	strSelectQuery = "SELECT UserID, Password, UserLevel from UserProfiles;";
	strDBData = m_DbConn.Exec((char *)strSelectQuery.c_str(), &dwQueryStatus);
	if(SQLITE_OK != dwQueryStatus)
	{
		LOGERR("DB_T : GetNumberOfAlarms : ErrCode from DB : %d",dwQueryStatus,0,0); 
		strcpy(messageWeb.Buffer,DB_QUERY_FAIL);
	}
	else
		strcpy(messageWeb.Buffer,strDBData.c_str());


	/* Send number of alarms to WEBSERVICE_T */
	messageWeb.msgID = TO_WEB_SERVICES_TASK_GET_ALL_USER_DETAILS;
	SendMsgToMsgQ(messageWeb,WEBSERVICE_MSG_Q_ID);
}

/**************************************************************************//**
* \brief   - Gets the IO configuration from DB
*      		
* \param   - NONE
*
* \return  - NONE
*
******************************************************************************/
void DataBaseManager::GetUserIOConfig(Message &message)
{
	string strQuery, strResponse;
    
	strQuery = "select PinFunction,ActiveStatus,CheckStatus from UserIO";
	strResponse = m_DbConn.Exec((char*)strQuery.c_str());
    
    memset(message.Buffer, 0x00, sizeof(message.Buffer));
    strcpy(message.Buffer, strResponse.c_str());
}
