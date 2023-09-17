/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Smart Stack Source File

***************************************************************************/
#include <iostream>
using namespace  std;

#include "Header/smartstack.h"
#include <QQmlContext>
#include <QDebug>
extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern Recipe *recipe;
extern QObject * object;

void SmartStack::smartStackInit()
{
    createFixedStackModel();
    createCurrentStackModel();
    createNewStackModel();
}
void SmartStack::createFixedStackModel()
{
    context->setContextProperty("fixedStackModel", QVariant::fromValue(m_DummyList));
    QString qstrParams = getFixedStackModel();
    for(int element=0; element <m_FixedModelList.size() ; element++)
    {
        QObject* ptrTemp = dynamic_cast<SmartStack*>(m_FixedModelList.at(element));
        delete ptrTemp;
        ptrTemp = NULL;
    }
    m_FixedModelList.clear();
    QStringList qstrParamsList = qstrParams.split(",");
    for(int dwIndex=0;dwIndex<qstrParamsList.size();dwIndex++)
    {
        SmartStack* ptrTemp = new SmartStack;

        ptrTemp->m_ParameterName = qstrParamsList[dwIndex];

        m_FixedModelList.append(ptrTemp);
    }
    context->setContextProperty("fixedStackModel", QVariant::fromValue(m_FixedModelList));
}
void SmartStack::createCurrentStackModel()
{
    context->setContextProperty("CurrentStackModel", QVariant::fromValue(m_DummyList));
    for(int element=0; element <m_CurrentModelList.size() ; element++)
    {
        QObject* ptrTemp = dynamic_cast<SmartStack*>(m_CurrentModelList.at(element));
        delete ptrTemp;
        ptrTemp = NULL;
    }
    m_CurrentModelList.clear();
    QStringList qstrParamsList = qstrCurrentSmartStack.split(",");
    for(int dwIndex=0;dwIndex<qstrParamsList.size();dwIndex++)
    {
        SmartStack* ptrTemp = new SmartStack;

        ptrTemp->m_ParameterName = qstrParamsList[dwIndex];

        m_CurrentModelList.append(ptrTemp);
    }
    context->setContextProperty("CurrentStackModel", QVariant::fromValue(m_CurrentModelList));
}
void SmartStack::createNewStackModel()
{
    context->setContextProperty("NewStackModel", QVariant::fromValue(m_DummyList));
    for(int element=0; element <m_NewModelList.size() ; element++)
    {
        QObject* ptrTemp = dynamic_cast<SmartStack*>(m_NewModelList.at(element));
        delete ptrTemp;
        ptrTemp = NULL;
    }
    m_NewModelList.clear();
    QStringList qstrParamsList = qstrNewSmartStack.split(",");
    for(int dwIndex=0;dwIndex<qstrParamsList.size();dwIndex++)
    {
        SmartStack* ptrTemp = new SmartStack;
        ptrTemp->m_ParameterName = qstrParamsList[dwIndex];
        m_NewModelList.append(ptrTemp);
    }
    context->setContextProperty("NewStackModel", QVariant::fromValue(m_NewModelList));
}
QString SmartStack::getFixedStackModel()
{
    QString params = "Stack Name:,"
                     "Stack Serial #:,"
                     "Amplitude,"
                     "Voltage,"
                     "Birth Date,"
                     "EDP,"
                     "Model";
    return params;
}
void SmartStack::sendChangeStackReqToSC()
{
    char *buff = NULL;
    bIsChangeStackClicked = true;
    CIObj.sendMessage(SCBL_CHANGE_STACK_REQ ,UIC_CHANGE_STACK_RSP,buff,0);

    char resetStatus[4]={'\0'};
    CIObj.recivedMessage(UIC_CHANGE_STACK_RSP,resetStatus,1);
//    changeStackInd("123456,90,520,28/05/2018,100-098-370,40kHz/800W");
}
void SmartStack::sendCancelStackReqToSC()
{
    char *buff = NULL;
    bIsChangeStackClicked = false;
    CIObj.sendMessage(SCBL_CHANGE_STACK_CANCEL_REQ ,UIC_CHANGE_STACK_CANCEL_RSP,buff,0);

    char resetStatus[4]={'\0'};
    CIObj.recivedMessage(UIC_CHANGE_STACK_CANCEL_RSP,resetStatus,1);
}
void SmartStack::changeStackInd(QString strSmartStack)
{
    qstrNewSmartStack = "DEFAULT," + strSmartStack;
    QStringList lSmartStack = strSmartStack.split(",");
    stSmartStack.StackSerialNum = lSmartStack[0].toInt();
    stSmartStack.Amplitude = lSmartStack[1].toInt();
    stSmartStack.Voltage = lSmartStack[2].toInt();
    strcpy(stSmartStack.BirthDate,lSmartStack[3].toStdString().c_str());
    strcpy(stSmartStack.EDP,lSmartStack[4].toStdString().c_str());
    strcpy(stSmartStack.Model,lSmartStack[5].toStdString().c_str());
    createNewStackModel();
    if(bIsChangeStackClicked == true)
    {
//        QMetaObject::invokeMethod(object,"turnOffToInstallStack");
        recipe->updateStatusText(TURNOFF_TO_INSTALL_STACK);
    }
    else
        QMetaObject::invokeMethod(object,"stackChangeConfirmation");
    bIsChangeStackClicked = false;
}
void SmartStack::clearNewStackInfo()
{
    qstrNewSmartStack = " , , , , , , ";
    createNewStackModel();
}
void SmartStack::copyNewStackToCurrentStack()
{
    qstrCurrentSmartStack = qstrNewSmartStack;
    createCurrentStackModel();
    clearNewStackInfo();
}

bool SmartStack::getIsChangeStackClickedStatus()
{
    return bIsChangeStackClicked;
}
void SmartStack::sendClearMemoryReqToSC()
{
    char *buff = NULL;
    CIObj.sendMessage(SCBL_CLEAR_MEM_OFFSET_REQ ,UIC_CLEAR_MEM_OFFSET_RSP,buff,0);
    char resetStatus[4]={'\0'};
    CIObj.recivedMessage(UIC_CLEAR_MEM_OFFSET_RSP,resetStatus,1);

    int dwStatus = resetStatus[0]-48;
    if(dwStatus)
        recipe->updateStatusText(MEMORY_CLEARED);
    else
        recipe->updateStatusText(MEMORY_NOTCLEARED);
}
void SmartStack::memoryClearedInd(QString strMemClearStatus)
{
    int dwStatus;
    char chReset = QString(strMemClearStatus[0]).toStdString()[0];
    dwStatus = chReset - 48;
    if(dwStatus)
        recipe->updateStatusText(MEMORY_CLEARED);
    else
       recipe->updateStatusText(MEMORY_NOTCLEARED);
}
