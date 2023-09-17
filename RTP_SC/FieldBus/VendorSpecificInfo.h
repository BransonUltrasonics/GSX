/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef FIELDBUS_VENDORSPECIFICINFO_H_
#define FIELDBUS_VENDORSPECIFICINFO_H_
#include "Common.h"
#include "CommonProperty.h"
#include "commons.h"
#include "EthernetIP.h"
#include  <map>
#include "SQLiteDB.h"

#define INT_TO_UINT8									4
#define MAX_CHAR_ARR_SIZE								28
#define MAX_INT32_TO_SEND								128
#define MAX_UINT8_TO_SEND								512
#define MAX_RETRIES										5
#define SQL_QUERY_SIZE									500
#define MESSSAGE_SIZE									100
#define NON_WRITABLE									-1
#define WELD_RECIPE_OBJECT_MAX_INSTANCES 				1000
#define WELD_RESULTS_OBJECT_MAX_INSTANCES 				200000
#define SUSPECT_REJECT_OBJECT_MAX_INSTANCES 			1000
#define STACK_RECIPE_OBJECT_MAX_INSTANCES				1000
#define SEEK_SCAN_TEST_RECIPE_OBJECT_MAX_INSTANCES		1
#define SEEK_SCAN_TEST_RESULTS_OBJECT_MAX_INSTANCES		100
#define ALARM_OBJECT_MAX_INSTANCES						100000
#define SYSTEM_INFO_OBJECT_MAX_INSTANCES				1
#define LAST_WELD_RESULTS_OBJECT_MAX_INSTANCES			1
#define VERSION_RTC_COUNTERS_OBJECT_MAX_INSTANCES		1
#define LAST_WELD_RESULTS_OBJECT_MAP_SIZE 29

enum DBDataTypes{
	DB_DEFAULT=-1,
	DB_INTEGER,
	DB_DATETIME,
	DB_TEXT,
	DB_REAL,
	DB_BOOLEAN,
};

enum WeldResultsTableIndex{
	WD_NONE,//to align index with Attribute Number
	WD_RECIPENUMBER,
	WD_WELDRECIPEVERNUMBER,
	WD_DATETIME,
	WD_STACKSERIALNO,
	WD_CYCLECOUNTER,
	WD_WELDMODE,
	WD_MAXWELDFORCE,
	WD_ENDHOLDFORCE,
	WD_WELDABSOLUTE,
	WD_TOTALABSOLUTE,
	WD_WELDCOLLAPSEDISTANCE,
	WD_HOLDCOLLAPSEDISTANCE,
	WD_TOTALCOLLAPSEDISTANCE,
	WD_TRIGGERDISTANCE,
	WD_VELOCITY,
	WD_WELDTIME,
	WD_WELDENERGY,
	WD_WELDPEAKPOWER,
	WD_STARTFREQUENCY,
	WD_FREQUENCYCHANGE,
	WD_CYCLETIME,
	WD_USERNAME,
	WD_PARTID,
	WD_BATCHID,
	WD_TRIGGERPOINT,
	WD_WELDSONICPOINT,
	WD_HOLDPOINT,
	WD_ALARMFLAG,
	WD_RECIPESTATUS,

};

enum ServiceType
{
   Get_Attributes_All = 0x01,
   Set_Attributes_All  = 0x02,
   Get_Attribute_Single= 0x0E,
   Set_Attribute_Single = 0x10,
};


typedef enum AllowedServices
{
   NOT_ALLOWED=-1,
   GET_ATTR_ALL,
   SET_ATTR_ALL,
   GET_ATTR_SINGLE,
   SET_ATTR_SINGLE,
} AllowedServices;

enum AllowedAccess
{
   NO_ALLOWED_ACCESS = -1,
   READ_ATTR,
   READ_WRITE_ATTR,
};

//Ethernet IP Standard Error codes for Explicit Messaging.
enum GeneralStatusCode
{
	STAT_SUCCESS = 0x00,
	STAT_PATH_DESTINATION_UNKNOWN = 0x05,
	STAT_SERVICE_NOT_SUPPORTED = 0x08,
	STAT_ATTRIBUTE_NOT_SETTABLE = 0x0E,
	STAT_VENDOR_SPECIFIC_ERROR = 0x1F,
	STAT_PRIVILEGE_VIOLATION = 0x0F,
	STAT_INVALID_ATTRIBUTE = 0x09,
	STAT_INVALID_PARAMETER_VALUE = 0x03,
};

//Ethernet IP Vendor specific Error codes for Explicit Messaging.
enum ExtendedStatusCode
{
	EXTSTAT_SUCCESS = 0x00,
	EXTSTAT_VAL_ERR_MISC = 0xFFFFFFFF,
};

struct EthIPStausCode
{
	GeneralStatusCode GSC;
	ExtendedStatusCode ESC;
};

struct VendorSpecificClassesAttributes
{
	INT32 AttributeNumber;
    string DBColumnName;
    INT32 AllowedAccessType;
    INT32 IndexInMinMaxValueMap;	//To find in Recipe Parameter Map
    INT32 DataType;
};
struct VendorSpecificClassesInfo
{
	INT32 ClassID;
    string DBTableName;
    string DBTableUniqueID;
    INT32 InstanceLimit;
    INT32 AllowedServicesMask;
    map<INT32,VendorSpecificClassesAttributes> AttributesMap ; 
};

class VendorSpecificInfo {

public:
	static VendorSpecificInfo* Getinstance();
	void UpdateLastWeldResultsData(WeldResult result, char * cBatchID,string sDateTime);

	EthIPStausCode ProcessRequest(uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,  uint8_t *  Data, UINT32 * SizeToSend ,INT32 AttributeNumber);
private:
	WeldResult 	LastWeldResults;
	char LastWeldBatchID[50];
	string LastWeldDateTime;
	map<INT32,VendorSpecificClassesInfo> m_MapClassesInfo;
	map<INT32,VendorSpecificClassesAttributes> m_MapWeldResultsAttributes;
	map<INT32,VendorSpecificClassesAttributes> m_MapLastWeldResultsAttributes;
	SQLiteDB  m_DbConn;
	CommonProperty			*CP;
	bool GetData(uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,   uint8_t * Data, UINT32 * SizeToSend, INT32 AttributeNumber);
	bool ProcessDatabaseRequest(uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,   uint8_t * Data, UINT32 * SizeToSend, INT32 AttributeNumber);

	bool ProcessDataBuffer(string queryResult, INT32 ClassID , uint8_t * Data, UINT32 * SizeToSend,INT32 AttributeNumber);
	bool IsAttributeAccessTypeValid(INT32 ClassID, INT32 RequestedAttributeNumber,uint8_t CIPServiceCode);
	string GetLastWeldResultData(INT32 AttributeNumber);
	bool IsInstanceWithinRange(INT32 ClassID, INT32 RequestedInstanceNumber);
	bool IsAttributeWithinRange(INT32 ClassID, INT32 RequestedAttributeNumber);
	bool IsClassWithinRange(INT32 ClassID);
	bool IsServiceCodeSupported(uint8_t CIPServiceCode, INT32 ClassID);
	AllowedServices GetAllowedService(uint8_t CIPServiceCode);
	void AddParamToVendorSpecificClassesMap(INT32 ClassID, string DBTableName, string DBTableUniqueID, INT32 InstanceLimit, INT32 AllowedServicesMask,map<INT32,VendorSpecificClassesAttributes> &AttributesMap);
	void AddParamToAttributesMap(INT32 AttributeNumber, string DBColumnName, INT32 AllowedAccessType, INT32 Index,INT32 DBType, map<INT32,VendorSpecificClassesAttributes> &AttributesMap);
	void CreateVendorSpecificClassesStructure();
	void CreateAttributesStructures();
	void PrintAttributes();
	void CreateWeldResultsAttributesStructure();
	void CreateLastWeldResultsAttributesStructure();
	static VendorSpecificInfo *m_objVendorSpecificInfo;
	void InitializeLastWeldResults();
	VendorSpecificInfo();
	virtual ~VendorSpecificInfo();
};

#endif /* FIELDBUS_VENDORSPECIFICINFO_H_ */
