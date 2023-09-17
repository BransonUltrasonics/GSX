#include <vxWorks.h>
#include <stdio.h>
#include <taskLib.h>
#include <ecat_def.h>
#include <ecatappl.h>
#include <applInterface.h>
#include <ecatSlavePalLib.h>
#include <usrLib.h>
#include <ecatSlaveUserApp.h>
#include <vxbPruIntcLib.h>
#include <sdLib.h>
#ifdef ACTUATION_CONTROLLER_BUILD
#include <AcEcAppl.h>
#else
#include <PcEcAppl.h>
#endif //ACTUATION_CONTROLLER_BUILD

#include "firmwareInfo.h"

#define ENABLE_ONLINE_FIRMWARE_UPGRADE
TASK_ID tidPDI = 0;

#ifdef ACTUATION_CONTROLLER_BUILD
TOOBJ4003 *ptrsRealTimeData = &sRealTimeData;
#else
PcRealtimeData *ptrsRealTimeData;
#endif //ACTUATION_CONTROLLER_BUILD
//SEM_ID SemSyncOutputMapping, SemSyncInputMapping;

void ecatMainTask(void)
{
	int idt;
		Uint16 errInit = 0xFFFF ;
		HW_Init();
		
		errInit = MainInit();
		
//		SemSyncInputMapping = semOpen ("/ESS_INMAP",SEM_TYPE_BINARY,SEM_EMPTY,SEM_Q_FIFO,OM_CREATE,0);
//		if(SemSyncInputMapping == NULL)
//		{
//			printf("EcSlave: Semaphore  SyncInputMapping Creation failed \n");
//		}
//		SemSyncOutputMapping = semOpen ("/ESS_OUTMAP",SEM_TYPE_BINARY,SEM_EMPTY,SEM_Q_FIFO,OM_CREATE,0);
//		if(SemSyncOutputMapping == NULL)
//		{
//			printf("EcSlave: Semaphore  SyncOutputMapping Creation failed\n");
//		}

		if(0 != errInit)
		{
			printf("EcSlave:EtherCAT slave stack Initialization failed\n");
		}
		else
		{
			bRunApplication = TRUE ;
			printf("\n\n\nEcSlave:Executing EtherCAT slave stack\n");
			
		do{
			MainLoop();			
			/* Yielding processor to other Tasks */ 
			taskDelay(0);
		}while(bRunApplication == TRUE);
	}
	HW_Release();
}

void PDIHandler()
{
	FOREVER
    {  	
		if (ERROR == eventReceive(VXEV02, EVENTS_WAIT_ANY, WAIT_FOREVER, NULL))
		{
		 	printf("PDI ISR Event Receive failed!!!\n");
 		} else
 		{
			PDI_Isr();
		}
    }
}


void init_ecat (void) 
{
	int id;
	MSG_Q_ID FWmsgQId;
	FirmWareUpgradeInfo *FirmWareBuff;	
	pruintcPDIRegister(PDI_IsrFlag); 	//Registering PDI_ISR handler
	
	#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
		FWmsgQId = msgQOpen("/Fw_Msgq_Notify", MSGQ_NOTIFY_TO_RTP_SIZE, 
				MSGQ_NOTIFY_TO_RTP_SIZE_EACH_MSG, MSG_Q_FIFO, OM_CREATE,0);
		if(FWmsgQId == MSG_Q_ID_NULL)
		{
			kprintf("Failed to Open FW_UPGRD_MSGQ!!!\n");
			exit(-1);	
		}
		
		SD_ID FWSD = sdOpen("/FirmwareUpgradeBuff", 0, OM_CREATE, 512, 0, SD_ATTR_RWX | SD_CACHE_OFF, (void **) &FirmWareBuff);
		
		if(SD_ID_NULL == FWSD)
		{
			kprintf("/FirmwareUpgradeBuff!!!\n");
			exit(-1);		
		}
	#endif

	SD_ID RtSD = sdOpen("/RealTimeData", 0, OM_CREATE, 512, 0, SD_ATTR_RWX | SD_CACHE_OFF, (void **) &ptrsRealTimeData);
	if(SD_ID_NULL == RtSD)
	{
		printf("Failed to Create Shared Data!!!\n");
		exit(-1);
	}
	
	id = taskSpawn("ecatMainTask", 103, 0, 1000,  (FUNCPTR)ecatMainTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	i(id);	
	tidPDI = taskOpen((char *)"/PDIInterrupt", 101, NULL, OM_CREATE, NULL, 0x1000, NULL, (FUNCPTR)PDIHandler, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);	
}
