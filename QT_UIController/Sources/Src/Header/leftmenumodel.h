/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Left Menu Model

***************************************************************************/
#ifndef LEFTMENUMODEL_H
#define LEFTMENUMODEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QQmlContext>

#define LEFT_MENU_DASHBOARD  QObject::tr("DASHBOARD")
#define LEFT_MENU_RECIPE  QObject::tr("RECIPES")
#define LEFT_MENU_PRODUCTION  QObject::tr("PRODUCTION")
#define LEFT_MENU_ANALYTICS  QObject::tr("ANALYTICS")
#define LEFT_MENU_SYSTEM  QObject::tr("SYSTEM")

struct leftMenuItem
{
    QString m_Name;
    QString m_Image;
    QString m_ClickImage;
    QString m_URL;
    int m_Index;
};

class LeftMenuData:public QObject
{
    Q_OBJECT

public:
    QString qstrColor;
    QString qstrClickColor;
    QQmlContext *m_Context;
    QList <QObject *> m_LeftMenuModelDataDummy;
    QList <QObject *> m_LeftMenuModelData;
    QList <leftMenuItem> m_LeftMenuItems;

    LeftMenuData();

    Q_INVOKABLE void resetLeftMenu();
    Q_INVOKABLE void resetParent(QString parentName);
};

class LeftMenuModel: public QObject
{
    Q_OBJECT

public:
    QString m_MenuOption;
    QString m_MenuIcon;
    QString m_MenuAction;
    QString m_MenuColor;
    int m_MenuIndex;

    Q_PROPERTY(QString MenuOption READ GetMenuOption WRITE SetMenuOption NOTIFY OnMenuOptionChanged)
    Q_PROPERTY(QString MenuIcon READ GetMenuIcon WRITE SetMenuIcon NOTIFY OnMenuIconChanged)
    Q_PROPERTY(QString MenuAction READ GetMenuAction WRITE SetMenuAction NOTIFY OnMenuActionChanged)
    Q_PROPERTY(QString MenuColor READ GetMenuColor WRITE SetMenuColor NOTIFY OnMenuColorChanged)
    Q_PROPERTY(int MenuIndex READ GetMenuIndex WRITE SetMenuIndex NOTIFY OnMenuIndexChanged)

    LeftMenuModel(QString a_MenuOption, QString a_MenuIcon,QString a_MenuAction, QString a_Color, int a_Index, QObject *ptr=NULL);

    QString GetMenuOption();
    QString GetMenuAction();
    QString GetMenuIcon();
    QString GetMenuColor();
    int GetMenuIndex();
    void SetMenuIndex(int);
    void SetMenuOption(QString);
    void SetMenuAction(QString);
    void SetMenuIcon(QString);
    void SetMenuColor(QString);

signals:
    void OnMenuOptionChanged();
    void OnMenuIconChanged();
    void OnMenuActionChanged();
    void OnMenuColorChanged();
    void OnMenuIndexChanged();
};

#endif // LEFTMENUMODEL_H
