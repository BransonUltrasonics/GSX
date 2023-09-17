/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Suspect Reject Limits

***************************************************************************/

#include "Header/suspectrejectparameter.h"

/**
 * @brief SuspectRejectParameter::SuspectRejectParameter : Constructor
 * @param parent
 */
SuspectRejectParameter::SuspectRejectParameter(QObject *parent)
    : QObject(parent)
{
}
/**
 * @brief SuspectRejectParameter::SuspectRejectParameter : Initialization of class members and memory for object
 * @param name
 * @param suspectLow
 * @param suspectHigh
 * @param rejectLow
 * @param rejectHigh
 * @param suspectStatus
 * @param rejectStatus
 * @param parent
 */
SuspectRejectParameter::SuspectRejectParameter(QString name,
                                               QString localeName,
                                               bool suspectStatus,
                                               bool suspectLowStatus,
                                               bool suspectHighStatus,
                                               QString suspectLow,
                                               QString suspectHigh,
                                               bool rejectStatus,
                                               bool rejectLowStatus,
                                               bool rejectHighStatus,
                                               QString rejectLow,
                                               QString rejectHigh,
                                               bool isSwitch,
                                               bool switchState,
                                               QObject *parent)
    : QObject(parent)
{
    m_parameterName = name;
    m_parameterLocaleName = localeName;
    m_suspectLow = suspectLow;
    m_suspectHigh = suspectHigh;
    m_rejectLow = rejectLow;
    m_rejectHigh = rejectHigh;
    m_suspectStatus = suspectStatus;
    m_suspectLowStatus = suspectLowStatus;
    m_suspectHighStatus = suspectHighStatus;
    m_rejectStatus = rejectStatus;
    m_rejectLowStatus = rejectLowStatus;
    m_rejectHighStatus = rejectHighStatus;
    m_ParameterSwitch = isSwitch;
    m_ParameterSwitchState = switchState;
}

/**
 * @brief SuspectRejectParameter::getParameterName : Return Name
 * @return
 */
QString SuspectRejectParameter::getParameterName() const
{
    return m_parameterName;
}

/**
 * @brief SuspectRejectParameter::getSuspectLow : Return suspect low
 * @return
 */
QString SuspectRejectParameter::getSuspectLow() const
{
    return m_suspectLow;
}

/**
 * @brief SuspectRejectParameter::getSuspectHigh : Return suspect high
 * @return
 */
QString SuspectRejectParameter::getSuspectHigh() const
{
    return m_suspectHigh;
}

/**
 * @brief SuspectRejectParameter::getRejectLow : Return reject low
 * @return
 */
QString SuspectRejectParameter::getRejectLow() const
{
    return m_rejectLow;
}

/**
 * @brief SuspectRejectParameter::getRejectHigh : Return reject high
 * @return
 */
QString SuspectRejectParameter::getRejectHigh() const
{
    return m_rejectHigh;
}

/**
 * @brief SuspectRejectParameter::getSuspectStatus : Return suspect status
 * @return
 */
bool SuspectRejectParameter::getSuspectStatus() const
{
    return m_suspectStatus;
}

/**
 * @brief SuspectRejectParameter::getRejectStatus : Return reject status
 * @return
 */
bool SuspectRejectParameter::getRejectStatus() const
{
    return m_rejectStatus;
}

/**
 * @brief RecipeParameter::getParameterSwitch : Required switch options
 * @return
 */
bool SuspectRejectParameter::getParameterSwitch()
{
    return m_ParameterSwitch;
}

/**
 * @brief RecipeParameter::getParameterSwitchState : Status of the switch
 * @return
 */
bool SuspectRejectParameter::getParameterSwitchState()
{
    return m_ParameterSwitchState;
}

/**
 * @brief SuspectRejectParameter::setParameterName : Set the name assign to member variable
 * @param parameter
 */
void SuspectRejectParameter::setParameterName(const QString parameter)
{
    if (parameter != m_parameterName)
    {
        m_parameterName = parameter;
        emit parameterNameChanged();
    }
}

/**
 * @brief SuspectRejectParameter::getParameterLocaleName : Returns Locale Name
 * @return
 */
QString SuspectRejectParameter::getParameterLocaleName() const
{
    return m_parameterLocaleName;
}

/**
 * @brief SuspectRejectParameter::setParameterLocaleName : Set the name assign to member variable
 * @param name
 */
void SuspectRejectParameter::setParameterLocaleName(const QString name)
{
    if (name != m_parameterLocaleName)
    {
        m_parameterLocaleName = name;
        emit parameterLocaleNameChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setSuspectLow : Set suspect low values assign to member variable
 * @param suspectLow
 */
void SuspectRejectParameter::setSuspectLow(const QString suspectLow)
{
    if (suspectLow != m_suspectLow)
    {
        m_suspectLow = suspectLow;
        emit SuspectLowValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setSuspectHigh : Set suspect hign values assign to member variable
 * @param suspectHigh
 */
void SuspectRejectParameter::setSuspectHigh(const QString suspectHigh)
{
    if (suspectHigh != m_suspectHigh)
    {
        m_suspectHigh = suspectHigh;
        emit SuspectHighValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setRejectLow : Set reject low values assign to member variable
 * @param rejectLow
 */
void SuspectRejectParameter::setRejectLow(const QString rejectLow)
{
    if (rejectLow != m_rejectLow)
    {
        m_rejectLow = rejectLow;
        emit RejectLowValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setRejectHigh : Set reject low values assign to member variable
 * @param rejectHigh
 */
void SuspectRejectParameter::setRejectHigh(const QString rejectHigh)
{
    if (rejectHigh != m_rejectHigh)
    {
        m_rejectHigh = rejectHigh;
        emit RejectHighValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setSuspectStatus : Set suspect status assign to member variable
 * @param status
 */
void SuspectRejectParameter::setSuspectStatus(const bool status)
{
    if (status != m_suspectStatus)
    {
        m_suspectStatus = status;
        emit SuspectStatusValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setRejectStatus : Set suspect status assign to member variable
 * @param status
 */
void SuspectRejectParameter::setRejectStatus(const bool status)
{
    if (status != m_rejectStatus)
    {
        m_rejectStatus = status;
        emit RejectStatusValueChanged();
    }
}

/**
 * @brief RecipeParameter::setParameterSwitch : Set values status assign to member variable
 * @param isSwitch
 */
void SuspectRejectParameter::setParameterSwitch(bool isSwitch)
{
    if (isSwitch != m_ParameterSwitch)
    {
        m_ParameterSwitch = isSwitch;
        emit ParameterSwitchChanged();
    }
}

/**
 * @brief RecipeParameter::setParameterSwitchState : Set status assign to member variable
 * @param switchState
 */
void SuspectRejectParameter::setParameterSwitchState(bool switchState)
{
    if (switchState != m_ParameterSwitchState)
    {
        m_ParameterSwitchState = switchState;
        emit ParameterSwitchStateChanged();
    }
}

/**
 * @brief SuspectRejectParameter::getSuspectLowStatus : Return suspect low status
 * @return
 */
bool SuspectRejectParameter::getSuspectLowStatus() const
{
    return m_suspectLowStatus;
}

/**
 * @brief SuspectRejectParameter::getSuspectHighStatus : Return suspect high status
 * @return
 */
bool SuspectRejectParameter::getSuspectHighStatus() const
{
    return m_suspectHighStatus;
}

/**
 * @brief SuspectRejectParameter::setSuspectLowStatus : Set suspect low status assign to member variable
 * @param status
 */
void SuspectRejectParameter::setSuspectLowStatus(const bool status)
{
    if (status != m_suspectLowStatus)
    {
        m_suspectLowStatus = status;
        emit SuspectLowStatusValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setSuspectHighStatus : Set suspect high status assign to member variable
 * @param status
 */
void SuspectRejectParameter::setSuspectHighStatus(const bool status)
{
    if (status != m_suspectLowStatus)
    {
        m_suspectLowStatus = status;
        emit SuspectLowStatusValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::getRejectLowStatus : Return reject low
 * @return
 */
bool SuspectRejectParameter::getRejectLowStatus() const
{
    return m_rejectLowStatus;
}

/**
 * @brief SuspectRejectParameter::getRejectHighStatus : Return reject high
 * @return
 */
bool SuspectRejectParameter::getRejectHighStatus() const
{
    return m_rejectHighStatus;
}

/**
 * @brief SuspectRejectParameter::setRejectLowStatus : Return reject low
 * @param status
 */
void SuspectRejectParameter::setRejectLowStatus(const bool status)
{
    if (status != m_rejectLowStatus)
    {
        m_rejectLowStatus = status;
        emit RejectLowStatusValueChanged();
    }
}

/**
 * @brief SuspectRejectParameter::setRejectHighStatus : Set reject status assign to member variable
 * @param status
 */
void SuspectRejectParameter::setRejectHighStatus(const bool status)
{
    if (status != m_rejectHighStatus)
    {
        m_rejectHighStatus = status;
        emit RejectHighStatusValueChanged();
    }
}
