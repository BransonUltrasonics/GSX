/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Horn Recipe Data

***************************************************************************/
#ifndef HORNRECIPEDATA_H
#include "Header/hornrecipedata.h"
#include "Header/common.h"
#endif

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#ifndef CHECKBOX_H
#include "Header/checkbox.h"
#endif

#include <QFile>
#include <QQmlContext>
#include "Header/sysconfigdata.h"
#include <QSettings>
using namespace std ;

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/* In the future the extern implementation of sysconfig data to remove and read from global class */
extern unsigned int psFreqGlobalVar;

/**
 * @brief HornRecipeParameter::HornRecipeParameter : Initialize the member and create memory
 * @param a_parameterName
 * @param a_parameterVal
 * @param a_parameterUnit
 */
HornRecipeParameter :: HornRecipeParameter(const QString &a_parameterName, const QString &a_parameterLocaleName, const QString &a_parameterVal,
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
 * @brief HornRecipeParameter::HornRecipeParameter
 * @param parent
 */
HornRecipeParameter::HornRecipeParameter(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief HornRecipeParameter::~HornRecipeParameter
 */
HornRecipeParameter :: ~HornRecipeParameter()
{
}

/**
 * @brief HornRecipeParameter::getParameterName
 * @return
 */
QString HornRecipeParameter :: getParameterName()
{
    return m_parameterName;
}

/**
 * @brief HornRecipeParameter::getParameterLocaleName
 * @return
 */
QString HornRecipeParameter :: getParameterLocaleName()
{
    return m_parameterLocaleName;
}

/**
 * @brief HornRecipeParameter::getParameterUnit
 * @return
 */
QString HornRecipeParameter :: getParameterUnit()
{
    return m_parameterUnit;
}

/**
 * @brief HornRecipeParameter::getParameterValue
 * @return
 */
QString HornRecipeParameter :: getParameterValue()
{
    return m_parameterValue ;
}

/**
 * @brief HornRecipeParameter::getParameterMinValue
 * @return
 */
QString HornRecipeParameter :: getParameterMinValue()
{
    return m_parameterMinValue ;
}

/**
 * @brief HornRecipeParameter::getParameterMaxValue
 * @return
 */
QString HornRecipeParameter :: getParameterMaxValue()
{
    return m_parameterMaxValue ;
}

/**
 * @brief HornRecipeParameter::setParameterValue
 * @param a_val
 */
void HornRecipeParameter :: setParameterValue(QString a_val)
{
    m_parameterValue = a_val;
}

/**
 * @brief HornRecipeParameter::getParameterSwitch
 * @return
 */
bool HornRecipeParameter :: getParameterSwitch()
{
    return m_parameterSwitch;
}

/**
 * @brief HornRecipeParameter::getParameterSwitchState
 * @return
 */
bool HornRecipeParameter :: getParameterSwitchState()
{
    return m_parameterSwitchState;
}

/**
 * @brief HornRecipeData::HornRecipeData
 */
HornRecipeData :: HornRecipeData()
{
    initializeHornRecipeData();
    m_CurrentSystemFreq = SYSTEM_TYPE_20KHZ;
    m_MetrixUnitSelected = UNIT_METRIC;
    getDefaultFrequencySelectionForScan();

    qstrlstParamValues.clear();
    qstrlstParamValues.append(0);
    qstrlstParamValues.append(0);
    qstrlstParamValues.append(0);
    qstrlstParamValues.append(0);

    createRightParameters();
}

/**
 * @brief HornRecipeData::initializeHornRecipeData : Default values for horn recipe data
 */
void HornRecipeData :: initializeHornRecipeData()
{
    stRecipeParmInfo stTempParam;


    stTempParam.ParamIndexInStructure = HORN_FREQUENCY_STEP;
    stTempParam.ParamUnit[UNIT_METRIC]   = HERTZ;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = HERTZ;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "100";
    m_MapHornRecipeParams["frequency step"] = stTempParam;


    stTempParam.ParamIndexInStructure = HORN_MAX_AMP;
    stTempParam.ParamUnit[UNIT_METRIC]   = PERCENTAGE;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = PERCENTAGE;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "10";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "10";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "10";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "100";
    m_MapHornRecipeParams["max amplitude"] = stTempParam;

    stTempParam.ParamIndexInStructure = HORN_MAX_CURRENT;
    stTempParam.ParamUnit[UNIT_METRIC]   = PERCENTAGE;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = PERCENTAGE;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "10";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "10";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "10";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "100";
    m_MapHornRecipeParams["max current"] = stTempParam;

    stTempParam.ParamIndexInStructure = HORN_DIGITAL_TUNE;
    stTempParam.ParamUnit[UNIT_METRIC]   = "";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "0";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1";
    m_MapHornRecipeParams["set digital tune with horn scan"] = stTempParam;

    stTempParam.ParamIndexInStructure = HORN_START_FREQUENCY;
    stTempParam.ParamUnit[UNIT_METRIC]   = HERTZ;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = HERTZ;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "19450";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "19450";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "20450";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "29250";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "29250";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "30750";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "38900";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "38900";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "40900";
    m_MapHornRecipeParams["start frequency"] = stTempParam;

    stTempParam.ParamIndexInStructure = HORN_STOP_FREQUENCY;
    stTempParam.ParamUnit[UNIT_METRIC]   = HERTZ;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = HERTZ;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "20450";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "19450";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "20450";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "30750";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "29250";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "30750";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "40900";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "38900";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "40900";
    m_MapHornRecipeParams["stop frequency"] = stTempParam;


    stTempParam.ParamIndexInStructure = HORN_TIME_DELAY;
    stTempParam.ParamUnit[UNIT_METRIC]   = SECONDS;
    stTempParam.ParamUnit[UNIT_IMPERIAL] = SECONDS;
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "0.01";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "0.01";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "0.1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "0.01";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "0.01";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "0.1";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "0.01";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "0.01";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "0.1";
    m_MapHornRecipeParams["time delay"] = stTempParam;
}

/**
 * @brief HornRecipeData::createHornRecipe : Set horn recipe name from list and update model
 */
void HornRecipeData :: createHornRecipe()
{
    context->setContextProperty("recipeParameterModel",QVariant::fromValue(m_dummyHornRecipe));
    for(int element=0; element <m_hornRecipeParamList.size() ; element++)
    {
        HornRecipeParameter *ptrRecipeParameter = dynamic_cast<HornRecipeParameter*>(m_hornRecipeParamList.at(element));
        delete ptrRecipeParameter;
        ptrRecipeParameter = NULL;
    }
    m_hornRecipeParamList.clear();

    QString m_recipeParameters = getHornRecipeParameterNames();
    QStringList m_recipeParametersList = m_recipeParameters.split(",");
    QString m_recipeParametersLocale = getHornRecipeLocaleParameterNames();
    QStringList m_recipeParametersListLocale = m_recipeParametersLocale.split(",");
    QString paramVal;
    for(int dwIndex = 0; dwIndex < m_recipeParametersList.count(); dwIndex++)
    {
        stRecipeParmInfo stTemp = m_MapHornRecipeParams[m_recipeParametersList[dwIndex].toLower()];
        HornRecipeParameter *hornRecipe = new HornRecipeParameter;
        hornRecipe->m_parameterName = m_recipeParametersList[dwIndex];
        hornRecipe->m_parameterLocaleName = m_recipeParametersListLocale[dwIndex];
        hornRecipe->m_parameterUnit = stTemp.ParamUnit[m_MetrixUnitSelected];
        hornRecipe->m_parameterMinValue = stTemp.ParamMinValue[m_CurrentSystemFreq];
        hornRecipe->m_parameterMaxValue = stTemp.ParamMaxValue[m_CurrentSystemFreq];
        paramVal = getHornRecipeStructureVal(dwIndex);
        if(hornRecipe->m_parameterUnit == "s" )
        {
            paramVal.setNum(paramVal.toDouble(),'f',3);
        }
        if(hornRecipe->m_parameterName == "Set Digital Tune with Horn Scan")
        {
            hornRecipe->m_parameterSwitch = true;
            int num = paramVal.toInt();
            hornRecipe->m_parameterSwitchState = num;
        }
        else
        {
            hornRecipe->m_parameterSwitch = false;
        }
        hornRecipe->m_parameterValue = paramVal;
        m_hornRecipeParamList.append(hornRecipe);
    }
    context->setContextProperty("recipeParameterModel",QVariant::fromValue(m_hornRecipeParamList));
}

/**
 * @brief HornRecipeData::setDefaultSystemValuesForScan : Default units and frequency values
 * @param dwMetrixUnit
 * @param psFreqlVal
 */
void HornRecipeData::setDefaultSystemValuesForScan(int dwMetrixUnit, int psFreqlVal)
{
    m_MetrixUnitSelected = static_cast<Units>(dwMetrixUnit);
    if(psFreqlVal == FrequencySelected20KHZ)
    {
        m_CurrentSystemFreq=SYSTEM_TYPE_20KHZ;
    }
    else if(psFreqlVal == FrequencySelected30KHZ)
    {
        m_CurrentSystemFreq=SYSTEM_TYPE_30KHZ;
    }
    else
    {
        m_CurrentSystemFreq=SYSTEM_TYPE_40KHZ;
    }
    getDefaultFrequencySelectionForScan();
}

/**
 * @brief HornRecipeData::getDefaultFrequencySelectionForScan : Default frequency values to display
 */
void HornRecipeData :: getDefaultFrequencySelectionForScan()
{
    hornRecipe.freqStep = m_MapHornRecipeParams["frequency step"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipe.maxAmp = m_MapHornRecipeParams["max amplitude"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipe.maxCurrent = m_MapHornRecipeParams["max current"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipe.SetDigTuneWithHornScan = m_MapHornRecipeParams["set digital tune with horn scan"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipe.startFreq = m_MapHornRecipeParams["start frequency"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipe.stopFreq = m_MapHornRecipeParams["stop frequency"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipe.timeDelay = m_MapHornRecipeParams["time delay"].ParamDefaultValue[m_CurrentSystemFreq].toDouble()*1000;
}

/**
 * @brief HornRecipeData::getHornRecipeParameterNames : Return horn recipe parameter names
 * @return
 */
QString HornRecipeData :: getHornRecipeParameterNames()
{
    QString param = "Frequency Step,"
                    "Max Amplitude,"
                    "Max Current,"
                    "Set Digital Tune with Horn Scan";

    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        param.append(",Start Frequency,Stop Frequency,Time Delay");
    }
    return param;
}

/**
 * @brief HornRecipeData::getHornRecipeLocaleParameterNames : Return horn recipe parameter names
 * @return
 */
QString HornRecipeData :: getHornRecipeLocaleParameterNames()
{
    QString param;
    param.append(HORN_RECIPE_FREQ_STEP);
    param.append(COMMA);
    param.append(HORN_RECIPE_MAX_AMP);
    param.append(COMMA);
    param.append(HORN_RECIPE_MAX_CUR);
    param.append(COMMA);
    param.append(HORN_RECIPE_SETDIGITAL);

    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        param.append(COMMA);
        param.append(HORN_RECIPE_START_FREQ);
        param.append(COMMA);
        param.append(HORN_RECIPE_STOP_FREQ);
        param.append(COMMA);
        param.append(HORN_RECIPE_TIME_DELAY);
    }
    return param;
}
/**
 * @brief HornRecipeData::getHornRecipeStructureVal : Assign recipe name to the structure
 * @param a_paramVal
 * @return
 */
QString HornRecipeData :: getHornRecipeStructureVal(int dwParamIndex)
{
    QString qstrRetValue="";
    switch(dwParamIndex)
    {
    case HORN_FREQUENCY_STEP:
        qstrRetValue = QString::number(hornRecipe.freqStep);
        break;
    case HORN_MAX_AMP:
        qstrRetValue = QString::number(hornRecipe.maxAmp);
        break;
    case HORN_MAX_CURRENT:
        qstrRetValue = QString::number(hornRecipe.maxCurrent);
        break;
    case HORN_DIGITAL_TUNE:
        qstrRetValue = QString::number(hornRecipe.SetDigTuneWithHornScan);
        break;
    case HORN_START_FREQUENCY:
        qstrRetValue = QString::number(hornRecipe.startFreq);
        break;
    case HORN_STOP_FREQUENCY:
        qstrRetValue = QString::number(hornRecipe.stopFreq);
        break;
    case HORN_TIME_DELAY:
        qstrRetValue = QString::number(hornRecipe.timeDelay);
        break;
    default :
        qstrRetValue = "";
        break;
    }
    if(dwParamIndex == HORN_TIME_DELAY)
    {
        double dval = qstrRetValue.toDouble();
        dval = dval/1000;
        qstrRetValue.setNum(dval,'f',3);
    }
    return qstrRetValue;
}

/**
 * @brief HornRecipeData::setHornRecipeStructureValue : Assign values to the structure
 * @param a_paramIndex
 * @param a_paramVal
 */
void HornRecipeData :: setHornRecipeStructureValue(qint8 a_paramIndex, QString a_paramVal)
{
    int dwParamIndex = a_paramIndex;
    QString paramValue = a_paramVal;

    if(dwParamIndex == HORN_TIME_DELAY)
    {
        double dval = paramValue.toDouble();
        dval = dval*1000;
        paramValue = QString::number(dval);
    }
    switch(dwParamIndex)
    {
    case HORN_FREQUENCY_STEP:
        hornRecipe.freqStep = paramValue.toInt();
        break;
    case HORN_MAX_AMP:
        hornRecipe.maxAmp = paramValue.toInt();
        break;
    case HORN_MAX_CURRENT:
        hornRecipe.maxCurrent = paramValue.toInt();
        break;
    case HORN_DIGITAL_TUNE:
        hornRecipe.SetDigTuneWithHornScan = paramValue.toInt();
        break;
    case HORN_START_FREQUENCY:
        hornRecipe.startFreq = paramValue.toInt();
        break;
    case HORN_STOP_FREQUENCY:
        hornRecipe.stopFreq = paramValue.toInt();
        break;
    case HORN_TIME_DELAY:
        hornRecipe.timeDelay = paramValue.toInt();
        break;
    default :
        break;
    }
}

/**
 * @brief HornRecipeData::init : This function is called while navigating to the Horn scan
*                      UI screen. It is called when the horn scan page is loaded
 * @return
 */
int HornRecipeData :: init()
{
    int retValLoadRecipe = OPERATION_SUCCESS ;
    m_psFreq = psFreqGlobalVar;
    updateDefaultParams();
    retValLoadRecipe = reqToLoadRecipeData();
    if( retValLoadRecipe != OPERATION_SUCCESS)
    {
        resetHornRecipeData();
    }
    else
    {
        createHornRecipe();
    }

    qstrlstParamValues.clear();
    qstrlstParamValues.append(0);
    qstrlstParamValues.append(0);
    qstrlstParamValues.append(0);
    qstrlstParamValues.append(0);
    return retValLoadRecipe ;
}

/**
 * @brief HornRecipeData::createRightParameters
 */
void HornRecipeData :: createRightParameters()
{
    context->setContextProperty("rightParameterModel",QVariant::fromValue(m_dummyHornRecipe));
    /* Construct the check box parameters for Horn scan window */
    m_rightParameters.append(new checkbox("#f03e3e",HORN_GRAPH_AMP,"%","true","0", true , 0));
    m_rightParameters.append(new checkbox("#009587",HORN_GRAPH_RATEDCUR,"%","true","0", true , 0));
    m_rightParameters.append(new checkbox("#000000",HORN_GRAPH_PHASE,"Deg","true","0", true , 1));
    m_rightParameters.append(new checkbox("#a82ba8",HORN_GRAPH_IMP,"","true","0", false , 1));
    context->setContextProperty("rightParameterModel",QVariant::fromValue(m_rightParameters));
}

/**
 * @brief HornRecipeData::reqToLoadRecipeData : This function loads the power up horn recipe data from DB
 * @return
 */
int HornRecipeData :: reqToLoadRecipeData()
{
    int paramInd = 0 ;
    QStringList hornRecipeFromDb;
    QString hornRecipeMsg;
    QString stringRead ;
    int retValue = OPERATION_SUCCESS ;

    /* Send message to get default horn recipe */
    CIObj.sendMessage(REQ_GET_DEFAULT_HORN_RECIPE , RES_GET_DEFAULT_HORN_RECIPE , QString::number(m_psFreq));
    CIObj.recivedMessage(RES_GET_DEFAULT_HORN_RECIPE , hornRecipeMsg);

    /* Filter the string with the delimiter */
    hornRecipeFromDb = hornRecipeMsg.split(',');

    /* Check for the length of the msg received */
    if(hornRecipeMsg=="" || (hornRecipeFromDb.size()%HORN_RECIPE_PARAM_COUNT)!=0)
    {
        retValue = NO_PARAM ;
    }
    else
    {
        /* Check the range and update the parameters with the current values */
        for(paramInd = 0 ; paramInd < hornRecipeFromDb.length() && paramInd < HORN_RECIPE_PARAM_COUNT ; paramInd++)
        {
            stringRead = hornRecipeFromDb.at(paramInd);
            if(paramInd == HORN_TIME_DELAY)
            {
                double dval = stringRead.toDouble();
                dval = dval/1000;
                stringRead = QString::number(dval);
            }
            setHornRecipeStructureValue(paramInd,stringRead);
        }
    }
    return retValue ;
}

/**
 * @brief HornRecipeData::reqToSaveRecipeData : This function requests to save the horn recipe data to DB
 * @return
 */
int HornRecipeData :: reqToSaveRecipeData()
{
    QString reqSaveRecipeDbMsg = "";
    QString resSaveRecipeDbMsg = "";

    int retValue = OPERATION_SUCCESS ;
    reqSaveRecipeDbMsg = QString :: number(m_psFreq) + ","
            + QString :: number(hornRecipe.freqStep) + ","
            + QString :: number(hornRecipe.maxAmp) + ","
            + QString :: number(hornRecipe.maxCurrent) + ","
            + QString :: number(hornRecipe.SetDigTuneWithHornScan) + ","
            + QString :: number(hornRecipe.startFreq) + ","
            + QString :: number(hornRecipe.stopFreq) + ","
            + QString :: number(hornRecipe.timeDelay);
    /* Sending horn recipe message */
    CIObj.sendMessage(REQ_SET_HORN_RECIPE_DATA, RES_SET_HORN_RECIPE_DATA, reqSaveRecipeDbMsg);

    /* Receive the horn recipe response message */
    CIObj.recivedMessage(RES_SET_HORN_RECIPE_DATA, resSaveRecipeDbMsg);

    if("" == resSaveRecipeDbMsg)
    {
        retValue = NO_PARAM ;
    }
    else
    {
        if(HORN_RECIPE_SAVED_SUCCESS != resSaveRecipeDbMsg.toStdString())
        {
            retValue = OPERATION_FAILURE ;
        }
    }

    return retValue ;
}

/**
 * @brief HornRecipeData::sendHornRecipeDataToSc : This function sends the updated horn recipe data to Supervisory Controller
 * @return
 */
int HornRecipeData :: sendHornRecipeDataToSc()
{
    int status = 0;
    char hornRecipeResponseMsg[4]={'\0'};

    char *buff=reinterpret_cast<char *>(&(hornRecipe));
    /* Sending horn recipe message */
    CIObj.sendMessage(REQ_SAVE_HORN_RECIPE, RES_SAVE_HORN_RECIPE, buff,sizeof(hornRecipe));

    /* Receive the horn recipe response message */
    CIObj.recivedMessage(RES_SAVE_HORN_RECIPE, hornRecipeResponseMsg , 1);

    status = (int)hornRecipeResponseMsg[0];

    return status ;

}

/**
 * @brief HornRecipeData::saveUserData : This function sends the updated horn recipe data to Supervisory Controller
 * @return
 */
int HornRecipeData :: saveUserData()
{
    int retVal = OPERATION_SUCCESS;
    /* Request to save the current horn recipe data to DB */
    retVal = reqToSaveRecipeData();
    return retVal ;
}

/**
 * @brief HornRecipeData::cancelUserData : This function Restore the current values with the last
*                      saved horn recipe values
 */
void HornRecipeData :: cancelUserData()
{
    reqToLoadRecipeData();
    createHornRecipe();
}

/**
 * @brief HornRecipeData::resetHornRecipeData
 * @return
 */
void HornRecipeData :: resetHornRecipeData()
{
    hornRecipe.freqStep = hornRecipeDefault.freqStep;
    hornRecipe.maxAmp = hornRecipeDefault.maxAmp;
    hornRecipe.maxCurrent = hornRecipeDefault.maxCurrent;
    hornRecipe.SetDigTuneWithHornScan = hornRecipeDefault.SetDigTuneWithHornScan;
    hornRecipe.startFreq = hornRecipeDefault.startFreq;
    hornRecipe.stopFreq = hornRecipeDefault.stopFreq;
    hornRecipe.timeDelay = hornRecipeDefault.timeDelay;
    createHornRecipe();
}

/**
 * @brief HornRecipeData::rangeCheck : Validating the range check according to units
 * @param a_tmpValue
 * @param a_paramIndex
 * @return
 */
QString HornRecipeData :: rangeCheck( QString keypadValue , QString keypadTextName)
{
    stRecipeParmInfo stTemp = m_MapHornRecipeParams[keypadTextName.toLower()];
    double currentValue = keypadValue.toDouble();
    double minValue = stTemp.ParamMinValue[m_CurrentSystemFreq].toDouble();
    double maxValue = stTemp.ParamMaxValue[m_CurrentSystemFreq].toDouble();
    int dwIndex = stTemp.ParamIndexInStructure;
    double freqVal;
    if(currentValue < minValue)
    {
        return getHornRecipeStructureVal(dwIndex);
    }
    else if(currentValue > maxValue)
    {
        return getHornRecipeStructureVal(dwIndex);
    }

    if(keypadTextName.toLower() == "start frequency")
    {
        freqVal = getHornRecipeStructureVal(HORN_STOP_FREQUENCY).toDouble();
        if(currentValue >= freqVal)
            return getHornRecipeStructureVal(dwIndex);
        else
            return keypadValue;
    }
    else if(keypadTextName.toLower() == "stop frequency")
    {
        freqVal = getHornRecipeStructureVal(HORN_START_FREQUENCY).toDouble();
        if(currentValue <= freqVal)
            return getHornRecipeStructureVal(dwIndex);
        else
            return keypadValue;
    }
    else
    {
        return keypadValue;
    }
}

/**
 * @brief HornRecipeData::modifyParameter : Modify the paramter and send to member function
 * @param a_paramValue
 * @param a_paramIndex
 */
void HornRecipeData :: modifyParameter(QString a_paramValue , QString a_paramName)
{
    int dwIndex;
    stRecipeParmInfo stTemp = m_MapHornRecipeParams[a_paramName.toLower()];
    dwIndex = stTemp.ParamIndexInStructure;
    setHornRecipeStructureValue(dwIndex,a_paramValue);
    createHornRecipe();
}

/**
 * @brief HornRecipeData::getPowerupLoadFlag
 * @return
 */
bool HornRecipeData :: getPowerupLoadFlag()
{
    return m_dataLoadedOnPowerup;
}

/**
 * @brief HornRecipeData::getParameterChangedFlag
 * @return
 */
bool HornRecipeData :: getParameterChangedFlag()
{
    return m_parameterChanged ;
}

/**
 * @brief HornRecipeData::getHornPsFreq
 * @return
 */
unsigned int HornRecipeData :: getHornPsFreq()
{
    return m_psFreq ;
}

/**
 * @brief HornRecipeData::updateDefaultParams : Updating default paramter values
 */
void HornRecipeData :: updateDefaultParams()
{
    hornRecipeDefault.freqStep = m_MapHornRecipeParams["frequency step"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipeDefault.maxAmp = m_MapHornRecipeParams["max amplitude"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipeDefault.maxCurrent = m_MapHornRecipeParams["max current"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipeDefault.SetDigTuneWithHornScan = m_MapHornRecipeParams["set digital tune with horn scan"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipeDefault.startFreq = m_MapHornRecipeParams["start frequency"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipeDefault.stopFreq = m_MapHornRecipeParams["stop frequency"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    hornRecipeDefault.timeDelay = m_MapHornRecipeParams["time delay"].ParamDefaultValue[m_CurrentSystemFreq].toDouble()*1000;
}

/**
 * @brief HornRecipeData::~HornRecipeData
 */
HornRecipeData :: ~HornRecipeData()
{
    checkbox* tmpRightParam ;
    int ind = 0 ;
    for(ind = 0 ; ind < m_rightParameters.length() ; ind++)
    {
        tmpRightParam = dynamic_cast<checkbox*>(m_rightParameters.at(ind));

        delete tmpRightParam ;
        tmpRightParam = NULL ;
    }
    m_rightParameters.clear();
}

/**
 * @brief HornRecipeData::setAxisIndex
 * @param dwIndex
 * @param isLeftSelected
 */
void HornRecipeData::setAxisIndex(int dwIndex, int isLeftSelected)
{
    QObject *ptr;
    checkbox *m_ptr;
    if(dwIndex < 0)
        dwIndex=0;
    ptr = m_rightParameters.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);

    if(isLeftSelected == 0)
        m_ptr->m_isLeftAxis = 0;
    else
        m_ptr->m_isLeftAxis = 1;
}

/**
 * @brief HornRecipeData::setCheckboxStatus : Set the checkbox status based on index
 * @param dwIndex
 * @param isChecked
 */
void HornRecipeData::setCheckboxStatus(int dwIndex, bool isChecked)
{
    QObject *ptr;
    checkbox *m_ptr;
    ptr = m_rightParameters.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    m_ptr->m_isCheckBoxSelected = isChecked;
    if(isChecked && qstrlstParamValues.at(dwIndex) != "")
        m_ptr->m_parameterVal = qstrlstParamValues.at(dwIndex);
    else
        m_ptr->m_parameterVal = "0";
}

/**
 * @brief HornRecipeData::getComboIndex
 * @param dwIndex
 * @return
 */
bool HornRecipeData::getComboIndex(int dwIndex)
{
    QObject *ptr;
    checkbox *m_ptr;
    if(dwIndex < 0)
        dwIndex = 0;
    ptr = m_rightParameters.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    if(m_ptr->m_isLeftAxis)
        return true;
    return false;
}

/**
 * @brief HornRecipeData::updateParameterVal : Update units in right parameter model
 * @param amp
 * @param curr
 * @param phase
 * @param impedence
 */
void HornRecipeData::updateParameterVal(QString amp, QString curr, QString phase, QString impedence)
{
    QObject *ptr;
    checkbox *m_ptr;

    qstrlstParamValues.clear();
    if(amp != "")
        qstrlstParamValues.append(amp);
    else
        qstrlstParamValues.append("0");

    if(curr != "")
        qstrlstParamValues.append(curr);
    else
        qstrlstParamValues.append("0");

    if(phase != "")
        qstrlstParamValues.append(phase);
    else
        qstrlstParamValues.append("0");

    if(impedence != "")
        qstrlstParamValues.append(impedence);
    else
        qstrlstParamValues.append("0");

    context->setContextProperty("rightParameterModel",QVariant::fromValue(m_rightParametersTemp));
    for(int i=0;i<4;i++)
    {
        ptr = m_rightParameters.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        if(m_ptr->m_isCheckBoxSelected)
            m_ptr->m_parameterVal = qstrlstParamValues.at(i);
        else
            m_ptr->m_parameterVal = "0";
    }
    context->setContextProperty("rightParameterModel",QVariant::fromValue(m_rightParameters));
}

/**
 * @brief HornRecipeData::setHornDefaultValues : Set horn recipe defualt parmater values
 */
void HornRecipeData::setHornDefaultValues()
{
    context->setContextProperty("rightParameterModel", QVariant::fromValue(m_dummyHornRecipe));
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("HornScanGraphCheckBoxStatus");
    QString checkValue = settings.value("HornScanCheckStatus").toString();
    settings.endGroup();
    settings.beginGroup("HornScanGraphComboBoxStatus");
    QString comboValue = settings.value("HornScanComboStatus").toString();
    settings.endGroup();
    if(checkValue.size()>1)
    {
        checkValue.remove(checkValue.length()-1,1);
        checkValue.remove(0,1);
    }
    else
    {
        checkValue="1,1,1,0";
    }
    if(comboValue.size()>1)
    {
        comboValue.remove(comboValue.length()-1,1);
        comboValue.remove(0,1);
    }
    else
    {
        comboValue="0,0,1,1";
    }

    QStringList temp1 = checkValue.split(",");
    QStringList temp2 = comboValue.split(",");
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<m_rightParameters.size();i++)
    {
        ptr = m_rightParameters.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        m_ptr->m_isCheckBoxSelected = temp1[i].toInt();
        m_ptr->m_isLeftAxis = temp2[i].toInt();
    }
    context->setContextProperty("rightParameterModel", QVariant::fromValue(m_rightParameters));
}

/**
 * @brief HornRecipeData::getHornDefaultValues : Return horn recipe default paramter values
 */
void HornRecipeData::getHornDefaultValues()
{
    QString qstrCheckOrder="",qstrComboOrder="";
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<m_rightParameters.size();i++)
    {
        ptr = m_rightParameters.at(i);
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
    settings.beginGroup("HornScanGraphCheckBoxStatus");
    settings.setValue("HornScanCheckStatus",qstrCheckOrder);
    settings.endGroup();
    settings.beginGroup("HornScanGraphComboBoxStatus");
    settings.setValue("HornScanComboStatus",qstrComboOrder);
    settings.endGroup();
}

/**
 * @brief HornRecipeData::clearList
 * @param modelName
 * @param list
 */
void HornRecipeData::clearList(const QString& modelName, QList<QObject*> list)
{
    if (!list.empty())
    {
        auto tempList = list;
        list.clear();
        context->setContextProperty(modelName, QVariant::fromValue(list));
        qDeleteAll(tempList);
        tempList.clear();
    }
}

/**
 * @brief HornRecipeData::getHornAutoGraphRefreshState
 * @return
 */
bool HornRecipeData::getHornAutoGraphRefreshState()
{
    return isHornAutoGraphRefreshState;
}

/**
 * @brief HornRecipeData::setHornAutoGraphRefreshState
 * @param state
 */
void HornRecipeData::setHornAutoGraphRefreshState(bool state)
{
    if(isHornAutoGraphRefreshState != state)
    {
        isHornAutoGraphRefreshState = state;
        emit HornAutoGraphRefreshStateChanged();
    }
}

/**
 * @brief HornRecipeData::getIsHornManualRefreshState
 * @return
 */
bool HornRecipeData::getIsHornManualRefreshState()
{
    return isHornManualRefreshState;
}

/**
 * @brief HornRecipeData::setIsHornManualRefreshState
 * @param state
 */
void HornRecipeData::setIsHornManualRefreshState(bool state)
{
    if(isHornManualRefreshState != state)
    {
        isHornManualRefreshState = state;
        emit IsHornManualRefreshStateChanged();
    }
}
