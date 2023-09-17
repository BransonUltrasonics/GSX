/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Digital IO

***************************************************************************/
#ifndef DIGITALIO_H
#define DIGITALIO_H
#include <Header/userio_io.h>
class DigitalIO:public IO
{
    Q_OBJECT

public :
    virtual void initializeIOPinsFunctions()=0;
    virtual void initializeIODefaultFunctions()=0;
    virtual void initializeIOPinsInformation()=0;
    virtual void getDataFromDBUsingIds(QStringList)=0;
    virtual void refreshModel()=0;
    virtual void saveDataToStructs(int a_Index,int a_Function,int a_ActiveStatus,int a_CheckboxStatus)=0;
protected:
    int  updateDigitalParamsModelComboBoxListIfNoneSelected(UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected);
    void setCurrentIndexInDigitalParamsModelComboboxLists(UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected, int actualIndex);
    void getQListOfSelectedItemInDigitalParamsModel(UserIODigitalParamsModel *temp_UserIOModelSelected, QList<QObject *> &qobjectListSelected);
};
#endif // DIGITALIO_H
