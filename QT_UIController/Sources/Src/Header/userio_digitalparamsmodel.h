/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Digital Parameter's Model

***************************************************************************/

#ifndef USERIODIGITALPARAMSMODEL_H
#define USERIODIGITALPARAMSMODEL_H

#include <QObject>
#include <Header/userio_comboboxmodel.h>
#include <QList>

class UserIODigitalParamsModel : public QObject
{
    Q_PROPERTY(QString Title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool IsChecked READ getIsChecked WRITE setIsChecked NOTIFY isCheckedChanged)
    Q_PROPERTY(QVariant ComboBoxList READ getComboBoxList WRITE setComboBoxList NOTIFY comboBoxListChanged)
    Q_PROPERTY(bool ActiveStatus READ getActiveStatus WRITE setActiveStatus NOTIFY ActiveStatusChanged)
    Q_PROPERTY(int CurrentIndexCombobox READ getCurrentIndexCombobox WRITE setCurrentIndexCombobox NOTIFY CurrentIndexComboboxChanged)
    Q_PROPERTY(int IsEnabled READ getIsEnabled WRITE setIsEnabled NOTIFY IsEnabledChanged)
    Q_OBJECT
public:
    QString getTitle();
    bool getIsChecked();
    bool getIsEnabled();
    void setIsEnabled(bool a_IsEnabled);
    bool getActiveStatus();
    int getCurrentIndexCombobox();
    Q_INVOKABLE void setTitle(QString a_Title);
    Q_INVOKABLE void setIsChecked(bool a_IsChecked);
    Q_INVOKABLE QVariant getComboBoxList();
    Q_INVOKABLE void setComboBoxList(QVariant a_ComboBoxList);
    Q_INVOKABLE void setActiveStatus(bool a_ActiveStatus);
    Q_INVOKABLE void setCurrentIndexCombobox(int a_CurrentIndexCombobox);
    explicit UserIODigitalParamsModel(QString a_Title, bool a_IsChecked,int a_CurrentIndexCombobox, QVariant a_ComboBoxList, bool a_ActiveStatus,bool a_IsEnabled,QObject *parent = nullptr);
private:
    QString m_Title;
    QVariant m_ComboBoxList;//use UserIOComboboxModel
    bool m_IsChecked;
    bool m_ActiveStatus;
    bool m_IsEnabled;
    int m_currentIndexCombobox;

signals:
    void titleChanged();
    void isCheckedChanged();
    void comboBoxListChanged();
    void ActiveStatusChanged();
    void CurrentIndexComboboxChanged();
    void IsEnabledChanged();
};
Q_DECLARE_METATYPE(QList<UserIODigitalParamsModel*>)
#endif // USERIODIGITALPARAMSMODEL_H
