/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

System Info Parameters

***************************************************************************/
#include "Header/systeminfoparameters.h"

/**
 * @brief SystemInfoParameter::SystemInfoParameter :Constructor
 * @param parent
 */
SystemInfoParameter::SystemInfoParameter(QObject *parent):QObject(parent)
{

}

/**
 * @brief SystemInfoParameter::SystemInfoParameter
 * @param name
 * @param content
 * @param parent
 */
SystemInfoParameter::SystemInfoParameter(const QString name, const QString content, QObject *parent) : QObject(parent), m_parameterName(name), m_parameterContent(content)
{

}

/**
 * @brief SystemInfoParameter::getParameterName : Get the Parameter name
 * @return
 */
QString SystemInfoParameter::getParameterName() const
{
    return m_parameterName;
}

/**
 * @brief SystemInfoParameter::setParameterName : Set the parameter name
 * @param value
 */
void SystemInfoParameter::setParameterName(const QString value)
{
    if(m_parameterName != value){
        m_parameterName = value;
        emit parameterNameChanged();
    }
}

/**
 * @brief SystemInfoParameter::getParameterContent : Get the Parameter Content
 * @return
 */
QString SystemInfoParameter::getParameterContent() const
{
    return m_parameterContent;
}

/**
 * @brief SystemInfoParameter::setParameterContent : Set the parameter Content
 * @param value
 */
void SystemInfoParameter::setParameterContent(const QString value)
{
    if(m_parameterContent != value)
    {
        m_parameterContent = value;
        emit parameterContentChanged();
    }
}
