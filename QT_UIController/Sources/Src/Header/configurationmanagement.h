/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle usermanagement

***************************************************************************/

#ifndef CONFIGURATIONMANAGEMENT_H
#define CONFIGURATIONMANAGEMENT_H

#include "Header/communicationinterface.h"
#include "Header/usermodel.h"

/**
 * @brief The UserManagement class: handle usermanagement class
 */
class UserManagement: public QObject
{
      Q_OBJECT
public:
    UserManagement();
    
    /**
      * @brief handleUserList - handle userlist fetch the data from server and fill the userlist
      */
    Q_INVOKABLE void handleUserList();
private:
    void setUserList(const QStringList& userList);
    void clearUserList(); 
    UserModel userModel;
};

#endif // CONFIGURATIONMANAGEMENT_H
