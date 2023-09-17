/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek History Data

***************************************************************************/
#ifndef SEEKHISTORYDATA_H
#define SEEKHISTORYDATA_H

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#ifndef HORNSCANDATA_H
#include "Header/seekscandata.h"
#endif

#ifndef HORNRESULTDATA_H
#include "Header/seekresultdata.h"
#endif

#include <QObject>
#include <QString>
#include <QFile>

//#define DEBUG

/* Macros */
#define SEEK_PARAMETERS_PER_RECORD 2
#define SCANID_NOT_FOUND 0
#define SCANID_FOUND 1

#define SCANID_REPEAT -4

using namespace std ;

struct t_seekhistRecord
{
    QString historyId ;
    QString dateTime ;
    QString recipeId ;
};

/*

Purpose: Holds the history data. Back End Interface between history List Control
UI window and DB for the Horn Scan History data

*/

//! A SeekHistoryItem Class
/*!
  This class derived from QObject, holds the history ID, date time, recipe ID.
*/
class SeekHistoryItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString scanId READ getSeekScanId NOTIFY seekscanIdChanged)
    Q_PROPERTY(QString dateTime READ getSeekDateTime NOTIFY seekdateTimeChanged)

public :
    SeekHistoryItem(const QString &a_scanId , const QString &a_dateTime);
    ~SeekHistoryItem();

    QString m_SeekscanId;
    QString m_SeekdateTime;

    QString getSeekScanId();
    QString getSeekDateTime();

    QString formatDateTime(QString a_dateTime , bool a_formatDateTime);

    void setScanId(QString a_newScanId);

signals:
    void seekscanIdChanged();
    void seekdateTimeChanged();

};



//!  A SeekHistoryData class.
/*!
  This class derived from QObject, handles backend functionality for history data, and a
also requesting the horn signature and horn result data for a history from the database
*/
class SeekHistoryData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool seekOpStatus READ getSeekOperationStatus NOTIFY seekOpStatusChanged)
    Q_PROPERTY(unsigned int seekIndSelected READ getSeekIndSelected WRITE setSeekIndSelected NOTIFY seekIndSelectedChanged)
public:
    // constructor.
    /*
      This Constructor initializes and creates SeekHistoryData object.
       * */
    explicit SeekHistoryData();

    // Destructor.
    /*
      This Destructor destroys SeekHistoryData object.
       * */
    ~SeekHistoryData();
    Q_INVOKABLE int updateSeekHistoryDataId(unsigned int a_positionInd , QString updatedId);
    Q_INVOKABLE int refreshSeekHistory();
    Q_INVOKABLE int retrieveSeekSigPoints(QAbstractSeries *a_series , quint8 a_type);
    Q_INVOKABLE int getSeekGraphDataFromDb(QString a_historyId);
    Q_INVOKABLE int getSeekHistoryDataFromDb( QString a_dateTime , QString a_countReq);
    //Q_INVOKABLE int abortHornScan();
    Q_INVOKABLE int performSeekScan();
    Q_INVOKABLE bool getSeekOperationStatus();
    Q_INVOKABLE unsigned int getSeekIndSelected();

    Q_INVOKABLE void setSeekIndSelected(unsigned int a_hornIndex);


    Q_INVOKABLE void receiveStartSeekScanRes(int status);
    Q_INVOKABLE void receiveSeekScanStatus(int samplesCounted);

    Q_INVOKABLE QList<qreal> seekAxisMaxValues();
    Q_INVOKABLE QList<qreal> seekAxisMinValues();

    int checkSeekScanIdRepeat(QString a_scanIdVal);

signals:
    /* Signal to be defined */
    void changeSeekProgress(qreal statusVal);
    void seekScanCompleted(int statusVal);
    void disableSeekConnections();
     void seekOpStatusChanged();
     void seekIndSelectedChanged();

public slots:
    /* Clearing of history data when invoked */
    void clearSeekHistoryData();
    void receiveScUiseekReqMsg(int reqId , QByteArray ScUiMsg);

private:
    SeekScanData m_seekScanData ;
    SeekResultData m_seekResultData;

    unsigned char m_opInProgress = false;
    QList<QObject*> m_seekhistoryItemList;

    QList<QObject*> m_dummyHistoryItemList ;

    unsigned int m_currentSeekInd = 0 ;

};


#endif // SEEKHISTORYDATA_H
