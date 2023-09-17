/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to send actuator calibration details to BL

***************************************************************************/
#include "Header/actuatorcalibration.h"
#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif
#include "Header/configuration.h"
#include "Header/recipe.h"

extern CommunicationInterface CIObj;
extern Recipe *recipe;
extern int dwCurrentUnit;


/*
Dummy change to include Sam QT changes
Add new alarm IDs and Descriptions for DB table limits
 */

/**
 * @brief ActuatorCalibration::ActuatorCalibration : Constructor
 * @param parent
 */
ActuatorCalibration::ActuatorCalibration(QObject *parent) : QObject(parent)
{
}

/**
 * @brief ActuatorCalibration::sendStart : Sends actuator calibration start request to BL
 * @return : int
 */
int ActuatorCalibration::sendStart()
{
    int nSent = 0;
    m_CalibrationReadings.CommandId = 1;
    m_CalibrationReadings.FirstReading = 0;
    m_CalibrationReadings.SecondReading = 0;
    char scStatus[4] = {0};
    char *m_data = reinterpret_cast<char *>(&m_CalibrationReadings);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_CALIBRATION_READ_REQ,UIC_CALIBRATION_READ_RES,m_data,sizeof(m_CalibrationReadings));
    CIObj.recivedMessage(UIC_CALIBRATION_READ_RES,scStatus,2);
    nSent = scStatus[0];
    QString status = scStatus;
    nSent = status.toInt();

#ifdef SIMULATESUCCESS
    nSent = SIMULATESUCCESS;
#endif
    return nSent;
}

/**
 * @brief ActuatorCalibration::sendFirstReading : Sends actuator calibration first reading to BL
 * @param : int nFirstReading
 * @return : int
 */
int ActuatorCalibration::sendFirstReading(int nFirstReading)
{
    int nSent = 0;
    m_CalibrationReadings.CommandId = 2;
    m_CalibrationReadings.FirstReading = nFirstReading;
    m_CalibrationReadings.SecondReading = 0;
    char scStatus[4] = {0};
    char *m_data = reinterpret_cast<char *>(&m_CalibrationReadings);

    /* Send message to BL*/
    CIObj.sendMessage(SCBL_CALIBRATION_READ_REQ,UIC_CALIBRATION_READ_RES,m_data,sizeof(m_CalibrationReadings));
    CIObj.recivedMessage(UIC_CALIBRATION_READ_RES,scStatus,2);

    nSent = scStatus[0];
    QString status = scStatus;
    nSent = status.toInt();

#ifdef SIMULATESUCCESS
    nSent = SIMULATESUCCESS;
#endif
    return nSent;
}

/**
 * @brief ActuatorCalibration::sendSecondReading : Sends actuator calibration second reading to BL
 * @param : int nFirstReading
 * @param : int nSecondReading
 * @return : int
 */
int ActuatorCalibration::sendSecondReading(int nFirstReading,int nSecondReading)
{
    int nSent = 0;
    m_CalibrationReadings.CommandId = 3;
    m_CalibrationReadings.FirstReading = nFirstReading;
    m_CalibrationReadings.SecondReading = nSecondReading;
    char scStatus[4] = {0};
    char *m_data = reinterpret_cast<char *>(&m_CalibrationReadings);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_CALIBRATION_READ_REQ,UIC_CALIBRATION_READ_RES,m_data,sizeof(m_CalibrationReadings));
    CIObj.recivedMessage(UIC_CALIBRATION_READ_RES,scStatus,2);

    nSent = scStatus[0];
    QString status = scStatus;
    nSent = status.toInt();

#ifdef SIMULATESUCCESS
    nSent = SIMULATESUCCESS;
#endif
    return nSent;
}

/**
 * @brief ActuatorCalibration::sendAbort : Sends actuator calibration abort request to BL
 * @return : int
 */
int ActuatorCalibration::sendAbort()
{
    int nSent = 0;
    m_CalibrationReadings.CommandId = 4;
    m_CalibrationReadings.FirstReading = 0;
    m_CalibrationReadings.SecondReading = 0;
    char scStatus[4] = {0};
    char *m_data = reinterpret_cast<char *>(&m_CalibrationReadings);

    /* Send message to BL */
    CIObj.sendMessage(SCBL_CALIBRATION_READ_REQ,UIC_CALIBRATION_READ_RES,m_data,sizeof(m_CalibrationReadings));
    CIObj.recivedMessage(UIC_CALIBRATION_READ_RES,scStatus,2);

    nSent = scStatus[0];
    QString status = scStatus;
    nSent = status.toInt();

#ifdef SIMULATESUCCESS
    nSent = SIMULATESUCCESS;
#endif
    return nSent;
}

/**
 * @brief ActuatorCalibration::getUnitsType : Returns unit type whether it is metrics or imperial
 * @return : int
 */
int ActuatorCalibration::getUnitsType()
{
    int dwUnit = dwCurrentUnit;
    return dwUnit;
}

/**
 * @brief ActuatorCalibration::onExit Sending the cancel request to BL
 */
void ActuatorCalibration::onExit()
{
	char *buff = NULL;
	CIObj.sendMessage(SCBL_SET_NEXT_OPERATION_REQ,SCBL_SET_NEXT_OPERATION_REQ,buff,0);
}
