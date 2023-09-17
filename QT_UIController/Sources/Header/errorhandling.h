/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle errors

***************************************************************************/

#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <QObject>
#include <QQmlEngine>
#include "Header/errorcodes.h"

#define ERROR_CODE_URI_NAME "com.branson.errorcodes"
#define ERROR_CODE_QML_MAJOR_VERSION 1
#define ERROR_CODE_QML_MINIOR_VERSION 0
#define ERROR_CODE_QML_NAME "ErrorCode"

class ErrorHandling : public QObject
{
    Q_OBJECT

    Q_ENUMS(ErrorCodes)

public:
    ErrorHandling() : QObject() {}

    enum ErrorCodes
    {
        ERROR_UNKNOWN  = -1,
        SUCESS = 0,
        ERROR_LOGIN_FAILURE = 1,
        ERROR_USER_LOCKED = 2
    };

    static void registerQmlType()
    {
        qmlRegisterType<ErrorHandling>(ERROR_CODE_URI_NAME, ERROR_CODE_QML_MAJOR_VERSION, ERROR_CODE_QML_MINIOR_VERSION, ERROR_CODE_QML_NAME);
    }
};


#endif // ERRORHANDLING_H
