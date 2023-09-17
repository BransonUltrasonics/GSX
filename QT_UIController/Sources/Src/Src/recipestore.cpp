/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Weld Recipe data store from UI

***************************************************************************/

#include <QList>
#include <QQmlContext>
#include <QDateTime>
#include <QTime>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include "Header/communicationinterface.h"
#include "Header/pressureparameter.h"
#include <Header/recipe.h>
#include <Header/graphtableview.h>
#include "Header/informationpanel.h"
#include "Header/configuration.h"
#include <Header/setupparameter.h>
#include "Header/localizeheader.h"
#include "Header/helperenums.h"
#include "Header/menuhandler.h"
#include "Header/login.h"
#include "Header/stackrecipedata.h"

extern CommunicationInterface CIObj;
#include "Header/utility.h"
extern unsigned int psFreqGlobalVar;
extern Login *login;
extern UserPrivilege *userPrivilageObj;
extern InformationPanel *PrimaryData;
/*dwCurrentUnit declared to update current system unit*/
extern int dwCurrentUnit;

/**
 * @brief Recipe::getRecipeList
 * @return
 */
QString Recipe::getRecipeList()
{
    QString qstrReceivedBuffer="";
    QString qstrSupportedModes = getWeldModesListIndexes();

    CIObj.sendMessage(REQ_WELD_RECIPE_LIST,RES_WELD_RECIPE_LIST,qstrSupportedModes);
    CIObj.recivedMessage(RES_WELD_RECIPE_LIST,qstrReceivedBuffer);
    return qstrReceivedBuffer;
}

/**
 * @brief Recipe::loadRecipeCards : Numbers of recipe in the list
 */
void Recipe::loadRecipeCards()
{
    refreshModel(ENUM_DummyRecipeModelList);
    clearList(&m_RecipeModelList);
    QString qstrRecipesList = getRecipeList();
    QStringList qstrLstRecipeInfoList=qstrRecipesList.split(',');
    int dwRecipeListCnt = qstrLstRecipeInfoList.size();
    if(qstrRecipesList != "" && dwRecipeListCnt%RECIPE_LIST_PARAM_COUNT ==0)
    {
        int dwRecipesCnt = dwRecipeListCnt/RECIPE_LIST_PARAM_COUNT;
        for(int dwRecipeIndex = 0; dwRecipeIndex < dwRecipesCnt; dwRecipeIndex++)
        {
            RecipeInfo* ptrTempRecipeInfo = new RecipeInfo;
            ptrTempRecipeInfo->m_RecipeNumber = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_RECIPE_NUMBER);
            ptrTempRecipeInfo->m_RecipeName = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_RECIPE_NAME);
            ptrTempRecipeInfo->m_CompanyName = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_COMPANY_NAME);
            ptrTempRecipeInfo->m_WeldMode = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_WELD_MODE);
            ModeInfo stModeInfo = m_WeldRecipeParam.getModeInfoByIndex(ptrTempRecipeInfo->m_WeldMode.toInt(),qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_WELD_MODE_VALUE));
            ptrTempRecipeInfo->m_WeldMode=stModeInfo.ModeName;
            ptrTempRecipeInfo->m_WeldModeUnit = stModeInfo.ModeUnit;
            ptrTempRecipeInfo->m_WeldModeValue = stModeInfo.ModeValue;
            ptrTempRecipeInfo->m_WeldModeNameScreen = stModeInfo.ModeNameScreen;
            ptrTempRecipeInfo->m_isActive = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_ACTIVE_STATE).toInt();
            ptrTempRecipeInfo->m_isLockedValue = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_LOCKED_STATE);
            ptrTempRecipeInfo->m_isValidate = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_VALIDATE_STATE).toInt();
            ptrTempRecipeInfo->m_weldRecipeNumber = qstrLstRecipeInfoList.at((dwRecipeIndex*RECIPE_LIST_PARAM_COUNT) + RECIPE_INFO_RECIPE_VERSION_NUMBER);
            if(m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER) == ptrTempRecipeInfo->m_RecipeNumber && m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER) == "0" && m_WeldRecipeParam.getStructureValueByIndex(UI_IS_MODIFIED) == "1" )
            {
                ptrTempRecipeInfo->m_isModified = true;
                stModifiedRecipeInfo stTemp = getCurrentRecipeInfo();
                ptrTempRecipeInfo->m_RecipeName = stTemp.qstrRecipeName;
                ptrTempRecipeInfo->m_WeldMode = stTemp.qstrWeldMode;
                ptrTempRecipeInfo->m_WeldModeNameScreen = stTemp.qstrModeNameScreen;
                ptrTempRecipeInfo->m_WeldModeUnit = stTemp.qstrUnit;
                ptrTempRecipeInfo->m_WeldModeValue = stTemp.qstrModeValue;
            }
            else
            {
                ptrTempRecipeInfo->m_isModified = false;
            }
            m_RecipeModelList.append(ptrTempRecipeInfo );
        }
    }
    RecipeInfo* recipeinformation=new RecipeInfo;
    recipeinformation->m_RecipeName = "";
    recipeinformation->m_RecipeNumber = "";
    m_RecipeModelList.append(recipeinformation);

    refreshModel(ENUM_RecipeModelList);
}

/**
 * @brief Recipe::getNewRecipeNumber : New recipe check and create buffer value
 * @return
 */
int Recipe::getNewRecipeNumber()
{
    QString recipeNumberBuffer = getMaxRecipeNumberFromDb();
    if(recipeNumberBuffer.isNull()||recipeNumberBuffer.isEmpty())
        recipeNumberBuffer="0";
    return recipeNumberBuffer.toInt()+1;
}

/**
 * @brief Recipe::loadDataForModel : Parameter recipe model to correspoding List
 * @param dwModelIndex
 */
void Recipe::loadDataForModel(int dwModelIndex)
{
    switch(dwModelIndex)
    {
    case 1:
        loadPreTriggerModel();
        loadEnergyBrakeModel();
        loadAfterBurstModel();
        break;
    case 2:
        loadAtoZParamModel();
        break;
    case 3:
        loadSetupLimitsModel();
        loadControlLimitsModel();
        loadSuspectRejectLimitsModel();
        break;
    case 4:
        loadBransonParamsModel();
        break;
    default:
        break;
    }
}

/**
 * @brief Recipe::editWeldRecipePramValue : Edit recipe name and value
 * @param qstrParamName
 * @param qstrParamValue
 */
void Recipe::editWeldRecipePramValue(QString qstrParamName, QString qstrParamValue)
{
    m_WeldRecipeParam.setStructureVariableByName(qstrParamName,qstrParamValue);
    StackRecipeData::RecipeNumber=m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
}

/**
 * @brief Recipe::editFixedRecipeParam : Edited recipe name and value to load in List
 * @param qstrParamName
 * @param qstrParamValue
 */
void Recipe::editFixedRecipeParam(QString qstrParamName, QString qstrParamValue)
{
    editWeldRecipePramValue(qstrParamName,qstrParamValue);
    loadFixedParamModel();
    StackRecipeData::RecipeNumber=m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
}

/**
 * @brief Recipe::setSelectedRecipeToActive : Check the selected recipe active state and make Active
 * @param qStrRecipeNum
 */
void Recipe::setSelectedRecipeToActive(QString qStrRecipeNum)
{
    QString qstrCurrentRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER);
    QString qstrCurrentRecipeVerNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER);
    int dwCurrentRecipeActiveStatus = m_WeldRecipeParam.getStructureValueByIndex(UI_ISACTIVE).toInt();
    StackRecipeData::RecipeNumber=qStrRecipeNum.toInt();

    if((qstrCurrentRecipeNumber == qStrRecipeNum && dwCurrentRecipeActiveStatus == 1) ||(qstrCurrentRecipeNumber == "0" && qstrCurrentRecipeVerNumber == qStrRecipeNum))
    {
        m_WeldRecipeParam.sendRecipeToSC();
        return;
    }

    CIObj.sendMessage(REQ_SET_ACTIVE_RECIPE,RES_SET_ACTIVE_RECIPE,qStrRecipeNum);
    QString a_recipeBuffer;
    CIObj.recivedMessage(RES_SET_ACTIVE_RECIPE,a_recipeBuffer);
    if(qstrCurrentRecipeNumber != qStrRecipeNum)
    {
        m_WeldRecipeParam.readRecipeFromDB(qStrRecipeNum);
    }
    m_WeldRecipeParam.sendRecipeToSC();
    m_WeldRecipeParam.setStructureVariableByIndex(UI_ISACTIVE,1);
    for(int count=0;count<m_RecipeModelList.size();count++)
    {
        RecipeInfo* ptrTempRecipeInfo = dynamic_cast<RecipeInfo *>(m_RecipeModelList.at(count));
        if(ptrTempRecipeInfo->m_RecipeNumber == qStrRecipeNum)
        {
            ptrTempRecipeInfo->m_isActive=true;
        }
        else
        {
            ptrTempRecipeInfo->m_isActive=false;
        }
    }
    return;
}

/**
 * @brief Recipe::getStructureValueByName
 * @param paramEditText
 * @return
 */
QString Recipe::getStructureValueByName(QString paramEditText)
{
    return m_WeldRecipeParam.getStructureValueByName(paramEditText);
}

/**
 * @brief Recipe::getMaxValue : Returns max values of a parameter
 * @param paramEditText
 * @return
 */
QString Recipe::getMaxValue(QString paramEditText)
{
    return m_WeldRecipeParam.getMaxValue(paramEditText);
}

/**
 * @brief Recipe::getStructureValue
 * @param dwIndex
 * @return
 */
int Recipe::getStructureValue(int dwIndex)
{
    return m_WeldRecipeParam.getStructureValueByIndex(dwIndex).toDouble();
}

/**
 * @brief Recipe::setStructureValue : Assign parameter names to particular structure
 * @param qstrParamName
 * @param qstrValue
 * @param bSendToBL
 */
void Recipe::setStructureValue(QString qstrParamName,QString qstrValue,bool bSendToBL)
{
    m_WeldRecipeParam.setStructureVariableByName(qstrParamName,qstrValue);
    if(bSendToBL)
    {
        int dwRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
        int dwRecipeVerNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER).toInt();

        if(dwRecipeNumber != 0)
        {
            m_WeldRecipeParam.setStructureVariableByIndex(UI_RECIPE_VERSION_NUMBER, dwRecipeVerNumber);
            m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER, dwRecipeNumber);
            m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_NUMBER, 0);
            m_WeldRecipeParam.m_ParamaterChangedFlag = SEND_ALL_PARAM;

            Configuration::getInstance()->setConigValues("recipezero","recipe_number",QString::number(dwRecipeNumber));
            Configuration::getInstance()->setConigValues("recipezero","recipe_version_number",QString::number(dwRecipeVerNumber));
        }
        m_WeldRecipeParam.storeRecipeToDB();
        setRecipeModifiedState(true);
        m_WeldRecipeParam.sendRecipeToSC();
        loadAllModels();
    }
    if(qstrParamName.toLower() == "expected part contact" || qstrParamName.toLower() == "ready offset")
    {
        loadActatorSetupParamsModel();
    }
}

/**
 * @brief Recipe::getRecipeName : Return the recipe name
 * @return
 */
QString Recipe::getRecipeName()
{
    QString qstrReturnName="NewRecipe";
    bool bIsNewRecipe = m_WeldRecipeParam.getStructureValueByIndex(UI_ISNEW_RECIPE).toInt();
    if(!bIsNewRecipe)
        qstrReturnName = m_WeldRecipeParam.getStructureValueByIndex(UI_RECIPE_NAME);
    return qstrReturnName;
}

/**
 * @brief Recipe::getRecipeNumber : Return the recipe number
 * @return
 */
QString Recipe::getRecipeNumber()
{
    QString qstrReturnNumber="0";
    QString qstrRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER);
    if(qstrRecipeNumber == "0")
        qstrReturnNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER);
    else
        qstrReturnNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER);
    StackRecipeData::RecipeNumber=qstrReturnNumber.toInt();
    return qstrReturnNumber;
}

/**
 * @brief Recipe::createNewRecipe : Click on create a new recipe
 */
void Recipe::createNewRecipe()
{
    m_WeldRecipeParam.bIsWeldForceChanged = false;
    m_EditRecipeClicked = true;
    m_WeldRecipeParam.initializeRecipeParamaterStructure(0);
    int dwNewRecipeNumber = getNewRecipeNumber();
    m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_NUMBER,dwNewRecipeNumber);
    m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER, 0);
    m_WeldRecipeParam.setStructureVariableByIndex(UI_RECIPE_NAME,"NewRecipe");
    m_WeldRecipeParam.setStructureVariableByIndex(UI_ISNEW_RECIPE,"1");
    setParamaterChangedFlag(0);
    m_WeldRecipeParam.storeRecipeToDB(true);
    m_WeldRecipeParam.setIsActiveRequired(false);
    setSelectedRecipeToActive(QString::number(dwNewRecipeNumber));
    loadAllModels();
}

/**
 * @brief Recipe::editExistingRecipe
 * @param qstrRecipeNumber
 */
void Recipe::editExistingRecipe(QString qstrRecipeNumber)
{
    m_WeldRecipeParam.bIsWeldForceChanged = false;
    setRecipeModifiedState(false);
    int dwRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
    int dwVersionNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER).toInt();
    if(dwRecipeNumber == 0 && dwVersionNumber == qstrRecipeNumber.toInt())
    {
        setRecipeModifiedState(true);
        loadAllModels();
        return;
    }

    m_WeldRecipeParam.setStructureVariableByIndex(UI_RECIPE_VERSION_NUMBER,m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER));

    m_WeldRecipeParam.readRecipeFromDB(qstrRecipeNumber);
    StackRecipeData::RecipeNumber=qstrRecipeNumber.toInt();
    int dwVerifyStatus = m_WeldRecipeParam.getStructureValueByIndex(UI_ISVALID).toInt();
    if(login->getUserLevel()== "Operator")
    {
        if(((userPrivilageObj->isPermissionAllowed(4, Features::SET_AS_ACTIVE) == true) && (dwVerifyStatus == 1)) ||
                (userPrivilageObj->isPermissionAllowed(4, Features::RUN_UNVALIDATED_WELD_RECIPE) == true))
        {
            setSelectedRecipeToActive(qstrRecipeNumber);
        }
    }
    else if(!m_WeldRecipeParam.getIsActiveRequired())
        setSelectedRecipeToActive(qstrRecipeNumber);
    loadAllModels();
}

/**
 * @brief Recipe::copySelectedRecipe : Copy the selected recipe
 * @param qstrSourceRecipeNumber
 * @param qstrNewRecipeNumber
 * @param isNewRecipe
 */
void Recipe::copySelectedRecipe(QString qstrSourceRecipeNumber,QString qstrNewRecipeNumber,bool isNewRecipe)
{
    m_WeldRecipeParam.readRecipeFromDB(qstrSourceRecipeNumber);
    m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_NUMBER,qstrNewRecipeNumber.toInt());
    m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER,0);
    m_WeldRecipeParam.setStructureVariableByIndex(UI_ISACTIVE,0);
    m_WeldRecipeParam.setStructureVariableByIndex(UI_ISVALID,0);
    if(isNewRecipe == true)
        m_WeldRecipeParam.setStructureVariableByIndex(UI_ISNEW_RECIPE,"1");
    setParamaterChangedFlag(0);
    m_WeldRecipeParam.storeRecipeToDB(isNewRecipe);
    setSelectedRecipeToActive(qstrNewRecipeNumber);
    loadAllModels();
}

/**
 * @brief Recipe::exisistingRecipeEdited
 * @param qstrSourceRecipeNumber
 * @param qstrNewRecipeNumber
 */
void Recipe::exisistingRecipeEdited(QString qstrSourceRecipeNumber,QString qstrNewRecipeNumber)
{
    m_WeldRecipeParam.m_oldRecipeNumber = qstrSourceRecipeNumber.toInt();
    if(qstrSourceRecipeNumber.toInt() != qstrNewRecipeNumber.toInt())
        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_NUMBER,qstrNewRecipeNumber.toInt());
    m_WeldRecipeParam.setStructureVariableByIndex(UI_ISNEW_RECIPE,"1");
    m_WeldRecipeParam.storeRecipeToDB();
    m_WeldRecipeParam.m_oldRecipeNumber = 0;
}

/**
 * @brief Recipe::storeRecipeToDB : Store the created recipe to database
 */
QString Recipe::storeRecipeToDB()
{
    QString retString="";
    if((m_WeldRecipeParam.getStructureValueByIndex(UI_ISVALID) == "0"))
    {
        updateStatusText(RECIPE_SAVED);
    }
    else
        updateStatusText(RECIPE_SAVED_INVALIDATED);

    int dwRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
    int dwVersionNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER).toInt();
    int dwUIVersionNumber = m_WeldRecipeParam.getStructureValueByIndex(UI_RECIPE_VERSION_NUMBER).toInt();

    if(dwRecipeNumber == 0)
    {
        setParamaterChangedFlag(0);
        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER, 0);
        m_WeldRecipeParam.storeRecipeToDB();

        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_NUMBER, dwVersionNumber);
        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER, dwUIVersionNumber);
        m_WeldRecipeParam.setStructureVariableByIndex(UI_RECIPE_VERSION_NUMBER, 0);
    }

//    int dwIsNewRecipe = m_WeldRecipeParam.getStructureValueByIndex(UI_ISNEW_RECIPE).toInt();
//    if(dwIsNewRecipe)
//    {
//        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER, 0);
//    }

    dwRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
    dwVersionNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER).toInt();
    if(dwRecipeNumber != 0)
    {
        dwVersionNumber++;
        m_WeldRecipeParam.setStructureVariableByIndex(SC_RECIPE_VER_NUMBER, dwVersionNumber);
    }

    StackRecipeData::RecipeNumber=dwRecipeNumber;

    setParamaterChangedFlag(0);
    m_WeldRecipeParam.storeRecipeToDB();
    setSelectedRecipeToActive(QString::number(dwRecipeNumber));
    setRecipeModifiedState(false);
    QString qStrVerNum = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER);
    retString.append("RecipeNumber:"+m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER)+"\n");
    retString.append("VersionNumber:"+qStrVerNum+"\n");
    retString.append("UserName:"+PrimaryData->getUsername()+"\n\n");
    return retString;
}

/**
 * @brief Recipe::getAutoGraphRefreshState
 * @return
 */
bool Recipe::getAutoGraphRefreshState()
{
    return isAutoGraphRefreshState;
}

/**
 * @brief Recipe::setAutoGraphRefreshState
 * @param state
 */
void Recipe::setAutoGraphRefreshState(bool state)
{
    if(isAutoGraphRefreshState != state)
    {
        isAutoGraphRefreshState = state;
        emit AutoGraphRefreshStateChanged();
    }
}

/**
 * @brief Recipe::getIsManualRefreshState
 * @return
 */
bool Recipe::getIsManualRefreshState()
{
    return isManualRefreshState;
}

/**
 * @brief Recipe::setIsManualRefreshState
 * @param state
 */
void Recipe::setIsManualRefreshState(bool state)
{
    if(isManualRefreshState != state)
    {
        isManualRefreshState = state;
        emit IsManualRefreshStateChanged();
    }
}

/**
 * @brief Recipe::hornDownRestoreDefaultAdvParameters : Restore to default values in horn down
 */
void Recipe::hornDownRestoreDefaultAdvParameters()
{
    m_WeldRecipeParam.setStructureVariableByName("down acceleration",getDefaultValue("down acceleration"));
    m_WeldRecipeParam.setStructureVariableByName("down max velocity",getDefaultValue("down max velocity"));
    m_WeldRecipeParam.setStructureVariableByName("down deceleration",getDefaultValue("down deceleration"));
    m_WeldRecipeParam.setStructureVariableByName("return acceleration",getDefaultValue("return acceleration"));
    m_WeldRecipeParam.setStructureVariableByName("return max velocity",getDefaultValue("return max velocity"));
    m_WeldRecipeParam.setStructureVariableByName("return deceleration",getDefaultValue("return deceleration"));
    loadActatorSetupAdvancedParamsModel();
}

/**
 * @brief Recipe::setActuatorRecipeValue
 * @param qstrValue
 * @param independentRecipeSendToBL
 */
void Recipe::setActuatorRecipeValue(QString qstrValue, bool independentRecipeSendToBL)
{
    m_WeldRecipeParam.bIsWeldForceChanged = independentRecipeSendToBL;
    if(independentRecipeSendToBL == true)
    {
        /*logic to store weld force value with one decimal in integer form*/
        if(dwCurrentUnit)
        {
            double dval = Utility::convertlbstoN(qstrValue.toDouble());
            qstrValue = QString::number(dval*10);
        }
        else
        {
            qstrValue = QString::number((qstrValue.toDouble()) * 10);
        }
        m_WeldRecipeParam.forceSettingIndependent(qstrValue.toDouble());
    }
    else
    {
        m_WeldRecipeParam.sendRecipeToSC();
    }
    loadActatorSetupParamsModel();
}

/**
 * @brief Recipe::updateAcutatorForceSetting
 */
void Recipe::updateAcutatorForceSetting()
{
    m_WeldRecipeParam.copyRecipeStructToForceStruct();
}

/**
 * @brief Recipe::getActuatorRecipeForceStructureValue
 * @return
 */
QString Recipe::getActuatorRecipeForceStructureValue()
{
    return m_WeldRecipeParam.getActuatorRecipeStructureValue();
}


/** Limits for all pararmeters */

/**
 * @brief Recipe::midPointValidation : Checking the paramter validation
 * @param a_paramName
 * @param a_low
 * @param a_high
 * @return
 */
bool Recipe::midPointValidation(QString a_paramName,double a_low, double a_high)
{
    int mode;
    mode = m_WeldRecipeParam.getStructureValueByIndex(SC_WELD_MODE).toInt();
    ModeInfo stModeInfo = m_WeldRecipeParam.getModeInfoByIndex(mode,"");
    double dValue = -1;
    if(!a_paramName.compare(WELD_PARAM_END_WELD_FORCE,Qt::CaseInsensitive))
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("weld force").toDouble();
    }
    else if(!a_paramName.compare(FIXED_PARAM_WELD_FORCE,Qt::CaseInsensitive))
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("weld force").toDouble();
    }
    else if(!a_paramName.compare(FIXED_PARAM_WELD_AMP,Qt::CaseInsensitive))
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("weld amplitude").toDouble();
    }
    else if(!a_paramName.compare(FIXED_PARAM_TRI_FORCE,Qt::CaseInsensitive))
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("trigger force").toDouble();
    }
    else if(!a_paramName.compare(FIXED_PARAM_HOLD_FORCE,Qt::CaseInsensitive))
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("hold force").toDouble();
    }
    else if(!a_paramName.compare(FIXED_PARAM_HOLD_TIME,Qt::CaseInsensitive))
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("hold time").toDouble();
    }
    else if(!a_paramName.compare(WELD_PARAM_TRI_DIS,Qt::CaseInsensitive))
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("trigger distance").toDouble();
    }
    else if(!a_paramName.compare(WELD_PARAM_FREQ,Qt::CaseInsensitive))
    {
        dValue = m_resonantFrequency;
    }
    else if (a_paramName == stModeInfo.ModeName)
    {
        dValue = m_WeldRecipeParam.getStructureValueByIndex(SC_MODE_VALUE).toDouble();;
        if (a_paramName == "Energy")
        {
            dValue = Utility::convertMilliJoulesToJoules(dValue);
        }
        else if(a_paramName == "Peak Power")
        {
        }
        else
        {
            dValue = m_WeldRecipeParam.convertMicrometreToMillimetre(dValue);
        }

    }

    if((a_low < 0 || a_low <= dValue) && (a_high < 0 || dValue <= a_high))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Recipe::controlLimitValidation
 * @param numpadTitle
 * @param keyInputText
 * @return
 */
bool Recipe::controlLimitValidation(QString numpadTitle,QString keyInputText)
{
    bool bIsSEnabled = false;
    bool bIsSLowEnabled = false;
    bool bIsSHighEnabled = false;
    bool bIsREnabled = false;
    bool bIsRLowEnabled = false;
    bool bIsRHighEnabled = false;
    double dwSL;
    double dwSH;
    double dwRL;
    double dwRH;
    double dwCLim = keyInputText.toDouble();
    double dwIndVal;

    if(numpadTitle.toLower() == "peak power cutoff")
    {
        bIsSEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_ENABLED).toInt();
        bIsSLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_LOW_ENABLED).toInt();
        bIsSHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_HIGH_ENABLED).toInt();
        bIsREnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_ENABLED).toInt();
        bIsRLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_LOW_ENABLED).toInt();
        bIsRHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_HIGH_ENABLED).toInt();

        if((bIsSEnabled == true) && (bIsSLowEnabled == true) && (bIsSHighEnabled == true) &&
                (bIsREnabled == true) && (bIsRLowEnabled == true) && (bIsRHighEnabled == true))
        {
            dwSL = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_LOW).toDouble();
            dwSH = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_HIGH).toDouble();
            dwRL = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_LOW).toDouble();
            dwRH = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_HIGH).toDouble();

            if((dwCLim > dwRL) && (dwCLim > dwSL) && (dwCLim < dwRH) && (dwCLim < dwSH))
            {
                return true;
            }
        }
    }
    else if(numpadTitle.toLower() == "absolute cutoff")
    {
        bIsSEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_ENABLED).toInt();
        bIsSLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_LOW_ENABLED).toInt();
        bIsSHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_HIGH_ENABLED).toInt();
        bIsREnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_ENABLED).toInt();
        bIsRLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_LOW_ENABLED).toInt();
        bIsRHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_HIGH_ENABLED).toInt();

        if((bIsSEnabled == true) && (bIsSLowEnabled == true) && (bIsSHighEnabled == true) &&
                (bIsREnabled == true) && (bIsRLowEnabled == true) && (bIsRHighEnabled == true))
        {
            dwSL = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_LOW).toDouble();
            dwSH = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_HIGH).toDouble();
            dwRL = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_LOW).toDouble();
            dwRH = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_HIGH).toDouble();
            dwIndVal = m_WeldRecipeParam.getStructureValueByIndex(SC_PRETRIGGER_DISTANCE).toDouble();
            //dwIndVal1 = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_HIGH).toDouble();

            if((dwCLim > dwRL) && (dwCLim > dwSL) && (dwCLim < dwRH) && (dwCLim < dwSH) && (dwCLim > dwIndVal))
            {
                return true;
            }
        }
    }
    else if(numpadTitle.toLower() == "collapse cutoff")
    {
        bIsSEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_ENABLED).toInt();
        bIsSLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_LOW_ENABLED).toInt();
        bIsSHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_HIGH_ENABLED).toInt();
        bIsREnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_ENABLED).toInt();
        bIsRLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_LOW_ENABLED).toInt();
        bIsRHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_HIGH_ENABLED).toInt();

        if((bIsSEnabled == true) && (bIsSLowEnabled == true) && (bIsSHighEnabled == true) &&
                (bIsREnabled == true) && (bIsRLowEnabled == true) && (bIsRHighEnabled == true))
        {
            dwSL = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_LOW).toDouble();
            dwSH = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_HIGH).toDouble();
            dwRL = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_LOW).toDouble();
            dwRH = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_HIGH).toDouble();

            if((dwCLim > dwRL) && (dwCLim > dwSL) && (dwCLim < dwRH) && (dwCLim < dwSH))
            {
                return true;
            }
        }
    }
    else if(numpadTitle.toLower() == "energy cutoff")
    {
        bIsSEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_ENABLED).toInt();
        bIsSLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_LOW_ENABLED).toInt();
        bIsSHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_HIGH_ENABLED).toInt();
        bIsREnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_ENABLED).toInt();
        bIsRLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_LOW_ENABLED).toInt();
        bIsRHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_HIGH_ENABLED).toInt();

        if((bIsSEnabled == true) && (bIsSLowEnabled == true) && (bIsSHighEnabled == true) &&
                (bIsREnabled == true) && (bIsRLowEnabled == true) && (bIsRHighEnabled == true))
        {
            dwSL = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_LOW).toDouble();
            dwSH = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_HIGH).toDouble();
            dwRL = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_LOW).toDouble();
            dwRH = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_HIGH).toDouble();

            if((dwCLim > dwRL) && (dwCLim > dwSL) && (dwCLim < dwRH) && (dwCLim < dwSH))
            {
                return true;
            }
        }
    }
    else if(numpadTitle.toLower() == "time cutoff")
    {
        bIsSEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_ENABLED).toInt();
        bIsSLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_LOW_ENABLED).toInt();
        bIsSHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_HIGH_ENABLED).toInt();
        bIsREnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_ENABLED).toInt();
        bIsRLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_LOW_ENABLED).toInt();
        bIsRHighEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_HIGH_ENABLED).toInt();

        if((bIsSEnabled == true) && (bIsSLowEnabled == true) && (bIsSHighEnabled == true) &&
                (bIsREnabled == true) && (bIsRLowEnabled == true) && (bIsRHighEnabled == true))
        {
            dwSL = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_LOW).toDouble();
            dwSH = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_HIGH).toDouble();
            dwRL = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_LOW).toDouble();
            dwRH = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_HIGH).toDouble();
            dwIndVal = m_WeldRecipeParam.getStructureValueByIndex(SC_MAX_WELD_TIMEOUT).toDouble();;

            if((dwCLim > dwRL) && (dwCLim > dwSL) && (dwCLim < dwRH) && (dwCLim < dwSH) && (dwCLim < dwIndVal) && (dwIndVal != 0))
            {
                return true;
            }
        }
    }
    else if(numpadTitle.toLower() == "frequency low cutoff")
    {
        bIsSEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_ENABLED).toInt();
        bIsSLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_LOW_ENABLED).toInt();
        bIsREnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_ENABLED).toInt();
        bIsRLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_LOW_ENABLED).toInt();

        if((bIsSEnabled == true) && (bIsSLowEnabled == true) &&
                (bIsREnabled == true) && (bIsRLowEnabled == true))
        {
            dwSL = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_LOW).toDouble();
            dwRL = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_LOW).toDouble();

            if((dwCLim > dwRL) && (dwCLim > dwSL))
            {
                return true;
            }
        }
    }
    else if(numpadTitle.toLower() == "frequency high cutoff")
    {
        bIsSEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_ENABLED).toInt();
        bIsSLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_HIGH_ENABLED).toInt();
        bIsREnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_ENABLED).toInt();
        bIsRLowEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_HIGH_ENABLED).toInt();

        if((bIsSEnabled == true) && (bIsSLowEnabled == true) &&
                (bIsREnabled == true) && (bIsRLowEnabled == true))
        {
            dwSL = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_HIGH).toDouble();
            dwRL = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_HIGH).toDouble();

            if((dwCLim > dwRL) && (dwCLim > dwSL))
            {
                return true;
            }
        }
    }
    else
        return false;
    return false;
}

/**
 * @brief Recipe::setupLimitValidation
 * @param keypadTextname
 * @param keypadValue
 * @return
 */
QString Recipe::setupLimitValidation(QString keypadTextname,QString keypadValue)
{
    bool bGlobalSetup;
    bool bIndividualSetup;
    keypadTextname = keypadTextname.toLower();
    bGlobalSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_GLOBAL_SETUP).toInt();
    if((keypadTextname == "time") || (keypadTextname == "peak power") ||
            (keypadTextname == "energy") || (keypadTextname == "absloute distance") ||
            (keypadTextname == "ground detect") || (keypadTextname == "collapse distance") ||
            (keypadTextname == "dynamic"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_SETUP_WELD_MODE_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation("weld mode",keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else if((keypadTextname == "weld amplitude"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_WELD_AMP_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation(keypadTextname,keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else  if((keypadTextname == "trigger force"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_TRIGGER_FORCE_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation(keypadTextname,keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else  if((keypadTextname == "weld force"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_WELD_FORCE_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation(keypadTextname,keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else  if((keypadTextname == "hold force"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_HOLD_FORCE_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation(keypadTextname,keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else  if((keypadTextname == "hold time"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_HOLD_TIME_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation(keypadTextname,keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else  if((keypadTextname == "reactivity"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_REACTIVITY_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation(keypadTextname,keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else  if((keypadTextname == "force level"))
    {
        bIndividualSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_FORCE_LEVEL_STATUS).toInt();
        if(bGlobalSetup && bIndividualSetup)
        {
            return m_WeldRecipeParam.rangeSetupLimitValidation(keypadTextname,keypadValue);
        }
        else
            return keypadlimitation(keypadTextname,keypadValue);
    }
    else
        return keypadlimitation(keypadTextname,keypadValue);
}

/**
 * @brief Recipe::keypadlimitation : Set the keypad limition setting
 * @param keypadtextname
 * @param keypadvalue
 * @return
 */
QString Recipe::keypadlimitation(QString keypadtextname,QString keypadvalue)
{
    return m_WeldRecipeParam.rangeValidation(keypadtextname,keypadvalue);
}

/**
 * @brief Recipe::checkControlLimits : Check control limits validation process
 * @param bState
 * @param qstrParamName
 * @param qstrParamValue
 * @return
 */
bool Recipe::checkControlLimits(bool bState,QString qstrName,QString qstrParamValue)
{
    if(!bState)
        return true;

    QString qstrParamName = qstrName.remove(" Cutoff");
    double dParamValue = qstrParamValue.toDouble();
    int dwRet = OFF;

    if(qstrParamName.contains("Frequency",Qt::CaseInsensitive))
        dwRet = m_WeldRecipeParam.CheckSuspectRejectRange("frequency",dParamValue);
    else
        dwRet = m_WeldRecipeParam.CheckSuspectRejectRange(qstrParamName,dParamValue);

    if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
        return false;
    if(dwRet == OFF)
    {
        QString qstrTemp = m_WeldRecipeParam.rangeValidation(qstrParamName,qstrParamValue);
        if(qstrTemp.toDouble() != dParamValue)
            return false;
    }
    if(!qstrParamName.compare("Absolute Distance Cutoff",Qt::CaseInsensitive))
    {
        double dPriTriggerDistance = m_WeldRecipeParam.getStructureValueByName("Pretrigger Distance").toDouble();
        double dMissingPartDistance = m_WeldRecipeParam.getStructureValueByName("expected part contact").toDouble();
        if(dPriTriggerDistance > dParamValue)
            return false;
        if(!dMissingPartDistance || dMissingPartDistance > dParamValue)
            return false;
    }
    return true;
}

/**
 * @brief Recipe::checkSetupLimits : Check setup limits validation process
 * @param setupLow
 * @param setupHigh
 * @param setupStatus
 * @param paramName
 * @return
 */
int Recipe::checkSetupLimits(double setupLow,double setupHigh,bool setupStatus,QString paramName)
{
    bool isValidRecipe = m_WeldRecipeParam.getStructureValueByIndex(UI_ISVALID).toInt();
    if(!setupStatus)
        return SUCCESS;

    if(setupLow >= setupHigh)
        return INVALID_ENTRY;

    QString qstrTemp = m_WeldRecipeParam.rangeValidation(paramName,QString::number(setupLow));
    if(qstrTemp.toDouble() != setupLow)
        return INVALID_ENTRY;

    qstrTemp = m_WeldRecipeParam.rangeValidation(paramName,QString::number(setupHigh));
    if(qstrTemp.toDouble() != setupHigh)
        return INVALID_ENTRY;

    if(!isValidRecipe)
        return SUCCESS;

    double dVal = m_WeldRecipeParam.getStructureValueByName(paramName).toDouble();
    bool bIsGlobalSetup = m_WeldRecipeParam.getStructureValueByIndex(UI_GLOBAL_SETUP).toInt();
    if(bIsGlobalSetup && ((dVal < setupLow) || (dVal > setupHigh)))
        return PARAM_OUTOF_RANGE;

    int dwRet = OFF;
    QStringList qlstNonSRParams;
    qlstNonSRParams.append("Weld Amplitude");
    qlstNonSRParams.append("Trigger Force");
    qlstNonSRParams.append("Hold Force");
    qlstNonSRParams.append("Hold Time");

    if(!qlstNonSRParams.contains(paramName,Qt::CaseInsensitive))
    {
        dwRet = m_WeldRecipeParam.CheckSuspectRejectRange(paramName,setupLow);
        if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
            return SR_OUTOF_RANGE;
        else if(dwRet == ON_SUCCESS)
        {
            dwRet = m_WeldRecipeParam.CheckSuspectRejectRange(paramName,setupHigh);
            if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
                return SR_OUTOF_RANGE;
        }
    }
    return SUCCESS;
}

int Recipe::checkSuspectRejectValues(bool isGlobalSuspectEnabled,bool isGlobalRejectEnabled,bool isSuspectEnabled, QString qstrSuspectLow, QString qstrSuspectHigh,
                                     bool isRejectEnabled, QString qstrRejectLow, QString qstrRejectHigh,
                                     QString qstrParamName)
{
    int bRetValue = INVALID_ENTRY;
    double dLowValue=-1,dHighValue=-1,dValue=0;
    double dSuspectLow = qstrSuspectLow.toDouble();
    double dSuspectHigh = qstrSuspectHigh.toDouble();
    double dRejectLow = qstrRejectLow.toDouble();
    double dRejectHigh = qstrRejectHigh.toDouble();
    double dPreTriggerDistance;
    QList<double> qlstLimitValues;

    QString qstrTemp,paramName;
    paramName = qstrParamName;
    if(qstrParamName == WELD_PARAM_END_WELD_FORCE)
        paramName = "weld force";
    else if (qstrParamName == WELD_PARAM_TRI_DIS)
        paramName = "trigger distance";
    else if (qstrParamName == WELD_PARAM_FREQ)
        paramName = "frequency low";

    if(isRejectEnabled && !qstrRejectLow.isEmpty())
    {
        qstrTemp = m_WeldRecipeParam.rangeValidation(paramName,QString::number(dRejectLow));
        if(qstrTemp.toDouble() != dRejectLow)
            return PARAM_INVALID;
    }
    if(isSuspectEnabled && !qstrSuspectLow.isEmpty())
    {
        qstrTemp = m_WeldRecipeParam.rangeValidation(paramName,QString::number(dSuspectLow));
        if(qstrTemp.toDouble() != dSuspectLow)
            return PARAM_INVALID;
    }
    if(isSuspectEnabled && !qstrSuspectHigh.isEmpty())
    {
        qstrTemp = m_WeldRecipeParam.rangeValidation(paramName,QString::number(dSuspectHigh));
        if(qstrTemp.toDouble() != dSuspectHigh)
            return PARAM_INVALID;
    }
    if(isRejectEnabled && !qstrRejectHigh.isEmpty())
    {
        qstrTemp = m_WeldRecipeParam.rangeValidation(paramName,QString::number(dRejectHigh));
        if(qstrTemp.toDouble() != dRejectHigh)
            return PARAM_INVALID;
    }
    qDebug() << "paramName2 : " << paramName;
    if(isGlobalSuspectEnabled && isSuspectEnabled && !qstrSuspectLow.isEmpty())
        dLowValue = dSuspectLow;
    else if(isGlobalRejectEnabled && isRejectEnabled && !qstrRejectLow.isEmpty())
        dLowValue = dRejectLow;

    if(isGlobalSuspectEnabled && isSuspectEnabled && !qstrSuspectHigh.isEmpty())
        dHighValue = dSuspectHigh;
    else if(isGlobalRejectEnabled && isRejectEnabled && !qstrRejectHigh.isEmpty())
        dHighValue = dRejectHigh;

    if(isRejectEnabled && !qstrRejectLow.isEmpty())
        qlstLimitValues.append(dRejectLow);
    if(isSuspectEnabled && !qstrSuspectLow.isEmpty())
        qlstLimitValues.append(dSuspectLow);
    if(isSuspectEnabled && !qstrSuspectHigh.isEmpty())
        qlstLimitValues.append(dSuspectHigh);
    if(isRejectEnabled && !qstrRejectHigh.isEmpty())
        qlstLimitValues.append(dRejectHigh);

    int dwCnt=qlstLimitValues.size();
    for(int i=0;dwCnt>1 && i<dwCnt-1 ;i++)
    {
        if(qlstLimitValues.at(i)>=qlstLimitValues.at(i+1))
            return PARAM_OUTOF_RANGE;
    }

    if(isGlobalSuspectEnabled == false && isGlobalRejectEnabled == false)
        return SUCCESS;

    //Midpoint Validation
    if(qstrParamName == WELD_PARAM_END_WELD_FORCE)
    {
        bRetValue = validateSRWithSetupLimits("weld force",dLowValue,dHighValue);
        if(!bRetValue)
        {
            m_WeldRecipeParam.setStructureVariableByIndex(UI_WELD_FORCE_STATUS,0);
            bRetValue = SR_OUTOF_RANGE;
        }

        dValue = m_WeldRecipeParam.getStructureValueByName("weld force").toDouble();
    }
    else if (qstrParamName == WELD_PARAM_TRI_DIS)
    {
        dValue = m_WeldRecipeParam.getStructureValueByName("trigger distance").toDouble();
    }
    else if (qstrParamName == WELD_PARAM_FREQ)
    {
        bRetValue = validateSRWithControlLimits("frequency low",dLowValue,dHighValue);
        if(!bRetValue)
        {
            m_WeldRecipeParam.setStructureVariableByIndex(SC_CL_FREQ_LOW,0);
            bRetValue = SR_OUTOF_RANGE;
        }
        else
        {
            bRetValue = validateSRWithControlLimits("frequency high",dLowValue,dHighValue);
            if(!bRetValue)
            {
                m_WeldRecipeParam.setStructureVariableByIndex(SC_CL_FREQ_HIGH,0);
                bRetValue = SR_OUTOF_RANGE;
            }
        }

        dValue = m_resonantFrequency;
    }
    else if (qstrParamName.compare("Time",Qt::CaseInsensitive) ||
             qstrParamName.compare("Energy",Qt::CaseInsensitive) ||
             qstrParamName.compare("Peak Power",Qt::CaseInsensitive) ||
             qstrParamName.compare("Absolute Distance",Qt::CaseInsensitive) ||
             qstrParamName.compare("Collapse Distance",Qt::CaseInsensitive))
    {
        bRetValue = validateSRWithSetupLimits("weld mode",dLowValue,dHighValue);
        if(!bRetValue)
        {
            m_WeldRecipeParam.setStructureVariableByIndex(UI_SETUP_WELD_MODE_STATUS,0);
            bRetValue = SR_OUTOF_RANGE;
        }
        else
        {
            bRetValue = validateSRWithControlLimits(qstrParamName,dLowValue,dHighValue);
            if(!bRetValue)
            {
                m_WeldRecipeParam.setStructureVariableByName(qstrParamName + " cutoff state","0");
                bRetValue = SR_OUTOF_RANGE;
            }
        }
        dValue = m_WeldRecipeParam.getStructureValueByName(qstrParamName).toDouble();
    }
    else
    {
        bRetValue = SUCCESS;
    }

    dPreTriggerDistance = m_WeldRecipeParam.getStructureValueByName("pretrigger distance").toDouble();

    if (qstrParamName == WELD_PARAM_TRI_DIS)
    {
        if((dLowValue < 0 || dPreTriggerDistance > dLowValue) && (dHighValue < 0 || dPreTriggerDistance < dHighValue))
            bRetValue = SUCCESS;
    }
    else if (qstrParamName == WELD_PARAM_END_WELD_FORCE || qstrParamName == WELD_PARAM_FREQ)
    {
        if((dLowValue < 0 || dValue > dLowValue) && (dHighValue < 0 || dValue < dHighValue))
            bRetValue = SUCCESS;
    }
    else
    {
        int dwWeldMode = m_WeldRecipeParam.getStructureValueByIndex(SC_WELD_MODE).toInt();
        ModeInfo stTemp = m_WeldRecipeParam.getModeInfoByIndex(dwWeldMode,"");
        if(stTemp.ModeName.compare(qstrParamName,Qt::CaseInsensitive) != 0)
        {
            bRetValue = SUCCESS;
        }
        else if(qstrParamName == WELD_MODE_ABS_DIS)
        {
            if((dLowValue < 0 || dValue > dLowValue) && (dHighValue < 0 || dValue < dHighValue) && (dHighValue < 0 || dHighValue > dPreTriggerDistance))
                bRetValue = SUCCESS;
        }
        else
        {
            if((dLowValue < 0 || dValue > dLowValue) && (dHighValue < 0 || dValue < dHighValue))
            {
                if(bRetValue == SR_OUTOF_RANGE)
                    bRetValue = PARAM_MID_POINT;
                else
                    bRetValue = SUCCESS;
            }
        }
    }
    return bRetValue;
}

/**
 * @brief Recipe::validateSuspectRejectValues
 * @param isSuspectEnabled
 * @param qstrSuspectLow
 * @param qstrSuspectHigh
 * @param isRejectEnabled
 * @param qstrRejectLow
 * @param qstrRejectHigh
 * @param qstrParamName
 * @return
 */
int Recipe::validateSuspectRejectValues(bool isSuspectEnabled, QString qstrSuspectLow, QString qstrSuspectHigh,
                                        bool isRejectEnabled, QString qstrRejectLow, QString qstrRejectHigh,
                                        QString qstrParamName)
{
    int bRetValue = 1;
    bool isGlobalSuspectEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_GLOBAL_SUSPECT).toInt();
    bool isGlobalRejectEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_GLOBAL_REJECT).toInt();
    bRetValue = checkSuspectRejectValues(isGlobalSuspectEnabled,isGlobalRejectEnabled,isSuspectEnabled,
                                         qstrSuspectLow, qstrSuspectHigh,isRejectEnabled, qstrRejectLow,
                                         qstrRejectHigh,qstrParamName);
    return bRetValue;
}

/**
 * @brief Recipe::paramRangeValidation : Validating the paramter name and values
 * @param keypadTextname
 * @param keypadValue
 * @return
 */
QString Recipe::paramRangeValidation(QString keypadTextname,QString keypadValue)
{
    double dModifiedValue = keypadValue.toDouble();
    QString qstrOldValue = m_WeldRecipeParam.getStructureValueByName(keypadTextname);
    QString qstrParameterName = keypadTextname;
    QStringList qlstModeParamValues;
    qlstModeParamValues.append("Time");
    qlstModeParamValues.append("Energy");
    qlstModeParamValues.append("Peak Power");
    qlstModeParamValues.append("Ground Detect");
    qlstModeParamValues.append("Scrub Time");
    qlstModeParamValues.append("Absolute Distance");
    qlstModeParamValues.append("Collapse Distance");
    qlstModeParamValues.append("Dynamic");
    qlstModeParamValues.append("Density");

    QStringList qlstSetupParamValues;
    qlstSetupParamValues.append("Weld Amplitude");
    qlstSetupParamValues.append("Weld Force");
    qlstSetupParamValues.append("Trigger Force");
    qlstSetupParamValues.append("Hold Force");
    qlstSetupParamValues.append("Hold Time");
    qlstSetupParamValues.append("Reactivity");
    qlstSetupParamValues.append("Force Level");

    QStringList qlstSRParamValues;
    qlstSRParamValues.append("frequency low");
    qlstSRParamValues.append("frequency high");
    qlstSRParamValues.append("start frequency");
    qlstSRParamValues.append("Trigger Distance");

    int dwRet = OFF;
    if(qlstModeParamValues.contains(qstrParameterName,Qt::CaseInsensitive))
    {
        //        double dCompareVal;
        //        dCompareVal = m_WeldRecipeParam.getStructureValueByName(qstrParameterName).toDouble();
        //        if(dModifiedValue >= dCompareVal)
        //            return qstrOldValue;

        if(login->getUserLevel()== "Operator")
        {
        dwRet = m_WeldRecipeParam.CheckSetupRange("weld mode",keypadValue.toDouble());
        if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
            return qstrOldValue;
        }
        if(dwRet == OFF)
        {
            dwRet = m_WeldRecipeParam.CheckSuspectRejectRange(qstrParameterName,dModifiedValue);
            if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
                return qstrOldValue;
            else if(dwRet == OFF)
                return m_WeldRecipeParam.rangeValidation(keypadTextname,keypadValue);
        }
        return keypadValue;
    }
    else if(qlstSetupParamValues.contains(qstrParameterName,Qt::CaseInsensitive))
    {
          if(login->getUserLevel()== "Operator")
          {
        dwRet = m_WeldRecipeParam.CheckSetupRange(qstrParameterName,keypadValue.toDouble());
        if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
            return qstrOldValue;
          }

        if(dwRet == OFF && qstrParameterName.compare("Weld Force",Qt::CaseInsensitive))
        {
            dwRet = m_WeldRecipeParam.CheckSuspectRejectRange("End Weld Force",dModifiedValue);
            if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
                return qstrOldValue;
        }

        if(dwRet == OFF)
            return m_WeldRecipeParam.rangeValidation(keypadTextname,keypadValue);

        return keypadValue;
    }
    else if(qlstSRParamValues.contains(qstrParameterName,Qt::CaseInsensitive))
    {
        if(qstrParameterName.contains("frequency",Qt::CaseInsensitive))
            dwRet = m_WeldRecipeParam.CheckSuspectRejectRange("frequency",dModifiedValue);
        else
            dwRet = m_WeldRecipeParam.CheckSuspectRejectRange(qstrParameterName,dModifiedValue);
        if(dwRet == ON_FAIL_LIMIT || dwRet == ON_FAIL_PARAM)
            return qstrOldValue;

        return keypadValue;
    }
    else
    {
        return m_WeldRecipeParam.rangeValidation(keypadTextname,keypadValue);
    }
}

/**
 * @brief Recipe::midPointValidationForAllParams
 * @param isGlobalSuspectEnabled
 * @param isGlobalRejectEnabled
 * @param isGlobalSetupEnabled
 * @return
 */
int Recipe::midPointValidationForAllParams(bool isGlobalSuspectEnabled,bool isGlobalRejectEnabled,bool isGlobalSetupEnabled)
{
    int mode = m_WeldRecipeParam.getStructureValueByIndex(SC_WELD_MODE).toInt();
    bool isSEnabled = false;
    bool isREnabled = false;
    bool isSLowEnabled = false;
    bool isSHighEnabled = false;
    bool isRLowEnabled = false;
    bool isRHighEnabled = false;
    double qstrSetupLow, qstrSetupHigh;
    QString qstrSuspectLow = "", qstrSuspectHigh = "",qstrRejectLow = "",qstrRejectHigh = "";
    QString qstrModeName = WELD_MODE_TIME;
    bool isModeEnabled = true;
    int retVal = 0;
    ModeInfo stModeInfo;

    if(!isGlobalSuspectEnabled)
        isGlobalSuspectEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_GLOBAL_SUSPECT).toInt();
    if(!isGlobalRejectEnabled)
        isGlobalRejectEnabled = m_WeldRecipeParam.getStructureValueByIndex(SC_GLOBAL_REJECT).toInt();
    if(!isGlobalSetupEnabled)
        isGlobalSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_GLOBAL_SETUP).toInt();
    //Validating mode values
    stModeInfo = m_WeldRecipeParam.getModeInfoByIndex(mode,"");
    if (stModeInfo.ModeName == WELD_MODE_TIME )
    {
        qstrModeName    = WELD_MODE_TIME;
        isSEnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_ENABLED).toInt();
        isREnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_ENABLED).toInt();
        isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_LOW_ENABLED).toInt();
        isSHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TIME_HIGH_ENABLED).toInt();
        isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_LOW_ENABLED).toInt();
        isRHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TIME_HIGH_ENABLED).toInt();

        if(isSLowEnabled)
            qstrSuspectLow  = m_WeldRecipeParam.getStructureValueByName("suspect time low value");

        if(isSHighEnabled)
            qstrSuspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect time high value");

        if(isRLowEnabled)
            qstrRejectLow   = m_WeldRecipeParam.getStructureValueByName("reject time low value");

        if(isRHighEnabled)
            qstrRejectHigh  = m_WeldRecipeParam.getStructureValueByName("reject time high value");
    }
    else if (stModeInfo.ModeName == WELD_MODE_ENERGY )
    {
        qstrModeName    = WELD_MODE_ENERGY;
        isSEnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_ENABLED).toInt();
        isREnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_ENABLED).toInt();
        isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_LOW_ENABLED).toInt();
        isSHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_ENERGY_HIGH_ENABLED).toInt();
        isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_LOW_ENABLED).toInt();
        isRHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_ENERGY_HIGH_ENABLED).toInt();

        if(isSLowEnabled)
            qstrSuspectLow  = m_WeldRecipeParam.getStructureValueByName("suspect energy low value");

        if(isSHighEnabled)
            qstrSuspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect energy high value");

        if(isRLowEnabled)
            qstrRejectLow   = m_WeldRecipeParam.getStructureValueByName("reject energy low value");

        if(isRHighEnabled)
            qstrRejectHigh  = m_WeldRecipeParam.getStructureValueByName("reject energy high value");

    }
    else if (stModeInfo.ModeName == WELD_MODE_PEAK_POWER )
    {
        qstrModeName    = WELD_MODE_PEAK_POWER;
        isSEnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_ENABLED).toInt();
        isREnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_ENABLED).toInt();
        isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_LOW_ENABLED).toInt();
        isSHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_POWER_HIGH_ENABLED).toInt();
        isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_LOW_ENABLED).toInt();
        isRHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_POWER_HIGH_ENABLED).toInt();

        if(isSLowEnabled)
            qstrSuspectLow  = m_WeldRecipeParam.getStructureValueByName("suspect peak power low value");

        if(isSHighEnabled)
            qstrSuspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect peak power high value");

        if(isRLowEnabled)
            qstrRejectLow   = m_WeldRecipeParam.getStructureValueByName("reject peak power low value");

        if(isRHighEnabled)
            qstrRejectHigh  = m_WeldRecipeParam.getStructureValueByName("reject peak power high value");

    }
    else if (stModeInfo.ModeName == WELD_MODE_ABS_DIS )
    {
        qstrModeName    = WELD_MODE_ABS_DIS;
        isSEnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_ENABLED).toInt();
        isREnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_ENABLED).toInt();
        isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_LOW_ENABLED).toInt();
        isSHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_AD_HIGH_ENABLED).toInt();
        isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_LOW_ENABLED).toInt();
        isRHighEnabled  = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_AD_HIGH_ENABLED).toInt();

        if(isSLowEnabled)
            qstrSuspectLow  = m_WeldRecipeParam.getStructureValueByName("suspect absolute distance low value");

        if(isSHighEnabled)
            qstrSuspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect absolute distance high value");

        if(isRLowEnabled)
            qstrRejectLow   = m_WeldRecipeParam.getStructureValueByName("reject absolute distance low value");

        if(isRHighEnabled)
            qstrRejectHigh  = m_WeldRecipeParam.getStructureValueByName("reject absolute distance high value");
    }
    else if (stModeInfo.ModeName == WELD_MODE_COL_DIS )
    {
        qstrModeName     = WELD_MODE_COL_DIS;
        isSEnabled       = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_ENABLED).toInt();
        isREnabled       = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_ENABLED).toInt();
        isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_LOW_ENABLED).toInt();
        isSHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_CD_HIGH_ENABLED).toInt();
        isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_LOW_ENABLED).toInt();
        isRHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_CD_HIGH_ENABLED).toInt();

        if(isSLowEnabled)
            qstrSuspectLow   = m_WeldRecipeParam.getStructureValueByName("suspect collapse distance low value");

        if(isSHighEnabled)
            qstrSuspectHigh  = m_WeldRecipeParam.getStructureValueByName("suspect collapse distance high value");

        if(isRLowEnabled)
            qstrRejectLow    = m_WeldRecipeParam.getStructureValueByName("reject collapse distance low value");

        if(isRHighEnabled)
            qstrRejectHigh   = m_WeldRecipeParam.getStructureValueByName("reject collapse distance high value");
    }
    else
    {
        isModeEnabled = false;
    }

    if(isModeEnabled)
    {
        retVal = checkSuspectRejectValues(isGlobalSuspectEnabled,isGlobalRejectEnabled,isSEnabled, qstrSuspectLow, qstrSuspectHigh,
                                          isREnabled, qstrRejectLow, qstrRejectHigh,
                                          qstrModeName);
        if(retVal)
            return retVal;

        if(isGlobalSetupEnabled)
        {
            qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup weld mode low").toDouble();
            qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup weld mode high").toDouble();

            bool isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_SETUP_WELD_MODE_STATUS).toDouble();
            if(isSetupEnabled)
            {
                retVal = midPointValidation(stModeInfo.ModeName,qstrSetupLow,qstrSetupHigh);
                if(!retVal)
                    return PARAM_MID_POINT;
            }
        }
    }

    //Validating Trigger diatance
    isSEnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TD_ENABLED).toInt();
    isREnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TD_ENABLED).toInt();
    isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TD_LOW_ENABLED).toInt();
    isSHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_TD_HIGH_ENABLED).toInt();
    isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TD_LOW_ENABLED).toInt();
    isRHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_TD_HIGH_ENABLED).toInt();

    if(isSLowEnabled)
        qstrSuspectLow  = m_WeldRecipeParam.getStructureValueByName("suspect trigger distance low value");

    if(isSHighEnabled)
        qstrSuspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect trigger distance high value");

    if(isRLowEnabled)
        qstrRejectLow   = m_WeldRecipeParam.getStructureValueByName("reject trigger distance low value");

    if(isRHighEnabled)
        qstrRejectHigh  = m_WeldRecipeParam.getStructureValueByName("reject trigger distance high value");

    retVal = checkSuspectRejectValues(isGlobalSuspectEnabled,isGlobalRejectEnabled,isSEnabled, qstrSuspectLow, qstrSuspectHigh,
                                      isREnabled, qstrRejectLow, qstrRejectHigh,
                                      WELD_PARAM_TRI_DIS);
    if(retVal)
        return retVal;

    if(isGlobalSetupEnabled)
    {
        qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup trigger force low").toDouble();
        qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup trigger force high").toDouble();

        bool isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_TRIGGER_FORCE_STATUS).toDouble();
        if(isSetupEnabled)
        {
            retVal = midPointValidation("trigger force",qstrSetupLow,qstrSetupHigh);
            if(!retVal)
                return PARAM_MID_POINT;
        }
    }

    //Validating Force value
    isSEnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_END_FORCE_ENABLED).toInt();
    isREnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_END_FORCE_ENABLED).toInt();
    isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_END_FORCE_LOW_ENABLED).toInt();
    isSHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_END_FORCE_HIGH_ENABLED).toInt();
    isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_END_FORCE_LOW_ENABLED).toInt();
    isRHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_END_FORCE_HIGH_ENABLED).toInt();

    if(isSLowEnabled)
        qstrSuspectLow  = m_WeldRecipeParam.getStructureValueByName("suspect end weld force low value");

    if(isSHighEnabled)
        qstrSuspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect end weld force high value");

    if(isRLowEnabled)
        qstrRejectLow   = m_WeldRecipeParam.getStructureValueByName("reject end weld force low value");

    if(isRHighEnabled)
        qstrRejectHigh  = m_WeldRecipeParam.getStructureValueByName("reject end weld force high value");

    retVal = checkSuspectRejectValues(isGlobalSuspectEnabled,isGlobalRejectEnabled,isSEnabled, qstrSuspectLow, qstrSuspectHigh,
                                      isREnabled, qstrRejectLow, qstrRejectHigh,
                                      WELD_PARAM_END_WELD_FORCE);
    if(retVal)
        return retVal;

    if(isGlobalSetupEnabled)
    {
        qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup weld force low").toDouble();
        qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup weld force high").toDouble();

        bool isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_WELD_FORCE_STATUS).toInt();
        if(isSetupEnabled)
        {
            retVal = midPointValidation("weld force",qstrSetupLow,qstrSetupHigh);
            if(!retVal)
                return PARAM_MID_POINT;
        }
    }
    //Validating Frequency value
    isSEnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_ENABLED).toInt();
    isREnabled      = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_ENABLED).toInt();
    isSLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_LOW_ENABLED).toInt();
    isSHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_SUSPECT_FREQ_HIGH_ENABLED).toInt();
    isRLowEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_LOW_ENABLED).toInt();
    isRHighEnabled   = m_WeldRecipeParam.getStructureValueByIndex(SC_REJECT_FREQ_HIGH_ENABLED).toInt();

    if(isSLowEnabled)
        qstrSuspectLow  = m_WeldRecipeParam.getStructureValueByName("suspect frequency low value");

    if(isSHighEnabled)
        qstrSuspectHigh = m_WeldRecipeParam.getStructureValueByName("suspect frequency high value");

    if(isRLowEnabled)
        qstrRejectLow   = m_WeldRecipeParam.getStructureValueByName("reject frequency low value");

    if(isRHighEnabled)
        qstrRejectHigh  = m_WeldRecipeParam.getStructureValueByName("reject frequency high value");

    retVal = checkSuspectRejectValues(isGlobalSuspectEnabled,isGlobalRejectEnabled,isSEnabled, qstrSuspectLow, qstrSuspectHigh,
                                      isREnabled, qstrRejectLow, qstrRejectHigh,
                                      WELD_PARAM_FREQ);
    if(retVal)
        return retVal;

    if(isGlobalSetupEnabled)
    {
        bool isSetupEnabled;
        qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup weld amplitude low").toDouble();
        qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup weld amplitude high").toDouble();

        isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_WELD_AMP_STATUS).toInt();
        if(isSetupEnabled)
        {
            retVal = midPointValidation("weld amplitude",qstrSetupLow,qstrSetupHigh);
            if(!retVal)
                return PARAM_MID_POINT;
        }

        qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup hold force low").toDouble();
        qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup hold force high").toDouble();

        isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_HOLD_FORCE_STATUS).toInt();
        if(isSetupEnabled)
        {
            retVal = midPointValidation("hold force",qstrSetupLow,qstrSetupHigh);
            if(!retVal)
                return PARAM_MID_POINT;
        }

        qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup hold time low").toDouble();
        qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup hold time high").toDouble();

        isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_HOLD_TIME_STATUS).toInt();
        if(isSetupEnabled)
        {
            retVal = midPointValidation("hold time",qstrSetupLow,qstrSetupHigh);
            if(!retVal)
                return PARAM_MID_POINT;
        }

        qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup scrub amplitude low").toDouble();
        qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup scrub amplitude high").toDouble();

        isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_SCRUB_AMP_STATUS).toInt();
        if(isSetupEnabled)
        {
            retVal = midPointValidation("scrub amplitude",qstrSetupLow,qstrSetupHigh);
            if(!retVal)
                return PARAM_MID_POINT;
        }

        qstrSetupLow    = m_WeldRecipeParam.getStructureValueByName("setup scrub time low").toDouble();
        qstrSetupHigh   = m_WeldRecipeParam.getStructureValueByName("setup scrub time high").toDouble();

        isSetupEnabled = m_WeldRecipeParam.getStructureValueByIndex(UI_SCRUB_TIME_STATUS).toInt();
        if(isSetupEnabled)
        {
            retVal = midPointValidation("scrub time",qstrSetupLow,qstrSetupHigh);
            if(!retVal)
                return PARAM_MID_POINT;
        }
    }
    return SUCCESS;
}

/**
 * @brief Recipe::validateSRWithSetupLimits
 * @param qstrParamName
 * @param dSRLow
 * @param dSRHigh
 * @return
 */
bool Recipe::validateSRWithSetupLimits(QString qstrParamName,double dSRLow, double dSRHigh)
{
    double bLow,bHigh;
    bool bState;
    bool isValidRecipe = m_WeldRecipeParam.getStructureValueByIndex(UI_ISVALID).toInt();
    bLow = m_WeldRecipeParam.getStructureValueByName("setup " + qstrParamName + " low").toDouble();
    bHigh = m_WeldRecipeParam.getStructureValueByName("setup " + qstrParamName + " high").toDouble();
    bState = m_WeldRecipeParam.getStructureValueByName("setup " + qstrParamName + " status").toInt();

    if((!isValidRecipe || !bState) || (dSRLow < 0 && dSRHigh < 0) || (dSRLow < 0 && bHigh <= dSRHigh) || (dSRHigh < 0 && dSRLow <= bLow) || (dSRLow <= bLow && bHigh <= dSRHigh))
        return true;
    else
        return false;
}

/**
 * @brief Recipe::validateSRWithControlLimits
 * @param qstrParamName
 * @param dSRLow
 * @param dSRHigh
 * @return
 */
bool Recipe::validateSRWithControlLimits(QString qstrParamName,double dSRLow, double dSRHigh)
{
    double dValue;
    bool bState;
    dValue = m_WeldRecipeParam.getStructureValueByName(qstrParamName + " cutoff").toDouble();
    bState = m_WeldRecipeParam.getStructureValueByName(qstrParamName + " cutoff state").toInt();

    if((!bState) || (dSRLow < 0 && dSRHigh < 0) || (dSRLow < 0 && dSRHigh >= dValue) || (dSRHigh < 0 && dSRLow <= dValue) || (dSRLow <= dValue && dValue <= dSRHigh))
        return true;
    else
        return false;
}


/**
 * @brief Recipe::displayandStoreGraphData:sends disable or enable collect graph data command
 * @param 
 * @return
 */
void Recipe::displayandStoreGraphData()
{
   QString GraphState;
   bool GraphDataState=false;

   if(isAutoGraphRefreshState)
   {
      GraphDataState =true;
   }
   else
   {
      GraphDataState=false;
   }

   GraphState=QString::number(GraphDataState);
   CIObj.sendMessage(REQ_STOP_COLLECT_GRAPH_DATA,RES_STOP_COLLECT_GRAPH_DATA,GraphState);
}

/**
 * @brief send and recieve the delete recipe command.
 * @param recipeNumber is the recipe number to be deleted.
 * @return the response code
 */
int Recipe::deleteArecipeCard(int recipeNumber)
{
    QString qstrReceivedBuffer = "";
    QString strRecipeNumber = QString::number(recipeNumber);
    CIObj.sendMessage(REQ_DELETE_RECIPE,RES_DELETE_RECIPE,strRecipeNumber);
    bool bRecivedData =  CIObj.recivedMessage(RES_DELETE_RECIPE,qstrReceivedBuffer);
    if(bRecivedData == false)
    {
        return -1;
    }

    //QString to int
    bool bStrToInt;
    int recResult = qstrReceivedBuffer.toInt(&bStrToInt);
    if(bStrToInt == false)
    {
        recResult = DeleteRecipeResponse::END_FIAL_DELETE;
    }
    else
    {
        if(recResult < DeleteRecipeResponse::DELETE_RECIPE_SUCCESS || recResult > DeleteRecipeResponse::RECIPE_NOT_PRESENT)
        {
            recResult = DeleteRecipeResponse::END_FIAL_DELETE;
        }
    }


    return recResult;

}

