/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Setup Limits

***************************************************************************/

#ifndef SETUPLIMITIS_H
#define SETUPLIMITIS_H
#include <QObject>
#include "custompropertyhelper.h"
class SetupLimits : public QObject
{
    Q_OBJECT

public:
    explicit SetupLimits(QObject *parent = nullptr): QObject(parent) {}
    SetupLimits(QString ParameterName,QString ParameterLocaleName,bool SetUpEnabled,
                QString SetUpMinValue,QString SetUpMaxValue,
                QString SetupUnit,bool SetUpWeld,
                bool ParameterSwitch,bool ParameterSwitchState,
                bool ParameterRec,QString WeldMode,bool SetUpParameterVisible) :
                m_ParameterName(ParameterName),
                m_ParameterLocaleName(ParameterLocaleName),
                m_SetUpEnabled(SetUpEnabled),
                m_SetUpMinValue(SetUpMinValue),
                m_SetUpMaxValue(SetUpMaxValue),
                m_SetupUnit(SetupUnit),
                m_SetUpWeld(SetUpWeld),
                m_ParameterSwitch(ParameterSwitch),
                m_ParameterSwitchState(ParameterSwitchState),
                m_ParameterRec(ParameterRec),
                m_WeldMode(WeldMode),
                m_SetUpParameterVisible(SetUpParameterVisible){}
    CUSTOM_PROPERTY(QString, ParameterName)
    CUSTOM_PROPERTY(QString, ParameterLocaleName)
    CUSTOM_PROPERTY(bool, SetUpEnabled)
    CUSTOM_PROPERTY(QString, SetUpMinValue)
    CUSTOM_PROPERTY(QString, SetUpMaxValue)
    CUSTOM_PROPERTY(QString, SetupUnit)
    CUSTOM_PROPERTY(bool, SetUpWeld)
    CUSTOM_PROPERTY(bool, ParameterSwitch)
    CUSTOM_PROPERTY(bool, ParameterSwitchState)
    CUSTOM_PROPERTY(bool, ParameterRec)
    CUSTOM_PROPERTY(QString, WeldMode)
    CUSTOM_PROPERTY(bool, SetUpParameterVisible)

public:
    QStringList m_limitsDropDownFullList;
};

#endif // SETUPLIMITIS_H
