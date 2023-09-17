/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply IO Digital Outputs

***************************************************************************/
#include "Header/userio_digitaloutputsps.h"
/**

 * @brief DigitalOutputsPS::DigitalOutputsPS : Initialize the default values
 */
DigitalOutputsPS::DigitalOutputsPS()
{
    initializeIOPinsFunctions();
    initializeActualIOPinsFunctions();
    initializeIODefaultFunctions();
    initializeIOPinsInformation();
}

/**
 * @brief DigitalOutputsPS::initializeIOPinsFunctions : Setting the Pin Names
 */
void DigitalOutputsPS::initializeIOPinsFunctions()
{
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED,PSOUTPUT_NOTUSED));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_READY,READY));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_SONICS_ACTIVE,SONICS_ACTIVE));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_GENERAL_ALARM,GENERAL_ALARM));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_CYCLE_RUNNING,CYCLE_RUNNING));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_CYCLE_OK,CYCLE_OK));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_EXTERNAL_BEEPER,EXTERNAL_BEEPER));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_CUSTOM_ALARM,CUSTOM_ALARM));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_REJECTPART_ALARM,REJECTPART_ALARM));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_SUSPECTPART_ALARM, SUSPECTPART_ALARM));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HORN_SEEK_OUT, HORN_SEEK_OUT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_WELD_ACTIVE, WELD_ACTIVE));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOLD_ACTIVE, PSHOLD_ACTIVE));
}

void DigitalOutputsPS::initializeActualIOPinsFunctions()
{
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED,PSOUTPUT_NOTUSED));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_READY,READY));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_SONICS_ACTIVE,SONICS_ACTIVE));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_GENERAL_ALARM,GENERAL_ALARM));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_CYCLE_RUNNING,CYCLE_RUNNING));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_CYCLE_OK,CYCLE_OK));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_REJECTPART_ALARM,REJECTPART_ALARM));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_SUSPECTPART_ALARM, SUSPECTPART_ALARM));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HORN_SEEK_OUT, HORN_SEEK_OUT));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_WELD_ACTIVE, WELD_ACTIVE));
    m_ActualIOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOLD_ACTIVE, PSHOLD_ACTIVE));
}

/**
 * @brief DigitalOutputsPS::initializeIODefaultFunctions : Assign the default values to the list
 */
void DigitalOutputsPS::initializeIODefaultFunctions()
{
    m_DefaultPinsFunctionsList.append(READY);
    m_DefaultPinsFunctionsList.append(SONICS_ACTIVE);
    m_DefaultPinsFunctionsList.append(GENERAL_ALARM);
    m_DefaultPinsFunctionsList.append(CYCLE_RUNNING);
    m_DefaultPinsFunctionsList.append(PSOUTPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(CYCLE_OK);
    m_DefaultPinsFunctionsList.append(PSOUTPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(PSOUTPUT_NOTUSED);
}

/**
 * @brief DigitalOutputsPS::initializeIOPinsInformation : Append the parameter name and status to the List
 */
void DigitalOutputsPS::initializeIOPinsInformation()
{
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN7,   IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN8,   IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN9,   IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN10,  IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN19, !IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN20,  IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN21, !IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
//    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN22, !IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));

    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN7,   IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN8,   IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN9,   IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN10,  IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN19, !IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN20,  IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN21, !IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN22, !IS_CHECKBOX_CHECKED,PSOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_ActualIOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));

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
 * @brief DigitalOutputsPS::getDataFromDBUsingIds
 */
void DigitalOutputsPS::getDataFromDBUsingIds(QStringList a_List)
{
    getDataFromDB(a_List);
}

/**
 * @brief DigitalOutputsPS::refreshModel : Refresh the model to get the updated values
 */
void DigitalOutputsPS::refreshModel()
{
    context->setContextProperty(PS_DIGITAL_OUTPUTS_PARAMS_CONTEXT_PROPERTY,QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief DigitalOutputsPS::saveDataToStructs : Saving the name, pin number and status into structure
 * @param a_Index
 * @param a_Function
 * @param a_ActiveStatus
 */
void DigitalOutputsPS::saveDataToStructs(int a_Index, int a_Function, int a_ActiveStatus,int a_CheckboxStatus)
{
    switch(a_Index)
    {
    case 0:
        m_stDigitalOutputs.J116_07.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_07.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_07 = a_CheckboxStatus;
        break;
    case 1:
        m_stDigitalOutputs.J116_08.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_08.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_08 = a_CheckboxStatus;
        break;
    case 2:
        m_stDigitalOutputs.J116_09.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_09.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_09 = a_CheckboxStatus;
        break;
    case 3:
        m_stDigitalOutputs.J116_10.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_10.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_10 = a_CheckboxStatus;
        break;
    case 4:
        m_stDigitalOutputs.J116_19.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_19.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_19 = a_CheckboxStatus;
        break;
    case 5:
        m_stDigitalOutputs.J116_20.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_20.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_20 = a_CheckboxStatus;
        break;
    case 6:
        m_stDigitalOutputs.J116_21.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_21.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_21 = a_CheckboxStatus;
        break;
    case 7:
        m_stDigitalOutputs.J116_22.PS_EVENT=(PowerSupplyOutput)(a_Function);
        m_stDigitalOutputs.J116_22.Status=a_ActiveStatus;
        m_stPSDigitalOutputsCheckStatus.J116_22 = a_CheckboxStatus;
        break;
    }
}

/**
 * @brief DigitalOutputsPS::getStructData : Getting the name, pin number and status from Structure
 * @return
 */
PS_DIGITALOUTPUT DigitalOutputsPS::getStructData()
{
    return m_stDigitalOutputs;
}

PS_DIGITALOUTPUT_CHECKSTATUS DigitalOutputsPS::getPSDigitalOutputStructData()
{
    return m_stPSDigitalOutputsCheckStatus;
}

/**
 * @brief DigitalOutputsPS::setUIAfterSaving : Maping the values
 */
void DigitalOutputsPS::setUIAfterSaving()
{
    QString toSend=EMPTY_STRING;
    toSend.append(QString::number(getStructData().J116_07.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_07.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_07));toSend.append(COMMA);

    toSend.append(QString::number(getStructData().J116_08.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_08.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_08));toSend.append(COMMA);

    toSend.append(QString::number(getStructData().J116_09.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_09.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_09));toSend.append(COMMA);

    toSend.append(QString::number(getStructData().J116_10.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_10.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_10));toSend.append(COMMA);

    toSend.append(QString::number(getStructData().J116_19.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_19.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_19));toSend.append(COMMA);

    toSend.append(QString::number(getStructData().J116_20.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_20.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_20));toSend.append(COMMA);

    toSend.append(QString::number(getStructData().J116_21.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_21.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_21));toSend.append(COMMA);

    toSend.append(QString::number(getStructData().J116_22.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_22.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalOutputStructData().J116_22));
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
