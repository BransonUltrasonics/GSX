/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Weld Recipe Parameter Data

***************************************************************************/
#include "Header/weldrecipeparameterdata.h"
#include "Header/communicationinterface.h"
#include "Header/configuration.h"
#include "Header/informationpanel.h"
#include "Header/login.h"
#include "Header/common.h"
#include "Header/localizeheader.h"
#include "Header/menuhandler.h"
#include "Header/stackrecipedata.h"
#include <QQmlContext>
#include <QRegularExpression>
#include "Header/utility.h"
#define WELD_RECIPE_PARAM_EXP_PART_CONT           "EXPECTED PART CONTACT"
#define WELD_RECIPE_PARAM_EXP_CONTACT_OFFSET      "EXPECTED CONTACT OFFSET"
#define WELD_RECIPE_PARAM_PART_CON_WIN_MINUS      "PART CONTACT WINDOW MINUS"


extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern InformationPanel *PrimaryData;
extern Login *login;
extern UserPrivilege *userPrivilageObj;
extern StackRecipeData *obj_stackrecipedata;


/**
 * @brief WeldRecipeParameter::WeldRecipeParameter :  Values assign to corresponding parameter
 */
WeldRecipeParameter::WeldRecipeParameter()
{
    m_UnitSelected = UNIT_SI;
    m_CurrentSystemFreq = SYSTEM_TYPE_20KHZ;
    m_IsActiveRequired = false;
    m_oldRecipeNumber = 0;
    m_ParamaterChangedFlag = SEND_ALL_PARAM;
    createRecipeParamaterStructure();
}

/**
 * @brief WeldRecipeParameter::addNewParmToMap : This function used to map default values according to KZ
 * @param qstrMetricUnit
 * @param qstrImperialUnit
 * @param qstr20KDefaultValue
 * @param qstr20KMinValue
 * @param qstr20KMaxValue
 * @param qstr30KDefaultValue
 * @param qstr30KMinValue
 * @param qstr30KMaxValue
 * @param qstr40KDefaultValue
 * @param qstr40KMinValue
 * @param qstr40KMaxValue
 * @param qstrParamName
 * @param dwIndex
 */
void WeldRecipeParameter::addNewParmToMap(QString qstrMetricUnit, QString qstrImperialUnit,
                                          QString qstr20KDefaultValue, QString qstr20KMinValue, QString qstr20KMaxValue,
                                          QString qstr30KDefaultValue, QString qstr30KMinValue, QString qstr30KMaxValue,
                                          QString qstr40KDefaultValue, QString qstr40KMinValue, QString qstr40KMaxValue,
                                          QString qstrParamName, int dwIndex)
{
    stRecipeParmInfo stTempParam;
    stTempParam.ParamIndexInStructure                   = dwIndex;
    stTempParam.ParamName                               = qstrParamName;
    stTempParam.ParamUnit[UNIT_SI]                      = qstrMetricUnit;
    stTempParam.ParamUnit[UNIT_SAE]                     = qstrImperialUnit;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ]    = qstr20KDefaultValue;
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ]        = qstr20KMinValue;
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ]        = qstr20KMaxValue;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ]    = qstr30KDefaultValue;
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ]        = qstr30KMinValue;
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ]        = qstr30KMaxValue;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ]    = qstr40KDefaultValue;
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ]        = qstr40KMinValue;
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ]        = qstr40KMaxValue;
    m_MapRecipeParams[qstrParamName]                    = stTempParam;
}

/**
 * @brief WeldRecipeParameter::initializeRecipeParamaterStructure : Initialize the parameter structure
 */
void WeldRecipeParameter::initializeRecipeParamaterStructure(int tmpRecipeNum)
{
    getDefaultFrequencySelection(tmpRecipeNum);
}

/**
 * @brief WeldRecipeParameter::createRecipeParamaterStructure : To save all recipe min max and default values
 */
void WeldRecipeParameter::createRecipeParamaterStructure()
{
    //              SIUnit  SAEUnit 20_DEF  20_Min  20_MAX  30_DEF  30_MIN  30_MAX  40_DEF  40_MIN  40_MAX  Name        Index
    //SC Structure
    addNewParmToMap("",     "",     "0",    "1",    "1000", "0",    "1",    "1000", "0",    "1",    "1000", "recipenumber",SC_RECIPE_NUMBER);
    addNewParmToMap("",     "",     "0",    "1",    "1000", "0",    "1",    "1000", "0",    "1",    "1000", "versionnumber",SC_RECIPE_VER_NUMBER);
    addNewParmToMap("",     "",     "1",    "1",    "6",    "1",    "1",    "6",    "1",    "1",    "6",    "weldmode",SC_WELD_MODE);
    addNewParmToMap("s",    "s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "modevalue",SC_MODE_VALUE);
    addNewParmToMap("s",    "s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "time",SC_MODE_VALUE_TIME);
    addNewParmToMap("J",    "J",    "1.0",    "1.0","120000","1.0",   "1.0",    "45000.0","1.0",    "1.0",    "24000.0","energy",SC_MODE_VALUE_ENERGY);
    addNewParmToMap("W",    "W",    "400",  "1",    "4000", "150",  "1",    "1500", "80",   "1",    "800",  "peak power",SC_MODE_VALUE_POWER);
    addNewParmToMap("mm",   "in",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  "absolute distance",SC_MODE_VALUE_AD);
    addNewParmToMap("mm",   "in",   "0.010","0.010","25",   "0.010","0.010","25",   "0.010","0.010","25",   "collapse distance",SC_MODE_VALUE_CD);
    addNewParmToMap("s",    "s",    "0.001","0.001","0.5",  "0.001","0.001","0.5",  "0.001","0.001","0.5",  "scrub time",SC_MODE_VALUE_ST);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "scrub amplitude",SC_SCRUB_AMPLITUDE);
    addNewParmToMap("s",    "s",    "0.001","0.001","0.5",  "0.001","0.001","0.5",  "0.001","0.001","0.5",  "ground detect",SC_MODE_VALUE_GD);
    addNewParmToMap("%",    "%",    "0",    "0",    "100",  "0",    "0",    "100",  "0",    "0",    "100",  "dynamic",SC_MODE_VALUE_DY);
    addNewParmToMap("%",    "%",    "0",    "0",    "100",  "0",    "0",    "100",  "0",    "0",    "100",  "density",SC_MODE_VALUE_DENSITY);
    addNewParmToMap("%",    "%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  "reactivity",SC_REACTIVITY);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "force level enable",SC_FORCE_LEVEL_ENABLED);
    addNewParmToMap("%",    "%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  "force level",SC_FORCE_LEVEL);
    addNewParmToMap("s",    "s",    "0",    "0",    "0.3",  "0",    "0",    "0.3",  "0",    "0",    "0.3",  "force level time",SC_FORCE_LEVEL_TIME);

    addNewParmToMap("s",    "s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "hold time",SC_HOLD_TIME);
    addNewParmToMap("N",    "lbs",  "25",   "5",    "2500", "25",   "5",    "2500", "25",   "5",   "2500", "trigger force",SC_TRIGGER_FORCE);
    addNewParmToMap("mm",   "in",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  "trigger distance",SC_TRIGGER_DISTANCE);
    addNewParmToMap("s",    "s",    "6",    "0.05", "30",   "6",    "0.05", "30",   "6",    "0.05", "30",   "max timeout",SC_MAX_WELD_TIMEOUT);
    addNewParmToMap("",     "",     "1",    "1",    "10",   "1",    "1",    "10",   "1",    "1",    "10",   "numamplitudesteps",SC_NUM_AMPLITUDE_STEPS);
    addNewParmToMap("",     "",     "1",    "1",    "7",    "1",    "1",    "7",    "1",    "1",    "7",    "amplitudestepat",SC_AMPLITUDE_STEP_AT);
    addNewParmToMap("",     "",     "1",    "1",    "10",   "1",    "1",    "10",   "1",    "1",    "10",   "numforcesteps",SC_NUM_FORCE_STEPS);
    addNewParmToMap("",     "",     "1",    "1",    "7",    "1",    "1",    "7",    "1",    "1",    "7",    "forcestepat",SC_FORCE_STEP_AT);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 1",SC_FORCE_STEP_VALUE1);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 2",SC_FORCE_STEP_VALUE2);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 3",SC_FORCE_STEP_VALUE3);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 4",SC_FORCE_STEP_VALUE4);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 5",SC_FORCE_STEP_VALUE5);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 6",SC_FORCE_STEP_VALUE6);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 7",SC_FORCE_STEP_VALUE7);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 8",SC_FORCE_STEP_VALUE8);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 9",SC_FORCE_STEP_VALUE9);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "force step value 10",SC_FORCE_STEP_VALUE10);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 1",SC_AMPLITUDE_STEP_VALUE1);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 2",SC_AMPLITUDE_STEP_VALUE2);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 3",SC_AMPLITUDE_STEP_VALUE3);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 4",SC_AMPLITUDE_STEP_VALUE4);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 5",SC_AMPLITUDE_STEP_VALUE5);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 6",SC_AMPLITUDE_STEP_VALUE6);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 7",SC_AMPLITUDE_STEP_VALUE7);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 8",SC_AMPLITUDE_STEP_VALUE8);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 9",SC_AMPLITUDE_STEP_VALUE9);
    addNewParmToMap("",     "",     "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "amplitude step value 10",SC_AMPLITUDE_STEP_VALUE10);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "weld amplitude",SC_AMPLITUDE_STEP1);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 2",SC_AMPLITUDE_STEP2);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 3",SC_AMPLITUDE_STEP3);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 4",SC_AMPLITUDE_STEP4);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 5",SC_AMPLITUDE_STEP5);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 6",SC_AMPLITUDE_STEP6);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 7",SC_AMPLITUDE_STEP7);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 8",SC_AMPLITUDE_STEP8);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 9",SC_AMPLITUDE_STEP9);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "amplitude 10",SC_AMPLITUDE_STEP10);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "extra cooling",SC_COOLING_VALVE);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "global suspect",SC_GLOBAL_SUSPECT);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "global reject",SC_GLOBAL_REJECT);
    addNewParmToMap("",     "",     "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "control",SC_CONTROL_LIMIT);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "peak power cutoff state",SC_CL_PEAKPOWER_CUTOFF);
    addNewParmToMap("W",    "W",    "400",  "1",    "4000", "150",  "1",    "1500", "80",   "1",    "800",  "peak power cutoff",SC_CL_PEAKPOWER_CUTOFF_VAL);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "absolute distance cutoff state",SC_CL_ABSOLUTE_CUTOFF);
    addNewParmToMap("mm",   "in",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  "absolute distance cutoff",SC_CL_ABSOLUTE_CUTOFF_VAL);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "collapse distance cutoff state",SC_CL_COLLAPSE_CUTOFF);
    addNewParmToMap("mm",   "in",   "25",   "0.01", "25",   "25",   "0.01", "25",   "25",   "0.01", "25",   "collapse distance cutoff",SC_CL_COLLAPSE_CUTOFF_VAL);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "time cutoff state",SC_CL_TIME_CUTOFF);
    addNewParmToMap("s",    "s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   "time cutoff",SC_CL_TIME_CUTOFF_VAL);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "frequency low cutoff state",SC_CL_FREQ_LOW);
    addNewParmToMap("Hz",   "Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", "frequency low cutoff",SC_CL_FREQ_LOW_VAL);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "frequency high cutoff state",SC_CL_FREQ_HIGH);
    addNewParmToMap("Hz",   "Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", "frequency high cutoff",SC_CL_FREQ_HIGH_VAL);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "energy cutoff state",SC_CL_ENERGY_CUTOFF);
    addNewParmToMap("J",    "J",    "120000","1.0",   "120000","45000.0","1.0",   "45000.0","24000.0","1.0",    "24000.0","energy cutoff",SC_CL_ENERGY_CUTOFF_VAL);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "ground detect cutoff",SC_CL_GROUND_DETECT_CUTOFF);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "pretrigger",SC_PRETRIGGER);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "distanceauto",SC_AUTO_PRETRIGGER);
    addNewParmToMap("",     "",     "1",    "0",    "1",    "1",    "0",    "1",    "1",    "0",    "1",    "pretrigger distance auto",SC_DISTANCE_PRETRIGGER);
    addNewParmToMap("mm",   "in",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  "pretrigger distance",SC_PRETRIGGER_DISTANCE);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "pretrigger amplitude",SC_PRETRIGGER_AMPLITUDE);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "afterburst",SC_AFTER_BURST);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "afterburst amplitude",SC_AB_AMPLITUDE);
    addNewParmToMap("s",    "s",    "0.1",  "0.050","2",    "0.1",  "0.050","2",    "0.1",  "0.050","2",    "afterburst delay",SC_AB_DELAY);
    addNewParmToMap("s",    "s",    "0.1",  "0.1",  "2",    "0.1",  "0.1",  "2",    "0.1",  "0.1",  "2",    "afterburst time",SC_AB_TIME);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "energy brake",SC_ENERGY_BRAKE);
    addNewParmToMap("%",    "%",    "3",    "1",    "100",  "3",    "1",    "100",  "3",    "1",    "100",  "energy brake amplitude",SC_ENERGY_BRAKE_AMPLITUDE);
    addNewParmToMap("s",    "s",    "0.02", "0.01", "1",    "0.02", "0.01", "1",    "0.02", "0.01", "1",    "energy brake time",SC_ENERGY_BRAKE_TIME);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect time",SC_SUSPECT_TIME_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect time low",SC_SUSPECT_TIME_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect time high",SC_SUSPECT_TIME_HIGH_ENABLED);
    addNewParmToMap("s",    "s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "suspect time low value",SC_SUSPECT_TIME_LOW);
    addNewParmToMap("s",    "s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   "suspect time high value",SC_SUSPECT_TIME_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect energy",SC_SUSPECT_ENERGY_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect energy low",SC_SUSPECT_ENERGY_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect energy high",SC_SUSPECT_ENERGY_HIGH_ENABLED);
    addNewParmToMap("J",    "J",    "1.0",    "1.0",    "120000","1.0",   "1.0",    "45000.0","1.0",    "1.0",    "24000.0","suspect energy low value",SC_SUSPECT_ENERGY_LOW);
    addNewParmToMap("J",    "J",    "120000","1.0",   "120000","45000.0","1.0",   "45000.0","24000.0","1.0",    "24000.0","suspect energy high value",SC_SUSPECT_ENERGY_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect peak power",SC_SUSPECT_POWER_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect peak power low",SC_SUSPECT_POWER_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect peak power high",SC_SUSPECT_POWER_HIGH_ENABLED);
    addNewParmToMap("W",    "W",    "1",    "1",    "4000", "1",    "1",    "1500", "1",    "1",    "800",  "suspect peak power low value",SC_SUSPECT_POWER_LOW);
    addNewParmToMap("W",    "W",    "4000", "1",    "4000", "1500", "1",    "1500", "800",  "1",    "800",  "suspect peak power high value",SC_SUSPECT_POWER_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect collapse distance",SC_SUSPECT_CD_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect collapse distance low",SC_SUSPECT_CD_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect collapse distance high",SC_SUSPECT_CD_HIGH_ENABLED);
    addNewParmToMap("mm",   "in",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   "suspect collapse distance low value",SC_SUSPECT_CD_LOW);
    addNewParmToMap("mm",   "in",   "25",   "0.01", "25",   "25",   "0.01", "25",   "25",   "0.01", "25",   "suspect collapse distance high value",SC_SUSPECT_CD_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect absolute distance",SC_SUSPECT_AD_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect absolute distance low",SC_SUSPECT_AD_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect absolute distance high",SC_SUSPECT_AD_HIGH_ENABLED);
    addNewParmToMap("mm",   "in",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  "suspect absolute distance low value",SC_SUSPECT_AD_LOW);
    addNewParmToMap("mm",   "in",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  "suspect absolute distance high value",SC_SUSPECT_AD_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect trigger distance",SC_SUSPECT_TD_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect trigger distance low",SC_SUSPECT_TD_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect trigger distance high",SC_SUSPECT_TD_HIGH_ENABLED);
    addNewParmToMap("mm",   "in",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  "suspect trigger distance low value",SC_SUSPECT_TD_LOW);
    addNewParmToMap("mm",   "in",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  "suspect trigger distance high value",SC_SUSPECT_TD_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect end weld force",SC_SUSPECT_END_FORCE_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect end weld force low",SC_SUSPECT_END_FORCE_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect end weld force high",SC_SUSPECT_END_FORCE_HIGH_ENABLED);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "suspect end weld force low value",SC_SUSPECT_END_FORCE_LOW);
    addNewParmToMap("N",    "lbs",  "2500", "25",   "2500", "2500", "25",   "2500", "2500", "25",   "2500", "suspect end weld force high value",SC_SUSPECT_END_FORCE_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect frequency",SC_SUSPECT_FREQ_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect frequency low",SC_SUSPECT_FREQ_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "suspect frequency high",SC_SUSPECT_FREQ_HIGH_ENABLED);
    addNewParmToMap("Hz",   "Hz",   "1",    "1",    "500",  "1",    "1",    "750",  "1",    "1",    "1000", "suspect frequency low value",SC_SUSPECT_FREQ_LOW);
    addNewParmToMap("Hz",   "Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", "suspect frequency high value",SC_SUSPECT_FREQ_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject time",SC_REJECT_TIME_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject time low",SC_REJECT_TIME_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject time high",SC_REJECT_TIME_HIGH_ENABLED);
    addNewParmToMap("s",    "s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "reject time low value",SC_REJECT_TIME_LOW);
    addNewParmToMap("s",    "s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   "reject time high value",SC_REJECT_TIME_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject energy",SC_REJECT_ENERGY_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject energy low",SC_REJECT_ENERGY_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject energy high",SC_REJECT_ENERGY_HIGH_ENABLED);
    addNewParmToMap("J",    "J",    "1.0",    "1.0",    "120000","1.0",   "1.0",    "45000.0","1.0",    "1.0",    "24000.0","reject energy low value",SC_REJECT_ENERGY_LOW);
    addNewParmToMap("J",    "J",    "120000","1.0",   "120000","45000.0","1.0",   "45000.0","24000.0","1.0",    "24000.0","reject energy high value",SC_REJECT_ENERGY_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject peak power",SC_REJECT_POWER_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject peak power low",SC_REJECT_POWER_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject peak power high",SC_REJECT_POWER_HIGH_ENABLED);
    addNewParmToMap("W",    "W",    "1",    "1",    "4000", "1",    "1",    "1500", "1",    "1",    "800",  "reject peak power low value",SC_REJECT_POWER_LOW);
    addNewParmToMap("W",    "W",    "4000", "1",    "4000", "1500", "1",    "1500", "800",  "1",    "800",  "reject peak power high value",SC_REJECT_POWER_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject collapse distance",SC_REJECT_CD_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject collapse distance low",SC_REJECT_CD_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject collapse distance high",SC_REJECT_CD_HIGH_ENABLED);
    addNewParmToMap("mm",   "in",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   "0.01", "0.01", "25",   "reject collapse distance low value",SC_REJECT_CD_LOW);
    addNewParmToMap("mm",   "in",   "25",   "0.01", "25",   "25",   "0.01", "25",   "25",   "0.01", "25",   "reject collapse distance high value",SC_REJECT_CD_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject absolute distance",SC_REJECT_AD_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject absolute distance low",SC_REJECT_AD_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject absolute distance high",SC_REJECT_AD_HIGH_ENABLED);
    addNewParmToMap("mm",   "in",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  "reject absolute distance low value",SC_REJECT_AD_LOW);
    addNewParmToMap("mm",   "in",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  "reject absolute distance high value",SC_REJECT_AD_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject trigger distance",SC_REJECT_TD_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject trigger distance low",SC_REJECT_TD_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject trigger distance high",SC_REJECT_TD_HIGH_ENABLED);
    addNewParmToMap("mm",   "in",   "3",    "3",    "125",  "3",    "3",    "125",  "3",    "3",    "125",  "reject trigger distance low value",SC_REJECT_TD_LOW);
    addNewParmToMap("mm",   "in",   "125",  "3",    "125",  "125",  "3",    "125",  "125",  "3",    "125",  "reject trigger distance high value",SC_REJECT_TD_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject end weld force",SC_REJECT_END_FORCE_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject end weld force low",SC_REJECT_END_FORCE_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject end weld force high",SC_REJECT_END_FORCE_HIGH_ENABLED);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "reject end weld force low value",SC_REJECT_END_FORCE_LOW);
    addNewParmToMap("N",    "lbs",  "2500", "25",   "2500", "2500", "25",   "2500", "2500", "25",   "2500", "reject end weld force high value",SC_REJECT_END_FORCE_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject frequency",SC_REJECT_FREQ_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject frequency low",SC_REJECT_FREQ_LOW_ENABLED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "reject frequency high",SC_REJECT_FREQ_HIGH_ENABLED);
    addNewParmToMap("Hz",   "Hz",   "1",    "1",    "500",  "1",    "1",    "750",  "1",    "1",    "1000", "reject frequency low value",SC_REJECT_FREQ_LOW);
    addNewParmToMap("Hz",   "Hz",   "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", "reject frequency high value",SC_REJECT_FREQ_HIGH);
    addNewParmToMap("",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "user",SC_USER);
    addNewParmToMap("",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "stackserialno",SC_STACK_SERIAL_NO);
    addNewParmToMap("",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "recipename", UI_RECIPE_NAME);
    addNewParmToMap("",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "companyname", UI_COMPANYNAME);
    addNewParmToMap("min",  "min",  "1",    "1",    "59",   "1",    "1",    "59",   "1",    "1",    "59",   "timed seek period",SC_TIMED_SEEK_PERIOD);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "timed seek",SC_TIMED_SEEK);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "pre-weld seek",SC_PRE_WELD_SEEK);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "post-weld seek",SC_POST_WELD_SEEK);
    // PC Structure
    addNewParmToMap("KHz",  "KHz",  "100",  "1",    "20000","205",  "1",    "20000","205",  "1",    "20000","amplitudeloopc1",PC_AMPLITUDE_LOOPC1);
    addNewParmToMap("KHz",  "KHz",  "300",  "1",    "20000","370",  "1",    "20000","370",  "1",    "20000","amplitudeloopc2",PC_AMPLITUDE_LOOPC2);
    addNewParmToMap("KHz",  "KHz",  "450",  "50",   "500",  "100",  "50",   "500",  "60",   "50",   "500",  "phase loop",PC_PHASE_LOOP);
    addNewParmToMap("KHz",  "KHz",  "450",  "50",   "500",  "200",  "50",   "500",  "200",  "50",   "500",  "phase loop cf",PC_PHASE_LOOPCF);
    addNewParmToMap("Hz",   "Hz",   "500",  "19450",  "20450",  "750",  "29250",   "30750",  "1000", "38950", "40950", "frequency low",PC_FREQUENCY_LOW);
    addNewParmToMap("Hz",   "Hz",   "500",  "19450",  "20450",  "750",  "29250",   "30750",  "1000", "38950", "40950", "frequency high",PC_FREQUENCY_HIGH);
    addNewParmToMap("s",    "s",    "0.5",  "0.001","20",   "0.5",  "0.001","20",   "0.5",  "0.001","20",   "phase limit time",PC_PHASE_LIMIT_TIME);
    addNewParmToMap("",     "",     "1000", "4",    "9765", "1000", "4",    "9765", "1000", "4",    "9765", "phase limit",PC_PHASE_LIMIT);
    addNewParmToMap("",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "control mode",PC_CONTROL_MODE);
    addNewParmToMap("",     "",     "0",    "0",    "1000", "0",    "0",    "1000", "0",    "0",    "1000", "blind time seek",PC_BLIND_TIME_SEEK);
    addNewParmToMap("",     "",     "0",    "0",    "1000", "0",    "0",    "1000", "0",    "0",    "1000", "blind time weld",PC_BLIND_TIME_WELD);
    addNewParmToMap("",     "",     "500",  "0",    "1000", "200",  "0",    "1000", "200",  "0",    "1000", "dds switch time",PC_DDS_SWITCH_TIME);
    addNewParmToMap("",     "",     "800",  "0",    "10000","1200", "0",    "10000","0",    "0",    "10000","parameter7",PC_PARAMETER7);
    addNewParmToMap("",     "",     "1000", "0",    "10000","1000", "0",    "10000","0",    "0",    "10000","parameter8",PC_PARAMETER8);
    addNewParmToMap("",     "",     "200",  "0",    "10000","200",  "0",    "10000","0",    "0",    "10000","parameter9",PC_PARAMETER9);
    addNewParmToMap("",     "",     "5",    "0",    "10000","5",    "0",    "10000","0",    "0",    "10000","parameter10",PC_PARAMETER10);
    addNewParmToMap("",     "",     "0",    "0",    "10000","0",    "0",    "10000","0",    "0",    "10000","parameter11",PC_PARAMETER11);
    addNewParmToMap("",     "",     "0",    "0",    "10000","0",    "0",    "10000","0",    "0",    "10000","parameter12",PC_PARAMETER12);
    addNewParmToMap("",     "",     "20000","0",   "100000","20000","0",   "100000","20000","0",   "100000","parameter13",PC_PARAMETER13);
    addNewParmToMap("",     "",     "20000","0",   "100000","20000","0",   "100000","20000","0",   "100000","parameter14",PC_PARAMETER14);
    addNewParmToMap("",     "",     "20000","0",   "100000","20000","0",   "100000","20000","0",   "100000","parameter15",PC_PARAMETER15);
    addNewParmToMap("s",    "s",    "1",    "0",    "1",    "1",    "0",    "1",    "1",    "0",    "1",    "f_limit time",PC_F_LIMIT_TIME);
    addNewParmToMap("KHz",  "KHz",  "100",  "1",    "500",  "205",  "1",    "500",  "205",  "1",    "500",  "amp proportional gain",PC_AMP_PROPORTIONAL_GAIN);
    addNewParmToMap("KHz",  "KHz",  "5",    "1",    "500",  "5",    "1",    "500",  "5",    "1",    "500",  "amp integral gain",PC_AMP_INTEGRAL_GAIN);
    addNewParmToMap("KHz",  "KHz",  "22",   "1",    "500",  "25",   "1",    "500",  "41",   "1",    "500",  "phase integral gain",PC_PHASE_INTEGRAL_GAIN);
    addNewParmToMap("KHz",  "KHz",  "5",    "1",    "500",  "5",    "1",    "500",  "5",    "1",    "500",  "phase proportional gain",PC_PHASE_PROPORTIONAL_GAIN);
    addNewParmToMap("",     "",     "100",  "1",    "500",  "100",  "1",    "500",  "100",  "1",    "500",  "frequency window size",PC_FREQUENCY_WINDOW_SIZE);
    addNewParmToMap("KHz",  "KHz",  "450",  "1",    "500",  "200",  "1",    "500",  "200",  "1",    "500",  "pf phase control loop",PC_PF_OF_PHASE_CONTROL_LOOP);
    addNewParmToMap("KHz",  "KHz",  "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", "pi frequency low",PC_PI_FREQUENCY_LOW);
    addNewParmToMap("KHz",  "KHz",  "500",  "1",    "500",  "750",  "1",    "750",  "1000", "1",    "1000", "pi frequency high",PC_PI_FREQUENCY_HIGH);
    addNewParmToMap("s",    "s",    "0.5",  "0.001","20",   "0.5",  "0.001","20",   "0.5",  "0.001","20",   "pi phase limit time",PC_PI_PHASE_LIMIT_TIME);
    addNewParmToMap("",     "",     "1000", "4",    "9765", "1000", "4",    "9765", "1000", "4",    "9765", "pi phase limit",PC_PI_PHASE_LIMIT);
    addNewParmToMap("s",    "s",    "0.08", "0.01", "1",    "0.08", "0.01", "1",    "0.08", "0.01", "1",    "weld ramp time",PC_WELD_RAMP_TIME);
    addNewParmToMap("Hz",   "Hz",   "19950","19450","20450","30000","29250","30750","39900","39100","40700","start frequency",PC_START_FREQUENCY);
    addNewParmToMap("",     "",     "0",    "0",    "10000","0",    "0",    "10000","0",    "0",    "10000","memory offset",PC_MEMORY_OFFSET);
    addNewParmToMap("Hz",   "Hz",   "19950","19450","20450","30000","29250","30750","39900","39100","40700","digital tune",PC_DIGITAL_TUNE);
    addNewParmToMap("Hz",   "Hz",   "19950","19450","20450","30000","29250","30750","39900","39100","40700","frequency offset",PC_FREQUENCY_OFFSET);
    //AC Structure
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "weld force",AC_WELD_FORCE);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force ramp time",AC_FORCE_RAMP_TIME);
    addNewParmToMap("",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "",     "holdmode",AC_HOLD_MODE);
    addNewParmToMap("mm",   "in",   "0",    "0",    "50",   "0",    "0",    "50",   "0",    "0",    "50",   "total collapse target",AC_TOTAL_COLLAPSE_TARGET);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "hold force",AC_HOLD_FORCE);
    addNewParmToMap("s",    "s",    "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "hold force ramp time",AC_HOLD_FORCE_RAMP_TIME);
    addNewParmToMap("mm",   "in",   "0",    "0",    "125",  "0",    "0",    "125",  "0",    "0",    "125",  "expected part contact",AC_EXPECTED_PART_CONTACT_POSITION);
    addNewParmToMap("mm",   "in",   "5",    "5",    "120",  "5",    "5",    "120",  "5",    "5",    "120",  "ready offset",AC_READY_POSITION);
    addNewParmToMap("mm/s2","in/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000","down acceleration",AC_DOWN_ACCELERATION);
    addNewParmToMap("mm/s2","in/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000", "down deceleration",AC_DOWN_DECELERATION);
    addNewParmToMap("mm/s", "in/s", "50",   "5",    "150",  "50",   "5",    "150",  "50",   "5",    "150",  "down max velocity",AC_DOWN_MAX_VELOCITY);
    addNewParmToMap("mm/s2","in/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000", "return acceleration",AC_RETURN_ACCELERATION);
    addNewParmToMap("mm/s2","in/s2","500",  "1",    "10000","500",  "1",    "10000","500",  "1",    "10000", "return deceleration",AC_RETURN_DECELERATION);
    addNewParmToMap("mm/s", "in/s", "50",   "5",    "150",  "50",   "5",    "150",  "50",   "5",    "150",  "return max velocity",AC_RETURN_MAX_VELOCITY);
    addNewParmToMap("mm",   "in",   "0.01", "0.01", "5",    "0.01", "0.01", "5",    "0.01", "0.01", "5",    "expected contact offset",AC_PART_CONTANCT_OFFSET);
    addNewParmToMap("mm",   "in",   "2",    "0.01", "10",   "2",    "0.01", "10",   "2",    "0.01", "10",   "part contact window minus",AC_PART_CONTANCT_WINDOW_MINUS);
    addNewParmToMap("mm",   "in",   "2",    "0.01", "10",   "2",    "0.01", "10",   "2",    "0.01", "10",   "part contact window plus",AC_PART_CONTANCT_WINDOW_PLUS);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 1",AC_FORCE_STEP_RAMP_VALUE1);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 2",AC_FORCE_STEP_RAMP_VALUE2);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 3",AC_FORCE_STEP_RAMP_VALUE3);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 4",AC_FORCE_STEP_RAMP_VALUE4);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 5",AC_FORCE_STEP_RAMP_VALUE5);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 6",AC_FORCE_STEP_RAMP_VALUE6);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 7",AC_FORCE_STEP_RAMP_VALUE7);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 8",AC_FORCE_STEP_RAMP_VALUE8);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 9",AC_FORCE_STEP_RAMP_VALUE9);
    addNewParmToMap("s",    "s",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "0.1",  "0",    "1",    "force step ramp value 10",AC_FORCE_STEP_RAMP_VALUE10);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 1",AC_FORCE_STEP1);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 2",AC_FORCE_STEP2);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 3",AC_FORCE_STEP3);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 4",AC_FORCE_STEP4);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 5",AC_FORCE_STEP5);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 6",AC_FORCE_STEP6);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 7",AC_FORCE_STEP7);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 8",AC_FORCE_STEP8);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 9",AC_FORCE_STEP9);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "force step 10",AC_FORCE_STEP10);
    addNewParmToMap("",     "",     "1",    "0",    "1",    "1",    "0",    "1",    "1",    "0",    "1",    "ready position toggle",AC_READY_POSITION_TOGGLE);
    addNewParmToMap("",     "",     "3",    "1",    "3",    "3",    "1",    "3",    "3",    "1",    "3",    "weld force control",AC_WELD_FORCE_CONTROL);

    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "global setup",UI_GLOBAL_SETUP);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup weld mode status",UI_SETUP_WELD_MODE_STATUS);
    addNewParmToMap("s",    "s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "setup weld mode low",UI_SETUP_WELD_MODE_LOW);
    addNewParmToMap("s",    "s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   "setup weld mode high",UI_SETUP_WELD_MODE_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup weld amplitude status",UI_WELD_AMP_STATUS);
    addNewParmToMap("%",    "%",    "10",   "10",   "100",  "10",   "10",   "100",  "10",   "10",   "100",  "setup weld amplitude low",UI_WELD_AMP_LOW);
    addNewParmToMap("%",    "%",    "100",  "10",   "100",  "100",  "10",   "100",  "100",  "10",   "100",  "setup weld amplitude high",UI_WELD_AMP_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup weld force status",UI_WELD_FORCE_STATUS);
    addNewParmToMap("N",    "lbs",  "50",   "25",   "2500", "50",   "25",   "2500", "50",   "25",   "2500", "setup weld force low",UI_WELD_FORCE_LOW);
    addNewParmToMap("N",    "lbs",  "2500", "25",   "2500", "2500", "25",   "2500", "2500", "25",   "2500", "setup weld force high",UI_WELD_FORCE_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup trigger force status",UI_TRIGGER_FORCE_STATUS);
    addNewParmToMap("N",    "lbs",  "25",   "5",    "2500", "25",   "5",    "2500", "25",   "5",    "2500", "setup trigger force low",UI_TRIGGER_FORCE_LOW);
    addNewParmToMap("N",    "lbs",  "2500", "5",    "2500", "2500", "5",    "2500", "2500", "5",    "2500", "setup trigger force high",UI_TRIGGER_FORCE_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup hold force status",UI_HOLD_FORCE_STATUS);
    addNewParmToMap("N",    "lbs",  "25",   "25",   "2500", "25",   "25",   "2500", "25",   "25",   "2500", "setup hold force low",UI_HOLD_FORCE_LOW);
    addNewParmToMap("N",    "lbs",  "2500", "25",   "2500", "2500", "15",   "2500", "2500", "25",   "2500", "setup hold force high",UI_HOLD_FORCE_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup hold time status",UI_HOLD_TIME_STATUS);
    addNewParmToMap("s",    "s",    "0.010","0.010","30",   "0.010","0.010","30",   "0.010","0.010","30",   "setup hold time low",UI_HOLD_TIME_LOW);
    addNewParmToMap("s",    "s",    "30",   "0.010","30",   "30",   "0.010","30",   "30",   "0.010","30",   "setup hold time high",UI_HOLD_TIME_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup scrub amplitude status",UI_SCRUB_AMP_STATUS);
    addNewParmToMap("%",    "%",    "1",    "1",    "100",  "1",    "1",    "100",  "1",    "1",    "100",  "setup scrub amplitude low",UI_SCRUB_AMP_LOW);
    addNewParmToMap("%",    "%",    "100",  "1",    "100",  "100",  "1",    "100",  "100",  "1",    "100",  "setup scrub amplitude high",UI_SCRUB_AMP_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup scrub time status",UI_SCRUB_TIME_STATUS);
    addNewParmToMap("s",    "s",    "0.001","0.001","0.5",  "0.001","0.001","0.5",  "0.001","0.001","0.5",  "setup scrub time low",UI_SCRUB_TIME_LOW);
    addNewParmToMap("s",    "s",    "0.5",  "0.001","0.5",  "0.5",  "0.001","0.5",  "0.5",  "0.001","0.5",  "setup scrub time high",UI_SCRUB_TIME_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup reactivity status",UI_REACTIVITY_STATUS);
    addNewParmToMap("%",    "%",    "0",    "0",    "100",  "0",    "0",    "100",  "0",    "0",    "100",  "setup reactivity low",UI_REACTIVITY_LOW);
    addNewParmToMap("%",    "%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  "setup reactivity high",UI_REACTIVITY_HIGH);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "setup force level status",UI_FORCE_LEVEL_STATUS);
    addNewParmToMap("%",    "%",    "0",    "0",    "100",  "0",    "0",    "100",  "0",    "0",    "100",  "setup force level low",UI_FORCE_LEVEL_LOW);
    addNewParmToMap("%",    "%",    "100",  "0",    "100",  "100",  "0",    "100",  "100",  "0",    "100",  "setup force level high",UI_FORCE_LEVEL_HIGH);

    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "modified flag",UI_IS_MODIFIED);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "modified flag",UI_ISNEW_RECIPE);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "modified flag",UI_ISVALID);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "modified flag",UI_ISLOCK);
    addNewParmToMap("",     "",     "0",    "0",    "1",    "0",    "0",    "1",    "0",    "0",    "1",    "modified flag",UI_ISACTIVE);
}

/**
 * @brief WeldRecipeParameter::initializeSuspectRejectLimits : To set all suspect and reject limits as a defaults values
 */
void WeldRecipeParameter::initializeSuspectRejectLimits()
{
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsEnabled = false;
    m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsEnabled = false;
    stSuspectRejectParamInfo stSuspectReject;
    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[GLOBAL_SUPSPECT];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[GLOBAL_REJECT];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_MODE_TIME];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_MODE_ENERGY];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_MODE_PEAK_POWER];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_MODE_ABS_DIS];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_MODE_COL_DIS];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_PARAM_TRI_DIS];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_PARAM_END_WELD_FORCE];

    stSuspectReject.RejectMin = "";
    stSuspectReject.SuspectMin = "";
    stSuspectReject.SuspectMax = "";
    stSuspectReject.RejectMax = "";
    stSuspectReject.rejectStatus = false;
    stSuspectReject.suspectStatus = false;
    m_MapSuspectRejectParams[WELD_PARAM_FREQ];
}

/**
 * @brief WeldRecipeParameter::getStructureByName and Map the values : Get the Name and return the Values
 * @param paramEditText
 * @return
 */
stRecipeParmInfo WeldRecipeParameter::getStructureByName(QString paramEditText)
{
    stRecipeParmInfo stParam;
    if(!m_MapRecipeParams.contains(paramEditText.toLower()))
        return stParam;
    stParam = m_MapRecipeParams[paramEditText.toLower()];
    return stParam;
}

/**
 * @brief WeldRecipeParameter::getStructureValueByName : Get the Name and Unit and return values
 * @param paramEditText
 * @return
 */
QString WeldRecipeParameter::getStructureValueByName(QString paramEditText)
{
    if(!m_MapRecipeParams.contains(paramEditText.toLower()))
        return ZERO_STR;
    int dwParamIndex = m_MapRecipeParams[paramEditText.toLower()].ParamIndexInStructure;

    if(dwParamIndex == SC_MODE_VALUE_TIME ||
            dwParamIndex == SC_MODE_VALUE_ENERGY ||
            dwParamIndex == SC_MODE_VALUE_POWER ||
            dwParamIndex == SC_MODE_VALUE_AD ||
            dwParamIndex == SC_MODE_VALUE_CD ||
            dwParamIndex == SC_MODE_VALUE_ST ||
            dwParamIndex == SC_MODE_VALUE_GD ||
            dwParamIndex == SC_MODE_VALUE_DY ||
            dwParamIndex == SC_MODE_VALUE_DENSITY)
    {
        dwParamIndex = SC_MODE_VALUE;
    }
    QString qstrRetValue=getStructureValueByIndex(dwParamIndex);
    QString qstrunit = m_MapRecipeParams[paramEditText.toLower()].ParamUnit[m_UnitSelected];

    /*if((paramEditText.toLower() == WELD_SETUPMODE_HIGH) ||
            (paramEditText.toLower() == WELD_SETUPMODE_LOW))
      Above statement needs to be commented, as using of MACRO causes conflict while non enlish language
      used.This occurs due to comparison of english string with that of respective translated string,
      which is not going to be true in any case, causing units handling issue in non-english system operations.*/

    if((paramEditText.toLower() == "setup weld mode high") ||
            (paramEditText.toLower() == "setup weld mode low"))
    {
        int mode;
        mode = getStructureValueByIndex(SC_WELD_MODE).toInt();
        ModeInfo stModeInfo = getModeInfoByIndex(mode,"");
        qstrunit = stModeInfo.ModeUnit;
    }

    if(qstrunit == CUST_NAME_UINTMilliMeter)
    {
        double dval = convertMicrometreToMillimetre(qstrRetValue.toDouble());
        /*For Collpse Distance and Absolute Distance precision when beyond limit value restored to default*/
        if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            qstrRetValue.setNum(dval,'f',2);
        }
        else
        {
            qstrRetValue.setNum(dval,'f',3);
        }
    }
    else if(qstrunit == CUST_NAME_UINTJOULES)
    {
        double dval = Utility::convertMilliJoulesToJoules(qstrRetValue.toDouble());
        qstrRetValue.setNum(dval,'f',1);
    }
    else if(qstrunit == CUST_NAME_UINTSEC)
    {
        double dval = convertMsecToSec(qstrRetValue.toDouble());
        qstrRetValue.setNum(dval,'f',3);
    }
    else if(qstrunit == CUST_NAME_UINTLBS)
    {
        double dval = convertNtolbs(qstrRetValue.toDouble());
        qstrRetValue = QString::number(round(dval));//.setNum(dval,'f',2);
    }
    else if(qstrunit == CUST_NAME_UINTINCHES)
    {
        double dval = convertmicrometretoinches(qstrRetValue.toDouble());
        qstrRetValue.setNum(dval,'f',4);
    }
    else if(qstrunit == CUST_NAME_UINIINSEC || qstrunit == CUST_NAME_UINIINSECSQU)
    {
        double dval = convertmillimetretoinches(qstrRetValue.toDouble());
        qstrRetValue.setNum(dval,'f',4);
    }
    int dwStepAt ;
    double dval;
    if((dwParamIndex >= SC_FORCE_STEP_VALUE1) && (dwParamIndex <= SC_FORCE_STEP_VALUE10))
    {
        dwStepAt = getStructureValueByIndex(SC_FORCE_STEP_AT).toInt();

        /*To handle changed in Enum sequence as Step@PK Power option is removed in ASX*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
           if(dwStepAt > STEP_AT_ENERGY)
           {
             dwStepAt = dwStepAt + 1;
           }
        }

        if(dwStepAt == STEP_AT_TIME)
        {
            dval = convertMsecToSec(qstrRetValue.toDouble());
            qstrRetValue = QString::number(dval);
            qstrRetValue.setNum(dval,'f',3);
        }
        else if(dwStepAt == STEP_AT_ENERGY)
        {
            dval = Utility::convertMilliJoulesToJoules(qstrRetValue.toDouble());
            qstrRetValue = QString::number(dval);
        }
        else if((dwStepAt == STEP_AT_ABSOLUTE_DISTANCE) || (dwStepAt == STEP_AT_COLLAPSE_DISTANCE))
        {
            if(m_UnitSelected == UNIT_SI)
            {
                dval = convertMicrometreToMillimetre(qstrRetValue.toDouble());
                qstrRetValue = QString::number(dval);
                /*To handle precision functionality during updated value reading for Weld Force stepping
                  in Step@Absolute and Step@Collapse dropdown*/
                if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
                {
                    qstrRetValue.setNum(dval,'f',2);
                }

                else
                {
                    qstrRetValue.setNum(dval,'f',3);
                }
            }
            else if(m_UnitSelected == UNIT_SAE)
            {
                dval = convertmicrometretoinches(qstrRetValue.toDouble());
                qstrRetValue = QString::number(dval);
                qstrRetValue.setNum(dval,'f',4);
            }
        }
    }
    if((dwParamIndex >= SC_AMPLITUDE_STEP_VALUE1) && (dwParamIndex <= SC_AMPLITUDE_STEP_VALUE10))
    {
        dwStepAt = getStructureValueByIndex(SC_AMPLITUDE_STEP_AT).toInt();

        /*To handle changed in Enum sequence as Step@PK Power option is removed in ASX*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
           if(dwStepAt > STEP_AT_ENERGY)
           {
             dwStepAt = dwStepAt + 1;
           }
        }

        if(dwStepAt == STEP_AT_TIME)
        {
            dval = convertMsecToSec(qstrRetValue.toDouble());
            qstrRetValue = QString::number(dval);
            qstrRetValue.setNum(dval,'f',3);
        }
        else if(dwStepAt == STEP_AT_ENERGY)
        {
            dval = Utility::convertMilliJoulesToJoules(qstrRetValue.toDouble());
            qstrRetValue = QString::number(dval);
        }
        else if((dwStepAt == STEP_AT_ABSOLUTE_DISTANCE) || (dwStepAt == STEP_AT_COLLAPSE_DISTANCE))
        {
            if(m_UnitSelected == UNIT_SI)
            {
                dval = convertMicrometreToMillimetre(qstrRetValue.toDouble());
                qstrRetValue = QString::number(dval);
                /*To handle precision functionality during updated value reading for Weld Amplitude stepping
                  in Step@Absolute and Step@Collapse dropdown*/
                if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
                {
                    qstrRetValue.setNum(dval,'f',2);
                }

                else
                {
                    qstrRetValue.setNum(dval,'f',3);
                }
            }
            else if(m_UnitSelected == UNIT_SAE)
            {
                dval = convertmicrometretoinches(qstrRetValue.toDouble());
                qstrRetValue = QString::number(dval);
                qstrRetValue.setNum(dval,'f',4);
            }
        }
    }
    return qstrRetValue;
}

/**
 * @brief WeldRecipeParameter::getStructureValueByIndex : Get the index of assign Name
 * @param a_Index
 * @return
 */
QString WeldRecipeParameter::getStructureValueByIndex(int a_Index)
{
    QString qstrRetValue="";
    switch(a_Index)
    {
    case SC_RECIPE_NUMBER:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeNumber);
        break;
    case SC_RECIPE_VER_NUMBER:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber);
        break;
    case SC_WELD_MODE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.WeldMode);
        break;
    case SC_MODE_VALUE:
        /*In order to make unit place of Absolute Distance to 0,for ASX precision handling while reading*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            INT32 localValue;
            localValue = ((m_weldRecipe.m_weldRecipe_SC.ModeValue) % 10);
            localValue = ((m_weldRecipe.m_weldRecipe_SC.ModeValue) - localValue);
            qstrRetValue = QString::number(localValue);
        }

        else
        {
            qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ModeValue);
        }
        break;
    case SC_HOLD_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.HoldTime);
        break;
    case SC_TRIGGER_FORCE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.TriggerForce);
        break;
    case SC_TRIGGER_DISTANCE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.TriggerDistance);
        break;
    case SC_MAX_WELD_TIMEOUT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.MaxWeldTimeout);
        break;
    case SC_NUM_AMPLITUDE_STEPS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.NumAmplitudeSteps);
        break;
    case SC_AMPLITUDE_STEP_AT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepAt);
        break;
    case SC_NUM_FORCE_STEPS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.NumForceSteps);
        break;
    case SC_FORCE_STEP_AT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepAt);
        break;
    case SC_FORCE_STEP_VALUE1:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[0]);
        break;
    case SC_FORCE_STEP_VALUE2:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[1]);
        break;
    case SC_FORCE_STEP_VALUE3:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[2]);
        break;
    case SC_FORCE_STEP_VALUE4:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[3]);
        break;
    case SC_FORCE_STEP_VALUE5:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[4]);
        break;
    case SC_FORCE_STEP_VALUE6:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[5]);
        break;
    case SC_FORCE_STEP_VALUE7:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[6]);
        break;
    case SC_FORCE_STEP_VALUE8:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[7]);
        break;
    case SC_FORCE_STEP_VALUE9:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[8]);
        break;
    case SC_FORCE_STEP_VALUE10:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[9]);
        break;
    case SC_AMPLITUDE_STEP_VALUE1:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[0]);
        break;
    case SC_AMPLITUDE_STEP_VALUE2:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[1]);
        break;
    case SC_AMPLITUDE_STEP_VALUE3:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[2]);
        break;
    case SC_AMPLITUDE_STEP_VALUE4:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[3]);
        break;
    case SC_AMPLITUDE_STEP_VALUE5:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[4]);
        break;
    case SC_AMPLITUDE_STEP_VALUE6:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[5]);
        break;
    case SC_AMPLITUDE_STEP_VALUE7:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[6]);
        break;
    case SC_AMPLITUDE_STEP_VALUE8:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[7]);
        break;
    case SC_AMPLITUDE_STEP_VALUE9:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[8]);
        break;
    case SC_AMPLITUDE_STEP_VALUE10:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[9]);
        break;
    case SC_AMPLITUDE_STEP1:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[0]);
        break;
    case SC_AMPLITUDE_STEP2:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[1]);
        break;
    case SC_AMPLITUDE_STEP3:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[2]);
        break;
    case SC_AMPLITUDE_STEP4:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[3]);
        break;
    case SC_AMPLITUDE_STEP5:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[4]);
        break;
    case SC_AMPLITUDE_STEP6:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[5]);
        break;
    case SC_AMPLITUDE_STEP7:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[6]);
        break;
    case SC_AMPLITUDE_STEP8:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[7]);
        break;
    case SC_AMPLITUDE_STEP9:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[8]);
        break;
    case SC_AMPLITUDE_STEP10:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[9]);
        break;
    case SC_COOLING_VALVE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.bIsCoolingValeEnabled);
        break;
    case SC_GLOBAL_SUSPECT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.IsGlobalSuspectEnabled);
        break;
    case SC_GLOBAL_REJECT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.IsGlobalRejectEnabled);
        break;
    case SC_CONTROL_LIMIT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.bIsControlLimitEnabled);
        break;
    case SC_CL_PEAKPOWER_CUTOFF:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.IsEnabled);
        break;
    case SC_CL_PEAKPOWER_CUTOFF_VAL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.Value);
        break;
    case SC_CL_ABSOLUTE_CUTOFF:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.IsEnabled);
        break;
    case SC_CL_ABSOLUTE_CUTOFF_VAL:
        /*In order to make unit place of Absolute Distance Cutoff to 0,for ASX precision handling while reading*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            INT32 localValue;
            localValue = ((m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value) % 10);
            localValue = ((m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value) - localValue);
            qstrRetValue = QString::number(localValue);
        }

        else
        {
            qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value);
        }
        break;
    case SC_CL_COLLAPSE_CUTOFF:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.CollapseCutoff.IsEnabled);
        break;
    case SC_CL_COLLAPSE_CUTOFF_VAL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.CollapseCutoff.Value);
        break;
    case SC_CL_TIME_CUTOFF:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.TimeCutoff.IsEnabled);
        break;
    case SC_CL_TIME_CUTOFF_VAL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.TimeCutoff.Value);
        break;
    case SC_CL_FREQ_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyLow.IsEnabled);
        break;
    case SC_CL_FREQ_LOW_VAL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyLow.Value);
        break;
    case SC_CL_FREQ_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyHigh.IsEnabled);
        break;
    case SC_CL_FREQ_HIGH_VAL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyHigh.Value);
        break;
    case SC_CL_ENERGY_CUTOFF:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.EnergyCutoff.IsEnabled);
        break;
    case SC_CL_ENERGY_CUTOFF_VAL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.EnergyCutoff.Value);
        break;
    case SC_CL_GROUND_DETECT_CUTOFF:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.GroundDetectCutoff);
        break;
    case SC_PRETRIGGER:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsEnabled);
        break;
    case SC_AUTO_PRETRIGGER:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsAutoEnabled);
        break;
    case SC_DISTANCE_PRETRIGGER:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsDistanceEnabled);
        break;
    case SC_PRETRIGGER_AMPLITUDE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.TriggerAmplitude);
        break;
    case SC_PRETRIGGER_DISTANCE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.PreTriggerDistance);
        break;
    case SC_AFTER_BURST:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.bIsEnabled);
        break;
    case SC_AB_AMPLITUDE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstAmplitude);
        break;
    case SC_AB_DELAY:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstDelay);
        break;
    case SC_AB_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstTime);
        break;
    case SC_ENERGY_BRAKE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.EnergyBrake.bIsEnabled);
        break;
    case SC_ENERGY_BRAKE_AMPLITUDE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.EnergyBrake.EnergyBrakeAmplitude);
        break;
    case SC_ENERGY_BRAKE_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.EnergyBrake.EnergyBrakeTime);
        break;
    case SC_SUSPECT_TIME_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsEnabled);
        break;
    case SC_SUSPECT_TIME_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsLowEnabled);
        break;
    case SC_SUSPECT_TIME_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsHighEnabled);
        break;
    case SC_SUSPECT_TIME_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.Low);
        break;
    case SC_SUSPECT_TIME_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.High);
        break;
    case SC_SUSPECT_ENERGY_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsEnabled);
        break;
    case SC_SUSPECT_ENERGY_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsLowEnabled);
        break;
    case SC_SUSPECT_ENERGY_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsHighEnabled);
        break;
    case SC_SUSPECT_ENERGY_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.Low);
        break;
    case SC_SUSPECT_ENERGY_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.High);
        break;
    case SC_SUSPECT_POWER_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsEnabled);
        break;
    case SC_SUSPECT_POWER_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsLowEnabled);
        break;
    case SC_SUSPECT_POWER_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsHighEnabled);
        break;
    case SC_SUSPECT_POWER_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.Low);
        break;
    case SC_SUSPECT_POWER_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.High);
        break;
    case SC_SUSPECT_CD_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsEnabled);
        break;
    case SC_SUSPECT_CD_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsLowEnabled);
        break;
    case SC_SUSPECT_CD_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsHighEnabled);
        break;
    case SC_SUSPECT_CD_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.Low);
        break;
    case SC_SUSPECT_CD_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.High);
        break;
    case SC_SUSPECT_AD_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsEnabled);
        break;
    case SC_SUSPECT_AD_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsLowEnabled);
        break;
    case SC_SUSPECT_AD_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsHighEnabled);
        break;
    case SC_SUSPECT_AD_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.Low);
        break;
    case SC_SUSPECT_AD_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High);
        break;
    case SC_SUSPECT_TD_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsEnabled);
        break;
    case SC_SUSPECT_TD_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsLowEnabled);
        break;
    case SC_SUSPECT_TD_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsHighEnabled);
        break;
    case SC_SUSPECT_TD_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.Low);
        break;
    case SC_SUSPECT_TD_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.High);
        break;
    case SC_SUSPECT_END_FORCE_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsEnabled);
        break;
    case SC_SUSPECT_END_FORCE_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsLowEnabled);
        break;
    case SC_SUSPECT_END_FORCE_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsHighEnabled);
        break;
    case SC_SUSPECT_END_FORCE_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.Low);
        break;
    case SC_SUSPECT_END_FORCE_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.High);
        break;
    case SC_SUSPECT_FREQ_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsEnabled);
        break;
    case SC_SUSPECT_FREQ_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsLowEnabled);
        break;
    case SC_SUSPECT_FREQ_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsHighEnabled);
        break;
    case SC_SUSPECT_FREQ_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.Low);
        break;
    case SC_SUSPECT_FREQ_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.High);
        break;
    case SC_REJECT_TIME_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsEnabled);
        break;
    case SC_REJECT_TIME_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsLowEnabled);
        break;
    case SC_REJECT_TIME_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsHighEnabled);
        break;
    case SC_REJECT_TIME_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.Low);
        break;
    case SC_REJECT_TIME_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.High);
        break;
    case SC_REJECT_ENERGY_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsEnabled);
        break;
    case SC_REJECT_ENERGY_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsLowEnabled);
        break;
    case SC_REJECT_ENERGY_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsHighEnabled);
        break;
    case SC_REJECT_ENERGY_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.Low);
        break;
    case SC_REJECT_ENERGY_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.High);
        break;
    case SC_REJECT_POWER_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsEnabled);
        break;
    case SC_REJECT_POWER_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsLowEnabled);
        break;
    case SC_REJECT_POWER_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsHighEnabled);
        break;
    case SC_REJECT_POWER_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.Low);
        break;
    case SC_REJECT_POWER_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.High);
        break;
    case SC_REJECT_CD_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsEnabled);
        break;
    case SC_REJECT_CD_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsLowEnabled);
        break;
    case SC_REJECT_CD_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsHighEnabled);
        break;
    case SC_REJECT_CD_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.Low);
        break;
    case SC_REJECT_CD_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.High);
        break;
    case SC_REJECT_AD_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsEnabled);
        break;
    case SC_REJECT_AD_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsLowEnabled);
        break;
    case SC_REJECT_AD_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsHighEnabled);
        break;
    case SC_REJECT_AD_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.Low);
        break;
    case SC_REJECT_AD_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High);
        break;
    case SC_REJECT_TD_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsEnabled);
        break;
    case SC_REJECT_TD_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsLowEnabled);
        break;
    case SC_REJECT_TD_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsHighEnabled);
        break;
    case SC_REJECT_TD_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.Low);
        break;
    case SC_REJECT_TD_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.High);
        break;
    case SC_REJECT_END_FORCE_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsEnabled);
        break;
    case SC_REJECT_END_FORCE_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsLowEnabled);
        break;
    case SC_REJECT_END_FORCE_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsHighEnabled);
        break;
    case SC_REJECT_END_FORCE_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.Low);
        break;
    case SC_REJECT_END_FORCE_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.High);
        break;
    case SC_REJECT_FREQ_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsEnabled);
        break;
    case SC_REJECT_FREQ_LOW_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsLowEnabled);
        break;
    case SC_REJECT_FREQ_HIGH_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsHighEnabled);
        break;
    case SC_REJECT_FREQ_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.Low);
        break;
    case SC_REJECT_FREQ_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.High);
        break;
    case SC_USER:
        qstrRetValue = m_weldRecipe.m_weldRecipe_SC.User;
        break;
    case SC_STACK_SERIAL_NO:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.StackSerialNo);
        break;
    case SC_REACTIVITY:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.Reactivity);
        break;
    case SC_FORCE_LEVEL_ENABLED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.IsForceLevelEnabled);
        break;
    case SC_FORCE_LEVEL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.ForceLevel);
        break;
    case SC_FORCE_LEVEL_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.ForceLevelTime);
        break;
    case UI_RECIPE_NAME:
        qstrRetValue = m_weldRecipe.m_weldRecipe_UI.RecipeName;
        break;
    case SC_TIMED_SEEK_PERIOD:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.TimedSeekPeriod);
        break;
    case SC_TIMED_SEEK:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.bIsTimedSeekEnabled);
        break;
    case SC_PRE_WELD_SEEK:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.bIsPreWeldSeekEnabled);
        break;
    case SC_POST_WELD_SEEK:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.bIsPostWeldSeekEnabled);
        break;
    case SC_SCRUB_AMPLITUDE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_SC.ScrubAmplitude);
        break;
        //PC
    case PC_AMPLITUDE_LOOPC1:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC1);
        break;
    case PC_AMPLITUDE_LOOPC2:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC2);
        break;
    case PC_PHASE_LOOP:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoop);
        break;
    case PC_PHASE_LOOPCF:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoopCF);
        break;
    case PC_FREQUENCY_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyLow);
        break;
    case PC_FREQUENCY_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyHigh);
        break;
    case PC_PHASE_LIMIT_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimitTime);
        break;
    case PC_PHASE_LIMIT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimit);
        break;
    case PC_CONTROL_MODE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.ControlMode);
        break;
    case PC_BLIND_TIME_SEEK:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.BlindtimeSeek);
        break;
    case PC_BLIND_TIME_WELD:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Blindtimeweld);
        break;
    case PC_DDS_SWITCH_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.DDSSwitchtime);
        break;
    case PC_PARAMETER7:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter7);
        break;
    case PC_PARAMETER8:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter8);
        break;
    case PC_PARAMETER9:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter9);
        break;
    case PC_PARAMETER10:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter10);
        break;
    case PC_PARAMETER11:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter11);
        break;
    case PC_PARAMETER12:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter12);
        break;
    case PC_PARAMETER13:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter13);
        break;
    case PC_PARAMETER14:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter14);
        break;
    case PC_PARAMETER15:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter15);
        break;
    case PC_F_LIMIT_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FLimitTime);
        break;
    case PC_AMP_PROPORTIONAL_GAIN:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpProportionalGain);
        break;
    case PC_AMP_INTEGRAL_GAIN:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpIntegralGain);
        break;
    case PC_PHASE_INTEGRAL_GAIN:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseIntegralGain);
        break;
    case PC_PHASE_PROPORTIONAL_GAIN:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseProportionalGain);
        break;
    case PC_FREQUENCY_WINDOW_SIZE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyWindowSize);
        break;
    case PC_PF_OF_PHASE_CONTROL_LOOP:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PFofPhasecontrolloop);
        break;
    case PC_PI_FREQUENCY_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyLow);
        break;
    case PC_PI_FREQUENCY_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyHigh);
        break;
    case PC_PI_PHASE_LIMIT_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimitTime);
        break;
    case PC_PI_PHASE_LIMIT:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimit);
        break;
    case PC_WELD_RAMP_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime);
        break;
    case PC_START_FREQUENCY:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.StartFrequency);
        break;

        //AC
    case AC_WELD_FORCE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce);
        break;
    case AC_FORCE_RAMP_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceRampTime);
        break;
    case AC_HOLD_MODE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldMode);
        break;
    case AC_TOTAL_COLLAPSE_TARGET:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.TotalCollapseTarget);
        break;
    case AC_HOLD_FORCE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForce);
        break;
    case AC_HOLD_FORCE_RAMP_TIME:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForceRampTime);
        break;
    case AC_EXPECTED_PART_CONTACT_POSITION:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactPosition);
        break;
    case AC_READY_POSITION:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPosition);
        break;
    case AC_DOWN_ACCELERATION:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownAcceleration);
        break;
    case AC_DOWN_MAX_VELOCITY:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownMaxVelocity);
        break;
    case AC_DOWN_DECELERATION:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownDeceleration);
        break;
    case AC_RETURN_ACCELERATION:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnAcceleration);
        break;
    case AC_RETURN_MAX_VELOCITY:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnMaxVelocity);
        break;
    case AC_RETURN_DECELERATION:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnDeceleration);
        break;
    case AC_PART_CONTANCT_OFFSET:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactOffset);
        break;
    case AC_PART_CONTANCT_WINDOW_MINUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowMinus);
        break;
    case AC_PART_CONTANCT_WINDOW_PLUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowPlus);
        break;
    case AC_FORCE_STEP_RAMP_VALUE1:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[0]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE2:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[1]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE3:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[2]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE4:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[3]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE5:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[4]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE6:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[5]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE7:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[6]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE8:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[7]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE9:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[8]);
        break;
    case AC_FORCE_STEP_RAMP_VALUE10:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[9]);
        break;
    case AC_FORCE_STEP1:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce);
        break;
    case AC_FORCE_STEP2:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[1]);
        break;
    case AC_FORCE_STEP3:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[2]);
        break;
    case AC_FORCE_STEP4:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[3]);
        break;
    case AC_FORCE_STEP5:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[4]);
        break;
    case AC_FORCE_STEP6:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[5]);
        break;
    case AC_FORCE_STEP7:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[6]);
        break;
    case AC_FORCE_STEP8:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[7]);
        break;
    case AC_FORCE_STEP9:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[8]);
        break;
    case AC_FORCE_STEP10:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[9]);
        break;
    case AC_READY_POSITION_TOGGLE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPositionToggle);
        break;
    case AC_WELD_FORCE_CONTROL:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForceControl);
        break;
        //UI
    case UI_ISACTIVE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.isActive);
        break;
    case UI_ISLOCK:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.isLock);
        break;
    case UI_COMPANYNAME:
        qstrRetValue = m_weldRecipe.m_weldRecipe_UI.CompanyName;
        break;
    case UI_ISVALID:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.isValid);
        break;
    case UI_ISNEW_RECIPE:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.isNewRecipe);
        break;
    case UI_IS_MODIFIED:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.isModified);
        break;
    case UI_RECIPE_VERSION_NUMBER:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.dwRecipeVerNumber);
        break;
    case UI_GLOBAL_SETUP:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.isGlobalSetup);
        break;
    case UI_SETUP_WELD_MODE_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeStatus);
        break;
    case UI_SETUP_WELD_MODE_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeLow);
        break;
    case UI_SETUP_WELD_MODE_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeHigh);
        break;
    case UI_WELD_AMP_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpStatus);
        break;
    case UI_WELD_AMP_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpLow);
        break;
    case UI_WELD_AMP_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpHigh);
        break;
    case UI_WELD_FORCE_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceStatus);
        break;
    case UI_WELD_FORCE_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceLow);
        break;
    case UI_WELD_FORCE_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceHigh);
        break;
    case UI_TRIGGER_FORCE_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceStatus);
        break;
    case UI_TRIGGER_FORCE_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceLow);
        break;
    case UI_TRIGGER_FORCE_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceHigh);
        break;
    case UI_HOLD_FORCE_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceStatus);
        break;
    case UI_HOLD_FORCE_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceLow);
        break;
    case UI_HOLD_FORCE_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceHigh);
        break;
    case UI_HOLD_TIME_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeStatus);
        break;
    case UI_HOLD_TIME_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeLow);
        break;
    case UI_HOLD_TIME_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeHigh);
        break;
    case UI_SCRUB_AMP_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpStatus);
        break;
    case UI_SCRUB_AMP_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpLow);
        break;
    case UI_SCRUB_AMP_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpHigh);
        break;
    case UI_SCRUB_TIME_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeStatus);
        break;
    case UI_SCRUB_TIME_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeLow);
        break;
    case UI_SCRUB_TIME_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeHigh);
        break;
    case UI_REACTIVITY_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityStatus);
        break;
    case UI_REACTIVITY_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityLow);
        break;
    case UI_REACTIVITY_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityHigh);
        break;
    case UI_FORCE_LEVEL_STATUS:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelStatus);
        break;
    case UI_FORCE_LEVEL_LOW:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelLow);
        break;
    case UI_FORCE_LEVEL_HIGH:
        qstrRetValue = QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelHigh);
        break;
    default :
        qstrRetValue = "";
        break;
    }

    return qstrRetValue;
}

/**
 * @brief WeldRecipeParameter::getDefaultFrequencySelection : Defalut Frequency Selection and Names to defined
 */
void WeldRecipeParameter::getDefaultFrequencySelection(int tmpRecipeNum)
{
    //SC
    if(tmpRecipeNum == 0)
    {
        setStructureVariableByName("recipenumber",getDefaultValue("recipenumber"));
        setStructureVariableByName("versionnumber",getDefaultValue("versionnumber"));
        setStructureVariableByName("weldmode",getFirstRuntimeIndex());
        setStructureVariableByName("modevalue",getDefaultValue("modevalue"));
    }
    setStructureVariableByName("reactivity",getDefaultValue("reactivity"));
    setStructureVariableByName("force level enable",getDefaultValue("force level enable"));
    setStructureVariableByName("force level",getDefaultValue("force level"));
    setStructureVariableByName("force level time",getDefaultValue("force level time"));
    setStructureVariableByName("scrub amplitude",getDefaultValue("scrub amplitude"));
    setStructureVariableByName("hold time",getDefaultValue("hold time"));
    setStructureVariableByName("trigger force",getDefaultValue("trigger force"));
    setStructureVariableByName("trigger distance",getDefaultValue("trigger distance"));
    setStructureVariableByName("max timeout",getDefaultValue("max timeout"));
    setStructureVariableByName("numamplitudesteps",getDefaultValue("numamplitudesteps"));
    setStructureVariableByName("amplitudestepat",getDefaultValue("amplitudestepat"));
    setStructureVariableByName("numforcesteps",getDefaultValue("numforcesteps"));
    setStructureVariableByName("forcestepat",getDefaultValue("forcestepat"));
    setStructureVariableByName("force step value 1",getDefaultValue("force step value 1"));
    setStructureVariableByName("force step value 2",getDefaultValue("force step value 2"));
    setStructureVariableByName("force step value 3",getDefaultValue("force step value 3"));
    setStructureVariableByName("force step value 4",getDefaultValue("force step value 4"));
    setStructureVariableByName("force step value 5",getDefaultValue("force step value 5"));
    setStructureVariableByName("force step value 6",getDefaultValue("force step value 6"));
    setStructureVariableByName("force step value 7",getDefaultValue("force step value 7"));
    setStructureVariableByName("force step value 8",getDefaultValue("force step value 8"));
    setStructureVariableByName("force step value 9",getDefaultValue("force step value 9"));
    setStructureVariableByName("force step value 10",getDefaultValue("force step value 10"));
    setStructureVariableByName("amplitude step value 1",getDefaultValue("amplitude step value 1"));
    setStructureVariableByName("amplitude step value 2",getDefaultValue("amplitude step value 2"));
    setStructureVariableByName("amplitude step value 3",getDefaultValue("amplitude step value 3"));
    setStructureVariableByName("amplitude step value 4",getDefaultValue("amplitude step value 4"));
    setStructureVariableByName("amplitude step value 5",getDefaultValue("amplitude step value 5"));
    setStructureVariableByName("amplitude step value 6",getDefaultValue("amplitude step value 6"));
    setStructureVariableByName("amplitude step value 7",getDefaultValue("amplitude step value 7"));
    setStructureVariableByName("amplitude step value 8",getDefaultValue("amplitude step value 8"));
    setStructureVariableByName("amplitude step value 9",getDefaultValue("amplitude step value 9"));
    setStructureVariableByName("amplitude step value 10",getDefaultValue("amplitude step value 10"));
    setStructureVariableByName("weld amplitude",getDefaultValue("weld amplitude"));
    setStructureVariableByName("amplitude 2",getDefaultValue("amplitude 2"));
    setStructureVariableByName("amplitude 3",getDefaultValue("amplitude 3"));
    setStructureVariableByName("amplitude 4",getDefaultValue("amplitude 4"));
    setStructureVariableByName("amplitude 5",getDefaultValue("amplitude 5"));
    setStructureVariableByName("amplitude 6",getDefaultValue("amplitude 6"));
    setStructureVariableByName("amplitude 7",getDefaultValue("amplitude 7"));
    setStructureVariableByName("amplitude 8",getDefaultValue("amplitude 8"));
    setStructureVariableByName("amplitude 9",getDefaultValue("amplitude 9"));
    setStructureVariableByName("amplitude 10",getDefaultValue("amplitude 10"));
    setStructureVariableByName("extra cooling",getDefaultValue("extra cooling"));
    setStructureVariableByName("global suspect",getDefaultValue("global suspect"));
    setStructureVariableByName("global reject",getDefaultValue("global reject"));
    setStructureVariableByName("control",getDefaultValue("control"));
    setStructureVariableByName("peak power cutoff state",getDefaultValue("peak power cutoff state"));
    setStructureVariableByName("peak power cutoff",getDefaultValue("peak power cutoff"));
    setStructureVariableByName("absolute distance cutoff state",getDefaultValue("absolute distance cutoff state"));
    setStructureVariableByName("absolute distance cutoff",getDefaultValue("absolute distance cutoff"));
    setStructureVariableByName("collapse distance cutoff state",getDefaultValue("collapse distance cutoff state"));
    setStructureVariableByName("collapse distance cutoff",getDefaultValue("collapse distance cutoff"));
    setStructureVariableByName("time cutoff state",getDefaultValue("time cutoff state"));
    setStructureVariableByName("time cutoff",getDefaultValue("time cutoff"));
    setStructureVariableByName("frequency low cutoff state",getDefaultValue("frequency low cutoff state"));
    setStructureVariableByName("frequency low cutoff",getDefaultValue("frequency low cutoff"));
    setStructureVariableByName("frequency high cutoff state",getDefaultValue("frequency high cutoff state"));
    setStructureVariableByName("frequency high cutoff",getDefaultValue("frequency high cutoff"));
    setStructureVariableByName("energy cutoff state",getDefaultValue("energy cutoff state"));
    setStructureVariableByName("energy cutoff",getDefaultValue("energy cutoff"));
    setStructureVariableByName("ground detect cutoff",getDefaultValue("ground detect cutoff"));
    setStructureVariableByName("pretrigger",getDefaultValue("pretrigger"));
    setStructureVariableByName("distanceauto",getDefaultValue("distanceauto"));
    setStructureVariableByName("pretrigger distance auto",getDefaultValue("pretrigger distance auto"));
    setStructureVariableByName("pretrigger amplitude",getDefaultValue("pretrigger amplitude"));
    setStructureVariableByName("pretrigger distance",getDefaultValue("pretrigger distance"));
    setStructureVariableByName("afterburst",getDefaultValue("afterburst"));
    setStructureVariableByName("afterburst amplitude",getDefaultValue("afterburst amplitude"));
    setStructureVariableByName("afterburst delay",getDefaultValue("afterburst delay"));
    setStructureVariableByName("afterburst time",getDefaultValue("afterburst time"));
    setStructureVariableByName("energy brake",getDefaultValue("energy brake"));
    setStructureVariableByName("energy brake amplitude",getDefaultValue("energy brake amplitude"));
    setStructureVariableByName("energy brake time",getDefaultValue("energy brake time"));
    setStructureVariableByName("suspect time",getDefaultValue("suspect time"));
    setStructureVariableByName("suspect time low",getDefaultValue("suspect time low"));
    setStructureVariableByName("suspect time high",getDefaultValue("suspect time high"));
    setStructureVariableByName("suspect time low value",getDefaultValue("suspect time low value"));
    setStructureVariableByName("suspect time high value",getDefaultValue("suspect time high value"));
    setStructureVariableByName("suspect energy",getDefaultValue("suspect energy"));
    setStructureVariableByName("suspect energy low",getDefaultValue("suspect energy low"));
    setStructureVariableByName("suspect energy high",getDefaultValue("suspect energy high"));
    setStructureVariableByName("suspect energy low value",getDefaultValue("suspect energy low value"));
    setStructureVariableByName("suspect energy high value",getDefaultValue("suspect energy high value"));
    setStructureVariableByName("suspect peak power",getDefaultValue("suspect peak power"));
    setStructureVariableByName("suspect peak power low",getDefaultValue("suspect peak power low"));
    setStructureVariableByName("suspect peak power high",getDefaultValue("suspect peak power high"));
    setStructureVariableByName("suspect peak power low value",getDefaultValue("suspect peak power low value"));
    setStructureVariableByName("suspect peak power high value",getDefaultValue("suspect peak power high value"));
    setStructureVariableByName("suspect collapse distance",getDefaultValue("suspect collapse distance"));
    setStructureVariableByName("suspect collapse distance low",getDefaultValue("suspect collapse distance low"));
    setStructureVariableByName("suspect collapse distance high",getDefaultValue("suspect collapse distance high"));
    setStructureVariableByName("suspect collapse distance low value",getDefaultValue("suspect collapse distance low value"));
    setStructureVariableByName("suspect collapse distance high value",getDefaultValue("suspect collapse distance high value"));
    setStructureVariableByName("suspect absolute distance",getDefaultValue("suspect absolute distance"));
    setStructureVariableByName("suspect absolute distance low",getDefaultValue("suspect absolute distance low"));
    setStructureVariableByName("suspect absolute distance high",getDefaultValue("suspect absolute distance high"));
    setStructureVariableByName("suspect absolute distance low value",getDefaultValue("suspect absolute distance low value"));
    setStructureVariableByName("suspect absolute distance high value",getDefaultValue("suspect absolute distance high value"));
    setStructureVariableByName("suspect trigger distance",getDefaultValue("suspect trigger distance"));
    setStructureVariableByName("suspect trigger distance low",getDefaultValue("suspect trigger distance low"));
    setStructureVariableByName("suspect trigger distance high",getDefaultValue("suspect trigger distance high"));
    setStructureVariableByName("suspect trigger distance low value",getDefaultValue("suspect trigger distance low value"));
    setStructureVariableByName("suspect trigger distance high value",getDefaultValue("suspect trigger distance high value"));
    setStructureVariableByName("suspect end weld force",getDefaultValue("suspect end weld force"));
    setStructureVariableByName("suspect end weld force low",getDefaultValue("suspect end weld force low"));
    setStructureVariableByName("suspect end weld force high",getDefaultValue("suspect end weld force high"));
    setStructureVariableByName("suspect end weld force low value",getDefaultValue("suspect end weld force low value"));
    setStructureVariableByName("suspect end weld force high value",getDefaultValue("suspect end weld force high value"));
    setStructureVariableByName("suspect frequency",getDefaultValue("suspect frequency"));
    setStructureVariableByName("suspect frequency low",getDefaultValue("suspect frequency low"));
    setStructureVariableByName("suspect frequency high",getDefaultValue("suspect frequency high"));
    setStructureVariableByName("suspect frequency low value",getDefaultValue("suspect frequency low value"));
    setStructureVariableByName("suspect frequency high value",getDefaultValue("suspect frequency high value"));
    setStructureVariableByName("reject time",getDefaultValue("reject time"));
    setStructureVariableByName("reject time low",getDefaultValue("reject time low"));
    setStructureVariableByName("reject time high",getDefaultValue("reject time high"));
    setStructureVariableByName("reject time low value",getDefaultValue("reject time low value"));
    setStructureVariableByName("reject time high value",getDefaultValue("reject time high value"));
    setStructureVariableByName("reject energy",getDefaultValue("reject energy"));
    setStructureVariableByName("reject energy low",getDefaultValue("reject energy low"));
    setStructureVariableByName("reject energy high",getDefaultValue("reject energy high"));
    setStructureVariableByName("reject energy low value",getDefaultValue("reject energy low value"));
    setStructureVariableByName("reject energy high value",getDefaultValue("reject energy high value"));
    setStructureVariableByName("reject peak power",getDefaultValue("reject peak power"));
    setStructureVariableByName("reject peak power low",getDefaultValue("reject peak power low"));
    setStructureVariableByName("reject peak power high",getDefaultValue("reject peak power high"));
    setStructureVariableByName("reject peak power low value",getDefaultValue("reject peak power low value"));
    setStructureVariableByName("reject peak power high value",getDefaultValue("reject peak power high value"));
    setStructureVariableByName("reject collapse distance",getDefaultValue("reject collapse distance"));
    setStructureVariableByName("reject collapse distance low",getDefaultValue("reject collapse distance low"));
    setStructureVariableByName("reject collapse distance high",getDefaultValue("reject collapse distance high"));
    setStructureVariableByName("reject collapse distance low value",getDefaultValue("reject collapse distance low value"));
    setStructureVariableByName("reject collapse distance high value",getDefaultValue("reject collapse distance high value"));
    setStructureVariableByName("reject absolute distance",getDefaultValue("reject absolute distance"));
    setStructureVariableByName("reject absolute distance low",getDefaultValue("reject absolute distance low"));
    setStructureVariableByName("reject absolute distance high",getDefaultValue("reject absolute distance high"));
    setStructureVariableByName("reject absolute distance low value",getDefaultValue("reject absolute distance low value"));
    setStructureVariableByName("reject absolute distance high value",getDefaultValue("reject absolute distance high value"));
    setStructureVariableByName("reject trigger distance",getDefaultValue("reject trigger distance"));
    setStructureVariableByName("reject trigger distance low",getDefaultValue("reject trigger distance low"));
    setStructureVariableByName("reject trigger distance high",getDefaultValue("reject trigger distance high"));
    setStructureVariableByName("reject trigger distance low value",getDefaultValue("reject trigger distance low value"));
    setStructureVariableByName("reject trigger distance high value",getDefaultValue("reject trigger distance high value"));
    setStructureVariableByName("reject end weld force",getDefaultValue("reject end weld force"));
    setStructureVariableByName("reject end weld force low",getDefaultValue("reject end weld force low"));
    setStructureVariableByName("reject end weld force high",getDefaultValue("reject end weld force high"));
    setStructureVariableByName("reject end weld force low value",getDefaultValue("reject end weld force low value"));
    setStructureVariableByName("reject end weld force high value",getDefaultValue("reject end weld force high value"));
    setStructureVariableByName("reject frequency",getDefaultValue("reject frequency"));
    setStructureVariableByName("reject frequency low",getDefaultValue("reject frequency low"));
    setStructureVariableByName("reject frequency high",getDefaultValue("reject frequency high"));
    setStructureVariableByName("reject frequency low value",getDefaultValue("reject frequency low value"));
    setStructureVariableByName("reject frequency high value",getDefaultValue("reject frequency high value"));
    setStructureVariableByName("timed seek period",getDefaultValue("timed seek period"));
    setStructureVariableByName("timed seek",getDefaultValue("timed seek"));
    setStructureVariableByName("pre-weld seek",getDefaultValue("pre-weld seek"));
    setStructureVariableByName("post-weld seek",getDefaultValue("post-weld seek"));
    if(tmpRecipeNum == 0)
    {
        setStructureVariableByName("user",getDefaultValue("user"));
        setStructureVariableByName("stackserialno","0");
        setStructureVariableByName("recipename",getDefaultValue("recipename"));
        setStructureVariableByName("companyname", "");
    }
    else
        setStructureVariableByName("companyname", "MyCompany");
    //PC
    setStructureVariableByName("amplitudeloopc1",getDefaultValue("amplitudeloopc1"));
    setStructureVariableByName("amplitudeloopc2",getDefaultValue("amplitudeloopc2"));
    setStructureVariableByName("phase loop",getDefaultValue("phase loop"));
    setStructureVariableByName("phase loop cf",getDefaultValue("phase loop cf"));
    setStructureVariableByName("frequency low",getDefaultValue("frequency low"));
    setStructureVariableByName("frequency high",getDefaultValue("frequency high"));
    setStructureVariableByName("phase limit time",getDefaultValue("phase limit time"));
    setStructureVariableByName("phase limit",getDefaultValue("phase limit"));
    setStructureVariableByName("control mode",getDefaultValue("control mode"));
    setStructureVariableByName("blind time seek",getDefaultValue("blind time seek"));
    setStructureVariableByName("blind time weld",getDefaultValue("blind time weld"));
    setStructureVariableByName("dds switch time",getDefaultValue("dds switch time"));
    setStructureVariableByName("parameter7",getDefaultValue("parameter7"));
    setStructureVariableByName("parameter8",getDefaultValue("parameter8"));
    setStructureVariableByName("parameter9",getDefaultValue("parameter9"));
    setStructureVariableByName("parameter10",getDefaultValue("parameter10"));
    setStructureVariableByName("parameter11",getDefaultValue("parameter11"));
    setStructureVariableByName("parameter12",getDefaultValue("parameter12"));
    setStructureVariableByName("parameter13",getDefaultValue("parameter13"));
    setStructureVariableByName("parameter14",getDefaultValue("parameter14"));
    setStructureVariableByName("parameter15",getDefaultValue("parameter15"));
    setStructureVariableByName("f_limit time",getDefaultValue("f_limit time"));
    setStructureVariableByName("amp proportional gain",getDefaultValue("amp proportional gain"));
    setStructureVariableByName("amp integral gain",getDefaultValue("amp integral gain"));
    setStructureVariableByName("phase integral gain",getDefaultValue("phase integral gain"));
    setStructureVariableByName("phase proportional gain",getDefaultValue("phase proportional gain"));
    setStructureVariableByName("frequency window size",getDefaultValue("frequency window size"));
    setStructureVariableByName("pf phase control loop",getDefaultValue("pf phase control loop"));
    setStructureVariableByName("pi frequency low",getDefaultValue("pi frequency low"));
    setStructureVariableByName("pi frequency high",getDefaultValue("pi frequency high"));
    setStructureVariableByName("pi phase limit time",getDefaultValue("pi phase limit time"));
    setStructureVariableByName("pi phase limit",getDefaultValue("pi phase limit"));
    setStructureVariableByName("weld ramp time",getDefaultValue("weld ramp time"));
    setStructureVariableByName("start frequency",getDefaultValue("start frequency"));
    setStructureVariableByName("memory offset",getDefaultValue("memory offset"));
    setStructureVariableByName("digital tune",getDefaultValue("digital tune"));
    setStructureVariableByName("frequency offset",getDefaultValue("frequency offset"));
    //AC
    setStructureVariableByName("weld force",getDefaultValue("weld force"));
    setStructureVariableByName("force ramp time",getDefaultValue("force ramp time"));
    setStructureVariableByName("holdmode",getDefaultValue("holdmode"));
    setStructureVariableByName("total collapse target",getDefaultValue("total collapse target"));
    setStructureVariableByName("hold force",getDefaultValue("hold force"));
    setStructureVariableByName("hold force ramp time",getDefaultValue("hold force ramp time"));
    setStructureVariableByName("expected part contact",getDefaultValue("expected part contact"));
    setStructureVariableByName("ready offset",getDefaultValue("ready offset"));
    setStructureVariableByName("down acceleration",getDefaultValue("down acceleration"));
    setStructureVariableByName("down max velocity",getDefaultValue("down max velocity"));
    setStructureVariableByName("down deceleration",getDefaultValue("down deceleration"));
    setStructureVariableByName("return acceleration",getDefaultValue("return acceleration"));
    setStructureVariableByName("return max velocity",getDefaultValue("return max velocity"));
    setStructureVariableByName("return deceleration",getDefaultValue("return deceleration"));
    setStructureVariableByName("expected contact offset",getDefaultValue("expected contact offset"));
    setStructureVariableByName("part contact window minus",getDefaultValue("part contact window minus"));
    setStructureVariableByName("part contact window plus",getDefaultValue("part contact window plus"));
    setStructureVariableByName("force step ramp value 1",getDefaultValue("force step ramp value 1"));
    setStructureVariableByName("force step ramp value 2",getDefaultValue("force step ramp value 2"));
    setStructureVariableByName("force step ramp value 3",getDefaultValue("force step ramp value 3"));
    setStructureVariableByName("force step ramp value 4",getDefaultValue("force step ramp value 4"));
    setStructureVariableByName("force step ramp value 5",getDefaultValue("force step ramp value 5"));
    setStructureVariableByName("force step ramp value 6",getDefaultValue("force step ramp value 6"));
    setStructureVariableByName("force step ramp value 7",getDefaultValue("force step ramp value 7"));
    setStructureVariableByName("force step ramp value 8",getDefaultValue("force step ramp value 8"));
    setStructureVariableByName("force step ramp value 9",getDefaultValue("force step ramp value 9"));
    setStructureVariableByName("force step ramp value 10",getDefaultValue("force step ramp value 10"));
    setStructureVariableByName("force step 1",getDefaultValue("force step 1"));
    setStructureVariableByName("force step 2",getDefaultValue("force step 2"));
    setStructureVariableByName("force step 3",getDefaultValue("force step 3"));
    setStructureVariableByName("force step 4",getDefaultValue("force step 4"));
    setStructureVariableByName("force step 5",getDefaultValue("force step 5"));
    setStructureVariableByName("force step 6",getDefaultValue("force step 6"));
    setStructureVariableByName("force step 7",getDefaultValue("force step 7"));
    setStructureVariableByName("force step 8",getDefaultValue("force step 8"));
    setStructureVariableByName("force step 9",getDefaultValue("force step 9"));
    setStructureVariableByName("force step 10",getDefaultValue("force step 10"));
    setStructureVariableByName("ready position toggle",getDefaultValue("ready position toggle"));
    setStructureVariableByName("weld force control",getDefaultValue("weld force control"));

    if(tmpRecipeNum == 0)
    {
        m_weldRecipe.m_weldRecipe_UI.isActive = false;
        m_weldRecipe.m_weldRecipe_UI.isLock = false;
        m_weldRecipe.m_weldRecipe_UI.isModified = false;
        m_weldRecipe.m_weldRecipe_UI.isNewRecipe = false;
        m_weldRecipe.m_weldRecipe_UI.isValid = false;
    }

    setStructureVariableByName("global setup",getDefaultValue("global setup"));
    setStructureVariableByName("setup weld mode status",getDefaultValue("setup weld mode status"));
    setStructureVariableByName("setup weld mode low",getDefaultValue("setup weld mode low"));
    setStructureVariableByName("setup weld mode high",getDefaultValue("setup weld mode high"));
    setStructureVariableByName("setup weld amplitude status",getDefaultValue("setup weld amplitude status"));
    setStructureVariableByName("setup weld amplitude low",getDefaultValue("setup weld amplitude low"));
    setStructureVariableByName("setup weld amplitude high",getDefaultValue("setup weld amplitude high"));
    setStructureVariableByName("setup weld force status",getDefaultValue("setup weld force status"));
    setStructureVariableByName("setup weld force low",getDefaultValue("setup weld force low"));
    setStructureVariableByName("setup weld force high",getDefaultValue("setup weld force high"));
    setStructureVariableByName("setup trigger force status",getDefaultValue("setup trigger force status"));
    setStructureVariableByName("setup trigger force low",getDefaultValue("setup trigger force low"));
    setStructureVariableByName("setup trigger force high",getDefaultValue("setup trigger force high"));
    setStructureVariableByName("setup hold force status",getDefaultValue("setup hold force status"));
    setStructureVariableByName("setup hold force low",getDefaultValue("setup hold force low"));
    setStructureVariableByName("setup hold force high",getDefaultValue("setup hold force high"));
    setStructureVariableByName("setup hold time status",getDefaultValue("setup hold time status"));
    setStructureVariableByName("setup hold time low",getDefaultValue("setup hold time low"));
    setStructureVariableByName("setup hold time high",getDefaultValue("setup hold time high"));
    setStructureVariableByName("setup scrub amplitude status",getDefaultValue("setup scrub amplitude status"));
    setStructureVariableByName("setup scrub amplitude low",getDefaultValue("setup scrub amplitude low"));
    setStructureVariableByName("setup scrub amplitude high",getDefaultValue("setup scrub amplitude high"));
    setStructureVariableByName("setup scrub time status",getDefaultValue("setup scrub time status"));
    setStructureVariableByName("setup scrub time low",getDefaultValue("setup scrub time low"));
    setStructureVariableByName("setup scrub time high",getDefaultValue("setup scrub time high"));
    setStructureVariableByName("setup reactivity status",getDefaultValue("setup reactivity status"));
    setStructureVariableByName("setup reactivity low",getDefaultValue("setup reactivity low"));
    setStructureVariableByName("setup reactivity high",getDefaultValue("setup reactivity high"));
    setStructureVariableByName("setup force level status",getDefaultValue("setup force level status"));
    setStructureVariableByName("setup force level low",getDefaultValue("setup force level low"));
    setStructureVariableByName("setup force level high",getDefaultValue("setup force level high"));
}

/**
 * @brief WeldRecipeParameter::setStructureVariableByName : Set the Name and IndexValues
 * @param paramEditText
 * @param qstrIndexValue
 */
void WeldRecipeParameter::setStructureVariableByName(QString paramEditText ,QString qstrIndexValue)
{
    if(paramEditText.toLower() == "abs distance")
        paramEditText = "absolute distance";
    if(paramEditText.toLower() == "col distance")
        paramEditText = "collapse distance";
    int dwParamIndex = m_MapRecipeParams[paramEditText.toLower()].ParamIndexInStructure;
    QString qstrunit = m_MapRecipeParams[paramEditText.toLower()].ParamUnit[m_UnitSelected];
    QString indexValue = qstrIndexValue;

    if((paramEditText.toLower() == WELD_SETUPMODE_HIGH) ||
            (paramEditText.toLower() == WELD_SETUPMODE_LOW))
    {
        int mode;
        mode = getStructureValueByIndex(SC_WELD_MODE).toInt();
        ModeInfo stModeInfo = getModeInfoByIndex(mode,"");
        qstrunit = stModeInfo.ModeUnit;
    }
    
    if(qstrunit == CUST_NAME_UINTMilliMeter)
    {
        double dval = convertMillimetreToMicrometre(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }
    else if(qstrunit == CUST_NAME_UINTJOULES)
    {
        double dval = Utility::convertJoulesToMilliJoules(qstrIndexValue.toDouble());
    	indexValue = QString::number(dval);
    }
    else if(qstrunit == CUST_NAME_UINTSEC)
    {
        double dval = convertSecToMsec(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }
    else if(qstrunit == CUST_NAME_UINTLBS)
    {
        double dval = convertlbstoN(qstrIndexValue.toDouble());
        indexValue = QString::number(round(dval));
    }
    else if(qstrunit == CUST_NAME_UINTINCHES)
    {
        double dval = convertinchestomicrometre(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }
    else if(qstrunit == CUST_NAME_UINIINSEC || qstrunit == CUST_NAME_UINIINSECSQU)
    {
        double dval = convertinchestomillimetre(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }

    if(dwParamIndex == SC_MODE_VALUE_TIME ||
            dwParamIndex == SC_MODE_VALUE_ENERGY ||
            dwParamIndex == SC_MODE_VALUE_POWER ||
            dwParamIndex == SC_MODE_VALUE_AD ||
            dwParamIndex == SC_MODE_VALUE_CD ||
            dwParamIndex == SC_MODE_VALUE_ST ||
            dwParamIndex == SC_MODE_VALUE_GD ||
            dwParamIndex == SC_MODE_VALUE_DY ||
            dwParamIndex == SC_MODE_VALUE_DENSITY)
    {
        dwParamIndex = SC_MODE_VALUE;
    }
    int dwStepAt ;
    double dval;
    if((dwParamIndex >= SC_FORCE_STEP_VALUE1) && (dwParamIndex <= SC_FORCE_STEP_VALUE10))
    {
        dwStepAt = getStructureValueByIndex(SC_FORCE_STEP_AT).toInt();

        /*To handle changed in Enum sequence as Step@PK Power option is removed in ASX*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
           if(dwStepAt > STEP_AT_ENERGY)
           {
             dwStepAt = dwStepAt + 1;
           }
        }

        if(dwStepAt == STEP_AT_TIME)
        {
            dval = convertSecToMsec(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
        else if(dwStepAt == STEP_AT_ENERGY)
        {
            dval = Utility::convertJoulesToMilliJoules(qstrIndexValue.toDouble());
        	indexValue = QString::number(dval);
        }
        else if((dwStepAt == STEP_AT_ABSOLUTE_DISTANCE) || (dwStepAt == STEP_AT_COLLAPSE_DISTANCE))
        {
            if(m_UnitSelected == UNIT_SI)
                dval = convertMillimetreToMicrometre(qstrIndexValue.toDouble());
            else
                dval = convertinchestomicrometre(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
    }
    else if((dwParamIndex >= SC_AMPLITUDE_STEP_VALUE1) && (dwParamIndex <= SC_AMPLITUDE_STEP_VALUE10))
    {
        dwStepAt = getStructureValueByIndex(SC_AMPLITUDE_STEP_AT).toInt();

        /*To handle changed in Enum sequence as Step@PK Power option is removed in ASX*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
           if(dwStepAt > STEP_AT_ENERGY)
           {
             dwStepAt = dwStepAt + 1;
           }
        }

        if(dwStepAt == STEP_AT_TIME)
        {
            dval = convertSecToMsec(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
        else if(dwStepAt == STEP_AT_ENERGY)
        {
            dval = Utility::convertJoulesToMilliJoules(qstrIndexValue.toDouble());
        	indexValue = QString::number(dval);
        }
        else if((dwStepAt == STEP_AT_ABSOLUTE_DISTANCE) || (dwStepAt == STEP_AT_COLLAPSE_DISTANCE))
        {
            if(m_UnitSelected == UNIT_SI)
                dval = convertMillimetreToMicrometre(qstrIndexValue.toDouble());
            else
                dval = convertinchestomicrometre(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
    }
    if(dwParamIndex == SC_PRETRIGGER )
    {
        setStructureVariableByIndex(dwParamIndex,indexValue);
    }
    if(paramEditText == WELD_DISTANCE_AUTO)
    {
        if(qstrIndexValue == "1")
        {
            indexValue = "1";
            dwParamIndex = SC_AUTO_PRETRIGGER;
            setStructureVariableByIndex(dwParamIndex,indexValue);
            indexValue = ZERO_STR;
            dwParamIndex = SC_DISTANCE_PRETRIGGER;
            setStructureVariableByIndex(dwParamIndex,indexValue);
        }
        else
        {
            indexValue = "1";
            dwParamIndex = SC_DISTANCE_PRETRIGGER;
            setStructureVariableByIndex(dwParamIndex,indexValue);
            indexValue = ZERO_STR;
            dwParamIndex = SC_AUTO_PRETRIGGER;
            setStructureVariableByIndex(dwParamIndex,indexValue);
        }
    }
    else
        setStructureVariableByIndex(dwParamIndex,indexValue);
}

/**************************************************************************//**
*
* \brief To get the default data from m_MapRecipeParams using EditText and IndexValue and
*       convert the default data to the raw default data following the unit.
*       The function will be not available for the user name, recipe name and company name variables.
*
* \param EditText, IndexValue
*
* \return raw default value.
*
******************************************************************************/
int WeldRecipeParameter::getDefaultVariableValue(QString paramEditText ,QString qstrIndexValue)
{
    if(paramEditText.toLower() == "abs distance")
        paramEditText = "absolute distance";
    if(paramEditText.toLower() == "col distance")
        paramEditText = "collapse distance";
    int dwParamIndex = m_MapRecipeParams[paramEditText.toLower()].ParamIndexInStructure;
    QString qstrunit = m_MapRecipeParams[paramEditText.toLower()].ParamUnit[m_UnitSelected];
    QString indexValue = qstrIndexValue;
    if((dwParamIndex == SC_USER) || (dwParamIndex == UI_RECIPE_NAME) || (dwParamIndex == UI_COMPANYNAME))
        return -1;

    if((paramEditText.toLower() == WELD_SETUPMODE_HIGH) ||
            (paramEditText.toLower() == WELD_SETUPMODE_LOW))
    {
        int mode;
        mode = getStructureValueByIndex(SC_WELD_MODE).toInt();
        ModeInfo stModeInfo = getModeInfoByIndex(mode,"");
        qstrunit = stModeInfo.ModeUnit;
    }

    if(qstrunit == CUST_NAME_UINTMilliMeter)
    {
        double dval = convertMillimetreToMicrometre(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }
    else if(qstrunit == CUST_NAME_UINTJOULES)
    {
        int val = qstrIndexValue.toDouble()*1000;
        indexValue = QString::number(val);
    }
    else if(qstrunit == CUST_NAME_UINTSEC)
    {
        double dval = convertSecToMsec(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }
    else if(qstrunit == CUST_NAME_UINTLBS)
    {
        double dval = convertlbstoN(qstrIndexValue.toDouble());
        indexValue = QString::number(round(dval));
    }
    else if(qstrunit == CUST_NAME_UINTINCHES)
    {
        double dval = convertinchestomicrometre(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }
    else if(qstrunit == CUST_NAME_UINIINSEC || qstrunit == CUST_NAME_UINIINSECSQU)
    {
        double dval = convertinchestomillimetre(qstrIndexValue.toDouble());
        indexValue = QString::number(dval);
    }

    if(dwParamIndex == SC_MODE_VALUE_TIME ||
            dwParamIndex == SC_MODE_VALUE_ENERGY ||
            dwParamIndex == SC_MODE_VALUE_POWER ||
            dwParamIndex == SC_MODE_VALUE_AD ||
            dwParamIndex == SC_MODE_VALUE_CD ||
            dwParamIndex == SC_MODE_VALUE_ST ||
            dwParamIndex == SC_MODE_VALUE_GD ||
            dwParamIndex == SC_MODE_VALUE_DY ||
            dwParamIndex == SC_MODE_VALUE_DENSITY)
    {
        dwParamIndex = SC_MODE_VALUE;
    }
    int dwStepAt ;
    double dval;
    if((dwParamIndex >= SC_FORCE_STEP_VALUE1) && (dwParamIndex <= SC_FORCE_STEP_VALUE10))
    {
        dwStepAt = getStructureValueByIndex(SC_FORCE_STEP_AT).toInt();

        /*To handle changed in Enum sequence as Step@PK Power option is removed in ASX*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
           if(dwStepAt > STEP_AT_ENERGY)
           {
             dwStepAt = dwStepAt + 1;
           }
        }

        if(dwStepAt == STEP_AT_TIME)
        {
            dval = convertSecToMsec(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
        else if(dwStepAt == STEP_AT_ENERGY)
        {
            dval = Utility::convertJoulesToMilliJoules(qstrIndexValue.toDouble());
        	indexValue = QString::number(dval);
        }
        else if((dwStepAt == STEP_AT_ABSOLUTE_DISTANCE) || (dwStepAt == STEP_AT_COLLAPSE_DISTANCE))
        {
            if(m_UnitSelected == UNIT_SI)
                dval = convertMillimetreToMicrometre(qstrIndexValue.toDouble());
            else
                dval = convertinchestomicrometre(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
    }
    else if((dwParamIndex >= SC_AMPLITUDE_STEP_VALUE1) && (dwParamIndex <= SC_AMPLITUDE_STEP_VALUE10))
    {
        dwStepAt = getStructureValueByIndex(SC_AMPLITUDE_STEP_AT).toInt();

        /*To handle changed in Enum sequence as Step@PK Power option is removed in ASX*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
           if(dwStepAt > STEP_AT_ENERGY)
           {
             dwStepAt = dwStepAt + 1;
           }
        }

        if(dwStepAt == STEP_AT_TIME)
        {
            dval = convertSecToMsec(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
        else if(dwStepAt == STEP_AT_ENERGY)
        {
            dval = Utility::convertJoulesToMilliJoules(qstrIndexValue.toDouble());
        	indexValue = QString::number(dval);
        }
        else if((dwStepAt == STEP_AT_ABSOLUTE_DISTANCE) || (dwStepAt == STEP_AT_COLLAPSE_DISTANCE))
        {
            if(m_UnitSelected == UNIT_SI)
                dval = convertMillimetreToMicrometre(qstrIndexValue.toDouble());
            else
                dval = convertinchestomicrometre(qstrIndexValue.toDouble());
            indexValue = QString::number(dval);
        }
    }
    return indexValue.toInt();
}

/**
 * @brief WeldRecipeParameter::setStructureVariableByIndex : Set the index of assign Name and Modified Values
 * @param Struct_Index
 * @param modifiedValue
 */
void WeldRecipeParameter::setStructureVariableByIndex(int Struct_Index, QString modifiedValue)
{
    switch(Struct_Index)
    {
    case SC_USER:
        memset(m_weldRecipe.m_weldRecipe_SC.User, 0, sizeof(m_weldRecipe.m_weldRecipe_SC.User));
        strcpy(m_weldRecipe.m_weldRecipe_SC.User,modifiedValue.toStdString().c_str());
        break;
    case UI_RECIPE_NAME:
        memset(m_weldRecipe.m_weldRecipe_UI.RecipeName, 0, sizeof(m_weldRecipe.m_weldRecipe_UI.RecipeName));
        strcpy(m_weldRecipe.m_weldRecipe_UI.RecipeName,modifiedValue.toStdString().c_str());
        break;
    case UI_COMPANYNAME:
        memset(m_weldRecipe.m_weldRecipe_UI.CompanyName, 0, sizeof(m_weldRecipe.m_weldRecipe_UI.CompanyName));
        strcpy(m_weldRecipe.m_weldRecipe_UI.CompanyName,modifiedValue.toStdString().c_str());
        break;
    default:
        setStructureVariableByIndex(Struct_Index, modifiedValue.toDouble());
        break;
    }
}

/**
 * @brief WeldRecipeParameter::setStructureVariableByIndex : Get the index of assign Name and return the values
 * @param Struct_Index
 * @param modifiedValue
 */
void WeldRecipeParameter::setStructureVariableByIndex(int Struct_Index, int modifiedValue)
{
    switch (Struct_Index)
    {
    case SC_RECIPE_NUMBER:
        m_weldRecipe.m_weldRecipe_SC.RecipeNumber = modifiedValue;
        break;
    case SC_RECIPE_VER_NUMBER:
        m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber = modifiedValue;
        break;
    case SC_WELD_MODE:
        m_weldRecipe.m_weldRecipe_SC.WeldMode = modifiedValue;
        break;
    case SC_MODE_VALUE:
        /*In order to make unit place of Absolute Distance to 0,for ASX precision handling while writing*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            INT32 localValue;
            localValue = (modifiedValue % 10);
            localValue = (modifiedValue - localValue);
            m_weldRecipe.m_weldRecipe_SC.ModeValue = localValue;
        }

        else
        {
            m_weldRecipe.m_weldRecipe_SC.ModeValue = modifiedValue;
        }
        break;        
    case SC_HOLD_TIME:
        m_weldRecipe.m_weldRecipe_SC.HoldTime = modifiedValue;
        break;
    case SC_TRIGGER_FORCE:
        m_weldRecipe.m_weldRecipe_SC.TriggerForce = modifiedValue;
        break;
    case SC_TRIGGER_DISTANCE:
        m_weldRecipe.m_weldRecipe_SC.TriggerDistance = modifiedValue;
        break;
    case SC_MAX_WELD_TIMEOUT:
        m_weldRecipe.m_weldRecipe_SC.MaxWeldTimeout = modifiedValue;
        break;
    case SC_NUM_AMPLITUDE_STEPS:
        m_weldRecipe.m_weldRecipe_SC.NumAmplitudeSteps = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_AT:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepAt = modifiedValue;
        break;
    case SC_NUM_FORCE_STEPS:
        m_weldRecipe.m_weldRecipe_SC.NumForceSteps = modifiedValue;
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.NumForceSteps = modifiedValue;
        break;
    case SC_FORCE_STEP_AT:
        m_weldRecipe.m_weldRecipe_SC.ForceStepAt = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE1:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[0] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE2:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[1] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE3:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[2] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE4:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[3] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE5:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[4] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE6:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[5] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE7:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[6] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE8:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[7] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE9:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[8] = modifiedValue;
        break;
    case SC_FORCE_STEP_VALUE10:
        m_weldRecipe.m_weldRecipe_SC.ForceStepValue[9] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE1:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[0] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE2:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[1] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE3:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[2] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE4:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[3] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE5:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[4] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE6:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[5] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE7:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[6] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE8:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[7] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE9:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[8] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP_VALUE10:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[9] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP1:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[0] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP2:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[1] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP3:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[2] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP4:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[3] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP5:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[4] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP6:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[5] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP7:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[6] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP8:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[7] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP9:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[8] = modifiedValue;
        break;
    case SC_AMPLITUDE_STEP10:
        m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[9] = modifiedValue;
        break;
    case SC_COOLING_VALVE:
        m_weldRecipe.m_weldRecipe_SC.bIsCoolingValeEnabled = modifiedValue;
        break;
    case SC_GLOBAL_SUSPECT:
        m_weldRecipe.m_weldRecipe_SC.IsGlobalSuspectEnabled = modifiedValue;
        break;
    case SC_GLOBAL_REJECT:
        m_weldRecipe.m_weldRecipe_SC.IsGlobalRejectEnabled = modifiedValue;
        break;
    case SC_CONTROL_LIMIT:
        m_weldRecipe.m_weldRecipe_SC.bIsControlLimitEnabled = modifiedValue;
        break;
    case SC_CL_PEAKPOWER_CUTOFF:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.IsEnabled = modifiedValue;
        break;
    case SC_CL_PEAKPOWER_CUTOFF_VAL:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.Value = modifiedValue;
        break;
    case SC_CL_ABSOLUTE_CUTOFF:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.IsEnabled = modifiedValue;
        break;
    case SC_CL_ABSOLUTE_CUTOFF_VAL:
        /*In order to make unit place of Absolute Distance Cutoff to 0,for ASX precision handling while writing*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            INT32 localValue;
            localValue = (modifiedValue % 10);
            localValue = (modifiedValue - localValue);
            m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value = localValue;
        }

        else
        {
            m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value = modifiedValue;
        }
        break;
    case SC_CL_COLLAPSE_CUTOFF:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.CollapseCutoff.IsEnabled = modifiedValue;
        break;
    case SC_CL_COLLAPSE_CUTOFF_VAL:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.CollapseCutoff.Value = modifiedValue;
        break;
    case SC_CL_TIME_CUTOFF:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.TimeCutoff.IsEnabled = modifiedValue;
        break;
    case SC_CL_TIME_CUTOFF_VAL:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.TimeCutoff.Value = modifiedValue;
        break;
    case SC_CL_FREQ_LOW:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyLow.IsEnabled = modifiedValue;
        break;
    case SC_CL_FREQ_LOW_VAL:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyLow.Value = modifiedValue;
        break;
    case SC_CL_FREQ_HIGH:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyHigh.IsEnabled = modifiedValue;
        break;
    case SC_CL_FREQ_HIGH_VAL:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyHigh.Value = modifiedValue;
        break;
    case SC_CL_ENERGY_CUTOFF:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.EnergyCutoff.IsEnabled = modifiedValue;
        break;
    case SC_CL_ENERGY_CUTOFF_VAL:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.EnergyCutoff.Value = modifiedValue;
        break;
    case SC_CL_GROUND_DETECT_CUTOFF:
        m_weldRecipe.m_weldRecipe_SC.ControlLimits.GroundDetectCutoff = modifiedValue;
        break;
    case SC_PRETRIGGER:
        m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsEnabled = modifiedValue;
        break;
    case SC_AUTO_PRETRIGGER:
        m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsAutoEnabled = modifiedValue;
        break;
    case SC_DISTANCE_PRETRIGGER:
        m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsDistanceEnabled = modifiedValue;
        break;
    case SC_PRETRIGGER_AMPLITUDE:
        m_weldRecipe.m_weldRecipe_SC.preTrigger.TriggerAmplitude = modifiedValue;
        break;
    case SC_PRETRIGGER_DISTANCE:
        m_weldRecipe.m_weldRecipe_SC.preTrigger.PreTriggerDistance = modifiedValue;
        break;
    case SC_AFTER_BURST:
        m_weldRecipe.m_weldRecipe_SC.AfterBurst.bIsEnabled = modifiedValue;
        break;
    case SC_AB_AMPLITUDE:
        m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstAmplitude = modifiedValue;
        break;
    case SC_AB_DELAY:
        m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstDelay = modifiedValue;
        break;
    case SC_AB_TIME:
        m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstTime = modifiedValue;
        break;
    case SC_ENERGY_BRAKE:
        m_weldRecipe.m_weldRecipe_SC.EnergyBrake.bIsEnabled = modifiedValue;
        break;
    case SC_ENERGY_BRAKE_AMPLITUDE:
        m_weldRecipe.m_weldRecipe_SC.EnergyBrake.EnergyBrakeAmplitude = modifiedValue;
        break;
    case SC_ENERGY_BRAKE_TIME:
        m_weldRecipe.m_weldRecipe_SC.EnergyBrake.EnergyBrakeTime = modifiedValue;
        break;
    case SC_SUSPECT_TIME_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_TIME_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_TIME_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_TIME_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.Low = modifiedValue;
        break;
    case SC_SUSPECT_TIME_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.High = modifiedValue;
        break;
    case SC_SUSPECT_ENERGY_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_ENERGY_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_ENERGY_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_ENERGY_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.Low = modifiedValue;
        break;
    case SC_SUSPECT_ENERGY_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.High = modifiedValue;
        break;
    case SC_SUSPECT_POWER_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_POWER_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_POWER_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_POWER_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.Low = modifiedValue;
        break;
    case SC_SUSPECT_POWER_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.High = modifiedValue;
        break;
    case SC_SUSPECT_CD_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_CD_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_CD_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_CD_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.Low = modifiedValue;
        break;
    case SC_SUSPECT_CD_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.High = modifiedValue;
        break;
    case SC_SUSPECT_AD_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_AD_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_AD_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_AD_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.Low = modifiedValue;
        break;
    case SC_SUSPECT_AD_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High = modifiedValue;
        break;
    case SC_SUSPECT_TD_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_TD_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_TD_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_TD_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.Low = modifiedValue;
        break;
    case SC_SUSPECT_TD_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.High = modifiedValue;
        break;
    case SC_SUSPECT_END_FORCE_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_END_FORCE_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_END_FORCE_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_END_FORCE_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.Low = modifiedValue;
        break;
    case SC_SUSPECT_END_FORCE_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.High = modifiedValue;
        break;
    case SC_SUSPECT_FREQ_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsEnabled = modifiedValue;
        break;
    case SC_SUSPECT_FREQ_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsLowEnabled = modifiedValue;
        break;
    case SC_SUSPECT_FREQ_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsHighEnabled = modifiedValue;
        break;
    case SC_SUSPECT_FREQ_LOW:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.Low = modifiedValue;
        break;
    case SC_SUSPECT_FREQ_HIGH:
        m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.High = modifiedValue;
        break;
    case SC_REJECT_TIME_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_TIME_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_TIME_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_TIME_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.Low = modifiedValue;
        break;
    case SC_REJECT_TIME_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.High = modifiedValue;
        break;
    case SC_REJECT_ENERGY_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_ENERGY_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_ENERGY_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_ENERGY_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.Low = modifiedValue;
        break;
    case SC_REJECT_ENERGY_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.High = modifiedValue;
        break;
    case SC_REJECT_POWER_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_POWER_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_POWER_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_POWER_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.Low = modifiedValue;
        break;
    case SC_REJECT_POWER_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.High = modifiedValue;
        break;
    case SC_REJECT_CD_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_CD_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_CD_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_CD_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.Low = modifiedValue;
        break;
    case SC_REJECT_CD_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.High = modifiedValue;
        break;
    case SC_REJECT_AD_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_AD_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_AD_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_AD_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.Low = modifiedValue;
        break;
    case SC_REJECT_AD_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High = modifiedValue;
        break;
    case SC_REJECT_TD_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_TD_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_TD_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_TD_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.Low = modifiedValue;
        break;
    case SC_REJECT_TD_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.High = modifiedValue;
        break;
    case SC_REJECT_END_FORCE_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_END_FORCE_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_END_FORCE_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_END_FORCE_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.Low = modifiedValue;
        break;
    case SC_REJECT_END_FORCE_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.High = modifiedValue;
        break;
    case SC_REJECT_FREQ_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsEnabled = modifiedValue;
        break;
    case SC_REJECT_FREQ_LOW_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsLowEnabled = modifiedValue;
        break;
    case SC_REJECT_FREQ_HIGH_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsHighEnabled = modifiedValue;
        break;
    case SC_REJECT_FREQ_LOW:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.Low = modifiedValue;
        break;
    case SC_REJECT_FREQ_HIGH:
        m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.High = modifiedValue;
        break;
    case SC_TIMED_SEEK_PERIOD:
        m_weldRecipe.m_weldRecipe_SC.TimedSeekPeriod = modifiedValue;
        break;
    case SC_TIMED_SEEK:
        m_weldRecipe.m_weldRecipe_SC.bIsTimedSeekEnabled = modifiedValue;
        break;
    case SC_PRE_WELD_SEEK:
        m_weldRecipe.m_weldRecipe_SC.bIsPreWeldSeekEnabled = modifiedValue;
        break;
    case SC_POST_WELD_SEEK:
        m_weldRecipe.m_weldRecipe_SC.bIsPostWeldSeekEnabled = modifiedValue;
        break;
    case SC_STACK_SERIAL_NO:
        m_weldRecipe.m_weldRecipe_SC.StackSerialNo = modifiedValue;
        break;
    case SC_REACTIVITY:
        m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.Reactivity = modifiedValue;
        break;
    case SC_FORCE_LEVEL_ENABLED:
        m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.IsForceLevelEnabled = modifiedValue;
        break;
    case SC_FORCE_LEVEL:
        m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.ForceLevel = modifiedValue;
        break;
    case SC_FORCE_LEVEL_TIME:
        m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.ForceLevelTime = modifiedValue;
        break;
    case SC_SCRUB_AMPLITUDE:
        m_weldRecipe.m_weldRecipe_SC.ScrubAmplitude = modifiedValue;
        break;
        //PC
    case PC_AMPLITUDE_LOOPC1:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC1 = modifiedValue;
        break;
    case PC_AMPLITUDE_LOOPC2:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC2 = modifiedValue;
        break;
    case PC_PHASE_LOOP:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoop = modifiedValue;
        break;
    case PC_PHASE_LOOPCF:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoopCF = modifiedValue;
        break;
    case PC_FREQUENCY_LOW:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyLow = modifiedValue;
        break;
    case PC_FREQUENCY_HIGH:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyHigh = modifiedValue;
        break;
    case PC_PHASE_LIMIT_TIME:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimitTime = modifiedValue;
        break;
    case PC_PHASE_LIMIT:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimit = modifiedValue;
        break;
    case PC_CONTROL_MODE:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.ControlMode = modifiedValue;
        break;
    case PC_BLIND_TIME_SEEK:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.BlindtimeSeek = modifiedValue;
        break;
    case PC_BLIND_TIME_WELD:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Blindtimeweld = modifiedValue;
        break;
    case PC_DDS_SWITCH_TIME:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.DDSSwitchtime = modifiedValue;
        break;
    case PC_PARAMETER7:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter7 = modifiedValue;
        break;
    case PC_PARAMETER8:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter8 = modifiedValue;
        break;
    case PC_PARAMETER9:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter9 = modifiedValue;
        break;
    case PC_PARAMETER10:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter10 = modifiedValue;
        break;
    case PC_PARAMETER11:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter11 = modifiedValue;
        break;
    case PC_PARAMETER12:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter12 = modifiedValue;
        break;
    case PC_PARAMETER13:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter13 = modifiedValue;
        break;
    case PC_PARAMETER14:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter14 = modifiedValue;
        break;
    case PC_PARAMETER15:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter15 = modifiedValue;
        break;
    case PC_F_LIMIT_TIME:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FLimitTime = modifiedValue;
        break;
    case PC_AMP_PROPORTIONAL_GAIN:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpProportionalGain = modifiedValue;
        break;
    case PC_AMP_INTEGRAL_GAIN:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpIntegralGain = modifiedValue;
        break;
    case PC_PHASE_INTEGRAL_GAIN:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseIntegralGain = modifiedValue;
        break;
    case PC_PHASE_PROPORTIONAL_GAIN:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseProportionalGain = modifiedValue;
        break;
    case PC_FREQUENCY_WINDOW_SIZE:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyWindowSize = modifiedValue;
        break;
    case PC_PF_OF_PHASE_CONTROL_LOOP:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PFofPhasecontrolloop = modifiedValue;
        break;
    case PC_PI_FREQUENCY_LOW:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyLow = modifiedValue;
        break;
    case PC_PI_FREQUENCY_HIGH:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyHigh = modifiedValue;
        break;
    case PC_PI_PHASE_LIMIT_TIME:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimitTime = modifiedValue;
        break;
    case PC_PI_PHASE_LIMIT:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimit = modifiedValue;
        break;
    case PC_WELD_RAMP_TIME:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime = modifiedValue;
        break;
    case PC_START_FREQUENCY:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.StartFrequency = modifiedValue;
        break;
    case PC_MEMORY_OFFSET:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.MemoryOffset = modifiedValue;
        break;
    case PC_DIGITAL_TUNE:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.DigitalTune = modifiedValue;
        break;
    case PC_FREQUENCY_OFFSET:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyOffset = modifiedValue;
        break;

        //AC
    case AC_WELD_FORCE:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce = modifiedValue;
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[0] = modifiedValue;
        break;
    case AC_FORCE_RAMP_TIME:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceRampTime = modifiedValue;
        break;
    case AC_HOLD_MODE:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldMode = modifiedValue;
        break;
    case AC_TOTAL_COLLAPSE_TARGET:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.TotalCollapseTarget = modifiedValue;
        break;
    case AC_HOLD_FORCE:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForce = modifiedValue;
        break;
    case AC_HOLD_FORCE_RAMP_TIME:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForceRampTime = modifiedValue;
        break;
    case AC_EXPECTED_PART_CONTACT_POSITION:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactPosition = modifiedValue;
        break;
    case AC_READY_POSITION:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPosition = modifiedValue;
        break;
    case AC_DOWN_ACCELERATION:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownAcceleration = modifiedValue;
        break;
    case AC_DOWN_MAX_VELOCITY:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownMaxVelocity = modifiedValue;
        break;
    case AC_DOWN_DECELERATION:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownDeceleration = modifiedValue;
        break;
    case AC_RETURN_ACCELERATION:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnAcceleration = modifiedValue;
        break;
    case AC_RETURN_MAX_VELOCITY:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnMaxVelocity = modifiedValue;
        break;
    case AC_RETURN_DECELERATION:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnDeceleration = modifiedValue;
        break;
    case AC_PART_CONTANCT_OFFSET:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactOffset = modifiedValue;
        break;
    case AC_PART_CONTANCT_WINDOW_MINUS:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowMinus = modifiedValue;
        break;
    case AC_PART_CONTANCT_WINDOW_PLUS:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowPlus = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE1:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[0] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE2:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[1] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE3:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[2] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE4:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[3] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE5:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[4] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE6:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[5] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE7:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[6] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE8:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[7] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE9:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[8] = modifiedValue;
        break;
    case AC_FORCE_STEP_RAMP_VALUE10:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[9] = modifiedValue;
        break;
    case AC_FORCE_STEP1:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce = modifiedValue;
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[0] = modifiedValue;
        break;
    case AC_FORCE_STEP2:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[1] = modifiedValue;
        break;
    case AC_FORCE_STEP3:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[2] = modifiedValue;
        break;
    case AC_FORCE_STEP4:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[3] = modifiedValue;
        break;
    case AC_FORCE_STEP5:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[4] = modifiedValue;
        break;
    case AC_FORCE_STEP6:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[5] = modifiedValue;
        break;
    case AC_FORCE_STEP7:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[6] = modifiedValue;
        break;
    case AC_FORCE_STEP8:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[7] = modifiedValue;
        break;
    case AC_FORCE_STEP9:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[8] = modifiedValue;
        break;
    case AC_FORCE_STEP10:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[9] = modifiedValue;
        break;
    case AC_READY_POSITION_TOGGLE:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPositionToggle = modifiedValue;
        break;
    case AC_WELD_FORCE_CONTROL:
        m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForceControl = modifiedValue;
        break;
        //UI
    case UI_ISACTIVE:
        m_weldRecipe.m_weldRecipe_UI.isActive = modifiedValue;
        break;
    case UI_ISLOCK:
        m_weldRecipe.m_weldRecipe_UI.isLock = modifiedValue;
        break;
    case UI_ISVALID:
        m_weldRecipe.m_weldRecipe_UI.isValid = modifiedValue;
        break;
    case UI_ISNEW_RECIPE:
        m_weldRecipe.m_weldRecipe_UI.isNewRecipe = modifiedValue;
        break;
    case UI_IS_MODIFIED:
        m_weldRecipe.m_weldRecipe_UI.isModified = modifiedValue;
        break;
    case UI_RECIPE_VERSION_NUMBER:
        m_weldRecipe.m_weldRecipe_UI.dwRecipeVerNumber = modifiedValue;
        break;
    case UI_GLOBAL_SETUP:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.isGlobalSetup = modifiedValue;
        break;
    case UI_SETUP_WELD_MODE_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeStatus = modifiedValue;
        break;
    case UI_SETUP_WELD_MODE_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeLow = modifiedValue;
        break;
    case UI_SETUP_WELD_MODE_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeHigh = modifiedValue;
        break;
    case UI_WELD_AMP_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpStatus = modifiedValue;
        break;
    case UI_WELD_AMP_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpLow = modifiedValue;
        break;
    case UI_WELD_AMP_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpHigh = modifiedValue;
        break;
    case UI_WELD_FORCE_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceStatus = modifiedValue;
        break;
    case UI_WELD_FORCE_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceLow = modifiedValue;
        break;
    case UI_WELD_FORCE_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceHigh = modifiedValue;
        break;
    case UI_TRIGGER_FORCE_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceStatus = modifiedValue;
        break;
    case UI_TRIGGER_FORCE_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceLow = modifiedValue;
        break;
    case UI_TRIGGER_FORCE_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceHigh = modifiedValue;
        break;
    case UI_HOLD_FORCE_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceStatus = modifiedValue;
        break;
    case UI_HOLD_FORCE_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceLow = modifiedValue;
        break;
    case UI_HOLD_FORCE_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceHigh = modifiedValue;
        break;
    case UI_HOLD_TIME_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeStatus = modifiedValue;
        break;
    case UI_HOLD_TIME_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeLow = modifiedValue;
        break;
    case UI_HOLD_TIME_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeHigh = modifiedValue;
        break;
    case UI_SCRUB_AMP_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpStatus = modifiedValue;
        break;
    case UI_SCRUB_AMP_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpLow = modifiedValue;
        break;
    case UI_SCRUB_AMP_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpHigh = modifiedValue;
        break;
    case UI_SCRUB_TIME_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeStatus = modifiedValue;
        break;
    case UI_SCRUB_TIME_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeLow = modifiedValue;
        break;
    case UI_SCRUB_TIME_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeHigh = modifiedValue;
        break;
    case UI_REACTIVITY_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityStatus = modifiedValue;
        break;
    case UI_REACTIVITY_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityLow = modifiedValue;
        break;
    case UI_REACTIVITY_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityHigh = modifiedValue;
        break;
    case UI_FORCE_LEVEL_STATUS:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelStatus = modifiedValue;
        break;
    case UI_FORCE_LEVEL_LOW:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelLow = modifiedValue;
        break;
    case UI_FORCE_LEVEL_HIGH:
        m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelHigh = modifiedValue;
        break;
    default :
        break;
    }
}

/**
 * @brief WeldRecipeParameter::readWeldRecipeParameterFromDB : Read WeldRecipeParameter From DB
 * @param a_RecivedBuffer
 */
void WeldRecipeParameter::readWeldRecipeParameterFromDB(QString a_RecivedBuffer)
{
    a_RecivedBuffer.replace("'","");
    QStringList query = a_RecivedBuffer.split(",");
    int dwDBIndex=0;
    WeldRecipe *temp = &m_weldRecipe;

    temp->m_weldRecipe_SC.RecipeNumber = query.at(dwDBIndex++).toInt();
    strcpy(temp->m_weldRecipe_UI.RecipeName,query.at(dwDBIndex++).toStdString().c_str());
    temp->m_weldRecipe_SC.RecipeVerNumber = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.WeldMode = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ModeValue = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.HoldTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.TriggerForce = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.TriggerDistance = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.NumAmplitudeSteps = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepAt = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[0] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[1] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[2] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[3] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[4] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[5] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[6] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[7] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[8] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStepValue[9] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[0] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[1] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[2] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[3] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[4] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[5] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[6] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[7] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[8] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AmplitudeStep[9] = query.at(dwDBIndex++).toInt();

    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceRampTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.TotalCollapseTarget = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForce = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForceRampTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactPosition = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPosition = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.DownAcceleration = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.DownMaxVelocity = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.DownDeceleration = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnAcceleration = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnMaxVelocity = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnDeceleration = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.NumForceSteps = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.NumForceSteps = temp->m_weldRecipe_SC.NumForceSteps;
    temp->m_weldRecipe_SC.ForceStepAt = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[0] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[1] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[2] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[3] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[4] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[5] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[6] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[7] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[8] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ForceStepValue[9] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[0] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[1] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[2] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[3] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[4] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[5] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[6] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[7] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[8] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[9] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[0] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[1] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[2] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[3] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[4] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[5] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[6] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[7] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[8] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[9] = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AfterBurst.bIsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AfterBurst.BurstDelay = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AfterBurst.BurstTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.AfterBurst.BurstAmplitude = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.preTrigger.bIsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.preTrigger.bIsAutoEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.preTrigger.bIsDistanceEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.preTrigger.TriggerAmplitude = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.preTrigger.PreTriggerDistance = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.EnergyBrake.bIsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.EnergyBrake.EnergyBrakeTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.EnergyBrake.EnergyBrakeAmplitude = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.bIsCoolingValeEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.MaxWeldTimeout = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.bIsTimedSeekEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.bIsPreWeldSeekEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.bIsPostWeldSeekEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.TimedSeekPeriod = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowMinus = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowPlus = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactOffset = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.bIsControlLimitEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.Value = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.IsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.FrequencyLow.Value = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.FrequencyLow.IsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.IsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.FrequencyHigh.Value = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.FrequencyHigh.IsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.CollapseCutoff.Value = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.CollapseCutoff.IsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.EnergyCutoff.Value = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.EnergyCutoff.IsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.TimeCutoff.Value = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.TimeCutoff.IsEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ControlLimits.GroundDetectCutoff = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_UI.isActive=query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_UI.isLock=query.at(dwDBIndex++).toInt();
    strcpy(temp->m_weldRecipe_UI.CompanyName,query.at(dwDBIndex++).toStdString().c_str());
    temp->m_weldRecipe_UI.isValid=query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPositionToggle = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForceControl = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.DynamicModeParam.Reactivity = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.DynamicModeParam.IsForceLevelEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.DynamicModeParam.ForceLevel = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.DynamicModeParam.ForceLevelTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.ScrubAmplitude = query.at(dwDBIndex++).toInt();
    // Suspect reject params
    dwDBIndex+=2;
    temp->m_weldRecipe_SC.IsGlobalSuspectEnabled = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_SC.IsGlobalRejectEnabled = query.at(dwDBIndex++).toInt();

    QString qstrLimits = query.at(dwDBIndex++);
    QStringList limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.Time.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.Energy.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.High = limits.at(9).toInt();

    //Branson Only Params
    dwDBIndex+=2;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC1 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC2 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoop = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoopCF = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyLow = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyHigh = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimitTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimit = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.ControlMode = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.BlindtimeSeek = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Blindtimeweld = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.DDSSwitchtime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter7 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter8 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter9 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter10 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter11 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter12 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter13 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter14 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter15 = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FLimitTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpProportionalGain = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpIntegralGain = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseIntegralGain = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseProportionalGain = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyWindowSize = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PFofPhasecontrolloop = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyLow = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyHigh = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimitTime = query.at(dwDBIndex++).toInt();
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimit = query.at(dwDBIndex++).toInt();
    /* Weld ramp time was moved to param AtoZ so restoring it form Branson only params table should be avoided */
    //temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime = query.at(dwDBIndex++).toInt();
    dwDBIndex++;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.StartFrequency = query.at(dwDBIndex++).toInt();

    dwDBIndex+=2;

    temp->m_weldRecipe_UI.SetupLimitsst.isGlobalSetup  = query.at(dwDBIndex++).toInt();
    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldModeStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldModeLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldModeHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldAmpStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldAmpLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldAmpHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldForceStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldForceLow  =  limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldForceHigh  =  limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwTriggerForceStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwTriggerForceLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwTriggerForceHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldForceStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldForceLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldForceHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldTimeStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldTimeLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldTimeHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubAmpStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubAmpLow = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubAmpHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubTimeStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubTimeLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubTimeHigh = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwReactivityStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwReactivityLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwReactivityHigh = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwForceLevelStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwForceLevelLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwForceLevelHigh = limits.at(2).toInt();
//  there is not any data fields for the DigtialTune, FrequencyOffset and Memoroffset so we commented out these code.
//    dwDBIndex++;
//    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.DigitalTune = query.at(dwDBIndex++).toInt();
//    dwDBIndex++;
//    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyOffset = query.at(dwDBIndex++).toInt();
//    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.MemoryOffset = 0;

}

/**************************************************************************//**
*
* \brief Separate the recived buffer to the General Data list, Suspect/Reject Data list, Branson Only Data list and Setup Data list.
*        If there are four data lists are not full data list after the detected, it will return SIZE_ERROR.
*        If there is any error during the data conversion from string to integer, it will return DATA_ERROR.
*        If there is any messy code in the Company Name, it will return STRING_ERROR.
*        If there is any issue on the upper limit of the suspect and reject variables, such as the data is more than default upper limit,
*        or the data is equal to ZERO, it will return RANGE_ERROR.
*
* \param received buffer from SC
*
* \return Error Code.
*
******************************************************************************/
int WeldRecipeParameter::parseWeldRecipeParameterFromDB(QString strRecivedBuffer)
{
    unsigned char ErrorCode = 0;
    strRecivedBuffer.replace("'","");
    QStringList query = strRecivedBuffer.split(",");
    if(query.size() < WELD_RECIPE_GENERAL_DATA_SIZE)
    {
        ErrorCode = SIZE_ERROR;
        while(query.size() < WELD_RECIPE_GENERAL_DATA_SIZE)
            query.append("0");
    }
    //Need to do best to parse the recipe general attribute information
    ErrorCode |= parseWeldRecipeGeneralDataFromDB(query);
    if(ErrorCode != 0)
        return ErrorCode;

    QStringList WeldRecipeGeneralDataList;
    QStringList WeldRecipeSuspectRejectDataList;
    QStringList WeldRecipeBransonOnlyDataList;
    QStringList WeldRecipeSetupDataList;
    QStringList WeldRecipeStackDataList;
    // To separate received buff
    QRegularExpression SuspectRejectRegExp("^\\d+[:]\\d+[:]\\d+[:]\\d+[:]\\d+[:]\\d+[:]\\d+[:]\\d+[:]\\d+[:]\\d+$");
    QRegularExpression SetupRegExp("^\\d+[:]\\d+[:]\\d+$");
    int SuspectRejectIndex = query.indexOf(SuspectRejectRegExp, 0);
    if(SuspectRejectIndex == -1)
        return SIZE_ERROR;
    WeldRecipeSuspectRejectDataList = query.filter(SuspectRejectRegExp);
    int SetupIndex = query.indexOf(SetupRegExp);
    if(SetupIndex == -1)
        return SIZE_ERROR;
    WeldRecipeSetupDataList = query.filter(SetupRegExp);
    for(int i = 0; i < SuspectRejectIndex; i++)
        WeldRecipeGeneralDataList.append(query[i]);

    for(int i = (SuspectRejectIndex + WeldRecipeSuspectRejectDataList.size()); i < SetupIndex; i++)
        WeldRecipeBransonOnlyDataList.append(query[i]);

//  There is not any data fields for the DigtialTune, FrequencyOffset and Memoroffset so we commented out these code.
//    for(int i = SetupIndex + WeldRecipeSetupDataList.size(); i < query.size(); i++)
//        WeldRecipeStackDataList.append(query[i]);

//    while(WeldRecipeGeneralDataList.size() < WELD_RECIPE_GENERAL_DATA_SIZE)
//        WeldRecipeGeneralDataList.append("0");
//    while(WeldRecipeSuspectRejectDataList.size() < WELD_RECIPE_SUSPECT_REJECT_DATA_SIZE)
//        WeldRecipeSuspectRejectDataList.append(SRLimitDefaultVal);
//    while(WeldRecipeBransonOnlyDataList.size() < WELD_RECIPE_BRANSON_ONLY_DATA_SIZE)
//        WeldRecipeBransonOnlyDataList.append("0");
//    while(WeldRecipeSetupDataList.size() < WELD_RECIPE_SETUP_DATA_SIZE)
//        WeldRecipeSetupDataList.append(SetupLimitDefaultVal);
    if(WeldRecipeGeneralDataList.size() < WELD_RECIPE_GENERAL_DATA_SIZE)
        return SIZE_ERROR;
    if(WeldRecipeSuspectRejectDataList.size() < WELD_RECIPE_SUSPECT_REJECT_DATA_SIZE)
        return SIZE_ERROR;
    if(WeldRecipeBransonOnlyDataList.size() < WELD_RECIPE_BRANSON_ONLY_DATA_SIZE)
        return SIZE_ERROR;
    if(WeldRecipeSetupDataList.size() < WELD_RECIPE_SETUP_DATA_SIZE)
        return SIZE_ERROR;
//  There is not any data fields for the DigtialTune, FrequencyOffset and Memoroffset so we commented out these code.
//    if(WeldRecipeStackDataList.size() < WELD_RECIPE_STACK_DATA_SIZE)
//        return SIZE_ERROR;

    parseWeldRecipeSuspectRejectDataFromDB(WeldRecipeSuspectRejectDataList);
    if(parseWeldRecipeBransonOnlyDataFromDB(WeldRecipeBransonOnlyDataList) != 0)
        return DATA_ERROR;
    parseWeldRecipeSetupDataFromDB(WeldRecipeSetupDataList);
//  There is not any data fields for the DigtialTune, FrequencyOffset and Memoroffset so we commented out these code.
//    if(parseWeldRecipeStackDataFromDB(WeldRecipeStackDataList))
//        return DATA_ERROR;

    ErrorCode |= parseWeldRecipeCompanyName();
    ErrorCode |= parseWeldRecipeSuspectRejectUpperLimit();

    return ErrorCode;
}

/**************************************************************************//**
*
* \brief Parse the received data to the weld recipe general variables.
*        If there is any error during the data conversion from string to integer, it will return DATA_ERROR.
*
* \param General Data list
*
* \return Error Code.
*
******************************************************************************/
int WeldRecipeParameter::parseWeldRecipeGeneralDataFromDB(QStringList query)
{
    int dwDBIndex = 0;
    bool bResult = true;
    WeldRecipe *temp = &m_weldRecipe;
    temp->m_weldRecipe_SC.RecipeNumber = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    if(temp->m_weldRecipe_SC.RecipeNumber > 999 || temp->m_weldRecipe_SC.RecipeNumber < 0)
        temp->m_weldRecipe_SC.RecipeNumber = 0;
    memset(temp->m_weldRecipe_UI.RecipeName, 0, sizeof(temp->m_weldRecipe_UI.RecipeName));
    strcpy(temp->m_weldRecipe_UI.RecipeName,query.at(dwDBIndex++).toStdString().c_str());
    temp->m_weldRecipe_SC.RecipeVerNumber = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.WeldMode = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ModeValue = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.HoldTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.TriggerForce = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.TriggerDistance = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.NumAmplitudeSteps = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.AmplitudeStepAt = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    for(int i = 0; i < 10; i++)
    {
        temp->m_weldRecipe_SC.AmplitudeStepValue[i] = query.at(dwDBIndex++).toInt(&bResult, 10);
        if(bResult == false)
            break;
    }
    if(bResult == false)
        return DATA_ERROR;

    for(int i = 0; i < 10; i++)
    {
        temp->m_weldRecipe_SC.AmplitudeStep[i] = query.at(dwDBIndex++).toInt(&bResult, 10);
        if(bResult == false)
            break;
    }
    if(bResult == false)
        return DATA_ERROR;

    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceRampTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.TotalCollapseTarget = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForce = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForceRampTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactPosition = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPosition = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.DownAcceleration = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.DownMaxVelocity = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.DownDeceleration = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnAcceleration = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnMaxVelocity = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnDeceleration = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.NumForceSteps = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.NumForceSteps = temp->m_weldRecipe_SC.NumForceSteps;

    temp->m_weldRecipe_SC.ForceStepAt = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;

    for(int i = 0; i < 10; i++)
    {
        temp->m_weldRecipe_SC.ForceStepValue[i] = query.at(dwDBIndex++).toInt(&bResult, 10);
        if(bResult == false)
            break;
    }
    if(bResult == false)
        return DATA_ERROR;

    for(int i = 0; i < 10; i++)
    {
        temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[i] = query.at(dwDBIndex++).toInt(&bResult, 10);
        if(bResult == false)
            break;
    }
    if(bResult == false)
        return DATA_ERROR;

    for(int i = 0; i < 10; i++)
    {
        temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[i] = query.at(dwDBIndex++).toInt(&bResult, 10);
        if(bResult == false)
            break;
    }
    if(bResult == false)
        return DATA_ERROR;

    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.AfterBurst.bIsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.AfterBurst.BurstDelay = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.AfterBurst.BurstTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.AfterBurst.BurstAmplitude = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.preTrigger.bIsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.preTrigger.bIsAutoEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.preTrigger.bIsDistanceEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.preTrigger.TriggerAmplitude = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.preTrigger.PreTriggerDistance = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.EnergyBrake.bIsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.EnergyBrake.EnergyBrakeTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.EnergyBrake.EnergyBrakeAmplitude = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.bIsCoolingValeEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.MaxWeldTimeout = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.bIsTimedSeekEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.bIsPreWeldSeekEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.bIsPostWeldSeekEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.TimedSeekPeriod = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowMinus = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowPlus = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactOffset = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.bIsControlLimitEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.Value = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.IsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.FrequencyLow.Value = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.FrequencyLow.IsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.IsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.FrequencyHigh.Value = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.FrequencyHigh.IsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.CollapseCutoff.Value = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.CollapseCutoff.IsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.EnergyCutoff.Value = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.EnergyCutoff.IsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.TimeCutoff.Value = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.TimeCutoff.IsEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ControlLimits.GroundDetectCutoff = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_UI.isActive=query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_UI.isLock=query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    memset(temp->m_weldRecipe_UI.CompanyName, 0, sizeof(temp->m_weldRecipe_UI.CompanyName));
    strcpy(temp->m_weldRecipe_UI.CompanyName,query.at(dwDBIndex++).toStdString().c_str());

    temp->m_weldRecipe_UI.isValid=query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPositionToggle = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForceControl = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.DynamicModeParam.Reactivity = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.DynamicModeParam.IsForceLevelEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.DynamicModeParam.ForceLevel = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.DynamicModeParam.ForceLevelTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.ScrubAmplitude = query.at(dwDBIndex++).toInt(&bResult, 10); //124
    if(bResult == false)
        return DATA_ERROR;
    dwDBIndex+=2;

    temp->m_weldRecipe_SC.IsGlobalSuspectEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_SC.IsGlobalRejectEnabled = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    return 0;
}

/**************************************************************************//**
*
* \brief Parse the Suspect Reject data structure from Suspect/Reject Data list
*
* \param Suspect/Reject Data list
*
* \return none.
*
******************************************************************************/
void WeldRecipeParameter::parseWeldRecipeSuspectRejectDataFromDB(QStringList query)
{
    int dwDBIndex=0;
    WeldRecipe *temp = &m_weldRecipe;
    // Suspect reject params
    QString qstrLimits = query.at(dwDBIndex++);
    QStringList limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.Time.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Time.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Time.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.Energy.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Energy.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Energy.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.PeakPower.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.PeakPower.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.High = limits.at(9).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SRLimitCnt)
    {
        QString temp = SRLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.IsEnabled = limits.at(0).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.IsLowEnabled = limits.at(1).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.IsHighEnabled = limits.at(2).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.Low = limits.at(3).toInt();
    temp->m_weldRecipe_SC.SuspectLimit.Frequency.High = limits.at(4).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.IsEnabled = limits.at(5).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.IsLowEnabled = limits.at(6).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.IsHighEnabled = limits.at(7).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.Low = limits.at(8).toInt();
    temp->m_weldRecipe_SC.RejectLimit.Frequency.High = limits.at(9).toInt();
}

/**************************************************************************//**
*
* \brief Parse the received data to the weld recipe Branson Only variables.
*        If there is any error during the data conversion from string to integer, it will return DATA_ERROR.
*
* \param Branson Only list
*
* \return Error Code.
*
******************************************************************************/
int WeldRecipeParameter::parseWeldRecipeBransonOnlyDataFromDB(QStringList query)
{
    int dwDBIndex=0;
    bool bResult = true;
    WeldRecipe *temp = &m_weldRecipe;
    //Branson Only Params
    dwDBIndex+=2;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC1 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC2 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoop = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoopCF = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyLow = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyHigh = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimitTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimit = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.ControlMode = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.BlindtimeSeek = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Blindtimeweld = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.DDSSwitchtime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter7 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter8 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter9 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter10 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter11 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter12 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter13 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter14 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter15 = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FLimitTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpProportionalGain = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpIntegralGain = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseIntegralGain = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseProportionalGain = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyWindowSize = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PFofPhasecontrolloop = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyLow = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyHigh = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimitTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimit = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    /* Weld ramp time was moved to param AtoZ so restoring it form Branson only params table should be avoided */
    //temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime = query.at(dwDBIndex++).toInt(&bResult, 10);
    dwDBIndex++;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.StartFrequency = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    dwDBIndex += 2;

    temp->m_weldRecipe_UI.SetupLimitsst.isGlobalSetup  = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    return 0;
}

/**************************************************************************//**
*
* \brief Parse the Setup data structure from Setup Data list
*
* \param Setup Data list
*
* \return none
*
******************************************************************************/
void WeldRecipeParameter::parseWeldRecipeSetupDataFromDB(QStringList query)
{
    int dwDBIndex=0;
    WeldRecipe *temp = &m_weldRecipe;
    // Suspect reject params
    QString qstrLimits = query.at(dwDBIndex++);
    QStringList limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldModeStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldModeLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldModeHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldAmpStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldAmpLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldAmpHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldForceStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldForceLow  =  limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwWeldForceHigh  =  limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwTriggerForceStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwTriggerForceLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwTriggerForceHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldForceStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldForceLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldForceHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldTimeStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldTimeLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwHoldTimeHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubAmpStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubAmpLow = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubAmpHigh  = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubTimeStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubTimeLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwScrubTimeHigh = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwReactivityStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwReactivityLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwReactivityHigh = limits.at(2).toInt();

    qstrLimits = query.at(dwDBIndex++);
    limits.clear();
    limits = qstrLimits.split(":");
    if(limits.size() != SetupLimitCnt)
    {
        QString temp = SetupLimitDefaultVal;
        limits = temp.split(":");
    }

    temp->m_weldRecipe_UI.SetupLimitsst.dwForceLevelStatus  = limits.at(0).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwForceLevelLow  = limits.at(1).toInt();
    temp->m_weldRecipe_UI.SetupLimitsst.dwForceLevelHigh = limits.at(2).toInt();
}

/**************************************************************************//**
*
* \brief Parse the Stack data structure from Stack Data list
*
* \param Stack Data list
*
* \return none
*
******************************************************************************/
int WeldRecipeParameter::parseWeldRecipeStackDataFromDB(QStringList query)
{
    int dwDBIndex=0;
    bool bResult = true;
    WeldRecipe *temp = &m_weldRecipe;
    //Stack Only Params
    dwDBIndex++;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.DigitalTune = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    dwDBIndex++;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyOffset = query.at(dwDBIndex++).toInt(&bResult, 10);
    if(bResult == false)
        return DATA_ERROR;
    temp->m_weldRecipe_PC_AC.m_weldRecipe_PC.MemoryOffset = 0;
    return 0;
}

/**************************************************************************//**
*
* \brief If there is any messy code in the Company Name, it will return STRING_ERROR.
*
* \param none
*
* \return Error Code
*
******************************************************************************/
int WeldRecipeParameter::parseWeldRecipeCompanyName()
{
    WeldRecipe *temp = &m_weldRecipe;
    QRegularExpression RegExp("^[a-zA-Z0-9]*$");
    QString strCompanyName(temp->m_weldRecipe_UI.CompanyName);
    if((strCompanyName.contains(RegExp) == false) && (strCompanyName != ""))
    {
        strCompanyName.clear();
        strCompanyName = "NewCompany";
        memset(temp->m_weldRecipe_UI.CompanyName, 0, sizeof(temp->m_weldRecipe_UI.CompanyName));
        strcpy(temp->m_weldRecipe_UI.CompanyName, strCompanyName.toStdString().c_str());
        return STRING_ERROR;
    }
    else
        return 0;
}

/**************************************************************************//**
*
* \brief If there is any issue on the upper limit of the suspect and reject variables, such as the data is more than default upper limit,
*        or the data is equal to ZERO, it will return RANGE_ERROR.
*
* \param none
*
* \return Error Code
*
******************************************************************************/
int WeldRecipeParameter::parseWeldRecipeSuspectRejectUpperLimit()
{
    unsigned char ErrorCode = 0;
    WeldRecipe *temp = &m_weldRecipe;
    unsigned int upperValue = 0;
    upperValue = getDefaultVariableValue("suspect time high value", getDefaultValue("suspect time high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.Time.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.Time.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.Time.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject time high value", getDefaultValue("reject time high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.Time.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.Time.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.Time.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    upperValue= getDefaultVariableValue("suspect energy high value", getDefaultValue("suspect energy high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.Energy.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.Energy.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.Energy.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject energy high value", getDefaultValue("reject energy high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.Energy.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.Energy.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.Energy.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    upperValue= getDefaultVariableValue("suspect peak power high value", getDefaultValue("suspect peak power high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.PeakPower.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.PeakPower.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.PeakPower.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject peak power high value", getDefaultValue("reject peak power high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.PeakPower.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.PeakPower.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.PeakPower.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    upperValue= getDefaultVariableValue("suspect absolute distance high value", getDefaultValue("suspect absolute distance high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject absolute distance high value", getDefaultValue("reject absolute distance high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    upperValue= getDefaultVariableValue("suspect collapse distance high value", getDefaultValue("suspect collapse distance high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.CollapseDistance.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject collapse distance high value", getDefaultValue("reject collapse distance high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.CollapseDistance.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    upperValue= getDefaultVariableValue("suspect trigger distance high value", getDefaultValue("suspect trigger distance high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.TriggerDistance.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject trigger distance high value", getDefaultValue("reject trigger distance high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.TriggerDistance.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    upperValue= getDefaultVariableValue("suspect end weld force high value", getDefaultValue("suspect end weld force high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.EndWeldForce.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject end weld force high value", getDefaultValue("reject end weld force high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.EndWeldForce.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    upperValue= getDefaultVariableValue("suspect frequency high value", getDefaultValue("suspect frequency high value"));
    if((temp->m_weldRecipe_SC.SuspectLimit.Frequency.High > upperValue) || (temp->m_weldRecipe_SC.SuspectLimit.Frequency.High == 0))
    {
        temp->m_weldRecipe_SC.SuspectLimit.Frequency.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }
    upperValue = getDefaultVariableValue("reject frequency high value", getDefaultValue("reject frequency high value"));
    if((temp->m_weldRecipe_SC.RejectLimit.Frequency.High > upperValue) || (temp->m_weldRecipe_SC.RejectLimit.Frequency.High == 0))
    {
        temp->m_weldRecipe_SC.RejectLimit.Frequency.High = upperValue;
        ErrorCode = RANGE_ERROR;
    }

    return ErrorCode;
}

/**
 * @brief WeldRecipeParameter::prepareWeldRecipeStringToDB : Converting WeldRecipe parameter to String
 * @return
 */
QString WeldRecipeParameter::prepareWeldRecipeStringToDB()
{
    QString paramstring="";
    if(m_weldRecipe.m_weldRecipe_SC.RecipeNumber != 0)
    {
//        m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber++;
        if(m_weldRecipe.m_weldRecipe_UI.isValid)
            m_weldRecipe.m_weldRecipe_UI.isValid = false;
    }
    int oldRecipe;
    if(m_oldRecipeNumber != 0)
    {
        oldRecipe = m_oldRecipeNumber;
    }
    else
        oldRecipe = m_weldRecipe.m_weldRecipe_SC.RecipeNumber;
    paramstring=
            QString::number(oldRecipe)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeNumber)+","+"'"
            +QString::fromStdString(m_weldRecipe.m_weldRecipe_UI.RecipeName)+"',"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.WeldMode)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ModeValue)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.HoldTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.TriggerForce)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.TriggerDistance)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.NumAmplitudeSteps)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepAt)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[0])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[1])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[2])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[3])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[4])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[5])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[6])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[7])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[8])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStepValue[9])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[0])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[1])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[2])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[3])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[4])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[5])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[6])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[7])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[8])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AmplitudeStep[9])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceRampTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.TotalCollapseTarget)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForce)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.HoldForceRampTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactPosition)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPosition)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownAcceleration)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownMaxVelocity)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.DownDeceleration)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnAcceleration)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnMaxVelocity)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReturnDeceleration)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.NumForceSteps)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepAt)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[0])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[1])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[2])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[3])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[4])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[5])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[6])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[7])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[8])+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ForceStepValue[9])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[0])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[1])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[2])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[3])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[4])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[5])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[6])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[7])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[8])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStepRampValue[9])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[0])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[1])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[2])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[3])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[4])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[5])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[6])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[7])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[8])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ForceStep[9])+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.bIsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstDelay)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.AfterBurst.BurstAmplitude)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsAutoEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.bIsDistanceEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.TriggerAmplitude)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.preTrigger.PreTriggerDistance)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.EnergyBrake.bIsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.EnergyBrake.EnergyBrakeTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.EnergyBrake.EnergyBrakeAmplitude)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.bIsCoolingValeEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.MaxWeldTimeout)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.bIsTimedSeekEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.bIsPreWeldSeekEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.bIsPostWeldSeekEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.TimedSeekPeriod)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowMinus)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.PartContactWindowPlus)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ExpectedPartContactOffset)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.bIsControlLimitEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.Value)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.PeakPowerCutoff.IsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyLow.Value)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyLow.IsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.Value)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.AbsoluteCutoff.IsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyHigh.Value)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.FrequencyHigh.IsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.CollapseCutoff.Value)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.CollapseCutoff.IsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.EnergyCutoff.Value)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.EnergyCutoff.IsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.TimeCutoff.Value)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.TimeCutoff.IsEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ControlLimits.GroundDetectCutoff)+","
            +QString::number(m_weldRecipe.m_weldRecipe_UI.isActive)+","
            +QString::number(m_weldRecipe.m_weldRecipe_UI.isLock)+","+"'"
            +m_weldRecipe.m_weldRecipe_UI.CompanyName +"'"+","
            +QString::number(m_weldRecipe.m_weldRecipe_UI.isValid)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.ReadyPositionToggle)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForceControl)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.Reactivity)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.IsForceLevelEnabled)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.ForceLevel)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.DynamicModeParam.ForceLevelTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.ScrubAmplitude);
    return paramstring;
}

/**
 * @brief WeldRecipeParameter::prepareSuspectRejectStringToDB : Converting suspect and reject parameter to string
 * @return
 */
QString WeldRecipeParameter::prepareSuspectRejectStringToDB()
{
    QString prepareString = "";
    int oldRecipe;
    if(m_oldRecipeNumber != 0)
        oldRecipe = m_oldRecipeNumber;
    else
        oldRecipe = m_weldRecipe.m_weldRecipe_SC.RecipeNumber;
    prepareString =
            QString::number(oldRecipe)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeNumber)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber) + ","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.IsGlobalSuspectEnabled) + ","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.IsGlobalRejectEnabled) + ",'"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Time.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Time.High)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Energy.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Energy.High)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.PeakPower.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.PeakPower.High)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.AbsoluteDistance.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.AbsoluteDistance.High)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.CollapseDistance.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.CollapseDistance.High)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.TriggerDistance.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.TriggerDistance.High)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.EndWeldForce.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.EndWeldForce.High)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.SuspectLimit.Frequency.High)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsLowEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.IsHighEnabled)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.Low)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RejectLimit.Frequency.High)+"'";

    return prepareString;
}

/**
 * @brief WeldRecipeParameter::prepareBransonOnlyParamsStringToDB : Converting BransonOnlyParams String ToDB
 * @return
 */
QString WeldRecipeParameter::prepareBransonOnlyParamsStringToDB()
{
    QString tempStrng = "";

    int oldRecipe;
    if(m_oldRecipeNumber != 0)
        oldRecipe = m_oldRecipeNumber;
    else
        oldRecipe = m_weldRecipe.m_weldRecipe_SC.RecipeNumber;
    tempStrng =
            QString::number(oldRecipe)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeNumber)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber) + ","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC1)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmplitudeLoopC2)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoop)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLoopCF)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyLow)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyHigh)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimitTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseLimit)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.ControlMode)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.BlindtimeSeek)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Blindtimeweld)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.DDSSwitchtime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter7)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter8)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter9)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter10)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter11)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter12)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter13)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter14)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.Parameter15)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FLimitTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpProportionalGain)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.AmpIntegralGain)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseIntegralGain)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PhaseProportionalGain)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.FrequencyWindowSize)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PFofPhasecontrolloop)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyLow)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIFrequencyHigh)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimitTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.PIPhaseLimit)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.WeldRampTime)+","
            +QString::number(m_weldRecipe.m_weldRecipe_PC_AC.m_weldRecipe_PC.StartFrequency);
    return tempStrng;
}

/**
 * @brief WeldRecipeParameter::prepareSetupLimitsStringToDB : PrepareSetupLimitsStringToDB
 * @return
 */
QString WeldRecipeParameter::prepareSetupLimitsStringToDB()
{
    QString tempStrng = "";
    int oldRecipe;
    if(m_oldRecipeNumber != 0)
        oldRecipe = m_oldRecipeNumber;
    else
        oldRecipe = m_weldRecipe.m_weldRecipe_SC.RecipeNumber;
    tempStrng =
            QString::number(oldRecipe)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeNumber)+","
            +QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber) + ","
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.isGlobalSetup)+",'"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldModeHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldAmpHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwWeldForceHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwTriggerForceHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldForceHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwHoldTimeHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubAmpHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwScrubTimeHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwReactivityHigh)+"','"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelStatus)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelLow)+":"
            +QString::number(m_weldRecipe.m_weldRecipe_UI.SetupLimitsst.dwForceLevelHigh)+"'";

    return tempStrng;
}

/**
 * @brief WeldRecipeParameter::prepareStackRecipeStringToDB : Converting Stack Recipe parameter to string
 * @return QString tempStrng: StackRecipe parameters on a string separated by comas
 */
QString WeldRecipeParameter :: prepareStackRecipeStringToDB()
{
   StackRecipeParameter* tmpRecipeParam ;
   QString tempStrng = "";

    /* Get the stack recipe parameters */
    INT32 a_DigitalTune;
    INT32 a_InternalOffsetFlag;
    INT32 a_InternalFreqOffset ;
    INT32 a_EndofWeldStore;

    if(m_weldRecipe.m_weldRecipe_UI.isNewRecipe == true)
    	obj_stackrecipedata->resetStackRecipeData();
    
    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(obj_stackrecipedata->m_StackRecipeParamList.at(0));
    a_DigitalTune = (INT32)tmpRecipeParam->getParameterValue();

    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(obj_stackrecipedata->m_StackRecipeParamList.at(1));
     a_InternalOffsetFlag = (INT32)tmpRecipeParam->getParameterValue();

    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(obj_stackrecipedata->m_StackRecipeParamList.at(2));
    a_InternalFreqOffset = (INT32)tmpRecipeParam->getParameterValue();

    tmpRecipeParam = dynamic_cast<StackRecipeParameter*>(obj_stackrecipedata->m_StackRecipeParamList.at(3));
    a_EndofWeldStore = (INT32)tmpRecipeParam->getParameterValue();

    /* Generate the STACK recipe msg to DB */
    tempStrng = QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeNumber)+","
            + QString :: number(a_DigitalTune) + ","
            + QString :: number(a_InternalOffsetFlag) + ","
            + QString :: number(a_InternalFreqOffset) +","
            + QString :: number(a_EndofWeldStore);
    qDebug()<<"Stack parameters To SC as following...";
    qDebug()<<"DigitalTune:"<<a_DigitalTune;
    qDebug()<<"Internal Offset Flag:"<<a_InternalOffsetFlag;
    qDebug()<<"End Of Weld Store:"<<a_EndofWeldStore;
    return tempStrng ;
}


/**
 * @brief WeldRecipeParameter::rangeValidation : Range Validation from minimum to maximum
 * @param keypadTextName
 * @param keypadValue
 * @return
 */
QString WeldRecipeParameter::rangeValidation(QString keypadTextName,QString keypadValue)
{
    if(!m_MapRecipeParams.contains(keypadTextName.toLower()))
        return keypadValue;

    stRecipeParmInfo stTemp = m_MapRecipeParams[keypadTextName.toLower()];
    double currentValue = keypadValue.toDouble();
    double minValue = getMinValue(keypadTextName).toDouble();
    double maxValue = getMaxValue(keypadTextName).toDouble();

    if(stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UINTSEC &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UINTMilliMeter &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_MMPERSECOND &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_MMPERSECONDSQU &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UINTINCHES &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UINIINSEC &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UINIINSECSQU &&
			stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UINTJOULES &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UINTLBS &&
            stTemp.ParamUnit[m_UnitSelected] != CUST_NAME_UNIT_NEWTON)
    {
        QString sample = keypadValue;
        for(int index = 0;index < sample.length();index++)
        {
            if(sample[index]=='.')
            {
                if((keypadTextName.toLower() == "weld force") && bIsWeldForceChanged == true)
                {
                    return getActuatorRecipeStructureValue();
                }
                return getStructureValueByName(keypadTextName);
            }
        }
    }

    if(currentValue < minValue)
    {
        if((keypadTextName.toLower() == "weld force") && bIsWeldForceChanged == true)
        {
            return getActuatorRecipeStructureValue();
        }
        return getStructureValueByName(keypadTextName);
    }
    else if(currentValue > maxValue)
    {
        if((keypadTextName.toLower() == "weld force") && bIsWeldForceChanged == true)
        {
            return getActuatorRecipeStructureValue();
        }
        return getStructureValueByName(keypadTextName);
    }
    else
    {
        return keypadValue;
    }
}

/**
 * @brief WeldRecipeParameter::rangeSetupLimitValidation
 * @param keypadTextName
 * @param keypadValue
 * @return
 */
QString WeldRecipeParameter::rangeSetupLimitValidation(QString keypadTextName,QString keypadValue)
{
    stRecipeParmInfo stTemp = m_MapRecipeParams["setup "+keypadTextName.toLower()+" low"];
    double currentValue = keypadValue.toDouble();
    double minValue = getStructureValueByName("setup "+keypadTextName.toLower()+" low").toDouble();
    double maxValue = getStructureValueByName("setup "+keypadTextName.toLower()+" high").toDouble();
    if(keypadTextName.toLower() == "weld mode")
    {
        int mode;
        mode = getStructureValueByIndex(SC_WELD_MODE).toInt();
        ModeInfo stModeInfo = getModeInfoByIndex(mode,"");
        keypadTextName = stModeInfo.ModeName;
        stTemp.ParamUnit[m_UnitSelected] = stModeInfo.ModeUnit;
    }

    if(stTemp.ParamUnit[m_UnitSelected] != "s" &&
            stTemp.ParamUnit[m_UnitSelected] != "mm" &&
            stTemp.ParamUnit[m_UnitSelected] != "mm/s" &&
            stTemp.ParamUnit[m_UnitSelected] != "mm/s2" &&
			stTemp.ParamUnit[m_UnitSelected] != "J" &&
            stTemp.ParamUnit[m_UnitSelected] != "in" &&
            stTemp.ParamUnit[m_UnitSelected] != "in/s" &&
            stTemp.ParamUnit[m_UnitSelected] != "in/s2" &&
            stTemp.ParamUnit[m_UnitSelected] != "lbs" &&
            stTemp.ParamUnit[m_UnitSelected] != "N")
    {
        QString sample = keypadValue;
        for(int index = 0;index < sample.length();index++)
        {
            if(sample[index]=='.')
            {
                return getStructureValueByName(keypadTextName);
            }
        }
    }

    if(currentValue < minValue)
    {
        return getStructureValueByName(keypadTextName);
    }
    else if(currentValue > maxValue)
    {
        return getStructureValueByName(keypadTextName);
    }
    else
    {
        return keypadValue;
    }
}

/**
 * @brief WeldRecipeParameter::getModeInfoByIndex
 * @param dwModeIndex
 * @param qstrModeValue
 * @return
 */
ModeInfo WeldRecipeParameter::getModeInfoByIndex(int dwModeIndex,QString qstrModeValue)
{
    ModeInfo stModeInfo;
    stModeInfo.ModeIndex = dwModeIndex;
    switch(dwModeIndex)
    {
    case 1:
        stModeInfo.ModeName = WELD_MODE_TIME;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_TIME;
        stModeInfo.ModeNameScreen = CUST_NAME_TIME;
        stModeInfo.ModeUnit = "s";
        double time;
        time = qstrModeValue.toDouble();
        time = convertMsecToSec(time);
        stModeInfo.ModeValue.setNum(time,'f',3);
        break;
    case 2:
        stModeInfo.ModeName = WELD_MODE_ENERGY;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_ENERGY;
        stModeInfo.ModeNameScreen = CUST_NAME_ENERGY;
        stModeInfo.ModeUnit = "J";
        stModeInfo.ModeValue = qstrModeValue;
        break;
    case 3:
        stModeInfo.ModeName = WELD_MODE_PEAK_POWER;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_PEAK_POWER;
        stModeInfo.ModeNameScreen = CUST_NAME_PEAK;
        stModeInfo.ModeUnit = "W";
        stModeInfo.ModeValue = qstrModeValue;
        break;
    case 4:
        stModeInfo.ModeName = WELD_MODE_GND_DET_VAL;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_ST;
        stModeInfo.ModeNameScreen = CUST_NAME_SCRUB;
        stModeInfo.ModeUnit = "s";
        double timeGnd;
        timeGnd = qstrModeValue.toDouble();
        timeGnd = convertMsecToSec(timeGnd);
        stModeInfo.ModeValue.setNum(timeGnd,'f',3);
        break;
    case 5:
        stModeInfo.ModeName = WELD_MODE_ABS_DIS;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_AD;
        stModeInfo.ModeNameScreen = CUST_NAME_ABSOLUTE;
        if(m_UnitSelected == UNIT_SI)
            stModeInfo.ModeUnit = MICROMETRE;
        else if(m_UnitSelected == UNIT_SAE)
            stModeInfo.ModeUnit = INCHES;
        break;
    case 6:
        stModeInfo.ModeName = WELD_MODE_COL_DIS;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_CD;
        stModeInfo.ModeNameScreen = CUST_NAME_COLLAPSE;
        if(m_UnitSelected == UNIT_SI)
            stModeInfo.ModeUnit = MICROMETRE;
        else if(m_UnitSelected == UNIT_SAE)
            stModeInfo.ModeUnit = INCHES;
        break;
    case 7:
        stModeInfo.ModeName = WELD_MODE_DYNAMIC_VAL;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_DENSITY;
        stModeInfo.ModeNameScreen = CUST_NAME_DENSITY;
        stModeInfo.ModeUnit = "%";
        stModeInfo.ModeValue = qstrModeValue;
        break;
    default:
        stModeInfo.ModeName = "Select mode";
        stModeInfo.ModeUnit = "";
        stModeInfo.ModeValue = "";
        break;
    }
    if(stModeInfo.ModeUnit == "mm")
    {
        double dval = convertMicrometreToMillimetre(qstrModeValue.toDouble());
        /*For Collpse Distance and Absolute Distance default value precision*/
        if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            stModeInfo.ModeValue.setNum(dval,'f',2);
        }
        else
        {
            stModeInfo.ModeValue.setNum(dval,'f',3);
        }
    }
    else if(stModeInfo.ModeUnit == "in")
    {
        double dval = convertmicrometretoinches(qstrModeValue.toDouble());
        stModeInfo.ModeValue.setNum(dval,'f',4);
    }
    else if(stModeInfo.ModeUnit == "J")
    {
        double dval = Utility::convertMilliJoulesToJoules(qstrModeValue.toDouble());
        stModeInfo.ModeValue.setNum(dval,'f',1);
    }
    return stModeInfo;
}

/**
 * @brief WeldRecipeParameter::getModeInfoByName : Getting mode information name
 * @param qstrModeName
 * @return
 */
ModeInfo WeldRecipeParameter::getModeInfoByName(QString qstrModeName)
{
    stRecipeParmInfo stTemp = m_MapRecipeParams[qstrModeName.toLower()];
    ModeInfo stModeInfo;
    stModeInfo.ModeName=qstrModeName;
    if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_TIME).toLower())
    {
        stModeInfo.ModeIndex = "1";
        stModeInfo.ModeLocaleName = RECIPE_PARAM_TIME;
    }
    else if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_ENERGY).toLower())
    {
        stModeInfo.ModeIndex = "2";
        stModeInfo.ModeLocaleName = RECIPE_PARAM_ENERGY;
    }
    else if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_PEAK_POWER).toLower())
    {
        stModeInfo.ModeIndex = "3";
        stModeInfo.ModeLocaleName = RECIPE_PARAM_PEAK_POWER;
    }
    else if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_GND_DET).toLower() || qstrModeName.toLower()==QString::fromStdString(WELD_MODE_GND_DET_VAL).toLower())
    {
        stModeInfo.ModeName = WELD_MODE_GND_DET_VAL;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_ST;
        stModeInfo.ModeIndex = "4";
    }
    else if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_ABS_DIS).toLower())
    {
        stModeInfo.ModeIndex = "5";
        stModeInfo.ModeLocaleName = RECIPE_PARAM_AD;
    }
    else if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_COL_DIS).toLower())
    {
        stModeInfo.ModeIndex = "6";
        stModeInfo.ModeLocaleName = RECIPE_PARAM_CD;
    }
    else if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_DYNAMIC).toLower())
    {
        stModeInfo.ModeName = WELD_MODE_DYNAMIC_VAL;
        stModeInfo.ModeLocaleName = RECIPE_PARAM_DENSITY;
        stModeInfo.ModeIndex = "7";
    }
    stModeInfo.ModeUnit = stTemp.ParamUnit[m_UnitSelected];
    stModeInfo.ModeValue = stTemp.ParamDefaultValue[m_CurrentSystemFreq];
    if(qstrModeName.toLower()==QString::fromStdString(WELD_MODE_ABS_DIS).toLower())
    {
        QString qstrTemp = getDefaultValue("absolute distance");
        stModeInfo.ModeValue = qstrTemp;
        if(stModeInfo.ModeUnit == "in")
            stModeInfo.ModeValue = QString::number(convertinchestomillimetre(qstrTemp.toDouble()));
    }
    else
        stModeInfo.ModeValue = stTemp.ParamDefaultValue[m_CurrentSystemFreq];

    return stModeInfo;
}

/**
 * @brief WeldRecipeParameter::setModeInfoByName : Set mode information by name
 * @param qstrModeName
 * @param isMultiMode
 * @param bstate
 */
void WeldRecipeParameter::setModeInfoByName(QString qstrModeName)
{
    ModeInfo stTemp = getModeInfoByName(qstrModeName.toLower());

    int dwVal = stTemp.ModeValue.toInt();

    if (stTemp.ModeUnit=="s" || stTemp.ModeUnit=="mm" || stTemp.ModeUnit == "in")
        dwVal =convertSecToMsec(stTemp.ModeValue.toDouble());
    if (stTemp.ModeUnit=="J")
           dwVal=Utility::convertJoulesToMilliJoules(stTemp.ModeValue.toDouble());
    setStructureVariableByIndex(SC_WELD_MODE,stTemp.ModeIndex.toInt());
    setStructureVariableByIndex(SC_MODE_VALUE,dwVal);
}

void WeldRecipeParameter::sendStructureToSC(int dwStructureIndex)
{
    WeldRecipe *stWeldRecipe = &m_weldRecipe;
    switch(dwStructureIndex)
    {
    case ACTUAL_RECIPE:
        stWeldRecipe = &m_weldRecipe;
        break;
    case ACTUATOR_FORCE_RECIPE:
        stWeldRecipe = &m_weldActuatorForceRecipe;
        break;
    }

    bool bIsChanged = false;
    if(stWeldRecipe->m_weldRecipe_SC.RecipeNumber == 0)
    {
        bIsChanged = true;
        stWeldRecipe->m_weldRecipe_SC.RecipeNumber = stWeldRecipe->m_weldRecipe_SC.RecipeVerNumber;
        stWeldRecipe->m_weldRecipe_SC.RecipeVerNumber = stWeldRecipe->m_weldRecipe_UI.dwRecipeVerNumber;
    }
    stWeldRecipe->m_weldRecipe_SC.bIsRecipeValidated = true;
    int dwVerifyStatus = getStructureValueByIndex(UI_ISVALID).toInt();
    if(login->getUserLevel()== "Operator")
    {
        if(dwVerifyStatus == 1)
            stWeldRecipe->m_weldRecipe_SC.bIsRecipeValidated = true;
        else if(!(((userPrivilageObj->isPermissionAllowed(4, Features::SET_AS_ACTIVE) == true) && (dwVerifyStatus == 1)) ||
             ((userPrivilageObj->isPermissionAllowed(4, Features::RUN_UNVALIDATED_WELD_RECIPE) == true))))
         {
            stWeldRecipe->m_weldRecipe_SC.bIsRecipeValidated = false;
         }
    }
    strcpy( stWeldRecipe->m_weldRecipe_SC.User, PrimaryData->getUsername().toStdString().c_str());
    WeldRecipe_SC stSC_Temp;
    char *buff=reinterpret_cast<char *>(&(stWeldRecipe->m_weldRecipe_SC));
    char *buffTemp=reinterpret_cast<char *>(&(stSC_Temp));
    memcpy(buffTemp,buff,sizeof(stSC_Temp));
    setlimitsFlagsForBL(stSC_Temp);
    /*********************UI To BL*****************************************/
    buff=reinterpret_cast<char *>(&(stSC_Temp));
    CIObj.sendMessage(SCBL_WELDRECIPE_REQ_SC,SCBL_WELDRECIPE_REQ_SC,buff,sizeof(stSC_Temp));

    buff=reinterpret_cast<char *>(&(stWeldRecipe->m_weldRecipe_PC_AC));
    CIObj.sendMessage(SCBL_WELDRECIPE_REQ_PC_AC,SCBL_WELDRECIPE_REQ_PC_AC,buff,sizeof(stWeldRecipe->m_weldRecipe_PC_AC));

    if(bIsChanged)
    {
        stWeldRecipe->m_weldRecipe_SC.RecipeVerNumber = stWeldRecipe->m_weldRecipe_SC.RecipeNumber;
        stWeldRecipe->m_weldRecipe_SC.RecipeNumber = 0;
    }
}

/**
 * @brief WeldRecipeParameter::sendRecipeToSC : Sendng Recipe to BL
 */
void WeldRecipeParameter::sendRecipeToSC()
{
    sendStructureToSC(ACTUAL_RECIPE);
}

/**
 * @brief Recipe::setParamaterChangedFlag
 * @param ParamaterChangedFlag
 */
void WeldRecipeParameter::setParamaterChangedFlag(int ParamaterChangedFlag)
{
    m_ParamaterChangedFlag = ParamaterChangedFlag;
}

/**
 * @brief WeldRecipeParameter::storeRecipeToDB : StoreRecipe value to Database
 */
void WeldRecipeParameter::storeRecipeToDB(bool bIsNewRecipe)
{
    //Recipe Params
    QString a_recipeBuffer="";
    QString qstrStoreBuff="";
    if(m_ParamaterChangedFlag == SEND_ALL_PARAM || m_ParamaterChangedFlag == SEND_WELD_PARAM)
    {
        qstrStoreBuff = prepareWeldRecipeStringToDB();
        CIObj.sendMessage(REQ_SET_WELD_RECIPE_PARAM,RES_SET_WELD_RECIPE_PARAM,qstrStoreBuff);
        CIObj.recivedMessage(RES_SET_WELD_RECIPE_PARAM,a_recipeBuffer);
    }
    if(m_ParamaterChangedFlag == SEND_ALL_PARAM || m_ParamaterChangedFlag == SEND_SUSPECT_REJECT_LIMITS_PARAM)
    {
        qstrStoreBuff = prepareSuspectRejectStringToDB();
        CIObj.sendMessage(REQ_SET_SUSPECT_REJECT_LIMITS_INFO,RES_SET_SUSPECT_REJECT_LIMITS_INFO,qstrStoreBuff);
        CIObj.recivedMessage(RES_SET_SUSPECT_REJECT_LIMITS_INFO,a_recipeBuffer);
    }
    if(m_ParamaterChangedFlag == SEND_ALL_PARAM || m_ParamaterChangedFlag == SEND_BRANSON_PARAM)
    {
        qstrStoreBuff = prepareBransonOnlyParamsStringToDB();
        CIObj.sendMessage(REQ_SET_BRANSONONLY_PARAMS,RES_SET_BRANSONONLY_PARAMS,qstrStoreBuff);
        CIObj.recivedMessage(RES_SET_BRANSONONLY_PARAMS,a_recipeBuffer);
    }
    if(m_ParamaterChangedFlag == SEND_ALL_PARAM || m_ParamaterChangedFlag == SEND_SETUP_LIMITS_PARAM)
    {
        qstrStoreBuff = prepareSetupLimitsStringToDB();
        CIObj.sendMessage(REQ_SET_SETUP_LIMITS,RES_SET_SETUP_LIMITS,qstrStoreBuff);
        CIObj.recivedMessage(RES_SET_SETUP_LIMITS,a_recipeBuffer);
    }
    if((m_ParamaterChangedFlag == SEND_ALL_PARAM) && (bIsNewRecipe))
    {
        qstrStoreBuff = prepareStackRecipeStringToDB();
        CIObj.sendMessage(REQ_SET_STACK_RECIPE_DATA, RES_SET_STACK_RECIPE_DATA, qstrStoreBuff);
        CIObj.recivedMessage(RES_SET_STACK_RECIPE_DATA, a_recipeBuffer);
    }

    m_ParamaterChangedFlag = SEND_ALL_PARAM;
    if(m_weldRecipe.m_weldRecipe_UI.isNewRecipe == true)
    {
        int oldRecipe;
        if(m_oldRecipeNumber != 0)
            oldRecipe = m_oldRecipeNumber;
        else
            oldRecipe = m_weldRecipe.m_weldRecipe_SC.RecipeNumber;

        QString qStrRecipeSetupInfo = "";
        qStrRecipeSetupInfo.append(QString::number(oldRecipe) + ",");
        qStrRecipeSetupInfo.append(QString::number(m_weldRecipe.m_weldRecipe_SC.RecipeNumber) + ",");
        qStrRecipeSetupInfo.append("0,'','',100000,'NA',0,0,0,0,0,0,0");
        CIObj.sendMessage(REQ_SET_WELD_RECIPE_SETUP_INFO,RES_SET_WELD_RECIPE_SETUP_INFO,qStrRecipeSetupInfo);
        QString result="";
        CIObj.recivedMessage(RES_SET_WELD_RECIPE_SETUP_INFO,result);
        m_weldRecipe.m_weldRecipe_UI.isNewRecipe  = false;
    }
    m_oldRecipeNumber = 0;
}

/**
 * @brief WeldRecipeParameter::readRecipeFromDB : Read Recipe value From DB
 * @param qstrRecipeNumber
 */
int WeldRecipeParameter::readRecipeFromDB(QString qstrRecipeNumber)
{
    unsigned char ErrorCode = 0;
    if(m_weldRecipe.m_weldRecipe_SC.RecipeNumber == 0 && qstrRecipeNumber != ZERO_STR)
    {
        m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber = 0;
        Configuration::getInstance()->setConigValues("recipezero","recipe_number",ZERO_STR);
        Configuration::getInstance()->setConigValues("recipezero","recipe_version_number",ZERO_STR);
        setParamaterChangedFlag(0);
        storeRecipeToDB();
    }
    QString qstrRecipeBuffer="";
    CIObj.sendMessage(REQ_GET_WELD_RECIPE_PARAM,RES_GET_WELD_RECIPE_PARAM,qstrRecipeNumber);
    CIObj.recivedMessage(RES_GET_WELD_RECIPE_PARAM,qstrRecipeBuffer);
    if(qstrRecipeBuffer.isEmpty())
    {
        initializeRecipeParamaterStructure(0);
        return 0;
    }
    if(qstrRecipeNumber == "0")
    {
        switch(Configuration::getInstance()->m_systemData.PcConfig.PSFreq)
        {
        case SYSTEM_TYPE_20KHZ:
            m_CurrentSystemFreq = SYSTEM_TYPE_20KHZ;
            break;
        case SYSTEM_TYPE_30KHZ:
            m_CurrentSystemFreq = SYSTEM_TYPE_30KHZ;
            break;
        case SYSTEM_TYPE_40KHZ:
            m_CurrentSystemFreq = SYSTEM_TYPE_40KHZ;
            break;
        default:
            m_CurrentSystemFreq = SYSTEM_TYPE_20KHZ;
            break;
        }

        ErrorCode = parseWeldRecipeParameterFromDB(qstrRecipeBuffer);
        if(((ErrorCode & SIZE_ERROR) == SIZE_ERROR) || ((ErrorCode & DATA_ERROR) == DATA_ERROR))
        {
            initializeRecipeParamaterStructure(m_weldRecipe.m_weldRecipe_SC.RecipeNumber);
        }
        if(ErrorCode != 0)
        {
            qDebug()<<"ErrorCode:"<<ErrorCode;
            m_weldRecipe.m_weldRecipe_UI.isNewRecipe = false;
            setParamaterChangedFlag(0);
            storeRecipeToDB();
        }
    }
    else
        readWeldRecipeParameterFromDB(qstrRecipeBuffer);

    if(m_weldRecipe.m_weldRecipe_SC.RecipeNumber == 0)
    {
        m_weldRecipe.m_weldRecipe_UI.isModified = true;
        auto recipeNumber = Configuration::getInstance()->getConfigValues("recipezero","recipe_number");
        auto recipeVersionNumber = Configuration::getInstance()->getConfigValues("recipezero","recipe_version_number");

        if (m_weldRecipe.m_weldRecipe_SC.RecipeVerNumber == recipeNumber.toInt())
            m_weldRecipe.m_weldRecipe_UI.dwRecipeVerNumber = recipeVersionNumber.toInt();
    }
    else
    {
        m_weldRecipe.m_weldRecipe_UI.isModified = false;
        m_weldRecipe.m_weldRecipe_UI.dwRecipeVerNumber = 0;
    }
    return ErrorCode;
}

/**
 * @brief WeldRecipeParameter::convertSecToMsec : Convert seconds to millseconds
 * @param time
 * @return
 */
double WeldRecipeParameter::convertSecToMsec(double time)
{
    double tmp;
    tmp=time*(double)1000;
    return tmp;
}

/**
 * @brief WeldRecipeParameter::convertMsecToSec : Convert Milliseconds to seconds
 * @param time
 * @return
 */
double WeldRecipeParameter::convertMsecToSec(double time)
{
    double tmp;
    tmp=time/(double)1000;
    return tmp;
}

/**
 * @brief WeldRecipeParameter::convertMicrometreToMillimetre : ConvertMicrometreToMillimetre
 * @param length
 * @return
 */
double WeldRecipeParameter::convertMicrometreToMillimetre(double length)
{
    double tmp;
    tmp=length/(double)1000;
    return tmp;
}

/**
 * @brief WeldRecipeParameter::convertMillimetreToMicrometre : Convert Millimetre To Micrometre
 * @param length
 * @return
 */
double WeldRecipeParameter::convertMillimetreToMicrometre(double length)
{
    double tmp;
    tmp=length*(double)1000;
    return tmp;
}

/**
 * @brief WeldRecipeParameter::convertinchestomicrometre : Convert inches to micrometre
 * @param dbInches
 * @return
 */
double WeldRecipeParameter::convertinchestomicrometre(double dbInches)
{
    double dbMicrometre;
    dbMicrometre = dbInches * (double)25400;
    return dbMicrometre;
}

/**
 * @brief WeldRecipeParameter::convertinchestomillimetre : Convert inches to millimetre
 * @param dbInches
 * @return
 */
double WeldRecipeParameter::convertinchestomillimetre(double dbInches)
{
    double dbMillimetre;
    dbMillimetre = dbInches * (double)25.4;
    return dbMillimetre;
}

/**
 * @brief WeldRecipeParameter::convertmicrometretoinches : Convert micrometre to inches
 * @param dbmicrometres
 * @return
 */
double WeldRecipeParameter::convertmicrometretoinches(double dbmicrometres)
{
    double dbInches;
    dbInches = dbmicrometres / (double)25400;
    return dbInches;
}

/**
 * @brief WeldRecipeParameter::convertmillimetretoinches : Convert millimetre to inches
 * @param dbmicrometres
 * @return
 */
double WeldRecipeParameter::convertmillimetretoinches(double dbmicrometres)
{
    double dbInches;
    dbInches = dbmicrometres / (double)25.4;
    return dbInches;
}



/**
 * @brief WeldRecipeParameter::convertNtolbs : Convert Newtons to LECIBALS
 * @param dbNewtons
 * @return
 */
double WeldRecipeParameter::convertNtolbs(double dbNewtons)
{
    double dbLecibals;
    dbLecibals = dbNewtons /(double)4.44822;
    return dbLecibals;
}

/**
 * @brief WeldRecipeParameter::convertlbstoN : Convert LECIBALS to Newtons
 * @param dbLecibals
 * @return
 */
double WeldRecipeParameter::convertlbstoN(double dbLecibals)
{
    double dbNeutons;
    dbNeutons = dbLecibals *(double)4.44822;
    return dbNeutons;
}

/**
 * @brief WeldRecipeParameter::getDefaultValue : GetDefaultValue and assign to the Paraticular Values and Unit
 * @param qstrParamName
 * @return
 */
QString WeldRecipeParameter::getDefaultValue(QString qstrParamName)
{
    QString qstrDefValue = 0;
    if(m_MapRecipeParams.contains(qstrParamName.toLower()))
    {
        QString qstrUnit = m_MapRecipeParams[qstrParamName.toLower()].ParamUnit[m_UnitSelected];

        if("absolute distance" == qstrParamName.toLower())
        {
            qstrDefValue = getStructureValueByName("expected part contact");
            return qstrDefValue;
        }
        else if("absolute distance cutoff" == qstrParamName.toLower())
        {
            qstrDefValue = QString::number(getStructureValueByName("expected part contact").toDouble() + getMaxValue("collapse distance").toDouble());
            return qstrDefValue;
        }
        else if(qstrParamName.toLower() == "setup weld force low")
        {
            qstrDefValue = getMinValue("weld force");
        }

        /*Here Trigger Force added intentionally as this value depends on system's operating frequency*/
        if (qstrParamName.toLower() == "trigger force")
        {
            if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                switch (m_CurrentSystemFreq)
                {
                case SYSTEM_TYPE_20KHZ:
                    qstrDefValue = "44";
                    break;

                case SYSTEM_TYPE_30KHZ:
                case SYSTEM_TYPE_40KHZ:
                    qstrDefValue = "22";
                    break;

                default:
                    break;
                }
            }

            else
            {
                qstrDefValue = "25";
            }
        }

        else if(qstrParamName.toLower() == "setup trigger force low")
        {
            qstrDefValue = getMinValue("trigger force");
        }
        else if(qstrParamName.toLower() == "setup trigger force high")
        {
            qstrDefValue = getMaxValue("trigger force");
        }
        else
            qstrDefValue = m_MapRecipeParams[qstrParamName.toLower()].ParamDefaultValue[m_CurrentSystemFreq];

        if(qstrUnit == INCHES || qstrUnit == INCHPERSECOND || qstrUnit == CUST_NAME_UINIINSECSQU)
        {
            double dval = convertmillimetretoinches(qstrDefValue.toDouble());
            qstrDefValue.setNum(dval,'f',4);
        }
        else if(qstrUnit == LECIBALS)
        {
            qstrDefValue = QString::number(convertNtolbs(qstrDefValue.toDouble()));
            qstrDefValue = QString::number(round(qstrDefValue.toDouble()));
        }

        if("weld force" == qstrParamName.toLower() || "force step 1" == qstrParamName.toLower())
        {
            QString dwForceValue = getStructureValueByName("trigger force");
            if(dwForceValue.toDouble() > qstrDefValue.toDouble())
                qstrDefValue = dwForceValue;
        }

        /*Default value to be display on keyboard*/
        if(qstrParamName.toLower() == "setup hold force low")
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                switch (m_CurrentSystemFreq)
                {
                case SYSTEM_TYPE_20KHZ:
                    qstrDefValue = "44";
                    break;

                case SYSTEM_TYPE_30KHZ:
                case SYSTEM_TYPE_40KHZ:
                    qstrDefValue = "22";
                    break;

                default:
                    break;
                }
            }
        }

        /*Default value to be display on keyboard*/
        if((qstrParamName.toLower() == "setup weld force high") || (qstrParamName.toLower() == "setup hold force high"))
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                qstrDefValue = "2000";
            }
        }

        /*Default values handling while new recipe creation*/
        if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            if(qstrParamName.toLower() == "suspect absolute distance low value")
            {
                {
                    qstrDefValue = "3.00";
                }
            }

            else if(qstrParamName.toLower() == "suspect absolute distance high value")
            {
                {
                    qstrDefValue = "125.00";
                }
            }

            else if(qstrParamName.toLower() == "suspect collapse distance low value")
            {
                {
                    qstrDefValue = "0.01";
                }
            }

            else if(qstrParamName.toLower() == "suspect collapse distance high value")
            {
                {
                    qstrDefValue = "25.00";
                }
            }

            else if(qstrParamName.toLower() == "suspect end weld force low value")
            {
                switch (m_CurrentSystemFreq)
                {
                case SYSTEM_TYPE_20KHZ:
                    qstrDefValue = "44";
                    break;

                case SYSTEM_TYPE_30KHZ:
                case SYSTEM_TYPE_40KHZ:
                    qstrDefValue = "22";
                    break;

                default:
                    break;
                }
            }

            else if(qstrParamName.toLower() == "suspect end weld force high value")
            {
                {
                    qstrDefValue = "2000";
                }
            }

            else if(qstrParamName.toLower() == "reject absolute distance low value")
            {
                {
                    qstrDefValue = "3.00";
                }
            }

            else if(qstrParamName.toLower() == "reject absolute distance high value")
            {
                {
                    qstrDefValue = "125.00";
                }
            }

            else if(qstrParamName.toLower() == "reject collapse distance low value")
            {
                {
                    qstrDefValue = "0.01";
                }
            }

            else if(qstrParamName.toLower() == "reject collapse distance high value")
            {
                {
                    qstrDefValue = "25.00";
                }
            }

            else if(qstrParamName.toLower() == "reject end weld force low value")
            {
                switch (m_CurrentSystemFreq)
                {
                case SYSTEM_TYPE_20KHZ:
                    qstrDefValue = "44";
                    break;

                case SYSTEM_TYPE_30KHZ:
                case SYSTEM_TYPE_40KHZ:
                    qstrDefValue = "22";
                    break;

                default:
                    break;
                }
            }

            else if(qstrParamName.toLower() == "reject end weld force high value")
            {
                {
                    qstrDefValue = "2000";
                }
            }
        }

        /*To handle precision functionality during default value reading for Weld Amplitude and Weld Force stepping
          in Step@Absolute and Step@Collapse dropdown*/
        if(qstrParamName.toLower() == "collapse distance")
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                double dval = qstrDefValue.toDouble();
                qstrDefValue.setNum(dval,'f',2);
            }

            else
            {
                double dval = qstrDefValue.toDouble();
                qstrDefValue.setNum(dval,'f',3);
            }
        }
    }
    return qstrDefValue;
}

/**
 * @brief WeldRecipeParameter::getMinValue : Get Minimum Value and assign to the Recipe Parameter
 * @param qstrParamName
 * @return
 */
QString WeldRecipeParameter::getMinValue(QString qstrParamName)
{
    QString qstrMinValue = "";
    if(m_MapRecipeParams.contains(qstrParamName.toLower()))
    {
        QString qstrUnit = m_MapRecipeParams[qstrParamName.toLower()].ParamUnit[m_UnitSelected];
        if("absolute distance" == qstrParamName.toLower() || "absolute distance cutoff" == qstrParamName.toLower())
        {
            qstrMinValue = QString::number(getStructureValueByName(WELD_RECIPE_PARAM_EXP_PART_CONT).toDouble() -
                                           getStructureValueByName(WELD_RECIPE_PARAM_PART_CON_WIN_MINUS).toDouble());
            return qstrMinValue;
        }
        else
            qstrMinValue = m_MapRecipeParams[qstrParamName.toLower()].ParamMinValue[m_CurrentSystemFreq];

        if(qstrUnit == "in" || qstrUnit == "in/s" || qstrUnit == "in/s2")
        {
            double dval = convertmillimetretoinches(qstrMinValue.toDouble());
            qstrMinValue.setNum(dval,'f',4);
        }
        else if(qstrUnit == "lbs")
        {
            qstrMinValue = QString::number(convertNtolbs(qstrMinValue.toDouble()));
            qstrMinValue = QString::number(round(qstrMinValue.toDouble()));
        }

        /*Here Trigger Force added intentionally as this value depends on system's operating frequency*/
        if(qstrParamName.toLower() == "trigger force")
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                switch (m_CurrentSystemFreq)
                {
                case SYSTEM_TYPE_20KHZ:
                    qstrMinValue = "44";
                    break;

                case SYSTEM_TYPE_30KHZ:
                case SYSTEM_TYPE_40KHZ:
                    qstrMinValue = "22";
                    break;

                default:
                    break;
                }
            }
        }

        if(qstrParamName.toLower() =="setup weld force low")
        {
            QString dwForceValue = getStructureValueByName("trigger force");
            if(dwForceValue.toDouble() > qstrMinValue.toDouble())
            qstrMinValue = dwForceValue;
        }

        /*Min value to be display on keyboard*/
        if(qstrParamName.toLower ()== "collapse distance")
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                qstrMinValue = "0.01";
            }
        }

        /*To handle min value to be enter and display on keyboard for Trigger Force, Weld Force, Hold Force
          and Setup Hold Force based on System Type and System Frequency*/
        if((qstrParamName.toLower ()== "trigger force") || (qstrParamName.toLower ()== "weld force")
                || (qstrParamName.toLower ()== "hold force") || (qstrParamName.toLower ()== "setup hold force low"))
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                switch (m_CurrentSystemFreq)
                {
                case SYSTEM_TYPE_20KHZ:
                    qstrMinValue = "44";
                    break;

                case SYSTEM_TYPE_30KHZ:
                case SYSTEM_TYPE_40KHZ:
                    qstrMinValue = "22";
                    break;

                default:
                    break;
                }
            }
        }
    }

    return qstrMinValue;
}

/**
 * @brief WeldRecipeParameter::getMaxValue : Get Maximum Value and assign to the Recipe Parameter
 * @param qstrParamName
 * @return
 */
QString WeldRecipeParameter::getMaxValue(QString qstrParamName)
{
    QString qstrMaxValue = "";
    int mode;
    if(m_MapRecipeParams.contains(qstrParamName.toLower()))
    {
        QString qstrUnit = m_MapRecipeParams[qstrParamName.toLower()].ParamUnit[m_UnitSelected];
        if("peak power" == qstrParamName.toLower())
            qstrMaxValue = QString::number(Configuration::getInstance()->m_systemData.PcConfig.PSWatt);
        else if("absolute distance" == qstrParamName.toLower() || "absolute distance cutoff" == qstrParamName.toLower())
        {
            mode = getStructureValueByIndex(SC_WELD_MODE).toInt();
            ModeInfo stModeInfo = getModeInfoByIndex(mode,"");
            if(stModeInfo.ModeName == WELD_MODE_COL_DIS)
                qstrMaxValue = QString::number(getStructureValueByName("expected part contact").toDouble() + getMaxValue("collapse distance").toDouble());
            else
                qstrMaxValue = QString::number(Configuration::getInstance()->m_systemData.ActConfig.MaxStrokelength);
            return qstrMaxValue;
        }
        else if(qstrParamName.toLower() == "ready offset")
        {
            double dValue = getStructureValueByIndex(AC_EXPECTED_PART_CONTACT_POSITION).toDouble();
            double dActualValue = convertMicrometreToMillimetre(dValue - 5000);
            if(dActualValue <= 0)
                qstrMaxValue = "0";
            else
                qstrMaxValue = QString::number(dActualValue);
            /*To handle precision for max value of Ready Offset parameter to be display on keyboard
              based on System Type*/
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                qstrMaxValue.setNum(qstrMaxValue.toDouble(),'f',2);
            }

            else
            {
                qstrMaxValue.setNum(qstrMaxValue.toDouble(),'f',3);
            }

            return qstrMaxValue;
        }
        else
            qstrMaxValue = m_MapRecipeParams[qstrParamName.toLower()].ParamMaxValue[m_CurrentSystemFreq];

        if(qstrUnit == "in" || qstrUnit == "in/s" || qstrUnit == "in/s2")
        {
            double dval = convertmillimetretoinches(qstrMaxValue.toDouble());
            qstrMaxValue.setNum(dval,'f',4);
        }
        else if(qstrUnit == "lbs")
        {
            qstrMaxValue = QString::number(convertNtolbs(qstrMaxValue.toDouble()));
            qstrMaxValue = QString::number(round(qstrMaxValue.toDouble()));
        }

        /*Max value to be display on keyboard*/
        if((qstrParamName.toLower() == "trigger force") || (qstrParamName.toLower() == "setup trigger force high"))
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                qstrMaxValue = "2000";
            }

            QString dwForceValue = getStructureValueByName("weld force");
            if(dwForceValue.toDouble() < qstrMaxValue.toDouble())
                qstrMaxValue = dwForceValue;
        }

        /*To handle max value to be enter and display on keyboard for Weld Force, Hold Force
         and Setup Hold Force based on System Type*/
        if((qstrParamName.toLower ()== "weld force") || (qstrParamName.toLower ()== "hold force")
                || (qstrParamName.toLower ()== "setup hold force high"))
        {
            if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                qstrMaxValue = "2000";
            }
        }
    }
    return qstrMaxValue;
}

/**
 * @brief WeldRecipeParameter::setMaxValue
 * @param qstrParamName
 * @param qstrParamVal
 */
void WeldRecipeParameter::setMaxValue(QString qstrParamName,int qstrParamVal)
{
    m_MapRecipeParams[qstrParamName.toLower()].ParamMaxValue[m_CurrentSystemFreq] = QString::number(qstrParamVal);
}

/**
 * @brief WeldRecipeParameter::setMetrixUnit
 * @param dwMetrixUnit
 */
void WeldRecipeParameter::setMetrixUnit(int dwMetrixUnit)
{
    m_UnitSelected = static_cast<ParamUnits>(dwMetrixUnit);
}

/**
 * @brief WeldRecipeParameter::setPsFreqValue : Set Powersupply Frequency Value
 * @param dwPsFreq
 */
void WeldRecipeParameter::setPsFreqValue(int dwPsFreq)
{
    m_CurrentSystemFreq = static_cast<SystemFreq>(dwPsFreq);
}

/**
 * @brief WeldRecipeParameter::getFirstRuntimeIndex
 * @return
 */
QString WeldRecipeParameter::getFirstRuntimeIndex()
{
    QStringList WeldModeModelList;

    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_TIME) == 1)
    {
        WeldModeModelList.append("1");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ENERGY) == 1)
    {
        WeldModeModelList.append("2");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_POWER) == 1)
    {
        WeldModeModelList.append("3");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_GROUND_DETECT) == 1)
    {
        WeldModeModelList.append("4");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ABSOLUTE_DISTANCE) == 1)
    {
        WeldModeModelList.append("5");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_COLLAPSE_DISTANCE) == 1)
    {
        WeldModeModelList.append("6");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_DYNAMIC) == 1)
    {
        WeldModeModelList.append("7");
    }
    if(WeldModeModelList.size() > 0)
        return WeldModeModelList[0];
    return "0";
}

/**
 * @brief WeldRecipeParameter::setlimitsFlagsForBL : keep the Limits and send to BL
 * @param stSC_Temp
 */
void WeldRecipeParameter::setlimitsFlagsForBL(WeldRecipe_SC &stSC_Temp)
{
    if(!stSC_Temp.SuspectLimit.AbsoluteDistance.IsEnabled)
    {
        stSC_Temp.SuspectLimit.AbsoluteDistance.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.AbsoluteDistance.IsHighEnabled = false;
    }

    if(!stSC_Temp.SuspectLimit.CollapseDistance.IsEnabled)
    {
        stSC_Temp.SuspectLimit.CollapseDistance.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.CollapseDistance.IsHighEnabled = false;
    }

    if(!stSC_Temp.SuspectLimit.EndWeldForce.IsEnabled)
    {
        stSC_Temp.SuspectLimit.EndWeldForce.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.EndWeldForce.IsHighEnabled = false;
    }

    if(!stSC_Temp.SuspectLimit.Energy.IsEnabled)
    {
        stSC_Temp.SuspectLimit.Energy.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.Energy.IsHighEnabled = false;
    }

    if(!stSC_Temp.SuspectLimit.Frequency.IsEnabled)
    {
        stSC_Temp.SuspectLimit.Frequency.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.Frequency.IsHighEnabled = false;
    }

    if(!stSC_Temp.SuspectLimit.PeakPower.IsEnabled)
    {
        stSC_Temp.SuspectLimit.PeakPower.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.PeakPower.IsHighEnabled = false;
    }

    if(!stSC_Temp.SuspectLimit.Time.IsEnabled)
    {
        stSC_Temp.SuspectLimit.Time.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.Time.IsHighEnabled = false;
    }

    if(!stSC_Temp.SuspectLimit.TriggerDistance.IsEnabled)
    {
        stSC_Temp.SuspectLimit.TriggerDistance.IsLowEnabled = false;
        stSC_Temp.SuspectLimit.TriggerDistance.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.AbsoluteDistance.IsEnabled)
    {
        stSC_Temp.RejectLimit.AbsoluteDistance.IsLowEnabled = false;
        stSC_Temp.RejectLimit.AbsoluteDistance.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.CollapseDistance.IsEnabled)
    {
        stSC_Temp.RejectLimit.CollapseDistance.IsLowEnabled = false;
        stSC_Temp.RejectLimit.CollapseDistance.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.EndWeldForce.IsEnabled)
    {
        stSC_Temp.RejectLimit.EndWeldForce.IsLowEnabled = false;
        stSC_Temp.RejectLimit.EndWeldForce.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.Energy.IsEnabled)
    {
        stSC_Temp.RejectLimit.Energy.IsLowEnabled = false;
        stSC_Temp.RejectLimit.Energy.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.Frequency.IsEnabled)
    {
        stSC_Temp.RejectLimit.Frequency.IsLowEnabled = false;
        stSC_Temp.RejectLimit.Frequency.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.PeakPower.IsEnabled)
    {
        stSC_Temp.RejectLimit.PeakPower.IsLowEnabled = false;
        stSC_Temp.RejectLimit.PeakPower.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.Time.IsEnabled)
    {
        stSC_Temp.RejectLimit.Time.IsLowEnabled = false;
        stSC_Temp.RejectLimit.Time.IsHighEnabled = false;
    }

    if(!stSC_Temp.RejectLimit.TriggerDistance.IsEnabled)
    {
        stSC_Temp.RejectLimit.TriggerDistance.IsLowEnabled = false;
        stSC_Temp.RejectLimit.TriggerDistance.IsHighEnabled = false;
    }
}

/**
 * @brief WeldRecipeParameter::CheckSetupRange : Set the range values for limits parameter
 * @param qstrParamName
 * @param qstrParamValue
 * @return
 */
int WeldRecipeParameter::CheckSetupRange(QString qstrParamName,double qstrParamValue)
{
    int dwRet = ON_FAIL_LIMIT;
      if (!getStructureValueByIndex(UI_GLOBAL_SETUP).toInt() ||
          !getStructureValueByName(SETUP+qstrParamName+STATUS).toInt())
     {
        dwRet = OFF;
    }
    else
    {
        double dMinVal = getStructureValueByName(SETUP+qstrParamName+LOW).toDouble();
        double dMaxVal = getStructureValueByName(SETUP+qstrParamName+HIGH).toDouble();
        if((qstrParamValue>=dMinVal) && (qstrParamValue<=dMaxVal))
            dwRet = ON_SUCCESS;
    }
    return dwRet;
}

/**
 * @brief WeldRecipeParameter::CheckSuspectRejectRange : Checking the Range for suspect and reject and return the status
 * @param qstrParamName
 * @param qstrParamValue
 * @return
 */
int WeldRecipeParameter::CheckSuspectRejectRange(QString qstrParamName,double qstrParamValue)
{
    int dwRet = ON_FAIL_LIMIT;

    bool bGlobalSuspect = getStructureValueByIndex(SC_GLOBAL_SUSPECT).toInt();
    bool bGlobalReject = getStructureValueByIndex(SC_GLOBAL_REJECT).toInt();
    bool bParamSuspect = getStructureValueByName(SUSPECT + qstrParamName).toInt();
    bool bParamReject = getStructureValueByName(REJECT + qstrParamName).toInt();
    bool bSuspectLowEnabled = getStructureValueByName(SUSPECT + qstrParamName + LOW).toInt();
    bool bSuspectHighEnabled = getStructureValueByName(SUSPECT + qstrParamName + HIGH).toInt();
    bool bRejectLowEnabled = getStructureValueByName(REJECT + qstrParamName + LOW).toInt();
    bool bRejectHighEnabled = getStructureValueByName(REJECT + qstrParamName + HIGH).toInt();

    double dMinValue = -1;
    double dMaxValue = -1;

    if(bGlobalSuspect && bParamSuspect && bSuspectLowEnabled)
        dMinValue = getStructureValueByName(SUSPECT + qstrParamName + LOW + VALUE).toDouble();
    else if(bGlobalReject && bParamReject && bRejectLowEnabled)
        dMinValue = getStructureValueByName(REJECT + qstrParamName + LOW + VALUE).toDouble();

    if(bGlobalSuspect && bParamSuspect && bSuspectHighEnabled)
        dMaxValue = getStructureValueByName(SUSPECT + qstrParamName + HIGH+VALUE).toDouble();
    else if(bGlobalReject && bParamReject && bRejectHighEnabled)
        dMaxValue = getStructureValueByName(REJECT + qstrParamName + HIGH+VALUE).toDouble();

    if(dMinValue < 0 && dMaxValue < 0)
    {
        dwRet = OFF;
    }
    else if(dMinValue < 0)
    {
        if(dMaxValue > qstrParamValue)
            dwRet = ON_SUCCESS;
    }
    else if(dMaxValue < 0)
    {
        if(dMinValue < qstrParamValue)
            dwRet = ON_SUCCESS;
    }
    else
    {
        if(dMaxValue > qstrParamValue && dMinValue < qstrParamValue)
            dwRet = ON_SUCCESS;
    }
    return dwRet;
}

/**
 * @brief WeldRecipeParameter::CheckControlLimits : Checking Control limits of Parameter and return the values
 * @param qstrParamName
 * @param qstrParamValue
 * @return
 */
int WeldRecipeParameter::CheckControlLimits(QString qstrParamName,double qstrParamValue)
{
    int dwRet = ON_FAIL_LIMIT;
    if(!m_weldRecipe.m_weldRecipe_UI.isValid ||
            !(getStructureValueByIndex(SC_CONTROL_LIMIT).toInt()) ||
            !(getStructureValueByName(qstrParamName+CUTOFF_STATE).toInt()))
    {
        dwRet = OFF;
    }
    else
    {
        dwRet = CheckSuspectRejectRange(qstrParamName,qstrParamValue);
    }
    return dwRet;
}

/**
 * @brief WeldRecipeParameter::isActiveRequired: This function will set a flag which is used to verify that recipe zero need to sedn to BL or not.
 * @param bIsActiveReq
 */
void WeldRecipeParameter::setIsActiveRequired(bool bIsActiveReq)
{
    m_IsActiveRequired = bIsActiveReq;
}

/**
 * @brief WeldRecipeParameter::getIsActiveRequired: This function will return the Is Active Req flag
 * @return
 */
bool WeldRecipeParameter::getIsActiveRequired()
{
    return m_IsActiveRequired;
}

/**
 * @brief WeldRecipeParameter::forceSettingIndependent
 * @param qstrParamValue
 */
void WeldRecipeParameter::forceSettingIndependent(QString qstrParamValue)
{
    memcpy(&m_weldActuatorForceRecipe,&m_weldRecipe,sizeof(m_weldRecipe));
    m_weldActuatorForceRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce = qstrParamValue.toInt();

    sendStructureToSC(ACTUATOR_FORCE_RECIPE);
}

/**
 * @brief WeldRecipeParameter::copyRecipeStructToForceStruct : Copies recipe structure to actuator force structure
 */
void WeldRecipeParameter::copyRecipeStructToForceStruct()
{
    memcpy(&m_weldActuatorForceRecipe,&m_weldRecipe,sizeof(m_weldRecipe));
}

/**
 * @brief WeldRecipeParameter::getActuatorRecipeStructureValue
 * @return
 */
QString WeldRecipeParameter::getActuatorRecipeStructureValue()
{
    return QString::number(m_weldActuatorForceRecipe.m_weldRecipe_PC_AC.m_weldRecipe_AC.WeldForce);
}
