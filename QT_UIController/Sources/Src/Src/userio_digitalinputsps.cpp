/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply Digital Inputs

***************************************************************************/
#include "Header/userio_digitalinputsps.h"

/**
 * @brief DigitalInputsPS::DigitalInputsPS : Initalizing pin name and number function
 */
DigitalInputsPS::DigitalInputsPS()
{
    initializeIOPinsFunctions();
    initializeIODefaultFunctions();
    initializeIOPinsInformation();
}

/**
 * @brief DigitalInputsPS::initializeIOPinsFunctions : Powersupply input model name has been used
 */
void DigitalInputsPS::initializeIOPinsFunctions()
{
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED,PSINPUT_NOTUSED));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_CYCLEABORT,CYCLEABORT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_US_DISABLE,US_DISABLE));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_RESET,RESET));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HOLD_DELAY,HOLD_DELAY));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_CONFIRM_REJECT,CONFIRM_REJECT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_HORN_SEEK_IN, HORN_SEEK_IN));
}

/**
 * @brief DigitalInputsPS::initializeIODefaultFunctions : Default values append to the List
 */
void DigitalInputsPS::initializeIODefaultFunctions()
{
    m_DefaultPinsFunctionsList.append(CYCLEABORT);
    m_DefaultPinsFunctionsList.append(US_DISABLE);
    m_DefaultPinsFunctionsList.append(RESET);
    m_DefaultPinsFunctionsList.append(PSINPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(HOLD_DELAY);
    m_DefaultPinsFunctionsList.append(PSINPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(PSINPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(PSINPUT_NOTUSED);
    m_DefaultPinsFunctionsList.append(PSINPUT_NOTUSED);
}

/**
 * @brief DigitalInputsPS::initializeIOPinsInformation : Pin number, pin name and status should be added to list
 */
void DigitalInputsPS::initializeIOPinsInformation()
{
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN1,   IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN2,   IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN3,   IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN4,  !IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN11,  IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN12, !IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN13, !IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN16, !IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIODigitalParamsModel(PS_PIN23, !IS_CHECKBOX_CHECKED,PSINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),DEFAULT_ACTIVE_STATUS, IS_PIN_ENABLED));
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
 * @brief DigitalInputsPS::getDataFromDBUsingIds : Get the values based of particular ids
 */
void DigitalInputsPS::getDataFromDBUsingIds(QStringList a_PsDi)
{
    getDataFromDB(a_PsDi);
}

/**
 * @brief DigitalInputsPS::refreshModel : Refresh the values based on updated changes
 */
void DigitalInputsPS::refreshModel()
{
    context->setContextProperty(PS_DIGITAL_INPUTS_PARAMS_CONTEXT_PROPERTY,QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief DigitalInputsPS::saveDataToStructs : Save the pin number and name to the structure
 * @param a_Index
 * @param a_Function
 * @param a_ActiveStatus
 */
void DigitalInputsPS::saveDataToStructs(int a_Index,int a_Function,int a_ActiveStatus,int a_CheckboxStatus)
{
    switch(a_Index)
    {
    case 0:
        m_stDigitalInputs.J116_01.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_01.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_01 = a_CheckboxStatus;
        break;
    case 1:
        m_stDigitalInputs.J116_02.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_02.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_02 = a_CheckboxStatus;
        break;
    case 2:
        m_stDigitalInputs.J116_03.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_03.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_03 = a_CheckboxStatus;
        break;
    case 3:
        m_stDigitalInputs.J116_04.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_04.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_04 = a_CheckboxStatus;
        break;
    case 4:
        m_stDigitalInputs.J116_11.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_11.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_11 = a_CheckboxStatus;
        break;
    case 5:
        m_stDigitalInputs.J116_12.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_12.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_12 = a_CheckboxStatus;
        break;
    case 6:
        m_stDigitalInputs.J116_13.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_13.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_13 = a_CheckboxStatus;
        break;
    case 7:
        m_stDigitalInputs.J116_16.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_16.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_16 = a_CheckboxStatus;
        break;
    case 8:
        m_stDigitalInputs.J116_23.PS_EVENT=(PowerSupplyInput)(a_Function);
        m_stDigitalInputs.J116_23.Status=a_ActiveStatus;
        m_stPSDigitalInputCheckStatus.J116_23 = a_CheckboxStatus;
        break;
    }
}

/**
 * @brief DigitalInputsPS::getStructData : Get the values from the structure
 * @return
 */
PS_DIGITALINPUT DigitalInputsPS::getStructData()
{
    return m_stDigitalInputs;
}

PS_DIGITALINPUT_CHECKSTATUS DigitalInputsPS::getPSDigitalInputStructData()
{
    return m_stPSDigitalInputCheckStatus;
}

/**
 * @brief DigitalInputsPS::setUIAfterSaving : Map the values based on structure
 */
void DigitalInputsPS::setUIAfterSaving()
{
    QString toSend=EMPTY_STRING;
    toSend.append(QString::number(getStructData().J116_01.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_01.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_01));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_02.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_02.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_02));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_03.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_03.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_03));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_04.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_04.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_04));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_11.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_11.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_11));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_12.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_12.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_12));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_13.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_13.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_13));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_16.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_16.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_16));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_23.PS_EVENT));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_23.Status));toSend.append(COMMA);
    toSend.append(QString::number(getPSDigitalInputStructData().J116_23));
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
