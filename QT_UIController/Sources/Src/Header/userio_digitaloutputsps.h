/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply IO Digital Outputs

***************************************************************************/
#ifndef DIGITALOUTPUTSPS_H
#define DIGITALOUTPUTSPS_H

#include <Header/userio_digitaloutputs.h>
class DigitalOutputsPS : public DigitalOutputs
{
public:
    PS_DIGITALOUTPUT getStructData();
    PS_DIGITALOUTPUT_CHECKSTATUS getPSDigitalOutputStructData();
    DigitalOutputsPS();
    void getDataFromDBUsingIds(QStringList);
    void refreshModel();
    void setUIAfterSaving();

private:
    PS_DIGITALOUTPUT m_stDigitalOutputs;
    PS_DIGITALOUTPUT_CHECKSTATUS m_stPSDigitalOutputsCheckStatus;
    void initializeIOPinsFunctions();
    void initializeActualIOPinsFunctions();
    void initializeIODefaultFunctions();
    void initializeIOPinsInformation();
    void saveDataToStructs(int a_Index,int a_Function,int a_ActiveStatus,int a_CheckboxStatus);

};

#endif // DIGITALOUTPUTSPS_H
