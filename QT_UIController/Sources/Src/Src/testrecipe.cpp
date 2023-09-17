/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Recipe

***************************************************************************/
#include <Header/testrecipe.h>

extern unsigned int psFreqGlobalVar;

/**
 * @brief TestRecipeModel::TestRecipeModel : Initialize the parameter values to particulars
 * @param a_parameterName
 * @param a_parameterVal
 * @param a_parameterUnit
 * @param parent
 */
TestRecipeModel :: TestRecipeModel(const QString &a_parameterName, const QString &a_parameterLocaleName, const qint32 &a_parameterVal, const QString &a_parameterUnit,const QString &a_parameterMinVal,
                                   const QString &a_parameterMaxVal,QObject *parent):QObject(parent)
{
    m_parameterName = a_parameterName;
    m_parameterLocaleName = a_parameterLocaleName;
    m_parameterValue = a_parameterVal;
    m_parameterUnit = a_parameterUnit;
    m_parameterMinValue = a_parameterMinVal;
    m_parameterMaxValue = a_parameterMaxVal;
}

TestRecipeModel::TestRecipeModel(QObject *parent) : QObject(parent)
{

}

/**
 * @brief TestRecipeModel::~TestRecipeModel
 */
TestRecipeModel :: ~TestRecipeModel()
{
}

/**
 * @brief TestRecipeModel::getParameterName : Return parameter name
 * @return
 */
QString TestRecipeModel :: getParameterName()
{
    return m_parameterName;
}

/**
 * @brief TestRecipeModel::getParameterLocaleName : Return parameter name
 * @return
 */
QString TestRecipeModel :: getParameterLocaleName()
{
    return m_parameterLocaleName;
}

/**
 * @brief TestRecipeModel::getParameterUnit : Return Parameter Unit
 * @return
 */
QString TestRecipeModel :: getParameterUnit()
{
    return m_parameterUnit;
}

/**
 * @brief TestRecipeModel::getParameterValue : Return parameter value
 * @return
 */
qint32 TestRecipeModel :: getParameterValue()
{
    return m_parameterValue ;
}

/**
 * @brief TestRecipeModel::setParameterValue : Set parameter value
 * @param a_val
 */
void TestRecipeModel :: setParameterValue(qint32 a_val)
{
    m_parameterValue = a_val;
}

/**
 * @brief TestRecipeModel::getParameterMinValue : Return minimum value
 * @return
 */
QString TestRecipeModel :: getParameterMinValue()
{
    return m_parameterMinValue;
}

/**
 * @brief TestRecipeModel::getParameterMaxValue : Return maximum value
 * @return
 */
QString TestRecipeModel :: getParameterMaxValue()
{
    return m_parameterMaxValue;
}
/**
 * @brief TestRecipeData::TestRecipeData : set the default values or units to corresponding Hertz
 * @param parent
 */
TestRecipeData::TestRecipeData(QObject *parent):QObject(parent)
{

    TestRecipe tempTestRecipe20Hz;
    tempTestRecipe20Hz.AmplitudeLoopC1=100;
    tempTestRecipe20Hz.AmplitudeLoopC2=300;
    tempTestRecipe20Hz.PhaseLoopI=450;
    tempTestRecipe20Hz.PhaseLoopCF=450;
    tempTestRecipe20Hz.FrequencyHigh=500;
    tempTestRecipe20Hz.FrequencyLow=500;
    tempTestRecipe20Hz.TestAmplitude=10;

    m_AdvancedDataDefaultList.insert(std::pair<int ,TestRecipe>(20000,tempTestRecipe20Hz)) ;

    TestRecipe tempTestRecipe30Hz;
    tempTestRecipe30Hz.AmplitudeLoopC1=205;
    tempTestRecipe30Hz.AmplitudeLoopC2=370;
    tempTestRecipe30Hz.PhaseLoopI=100;
    tempTestRecipe30Hz.PhaseLoopCF=200;
    tempTestRecipe30Hz.FrequencyHigh=750;
    tempTestRecipe30Hz.FrequencyLow=750;
    tempTestRecipe30Hz.TestAmplitude=10;

    m_AdvancedDataDefaultList.insert(std::pair<int ,TestRecipe>(30000,tempTestRecipe30Hz)) ;

    TestRecipe tempTestRecipe40Hz;
    tempTestRecipe40Hz.AmplitudeLoopC1=100;
    tempTestRecipe40Hz.AmplitudeLoopC2=300;
    tempTestRecipe40Hz.PhaseLoopI=450;
    tempTestRecipe40Hz.PhaseLoopCF=450;
    tempTestRecipe40Hz.FrequencyHigh=1000;
    tempTestRecipe40Hz.FrequencyLow=1000;
    tempTestRecipe40Hz.TestAmplitude=10;

    m_AdvancedDataDefaultList.insert(std::pair<int ,TestRecipe>(40000,tempTestRecipe40Hz)) ;
}
/**
 * @brief TestRecipeData::operator =
 * @param a_TestRecipeData
 * @return
 */

TestRecipeData &TestRecipeData::operator =(TestRecipeData a_TestRecipeData)
{
    m_stTestRecipe.AmplitudeLoopC1=a_TestRecipeData.m_stTestRecipe.AmplitudeLoopC1;
    m_stTestRecipe.AmplitudeLoopC2=a_TestRecipeData.m_stTestRecipe.AmplitudeLoopC2;
    m_stTestRecipe.FrequencyHigh=a_TestRecipeData.m_stTestRecipe.FrequencyLow;
    m_stTestRecipe.FrequencyLow=a_TestRecipeData.m_stTestRecipe.PhaseLoopCF;
    m_stTestRecipe.PhaseLoopCF=a_TestRecipeData.m_stTestRecipe.PhaseLoopCF;
    m_stTestRecipe.PhaseLoopI=a_TestRecipeData.m_stTestRecipe.PhaseLoopI;
    m_stTestRecipe.TestAmplitude=a_TestRecipeData.m_stTestRecipe.TestAmplitude;

    return *this;
}
