/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Hanlde authority level management

***************************************************************************/

#ifndef AUTHORITYLEVELMANAGEMENT_H
#define AUTHORITYLEVELMANAGEMENT_H

#include <iostream>
#include <memory>
#include <QObject>
#include <QQmlListProperty>
#include <Header/authoritylevelmodel.h>

#define AUTHORITY_EXCPECTED_SIZE       7
#define AUTHORITY_SPLITTER             ","
#define AUTHORTIY_REQ_DATA             "4"

class AuthorityLevelManagement : public QObject
{
    Q_OBJECT
public:
    explicit AuthorityLevelManagement(QObject *parent = nullptr);
    virtual ~AuthorityLevelManagement();
    AuthorityLevelModel *getAuthorityModel();

    Q_INVOKABLE void handleAuthorityOption();
    Q_INVOKABLE int readTempFile();

private:
    AuthorityLevelModel* m_authorityModel;

    void clearAuthorityLevelList();
    void setAuthorityLevelList(const QStringList& authorityLevelList);
};

#endif // AUTHORITYLEVELMANAGEMENT_H
