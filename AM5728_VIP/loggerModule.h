/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef LOGGERMODULE_H_
#define LOGGERMODULE_H_

#include <stdio.h>
#include <stdlib.h>
#include <taskLib.h>
#include <logLib.h>

#define MAX_LOGS_COUNT 				2000 					/* Maximum line Count */
#define ENABLE_LOGFILE 				1 						/* Enables logs into a file */
#define LOGFILE1 					"/emmc0:0/logFile1.txt" /* Path to store the logfile1 */
#define LOGFILE2 					"/emmc0:0/logFile2.txt" /* Path to store the logfile2 */
#define LOGGER_TASK_NAME 			"tLogger"
#define LOGGER_TASK_PRIORITY 		230
#define LOGMSG_LEN 					512
#define LOGGER_T_STACK_SIZE			(1024 * 8)

/* Message queue OFFSETS */
#define MAX_MSG						1000 
#define MAX_MSG_LEN					1024 
#define MSG_Q_FIFO					0x00
#define MAX_SIZE_OF_RECVMSG_LENGTH 	514 	/* Maximum message length */

#define	_FCREAT						0x0200	/* Open with file create */
#define	_FAPPEND					0x0008	/* Append (writes guaranteed at the end) */
#define	_FTRUNC						0x0400	/* Open with truncation */
#define	_FEXCL						0x0800	/* error on open if file exists */
#define	O_RDWR						2		/* +1 == FREAD|FWRITE */
#define	O_CREAT						_FCREAT
#define	O_APPEND					_FAPPEND
#define	O_TRUNC						_FTRUNC
#define O_EXCL						_FEXCL

typedef enum 
{
	LOG_CRITICAL = 1,
	LOG_ERROR,
	LOG_WARNING,
	LOG_DEBUG
}LOG_LEVEL_LIST;

typedef struct 
{
	UINT8 LogLevel;
	UINT8 FileFlag;
	char  Buffer[LOGMSG_LEN];	
}LogMessage;

/* Function loads the logger application and starts the entry point called loggerInit() */
STATUS loggerInit(void);
void   UpdateLogCounter();
UINT16 GetLineCount(const char *fileName);

#endif /* LOGGERMODULE_H_ */
