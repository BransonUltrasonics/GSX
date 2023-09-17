/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Actuator calibration

***************************************************************************/
#ifndef ACTUATORCALIBRATION_H
#define ACTUATORCALIBRATION_H

#include <QObject>
#include "header/welduitobl.h"

struct t_CalibrationReadings
{
       UINT8  CommandId;           /* START = 1, NEXT = 2 and FINSIH = 3 */
       UINT32  FirstReading;      /* First reading value of FORCE */
       UINT32  SecondReading;     /* Second reading value of FORCE */
};

class ActuatorCalibration : public QObject
{
    Q_OBJECT
public:
    explicit ActuatorCalibration(QObject *parent = nullptr);

    Q_INVOKABLE int sendStart();
    Q_INVOKABLE int sendFirstReading(int nFirstReading);
    Q_INVOKABLE int sendSecondReading(int nFirstReading,int nSecondReading);
    Q_INVOKABLE int sendAbort();
    Q_INVOKABLE int getUnitsType();
	Q_INVOKABLE void onExit();

private:
    struct t_CalibrationReadings m_CalibrationReadings;
};

#endif // ACTUATORCALIBRATION_H
