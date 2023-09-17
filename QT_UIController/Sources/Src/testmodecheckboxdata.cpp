/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Mode Checkbox Model

***************************************************************************/
#include "Header/testmodecheckboxdata.h"

/**
 * @brief TestModeCheckBoxData::TestModeCheckBoxData : Constructor
 * @param parent
 */
TestModeCheckBoxData::TestModeCheckBoxData(QObject *parent):QObject(parent)
{
    Q_UNUSED(parent)
}
/**
 * @brief TestModeCheckBoxData::TestModeCheckBoxData
 * @param paramName
 * @param checkBoxColor
 * @param checkBoxStatus
 * @param paramValue
 * @param paramUnit
 * @param parent
 */
TestModeCheckBoxData::TestModeCheckBoxData(QString paramName
                     , QString checkBoxColor
                     , bool checkBoxStatus
                     , QString paramValue
                     , QString paramUnit,QObject *parent):QObject(parent)
    , m_ParamName(paramName)
    , m_CheckBoxColor(checkBoxColor)
    , m_CheckBoxStatus(checkBoxStatus)
    , m_ParamValue(paramValue)
    , m_ParamUnit(paramUnit)
{
}

/**
 * @brief TestModeCheckBoxData::setParamName : Set the Parameter Name
 * @param paramName
 */
void TestModeCheckBoxData::setParamName(const QString &paramName)
{
    m_ParamName = paramName;
}

/**
 * @brief TestModeCheckBoxData::setCheckBoxColor : set the Checkbox color
 * @param boxColor
 */
void TestModeCheckBoxData::setCheckBoxColor(const QString &boxColor)
{
    m_CheckBoxColor = boxColor;
}

/**
 * @brief TestModeCheckBoxData::setCheckBoxStatus : Set teh Checkbox Status
 * @param boxStatus
 */
void TestModeCheckBoxData::setCheckBoxStatus(bool boxStatus)
{
    m_CheckBoxStatus = boxStatus;
}

/**
 * @brief TestModeCheckBoxData::setParamValue : set the parameter value
 * @param paramValue
 */
void TestModeCheckBoxData::setParamValue(const QString &paramValue)
{
    m_ParamValue = paramValue;
}

/**
 * @brief TestModeCheckBoxData::setParamUnit : set the parameter unit
 * @param paramUnit
 */
void TestModeCheckBoxData::setParamUnit(const QString &paramUnit)
{
    m_ParamUnit = paramUnit;
}

/**
 * @brief TestModeCheckBoxData::getParamName : Get the parameter unit
 * @return
 */
QString TestModeCheckBoxData::getParamName() const
{
    return m_ParamName;
}

/**
 * @brief TestModeCheckBoxData::getCheckBoxColor : Get the chekbox color
 * @return
 */
QString TestModeCheckBoxData::getCheckBoxColor() const
{
    return m_CheckBoxColor;
}

/**
 * @brief TestModeCheckBoxData::getCheckBoxStatus : Get the checkbox status
 * @return
 */
bool TestModeCheckBoxData::getCheckBoxStatus() const
{
    return m_CheckBoxStatus;
}

/**
 * @brief TestModeCheckBoxData::getParamValue
 * @return
 */
QString TestModeCheckBoxData::getParamValue() const
{
    return m_ParamValue;
}

/**
 * @brief TestModeCheckBoxData::getParamUnit : get the parameter unit
 * @return
 */
QString TestModeCheckBoxData::getParamUnit() const
{
    return m_ParamUnit;
}
