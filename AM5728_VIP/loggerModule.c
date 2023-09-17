/*****************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    LoggerModule implemented to capture logs for debug with varies level.  
 
****************************************************************************/

#include <vxWorks.h>
#include "loggerModule.h"

UINT16 		logCount 	 	= 0; 			/* Number of line logged in active file */
UINT16 		toggleFlag 		= 0; 			/* Used to decide active file to be logged */
MSG_Q_ID 	loggerMsgQId	= MSG_Q_ID_NULL;
TASK_ID 	loggerTaskId 	= TASK_ID_ERROR;

/**************************************************************************//**
* \brief   - Counts number line present in logger file.
*
* \param   - None.
*
* \return  - UINT16 lineCount.
*
******************************************************************************/
UINT16 GetLineCount(const char *fileName)
{
	UINT16 lineCount = 0;
	FILE   *fp		 = NULL;
	INT8   value	 = 0;

	fp = fopen(fileName, "r");
	if(NULL == fp)
	{
		logMsg("GetLineCount : File open failed\n",0,0,0,0,0,0);
	}
	else
	{
		 for(value = fgetc(fp); value != EOF; value = fgetc(fp))
		{
			if(value == '\n')
			{
				lineCount++;
			}
		}
		
		fclose(fp);
	}
	
	return lineCount;
}

/**************************************************************************//**
* \brief   - Updates the logCount on every power up with latest line count.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void UpdateLogCounter()
{
	UINT16 numOfLine = 0;
	
	numOfLine = GetLineCount(LOGFILE1);
	if(numOfLine >= MAX_LOGS_COUNT)
	{
		numOfLine = GetLineCount(LOGFILE2);
		if(numOfLine < MAX_LOGS_COUNT)
		{
			toggleFlag ^= 1; /* Decides which file to log(logFile1 or logFile2) */
		}
	}
	
	logCount = numOfLine;
}

/**************************************************************************//**
* \brief   - Writes received logs into logFile1.txt or logFile2.txt based on
* 			 maximum number of lines configuration.
*
* \param   - char *buf - information to be logged into file.
*
* \return  - STATUS (OK or ERROR)
*
******************************************************************************/
STATUS loggerFile(char *buf)
{
	STATUS ret	 = OK;
	INT32 logFd1 = ERROR;
	INT32 logFd2 = ERROR;

	if(logCount >= MAX_LOGS_COUNT) /* When Maximum line count reached, log to another file */
	{
		toggleFlag ^= 1; /* Decides which file to log(logFile1 or logFile2) */

		if(0 == toggleFlag)
		{	 
			logFd1 = open(LOGFILE1, O_TRUNC, 0);
			if(logFd1 < 0)
			{
				logMsg("Error in truncating file 1\n",0,0,0,0,0,0);
				ret = ERROR;
			}
			else
			{
				lseek(logFd1, 0, SEEK_SET);
				close(logFd1);

				logFd1 = open(LOGFILE1, O_RDWR | O_APPEND, 0);
				if(logFd1 < 0)
				{
					logMsg("Error in opening file 1\n",0,0,0,0,0,0);
					ret = ERROR;
				}
			}
		}
		else if(1 == toggleFlag)
		{	
			logFd2 = open(LOGFILE2, O_TRUNC , 0);

			if(logFd2 < 0)
			{
				logMsg("Error in truncating file 2\n",0,0,0,0,0,0);
				ret = ERROR;
			}
			else
			{
				lseek(logFd2, 0, SEEK_SET);
				close(logFd2);

				logFd2 = open(LOGFILE2, O_RDWR | O_APPEND, 0);
				if(logFd2 < 0)
				{
					logMsg("Error in opening file 2\n",0,0,0,0,0,0);
					ret = ERROR;
				}
			}
		}
		else
		{
			logMsg("Invalid file toggle option\n",0,0,0,0,0,0);
			ret = ERROR;
		}

		logCount = 1; /* reset file count */ 
	}
	else /* When file does not reached the maximum no.of line limit */
	{
		if(0 == toggleFlag)
		{
			logFd1 = open(LOGFILE1, O_RDWR | O_APPEND, 0);
			if(logFd1 < 0)
			{
				logMsg("Files_1 does not exist with Error code : logFd1 : %d\n",logFd1,0,0,0,0,0);
				ret = ERROR;
			}
		}
		else if(1 == toggleFlag)
		{
			logFd2 = open(LOGFILE2, O_RDWR | O_APPEND, 0);
			if(logFd2 < 0)
			{
				logMsg("Files_2 does not exist with Error code : logFd2 : %d\n",logFd2,0,0,0,0,0);
				ret = ERROR;
			}
		}
		else
		{
			logMsg("Invalid file toggle option while opening file\n",0,0,0,0,0,0);
			ret = ERROR;
		}
	}

	if(OK == ret)
	{
		if(0 == toggleFlag) /* Logging into file 1 */
		{
			ret = write(logFd1, buf, strlen(buf));
			logCount++;
			close(logFd1);
		}
		else if(1 == toggleFlag) /* Logging into file 2 */
		{
			ret = write(logFd2, buf, strlen(buf)); 
			logCount++;
			close(logFd2);
		}
	}
	
	return ret;
}

/**************************************************************************//**
* \brief   - Logger module task function routine. Identifies log level and 
* 			 Based on log level takes action.
* 			 
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void loggerTask(void)
{
	char RecvMsgBuffer[MAX_SIZE_OF_RECVMSG_LENGTH];
	UINT8 LogLevel = 0;
	UINT8 FileFlag = 0;
	LogMessage logMsgBuffer;

	UpdateLogCounter();
	
	for(;;)
	{
		memset(RecvMsgBuffer,0x00,sizeof(RecvMsgBuffer));
		memset(&logMsgBuffer,0x00,sizeof(logMsgBuffer));

		if(msgQReceive(loggerMsgQId, RecvMsgBuffer, sizeof(RecvMsgBuffer), WAIT_FOREVER) != ERROR)
		{
			memcpy((char *)&logMsgBuffer, RecvMsgBuffer, sizeof(RecvMsgBuffer));

			switch(logMsgBuffer.LogLevel)
			{
				case LOG_CRITICAL:
					if(ENABLE_LOGFILE == logMsgBuffer.FileFlag)
					{
						if(loggerFile(logMsgBuffer.Buffer) < 0)
						{
							logMsg("loggerTask: Error in logging into a file\n",0,0,0,0,0,0);
						}
					}
	
					if(logMsgNoWait(logMsgBuffer.Buffer, 0, 0, 0, 0, 0, 0) == ERROR)
					{
						logMsg("loggerTask: Error in displaying on console\n",0,0,0,0,0,0);
					}
					break;
	
				case LOG_ERROR:
					if(ENABLE_LOGFILE == logMsgBuffer.FileFlag)
					{
						if(loggerFile(logMsgBuffer.Buffer) < 0)
						{
							logMsg("loggerTask: Error in logging into a file\n",0,0,0,0,0,0);
						}
					}
	
					if(logMsgNoWait(logMsgBuffer.Buffer, 0, 0, 0, 0, 0, 0) == ERROR)
					{
						logMsg("loggerTask: Error in displaying on console\n",0,0,0,0,0,0);
					}
					break;
	
				case LOG_WARNING:
					if(logMsg(logMsgBuffer.Buffer, 0, 0, 0, 0, 0, 0) == ERROR)
					{
						logMsg("loggerTask: Error in displaying on console\n",0,0,0,0,0,0);
					}
					break;
	
				case LOG_DEBUG:
					if(logMsg(logMsgBuffer.Buffer, 0, 0, 0, 0, 0, 0) == ERROR)
					{
						logMsg("loggerTask: Error in displaying on console\n",0,0,0,0,0,0);
					}
					break;
			}
		}
		else /* message queue receive - if */
		{
			logMsg("LoggerModule : Error on receiving message from MsgQ\n",0,0,0,0,0,0);
		}
		
		taskDelay(1);	
	}
}

/**************************************************************************//**
* \brief   - Initializes prerequisite for logger module.
* 			 
* \param   - None.
*
* \return  - STATUS (OK or ERROR)
*
******************************************************************************/
STATUS loggerInit(void)
{
	STATUS ret 	 = OK;
	INT32 logFd1 = ERROR;
	INT32 logFd2 = ERROR;
	
	logFd1 = open(LOGFILE1, O_RDWR | O_CREAT | O_EXCL, 0644);
	if((logFd1 < 0) && (EEXIST != errno))
	{
		logMsg("loggerInit : logFile_1 creation failed\n",0,0,0,0,0,0);
		ret = ERROR;
	}
	else
	{
		close(logFd1);
	}

	logFd2 = open(LOGFILE2, O_RDWR | O_CREAT | O_EXCL, 0644);
	if((logFd2 < 0) && (EEXIST != errno))
	{
		logMsg("loggerInit : logFile_2 creation failed\n",0,0,0,0,0,0);
		ret = ERROR;
	}
	else
	{
		close(logFd2);
	}
	
	if(MSG_Q_ID_NULL == loggerMsgQId)
	{
		loggerMsgQId = msgQOpen("/LOG_Task_Q", MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO, OM_CREATE, 0);
	}

    if(MSG_Q_ID_NULL == loggerMsgQId)
	{
    	logMsg("LoggerModule : MessageQueue creation failed",0,0,0,0,0,0);
    	ret = ERROR;
	}
    
   	loggerTaskId = taskSpawn (LOGGER_TASK_NAME, LOGGER_TASK_PRIORITY, 0, LOGGER_T_STACK_SIZE, (FUNCPTR)loggerTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
    if(TASK_ID_ERROR == loggerTaskId)
	{
    	(void) msgQDelete (loggerMsgQId);
    	logMsg("LoggerModule : TaskSpwaning creation failed",0,0,0,0,0,0);
    	ret = ERROR;
	}
    
    return ret;
}
