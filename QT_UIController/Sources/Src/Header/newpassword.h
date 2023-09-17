/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

New Password

***************************************************************************/
#ifndef NEWPASSWORD_H
#define NEWPASSWORD_H
#include <QString>
#include <QObject>

struct stNewPassword
{
    QString m_UserID;
    QString m_CurrentPassword;
};


class NewPassword:public QObject
{
    Q_OBJECT
public:
    NewPassword(QObject *parent=0);

    Q_INVOKABLE void setUserID(QString a_UserID);
    Q_INVOKABLE void setNewPassword(QString a_NewPassword);

    Q_INVOKABLE QString getUserID();
    Q_INVOKABLE QString getCurrentPassword();
    Q_INVOKABLE void getPasswordFromDB();


    Q_INVOKABLE bool isUserDataValid(QString a_UserID,QString a_CurrentPassword);
    Q_INVOKABLE bool isConfirmPasswordSame(QString a_NewPassword,QString a_ConfirmPassword);
    Q_INVOKABLE void setContext();

private:
        stNewPassword m_stNewPassword;

};


#endif // NEWPASSWORD_H
