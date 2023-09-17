/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

New Password

***************************************************************************/
#include <QString>
#include <QObject>
#include <QQmlContext>
#include <QDateTime>
#include "Header/communicationinterface.h"
#include "Header/newpassword.h"
#include "Header/sysconfigdata.h"

extern QQmlContext* context ;
extern CommunicationInterface CIObj;
extern NewPassword *m_NewPassword;

/**
 * @brief NewPassword::NewPassword
 * @param parent
 */
NewPassword::NewPassword(QObject *parent) : QObject(parent)
{
}

/**
 * @brief NewPassword::getUserID : Return new password for particular userid
 * @return
 */
QString NewPassword::getUserID()
{
    return m_stNewPassword.m_UserID;
}

/**
 * @brief NewPassword::setUserID : set password for particular ID
 * @param a_UserID
 */
void NewPassword::setUserID(QString a_UserID)
{
    m_stNewPassword.m_UserID=a_UserID;
}

/**
 * @brief NewPassword::getPasswordFromDB : Retrive password from database
 */
void NewPassword::getPasswordFromDB()
{
    QString qstrPassword = "";
    CIObj.sendMessage(REQ_GET_USER_PASSWORD,RES_GET_USER_PASSWORD,m_stNewPassword.m_UserID);
    CIObj.recivedMessage(RES_GET_USER_PASSWORD,qstrPassword);
    m_stNewPassword.m_CurrentPassword=qstrPassword;
}

/**
 * @brief NewPassword::getCurrentPassword : Return new password
 * @return
 */
QString NewPassword::getCurrentPassword()
{
    return m_stNewPassword.m_CurrentPassword;
}

/**
 * @brief NewPassword::setNewPassword : Set new password and Insert the value
 * @param a_NewPassword
 */
void NewPassword::setNewPassword(QString a_NewPassword)
{
    m_stNewPassword.m_CurrentPassword=a_NewPassword;

    QString dateString = SysConfigData::getInstance()->getDateAndTime().toString("yyyy/MM/dd hh:mm:ss");

    QString qstrData = m_stNewPassword.m_UserID + "," + m_stNewPassword.m_CurrentPassword + "," + dateString;
    CIObj.sendMessage(REQ_SET_USER_PASSWORD,RES_SET_USER_PASSWORD,qstrData);
    CIObj.recivedMessage(RES_SET_USER_PASSWORD,qstrData);
}

/**
 * @brief NewPassword::isUserDataValid : Validate userid and passwords
 * @param a_UserID
 * @param a_CurrentPassword
 * @return
 */
bool NewPassword::isUserDataValid(QString a_UserID,QString a_CurrentPassword)
{
    getPasswordFromDB();
    if(a_UserID.compare(m_stNewPassword.m_UserID)==0 &&
            a_CurrentPassword.compare(m_stNewPassword.m_CurrentPassword)==0)
    {
        return true;
    }
    return false;
}

/**
 * @brief NewPassword::isConfirmPasswordSame : Validate both password
 * @param a_NewPassword
 * @param a_ConfirmPassword
 * @return
 */
bool NewPassword::isConfirmPasswordSame(QString a_NewPassword,QString a_ConfirmPassword)
{
    if(a_NewPassword.compare(a_ConfirmPassword)==0)
    {
        return true;
    }
    return false;
}

/**
 * @brief NewPassword::setContext : Update the information
 */
void NewPassword :: setContext()
{
    context->setContextProperty("m_NewPassword" , QVariant :: fromValue(m_NewPassword));
}
