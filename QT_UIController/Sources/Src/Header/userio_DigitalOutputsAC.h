/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Actuator IO Digital Outputs

***************************************************************************/



#ifndef DIGITALOUTPUTSAC_H
#define DIGITALOUTPUTSAC_H
#include <Header/userio_digitaloutputs.h>

class DigitalOutputsAC : public DigitalOutputs
{
public :
    AC_DIGITALOUTPUT getStructData();
    AC_DIGITALOUTPUT_CHECKSTATUS getACDigitalOutputStructData();
    DigitalOutputsAC();
    void getDataFromDBUsingIds(QStringList);
    void refreshModel();
    void setUIAfterSaving();

private:
    AC_DIGITALOUTPUT m_stDigitalOutputs;
    AC_DIGITALOUTPUT_CHECKSTATUS m_stACDigitalOutputsCheckStatus;
    void saveDataToStructs(int a_Index,int a_Function,int a_ActiveStatus,int a_CheckboxStatus);
    void initializeIOPinsFunctions();
    void initializeActualIOPinsFunctions();
    void initializeIODefaultFunctions();
    void initializeIOPinsInformation();

};

#endif // DIGITALOUTPUTSAC_H
