/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power Supply IO handles UI

***************************************************************************/
#include "Header/userio_ps.h"
#include <QDebug>

extern QQmlContext * context;
extern QQmlApplicationEngine *enginePointer;

/**
 * @brief UserIOPS::UserIOPS : Constructor
 * @param parent
 */
UserIOPS::UserIOPS(QObject *parent) : QObject(parent),
    m_DigitalInputs(),
    m_DigitalOutputs(),
    m_AnalogInputs(),
    m_AnalogOutputs()
{
    context->setContextProperty(PS_DIGITAL_INPUT_CONTEXT_PROPERTY,&m_DigitalInputs);
    context->setContextProperty(PS_DIGITAL_OUTPUT_CONTEXT_PROPERTY,&m_DigitalOutputs);
    context->setContextProperty(PS_ANALOG_INPUT_CONTEXT_PROPERTY  ,&m_AnalogInputs);
    context->setContextProperty(PS_ANALOG_OUTPUT_CONTEXT_PROPERTY,&m_AnalogOutputs);
}

/**
 * @brief UserIOPS::getDataFromDB : Getting Digital Inputs and Outputs Data from DB
 */
void UserIOPS::getDataFromDB(QStringList a_PSList)
{
    //Digital Inputs
    QStringList m_PsDi,m_PsDo,m_PsAi,m_PsAo;
    int itr;
    for(itr = 0;itr < 27; itr++)
    {
        m_PsDi.append(a_PSList[itr]);
    }
    m_DigitalInputs.getDataFromDBUsingIds(m_PsDi);
    //Digital Outputs
    for(itr = 27;itr < 51; itr++)
    {
        m_PsDo.append(a_PSList[itr]);
    }
    m_DigitalOutputs.getDataFromDBUsingIds(m_PsDo);
    //Analog Inputs
    for(itr = 51;itr < 57; itr++)
    {
        m_PsAi.append(a_PSList[itr]);
    }
    m_AnalogInputs.getDataFromDBUsingIds(m_PsAi);
    //Analog Outputs
    for(itr = 57;itr < 63; itr++)
    {
        m_PsAo.append(a_PSList[itr]);
    }
    m_AnalogOutputs.getDataFromDBUsingIds(m_PsAo);

}

/**
 * @brief UserIOPS::refreshModel : Refresh the Model for Updated values to reflect the changes
 */
void UserIOPS::refreshModel()
{
    //Digital Inputs
    m_DigitalInputs.refreshModel();
    //Digital Outputs
    m_DigitalOutputs.refreshModel();
    //Analog Inputs
    m_AnalogInputs.refreshModel();
    //Analog Outputs
    m_AnalogOutputs.refreshModel();
}

/**
 * @brief UserIOPS::getAnalogOutputs : Return Analog Outputs
 * @return
 */
const AnalogOutputsPS &UserIOPS::getAnalogOutputs()
{
    return m_AnalogOutputs;
}

/**
 * @brief UserIOPS::getAnalogInputs : Return Analog Inputs
 * @return
 */
const AnalogInputsPS &UserIOPS::getAnalogInputs()
{
    return m_AnalogInputs;
}

/**
 * @brief UserIOPS::getDigitalOutputs : Return Digital Outputs
 * @return
 */
const DigitalOutputsPS &UserIOPS::getDigitalOutputs()
{
    return m_DigitalOutputs;
}

/**
 * @brief UserIOPS::getDigitalInputs : Return Digital Inputs
 * @return
 */
const DigitalInputsPS &UserIOPS::getDigitalInputs()
{
    return m_DigitalInputs;
}

/**
 * @brief UserIOPS::setUIAfterSaving : Refresh The Information After saving the Data
 */
void UserIOPS::setUIAfterSaving()
{
    m_DigitalInputs.setUIAfterSaving();
    m_DigitalOutputs.setUIAfterSaving();
    m_AnalogInputs.setUIAfterSaving();
    m_AnalogOutputs.setUIAfterSaving();
}
