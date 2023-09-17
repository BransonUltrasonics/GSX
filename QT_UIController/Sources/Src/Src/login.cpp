/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Login Page

***************************************************************************/

#include <QDateTime>
#include "Header/login.h"
#include "Header/communicationinterface.h"
#include "Header/sysconfigdata.h"
#include "Header/userio.h"

static constexpr int LOGIN_SUCESS  = 1;
static constexpr int LOGIN_FAILURE  = 0;

extern CommunicationInterface CIObj;
extern UserIO *userIO;

/**
 * @brief Login::Login
 * @param parent
 */
Login::Login(QObject *parent) : QObject(parent)
{
    QObject::connect(&CIObj , &CommunicationInterface::webserviceUserLoggedInfo , this , &Login::setWebserviceUserLoggedInfo);
    m_password = "";
    m_RestrictedScreenType = (int)WSRestrictionScreen::WSRestrictionScreenEnum::NO_RESTRICTED;
}

/**
 * @brief Login::loginValidate : Vlidating username and password
 * @param a_userName
 * @param a_password
 * @return
 */
int Login::loginValidate ( QString a_userName, QString a_password )
{
    QString userDetailsResponse = "";
    QString requestMessage = "";

    requestMessage = a_userName + "," + a_password;

    CIObj.sendMessage(REQ_GET_USER_DETAILS, RES_GET_USER_DETAILS, requestMessage);
    CIObj.recivedMessage(RES_GET_USER_DETAILS, userDetailsResponse);

    QStringList userDetails =  userDetailsResponse.split(",");
    if (!userDetailsResponse.isEmpty() && (userDetails.size()%6)==0)
    {
            m_pUserProfile = std::make_shared<User>(userDetails[0], "", userDetails[1], userDetails[2]
                    , userDetails[3], userDetails[4], userDetails[5]);

        return static_cast<int>(SUCESS);
    }
    else if(userDetailsResponse.trimmed() == "Locked")
    {
        return static_cast<int>(ERROR_USER_LOCKED);
    }
    else
    {
        return static_cast<int>(ERROR_LOGIN_FAILURE);
    }
}

/**
 * @brief Login::passwordEnable
 * @param a_userNameEmpty
 * @return
 */
bool Login:: passwordEnable ( QString a_userNameEmpty  )
{
    if ( a_userNameEmpty.isEmpty() == true )
    {
        return true;
    }
    return false;
}

/**
 * @brief Login::logout
 */
void Login::logout ()
{
    m_password = "";
}

/**
 * @brief Login::getUserLevel : Return userlevel according to user
 * @return
 */
QString Login::getUserLevel()
{
    if(m_pUserProfile)
        return m_pUserProfile->getUserLevel();
    else
        return QString("");
}

/**
 * @brief Login::setUserDetails : Set user operator mode
 * @param userName
 */
void Login::setUserDetails(QString userName)
{
    if (!m_pUserProfile)
    {
        m_pUserProfile = std::make_shared<User>(userName, "","Executive", "Enabled", "", "", "");
    }
}

/**
 * @brief Login::validatePasswordExpiry : Checking the password expiry date
 * @return
 */
int Login::validatePasswordExpiry()
{
    //bool validatePasswordExpiry = false;
    QDateTime currentDate = SysConfigData::getInstance()->getDateAndTime();
    QDateTime lastChangedPassword = QDateTime::fromString(m_pUserProfile->getLastChangedPassword(), "yyyy/MM/dd hh:mm:ss");

    //auto addExpirydays = lastChangedPassword.addDays(90);
    auto differenceInDays = lastChangedPassword.daysTo(currentDate);
    return differenceInDays;
}

/**
 * @brief Login::isFirstLogin : Return when user first login
 * @return
 */
bool Login::isFirstLogin() const
{
    if (m_pUserProfile->getLastModified() == m_pUserProfile->getLastLoginDate())
           return true;
    else
        return false;
}

/**
 * @brief Login::updateUserLogin : Updated infromation send to database
 */
void Login::updateUserLogin()
{
    QString userDetailsResponse = "";
    QString requestMessage = "";
    QDateTime currentDate = SysConfigData::getInstance()->getDateAndTime();
    m_pUserProfile->setLastLoginDate(currentDate.toString("yyyy/MM/dd hh:mm:ss"));

    requestMessage = m_pUserProfile->getUserId() + "," + currentDate.toString("yyyy/MM/dd hh:mm:ss");
    CIObj.sendMessage(REQ_UPDATE_USER_LOGIN, RES_UPDATE_USER_LOGIN, requestMessage);
    CIObj.recivedMessage(RES_UPDATE_USER_LOGIN, userDetailsResponse);
}

/**
 * @brief Login::webserviceLoginSetup : Setup after a webservice login
 */
void Login::webserviceLoginSetup()
{
        userIO->userIOInit();
        userIO->sendIndicationToSCToReadDBData();
}

/**
 * @brief Login::setUserOnModifyScreen : Sets the actual user that is being modified on the UI
 */
void Login::setUserOnModifyScreen(QString UserOnModify)
{
    m_UserOnModifyScreen = UserOnModify;
}

/**
 * @brief Login::setWebserviceUserLoggedInfo : set webservices logged User information
 */
void Login::setWebserviceUserLoggedInfo(QString WebServicesLoggedUserInfo)
{
    QStringList WSUserInfo = WebServicesLoggedUserInfo.split(",");
    QString qstrResponse = LOGIN_DENIED;

    if(LOG_IN == WSUserInfo.at(WS_LOG_STATUS).toInt())
    {
        //Check if the actual screen allows the Webservices to login
        if ((int)WSRestrictionScreen::WSRestrictionScreenEnum::NO_RESTRICTED == m_RestrictedScreenType)
            qstrResponse = LOGIN_ALLOWED;
        else if (((int)WSRestrictionScreen::WSRestrictionScreenEnum::USER_MANAGEMENT == m_RestrictedScreenType) && (m_UserOnModifyScreen != WSUserInfo.at(WS_USER_NAME)))
            qstrResponse = LOGIN_ALLOWED;
        else if (((int)WSRestrictionScreen::WSRestrictionScreenEnum::USER_OPERATOR_AUTHORITY == m_RestrictedScreenType) && ((int)HelperEnums::UserLevel::OPERATOR != WSUserInfo.at(WS_USER_LEVEL).toInt()))
            qstrResponse = LOGIN_ALLOWED;
        else if (((int)WSRestrictionScreen::WSRestrictionScreenEnum::CHANGE_PASSWORD == m_RestrictedScreenType) && (m_pUserProfile->getUserId() != WSUserInfo.at(WS_USER_NAME)))
            qstrResponse = LOGIN_ALLOWED;

        if (LOGIN_ALLOWED == qstrResponse)
        {
            m_WSLoggedUserName = WSUserInfo.at(WS_USER_NAME);
            m_WSUserLevelStatus = WSUserInfo.at(WS_USER_LEVEL).toInt();
            m_bIsWebServicesLogged = WSUserInfo.at(WS_LOG_STATUS).toInt();

            webserviceLoginSetup();
        }
        //Notify if the request was granted or not based on m_RestrictedScreenType
        CIObj.sendMessage(SCBL_WS_LOGIN_PERMISSION_RES, SCBL_WS_LOGIN_PERMISSION_RES, qstrResponse);
    }
    else if(LOG_OUT == WSUserInfo.at(WS_LOG_STATUS).toInt())
    {
        m_WSLoggedUserName = WSUserInfo.at(WS_USER_NAME);
        m_WSUserLevelStatus = WSUserInfo.at(WS_USER_LEVEL).toInt();
        m_bIsWebServicesLogged = WSUserInfo.at(WS_LOG_STATUS).toInt();
    }
}

/**
 * @brief Login::getWSUserAndLevelLogged : After a UI login, asks the SC if there is someone logged into the
 *                                         webservices, its username and level. If no one is logged in, the SC
 *                                         will respond with "WS_N/A" as user and NO_LEVEL(0) as its level.
 */
void Login::getWSUserAndLevelLogged()
{
    QString WSUserInfoBuffer;
    CIObj.sendMessage(REQ_GET_WEBSERVICES_USER_AND_LEVEL, RES_GET_WEBSERVICES_USER_AND_LEVEL, "");
    CIObj.recivedMessage(RES_GET_WEBSERVICES_USER_AND_LEVEL,WSUserInfoBuffer);

    if(!WSUserInfoBuffer.isEmpty())
    {
        QStringList WSUserInfo = WSUserInfoBuffer.split(",");

        m_WSLoggedUserName = WSUserInfo.at(WS_USER_NAME);
        m_WSUserLevelStatus = WSUserInfo.at(WS_USER_LEVEL).toInt();

        if(LOG_IN == WSUserInfo.at(WS_LOG_STATUS).toInt())
            m_bIsWebServicesLogged = true;
        else if(LOG_OUT == WSUserInfo.at(WS_LOG_STATUS).toInt())
            m_bIsWebServicesLogged = false;
    }
    else
        return;

}

/**
 * @brief Login::setWSRestrictedScreens : Set which is the current active screen and if it restircts
 *                                        WS login or not.
 */
void Login::setWSRestrictedScreens(int restrictedScreen)
{
    m_RestrictedScreenType = restrictedScreen;
}

/**
 * @brief Login::GetLoginUserName : Return user name
 * @return
 */
QString Login::GetLoginUserName()
{
    return m_pUserProfile->getUserId();
}

/**
 * @brief Login::GetPassword : Return password
 * @return
 */
QString Login::GetPassword()
{
    return m_password;
}

/**
 * @brief Login::GetUserType : Return user type
 * @return
 */
QString Login::GetUserType()
{
    return m_pUserProfile->getUserLevel();
}

/**
 * @brief Login::GetUserStatus : Return user level
 * @return
 */
QString Login::GetUserStatus()
{
    return m_pUserProfile->getUserStatus();
}

/**
 * @brief Login::setAuthorityCheckStatus : Set Authority check status
 * @param status
 */
void Login::setAuthorityCheckStatus(int status)
{
    isAuthorityCheckReq = status;
}

/**
 * @brief Login::getAuthorityCheckStatus : Get user authority check
 * @return
 */
int Login::getAuthorityCheckStatus()
{
    return isAuthorityCheckReq;
}

/**
 * @brief Login::getLastChangedPassword : Return last change password
 * @return
 */
QString Login::getLastChangedPassword()
{
    return m_pUserProfile->getLastChangedPassword();
}

/**
 * @brief Login::getWSLoggedUserName : Return actual WS user logged
 * @return
 */
QString Login::getWSLoggedUserName()
{
    return m_WSLoggedUserName;
}

/**
 * @brief Login::getWSLoggedUserName : Return user being modified on Modify screen
 * @return
 */
QString Login::getUserOnModifyScreen()
{
    return m_UserOnModifyScreen;
}

/**
 * @brief Login::getWSUserLevelStatus : Return actual WS user level
 * @return
 */
int Login::getWSUserLevelStatus()
{
    return m_WSUserLevelStatus;
}

/**
 * @brief Login::getIsWebServicesLogged : Return WebServices Log status
 * @return
 */
bool Login::getIsWebServicesLogged()
{
    return m_bIsWebServicesLogged;
}
