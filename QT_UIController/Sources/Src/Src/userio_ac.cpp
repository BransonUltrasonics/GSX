/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Actuator Controller UI handles UI

***************************************************************************/
#include "Header/userio_ac.h"
#include <QDebug>
extern QQmlContext * context;
extern QQmlApplicationEngine *enginePointer;

/**
 * @brief UserIOAC::UserIOAC :Initializes Actuator Controller User IO
 * @param parent
 */
UserIOAC::UserIOAC(QObject *parent) : QObject(parent),
    m_DigitalInputs(),
    m_DigitalOutputs()
{
    context->setContextProperty(AC_DIGITAL_INPUT_CONTEXT_PROPERTY,&m_DigitalInputs);
    context->setContextProperty(AC_DIGITAL_OUTPUT_CONTEXT_PROPERTY,&m_DigitalOutputs);
}

/**
 * @brief UserIOAC::getDataFromDB : Gets Data From DB
 */
void UserIOAC::getDataFromDB(QStringList a_AcList)
{
    QStringList m_AcDi,m_AcDo;
    int itr;

    //Digital Inputs
    for(itr = 0;itr <= 14; itr++)
    {
        m_AcDi.append(a_AcList[itr]);
    }
    m_DigitalInputs.getDataFromDBUsingIds(m_AcDi);

    //Digital Outputs
    for(itr = 15;itr < 30; itr++)
    {
        m_AcDo.append(a_AcList[itr]);
    }
    m_DigitalOutputs.getDataFromDBUsingIds(m_AcDo);

}

/**
 * @brief UserIOAC::refreshModel : Refresh Model to show the updated values
 */
void UserIOAC::refreshModel()
{
    //Digital Inputs
    m_DigitalInputs.refreshModel();

    //Digital Outputs
    m_DigitalOutputs.refreshModel();
}

/**
 * @brief UserIOAC::getDigitalOutputs : Return the output values
 * @return
 */
const DigitalOutputsAC &UserIOAC::getDigitalOutputs()
{
    return m_DigitalOutputs;
}

/**
 * @brief UserIOAC::getDigitalInputs
 * @return
 */
const DigitalInputsAC &UserIOAC::getDigitalInputs()
{
    return m_DigitalInputs;
}

/**
 * @brief UserIOAC::setUIAfterSaving :set UI After Clicking on Save button
 */
void UserIOAC::setUIAfterSaving()
{

    m_DigitalInputs.setUIAfterSaving();
    m_DigitalOutputs.setUIAfterSaving();
}
