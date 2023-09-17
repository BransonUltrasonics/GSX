/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Last Weld Results

***************************************************************************/
#include "Header/lastweldresults.h"

/**
 * @brief LastWeldResults::LastWeldResults
 * @param parent
 */
LastWeldResults::LastWeldResults(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief LastWeldResults::LastWeldResults
 * @param name
 * @param value
 * @param unit
 * @param parent
 */
LastWeldResults::LastWeldResults(const QString name,const QString value,const QString unit, QObject *parent)
    : QObject(parent), m_ParameterName(name),m_ParameterValue(value),m_ParameterUnit(unit)
{
}

/**
 * @brief LastWeldResults::getParameterName
 * @return
 */
QString LastWeldResults::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief LastWeldResults::setParameterName
 * @param parameter
 */
void LastWeldResults::setParameterName(const QString parameter)
{
    if (parameter != m_ParameterName) {
        m_ParameterName = parameter;
        emit parameterNameChanged();
    }
}

/**
 * @brief LastWeldResults::getParameterValue
 * @return
 */
QString LastWeldResults::getParameterValue() const
{
    return m_ParameterValue;
}

/**
 * @brief LastWeldResults::setParameterValue
 * @param value
 */
void LastWeldResults::setParameterValue(const QString value)
{
    if (value != m_ParameterValue) {
        m_ParameterValue = value;
        emit parameterValueChanged();
    }
}

/**
 * @brief LastWeldResults::getParameterUnit
 * @return
 */
QString LastWeldResults::getParameterUnit() const
{
    return m_ParameterUnit;
}

/**
 * @brief LastWeldResults::setParameterUnit
 * @param unit
 */
void LastWeldResults::setParameterUnit(const QString unit)
{
    if (unit != m_ParameterUnit) {
        m_ParameterUnit = unit;
        emit parameterUnitChanged();
    }
}
