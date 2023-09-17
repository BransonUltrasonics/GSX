/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Actuator Controller UI handles UI

***************************************************************************/

#ifndef USERIOAC_H
#define USERIOAC_H
#include <algorithm>
#include <QObject>
#include <QList>
#include <Header/userio_digitalparamsmodel.h>
#include <Header/userio_analogparamsmodel.h>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "Header/userio_structs.h"
#include <Header/userio_digitalinputsac.h>
#include <Header/userio_digitaloutputsac.h>

class   UserIOAC : public QObject
{
    Q_OBJECT

public:
    explicit UserIOAC(QObject *parent = nullptr);
    void getDataFromDB(QStringList);
    Q_INVOKABLE void refreshModel();
    Q_INVOKABLE void setUIAfterSaving();
    const DigitalInputsAC &getDigitalInputs() ;
    const DigitalOutputsAC &getDigitalOutputs() ;
private:
    DigitalInputsAC m_DigitalInputs;
    DigitalOutputsAC m_DigitalOutputs;
};
#endif // USERIOAC_H
