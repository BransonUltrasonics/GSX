/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle authority level model

***************************************************************************/

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include "Header/authoritylevelmodel.h"
#include "Header/communicationinterface.h"

extern CommunicationInterface CIObj;

/**
 * @brief createTempFile : Create a file and opens in writeonly mode
 * @param : int permissionId
 * @param : int grant
 */
void createTempFile(int permissionId, int grant)
{
    Q_UNUSED(permissionId)
    // Create a new file
    QFile file("logincheck.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << grant;
}

/**
 * @brief AuthorityLevelModel::AuthorityLevelModel : Constructor
 * @param : QObject parent
 */
AuthorityLevelModel::AuthorityLevelModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

/**
 * @brief AuthorityLevelModel::updateAuthority : Updates authority options in the map
 * @param : int permissionId
 * @param : QString module
 * @param : QString title
 * @param : int grant
 */
void AuthorityLevelModel::updateAuthority(int permissionId, const QString &module, const QString &title, int grant)
{
    Q_UNUSED(module)
    Q_UNUSED(title)
    m_authorityMap[permissionId] = grant;
}

/**
 * @brief AuthorityLevelModel::sendAuthorityStatusToDB : Stores authority model data into DB
 */
void AuthorityLevelModel::sendAuthorityStatusToDB()
{
    QMapIterator<int, int> i(m_authorityMap);
    QString reqData = "";

    while (i.hasNext())
    {
        i.next();
        int permissionId = i.key();
        int grant = i.value();
        reqData += AUTHPRITY_OPTION + QString::number(permissionId) + " = " + QString::number(grant)+",";
    }

    reqData.chop(1);

    /* Send message to DB */
    CIObj.sendMessage(REQ_UPDATE_ALL_AUTHORITY_LEVEL, RES_UPDATE_ALL_AUTHORITY_LEVEL, reqData);
    QString qstrRes;
    CIObj.recivedMessage(RES_UPDATE_ALL_AUTHORITY_LEVEL, qstrRes);
}

/**
 * @brief AuthorityLevelModel::addAuthority : Adds authority level model
 * @param : int permissionId
 * @param : QString module
 * @param : QString title
 * @param : int grant
 */
void AuthorityLevelModel::addAuthority(int permissionId, const QString &module, const QString &title, int grant)
{
    addAuthority(Authority(permissionId, module, title, grant));
}

/**
 * @brief AuthorityLevelModel::addAuthority : Adds or inserts quthority model details into list
 * @param : Authority authority
 */
void AuthorityLevelModel::addAuthority(const Authority &authority)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_authorityList << authority;
    endInsertRows();
}

/**
 * @brief AuthorityLevelModel::roleNames : Returns role names
 * @return : QHash
 */
QHash<int, QByteArray> AuthorityLevelModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[PermissionIdRole] = AUTHPRITY_PERMISSION_ID;
    roles[ModuleRole] = AUTHPRITY_MODULE;
    roles[TitleRole] = AUTHPRITY_TITLE;
    roles[GrantRole] = AUTHPRITY_GRANT;
    return roles;
}

/**
 * @brief AuthorityLevelModel::get :
 * @param : int row
 * @return : QVariantMap
 */
QVariantMap AuthorityLevelModel::get(int row)
{
    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> itr(names);
    QVariantMap res;
    while (itr.hasNext())
    {
        itr.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(itr.key());
        res[itr.value()] = data;
    }
    return res;
}

/**
 * @brief AuthorityLevelModel::rowCount : Returns number of authority options
 * @param : QModelIndex parent
 * @return : int
 */
int AuthorityLevelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_authorityList.count();
}

/**
 * @brief AuthorityLevelModel::data : Returns authorityObject members
 * @param : QModelIndex index
 * @param : int role
 * @return : QVariant
 */
QVariant AuthorityLevelModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_authorityList.count())
    {
        return QVariant();
    }
    const Authority &authorityObject = m_authorityList[index.row()];

    switch(role)
    {
    case PermissionIdRole:
        return authorityObject.m_permissionId;
    case ModuleRole:
        return authorityObject.m_module;
    case TitleRole:
        return authorityObject.m_title;
    case GrantRole:
        return authorityObject.m_grant;
    default:
        return QVariant();
    }
}

/**
 * @brief AuthorityLevelModel::removeRows : Removes rows from authority option list
 * @return : bool
 */
bool AuthorityLevelModel::removeRows()
{
    auto count = rowCount();
    if (count <= 0)
    {
        return false;
    }
    int row = 0;
    beginRemoveRows(QModelIndex(), row, count);
    for(auto userCount = 0; userCount < count; ++userCount)
    {
        m_authorityList.pop_back();
    }
    endRemoveRows();
    return true;
}
