/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle user data

***************************************************************************/

#ifndef USER_H
#define USER_H

#include <QString>

/**
 * @brief The User class: user details are stored in this class
 */
class User
{
public:
    /**
     * @brief User
     * @param userId
     * @param password
     * @param userLevel
     * @param userStatus
     * @param lastModified
     * @param lastChangedPassword
     * @param lastLoginDate
     */
    User(const QString& userId, const QString& password, const QString& userLevel,
         const QString& userStatus, const QString& lastModified,
         const QString& lastChangedPassword, const QString& lastLoginDate);

    /**
     * @brief User
     * @param userId
     * @param password
     * @param userLevel
     * @param userStatus
     * @param lastModified
     * @param lastChangedPassword
     * @param lastLoginDate
     * @param userLock
     * @param numLoginAttempt
     * @param TimeLoginFrom
     * @param TimeLoginTo
     */
    User(const QString& userId, const QString& password, const QString& userLevel,
         const QString& userStatus, const QString& lastModified, const QString& lastChangedPassword,
         const QString& lastLoginDate, const QString& userLock, const QString& numLoginAttempt,
         const QString& timeLoginFrom, const QString& timeLoginTo);

    /**
     * @brief setUserId - set user id
     * @param userId
     */
    void setUserId(const QString& userId);

    /**
     * @brief setUserPassword - set user password
     * @param password
     */
    void setUserPassword(const QString& password);

    /**
     * @brief setUserLevel - set user level
     * @param userLevel
     */
    void setUserLevel(const QString& userLevel);

    /**
     * @brief setUserStatus - set user status
     * @param userStatus
     */
    void setUserStatus(const QString& userStatus);

    /**
     * @brief setUserLock - set user lock
     * @param userStatus
     */
    void setUserLock(const QString& userLock);

    /**
     * @brif setNumLoginAttempt - set Login Attempt Number
     *
     */
    void setNumLoginAttempt(const QString& numLoginAttempt);

    /**
     * @brif setTimeLoginFrom - set Time Login From
     *
     */
    void setTimeLoginFrom(const QString& timeLoginFrom);

    /**
     * @brif setTimeLoginTo - set Time Login To
     *
     */
    void setTimeLoginTo(const QString& timeLoginTo);

    /**
     * @brief setLastModified - set last modified date
     * @param lastModified
     */
    void setLastModified(const QString& lastModified);

    /**
     * @brief setLastChangedPassword - set the last changed password
     * @param lastChangedPassword
     */
    void setLastChangedPassword(const QString& lastChangedPassword);

    /**
     * @brief getUserId - get userid
     * @return userid
     */
    QString getUserId() const;

    /**
     * @brief getUserPassword - get user password
     * @return user password
     */
    QString getUserPassword() const;

    /**
     * @brief getUserLevel - get user level
     * @return user level
     */
    QString getUserLevel() const;

    /**
     * @brief getUserStatus - get user status
     * @return user status
     */
    QString getUserStatus() const;

    /**
     * @brief getUserStatus - get user status
     * @return user status
     */
    QString getUserLock() const;

    /**
     * @brief getNumLoginAttempt - get Login Attempt Number
     * @return m_numLoginAttempt
     */
    QString getNumLoginAttempt() const;

    /**
     * @brief getTimeLoginFrom - get Time Login From
     * @return m_timeLoginFrom
     */
    QString getTimeLoginFrom() const;

    /**
     * @brief getTimeLoginTo - get Time Login To
     * @return m_timeLoginTo
     */
    QString getTimeLoginTo() const;

    /**
     * @brief getLastModified - get user last modified date
     * @return user last modified date
     */
    QString getLastModified() const;

    /**
     * @brief getLastChangedPassword - get the last changed password
     * @return user last modifed password
     */
    QString getLastChangedPassword() const;

    QString getLastLoginDate() const;

    void setLastLoginDate(const QString &lastLoginDate);

private:
    QString m_userId;               ///< user id
    QString m_password;             ///< user password
    QString m_userLevel;            ///< user level
    QString m_userStatus;           ///< user status
    QString m_lastModified;         ///< user last modified date
    QString m_lastChangedPassword;  ///< user last Changed Password
    QString m_lastLoginDate;        ///< user last login datetime
    QString m_userLock;             ///< user lock
    QString m_numLoginAttempt;      ///< wrong password login attempt number >
    QString m_timeLoginFrom;        ///< time login from >
    QString m_timeLoginTo;          ///< time login to >
};

#endif // USER_H
