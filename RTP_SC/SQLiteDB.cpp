/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 	 It contains the SQLite Data base related functions   
 
**********************************************************************************************************/
#include <semLib.h>
#include <ioLib.h>
#include <usrFsLib.h>
#include "SQLiteDB.h"
#include "Logger.h"
#include "commons.h"
#include "Common.h"

using namespace std;

SEM_ID SQLiteDB::SCDBMutex = 0;

/******************************************************************************
* \brief - Callback function called from Exec method.
*
* \param:
* vPtrData - Stores result in vPtrData as comma separated string values.
* argc - The number of columns in the result.
* argv - An array of pointers to strings obtained as if from sqlite3_column_text(), one for each column.
* azColName - An array of pointers to strings where each entry represents the name of corresponding result column as obtained from sqlite3_column_name().
*  
* \return - 0. Stores result in vPtrData as comma separated string values.
*
******************************************************************************/
static int Callback(void *vPtrData, int argc, char **argv, char **azColName)
{
	int nIndex = 0;
	
	std::string *strData = static_cast<std::string*>(vPtrData);
		
	for(nIndex = 0; nIndex < argc; nIndex++)
	{
		if(argv[nIndex] != NULL)
		{
			strData->append(argv[nIndex]);
			strData->append(DB_VALUE_SEPARATOR);
		}
		else
		{
			strData->append(DB_VALUE_SEPARATOR);
		}
	}
	
	return 0;
}

/**************************************************************************//**
* 
* \brief   - Constructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SQLiteDB::SQLiteDB()
{
	if(0 == SQLiteDB::SCDBMutex)
		SQLiteDB::SCDBMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
		
	m_ptrDB = NULL;
	m_bIsDBOpen = false;	
}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
SQLiteDB::~SQLiteDB()
{
	Close();
}

/**************************************************************************//**
* \brief   - Establishes the SQLite DB connection in SQLite handler object.
*
* \param   - None.
*
* \return  - SQLITE_OK in case of success else SQLite error code
*
******************************************************************************/
int SQLiteDB::EstablishDataBaseConnection()
{
	int DBErrCode = sqlite3_open_v2(DB_EMMC, &m_ptrDB, SQLITE_OPEN_READWRITE, NULL);
	if(DBErrCode)
	{
		m_bIsDBOpen = false;
		std::cout <<"\nSQLiteDB : EstablishDataBaseConnection Failure" << std::endl;
	}
	else
	{
		m_bIsDBOpen = true;
	}
	return DBErrCode;
}

/**************************************************************************//**
* 
* \brief   - Closes the SQLite DB connection.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int SQLiteDB::Close()
{
	int nErrCode = SQLITE_ERROR;
	
	if(m_ptrDB != NULL)
	{
		nErrCode = sqlite3_close(m_ptrDB);
		if(nErrCode != SQLITE_OK)
		{
			LOGERR("DataBase Connection not closed. nErrCode : %d",nErrCode,0,0);
		}
	}
		
	m_ptrDB = NULL;
	m_bIsDBOpen = false;
	
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Establish the SQLite DB connection and set below DB parameters:
*			auto_vacuum=none
*			journal_mode=wal
* \param   - None.
*
* \return  - SQLITE_OK in case of success else SQLite error code
*
******************************************************************************/
int SQLiteDB::SetDBParameters()
{
	int nResult = SQLITE_CANTOPEN;

	nResult = EstablishDataBaseConnection();
	if(nResult == SQLITE_OK)
	{
		nResult = setAutoVacuum();
		if(nResult == SQLITE_OK)
		{
			nResult = setJournalMode();
		}
	}
	
	return nResult;
}

/**************************************************************************//**
* 
* \brief - Calls the sqlite3_exec function to execute specific statement.
* sqlite3_exec() called from Exec() has a Callback function involved.
* Exec() is declared public.
* \param:
* strSqlStatement - SQLite query/statement to be executed.
* pnStatus - Query execution status/result.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.

* \return - Returns the result of execution of query. In case of database
* read operation (Select queries), the read database values are 
* returned as comma separated values.
*
******************************************************************************/
string SQLiteDB::Exec(std::string strSqlStatement, int *pnStatus, int nRetryCounter)
{
	std::string strOutBuffer = "";
	int nErrCode = SQLITE_ERROR;

	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR("\nSQLiteDB - Exec ERROR, Connection not opened",0,0,0);
	}
	else
	{	//Callback function returns the comma separated string values in "strOutBuffer".
		do
		{
			nErrCode = sqlite3_exec(m_ptrDB, strSqlStatement.c_str(), Callback, &strOutBuffer, NULL);
			switch(nErrCode)
			{
				case SQLITE_OK:
				break;
				
				case SQLITE_BUSY:
				case SQLITE_LOCKED:
				if(nRetryCounter > 0)
				{
					taskDelay(SQLITE_BUSY_RETRY_INTERVAL);
				}
				break;		
					
				default:
				break;
			}
			nRetryCounter--;		
		}while( ( nRetryCounter > 0 ) && ( nErrCode == SQLITE_BUSY || nErrCode == SQLITE_LOCKED) );
		
		if(!strOutBuffer.empty())
		{
			//Erase the last DB_VALUE_SEPARATOR (,) from the returned comma separated string.
			strOutBuffer.erase(strOutBuffer.size() - 1);
		}		
	}
	
	if(pnStatus != NULL)
	{
		*pnStatus = nErrCode;	
	}
	
	if(nErrCode != SQLITE_OK)
	{
		LOGERR("\nSQLiteDB - Exec FAILED, nErrCode: %d",nErrCode,0,0);
	}
	
	return strOutBuffer;
}

/******************************************************************************
* 
* \brief - Calls sqlite3 API for writing data into DB.
*
* \param:
* strSqlStatement - SQLite Query.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.

* \return  - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::SingleTransaction(std::string strSqlStatement, int nRetryCounter)
{
	return executeDBQuery(strSqlStatement, nRetryCounter);
}

/******************************************************************************
* 
* \brief - Bind the operation with transaction for writing bulk data.
*
* \param:
* strSqlStatement - SQLite Query.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::BulkTransaction(std::string strSqlStatement, int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR("BulkTransaction - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		nErrCode = executeDBQuery("BEGIN TRANSACTION;", nRetryCounter);
		if(nErrCode == SQLITE_OK)
		{
			nErrCode = executeDBQuery(strSqlStatement, 0);
			
			if(nErrCode == SQLITE_OK)
			{
				nErrCode = executeDBQuery("END TRANSACTION;", 0); //END TRANSACTION is an alias for COMMIT
				if(nErrCode != SQLITE_OK)
				{
					LOGERR("BulkTransaction - ERROR in END TRANSACTION, nErrCode: %d",nErrCode,0,0);	
				}
			}
			else
			{
				LOGERR((char *)"BulkTransaction - ERROR in executeDBQuery, Rolling Back, nErrCode: %d",nErrCode,0,0);
				if (executeDBQuery("ROLLBACK TRANSACTION;", 0) != SQLITE_OK)
				{
					LOGERR("BulkTransaction - ERROR in ROLLBACK TRANSACTION", 0,0,0);		
				}	
			}
		}
		else
		{
			LOGERR("BulkTransaction - ERROR, BEGIN TRANSACTION, nErrCode:%d",nErrCode,0,0);
		}
	}
	
	return nErrCode;
}

/******************************************************************************
* 
* \brief - Calls the SQLite commit via executeDBQuery.
*
* \param:
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::Commit(int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	std::string strQuery = "COMMIT;";
			
	nErrCode = executeDBQuery(strQuery, nRetryCounter);	
	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char *)"COMMIT - ERROR: %d",nErrCode,0,0);
 	}
 	
	return nErrCode;	
}

/******************************************************************************
 * \brief - This function performs Database Vacuum.
 *
 * \param:
 * nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
 *
 *  return - SQLITE_OK (0) on success, else error code.
 *
 ******************************************************************************/
int SQLiteDB::DBVacuum(int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	std::string strQuery = "VACUUM;";		
	nErrCode = executeDBQuery(strQuery, nRetryCounter);	
	if(nErrCode != SQLITE_OK)
	{
		LOGERR((char *)"SQLiteDB - DBVacuum - ERROR: %d",nErrCode,0,0);
 	}
	else
	{
		LOGDBG((char *)"SQLiteDB - DBVacuum - Successfully Done DB VACUUM",0,0,0);
	}
	
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Returns the DB Column Names
*
* \param   - SQLite Prepare Statement
*
* \return  - String 
*
******************************************************************************/
string SQLiteDB::GetDBColumnName(const char *SqlStatement, int *pnStatus)
{
	INT32 coloumnIndex = 0, columnCount = 0, ret = 0;
	string columnHeaderBuf = "";
	sqlite3_stmt* sqlStmt = NULL;
	
	if(m_bIsDBOpen == false)
	{
		ret = SQLITE_CANTOPEN;
		LOGERR("GetDBColumnName - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		ret = sqlite3_prepare_v2(m_ptrDB, SqlStatement, -1, &sqlStmt, 0);

		if(SQLITE_OK != ret)
		{
			LOGERR("SQLiteDB : GetDBColumnName : Error in preparing SQLite Statement --- %d",ret,0,0);
		}
		else
		{
			columnCount = sqlite3_column_count(sqlStmt);

			for(coloumnIndex = 0; coloumnIndex < columnCount; coloumnIndex++)
			{
				columnHeaderBuf += sqlite3_column_name(sqlStmt, coloumnIndex);
				
				if(coloumnIndex < ( columnCount - 1) )
				{
					columnHeaderBuf.append(DB_VALUE_SEPARATOR);			
				}
			}
		}

		ret = sqlite3_finalize(sqlStmt);

		if(SQLITE_OK != ret)
		{
			LOGERR("SQLiteDB : GetColumnCount : Finalize SQLite Statement with ErrCode : %d",ret,0,0);
		}
	}
	
	if(pnStatus != NULL)
	{
		*pnStatus = ret;	
	}
	
	return columnHeaderBuf;
}

/**************************************************************************//**
* \brief   - Returns the Number of Columns of DB
*
* \param   - SQLite Prepare Statement
*
* \return  - INT32 - columnCount 
*
******************************************************************************/
INT32 SQLiteDB::GetDBColumnCount(const char *SqlStatement, int *pnStatus)
{
	INT32 ret = 0, columnCount = 0;
	sqlite3_stmt* sqlStmt = NULL;
	
	if(m_bIsDBOpen == false)
	{
		ret = SQLITE_CANTOPEN;
		LOGERR("GetDBColumnCount - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		ret = sqlite3_prepare_v2(m_ptrDB, SqlStatement, -1, &sqlStmt, 0);
		
		if(SQLITE_OK != ret)
		{
			LOGERR("SQLiteDB : GetColumnCount : Preparing SQLite Statement with ErrCode : %d",ret,0,0);
		}
		else
		{
			columnCount = sqlite3_column_count(sqlStmt);
		}
		
		ret = sqlite3_finalize(sqlStmt);
		
		if(SQLITE_OK != ret)
		{
			LOGERR("SQLiteDB : GetColumnCount : Finalize SQLite Statement with ErrCode : %d",ret,0,0);
		}
	}
	
	if(pnStatus != NULL)
	{
		*pnStatus = ret;	
	}

	return columnCount;
}

/**************************************************************************//**
* \brief   - Reads recipe from specified table and copies into recipe vector
*
* \param   - string SqlStatement, UINT8 columnCount, vector<INT> &recipe
*
* \return  - UINT8 status (0-failed or 1-success)  
*
******************************************************************************/
UINT8 SQLiteDB::ReadRecipe(std::string SqlStatement, UINT8 columnCount, std::vector<INT32> &recipe)
{
	UINT8 status 		= 0;
	INT32 ErrCode 		= 0;
	sqlite3_stmt* stmt 	= NULL;
	UINT8 index 		= 0;
	
	if(columnCount > 0)
	{
		ErrCode = sqlite3_prepare_v2(m_ptrDB, SqlStatement.data(), SqlStatement.length(), &stmt, NULL);

		if(SQLITE_OK == ErrCode)
		{
			ErrCode = sqlite3_step(stmt);

			if(SQLITE_ROW == ErrCode)
			{
				while(index < columnCount)
				{
					recipe.push_back(sqlite3_column_int(stmt, index));
					index = index + 1;
				}
				status = 1;		/* return success */
			}
			else
			{
				LOGERR("ReadRecipe : Row failed with ErrCode : %d",ErrCode,0,0);
			}
		}
		else
		{
			LOGERR("ReadRecipe : prepare failed with ErrCode : %d",ErrCode,0,0);
		}
		
		ErrCode = sqlite3_finalize(stmt);
		if(SQLITE_OK != ErrCode)
		{
			LOGERR("SQLiteDB : ReadRecipe : Finalize SQLite Statement with ErrCode : %d",ErrCode,0,0);
		}
	}
	else
	{
		LOGERR("ReadRecipe : Invalid column count received : %d",columnCount,0,0);
	}

	return status;
}

/******************************************************************************
* \brief - sqlite3_exec wrapper with retry mechanism for SQLITE_BUSY/SQLITE_LOCKED error.
*sqlite3_exec() called from executeDBQuery() has NO Callback function involved.
*executeDBQuery() is declared private. 
* \param:
* strSqlStatement - Query to be executed.
* nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return - SQLITE_OK (0) on success, else error code.
*
******************************************************************************/
int SQLiteDB::executeDBQuery(std::string strSqlStatement, int nRetryCounter)
{
	int nErrCode = SQLITE_ERROR;
	
	if(m_bIsDBOpen == false)
	{
		nErrCode = SQLITE_CANTOPEN;
		LOGERR("executeDBQuery - ERROR, Connection not opened",0,0,0);
	}
	else
	{
		do
		{
			nErrCode = sqlite3_exec(m_ptrDB, strSqlStatement.c_str(), NULL, NULL, NULL);
			switch(nErrCode)
			{
				case SQLITE_OK:
				break;
				
				case SQLITE_BUSY:
				case SQLITE_LOCKED:
				if(nRetryCounter > 0)
				{
					taskDelay(SQLITE_BUSY_RETRY_INTERVAL);
				}
				break;	
						
				default:
				break;
			}
			nRetryCounter--;		
		}while( ( nRetryCounter > 0 ) && ( nErrCode == SQLITE_BUSY || nErrCode == SQLITE_LOCKED) );
	}
	
	if(nErrCode != SQLITE_OK)
	{
		LOGERR("executeDBQuery - FAILED, nErrCode: %d",nErrCode,0,0);
	}
	
	return nErrCode;
}

/**************************************************************************//**
* \brief   - Reads auto_vacuum value for database and sets to 0/none if not 
* 			 set to 0/none previously
*
* \param   - nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return  - SQLITE_OK if auto_vacuum is set to none/0, else SQLite error code. 
*
******************************************************************************/
int SQLiteDB::setAutoVacuum(int nRetryCounter)
{
	int nResult = SQLITE_ERROR;
	
	//Read the auto_vacuum value.
	std::string strQuery = "PRAGMA auto_vacuum;";
	std::string strAutoVacuumValue = Exec((char *)strQuery.c_str(), &nResult, nRetryCounter);
	if(nResult != SQLITE_OK)
	{
		LOGERR("SQLiteDB : Error in reading DB auto_vacuum value, nResult: %d",nResult,0,0);
	}
	else
	{
		//Check if auto_vacuum=0, if not equal to 0, then set to 0/none. 0 is same as none.
		if(strAutoVacuumValue != "0")
		{
			//Set the auto_vacuum to none if not set previously.	
			strQuery = "PRAGMA auto_vacuum=none;";
			Exec((char *)strQuery.c_str(), &nResult, nRetryCounter);
			if(nResult != SQLITE_OK)
			{
				LOGERR("SQLiteDB : Error in setting DB auto_vacuum to none, nResult: %d",nResult,0,0);
			}
		}
		else
		{
			nResult = SQLITE_OK;
		}
	}
	
	return nResult;
}

/**************************************************************************//**
* \brief   - Reads journal_mode for database and sets to WAL if not set to WAL 
* 			previously
*
* \param   - nRetryCounter - Number of retries in case SQLITE_BUSY/SQLITE_LOCKED error.
*
* \return  - SQLITE_OK if journal_mode is set to wal, else SQLite error code.  
*
******************************************************************************/
int SQLiteDB::setJournalMode(int nRetryCounter)
{
	int nResult = SQLITE_ERROR;
	
	//Read the journal_mode value.
	std::string strQuery = "PRAGMA journal_mode;";
	std::string strJournalModeValue = Exec((char *)strQuery.c_str(), &nResult, nRetryCounter);
	if(nResult != SQLITE_OK)
	{
		LOGERR("SQLiteDB : Error in reading DB journal_mode value, nResult: %d",nResult,0,0);
	}
	else
	{
		//Check if journal_mode=wal, if not equal to wal, then set to wal.
		if(strJournalModeValue != "wal")
		{
			//Set the journal_mode to WAL if not set previously.	
			strQuery = "PRAGMA journal_mode=WAL;";
			Exec((char *)strQuery.c_str(), &nResult, nRetryCounter);
			if(nResult != SQLITE_OK)
			{
				LOGERR("SQLiteDB : Error in setting DB journal_mode to WAL, nResult: %d",nResult,0,0);
			}
		}
		else
		{
			nResult = SQLITE_OK;
		}
	}
	
	return nResult;
}

/*************************************************************************//**
* \brief   -  Constructor.
*
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
DBFileHandler::DBFileHandler()
{

}

/*************************************************************************//**
* \brief   -  Destructor.
*
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
DBFileHandler::~DBFileHandler()
{

}

/*************************************************************************//**
* \brief   -  Delete DB file from EMMC along with Recipe0.txt file.
*
* \param   -  None
*
* \return  -  true on success else false
*
******************************************************************************/
bool DBFileHandler::DeleteDBFileFromEMMC()
{
	bool bIsDeleted = false;
	
	if(ERROR == rm(DB_EMMC))
	{
		LOGERR("Error deleting Database from EMMC",0,0,0);
	}
	else
	{
		bIsDeleted = true;
		LOGDBG("Database deleted from EMMC",0,0,0);	
		
		if (deleteDBWALAndSHMFilesFromEMMC() == false)
		{
			LOGERR("Error deleting Database WAL/SHM files from EMMC",0,0,0);
		}
	}
	
	if(ERROR == rm(RECIPE_ZERO_PATH))
	{
		LOGERR("Error deleting Recipe Zero file from EMMC",0,0,0);
	}
	
	return bIsDeleted;
}

/*************************************************************************//**
* \brief   -  Copies DB file from eMMC to external storage(USB or SD)
* 			  or from external storage(USB or SD) to eMMC.
*
* \param   -  UINT8 typeOfDevice
*
* \return  -  true on success else false
*
******************************************************************************/
bool DBFileHandler::CopyDBFile(DB_COPY_DIRECTION enDBCopyDirection)
{
	INT32 fd = OK;
	bool bIsCopied = false;
	bool bErrorOccured = false;
	string strSrcPath = "",strDestPath = "";
	bool bCopyDBWalAndDBShmFile = false;
	switch(enDBCopyDirection)
	{
		//Copy database From  EMMC to SD Card.
		//This case is not used/called from the code, but can be useful in future.
		case EMMC_TO_SD_CARD:
			strSrcPath.append(DB_EMMC);
			strDestPath.append(DB_COPY_SD);
			bCopyDBWalAndDBShmFile = true;
			break;
		
		//Copy database From  EMMC to USB.
		case EMMC_TO_USB:
			strSrcPath.append(DB_EMMC);
			strDestPath.append(DB_COPY_USB);
			bCopyDBWalAndDBShmFile = true;
			break;
		
		//Copy database From  SD Card to EMMC.
		//This case is not used/called from the code, but can be useful in future.	
		case SD_CARD_TO_EMMC:
			strSrcPath.append(SD_DB_PATH);
			strDestPath.append(EMMC_DISK_PATH);
			//Delete database sample.db-wal and sample.db-shm files from EMMC before copying sample.db file from SD card to EMMC
			if(deleteDBWALAndSHMFilesFromEMMC() == false)
			{
				bErrorOccured = true;
			}
			break;
		
		//Copy database From  USB to EMMC.
		//This case is not used/called from the code, but can be useful in future.
		case USB_TO_EMMC:
			strSrcPath.append(USB_COPY_PATH);
			strDestPath.append(EMMC_DISK_PATH);
			//Delete database sample.db-wal and sample.db-shm files from EMMC before copying sample.db file from USB to EMMC
			if(deleteDBWALAndSHMFilesFromEMMC() == false)
			{
				bErrorOccured = true;
			}
			break;
			
		default:
			bErrorOccured = true;
			break;
	}
		
	if(bErrorOccured == false)
	{
		fd = open(strDestPath.c_str(), O_CREAT, FSTAT_DIR);
		if(OK != fd)
		{
			if(ERROR == cp(strSrcPath.c_str(), strDestPath.c_str()))
			{
				LOGERR("Error copying Database from EMMC to external storage",0,0,0);
			}
			else
			{
				//copy database -wal and -shm files to destination in case of EMMC to SD card or EMMC to USB copy only and not in case of USB to EMMC or SD card to EMMC copy.
				if (bCopyDBWalAndDBShmFile == true)
				{
					if( copyDBWALAndSHMFilesFromEMMC(strDestPath) == false)
					{
						LOGERR("Error copying Database WAL/SHM files from EMMC to external storage",0,0,0);
					}
					else
					{
						bIsCopied = true;
						LOGDBG("Database copied. Check the emmcDB directory in external storage",0,0,0);
					}
				}
				else
				{
					LOGDBG("Database copied. Check the EMMC storage",0,0,0);
					bIsCopied = true;
				}
			}
			
			if(fd >= OK)
			{
				close(fd);
			}
		}
	}
	
	return bIsCopied;
}

/*************************************************************************//**
* \brief   -  Delete DB -wal and -shm files from EMMC if they exist on EMMC.
*
* \param   -  None
*
* \return  -  true on success else false
*
******************************************************************************/
bool DBFileHandler::deleteDBWALAndSHMFilesFromEMMC()
{
	bool bResult = false;
	bool bErrorOCcured = false;
	
	if( access( DB_WAL_EMMC, F_OK ) == OK ) 
	{
		if( rm(DB_WAL_EMMC) == ERROR )
		{
			bErrorOCcured = true;
			LOGERR("Error deleting Database WAL file from EMMC",0,0,0);	
		}
    }
	//The purpose of DeleteDBWALAndSHMFiles() method is to check if EMMC has 
	//sample.db-wal file present in EMMC and if present then delete it.
	//If the file is not present, then that is not an error condition.
	//Hence the else part is not needed here.
	
	if( access( DB_SHM_EMMC, F_OK ) == OK ) 
	{
		if( rm(DB_SHM_EMMC) == ERROR )
		{
			bErrorOCcured = true;
			LOGERR("Error deleting Database SHM file from EMMC",0,0,0);
		}
	}
	//The purpose of DeleteDBWALAndSHMFiles() method is to check if EMMC has 
	//sample.db-shm file present in EMMC and if present then delete it.
	//If the file is not present, then that is not an error condition.
	//Hence the else part is not needed here.
	
	if(bErrorOCcured == false)
	{
		bResult = true;
	}
	return bResult;
}

/*************************************************************************//**
* \brief   -  Copy DB -wal and -shm files from EMMC if they exist on EMMC.
*
* \param   -  strDestinationPath - Destination Path where we want to copy DB 
			  -wal and -shm files from EMMC
*
* \return  -   true on success else false
*
******************************************************************************/
bool DBFileHandler::copyDBWALAndSHMFilesFromEMMC(std::string strDestinationPath)
{
	bool bResult = false;
	bool bErrorOCcured = false;
	
	if( access( DB_WAL_EMMC, F_OK ) == OK ) 
	{
		if( cp(DB_WAL_EMMC, strDestinationPath.c_str()) == ERROR )
		{
			bErrorOCcured = true;
			LOGERR("Error Copying Database WAL file from EMMC to destination",0,0,0);
		}
	}
	
	if( access( DB_SHM_EMMC, F_OK ) == OK ) 
	{	
		if( cp(DB_SHM_EMMC, strDestinationPath.c_str()) == ERROR )
		{
			bErrorOCcured = true;
			LOGERR("Error Copying Database SHM file from EMMC to destination",0,0,0);
		}
	}
	
	if(bErrorOCcured == false)
	{
		bResult = true;
	}
	
	return bResult;
}


