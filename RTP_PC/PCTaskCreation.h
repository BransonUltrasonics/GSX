/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef PCTASKCREATION_H_
#define PCTASKCREATION_H_

#include<taskLib.h>
#include"PCMain.h"
#include"PCControlTask.h"
#include"CommFPGATask.h"
extern "C"
{
#include<objdef.h>
}

#ifndef STACKSIZE
#define STACKSIZE 2048
#endif

#ifndef BUFFERSIZE
#define BUFFERSIZE 1024
#endif

#ifndef NUMOFTASKS
#define NUMOFTASKS 3
#endif 

#define PCCONTROLTASK_PRIORITY	100
#define COMMFPGATASK_PRIORITY	101

#define PCCONTROLTASK_ID	0
#define COMMFPGATASK_ID		1

TASK_ID TaskId[NUMOFTASKS];

class PCTaskCreation
{
public:
	PCTaskCreation();
	~PCTaskCreation();
	void SpawnTask();
	
};

UINT16 MBXMEM * Buffer;	// Global buffer used to store Pdo data from OD 

#endif /* PCTASKCREATION_H_ */
