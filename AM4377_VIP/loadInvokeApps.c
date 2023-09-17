/*
 * loadInvokeApps.c
 *
 *  Created on: Aug 28, 2017
 *      Author: ee209315
 */

#include <loadInvokeApps.h>
extern int addSyscall (void);
extern void init_ecat(void);

void loadInvokeApps()
{
	taskDelay(sysClkRateGet( ));		// To let the VxWorks boot up completely(By mounting file system) before loading modules	
	printErr("Waiting for filesystem mount!!!\n");
	MODULE_ID 	moduleId;
	INT32 		fd;
	BOOL 		isAC = FALSE;
	    
	printErr("Opening ESS!!!\n");
   	fd = open (ESS_DKM_PATH, 0, 0);		// Add the SD Card ESS Application path    	
   	if (fd == -1) {
   		printErr("ERROR in Opening ESS Executable!!!\n");
   		exit(-1); 
   	}    	
    moduleId = loadModule (fd , GLOBAL_SYMBOLS);    	
    (void) close (fd);    
    if( moduleId  == NULL) {
    	printErr("ERROR in Loading ESS Executable!!!\n");
    	exit(-1); 
    }  
    else {    
    	FUNCPTR startfunction;
    	SYM_TYPE symtype;
    	STATUS rc;        	
		rc = symFindByName(sysSymTbl, "addSyscall", (char **)&startfunction, &symtype);	
    	if ( rc == 0 ) {    		
    		startfunction();			// Register Dynamic Custom system call
    		rc = symFindByName(sysSymTbl, "init_ecat", (char **)&startfunction, &symtype);
    		if ( rc == 0 ) {
    			startfunction();		// Start the EtherCat application
    		}
    		else {
    		    printErr("Failed to run the EtherCat application!!!\n");
    		    exit(-1);
    		}
		}
    	else {
    		printErr("Failed to add Dynamic Custom System calls'!!!\n", errno);
    		exit(-1); 
		}
	}                
}



void spawnRtpApp()
{
	int spawnStatus = 0;
	taskDelay(sysClkRateGet( ));
	/* Only one Executable must be present in the SD Card (AC/PC). Trying to Run AC Executable */
	spawnStatus = rtpSp(RTP_AC_PATH, INIT_TASK_PRIORITY, USER_STACK_SIZE, LAUNCH_OPTIONS, LAUNCH_TASK_OPTIONS);		//Default options to spawn AC process
	if (spawnStatus == -1)
	{
		printErr("ERROR in Spawning AC Executable!!!\nAttempting to run as PC\n");
		spawnStatus = rtpSp(RTP_PC_PATH, INIT_TASK_PRIORITY, USER_STACK_SIZE, LAUNCH_OPTIONS, LAUNCH_TASK_OPTIONS);	//Default options to spawn PC process
		if (spawnStatus == -1)
		{
			printErr("ERROR in Opening Both Executables!!!\n");    		
			exit(-1);
		}
	}	
}
