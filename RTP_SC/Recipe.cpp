/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Recipe default, min and max values validation and checks with other parameters   

 **********************************************************************************************************/
#include "Recipe.h"
#include <vector>
#include <sstream>
#include  "RunTimeFeature.h"
#include "UIDBInterface.h"
#include <string>
#include <cmath>
RunTime RT;

/* static initializations */
Recipe* Recipe::m_RObj	= NULL;

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - Recipe* Object 
*
******************************************************************************/
Recipe* Recipe::getInstance()
{
	return m_RObj ? m_RObj : (m_RObj = new Recipe());
}

/**************************************************************************//**
* \brief   - Constructor - Read default recipe and load into MAP.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
Recipe::Recipe()
{
	CP = CommonProperty::getInstance();
	/* Getting system Details for the system type frequency*/
	intf_SysConfig_t  sysConfig;
	CP->GetSysConfContext(sysConfig);
	
	dwSystemFrequnecyType 	= sysConfig.PcConfig.PSFreq;
	UI_MSG_Q_ID 			= CP->GetMsgQId(cTaskName[UI_T]);
	
	CreateRecipeParamaterStructure();
	//if system is ASX
	if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
	{
		AddASXRecipeParamaterToStructure();
	}else
	{
		AddGSXE1RecipeParamaterToStructure(); 		
	}
	RT.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);
}

/**************************************************************************//**
 * \brief  - Destructor
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
Recipe::~Recipe()
{
	CP = NULL;
}

/**************************************************************************//**
 * \brief  - Stores all recipe parameters data in map
 *
 * \param  - string strMetricUnit,
		string str20KDefaultValue, string str20KMinValue, string str20KMaxValue,
		string str30KDefaultValue, string str30KMinValue, string str30KMaxValue,
		string str40KDefaultValue, string str40KMinValue, string str40KMaxValue,
		int dwDBIndex, int dwIndex
 *
 * \return  - None
 *
 ******************************************************************************/
void Recipe::AddNewParamToMap(string strMetricUnit,
		string str20KDefaultValue, string str20KMinValue, string str20KMaxValue,
		string str30KDefaultValue, string str30KMinValue, string str30KMaxValue,
		string str40KDefaultValue, string str40KMinValue, string str40KMaxValue,
		int dwDBIndex, int dwIndex)
{
	stRecipeParmInfo stTempParam;
	stTempParam.dwIndexInDB								= dwDBIndex;
	stTempParam.ParamUnit                      			= strMetricUnit;
	stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ]    = str20KDefaultValue;
	stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ]        = str20KMinValue;
	stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ]        = str20KMaxValue;
	stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ]    = str30KDefaultValue;
	stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ]        = str30KMinValue;
	stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ]        = str30KMaxValue;
	stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ]    = str40KDefaultValue;
	stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ]        = str40KMinValue;
	stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ]        = str40KMaxValue;
	m_MapRecipeParams[dwIndex]                    		= stTempParam;
}

/**************************************************************************//**
 * \brief  - Adding ASX specific parameters of recipe
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
void Recipe::AddASXRecipeParamaterToStructure()
{
	/*              SIUnit  20_DEF  20_Min  20_MAX  30_DEF  30_MIN  30_MAX  40_DEF  40_MIN  40_MAX  	Name        			Index */
	AddNewParamToMap("",     "1",    "1",    "6",   "1",    "1",    "6",    "1",   "1",    "6",    WR_DB_WELD_MODE, WELD_MODE);
	AddNewParamToMap("",     "1",    "1",    "3",   "1",    "1",    "3",   "1",    "1",    "3",   WR_DB_NUM_FORCE_STEPS, NUM_FORCE_STEPS);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_TRIGGER_FORCE, TRIGGER_FORCE);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000", WR_DB_HOLD_FORCE, HOLD_FORCE);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP1, FORCE_STEP1);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP2, FORCE_STEP2);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP3, FORCE_STEP3);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP4, FORCE_STEP4);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP5, FORCE_STEP5);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP6, FORCE_STEP6);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP7, FORCE_STEP7);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP8, FORCE_STEP8);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP9, FORCE_STEP9);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000",  WR_DB_FORCE_STEP10, FORCE_STEP10);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",  "22",   "2000", SR_DB_SUSPECT_ENDWELDFORCE_LOW_VALUE, SUSPECT_ENDWELDFORCE_LOW_VALUE);
	AddNewParamToMap("N",    "2000", "44",   "2000", "2000", "22",   "2000", "2000", "22",   "2000", SR_DB_SUSPECT_ENDWELDFORCE_HIGH_VALUE, SUSPECT_ENDWELDFORCE_HIGH_VALUE);
	AddNewParamToMap("N",    "44",   "44",   "2000", "22",   "22",   "2000", "22",   "22",   "2000", SR_DB_REJECT_ENDWELDFORCE_LOW_VALUE, REJECT_ENDWELDFORCE_LOW_VALUE);
	AddNewParamToMap("N",    "2000", "44",   "2000", "2000", "22",   "2000", "2000", "22",   "2000", SR_DB_REJECT_ENDWELDFORCE_HIGH_VALUE, REJECT_ENDWELDFORCE_HIGH_VALUE);
}

/**************************************************************************//**
 * \brief  - Adding default parameters of recipe
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
void Recipe::AddGSXE1RecipeParamaterToStructure()
{
	/*              SIUnit  20_DEF  20_Min  20_MAX  30_DEF  30_MIN  30_MAX  40_DEF  40_MIN  40_MAX  	Name        			Index */
	AddNewParamToMap("",     "1",    "1",    "7",    "1",    "1",    "7",    "1",    "1",    "7",    WR_DB_WELD_MODE, WELD_MODE);
	AddNewParamToMap("",     "1",    "1",    "10",   "1",    "1",    "10",   "1",    "1",    "10",   WR_DB_NUM_FORCE_STEPS, NUM_FORCE_STEPS);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP1, FORCE_STEP1);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP2, FORCE_STEP2);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP3, FORCE_STEP3);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP4, FORCE_STEP4);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP5, FORCE_STEP5);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP6, FORCE_STEP6);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP7, FORCE_STEP7);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP8, FORCE_STEP8);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP9, FORCE_STEP9);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_FORCE_STEP10, FORCE_STEP10);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE4, FORCE_STEP_VALUE4);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE5, FORCE_STEP_VALUE5);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE6, FORCE_STEP_VALUE6);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE7, FORCE_STEP_VALUE7);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE8, FORCE_STEP_VALUE8);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE9, FORCE_STEP_VALUE9);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE10, FORCE_STEP_VALUE10);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE4, FORCE_STEP_RAMP_VALUE4);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE5, FORCE_STEP_RAMP_VALUE5);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE6, FORCE_STEP_RAMP_VALUE6);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE7, FORCE_STEP_RAMP_VALUE7);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE8, FORCE_STEP_RAMP_VALUE8);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE9, FORCE_STEP_RAMP_VALUE9);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE10, FORCE_STEP_RAMP_VALUE10);
	AddNewParamToMap("N",    "25",   "5",    "2500", "25",   "5",    "2500", "25",   "5",    "2500", WR_DB_TRIGGER_FORCE, TRIGGER_FORCE);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", WR_DB_HOLD_FORCE, HOLD_FORCE);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", SR_DB_SUSPECT_ENDWELDFORCE_LOW_VALUE, SUSPECT_ENDWELDFORCE_LOW_VALUE);
	AddNewParamToMap("N",    "2500", "25",   "2500", "2500", "25",   "2500", "2500", "25",   "2500", SR_DB_SUSPECT_ENDWELDFORCE_HIGH_VALUE, SUSPECT_ENDWELDFORCE_HIGH_VALUE);
	AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", SR_DB_REJECT_ENDWELDFORCE_LOW_VALUE, REJECT_ENDWELDFORCE_LOW_VALUE);
	AddNewParamToMap("N",    "2500", "25",   "2500", "2500", "25",   "2500", "2500", "25",   "2500", SR_DB_REJECT_ENDWELDFORCE_HIGH_VALUE, REJECT_ENDWELDFORCE_HIGH_VALUE);
}

/**************************************************************************//**
 * \brief  - Adding default values for all the parameters of recipe
 *
 * \param  - None
 *
 * \return  - None
 *
 ******************************************************************************/
void Recipe::CreateRecipeParamaterStructure()
{
	m_MapRecipeParams.clear();
	/*              SIUnit  20_DEF  20_Min  20_MAX  30_DEF  30_MIN  30_MAX  40_DEF  40_MIN  40_MAX  Name        Index */

	AddNewParamToMap("s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_MODE_VALUE, MODE_VALUE);
	AddNewParamToMap("",     "1",    "1",    "10",   "1",    "1",    "10",   "1",    "1",    "10",   WR_DB_AMPLITUDE_STEPS_CNT, AMPLITUDESTEPSCNT);
	AddNewParamToMap("",     "1",    "1",    "7",    "1",    "1",    "7",    "1",    "1",    "7",    WR_DB_AMPLITUDE_STEP_AT, AMPLITUDE_STEP_AT);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE1, AMPLITUDE_STEP_VALUE1);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE2, AMPLITUDE_STEP_VALUE2);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE3, AMPLITUDE_STEP_VALUE3);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE4, AMPLITUDE_STEP_VALUE4);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE5, AMPLITUDE_STEP_VALUE5);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE6, AMPLITUDE_STEP_VALUE6);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE7, AMPLITUDE_STEP_VALUE7);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE8, AMPLITUDE_STEP_VALUE8);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE9, AMPLITUDE_STEP_VALUE9);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_AMPLITUDE_STEP_VALUE10, AMPLITUDE_STEP_VALUE10);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP1, AMPLITUDE_STEP1);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP2, AMPLITUDE_STEP2);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP3, AMPLITUDE_STEP3);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP4, AMPLITUDE_STEP4);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP5, AMPLITUDE_STEP5);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP6, AMPLITUDE_STEP6);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP7, AMPLITUDE_STEP7);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP8, AMPLITUDE_STEP8);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP9, AMPLITUDE_STEP9);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AMPLITUDE_STEP10, AMPLITUDE_STEP10);
	AddNewParamToMap("",     "1",    "1",    "7",    "1",    "1",    "7",    "1",    "1",    "7",    WR_DB_FORCE_STEP_AT, FORCE_STEP_AT);
	AddNewParamToMap("s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_HOLD_TIME, HOLD_TIME);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_PRETRIGGER, PRETRIGGER);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_AUTO_PRETRIGGER, AUTO_PRETRIGGER);
	AddNewParamToMap("",     "1",    "0",    "1",    "1",    "0",    "1",    "1",    "0",    "1",    WR_DB_DISTANCE_PRETRIGGER, DISTANCE_PRETRIGGER);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_PRETRIGGER_AMPLITUDE, PRETRIGGER_AMPLITUDE);
	AddNewParamToMap("mm",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  WR_DB_PRETRIGGER_DISTANCE, PRETRIGGER_DISTANCE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_ENERGY_BRAKE, ENERGY_BRAKE);
	AddNewParamToMap("s",    "0.02", "0.01", "1",    "0.02", "0.01", "1",    "0.02", "0.01", "1",    WR_DB_ENERGY_BRAKE_TIME, ENERGY_BRAKE_TIME);
	AddNewParamToMap("%",    "3",    "1",    "100",  "3",    "1",    "100",  "3",    "1",    "100",  WR_DB_ENERGY_BRAKE_AMPLITUDE, ENERGY_BRAKE_AMPLITUDE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_AFTER_BRUST, AFTER_BRUST);
	AddNewParamToMap("s",    "0.1",  "0.050","2",    "0.1",  "0.050","2",    "0.1",  "0.050","2",    WR_DB_AB_DELAY, AFTER_BRUST_DELAY);
	AddNewParamToMap("s",    "0.1",  "0.1",  "2",    "0.1",  "0.1",  "2",    "0.1",  "0.1",  "2",    WR_DB_AB_TIME, AFTER_BRUST_TIME);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_AB_AMPLITUDE, AFTER_BRUST_AMPLITUDE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_COOLING_VALVE, EXTRA_COOLING);
	AddNewParamToMap("s",    "6",    "0.05", "30",   "6",    "0.05", "30",   "6",    "0.05", "30",   WR_DB_MAX_WELD_TIMEOUT, MAX_WELD_TIMEOUT);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_POST_WELD_SEEK, POST_WELD_SEEK);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_PRE_WELD_SEEK, PRE_WELD_SEEK);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_TIMED_SEEK, TIMED_SEEK);
	AddNewParamToMap("min",  "1",    "1",    "59",   "1",    "1",    "59",   "1",    "1",    "59",   WR_DB_TIMED_SEEK_PERIOD, TIMED_SEEK_PERIOD);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_GLOBALSUSPECT, GLOBALSUSPECT);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_GLOBALREJECT, GLOBALREJECT);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_TIME_ENABLED, SUSPECT_TIME_ENABLED);
	AddNewParamToMap("s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   SR_DB_SUSPECT_TIME_LOW_VALUE, SUSPECT_TIME_LOW_VALUE);
	AddNewParamToMap("s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   SR_DB_SUSPECT_TIME_HIGH_VALUE, SUSPECT_TIME_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_TIME_ENABLED, REJECT_TIME_ENABLED);
	AddNewParamToMap("s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   SR_DB_REJECT_TIME_LOW_VALUE, REJECT_TIME_LOW_VALUE);
	AddNewParamToMap("s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   SR_DB_REJECT_TIME_HIGH_VALUE, REJECT_TIME_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ENERGY_ENABLED, SUSPECT_ENERGY_ENABLED);
	AddNewParamToMap("J",    "1.0",  "1.0",   "120000","1.0",  "1.0","45000","1.0",  "1.0",  "24000",SR_DB_SUSPECT_ENERGY_LOW_VALUE, SUSPECT_ENERGY_LOW_VALUE);
	AddNewParamToMap("J",    "120000","1.0",  "120000","45000","1.0","45000","24000","1.0",  "24000",SR_DB_SUSPECT_ENERGY_HIGH_VALUE, SUSPECT_ENERGY_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ENERGY_ENABLED, REJECT_ENERGY_ENABLED);
	AddNewParamToMap("J",    "1.0",  "1.0",  "120000","1.0",  "1.0", "45000","1.0",  "1.0","24000",SR_DB_REJECT_ENERGY_LOW_VALUE, REJECT_ENERGY_LOW_VALUE);
	AddNewParamToMap("J",    "120000","1.0", "120000","45000","1.0", "45000","24000","1.0",  "24000",SR_DB_REJECT_ENERGY_HIGH_VALUE, REJECT_ENERGY_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_PEAKPOWER_ENABLED, SUSPECT_PEAKPOWER_ENABLED);
	AddNewParamToMap("W",    "1",    "1",    "4000", "1",    "1",    "1500", "1",    "1",    "800",  SR_DB_SUSPECT_PEAKPOWER_LOW_VALUE, SUSPECT_PEAKPOWER_LOW_VALUE);
	AddNewParamToMap("W",    "4000", "1",    "4000", "1500", "1",    "1500", "800",  "1",    "800",  SR_DB_SUSPECT_PEAKPOWER_HIGH_VALUE, SUSPECT_PEAKPOWER_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_PEAKPOWER_ENABLED, REJECT_PEAKPOWER_ENABLED);
	AddNewParamToMap("W",    "1",    "1",    "4000", "1",    "1",    "1500", "1",    "1",    "800",  SR_DB_REJECT_PEAKPOWER_LOW_VALUE, REJECT_PEAKPOWER_LOW_VALUE);
	AddNewParamToMap("W",    "4000", "1",    "4000", "1500", "1",    "1500", "800",  "1",    "800",  SR_DB_REJECT_PEAKPOWER_HIGH_VALUE, REJECT_PEAKPOWER_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ABSOLUTEDISTANCE_ENABLED, SUSPECT_ABSOLUTEDISTANCE_ENABLED);
	AddNewParamToMap("mm",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  SR_DB_SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE, SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE);
	AddNewParamToMap("mm",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  SR_DB_SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE, SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ABSOLUTEDISTANCE_ENABLED, REJECT_ABSOLUTEDISTANCE_ENABLED);
	AddNewParamToMap("mm",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  SR_DB_REJECT_ABSOLUTEDISTANCE_LOW_VALUE, REJECT_ABSOLUTEDISTANCE_LOW_VALUE);
	AddNewParamToMap("mm",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  SR_DB_REJECT_ABSOLUTEDISTANCE_HIGH_VALUE, REJECT_ABSOLUTEDISTANCE_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_COLLAPSEDISTANCE_ENABLED, SUSPECT_COLLAPSEDISTANCE_ENABLED);
	AddNewParamToMap("mm",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   SR_DB_SUSPECT_COLLAPSEDISTANCE_LOW_VALUE, SUSPECT_COLLAPSEDISTANCE_LOW_VALUE);
	AddNewParamToMap("mm",   "25",   "0.01", "25",   "25",   "0.01", "25",   "25",   "0.01", "25",   SR_DB_SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE, SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_COLLAPSEDISTANCE_ENABLED, REJECT_COLLAPSEDISTANCE_ENABLED);
	AddNewParamToMap("mm",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   SR_DB_REJECT_COLLAPSEDISTANCE_LOW_VALUE, REJECT_COLLAPSEDISTANCE_LOW_VALUE);
	AddNewParamToMap("mm",   "25",   "0.01", "25",   "25",   "0.01", "25",   "25",   "0.01", "25",   SR_DB_REJECT_COLLAPSEDISTANCE_HIGH_VALUE, REJECT_COLLAPSEDISTANCE_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_TRIGGERDISTANCE_ENABLED, SUSPECT_TRIGGERDISTANCE_ENABLED);
	AddNewParamToMap("mm",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  SR_DB_SUSPECT_TRIGGERDISTANCE_LOW_VALUE, SUSPECT_TRIGGERDISTANCE_LOW_VALUE);
	AddNewParamToMap("mm",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  SR_DB_SUSPECT_TRIGGERDISTANCE_HIGH_VALUE, SUSPECT_TRIGGERDISTANCE_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_TRIGGERDISTANCE_ENABLED, REJECT_TRIGGERDISTANCE_ENABLED);
	AddNewParamToMap("mm",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  SR_DB_REJECT_TRIGGERDISTANCE_LOW_VALUE, REJECT_TRIGGERDISTANCE_LOW_VALUE);
	AddNewParamToMap("mm",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  SR_DB_REJECT_TRIGGERDISTANCE_HIGH_VALUE, REJECT_TRIGGERDISTANCE_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ENDWELDFORCE_ENABLED, SUSPECT_ENDWELDFORCE_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ENDWELDFORCE_ENABLED, REJECT_ENDWELDFORCE_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_FREQUENCY_ENABLED, SUSPECT_FREQUENCY_ENABLED);
	AddNewParamToMap("Hz",   "1",    "1",    "500",  "1",    "1",    "750",  "1",    "1",    "1000", SR_DB_SUSPECT_FREQUENCY_LOW_VALUE, SUSPECT_FREQUENCY_LOW_VALUE);
	AddNewParamToMap("Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", SR_DB_SUSPECT_FREQUENCY_HIGH_VALUE, SUSPECT_FREQUENCY_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_FREQUENCY_ENABLED, REJECT_FREQUENCY_ENABLED);
	AddNewParamToMap("Hz",   "1",    "1",    "500",  "1",    "1",    "750",  "1",    "1",    "1000", SR_DB_REJECT_FREQUENCY_LOW_VALUE, REJECT_FREQUENCY_LOW_VALUE);
	AddNewParamToMap("Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", SR_DB_REJECT_FREQUENCY_HIGH_VALUE, REJECT_FREQUENCY_HIGH_VALUE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_GLOBAL_CONTROL, GLOBALCONTROL);
	AddNewParamToMap("W",    "400",  "1",    "4000", "150",  "1",    "1500", "80",   "1",    "800",  WR_DB_PEAKPOWER_CUTOFF, PEAKPOWERCUTOFF);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_PEAKPOWER_CUTOFF_ENABLE, PEAKPOWERCUTOFFENABLE);
	AddNewParamToMap("Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", WR_DB_FREQUENCY_LOW_CUTOFF, FREQUENCYLOWCUTOFF);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FREQUENCY_LOW_CUTOFF_ENABLE, FREQUENCYLOWCUTOFFENABLE);
	AddNewParamToMap("mm",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  WR_DB_ABSOLUTE_CUTOFF, ABSOLUTECUTOFF);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_ABSOLUTE_CUTOFF_ENABLE, ABSOLUTECUTOFFENABLE);
	AddNewParamToMap("Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", WR_DB_FREQUENCY_HIGH_CUTOFF, FREQUENCYHIGHCUTOFF);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FREQUENCY_HIGH_CUTOFFENABLE, FREQUENCYHIGHCUTOFFENABLE);
	AddNewParamToMap("mm",   "25",   "0.01", "25",   "25",   "0.01", "25",   "25",   "0.01", "25",   WR_DB_COLLAPSE_CUTOFF, COLLAPSECUTOFF);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_COLLAPSE_CUTOFF_ENABLE, COLLAPSECUTOFFENABLE);
	AddNewParamToMap("J",    "120000","1.0", "120000","45000","1.0",   "45000","24000","1.0","24000",WR_DB_ENERGY_CUTOFF, ENERGYCUTOFF);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_ENERGY_CUTOFF_ENABLE, ENERGYCUTOFFENABLE);
	AddNewParamToMap("s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   WR_DB_TIME_CUTOFF, TIMECUTOFF);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_TIME_CUTOFF_ENABLE, TIMECUTOFFENABLE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_GROUND_DETECT_ENABLE, GROUNDDETECTENABLE);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE1, FORCE_STEP_VALUE1);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE2, FORCE_STEP_VALUE2);
	AddNewParamToMap("",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WR_DB_FORCE_STEP_VALUE3, FORCE_STEP_VALUE3);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE1, FORCE_STEP_RAMP_VALUE1);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE2, FORCE_STEP_RAMP_VALUE2);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_STEP_RAMP_VALUE3, FORCE_STEP_RAMP_VALUE3);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_RAMP_TIME, FORCE_RAMP_TIME);
	AddNewParamToMap("s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_HOLD_FORCE_RAMP_TIME, HOLD_FORCE_RAMP_TIME);
	AddNewParamToMap("mm",   "5",    "5",    "120",  "5",    "5",    "120",  "5",    "5",    "120",  WR_DB_READY_POSITION, READY_POSITION);
	AddNewParamToMap("",     "3",    "1",    "3",    "3",    "1",    "3",    "3",    "1",    "3",    WR_DB_WELD_FORCE_CONTROL, WELD_FORCE_CONTROL);
	AddNewParamToMap("",     "1",    "0",    "1",    "1",    "0",    "1",    "1",    "0",    "1",    WR_DB_READY_POSITION_TOGGLE, READY_POSITION_TOGGLE);
	AddNewParamToMap("mm",   "0",    "0",    "125",  "0",    "0",    "125",  "0",    "0",    "125",  WR_DB_EXPECTED_PART_CONTACT_POSITION, EXPECTED_PART_CONTACT_POSITION);
	AddNewParamToMap("mm",   "0.01", "0.01", "5",    "0.01", "0.01", "5",    "0.01", "0.01", "5",    WR_DB_PART_CONTACT_WINDOW_OFFSET, PART_CONTACT_WINDOW_OFFSET);
	AddNewParamToMap("mm",   "2",    "0.01", "10",   "2",    "0.01", "10",   "2",    "0.01", "10",   WR_DB_PART_CONTACT_WINDOW_MINUS, PART_CONTACT_WINDOW_MINUS);
	AddNewParamToMap("mm",   "2",    "0.01", "10",   "2",    "0.01", "10",   "2",    "0.01", "10",   WR_DB_PART_CONTACT_WINDOW_PLUS, PART_CONTACT_WINDOW_PLUS);
	AddNewParamToMap("mm/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000",WR_DB_DOWN_ACCELERATION, DOWN_ACCELERATION);
	AddNewParamToMap("mm/s", "50",   "5",    "150",  "50",   "5",    "150",  "50",   "5",    "150",  WR_DB_DOWN_MAX_VELOCITY, DOWN_MAX_VELOCITY);
	AddNewParamToMap("mm/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000",WR_DB_DOWN_DECELERATION, DOWN_DECELERATION);
	AddNewParamToMap("mm/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000",WR_DB_RETURN_ACCELERATION, RETURN_ACCELERATION);
	AddNewParamToMap("mm/s", "50",   "5",    "150",  "50",   "5",    "150",  "50",   "5",    "150",  WR_DB_RETURN_MAX_VELOCITY, RETURN_MAX_VELOCITY);
	AddNewParamToMap("mm/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000",WR_DB_RETURN_DECELERATION, RETURN_DECELERATION);
	AddNewParamToMap("s",    "0.08", "0.01", "1",    "0.08", "0.01", "1",    "0.08", "0.01", "1",    WR_DB_WELD_RAMP_TIME, WELD_RAMP_TIME);
	AddNewParamToMap("%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  WR_DB_REACTIVITY, REACTIVITY);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_FORCE_LEVEL_ENABLED, FORCE_LEVEL_ENABLED);
	AddNewParamToMap("%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  WR_DB_FORCE_LEVEL, FORCE_LEVEL);
	AddNewParamToMap("s",    "0",    "0",    "0.3",  "0",    "0",    "0.3",  "0",    "0",    "0.3",  WR_DB_FORCE_LEVEL_TIME, FORCE_LEVEL_TIME);
	AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  WR_DB_SCRUB_AMPLITUDE,SCRUB_AMPLITUDE);
	
	AddNewParamToMap("s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   WELD_MODE_TIME, WELD_MODE_TIME);
	AddNewParamToMap("J",    "1.0",  "1.0",  "120000","1.0", "1.0",  "45000","1.0",  "1.0",  "24000",WELD_MODE_ENERGY, WELD_MODE_ENERGY);
	AddNewParamToMap("W",    "400",  "1",    "4000", "150",  "1",    "1500", "80",   "1",    "800",  WELD_MODE_POWER, WELD_MODE_POWER);
	AddNewParamToMap("mm",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  WELD_MODE_ABSOLUTE_DISTANCE, WELD_MODE_ABSOLUTE_DISTANCE);
	AddNewParamToMap("mm",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   WELD_MODE_COLLAPSE_DISTANCE, WELD_MODE_COLLAPSE_DISTANCE);
	AddNewParamToMap("s",    "0.001","0.001","0.5",  "0.001","0.001","0.5",  "0.001","0.001","0.5",  WELD_MODE_SCRUB_TIME, WELD_MODE_SCRUB_TIME);
	AddNewParamToMap("%",    "0",    "0",    "100",  "0",    "0",    "100",  "0",    "0",    "100",  WELD_MODE_DENSITY, WELD_MODE_DENSITY);
	
	// For local suspect reject Validation purpose only
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_TIME_LOW_ENABLED, SUSPECT_TIME_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_TIME_HIGH_ENABLED, SUSPECT_TIME_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_TIME_LOW_ENABLED, REJECT_TIME_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_TIME_HIGH_ENABLED, REJECT_TIME_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ENERGY_LOW_ENABLED, SUSPECT_ENERGY_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ENERGY_HIGH_ENABLED, SUSPECT_ENERGY_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ENERGY_LOW_ENABLED, REJECT_ENERGY_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ENERGY_HIGH_ENABLED, REJECT_ENERGY_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_PEAKPOWER_LOW_ENABLED, SUSPECT_PEAKPOWER_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_PEAKPOWER_HIGH_ENABLED, SUSPECT_PEAKPOWER_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_PEAKPOWER_LOW_ENABLED, REJECT_PEAKPOWER_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_PEAKPOWER_HIGH_ENABLED, REJECT_PEAKPOWER_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ABSOLUTEDISTANCE_LOW_ENABLED, SUSPECT_ABSOLUTEDISTANCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ABSOLUTEDISTANCE_HIGH_ENABLED, SUSPECT_ABSOLUTEDISTANCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ABSOLUTEDISTANCE_LOW_ENABLED, REJECT_ABSOLUTEDISTANCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ABSOLUTEDISTANCE_HIGH_ENABLED, REJECT_ABSOLUTEDISTANCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_COLLAPSEDISTANCE_LOW_ENABLED, SUSPECT_COLLAPSEDISTANCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_COLLAPSEDISTANCE_HIGH_ENABLED, SUSPECT_COLLAPSEDISTANCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_COLLAPSEDISTANCE_LOW_ENABLED, REJECT_COLLAPSEDISTANCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_COLLAPSEDISTANCE_HIGH_ENABLED, REJECT_COLLAPSEDISTANCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_TRIGGERDISTANCE_LOW_ENABLED, SUSPECT_TRIGGERDISTANCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_TRIGGERDISTANCE_HIGH_ENABLED, SUSPECT_TRIGGERDISTANCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_TRIGGERDISTANCE_LOW_ENABLED, REJECT_TRIGGERDISTANCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_TRIGGERDISTANCE_HIGH_ENABLED, REJECT_TRIGGERDISTANCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ENDWELDFORCE_LOW_ENABLED, SUSPECT_ENDWELDFORCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_ENDWELDFORCE_HIGH_ENABLED, SUSPECT_ENDWELDFORCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ENDWELDFORCE_LOW_ENABLED, REJECT_ENDWELDFORCE_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_ENDWELDFORCE_HIGH_ENABLED, REJECT_ENDWELDFORCE_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_FREQUENCY_LOW_ENABLED, SUSPECT_FREQUENCY_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SUSPECT_FREQUENCY_HIGH_ENABLED, SUSPECT_FREQUENCY_HIGH_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_FREQUENCY_LOW_ENABLED, REJECT_FREQUENCY_LOW_ENABLED);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_REJECT_FREQUENCY_HIGH_ENABLED, REJECT_FREQUENCY_HIGH_ENABLED);
	/*added for SET WELD RECIPE PARAM recipe validaion*/
	AddNewParamToMap("N",    "2500", "25",   "2500", "2500", "25",   "2500", "2500", "25",   "2500", WR_DB_WELD_FORCE, WELD_FORCE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_IS_ACTIVE, ISACTIVE);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_IS_LOCKED, ISLOCK);
	AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    WR_DB_IS_VALIDATE, ISVALID);
	AddNewParamToMap("",     "1",    "1",    "1000", "1",    "1",    "1000", "1",    "1",    "1000", WR_DB_RECIPE_NO, RECIPE_NO);
	
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_IS_GLOBAL_SETUP,SETUP_IS_GLOBAL_SETUP);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_WELD_MODE_STATUS,SETUP_WELD_MODE_STATUS);
    AddNewParamToMap("s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   SR_DB_SETUP_WELD_MODE_LOW,SETUP_WELD_MODE_LOW);
    AddNewParamToMap("s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   SR_DB_SETUP_WELD_MODE_HIGH,SETUP_WELD_MODE_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_WELD_AMP_STATUS,SETUP_WELD_AMP_STATUS);
    AddNewParamToMap("%",    "10",   "10",   "100",  "10",   "10",   "100",  "10",   "10",   "100",  SR_DB_SETUP_WELD_AMP_LOW,SETUP_WELD_AMP_LOW);
    AddNewParamToMap("%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  SR_DB_SETUP_WELD_AMP_HIGH,SETUP_WELD_AMP_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_WELD_FORCE_STATUS,SETUP_WELD_FORCE_STATUS);
    AddNewParamToMap("N",    "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", SR_DB_SETUP_WELD_FORCE_LOW,SETUP_WELD_FORCE_LOW);
    AddNewParamToMap("N",    "2500", "25",   "2500", "2500", "25",   "2500", "2500", "25",   "2500", SR_DB_SETUP_WELD_FORCE_HIGH,SETUP_WELD_FORCE_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_TRIGGER_FORCE_STATUS,SETUP_TRIGGER_FORCE_STATUS);
    AddNewParamToMap("N",    "25",   "5",    "2500", "25",   "5",    "2500", "25",   "5",    "2500", SR_DB_SETUP_TRIGGER_FORCE_LOW,SETUP_TRIGGER_FORCE_LOW);
    AddNewParamToMap("N",    "2500", "5",    "2500", "2500", "5",    "2500", "2500", "5",    "2500", SR_DB_SETUP_TRIGGER_FORCE_HIGH,SETUP_TRIGGER_FORCE_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_HOLD_FORCE_STATUS,SETUP_HOLD_FORCE_STATUS);
    AddNewParamToMap("N",    "25",   "25",   "2500", "25",   "25",   "2500", "25",   "25",   "2500", SR_DB_SETUP_HOLD_FORCE_LOW,SETUP_HOLD_FORCE_LOW);
    AddNewParamToMap("N",    "2500", "25",   "2500", "2500", "15",   "2500", "2500", "25",   "2500", SR_DB_SETUP_HOLD_FORCE_HIGH,SETUP_HOLD_FORCE_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_HOLD_TIME_STATUS,SETUP_HOLD_TIME_STATUS);
    AddNewParamToMap("s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   SR_DB_SETUP_HOLD_TIME_LOW,SETUP_HOLD_TIME_LOW);
    AddNewParamToMap("s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   SR_DB_SETUP_HOLD_TIME_HIGH,SETUP_HOLD_TIME_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_SCRUB_AMP_STATUS,SETUP_SCRUB_AMP_STATUS);
    AddNewParamToMap("%",    "1",    "1",    "100",  "1",    "1",    "100",  "1",    "1",    "100",  SR_DB_SETUP_SCRUB_AMP_LOW,SETUP_SCRUB_AMP_LOW);
    AddNewParamToMap("%",    "100",  "1",    "100",  "100",  "1",    "100",  "100",  "1",    "100",  SR_DB_SETUP_SCRUB_AMP_HIGH,SETUP_SCRUB_AMP_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_SCRUB_TIME_STATUS,SETUP_SCRUB_TIME_STATUS);
    AddNewParamToMap("s",    "0.001","0.001","0.5",  "0.001","0.001","0.5",  "0.001","0.001","0.5",  SR_DB_SETUP_SCRUB_TIME_LOW,SETUP_SCRUB_TIME_LOW);
    AddNewParamToMap("s",    "0.5",  "0.001","0.5",  "0.5",  "0.001","0.5",  "0.5",  "0.001","0.5",  SR_DB_SETUP_SCRUB_TIME_HIGH,SETUP_SCRUB_TIME_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_REACTIVITY_STATUS,SETUP_REACTIVITY_STATUS);
    AddNewParamToMap("%",    "0",    "0",    "100",  "0",    "0",    "100",  "0",    "0",    "100",  SR_DB_SETUP_REACTIVITY_LOW,SETUP_REACTIVITY_LOW);
    AddNewParamToMap("%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  SR_DB_SETUP_REACTIVITY_HIGH,SETUP_REACTIVITY_HIGH);
    AddNewParamToMap("",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    SR_DB_SETUP_FORCE_LEVEL_STATUS,SETUP_FORCE_LEVEL_STATUS);
    AddNewParamToMap("%",    "0",    "0",    "100",  "0",    "0",    "100",  "0",    "0",    "100",  SR_DB_SETUP_FORCE_LEVEL_LOW,SETUP_FORCE_LEVEL_LOW);
    AddNewParamToMap("%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  SR_DB_SETUP_FORCE_LEVEL_HIGH,SETUP_FORCE_LEVEL_HIGH);
}

/**************************************************************************//**
 * \brief  - Returns index of type of weld mode
 *
 * \param  - None
 *
 * \return  - int
 *
 ******************************************************************************/
int Recipe::GetModeValueIndexByWeldMode()
{
	int dwModeIndex,dwModeValueIndex;

	GetParamValueByID(WELD_MODE,dwModeIndex);

	switch(dwModeIndex)
	{
	case WM_TIME:
		dwModeValueIndex = WELD_MODE_TIME;
		break;

	case WM_ENERGY:
		dwModeValueIndex = WELD_MODE_ENERGY;
		break;

	case WM_POWER:
		dwModeValueIndex = WELD_MODE_POWER;
		break;
		
	case WM_GROUND_DETECT:
		dwModeValueIndex = WELD_MODE_SCRUB_TIME;
		break;

	case WM_ABSOLUTE_DISTANCE:
		dwModeValueIndex = WELD_MODE_ABSOLUTE_DISTANCE;
		break;

	case WM_COLLAPSE_DISTANCE:
		dwModeValueIndex = WELD_MODE_COLLAPSE_DISTANCE;
		break;
		
	case WM_DYNAMIC:
		dwModeValueIndex = WELD_MODE_DENSITY;
		break;


	default:
		dwModeValueIndex = WELD_MODE_TIME;
		break;
	}
	return dwModeValueIndex;
}

/**************************************************************************//**
 * \brief  - Returns Default value of recipe parameter from map
 * 
 * \param  - int dwParamID
 *
 * \return  - int
 *
 ******************************************************************************/
int Recipe::GetDefaultValue(int dwParamID)
{
	double dDefaultVal;
	int iDefaultVal = 0;
	char chValue[8]={0};
	map<int,stRecipeParmInfo>::iterator it;
	it = m_MapRecipeParams.find(dwParamID);
	
	if(it != m_MapRecipeParams.end())
	{
		//The default value for Absolute Distance Weld Mode Value is the actual Expected Part Contact value
		if(WELD_MODE_ABSOLUTE_DISTANCE == dwParamID)
		{
			GetParamValueByID(EXPECTED_PART_CONTACT_POSITION, iDefaultVal);
		}
		else
		{
			dDefaultVal = atof(m_MapRecipeParams[dwParamID].ParamDefaultValue[dwSystemFrequnecyType].c_str());
			ConvertValueUnitForID(dwParamID,dDefaultVal,chValue,false);
			iDefaultVal = atoi(chValue);
		}
	}
	return iDefaultVal;
}

/**************************************************************************//**
 * \brief  - Returns minimum value of recipe parameter from map
 * 
 * \param  - int dwParamID
 *
 * \return  - int
 *
 ******************************************************************************/
int Recipe::GetMinValue(int dwParamID)
{
	int dwMinValue = 0;
	double dMinVal;
	char chValue[8]={0};
	map<int,stRecipeParmInfo>::iterator it;
	it = m_MapRecipeParams.find(dwParamID);
	if(it != m_MapRecipeParams.end())
	{
		if(dwParamID == WELD_MODE_ABSOLUTE_DISTANCE || dwParamID == ABSOLUTECUTOFF )
		{
			GetParamValueByID(EXPECTED_PART_CONTACT_POSITION, dwMinValue);
			//The minimum value for Absolute Distance parameter and Absolute Cutoff is Expected Part Contact value minus 2mm
			return (dwMinValue - 2000);
		}
		else
			dMinVal = atof(m_MapRecipeParams[dwParamID].ParamMinValue[dwSystemFrequnecyType].c_str());

		if(dwParamID == FORCE_STEP1)
		{
			int dwTriggerForceValue = 0;
			GetParamValueByID(TRIGGER_FORCE, dwTriggerForceValue);
			if(dwTriggerForceValue > dMinVal)
			{
				dwMinValue = dwTriggerForceValue;
				return dwMinValue;
			}
			
		}
		ConvertValueUnitForID(dwParamID,dMinVal,chValue,false);
		dwMinValue = atoi(chValue);
	}
	return dwMinValue;
}

/**************************************************************************//**
 * \brief  - Returns maximum value of recipe parameter from map
 * 
 * \param  - int dwParamID
 *
 * \return  - int
 *
 ******************************************************************************/
int Recipe::GetMaxValue(int dwParamID)
{
	int dwMaxValue = 0;
	double dMaxVal;
	char chValue[8]={0};
	map<int,stRecipeParmInfo>::iterator it;
	it = m_MapRecipeParams.find(dwParamID);
	if(it != m_MapRecipeParams.end())
	{
		if(WELD_MODE_POWER == dwParamID)
		{
			intf_SysConfig_t SysConfigInfo;
			CP->GetSysConfContext(SysConfigInfo);
			return SysConfigInfo.PcConfig.PSWatt;
		}
		else if(dwParamID == READY_POSITION)
		{
			int EPCValue = 0;
			GetParamValueByID(EXPECTED_PART_CONTACT_POSITION, EPCValue);
			if(EPCValue <= 0)
				dwMaxValue = 0;
			else
				//Maximum value for ready position is Expected Part Contact value minus 5mm
				dwMaxValue = (EPCValue - 5000);
			return dwMaxValue;
		}
		else if(dwParamID == WELD_MODE_ABSOLUTE_DISTANCE || dwParamID == ABSOLUTECUTOFF )
		{
			dMaxVal = atof(m_MapRecipeParams[EXPECTED_PART_CONTACT_POSITION].ParamMaxValue[dwSystemFrequnecyType].c_str());
		}
		else
			dMaxVal = atof(m_MapRecipeParams[dwParamID].ParamMaxValue[dwSystemFrequnecyType].c_str());

		if(dwParamID == TRIGGER_FORCE)
		{
			int dwForceValue = 0;
			GetParamValueByID(FORCE_STEP1, dwForceValue);
			if(dwForceValue < dMaxVal)
			{
				dwMaxValue = dwForceValue;
				return dwMaxValue;
			}
		}
		ConvertValueUnitForID(dwParamID,dMaxVal,chValue,false);
		dwMaxValue = atoi(chValue);
	}
	
	return dwMaxValue;
}

/**************************************************************************//**
 * \brief  - Converts parameter value based on unit present in map
 * 
 * \param  - int dwParamID, double dOriginalValue, char *chConvertedValue,
 * 			 bool bIsValueReadingFromDB
 *
 * \return  - None
 *
 ******************************************************************************/
void Recipe::ConvertValueUnitForID(int dwParamID, double dOriginalValue, char *chConvertedValue, bool bIsValueReadingFromDB)
{
	if(dwParamID == MODE_VALUE)
		dwParamID = GetModeValueIndexByWeldMode();

	string strUnit = m_MapRecipeParams[dwParamID].ParamUnit;
	
	if(!strcmp(strUnit.c_str(),"s"))
	{
		if(bIsValueReadingFromDB)
			sprintf(chConvertedValue,"%.03f",dOriginalValue / 1000.0);
		else
			sprintf(chConvertedValue,"%.0f",dOriginalValue * 1000.0);
	}
	else if(!strcmp(strUnit.c_str(),"mm"))
	{
		if(bIsValueReadingFromDB)
			sprintf(chConvertedValue,"%.03f",dOriginalValue / 1000.0);
		else
			sprintf(chConvertedValue,"%.0f",dOriginalValue * 1000.0);
	}
	else if(!strcmp(strUnit.c_str(),"J"))
	{
		if(bIsValueReadingFromDB)
			sprintf(chConvertedValue,"%.01f",dOriginalValue / 1000.0);
		else
			sprintf(chConvertedValue,"%.0f",dOriginalValue * 1000.0);
	}
	else
	{
		sprintf(chConvertedValue,"%.0f",dOriginalValue);
	}
}

/**************************************************************************//**
 * \brief  - Returns parameter value for a particular parameter ID from recipe zero
 * 
 * \param  - int dwParamID, int &dwParamValue
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS Recipe::GetParamValueByID(int dwParamID, int &dwParamValue)
{
	vector<string> vRecipeParams,vSuspectRejectParams;
	int dwSuspectRejectIndex;
	int dwParameterDBIndex = m_MapRecipeParams[dwParamID].dwIndexInDB;
	if(dwParameterDBIndex >= WR_DB_RECIPE_NUMBER && dwParameterDBIndex <= END_OF_WR_PARAMETERS)
	{
		CP->StringToTokens(DBInterface::m_WeldRecipeZero.c_str(),vRecipeParams,',');

		dwParamValue = atoi(vRecipeParams[dwParameterDBIndex].c_str());
	}
	else if(dwParameterDBIndex >= SR_DB_GLOBALSUSPECT && dwParameterDBIndex <= SR_DB_REJECT_FREQUENCY_HIGH_VALUE)
	{
		CP->StringToTokens(DBInterface::m_SuspectRejectLimitsZero.c_str(),vRecipeParams,',');
		if(dwParameterDBIndex == SR_DB_GLOBALSUSPECT)
			dwParamValue = atoi(vRecipeParams[2].c_str());
		else if(dwParameterDBIndex == SR_DB_GLOBALREJECT)
			dwParamValue = atoi(vRecipeParams[3].c_str());
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_TIME_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_TIME_HIGH_VALUE))
		{
			vRecipeParams[SR_TIME].erase(vRecipeParams[SR_TIME].begin());
			vRecipeParams[SR_TIME].erase(vRecipeParams[SR_TIME].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_TIME].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_TIME_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_ENERGY_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_ENERGY_HIGH_VALUE))
		{
			vRecipeParams[SR_ENERGY].erase(vRecipeParams[SR_ENERGY].begin());
			vRecipeParams[SR_ENERGY].erase(vRecipeParams[SR_ENERGY].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_ENERGY].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_ENERGY_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_PEAKPOWER_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_PEAKPOWER_HIGH_VALUE))
		{
			vRecipeParams[SR_POWER].erase(vRecipeParams[SR_POWER].begin());
			vRecipeParams[SR_POWER].erase(vRecipeParams[SR_POWER].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_POWER].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_PEAKPOWER_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_ABSOLUTEDISTANCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_ABSOLUTEDISTANCE_HIGH_VALUE))
		{
			vRecipeParams[SR_ABSOLUTE_DISTANCE].erase(vRecipeParams[SR_ABSOLUTE_DISTANCE].begin());
			vRecipeParams[SR_ABSOLUTE_DISTANCE].erase(vRecipeParams[SR_ABSOLUTE_DISTANCE].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_ABSOLUTE_DISTANCE].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_ABSOLUTEDISTANCE_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_COLLAPSEDISTANCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_COLLAPSEDISTANCE_HIGH_VALUE))
		{
			vRecipeParams[SR_COLLAPSE_DISTANCE].erase(vRecipeParams[SR_COLLAPSE_DISTANCE].begin());
			vRecipeParams[SR_COLLAPSE_DISTANCE].erase(vRecipeParams[SR_COLLAPSE_DISTANCE].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_COLLAPSE_DISTANCE].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_COLLAPSEDISTANCE_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_TRIGGERDISTANCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_TRIGGERDISTANCE_HIGH_VALUE))
		{
			vRecipeParams[SR_TRIGGER_DISTANCE].erase(vRecipeParams[SR_TRIGGER_DISTANCE].begin());
			vRecipeParams[SR_TRIGGER_DISTANCE].erase(vRecipeParams[SR_TRIGGER_DISTANCE].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_TRIGGER_DISTANCE].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_TRIGGERDISTANCE_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_ENDWELDFORCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_ENDWELDFORCE_HIGH_VALUE))
		{
			vRecipeParams[SR_END_WELD_FORCE].erase(vRecipeParams[SR_END_WELD_FORCE].begin());
			vRecipeParams[SR_END_WELD_FORCE].erase(vRecipeParams[SR_END_WELD_FORCE].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_END_WELD_FORCE].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_ENDWELDFORCE_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_FREQUENCY_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_FREQUENCY_HIGH_VALUE))
		{
			vRecipeParams[SR_FREQUENCY].erase(vRecipeParams[SR_FREQUENCY].begin());
			vRecipeParams[SR_FREQUENCY].erase(vRecipeParams[SR_FREQUENCY].end() - 1);
			CP->StringToTokens(vRecipeParams[SR_FREQUENCY].c_str(),vSuspectRejectParams,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_FREQUENCY_ENABLED;
			dwParamValue = atoi(vSuspectRejectParams[dwSuspectRejectIndex].c_str());
		}
	}
	else
	{
		return MISMATCH_PARAMID;
	}

	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief  - Set parameter value for a particular parameter ID in recipe zero
 * 
 * \param  - int dwParamID, double dParamValue
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS Recipe::SetParamValueByID(int dwParamID, double dParamValue)
{
	/* updating the value in RecipeZero strings */
	vector<string> vWeldRecipeParams,vSRRecipeParams,vSRIndividualValues;
	int dwSuspectRejectIndex;
	int dwParameterDBIndex = m_MapRecipeParams[dwParamID].dwIndexInDB;
	char chValue[8]={0};
	int dwWeldMode = 0;
	string strProperStringFormatToStoreInDB = "";
	
	CP->StringToTokens(DBInterface::m_WeldRecipeZero.c_str(),vWeldRecipeParams,',');
	CP->StringToTokens(DBInterface::m_SuspectRejectLimitsZero.c_str(),vSRRecipeParams,',');
	
	if(dwParameterDBIndex >= WR_DB_RECIPE_NUMBER && dwParameterDBIndex <= END_OF_WR_PARAMETERS)
	{
		if(dwParameterDBIndex == WR_DB_WELD_MODE)
		{			
			int dwDefaultModeValue = 0;
			GetParamValueByID(WELD_MODE,dwWeldMode);
			if(dwWeldMode != (int)dParamValue)
			{
				switch((int)dParamValue)
				{
				case WM_TIME:
					dwDefaultModeValue = GetDefaultValue(WELD_MODE_TIME);					
					break;
				case WM_ENERGY:
					dwDefaultModeValue = GetDefaultValue(WELD_MODE_ENERGY);
					break;
				case WM_POWER:
					dwDefaultModeValue = GetDefaultValue(WELD_MODE_POWER);
					break;
				case WM_GROUND_DETECT:
					dwDefaultModeValue = GetDefaultValue(WELD_MODE_SCRUB_TIME);
					break;
				case WM_ABSOLUTE_DISTANCE:
					dwDefaultModeValue = GetDefaultValue(WELD_MODE_ABSOLUTE_DISTANCE);
					break;
				case WM_COLLAPSE_DISTANCE:
					dwDefaultModeValue = GetDefaultValue(WELD_MODE_COLLAPSE_DISTANCE);
					break;
				case WM_DYNAMIC:
					dwDefaultModeValue = GetDefaultValue(WELD_MODE_DENSITY);
					break;
				default:
					break;
				}
				SetParamValueInStructure(MODE_VALUE,dwDefaultModeValue);
				sprintf(chValue,"%d",dwDefaultModeValue);
				vWeldRecipeParams[WR_DB_MODE_VALUE] = chValue;
				sprintf(chValue,"%d",(int)dParamValue);
				vWeldRecipeParams[WR_DB_WELD_MODE] = chValue;
			}
		}
		else if(dwParameterDBIndex == WR_DB_AMPLITUDE_STEP_AT)
		{
			int dwDefaultValue, dwModeIndex;
			dwModeIndex = WELD_MODE_TIME + (dParamValue - 1);
			dwDefaultValue = GetDefaultValue(dwModeIndex);
			for(int dwIndex = AMPLITUDE_STEP_VALUE1, dwDBIndex = WR_DB_AMPLITUDE_STEP_VALUE1; dwIndex <= AMPLITUDE_STEP_VALUE10; dwIndex++,dwDBIndex++)
			{			
				SetParamValueInStructure(dwIndex,dwDefaultValue);
				sprintf(chValue,"%d",dwDefaultValue);
				vWeldRecipeParams[dwDBIndex] = chValue;
			}
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
		/* Converting Amplitude step values based on step at value */
		else if((dwParameterDBIndex >= WR_DB_AMPLITUDE_STEP_VALUE1) && (dwParameterDBIndex <= WR_DB_AMPLITUDE_STEP_VALUE10))
		{
			int dwStepAt = 0;
			int dwWeldModeParamId;
			GetParamValueByID(AMPLITUDE_STEP_AT,dwStepAt);
			dwWeldModeParamId = WELD_MODE_TIME + (dwStepAt - 1);
			
			ConvertValueUnitForID(dwWeldModeParamId,dParamValue,chValue,false);
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
		else if(dwParameterDBIndex == WR_DB_FORCE_STEP_AT)
		{
			int dwDefaultValue, dwModeIndex;
			dwModeIndex = WELD_MODE_TIME + (dParamValue - 1);
			dwDefaultValue = GetDefaultValue(dwModeIndex);
			for(int dwIndex = FORCE_STEP_VALUE1, dwDBIndex = WR_DB_FORCE_STEP_VALUE1; dwIndex <= FORCE_STEP_VALUE10; dwIndex++,dwDBIndex++)
			{			
				SetParamValueInStructure(dwIndex,dwDefaultValue);
				sprintf(chValue,"%d",dwDefaultValue);
				vWeldRecipeParams[dwDBIndex] = chValue;
			}
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
		/* Converting Force step values based on step at value */
		else if((dwParameterDBIndex >= WR_DB_FORCE_STEP_VALUE1) && (dwParameterDBIndex <= WR_DB_FORCE_STEP_VALUE10))
		{
			int dwStepAt = 0;
			int dwWeldModeParamId;
			GetParamValueByID(FORCE_STEP_AT,dwStepAt);
			dwWeldModeParamId = WELD_MODE_TIME + (dwStepAt - 1);
			
			ConvertValueUnitForID(dwWeldModeParamId,dParamValue,chValue,false);
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
		// When setting value to ForceStepForce 1 also sets the value to Weld Force
		else if (WR_DB_FORCE_STEP1 == dwParameterDBIndex)
		{
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			
			vWeldRecipeParams[WR_DB_WELD_FORCE] = chValue;
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
		//When changing the value of Auto Pretrigger also changes the value of Distance Pretrigger 
		else if(WR_DB_AUTO_PRETRIGGER == dwParameterDBIndex)
		{
			int tempParamValue;
			
			if(dParamValue)
				tempParamValue = 0;
			else
				tempParamValue = 1;
			
			ConvertValueUnitForID(dwParamID,tempParamValue,chValue,false);
			vWeldRecipeParams[WR_DB_DISTANCE_PRETRIGGER] = chValue;
			
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
		//When changing the value of Distance Pretrigger also changes the value of Auto Pretrigger 
		else if(WR_DB_DISTANCE_PRETRIGGER == dwParameterDBIndex)
		{
			int tempParamValue;
			
			if(dParamValue)
				tempParamValue = 0;
			else
				tempParamValue = 1;
			
			ConvertValueUnitForID(dwParamID,tempParamValue,chValue,false);
			vWeldRecipeParams[WR_DB_AUTO_PRETRIGGER] = chValue;
			
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
		else
		{
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vWeldRecipeParams[dwParameterDBIndex] = chValue;
		}
	}
	else if(dwParameterDBIndex >= SR_DB_GLOBALSUSPECT && dwParameterDBIndex <= SR_DB_REJECT_FREQUENCY_HIGH_VALUE)
	{
		if(dwParameterDBIndex == SR_DB_GLOBALSUSPECT)
		{
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRRecipeParams[SR_GLOBAL_SUSPECT] = chValue;
		}
		else if(dwParameterDBIndex == SR_DB_GLOBALREJECT)
		{
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRRecipeParams[SR_GLOBAL_REJECT] = chValue;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_TIME_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_TIME_HIGH_VALUE))
		{
			//Removing the Start and End quotes ' '
			vSRRecipeParams[SR_TIME].erase(vSRRecipeParams[SR_TIME].begin());
			vSRRecipeParams[SR_TIME].erase(vSRRecipeParams[SR_TIME].end() - 1);
			//Separate the SR on individual parameters
			CP->StringToTokens(vSRRecipeParams[SR_TIME].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_TIME_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_TIME_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_TIME_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_TIME_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_TIME_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_TIME],vSRIndividualValues,':');
			
			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_TIME].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_TIME] = strProperStringFormatToStoreInDB;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_ENERGY_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_ENERGY_HIGH_VALUE))
		{
			vSRRecipeParams[SR_ENERGY].erase(vSRRecipeParams[SR_ENERGY].begin());
			vSRRecipeParams[SR_ENERGY].erase(vSRRecipeParams[SR_ENERGY].end() - 1);
			CP->StringToTokens(vSRRecipeParams[SR_ENERGY].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_ENERGY_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_ENERGY_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_ENERGY_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_ENERGY_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_ENERGY_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_ENERGY],vSRIndividualValues,':');
			
			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_ENERGY].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_ENERGY] = strProperStringFormatToStoreInDB;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_PEAKPOWER_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_PEAKPOWER_HIGH_VALUE))
		{
			vSRRecipeParams[SR_POWER].erase(vSRRecipeParams[SR_POWER].begin());
			vSRRecipeParams[SR_POWER].erase(vSRRecipeParams[SR_POWER].end() - 1);
			CP->StringToTokens(vSRRecipeParams[SR_POWER].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_PEAKPOWER_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_PEAKPOWER_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_PEAKPOWER_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_PEAKPOWER_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_PEAKPOWER_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_POWER],vSRIndividualValues,':');

			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_POWER].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_POWER] = strProperStringFormatToStoreInDB;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_ABSOLUTEDISTANCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_ABSOLUTEDISTANCE_HIGH_VALUE))
		{
			vSRRecipeParams[SR_ABSOLUTE_DISTANCE].erase(vSRRecipeParams[SR_ABSOLUTE_DISTANCE].begin());
			vSRRecipeParams[SR_ABSOLUTE_DISTANCE].erase(vSRRecipeParams[SR_ABSOLUTE_DISTANCE].end() - 1);
			CP->StringToTokens(vSRRecipeParams[SR_ABSOLUTE_DISTANCE].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_ABSOLUTEDISTANCE_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_ABSOLUTEDISTANCE_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_ABSOLUTEDISTANCE_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_ABSOLUTE_DISTANCE],vSRIndividualValues,':');

			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_ABSOLUTE_DISTANCE].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_ABSOLUTE_DISTANCE] = strProperStringFormatToStoreInDB;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_COLLAPSEDISTANCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_COLLAPSEDISTANCE_HIGH_VALUE))
		{
			vSRRecipeParams[SR_COLLAPSE_DISTANCE].erase(vSRRecipeParams[SR_COLLAPSE_DISTANCE].begin());
			vSRRecipeParams[SR_COLLAPSE_DISTANCE].erase(vSRRecipeParams[SR_COLLAPSE_DISTANCE].end() - 1);
			CP->StringToTokens(vSRRecipeParams[SR_COLLAPSE_DISTANCE].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_COLLAPSEDISTANCE_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_COLLAPSEDISTANCE_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_COLLAPSEDISTANCE_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_COLLAPSEDISTANCE_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_COLLAPSE_DISTANCE],vSRIndividualValues,':');

			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_COLLAPSE_DISTANCE].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_COLLAPSE_DISTANCE] = strProperStringFormatToStoreInDB;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_TRIGGERDISTANCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_TRIGGERDISTANCE_HIGH_VALUE))
		{
			vSRRecipeParams[SR_TRIGGER_DISTANCE].erase(vSRRecipeParams[SR_TRIGGER_DISTANCE].begin());
			vSRRecipeParams[SR_TRIGGER_DISTANCE].erase(vSRRecipeParams[SR_TRIGGER_DISTANCE].end() - 1);
			CP->StringToTokens(vSRRecipeParams[SR_TRIGGER_DISTANCE].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_TRIGGERDISTANCE_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_TRIGGERDISTANCE_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_TRIGGERDISTANCE_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_TRIGGERDISTANCE_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_TRIGGERDISTANCE_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_TRIGGER_DISTANCE],vSRIndividualValues,':');

			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_TRIGGER_DISTANCE].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_TRIGGER_DISTANCE] = strProperStringFormatToStoreInDB;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_ENDWELDFORCE_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_ENDWELDFORCE_HIGH_VALUE))
		{
			vSRRecipeParams[SR_END_WELD_FORCE].erase(vSRRecipeParams[SR_END_WELD_FORCE].begin());
			vSRRecipeParams[SR_END_WELD_FORCE].erase(vSRRecipeParams[SR_END_WELD_FORCE].end() - 1);
			CP->StringToTokens(vSRRecipeParams[SR_END_WELD_FORCE].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_ENDWELDFORCE_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_ENDWELDFORCE_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_ENDWELDFORCE_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_ENDWELDFORCE_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_ENDWELDFORCE_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_END_WELD_FORCE],vSRIndividualValues,':');

			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_END_WELD_FORCE].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_END_WELD_FORCE] = strProperStringFormatToStoreInDB;
		}
		else if((dwParameterDBIndex >= SR_DB_SUSPECT_FREQUENCY_ENABLED) && (dwParameterDBIndex <= SR_DB_REJECT_FREQUENCY_HIGH_VALUE))
		{
			vSRRecipeParams[SR_FREQUENCY].erase(vSRRecipeParams[SR_FREQUENCY].begin());
			vSRRecipeParams[SR_FREQUENCY].erase(vSRRecipeParams[SR_FREQUENCY].end() - 1);
		
			CP->StringToTokens(vSRRecipeParams[SR_FREQUENCY].c_str(),vSRIndividualValues,':');
			dwSuspectRejectIndex = dwParameterDBIndex - SR_DB_SUSPECT_FREQUENCY_ENABLED;
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
			vSRIndividualValues[dwSuspectRejectIndex] = chValue;
			if(dwParameterDBIndex == SR_DB_SUSPECT_FREQUENCY_LOW_VALUE)
				vSRIndividualValues[SUS_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_SUSPECT_FREQUENCY_HIGH_VALUE)
				vSRIndividualValues[SUS_HIGH_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_FREQUENCY_LOW_VALUE)
				vSRIndividualValues[REJ_LOW_ENABLED] = "1";
			else if(dwParameterDBIndex == SR_DB_REJECT_FREQUENCY_HIGH_VALUE)
				vSRIndividualValues[REJ_HIGH_ENABLED] = "1";
			CP->TokensToString(vSRRecipeParams[SR_FREQUENCY],vSRIndividualValues,':');

			strProperStringFormatToStoreInDB.append("'");
			strProperStringFormatToStoreInDB.append(vSRRecipeParams[SR_FREQUENCY].c_str());
			strProperStringFormatToStoreInDB.append("'");			
			vSRRecipeParams[SR_FREQUENCY] = strProperStringFormatToStoreInDB;
		}
		else
		{
			/* This case will never happen, kept just for the developer information. */ 
			ConvertValueUnitForID(dwParamID,dParamValue,chValue,false);
		}
	}
	else
	{
		return MISMATCH_PARAMID;
	}
	
	if((atoi(vWeldRecipeParams[WR_DB_RECIPE_NUMBER].c_str()) > 0) || (atoi(vSRRecipeParams[WR_DB_RECIPE_NUMBER].c_str()) > 0))
	{
		vWeldRecipeParams[WR_DB_WELD_RECIPE_VERSION_NUMBER] = vWeldRecipeParams[WR_DB_RECIPE_NUMBER];
		vWeldRecipeParams[WR_DB_RECIPE_NUMBER] = "0";
		
		vSRRecipeParams[1] = vSRRecipeParams[WR_DB_RECIPE_NUMBER];
		vSRRecipeParams[WR_DB_RECIPE_NUMBER] = "0";
	}
	
	CP->TokensToString(DBInterface::m_WeldRecipeZero,vWeldRecipeParams,',');
	CP->TokensToString(DBInterface::m_SuspectRejectLimitsZero,vSRRecipeParams,',');
	
	int dwParamValue = atoi(chValue);
	
	return SetParamValueInStructure(dwParamID,dwParamValue);	
}

/**************************************************************************//**
 * \brief  - Set parameter value for a particular parameter ID in recipe zero
 * 
 * \param  - int dwParamID, double dParamValue
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS Recipe::SetParamValueInStructure(int dwParamID, int dwParamValue)
{
	Message message;
	/* Checking the ID is SC parameter ID */
	if(SC_PARAM == GetParamType(dwParamID)) 
	{
		WeldRecipeSC stSCRecipe;
		CP->m_RecipeSC.GetActiveRecipe(stSCRecipe);
		switch(dwParamID)
		{
		case WELD_MODE:
			stSCRecipe.WeldMode = dwParamValue;
			break;
		case MODE_VALUE: 
			stSCRecipe.ModeValue = dwParamValue;
			break;
		case AMPLITUDESTEPSCNT: 
			stSCRecipe.NumAmplitudeSteps = dwParamValue;
			break;
		case AMPLITUDE_STEP_AT: 
			stSCRecipe.AmplitudeStepAt = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE1: 
			stSCRecipe.AmplitudeStepValue[0] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE2: 
			stSCRecipe.AmplitudeStepValue[1] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE3: 
			stSCRecipe.AmplitudeStepValue[2] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE4: 
			stSCRecipe.AmplitudeStepValue[3] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE5: 
			stSCRecipe.AmplitudeStepValue[4] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE6: 
			stSCRecipe.AmplitudeStepValue[5] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE7: 
			stSCRecipe.AmplitudeStepValue[6] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE8: 
			stSCRecipe.AmplitudeStepValue[7] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE9: 
			stSCRecipe.AmplitudeStepValue[8] = dwParamValue;
			break;
		case AMPLITUDE_STEP_VALUE10: 
			stSCRecipe.AmplitudeStepValue[9] = dwParamValue;
			break;
		case AMPLITUDE_STEP1: 
			stSCRecipe.AmplitudeStep[0] = dwParamValue;
			break;
		case AMPLITUDE_STEP2: 
			stSCRecipe.AmplitudeStep[1] = dwParamValue;
			break;
		case AMPLITUDE_STEP3: 
			stSCRecipe.AmplitudeStep[2] = dwParamValue;
			break;
		case AMPLITUDE_STEP4: 
			stSCRecipe.AmplitudeStep[3] = dwParamValue;
			break;
		case AMPLITUDE_STEP5: 
			stSCRecipe.AmplitudeStep[4] = dwParamValue;
			break;
		case AMPLITUDE_STEP6: 
			stSCRecipe.AmplitudeStep[5] = dwParamValue;
			break;
		case AMPLITUDE_STEP7: 
			stSCRecipe.AmplitudeStep[6] = dwParamValue;
			break;
		case AMPLITUDE_STEP8: 
			stSCRecipe.AmplitudeStep[7] = dwParamValue;
			break;
		case AMPLITUDE_STEP9: 
			stSCRecipe.AmplitudeStep[8] = dwParamValue;
			break;
		case AMPLITUDE_STEP10: 
			stSCRecipe.AmplitudeStep[9] = dwParamValue;
			break;
		case TRIGGER_FORCE: 
			stSCRecipe.TriggerForce = dwParamValue;
			break;
		case NUM_FORCE_STEPS: 
			stSCRecipe.NumForceSteps = dwParamValue;
			break;
		case FORCE_STEP_AT: 
			stSCRecipe.ForceStepAt = dwParamValue;
			break;
		case HOLD_TIME: 
			stSCRecipe.HoldTime = dwParamValue;
			break;
		case PRETRIGGER: 
			stSCRecipe.PreTrigger.bIsEnabled = dwParamValue;
			break;
		case AUTO_PRETRIGGER:
			stSCRecipe.PreTrigger.bIsAutoEnabled = dwParamValue;
			stSCRecipe.PreTrigger.bIsDistanceEnabled = !stSCRecipe.PreTrigger.bIsAutoEnabled;
			break;
		case DISTANCE_PRETRIGGER: 
			stSCRecipe.PreTrigger.bIsDistanceEnabled = dwParamValue;
			stSCRecipe.PreTrigger.bIsAutoEnabled = !stSCRecipe.PreTrigger.bIsDistanceEnabled;
			break;
		case PRETRIGGER_AMPLITUDE: 
			stSCRecipe.PreTrigger.TriggerAmplitude = dwParamValue;
			break;
		case PRETRIGGER_DISTANCE: 
			stSCRecipe.PreTrigger.PreTriggerDistance = dwParamValue;
			break;
		case ENERGY_BRAKE: 
			stSCRecipe.EnergyBrake.bIsEnabled = dwParamValue;
			break;
		case ENERGY_BRAKE_TIME: 
			stSCRecipe.EnergyBrake.EnergyBrakeTime = dwParamValue;
			break;
		case ENERGY_BRAKE_AMPLITUDE: 
			stSCRecipe.EnergyBrake.EnergyBrakeAmplitude = dwParamValue;
			break;
		case AFTER_BRUST: 
			stSCRecipe.AfterBurst.bIsEnabled = dwParamValue;
			break;
		case AFTER_BRUST_DELAY: 
			stSCRecipe.AfterBurst.BurstDelay = dwParamValue;
			break;
		case AFTER_BRUST_TIME: 
			stSCRecipe.AfterBurst.BurstTime = dwParamValue;
			break;
		case AFTER_BRUST_AMPLITUDE: 
			stSCRecipe.AfterBurst.BurstAmplitude = dwParamValue;
			break;
		case EXTRA_COOLING: 
			stSCRecipe.bIsCoolingValveEnabled = dwParamValue;
			break;
		case MAX_WELD_TIMEOUT:
			stSCRecipe.MaxWeldTimeout = dwParamValue;
			break;
		case POST_WELD_SEEK: 
			stSCRecipe.bIsPostWeldSeekEnabled = dwParamValue;
			break;
		case PRE_WELD_SEEK: 
			stSCRecipe.bIsPreWeldSeekEnabled = dwParamValue;
			break;
		case TIMED_SEEK: 
			stSCRecipe.bIsTimedSeekEnabled = dwParamValue;
			break;
		case TIMED_SEEK_PERIOD: 
			stSCRecipe.TimedSeekPeriod = dwParamValue;
			break;
		case GLOBALSUSPECT: 
			stSCRecipe.bIsSuspectLimitEnabled = dwParamValue;
			break;
		case GLOBALREJECT: 
			stSCRecipe.bIsRejectLimitEnabled = dwParamValue;
			break;
		case SUSPECT_TIME_ENABLED: 
			stSCRecipe.SuspectLimit.Time.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_TIME_LOW_VALUE: 
			stSCRecipe.SuspectLimit.Time.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.Time.bIsLowEnabled = true;
			break;
		case SUSPECT_TIME_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.Time.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.Time.bIsHighEnabled = true;
			break;
		case REJECT_TIME_ENABLED: 
			stSCRecipe.RejectLimit.Time.bIsEnabled = dwParamValue;
			break;
		case REJECT_TIME_LOW_VALUE: 
			stSCRecipe.RejectLimit.Time.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.Time.bIsLowEnabled = true;
			break;
		case REJECT_TIME_HIGH_VALUE: 
			stSCRecipe.RejectLimit.Time.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.Time.bIsHighEnabled = true;
			break;
		case SUSPECT_ENERGY_ENABLED: 
			stSCRecipe.SuspectLimit.Energy.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_ENERGY_LOW_VALUE: 
			stSCRecipe.SuspectLimit.Energy.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.Energy.bIsLowEnabled = true;
			break;
		case SUSPECT_ENERGY_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.Energy.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.Energy.bIsHighEnabled = true;
			break;
		case REJECT_ENERGY_ENABLED: 
			stSCRecipe.RejectLimit.Energy.bIsEnabled = dwParamValue;
			break;
		case REJECT_ENERGY_LOW_VALUE: 
			stSCRecipe.RejectLimit.Energy.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.Energy.bIsLowEnabled = true;
			break;
		case REJECT_ENERGY_HIGH_VALUE: 
			stSCRecipe.RejectLimit.Energy.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.Energy.bIsHighEnabled = true;
			break;
		case SUSPECT_PEAKPOWER_ENABLED: 
			stSCRecipe.SuspectLimit.PeakPower.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_PEAKPOWER_LOW_VALUE: 
			stSCRecipe.SuspectLimit.PeakPower.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.PeakPower.bIsLowEnabled = true;
			break;
		case SUSPECT_PEAKPOWER_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.PeakPower.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.PeakPower.bIsHighEnabled = true;
			break;
		case REJECT_PEAKPOWER_ENABLED: 
			stSCRecipe.RejectLimit.PeakPower.bIsEnabled = dwParamValue;
			break;
		case REJECT_PEAKPOWER_LOW_VALUE: 
			stSCRecipe.RejectLimit.PeakPower.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.PeakPower.bIsLowEnabled = true;
			break;
		case REJECT_PEAKPOWER_HIGH_VALUE: 
			stSCRecipe.RejectLimit.PeakPower.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.PeakPower.bIsHighEnabled = true;
			break;
		case SUSPECT_ABSOLUTEDISTANCE_ENABLED: 
			stSCRecipe.SuspectLimit.AbsoluteDistance.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE: 
			stSCRecipe.SuspectLimit.AbsoluteDistance.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.AbsoluteDistance.bIsLowEnabled = true;
			break;
		case SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.AbsoluteDistance.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.AbsoluteDistance.bIsHighEnabled = true;
			break;
		case REJECT_ABSOLUTEDISTANCE_ENABLED: 
			stSCRecipe.RejectLimit.AbsoluteDistance.bIsEnabled = dwParamValue;
			break;
		case REJECT_ABSOLUTEDISTANCE_LOW_VALUE: 
			stSCRecipe.RejectLimit.AbsoluteDistance.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.AbsoluteDistance.bIsLowEnabled = true;
			break;
		case REJECT_ABSOLUTEDISTANCE_HIGH_VALUE: 
			stSCRecipe.RejectLimit.AbsoluteDistance.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.AbsoluteDistance.bIsHighEnabled = true;
			break;
		case SUSPECT_COLLAPSEDISTANCE_ENABLED: 
			stSCRecipe.SuspectLimit.CollapseDistance.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_COLLAPSEDISTANCE_LOW_VALUE: 
			stSCRecipe.SuspectLimit.CollapseDistance.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.CollapseDistance.bIsLowEnabled = true;
			break;
		case SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.CollapseDistance.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.CollapseDistance.bIsHighEnabled = true;
			break;
		case REJECT_COLLAPSEDISTANCE_ENABLED: 
			stSCRecipe.RejectLimit.CollapseDistance.bIsEnabled = dwParamValue;
			break;
		case REJECT_COLLAPSEDISTANCE_LOW_VALUE: 
			stSCRecipe.RejectLimit.CollapseDistance.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.CollapseDistance.bIsLowEnabled = true;
			break;
		case REJECT_COLLAPSEDISTANCE_HIGH_VALUE: 
			stSCRecipe.RejectLimit.CollapseDistance.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.CollapseDistance.bIsHighEnabled = true;
			break;
		case SUSPECT_TRIGGERDISTANCE_ENABLED: 
			stSCRecipe.SuspectLimit.TriggerDistance.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_TRIGGERDISTANCE_LOW_VALUE: 
			stSCRecipe.SuspectLimit.TriggerDistance.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.TriggerDistance.bIsLowEnabled = true;
			break;
		case SUSPECT_TRIGGERDISTANCE_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.TriggerDistance.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.TriggerDistance.bIsHighEnabled = true;
			break;
		case REJECT_TRIGGERDISTANCE_ENABLED: 
			stSCRecipe.RejectLimit.TriggerDistance.bIsEnabled = dwParamValue;
			break;
		case REJECT_TRIGGERDISTANCE_LOW_VALUE: 
			stSCRecipe.RejectLimit.TriggerDistance.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.TriggerDistance.bIsLowEnabled = true;
			break;
		case REJECT_TRIGGERDISTANCE_HIGH_VALUE: 
			stSCRecipe.RejectLimit.TriggerDistance.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.TriggerDistance.bIsHighEnabled = true;
			break;
		case SUSPECT_ENDWELDFORCE_ENABLED: 
			stSCRecipe.SuspectLimit.WeldForce.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_ENDWELDFORCE_LOW_VALUE: 
			stSCRecipe.SuspectLimit.WeldForce.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.WeldForce.bIsLowEnabled = true;
			break;
		case SUSPECT_ENDWELDFORCE_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.WeldForce.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.WeldForce.bIsHighEnabled = true;
			break;
		case REJECT_ENDWELDFORCE_ENABLED: 
			stSCRecipe.RejectLimit.WeldForce.bIsEnabled = dwParamValue;
			break;
		case REJECT_ENDWELDFORCE_LOW_VALUE: 
			stSCRecipe.RejectLimit.WeldForce.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.WeldForce.bIsLowEnabled = true;
			break;
		case REJECT_ENDWELDFORCE_HIGH_VALUE:
			stSCRecipe.RejectLimit.WeldForce.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.WeldForce.bIsHighEnabled = true;
			break;
		case SUSPECT_FREQUENCY_ENABLED: 
			stSCRecipe.SuspectLimit.Frequency.bIsEnabled = dwParamValue;
			break;
		case SUSPECT_FREQUENCY_LOW_VALUE: 
			stSCRecipe.SuspectLimit.Frequency.LowLimit = dwParamValue;
			stSCRecipe.SuspectLimit.Frequency.bIsLowEnabled = true;
			break;
		case SUSPECT_FREQUENCY_HIGH_VALUE: 
			stSCRecipe.SuspectLimit.Frequency.HighLimit = dwParamValue;
			stSCRecipe.SuspectLimit.Frequency.bIsHighEnabled = true;
			break;
		case REJECT_FREQUENCY_ENABLED: 
			stSCRecipe.RejectLimit.Frequency.bIsEnabled = dwParamValue;
			break;
		case REJECT_FREQUENCY_LOW_VALUE: 
			stSCRecipe.RejectLimit.Frequency.LowLimit = dwParamValue;
			stSCRecipe.RejectLimit.Frequency.bIsLowEnabled = true;
			break;
		case REJECT_FREQUENCY_HIGH_VALUE: 
			stSCRecipe.RejectLimit.Frequency.HighLimit = dwParamValue;
			stSCRecipe.RejectLimit.Frequency.bIsHighEnabled = true;
			break;
		case GLOBALCONTROL: 
			stSCRecipe.bIsCutoffLimitEnabled = dwParamValue;
			break;
		case PEAKPOWERCUTOFF: 
			stSCRecipe.CutoffLimit.PeakPowerCutoff.Value = dwParamValue;
			break;
		case PEAKPOWERCUTOFFENABLE: 
			stSCRecipe.CutoffLimit.PeakPowerCutoff.bIsEnabled = dwParamValue;
			break;
		case FREQUENCYLOWCUTOFF: 
			stSCRecipe.CutoffLimit.FrequencyLowCutoff.Value = dwParamValue;
			break;
		case FREQUENCYLOWCUTOFFENABLE: 
			stSCRecipe.CutoffLimit.FrequencyLowCutoff.bIsEnabled = dwParamValue;
			break;
		case ABSOLUTECUTOFF: 
			stSCRecipe.CutoffLimit.AbsoluteDistanceCutoff.Value = dwParamValue;
			break;
		case ABSOLUTECUTOFFENABLE: 
			stSCRecipe.CutoffLimit.AbsoluteDistanceCutoff.bIsEnabled = dwParamValue;
			break;
		case FREQUENCYHIGHCUTOFF: 
			stSCRecipe.CutoffLimit.FrequencyHighCutoff.Value = dwParamValue;
			break;
		case FREQUENCYHIGHCUTOFFENABLE: 
			stSCRecipe.CutoffLimit.FrequencyHighCutoff.bIsEnabled = dwParamValue;
			break;
		case COLLAPSECUTOFF: 
			stSCRecipe.CutoffLimit.CollapseDistanceCutoff.Value = dwParamValue;
			break;
		case COLLAPSECUTOFFENABLE: 
			stSCRecipe.CutoffLimit.CollapseDistanceCutoff.bIsEnabled = dwParamValue;
			break;
		case ENERGYCUTOFF: 
			stSCRecipe.CutoffLimit.EnergyCutoff.Value = dwParamValue;
			break;
		case ENERGYCUTOFFENABLE:
			stSCRecipe.CutoffLimit.EnergyCutoff.bIsEnabled = dwParamValue;
			break;
		case TIMECUTOFF: 
			stSCRecipe.CutoffLimit.TimeCutoff.Value = dwParamValue;
			break;
		case TIMECUTOFFENABLE: 
			stSCRecipe.CutoffLimit.TimeCutoff.bIsEnabled = dwParamValue;
			break;
		case GROUNDDETECTENABLE: 
			stSCRecipe.CutoffLimit.GroundDetectCutoff = dwParamValue;
			break;
		case FORCE_STEP_VALUE1: 
			stSCRecipe.ForceStepAtValue[0] = dwParamValue;
			break;
		case FORCE_STEP_VALUE2: 
			stSCRecipe.ForceStepAtValue[1] = dwParamValue;
			break;
		case FORCE_STEP_VALUE3: 
			stSCRecipe.ForceStepAtValue[2] = dwParamValue;
			break;
		case FORCE_STEP_VALUE4: 
			stSCRecipe.ForceStepAtValue[3] = dwParamValue;
			break;
		case FORCE_STEP_VALUE5: 
			stSCRecipe.ForceStepAtValue[4] = dwParamValue;
			break;
		case FORCE_STEP_VALUE6: 
			stSCRecipe.ForceStepAtValue[5] = dwParamValue;
			break;
		case FORCE_STEP_VALUE7: 
			stSCRecipe.ForceStepAtValue[6] = dwParamValue;
			break;
		case FORCE_STEP_VALUE8: 
			stSCRecipe.ForceStepAtValue[7] = dwParamValue;
			break;
		case FORCE_STEP_VALUE9: 
			stSCRecipe.ForceStepAtValue[8] = dwParamValue;
			break;
		case FORCE_STEP_VALUE10: 
			stSCRecipe.ForceStepAtValue[9] = dwParamValue;
			break;
		case REACTIVITY: 
			stSCRecipe.DynamicModeParam.Reactivity = dwParamValue;
			break;
		case FORCE_LEVEL_ENABLED: 
			stSCRecipe.DynamicModeParam.IsForceLevelEnabled = dwParamValue;
			break;
		case FORCE_LEVEL: 
			stSCRecipe.DynamicModeParam.ForceLevel = dwParamValue;
			break;
		case FORCE_LEVEL_TIME: 
			stSCRecipe.DynamicModeParam.ForceLevelTime = dwParamValue;
			break;
		case SCRUB_AMPLITUDE:
			stSCRecipe.ScrubAmplitude = dwParamValue;
			break;
		default:
			return MISMATCH_PARAMID;
		}
		/* Send SC Structure to User Interface to make it active*/
		message.msgID = TO_UI_TASK_WELD_RECIPE_SC;
		memset(message.Buffer,0x00,sizeof(message.Buffer));
		memcpy(message.Buffer,&stSCRecipe,sizeof(stSCRecipe));

		if(msgQSend(UI_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			LOGERR("Recipe : Message send failed to MSG Q",0,0,0);
		}				
	}
	/* Checking the ID is AC/PC parameter ID */
	else if((AC_PARAM == GetParamType(dwParamID)) || (PC_PARAM == GetParamType(dwParamID)))
	{
		WeldRecipeAC stACRecipe;		
		WeldRecipePC stPCRecipe;
		CP->m_RecipeAC.GetWeldRecipe(stACRecipe);
		CP->m_RecipePC.GetWeldRecipe(stPCRecipe);
		switch(dwParamID)
		{
		case FORCE_STEP1: 
			stACRecipe.ForceStepForce[0] = dwParamValue;
			stACRecipe.WeldForce = dwParamValue;
			break;
		case FORCE_STEP2:
			stACRecipe.ForceStepForce[1] = dwParamValue;
			break;
		case FORCE_STEP3: 
			stACRecipe.ForceStepForce[2] = dwParamValue;
			break;
		case FORCE_STEP4: 
			stACRecipe.ForceStepForce[3] = dwParamValue;
			break;
		case FORCE_STEP5: 
			stACRecipe.ForceStepForce[4] = dwParamValue;
			break;
		case FORCE_STEP6: 
			stACRecipe.ForceStepForce[5] = dwParamValue;
			break;
		case FORCE_STEP7: 
			stACRecipe.ForceStepForce[6] = dwParamValue;
			break;
		case FORCE_STEP8: 
			stACRecipe.ForceStepForce[7] = dwParamValue;
			break;
		case FORCE_STEP9: 
			stACRecipe.ForceStepForce[8] = dwParamValue;
			break;
		case FORCE_STEP10: 
			stACRecipe.ForceStepForce[9] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE1: 
			stACRecipe.ForceStepRampTime[0] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE2: 
			stACRecipe.ForceStepRampTime[1] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE3: 
			stACRecipe.ForceStepRampTime[2] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE4: 
			stACRecipe.ForceStepRampTime[3] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE5: 
			stACRecipe.ForceStepRampTime[4] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE6: 
			stACRecipe.ForceStepRampTime[5] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE7: 
			stACRecipe.ForceStepRampTime[6] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE8: 
			stACRecipe.ForceStepRampTime[7] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE9: 
			stACRecipe.ForceStepRampTime[8] = dwParamValue;
			break;
		case FORCE_STEP_RAMP_VALUE10: 
			stACRecipe.ForceStepRampTime[9] = dwParamValue;
			break;
		case FORCE_RAMP_TIME: 
			stACRecipe.ForceRampTime = dwParamValue;
			break;
		case HOLD_FORCE: 
			stACRecipe.HoldForce = dwParamValue;
			break;
		case HOLD_FORCE_RAMP_TIME: 
			stACRecipe.HoldForceRampTime = dwParamValue;
			break;
		case READY_POSITION: 
			stACRecipe.ReadyPosition = dwParamValue;
			break;
		case WELD_FORCE_CONTROL: 
			stACRecipe.WeldForceControl = dwParamValue;
			break;
		case READY_POSITION_TOGGLE: 
			stACRecipe.ReadyPositionToggle = dwParamValue;
			break;
		case EXPECTED_PART_CONTACT_POSITION: 
			stACRecipe.ExpectedPartContactPosition = dwParamValue;
			break;
		case PART_CONTACT_WINDOW_OFFSET: 
			stACRecipe.ExpectedPartContactOffset = dwParamValue;
			break;
		case PART_CONTACT_WINDOW_MINUS: 
			stACRecipe.PartContactWindowMinus = dwParamValue;
			break;
		case PART_CONTACT_WINDOW_PLUS: 
			stACRecipe.PartContactWindowPlus = dwParamValue;
			break;
		case DOWN_ACCELERATION: 
			stACRecipe.DownAcceleration = dwParamValue;
			break;
		case DOWN_MAX_VELOCITY: 
			stACRecipe.DownMaxVelocity = dwParamValue;
			break;
		case DOWN_DECELERATION: 
			stACRecipe.DownDeceleration = dwParamValue;
			break;
		case RETURN_ACCELERATION: 
			stACRecipe.ReturnAcceleration = dwParamValue;
			break;
		case RETURN_MAX_VELOCITY: 
			stACRecipe.ReturnMaxVelocity = dwParamValue;
			break;
		case RETURN_DECELERATION: 
			stACRecipe.ReturnDeceleration = dwParamValue;
			break;
		case WELD_RAMP_TIME: 
			stPCRecipe.WeldRampTime = dwParamValue;
			break;
		default:
			return MISMATCH_PARAMID;
		}
		/* Send PC + AC Structure to User Interface to make it active */
		message.msgID = TO_UI_TASK_WELD_RECIPE;
		memset(message.Buffer,0x00,sizeof(message.Buffer));
		memcpy(&message.Buffer[0],&stPCRecipe,sizeof(stPCRecipe));
		memcpy(&message.Buffer[sizeof(stPCRecipe)],&stACRecipe,sizeof(stACRecipe));

		if(msgQSend(UI_MSG_Q_ID,reinterpret_cast<char*>(&message),sizeof(message),NO_WAIT,MSG_PRI_NORMAL) != OK)
		{
			LOGERR("Recipe : Message send failed to MSG Q",0,0,0);
		}
	}
	else
	{
		return MISMATCH_PARAMID;
	}

	return WS_SUCCESS;
}

/**************************************************************************//**
 * \brief  - Validates the parameter ID received is in range or not
 * 
 * \param  - int dwParamID
 *
 * \return  - bool
 *
 ******************************************************************************/
bool Recipe::ValidateParamID(int dwParamID, char *chInputParamValue)
{
	/* for ASX blocking FORCE_STEP,FORCE_STEP_VALUE,FORCE_STEP_RAMP_VALUE between the range(3-10) */
	if( ((dwParamID >= FORCE_STEP4) && (dwParamID <= FORCE_STEP10)) || ((dwParamID >= FORCE_STEP_VALUE4) && (dwParamID <= FORCE_STEP_VALUE10))
		|| ((dwParamID >= FORCE_STEP_RAMP_VALUE4) && (dwParamID <= FORCE_STEP_RAMP_VALUE10)))
	{
		//if system is ASX
		if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
		{
			return false;
		}
	}
	
	if(dwParamID >= GLOBALSUSPECT && dwParamID <= GROUNDDETECTENABLE && RT.ModelFeature.RtFeature2.Limits_Flag_B38 == DISABLE)
		return false;
	else if(dwParamID >= PRETRIGGER && dwParamID <= PRETRIGGER_DISTANCE && RT.ModelFeature.RtFeature1.PreTrigger_Flag_B8 == DISABLE)
		return false;
	else if(dwParamID >= ENERGY_BRAKE && dwParamID <= ENERGY_BRAKE_AMPLITUDE && RT.ModelFeature.RtFeature1.EnergyBrake_Flag_B10 == DISABLE)
		return false;
	else if(dwParamID >= AFTER_BRUST && dwParamID <= AFTER_BRUST_AMPLITUDE && RT.ModelFeature.RtFeature1.AfterBurst_Flag_B9 == DISABLE)
		return false;
	else if(dwParamID >= AFTER_BRUST && dwParamID <= AFTER_BRUST_AMPLITUDE && RT.ModelFeature.RtFeature1.AfterBurst_Flag_B9 == DISABLE)
		return false;
	else if(dwParamID >= REACTIVITY && dwParamID <= FORCE_LEVEL_TIME && RT.ModelFeature.RtFeature2.Dynamic_Flag_B49 != ENABLE)
		return false;
	
	else if((WELD_MODE == dwParamID) && (WM_DYNAMIC == atoi(chInputParamValue)) && (ENABLE != RT.ModelFeature.RtFeature2.Dynamic_Flag_B49))
		return false;
	
	else if((dwParamID == AMPLITUDESTEPSCNT || dwParamID == AMPLITUDE_STEP_AT 
			|| (dwParamID >= AMPLITUDE_STEP_VALUE2 && dwParamID <= AMPLITUDE_STEP_VALUE10)
			|| (dwParamID >= AMPLITUDE_STEP1 && dwParamID <= AMPLITUDE_STEP10))
			&& RT.ModelFeature.RtFeature1.AmplitudeStep_Flag_B11 == DISABLE)
		return false;
	else if((dwParamID == NUM_FORCE_STEPS || dwParamID == FORCE_STEP_AT 
			|| (dwParamID >= FORCE_STEP2 && dwParamID <= FORCE_STEP10)
			|| (dwParamID >= WR_DB_FORCE_STEP_VALUE1 && dwParamID <= WR_DB_FORCE_STEP_VALUE10)
			|| (dwParamID >= WR_DB_FORCE_STEP_RAMP_VALUE1 && dwParamID <= WR_DB_FORCE_STEP_RAMP_VALUE10))
			&& RT.ModelFeature.RtFeature1.ForceStep_Flag_B12 == DISABLE)
		return false;
	else if(dwParamID > RECIPE_PARAM_IDS_START && dwParamID < RECIPE_PARAM_IDS_FINISH)
		return true;

	return false;
}

/**************************************************************************//**
 * \brief  - Validates the parameter value for particular ID received is in range or not
 * 
 * \param  - int dwParamID, char *chInputParamValue
 *
 * \return  - RecipeValidateStatus
 *
 ******************************************************************************/
RecipeValidateStatus Recipe::ValidateParamValueByID(int dwParamID, char *chInputParamValue)
{
	stringstream strInputParamValue; 
	string strWholeDecimal, strDecimalDigits;
	int decimalPoints = 0, dwStepAtMode = 0, prevParamID = 0, EPCValue = 0;
	//If any Absolute Distance value is selected make sure there is a Part Contact set first
	if((WELD_MODE == dwParamID) && (WM_ABSOLUTE_DISTANCE == atoi(chInputParamValue)))
	{
		GetParamValueByID(EXPECTED_PART_CONTACT_POSITION, EPCValue);
		if(0 >= EPCValue)
			return EXPECTED_PART_CONTACT_NOT_SET;
	}
	else if(((AMPLITUDE_STEP_AT == dwParamID) || (FORCE_STEP_AT == dwParamID)) && (STEP_AT_ABSOLUTE_DISTANCE == atoi(chInputParamValue)))
	{
		GetParamValueByID(EXPECTED_PART_CONTACT_POSITION, EPCValue);
		if(0 >= EPCValue)
			return EXPECTED_PART_CONTACT_NOT_SET;
	}
	/* Getting ParamIDs with units  
	 * Based on the Weld Mode or Step @ value (Amplitude or Force stepping)
	 * */
	else if(dwParamID == MODE_VALUE)
	{
		prevParamID = dwParamID;
		dwParamID = GetModeValueIndexByWeldMode();
	}
	else if (dwParamID >= AMPLITUDE_STEP_VALUE1 && dwParamID <= AMPLITUDE_STEP_VALUE10)
	{
		prevParamID = dwParamID;
		GetParamValueByID(AMPLITUDE_STEP_AT,dwStepAtMode);
		dwParamID = WELD_MODE_TIME + (dwStepAtMode - 1);
	}
	else if(dwParamID >= FORCE_STEP_VALUE1 && dwParamID <= FORCE_STEP_VALUE10)
	{
		prevParamID = dwParamID;
		GetParamValueByID(FORCE_STEP_AT,dwStepAtMode);
		dwParamID = WELD_MODE_TIME + (dwStepAtMode - 1);
	}
	
	/* Validating the decimal digits 
	 * Only sec, J and mm should have decimals, For others decimal is an error. 
	 * */
	strInputParamValue.str(chInputParamValue);
	getline(strInputParamValue, strWholeDecimal, '.');
	getline(strInputParamValue, strDecimalDigits);
	if(strDecimalDigits.size() == 0)
		decimalPoints = 0;
	else
		decimalPoints = strDecimalDigits.size();
	
	if(!strcmp(SystemConfiguration::systemTypeName,"ASX")) 
	{
	/* for collapse distance only 2 decimal points are allowed in ASX*/
		if((dwParamID == SUSPECT_COLLAPSEDISTANCE_LOW_VALUE) ||(dwParamID == SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE)
				||(dwParamID == REJECT_COLLAPSEDISTANCE_LOW_VALUE)  ||(dwParamID == REJECT_COLLAPSEDISTANCE_HIGH_VALUE) 	
				||(dwParamID == COLLAPSECUTOFF) ||(dwParamID == WELD_MODE_COLLAPSE_DISTANCE))
		{	
			if((decimalPoints > 2)) 
			{
				return WRONG_DECIMAL_VALUE;
			}
		}
		
	/* for absolute distance only 2 decimal points are allowed in ASX*/
		if((dwParamID == SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE) ||(dwParamID == SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE)
				||(dwParamID == REJECT_ABSOLUTEDISTANCE_LOW_VALUE)  ||(dwParamID == REJECT_ABSOLUTEDISTANCE_HIGH_VALUE) 	
				||(dwParamID == ABSOLUTECUTOFF) ||(dwParamID == WELD_MODE_ABSOLUTE_DISTANCE))
		{	
			if((decimalPoints > 2)) 
			{
				return WRONG_DECIMAL_VALUE;
			}
		}
	}
			
	string strUnit = m_MapRecipeParams[dwParamID].ParamUnit;
	
	/* We are accepting decimals only for sec, J and mm.
	 * For seconds we will accept up to 3 decimals. If there are more, then it´s invalid.
	 * For millimeters we will accept up to 3 decimals. If there are more then it´s invalid
	 *  For Joules we will accept 1 decimal. If there are more then it´s invalid
	 *  */
	if(!strcmp(strUnit.c_str(),"s") && decimalPoints > 3)
	{
		return WRONG_DECIMAL_VALUE;
	}
	else if(!strcmp(strUnit.c_str(),"mm") && 
			(decimalPoints > 3))
	{
		return WRONG_DECIMAL_VALUE;
	}
	else if(!strcmp(strUnit.c_str(),"J") && 
			(decimalPoints > 1))
	{
		return WRONG_DECIMAL_VALUE;
	}
	else if(strcmp(strUnit.c_str(),"s") && 
			strcmp(strUnit.c_str(),"mm") &&
			strcmp(strUnit.c_str(),"J") &&
			decimalPoints > 0)
		return WRONG_DECIMAL_VALUE;

	char chParamValue[8]={0};
	double dParamValue = atof(chInputParamValue);
	ConvertValueUnitForID(dwParamID,dParamValue,chParamValue,false);
	float dwParamValue = atof(chParamValue);

	/* Validating the Suspect Reject range */
	if(prevParamID == MODE_VALUE)
	{
		if(!ValidateParamValueRangeInSuspectRejectLimits(prevParamID,dwParamValue))
			return SUSPECT_REJECT__LIMITS_RANGE_MISMATCH;
	}
	else if((dwParamID >= FORCE_STEP1 && dwParamID <= FORCE_STEP10)
			|| dwParamID == TIMECUTOFF
			|| dwParamID == ENERGYCUTOFF
			|| dwParamID == PEAKPOWERCUTOFF
			|| dwParamID == ABSOLUTECUTOFF
			|| dwParamID == COLLAPSECUTOFF)
	{
		if(!ValidateParamValueRangeInSuspectRejectLimits(dwParamID,dwParamValue))
			return SUSPECT_REJECT__LIMITS_RANGE_MISMATCH;
	}

	/* validating the suspect reject values with param values */
	if(dwParamID >= GLOBALSUSPECT && dwParamID <= REJECT_FREQUENCY_HIGH_VALUE)
	{
		if(!ValidateSuspectRejectLimitsWithParamValue(dwParamID,dwParamValue))
			return SUSPECT_REJECT__LIMITS_RANGE_MISMATCH;
	}
	/* for ASX blocking Peakpower(3) and Ground detect(4) weld modes. */
	if(dwParamID == WELD_MODE)
	{
		int dwAsxParamValue = (int)dwParamValue;
		//if system is ASX
		if(!strcmp(SystemConfiguration::systemTypeName,"ASX")  && ((dwAsxParamValue == WM_POWER) || (dwAsxParamValue == WM_GROUND_DETECT)))
		{
			return MIN_MAX_RANGE_MISMATCH;
		}
	}
	
	/* Validating the min and max value range */	
	int dwMinValue = GetMinValue(dwParamID);
	if(dwParamID == FORCE_STEP1)
	{
		float minimumValue = dwMinValue / 10.0;
		dwMinValue = (int)round(minimumValue);
	}
	/* To validate with actual entered Parameter value */
	if(dwParamID == TRIGGER_FORCE || dwParamID == HOLD_FORCE || dwParamID == FORCE_STEP1 || dwParamID == FORCE_STEP2
		|| dwParamID == FORCE_STEP3 || dwParamID == FORCE_STEP4 || dwParamID == FORCE_STEP5 || dwParamID == FORCE_STEP6
		|| dwParamID == FORCE_STEP7 || dwParamID == FORCE_STEP8 || dwParamID == FORCE_STEP9 || dwParamID == FORCE_STEP10
		|| dwParamID == SUSPECT_ENDWELDFORCE_LOW_VALUE || dwParamID == SUSPECT_ENDWELDFORCE_HIGH_VALUE
		|| dwParamID == REJECT_ENDWELDFORCE_LOW_VALUE || dwParamID == REJECT_ENDWELDFORCE_HIGH_VALUE)
	{
		dwParamValue = dwParamValue / 10.0;
	}
	else if(dwParamID == DOWN_ACCELERATION || dwParamID == RETURN_ACCELERATION || dwParamID == DOWN_MAX_VELOCITY
			|| dwParamID == RETURN_MAX_VELOCITY || dwParamID == DOWN_DECELERATION || dwParamID == RETURN_DECELERATION)
	{
		dwParamValue = dwParamValue / 10000.0;
	}
	int dwMaxValue = GetMaxValue(dwParamID);
	if(dwParamID == TRIGGER_FORCE)
	{
		float maximumValue = dwMaxValue / 10.0;
		dwMaxValue = (int)round(maximumValue);
	}

	if(dwParamValue < dwMinValue || dwParamValue > dwMaxValue)
	{
		return MIN_MAX_RANGE_MISMATCH;
	}
	
	return  VALUE_INRANGE;
}

/**************************************************************************//**
 * \brief  - Validates the parameter value for particular suspect reject paramters
 * 			 received is in range or not
 * 
 * \param  - int dwParamID, int dwParamValue
 *
 * \return  - bool
 *
 ******************************************************************************/
bool Recipe::ValidateParamValueRangeInSuspectRejectLimits(int dwParamID, int dwParamValue)
{
	int dwIndex = 0, dwGlobalSuspect, dwGlobalReject;
	GetParamValueByID(GLOBALSUSPECT, dwGlobalSuspect);
	GetParamValueByID(GLOBALREJECT, dwGlobalReject);
	if(dwGlobalSuspect == 0 && dwGlobalReject == 0)
		return true;
	
	if(dwParamID == MODE_VALUE)
	{
		dwParamID = GetModeValueIndexByWeldMode();
		switch(dwParamID)
		{
		case WELD_MODE_TIME:
			dwIndex = SR_TIME;
			break;
		case WELD_MODE_ENERGY:
			dwIndex = SR_ENERGY;
			break;
		case WELD_MODE_POWER:
			dwIndex = SR_POWER;
			break;
		case WELD_MODE_ABSOLUTE_DISTANCE:
			dwIndex = SR_ABSOLUTE_DISTANCE;
			break;
		case WELD_MODE_COLLAPSE_DISTANCE:
			dwIndex = SR_COLLAPSE_DISTANCE;
			break;
		case WELD_MODE_SCRUB_TIME:
			dwIndex = SR_TRIGGER_DISTANCE;
			break;
		case WELD_MODE_DENSITY:
			//No SR
			dwIndex = 0;
			break;
		default:
			dwIndex = 0;
			break;
		}
	}
	else if(dwParamID >= FORCE_STEP1 && dwParamID <= FORCE_STEP10)
	{
		dwIndex = SR_END_WELD_FORCE;
	}
	else if(dwParamID == TIMECUTOFF)
	{
		dwIndex = SR_TIME;
	}
	else if(dwParamID == ENERGYCUTOFF)
	{
		dwIndex = SR_ENERGY;
	}
	else if(dwParamID == PEAKPOWERCUTOFF)
	{
		dwIndex = SR_POWER;
	}
	else if(dwParamID == ABSOLUTECUTOFF)
	{
		dwIndex = SR_ABSOLUTE_DISTANCE;
	}
	else if(dwParamID == COLLAPSECUTOFF)
	{
		dwIndex = SR_COLLAPSE_DISTANCE;
	}
	else
	{
		return true;
	}
	
	if(dwIndex == 0)
		return true;
	
	return ValidateValueWithSuspectRejectByID(dwIndex, dwParamValue);
}

/**************************************************************************//**
 * \brief  - Validates the parameter value for particular suspect reject paramters
 * 			 received is in range or not
 * 
 * \param  - int dwParamID, int dwValidateValue, int dwSRModifiedParam, int dwSRModifidParamValue
 *
 * \return  - bool
 *
 ******************************************************************************/
bool Recipe::ValidateValueWithSuspectRejectByID(int dwParamID, int dwValidateValue, int dwSRModifiedParam, int dwSRModifidParamValue)
{
	int dwGlobalSuspect = 0, dwSuspect = 0, dwSuspectLow = 0, dwSuspectLowValue, dwSuspectHigh = 0, dwSuspectHighValue; 
	int dwGlobalReject = 0, dwReject = 0, dwRejectLow = 0, dwRejectLowValue, dwRejectHigh = 0, dwRejectHighValue; 
	int dwIndex = 0,dwLocalIndex = 0;
	switch(dwParamID)
	{
	case SR_TIME:
		dwIndex = SUSPECT_TIME_ENABLED;
		dwLocalIndex = SUSPECT_TIME_LOW_ENABLED;
		break;
	case SR_ENERGY:
		dwIndex = SUSPECT_ENERGY_ENABLED;
		dwLocalIndex = SUSPECT_ENERGY_LOW_ENABLED;
		break;
	case SR_POWER:
		dwIndex = SUSPECT_PEAKPOWER_ENABLED;
		dwLocalIndex = SUSPECT_PEAKPOWER_LOW_ENABLED;
		break;
	case SR_ABSOLUTE_DISTANCE:
		dwIndex = SUSPECT_ABSOLUTEDISTANCE_ENABLED;
		dwLocalIndex = SUSPECT_ABSOLUTEDISTANCE_LOW_ENABLED;
		break;
	case SR_COLLAPSE_DISTANCE:
		dwIndex = SUSPECT_COLLAPSEDISTANCE_ENABLED;
		dwLocalIndex = SUSPECT_COLLAPSEDISTANCE_LOW_ENABLED;
		break;
	case SR_TRIGGER_DISTANCE:
		dwIndex = SUSPECT_TRIGGERDISTANCE_ENABLED;
		dwLocalIndex = SUSPECT_TRIGGERDISTANCE_LOW_ENABLED;
		break;
	case SR_END_WELD_FORCE:
		dwIndex = SUSPECT_ENDWELDFORCE_ENABLED;
		dwLocalIndex = SUSPECT_ENDWELDFORCE_LOW_ENABLED;
		break;
	case SR_FREQUENCY:
		dwIndex = SUSPECT_FREQUENCY_ENABLED;
		dwLocalIndex = SUSPECT_FREQUENCY_LOW_ENABLED;
		break;
	default:
		dwIndex = 0;
		break;
	}
	GetParamValueByID(GLOBALSUSPECT, dwGlobalSuspect);
	GetParamValueByID(GLOBALREJECT, dwGlobalReject);
	
	GetParamValueByID(dwIndex, dwSuspect);
	GetParamValueByID(dwIndex+1, dwSuspectLowValue);
	GetParamValueByID(dwIndex+2, dwSuspectHighValue);
	GetParamValueByID(dwIndex+3, dwReject);
	GetParamValueByID(dwIndex+4, dwRejectLowValue);
	GetParamValueByID(dwIndex+5, dwRejectHighValue);

	GetParamValueByID(dwLocalIndex, dwSuspectLow);
	GetParamValueByID(dwLocalIndex+1, dwSuspectHigh);
	GetParamValueByID(dwLocalIndex+2, dwRejectLow);
	GetParamValueByID(dwLocalIndex+3, dwRejectHigh);
	
	switch(dwSRModifiedParam)
	{
	case GLOBAL_SUSPECT:
		dwGlobalSuspect = dwSRModifidParamValue;
		break;
	case GLOBAL_REJECT:
		dwGlobalReject = dwSRModifidParamValue;
		break;
	case SUSPECT_ENABLED:
		dwSuspect = dwSRModifidParamValue;
		break;
	case SUSPECT_LOW_ENABLED:
		dwSuspectLow = dwSRModifidParamValue;
		break;
	case SUSPECT_HIGH_ENABLED:
		dwSuspectHigh = dwSRModifidParamValue;
		break;
	case SUSPECT_LOW_VALUE:
		dwSuspectLowValue = dwSRModifidParamValue;
		break;
	case SUSPECT_HIGH_VALUE:
		dwSuspectHighValue = dwSRModifidParamValue;
		break;
	case REJECT_ENABLED:
		dwReject = dwSRModifidParamValue;
		break;
	case REJECT_LOW_ENABLED:
		dwRejectLow = dwSRModifidParamValue;
		break;
	case REJECT_HIGH_ENABLED:
		dwRejectHigh = dwSRModifidParamValue;
		break;
	case REJECT_LOW_VALUE:
		dwRejectLowValue = dwSRModifidParamValue;
		break;
	case REJECT_HIGH_VALUE:
		dwRejectHighValue = dwSRModifidParamValue;
		break;
	default:
		break;
	}
	if(dwIndex == 0)
		return true;
	return ValidateValueWithSuspectRejectByValues(dwValidateValue,
			dwGlobalSuspect, dwSuspect, dwSuspectLow, dwSuspectLowValue, dwSuspectHigh, dwSuspectHighValue,
			dwGlobalReject, dwReject, dwRejectLow, dwRejectLowValue, dwRejectHigh, dwRejectHighValue);
}

/**************************************************************************//**
 * \brief  - Validates the suspect reject paramters are correct or not
 * 
 * \param  -int dwParamValue,
 *			int dwGlobalSuspect, 	int dwSuspect, 
 *			int dwSuspectLow, 		int dwSuspectLowValue, 	int dwSuspectHigh, 	int dwSuspectHighValue, 
 *			int dwGlobalReject, 	int dwReject, 
 *			int dwRejectLow, 		int dwRejectLowValue, 	int dwRejectHigh, 	int dwRejectHighValue
 *
 * \return  - bool
 *
 ******************************************************************************/
bool Recipe::ValidateValueWithSuspectRejectByValues(int dwParamValue,
		int dwGlobalSuspect, int dwSuspect, 
		int dwSuspectLow, int dwSuspectLowValue, int dwSuspectHigh, int dwSuspectHighValue, 
		int dwGlobalReject, int dwReject, 
		int dwRejectLow, int dwRejectLowValue, int dwRejectHigh, int dwRejectHighValue)
{
	int dwMinValue = -1, dwMaxValue = -1;

	if(dwGlobalReject && dwReject && dwRejectLow)
		dwMinValue = dwRejectLowValue;
	else if(dwGlobalSuspect && dwSuspect && dwSuspectLow)
		dwMinValue = dwSuspectLowValue;
	
	if(dwGlobalSuspect && dwSuspect && dwSuspectHighValue)
		dwMaxValue = dwSuspectHighValue;
	else if(dwGlobalReject && dwReject && dwRejectHighValue)
		dwMaxValue = dwRejectHighValue;
	
	if(dwMinValue == -1 && dwMaxValue == -1)
		return true;
	
	if(dwParamValue < dwMinValue || dwParamValue > dwMaxValue)
		return false;

	return true;
}

/**************************************************************************//**
 * \brief  - Validates the parameter value for particular suspect reject paramters
 * 			 received is in range or not
 * 
 * \param  - int dwParamID, int dwParamValue
 *
 * \return  - bool
 *
 ******************************************************************************/
bool Recipe::ValidateSuspectRejectLimitsWithParamValue(int dwParamID, int dwParamValue)
{
	int dwGlobalSuspect, dwGlobalReject;
	int dwMinValue, dwMaxValue, dwModeValue, dwMode, dwTempValue,dwSRParamID,dwSRParamLocalIndex;
	int dwLocalMode;
	bool isCheckRequired = false;
	bool bReturnStatus = false;

	GetParamValueByID(GLOBALSUSPECT, dwGlobalSuspect);
	GetParamValueByID(GLOBALREJECT, dwGlobalReject);
	GetParamValueByID(MODE_VALUE, dwModeValue);
	GetParamValueByID(WELD_MODE, dwMode);

	if(dwParamID != GLOBALSUSPECT || dwParamID != GLOBALREJECT )
	{
		if(dwParamID == SUSPECT_TIME_ENABLED || 
				dwParamID == SUSPECT_TIME_LOW_VALUE ||
				dwParamID == SUSPECT_TIME_HIGH_VALUE ||
				dwParamID == SUSPECT_ENERGY_ENABLED ||
				dwParamID == SUSPECT_ENERGY_LOW_VALUE ||
				dwParamID == SUSPECT_ENERGY_HIGH_VALUE ||
				dwParamID == SUSPECT_PEAKPOWER_ENABLED ||
				dwParamID == SUSPECT_PEAKPOWER_LOW_VALUE ||
				dwParamID == SUSPECT_PEAKPOWER_HIGH_VALUE ||
				dwParamID == SUSPECT_ABSOLUTEDISTANCE_ENABLED ||
				dwParamID == SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE ||
				dwParamID == SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE ||
				dwParamID == SUSPECT_COLLAPSEDISTANCE_ENABLED ||
				dwParamID == SUSPECT_COLLAPSEDISTANCE_LOW_VALUE ||
				dwParamID == SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE ||
				dwParamID == SUSPECT_TRIGGERDISTANCE_ENABLED ||
				dwParamID == SUSPECT_TRIGGERDISTANCE_LOW_VALUE ||
				dwParamID == SUSPECT_TRIGGERDISTANCE_HIGH_VALUE ||
				dwParamID == SUSPECT_ENDWELDFORCE_ENABLED ||
				dwParamID == SUSPECT_ENDWELDFORCE_LOW_VALUE ||
				dwParamID == SUSPECT_ENDWELDFORCE_HIGH_VALUE ||
				dwParamID == SUSPECT_FREQUENCY_ENABLED ||
				dwParamID == SUSPECT_FREQUENCY_LOW_VALUE ||
				dwParamID == SUSPECT_FREQUENCY_HIGH_VALUE)
		{
			if(!dwGlobalSuspect)
				return true;
		}
		else
		{
			if(!dwGlobalReject)
				return true;
		}
	}
	
	
	switch(dwParamID)
	{
	case GLOBALSUSPECT:
		if(!dwParamValue)
		{
			return true;
		}
		switch(dwMode)
		{
		case 1:
			dwSRParamID = SR_TIME;
			break;
		case 2:
			dwSRParamID = SR_ENERGY;
			break;
		case 3:
			dwSRParamID = SR_POWER;
			break;
		case 4:
			dwSRParamID = SR_ABSOLUTE_DISTANCE;
			break;
		case 5:
			dwSRParamID = SR_COLLAPSE_DISTANCE;
			break;
		case 6:
			dwSRParamID = SR_TRIGGER_DISTANCE;
			break;
		}
		bReturnStatus = ValidateValueWithSuspectRejectByID(dwSRParamID,dwModeValue,GLOBAL_SUSPECT,dwParamValue);
		if(!bReturnStatus)
			return false;
		
		bReturnStatus = ValidateValueWithSuspectRejectByID(SR_END_WELD_FORCE,dwModeValue,GLOBAL_SUSPECT,dwParamValue);
		
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = GLOBAL_SUSPECT;
		break;
	case GLOBALREJECT:
		if(!dwParamValue)
		{
			return true;
		}
		switch(dwMode)
		{
		case 1:
			dwSRParamID = SR_TIME;
			break;
		case 2:
			dwSRParamID = SR_ENERGY;
			break;
		case 3:
			dwSRParamID = SR_POWER;
			break;
		case 4:
			dwSRParamID = SR_ABSOLUTE_DISTANCE;
			break;
		case 5:
			dwSRParamID = SR_COLLAPSE_DISTANCE;
			break;
		case 6:
			dwSRParamID = SR_TRIGGER_DISTANCE;
			break;
		}
		bReturnStatus = ValidateValueWithSuspectRejectByID(dwSRParamID,dwModeValue,GLOBAL_REJECT,dwParamValue);
		if(!bReturnStatus)
			return false;
		
		bReturnStatus = ValidateValueWithSuspectRejectByID(SR_END_WELD_FORCE,dwModeValue,GLOBAL_REJECT,dwParamValue);
		
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = GLOBAL_REJECT;
		break;
	case SUSPECT_TIME_ENABLED:
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_TIME;
		dwSRParamID = SR_TIME;
		dwSRParamLocalIndex = SUSPECT_LOW_ENABLED;
		break;
	case SUSPECT_TIME_LOW_VALUE:
		dwLocalMode = WM_TIME;
		dwSRParamID = SR_TIME;
		dwSRParamLocalIndex = SUSPECT_LOW_VALUE;
		break;
	case SUSPECT_TIME_HIGH_VALUE:
		dwLocalMode = WM_TIME;
		dwSRParamID = SR_TIME;
		dwSRParamLocalIndex = SUSPECT_HIGH_VALUE;
		break;
	case REJECT_TIME_ENABLED:
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_TIME;
		dwSRParamID = SR_TIME;
		dwSRParamLocalIndex = REJECT_ENABLED;
		break;
	case REJECT_TIME_LOW_VALUE:
		dwLocalMode = WM_TIME;
		dwSRParamID = SR_TIME;
		dwSRParamLocalIndex = REJECT_LOW_VALUE;
		break;
	case REJECT_TIME_HIGH_VALUE:
		dwLocalMode = WM_TIME;
		dwSRParamID = SR_TIME;
		dwSRParamLocalIndex = REJECT_HIGH_VALUE;
		break;
	case SUSPECT_ENERGY_ENABLED:
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_ENERGY;
		dwSRParamID = SR_ENERGY;
		dwSRParamLocalIndex = SUSPECT_ENABLED;
		break;
	case SUSPECT_ENERGY_LOW_VALUE:
		dwLocalMode = WM_ENERGY;
		dwSRParamID = SR_ENERGY;
		dwSRParamLocalIndex = SUSPECT_LOW_VALUE;
		break;
	case SUSPECT_ENERGY_HIGH_VALUE:
		dwLocalMode = WM_ENERGY;
		dwSRParamID = SR_ENERGY;
		dwSRParamLocalIndex = SUSPECT_HIGH_VALUE;
		break;
	case REJECT_ENERGY_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_ENERGY;
		dwSRParamID = SR_ENERGY;
		dwSRParamLocalIndex = REJECT_ENABLED;
		break;
	case REJECT_ENERGY_LOW_VALUE:
		dwLocalMode = WM_ENERGY;
		dwSRParamID = SR_ENERGY;
		dwSRParamLocalIndex = REJECT_LOW_VALUE;
		break;
	case REJECT_ENERGY_HIGH_VALUE:
		dwLocalMode = WM_ENERGY;
		dwSRParamID = SR_ENERGY;
		dwSRParamLocalIndex = REJECT_HIGH_VALUE;
		break;
	case SUSPECT_PEAKPOWER_ENABLED:
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_POWER;
		dwSRParamID = SR_POWER;
		dwSRParamLocalIndex = SUSPECT_ENABLED;
		break;
	case SUSPECT_PEAKPOWER_LOW_VALUE:
		dwLocalMode = WM_POWER;
		dwSRParamID = SR_POWER;
		dwSRParamLocalIndex = SUSPECT_LOW_VALUE;
		break;
	case SUSPECT_PEAKPOWER_HIGH_VALUE:
		dwLocalMode = WM_POWER;
		dwSRParamID = SR_POWER;
		dwSRParamLocalIndex = SUSPECT_HIGH_VALUE;
		break;
	case REJECT_PEAKPOWER_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_POWER;
		dwSRParamID = SR_POWER;
		dwSRParamLocalIndex = REJECT_ENABLED;
		break;
	case REJECT_PEAKPOWER_LOW_VALUE:
		dwLocalMode = WM_POWER;
		dwSRParamID = SR_POWER;
		dwSRParamLocalIndex = REJECT_LOW_VALUE;
		break;
	case REJECT_PEAKPOWER_HIGH_VALUE:
		dwLocalMode = WM_POWER;
		dwSRParamID = SR_POWER;
		dwSRParamLocalIndex = REJECT_HIGH_VALUE;
		break;
	case SUSPECT_ABSOLUTEDISTANCE_ENABLED:
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_ABSOLUTE_DISTANCE;
		dwSRParamID = SR_ABSOLUTE_DISTANCE;
		dwSRParamLocalIndex = SUSPECT_ENABLED;
		break;
	case SUSPECT_ABSOLUTEDISTANCE_LOW_VALUE:
		dwLocalMode = WM_ABSOLUTE_DISTANCE;
		dwSRParamID = SR_ABSOLUTE_DISTANCE;
		dwSRParamLocalIndex = SUSPECT_LOW_VALUE;
		break;
	case SUSPECT_ABSOLUTEDISTANCE_HIGH_VALUE:
		dwLocalMode = WM_ABSOLUTE_DISTANCE;
		dwSRParamID = SR_ABSOLUTE_DISTANCE;
		dwSRParamLocalIndex = SUSPECT_HIGH_VALUE;
		break;
	case REJECT_ABSOLUTEDISTANCE_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_ABSOLUTE_DISTANCE;
		dwSRParamID = SR_ABSOLUTE_DISTANCE;
		dwSRParamLocalIndex = REJECT_ENABLED;
		break;
	case REJECT_ABSOLUTEDISTANCE_LOW_VALUE:
		dwLocalMode = WM_ABSOLUTE_DISTANCE;
		dwSRParamID = SR_ABSOLUTE_DISTANCE;
		dwSRParamLocalIndex = REJECT_LOW_VALUE;
		break;
	case REJECT_ABSOLUTEDISTANCE_HIGH_VALUE:
		dwLocalMode = WM_ABSOLUTE_DISTANCE;
		dwSRParamID = SR_ABSOLUTE_DISTANCE;
		dwSRParamLocalIndex = REJECT_HIGH_VALUE;
		break;
	case SUSPECT_COLLAPSEDISTANCE_ENABLED:
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_COLLAPSE_DISTANCE;
		dwSRParamID = SR_COLLAPSE_DISTANCE;
		dwSRParamLocalIndex = SUSPECT_ENABLED;
		break;
	case SUSPECT_COLLAPSEDISTANCE_LOW_VALUE:
		dwLocalMode = WM_COLLAPSE_DISTANCE;
		dwSRParamID = SR_COLLAPSE_DISTANCE;
		dwSRParamLocalIndex = SUSPECT_LOW_VALUE;
		break;
	case SUSPECT_COLLAPSEDISTANCE_HIGH_VALUE:
		dwLocalMode = WM_COLLAPSE_DISTANCE;
		dwSRParamID = SR_COLLAPSE_DISTANCE;
		dwSRParamLocalIndex = SUSPECT_HIGH_VALUE;
		break;
	case REJECT_COLLAPSEDISTANCE_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwLocalMode = WM_COLLAPSE_DISTANCE;
		dwSRParamID = SR_COLLAPSE_DISTANCE;
		dwSRParamLocalIndex = REJECT_ENABLED;
		break;
	case REJECT_COLLAPSEDISTANCE_LOW_VALUE:
		dwLocalMode = WM_COLLAPSE_DISTANCE;
		dwSRParamID = SR_COLLAPSE_DISTANCE;
		dwSRParamLocalIndex = REJECT_LOW_VALUE;
		break;
	case REJECT_COLLAPSEDISTANCE_HIGH_VALUE:
		dwLocalMode = WM_COLLAPSE_DISTANCE;
		dwSRParamID = SR_COLLAPSE_DISTANCE;
		dwSRParamLocalIndex = REJECT_HIGH_VALUE;
		break;
	case SUSPECT_ENDWELDFORCE_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwSRParamID = SR_END_WELD_FORCE;
		dwSRParamLocalIndex = SUSPECT_ENABLED;
		break;
	case SUSPECT_ENDWELDFORCE_LOW_VALUE:
		dwSRParamID = SR_END_WELD_FORCE;
		dwSRParamLocalIndex = SUSPECT_LOW_VALUE;
		break;
	case SUSPECT_ENDWELDFORCE_HIGH_VALUE:
		dwSRParamID = SR_END_WELD_FORCE;
		dwSRParamLocalIndex = SUSPECT_HIGH_VALUE;
		break;
	case REJECT_ENDWELDFORCE_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwSRParamID = SR_END_WELD_FORCE;
		dwSRParamLocalIndex = REJECT_ENABLED;
		break;
	case REJECT_ENDWELDFORCE_LOW_VALUE:
		dwSRParamID = SR_END_WELD_FORCE;
		dwSRParamLocalIndex = REJECT_LOW_VALUE;
		break;
	case REJECT_ENDWELDFORCE_HIGH_VALUE:
		dwSRParamID = SR_END_WELD_FORCE;
		dwSRParamLocalIndex = REJECT_HIGH_VALUE;
		break;
	case SUSPECT_FREQUENCY_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = SUSPECT_ENABLED;
		break;
	case SUSPECT_FREQUENCY_LOW_VALUE:
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = SUSPECT_LOW_VALUE;
		break;
	case SUSPECT_FREQUENCY_HIGH_VALUE:
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = SUSPECT_HIGH_VALUE;
		break;
	case REJECT_FREQUENCY_ENABLED: 
		if(!dwParamValue)
		{
			bReturnStatus = true;
			break;
		}
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = REJECT_ENABLED;
		break;
	case REJECT_FREQUENCY_LOW_VALUE:
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = REJECT_LOW_VALUE;
		break;
	case REJECT_FREQUENCY_HIGH_VALUE:
		dwSRParamID = SR_FREQUENCY;
		dwSRParamLocalIndex = REJECT_HIGH_VALUE;
		break;
	default:
		break;
	}
	
	if(dwMode != dwLocalMode && dwMode != 0)
	{
		bReturnStatus = true;
	}
	else
	{
		bReturnStatus = ValidateValueWithSuspectRejectByID(dwSRParamID,dwModeValue,dwSRParamLocalIndex,dwParamValue);
	}

	return bReturnStatus;
}

/**************************************************************************//**
 * \brief  - Returns the converted parameter value for specific ID 
 * 
 * \param  - int dwParamID, char *chParamValue
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS Recipe::GetParamValue(int dwParamID, char *chParamValue)
{
	if(!ValidateParamID(dwParamID, chParamValue))
		return MISMATCH_PARAMID;

	int dwValue;
	WS_STATUS status = GetParamValueByID(dwParamID, dwValue);
	int dwStepAt = 0;
	/* Converting Amplitude step values based on step at value */
	if((dwParamID >= AMPLITUDE_STEP_VALUE1) && (dwParamID <= AMPLITUDE_STEP_VALUE10))
	{
		GetParamValueByID(AMPLITUDE_STEP_AT,dwStepAt);
		if((dwStepAt == 1))
		{
			dwParamID = WELD_MODE_TIME;			
		}
		if((dwStepAt == 2))
		{
			dwParamID = WELD_MODE_ENERGY;			
		}
		else if(dwStepAt == 4) 
		{		
			dwParamID = WELD_MODE_ABSOLUTE_DISTANCE;			
		}
		else if(dwStepAt == 5)
		{
			dwParamID = WELD_MODE_COLLAPSE_DISTANCE;
		}
	}
	/* Converting Force step values based on step at value */
	else if((dwParamID >= FORCE_STEP_VALUE1) && (dwParamID <= FORCE_STEP_VALUE10))
	{
		GetParamValueByID(FORCE_STEP_AT,dwStepAt);
		if((dwStepAt == 1))
		{
			dwParamID = WELD_MODE_TIME;
		}
		if((dwStepAt == 2))
		{
			dwParamID = WELD_MODE_ENERGY;			
		}
		else if(dwStepAt == 4) 
		{		
			dwParamID = WELD_MODE_ABSOLUTE_DISTANCE;			
		}
		else if(dwStepAt == 5)
		{
			dwParamID = WELD_MODE_COLLAPSE_DISTANCE;
		}
	}
	ConvertValueUnitForID(dwParamID,dwValue,chParamValue,true);
	return status;
}

/**************************************************************************//**
 * \brief  - Sets the converted parameter value for specific ID 
 * 
 * \param  - int dwParamID, char *chParamValue
 *
 * \return  - WS_STATUS
 *
 ******************************************************************************/
WS_STATUS Recipe::SetParamValue(int dwParamID, char *chParamValue)
{
	double dParamValue = atof(chParamValue);
	if(!ValidateParamID(dwParamID,chParamValue)) 
	{
		return MISMATCH_PARAMID;
	}
	if (ValidateParamValueByID(dwParamID,chParamValue))
	{
		return DATA_NOT_FOUND_IN_SPECIFIEDRANGE;
	}
	return SetParamValueByID(dwParamID, dParamValue);
}

/**************************************************************************//**
 * \brief  - For a given parameter return the type of recipe it belongs to. (SC,AC or PC)
 * 
 * \param  - int dwParamID
 *
 * \return  - ParamType
 *
 ******************************************************************************/
ParamType Recipe::GetParamType(int dwParamID)
{
	ParamType TypeOfParam;
	
	if((dwParamID >= WELD_MODE && dwParamID <= FORCE_STEP_AT)
			|| (dwParamID >= HOLD_TIME && dwParamID <= FORCE_STEP_VALUE10)
				|| (dwParamID >= REACTIVITY && dwParamID <= SCRUB_AMPLITUDE))
		TypeOfParam = SC_PARAM;
	else if((dwParamID >= FORCE_STEP1 && dwParamID <= FORCE_STEP10)
			|| (dwParamID >= FORCE_STEP_RAMP_VALUE1 && dwParamID <= RETURN_DECELERATION))
		TypeOfParam = AC_PARAM;
	else if (dwParamID == WELD_RAMP_TIME)
		TypeOfParam = PC_PARAM;
	else
		TypeOfParam = UNKNOWN_PARAM;
	
		return TypeOfParam;
}
