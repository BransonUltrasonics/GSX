#ifndef ANALOGIO_H
#define ANALOGIO_H
#include <Header/userio_io.h>
class AnalogIO:public IO
{
public:
    virtual void initializeIOPinsFunctions()=0;
    virtual void initializeIODefaultFunctions()=0;
    virtual void initializeIOPinsInformation()=0;
    virtual void getDataFromDBUsingIds(QStringList)=0;
    virtual void refreshModel()=0;
    virtual void saveDataToStructs(int a_Index,int a_Function,int a_CheckboxStatus)=0;
protected:
    void getQListOfSelectedItemInAnalogParamsModel(UserIOAnalogParamsModel *temp_UserIOModelSelected, QList<QObject *> &qobjectListSelected);
    int  updateAnalogParamsModelComboBoxListIfNoneSelected(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected);
    void setCurrentIndexInAnalogParamsModelComboboxLists(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected, int actualIndex);


};
#endif // ANALOGIO_H
