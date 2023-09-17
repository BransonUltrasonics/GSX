/*
 * Common.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <sys\types.h>
#include"Commons.h"

typedef
enum { EVNORMAL, EVTRANSITION, EVCYCLEABORT } EVENTS_1;

typedef
enum {CMD_WELD,CMD_WELDHD,CMD_SEEK,CMD_SCAN,CMD_CYCLEABORT}PC_CMD; //Only for debug


typedef enum PDOList {
	AC_CMD_PDO,
	PC_CMD_PDO,
	AC_STATUS_PDO,
	PC_STATUS_PDO
} PdoType;


//Cmd from SC Master to AC slave
typedef struct Struct_AC_CMD_PDO {
	UINT8    SC_State;
	UINT8    AC_ControlEvent;
	UINT8    AC_Outputs;
}S_AC_CMD_PDO;


//Status from AC slave to SC Master
typedef struct Struct_AC_STATUS_PDO {
	UINT32 	 ActualDistance;
	UINT16   ActualForce;
	UINT8  	 ACStatusEvent;
	ACSTATES ACState;
	UINT8    ACInputs;
}S_AC_STATUS_PDO;

//Cmd from SC Master to PC slave
typedef struct Struct_PC_CMD_PDO {
	UINT16 	 Amplitude;
	SCSTATES SCState;
	UINT8 	 PC_ControlEvent;
}S_PC_CMD_PDO;

typedef struct RxPDO_PC
{
	UINT8	MasterState;
	UINT16	Amplitudechange;
	UINT16	MasterEvents;
	UINT32  Frequency;
}RxPDO_PC;


typedef struct TxPDO_PC
{
	
	UINT16  Amplitude;
	INT16  Phase;
	UINT16  Power;
	UINT32  Frequency;
	UINT16  Current;
	UINT8   PC_StatusEvent;
	UINT8	PCState;
}TxPDO_PC;



//Status from PC slave to SC Master
typedef struct Struct_PC_STATUS_PDO {
	UINT32   Frequency;
	UINT16   Amplitude;
	INT16   Phase;
	UINT16   Power;
	UINT16   Current;
	PCSTATES PCState;
	UINT8    PC_StatusEvent;
} S_PC_STATUS_PDO;


//Cmd and Status PDO structures
typedef struct PdoStructure {
	S_PC_CMD_PDO	PcCmdPdo;
	S_AC_CMD_PDO	AcCmdPdo;
	S_PC_STATUS_PDO	PcStatusPdo;
	S_AC_STATUS_PDO	AcStatusPdo;
}PdoObject;

typedef struct HornScanGraphPointData
{
	UINT32 Frequency;
	UINT16 PWMValue;
	UINT16 Amplitude;
	UINT16 Current;
	UINT16 Power;
	INT16 Phase;
	UINT16 Status;
}ScanData;

typedef struct 
{	
	UINT16  u16SubIndex0;
	UINT16 	CRC_Checksum;
	UINT8 	upgrade_status;
	UINT8   progress;
} 
PcFirmwareUpgradeInfo;

UINT32 PSFrequency;
UINT32 PSPower;

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define CYCLE_TIME 250
#define MIN_PWMAMP_VALUE  0
#define MAX_PWMAMP_VALUE  0x10000000
#define ADC15BITVALUE     32767
#define SCALING_FACTOR     100
#define PCTIME  1000
#define	ECAT_CYCLE_TIME_IN_US 250
#define FREQ_MIN          -5000

#define F20KHZF_FACTOR 100
#define F20KHZMIDBAND 199500
#define F20KHZ        20
#define P20KHZ4000W        4000
#define P20KHZ2500W        2500
#define P20KHZ1250W        1250

#define F30KHZF_FACTOR 150
#define F30KHZMIDBAND 300000
#define P30KHZ1500W        1500

#define F40KHZF_FACTOR 200
#define F40KHZMIDBAND 399000
#define P40KHZ800W         800

#define FREQUENCY_MULTFACTOR   40

#define SGN(x) ((x>=0)?(x):(-x)) 
#define BIT(x) (1 << x)

#define BITS_ON_BYTE 				8
#define BITS_ON_SHORT_WORD			16
#define WORDS_ON_SIZE64				2
#define BYTES_ON_SIZE64				8
#define BYTE_MASK		0XFF


#define CONV_MILLI_MICROSEC(x)  (x * 1000)
#define CONVMICRO_MILLI(x)      (x / 1000)

// Control Loop Definitions
#define AMPLOOP_C1FACTOR   		10000
#define AMPLOOP_C2FACTOR   		1000000
#define FREQLOOP_KI				10000
#define FREQLOOP_KP				10000
#define AMPLOOP_C1FACTORNEW   	10000
#define AMPLOOP_C2FACTORNEW   	1000000
#define MAX_NUM_PHASE_AVERAGES 	500
#define MIN_NUM_PHASE_AVERAGES 	1
#define ADC11BITVALUE      		2047//10000
#define SCALING_FACTOR     		100
#define REFERENCE_ADCVOLT  		2500 // 2.5V
#define MAX_PHASE_DEGREES		1800  //180.0°
//#define MAX_PHASE_DEGREES		180  //180°
#define PHASE_FACTOR			104  // 5v/4.8v *100 where 4.8v = 180°

typedef
	enum {STANDARD, STANDARD_PLUS, AMPLITUDE_LOOP_ON_STD_DDS, AMPLITUDE_LOOP_OFF_STD_DDS, SCAMPLITUDE_LOOP_ON_SYNC_DDS,  AMPLITUDE_LOOP_ON_SYNC_DDS}HDMODES;


enum FUNCTION
{
   SEEKFUNCTION=1,              /* seek operation*/
   WELDFUNCTION,                /* weld operation*/
   TESTFUNCTION					  /* Test operation*/
};

typedef
		enum FREQUENCY
		{
			FREQ_20KHZ = 20,
			FREQ_30KHZ = 30,
			FREQ_40KHZ = 40,
		}FREQUENCY;
		
	enum DUPS_FREQCONVERSION   /*The appropriate units are as follows:*/
	{   DUPS_15KHZ = 3,   /*  15Khz Units = (3 * 0.0025) 0.075hz/bit*/
		DUPS_20KHZ = 4,   /*  20Khz Units = (4 * 0.025 ) 0.100hz/bit*/
		DUPS_30KHZ = 6,   /*  30Khz Units = (6 * 0.025 ) 0.150hz/bit*/
		DUPS_40KHZ = 8    /*  40hz  Units = (8 * 0.025 ) 0.200hz/bit*/
	};
	
	enum DDSMode
		{
			STD_DDS_AMPLITUDE_LOOP_ON = 0,
			STD_DDS_AMPLITUDE_LOOP_OFF = 1,
			SYNC_DDS_AMPLITUDE_LOOP_ON = 2,
			SYNC_DDS_AMPLITUDE_LOOP_OFF = 3,
		};
			
		
TxPDO_PC PC_TxPdo,PrevPC_TxPdo;

RxPDO_PC PC_RxPdo,PrevPC_RxPdo;

HornScanGraphPointData PC_ScanTxPdo;

#endif /* COMMON_H_ */
