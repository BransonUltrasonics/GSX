/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Information Panel

***************************************************************************/
#ifndef INFORMATIONPANEL_H
#define INFORMATIONPANEL_H

#include <QObject>

class InformationPanel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString m_userName READ getUsername WRITE setUsername NOTIFY Usernamechanged)
    Q_PROPERTY(QString m_machinenumber READ  getInformationpanelData WRITE setInformationpanelData NOTIFY InformationpanelDataChanged)

public:
    QString m_userName ;
    QString  m_machinenumber;
    int Machinenumber;

    explicit InformationPanel(QObject *parent = nullptr);

public slots:
    void setUsername(QString Username);
    void setInformationpanelData(QString );
    QString getUsername();
    QString getInformationpanelData();

signals:
    void Usernamechanged();
    void InformationpanelDataChanged();
};

#endif // INFORMATIONPANEL_H
