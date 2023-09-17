/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Login Page

***************************************************************************/

#ifndef LOGIN_H
#define LOGIN_H
#include <QObject>
#include <memory>
#include <Header/user.h>
#include "Header/errorcodes.h"
#include "Header/helperenums.h"

//!  A Login class.
/*!
  This class derived from QObject, handles Login BackEnd functionality.
*/

#define LOG_IN   1
#define LOG_OUT  0

#define LOGIN_ALLOWED  "1"
#define LOGIN_DENIED   "0"

#define NO_USER "WS_N/A"

enum{
    WS_USER_NAME,
    WS_USER_LEVEL,
    WS_LOG_STATUS
};

class Login: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_username READ GetLoginUserName)
    Q_PROPERTY(QString m_password READ GetPassword)
    Q_PROPERTY(QString m_userType READ GetUserType)
    Q_PROPERTY(QString userStatus READ GetUserStatus)
    Q_PROPERTY(QString lastChangedPassword READ getLastChangedPassword)

public:
    explicit Login(QObject *parent = 0);

    Q_PROPERTY(bool IsWebServicesLogged READ getIsWebServicesLogged)
    Q_PROPERTY(QString WSLoggedUserName READ getWSLoggedUserName)
    Q_PROPERTY(int WSUserLevelStatus READ getWSUserLevelStatus)
    Q_PROPERTY(QString UserOnModifyScreen READ getUserOnModifyScreen WRITE setUserOnModifyScreen)

    QString GetLoginUserName();
    QString GetPassword();
    QString GetUserStatus();
    QString getLastChangedPassword();
    QString getWSLoggedUserName();
    QString getUserOnModifyScreen();
    int getWSUserLevelStatus();
    bool getIsWebServicesLogged();
    void webserviceLoginSetup();
    void setUserOnModifyScreen(QString UserOnModify);

    Q_INVOKABLE void setWebserviceUserLoggedInfo(QString WebServicesLoggedUserInfo);
    Q_INVOKABLE QString GetUserType();
    Q_INVOKABLE void setAuthorityCheckStatus(int status);
    Q_INVOKABLE int getAuthorityCheckStatus();
    Q_INVOKABLE void getWSUserAndLevelLogged();
    Q_INVOKABLE void setWSRestrictedScreens(int restrictedScreen);

    /*! This function holds the functionality to Validate the login details.
     *
     *  @param  a_userName: A QString variable, holding the value username.
     *  @param  a_password: A QString variable, holding the value of password.
     */
    Q_INVOKABLE int loginValidate(QString  a_userName , QString  a_password) ;
    /*! This function holds the functionality to Enable the password field.
     *
     *  @param  a_userNameEmpty: A QString variable, holding the value username.
     */
    Q_INVOKABLE bool passwordEnable(QString  a_userNameEmpty ) ;
    /*! This function holds the functionality to logout.
     *
     */
    Q_INVOKABLE void logout();
    Q_INVOKABLE void setUserDetails(QString userName);    
    Q_INVOKABLE void updateUserLogin();
    Q_INVOKABLE int validatePasswordExpiry();
    Q_INVOKABLE bool isFirstLogin() const;
    Q_INVOKABLE QString getUserLevel();

private:
    QString m_password;
    QString m_WSLoggedUserName;
    QString m_UserOnModifyScreen;
    int isAuthorityCheckReq;
    int m_RestrictedScreenType;
    int m_WSUserLevelStatus;
    bool m_bIsWebServicesLogged;
    std::shared_ptr<User> m_pUserProfile;
};

#endif // LOGIN_H
