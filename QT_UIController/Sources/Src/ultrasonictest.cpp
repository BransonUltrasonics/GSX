/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Mode

***************************************************************************/
#include "Header/ultrasonictest.h"
#include "Header/localizeheader.h"
#include "Header/common.h"
#include <QSettings>
#include "Header/qtcommon.h"


extern QQmlContext* context;
extern CommunicationInterface CIObj;

extern unsigned int psFreqGlobalVar;

/**
 * @brief UltrasonicTest::UltrasonicTest : Constructor
 * @param parent
 */
UltrasonicTest::UltrasonicTest(QObject *parent) : QObject(parent)
{
    /* Refresh UI Model */
    context->setContextProperty("ultrasonictest",this);

    /* Connecting a signal from communication interface and emit the slot*/
    QObject::connect(&CIObj , &CommunicationInterface::setProgressData , this ,&UltrasonicTest :: getTestModeProgressbarDataFromSC );

    m_StartTestCommand=0;
    m_StatusText = TEST_RECIPE_READY_STATUS;

    m_StartTestCommand=0;

    TestModeProgressBar *progressbarTemp=new TestModeProgressBar(CUST_NAME_FREQUENCY,0.00,HERTZ);
    m_listTestModeProgressbar.append(progressbarTemp);
    progressbarTemp=new TestModeProgressBar(CUST_NAME_MEMORY,0.00,"");
    m_listTestModeProgressbar.append(progressbarTemp);
    progressbarTemp=new TestModeProgressBar(CUST_NAME_AMPLITUDE,0.00,PERCENTAGE);
    m_listTestModeProgressbar.append(progressbarTemp);
    progressbarTemp=new TestModeProgressBar(CUST_NAME_POWER,0.00,WATT);
    m_listTestModeProgressbar.append(progressbarTemp);

    checkbox *checkBoxtmp=new checkbox("#4285f4",CUST_NAME_FREQUENCY,HERTZ,"true","0",true,1);
    m_listTestModeCheckBoxData.append(checkBoxtmp);
    checkBoxtmp=new checkbox("#f03e3e",CUST_NAME_AMPLITUDE,PERCENTAGE,"true","0",true,0);
    m_listTestModeCheckBoxData.append(checkBoxtmp);
    checkBoxtmp=new checkbox("#a82ba8",CUST_NAME_POWER,WATT,"true","0",true,0);
    m_listTestModeCheckBoxData.append(checkBoxtmp);
    checkBoxtmp=new checkbox("#009587",CUST_NAME_CURRENT,PERCENTAGE,"true","0",true,0);
    m_listTestModeCheckBoxData.append(checkBoxtmp);
    checkBoxtmp=new checkbox("black",CUST_NAME_PHASE,DEGREE,"true","0",true,0);
    m_listTestModeCheckBoxData.append(checkBoxtmp);

    if (context)
    {
        context->setContextProperty("testProgressbarModel", QVariant::fromValue(m_listTestModeProgressbar));
        context->setContextProperty("testCheckboxModel", QVariant::fromValue(m_listTestModeCheckBoxData));
    }
    initializeTestRecipeData();
    m_CurrentSystemFreq = SYSTEM_TYPE_20KHZ;
    m_MetrixUnitSelected = UNIT_METRIC;
    getDefaultFrequencySelectionForTest();
}

void UltrasonicTest::setDefaultSystemValuesForTest(int dwMetrixUnit, int psFreqGlobalVar)
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
    getDefaultFrequencySelectionForTest();
}
void UltrasonicTest::getDefaultFrequencySelectionForTest()
{
    m_stTestRecipe.TestAmplitude = m_MapTestRecipeParams["test amplitude"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_stTestRecipe.AmplitudeLoopC1 = m_MapTestRecipeParams["amplitude loop c1"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_stTestRecipe.AmplitudeLoopC2 = m_MapTestRecipeParams["amplitude loop c2"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_stTestRecipe.PhaseLoopI = m_MapTestRecipeParams["phase loop 1"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_stTestRecipe.PhaseLoopCF = m_MapTestRecipeParams["phase loop cf"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_stTestRecipe.FrequencyHigh= m_MapTestRecipeParams["frequency high"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_stTestRecipe.FrequencyLow= m_MapTestRecipeParams["frequency low"].ParamDefaultValue[m_CurrentSystemFreq].toInt();

}
void UltrasonicTest :: initializeTestRecipeData()
{
    stRecipeParmInfo stTempParam;

    stTempParam.ParamIndexInStructure = TEST_AMPLITUDE_PER;
    stTempParam.ParamUnit[UNIT_METRIC]  = "%";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "%";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "50";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "10";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "50";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "10";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "100";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "50";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "10";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "100";
    m_MapTestRecipeParams["test amplitude"] = stTempParam;

    stTempParam.ParamIndexInStructure = TEST_AMPLITUDE_LOOP_C1;
    stTempParam.ParamUnit[UNIT_METRIC]  = "";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "100";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "205";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "205";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "20000";
    m_MapTestRecipeParams["amplitude loop c1"] = stTempParam;

    stTempParam.ParamIndexInStructure = TEST_AMPLITUDE_LOOP_C2;
    stTempParam.ParamUnit[UNIT_METRIC]  = "";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "300";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "370";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "20000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "370";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "1";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "20000";
    m_MapTestRecipeParams["amplitude loop c2"] = stTempParam;

    stTempParam.ParamIndexInStructure = TEST_PHASE_LOOP1;
    stTempParam.ParamUnit[UNIT_METRIC]  = "";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "450";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "100";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "60";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "500";
    m_MapTestRecipeParams["phase loop 1"] = stTempParam;

    stTempParam.ParamIndexInStructure = TEST_PHASE_LOOP_CF;
    stTempParam.ParamUnit[UNIT_METRIC]  = "";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "450";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "200";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "200";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "50";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "500";
    m_MapTestRecipeParams["phase loop cf"] = stTempParam;

    stTempParam.ParamIndexInStructure = TEST_FREQ_HIGH;
    stTempParam.ParamUnit[UNIT_METRIC]  = "Hz";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "Hz";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "750";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "1000";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1000";
    m_MapTestRecipeParams["frequency high"] = stTempParam;

    stTempParam.ParamIndexInStructure = TEST_FREQ_LOW;
    stTempParam.ParamUnit[UNIT_METRIC]  = "Hz";
    stTempParam.ParamUnit[UNIT_IMPERIAL] = "Hz";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMinValue[SYSTEM_TYPE_20KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_20KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamMinValue[SYSTEM_TYPE_30KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_30KHZ] = "1000";
    stTempParam.ParamDefaultValue[SYSTEM_TYPE_40KHZ] = "500";
    stTempParam.ParamMinValue[SYSTEM_TYPE_40KHZ] = "500";
    stTempParam.ParamMaxValue[SYSTEM_TYPE_40KHZ] = "1000";
    m_MapTestRecipeParams["frequency low"] = stTempParam;
}
int UltrasonicTest :: init()
{
    int retValLoadRecipe = OPERATION_SUCCESS ;
    m_psFreq = psFreqGlobalVar ;
    updateDefaultParams();
    retValLoadRecipe = reqToLoadRecipeData();
    if( retValLoadRecipe != OPERATION_SUCCESS)
    {
        resetTestRecipeData();
    }
    else
    {
        createTestRecipe();
    }
    return retValLoadRecipe ;
}

QString UltrasonicTest :: getTestRecipeParameterNames()
{
    QString paramNames;
    paramNames.append("Test Amplitude");


    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        paramNames.append(",Amplitude Loop C1,"
                          "Amplitude Loop C2,"
                          "Phase loop 1,"
                          "Phase loop CF,"
                          "Frequency High,"
                          "Frequency Low");
    }
    return paramNames;
}

QString UltrasonicTest :: getTestRecipeLocaleParameterNames()
{
    QString paramNames;
    paramNames.append(TEST_RECIPE_TEST_AMPLITUDE);

    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        paramNames.append(COMMA);
        paramNames.append(TEST_RECIPE_AMPLITUDE_C1);
        paramNames.append(COMMA);
        paramNames.append(TEST_RECIPE_AMPLITUDE_C2);
        paramNames.append(COMMA);
        paramNames.append(TEST_RECIPE_PHASE_LOOP);
        paramNames.append(COMMA);
        paramNames.append(TEST_RECIPE_PHASE_CF);
        paramNames.append(COMMA);
        paramNames.append(TEST_RECIPE_FREQUENCY_HIGH);
        paramNames.append(COMMA);
        paramNames.append(TEST_RECIPE_FREQUENCY_LOW);
    }
    return paramNames;
}

void UltrasonicTest :: createTestRecipe()
{
    context->setContextProperty("testModeAdvancedData",QVariant::fromValue(m_dummyListTestModeCheckBoxData));
    for(int element=0; element <m_testModeAdvancedData.size() ; element++)
    {
        TestRecipeModel *ptrRecipeParameter = dynamic_cast<TestRecipeModel*>(m_testModeAdvancedData.at(element));
        delete ptrRecipeParameter;
        ptrRecipeParameter = NULL;
    }
    m_testModeAdvancedData.clear();

    QString m_recipeParameters = getTestRecipeParameterNames();
    QStringList m_recipeParametersList = m_recipeParameters.split(COMMA);
    QString m_recipeParametersLocale = getTestRecipeLocaleParameterNames();
    QStringList m_recipeParametersListLocale = m_recipeParametersLocale.split(COMMA);
    QString paramVal;
    for(int dwIndex = 0; dwIndex < m_recipeParametersList.count(); dwIndex++)
    {
        stRecipeParmInfo stTemp = m_MapTestRecipeParams[m_recipeParametersList[dwIndex].toLower()];
        TestRecipeModel *testRecipe = new TestRecipeModel;
        testRecipe->m_parameterName = m_recipeParametersList[dwIndex];
        testRecipe->m_parameterLocaleName = m_recipeParametersListLocale[dwIndex];
        testRecipe->m_parameterUnit = stTemp.ParamUnit[m_MetrixUnitSelected];
        testRecipe->m_parameterMinValue = stTemp.ParamMinValue[m_CurrentSystemFreq];
        testRecipe->m_parameterMaxValue = stTemp.ParamMaxValue[m_CurrentSystemFreq];
        paramVal = getTestRecipeStructureVal(dwIndex);

        testRecipe->m_parameterValue = paramVal.toInt();
        m_testModeAdvancedData.append(testRecipe);
    }
    context->setContextProperty("testModeAdvancedData",QVariant::fromValue(m_testModeAdvancedData));
}

QString UltrasonicTest :: getTestRecipeStructureVal(int dwParamIndex)
{
    QString qstrRetValue="";
    switch(dwParamIndex)
    {
    case TEST_AMPLITUDE_PER:
        qstrRetValue = QString::number(m_stTestRecipe.TestAmplitude);
        break;
    case TEST_AMPLITUDE_LOOP_C1:
        qstrRetValue = QString::number(m_stTestRecipe.AmplitudeLoopC1);
        break;
    case TEST_AMPLITUDE_LOOP_C2:
        qstrRetValue = QString::number(m_stTestRecipe.AmplitudeLoopC2);
        break;
    case TEST_PHASE_LOOP1:
        qstrRetValue = QString::number(m_stTestRecipe.PhaseLoopI);
        break;
    case TEST_PHASE_LOOP_CF:
        qstrRetValue = QString::number(m_stTestRecipe.PhaseLoopCF);
        break;
    case TEST_FREQ_HIGH:
        qstrRetValue = QString::number(m_stTestRecipe.FrequencyHigh);
        break;
    case TEST_FREQ_LOW:
        qstrRetValue = QString::number(m_stTestRecipe.FrequencyLow);
        break;
    default :
        qstrRetValue = "";
        break;
    }
    return qstrRetValue;
}

void UltrasonicTest :: setTestRecipeStructureValue(qint8 a_paramIndex, QString a_paramVal)
{
    int dwParamIndex = a_paramIndex;
    QString paramValue = a_paramVal;


    switch(dwParamIndex)
    {
    case TEST_AMPLITUDE_PER:
        m_st_tempTestRecipe.TestAmplitude=m_stTestRecipe.TestAmplitude;
        m_stTestRecipe.TestAmplitude = paramValue.toInt();
        break;
    case TEST_AMPLITUDE_LOOP_C1:
        m_st_tempTestRecipe.AmplitudeLoopC1=m_stTestRecipe.AmplitudeLoopC1;
        m_stTestRecipe.AmplitudeLoopC1 = paramValue.toInt();
        break;
    case TEST_AMPLITUDE_LOOP_C2:
        m_st_tempTestRecipe.AmplitudeLoopC2=m_stTestRecipe.AmplitudeLoopC2;
        m_stTestRecipe.AmplitudeLoopC2 = paramValue.toInt();
        break;
    case TEST_PHASE_LOOP1:
        m_st_tempTestRecipe.PhaseLoopI=m_stTestRecipe.PhaseLoopI;
        m_stTestRecipe.PhaseLoopI = paramValue.toInt();
        break;
    case TEST_PHASE_LOOP_CF:
        m_st_tempTestRecipe.PhaseLoopCF=m_stTestRecipe.PhaseLoopCF;
        m_stTestRecipe.PhaseLoopCF = paramValue.toInt();
        break;
    case TEST_FREQ_HIGH:
        m_st_tempTestRecipe.FrequencyHigh=m_stTestRecipe.FrequencyHigh;
        m_stTestRecipe.FrequencyHigh = paramValue.toInt();
        break;
    case TEST_FREQ_LOW:
        m_st_tempTestRecipe.FrequencyLow=m_stTestRecipe.FrequencyLow;
        m_stTestRecipe.FrequencyLow = paramValue.toInt();
        break;
    default :
        break;
    }
    createTestRecipe();
}


/**
  * @brief sendTestRecipeDataToSC:- sends Test Recipe Data To SC
  * @return Status from SC
*/
bool UltrasonicTest::sendTestRecipeDataToSC()
{
    setStatusText("Running");
    char ch[4]={'\0'};
    char *m_data=reinterpret_cast<char*>(&m_stTestRecipe);

    CIObj.sendMessage(SCBL_TEST_RECIPE_REQ,UIC_TEST_RECIPE_RES,m_data,sizeof(m_stTestRecipe));
    CIObj.recivedMessage(UIC_TEST_RECIPE_RES,ch,1);

    memcpy(&m_StartTestCommand,ch,1);
    if(m_StartTestCommand=='1')
        return true;
    return false;
}


void UltrasonicTest :: updateDefaultParams()
{
    m_TestRecipeStrDefault.TestAmplitude = m_MapTestRecipeParams["test amplitude"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_TestRecipeStrDefault.AmplitudeLoopC1 = m_MapTestRecipeParams["amplitude loop c1"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_TestRecipeStrDefault.AmplitudeLoopC2 = m_MapTestRecipeParams["amplitude loop c2"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_TestRecipeStrDefault.PhaseLoopI = m_MapTestRecipeParams["phase loop 1"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_TestRecipeStrDefault.PhaseLoopCF = m_MapTestRecipeParams["phase loop cf"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_TestRecipeStrDefault.FrequencyHigh= m_MapTestRecipeParams["frequency high"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
    m_TestRecipeStrDefault.FrequencyLow= m_MapTestRecipeParams["frequency low"].ParamDefaultValue[m_CurrentSystemFreq].toInt();
}

void UltrasonicTest :: resetTestRecipeData()
{
    updateDefaultParams();
    m_stTestRecipe.TestAmplitude    =    m_TestRecipeStrDefault.TestAmplitude  ;
    m_stTestRecipe.AmplitudeLoopC1  =    m_TestRecipeStrDefault.AmplitudeLoopC1;
    m_stTestRecipe.AmplitudeLoopC2  =    m_TestRecipeStrDefault.AmplitudeLoopC2;
    m_stTestRecipe.PhaseLoopI       =    m_TestRecipeStrDefault.PhaseLoopI     ;
    m_stTestRecipe.PhaseLoopCF      =    m_TestRecipeStrDefault.PhaseLoopCF    ;
    m_stTestRecipe.FrequencyHigh    =    m_TestRecipeStrDefault.FrequencyHigh  ;
    m_stTestRecipe.FrequencyLow     =    m_TestRecipeStrDefault.FrequencyLow   ;
    createTestRecipe();
}

/**
  * @brief getTestModeAdvancedDataFromDB:- gets Test Mode Data from Database
*/
void UltrasonicTest::getTestModeAdvancedDataFromDB()
{
    int frequency=psFreqGlobalVar;
    QString tempRecipeData;

    CIObj.sendMessage(REQ_GET_TEST_RECIPE_DATA, RES_GET_TEST_RECIPE_DATA , QString::number(frequency));
    CIObj.recivedMessage(RES_GET_TEST_RECIPE_DATA , tempRecipeData);

    if(!tempRecipeData.isEmpty())
    {
        QStringList query=tempRecipeData.split(',');
        if(tempRecipeData.length()!=0 || query.size() == 7)
        {
            m_stTestRecipe.TestAmplitude           = query.at(0).toInt();
            m_stTestRecipe.AmplitudeLoopC1         = query.at(1).toInt();
            m_stTestRecipe.AmplitudeLoopC2         = query.at(2).toInt();
            m_stTestRecipe.PhaseLoopI              = query.at(3).toInt();
            m_stTestRecipe.PhaseLoopCF             = query.at(4).toInt();
            m_stTestRecipe.FrequencyHigh           = query.at(5).toInt();
            m_stTestRecipe.FrequencyLow            = query.at(6).toInt();

            m_st_tempTestRecipe.TestAmplitude      = query.at(0).toInt();
            m_st_tempTestRecipe.AmplitudeLoopC1    = query.at(1).toInt();
            m_st_tempTestRecipe.AmplitudeLoopC2    = query.at(2).toInt();
            m_st_tempTestRecipe.PhaseLoopI         = query.at(3).toInt();
            m_st_tempTestRecipe.PhaseLoopCF        = query.at(4).toInt();
            m_st_tempTestRecipe.FrequencyHigh      = query.at(5).toInt();
            m_st_tempTestRecipe.FrequencyLow       = query.at(6).toInt();
            createTestRecipe();
        }
        else
        {
            resetTestRecipeData();
        }
    }
    else
        return;
}

/**
  * @brief saveTestRecipeData:-saves Test Recipe Data to Database
*/
bool UltrasonicTest::saveTestRecipeData()
{
    m_st_tempTestRecipe= m_stTestRecipe;
    int frequency=psFreqGlobalVar;
    QString sendToDb=QString::number(frequency)+QString::fromStdString(",")+QString::number(m_stTestRecipe.AmplitudeLoopC1)+","+QString::number(m_stTestRecipe.AmplitudeLoopC2)+","+
            QString::number(m_stTestRecipe.PhaseLoopI)+","+QString::number(m_stTestRecipe.PhaseLoopCF)+","+QString::number(m_stTestRecipe.FrequencyHigh)+","+
            QString::number(m_stTestRecipe.FrequencyLow)+","+QString::number(m_stTestRecipe.TestAmplitude);
    CIObj.sendMessage(REQ_SET_TEST_RECIPE_DATA,RES_SET_TEST_RECIPE_DATA,sendToDb);
    QString result="";
    CIObj.recivedMessage(RES_SET_TEST_RECIPE_DATA,result);
    createTestRecipe();
    if(!result.isEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief UltrasonicTest::reqToLoadRecipeData : Send message to get default horn recipe
 * @return
 */
int UltrasonicTest :: reqToLoadRecipeData()
{
    int paramInd = 0 ;
    m_psFreq = psFreqGlobalVar ;
    QStringList testRecipeFromDb;
    QString testRecipeMsg;
    QString stringRead ;
    int retValue = OPERATION_SUCCESS ;

    /* Send message to get default horn recipe */
    CIObj.sendMessage(REQ_GET_TEST_RECIPE_DATA , RES_GET_TEST_RECIPE_DATA , QString::number(m_psFreq));
    CIObj.recivedMessage(RES_GET_TEST_RECIPE_DATA , testRecipeMsg);

    /* Filter the string with the delimiter */
    testRecipeFromDb = testRecipeMsg.split(',');

    /* Check for the length of the msg received */
    if(7 != testRecipeFromDb.length())
    {
        retValue = NO_PARAM ;
    }
    else
    {
        /* Check the range and update the parameters with the current values */
        for(paramInd = 0 ; paramInd < testRecipeFromDb.length() && paramInd < 7 ; paramInd++)
        {
            stringRead = testRecipeFromDb.at(paramInd);
            setTestRecipeStructureValue(paramInd,stringRead );
        }
    }
    return retValue ;
}

/**
  * @brief cancelTestRecipeData:-cancels Test Recipe Data,changed by user
*/
void UltrasonicTest::cancelTestRecipeData()
{
    int retValLoadRecipe = OPERATION_SUCCESS ;
    retValLoadRecipe = reqToLoadRecipeData();
    if( retValLoadRecipe != OPERATION_SUCCESS)
    {
        resetTestRecipeData();
    }
    else
    {
        createTestRecipe();
    }
}

void UltrasonicTest :: modifyParameter(QString a_paramValue , QString a_paramName)
{
    int dwIndex;
    stRecipeParmInfo stTemp = m_MapTestRecipeParams[a_paramName.toLower()];
    dwIndex = stTemp.ParamIndexInStructure;
    setTestRecipeStructureValue(dwIndex,a_paramValue);
    createTestRecipe();
}

/**
  * @brief startTest:- to start test
*/
void UltrasonicTest::startTest()
{
    char ch[4]={'\0'};
    CIObj.sendMessage(SCBL_TEST_CMD_REQ,UIC_TEST_CMD_RES,ch,0);
}

/**
  * @brief setCheckBoxModelData sets the check box model
  */
void UltrasonicTest::setCheckBoxModelData()
{
    context->setContextProperty("testCheckboxModel", QVariant::fromValue(m_listTestModeCheckBoxData));
}

/**
  * @brief rangeCheck:-checks whether changed value is within the range or not
  * @param a_value:-value that has to be checked
  * @param a_index:-index of parameter
*/
QString UltrasonicTest::rangeCheck( QString keypadValue , QString keypadTextName)
{
    stRecipeParmInfo stTemp = m_MapTestRecipeParams[keypadTextName.toLower()];
    double currentValue = keypadValue.toDouble();
    double minValue = stTemp.ParamMinValue[m_CurrentSystemFreq].toDouble();
    double maxValue = stTemp.ParamMaxValue[m_CurrentSystemFreq].toDouble();
    int dwIndex = stTemp.ParamIndexInStructure;
    double freqVal;
    if(currentValue < minValue)
    {
        return getTestRecipeStructureVal(dwIndex);
    }
    else if(currentValue > maxValue)
    {
        return getTestRecipeStructureVal(dwIndex);
    }

    if(keypadTextName.toLower() == "frequency low")
    {
        freqVal = getTestRecipeStructureVal(TEST_FREQ_HIGH).toDouble();
        if(currentValue >= freqVal)
            return getTestRecipeStructureVal(dwIndex);
        else
            return keypadValue;
    }
    else if(keypadTextName.toLower() == "frequency high")
    {
        freqVal = getTestRecipeStructureVal(TEST_FREQ_LOW).toDouble();
        if(currentValue <= freqVal)
            return getTestRecipeStructureVal(dwIndex);
        else
            return keypadValue;
    }
    else
    {
        return keypadValue;
    }
}

/**
 * @brief getStatusText :returns status of ultrasonic test
 * @return QString
 */
QString UltrasonicTest::getStatusText()
{
    return m_StatusText;
}

/**
 * @brief setStatusText :sets status of ultrasonic test
 * @param a_Status :status to be set
 */
void UltrasonicTest::setStatusText(QString a_Status)
{
    if(m_StatusText!=a_Status)
    {
        m_StatusText=a_Status;
        statusTextChanged();
    }
}

/**
  * @brief sendTestResponseToSC:-sends Test Response To SC
  * @return bool:status of abort command
*/
bool UltrasonicTest::sendTestResponseToSC()
{
    m_StartTestCommand=0;
    char requestMessage[] = "";

    CIObj.sendMessage(SCBL_TEST_ABORT_CMD_REQ  ,UIC_TEST_ABORT_CMD_RES, requestMessage, 0);
    char ch[4]={'\0'};
    CIObj.recivedMessage(UIC_TEST_ABORT_CMD_RES,ch,1);
    if(ch[0]=='1')
    {
        setStatusText("Success");
        return true;
    }
    else
    {
        setStatusText("Failed");
        return false;
    }
    return false;
}

/**
  * @brief getTestModeProgressbarDataFromSC:-gets Test Mode Progress bar Data From SC
*/
void UltrasonicTest::getTestModeProgressbarDataFromSC(QString a_ResultData)
{
    if(a_ResultData.size()==0)
    {
        for(int index=0;index<m_listTestModeProgressbar.size();index++)
        {
            TestModeProgressBar* progressBar=dynamic_cast<TestModeProgressBar*>(m_listTestModeProgressbar.at(index));
            progressBar->setParamValue(0);

        }
        context->setContextProperty("testProgressbarModel", QVariant::fromValue(m_listTestModeProgressbar));

        return;
    }
    else
    {
        QStringList query=a_ResultData.split(',');
        if(query.size()!=4){

            for(int index=0;index<m_listTestModeProgressbar.size();index++)
            {
                TestModeProgressBar* progressBar=dynamic_cast<TestModeProgressBar*>(m_listTestModeProgressbar.at(index));
                progressBar->setParamValue(0);

            }

            context->setContextProperty("testProgressbarModel", QVariant::fromValue(m_listTestModeProgressbar));

            return;
        }
        else
        {
            for(int index=0;index<m_listTestModeProgressbar.size();index++)
            {
                TestModeProgressBar* progressBar=dynamic_cast<TestModeProgressBar*>(m_listTestModeProgressbar.at(index));
                progressBar->setParamValue(query.at(index).toDouble());
            }
        }
    }
    context->setContextProperty("testProgressbarModel", QVariant::fromValue(m_listTestModeProgressbar));
}

/**
  * @brief setCheckboxStatus:-sets the checkbox status in testmode
  * @param dwIndex:-index of checkbox
  * @param isChecked:-status
*/
void UltrasonicTest::setCheckboxStatus(int dwIndex, bool isChecked)
{
    QObject *ptr;
    checkbox *m_ptr;
    ptr = m_listTestModeCheckBoxData.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    m_ptr->setIsCheckBoxSelected(isChecked);
}

/**
  * @brief setAxisIndex:-sets the Axis Position info whether it is in Left or not in testmode
  * @param dwIndex:-index of combobox
  * @param isLeftSelected:-status
*/
void UltrasonicTest::setAxisIndex(int dwIndex, int isLeftSelected)
{
    QObject *ptr;
    checkbox *m_ptr;
    ptr = m_listTestModeCheckBoxData.at(dwIndex);
    m_ptr = reinterpret_cast<checkbox*>(ptr);
    m_ptr->setIsLeftAxisSelected(isLeftSelected);
}

/**
 * @brief UltrasonicTest::setTestDefaultValues
 */
void UltrasonicTest::setTestDefaultValues()
{
    context->setContextProperty("testCheckboxModel", QVariant::fromValue(m_dummyListTestModeCheckBoxData));
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("TestGraphCheckBoxStatus");
    QString checkValue = settings.value("TestCheckStatus").toString();
    settings.endGroup();
    settings.beginGroup("TestGraphComboBoxStatus");
    QString comboValue = settings.value("TestComboStatus").toString();
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
        comboValue="0,0,0,1,1";
    }

    QStringList temp1 = checkValue.split(",");
    QStringList temp2 = comboValue.split(",");
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<m_listTestModeCheckBoxData.size();i++)
    {
        ptr = m_listTestModeCheckBoxData.at(i);
        m_ptr = reinterpret_cast<checkbox*>(ptr);
        m_ptr->m_isCheckBoxSelected = temp1[i].toInt();
        m_ptr->m_isLeftAxis = temp2[i].toInt();
    }
    context->setContextProperty("testCheckboxModel", QVariant::fromValue(m_listTestModeCheckBoxData));
}

/**
 * @brief UltrasonicTest::getTestDefaultValues
 */
void UltrasonicTest::getTestDefaultValues()
{
    QString qstrCheckOrder="",qstrComboOrder="";
    QObject *ptr;
    checkbox *m_ptr;
    for(int i=0;i<m_listTestModeCheckBoxData.size();i++)
    {
        ptr = m_listTestModeCheckBoxData.at(i);
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
    settings.beginGroup("TestGraphCheckBoxStatus");
    settings.setValue("TestCheckStatus",qstrCheckOrder);
    settings.endGroup();
    settings.beginGroup("TestGraphComboBoxStatus");
    settings.setValue("TestComboStatus",qstrComboOrder);
    settings.endGroup();
}
