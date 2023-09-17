/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

System Info Parameters

***************************************************************************/
#ifndef SYSTEMINFOPARAMETERS_H
#define SYSTEMINFOPARAMETERS_H

#include <QObject>

class SystemInfoParameter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString ParameterContent READ getParameterContent WRITE setParameterContent NOTIFY parameterContentChanged)


public:
    SystemInfoParameter(QObject *parent=0);
    SystemInfoParameter(const QString name,const QString content,QObject *parent=0);
    QString m_parameterName;
    QString m_parameterContent;
    QString getParameterName() const;
    void setParameterName(const QString value);

    QString getParameterContent() const;
    void setParameterContent(const QString value);
signals:
    void parameterNameChanged();
    void parameterContentChanged();

};



#endif // SYSTEMINFOPARAMETERS_H



