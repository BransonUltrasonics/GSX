/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Power supply Analog Outputs

***************************************************************************/
#include "Header/userio_analogoutputsps.h"

/**
 * @brief AnalogOutputsPS::AnalogOutputsPS : Initialize the default values
 */
AnalogOutputsPS::AnalogOutputsPS()
{
    initializeIOPinsFunctions();
    initializeIODefaultFunctions();
    initializeIOPinsInformation();
}

/**
 * @brief AnalogOutputsPS::initializeIOPinsFunctions : Default Pin Name to list
 */
void AnalogOutputsPS::initializeIOPinsFunctions()
{

    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_NOT_USED,ANALOGOUTPUT_NOTUSED));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_POWER_OUT,POWER_OUT));
    m_IOPinsFunctionsList.append(new UserIOComboboxModel(PIN_FUNCTION_NAME_FREQUENCY_OUT,FREQUENCY_OUT));
}

/**
 * @brief AnalogOutputsPS::initializeIODefaultFunctions : Default Pin Name to list
 */
void AnalogOutputsPS::initializeIODefaultFunctions()
{
    m_DefaultPinsFunctionsList.append(POWER_OUT);
    m_DefaultPinsFunctionsList.append(FREQUENCY_OUT);
}

/**
 * @brief AnalogOutputsPS::initializeIOPinsInformation : Default Pin number to list
 */
void AnalogOutputsPS::initializeIOPinsInformation()
{
    m_IOPinsInformationList.append(new UserIOAnalogParamsModel(PS_PIN24,!IS_CHECKBOX_CHECKED,ANALOGOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),IS_PIN_ENABLED));
    m_IOPinsInformationList.append(new UserIOAnalogParamsModel(PS_PIN25,!IS_CHECKBOX_CHECKED,ANALOGOUTPUT_NOTUSED,QVariant::fromValue(copyIOPinsFunctionsList(m_IOPinsFunctionsList)),IS_PIN_ENABLED));
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        saveDataToStructs(index,ANALOGOUTPUT_NOTUSED,!IS_CHECKBOX_CHECKED);
    }
    //giving ownership of QObject* to c++
    for(int index=0;index<m_IOPinsInformationList.size();index++)
    {
        enginePointer->setObjectOwnership(m_IOPinsInformationList.at(index),QQmlEngine::CppOwnership);
    }
    refreshModel();
}

/**
 * @brief AnalogOutputsPS::getDataFromDBUsingIds : Get the ids from database
 */
void AnalogOutputsPS::getDataFromDBUsingIds(QStringList a_List)
{
    getDataFromDB(a_List);
}

/**
 * @brief AnalogOutputsPS::refreshModel : Refresh the updated values
 */
void AnalogOutputsPS::refreshModel()
{
    context->setContextProperty(PS_ANALOG_OUTPUTS_PARAMS_CONTEXT_PROPERTY,QVariant::fromValue(m_IOPinsInformationList));
}

/**
 * @brief AnalogOutputsPS::saveDataToStructs : Set the values to the structure
 * @param a_Index
 * @param a_Function
 */
void AnalogOutputsPS::saveDataToStructs(int a_Index, int a_Function,int a_CheckboxStatus)
{
    switch(a_Index)
    {
    case 0:
        m_stAnalogOutputs.J116_24=(AnalogOutput)(a_Function);
        m_stPSAnalogOutputCheckStatus.J116_24 = a_CheckboxStatus;
        break;
    case 1:
        m_stAnalogOutputs.J116_25=(AnalogOutput)(a_Function);
        m_stPSAnalogOutputCheckStatus.J116_25 = a_CheckboxStatus;
        break;
    }
}

/**
 * @brief AnalogOutputsPS::getStructData : Get the values from the structure
 * @return
 */
PS_ANALOGOUTPUT AnalogOutputsPS::getStructData()
{
    return m_stAnalogOutputs;
}

PS_ANALOGOUTPUT_CHECKSTATUS AnalogOutputsPS::getPSAnalogOutputStructData()
{
    return m_stPSAnalogOutputCheckStatus;
}

/**
 * @brief AnalogOutputsPS::setUIAfterSaving : Map the values based on structure
 */
void AnalogOutputsPS::setUIAfterSaving()
{
    QString toSend=EMPTY_STRING;
    toSend.append(QString::number(getStructData().J116_24));toSend.append(COMMA);
    toSend.append(QString::number(ACTIVE_STATUS_NOT_APPLICABLE));toSend.append(COMMA);
    toSend.append(QString::number(getPSAnalogOutputStructData().J116_24));toSend.append(COMMA);
    toSend.append(QString::number(getStructData().J116_25));toSend.append(COMMA);
    toSend.append(QString::number(ACTIVE_STATUS_NOT_APPLICABLE));toSend.append(COMMA);
    toSend.append(QString::number(getPSAnalogOutputStructData().J116_25));
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
