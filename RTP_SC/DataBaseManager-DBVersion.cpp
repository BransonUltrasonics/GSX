/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2019
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Data base manager functions to store the Weld/Seek results to Database
 
**********************************************************************************************************/

#include  "DataBaseManager.h"
#include  "RunTimeFeature.h"
#include  "ReportGeneration.h"
#include  <sys/stat.h>
#include  "commons.h"
#include  "ControlTask.h"
#include  "AlarmManager.h"
#include  "DataBaseConfiguration.h"
#include  "UIDBInterface.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>

extern "C"
{
	#include "customSystemCall.h"	
}

#include <sstream>
#include <vector>
#include <bitset>
using namespace std;


string recipeData="", WeldRecipeZero="", SuspectRejectLimitsZero="",  BransonOnlyZero="", SetupLimitsZero="", StackRecipeZero="";
//Recipe zero back up strings
string WeldRecipeZeroBU="", SuspectRejectLimitsZeroBU="",  BransonOnlyZeroBU="", SetupLimitsZeroBU="", StackRecipeZeroBU="";	

/**************************************************************************//**
* \brief   - Checks for the latest DB version and updates the DB is latest isn't present
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::VersionCheck()
{
	INT32 currentDB = -2;
    std::string strQuery, queryReturn;
    
    //For this to work, going forward, we need to only use a single integer for DB version numbers
    //atoi function will ignore anything after the first non numerical character, so "1.0.0" returns 1
    strQuery = "select VersionNo from DBVersion";
    queryReturn = m_DbConn.Exec((char*)strQuery.c_str());
    currentDB = atoi((char*)queryReturn.c_str());
	
    if(currentDB != DB_VERSION)
    {
    	BackUpRecipeZero();
    	currentDB = UpdateDB(currentDB);
    }
    
	if(currentDB > 0)
	{
		//If DB upgrade success, loads recipe zero contents on UIDBInterface members
		//This is required because UIDBInterface reads the recipezero.txt contents before the upgrade,
		//so it will need a refresh after the upgrade
		GetRecipeZero();
		
		DBInterface::m_WeldRecipeZero = WeldRecipeZero;
		DBInterface::m_SuspectRejectLimitsZero = SuspectRejectLimitsZero;
		DBInterface::m_BransonOnlyZero = BransonOnlyZero;
		DBInterface::m_SetupLimitsZero = SetupLimitsZero;
		DBInterface::m_StackRecipeZero = StackRecipeZero;
	}else
	{
		//If DB upgrade fails, revert the Recipe zero changes
		RevertRecipeZeroChanges();
	}
}


/**************************************************************************//**
* \brief   - Executes SQL commands to convert from an earlier DB version to the latest version of the DB
*
* \param   - existingDB - the version that is currently present to be upgraded
*
* \return  - Upgraded DB Version #
*
******************************************************************************/
INT32 DataBaseManager::UpdateDB(INT32 existingDB)
{
	INT32 updatedVersion = -1, errorCode = 0;
	std::string strQuery, queryReturn;
	std::ostringstream ss;
	
	//Begin transaction to disable auto-commit, allowing rollback if there is an error
	strQuery = "BEGIN TRANSACTION;";
	m_DbConn.Exec((char*)strQuery.c_str(), &errorCode);
	
	if(SQLITE_OK == errorCode)
	{
		//switch with conditional fall-through to start at the existing DB version 
		//and keep updating step by step until the latest version is reached
		switch (existingDB)
		{
		case 0: // this will probably never run, as the eMMC was reconfigured with version 1.0.0 causing older DBs to be deleted entirely
			strQuery = "alter table WeldRecipeTable add column WeldForceControl INTEGER NOT NULL DEFAULT (3);";
			m_DbConn.Exec((char*)strQuery.c_str(), &errorCode);
			if(SQLITE_OK != errorCode)
				break;

		case 1:
			errorCode = DBUpdateToVer2();
			if(SQLITE_OK != errorCode)
				break;
			
		case 2:	
			errorCode = DBUpdateToVer3();  // update the stackrecipe table when upgrading from database version 2 to 3
			if(SQLITE_OK != errorCode)
				break;
			UpdateRecipeZeroToDBVer3();
			 		
		case 3:
			errorCode = DBUpdateToVer4();
			if(SQLITE_OK != errorCode)
				break;

		case 4:
			errorCode = DBUpdateToVer5();
			if(SQLITE_OK != errorCode)
				break;
			UpdateRecipeZeroToDBVer5();
			
		case 5:
			errorCode = DBUpdateToVer6();
			if(SQLITE_OK != errorCode)
				break;	
		case 6:
			errorCode = DBUpdateToVer7();
			
			if(SQLITE_OK != errorCode)
				break;		
			UpdateRecipeZeroToDBVer7();

		case 7:
			errorCode = DBUpdateToVer8();
			if(SQLITE_OK != errorCode)
				break;
			UpdateRecipeZeroToDBVer8();
			UpdateDBVersion();
			break;
			
		default:
			errorCode = -1;
			//Existing DB version unknown - exit without setting updatedVersion (will return -1)
			break;
		}	
	}
	if(SQLITE_OK == errorCode)
	{
		m_DbConn.Commit();
		m_DbConn.DBVacuum();
		updatedVersion = DB_VERSION;
	} else
	{
		LOGERR("DB_T : UpdateDB failed with ErrCode : %d",errorCode,0,0);
		strQuery = "ROLLBACK;";
		m_DbConn.Exec((char*)strQuery.c_str(), &errorCode);
	}
	
	return updatedVersion;
}

/**************************************************************************//**
* \brief   - Back up recipe zero contents before doing any modification
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::BackUpRecipeZero()
{
	string recipeZero[5] = {"", "", "", "", ""};
	string temp;
	fstream fp;
	UINT8 idx = 0;

	/* Read from file */
	if(CP->IsFileExist(RECIPE_ZERO_PATH))
	{
		fp.open(RECIPE_ZERO_PATH, ios::in);

		if(fp.is_open())
		{
			while(getline(fp,temp))
			{
				if(idx < 5)
				{
					recipeZero[idx] = temp;
				}
				idx++;
			}
			fp.close();
			
			WeldRecipeZeroBU 			= recipeZero[0];
			SuspectRejectLimitsZeroBU 	= recipeZero[1];
			BransonOnlyZeroBU			= recipeZero[2];
			SetupLimitsZeroBU			= recipeZero[3];
			StackRecipeZeroBU			= recipeZero[4];
		}
		else
		{
			LOGERR("DB_T : Recipe zero file open failed",0,0,0);
		}
	}
	else
	{
		fp.open(RECIPE_ZERO_PATH, fstream::in | fstream::out | fstream::trunc);
		fp.close();
		LOGERR("DB_T : Recipe zero file does not exist",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Revert recipe zero cahnges if DB upgrade fails
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::RevertRecipeZeroChanges()
{
	string recipeData;
	fstream fp;
	unsigned short recipeZeroCrc=0;
	bool insertResult=0;
	recipeData.append(WeldRecipeZeroBU);
	recipeData.append("\n");
	recipeData.append(SuspectRejectLimitsZeroBU);
	recipeData.append("\n");
	recipeData.append(BransonOnlyZeroBU);
	recipeData.append("\n");
	recipeData.append(SetupLimitsZeroBU);
	recipeData.append("\n");
	recipeData.append(StackRecipeZeroBU);
	recipeData.append("\n");
	
	fp.open(RECIPE_ZERO_PATH, ios::out);
	
	if(fp.is_open())	
	{
		fp << recipeData;
		fp.close();
		recipeZeroCrc= m_RecipeZerodbconn.CalCrcRecipeZeroFile(0,RECIPE_ZERO_PATH);
		insertResult=m_RecipeZerodbconn.InsertCrcInRecipeZeroFile(recipeZeroCrc);
		if(insertResult)
		{
			m_RecipeZerodbconn.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
		}
		else
		{
			LOGERR("DB_T : InsertCrcInRecipeZeroFile failed in RevertRecipeZeroChanges() ",0,0,0);
		}
	}
	else	
	{
		LOGERR("Error on writing into recipe zero file. File open failed",0,0,0);
	}
}


/**************************************************************************//**
* \brief   - update stack recipe, and populate the stack recipe table 
*
* \param   - None
*
* \return  - Returns the error code if there was an error sending the command.
*
******************************************************************************/
INT32 DataBaseManager::DBUpdateToVer2()
{
	vector<string> DBUpdate;
	
	DBUpdate.clear();
	
	DBUpdate.push_back("alter table AlarmLogs add column UserName TEXT;");
	DBUpdate.push_back("CREATE TABLE EventLogTableNew (EventNum INTEGER PRIMARY KEY AUTOINCREMENT, EventName TEXT, DateTime DATETIME DEFAULT (strftime('%Y/%m/%d %H:%M:%S','now','localtime')), PSSerialNo TEXT, ACSerialNo TEXT, AUXSerialNo TEXT, UserID TEXT, EventID TEXT, Identifier TEXT, Old TEXT, New TEXT, Comment TEXT);");
	DBUpdate.push_back("INSERT INTO EventLogTableNew(EventNum, EventName, DateTime, PSSerialNo, UserID, Comment) SELECT EventID, EventType, DateTime, PSSerialNo, UserID, Comment FROM EventLogTable;");
	DBUpdate.push_back("DROP TABLE EventLogTable;");
	DBUpdate.push_back("ALTER TABLE EventLogTableNew RENAME TO EventLogTable;");
	DBUpdate.push_back("alter table OperatorAuthorityOptions add column Option7 INTEGER;");
	DBUpdate.push_back("alter table WeldResultTable add column RecipeStatus INTEGER;");
	DBUpdate.push_back("alter table UserProfiles add column isLocked TEXT DEFAULT 'Unlocked';");
	DBUpdate.push_back("alter table UserProfiles add column NumLoginAttempt INTEGER DEFAULT 0;");
	DBUpdate.push_back("alter table UserProfiles add column TimeLoginFrom DATETIME;");
	DBUpdate.push_back("alter table UserProfiles add column TimeLoginTo DATETIME;");

	return DBSendCommands(DBUpdate);
}

/**************************************************************************//**
* \brief   - update stack recipe, and populate the stack recipe table 
*
* \param   - None
*
* \return  - Returns the error code if there was an error sending the command.
*
******************************************************************************/
INT32 DataBaseManager::DBUpdateToVer3()	
{
	vector<string> DBUpdate;
		
	DBUpdate.clear();
	
	DBUpdate.push_back("CREATE TABLE StackRecipeTable_new(RecipeNumber INTEGER, DigitialTune INTEGER, InternalOffsetFlag INTEGER, InternalFreqOffset INTEGER, EndOfWeldStore INTEGER);");
	DBUpdate.push_back("INSERT INTO StackRecipeTable_new(RecipeNUmber, DigitialTune, InternalOffsetFlag, InternalFreqOffset, EndOfWeldStore) select DISTINCT t1.RecipeNumber, t2.DigitialTune, t2.InternalOffsetFlag, t2.InternalFreqOffset, t2.EndOfWeldStore from  WeldRecipeTable t1 INNER join StackRecipeTable t2 ON 1=1;");
	DBUpdate.push_back("DROP TABLE StackRecipeTable;");
	DBUpdate.push_back("ALTER TABLE StackRecipeTable_new RENAME TO StackRecipeTable;");
	
	return DBSendCommands(DBUpdate);	
}

/**************************************************************************//**
* \brief   - update some I/O pin names in the UserIO table
*
* \param   - None
*
* \return  - Returns the error code if there was an error sending the command.
*
******************************************************************************/
INT32 DataBaseManager::DBUpdateToVer4()
{
	vector<string> DBUpdate;
		
	DBUpdate.clear();
	
	DBUpdate.push_back("UPDATE UserIO SET PinName = 'J58-09' WHERE id = 25;");
	DBUpdate.push_back("UPDATE UserIO SET PinName = 'J58-10' WHERE id = 26;");
	DBUpdate.push_back("UPDATE UserIO SET PinName = 'J58-05' WHERE id = 27;");
	DBUpdate.push_back("UPDATE UserIO SET PinName = 'J58-06' WHERE id = 28;");
	DBUpdate.push_back("UPDATE UserIO SET PinName = 'J58-07' WHERE id = 29;");
	DBUpdate.push_back("UPDATE UserIO SET PinName = 'J58-13' WHERE id = 30;");
	DBUpdate.push_back("UPDATE UserIO SET PinName = 'J58-14' WHERE id = 31;");
	
	return DBSendCommands(DBUpdate);
}

/**************************************************************************//**
* \brief   - Update WeldRecipeTable and BransonOnlyParameters with parameters needed for the Dynamic mode
*
* \param   - None
*
* \return  - Returns the error code if there was an error sending the command.
*
******************************************************************************/
INT32 DataBaseManager::DBUpdateToVer5()
{
	vector<string> DBUpdate;
	
	DBUpdate.clear();

	DBUpdate.push_back("alter table WeldRecipeTable add column Reactivity INTEGER DEFAULT 100;");
	DBUpdate.push_back("alter table WeldRecipeTable add column ForceLevelEnable INTEGER DEFAULT 0;");
	DBUpdate.push_back("alter table WeldRecipeTable add column ForceLevel INTEGER DEFAULT 100;");
	DBUpdate.push_back("alter table WeldRecipeTable add column ForceLevelTime INTEGER DEFAULT 0;");
	DBUpdate.push_back("alter table SetupLimits add column Reactivity TEXT DEFAULT '0:0:100';");
	DBUpdate.push_back("alter table SetupLimits add column ForceLevel TEXT DEFAULT '0:0:100';");
	DBUpdate.push_back("alter table WeldRecipeTable add column ScrubAmplitude INTEGER DEFAULT 100;");
	
	return DBSendCommands(DBUpdate);
}

/**************************************************************************//**
* \brief   - Add a column "RecordNumber" in "WeldResultSignature" Table.
			 The "RowID" column from "WeldResult" Table will be mapped to this 
			 "RecordNumber" column in "WeldResultSignature" Table based on 
			 "RecipeNumber", "WeldRecipeVerNumber" and "CycleCounter" column 
			 match.
* \param   - None
*
* \return  - Returns the error code if there was an error sending the command.
*
******************************************************************************/
INT32 DataBaseManager::DBUpdateToVer6()
{
	vector<string> DBUpdate;
	
	DBUpdate.clear();

	DBUpdate.push_back("ALTER TABLE WeldResultSignature ADD COLUMN RecordNumber INTEGER;");
	DBUpdate.push_back("UPDATE WeldResultSignature SET RecordNumber = (SELECT ROWID FROM WeldResultTable WHERE WeldResultTable.RecipeNumber = WeldResultSignature.RecipeNumber AND WeldResultTable.WeldRecipeVerNumber = WeldResultSignature.WeldRecipeVerNumber AND WeldResultTable.CycleCounter = WeldResultSignature.CycleCounter);");

	return DBSendCommands(DBUpdate);
}


/****************************************************************************
* \brief   - Energy is now saved in millijoules, this function updates
* 			 WeldResultTable,WeldRecipeTable,SuspectRejectLimits and SetupLimits
* 			 
* \param   - none
*
* \return  - the result of executing DBSendCommands
*
******************************************************************************/
INT32 DataBaseManager::DBUpdateToVer7()
{
	vector<string> DBUpdate;
	DBUpdate.clear();
	DBUpdate.push_back("UPDATE WeldResultTable SET WeldEnergy =(WeldEnergy*1000);");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ModeValue =(ModeValue*1000) where WeldMode=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET EnergyCutoff=(EnergyCutoff*1000);");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue1 =(AmplitudeStepValue1 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue2 =(AmplitudeStepValue2 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue3 =(AmplitudeStepValue3 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue4 =(AmplitudeStepValue4 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue5 =(AmplitudeStepValue5 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue6 =(AmplitudeStepValue6 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue7 =(AmplitudeStepValue7 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue8 =(AmplitudeStepValue8 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue9 =(AmplitudeStepValue9 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET AmplitudeStepValue10 =(AmplitudeStepValue10 * 1000) where AmplitudeStepAt=2;");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue1 =(ForceStepValue1 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue2 =(ForceStepValue2 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue3 =(ForceStepValue3 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue4 =(ForceStepValue4 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue5 =(ForceStepValue5 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue6 =(ForceStepValue6 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue7 =(ForceStepValue7 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue8 =(ForceStepValue8 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue9 =(ForceStepValue9 * 1000) where ForceStepAt=2;");	
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ForceStepValue10 =(ForceStepValue10 * 1000) where ForceStepAt=2;");

	std::string strQuery = "SELECT RecipeNumber FROM SuspectRejectLimits order by RecipeNumber ASC;";
	string RecipeNumberList =m_DbConn.Exec((char*)strQuery.c_str());
	INT32 TotalRecipes=0;
	string strInsertIntoSR="";
	size_t recipeLastPos = 0,recipeFirstPos=0;
	string RecipeNumber = "";
	bool quit=false;
	while(recipeLastPos != string::npos && TotalRecipes<1000 && !quit)
	{
		TotalRecipes++;
		recipeLastPos	= RecipeNumberList.find(DB_DELIMITER,recipeLastPos+1);
		RecipeNumber= RecipeNumberList.substr(recipeFirstPos, recipeLastPos-recipeFirstPos);
		if (RecipeNumber!="")
		{
			recipeFirstPos	= recipeLastPos+1;		
			strQuery = "SELECT * from SuspectRejectLimits where RecipeNumber = '"+RecipeNumber+"';";
			string SROldRecipe = m_DbConn.Exec((char*)strQuery.c_str());
			strInsertIntoSR="Insert into SuspectRejectLimits values ("+GetNewSRRecipeStr(SROldRecipe)+");";
			strQuery = "DELETE from SuspectRejectLimits where RecipeNumber = "+RecipeNumber+";";
			m_DbConn.Exec((char*)strQuery.c_str());
			DBUpdate.push_back(strInsertIntoSR);
		}
		else
		{
			printf("\nerror, no recipe to modify");
			quit=true;
		}

	}

	strQuery= "select RecipeNumber from SetupLimits where RecipeNumber in (select RecipeNumber from WeldRecipeTable where WeldMode=2) order by RecipeNumber ASC;";
	RecipeNumberList =m_DbConn.Exec((char*)strQuery.c_str());

	string strUpdateSetupLimits="";
	recipeLastPos = 0;
	recipeFirstPos=0;
	recipeLastPos=0;
	TotalRecipes=0;
	quit=false;
	while(recipeLastPos != string::npos && TotalRecipes<1000 && !quit)
	{		
		TotalRecipes++;			
		recipeLastPos	= RecipeNumberList.find(DB_DELIMITER,recipeLastPos+1);
		RecipeNumber= RecipeNumberList.substr(recipeFirstPos, recipeLastPos-recipeFirstPos);
		if (RecipeNumber!="")
		{
			recipeFirstPos	= recipeLastPos+1;
			strQuery = "SELECT WeldMode from SetupLimits where RecipeNumber = '"+RecipeNumber+"';";
			string SetupLimitsOldWeldModeValue = m_DbConn.Exec((char*)strQuery.c_str());
			strUpdateSetupLimits=  "update SetupLimits set WeldMode= '"+GetNewSetupLimitsStr(SetupLimitsOldWeldModeValue);
			strUpdateSetupLimits+="' where RecipeNumber = '";
			strUpdateSetupLimits.append(RecipeNumber+"';");
			DBUpdate.push_back(strUpdateSetupLimits);
		}
		else
		{
			printf("\nerror, no recipe to modify");
			quit=true;
		}
		

	}
	return DBSendCommands(DBUpdate);
}

/****************************************************************************
* \brief   - Actuator Setup Advanced Feature Parameter is now saved in (millimeter * 10000) to support imperial conversion, 
*            this function updates WeldRecipeTable.
* 			 
* \param   - none
*
* \return  - the result of executing DBSendCommands
*
******************************************************************************/
INT32 DataBaseManager::DBUpdateToVer8()
{
	vector<string> DBUpdate;
	DBUpdate.clear();
	DBUpdate.push_back("UPDATE WeldRecipeTable SET DownAcceleration=(DownAcceleration*10000);");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET DownMaxVelocity=(DownMaxVelocity*10000);");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET DownDeceleration=(DownDeceleration*10000);");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ReturnAcceleration=(ReturnAcceleration*10000);");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ReturnMaxVelocity=(ReturnMaxVelocity*10000);");
	DBUpdate.push_back("UPDATE WeldRecipeTable SET ReturnDeceleration=(ReturnDeceleration*10000);");

	return DBSendCommands(DBUpdate);

}
/****************************************************************************
* \brief   - since energy is now saved in millijoules we need to multiply
* 		the energy values * 1000, this function deals with setuplimits
*  			 
* \param   - WeldModeLimit: a string containing all setuplimits values
* 
*
* \return  - the new setuplimits values
*
******************************************************************************/

string DataBaseManager::GetNewSetupLimitsStr(string WeldModeLimit)
{
	string strQuery="";
	UINT16 weldRecipeIdx;										
	vector<UINT32> SetupLimitStringList;							
	string token;								
	UINT32 recipeValue = 0;
	UINT8 status = 1;
	SetupLimitStringList.clear();	
	stringstream WeldModeLimitString(WeldModeLimit);
	
	while( getline(WeldModeLimitString, token, ':') )
	{
		recipeValue = atoi(token.c_str());
		SetupLimitStringList.push_back(recipeValue);	
	}
	
	for (weldRecipeIdx = 0; weldRecipeIdx < SetupLimitStringList.size(); weldRecipeIdx++)
	{
		char 	value[10] = {0x00};	
		memset(value, 0x00, sizeof(value));
		sprintf(value,"%d",SetupLimitStringList.at(weldRecipeIdx));
		INT32 newvalue=0;
		switch(weldRecipeIdx)		
		{
		case 0:
			memset(value, 0x00, sizeof(value));
			sprintf(value,"%d",SetupLimitStringList.at(weldRecipeIdx));
			strQuery.append((const char *)value);
			strQuery+=":";
			break;
		case 1:
			memset(value, 0x00, sizeof(value));
			 newvalue=SetupLimitStringList.at(weldRecipeIdx)*1000;
			
			if(newvalue>=0)
			{
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			else{
				newvalue=0;
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			strQuery+=":";
			break;
		case 2:
			memset(value, 0x00, sizeof(value));
			newvalue=SetupLimitStringList.at(weldRecipeIdx)*1000;
			if(newvalue>=0)
			{
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			else{
				newvalue=0;
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			break;
		default:
			break;
		}
	}
	if(SetupLimitStringList.size() == 3)	
	{
		status = 0;
		LOGDBG("DB_T : SetupLimits read from DB", 0, 0, 0);		
	}
	else
	{
		status = 1;
		LOGERR("DB_T : Failed to read the SetupLimits from DB", 0, 0, 0);		
	}
	return (strQuery);	

}
/**************************************************************************s**
* \brief   - Energy is now saved in millijoules, this function converts 
* 			 energy Suspect and Reject limits to proper values (*1000) 			 
* \param   - SRRecipeAtIdx: the old recipe values
*
* \return  - strQuery: the new Suspect Reject values
*
******************************************************************************/

string DataBaseManager::GetNewSRRecipeStr( string SRRecipeAtIdx)
{
	string strQuery="";
	UINT16 weldRecipeIdx;										
	vector<UINT32> SRWeldRecipe;							
	string SusRejRecipe, token;								
	stringstream SRRecipeIdxSS(SRRecipeAtIdx);
	UINT32 recipeValue = 0;
	UINT8 status = 1;
	SRWeldRecipe.clear();	
	INT32 newvalue=0;

	while( getline(SRRecipeIdxSS, SusRejRecipe, ',') )
	{
		stringstream SusRejRecipeSS(SusRejRecipe);
		while( getline(SusRejRecipeSS, token, ':') )
		{
			recipeValue = atoi(token.c_str());
			SRWeldRecipe.push_back(recipeValue);		
		}
	}

	/* Write the data to the Weld Recipe structure. */
	for (weldRecipeIdx = 0; weldRecipeIdx < SRWeldRecipe.size(); weldRecipeIdx++)
	{
		char 	value[20] = {0x00};	
		memset(value, 0x00, sizeof(value));
		sprintf(value,"%d",SRWeldRecipe.at(weldRecipeIdx));


#ifdef DEBUG_PRINT	
		cout << "Case" << weldRecipeIdx << ": " << SRWeldRecipe.at(weldRecipeIdx) << "\n";		
#endif		
		switch(weldRecipeIdx)		
		{
		case SC_SR_RECIPE_NUMBER:		
		case SC_SR_RECIPE_REV_NUMBER:		
		case SC_SUSPECT_LIMIT_ENABLED:
		case SC_REJECT_LIMIT_ENABLED:
			strQuery.append((const char *)value);
			strQuery+=",";
			break;

		case SC_SUSPECT_LIMIT_TIME_ENABLED:
			strQuery+="'";
		case SC_SUSPECT_LIMIT_TIME_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_TIME_HIGH_ENABLED:
		case SC_SUSPECT_LIMIT_TIME_LOW:
		case SC_SUSPECT_LIMIT_TIME_HIGH:
		case SC_REJECT_LIMIT_TIME_ENABLED:
		case SC_REJECT_LIMIT_TIME_LOW_ENABLED:
		case SC_REJECT_LIMIT_TIME_HIGH_ENABLED:
		case SC_REJECT_LIMIT_TIME_LOW:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_TIME_HIGH:
			strQuery.append((const char *)value);
			strQuery+="','";
			break;

		case SC_SUSPECT_LIMIT_ENERGY_ENABLED:
		case SC_SUSPECT_LIMIT_ENERGY_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_ENERGY_HIGH_ENABLED:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;
		case SC_SUSPECT_LIMIT_ENERGY_LOW:
			newvalue=0;
			newvalue=SRWeldRecipe.at(weldRecipeIdx)*1000;
			if(newvalue>=0)
			{
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			else{
				newvalue=0;
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			strQuery+=":";
			break;

		case SC_SUSPECT_LIMIT_ENERGY_HIGH:
			newvalue=0;
			newvalue=SRWeldRecipe.at(weldRecipeIdx)*1000;
			if(newvalue>=0)
			{
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			else{
				newvalue=0;
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_ENERGY_ENABLED:
		case SC_REJECT_LIMIT_ENERGY_LOW_ENABLED:
		case SC_REJECT_LIMIT_ENERGY_HIGH_ENABLED:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;
		case SC_REJECT_LIMIT_ENERGY_LOW:
			newvalue=0;
			newvalue=SRWeldRecipe.at(weldRecipeIdx)*1000;
			if(newvalue>=0)
			{
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			else{
				newvalue=0;
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_ENERGY_HIGH:
			newvalue=0;
			newvalue=SRWeldRecipe.at(weldRecipeIdx)*1000;
			if(newvalue>=0)
			{
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			else{
				newvalue=0;
				sprintf(value,"%d",newvalue);
				strQuery.append((const char *)value);
			}
			strQuery+="','";
			break;

		case SC_SUSPECT_LIMIT_PEAK_POWER_ENABLED:
		case SC_SUSPECT_LIMIT_PEAK_POWER_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_PEAK_POWER_HIGH_ENABLED:
		case SC_SUSPECT_LIMIT_PEAK_POWER_LOW:
		case SC_SUSPECT_LIMIT_PEAK_POWER_HIGH:
		case SC_REJECT_LIMIT_PEAK_POWER_ENABLED:
		case SC_REJECT_LIMIT_PEAK_POWER_LOW_ENABLED:
		case SC_REJECT_LIMIT_PEAK_POWER_HIGH_ENABLED:
		case SC_REJECT_LIMIT_PEAK_POWER_LOW:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;
		case SC_REJECT_LIMIT_PEAK_POWER_HIGH:
			strQuery.append((const char *)value);
			strQuery+="','";
			break;

		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_ENABLED:
		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_HIGH_ENABLED:
		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_LOW:
		case SC_SUSPECT_LIMIT_ABSOLUTE_DISTANCE_HIGH:
		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_ENABLED:
		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_LOW_ENABLED:
		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_HIGH_ENABLED:
		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_LOW:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_ABSOLUTE_DISTANCE_HIGH:
			strQuery.append((const char *)value);
			strQuery+="','";
			break;

		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_ENABLED:
		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_HIGH_ENABLED:
		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_LOW:
		case SC_SUSPECT_LIMIT_COLLAPSE_DISTANCE_HIGH:
		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_ENABLED:
		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_LOW_ENABLED:
		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_HIGH_ENABLED:
		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_LOW:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_COLLAPSE_DISTANCE_HIGH:
			strQuery.append((const char *)value);
			strQuery+="','";
			break;

		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_ENABLED:
		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_HIGH_ENABLED:
		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_LOW:
		case SC_SUSPECT_LIMIT_TRIGGER_DISTANCE_HIGH:
		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_ENABLED:
		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_LOW_ENABLED:
		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_HIGH_ENABLED:
		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_LOW:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_TRIGGER_DISTANCE_HIGH:
			strQuery.append((const char *)value);
			strQuery+="','";
			break;

		case SC_SUSPECT_LIMIT_WELD_FORCE_ENABLED:
		case SC_SUSPECT_LIMIT_WELD_FORCE_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_WELD_FORCE_HIGH_ENABLED:
		case SC_SUSPECT_LIMIT_WELD_FORCE_LOW:
		case SC_SUSPECT_LIMIT_WELD_FORCE_HIGH:
		case SC_REJECT_LIMIT_WELD_FORCE_ENABLED:
		case SC_REJECT_LIMIT_WELD_FORCE_LOW_ENABLED:
		case SC_REJECT_LIMIT_WELD_FORCE_HIGH_ENABLED:
		case SC_REJECT_LIMIT_WELD_FORCE_LOW:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_WELD_FORCE_HIGH:
			strQuery.append((const char *)value);
			strQuery+="','";
			break;

		case SC_SUSPECT_LIMIT_FREQUENCY_ENABLED:
		case SC_SUSPECT_LIMIT_FREQUENCY_LOW_ENABLED:
		case SC_SUSPECT_LIMIT_FREQUENCY_HIGH_ENABLED:
		case SC_SUSPECT_LIMIT_FREQUENCY_LOW:
		case SC_SUSPECT_LIMIT_FREQUENCY_HIGH:
		case SC_REJECT_LIMIT_FREQUENCY_ENABLED:
		case SC_REJECT_LIMIT_FREQUENCY_LOW_ENABLED:
		case SC_REJECT_LIMIT_FREQUENCY_HIGH_ENABLED:
		case SC_REJECT_LIMIT_FREQUENCY_LOW:
			strQuery.append((const char *)value);
			strQuery+=":";
			break;

		case SC_REJECT_LIMIT_FREQUENCY_HIGH:
			strQuery.append((const char *)value);
			strQuery+="'";
			break;			
		default:
			break;
		}
	}		
#ifdef DEBUG_PRINT	
	cout << endl;		
#endif	

	if(SRWeldRecipe.size() == NUM_OF_SUS_REJ_RECIPE_DB_DATA)	
	{
		status = 0;
		LOGDBG("DB_T : Suspect Reject active recipe read from DB", 0, 0, 0);		
	}
	else
	{
		status = 1;
		LOGERR("DB_T : Failed to read the Suspect Reject active recipe from DB", 0, 0, 0);		
	}
	return (strQuery);			

}
/**************************************************************************//**
* \brief   - update the recipe zero txt file
*
* \param   - none
*
* \return  - none
*
******************************************************************************/
void DataBaseManager::UpdateRecipeZeroToDBVer3()
{
	INT32 errorCode = 0;
	string strQuery, recipeData;
	fstream fp;
	unsigned short recipeZeroCrc ;bool insertResult;
	GetRecipeZero();
	
    strQuery = "select * from StackRecipeTable where RecipeNumber =(select RecipeNumber from WeldRecipeTable where IsActive=1);";
 
    StackRecipeZero = m_DbConn.Exec((char*)strQuery.c_str(), &errorCode);
	
	recipeData.append(WeldRecipeZero);
	recipeData.append("\n");
	recipeData.append(SuspectRejectLimitsZero);
	recipeData.append("\n");
	recipeData.append(BransonOnlyZero);
	recipeData.append("\n");
	recipeData.append(SetupLimitsZero);
	recipeData.append("\n");
	recipeData.append(StackRecipeZero);
	recipeData.append("\n");
	
	fp.open(RECIPE_ZERO_PATH, ios::out);

	if(fp.is_open())
	{
		fp << recipeData;
		fp.close();
		recipeZeroCrc= m_RecipeZerodbconn.CalCrcRecipeZeroFile(0,RECIPE_ZERO_PATH);
		insertResult=m_RecipeZerodbconn.InsertCrcInRecipeZeroFile(recipeZeroCrc);
		if(insertResult)
		{
			m_RecipeZerodbconn.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
		}
		else
		{
			LOGERR("DB_T : InsertCrcInRecipeZeroFile failed in UpdateRecipeZeroToDBVer3() ",0,0,0);
		}

	}else
	{
		LOGERR("Error on writing into recipe zero file. File open failed",0,0,0);	
	}
}

/**************************************************************************//**
* \brief   - Updates recipezero.txt according DB version 5
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::UpdateRecipeZeroToDBVer5()
{
	
	string recipeData;
	fstream fp;
	unsigned short recipeZeroCrc ;bool insertResult;
	GetRecipeZero();
	
	WeldRecipeZero += DB_VER5_RECIPE_ZERO_PARAMS;
	SetupLimitsZero += DB_VER5_SETUP_LIMITS;
	
	recipeData.append(WeldRecipeZero);
	recipeData.append("\n");
	recipeData.append(SuspectRejectLimitsZero);
	recipeData.append("\n");
	recipeData.append(BransonOnlyZero);
	recipeData.append("\n");
	recipeData.append(SetupLimitsZero);
	recipeData.append("\n");
	recipeData.append(StackRecipeZero);
	recipeData.append("\n");
	
	fp.open(RECIPE_ZERO_PATH, ios::out);

	if(fp.is_open())
	{
		fp << recipeData;
		fp.close();
		recipeZeroCrc= m_RecipeZerodbconn.CalCrcRecipeZeroFile(0,RECIPE_ZERO_PATH);
		insertResult=m_RecipeZerodbconn.InsertCrcInRecipeZeroFile(recipeZeroCrc);
		if(insertResult)
		{
			m_RecipeZerodbconn.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
		}
		else
		{
			LOGERR("DB_T : InsertCrcInRecipeZeroFile failed in UpdateRecipeZeroToDBVer5() ",0,0,0);
		}
	}
	else
	{
		LOGERR("Error on writing into recipe zero file. File open failed",0,0,0);
	}
}


/****************************************************************************
* \brief   - Energy is now saved in millijoules, this function updates
* 			 the recipe zero file to version 7 
* 			 
* \param   - none
*
* \return  - none
* 
******************************************************************************/

void DataBaseManager::UpdateRecipeZeroToDBVer7()
{
	string recipeData;
	fstream fp;
	unsigned short recipeZeroCrc ;bool insertResult;
	GetRecipeZero();
	vector<string> vRecipeParams,vSetupRecipeParams;
	string strWeldRecipeZero="", strSetupLimitsZero;
	char 	value[20] = {0x00};	
	memset(value, 0x00, sizeof(value));

	strWeldRecipeZero = WeldRecipeZero;
    strSetupLimitsZero = SetupLimitsZero;
    
	CP->StringToTokens(strWeldRecipeZero.c_str(),vRecipeParams,',');

	INT32 weldmode= atoi(vRecipeParams[WR_DB_WELD_MODE].c_str());
	INT32 weldmodevalue=0;
	sprintf(value,"%d","0");
	if (weldmode==2)
	{
		weldmodevalue = atoi(vRecipeParams[WR_DB_MODE_VALUE].c_str())*1000;
		if(weldmodevalue>=0)
			sprintf(value,"%d",weldmodevalue);
		else
			printf("\nSomething went wrong 1");
		vRecipeParams[WR_DB_MODE_VALUE]=value;
	}

	
	INT32 StepAt=atoi(vRecipeParams[WR_DB_AMPLITUDE_STEP_AT].c_str());
	INT32 StepValue=0;
	if(StepAt==2)
	{
		for(int i=WR_DB_AMPLITUDE_STEP_VALUE1; i<=WR_DB_AMPLITUDE_STEP_VALUE10;i++)
		{
			sprintf(value,"%d","0");	
			StepValue=atoi(vRecipeParams[i].c_str())*1000;
			if(StepValue>=0)
				sprintf(value,"%d",StepValue);
			else
				printf("\nSomething went wrong 2");
			vRecipeParams[i]=value;
		}
	}
	StepAt=atoi(vRecipeParams[WR_DB_FORCE_STEP_AT].c_str());
	StepValue=0;
	if(StepAt==2)
	{
		for(int i=WR_DB_FORCE_STEP_VALUE1; i<=WR_DB_FORCE_STEP_VALUE10;i++)
		{
			sprintf(value,"%d","0");	
			StepValue=atoi(vRecipeParams[i].c_str())*1000;
			if(StepValue>=0)
				sprintf(value,"%d",StepValue);
			else
				printf("\nSomething went wrong 3");
			vRecipeParams[i]=value;
		}
	}
	INT32 EnergyCutoff=atoi(vRecipeParams[WR_DB_ENERGY_CUTOFF].c_str())*1000;
	sprintf(value,"%d","0");	
	if(EnergyCutoff>=0)
	{
		sprintf(value,"%d",EnergyCutoff);	
		vRecipeParams[WR_DB_ENERGY_CUTOFF]=value;
	}
	else
	{
		vRecipeParams[WR_DB_ENERGY_CUTOFF]=value;
		printf("\nSomething went wrong 4");
	}
	
	

	
	CP->TokensToString(strWeldRecipeZero,vRecipeParams,',');
    string strSRLimitsZero=GetNewSRRecipeStr(SuspectRejectLimitsZero);
	
	
	if(weldmode==2)
	{
		CP->StringToTokens(strSetupLimitsZero.c_str(),vSetupRecipeParams,',');
				
		vSetupRecipeParams[3].erase(vSetupRecipeParams[3].begin());
		vSetupRecipeParams[3].erase(vSetupRecipeParams[3].end() - 1);
		string SetupLimitsOldWeldModeValue =vSetupRecipeParams[3];
		printf("\n");
		printf(SetupLimitsOldWeldModeValue.c_str());
		string SetupWeldModeLimits = GetNewSetupLimitsStr(SetupLimitsOldWeldModeValue);
		vSetupRecipeParams[3]="'"+SetupWeldModeLimits+"'";
		
		
		CP->TokensToString(strSetupLimitsZero,vSetupRecipeParams,',');
	}
	
	recipeData.append(strWeldRecipeZero);
	recipeData.append("\n");
	recipeData.append(strSRLimitsZero);
	recipeData.append("\n");
	recipeData.append(BransonOnlyZero);
	recipeData.append("\n");
	recipeData.append(strSetupLimitsZero);
	recipeData.append("\n");
	recipeData.append(StackRecipeZero);
	recipeData.append("\n");
    
			
	fp.open(RECIPE_ZERO_PATH, ios::out);

	if(fp.is_open())
	{
		fp << recipeData;
		fp.close();
		recipeZeroCrc= m_RecipeZerodbconn.CalCrcRecipeZeroFile(0,RECIPE_ZERO_PATH);
		insertResult=m_RecipeZerodbconn.InsertCrcInRecipeZeroFile(recipeZeroCrc);
		if(insertResult)
		{
			m_RecipeZerodbconn.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
		}
		else
		{
			LOGERR("DB_T : InsertCrcInRecipeZeroFile failed in UpdateRecipeZeroToDBVer7() ",0,0,0);
		}
	}
	else
	{
		LOGERR("Error on writing into recipe zero file. File open failed",0,0,0);
	}
}

/****************************************************************************
* \brief   - Actuator Setup Advanced Feature Parameter is now saved in (millimeter * 10000) to support imperial conversion, 
*            this function updates the recipe zero file to version 8
* 
* \param   - none
*
* \return  - none
* 
******************************************************************************/
void DataBaseManager::UpdateRecipeZeroToDBVer8()
{
	string recipeData;
	fstream fp;
	unsigned short recipeZeroCrc ;bool insertResult;
	GetRecipeZero();
	vector<string> vRecipeParams;
	string strWeldRecipeZero = "", strSetupLimitsZero;
	char 	value[20] = {0x00};	
	memset(value, 0x00, sizeof(value));

	strWeldRecipeZero = WeldRecipeZero;
    strSetupLimitsZero = SetupLimitsZero;
    
	CP->StringToTokens(strWeldRecipeZero.c_str(),vRecipeParams,',');

	INT32 actSetupAdvFeatureParamvalue = 0;
	sprintf(value,"%d","0");
	for(int i = WR_DB_DOWN_ACCELERATION; i <= WR_DB_RETURN_DECELERATION ; i++)
	{
		/* on Login, New recipe set Actuator SetUp Advanced Feature parameter default value from RecipeZero.txt.
		 * To support imperial conversion this parameter Value shall be multiplied with 10000 */
		actSetupAdvFeatureParamvalue = atoi(vRecipeParams[i].c_str())*10000;
		if( actSetupAdvFeatureParamvalue >= 0 )
			sprintf(value,"%d",actSetupAdvFeatureParamvalue);
		else
		    LOGERR("----------- Something Went Wrong -----------", 0, 0, 0);
		vRecipeParams[i]=value;
	}

	CP->TokensToString(strWeldRecipeZero,vRecipeParams,',');
    string strSRLimitsZero = GetNewSRRecipeStr(SuspectRejectLimitsZero);
	
	recipeData.append(strWeldRecipeZero);
	recipeData.append("\n");
	recipeData.append(strSRLimitsZero);
	recipeData.append("\n");
	recipeData.append(BransonOnlyZero);
	recipeData.append("\n");
	recipeData.append(strSetupLimitsZero);
	recipeData.append("\n");
	recipeData.append(StackRecipeZero);
	recipeData.append("\n");
    
	fp.open(RECIPE_ZERO_PATH, ios::out);

	if(fp.is_open())
	{
		fp << recipeData;
		fp.close();
		recipeZeroCrc= m_RecipeZerodbconn.CalCrcRecipeZeroFile(0,RECIPE_ZERO_PATH);
		insertResult=m_RecipeZerodbconn.InsertCrcInRecipeZeroFile(recipeZeroCrc);
		if(insertResult)
		{
			m_RecipeZerodbconn.CopyFile(RECIPE_ZERO_PATH, RECIPE_ZERO_BKUP_PATH );
		}
		else
		{
			LOGERR("DB_T : InsertCrcInRecipeZeroFile failed in UpdateRecipeZeroToDBVer7() ",0,0,0);
		}
	}
	else
	{
		LOGERR("Error on writing into recipe zero file. File open failed",0,0,0);
	}
}

/**************************************************************************//**
* \brief   - Gets all the contents from recipezero.txt
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::GetRecipeZero()
{
	string recipeZero[5] = {"", "", "", "", ""};
	string temp;
	fstream fp;
	UINT8 idx = 0;

	/* Read from file */
	if(CP->IsFileExist(RECIPE_ZERO_PATH))
	{
		if(!m_RecipeZerodbconn.ValidateRecipeZeroFile(RECIPE_ZERO_PATH))
		{		
			if(m_RecipeZerodbconn.ValidateRecipeZeroFile(RECIPE_ZERO_BKUP_PATH))
			{
				m_RecipeZerodbconn.CopyFile( RECIPE_ZERO_BKUP_PATH,RECIPE_ZERO_PATH);
			}
			else
			{
				LOGERR("DB_T : Recipe zero and Bkup Recipe zero  file validation failed",0,0,0);
			}
		}	
		
		if(m_RecipeZerodbconn.ValidateRecipeZeroFile(RECIPE_ZERO_PATH))
		{
			fp.open(RECIPE_ZERO_PATH, ios::in);

			if(fp.is_open())
			{
				while(getline(fp,temp))
				{
					if(idx < 5)
					{
						recipeZero[idx] = temp;
					}
					idx++;
				}
				fp.close();
			
				WeldRecipeZero 			= recipeZero[0];
				SuspectRejectLimitsZero = recipeZero[1];
				BransonOnlyZero			= recipeZero[2];
				SetupLimitsZero			= recipeZero[3];
				StackRecipeZero			= recipeZero[4];
			}
			else
			{
				LOGERR("DB_T : Recipe zero file open failed",0,0,0);
			}
		}	
	}
	else
	{
		fp.open(RECIPE_ZERO_PATH, fstream::in | fstream::out | fstream::trunc);
		fp.close();
		LOGERR("DB_T : Recipe zero file does not exist",0,0,0);
	}
	
}

/**************************************************************************//**
* \brief   - update the DB version
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void DataBaseManager::UpdateDBVersion()	
{
	std::ostringstream ss;
	std::string strQuery, queryReturn;	
	INT32 errorCode = 0;
	
    ss << "update DBVersion set VersionNo='" << DB_VERSION << "';";
    strQuery = ss.str();
    ss.flush();
    m_DbConn.Exec((char*)strQuery.c_str(), &errorCode);
    if(errorCode != SQLITE_OK)
      LOGERR("Unable to update DBVersion ",0,0,0);
}


/**************************************************************************//**
* \brief   - Send a stream of commands to the SQLiteDB.
*
* \param   - CommandStream: Vector of commands that will be sent to the SQLiteDB.
* 
*
* \return  - errorCode: Returns the error code if there was an error sending the command.
*
*
******************************************************************************/
INT32 DataBaseManager::DBSendCommands(vector<string> &CommandStream)
{
	INT32 errorCode = -1;
	
	for (int i = 0; i < CommandStream.size(); i++)
	{
		m_DbConn.Exec((char*)CommandStream[i].c_str(), &errorCode);
		
		if (SQLITE_OK != errorCode)	
		{
			LOGERR("DataBaseManager::DBSendCommands: Unable to update DB ErrCode: %d", errorCode,0,0);
			break;
		}
	}
	
	CommandStream.clear();
	
	return errorCode;
}
