/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Left Menu Model

***************************************************************************/
#include "Header/leftmenumodel.h"
#include "Header/configuration.h"

extern QQmlContext* context;

#define LEFT_MENU_DASHBOARD  QObject::tr("DASHBOARD")
#define LEFT_MENU_RECIPE  QObject::tr("RECIPES")
#define LEFT_MENU_PRODUCTION  QObject::tr("PRODUCTION")
#define LEFT_MENU_ANALYTICS  QObject::tr("ANALYTICS")
#define LEFT_MENU_SYSTEM  QObject::tr("SYSTEM")
/**
 * @brief LeftMenuData::LeftMenuData
 */
LeftMenuData::LeftMenuData()
{
    resetLeftMenu();
}

/**
 * @brief LeftMenuData::resetLeftMenu : Create menu based on runtime configuration and update model
 */
void LeftMenuData::resetLeftMenu()
{
    qstrColor = "#ffffff";
    /*To update onClick menu color based on System Type*/
    qstrClickColor = (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX) ?
                      "#00AA7E" : "#68A4EB";
    leftMenuItem stleftMenuItem;
    m_LeftMenuItems.clear();
    if(Configuration::getInstance()->getFeatureFlagStatus(DASHBOARD_SCREEN) == 1)
    {
        stleftMenuItem.m_Name       =LEFT_MENU_DASHBOARD;
        stleftMenuItem.m_Image      ="qrc:/images/image/dashboard.svg";
        /*Image icon update based on System Type*/
        if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
        {
            stleftMenuItem.m_ClickImage ="qrc:/images/image/OnClickdashboard_ASX.svg";
        }
        else
        {
            stleftMenuItem.m_ClickImage ="qrc:/images/image/OnClickdashboard.svg";
        }
        stleftMenuItem.m_URL        ="Dashboard.qml";
        stleftMenuItem.m_Index      = 1;
        m_LeftMenuItems.append(stleftMenuItem);
    }

    stleftMenuItem.m_Name       =LEFT_MENU_RECIPE;
    stleftMenuItem.m_Image      ="qrc:/images/image/recipes.svg";
    /*Image icon update based on System Type*/
    if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/OnclickRecipes_ASX.svg";
    }
    else
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/OnclickRecipes.svg";
    }
    stleftMenuItem.m_URL        ="RecipeListControl.qml";
    stleftMenuItem.m_Index      = 2;
    m_LeftMenuItems.append(stleftMenuItem);

    stleftMenuItem.m_Name       =LEFT_MENU_PRODUCTION;
    stleftMenuItem.m_Image      ="qrc:/images/image/production.svg";
    /*Image icon update based on System Type*/
    if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/Onclickproduction_ASX.svg";
    }
    else
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/Onclickproduction.svg";
    }
    stleftMenuItem.m_URL        ="ProductionWindow.qml";
    stleftMenuItem.m_Index      = 3;
    m_LeftMenuItems.append(stleftMenuItem);

    stleftMenuItem.m_Name       =LEFT_MENU_ANALYTICS;
    stleftMenuItem.m_Image      ="qrc:/images/image/analytics.svg";
    /*Image icon update based on System Type*/
    if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/Onclickanalytics_ASX.svg";
    }
    else
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/Onclickanalytics.svg";
    }
    stleftMenuItem.m_URL        ="Analytics.qml";
    stleftMenuItem.m_Index      = 4;
    m_LeftMenuItems.append(stleftMenuItem);

    stleftMenuItem.m_Name       =LEFT_MENU_SYSTEM;
    stleftMenuItem.m_Image      ="qrc:/images/image/system.svg";
    /*Image icon update based on System Type*/
    if (Configuration::getInstance()->m_systemData.systemType == SYSTEM_TYPE_ASX)
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/Onclicksystem_ASX.svg";
    }
    else
    {
        stleftMenuItem.m_ClickImage ="qrc:/images/image/Onclicksystem.svg";
    }
    stleftMenuItem.m_URL        ="System.qml";
    stleftMenuItem.m_Index      = 5;
    m_LeftMenuItems.append(stleftMenuItem);

    context->setContextProperty("leftMenuModelData",QVariant::fromValue(m_LeftMenuModelDataDummy));
    for(int i=0;i<m_LeftMenuModelData.size();i++)
    {
        LeftMenuModel *temp =reinterpret_cast<LeftMenuModel*>(m_LeftMenuModelData.at(i));
        delete temp;
        temp = NULL;
    }

    m_LeftMenuModelData.clear();

    for(int i=0;i<m_LeftMenuItems.size();i++)
    {
        m_LeftMenuModelData.append(new LeftMenuModel( m_LeftMenuItems.at(i).m_Name,m_LeftMenuItems.at(i).m_Image,m_LeftMenuItems.at(i).m_URL,qstrColor,m_LeftMenuItems.at(i).m_Index));
    }

    context->setContextProperty("leftMenuModelData",QVariant::fromValue(m_LeftMenuModelData));
}

/**
 * @brief LeftMenuData::resetParent : Click menu information color changes
 * @param parentName
 */
void LeftMenuData::resetParent(QString parentName)
{
    context->setContextProperty("leftMenuModelData",QVariant::fromValue(m_LeftMenuModelDataDummy));
    int dwCnt = m_LeftMenuModelData.size();
    for(int i=0;i<dwCnt;i++)
    {
        LeftMenuModel *temp =reinterpret_cast<LeftMenuModel*>(m_LeftMenuModelData.at(i));
        if(temp->m_MenuOption.toLower() == parentName.toLower())
        {
            temp->m_MenuIcon = m_LeftMenuItems.at(i).m_ClickImage;
            temp->m_MenuColor = qstrClickColor;
        }
        else
        {
            temp->m_MenuIcon = m_LeftMenuItems.at(i).m_Image;
            temp->m_MenuColor = qstrColor;
        }
    }
    context->setContextProperty("leftMenuModelData",QVariant::fromValue(m_LeftMenuModelData));
}

/**
 * @brief LeftMenuModel::LeftMenuModel
 * @param a_MenuOption
 * @param a_MenuIcon
 * @param a_MenuAction
 * @param a_Color
 * @param a_Index
 * @param ptr
 */
LeftMenuModel::LeftMenuModel(QString a_MenuOption, QString a_MenuIcon, QString a_MenuAction, QString a_Color, int a_Index, QObject *ptr):QObject(ptr)
{
    m_MenuOption    = a_MenuOption;
    m_MenuIcon      = a_MenuIcon;
    m_MenuAction    = a_MenuAction;
    m_MenuColor     = a_Color;
    m_MenuIndex     = a_Index;
}

/**
 * @brief LeftMenuModel::GetMenuOption : Return menu option
 * @return
 */
QString LeftMenuModel::GetMenuOption()
{
    return m_MenuOption;
}

/**
 * @brief LeftMenuModel::SetMenuOption : Set menu option
 * @param a_menuoption
 */
void LeftMenuModel::SetMenuOption(QString a_menuoption)
{
    m_MenuOption = a_menuoption;
}

/**
 * @brief LeftMenuModel::GetMenuAction : Retun menu action on clicking
 * @return
 */
QString LeftMenuModel::GetMenuAction()
{
    return m_MenuAction;
}

/**
 * @brief LeftMenuModel::SetMenuAction : Set menu action while clicking
 * @param a_menuaction
 */
void LeftMenuModel::SetMenuAction(QString a_menuaction)
{
    m_MenuAction = a_menuaction;
}

/**
 * @brief LeftMenuModel::GetMenuIcon : Return menu icon
 * @return
 */
QString LeftMenuModel::GetMenuIcon()
{
    return m_MenuIcon;
}

/**
 * @brief LeftMenuModel::SetMenuIcon : Set menu icon
 * @param a_menuicon
 */
void LeftMenuModel::SetMenuIcon(QString a_menuicon)
{
    m_MenuIcon = a_menuicon;
}

/**
 * @brief LeftMenuModel::GetMenuColor : Get menu color
 * @return
 */
QString LeftMenuModel::GetMenuColor()
{
    return m_MenuColor;
}

/**
 * @brief LeftMenuModel::SetMenuColor : Set menu color
 * @param a_MenuColor
 */
void LeftMenuModel::SetMenuColor(QString a_MenuColor)
{
    m_MenuColor = a_MenuColor;
}

/**
 * @brief LeftMenuModel::GetMenuIndex : Return selected menu index
 * @return
 */
int LeftMenuModel::GetMenuIndex()
{
    return m_MenuIndex;
}

/**
 * @brief LeftMenuModel::SetMenuIndex : Set menu index
 * @param a_MenuIndex
 */
void LeftMenuModel::SetMenuIndex(int a_MenuIndex)
{
    m_MenuIndex = a_MenuIndex;
}
