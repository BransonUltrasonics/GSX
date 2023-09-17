
/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Production Setup Info

***************************************************************************/
#include <QString>
#include <QObject>
#include "Header/communicationinterface.h"
#include "Header/productionsetupinfo.h"

extern CommunicationInterface CIObj;

/**
 * @brief GetRecipeSetupInfo : Set recipe number to BL
 * @param qStrRecipeName
 * @return
 */
QString GetRecipeSetupInfo(QString qStrRecipeNum)
{
    CIObj.sendMessage(REQ_GET_RECIPE_SETUP_INFO,RES_GET_RECIPE_SETUP_INFO,qStrRecipeNum);
    QString recipeInfo;
    CIObj.recivedMessage(RES_GET_RECIPE_SETUP_INFO,recipeInfo);
    return recipeInfo;
}

/**
 * @brief ProductionSetUpInfo::ProductionSetUpInfo : Constructor
 * @param parent
 */
ProductionSetUpInfo::ProductionSetUpInfo(QObject *parent) : QObject(parent)
{
}

/**
 * @brief ProductionSetUpInfo::recipeSetupInfoInit : Recipe information in production screen
 * @param qStrRecipeName
 * @param qStrRecipeNumber
 */
void ProductionSetUpInfo::recipeSetupInfoInit(QString qStrRecipeNumber,QString qstrRecipeName )
{
    m_stProductionSetUpInfo.m_CycleCount = "0";
    m_stProductionSetUpInfo.m_ProductionInstruction = CUST_NAME_NA;
    m_stProductionSetUpInfo.m_RecipeDescription = CUST_NAME_NA;
    m_stProductionSetUpInfo.m_BatchSetupEnable = 0;
    m_stProductionSetUpInfo.m_CountWithAlarmEnable = 0;
    m_stProductionSetUpInfo.m_BatchCount = CUST_NAME_BATCHCOUNT_DEFAULT;
    m_stProductionSetUpInfo.m_BatchId = CUST_NAME_NA;
    m_stProductionSetUpInfo.m_RecipeNumber = qStrRecipeNumber;

    m_qstrRecipeName = qstrRecipeName;
    m_qstrRecipeNumber = qStrRecipeNumber;
    QString m_recipeInfo = GetRecipeSetupInfo(qStrRecipeNumber);
    QStringList m_recipeInfoBuffer = m_recipeInfo.split(",");
    if ( !m_recipeInfo.isEmpty() && (m_recipeInfoBuffer.size()%7)==0 )
    {
        FillRecipeInfo(m_recipeInfoBuffer);
        m_recipeInfo.clear();
    }
}

/**
 * @brief ProductionSetUpInfo::FillRecipeInfo : Set values to Structure
 * @param m_recipeInfoBuffer
 */
void ProductionSetUpInfo::FillRecipeInfo(QStringList m_recipeInfoBuffer)
{
    m_stProductionSetUpInfo.m_CycleCount            = m_recipeInfoBuffer[0];
    m_stProductionSetUpInfo.m_ProductionInstruction = m_recipeInfoBuffer[1];
    m_stProductionSetUpInfo.m_RecipeDescription     = m_recipeInfoBuffer[2];
    m_stProductionSetUpInfo.m_BatchCount            = m_recipeInfoBuffer[3];
    m_stProductionSetUpInfo.m_BatchId               = m_recipeInfoBuffer[4];
    m_stProductionSetUpInfo.m_BatchSetupEnable      = m_recipeInfoBuffer[5].toInt();
    m_stProductionSetUpInfo.m_CountWithAlarmEnable  = m_recipeInfoBuffer[6].toInt();
}

/**
 * @brief ProductionSetUpInfo::getRecipeName : Return recipe name
 * @return
 */
QString ProductionSetUpInfo::getRecipeName()
{
    return m_qstrRecipeName;
}

/**
 * @brief ProductionSetUpInfo::getRecipeNumber : Return recipe number
 * @return
 */
QString ProductionSetUpInfo::getRecipeNumber()
{
    return m_qstrRecipeNumber;
}

/**
 * @brief ProductionSetUpInfo::getCycleCount : Return cycle count
 * @return
 */
QString ProductionSetUpInfo::getCycleCount()
{
    return m_stProductionSetUpInfo.m_CycleCount;
}

/**
 * @brief ProductionSetUpInfo::setCycleCount : Set cycle count
 * @param a_CycleCount
 */
void ProductionSetUpInfo::setCycleCount(QString a_CycleCount)
{
    m_stProductionSetUpInfo.m_CycleCount=a_CycleCount;
}

/**
 * @brief ProductionSetUpInfo::getProductionInstruction : Return production instruction value
 * @return
 */
QString ProductionSetUpInfo::getProductionInstruction()
{
    return m_stProductionSetUpInfo.m_ProductionInstruction;
}

/**
 * @brief ProductionSetUpInfo::setProductionInstruction : Production instruction update to structure
 * @param a_ProductionInstruction
 */
void ProductionSetUpInfo::setProductionInstruction(QString a_ProductionInstruction)
{
    m_stProductionSetUpInfo.m_ProductionInstruction=a_ProductionInstruction;
}

/**
 * @brief ProductionSetUpInfo::getRecipeDescription : Recipe description update to structure
 * @return
 */
QString ProductionSetUpInfo::getRecipeDescription()
{
    return m_stProductionSetUpInfo.m_RecipeDescription;
}

/**
 * @brief ProductionSetUpInfo::getBatchId : Return batch id
 * @return
 */
QString ProductionSetUpInfo::getBatchId()
{
    return m_stProductionSetUpInfo.m_BatchId;
}

/**
 * @brief ProductionSetUpInfo::setRecipeDescription : Set recipe description
 * @param a_RecipeDescription
 */
void ProductionSetUpInfo::setRecipeDescription(QString a_RecipeDescription)
{
    m_stProductionSetUpInfo.m_RecipeDescription=a_RecipeDescription;
}

/**
 * @brief ProductionSetUpInfo::getBatchCount
 * @return
 */
QString ProductionSetUpInfo::getBatchCount()
{
    return m_stProductionSetUpInfo.m_BatchCount;
}

/**
 * @brief ProductionSetUpInfo::setBatchCount : Set batch count
 * @param a_BatchCount
 */
void ProductionSetUpInfo::setBatchCount(QString a_BatchCount)
{
    m_stProductionSetUpInfo.m_BatchCount=a_BatchCount;
}

/**
 * @brief ProductionSetUpInfo::getCountWithAlarmEnable
 * @return
 */
int ProductionSetUpInfo::getCountWithAlarmEnable()
{
    return m_stProductionSetUpInfo.m_CountWithAlarmEnable;
}

/**
 * @brief ProductionSetUpInfo::setCountWithAlarmEnable  : Set production count with Alarm Enable
 * @param a_CountWithAlarmEnable
 */
void ProductionSetUpInfo::setCountWithAlarmEnable(int a_CountWithAlarmEnable)
{
    m_stProductionSetUpInfo.m_CountWithAlarmEnable = a_CountWithAlarmEnable;
}

/**
 * @brief ProductionSetUpInfo::setBatchId : Set batch count
 * @param a_BatchId
 */
void ProductionSetUpInfo::setBatchId(QString a_BatchId)
{
    m_stProductionSetUpInfo.m_BatchId = a_BatchId;
}

/**
 * @brief ProductionSetUpInfo::getBatchSetupEnable
 * @return
 */
int ProductionSetUpInfo::getBatchSetupEnable()
{
    return m_stProductionSetUpInfo.m_BatchSetupEnable;
}

/**
 * @brief ProductionSetUpInfo::setBatchSetupEnable
 * @param a_BatchSetupEnable
 */
void ProductionSetUpInfo::setBatchSetupEnable(int a_BatchSetupEnable)
{
    m_stProductionSetUpInfo.m_BatchSetupEnable = a_BatchSetupEnable;
}

/**
 * @brief ProductionSetUpInfo::ProductionSetUpInfo
 * @param a_productionSetUpInfo
 * @param parent
 */
ProductionSetUpInfo::ProductionSetUpInfo(const ProductionSetUpInfo &a_productionSetUpInfo,QObject *parent):QObject(parent)
{
    m_stProductionSetUpInfo.m_CycleCount = a_productionSetUpInfo.m_stProductionSetUpInfo.m_CycleCount;
    m_stProductionSetUpInfo.m_ProductionInstruction = a_productionSetUpInfo.m_stProductionSetUpInfo.m_ProductionInstruction;
    m_stProductionSetUpInfo.m_RecipeDescription = a_productionSetUpInfo.m_stProductionSetUpInfo.m_RecipeDescription;
    m_stProductionSetUpInfo.m_BatchCount    = a_productionSetUpInfo.m_stProductionSetUpInfo.m_BatchCount;
    m_stProductionSetUpInfo.m_BatchSetupEnable    = a_productionSetUpInfo.m_stProductionSetUpInfo.m_BatchSetupEnable;
    m_stProductionSetUpInfo.m_CountWithAlarmEnable    = a_productionSetUpInfo.m_stProductionSetUpInfo.m_CountWithAlarmEnable;
    m_stProductionSetUpInfo.m_BatchId   = a_productionSetUpInfo.m_stProductionSetUpInfo.m_BatchId;
}

/**
 * @brief ProductionSetUpInfo::operator =
 * @param a_productionSetUpInfo
 * @return
 */
ProductionSetUpInfo &ProductionSetUpInfo::operator =(ProductionSetUpInfo a_productionSetUpInfo){

    m_stProductionSetUpInfo.m_CycleCount = a_productionSetUpInfo.m_stProductionSetUpInfo.m_CycleCount;
    m_stProductionSetUpInfo.m_ProductionInstruction = a_productionSetUpInfo.m_stProductionSetUpInfo.m_ProductionInstruction;
    m_stProductionSetUpInfo.m_RecipeDescription = a_productionSetUpInfo.m_stProductionSetUpInfo.m_RecipeDescription;
    m_stProductionSetUpInfo.m_BatchCount = a_productionSetUpInfo.m_stProductionSetUpInfo.m_BatchCount;
    m_stProductionSetUpInfo.m_BatchSetupEnable = a_productionSetUpInfo.m_stProductionSetUpInfo.m_BatchSetupEnable;
    m_stProductionSetUpInfo.m_CountWithAlarmEnable = a_productionSetUpInfo.m_stProductionSetUpInfo.m_CountWithAlarmEnable;
    m_stProductionSetUpInfo.m_BatchId   = a_productionSetUpInfo.m_stProductionSetUpInfo.m_BatchId;
    return *this;
}

/**
 * @brief ProductionSetUpInfo::savetoDB : Save production information in DB
 */
void ProductionSetUpInfo::savetoDB()
{
    QString qStrRecipeSetupInfo =
             m_qstrRecipeNumber+","
            +m_stProductionSetUpInfo.m_CycleCount+",'"
            +m_stProductionSetUpInfo.m_ProductionInstruction+"','"
            +m_stProductionSetUpInfo.m_RecipeDescription+"',"
            +m_stProductionSetUpInfo.m_BatchCount+",'"
            +m_stProductionSetUpInfo.m_BatchId+"',"
            +QString::number(m_stProductionSetUpInfo.m_BatchSetupEnable)+","
            +QString::number(m_stProductionSetUpInfo.m_CountWithAlarmEnable);
    CIObj.sendMessage(REQ_SET_RECIPE_SETUP_INFO,RES_SET_RECIPE_SETUP_INFO,qStrRecipeSetupInfo);
    QString result;
    CIObj.recivedMessage(RES_SET_RECIPE_SETUP_INFO,result);
}

/**
 * @brief ProductionSetUpInfo::checkBatchCount : Check the updated batch count
 * @return
 */
int ProductionSetUpInfo::checkBatchCount()
{
    CIObj.sendMessage(REQ_GET_BATCH_COUNT,RES_GET_BATCH_COUNT,m_qstrRecipeNumber);
    QString result;
    CIObj.recivedMessage(RES_GET_BATCH_COUNT,result);
    if(!result.isEmpty())
    {
        QStringList currentBatchResult = result.split(",");
        m_batchCountDBVal = currentBatchResult[0];
        if(currentBatchResult[0].toInt() == m_stProductionSetUpInfo.m_BatchCount.toInt())
        {
            return OVERWRITE_BATCHCOUNT;
        }
        else if(m_stProductionSetUpInfo.m_BatchCount.toInt() > currentBatchResult[0].toInt())
        {
            return OVERWRITE_BATCHCOUNT;
        }
        else if((m_stProductionSetUpInfo.m_BatchCount.toInt() < currentBatchResult[0].toInt()) && (m_stProductionSetUpInfo.m_BatchCount.toInt() > currentBatchResult[1].toInt()))
        {
            return OVERWRITE_BATCHCOUNT;
        }
        else if((m_stProductionSetUpInfo.m_BatchCount.toInt() < currentBatchResult[0].toInt()) && (m_stProductionSetUpInfo.m_BatchCount.toInt() < currentBatchResult[1].toInt()))
        {
            return POPUP_BATCHCOUNT;
        }
        return OVERWRITE_BATCHCOUNT;
    }
    else
        return 0;
}

/**
 * @brief ProductionSetUpInfo::getActiveRecipeNumberForProduction : Active recipe information to production screen
 * @return
 */
QString ProductionSetUpInfo::getActiveRecipeNumberForProduction()
{
    CIObj.sendMessage(REQ_GET_ACTIVE_RECIPE_NUMBER,RES_GET_ACTIVE_RECIPE_NUMBER,"");
    QString result;
    CIObj.recivedMessage(RES_GET_ACTIVE_RECIPE_NUMBER,result);
    return result;
}

/**
 * @brief ProductionSetUpInfo::sendResetBatchCounterReqToSC : Reset the batch count information Request to BL
 * @return
 */
int ProductionSetUpInfo::sendResetBatchCounterReqToSC()
{
    int status,SUCCESS = 1;
    char *buff = NULL;
    QString recipeDetails;
    QString result;
    QStringList recipeNameAndNumber;
    recipeDetails = getActiveRecipeNumberForProduction();
    recipeNameAndNumber = recipeDetails.split(",");
    if(recipeNameAndNumber[0] != m_stProductionSetUpInfo.m_RecipeNumber)
    {
        CIObj.sendMessage(REQ_RESET_BATCH_COUNTER,RES_RESET_BATCH_COUNTER,m_stProductionSetUpInfo.m_RecipeNumber);
        CIObj.recivedMessage(RES_RESET_BATCH_COUNTER,result);
        return SUCCESS;
    }
    else
    {
        CIObj.sendMessage(SCBL_RESET_BATCH_COUNTER_REQ,UIC_RESET_BATCH_COUNTER_RSP,buff,0);
        char resetStatus[4]={'\0'};
        CIObj.recivedMessage(UIC_RESET_BATCH_COUNTER_RSP,resetStatus,1);
        status = resetStatus[0]-48;
        return status;
    }
    return SUCCESS;
}

/**
 * @brief ProductionSetUpInfo::replaceBatchCountWithOldVal : Replace the old batch count to new batch count
 */
void ProductionSetUpInfo::replaceBatchCountWithOldVal()
{
    m_stProductionSetUpInfo.m_BatchCount = m_batchCountDBVal;
}
