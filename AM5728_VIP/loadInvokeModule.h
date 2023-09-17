/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef LOADAPPMODULE_H_
#define LOADAPPMODULE_H_

#include <stdio.h>
#include <stdlib.h>
#include <loadLib.h>
#include <usrFsLib.h>
#include <sysSymTbl.h>
#include <unldLib.h>
#include <taskLib.h>
#include <errno.h>
#include <prjParams.h>
#include <private/dosFsLibP.h>

#define KERNEL 			0
#define USER			1
#define ROM_FS          
#define IP_STRING_LENGTH 15
/* Delay in seconds */
#define DELAY			8 // 1 + 1 + 1sec for SD card, EMMC & SATA mount
						  // + 4 (QSPI boot delay mitigation)

#define ICSS_PORT
//#define DOSFS

// IP configuration and MAC
#define EEPROM_PATH								"/eeprom/0"
#define EEPROM_IPCONF_OFFSET         			0x160
#define IPCONF_PARAM_LENGTH 					12
#define	INET_ADDR_LEN							4
#define NETMASK_LEN 							4
#define EEPROM_MAC_ADDRESS_OFFSET   			0x2E0
#define EEPROM_MAC_ADDRESS_SIZE   				6


#ifdef ROM_FS
	#define ICSS_LL_PATH						"/romfs/emllICSS.out"
	#define CPSW_LL_PATH						"/romfs/emllCPSW.out"
	#define ECMASTER_PATH						"/romfs/AM5728_DKM_ECM.out"
	#define ENI_PATH	 						"/romfs/SC_PC_AC_L1A.xml"
	#define SC_RTP_PATH							"/romfs/RTP_SC.vxe"
	#define UPDATED_WELD_RECIPE_PATH			"/romfs/UpdatedWeldRecipe.txt"
	#define DEFAULT_WELD_RECIPE_PATH			"/romfs/WeldRecipe.txt"
	#define UPDATED_SEEK_RECIPE_PATH			"/romfs/UpdatedSeekRecipe.txt"
	#define DEFAULT_SEEK_RECIPE_PATH			"/romfs/SeekRecipe.txt"
	#define SCAN_RECIPE_PATH					"/romfs/HornScanRecipe.txt"
	#define SCAN_RESULT_PATH					"/romfs/HornScanResults/ScanResult.txt"
	#define DEFAULT_DINPUT_RECIPE_PATH			"/romfs/InputRecipe.txt"
	#define TIMER_INTERVAL_RECIPE_PATH			"/romfs/TimerIntervalRecipe.txt"
	#define DATA_BASE_PATH						"/romfs/sample.db"
	#define PORT_CONFIG_PATH                    "/romfs/Port.cfg"
    #define PDF_REPORT_BRANSON_LOGO_PATH		"/romfs/Branson_gray_resize.jpg"
    #define PDF_REPORT_FONT_PATH                "/romfs/Courier.ttf"
	#define BUS_SCAN_CPSW						"-f /romfs/SC_PC_AC_L1A.xml -t 4000 -auxclk 250 -cpsw 1or2 1 1 m am572x-idk -v 3"
	#ifdef CONFIG_CUSTOM_SC_AM5728
//		#define GPIO4_5							0x85 	/* D2, GPIO Bank 4, ECAT PHY RESET PIN*/
		#define GPIO4_5							0x65 	/* D2, GPIO Bank 4, ECAT PHY RESET PIN*/
		#define GPIO_HIGH						1
		#define GPIO_LOW						0
		#define GPIO_OUTPUT						1
		#define GPIO_INPUT						0
		#define PHY_DELAY						200 	/* Micro Seconds 	*/
//		#define GPIO4_1							0x81	/* F2, 24V LOW 		*/	
		#define GPIO4_1							0x61	/* F2, 24V LOW 		*/	
		#define GPIO_24V_LOW					GPIO4_1
//		#define GPIO4_9							0x89	/* F5, EXT_PWR		*/
		#define GPIO4_9							0x69	/* F5, EXT_PWR		*/
		#define GPIO_EXT_PWR					GPIO4_9
		#define BUS_SCAN_ICSS					"-f /romfs/SC_PC_AC_L1A.xml -t 4000 -auxclk 250 -icss 3 1 m am572x-emerson -v 3"
	#else /* BOARD_TI_AM5728_IDK */
		#define BUS_SCAN_ICSS					"-f /romfs/SC_PC_AC_L1A.xml -t 4000 -auxclk 250 -icss 4 1 m am572x-idk -v 3"
	#endif

#else
	
	#define ICSS_LL_PATH						"/sd0:1/emllICSS.out"
	#define CPSW_LL_PATH						"/sd0:1/emllCPSW.out"
	#define ECMASTER_PATH						"/sd0:1/AM5728_DKM_ECM.out"
	#define ENI_PATH	 						"/sd0:1/SC_PC_AC_L1A.xml"
	#define SC_RTP_PATH							"/sd0:1/RTP_SC.vxe"
	#define UPDATED_WELD_RECIPE_PATH			"/sd0:1/UpdatedWeldRecipe.txt"
	#define DEFAULT_WELD_RECIPE_PATH			"/sd0:1/WeldRecipe.txt"
	#define UPDATED_SEEK_RECIPE_PATH			"/sd0:1/UpdatedSeekRecipe.txt"
	#define DEFAULT_SEEK_RECIPE_PATH			"/sd0:1/SeekRecipe.txt"
	#define SCAN_RECIPE_PATH					"/sd0:1/HornScanRecipe.txt"
	#define SCAN_RESULT_PATH					"/sd0:1/HornScanResults/ScanResult.txt"
	#define DEFAULT_DINPUT_RECIPE_PATH			"/sd0:1/InputRecipe.txt"
	#define TIMER_INTERVAL_RECIPE_PATH			"/sd0:1/TimerIntervalRecipe.txt"
	#define DATA_BASE_PATH						"/sd0:1/sample.db"
	#define PORT_CONFIG_PATH                    "/sd0:1/Port.cfg"
	#define BUS_SCAN_CPSW						"-f /sd0:1/SC_PC_AC_L1A.xml -t 4000 -auxclk 250 -cpsw 1or2 1 1 m am572x-idk -v 3"
	#ifdef CONFIG_CUSTOM_SC_AM5728
		#define GPIO4_5							0x85 	/* D2, GPIO Bank 4, ECAT PHY RESET PIN*/
		#define GPIO_HIGH						1
		#define GPIO_LOW						0
		#define GPIO_OUTPUT						1
		#define GPIO_INPUT						0
		#define PHY_DELAY						200 	/* Micro Seconds 	*/
		#define GPIO4_1							0x81	/* F2, 24V LOW 		*/	
		#define GPIO_24V_LOW					GPIO4_1
		#define GPIO4_9							0x89	/* F5, EXT_PWR		*/
		#define GPIO_EXT_PWR					GPIO4_9
		#define BUS_SCAN_ICSS					"-f /sd0:1/SC_PC_AC_L1A.xml -t 4000 -auxclk 250 -icss 3 1 m am572x-idk -v 3"
	#else /* BOARD_TI_AM5728_IDK */
		#define BUS_SCAN_ICSS					"-f /sd0:1/SC_PC_AC_L1A.xml -t 4000 -auxclk 250 -icss 4 1 m am572x-idk -v 3"
	#endif


#endif
				
#define RTP_CORE_DUMP_PATH	"/sd0:1/rtpCoreDump/"
									
/* Function loads the supervisory application and starts the entry point called SC_MAIN() */
STATUS loadApplicationModule(INT8 typeOfApp);

#ifdef CONFIG_CUSTOM_SC_AM5728
	int ecatPhyReset(void);
	int enableSATA(void);
#endif
int initCpsw1();
INT32 initCpsw1Defaults();

#endif /* LOADAPPMODULE_H_ */
