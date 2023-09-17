/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Combobox's Model

***************************************************************************/

#ifndef USERIOCOMBOBOXMODEL_H
#define USERIOCOMBOBOXMODEL_H

#include <QObject>

class UserIOComboboxModel : public QObject
{
    Q_PROPERTY(QString ParamName READ getParamName WRITE setParamName NOTIFY paramNameChanged)
    Q_PROPERTY(int ActualIndex READ getActualIndex WRITE setActualIndex NOTIFY actualIndexChanged)
    Q_OBJECT
public:
    explicit UserIOComboboxModel(QString a_ParamName,int a_Index,QObject *parent = nullptr);
    UserIOComboboxModel(QObject *parent = nullptr);
    UserIOComboboxModel(const UserIOComboboxModel& userIO,QObject *parent=nullptr);
    QString getParamName();
    void setParamName(QString a_ParamName);
    void setActualIndex(int a_Index);
    int getActualIndex();

private:
    QString m_ParamName;
    int m_ActualIndex;
signals:
    void paramNameChanged();
    void actualIndexChanged();

};
Q_DECLARE_METATYPE(UserIOComboboxModel)
#endif // USERIOCOMBOBOXMODEL_H
