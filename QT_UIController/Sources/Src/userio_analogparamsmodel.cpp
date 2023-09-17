/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Analog Parameter's Model

***************************************************************************/
#include "Header/userio_analogparamsmodel.h"

/**
 * @brief UserIOAnalogParamsModel::getTitle : Retun the title of Analog parameter
 * @return
 */
QString UserIOAnalogParamsModel::getTitle()
{
    return m_Title;
}

/**
 * @brief UserIOAnalogParamsModel::setTitle : Set the parameter title
 * @param a_Title
 */
void UserIOAnalogParamsModel::setTitle(QString a_Title)
{
    m_Title=a_Title;
}

/**
 * @brief UserIOAnalogParamsModel::getIsChecked : Checking the enabled status
 * @return
 */
bool UserIOAnalogParamsModel::getIsChecked()
{
    return m_IsChecked;
}

/**
 * @brief UserIOAnalogParamsModel::setIsChecked : Set the checked status
 * @param a_IsChecked
 */
void UserIOAnalogParamsModel::setIsChecked(bool a_IsChecked)
{
    m_IsChecked=a_IsChecked;
    emit isCheckedChanged();
}
/**
 * @brief UserIOAnalogParamsModel::getComboBoxList : Get the List of Combobox
 * @return
 */
QVariant UserIOAnalogParamsModel::getComboBoxList()
{
    return m_ComboBoxList;
}

/**
 * @brief UserIOAnalogParamsModel::setComboBoxList : Set the list of combobox
 * @param a_ComboBoxList
 */
void UserIOAnalogParamsModel::setComboBoxList(QVariant a_ComboBoxList)
{
    m_ComboBoxList=a_ComboBoxList;
    emit comboBoxListChanged();
}

/**
 * @brief UserIOAnalogParamsModel::getIsEnabled : Get the enabled status
 * @return
 */
bool UserIOAnalogParamsModel::getIsEnabled()
{
    return m_IsEnabled;
}

/**
 * @brief UserIOAnalogParamsModel::setIsEnabled : Set the enabled status
 * @param a_IsEnabled
 */
void UserIOAnalogParamsModel::setIsEnabled(bool a_IsEnabled)
{
    m_IsEnabled=a_IsEnabled;
    IsEnabledChanged();
}

/**
 * @brief UserIOAnalogParamsModel::getCurrentIndexCombobox : Get the current index of combobox
 * @return
 */
int UserIOAnalogParamsModel::getCurrentIndexCombobox()
{
    return m_currentIndexCombobox;
}

/**
 * @brief UserIOAnalogParamsModel::setCurrentIndexCombobox : Set the current index of combobox
 * @param a_CurrentIndexCombobox
 */
void UserIOAnalogParamsModel::setCurrentIndexCombobox(int a_CurrentIndexCombobox)
{
    m_currentIndexCombobox=a_CurrentIndexCombobox;
    emit CurrentIndexComboboxChanged();
}

/**
 * @brief UserIOAnalogParamsModel::UserIOAnalogParamsModel : Set the paramater values
 * @param a_Title
 * @param a_IsChecked
 * @param a_CurrentIndexCombobox
 * @param a_ComboBoxList
 * @param parent
 */
UserIOAnalogParamsModel::UserIOAnalogParamsModel(QString a_Title, bool a_IsChecked,int a_CurrentIndexCombobox, QVariant a_ComboBoxList,bool a_IsEnabled,QObject *parent):QObject(parent)
{
    m_Title=a_Title;
    m_IsChecked=a_IsChecked;
    m_ComboBoxList=a_ComboBoxList;
    m_IsEnabled=a_IsEnabled;
    m_currentIndexCombobox=a_CurrentIndexCombobox;
}

/**
 * @brief UserIOAnalogParamsModel::UserIOAnalogParamsModel
 * @param parent
 */
UserIOAnalogParamsModel::UserIOAnalogParamsModel(QObject *parent):QObject(parent)
{
}

/**
 * @brief UserIOAnalogParamsModel::UserIOAnalogParamsModel : Assign the values to the particular names
 * @param userio
 * @param parent
 */
UserIOAnalogParamsModel::UserIOAnalogParamsModel(const UserIOAnalogParamsModel &userio, QObject *parent):QObject(parent)
{
    m_Title=userio.m_Title;
    m_IsChecked=userio.m_IsChecked;
    m_ComboBoxList=userio.m_ComboBoxList;
    m_currentIndexCombobox=userio.m_currentIndexCombobox;
    m_IsEnabled=userio.m_IsEnabled;
}

