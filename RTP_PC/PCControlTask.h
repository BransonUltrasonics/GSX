/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef PCCONTROLTASK_H_
#define PCCONTROLTASK_H_

#include<eventLib.h>
#include<sys/types.h>
#include"PCControlTask.h"
#include"PCMain.h"


#ifndef EVENT_CTRL_TASK
#define EVENT_CTRL_TASK 0x00000001
#endif

#ifndef EVENT_FGPA_TASK
#define EVENT_FGPA_TASK 0x00000002
#endif
void PCControlTask();

#endif /* PCCONTROLTASK_H_ */
