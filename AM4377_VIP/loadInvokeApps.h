/*
 * loadInvokeApps.h
 *
 *  Created on: Aug 28, 2017
 *      Author: ee209315
 */

#ifndef LOADINVOKEAPPS_H_
#define LOADINVOKEAPPS_H_

/* Header files and Macros*/
#include <vxWorks.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <loadLib.h>
#include <sysSymTbl.h>
#include <unldLib.h>


#define ROM_FS  1 /*Remove the comment,to boot from  ROMFs*/

#ifdef ROM_FS
	#define ESS_DKM_PATH				"/romfs/AM4377_DKM_ESS.out"		// ESS DKM Path
	#define RTP_AC_PATH					"/romfs/RTP_AC.vxe"			// AC RTP Application Path
	#define RTP_PC_PATH					"/romfs/RTP_PC.vxe"			// PC RTP Application Path
#else
	#define ESS_DKM_PATH				"/sd0:1/AM4377_DKM_ESS.out"		// ESS DKM Path
	#define RTP_AC_PATH					"/sd0:1/RTP_AC.vxe"			// AC RTP Application Path
	#define RTP_PC_PATH					"/sd0:1/RTP_PC.vxe"			// PC RTP Application Path
#endif

#define INIT_TASK_PRIORITY			102
#define USER_STACK_SIZE				(64*1024)
#define LAUNCH_OPTIONS				0
#define LAUNCH_TASK_OPTIONS			0x01000000

/* Function declarations */
void loadInvokeApps();
void spawnRtpApp();


#endif /* LOADINVOKEAPPS_H_ */
