/*
 * PRU-subsystem Interrupt Controller driver for AM437x IDK
*	
 *	The driver configures:
 *	1. Mapping from EVENT to HOST
 *	2. Register Event ISR 
 *	3. Register Single Interrupt Handler which further register Event ISRs
 *	4. Define Event ISR, which are called when ARM-PRU event happens.
 */

/*
 * Copyright information - To be Done.
 *
 */

/* includes */
#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lstLib.h>
#include <iosLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <hwif/buslib/vxbI2cLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <time.h>
#include <private/timeP.h>
#include <subsys/int/vxbIntLib.h>
#include "vxbFdtTiPrussIntcEnd.h" /* Driver Header FIle */
#include "vxbFdtTiPrussIntcMapping.h" /* INTC Mapping*/
#include "vxbFdtTiHwIcssIntc.h"
#include "pruicss_v1.h"
#include <pruicss.h>
#include "pruicss_osal.h"
#include <vxbPruIntcLib.h>


typedef void (*PRU_PDI_IRQ_HANDLER)(void);
 //PRU_IRQ_HANDLER pIrqHandler;  
PRU_PDI_IRQ_HANDLER pPDIIrqHandler ;

typedef struct prussIntc
    {
    VXB_DEV_ID			pDev;
    void *				pRegbase;
    void *				pHandle;
    VXB_RESOURCE      * intRes[10];
    VXB_PRU_INTC_FUNC * intcFunc;
    SEM_ID				semHandle;
    }PRUSS_INTC_CTRL;
   
    
/*!
 *  @brief      PRUICSS system to channel map
 *
 */
 PRUICSS_IntcInitData pruss0_intc_initdata = PRU_ICSS0_INTC_INITDATA;

 PRUICSS_IntcInitData pruss1_intc_initdata = PRU_ICSS1_INTC_INITDATA;
    

 typedef struct hwi_params
 {
 	void * arg;
 	int32_t eventId;
 	BOOL enableInt ;
 	int maskSetting ;
 }Hwi_Params;

 typedef struct hwi_handle
 {
 	int32_t intNum;
 	void * funcptr;
 	Hwi_Params *hwiParams;
 }Hwi_Handle;

 
LOCAL PRUSS_INTC_CTRL * pPrussIntcCtrl = NULL;

LOCAL STATUS	prussIntcProbe			(VXB_DEV_ID);
LOCAL STATUS    prussIntcAttach     	(VXB_DEV_ID);
LOCAL STATUS 	prussIntcInit			(VXB_DEV_ID pDev,
										PRUICSS_IntcInitData *prussintc_init_data);
LOCAL STATUS 	prussIntcIrqHandler		(VXB_DEV_ID);
LOCAL STATUS	prussIntcSendEvent	(VXB_DEV_ID pDev,uint32_t eventnum);
int32_t 		prussIntcClearEvent	(VXB_DEV_ID pDev,uint32_t eventnum);
int 			prussIntcWaitEvent	(VXB_DEV_ID		pDev,uint32_t pruEvtoutNum );
LOCAL STATUS 	prussIntcHwiIntHandler	(void *arg);
int32_t 		prussIntcEventStatus		(VXB_DEV_ID pDev);
int 			prussIntcRegisterEvent				(VXB_DEV_ID pDev, uint32_t pruEvtoutNum, int intrNum, 
										PRUICSSDRV_IRQ_HANDLER irqHandler);
LOCAL STATUS 	prussIntcEcatISR					(void* args);
LOCAL STATUS 	prussIntcEscCmdLowAckISR			( void *	args) ;
LOCAL STATUS 	prussIntcSync0ISR				( void * args) ;
LOCAL STATUS 	prussIntcSync1ISR				( void * args) ;
static void 	prussIntcSetCmr		(VXB_DEV_ID		pDev,
										uint8_t sysevt,
										uint8_t channel,
										uint8_t polarity,
										uint8_t type);
static void	 	prussIntcSetHmr		(VXB_DEV_ID		pDev,
										uint8_t channel,
										uint8_t host);
int 	prussIntcRegisterIrqHandler		(VXB_DEV_ID	pDev,
										PRUICSS_IrqArgs* irqArgs,
										int pruEvtoutNum,
										int intrNum,
										int eventNum,
										uint8_t waitEnable,
										PRUICSSDRV_IRQ_HANDLER irqHandler);
int		prussIntcSendWaitClearEvent	(VXB_DEV_ID		pDev,
										uint32_t sendEventNum,
										uint32_t pruEvtoutNum,
										uint32_t ackEventNum);
LOCAL STATUS prussIntcStringLookup		(int     offset,
										char        * propName,
										char        * string);

LOCAL VOID pruRegister 	(VXB_DEV_ID pDev,PRU_PDI_IRQ_HANDLER userHandler) ;

/* driver utility functions */
LOCAL VXB_DRV_METHOD prussIntcMethods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),     (FUNCPTR)prussIntcProbe		},
    { VXB_DEVMETHOD_CALL(vxbDevAttach),    (FUNCPTR)prussIntcAttach		},
    { 0, 0 }
};

VXB_DRV  prussIntcDrv =
    {
    { NULL } ,
    "pruicss_intc",			/* drvName */
    "PRU Sub-system INTC",	/* Description */
    VXB_BUSID_FDT,      	/* Class */
    0,                  	/* Flags */
    0,                  	/* Reference count */
	prussIntcMethods        	/* Method table */
    };
VXB_DRV_DEF(prussIntcDrv)

/* struct used for matching a device */
LOCAL VXB_FDT_DEV_MATCH_ENTRY prussIntcMatch[] =
    {
        {
        "ti,am437x-prussIntc",				/* compatible */
        NULL,							/* no configuration */
        },
        {} 								/* Empty terminated list */
    };

/******************************************************************************
*
* prussIntcProbe - probe for device presence at specific address
*
* This routine probes for device presence at specific address.
*
* RETURNS: OK if probe passes.  ERROR otherwise.
*
* ERRNO: N/A
*/


LOCAL STATUS prussIntcProbe
    (
    VXB_DEV_ID pDev
    )
    {
    return vxbFdtDevMatch (pDev, prussIntcMatch, NULL);
    }


/******************************************************************************
*
* pruIntcAttach - attach PRU INTC device
*
* This is the PRU INTC initialization routine.
*
* RETURNS: OK or ERROR when initialize failed
*
* ERRNO: N/A
*/

LOCAL STATUS prussIntcAttach
    (
    VXB_DEV_ID          pDev
    )
    {
	VXB_FDT_DEV      * pFdtDev 	= NULL;
    VXB_RESOURCE_ADR * pResAdr 	= NULL;
    VXB_RESOURCE     * pRes		= NULL;
	VXB_PRU_INTC_FUNC* pIntcFunc = NULL;
    VXB_RESOURCE_IRQ * pResIrq 	= NULL;
    VXB_RESOURCE     * pIRes		= NULL;
	PRUICSS_V1_Object* object;
	PRUICSS_Handle 	   handle;
	PRUICSS_IrqArgs  * irqArgs;
	int 			   ret, result, len, i, strLen;
	uint32_t pruIsrNum = 0;
	uint32_t evtOutNum = 0;
	char    * intc0  = "pruicss0_intc";
	char    * intc1  = "pruicss1_intc";
		
	pPrussIntcCtrl = vxbMemAlloc (sizeof (PRUSS_INTC_CTRL));
    
	if (pPrussIntcCtrl == NULL)
        {
        PRUINTC_DBG_MSG (INTC_DBG_ERR,
        		"driver structure vxbMemAlloc failed\n");
        return ERROR;
        }

	pFdtDev = vxbFdtDevGet (pDev);
	
	if (pFdtDev == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR," pFdtDevGet failed\n");
		vxbMemFree (pPrussIntcCtrl);
		return ERROR;
		}
	
	strLen    = strlen(pFdtDev->name);
	
	/* create controller driver context structure for core */
	
	result = vxFdtPropGet(pFdtDev->offset, "reg", &len);
    
    pRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 0);
    if(pRes == NULL)
    	{
    	PRUINTC_DBG_MSG (INTC_DBG_ERR,"Register Memory allocation failed\n");
    	goto FAIL;
    	}
        
    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;   
    if (pResAdr == NULL)
    	{
    	PRUINTC_DBG_MSG (INTC_DBG_ERR,"Register Resource Address NULL\n");
    	goto FAIL;
    	}
        
    pPrussIntcCtrl->pHandle = pResAdr->pHandle;
    pPrussIntcCtrl->pRegbase= (void *)pResAdr->virtual;
    
    /* save pDrvCtrl in VXB_DEVICE structure */
    
    vxbDevSoftcSet (pDev, pPrussIntcCtrl);
   
    /* Initialize the Interrupt Controller */
    
    prussIntcInit(pDev,&pruss1_intc_initdata);
    
    /* initialize the INTC Library */
    
    pIntcFunc = (VXB_PRU_INTC_FUNC *)vxbMemAlloc(sizeof(VXB_PRU_INTC_FUNC));
	if (pIntcFunc == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,
				"library structure memory allocation failed\n");
		goto FAIL;
		}

	pIntcFunc->clearEvent 			= prussIntcClearEvent;
	pIntcFunc->sendEvent 			= prussIntcSendEvent;
	pIntcFunc->waitEvent 			= prussIntcWaitEvent;
	pIntcFunc->registerEvent		= prussIntcRegisterEvent;
	pIntcFunc->sendWaitClearEvent 	= prussIntcSendWaitClearEvent;
	pIntcFunc->eventStatus			= prussIntcEventStatus;
	pIntcFunc->eventPdiRegister		= pruRegister;
	pruintcRegister(pDev, pIntcFunc,pFdtDev->name);
		
	if(strncmp(pFdtDev->name, intc1, strLen) == 0)
		{
		PRUINTC_DBG_MSG (INTC_DBG_INFO,
				"dts interrupt node name %s string name %s\n",
				pFdtDev->name,intc1);

		object = vxbMemAlloc (sizeof (PRUICSS_V1_Object));
		if(object == NULL)
		{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,
							"object memory allocation failed\n");
			goto FAIL;
		}
		
		handle = vxbMemAlloc (sizeof (PRUICSS_Config));
		if(handle == NULL)
		{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,
									"handle memory allocation failed\n");
			goto FAIL;
		}
		
		object->pDev = pDev;
		handle->object = object;
		handle->hwAttrs = pPrussIntcCtrl;
		irqArgs = vxbMemAlloc (sizeof (PRUICSS_IrqArgs));
		if(irqArgs == NULL)
		{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,
									"irqArgs memory allocation failed\n");
			goto FAIL;
		}
		
		irqArgs->handle = handle;
		pruIsrNum = 23 + 32;
		evtOutNum = 23 - 20;
		

		
		prussIntcRegisterIrqHandler(pDev,
			irqArgs,
			HOST2,//host2
			HOST2_ARM_INT,
			1,
			1,
			(PRUICSSDRV_IRQ_HANDLER)&prussIntcEcatISR);
		
		
		object = vxbMemAlloc (sizeof (PRUICSS_V1_Object));
		if(object == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
									"object memory allocation failed\n");
					goto FAIL;
				}
		
		handle = vxbMemAlloc (sizeof (PRUICSS_Config));
		if(handle == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
											"handle memory allocation failed\n");
					goto FAIL;
				}
		
		object->pDev = pDev;
		handle->object = object;
		handle->hwAttrs = pPrussIntcCtrl;
		irqArgs = vxbMemAlloc (sizeof (PRUICSS_IrqArgs));
		if(irqArgs == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
											"irqArgs memory allocation failed\n");
					goto FAIL;
				}
		
		irqArgs->handle = handle;
		prussIntcRegisterIrqHandler(pDev,
			irqArgs,
			HOST3,//host3
			HOST3_ARM_INT,
			1,
			1,
			(PRUICSSDRV_IRQ_HANDLER)&prussIntcSync0ISR);

		
		object = vxbMemAlloc (sizeof (PRUICSS_V1_Object));
		if(object == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
									"object memory allocation failed\n");
					goto FAIL;
				}
		
		handle = vxbMemAlloc (sizeof (PRUICSS_Config));
		if(handle == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
											"handle memory allocation failed\n");
					goto FAIL;
				}
		
		object->pDev = pDev;
		handle->object = object;
		handle->hwAttrs = pPrussIntcCtrl;
		irqArgs = vxbMemAlloc (sizeof (PRUICSS_IrqArgs));
		if(irqArgs == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
											"irqArgs memory allocation failed\n");
					goto FAIL;
				}
		
		irqArgs->handle = handle;
		prussIntcRegisterIrqHandler(pDev,
			irqArgs,
			HOST4,//host4
			HOST4_ARM_INT,
			1,
			1,
			(PRUICSSDRV_IRQ_HANDLER)&prussIntcSync1ISR);
		
		object = vxbMemAlloc (sizeof (PRUICSS_V1_Object));
		if(object == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
									"object memory allocation failed\n");
					goto FAIL;
				}
		
		handle = vxbMemAlloc (sizeof (PRUICSS_Config));
		if(handle == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
											"handle memory allocation failed\n");
					goto FAIL;
				}
		
		object->pDev = pDev;
		handle->object = object;
		handle->hwAttrs = pPrussIntcCtrl;
		irqArgs = vxbMemAlloc (sizeof (PRUICSS_IrqArgs));
		if(irqArgs == NULL)
				{
					PRUINTC_DBG_MSG (INTC_DBG_ERR,
											"irqArgs memory allocation failed\n");
					goto FAIL;
				}
		
		irqArgs->handle = handle;
		prussIntcRegisterIrqHandler(pDev,
			irqArgs,
			HOST5,//host6
			HOST5_ARM_INT,
			1,
			1,
			(PRUICSSDRV_IRQ_HANDLER)&prussIntcEscCmdLowAckISR);
		
		}

#if INTC_EVENT_DEBUG
	prussIntcSendEvent(pDev,56);//host 2->i = 0 
	prussIntcSendEvent(pDev,20);//host 6->i = 4
	prussIntcSendEvent(pDev,14);//host 3->i = 1
	prussIntcSendEvent(pDev,13);//host4 ->i = 2
#endif
	
    return OK;
        
FAIL:

	if (pRes != NULL)
		{
		(void) vxbResourceFree (pDev, pRes);
		}
	if (pPrussIntcCtrl != NULL || pIntcFunc != NULL)
		{
		vxbMemFree (pIntcFunc);
		vxbMemFree (pPrussIntcCtrl);
		vxbDevSoftcSet (pDev, NULL);
		}
	return ERROR;
	}

/******************************************************************************
 *
 * pruicssIntcInit : this function initializes INTC.
 * 
 * This function sets :
 * 	1.the polarity to active high for all system interrupts
 * 	2.the type to pulse interrupt for all system interrupts
 * 	3.clear all 16 interrupt channel map registers
 * 	4.map event to channel as per the prussintc_init_data parameter
 * 	5.clear all 3 host interrupt map registers
 * 	6.map channel to host as per the prussintc_init_data parameter
 * 	
 */
LOCAL STATUS prussIntcInit
	(
	VXB_DEV_ID pDev,
	PRUICSS_IntcInitData * prussintc_init_data
	)
	{
	int i;
	UINT32 val;
	int mask1,mask2;
	mask1 = mask2 = 0;
	
	/* configure the polarity to active high for all system interrupts */
	CSR_WRITE_4(pDev, PRU_INTC_SIPR0, 0xffffffff);
	CSR_WRITE_4(pDev, PRU_INTC_SIPR1, 0xffffffff);
	
	/* configure the type to pulse interrupt for all system interrupts */
	CSR_WRITE_4(pDev, PRU_INTC_SITR0, 0xffffffff);
	CSR_WRITE_4(pDev, PRU_INTC_SITR1, 0xffffffff);
	
	/* clear all 16 interrupt channel map registers */
	for (i = 0; i < (PRUICSS_NUM_PRU_SYS_EVTS + 3) >> 2; i++)
		CSR_WRITE_4(pDev,HW_ICSS_INTC_CMR0  + (i << 2),0);

	for (i = 0;
		((prussintc_init_data->sysevt_to_channel_map[i].sysevt != 0xFF)
		  && (prussintc_init_data->sysevt_to_channel_map[i].channel != 0xFF));
		i++)
		{
		prussIntcSetCmr(pDev,prussintc_init_data->sysevt_to_channel_map[i].sysevt,
				 prussintc_init_data->sysevt_to_channel_map[i].channel,
				 (~prussintc_init_data->sysevt_to_channel_map[i].polarity)&0x01,
				 (~prussintc_init_data->sysevt_to_channel_map[i].type)&0x01
				  );
		}
	
	/* clear all 3 host interrupt map registers */
	for (i = 0; i < (PRUICSS_NUM_PRU_HOSTS + 3) >> 2; i++)
		CSR_WRITE_4(pDev, HW_ICSS_INTC_HMR0 + (i << 2), 0);

	for (i = 0;
		 ((prussintc_init_data->channel_to_host_map[i].channel != 0xFF)
		  && (prussintc_init_data->channel_to_host_map[i].host != 0xFF));
		 i++)
		{
		prussIntcSetHmr(pDev,prussintc_init_data->channel_to_host_map[i].channel,
							prussintc_init_data->channel_to_host_map[i].host);
		}
	
	for (i = 0; (uint8_t)prussintc_init_data->sysevts_enabled[i] != 0xFF; i++)
	{
		if (prussintc_init_data->sysevts_enabled[i] < 32)
		{
			mask1 = mask1 + (1 << (prussintc_init_data->sysevts_enabled[i]));
		}
		else if (prussintc_init_data->sysevts_enabled[i] < 64)
		{
			mask2 = mask2 + (1 << (prussintc_init_data->sysevts_enabled[i] - 32));
		}
		else
		{
			/* Error */
			return PRUICSS_RETURN_FAILURE;
		}
	}

		/* Enable 0-31 Events */
		val = 0;
		val = CSR_READ_4(pDev, HW_ICSS_INTC_ESR0);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_ESR0:0x%08x\n",val);
		val |= HW_ICSS_INTC_ESR0_ENABLE_SET_31_0_MASK;
		val |= (mask1 << HW_ICSS_INTC_ESR0_ENABLE_SET_31_0_SHIFT);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"Value to write:0x%08x\n",val);
		val = 0xffffffff;
		CSR_WRITE_4(pDev, HW_ICSS_INTC_ESR0,val);
		val = CSR_READ_4(pDev, HW_ICSS_INTC_ESR0);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_ESR0 After:0x%08x\n",val);
				
		/* Clear 0-31 Events */
		val =  0;
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR0);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_SECR0:0x%08x\n",val);
		val |= HW_ICSS_INTC_SECR0_ENA_STATUS_31_0_MASK;
		val |= (mask1 << HW_ICSS_INTC_SECR0_ENA_STATUS_31_0_SHIFT);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"Value to write:0x%08x\n",val);
		val = 0xffffffff;
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SECR0,val);
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR0);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_SECR0 After:0x%08x\n",val);
		
		/* Enable 32-63 Events */
		val =  0;
		val = CSR_READ_4(pDev, HW_ICSS_INTC_ERS1);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_ERS1:0x%08x\n",val);
		val |= HW_ICSS_INTC_ERS1_ENABLE_SET_63_32_MASK;
		val |= (mask2 << HW_ICSS_INTC_ERS1_ENABLE_SET_63_32_SHIFT);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"Value to write:0x%08x\n",val);
		val = 0xffffffff;
		CSR_WRITE_4(pDev, HW_ICSS_INTC_ERS1,val);
		val = CSR_READ_4(pDev, HW_ICSS_INTC_ERS1);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_ERS1 After:0x%08x\n",val);

		/* Enable 32-63 Events */
		val =  0;
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR1);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_SECR1:0x%08x\n",val);
		val |= HW_ICSS_INTC_SECR1_ENA_STATUS_63_32_MASK;
		val |= (mask2 << HW_ICSS_INTC_SECR1_ENA_STATUS_63_32_SHIFT);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"Value to write:0x%08x\n",val);
		val = 0xffffffff;
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SECR1,val);
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR1);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_SECR1 After:0x%08x\n",val);
		
		/* Enable HOST Events */
		val =  0;
		val = CSR_READ_4(pDev, HW_ICSS_INTC_HIER);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_HIER:0x%08x\n",val);
		val |= HW_ICSS_INTC_HIER_ENABLE_HINT_MASK;
		val |= 
			(prussintc_init_data->host_enable_bitmask << HW_ICSS_INTC_HIER_ENABLE_HINT_SHIFT);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"Value to write:0x%08x\n",val);
		val = 0x3ff;
		CSR_WRITE_4(pDev, HW_ICSS_INTC_HIER,val);
		val = CSR_READ_4(pDev, HW_ICSS_INTC_HIER);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_HIER After:0x%08x\n",val);
		
		/* Enable Global Events */
		val =  0;
		val = CSR_READ_4(pDev, HW_ICSS_INTC_GER);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_GER:0x%08x\n",val);
		val |= HW_ICSS_INTC_GER_ENABLE_HINT_ANY_MASK;
		val |= (0x1 << HW_ICSS_INTC_GER_ENABLE_HINT_ANY_SHIFT);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"Value to write:0x%08x\n",val);
		val = 0x1;
		CSR_WRITE_4(pDev, HW_ICSS_INTC_GER,val);
		val = CSR_READ_4(pDev, HW_ICSS_INTC_GER);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"HW_ICSS_INTC_GER After:0x%08x\n",val);

	return OK;
	}
 
/******************************************************************************
 *
 * pruicssIntcSetCmr: channel map register
 * 
 * This function maps event to channel
 */

static void prussIntcSetCmr
	(
	VXB_DEV_ID			pDev,
	uint8_t sysevt,
	uint8_t channel,
	uint8_t polarity,
	uint8_t type
	)
	{
	UINT32 val;
	val = CSR_READ_4(pDev,(HW_ICSS_INTC_CMR0 + (sysevt & ~(0x3))));
	val |= ((channel & 0xF) << ((sysevt & 0x3) << 3));
	CSR_WRITE_4(pDev, (HW_ICSS_INTC_CMR0 + (sysevt & ~(0x3))) , val);

    if(sysevt < 32)
    	{
    	/* SET POLARITY */
    	val = CSR_READ_4(pDev,HW_ICSS_INTC_SIPR0);
		val &= ~(polarity << sysevt);
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SIPR0 , val);
		
		/* SET TYPE */
		val = CSR_READ_4(pDev,HW_ICSS_INTC_SITR0);
		val &= ~(type << sysevt);
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SIPR0 , val);
    	}
    else
    	{
    	/* SET POLARITY */
    	val = CSR_READ_4(pDev,HW_ICSS_INTC_SIPR1);
		val &= ~(polarity << (sysevt - 32));
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SIPR0 , val);
		
		/* SET TYPE */
		val = CSR_READ_4(pDev,HW_ICSS_INTC_SITR1);
		val &= ~(type << (sysevt - 32));
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SIPR0 , val); 
    	}
	}


/******************************************************************************
 *
 * pruicssIntcSetHmr: Host map register
 * 
 * This function maps channel to host
 */
static void prussIntcSetHmr
	(
	VXB_DEV_ID			pDev,
	uint8_t channel,
    uint8_t host
	)
	{
	CSR_WRITE_4(pDev, (HW_ICSS_INTC_HMR0 + (channel & ~(0x3))) ,
			(CSR_READ_4(pDev, (HW_ICSS_INTC_HMR0 + (channel & ~(0x3))))
                         | (((host) & 0xF) << (((channel) & 0x3) << 3))));
	}


/******************************************************************************
 *
 *  prussIntcRegisterEvent : Wrapper function for Event registration
 * 
 * This function gets the pruEvtNum, intrNum and ISR as parameter,
 * from the user space.It stores the pDev instance, pDrvCtrl instance in the 
 * irqArgs structure and call register Function.
 */

int prussIntcRegisterEvent
	(
	VXB_DEV_ID pDev,
	uint32_t pruEvtoutNum,
    int intrNum,
	PRUICSSDRV_IRQ_HANDLER irqHandler
	)
	{
	PRUICSS_V1_Object       * object;
	PRUICSS_Handle 			  handle;
	PRUICSS_IrqArgs 		* irqArgs;
	PRUSS_INTC_CTRL 		* pDrvCtrl = NULL;
	int 					  ret;
	
	pDrvCtrl = (PRUSS_INTC_CTRL *)vxbDevSoftcGet(pDev);
	
	if (pDrvCtrl == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"driver structure instance NULL\n");
		return ERROR;
		}

	object = vxbMemAlloc (sizeof (PRUICSS_V1_Object));
    if (object == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Object NULL\n");
		return ERROR;
		}
    
	handle = vxbMemAlloc (sizeof (PRUICSS_Config));
	if (handle == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Handle NULL\n");
		return ERROR;
		}
	
	irqArgs = vxbMemAlloc (sizeof (PRUICSS_IrqArgs));
	if (irqArgs == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Argument structure NULL\n");
		return ERROR;
		}
	
	object->pDev = pDrvCtrl->pDev;
	handle->object = object;
	handle->hwAttrs = pDrvCtrl;
	irqArgs->handle = handle;
	
	ret = prussIntcRegisterIrqHandler(pDev,
							   irqArgs,
	                           pruEvtoutNum,
			                   intrNum,
			                   1,
							   1,
							   irqHandler
							   );
	return ret;
	}


/******************************************************************************
 *
 * prussIntcRegisterIrqHandler: Registers an Interrupt Handler for an event
 * 
 * This function first maps the pruFunMap[pruEventNum] from the irqargs->object to 
 * the irqargs->pruFunMap and pass irqarg as an arg parameter to the vxbIntConnect.
 * So, that when interrupt occurs the ISR will get irqargs as parameter and run the
 * mapped pruFunMap->irqHandler which is "PRUICSSDRV_IRQ_HANDLER irqHandler".
 * 
 * RETURNS: 
 * 		PRUICSS_RETURN_SUCCESS
 *
 * ERRNO: N/A
 **/

int prussIntcRegisterIrqHandler
	(
	VXB_DEV_ID	pDev,
	PRUICSS_IrqArgs* irqArgs,
    int pruEvtoutNum,
	int intrNum,
	int eventNum,
	uint8_t waitEnable,
	PRUICSSDRV_IRQ_HANDLER irqHandler
	)
	{
	PRUICSS_Handle 		handle;
	PRUICSS_V1_Object*  object;
	PRUSS_INTC_CTRL  *  pDrvCtrl = NULL;
	VXB_RESOURCE_IRQ *  pResIrq 	= NULL;
    VXB_RESOURCE     *  pRes		= NULL;
    VXB_RESOURCE     *  pIRes		= NULL;
    VXB_FDT_DEV      *  pFdtDev 	= NULL;
    VXB_DEV_ID 			pDev_t;
    UINT32 				val;
    int                 regIndex     = 0xFF;
    static int 			i_e;
    char 			 *  (*ptr)[7] = &interrupt_host;
    void    		 *  hwiHandle;
	void			 *	semHandle = NULL;

	pDrvCtrl = (PRUSS_INTC_CTRL *)vxbDevSoftcGet(pDev);
	
	if (pDrvCtrl == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"driver structure instance NULL\n");
		return ERROR;
		}
	
	if (irqArgs == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Argument structure NULL\n");
		return ERROR;
		}
	
	handle = irqArgs->handle;
	if (handle == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Handle NULL\n");
		return ERROR;
		}
	
    object = handle->object;
    if (object == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Object NULL\n");
		return ERROR;
		}
    
	irqArgs->pruFunMap = &(object->pruEvntOutFnMapArray[pruEvtoutNum]);
	
	if (irqArgs->pruFunMap == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Function Mapping NULL\n");
		return ERROR;
		}
	
	if ((pruEvtoutNum > PRUICSS_MAX_WAIT_EVENTS) || (irqHandler == NULL))
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"PRU Event Number Out of Range\n");
    	return PRUICSS_RETURN_FAILURE;
		}
    
    /* create a Semaphore Instance */
	if(waitEnable)
	{
		pDrvCtrl->semHandle = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);

		if(NULL == pDrvCtrl->semHandle)
			return PRUICSS_RETURN_FAILURE;
	}
	
    /* connect and enable interrupt */
	
    pFdtDev = vxbFdtDevGet(pDev);
    
    if (pFdtDev == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"FileDeviceTree Object NULL\n");
		return ERROR;
		}
	
	pDev_t = object->pDev;
	
	if (pDev_t == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
		return ERROR;
		}
	
	PRUINTC_DBG_MSG (INTC_DBG_INFO,"Driver ID %d %x \n",pDev_t,pDev_t);
	PRUINTC_DBG_MSG (INTC_DBG_INFO,"\n\nEvent NUmber %d\n",pruEvtoutNum);
	
	pIRes = vxbResourceAlloc (pDev, VXB_RES_IRQ, pruEvtoutNum);
	
	if (pIRes == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"vxbResourceAlloc failed for ISR\n");
		(void) vxbResourceFree (pDev, pIRes);
		return ERROR;
		}
	
	pResIrq = (VXB_RESOURCE_IRQ *)pIRes->pRes;
	
	if (pResIrq == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Resource NULL\n");
		return ERROR;
		}
		
	PRUINTC_DBG_MSG (INTC_DBG_INFO,
			"Resource Allocation for Hardware vector %d %x "\
			"and Logical Vector %d %x .SUCCESS!!\n",
			pResIrq->hVec, pResIrq->hVec, pResIrq->lVec, pResIrq->lVec);            

	/*
	 * save the interrupt resource which is used to
	 * enable/disconnect/disable for other functions
	 */

	pPrussIntcCtrl->intRes[pruEvtoutNum] = pIRes;
	irqArgs->host = pruEvtoutNum;
	if (vxbIntConnect (pDev, pIRes, (VOIDFUNCPTR)prussIntcHwiIntHandler, irqArgs) == ERROR)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,""
				"Local ISR connect failed for interrupt:%d\n",pResIrq->hVec);
		return ERROR;
		}
	else
		{
		if (vxbIntEnable (pDev, pIRes) == ERROR)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,
					"interrupt enable failed %d\n",pResIrq->hVec);
			(void) vxbIntDisconnect (pDev, pIRes);
			return ERROR;
			}
		}

	object->pruEvntOutFnMapArray[pruEvtoutNum].semHandle = semHandle;
    object->pruEvntOutFnMapArray[pruEvtoutNum].irqHandler = irqHandler;
    
	return PRUICSS_RETURN_SUCCESS;
	}


/******************************************************************************
 *
 * pruicssIrqHandler: INTC ISR routine handler
 * 
 * This function task is to reroute the ARM attention to the event handler.
 */
LOCAL STATUS prussIntcHwiIntHandler(void *arg)
	{
	PRUICSS_IrqArgs   *	irqArgs = arg;
	PRUICSS_Handle 		handle;
	PRUICSS_IrqFunMap *	pruFunMap;
	PRUICSS_V1_Object * object;
	VXB_DEV_ID 			pDev_t;
	int 				host;

	if (irqArgs == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Argument structure NULL\n");
		return ERROR;
		}
	
	handle = irqArgs->handle;
	if (handle == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Handle NULL\n");
		return ERROR;
		}
	
	object = handle->object;
    if (object == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Object NULL\n");
		return ERROR;
		}
    
	pDev_t = object->pDev;
	if (pDev_t == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
		return ERROR;
		}
	
	host = irqArgs->host;
	
	pruFunMap = irqArgs->pruFunMap;
    if(pruFunMap != NULL)
    	{
    	//pruFunMap->irqHandler((void*)handle);
    	pruFunMap->irqHandler((void*)arg);
    	//if(pruFunMap->semHandle != NULL)
    		//semTake(pruFunMap->semHandle);
    	}
    else
    	{
    	PRUINTC_DBG_MSG (INTC_DBG_ERR,"Function Mapping NULL\n");
    	return ERROR;
    	}
    
    return PRUICSS_RETURN_SUCCESS;
	}


/******************************************************************************
 *
 * prussIntcClearEvent: Generates INTC event
 * 
 * This function Generates an INTC event
 * 
 * RETURNS: 
 * 		PRUICSS_RETURN_SUCCESS
 *
 * ERRNO: N/A
 **/

LOCAL STATUS prussIntcSendEvent
	(
	VXB_DEV_ID pDev,
	uint32_t eventnum
	)
	{
	PRUICSS_HwAttrs  const    *hwAttrs;
	PRUSS_INTC_CTRL 		  *pDrvCtrl = NULL;
    uint32_t 				   baseaddr;
    int 					   val;
    
    if (pDev == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
		return ERROR;
		}
	
	pDrvCtrl = (PRUSS_INTC_CTRL *)vxbDevSoftcGet(pDev);
	if (pDrvCtrl == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Structure NULL\n");
		return ERROR;
		}
	
	prussIntcEventStatus(pDev);
	
	PRUINTC_DBG_MSG (INTC_DBG_INFO,"========= SENDING EVENT ===========\n"); 
	
	if(eventnum < 32)
		{
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SRSR0);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"ERS: Enable SysEvt for trigger "
				"0x%08x\n",val);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"eventnum %d %x\n",eventnum,eventnum);
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SRSR0,val | (1 << eventnum));
		}

	else
		{
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"eventnum %d %x\n",eventnum);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"(eventnum - 32) %d %x\n",
				(eventnum - 32),(eventnum - 32));
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SRSR1);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"ERS: Enable SysEvt %d "
				"for trigger  0x%08x\n",eventnum,val);
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SRSR1,val | (1 << (eventnum - 32)));
		}
		
	/* Check Status */
	prussIntcEventStatus(pDev);
		
	return  PRUICSS_RETURN_SUCCESS;
	}


/******************************************************************************
 *
 * prussIntcClearEvent: clears INTC event
 * 
 * This function clears an INTC event
 * 
 * RETURNS: 
 * 		PRUICSS_RETURN_SUCCESS
 *
 * ERRNO: N/A
 **/

int32_t prussIntcClearEvent
	(
	VXB_DEV_ID pDev,
	uint32_t eventnum
	)
	{
	PRUICSS_HwAttrs const    *hwAttrs;
	PRUSS_INTC_CTRL 	     *pDrvCtrl = NULL;
	PRUICSS_V1_Object        *object;
	PRUICSS_Handle 			  handle;
	int 					  ret, val, i;
	uint32_t 				  baseaddr;
	
    if (pDev == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
		return ERROR;
		}
	
	pDrvCtrl = (PRUSS_INTC_CTRL *)vxbDevSoftcGet(pDev);
	if (pDrvCtrl == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Structure NULL\n");
		return ERROR;
		}
	
	prussIntcEventStatus(pDev);
	
	PRUINTC_DBG_MSG (INTC_DBG_INFO,"========= CLEARING EVENT ===========\n");
	/* Clear Interrupt using SECR*/
	if(eventnum < 32)
		{
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR0);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"SECR0: Shows/Clears Pending Interrupt "
				"SysEvt if triggered  0x%08x\n",val);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"eventnum %d %x\n",eventnum,eventnum);
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SECR0,val & ~(1 << eventnum));
		}

	else
		{
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"eventnum %d %x\n",eventnum);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"(eventnum - 32) %d %x\n",
				(eventnum - 32),(eventnum - 32));
		val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR1);
		PRUINTC_DBG_MSG (INTC_DBG_INFO,"SECR1: Shows/Clears Pending Interrupt "
				"SysEvt if triggered  0x%08x\n",val);
		CSR_WRITE_4(pDev, HW_ICSS_INTC_SECR1,val & ~(1 << (eventnum - 32)));
		}
	
	/* Clear Interrupt using SICR */
	val = CSR_READ_4(pDev, HW_ICSS_INTC_SICR);
	CSR_WRITE_4(pDev, HW_ICSS_INTC_SICR,eventnum);
	val = CSR_READ_4(pDev, HW_ICSS_INTC_SICR);
	PRUINTC_DBG_MSG (INTC_DBG_INFO,"The Clear register 0x%08x \n",val);
	
	prussIntcEventStatus(pDev);

	
	return PRUICSS_RETURN_SUCCESS;

	}


/******************************************************************************
 *
 * @brief  This function waits for a System event to happen \n
 *
 * @param   handle     Pruss's driver handle
 * @param   pruEvtoutNum   The AINTC Event number.\n
 *
 * @return  0 in case of successful transition, -1 otherwise. \n
 */
int prussIntcWaitEvent(VXB_DEV_ID pDev,uint32_t pruEvtoutNum )
{


    if(pruEvtoutNum >= PRUICSS_MAX_WAIT_EVENTS)
    	return PRUICSS_RETURN_FAILURE;
#if 0
    if(object->pruEvntOutFnMapArray[pruEvtoutNum].semHandle != NULL)
    	return  PRUICSS_RETURN_SUCCESS;
#endif
    
	return  PRUICSS_RETURN_SUCCESS;
}


/******************************************************************************
 *
 * @brief  This function generates and INTC event, waits for AINTC event and
 *		   clears an INTC event \n
 *
 * @param 	handle         Pruss's driver handle
 * @param   sendEventNum   Event number.\n
 * @param   pruEvtoutNum   PRU Event number.\n
 * @param   ackEventNum    Acknowlodegement event number.\n
 *
 * \return  0 in case of successful transition, -1 otherwise. \n
 */
 int prussIntcSendWaitClearEvent
 	 ( 
 	 VXB_DEV_ID pDev,
	 uint32_t sendEventNum,
	 uint32_t pruEvtoutNum,
	 uint32_t ackEventNum
	)
 	{	
	prussIntcSendEvent(pDev,sendEventNum);
	prussIntcWaitEvent(pDev,pruEvtoutNum);
	prussIntcClearEvent(pDev,ackEventNum);
	return PRUICSS_RETURN_SUCCESS;
 	}

/******************************************************************************
 *
 * prussIntcEventStatus: INTC Status routine
 * 
 * This function task is to show the status of events
 * 
 * RETURNS: 
 * 		PRUICSS_RETURN_SUCCESS
 *
 * ERRNO: N/A
 **/
 
 int32_t prussIntcEventStatus
 	(
 	VXB_DEV_ID pDev
 	)
 	{
 	PRUICSS_HwAttrs  const    * hwAttrs;
 	PRUSS_INTC_CTRL           * pDrvCtrl = NULL;
 	PRUICSS_V1_Object         * object;
 	PRUICSS_Handle 			    handle;
 	uint32_t 					baseaddr;
 	int 						ret, val, i;
 	 	
	if (pDev == NULL)
		{
		PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
		return ERROR;
		}
 	
	PRUINTC_DBG_MSG (INTC_DBG_INFO,
			"========= CHECKING EVENT STATUS ===========\n");
 	/* Check Status */
 	val = CSR_READ_4(pDev, HW_ICSS_INTC_SICR);
 	PRUINTC_DBG_MSG (INTC_DBG_INFO,"Clear register 0x%08x \n",val);
 	val = CSR_READ_4(pDev, HW_ICSS_INTC_SRSR0);
 	PRUINTC_DBG_MSG (INTC_DBG_INFO,
 			"ERS0: Enable SysEvt  for trigger  0x%08x\n",val);
 	val = CSR_READ_4(pDev, HW_ICSS_INTC_SRSR1);
 	PRUINTC_DBG_MSG (INTC_DBG_INFO,"ERS1: Enable SysEvt  for trigger "
 			"0x%08x\n",val);
 	val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR0);
 	PRUINTC_DBG_MSG (INTC_DBG_INFO,"SECR0: Shows/Clears Pending Interrupt"
 			" SysEvt if triggered  0x%08x\n",val);
 	val = CSR_READ_4(pDev, HW_ICSS_INTC_SECR1);
 	PRUINTC_DBG_MSG (INTC_DBG_INFO,
 			"SECR1: Shows/Clears Pending Interrupt SysEvt "
 			"if triggered 0x%08x\n",val);
 	PRUINTC_DBG_MSG (INTC_DBG_INFO,
 			"HOST Wise Priority Event\n");
 	for( i=0 ; i<10 ; i++ )
 		{
 		val = CSR_READ_4(pDev, PRU_INTC_HIPIR(i));
 		PRUINTC_DBG_MSG (INTC_DBG_INFO,
 				"HIPIR(%d) : Show highest priority current pending "
 				"interrupt for host interrupt:0x%08x\n",i,val);
 		}
 	val = CSR_READ_4(pDev, PRU_INTC_GPIR);
 	PRUINTC_DBG_MSG (INTC_DBG_INFO,"GPIR : event number of highest "
 			"priority event pending across all host interrupt :0x%08x\n",val);
 	
 	return PRUICSS_RETURN_SUCCESS;
 	}

 
 /******************************************************************************
  *
  * prussIntcStringLookup - find string in a list and return index.
  *
  * This function find string in a list and return index of matched string, for
  * example, here is an dts node:

  * RETURNS: 
  *   0: for first string in list
  *   1: for second string in list
  *   2: for third string in list
  *   n: for n'TH string in list
  *(-1): when failed.
  *
  * ERRNO: N/A
  */

 LOCAL STATUS prussIntcStringLookup
     (
     int     	   offset,
     char        * propName,
     char        * string
     )
     {
     char  * pPropStr;
     int     propLen, strLen;
     int     ix = 0;

     if (propName == NULL || string == NULL)
         {
    	 PRUINTC_DBG_MSG (INTC_DBG_ERR,"parameter error.\n");
         return (-1);
         }
     
     pPropStr = (char*)vxFdtPropGet (offset, propName, &propLen);
     if (pPropStr == NULL)
         {
    	 PRUINTC_DBG_MSG (INTC_DBG_ERR,"search %s error.\n",propName);
         return (-1);
         }

     while (propLen > 0)
         {
         strLen = strlen(pPropStr);
         if (strncmp(pPropStr, string, strLen) == 0)
             {
             return ix;
             }
         propLen  = propLen - strLen - 1;
         pPropStr = pPropStr + strLen + 1; /*move the string point to next string*/
         ix++;
         }

     /* not found, return -1 */
     PRUINTC_DBG_MSG (INTC_DBG_ERR,"not found %s error.\n",propName);
     return (-1);    
     }

 /******************************************************************************
  *
  * prussIntcEcatISR: ECAT ISR routine
  * 
  * function:
  * 
  * RETURNS:N/A 
  * 		
  * ERRNO: N/A
  **/
  
 LOCAL STATUS prussIntcEcatISR
  	(
  	void *			arg		
  	) 
 	 {
		PRUICSS_IrqArgs   *	irqArgs = arg;
		PRUICSS_Handle 		handle;
		PRUICSS_IrqFunMap *	pruFunMap;
		PRUICSS_V1_Object * object;
		VXB_DEV_ID 			pDev_t;
		int 				host;

		if (irqArgs == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Argument structure NULL\n");
			return ERROR;
			}
		
		handle = irqArgs->handle;
		if (handle == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Handle NULL\n");
			return ERROR;
			}
		
		object = handle->object;
	    if (object == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Object NULL\n");
			return ERROR;
			}
	    
		pDev_t = object->pDev;
		if (pDev_t == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
			return ERROR;
			}
		
		
	// kprintf("\nprussIntcEcatISR ISR Called Clearing Event\n\n");
	/* Presently ESS Running in polling mode so PDI_Isr is commented. */
	 //PDI_Isr();
	 //WriteDword_dram_iep(,,0x3);
	 prussIntcClearEvent(pDev_t,56);
	 
	 return PRUICSS_RETURN_SUCCESS;
	 }

 /******************************************************************************
  *
  * prussIntcEscCmdLowAckISR: EscCmdLowAck ISR routine
  * 
  * function:
  * 
  * RETURNS:N/A 
  * 		
  * ERRNO: N/A
  **/
    
 LOCAL STATUS prussIntcEscCmdLowAckISR
  	(
  	void *			arg			
  	) 
 	 {
		PRUICSS_IrqArgs   *	irqArgs = arg;
		PRUICSS_Handle 		handle;
		PRUICSS_IrqFunMap *	pruFunMap;
		PRUICSS_V1_Object * object;
		VXB_DEV_ID 			pDev_t;
		int 				host;

		if (irqArgs == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Argument structure NULL\n");
			return ERROR;
			}
		
		handle = irqArgs->handle;
		if (handle == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Handle NULL\n");
			return ERROR;
			}
		
		object = handle->object;
	    if (object == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Object NULL\n");
			return ERROR;
			}
	    
		pDev_t = object->pDev;
		if (pDev_t == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
			return ERROR;
			}
		/* Callinig PDI_Isr */
		(*pPDIIrqHandler)();
	 //kprintf("\nprussIntcEscCmdLowAckISR ISR Called Clearing Event\n\n");
	 prussIntcClearEvent(pDev_t,20);
	 
	 return PRUICSS_RETURN_SUCCESS;
     }
 
 /******************************************************************************
  *
  * prussIntcSync0ISR: Sync0 ISR routine
  * 
  * function:
  * 
  * RETURNS:N/A 
  * 		
  * ERRNO: N/A
  **/
   
 LOCAL STATUS prussIntcSync0ISR
  	(
  	void *			arg			
  	) 
 	 {
		PRUICSS_IrqArgs   *	irqArgs = arg;
		PRUICSS_Handle 		handle;
		PRUICSS_IrqFunMap *	pruFunMap;
		PRUICSS_V1_Object * object;
		VXB_DEV_ID 			pDev_t;
		int 				host;

		if (irqArgs == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Argument structure NULL\n");
			return ERROR;
			}
		
		handle = irqArgs->handle;
		if (handle == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Handle NULL\n");
			return ERROR;
			}
		
		object = handle->object;
	    if (object == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Object NULL\n");
			return ERROR;
			}
	    
		pDev_t = object->pDev;
		if (pDev_t == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
			return ERROR;
			}
		
	 //kprintf("\nprussIntcSync0ISR ISR Called Clearing Event\n\n");
	/* Presently ESS Running in polling mode so Sync0_Isr is commented. */
	 //Sync0_Isr();
	 prussIntcClearEvent(pDev_t,14);
	 
	 return PRUICSS_RETURN_SUCCESS;
    }

 /******************************************************************************
  *
  * prussIntcSync1ISR: Sync1 ISR routine
  * 
  * function:
  * 
  * RETURNS:N/A 
  * 		
  * ERRNO: N/A
  **/
   
 LOCAL STATUS prussIntcSync1ISR
  	(
  	void *			arg			
  	) 
 	 {
		PRUICSS_IrqArgs   *	irqArgs = arg;
		PRUICSS_Handle 		handle;
		PRUICSS_IrqFunMap *	pruFunMap;
		PRUICSS_V1_Object * object;
		VXB_DEV_ID 			pDev_t;
		int 				host;

		if (irqArgs == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Argument structure NULL\n");
			return ERROR;
			}
		
		handle = irqArgs->handle;
		if (handle == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Handle NULL\n");
			return ERROR;
			}
		
		object = handle->object;
	    if (object == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"IRQ Object NULL\n");
			return ERROR;
			}
	    
		pDev_t = object->pDev;
		if (pDev_t == NULL)
			{
			PRUINTC_DBG_MSG (INTC_DBG_ERR,"Driver Instance NULL\n");
			return ERROR;
			}
		
	 //kprintf("\nprussIntcSync1ISR ISR Called Clearing Event\n\n");
	 /* Presently ESS Running in polling mode so Sync1_Isr is commented. */
	 //Sync1_Isr();
	 prussIntcClearEvent(pDev_t,13);
	 
	 return PRUICSS_RETURN_SUCCESS;
     }

 /******************************************************************************
  *
  * pruRegister: PDI User ISR Register
  * 
  * function:
  * 
  * RETURNS:N/A 
  * 		
  * ERRNO: N/A
  **/
 
 
 LOCAL VOID pruRegister
  	(
  			VXB_DEV_ID pDev,
  			PRU_PDI_IRQ_HANDLER userHandler 			
  	) 
 {
//	 kprintf("In PDI User Register\n");
	 pPDIIrqHandler = userHandler;
 }