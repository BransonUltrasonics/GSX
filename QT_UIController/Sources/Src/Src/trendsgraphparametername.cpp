/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Trends Graph

***************************************************************************/
#include "Header/trendsgraphparametername.h"

/**
 * @brief Trendsgraphparametername::getParameterName : Get the parameter name
 * @return
 */
QString Trendsgraphparametername::getParameterName() const
{
    return m_ParameterName;
}

/**
 * @brief trendsgraphparametername::setParameterName : Set the parameter name
 * @param parameter
 */
void Trendsgraphparametername::setParameterName(const QString parameter)
{
    if (parameter != m_ParameterName)
    {
        m_ParameterName = parameter;
        emit parameterNameChanged();
    }
}

/**
 * @brief Trendsgraphparametername::getCycleCounterVal : Get cycle count value
 * @return
 */
QString Trendsgraphparametername::getCycleCounterVal() const
{
    return m_CycleCounterVal;
}

/**
 * @brief trendsgraphparametername::setCycleCounterVal : Set Cycle count value
 * @param parameter
 */
void Trendsgraphparametername::setCycleCounterVal(const QString parameter)
{
    if (parameter != m_CycleCounterVal)
    {
        m_CycleCounterVal = parameter;
        emit cycleCounterValChanged();
    }
}

/**
 * @brief trendsgraphparametername::trendsgraphparametername : Constructor
 * @param parent
 */
Trendsgraphparametername::Trendsgraphparametername(QObject *parent): QObject(parent)
{

}

/**
 * @brief trendsgraphparametername::trendsgraphparametername
 * @param name
 * @param parent
 */
Trendsgraphparametername::Trendsgraphparametername(const QString name, QObject *parent)  : QObject(parent), m_ParameterName(name)
{

}

/**
 * @brief TrendsGraphCycleCounterVal::TrendsGraphCycleCounterVal : Send CycleCounter value and status
 * @param a_labelName
 * @param a_xAxisVal
 */
TrendsGraphCycleCounterVal :: TrendsGraphCycleCounterVal(const QString &a_cycleCountName,const int &a_alarmFlag )
{
    m_cycleCountName = a_cycleCountName ;
    m_alarmFlag =a_alarmFlag;
}

/**
 * @brief TrendsGraphCycleCounterVal::~TrendsGraphCycleCounterVal
 */
TrendsGraphCycleCounterVal :: ~TrendsGraphCycleCounterVal()
{

}

/**
 * @brief TrendsGraphCycleCounterVal::getCycleCountName : Get cycle count name
 * @return
 */
QString TrendsGraphCycleCounterVal :: getCycleCountName()
{
    return m_cycleCountName;
}

/**
 * @brief TrendsGraphCycleCounterVal::getAlarmFlag : Get alarm status
 *
 * @return
 */
int TrendsGraphCycleCounterVal :: getAlarmFlag()
{
    return m_alarmFlag;
}
