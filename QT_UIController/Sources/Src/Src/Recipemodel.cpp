/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Weld Recipe Model

***************************************************************************/

#include <QList>
#include <QQmlContext>
#include <QDateTime>
#include <QTime>
#include <QSettings>
#include "Header/communicationinterface.h"
#include "Header/pressureparameter.h"
#include <Header/recipe.h>
#include <Header/graphtableview.h>
#include "Header/informationpanel.h"
#include "Header/configuration.h"
#include <Header/setupparameter.h>
#include <Header/setuplimitis.h>
#include <Header/localizeheader.h>
#include "Header/utility.h"

extern QQmlContext* context;
/**
 * @brief Recipe::getSteppingComboBoxList
 * @return
 */
QStringList Recipe::getSteppingComboBoxList()
{
    QStringList SteppingModeModel;

    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_TIME) == 1)
    {
        SteppingModeModel.append(RECIPE_PARAM_STEP_TIME);
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ENERGY) == 1)
    {
        SteppingModeModel.append(RECIPE_PARAM_STEP_ENERGY);
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_POWER) == 1)
    {
        SteppingModeModel.append(RECIPE_PARAM_STEP_POWER);
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ABSOLUTE_DISTANCE) == 1)
    {
        SteppingModeModel.append(RECIPE_PARAM_STEP_AD);
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_COLLAPSE_DISTANCE) == 1)
    {
        SteppingModeModel.append(RECIPE_PARAM_STEP_CD);
    }

    SteppingModeModel.append(RECIPE_PARAM_STEP_EXT_SIG);
    return SteppingModeModel;
}

/**
 * @brief Recipe::getWeldModesList
 * @return
 */
QString Recipe::getWeldModesList()
{
    QString WeldModeModel="";

    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_TIME) == 1)
    {
        WeldModeModel.append("Time,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ENERGY) == 1)
    {
        WeldModeModel.append("Energy,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_POWER) == 1)
    {
        WeldModeModel.append("Peak Power,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_GROUND_DETECT) == 1)
    {
        WeldModeModel.append("Ground Detect,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ABSOLUTE_DISTANCE) == 1)
    {
        WeldModeModel.append("Absolute Distance,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_COLLAPSE_DISTANCE) == 1)
    {
        WeldModeModel.append("Collapse Distance,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_DYNAMIC) == 1)
    {
        WeldModeModel.append("Dynamic,");
    }

    WeldModeModel.remove(WeldModeModel.length()-1,1);
    return WeldModeModel;
}

/**
 * @brief Recipe::getLocaleWeldModesList
 * @return
 */
QString Recipe::getLocaleWeldModesList()
{
    QString WeldModeModel="";

    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_TIME) == 1)
    {
        WeldModeModel.append(RECIPE_PARAM_TIME+",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ENERGY) == 1)
    {
        WeldModeModel.append(RECIPE_PARAM_ENERGY+",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_POWER) == 1)
    {
        WeldModeModel.append(RECIPE_PARAM_PEAK_POWER+",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_GROUND_DETECT) == 1)
    {
        WeldModeModel.append(RECIPE_PARAM_GD+",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ABSOLUTE_DISTANCE) == 1)
    {
        WeldModeModel.append(RECIPE_PARAM_AD+",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_COLLAPSE_DISTANCE) == 1)
    {
        WeldModeModel.append(RECIPE_PARAM_CD+",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_DYNAMIC) == 1)
    {
        WeldModeModel.append(RECIPE_PARAM_DY+",");
    }

    WeldModeModel.remove(WeldModeModel.length()-1,1);
    return WeldModeModel;
}

/**
 * @brief Recipe::getWeldModesListIndexes
 * @return
 */
QString Recipe::getWeldModesListIndexes()
{
    QString WeldModeModel="";

    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_TIME) == 1)
    {
        WeldModeModel.append("1,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ENERGY) == 1)
    {
        WeldModeModel.append("2,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_POWER) == 1)
    {
        WeldModeModel.append("3,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_GROUND_DETECT) == 1)
    {
        WeldModeModel.append("4,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ABSOLUTE_DISTANCE) == 1)
    {
        WeldModeModel.append("5,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_COLLAPSE_DISTANCE) == 1)
    {
        WeldModeModel.append("6,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_DYNAMIC) == 1)
    {
        WeldModeModel.append("7,");
    }

    WeldModeModel.remove(WeldModeModel.length()-1,1);
    return WeldModeModel;
}

/**
 * @brief Recipe::getLimitsMenu
 * @return QString
 */
QString Recipe::getLimitsMenu()
{
    QString menulimits = "Setup,Control,Suspect & Reject";
    return menulimits;
}

/**
 * @brief Recipe::getLocaleLimitsMenu
 * @return QString
 */
QString Recipe::getLocaleLimitsMenu()
{
    QString menulimits = RECIPE_PARAM_MENU_SETUP + "," + RECIPE_PARAM_MENU_CONTROL + "," + RECIPE_PARAM_MENU_SPT_RJT;
    return menulimits;
}

/**
 * @brief Recipe::getWeldProcessMenu
 * @return
 */
QString Recipe::getWeldProcessMenu()
{
    QString menuLimits = "";

    if(Configuration::getInstance()->getFeatureFlagStatus(PRE_TRIGGER) == 1)
    {
        menuLimits.append("Pretrigger,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(ENERGY_BRAKE) == 1)
    {
        menuLimits.append("Energy Brake,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(AFTER_BURST) == 1)
    {
        menuLimits.append("Afterburst,");
    }

    menuLimits.append("Stack Recipe");
    return menuLimits;
}

/**
 * @brief Recipe::getLocaleWeldProcessMenu
 * @return
 */
QString Recipe::getLocaleWeldProcessMenu()
{
    QString menuLimits = "";

    if(Configuration::getInstance()->getFeatureFlagStatus(PRE_TRIGGER) == 1)
    {
        menuLimits.append(RECIPE_PARAM_PRETRIGGER+",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(ENERGY_BRAKE) == 1)
    {
        menuLimits.append(RECIPE_PARAM_EB+ ",");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(AFTER_BURST) == 1)
    {
        menuLimits.append(RECIPE_PARAM_AB+",");
    }

    menuLimits.append("Stack Recipe");

    return menuLimits;
}

/**
 * @brief Recipe::getFixedParamsList
 * @return
 */
QString Recipe::getFixedParamsList()
{
    QString params = "WELD MODE,"
                     "WELD AMPLITUDE,"
                     "TRIGGER FORCE,"
                     "WELD FORCE,"
                     "FORCE RAMP TIME,"
                     "HOLD TIME,"
                     "HOLD FORCE,"
                     "REACTIVITY,"
                     "FORCE LEVEL";
    return params;
}

/**
 * @brief Recipe::getFixedParamsList
 * @return
 */
QString Recipe::getLocaleFixedParamsList()
{
    QString params = CUST_NAME_WELD+ ","+
                     RECIPE_PARAM_WELD_AMP+","+
                     RECIPE_PARAM_TRI_FORCE+ ","+
                     RECIPE_PARAM_WELD_FORCE+","+
                     RECIPE_PARAM_FORCE_RAMP_TIME+","+
                     RECIPE_PARAM_HOLD_TIME+","+
                     RECIPE_PARAM_HOLD_FORCE+","+
                     RECIPE_PARAM_REACTIVITY+","+
                     RECIPE_PARAM_FORCE_LEVEL;
    return params;
}

/**
 * @brief Recipe::getPreTriggerParamsList
 * @return
 */
QString Recipe::getPreTriggerParamsList()
{
    QString params = "Pretrigger,"
                     "Pretrigger Amplitude,"
                     "DistanceAuto,"
                     "Pretrigger Distance";
    return params;
}

/**
 * @brief Recipe::getPreTriggerParamsList
 * @return
 */
QString Recipe::getLocalePreTriggerParamsList()
{
    QString params = RECIPE_PARAM_PRETRIGGER+ ","+
                     RECIPE_PARAM_PRE_AMP+","+
                     WELD_DISTANCE_AUTO+ ","+
                     RECIPE_PARAM_PRE_DIS;
    return params;
}

/**
 * @brief Recipe::getEnergyBrakeParamsList
 * @return
 */
QString Recipe::getEnergyBrakeParamsList()
{
    QString params = "Energy Brake,"
                     "Energy Brake Time,"
                     "Energy Brake Amplitude";
    return params;
}

/**
 * @brief Recipe::getEnergyBrakeParamsList
 * @return
 */
QString Recipe::getLocaleEnergyBrakeParamsList()
{
    QString params = RECIPE_PARAM_EB+ ","+
                     RECIPE_PARAM_EB_TIME+ ","+
                     RECIPE_PARAM_EB_AMP;
    return params;
}

/**
 * @brief Recipe::getAfterBurstParamsList
 * @return
 */
QString Recipe::getAfterBurstParamsList()
{
    QString params = "Afterburst,"
                     "Afterburst Delay,"
                     "Afterburst Time,"
                     "Afterburst Amplitude";
    return params;
}

/**
 * @brief Recipe::getAfterBurstParamsList
 * @return
 */
QString Recipe::getLocaleAfterBurstParamsList()
{
    QString params = RECIPE_PARAM_AB+ ","+
                     RECIPE_PARAM_AB_DELAY+","+
                     RECIPE_PARAM_AB_TIME+","+
                     RECIPE_PARAM_AB_AMP;
    return params;
}

void Recipe::getAtoZParamsMap()
{
    paramAtoZList.clear();
    paramAtoZLocaleList.clear();
    QMap<QString,QString>qmapAtoZParams;
    qmapAtoZParams.insert(RECIPE_PARAM_HOLD_FORCE_RAMP_TIME,"Hold Force Ramp Time");
    qmapAtoZParams.insert(RECIPE_PARAM_MAX_TIMEOUT,"Max Timeout");
    qmapAtoZParams.insert(RECIPE_PARAM_TIMED_SEEK,"Timed Seek");
    qmapAtoZParams.insert(RECIPE_PARAM_TIMED_SEEK_PERIOD,"Timed Seek Period");
    qmapAtoZParams.insert(RECIPE_PARAM_POST_WELD_SEEK,"Post-Weld Seek");
    qmapAtoZParams.insert(RECIPE_PARAM_PRE_WELD_SEEK,"Pre-Weld Seek");
    qmapAtoZParams.insert(RECIPE_PARAM_READY_OFFSET,"Ready Offset");
    //qmapAtoZParams.insert(RECIPE_PARAM_TOTAL_COLLAPSE_TARGET,"Total Collapse Target");
    qmapAtoZParams.insert(RECIPE_PARAM_WELD_RAMP_TIME,"WELD RAMP TIME");
    qmapAtoZParams.insert(RECIPE_PARAM_EXTRA_COOLING,"Extra Cooling");
    qmapAtoZParams.insert(RECIPE_PARAM_WELD_FORCE_CONTROL,"Weld Force Control");

    if(Configuration::getInstance()->getFeatureFlagStatus(PRE_TRIGGER) == 1)
    {
        qmapAtoZParams.insert(RECIPE_PARAM_PRETRIGGER,"Pretrigger");
        qmapAtoZParams.insert(RECIPE_PARAM_PRE_AMP,"Pretrigger Amplitude");
        qmapAtoZParams.insert(WELD_DISTANCE_AUTO,"DistanceAuto");
        qmapAtoZParams.insert(RECIPE_PARAM_PRE_DIS,"Pretrigger Distance");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(ENERGY_BRAKE) == 1)
    {
        qmapAtoZParams.insert(RECIPE_PARAM_EB,"Energy Brake");
        qmapAtoZParams.insert(RECIPE_PARAM_EB_TIME,"Energy Brake Time");
        qmapAtoZParams.insert(RECIPE_PARAM_EB_AMP,"Energy Brake Amplitude");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(AFTER_BURST) == 1)
    {
        qmapAtoZParams.insert(RECIPE_PARAM_AB,"Afterburst");
        qmapAtoZParams.insert(RECIPE_PARAM_AB_DELAY,"Afterburst Delay");
        qmapAtoZParams.insert(RECIPE_PARAM_AB_TIME,"Afterburst Time");
        qmapAtoZParams.insert(RECIPE_PARAM_AB_AMP,"Afterburst Amplitude");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_DYNAMIC) == 1)
    {
          qmapAtoZParams.insert(RECIPE_PARAM_FORCE_LEVEL_ENABLE,"Force Level Enable");
          qmapAtoZParams.insert(RECIPE_PARAM_FORCE_LEVEL_TIME,"Force Level Time");
    }

    qmapAtoZParams.insert(RECIPE_PARAM_SCRUB_AMP,"Scrub Amplitude");

    QMap<QString,QString>::const_iterator mapItr = qmapAtoZParams.constBegin();
    for(int dwIndex=0;dwIndex<qmapAtoZParams.size();dwIndex++)
    {
        paramAtoZLocaleList.append(mapItr.key());
        paramAtoZList.append(mapItr.value());
        ++mapItr;
    }
}

/**
 * @brief Recipe::getSetupLimitsParamsList
 * @return
 */
QString Recipe::getSetupLimitsParamsList()
{
    QString setupLimits = "Global Setup,"
                          "Weld Mode,"
                          "Weld Amplitude,"
                          "Weld Force,"
                          "Trigger Force,"
                          "Hold Force,"
                          "Hold Time,"
                          "Reactivity,"
                          "Force Level";
    return setupLimits;
}

/**
 * @brief Recipe::getLocaleSetupLimitsParamsList
 * @return
 */
QString Recipe::getLocaleSetupLimitsParamsList()
{
    QString setupLimits = RECIPE_PARAM_GLOBAL_SETUP+","+
                          CUST_NAME_WELD+","+
                          RECIPE_PARAM_WELD_AMP+","+
                          RECIPE_PARAM_WELD_FORCE+","+
                          RECIPE_PARAM_TRI_FORCE+","+
                          RECIPE_PARAM_HOLD_FORCE+","+
                          RECIPE_PARAM_HOLD_TIME+","+
                          RECIPE_PARAM_REACTIVITY+","+
                          RECIPE_PARAM_FORCE_LEVEL;
    return setupLimits;
}

/**
 * @brief Recipe::getControlLimitsParamsList
 * @return
 */
QString Recipe::getControlLimitsParamsList()
{
    QString controlLimits;
    /*To handle availability of Peak Power Cutoff and Ground Detect Cutoff in Control Limits parameter list
      based on System Type*/
    if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        controlLimits = "Control,"
                        "Frequency Low Cutoff,"
                        "Absolute Distance Cutoff,"
                        "Frequency High Cutoff,"
                        "Collapse Distance Cutoff,"
                        "Energy Cutoff,"
                        "Time Cutoff";
    }

    else
    {
        controlLimits = "Control,"
                        "Peak Power Cutoff,"
                        "Frequency Low Cutoff,"
                        "Absolute Distance Cutoff,"
                        "Frequency High Cutoff,"
                        "Collapse Distance Cutoff,"
                        "Energy Cutoff,"
                        "Time Cutoff,"
                        "Ground Detect Cutoff";
    }

    return controlLimits;
}

/**
 * @brief Recipe::getControlLimitsParamsList
 * @return
 */
QString Recipe::getLocaleControlLimitsParamsList()
{
    QString controlLimits;
    /*To handle availability of Peak Power Cutoff and Ground Detect Cutoff in Control Limits parameter list
      based on System Type*/
    if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        controlLimits = RECIPE_PARAM_CONTROL+","+
                RECIPE_PARAM_FREQ_LO_CO+","+
                RECIPE_PARAM_AD_CO+","+
                RECIPE_PARAM_FREQ_HI_CO+","+
                RECIPE_PARAM_CD_CO+","+
                RECIPE_PARAM_ENERGY_CO+","+
                RECIPE_PARAM_TIME_CO;
    }

    else
    {
        controlLimits = RECIPE_PARAM_CONTROL+","+
                RECIPE_PARAM_POWER_CO+","+
                RECIPE_PARAM_FREQ_LO_CO+","+
                RECIPE_PARAM_AD_CO+","+
                RECIPE_PARAM_FREQ_HI_CO+","+
                RECIPE_PARAM_CD_CO+","+
                RECIPE_PARAM_ENERGY_CO+","+
                RECIPE_PARAM_TIME_CO+","+
                RECIPE_PARAM_GD_CO;
    }

    return controlLimits;
}

/**
 * @brief Recipe::getSuspectRejectLimitsParamsList
 * @return
 */
QString Recipe::getSuspectRejectLimitsParamsList()
{
    QString suspectRejectLimits;
    /*To handle availability of Peak Power in Suspect and Reject Limits parameter list based on System Type*/
    if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
     suspectRejectLimits = "Global Suspect,"
                                  "Global Reject,"
                                  "Time,"
                                  "Energy,"
                                  "Absolute Distance,"
                                  "Collapse Distance,"
                                  //"Trigger Distance,"
                                  "End Weld Force,"
                                  "Frequency";
    }

    else
    {
        suspectRejectLimits = "Global Suspect,"
                                     "Global Reject,"
                                     "Time,"
                                     "Energy,"
                                     "Peak Power,"
                                     "Absolute Distance,"
                                     "Collapse Distance,"
                                     //"Trigger Distance,"
                                     "End Weld Force,"
                                     "Frequency";
    }

    return suspectRejectLimits;
}

/**
 * @brief Recipe::getSuspectRejectLimitsParamsList
 * @return
 */
QString Recipe::getLocaleSuspectRejectLimitsParamsList()
{
    QString suspectRejectLimits;
    /*To handle availability of Peak Power in Suspect and Reject Limits parameter list based on System Type*/
    if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
     suspectRejectLimits = RECIPE_PARAM_G_SUSPECT+","+
                                  RECIPE_PARAM_G_REJECT+","+
                                  CUST_NAME_TIME+","+
                                  CUST_NAME_ENERGY+","+
                                  CUST_NAME_ABSOLUTE+","+
                                  CUST_NAME_COLLAPSE+","+
                                  //RECIPE_PARAM_TRI_DIS+","+
                                  RECIPE_PARAM_END_WELD_FORCE+","+
                                  RECIPE_PARAM_FREQ;
    }

    else
    {
        suspectRejectLimits = RECIPE_PARAM_G_SUSPECT+","+
                                     RECIPE_PARAM_G_REJECT+","+
                                     CUST_NAME_TIME+","+
                                     CUST_NAME_ENERGY+","+
                                     CUST_NAME_PEAK+","+
                                     CUST_NAME_ABSOLUTE+","+
                                     CUST_NAME_COLLAPSE+","+
                                     //RECIPE_PARAM_TRI_DIS+","+
                                     RECIPE_PARAM_END_WELD_FORCE+","+
                                     RECIPE_PARAM_FREQ;
    }

    return suspectRejectLimits;
}

/**
 * @brief Recipe::getBransonParamsList
 * @return
 */
QString Recipe::getBransonParamsList()
{
    QString params = "AmplitudeLoopC1,"
                     "AmplitudeLoopC2,"
                     "Phase Loop,"
                     "Phase Loop CF,"
                     "Frequency Low,"
                     "Frequency High,"
                     "Phase Limit Time,"
                     "Phase Limit,"
                     "Control Mode,"
                     "Blind Time Weld,"
                     "Blind Time Seek,"
                     "DDS Switch time,"
                     "Parameter7,"
                     "Parameter8,"
                     "Parameter9,"
                     "Parameter10,"
                     "Parameter11,"
                     "Parameter12,"
                     "Parameter13,"
                     "Parameter14,"
                     "Parameter15,"
                     "F_Limit Time,"
                     "Amp Proportional Gain,"
                     "Amp Integral Gain,"
                     "Phase Integral Gain,"
                     "Phase Proportional Gain,"
                     "Frequency Window Size,"
                     "PF Phase Control Loop,"
                     "PI Frequency Low,"
                     "PI Frequency High,"
                     "PI Phase Limit Time,"
                     "PI Phase Limit";
    return params;
}

/**
 * @brief Recipe::getBransonParamsList
 * @return
 */
QString Recipe::getLocaleBransonParamsList()
{
    QString params = RECIPE_PARAM_AMP_C1+","+
                     RECIPE_PARAM_AMP_C2+","+
                     RECIPE_PARAM_PHASE_LOOP+","+
                     RECIPE_PARAM_PHASE_LOOP_CF+","+
                     RECIPE_PARAM_FREQ_LOW+","+
                     RECIPE_PARAM_FREQ_HIGH+","+
                     RECIPE_PARAM_PHASE_LIMIT_TIME+","+
                     RECIPE_PARAM_PHASE_LIMIT+","+
                     RECIPE_PARAM_CONTROL_MODE+","+
                     RECIPE_PARAM_BLIND_TIME_WELD+","+
                     RECIPE_PARAM_BLIND_TIME_SEEK+","+
                     RECIPE_PARAM_DDS_SWITCH_TIME+","+
                     RECIPE_PARAM_PARAMETER_7+","+
                     RECIPE_PARAM_PARAMETER_8+","+
                     RECIPE_PARAM_PARAMETER_9+","+
                     RECIPE_PARAM_PARAMETER_10+","+
                     RECIPE_PARAM_PARAMETER_11+","+
                     RECIPE_PARAM_PARAMETER_12+","+
                     RECIPE_PARAM_PARAMETER_13+","+
                     RECIPE_PARAM_PARAMETER_14+","+
                     RECIPE_PARAM_PARAMETER_15+","+
                     RECIPE_PARAM_F_LIMIT_TIME+","+
                     RECIPE_PARAM_AMP_PROP_GAIN+","+
                     RECIPE_PARAM_AMP_INT_GAIN+","+
                     RECIPE_PARAM_PHASE_INT_GAIN+","+
                     RECIPE_PARAM_PHASE_PROP_GAIN+","+
                     RECIPE_PARAM_FREQ_WIN_SIZE+","+
                     RECIPE_PARAM_PF_PHASE_CONTROL_LOOP+","+
                     RECIPE_PARAM_PI_FREQ_LOW+","+
                     RECIPE_PARAM_PI_FREQ_HIGH+","+
                     RECIPE_PARAM_PI_PHASE_LIMIT_TIME+","+
                     RECIPE_PARAM_PI_PHASE_LIMIT;
    return params;
}

/**
 * @brief Recipe::getActuatorSetupParamsList
 * @return
 */
QString Recipe::getActuatorSetupParamsList()
{
    QString param = "Weld Force,"
                    "Ready Offset,"
                    "Expected Part Contact,"
                    "Expected Contact Offset,"
                    "Expected Contact Window";
    return param;
}

/**
 * @brief Recipe::getActuatorSetupParamsList
 * @return
 */
QString Recipe::getLocaleActuatorSetupParamsList()
{
    QString param = RECIPE_PARAM_WELD_FORCE+","+
                    RECIPE_PARAM_READY_OFFSET+","+
                    RECIPE_PARAM_EXP_PART_CONT+","+
                    RECIPE_PARAM_EXP_CONTACT_OFFSET+","+
                    RECIPE_PARAM_EXP_CONTACT_WINDOW;
    return param;
}

/**
 * @brief Recipe::getActuatorSetupAdvancedParamsList
 * @return
 */
QString Recipe::getActuatorSetupAdvancedParamsList()
{
    QString param = "Down Acceleration,"
                    "Down Max Velocity,"
                    "Down Deceleration,"
                    "Return Acceleration,"
                    "Return Max Velocity,"
                    "Return Deceleration";
    return param;
}

/**
 * @brief Recipe::getActuatorSetupAdvancedParamsList
 * @return
 */
QString Recipe::getLocaleActuatorSetupAdvancedParamsList()
{
    QString param = RECIPE_PARAM_DOWN_ACC+","+
                    RECIPE_PARAM_DOWN_MAX_VEL+","+
                    RECIPE_PARAM_DOWN_DEC+","+
                    RECIPE_PARAM_RET_ACC+","+
                    RECIPE_PARAM_RET_MAX_VEL+","+
                    RECIPE_PARAM_RET_DEC;
    return param;
}

/**
 * @brief Recipe::createAllModels
 */
void Recipe::createAllModels()
{
    createSteppingComboBoxModel();
    createWeldModesModel();
    createLimitsMenu();
    createWeldProcessMenu();
    createFixedParamModel();
    createPreTriggerModel();
    createEnergyBrakeModel();
    createAfterBurstModel();
    createAtoZParamModel();
    createSetupLimitsModel();
    createControlLimitsModel();
    createSuspectRejectLimitsModel();
    createBransonParamsModel();
    createActuatorSetupParamsModel();
    createActuatorSetupAdvancedParamsModel();
}

/**
 * @brief Recipe::createSteppingComboBoxModel
 */
void Recipe::createSteppingComboBoxModel()
{
    refreshModel(ENUM_DummySteppingModes);
    clearList(&m_steppingmodes);
    //QString qstrSteppingModel = getSteppingComboBoxList();
    QStringList steppingmodellist = getSteppingComboBoxList();
    if(steppingmodellist.size()>1)
    {
        for(int dwIndex=0;dwIndex<steppingmodellist.size();dwIndex++)
        {
            WeldModes* weldmodelist = new WeldModes;
            weldmodelist->m_ParameterName=steppingmodellist[dwIndex];
            m_steppingmodes.append(weldmodelist);
        }
    }
    refreshModel(ENUM_SteppingModes);
}

/**
 * @brief Recipe::createWeldModesModel
 */
void Recipe::createWeldModesModel()
{
    clearList(&m_weldmode);
    QString weldmodel = getWeldModesList();
    QString weldModelLocale = getLocaleWeldModesList();
    QStringList recipeweldmodellist = weldmodel.split(",");
    QStringList recipeWeldModelLocaleList = weldModelLocale.split(",");

    if(weldmodel != "")
    {
        for(int dwIndex=0;dwIndex<recipeweldmodellist.size();dwIndex++)
        {
            WeldModes* weldmodelist = new WeldModes;
            weldmodelist->m_ParameterName = recipeweldmodellist[dwIndex];
            weldmodelist->m_ParameterLocaleName = recipeWeldModelLocaleList[dwIndex];
            m_weldmode.append(weldmodelist);
        }
    }
    refreshModel(ENUM_WeldMode);
}

/**
 * @brief Recipe::createLimitsMenu
 */
void Recipe::createLimitsMenu()
{
    clearList(&m_menuList);

    QString menulimits = getLimitsMenu();
    QString menuLimitsLocale = getLocaleLimitsMenu();
    QStringList recipeMenuList = menulimits.split(",");
    QStringList recipeMenuListLocale = menuLimitsLocale.split(",");
    for(int dwIndex=0;dwIndex<recipeMenuList.size();dwIndex++)
    {
        MenuLimits* menulimitslist = new MenuLimits;
        menulimitslist->m_ParameterName=recipeMenuList[dwIndex];
        menulimitslist->m_ParameterLocaleName = recipeMenuListLocale[dwIndex];
        m_menuList.append(menulimitslist);
    }
    refreshModel(ENUM_MenuList);
}

/**
 * @brief Recipe::createWeldProcessMenu
 */
void Recipe::createWeldProcessMenu()
{
    refreshModel(ENUM_DummyWeldProcessModelList);
    clearList(&m_weldProcessModelList);
    QString menuWeldProcessLimits = getWeldProcessMenu();
    QString menuWeldProcessLimitsLocale = getLocaleWeldProcessMenu();
    QStringList recipeMenuList = menuWeldProcessLimits.split(",");
    QStringList recipeMenuListLocale = menuWeldProcessLimitsLocale.split(",");
    for (int dwIndex = 0; dwIndex < recipeMenuList.size()-1;dwIndex++)
    {
        MenuLimits* menulimitslist = new MenuLimits;
        menulimitslist->m_ParameterName = recipeMenuList[dwIndex];
        menulimitslist->m_ParameterLocaleName = recipeMenuListLocale[dwIndex];
        m_weldProcessModelList.append(menulimitslist);
    }
    refreshModel(ENUM_WeldProcessModelList);
}

/**
 * @brief Recipe::createParamModel
 * @param ptrModelList
 * @param qstrParams
 * @param qstrParamsLocale
 */
void Recipe::createParamModel(QList<QObject*> *ptrModelList,QString qstrParams,QString qstrParamsLocale)
{
    clearList(ptrModelList);
    QStringList qstrParamsList = qstrParams.split(",");
    QStringList qstrParamsListLocale = qstrParamsLocale.split(",");
    QString paramValue;
    for(int dwIndex=0;dwIndex<qstrParamsList.size();dwIndex++)
    {
        stRecipeParmInfo stTemp = m_WeldRecipeParam.getStructureByName(qstrParamsList[dwIndex]);
        RecipeParameter* ptrTemp = new RecipeParameter;

        ptrTemp->m_ParameterName=qstrParamsList[dwIndex];
        ptrTemp->m_ParameterLocaleName = qstrParamsListLocale[dwIndex];
        ptrTemp->m_ParameterIndex = stTemp.ParamIndexInStructure;
        ptrTemp->m_ParameterUnit = stTemp.ParamUnit[m_Units];
        ptrTemp->m_ParameterMinValue = m_WeldRecipeParam.getMinValue(qstrParamsList[dwIndex]);
        ptrTemp->m_ParameterMaxValue = m_WeldRecipeParam.getMaxValue(qstrParamsList[dwIndex]);
        paramValue = stTemp.ParamDefaultValue[m_CurrentSystemFreq];

        if(ptrTemp->m_ParameterName == "Expected Contact Window")
        {
            ptrTemp->m_ParameterStripColor = "#645287";
        }
        else if((ptrTemp->m_ParameterName == "Expected Contact Offset") ||
                (ptrTemp->m_ParameterName == "Expected Part Contact"))
        {
            ptrTemp->m_ParameterStripColor = "#d26d19";
        }
        else if(ptrTemp->m_ParameterName == "Ready Offset")
        {
            ptrTemp->m_ParameterStripColor = "#8fa350";
        }
        else if(ptrTemp->m_ParameterName == "Weld Force")
        {
            ptrTemp->m_ParameterStripColor = "#68a4eb";
        }
        QStringList qstrSwitchParams;
        qstrSwitchParams    <<"Pretrigger"
                            <<"DistanceAuto"
                            <<"Afterburst"
                            <<"Extra Cooling"
                            <<"Timed Seek"
                            <<"Pre-Weld Seek"
                            <<"Post-Weld Seek"
                            <<"Control"
                            <<"Ground Detect Cutoff"
                            <<"Energy Brake"
                            <<"Setup"
							<<"Force Level Enable";

        if(qstrSwitchParams.contains(ptrTemp->m_ParameterName,Qt::CaseInsensitive))
        {
            ptrTemp->m_ParameterSwitch = true;
            int num = paramValue.toInt();
            ptrTemp->m_ParameterSwitchState = num;

            if(ptrTemp->m_ParameterName == "DistanceAuto"){
                ptrTemp->m_ParameterDoubleSwitch = true;
                ptrTemp->m_DoubleSwitchLeftText = "DISTANCE";
                ptrTemp->m_DoubleSwitchRightText = "AUTO";
            }
            else
                ptrTemp->m_ParameterDoubleSwitch = false;
        }
        else
        {
            ptrTemp->m_ParameterSwitch = false;
            ptrTemp->m_ParameterDoubleSwitch = false;
        }

        if(ptrTemp->m_ParameterName == "Weld Force Control")
            ptrTemp->m_ParameterTripleSwitch = true;
        else
            ptrTemp->m_ParameterTripleSwitch = false;

        if(ptrTemp->m_ParameterName == "Expected Contact Window")
        {
            paramValue = m_WeldRecipeParam.getStructureValueByName("part contact window minus") + ":" + m_WeldRecipeParam.getStructureValueByName("part contact window plus");
        }
        else if(ptrTemp->m_ParameterUnit == "s" )
        {
            paramValue.setNum(paramValue.toDouble(),'f',3);
            ptrTemp->m_ParameterMinValue.setNum(ptrTemp->m_ParameterMinValue.toDouble(),'f',3);
            ptrTemp->m_ParameterMaxValue.setNum(ptrTemp->m_ParameterMaxValue.toDouble(),'f',3);
        }
        else if(ptrTemp->m_ParameterUnit == "mm")
        {
            /*Min max precision display for Expected Part Contact keyboard*/
            if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                paramValue.setNum(paramValue.toDouble(),'f',2);
                ptrTemp->m_ParameterMinValue.setNum(ptrTemp->m_ParameterMinValue.toDouble(),'f',2);
                ptrTemp->m_ParameterMaxValue.setNum(ptrTemp->m_ParameterMaxValue.toDouble(),'f',2);
            }

            else
            {
                paramValue.setNum(paramValue.toDouble(),'f',3);
                ptrTemp->m_ParameterMinValue.setNum(ptrTemp->m_ParameterMinValue.toDouble(),'f',3);
                ptrTemp->m_ParameterMaxValue.setNum(ptrTemp->m_ParameterMaxValue.toDouble(),'f',3);
            }
        }
        else if(ptrTemp->m_ParameterUnit == "in")
        {
            paramValue.setNum(paramValue.toDouble(),'f',4);
            ptrTemp->m_ParameterMinValue.setNum(ptrTemp->m_ParameterMinValue.toDouble(),'f',4);
            ptrTemp->m_ParameterMaxValue.setNum(ptrTemp->m_ParameterMaxValue.toDouble(),'f',4);
        }
        /* To convert N to lbs, When Imperial units is selected on Recipe screen */
        else if(ptrTemp->m_ParameterUnit == "lbs")
        {
            double convertedValue = Utility::convertNtolbs(paramValue.toDouble());
            paramValue.setNum(convertedValue, 'f', 1);
        }
        ptrTemp->m_ParameterValue = paramValue;
        ptrModelList->append(ptrTemp);
    }
}

/**
 * @brief Recipe::createFixedParamModel
 */
void Recipe::createFixedParamModel()
{
    refreshModel(ENUM_DummyCurrentRecipeFixedSixParameter);
    QString qstrParams = getFixedParamsList();
    QString qstrLocaleParams = getLocaleFixedParamsList();
    createParamModel(&m_FixedParamModelList,qstrParams,qstrLocaleParams);
    refreshModel(ENUM_CurrentRecipeFixedSixParameter);
}

/**
 * @brief Recipe::createPreTriggerModel
 */
void Recipe::createPreTriggerModel()
{
    refreshModel(ENUM_DummyweldProcessPreTriggerModelList);
    QString qstrParams = getPreTriggerParamsList();
    QString qstrParamsLocale = getLocalePreTriggerParamsList();
    createParamModel(&m_PreTriggerModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_WeldProcessPreTriggerModelList);
}

/**
 * @brief Recipe::createEnergyBrakeModel
 */
void Recipe::createEnergyBrakeModel()
{
    refreshModel(ENUM_DummyweldProcessEnergyBrakeModelList);
    QString qstrParams = getEnergyBrakeParamsList();
    QString qstrParamsLocale = getLocaleEnergyBrakeParamsList();
    createParamModel(&m_EnergyBrakeModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_WeldProcessEnergyBrakeModelList);
}

/**
 * @brief Recipe::createAfterBurstModel
 */
void Recipe::createAfterBurstModel()
{
    refreshModel(ENUM_DummyweldProcessAfterBurstModelList);
    QString qstrParams = getAfterBurstParamsList();
    QString qstrParamsLocale = getLocaleAfterBurstParamsList();
    createParamModel(&m_AfterBurstModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_WeldProcessAfterBurstModelList);
}

/**
 * @brief Recipe::createAtoZParamModel
 */
void Recipe::createAtoZParamModel()
{
    refreshModel(ENUM_DummyParamAtoZ);
    QString qstrParams;
    QString qstrParamsLocale;
    getAtoZParamsMap();
    for(auto itr = 0;itr < paramAtoZList.size();itr++)
    {
        qstrParams.append(paramAtoZList[itr]);
        qstrParamsLocale.append(paramAtoZLocaleList[itr]);
        if(!(itr == paramAtoZList.size()-1))
        {
            qstrParams.append(",");
            qstrParamsLocale.append(",");
        }
    }
    createParamModel(&m_AtoZParamModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_ParamAtoZ);
}

/**
 * @brief Recipe::createSetupLimitsModel
 */
void Recipe::createSetupLimitsModel()
{
    refreshModel(ENUM_DummySetUpList);
    clearList(&m_setupList);
    QString setupLimits = getSetupLimitsParamsList();
    QStringList setuplist = setupLimits.split(",");
    QString setupLimitsLocale = getLocaleSetupLimitsParamsList();
    QStringList setuplistLocale = setupLimitsLocale.split(",");
    for(int dwIndex=0;dwIndex<setuplist.size();dwIndex++)
    {
        SetupLimits* setupLimitsList = new SetupLimits;
        QString qstrParamName = setuplist.at(dwIndex);
        QString qstrParamNameLocale = setuplistLocale.at(dwIndex);
        stRecipeParmInfo stTemp = m_WeldRecipeParam.getStructureByName("setup "+qstrParamName.toLower()+" low");

        setupLimitsList->m_ParameterName = qstrParamName;
        setupLimitsList->m_ParameterLocaleName = qstrParamNameLocale;
        setupLimitsList->m_SetUpEnabled  = m_WeldRecipeParam.getDefaultValue("setup "+qstrParamName.toLower()+" status").toInt();
        setupLimitsList->m_SetUpMinValue = m_WeldRecipeParam.getDefaultValue("setup "+qstrParamName.toLower()+" low").toInt();
        setupLimitsList->m_SetUpMaxValue = m_WeldRecipeParam.getDefaultValue("setup "+qstrParamName.toLower()+" high").toInt();

        setupLimitsList->m_SetupUnit = stTemp.ParamUnit[m_Units];
        setupLimitsList->m_SetUpWeld = false;
        setupLimitsList->m_ParameterRec = false;

        if(setupLimitsList->m_ParameterName == "Global Setup")
        {
            setupLimitsList->m_ParameterSwitch = true;
            setupLimitsList->m_ParameterSwitchState = false;
            setupLimitsList->m_ParameterRec = false;
            setupLimitsList->m_SetUpWeld = false;
        }
        else if(setupLimitsList->m_ParameterName == "Weld Mode")
        {
            setupLimitsList->m_SetUpWeld = true;
            setupLimitsList->m_ParameterRec = false;
            setupLimitsList->m_ParameterSwitch = false;
            setupLimitsList->m_ParameterSwitchState = false;
        }
        else
        {
            setupLimitsList->m_ParameterRec = true;
            setupLimitsList->m_SetUpWeld = false;
            setupLimitsList->m_ParameterSwitch = false;
            setupLimitsList->m_ParameterSwitchState = false;
        }
        setupLimitsList->m_WeldMode = "Time";
        setupLimitsList->m_SetUpParameterVisible = true;
        if(setupLimitsList->m_SetupUnit == "s" )
        {
            setupLimitsList->m_SetUpMinValue.setNum(setupLimitsList->m_SetUpMinValue.toDouble(),'f',3);
            setupLimitsList->m_SetUpMaxValue.setNum(setupLimitsList->m_SetUpMaxValue.toDouble(),'f',3);
        }
        else if(setupLimitsList->m_SetupUnit == "mm" )
        {
            /*Min max precision display for Expected Contact Offset keyboard*/
            if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                setupLimitsList->m_SetUpMinValue.setNum(setupLimitsList->m_SetUpMinValue.toDouble(),'f',2);
                setupLimitsList->m_SetUpMaxValue.setNum(setupLimitsList->m_SetUpMaxValue.toDouble(),'f',2);
            }

            else
            {
                setupLimitsList->m_SetUpMinValue.setNum(setupLimitsList->m_SetUpMinValue.toDouble(),'f',3);
                setupLimitsList->m_SetUpMaxValue.setNum(setupLimitsList->m_SetUpMaxValue.toDouble(),'f',3);
            }
        }
        else if(setupLimitsList->m_SetupUnit == "in" )
        {
            setupLimitsList->m_SetUpMinValue.setNum(setupLimitsList->m_SetUpMinValue.toDouble(),'f',4);
            setupLimitsList->m_SetUpMaxValue.setNum(setupLimitsList->m_SetUpMaxValue.toDouble(),'f',4);
        }
        else if(setupLimitsList->m_SetupUnit == "lbs" )
        {
            setupLimitsList->m_SetUpMinValue.setNum(setupLimitsList->m_SetUpMinValue.toDouble(),'f',1);
            setupLimitsList->m_SetUpMaxValue.setNum(setupLimitsList->m_SetUpMaxValue.toDouble(),'f',1);
        }
        m_setupList.append(setupLimitsList);
    }
    refreshModel(ENUM_SetUplist);
}

/**
 * @brief Recipe::createControlLimitsModel
 */
void Recipe::createControlLimitsModel()
{
    refreshModel(ENUM_DummyControlList);
    QString qstrParams = getControlLimitsParamsList();
    QString qstrParamsLocale = getLocaleControlLimitsParamsList();
    createParamModel(&m_ControlLimitsModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_ControlList);
}

/**
 * @brief Recipe::createSuspectRejectLimitsModel
 */
void Recipe::createSuspectRejectLimitsModel()
{
    refreshModel(ENUM_DummySuspectRecjectParamList);
    clearList(&m_suspectRejectParamList);
    QString suspectRejectLimits = getSuspectRejectLimitsParamsList();
    QStringList suspectRejectlist =suspectRejectLimits.split(",");
    QString suspectRejectLimitsLocale = getLocaleSuspectRejectLimitsParamsList();
    QStringList suspectRejectlistLocale = suspectRejectLimitsLocale.split(",");
    for(int dwIndex=0;dwIndex<suspectRejectlist.size();dwIndex++)
    {
        SuspectRejectParameter* suspectRejectLimitsList = new SuspectRejectParameter;
        QString qstrParamName = suspectRejectlist.at(dwIndex);
        QString qstrParamNameLocale = suspectRejectlistLocale.at(dwIndex);
        /* To read system unit stored in suspect reject QMAP */
        stRecipeParmInfo stTemp = m_WeldRecipeParam.getStructureByName("suspect "+qstrParamName.toLower()+" low value");

        suspectRejectLimitsList->m_ParameterUnit = stTemp.ParamUnit[m_Units];
        suspectRejectLimitsList->m_parameterName = qstrParamName;
        suspectRejectLimitsList->m_parameterLocaleName = qstrParamNameLocale;
        suspectRejectLimitsList->m_suspectStatus = m_WeldRecipeParam.getDefaultValue("suspect "+qstrParamName.toLower()).toInt();
        suspectRejectLimitsList->m_suspectLowStatus = m_WeldRecipeParam.getDefaultValue("suspect "+qstrParamName.toLower()+" low").toInt();
        suspectRejectLimitsList->m_suspectHighStatus = m_WeldRecipeParam.getDefaultValue("suspect "+qstrParamName.toLower()+" high").toInt();
        suspectRejectLimitsList->m_suspectLow = m_WeldRecipeParam.getDefaultValue("suspect "+qstrParamName.toLower()+" low value").toInt();
        suspectRejectLimitsList->m_suspectHigh = m_WeldRecipeParam.getDefaultValue("suspect "+qstrParamName.toLower()+" high value").toInt();
        suspectRejectLimitsList->m_rejectStatus = m_WeldRecipeParam.getDefaultValue("reject "+qstrParamName.toLower()).toInt();
        suspectRejectLimitsList->m_rejectLowStatus = m_WeldRecipeParam.getDefaultValue("reject "+qstrParamName.toLower()+" low").toInt();
        suspectRejectLimitsList->m_rejectHighStatus = m_WeldRecipeParam.getDefaultValue("reject "+qstrParamName.toLower()+" High").toInt();
        suspectRejectLimitsList->m_rejectLow = m_WeldRecipeParam.getDefaultValue("reject "+qstrParamName.toLower()+" low value").toInt();
        suspectRejectLimitsList->m_rejectHigh = m_WeldRecipeParam.getDefaultValue("reject "+qstrParamName.toLower()+" High value").toInt();

        if((suspectRejectLimitsList->m_parameterName == "Global Suspect") ||
                (suspectRejectLimitsList->m_parameterName == "Global Reject"))
        {
            suspectRejectLimitsList->m_ParameterSwitch = true;
            suspectRejectLimitsList->m_ParameterSwitchState = false;
        }
        else
        {
            suspectRejectLimitsList->m_ParameterSwitch = false;
        }

        m_suspectRejectParamList.append(suspectRejectLimitsList);
    }
    refreshModel(ENUM_SuspectRejectParamList);
}

/**
 * @brief Recipe::createBransonParamsModel
 */
void Recipe::createBransonParamsModel()
{
    refreshModel(ENUM_DUMMYBransonOnlyList);
    QString qstrParams = getBransonParamsList();
    QString qstrParamsLocale = getLocaleBransonParamsList();
    createParamModel(&m_BransonParamsModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_BransonOnlyList);
}

/**
 * @brief Recipe::createActuatorSetupParamsModel
 */
void Recipe::createActuatorSetupParamsModel()
{
    refreshModel(ENUM_DummyActuatorSetUpdata);
    QString qstrParams = getActuatorSetupParamsList();
    QString qstrParamsLocale = getLocaleActuatorSetupParamsList();
    createParamModel(&m_ActuatorSetupParamsModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_ActuatorSetUpdata);
}

/**
 * @brief Recipe::createActuatorSetupAdvancedParamsModel
 */
void Recipe::createActuatorSetupAdvancedParamsModel()
{
    refreshModel(ENUM_DummyActuatorSetUpAdvanceddata);
    QString qstrParams = getActuatorSetupAdvancedParamsList();
    QString qstrParamsLocale = getLocaleActuatorSetupAdvancedParamsList();
    createParamModel(&m_ActuatorSetupAdvancedParamsModelList,qstrParams,qstrParamsLocale);
    refreshModel(ENUM_ActuatorSetUpAdvanceddata);
}

/**
 * @brief Recipe::loadAllModels
 */
void Recipe::loadAllModels()
{
    loadFixedParamModel();
    loadPreTriggerModel();
    loadEnergyBrakeModel();
    loadAfterBurstModel();
    loadAtoZParamModel();
    loadSetupLimitsModel();
    loadControlLimitsModel();
    loadSuspectRejectLimitsModel();
    loadBransonParamsModel();
    loadActatorSetupParamsModel();
    loadActatorSetupAdvancedParamsModel();
}

/**
 * @brief Recipe::LoadParamVal
 * @param ptrModelList
 */
void Recipe::LoadParamVal(QList<QObject*> *ptrModelList)
{
    int dwParamCnt = ptrModelList->size();
    for(int i=0;i<dwParamCnt;i++)
    {
        RecipeParameter* ptrTemp = dynamic_cast<RecipeParameter *>(ptrModelList->at(i));
        if(ptrTemp->m_ParameterName == "Expected Contact Window")
        {
            ptrTemp->m_ParameterValue = m_WeldRecipeParam.getStructureValueByName("part contact window minus") + ":" + m_WeldRecipeParam.getStructureValueByName("part contact window plus");
        }
        else if((ptrTemp->m_ParameterName.toLower() == "weld force") && m_WeldRecipeParam.bIsWeldForceChanged == true)
        {
            ptrTemp->m_ParameterValue = m_WeldRecipeParam.getActuatorRecipeStructureValue();
        }
        else
            ptrTemp->m_ParameterValue = m_WeldRecipeParam.getStructureValueByName(ptrTemp->m_ParameterName);
        if(ptrTemp->m_ParameterSwitch)
            ptrTemp->m_ParameterSwitchState = ptrTemp->m_ParameterValue.toInt();
        if(ptrTemp->m_ParameterDoubleSwitch)
            ptrTemp->m_ParameterSwitchState = ptrTemp->m_ParameterValue.toInt();
        if(ptrTemp->m_ParameterName.toLower() == "ready offset")
        {
            ptrTemp->m_ParameterMaxValue = m_WeldRecipeParam.getMaxValue("ready offset");
        }
        if(ptrTemp->m_ParameterName.toLower() == "weld force")
        {
            ptrTemp->m_ParameterMinValue = m_WeldRecipeParam.getMinValue("weld force");
        }
        if(ptrTemp->m_ParameterName.toLower() == "trigger force")
        {
            ptrTemp->m_ParameterMaxValue = m_WeldRecipeParam.getStructureValueByName("weld force");
        }
    }
}

/**
 * @brief Recipe::getSelectedIndex
 * @return
 */
int Recipe::getSelectedIndex()
{
    int currentIndex=0;
    int dwSelectedMode = m_WeldRecipeParam.getStructureValueByIndex(SC_WELD_MODE).toInt();
    QString qstrSupportModes = getWeldModesListIndexes();
    if(qstrSupportModes == "")
        return currentIndex;
    QStringList qstrlstModes = qstrSupportModes.split(",");
    for(int dwIndex=0;dwIndex<qstrlstModes.length();dwIndex++)
    {
        if(qstrlstModes[dwIndex].toInt() == dwSelectedMode)
        {
            currentIndex = dwIndex;
        }
    }
    return currentIndex;
}

/**
 * @brief Recipe::loadFixedParamModel
 */
void Recipe::loadFixedParamModel()
{
    refreshModel(ENUM_DummyCurrentRecipeFixedSixParameter);
    LoadParamVal(&m_FixedParamModelList);

    RecipeParameter* ptrTemp = dynamic_cast<RecipeParameter *>(m_FixedParamModelList.at(0));
    //Pointer containing Reactivity parameter object
    RecipeParameter* ptrReactivityTemp = dynamic_cast<RecipeParameter *>(m_FixedParamModelList.at(7));
    //Pointer containing ForceLevel parameter object
    RecipeParameter* ptrForceLevelTemp = dynamic_cast<RecipeParameter *>(m_FixedParamModelList.at(8));
    int dwForceLevelEnable = m_WeldRecipeParam.getStructureValueByIndex(SC_FORCE_LEVEL_ENABLED).toInt();
    int dwWeldMode = m_WeldRecipeParam.getStructureValueByIndex(SC_WELD_MODE).toInt();
    QString dwModeValue = m_WeldRecipeParam.getStructureValueByIndex(SC_MODE_VALUE);
    ModeInfo stModeInfo = m_WeldRecipeParam.getModeInfoByIndex(dwWeldMode,dwModeValue);
    ptrTemp->m_ParameterName = stModeInfo.ModeName;
    ptrTemp->m_ParameterLocaleName = stModeInfo.ModeLocaleName;
    ptrTemp->m_ParameterValue = stModeInfo.ModeValue;
    ptrTemp->m_ParameterUnit = stModeInfo.ModeUnit;
    ptrTemp->m_ParameterMinValue = m_WeldRecipeParam.getMinValue(ptrTemp->m_ParameterName);
    ptrTemp->m_ParameterMaxValue = m_WeldRecipeParam.getMaxValue(ptrTemp->m_ParameterName);
    //Shows the Reactivity parameter just when the Dynamic mode is selected
    //and the Force Level parameter when the Force Level is enabled
    if ("Density" != stModeInfo.ModeName){
         ptrReactivityTemp->m_ParameterVisible = false;
         ptrForceLevelTemp->m_ParameterVisible = false;
    }
    else if(false == dwForceLevelEnable){
        ptrReactivityTemp->m_ParameterVisible = true;
        ptrForceLevelTemp->m_ParameterVisible = false;
    }
    else{
         ptrReactivityTemp->m_ParameterVisible = true;
         ptrForceLevelTemp->m_ParameterVisible = true;
    }
    refreshModel(ENUM_CurrentRecipeFixedSixParameter);
}

/**
 * @brief Recipe::loadPreTriggerModel
 */
void Recipe::loadPreTriggerModel()
{
    refreshModel(ENUM_DummyweldProcessPreTriggerModelList);
    LoadParamVal(&m_PreTriggerModelList);
    refreshModel(ENUM_WeldProcessPreTriggerModelList);
}

/**
 * @brief Recipe::loadEnergyBrakeModel
 */
void Recipe::loadEnergyBrakeModel()
{
    refreshModel(ENUM_DummyweldProcessEnergyBrakeModelList);
    LoadParamVal(&m_EnergyBrakeModelList);
    refreshModel(ENUM_WeldProcessEnergyBrakeModelList);
}

/**
 * @brief Recipe::loadAfterBurstModel
 */
void Recipe::loadAfterBurstModel()
{
    refreshModel(ENUM_DummyweldProcessAfterBurstModelList);
    LoadParamVal(&m_AfterBurstModelList);
    refreshModel(ENUM_WeldProcessAfterBurstModelList);
}

/**
 * @brief Recipe::loadAtoZParamModel
 */
void Recipe::loadAtoZParamModel()
{
    refreshModel(ENUM_DummyParamAtoZ);
    LoadParamVal(&m_AtoZParamModelList);
    refreshModel(ENUM_ParamAtoZ);
}

/**
 * @brief Recipe::loadSetupLimitsModel
 */
void Recipe::loadSetupLimitsModel()
{
    int dwForceLevelEnable = m_WeldRecipeParam.getStructureValueByIndex(SC_FORCE_LEVEL_ENABLED).toInt();

    refreshModel(ENUM_DummySetUpList);
    for(int dwIndex=0;dwIndex<m_setupList.size();dwIndex++)
    {
        SetupLimits* setupLimitsList = dynamic_cast<SetupLimits *>(m_setupList.at(dwIndex));
        QString qstrParamName = setupLimitsList->m_ParameterName;
        int mode;
        mode = m_WeldRecipeParam.getStructureValueByIndex(SC_WELD_MODE).toInt();
        ModeInfo stModeInfo = m_WeldRecipeParam.getModeInfoByIndex(mode,"");
        if(qstrParamName.toLower() == "global setup")
        {
            setupLimitsList->m_ParameterSwitchState = m_WeldRecipeParam.getStructureValueByName(qstrParamName.toLower()).toInt();
        }
        else
        {
            if(qstrParamName.toLower() == "weld mode")
            {
                setupLimitsList->m_ParameterLocaleName = stModeInfo.ModeLocaleName;
                setupLimitsList->m_WeldMode = stModeInfo.ModeName;
                setupLimitsList->m_SetupUnit = stModeInfo.ModeUnit;
            }
            else
            {
                setupLimitsList->m_WeldMode = "";
            }
            setupLimitsList->m_SetUpMinValue = m_WeldRecipeParam.getStructureValueByName("setup "+qstrParamName.toLower()+" low");
            setupLimitsList->m_SetUpMaxValue = m_WeldRecipeParam.getStructureValueByName("setup "+qstrParamName.toLower()+" high");
            setupLimitsList->m_SetUpEnabled  = m_WeldRecipeParam.getStructureValueByName("setup "+qstrParamName.toLower()+" status").toInt();
        }

        if("reactivity" == qstrParamName.toLower())
        {
            if("Density" == stModeInfo.ModeName)
                setupLimitsList->m_SetUpParameterVisible = true;
            else
                setupLimitsList->m_SetUpParameterVisible = false;
        }else if ("force level" == qstrParamName.toLower())
        {
            if(("Density" == stModeInfo.ModeName) && (true == dwForceLevelEnable))
                setupLimitsList->m_SetUpParameterVisible = true;
            else
                setupLimitsList->m_SetUpParameterVisible = false;
        }else
        {
            setupLimitsList->m_SetUpParameterVisible = true;
        }


    }
    refreshModel(ENUM_SetUplist);
}

/**
 * @brief Recipe::loadControlLimitsModel
 */
void Recipe::loadControlLimitsModel()
{
    refreshModel(ENUM_DummyControlList);
    LoadParamVal(&m_ControlLimitsModelList);
    refreshModel(ENUM_ControlList);
}

void Recipe::loadSuspectRejectLimitsModel()
{
    refreshModel(ENUM_DummySuspectRecjectParamList);
    for(int dwIndex=0;dwIndex<m_suspectRejectParamList.size();dwIndex++)
    {
        SuspectRejectParameter* suspectRejectLimitsList = dynamic_cast<SuspectRejectParameter *>(m_suspectRejectParamList.at(dwIndex));
        QString qstrParamName = suspectRejectLimitsList->m_parameterName;
        if(qstrParamName.toLower() == "global suspect" || qstrParamName.toLower() == "global reject")
        {
            suspectRejectLimitsList->m_ParameterSwitchState = m_WeldRecipeParam.getStructureValueByName(qstrParamName.toLower()).toInt();
        }
        else
        {
            suspectRejectLimitsList->m_suspectStatus = m_WeldRecipeParam.getStructureValueByName("suspect "+qstrParamName.toLower()).toInt();
            suspectRejectLimitsList->m_suspectLowStatus = m_WeldRecipeParam.getStructureValueByName("suspect "+qstrParamName.toLower()+" low").toInt();
            suspectRejectLimitsList->m_suspectHighStatus = m_WeldRecipeParam.getStructureValueByName("suspect "+qstrParamName.toLower()+" high").toInt();
            suspectRejectLimitsList->m_suspectLow = m_WeldRecipeParam.getStructureValueByName("suspect "+qstrParamName.toLower()+" low value");
            suspectRejectLimitsList->m_suspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect "+qstrParamName.toLower()+" high value");
            suspectRejectLimitsList->m_rejectStatus = m_WeldRecipeParam.getStructureValueByName("reject "+qstrParamName.toLower()).toInt();
            suspectRejectLimitsList->m_rejectLowStatus = m_WeldRecipeParam.getStructureValueByName("reject "+qstrParamName.toLower()+" low").toInt();
            suspectRejectLimitsList->m_rejectHighStatus = m_WeldRecipeParam.getStructureValueByName("reject "+qstrParamName.toLower()+" High").toInt();
            suspectRejectLimitsList->m_rejectLow = m_WeldRecipeParam.getStructureValueByName("reject "+qstrParamName.toLower()+" low value");
            suspectRejectLimitsList->m_rejectHigh = m_WeldRecipeParam.getStructureValueByName("reject "+qstrParamName.toLower()+" High value");
        }
    }
    refreshModel(ENUM_SuspectRejectParamList);
}

/**
 * @brief Recipe::loadBransonParamsModel
 */
void Recipe::loadBransonParamsModel()
{
    refreshModel(ENUM_DUMMYBransonOnlyList);
    LoadParamVal(&m_BransonParamsModelList);
    refreshModel(ENUM_BransonOnlyList);
}

/**
 * @brief Recipe::loadActatorSetupParamsModel
 */
void Recipe::loadActatorSetupParamsModel()
{
    refreshModel(ENUM_DummyActuatorSetUpdata);
    LoadParamVal(&m_ActuatorSetupParamsModelList);
    refreshModel(ENUM_ActuatorSetUpdata);
}

/**
 * @brief Recipe::loadActatorSetupAdvancedParamsModel
 */
void Recipe::loadActatorSetupAdvancedParamsModel()
{
    refreshModel(ENUM_DummyActuatorSetUpAdvanceddata);
    LoadParamVal(&m_ActuatorSetupAdvancedParamsModelList);
    refreshModel(ENUM_ActuatorSetUpAdvanceddata);
}

/**
 * @brief Recipe::refreshModel
 * @param modelIndexNum
 */
void Recipe::refreshModel(int modelIndexNum)
{
    switch (modelIndexNum)
    {
    case ENUM_CurrentRecipeFixedSixParameter:
        context->setContextProperty("appModel", QVariant::fromValue(m_FixedParamModelList));
        break;
    case ENUM_DummyCurrentRecipeFixedSixParameter:
        context->setContextProperty("appModel", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_DummyNewRecipeFixedParameter:
        context->setContextProperty("addModel", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_ParamAtoZ:
        context->setContextProperty("modelParamAtoZ", QVariant::fromValue(m_AtoZParamModelList));
        break;
    case ENUM_DummyParamAtoZ:
        context->setContextProperty("modelParamAtoZ", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_WeldResults:
        context->setContextProperty("weldResultsModel", QVariant::fromValue(weldResults));
        break;
    case ENUM_DummyWeldResults:
        context->setContextProperty("weldResultsModel", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_MenuList:
        context->setContextProperty("modelLimitsMenu", QVariant::fromValue(m_menuList));
        break;
    case ENUM_WeldMode:
        context->setContextProperty("modelRecipeWeldMode", QVariant::fromValue(m_weldmode));
        break;
    case ENUM_WeldMode_DUMMY:
        context->setContextProperty("modelRecipeWeldMode", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_SteppingModes:
        context->setContextProperty("modelSteppingMode", QVariant::fromValue(m_steppingmodes));
        break;
    case ENUM_DummySteppingModes:
        context->setContextProperty("modelSteppingMode", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_SuspectRejectParamList:
        context->setContextProperty("modelSuspectRejectLimits", QVariant::fromValue(m_suspectRejectParamList));
        break;
    case ENUM_DummySuspectRecjectParamList:
        context->setContextProperty("modelSuspectRejectLimits", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_ActuatorSetUpdata:
        context->setContextProperty("setupModel", QVariant::fromValue(m_ActuatorSetupParamsModelList));
        break;
    case ENUM_DummyActuatorSetUpdata:
        context->setContextProperty("setupModel", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_ActuatorSetUpAdvanceddata:
        context->setContextProperty("setupAdvancedModel", QVariant::fromValue(m_ActuatorSetupAdvancedParamsModelList));
        break;
    case ENUM_DummyActuatorSetUpAdvanceddata:
        context->setContextProperty("setupAdvancedModel", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_PressureList:

        break;
    case ENUM_DummyPressureList:

        break;
    case ENUM_ControlList:
        context->setContextProperty("modelControlLimits", QVariant::fromValue(m_ControlLimitsModelList));
        break;
    case ENUM_DummyControlList:
        context->setContextProperty("modelControlLimits", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_SetUplist:
        context->setContextProperty("modelSetupLimits", QVariant::fromValue(m_setupList));
        break;
    case ENUM_DummySetUpList:
        context->setContextProperty("modelSetupLimits", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_RecipeModelList:
        context->setContextProperty("recipeModel", QVariant::fromValue(m_RecipeModelList));
        break;
    case ENUM_DummyRecipeModelList:
        context->setContextProperty("recipeModel",QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_WeldProcessModelList:
        context->setContextProperty("modelWeldProcessMenu", QVariant::fromValue(m_weldProcessModelList));
        break;
    case ENUM_DummyWeldProcessModelList:
        context->setContextProperty("modelWeldProcessMenu", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_WeldProcessAfterBurstModelList:
        context->setContextProperty("modelAfterBurstParams", QVariant::fromValue(m_AfterBurstModelList));
        break;
    case ENUM_DummyweldProcessAfterBurstModelList:
        context->setContextProperty("modelAfterBurstParams", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_WeldProcessPreTriggerModelList:
        context->setContextProperty("modelPreTriggerParams", QVariant::fromValue(m_PreTriggerModelList));
        break;
    case ENUM_DummyweldProcessPreTriggerModelList:
        context->setContextProperty("modelPreTriggerParams", QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_WeldProcessEnergyBrakeModelList:
        context->setContextProperty("modelEnergyBrakeParams",QVariant::fromValue(m_EnergyBrakeModelList));
        break;
    case ENUM_DummyweldProcessEnergyBrakeModelList:
        context->setContextProperty("modelEnergyBrakeParams",QVariant::fromValue(m_DummyModelList));
        break;
    case ENUM_BransonOnlyList:
        context->setContextProperty("bransonOnlyModel",QVariant::fromValue(m_BransonParamsModelList));
        break;
    case ENUM_DUMMYBransonOnlyList:
        context->setContextProperty("bransonOnlyModel",QVariant::fromValue(m_DummyModelList));
        break;
    default:
        break;
    }
}

/**
 * @brief Recipe::changeWeldMode
 * @param weldmode
 */
void Recipe::changeWeldMode(QString weldmode)
{
    int dwRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
    int dwRecipeVerNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER).toInt();

    if(dwRecipeNumber != 0)
    {
        m_WeldRecipeParam.setStructureVariableByIndex(UI_RECIPE_VERSION_NUMBER,dwRecipeVerNumber);
        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER,dwRecipeNumber);
        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_NUMBER,0);

        m_WeldRecipeParam.m_ParamaterChangedFlag = SEND_ALL_PARAM;
        Configuration::getInstance()->setConigValues("recipezero","recipe_number",QString::number(dwRecipeNumber));
        Configuration::getInstance()->setConigValues("recipezero","recipe_version_number",QString::number(dwRecipeVerNumber));
    }
    setParamaterChangedFlag(0);
    m_WeldRecipeParam.setModeInfoByName(weldmode);
    // Setup Limits dynamic change
    m_WeldRecipeParam.setStructureVariableByName("setup weld mode low",m_WeldRecipeParam.getMinValue(weldmode.toLower()));
    m_WeldRecipeParam.setStructureVariableByName("setup weld mode high",m_WeldRecipeParam.getMaxValue(weldmode.toLower()));

    m_WeldRecipeParam.storeRecipeToDB();
    setRecipeModifiedState(true);
    m_WeldRecipeParam.sendRecipeToSC();
    loadFixedParamModel();
    loadSetupLimitsModel();
}


/**
 * @brief Recipe::clearList
 * @param tempList
 */
void Recipe::clearList(QList<QObject*> *tempList)
{
    for(int element=0; element <tempList->size() ; element++)
    {
        QObject* ptrTemp = dynamic_cast<SetupParameter*>(tempList->at(element));
        delete ptrTemp;
        ptrTemp = NULL;
    }
    tempList->clear();
}

/**
 * @brief Recipe::clearWeldResultList
 * @param tempList
 */
void Recipe::clearWeldResultList(QList<QObject*> *tempList)
{
    for(int element=0; element <tempList->size() ; element++)
    {
        WeldResultData* ptrTemp = dynamic_cast<WeldResultData*>(tempList->at(element));
        delete ptrTemp;
        ptrTemp = NULL;
    }
    tempList->clear();
}

/**
 * @brief Recipe::clearWeldResultTableList
 * @param tempList
 */
void Recipe::clearWeldResultTableList(QList<QObject*> *tempList)
{
    for(int element=0; element <tempList->size() ; element++)
    {
        TableRowData* ptrTemp = dynamic_cast<TableRowData*>(tempList->at(element));
        delete ptrTemp;
        ptrTemp = NULL;
    }
    tempList->clear();
}

