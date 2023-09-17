/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Input Output Base class

***************************************************************************/
#ifndef IO_H
#define IO_H
#include <algorithm>
#include <QObject>
#include <QList>
#include <Header/userio_digitalparamsmodel.h>
#include <Header/userio_analogparamsmodel.h>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "Header/userio_structs.h"
#include <Header/communicationinterface.h>
#include <Header/userio_pindata.h>

extern QQmlApplicationEngine *enginePointer;
extern QQmlContext* context;

class IO:public QObject
{

    Q_OBJECT
public:
    Q_INVOKABLE void clearGarbage();
protected:
    QList<QObject*> m_IOPinsInformationList;
    QList<QObject*> m_IOPinsFunctionsList;
    QList<QObject*> m_ActualIOPinsFunctionsList;
    QList<int> m_DefaultPinsFunctionsList;
    QList<QObject*> m_GarbageComboboxList;//UserIOComboboxModel
    QList<QObject *> copyIOPinsFunctionsList(const QList<QObject *> &a_list);
    int searchIndexInComboboxlist(QList<QObject *> &qobjectListIfNoneSelected, int actualIndex);
    virtual void updatePinsInformationFromDBData(QList<PinData> a_DataList)=0;
    void getDataFromDB(QStringList);
    virtual void initializeIOPinsFunctions()=0;
    virtual void initializeIODefaultFunctions()=0;
    virtual void initializeIOPinsInformation()=0;
    virtual void refreshModel()=0;
};


#endif // IO_H
