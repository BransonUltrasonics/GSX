/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek Recipe Data

***************************************************************************/
#ifndef HORNRECIPEDATA_H
#include "Header/seekrecipedata.h"
#include "Header/common.h"
#endif

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#include<QFile>
#include <QQmlContext>
#include <QSettings>
using namespace std ;

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/* In the future the extern implementation of sysconfig data to remove and read from global class */
extern unsigned int psFreqGlobalVar;

/**
 * @brief SeekRecipeParameter::SeekRecipeParameter : Initialization of class members and memory for object
 * @param a_parameterName
 * @param a_parameterVal
 * @param a_parameterUnit
 * @param a_parameterMinVal
 * @param a_parameterMaxVal
 * @param isSwitch
 * @param switchState
 */
SeekRecipeParameter :: SeekRecipeParameter(const QString &a_parameterName, const QString &a_parameterLocaleName, const QString &a_parameterVal,
                                           const QString &a_parameterUnit,const QString &a_parameterMinVal,
                                           const QString &a_parameterMaxVal,bool isSwitch,
                                           bool switchState)
{
    m_parameterName = a_parameterName;
    m_parameterLocaleName = a_parameterLocaleName;
    m_parameterValue = a_parameterVal;
    m_parameterUnit = a_parameterUnit;
    m_parameterMinValue = a_parameterMinVal;
    m_parameterMaxValue = a_parameterMaxVal;
    m_parameterSwitch = isSwitch;
    m_parameterSwitchState = switchState;
}

/**
 * @brief SeekRecipeParameter::SeekRecipeParameter
 * @param parent
 */
SeekRecipeParameter::SeekRecipeParameter(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief SeekRecipeParameter::~SeekRecipeParameter
 */
SeekRecipeParameter :: ~SeekRecipeParameter()
{
}

/**
 * @brief SeekRecipeParameter::getParameterName : Return the name
 * @return
 */
QString SeekRecipeParameter :: getParameterName()
{
    return m_parameterName;
}

/**
 * @brief SeekRecipeParameter::getParameterLocaleName : Return the name
 * @return
 */
QString SeekRecipeParameter :: getParameterLocaleName()
{
    return m_parameterLocaleName;
}

/**
 * @brief SeekRecipeParameter::getParameterUnit : Return the unit
 * @return
 */
QString SeekRecipeParameter :: getParameterUnit()
{
    return m_parameterUnit;
}

/**
 * @brief SeekRecipeParameter::getParameterValue : Return the value
 * @return
 */
QString SeekRecipeParameter :: getParameterValue()
{
    return m_parameterValue ;
}

/**
 * @brief SeekRecipeParameter::getParameterMinValue : Return minimum value
 * @return
 */
QString SeekRecipeParameter :: getParameterMinValue()
{
    return m_parameterMinValue;
}

/**
 * @brief SeekRecipeParameter::getParameterMaxValue : Return maximum value
 * @return
 */
QString SeekRecipeParameter :: getParameterMaxValue()
{
    return m_parameterMaxValue;
}

/**
 * @brief SeekRecipeParameter::getParameterSwitch : Return the switch enabled
 * @return
 */
bool SeekRecipeParameter :: getParameterSwitch()
{
    return m_parameterSwitch;
}

/**
 * @brief SeekRecipeParameter::getParameterSwitchState : Return switch state
 * @return
 */
bool SeekRecipeParameter :: getParameterSwitchState()
{
    return m_parameterSwitchState;
}

/**
 * @brief HornRecipeParameter::setParameterValue
 * @param a_val
 */
void SeekRecipeParameter :: setParameterValue(QString a_val)
{
    m_parameterValue = a_val;
}

/**
 * @brief SeekRecipeData::SeekRecipeData
 */
SeekRecipeData :: SeekRecipeData()
{
    initializeSeekRecipeData();
    m_CurrentSystemFreq = SYSTEM_TYPE_20KHZ;
    m_MetrixUnitSelected = UNIT_METRIC;
    getDefaultFrequencySelectionForSeek();
    setSeekCheckBoxModelData();
}

/**
 * @brief SeekRecipeData::setDefaultSystemValuesForSeek : Set Default frequency and unit
 * @param dwMetrixUnit
 * @param psFreqGlobalVar
 */
void SeekRecipeData::setDefaultSystemValuesForSeek(int dwMetrixUnit, int psFreqGlobalVar)
{
    m_MetrixUnitSelected = static_cast<Units>(dwMetrixUnit);
    if(psFreqGlobalVar == FrequencySelected20KHZ)
    {
        m_CurrentSystemFreq=SYSTEM_TYPE_20KHZ;
    }
    else if(psFreqGlobalVar == FrequencySelected30KHZ)
    {
        m_CurrentSystemFreq=SYSTEM_TYPE_30KHZ;
    }
    else
    {
        m_CurrentSystemFreq=SYSTEM_TYPE_40KHZ;
    }
    getDefaultFrequencySelectionForSeek();
    updateDefaultParams();
}

/**
 * @brief SeekRecipeData::getDefaultFrequencySelectionForSeek : Get default name frequency and unit
 */
void SeekRecipeData :: getDefaultFrequencySelectionForSeek()
{
    seekRecipe.SeekTime = m_MapSeekRecipeParams["seek time"].ParamDefaultValue[m_CurrentSystemFreq].toDouble()*1000;
    seekRecipe.SeekRampTime = m_MapSeekRecipeParams["seek ramp time"].ParamDefaultValue[m_CurrentSystemFreq].toDouble()*1000;
    seekRecipe.SeekFreqOffset = m_MapSeekRecipeParams["seek frequency offset"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipe.SeekMemClearbeforeSeek = m_MapSeekRecipeParams["memory clear before seek"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipe.SeekAmplitudeLoopC1 = m_MapSeekRecipeParams["seek amplitude loop c1"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipe.SeekAmplitudeLoopC2= m_MapSeekRecipeParams["seek amplitude loop c2"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipe.SeekFreqLimitHigh= m_MapSeekRecipeParams["seek frequency limit high"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipe.SeekFreqLimitLow = m_MapSeekRecipeParams["seek frequency limit low"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipe.SeekPhaseloopI = m_MapSeekRecipeParams["seek phase loop i"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipe.SeekPhaseloopCF = m_MapSeekRecipeParams["seek phase loop cf"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
}

/**
 * @brief SeekRecipeData::initializeSeekRecipeData
 */
void SeekRecipeData :: initializeSeekRecipeData()
{
    stRecipeParmInfo stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_TIME;
    stTempParam.ParamUnit[UNIT_METRIC]  = SECONDS;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = SECONDS;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "0.5";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "0.001";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "0.5";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "0.001";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "0.5";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "0.001";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1";
    m_MapSeekRecipeParams["seek time"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_RAMP_TIME;
    stTempParam.ParamUnit[UNIT_METRIC]  = SECONDS;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = SECONDS;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "0.08";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "0.01";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "0.08";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "0.01";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "0.08";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "0.01";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1";
    m_MapSeekRecipeParams["seek ramp time"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_FREQ_OFFSET;
    stTempParam.ParamUnit[UNIT_METRIC]  = HERTZ;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = HERTZ;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "0";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "0";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "750";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "0";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1000";
    m_MapSeekRecipeParams["seek frequency offset"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_MEMORY_CLEAR_BEFORE_SEEK;
    stTempParam.ParamUnit[UNIT_METRIC]  = " ";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = " ";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "0";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "0";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "0";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1";
    m_MapSeekRecipeParams["memory clear before seek"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_AMP_LOOPC1;
    stTempParam.ParamUnit[UNIT_METRIC]  = " ";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = " ";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "100";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "205";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "205";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "20000";
    m_MapSeekRecipeParams["seek amplitude loop c1"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_AMP_LOOPC2;
    stTempParam.ParamUnit[UNIT_METRIC]  = " ";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = " ";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "300";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "370";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "370";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "20000";
    m_MapSeekRecipeParams["seek amplitude loop c2"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_FREQ_LIMIT_HIGH;
    stTempParam.ParamUnit[UNIT_METRIC]  = HERTZ;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = HERTZ;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "750";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "1000";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1000";
    m_MapSeekRecipeParams["seek frequency limit high"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_FREQ_LIMIT_LOW;
    stTempParam.ParamUnit[UNIT_METRIC]  = HERTZ;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = HERTZ;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "750";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "1000";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1000";
    m_MapSeekRecipeParams["seek frequency limit low"] = stTempParam;


    stTempParam.ParamIndexInStructure = SEEK_PHASE_LOOPI;
    stTempParam.ParamUnit[UNIT_METRIC]  = " ";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = " ";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "450";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "100";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "60";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "500";
    m_MapSeekRecipeParams["seek phase loop i"] = stTempParam;

    stTempParam.ParamIndexInStructure = SEEK_PHASE_LOOPCF;
    stTempParam.ParamUnit[UNIT_METRIC]  = " ";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = " ";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "450";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "200";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "200";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "500";
    m_MapSeekRecipeParams["seek phase loop cf"] = stTempParam;
}

/**
 * @brief SeekRecipeData::init
 * @return
 */
int SeekRecipeData :: init()
{
    int retValLoadRecipe = OPERATION_SUCCESS ;
    m_psFreq = psFreqGlobalVar ;
    updateDefaultParams();
    retValLoadRecipe = reqToLoadRecipeData();
    if( retValLoadRecipe != OPERATION_SUCCESS)
    {
        resetSeekRecipeData();
    }
    else
    {
        createSeekRecipe();
    }
    return retValLoadRecipe ;
}

/**
 * @brief SeekRecipeData::getSeekRecipeParameterNames : Parameter names
 * @return
 */
QString SeekRecipeData :: getSeekRecipeParameterNames()
{
    QString paramNames;
    paramNames.append(SEEK_RECP_TIME);
    paramNames.append(COMMA);
    paramNames.append(SEEK_RECP_RAMP);
    paramNames.append(COMMA);
    paramNames.append(SEEK_RECP_OFFSET);
    paramNames.append(COMMA);
    paramNames.append(SEEK_RECP_MEMORY);

    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        paramNames.append(",Seek Amplitude Loop C1,"
                          "Seek Amplitude Loop C2,"
                          "Seek Frequency Limit High,"
                          "Seek Frequency Limit Low,"
                          "Seek Phase loop I,"
                          "Seek Phase loop CF");
    }
    return paramNames;
}

/**
 * @brief SeekRecipeData::getLocaleSeekRecipeParameterNames : Parameter translated names
 * @return
 */
QString SeekRecipeData :: getLocaleSeekRecipeParameterNames()
{
    QString paramNames;
    paramNames.append(SEEK_RECP_TIME);
    paramNames.append(COMMA);
    paramNames.append(SEEK_RECP_RAMP);
    paramNames.append(COMMA);
    paramNames.append(SEEK_RECP_OFFSET);
    paramNames.append(COMMA);
    paramNames.append(SEEK_RECP_MEMORY);

    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        paramNames.append(COMMA);
        paramNames.append(SEEK_RECP_AMPLOOPC1);
        paramNames.append(COMMA);
        paramNames.append(SEEK_RECP_AMPLOOPC2);
        paramNames.append(COMMA);
        paramNames.append(SEEK_RECP_FREQ_LIMIT_HIGH);
        paramNames.append(COMMA);
        paramNames.append(SEEK_RECP_FREQ_LIMIT_LOW);
        paramNames.append(COMMA);
        paramNames.append(SEEK_RECP_PHASE_LOOP_I);
        paramNames.append(COMMA);
        paramNames.append(SEEK_RECP_PHASE_LOOP_CF);
    }
    return paramNames;
}

/**
 * @brief SeekRecipeData::createSeekRecipe : update model refresh status
 */
void SeekRecipeData :: createSeekRecipe()
{
    context->setContextProperty("seekrecipeParameterModel",QVariant::fromValue(m_dummyRecipeParamList));
    for(int element=0; element <m_seekRecipeParamList.size() ; element++)
    {
        SeekRecipeParameter *ptrRecipeParameter = dynamic_cast<SeekRecipeParameter*>(m_seekRecipeParamList.at(element));
        delete ptrRecipeParameter;
        ptrRecipeParameter = NULL;
    }
    m_seekRecipeParamList.clear();

    QString m_recipeParameters = getSeekRecipeParameterNames();
    QStringList m_recipeParametersList = m_recipeParameters.split(",");
    QString m_recipeParametersLocale = getLocaleSeekRecipeParameterNames();
    QStringList m_recipeParametersListLocale = m_recipeParametersLocale.split(",");
    QString paramVal;
    for(int dwIndex = 0; dwIndex < m_recipeParametersList.count(); dwIndex++)
    {
        stRecipeParmInfo stTemp = m_MapSeekRecipeParams[m_recipeParametersList[dwIndex].toLower()];
        SeekRecipeParameter *seekRecipe = new SeekRecipeParameter;
        seekRecipe->m_parameterName = m_recipeParametersList[dwIndex];
        seekRecipe->m_parameterLocaleName = m_recipeParametersListLocale[dwIndex];
        seekRecipe->m_parameterUnit = stTemp.ParamUnit[m_MetrixUnitSelected];
        seekRecipe->m_parameterMinValue = stTemp.ParamMinValue[m_CurrentSystemFreq];
        seekRecipe->m_parameterMaxValue = stTemp.ParamMaxValue[m_CurrentSystemFreq];
        paramVal = getSeekRecipeStructureVal(dwIndex);
        if(seekRecipe->m_parameterUnit == "s" )
        {
            paramVal.setNum(paramVal.toDouble(),'f',3);
        }
        if(seekRecipe->m_parameterName == "Memory Clear Before Seek")
        {
            seekRecipe->m_parameterSwitch = true;
            int num = paramVal.toInt();
            seekRecipe->m_parameterSwitchState = num;
        }
        else
        {
            seekRecipe->m_parameterSwitch = false;
        }
        seekRecipe->m_parameterValue = paramVal;
        m_seekRecipeParamList.append(seekRecipe);
    }
    context->setContextProperty("seekrecipeParameterModel",QVariant::fromValue(m_seekRecipeParamList));
}

/**
 * @brief SeekRecipeData::getSeekRecipeStructureVal : Get the value based on structure
 * @param dwParamIndex
 * @return
 */
QString SeekRecipeData :: getSeekRecipeStructureVal(int dwParamIndex)
{
    QString qstrRetValue="";
    switch(dwParamIndex)
    {
    case SEEK_TIME:
        qstrRetValue = QString::number(seekRecipe.SeekTime);
        break;
    case SEEK_RAMP_TIME:
        qstrRetValue = QString::number(seekRecipe.SeekRampTime);
        break;
    case SEEK_FREQ_OFFSET:
        qstrRetValue = QString::number(seekRecipe.SeekFreqOffset);
        break;
    case SEEK_MEMORY_CLEAR_BEFORE_SEEK:
        qstrRetValue = QString::number(seekRecipe.SeekMemClearbeforeSeek);
        break;
    case SEEK_AMP_LOOPC1:
        qstrRetValue = QString::number(seekRecipe.SeekAmplitudeLoopC1);
        break;
    case SEEK_AMP_LOOPC2:
        qstrRetValue = QString::number(seekRecipe.SeekAmplitudeLoopC2);
        break;
    case SEEK_FREQ_LIMIT_HIGH:
        qstrRetValue = QString::number(seekRecipe.SeekFreqLimitHigh);
        break;
    case SEEK_FREQ_LIMIT_LOW:
        qstrRetValue = QString::number(seekRecipe.SeekFreqLimitLow);
        break;
    case SEEK_PHASE_LOOPI:
        qstrRetValue = QString::number(seekRecipe.SeekPhaseloopI);
        break;
    case SEEK_PHASE_LOOPCF:
        qstrRetValue = QString::number(seekRecipe.SeekPhaseloopCF);
        break;
    default :
        qstrRetValue = "";
        break;
    }
    if(dwParamIndex == 0 || dwParamIndex == 1)
    {
        double dval = qstrRetValue.toDouble();
        dval = dval/1000;
        qstrRetValue.setNum(dval,'f',3);
    }
    return qstrRetValue;
}

/**
 * @brief SeekRecipeData::setSeekRecipeStructureValue : Set values by particular structure
 * @param a_paramIndex
 * @param a_paramVal
 */
void SeekRecipeData :: setSeekRecipeStructureValue(qint8 a_paramIndex, QString a_paramVal)
{
    int dwParamIndex = a_paramIndex;
    QString paramValue = a_paramVal;

    if(dwParamIndex == 0 || dwParamIndex == 1)
    {
        double dval = paramValue.toDouble();
        dval = dval*1000;
        paramValue = QString::number(dval);
    }
    switch(dwParamIndex)
    {
    case SEEK_TIME:
        seekRecipe.SeekTime = paramValue.toInt();
        break;
    case SEEK_RAMP_TIME:
        seekRecipe.SeekRampTime = paramValue.toInt();
        break;
    case SEEK_FREQ_OFFSET:
        seekRecipe.SeekFreqOffset = paramValue.toInt();
        break;
    case SEEK_MEMORY_CLEAR_BEFORE_SEEK:
        seekRecipe.SeekMemClearbeforeSeek = paramValue.toInt();
        break;
    case SEEK_AMP_LOOPC1:
        seekRecipe.SeekAmplitudeLoopC1 = paramValue.toInt();
        break;
    case SEEK_AMP_LOOPC2:
        seekRecipe.SeekAmplitudeLoopC2 = paramValue.toInt();
        break;
    case SEEK_FREQ_LIMIT_HIGH:
        seekRecipe.SeekFreqLimitHigh = paramValue.toInt();
        break;
    case SEEK_FREQ_LIMIT_LOW:
        seekRecipe.SeekFreqLimitLow = paramValue.toInt();
        break;
    case SEEK_PHASE_LOOPI:
        seekRecipe.SeekPhaseloopI = paramValue.toInt();
        break;
    case SEEK_PHASE_LOOPCF:
        seekRecipe.SeekPhaseloopCF = paramValue.toInt();
        break;
    default :
        break;
    }
}

/**
 * @brief SeekRecipeData::reqToLoadRecipeData : Send message to get default horn recipe
 * @return
 */
int SeekRecipeData :: reqToLoadRecipeData()
{
    int paramInd = 0 ;
    QStringList seekRecipeFromDb;
    QString seekRecipeMsg;
    QString stringRead ;
    int retValue = OPERATION_SUCCESS ;

    /* Send message to get default horn recipe */
    CIObj.sendMessage(REQ_GET_DEFAULT_SEEK_RECIPE , RES_GET_DEFAULT_SEEK_RECIPE , QString::number(m_psFreq));
    CIObj.recivedMessage(RES_GET_DEFAULT_SEEK_RECIPE , seekRecipeMsg);

    /* Filter the string with the delimiter */
    seekRecipeFromDb = seekRecipeMsg.split(',');

    /* Check for the length of the msg received */
    if(SEEK_RECIPE_PARAM_COUNT != seekRecipeFromDb.length())
    {
        retValue = NO_PARAM ;
    }
    else
    {
        /* Check the range and update the parameters with the current values */
        for(paramInd = 0 ; paramInd < seekRecipeFromDb.length() && paramInd < SEEK_RECIPE_PARAM_COUNT ; paramInd++)
        {
            stringRead = seekRecipeFromDb.at(paramInd);
            if(paramInd == 0 || paramInd == 1)
            {
                double dval = stringRead.toDouble();
                dval = dval/1000;
                stringRead = QString::number(dval);
            }
            setSeekRecipeStructureValue(paramInd,stringRead );
        }
    }
    return retValue ;
}

/**
 * @brief SeekRecipeData::reqToSaveRecipeData : Save message to get default horn recipe
 * @return
 */
int SeekRecipeData :: reqToSaveRecipeData()
{
    int retValue = PARAM_IN_RANGE;
    QString reqSaveRecipeDbMsg = "";
    QString resSaveRecipeDbMsg = "";

    /* Generate the seek recipe msg to DB */
    reqSaveRecipeDbMsg = QString :: number(m_psFreq) + ","
            + QString :: number(seekRecipe.SeekTime) + ","
            + QString :: number(seekRecipe.SeekRampTime) + ","
            + QString :: number(seekRecipe.SeekFreqOffset) + ","
            + QString :: number(seekRecipe.SeekMemClearbeforeSeek)+ ","
            + QString :: number(seekRecipe.SeekAmplitudeLoopC1) + ","
            + QString :: number(seekRecipe.SeekAmplitudeLoopC2) + ","
            + QString :: number(seekRecipe.SeekFreqLimitHigh) + ","
            + QString :: number(seekRecipe.SeekFreqLimitLow) + ","
            + QString :: number(seekRecipe.SeekPhaseloopI) + ","
            + QString :: number(seekRecipe.SeekPhaseloopCF);
    /* Sending Seek recipe message */
    CIObj.sendMessage(REQ_SET_SEEK_RECIPE_DATA, RES_SET_SEEK_RECIPE_DATA, reqSaveRecipeDbMsg);

    /* Receive the seek recipe response message */
    CIObj.recivedMessage(RES_SET_SEEK_RECIPE_DATA, resSaveRecipeDbMsg);

    if("" == resSaveRecipeDbMsg)
    {
        retValue = NO_PARAM ;
    }
    else
    {
        if(SEEK_RECIPE_SAVED_SUCCESS != resSaveRecipeDbMsg.toStdString())
        {
            retValue = OPERATION_FAILURE ;
        }
    }

    return retValue ;
}

/**
 * @brief SeekRecipeData::sendSeekRecipeDataToSc : Send seek recipe to BL
 * @return
 */
int SeekRecipeData :: sendSeekRecipeDataToSc()
{
    int status = 0;

    char seekRecipeResponseMsg[4]={0};

    char *buff=reinterpret_cast<char *>(&(seekRecipe));
    /* Sending horn recipe message */
    CIObj.sendMessage(SCBL_SEEK_RECIPE_REQ, UIC_SEEK_RECIPE_RES, buff, sizeof(seekRecipe));

    /* Receive the horn recipe response message */
    CIObj.recivedMessage(UIC_SEEK_RECIPE_RES, seekRecipeResponseMsg , 1);

    status = (int)seekRecipeResponseMsg[0];

    return status ;
}

/**
 * @brief SeekRecipeData::saveUserData
 * @return
 */
int SeekRecipeData :: saveUserData()
{
    int retVal = OPERATION_SUCCESS;
    /* Request to save the current seek recipe data to DB */
    retVal = reqToSaveRecipeData();
    return retVal ;
}

/**
 * @brief SeekRecipeData::cancelUserData
 */
void SeekRecipeData :: cancelUserData()
{
    reqToLoadRecipeData();
    createSeekRecipe();
}
/**
 * @brief SeekRecipeData::resetSeekRecipeData
 * @return
 */
void SeekRecipeData :: resetSeekRecipeData()
{
    seekRecipe.SeekTime = seekRecipeDefault.SeekTime;
    seekRecipe.SeekRampTime = seekRecipeDefault.SeekRampTime;
    seekRecipe.SeekFreqOffset = seekRecipeDefault.SeekFreqOffset;
    seekRecipe.SeekMemClearbeforeSeek = seekRecipeDefault.SeekMemClearbeforeSeek;
    seekRecipe.SeekAmplitudeLoopC1 = seekRecipeDefault.SeekAmplitudeLoopC1;
    seekRecipe.SeekAmplitudeLoopC2 = seekRecipeDefault.SeekAmplitudeLoopC2;
    seekRecipe.SeekFreqLimitHigh = seekRecipeDefault.SeekFreqLimitHigh;
    seekRecipe.SeekFreqLimitLow = seekRecipeDefault.SeekFreqLimitLow;
    seekRecipe.SeekPhaseloopI = seekRecipeDefault.SeekPhaseloopI;
    seekRecipe.SeekPhaseloopCF = seekRecipeDefault.SeekPhaseloopCF;
    createSeekRecipe();
}

/**
 * @brief SeekRecipeData::rangeCheck
 * @param a_tmpValue
 * @param a_paramIndex
 * @return
 */
QString SeekRecipeData :: rangeCheck( QString keypadValue , QString keypadTextName)
{
    stRecipeParmInfo stTemp = m_MapSeekRecipeParams[keypadTextName.toLower()];
    double currentValue = keypadValue.toDouble();
    double minValue = stTemp.ParamMinValue[m_CurrentSystemFreq].toDouble();
    double maxValue = stTemp.ParamMaxValue[m_CurrentSystemFreq].toDouble();
    int dwIndex = stTemp.ParamIndexInStructure;
    double freqVal;
    if(currentValue < minValue)
    {
        return getSeekRecipeStructureVal(dwIndex);
    }
    else if(currentValue > maxValue)
    {
        return getSeekRecipeStructureVal(dwIndex);
    }

    if(keypadTextName.toLower() == "seek frequency limit low")
    {
        freqVal = getSeekRecipeStructureVal(SEEK_FREQ_LIMIT_HIGH).toDouble();
        if(currentValue > freqVal)
            return getSeekRecipeStructureVal(dwIndex);
        else
            return keypadValue;
    }
    else if(keypadTextName.toLower() == "seek frequency limit high")
    {
        freqVal = getSeekRecipeStructureVal(SEEK_FREQ_LIMIT_LOW).toDouble();
        if(currentValue < freqVal)
            return getSeekRecipeStructureVal(dwIndex);
        else
            return keypadValue;
    }
    else
    {
        return keypadValue;
    }
}

/**
 * @brief SeekRecipeData::modifyParameter
 * @param a_paramValue
 * @param a_paramIndex
 */
void SeekRecipeData :: modifyParameter(QString a_paramValue , QString a_paramName)
{
    int dwIndex;
    stRecipeParmInfo stTemp = m_MapSeekRecipeParams[a_paramName.toLower()];
    dwIndex = stTemp.ParamIndexInStructure;
    setSeekRecipeStructureValue(dwIndex,a_paramValue);
    createSeekRecipe();
}

/**
 * @brief SeekRecipeData::getPowerupLoadFlag
 * @return
 */
bool SeekRecipeData :: getPowerupLoadFlag()
{
    return m_dataLoadedOnPowerup;
}

/**
 * @brief SeekRecipeData::getParameterChangedFlag
 * @return
 */
bool SeekRecipeData :: getParameterChangedFlag()
{
    return m_parameterChanged ;
}

/**
 * @brief SeekRecipeData::getSeekPsFreq
 * @return
 */
unsigned int SeekRecipeData :: getSeekPsFreq()
{
    return m_psFreq ;
}

/**
 * @brief SeekRecipeData::updateDefaultParams
 */
void SeekRecipeData :: updateDefaultParams()
{
    seekRecipeDefault.SeekTime = m_MapSeekRecipeParams["seek time"].ParamDefaultValue[m_CurrentSystemFreq].toDouble()*1000;
    seekRecipeDefault.SeekRampTime = m_MapSeekRecipeParams["seek ramp time"].ParamDefaultValue[m_CurrentSystemFreq].toDouble()*1000;
    seekRecipeDefault.SeekFreqOffset = m_MapSeekRecipeParams["seek frequency offset"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipeDefault.SeekMemClearbeforeSeek = m_MapSeekRecipeParams["memory clear before seek"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipeDefault.SeekAmplitudeLoopC1 = m_MapSeekRecipeParams["seek amplitude loop c1"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipeDefault.SeekAmplitudeLoopC2= m_MapSeekRecipeParams["seek amplitude loop c2"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipeDefault.SeekFreqLimitHigh= m_MapSeekRecipeParams["seek frequency limit high"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipeDefault.SeekFreqLimitLow = m_MapSeekRecipeParams["seek frequency limit low"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipeDefault.SeekPhaseloopI = m_MapSeekRecipeParams["seek phase loop i"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    seekRecipeDefault.SeekPhaseloopCF = m_MapSeekRecipeParams["seek phase loop cf"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
}

/**
 * @brief SeekRecipeData::~SeekRecipeData
 */
SeekRecipeData :: ~SeekRecipeData()
{
    SeekRecipeParameter* tmpRecipe;

    int ind = 0 ;

    for(ind = 0 ; ind < m_seekRecipeParamList.length() ; ind++)
    {
        tmpRecipe = dynamic_cast<SeekRecipeParameter*>(m_seekRecipeParamList.at(ind));

        delete tmpRecipe ;
        tmpRecipe = NULL;
    }
    m_seekRecipeParamList.clear();
}

/**
 * @brief SeekRecipeData::setSeekCheckBoxModelData
 */
void SeekRecipeData::setSeekCheckBoxModelData()
{
    context->setContextProperty("seekScanCheckBoxModel", QVariant::fromValue(m_seekCheckBoxDataTemp));
    for(auto count=0;count<m_seekCheckBoxData.size();count++){

        auto tempSeekCheckBoxData=dynamic_cast<checkbox*>(m_seekCheckBoxData.at(count))   ;
        delete tempSeekCheckBoxData;
        tempSeekCheckBoxData = NULL;

    }
    m_seekCheckBoxData.clear();
    m_seekCheckBoxData.append(new checkbox("#f03e3e",SEEK_GRAPH_AMP,"%","true","0",true,0));
    m_seekCheckBoxData.append(new checkbox("#4285f4",SEEK_GRAPH_FRQ,"Hz","true","0",true,1));
    m_seekCheckBoxData.append(new checkbox("#a82ba8",SEEK_GRAPH_POW,"W","true","0",true,0));
    m_seekCheckBoxData.append(new checkbox("#009587",SEEK_GRAPH_RATEDCUR,"%","true","0",true,1));
    m_seekCheckBoxData.append(new checkbox("black",SEEK_GRAPH_PHASE,"Deg","true","0",true,0));

    context->setContextProperty("seekScanCheckBoxModel", QVariant::fromValue(m_seekCheckBoxData));
}

/**
 * @brief SeekRecipeData::setCheckboxStatus
 * @param dwIndex
 * @param isChecked
 */
void SeekRecipeData::setCheckboxStatus(int dwIndex, bool isChecked)
{
    QObject *ptr;
    checkbox *m_ptr;
    ptr = m_seekCheckBoxData.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    m_ptr->setIsCheckBoxSelected( isChecked);

}

/**
 * @brief SeekRecipeData::getComboIndex
 * @param dwIndex
 * @return
 */
int SeekRecipeData::getComboIndex(int dwIndex)
{
    QObject *ptr;
    checkbox *m_ptr;
    ptr = m_seekCheckBoxData.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    return m_ptr->m_isLeftAxis;
}

/**
 * @brief SeekRecipeData::setAxisIndex
 * @param dwIndex
 * @param isLeftSelected
 */
void SeekRecipeData::setAxisIndex(int dwIndex, int isLeftSelected)
{
    QObject *ptr;
    checkbox *m_ptr;
    ptr = m_seekCheckBoxData.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);

    if(isLeftSelected == 0)
        m_ptr->m_isLeftAxis = 0;
    if(isLeftSelected == 1)
        m_ptr->m_isLeftAxis = 1;
}

/**
 * @brief SeekRecipeData::setParamValues
 * @param dwAmp
 * @param dwFreq
 * @param dwPower
 * @param dwCurr
 * @param dwPhase
 */
void SeekRecipeData::setParamValues(int dwAmp, int dwFreq, int dwPower, int dwCurr, int dwPhase)
{
    context->setContextProperty("seekScanCheckBoxModel", QVariant::fromValue(m_seekCheckBoxDataTemp));
    checkbox *m_ptr;
    m_ptr = reinterpret_cast<checkbox*>(m_seekCheckBoxData.at(0));
    m_ptr->setParameterVal(QString::number(dwAmp));
    m_ptr = reinterpret_cast<checkbox*>(m_seekCheckBoxData.at(1));
    m_ptr->setParameterVal(QString::number(dwFreq));
    m_ptr = reinterpret_cast<checkbox*>(m_seekCheckBoxData.at(2));
    m_ptr->setParameterVal(QString::number(dwPower));
    m_ptr = reinterpret_cast<checkbox*>(m_seekCheckBoxData.at(3));
    m_ptr->setParameterVal(QString::number(dwCurr));
    m_ptr = reinterpret_cast<checkbox*>(m_seekCheckBoxData.at(4));
    m_ptr->setParameterVal(QString::number(dwPhase));
    context->setContextProperty("seekScanCheckBoxModel", QVariant::fromValue(m_seekCheckBoxData));
}

/**
 * @brief SeekRecipeData::setSeekDefaultValues
 */
void SeekRecipeData::setSeekDefaultValues()
{
    context->setContextProperty("seekScanCheckBoxModel", QVariant::fromValue(m_dummyRecipeParamList));
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("SeekGraphCheckBoxStatus");
    QString checkValue = settings.value("SeekCheckStatus").toString();
    settings.endGroup();
    settings.beginGroup("SeekGraphComboBoxStatus");
    QString comboValue = settings.value("SeekComboStatus").toString();
    settings.endGroup();
    if(checkValue.size()>1)
    {
        checkValue.remove(checkValue.length()-1,1);
        checkValue.remove(0,1);
    }
    else
    {
        checkValue="0,0,0,0,0";
    }
    if(comboValue.size()>1)
    {
        comboValue.remove(comboValue.length()-1,1);
        comboValue.remove(0,1);
    }
    else
    {
        comboValue="0,0,1,1,1";
    }

    QStringList temp1 = checkValue.split(",");
    QStringList temp2 = comboValue.split(",");
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<m_seekCheckBoxData.size();i++)
    {
        ptr = m_seekCheckBoxData.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        m_ptr->m_isCheckBoxSelected = temp1[i].toInt();
        m_ptr->m_isLeftAxis = temp2[i].toInt();
    }
    context->setContextProperty("seekScanCheckBoxModel", QVariant::fromValue(m_seekCheckBoxData));
}

/**
 * @brief SeekRecipeData::getSeekDefaultValues
 */
void SeekRecipeData::getSeekDefaultValues()
{
    QString qstrCheckOrder="",qstrComboOrder="";
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<m_seekCheckBoxData.size();i++)
    {
        ptr = m_seekCheckBoxData.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        qstrComboOrder.append(QString::number(m_ptr->m_isLeftAxis));
        qstrComboOrder.append(",");
        qstrCheckOrder.append(QString::number(m_ptr->m_isCheckBoxSelected));
        qstrCheckOrder.append(",");
    }
    qstrComboOrder.remove(qstrComboOrder.length()-1,1);
    qstrComboOrder.insert(0,"\"");
    qstrComboOrder.insert(qstrComboOrder.length(),"\"");

    qstrCheckOrder.remove(qstrCheckOrder.length()-1,1);
    qstrCheckOrder.insert(0,"\"");
    qstrCheckOrder.insert(qstrCheckOrder.length(),"\"");

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("SeekGraphCheckBoxStatus");
    settings.setValue("SeekCheckStatus",qstrCheckOrder);
    settings.endGroup();
    settings.beginGroup("SeekGraphComboBoxStatus");
    settings.setValue("SeekComboStatus",qstrComboOrder);
    settings.endGroup();
}
