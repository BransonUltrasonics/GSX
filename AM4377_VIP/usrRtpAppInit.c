/* usrRtpAppInit.c - RTP Application Launch and Initialization Configlette */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,07may04,yvp  Moved rtpLaunchFromList() rtplaunch.c.
01c,26apr04,yvp  Check return value from rtpSpawn() - SPR 94883.
01b,31mar04,yvp  Fixed compiler warnings.
01a,26feb04,yvp  written
*/

/*
DESCRIPTION
User-defined code to launch RTP applications after system boot.
*/ 

/* Function declarations */
#include <vxWorks.h>
#include <loadLib.h>
#include <sysSymTbl.h>
#include <unldLib.h>


extern void spawnRtpApp();
/******************************************************************************
*
* usrRtpAppInit - initialize the users RTP applications.
* 
******************************************************************************/ 
void usrRtpAppInit (void)
{
	spawnRtpApp();
}


