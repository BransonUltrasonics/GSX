/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#include  "DataBaseManager.h"
#include  "RunTimeFeature.h"
#include  "ReportGeneration.h"
#include  <sys/stat.h>
#include  "commons.h"
#include  "ControlTask.h"
#include  "AlarmManager.h"
#include  "DataBaseConfiguration.h"

extern "C"
{
	#include "customSystemCall.h"	
}

#include <sstream>
#include <vector>
#include <bitset>
using namespace std;

/* Static member variables are initialized */
UINT8  DataBaseManager::DBStatusFlag = 0;


/**************************************************************************//**
* \brief   - 
*
* \param   - LIMITED_TABLES DbTable - Which table to check
*
* \return  - None.
*
******************************************************************************/
void DataBaseManager::ProcessTableCapacity(LIMITED_TABLES DbTable)	
{		
	string DbQuery, tableName, queryReturn;
	INT32 queryStatus = RESPONSE_FAIL;
	UINT32 storageLimit = 0, numEntries = 0;
	intf_SysConfig_t  sysConfig;
	UINT8   DBStorageStatus;
	
	
	CP->GetSysConfContext(sysConfig);

	switch(DbTable)
	{
	case ALARM_LOG_TABLE:
		tableName = TABLE_ALARM_LOG;
		storageLimit = DB_RECORDS_ALARM_STORAGE_LIMIT;
		break;
	case EVENT_LOG_TABLE:
		tableName = TABLE_EVENT_LOG;
		storageLimit = DB_RECORDS_EVENT_STORAGE_LIMIT;
		break;
	case WELD_RESULT_TABLE:
		tableName = TABLE_WELD_RESULT;
		storageLimit = DB_RECORDS_STORAGE_WELD_RESULT_LIMIT;
		break;
	case WELD_SIGNATURE_TABLE:
		tableName = TABLE_WELD_SIGNATURE;
		storageLimit = DB_RECORDS_STORAGE_WELD_SIGNATURE_LIMIT;
		break;
	case HORN_SCAN_TABLE: //In case of horn scan the TABLE_SCAN_RESULT and TABLE_SCAN_SIGNATURE are both considered
		tableName = TABLE_SCAN_SIGNATURE;
		storageLimit = DB_RECORDS_SCAN_STORAGE_LIMIT;
		break;
	default:
		break;
	}

	DbQuery = "SELECT COUNT(1) FROM "+ tableName + ";";
	queryReturn = m_DbConn.Exec((char *)DbQuery.c_str(), &queryStatus);

	if(SQLITE_OK == queryStatus)
	{	
		numEntries = atoi(queryReturn.c_str());	
		
		//check for 80% full
		if(MEM_FULL_STOP == sysConfig.MemFullActionIs && numEntries == ((storageLimit * (UINT64)DB_STORAGE_ALERTLEVEL1)/100))
		{
			SendCapacityWarning(DbTable);
		}

		//Check for full
		if(numEntries >= storageLimit)
		{
			if(MEM_FULL_STOP == sysConfig.MemFullActionIs)
			{
				SendCapacityAlarm(DbTable);
			}
			else
			{
				while(numEntries > storageLimit)
				{
					DeleteOldRecords(DbTable);
					numEntries--;
				}
			}
		}
	}	
	else
	{
		LOGERR("DB_T : Get row count : ErrCode from DB : %d", queryStatus, 0, 0);
	}

	//check total memory capacity
	CheckDBStorageStatus();
}


/**************************************************************************//**
* \brief   - Sends alarm for table full and sets table full flag in common property
*
* \param   - LIMITED_TABLES DbTable - Which table
*
* \return  - None.
*
******************************************************************************/
void DataBaseManager::SendCapacityAlarm(LIMITED_TABLES DbTable)	
{	
	switch(DbTable)
		{
		case ALARM_LOG_TABLE:
			CP->SetMemStorageFullStatus(ALARM_MEM_CB, true);	
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ALARM_LOG_FULL_EFA);
			break;
		case EVENT_LOG_TABLE:
			CP->SetMemStorageFullStatus(EVENT_MEM_CB, true);
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_EVENT_LOG_FULL_EFA);
			break;
		case WELD_RESULT_TABLE:
			CP->SetMemStorageFullStatus(WELD_MEM_CB, true); /* To prevent welding until DB is cleared */
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELD_RESULT_FULL_EFA);
			break;
		case WELD_SIGNATURE_TABLE:
			CP->SetMemStorageFullStatus(WELD_MEM_CB, true); /* To prevent welding until DB is cleared */
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELD_GRAPH_FULL_EFA);
			break;
		case HORN_SCAN_TABLE: 
			CP->SetMemStorageFullStatus(SCAN_MEM_CB, true);	/* To prevent Scans until DB is cleared */
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_HORN_SCAN_FULL_EFA);
			break;
		default:
			break;
		}
}


/**************************************************************************//**
* \brief   - Sends alarm for table 80% full
*
* \param   - LIMITED_TABLES DbTable - Which table
*
* \return  - None.
*
******************************************************************************/
void DataBaseManager::SendCapacityWarning(LIMITED_TABLES DbTable)	
{	
	switch(DbTable)
		{
		case ALARM_LOG_TABLE:
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_ALARM_LOG_FULL_WRA);
			break;
		case EVENT_LOG_TABLE:
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_EVENT_LOG_FULL_WRA);
			break;
		case WELD_RESULT_TABLE:
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELD_RESULT_FULL_WRA);
			break;
		case WELD_SIGNATURE_TABLE:
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_WELD_GRAPH_FULL_WRA);
			break;
		case HORN_SCAN_TABLE: 
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_HORN_SCAN_FULL_WRA);
			break;
		default:
			break;
		}
}


/**************************************************************************//**
* \brief   -  Delete record for particular log
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::DeleteOldRecords(LIMITED_TABLES DbTable)	
{	
	string 	DbQuery, freeList, sRowID, selectQry, weldData;
	bool	bIsIdValid = true;
	INT32 	queryStatus = RESPONSE_FAIL;	
	UINT8 	status = 0;

	switch(DbTable)
	{	
		case WELD_SIGNATURE_TABLE:
			DbQuery = "DELETE FROM WeldResultSignature WHERE ROWID IN (SELECT ROWID FROM WeldResultSignature ORDER BY ROWID ASC LIMIT 1);";
			break;
			
		case WELD_RESULT_TABLE:	
			DbQuery = "DELETE FROM WeldResultTable WHERE ROWID IN (SELECT ROWID FROM WeldResultTable ORDER BY ROWID ASC LIMIT 1);";	
			break;
	
		case HORN_SCAN_TABLE:		
			DbQuery = "DELETE FROM HornSignatureTable WHERE ROWID IN (SELECT ROWID FROM HornSignatureTable ORDER BY ROWID ASC LIMIT 1);DELETE FROM HornResultTable WHERE ROWID IN (SELECT ROWID FROM HornResultTable ORDER BY ROWID ASC LIMIT 1);";		
			break;
	
		case ALARM_LOG_TABLE:	
			DbQuery = "DELETE FROM AlarmLogs WHERE ROWID IN (SELECT ROWID FROM AlarmLogs ORDER BY ROWID ASC LIMIT 1);";
			break;
	
		case EVENT_LOG_TABLE:
			DbQuery = "DELETE FROM EventLogTable WHERE ROWID IN (SELECT ROWID FROM EventLogTable ORDER BY ROWID ASC LIMIT 1);";
			break;	
	
		default:
			bIsIdValid = false;
			LOGDBG("DB_T : Trying to send Invalid Msg id for deleting DB records in circular manner : %d",m_DbMsgID,0,0); 
			break;
	}

	if(bIsIdValid)
	{
#ifdef DEBUG_PRINT
		cout << "Row delete query: " << DbQuery << endl; 
#endif /* DEBUG_PRINT */

		if(TO_DB_TASK_HORN_SCAN_RESULT == m_DbMsgID)
		{			
			queryStatus = m_DbConn.BulkTransaction((char *)DbQuery.c_str());	
		}
		else
		{
			queryStatus = m_DbConn.SingleTransaction((char *)DbQuery.c_str());	
		}	

		if(0 != queryStatus)						
		{
			LOGERR("DB_T : Delete record failed with an ErrCode : %d",queryStatus,0,0);			
			if(TO_DB_TASK_HORN_SCAN_RESULT == m_DbMsgID)
			{				
				RetryMechanism((char *)DbQuery.c_str(), BULK_TRANSACTION, status);
			}
			else
			{
				RetryMechanism((char *)DbQuery.c_str(), SINGLE_TRANSACTION, status);
			}						
		}

		if( (0 == queryStatus) || (RESPONSE_SUCCESS == status) )
		{			
			LOGDBG("DB_T : Record was successfully deleted",0,0,0);
		}
		else
		{
			LOGERR("DB_T : Delete record failed even after retries!!!",0,0,0);	
		}
	}
}


/**************************************************************************//**
* 
* \brief   - Checks table capacities for alarms if configuration set to stop or clears flags if set to continue - called when changes are made to config
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void DataBaseManager :: CheckCapacitiesForNewConfig()
{	//TODO: redo this function
	string DbTable;
	intf_SysConfig_t  sysConfig;
	bool			  storageLimitCrossed = false;
	CP->GetSysConfContext(sysConfig);
	if(MEM_FULL_STOP == sysConfig.MemFullActionIs)
	{
		for(UINT8 iTable = 0; iTable <= HORN_SCAN_TABLE;iTable++)
		{
			ProcessTableCapacity((LIMITED_TABLES)iTable);
		}
	}	
	else if(MEM_FULL_CONTINUE == sysConfig.MemFullActionIs)
	{		
		CP->SetMemStorageFullStatus(RESET_MEM_FULL_CB);				
	}
}


/**************************************************************************//**
* \brief   - Check the EMMC memory capacity and raise alarms as capacity is approached or reached
* 
* \brief   - Uses DBStatusFlag to keep track of which warning levels have already been sent
*
* \param   - None
*
* \return  - UINT8 - statusCount 
*
******************************************************************************/
void DataBaseManager::CheckDBStorageStatus()
{
	bool setFlag=0;
	INT16 ret = OK;
	UINT64 totalSpace=0, freeSpace=0, usedSpace=0;
	struct statfs fileInf;
	
	ret = statfs(EMMC_DISK_PATH, &fileInf);
	
	if(OK == ret)
	{
		totalSpace= (UINT64)fileInf.f_bsize * (UINT64)fileInf.f_blocks;
		freeSpace= (UINT64)fileInf.f_bsize * (UINT64)fileInf.f_bavail;
		usedSpace=totalSpace - freeSpace;
		
		if(usedSpace<((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL1)/100))
		{
			DBStatusFlag = 0;
		}
		else if((usedSpace>=((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL1)/100)) && (usedSpace<((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL2))/100))
		{
			setFlag= (DBStatusFlag >> CHECK_STATUS_BIT0) & 1;
			
			if(setFlag==false)
			{
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_DISK_MEM_FULL_WRA);
				DBStatusFlag |= (1<< CHECK_STATUS_BIT0);
			}
		}
		else if((usedSpace>=((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL2)/100)) && (usedSpace<((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL3))/100))
		{
			setFlag= (DBStatusFlag >> CHECK_STATUS_BIT1) & 1;
			
			if(setFlag==false)
			{
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_DISK_MEM_FULL_WRA);
				DBStatusFlag |= (1<< CHECK_STATUS_BIT1);
			}
		}
		else if((usedSpace>=((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL3)/100)) && (usedSpace<((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL4)/100)))
		{
			setFlag= (DBStatusFlag >> CHECK_STATUS_BIT2) & 1;
			
			if(setFlag==false)
			{
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_DISK_MEM_FULL_WRA);
				DBStatusFlag |= (1<< CHECK_STATUS_BIT2);
			}	
		}
		else if((usedSpace>=((totalSpace *  (UINT64)DB_STORAGE_ALERTLEVEL4)/100)))
		{
			setFlag= (DBStatusFlag >> CHECK_STATUS_BIT3) & 1;
			
			if(setFlag==false)
			{
				AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_DISK_MEM_FULL_EFA);
				CP->SetMemStorageFullStatus(DISK_MEM_FULL_CB, true);
				DBStatusFlag |= (1<< CHECK_STATUS_BIT3);
			}	
		}

	}
	else
	{
		LOGERR("DB_T : CheckDBStorageStatus : Error in Reading EMMC FIle System Information: %d",ret,0,0); 
	}
	
}

/**************************************************************************//**
* \brief  - Runs optimization on the DB by using the vacuum query.
* 			The free list displays while the operation
* 			takes place. The optimization can take anywhere between 2-60
* 			seconds therefore should not be performed while cycling.
*
* \param  - None
*
* \return - None
*
******************************************************************************/
string DataBaseManager::OptimizeDB()
{
	SQLiteDB dataBase;
	string 	DbQuery, freeList;
	string response ("Records optimization failed");
	INT32 	queryStatus = SQLITE_ERROR;	
	float startTime;
	float endTime;
	float elapsedTime;
	char results[MSG_SIZE];
		
	if (dataBase.EstablishDataBaseConnection() == SQLITE_OK)
	{
		DbQuery.clear();
		DbQuery = "PRAGMA freelist_count;";
		freeList = dataBase.Exec((char *)DbQuery.c_str(), &queryStatus);
		
		if(SQLITE_OK == queryStatus)
		{			
			printf ("\nDataBase free records count: %s Optimizing...\n", freeList.c_str());
			
			startTime = TimeStamp();
			dataBase.DBVacuum();
			endTime = TimeStamp();
			elapsedTime = endTime-startTime;
			
			snprintf (results, sizeof (results), "%s records optimized in %f seconds.", freeList.c_str(), elapsedTime);
			response = results;
			printf ("\n");
			printf (results);
			printf ("\n\n");
		}
		else
		{
			printf ("\nRecords optimization failed.\n\n");
		}
		
		dataBase.Close();
	}
	
	return response;
}
