/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Handles usermanagement

***************************************************************************/

#include <iostream>
#include <QQmlContext>
#include <QQuickView>
#include "Header/configurationmanagement.h"

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief UserManagement::UserManagement : Constructor
 */
UserManagement::UserManagement()
{
    /* Refreshing UI user model */
    context->setContextProperty("userModel", &userModel);
}

/**
 * @brief UserManagement::handleUserList : Reads all users from DB and store in list
 */
void UserManagement::handleUserList()
{
    clearUserList();

    QString userResponse = "";

    CIObj.sendMessage(REQ_GET_ALL_USERS, RES_GET_ALL_USERS, "");

    CIObj.recivedMessage(RES_GET_ALL_USERS, userResponse);

    QStringList userList = userResponse.split(",");
    if (!userResponse.isEmpty() && userList.size() >= 1)
    {
        setUserList(userList);
    }
}

/**
 * @brief UserManagement::setUserList : Sets all users in model
 * @param userList
 */
void UserManagement::setUserList(const QStringList& userList)
{
    QString userId = "";
    QString userPassword = "";
    QString userLevel = "";
    QString userStatus = "";
    QString userLock   = "";
    QString operatorPrivilages = "";
    QString userLastModified = "";
    QString lastChangedPassword = "";
    QString lastLogin = "";
    QString numLoginAttempt = "";
    QString timeLoginFrom = "";
    QString timeLoginTo = "";

    for (auto userCount = 0; userCount < userList.size(); ++userCount)
    {
        userId              = userList[userCount];
        userPassword        = userList[++userCount];
        userLevel           = userList[++userCount];
        userStatus          = userList[++userCount];
        operatorPrivilages  = userList[++userCount];
        userLastModified    = userList[++userCount];
        lastChangedPassword = userList[++userCount];
        lastLogin           = userList[++userCount];
        userLock            = userList[++userCount];
        numLoginAttempt     = userList[++userCount];
        timeLoginFrom       = userList[++userCount];
        timeLoginTo         = userList[++userCount];
        userModel.addUser(User(userId, userPassword, userLevel, userStatus, userLastModified, lastChangedPassword,
                               lastLogin, userLock, numLoginAttempt, timeLoginFrom, timeLoginTo));
    }
}

/**
 * @brief UserManagement::clearUserList : Clears user from model
 */
void UserManagement::clearUserList()
{
    userModel.removeRows();
}
