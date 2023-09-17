/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power Supply IO handles UI

***************************************************************************/
#ifndef USERIOSC_H
#define USERIOSC_H

#include <Header/userio_digitalinputsps.h>
#include <Header/userio_digitaloutputsps.h>
#include <Header/userio_analoginputsps.h>
#include <Header/userio_analogoutputsps.h>

class   UserIOPS : public QObject
{
    Q_OBJECT

public:
    explicit UserIOPS(QObject *parent = nullptr);
    void getDataFromDB(QStringList);
    Q_INVOKABLE void refreshModel();
    Q_INVOKABLE void setUIAfterSaving();
    const DigitalInputsPS  & getDigitalInputs() ;
    const DigitalOutputsPS & getDigitalOutputs() ;
    const AnalogInputsPS   & getAnalogInputs() ;
    const AnalogOutputsPS  & getAnalogOutputs() ;
private:
    DigitalInputsPS m_DigitalInputs;
    DigitalOutputsPS m_DigitalOutputs;
    AnalogInputsPS m_AnalogInputs;
    AnalogOutputsPS m_AnalogOutputs;
};

#endif // USERIOSC_H
