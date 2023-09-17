/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Read EEPROM

***************************************************************************/
#ifndef INIT_H
#define INIT_H
#include<QObject>

class Init: public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void readepromData();
};

#endif // INIT_H
