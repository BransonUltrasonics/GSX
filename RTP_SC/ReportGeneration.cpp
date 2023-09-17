/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Report Generation functions to generate the CSV and PDF Report for WeldResult and Graphdata

 **********************************************************************************************************/
#include <sys/stat.h>
#include <ioLib.h>
#include <cmath>
#include "Common.h"
#include "ReportGeneration.h"
#include "versions.h"
#include "Counters.h"
#include "Logger.h"
#include  "DataBaseConfiguration.h"
#include "SysConfigEeprom.hpp"
#include "ActiveRecipe.h"
#include <string>
#include <usrFsLib.h>
#include <sstream>
#include  "DataBaseManager.h"
#include  "SC.h"
#include "AlarmNames.h"
#include "version_common.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "SysConfigEeprom.hpp"

using namespace std;

#ifdef PDF_GENERATION
using namespace PDFHummus;
#endif /* PDF_GENERATION */

/* Static member variables are initialized */
UINT32 ReportGeneration::RecordCount=0;
Report_Message ReportGeneration::m_reportReqData = {0};
INT16 CSVFormat::csvFd = -1;
bool ReportGeneration::m_reportGenerationFlag = true;

char* PDFReportGeneration::ActuatorInputStr[]=
{
	"ACTUATORINPUT_NOTUSED",
    "PARTPRESENT",
	"TOOLING_IN",
    "HOMEPOSITION_IN",
    "READY_POSITION_IN"
};

char* PDFReportGeneration::PowerSupplyInputStr[]=
{
	"PSINPUT_NOTUSED",
    "CYCLEABORT",
    "US_DISABLE",
    "RESET",
    "HOLD_DELAY",	
	"CONFIRM_REJECT",
	"HORN_SEEK_IN"
};

char* PDFReportGeneration::ActuatorOutputStr[]=
{
	"ACTUATOROUTPUT_NOTUSED",
    "HOMEPOSITION_OUT",
    "READY_POSITION_OUT",
    "PB_RELEASE",
    "TOOLING_OUT",
	"SOLENOID_VALVE",
	"HOLD_ACTIVE"
};

char* PDFReportGeneration::PowerSupplyOutputStr[]=
{
	"PSOUTPUT_NOTUSED",
    "READY",
    "SONICS_ACTIVE",
    "GENERAL_ALARM",
    "CYCLE_RUNNING",
    "CYCLE_OK",
	"EXTERNAL_BEEPER",
	"CUSTOM_ALARM",
    "REJECTPART_ALARM",
    "SUSPECTPART_ALARM",
	"HORN_SEEK_OUT",
	"WELD_ACTIVE",
	"PSHOLD_ACTIVE"
};

char* PDFReportGeneration::AnalogInputStr[]=
{ 
	"ANALOGINPUT_NOTUSED",
    "AMPLITUDE_SETTING", 
    "FREQUENCY_OFFSET"
};

char* PDFReportGeneration::AnalogOutputStr[]= 
{
	"ANALOGOUTPUT_NOTUSED",
    "POWER_OUT",
    "FREQUENCY_OUT"
} ;

/**************************************************************************//**
* \brief   - Constructor
*
* \param   - None
*
* \return  - None 
******************************************************************************/
ReportGeneration::ReportGeneration()
 {

	m_reportType = 0 ;
	m_storageFormat = 0 ;
	m_deleteID = 0;
	m_storageDestination = NULL ;
	m_alarmFlag = 0;
	m_storageFd = 0;
	m_LastRowNum = 0;
	m_FirstRowNum = 0;
	m_currentRecipeNum = 0;
	m_currentCycleCounter = 0;
	m_currentDateTime = "";
	
	/* Initialize the DB connection pointer */
	m_DbConn = &(m_DbConnInstance);
	
	/*Initialize Report Generation Directory Descriptor*/
	reportDir=NULL;
	CP = CommonProperty :: getInstance(); 
	UI_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[UI_T]);
 }

/**************************************************************************//**
* \brief   - Parameter Constructor
*
* \param   - Report_Message& a_reportMsg
* \param   - SQLiteDB* a_dbPtr
*
* \return  - None
*
******************************************************************************/
ReportGeneration::ReportGeneration(Report_Message& a_reportMsg)
{
	UINT8 strLength  = 0;
	m_storageDestination = NULL;
	/* Initializing the Message parameters */
	m_reportType = a_reportMsg.reportID ;
	m_storageFormat = a_reportMsg.storageFormat ;
	m_deleteID = a_reportMsg.deleteID;

	strLength = strlen(a_reportMsg.reportBuff);

	if(strLength == 0)
	{
		LOGDBG("DB_T : Constructor: No Storage destination specified",0,0,0);
	}
	else
	{	
		m_storageDestination = (UINT8*)malloc(sizeof(UINT8) * strLength) ;
	}
	
	/* TODO: Currently the default storage destination is default USB. Later a check is to be placed to see if the storage destination is as expected */
	if(NULL == m_storageDestination)
	{
		LOGERR("ReportGeneration_T : Constructor: Memory allocation failed",0,0,0);
	}
	else
	{
		memcpy(m_storageDestination , a_reportMsg.reportBuff , strLength) ;
	}

	/* Initialize the DB connection pointer */
//	m_DbConn = a_dbPtr ;
	m_DbConn = &(m_DbConnInstance);
	
	/*Initialize the alarm flag*/
	m_alarmFlag=0;
	/*Initialize the File Descriptor for USB Stick*/
	m_storageFd = 0;
	
	/*Initialize Report Generation Directory Descriptor*/
	reportDir=NULL;
	
	/* Initialize the string list with units */
	m_stdUnitMap["TotalAbsolute"] = "mm" ;
	m_stdUnitMap["DateTime"] = "y/m/d hh:mm:ss" ;
	m_stdUnitMap["WeldEnergy"] = "J" ;
	m_stdUnitMap["WeldPeakPower"] = "%" ;
	m_stdUnitMap["StartFrequency"] = "Hz" ;
	m_stdUnitMap["FrequencyChange"] = "Hz" ;
	m_stdUnitMap["Amplitude"] = "%" ;
	m_stdUnitMap["Frequency"] = "Hz" ;
	m_stdUnitMap["Power"] = "%" ;
	m_stdUnitMap["Current"] = "%" ;
	m_stdUnitMap["Phase"] = "deg" ;
	m_stdUnitMap["Energy"] = "J" ;
	m_stdUnitMap["Velocity"] = "mm/s" ;
	m_stdUnitMap["AbsoluteDistance"] = "mm" ;
	m_stdUnitMap["CollapseDistance"] = "mm" ;
	m_stdUnitMap["WeldTime"] = "s" ;
	m_stdUnitMap["Time"] = "s" ;
	m_stdUnitMap["MaxWeldForce"] = "N" ;
	m_stdUnitMap["EndHoldForce"] = "N" ;
	m_stdUnitMap["TriggerPoint"] = "mm" ;
	
	m_UsedSizeInBytes = 0;
	m_LastRowNum = 0;
	m_FirstRowNum = 0;
	m_currentRecipeNum = 0;
	m_currentCycleCounter = 0;
	m_currentDateTime = "";
	CP = CommonProperty :: getInstance(); 
	UI_MSG_Q_ID 	= CP->GetMsgQId(cTaskName[UI_T]);
}

/**************************************************************************//**
* \brief   - Destructor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
ReportGeneration::~ReportGeneration()
{

	free(m_storageDestination);
	m_storageDestination=NULL;
}

/**************************************************************************//**
* \brief   - Returns the Report ID
*
* \param   - None
*
* \return  - UINT8 - ReportID
*
******************************************************************************/
Report_Message ReportGeneration::GetReportData()
 {
	return m_reportReqData;
 }

/**************************************************************************//**
* \brief   - Establish the DB connection
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void ReportGeneration::ConnectDB()
{
	m_DbConn->EstablishDataBaseConnection();
}

/**************************************************************************//**
* \brief   - Close the DB connection and release memory associated with DB
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ReportGeneration::CloseDB()
{
	m_DbConn->Close();
}

/**************************************************************************//**
* \brief   - Check the Storage Device whether it is connected or not
*
* \param   - None
*
* \return  - STATUS - SUCCESS or FAILURE
*
******************************************************************************/
STATUS ReportGeneration::CheckStorageDevice()
{
	INT8 status=OK;
	
	m_storageFd=open(USB_DEVICEPATH,  0, 0);

	if(m_storageFd<0)
	{
		status=FAILURE;
	}
	else
	{
		status = OpenDirectoryFileDescriptor();
		
		if(FAILURE==status)
		{
			status = USB_MOUNT_ERROR;
		}
	}

	return status;
}
/**************************************************************************//**
* \brief   - Close the opened storage device
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ReportGeneration::CloseStorageDevice()
{
	close(m_storageFd);
	m_storageFd = -1;
}

/**************************************************************************//**
* \brief   - Opens Report Generation Directory if not there, create new directory
*
* \param   - None
*
* \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS ReportGeneration::OpenDirectoryFileDescriptor()
{
	STATUS status = OK;
	
	reportDir=opendir(REPORT_GEN_DIRECTORY_PATH);

	/*Creating new Directory for storing Report Generation Files*/
	if(NULL==reportDir)
	{
		if((ERROR==mkdir(REPORT_GEN_DIRECTORY_PATH, 0644)))
		{
			LOGERR("ReportGeneration : OpenDirectoryFileDescriptor: Error in Creating Report File Directory due to unknown File System",0,0,0);
			status = FAILURE ;
		}	
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Close Report Generation Directory
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void ReportGeneration::CloseDirectoryFileDescriptor()
{
	closedir(reportDir);
	reportDir=NULL;
}

/**************************************************************************//**
* \brief   - Check whether DB is empty or not
*
* \param   - None
*
* \return  - bool - TRUE/FALSE
*
******************************************************************************/
bool ReportGeneration::CheckDBEmpty()
{
	bool status = false;
	
	INT32 queryStatus = 0;
	UINT32 recordCount=0;
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	string DBCount="";
	
	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT COUNT(1) FROM WeldResultTable");
			break;
			
		case WELDSIGNATURE_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT COUNT(1) FROM WeldResultSignature");
			break;
		
		case EVENT_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT COUNT(1) FROM EventLogTable");
			break;
			
		case ALARM_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT COUNT(1) FROM AlarmLogs");
			break;

		case RECIPE_SETTING:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT COUNT(1) FROM WeldRecipeTable");
			break;
		
		case USER_ID:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT COUNT(1) FROM UserProfiles");
			break;

		case SYSTEM_SETUP:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT COUNT(1) FROM SystemConfiguration");
			break;
			
		default:
		LOGERR("ReportGeneration : CheckDBEmpty: Unknown Report ID Received",0,0,0);
				break;
	}
	
	DBCount = m_DbConn->Exec(SelectQuery, &queryStatus);
	
	if(queryStatus==0)
	{
		recordCount=atoi(DBCount.c_str());
	}
	else
	{
		LOGERR("ReportGeneration : CheckDBEmpty: DB Query Error",0,0,0);
	}
	
	if(recordCount==0)
	{
		status = true;
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Get the number of Column Names
*
* \param   - None
*
* \return  - string - Report Header 
*
******************************************************************************/
string ReportGeneration::ReadReportHeader()
{
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	string columnHeaderBuf = "";
	intf_SysConfig_t SysConfigInfo;
	RT.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);
	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
			switch(m_storageFormat)
			{
				case CSV_FORMAT:
					snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM WeldResultTable WHERE 0=1;", WELD_RESULT_CSV_PARAM_STRING);
					break;
		
				case PDF_FORMAT:
					snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM WeldResultTable WHERE 0=1;",WELD_RESULT_PARAM_STRING);
					break;
			}
			columnHeaderBuf = m_DbConn->GetDBColumnName(SelectQuery);
			break;

		case WELDSIGNATURE_TYPE:
			columnHeaderBuf = WELD_SIGNATURE_PARAM_STRING;
			break;

		case EVENT_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM EventLogTable WHERE 0=1;",EVENT_LOG_PARAM_STRING);
			columnHeaderBuf = m_DbConn->GetDBColumnName(SelectQuery);
			break;

		case ALARM_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM AlarmLogs WHERE 0=1;",ALARM_LOG_PARAM_STRING);
			columnHeaderBuf = m_DbConn->GetDBColumnName(SelectQuery);
			columnHeaderBuf += ", AlarmName";
			Replace(columnHeaderBuf, "RecipeNumber", "Recipe");
			Replace(columnHeaderBuf, "RecipeVerNumber", "RecipeVer");
			break;
			
		case RECIPE_SETTING:
			columnHeaderBuf = WELD_RECIPE_PARAM_STRING;
			columnHeaderBuf += DB_DELIMITER;
			columnHeaderBuf += STACK_RECIPE_PARAM_STRING;
			
			//Shows Dynamic parameters if Dynamic RunTime flag is enabled
			if(ENABLE == RT.ModelFeature.RtFeature2.Dynamic_Flag_B49){
				columnHeaderBuf += DB_DELIMITER;
				columnHeaderBuf += DYNAMIC_RECIPE_PARAM_STRING;
			}
			break;
		
		case USER_ID:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM UserProfiles WHERE 0=1;",USER_ID_PARAM_STRING);
			columnHeaderBuf = m_DbConn->GetDBColumnName(SelectQuery);
			break;

		case SYSTEM_SETUP:
			break;
			
		default:
			LOGERR("ReportGeneration : ReadReportHeader: Unknown Report ID Received",0,0,0);
			break;
	}
	
	if(columnHeaderBuf.empty())
	{
		LOGERR("ReportGeneration : ReadReportHeader: Error in Getting the DB Column Name",0,0,0);
	}
	else
	{
		if((WELDRESULT_TYPE == m_reportType) && (CSV_FORMAT == m_storageFormat))
		{
			Replace(columnHeaderBuf, ALARM_FLAG_PARAM_STRING, ALARM_ID_PARAM_STRING);
			columnHeaderBuf.append("\n");
			/* To access system unit */
			CP->GetSysConfContext(SysConfigInfo);
			/*Insert Imperial Units Heder for WeldResult only*/
			if(SysConfigInfo.UnitIs == IMPERIAL)
			{
				columnHeaderBuf.append(""WELD_RESULT_CSV_IMPERIAL_UNITS_STRING"");
			}
			/*Insert Metric Units Heder for WeldResult only*/
			else
			{
				columnHeaderBuf.append(""WELD_RESULT_CSV_METRIC_UNITS_STRING"");
			}
		}
	}
	
	return columnHeaderBuf;
}

/**************************************************************************//**
* \brief   - Gets RTC Date and TimeStamp.
*
* \param   - string DateTime 
*
* \return  - None
*
******************************************************************************/
void ReportGeneration::GetDateTimeStamp(string &TimeStamp)
{
	RTCtimerStr SysTime={0x00};
	char timeBuf[TIMESTAMP] = {0x00};
	GetRTC(&SysTime);
	
	SysTime.tm_year = 1900 + SysTime.tm_year;
	SysTime.tm_mon  = 1 + SysTime.tm_mon;
	sprintf(timeBuf,"_%d%02d%02d_%02d%02d%02d",SysTime.tm_year,SysTime.tm_mon,SysTime.tm_mday,SysTime.tm_hour,SysTime.tm_min,SysTime.tm_sec);
	TimeStamp = timeBuf;
}

/**************************************************************************//**
* \brief   - Get the number of Columns
*
* \param   - None
*
* \return  - INT16 - Total number of Columns   
*
******************************************************************************/
INT32 ReportGeneration::GetColumnCount()
{
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	INT32 columnCount=0;
	
	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
		switch(m_storageFormat)
		{
				case CSV_FORMAT:
					snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM WeldResultTable WHERE 0=1;", WELD_RESULT_CSV_PARAM_STRING);
					break;
					
				case PDF_FORMAT:
					snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM WeldResultTable WHERE 0=1;",WELD_RESULT_PARAM_STRING);
					break;
		 }
		break;
								
		case WELDSIGNATURE_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT * FROM WeldResultSignature WHERE 0=1");
			break;	
			
		case EVENT_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT * FROM EventLogTable WHERE 0=1");
			break;	
		
		case ALARM_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT * FROM AlarmLogs WHERE 0=1");
			break;

		case RECIPE_SETTING:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT * FROM WeldRecipeTable WHERE 0=1");
			break;
			
		case USER_ID:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM UserProfiles WHERE 0=1;",USER_ID_PARAM_STRING);
			break;

		case SYSTEM_SETUP:
			//snprintf(SelectQuery,sizeof(SelectQuery), "SELECT * FROM SystemConfiguration WHERE 0=1");
			break;
			
		default:
			LOGERR("ReportGeneration : GetColumnCount: Unknown Report ID Received",0,0,0);
			break;
	}
	columnCount=m_DbConn->GetDBColumnCount(SelectQuery);

	if(0==columnCount)
	{
		LOGERR("ReportGeneration : GetColumnCount: Error in Getting the DB Column Count",0,0,0);
	}
	return columnCount;
}

/**************************************************************************//**
* \brief   - Get the count of database
*
* \param   - None
*
* \return  - INT32 - Total count of Records   
*
******************************************************************************/
INT32 ReportGeneration::GetDBCount()
{
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	INT32 queryStatus = 0;
	string dbCountStr = "";
	INT32 dbCount=0;
	
	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT count(*) FROM WeldResultTable;");
			break;
								
		case WELDSIGNATURE_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT count(*) FROM WeldResultSignature;");
			break;	

		case EVENT_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT count(*) FROM EventLogTable;");
			break;	
			
		case ALARM_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT count(*) FROM AlarmLogs;");
			break;	
			
		case RECIPE_SETTING:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT count(*) FROM WeldRecipeTable;");
			break;	
			
		case USER_ID:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT count(*) FROM UserProfiles;");
			break;				

		case SYSTEM_SETUP:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT count(*) FROM SystemConfiguration;");
			break;	
			
		default:
			LOGERR("ReportGeneration : GetDBCount: Unknown Report ID Received",0,0,0);
			break;
	}
	dbCountStr = m_DbConn->Exec(SelectQuery, &queryStatus);
	
	dbCount = atoi(dbCountStr.c_str());

	if(0 != queryStatus)
	{
			LOGERR("ReportGeneration : GetDBCount : ErrCode from DB : %d",queryStatus,0,0); 
	}
	return dbCount;
}


/**************************************************************************//**
* \brief   - Get the Max RowID of database
*
* \param   - None
*
* \return  - INT32 - Total count of Records   
*
******************************************************************************/
UINT64 ReportGeneration::GetDBMinRowID()
{
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	INT32 queryStatus = 0;
	string dbCountStr = "";
	UINT64 dbCount=0;
	
	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT min(ROWID) FROM WeldResultTable;");
			break;
								
		case WELDSIGNATURE_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT min(ROWID) FROM WeldResultSignature;");
			break;	

		case EVENT_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT min(ROWID) FROM EventLogTable;");
			break;	
			
		case ALARM_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT min(ROWID) FROM AlarmLogs;");
			break;	
			
		case RECIPE_SETTING:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT min(ROWID) FROM WeldRecipeTable;");
			break;	
			
		case USER_ID:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT min(ROWID) FROM UserProfiles;");
			break;				

		case SYSTEM_SETUP:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT min(ROWID) FROM SystemConfiguration;");
			break;	
			
		default:
			LOGERR("ReportGeneration : GetDBMinRowID: Unknown Report ID Received",0,0,0);
			break;
	}
	dbCountStr = m_DbConn->Exec(SelectQuery, &queryStatus);
	
	dbCount = atoi(dbCountStr.c_str());

	if(0 != queryStatus)
	{
			LOGERR("ReportGeneration : GetDBMinRowID : ErrCode from DB : %d",queryStatus,0,0); 
	}
	return dbCount;
}
/**************************************************************************//**
* \brief   - Get the number of Records
*
* \param   - INT32 RowNumber
*
* \return  - UINT32 - recordCount Total number of Records in Blob Signature 
*
******************************************************************************/
UINT32 ReportGeneration::GetWeldSignatureRecordCount(INT32 RowNumber)
{
	INT8 colIndex=0;
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	string BlobDataBuffer="";
	INT32 queryStatus = 0;
	UINT32 recordCount=0, colPos=0;
	
	snprintf(SelectQuery,sizeof(SelectQuery),"SELECT GraphData FROM WeldResultSignature WHERE ROWID=%d;",RowNumber);
	BlobDataBuffer = m_DbConn->Exec(SelectQuery, &queryStatus);
	
	if(0 != queryStatus)
	{
			LOGERR("ReportGeneration : GetWeldSignatureRecordCount : ErrCode from DB : %d",queryStatus,0,0); 
	}
	else
	{
			/*Logic for Extracting blob data*/
			for(colIndex=0; colIndex<BLOB_PARAMETERS_LEN; colIndex++)
			{
				colPos=BlobDataBuffer.find(DB_DELIMITER, colPos+1);
		
				if((BLOB_PARAMETERS_LEN-1) == colIndex)
				{
					recordCount++;
					colIndex=-1;
				}
				
				if(colPos==string::npos)
				{
					break;
				}
			}
	}
			
	return recordCount;
}

/**************************************************************************//**
* \brief   - Read WeldResult Records from DB.
*
* \param   - None
*
* \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS ReportGeneration::ReadWeldResultFromDBToStorageDevice()
{
	UINT8 readCount = 0;
	INT16 status = OK;
	STATUS fdStatus = OK;
	INT32 queryStatus = 0, recipeQueryStatus = 0, counterQueryStatus = 0, DateTimeQueryStatus = 0, RowIDQueryStatus = 0, recpNum = 0, cycleCount = 0, RowNumber = 0, recipeLastNum = -1, recipeVerLastNum = -1;
	UINT32 totalRecords, recordCount = 0;
	INT32 maxLimitDiff = ERROR;
	UINT32 startPoint = 0, endPoint = 0;
	size_t recipeLastPos = 0, recipeFirstPos = 0, counterFirstPos = 0, counterLastPos = 0, DateTimeFirstPos = 0, DateTimeLastPos = 0, RowIDFirstPos = 0,RowIDLastPos = 0;
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	
	string RecipeNumber = "", CycleCounter = "", strDateTime = "", strRowNum = "", recipeList = "",cycleCountList = "",  DateTimeList = "", RowIDList = "", DBBuffer = "", alarmFlag; 
	
	maxLimitDiff = CheckMaxRecordLimit(TABLE_WELD_RESULT);
	
	if(0 == maxLimitDiff)
	{
		snprintf(SelectQuery,sizeof(SelectQuery),"SELECT RecipeNumber FROM WeldResultTable ORDER BY DateTime ASC;");
	
		recipeList = m_DbConn->Exec(SelectQuery, &recipeQueryStatus);
	
		memset(SelectQuery, 0, sizeof(SelectQuery));
		snprintf(SelectQuery,sizeof(SelectQuery),"SELECT CycleCounter FROM WeldResultTable ORDER BY DateTime ASC;");
		cycleCountList = m_DbConn->Exec(SelectQuery, &counterQueryStatus);
		
		memset(SelectQuery, 0, sizeof(SelectQuery));
		snprintf(SelectQuery,sizeof(SelectQuery),"SELECT DateTime FROM WeldResultTable ORDER BY DateTime ASC;");
		DateTimeList = m_DbConn->Exec(SelectQuery, &DateTimeQueryStatus);
		
		memset(SelectQuery, 0, sizeof(SelectQuery));
		snprintf(SelectQuery,sizeof(SelectQuery),"SELECT ROWID FROM WeldResultTable ORDER BY DateTime ASC;");
		RowIDList = m_DbConn->Exec(SelectQuery, &RowIDQueryStatus);
	}
	else if(maxLimitDiff > 0)
	{
		if(ERROR != GetRowID(TABLE_WELD_RESULT, startPoint, endPoint))
		{
			startPoint = startPoint + maxLimitDiff;
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT RecipeNumber FROM WeldResultTable WHERE ROWID BETWEEN %d AND %d ORDER BY DateTime ASC;",startPoint,endPoint);

			recipeList = m_DbConn->Exec(SelectQuery, &recipeQueryStatus);

			memset(SelectQuery, 0, sizeof(SelectQuery));
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT CycleCounter FROM WeldResultTable WHERE ROWID BETWEEN %d AND %d ORDER BY DateTime ASC;",startPoint,endPoint);
			cycleCountList = m_DbConn->Exec(SelectQuery,&counterQueryStatus);
			
			memset(SelectQuery, 0, sizeof(SelectQuery));
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT DateTime FROM WeldResultTable WHERE ROWID BETWEEN %d AND %d ORDER BY DateTime ASC;", startPoint, endPoint);
			DateTimeList = m_DbConn->Exec(SelectQuery, &DateTimeQueryStatus);
			
			memset(SelectQuery, 0, sizeof(SelectQuery));
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT ROWID FROM WeldResultTable WHERE ROWID BETWEEN %d AND %d ORDER BY DateTime ASC;", startPoint, endPoint);
			RowIDList = m_DbConn->Exec(SelectQuery, &RowIDQueryStatus);
		}
		else
		{
			LOGERR("ReportGeneration-WeldResult : Error on getting start and end row id",0,0,0);
			maxLimitDiff = -2;
		}
	}
	else
	{
		LOGERR("ReportGeneration-WeldResult : MaxRecordLimit status failed with ErrCode : %d",maxLimitDiff,0,0);
	}
	
	if(0 == recipeQueryStatus && 0 == counterQueryStatus && maxLimitDiff >= 0)	
	{
		while(!(counterLastPos == string::npos && recipeLastPos == string::npos && DateTimeLastPos == string::npos && RowIDLastPos == string::npos )) 
		{
			recipeLastPos	= recipeList.find(DB_DELIMITER,recipeLastPos+1);
			counterLastPos = cycleCountList.find(DB_DELIMITER, counterLastPos+1);
			DateTimeLastPos = DateTimeList.find(DB_DELIMITER, DateTimeLastPos+1);
			RowIDLastPos = RowIDList.find(DB_DELIMITER, RowIDLastPos+1);
			
			RecipeNumber = recipeList.substr(recipeFirstPos, recipeLastPos-recipeFirstPos);
			recipeFirstPos	= recipeLastPos+1;
			
			CycleCounter = cycleCountList.substr(counterFirstPos, counterLastPos-counterFirstPos);
			counterFirstPos = counterLastPos+1;
			
			strDateTime = DateTimeList.substr(DateTimeFirstPos, DateTimeLastPos-DateTimeFirstPos);
			DateTimeFirstPos = DateTimeLastPos+1;
			
			strRowNum = RowIDList.substr(RowIDFirstPos, RowIDLastPos-RowIDFirstPos);
			RowIDFirstPos = RowIDLastPos+1;
			
			recpNum = atoi(RecipeNumber.c_str());
			cycleCount = atoi(CycleCounter.c_str());
			RowNumber =  atoi(strRowNum.c_str());
			
			/* Will be used to fetch the alarm ID logs in case of weld resulted in alarm */
			m_currentRecipeNum 	  = recpNum;
			m_currentCycleCounter = cycleCount;
			m_currentDateTime = strDateTime;
			
			if(recipeLastNum != recpNum)
			{
				recipeLastNum = recpNum;

				FillRecipeInformationTable(recipeLastNum , 0);

				FillRecipeWeldMode(recipeLastNum , 0);
			}
			
			memset(SelectQuery, 0, sizeof(SelectQuery));
			switch(m_storageFormat)
			{
				case CSV_FORMAT:
					snprintf(SelectQuery,sizeof(SelectQuery),"SELECT %s FROM WeldResultTable WHERE ROWID=%d;", WELD_RESULT_CSV_PARAM_STRING, RowNumber);
					break;
					
				case PDF_FORMAT:
					snprintf(SelectQuery,sizeof(SelectQuery),"SELECT %s FROM WeldResultTable WHERE ROWID=%d;",WELD_RESULT_PARAM_STRING, RowNumber);
					break;
					
				default:
					LOGERR("ReportGeneration : GetColumnCount: Unknown Report ID Received",0,0,0);
					break;
			}
			
			DBBuffer = m_DbConn->Exec(SelectQuery,&queryStatus);
			
			if(SQLITE_OK == queryStatus)
			{
				/* Check - Is file reached MAX_FILE_SIZE before write next record */
				if(!LimitFileWrite(DBBuffer.length()))
				{
					string reportFile = ReportNameGeneration();
					CSVFormat::CloseFileDescriptor();
					fdStatus = CSVFormat::OpenFileDescriptor(reportFile);

					if(fdStatus != FAILURE)
					{
						/*Inserting Report Header*/
						string ReportHeader = ReadReportHeader();
						ReportHeader.append("\n");
						write(CSVFormat::GetCSVFileDescriptor(), ReportHeader.c_str(), strlen(ReportHeader.c_str()));
					}
				}

				if(fdStatus != FAILURE)
				{
					memset(SelectQuery, 0, sizeof(SelectQuery));
					snprintf(SelectQuery,sizeof(SelectQuery),"SELECT AlarmFlag FROM WeldResultTable WHERE ROWID=%d;",RowNumber);
					alarmFlag = m_DbConn->Exec(SelectQuery,&queryStatus);
					m_alarmFlag = atoi(alarmFlag.c_str());			
					//LOGDBG("ReportGeneration : ReadWeldResultFromDBToStorageDevice : Reading m_alarmFlag=%d",m_alarmFlag,0,0);

					if(SQLITE_OK == queryStatus)
					{
						if(m_deleteID == TRUE && readCount == 0) //storing First Weld Result Recipe # and Cycle Counter and to be used while deleting Weld Results(if Delete Option enable)
						{
							m_FirstRowNum = RowNumber;
							readCount++;							
						}
						status = WriteDBRecordToUSB(DBBuffer);
						DBBuffer.clear();

						if(OK != status)
						{
							break;
						}
					}
					else
					{
						LOGERR("ReportGeneration : ReadWeldResultFromDBToStorageDevice : Alarm flag read error from DB : %d",queryStatus,0,0); 
						status = DB_READ_ERROR;
						break;
					}
				}
				else
				{
					LOGERR("ReportGeneration : ReadWeldResultFromDBToStorageDevice : csvFd open failed",0,0,0);
					status = DATA_NOT_COMPLETE;
					break;
				}
			}
			else
			{
				LOGERR("ReportGeneration : ReadWeldResultFromDBToStorageDevice : Weld result logs read error from DB : %d",queryStatus,0,0); 
				status = DB_READ_ERROR;
				break;
			}
			
		}
		//storing Last Weld Result RowNumber to be used while deleting Weld Results 
		m_LastRowNum = RowNumber;
		LOGDBG("ReportGeneration Done : LastRowNum:%d", m_LastRowNum,0,0);
	}
	else 
	{
		LOGERR("ReportGeneration : ReadWeldResultFromDBToStorageDevice : Record not found : %d %d",recipeQueryStatus,counterQueryStatus,0);  
		status = DB_READ_ERROR;
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Read WeldSignature Records from DB.
*
* \param   - None
*
* \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS ReportGeneration::ReadWeldSignatureFromDBToStorageDevice()
{
	UINT8 readCount = 0;
	INT16 status = OK;
	INT32 queryStatus = 0,RowNumber = 0;
	UINT32 recordsCount=0;
	STATUS fdStatus = OK;
	size_t RowIDFirstPos = 0,RowIDLastPos = 0;
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	string strRowNum = "", RecipeList="", DBBuffer="";
	INT32 maxLimitDiff = ERROR;
	UINT32 startPoint = 0, endPoint = 0;
	
	maxLimitDiff = CheckMaxRecordLimit(TABLE_WELD_SIGNATURE);
	
	if(0 == maxLimitDiff)
	{
		snprintf(SelectQuery,sizeof(SelectQuery),"SELECT ROWID FROM WeldResultSignature ORDER BY ROWID ASC;");
		RecipeList=m_DbConn->Exec(SelectQuery,&queryStatus);
	}
	else if(maxLimitDiff > 0)
	{
		if(ERROR != GetRowID(TABLE_WELD_SIGNATURE, startPoint, endPoint))
		{
			startPoint = startPoint + maxLimitDiff;
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT ROWID FROM WeldResultSignature WHERE ROWID BETWEEN %d AND %d ORDER BY ROWID ASC;",startPoint,endPoint);
			RecipeList=m_DbConn->Exec(SelectQuery, &queryStatus);
		}
		else
		{
			LOGERR("ReportGeneration-WeldSignature : Error on getting start and end row id",0,0,0);
			maxLimitDiff = -2;
		}
	}
	else
	{
		LOGERR("ReportGeneration-WeldSignature : MaxRecordLimit status failed with ErrCode : %d",maxLimitDiff,0,0);
	}
		
	if(0 == queryStatus && maxLimitDiff >= 0)
	{
		while(!(RowIDLastPos==string::npos))
		{
			RowIDLastPos=RecipeList.find(DB_DELIMITER,RowIDFirstPos);
			strRowNum=RecipeList.substr(RowIDFirstPos, RowIDLastPos-RowIDFirstPos);
			RowIDFirstPos=RowIDLastPos+1;

			RowNumber = atoi(strRowNum.c_str());
					
			memset(SelectQuery, 0, sizeof(SelectQuery));
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT RecipeNumber,WeldRecipeVerNumber,CycleCounter,GraphData FROM WeldResultSignature WHERE ROWID=%d;",RowNumber);
			DBBuffer = m_DbConn->Exec(SelectQuery, &queryStatus);
			
			if(0 == queryStatus)
			{
				/* Check - Is file reached MAX_FILE_SIZE before write next record */
				if(!LimitFileWrite(DBBuffer.length()))
				{
					string reportFile = ReportNameGeneration();
					CSVFormat::CloseFileDescriptor();
					fdStatus = CSVFormat::OpenFileDescriptor(reportFile);

					if(fdStatus != FAILURE)
					{
						/*Inserting Report Header*/
						string ReportHeader = ReadReportHeader();
						ReportHeader.append("\n");
						write(CSVFormat::GetCSVFileDescriptor(), ReportHeader.c_str(), strlen(ReportHeader.c_str()));
					}
				}
				
				if(fdStatus != FAILURE)
				{
					if(m_deleteID == TRUE && readCount == 0) //storing First Weld Result Recipe # and Cycle Counter and to be used while deleting Weld Results(if Delete Option enable) 
					{
						m_FirstRowNum = RowNumber;
						readCount++;							
					}
					
					recordsCount = GetWeldSignatureRecordCount(RowNumber);

					status = FormatWeldSignatureResultToRecords(DBBuffer, recordsCount);
					DBBuffer.clear();
					if(OK != status)
					{
						LOGERR("ReportGeneration : ReadWeldSignatureFromDBToStorageDevice : csvFd open failed",0,0,0);
						status = DATA_NOT_COMPLETE;
						break;
					}
				}
				else
				{
					LOGERR("ReportGeneration : ReadWeldSignatureFromDBToStorageDevice : Weld signature read error from DB : %d",queryStatus,0,0); 
					status = DB_READ_ERROR;
					break;
				}
			}
			else
			{
				LOGERR("DB_T : ReadWeldSignatureFromDBToStorageDevice : DB Data Read Error: %d",queryStatus,0,0); 
				status = DB_READ_ERROR;
				break;
			}
			
		}
		//storing Last Weld Result RowNumber Counter to be used while deleting Weld Results 
		m_LastRowNum = RowNumber;
		LOGDBG("ReportGeneration Done : LastRowNum:%d", m_LastRowNum,0,0);
	}
	else 
	{
		LOGERR("ReportGeneration : ReadWeldSignatureFromDBToStorageDevice : Record not found : %d",queryStatus,0,0); 
		status = DB_READ_ERROR;
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Checks the USB available space for WeldSignature Result.
*
* \param   - None
*
* \return  - STATUS 
*
******************************************************************************/
STATUS ReportGeneration::CheckDiskStorageSpace() 
{
	return 0;
}

/**************************************************************************//**
* \brief   - Formats the WeldSignature Result into WeldSignature Records.
*
* \param   - string& DBBuffer
* \param   - UINT32 totalRecords
*
* \return  - STATUS - OK or FAILURE 
*
******************************************************************************/
STATUS ReportGeneration::FormatWeldSignatureResultToRecords(string& DBBuffer, UINT32 totalRecords) 
{	
	INT8 colIndex=0, status=OK;
	INT16 colCount=BLOB_PARAMETERS_LEN;			
	UINT32 recordCount=0;
	std::size_t colPos=0, firstPos=0;
	string RecordBuffer="", RecpieInfoBuffer="";

	if(OK==status)
	{
		/*Extract the Recipe, Recipe version number and Cycle Counter from Weld Signature*/
		for(colIndex=0; colIndex<3; colIndex++) //From DB Query, extracting only Recipe, Recipe Version number and Cycle Counter; hence loop is iterating 3 times
		{
			colPos=DBBuffer.find(DB_DELIMITER, colPos+1);
			
			RecpieInfoBuffer += DBBuffer.substr(firstPos, colPos-firstPos);
			RecpieInfoBuffer.append(",");
			firstPos=colPos+1;
		}
		
		/*Updating index to point to blob data*/
		firstPos=colPos+1;
		
		/*Logic for Extracting blob data*/
		for(colIndex=0; colIndex<colCount; colIndex++)
		{
			colPos=DBBuffer.find(DB_DELIMITER, colPos+1);

			if(colIndex==(colCount-1))
			{
				RecordBuffer = RecpieInfoBuffer;  
				
				RecordBuffer += DBBuffer.substr(firstPos, colPos-firstPos);

				status=WriteDBRecordToUSB(RecordBuffer);

				if( OK != status)
				{
					break;
				}
				else
				{
					recordCount++;
				}

				firstPos=colPos+1;
				RecordBuffer.clear();
				colIndex=-1;
			}

			if(colPos==string::npos)
			{
				break;
			}
		}
	}

	if(totalRecords==recordCount)
	{
		status=OK;
	}

	return status;
}

/**************************************************************************//**
* \brief   - Write the DB Record into USB.
*
* \param   - string& DBRecord
*
* \return  - STATUS 
*
******************************************************************************/
STATUS ReportGeneration::WriteDBRecordToUSB(string& DBRecord)
{
	return 0;
}

/**************************************************************************//**
* \brief   - Delete the WeldSignature records from DB.
*
* \param   - None
*
* \return  - STATUS - OK or FAILURE 
*
******************************************************************************/
STATUS ReportGeneration::DeleteWeldResultFromDB()
{
	INT8 status = OK;
	INT32 queryStatus = 0 ,queryStatus1 = 0, queryStatus2 = 0;
	UINT32 firstRowID = 0, lastRowID = 0;
	char DeleteQuery[MSG_SIZE * 4] = {0x00};
	string FirstRowID="", LastRowID="";

	snprintf(DeleteQuery, sizeof(DeleteQuery),"SELECT ROWID FROM WeldResultTable where ROWID=%d;",m_FirstRowNum);
	FirstRowID = m_DbConn->Exec(DeleteQuery, &queryStatus1);
	firstRowID = atoi(FirstRowID.c_str());
	
	memset(DeleteQuery, 0, sizeof(DeleteQuery));
	snprintf(DeleteQuery,sizeof(DeleteQuery),"SELECT ROWID FROM WeldResultTable where ROWID=%d;",m_LastRowNum);
	LastRowID = m_DbConn->Exec(DeleteQuery,&queryStatus2);
	lastRowID = atoi(LastRowID.c_str());
	
	if((queryStatus1 != 0) and (queryStatus2 != 0))
	{
		LOGERR("ReportGeneration : DeleteWeldResultFromDB : DB Data Read Error: %d %d",queryStatus1,queryStatus2,0);
		status=FAILURE;
	}
	else
	{
		memset(DeleteQuery, 0, sizeof(DeleteQuery));
		snprintf(DeleteQuery,sizeof(DeleteQuery),"DELETE FROM WeldResultTable WHERE ROWID BETWEEN %d and %d;", firstRowID, lastRowID);
		m_DbConn->Exec(DeleteQuery, &queryStatus);
	
		if(0==queryStatus)
		{
			LOGDBG("ReportGeneration : DeleteWeldResultFromDB : Deleted Weld Results from DB Successfully. Last ROWID:%d",m_LastRowNum,0,0);
			DataBaseManager::EnterEventLogDeleteLog (WELD_RESULTS_LOG_ID);

		}
		else
		{
			status=FAILURE;
			LOGERR("ReportGeneration : DeleteWeldResultFromDB : Error in Delete: %d",queryStatus,0,0);
		}
		
	}
	
	return status;

}

/**************************************************************************//**
* \brief   - Delete the WeldSignature records from DB.
*
* \param   - None
*
* \return  - STATUS - OK or FAILURE 
*
******************************************************************************/
STATUS ReportGeneration::DeleteWeldSignatureFromDB()
{
	INT8 status = OK;
	INT32 queryStatus = 0 ,queryStatus1 = 0, queryStatus2 = 0;
	UINT32 firstRowID = 0, lastRowID = 0;
	char DeleteQuery[MSG_SIZE * 4] = {0x00};
	string FirstRowID="", LastRowID="";

	sprintf(DeleteQuery,"SELECT ROWID FROM WeldResultSignature where ROWID=%d;",m_FirstRowNum);
	FirstRowID = m_DbConn->Exec(DeleteQuery, &queryStatus1);
	firstRowID = atoi(FirstRowID.c_str());
	
	memset(DeleteQuery, 0, sizeof(DeleteQuery));
	sprintf(DeleteQuery,"SELECT ROWID FROM WeldResultSignature where ROWID=%d;",m_LastRowNum);
	LastRowID = m_DbConn->Exec(DeleteQuery, &queryStatus2);
	lastRowID = atoi(LastRowID.c_str());
	
	if((queryStatus1 != 0) and (queryStatus2 != 0))
	{
		LOGERR("ReportGeneration : DeleteWeldResultFromDB : DB Data Read Error: %d %d",queryStatus1,queryStatus2,0);
		status=FAILURE;
	}
	else
	{
		memset(DeleteQuery, 0, sizeof(DeleteQuery));
		sprintf(DeleteQuery,"DELETE FROM WeldResultSignature WHERE ROWID BETWEEN %d and %d;", firstRowID, lastRowID);
		m_DbConn->Exec(DeleteQuery, &queryStatus);
	
		if(0==queryStatus)
		{
			LOGDBG("ReportGeneration : DeleteWeldResultFromDB : Deleted Weld Results from DB Successfully. Last ROWID:%d",m_LastRowNum,0,0);
			DataBaseManager::EnterEventLogDeleteLog (WELD_SIGNATURE_LOG_ID);
		}
		else
		{
			status=FAILURE;
			LOGERR("ReportGeneration : DeleteWeldResultFromDB : Error in Delete: %d",queryStatus,0,0);
		}
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Virtual Function to be override in Derived Class
* 			 (for Report Generation)
*
* \param   - UINT8 a_checkMemoryStatusCmd
*
* \return  - STATUS
*
******************************************************************************/
STATUS ReportGeneration::GenerateReport(UINT8 a_checkMemoryStatusCmd )
{
	return 0;
}

/**************************************************************************//**
* \brief   - Virtual Function to be override in Derived Class 
* 			 (for Report file name generation)
*
* \param   - None
*
* \return  - string - Report File Name 
*
******************************************************************************/
string ReportGeneration::ReportNameGeneration()
{
	string reportFile="", timeStamp="";

	switch(m_reportType)
	{
	case WELDRESULT_TYPE:
		reportFile = WELDRESULT_FILEPATH ;
		break;

	case WELDSIGNATURE_TYPE:
		reportFile = WELDSIGNATURE_FILEPATH;
		break;
	
	case ALARM_LOG:
		reportFile = ALARMLOG_FILEPATH;
		break;

	case SYSTEM_SETUP:
		reportFile = SYSTEMSETUP_FILEPATH;
		break;

	case EVENT_LOG:
		reportFile = EVENTLOG_FILEPATH;
		break;

	case USER_ID:
		reportFile = USERID_FILEPATH;
		break;
		
	case RECIPE_SETTING:
		reportFile = RECIPESETTING_FILEPATH;
		break;
		
	default:
		/* Sending back an empty string */
		reportFile.clear();
		break;
	}

	if(0==reportFile.empty())
	{
		/* Timestamp string */
		GetDateTimeStamp(timeStamp);
		reportFile.append(timeStamp);
		
		/* Append the storage format extension */
		switch(m_storageFormat)
		{
		case CSV_FORMAT:
			reportFile.append(".csv");
			break;
		
		case PDF_FORMAT:
			reportFile.append(".pdf");
			break;
		
		default:
			/* Sending back an empty string */
			reportFile.clear();
			break;
		}
	}

	return reportFile ;
}

/**************************************************************************//**
* \brief   - Function to split the string based on separator
*
* \param   - string& a_str
* \param   - vector<string>& a_vecData
* \param   - UINT8 a_separator
*
* \return  - STATUS
*
******************************************************************************/
STATUS ReportGeneration::SplittingData(string& a_str , vector<string>& a_vecData, UINT8 a_separator) 
{
	string wordStr= "";
	UINT32 index = 0 ;

	for(index = 0 ; index <= a_str.length() ; index++)
	{
		if((a_str[index] == a_separator) || a_str[index] == '\0')
		{
			a_vecData.push_back(wordStr);
			wordStr.clear();
		}
		else
		{
			wordStr += a_str[index] ;
		}
	}

	return 0 ;
}

/**************************************************************************//**
* \brief   - Function to convert string to lower case
*
* \param   - string a_stringData
*
* \return  - STATUS
*
*******************************************************************************/
string ReportGeneration::ConvertStringToLower(string a_stringData)
{
	string retString="" ;
	UINT8 index = 0 ;

	retString.resize(a_stringData.length());

	for(index = 0 ; index < a_stringData.length(); index++)
	{
		retString[index] = tolower(a_stringData[index]);
	}

	return retString;
}

/**************************************************************************//**
* \brief   - Cross verifies the max record limit for the table.
*
* \param   - string a_tableName
*
* \return  - INT32 count
*
******************************************************************************/
INT32 ReportGeneration::CheckMaxRecordLimit(std::string a_tableName)
{
	string selectQry 	= "";
	string result 		= "";
	INT32  queryStatus 	= 0;
	INT32  count 		= 0;
	UINT32 limit		= 0;
	
	if(0 == a_tableName.compare(TABLE_WELD_RESULT))
		limit = DB_RECORDS_STORAGE_WELD_RESULT_LIMIT;
	else if(0 == a_tableName.compare(TABLE_WELD_SIGNATURE))
		limit = DB_RECORDS_STORAGE_WELD_SIGNATURE_LIMIT;
	
	selectQry = "SELECT COUNT(1) FROM " + a_tableName + ";";
	result = m_DbConn->Exec((char *)selectQry.c_str(), &queryStatus);
		
	if(SQLITE_OK == queryStatus)
	{
		count = atoi(result.c_str());
		if(count > limit)
		{
			count = count - limit;
		}
		else
		{
			count = 0;
		}
	}
	else
	{
		LOGERR("ReportGeneration : Failed to get total count from table with an ErrCode : %d",queryStatus,0,0);
		count = -1;
	}
	
	return count;
}

/**************************************************************************//**
* \brief   	- Retrieves first and last row id from the table.
*
* \param   	- string  a_TableName (IN)
* \param	- UINT32& a_startID (OUT)
* \param	- UINT32& a_endID (OUT)
*
* \return  	- INT32 queryStatus
*
******************************************************************************/
INT32 ReportGeneration::GetRowID(std::string a_tableName, UINT32& a_startID, UINT32& endID)
{
	string selectQry 	= "";
	string result 		= "";
	INT32  queryStatus 	= 0;
	
	selectQry = "SELECT ROWID FROM " + a_tableName + " ORDER BY ROWID ASC LIMIT 1;";
	result = m_DbConn->Exec((char *)selectQry.c_str(), &queryStatus);
	
	if(SQLITE_OK == queryStatus)
	{
		a_startID = atoi(result.c_str());
		
		selectQry.clear();
		selectQry = "SELECT ROWID FROM " + a_tableName + " ORDER BY ROWID DESC LIMIT 1;";
		result = m_DbConn->Exec((char *)selectQry.c_str(), &queryStatus);
		
		if(SQLITE_OK == queryStatus)
		{
			endID = atoi(result.c_str());
		}
		else
		{
			queryStatus = ERROR;
			LOGERR("ReportGeneration : Failed to get ending row id with an ErrCode : %d",queryStatus,0,0);
		}
	}
	else
	{
		queryStatus = ERROR;
		LOGERR("ReportGeneration : Failed to get starting row id with an ErrCode : %d",queryStatus,0,0);
	}
	
	return queryStatus;
}

/**************************************************************************//**
* \brief   - Virtual Function to be override in PDF Derived Class
* 			  (for Weld Recipe table information)
*
* \param   - INT32 a_recipeNum
* \param   - INT32 a_recipeVerNum
*
* \return  - STATUS
*
******************************************************************************/
STATUS ReportGeneration::FillRecipeInformationTable(INT32 a_recipeNum , INT32 a_recipeVerNum)
{
	return 0;
}

/**************************************************************************//**
* \brief   - Virtual Function to be override in PDF Derived Class
* 			  (for Weld Recipe mode information)
*
* \param   - INT32 a_recipeNum
* \param   - INT32 a_recipeVerNum
*
* \return  - STATUS
*
******************************************************************************/
STATUS ReportGeneration::FillRecipeWeldMode(INT32 a_recipeNum , INT32 a_recipeVerNum)
{
	return 0;
}


/**************************************************************************//**
* \brief   - Validates data size and restricts if already target size achieved.
*
*
* \param   - UINT32 a_sizeInBytes
*
* \return  - bool - status
*
******************************************************************************/
bool ReportGeneration::LimitFileWrite(UINT32 a_sizeInBytes)
{
	bool status = true; 
	
	m_UsedSizeInBytes = m_UsedSizeInBytes + a_sizeInBytes;
 
	if(m_UsedSizeInBytes > MAX_FILE_SIZE)           /* 4 GB - 512 MB */
	{
	cout << "File size limit crossed : " << m_UsedSizeInBytes << endl;
		status = false;
		ResetFileLimitBytes();
	}
	return status;
}

/**************************************************************************//** 
* \brief   - Resets previous calculated bytes.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void ReportGeneration::ResetFileLimitBytes()
{
	m_UsedSizeInBytes = 0;
}

/**************************************************************************//**
* \brief   - Posting the Delete Response message to destination message queue.
*
* \param   - None
* 
* \return  - STATUS - OK or ERROR
*
******************************************************************************/
STATUS ReportGeneration::PostDeleteResponse()
{
	STATUS ret = OK;
	UINT8 reportID = 0;	
	Message message = {0x00};
	DeleteReport delReport = {0x00};
	
	if(WELDRESULT_TYPE == m_reportType)
	{		
		delReport.reportID = WELD_REP;
		delReport.status = 0;
	}
	message.msgID = TO_UI_TASK_REPORT_DELETE_RSP;
	memcpy(message.Buffer, &delReport, sizeof(DeleteReport));
	
	if(msgQSend(UI_MSG_Q_ID, reinterpret_cast<char*>(&message), sizeof(message), NO_WAIT, MSG_PRI_NORMAL) != OK)
	{
		LOGERR("ReportGeneration : CSVFormat : -----------Error on msgQSend----------",0,0,0);
		ret = ERROR;
	}
	return ret;
}

/**************************************************************************//** 
* \brief   - Find and replaces the given string.
*
* \param   - string& a_src 		- Source.
* \param   - string a_find 		- Data needs to be find from source.
* \param   - string a_replace 	- Data needs to be replaced in source.
*
* \return  - STATUS (OK or ERROR).
*
******************************************************************************/
STATUS ReportGeneration::Replace(std::string& a_src, std::string a_find, std::string a_replace)
{
	STATUS ret	= ERROR;
	INT16  pos 	= 0;
	UINT16 it	= 0;
	
	if(((a_src.length()) > (a_replace.length())) && (!a_find.empty()))
	{
		do
		{
			pos = a_src.find(a_find,pos);
			if((pos > ERROR) && (pos < a_src.length()))
			{
				a_src.replace(pos,a_find.length(),a_replace);
				it 	= it + 1;
				ret = OK;
			}
			else
			{
				if(0 == it)		/* First attempt itself failed to find the data */
				{	
					LOGERR("Report : Data not found in given source",0,0,0);
				}	
			}
		}while((pos > ERROR) && (pos < a_src.length()));
	}
	else
	{
		LOGERR("Report : Invalid source/find data passed",0,0,0);
	}
	
	return ret;
}

/**************************************************************************//**
* \brief   - Fetches alarm IDs that are associated with particular cycle counter and recipe number.
* 			 If more than one rows match the particular cycle counter and recipe number, 
* 			 then uses DateTime value to identify correct alarm IDs.
*
* \param   - UINT32 recipeNumber 	- Recipe number used during weld.
* \param   - UINT32 cycleCounter 	- Weld cycle counter.  
* \param   - UINT32 cycleCounter 	- Weld Date Time. 
*
* \return  - string - alarm ids separated by ALARM_ID_DELIMITER
*
******************************************************************************/
std::string	ReportGeneration::GetAlarmIDForWeldResult(UINT32 recipeNumber, UINT32 cycleCounter, string strDateTime)
{
	//todo: combine this with CSVFormat::AppendAlarmIDWithWeldResult to avoid duplication of code
	char selectQry[MSG_SIZE * 4] = {0x00};
	string alarmID = "";
	INT32 AlarmRowCount = 0;
	string strAlarmRowCount = "";
	INT32 ErrCode = ERROR;
	bool bErrorOccured = false;
	
	snprintf(selectQry,sizeof(selectQry), "SELECT count(*) FROM AlarmLogs WHERE RecipeNumber = %d and CycleCounter = %d;",recipeNumber,cycleCounter);
	strAlarmRowCount = m_DbConn->Exec(selectQry, &ErrCode);
	
	if(SQLITE_OK == ErrCode)
	{
		AlarmRowCount = atoi(strAlarmRowCount.c_str());
				
		if(AlarmRowCount == 1)
		{		
			snprintf(selectQry,sizeof(selectQry),"SELECT AlarmID FROM AlarmLogs WHERE RecipeNumber = %d and CycleCounter = %d;",recipeNumber,cycleCounter);
		}
		else if (AlarmRowCount > 1)
		{
			time_t EpochTimeForCurrentDateTime = DateTimeToEpoch(strDateTime);
			string strDateTimeStart = EpochToDateTime(EpochTimeForCurrentDateTime - DATE_TIME_SECONDS_WINDOW);
			string strDateTimeEnd = EpochToDateTime(EpochTimeForCurrentDateTime + DATE_TIME_SECONDS_WINDOW);
					
			snprintf(selectQry,sizeof(selectQry),"SELECT AlarmID FROM AlarmLogs WHERE RecipeNumber = %d and CycleCounter = %d AND DateTime BETWEEN '%s' AND '%s';",recipeNumber,cycleCounter, strDateTimeStart.c_str(), strDateTimeEnd.c_str());					
		}
		else
		{
			ErrCode = ERROR;
			LOGERR("ReportGeneration : 0 AlarmID found to add it in weld result",0,0,0);	
			bErrorOccured = true;
		}
		
		if(bErrorOccured == false)
		{
			alarmID = m_DbConn->Exec(selectQry,&ErrCode);
		}
	}
	else
	{
		LOGERR("ReportGeneration : AlarmID reading failed with ErrCode : %d",ErrCode,0,0);
	}
	return alarmID;
}

/**************************************************************************//**
* \brief   - Returns the data member´s m_reportGenerationFlag value to see 
* if the generation of the report (PDF or CSV) is completed.
*
* \param   - None
*
* \return  - true (completed) or false (not completed)
*
******************************************************************************/
bool ReportGeneration::GetReportGenerationFlag()
{
	return m_reportGenerationFlag;
}

/**************************************************************************//**

* \brief   - Converts passed DateTime string to EpochTime 

*
* \param   - DateTime string
*
* \return  - EpochTime for passed DateTime string
*
******************************************************************************/
time_t ReportGeneration::DateTimeToEpoch(std::string strDateTime)
{
	time_t EpochTime = 0;
	struct tm SysTime = {0x00};
	
	sscanf(strDateTime.c_str(),"%d/%d/%d %d:%d:%d",&SysTime.tm_year,&SysTime.tm_mon,&SysTime.tm_mday,&SysTime.tm_hour,&SysTime.tm_min,&SysTime.tm_sec);
	SysTime.tm_year = SysTime.tm_year-1900;
	SysTime.tm_mon = SysTime.tm_mon-1;// Month, where 0 = jan
	SysTime.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
	
	EpochTime = mktime(&SysTime);
	
	return EpochTime;
}

/**************************************************************************//**
* \brief   - Converts passed EpochTime to DateTime string  

*
* \param   - EpochTime
*
* \return  - DateTime string for passed EpochTime
*
******************************************************************************/
std::string ReportGeneration::EpochToDateTime(time_t EpochTime)
{
	string strDateTime = "";
	struct tm  SysTime = {0x00};
    char szDateTime[100] = {0x00};

    SysTime = *localtime(&EpochTime);
    strftime(szDateTime, sizeof(szDateTime), "%Y/%m/%d %H:%M:%S", &SysTime);
	strDateTime = szDateTime;
	
	return strDateTime;
}


/**************************************************************************//**
* \brief   - Parameterized Constructor - Creates a File Descriptor 
* 			 for WeldData/Weld Signature 
*
* \param   - Report_Message &a_reportMsg
* \param   - SQLiteDB* a_dbPtr
*
* \return  - None
*
******************************************************************************/
CSVFormat::CSVFormat(Report_Message &a_reportMsg): ReportGeneration(a_reportMsg)
{
	
}

/**************************************************************************//**
* \brief   - Default Constructor
* 
* \param   - None 
*
* \return  - None
*
******************************************************************************/
CSVFormat::CSVFormat()
{

}

/**************************************************************************//**
* \brief   - Destructor - Closes the Opened File Descriptor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
CSVFormat::~CSVFormat()
{
	
}

/**************************************************************************//**
* \brief   - Opens new file descriptor for file slicing
*
* \param   - string& a_reportFile
*
* \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS CSVFormat::OpenFileDescriptor(string& a_reportFile)
{
	STATUS status = OK;
	if(1 == a_reportFile.empty())
	{
		LOGERR("ReportGeneration : CSVFormat: Error in Retrieving file name",0,0,0);
		status = FAILURE;
	}
	else
	{
		/*Creating a CSV File*/
		csvFd=open(a_reportFile.c_str(),  O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if(csvFd<0)
		{
			LOGERR("ReportGeneration : CSVFormat: Error in Creating Report File: FD:%d, File:%s",csvFd,(_Vx_usr_arg_t)a_reportFile.c_str(),0);
			status = FAILURE;
		}
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - closes file descriptor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void CSVFormat::CloseFileDescriptor()
{
	close(csvFd);
	csvFd = -1;
}
 
/**************************************************************************//**
* \brief   - Gets file descriptor which is being currently opened
*
* \param   - None
*
* \return  - INT16 - csvFd
*
******************************************************************************/
INT16 CSVFormat::GetCSVFileDescriptor()
{
	return csvFd;
}

/**************************************************************************//**
* \brief   - Extracts the data from DB 
*
* \param   - UINT8 checkMemoryCmd
*
* \return  - STATUS - SUCCESS or FAILURE
*
******************************************************************************/
STATUS CSVFormat::GenerateReport(UINT8 checkMemoryCmd)
{
	INT16 status = OK, delStatus1 = OK, delStatus2 = OK;
	STATUS fdStatus = OK;
	string ReportHeader = "";
	string reportFile;
	
	if((SC::stateMachine.GetStateType() == READY) || (SC::stateMachine.GetStateType() == SC_ALARM))
	{
		m_reportGenerationFlag = false;
	
		/*Connecting DB handler*/
		ReportGeneration::ConnectDB();
		
		/*Checking USB device whether it is connected or not*/
		status = CheckStorageDevice();
		
		if(FAILURE == status)
		{
			status = USB_STORAGE_NOT_PRESENT;
			LOGERR("ReportGeneration : GenerateCSVFile: USB Device not found",0,0,0);
		}
		else if(USB_MOUNT_ERROR == status)
		{
			LOGERR("ReportGeneration : GenerateCSVFile: USB Mount Error",0,0,0);
		}
		else
		{
			if(INITIAL_MEMORY_CHECK == checkMemoryCmd)
			{
				/*Checking free space in USB*/
				status=CheckDiskStorageSpace();
				
				if(USB_INSUFFICIENT_MEMORY == status)
				{
					m_reportReqData.reportID = m_reportType;
					m_reportReqData.storageFormat = m_storageFormat ;
					m_reportReqData.deleteID = m_deleteID;
					memcpy(m_reportReqData.reportBuff , m_storageDestination , strlen((const char*)m_storageDestination));
				}
			}
			
			if((OK == status) || (MEMORY_CHECK_DONE == checkMemoryCmd))
			{
				if(!CheckDBEmpty())
				{
					/* String for ReportName Generation */
					reportFile = ReportNameGeneration();
					fdStatus   = OpenFileDescriptor(reportFile);			/* Opens file descriptor for CSV */
					
					if(fdStatus != FAILURE)
					{
						/*Inserting Report Header*/
						ReportHeader = ReadReportHeader();
												
						ReportHeader.append("\n");
						write(csvFd, ReportHeader.c_str(), strlen(ReportHeader.c_str()));
						switch(m_reportType)
						{
							case WELDRESULT_TYPE:
								/*Extracting WeldSignature Records from DB into USB*/
								status = ReadWeldResultFromDBToStorageDevice();
								if(OK == status)
								{
									status = REPORT_GENERATE_SUCCESS;
								/*Deleting WeldSignature Records From DB*/
									if(TRUE == m_deleteID)
									{
										delStatus1 = DeleteWeldResultFromDB();
										if(OK == delStatus1)
										{
											PostDeleteResponse();
											m_DbConn->DBVacuum();
										}
									}
								}			
								break;
		
							case WELDSIGNATURE_TYPE:
								/*Extracting WeldSignature Records from DB into USB*/
								status = ReadWeldSignatureFromDBToStorageDevice();
								if(OK == status)
								{
									status = REPORT_GENERATE_SUCCESS;
	
									/*Deleting WeldSignature Records From DB*/
									if(TRUE == m_deleteID)
									{
										delStatus2 = DeleteWeldSignatureFromDB();
										if(OK == delStatus2)
										{
											m_DbConn->DBVacuum();
										}	
									}
								}
								break;
						}
			
						LOGDBG("ReportGeneration : GenerateCSVFile: Reported is generated for %u Records",RecordCount,0,0);
						RecordCount = 0;	
					}			
					else
					{
						LOGERR("ReportGeneration : GenerateCSVFile: REPORT_GENERATION_FAILED - fdStatus:%d",fdStatus,0,0);
					}
				
				}
				else
				{
					status = DB_EMPTY;
					LOGERR("ReportGeneration : GenerateCSVFile: DB Empty - ",0,0,0);
				}
			}
		
		}
		/*closing all file descriptors*/
		CSVFormat::CloseFileDescriptor();
		CSVFormat::CloseDirectoryFileDescriptor();
		ReportGeneration::CloseStorageDevice();
		
		/*Closing the DB Connection*/
		ReportGeneration::CloseDB();
		m_reportGenerationFlag = true;
	}
	else
	{
		status = SC_INVALID_STATE;
		LOGERR("ERROR: SC_INVALID_STATE\n",0,0,0);
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Checks the USB available space for WeldSignature Result.
*
* \param   - UINT32 totalRecordCount
*
* \return  - STATUS 
*
******************************************************************************/
STATUS CSVFormat::CheckDiskStorageSpace()
{
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	string recordCount="";
	INT8 status=OK;
	INT16 colCount=0;
	INT32 queryStatus = 0;
	UINT64 freeSpace=0, recordSize=0, totalRecordCount=0;
	struct statfs fileInf={0x00};
			
	if(m_storageFd<0)
	{
		LOGERR("ReportGeneration : CheckDiskStorageSpace: Error in checking the disk size",0,0,0);
		status=USB_STORAGE_NOT_PRESENT;
	}
	else
	{
		switch(m_reportType)
		{
		case WELDRESULT_TYPE:
			colCount=GetColumnCount();		
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT Count(1) FROM WeldResultTable");
			recordCount = m_DbConn->Exec(SelectQuery, &queryStatus);
					
			if(0 != queryStatus)
			{
				LOGERR("ReportGeneration : CheckDiskStorageSpace : ErrCode from DB : %d",queryStatus,0,0);  
			}
			
			totalRecordCount=atoi(recordCount.c_str());
			recordSize=  (UINT64)DB_COLUMN_SIZE  * (UINT64)colCount * totalRecordCount;
			break;
			
		case WELDSIGNATURE_TYPE:
			colCount = GetColumnCount();
			colCount = colCount -1; //Reduce by 1 as we are not using "RecordNumber" column of WeldResultSignature Table
			colCount += (BLOB_PARAMETERS_LEN-1);
			snprintf(SelectQuery,sizeof(SelectQuery),"SELECT Count(1) FROM WeldResultSignature");
			recordCount = m_DbConn->Exec(SelectQuery, &queryStatus);
					
			if(queryStatus != 0)
			{
				LOGERR("ReportGeneration : CheckDiskStorageSpace : ErrCode from DB : %d",queryStatus,0,0); 
			}
			
			totalRecordCount=atoi(recordCount.c_str());
			
			recordSize=  (UINT64)DB_COLUMN_SIZE  * (UINT64)colCount * totalRecordCount;
			break;
		}
		
		if(OK == statfs(USB_DEVICEPATH, &fileInf))
		{
			freeSpace= (UINT64)fileInf.f_bsize * (UINT64)fileInf.f_bavail;
		}
		
	
		if(recordSize > freeSpace)
		{
			status=USB_INSUFFICIENT_MEMORY;
		}
	}
	
	return status;
}

/**************************************************************************//**
* \brief   - Write the DB Record into USB.
*
* \param   - string& DBRecord
*
* \return  - STATUS - OK or FAILURE 
*
******************************************************************************/
STATUS CSVFormat::WriteDBRecordToUSB(string& DBRecord)
{
	char ch;
	INT8 status = FAILURE;
	std::size_t len,ret, count;
	string headerStr = "";
	float tmpConvert = 0.0;
	string contentStr = "";
	int index=0,num=0;
	vector<string> vecDbStr;
	string DBRecordNewStr = "";/*New DBrecord string*/
	intf_SysConfig_t SysConfigInfo;

	if(csvFd < 0)
	{
		LOGERR("ReportGeneration : WriteDBRecordToUSB: Error in opening weld data file",0,0,0);
	}
	else
	{
		/* Fetch alarm IDs that are associated with current record's recipe number and cycle counter */
		if(TRUE == m_alarmFlag)
		{
			// TODO : Need to handle the FAILURE status when alarm IDs fetch fails 
			AppendAlarmIDWithWeldResult(DBRecord);
		}
		
			
		if(m_reportType == WELDRESULT_TYPE)
		{
			headerStr = DBRecord.c_str() ;		
			/*Get the Comma Seperated string*/
			stringstream ssDbStr(headerStr);
			while (ssDbStr.good())
			{
				string substr;
				getline(ssDbStr, substr, ',');
				vecDbStr.push_back(substr);
			}
			/*To access system unit*/
			CP->GetSysConfContext(SysConfigInfo);
			/* Convert the Units for Weld Data */
			for(index=0 ; index < vecDbStr.size();index++)
			{
				if(index == MAX_WELD_FORCE  || index == END_HOLD_FORCE || index == WELD_ABSOLUTE ||  index == TOTAL_ABSOLUTE || index == TOTAL_COLLAPSE_DISTANCE 
				|| index == WELD_COLLAPSE_DISTANCE || index == HOLD_COLLAPSE_DISTANCE || index == TOTAL_COLLAPSE_DISTANCE
				|| index == TRIGGER_DISTANCE || index == WELD_TIME
				|| index == WELD_ENERGY || index == CYCLE_TIME || index == TRIGGER_POINT)
				{
					num = atoi(vecDbStr[index].c_str());
					if(index == WELD_ENERGY)
					{
						stringstream stream;
						tmpConvert = num /1000.0;
						/*Set Precision Upto 1 digit*/
						stream.precision(1);
						stream <<fixed;
						stream << tmpConvert;
						vecDbStr[index] = stream.str();
						
					}
					else if(index == MAX_WELD_FORCE || index == END_HOLD_FORCE)
					{
						tmpConvert = num /10.0;
						/* Max weld force and End Hold Force conversion For imperial unit */
						if(SysConfigInfo.UnitIs == IMPERIAL)
						{
							tmpConvert = Util->convertNtolbs(tmpConvert);
							stringstream stream;
							stream.precision(1);
							stream <<fixed;
							stream << tmpConvert;
							vecDbStr[index] = stream.str();
						}
						/* Max weld force and End Hold Force conversion For Metric unit */
						else
						{
							int newtonValue = (int)round(tmpConvert);
							stringstream stream;
							stream << newtonValue;
							vecDbStr[index] = stream.str();
						}
					}
					else
					{
						tmpConvert = num /1000.0;
						if((index == WELD_ABSOLUTE ||  index == TOTAL_ABSOLUTE || index == TOTAL_COLLAPSE_DISTANCE || index == WELD_COLLAPSE_DISTANCE 
								|| index == HOLD_COLLAPSE_DISTANCE || index == TRIGGER_DISTANCE ) && (!strcmp(SystemConfiguration::systemTypeName,"ASX")))
						{
							/*Set Precision Upto 2 digit*/
							stringstream stream;
							stream.precision(2);
							stream <<fixed;
							stream << tmpConvert;
							vecDbStr[index] = stream.str();
						}
						else
						{
							/* All Weld Force parameter conversion For imperial unit */
							if((SysConfigInfo.UnitIs == IMPERIAL) && ((index != WELD_TIME) && (index != CYCLE_TIME) && (index != TRIGGER_POINT)))
							{
								stringstream stream;
								tmpConvert = Util->convertmillimetretoinches(tmpConvert);
								/*Set Precision Upto 4 digit*/
							    stream.precision(4);
							    stream <<fixed;
							    stream << tmpConvert;
							    vecDbStr[index] = stream.str();
							}
							else
							{
								/*Set Precision Upto 3 digit*/
							    stringstream stream;
							    stream.precision(3);
							    stream <<fixed;
							    stream << tmpConvert;
							    vecDbStr[index] = stream.str();
							}
						}
					}
				}
			}
			/*Push Back The Converted string */
			for(size_t i = 0; i < vecDbStr.size(); i++)
			{
				DBRecordNewStr += vecDbStr[i] + ',';
			}
			/*Copy the New modified string to original string*/
			/* Appending the return character here for the CSV write */
			DBRecordNewStr.append("\n");
			DBRecord = DBRecordNewStr.c_str();
			len = strlen(DBRecord.c_str());/*Update New Length*/
			
		}
		else
		{
			/* Appending the return character here for the CSV write */
			DBRecord.append("\n");
			len = strlen(DBRecord.c_str());
		}

		ret = write(csvFd, DBRecord.c_str(), len);
		
		DBRecord.clear();

		if(ret == len)
		{
			RecordCount++;
			status = OK;
		}
		else if(ENOSPC == ret)
		{
			status = USB_STORAGE_FULL;
		}
		else if(ret < len) /* Logic to delete the partial DB Record */
		{
			lseek(csvFd, -ret, SEEK_CUR);
			for(count = 0; count < ret; count++)
			{
				read(csvFd, &ch, sizeof(ch));
				ch = CLEAR_RECORD_DATA;
				lseek(csvFd, -1, SEEK_CUR);
				write(csvFd, &ch, sizeof(ch));						
			}
			status = FAILURE;
		}
	}
	
	return status;
	
}

/**************************************************************************//**
* \brief   - Fetches alarm IDs that are associated with particular cycle counter
* 			  and recipe number then appends with the weld result.
			  If more than one rows match the particular cycle counter
* 			  and recipe number, then uses DateTime value to identify correct
			  alarm IDs.	
*
* \param   - string& weldResult		- Weld result's complete one record.
*
* \return  - STATUS - OK or ERROR 
*
******************************************************************************/
STATUS CSVFormat::AppendAlarmIDWithWeldResult(string& weldResult)
{
	char selectQry[MSG_SIZE * 4] = {0x00};
	string alarmID = "";
	INT32 AlarmRowCount = 0;
	string strAlarmRowCount = "";
	INT32 ErrCode = ERROR;
	bool bErrorOccured = false;
	
	snprintf(selectQry,sizeof(selectQry), "SELECT count(*) FROM AlarmLogs WHERE RecipeNumber = %d and CycleCounter = %d;",m_currentRecipeNum,m_currentCycleCounter);
	strAlarmRowCount = m_DbConn->Exec(selectQry, &ErrCode);
	
	if(SQLITE_OK == ErrCode)
	{	
		AlarmRowCount = atoi(strAlarmRowCount.c_str());
		
		if(AlarmRowCount == 1)
		{		
			snprintf(selectQry,sizeof(selectQry),"SELECT AlarmID FROM AlarmLogs WHERE RecipeNumber = %d and CycleCounter = %d;",m_currentRecipeNum,m_currentCycleCounter);
		}
		else if (AlarmRowCount > 1)
		{
			time_t EpochTimeForCurrentDateTime = DateTimeToEpoch(m_currentDateTime);
			string strDateTimeStart = EpochToDateTime(EpochTimeForCurrentDateTime - DATE_TIME_SECONDS_WINDOW);
			string strDateTimeEnd = EpochToDateTime(EpochTimeForCurrentDateTime + DATE_TIME_SECONDS_WINDOW);
			
			snprintf(selectQry,sizeof(selectQry),"SELECT AlarmID FROM AlarmLogs WHERE RecipeNumber = %d and CycleCounter = %d AND DateTime BETWEEN '%s' AND '%s';",m_currentRecipeNum,m_currentCycleCounter, strDateTimeStart.c_str(), strDateTimeEnd.c_str());					
		}
		else
		{
			ErrCode = ERROR;
			LOGERR("ReportGeneration : 0 AlarmID found to add it in weld result",0,0,0);	
			bErrorOccured = true;
		}

		if(bErrorOccured == false)
		{
			alarmID = m_DbConn->Exec(selectQry, &ErrCode);
			
			if(SQLITE_OK == ErrCode)
			{
				if(!weldResult.empty())
				{
					weldResult.erase(weldResult.size() - 1);	/* Remove AlarmFlag data (1) and replace with AlarmID */
						
					if(!alarmID.empty())
					{
						alarmID.append(DB_DELIMITER);
						ErrCode = Replace(alarmID, DB_DELIMITER, ALARM_ID_DELIMITER);
							
						/* List of associated alarm IDs will be appended in weld result by semicolon(;) separated */
						if(OK == ErrCode)
						{
							alarmID.erase(alarmID.size() - 1);	/* To remove semicolon added at last */
							weldResult.append(alarmID);
						}
						else
						{
							LOGERR("ReportGeneration : Replace AlarmID failed",0,0,0);
						}
					}
					else
					{
						ErrCode = ERROR;
						LOGERR("ReportGeneration : No AlarmID found to add it in weld result",0,0,0);
					}
				}
				else
				{
					ErrCode = ERROR;
					LOGERR("ReportGeneration : Invalid weld result record received",0,0,0);
				}
			}
			else
			{
				LOGERR("ReportGeneration : AlarmID reading failed with ErrCode : %d",ErrCode,0,0);
			}
		}
	}
	else
	{
		LOGERR("ReportGeneration : AlarmID reading failed with ErrCode : %d",ErrCode,0,0);
	}
	
	return ErrCode;
}
/* Pdf library libharu */

void PDFReportGeneration::error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
	LOGERR ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,(HPDF_UINT)detail_no, 0);
	HPDF_Doc _obj_PDF = ((PDFReportGeneration*)user_data)->pdf;
	if(!_obj_PDF)
	{
		HPDF_Free(_obj_PDF);
		((PDFReportGeneration*)user_data)->CloseDirectoryFileDescriptor();
		((PDFReportGeneration*)user_data)->CloseStorageDevice();
		((PDFReportGeneration*)user_data)->CloseDB();
	}
		
}

/**************************************************************************//**
 * \brief   - Default Constructor
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
PDFReportGeneration::PDFReportGeneration()
{
}

PDFReportGeneration::PDFReportGeneration(Report_Message &a_reportMsg, SQLiteDB* a_dbPtr) 
	: ReportGeneration(a_reportMsg)
{
	 LOGERR("\nParametrized constructor of PDF Report Gen is called\n",0,0,0);
	 
	 XCord = PRINTWELDSTARTXCORD ;
	 YCoordinate = TWOWELDLINEGAP;
	 Unit_Heading_Gap = 0;
	
	 totalFileNumber = 1;
	 PageCount = 1;
	 minRowID = 0;
	 
	 UserIdStrHdr = "User ID";
	 PresetStrHdr = "Preset #";
	 PresetRevStrHdr = "Preset rev:N/A";
	
	 R = 0.0;
	 G = 0.0;
	 B = 0.0;
		 
	 PDFParamWidthWeld[0]  = 40;
	 PDFParamWidthWeld[1]  = 83;  //Date Time
	 PDFParamWidthWeld[2]  = 40;
	 PDFParamWidthWeld[3]  = 37;
	 PDFParamWidthWeld[4]  = 37;
	 PDFParamWidthWeld[5]  = 45;
	 PDFParamWidthWeld[6]  = 37;
	 PDFParamWidthWeld[7]  = 37;
	 PDFParamWidthWeld[8]  = 41;
	 PDFParamWidthWeld[9]  = 41;
	 PDFParamWidthWeld[10]  = 41;
	 PDFParamWidthWeld[11]  = 41;
	 PDFParamWidthWeld[12]  = 41;
	 PDFParamWidthWeld[13]  = 41;
	 PDFParamWidthWeld[14]  = 41;
	 PDFParamWidthWeld[15]  = 37;
	 PDFParamWidthWeld[16]  = 37;
	 PDFParamWidthWeld[17]  = 45;
	 PDFParamWidthWeld[18]  = 45;
	 PDFParamWidthWeld[19]  = 45;
	 PDFParamWidthWeld[20]  = 37;
	 PDFParamWidthWeld[21]  = 45;  
	 PDFParamWidthWeld[22]  = 37;
	 PDFParamWidthWeld[23]  = 37;
	 PDFParamWidthWeld[24]  = 50; //Part-ID
	 
	 PDFParamWidthEvent[0] = 65;   // Line Gap for EventLog
	 PDFParamWidthEvent[1] = 170;
	 PDFParamWidthEvent[2] = 90;
	 PDFParamWidthEvent[3] = 55;
	 PDFParamWidthEvent[4] = 70;
	 PDFParamWidthEvent[5] = 70;		 
	 PDFParamWidthEvent[6] = 75;
	 PDFParamWidthEvent[7] = 70;
	 PDFParamWidthEvent[8] = 140;
	 PDFParamWidthEvent[9] = 70;
	 PDFParamWidthEvent[10] = 70;
	 PDFParamWidthEvent[11] = 0;
	 
	 ValidStr ="Non Validated";
	
	 UnitHeadingPos = false;
}

PDFReportGeneration::~PDFReportGeneration()
{
}

/***************************************************************************
* \brief
*   This function draws text in PDF according to the passed parameters.
*   The origin of the PDF coordinate system (0, 0) represents the bottom-left
*   corner of the PDF page.So page_height - (text y coordinate) will position
*   text correctly in PDF.
* \param   - None
*   HPDF_Page *page: pointer to HPDF_Page page
*   int X_Cord: X_Cord
*   int Y_Cord: Y_Cord
*   char *STR: string to be drawn
* \return
*   none
***************************************************************************/
void PDFReportGeneration::DrawPDFText(HPDF_Page *page,int X_Cord,int Y_Cord,char *STR)
{
   HPDF_Page_SetRGBFill(*page, R,G,B);
   HPDF_Page_BeginText (*page);
   HPDF_Page_MoveTextPos (*page,(HPDF_REAL)X_Cord,HPDF_Page_GetHeight(*page)-Y_Cord);
   HPDF_Page_ShowText (*page,STR);
   HPDF_Page_EndText (*page);

}

void PDFReportGeneration::PdfWithImage(HPDF_Doc * pdf,HPDF_Page * page,HPDF_Font *def_font)
{   
   /* Draw image to the canvas. (normal-mode with actual size.)*/
//   HPDF_Page_DrawImage (*page, image, HPDF_Page_GetWidth(*page)-230.0f,HPDF_Page_GetHeight(*page)-35.0f, (HPDF_REAL)(HPDF_Image_GetWidth (image)), (HPDF_REAL)(HPDF_Image_GetHeight (image)));
   HPDF_Page_SetLineWidth (*page,2);
   HPDF_Page_MoveTo (*page, 2, HPDF_Page_GetHeight(*page)-THREEWELDLINEGAP);
   HPDF_Page_LineTo (*page, HPDF_Page_GetWidth(*page)-10, HPDF_Page_GetHeight(*page)-THREEWELDLINEGAP) ;
   HPDF_Page_Stroke (*page);
}

STATUS PDFReportGeneration::CreateNewPage(HPDF_Doc &pdf,HPDF_Page &page, HPDF_Font &def_font, UINT32 pageNumber)
{
	INT8 retStatus = OK;
	char STR[30];
	UINT8 index = 0;
	UINT32 headerLineGap = 0;
	UINT32 columnNumber = 0;
	intf_SysConfig_t SysConfigInfo;
	
	string headerStr = "",unitsStr = "";
	vector<string> headerStrVec;
	vector<string> unitsStrVec;
	
	char  tmpStr[20] = { 0 };
	char  rowStr[COLUMNWELDRESULT][3][20] = { 0 };
	int rowIndex = 0;
	int strIndex = 0;
	
	//string columnKeyStr;
	char columnKeyStr[64] = {0x00};
	char unitsColumnKeyStr[64] = {0x00};
	
	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
			strcpy(STR,"BRANSON------Weld Data");
			headerLineGap = LINEGAPWELDRESULT;
			columnNumber = COLUMNWELDRESULT;
			break;

		case EVENT_LOG:
			strcpy(STR,"BRANSON------Event Log");
			headerLineGap = LINEGAPEVENTLOG;
			columnNumber = COLUMNEVENTLOG;
			break;

		case ALARM_LOG:
			strcpy(STR,"BRANSON------Alarm Log");
			headerLineGap = LINEGAPALARMLOG;
			columnNumber = COLUMNALARMLOG;
			break;

		case RECIPE_SETTING:
			strcpy(STR,"BRANSON------Recipe Setting");
			headerLineGap = LINEGAPRECIPE;
			columnNumber = COLUMNRECIPE;
			break;

		case USER_ID:
			strcpy(STR,"BRANSON------User Setting");
			headerLineGap = LINEGAPUSERID;
			columnNumber = COLUMNUSERID;
			break;

		case SYSTEM_SETUP:
			strcpy(STR,"System Setup");
			headerLineGap = LINEGAPSYSTEMSETUP;
			columnNumber = COLUMNSYSTEMSETUP;
			break;
			
		default:
			strcpy(STR,"Unknown Report ID");
			LOGERR("ReportGeneration : ReadReportHeader: Unknown Report ID Received",0,0,0);
			break;
	}
	
	HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A3, HPDF_PAGE_LANDSCAPE);
	
	
	/* Draw Pdf file header */
	// TODO Add Fonts 
	//fontname = HPDF_LoadTTFontFromFile(pdf, PDF_FONT_PATH, HPDF_TRUE);
	//def_font = HPDF_GetFont(pdf, fontname, "UTF-8");

	HPDF_Page_SetFontAndSize (page, def_font,25);
	
	DrawPDFText(&page,10,TWOWELDLINEGAP,STR);
	
	PdfWithImage(&pdf, &page, &def_font);
	
	/* Page Number */
	HPDF_Page_SetFontAndSize (page, def_font,12);
	int x = (int)HPDF_Page_GetWidth(page)-90;
	memset(STR,0,sizeof(STR));
	sprintf(STR,"Page: %d",pageNumber);
	DrawPDFText(&page,x,TWOWELDLINEGAP+10,STR);
	
	if(m_reportType == WELDRESULT_TYPE)
	{
		HPDF_Page_SetFontAndSize(page,def_font,6);	
	}
	else if (m_reportType == EVENT_LOG)
	{	
		HPDF_Page_SetFontAndSize(page,def_font,8);	
	}
	else
	{
		HPDF_Page_SetFontAndSize(page,def_font,12);	
	}
	
	/* Draw Pdf file Header Content */
	tableColumnCount = GetColumnCount();
	headerStr = ReadReportHeader();
	
	if(TRUE == headerStr.empty())
	{
		retStatus = FAILURE ;
	}
	else
	{
		/* Split the string to individual keys */
		SplittingData(headerStr , headerStrVec , ',');

		/* Check for integrity of the keys */
		if(headerStrVec.size() != tableColumnCount)
		{
			/* Incorrect number of keys present, the program has to exit here */
			if((m_reportType != SYSTEM_SETUP)&&(m_reportType != ALARM_LOG)&&(m_reportType != RECIPE_SETTING))		
				retStatus = FAILURE ;
		}		
	}
	
	YCoordinate = YCoordinate + TWOWELDLINEGAP;
	
	if(OK == retStatus)	
	{
		for(index = 0 ; index < headerStrVec.size(); index++)
		{				
			/* Layout the Header for weld result report */
			if(m_reportType == WELDRESULT_TYPE)
			{
				if(index == i_Username)      // Replace the "Username" to "UserName"
					headerStrVec[index] = "UserName";
				else if(index == i_AlarmFlag)
					headerStrVec[index] = "AlarmID";
				
				sprintf(tmpStr, "%s", headerStrVec[index].c_str());
				for (int i = 0; i < strlen(tmpStr); i++)
				{
					if (tmpStr[i] >= 'A'&& tmpStr[i] <= 'Z' && i != 0)
					{
						rowIndex += 1;
						strIndex = 0;
						if (rowIndex == 3)   // max 3 header lines
							break;
					}
					if(tmpStr[i] == 'I')     // ID
					{
						rowStr[index][rowIndex][strIndex] = tmpStr[i];
						rowStr[index][rowIndex][strIndex+1] = 'D';
						break;
					}
					rowStr[index][rowIndex][strIndex] = tmpStr[i];
					strIndex++;

				}

				rowIndex = 0;
				strIndex = 0;
				memset(tmpStr, 0, sizeof(tmpStr));
				
			}
			else
			{
				if((index == 1)&&(m_reportType == USER_ID))
					sprintf(columnKeyStr,"Created DateTime");
				else if((index == 4)&&(m_reportType == ALARM_LOG))
					sprintf(columnKeyStr,"UserID");
				else if((index == 54)&&(m_reportType == RECIPE_SETTING))
					sprintf(columnKeyStr,"AfterBurst");
				else if((index == 75)&&(m_reportType == RECIPE_SETTING))
					sprintf(columnKeyStr,"GlobalLimitControl");
				else
					sprintf(columnKeyStr,"%s",headerStrVec[index].c_str());
				
				DrawPDFText(&page, XCord, YCoordinate,columnKeyStr);
				
				if(m_reportType == EVENT_LOG)
					XCord += PDFParamWidthEvent[index];
				else
					XCord += headerLineGap;
				memset(columnKeyStr, 0, sizeof(columnKeyStr));	
				if((index+1)%columnNumber == 0)
				{
					YCoordinate = YCoordinate + TWOWELDLINEGAP;
					XCord = PRINTWELDSTARTXCORD;
				}
			}
		}
		
		/* Print the Header and Units for Weld Result Report */
		if(m_reportType == WELDRESULT_TYPE)
		{
			/* Print Header */
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<COLUMNWELDRESULT; j++)
				{
					DrawPDFText(&page, XCord, YCoordinate,rowStr[j][i]);
					XCord += PDFParamWidthWeld[j];
				}

				YCoordinate = YCoordinate + LINEGAP;
				XCord = PRINTWELDSTARTXCORD;
			}
			
			/* Print Units for Weld Result report */
			YCoordinate += LINEGAP;
			XCord = PRINTWELDSTARTXCORD;
			/*To access system unit*/
			CP->GetSysConfContext(SysConfigInfo);
			/*To insert Imperial Units header for Weld Result*/
			if(SysConfigInfo.UnitIs == IMPERIAL)
			{
				unitsStr = WELD_RESULT_PDF_IMPERIAL_UNITS_STRING;
			}
			/*To insert Metric Units header for Weld Result*/
			else
			{
				unitsStr = WELD_RESULT_PDF_METRIC_UNITS_STRING;
			}
			SplittingData(unitsStr , unitsStrVec , ',');
			for(index = 0 ; index < unitsStrVec.size(); index++)
			{				
				sprintf(unitsColumnKeyStr,"%s",unitsStrVec[index].c_str());

				DrawPDFText(&page, XCord, YCoordinate,unitsColumnKeyStr);
				if(index == 0)
					XCord += 30;
				else if(index == 1)
					XCord += (PDFParamWidthWeld[0]+PDFParamWidthWeld[1]-30);	//Retract for yy/mm/dd hh/mm/ss				
				else
					XCord += PDFParamWidthWeld[index];
				
				memset(unitsColumnKeyStr, 0, sizeof(unitsColumnKeyStr));	
			}
		}	
		
	}		

	return retStatus;
}

STATUS PDFReportGeneration::GenerateReport(UINT8 a_checkStatusCmd)
{	
	INT8 retStatus = OK, status = OK ;
	INT32 queryStatus = 0;
	HPDF_STATUS pdfStatus = 0xffff;
	string reportFile="";
	string dateTime="";
	string IOstr = "";
	string AlarmCFGStr = "";
	char strQuery[MSG_SIZE * 4] = {0x00};
	char STR[30];
	char columnKeyStr[64] = {0x00};
	char contentStr[64] = {0};
	char ioTypeStr[64] = {0};
	char dateTimeStr[64]= {0};
	char systemSetupStr[128] = {0x00};
	string headerStr = "";
	
	vector<string> headerStrVec;
	vector<string> contentStrVec;
	
	UINT8 ioType =0;
	UINT32 totalResults = 0;
	UINT32 pageNumber = 0;
	UINT32 YcordTmp =0, XcordTmp =0;
	
	HPDF_Font def_font,def_font_bold;
//	HPDF_Image image;
	
	if((SC::stateMachine.GetStateType() == READY) || (SC::stateMachine.GetStateType() == SC_ALARM))
	{
		m_reportGenerationFlag = false;
		
		ConnectDB();
		
		status = CheckStorageDevice();	
		if(OK != status)
		{
			LOGERR("ERROR: USB_STORAGE_NOT_PRESENT\n",0,0,0);
			retStatus = USB_STORAGE_NOT_PRESENT ;
			
			CloseDB();
			m_reportGenerationFlag = true;
			
			return retStatus;
		}	
				
		if(m_reportType != SYSTEM_SETUP)  /* Decide to Move the System setup storage to Eeprom */
		{		
			/* Get records counts */
			totalResults = GetDBCount();
			minRowID = GetDBMinRowID();
			if(totalResults > 0)	
			{	
				/* Split the pdf report to several files, Max 50000 records per file */
				if(totalResults > MAXRECORDLENGTH)
				{					
					if(totalResults % MAXRECORDLENGTH == 0)
						totalFileNumber = totalResults/MAXRECORDLENGTH;
					else 
						totalFileNumber = totalResults/MAXRECORDLENGTH + 1;							
				}
				else
				{
					totalFileNumber = 1;
				}
				
				for(int fileCnt=0; fileCnt  <totalFileNumber; fileCnt++)
				{
					pdf = HPDF_New(PDFReportGeneration::error_handler, this);	
					if (!pdf) 
					{
						LOGERR("ERROR: cannot create pdf object. File= %d\n",fileCnt,0,0);
						retStatus = REPORT_GENERATION_FAILED;
						
						CloseDirectoryFileDescriptor();
						CloseStorageDevice();
						CloseDB();
						m_reportGenerationFlag = true;
									
						return retStatus;
					}
				
					def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
					def_font_bold = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
					HPDF_UseUTFEncodings(pdf);		  
					//image = HPDF_LoadJpegImageFromFile (pdf, PDF_IMAGE_PATH);
					//image = HPDF_LoadJpegImageFromMem(pdf, NULL, 0);
				
					if(totalFileNumber==1)
					{
						if(m_reportType == RECIPE_SETTING)
							pageNumber = totalResults;
						else
						{
							if(totalResults % ROWSPERPAGE == 0)
								pageNumber = totalResults/ROWSPERPAGE;
							else 
								pageNumber = totalResults/ROWSPERPAGE + 1;					
						}
					}
					else
					{
						if(fileCnt <(totalFileNumber-1))
						{
							if(m_reportType == RECIPE_SETTING)
								pageNumber = MAXRECORDLENGTH;
							else
							{
								if(MAXRECORDLENGTH % ROWSPERPAGE == 0)
									pageNumber = MAXRECORDLENGTH/ROWSPERPAGE;
								else 
									pageNumber = MAXRECORDLENGTH/ROWSPERPAGE + 1;						
							}
						}
						else
						{											
							if(m_reportType == RECIPE_SETTING)
								pageNumber = totalResults - fileCnt*MAXRECORDLENGTH;
							else
							{
								if((totalResults - fileCnt*MAXRECORDLENGTH) % ROWSPERPAGE == 0)
									pageNumber = (totalResults - fileCnt*MAXRECORDLENGTH)/ROWSPERPAGE;
								else 
									pageNumber = (totalResults - fileCnt*MAXRECORDLENGTH)/ROWSPERPAGE + 1;						
							}
						}
					}
			
					for(int pageCnt=0; pageCnt<pageNumber; pageCnt++)
					{
						HPDF_Page page = HPDF_AddPage (pdf);
						XCord = PRINTWELDSTARTXCORD;
						YCoordinate = TWOWELDLINEGAP;
		
						status = CreateNewPage(pdf,page,def_font_bold, pageCnt+1);
						if(OK == status)	
						{
							status = CreatePdfContent(&page,def_font,pageCnt,fileCnt);
							if(OK != status)
							{
								retStatus = REPORT_GENERATION_FAILED;
								LOGERR("ERROR: Write data to pdf failed: fileNumber = %d, pageNumber = %d\n",fileCnt, pageCnt,0);
							}
							else
							{
								if(m_reportType == RECIPE_SETTING)
								{
									HPDF_Page page4Recipe = HPDF_AddPage (pdf);
									XCord = PRINTWELDSTARTXCORD;
									YCoordinate = TWOWELDLINEGAP;
	
									status = createSuspectRejectLimits(&pdf,&page4Recipe,def_font,pageCnt);
									if(OK != status)	
									{
										retStatus = REPORT_GENERATION_FAILED;
										LOGERR("ERROR: CreateNewPage4Recipe failed: fileNumber = %d, pageNumber = %d\n",fileCnt, pageCnt,0);
									}
		
								}
								
								
							}
						}
						else
						{
							retStatus = REPORT_GENERATION_FAILED;
							LOGERR("ERROR: CreateNewPage failed: fileNumber = %d, pageNumber = %d\n",fileCnt, pageCnt,0);
						}
										
					}
							
					if(OK == status)	
					{		
						/* Save document to file */
						
						reportFile = ReportNameGeneration();
						sprintf(STR,"%d",fileCnt);
						reportFile.insert(reportFile.length()- 4, "_"+string(STR));
						memset(STR,0,sizeof(STR));
						
						pdfStatus = HPDF_SaveToFile (pdf, reportFile.c_str());
						if(OK == pdfStatus)
						{
							retStatus = REPORT_GENERATE_SUCCESS;
							LOGERR("Libharu Saved...\n",0,0,0);
							
							if(TRUE == m_deleteID && ( fileCnt == (totalFileNumber - 1) ) )
							{
								status = clearDataFromDB4Pdf();
								if(OK != status)
								{
									LOGERR("Libharu clearDataFromDB4Pdf failed\n",0,0,0);	
								}
							}
							
						}
						else
						{
							retStatus = REPORT_GENERATION_FAILED;
							LOGERR("Libharu Save pdf file failed...\n",0,0,0);						
						}
												
						LOGERR("Libharu done...\n",0,0,0);
						
					}
					
					HPDF_Free(pdf);
				}
			}
			else
			{
				retStatus = DB_EMPTY;
				LOGERR("No Content in DB\n",0,0,0);
			}
		}
		else
		{    /* Create the System Setup Report from Eeprom */
			LOGERR("ReportGeneration: Create Pdf file of SystemSetup from eeprom",0,0,0);
			
			pdf = HPDF_New(PDFReportGeneration::error_handler, this);	
			if (!pdf) 
			{
				LOGERR("ERROR: cannot create pdf object.\n",0,0,0);
				retStatus = REPORT_GENERATION_FAILED;
				
				CloseDirectoryFileDescriptor();
				CloseStorageDevice();
				CloseDB();
				m_reportGenerationFlag = true;
				
				return retStatus;
			}
			
			def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
			def_font_bold = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
			HPDF_UseUTFEncodings(pdf);
			
			/* Page 1 - System Setup From eeprom */
			HPDF_Page page = HPDF_AddPage (pdf);
			
			HPDF_Page_SetSize (page, HPDF_PAGE_SIZE_A3, HPDF_PAGE_LANDSCAPE);
							
			/* Draw Pdf file header */
			HPDF_Page_SetFontAndSize (page, def_font_bold,25);
			
			strcpy(STR,"BRANSON------System Setup");
					
			DrawPDFText(&page,10,TWOWELDLINEGAP,STR);
			
			PdfWithImage(&pdf, &page, &def_font_bold);
			
			HPDF_Page_SetFontAndSize(page,def_font_bold,14);	
			
			XCord = PRINTWELDSTARTXCORD ;
			YCoordinate = FOURWELDLINEGAP;
					
			DrawPDFText(&page, XCord, YCoordinate,"Page 1:");
											
			GetDateAndTime(dateTime);
			sprintf(dateTimeStr,"%s",dateTime.c_str());
			YCoordinate = FIVEWELDLINEGAP;
			XCord = PRINTWELDSTARTXCORD;
			DrawPDFText(&page, XCord, YCoordinate,dateTimeStr);
			dateTime.clear();
			memset(dateTimeStr,0x0,sizeof(dateTimeStr));
			
			YCoordinate = SIXWELDLINEGAP;
			XCord = PRINTXCORDCENTER;
			HPDF_Page_SetFontAndSize(page,def_font_bold,16);
			DrawPDFText(&page, XCord, YCoordinate,"SYSTEM SETUP");
			
			YCoordinate = EIGHTWELDLINEGAP;
			XCord = 40;	
			HPDF_Page_SetFontAndSize(page,def_font_bold,12);
			
			/* Create Header for System Setup */
			headerStr = SYSTEM_SETUP_PARAM_STRING_FROM_EEPROM;  // Total 22
			SplittingData(headerStr , headerStrVec , ',');
			for(int index = 0 ; index < headerStrVec.size(); index++)
			{		
				sprintf(columnKeyStr,"%s",headerStrVec[index].c_str());
				DrawPDFText(&page, XCord, YCoordinate,columnKeyStr);
				XCord += LINEGAPSYSTEMSETUP;
				memset(columnKeyStr, 0, sizeof(columnKeyStr));	
				if((index+1)%COLUMNSYSTEMSETUP == 0)
				{
					YCoordinate = YCoordinate + TWOWELDLINEGAP;
					XCord = 40;
				}
			}
			/* Create Content for System Setup */	
			status = CreatePdfContent(&page, def_font,0,0);
			if(OK != status)	
			{
				retStatus = REPORT_GENERATION_FAILED;
				LOGERR("ERROR: CreatePdfContent for weld setup failed",0,0,0);
			}
			else
			{
				/* Page 1 - Limits Configuration */
				YCoordinate = YCoordinate + ONEWELDLINEGAP;
				XCord = PRINTXCORDCENTER-36;
				HPDF_Page_SetFontAndSize(page, def_font_bold, 16);
				DrawPDFText(&page, XCord, YCoordinate,"ALARM CONFIGURATION");
				
				YCoordinate = YCoordinate + TWOWELDLINEGAP;
				XCord = 275;	
	
				HPDF_Page_SetFontAndSize(page, def_font_bold, 12);
						
				headerStr.clear();
				headerStrVec.clear();
				contentStrVec.clear();
				memset(columnKeyStr, 0, sizeof(columnKeyStr));
						
				/* Draw Alarm CFG Header */
				headerStr = ALARM_CFG_PARAM_STRING;  // Total 4
				SplittingData(headerStr , headerStrVec , ',');
				for(int index = 0 ; index < headerStrVec.size(); index++)
				{		
					sprintf(columnKeyStr,"%s",headerStrVec[index].c_str());
					DrawPDFText(&page, XCord, YCoordinate,columnKeyStr);
					XCord += LINEGAPALARMCFG;
					memset(columnKeyStr, 0, sizeof(columnKeyStr));	
					if((index+1)%COLUMNALARMCFG == 0)
					{
						YCoordinate = YCoordinate + TWOWELDLINEGAP;
						XCord = 275;
					}
			
				}
						
				/* Draw Alarm CFG Content */
				HPDF_Page_SetFontAndSize(page,def_font,10);
				sprintf(strQuery,"select %s from AlarmConfiguration",ALARM_CFG_PARAM_STRING);
				AlarmCFGStr = m_DbConn->Exec(strQuery, &queryStatus);
				if(AlarmCFGStr.empty())
				{
					retStatus = REPORT_GENERATION_FAILED;
					LOGERR("ERROR: Create Alarm CFG failed",0,0,0);			    	
				}
				else
				{
					SplittingData(AlarmCFGStr, contentStrVec, ',');
	
					for(int index = 0 ; index < contentStrVec.size(); index++)
					{
						sprintf(contentStr,"%s",contentStrVec[index].c_str());
								
						if( (index+1+2)%COLUMNALARMCFG == 0) // Reset Required
						{
							int n = atoi(contentStrVec[index].c_str());
							sprintf(contentStr, n==0?"No":"Yes");
						}
								
						if( (index+1+1)%COLUMNALARMCFG == 0) // Log Alarm
						{
							int n = atoi(contentStrVec[index].c_str());
							sprintf(contentStr, n==0?"No":"Yes");
						}
								
						if( (index+1+0)%COLUMNALARMCFG == 0) // General Alarm
						{
							int n = atoi(contentStrVec[index].c_str());
							sprintf(contentStr, n==0?"No":"Yes");
						}								
								
						DrawPDFText(&page, XCord, YCoordinate,contentStr);
						XCord += LINEGAPALARMCFG;
						memset(contentStr, 0, sizeof(contentStr));
						if((index+1) % COLUMNALARMCFG == 0)
						{
							XCord = 275;
							YCoordinate = YCoordinate + TWOWELDLINEGAP;
						}
					}
				}
							
			} //Page one done				
					
			/* Page 2 - I/O Configuration */
			HPDF_Page page2 = HPDF_AddPage (pdf);
			
			HPDF_Page_SetSize (page2, HPDF_PAGE_SIZE_A3, HPDF_PAGE_LANDSCAPE);
							
			/* Draw Pdf file header */
			// TODO Add Fonts 
			//fontname = HPDF_LoadTTFontFromFile(pdf, PDF_FONT_PATH, HPDF_TRUE);
			//def_font = HPDF_GetFont(pdf, fontname, "UTF-8");
	
			HPDF_Page_SetFontAndSize (page2, def_font_bold,25);
			
			strcpy(STR,"System Setup");
			
			DrawPDFText(&page2,10,TWOWELDLINEGAP,STR);
			
			PdfWithImage(&pdf, &page, &def_font);
			
			HPDF_Page_SetFontAndSize(page2,def_font_bold,14);	
			
			XCord = PRINTWELDSTARTXCORD ;
			YCoordinate = FOURWELDLINEGAP;
				
			DrawPDFText(&page2, XCord, YCoordinate,"Page 2:");
									
			GetDateAndTime(dateTime);
			sprintf(dateTimeStr,"%s",dateTime.c_str());
			YCoordinate = FIVEWELDLINEGAP;
			XCord = PRINTWELDSTARTXCORD;
			DrawPDFText(&page2, XCord, YCoordinate,dateTimeStr);
			dateTime.clear();
			memset(dateTimeStr,0x0,sizeof(dateTimeStr));
			
			YCoordinate = SIXWELDLINEGAP;
			XCord = PRINTXCORDCENTER;
			HPDF_Page_SetFontAndSize(page2,def_font_bold,16);
			DrawPDFText(&page2, XCord, YCoordinate,"I/O SETUP");
			
			YCoordinate = EIGHTWELDLINEGAP;
			XCord = LINEGAPSYSTEMSETUP;	
			HPDF_Page_SetFontAndSize(page2,def_font_bold,14);
			
			/* Create Header for I/O Setup */
			headerStr.clear();
			headerStrVec.clear();
			contentStrVec.clear();
			memset(columnKeyStr, 0, sizeof(columnKeyStr));
			
			headerStr = IO_SETUP_PARAM_STRING;  // Total 5
			SplittingData(headerStr , headerStrVec , ',');
			for(int index = 0 ; index < headerStrVec.size(); index++)
			{		
				sprintf(columnKeyStr,"%s",headerStrVec[index].c_str());
				DrawPDFText(&page2, XCord, YCoordinate,columnKeyStr);
				if(index == 2)
					XCord += TENWELDLINEGAP;
				else
					XCord += LINEGAPSYSTEMSETUP;
				memset(columnKeyStr, 0, sizeof(columnKeyStr));				
			}
					
			/* Create Content for I/O Setup */
			YCoordinate = TENWELDLINEGAP;
			XCord = LINEGAPSYSTEMSETUP;	
			HPDF_Page_SetFontAndSize(page2,def_font,12);
								
			sprintf(strQuery,"select Id,PinName,PinFunction,ActiveStatus,CheckStatus from UserIO ORDER BY Id ASC");
			IOstr = m_DbConn->Exec(strQuery,&queryStatus);
			if(IOstr.empty())
			{
				retStatus = REPORT_GENERATION_FAILED;
				LOGERR("ERROR: Create IO setup failed",0,0,0);			    	
			}
			else
			{
				SplittingData(IOstr, contentStrVec, ',');
	
				for(int index = 0; index < contentStrVec.size(); index++)
				{
					sprintf(contentStr, "%s", contentStrVec[index].c_str());
							
					if((index+1+4)%COLUMNIOSETUP == 0) // Id
					{
						int n = atoi(contentStrVec[index].c_str());
						if(n<10)    // PS_DIGITALINPUT
						{
							sprintf(ioTypeStr, "PS_DIGITALINPUT");
							ioType = PSDIGITALINPUT;
						}
						else if(n<18) // PS_DIGITALOUTPUT
						{
							sprintf(ioTypeStr, "PS_DIGITALOUTPUT");
							ioType = PSDIGITALOUTPUT;
						}
						else if(n<20) // PS_ANALOGINPUT
						{
							sprintf(ioTypeStr, "PS_ANALOGINPUT");
							ioType = PSANALOGINPUT;
						}
						else if(n<22) // PS_ANALOGOUTPUT
						{
							sprintf(ioTypeStr, "PS_ANALOGOUTPUT");
							ioType = PSANALOGOUTPUT;
						}
						else if(n<27) // AC_DIGITALINPUT
						{
							sprintf(ioTypeStr, "AC_DIGITALINPUT");
							ioType = ACDIGITALINPUT;
						}
						else          // AC_DIGITALOUTPUT
						{
							sprintf(ioTypeStr, "AC_DIGITALOUTPUT");
							ioType = ACDIGITALOUTPUT;
						}
					}
					
					if( (index+1+2)%COLUMNIOSETUP == 0) // Pin Function
					{
						int n = atoi(contentStrVec[index].c_str());
						memset(contentStr, 0, sizeof(contentStr));
						switch(ioType)
						{
							case PSDIGITALINPUT:
								sprintf(contentStr, PowerSupplyInputStr[n]);
								break;
							case PSDIGITALOUTPUT:
								sprintf(contentStr, PowerSupplyOutputStr[n]);
								break;
							case PSANALOGINPUT:
								sprintf(contentStr, AnalogInputStr[n]);
								break;
							case PSANALOGOUTPUT:
								sprintf(contentStr, AnalogOutputStr[n]);
								break;
							case ACDIGITALINPUT:
								sprintf(contentStr, ActuatorInputStr[n]);
								break;
							case ACDIGITALOUTPUT:
								sprintf(contentStr, ActuatorOutputStr[n]);
								break;
							default:
								sprintf(contentStr, "--");
								break;
						}
					}
					
					if( (index+1+1)%COLUMNIOSETUP == 0) // Active Status
					{
						int n = atoi(contentStrVec[index].c_str());
						sprintf(contentStr, n==0?"Low":"High");
					}
				
					if( (index+1+0)%COLUMNIOSETUP == 0) // Check Status
					{
						int n = atoi(contentStrVec[index].c_str());
						sprintf(contentStr, n==0?"Disable":"Enable");
					}
					
					if ((ioType != PSANALOGINPUT) && (ioType != PSANALOGOUTPUT)) // Do not print Analog I/Os 
					{
			
						if( !((index+1+4)%COLUMNIOSETUP == 0)) // Do not Print ID
						{
							DrawPDFText(&page2, XCord, YCoordinate,contentStr);					
							memset(contentStr, 0, sizeof(contentStr));
						}
						
						if( ((index+1+3)%COLUMNIOSETUP == 0))
						{
							XCord += LINEGAPSYSTEMSETUP;							
							sprintf(contentStr, ioTypeStr);
							DrawPDFText(&page2, XCord, YCoordinate,contentStr);					
							memset(contentStr, 0, sizeof(contentStr));
							memset(ioTypeStr, 0, sizeof(ioTypeStr));
						}
						
						if((index+1) % COLUMNIOSETUP == 0)
						{
							XCord = LINEGAPSYSTEMSETUP;
							YCoordinate = YCoordinate + ONEWELDLINEGAP;
						}
						else if((index+1+2) % COLUMNIOSETUP == 0)
							XCord += TENWELDLINEGAP;
						else if( !((index+1+4)%COLUMNIOSETUP == 0))
							XCord += LINEGAPSYSTEMSETUP;
						}
				}
			}
	
			if(OK != status)	
			{
				retStatus = REPORT_GENERATION_FAILED;
				LOGERR("ERROR: Create system IO setup failed",0,0,0);
			}
			else	
			{		
				/* Save document to file */
				
				reportFile = ReportNameGeneration();
				
				pdfStatus = HPDF_SaveToFile (pdf, reportFile.c_str());
						
				if(OK == pdfStatus)
				{
					retStatus = REPORT_GENERATE_SUCCESS;
					LOGERR("Libharu Saved...\n",0,0,0);
					
					if(TRUE == m_deleteID)
					{
						status = clearDataFromDB4Pdf();
						if(OK != status)
						{
							LOGERR("Libharu clearDataFromDB4Pdf failed\n",0,0,0);	
						}
					}
					
				}
				else
				{
					retStatus = REPORT_GENERATION_FAILED;
					LOGERR("Libharu Save pdf file failed...\n",0,0,0);						
				}
				LOGERR("Libharu done...\n",0,0,0);
			}
			HPDF_Free(pdf);
		} /* else System setup pdf create */
	
		CloseDirectoryFileDescriptor();
		CloseStorageDevice();
		CloseDB();
		m_reportGenerationFlag = true;
	}
	else
	{
		status = SC_INVALID_STATE;
		LOGERR("ERROR: SC_INVALID_STATE\n",0,0,0);
		
		retStatus = SC_INVALID_STATE;
	}
	
		return retStatus;
}

void PDFReportGeneration::drawLOGParameterHeading(HPDF_Doc * pdf, HPDF_Page * page, HPDF_Font *def_font)
{

	HPDF_Page_SetRGBFill(*page, 0.75, 0.75, 0.75);

	HPDF_Page_SetLineWidth (*page,2);
   
	if(UnitHeadingPos)
	{
	  HPDF_Page_Rectangle(*page, 2,HPDF_Page_GetHeight(*page)-YCoordinate-140, HPDF_Page_GetWidth(*page)-10, THREEWELDLINEGAP); 
	  Unit_Heading_Gap = UNITHEADINGGAP;
	}
	else
	{
	  HPDF_Page_Rectangle(*page, 2,HPDF_Page_GetHeight(*page)-YCoordinate-THREEWELDLINEGAP, HPDF_Page_GetWidth(*page)-10, HEADERHEIGHT);
	  Unit_Heading_Gap = 0;
	}   
	
	HPDF_Page_FillStroke(*page);
	HPDF_Page_SetRGBFill(*page, 0.0, 0.0, 0.0);
	  
}

void PDFReportGeneration::drawHeading(HPDF_Doc * pdf, HPDF_Page * page, HPDF_Font *def_font)
{
	int XCord=PRINTWELDSTARTXCORD;
	
	memset (Heading_Str, 0, 256);
	
	HPDF_Page_SetRGBFill(*page, 0.75, 0.75, 0.75);
	
	HPDF_Page_SetLineWidth (*page,2);
	HPDF_Page_Rectangle(*page, 2,HPDF_Page_GetHeight(*page)-YCoordinate-THREEWELDLINEGAP, HPDF_Page_GetWidth(*page)-450, HEADERHEIGHT);
	HPDF_Page_FillStroke(*page);
	HPDF_Page_SetRGBFill(*page, 0.0, 0.0, 0.0);

	sprintf(Heading_Str,"%s,%s,%s:%s,%s,%s",UserIdStrHdr.c_str(),PresetStrHdr.c_str(),PresetRevStrHdr.c_str(),"0","0",ValidStr.c_str());
	DrawPDFText(page,XCord, YCoordinate + ONEWELDLINEGAP,Heading_Str);
	
	/* TODO Missing Settings */
	
}

STATUS PDFReportGeneration::clearDataFromDB4Pdf()
{
	INT8 status = OK;
	INT32 queryStatus = 0 ;
	char DeleteQuery[MSG_SIZE * 4] = {0x00};
	EVENT_LOGS logID = NO_LOG_ID;

	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
			sprintf(DeleteQuery, "DELETE FROM WeldResultTable;");
			logID = WELD_RESULTS_LOG_ID; 
			break;

		case WELDSIGNATURE_TYPE:
			sprintf(DeleteQuery, "DELETE FROM WeldResultSignature;");
			logID = WELD_SIGNATURE_LOG_ID; 
			break;
			
		case EVENT_LOG:
			sprintf(DeleteQuery, "DELETE FROM EventLogTable;");
			logID = EVENT_LOG_LOG_ID; 
			break;	
			
		case ALARM_LOG:
			sprintf(DeleteQuery, "DELETE FROM AlarmLogs;");
			logID = ALARM_LOG_LOG_ID; 
			break;	
			
		/* Can we delete the Recipe setting DB and USER ID DB? we will see */
//		case RECIPE_SETTING:
//			sprintf(DeleteQuery, "DELETE FROM WeldRecipeTable;");
//			break;	
//			
//		case USER_ID:
//			sprintf(DeleteQuery, "DELETE FROM UserProfiles;");
//			break;				
		
		default:
			LOGERR("clearDataFromDB4Pdf : Unknown Report ID Received",0,0,0);
			break;
	}
	
	m_DbConn->Exec(DeleteQuery, &queryStatus);
	
	if(0==queryStatus)
	{
		if (m_reportType == EVENT_LOG)
			m_DbConn->Exec("UPDATE SQLITE_SEQUENCE SET SEQ=0 WHERE NAME='EventLogTable';", &queryStatus);
		
		LOGDBG("ReportGeneration : clearDataFromDB4Pdf : Deleted entries from DB successfully",0,0,0);
		DataBaseManager::EnterEventLogDeleteLog (logID);
	}
	else
	{
		status=FAILURE;
		LOGERR("ReportGeneration : clearDataFromDB4Pdf : Error in Delete: %d",queryStatus,0,0);
	}

	/* TODO Delete Response need to be completed for all report types */
	
	PostDeleteResponse();
	m_DbConn->DBVacuum(); 
	
	return status;

}

/**************************************************************************//**
* \brief   - Read WeldResult Records from DB and Store to pdf
*
* \param   - None
*
* \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS PDFReportGeneration::CreatePdfContent(HPDF_Page * page, HPDF_Font &def_font,UINT32 pageNumber,UINT32 fileNumber)
{
	UINT32 index = 0;
	INT16 status = OK;
	UINT32 columnNumber = 0, rowGap =0,colGap=0, recpNum = 0;
	INT32 queryStatus = 0;
	UINT64 rangeA =0, rangeB=0;
	float tmpConvert = 0.0;
	size_t recipeLastPos=0, recipeFirstPos=0;
	SystemConfiguration SysConfig;
	intf_SysConfig_t SysConfigInfo;
	
	char buffer[512] = {0x00};
	char systemSetupChar[512] = {0x00};
	char SelectQuery[MSG_SIZE * 4] = {0x00};
	
	char version_SC[SYSINFO_SIZE];
	char version_PC[SYSINFO_SIZE];
	char version_AC[SYSINFO_SIZE];
	char version_UI[SYSINFO_SIZE] = { 0x00 };
	
	string DBBuffer = "";
	string RecipeNumber="";

	RT.FeaturesFlagRead(EEPROM_MODEL_ADDRESS);
	
	char contentStr[64] = {0};
	char weldDataStr[512] = {0};
	vector<string> contentStrVec;
	Eeprom eeprom;
	memset(SelectQuery, 0, sizeof(SelectQuery));
	
	rangeA = minRowID + ROWSPERPAGE * pageNumber + fileNumber * MAXRECORDLENGTH;
	rangeB = rangeA + 35;
	
	switch(m_reportType)
	{
		case WELDRESULT_TYPE:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM WeldResultTable WHERE ROWID >= %llu AND ROWID < %llu;",WELD_RESULT_PARAM_STRING, rangeA, rangeB);
			columnNumber = COLUMNWELDRESULT;
			colGap = LINEGAPWELDRESULT;
			rowGap = ONEWELDLINEGAP;
			
			YCoordinate += ONEWELDLINEGAP;
			XCord = PRINTWELDSTARTXCORD;			
			break;

		case WELDSIGNATURE_TYPE:
			LOGERR("ReportGeneration: No Weld Signature for Pdf",0,0,0);
			break;

		case EVENT_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM EventLogTable WHERE ROWID >= %llu AND ROWID < %llu;",EVENT_LOG_PARAM_STRING, rangeA, rangeB);
			//snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM EventLogTable ORDER BY EventNum DESC LIMIT %d OFFSET %d;",EVENT_LOG_PARAM_STRING, ROWSPERPAGE, ROWSPERPAGE * pageNumber + fileNumber * MAXRECORDLENGTH);
			columnNumber = COLUMNEVENTLOG;
			colGap = LINEGAPEVENTLOG;
			rowGap = ONEWELDLINEGAP;
			
			YCoordinate = SIXWELDLINEGAP;
			XCord = PRINTWELDSTARTXCORD;
			break;

		case ALARM_LOG:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s,AlarmID FROM AlarmLogs WHERE ROWID >= %llu AND ROWID < %llu;",ALARM_LOG_PARAM_STRING, rangeA, rangeB);
			//snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM AlarmLogs ORDER BY DateTime DESC LIMIT %d OFFSET %d;",ALARM_LOG_PARAM_STRING, ROWSPERPAGE, ROWSPERPAGE * pageNumber + fileNumber * MAXRECORDLENGTH);
			columnNumber = COLUMNALARMLOG;
			colGap = LINEGAPALARMLOG;
			rowGap = ONEWELDLINEGAP;
			
			YCoordinate = SIXWELDLINEGAP;
			XCord = PRINTWELDSTARTXCORD;
			break;

		case RECIPE_SETTING:
			//sprintf(SelectQuery, "SELECT %s FROM WeldRecipeTable WHERE ROWID >= %llu AND ROWID < %llu;",ROWSPERPAGERECIPE, rangeA, rangeB);
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM WeldRecipeTable ORDER BY RecipeNumber ASC LIMIT %d OFFSET %d;",WELD_RECIPE_PARAM_STRING, ROWSPERPAGERECIPE, ROWSPERPAGERECIPE * pageNumber);
			columnNumber = COLUMNRECIPE;
			colGap = LINEGAPRECIPE;
			rowGap = TWOWELDLINEGAP;
			
			YCoordinate = FIVEWELDLINEGAP;
			XCord = PRINTWELDSTARTXCORD;
			break;

		case USER_ID:
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM UserProfiles WHERE ROWID >= %llu AND ROWID < %llu;",USER_ID_PARAM_STRING, rangeA, rangeB);
			//sprintf(SelectQuery, "SELECT %s FROM UserProfiles ORDER BY DateTime ASC LIMIT %d OFFSET %d;", USER_ID_PARAM_STRING, ROWSPERPAGE, ROWSPERPAGE * pageNumber + fileNumber * MAXRECORDLENGTH);
			columnNumber = COLUMNUSERID;
			colGap = LINEGAPUSERID;
			rowGap = ONEWELDLINEGAP;
			
			YCoordinate = SIXWELDLINEGAP;
			XCord = PRINTWELDSTARTXCORD;
			break;

		case SYSTEM_SETUP:
			//sprintf(SelectQuery, "SELECT * FROM SystemConfiguration;");
			columnNumber = COLUMNSYSTEMSETUP;
			colGap = LINEGAPSYSTEMSETUP;
			rowGap = TWOWELDLINEGAP;
			
			YCoordinate = EIGHTWELDLINEGAP + 18;
			XCord = 40;
			break;
			
		default:
			LOGERR("ReportGeneration: Unknown Report ID Received",0,0,0);
			break;
	}
	

	if(m_reportType != SYSTEM_SETUP)
	{
		DBBuffer = m_DbConn->Exec(SelectQuery,&queryStatus);
		
		if(SQLITE_OK == queryStatus && m_reportType == RECIPE_SETTING)
		{
			recipeLastPos = DBBuffer.find(DB_DELIMITER, recipeFirstPos);
			RecipeNumber = DBBuffer.substr(recipeFirstPos, recipeLastPos - recipeFirstPos);
			recpNum = atoi(RecipeNumber.c_str());
			
			memset(SelectQuery, 0, sizeof(SelectQuery));
			snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM StackRecipeTable WHERE RecipeNumber = %d LIMIT 1;", STACK_RECIPE_PARAM_STRING, recpNum);
			
			DBBuffer += DB_DELIMITER;
			DBBuffer += m_DbConn->Exec(SelectQuery,&queryStatus);
			
			//Shows Dynamic parameters if Dynamic RunTime flag is enabled
			if(ENABLE == RT.ModelFeature.RtFeature2.Dynamic_Flag_B49){
				memset(SelectQuery, 0, sizeof(SelectQuery));
				snprintf(SelectQuery,sizeof(SelectQuery), "SELECT %s FROM WeldRecipeTable WHERE RecipeNumber = %d LIMIT 1;",DYNAMIC_RECIPE_PARAM_STRING, recpNum);
				
				DBBuffer += DB_DELIMITER;
				DBBuffer += m_DbConn->Exec(SelectQuery, &queryStatus);
			}
		}
	}
	
	
	if((SQLITE_OK == queryStatus)||(m_reportType == SYSTEM_SETUP))  
	{	
		if(TRUE == DBBuffer.empty())
		{
			/* No Weld Setup Record in DB, Read from EEPROM */
			if(m_reportType == SYSTEM_SETUP)
			{
				SysConfig.SysConfigGet(buffer);
				memcpy(&SysConfigInfo,buffer,sizeof(SysConfigInfo));
				
				eeprom.Read ((char *) version_UI, SYSINFO_SIZE, UIC_FW_VER_EEPROM_OFFSET);

#ifdef MAINLINE_BUGFIX_BUILD
									
				sprintf(systemSetupChar, "%s, %d.%d.%d.%d, %d.%d.%d.%d, %d.%d.%d.%d, %d, %d, %s, %s, %s, %s, %s, %d, %d, %d, %s, %s, %c, %s, %s, %s, %s, %s",
						version_UI,
						FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),FWVersion::Get(FW_VERSION_SC, VERSION_BUILD),SC_FW_VERSION_AUTOS,
						FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),FWVersion::Get(FW_VERSION_PC, VERSION_BUILD),((FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR) == 2 && FWVersion::Get(FW_VERSION_PC, VERSION_MINOR) == 0) ? 1 : PC_FW_VERSION_AUTOS),
						FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),FWVersion::Get(FW_VERSION_AC, VERSION_BUILD),AC_FW_VERSION_AUTOS,
						Counters::getCounter(LIFETIME_WELDS),
						SysConfigInfo.bIsHornClamp,GetLangugeStr(SysConfigInfo.LanguageIs),GetScreenStr(SysConfigInfo.Sscreen),GetPowerUpOptionStr(SysConfigInfo.PsPowerUpOptionIs),SysConfigInfo.MemFullActionIs ==0?"Stop":"Continue",																																SysConfigInfo.AuthCheckLevel==0?"Disabled":"Full",SysConfigInfo.ActConfig.MaxStrokelength,setFrequencyValue(SysConfigInfo.PcConfig.PSFreq),SysConfigInfo.PcConfig.PSWatt,																															SysConfigInfo.PcConfig.PsTypeIs==0?"Standard":"Unknown",SysConfigInfo.machineName,SysConfigInfo.PresetRecipeBarcodeStart,SysConfigInfo.bPartIDSwitchStatus==0?"Off":"On",
						SysConfigInfo.SC_SerialNumber,SysConfigInfo.PC_SerialNumber,SysConfigInfo.AC_SerialNumber,SysConfigInfo.systemType==0?"GSX-E1":"Unknown");
#else
				
				sprintf(systemSetupChar, "%s, %d.%d.%d, %d.%d.%d, %d.%d.%d, %d, %d, %s, %s, %s, %s, %s, %d, %d, %d, %s, %s, %c, %s, %s, %s, %s, %s",
						version_UI,
						FWVersion::Get(FW_VERSION_SC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_SC, VERSION_MINOR),FWVersion::Get(FW_VERSION_SC, VERSION_BUILD),
						FWVersion::Get(FW_VERSION_PC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_PC, VERSION_MINOR),FWVersion::Get(FW_VERSION_PC, VERSION_BUILD),
						FWVersion::Get(FW_VERSION_AC, VERSION_MAJOR),FWVersion::Get(FW_VERSION_AC, VERSION_MINOR),FWVersion::Get(FW_VERSION_AC, VERSION_BUILD),
						Counters::getCounter(LIFETIME_WELDS),
						SysConfigInfo.bIsHornClamp,GetLangugeStr(SysConfigInfo.LanguageIs),GetScreenStr(SysConfigInfo.Sscreen),GetPowerUpOptionStr(SysConfigInfo.PsPowerUpOptionIs),SysConfigInfo.MemFullActionIs ==0?"Stop":"Continue",																																SysConfigInfo.AuthCheckLevel==0?"Disabled":"Full",SysConfigInfo.ActConfig.MaxStrokelength,setFrequencyValue(SysConfigInfo.PcConfig.PSFreq),SysConfigInfo.PcConfig.PSWatt,																															SysConfigInfo.PcConfig.PsTypeIs==0?"Standard":"Unknown",SysConfigInfo.machineName,SysConfigInfo.PresetRecipeBarcodeStart,SysConfigInfo.bPartIDSwitchStatus==0?"Off":"On",
						SysConfigInfo.SC_SerialNumber,SysConfigInfo.PC_SerialNumber,SysConfigInfo.AC_SerialNumber,SysConfigInfo.systemType==0?"GSX-E1":"Unknown");
#endif
				
				DBBuffer = systemSetupChar;
				
				
				/* Split the string to individual keys */
				SplittingData(DBBuffer , contentStrVec , ',');
				
			}
			else
			{
				//printf("DBBuffer empty\n");
				status = FAILURE ;
			}
		}
		else
		{
			/* Split the string to individual keys */
			SplittingData(DBBuffer , contentStrVec , ',');

		}

		if (m_reportType == WELDRESULT_TYPE)
		{
			HPDF_Page_SetFontAndSize(*page,def_font,6);	
		}
		else if ( m_reportType == EVENT_LOG)
		{	
			HPDF_Page_SetFontAndSize(*page,def_font,8);	
		}
		else
		{	HPDF_Page_SetFontAndSize(*page,def_font,10);}
		
		if(OK == status)	
		{
			for(index = 0 ; index < contentStrVec.size(); index++)
			{
				if(contentStrVec[index].c_str() == NULL)
				{
					sprintf(contentStr," ");
				}
				else
					sprintf(contentStr,"%s",contentStrVec[index].c_str());
				
				/* format the weld data units */ 
				if(m_reportType == WELDRESULT_TYPE)
				{
					if( index % COLUMNWELDRESULT == i_RecipeStatus ) // Recipe Status
					{
						int n = atoi(contentStrVec[index].c_str());
						switch(n)
						{
						  case SAVED:
							sprintf(contentStr, "Saved");
						    break;
						  case VALIDATED:
							sprintf(contentStr, "Validated");
							break;
						  case UNSAVED:
							sprintf(contentStr, "Unsaved");
							break;
						  default:
							sprintf(contentStr, "--");
							break;
						 }
					}			 		
					if( index % COLUMNWELDRESULT == i_WeldMode ) // Weld Mode
					{
						int n = atoi(contentStrVec[index].c_str());
						switch(n)
						{
						case TIMEMODE:
								sprintf(contentStr, "Time");
								break;
							case ENERGYMODE:
								sprintf(contentStr, "Energy");
								break;
							case PEAKPOWERMODE:
								sprintf(contentStr, "PkPower");
								break;
							case GRDDETECTMODE:
								sprintf(contentStr, "GRDDetect");
								break;
							case ABSMODE:
								sprintf(contentStr, "ABS");
								break;
							case COLLAPSEMODE:
								sprintf(contentStr, "Collapse");
								break;
							case DYNAMICMODE:
								sprintf(contentStr, "Dynamic");
								break;
							case CONTINUOUS:
								sprintf(contentStr, "Continuous");
								break;
							case MULTI:
								sprintf(contentStr, "Multi");
								break;
							default:
								sprintf(contentStr, "--");
								break;
						}
					}
					if( index % COLUMNWELDRESULT == i_AlarmFlag) // Alarm Flag
					{
						int n = atoi(contentStrVec[index].c_str());
						if(1 == n)
						{
							UINT32 recipeNum = atoi(contentStrVec[(index - i_AlarmFlag + i_RecipeNumber)].c_str());
							UINT32 cycleNum = atoi(contentStrVec[(index - i_AlarmFlag + i_CycleCounter)].c_str());
							string strDateTime = contentStrVec[(index - i_AlarmFlag + i_DateTime)];
							sprintf(contentStr, GetAlarmIDForWeldResult(recipeNum, cycleNum, strDateTime).c_str());
						} 
					}
					/*To access system unit*/
					CP->GetSysConfContext(SysConfigInfo);
					/* Convert the Units for Weld Data */
					if((index % COLUMNWELDRESULT == i_MaxWeldForce) || (index % COLUMNWELDRESULT == i_EndHoldForce)) // Max Weld Force and End Hold Force
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 10.0;
						/*Max weld force and End Hold Force conversion For imperial unit*/
						if(SysConfigInfo.UnitIs == IMPERIAL)
						{
							tmpConvert = Util->convertNtolbs(tmpConvert);
							sprintf(contentStr, "%.1f", tmpConvert);
						}
						else
						{
							int newtonValue = (int)round(tmpConvert);
							sprintf(contentStr, "%d", newtonValue);
						}
					}
					else if( index % COLUMNWELDRESULT == i_WeldAbsolute ) // Weld Absolute
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
						{
							sprintf(contentStr, "%.2f", tmpConvert);
						}
						else
						{
							/*imperial unit conversion for weld Absolute Distance*/
							if(SysConfigInfo.UnitIs == IMPERIAL)
							{
								tmpConvert = Util->convertmillimetretoinches(tmpConvert);
								sprintf(contentStr, "%.4f", tmpConvert);
							}
							else
							{
								sprintf(contentStr, "%.3f", tmpConvert);
							}
						}
					}
					else if( index % COLUMNWELDRESULT == i_WeldEnergy ) // Weld Energy
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						sprintf(contentStr, "%.1f", tmpConvert);
					}
					else if( index % COLUMNWELDRESULT == i_TotalAbsolute ) // Total Absolute
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
						{
							sprintf(contentStr, "%.2f", tmpConvert);
						}
						else
						{
							/*imperial unit conversion for Total Absolute Distance*/
							if(SysConfigInfo.UnitIs == IMPERIAL)
							{
								tmpConvert = Util->convertmillimetretoinches(tmpConvert);
								sprintf(contentStr, "%.4f", tmpConvert);
							}
							else
							{
								sprintf(contentStr, "%.3f", tmpConvert);
							}
						}
					}
					else if( index % COLUMNWELDRESULT == i_WeldCollapseDistance ) // Weld Col
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
						{
							sprintf(contentStr, "%.2f", tmpConvert);
						}
						else
						{
							/*imperial unit conversion for Weld Collapse Distance*/
							if(SysConfigInfo.UnitIs == IMPERIAL)
							{
								tmpConvert = Util->convertmillimetretoinches(tmpConvert);
								sprintf(contentStr, "%.4f", tmpConvert);
							}
							else
							{
								sprintf(contentStr, "%.3f", tmpConvert);
							}
						}
					}
					else if( index % COLUMNWELDRESULT == i_HoldCollapseDistance ) // Hold Col
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
						{
							sprintf(contentStr, "%.2f", tmpConvert);
						}
						else
						{
							/*imperial unit conversion for Hold Collapse Distance*/
							if(SysConfigInfo.UnitIs == IMPERIAL)
							{
								tmpConvert = Util->convertmillimetretoinches(tmpConvert);
								sprintf(contentStr, "%.4f", tmpConvert);
							}
							else
							{
								sprintf(contentStr, "%.3f", tmpConvert);
							}
						}
					}
					else if( index % COLUMNWELDRESULT == i_TotalCollapseDistance ) // Total Col
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
						{
							sprintf(contentStr, "%.2f", tmpConvert);
						}
						else
						{
							/*imperial unit conversion for Total Collapse Distance*/
							if(SysConfigInfo.UnitIs == IMPERIAL)
							{
								tmpConvert = Util->convertmillimetretoinches(tmpConvert);
								sprintf(contentStr, "%.4f", tmpConvert);
							}
							else
							{
								sprintf(contentStr, "%.3f", tmpConvert);
							}
						}
					}
					else if( index % COLUMNWELDRESULT == i_TriggerDistance ) // Trigger Distance
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						if(!strcmp(SystemConfiguration::systemTypeName,"ASX"))
						{
							sprintf(contentStr, "%.2f", tmpConvert);
						}
						else
						{
							/*imperial unit conversion for Trigger Distance*/
							if(SysConfigInfo.UnitIs == IMPERIAL)
							{
								tmpConvert = Util->convertmillimetretoinches(tmpConvert);
								sprintf(contentStr, "%.4f", tmpConvert);
							}
							else
							{
								sprintf(contentStr, "%.3f", tmpConvert);
							}
						}
					}
					else if( index % COLUMNWELDRESULT == i_WeldTime ) // Weld Time
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						sprintf(contentStr, "%.3f", tmpConvert);
					}
					else if( index % COLUMNWELDRESULT == i_WeldPeakPower ) // Peak Power
					{
						int n = atoi(contentStrVec[index].c_str());
						//tmpConvert = n * 40.0;   // %
						sprintf(contentStr, "%d", n);
					}
					else if( index % COLUMNWELDRESULT == i_CycleTime ) // Cycle Time
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						sprintf(contentStr, "%.3f", tmpConvert);
					}
					else if( index % COLUMNWELDRESULT == i_TriggerPoint ) // Trigger Point
					{
						int n = atoi(contentStrVec[index].c_str());
						tmpConvert = n / 1000.0;
						sprintf(contentStr, "%.3f", tmpConvert);
					}
					else if((index+23) % COLUMNWELDRESULT == i_AlarmFlag) // Alarm Flag
					{
						if((index+COLUMNWELDRESULT-2)<contentStrVec.size())
						{
						int n = atoi(contentStrVec[index+COLUMNWELDRESULT-2].c_str());
						if(n == 0)
    						SetTxtRGBColor(0.0,0.0,0.0);
						else
							SetTxtRGBColor(0.9,0,0);
						}
					}
				}
				
				if(m_reportType == RECIPE_SETTING)
				{
					if(index == 3 || index == 8 || index == 32) // WeldMode, AmplitudeStepAt, ForceStepAt
					{
						int n = atoi(contentStrVec[index].c_str());
						switch(n)
						{
							case TIMEMODE:
								sprintf(contentStr, "Time");
								break;
							case ENERGYMODE:
								sprintf(contentStr, "Energy");
								break;
							case PEAKPOWERMODE:
								sprintf(contentStr, "PkPower");
								break;
							case GRDDETECTMODE:
								sprintf(contentStr, "GRDDetect");
								break;
							case ABSMODE:
								sprintf(contentStr, "ABS");
								break;
							case COLLAPSEMODE:
								sprintf(contentStr, "Collapse");
								break;
							case DYNAMICMODE:
								sprintf(contentStr, "Dynamic");
								break;
							case CONTINUOUS:
								sprintf(contentStr, "Continuous");
								break;
							case MULTI:
								sprintf(contentStr, "Multi");
								break;
							default:
								sprintf(contentStr, "--");
								break;
						}
					}
					      // After-Burst,    PreTrigger,  AutoPreTrigger, DisPreTrigger, EnergyBrake,   TimedSeek,     PreWeldSeek,  PostWeldSeek  GlobalLimitCtr,PkPowerCutoffEnable,FreqLowCutoffEnable,
					else if((index == 54)||(index == 58)||(index == 59)||(index == 60)||(index == 63)||(index == 68)||(index == 69)||(index == 70)||(index == 75)|| (index == 77)   || (index == 79)   
								//AbsCutoffEnable,  FreqHighCutoffEnable,CollapseCutoffEnable,EnergyCutoffEnable,TimeCutoffEnable,GNDEnable,     ReadyPosToggle,IntOffsteFlag,EndOfWeldStore,  ForceLevelEnable
								|| (index == 81)  || (index == 83)    || (index == 85)      || (index == 87)  || (index == 89) || (index == 90)||(index == 93)||(index == 97)||(index == 99)||(index == 101))  
					{
						int n = atoi(contentStrVec[index].c_str());
						sprintf(contentStr, n==0?"Off":"On");
					}
					else if((index == 91)||(index==92)) // IsActive, IsValidate
					{
						int n = atoi(contentStrVec[index].c_str());
						sprintf(contentStr, n==0?"No":"Yes");
					}
					else if(index == 94) // Weld Force Control
					{
						int n = atoi(contentStrVec[index].c_str());
						if(n == 1)
							sprintf(contentStr, "Low");
						else if(n == 2)
							sprintf(contentStr, "Medium");
						else if(n == 3)
							sprintf(contentStr, "High");
					}
				}
				
				if(m_reportType == ALARM_LOG)
				{
					if(index % COLUMNALARMLOG == COLUMNALARMLOG - 1) // AlarmID
					{
						sprintf(contentStr, AlarmNames::errorNameforAlarm(contentStrVec[index].c_str()).c_str());
					}
//					Saved for when the assembly numbers on the PDF alarm report are implemented.
//					if( ((index+1+1)%COLUMNALARMLOG == 0) || ((index+1+0)%COLUMNALARMLOG == 0) ) // Standardize NA to N/A
//					{
//						if(contentStrVec[index] == "NA")
//							sprintf(contentStr, "N/A");
//					}	
				}
				
				DrawPDFText(page, XCord, YCoordinate,contentStr);					
				memset(contentStr, 0, sizeof(contentStr));
				
				if((index+1) % columnNumber == 0)
				{
					XCord = PRINTWELDSTARTXCORD;
					YCoordinate += rowGap;
					if(m_reportType == SYSTEM_SETUP)
						XCord = 40;
				}
				else
				{
					if(m_reportType == WELDRESULT_TYPE)
					{
						/* Layout Data Time - Retract yy/MM/dd HH/mm/ss */
						if((index+1+26)%COLUMNWELDRESULT == 0)
							XCord += 30;
						else if((index+1+25)%COLUMNWELDRESULT == 0) 
							XCord += (PDFParamWidthWeld[0]+PDFParamWidthWeld[1] -30);
						else
							XCord += PDFParamWidthWeld[index%COLUMNWELDRESULT];

					}
					else if(m_reportType == EVENT_LOG)
						XCord += PDFParamWidthEvent[index%COLUMNEVENTLOG];
					else
						XCord += colGap;
				}
					
			}
						
		}
		else
		{
				LOGERR("ReportGeneration : ReadWeldResultFromDBToStorageDevice : Weld result logs read error from DB : %d",queryStatus,0,0); 
				status = DB_READ_ERROR;
		}							
	}

	else 
	{
		LOGERR("ReportGeneration : ReadWeldResultFromDBToStorageDevice : Record not found",0,0,0);  
		status = DB_READ_ERROR;
	}
			
	return status;
}

void PDFReportGeneration::SetTxtRGBColor(float r,float g, float b)
{
	R = r;
	G = g;
	B = b;
}

void PDFReportGeneration::GetDateAndTime(string &DateTime)
{
	RTCtimerStr SysTime;
	char temp[TIMESTAMP] = {0x00};
	GetRTC(&SysTime);
	
	SysTime.tm_year = 1900 + SysTime.tm_year;
	SysTime.tm_mon  = 1 + SysTime.tm_mon;
	sprintf(temp,"%d/%02d/%02d %02d:%02d:%02d",SysTime.tm_year,SysTime.tm_mon,SysTime.tm_mday,SysTime.tm_hour,SysTime.tm_min,SysTime.tm_sec);
	DateTime = temp;
}

const char *PDFReportGeneration::GetScreenStr(UINT16 EnumVal)
{
	const char *ScreenStr[]={"DASHBOARD","PRODUCTION","RECIPES","ANALYTICS"};
	
	return ScreenStr[EnumVal];
}

const char *PDFReportGeneration::GetLangugeStr(UINT16 EnumVal)
{
	const char *LanguageStr[] = {"ENGLISH", "FRENCH", "GERMAN", "SPANISH", "KOREAN", "TRADITIONAL_CHINESE", "SIMPLIFIED_CHINESE",
			"ITALIAN", "JAPANESE","SLOVAK", "DANISH"};

	return LanguageStr[EnumVal];
}

const char *PDFReportGeneration::GetPowerUpOptionStr(UINT16 EnumVal)
{
	const char* PowerUpOptionStr[]={"PS_SEEK","PS_SCAN","PS_NONE"};
	
	return PowerUpOptionStr[EnumVal];
}


/**************************************************************************//**
* \brief   - Convert the value stored in the EEPROM to real frequency value
*
* \param   - EEPROM enum value
*
* \return  - real frequency value
*
******************************************************************************/
UINT16 PDFReportGeneration::setFrequencyValue(UINT16 PsFreq)
{
	UINT16 FrequencyValue=20000;
	
	switch(PsFreq)
	{
	   case KHz_20:
		 FrequencyValue=20000; 		
		 break;		
	   case KHz_30:
		 FrequencyValue=30000;			
	     break;	
	   case KHz_40:
		 FrequencyValue=40000;	
		 break;		
	   default:
		 LOGERR("Invalid system frequency, Changing to default values - 20KHz",0,0,0);
		 FrequencyValue=20000; 
		 break;
	}
	
	return FrequencyValue;
}

STATUS PDFReportGeneration::createSuspectRejectLimits(HPDF_Doc * pdf,HPDF_Page * page, HPDF_Font &def_font,UINT32 pageNumber)
{
	enum {
		_recipeNum,
		_recipeVersion,
		_globalSuspect,
		_globalReject,
		_timeLimits,
		_energyLimits,
		_powerLimits,
		_absoluteLimits,
		_collapseLimits,
		_triggerLimits,
		_forceLimits,
		_frequencyLimits
	};
	enum {
		_suspectEnable,
		_suspectLowEnable,
		_suspectHighEnable,
		_suspectLowVal,
		_suspectHighVal,
		_rejectEnable,
		_rejectLowEnable,
		_rejectHighEnable,
		_rejectLowVal,
		_rejectHighVal,
	};
	
	vector<string> 	vLimits;
	vector<string> 	vRecipeValues;

	stringstream 	ssLimits;
	stringstream	ssRecipeValues;
	string 			details;
	
	char			buffer[MSG_SIZE * 4];
	int 			pos;
	int 			lineGap = 17;
			
	string tmpStr = "";
	vector<string> tmpStrVec;
	char tmpChar[MSG_SIZE * 4] = {0x00};
	char STR[64];
	INT32 queryStat = 0;
	INT16 status = OK;
	
	HPDF_Page_SetSize (*page, HPDF_PAGE_SIZE_A3, HPDF_PAGE_LANDSCAPE);

	HPDF_Page_SetFontAndSize (*page, def_font,25);
	
	strcpy(STR,"BRANSON------Recipe Setting - Limits");
	
	DrawPDFText(page,10,TWOWELDLINEGAP,STR);
	
	PdfWithImage(pdf, page, &def_font);
	
	/* Page Number */
	HPDF_Page_SetFontAndSize (*page, def_font,12);
	int x = (int)HPDF_Page_GetWidth(*page)-90;
	memset(STR,0,sizeof(STR));
	sprintf(STR,"Page: %d-1",pageNumber+1);
	DrawPDFText(page,x,TWOWELDLINEGAP+10,STR);
	
	
	YCoordinate = FOURWELDLINEGAP;
	XCord = PRINTWELDSTARTXCORD;
	HPDF_Page_SetFontAndSize(*page,def_font,10);
	
	
	memset(tmpChar,0x0,sizeof(tmpChar));
	snprintf(tmpChar, sizeof(tmpChar), "SELECT * FROM SuspectRejectLimits ORDER BY RecipeNumber ASC LIMIT 1 OFFSET %d;",pageNumber);
	tmpStr = m_DbConn->Exec(tmpChar, &queryStat);
	if(SQLITE_OK != queryStat)
	{
		status = FAILURE;
		LOGERR("ReportGeneration: Recipe Error",0,0,0);
		return status;
	}

	if(TRUE == tmpStr.empty())
	{
		status = FAILURE;
		LOGERR("ReportGeneration: Recipe Empty",0,0,0);
		return status;
	}
	

    // breakup incoming limits
	ssLimits.str (tmpStr);
	

	if (ssLimits.str().empty())
	{
		return ERROR;
	}
	
	// build
	while (getline (ssLimits, details,','))
		vLimits.push_back (details);
	
	// --- check globals enables
	snprintf (buffer, sizeof (buffer), "SUSPECT_GLOBAL_ENABLE - %s", vLimits[_globalSuspect]=="1"?"Yes":"No");  //123
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;			

	snprintf (buffer, sizeof (buffer), "REJECT_GLOBAL_ENABLE - %s",  vLimits[_globalReject]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	ssRecipeValues.clear();
	vRecipeValues.clear();
	
// --- check timeLimits
	ssRecipeValues << vLimits[_timeLimits]; 
	while (getline (ssRecipeValues, details,':'))			
		vRecipeValues.push_back (details);
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TIME_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");  //125
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_TIME_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
		
	snprintf (buffer, sizeof (buffer), "SUSPECT_TIME_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_TIME_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TIME_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_TIME_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TIME_LOW - %s", vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_TIME_LOW - %s",  vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TIME_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_TIME_HIGH - %s",  vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	ssRecipeValues.clear();
	vRecipeValues.clear();

	// --- check energyLimits		
	ssRecipeValues << vLimits[_energyLimits]; 
	
	while (getline (ssRecipeValues, details,':'))
		vRecipeValues.push_back (details);
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ENERGY_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ENERGY_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ENERGY_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ENERGY_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;

	snprintf (buffer, sizeof (buffer), "SUSPECT_ENERGY_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ENERGY_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ENERGY_LOW - %s",  vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ENERGY_LOW - %s", vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ENERGY_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_ENERGY_HIGH - %s", vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	ssRecipeValues.clear();
	vRecipeValues.clear();
	// --- check powerLimits
	ssRecipeValues << vLimits[_powerLimits]; 
	while (getline (ssRecipeValues, details,':'))
		vRecipeValues.push_back (details);		

	
	snprintf (buffer, sizeof (buffer), "SUSPECT_POWER_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_POWER_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_POWER_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
		
	snprintf (buffer, sizeof (buffer), "REJECT_POWER_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_POWER_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
		
	snprintf (buffer, sizeof (buffer), "REJECT_POWER_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_POWER_LOW - %s",  vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_POWER_LOW - %s", vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_POWER_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
		
	snprintf (buffer, sizeof (buffer), "REJECT_POWER_HIGH - %s", vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;

	ssRecipeValues.clear();
	vRecipeValues.clear();
// --- check powerLimits
	ssRecipeValues << vLimits[_absoluteLimits]; 

	while (getline (ssRecipeValues, details,':'))
		vRecipeValues.push_back (details);

	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ABSOLUTE_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ABSOLUTE_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ABSOLUTE_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ABSOLUTE_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ABSOLUTE_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ABSOLUTE_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");	
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ABSOLUTE_LOW - %s",  vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_ABSOLUTE_LOW - %s", vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_ABSOLUTE_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_ABSOLUTE_HIGH - %s", vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;

	ssRecipeValues.clear();
	vRecipeValues.clear();
	ssRecipeValues << vLimits[_collapseLimits]; 

	while (getline (ssRecipeValues, details,':'))
		vRecipeValues.push_back (details);

	
	snprintf (buffer, sizeof (buffer), "SUSPECT_COLLAPSE_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_COLLAPSE_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_COLLAPSE_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_COLLAPSE_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_COLLAPSE_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_COLLAPSE_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;

	snprintf (buffer, sizeof (buffer), "SUSPECT_COLLAPSE_LOW - %s",  vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_COLLAPSE_LOW - %s", vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_COLLAPSE_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_COLLAPSE_HIGH - %s", vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;

	ssRecipeValues.clear();
	vRecipeValues.clear();
	ssRecipeValues << vLimits[_triggerLimits]; 

	while (getline (ssRecipeValues, details,':'))
		vRecipeValues.push_back (details);
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TRIGGER_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_TRIGGER_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TRIGGER_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_TRIGGER_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
		
	snprintf (buffer, sizeof (buffer), "SUSPECT_TRIGGER_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_TRIGGER_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TRIGGER_LOW - %s",  vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_TRIGGER_LOW - %s", vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_TRIGGER_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
		
	snprintf (buffer, sizeof (buffer), "REJECT_TRIGGER_HIGH - %s", vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	ssRecipeValues.clear();
	vRecipeValues.clear();
	ssRecipeValues << vLimits[_forceLimits]; 

	while (getline (ssRecipeValues, details,':'))
		vRecipeValues.push_back (details);

	snprintf (buffer, sizeof (buffer), "SUSPECT_END_FORCE_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_END_FORCE_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_END_FORCE_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_END_FORCE_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_END_FORCE_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_END_FORCE_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_END_FORCE_LOW - %s",  vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_END_FORCE_LOW - %s", vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_END_FORCE_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_END_FORCE_HIGH - %s", vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;

	ssRecipeValues.clear();
	vRecipeValues.clear();
	ssRecipeValues << vLimits[_frequencyLimits]; 

	while (getline (ssRecipeValues, details,':'))
		vRecipeValues.push_back (details);

	snprintf (buffer, sizeof (buffer), "SUSPECT_FREQUENCY_ENABLE - %s",  vRecipeValues[_suspectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_FREQUENCY_ENABLE - %s",  vRecipeValues[_rejectEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_FREQUENCY_ENABLE_LOW - %s",  vRecipeValues[_suspectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_FREQUENCY_ENABLE_LOW - %s",  vRecipeValues[_rejectLowEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_FREQUENCY_ENABLE_HIGH - %s",  vRecipeValues[_suspectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_FREQUENCY_ENABLE_HIGH - %s",  vRecipeValues[_rejectHighEnable]=="1"?"Yes":"No");
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_FREQUENCY_LOW - %s",  vRecipeValues[_suspectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;
	
	snprintf (buffer, sizeof (buffer), "REJECT_FREQUENCY_LOW - %s", vRecipeValues[_rejectLowVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
	
	snprintf (buffer, sizeof (buffer), "SUSPECT_FREQUENCY_HIGH - %s", vRecipeValues[_suspectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	XCord = PRINTXCORDCENTER;

	snprintf (buffer, sizeof (buffer), "REJECT_FREQUENCY_HIGH - %s", vRecipeValues[_rejectHighVal].c_str());
	DrawPDFText(page, XCord, YCoordinate,buffer);
	YCoordinate +=lineGap;
	XCord = PRINTWELDSTARTXCORD;
		
	return OK;
}

#ifdef PDF_GENERATION
/**************************************************************************//**
 * \brief   - Default Constructor
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
PDFReportGeneration::PDFReportGeneration()
{
}

/**************************************************************************//**
 * \brief   - Default Destructor
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
PDFReportGeneration::~PDFReportGeneration()
{
}

/**************************************************************************//**
 * \brief   - Checks the Disk storage Space. TODO 
 *
 * \param   - None
 *
 * \return  - STATUS
 *
 ******************************************************************************/
STATUS PDFReportGeneration::CheckDiskStorageSpace()
{
	/* Implementation to do later */
	return RESPONSE_SUCCESS ;
}


/**************************************************************************//**
* \brief   - Initializes the image and font objects in the PDF writer library
* 			  that is used to generate the report
*
* \param   - None
*
* \return  - STATUS
*
******************************************************************************/
STATUS PDFReportGeneration::InitializeImgFontObjects()
{
	PDFFormXObject* image = NULL ;
	PDFUsedFont* font = NULL ;
	string imageFileName , fontFileName ;

	INT8 retStatus = OK ;

	/* Currently since only single font and image is used, we use a string, later vector can be used to support multiple strings and images */
	imageFileName.append(BRANSON_LOGO_STRING);
	fontFileName.append(COURIER_STRING);

	//LOGDBG("PDF_REPORT : Image and Font name is %s, %s",imageFileName.c_str(),fontFileName.c_str(),0); 

	image = pdfWriter.CreateFormXObjectFromJPGFile(PDF_IMAGE_PATH + imageFileName);
	if( 0 == image)
	{
		retStatus = FAILURE;
	}

	if(OK == retStatus)
	{
		font = pdfWriter.GetFontForFile(PDF_FONT_PATH + fontFileName);
		if(0 == font)
		{
			retStatus = FAILURE ;
		}

	}

	if(OK == retStatus)
	{
		/* Append the font and the image into the Map tables */
		m_imageFiles[BRANSON_LOGO] = image ;
		m_fontFiles[COURIER] = font ;
		LOGDBG("PDF_REPORT : Image and Font objects for PDF report Generation created successfully",0,0,0);
	}

	return retStatus ;
}

/**************************************************************************//**
 * \brief   - Create the PDF page with standard document header and footer info
 * 			  along with logo and title
 *
 * \param   - None
 *
 * \return  - STATUS - OK or FAILURE
 *
 ******************************************************************************/
STATUS PDFReportGeneration :: CreatePDFPage()

{
	PDFPage* pdfPage = NULL ;
	INT8 retStatus = OK ;
	/* Create a new page, store the content context, Insert document Header Information */
	pdfPage = new PDFPage() ;
	if(0 == pdfPage)
	{
		LOGERR("PDFReportGeneration: CreatePDFPage: Pdf Page memory allocation failed",0,0,0);
		retStatus = FAILURE ;
	}
	else
	{
		pdfPage->SetMediaBox(PDFRectangle(0,0,A4_WIDTH,A4_HEIGHT));//landscape mode of pdf

		m_pdfPageContentContext = pdfWriter.StartPageContentContext(pdfPage);
		
		if(NULL == m_pdfPageContentContext)
		{
			retStatus = FAILURE ;
		}

		if(OK == retStatus)
		{
			/* Insert the logo, title, separators and report header */
			retStatus = InsertDocumentHeaderInformation();
		}

		if(OK == retStatus)
		{
			/* Insert rectangle and write the report header information */
				retStatus = FillReportHeaderInformation();
		}
	}		
			
	return retStatus ;

/**************************************************************************//**
* \brief   - Inserts the document header information. Logo and the 
* 			  line separators
*
* \param   - None
*
 * \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS PDFReportGeneration::InsertDocumentHeaderInformation()
{
PDFFormXObject* image = m_imageFiles[BRANSON_LOGO] ;
PDFUsedFont* font = m_fontFiles[COURIER] ;
PDFPage* pdfPage  = NULL;
INT8 retStatus = OK ;

pdfPage = m_pdfPageContentContext->GetAssociatedPage();

if(NULL == pdfPage)
{
LOGERR("ReportGeneration : InsertDocumentHeaderInformation: Error in retrieving the PDF file",0,0,0);
retStatus = FAILURE ;
}
else
{
/* Insert the branson logo */
m_pdfPageContentContext->q();
m_pdfPageContentContext->K(0,0,0,1);
m_pdfPageContentContext->cm(0.7,0,0,0.7, (0.8 * A4_WIDTH) , (0.95 * A4_HEIGHT));
m_pdfPageContentContext->Do(pdfPage->GetResourcesDictionary().AddFormXObjectMapping(image->GetObjectID()));
m_pdfPageContentContext->Q();

/* Insert Title information */
m_pdfPageContentContext->BT();
m_pdfPageContentContext->k(0,0,0,1);//this is being used for color chnahge of your text
m_pdfPageContentContext->Tf(font,TITLE_STR_FONT_SIZE);//size of your text 
//first argument for text size changes second for rotation of text

m_pdfPageContentContext->Tm(1,0,0,1,(0.02 * A4_WIDTH) , (0.95 * A4_HEIGHT));

switch(m_reportType)
{
case WELDRESULT_TYPE:
		m_pdfPageContentContext->Tj("Weld Data");
		break ;

	case WELDSIGNATURE_TYPE:
		m_pdfPageContentContext->Tj("Graph Data");
		break;

	default:
		/* Is this the right defense mechanism? */
		m_pdfPageContentContext->Tj("Incorrect Data");
		break;
	}

	m_pdfPageContentContext->ET();

	/* Draw Horizontal line */
	m_pdfPageContentContext->q();
	m_pdfPageContentContext->w(1.6);
	m_pdfPageContentContext->K(0,0,0,1);
	m_pdfPageContentContext->m(2,A4_HEIGHT - 40);
	m_pdfPageContentContext->l(A4_WIDTH - 4 , A4_HEIGHT - 40);
	m_pdfPageContentContext->s();
	m_pdfPageContentContext->Q();

	/* Update the spaceleftout in column information */
	spaceLeftOutToWriteRecords = A4_HEIGHT - 60 ;


	/* Draw horizontal line at the end */
	m_pdfPageContentContext->q();
	m_pdfPageContentContext->w(1.6);
	m_pdfPageContentContext->K(0,0,0,1);
	m_pdfPageContentContext->m(2,8);
	m_pdfPageContentContext->l(A4_WIDTH - 4 , 8);
	m_pdfPageContentContext->s();
	m_pdfPageContentContext->Q();		
}

return retStatus ;
}

/**************************************************************************//**
* \brief   - Insert the recipe table information
*
* \param   - None
*
 * \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS PDFReportGeneration::FillRecipeInformationTable(INT32 a_recipeNum , INT32 a_recipeVerNum)
{
char selectQuery[MSG_SIZE * 4] = {0x00};
char recipeDataBuff[256] = {0x00};
string dbExecQueryStr ;
vector<string> dbExecQueryVec;
INT32 queryStatus = 0 ;
string trigForce, recipeName, recipeVerNum, ampStepsCnt, numForceSteps, isValidate;
string recipeHeaderStr, recipeDataStr, recipeAssemStr, recipeSerNumStr, tmpStr ;
UINT8 recipeBoxColCount = 0 ;
STATUS retStatus = 0 ;
INT16 length = 0 ;
INT16 index = 0 ;

/* Retrieve the recipe info from the table */
if(OK == retStatus)
{
	snprintf(SelectQuery,sizeof(SelectQuery), "SELECT WeldRecipeVerNumber,TriggerForce,AmplitudeStepsCnt,NumForceSteps,IsValidate,WeldRecipeName FROM WeldRecipeTable WHERE RecipeNumber=%d",a_recipeNum);
	dbExecQueryStr = m_DbConn->Exec(selectQuery, queryStatus);

	printf("\nThe select query returned %s\n",dbExecQueryStr.c_str());
	
	if(0 != queryStatus)
	{
		LOGERR("PDFReportGeneration: FillRecipeInfoTable: Failed to retrieve Weld Recipe Data",0,0,0);
		retStatus = FAILURE ;
	}
	
	SplittingData(dbExecQueryStr,dbExecQueryVec,',');
		
		if(6 != dbExecQueryVec.size() )
		{
			LOGERR("PDFReportGeneration: FillRecipeInfoTable: Failed to retrieve Weld Recipe Data",0,0,0);
			retStatus = FAILURE ;
		}
		else
		{
			recipeVerNum = dbExecQueryVec[0];
			trigForce = dbExecQueryVec[1];
			ampStepsCnt = dbExecQueryVec[2];
			numForceSteps = dbExecQueryVec[3];
			isValidate = dbExecQueryVec[4];
			recipeName = dbExecQueryVec[5];
		}
	}

	/* Insertion of the recipe information */
	/* Insertion of the preset header */
	sprintf(recipeDataBuff, "User ID, Recipe #, Recipe rev:%s,%d,%s,%s",recipeName.c_str(),a_recipeNum,recipeVerNum.c_str(),(atoi(isValidate.c_str()) == 1?"Validated":"NonValidated"));
	recipeHeaderStr.append(recipeDataBuff);
	recipeBoxColCount++;

	length = atoi(ampStepsCnt.c_str());
	for(index = 1 ; index <= length ; index ++)
	{
		snprintf(SelectQuery,sizeof(SelectQuery), "SELECT AmplitudeStep%d FROM WeldRecipeTable WHERE RecipeNumber=%d",index,a_recipeNum);
		dbExecQueryStr = m_DbConn->Exec(selectQuery, queryStatus);

		if(0 != queryStatus)
		{
			LOGERR("PDFReportGeneration: FillRecipeInfoTable: Failed to retrieve Amp Stepping Values",0,0,0);
			retStatus = FAILURE ;
			
			break;
		}

		tmpStr = m_stdUnitMap["Amplitude"];
		sprintf(recipeDataBuff,"Set Amp %d=%s(%s),",index , dbExecQueryStr.c_str(),tmpStr.c_str());
		recipeDataStr.append(recipeDataBuff);
	}

	length = atoi(numForceSteps.c_str());
	for(index = 1 ; index <= length ; index++)
	{
		snprintf(SelectQuery,sizeof(SelectQuery), "SELECT ForceStep%d FROM WeldRecipeTable WHERE RecipeNumber=%d",index,a_recipeNum);
		dbExecQueryStr = m_DbConn->Exec(selectQuery, queryStatus);

		if(0 != queryStatus)
		{
			LOGERR("PDFReportGeneration: FillRecipeInfoTable: Failed to retrieve Amp Stepping Values",0,0,0);
			retStatus = FAILURE ;

			break;
		}

		tmpStr = m_stdUnitMap["MaxWeldForce"];
		sprintf(recipeDataBuff,"Set Force %d=%s(%s), ",index , dbExecQueryStr.c_str(), tmpStr.c_str());
		recipeDataStr.append(recipeDataBuff);
	}
	
	tmpStr = m_stdUnitMap["MaxWeldForce"];
	sprintf(recipeDataBuff,"Trigger Force=%s(%s)",trigForce.c_str(), tmpStr.c_str());
	recipeDataStr.append(recipeDataBuff);
	recipeBoxColCount++;
	
	sprintf(recipeDataBuff,"Act Assem#, P/S Assem#, Stack Assem#:%s","DEFAULT, DEFAULT, DEFAULT");
	recipeAssemStr.append(recipeDataBuff);
	recipeBoxColCount++;
	
	sprintf(recipeDataBuff,"PS Serial Number, Act Serial Number:%s","DEF1234, BUC052015");
	recipeSerNumStr.append(recipeDataBuff);
	recipeBoxColCount++;
	
	recipeBoxColCount++;
	
	if(recipeBoxColCount * MIN_SIZE_TO_ADD_RECORD > (spaceLeftOutToWriteRecords - MIN_SIZE_TO_ADD_RECORD))
	{
		EndPDFPage();
		
		CreatePDFPage();
	}
	
	m_pdfPageContentContext->BT();

	/* Insertion of Recipe Information */
	spaceLeftOutToWriteRecords -= MIN_SIZE_TO_ADD_RECORD;	
	m_pdfPageContentContext->k(2,1,0,1);
	m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
	m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[0],spaceLeftOutToWriteRecords);
	m_pdfPageContentContext->TL(10);
	m_pdfPageContentContext->Tj(recipeHeaderStr);
	spaceLeftOutToWriteRecords -= MIN_SIZE_TO_ADD_RECORD ;
	
	m_pdfPageContentContext->k(2,1,0,1);
	m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
	m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[0],spaceLeftOutToWriteRecords);
	m_pdfPageContentContext->TL(10);
	m_pdfPageContentContext->Tj(recipeDataStr);
	spaceLeftOutToWriteRecords -= MIN_SIZE_TO_ADD_RECORD ;
	
	m_pdfPageContentContext->k(2,1,0,1);
	m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
	m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[0],spaceLeftOutToWriteRecords);
	m_pdfPageContentContext->TL(10);
	m_pdfPageContentContext->Tj(recipeAssemStr);
	spaceLeftOutToWriteRecords -= MIN_SIZE_TO_ADD_RECORD ;
	
	m_pdfPageContentContext->k(2,1,0,1);
	m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
	m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[0],spaceLeftOutToWriteRecords);
	m_pdfPageContentContext->TL(10);
	m_pdfPageContentContext->Tj(recipeSerNumStr);
	spaceLeftOutToWriteRecords -= MIN_SIZE_TO_ADD_RECORD ;
	
	/* Draw the gray rectangle border around the Report Header String */
	//To draw the rectangle
	m_pdfPageContentContext->q();
	m_pdfPageContentContext->w(2);
	m_pdfPageContentContext->k(0,0,0,1);

	m_pdfPageContentContext->re(2, spaceLeftOutToWriteRecords, (A4_WIDTH - (A4_WIDTH/2) - 40), (recipeBoxColCount * 11));
	m_pdfPageContentContext->s();
	m_pdfPageContentContext->Q();

	//To color the rectangle in gray
	m_pdfPageContentContext->q();
	m_pdfPageContentContext->k(0,0,0,0.3);

	m_pdfPageContentContext->re(3.5, (spaceLeftOutToWriteRecords+1.5), (A4_WIDTH - (A4_WIDTH/2) - 40-(1.5*2)), ((recipeBoxColCount * 11)-(1.5*2)));
	m_pdfPageContentContext->f();
	m_pdfPageContentContext->Q();
	
	spaceLeftOutToWriteRecords -= MIN_SIZE_TO_ADD_RECORD;
	
	m_pdfPageContentContext->ET();

	return retStatus ;	
}

/**************************************************************************//**
 * \brief   - Insert the recipe weld mode information
 *
 * \param   - None
 *
 * \return  - STATUS - OK or FAILURE
 *
 ******************************************************************************/
STATUS PDFReportGeneration::FillRecipeWeldMode(INT32 a_recipeNum , INT32 a_recipeVerNum)
{
	char selectQuery[MSG_SIZE * 4] = {0x00};
	char recipeDataBuff[256] = {0x00};
	string weldModeStr , recipeNameStr, recipeWeldModeStr ;
	INT32 queryStatus = 0 ;
	INT16 weldModeVal = 0 ;
	STATUS retStatus = 0 ;
	INT16 length = 0 ;
	INT16 index = 0 ;
	
	/* Retrieve the weld mode info from the table */
	if(OK == retStatus)
	{
		snprintf(SelectQuery,sizeof(SelectQuery), "SELECT WeldMode FROM WeldRecipeTable WHERE RecipeNumber=%d",a_recipeNum);
		weldModeStr = m_DbConn->Exec(selectQuery, queryStatus);
		
		if(0 != queryStatus)
		{
			LOGERR("PDFReportGeneration: FillRecipeWeldModeInfo: Failed to retrieve Weld Mode Data",0,0,0);
			retStatus = FAILURE ;
		}
		else
		{
			weldModeVal = atoi(weldModeStr.c_str());
		}
	}
	
	if(OK == retStatus)
	{
		snprintf(SelectQuery,sizeof(SelectQuery), "SELECT WeldRecipeName FROM WeldRecipeTable WHERE RecipeNumber=%d",a_recipeNum);
		recipeNameStr = m_DbConn->Exec(selectQuery, queryStatus);
		
		if(0 != queryStatus)
		{
			LOGERR("PDFReportGeneration: FillRecipeWeldModeInfo: Failed to retrieve Weld Mode Data",0,0,0);
			retStatus = FAILURE ;
		}
		else
		{
			switch(weldModeVal)
			{
			case TIMEMODE:
				sprintf(recipeDataBuff,"TimeMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			case ENERGYMODE:
				sprintf(recipeDataBuff,"EnergyMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			case PEAKPOWERMODE:
				sprintf(recipeDataBuff,"PeakPowerMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			case GRDDETECTMODE:
				sprintf(recipeDataBuff,"GroundDetectMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			case ABSMODE:
				sprintf(recipeDataBuff,"AbsoluteMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			case COLLAPSEMODE:
				sprintf(recipeDataBuff,"CollapseMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			case CONTINUOUS:
				sprintf(recipeDataBuff,"ContinuousMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			case MULTI:
				sprintf(recipeDataBuff,"MultiMode, Recipe# %d - %s",a_recipeNum,recipeNameStr.c_str());
				break;
				
			default:
				retStatus = FAILURE;
				break;
										
			
			}

		}

	}


	/* Insertion of the recipe information */	
	/* Insertion of the Recipe Weld Mode */
	recipeWeldModeStr.append(recipeDataBuff);

	if(spaceLeftOutToWriteRecords < MIN_SIZE_TO_ADD_RECORD)
	{
		EndPDFPage();
		CreatePDFPage();
	}	

	m_pdfPageContentContext->BT();
	/* Insertion of Recipe Information */
	m_pdfPageContentContext->k(2,1,0,1);
	m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
	m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[0],spaceLeftOutToWriteRecords);
	m_pdfPageContentContext->TL(10);
	m_pdfPageContentContext->Tj(recipeWeldModeStr);

	m_pdfPageContentContext->ET();
	spaceLeftOutToWriteRecords -= MIN_SIZE_TO_ADD_RECORD ;

	return retStatus ;
}

/**************************************************************************//**
* \brief   - Write the DB Record into USB.
*
* \param   - string& DBRecord
*
* \return  - STATUS 
*
******************************************************************************/
STATUS PDFReportGeneration::FillReportHeaderInformation()
{
	string reportStr ;
	UINT8 index = 0 ;
	UINT8 ind = 0 ;
	vector<string> reportStrVec ;
	string columnKeyStr  ;
	vector<string> columnKeyVec;
	string tmpStr ;
	float lengthOfColumn ;
	BOOL titleNotAdded = 0 ;
	UINT8 foundIndex = 0 ;
	INT8 retStatus = OK ;
	UINT8 maxNumberOfColumns = 1;
	UINT8 countOfColumns = 1;
	UINT8 j = 0;
	UINT8 columnLength = 0 ;
	string unitStr ;
	vector<string> unitStrVec ;	

	/* Retrieve the report header string */
	reportStr = ReadReportHeader();

	if(1 == reportStr.empty())
	{
		retStatus = FAILURE ;
	}
	else
	{
		/* Split the string to individual keys */
		SplittingData(reportStr , reportStrVec , ',');

		/* Check for integrity of the keys */
		if(reportStrVec.size() != tableColumnCount)
		{
			/* Incorrect number of keys present, the program has to exit here */
			retStatus = FAILURE ;
		}

	}
	
	if(OK == retStatus)	
	{
	
		for(index = 0 ; index < reportStrVec.size(); index++)
		{
			columnKeyStr = reportStrVec[index];

			/* Split the key according to upper case and populate the vector */
			for(ind = 0 ; ind <= columnKeyStr.length() ; ind++)
			{
				if((columnKeyStr[ind] == '\0') || ((0 != ind) && (0 != isupper(columnKeyStr[ind]))))
				{
					columnKeyVec.push_back(tmpStr);
					tmpStr.clear();
				}
				tmpStr += columnKeyStr[ind] ;
			}
			/* Reinitializing the values to accomodate the string */
			tmpStr.clear();
			titleNotAdded = 1 ;
			countOfColumns  = 1;
			columnLength = 0;

			/* Calculate the size of the column that is configured */
			lengthOfColumn = tableSpacing[index + 1] - tableSpacing[index] ;

			/* Insert the string onto the header */
			m_pdfPageContentContext->BT();
			m_pdfPageContentContext->k(2,1,0,1);
			m_pdfPageContentContext->Tf((PDFUsedFont*)(m_fontFiles[COURIER]),TABLE_STR_FONT_SIZE);
			m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[index],spaceLeftOutToWriteRecords);
			m_pdfPageContentContext->TL(8);

			for(j = 0 ; j< columnKeyVec.size() ; j++)
			{
				tmpStr.append(columnKeyVec[j]);
				columnLength += (tmpStr.length() - 1) ;

				if( (j == (columnKeyVec.size() -1)) || ((columnLength + (columnKeyVec[j+1].length() - 1)) > (lengthOfColumn - 2)/TABLE_STR_FONT_SIZE))
				{
					/* We first add the title string */
					if(1 == titleNotAdded)
					{
						m_pdfPageContentContext->Tj(tmpStr);
						titleNotAdded = 0 ;
					}
					/* Post adding the title, the strings will be quoted to the next line */
					else
					{
						m_pdfPageContentContext->k(0,0,0,1);
						m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
						m_pdfPageContentContext->Quote(tmpStr);//value of each key

						countOfColumns++ ;
					}
					columnLength = 0;
					tmpStr.clear();
				}
			}
			columnKeyVec.clear();
			tmpStr.clear();
			maxNumberOfColumns = max(maxNumberOfColumns , countOfColumns) ;	
		}
		
		/* Entering the units fields */
		for(index = 0 ; index < reportStrVec.size() ; index++)
		{
			tmpStr = reportStrVec[index] ;
			foundIndex = m_stdUnitMap.count(tmpStr) ;
			if(1 == foundIndex)
			{
				//LockTask();
				m_pdfPageContentContext->k(2,1,0,1);
				m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
				m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[index],spaceLeftOutToWriteRecords - ( TABLE_STR_FONT_SIZE * maxNumberOfColumns ));
				m_pdfPageContentContext->TL(10);
				m_pdfPageContentContext->Tj("("+m_stdUnitMap[tmpStr]+")");
				//UnlockTask();				
			}
			tmpStr.clear();
		}
		m_pdfPageContentContext->ET();
		
		maxNumberOfColumns++ ;
		maxNumberOfColumns++;
		spaceLeftOutToWriteRecords -= maxNumberOfColumns * ( TABLE_STR_FONT_SIZE + 2 ) ;

		/* Draw the gray rectangle border around the Report Header String */
		//To draw the rectangle
		m_pdfPageContentContext->q();
		m_pdfPageContentContext->w(2);
		m_pdfPageContentContext->k(0,0,0,1);

		m_pdfPageContentContext->re(2, (spaceLeftOutToWriteRecords + ( TABLE_STR_FONT_SIZE + 2 )) , A4_WIDTH - 4, maxNumberOfColumns * (TABLE_STR_FONT_SIZE + 2));
		m_pdfPageContentContext->s();
		m_pdfPageContentContext->Q();

		//To color the rectangle in gray
		m_pdfPageContentContext->q();
		m_pdfPageContentContext->k(0,0,0,0.3);

		m_pdfPageContentContext->re(3.5,(spaceLeftOutToWriteRecords + ( TABLE_STR_FONT_SIZE + 2 ) + 1.5) , A4_WIDTH - 4 - (1.5 * 2) , maxNumberOfColumns * (TABLE_STR_FONT_SIZE + 2)  - (1.5 * 2));
		m_pdfPageContentContext->f();
		m_pdfPageContentContext->Q();

	}
	return retStatus ;	
}

/**************************************************************************//**
 * \brief   - Finalize the given PDF page.
 *
 * \param   - None
 *
 * \return  - STATUS
 *
 ******************************************************************************/
STATUS PDFReportGeneration::EndPDFPage()
{
	INT8 retStatus = 0 ;
	PDFPage* pdfPage ;

	/* End the previous page here*/
	if(NULL == m_pdfPageContentContext)
	{

		/* Write a return status val indicating there is no page context to End */
		retStatus = FAILURE ;
	}
	else
	{

		pdfPage = m_pdfPageContentContext->GetAssociatedPage();

		retStatus = pdfWriter.EndPageContentContext(m_pdfPageContentContext);

		if(OK == retStatus)
		{
			retStatus = pdfWriter.WritePageAndRelease(pdfPage); 
		}

		/* Reset the content context information as the page is written and released */
		m_pdfPageContentContext = NULL ;
	}
	
	return retStatus ;
}

/**************************************************************************//**
* \brief   - Delete the WeldResult records from DB.
*
* \param   - None
*
* \return  - UINT32 
*
******************************************************************************/
STATUS PDFReportGeneration::WriteDBRecordToUSB(string& a_dbBuffer)
{
	PDFPage* pdfPage ;
	string wordStr ;
	UINT8 columnInd = 0;
	vector<string> dbDataVec ;
	INT8 retStatus = 0 ;
	INT8 status = 0 ;
	UINT32 index = 0 ;
	
	UINT32 partialDataIndex = (UINT32)-1;
	INT8 partialDataFlag = FALSE ;
	
	/* Data split to vector */
	SplittingData(a_dbBuffer , dbDataVec , ',');
	if(0 != (dbDataVec.size() % tableColumnCount))
	{
		partialDataFlag = TRUE ;
		partialDataIndex = dbDataVec.size() / tableColumnCount ;
		retStatus = DATA_NOT_COMPLETE ;
	}

	/* Check if the string is report header or data type */	
	while((index  < dbDataVec.size()) && (partialDataIndex > 0))
	{
		if( NULL == m_pdfPageContentContext )
		{
			status = CreatePDFPage();
			if(OK != status)
			{
				retStatus = FAILURE ;
				break ;
			}
		}

		/* Continuously add each record information */
		wordStr = dbDataVec[index];

		m_pdfPageContentContext->BT();
		/*Checking for alarm data*/
		if(m_alarmFlag)
		{
			m_pdfPageContentContext->rg(1,0,0); /*Writing in red colour to indicate Alarm data*/
		}
		else
		{
			m_pdfPageContentContext->k(2,1,0,1);/*Black colour to indicate Weld data*/
		}
		m_pdfPageContentContext->Tf(m_fontFiles[COURIER],TABLE_STR_FONT_SIZE);
		m_pdfPageContentContext->Tm(1,0,0,1,tableSpacing[columnInd],spaceLeftOutToWriteRecords);
		m_pdfPageContentContext->TL(10);
		m_pdfPageContentContext->Tj(wordStr);
		m_pdfPageContentContext->ET() ;

		columnInd++ ;
		if(0 == (columnInd %= tableColumnCount))
		{
			if(TRUE == partialDataFlag)
			{
				partialDataIndex-- ;
			}
			
			spaceLeftOutToWriteRecords -= SIZE_FOR_EACH_ROW ;
		}
		if(MIN_SIZE_TO_ADD_RECORD > spaceLeftOutToWriteRecords)
		{
			status = EndPDFPage();
			if(OK != status)
			{
				retStatus = FAILURE ;
				break ;
			}
			
		}
		index++ ;
	}
	
	m_alarmFlag=0;
	return retStatus ;
}


/**************************************************************************//**
 * \brief   - Initialize the configuration data to insert DB records into table
 *
 * \param   - None
 *
 * \return  - STATUS - OK or FAILURE
 *
 ******************************************************************************/
STATUS PDFReportGeneration::InitializeTableConfigData()
{
	INT8 retStatus = 0 ;
	string headerStr = ReadReportHeader();
	vector<string> strWords ;
	string wordStr ;
	UINT8 index = 0 ;
	UINT8 countLongString = 0 ;
	UINT8 countLongLongString = 0 ;
	UINT64 bitPosLongString = 0 ;
	UINT32 posCalculator = 1 ;
	string tmpStr ;
	double d = 0 ;
	double diffVal = 0 ;

	tableColumnCount = GetColumnCount() ;
	spaceLeftOutToWriteRecords = A4_HEIGHT ;

	/* Split the key values */
	SplittingData(headerStr , strWords , ',');

	if(tableColumnCount != strWords.size())
	{
		LOGERR("PDFReportGeneration : InitializeTableConfigData: Table Count does not match the string words size. Table Size = %u, String Words Size: %u",tableColumnCount , strWords.size(),0);
		retStatus = FAILURE; 
	}
	else 
	{

		/* Determine the count of the probable long strings
		 * For this those strings with "time" or "name" will be considered long 
		 * Calculate the positions of the long string and encode it in bit position */
		index = 0 ;
		while(index < strWords.size())
		{
			wordStr = strWords[index];
			tmpStr = ConvertStringToLower(wordStr);

			/* TODO: For now we consider only datetime as a long string, rest all to take care later when the whole report column structure is 
			 * finalised */
			if((strstr(tmpStr.c_str() , "datetime")))
			{
				if(strstr(tmpStr.c_str() , "datetime"))
				{
					/* String has been found. Set the bit position and increment the count of long strings */
					bitPosLongString |= (3 << (2 * index));
					countLongLongString++ ;
				}
				else
				{
					/* String has been found. Set the bit position and increment the count of long strings */
					bitPosLongString |= (2 << (2 * index));
					countLongString++ ;
				}

			}
			index++ ;
		}

		/* Initialize the table spacing parameters. The last spacing will be redundant */
		tableSpacing = (double*)malloc(tableColumnCount * sizeof(double));
		if(NULL == tableSpacing)
		{
			LOGERR("PDFReportGeneration : InitializeTableConfigData: Memory allocation failure",0,0,0);
			retStatus = FAILURE ;
		}
		else
		{
			d = (double)( A4_WIDTH - 2 * TABLE_OFFSET_LENGTH ) / (double)tableColumnCount ;

			diffVal = (double)(0.5 * countLongString + countLongLongString)  / (double)( tableColumnCount - (countLongLongString + countLongString) ) ;

			/* The first column space initialized to the the table offset length */
			tableSpacing[0] = TABLE_OFFSET_LENGTH ;
			for(index = 0 ; index < (tableColumnCount - 1) ; index++)
			{
				if(3 == ((bitPosLongString >> (2 * index)) & 3))
				{
					/* For Long strings we will initialize the spacing at 200 % */
					tableSpacing[index+1] = tableSpacing[index] + d * (1 + 1) ;
				}
				else if(2 == ((bitPosLongString >> (2 * index)) & 3))
				{
					/* For Long strings we will initialize the spacing at 150 % */
					tableSpacing[index+1] = tableSpacing[index] + d * (1 + 0.5) ;
				}
				else
				{
					/* The short string values are equally reduced in length */
					tableSpacing[index + 1] = tableSpacing[index] + d * (1 - diffVal);
				}
			}
		}

	}
	
	return retStatus;
}

/**************************************************************************//**
 * \brief   - Inherited function to generate the PDF report
*
* \param   - None
*
 * \return  - STATUS - OK or FAILURE
*
******************************************************************************/
STATUS PDFReportGeneration::GenerateReport(UINT8 a_checkStatusCmd)
{
	string fileNameString, filePathString ;
	INT8 retStatus = REPORT_GENERATE_SUCCESS ;
	/* Status - Return value of functions used in PDF lib. RetStatus = Report generation return status */
	INT8 status = OK ;
	UINT32 totalRecordsThatCanBeAllocated = 0 ;
	UINT32 totalRecordsInDb = 0 ;
	string dataReadFromDB ;
	UINT32 deletedRecordCount = 0;
	DIR *reportDir=NULL;

	/* Check the USB Device */
	status = CheckStorageDevice();	
	if(OK != status)
	{
		retStatus = USB_STORAGE_NOT_PRESENT ;
	}
	else
	{
		reportDir=opendir(REPORT_GEN_DIRECTORY_PATH);

		/*Creating new Directory for storing Report Generation Files*/
		if(NULL==reportDir)
		{
			if((ERROR==mkdir(REPORT_GEN_DIRECTORY_PATH, 0644)))
			{
				LOGDBG("PDFReportGeneration : GenerateReport: Error in Creating Report File Directory",0,0,0);
				status = FAILURE ;
			}	
		}
		
		if(OK != status)
		{
			retStatus = REPORT_GENERATION_FAILED;
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: Check storage device is successful",0,0,0);
			/* Create the filename */
			fileNameString = ReportNameGeneration();

			/* Start the PDF file and check status */
			status = pdfWriter.StartPDF(fileNameString , ePDFVersion13 , LogConfiguration::DefaultLogConfiguration() , PDFCreationSettings(true,true));		
		}
		
		if(OK != status)
		{
			retStatus = REPORT_GENERATION_FAILED ;		
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: Starting PDF report generation is successful",0,0,0);
			/* Create the image and font objects required for PDF */
			status = InitializeImgFontObjects();
		}

		if(OK != status)
		{
			retStatus = REPORT_GENERATION_FAILED ;
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: Initializing image and font objects is successful",0,0,0);
			status = InitializeTableConfigData() ;
		}

		/* Initialize the configurations of the table data */
		if(OK != status)
		{
			retStatus = REPORT_GENERATION_FAILED ;
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: Initialize table configuration is successful",0,0,0);
			/* TODO: check Memory storage space */
			CheckDiskStorageSpace();

			/* Check if all records can be allocated into the space */
			/* TBD later: as this design is not clear as of now */
			/*  Start PDF Page */
			status = CreatePDFPage() ;
		}
		
		if(TRUE != ((OK == status) || (DATA_NOT_COMPLETE == status)))
		{
			retStatus = REPORT_GENERATION_FAILED ;
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: Creation of initial PDF page is successful",0,0,0);
			
			/* Read the Records from DB */
			switch(m_reportType)
			{
			case WELDRESULT_TYPE:
				status = ReadWeldResultFromDBToStorageDevice();
				break;

			case WELDSIGNATURE_TYPE:
				/* TODO: Weld Signature report generation not supported for the current release */
//				status = FAILURE ;
				status = ReadWeldSignatureFromDBToStorageDevice();
				break;
			}

		}

		if(OK != status)
		{
			retStatus = REPORT_GENERATION_FAILED ;
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: Writing the DB data is successful",0,0,0);
			/* Forcefully end the page */
			status = EndPDFPage() ;
		}
		
		if(OK != status)
		{
			retStatus = REPORT_GENERATION_FAILED;
		}
		else
		{

			/* End the PDF file */
			status = pdfWriter.EndPDF();
		}


		/* Delete the DB entry for the data written */	
		if(OK != status)
		{
			retStatus = REPORT_GENERATION_FAILED ;
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: Complete and finalizing the PDF file is successful",0,0,0);
			switch(m_reportType)
			{
				case WELDRESULT_TYPE:
							deletedRecordCount=DeleteWeldResultFromDB();
							retStatus=REPORT_GENERATE_SUCCESS;
							break;
				case WELDSIGNATURE_TYPE:
							deletedRecordCount=DeleteWeldSignatureFromDB();
							retStatus=REPORT_GENERATE_SUCCESS;
				break;
			}
			
			LOGDBG("PDFReportGeneration : GenerateReport: Reported is generated for %u out of %u Records",deletedRecordCount,RecordCount,0);
					LastRecipeNum=0, LastCycleCounter=0, RecordCount=0;
				
		}
		/* Delete the DB file generated in case the operation fails */
		if(REPORT_GENERATE_SUCCESS != retStatus)	
		{					
			LOGERR("PDFReportGeneration : GenerateReport: PDF generation of report failed",0,0,0);			
			if(OK != rm(fileNameString.c_str()))
			{
				LOGERR("PDFReportGeneration : GenerateReport: PDF file is corrupted and could not be deleted",0,0,0);
			}
			
		}
		else
		{
			LOGDBG("PDFReportGeneration : GenerateReport: PDF generation of report is successful",0,0,0);
		}
			
	}
	return retStatus ;
}




#endif /* PDF_GENERATION */
