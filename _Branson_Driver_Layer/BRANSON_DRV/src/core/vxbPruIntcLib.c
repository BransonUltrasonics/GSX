/*
 * vxbPruIntcLib.c
 *
 *  Created on: Mar 24, 2017
 *      Author: emerson
 */



/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <vxbPruIntcLib.h>

/* locals */

LOCAL struct vxbPruIntcHandle pruintcHandle;


/*****************************************************************************
*
* pruintcRegister - register pru routine to VxBus PRU-INTC subsystem.
*
* This function register PRU routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void pruintcRegister
    (
    VXB_DEV_ID pDev,
    VXB_PRU_INTC_FUNC * intc,
	char    name[32]
    )
    {
    char    * intc0  = "pruicss0_intc";
    char    * intc1  = "pruicss1_intc";
	int       strLen    = strlen(name);
	if(strncmp(name, intc1, strLen) == 0)
	{
		kprintf("name %s intc1 %s\n",name,intc1);
		pruintcHandle.pInst = pDev;
	}
		
	if(strncmp(name, intc0, strLen) == 0)
	{
		kprintf("name %s intc0 %s\n",name,intc1);
		pruintcHandle.pInst0 = pDev;
	}
	
    pruintcHandle.pPruIntcFunc = intc;
    }


/*******************************************************************************
*
* vxbClearEvent - routine clear any event sspecified by eventNum.
*
* This function clear any event sspecified by eventNum.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbClearEvent
    (
    int eventNum
    )
    {
	return pruintcHandle.pPruIntcFunc->clearEvent(pruintcHandle.pInst, eventNum);
    }


/*******************************************************************************
*
* vxbSendEvent - routine send/generate any event specified by eventNum.
*
* This function send/generate any event specified by eventNum.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbSendEvent
    (
    int eventNum
    )
    {
	return pruintcHandle.pPruIntcFunc->sendEvent(pruintcHandle.pInst, eventNum);
    }


/*******************************************************************************
*
* vxbWaitEvent - wait for event
*
* This routine wait for any event.
*
* RETURNS: NULL.
*
* ERRNO: N/A.
*/

STATUS vxbWaitEvent
    (
    int eventNum
    )
    {
	return pruintcHandle.pPruIntcFunc->waitEvent(pruintcHandle.pInst, eventNum);
    }


/*******************************************************************************
*
* vxbSendWaitClearEvent - send, wait and then clear event
*
* This routine send, wait and then clear event.
*
* RETURNS: NULL.
*
* ERRNO: N/A.
*/
STATUS vxbSendWaitClearEvent
    (
    int sendEventNum, int pruEvtoutNum, int ackEventNum
    )
    {
	return pruintcHandle.pPruIntcFunc->sendWaitClearEvent(pruintcHandle.pInst, sendEventNum, pruEvtoutNum, ackEventNum);
    }


/*******************************************************************************
*
* vxbRegisterEvent - register event
*
* This routine registers event.
*
* RETURNS: NULL.
*
* ERRNO: N/A.
*/
void  vxbRegisterEvent(
       uint32_t pruEvtoutNum,
       int intrNum,
	   PRUICSSDRV_IRQ_HANDLER irqHandler)
{
	return pruintcHandle.pPruIntcFunc->registerEvent(
    	   pruintcHandle.pInst,
           pruEvtoutNum,
           intrNum,
		   irqHandler);
}

/*******************************************************************************
*
* vxbEventStatus - status of the event
*
* This routine displays the event registers.
*
* RETURNS: NULL.
*
* ERRNO: N/A.
*/
STATUS vxbEventStatus (void)
{
	return pruintcHandle.pPruIntcFunc->eventStatus(
    	   pruintcHandle.pInst);
}

/*******************************************************************************
*
* pruintcPDIRegister - Register PDI_ISR
*
* This routine register PDI_ISR
*
* RETURNS: NULL.
*
* ERRNO: N/A.
*/


void pruintcPDIRegister
    (
	PRU_PDI_IRQ_HANDLER pHandler
    )
    {
	return pruintcHandle.pPruIntcFunc->eventPdiRegister(
	    	   pruintcHandle.pInst,pHandler);
    }
