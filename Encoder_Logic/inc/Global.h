// $Header:   D:/databases/VMdb/archives/EN13849/inc/Global.h_v   1.25   09 Apr 2015 17:17:34   ewomack  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------
This module has  the list of all the Global variables used.

Module name: Global

Filename:    Global.h

--------------------------- TECHNICAL NOTES -------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/Global.h_v  $
 * 
 *    Rev 1.25   09 Apr 2015 17:17:34   ewomack
 * Update software version to 32.
 * 
 *    Rev 1.24   02 Jan 2015 14:57:06   ewomack
 * Update software version to 31
 * 
 *    Rev 1.22   20 Sep 2011 17:20:34   ASharma6
 * CRC of the executable is printed out on hyperterminal when program boots up.
 * 
 *    Rev 1.21   19 Sep 2011 10:35:34   ASharma6
 * For IW+ only: After ULS, the Trigger switch is not looked at for declaring part contact until the horn goes 1/8 of an inch past the ULS.
 * 
 *    Rev 1.19   24 Aug 2011 11:18:18   ASharma6
 * For IW+ only - if in part contact state, if palm buttons are still pressed then stay in that state and the horn position becomes the new reference for collapse until the hands are released.
 * 
 *    Rev 1.17   10 Aug 2011 12:36:50   ASharma6
 * Printf function does nothing.
 * 
 *    Rev 1.16   03 Aug 2011 10:35:12   ASharma6
 * For machine configuration C, the part contact lost algorithm shall declare lost 150ms after trigger goes away.
 * 
 *    Rev 1.15   25 Jul 2011 15:24:50   ASharma6
 * Modifications in WatchDog module - At power up, the voltage monitor resets the CPU first. On coming from a reset, the CPU should wait indefinitely for a WatchDog to reset it.
 * 
 *    Rev 1.14   20 Jul 2011 16:47:14   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.13   12 Jul 2011 18:32:40   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction 
 * 
 *    Rev 1.12   07 Jul 2011 12:21:06   ASharma6
 * TRS Diagnostics for IW+ machine configuration is modified
 * 
 *    Rev 1.11   05 Jul 2011 10:56:36   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.7   22 Jun 2011 18:14:14   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.1.1.9   15 Jun 2011 17:39:26   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.1.1.8   08 Jun 2011 07:58:10   pdwivedi
 * Added changes for phase 8.
 *
 *    Rev 1.1.1.7   20 May 2011 20:06:30   ygupta
 * Added new Variables.
 *
 *    Rev 1.1.1.6   10 May 2011 07:35:34   pdwivedi
 * Added changes for ULS inputs and removed LED module.
 *
 *    Rev 1.1.1.5   04 May 2011 08:58:32   pdwivedi
 * Added changes for cross monitoring(module level test cases), Device Diagnostics(channged) and SBeam Diagnostics(modified).
 *
 *    Rev 1.1.1.4   03 May 2011 06:59:42   pdwivedi
 * Added changes for Logic unit BIST.
 *
 *    Rev 1.1.1.3   02 May 2011 04:01:18   pdwivedi
 * Synchronize with CVS.
 *
 *    Rev 1.1.1.2   08 Apr 2011 08:38:52   PDwivedi
 * Added after sanity check.
 *
 *    Rev 1.1.1.1   01 Apr 2011 11:28:32   PDwivedi
 * Fixex after review.
 *
 *    Rev 1.1.1.0   31 Mar 2011 11:57:08   PDwivedi
 * Fixes after review.
 *
 *    Rev 1.2   29 Mar 2011 19:19:08   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:16:46   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "lpc_types.h"


// SW Revision number
#define SWREVISION 0x31

#define BIT_MASK(x)		(1<<x)
//#define PROTOTYPEBOARD  // Comment me for official release.

typedef unsigned char   BOOL;
typedef   signed char  tSINT8;
typedef unsigned char  tUINT8;
typedef   signed short tSINT16;
typedef unsigned short tUINT16;
typedef   signed long  tSINT32;
typedef unsigned long  tUINT32;

#define  BRANCH_1 1
#define  BRANCH_2 2
#define  BRANCH_3 3
#define  BRANCH_4 4
#define  BRANCH_5 5
#define  BRANCH_6 6
#define  BRANCH_7 7
#define  BRANCH_8 8
#define  BRANCH_9 9
#define  BRANCH_10 10
#define  BRANCH_11 11
#define  BRANCH_12 12
#define  BRANCH_13 13
#define  BRANCH_14 14
#define  BRANCH_15 15
#define  BRANCH_16 16
#define  BRANCH_17 17
#define  BRANCH_18 18
#define  BRANCH_19 19
#define  BRANCH_20 20
#define  BRANCH_21 21
#define  BRANCH_22 22
#define  BRANCH_23 23
#define  BRANCH_24 24
#define  BRANCH_25 25
#define  BRANCH_26 26
#define  BRANCH_27 27

#define WDTIMEOUT 300

extern tSINT32 WDCount;
extern BOOL NotMyFault;
extern tUINT8 State;
extern tSINT32 FaultFlag;
extern BOOL EStop;
extern tSINT32 BothHandStatus;
extern tSINT32 SV1_REQ;
extern tSINT32 SV2_REQ;
extern tSINT32 SV2;
extern tSINT32 SV1;
extern tSINT32 SONICS_EN;
extern tSINT32 BEEPER;
extern tSINT32 PB_OUT;
extern tSINT32 FaultReset;
extern tSINT32 PartContactStatus;
extern tSINT32 UFAIL24Status;
extern tSINT32 UFAIL24V;
extern tUINT32 DebouncedInputs;
extern tSINT32 EStopStatus;
extern tSINT32 LUTestStatus; 
extern tSINT32 LUTestErrMask;
extern tSINT32 ESTOP1;
extern tSINT32 ESTOP2;
extern tSINT32 DRIVE_FAIL;
extern tSINT32 SV2_FAIL;
extern tSINT32 FaultReset;
extern tSINT32 SVRelayCount;
extern tSINT32 DriveStatus;
extern tSINT32 SonicsEnStatus;
extern tSINT32 SBeamStatus;
extern tSINT32 LEStatus;
extern tSINT32 QEIPosition;
extern tSINT32 QEIPositionOld;
extern tSINT32 ULS;
extern tSINT32 ULSPosition;
extern tUINT32 OldULSPosition;
extern tSINT32 TRSStatus;
extern tSINT32 TRS;
extern tSINT32 DRIVE_FAIL;
extern tSINT32 SONICS_FAIL;
extern tSINT32 ESTOP_LO;
extern tSINT32 PB1_NO_IN;
extern tSINT32 PB2_NO_IN ;
extern tSINT32 ADCStatus;
extern tSINT32 EStopCount;
extern tSINT32 CurrForce;
extern tSINT32 WDStatus;
extern tSINT32 CurrVelocity;
extern tSINT32 PCForce;
extern tSINT32 PCLeftindex;
extern tSINT32 PeakVelocity;
extern tSINT32 volatile PB1_NO_INPUT;
extern tSINT32 volatile PB2_NO_INPUT;
extern tSINT32 volatile IsThisCPU1;
extern tSINT32 BytesToTx;
extern tUINT32 PrintfEnabled;
extern tUINT32 StoredFlashCRC;
extern tSINT32 ALARM; 


extern tSINT32 PBStatus;
//Current Input on CPU input Pin for PB1.
extern tSINT32 volatile PB1_INPUT;
//Current Input on CPU input Pin for PB2.
extern tSINT32 volatile PB2_INPUT;

tUINT32 BIST();
tSINT32 LogicUnitTest(void);
void TimerCallback(void);
extern void LEDInit(void);
extern void IOInit(void);
extern void SerialInit(tSINT32 port);
extern void InitSSP();
extern void QuadratureEncoderInit(tSINT32 maxPos);
extern void TestWD();
extern void ResetExternalWatchdog(void);
extern tSINT32 ChkFunReturn;
extern volatile tUINT32 ADC0IntDone;
extern tSINT32 MsInterruptCounter;
extern tSINT32 InterruptTestResult;
typedef union {
tSINT32 AllInputs;
   struct bitInput {
      unsigned U2Reset:1;
      unsigned TRS:1;
      unsigned DriveFail:1;
      unsigned SV2Fail:1; 
      unsigned SONICSFail:1;
      unsigned SV1Req:1;
      unsigned SV2Req:1; 
      unsigned EStopLow:1;
      unsigned EStopHi:1;
      unsigned EStop1:1;
      unsigned EStop2:1;
      unsigned ULS:1;
      unsigned PB2_NO:1;
      unsigned PB1_NO:1;
      unsigned UFail24V:1;
      unsigned MachConfig:1;
      unsigned unUsed:19;
   } InputBits;
}Input;

//Machine Configuration
extern tSINT32 MachineConfiguration;
extern int LastFault;
enum MACHINETYPE
{
   NOTIWPLUS,
   IWPLUS,
   GSXE,
   GSXP,
   GSXI
};

enum TESTRESULT
{
   FAIL,
   PASS,
};



// Enum for BIST Test. Total of 16ms and one test will be done at a time
// either on each ms or on every other ms according to the TEST number in this enum.
enum BIST_TEST
{
   BIST_REGISTER_TEST = 0,
   BIST_RAMSAFEAREA_TEST = 2,
   BIST_PC_TEST = 5,
   BIST_RAMDATAAREA_TEST = 6,
   BIST_ADC_TEST = 10,
   BIST_RAMSTACKAREA_TEST = 12,
   BIST_FLASH_TEST1 = 16,
   BIST_FLASH_TEST2 = 19,
   BIST_FLASH_TEST3 = 22,
   BIST_FLASH_TEST4 = 25,
   BIST_END = 27
};

#define FaultClr    0
tSINT32 PCDummy1(void);
tSINT32 PCDummy2 (void);
tSINT32 PCDummy3 (void);
tSINT32 PCDummy4(void);
tSINT32 PCDummy5 (void);

#endif /* GLOBAL_H_ */

