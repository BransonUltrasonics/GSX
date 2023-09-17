/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/



#ifndef ACCOMMON_H_
#define ACCOMMON_H_

/* Header files and Macros */
#include "AC.h"
extern "C"
{
	#include <customSystemCall.h>
}
#include "DynamicSystemCall.h"
#define TX_RX_PDO_INDEX				0x4003
#define WELD_RECIPE_INDEX   		0x4000
#define WELD_RESULT_INDEX			0x4001
#define FIRMWARE_INFO_INDEX			0x4002
#define DIAGNOSTIC_DATA_INDEX		0x4005
#define CALIBRATION_READINGS_INDEX	0x4004
#define FIRMWARE_UPGRADE_INDEX     	0x4020
#define TXPDO_DATA_SIZE 			9
#define RXPDO_DATA_SIZE 			3
#define NUM_OF_TXPDO_ENTRY 			5
#define NUM_OF_RXPDO_ENTRY 			3
#define NUM_OF_WELD_RECIPE_ENTRY 	35
#define NUM_OF_WELD_RESULT_ENTRY 	4
#define NUM_OF_DIAGNOSTIC_DATA_ENTRY 	12
#define NUM_OF_FW_INFO_ENTRY 		5
#define NUM_OF_CALIBRATION_ENTRY	11
#define NUM_OF_FWUPGRD_INFO_ENTRY 		3
#define SUB_INDEX 					0 		//Sub Index of the Object containing the number of variables
#define COMPLETE_ACCESS 			1 
#define SUCCESS 					0
#define FAILURE 					-1
//#ifndef DEBUG
//#define DEBUG
//#endif
#ifndef ERROR_PRINT 
#define ERROR_PRINT								
#endif
#define LOG ACLog :: pLogInfo->PrintMsg		//For Print
using namespace std;



/* Variables */			

/* AcCtrltaskRt - Real time data from AC to SC and vice-versa. This struct is used to create the local copy of OD */
typedef struct STRUCT_PACKED_START		
	{
	UINT16 	  u16SubIndex0;			//Contains the highest Subindex (last Object Entry)   		
   	UINT16    ActualForce;			
   	UINT32    ActualDistance;
   	UINT8	  AcStatusEvent;			
   	UINT8	  AcState;				
   	UINT8	  AcInputs;  
   	UINT8	  ScState;			
   	UINT8 	  AcControlEvent;		
   	UINT8	  AcOutputs;
   	} STRUCT_PACKED_END AcCtrltaskRt;
   	

   	
/* Class LogInfo for logging the required messages to the console */   	
class LogInfo
{   	 
 	 public:
   		void PrintMsg(char *msg, _Vx_usr_arg_t arg1 = 0, _Vx_usr_arg_t arg2 = 0, _Vx_usr_arg_t arg3 = 0, _Vx_usr_arg_t arg4 = 0, _Vx_usr_arg_t arg5 = 0, _Vx_usr_arg_t arg6 = 0);
		void printWRecipe(AcCtrltaskWeldRecipe &);				   		
};

   	
namespace ACLog
{
	static LogInfo *pLogInfo = NULL;		//For Print
	LogInfo printInfo; 	
}



#endif /* ACCOMMON_H_ */
