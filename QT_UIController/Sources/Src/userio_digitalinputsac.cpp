/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Actuator IO Digital Inputs

***************************************************************************/
#include "Header/userio_digitalinputsac.h"

/**
 * @brief DigitalInputsAC::DigitalInputsAC : Actuator input model name has been used
 */
DigitalInputsAC::DigitalInputsAC()
{
    initializeIOPinsFunctions();
    initializeActualIOPinsFunctions();
    initializeIODefaultFunctions();
    initializeIOPinsInformation();
}

/**
 * @brief DigitalInputsAC::initializeIOPinsFunctions
 */
void DigitalInputsAC::initializeIOPinsFunctions()
{
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED,ACTUATORINPUT_NOTUSED));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_PART_PRESENT,PARTPRESENT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_TOOLING_IN,TOOLING_IN));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOME_POSITION_IN, HOME_POSITION_IN));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_READY_POSITION_IN, READY_POSITION_IN));

}

void DigitalInputsAC::initializeActualIOPinsFunctions()
{
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED,ACTUATORINPUT_NOTUSED));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_PART_PRESENT,PARTPRESENT));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOME_POSITION_IN, HOME_POSITION_IN));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_READY_POSITION_IN, READY_POSITION_IN));
}

/**
 * @brief DigitalInputsAC::initializeIODefaultFunctions : Default values append to the List
 */
void DigitalInputsAC::initializeIODefaultFunctions()
{
    m_DefaultPinsFunctionsList.append(ACTUATORINPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(PARTPRESENT);
    m_DefaultPinsFunctionsList.append(ACTUATORINPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(ACTUATORINPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(ACTUATORINPUT_NOTUSED);
}

/**
 * @brief DigitalInputsAC::initializeIOPinsInformation : Pin number, pin name and status should be added to list
 */
void DigitalInputsAC::initializeIOPinsInformation()
{
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN1, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN2, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN3, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN9, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN10, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));

    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN1, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN2, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN3, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN9, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN10, !IS_CHECKBOX_CHECKED,ACTUATORINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));

    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        QObject *temp = m_IOPinsInformationList.at(index);
        UserIODigitalParamsModel *pObjIOIPPS = (UserIODigitalParamsModel *)temp;
        saveDataToStructs(index,pObjIOIPPS->getCurrentIndexCombobox(),pObjIOIPPS->getActiveStatus(),pObjIOIPPS->getIsChecked());
    }

    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        enginePointer->setObjectOwnership(m_IOPinsInformationList.at(index),QQmlEngine::CppOwnership);
    }
    refreshModel();
}

/**
 * @brief DigitalInputsAC::getDataFromDBUsingIds : Get the values based of particular ids
 */
void DigitalInputsAC::getDataFromDBUsingIds(QStringList a_List)
{
    getDataFromDB(a_List);
}

/**
 * @brief DigitalInputsAC::refreshModel
 */
void DigitalInputsAC::refreshModel()
{
    context->setContextProperty(AC_DIGITAL_INPUTS_PARAMS_CONTEXT_PROPERTY,QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief DigitalInputsAC::saveDataToStructs : Save the pin number and name to the structure
 * @param a_Index
 * @param a_Function
 * @param a_ActiveStatus
 */
void DigitalInputsAC::saveDataToStructs(int a_Index, int a_Function, int a_ActiveStatus,int a_CheckboxStatus)
{
    switch(a_Index)
    {
    case 0:
        m_stDigitalInputs.J710_01.AC_EVENT=(ActuatorInput)(a_Function);
        m_stDigitalInputs.J710_01.Status=a_ActiveStatus;
        m_stACDigitalInputsCheckStatus.J710_01 = a_CheckboxStatus;
        break;
    case 1:
        m_stDigitalInputs.J710_02.AC_EVENT=(ActuatorInput)(a_Function);
        m_stDigitalInputs.J710_02.Status=a_ActiveStatus;
        m_stACDigitalInputsCheckStatus.J710_02 = a_CheckboxStatus;
        break;
    case 2:
        m_stDigitalInputs.J710_03.AC_EVENT=(ActuatorInput)(a_Function);
        m_stDigitalInputs.J710_03.Status=a_ActiveStatus;
        m_stACDigitalInputsCheckStatus.J710_03 = a_CheckboxStatus;
        break;
    case 3:
        m_stDigitalInputs.J710_09.AC_EVENT=(ActuatorInput)(a_Function);
        m_stDigitalInputs.J710_09.Status=a_ActiveStatus;
        m_stACDigitalInputsCheckStatus.J710_09 = a_CheckboxStatus;
        break;
    case 4:
        m_stDigitalInputs.J710_10.AC_EVENT=(ActuatorInput)(a_Function);
        m_stDigitalInputs.J710_10.Status=a_ActiveStatus;
        m_stACDigitalInputsCheckStatus.J710_10 = a_CheckboxStatus;
        break;
    }
}

/**
 * @brief DigitalInputsAC::getStructData : Get the values from the structure
 * @return
 */
AC_DIGITALINPUT DigitalInputsAC::getStructData()
{
    return m_stDigitalInputs;
}

AC_DIGITALINPUT_CHECHSTATUS DigitalInputsAC::getACDigitalInputStructData()
{
    return m_stACDigitalInputsCheckStatus;
}

/**
 * @brief DigitalInputsAC::setUIAfterSaving : Map the values based on structure
 */
void DigitalInputsAC::setUIAfterSaving()
{
    QString toSend=EMPTY_STRING;
    toSend.append(QString::number(getStructData().J710_01.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_01.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalInputStructData().J710_01));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_02.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_02.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalInputStructData().J710_02));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_03.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_03.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalInputStructData().J710_03));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_09.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_09.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalInputStructData().J710_09));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_10.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_10.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalInputStructData().J710_10));
    if(toSend!=EMPTY_STRING)
    {
        QStringList query=toSend.split(COMMA);
        QList <PinData>tempDataList;
        for(int index=0;index<query.size();)
        {
            PinData temp_PinData;
            temp_PinData.setFunction(query.at(index).toInt());
            temp_PinData.setActiveStatus(query.at(index+1).toInt());
            temp_PinData.setCheckStatus(query.at(index+2).toInt());
            tempDataList.push_back(temp_PinData);
            index+=3;
        }
        updatePinsInformationFromDBData(tempDataList);
    }
}
