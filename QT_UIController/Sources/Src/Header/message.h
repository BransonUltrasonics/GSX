/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Message

***************************************************************************/
#ifndef MESSAGE_H
#define MESSAGE_H
#include <QTcpSocket>
#include <QDataStream>

struct Message
{
       qint32 msgID;
       int length;
       char Buffer[1500];
};

#endif // MESSAGE_H
