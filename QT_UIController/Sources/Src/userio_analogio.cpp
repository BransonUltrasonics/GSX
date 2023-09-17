/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Analog IO

***************************************************************************/
#include <Header/userio_analogio.h>

/**
 * @brief AnalogIO::getQListOfSelectedItemInAnalogParamsModel : Get the list selected in analog
 * @param temp_UserIOAnalogParamsModelSelected
 * @param qobjectListSelected
 */
void AnalogIO::getQListOfSelectedItemInAnalogParamsModel(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected, QList<QObject *> &qobjectListSelected)
{
    if (temp_UserIOAnalogParamsModelSelected->getComboBoxList().canConvert(QMetaType::QVariantList))
    {
        QSequentialIterable temp_iterable = temp_UserIOAnalogParamsModelSelected->getComboBoxList().value<QSequentialIterable>();
        foreach (const QVariant& temp_Item, temp_iterable)
        {
            QObject* object = temp_Item.value<QObject*>();
            if (object)
            {
                qobjectListSelected.append(object);
            }
        }
    }
}

/**
 * @brief AnalogIO::updateAnalogParamsModelComboBoxListIfNoneSelected
 * @param temp_UserIOAnalogParamsModelIfNoneSelected
 * @param qobjectListIfNoneSelected
 * @return
 */
int AnalogIO::updateAnalogParamsModelComboBoxListIfNoneSelected(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected)
{
    int actualIndex=0;
    if (temp_UserIOAnalogParamsModelIfNoneSelected->getComboBoxList().canConvert(QMetaType::QVariantList))
    {
        QSequentialIterable temp_iterableIfNoneSelected = temp_UserIOAnalogParamsModelIfNoneSelected->getComboBoxList().value<QSequentialIterable>();
        int counterIfNoneSelected=0;
        //getting the QList of model of combobox
        foreach (const QVariant& temp_ItemIfNoneSelected, temp_iterableIfNoneSelected)
        {
            QObject* objectIfNoneSelected= temp_ItemIfNoneSelected.value<QObject*>();
            if (objectIfNoneSelected)
            {
                UserIOComboboxModel *temp_UserIOComboboxModelIfNoneSelected=dynamic_cast<UserIOComboboxModel *>(objectIfNoneSelected);
                if(counterIfNoneSelected==temp_UserIOAnalogParamsModelIfNoneSelected->getCurrentIndexCombobox())
                {
                    //getting actual index of selected index (old index)
                    actualIndex=temp_UserIOComboboxModelIfNoneSelected->getActualIndex();
                }
                qobjectListIfNoneSelected.append(objectIfNoneSelected);
            }
            counterIfNoneSelected++;
        }
    }
    return actualIndex;
}

/**
 * @brief AnalogIO::setCurrentIndexInAnalogParamsModelComboboxLists
 * @param temp_UserIOAnalogParamsModelIfNoneSelected
 * @param qobjectListIfNoneSelected
 * @param actualIndex
 */
void AnalogIO::setCurrentIndexInAnalogParamsModelComboboxLists(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected, int actualIndex)
{
    int indexToSelect=searchIndexInComboboxlist(qobjectListIfNoneSelected,actualIndex);
    if(indexToSelect!=-1)
    {
        temp_UserIOAnalogParamsModelIfNoneSelected->setCurrentIndexCombobox(indexToSelect);
        temp_UserIOAnalogParamsModelIfNoneSelected->setComboBoxList(QVariant::fromValue(qobjectListIfNoneSelected));
    }
}
