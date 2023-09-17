/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Actuator IO Digital Outputs

***************************************************************************/
#include "Header/userio_DigitalOutputsAC.h"

/**
 * @brief DigitalOutputsAC::DigitalOutputsAC
 */
DigitalOutputsAC::DigitalOutputsAC()
{
    initializeIOPinsFunctions();
    initializeActualIOPinsFunctions();
    initializeIODefaultFunctions();
    initializeIOPinsInformation();
}

/**
 * @brief DigitalOutputsAC::initializeIOPinsFunctions : Initialize the default values
 */
void DigitalOutputsAC::initializeIOPinsFunctions()
{

    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED, ACTUATOROUTPUT_NOTUSED));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOMEPOSITION_ULS, HOMEPOSITION_OUT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_READY_POSITION, READY_POSITION_OUT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_PB_RELEASE, PB_RELEASE));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_TOOLING_OUT, TOOLING_OUT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_SOLENOID_VALVE, SOLENOID_VALVE));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOLD_ACTIVE, HOLD_ACTIVE));
}

void DigitalOutputsAC::initializeActualIOPinsFunctions()
{
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED, ACTUATOROUTPUT_NOTUSED));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOMEPOSITION_ULS, HOMEPOSITION_OUT));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_READY_POSITION, READY_POSITION_OUT));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_PB_RELEASE, PB_RELEASE));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOLD_ACTIVE, HOLD_ACTIVE));
}

/**
 * @brief DigitalOutputsAC::initializeIODefaultFunctions : Initialize the default values to combobox
 */
void DigitalOutputsAC::initializeIODefaultFunctions()
{
    m_DefaultPinsFunctionsList.append(ACTUATOROUTPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(ACTUATOROUTPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(ACTUATOROUTPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(ACTUATOROUTPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(ACTUATOROUTPUT_NOTUSED);
}

/**
 * @brief DigitalOutputsAC::initializeIOPinsInformation : Initialize the default values to pin names and numbers
 */
void DigitalOutputsAC::initializeIOPinsInformation()
{
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN5,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN6,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN7,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN13,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN14,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));

    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN5,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN6,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN7,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN13,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(AC_PIN14,!IS_CHECKBOX_CHECKED,ACTUATOROUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS,IS_PIN_ENABLED));

    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        QObject *temp = m_IOPinsInformationList.at(index);
        UserIODigitalParamsModel *pObjIOIPPS = (UserIODigitalParamsModel *)temp;
        saveDataToStructs(index,pObjIOIPPS->getCurrentIndexCombobox(),pObjIOIPPS->getActiveStatus(),pObjIOIPPS->getIsChecked());
    }
    //giving ownership of QObject* to c++
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        enginePointer->setObjectOwnership(m_IOPinsInformationList.at(index),QQmlEngine::CppOwnership);
    }
    refreshModel();
}

/**
 * @brief DigitalOutputsAC::getDataFromDBUsingIds : Get the values from DB
 */
void DigitalOutputsAC::getDataFromDBUsingIds(QStringList a_List)
{
    getDataFromDB(a_List);
}

/**
 * @brief DigitalOutputsAC::refreshModel : Refresh the model to get he updated changes
 */
void DigitalOutputsAC::refreshModel()
{
    context->setContextProperty(AC_DIGITAL_OUTPUTS_PARAMS_CONTEXT_PROPERTY,QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief DigitalOutputsAC::saveDataToStructs : Saving the values to structures
 * @param a_Index
 * @param a_Function
 * @param a_ActiveStatus
 */
void DigitalOutputsAC::saveDataToStructs(int a_Index, int a_Function, int a_ActiveStatus,int a_CheckboxStatus)
{
    switch(a_Index)
    {
    case 0:
        m_stDigitalOutputs.J710_05.AC_EVENT=(ActuatorOutput)(a_Function);
        m_stDigitalOutputs.J710_05.Status=a_ActiveStatus;
        m_stACDigitalOutputsCheckStatus.J710_05 = a_CheckboxStatus;
        break;
    case 1:
        m_stDigitalOutputs.J710_06.AC_EVENT=(ActuatorOutput)(a_Function);
        m_stDigitalOutputs.J710_06.Status=a_ActiveStatus;
        m_stACDigitalOutputsCheckStatus.J710_06 = a_CheckboxStatus;
        break;
    case 2:
        m_stDigitalOutputs.J710_07.AC_EVENT=(ActuatorOutput)(a_Function);
        m_stDigitalOutputs.J710_07.Status=a_ActiveStatus;
        m_stACDigitalOutputsCheckStatus.J710_07 = a_CheckboxStatus;
        break;
    case 3:
        m_stDigitalOutputs.J710_13.AC_EVENT=(ActuatorOutput)(a_Function);
        m_stDigitalOutputs.J710_13.Status=a_ActiveStatus;
        m_stACDigitalOutputsCheckStatus.J710_13 = a_CheckboxStatus;
        break;
    case 4:
        m_stDigitalOutputs.J710_14.AC_EVENT=(ActuatorOutput)(a_Function);
        m_stDigitalOutputs.J710_14.Status=a_ActiveStatus;
        m_stACDigitalOutputsCheckStatus.J710_14 = a_CheckboxStatus;
        break;
    }
}

/**
 * @brief DigitalOutputsAC::getStructData : Get the values from the structure
 * @return
 */
AC_DIGITALOUTPUT DigitalOutputsAC::getStructData()
{
    return m_stDigitalOutputs;
}

AC_DIGITALOUTPUT_CHECKSTATUS DigitalOutputsAC::getACDigitalOutputStructData()
{
    return m_stACDigitalOutputsCheckStatus;
}

/**
 * @brief DigitalOutputsAC::setUIAfterSaving : Maping the function to set the default values
 */
void DigitalOutputsAC::setUIAfterSaving()
{
    QString toSend=EMPTY_STRING;
    toSend.append(QString::number(getStructData().J710_05.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_05.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalOutputStructData().J710_05));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_06.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_06.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalOutputStructData().J710_06));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_07.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_07.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalOutputStructData().J710_07));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_13.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_13.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalOutputStructData().J710_13));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_14.AC_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J710_14.Status));toSend.append(COMMA);
    toSend.append(QString::number(getACDigitalOutputStructData().J710_14));
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
