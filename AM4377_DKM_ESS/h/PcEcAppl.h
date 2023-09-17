/**
 * ecatSlaveAppl.h
 * 
 *
 *
*/


#ifdef _PRUESC_HW_
    #define PROTO
#else
    #define PROTO extern
#endif

#if PRUESC_HW
#include <ecatSlaveHw.h>

#define TXPDO_MAP_NO_ENTRIES 		7

#define RXPDO_MAP_NO_ENTRIES		4
#define MAX_NUM_OF_FORCE_STEPS		10
/*-----------------------------------------------------------------------------------------
------
------    ECAT SLAVE application specific objects
------
-----------------------------------------------------------------------------------------*/

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
*				Object 0x2000Weld Recipe Structure
************************************************************/
/* Sample SDO data to observe on twinCAT*/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2000[] = {
		{DEFTYPE_UNSIGNED8 , 0x08 ,  ACCESS_READ },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
        {DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32 ,0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
	};


OBJCONST Uint8 OBJMEM aName0x2000[] = "Weld Recipe Structure\000Amplitude Loop C1\000Amplitude Loop C2\000Phase Loop\000Phase Loop CF\000Frequency Low\000Frequency High\000Phase Limit Time\000Phase Limit\000Control Mode\000Blindtime Seek\000Blindtime weld\000DDS Switchtime\000Parameter7\000Parameter8\000Parameter9\000Parameter10\000Parameter11\000Parameter12\000Parameter13\000Parameter14\000Parameter15\000F-LimitTime\000Amp Proportional Gain\000Amp Integral Gain\000Phase Integral Gain\000Phase ProportionalGain\000Frequency Window Size\000RF of Phase control loop\000PI Frequency Low\000PI Frequency High\000PI PhaseLimit Time\000PI Phase Limit\000Weld Ramp time\000Start Frequency\000Memory Offset\000Digital Tune\000Frequency Offset\000\377" ;
#endif

typedef struct
{
Uint16 Entries ;
Int32 AmplitudeLoopC1;
Int32 AmplitudeLoopC2;
Int32 PhaseLoop ;
Int32 PhaseLoopCF;
Int32 FrequencyLow;
Int32 FrequencyHigh ;
Int32 PhaseLimitTime;
Int32 PhaseLimit ;
Int32 ControlMode;
Int32 BlindtimeSeek ;
Int32 Blindtimeweld;
Int32 DDSSwitchtime;
Int32 Parameter7 ;
Int32 Parameter8;
Int32 Parameter9;
Int32 Parameter10;
Int32 Parameter11;
Int32 Parameter12;
Int32 Parameter13;
Int32 Parameter14;
Int32 Parameter15;
Int32 F_LimitTime;
Int32 AmpProportionalGain;
Int32 AmpIntegralGain;
Int32 PhaseIntegralGain;
Int32 PhaseProportionalGain;
Int32 FrequencyWindowSize;
Int32 RFofPhasecontrolloop;
Int32 PIFrequencyLow;
Int32 PIFrequencyHigh;
Int32 PIPhaseLimitTime;
Int32 PIPhaseLimit;
Int32 WELDRamptime;
Uint32 StartFrequency;
Int32 MemoryOffset;
Int32 DigitalTune;
Int32 FrequencyOffset;
} STRUCT_PACKED_END
TWELDRECIPEPARAMS;


PROTO TWELDRECIPEPARAMS sWeldRecipeParams
#ifdef _PRUESC_HW_
= {37 , 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0X33,0x34,0x35, 0x36}
#endif
;

/************************************************************
*				Object 0x2001 Power Up Data Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2001[] = {
		{DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED8   ,  0x08 ,  ACCESS_READWRITE }
	
	};


OBJCONST Uint8 OBJMEM aName0x2001[] = "Power Up DataStructure\000Frequency \000PoWer \000Weld SYSTEM_TYPE \000CheckSum \000Serial_No \000\377" ;
#endif

typedef struct
{
	Uint16 NumofEntries;
	Uint32 Frequency ;
	Uint32 Power ;
	Uint32 SYSTEMTYPE;
	Uint32 CheckSum ;
	Uint8 SerialNo ;
}STRUCT_PACKED_END
TPOWERUPPARAMS;

PROTO TPOWERUPPARAMS sPowerupparamsParams
#ifdef _PRUESC_HW_
= {5,0x28,0x320,0x13,0x14,0x15}
#endif
;

/************************************************************
*				Object 0x2004 Timer Interval Data Structure	
************************************************************/

#ifdef _OBJD_		
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2004[] = {
		{DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE  }
	};


OBJCONST Uint8 OBJMEM aName0x2004[] = "Timer Interval DataStructure\000Timer Interval PC \000\377" ;
#endif

typedef struct		
{
	Uint16 NumofEntries;
	Int32 TimerIntervalPC;
}STRUCT_PACKED_END
TTMRINTERVALPARAMS;

PROTO TTMRINTERVALPARAMS sTimerIntervalparamsParams		
#ifdef _PRUESC_HW_
= {1,0x3E8}
#endif
;

/************************************************************
*				Object 0x2002Alarm Data Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2002[] = {
                {DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
                {DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE }
};

OBJCONST Uint8 OBJMEM aName0x2002[] = "Alarm Data Structure\000Alarms Type \000\377" ;
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
*				Object 0x2008 Horn Scan Recipe Data Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2008[] = {
		{DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE }
	};


OBJCONST Uint8 OBJMEM aName0x2008[] = "Horn Scan Recipe DataStructure\000FrequencyStart \000FrequencyStop \000FrequencyStep \000TimeDelay \000MaxAmplitude \000MaxCurrent \000\377" ;
#endif

typedef struct
{
   Uint16 NumOfEntries;
   Uint32 FrequencyStart;
   Uint32 FrequencyStop;
   Uint16 FrequencyStep;	
   Uint16 TimeDelay;
   Uint16 MaxAmplitude;
   Uint16 MaxCurrent;
}STRUCT_PACKED_END
THORNSCANRECIPEPARAMS;  

PROTO THORNSCANRECIPEPARAMS sHornScanRecipeparamsParams
#ifdef _PRUESC_HW_
= {6,0x32,0x2710,0x1,0x1,0x15,0x16}
#endif
;

/************************************************************
*				Object 0x2007 Horn Scan Results Data Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2007[] = {
		{DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE }
	};


OBJCONST Uint8 OBJMEM aName0x2007[] = "Horn Scan Results DataStructure\000MainParallelFrequency \000SecondaryParallelFrequency \000TertiaryParallelFrequency \000FirstSeriesFrequency \000SecondarySeriesFrequency \000TertiarySeriesFrequency \000\377" ;
#endif

typedef struct
{
   Uint16 NumOfEntries;
   Uint32 MainParallelFrequenc;
   Uint16 SecondaryParallelFrequency;
   Uint32 TertiaryParallelFrequency;
   Uint32 FirstSeriesFrequency;
   Uint32 SecondarySeriesFrequency;
   Uint32 TertiarySeriesFrequency;
}STRUCT_PACKED_END
THORNSCANRESULTSPARAMS;  

PROTO THORNSCANRESULTSPARAMS sHornScanResultsparamsParams
#ifdef _PRUESC_HW_
= {6,0x11,0x12,0x13,0x14,0x15,0x16}
#endif
;


/************************************************************
*				Object 0x2005: Firmware Info Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2005[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE},
};


OBJCONST Uint8 OBJMEM aName0x2005[] = "Version Info Data\000FwMajor\000FwMinor\000FwBuild\000FwCrc\000FwInfo\000\377";
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
*				Object 0x2010Seek Recipe Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2010[] = {
		{DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE }
};


OBJCONST Uint8 OBJMEM aName0x2010[] = "Seek Recipe Structure\000SeekAmplitudeLoopC1\000SeekAmplitudeLoopC2\000SeekFreqLimitHigh\000SeekFreqLimitLow\000SeekPhaseloopI\000SeekPhaseloopCF\000SeekRampTime\000SeekFreqOffset\000\377" ;
#endif

typedef struct
{
	Uint16  NumOfEntries ;
	INT32	SeekAmplitudeLoopC1;
	INT32	SeekAmplitudeLoopC2;
	INT32 	SeekFreqLimitHigh;
	INT32 	SeekFreqLimitLow;
	INT32	SeekPhaseloopI;
	INT32	SeekPhaseloopCF;
	INT32	SeekRampTime;
	INT32  	SeekFreqOffset;
}STRUCT_PACKED_END
TSEEKRECIPEPARAMS;

PROTO TSEEKRECIPEPARAMS sSeekRecipeParams
#ifdef _PRUESC_HW_
= { 8,0x12345678,0x12345678,0x12345678,0x12345678,0x12345678,0x12345678,0x12345678,0x12345678 }
#endif
;

/************************************************************
*				Object 0x2011Seek Result Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2011[] = {
		{DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER16    , 0x10 , ACCESS_READWRITE }
};

OBJCONST Uint8 OBJMEM aName0x2011[] = "Seek Result Structure\000Seek Start Frequency\000Seek Peak Power\000Seek End Frequency\000Frequency Change\000\377" ;
#endif

typedef struct
{
	Uint16 NumOfEntries;
	Uint32 SeekStartFrequency;
	Uint16 SeekPeakPower;
	Uint32 SeekEndFrequency;
	Int16 FrequencyChange;
}STRUCT_PACKED_END
TSEEKRESULTPARAMS;

PROTO TSEEKRESULTPARAMS sSeekResultParams
#ifdef _PRUESC_HW_
= { 4 , 0x1A , 0x1B , 0x1C , 0x1D }
#endif
;

/************************************************************
*				Object 0x1603: PC RxPDO Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1603[] = {
        {DEFTYPE_UNSIGNED8  , 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED32 , 0x20, ACCESS_READ},
        {DEFTYPE_UNSIGNED32 , 0x20 ,  ACCESS_READ},
		{DEFTYPE_UNSIGNED32 , 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32 , 0x20, ACCESS_READ},
};

OBJCONST Uint8 OBJMEM aName0x1603[] = "RxPDO1-Map\000SubIndex 000\000SubIndex 001\000SubIndex 002\000SubIndex 003\000\000SubIndex 004\000\377";
#endif

typedef struct STRUCT_PACKED_START {
        Uint16       u16SubIndex0;
        Uint32    SubIndex001 ;
        Uint32    SubIndex002;
        Uint32     SubIndex003;
        Uint32    SubIndex004;
}STRUCT_PACKED_END
TOOBJ1603;

PROTO TOOBJ1603 sAIRxPDOMap                //todo
#ifdef _PRUESC_HW_
= {RXPDO_MAP_NO_ENTRIES , 0x20030110, 0x20030210, 0x20030310,0x20030420}        //todo
#endif
;

/************************************************************
*				Object 0x1A04: PC TxPDO Structure
************************************************************/


#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1A04[] = {
        {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ},
        {DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READ},
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READ},
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READ},
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READ},
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READ},
		{DEFTYPE_UNSIGNED32	  , 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32	  , 0x20, ACCESS_READ}
};
OBJCONST Uint8 OBJMEM aName0x1A04[] = "TxPDO1-Map\000SubIndex 0001\000SubIndex 2\000SubIndex 3\000SubIndex 4\000SubIndex 5\000SubIndex 6\000SubIndex 7\000\377";
#endif

typedef struct STRUCT_PACKED_START {
            Uint16       u16SubIndex0;
            Uint32    SubIndex001 ;
            Uint32    SubIndex002;
            Uint32      SubIndex003;
			Uint32     SubIndex004;
			Uint32      SubIndex005;
			Uint32      SubIndex006;
			Uint32      SubIndex007;
}STRUCT_PACKED_END
TOOBJ1A04;

PROTO TOOBJ1A04 sAITxPDOMap
#ifdef _PRUESC_HW_
= {TXPDO_MAP_NO_ENTRIES, 0x20030410, 0x20030510, 0x20030610,0x20030720,0x20030810,0x20030908,0x20030A08}
#endif
;

/************************************************************
*				Object 0x2003 Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x2003[] = {
        {DEFTYPE_UNSIGNED8 , 0x08, ACCESS_READ},		
		{DEFTYPE_UNSIGNED16 ,0x10, ACCESS_READ | OBJACCESS_RXPDOMAPPING},
        {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ | OBJACCESS_RXPDOMAPPING},
		{DEFTYPE_UNSIGNED16 ,0x10, ACCESS_READ | OBJACCESS_RXPDOMAPPING},
		{DEFTYPE_UNSIGNED32 ,0x20, ACCESS_READ | OBJACCESS_RXPDOMAPPING},
		{DEFTYPE_UNSIGNED16,  0x10, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_INTEGER16, 0x10, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_UNSIGNED8 , 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_UNSIGNED8 , 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING}		
};

OBJCONST Uint8 OBJMEM aName0x2003[] = "Real Time Data\000Master State\000Amplitude 1\000Master Events\000Frequency 1\000Amplitude 2\000Phase\000Power\000Frequency 2\000Current\000Pc Status\000Pc State\000\377";
#endif

typedef struct STRUCT_PACKED_START {
        Uint16     u16SubIndex0;        
        Uint16     MasterState;
        Uint16     Amplitude_1;
        Uint16     MasterEvents;
        UINT32     Frequency_1;
        Uint16     Amplitude_2;
        Int16      Phase;
        Uint16     Power;
        Uint32     Frequency;
        Uint16     Current;
		Uint8      PCStatus;
		Uint8      PCstate;
		
}STRUCT_PACKED_END
TOOBJ0x2003;

PROTO TOOBJ0x2003 sRealTimeData
#ifdef _PRUESC_HW_
= { (RXPDO_MAP_NO_ENTRIES + TXPDO_MAP_NO_ENTRIES) , 0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0x91,0x92}
#endif
;
/*
#ifdef _PRUESC_HW_
= { (RXPDO_MAP_NO_ENTRIES + TXPDO_MAP_NO_ENTRIES) , 0x12,0x23,0x34,0xFFFF,0xFFFF,0xFFFF,0xFFFFFFFF,0xFFFF,0xFF,0xAB}
#endif
;*/




/************************************************************
*				Object 0x2006 Test Recipe Structure
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2006[] = {
{DEFTYPE_UNSIGNED8 , 0x08 ,  ACCESS_READ },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
        {DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32 ,0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_INTEGER32 , 0x20 ,  ACCESS_READWRITE },
	};

OBJCONST Uint8 OBJMEM aName0x2006[] = "Test Recipe Structure\000Amplitude Loop C1\000Amplitude Loop C2\000Phase Loop\000Phase Loop CF\000Frequency Low\000Frequency High\000Phase Limit Time\000Phase Limit\000Control Mode\000Blindtime Seek\000Blindtime weld\000DDS Switchtime\000Parameter7\000Parameter8\000Parameter9\000Parameter10\000Parameter11\000Parameter12\000Parameter13\000Parameter14\000Parameter15\000F-LimitTime\000Amp Proportional Gain\000Amp Integral Gain\000Phase Integral Gain\000Phase ProportionalGain\000Frequency Window Size\000RF of Phase control loop\000PI Frequency Low\000PI Frequency High\000PI PhaseLimit Time\000PI Phase Limit\000Weld Ramp time\000Start Frequency\000Memory Offset\000Digital Tune\000Frequency Offset\000\377" ;
#endif

typedef struct
{
Uint16 Entries ;
Int32 AmplitudeLoopC1;
Int32 AmplitudeLoopC2;
Int32 PhaseLoop ;
Int32 PhaseLoopCF;
Int32 FrequencyLow;
Int32 FrequencyHigh ;
Int32 PhaseLimitTime;
Int32 PhaseLimit ;
Int32 ControlMode;
Int32 BlindtimeSeek ;
Int32 Blindtimeweld;
Int32 DDSSwitchtime;
Int32 Parameter7 ;
Int32 Parameter8;
Int32 Parameter9;
Int32 Parameter10;
Int32 Parameter11;
Int32 Parameter12;
Int32 Parameter13;
Int32 Parameter14;
Int32 Parameter15;
Int32 F_LimitTime;
Int32 AmpProportionalGain;
Int32 AmpIntegralGain;
Int32 PhaseIntegralGain;
Int32 PhaseProportionalGain;
Int32 FrequencyWindowSize;
Int32 RFofPhasecontrolloop;
Int32 PIFrequencyLow;
Int32 PIFrequencyHigh;
Int32 PIPhaseLimitTime;
Int32 PIPhaseLimit;
Int32 WELDRamptime;
Uint32 StartFrequency;
Int32 MemoryOffset;
Int32 DigitalTune;
Int32 FrequencyOffset;
} STRUCT_PACKED_END
TTESTRECIPEPARAMS;


PROTO TTESTRECIPEPARAMS sTestRecipeParams
#ifdef _PRUESC_HW_
= {37 , 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2D,0x2D,0x2D,0x2E,0x2F}
#endif
;
/************************************************************
*				Object 0x2020: Firmware Upgrade Information
************************************************************/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x2020[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READWRITE},
};


OBJCONST Uint8 OBJMEM aName0x2020[] = "Firmware Upgrade Information\000CRC_Checksum\000upgrade_status\000progress\000\377";
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
	{NULL,NULL,  0x1603, {DEFTYPE_PDOMAPPING, 4 | (OBJCODE_REC << 8)}, asEntryDesc0x1603, aName0x1603, &sAIRxPDOMap, NULL, NULL, 0x0000 },
		   
	/* Object 0x1A04 */
	{NULL,NULL,  0x1A04, {DEFTYPE_PDOMAPPING, 7 | (OBJCODE_REC << 8)}, asEntryDesc0x1A04, aName0x1A04, &sAITxPDOMap, NULL, NULL, 0x0000 },

	/* Object 0x2000 */
	{NULL,NULL,  0x2000, {DEFTYPE_RECORD, 37 | (OBJCODE_REC << 8)}, asEntryDesc0x2000, aName0x2000, &sWeldRecipeParams, NULL, NULL, 0x0000 },
	/* Object 0x2001 */
	{NULL,NULL,  0x2001, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x2001, aName0x2001, &sPowerupparamsParams, NULL, NULL, 0x0000 },
	
	 /* Object 0x2002 */
	{NULL,NULL,  0x2002, {DEFTYPE_RECORD, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x2002, aName0x2002, &sAlarmDataParams, NULL, NULL, 0x0000 },
	
	/* Object 0x2008 */
	{NULL,NULL,  0x2008, {DEFTYPE_RECORD, 6 | (OBJCODE_REC << 8)}, asEntryDesc0x2008, aName0x2008, &sHornScanRecipeparamsParams, NULL, NULL, 0x0000 },
					
	/* Object 0x2006 */
	{NULL,NULL,  0x2006, {DEFTYPE_RECORD, 37 | (OBJCODE_REC << 8)}, asEntryDesc0x2006, aName0x2006, &sTestRecipeParams, NULL, NULL, 0x0000 },			
	
	/* Object 0x2007 */
	{NULL,NULL,  0x2007, {DEFTYPE_RECORD, 6 | (OBJCODE_REC << 8)}, asEntryDesc0x2007, aName0x2007, &sHornScanResultsparamsParams, NULL, NULL, 0x0000 },

	/* Object 0x2010 */
	{NULL,NULL,  0x2010, {DEFTYPE_RECORD, 8 | (OBJCODE_REC << 8)}, asEntryDesc0x2010, aName0x2010, &sSeekRecipeParams, NULL, NULL, 0x0000 },

	/* Object 0x2011 */
	{NULL,NULL,  0x2011, {DEFTYPE_RECORD, 4 | (OBJCODE_REC << 8)}, asEntryDesc0x2011, aName0x2011, &sSeekResultParams, NULL, NULL, 0x0000 },
	
	/* Object 0x2003 */
	{NULL,NULL,  0x2003, {DEFTYPE_RECORD , 11 | (OBJCODE_REC << 8)}, asEntryDesc0x2003, aName0x2003, &sRealTimeData, NULL, NULL, 0x0000 },
	   
	/* Object 0x1C12 */
   {NULL,NULL,   0x1C12, {DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C12, aName0x1C12, &sRxPDOassign, NULL, NULL, 0x0000 },
     /* Object 0x1C13 */
    {NULL,NULL,   0x1C13, {DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C13, aName0x1C13, &sTxPDOassign, NULL, NULL, 0x0000 },
	/* Object 0x2005 */
	{NULL,NULL,   0x2005, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x2005, aName0x2005, &sFirmwareInfoParams, NULL, NULL, 0x0000 },
	/* Object 0x2004 */
	{NULL,NULL,  0x2004, {DEFTYPE_RECORD, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x2004, aName0x2004, &sTimerIntervalparamsParams, NULL, NULL, 0x0000 }, 
	
	/* Object 0x2020 */ 
	{NULL,NULL,   0x2020, {DEFTYPE_RECORD, 3 | (OBJCODE_REC << 8)}, asEntryDesc0x2020, aName0x2020, &sFirmwareUpgradeInfoparams, NULL, NULL, 0x0000 },
	
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


typedef struct PcRealtimeData
{	
	UINT16 u16SubIndex0;  
	UINT16 MasterState;
	UINT16 Amplitude_1;
	UINT16 MasterEvents;
	UINT32 Frequency_1;
	UINT16 Amplitude_2;	
	INT16 Phase;
	UINT16 Power;
	UINT32 Frequency;
	UINT16 Current;
	UINT8  PCStatus;
	UINT8  PCstate;
}PcRealtimeData;

#endif //#if PRUESC_HW

#undef PROTO

