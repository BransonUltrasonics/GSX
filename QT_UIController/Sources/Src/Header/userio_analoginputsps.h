/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply Analog Inputs

***************************************************************************/
#ifndef ANALOGINPUTSPS_H
#define ANALOGINPUTSPS_H

#include <Header/userio_analoginputs.h>
class AnalogInputsPS : public AnalogInputs
{
public:
    AnalogInputsPS();
    PS_ANALOGINPUT getStructData();
    PS_ANALOGINPUT_CHECKSTATUS getPSAnalogInputStructData();
    void getDataFromDBUsingIds(QStringList);
    void refreshModel();
    void setUIAfterSaving();
private:
    PS_ANALOGINPUT m_stAnalogInputs;
    PS_ANALOGINPUT_CHECKSTATUS m_stPSAnalogInputCheckStatus;
    void initializeIOPinsFunctions();
    void initializeIODefaultFunctions();
    void initializeIOPinsInformation();
    void saveDataToStructs(int a_Index,int a_Function,int a_CheckboxStatus);
};

#endif // ANALOGINPUTSPS_H
