/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle usermodel

***************************************************************************/


#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "Header/user.h"

/**
 * @brief The UserModel class : implement usermodel by inheriting QAbstractListModel
 */
class UserModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        UserIdRole = Qt::UserRole + 1,
        UserLevelRole,
        UserStatusRole,
        UserLastModifiedRole,
        UserPassword,
        UserLastChangedPassword,
        UserLastLoginDate,
        UserLockRole,
        UserNumLoginAttempt,
        UserTimeLoginFrom,
        UserTimeLoginTo,
    };

    UserModel(QObject *parent = 0);

    /**
      * @brief addUserDetails:- add user data to usermodel list
      * @param userId : user id
      * @param password : user password
      * @param userLevel : user level
      * @param userStatus : user status
      * @param lastModified : user last modified date
      */
    Q_INVOKABLE void addUserDetails(const QString &userId, const QString& password
                                    , const QString &userLevel, const QString &userStatus
                                    , const QString &lastModified, const QString& lastChangedPassword
                                    , const QString &lastLogin);

    /**
      * @brief addUserDetails:- add user data to usermodel list
      * @param userId : user id
      * @param password : user password
      * @param userLevel : user level
      * @param userStatus : user status
      * @param lastModified : user last modified date
      * @param userLock :  user lock status
      * @param numLoginAttempt : wrong password login attempt number
      * @param timeLoginFrom : time login from
      * @param timeLoginTo : time login to
      */
    Q_INVOKABLE void addUserDetails(const QString &userId, const QString& password
                                    , const QString &userLevel, const QString &userStatus
                                    , const QString &lastModified, const QString& lastChangedPassword
                                    , const QString &lastLogin, const QString &userLock
                                    , const QString &numLoginAttempt, const QString &timeLoginFrom, const QString &timeLoginTo);


    /**
      * @brief updateUserDetails :- update user details in usermodel
      * @param row - update row number
      * @param userId : user id
      * @param password : user password
      * @param userLevel : user level
      * @param userStatus : user status
      * @param lastModified : user last modified date
      */
    Q_INVOKABLE void updateUserDetails(int row, const QString &userId, const QString &password
                                       , const QString &userLevel, const QString &userStatus
                                       , const QString &lastModified, const QString& lastChangedPassword
                                       , const QString &lastLogin);

    /**
      * @brief updateUserDetails :- update user details in usermodel
      * @param row - update row number
      * @param userId : user id
      * @param password : user password
      * @param userLevel : user level
      * @param userStatus : user status
      * @param userLock : user Lock status
      * @param lastModified : user last modified date
      */
    Q_INVOKABLE void updateUserDetails(int row, const QString &userId, const QString &password
                                       , const QString &userLevel, const QString &userStatus
                                       , const QString &lastModified, const QString& lastChangedPassword
                                       , const QString &lastLogin, const QString &userLock
                                       , const QString &numLoginAttempt, const QString &timeLoginFrom, const QString &timeLoginTo);



    Q_INVOKABLE QVariantMap get(int row);


    /**
      * @brief checkUserId:- check userid exist or not
      * @param userId - user id
      * @return ture if userid exist
      */
    Q_INVOKABLE bool checkUserId(const QString& userId);

    /**
     * @brief addUser : add user to usermodel
     * @param user - User object
     */
    void addUser(const User& user);

    /**
     * @brief removeRows : remove rows in the usermodel
     * @return if all the rows are removed
     */
    bool removeRows();

private:
    QList<User> m_userList;

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
};

#endif // USERMODEL_H
