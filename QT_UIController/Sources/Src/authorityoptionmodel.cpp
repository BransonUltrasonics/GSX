/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle authority options model and sys config tabs

***************************************************************************/

#include <QQmlContext>
#include <iterator>
#include "Header/authorityoptionmodel.h"
#include "Header/menuhandler.h"
#include "Header/helperenums.h"
#include "Header/sysconfigdata.h"
#include "Header/configuration.h"
#include "Header/login.h"

extern QQmlContext* context;
extern Login *login;
/**
 * @brief AuthorityOptionModel::AuthorityOptionModel : Constructor
 * @param parent
 */
AuthorityOptionModel::AuthorityOptionModel(QObject *parent) : QObject(parent)
{
    m_userlevel = 1;
    m_defaultIndex = 0;
}

/**
 * @brief AuthorityOptionModel::~AuthorityOptionModel : Destructor
 */
AuthorityOptionModel::~AuthorityOptionModel()
{
    qDeleteAll(m_systemLevelConfigList);
}
void AuthorityOptionModel::authorityOptionModelInit()
{
    m_accesslist[Features::Features::USER_IO] =  AUTHORITYOPTIONMODEL_USERIO;
    m_accesslist[Features::Features::GENERAL] = AUTHORITYOPTIONMODEL_GENERAL;
    m_accesslist[Features::Features::USER_LIST] =  AUTHORITYOPTIONMODEL_USER_MANAGEMENT;
    m_accesslist[Features::Features::USER_AUTHORITY] = AUTHORITYOPTIONMODEL_USER_AUTHORIYT;
    m_accesslist[Features::Features::CHANGE_PASSWORD] = AUTHORITYOPTIONMODEL_CHANGE_PASSWORD;
    m_accesslist[Features::Features::ALARAM_MANAGEMENT] = AUTHORITYOPTIONMODEL_ALARM_MANAGEMENT;
}

/**
 * @brief AuthorityOptionModel::updateList : Updates the authority option model list
 * @param : int userLevel
 */
void AuthorityOptionModel::updateList(int userLevel)
{
    clearList("systemConfigModel", m_systemLevelConfigList);

    if(SysConfigData::getInstance()->getBransonKeyGlobalFlag())
    {
        m_accesslist[Features::Features::BRANSON] = AUTHORITYOPTIONMODEL_BRANSON;
    }
    else
    {
        m_accesslist.remove(Features::Features::BRANSON);
    }

    if(Configuration::getInstance()->getFeatureFlagStatus(USER_IO) == 1)
    {
        m_accesslist[Features::Features::USER_IO] =  AUTHORITYOPTIONMODEL_USERIO;
    }

    auto access = m_accesslist.begin();
    auto userPrivilageObj = UserPrivilege::getInstance();
    m_userlevel = userLevel;
    auto index = 0;
    auto isDefaultIndexset = false;

    for (;access != m_accesslist.end(); ++access)
    {
        auto enabledItem = userPrivilageObj->isPermissionAllowed(m_userlevel, access.key());

        if (!isDefaultIndexset && enabledItem)
        {
            m_defaultIndex = index;
            isDefaultIndexset = true;
        }
        if((Configuration::getInstance()->m_systemData.AuthCheckLevel == 0) &&
                (access.value() == AUTHORITYOPTIONMODEL_USER_AUTHORIYT))
        {
            enabledItem = false;
        }
        if((login->GetLoginUserName() == AUTHORITYOPTIONMODEL_NA) && (access.value() ==  AUTHORITYOPTIONMODEL_CHANGE_PASSWORD))
        {
            enabledItem = false;
        }
        m_systemLevelConfigList.push_back(new ConfigurationData(access.key() - 16, access.value(), enabledItem));
        ++index;
    }

    /* Refreshing UI system config menu model */
    context->setContextProperty("systemConfigModel", QVariant::fromValue(m_systemLevelConfigList));
}

/**
 * @brief AuthorityOptionModel::setUserLevel : Sets user level
 * @param : int userLevel
 */
void AuthorityOptionModel::setUserLevel(int userLevel)
{
    m_userlevel = userLevel;
}

/**
 * @brief AuthorityOptionModel::getDefaultIndex : Returns index of model
 * @return : int
 */
int AuthorityOptionModel::getDefaultIndex()
{
    return m_defaultIndex;
}

/**
 * @brief AuthorityOptionModel::clearList : Clears user list
 * @param : QString &modelName
 * @param : QList<QObject*>& usedList
 */
void AuthorityOptionModel::clearList(const QString& modelName, QList<QObject*>& usedList)
{
    if (!usedList.empty())
    {
        auto tempList = usedList;
        usedList.clear();
        context->setContextProperty(modelName, QVariant::fromValue(usedList));
        qDeleteAll(tempList);
    }
}
