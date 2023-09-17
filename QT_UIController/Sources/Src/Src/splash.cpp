/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Splash Screen

***************************************************************************/

#include "Header/splash.h"
#include <QQuickView>
#include "Header/communicationinterface.h"
#include "Header/configuration.h"
#include "Header/alarmfromsc.h"
#include <QQmlContext>
#include "Header/alarmnotification.h"

extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern Configuration *config;
extern AlarmNotification *alarmNotify;
extern QObject * object;
/**
 * @brief Splash::Splash : Constructor getting system ready status signals and slots
 * @param parent
 */
Splash::Splash(QObject *parent) : QObject(parent)
{
    m_SystemReadyStatus="0";
    context->setContextProperty("splash",this);
    QObject::connect(&CIObj , &CommunicationInterface::systemStatusData , this , &Splash::systemReadyStatusChanged);
}

/**
 * @brief Splash::systemReadyStatusChanged : Changed the ready state
 * @param a_ReqBuffer
 */
void Splash::systemReadyStatusChanged(QString a_ReqBuffer)
{
    m_SystemReadyStatus=a_ReqBuffer;
    if(m_SystemReadyStatus.toInt() == 1)
       {
           QMetaObject::invokeMethod(object,"closeAllPopups");
           alarmNotify->clearAlarmNotificationList();
           QMetaObject::invokeMethod(object, "alarmRaisedWithoutLogin");
       }
}

/**
 * @brief Splash::getSystemReadyStatus : Return the ready state
 * @return
 */
int Splash::getSystemReadyStatus()
{
    return m_SystemReadyStatus.toInt();
}

/**
 * @brief Splash::getSCStatus : Return the ready status whether success or fail
 * @return
 */
int Splash::getSCStatus()
{
    char buff[] = "";
    CIObj.sendMessage(SCBL_SC_STATE_REQ ,UIC_SC_STATE_RES,buff,0);
    char scStatus[4] = {0};
    CIObj.recivedMessage(UIC_SC_STATE_RES,scStatus,2);
    QString status = scStatus;
    m_SCStatus = status.toInt();
    m_SystemReadyStatus = status;
    return m_SCStatus;

}
