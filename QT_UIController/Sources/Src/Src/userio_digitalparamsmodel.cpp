/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Digital Parameter's Model

***************************************************************************/
#include "Header/userio_digitalparamsmodel.h"

/**
 * @brief UserIODigitalParamsModel::getTitle : Return the title of the Pin
 * @return
 */
QString UserIODigitalParamsModel::getTitle()
{
    return m_Title;
}

/**
 * @brief UserIODigitalParamsModel::setTitle : Set the Title of the Pin
 * @param a_Title
 */
void UserIODigitalParamsModel::setTitle(QString a_Title)
{
    m_Title=a_Title;
}

/**
 * @brief UserIODigitalParamsModel::getIsChecked : Return the checked status
 * @return
 */
bool UserIODigitalParamsModel::getIsChecked()
{
    return m_IsChecked;
}

/**
 * @brief UserIODigitalParamsModel::setIsChecked : Set the Checked status
 * @param a_IsChecked
 */
void UserIODigitalParamsModel::setIsChecked(bool a_IsChecked)
{
    m_IsChecked=a_IsChecked;
    emit isCheckedChanged();
}

/**
 * @brief UserIODigitalParamsModel::getComboBoxList : Return the Combobox List
 * @return
 */
QVariant UserIODigitalParamsModel::getComboBoxList()
{
return m_ComboBoxList;
}

/**
 * @brief UserIODigitalParamsModel::setComboBoxList : Se the Combobox List
 * @param a_ComboBoxList
 */
void UserIODigitalParamsModel::setComboBoxList(QVariant a_ComboBoxList)
{
    m_ComboBoxList=a_ComboBoxList;
    emit comboBoxListChanged();
}

/**
 * @brief UserIODigitalParamsModel::getActiveStatus : Get the Active Status value
 * @return
 */
bool UserIODigitalParamsModel::getActiveStatus()
{
    return m_ActiveStatus;
}

/**
 * @brief UserIODigitalParamsModel::setActiveStatus : Set Active status value
 * @param a_ActiveStatus
 */
void UserIODigitalParamsModel::setActiveStatus(bool a_ActiveStatus)
{
    m_ActiveStatus=a_ActiveStatus;
    emit ActiveStatusChanged();
}

/**
 * @brief UserIODigitalParamsModel::getCurrentIndexCombobox : Particular dropdown selected index
 * @return
 */
int UserIODigitalParamsModel::getCurrentIndexCombobox()
{
    return m_currentIndexCombobox;
}

/**
 * @brief UserIODigitalParamsModel::setCurrentIndexCombobox : Set dropdown particular index
 * @param a_CurrentIndexCombobox
 */
void UserIODigitalParamsModel::setCurrentIndexCombobox(int a_CurrentIndexCombobox)
{
    m_currentIndexCombobox=a_CurrentIndexCombobox;
    emit CurrentIndexComboboxChanged();
}

/**
 * @brief UserIOAnalogParamsModel::getIsEnabled : Get the Enable status
 * @return
 */
bool UserIODigitalParamsModel::getIsEnabled()
{
    return m_IsEnabled;
}

/**
 * @brief UserIOAnalogParamsModel::setIsEnabled : Set the Enable status
 * @param a_IsEnabled
 */

void UserIODigitalParamsModel::setIsEnabled(bool a_IsEnabled)
{
    m_IsEnabled=a_IsEnabled;
    IsEnabledChanged();
}

/**
 * @brief UserIODigitalParamsModel::UserIODigitalParamsModel : Digital Parameter model to set and Intiazile the values
 * @param a_Title
 * @param a_IsChecked
 * @param a_CurrentIndexCombobox
 * @param a_ComboBoxList
 * @param a_ActiveStatus
 * @param parent
 */
UserIODigitalParamsModel::UserIODigitalParamsModel(QString a_Title, bool a_IsChecked,int a_CurrentIndexCombobox, QVariant a_ComboBoxList, bool a_ActiveStatus,bool a_IsEnabled, QObject *parent):QObject(parent)
{
    m_Title=a_Title;
    m_IsChecked=a_IsChecked;
    m_ComboBoxList=a_ComboBoxList;
    m_ActiveStatus=a_ActiveStatus;
    m_currentIndexCombobox=a_CurrentIndexCombobox;
    m_IsEnabled=a_IsEnabled;
}


