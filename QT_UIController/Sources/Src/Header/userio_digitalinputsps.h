/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply Digital Inputs

***************************************************************************/
#ifndef DIGITALINPUTSPS_H
#define DIGITALINPUTSPS_H
#include <Header/userio_digitalinputs.h>

class DigitalInputsPS : public DigitalInputs
{
public:
    PS_DIGITALINPUT getStructData();
    PS_DIGITALINPUT_CHECKSTATUS getPSDigitalInputStructData();
    DigitalInputsPS();
    void getDataFromDBUsingIds(QStringList);
    void refreshModel();
    void setUIAfterSaving();

 private:
    PS_DIGITALINPUT m_stDigitalInputs;
    PS_DIGITALINPUT_CHECKSTATUS m_stPSDigitalInputCheckStatus;
    void initializeIOPinsFunctions();
    void initializeIODefaultFunctions();
    void initializeIOPinsInformation();
    void saveDataToStructs(int a_Index,int a_Function,int a_ActiveStatus,int a_CheckboxStatus);

};

#endif // DIGITALINPUTSPS_H
