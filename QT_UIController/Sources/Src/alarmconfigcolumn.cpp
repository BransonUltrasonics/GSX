/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to set and get Alarm configuration status from UI

***************************************************************************/
#include "Header/alarmconfigcolumn.h"

/**
 * @brief Alarmconfigcolumn::getParameterName : Returns the alarm options name
 * @return : QString
 */
QString Alarmconfigcolumn::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief Alarmconfigcolumn::setParameterName : Sets the alarm options name
 * @param : QString parameter
 */
void Alarmconfigcolumn::setParameterName(const QString parameter)
{
    if (parameter != m_ParameterName)
    {
        m_ParameterName = parameter;
        emit parameterNameChanged();
    }
}

/**
 * @brief Alarmconfigcolumn::getRowName : Returns alarm parameter row name
 * @return : QString
 */
QString Alarmconfigcolumn::getRowName() const
{
    return m_RowName;
}

/**
 * @brief Alarmconfigcolumn::setRowName : Sets the alarm parameter row name
 * @param : QString parameter
 */
void Alarmconfigcolumn::setRowName(const QString parameter)
{
    if (parameter != m_RowName)
    {
        m_RowName = parameter;
        emit rowNameChanged();
    }
}

/**
 * @brief Alarmconfigcolumn::getAlarmCheckboxStatus : Returns the checkbox state
 * @return : bool
 */
bool Alarmconfigcolumn::getAlarmCheckboxStatus()
{
    return m_checkboxstatus;
}

/**
 * @brief Alarmconfigcolumn::setCheckboxStatus : Sets the checkbox state
 * @param : bool parameter
 */
void Alarmconfigcolumn::setCheckboxStatus(bool parameter)
{
    if(parameter != m_checkboxstatus)
    {
        m_checkboxstatus = parameter;
        emit checkboxStatusChanged();
    }
}

/**
 * @brief Alarmconfigcolumn::Alarmconfigcolumn : Constructor
 * @param : QObject parent
 */
Alarmconfigcolumn::Alarmconfigcolumn(QObject *parent): QObject(parent)
{

}

/**
 * @brief Alarmconfigcolumn::Alarmconfigcolumn : Initialization of class members and memory for object
 * @param : QString paramName
 * @param : QString rowName
 * @param : bool status
 * @param : QObject parent
 */
Alarmconfigcolumn::Alarmconfigcolumn(const QString paramName, const QString rowName, bool status, QObject *parent)  :
    QObject(parent), m_ParameterName(paramName), m_RowName(rowName), m_checkboxstatus(status)
{
}
