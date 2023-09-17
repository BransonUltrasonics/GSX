/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Menu Handler

***************************************************************************/

#ifndef FEATURESENUM_H
#define FEATURESENUM_H

#include <QObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>

#include "Header/helperenums.h"
#include "Header/authoritylevelmanagement.h"


class UserPrivilege : public QObject
{
    Q_OBJECT
public:
    static UserPrivilege *getInstance();

    Q_INVOKABLE bool isPermissionAllowed(int userLevel, int privilege);
    Q_INVOKABLE void init(int userLevel);

private:
    AuthorityLevelManagement authorityLevelObj;
    QList<int> opertorRestrictedPrivilegeList;
    QList<int> technicianRestrictedPrivilegeList;
    QList<int> supervisorRestrictedPrivilegeList;
    static UserPrivilege* m_pUserPrivilege;

    UserPrivilege();
    ~UserPrivilege();

    bool checkPermission(int userLevel, int privilege);
    bool checkOpertorPrivilegeList(int privilege);
    bool checkSupervisorPrivilegeList(int privilege);
    bool checkTechnicianPrivilegeList(int privilege);
    void initOperatorRestrictedPrivileges();
    void initTechnicianRestrictedPrivileges();
    void initSupervisorRestrictedPrivileges();
};


#endif // FEATURESENUM_H
