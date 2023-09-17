/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Menu Limits

***************************************************************************/
#include "Header/menulimits.h"

/**
 * @brief MenuLimits::getParameterName
 * @return
 */
QString MenuLimits::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief MenuLimits::setParameterName
 * @param parameter
 */
void MenuLimits::setParameterName(const QString parameter)
{
    if (parameter != m_ParameterName)
    {
        m_ParameterName = parameter;
        emit parameterNameChanged();
    }
}

/**
 * @brief MenuLimits::getParameterLocaleName
 * @return
 */
QString MenuLimits::getParameterLocaleName() const
{
    return m_ParameterLocaleName;
}

/**
 * @brief MenuLimits::setParameterLocaleName
 * @param parameter
 */
void MenuLimits::setParameterLocaleName(const QString parameter)
{
    if (parameter != m_ParameterLocaleName)
    {
        m_ParameterLocaleName = parameter;
        emit parameterLocaleNameChanged();
    }
}

/**
 * @brief MenuLimits::MenuLimits
 * @param parent
 */
MenuLimits::MenuLimits(QObject *parent): QObject(parent)
{
}

/**
 * @brief MenuLimits::MenuLimits
 * @param name
 * @param parent
 */
MenuLimits::MenuLimits(const QString name,const QString Localename, QObject *parent)  : QObject(parent), m_ParameterName(name),m_ParameterLocaleName(Localename)
{
}
