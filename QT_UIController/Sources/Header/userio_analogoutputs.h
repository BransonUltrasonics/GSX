/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Analog Outputs

***************************************************************************/
#ifndef ANALOGOUTPUTS_H
#define ANALOGOUTPUTS_H
#include <Header/userio_analogio.h>
class AnalogOutputs:public AnalogIO
{
    Q_OBJECT

public:
    Q_INVOKABLE void updateAnalogOutputsModelList(int a_ListViewIndex, int a_ComboboxIndex, bool a_CheckBoxState);
    Q_INVOKABLE void setDefaultValuesForAnalogOutputParams();
protected:
    void updatePinsInformationFromDBData(QList<PinData> a_DataList);
    virtual void initializeIOPinsFunctions()=0;
    virtual void initializeIODefaultFunctions()=0;
    virtual void initializeIOPinsInformation()=0;
    virtual void getDataFromDBUsingIds(QStringList)=0;
    virtual void refreshModel()=0;
    virtual void saveDataToStructs(int a_Index,int a_Function,int a_CheckboxStatus)=0;


};
#endif // ANALOGOUTPUTS_H
