/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO handles UI

***************************************************************************/
#include "Header/userio.h"
extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief UserIO::UserIO constructor : Initializes UI data
 * @param ptr
 */
UserIO::UserIO(QObject *ptr):QObject(ptr),m_UserIOPS(),m_UserIOAC()
{
    context->setContextProperty(PS_CONTEXT_PROPERTY,&m_UserIOPS);
    context->setContextProperty(AC_CONTEXT_PROPERTY,&m_UserIOAC);
    getPSDataFromUI();
    getACDataFromUI();
    m_InitOnce=false;
    //    getUserIODataFromDB();
}

/**
 * @brief UserIO::userIOInit
 */
void UserIO::userIOInit()
{
    if(m_InitOnce==false)
    {
        m_InitOnce=true;
        getUserIODataFromDB();
    }
}
/**
 * @brief UserIO::getUserIODataFromDB :gets data from DB
 */
void UserIO::getUserIODataFromDB()
{
    QString  tempUserIO;
    CIObj.sendMessage(REQ_GET_USERIO_DATA, RES_GET_USERIO_DATA ,"");
    CIObj.recivedMessage(RES_GET_USERIO_DATA , tempUserIO);
    if(tempUserIO != "")
    {
        QStringList m_PSList, m_ACList, m_IOList;
        m_IOList = tempUserIO.split(",");
        int itr;
        for(itr = 0; itr < 63 ; itr++)
        {
            m_PSList.append(m_IOList[itr]);
        }
        for(itr = 63; itr < 93;itr++)
        {
           m_ACList.append(m_IOList[itr]);
        }
        m_UserIOPS.getDataFromDB(m_PSList);
        getPSDataFromUI();
        m_UserIOAC.getDataFromDB(m_ACList);
        getACDataFromUI();
    }
}

/**
 * @brief UserIO::getPSDataFromUI : Get PowerSupply IO data from UI
 */
void UserIO::getPSDataFromUI()
{   m_UserIO.PS_USERIO.PsDigitalInput=(const_cast<DigitalInputsPS*>(&m_UserIOPS.getDigitalInputs()))->getStructData();
    m_UserIO.PS_USERIO.PsDigitalOutput=(const_cast<DigitalOutputsPS*>(&m_UserIOPS.getDigitalOutputs()))->getStructData();
    m_UserIO.PS_USERIO.PsAnalogInput=(const_cast<AnalogInputsPS*>(&m_UserIOPS.getAnalogInputs()))->getStructData();
    m_UserIO.PS_USERIO.PsAnalogOutput=(const_cast<AnalogOutputsPS*>(&m_UserIOPS.getAnalogOutputs()))->getStructData();
    m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus = (const_cast<DigitalInputsPS*>(&m_UserIOPS.getDigitalInputs()))->getPSDigitalInputStructData();
    m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus = (const_cast<DigitalOutputsPS*>(&m_UserIOPS.getDigitalOutputs()))->getPSDigitalOutputStructData();
    m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogInputCheckStatus = (const_cast<AnalogInputsPS*>(&m_UserIOPS.getAnalogInputs()))->getPSAnalogInputStructData();
    m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogOutputCheckStatus = (const_cast<AnalogOutputsPS*>(&m_UserIOPS.getAnalogOutputs()))->getPSAnalogOutputStructData();
}

/**
 * @brief UserIO::getACDataFromUI : Gets Actuator Controller IO data from UI
 */
void UserIO::getACDataFromUI()
{
    m_UserIO.AC_USERIO.ACDigitalInput=(const_cast<DigitalInputsAC*>(&m_UserIOAC.getDigitalInputs()))->getStructData();
    m_UserIO.AC_USERIO.ACDigitalOutput=(const_cast<DigitalOutputsAC*>(&m_UserIOAC.getDigitalOutputs()))->getStructData();
    m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus = (const_cast<DigitalInputsAC*>(&m_UserIOAC.getDigitalInputs()))->getACDigitalInputStructData();
    m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus = (const_cast<DigitalOutputsAC*>(&m_UserIOAC.getDigitalOutputs()))->getACDigitalOutputStructData();
}

/**
 * @brief UserIO::saveDataToDB : Saves UI data to database
 */
bool UserIO::saveDataToDB()
{
    //UserIO PS;
    //format is (  id , pinname , pinfunction , activestatus )
    //database query needs string to be enclosed within ' '(single inverted commas)
    int rowID=1;
    
    //TODO: Remove the pin numbers being sent from the UI as they should never change on the DB.
    std::vector< std::vector<QString> > temp_PinInfoRows=
    {
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN1, INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_01.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_01.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_01), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN2, INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_02.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_02.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_02), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN3, INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_03.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_03.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_03), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN4, INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_04.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_04.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_04), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN11,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_11.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_11.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_11), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN12,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_12.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_12.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_12), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN13,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_13.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_13.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_13), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN16,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_16.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_16.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_16), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN23,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_23.PS_EVENT),  COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalInput.J116_23.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_23),  CLOSING_PARENTHESES, COMMA },

        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN7, INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_07.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_07.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_07), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN8, INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_08.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_08.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_08), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN9, INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_09.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_09.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_09), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN10,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_10.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_10.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_10), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN19,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_19.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_19.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_19), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN20,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_20.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_20.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_20), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN21,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_21.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_21.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_21), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN22,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_22.PS_EVENT), COMMA, QString::number(m_UserIO.PS_USERIO.PsDigitalOutput.J116_22.Status), COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_22), CLOSING_PARENTHESES, COMMA },

        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN17,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsAnalogInput.J116_17),            COMMA, QString::number(ACTIVE_STATUS_NOT_APPLICABLE),                      COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogInputCheckStatus.J116_17), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN18,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsAnalogInput.J116_18),            COMMA, QString::number(ACTIVE_STATUS_NOT_APPLICABLE),                      COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogInputCheckStatus.J116_18), CLOSING_PARENTHESES, COMMA },

        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN24,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsAnalogOutput.J116_24),           COMMA, QString::number(ACTIVE_STATUS_NOT_APPLICABLE),                      COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogOutputCheckStatus.J116_24), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, PS_PIN25,INVERTED_COMMA, COMMA, QString::number(m_UserIO.PS_USERIO.PsAnalogOutput.J116_25),           COMMA, QString::number(ACTIVE_STATUS_NOT_APPLICABLE),                      COMMA, QString::number(m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogOutputCheckStatus.J116_25), CLOSING_PARENTHESES, COMMA },

        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN1,INVERTED_COMMA,  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_01.AC_EVENT),  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_01.Status),  COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_01), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN2,INVERTED_COMMA,  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_02.AC_EVENT),  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_02.Status),  COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_02), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN3,INVERTED_COMMA,  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_03.AC_EVENT),  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_03.Status),  COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_03), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN9,INVERTED_COMMA,  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_09.AC_EVENT),  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_09.Status),  COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_09), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN10,INVERTED_COMMA,  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_10.AC_EVENT),  COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalInput.J710_10.Status),  COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_10), CLOSING_PARENTHESES, COMMA },

        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN5, INVERTED_COMMA, COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_05.AC_EVENT), COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_05.Status), COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_05), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN6,INVERTED_COMMA, COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_06.AC_EVENT), COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_06.Status), COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_06), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN7,INVERTED_COMMA, COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_07.AC_EVENT), COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_07.Status), COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_07), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN13,INVERTED_COMMA, COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_13.AC_EVENT), COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_13.Status), COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_13), CLOSING_PARENTHESES, COMMA },
        { OPENING_PARENTHESES, QString::number(rowID++), COMMA, INVERTED_COMMA, AC_PIN14,INVERTED_COMMA, COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_14.AC_EVENT), COMMA, QString::number(m_UserIO.AC_USERIO.ACDigitalOutput.J710_14.Status), COMMA, QString::number(m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_14), CLOSING_PARENTHESES, EMPTY_STRING }

    };

    //Preparing string to be sent to dbmanager
    QString toSend=EMPTY_STRING;

    for(uint row=0;row<temp_PinInfoRows.size();row++)
    {
        for(uint column=0;column<temp_PinInfoRows[0].size();column++)
        {
            toSend+=temp_PinInfoRows[row][column];
        }
    }

    QString response=EMPTY_STRING;
    
    CIObj.sendMessage(REQ_SET_USERIO_DATA, RES_SET_USERIO_DATA ,toSend);
    CIObj.recivedMessage(RES_SET_USERIO_DATA , response);
    if(response=="1")
    {
        sendIndicationToSCToReadDBData();
        return true;
    }
    return false;
}

/**
 * @brief UserIO::sendIndicationToSCToReadDBData
 */
void UserIO::sendIndicationToSCToReadDBData()
{
    USERIO userIO;
    char *data1 = reinterpret_cast<char*>(&m_UserIO);
    char *data2 = reinterpret_cast<char*>(&userIO);
    memcpy(data2,data1,sizeof(m_UserIO));

    userIO.AC_USERIO.ACDigitalInput.J710_01.AC_EVENT = (ActuatorInput)(userIO.AC_USERIO.ACDigitalInput.J710_01.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_01);
    userIO.AC_USERIO.ACDigitalInput.J710_02.AC_EVENT = (ActuatorInput)(userIO.AC_USERIO.ACDigitalInput.J710_02.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_02);
    userIO.AC_USERIO.ACDigitalInput.J710_03.AC_EVENT = (ActuatorInput)(userIO.AC_USERIO.ACDigitalInput.J710_03.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_03);
    userIO.AC_USERIO.ACDigitalInput.J710_09.AC_EVENT = (ActuatorInput)(userIO.AC_USERIO.ACDigitalInput.J710_09.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_09);
    userIO.AC_USERIO.ACDigitalInput.J710_10.AC_EVENT = (ActuatorInput)(userIO.AC_USERIO.ACDigitalInput.J710_10.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalInputCheckStatus.J710_10);

    userIO.AC_USERIO.ACDigitalOutput.J710_05.AC_EVENT = (ActuatorOutput)(userIO.AC_USERIO.ACDigitalOutput.J710_05.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_05);
    userIO.AC_USERIO.ACDigitalOutput.J710_06.AC_EVENT = (ActuatorOutput)(userIO.AC_USERIO.ACDigitalOutput.J710_06.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_06);
    userIO.AC_USERIO.ACDigitalOutput.J710_07.AC_EVENT = (ActuatorOutput)(userIO.AC_USERIO.ACDigitalOutput.J710_07.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_07);
    userIO.AC_USERIO.ACDigitalOutput.J710_13.AC_EVENT = (ActuatorOutput)(userIO.AC_USERIO.ACDigitalOutput.J710_13.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_13);
    userIO.AC_USERIO.ACDigitalOutput.J710_14.AC_EVENT = (ActuatorOutput)(userIO.AC_USERIO.ACDigitalOutput.J710_14.AC_EVENT * m_UserIOCheckStatus.AC_USERIO_CHECKSTATUS.ACDigitalOutputCheckStatus.J710_14);

    userIO.PS_USERIO.PsAnalogInput.J116_17 = (AnalogInput)(userIO.PS_USERIO.PsAnalogInput.J116_17 * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogInputCheckStatus.J116_17);
    userIO.PS_USERIO.PsAnalogInput.J116_18 = (AnalogInput)(userIO.PS_USERIO.PsAnalogInput.J116_18 * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogInputCheckStatus.J116_18);

    userIO.PS_USERIO.PsAnalogOutput.J116_24 = (AnalogOutput)(userIO.PS_USERIO.PsAnalogOutput.J116_24 * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogOutputCheckStatus.J116_24);
    userIO.PS_USERIO.PsAnalogOutput.J116_25 = (AnalogOutput)(userIO.PS_USERIO.PsAnalogOutput.J116_25 * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsAnalogOutputCheckStatus.J116_25);

    userIO.PS_USERIO.PsDigitalInput.J116_01.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_01.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_01);
    userIO.PS_USERIO.PsDigitalInput.J116_02.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_02.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_02);
    userIO.PS_USERIO.PsDigitalInput.J116_03.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_03.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_03);
    userIO.PS_USERIO.PsDigitalInput.J116_04.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_04.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_04);
    userIO.PS_USERIO.PsDigitalInput.J116_11.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_11.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_11);
    userIO.PS_USERIO.PsDigitalInput.J116_12.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_12.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_12);
    userIO.PS_USERIO.PsDigitalInput.J116_13.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_13.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_13);
    userIO.PS_USERIO.PsDigitalInput.J116_16.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_16.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_16);
    userIO.PS_USERIO.PsDigitalInput.J116_23.PS_EVENT = (PowerSupplyInput)(userIO.PS_USERIO.PsDigitalInput.J116_23.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalInputCheckStatus.J116_23);

    userIO.PS_USERIO.PsDigitalOutput.J116_07.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_07.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_07);
    userIO.PS_USERIO.PsDigitalOutput.J116_08.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_08.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_08);
    userIO.PS_USERIO.PsDigitalOutput.J116_09.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_09.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_09);
    userIO.PS_USERIO.PsDigitalOutput.J116_10.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_10.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_10);
    userIO.PS_USERIO.PsDigitalOutput.J116_19.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_19.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_19);
    userIO.PS_USERIO.PsDigitalOutput.J116_20.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_20.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_20);
    userIO.PS_USERIO.PsDigitalOutput.J116_21.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_21.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_21);
    userIO.PS_USERIO.PsDigitalOutput.J116_22.PS_EVENT = (PowerSupplyOutput)(userIO.PS_USERIO.PsDigitalOutput.J116_22.PS_EVENT * m_UserIOCheckStatus.PS_USERIO_CHECKSTATUS.PsDigitalOutputCheckStatus.J116_22);

    CIObj.sendMessage(SCBL_USERIO_READ_REQ ,SCBL_USERIO_READ_RES ,data2,sizeof(userIO));
    //CIObj.recivedMessage(SCBL_USERIO_READ_RES,ch,1);
}

