/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Analog Outputs

***************************************************************************/
#include <Header/userio_analogoutputs.h>

/**
 * @brief AnalogOutputs::setDefaultValuesForAnalogOutputParams: Set the default values for the analog output
 */
void AnalogOutputs::setDefaultValuesForAnalogOutputParams()
{
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected=
                dynamic_cast<UserIOAnalogParamsModel *>(m_IOPinsInformationList.at(index));
        temp_UserIOAnalogParamsModelSelected->setCurrentIndexCombobox(m_DefaultPinsFunctionsList.at(index));
        bool bIsChecked = IS_CHECKBOX_CHECKED;
        if(m_DefaultPinsFunctionsList.at(index) == 0)
            bIsChecked = !IS_CHECKBOX_CHECKED;
        temp_UserIOAnalogParamsModelSelected->setIsChecked(bIsChecked);
        saveDataToStructs(index,temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox(),temp_UserIOAnalogParamsModelSelected->getIsChecked());
    }
    refreshModel();
}

/**
 * @brief AnalogOutputs::updatePinsInformationFromDBData : Update the default values from the database
 * @param a_DataList
 */
void AnalogOutputs::updatePinsInformationFromDBData(QList<PinData> a_DataList)
{
    for(int index=0;index<a_DataList.size();index++)
    {
        UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected=
                dynamic_cast<UserIOAnalogParamsModel *>(m_IOPinsInformationList.at(index));
        temp_UserIOAnalogParamsModelSelected->setCurrentIndexCombobox(a_DataList.at(index).getFunction());
        temp_UserIOAnalogParamsModelSelected->setIsChecked(a_DataList.at(index).getCheckStatus());
        saveDataToStructs(index,temp_UserIOAnalogParamsModelSelected->getCurrentIndexCombobox(),temp_UserIOAnalogParamsModelSelected->getIsChecked());
    }
    refreshModel();
}

/**
 * @brief AnalogOutputs::updateAnalogOutputsModelList : Update the model list
 * @param a_ListViewIndex
 * @param a_ComboboxIndex
 * @param a_CheckBoxState
 */
void AnalogOutputs::updateAnalogOutputsModelList(int a_ListViewIndex, int a_ComboboxIndex, bool a_CheckBoxState)
{
    UserIOAnalogParamsModel *temp_UserIOAnalogParamsModelSelected=dynamic_cast<UserIOAnalogParamsModel*>(m_IOPinsInformationList.at(a_ListViewIndex));
    //set the current index of current combobox
    temp_UserIOAnalogParamsModelSelected->setCurrentIndexCombobox(a_ComboboxIndex);
    //set the checked status of current checkbox
    temp_UserIOAnalogParamsModelSelected->setIsChecked(a_CheckBoxState);
    saveDataToStructs(a_ListViewIndex,a_ComboboxIndex,a_CheckBoxState);
    //refresh the model
    refreshModel();
}


