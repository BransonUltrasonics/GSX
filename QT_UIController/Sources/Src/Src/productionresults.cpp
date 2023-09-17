/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Production Results

***************************************************************************/

#include "Header/productionresults.h"

/**
 * @brief ProductionResults::ProductionResults
 * @param parent
 */
ProductionResults::ProductionResults(QObject *parent)
    : QObject(parent)
{
}
/**
 * @brief ProductionResults::ProductionResults
 * @param name
 * @param value
 * @param color
 * @param font
 * @param parent
 */
ProductionResults::ProductionResults(const QString name,const QString value, const QString color, const int font, QObject *parent)
    : QObject(parent), m_ParameterName(name),m_ParameterValue(value), m_ParameterColor(color), m_FontSize(font)
{
}

/**
 * @brief ProductionResults::getParameterName : Get the result paramter name
 * @return
 */
QString ProductionResults::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief ProductionResults::setParameterName : Set the result paramter name
 * @param parameter
 */
void ProductionResults::setParameterName(const QString parameter)
{
    if (parameter != m_ParameterName)
    {
        m_ParameterName = parameter;
        emit parameterNameChanged();
    }
}

/**
 * @brief ProductionResults::getParameterValue : Get the result parameter value
 * @return
 */
QString ProductionResults::getParameterValue() const
{
    return m_ParameterValue;
}

/**
 * @brief ProductionResults::setParameterValue : Set the result paramter value
 * @param value
 */
void ProductionResults::setParameterValue(const QString value)
{
    if (value != m_ParameterValue)
    {
        m_ParameterValue = value;
        emit parameterValueChanged();
    }
}

/**
 * @brief ProductionResults::getParameterRecColor : Return the color
 * @return
 */
QString ProductionResults::getParameterRecColor() const
{
    return m_ParameterColor;
}

/**
 * @brief ProductionResults::setParameterRecColor : Set the parameter color
 * @param parameter
 */
void ProductionResults::setParameterRecColor(const QString parameter)
{
    if (parameter != m_ParameterColor)
    {
        m_ParameterColor = parameter;
        emit parameterRecColorChanged();
    }
}

/**
  * @brief ProductionResults::getFontSize : Return the font size
  * @return
  */
 int ProductionResults::getFontSize() const
 {
    return m_FontSize;
 }

 /**
  * @brief ProductionResults::setFontSize : Set the font size
  * @param value
  */
 void ProductionResults::setFontSize(const int value)
 {
     if (value != m_FontSize)
     {
         m_FontSize = value;
         emit fontSizeChanged();
     }
 }
