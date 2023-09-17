/*
 * MainTask.h
 *
 *  Created on: Sep 14, 2022
 *      Author: rvite
 */

#ifndef MAINTASK_H_
#define MAINTASK_H_

#include <taskLib.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define MAX_MSG					10 
#define MAX_MSG_LEN				1024 
#define MSG_Q_FIFO				0x00

enum
{
	ARGV_TEST_ID= 1, 
	ARGV_COMMAND_ID, 
	ARGV_PARAM_LIST
};

typedef struct
{
	UINT32 msgID;
	char Buffer[100];
} MESSAGE;

enum TEST_INDEX
{
	NONE_TEST= 0,
	MAILBOX_TEST,
	ALL_TEST, // Should be the last in the list
};

int MailBoxTest();


#endif /* MAINTASK_H_ */
