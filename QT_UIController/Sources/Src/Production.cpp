/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Production

***************************************************************************/
#include <QQuickView>
#include <QQmlContext>
#include <QList>
#include "Header/communicationinterface.h"
#include "Header/dashboard.h"
#include "Header/configuration.h"
#include "Header/Production.h"
#include "Header/recipe.h"
#include "Header/common.h"

#include "Header/utility.h"
extern Recipe *recipe;
extern QObject * object;
extern CommunicationInterface CIObj;
extern QQmlContext* context;
extern unsigned int psFreqGlobalVar;
extern int dwCurrentUnit;

/**
 * @brief Production::GetActiveRecipeForProduction : Get the Active recipe information for production screen
 * @param qstrSupportedModes
 * @return
 */
QString Production::GetActiveRecipeForProduction(QString qstrSupportedModes)
{
    CIObj.sendMessage(REQ_GET_ACTIVE_RECIPE, RES_GET_ACTIVE_RECIPE, qstrSupportedModes);
    QString a_activeRecipeBuffer;
    CIObj.recivedMessage(RES_GET_ACTIVE_RECIPE,a_activeRecipeBuffer);
    return a_activeRecipeBuffer;
}

/**
 * @brief Production::GetProductionWeldResultForProduction : Get production weld result production
 * @return
 */
QString Production::GetProductionWeldResultForProduction()
{
    CIObj.sendMessage(REQ_GET_PRODUCTION_WELD_RESULT, RES_GET_PRODUCTION_WELD_RESULT, m_recipeNumber);
    QString a_productionWeldResultBuffer;
    CIObj.recivedMessage(RES_GET_PRODUCTION_WELD_RESULT,a_productionWeldResultBuffer);
    return a_productionWeldResultBuffer;
}

/**
 * @brief Production::GetLastWeldPeakPower : Get last weld peak power information
 * @return
 */
QString Production::GetLastWeldPeakPower()
{   
    CIObj.sendMessage(REQ_GET_LAST_WELD_PEAKPOWER, RES_GET_LAST_WELD_PEAKPOWER, m_recipeNumber);
    QString a_productionPeakPowerBuffer;
    CIObj.recivedMessage(RES_GET_LAST_WELD_PEAKPOWER,a_productionPeakPowerBuffer);
    return a_productionPeakPowerBuffer;
}

/**
 * @brief Production::GetSupportedModes : Get all the supported modes
 * @return
 */
QString Production::GetSupportedModes()
{
    QString supportedModes="";

    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_TIME) == 1)
    {
        supportedModes.append("1,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ENERGY) == 1)
    {
        supportedModes.append("2,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_POWER) == 1)
    {
        supportedModes.append("3,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_GROUND_DETECT) == 1)
    {
        supportedModes.append("4,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_ABSOLUTE_DISTANCE) == 1)
    {
        supportedModes.append("5,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_COLLAPSE_DISTANCE) == 1)
    {
        supportedModes.append("6,");
    }
    if(Configuration::getInstance()->getFeatureFlagStatus(MODE_DYNAMIC) == 1)
    {
        supportedModes.append("7,");
    }

    supportedModes.remove(supportedModes.length()-1,1);
    return supportedModes;
}

/**
 * @brief Production::Production : Initialization of class members
 * @param parent
 */
Production::Production(QObject *parent) : QObject(parent)
{
    m_goodNum = 0;
    m_rejectNum = 0;
    m_suspectNum = 0;
    m_partsPerMin = 0;
    m_batchCount = 0;
    m_BatchCounter = 0;
    m_cycleCount = 0;
    m_weldProgressPerc =0;
    m_goodByGoalPerc = 0;
    m_batchSetupEnable = 0;

    m_weldStatus = CUST_NAME_LOAD_PART;
    QObject::connect(&CIObj , &CommunicationInterface::productionUpdate , this , &Production :: updateProductionResults);
    QObject::connect(&CIObj , &CommunicationInterface::partidstatus , this , &Production :: partidstatusInfo);
    QObject::connect(&CIObj , &CommunicationInterface::productionPowerUpdate , this , &Production :: updateProductionPower);
}

/**
 * @brief Production::productionInit : Get supported modes and assign in to list
 */
void Production::productionInit()
{
    QString supportedModes = GetSupportedModes();
    QString m_productionActive = Production::GetActiveRecipeForProduction(supportedModes);
    QStringList m_activeBuffer = m_productionActive.split(",");

    if ( !m_productionActive.isEmpty()  && (m_activeBuffer.size()%8)==0)
    {
        Production::FillActiveRecipe(m_activeBuffer);
        m_productionActive.clear();
    }
    else
    {
        m_recipeNumber = "NA";
        m_recipeName = "NA";
        m_controlValue = "NA";
        m_weldMode = "NA";
        m_controlValueUnit = "NA";
    }

    updateProductionScreenResults();
    QString peakpower = GetLastWeldPeakPower();
    if(peakpower.isEmpty() || peakpower.isNull())
        peakpower="0";
    int dwMaxPower = Configuration::getInstance()->m_systemData.PcConfig.PSWatt;

    double dwCurrentPower = peakpower.toDouble();
    double dwPer = (dwCurrentPower*100)/dwMaxPower;
    m_weldProgressPerc = round(dwPer);
    m_peakPower = peakpower.toInt();
    emit partidstatusInfo("");
}

/**
 * @brief Production::updateProductionScreenResults : Update the production screen result
 */
void Production::updateProductionScreenResults()
{
    QString m_productionWeldResult = GetProductionWeldResultForProduction();
    if (!m_productionWeldResult.isEmpty())
    {
        QStringList m_productionWeldResultBuffer = m_productionWeldResult.split(",");
        if((m_productionWeldResultBuffer.size()%8) == 0)
        {
            FillProductionWeldResult(m_productionWeldResultBuffer);
        }
        m_productionWeldResult.clear();
    }

    if(m_batchCount)
        m_goodByGoalPerc = ((m_BatchCounter*100)/m_batchCount)*2;
    else
        m_goodByGoalPerc = 100;
    emit OnGoodByGoalPercChanged();
}

/**
 * @brief Production::productionStatus : Production status
 * @param a_status
 */
void Production::productionStatus(bool a_status)
{
    productionPageStatus = a_status;
}

/**
 * @brief Production::FillActiveRecipe : Assign string list values to member variable
 * @param m_activeBuffer
 */
void Production::FillActiveRecipe ( QStringList m_activeBuffer )
{
    double dTime;
    m_recipeNumber = m_activeBuffer[0];
    m_recipeName = m_activeBuffer[1];
    int dwUnit = Configuration::getInstance()->m_systemData.UnitIs;
    QString distanceUnit = CUST_NAME_UINTMilliMeter;
    if(dwUnit)
        distanceUnit = CUST_NAME_UINTINCHES;
    if(recipe->getModifiedStatus(m_recipeNumber))
    {
        stModifiedRecipeInfo stTemp = recipe->getCurrentRecipeInfo();
        m_recipeName = stTemp.qstrRecipeName;
        m_weldMode = stTemp.qstrModeNameScreen;
        m_controlValueUnit = stTemp.qstrUnit;
        m_controlValue = stTemp.qstrModeValue;
    }
    else
    {
        m_weldMode = m_activeBuffer[3];
        if (m_weldMode == WELDMODE_TIME)
		{
            m_controlValueUnit = SECONDS;
            m_weldMode = CUST_NAME_TIME;
        }
        else if (m_weldMode == WELDMODE_ENERGY)
		{
            m_weldMode = CUST_NAME_ENERGY;
            m_controlValueUnit =JOULE;
        }
        else if (m_weldMode == WELDMODE_PEAK_POWER)
		{
            m_weldMode = CUST_NAME_PEAK;
            m_controlValueUnit = WATT;
        }
        else if (m_weldMode == WELDMODE_GROUND_DETECT)
		{
            m_weldMode=CUST_NAME_SCRUB;
            m_controlValueUnit = SECONDS;
        }
        else if (m_weldMode == WELDMODE_ABS_DISTANCE)
		{
            m_weldMode=CUST_NAME_ABSOLUTE;
            m_controlValueUnit = distanceUnit;
        }
         else if (m_weldMode == WELDMODE_COL_DISTANCE)
		{
            m_weldMode=CUST_NAME_COLLAPSE;
            m_controlValueUnit = distanceUnit;
        }
        else if (m_weldMode == WELDMODE_DYNAMIC)
       {
           m_weldMode=CUST_NAME_DYNAMIC;
           m_controlValueUnit = PERCENTAGE;
       }
        if (m_controlValueUnit == CUST_NAME_UINTSEC)
        {           
                dTime = Configuration::getInstance()->convertMsecToSec(m_activeBuffer[4].toDouble());
                m_controlValue.setNum(dTime,'f',3);
        }
        else if( m_controlValueUnit == CUST_NAME_UINTMilliMeter)
        {
                dTime = Configuration::getInstance()->convertMicrometreToMillimetre(m_activeBuffer[4].toDouble());
                /*For default value precision for Collpse Distance and Absolute Distance on production screen*/
                if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
                {
                    m_controlValue.setNum(dTime,'f',2);
                }

                else
                {
                    m_controlValue.setNum(dTime,'f',3);
                }
        }
        else if( m_controlValueUnit == CUST_NAME_UINTINCHES)
        {
                dTime = Configuration::getInstance()->convertmicrometretoinches(m_activeBuffer[4].toDouble());
                m_controlValue.setNum(dTime,'f',4);
        }
        else if( m_controlValueUnit == JOULE)
        {
                dTime = Utility::convertMilliJoulesToJoules(m_activeBuffer[4].toDouble());
                m_controlValue.setNum(dTime,'f',1);
        }
        else
        {
            m_controlValue = m_activeBuffer[4];
        }
    }
}

/**
 * @brief Production::FillProductionWeldResult
 * @param m_productionWeldResultBuffer
 */
void Production::FillProductionWeldResult ( QStringList m_productionWeldResultBuffer )
{
    m_goodNum = m_productionWeldResultBuffer[0].toInt();
    m_partsPerMin = 0;
    m_rejectNum = m_productionWeldResultBuffer[2].toInt();
    m_suspectNum = m_productionWeldResultBuffer[3].toInt();
    m_batchCount = m_productionWeldResultBuffer[4].toInt();
    m_BatchCounter = m_productionWeldResultBuffer[5].toInt();
    m_cycleCount = m_productionWeldResultBuffer[6].toInt();
    m_batchSetupEnable = m_productionWeldResultBuffer[7].toInt();

    if(CIObj.queueWelds.size() >= 1)
        m_partsPerMin = CIObj.queueWelds.last() - CIObj.queueWelds.first();

    //SetBatchCounterNum(m_productionWeldResultBuffer[5]);
}

/**
 * @brief Production::refreshPartsPerMin : Refreshes parts per minute in UI
 */
void Production::refreshPartsPerMin()
{
    int dwValue = 0;
    if(CIObj.queueWelds.size() >= 1)
        dwValue = CIObj.queueWelds.last() - CIObj.queueWelds.first();
    SetPartPerMin(dwValue);
}

/**
 * @brief Production::GetRecipeName : Return recipe name
 * @return
 */
QString Production::GetRecipeName()
{
    return m_recipeName;
}

/**
 * @brief Production::GetRecipeNumber : Return recipe number
 * @return
 */
QString Production::GetRecipeNumber()
{
    return m_recipeNumber;
}

/**
 * @brief Production::SetRecipeName : Set recipe name
 * @param RecipeName
 */
void Production::SetRecipeName(QString RecipeName)
{
    if(m_recipeName!=RecipeName)
    {
        m_recipeName=RecipeName;
        emit OnRecipeNameChanged();
    }
}

/**
 * @brief Production::GetWeldStatus : Return weld status
 * @return
 */
QString Production::GetWeldStatus()
{
    return m_weldStatus;
}

/**
 * @brief Production::SetWeldStatus : Set weld status and emit the signal
 * @param a_weldStatus
 */
void Production::SetWeldStatus(QString a_weldStatus)
{
    if(m_weldStatus!=a_weldStatus)
    {
        m_weldStatus=a_weldStatus;
        emit OnWeldStatusChanged();
    }
}

/**
 * @brief Production::GetWeldMode : Return weld mode
 * @return
 */
QString Production::GetWeldMode()
{
    return m_weldMode;
}

/**
 * @brief Production::GetControlValue : Return control value
 * @return
 */
QString Production::GetControlValue()
{
    return m_controlValue;
}

/**
 * @brief Production::GetControlValueUnit : Return control value unit
 * @return
 */
QString Production::GetControlValueUnit()
{
    return m_controlValueUnit;
}

/**
 * @brief Production::GetPartPerMinChanged : Return part per minutes changed
 * @return
 */
int Production::GetPartPerMinChanged()
{
    return m_partsPerMin;
}

/**
 * @brief Production::SetPartPerMin : Set part per minutes
 * @param a_PartPerMin
 */
void Production::SetPartPerMin(int a_PartPerMin)
{
    if(m_partsPerMin!=a_PartPerMin)
    {
        m_partsPerMin=a_PartPerMin;
        emit OnPartPerMinChanged();
    }
}

/**
 * @brief Production::GetWeldProgressPerc : Return weld progress
 * @return
 */
int Production::GetWeldProgressPerc()
{
    return m_weldProgressPerc;
}

/**
 * @brief Production::SetWeldProgressPerc : Set weld progess
 * @param a_weldProgressPerc
 */
void Production::SetWeldProgressPerc(int a_weldProgressPerc)
{
    if(m_weldProgressPerc!=a_weldProgressPerc)
    {
        m_weldProgressPerc=a_weldProgressPerc;
        emit OnWeldProgressPercChanged();
    }
}

/**
 * @brief Production::GetGoodNum : Return good weld number
 * @return
 */
int Production::GetGoodNum()
{
    return m_goodNum;
}

/**
 * @brief Production::SetGoodNum : Set good weld number
 * @param Good
 */
void Production::SetGoodNum(int Good)
{
    if(m_goodNum!=Good)
    {
        m_goodNum=Good;
        emit OnGoodNumChanged();
    }
}

/**
 * @brief Production::GetRejectNum : Return reject number
 * @return
 */
int Production::GetRejectNum()
{
    return m_rejectNum;
}

/**
 * @brief Production::SetRejectNum : Set reject number
 * @param Reject
 */
void Production::SetRejectNum(int Reject)
{
    if(m_rejectNum!=Reject)
    {
        m_rejectNum=Reject;
        emit OnRejectNumChanged();
    }
}

/**
 * @brief Production::GetSuspectNum : Get suspect number weld number
 * @return
 */
int Production::GetSuspectNum()
{
    return m_suspectNum;
}

/**
 * @brief Production::SetSuspectNum : Set suspect number weld number
 * @param Suspect
 */
void Production::SetSuspectNum(int Suspect)
{
    if(m_suspectNum!=Suspect)
    {
        m_suspectNum=Suspect;
        emit OnSuspectNumChanged();
    }
}

/**
 * @brief Production::GetCycleCountNum : Return cycle count number
 * @return
 */
int Production::GetBatchCountNum()
{
    return m_batchCount;
}

/**
 * @brief Production::GetBatchCounterNum : Return batch count number
 * @return
 */
int Production::GetBatchCounterNum()
{
    return m_BatchCounter;
}

/**
 * @brief Production::GetCycleCountNum : Return cycle count number
 * @return
 */
int Production::GetCycleCountNum()
{
    return m_cycleCount;
}

/**
 * @brief Production::GetBatchSetupEnable : Return batch setup
 * @return
 */
bool Production::GetBatchSetupEnable()
{
    return m_batchSetupEnable;
}

/**
 * @brief Production::SetBatchSetupEnable : Set batch setup
 * @param BatchSetupEnable
 */
void Production::SetBatchSetupEnable(bool BatchSetupEnable)
{
    m_batchSetupEnable = BatchSetupEnable;
    emit OnBatchSetupEnableChanged();
}

/**
 * @brief Production::SetCycleCounttNum : Set cycle count number
 * @param Cyclecount
 */
void Production::SetBatchCounttNum(int Batchcount)
{
    if(m_batchCount!=Batchcount)
    {
        m_batchCount=Batchcount;
        emit OnBatchCountNumChanged();
    }
}

/**
 * @brief Production::SetBatchCounterNum : Set batch count number
 * @param BatchCounter
 */
void Production::SetBatchCounterNum(int BatchCounter)
{
    m_BatchCounter = BatchCounter;
    emit OnBatchCounterNumChanged();
}

/**
 * @brief Production::SetCycleCountNum : Set cycle count number
 * @param CycleCount
 */
void Production::SetCycleCountNum(int CycleCount)
{
    m_cycleCount = CycleCount;
    emit OnCycleCountNumChanged();
}

/**
 * @brief Production::GetGoodByGoalPerc
 * @return
 */
int Production::GetGoodByGoalPerc()
{
    return m_goodByGoalPerc;
}

void Production::SetGoodByGoalPerc(int Goodbygoal)
{
    if(m_goodByGoalPerc!=Goodbygoal)
    {
        m_goodByGoalPerc=Goodbygoal;
        emit OnGoodByGoalPercChanged();
    }
}

/**
 * @brief Production::GetPeakPowerNum
 * @return
 */
int Production::GetPeakPowerNum()
{
    return m_peakPower;
}

void Production::SetPeakPowerNum(int peakPower)
{
    if(m_peakPower!=peakPower)
    {
        m_peakPower=peakPower;
        emit OnPeakPowerNumChanged();
    }
}

/**
 * @brief Production::partidstatusInfo
 * @param sPartIdStatusInfo
 */
void Production::partidstatusInfo(QString sPartIdStatusInfo)
{
    bool bPartIdRequired=Configuration::getInstance()->m_systemData.bPartIDSwitchStatus;
    QString sStatusText;
    if(bPartIdRequired)
    {
        sStatusText = PRODUCTION_PARTID_STATUS;

        if(sPartIdStatusInfo.length() > 0)
            sStatusText = PRODUCTION_PARTID_INFO + sPartIdStatusInfo;

    }
    emit updatePartStatus(sStatusText);
}

/**
 * @brief Production::updateProductionResults
 */
void Production::updateProductionResults()
{
    if(productionPageStatus == true)
    {
        updateProductionScreenResults();
        QMetaObject::invokeMethod(object, "updateProductionResults");
        emit partidstatusInfo("");
    }
}

/**
 * @brief Production::updateProductionPower : update production power status
 * @param powerStatus
 */
void Production::updateProductionPower(QString powerStatus)
{
    int dwMaxPower;
    dwMaxPower = Configuration::getInstance()->m_systemData.PcConfig.PSWatt;

    double dwCurrentPower = powerStatus.toDouble();
    double dwPer = (dwCurrentPower*100)/dwMaxPower;
    m_weldProgressPerc =  round(dwPer);
    m_peakPower = powerStatus.toInt();
    QMetaObject::invokeMethod(object, "updateProductionPowerStatus");
}
