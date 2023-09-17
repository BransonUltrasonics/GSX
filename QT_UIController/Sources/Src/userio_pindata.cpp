/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Pin Data

***************************************************************************/
#include "Header/userio_pindata.h"

/**
 * @brief PinData::getFunction : Get the pin number and name
 * @return
 */
int PinData::getFunction() const
{
    return m_Function;
}

/**
 * @brief PinData::getActiveStatus : Status of the pin values
 * @return
 */
bool PinData::getActiveStatus() const
{
    return m_ActiveStatus;
}

/**
 * @brief PinData::setActiveStatus : Set the pin status
 * @param a_ActiveStatus
 */
void PinData::setActiveStatus(bool a_ActiveStatus)
{
    m_ActiveStatus = a_ActiveStatus;
}

/**
 * @brief PinData::setFunction : set the Pin name and number
 * @param a_Function
 */
void PinData::setFunction(int a_Function)
{
    m_Function = a_Function;
}

/**
 * @brief PinData::getCheckStatus
 * @return
 */
bool PinData::getCheckStatus() const
{
    return m_CheckStatus;
}

/**
 * @brief PinData::setCheckStatus
 * @param a_ChechStatus
 */
void PinData::setCheckStatus(bool a_ChechStatus)
{
    m_CheckStatus = a_ChechStatus;
}
