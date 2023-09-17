/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Combobox's Model

***************************************************************************/
#include "Header/userio_comboboxmodel.h"

/**
 * @brief UserIOComboboxModel::UserIOComboboxModel : Selected index from combobox has been saved
 * @param a_ParamName
 * @param a_Index
 * @param parent
 */
UserIOComboboxModel::UserIOComboboxModel(QString a_ParamName, int a_Index, QObject *parent):QObject(parent)
{
    m_ParamName=a_ParamName;
    m_ActualIndex=a_Index;
}

/**
 * @brief UserIOComboboxModel::UserIOComboboxModel
 * @param parent
 */
UserIOComboboxModel::UserIOComboboxModel(QObject *parent) : QObject(parent){}

/**
 * @brief UserIOComboboxModel::UserIOComboboxModel : Parameter Name Combobox Model
 * @param userIO
 * @param parent
 */
UserIOComboboxModel::UserIOComboboxModel(const UserIOComboboxModel &userIO, QObject *parent) : QObject(parent)
{
    m_ParamName=userIO.m_ParamName;
    m_ActualIndex=userIO.m_ActualIndex;
}

/**
 * @brief UserIOComboboxModel::getParamName : Return the paramater name
 * @return
 */
QString UserIOComboboxModel::getParamName()
{
    return m_ParamName;
}

/**
 * @brief UserIOComboboxModel::setParamName : Set the paramater name
 * @param a_ParamName
 */
void UserIOComboboxModel::setParamName(QString a_ParamName)
{
    m_ParamName=a_ParamName;
    emit paramNameChanged();
}

/**
 * @brief UserIOComboboxModel::getActualIndex : Get the paramater index
 * @return
 */
int UserIOComboboxModel::getActualIndex()
{
    return m_ActualIndex;
}

/**
 * @brief UserIOComboboxModel::setActualIndex : Set the paramater index
 * @param a_Index
 */
void UserIOComboboxModel::setActualIndex(int a_Index)
{
    m_ActualIndex=a_Index;
    emit actualIndexChanged();
}
