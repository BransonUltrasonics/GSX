/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek Result Data

***************************************************************************/
#ifndef SEEKRESULTDATA_H
#define SEEKRESULTDATA_H

#ifndef COMMUNICATIONINTERFACE_H
    #include "Header/communicationinterface.h"
#endif

#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#include <QObject>
#include <QString>
#include <fstream>
#include<QFile>

using namespace std ;

#define COUNT_RESONANT_POINTS 6

//! A hornResonantPoint Class
/*!
  This class derived from QObject, holds the weld points to be plotted on the weld Graph UI.
*/
class SeekResonantPoints : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString labelName READ getLabelName NOTIFY labelNameChanged)
    Q_PROPERTY(int xAxisPos READ getXAxisPos NOTIFY xAxisPosChanged)
    Q_PROPERTY(QString custColor READ getCustColor NOTIFY custColorChanged)

public :
    SeekResonantPoints(const QString &a_labelName , const int &a_xAxisVal, const QString &a_custColor);
    ~SeekResonantPoints();

    QString m_labelName;
    int m_timePos;
    QString m_custColor;
    QString getLabelName();
    int getXAxisPos();
    QString getCustColor();

    void setXAxisPos(int val);

signals:
    void labelNameChanged();
    void xAxisPosChanged();
    void custColorChanged();    
};

//!  A seekResultData class.
/*!
  This class derived from QObject, handles backend functionality to hold the horn result data
*/
class SeekResultData : public QObject
{
    Q_OBJECT
public:
    // constructor.
    /*
      This Constructor initializes and creates HornResultData object.
       * */
    explicit SeekResultData();
    // Destructor.
    /*
      This Destructor destroys HornResultData object.
       * */
    ~SeekResultData();

    Q_INVOKABLE int getSeekResultDbRes(QString a_historyId);

    void resetSeekResonantPoints();
public slots:

signals:


private:
    QList<QObject*> m_SeekresonantPoints;

    QList<QObject*> m_dummyresonantPoints;
};


#endif // SEEKRESULTDATA_H
