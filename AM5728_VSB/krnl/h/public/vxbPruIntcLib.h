/*
 * vxbPruIntcLib.h
 *
 *  Created on: Mar 24, 2017
 *      Author: emerson
 */


#ifndef __INCvxbPruIntcLibh
#define __INCvxbPruIntcLibh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>
#include <pruicss.h>

#define  MAX_ALARMS                2  /* The maximum number of the hardware alarm */

#define  PRU_INTC_ALARM_SEC_SUPPORT     0x01

/* alarm callback */

typedef STATUS (*PRU_INTC_ALARM_FUNC) (VOID *pArg); 

// Pointer to PDI ISR
typedef void (*PRU_PDI_IRQ_HANDLER)(void);
 
/* structure to be used and updated RTC time and Alarm */

typedef struct vxbPruIntcFunctionality
    {
    char   timerName[MAX_DRV_NAME_LEN + 1];
    UINT8  alarmNum;                  /* Alarm numbers */
    UINT8  alarmCap[MAX_ALARMS];      /* Alarm attribute */
    int (*waitEvent)(VXB_DEV_ID		pDev,uint32_t pruEvtoutNum );
    int32_t (*clearEvent)(VXB_DEV_ID		pDev,uint32_t eventnum);
    int32_t (*sendEvent)(VXB_DEV_ID		pDev,uint32_t eventnum);
    void (*registerEvent)(	VXB_DEV_ID pDev,
    		uint32_t pruEvtoutNum,
    	    int intrNum,
    		PRUICSSDRV_IRQ_HANDLER irqHandler);
    int (*sendWaitClearEvent)(  VXB_DEV_ID		pDev,
                                          uint32_t sendEventNum,
   										uint32_t pruEvtoutNum,
   										uint32_t ackEventNum
   										); 
    int32_t (*eventStatus)	(VXB_DEV_ID pDev);  
    void (*eventPdiRegister)   	(VXB_DEV_ID pDev,PRU_PDI_IRQ_HANDLER userHandler) ;
    } VXB_PRU_INTC_FUNC;

struct vxbPruIntcHandle
    {
    struct vxbPruIntcFunctionality *pPruIntcFunc;
    VXB_DEV_ID                  pInst;
    VXB_DEV_ID                  pInst0;
    };

/* IMPORT */

/*IMPORT STATUS vxbPruIntcGet (struct tm * rtcTime);
IMPORT STATUS vxbPruIntcSet (struct tm * rtcTime);
IMPORT STATUS sysPruIntcSet (void);
*/
/* 0: simple show; 1: full show */
//IMPORT STATUS sysPruIntcGet (int level);
IMPORT STATUS vxbClearEvent
    (
    int eventNum
    );
IMPORT STATUS vxbSendEvent
    (
    int eventNum
    );
IMPORT STATUS vxbWaitEvent
    (
    int eventNum
    );
IMPORT STATUS vxbSendWaitClearEvent
    (
    int sendEventNum, int pruEvtoutNum, int ackEventNum
    );
IMPORT STATUS vxbEventStatus
    (void);
IMPORT void  vxbRegisterEvent(
	       uint32_t pruEvtoutNum,
	       int intrNum,
		   PRUICSSDRV_IRQ_HANDLER irqHandler);

IMPORT void pruintcRegister
    (
    VXB_DEV_ID pDev,
    VXB_PRU_INTC_FUNC * rtc,
	char    name[32]
    );
    
IMPORT void pruintcPDIRegister
    (
    PRU_PDI_IRQ_HANDLER pHandler
    );
#ifdef __cplusplus
}
#endif

#endif /* _INCvxbPruIntcLibh */
