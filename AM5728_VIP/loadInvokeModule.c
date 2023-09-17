/*****************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    LoadInvokeModule auto launches kernel and user space applications.   
 
****************************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if_dl.h>
#include <net/if.h>
#include <vxWorks.h>
#include "loadInvokeModule.h"

// MAC address related
#define EIOCGADDR _IORU('e', 7, END_PHYADDR) /* Get device address */
typedef unsigned char END_PHYADDR [16];

/**************************************************************************//**
* \brief   - Copies given support files into emmc storage.
* 			
* \param   - char *supportFilePath - Absolute path of the file to be copy.
*
* \return  - None.
*
******************************************************************************/
void copyFile(char * supportFilePath)
{
	INT32 	fd;
	STATUS	status = OK;
	
	/* Check the file is present in ROMFS/SD card */	
	fd = open(supportFilePath, 0, 0);
	
	if (ERROR == fd)
	{
		printf("Error opening file from %s \n", supportFilePath);
	}
	else
	{
		/* If file is present, then copy to eMMC */
		status = cp(supportFilePath , "/emmc0:0");
		
		if( OK != status)
		{
			printf("Error copying file from %s to EMMC\n", supportFilePath);
		}
		
		/* Close the file */
		close(fd);
	}
	
}

/**************************************************************************//**
* \brief   - Implemented to format the emmc storage device to HRFS.
* 			
* \param   - None.
*
* \return  - INT32 ret (OK or ERROR)
*
******************************************************************************/
INT32 HRFSEMMCFormat(void)
{
	INT32 fd 	= ERROR;
	INT32 ret 	= ERROR;
	device_t 	EMMCdevProperty;
	
	fd = open("/emmc0:0",0,0);
	/* Check for EMMC mount point */
	if(ERROR != fd)
	{
		EMMCdevProperty = devGetByName("/emmc0:0");
		/* device file system check */
		ret = hrfsDiskProbe(EMMCdevProperty);
		if(OK == ret)
		{
			printf("\nEMMC is already HRFS formatted! \n");
		}
		else
		{
			printf("Formatting EMMC with HRFS........ \n");
			ret = hrfsFormat ("/emmc0:0", 0ll,0, 0);
			if(ERROR == ret)
			{
				printf("\nFormat error\n");
			}
			else
			{
				/* HRFS Format */
				printf("\nEMMC HRFS format is successful\n");
				ret = OK;
			}
		}
		close(fd); /* EMMC - File Descriptor Closed */
	}
	else
	{
		printf("\n No Emmc device found\n");
	}
	return ret;
}

/**************************************************************************//**
* \brief   - Implemented to format the SATA storage device to HRFS.
* 			
* \param   - None.
*
* \return  - INT32 ret (OK or ERROR)
*
******************************************************************************/
INT32 HRFSSATAFormat(void)
{
	INT32 fd 	= ERROR;
	INT32 ret 	= ERROR;
	device_t 	SATAdevProperty;
	INT32 		mount_delay;
	
	/* Delay for SATA device to get mounted */
	for(mount_delay=0;mount_delay<0xffffffff;mount_delay++);
				
	fd = open("/ata0:1",0,0);
	
	/* Check for SATA mount point */
	if(ERROR != fd)
	{
		SATAdevProperty = devGetByName("/ata0:1");
		/* device file system check */
		ret = hrfsDiskProbe(SATAdevProperty);
		if(OK == ret)
		{
			printf("\nAlready a HRFS format\n");
		}
		else
		{
			printf("Formatting as HRFS........");
			/* HRFS Format */
			ret = hrfsFormat ("/ata0:1", 0ll,0, 0);
			if(ERROR == ret)
			{
				printf("\nFormat error\n");
			}
			else
			{
				printf("\nFormat successful\n");
				ret = OK;
			}
		}
		close(fd); /* ATA - File Descriptor Closed */
	}
	else
	{
		printf("\n No SATA device found\n");
	}
	return ret;
}

/**************************************************************************//**
* \brief   - Passes support files that needs to be copied into emmc from romfs.
* 			
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void copySupportFilesToEMMC()
{
	copyFile(UPDATED_WELD_RECIPE_PATH);		/* Copy UpdatedWeldRecipe.txt to eMMC */
	copyFile(DEFAULT_WELD_RECIPE_PATH);		/* Copy WeldRecipe.txt to eMMC */
	copyFile(DEFAULT_DINPUT_RECIPE_PATH);	/* Copy InputRecipe.txt to eMMC */
	copyFile(TIMER_INTERVAL_RECIPE_PATH);	/* Copy TimerIntervalRecipe.txt to eMMC */
	copyFile(PORT_CONFIG_PATH);				/* Copy Port.cfg to eMMC */
	copyFile(PDF_REPORT_BRANSON_LOGO_PATH); /* Copy Branson_gray_resize.jpg to eMMC */
	copyFile(PDF_REPORT_FONT_PATH);         /* Copy Courier.ttf to eMMC */
}

/**************************************************************************//**
* \brief   - Loads symbol files which are required to launch the DKM_EMC
* 			 RTP_SC applications.
* 			
* \param   - INT8 typeOfApp - (DKM_ECM and RTP_SC).
*
* \return  - STATUS status (OK or ERROR).
*
******************************************************************************/
STATUS loadApplicationModule(INT8 typeOfApp)
{
	MODULE_ID	moduleId	= NULL;
	INT32 		fdLinkLayer	= ERROR;
	INT32		fdEcmDemo	= ERROR;
	FUNCPTR		startfunction;
	SYM_TYPE	symtype;
	STATUS		status		= OK;
	TASK_ID		tid			= ERROR;
	
	taskDelay(2 * sysClkRateGet( ));
	if(KERNEL == typeOfApp)		/* Spawn Kernel space(DKM) application */
	{
		#ifdef CONFIG_CUSTOM_SC_AM5728
			status = ecatPhyReset();
			if(ERROR == status)
				return status;
		#endif
			
		#ifdef ICSS_PORT
			/* Open ICSS LinkLayer module */
			fdLinkLayer = open (ICSS_LL_PATH, 0, 0);   	
			if (fdLinkLayer == ERROR) 
			{
				printErr("ERROR in Opening link layer - emllICSS.out\n");
				return ERROR;
			} 
		#else
			/* Load CPSW LinkLayer module */
			fdLinkLayer = open (CPSW_LL_PATH, 0, 0);   	
			if (fdLinkLayer == ERROR) 
			{
				printErr("ERROR in Opening link layer - emllCPSW.out\n");
				return ERROR;
			}
		#endif
		
		moduleId = loadModule (fdLinkLayer , LOAD_ALL_SYMBOLS);     
		(void) close (fdLinkLayer);    
		if( moduleId  == NULL) 
		{	
		#ifdef ICSS_PORT
			printErr("ERROR in loading link layer emllICSS.out\n");
		#else
			printErr("ERROR in loading link layer emllCPSW.out\n");
		#endif			
			return ERROR; 
		}
		
		/* Open EtherCAT Master Application */
		fdEcmDemo = open (ECMASTER_PATH, 0, 0);		
		if (fdEcmDemo < 0) 
		{
			printErr("ERROR in Opening EtherCAT Master\n");    		
			return ERROR; 
		}    	    
		
		moduleId = loadModule (fdEcmDemo , LOAD_ALL_SYMBOLS);
		(void) close (fdEcmDemo);
		if( moduleId  != NULL) 
		{    	
			status = symFindByName(sysSymTbl, "scMain", (char **)&startfunction, &symtype);
			if ( status == 0 ) 
			{
			#ifdef ICSS_PORT
			tid = taskSpawn("SC_MAIN",100,0,4096,
							(FUNCPTR)startfunction(BUS_SCAN_ICSS),
							0,0,0,0,0,0,0,0,0,0);
			#else
			tid = taskSpawn("SC_MAIN",100,0,4096,
							(FUNCPTR)startfunction(BUS_SCAN_CPSW),
							0,0,0,0,0,0,0,0,0,0);
			#endif
				
				if(tid == ERROR)
				{
					printErr("EtherCAT Master spawning failed!\n",0,0,0,0,0,0);
				}
			}
			else 
			{
				printErr("scMain Entry point not found error: %08X\n", errno); 
			} 
		}   
		else 
		{
			printErr("ERROR in loading EtherCAT Master\n");
			return ERROR; 
		}
	}
	else if(USER == typeOfApp) /* Spawn User space(RTP) application */
	{
		DOS_VOLUME_DESC *pVolDesc	= NULL;
		int dataBase				= ERROR;
						
#ifdef DOSFS
		/* Check if the mounted volume has DOS FS mounted */
		if(NULL == dosFsVolDescGet("/emmc0:0", 0))
		{	
			/* If the volume is a raw device or other file system,
			 * format and mount DOS FS 
			 */
			status = dosFsVolFormat ("/emmc0:0", 0, 0);
			if( OK != status)
				printf("Formatting error!\n");	
			
		}
	
#else	
		/* HRFS format call for EMMC */
		if(ERROR == HRFSEMMCFormat())
		{
			printf("\nEMMC format failed\n");
		}
		
#if TBD		/* TODO: SATA */
		/* HRFS format call for SATA */
		if(ERROR == HRFSSATAFormat())
			printf("\nSATA format failed\n");
#endif
#endif	
		/* Check if the emmc is hosting the database */
		dataBase = open ("/emmc0:0/sample.db", 0, 0);   	
		if (ERROR == dataBase) 
		{
			printf("Database doesnt exist in EMMC.\n");
			/* If there is no database in emmc,
			 * copy it from ROMFS/sd card.*/
			
			//Remove sample.db-wal and sample.db-shm DB files if they exist in EMMC.
			if( access( "/emmc0:0/sample.db-wal", F_OK ) == OK ) 
			{
				if (rm("/emmc0:0/sample.db-wal") == ERROR)
				{
					printf("\nERROR DELETING sample.db-wal!\n");
				}
			}
			
			if( access( "/emmc0:0/sample.db-shm", F_OK ) == OK ) 
			{
				if (rm("/emmc0:0/sample.db-shm") == ERROR)
				{
					printf("\n ERROR DELETING sample.db-shm!\n");
				}
			}
			 
			status = cp(DATA_BASE_PATH , "/emmc0:0");
			if(OK != status)
			{
				printf("Cannot copy database. File copy error!\n");
			}
		}
		else
		{
			close(dataBase);	/* DB file descriptor close */
		}
		
		/* Copy Support Files to EMMC from ROM_FS */
		copySupportFilesToEMMC();
		
		/* Loads and starts logger application */
		if (ERROR == loggerInit())					/* LoggerModule task will be spawned after eMMC storage is completely ready */
		{
			printf("logger application loading error!\n");
		}
		
		/* Spawning RTP SC */		
		rtpSp(SC_RTP_PATH, 230, (128*1024), 0, 0x01000000);	
	}	
	return status;      
}

#ifdef CONFIG_CUSTOM_SC_AM5728
	/* EtherCAT PHY reset for AM572x SC Custom board */
	int ecatPhyReset(void)
	{
		STATUS	status	= OK;
		
		status = vxbGpioAlloc(GPIO4_5);
		if(ERROR == status)
		{
			return status;
		}
		else
		{
			status = vxbGpioSetDir(GPIO4_5, GPIO_OUTPUT);		
			if(OK == status)
			{
				status = vxbGpioSetValue(GPIO4_5, GPIO_HIGH);		
				if(OK == status)
					(void)vxbUsDelay(PHY_DELAY);
				else 
					return status;
				status = vxbGpioSetValue(GPIO4_5, GPIO_LOW);
			}
		}
		return status;
	}
	
	/* Enables 1 TB SATA device on SC Custom board */	
	int enableSATA(void)
	{
		STATUS	status	= OK;
		
		/*Enable 24V supply*/		
		status = vxbGpioAlloc(GPIO_24V_LOW);
		if(ERROR == status)
		{
			return status;
		}
		else
		{
			status = vxbGpioSetDir(GPIO_24V_LOW, GPIO_OUTPUT);		
			if(OK == status)
				status = vxbGpioSetValue(GPIO_24V_LOW, GPIO_LOW);
			else
				return status;
		}
		
		/*Enable EXT PWR supply*/		
		status = vxbGpioAlloc(GPIO_EXT_PWR);
		if(ERROR == status)
		{
			return status;
		}
		else
		{
			status = vxbGpioSetDir(GPIO_EXT_PWR, GPIO_OUTPUT);		
			if(OK == status)
				status = vxbGpioSetValue(GPIO_EXT_PWR, GPIO_LOW);
			else
				return status;
		}
		return status;
	}
#endif

/**************************************************************************//**
* \brief   -	Second instance (CPSW1) needs explicit
* 				instantiation. This will read the IP configuration from EEPROM
* 				or use defaults, if a failure occurs anywhere. Will also record the
*				MAC address for this device.
* 			
* \param   - None
*
* \return  - INT32 status
*
******************************************************************************/
INT32 initCpsw1()
{
	char IPAddr[IP_STRING_LENGTH+1]={0x00}, SubnetAddr[IP_STRING_LENGTH+1]={0x00}, GatewayAddr[IP_STRING_LENGTH+1]={0x00}, macAddr[100]={0x00};
	INT32 status = OK;
	int fd;
	char ipConfigCmd[100] ={0x00}, gatewayCmd[100]={0x00};
	char macaddr [6];

	/* Attach CPSW1 to the VxBus */
	status = ipAttach(1, 	/* CPSW Instance		 */		\
					 "cpsw"	/* Network device name	 */	);
	
	if(OK != status)
		printErr("initCpsw1: ipAttach: Attach failed! \n");
	else
	{
		if ((fd = open(EEPROM_PATH, O_RDWR, 0)) == ERROR)
		{
			printf("initCpsw1: Open: Failed to open IP_CONFIG_PATH using defaults\n");
			status = initCpsw1Defaults();
		}
		else
		{
			// get the MAC address and save now that EEPROM is open
			if (muxIoctl (muxTkCookieGet ("cpsw", 1), EIOCGADDR, (void*) macaddr) == OK)
			{
				snprintf(macAddr, sizeof (macAddr), "0x%02X%02X%02X%02X%02X%02X",
						macaddr[0],
						macaddr[1],
						macaddr[2],
						macaddr[3],
						macaddr[4],
						macaddr[5]);
#ifdef IPCONFIG_DBG
				printf("initCpsw1: muxIoctl: MAC address: %s\n", macAddr);
#endif
				
				ioctl (fd, FIOSEEK, EEPROM_MAC_ADDRESS_OFFSET);
				write(fd, macaddr, EEPROM_MAC_ADDRESS_SIZE);
				printf("initCpsw1: write: EEPROM_PATH: MAC address saved\n");
				
			}

			status = ERROR;
			
			// read saved IP configuration
			ioctl (fd, FIOSEEK, EEPROM_IPCONF_OFFSET);
			read(fd, ipConfigCmd, IPCONF_PARAM_LENGTH);
			
			sprintf(IPAddr,"%d.%d.%d.%d",ipConfigCmd[INET_ADDR_LEN-4],  ipConfigCmd[INET_ADDR_LEN-3], ipConfigCmd[INET_ADDR_LEN-2], ipConfigCmd[INET_ADDR_LEN-1]); 
			sprintf(SubnetAddr,"%d.%d.%d.%d", ipConfigCmd[NETMASK_LEN], ipConfigCmd[NETMASK_LEN+1], ipConfigCmd[NETMASK_LEN+2], ipConfigCmd[NETMASK_LEN+3]);
			sprintf(GatewayAddr,"%d.%d.%d.%d",ipConfigCmd[IPCONF_PARAM_LENGTH-4],  ipConfigCmd[IPCONF_PARAM_LENGTH-3], ipConfigCmd[IPCONF_PARAM_LENGTH-2], ipConfigCmd[IPCONF_PARAM_LENGTH-1]);
			
			// sanity check
			if (ipConfigCmd[INET_ADDR_LEN-4] != 255)
				if (ipConfigCmd[INET_ADDR_LEN-3] != 255)
					if (ipConfigCmd[INET_ADDR_LEN-2] != 255)
						if (ipConfigCmd[INET_ADDR_LEN-1] != 255)
							status = OK;
			
			if (status == OK)
			{
#ifdef IPCONFIG_DBG
				printf("initCpsw1: Open: using IP_CONFIG_PATH: IP address: %s subbet: %s\n", IPAddr, SubnetAddr);
#endif				
				memset(ipConfigCmd, 0x00, sizeof(ipConfigCmd));
				snprintf (ipConfigCmd, sizeof (ipConfigCmd), "%s %s netmask %s up", "cpsw1", IPAddr, SubnetAddr);
				status = ifconfig(ipConfigCmd);
				
				if(OK != status)
					printErr("initCpsw1: IP_CONFIG_PATH configuration failed! \n");
				else
				{
					// set gateway command
					snprintf(gatewayCmd, sizeof (gatewayCmd), "add default %s", GatewayAddr);
		
					// make it so
					status = routec(gatewayCmd); 
	
					if(status != OK)
						printErr("initCpsw1: routec: Error in setting Gateway\n");
#ifdef IPCONFIG_DBG
					else

						printf("initCpsw1: Open: using IP_CONFIG_PATH: Gateway address: %s routec(gatewayCmd) STATUS: %d\n", GatewayAddr, status);
#endif
				}
			}
			else
			{
				printf("initCpsw1: Open: IP_CONFIG_PATH invalid data using defaults\n");
				status = initCpsw1Defaults();
			}
		}
	}
	
	return status;
}


/**************************************************************************//**
* \brief   - Instantiate CPSW 1 using defaults, since a failure has occurred
*			 while trying to set the user IP configuration.	
* 			
* \param   - None
*
* \return  - INT32 status
*
******************************************************************************/
INT32 initCpsw1Defaults()
{
	char gatewayCmd[100]={0x00};
	INT32 status = OK;
	
	/* Setting up IP Configuration */
	status = ifconfig("cpsw1"					/* CPSW instance -2 */	\
					 " "						/* space */				\
					 "192.168.2.101"			/* CPSW1 IP address */	\
					 " "						/* space */				\
					 "netmask 255.255.255.0"	/* sub-net mask		*/	\
					 " "						/* space */				\
					 "up");						/* service start	*/
	
	if(OK != status)
		printErr("initCpsw1Defaults: CPSW1 default configuration failed! \n");
	else
	{
		// set default gateway command
		snprintf(gatewayCmd, sizeof (gatewayCmd), "add default 192.168.2.1");

		// make it so
		status = routec(gatewayCmd); 

		if(status != OK)
			printErr("initCpsw1Defaults: routec: Error in setting Gateway\n");
#ifdef IPCONFIG_DBG
		else
			printf("initCpsw1Defaults: Gateway address: %s \n", "192.168.2.1");
#endif
	}
	
	return status;
}
