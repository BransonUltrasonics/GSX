/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef REPORTGENERATION_H_
#define REPORTGENERATION_H_

#include  "SQLiteDB.h"
#include  "CommonProperty.h"
#include  "RunTimeFeature.h"
#include "Utils.h"
#define DELETE_FROM_DB

/* libharu includes */
#include "hpdf.h"
/* end libharu includes */

#ifdef PDF_GENERATION
/* pdfwriter library includes */
#include "PDFWriter.h"
#include "PDFPage.h"
#include "PageContentContext.h"
#include "PDFFormXObject.h"
#include "ResourcesDictionary.h"
#include "XObjectContentContext.h"
/* end pdfwriter library includes */
#endif /* PDF_GENERATION */

#include <dirent.h>
#include <vector>
#include <map>

#define USB_DEVICEPATH				"/bd0:1"
#define REPORT_GEN_DIRECTORY_PATH 	"/bd0:1/ReportGenerationFiles"
#define WELDSIGNATURE_FILEPATH		"/bd0:1/ReportGenerationFiles/WeldResultSignature"
#define WELDRESULT_FILEPATH			"/bd0:1/ReportGenerationFiles/WeldResultTable"
#define ALARMLOG_FILEPATH			"/bd0:1/ReportGenerationFiles/AlarmLog"
#define SYSTEMSETUP_FILEPATH		"/bd0:1/ReportGenerationFiles/SystemSetup"
#define EVENTLOG_FILEPATH			"/bd0:1/ReportGenerationFiles/EventLog"
#define USERID_FILEPATH				"/bd0:1/ReportGenerationFiles/UserID"
#define RECIPESETTING_FILEPATH		"/bd0:1/ReportGenerationFiles/RecipeSetting"

#define DB_DELIMITER				","
#define ALARM_ID_DELIMITER			";"
#define REPORT_BUF_SIZE 			5
#define DB_COLUMN_SIZE				15

#define CLEAR_RECORD_DATA			32

#define FAILURE 	 				-1

#define INITIAL_MEMORY_CHECK	    0
#define MEMORY_CHECK_DONE       	1

#define A4_WIDTH 842
#define A4_HEIGHT 595
#define TABLE_STR_FONT_SIZE 7
#define TITLE_STR_FONT_SIZE 20
#define SIZE_FOR_EACH_ROW TABLE_STR_FONT_SIZE + 3
#define MIN_SIZE_TO_ADD_RECORD 11 
#define TABLE_OFFSET_LENGTH 5

/* String names for font objects */
#define COURIER_STRING "Courier.ttf"

/* String Names for Image Objects */
#define BRANSON_LOGO_STRING "Branson_gray_resize.jpg"
#define PDF_FONT_PATH  "/emmc0:0/Courier.ttf"
#define PDF_IMAGE_PATH "/emmc0:0/Branson_gray_resize.jpg"

/* Weld Data Parameters string for header display */
#define WELD_RESULT_PARAM_STRING 	"CycleCounter,DateTime,RecipeNumber,WeldRecipeVerNumber,RecipeStatus,WeldMode,MaxWeldForce,EndHoldForce,WeldAbsolute,TotalAbsolute,WeldCollapseDistance,HoldCollapseDistance,TotalCollapseDistance,TriggerDistance,WeldTime,WeldEnergy,WeldPeakPower,StartFrequency,FrequencyChange,CycleTime,Username,BatchID,TriggerPoint,AlarmFlag,PartID"
/* Weld Data Parameters Metric unit string for header display in PDF Format */
#define WELD_RESULT_PDF_METRIC_UNITS_STRING    "(#),(yy/MM/dd HH/mm/ss),(--),(--),(--),(--),(N),(N),(mm),(mm),(mm),(mm),(mm),(mm),(s),(J),(W),(Hz),(Hz),(s),(--),(--),(s),(--),(--)"
/* Weld Data Parameters Imperial unit string for header display in PDF Format */
#define WELD_RESULT_PDF_IMPERIAL_UNITS_STRING    "(#),(yy/MM/dd HH/mm/ss),(--),(--),(--),(--),(lbs),(lbs),(in),(in),(in),(in),(in),(in),(s),(J),(W),(Hz),(Hz),(s),(--),(--),(s),(--),(--)"
/* Event Log Parameters string for header display */
#define EVENT_LOG_PARAM_STRING      "EventNum,EventName,DateTime,UserID,PSSerialNo,ACSerialNo,AUXSerialNo,EventID,Identifier,Old,New,Comment"

/* Alarm Log Parameters string for header display */
//#define ALARM_LOG_PARAM_STRING      "DateTime,RecipeNumber,AlarmID,Machine,AlarmType,AlarmSubType,CycleCounter,RecipeVerNumber,PcAssembly,AcAssembly,UserName"
#define ALARM_LOG_PARAM_STRING      "DateTime,RecipeNumber,RecipeVerNumber,AlarmID,UserName,CycleCounter"

//#define ALARM_CFG_PARAM_STRING      "AlarmType,ResetRequired,LogAlarm,GeneralAlarm,CustomAlarmOutput,CycleCounter"
#define ALARM_CFG_PARAM_STRING      "AlarmType,ResetRequired,LogAlarm,GeneralAlarm"

//#define WELD_RECIPE_PARAM_STRING    "RecipeNumber,WeldRecipeName,WeldRecipeVerNumber,WeldMode,ModeValue,HoldTime,TriggerForce,TriggerDistance,AmplitudeStepsCnt,AmplitudeStepAt,AmplitudeStepValue1,AmplitudeStepValue2,AmplitudeStepValue3,AmplitudeStepValue4,AmplitudeStepValue5,AmplitudeStepValue6,AmplitudeStepValue7,AmplitudeStepValue8,AmplitudeStepValue9,AmplitudeStepValue10,AmplitudeStep1,AmplitudeStep2,AmplitudeStep3,AmplitudeStep4,AmplitudeStep5,AmplitudeStep6,AmplitudeStep7,AmplitudeStep8,AmplitudeStep9,AmplitudeStep10,WeldForce,ForceRampTime,TotalCollapseTarget,HoldForce,HoldForceRampTime,ExpectedPartContactPosition,ReadyPosition,DownAcceleration,DownMaxVelocity,DownDeceleration,ReturnAcceleration,ReturnMaxVelocity,ReturnDeceleration,NumForceSteps,ForceStepAt,ForceStepValue1,ForceStepValue2,ForceStepValue3,ForceStepValue4,ForceStepValue5,ForceStepValue6,ForceStepValue7,ForceStepValue8,ForceStepValue9,ForceStepValue10,ForceStepRampValue1,ForceStepRampValue2,ForceStepRampValue3,ForceStepRampValue4,ForceStepRampValue5,ForceStepRampValue6,ForceStepRampValue7,ForceStepRampValue8,ForceStepRampValue9,ForceStepRampValue10,ForceStep1,ForceStep2,ForceStep3,ForceStep4,ForceStep5,ForceStep6,ForceStep7,ForceStep8,ForceStep9,ForceStep10,weldRampTime,AfterBrust,ABDelay,ABTime,ABAmplitude,PreTrigger,AutoPreTrigger,DistancePreTrigger,PreTriggerAmplitude,PreTriggerDistance,EnergyBrake,EnergyBrakeTime,EnergyBrakeAmplitude,CoolingValve,MaxWeldTimeOut,TimedSeek,PreWeldSeek,PostWeldSeek,TimedSeekPeriod,PartContactWindowMinus,PartContactWindowPlus,PartContactWindowOffset,GlobalControl,PeakPowerCutoff,PeakPowerCutoffEnable,FrequencyLowCutoff,FrequencyLowCutoffEnable,AbsoluteCutoff,AbsoluteCutoffEnable,FrequencyHighCutoff,FrequencyHighCutoffEnable,CollapseCutoff,CollapseCutoffEnable,EnergyCutoff,EnergyCutoffEnable,TimeCutoff,TimeCutoffEnable,GroundDetectEnable,IsActive,IsLocked,CompanyName,IsValidate,ReadyPositionToggle,WeldForceControl"
#define WELD_RECIPE_PARAM_STRING    "RecipeNumber,WeldRecipeName,WeldRecipeVerNumber,WeldMode,ModeValue,HoldTime,TriggerForce,AmplitudeStepsCnt,AmplitudeStepAt,AmplitudeStep1,AmplitudeStep2,AmplitudeStep3,AmplitudeStep4,AmplitudeStep5,AmplitudeStep6,AmplitudeStep7,AmplitudeStep8,AmplitudeStep9,AmplitudeStep10,WeldForce,ForceRampTime,HoldForce,HoldForceRampTime,ExpectedPartContactPosition,ReadyPosition,DownAcceleration,DownMaxVelocity,DownDeceleration,ReturnAcceleration,ReturnMaxVelocity,ReturnDeceleration,NumForceSteps,ForceStepAt,ForceStepRampValue1,ForceStepRampValue2,ForceStepRampValue3,ForceStepRampValue4,ForceStepRampValue5,ForceStepRampValue6,ForceStepRampValue7,ForceStepRampValue8,ForceStepRampValue9,ForceStepRampValue10,ForceStep1,ForceStep2,ForceStep3,ForceStep4,ForceStep5,ForceStep6,ForceStep7,ForceStep8,ForceStep9,ForceStep10,weldRampTime,AfterBrust,ABDelay,ABTime,ABAmplitude,PreTrigger,AutoPreTrigger,DistancePreTrigger,PreTriggerAmplitude,PreTriggerDistance,EnergyBrake,EnergyBrakeTime,EnergyBrakeAmplitude,CoolingValve,MaxWeldTimeOut,TimedSeek,PreWeldSeek,PostWeldSeek,TimedSeekPeriod,PartContactWindowMinus,PartContactWindowPlus,PartContactWindowOffset,GlobalControl,PeakPowerCutoff,PeakPowerCutoffEnable,FrequencyLowCutoff,FrequencyLowCutoffEnable,AbsoluteCutoff,AbsoluteCutoffEnable,FrequencyHighCutoff,FrequencyHighCutoffEnable,CollapseCutoff,CollapseCutoffEnable,EnergyCutoff,EnergyCutoffEnable,TimeCutoff,TimeCutoffEnable,GroundDetectEnable,IsActive,IsValidate,ReadyPositionToggle,WeldForceControl,ScrubAmplitude"

#define DYNAMIC_RECIPE_PARAM_STRING "Reactivity,ForceLevelEnable,ForceLevel,ForceLevelTime"

//#define STACK_RECIPE_PARAM_STRING	"RecipeNumber,WeldRecipeVerNumber,psFreq,DigitalTune,InternalOffsetFlag,InternalFreqOffset,EndOfWeldStore"
#define STACK_RECIPE_PARAM_STRING	"DigitialTune,InternalOffsetFlag,InternalFreqOffset,EndOfWeldStore"

/* Weld Signature Parameters string for header display*/
#define WELD_SIGNATURE_PARAM_STRING "RecipeNumber,WeldRecipeVerNumber,CycleCounter,Time,Frequency,Power,Current,Amplitude,Phase,Energy,Force,Velocity,AbsoluteDistance,CollapseDistance"

/* User ID Parameters string for header display */
#define USER_ID_PARAM_STRING        "UserID,DateTime,UserLevel,UserStatus,LastChangedPassword,LastLogin,isLocked"

/* System Setup Parameters string for header display 21*/
#define SYSTEM_SETUP_PARAM_STRING   "ExtraCooling,ExternalPresets,HornClamp,LanguageIs,Sscreen,Unit,PsPowerUpOption,MemFullAction,AuthCheckLevel,PresetRecipeBarcodeStart,PartIDSwitchStatus,PowerUpMemory,ResetMemory,MachineName"
//#define SYSTEM_SETUP_PARAM_STRING_FROM_EEPROM   "ExtraCooling,ExternalPresets,HornClamp,Language,StartScreen,Units,PsPowerUpOption,MemFullAction,AuthCheckLevel,ActuatorConfig,MaxStrokelength,ControlLevelIs,PsFreq,PsWatt,PsType,PowerUpMem,MachineName,RecipePrefix,PartIDSwitch,ScSerialNum,SystemType"
#define SYSTEM_SETUP_PARAM_STRING_FROM_EEPROM   "UIVersion,ScVersion,PcVersion,AcVersion,LifetimeCnt,HornClamp,Language,StartScreen,PsPowerUpOption,MemFullAction,AuthCheckLevel,MaxStrokelength,PsFreq,PsWatt,PsType,MachineName,RecipePrefix,PartIDSwitch,ScSerialNum,PcSerialNum,AcSerialNum,SystemType"

#define IO_SETUP_PARAM_STRING   "Pin Number,I/O Type,Pin Function,Logic Level,Status"

/*This Length is based on above Weld Signature Parameters[WELD_SIGNATURE_PARAM_STRING] excluding recipe, recipe version number and cycle counter*/
#define BLOB_PARAMETERS_LEN			11

//#define MAX_FILE_SIZE (4294967296 - 536870912) /* (4GB - 512 MB) */

/*The below definition used for file slicing during weld extraction*/
#define MAX_FILE_SIZE (1048576 * 40)	/* (M * N) Where M is 1024 KB and N is number of MB */

/* Below string will be used in weld result header - CSV */
#define ALARM_ID_PARAM_STRING	"AlarmID"

/* Below string will be replaced by ALARM_ID_PARAM_STRING in weld result CSV header */
#define ALARM_FLAG_PARAM_STRING "AlarmFlag"

/* Below string will be used as header for weld result CSV report.
 * Note : Please make sure AlarmFlag parameter as last one in the header whenever modifying order of below header information */
#define WELD_RESULT_CSV_PARAM_STRING "RecipeNumber,WeldRecipeVerNumber,DateTime,RecipeStatus,CycleCounter,WeldMode,MaxWeldForce,EndHoldForce,WeldAbsolute,TotalAbsolute,WeldCollapseDistance,HoldCollapseDistance,TotalCollapseDistance,TriggerDistance,WeldTime,WeldEnergy,WeldPeakPower,StartFrequency,FrequencyChange,CycleTime,Username,PartID,BatchID,TriggerPoint,WeldSonicPoint,HoldPoint,AlarmFlag"
/* Weld Data Parameters metric unit string for header display in CSV Format*/
#define WELD_RESULT_CSV_METRIC_UNITS_STRING "(--),(--),(yy/MM/dd HH/mm/ss),(--),(#),(--),(N),(N),(mm),(mm),(mm),(mm),(mm),(mm),(s),(J),(W),(Hz),(Hz),(s),(--),(--),(--),(s),(--),(--),(--)"
/* Weld Data Parameters Imperial unit string for header display in CSV Format*/
#define WELD_RESULT_CSV_IMPERIAL_UNITS_STRING "(--),(--),(yy/MM/dd HH/mm/ss),(--),(#),(--),(lbs),(lbs),(in),(in),(in),(in),(in),(in),(s),(J),(W),(Hz),(Hz),(s),(--),(--),(--),(s),(--),(--),(--)"

/* Pdf Libharu parameters */
#define MAXPARTIDLEN                    14
#define MAXRECORDLENGTH                 50000
#define GRAPHPARAMWIDTH					100
#define LINEGAP					        10
#define ONELINEGAP						25
#define TWOLINEGAP						50
#define THREELINEGAP					75
#define FIVELINEGAP						125
#define ONEWELDLINEGAP		            20
#define TWOWELDLINEGAP					40
#define THREEWELDLINEGAP				60
#define FOURWELDLINEGAP					80
#define FIVEWELDLINEGAP					100
#define SIXWELDLINEGAP					120
#define SEVENWELDLINEGAP				160
#define EIGHTWELDLINEGAP				180
#define NINEWELDLINEGAP				    200
#define TENWELDLINEGAP                  220
#define HEADERHEIGHT					70

#define MAXSYSFILE_LEN			    	10
#define MAX_WELD_PDF_PAGES		    	2000
#define MAX_EVENT_PDF_PAGES		    	2000


#define PRINTSEQUENCE					23
#define PRINT_HISTORY_DATA				24
#define PARAMETERSTARTXCORD				75
#define PARAMETERSTARTYCORD				250
#define PRINTWELDSTARTXCORD				5
#define PRINTSYSTEMSTARTXCORD		    20
#define PARAMWIDTH						69
#define PARAMWIDTHLIVE					28
#define PRINTXCORDCENTER				500
#define SUSPECTLIMITSTART               600

#define ONEHEADINGGAP	                17
#define TWOHEADINGGAP					34
#define ABSGAP							15
#define UNITHEADINGGAP					68

#define MAXPARAMVALUES  				34

#define COLUMNWELDRESULT 				25
#define COLUMNEVENTLOG   				12
#define COLUMNALARMLOG   				7
#define COLUMNRECIPE    				7
#define COLUMNUSERID    				7
#define COLUMNSYSTEMSETUP  				7
#define COLUMNIOSETUP    				5
#define COLUMNALARMCFG    				4

#define ROWSPERPAGE      	   			35
#define ROWSPERPAGERECIPE      			1


#define LINEGAPWELDRESULT     			41
#define LINEGAPEVENTLOG       			100
#define LINEGAPALARMLOG       			150
#define LINEGAPRECIPE         			170
#define LINEGAPUSERID            		160
#define LINEGAPSYSTEMSETUP    			160
#define LINEGAPALARMCFG                 185

//Match the DateTime when fetching AlarmID during Weld Result CSV/PDF report generation within + And - 3 seconds window of passed DateTime value.
#define DATE_TIME_SECONDS_WINDOW 3

/*This Enum Structures indicates the type of report*/
typedef enum
{
	WELDRESULT_TYPE,
	ALARM_LOG,
	SYSTEM_SETUP,
	EVENT_LOG,
	USER_ID,
	RECIPE_SETTING,
	WELDSIGNATURE_TYPE,
	WELDRESULT_AND_WELDSIGNATURE_TYPE
} ReportID;

/*This Enum Structures indicates the type of report format*/
typedef enum
{
	CSV_FORMAT,
	PDF_FORMAT
} ReportType;

/*This Enum Structures indicates that error information that reported to UIC*/
typedef enum
{
	SC_INVALID_STATE			= 0,
	REPORT_GENERATE_SUCCESS		= 1,
	
	/*USB related error code*/
	USB_STORAGE_NOT_PRESENT		= 16,
	USB_STORAGE_FULL			= 17,
	USB_INSUFFICIENT_MEMORY		= 18,
	USB_MOUNT_ERROR				= 19,
	
	/*DB related error code*/
	DB_READ_ERROR 				= 32,
	DATA_NOT_COMPLETE 			= 33,
	DB_EMPTY					= 34,
	REPORT_GENERATION_FAILED	= 0xff,
} REPORT_ERROR_CODE;

/*Report Structure defined for UIC request*/
typedef struct
{
	UINT8 reportID;
	UINT8 storageFormat;
	char  reportBuff[REPORT_BUF_SIZE];
	UINT8 deleteID;
} Report_Message;

typedef enum {
	BRANSON_LOGO = 0
}IMAGE_CODES;

typedef enum {
	COURIER = 0
}FONT_CODES;

typedef enum
{
	PSDIGITALINPUT ,
	PSDIGITALOUTPUT,
	PSANALOGINPUT,
	PSANALOGOUTPUT,
	ACDIGITALINPUT,
	ACDIGITALOUTPUT
	
}IO_TYPE;

typedef enum
{
	i_CycleCounter,
	i_DateTime,
	i_RecipeNumber,
	i_WeldRecipeVerNumber,
	i_RecipeStatus,
	i_WeldMode,
	i_MaxWeldForce,
	i_EndHoldForce,
	i_WeldAbsolute,
	i_TotalAbsolute,
	i_WeldCollapseDistance,
	i_HoldCollapseDistance,
	i_TotalCollapseDistance,
	i_TriggerDistance,
	i_WeldTime,
	i_WeldEnergy,
	i_WeldPeakPower,
	i_StartFrequency,
	i_FrequencyChange,
	i_CycleTime,
	i_Username,
	i_BatchID,
	i_TriggerPoint,
	i_AlarmFlag,
	i_PartID
}WELD_RESULT_INDEX;
/*csv parameters**********************************************************/
typedef enum 
{
	MAX_WELD_FORCE = 6,
	END_HOLD_FORCE,
	WELD_ABSOLUTE ,
	TOTAL_ABSOLUTE,
	WELD_COLLAPSE_DISTANCE ,
	HOLD_COLLAPSE_DISTANCE ,
	TOTAL_COLLAPSE_DISTANCE,
	TRIGGER_DISTANCE,
	WELD_TIME,
	WELD_ENERGY,
	CYCLE_TIME =19,
	TRIGGER_POINT =23
	
}WELD_VALUE_IN_MM_MS;//Weld value in millimeter and milli second
/*ReportGeneration Base class is defined for both PDF and CSV file format*/
class ReportGeneration
{
public: 
	ReportGeneration();
	virtual ~ReportGeneration();

	/* Initialize the member variables */
	ReportGeneration(Report_Message &ReportId);
	void			ConnectDB();
	void 			CloseDB();
	STATUS 			CheckStorageDevice();
	void 			CloseStorageDevice();
	STATUS			OpenDirectoryFileDescriptor();
	void			CloseDirectoryFileDescriptor();
	bool			CheckDBEmpty();
	std::string		ReadReportHeader();
	void 			GetDateTimeStamp(std::string &TimeStamp);
	INT32 			GetColumnCount();
	INT32 			GetDBCount();
	UINT64          GetDBMinRowID();
	UINT32 			GetWeldSignatureRecordCount(INT32 RowNumber);
	STATUS 			ReadWeldResultFromDBToStorageDevice();
	STATUS 			ReadWeldSignatureFromDBToStorageDevice();
	
	INT32	CheckMaxRecordLimit(std::string a_tableName);
	INT32	GetRowID(std::string a_tableName, UINT32& a_startID, UINT32& endID);
	
	/* Function to split the string based on separator and convert string to lower case. Used currently by PDF */
	std::string 	ConvertStringToLower(std::string a_stringData);
	STATUS 			SplittingData(std::string& a_str , std::vector<std::string>& a_vecData, UINT8 a_separator) ;
	
	static Report_Message GetReportData();
	static	bool	GetReportGenerationFlag();
	
	/* Function to generate Report Name */
	std::string ReportNameGeneration();
	STATUS 	DeleteWeldResultFromDB();
	STATUS 	DeleteWeldSignatureFromDB();
	STATUS 	FormatWeldSignatureResultToRecords(std::string& DBBuffer, UINT32 totalRecords);
	bool    LimitFileWrite(UINT32 a_sizeInBytes);
	void    ResetFileLimitBytes();

	STATUS  PostDeleteResponse();
	
	virtual STATUS 	CheckDiskStorageSpace();
	virtual STATUS 	WriteDBRecordToUSB(std::string& DBRecord);
	virtual STATUS 	GenerateReport(UINT8 checkMemoryStatusCmd = 0);
	virtual STATUS FillRecipeInformationTable(INT32 a_recipeNum , INT32 a_recipeVerNum);
	virtual STATUS FillRecipeWeldMode(INT32 a_recipeNum , INT32 a_recipeVerNum);
	
protected:
	STATUS	Replace(std::string& a_src, std::string a_find, std::string a_replace);
	std::string	GetAlarmIDForWeldResult(UINT32 recipeNumber, UINT32 cycleCounter, std::string strDateTime);	
	time_t DateTimeToEpoch(std::string strDateTime);
	std::string EpochToDateTime(time_t EpochTime);
	
private:
//	INT32	CheckMaxRecordLimit(std::string a_tableName);
//	INT32	GetRowID(std::string a_tableName, UINT32& a_startID, UINT32& endID);
	
protected:
	static UINT32  RecordCount;
	static Report_Message m_reportReqData ;
	static bool    m_reportGenerationFlag;
	
	MSG_Q_ID 		UI_MSG_Q_ID;
	CommonProperty 	*CP;
	INT32		   m_storageFd;			
	INT32   	   m_LastRowNum;
	INT32		   m_FirstRowNum;
	UINT8 		   m_reportType;
	UINT8 		   m_storageFormat;
	UINT8		   m_deleteID;
	UINT8* 		   m_storageDestination;
	SQLiteDB	  *m_DbConn;
	SQLiteDB	   m_DbConnInstance;
	UINT8		   m_alarmFlag;
	UINT64 		   m_UsedSizeInBytes;
	UINT32		   m_currentRecipeNum;
	UINT32		   m_currentCycleCounter;
	std::string 	   m_currentDateTime;
	
	RunTime 	   RT;
	
	DIR 		  *reportDir; /*File Descriptor for Report Generation Directory*/
	
	std::map<std::string , std::string> m_stdUnitMap;
};

/*Defined CSV derived class for CSV file generation*/
class CSVFormat : public ReportGeneration
{
public:
	CSVFormat();
	~CSVFormat();

	CSVFormat(Report_Message &a_reportMsg);
	
	STATUS 			GenerateReport(UINT8 checkMemoryStatusCmd = 0);
	STATUS 			CheckDiskStorageSpace();
	STATUS 			WriteDBRecordToUSB(std::string& DBRecord);
	static STATUS 	OpenFileDescriptor(std::string& a_reportFile);
	static void		CloseFileDescriptor();
	static INT16	GetCSVFileDescriptor();
	
private:
	STATUS			AppendAlarmIDWithWeldResult(std::string& weldResult);
	
private:
	static INT16 	csvFd; /*CSV File Descriptor*/
	 Utils  *Util;
};

class PDFReportGeneration : public ReportGeneration
{
public:
	PDFReportGeneration();
	~PDFReportGeneration();

	PDFReportGeneration(Report_Message &a_reportMsg, SQLiteDB* a_dbPtr);

	STATUS GenerateReport(UINT8 a_checkMemoryStatusCmd = 0);
	
	STATUS CreatePdfContent(HPDF_Page * page, HPDF_Font &def_font,UINT32 pageNumber,UINT32 fileNumber);
	STATUS CreateNewPage(HPDF_Doc &pdf,HPDF_Page &page, HPDF_Font &def_font, UINT32 pageNumber);
	void DrawPDFText(HPDF_Page *page,int X_Cord,int Y_Cord,char *STR);
	void drawHeading(HPDF_Doc * pdf, HPDF_Page * page, HPDF_Font *def_font);
	void drawLOGParameterHeading(HPDF_Doc * pdf, HPDF_Page * page, HPDF_Font *def_font);
	STATUS createSuspectRejectLimits(HPDF_Doc * pdf,HPDF_Page * page, HPDF_Font &def_font,UINT32 pageNumber);
		
	void PdfWithImage(HPDF_Doc * pdf, HPDF_Page * page,HPDF_Font *def_font);
	
	STATUS clearDataFromDB4Pdf();		
	void DisplaySystemConfiguration();
	
	void SetTxtRGBColor(float r,float g, float b);
	void GetDateAndTime(string &DateTime);
	
	const char *GetLangugeStr(UINT16 EnumVal);
	const char *GetScreenStr(UINT16 EnumVal);
	const char *GetPowerUpOptionStr(UINT16 EnumVal);
	
	static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data);
	
private:

	
	UINT32 tableColumnCount;
	double* tableSpacing;
	UINT32 totalFileNumber;
	UINT64 minRowID;
	
	UINT32 XCord;
	UINT32 YCoordinate;
	UINT32 Unit_Heading_Gap;
	UINT32 PDFParamWidth[MAXPARAMVALUES];
	
	UINT32 PageCount;
	char Heading_Str[256];
	
	UINT32 PDFParamWidthWeld[COLUMNWELDRESULT];
	UINT32 PDFParamWidthEvent[COLUMNEVENTLOG];
	string UserIdStrHdr;
	string PresetStrHdr;
	string PresetRevStrHdr;
	Utils  *Util;
	
	string ValidStr;
	
	bool UnitHeadingPos;
	
	HPDF_Doc pdf;
	
	float R,G,B;

	static char* ActuatorInputStr[];
	static char* PowerSupplyInputStr[];
	static char* ActuatorOutputStr[];
	static char* PowerSupplyOutputStr[];
	static char* AnalogInputStr[];
	static char* AnalogOutputStr[]; 
	UINT16 setFrequencyValue(UINT16 PsFreq);

};


#ifdef PDF_GENERATION
class PDFReportGeneration : public ReportGeneration
{
public:
	PDFReportGeneration();
	~PDFReportGeneration();
	
	PDFReportGeneration(Report_Message &a_reportMsg, SQLiteDB* a_dbPtr) : ReportGeneration(a_reportMsg, a_dbPtr)
	{
		printf("\nParametrized constructor of PDF Report Gen is called\n");
	}

	/* Constructor to generate */
	
	/* Create the font and image file map */
	STATUS InitializeImgFontObjects();
	
	/* Initialize the table spacing and column count information */

	STATUS WriteDBRecordToUSB(std::string& DBRecord);
	STATUS WriteWeldSignatureRecordToStorage(std::string& DBRecord);
	STATUS CheckDiskStorageSpace();
	STATUS GenerateReport(UINT8 checkMemoryStatusCmd = 0);
	STATUS CreatePDFPage();
	STATUS EndPDFPage();
	STATUS InsertDocumentHeaderInformation();
	STATUS FillReportHeaderInformation();
	STATUS InitializeTableConfigData();
	
	STATUS FillRecipeInformationTable(INT32 a_recipeNum , INT32 a_recipeVerNum);
	STATUS FillRecipeWeldMode(INT32 a_recipeNum , INT32 a_recipeVerNum);

private:
	PDFWriter pdfWriter ;
	std::map<UINT8 , PDFFormXObject*> m_imageFiles ;
	std::map<UINT8 , PDFUsedFont*> m_fontFiles ;
	
	/* Column Related information */
	double* tableSpacing ;
	UINT8 tableColumnCount ;
	UINT16 spaceLeftOutToWriteRecords ;
	
	PageContentContext* m_pdfPageContentContext ;
};
#endif /* PDF_GENERATION */

#endif /* REPORT_GENERATION_H_ */
