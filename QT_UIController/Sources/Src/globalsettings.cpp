/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Updates global settings in UI from DB and vice versa

***************************************************************************/

#include <QQmlContext>
#include <QQuickView>
#include "Header/communicationinterface.h"
#include "Header/globalsettings.h"
#include "Header/sysconfigdata.h"
extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief GlobalSettings::GlobalSettings : Constructor, Initialization of class members and memory for object
 * @param parent
 */
GlobalSettings::GlobalSettings(QObject *parent)
    : m_passwordExpiry("90")
    , m_idealLogoffTime("30")
{
    Q_UNUSED(parent)
    context ->setContextProperty("qmlGlobalSettingsObject", this);
}

/**
 * @brief GlobalSettings::handleGlobalSettings : Reads global settings from DB and updates in variables
 */
void GlobalSettings::handleGlobalSettings()
{
    QString userResponse = "";
    QString reqData = "";
    SysConfigData *sysconfig = SysConfigData::getInstance();

    /* Send message to DB */
    CIObj.sendMessage(REQ_GET_GLOBAL_SETTINGS, RES_GET_GLOBAL_SETTINGS, reqData);
    CIObj.recivedMessage(RES_GET_GLOBAL_SETTINGS, userResponse);

    QStringList authorityLevelList = userResponse.split(",");
    if (!userResponse.isEmpty() && (authorityLevelList.size()%3)==0)
    {
        m_passwordExpiry = authorityLevelList[0];
        if(m_passwordExpiry.toInt() == 0)
            m_passwordExpiry = "90";
        m_idealLogoffTime = authorityLevelList[1];
        if(m_idealLogoffTime.toInt() == 0)
            m_idealLogoffTime = "30";
        sysconfig->m_GlobalCheckFlag = authorityLevelList[2].toInt();
    }
    else
    {
        m_passwordExpiry = "90";
        m_idealLogoffTime = "30";
        sysconfig->m_GlobalCheckFlag = false;
    }
}

/**
 * @brief GlobalSettings::updateGlobalSettings : Sets modified global settings in to DB
 * @param passwordExpiry
 * @param idealLogoffTime
 */
void GlobalSettings::updateGlobalSettings(QString passwordExpiry, QString idealLogoffTime)
{
    m_passwordExpiry = passwordExpiry;
    m_idealLogoffTime = idealLogoffTime;
    QString userResponse = "";
    QString reqData = "";
    SysConfigData *sysconfig = SysConfigData::getInstance();

    /* Send message to DB */
    CIObj.sendMessage(REQ_GET_GLOBAL_SETTINGS, RES_GET_GLOBAL_SETTINGS, reqData);
    CIObj.recivedMessage(RES_GET_GLOBAL_SETTINGS, userResponse);

    QStringList authorityLevelList = userResponse.split(",");
    if (!userResponse.isEmpty() && (authorityLevelList.size()%3)==0)
    {
        sysconfig->m_GlobalCheckFlag = authorityLevelList[2].toInt();
    }

    reqData = "PASSWORD_EXPIRY_IN_DAYS = " + passwordExpiry
            + " , IDEAL_TIME_LOGOGG_IN_MIN = " + idealLogoffTime + " , PASSWORD_OVERRIDE = " + QString::number(sysconfig->m_GlobalCheckFlag);

    /* Send messages to DB */
    CIObj.sendMessage(REQ_SET_GLOBAL_SETTINGS, RES_SET_GLOBAL_SETTINGS, reqData);
    CIObj.recivedMessage(RES_SET_GLOBAL_SETTINGS, userResponse);
}

/**
 * @brief GlobalSettings::setPasswordExpiryInDays : Sets password expiry in days
 * @param passwrodExpiry
 */
void GlobalSettings::setPasswordExpiryInDays(QString passwrodExpiry)
{
    m_passwordExpiry = passwrodExpiry;
}

/**
 * @brief GlobalSettings::setIdealLogoffTime : Sets ideal logout time
 * @param idealLogoffTime
 */
void GlobalSettings::setIdealLogoffTime(QString idealLogoffTime)
{
    m_idealLogoffTime = idealLogoffTime;
}

/**
 * @brief GlobalSettings::passwordExpiryInDays : Returns password expiry in days
 * @return
 */
QString GlobalSettings::passwordExpiryInDays() const
{
    return m_passwordExpiry;
}

/**
 * @brief GlobalSettings::idealLogoffTime : Returns ideal logout time
 * @return
 */
QString GlobalSettings::idealLogoffTime() const
{
    return m_idealLogoffTime;
}
