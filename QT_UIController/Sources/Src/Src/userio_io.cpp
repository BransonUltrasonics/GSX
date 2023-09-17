/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Input Output Base class

***************************************************************************/
#include <Header/userio_io.h>
extern QObject * object;

/**
 * @brief IO::searchIndexInComboboxlist : Find the Index of the value of selected Item in Combobox
 * @param qobjectList
 * @param actualIndex
 * @return
 */
int IO::searchIndexInComboboxlist(QList<QObject*> &qobjectList, int actualIndex)
{
    int leftIndex=0;
    int rightIndex=qobjectList.size();
    while (leftIndex <= rightIndex)
    {
        int midIndex = leftIndex + (rightIndex-leftIndex)/2;
        if(midIndex>=qobjectList.size()||midIndex<0)
            return -1;
        int newCurrentIndex=dynamic_cast<UserIOComboboxModel *>(qobjectList.at(midIndex))->getActualIndex();

        if (newCurrentIndex == actualIndex)
        {
            return midIndex;
        }

        if (newCurrentIndex < actualIndex)
        {
            leftIndex = midIndex + 1;
        }

        else
        {
            rightIndex = midIndex - 1;
        }
    }
    return -1;
}

/**
 * @brief IO::clearGarbage : Clear the old Values from the List
 */
void IO::clearGarbage()
{
    if(m_GarbageComboboxList.size()>0)
    {
        for(int index=0;index<m_GarbageComboboxList.size();index++)
        {
            UserIOComboboxModel *garbage=dynamic_cast<UserIOComboboxModel *>(m_GarbageComboboxList.at(index));
            delete garbage;
        }
        m_GarbageComboboxList.clear();
    }
}

/**
 * @brief IO::getDataFromDB : Get the pin nae and number form data Base
 * @param a_ReqID
 * @param a_ResID
 * @param a_Range
 */
void IO::getDataFromDB(QStringList a_List)
{
    QList <PinData>tempDataList;
    for(int index=0;index<a_List.size();)
    {
        PinData temp_PinData;
        temp_PinData.setFunction(a_List.at(index).toInt());
        temp_PinData.setActiveStatus(a_List.at(index+1).toInt());
        temp_PinData.setCheckStatus(a_List.at(index+2).toInt());
        tempDataList.push_back(temp_PinData);
        index+=3;
    }
    updatePinsInformationFromDBData(tempDataList);
}

/**
 * @brief IO::copyIOPinsFunctionsList : Copy the name to particular list
 * @param a_list
 * @return
 */

QList<QObject *> IO::copyIOPinsFunctionsList(const QList<QObject *> &a_list)
{
    QList<QObject *> comboBoxModelList;
    for(int index=0;index<a_list.size();index++)
    {
        UserIOComboboxModel * model=dynamic_cast<UserIOComboboxModel*>(a_list.at(index));
        comboBoxModelList.append(new UserIOComboboxModel(model->getParamName(),model->getActualIndex()) );
        enginePointer->setObjectOwnership(comboBoxModelList.at(index),QQmlEngine::CppOwnership);
    }
    return comboBoxModelList;
}


