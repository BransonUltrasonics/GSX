/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Information Panel

***************************************************************************/
#include "Header/informationpanel.h"

/**
 * @brief InformationPanel::InformationPanel : Constuctor Initialize default username
 * @param parent
 */
InformationPanel::InformationPanel(QObject *parent) : QObject(parent)
{
    m_userName="ADMIN";
    m_machinenumber="Machine: UW_PC";
}

/**
 * @brief InformationPanel::setUsername : Set the Username emit the signal to notify the changes
 * @param Username
 */
void InformationPanel::setUsername(QString Username)
{
    if(m_userName!=Username)
    {
        m_userName=Username;
        emit Usernamechanged();
    }
}

/**
 * @brief InformationPanel::getUsername : Return username
 * @return
 */
QString InformationPanel::getUsername()
{
    return m_userName;
}

/**
 * @brief InformationPanel::setInformationpanelData : Set information machine name
 * @param machine
 */
void InformationPanel::setInformationpanelData(QString machine)
{
    if(m_machinenumber !=machine)
    {
        m_machinenumber=machine;
        emit InformationpanelDataChanged();
    }
}

/**
 * @brief InformationPanel::getInformationpanelData : Return machine number
 * @return
 */
QString InformationPanel::getInformationpanelData()
{
    return m_machinenumber;
}
