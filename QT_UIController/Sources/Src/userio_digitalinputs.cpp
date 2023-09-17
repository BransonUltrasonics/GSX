/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Digital Inputs

***************************************************************************/
#include <Header/userio_digitalinputs.h>

/**
 * @brief DigitalInputs::setDefaultValuesForDigitalInputParams : set the values paramater to model data
 */
void DigitalInputs::setDefaultValuesForDigitalInputParams()
{
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        UserIODigitalParamsModel *temp_UserIODigitalParamsModel=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
        updateDigitalInputsModelList(index,NOT_USED,temp_UserIODigitalParamsModel->getIsChecked(),temp_UserIODigitalParamsModel->getActiveStatus());
    }
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        UserIODigitalParamsModel *temp_UserIODigitalParamsModel=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
        QList<QObject*> qobjectListSelected;
        getQListOfSelectedItemInDigitalParamsModel(temp_UserIODigitalParamsModel,qobjectListSelected);
        int indexToSelect=0;
        indexToSelect=searchIndexInComboboxlist(qobjectListSelected,m_DefaultPinsFunctionsList.at(index));
        if(indexToSelect!=-1)
        {
            temp_UserIODigitalParamsModel->setActiveStatus(temp_UserIODigitalParamsModel->getIsEnabled()&&DEFAULT_ACTIVE_STATUS);
            bool bIsChecked = IS_CHECKBOX_CHECKED;
            if(indexToSelect == 0)
                bIsChecked = !IS_CHECKBOX_CHECKED;

            //TODO: If list size changes,need to change the code as well
            if(m_IOPinsInformationList.size() == 9 && m_DefaultPinsFunctionsList.at(index) == HOLD_DELAY)
                bIsChecked = !IS_CHECKBOX_CHECKED;
            if(m_IOPinsInformationList.size() == 5 && m_DefaultPinsFunctionsList.at(index) == PARTPRESENT)
                bIsChecked = !IS_CHECKBOX_CHECKED;

            updateDigitalInputsModelList(index,indexToSelect,bIsChecked,temp_UserIODigitalParamsModel->getActiveStatus());
        }
    }
    // context->setContextProperty("digitalInputParamsPS",QVariant::fromValue(m_IOPinsInformationList));
    refreshModel();
}

/**
 * @brief DigitalInputs::updatePinsInformationFromDBData : Updating the information fetch fom database
 * @param a_DataList
 */
void DigitalInputs::updatePinsInformationFromDBData(QList<PinData> a_DataList)
{
    for(int index=0;index<a_DataList.size();index++)
    {
        UserIODigitalParamsModel *temp_UserIODigitalParamsModel=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
        updateDigitalInputsModelList(index,0,temp_UserIODigitalParamsModel->getIsChecked(),temp_UserIODigitalParamsModel->getActiveStatus());
    }
    for(int index=0;index<a_DataList.size();index++)
    {
        UserIODigitalParamsModel *temp_UserIODigitalParamsModel=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
        QList<QObject*> qobjectListSelected;
        getQListOfSelectedItemInDigitalParamsModel(temp_UserIODigitalParamsModel,qobjectListSelected);
        int indexToSelect=0;
        indexToSelect=searchIndexInComboboxlist(qobjectListSelected,a_DataList.at(index).getFunction());
        if(indexToSelect!=-1)
        {
            updateDigitalInputsModelList(index,indexToSelect,a_DataList.at(index).getCheckStatus(),a_DataList.at(index).getActiveStatus());
        }
    }
    refreshModel();
    // context->setContextProperty("digitalInputParamsPS",QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief DigitalInputs::updateDigitalInputParamsModelComboBoxListIfNoneNotSelected
 * @param temp_UserIODigitalParamsModelIfNoneNotSelected
 * @param qobjectListIfNoneNotSelected
 * @param indexToRemove
 * @return
 */
int DigitalInputs::updateDigitalInputParamsModelComboBoxListIfNoneNotSelected(UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneNotSelected, QList<QObject*> &qobjectListIfNoneNotSelected,int indexToRemove)
{
    int actualIndex=0;
    if (temp_UserIODigitalParamsModelIfNoneNotSelected->getComboBoxList().canConvert(QMetaType::QVariantList))
    {
        QSequentialIterable temp_iterableIfNoneNotSelected = temp_UserIODigitalParamsModelIfNoneNotSelected->getComboBoxList().value<QSequentialIterable>();
        int counterIfNoneNotSelected=0;
        //getting the QList of model of combobox
        foreach (const QVariant& temp_ItemIfNoneNotSelected, temp_iterableIfNoneNotSelected)
        {
            QObject* objectIfNoneNotSelected = temp_ItemIfNoneNotSelected.value<QObject*>();
            if (objectIfNoneNotSelected)
            {
                UserIOComboboxModel *temp_UserIOComboboxModelIfNoneNotSelected=dynamic_cast<UserIOComboboxModel *>(objectIfNoneNotSelected);
                if(counterIfNoneNotSelected==temp_UserIODigitalParamsModelIfNoneNotSelected->getCurrentIndexCombobox())
                {
                    //getting actual index of selected index (old index)
                    actualIndex=temp_UserIOComboboxModelIfNoneNotSelected->getActualIndex();
                }
                //find the selected parameter in other comboboxes with the help of actual index and when it is found,
                //will not be added to the QList model
                if(temp_UserIOComboboxModelIfNoneNotSelected->getActualIndex()!=indexToRemove||(isOptionAvailableInComboBox(temp_UserIODigitalParamsModelIfNoneNotSelected,indexToRemove)))
                {
                    qobjectListIfNoneNotSelected.append(objectIfNoneNotSelected);
                }
                else if(temp_UserIOComboboxModelIfNoneNotSelected->getActualIndex()==indexToRemove)
                {
                    m_GarbageComboboxList.push_back(objectIfNoneNotSelected);
                }
            }
            counterIfNoneNotSelected++;
        }
    }
    //determine the new index in updated list
    return actualIndex;
}

/**
 * @brief DigitalInputs::updateDigitalInputParamsComboboxList
 * @param qobjectListIfNoneSelected
 * @param previousSelectedActualIndex
 * @param previousSelectedIndex
 */
void DigitalInputs::updateDigitalInputParamsComboboxList(QList<QObject*> &qobjectListIfNoneSelected, int previousSelectedActualIndex, int previousSelectedIndex)
{
    QObject* qobjectCopyToAddAfterSelectingNone=copyIOPinsFunctionsList(m_IOPinsFunctionsList).at(previousSelectedActualIndex);
    if(previousSelectedIndex!=0)
    {
        qobjectListIfNoneSelected.append(qobjectCopyToAddAfterSelectingNone);
    }
}

/**
 * @brief DigitalInputs::updateDigitalInputsModelList
 * @param a_ListViewIndex
 * @param a_ComboboxIndex
 * @param a_CheckBoxState
 * @param a_ActiveStatus
 */
void DigitalInputs::updateDigitalInputsModelList(int a_ListViewIndex,int a_ComboboxIndex,bool a_CheckBoxState,bool a_ActiveStatus)
{
    UserIODigitalParamsModel *temp_UserIODigitalParamsModelSelected=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(a_ListViewIndex));
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    getQListOfSelectedItemInDigitalParamsModel(temp_UserIODigitalParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(a_ComboboxIndex));
    int indexToRemove=temp_UserIOComboboxModelSelected->getActualIndex();
    int temp_CurrentIndex=temp_UserIODigitalParamsModelSelected->getCurrentIndexCombobox();
    saveDataToStructs(a_ListViewIndex,indexToRemove,a_ActiveStatus,a_CheckBoxState);
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        //if Parameter other than None is selected
        if(index!=a_ListViewIndex&&indexToRemove!=0)
        {
            UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneNotSelected=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
            QList<QObject*> qobjectListIfNoneNotSelected;
            int actualIndex=0;
            actualIndex=updateDigitalInputParamsModelComboBoxListIfNoneNotSelected(temp_UserIODigitalParamsModelIfNoneNotSelected, qobjectListIfNoneNotSelected,indexToRemove);
            int previousSelectedIndex=temp_UserIODigitalParamsModelSelected->getCurrentIndexCombobox();
            int previousSelectedActualIndex=(dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(previousSelectedIndex)))->getActualIndex();
            if(searchIndexInComboboxlist(qobjectListIfNoneNotSelected,previousSelectedActualIndex)==-1){
                if(temp_CurrentIndex!=a_ComboboxIndex/*&&isOptionAvailableInOtherComboBoxes(a_ListViewIndex,previousSelectedActualIndex,a_CheckBoxState)*/)
                {
                    updateDigitalInputParamsComboboxList(qobjectListIfNoneNotSelected, previousSelectedActualIndex, previousSelectedIndex);
                }
            }
            //sort QList of combobox according to actual index
            std::sort(qobjectListIfNoneNotSelected.begin(),qobjectListIfNoneNotSelected.end(),[](QObject * qobjL,QObject * qobjR)
            {
                int leftI=dynamic_cast<UserIOComboboxModel *>(qobjL)->getActualIndex();
                int rightI=dynamic_cast<UserIOComboboxModel *>(qobjR)->getActualIndex();
                return leftI<rightI;
            });
            //determine the new index in updated list
            setCurrentIndexInDigitalParamsModelComboboxLists(temp_UserIODigitalParamsModelIfNoneNotSelected, qobjectListIfNoneNotSelected, actualIndex);
        }
        //if None is selected
        else if(index!=a_ListViewIndex&&indexToRemove==0)
        {
            UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneSelected=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
            QList<QObject*> qobjectListIfNoneSelected;
            int actualIndex = updateDigitalParamsModelComboBoxListIfNoneSelected(temp_UserIODigitalParamsModelIfNoneSelected, qobjectListIfNoneSelected);
            int previousSelectedIndex=temp_UserIODigitalParamsModelSelected->getCurrentIndexCombobox();
            int previousSelectedActualIndex=(dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(previousSelectedIndex)))->getActualIndex();
            //copy of object, needs to be added in all other comboboxes if none is selected in one combobox
            if(searchIndexInComboboxlist(qobjectListIfNoneSelected,previousSelectedActualIndex)==-1)
            {
                if(temp_CurrentIndex!=a_ComboboxIndex&&(!isOptionAvailableInOtherComboBoxes(a_ListViewIndex,previousSelectedActualIndex)))
                {
                    updateDigitalInputParamsComboboxList(qobjectListIfNoneSelected, previousSelectedActualIndex, previousSelectedIndex);
                }
            }
            //sort QList of combobox according to actual index
            std::sort(qobjectListIfNoneSelected.begin(),qobjectListIfNoneSelected.end(),[](QObject * qobjL,QObject * qobjR)
            {
                int leftI=dynamic_cast<UserIOComboboxModel *>(qobjL)->getActualIndex();
                int rightI=dynamic_cast<UserIOComboboxModel *>(qobjR)->getActualIndex();
                return leftI<rightI;
            });
            //determine the new index in updated list
            setCurrentIndexInDigitalParamsModelComboboxLists(temp_UserIODigitalParamsModelIfNoneSelected, qobjectListIfNoneSelected, actualIndex);
        }
    }
    //set the current index of current combobox
    temp_UserIODigitalParamsModelSelected->setCurrentIndexCombobox(a_ComboboxIndex);
    //set the checked status of current checkbox
    temp_UserIODigitalParamsModelSelected->setIsChecked(a_CheckBoxState);
    //set the switch status
    temp_UserIODigitalParamsModelSelected->setActiveStatus(a_ActiveStatus);
    //refresh the model
    refreshModel();
}

/**
 * @brief DigitalInputs::onUnchecked
 * @param a_ListViewIndex
 * @param a_ComboboxIndex
 * @param a_CheckBoxState
 * @param a_ActiveStatus
 */
void DigitalInputs::onUnchecked(int a_ListViewIndex,int a_ComboboxIndex,bool a_CheckBoxState,bool a_ActiveStatus)
{
    UserIODigitalParamsModel *temp_UserIODigitalParamsModelSelected=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(a_ListViewIndex));
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    getQListOfSelectedItemInDigitalParamsModel(temp_UserIODigitalParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(a_ComboboxIndex));
    int indexToRemove=temp_UserIOComboboxModelSelected->getActualIndex();
    //int temp_CurrentIndex=temp_UserIODigitalParamsModelSelected->m_currentIndexCombobox;
    saveDataToStructs(a_ListViewIndex,indexToRemove,a_ActiveStatus,a_CheckBoxState);
    if(indexToRemove!=0)
    {
        for(int index=0;index<m_IOPinsInformationList.size();index++)
        {
            //if None is selected
            if(index!=a_ListViewIndex&&indexToRemove!=0)
            {
                UserIODigitalParamsModel *temp_UserIODigitalParamsModelIfNoneSelected=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
                QList<QObject*> qobjectListIfNoneSelected;
                int actualIndex = updateDigitalParamsModelComboBoxListIfNoneSelected(temp_UserIODigitalParamsModelIfNoneSelected, qobjectListIfNoneSelected);
                int previousSelectedIndex=temp_UserIODigitalParamsModelSelected->getCurrentIndexCombobox();
                int previousSelectedActualIndex=(dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(previousSelectedIndex)))->getActualIndex();
              //copy of object, needs to be added in all other comboboxes if none is selected in one combobox
                if(searchIndexInComboboxlist(qobjectListIfNoneSelected,previousSelectedActualIndex)==-1)
                {
                    updateDigitalInputParamsComboboxList(qobjectListIfNoneSelected, previousSelectedActualIndex, previousSelectedIndex);
                }
                //sort QList of combobox according to actual index
                std::sort(qobjectListIfNoneSelected.begin(),qobjectListIfNoneSelected.end(),[](QObject * qobjL,QObject * qobjR)
                {
                    int leftI=dynamic_cast<UserIOComboboxModel *>(qobjL)->getActualIndex();
                    int rightI=dynamic_cast<UserIOComboboxModel *>(qobjR)->getActualIndex();
                    return leftI<rightI;
                });
                //determine the new index in updated list
                setCurrentIndexInDigitalParamsModelComboboxLists(temp_UserIODigitalParamsModelIfNoneSelected, qobjectListIfNoneSelected, actualIndex);
            }
        }
    }
    //set the current index of current combobox
    //temp_UserIODigitalParamsModelSelected->m_currentIndexCombobox=a_ComboboxIndex;
    //set the checked status of current checkbox
    temp_UserIODigitalParamsModelSelected->setIsChecked(a_CheckBoxState);
    //set the switch status
    temp_UserIODigitalParamsModelSelected->setActiveStatus(a_ActiveStatus);
    //refresh the model
    refreshModel();
}

/**
 * @brief DigitalInputs::onChecking
 * @param a_ListViewIndex
 * @param a_ComboboxIndex
 * @param a_CheckBoxState
 * @param a_ActiveStatus
 */
void DigitalInputs::onChecking(int a_ListViewIndex,int a_ComboboxIndex,bool a_CheckBoxState,bool a_ActiveStatus)
{
    UserIODigitalParamsModel *temp_UserIODigitalParamsModelSelected=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(a_ListViewIndex));
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    getQListOfSelectedItemInDigitalParamsModel(temp_UserIODigitalParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(a_ComboboxIndex));
    int actualIndexToRemove=temp_UserIOComboboxModelSelected->getActualIndex();
    if(actualIndexToRemove!=0)
    {
        if(isOptionAvailableInOtherComboBoxes(a_ListViewIndex,temp_UserIOComboboxModelSelected->getActualIndex()))
        {
            //m_IOPinsInformationList.erase( m_IOPinsInformationList.begin()+ temp_UserIODigitalParamsModelSelected->m_currentIndexCombobox);
            updateDigitalInputsModelList(a_ListViewIndex, 0,a_CheckBoxState,a_ActiveStatus);
            int indexToRemove=searchIndexInComboboxlist(qobjectListSelected,actualIndexToRemove);
            qobjectListSelected.removeAt(indexToRemove);
            temp_UserIODigitalParamsModelSelected->setComboBoxList(QVariant::fromValue(qobjectListSelected));
            refreshModel();
        }
        else
        {
            updateDigitalInputsModelList(a_ListViewIndex, a_ComboboxIndex,a_CheckBoxState,a_ActiveStatus);
        }
    }
    else
    {
        temp_UserIODigitalParamsModelSelected->setIsChecked(a_CheckBoxState);
        //set the switch status
        temp_UserIODigitalParamsModelSelected->setActiveStatus(a_ActiveStatus);
        saveDataToStructs(a_ListViewIndex,NOT_USED,a_ActiveStatus,a_CheckBoxState);
        refreshModel();
    }
}

/**
 * @brief DigitalInputs::isOptionAvailableInOtherComboBoxes
 * @param a_CurrentIndex
 * @param a_actualIndex
 * @param a_CheckboxStatus
 * @return
 */
bool DigitalInputs::isOptionAvailableInOtherComboBoxes(int a_CurrentIndex,int a_actualIndex)
{
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        if(index!=a_CurrentIndex)
        {
            UserIODigitalParamsModel *temp_UserIODigitalParamsModelSelected=dynamic_cast<UserIODigitalParamsModel*>(m_IOPinsInformationList.at(index));
            //converting qvariant type to QList<QObject*>
            QList<QObject*> qobjectListSelected;
            int temp_currentIndex=temp_UserIODigitalParamsModelSelected->getCurrentIndexCombobox();
            int temp_checkStatus=temp_UserIODigitalParamsModelSelected->getIsChecked();
            getQListOfSelectedItemInDigitalParamsModel(temp_UserIODigitalParamsModelSelected,qobjectListSelected);
            UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(temp_currentIndex));
            if((temp_UserIOComboboxModelSelected->getActualIndex()==a_actualIndex)&&(temp_checkStatus))
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief DigitalInputs::isOptionAvailableInComboBox
 * @param temp_UserIODigitalParamsModelSelected
 * @param a_actualIndex
 * @return
 */
bool DigitalInputs::isOptionAvailableInComboBox(UserIODigitalParamsModel *temp_UserIODigitalParamsModelSelected,int a_actualIndex)
{
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    int temp_currentIndex=temp_UserIODigitalParamsModelSelected->getCurrentIndexCombobox();
    getQListOfSelectedItemInDigitalParamsModel(temp_UserIODigitalParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(temp_currentIndex));
    if((temp_UserIOComboboxModelSelected->getActualIndex()==a_actualIndex))
    {
        return true;
    }
    return false;
}

