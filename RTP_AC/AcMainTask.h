/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/



#ifndef ACMAINTASK_H_
#define ACMAINTASK_H_


/* Header files */
#include <vxworks.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sdLib.h>
#include "AcControlTask.h"		
#include "AcDgtInput.h"
#include "AcDgtOutput.h"
using namespace std;


/* Macros */
#ifndef STACK_SIZE
#define STACK_SIZE 2048
#endif

#ifndef PDI_INTERRUPT
#define PDI_INTERRUPT VXEV03
#endif

#ifndef DELAY
#define DELAY	  5		// Delay in Ticks per Second unit
#endif

#ifndef NUM_OF_TASKS
#define NUM_OF_TASKS 6
#endif


#ifndef BUFFER_SIZE
#define BUFFER_SIZE 100
#endif


/* Variables and Function declarations */

/* TaskInfo - Details of Tasks' */
typedef struct 		
	{		
	char* TaskName[NUM_OF_TASKS];	
	enum TaskPriority{SERVOTASK_PRIORITY = 100, CONTROLTASK_PRIORITY,
		DGTINPUT_TASK_PRIORITY = 101, 
		DGTOUTPUT_TASK_PRIORITY = 102, FWUPGRADE_TASK_PRIORITY = 103};	
	enum taskIndex{SERVOTASK_INDEX = 0, CONTROLTASK_INDEX, DGTINPUT_TASK_INDEX, DGTOUTPUT_TASK_INDEX, FWUPGRD_TASK_INDEX};	
	TASK_ID tidControl;			// Control Task ID
	TASK_ID tidServo;			//Servo Task ID	
	TASK_ID tidDgtInput;		//Digital IO Input Task ID	
	TASK_ID tidDgtOutput;		//Digital IO Output Task ID	
	TASK_ID tidFwUpgrade;		//Firmware Upgrade Task ID	
	} TaskInfo;		
		
//Global Function spawned from Main Task
void controlTask();							
// Servo Task
void servoTask();				
// FirmwareUpgradeTask
void FwUpgradeRHandler();	

#endif /* ACMAINTASK_H_ */
