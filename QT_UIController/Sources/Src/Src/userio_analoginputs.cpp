/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Analog Inputs

***************************************************************************/
#include <Header/userio_analoginputs.h>

/**
 * @brief AnalogInputs::setDefaultValuesForAnalogInputParams : Set the default values fron analog Inputs
 */
void AnalogInputs::setDefaultValuesForAnalogInputParams()
{
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        UserIOAnalogParamsModel *temp_UserIOAnalogParamsModel=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
        //setting current option to "Not Used" so that all options will be available for all pins
        updateAnalogInputsModelList(index,NOT_USED,temp_UserIOAnalogParamsModel->getIsChecked());
    }
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        UserIOAnalogParamsModel *temp_UserIOAnalogParamsModel=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
        QList<QObject*> qobjectListSelected;
        getQListOfSelectedItemInAnalogParamsModel(temp_UserIOAnalogParamsModel,qobjectListSelected);
        int indexToSelect=0;
        indexToSelect=searchIndexInComboboxlist(qobjectListSelected,m_DefaultPinsFunctionsList.at(index));
        if(indexToSelect!=-1)
        {   //one by one setting default values and removing from rest of the comboboxes
            bool bIsChecked = IS_CHECKBOX_CHECKED;
            if(indexToSelect == 0)
                bIsChecked = !IS_CHECKBOX_CHECKED;
            updateAnalogInputsModelList(index,indexToSelect,bIsChecked);
        }
    }
    refreshModel();
    //context->setContextProperty("analogInputParamsPS",QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief AnalogInputs::updatePinsInformationFromDBData : Update the infoprmation fron Database
 * @param a_DataList
 */
void AnalogInputs::updatePinsInformationFromDBData(QList<PinData> a_DataList)
{
    for(int index=0;index<a_DataList.size();index++)
    {  //setting current option to "Not Used" so that all options will be available for all pins
        UserIOAnalogParamsModel *temp_UserIOAnalogParamsModel=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
        updateAnalogInputsModelList(index,NOT_USED,temp_UserIOAnalogParamsModel->getIsChecked());
    }
    for(int index=0;index<a_DataList.size();index++)
    {
        UserIOAnalogParamsModel *temp_UserIOAnalogParamsModel=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
        QList<QObject*> qobjectListSelected;
        getQListOfSelectedItemInAnalogParamsModel(temp_UserIOAnalogParamsModel,qobjectListSelected);
        int indexToSelect=0;
        indexToSelect=searchIndexInComboboxlist(qobjectListSelected,a_DataList.at(index).getFunction());
        if(indexToSelect!=-1)
        {   //one by one setting "a_DataList" values and removing from rest of the comboboxes
            updateAnalogInputsModelList(index,indexToSelect,a_DataList.at(index).getCheckStatus());
        }
    }
    refreshModel();
}

/**
 * @brief AnalogInputs::updateAnalogInputParamsModelComboBoxListIfNoneNotSelected
 * @param temp_UserIOAnalogParamsModelIfNoneNotSelected
 * @param qobjectListIfNoneNotSelected
 * @param indexToRemove
 * @return
 */
int AnalogInputs::updateAnalogInputParamsModelComboBoxListIfNoneNotSelected(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneNotSelected, QList<QObject*> &qobjectListIfNoneNotSelected,int indexToRemove)
{
    int actualIndex=0;
    if (temp_UserIOAnalogParamsModelIfNoneNotSelected->getComboBoxList().canConvert(QMetaType::QVariantList))
    {
        QSequentialIterable temp_iterableIfNoneNotSelected = temp_UserIOAnalogParamsModelIfNoneNotSelected->getComboBoxList().value<QSequentialIterable>();
        int counterIfNoneNotSelected=0;
        //getting the QList of model of combobox
        foreach (const QVariant& temp_ItemIfNoneNotSelected, temp_iterableIfNoneNotSelected)
        {
            QObject* objectIfNoneNotSelected = temp_ItemIfNoneNotSelected.value<QObject*>();
            if (objectIfNoneNotSelected)
            {
                UserIOComboboxModel *temp_UserIOComboboxModelIfNoneNotSelected=dynamic_cast<UserIOComboboxModel *>(objectIfNoneNotSelected);
                if(counterIfNoneNotSelected==temp_UserIOAnalogParamsModelIfNoneNotSelected->getCurrentIndexCombobox())
                {
                    //getting actual index of selected index (old index)
                    actualIndex=temp_UserIOComboboxModelIfNoneNotSelected->getActualIndex();
                }
                //find the selected parameter in other comboboxes with the help of actual index and when it is found,
                //will not be added to the QList model
                if(temp_UserIOComboboxModelIfNoneNotSelected->getActualIndex()!=indexToRemove||(isOptionAvailableInComboBox(temp_UserIOAnalogParamsModelIfNoneNotSelected,indexToRemove)))
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
    return actualIndex;
}

/**
 * @brief AnalogInputs::updateAnalogInputParamsComboboxList
 * @param qobjectListIfNoneSelected
 * @param previousSelectedActualIndex
 * @param previousSelectedIndex
 */
void AnalogInputs::updateAnalogInputParamsComboboxList(QList<QObject*> &qobjectListIfNoneSelected, int previousSelectedActualIndex, int previousSelectedIndex)
{
    QObject* qobjectCopyToAddAfterSelectingNone=copyIOPinsFunctionsList(m_IOPinsFunctionsList).at(previousSelectedActualIndex);
    if(previousSelectedIndex!=0)
    {
        //adding element to list
        qobjectListIfNoneSelected.append(qobjectCopyToAddAfterSelectingNone);
    }
}

/**
 * @brief AnalogInputs::updateAnalogInputsModelList : Update the Model
 * @param a_ListViewIndex
 * @param a_ComboboxIndex
 * @param a_CheckBoxState

 */
void AnalogInputs::updateAnalogInputsModelList(int a_ListViewIndex,int a_ComboboxIndex,bool a_CheckBoxState)
{
    UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(a_ListViewIndex));
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    getQListOfSelectedItemInAnalogParamsModel(temp_UserIOAnalogParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(a_ComboboxIndex));
    int indexToRemove=temp_UserIOComboboxModelSelected->getActualIndex();
    int temp_CurrentIndex=temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox();
    saveDataToStructs(a_ListViewIndex,indexToRemove,a_CheckBoxState);
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        //if Parameter other than None is selected
        if(index!=a_ListViewIndex&&indexToRemove!=0)
        {
            UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneNotSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
            QList<QObject*> qobjectListIfNoneNotSelected;
            int actualIndex=0;
            actualIndex=updateAnalogInputParamsModelComboBoxListIfNoneNotSelected(temp_UserIOAnalogParamsModelIfNoneNotSelected, qobjectListIfNoneNotSelected,indexToRemove);
            int previousSelectedIndex=temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox();
            int previousSelectedActualIndex=(dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(previousSelectedIndex)))->getActualIndex();
            if(searchIndexInComboboxlist(qobjectListIfNoneNotSelected,previousSelectedActualIndex)==-1)
            {
                if(temp_CurrentIndex!=a_ComboboxIndex/*&&isOptionAvailableInOtherComboBoxes(a_ListViewIndex,previousSelectedActualIndex,a_CheckBoxState)*/)
                {
                    updateAnalogInputParamsComboboxList(qobjectListIfNoneNotSelected, previousSelectedActualIndex, previousSelectedIndex);
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
            setCurrentIndexInAnalogParamsModelComboboxLists(temp_UserIOAnalogParamsModelIfNoneNotSelected, qobjectListIfNoneNotSelected, actualIndex);

        }
        //if None is selected
        else if(index!=a_ListViewIndex&&indexToRemove==0)
        {
            UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
            QList<QObject*> qobjectListIfNoneSelected;
            int actualIndex = updateAnalogParamsModelComboBoxListIfNoneSelected(temp_UserIOAnalogParamsModelIfNoneSelected, qobjectListIfNoneSelected);
            int previousSelectedIndex=temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox();
            int previousSelectedActualIndex=(dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(previousSelectedIndex)))->getActualIndex();
            //copy of object, needs to be added in all other comboboxes if none is selected in one combobox
            if(searchIndexInComboboxlist(qobjectListIfNoneSelected,previousSelectedActualIndex)==-1)
            {
                if(temp_CurrentIndex!=a_ComboboxIndex&&(!isOptionAvailableInOtherComboBoxes(a_ListViewIndex,previousSelectedActualIndex)))
                {
                    updateAnalogInputParamsComboboxList(qobjectListIfNoneSelected, previousSelectedActualIndex, previousSelectedIndex);
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
            setCurrentIndexInAnalogParamsModelComboboxLists(temp_UserIOAnalogParamsModelIfNoneSelected, qobjectListIfNoneSelected, actualIndex);
        }
    }
    //set the current index of current combobox
    temp_UserIOAnalogParamsModelSelected->setCurrentIndexCombobox(a_ComboboxIndex);
    //set the checked status of current checkbox
    temp_UserIOAnalogParamsModelSelected->setIsChecked(a_CheckBoxState);
    //refresh the model
    refreshModel();
}

/**
 * @brief AnalogInputs::onUnchecked : on Unchecked status to set he defaults values
 * @param a_ListViewIndex
 * @param a_ComboboxIndex
 * @param a_CheckBoxState
 */

void AnalogInputs::onUnchecked(int a_ListViewIndex,int a_ComboboxIndex,bool a_CheckBoxState)
{
    UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(a_ListViewIndex));
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    getQListOfSelectedItemInAnalogParamsModel(temp_UserIOAnalogParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(a_ComboboxIndex));
    int indexToRemove=temp_UserIOComboboxModelSelected->getActualIndex();
    //int temp_CurrentIndex=temp_UserIOAnalogParamsModelSelected->m_currentIndexCombobox;
    saveDataToStructs(a_ListViewIndex,indexToRemove,a_CheckBoxState);
    if(indexToRemove!=0)
    {
        for(int index=0;index<m_IOPinsInformationList.size();index++)
        {
            //if None is selected
            if(index!=a_ListViewIndex&&indexToRemove!=0)
            {
                UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelIfNoneSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
                QList<QObject*> qobjectListIfNoneSelected;
                int actualIndex = updateAnalogParamsModelComboBoxListIfNoneSelected(temp_UserIOAnalogParamsModelIfNoneSelected, qobjectListIfNoneSelected);
                int previousSelectedIndex=temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox();
                int previousSelectedActualIndex=(dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(previousSelectedIndex)))->getActualIndex();
                //copy of object, needs to be added in all other comboboxes if none is selected in one combobox
                if(searchIndexInComboboxlist(qobjectListIfNoneSelected,previousSelectedActualIndex)==-1)
                {
                    updateAnalogInputParamsComboboxList(qobjectListIfNoneSelected, previousSelectedActualIndex, previousSelectedIndex);
                }
                //sort QList of combobox according to actual index
                std::sort(qobjectListIfNoneSelected.begin(),qobjectListIfNoneSelected.end(),[](QObject * qobjL,QObject * qobjR)
                {
                    int leftI=dynamic_cast<UserIOComboboxModel *>(qobjL)->getActualIndex();
                    int rightI=dynamic_cast<UserIOComboboxModel *>(qobjR)->getActualIndex();
                    return leftI<rightI;
                });
                //determine the new index in updated list
                setCurrentIndexInAnalogParamsModelComboboxLists(temp_UserIOAnalogParamsModelIfNoneSelected, qobjectListIfNoneSelected, actualIndex);
            }
        }
    }
    //set the checked status of current checkbox
    temp_UserIOAnalogParamsModelSelected->setIsChecked(a_CheckBoxState);
    //refresh the model
    refreshModel();
}

/**
 * @brief AnalogInputs::onChecking : checked the status to visible to user
 * @param a_ListViewIndex
 * @param a_ComboboxIndex
 * @param a_CheckBoxState
 */
void AnalogInputs::onChecking(int a_ListViewIndex,int a_ComboboxIndex,bool a_CheckBoxState)
{
    UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(a_ListViewIndex));
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    getQListOfSelectedItemInAnalogParamsModel(temp_UserIOAnalogParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(a_ComboboxIndex));
    int actualIndexToRemove=temp_UserIOComboboxModelSelected->getActualIndex();
    if(actualIndexToRemove!=0)
    {
        if(isOptionAvailableInOtherComboBoxes(a_ListViewIndex,temp_UserIOComboboxModelSelected->getActualIndex()))
        {
            //if current pin function that was unchecked, already selected in any other combobox then current option should
            //be removed from this combobox and should be replaced with "Not Used" on checking
            updateAnalogInputsModelList(a_ListViewIndex, NOT_USED,a_CheckBoxState);
            int indexToRemove=searchIndexInComboboxlist(qobjectListSelected,actualIndexToRemove);
            qobjectListSelected.removeAt(indexToRemove);
            temp_UserIOAnalogParamsModelSelected->setComboBoxList(QVariant::fromValue(qobjectListSelected));
            refreshModel();
        }
        else
        {
            updateAnalogInputsModelList(a_ListViewIndex, a_ComboboxIndex,a_CheckBoxState);
        }

    }
    else
    {
        temp_UserIOAnalogParamsModelSelected->setIsChecked(a_CheckBoxState);
        saveDataToStructs(a_ListViewIndex,NOT_USED,a_CheckBoxState);
        refreshModel();
    }
}

/**
 * @brief AnalogInputs::isOptionAvailableInOtherComboBoxes : OPtional values to display
 * @param a_CurrentIndex
 * @param a_actualIndex
 * @return
 */
bool AnalogInputs::isOptionAvailableInOtherComboBoxes(int a_CurrentIndex,int a_actualIndex)
{
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        if(index!=a_CurrentIndex)
        {
            UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(index));
            //converting qvariant type to QList<QObject*>
            QList<QObject*> qobjectListSelected;
            int temp_currentIndex=temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox();
            int temp_checkStatus=temp_UserIOAnalogParamsModelSelected->getIsChecked();
            getQListOfSelectedItemInAnalogParamsModel(temp_UserIOAnalogParamsModelSelected,qobjectListSelected);
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
 * @brief AnalogInputs::isOptionAvailableInComboBox
 * @param temp_UserIOAnalogParamsModelSelected
 * @param a_actualIndex
 * @return
 */
bool AnalogInputs::isOptionAvailableInComboBox(UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected,int a_actualIndex)
{
    //converting qvariant type to QList<QObject*>
    QList<QObject*> qobjectListSelected;
    int temp_currentIndex=temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox();
    getQListOfSelectedItemInAnalogParamsModel(temp_UserIOAnalogParamsModelSelected,qobjectListSelected);
    UserIOComboboxModel *temp_UserIOComboboxModelSelected=dynamic_cast<UserIOComboboxModel *>(qobjectListSelected.at(temp_currentIndex));
    if((temp_UserIOComboboxModelSelected->getActualIndex()==a_actualIndex))
    {
        return true;
    }
    return false;
}

