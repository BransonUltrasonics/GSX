/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Recipe Parameter

***************************************************************************/

#include "Header/recipeparameter.h"

/**
 * @brief RecipeParameter::RecipeParameter
 * @param parent
 */
RecipeParameter::RecipeParameter(QObject *parent)
    : QObject(parent)
{
    m_ParameterSwitch = false;
    m_ParameterSwitchState = false;
}

/**
 * @brief RecipeParameter::RecipeParameter : Initialization of class members and memory for object
 * @param name
 * @param value
 * @param unit
 * @param isSwitch
 * @param switchState
 * @param parent
 */
RecipeParameter::RecipeParameter(const QString name, int index, const QString value,
                                 const QString unit, bool isSwitch,  bool doubleSwitch,
                                 bool switchState, bool tripleSwitch, QObject *parent,
                                 bool parameterVisible)
    : QObject(parent)
{
    m_ParameterName = name;
    m_ParameterIndex = index;
    m_ParameterValue = value;
    m_ParameterUnit = unit;
    m_ParameterSwitch = isSwitch;
    m_ParameterDoubleSwitch = doubleSwitch;
    m_ParameterSwitchState = switchState;
    m_ParameterTripleSwitch = tripleSwitch;
    m_ParameterVisible = parameterVisible;
//    m_ParameterMinValue = minVal;
//    m_ParameterMaxValue = maxVal;
//    m_ParameterStripColor = stripColor;
}

/**
 * @brief RecipeParameter::getParameterName
 * @return
 */
QString RecipeParameter::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief RecipeParameter::setParameterName
 * @param name
 */
void RecipeParameter::setParameterName(const QString name)
{
    if (name != m_ParameterName)
    {
        m_ParameterName = name;
        emit parameterNameChanged();
    }
}


/**
 * @brief RecipeParameter::getParameterLocaleName
 * @return
 */
QString RecipeParameter::getParameterLocaleName() const
{
    return m_ParameterLocaleName;
}

/**
 * @brief RecipeParameter::setParameterLocaleName
 * @param name
 */
void RecipeParameter::setParameterLocaleName(const QString name)
{
    if (name != m_ParameterLocaleName)
    {
        m_ParameterLocaleName = name;
        emit parameterLocaleNameChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterIndex
 * @return
 */
int RecipeParameter::getParameterIndex()
{
    return m_ParameterIndex;
}

/**
 * @brief RecipeParameter::setParameterIndex
 * @param index
 */
void RecipeParameter::setParameterIndex(int index)
{
    if (index != m_ParameterIndex)
    {
        m_ParameterIndex = index;
        emit parameterIndexChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterValue
 * @return
 */

QString RecipeParameter::getParameterValue() const
{
    return m_ParameterValue;
}

/**
 * @brief RecipeParameter::setParameterValue
 * @param value
 */
void RecipeParameter::setParameterValue(const QString value)
{
    if (value != m_ParameterValue)
    {
        m_ParameterValue = value;
        emit parameterValueChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterUnit
 * @return
 */
QString RecipeParameter::getParameterUnit() const
{
    return m_ParameterUnit;
}

/**
 * @brief RecipeParameter::setParameterUnit
 * @param unit
 */
void RecipeParameter::setParameterUnit(const QString unit)
{
    if (unit != m_ParameterUnit)
    {
        m_ParameterUnit = unit;
        emit parameterUnitChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterSwitch
 * @return
 */
bool RecipeParameter::getParameterSwitch()
{
    return m_ParameterSwitch;
}

/**
 * @brief RecipeParameter::setParameterSwitch
 * @param isSwitch
 */
void RecipeParameter::setParameterSwitch(bool isSwitch)
{
    if (isSwitch != m_ParameterSwitch)
    {
        m_ParameterSwitch = isSwitch;
        emit ParameterSwitchChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterDoubleSwitch
 * @return
 */
bool RecipeParameter::getParameterDoubleSwitch()
{
    return m_ParameterDoubleSwitch;
}

/**
 * @brief RecipeParameter::setParameterDoubleSwitch
 * @param isSwitch
 */
void RecipeParameter::setParameterDoubleSwitch(bool isSwitch)
{
    if (isSwitch != m_ParameterDoubleSwitch)
    {
        m_ParameterDoubleSwitch = isSwitch;
        emit ParameterDoubleSwitchChanged();
    }
}

/**
  * @brief RecipeParameter::getParameterSwitchState
  * @return
  */
bool RecipeParameter::getParameterSwitchState()
{
    return m_ParameterSwitchState;
}

/**
 * @brief RecipeParameter::setParameterSwitchState
 * @param switchState
 */
void RecipeParameter::setParameterSwitchState(bool switchState)
{
    if (switchState != m_ParameterSwitchState)
    {
        m_ParameterSwitchState = switchState;
        emit ParameterSwitchStateChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterMinValue
 * @return
 */
QString RecipeParameter::getParameterMinValue() const
{
    return m_ParameterMinValue;
}

/**
 * @brief RecipeParameter::setParameterMinValue
 * @param value
 */
void RecipeParameter::setParameterMinValue(const QString minValue)
{
    if (minValue != m_ParameterMinValue)
    {
        m_ParameterMinValue = minValue;
        emit parameterMinValueChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterMaxValue
 * @return
 */
QString RecipeParameter::getParameterMaxValue() const
{
    return m_ParameterMaxValue;
}

/**
 * @brief RecipeParameter::setParameterMaxValue
 * @param value
 */
void RecipeParameter::setParameterMaxValue(const QString maxValue)
{
    if (maxValue != m_ParameterMaxValue)
    {
        m_ParameterMaxValue = maxValue;
        emit parameterMaxValueChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterStripColor
 * @return
 */
QString RecipeParameter::getParameterStripColor() const
{
    return m_ParameterStripColor;
}

/**
 * @brief RecipeParameter::setParameterStripColor
 * @param stripColor
 */
void RecipeParameter::setParameterStripColor(const QString stripColor)
{
    if(stripColor != m_ParameterStripColor)
    {
        m_ParameterStripColor = stripColor;
        emit parameterStripColorChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterTripleSwitch
 * @return
 */
bool RecipeParameter::getParameterTripleSwitch()
{
    return m_ParameterTripleSwitch;
}

/**
 * @brief RecipeParameter::setParameterTripleSwitch
 * @param isSwitch
 */
void RecipeParameter::setParameterTripleSwitch(bool isSwitch)
{
    if (isSwitch != m_ParameterTripleSwitch)
    {
        m_ParameterTripleSwitch = isSwitch;
        emit ParameterTripleSwitchChanged();
    }
}

/**
 * @brief RecipeParameter::getParameterVisible
 * @return m_ParameterVisible
 */
bool RecipeParameter::getParameterVisible()
{
    return m_ParameterVisible;
}

/**
 * @brief RecipeParameter::setParameterVisible
 * @return ParamVisible
 */
void RecipeParameter::setParameterVisible(bool ParamVisible)
{
    if (ParamVisible != m_ParameterVisible)
    {
        m_ParameterVisible = ParamVisible;
        emit ParameterVisibleChanged();
    }
}

/**
 * @brief RecipeParameter::getDoubleSwitchLeftText
 * @return m_DoubleSwitchLeftText
 */
QString RecipeParameter::getDoubleSwitchLeftText() const
{
    return m_DoubleSwitchLeftText;
}

/**
 * @brief RecipeParameter::setDoubleSwitchLeftText
 * @param leftText
 */
void RecipeParameter::setDoubleSwitchLeftText(const QString leftText)
{
    if (leftText != m_DoubleSwitchLeftText)
    {
        m_DoubleSwitchLeftText = leftText;
        emit DoubleSwitchLeftTextChanged();
    }
}

/**
 * @brief RecipeParameter::getDoubleSwitchRightText
 * @return m_DoubleSwitchRightText
 */
QString RecipeParameter::getDoubleSwitchRightText() const
{
    return m_DoubleSwitchRightText;
}

/**
 * @brief RecipeParameter::setDoubleSwitchRightText
 * @param rightText
 */
void RecipeParameter::setDoubleSwitchRightText(const QString rightText)
{
    if (rightText != m_DoubleSwitchRightText)
    {
        m_DoubleSwitchRightText = rightText;
        emit DoubleSwitchRightTextChanged();
    }
}
