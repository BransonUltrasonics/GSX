// $Header:   D:/databases/VMdb/archives/EN13849/inc/StateMachine.h_v   1.10   20 Jul 2011 16:48:24   ASharma6  $
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
This module has the implementation of the State Machine of the Plastic
 Welder
 Module name: StateMachine
 File name: StateMachine.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/StateMachine.h_v  $
 * 
 *    Rev 1.10   20 Jul 2011 16:48:24   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.9   05 Jul 2011 10:57:26   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.6   22 Jun 2011 18:15:14   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 07:06:14   ygupta
 * Modification done to comply to coding standards.
 * 
 *    Rev 1.1   25 Mar 2011 09:20:02   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/


#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "StateMachineEntryFunc.h"
#include "StateMachineExitFunc.h"

#define FLASHONTIME  1000
#define FLASHOFFTIME 1000
#define FLASHOFFTIMEOFFPL 5000
#define MOTIONDELAY 30


/* Enum Structure for various states */
typedef enum WELDSTATES
{            	  /* Available weld states                 */
   PREREADY,
   READY,
   FAULT,
   FAULTRESET,
   DRIVEON,
   PCDETECTED,
   POWERUP
}ENSTATES;
/* Enum Structure for various states of Palm Button Status.*/
enum PALMBUTTON_STATUS
{
   BOTHHANDS_OFF,
   ONEHAND_OFF,
   BOTHHANDS_ON,
   SECONDHANDTOOLATE,
};
/* Enum Structure for various states of Pilot Light Status.*/
enum BEEPER
{
   OFF,
   ON,
   BEEPING
};
/* Enum Structure for various states of EStop Status.*/
enum ESTOPBUTTONSTATUS
{
   NOTPRESSED,
   PRESSED
};
/* Enum Structure for various states of Palm Button output Status.*/
enum PBOUTSTATUS
{
   PB_ENABLED,
   PB_DISABLED
};


enum ALARM		
{
	ALARM_0,
	ALARM_1,
	ALARM_2,
	ALARM_3,
};



enum DRIVESTATUS		
{
	 DRIVE_OFF,
	 DRIVE_ON
};

typedef void (*StateFunc)(void);		

typedef struct State {		
  
     StateFunc              Entry;         /* Pointer to executable function      */
     StateFunc  			Loop;        /* Pointer to next state if flag true  */
     StateFunc              Exit;       /* Pointer to next state if flag false */ 
} STATE;

extern STATE *CurrState;

void ChangeState(ENSTATES); 
void StateMachine(void);

//States loop function
void PreReadyState(void);
void ReadyState(void);
void DriveOnState(void);
void PCDetectedState(void);
void FaultState(void);
void FaultResetState(void);
void PowerUpState(void);




#endif /* STATEMACHINE_H_ */
