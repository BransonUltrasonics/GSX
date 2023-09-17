/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     This file contains task creation of Control Task, FPGA and PCStub task etc...
 
**********************************************************************************************************/

#include "PCTaskCreation.h"
#include "FirmWareUpgrade.h"
using namespace std;

/******************************************************************************
*      Function    :   PCTaskCreation
*      Description :   Constructor for PCTaskCreation object
*      Input       :   void
*      Output      :   - 
*      Called by   :   main()
*******************************************************************************/
PCTaskCreation::PCTaskCreation()
{
	Buffer = (UINT16 MBXMEM*)malloc(BUFFERSIZE);
	for(int TaskCreationIter = 0;TaskCreationIter < NUMOFTASKS; TaskCreationIter++)
	{		
		TaskId[TaskCreationIter] = -1;
	}
}

/******************************************************************************
*      Function    :   ~PCTaskCreation
*      Description :   Destructor for PCTaskCreation object
*      Input       :   void
*      Output      :   - 
*      Called by   :  main
*******************************************************************************/
PCTaskCreation::~PCTaskCreation()
{
	#ifdef PRINT
		LOG("PC object destroying\n");
	#endif
}

/******************************************************************************
*      Function    :   SpawnTask
*      Description :   This function is responsible for spawning tasks
*      Input       :   void
*      Output      :   void
*      Called by   :   main()
*******************************************************************************/
void PCTaskCreation::SpawnTask()
{
	char *TaskName[NUMOFTASKS]={"/ControlTask", "CommFPGATask","FwUpgradeTask"}; 
	int TaskPriority[NUMOFTASKS]={100, 101,103};
	void (*ptrTaskFunc[NUMOFTASKS])(void)={PCControlTask, CommFPGATask, FwUpgradeRHandler};
	int Status;
	for(int TaskSpawnIter=0;TaskSpawnIter<NUMOFTASKS;TaskSpawnIter++)
	{		
		if((Status=taskSpawn(TaskName[TaskSpawnIter],TaskPriority[TaskSpawnIter],0,STACKSIZE,(FUNCPTR)(ptrTaskFunc[TaskSpawnIter]),0,0,0,0,0,0,0,0,0,0))==ERROR)
		{
			#ifdef ERROR_PRINT
				LOG("Task Creation failed\n");
			#endif
			return;
		}
		else
		{
			TaskId[TaskSpawnIter]=Status;			
			#ifdef PRINT
				LOG(" -------------------- Task Created with ID %d --------------------\n\n",TaskId[TaskSpawnIter]);
			#endif
		}
	}	
	
}


