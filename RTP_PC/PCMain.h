/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_

#include<unistd.h>
#include<iostream>
#include<eventLib.h>
#include"PCTaskCreation.h"

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILED
#define FAILED -1
#endif

#ifndef PRINT
//#define PRINT 1
#endif

#ifndef DEBUG
//#define DEBUG 1
#endif

#ifndef ERROR_PRINT
#define ERROR_PRINT	1
#endif

#ifndef COMPLETE_ACCESS
#define COMPLETE_ACCESS 1
#endif

#ifndef GLOBAL_BUFFER_SIZE
#define GLOBAL_BUFFER_SIZE 1024
#endif

#ifndef LOG
#define LOG PCObject :: pLogInfo->PrintMsg		//For Print
#endif

#ifndef TICKS_FOR_250US
#define TICKS_FOR_250US 4000
#endif
#ifndef TICKS_FOR_1MS
#define TICKS_FOR_1MS 1000  // 1ms
#endif


#ifndef READY
#define READY 1
#endif 

#ifndef DELAY
#define DELAY 5
#endif

#ifndef SUBINDEX
#define SUBINDEX 0
#endif



class LogInfo
{
public:
       void PrintMsg(char *msg, _Vx_usr_arg_t val1=0, _Vx_usr_arg_t val2=0, _Vx_usr_arg_t val3=0, _Vx_usr_arg_t val4=0, _Vx_usr_arg_t val5=0, _Vx_usr_arg_t val6=0);
};

namespace PCObject
{
	static LogInfo *pLogInfo = NULL;		//For Print
}

#endif /* MAIN_H_ */
