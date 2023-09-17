/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Analog Parameter's Model

***************************************************************************/

#ifndef USERIOANALOGPARAMSMODEL_H
#define USERIOANALOGPARAMSMODEL_H

#include <QObject>

#include <QObject>
#include <Header/userio_comboboxmodel.h>
#include <QList>
#include <QVariant>
class UserIOAnalogParamsModel : public QObject
{
    Q_PROPERTY(QString Title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool IsChecked READ getIsChecked WRITE setIsChecked NOTIFY isCheckedChanged)
    Q_PROPERTY(QVariant ComboBoxList READ getComboBoxList WRITE setComboBoxList NOTIFY comboBoxListChanged)
    Q_PROPERTY(int CurrentIndexCombobox READ getCurrentIndexCombobox WRITE setCurrentIndexCombobox NOTIFY CurrentIndexComboboxChanged)
    Q_PROPERTY(int IsEnabled READ getIsEnabled WRITE setIsEnabled NOTIFY IsEnabledChanged)
    Q_OBJECT
public:
    QString getTitle();
    Q_INVOKABLE void setTitle(QString a_Title);
    bool getIsChecked();
    Q_INVOKABLE void setIsChecked(bool a_IsChecked);
    Q_INVOKABLE QVariant getComboBoxList();
    Q_INVOKABLE void setComboBoxList(QVariant a_ComboBoxList);
    bool getIsEnabled();
    void setIsEnabled(bool a_IsEnabled);
    int getCurrentIndexCombobox();
    Q_INVOKABLE void setCurrentIndexCombobox(int a_CurrentIndexCombobox);
    explicit UserIOAnalogParamsModel(QString a_Title, bool a_IsChecked,int a_CurrentIndexCombobox, QVariant a_ComboBoxList,bool a_IsEnabled,QObject *parent = nullptr);
    UserIOAnalogParamsModel( QObject *parent=nullptr);
    UserIOAnalogParamsModel(const UserIOAnalogParamsModel &userio, QObject *parent=nullptr);
private:
    QString m_Title;
    bool m_IsChecked;
    QVariant m_ComboBoxList;//use UserIOComboboxModel
    int m_currentIndexCombobox;
    bool m_IsEnabled;
signals:
    void titleChanged();
    void isCheckedChanged();
    void comboBoxListChanged();
    void CurrentIndexComboboxChanged();
    void IsEnabledChanged();

};
Q_DECLARE_METATYPE(UserIOAnalogParamsModel)
#endif // USERIOANALOGPARAMSMODEL_H
