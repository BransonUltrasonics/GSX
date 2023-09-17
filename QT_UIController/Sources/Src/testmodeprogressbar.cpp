/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Mode Progressbar Model

***************************************************************************/
#include "Header/testmodeprogressbar.h"

/**
 * @brief TestModeProgressBar::TestModeProgressBar
 * @param parent
 */
TestModeProgressBar::TestModeProgressBar(QObject *parent): QObject(parent)
{
    Q_UNUSED(parent)
}

/**
 * @brief TestModeProgressBar::TestModeProgressBar : Constructor
 * @param paramName
 * @param paramValue
 * @param paramUnit
 * @param parent
 */
TestModeProgressBar::TestModeProgressBar(QString paramName, double paramValue, QString paramUnit,QObject *parent):QObject(parent)
  ,m_ParamName(paramName)
    , m_ParamValue(paramValue)
    , m_ParamUnit(paramUnit)
{
}

/**
 * @brief TestModeProgressBar::setParamName : Set Parameter name
 * @param paramName
 */
void TestModeProgressBar::setParamName(const QString &paramName)
{
    m_ParamName = paramName;
}

/**
 * @brief TestModeProgressBar::setParamValue : Set Parameter value
 * @param paramValue
 */
void TestModeProgressBar::setParamValue(double paramValue)
{
    m_ParamValue = paramValue;
}

/**
 * @brief TestModeProgressBar::setParamUnit : Set parameter unit
 * @param paramUnit
 */
void TestModeProgressBar::setParamUnit(const QString &paramUnit)
{
    m_ParamUnit = paramUnit;
}

/**
 * @brief TestModeProgressBar::getParamName : Return Parameter Name
 * @return
 */
QString TestModeProgressBar::getParamName() const
{
    return m_ParamName;
}

/**
 * @brief TestModeProgressBar::getParamValue : Return Parameter Value
 * @return
 */
double TestModeProgressBar::getParamValue() const
{
    return m_ParamValue;
}

/**
 * @brief TestModeProgressBar::getParamUnit : Return Parameter Unit
 * @return
 */
QString TestModeProgressBar::getParamUnit() const
{
    return m_ParamUnit;
}
