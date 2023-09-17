/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle user data

***************************************************************************/

#include "Header/user.h"

/**
 * @brief User::User : User detail information
 * @param userId
 * @param password
 * @param userLevel
 * @param userStatus
 * @param lastModified
 * @param lastChangedPassword
 */
User::User(const QString &userId, const QString& password, const QString &userLevel
           , const QString &userStatus, const QString &lastModified, const QString &lastChangedPassword, const QString &lastLoginDate)
    : m_userId(userId)
    , m_password(password)
    , m_userLevel(userLevel)
    , m_userStatus(userStatus)
    , m_lastModified(lastModified)
    , m_lastChangedPassword(lastChangedPassword)
    , m_lastLoginDate(lastLoginDate)
{

}

User::User(const QString &userId, const QString &password, const QString &userLevel,
           const QString &userStatus, const QString &lastModified, const QString &lastChangedPassword,
           const QString &lastLoginDate, const QString &userLock, const QString &numLoginAttempt,
           const QString &timeLoginFrom, const QString &timeLoginTo)
    : m_userId(userId)
    , m_password(password)
    , m_userLevel(userLevel)
    , m_userStatus(userStatus)
    , m_lastModified(lastModified)
    , m_lastChangedPassword(lastChangedPassword)
    , m_lastLoginDate(lastLoginDate)
    , m_userLock(userLock)
    , m_numLoginAttempt(numLoginAttempt)
    , m_timeLoginFrom(timeLoginFrom)
    , m_timeLoginTo(timeLoginTo)
{

}

/**
 * @brief User::setUserId : Set the userid information
 * @param userId
 */
void User::setUserId(const QString &userId)
{
    m_userId = userId;
}

/**
 * @brief User::setUserPassword : Set user passwords
 * @param password
 */
void User::setUserPassword(const QString &password)
{
    m_password = password;
}

/**
 * @brief User::setUserLevel : Set the user level authority option
 * @param userLevel
 */
void User::setUserLevel(const QString &userLevel)
{
    m_userLevel = userLevel;
}

/**
 * @brief User::setUserStatus : set the user status
 * @param userStatus
 */
void User::setUserStatus(const QString &userStatus)
{
    m_userStatus = userStatus;
}

/**
 * @brief User::setUserLock : set the user Lock status
 * @param userStatus
 */
void User::setUserLock(const QString &userLock)
{
    m_userLock = userLock;
}

/**
 * @brief User::setNumLoginAttept : set the Login Attempt Number
 * @param numLoginAttempt
 */
void User::setNumLoginAttempt(const QString &numLoginAttempt)
{
    m_numLoginAttempt = numLoginAttempt;
}

/**
 * @brief User::setTimeLoginFrom : set Time Login From
 * @param timeLoginFrom
 */
void User::setTimeLoginFrom(const QString &timeLoginFrom)
{
    m_timeLoginFrom = timeLoginFrom;
}

/**
 * @brief User::setTimeLoginFrom : set Time Login To
 * @param timeLoginTo
 */
void User::setTimeLoginTo(const QString &timeLoginTo)
{
    m_timeLoginTo = timeLoginTo;
}

/**
 * @brief User::setLastModified : Set the last modified user
 * @param lastModified
 */
void User::setLastModified(const QString &lastModified)
{
    m_lastModified = lastModified;
}

/**
 * @brief User::setLastChangedPassword : Set the last change password
 * @param lastChangedPassword
 */
void User::setLastChangedPassword(const QString &lastChangedPassword)
{
    m_lastChangedPassword = lastChangedPassword;
}

/**
 * @brief User::getUserId : get the user id
 * @return
 */
QString User::getUserId() const
{
    return m_userId;
}

/**
 * @brief User::getUserPassword : Get the user password
 * @return
 */
QString User::getUserPassword() const
{
    return m_password;
}

/**
 * @brief User::getUserLevel : Get the user level
 * @return
 */
QString User::getUserLevel() const
{
    return m_userLevel;
}

/**
 * @brief User::getUserStatus  : Get the user authority options
 * @return
 */
QString User::getUserStatus() const
{
    return m_userStatus;
}

/**
 * @brief User::getUserLock  : Get the user Lock options
 * @return
 */
QString User::getUserLock() const
{
    return m_userLock;
}

/**
 * @brief User::getNumLoginAttempt  : Get the login attempt number
 * @return
 */
QString User::getNumLoginAttempt() const
{
    return m_numLoginAttempt;
}

/**
 * @brief User::getTimeLoginFrom  : Get Time Login From
 * @return
 */
QString User::getTimeLoginFrom() const
{
    return m_timeLoginFrom;
}

/**
 * @brief User::getTimeLoginTo  : Get Time Login To
 * @return
 */
QString User::getTimeLoginTo() const
{
    return m_timeLoginTo;
}

/**
 * @brief User::getLastModified : Get the Last modified user
 * @return
 */
QString User::getLastModified() const
{
    return m_lastModified;
}

/**
 * @brief User::getLastChangedPassword : Get the Last changed password
 * @return
 */
QString User::getLastChangedPassword() const
{
    return m_lastChangedPassword;
}

/**
 * @brief User::getLastLoginDate : Get the last loginin date
 * @return
 */
QString User::getLastLoginDate() const
{
    return m_lastLoginDate;
}

/**
 * @brief User::setLastLoginDate :  Set the last loginin date
 * @param lastLoginDate
 */
void User::setLastLoginDate(const QString &lastLoginDate)
{
    m_lastLoginDate = lastLoginDate;
}
