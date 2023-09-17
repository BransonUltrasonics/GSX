/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#include <VendorSpecificInfo.h>

VendorSpecificInfo* VendorSpecificInfo::m_objVendorSpecificInfo = NULL;

/**************************************************************************//**
* \brief  - VendorSpecificInfo Constructor
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
VendorSpecificInfo::VendorSpecificInfo() 
{
	// TODO Auto-generated constructor stub
	CreateAttributesStructures();
	CreateVendorSpecificClassesStructure();
	InitializeLastWeldResults();
	int err;
	
	err = m_DbConn.EstablishDataBaseConnection();
	if ( err != SQLITE_OK)
		LOGERR("\nERROR OPENING DB CONNECTION %d",err,0,0 );
	else
		LOGERR("\nSUCCESS OPENING DB CONNECTION %d",err,0,0 );
	

	
		CP = CommonProperty::getInstance();
}

/**************************************************************************//**
* \brief  - This function returns the only instance of this class, as there's
* 			no reason for multiple of them to exist, if there is no instance
* 			it is created
* 
* \param  - None
*
* \return - m_objVendorSpecificInfo (VendorSpecificInfo instance)
* 
******************************************************************************/

VendorSpecificInfo* VendorSpecificInfo::Getinstance()
{
	if(m_objVendorSpecificInfo == NULL)
	{
		m_objVendorSpecificInfo = new (std::nothrow) VendorSpecificInfo();
	}

	return m_objVendorSpecificInfo;
}

/**************************************************************************//**
* \brief  - Destructor
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
VendorSpecificInfo::~VendorSpecificInfo() 
{

	
}

/**************************************************************************//**
* \brief  - Saves the received Vendor Specific Classes information to a temp variable
* 			and adds it to the map of Vendor Specific Classes
* 
* \param  - INT32 ClassID, string DBTableName, string DBTableUniqueID, 
* 			INT32 InstanceLimit, INT32 AllowedServicesMask
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::AddParamToVendorSpecificClassesMap(INT32 ClassID, string DBTableName, string DBTableUniqueID, INT32 InstanceLimit, INT32 AllowedServicesMask, 
		map<INT32,VendorSpecificClassesAttributes>  & AttributesMap)
{
	
	VendorSpecificClassesInfo TempVSCIObj;
	char StatusMessage[MESSSAGE_SIZE] = "";
	
	TempVSCIObj.ClassID = ClassID;
	TempVSCIObj.DBTableName = DBTableName;
	TempVSCIObj.DBTableUniqueID = DBTableUniqueID;
	TempVSCIObj.InstanceLimit = InstanceLimit;
	TempVSCIObj.AllowedServicesMask = AllowedServicesMask;
	TempVSCIObj.AttributesMap = AttributesMap;
	m_MapClassesInfo[ClassID] = TempVSCIObj;
	sprintf(StatusMessage, "%sAdded #%08X\n%s", KRED, ClassID, KNRM);
	LOGDBG (StatusMessage, 0, 0, 0);
	
}

/**************************************************************************//**
* \brief  - Call the functions for creating attributes structures
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::CreateAttributesStructures()
{
	CreateWeldResultsAttributesStructure();
	CreateLastWeldResultsAttributesStructure();
	
}


/**************************************************************************//**
* \brief  - Prints the attributes in WELD_RESULTS_OBJECT map
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::PrintAttributes()
{
	char StatusMessage[MESSSAGE_SIZE] = "";
	sprintf(StatusMessage,"\nclass %d  \n\n",m_MapClassesInfo[WELD_RESULTS_OBJECT].ClassID);
	LOGDBG (StatusMessage,0,0,0);
	INT32 MapSize = m_MapClassesInfo[WELD_RESULTS_OBJECT].AttributesMap.size();
	for (INT32 itr = 1; itr <= MapSize; itr++) 
	{
		sprintf(StatusMessage,"\nATTRIBUTE NUMBER %d  \nATTRIBUTE COLUMN NAME %s \nATTRIBUTE  ALLOWED ACCESS TYPE %d\n",
						 m_MapClassesInfo[WELD_RESULTS_OBJECT].AttributesMap[itr].AttributeNumber,
						  m_MapClassesInfo[WELD_RESULTS_OBJECT].AttributesMap[itr].DBColumnName.c_str(),
						  m_MapClassesInfo[WELD_RESULTS_OBJECT].AttributesMap[itr].AllowedAccessType);

		LOGDBG (StatusMessage,0,0,0);
		sprintf(StatusMessage,"");
	}

}
/**************************************************************************//**
* \brief  - Sends all of the Vendor Specific Classes information to the function
* 			AddParamToVendorSpecificClassesMap
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::CreateVendorSpecificClassesStructure()
{
	m_MapClassesInfo.clear();
	AddParamToVendorSpecificClassesMap(WELD_RESULTS_OBJECT,"WeldResultTable","ROWID", WELD_RESULTS_OBJECT_MAX_INSTANCES,(BIT_MASK(GET_ATTR_ALL)|BIT_MASK(GET_ATTR_SINGLE)), m_MapWeldResultsAttributes );
	AddParamToVendorSpecificClassesMap(LAST_WELD_RESULTS_OBJECT,"NA","NA", LAST_WELD_RESULTS_OBJECT_MAX_INSTANCES,(BIT_MASK(GET_ATTR_ALL)|BIT_MASK(GET_ATTR_SINGLE)), m_MapLastWeldResultsAttributes );
}

/**************************************************************************//**
* \brief  - Function to help insert Parameters in the Attributes Map indicated
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::AddParamToAttributesMap(INT32 AttributeNumber, string DBColumnName, INT32 AllowedAccessType, INT32 Index, INT32 DBType,
		map<INT32,VendorSpecificClassesAttributes>  & AttributesMap)
{
	VendorSpecificClassesAttributes TempAttributeEntry;
	
	TempAttributeEntry.AttributeNumber = AttributeNumber;
	TempAttributeEntry.DBColumnName = DBColumnName;
	TempAttributeEntry.AllowedAccessType = AllowedAccessType;
	TempAttributeEntry.IndexInMinMaxValueMap = Index;
	TempAttributeEntry.DataType = DBType;
	AttributesMap[AttributeNumber] = TempAttributeEntry;	
	
}


/**************************************************************************//**
* \brief  - Inserts all the attributes of Weld Results Vendor Specific Class
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::CreateWeldResultsAttributesStructure()
{
	m_MapWeldResultsAttributes.clear();
	AddParamToAttributesMap(WD_RECIPENUMBER,"RecipeNumber",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDRECIPEVERNUMBER,"WeldRecipeVerNumber",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_DATETIME,"DateTime",READ_ATTR,NON_WRITABLE,DB_DATETIME, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_STACKSERIALNO,"StackSerialNo",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_CYCLECOUNTER,"CycleCounter",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDMODE,"WeldMode",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_MAXWELDFORCE,"MaxWeldForce",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_ENDHOLDFORCE,"EndHoldForce",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDABSOLUTE,"WeldAbsolute",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_TOTALABSOLUTE,"TotalAbsolute",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDCOLLAPSEDISTANCE,"WeldCollapseDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_HOLDCOLLAPSEDISTANCE,"HoldCollapseDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_TOTALCOLLAPSEDISTANCE,"TotalCollapseDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_TRIGGERDISTANCE,"TriggerDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_VELOCITY,"Velocity",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDTIME,"WeldTime",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDENERGY,"WeldEnergy",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDPEAKPOWER,"WeldPeakPower",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_STARTFREQUENCY,"StartFrequency",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_FREQUENCYCHANGE,"FrequencyChange",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_CYCLETIME,"CycleTime",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_USERNAME,"Username",READ_ATTR,NON_WRITABLE,DB_TEXT, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_PARTID,"PartID",READ_ATTR,NON_WRITABLE,DB_TEXT, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_BATCHID,"BatchID",READ_ATTR,NON_WRITABLE,DB_TEXT, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_TRIGGERPOINT,"TriggerPoint",READ_ATTR,NON_WRITABLE,DB_REAL, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDSONICPOINT,"WeldSonicPoint",READ_ATTR,NON_WRITABLE,DB_REAL, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_HOLDPOINT,"HoldPoint",READ_ATTR,NON_WRITABLE,DB_REAL, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_ALARMFLAG,"AlarmFlag",READ_ATTR,NON_WRITABLE,DB_BOOLEAN, m_MapWeldResultsAttributes);
	AddParamToAttributesMap(WD_RECIPESTATUS,"RecipeStatus",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapWeldResultsAttributes);

}

/**************************************************************************//**
* \brief  - Inserts all the attributes of Last Weld Results Vendor Specific Class
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::CreateLastWeldResultsAttributesStructure()
{
	m_MapLastWeldResultsAttributes.clear();
	AddParamToAttributesMap(WD_RECIPENUMBER,"RecipeNumber",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDRECIPEVERNUMBER,"WeldRecipeVerNumber",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_DATETIME,"DateTime",READ_ATTR,NON_WRITABLE,DB_DATETIME, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_STACKSERIALNO,"StackSerialNo",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_CYCLECOUNTER,"CycleCounter",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDMODE,"WeldMode",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_MAXWELDFORCE,"MaxWeldForce",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_ENDHOLDFORCE,"EndHoldForce",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDABSOLUTE,"WeldAbsolute",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_TOTALABSOLUTE,"TotalAbsolute",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDCOLLAPSEDISTANCE,"WeldCollapseDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_HOLDCOLLAPSEDISTANCE,"HoldCollapseDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_TOTALCOLLAPSEDISTANCE,"TotalCollapseDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_TRIGGERDISTANCE,"TriggerDistance",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_VELOCITY,"Velocity",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDTIME,"WeldTime",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDENERGY,"WeldEnergy",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDPEAKPOWER,"WeldPeakPower",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_STARTFREQUENCY,"StartFrequency",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_FREQUENCYCHANGE,"FrequencyChange",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_CYCLETIME,"CycleTime",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_USERNAME,"Username",READ_ATTR,NON_WRITABLE,DB_TEXT, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_PARTID,"PartID",READ_ATTR,NON_WRITABLE,DB_TEXT, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_BATCHID,"BatchID",READ_ATTR,NON_WRITABLE,DB_TEXT, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_TRIGGERPOINT,"TriggerPoint",READ_ATTR,NON_WRITABLE,DB_REAL, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_WELDSONICPOINT,"WeldSonicPoint",READ_ATTR,NON_WRITABLE,DB_REAL, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_HOLDPOINT,"HoldPoint",READ_ATTR,NON_WRITABLE,DB_REAL, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_ALARMFLAG,"AlarmFlag",READ_ATTR,NON_WRITABLE,DB_BOOLEAN, m_MapLastWeldResultsAttributes);
	AddParamToAttributesMap(WD_RECIPESTATUS,"RecipeStatus",READ_ATTR,NON_WRITABLE,DB_INTEGER, m_MapLastWeldResultsAttributes);

}


/**************************************************************************//**
* \brief  - This function verifies that the requested Service Code is supported
* 			by the specified Class ID
* 
* \param  - uint8_t CIPServiceCode, INT32 ClassID
*
* \return - bool IsRequestValid
* 
******************************************************************************/
bool VendorSpecificInfo::IsServiceCodeSupported(uint8_t CIPServiceCode, INT32 ClassID)
{
	bool IsRequestValid = false;
	char StatusMessage[MESSSAGE_SIZE] = "";
	AllowedServices RequestedService;
	RequestedService = (GetAllowedService(CIPServiceCode));
	
	if(RequestedService != NOT_ALLOWED){
		if((m_MapClassesInfo[ClassID].AllowedServicesMask & (BIT_MASK(RequestedService))) == BIT_MASK(RequestedService))
			IsRequestValid = true;
	}
	if(false == IsRequestValid)
	{
		sprintf(StatusMessage,"\n REQUEST STATUS NOT VALID %d \n\n",CIPServiceCode);
		LOGERR(StatusMessage,0,0,0);
	}
	
	return IsRequestValid;
}

/**************************************************************************//**
* \brief  - This function processes an Explicit Message Request and returns 
* 			the status of the request
* 
* \param  - uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,
* 			 uint8_t *  Data, UINT32 * SizeToSend ,INT32 AttributeNumber

*
* \return - EthIPStausCode RequestProcessedStatus
* 
******************************************************************************/
EthIPStausCode VendorSpecificInfo::ProcessRequest(uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID, uint8_t *  Data, UINT32 * SizeToSend ,INT32 AttributeNumber)
{
	EthIPStausCode RequestProcessedStatus;
	bool ServiceCodeIsSupported = false;
	bool ClassIsInValidRange = false;
	bool AttributeNumberIsValid = false;
	bool InstanceNumberIsValid = false;
	bool AttributeAccessTypeIsValid = false;
	bool DataWasProcessed = false;
	RequestProcessedStatus.ESC = EXTSTAT_VAL_ERR_MISC;
	RequestProcessedStatus.GSC = STAT_PATH_DESTINATION_UNKNOWN;
	
	ClassIsInValidRange = IsClassWithinRange(ClassID);
	if(true == ClassIsInValidRange)
	{
		ServiceCodeIsSupported = IsServiceCodeSupported(CIPServiceCode, ClassID);
	
		if(true == ServiceCodeIsSupported)
		{
			AttributeNumberIsValid = IsAttributeWithinRange(ClassID, AttributeNumber);
			
			if(true == AttributeNumberIsValid)
			{
				InstanceNumberIsValid = IsInstanceWithinRange(ClassID, InstanceNumber);
				if(true == InstanceNumberIsValid)
				{
					AttributeAccessTypeIsValid = IsAttributeAccessTypeValid( ClassID,  AttributeNumber, CIPServiceCode);
					if(true == AttributeAccessTypeIsValid)
					{
						DataWasProcessed = GetData(CIPServiceCode, InstanceNumber, ClassID, Data, SizeToSend, AttributeNumber);
						if(true == DataWasProcessed)
						{
							RequestProcessedStatus.GSC = STAT_SUCCESS;
							RequestProcessedStatus.ESC = EXTSTAT_SUCCESS;
						}
						else
						{
							RequestProcessedStatus.GSC = STAT_PATH_DESTINATION_UNKNOWN;
						}
					}
					else
					{
						RequestProcessedStatus.GSC = STAT_INVALID_ATTRIBUTE;
					}
				}
				else
				{
					RequestProcessedStatus.GSC = STAT_PATH_DESTINATION_UNKNOWN;
				}
			}
			else
			{
				RequestProcessedStatus.GSC = STAT_INVALID_ATTRIBUTE;
			}
		}
		else
		{
			RequestProcessedStatus.GSC = STAT_SERVICE_NOT_SUPPORTED;
		}
	}
	else
	{
		RequestProcessedStatus.GSC = STAT_SERVICE_NOT_SUPPORTED;
	}
	return RequestProcessedStatus;
}

/**************************************************************************//**
* \brief  - Converts a given ServiceType number from CommonServices
* 			to the bit position for comparison between a requested service
* 			and the allowed services for a Vendor Specific Class through
* 			Explicit Messaging
*
* \param  - uint8_t CIPServiceCode
*
* \return - INT32 RequestedService
* 
******************************************************************************/
AllowedServices VendorSpecificInfo::GetAllowedService(uint8_t CIPServiceCode)
{
	AllowedServices RequestedService = NOT_ALLOWED;
	switch (CIPServiceCode)
	{
	case Get_Attributes_All:
		RequestedService = GET_ATTR_ALL;
		break;
		
	case Set_Attributes_All:
		RequestedService =  SET_ATTR_ALL;
		break;
		
	case Get_Attribute_Single:
		RequestedService =  GET_ATTR_SINGLE;
		break;
		
	case Set_Attribute_Single:
		RequestedService =  SET_ATTR_SINGLE;
		break;
		
	default:
		RequestedService =  NOT_ALLOWED;
		break;
	}
	
	return RequestedService;
}


/**************************************************************************//**
* \brief  - This function verifies that the requested Vendor Specific Class
* 			falls within valid range
* 
* \param  - INT32 ClassID
*
* \return - bool IsRequestValid
* 
******************************************************************************/
bool VendorSpecificInfo::IsClassWithinRange(INT32 ClassID)
{
	bool IsRequestValid = false;
	char StatusMessage[MESSSAGE_SIZE] = "";
	
	for (INT32 itr = ETHIP_VENDORSPECIFIC_GSX_START; itr < ETHIP_VENDOR_OBJECT_LAST_USED; itr++) {
		 if(m_MapClassesInfo[itr].ClassID == ClassID)
		 {
			 IsRequestValid = true;
			 return IsRequestValid;
		 }
		 
	}
	
	sprintf(StatusMessage,"\n REQUESTED CLASS ID WAS NOT FOUND %d",ClassID );
	LOGERR(StatusMessage,0,0,0);
	return IsRequestValid;
}
/**************************************************************************//**
* \brief  - This function verifies that the requested Vendor Specific Class
* 			Attribute falls within valid range
* 
* \param  - INT32 ClassID
*
* \return - bool IsRequestValid
* 
******************************************************************************/
bool VendorSpecificInfo::IsAttributeWithinRange(INT32 ClassID, INT32 RequestedAttributeNumber)
{
	bool IsRequestValid = false;
	char StatusMessage[MESSSAGE_SIZE] = "";
	
	INT32 MapSize = m_MapClassesInfo[ClassID].AttributesMap.size();
	
	if(RequestedAttributeNumber <= MapSize)
	{
		IsRequestValid = true;
	}
	
	else
	{

		sprintf(StatusMessage,"\n REQUESTED ATTRIBUTE NUMBER WAS NOT FOUND %d",RequestedAttributeNumber );
		LOGERR(StatusMessage,0,0,0);
	 }

	return IsRequestValid;
}

/**************************************************************************//**
* \brief  - This function verifies that the requested Vendor Specific Class
* 			Instance falls within valid range
* 
* \param  - INT32 ClassID
*
* \return - bool IsRequestValid
* 
******************************************************************************/
bool VendorSpecificInfo::IsInstanceWithinRange(INT32 ClassID, INT32 RequestedInstanceNumber)
{
	bool IsRequestValid = false;
	char StatusMessage[MESSSAGE_SIZE] = "";
	
	INT32 MaxInstance = m_MapClassesInfo[ClassID].InstanceLimit;
	
	if(RequestedInstanceNumber >= 0 && RequestedInstanceNumber <= MaxInstance)
	{
		IsRequestValid = true;
	}
	
	else
	{

		sprintf(StatusMessage,"\n REQUESTED INSTANCE NUMBER NOT WITHIN VALID RANGE %d",RequestedInstanceNumber );
		LOGERR(StatusMessage,0,0,0);
	 }

	return IsRequestValid;
}


/**************************************************************************//**
* \brief  - This function verifies that the requested Attribute Access Type
* 			is valid
* 
* \param  - INT32 ClassID, INT32 RequestedAttributeNumber,uint8_t CIPServiceCode
*
* \return - bool IsRequestValid
* 
******************************************************************************/
bool VendorSpecificInfo::IsAttributeAccessTypeValid(INT32 ClassID, INT32 RequestedAttributeNumber,uint8_t CIPServiceCode)
{
	bool IsRequestValid = false;
	char StatusMessage[MESSSAGE_SIZE] = "";
	
	switch (CIPServiceCode)
	{
	case Get_Attributes_All:
		//Intentional fall through
	case Get_Attribute_Single:
		if(m_MapClassesInfo[ClassID].AttributesMap[RequestedAttributeNumber].AllowedAccessType >= READ_ATTR)
			IsRequestValid = true;
		break;
		
	case Set_Attributes_All:
		//Intentional fall through
	case Set_Attribute_Single:
		if(m_MapClassesInfo[ClassID].AttributesMap[RequestedAttributeNumber].AllowedAccessType >= READ_WRITE_ATTR)
			IsRequestValid = true;
		break;
		
	default:
		sprintf(StatusMessage,"\n REQUESTED ATTRIBUTE ACCESS TYPE IS NOT VALID %d", CIPServiceCode );
		LOGERR(StatusMessage,0,0,0);
		break;
	}

	return IsRequestValid;
}

/**************************************************************************//**
* \brief  - Uses the elements of the Class and Attributes maps to make a query 
* 			based on Explicit Message Request parameters
* 
* \param  - INT32 CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,  
* 			 uint8_t * Data, UINT32 * SizeToSend, INT32 AttributeNumber
*
* \return - DataWasProcessed
* 
******************************************************************************/
bool VendorSpecificInfo::ProcessDatabaseRequest(uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,   uint8_t * Data, UINT32 * SizeToSend, INT32 AttributeNumber)
{
	bool DataWasProcessed = false;
	char SQLStatement[SQL_QUERY_SIZE] = "";

	INT32 queryStatus = RESPONSE_FAIL;
	string  queryReturn = "";
	string selector = "";
	if (CIPServiceCode == Get_Attribute_Single)
	{
		selector = m_MapClassesInfo[ClassID].AttributesMap[AttributeNumber].DBColumnName;
	}
	else if(CIPServiceCode == Get_Attributes_All)
	{
		selector = "*";
	}
	
	switch(CIPServiceCode)
	{
	case Get_Attributes_All:
		//Intentional fall through
	case Get_Attribute_Single:
		sprintf(SQLStatement, "\nSELECT %s FROM %s WHERE %s = %d",selector.c_str(),
			m_MapClassesInfo[ClassID].DBTableName.c_str(),
			m_MapClassesInfo[ClassID].DBTableUniqueID.c_str(),
			InstanceNumber);
		break;
		
	case Set_Attributes_All:
		//Intentional fall through
	case Set_Attribute_Single:
		//placeholder for future functionality
		break;
		
	default:
		break;
	}
	queryReturn = m_DbConn.Exec(SQLStatement, &queryStatus);

	if(SQLITE_OK != queryStatus)
		LOGERR("\nERROR EXECUTING QUERY, STATUS:",queryStatus,0,0);
	else
	{
		if(queryReturn.size() > 0)
			DataWasProcessed = ProcessDataBuffer(queryReturn,ClassID , Data, SizeToSend, AttributeNumber);
		else
			LOGERR("\nREQUESTED DATA WAS NOT FOUND",0,0,0);
	}
	
	return DataWasProcessed;
}

/**************************************************************************//**
* \brief  - Prepares the data from the database to be sent as UINT8 data
* 
* \param  - string queryResult, INT32 ClassID , uint8_t * Data, UINT32 * SizeToSend
*
* \return - DataWasFormatted
* 
******************************************************************************/
bool VendorSpecificInfo::ProcessDataBuffer(string queryResult, INT32 ClassID , uint8_t * Data, UINT32 * SizeToSend,INT32 AttributeNumber)
{
	bool DataWasFormatted = false;
	INT32 DataToSend[MAX_INT32_TO_SEND];
	INT32 DTSCounter = 0;
	vector <string>vecQueryResultData;
	vector <string>vecDateTime;
	vector <string>vecTime;
	vector <string>vecDate;
	INT32 dwIndex = 0;
	INT32 Size = 0;
	
	INT32 vecQueryResultDataVal = 0 ;

	INT32 DataType = DB_DEFAULT;
	if(queryResult.size() > 0)//ToDo determine upper bound size
	{
		CP->StringToTokens(queryResult.c_str(),vecQueryResultData,',');
		
		for(dwIndex = 0 ; dwIndex < vecQueryResultData.size() ; dwIndex++)
		{
			if (vecQueryResultData.size()>1)
				DataType = m_MapClassesInfo[ClassID].AttributesMap[dwIndex+1].DataType ;
			else
				DataType = m_MapClassesInfo[ClassID].AttributesMap[AttributeNumber].DataType ;
			
			if(DTSCounter<MAX_INT32_TO_SEND)
			{
				switch(DataType)
				{
					case	DB_INTEGER:
					{
						vecQueryResultDataVal = atoi(vecQueryResultData[dwIndex].c_str());
						DataToSend[DTSCounter] = vecQueryResultDataVal;
						DTSCounter++;
						break;
					}
					case	DB_DATETIME:
					{
						CP->StringToTokens(vecQueryResultData[dwIndex].c_str(),vecDateTime,' ');
						CP->StringToTokens(vecDateTime[0].c_str(),vecDate,'/');
						CP->StringToTokens(vecDateTime[1].c_str(),vecTime,':');
						
						for(INT32 f = 0; f<3;f++)
						{
							DataToSend[DTSCounter] = atoi(vecDate[f].c_str());
							DTSCounter++;
						}
						
						for(INT32 f = 0; f<3;f++)
						{
							DataToSend[DTSCounter] = atoi(vecTime[f].c_str());
							DTSCounter++;
						}
						break;
					}
					case	DB_TEXT:
					{
						char textArr[MAX_CHAR_ARR_SIZE];
						strncpy(textArr, vecQueryResultData[dwIndex].c_str(), sizeof(vecQueryResultData[dwIndex]));
						
						for(INT32 i = 0; i<sizeof(vecQueryResultData[dwIndex]) ; i++)
						{
							DataToSend[DTSCounter] = (INT32)textArr[i];
							DTSCounter++;
						}
						break;
					}
					case	DB_REAL:
					{
						float dval = atof(vecQueryResultData[dwIndex].c_str());
						vecQueryResultDataVal = static_cast<INT32>(dval*10);
						DataToSend[DTSCounter] = vecQueryResultDataVal;
						DTSCounter++;
						break;
					}
					case	DB_BOOLEAN:
					{
						vecQueryResultDataVal = atoi(vecQueryResultData[dwIndex].c_str());
						DataToSend[DTSCounter] = vecQueryResultDataVal;
						DTSCounter++;
						break;
					}
					default:
					{
						DataToSend[DTSCounter] = 0;
						DTSCounter++;
						break;
					}
				}

			}
		}
		*SizeToSend = DTSCounter*INT_TO_UINT8;		
		memcpy(Data, DataToSend, *SizeToSend);
		DataWasFormatted = true;
	}
	return DataWasFormatted;
}

/**************************************************************************//**
* \brief  - Decides where to retrieve the requested data from
* 
* \param  -uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,   
* 			uint8_t * Data, UINT32 * SizeToSend, INT32 AttributeNumber
* 			
* \return - DataWasRetrieved
* 
******************************************************************************/
bool VendorSpecificInfo::GetData(uint8_t CIPServiceCode, INT32 InstanceNumber, INT32 ClassID,   uint8_t * Data, UINT32 * SizeToSend, INT32 AttributeNumber)
{
	bool DataWasRetrieved = false;
	
	switch(ClassID)
	{
		case	WELD_RESULTS_OBJECT:
		{
			DataWasRetrieved = ProcessDatabaseRequest( CIPServiceCode,  InstanceNumber,  ClassID,   Data,  SizeToSend,  AttributeNumber);
			break;
		}
		case	LAST_WELD_RESULTS_OBJECT:
		{
			string  sResults = "";
			if (CIPServiceCode == Get_Attribute_Single)
			{
				sResults = GetLastWeldResultData(AttributeNumber);
				
			}
			else if(CIPServiceCode == Get_Attributes_All)
			{
				for(int i = 1; i <= LAST_WELD_RESULTS_OBJECT_MAP_SIZE;i++)
				{
					string tempRes = "";
					tempRes = GetLastWeldResultData(i);
					sResults+= tempRes +",";
				}
			}
			DataWasRetrieved = ProcessDataBuffer( sResults,ClassID , Data, SizeToSend, AttributeNumber);
			break;
		}
		default:
			break;
	}
	
	
	
	
	return DataWasRetrieved;
}


/**************************************************************************//**
* \brief  - Gets the value of the requested Attribute from Last Weld
* 			results object
* 
* \param  - INT32 AttributeNumber
*
* \return - AttributeVal
* 
******************************************************************************/
string VendorSpecificInfo::GetLastWeldResultData (INT32 AttributeNumber)
{
	char AttributeVal[MAX_INT32_TO_SEND] = "" ;
	string strRetResult;
		
	switch (AttributeNumber)
	{
		case WD_MAXWELDFORCE:
			sprintf(AttributeVal , "%d", LastWeldResults.MaxWeldForce);
			break; 
		case WD_ENDHOLDFORCE:
			sprintf(AttributeVal , "%d",LastWeldResults.EndHoldForce);
			break; 
		case WD_WELDABSOLUTE:
			sprintf(AttributeVal , "%d",LastWeldResults.WeldAbsolute);
			break; 
		case WD_TOTALABSOLUTE:
			sprintf(AttributeVal , "%d",LastWeldResults.TotalAbsolute);
			break; 
		case WD_CYCLECOUNTER:
			sprintf(AttributeVal , "%d",LastWeldResults.CycleCounter);
			break; 
		case WD_RECIPENUMBER:
			sprintf(AttributeVal , "%d",LastWeldResults.RecipeNumber);
			break; 
		case WD_WELDTIME:
			sprintf(AttributeVal , "%d",LastWeldResults.WeldTime);
			break; 
		case WD_CYCLETIME:
			sprintf(AttributeVal , "%d",LastWeldResults.CycleTime);
			break; 
		case WD_WELDENERGY:
			sprintf(AttributeVal , "%d",LastWeldResults.TotalEnergy);
			break; 
		case WD_WELDPEAKPOWER:
			sprintf(AttributeVal , "%d",LastWeldResults.PeakPower);
			break; 
		case WD_STARTFREQUENCY:
			sprintf(AttributeVal , "%d",LastWeldResults.StartFrequency);
			break; 
		case WD_FREQUENCYCHANGE:
			sprintf(AttributeVal , "%d",LastWeldResults.FrequencyChange);
			break; 
		case WD_WELDCOLLAPSEDISTANCE:
			sprintf(AttributeVal , "%d",LastWeldResults.WeldCollapseDistance);
			break; 
		case WD_HOLDCOLLAPSEDISTANCE:
			sprintf(AttributeVal , "%d",LastWeldResults.HoldCollapseDistance);
			break; 
		case WD_TOTALCOLLAPSEDISTANCE:
			sprintf(AttributeVal , "%d",LastWeldResults.TotalCollapseDistance);
			break; 
		case WD_TRIGGERPOINT:
			sprintf(AttributeVal , "%d",LastWeldResults.TriggerTime);
			break; 
		case WD_WELDSONICPOINT:
			sprintf(AttributeVal , "%d",LastWeldResults.WeldSonicTime);
			break; 
		case WD_HOLDPOINT:
			sprintf(AttributeVal , "%d",LastWeldResults.HoldTime);
			break; 
		case WD_TRIGGERDISTANCE:
			sprintf(AttributeVal ,"%d",LastWeldResults.TriggerDistance);
			break;
		case WD_VELOCITY:
			sprintf(AttributeVal ,"%d",LastWeldResults.Velocity);
			break; 
		case WD_ALARMFLAG:
			sprintf(AttributeVal , "%d",LastWeldResults.bIsAlarmflag);
			break; 
		case WD_WELDRECIPEVERNUMBER:
			sprintf(AttributeVal , "%d",LastWeldResults.RecipeRevNumber);
			break; 
		case WD_WELDMODE:
			sprintf(AttributeVal , "%d",LastWeldResults.WeldMode);
			break; 
		case WD_USERNAME:
			sprintf(AttributeVal , "%s",LastWeldResults.UserName);
			break; 
		case WD_STACKSERIALNO:
			sprintf(AttributeVal , "%d",LastWeldResults.StackSerialNo);
			break; 
		case WD_RECIPESTATUS:
			sprintf(AttributeVal , "%d",LastWeldResults.recipeStatus);
			break; 
		case WD_BATCHID:
			sprintf(AttributeVal , "%s",LastWeldBatchID);
			break; 
		case WD_DATETIME:
			sprintf(AttributeVal , "%s",LastWeldDateTime.c_str());
			break; 
		case WD_PARTID:
			sprintf(AttributeVal , "%s",LastWeldResults.PartID);
			break; 
		default:
			sprintf(AttributeVal , 	"%d",0);
			LOGERR("WeldResult : Invalid parameter index passed : %d",AttributeNumber,0,0);
			break;
	}
	
	strRetResult.append(AttributeVal);

	return strRetResult;
}

/**************************************************************************//**
* \brief  - Updates the Last Weld Result Object
* 
* \param  - WeldResult result,char * cBatchID,string sDateTime
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::UpdateLastWeldResultsData(WeldResult result,char * cBatchID,string sDateTime)
{
	memcpy(&LastWeldResults,&result,sizeof(LastWeldResults));
	LastWeldDateTime = sDateTime;
	sprintf(LastWeldBatchID , 	"%s",cBatchID);
}


/**************************************************************************//**
* \brief  - Initializes Last Weld Result Object 
* 
* \param  - None
*
* \return - None
* 
******************************************************************************/
void VendorSpecificInfo::InitializeLastWeldResults()
{
	LastWeldResults.MaxWeldForce 		= 0;
	LastWeldResults.EndHoldForce 		= 0;
	LastWeldResults.TotalAbsolute 		= 0;
	LastWeldResults.Velocity            = 0;
	LastWeldResults.TriggerDistance     = 0;
	LastWeldResults.CycleCounter 		= 0;
	LastWeldResults.RecipeNumber 		= 0;
	LastWeldResults.RecipeRevNumber		= 0;
	LastWeldResults.WeldTime 			= 0;
	LastWeldResults.CycleTime			= 0;
	LastWeldResults.TotalEnergy 		= 0;
	LastWeldResults.PeakPower 			= 0;
	LastWeldResults.StartFrequency 		= 0;
	LastWeldResults.FrequencyChange 	= 0;
	LastWeldResults.WeldCollapseDistance= 0;
	LastWeldResults.HoldCollapseDistance= 0;
	LastWeldResults.TriggerTime 		= 0;
	LastWeldResults.WeldSonicTime 		= 0;
	LastWeldResults.HoldTime 			= 0;
	LastWeldResults.bIsAlarmflag		= false;
	LastWeldResults.WeldStatus			= 0;
	LastWeldResults.WeldMode			= 0;
	LastWeldResults.StackSerialNo		= 0;
	
	LastWeldDateTime = "0000/00/00 00:00:00";
	sprintf(LastWeldBatchID , 	"%s","0");
	strcpy(LastWeldResults.UserName,"0");	
}
