/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Horn Down Source File

***************************************************************************/
#include <iostream>
using namespace  std;

#include "Header/horndown.h"
#include <QQmlContext>
#include "Header/configuration.h"
#include "Header/communicationinterface.h"
#include "Header/sysconfigdata.h"
#include "Header/configuration.h"
#include "Header/recipe.h"
#include "Header/utility.h"
#include <QDebug>

extern QQmlContext *context;
extern unsigned int psFreqGlobalVar;
extern QObject * object;

extern CommunicationInterface CIObj;
extern Configuration configobj;
extern Recipe *recipe;
extern int dwCurrentUnit;
#define HORN_FORCE                      "Force"
#define HORN_READY_OFFSET               "Ready Offset"
#define HORN_EXPECTED_PART_CONTACT      "Expected Part Contact"
#define HORN_EXPECTED_OFFSET            "Expected Contact Offset"
#define HORN_EXPECTED_WINDOW            "Expected Contact Window"
#define HORN_DOWN_ACC                   "Down Acceleration"
#define HORN_DOWN_MAX                   "Down Max Velocity"
#define HORN_DOWN_DE_ACC                "Down De-acceleration"
#define HORN_RETURN_ACC                 "Return Acceleration"
#define HORN_RETURN_MAX                 "Return Max Velocity"
#define HORN_RETURN_DE_ACC              "Return De-acceleration"

#define HORN_PART_MSG_SUCCESS           QObject::tr("Find Part Contact is successful")
#define HORN_JOG_UP_SUCCESS             QObject::tr("Jog Up is successful")
#define HORN_JOG_DOWN_SUCCESS           QObject::tr("Jog Down is successful")
#define HORN_JOG_READY_SUCCESS          QObject::tr("Jog Ready Position is successful")
#define HORN_DOWN_SUCCESS               QObject::tr("Horn Down is successful")
#define HORN_RETRACT_SUCCESS            QObject::tr("Horn Retract is successful")
#define HORN_RETRACT_WARNING            QObject::tr("CAUTION: Clear tool before retracting");

#define HORN_PART_MSG_FAILURE           QObject::tr("Find Part Contact Failed")
#define HORN_JOG_UP_FAILURE             QObject::tr("Jog Up Failed")
#define HORN_JOG_DOWN_FAILURE           QObject::tr("Jog Down Failed")
#define HORN_JOG_READY_FAILURE          QObject::tr("Jog Ready Position Failed")
#define HORN_DOWN_FAILURE               QObject::tr("Horn Down Failed")
#define HORN_RETRACT_FAILURE            QObject::tr("Horn Retract Failed")

/**
 * @brief HornDown::HornDown
 * @param parent
 * comments: constructor called and List model and Map defined of related to Actuator setup
 */
HornDown::HornDown(QObject *parent): QObject(parent)
{
    m_StrokeLength  = "125000";
    m_ActuatorData.PrePartContact = 0;
    setactualPPC(QString::number(m_ActuatorData.PrePartContact));
    m_ActuatorData.PrePartContactVelocity = 0;
    setpartContactVelocity(QString::number(m_ActuatorData.PrePartContactVelocity));
    m_ActuatorData.ActualDistance = 0;
    sethornPointer(QString::number(m_ActuatorData.ActualDistance));
    m_ActuatorData.AbsolutePosition = 0;
    setabsolutePosition(QString::number(m_ActuatorData.AbsolutePosition));
    m_ActuatorData.ActualForce = 0;
    setactualForce(QString::number(m_ActuatorData.ActualForce));
    m_ActuatorData.ExcpetedPartContact = 0;
}

/**
 * @brief HornDown::resetValues : Reset the store values into structure
 */
void HornDown::resetValues()
{
    m_ActuatorData.AbsolutePosition = 0;
    setabsolutePosition(QString::number(m_ActuatorData.AbsolutePosition));
    m_ActuatorData.PrePartContact = 0;
    setactualPPC(QString::number(m_ActuatorData.PrePartContact));
    m_ActuatorData.PrePartContactVelocity = 0;
    setpartContactVelocity(QString::number(m_ActuatorData.PrePartContactVelocity));
    m_ActuatorData.ActualForce = 0;
    setactualForce(QString::number(m_ActuatorData.ActualForce));
}

/**
 * @brief HornDown::UpdateStructureFromBL : Update the structure value received from BL
 * @param a_structdata
 */
void HornDown::UpdateStructureFromBL(QString a_structdata)
{
    double dwEPC,dwHornPosition;
    QStringList a_structdatalist = a_structdata.split(",");
    if(dwCurrentUnit)
    {
        m_ActuatorData.AbsolutePosition = a_structdatalist.at(0).toDouble()/(double)25400;
        dwEPC = a_structdatalist.at(3).toDouble()/(double)25400;
        if(!dwEPC)
            dwEPC = m_ActuatorData.ExcpetedPartContact;
        else
            m_ActuatorData.ExcpetedPartContact = dwEPC;
        dwHornPosition = a_structdatalist.at(1).toDouble()/(double)25400;
        m_ActuatorData.PrePartContact = a_structdatalist.at(4).toDouble()/25400;
        m_ActuatorData.ActualForce = a_structdatalist.at(2).toDouble()/(double)4.44822;
        m_ActuatorData.PrePartContactVelocity = a_structdatalist.at(5).toDouble();
    }
    else
    {
        m_ActuatorData.AbsolutePosition = a_structdatalist.at(0).toDouble()/1000;
        dwEPC = a_structdatalist.at(3).toDouble()/(double)1000;
        if(!dwEPC)
            dwEPC = m_ActuatorData.ExcpetedPartContact;
        else
            m_ActuatorData.ExcpetedPartContact = dwEPC;
        dwHornPosition = a_structdatalist.at(1).toDouble()/(double)1000;
        m_ActuatorData.PrePartContact = a_structdatalist.at(4).toDouble()/1000;
        m_ActuatorData.ActualForce = a_structdatalist.at(2).toDouble();
        m_ActuatorData.PrePartContactVelocity = a_structdatalist.at(5).toDouble()/(double)25400;
    }

    m_ActuatorData.ActualDistance = dwHornPosition;
    m_ActuatorData.ResponseStatus = a_structdatalist.at(6).toDouble();
    m_ActuatorData.ResponseType = a_structdatalist.at(7).toDouble();
    recipe->setParamaterChangedFlag(1);
    if(m_ActuatorData.ResponseType==PART_CONTACT)
    {
        double dEPC = recipe->getStructureValueByName("expected part contact").toDouble();
        recipe->setStructureValue("expected part contact",QString::number(dwEPC),true);
        if(dEPC == 0)
            recipe->setStructureValue("absolute distance cutoff",QString::number(recipe->getStructureValueByName("expected part contact").toDouble() + recipe->getMaxValue("collapse distance").toDouble()));
        QString a_WeldForce = recipe->getActuatorRecipeForceStructureValue();
        recipe->setActuatorRecipeValue(a_WeldForce,true);
    }

    if(m_ActuatorData.ResponseStatus==1)
    {
        updateEPCDataFromBL();
        setppcPosition(QString::number(m_ActuatorData.PrePartContact));
        fillActiveRecipeForActuator(0);
        sethornPointer(QString::number(a_structdatalist.at(1).toDouble()));
        setactualPPC(QString::number(m_ActuatorData.PrePartContact));
        setabsolutePosition(QString::number(m_ActuatorData.AbsolutePosition));
        setpartContactVelocity(QString::number(m_ActuatorData.PrePartContactVelocity));
        setactualForce(QString::number(m_ActuatorData.ActualForce));

        successMessageStatusOfRequest(m_ActuatorData.ResponseType);
    }
    else
    {
        messageStatusOfRequest(m_ActuatorData.ResponseType);
    }
    updateHornPositionValues();
}
int HornDown::getResponseType()
{
    return static_cast<int>(m_ActuatorData.ResponseType);
}

/**
 * @brief HornDown::successMessageStatusOfRequest : Horn down success status message
 * @param dwType
 */
void HornDown::successMessageStatusOfRequest(int dwType)
{
    QString findcontactSuccessMessage=HORN_PART_MSG_SUCCESS;
    QString JogUpSuccessMessage=HORN_JOG_UP_SUCCESS;
    QString JogDownSuccessMessage=HORN_JOG_DOWN_SUCCESS;
    QString JogReadyPositionSuccessMessage=HORN_JOG_READY_SUCCESS;
    QString HornDownSuccessMessage=HORN_DOWN_SUCCESS;
    QString HornRetractSuccessMessage=HORN_RETRACT_SUCCESS;
    QString HornRetractWarningMessage=HORN_RETRACT_WARNING;

    switch (dwType)
    {
    case PART_CONTACT:
        updateHornDownStatusText(findcontactSuccessMessage);
        break;
    case JOG_UP:
        updateHornDownStatusText(JogUpSuccessMessage);
        break;
    case JOG_DOWN:
        updateHornDownStatusText(JogDownSuccessMessage);
        break;
    case JOG_READY_POSITION:
        updateHornDownStatusText(JogReadyPositionSuccessMessage);
        break;
    case HORN_DOWN_CLAMP_ON:
         updateHornDownStatusText(HornRetractWarningMessage);
         break;
    case HORN_DOWN_CLAMP_OFF:
        updateHornDownStatusText(HornDownSuccessMessage);
        break;
    case HORN_RETRACT:
        updateHornDownStatusText(HornRetractSuccessMessage);
        break;
    default:
        break;
    }
}

/**
 * @brief HornDown::messageStatusOfRequest : Horn down failure status message
 * @param dwType
 */
void HornDown::messageStatusOfRequest(int dwType)
{
    QString findcontactFailMessage=HORN_PART_MSG_FAILURE;
    QString JogUpFailMessage=HORN_JOG_UP_FAILURE;
    QString JogDownFailMessage=HORN_JOG_DOWN_FAILURE;
    QString JogReadyPositionFailMessage=HORN_JOG_READY_FAILURE;
    QString HornDownFailMessage=HORN_DOWN_FAILURE;
    QString HornRetractFailMessage=HORN_RETRACT_FAILURE;
    switch (dwType)
    {
    case PART_CONTACT:
        updateHornDownStatusText(findcontactFailMessage);
        break;
    case JOG_UP:
        updateHornDownStatusText(JogUpFailMessage);
        break;
    case JOG_DOWN:
        updateHornDownStatusText(JogDownFailMessage);
        break;
    case JOG_READY_POSITION:
        updateHornDownStatusText(JogReadyPositionFailMessage);
        break;
    case HORN_DOWN_CLAMP_ON:
    case HORN_DOWN_CLAMP_OFF:
        updateHornDownStatusText(HornDownFailMessage);
        break;
    case HORN_RETRACT:
        updateHornDownStatusText(HornRetractFailMessage);
        break;
    default:
        break;
    }
}

/**
 * @brief HornDown::updateActuatorList
 * @param a_index
 */
void HornDown::updateActuatorList(int a_index)
{
    QString qstrTempString = "";
    switch(a_index)
    {
    case 1:
        qstrTempString = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName(HORN_READY_OFFSET).toDouble()))));
        setreadyposition(qstrTempString);
        break;
    case 2:
        setepcPosition(recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT));
        qstrTempString = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName(HORN_READY_OFFSET).toDouble()))));
        setreadyposition(qstrTempString);
        setppcPosition(QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-(recipe->getStructureValueByName("Expected Contact Offset").toDouble()))));
        setwpPosition(QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName("part contact window minus").toDouble())))));
        setmpPosition(QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())+((recipe->getStructureValueByName("part contact window plus").toDouble())))));
        break;
    case 3:
        setppcPosition(QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-(recipe->getStructureValueByName("Expected Contact Offset").toDouble()))));
        break;
    case 4:
        setwpPosition(QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName("part contact window minus").toDouble())))));
        setmpPosition(QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())+((recipe->getStructureValueByName("part contact window plus").toDouble())))));
        break;
    default:
        break;
    }
}

/**
 * @brief HornDown::findPartContactRequestToBL
 * @purpose: this will trigger and send request to BL,when click on Find Part contact.
 */
void HornDown::findPartContactRequestToBL()
{
    char requestMessage=static_cast<char>(PART_CONTACT);
    CIObj.sendMessage(SCBL_SETUP_READ_REQ,SCBL_SETUP_RES, &requestMessage, 1);

}

/**
 * @brief HornDown::updateEPCDataFromBL
 * @param a_distance
 * @purpose:Response Recived by signal and this function will worked as slot to update EPC.
 */
void HornDown::updateEPCDataFromBL()
{
    QString qstrTempString = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName(HORN_READY_OFFSET).toDouble()))));
    setreadyposition(qstrTempString);
    setepcPosition(recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT));
}

/**
 * @brief HornDown::jogUpRequestToBL
 * @purpose:this will trigger and send request to BL,when click on Jog Up Image.
 */
void HornDown::jogUpRequestToBL()
{
    char requestMessage=static_cast<char>(JOG_UP);
    CIObj.sendMessage(SCBL_SETUP_READ_REQ,SCBL_SETUP_RES,&requestMessage,1);
}

/**
 * @brief HornDown::jogDownRequestToBL
 * @purpose:this will trigger and send request to BL,when click on Jog Down Image.
 */
void HornDown::jogDownRequestToBL()
{
    char requestMessage=static_cast<char>(JOG_DOWN);
    CIObj.sendMessage(SCBL_SETUP_READ_REQ,SCBL_SETUP_RES,&requestMessage,1);
}

/**
 * @brief HornDown::setMyReadyPositionRequestToBL
 * @purpose:this will trigger and send request to BL,when click on Set My ready Position.
 */
void HornDown::setMyReadyPositionRequestToBL(QString qstrValue)
{
    double dwReadyOffset = recipe->getStructureValueByName("expected part contact").toDouble() - qstrValue.toDouble();
    recipe->setParamaterChangedFlag(1);
    recipe->setStructureValue("ready offset",QString::number(dwReadyOffset));
    QString qstrTempVal = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName(HORN_READY_OFFSET).toDouble()))));
    setreadyposition(qstrTempVal);
    //    char requestMessage=(char)JOG_READY_POSITION;
    //    CIObj.sendMessage(SCBL_SETUP_READ_REQ,SCBL_SETUP_RES,&requestMessage,1);
}

/**
 * @brief HornDown::horndownRequestToBL
 * @purpose:this will trigger and send request to BL,when click on Horn down Radio Button.
 */
void HornDown::horndownRequestToBL(bool isHornClamp)
{
    char requestMessage=static_cast<char>(HORN_DOWN_CLAMP_OFF);
    if(isHornClamp)
        requestMessage=static_cast<char>(HORN_DOWN_CLAMP_ON);

    CIObj.sendMessage(SCBL_SETUP_READ_REQ,SCBL_SETUP_RES,&requestMessage,1);
}

/**
 * @brief HornDown::retractRequestToBL
 * @purpose:this will trigger and send request to BL,when click on Retract Button.
 */
void HornDown::retractRequestToBL()
{
    char requestMessage=static_cast<char>(HORN_RETRACT);
    CIObj.sendMessage(SCBL_SETUP_READ_REQ,SCBL_SETUP_RES,&requestMessage,1);
}

/**
 * @brief HornDown::getActiveRecipeForActuator
 * @return Qstring
 * @purpose:this will provide Qstring data from DB,which is only Active Recipe.
 */
QString HornDown::getActiveRecipeForActuator()
{
    CIObj.sendMessage(REQ_GET_ACTIVE_RECIPE_FOR_ACTUATOR, RES_GET_ACTIVE_RECIPE_FOR_ACTUATOR,"");
    QString a_activeRecipeBuffer = "";
    CIObj.recivedMessage(RES_GET_ACTIVE_RECIPE_FOR_ACTUATOR,a_activeRecipeBuffer);
    return a_activeRecipeBuffer;
}

/**
 * @brief HornDown::fillActiveRecipeForActuator
 * @purpose:this will Fill the QstringList when Recipe data is recived from this function getActiveRecipeForActuator().
 */
void HornDown::fillActiveRecipeForActuator(int dwFlag)
{
    m_StrokeLength = QString::number(Configuration::getInstance()->m_systemData.ActConfig.MaxStrokelength);
    QString qstrVal="";
    m_actuatorRecipeBuffer.clear();
    QString recipeDetails = getActiveRecipeForActuator();
    if(!recipeDetails.isEmpty())
    {
        QStringList recipeDetailsList = recipeDetails.split(",");
        m_actuatorRecipeBuffer.append(recipeDetailsList[0]);
        m_actuatorRecipeBuffer.append(recipeDetailsList[1]);
    }
    else
    {
        m_actuatorRecipeBuffer.append("NA");
        m_actuatorRecipeBuffer.append("NA");
    }
    recipe->loadActatorSetupParamsModel();
    recipe->loadActatorSetupAdvancedParamsModel();
    qstrVal = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName(HORN_READY_OFFSET).toDouble()))));
    setreadyposition(qstrVal);
    qstrVal = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-((recipe->getStructureValueByName("part contact window minus").toDouble()))));
    setwpPosition(qstrVal);
    qstrVal = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())-(recipe->getStructureValueByName("Expected Contact Offset").toDouble())));
    setppcPosition(qstrVal);
    qstrVal = recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT);
    setepcPosition(qstrVal);
    m_ActuatorData.ExcpetedPartContact = qstrVal.toDouble();
    qstrVal = QString::number(((recipe->getStructureValueByName(HORN_EXPECTED_PART_CONTACT).toDouble())+((recipe->getStructureValueByName("part contact window plus").toDouble()))));
    setmpPosition(qstrVal);

    if(dwFlag)
    {
        m_ActuatorData.AbsolutePosition = 0;
        m_ActuatorData.PrePartContact = 0;
        m_ActuatorData.PrePartContactVelocity = 0;
        m_ActuatorData.ActualForce = 0;
        double dVal = 0;
        if(dwCurrentUnit)
            m_Hornpointer.setNum(dVal,'f',4);
        else
        {
            /*Precision handling for horn's Home position*/
            if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
            {
                m_Hornpointer.setNum(dVal,'f',2);
            }

            else
            {
                m_Hornpointer.setNum(dVal,'f',3);
            }
        }

        emit hornPointerChanged();
    }
    setactualPPC(QString::number(m_ActuatorData.PrePartContact));
    setabsolutePosition(QString::number(m_ActuatorData.AbsolutePosition));
    setpartContactVelocity(QString::number(m_ActuatorData.PrePartContactVelocity));
    setactualForce(QString::number(m_ActuatorData.ActualForce));
}

/**
 * @brief HornDown::getRecipeNumberForActuator
 * @return
 * @purpose:this will give the Recipe number if exist.
 */
QString HornDown::getRecipeNumberForActuator()
{
    if(!m_actuatorRecipeBuffer.empty())
        return m_actuatorRecipeBuffer[0];
    else
        return "NA";
}

/**
 * @brief HornDown::getRecipeNameForActuator
 * @return
 * @purpose:this will give the Recipe name if exist.
 */
QString HornDown::getRecipeNameForActuator()
{
    if(!m_actuatorRecipeBuffer.empty())
        return m_actuatorRecipeBuffer[1];
    else
        return "NA";
}

/**
 * @brief HornDown::toPixel
 * @param a_valueInMetric
 * @return
 * @purpose:this will convert mm to Pixel which is used by UI.
 */
QString HornDown::toPixel(QString a_valueInMetric)
{
    double temp_a_valueInMetric = (a_valueInMetric.toDouble());
    double dActualPosition ;
    QString m_tempStrokeLen = m_StrokeLength;
    if(dwCurrentUnit)
        m_tempStrokeLen = QString::number(Configuration::getInstance()->convertMillimeterToInches(m_StrokeLength.toDouble()));

    dActualPosition = (temp_a_valueInMetric*m_StrokeLengthInPixel.toDouble())/m_tempStrokeLen.toDouble();
    QString qstrTemp = QString::number(dActualPosition);
    return qstrTemp;
}

/**
 * @brief HornDown::getreadyposition
 * @return
 * @purpose:this will give Ready position of Horn.
 */
QString HornDown::getreadyposition() const
{
    return m_ReadyPosition;
}

/**
 * @brief HornDown::setreadyposition
 * @param name
 * @purpose:this will set Ready position of Horn.
 */
void HornDown::setreadyposition(const QString name)
{
    m_ReadyPosition = name;
    double dVal = m_ReadyPosition.toDouble();
    if(dVal<0)
        dVal = 0;
    else if(dVal > m_StrokeLength.toDouble())
        dVal = m_StrokeLength.toDouble();
    if(dwCurrentUnit)
    {
        m_ReadyPosition.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for horn's Ready position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_ReadyPosition.setNum(dVal,'f',2);
        }

        else
        {
            m_ReadyPosition.setNum(dVal,'f',3);
        }
    }

    emit readypositionChanged();
}

/**
 * @brief HornDown::getppcPosition
 * @return
 * @purpose:this will give PPC position of Horn.
 */
QString HornDown::getppcPosition() const
{
    return m_PpcPosition;
}

/**
 * @brief HornDown::setppcPosition
 * @param name
 * @purpose:this will set PPC position of Horn.
 */
void HornDown::setppcPosition(const QString name)
{
    m_PpcPosition = name;
    double dVal = m_PpcPosition.toDouble();
    if(dVal<0)
        dVal = 0;
    else if(dVal > m_StrokeLength.toDouble())
        dVal = m_StrokeLength.toDouble();
    if(dwCurrentUnit)
    {
        // dVal = Configuration::getInstance()->convertMillimeterToInches(m_PpcPosition.toDouble());
        m_PpcPosition.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for horn's PPC position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_PpcPosition.setNum(dVal,'f',2);
        }

        else
        {
            m_PpcPosition.setNum(dVal,'f',3);
        }
    }

    emit ppcPositionChanged();
}

/**
 * @brief HornDown::getepcPosition
 * @return
 * @purpose:this will give EPC position of Horn.
 */
QString HornDown::getepcPosition() const
{
    return m_EpcPosition;
}

/**
 * @brief HornDown::setepcPosition
 * @param name
 * @purpose:this will Set EPC position of Horn.
 */
void HornDown::setepcPosition(const QString name)
{
    m_EpcPosition = name;
    double dVal = m_EpcPosition.toDouble();
    if(dVal<0)
        dVal = 0;
    else if(dVal > m_StrokeLength.toDouble())
        dVal = m_StrokeLength.toDouble();
    if(dwCurrentUnit)
    {
        // dVal = Configuration::getInstance()->convertMillimeterToInches(m_EpcPosition.toDouble());
        m_EpcPosition.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for horn's EPC position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_EpcPosition.setNum(dVal,'f',2);
        }

        else
        {
            m_EpcPosition.setNum(dVal,'f',3);
        }
    }

    emit epcPositionChanged();
}

/**
 * @brief HornDown::getmpPosition
 * @return
 * @purpose:this will give MP position of Horn.
 */
QString HornDown::getmpPosition() const
{
    return m_MpPosition;
}

/**
 * @brief HornDown::setmpPosition
 * @param name
 * @purpose:this will Set MP position of Horn.
 */
void HornDown::setmpPosition(const QString name)
{
    m_MpPosition = name;
    double dVal = m_MpPosition.toDouble();
    if(dVal<0)
        dVal = 0;
    else if(dVal > m_StrokeLength.toDouble())
        dVal = m_StrokeLength.toDouble();
    if(dwCurrentUnit)
    {
        //dVal = Configuration::getInstance()->convertMillimeterToInches(m_MpPosition.toDouble());
        m_MpPosition.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for horn's MP position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_MpPosition.setNum(dVal,'f',2);
        }

        else
        {
            m_MpPosition.setNum(dVal,'f',3);
        }
    }

    emit mpPositionChanged();
}

/**
 * @brief HornDown::getwpPosition
 * @return
 * @purpose:this will give WP position of Horn.
 */
QString HornDown::getwpPosition() const
{
    return m_WpPosition;
}

/**
 * @brief HornDown::setwpPosition
 * @param name
 * @purpose:this will Set Wp position of Horn.
 */
void HornDown::setwpPosition(const QString name)
{
    m_WpPosition = name;
    double dVal = m_WpPosition.toDouble();
    if(dVal<0)
        dVal = 0;
    else if(dVal > m_StrokeLength.toDouble())
        dVal = m_StrokeLength.toDouble();
    if(dwCurrentUnit)
    {
        //dVal = Configuration::getInstance()->convertMillimeterToInches(m_WpPosition.toDouble());
        m_WpPosition.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for horn's WP position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_WpPosition.setNum(dVal,'f',2);
        }

        else
        {
            m_WpPosition.setNum(dVal,'f',3);
        }
    }

    emit wpPositionChanged();
}

/**
 * @brief HornDown::gethornPointer
 * @return
 * @purpose:this will give Horn Pointer(green image) of Horn.
 */
QString HornDown::gethornPointer() const
{
    return m_Hornpointer;
}

/**
 * @brief HornDown::sethornPointer
 * @param name
 * @purpose:this will Set Horn pointer of Horn.
 */
void HornDown::sethornPointer(const QString name)
{
    m_Hornpointer = name;
    double dVal = Configuration::getInstance()->convertMicrometreToMillimetre(m_Hornpointer.toDouble());
    if(dwCurrentUnit)
    {
        dVal = Configuration::getInstance()->convertmicrometretoinches(m_Hornpointer.toDouble());
        m_Hornpointer.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for Horn Pointer position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_Hornpointer.setNum(dVal,'f',2);
        }

        else
        {
            m_Hornpointer.setNum(dVal,'f',3);
        }
    }

    emit hornPointerChanged();
}

/**
 * @brief HornDown::getstrokeLengthInPixel
 * @return
 * @purpose:this will give StrokeLength in pixel of Horn.
 */
QString HornDown::getstrokeLengthInPixel() const
{
    return m_StrokeLengthInPixel;
}

/**
 * @brief HornDown::setstrokeLengthInPixel
 * @param name
 * @purpose:this will set StrokeLength in pixel of Horn.
 */
void HornDown::setstrokeLengthInPixel(const QString name)
{
    m_StrokeLengthInPixel = name;
    emit strokeLengthInPixelChanged();
    emit readypositionChanged();
    emit epcPositionChanged();
    emit ppcPositionChanged();
}

/**
 * @brief HornDown::getabsolutePosition
 * @return
 */
QString HornDown::getabsolutePosition() const
{
    return m_AbsolutePosition;
}

/**
 * @brief HornDown::setabsolutePosition
 * @param name
 */
void HornDown::setabsolutePosition(const QString name)
{
    m_AbsolutePosition = name;
    double dVal;
    dVal = m_AbsolutePosition.toDouble();
    if(dwCurrentUnit)
    {
        //dVal = Configuration::getInstance()->convertMillimeterToInches(m_AbsolutePosition.toDouble());
        m_AbsolutePosition.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for Absolute Position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_AbsolutePosition.setNum(dVal,'f',2);
        }

        else
        {
            m_AbsolutePosition.setNum(dVal,'f',3);
        }
    }

    emit absolutePositionChanged();
}

/**
 * @brief HornDown::getpartContactVelocity
 * @return
 */
QString HornDown::getpartContactVelocity() const
{
    return m_PartContactVelocity;
}

/**
 * @brief HornDown::setpartContactVelocity
 * @param name
 */
void HornDown::setpartContactVelocity(const QString name)
{
    m_PartContactVelocity = name;
    double dVal = m_PartContactVelocity.toDouble();
    if(dwCurrentUnit)
    {
        //        dVal = Configuration::getInstance()->convertMillimeterToInches(m_PartContactVelocity.toDouble());
        m_PartContactVelocity.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for Part Contact Velocity*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_PartContactVelocity.setNum(dVal,'f',2);
        }

        else
        {
            m_PartContactVelocity.setNum(dVal,'f',3);
        }
    }

    emit partContactVelocityChanged();
}

/**
 * @brief HornDown::getactualForce
 * @return
 */
QString HornDown::getactualForce() const
{
    return m_ActualForce;
}

/**
 * @brief HornDown::setactualForce
 * @param name
 */
void HornDown::setactualForce(const QString name)
{
    if(dwCurrentUnit)
    {
        /*To display one decimal value for imperial Unit*/
        m_ActualForce = Utility::convertNtolbs(name);
        m_ActualForce.setNum(m_ActualForce.toDouble(),'f',1);
    }
    else
    {
        m_ActualForce = name;
    }
    emit actualForceChanged();
}

/**
 * @brief HornDown::getactualPPC
 * @return
 */
QString HornDown::getactualPPC() const
{
    return m_PrePartContact;
}

/**
 * @brief HornDown::setactualPPC
 * @param name
 */
void HornDown::setactualPPC(const QString name)
{
    m_PrePartContact = name;
    double dVal = m_PrePartContact.toDouble();
    if(dwCurrentUnit)
    {
        // dVal = Configuration::getInstance()->convertMillimeterToInches(m_PrePartContact.toDouble());
        m_PrePartContact.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for horn's PPC position*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            m_PrePartContact.setNum(dVal,'f',2);
        }

        else
        {
            m_PrePartContact.setNum(dVal,'f',3);
        }
    }

    emit actualPPCChanged();
}

//QString HornDown::getactualPPC()
//{
//    return QString::number(m_ActuatorData.PrePartContact);
//}

/**
 * @brief HornDown::RefreshAllData
 */
void HornDown::RefreshAllData()
{
    emit strokeLengthInPixelChanged();
    emit readypositionChanged();
    emit epcPositionChanged();
    emit ppcPositionChanged();
    emit mpPositionChanged();
    emit wpPositionChanged();
    emit absolutePositionChanged();
    emit partContactVelocityChanged();
    emit actualForceChanged();
}

/**
 * @brief HornDown::splitdata
 * @param a_commonString
 * @param index
 * @return
 */
QString HornDown::splitdata(QString a_commonString,int index)
{
    if(a_commonString.contains(":"))
    {
        QStringList stringSplit=a_commonString.split(":");
        return stringSplit[index];
    }
    return "";
}

/**
 * @brief HornDown::getStrokeLengthFormConfig
 * @return
 */
QString HornDown::getStrokeLengthFormConfig()
{
    double dVal;
    QString retVal;
    retVal = QString::number(Configuration::getInstance()->m_systemData.ActConfig.MaxStrokelength);
    dVal = retVal.toDouble();
    if(dwCurrentUnit)
    {
        dVal = Configuration::getInstance()->convertMillimeterToInches(retVal.toDouble());
        retVal.setNum(dVal,'f',4);
    }
    else
    {
        /*Precision handling for Stoke Length*/
        if(Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            retVal.setNum(dVal,'f',2);
        }

        else
        {
            retVal.setNum(dVal,'f',3);
        }
    }

    return retVal;
}

/**
 * @brief HornDown::getUnitsType
 * @return
 */
int HornDown::getUnitsType()
{
    int dwUnit = dwCurrentUnit;
    return dwUnit;
}

/**
 * @brief HornDown::startOrStopWeld
 */
void HornDown::startOrStopWeld(int subID)
{
    char buff;
    if(subID == 0)
        buff = static_cast<char>(NO);
    else if(subID == 1)
        buff = static_cast<char>(WELD);
    CIObj.sendMessage(SCBL_SET_NEXT_OPERATION_REQ,SCBL_SET_NEXT_OPERATION_REQ,&buff,1);
}

/**
 * @brief HornDown::setHornDownRememberModes
 * @param bRFPC
 * @param bRJOG
 * @param bRHORNDOWN
 * @param bFPCStatus
 * @param bJOGStatus
 * @param bHORNDOWNStatus
 * @param bUpFlag
 * @param bDownFlag
 */
void HornDown::setHornDownRememberModes(bool bRFPC, bool bRJOG, bool bRHORNDOWN, bool bFPCStatus, bool bJOGStatus, bool bHORNDOWNStatus, bool bUpFlag, bool bDownFlag)
{
    QString strToStore;
    strToStore = QString::number(bRFPC) + "," + QString::number(bRJOG) + "," +
            QString::number(bRHORNDOWN) + "," + QString::number(bFPCStatus) + "," +
            QString::number(bJOGStatus) + "," +
            QString::number(bHORNDOWNStatus) + "," +
            QString::number(bUpFlag) + "," +
            QString::number(bDownFlag);
    Configuration::getInstance()->setConigValues("HornDownModes", "HornDownModesStatus", strToStore);
}

/**
 * @brief HornDown::getHornDownRememberModes
 * @return
 */
QString HornDown::getHornDownRememberModes()
{
    QString strModes = Configuration::getInstance()->getConfigValues("HornDownModes", "HornDownModesStatus");
    if(strModes.isEmpty())
    {
        strModes = "1,0,0,1,0,0,1,0";
    }
    return strModes;
}

void HornDown::updateHornPositionValues()
{
    QMetaObject::invokeMethod(object,"updateHornPositions");
    emit updateHornPositions();
}
