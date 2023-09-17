/*
 * SC_Common.h

 *
 *  Created on: May 17, 2017
 *      Author: lzhu
 */
#include "vxWorks.h"
#include <time.h>

#ifndef HEADER_FILES_SC_COMMON_H_
#define HEADER_FILES_SC_COMMON_H_

#define CONTROL_TASK_PRIORITY	100
#define PDO_TASK_PRIORITY		101
#define UI_TASK_PRIORITY		102
#define SDO_TASK_PRIORITY		103
#define DATABASE_PRIORITY		104

#define TASK_CONTROL_IDX		0
#define TASK_PDO_IDX			1
#define TASK_UI_IDX				2
#define TASK_SDO_IDX			3
#define TASK_DATABASE_IDX		4
#define TASK_SIZE				5

#define CONTROLTASK_STACK_SIZE 	2000


typedef struct {
	UINT16 UiPriority;
	UINT16 UiStackSize;
	UINT16 UiTask_id;
}Task_struct;


#endif /* HEADER_FILES_SC_COMMON_H_ */
