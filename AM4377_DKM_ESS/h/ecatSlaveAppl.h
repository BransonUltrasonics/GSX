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

#define TXPDO1_BANK_DIAG_NUM_ENTRIES 		3
#define TXPDO2_STATUS_STRUCT_NUM_ENTRIES		5

#define RXPDO_POWER_DATA_NUM_ENTRIES		5

#define TXPDO1_INDEX 0x1A04
#define TXPDO2_INDEX 0x1A05

#define RXPDO1_INDEX 0x1603
/*-----------------------------------------------------------------------------------------
------
------    ECAT SLAVE application specific objects
------
-----------------------------------------------------------------------------------------*/
#if 0
/******************************************************************************
*					Object 0x1601: digital output RxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1601[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: SubIndex 001 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: SubIndex 002 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 003: SubIndex 003 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 004: SubIndex 004 */

   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 005: SubIndex 005 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 006: SubIndex 006 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 007: SubIndex 007 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}}; /* SubIndex 008: SubIndex 008 */

OBJCONST Uint8 OBJMEM aName0x1601[] = "RxPDO-Map\000SubIndex 1\000SubIndex 2\000SubIndex 3\000SubIndex 4\000SubIndex 5\000SubIndex 6\000SubIndex 7\000SubIndex 8\000\377";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint32   aEntries[8];
} STRUCT_PACKED_END
TOBJ1601;

PROTO TOBJ1601 RxPDOMap
#ifdef _PRUESC_HW_
 = {8, {0x70100101, 0x70100201, 0x70100301, 0x70100401, 0x70100501, 0x70100601, 0x70100701, 0x70100801}}
#endif
;
/******************************************************************************
*					Object 0x1602:  Motor output RxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1602[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: SubIndex 001 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: SubIndex 002 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}}; /* SubIndex 003: SubIndex 003 */
   
OBJCONST Uint8 OBJMEM aName0x1602[] = "RxPDO1-Map\000SubIndex 1\000SubIndex 2\000SubIndex 3\000\377";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint32   aEntries[3];
} STRUCT_PACKED_END
TOBJ1602;

PROTO TOBJ1602 RxPDO1Map
#ifdef _PRUESC_HW_
 = {3, {0x70200108, 0x70200208, 0x70200310}}
#endif
;

/******************************************************************************
*					Object 0x1A00: digital input TxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1A00[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}};
   
OBJCONST Uint8 OBJMEM aName0x1A00[] = "TxPDO-Map\000SubIndex 1\000SubIndex 2\000SubIndex 3\000SubIndex 4\000SubIndex 5\000SubIndex 6\000SubIndex 7\000SubIndex 8\000\377";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint32   aEntries[8];
} STRUCT_PACKED_END
TOBJ1A00;

PROTO TOBJ1A00 TxPDOMap
#ifdef _PRUESC_HW_
 = {8, {0x60000101, 0x60000201, 0x60000301, 0x60000401,0x60000501, 0x60000601, 0x60000701, 0x60000801}}
#endif
;

/******************************************************************************
*					Object 0x1A03: Motor analog input TxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1A03[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: SubIndex 001 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}}; /* SubIndex 002: SubIndex 002 */

OBJCONST Uint8 OBJMEM aName0x1A03[] = "TxPDO1-Map\000SubIndex 1\000SubIndex 2\000\377";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint32   aEntries[2];
} STRUCT_PACKED_END
TOBJ1A03;
PROTO TOBJ1A03 sAITxPDO1Map

#ifdef _PRUESC_HW_
= {2, {0x60300120, 0x60300210}}
#endif
;
#endif //#if 0

/******************************************************************************
*					Object 0x1A04: Bank Diagnostic Info TxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1A04[] = {
		{DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}
};

OBJCONST Uint8 OBJMEM aName0x1A04[] = "TxPDO1-Map\000SubIndex 1\000SubIndex 2\000SubIndex 3\000\377";
#endif

typedef struct STRUCT_PACKED_START {
	   Uint16   u16SubIndex0;
	   Uint32   aEntries[TXPDO1_BANK_DIAG_NUM_ENTRIES];
}STRUCT_PACKED_END
TOOBJ1A04;

PROTO TOOBJ1A04 sAITxPDO1Map
#ifdef _PRUESC_HW_
= {3 , {0x200E0508, 0x200E0208, 0x200E0308}} // Macro TXPDO1_BANK_DIAG_NUM_ENTRIES
#endif
;

/******************************************************************************
*					Object 0x1A05: Status Structure TxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1A05[] = {
		{DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}
};

OBJCONST Uint8 OBJMEM aName0x1A05[] = "TxPDO2-Map\000SubIndex 1\000SubIndex 2\000SubIndex 3\000SubIndex 4\000SubIndex 5\000\377";
#endif

typedef struct STRUCT_PACKED_START {
	   Uint16   u16SubIndex0;
	   Uint32   aEntries[TXPDO2_STATUS_STRUCT_NUM_ENTRIES];
}STRUCT_PACKED_END
TOOBJ1A05;

PROTO TOOBJ1A05 sAITxPDO2Map
#ifdef _PRUESC_HW_
= {5, {0x22040208, 0x23040208, 0x24040208, 0x25040208, 0x26040208}} // Macro TXPDO2_STATUS_STRUCT_NUM_ENTRIES
#endif
;


/******************************************************************************
*					Object 0x1603: Laser Power Data Structure RxPDO
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1603[] = {
		{DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
		{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}
};

OBJCONST Uint8 OBJMEM aName0x1603[] = "RxPDO1-Map\000SubIndex 1\000SubIndex 2\000SubIndex 3\000SubIndex 4\000SubIndex 5\000\377";
#endif

typedef struct STRUCT_PACKED_START {
	   Uint16   u16SubIndex0;
	   Uint32   aEntries[RXPDO_POWER_DATA_NUM_ENTRIES];
}STRUCT_PACKED_END
TOOBJ1603;

PROTO TOOBJ1603 sAIRxPDO1Map
#ifdef _PRUESC_HW_
= {5, {0x22080308, 0x23080308, 0x24080308, 0x25080308, 0x26080308}} // Macro RXPDO_POWER_DATA_NUM_ENTRIES
#endif
;


/******************************************************************************
*					Object 0x1C13: TxPDO assignment
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1C13[] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}};
OBJCONST Uint8 OBJMEM aName0x1C13[] = "TxPDO assign";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint16   aEntries[2];
} STRUCT_PACKED_END
TOBJ1C13;

PROTO TOBJ1C13 sTxPDOassign
#ifdef _PRUESC_HW_
= {0x02, {0x1A04, 0x1A05}}
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

#if 0
/******************************************************************************
*					Object 0x6000: digital input object
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x6000[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 001: Switch 1 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 002: Switch 2 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 003: Switch 3 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 004: Switch 4 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 005: Switch 5 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 006: Switch 6 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 007: Switch 7 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_TXPDOMAPPING}}; /* SubIndex 008: Switch 8 */
   
   
OBJCONST Uint8 OBJMEM aName0x6000[] = "DI Inputs\000Switch 1\000Switch 2\000Switch 3\000Switch 4\000Switch 5\000Switch 6\000Switch 7\000Switch 8\000\377";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint8 	switchs;
} STRUCT_PACKED_END
TOBJ6000;

PROTO TOBJ6000 sDIInputs
#ifdef _PRUESC_HW_
= {1, 0x00}
#endif
;

/******************************************************************************
*					Object 0x6030: analog input object
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x6030[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_INTEGER32, 0x20, ACCESS_READ | OBJACCESS_TXPDOMAPPING}, /* SubIndex 001: Info 1 */
   {DEFTYPE_INTEGER16, 0x10, ACCESS_READ | OBJACCESS_TXPDOMAPPING}};//, /* SubIndex 002: Info 2 */

   
OBJCONST Uint8 OBJMEM aName0x6030[] = "Motor AI Inputs\000Info 1\000Info 2\000\377";
#endif //#ifdef _OBJD_

typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Int32   info1;
   Int16   info2;
} STRUCT_PACKED_END
TOBJ6030;

PROTO TOBJ6030 sAI1Inputs
#ifdef _PRUESC_HW_
= {2, 0x0000, 0x00}
#endif
;

/******************************************************************************
*					Object 0x7010: digital output object
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x7010 [] = {
    {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 001: LED 1 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 002: LED 2 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 003: LED 3 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 004: LED 4 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 005: LED 5 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 006: LED 6 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 007: LED 7 */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ | OBJACCESS_RXPDOMAPPING}}; /* SubIndex 008: LED 8 */
OBJCONST Uint8 OBJMEM aName0x7010[] = "DO Outputs\000LED 1\000LED 2\000LED 3\000LED 4\000LED 5\000LED 6\000LED 7\000LED 8\000\377";
#endif //#ifdef _OBJD_


typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint8    LEDs;
} STRUCT_PACKED_END
TOBJ7010;

PROTO TOBJ7010 sDOOutputs 
#ifdef _PRUESC_HW_
= {1, 0x0}
#endif
;
/******************************************************************************
*					Object 0x7020: Motor output object
******************************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x7020 [] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 001: motor count */
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_RXPDOMAPPING}, /* SubIndex 002: motor command */
   {DEFTYPE_INTEGER16, 0x10, ACCESS_READ | OBJACCESS_RXPDOMAPPING}}; /* SubIndex 003: motor data */
OBJCONST Uint8 OBJMEM aName0x7020[] = "Motor Outputs\000Count\000Command\000Data\000\377";
#endif //#ifdef _OBJD_


typedef struct STRUCT_PACKED_START {
   Uint16   u16SubIndex0;
   Uint8	Count;
   Uint8	Cmd;
   Int16	MotorData;
} STRUCT_PACKED_END
TOBJ7020;

PROTO TOBJ7020 sDO1Outputs 
#ifdef _PRUESC_HW_
= {3, 0x00, 0x00, 0x0000}
#endif
;
#endif //#if 0
/************************************************************
*			Object 0x200E: Bank Diagnostic Information
************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x200E[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ },
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING },
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ },
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ }
};

OBJCONST Uint8 OBJMEM aName0x200E[] = "Bank Diagnostic Information\000Reserved\000Humidity\000Bank Temperature\000Reserved\000Operational State Bit Field\000Reserved\000Reserved\000\377";
#endif

typedef struct STRUCT_PACKED_START {
	Uint16 u16SubIndex0 ;
	Uint8 Reserved ;
	Uint8 Humidity ;
	Uint8 BankTemperature ;
	Uint8 Reserved1 ;
	Uint8 OperationalStateBitField ;
	Uint8 Reserved2 ;
	Uint8 Reserved3 ;
} STRUCT_PACKED_END
TOOBJ200E;

PROTO TOOBJ200E sTxPDOBankDiagInfo
#ifdef _PRUESC_HW_
= {7 , 0x0 , 0xFF, 0xFF, 0x0, 0xFF, 0x0, 0x0}
#endif
;

/************************************************************
*				Object 0x210A: Weld Recipe Structure
************************************************************/
/* Sample SDO data to observe on twinCAT*/

#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x210A[] = {
		{DEFTYPE_UNSIGNED8    , 0x08 ,  ACCESS_READ },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED32   , 0x20 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED8   , 0x8 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED8   , 0x8 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE },
		{DEFTYPE_UNSIGNED16   , 0x10 ,  ACCESS_READWRITE }
};


OBJCONST Uint8 OBJMEM aName0x210A[] = "Weld Recipe Structure\000Weld Delay\000Weld Time\000Intensity\000Control Recipe\000Intensity Factor\000Part Reflectivity Factor\000Tool Reflectivity Factor\000\377" ;
#endif

typedef struct {
	Uint16			u16subIndex0 ;
	Uint16			WeldDelay ;
	Uint32			WeldTime  ;
	Uint8			Intensity ;
	Uint8			ControlRecipe ;
	Uint16			IntensityFactor ;
	Uint16			PartReflectivityFactor ;
	Uint16			ToolReflectivityFactor ;
} STRUCT_PACKED_END
TWELDRECIPEPARAMS;

PROTO TWELDRECIPEPARAMS sWeldRecipeParams
#ifdef _PRUESC_HW_
= { 7 , 0x1111 , 0x22222222 , 0x33 , 0x44 , 0x5555 , 0x6666 , 0x7777 }
#endif
;


/************************************************************
*			Object 0x2204: Channel Status Structure 1
************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x2204[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING}
};
#endif

typedef struct STRUCT_PACKED_START {
	Uint16 u16subIndex0 ;
	Uint8 Reserved ;
	Uint8 LaserChannelState ;
} STRUCT_PACKED_END
TOOBJ2204;

#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2204[] = "Channel Status Structure 1\000Reserved\000Laser Channel State 1\000\377" ;
#endif

PROTO TOOBJ2204 sTxPDOLaserChState1
#ifdef _PRUESC_HW_
= {2 , 0x0, 0xFF}
#endif
;

/************************************************************
*			Object 0x2304: Channel Status Structure 2
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2304[] = "Channel Status Structure 2\000Reserved\000Laser Channel State 2\000\377" ;
#endif

PROTO TOOBJ2204 sTxPDOLaserChState2
#ifdef _PRUESC_HW_
= {2 , 0x0, 0xFF}
#endif
;

/************************************************************
*			Object 0x2304: Channel Status Structure 3
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2404[] = "Channel Status Structure 3\000Reserved\000Laser Channel State 3\000\377" ;
#endif

PROTO TOOBJ2204 sTxPDOLaserChState3
#ifdef _PRUESC_HW_
= {2 , 0x0, 0xFF}
#endif
;

/************************************************************
*			Object 0x2304: Channel Status Structure 4
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2504[] = "Channel Status Structure 4\000Reserved\000Laser Channel State 4\000\377" ;
#endif

PROTO TOOBJ2204 sTxPDOLaserChState4
#ifdef _PRUESC_HW_
= {2 , 0x0, 0xFF}
#endif
;

/************************************************************
*			Object 0x2304: Channel Status Structure 5
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2604[] = "Channel Status Structure 5\000Reserved\000Laser Channel State 5\000\377" ;
#endif

PROTO TOOBJ2204 sTxPDOLaserChState5
#ifdef _PRUESC_HW_
= {2 , 0x0, 0xFF}
#endif
;

/************************************************************
*			Object 0x2208: Laser Power Data Structure 1
************************************************************/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x2208[] = {
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_RXPDOMAPPING},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
		{DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ}
};
#endif

typedef struct STRUCT_PACKED_START {
	Uint16 u16subIndex0 ;
	Uint8  Reserved ;
	Uint8  Reserved1 ;
	Uint8  LaserChannelTemp ;
	Uint8  Reserved2 ;
	Uint8  Reserved3 ;
} STRUCT_PACKED_END
TOOBJ2208;

#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2208[] = "Laser Power Data Structure 1\000Reserved\000Reserved\000LaserChannelTemp 1\000Reserved\000Reserved\000\377" ;
#endif

PROTO TOOBJ2208 sRxPDOPowerDataStructure1
#ifdef _PRUESC_HW_
= {5 , 0x0, 0x00, 0xFF, 0x00, 0x00}
#endif
;

/************************************************************
*			Object 0x2308: Laser Power Data Structure 2
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2308[] = "Laser Power Data Structure 2\000Reserved\000Reserved\000LaserChannelTemp 2\000Reserved\000Reserved\000\377" ;
#endif

PROTO TOOBJ2208 sRxPDOPowerDataStructure2
#ifdef _PRUESC_HW_
= {5 , 0x0, 0x0, 0xFF, 0x0, 0x0}
#endif
;

/************************************************************
*			Object 0x2408: Laser Power Data Structure 3
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2408[] = "Laser Power Data Structure 3\000Reserved\000Reserved\000LaserChannelTemp 3\000Reserved\000Reserved\000\377" ;
#endif

PROTO TOOBJ2208 sRxPDOPowerDataStructure3
#ifdef _PRUESC_HW_
= {5 , 0x0, 0x0, 0xFF, 0x0, 0x0}
#endif
;

/************************************************************
*			Object 0x2508: Laser Power Data Structure 4
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2508[] = "Laser Power Data Structure 4\000Reserved\000Reserved\000LaserChannelTemp 4\000Reserved\000Reserved\000\377" ;
#endif

PROTO TOOBJ2208 sRxPDOPowerDataStructure4
#ifdef _PRUESC_HW_
= {5 , 0x0, 0x0, 0xFF, 0x0, 0x0}
#endif
;

/************************************************************
*			Object 0x2608: Laser Power Data Structure 5
************************************************************/
#ifdef _OBJD_
OBJCONST Uint8 OBJMEM aName0x2608[] = "Laser Power Data Structure 5\000Reserved\000Reserved\000LaserChannelTemp 5\000Reserved\000Reserved\000\377" ;
#endif

PROTO TOOBJ2208 sRxPDOPowerDataStructure5
#ifdef _PRUESC_HW_
= {5 , 0x0, 0x0, 0xFF, 0x0, 0x0}
#endif
;

/***********************************************************/

#ifdef _OBJD_
/*if _PIC18 is enabled the object dictionary is fixed defined in coeappl.c*/
TOBJECT	OBJMEM ApplicationObjDic[] = {
   /* Object 0x1603 */
   {NULL,NULL,  0x1603, {DEFTYPE_PDOMAPPING, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x1603, aName0x1603, &sAIRxPDO1Map, NULL, NULL, 0x0000 },
   /* Object 0x1A04 */
   {NULL,NULL,  0x1A04, {DEFTYPE_PDOMAPPING, 3 | (OBJCODE_REC << 8)}, asEntryDesc0x1A04, aName0x1A04, &sAITxPDO1Map, NULL, NULL, 0x0000 },
   /* Object 0x1A00 */
  // {NULL,NULL,   0x1A00, {DEFTYPE_PDOMAPPING, 8 | (OBJCODE_REC << 8)}, asEntryDesc0x1A00, aName0x1A00, &TxPDOMap, NULL, NULL, 0x0000 },
   /* Object 0x1A05 */
   {NULL,NULL,   0x1A05, {DEFTYPE_PDOMAPPING, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x1A05, aName0x1A05, &sAITxPDO2Map, NULL, NULL, 0x0000 },
    /* Object 0x1C12 */
   {NULL,NULL,   0x1C12, {DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C12, aName0x1C12, &sRxPDOassign, NULL, NULL, 0x0000 },
     /* Object 0x1C13 */
    {NULL,NULL,   0x1C13, {DEFTYPE_UNSIGNED16, 2 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C13, aName0x1C13, &sTxPDOassign, NULL, NULL, 0x0000 },
	/* Object 0x200E */
		{NULL,NULL,   0x200E, {DEFTYPE_RECORD, 7 | (OBJCODE_REC << 8)}, asEntryDesc0x200E, aName0x200E, &sTxPDOBankDiagInfo, NULL, NULL, 0x0000 },
	/* Object 0x210A */
	{NULL,NULL,   0x210A, {DEFTYPE_RECORD, 7 | (OBJCODE_REC << 8)}, asEntryDesc0x210A, aName0x210A, &sWeldRecipeParams, NULL, NULL, 0x0000 },
	/* Object 0x2204 */
		{NULL,NULL,   0x2204, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x2204, aName0x2204, &sTxPDOLaserChState1, NULL, NULL, 0x0000 },
	/* Object 0x2304 */
		{NULL,NULL,   0x2304, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x2204, aName0x2304, &sTxPDOLaserChState2, NULL, NULL, 0x0000 },
	/* Object 0x2404 */
		{NULL,NULL,   0x2404, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x2204, aName0x2404, &sTxPDOLaserChState3, NULL, NULL, 0x0000 },
	/* Object 0x2504 */
		{NULL,NULL,   0x2504, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x2204, aName0x2504, &sTxPDOLaserChState4, NULL, NULL, 0x0000 },
	/* Object 0x2604 */
		{NULL,NULL,   0x2604, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x2204, aName0x2604, &sTxPDOLaserChState5, NULL, NULL, 0x0000 },
	/* Object 0x2208 */
		{NULL,NULL,   0x2208, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x2208, aName0x2208, &sRxPDOPowerDataStructure1, NULL, NULL, 0x0000 },
	/* Object 0x2308 */
		{NULL,NULL,   0x2308, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x2208, aName0x2308, &sRxPDOPowerDataStructure2, NULL, NULL, 0x0000 },
	/* Object 0x2408 */
		{NULL,NULL,   0x2408, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x2208, aName0x2408, &sRxPDOPowerDataStructure3, NULL, NULL, 0x0000 },
	/* Object 0x2508 */
		{NULL,NULL,   0x2508, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x2208, aName0x2508, &sRxPDOPowerDataStructure4, NULL, NULL, 0x0000 },
	/* Object 0x2608 */
		{NULL,NULL,   0x2608, {DEFTYPE_RECORD, 5 | (OBJCODE_REC << 8)}, asEntryDesc0x2208, aName0x2608, &sRxPDOPowerDataStructure5, NULL, NULL, 0x0000 },
/* Object 0x6000 */
//   {NULL,NULL,   0x6000, {DEFTYPE_RECORD, 8 | (OBJCODE_REC << 8)}, asEntryDesc0x6000, aName0x6000, &sDIInputs, NULL, NULL, 0x0000 },
   /* Object 0x6020 */
 //  {NULL,NULL,   0x6030, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x6030, aName0x6030, &sAI1Inputs, NULL, NULL, 0x0000 },
   /* Object 0x7010 */
 //  {NULL,NULL,   0x7010, {DEFTYPE_RECORD, 8 | (OBJCODE_REC << 8)}, asEntryDesc0x7010, aName0x7010, &sDOOutputs, NULL, NULL, 0x0000 },
   /* Object 0x7020 */
 //  {NULL,NULL,   0x7020, {DEFTYPE_RECORD, 3 | (OBJCODE_REC << 8)}, asEntryDesc0x7020, aName0x7020, &sDO1Outputs, NULL, NULL, 0x0000 },
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

