/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Weld Mode Model

***************************************************************************/
#include "Header/modelweldmodes.h"

/**
 * @brief WeldModes::WeldModes
 * @param parent
 */
WeldModes::WeldModes(QObject *parent)
    : QObject(parent)
{

}
/**
 * @brief WeldModes::WeldModes
 * @param name
 * @param parent
 */
WeldModes::WeldModes(const QString name, QObject *parent)
    : QObject(parent), m_ParameterName(name)
{
}

/**
 * @brief WeldModes::getParameterName
 * @return
 */
QString WeldModes::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief WeldModes::setParameterName : Set parameter name and emit the signal to notify
 * @param parameter
 */
void WeldModes::setParameterName(const QString parameter)
{
    if (parameter != m_ParameterName)
    {
        m_ParameterName = parameter;
        emit parameterNameChanged();
    }
}

/**
 * @brief WeldModes::getParameterLocaleName
 * @return
 */
QString WeldModes::getParameterLocaleName() const
{
    return m_ParameterLocaleName;
}

/**
 * @brief WeldModes::setParameterLocaleName : Set parameter locale name and emit the signal to notify
 * @param parameter
 */
void WeldModes::setParameterLocaleName(const QString parameter)
{
    if (parameter != m_ParameterLocaleName)
    {
        m_ParameterLocaleName = parameter;
        emit parameterLocaleNameChanged();
    }
}
