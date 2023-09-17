/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef OBJDICTIONARY_H_
#define OBJDICTIONARY_H_

#include<sys/types.h>
#include"PCMain.h"
#include"Common.h"
#include "PCStateMachine.h"

#define STRUCT_PACKED __attribute__((packed))

#ifndef INDEX_WELD_RECIPE 
#define INDEX_WELD_RECIPE 0x2000
#endif

#ifndef INDEX_PC_ALARM 
#define INDEX_PC_ALARM    0x2002
#endif

#ifndef INDEX_RxPDO_TxPDO 
#define INDEX_RxPDO_TxPDO 0x2003
#define NUM_OF_TXPDO_ENTRY 7
#define COMPLETE_ACCESS 1
#endif

#ifndef FIRMWARE_INFO_INDEX 
#define FIRMWARE_INFO_INDEX			0x2005
#define NUM_OF_FW_INFO_ENTRY 		5
#define SUB_INDEX 					0 
#endif

#ifndef INDEX_TIMER_INTERVAL
#define INDEX_TIMER_INTERVAL  		0x2004
#endif 

#ifndef INDEX_HORN_SCAN_RECIPE 
#define INDEX_HORN_SCAN_RECIPE 0x2008
#endif
#ifndef INDEX_HORNSCAN_RESULT
#define INDEX_HORNSCAN_RESULT 0x2007
#define NUM_OF_HORNSCAN_RESULT_ENTRY 6
#endif

#ifndef INDEX_HORN_SCAN_RECIPE 
#define INDEX_HORN_SCAN_RECIPE 0x2008
#endif

#ifndef INDEX_SEEK_RECIPE 
#define INDEX_SEEK_RECIPE 0x2010
#endif
#ifndef FIRMWARE_UPGRADE_INDEX
#define FIRMWARE_UPGRADE_INDEX 0x2020
#endif

#ifndef INDEX_TEST_RECIPE 
#define INDEX_TEST_RECIPE 0x2006 
#endif

#ifndef FAILURE 
#define FAILURE -1 
#endif

#ifndef TEST_MODE 
//#define TEST_MODE
#endif



class TxPDO
{
	public:
	    void sendTxPDOToOD();
	private:
		UINT16 Amplitude;	
		UINT16 Phase;
		UINT16 Power;
		UINT32 Frequency;
		UINT16 Current;
		UINT8 PCevent;
		UINT8 PCstate;
};

class RxPDO
{
 	public:
		void processRxPDO();
		void recieveRxPDOFromOD();
};

#define STRUCT_PACKED __attribute__((packed))

#ifndef INDEX_WELD_RECIPE 
#define INDEX_WELD_RECIPE 0x2000
#endif

#ifndef INDEX_PC_ALARM 
#define INDEX_PC_ALARM    0x2002
#endif

typedef struct PcRealtimeData
{	
	UINT16 u16SubIndex0;  
	UINT16 MasterState;
	UINT16 Amplitude_1;
	UINT16 MasterEvents;
	UINT32 Frequency_1;
	UINT16 Amplitude_2;	
	INT16  Phase;
	UINT16 Power;
	UINT32 Frequency;
	UINT16 Current;
	UINT8  PCStatus;
	UINT8  PCstate;
}PcRealtimeData;


typedef struct 
{ 
	UINT16 Entries ;
	INT32 AmplitudeLoopC1;
	INT32 AmplitudeLoopC2;
	INT32 PhaseLoop ;
	INT32 PhaseLoopCF;
	INT32 FrequencyLow;
	INT32 FrequencyHigh ;
	INT32 PhaseLimitTime;
	INT32 PhaseLimit ;
	INT32 ControlMode;
	INT32 BlindtimeSeek ;
	INT32 Blindtimeweld;
	INT32 DDSSwitchtime;
	INT32 Parameter7 ;
	INT32 Parameter8;
	INT32 Parameter9;
	INT32 Parameter10;
	INT32 Parameter11 ;
	INT32 Parameter12;
	INT32 Parameter13;
	INT32 Parameter14;
	INT32 Parameter15;
	INT32 F_LimitTime;
	INT32 AmpProportionalGain;
	INT32 AmpIntegralGain;
	INT32 PhaseIntegralGain;
	INT32 PhaseProportionalGain;
	INT32 FrequencyWindowSize;
	INT32 RFofPhasecontrolloop;
	INT32 PIFrequencyLow;
	INT32 PIFrequencyHigh;
	INT32 PIPhaseLimitTime;
	INT32 PIPhaseLimit;
	INT32 WELDRamptime;
	INT32 StartFrequency;
	INT32 MemoryOffset;
	INT32 DigitalTune;
	INT32 FrequencyOffset;
}STRUCT_PACKED
WeldRecipeParam;

class WeldRecipe
{
	friend class Ready;
	friend class SeekRun;
	friend class WeldRun;
	friend class FPGA;
	friend class Sonics;
	friend class Estop;
	
	public:
		void ReadWeldRecipeFromOD();
	private:		
		WeldRecipeParam  PCWeldRecipe;
};
WeldRecipe WeldRecipeObj;
typedef struct 
{
	UINT16 	NumOfEntries;
	INT32	SeekAmplitudeLoopC1;
	INT32	SeekAmplitudeLoopC2;
	INT32 	SeekFreqLimitHigh;
	INT32 	SeekFreqLimitLow;
	INT32	SeekPhaseloopI;
	INT32	SeekPhaseloopCF;
	INT32	SeekRampTime;
	INT32  	SeekFreqOffset;
}STRUCT_PACKED
SeekRecipeParam;

class SeekRecipe
{
	friend class SeekRun; // Hugo
	public:
		void ReadSeekRecipeFromOD();
		SeekRecipeParam PCSeekRecipe;
	//private:	
		
};
SeekRecipe SeekRecipobj;

class VersionInfoSDO
{
	public:
		VersionInfoSDO ();
		INT16 WriteFirmwareInfoToOD ();
	
	private:
		
		// props to the firmware info data in OD (entry, length, offset)
		ULONG 						ObjSizeFWInfo;
		OBJCONST TOBJECT OBJMEM* 	pObjEntryFWInfo;
		UINT16 						offsetFWInfo[NUM_OF_FW_INFO_ENTRY];
		
};


class TestRecipe
{
	friend class WeldRun;
	friend class Sonics;
	public:
		void ReadTestRecipeFromOD();
	private:	
		WeldRecipeParam PCTestRecipe;
};
TestRecipe TestRecipeObj;


#endif


