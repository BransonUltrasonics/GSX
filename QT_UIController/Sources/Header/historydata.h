/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

History Data

***************************************************************************/
#ifndef HISTORYDATA_H
#define HISTORYDATA_H

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#ifndef HORNSCANDATA_H
#include "Header/hornscandata.h"
#endif

#ifndef HORNRESULTDATA_H
#include "Header/hornresultdata.h"
#endif

#include <QObject>
#include <QString>
#include <QFile>

//#define DEBUG

/* Macros */
#define PARAMETERS_PER_RECORD 3
#define SCANID_NOT_FOUND 0
#define SCANID_FOUND 1

#define SCANID_REPEAT -4

using namespace std ;

struct t_histRecord
{
    QString historyId ;
    QString dateTime ;
    QString recipeId ;
} ;

/*

Purpose: Holds the history data. Back End Interface between history List Control
UI window and DB for the Horn Scan History data

*/

//! A HistoryItem Class
/*!
  This class derived from QObject, holds the history ID, date time, recipe ID.
*/
class HistoryItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString scanName READ getScanName NOTIFY scanNameChanged)
    Q_PROPERTY(QString scanId READ getScanId NOTIFY scanIdChanged)
    Q_PROPERTY(QString dateTime READ getDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(quint16 recipeId READ getRecipeId NOTIFY recipeIdChanged)

public :
    QString m_scanName;
    QString m_scanId;
    quint16 m_recipeId;
    QString m_dateTime;

    HistoryItem(const QString &a_scanName,const QString &a_scanId , const QString &a_dateTime);
    ~HistoryItem();

    QString getScanName();
    QString getScanId();
    QString getDateTime();
    quint16 getRecipeId();
    QString formatDateTime(QString a_dateTime , bool a_formatDateTime);
    void setScanName(QString a_newScanName);

signals:
    void scanNameChanged();
    void scanIdChanged();
    void dateTimeChanged();
    void recipeIdChanged();
};

//!  A historyData class.
/*!
  This class derived from QObject, handles backend functionality for history data, and a
also requesting the horn signature and horn result data for a history from the database
*/
class HistoryData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hornOpStatus READ getHornOperationStatus NOTIFY hornOpStatusChanged)
    Q_PROPERTY(unsigned int hornIndSelected READ getHornIndSelected WRITE setHornIndSelected NOTIFY hornIndSelectedChanged)
public:
    // constructor.
    /*
      This Constructor initializes and creates HistoryData object.
       * */
    explicit HistoryData();

    // Destructor.
    /*
      This Destructor destroys HistoryData object.
       * */
    ~HistoryData();
    Q_INVOKABLE int updateHistoryDataId(unsigned int a_positionInd , QString updatedId);
    Q_INVOKABLE int refreshHistory();
    Q_INVOKABLE int retrieveHornSigPoints(QAbstractSeries *a_series , quint8 a_type);
    Q_INVOKABLE int getHornGraphDataFromDb(QString a_historyId);
    Q_INVOKABLE int getHistoryDataFromDb( QString a_dateTime , QString a_countReq);
    Q_INVOKABLE int abortHornScan();
    Q_INVOKABLE int performHornScan();
    Q_INVOKABLE bool getHornOperationStatus();
    Q_INVOKABLE unsigned int getHornIndSelected();

    Q_INVOKABLE void setHornIndSelected(unsigned int a_hornIndex);


    Q_INVOKABLE void receiveStartHornScanRes(int status);
    Q_INVOKABLE void receiveHornScanStatus(int samplesCounted);

    Q_INVOKABLE QList<qreal> hornAxisMaxValues();
    Q_INVOKABLE QList<qreal> hornAxisMinValues();

    int checkScanIdRepeat(QString a_scanIdVal);

signals:
    /* Signal to be defined */
    void changeProgress(qreal statusVal);
    void hornScanCompleted(int statusVal);
    void disableConnections();
    void hornOpStatusChanged();
    void hornIndSelectedChanged();

public slots:
    /* Clearing of history data when invoked */
    void clearHistoryData();
    void receiveScUiReqMsg(int reqId , QByteArray ScUiMsg);

private:
    HornScanData m_hornScanData ;
    HornResultData m_hornResultData;
    unsigned char m_opInProgress = false;
    QList<QObject*> m_historyItemList;
    QList<QObject*> m_dummyHornHistoryItemList;
    unsigned int m_currentHornInd = 0 ;
};

#endif // HORNRECIPEDATA_H
