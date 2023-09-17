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


/******************************************************************************
*
* usrRtpAppInit - initialize the users RTP applications.
*/
#include "vxworks.h"
#include "loadInvokeModule.h"
#include "loggerModule.h"

void usrRtpAppInit (void)
{	
	if(OK != WMB_WEBCLI_COMPONENT_Start())
	{
		printf("Web Server loading error!\n");
	}
	
	/* Loads and starts SC application */
	if (ERROR == loadApplicationModule(USER))
		printf("User application loading error!\n");
			
	//TODO: Temporarily EcMaster DKM is loaded here after launching RTP
	//		from usrRtpAppInit.c to enable autoboot
	if (ERROR == loadApplicationModule(KERNEL))
		kprintf("Kernel application loading error!\n");
}


