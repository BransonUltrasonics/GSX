/**
 * ecatSlaveAppl.h
 * 
 *
 *
*/

// Comment-1 for test

#ifdef _PRUESC_HW_
    #define PROTO
#else
    #define PROTO extern
#endif

#if PRUESC_HW
#include <ecatSlaveHw.h>

#define TXPDO1_MAP_NO_ENTRIES 		5
#define RXPDO_MAP_NO_ENTRIES		3
#define MAX_NUM_OF_FORCE_STEPS		10

/*-----------------------------------------------------------------------------------------
------
------    ECAT SLAVE application specific objects
------
-----------------------------------------------------------------------------------------*/

/******************************************************************************		
*					Object 0x1A04: AC Info TxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1A04[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}
};

OBJCONST Uint8 OBJMEM aName0x1A04[] = "TxPDO Mapping 001\000SubIndex 001\000SubIndex 002\000SubIndex 003\000SubIndex 004\000SubIndex 005\000\377";
#endif

typedef struct STRUCT_PACKED_START {
		Uint16 	  u16SubIndex0;			
		Uint32    SubIndex001; 		
		Uint32    SubIndex002;			
		Uint32	  SubIndex003;		
		Uint32	  SubIndex004;				
		Uint32	  SubIndex005;
}STRUCT_PACKED_END
TOOBJ1A04;

PROTO TOOBJ1A04 sAITxPDO1Map				
#ifdef _PRUESC_HW_
= {TXPDO1_MAP_NO_ENTRIES, 0x40030110, 0x40030220, 0x40030308, 0x40030408, 0x40030508}		
#endif
;

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x4003[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},		//RxPdo
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ}
};

OBJCONST Uint8 OBJMEM aName0x4003[] = "Real Time Data\000Actual Distance\000Actual Force\000Ac Status Event\000Ac State\000Ac Inputs\000Sc State\000Ac Control Event\000Ac Outputs\000\377";
#endif

typedef struct STRUCT_PACKED_START {
		Uint16 	  u16SubIndex0;			
		Uint16    ActualForce;	//TxPdo		
		Uint32    ActualDistance;			
		Uint8	  AcStatusEvent;		
		Uint8	  AcState;				
		Uint8	  AcInputs;
		Uint8	  ScState;		//RxPdo		
		Uint8 	  AcControlEvent;		
		Uint8	  AcOutputs;	
}STRUCT_PACKED_END
TOOBJ4003;

PROTO TOOBJ4003 sRealTimeData				
#ifdef _PRUESC_HW_
= {(TXPDO1_MAP_NO_ENTRIES + RXPDO_MAP_NO_ENTRIES), 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}		
#endif
;

/******************************************************************************		
*					Object 0x1603: AC RxPDO
******************************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1603[] = {
		{DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}	
};

OBJCONST Uint8 OBJMEM aName0x1603[] = "RxPDO mapping 001\000SubIndex 0001\000SubIndex 002\000SubIndex 003\000\377";
#endif

typedef struct STRUCT_PACKED_START {
	        Uint16 	  u16SubIndex0;			
			Uint32    SubIndex001 ;
			Uint32    SubIndex002;			
			Uint32	  SubIndex003;
}STRUCT_PACKED_END
TOOBJ1603;

PROTO TOOBJ1603 sAIRxPDO1Map
#ifdef _PRUESC_HW_
= {RXPDO_MAP_NO_ENTRIES, 0x40030608, 0x40030708, 0x40030808}
#endif
; 



/******************************************************************************		
*					Object 0x1C13: TxPDO assignment
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1C13[] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}
   };
OBJCONST Uint8 OBJMEM aName0x1C13[] = "TxPDO assign";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint16   aEntries[1];
} STRUCT_PACKED_END
TOBJ1C13;

PROTO TOBJ1C13 sTxPDOassign
#ifdef _PRUESC_HW_
= {0x01, {0x1A04}}
#endif
;

/******************************************************************************		
*					Object 0x1C12: RxPDO assignment
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1C12[] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
   //{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}};
OBJCONST Uint8 OBJMEM aName0x1C12[] = "RxPDO assign";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint16   aEntries[1];
} STRUCT_PACKED_END
TOBJ1C12;


PROTO TOBJ1C12 sRxPDOassign
#ifdef _PRUESC_HW_
= {0x01, {0x1603}}
#endif
;



/************************************************************
*				Object 0x4000: Weld Recipe Data
************************************************************/
/* Sample SDO data to observe on twinCAT*/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4000[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},		//ARRAY	
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},		//ARRAY	
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},		/* ARRAY END */
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}
};


OBJCONST Uint8 OBJMEM aName0x4000[] = "Weld Recipe Data\000Weld Force\000Force Ramp Time\000Hold Mode\000Total Collapse Target\000Hold Force\000Hold Force Ramp Time\000Expected PC Position\000Ready Position\000Down Acceleration\000Down Max Velocity\000Down Deceleration\000Return Acceleration\000Return Max Velocity\000Return Deceleration\000Expected PartContact Offset\000PartContact WindowMinus\000PartContact WindowPlus\000Num Force Steps\000Force Step Ramp Time 1\000Force Step Ramp Time 2\000Force Step Ramp Time 3\000Force Step Ramp Time 4\000Force Step Ramp Time 5\000Force Step Ramp Time 6\000Force Step Ramp Time 6\000Force Step Ramp Time 7\000Force Step Ramp Time 8\000Force Step Ramp Time 9\000Force Step Ramp Time 10\000Force Step 1\000Force Step 2\000Force Step 3\000Force Step 4\000Force Step 5\000Force Step 6\000Force Step 7\000Force Step 8\000Force Step 9\000Force Step 10\000ReadyPositionToggle\000WeldForceControl\000\377";
#endif

typedef struct STRUCT_PACKED_START {
	Uint16 u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
	Uint16 WeldForce;
	Uint16 ForceRampTime;
	Uint16 HoldMode;	
	Uint16 TotalCollapseTarget;
	Uint16 HoldForce;
	Uint16 HoldForceRampTime;
	Uint32 ExpectedPCPosition;
	Uint32 ReadyPosition;
	Uint16 DownAcceleration;
	Uint16 DownMaxVelocity;
	Uint16 DownDeceleration;
	Uint16 ReturnAcceleration;
	Uint16 ReturnMaxVelocity;
	Uint16 ReturnDeceleration;
	Uint16 ExpectedPartContactOffset;
	Uint16 PartContactWindowMinus; 
	Uint16 PartContactWindowPlus;
	Uint16 NumForceSteps;
	Uint16 ForceStepRampTime[MAX_NUM_OF_FORCE_STEPS];
	Uint16 ForceStepForce[MAX_NUM_OF_FORCE_STEPS];
	Uint16 ReadyPositionToggle;
	Uint16 WeldForceControl;
} STRUCT_PACKED_END
TWELDRECIPEPARAMS;

PROTO TWELDRECIPEPARAMS sWeldRecipeParams			
#ifdef _PRUESC_HW_
= {40, 0xFFFA, 0xFFFB, 0xFFFC, 0xFFFD, 0xFFFE, 0xFFFF, 0xFFFFFFFE, 0xFFFFFFFD, 0xFFEA, 0xFFEB, 0xFFEC, 0xFFED, 0xFFEE, 0xFFEF, 0xFFE1, 0xFFE2, 0xFFE3, 0xFFE4, {0xFFFA, 0xFFFB, 0xFFFC, 0xFFFD, 0xFFFE, 0xFFFF, 0xFFC0, 0xFFC1, 0xFFC2, 0xFFC3}, {0xEFFA, 0xEFFB, 0xEFFC, 0xEFFD, 0xEFFE, 0xEFFF, 0xEFC0, 0xEFC1, 0xEFC2, 0xEFC3}, 0x0000, 0x0003}
#endif
;


/************************************************************
*				Object 0x4001: Weld Result Structure
************************************************************/
/* Sample SDO data to observe on twinCAT*/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4001[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}	
};


OBJCONST Uint8 OBJMEM aName0x4001[] = "Weld Result Data\000Max Weld Force\000Actual Force\000Total Absolute\000PC position\000\377";
#endif

typedef struct {
	    Uint16  u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
	   	Uint16  MaxWeldForce;
	   	Uint16  MaxHoldForce;
	   	Uint32  TotalAbsolute;
	   	Uint32  PCposition;  
} STRUCT_PACKED_END
TWELDRESULTPARAMS;

PROTO TWELDRESULTPARAMS sWeldResultParams			
#ifdef _PRUESC_HW_
= {4, 0xFFF7, 0xFFF8, 0xFFFFFFFE, 0xFFFFFFFD}
#endif
;

/************************************************************
*				Object 0x4005: Diagnostic Data Structure
*****************************************************************/


#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4005[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE}	,
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},		
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}
	
};
OBJCONST Uint8 OBJMEM aName0x4005[] = "Diagnostic Data\000AC Alarm \000Actuator Type\000Jog Speed\000GTP Speed\000GTP Position\000Spring Constant\000Callibration Force Reading\000Actuator Cycles\000Cycle Since Actuator Maintenance\000Cycle Since Full Stroke\000FW Version\000FW CRC\000\377";
#endif


typedef struct {
	    Uint16  u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
	   	Uint8  ACAlarm;
	   	Uint8  ActuatorType;
		Uint8  JogSpeed;
		Uint8  GTPSpeed;
	   	Uint32  GTPPosition;	   	
		Uint32 SpringConstant;
		Uint16 CallibrationForceReading;
		Uint32 ActuatorCycles;
		Uint32 CycleSinceActuatorMaintenance;
		Uint32 CycleSinceFullStroke;
		Uint32 FWVersion;
		Uint16 FWCRC;
		
		  
} STRUCT_PACKED_END
TDIAGNOSTICDATAPARAMS;

PROTO TDIAGNOSTICDATAPARAMS sDiagnosticDataParams			
#ifdef _PRUESC_HW_
= {12, 0x00, 0x0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

#endif
;


/************************************************************
*				Object 0x4002: Firmware Info Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4002[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
};


OBJCONST Uint8 OBJMEM aName0x4002[] = "Version Info Data\000FwMajor\000FwMinor\000FwBuild\000FwCrc\000FwInfo\000\377";
#endif

typedef struct {
	    Uint16  u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
		UINT16 	FwMajor;
		UINT16 	FwMinor;
		UINT16 	FwBuild;
		UINT16	FwCrc;
		UINT32 	FwInfo;
} STRUCT_PACKED_END
TFWINFOPARAMS;

PROTO TFWINFOPARAMS sFirmwareInfoParams			
#ifdef _PRUESC_HW_
= {5, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif
;


/************************************************************
*				Object 0x4006Alarm Data Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4006[] = {
                {DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
                {DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE }
};

OBJCONST Uint8 OBJMEM aName0x4006[] = "Alarm Data Structure\000Alarms Type \000\377" ;
#endif

typedef struct
{
        Uint16 NumOfEntries;
        Uint32 AlarmTypeMask;
}STRUCT_PACKED_END
TALARMDATAPARAMS;

PROTO TALARMDATAPARAMS sAlarmDataParams
#ifdef _PRUESC_HW_
= { 2 , 0x00 , 0x00 }
#endif
;



/************************************************************
*				Object 0x4004: Calibration Readings Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4004[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
};


OBJCONST Uint8 OBJMEM aName0x4004[] = "Calibration Readings Data\000firstReading\000secondReading\000tm_sec\000tm_min\000tm_hour\000tm_mday\000tm_mon\000tm_year\000tm_wday\000tm_yday\000tm_isdst\000\377";
#endif

typedef struct {
	    Uint16  u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
		UINT32 	firstReading;
		UINT32 	secondReading;
		Uint16 tm_sec;         /* seconds after the minute     - [0, 59] */
		Uint16 tm_min;         /* minutes after the hour       - [0, 59] */
		Uint16 tm_hour;        /* hours after midnight         - [0, 23] */
		Uint16 tm_mday;        /* day of the month             - [1, 31] */
		Uint16 tm_mon;         /* months since January         - [0, 11] */
		Uint16 tm_year;        /* years since 1900     */
		Uint16 tm_wday;        /* days since Sunday            - [0, 6] */
		Uint16 tm_yday;        /* days since January 1         - [0, 365] */
		Uint16 tm_isdst;       /* Daylight Saving Time flag */
} STRUCT_PACKED_END
TCALIBRATIONPARAMS;

PROTO TCALIBRATIONPARAMS sCalibrationReadingsParams			
#ifdef _PRUESC_HW_
= {11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif
;

/************************************************************
*				Object 0x4020: Firmware Upgrade Information
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x4020[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE},
};


OBJCONST Uint8 OBJMEM aName0x4020[] = "Firmware Upgrade Information\000CRC_Checksum\000upgrade_status\000progress\000\377";
#endif

typedef struct {
	    Uint16  u16SubIndex0;			//Contains the highest Subindex (last Object Entry)
		UINT16 	CRC_Checksum;
		UINT8 	upgrade_status;
		UINT8   progress;
} STRUCT_PACKED_END
TFWUPGRADEINFO;

PROTO TFWUPGRADEINFO sFirmwareUpgradeInfoparams		
#ifdef _PRUESC_HW_
= {3, 0x00, 0x00, 0x00}
#endif
;

/***********************************************************/		

#ifdef _OBJD_
/*if _PIC18 is enabled the object dictionary is fixed defined in coeappl.c*/
TOBJECT	OBJMEM ApplicationObjDic[] = {		   		
	/* Object 0x1603 */
	{NULL,NULL,  0x1603, {DEFTYPE_PDOMAPPING, 3 | (OBJCODE_REC << 8)}, asEntryDesc0x1603, aName0x1603, &sAIRxPDO1Map, NULL, NULL, 0x0000 },
		   
	/* Object 0x1A04 */
	{NULL,NULL,  0x1A04, {DEFTYPE_PDOMAPPING, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x1A04, aName0x1A04, &sAITxPDO1Map, NULL, NULL, 0x0000 },    
   
	/* Object 0x4003 */
	{NULL,NULL,  0x4003, {DEFTYPE_PDOMAPPING, 8 | (OBJCODE_REC << 8)}, asEntryDesc0x4003, aName0x4003, &sRealTimeData, NULL, NULL, 0x0000 }, 

		      	
	
    /* Object 0x1C12 */
   {NULL,NULL,   0x1C12, {DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C12, aName0x1C12, &sRxPDOassign, NULL, NULL, 0x0000 },
     /* Object 0x1C13 */
    {NULL,NULL,   0x1C13, {DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C13, aName0x1C13, &sTxPDOassign, NULL, NULL, 0x0000 },
	
	
				
		
	/* Object 0x4000 */		//TODO
	{NULL,NULL,   0x4000, {DEFTYPE_RECORD, 40 | (OBJCODE_REC << 8)}, asEntryDesc0x4000, aName0x4000, &sWeldRecipeParams, NULL, NULL, 0x0000 },
	
	/* Object 0x4001 */
	{NULL,NULL,   0x4001, {DEFTYPE_RECORD, 4 | (OBJCODE_REC << 8)}, asEntryDesc0x4001, aName0x4001, &sWeldResultParams, NULL, NULL, 0x0000 },
	
	/* Object 0x4005 */
	{NULL,NULL,  0x4005, {DEFTYPE_RECORD, 12 | (OBJCODE_REC << 8)}, asEntryDesc0x4005, aName0x4005, &sDiagnosticDataParams, NULL, NULL, 0x0000 }, 
	
	/* Object 0x4002 */
 
	{NULL,NULL,   0x4002, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x4002, aName0x4002, &sFirmwareInfoParams, NULL, NULL, 0x0000 },

	/* Object 0x4004 */ 
	{NULL,NULL,   0x4004, {DEFTYPE_RECORD, 11 | (OBJCODE_REC << 8)}, asEntryDesc0x4004, aName0x4004, &sCalibrationReadingsParams, NULL, NULL, 0x0000 },
	
	/* Object 0x4006 */ 
	{NULL,NULL,  0x4006, {DEFTYPE_RECORD, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x4006, aName0x4006, &sAlarmDataParams, NULL, NULL, 0x0000 },

	/* Object 0x4020 */ 
	{NULL,NULL,   0x4020, {DEFTYPE_RECORD, 3 | (OBJCODE_REC << 8)}, asEntryDesc0x4020, aName0x4020, &sFirmwareUpgradeInfoparams, NULL, NULL, 0x0000 },
		
	{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL, NULL, 0x0000}};
#endif	//#ifdef _OBJD_

#if EXPLICIT_DEVICE_ID
PROTO Uint16 APPL_GetDeviceID();
#endif
/*ECATCHANGE_START(V4.42.1) ECAT2*/
PROTO void APPL_Application(void);

PROTO void   APPL_AckErrorInd(Uint16 stateTrans);
PROTO Uint16 APPL_StartMailboxHandler(void);
PROTO Uint16 APPL_StopMailboxHandler(void);
PROTO Uint16 APPL_StartInputHandler(Uint16 *pIntMask);
PROTO Uint16 APPL_StopInputHandler(void);
PROTO Uint16 APPL_StartOutputHandler(void);
PROTO Uint16 APPL_StopOutputHandler(void);

PROTO Uint16 APPL_GenerateMapping(Uint16 *pInputSize,Uint16 *pOutputSize);
PROTO void APPL_InputMapping(Uint16* pData);
PROTO void APPL_OutputMapping(Uint16* pData);

/*ECATCHANGE_END(V4.42.1) ECAT2*/

#endif //#if PRUESC_HW

#undef PROTO

// Comment-2 for test
