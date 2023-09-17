/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SQLITEDB_H_
#define SQLITEDB_H_

#include  <iostream>
#include  <vector>
#include "sqlite3.h"

enum DB_COPY_DIRECTION
{
	EMMC_TO_SD_CARD	= 1,
	EMMC_TO_USB,
	SD_CARD_TO_EMMC,
	USB_TO_EMMC
};

//Values read from database are separated by comma
#define DB_VALUE_SEPARATOR	","

//Retry interval in case SQLITE_BUSY/SQLITE_LOCKED error returned.
#define SQLITE_BUSY_RETRY_INTERVAL	SEVENTEEN_MS_DELAY
					
#define RETRY_COUNT_FOR_ONE_FIFTY_MILLISEC_DELAY 	9
#define RETRY_COUNT_FOR_TWO_HUNDRED_MILLISEC_DELAY 	12
#define RETRY_COUNT_FOR_FIVE_SEC_DELAY 				300

//Default SQLITE_BUSY/SQLITE_LOCKED retrie. So default timeout becomes 12 retries of ~17 milliseconds (16.6666667 milliseconds) gap = ~200 milliseconds. 
#define SQLITE_BUSY_DEFAULT_RETRY_COUNT 	RETRY_COUNT_FOR_TWO_HUNDRED_MILLISEC_DELAY

//SQLite Database access wrapper class.
class SQLiteDB
{
public:
	
	SQLiteDB();
	~SQLiteDB();
	
	static SEM_ID& getSCDBMutex(){return SCDBMutex;};
	
	int EstablishDataBaseConnection();
	int Close();
	int SetDBParameters();
	
	std::string Exec(std::string strSqlStatement, int *pnStatus = NULL, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int SingleTransaction(std::string strSqlStatement, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int BulkTransaction(std::string strSqlStatement, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	
	int Commit(int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int DBVacuum(int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	
	std::string GetDBColumnName(const char *SqlStatement, int *pnStatus = NULL);
	INT32 GetDBColumnCount(const char *SqlStatement, int *pnStatus = NULL);
	UINT8 ReadRecipe(std::string SqlStatement, UINT8 columnCount, std::vector<INT32> &recipe);
	
private:
	int executeDBQuery(std::string strSqlStatement, int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int setAutoVacuum(int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	int setJournalMode(int nRetryCounter = SQLITE_BUSY_DEFAULT_RETRY_COUNT);
	
	static SEM_ID SCDBMutex;
	bool m_bIsDBOpen;
	sqlite3 *m_ptrDB;
};

//Class to handle database file copy/delete operations.
class DBFileHandler
{
public:
	DBFileHandler();
	~DBFileHandler();
	bool DeleteDBFileFromEMMC();
	bool CopyDBFile(DB_COPY_DIRECTION enDBCopyDirection);
	
private:
	bool deleteDBWALAndSHMFilesFromEMMC();
	bool copyDBWALAndSHMFilesFromEMMC(std::string strDestPath);
};


#endif /* SQLITEDB_H_ */
