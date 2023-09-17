/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Recipe Information

***************************************************************************/
#include <QDir>
#include <QList>
#include <QQmlContext>
#include "Header/communicationinterface.h"
#include "Header/common.h"
#include "Header/configuration.h"
#include "Header/RecipeInformation.h"

#include "Header/utility.h"
extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern Recipe *recipe;
extern int dwCurrentUnit;

/**
 * @brief GetRecipeInfo : Recipe number to BL
 * @param qStrRecipeNumber
 * @return
 */
QString GetRecipeInfo(QString qStrRecipeNumber)
{
    CIObj.sendMessage(REQ_GET_RECIPE_INFO,RES_GET_RECIPE_INFO,qStrRecipeNumber);
    QString recipeInfo;
    CIObj.recivedMessage(RES_GET_RECIPE_INFO,recipeInfo);
    return recipeInfo;
}

/**
 * @brief RecipeInformation::RecipeInformation
 * @param parent
 */
RecipeInformation::RecipeInformation(QObject *parent) : QObject(parent)
{
}

/**
 * @brief RecipeInformation::InitializationOfParams : Default values for recipe information
 */
void RecipeInformation::InitializationOfParams()
{
    m_recipeDescription = "NA";
    m_productionInstruction = "NA";
    m_Company = "NA";
    m_weldMode = "NA";
    m_controlValue = "NA";
    m_controlValueUnit = "NA";
}

/**
 * @brief RecipeInformation::recipeInfoInit : Detail recipe information panel
 * @param qStrRecipeNumber
 * @param qStrRecipeName
 */
void RecipeInformation::recipeInfoInit(QString qStrRecipeNumber,QString qStrRecipeName)
{
    m_infoRecipeName = qStrRecipeName;
    m_infoRecipeNumber = qStrRecipeNumber;
    InitializationOfParams();
    QString m_recipeInfo = GetRecipeInfo(qStrRecipeNumber);
    if ( !m_recipeInfo.isEmpty() )
    {
        QStringList m_recipeInfoBuffer = m_recipeInfo.split(",");
        if((m_recipeInfoBuffer.size())%5 == 0)
        {
            FillRecipeInfo(m_recipeInfoBuffer);
        }
        else
        {
            InitializationOfParams();
        }

        m_recipeInfo.clear();
    }
    else
    {
        InitializationOfParams();
    }
}

/**
 * @brief RecipeInformation::FillRecipeInfo : Assign Units based of modes
 * @param m_recipeInfoBuffer
 */
void RecipeInformation::FillRecipeInfo(QStringList m_recipeInfoBuffer)
{
    double dTime;
    m_recipeDescription = m_recipeInfoBuffer[0];
    m_productionInstruction = m_recipeInfoBuffer[1];
    m_Company = m_recipeInfoBuffer[2];
    int dwUnit = dwCurrentUnit;
    QString distanceUnit = "mm";
    if(dwUnit)
        distanceUnit = "in";
    if(recipe->getModifiedStatus(m_infoRecipeNumber))
    {
        stModifiedRecipeInfo stTemp = recipe->getCurrentRecipeInfo();
        m_weldMode = stTemp.qstrModeNameScreen;
        m_controlValueUnit = stTemp.qstrUnit;
        m_controlValue = stTemp.qstrModeValue;
    }
    else
    {
        m_weldMode = m_recipeInfoBuffer[3];
        if (m_weldMode == WELDMODE_TIME)
		{
            m_controlValueUnit = CUST_NAME_UINTSEC;
            m_weldMode = CUST_NAME_TIME;
        }
        else if (m_weldMode == WELDMODE_ENERGY)
		{
            m_weldMode = CUST_NAME_ENERGY;
            m_controlValueUnit = CUST_NAME_UINTJOULES;
        }
        else if (m_weldMode == WELDMODE_PEAK_POWER)
		{
            m_weldMode = CUST_NAME_PEAK;
            m_controlValueUnit = CUST_NAME_UINTWATTS;
        }
        else if (m_weldMode == WELDMODE_GROUND_DETECT)
		{
            m_weldMode = CUST_NAME_SCRUB;
            m_controlValueUnit = CUST_NAME_UINTSEC;
        }
        else if (m_weldMode == WELDMODE_ABS_DISTANCE)
		{
            m_weldMode="Absolute Distance";
            m_controlValueUnit = distanceUnit;
        }
        else if (m_weldMode == WELDMODE_COL_DISTANCE)
		{
            m_weldMode="Collapse Distance";
            m_controlValueUnit = distanceUnit;
        }
        else if (m_weldMode == WELDMODE_DYNAMIC)
       {
           m_weldMode=CUST_NAME_DYNAMIC;
           m_controlValueUnit = PERCENTAGE;
       }

        if (m_controlValueUnit == CUST_NAME_UINTSEC)
        {
            dTime = Configuration::getInstance()->convertMsecToSec(m_recipeInfoBuffer[4].toDouble());
            m_controlValue.setNum(dTime,'f',3);
        }
        else if ( m_controlValueUnit == CUST_NAME_UINTMilliMeter)
        {
            dTime = Configuration::getInstance()->convertMicrometreToMillimetre(m_recipeInfoBuffer[4].toDouble());
            /*For Collpse Distance and Absolute Distance default value precision in Recipe Information popup window*/
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
            dTime = Configuration::getInstance()->convertmicrometretoinches(m_recipeInfoBuffer[4].toDouble());
            m_controlValue.setNum(dTime,'f',4);
        }
        else if( m_controlValueUnit == CUST_NAME_UINTJOULES)
        {
                dTime = Utility::convertMilliJoulesToJoules(m_recipeInfoBuffer[4].toDouble());
                m_controlValue.setNum(dTime,'f',1);
        }
        else
        {
            m_controlValue = m_recipeInfoBuffer[4];
        }
    }
}

/**
 * @brief RecipeInformation::getInfoRecipeName : Return the recipe name
 * @return
 */
QString RecipeInformation::getInfoRecipeName()
{
    return m_infoRecipeName;
}

/**
 * @brief RecipeInformation::getRecipeDescription : Return the recipe description
 * @return
 */
QString RecipeInformation::getRecipeDescription()
{
    return m_recipeDescription;
}

/**
 * @brief RecipeInformation::getProductionInstruction : Return the production Information
 * @return
 */
QString RecipeInformation::getProductionInstruction()
{
    return m_productionInstruction;
}

/**
 * @brief RecipeInformation::getCompany
 * @return
 */
QString RecipeInformation::getCompany()
{
    return m_Company;
}

/**
 * @brief RecipeInformation::getControlmode : Return the control mode
 * @return
 */
QString RecipeInformation::getControlmode()
{
    return m_weldMode;
}

/**
 * @brief RecipeInformation::getControlmodeValue
 * @return
 */
QString RecipeInformation::getControlmodeValue()
{
    return m_controlValue;
}

/**
 * @brief RecipeInformation::getControlModeUnit
 * @return
 */
QString RecipeInformation::getControlModeUnit()
{
    return m_controlValueUnit;
}
