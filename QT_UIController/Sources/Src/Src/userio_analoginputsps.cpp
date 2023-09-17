/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply Analog Inputs

***************************************************************************/
#include "Header/userio_analoginputsps.h"

/**
 * @brief AnalogInputsPS::AnalogInputsPS : Initialize the default values
 */
AnalogInputsPS::AnalogInputsPS()
{
    initializeIOPinsFunctions();
    initializeIODefaultFunctions();
    initializeIOPinsInformation();
}

/**
 * @brief AnalogInputsPS::initializeIOPinsFunctions : Initialize the default values to List
 */
void AnalogInputsPS::initializeIOPinsFunctions()
{
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED,ANALOGINPUT_NOTUSED));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_AMPLITUDE_SETTING,AMPLITUDE_SETTING));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_FREQUENCY_OFFSET,FREQUENCY_OFFSET));
}

/**
 * @brief AnalogInputsPS::initializeIODefaultFunctions : Initialize the default values to pin name
 */
void AnalogInputsPS::initializeIODefaultFunctions()
{
    m_DefaultPinsFunctionsList.append(AMPLITUDE_SETTING);
    m_DefaultPinsFunctionsList.append(FREQUENCY_OFFSET);
}

/**
 * @brief AnalogInputsPS::initializeIOPinsInformation : Initialize the default values to List
 */
void AnalogInputsPS::initializeIOPinsInformation()
{
    m_IOPinsInformationList.append(new UserIOAnalogParamsModel(PS_PIN17,!IS_CHECKBOX_CHECKED,ANALOGINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIOAnalogParamsModel(PS_PIN18,!IS_CHECKBOX_CHECKED,ANALOGINPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),IS_PIN_ENABLED));
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        saveDataToStructs(index,ANALOGINPUT_NOTUSED,IS_CHECKBOX_CHECKED);
    }
    //giving ownership of QObject* to c++
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        enginePointer->setObjectOwnership(m_IOPinsInformationList.at(index),QQmlEngine::CppOwnership);
    }
    refreshModel();
}

/**
 * @brief AnalogInputsPS::getDataFromDBUsingIds : Get the inputs value from database
 */
void AnalogInputsPS::getDataFromDBUsingIds(QStringList a_List)
{

    getDataFromDB(a_List);
}

/**
 * @brief AnalogInputsPS::refreshModel
 */
void AnalogInputsPS::refreshModel()
{
    context->setContextProperty(PS_ANALOG_INPUT_PARAMS_CONTEXT_PROPERTY,QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief AnalogInputsPS::saveDataToStructs : save the values to the structure
 * @param a_Index
 * @param a_Function
 */
void AnalogInputsPS::saveDataToStructs(int a_Index, int a_Function,int a_CheckboxStatus)
{
    switch(a_Index)
    {
    case 0:
        m_stAnalogInputs.J116_17=(AnalogInput)(a_Function);
        m_stPSAnalogInputCheckStatus.J116_17 = a_CheckboxStatus;
        break;
    case 1:
        m_stAnalogInputs.J116_18=(AnalogInput)(a_Function);
        m_stPSAnalogInputCheckStatus.J116_18 = a_CheckboxStatus;
        break;
    }
}

/**
 * @brief AnalogInputsPS::getStructData : Get the values from the structure
 * @return
 */
PS_ANALOGINPUT AnalogInputsPS::getStructData()
{
    return m_stAnalogInputs;
}

PS_ANALOGINPUT_CHECKSTATUS AnalogInputsPS::getPSAnalogInputStructData()
{
    return m_stPSAnalogInputCheckStatus;
}

/**
 * @brief AnalogInputsPS::setUIAfterSaving  : Maping the function to set the default values
 */
void AnalogInputsPS::setUIAfterSaving()
{
    QString toSend=EMPTY_STRING;
    toSend.append(QString::number(getStructData().J116_17));toSend.append(COMMA);
    toSend.append(QString::number(ACTIVE_STATUS_NOT_APPLICABLE));toSend.append(COMMA);
    toSend.append(QString::number(getPSAnalogInputStructData().J116_17));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_18));toSend.append(COMMA);
    toSend.append(QString::number(ACTIVE_STATUS_NOT_APPLICABLE));toSend.append(COMMA);
    toSend.append(QString::number(getPSAnalogInputStructData().J116_18));
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
