/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

   Menu Handler

***************************************************************************/

#include "Header/menuhandler.h"
#include "Header/authoritylevelmanagement.h"
#include "Header/authoritylevelmodel.h"

extern QQmlContext* context;

/**
 * @brief UserPrivilege::m_pUserPrivilege
 */
UserPrivilege* UserPrivilege::m_pUserPrivilege = nullptr;

/**
 * @brief UserPrivilege::UserPrivilege : Update the user privilage request
 */
UserPrivilege::UserPrivilege()
{
    context->setContextProperty("authorityLevelManagement" , &authorityLevelObj);
}

/**
 * @brief UserPrivilege::~UserPrivilege
 */
UserPrivilege::~UserPrivilege()
{
    if (m_pUserPrivilege)
        delete m_pUserPrivilege;
}

/**
 * @brief UserPrivilege::UserPrivilege
 * @param parent
 */
UserPrivilege *UserPrivilege::getInstance()
{
    if (!m_pUserPrivilege)
    {
        m_pUserPrivilege = new UserPrivilege();
    }
    return m_pUserPrivilege;
}

/**
 * @brief UserPrivilege::isPermissionAllowed : Update the permission log status
 * @param userLevel
 * @param privilege
 * @return
 */
bool UserPrivilege::isPermissionAllowed(int userLevel, int privilege)
{
    return checkPermission(userLevel, privilege);
}

/**
 * @brief UserPrivilege::init
 * @param userLevel
 */
void UserPrivilege::init(int userLevel)
{
    Q_UNUSED(userLevel)
    opertorRestrictedPrivilegeList.clear();
    technicianRestrictedPrivilegeList.clear();
    supervisorRestrictedPrivilegeList.clear();
    initOperatorRestrictedPrivileges();
    initTechnicianRestrictedPrivileges();
    initSupervisorRestrictedPrivileges();
}

/**
 * @brief UserPrivilege::checkPermission : Check permision userlevel status
 * @param userLevel
 * @param privilege
 * @return
 */
bool UserPrivilege::checkPermission(int userLevel, int privilege)
{
    bool permissionAlowed = false;

    HelperEnums::UserLevel eUserLevel = static_cast<HelperEnums::UserLevel>(userLevel);

    switch (eUserLevel)
    {
    case HelperEnums::UserLevel::EXECUTIVE:
        permissionAlowed = true;
        break;
    case HelperEnums::UserLevel::SUPERVISOR:
        permissionAlowed = checkSupervisorPrivilegeList(privilege);
        break;
    case HelperEnums::UserLevel::TECHNICIAN:
        permissionAlowed = checkTechnicianPrivilegeList(privilege);
        break;
    case HelperEnums::UserLevel::OPERATOR:
        if(privilege == Features::FeaturesEnum::CLEAR_MEMORY || privilege == Features::FeaturesEnum::SCAN_SEEK_TEST)
        {
            permissionAlowed = 0;
        }
        else
        {
            permissionAlowed = checkOpertorPrivilegeList(privilege);
        }
        break;
    default:
        break;
    }

    return permissionAlowed;
}

/**
 * @brief UserPrivilege::initOperatorRestrictedPrivileges : Operator based enable features
 */
void UserPrivilege::initOperatorRestrictedPrivileges()
{
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::HORN_DOWN);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::DIAGONISTICS);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::CLEAR_MEMORY);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SCAN_SEEK_TEST);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::MODIFY_WELD_PARAMS_WITH_MIN_MAX);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::VALIDATE_AND_LOCK_WELD_RECIPE);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::MODIFY_LOCKED_WELD_RECIPE);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::ADD_OR_MODIFY_USER_ID_SETUP);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SET_AUTHORITY_CHECK_TO_YES);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SET_MEMORY_FULL_ACTION_TO_YES);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::GENERAL);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::USER_LIST);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::USER_IO);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::USER_AUTHORITY);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::ALARAM_MANAGEMENT);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::IP_CONFIGURATION);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::COMPONENTS);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::EXTERNAL_STATUS);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::RESET_WELD_COUNTER);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SYSTEM_DATA);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::QUICK_CALIBRATION);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::BATCH_SETUP);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SYSTEM_CLOCK);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::NEW_SAVE_COPY_VALIDATED_UNVALIDATED_RECIPE);
    opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SECURITY);

    authorityLevelObj.handleAuthorityOption();

    auto authorityModel = authorityLevelObj.getAuthorityModel();
    auto authorityCount = authorityModel->rowCount();

    for (auto count = 0; count < authorityCount; ++count)
    {
        auto permissionId = authorityModel->get(count).value("permissionId").toInt();
        auto grant =  authorityModel->get(count).value("grant").toInt();
        QString module = authorityModel->get(count).value("module").toString();

        if (!grant)
        {
            if(module == QObject::tr("run unvalidated weld recipe"))
            {
                opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::RUN_UNVALIDATED_WELD_RECIPE);
            }
            else if(module == QObject::tr("reset alarms"))
            {
                opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::RESET_ALARAMS);
            }
            else if(module == QObject::tr("set as active"))
            {
                opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SET_AS_ACTIVE);
            }
            else if(module == QObject::tr("reset batch counter"))
            {
                opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::RESET_BATCH_COUNTER);
            }
            else if(module == QObject::tr("quick calibration"))
            {
                opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::QUICK_CALIBRATION);
            }
            else if(module == QObject::tr("actuator setup"))
            {
                opertorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::WELD_SETUP);
            }
        }
    }
}

/**
 * @brief UserPrivilege::initTechnicianRestrictedPrivileges : Technician based enable features
 */
void UserPrivilege::initTechnicianRestrictedPrivileges()
{
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::VALIDATE_AND_LOCK_WELD_RECIPE);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::MODIFY_LOCKED_WELD_RECIPE);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::ADD_OR_MODIFY_USER_ID_SETUP);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SET_AUTHORITY_CHECK_TO_YES);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SET_MEMORY_FULL_ACTION_TO_YES);

    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::GENERAL);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::USER_LIST);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::USER_AUTHORITY);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::ALARAM_MANAGEMENT);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::COMPONENTS);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::EXTERNAL_STATUS);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SYSTEM_CLOCK);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SECURITY);
    technicianRestrictedPrivilegeList.push_back(Features::FeaturesEnum::RESET_WELD_COUNTER);
}

void UserPrivilege::initSupervisorRestrictedPrivileges()
{
    supervisorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SET_AUTHORITY_CHECK_TO_YES);
    supervisorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SET_MEMORY_FULL_ACTION_TO_YES);
    supervisorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::ADD_OR_MODIFY_USER_ID_SETUP);
    supervisorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::USER_LIST);
    supervisorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::SYSTEM_CLOCK);
    supervisorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::USER_AUTHORITY);
    supervisorRestrictedPrivilegeList.push_back(Features::FeaturesEnum::RESET_WELD_COUNTER);
}

/**
 * @brief UserPrivilege::checkOpertorPrivilegeList
 * @param privilege
 * @return
 */
bool UserPrivilege::checkOpertorPrivilegeList(int privilege)
{
    if (opertorRestrictedPrivilegeList.indexOf(privilege) == -1)
        return true;
    else
        return false;
}

/**
 * @brief UserPrivilege::checkSupervisorPrivilegeList : Set supervisor privilege list
 * @param privilege
 * @return
 */
bool UserPrivilege::checkSupervisorPrivilegeList(int privilege)
{
    if(supervisorRestrictedPrivilegeList.indexOf(privilege) == -1)
        return true;
    else
        return false;
}

/**
 * @brief UserPrivilege::checkTechnicianPrivilegeList : checking the technician privilege list
 * @param privilege
 * @return
 */
bool UserPrivilege::checkTechnicianPrivilegeList(int privilege)
{
    if (technicianRestrictedPrivilegeList.indexOf(privilege) == -1)
        return true;
    else
        return false;
}

