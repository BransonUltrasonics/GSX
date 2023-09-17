/*
************************************************************************************************************
*      File    :   ACOD.h
*      Author  :   sunilkumar.m@sasken.com  
*      Module  :   AC
*      Date    :   09/10/2017
************************************************************************************************************
*  VERSION HISTORY
************************************************************************************************************
*  VERSION     DATE        AUTHOR               COMMENTS
************************************************************************************************************
*      1     09/10/2017    Sunil			Object Dictionary function declarations
*      2 					
************************************************************************************************************
*/


#ifndef ACOD_H_
#define ACOD_H_

/* Header files */
#include <vxWorks.h>
#include <sys/types.h>
#include <stdlib.h>
#include "AcCommon.h"
#include "FirmwareUpgrade.h"


/* AC Object Dictionary class to get OD data properties like Offset, Object Size etc... */
class ACOD
{
	//To let control task access OD members
	friend class AcControlTask;		
	friend class FirmwareUpgrade;
	public:
		ACOD();
		~ACOD();
		void GetODDataProp();
	
	private:
		// To Get the pointer to the Real time data and Non Real time data of OD
		OBJCONST TOBJECT OBJMEM* pObjEntryRt;				
		OBJCONST TOBJECT OBJMEM* pObjEntryWrecipe;		
		OBJCONST TOBJECT OBJMEM* pObjEntryDiagData;	
		OBJCONST TOBJECT OBJMEM* pObjEntryFWInfo;			
		OBJCONST TOBJECT OBJMEM* pObjEntryCalibration;	
		OBJCONST TOBJECT OBJMEM* pObjEntryFWUpgrade;
		
		// To find the Length of the Real time data and Non Real time data Object
		ULONG ObjSizeRt;
		ULONG ObjSizeWrecipe;
		ULONG ObjSizeDiagData;
		ULONG ObjSizeFWInfo; 	
		ULONG ObjSizeCalibration; 
		ULONG ObjSizeFWUPGRDInfo; 
		
		// Holds the offset of Real time data and Non Real time data entries
		UINT16 offsetRt[NUM_OF_TXPDO_ENTRY];
		UINT16 offsetWrecipe[NUM_OF_WELD_RECIPE_ENTRY];
		UINT16 offsetDiagData[NUM_OF_DIAGNOSTIC_DATA_ENTRY];
		UINT16 offsetFWInfo[NUM_OF_FW_INFO_ENTRY];			
		UINT16 offsetCalibration[NUM_OF_CALIBRATION_ENTRY];
		UINT16 offsetFWUpgrade[NUM_OF_FWUPGRD_INFO_ENTRY];
		

		
		// Buffer to store the OD data after ESS Object Read
		USHORT MBXMEM* bufferRt;
		USHORT MBXMEM* bufferWrecipe;
		USHORT MBXMEM* bufferCalibration;
		USHORT MBXMEM* bufferFwUpgrade;
		// Size of each entry in TxPdo and Weld Result data required to write to OD using obj_writeSc
		UINT8 TxPdoSizes[NUM_OF_TXPDO_ENTRY];
		UINT8 DiagDataSizes[NUM_OF_DIAGNOSTIC_DATA_ENTRY];
		UINT8 FWInfoSizes[NUM_OF_FW_INFO_ENTRY];	
		UINT8 FWUPGRDSizes[NUM_OF_FWUPGRD_INFO_ENTRY];
};


/* Class objects */
namespace ACObjDict
{
	ACOD acOD;
}



#endif /* ACOD_H_ */
