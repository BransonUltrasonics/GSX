/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply Analog Outputs

***************************************************************************/
#ifndef ANALOGOUTPUTSPS_H
#define ANALOGOUTPUTSPS_H

#include <Header/userio_analogoutputs.h>
class AnalogOutputsPS : public AnalogOutputs
{
public:
    AnalogOutputsPS();
    PS_ANALOGOUTPUT getStructData();
    PS_ANALOGOUTPUT_CHECKSTATUS getPSAnalogOutputStructData();
    void getDataFromDBUsingIds(QStringList);
    void refreshModel();
    void setUIAfterSaving();
private:
    PS_ANALOGOUTPUT m_stAnalogOutputs;
    PS_ANALOGOUTPUT_CHECKSTATUS m_stPSAnalogOutputCheckStatus;
    void initializeIOPinsFunctions();
    void initializeIODefaultFunctions();
    void initializeIOPinsInformation();
    void saveDataToStructs(int a_Index,int a_Function,int a_CheckboxStatus);
};

#endif // ANALOGOUTPUTSPS_H
