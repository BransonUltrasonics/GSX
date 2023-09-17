/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle usermodel

***************************************************************************/

#include "Header/usermodel.h"
#include <QQmlContext>
#include <QQuickView>
#include "Header/communicationinterface.h"
#include <iostream>

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief sendUserData : Sending user details to BL with comma seperatation
 * @param userId
 * @param password
 * @param userLevel
 * @param userStatus
 * @param lastModified
 * @param lastChangedPassword
 * @param lastLogin
 */
void sendUserData(const QString &userId, const QString &password, const QString &userLevel
                  , const QString &userStatus, const QString &lastModified
                  , const QString& lastChangedPassword, const QString &lastLogin)
{
    QStringList userSetupBuff;

    QString userSetupMsg;

    userSetupBuff.append(userId);
    userSetupBuff.append(password);
    userSetupBuff.append(userLevel);
    userSetupBuff.append("");
    userSetupBuff.append(userStatus);
    userSetupBuff.append(lastModified);

    userSetupMsg = userSetupBuff.join(",");

    QString privileges = "1111";
    QString reqData = "'"+userId+"','"+password+"','"+userLevel+"','"+privileges+"','"+userStatus+"','"+lastModified+"','"+
            lastChangedPassword+"','"+lastLogin+"'";
    CIObj.sendMessage(REQ_SET_USER_INFO, RES_SET_USER_INFO, reqData);
}

/**
 * @brief sendUserData : Sending user details to BL with comma seperatation
 * @param userId
 * @param password
 * @param userLevel
 * @param userStatus
 * @param lastModified
 * @param lastChangedPassword
 * @param lastLogin
 * @param userLock
 * @param numLoginAttempt
 * @param timeLoginFrom
 * @param timeLoginTo
 */
void sendUserData(const QString &userId, const QString &password, const QString &userLevel, const QString &userStatus,
                             const QString &lastModified, const QString &lastChangedPassword, const QString &lastLogin,
                             const QString &userLock, const QString& numLoginAttempt, const QString &timeLoginFrom, const QString &timeLoginTo)
{
    QString privileges = "1111";
    QString reqData = "'" + userId + "','" + password + "','" + userLevel + "','" + privileges + "','" + userStatus + "','" +
            lastModified + "','" + lastChangedPassword + "','" + lastLogin + "','" + userLock + "','" +
            numLoginAttempt + "','" + timeLoginFrom + "','" + timeLoginTo + "'";
    CIObj.sendMessage(REQ_SET_USER_INFO, RES_SET_USER_INFO, reqData);
}

/**
 * @brief UserModel::UserModel : Constructor
 * @param parent
 */
UserModel::UserModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

/**
 * @brief UserModel::updateUserDetails : Update user details received from Data Base
 * @param row
 * @param userId
 * @param password
 * @param userLevel
 * @param userStatus
 * @param lastModified
 * @param lastChangedPassword
 * @param lastLogin
 */
void UserModel::updateUserDetails(int row, const QString &userId, const QString &password, const QString &userLevel
                                  , const QString &userStatus, const QString &lastModified
                                  , const QString& lastChangedPassword, const QString &lastLogin)
{
    User& userObject = m_userList[row];

    userObject.setUserLevel(userLevel);
    userObject.setUserStatus(userStatus);
    userObject.setLastModified(lastModified);

    const QModelIndex idx = index(row);
    emit dataChanged(idx ,idx );

    sendUserData(userId, password, userLevel, userStatus, lastModified, lastChangedPassword, lastLogin);
}

/**
 * @brief UserModel::updateUserDetails : Update user details received from Data Base
 * @param row
 * @param userId
 * @param password
 * @param userLevel
 * @param userStatus
 * @param userLock
 * @param lastModified
 * @param lastChangedPassword
 * @param lastLogin
 */
void UserModel::updateUserDetails(int row, const QString &userId, const QString &password, const QString &userLevel,
                                  const QString &userStatus, const QString &lastModified, const QString &lastChangedPassword,
                                  const QString &lastLogin, const QString &userLock, const QString& numLoginAttempt,
                                  const QString &timeLoginFrom, const QString &timeLoginTo)
{
    User& userObject = m_userList[row];

    userObject.setUserLevel(userLevel);
    userObject.setUserStatus(userStatus);
    userObject.setUserLock(userLock);
    userObject.setLastModified(lastModified);

    const QModelIndex idx = index(row);
    emit dataChanged(idx ,idx );

    sendUserData(userId, password, userLevel, userStatus, lastModified, lastChangedPassword,
                 lastLogin, userLock, numLoginAttempt, timeLoginFrom, timeLoginTo);
}

/**
 * @brief UserModel::addUserDetails : Add modified user details
 * @param userId
 * @param password
 * @param userLevel
 * @param userStatus
 * @param lastModified
 * @param lastChangedPassword
 */
void UserModel::addUserDetails(const QString &userId, const QString &password, const QString &userLevel
                               , const QString &userStatus, const QString &lastModified
                               , const QString& lastChangedPassword, const QString &lastLogin)
{
    addUser(User(userId, password, userLevel, userStatus, lastModified, lastChangedPassword, lastLogin));
    sendUserData(userId, password, userLevel, userStatus, lastModified, lastChangedPassword, lastLogin);
}

/**
 * @brief UserModel::addUserDetails : Add modified user details
 * @param userId
 * @param password
 * @param userLevel
 * @param userStatus
 * @param userLock
 * @param lastModified
 * @param lastChangedPassword
 */
void UserModel::addUserDetails(const QString &userId, const QString &password, const QString &userLevel, const QString &userStatus,
                               const QString &lastModified, const QString &lastChangedPassword, const QString &lastLogin,
                               const QString &userLock, const QString &numLoginAttempt, const QString &timeLoginFrom, const QString &timeLoginTo)
{
    addUser(User(userId, password, userLevel, userStatus, lastModified, lastChangedPassword,
                 lastLogin, userLock, numLoginAttempt, timeLoginFrom, timeLoginTo));
    sendUserData(userId, password, userLevel, userStatus, lastModified, lastChangedPassword,
                 lastLogin, userLock, numLoginAttempt, timeLoginFrom, timeLoginTo);
}

/**
 * @brief UserModel::addUser : Storing All the userdetails  into List
 * @param user
 */
void UserModel::addUser(const User &user)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_userList << user;
    endInsertRows();
}

/**
 * @brief UserModel::removeRows : Removing particular details
 * @return
 */
bool UserModel::removeRows()
{
    auto count = rowCount();
    if (count <= 0)
        return false;

    int row = 0;

    beginRemoveRows(QModelIndex(), row, count);
    for(auto userCount = 0; userCount < count; ++userCount)
    {
        m_userList.pop_back();
    }
    endRemoveRows();
    return true;
}

/**
 * @brief UserModel::rowCount : Numbers of user counts
 * @param parent
 * @return
 */
int UserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_userList.count();
}

/**
 * @brief UserModel::data : Modified user details stored into List
 * @param index
 * @param role
 * @return
 */
QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_userList.count())
        return QVariant();

    const User &userObject = m_userList[index.row()];

    switch(role)
    {
    case UserIdRole:
        return userObject.getUserId();
    case UserLevelRole:
        return userObject.getUserLevel();
    case UserStatusRole:
        return userObject.getUserStatus();
    case UserLastModifiedRole:
        return userObject.getLastModified();
    case UserPassword:
        return userObject.getUserPassword();
    case UserLastChangedPassword:
        return userObject.getLastChangedPassword();
    case UserLastLoginDate:
        return userObject.getLastLoginDate();
    case UserLockRole:
        return userObject.getUserLock();
    case UserNumLoginAttempt:
        return userObject.getNumLoginAttempt();
    case UserTimeLoginFrom:
        return userObject.getTimeLoginFrom();
    case UserTimeLoginTo:
        return userObject.getTimeLoginTo();
    default:
        return QVariant();
    }
}

/**
 * @brief UserModel::roleNames : Details of the User
 * @return
 */
QHash<int, QByteArray> UserModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserIdRole] =                 "userId";
    roles[UserLevelRole] =              "userLevel";
    roles[UserStatusRole] =             "userStatus";
    roles[UserLastModifiedRole] =       "lastModified";
    roles[UserPassword] =               "userPassword";
    roles[UserLastChangedPassword] =    "lastChangedPassword";
    roles[UserLastLoginDate] =          "userLastLoginDate";
    roles[UserLockRole] =               "userLock";
    roles[UserNumLoginAttempt] =        "userNumLoginAttempt";
    roles[UserTimeLoginFrom] =          "userTimeLoginFrom";
    roles[UserTimeLoginTo] =            "userTimeLoginTo";
    return roles;
}

/**
 * @brief UserModel::get : Map the particular user according to UserID
 * @param row
 * @return
 */
QVariantMap UserModel::get(int row) {
    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }
    return res;
}

/**
 * @brief UserModel::removeRow : Selected Rows has been Removed
 * @param row
 * @param parent
 * @return
 */
bool UserModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_userList.size()) return false;
    beginRemoveRows(QModelIndex(), row, row);
    m_userList.removeAt(row);
    endRemoveRows();
    return true;
}

/**
 * @brief UserModel::checkUserId : Checking the particular user based on userId
 * @param userId
 * @return
 */
bool UserModel::checkUserId(const QString& userId)
{
    for(auto& user : m_userList)
    {
        if(user.getUserId() == userId)
            return true;
    }
    return false;
}
