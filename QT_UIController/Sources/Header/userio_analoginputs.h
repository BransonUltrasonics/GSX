/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Analog Inputs

***************************************************************************/
#ifndef ANALOGINPUTS_H
#define ANALOGINPUTS_H
#include <Header/userio_analogio.h>
class AnalogInputs:public AnalogIO
{
    Q_OBJECT

private:
    bool isOptionAvailableInOtherComboBoxes(int a_CurrentIndex, int a_actualIndex);
    bool isOptionAvailableInComboBox(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected, int a_actualIndex);
public:
    Q_INVOKABLE void updateAnalogInputsModelList(int a_ListViewIndex, int a_ComboboxIndex, bool a_CheckBoxState);
    Q_INVOKABLE void setDefaultValuesForAnalogInputParams();
    Q_INVOKABLE void onUnchecked(int a_ListViewIndex, int a_ComboboxIndex, bool a_CheckBoxState);
    Q_INVOKABLE void onChecking(int a_ListViewIndex, int a_ComboboxIndex, bool a_CheckBoxState);
protected:
    void updatePinsInformationFromDBData(QList<PinData> a_DataList);
    virtual void initializeIOPinsFunctions()=0;
    virtual void initializeIODefaultFunctions()=0;
    virtual void initializeIOPinsInformation()=0;
    virtual void getDataFromDBUsingIds(QStringList)=0;
    virtual void refreshModel()=0;
    void updateAnalogInputParamsComboboxList(QList<QObject*> &qobjectListIfNoneSelected, int previousSelectedActualIndex, int previousSelectedIndex);
    int  updateAnalogInputParamsModelComboBoxListIfNoneNotSelected(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneNotSelected, QList<QObject*> &qobjectListIfNoneNotSelected,int indexToRemove);
    virtual void saveDataToStructs(int a_Index,int a_Function,int a_CheckboxStatus)=0;

};
#endif // ANALOGINPUTS_H
