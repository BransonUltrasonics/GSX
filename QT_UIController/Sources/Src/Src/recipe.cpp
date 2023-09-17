/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Weld Recipe

***************************************************************************/


#include <QList>
#include <QQmlContext>
#include <QDateTime>
#include <QTime>
#include <QSettings>
#include <qmath.h>
#include "Header/communicationinterface.h"
#include "Header/pressureparameter.h"
#include "Header/recipe.h"
#include "Header/graphtableview.h"
#include "Header/informationpanel.h"
#include "Header/configuration.h"
#include "Header/setupparameter.h"
#include "Header/localizeheader.h"
#include "Header/stackrecipedata.h"
#include "Header/helperenums.h"
#include "Header/stackrecipedata.h"
#include "Header/common.h"

int SelectedIndex=0;
extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern unsigned int psFreqGlobalVar;
extern GraphTableView *graphtableview;
extern StackRecipeData *obj_stackrecipedata;
extern int dwCurrentUnit;
double partContactWindowMinus;
double partContactWindowPlus;
double partContactWindowOffset;

/**
 * @brief Recipe::Recipe
 * @param parent
 */
Recipe::Recipe(QObject *parent) : QObject(parent)
{
    m_CurrentSystemFreq = SYSTEM_TYPE_20KHZ;
    m_Units = UNIT_SI;
    dwLastSelectedResultIndex = 0;
    m_MachineFrequency = FrequencySelected20KHZ;
    m_clickedRecipe_Index = -1;
    m_isValidatevalue = 0;
    m_CopiedRecipeNumber = 0;
    oldRecipeNumber = "-1";
    m_RecipeModifyStatusFlag = 0;
    m_EditRecipeClicked = false;
    persistentRecipeNum = "";
    m_WeldRecipeParam.bIsWeldForceChanged = false;
    createAllModels();
    clearWeldResultList(&weldResults);
    refreshModel(ENUM_WeldResults);
    m_recipeButtonFullList.append(RECIPE_NEW);
    m_recipeButtonFullList.append(RECIPE_SAVE);
    m_recipeButtonFullList.append(RECIPE_COPY);
//    m_recipeButtonFullList.append(RECIPE_ACTIVE);
    m_recipeButtonFullList.append(RECIPE_VALIDATE);

    m_recipeButtonFullListOtherUsers.append(RECIPE_NEW);
    m_recipeButtonFullListOtherUsers.append(RECIPE_SAVE);
    m_recipeButtonFullListOtherUsers.append(RECIPE_COPY);
//    m_recipeButtonFullListOtherUsers.append(RECIPE_ACTIVE);

    m_recipeButtonFullListUnvalidated.append(RECIPE_NEW);
    m_recipeButtonFullListUnvalidated.append(RECIPE_SAVE);
    m_recipeButtonFullListUnvalidated.append(RECIPE_COPY);
    m_recipeButtonFullListUnvalidated.append(RECIPE_INVALIDATE);
    m_newRecipeCreated = false;

    QObject::connect(&CIObj , &CommunicationInterface::ChangeFromWSSignal , this , &Recipe::refreshScreenPopUp);

    auto recipeNumber = Configuration::getInstance()->getConfigValues("recipezero","recipe_number");

    if (recipeNumber.isEmpty())
    {
        Configuration::getInstance()->setConigValues("recipezero","recipe_number",QString::number(0));
    }

    auto recipeVersionNumber = Configuration::getInstance()->getConfigValues("recipezero","recipe_version_number");

    if (recipeVersionNumber.isEmpty())
    {
        Configuration::getInstance()->setConigValues("recipezero","recipe_version_number",QString::number(0));
    }
    
    isAutoGraphRefreshState = false;


}

/**
 * @brief Recipe::readRecipeFromDB
 * @param qstrRecipeNumber
 */
int Recipe::readRecipeFromDB(QString qstrRecipeNumber)
{
    unsigned char ErrorCode = 0;
    ErrorCode = m_WeldRecipeParam.readRecipeFromDB(qstrRecipeNumber);
    StackRecipeData::RecipeNumber=m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER).toInt();
    return ErrorCode;
}

/**
 * @brief Recipe::validateRecipeNumber
 * @param qstrNewRecipeNumber
 * @return
 */
int Recipe::validateRecipeNumber(QString qstrNewRecipeNumber)
{
    for(int index = 0; index < m_RecipeModelList.size(); index++)
    {
        RecipeInfo* ptrTempRecipeInfo = dynamic_cast<RecipeInfo *>(m_RecipeModelList.at(index));
        if(ptrTempRecipeInfo->m_RecipeNumber == qstrNewRecipeNumber)
            return 0;
    }

    return qstrNewRecipeNumber.toInt();
}

/**
 * @brief Recipe::getMaxRecipeNumberFromDb
 * @return
 */
QString Recipe::getMaxRecipeNumberFromDb()
{
    CIObj.sendMessage(REQ_GET_ADD_RECIPENUMBER,RES_SET_ADD_RECIPENUMBER,"");
    QString a_recipeBuffer="";
    CIObj.recivedMessage(RES_SET_ADD_RECIPENUMBER,a_recipeBuffer);
    return a_recipeBuffer;
}

/**
 * @brief Recipe::updateRecipeMenuList
 * @param userLevel
 */
void Recipe::updateRecipeMenuList(int userLevel)
{
    if (userLevel == static_cast<int>(HelperEnums::UserLevel::EXECUTIVE) || userLevel == static_cast<int>(HelperEnums::UserLevel::SUPERVISOR))
    {
        if(m_WeldRecipeParam.getStructureValueByIndex(UI_IS_MODIFIED) == "1")
        {
            context->setContextProperty("recipeButtonModel", QVariant::fromValue(m_recipeButtonFullListOtherUsers));
        }
        else if(m_WeldRecipeParam.getStructureValueByIndex(UI_ISVALID) == "0")
        {
            context->setContextProperty("recipeButtonModel", QVariant::fromValue(m_recipeButtonFullList));
        }
        else
        {
            context->setContextProperty("recipeButtonModel", QVariant::fromValue(m_recipeButtonFullListUnvalidated));
        }
    }
    else
        context->setContextProperty("recipeButtonModel", QVariant::fromValue(m_recipeButtonFullListOtherUsers));
}

/**
 * @brief Recipe::SetDefaultProductionDetail
 */
void Recipe::SetDefaultProductionDetail()
{
    QString qStrRecipeSetupInfo = "";
    qStrRecipeSetupInfo.append(m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER) + ",");
    qStrRecipeSetupInfo.append("0,0,'','',0,0,0,0");

    if(oldRecipeNumber == "-1")
    {

        CIObj.sendMessage(REQ_SET_RECIPE_SETUP_INFO,RES_SET_RECIPE_SETUP_INFO,qStrRecipeSetupInfo);
        QString result="";
        CIObj.recivedMessage(RES_SET_RECIPE_SETUP_INFO,result);
    }
    else
    {
        qStrRecipeSetupInfo = oldRecipeNumber + "," + qStrRecipeSetupInfo;
        CIObj.sendMessage(REQ_SET_RECIPE_SETUP_INFO_FOR_UPDATED,RES_SET_RECIPE_SETUP_INFO_FOR_UPDATED,qStrRecipeSetupInfo);
        QString result="";
        CIObj.recivedMessage(RES_SET_RECIPE_SETUP_INFO_FOR_UPDATED,result);
    }
}

/**
 * @brief Recipe::GetModeInfoByIndex
 * @param dwModeIndex
 * @param qstrModeValue
 * @return
 */
ModeInfo Recipe::GetModeInfoByIndex(int dwModeIndex,QString qstrModeValue)
{
    ModeInfo stModeInfo = m_WeldRecipeParam.getModeInfoByIndex(dwModeIndex,qstrModeValue);
    return stModeInfo;
}

/**
 * @brief Recipe::updateWeldHistory: This function will be called when weld finish indication comes from BL
 */
void Recipe::updateWeldHistory()
{
    if(isAutoGraphRefreshState)
        setIsManualRefreshState(true);
    else
        setIsManualRefreshState(false);
    updateWeldHistoryList(ENUM_LOAD_RECENT);
}

void Recipe::setHistoryToNull()
{
    refreshModel(ENUM_DummyWeldResults);
}

void Recipe::updateWeldHistoryList(int dwInsertIndex)
{
    int dwReqID,dwResID;
    int dwActivePageIndex = Configuration::getInstance()->getActivePage();
    QString qstrRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER);
    QString qstrCurrentCycleCounter = "0";
    if(qstrRecipeNumber=="0")
        qstrRecipeNumber = m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER);
    if(dwActivePageIndex == (int)OpenedScreenEnums::OpenedScreen::ANALYTICS)
    {
        qstrRecipeNumber="";
    }

	if(dwInsertIndex == ENUM_LOAD_NEXT_PAGE)
		setIsManualRefreshState(false);

    refreshModel(ENUM_DummyWeldResults);
    context->setContextProperty("deltaParametersModel",QVariant::fromValue(m_DummyModelList));
    context->setContextProperty("tableGridViewmodel", QVariant::fromValue(m_DummyModelList));
    switch(dwInsertIndex)
    {
    case ENUM_LOAD_ALL: //Reading the New list
        clearWeldResultList(&weldResults);
        clearWeldResultTableList(&m_TableGraphRow);
        dwReqID = REQ_GET_WELD_HISTORY_DATA;
        dwResID = RES_GET_WELD_HISTORY_DATA;
        break;
    case ENUM_LOAD_RECENT: //Reading the result which is happen just now
        dwReqID = REQ_GET_LAST_WELD_HISTORY_DATA;
        dwResID = RES_GET_LAST_WELD_HISTORY_DATA;
        if(weldResults.count() > 0)
        {
            WeldResultData *temp = dynamic_cast<WeldResultData *>(weldResults.at(0));
            qstrCurrentCycleCounter = temp->GetWeldResultNumber() + "," + temp->GetWeldResultID();
        }
        else
            qstrCurrentCycleCounter = "0";
        break;
    case ENUM_LOAD_NEXT_PAGE: //Reading the next page results
        dwReqID = REQ_GET_NEXT_WELD_HISTORY_DATA;
        dwResID = RES_GET_NEXT_WELD_HISTORY_DATA;
        if(weldResults.count() > 0)
        {
            WeldResultData *temp = dynamic_cast<WeldResultData *>(weldResults.at(weldResults.size()-1));
            qstrCurrentCycleCounter = temp->GetWeldResultNumber() + "," + temp->GetWeldResultID();
        }
        break;
    default:
        clearWeldResultList(&weldResults);
        dwReqID = REQ_GET_WELD_HISTORY_DATA;
        dwResID = RES_GET_WELD_HISTORY_DATA;
        break;
    }
    qstrRecipeNumber.append(","+qstrCurrentCycleCounter);
    QString result = "";
    QStringList WeldResultListObj;
    QStringList WeldResultList;

    CIObj.sendMessage(dwReqID,dwResID,qstrRecipeNumber);
    CIObj.recivedMessage(dwResID,result);

        /* Code that skip the stack serial number from the SC request result */
        WeldResultListObj = result.split(",");
        int OldTotalFieldsCrt=GRAPH_TABLE_MAX_FIELDS + 1; // full fields from query
        int TotalRowsCrt=(int)(WeldResultListObj.size()/OldTotalFieldsCrt); // calculate total rows
        int StartIndexCrt=0;
        WeldResultList.clear();
        for(int CountRows=0;CountRows<TotalRowsCrt;CountRows++)
        {
            for(int CountColumns=StartIndexCrt;CountColumns<OldTotalFieldsCrt+StartIndexCrt;CountColumns++)
            {
                int ColumnNumCrt = CountColumns-(CountRows*OldTotalFieldsCrt);
                if((ColumnNumCrt != GRAPH_TABLE_SKIP_STACK_SN) && (ColumnNumCrt != GRAPH_TABLE_SKIP_VELOCITY))
                {  /*Skip Stack Serial Number and Velocity*/
                    WeldResultList.append(WeldResultListObj[CountColumns]);
                }
            }
            StartIndexCrt+=OldTotalFieldsCrt;
        }
        /* end of skip stack serial number */

    int i,size=0;
    size = WeldResultList.size();

    if((result == "") || (size%(GRAPH_TABLE_MAX_FIELDS - 1) != 0))
    {
        refreshModel(ENUM_WeldResults);
        context->setContextProperty("tableGridViewmodel", QVariant::fromValue(m_TableGraphRow));
        context->setContextProperty("deltaParametersModel",QVariant::fromValue(graphtableview->m_CheckBoxParamList));
        return;
    }
    int dwTableIndex=0;
    int startIndex=0;
    for (i = 0;i<size;)
    {
        switch(dwInsertIndex)
        {
        case ENUM_LOAD_RECENT: //Insert in the top of the list
            weldResults.insert(dwTableIndex,new WeldResultData(WeldResultList[i+3], WeldResultList[i+4], WeldResultList[i],
                    WeldResultList[i+2], "", WeldResultList[i+25], WeldResultList[i+27]));
            m_TableGraphRow.insert(dwTableIndex,new TableRowData(graphtableview->joinQStringList(WeldResultList,startIndex,GRAPH_TABLE_MAX_FIELDS-2)));
            dwTableIndex++;
            break;
        case ENUM_LOAD_ALL: //Replace list with new list
        case ENUM_LOAD_NEXT_PAGE: //Insert in the bottom of the list
        default:
            weldResults.append(new WeldResultData(WeldResultList[i+3], WeldResultList[i+4], WeldResultList[i],
                    WeldResultList[i+2], "", WeldResultList[i+25], WeldResultList[i+27]));
            m_TableGraphRow.append(new TableRowData(graphtableview->joinQStringList(WeldResultList,startIndex,GRAPH_TABLE_MAX_FIELDS-2)));
        }

        startIndex+=(GRAPH_TABLE_MAX_FIELDS-1);
        i = i+(GRAPH_TABLE_MAX_FIELDS-1);
    }
    refreshModel(ENUM_WeldResults);
    context->setContextProperty("tableGridViewmodel", QVariant::fromValue(m_TableGraphRow));
    context->setContextProperty("deltaParametersModel",QVariant::fromValue(graphtableview->m_CheckBoxParamList));
}

void Recipe::updateTableGridViewModel()
{
    context->setContextProperty("tableGridViewmodel", QVariant::fromValue(m_DummyModelList));
    context->setContextProperty("tableGridViewmodel", QVariant::fromValue(m_TableGraphRow));
}

void Recipe::updateTableGraphModel()
{
    graphtableview->getWeldResultData();
}

/**
 * @brief Recipe::getCurrentRecipeName
 * @return
 */
QString Recipe::getCurrentRecipeName()
{
    return m_WeldRecipeParam.getStructureValueByIndex(UI_RECIPE_NAME);
}

/**
 * @brief Recipe::getDefaultValue
 * @param qstrParamName
 * @return
 */
QString Recipe::getDefaultValue(QString qstrParamName)
{
    return m_WeldRecipeParam.getDefaultValue(qstrParamName);
}

/**
 * @brief Recipe::existingRecipeCount
 * @return
 */
int Recipe::existingRecipeCount()
{
    return m_RecipeModelList.size()-1;
}

/**
 * @brief Recipe::setParamaterChangedFlag
 * @param flag
 */
void Recipe::setParamaterChangedFlag(int flag)
{
    m_WeldRecipeParam.setParamaterChangedFlag(flag);
}

/**
 * @brief Recipe::recallRecipeByBarcode
 * @param sRecipeNumber
 */
void Recipe::recallRecipeByBarcode(QString sRecipeNumber)
{
    int nReceipeNumber =0;
    QString sStatusText;
    QString sRecallResponse = QString::number(RECIPE_BARCODE_ACTIVE_RECIPE_NOT_FOUND);
    if(sRecipeNumber.length()> 0 )
    {
        loadRecipeCards();
        memcpy(&nReceipeNumber,sRecipeNumber.data(),sRecipeNumber.size());
        QString sActiveReceipe = QString::number(nReceipeNumber);
        int nRecipeCount = m_RecipeModelList.size();

        int nRecipeFound = 0;
        for(int nLoopCounter = 0;nLoopCounter < nRecipeCount;nLoopCounter++)
        {
            RecipeInfo* ptrTempRecipeInfo = dynamic_cast<RecipeInfo *>(m_RecipeModelList.at(nLoopCounter));
            if(ptrTempRecipeInfo->m_RecipeNumber == sActiveReceipe)
            {
                nRecipeFound =1;
            }
        }

        //check recipe here and display status in UI
        if(nRecipeFound == 1)
        {
            setSelectedRecipeToActive(sActiveReceipe);
            sStatusText = BARCODE_RECIPE + sActiveReceipe + RECIPE_BARCODE_FOUND;
            sRecallResponse = QString::number(RECIPE_BARCODE_ACTIVE_RECIPE_FOUND);
            emit refreshPage(Configuration::getInstance()->getActivePage());
        }
        else
            sStatusText = BARCODE_RECIPE + sActiveReceipe + RECIPE_BARCODE_NOTFOUND;

        //Display the text in main status bar
        emit updateStatusText(sStatusText);

    }
    //Send active recipe response to SC BL
    CIObj.sendMessage(SCBL_BARCODE_RECIPE_RECALL_RES,UIC_BARCODE_RECIPE_RECALL_REQ,sRecallResponse);
}

/**
 * @brief Recipe::barcodeConnectionStatus
 * @param sbarcodeConnectionStatus
 */
void Recipe::barcodeConnectionStatus(QString sbarcodeConnectionStatus)
{
    if(sbarcodeConnectionStatus.length() > 0 )
    {
        //will update the barcode connectivity status to the UI
        int nBarcodeConnectionStatus = sbarcodeConnectionStatus.toInt();

        QString strBarcodeConnectionStatus = ( (nBarcodeConnectionStatus == 1) ? RECIPE_BARCODE_PRESENT: RECIPE_BARCODE_NOTPRESENT);
        emit updateStatusText(strBarcodeConnectionStatus);
    }
}

/**
 * @brief Recipe::barcodeProcessInfo
 * @param sbarcodeProcessInfo
 */
void Recipe::barcodeProcessInfo(QString sbarcodeProcessInfo)
{
    if(sbarcodeProcessInfo.length() > 0 )
    {
        QString sbarcodeProcessInfostr;

        int nBarcodeProcessInformation = sbarcodeProcessInfo.toInt();
        switch(nBarcodeProcessInformation)
        {
        case 1:
            sbarcodeProcessInfostr = BARCODE_FAILURE_INVALID_RECIPE_BARCODE_NOT_FOUND;
            break;
        case 2:
            sbarcodeProcessInfostr = BARCODE_FAILURE_INVALID_RECIPE_BARCODE_LENGTH;
            break;
        case 3:
            sbarcodeProcessInfostr = BARCODE_FAILURE_INVALID_RECIPE_BARCODE;
            break;
        case 4:
            sbarcodeProcessInfostr = BARCODE_FAILURE_INVALID_PARTID;
            break;
        case 5:
            sbarcodeProcessInfostr = BARCODE_FAILURE_INVALID_SMART_STACK;
            break;
        default:
            sbarcodeProcessInfostr = BARCODE_FAILURE_INVALID_BARCODE;
            break;

        }
        emit updateStatusText(sbarcodeProcessInfostr);
    }
}

/**
 * @brief Recipe::getRecipeModifiedState
 * @return
 */
bool Recipe::getRecipeModifiedState()
{
    return m_WeldRecipeParam.getStructureValueByIndex(UI_IS_MODIFIED).toInt();
}

/**
 * @brief Recipe::setRecipeModifiedState
 * @param bState
 */
void Recipe::setRecipeModifiedState(bool bState)
{
    bool m_isRecipeModified = m_WeldRecipeParam.getStructureValueByIndex(UI_IS_MODIFIED).toInt();
    if(m_isRecipeModified != bState)
    {
        if(bState)
            m_WeldRecipeParam.setStructureVariableByIndex(UI_IS_MODIFIED,"1");
        else
            m_WeldRecipeParam.setStructureVariableByIndex(UI_IS_MODIFIED,"0");
        emit RecipeModifiedStateChanged();
    }
}

/**
 * @brief Recipe::getCurrentRecipeInfo
 * @return
 */
stModifiedRecipeInfo Recipe::getCurrentRecipeInfo()
{
    stModifiedRecipeInfo stInfo;
    stInfo.qstrRecipeName = m_WeldRecipeParam.getStructureValueByIndex(UI_RECIPE_NAME);
    int dwMode = m_WeldRecipeParam.getStructureValueByIndex(SC_WELD_MODE).toInt();
    ModeInfo stModeInfo = m_WeldRecipeParam.getModeInfoByIndex(dwMode,m_WeldRecipeParam.getStructureValueByIndex(SC_MODE_VALUE));
    stInfo.qstrWeldMode = stModeInfo.ModeName;
    stInfo.qstrModeNameScreen = stModeInfo.ModeNameScreen;
    stInfo.qstrModeValue = stModeInfo.ModeValue;
    stInfo.qstrUnit = stModeInfo.ModeUnit;
    return stInfo;
}

/**
 * @brief Recipe::getModifiedStatus
 * @param qstrRecipeNumber
 * @return
 */
bool Recipe::getModifiedStatus(QString qstrRecipeNumber)
{
    StackRecipeData::RecipeNumber = qstrRecipeNumber.toInt();
    if(qstrRecipeNumber == m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_VER_NUMBER) &&
         "0" == m_WeldRecipeParam.getStructureValueByIndex(SC_RECIPE_NUMBER))
    {
        return m_WeldRecipeParam.getStructureValueByIndex(UI_IS_MODIFIED).toInt();
    }
    return false;
}

/**
 * @brief Recipe::setSystemConfigData
 * @param dwMetrix
 * @param dwPsFreq
 */
void Recipe::setSystemConfigData(int dwMetrix, int dwPsFreq)
{
    m_Units = static_cast<ParamUnits>(dwMetrix);
    m_WeldRecipeParam.setMetrixUnit(dwMetrix);
    m_WeldRecipeParam.setPsFreqValue(dwPsFreq);
}

/**
 * @brief Recipe::setSelectedRecipeToValidate
 * @param qStrRecipeNum
 * @return
 */
int Recipe::setSelectedRecipeToValidate(QString qStrRecipeNum)
{
    if(m_WeldRecipeParam.getStructureValueByIndex(UI_ISVALID) == "0")
    {
        m_WeldRecipeParam.setStructureVariableByIndex(UI_ISVALID,1);
        CIObj.sendMessage(REQ_SET_RECIPE_VALIDATE,RES_SET_RECIPE_VALIDATE,qStrRecipeNum);
        QString a_recipeBuffer;
        CIObj.recivedMessage(RES_SET_RECIPE_VALIDATE,a_recipeBuffer);
        updateStatusText(RECIPE_VALIDATED);
    }
    else
    {
        m_WeldRecipeParam.setStructureVariableByIndex(UI_ISVALID,0);
        CIObj.sendMessage(REQ_SET_RECIPE_UNVALIDATE,RES_SET_RECIPE_UNVALIDATE,qStrRecipeNum);
        QString a_recipeBuffer;
        CIObj.recivedMessage(RES_SET_RECIPE_UNVALIDATE,a_recipeBuffer);
        updateStatusText(RECIPE_INVALIDATED);
    }
    return m_WeldRecipeParam.getStructureValueByIndex(UI_ISVALID).toInt();
}

/**
 * @brief Recipe::readResonantFrequencyFromDB
 */
void Recipe::readResonantFrequencyFromDB()
{
    CIObj.sendMessage(REQ_RESONANT_FREQUENCY,RES_RESONANT_FREQUENCY,"");
    QString a_response = "";
    CIObj.recivedMessage(RES_RESONANT_FREQUENCY,a_response);
    if(!a_response.isEmpty())
    {
        m_resonantFrequency = a_response.toDouble();
    }
    else
    {
        m_resonantFrequency = psFreqGlobalVar;
    }

}

/**
 * @brief WeldResultData::WeldResultData
 * @param WeldResultID
 * @param WeldResultDate
 * @param WeldResultTime
 */
WeldResultData::WeldResultData(QString WeldResultID,
                               QString WeldResultMode,
                               QString WeldResultNumber,
                               QString WeldResultDate,
                               QString WeldResultTime,
                               QString AlaramRaised,
                               QString RowId)
{
    m_WeldResultID = WeldResultID;
    m_WeldResultNumber = WeldResultNumber;
    m_WeldResultDate = WeldResultDate;
    m_WeldResultTime = WeldResultTime;
    m_AlaramRaised = AlaramRaised;
    if(WeldResultMode == WELDMODE_TIME)
        m_WeldResultMode = RECIPE_PARAM_TIME;
    else if(WeldResultMode == WELDMODE_ENERGY)
        m_WeldResultMode = RECIPE_PARAM_ENERGY;
    else if(WeldResultMode == WELDMODE_PEAK_POWER)
        m_WeldResultMode = RECIPE_PARAM_PEAK_POWER_SHORT_NAME;
    else if(WeldResultMode == WELDMODE_GROUND_DETECT)
        m_WeldResultMode = RECIPE_PARAM_GD_SHORT_NAME;
    else if(WeldResultMode == WELDMODE_ABS_DISTANCE)
        m_WeldResultMode = RECIPE_PARAM_AD_SHORT_NAME;
    else if(WeldResultMode == WELDMODE_COL_DISTANCE)
        m_WeldResultMode = RECIPE_PARAM_CD_SHORT_NAME;
    else if(WeldResultMode == WELDMODE_DYNAMIC)
        m_WeldResultMode = RECIPE_PARAM_DY;
    else
        m_WeldResultMode = RECIPE_PARAM_MULTI_MODE;
    m_WeldResultRowId = RowId;
}

/**
 * @brief WeldResultData::GetWeldResultID
 * @return
 */
QString WeldResultData::GetWeldResultID()
{
    return m_WeldResultID;
}

/**
 * @brief WeldResultData::GetWeldResultDate
 * @return
 */
QString WeldResultData::GetWeldResultDate()
{
    return m_WeldResultDate;
}

/**
 * @brief WeldResultData::GetAlaramRaised
 * @return
 */
QString WeldResultData::GetAlaramRaised()
{
    return m_AlaramRaised;
}

/**
 * @brief WeldResultData::GetWeldResultRowId
 * @return
 */
QString WeldResultData::GetWeldResultRowId()
{
    return m_WeldResultRowId;
}

/**
 * @brief WeldResultData::GetWeldResultTime
 * @return
 */
QString WeldResultData::GetWeldResultTime()
{
    return m_WeldResultTime;
}

/**
 * @brief WeldResultData::GetWeldResultMode
 * @return
 */
QString WeldResultData::GetWeldResultMode()
{
    return m_WeldResultMode;
}

/**
 * @brief WeldResultData::GetWeldResultNumber
 * @return
 */
QString WeldResultData::GetWeldResultNumber()
{
    return m_WeldResultNumber;
}

/*class Recipe Infoo Q_Property function definition.*/
/**
 * @brief RecipeInfo::getModelRecipeNumber
 * @return
 */
QString RecipeInfo::getModelRecipeNumber()
{
    return m_RecipeNumber;
}

/**
 * @brief RecipeInfo::getModelRecipeName
 * @return
 */
QString RecipeInfo::getModelRecipeName()
{
    return m_RecipeName;
}

/**
 * @brief RecipeInfo::getModelCompanyName
 * @return
 */
QString RecipeInfo::getModelCompanyName()
{
    return m_CompanyName;
}

/**
 * @brief RecipeInfo::getModelWeldMode
 * @return
 */
QString RecipeInfo::getModelWeldMode()
{
    return m_WeldMode;
}

/**
 * @brief RecipeInfo::getModelWeldModeNameScreen
 * @return
 */
QString RecipeInfo::getModelWeldModeNameScreen()
{
    return m_WeldModeNameScreen;
}

/**
 * @brief RecipeInfo::getModelWeldModeUnit
 * @return
 */
QString RecipeInfo::getModelWeldModeUnit()
{
    return m_WeldModeUnit;
}

/**
 * @brief RecipeInfo::getModelWeldModeValue
 * @return
 */
QString RecipeInfo::getModelWeldModeValue()
{
    return m_WeldModeValue;
}

/**
 * @brief RecipeInfo::getModelisActiveValue
 * @return
 */
bool RecipeInfo::getModelisActive()
{
    return m_isActive;
}

/**
 * @brief RecipeInfo::getModelisLockedValue
 * @return
 */
QString RecipeInfo::getModelisLockedValue()
{
    return m_isLockedValue;
}

/**
 * @brief RecipeInfo::getModelisValidateRecipe
 * @return
 */
bool RecipeInfo::getModelisValidate()
{
    return m_isValidate;
}

/**
 * @brief RecipeInfo::getModelisModified
 * @return
 */
bool RecipeInfo::getModelisModified()
{
    return m_isModified;
}

/**
 * @brief Recipe::getPastSelectIndex
 * @param qstrPrevResultSelected
 * @return
 */
int Recipe::getPastSelectIndex(QString qstrPrevResultSelected)
{
    int dwCnt = weldResults.size();
    for(int dwIndex = 0; dwIndex < dwCnt; dwIndex++)
    {
        WeldResultData *stTemp = dynamic_cast<WeldResultData *>(weldResults.at(dwIndex));
        QString qstrTemp = stTemp->GetWeldResultID();
        if(qstrTemp == qstrPrevResultSelected)
            return dwIndex;
    }
    return 0;
}

/**
 * @brief Recipe::weldDataDeleteIndication
 * @param qstrBuff
 */
void Recipe::weldDataDeleteIndication(QString qstrBuff)
{
    QStringList receivedBuffList = qstrBuff.split(",");

    if(!qstrBuff.isEmpty() && receivedBuffList.size() == 3)
    {
        QString recipeNum = receivedBuffList[0];
        QString cycleCounter = receivedBuffList[2];
        int dwCnt = weldResults.size();
        for(int dwIndex = 0; dwIndex < dwCnt; dwIndex++)
        {
            WeldResultData *stTemp = dynamic_cast<WeldResultData *>(weldResults.at(dwIndex));
            if((stTemp->GetWeldResultNumber() == recipeNum) && (stTemp->GetWeldResultID() == cycleCounter))
            {
                delete stTemp;
                stTemp = NULL ;
                weldResults.removeAt(dwIndex);
                break;
            }
            dwCnt = weldResults.size();
        }
        refreshModel(ENUM_WeldResults);
    }
}

/**
 * @brief Recipe::getLastSelectedResult This function will return the Last selected result Index
 * @return
 */
QString Recipe::getLastSelectedResult()
{
    return QString::number(dwLastSelectedResultIndex);
}

/**
 * @brief Recipe::setLastSelectedResult: This function will set the Last Selected index with argument
 * @param dwIndex
 */
void Recipe::setLastSelectedResult(QString qstrIndex)
{
    dwLastSelectedResultIndex = qstrIndex.toInt();
}

/**
 * @brief Recipe::isActiveRequired: This function will set a flag which is used to verify that recipe zero need to sedn to BL or not.
 * @param bIsActiveReq
 */
void Recipe::isActiveRequired(bool bIsActiveReq)
{
    m_WeldRecipeParam.setIsActiveRequired(bIsActiveReq);
}

bool Recipe::isRuntimeFeatureEnabledForCFR()
{
#ifdef CFR_ENABLED
    return true;
#else
    return false;
#endif
}

/**
 * @brief Recipe::refreshScreenPopUp: This function will send a pop up to the user to let him know that
 *                                    there was a change from the Web Services.
 * @param
 */
void Recipe::refreshScreenPopUp(QString typeOfChange)
{
    QString popUpString = WEB_SERVICES_CHANGED;

    switch(typeOfChange.toInt())
    {
    case SET_ACTIVE_RECIPE:
        popUpString += ACTIVE_RECIPE_CHANGED;
        break;
    case SAVE_CURRENT_RECIPE:
        popUpString += ACTIVE_RECIPE_SAVED;
        break;
    case SET_RECIPE_VALUE:
        popUpString += RECIPE_PARAMETER_CHANGED;
        break;
    case DELETE_RECIPE:
        popUpString += RECIPE_DELETED;
        break;
    default:
        break;
    }

    readRecipeFromDB("0");

    loadAllModels();
    if ((int)OpenedScreenEnums::OpenedScreen::RECIPE == Configuration::getInstance()->getActivePage())
    {
        loadRecipeCards();
    }

    popUpString += SWITCH_SCREENS_MESSAGE;

    emit refreshScreenPopUpSignal(popUpString, typeOfChange.toInt());
}
