/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Digital IO

***************************************************************************/
#include <Header/userio_digitalio.h>

/**
 * @brief DigitalIO::setCurrentIndexInDigitalParamsModelComboboxLists : Set the current index to model
 * @param temp_UserIODigitalParamsModelIfNoneSelected
 * @param qobjectListIfNoneSelected
 * @param actualIndex
 */
void DigitalIO::setCurrentIndexInDigitalParamsModelComboboxLists(UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected, int actualIndex)
{
    int indexToSelect=searchIndexInComboboxlist(qobjectListIfNoneSelected,actualIndex);
    if(indexToSelect!=-1)
    {
        temp_UserIODigitalParamsModelIfNoneSelected->setCurrentIndexCombobox(indexToSelect);
        temp_UserIODigitalParamsModelIfNoneSelected->setComboBoxList(QVariant::fromValue(qobjectListIfNoneSelected));
    }
}

/**
 * @brief DigitalIO::updateDigitalParamsModelComboBoxListIfNoneSelected : Return the Information values is not changed
 * @param temp_UserIODigitalParamsModelIfNoneSelected
 * @param qobjectListIfNoneSelected
 * @return
 */
int DigitalIO::updateDigitalParamsModelComboBoxListIfNoneSelected(UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneSelected, QList<QObject*> &qobjectListIfNoneSelected)
{
    int actualIndex=0;
    if (temp_UserIODigitalParamsModelIfNoneSelected->getComboBoxList().canConvert(QMetaType::QVariantList))
    {
        QSequentialIterable temp_iterableIfNoneSelected = temp_UserIODigitalParamsModelIfNoneSelected->getComboBoxList().value<QSequentialIterable>();
        int counterIfNoneSelected=0;
        //getting the QList of model of combobox
        foreach (const QVariant& temp_ItemIfNoneSelected, temp_iterableIfNoneSelected)
        {
            QObject* objectIfNoneSelected= temp_ItemIfNoneSelected.value<QObject*>();
            if (objectIfNoneSelected)
            {
                UserIOComboboxModel *temp_UserIOComboboxModelIfNoneSelected=dynamic_cast<UserIOComboboxModel *>(objectIfNoneSelected);
                if(counterIfNoneSelected==temp_UserIODigitalParamsModelIfNoneSelected->getCurrentIndexCombobox())
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
 * @brief DigitalIO::getQListOfSelectedItemInDigitalParamsModel : selected item need to checked
 * @param temp_UserIODigitalParamsModelSelected
 * @param qobjectListSelected
 */
void DigitalIO::getQListOfSelectedItemInDigitalParamsModel(UserIODigitalParamsModel *temp_UserIODigitalParamsModelSelected, QList<QObject *> &qobjectListSelected)
{
    if (temp_UserIODigitalParamsModelSelected->getComboBoxList().canConvert(QMetaType::QVariantList))
    {
        QSequentialIterable temp_iterable = temp_UserIODigitalParamsModelSelected->getComboBoxList().value<QSequentialIterable>();

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
