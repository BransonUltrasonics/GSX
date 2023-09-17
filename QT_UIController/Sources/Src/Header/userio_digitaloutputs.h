/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Digital Outputs

***************************************************************************/
#ifndef DIGITALOUTPUTS_H
#define DIGITALOUTPUTS_H
#include <Header/userio_digitalio.h>
class DigitalOutputs:public DigitalIO
{

    Q_OBJECT

private:
    bool isOptionAvailableInOtherComboBoxes(int a_CurrentIndex, int a_actualIndex);
    bool isOptionAvailableInComboBox(UserIODigitalParamsModel *temp_UserIODigitalParamsModelSelected, int a_actualIndex);
public:
    Q_INVOKABLE void setDefaultValuesForDigitalOutputParams();
    Q_INVOKABLE void updateDigitalOutputsModelList(int a_ListViewIndex,int a_ComboboxIndex,bool a_CheckBoxState,bool a_ActiveStatus);
    Q_INVOKABLE void onUnchecked(int a_ListViewIndex, int a_ComboboxIndex, bool a_CheckBoxState, bool a_ActiveStatus);
    Q_INVOKABLE void onChecking(int a_ListViewIndex, int a_ComboboxIndex, bool a_CheckBoxState, bool a_ActiveStatus);
protected:
    void updatePinsInformationFromDBData(QList<PinData> a_DataList);
    int updateDigitalOutputParamsModelComboBoxListIfNoneNotSelected(UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneNotSelected, QList<QObject*> &qobjectListIfNoneNotSelcted,int indexToRemove);
	void updateDigitalOutputParamsComboboxList(QList<QObject*> &qobjectListIfNoneSelected, int previousSelectedActualIndex, int previousSelectedIndex);
    virtual void initializeIOPinsFunctions()=0;
    virtual void getDataFromDBUsingIds(QStringList)=0;
    virtual void refreshModel()=0;
    virtual void initializeIODefaultFunctions()=0;
    virtual void initializeIOPinsInformation()=0;
    virtual void saveDataToStructs(int a_Index,int a_Function,int a_ActiveStatus,int a_CheckboxStatus)=0;

};
#endif // DIGITALOUTPUTS_H
