/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Actuator IO Digital Inputs

***************************************************************************/
#ifndef DIGITALINPUTSAC_H
#define DIGITALINPUTSAC_H

#include <Header/userio_digitalinputs.h>
class DigitalInputsAC : public DigitalInputs
{
public :
    DigitalInputsAC();
    AC_DIGITALINPUT getStructData();
    AC_DIGITALINPUT_CHECHSTATUS getACDigitalInputStructData();
    void getDataFromDBUsingIds(QStringList);
    void refreshModel();
    void setUIAfterSaving();
private:
    AC_DIGITALINPUT m_stDigitalInputs;
    AC_DIGITALINPUT_CHECHSTATUS m_stACDigitalInputsCheckStatus;
    void initializeIOPinsFunctions();
    void initializeActualIOPinsFunctions();
    void initializeIODefaultFunctions();
    void initializeIOPinsInformation();
    void saveDataToStructs(int a_Index,int a_Function,int a_ActiveStatus,int a_CheckboxStatus);
};

#endif // DIGITALINPUTSAC_H
