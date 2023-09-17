/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

/*------------------------------ DESCRIPTION --------------------------------

     All database related configuration and setting will be stored
     
----------------------------------------------------------------------------*/

#ifndef DATABASECONFIGURATION_H_
#define DATABASECONFIGURATION_H_

#define DB_QUERY_SIZE								1024
#define SEEK_RECIPE_COLUMN_COUNT					10
#define	SCAN_RECIPE_COLUMN_COUNT					7
#define	TEST_RECIPE_COLUMN_COUNT					7
#define FREE_LIST_BENCHMARK							1000

#define DB_RECORDS_STORAGE_WELD_SIGNATURE_LIMIT		1000
#define DB_RECORDS_STORAGE_WELD_RESULT_LIMIT		200000
#define DB_RECORDS_SCAN_STORAGE_LIMIT				100
#define DB_RECORDS_ALARM_STORAGE_LIMIT				100000
#define DB_RECORDS_EVENT_STORAGE_LIMIT				100000
#define DB_MAX_STORAGE_PERCENTAGE					95/100
#define DB_STORAGE_LIMIT_PERCENTAGE					80/100

/* TABLE NAME IN DB */
#define TABLE_CALIBRATION	 						"Calibration"
#define TABLE_SEEK_SIGNATURE 						"SeekSignature"
#define TABLE_SEEK_RESULT	 						"SeekResults"
#define TABLE_TEST_SIGNATURE 						"TestSignature"
#define TABLE_TEST_RESULT	 						"TestResult"
#define TABLE_ALARM_CONFIGURATION	 				"AlarmConfiguration"
#define TABLE_WELD_RESULT							"WeldResultTable"
#define TABLE_WELD_SIGNATURE						"WeldResultSignature"
#define TABLE_TEST_RECIPE							"TestRecipe"
#define TABLE_SCAN_SIGNATURE						"HornSignatureTable" 
#define TABLE_SCAN_RESULT							"HornResultTable"
#define TABLE_ALARM_LOG								"AlarmLogs"
#define TABLE_EVENT_LOG								"EventLogTable"

/* Production table column name */
#define COLUMN_NAME_GOOD 							"Good";
#define COLUMN_NAME_THROUGHPUT						"PartsPerMin";
#define COLUMN_NAME_SUSPECT							"Suspect";
#define COLUMN_NAME_REJECT							"Reject";

#endif /* DATABASECONFIGURATION_H_ */
