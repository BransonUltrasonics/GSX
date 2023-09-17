/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Pressure Limits

***************************************************************************/

#include "Header/pressureparameter.h"

/**
 * @brief PressureParameter::PressureParameter
 * @param parent
 */
PressureParameter::PressureParameter(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief PressureParameter::PressureParameter
 * @param name
 * @param parent
 */
PressureParameter::PressureParameter(const QString name, QObject *parent)
    : QObject(parent), m_ParameterName(name)
{
}

/**
 * @brief PressureParameter::getParameterName
 * @return
 */
QString PressureParameter::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief PressureParameter::setParameterName
 * @param parameter
 */
void PressureParameter::setParameterName(const QString parameter)
{
    if (parameter != m_ParameterName)
    {
        m_ParameterName = parameter;
        emit parameterNameChanged();
    }
}
