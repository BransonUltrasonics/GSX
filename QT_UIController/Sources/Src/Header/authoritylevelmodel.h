/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle authority level model

***************************************************************************/

#ifndef AUTHORITYLEVELMODEL_H
#define AUTHORITYLEVELMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>

#define AUTHPRITY_PERMISSION_ID         "permissionId"
#define AUTHPRITY_MODULE                "module"
#define AUTHPRITY_TITLE                 "Title"
#define AUTHPRITY_GRANT                 "grant"
#define AUTHPRITY_OPTION                "Option"
struct Authority
{
    Authority(int permissionId, const QString& module, const QString& title, int grant)
        : m_permissionId(permissionId)
        , m_module(module)
        , m_title(title)
        , m_grant(grant)
    {

    }
    int m_permissionId;
    QString m_module;
    QString m_title;
    int m_grant;
};

class AuthorityLevelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        PermissionIdRole = Qt::UserRole + 1,
        ModuleRole,
        TitleRole,
        GrantRole
    };

    AuthorityLevelModel(QObject *parent = 0);

    Q_INVOKABLE void updateAuthority(int permissionId, const QString& module, const QString& title, int grant);
    Q_INVOKABLE void sendAuthorityStatusToDB();
    Q_INVOKABLE void addAuthority(int permissionId, const QString& module, const QString& title, int grant);
    Q_INVOKABLE QVariantMap get(int row);

    void addAuthority(const Authority& authority);
    bool removeRows();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Authority> m_authorityList;
    QMap<int, int> m_authorityMap;
};

#endif // AUTHORITYLEVELMODEL_H
