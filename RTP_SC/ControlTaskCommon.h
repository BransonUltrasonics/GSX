/*
 * ControlTaskCommon.h
 *
 *  Created on: Jun 1, 2017
 *      Author: lzhu
 */

#ifndef CONTROLTASKCOMMON_H_
#define CONTROLTASKCOMMON_H_

#include  <stdio.h>
#include  "vxWorks.h"
  
typedef enum {FREQ_NONE, FREQ_SERIES, FREQ_PARALLEL}RESONANCE_FREQ_TYPE;


typedef enum PDOList 
	{
		AC_CMD_PDO,
		PC_CMD_PDO,
		AC_STATUS_PDO,
		PC_STATUS_PDO
	}PdoType;

	
/*	Cmd from SC Master to AC slave	*/
typedef struct Struct_AC_CMD_PDO 
	{
		UINT8    SC_State;
		UINT8    AC_ControlEvent;
		UINT8    AC_Outputs;
	}S_AC_CMD_PDO;


/*	Status from AC slave to SC Master	*/
typedef struct Struct_AC_STATUS_PDO
	{
		UINT16	ActualForce;
		UINT32	ActualDistance;
		UINT8	ACStatusEvent;
		UINT8	ACState;
		UINT8   ACInputs;
	}S_AC_STATUS_PDO;

	
/*	Cmd from SC Master to PC slave	*/
typedef struct Struct_PC_CMD_PDO
	{
		UINT16	Amplitude;
		UINT16	SCState;
		UINT16	PC_ControlEvent;
	}S_PC_CMD_PDO;


/*	Status from PC slave to SC Master	*/
typedef struct Struct_PC_STATUS_PDO
	{
		UINT32   Frequency;
		UINT16   Amplitude;
		INT16    Phase;
		UINT16   Power;
		UINT16   Current;
		UINT8 	 PCState;
		UINT8    PC_StatusEvent;
	}S_PC_STATUS_PDO;


/*	Cmd and Status PDO structures	*/
typedef struct PdoStructure
	{
		S_PC_CMD_PDO	PcCmdPdo;
		S_AC_CMD_PDO	AcCmdPdo;
		S_PC_STATUS_PDO	PcStatusPdo;
		S_AC_STATUS_PDO	AcStatusPdo;
	}PdoObject;

	
/*	Cmd and Status SDO structures	*/	
typedef struct SDOStructure   //edited on 10/23/17
{
   UINT32 WeldTime;
   UINT32 HoldTime;
   UINT32 TriggerDistance;
   UINT32 TriggerForce;
   UINT32 WeldEnergy;
   UINT32 AbsoluteDistance;
   UINT32 CollapseDistance;
   UINT32 ScrubTime;
   UINT16 PeakPower;
 }SdoObject;

	
#define PB_ACTIVE_TIME_OUT_IN_US	6000000	//6 sec
#define PP_ACTIVE_TIME_OUT_IN_US	4000000	//4 sec
#define SLAVE_TIME_OUT_IN_US		100000	//100 msec
#define	ECAT_CYCLE_TIME_IN_US 		250
#define STAGGER_TIME_IN_US    		200000
#define TRIGGER_TIME_IN_US 			1000   
#define WAIT_FOR_HOME_TIME_IN_US 	1000
#define SEEK_TIME_IN_US             1000
#define INTERVAL_INDICATION			1000	// (2000/4 = 500ms)
#define WAIT_FOR_RDY_POS_TIME_IN_US	4000000 //4 seconds
#define AC_ACTIVE_MOVE_DELAY_IN_US	500000
 
	

#define SS1MASK 					0x01
#define SS2MASK 					0x02
#define BOTHSTARTSWITCHMASK 		0x03  	
#define GRDDETMASK 					0x04 
#define ALARMRESETMASK				0x08



#endif /* CONTROLTASKCOMMON_H_ */
