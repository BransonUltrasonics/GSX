/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Hanlde authority level management

***************************************************************************/

#include "Header/authoritylevelmanagement.h"
#include "Header/communicationinterface.h"
#include "Header/sysconfigdata.h"
#include "Header/SysConfig.h"
#include "Header/systemconfiggeneral.h"
#include <QQmlContext>
#include <QQuickView>

#define AUTHORITY_OPTION_RUN_UNVALIDATED_RECIPE     QObject::tr("Run unvalidated weld recipe")
#define AUTHORITY_OPTION_RESET_ALARMS               QObject::tr("Reset alarms")
#define AUTHORITY_OPTION_SET_AS_ACTIVE              QObject::tr("Set as active")
#define AUTHORITY_OPTION_RESET_BATCH_COUNTER        QObject::tr("Reset batch counter")
#define AUTHORITY_OPTION_QUICK_CALIBRATION          QObject::tr("Quick Calibration")
#define AUTHORITY_OPTION_BARCODE_RECIPE_RECALL      QObject::tr("Barcode Recipe Recall")
#define AUTHORITY_OPTION_ACTUATOR_SETUP             QObject::tr("Actuator Setup")

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief AuthorityLevelManagement::AuthorityLevelManagement : Constructor
 * @param parent
 */
AuthorityLevelManagement::AuthorityLevelManagement(QObject *parent)
    : QObject(parent)
    , m_authorityModel(new AuthorityLevelModel(this))
{
    if (m_authorityModel)
    {
        /* Refreshing UI authority model */
        context->setContextProperty("authorityModel", m_authorityModel);
    }
}

/**
 * @brief AuthorityLevelManagement::~AuthorityLevelManagement : Destructor
 */
AuthorityLevelManagement::~AuthorityLevelManagement()
{
    if (m_authorityModel)
    {
        delete m_authorityModel;
    }
}

/**
 * @brief AuthorityLevelManagement::handleAuthorityOption : Gets authority options form DB and
 *                                                          updates in the list
 */
void AuthorityLevelManagement::handleAuthorityOption()
{
    clearAuthorityLevelList();
    QString userResponse = "";
    QString reqData = AUTHORTIY_REQ_DATA; // need to pass this value from qml

    /* Send messgae to DB */
    CIObj.sendMessage(REQ_GET_ALL_AUTHORITY_LEVEL, RES_GET_ALL_AUTHORITY_LEVEL, reqData);
    CIObj.recivedMessage(RES_GET_ALL_AUTHORITY_LEVEL, userResponse);

    if (!userResponse.isEmpty())
    {
        QStringList authorityLevelList = userResponse.split(AUTHORITY_SPLITTER);
        if(authorityLevelList.size() != AUTHORITY_EXCPECTED_SIZE)
        {
            userResponse = "0,0,0,0,0,0,0";
            authorityLevelList = userResponse.split(AUTHORITY_SPLITTER);
        }
        setAuthorityLevelList(authorityLevelList);
    }
}

/**
 * @brief AuthorityLevelManagement::clearAuthorityLevelList : Clears authority options list
 */
void AuthorityLevelManagement::clearAuthorityLevelList()
{
    m_authorityModel->removeRows();
}

/**
 * @brief AuthorityLevelManagement::setAuthorityLevelList : Sets authority options in list
 * @param : QStringList authorityLevelList
 */
void AuthorityLevelManagement::setAuthorityLevelList(const QStringList &authorityLevelList)
{
    QString permissionId = "1,2,3,4,5,6,7";

    QStringList permissionIdList = permissionId.split(AUTHORITY_SPLITTER);
    QStringList moduleList;
    SysConfigData *sysconfig = SysConfigData::getInstance();
    moduleList.append(AUTHORITY_OPTION_RUN_UNVALIDATED_RECIPE.toLower());
    moduleList.append(AUTHORITY_OPTION_RESET_ALARMS.toLower());
    moduleList.append(AUTHORITY_OPTION_SET_AS_ACTIVE.toLower());
    moduleList.append(AUTHORITY_OPTION_RESET_BATCH_COUNTER.toLower());
    if(sysconfig->getCurrentNameOfComboBox(SYS_CONFIG_SYSTEM_TYPE) != SystemType::SYSTEM_TYPE_GSX_E1)
    {
        moduleList.append(AUTHORITY_OPTION_QUICK_CALIBRATION.toLower());
    }
    moduleList.append(AUTHORITY_OPTION_BARCODE_RECIPE_RECALL.toLower());
    moduleList.append(AUTHORITY_OPTION_ACTUATOR_SETUP.toLower());

    QStringList titleList;
    titleList.append(AUTHORITY_OPTION_RUN_UNVALIDATED_RECIPE);
    titleList.append(AUTHORITY_OPTION_RESET_ALARMS);
    titleList.append(AUTHORITY_OPTION_SET_AS_ACTIVE);
    titleList.append(AUTHORITY_OPTION_RESET_BATCH_COUNTER);
    if(sysconfig->getCurrentNameOfComboBox(SYS_CONFIG_SYSTEM_TYPE) != SystemType::SYSTEM_TYPE_GSX_E1)
    {
        titleList.append(AUTHORITY_OPTION_QUICK_CALIBRATION);
    }
    titleList.append(AUTHORITY_OPTION_BARCODE_RECIPE_RECALL);
    titleList.append(AUTHORITY_OPTION_ACTUATOR_SETUP);

    for (int authorityCount = 0; authorityCount < authorityLevelList.size(); authorityCount++)
    {
        if(authorityCount >= permissionIdList.size())
            break;
        if(authorityCount >= moduleList.size())
            break;
        if(authorityCount >= titleList.size())
            break;
        if(authorityCount >= authorityLevelList.size())
            break;
        m_authorityModel->addAuthority(permissionIdList[authorityCount].toInt(), moduleList[authorityCount], titleList[authorityCount], authorityLevelList[authorityCount].toInt());
    }
}

/**
 * @brief AuthorityLevelManagement::readTempFile : Opens logincheck.txt file and prints
 * @return
 */
int AuthorityLevelManagement::readTempFile()
{
    int grant = 0;
    QFile file("logincheck.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream out(&file);
    out >> grant;

    return grant;
}

/**
 * @brief AuthorityLevelManagement::getAuthorityModel : Returns authority model
 * @return : AuthorityLevelModel*
 */
AuthorityLevelModel* AuthorityLevelManagement::getAuthorityModel()
{
    return m_authorityModel;
}
