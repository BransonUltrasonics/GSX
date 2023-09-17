/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO handles UI

***************************************************************************/
#ifndef USERIO_H
#define USERIO_H

#include <QObject>
#include <QStringList>
#include <QQmlContext>
#include <Header/userio_comboboxmodel.h>
#include <QList>
#include <Header/userio_ps.h>
#include <Header/userio_ac.h>
#include <Header/communicationinterface.h>

class UserIO:public QObject
{
    Q_OBJECT

public:
    explicit UserIO(QObject* ptr=nullptr);
    Q_INVOKABLE bool saveDataToDB();
    Q_INVOKABLE void getPSDataFromUI();
    Q_INVOKABLE void getACDataFromUI();
    Q_INVOKABLE void userIOInit();
    Q_INVOKABLE void sendIndicationToSCToReadDBData();

private: 
    USERIO m_UserIO;
    USERIO_CHECKSTATUS m_UserIOCheckStatus;
    UserIOPS m_UserIOPS;
    UserIOAC m_UserIOAC;

    void getUserIODataFromDB();
    bool m_InitOnce;
};

#endif // USERIO_H
