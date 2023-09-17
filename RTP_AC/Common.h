#include <stdio.h>
#include "semLib.h"

#ifndef COMMON_H_
#define COMMON_H_



#define MAX_NUM_OF_FORCE_STEPS 		10
#define CYCLES_PER_MS 4

/** 
STRUCT_PACKED_START: Is defined before the typedef struct construct to pack the generic structures if necessary */
#ifndef STRUCT_PACKED_START
#define STRUCT_PACKED_START 
#endif

/** 
STRUCT_PACKED_END: Is defined after the typedef struct {} construct to pack the generic structures if necessary */
#ifndef STRUCT_PACKED_END
#define STRUCT_PACKED_END __attribute__((packed))
#endif


typedef struct {
	UINT16 Priority;
	UINT16 StackSize;
	UINT16 Task_id;
} Task_struct;

//Cmd from SC Master to AC slave
typedef struct Struct_AC_CMD_PDO {
	UINT8 SC_State;
	UINT8 AC_ControlEvent;
	UINT8 AC_Outputs;
} S_AC_CMD_PDO;

//Status from AC slave to SC Master
typedef struct Struct_AC_STATUS_PDO {
	UINT16 ActualForce;
	UINT32 ActualDistance;	
	UINT8 ACStatusEvent;
	UINT8 ACState;	
	UINT8 ACInputs;
} S_AC_STATUS_PDO;

//Cmd from SC Master to PC slave
typedef struct Struct_PC_CMD_PDO {
	UINT16 Amplitude;
	UINT8 SCState;
	UINT8 PC_ControlEvent;
} S_PC_CMD_PDO;

//Status from PC slave to SC Master
typedef struct Struct_PC_STATUS_PDO {
	UINT32 Frequency;
	UINT16 Amplitude;
	UINT16 Phase;
	UINT16 Power;
	UINT16 Current;
	UINT8 PCState;
	UINT8 PC_StatusEvent;
} S_PC_STATUS_PDO;

//Cmd and Status PDO structures
typedef struct PdoStructure {
	S_PC_CMD_PDO PcCmdPdo;
	S_AC_CMD_PDO AcCmdPdo;
	S_PC_STATUS_PDO PcStatusPdo;
	S_AC_STATUS_PDO AcStatusPdo;
} PdoObject;


/* AcCtrltaskWeldRecipe - Non Real time data to AC from SC. Data Address starting from 4000h. This struct is used to 
 * create the local copy of OD and is the input for the Weld operation. */
typedef struct STRUCT_PACKED_START
{	
	UINT16 u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
	UINT16 WeldForce;
	UINT16 ForceRampTime;
	UINT16 HoldMode;
	UINT16 TotalCollapseTarget;
	UINT16 HoldForce;
	UINT16 HoldForceRampTime;
	UINT32 ExpectedPartContactPosition;
	UINT32 ReadyOffset;
	UINT16 DownAcceleration;
	UINT16 DownMaxVelocity;
	UINT16 DownDeceleration;
	UINT16 ReturnAcceleration;
	UINT16 ReturnMaxVelocity;
	UINT16 ReturnDeceleration;
	UINT16 ExpectedPartContactOffset;
	UINT16 PartContactWindowMinus; 
	UINT16 PartContactWindowPlus;
	UINT16 NumForceSteps;		
	UINT16 ForceStepRampTime[MAX_NUM_OF_FORCE_STEPS];
	UINT16 ForceStepForce[MAX_NUM_OF_FORCE_STEPS];
	UINT16 ReadyPositionToggle;
	UINT16 WeldForceControl;
} STRUCT_PACKED_END 
AcCtrltaskWeldRecipe;

typedef struct STRUCT_PACKED_START
{
	UINT16 u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
   	UINT32  FirstReading;
   	UINT32  SecondReading;
   	UINT16 tm_sec;         /* seconds after the minute     - [0, 59] */
	UINT16 tm_min;         /* minutes after the hour       - [0, 59] */
	UINT16 tm_hour;        /* hours after midnight         - [0, 23] */
	UINT16 tm_mday;        /* day of the month             - [1, 31] */
	UINT16 tm_mon;         /* months since January         - [0, 11] */
	UINT16 tm_year;        /* years since 1900     */
	UINT16 tm_wday;        /* days since Sunday            - [0, 6] */
	UINT16 tm_yday;        /* days since January 1         - [0, 365] */
	UINT16 tm_isdst;       /* Daylight Saving Time flag */
} STRUCT_PACKED_END
CalibrationReadings;
   	
typedef struct 
{	
	UINT16  u16SubIndex0;
	UINT16 	CRC_Checksum;
	UINT8 	upgrade_status;
	UINT8   progress;
} 
AcFirmwareUpgradeInfo;
SEM_ID ControlTaskSyncSem;

#endif
