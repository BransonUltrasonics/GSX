/*
 * PRU-subsystem driver for AM437x IDK
 *
 *	This is child node for the PRU-ICSS.
 *	This driver code comprises basic functionality for PRUICSS 1 & 2.
 *	This driver will call child nodes to be configured for PRUICSS 1 & 2
 *	This function configures:
 *	1. Map DRAM0, 1  & Shared RAM
 *	2. Write Shared RAM with configuration needed by firmware to configure/identify 
 *		MDIO, MII_RT, etc.
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
#include <intLib.h>
#include <stdio.h>
#include <string.h>
#include <lstLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <wdLib.h>
#include <vmLib.h>
#include <etherMultiLib.h>
#include <end.h>
#include <endLib.h>
#include <endMedia.h>
#include <cacheLib.h>
#include <iosLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <hwif/buslib/vxbI2cLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <time.h>
#include <private/timeP.h>

#include <PRUESC.h>

#include <vxbFdtTiPrussEnd.h> /* Driver Header FIle */
#include <vxbPru.h>
#include <ecatSlavePalLib.h>
#include <subsys/gpio/vxbGpioLib.h>

static t_sm_properties sm_properties[MAX_SYNC_MAN];

/* Pointer to PRUICSS Device Structure  */
PRUICSS_CTRL         * pPruicssCtrl = NULL;

/* driver utility functions */
LOCAL VXB_DRV_METHOD prussMethods[] = {
    { VXB_DEVMETHOD_CALL (vxbDevProbe),     (FUNCPTR)pruicssProbe       },
    { VXB_DEVMETHOD_CALL (vxbDevAttach),    (FUNCPTR)pruicssAttach      },
    { VXB_DEVMETHOD_CALL (vxbFdtDevGet),    (FUNCPTR)pruicssFdtDevGet   },
    { VXB_DEVMETHOD_CALL (vxbResourceFree), (FUNCPTR)pruicssResFree     },
    { VXB_DEVMETHOD_CALL (vxbResourceAlloc),(FUNCPTR)pruicssResAlloc    },
    { 0, 0 }
};


/* VxBus driver definition */
VXB_DRV  prussDrv =
    {
    { NULL } ,
    "pruicss(0/1)",         /* drvName */
    "PRU e-CAT",            /* Description */
    VXB_BUSID_FDT,          /* Class */
    0,                      /* Flags */
    0,                      /* Reference count */
    prussMethods            /* Method table */
    };
VXB_DRV_DEF(prussDrv)
    

/* struct used for matching a device */
LOCAL VXB_FDT_DEV_MATCH_ENTRY prussMatch[] =
    {
        {
        "ti,pruss",             /* compatible */
        NULL,                           /* no configuration */
        },
        {}                              /* Empty terminated list */
    };


/******************************************************************************
*
* pruicssProbe - probe for device presence at specific address
*
* This routine probes for device presence at specific address.
*
* RETURNS: OK if probe passes.  ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS pruicssProbe
    (
    VXB_DEV_ID pDev
    )
    {
    return vxbFdtDevMatch (pDev, prussMatch, NULL);
    }


/******************************************************************************
*
* pruicssAttach - attach PRUICSS device
*
* This is the PRU Industrial Communication subsystem initialization routine.
*
* RETURNS: OK or ERROR when initialize failed
*
* ERRNO: N/A
*/
LOCAL STATUS pruicssAttach
    (
    VXB_DEV_ID          pDev
    )
    {
    VXB_FDT_DEV      * pFdtDev  = NULL;
    VXB_RESOURCE_ADR * pResAdr  = NULL;
    VXB_RESOURCE     * pRes     = NULL;
    int                regIndex = 0;
    uint32_t           result   = 0;
    int                len      = 0;
    uint8_t 		*  pBase;
    uint32_t 		   i, retVal;
    VXB_PRU_FUNC 	*  pPru;
    char               name[50];
    
    /* Check Driver Instance  ID*/
    if (pDev == NULL)
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR, "parameter error\n");
        return ERROR;
        }
    
    /* Get the FDT bus device information */
    pFdtDev = vxbFdtDevGet(pDev);       
    if (pFdtDev == NULL)
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR, "no fdtDev info\n");
        return ERROR;
        }

    /* Allocate Driver Structure */
    pPruicssCtrl = vxbMemAlloc (sizeof (PRUICSS_CTRL));
    
    if (pPruicssCtrl == NULL)
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR, "vxbMemAlloc failed\n");
        goto FAIL;
        }

    /* Save Driver Instance */
    vxbDevSoftcSet(pDev, pPruicssCtrl);
    pPruicssCtrl->pDev = pDev;

    /* Enable PinMux & Clock */
    (void) vxbPinMuxEnable (pDev);
    (void) vxbClkEnableAll (pDev);
    
    /* Semaphore initialization */
    pPruicssCtrl->pruSem = semMCreate (SEM_DELETE_SAFE | SEM_Q_PRIORITY);

    result = vxFdtPropGet(pFdtDev->offset, "reg", &len);
    
    /* create connect between pSwCtrl and it's pDev */
    if (pruicssRegbaseSetup(pPruicssCtrl, pDev) != OK)
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, 
				"Unable to set up the Register Base\n");
        goto FAIL;
        }
    
    /* Check if the current driver instance is pruicss1 or not */ 
    if(strncmp(pFdtDev->name, "pruicss1", strlen(pFdtDev->name)) == 0)
        {
    	
    	/* Set DRAM0 (8 kB) to 0 */
    	if (pPruicssCtrl->pDram0Regbase != NULL)
    		{
    		pBase = (uint8_t*)pPruicssCtrl->pDram0Regbase;
    		}
    	
        for( i = 0	; i < PRU0_DATA_RAM ; i += 4)
        	{
			retVal = vxbRead32(pPruicssCtrl->pDram0Handle,(UINT32 *)(pBase + i));
			vxbWrite32(pPruicssCtrl->pDram0Handle,(UINT32 *)(pBase + i),0);
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"DRAM0 @0x%x %x \n",i,
						vxbRead32(pPruicssCtrl->pDram0Handle,(pBase + i)));
			}
        
        /* Set Shared RAM (12 kB) to 0 */
        if (pPruicssCtrl->pSDram1Regbase != NULL)
        	{
        	pBase = (uint8_t*)pPruicssCtrl->pSDram1Regbase;
        	}
        
        for( i = 0 ; i <  ICSS_SHARED_RAM ; i += 4)
        	{
			retVal = vxbRead32(pPruicssCtrl->pSDram1Handle,(UINT32 *)(pBase + i));
			vxbWrite32(pPruicssCtrl->pSDram1Handle,(UINT32 *)(pBase + i),0);
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",i,
						vxbRead32(pPruicssCtrl->pSDram1Handle,(UINT32 *)(pBase + i)));
        	}	
        
        /* Set DRAM1 (1 kB) to 0 */
        if (pPruicssCtrl->pDram1Regbase != NULL)
			{
			pBase = (uint8_t*)pPruicssCtrl->pDram1Regbase;
			}
        
		for( i = 0 ; i <  PRU1_DATA_RAM_RESERVED_REGION ; i += 4)
			{
			retVal = vxbRead32(pPruicssCtrl->pDram1Handle,(UINT32 *)(pBase + i));
			vxbWrite32(pPruicssCtrl->pDram1Handle,(UINT32 *)(pBase + i),0);
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"DRAM1 @0x%x %x \n",i,
						vxbRead32(pPruicssCtrl->pDram1Handle,(UINT32 *)(pBase + i)));
			}
		
		/* Set DRAM1 (4 kB) to 0x3 */
		for( i = PRU1_DATA_RAM_RESERVED_REGION ; 
				i < PRU1_DATA_RAM_REG_PROP_REGION; i += 4)
			{
			vxbRead8(pPruicssCtrl->pDram1Handle,(UINT8 *)(pBase + i));
			vxbWrite8(pPruicssCtrl->pDram1Handle,(UINT8 *)(pBase + i),3);
			retVal = vxbRead8(pPruicssCtrl->pDram1Handle,(UINT8 *)(pBase + i));
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"DRAM1 @0x%x %x \n",
					i,retVal);
			}
		
		/* TODO: Initialize the Command interface pointer */
        //pruicssCmdIntfWrite(0xFF, &pHost2PruIntfc->cmdlow);

        /* Initialize the PRUICSS REG permissions */
        pruicssRegPermInit(pDev);

        /* Initialize PRUICSS Shared RAM */
        if (pPruicssCtrl->pSDram1Regbase != NULL)
        	{
        	pBase = (uint8_t*)pPruicssCtrl->pSDram1Regbase;
        	}
        
        /* Check and Update ESC_ADDR_TI_PORT0_TX_START_DELAY & 
         * 					ESC_ADDR_TI_PORT1_TX_START_DELAY in Shared RAM */
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT0_TX_START_DELAY,
				vxbRead16(pPruicssCtrl->pSDram1Handle,
						(UINT32 *)(pBase + ESC_ADDR_TI_PORT0_TX_START_DELAY)));
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT1_TX_START_DELAY,
				vxbRead16(pPruicssCtrl->pSDram1Handle,
						(UINT32 *)(pBase + ESC_ADDR_TI_PORT1_TX_START_DELAY)));
		vxbWrite16(pPruicssCtrl->pSDram1Handle,
				(UINT16 *)(pBase + ESC_ADDR_TI_PORT0_TX_START_DELAY),
				TIESC_PORT0_TX_DELAY);
		vxbWrite16(pPruicssCtrl->pSDram1Handle,
				(UINT16 *)(pBase + ESC_ADDR_TI_PORT1_TX_START_DELAY),
				TIESC_PORT1_TX_DELAY);
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT0_TX_START_DELAY,
				vxbRead16(pPruicssCtrl->pSDram1Handle,
						(UINT16 *)(pBase + ESC_ADDR_TI_PORT0_TX_START_DELAY)));
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT1_TX_START_DELAY,
				vxbRead16(pPruicssCtrl->pSDram1Handle,
						(UINT16 *)(pBase + ESC_ADDR_TI_PORT1_TX_START_DELAY)));
        
		 
		/* Check and Update ESC_ADDR_MI_ECAT_ACCESS in Shared RAM */
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
        		ESC_ADDR_MI_ECAT_ACCESS,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_ECAT_ACCESS)));
		vxbWrite8(pPruicssCtrl->pSDram1Handle,
				(UINT8 *)(pBase + ESC_ADDR_MI_ECAT_ACCESS),
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_ECAT_ACCESS)) | 1);
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_MI_ECAT_ACCESS,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_ECAT_ACCESS)));
			
		
		 //Acquire PDI access over MDIO/MI interface
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_MI_PDI_ACCESS,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_PDI_ACCESS)));
		vxbWrite8(pPruicssCtrl->pSDram1Handle,
				(pBase + ESC_ADDR_MI_PDI_ACCESS), 
				((vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_PDI_ACCESS)))|1));
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_MI_PDI_ACCESS,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_PDI_ACCESS)));
		
		
		/* Check and Update ESC_ADDR_TI_PORT0_PHYADDR in Shared RAM */
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT0_PHYADDR,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT32 *)(pBase + ESC_ADDR_TI_PORT0_PHYADDR)));
		vxbWrite8(pPruicssCtrl->pSDram1Handle,
				(UINT8 *)(pBase + ESC_ADDR_TI_PORT0_PHYADDR),0);
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT0_PHYADDR,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_TI_PORT0_PHYADDR)));

		/* Check and Update ESC_ADDR_TI_PORT1_PHYADDR in Shared RAM */
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT1_PHYADDR,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_TI_PORT1_PHYADDR)));
		vxbWrite8(pPruicssCtrl->pSDram1Handle,
				(UINT8 *)(pBase + ESC_ADDR_TI_PORT1_PHYADDR),1);
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PORT0_PHYADDR,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_TI_PORT1_PHYADDR)));

		/* Check and Update ESC_ADDR_TI_PDI_ISR_PINSEL in Shared RAM */
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PDI_ISR_PINSEL,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_TI_PDI_ISR_PINSEL)));
		vxbWrite8(pPruicssCtrl->pSDram1Handle,
				(UINT8 *)(pBase + ESC_ADDR_TI_PDI_ISR_PINSEL), 
				(1 << PDI_ISR_EDIO_NUM));
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PDI_ISR_PINSEL,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_TI_PDI_ISR_PINSEL)));
		
		/* Check and Update ESC_ADDR_TI_PHY_LINK_POLARITY in Shared RAM */
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_TI_PHY_LINK_POLARITY,
				vxbRead32(pPruicssCtrl->pSDram1Handle,
						(UINT32 *)(pBase + ESC_ADDR_TI_PHY_LINK_POLARITY)));
		vxbWrite32(pPruicssCtrl->pSDram1Handle,
				(UINT32 *)(pBase + ESC_ADDR_TI_PHY_LINK_POLARITY), 
				(TIESC_LINK_POL_ACTIVE_HIGH << 0) | 
				(TIESC_LINK_POL_ACTIVE_LOW << 1));
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
					ESC_ADDR_TI_PHY_LINK_POLARITY,
					vxbRead32(pPruicssCtrl->pSDram1Handle,
							(UINT32 *)(pBase + ESC_ADDR_TI_PHY_LINK_POLARITY)));
		
		//Release PDI access over MDIO/MI interface
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_MI_PDI_ACCESS,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_PDI_ACCESS)));
		vxbWrite8(pPruicssCtrl->pSDram1Handle,
				(pBase + ESC_ADDR_MI_PDI_ACCESS), 
				((vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_PDI_ACCESS)))&~1));
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"SDRAM @0x%x %x \n",
				ESC_ADDR_MI_PDI_ACCESS,
				vxbRead8(pPruicssCtrl->pSDram1Handle,
						(UINT8 *)(pBase + ESC_ADDR_MI_PDI_ACCESS)));				
	    }


        
    /* Call child nodes */
    if (pruicssChildCreate (pDev, pFdtDev) == ERROR)
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, 
        			"pruicssCreate() return error\n");
        goto FAIL;
        }

    /* ONLY for DEBUG Purpose */
#if PRUSS_END_DEBUG
    if(strncmp(pFdtDev->name, "pruicss1", strlen(pFdtDev->name)) == 0)
		{
		pBase               = (uint8_t*)pPruicssCtrl->pSDram1Regbase;
		
		for( i = 0 ; i < ICSS_SHARED_RAM ; i += 4)
			{
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
					"After Firmware Upload Checking PRU SDRAM @0x%x %x \n",
					i,
					vxbRead32(pPruicssCtrl->pSDram1Handle,(pBase + i)));
			}
		}
#endif
    
	/* Add the device to the I/O systems device list */
	(void) snprintf (name, 50, "/%s/%d","pru", 0);

	pPru = (VXB_PRU_FUNC *)vxbMemAlloc (sizeof (VXB_PRU_FUNC));
	
	if (pPru == NULL) {
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"memAlloc failed\n");
		goto FAIL;	
	}

	pPru->pruRead   = vxbPruRead;
	pPru->pruWrite  = vxbPruWrite;                  
	
	if (pruDevCreate (name, pDev, pPru) != OK)
		{
		vxbMemFree (pPru);
		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"NULL");
		goto FAIL;
		}
	
	vxbMemFree (pPru);

	/* Tricolor LED */
	
	/* My GPIO 2 -> gpio2[23] -> A23 */
    vxbGpioAlloc(GPIO_BANK2_PIN23);
    vxbGpioSetDir(GPIO_BANK2_PIN23,GPIO_DIR_OUTPUT);
    vxbGpioSetValue(GPIO_BANK2_PIN23,GPIO_VALUE_LOW);
    
	/* My GPIO 2 -> gpio2[24] -> A22 */
    vxbGpioAlloc(GPIO_BANK2_PIN24);
    vxbGpioSetDir(GPIO_BANK2_PIN24,GPIO_DIR_OUTPUT);
    vxbGpioSetValue(GPIO_BANK2_PIN24,GPIO_VALUE_LOW);
    
	/* My GPIO 2 -> gpio2[25] -> A24 */
    vxbGpioAlloc(GPIO_BANK2_PIN25);
    vxbGpioSetDir(GPIO_BANK2_PIN25,GPIO_DIR_OUTPUT);
    vxbGpioSetValue(GPIO_BANK2_PIN25,GPIO_VALUE_LOW);
    
	/* My GPIO 2 -> gpio2[22] -> B23 */
    vxbGpioAlloc(GPIO_BANK2_PIN22);
    vxbGpioSetDir(GPIO_BANK2_PIN22,GPIO_DIR_OUTPUT);
    vxbGpioSetValue(GPIO_BANK2_PIN22,GPIO_VALUE_LOW);
    
	/* My GPIO 4 -> gpio4[13] -> AB25 */
    vxbGpioAlloc(GPIO_BANK4_PIN13);
    vxbGpioSetDir(GPIO_BANK4_PIN13,GPIO_DIR_OUTPUT);
    vxbGpioSetValue(GPIO_BANK4_PIN13,GPIO_VALUE_LOW);
    
	/* My GPIO 4 -> gpio4[16] -> AD21 */
    vxbGpioAlloc(GPIO_BANK4_PIN16);
    vxbGpioSetDir(GPIO_BANK4_PIN16,GPIO_DIR_OUTPUT);
    vxbGpioSetValue(GPIO_BANK4_PIN16,GPIO_VALUE_LOW);
	
	
	/* Register with ESS */
	ecatSlave_Register (pDev);
	
    return OK;
            
    FAIL:
	    if (pRes != NULL)
            {
            (void) vxbResourceFree (pDev, pRes);
            }   
	    
	    if (pPruicssCtrl->pruSem != SEM_ID_NULL)
	        {
	        (void)semDelete (pPruicssCtrl->pruSem);
	        }
	    
        if (pPruicssCtrl != NULL)
            {
            vxbMemFree (pPruicssCtrl);
            }
        
        return ERROR;
    }


/*******************************************************************************
*
* pruicssChildCreate - create the pruicss child devices - INTC, CORE 0/1, MDIO
*
* This function implements the VxBus gen2 bus attach routine for pruicss
* device instance.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/ 

LOCAL STATUS pruicssChildCreate
    (
    VXB_DEV_ID  pDev,
    VXB_FDT_DEV * pParFdtDev
    )
    {
    VXB_FDT_DEV        * pFdtDev;
    VXB_PRUICSS_INFO   * pPruicssDevInfo;
    VXB_DEV_ID           pCur    = NULL;
    int                  offset  = pParFdtDev->offset;

    for (offset = VX_FDT_CHILD(offset);
         offset > 0;
         offset = VX_FDT_PEER(offset)
        )
        {
        pCur = NULL;

        if (vxbDevCreate (&pCur) != OK)
            continue;

        pPruicssDevInfo =
        		(VXB_PRUICSS_INFO *)vxbMemAlloc(sizeof(*pPruicssDevInfo));

        if (pPruicssDevInfo == NULL)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, "Mem alloc failed\n");
            (void) vxbDevDestroy(pCur);
            continue;
            }

        pFdtDev = &pPruicssDevInfo->vxbFdtDev;

        vxbFdtDevSetup(offset, pFdtDev);
        vxbDevNameSet(pCur, pFdtDev->name, FALSE);

        /* Get the device register and interrupt infomation  */

        if (vxbResourceInit(&pPruicssDevInfo->vxbResList) != OK)
            {
            (void) vxbDevDestroy (pCur);
            vxbMemFree (pPruicssDevInfo);
            continue;
            }

        if(vxbFdtRegGet(&pPruicssDevInfo->vxbResList, pFdtDev) == ERROR)
            {
            vxbFdtResFree(&pPruicssDevInfo->vxbResList);
            vxbMemFree(pPruicssDevInfo);
            (void) vxbDevDestroy(pCur);
            continue;
            }

        if (vxbFdtIntGet(&pPruicssDevInfo->vxbResList, pFdtDev) == ERROR)
            {
            vxbFdtResFree(&pPruicssDevInfo->vxbResList);
            vxbMemFree(pPruicssDevInfo);
            (void) vxbDevDestroy(pCur);
            continue;
            }

        /* Assign the bus internal variable and type  */

        vxbDevIvarsSet(pCur, (void *) pPruicssDevInfo);
        vxbDevClassSet(pCur, VXB_BUSID_FDT);        
        vxbDevAdd(pDev, pCur);
        }

    return OK;
    }


/*********************************************************************************
*
* pruicssFdtDevGet - get the FDT bus device information
*
* This routine gets the FDT bus device information for the child nodes of PRU ESC.
*
* The routine vxbFdtDevGet (pDev); would be replaced with the method
* pruicssFdtDevGet (pDev); in the child node.
*
* RETURNS: device information pointer, or NULL if parent is NULL
*
* ERRNO: N/A
*/ 
    
LOCAL VXB_FDT_DEV * pruicssFdtDevGet
    (
    VXB_DEV_ID pDev,
    VXB_DEV_ID pChild
    )
    {
    VXB_PRUICSS_INFO * pPruDevInfo;
    if (pChild == NULL)
        return NULL;

    pPruDevInfo = (VXB_PRUICSS_INFO *) vxbDevIvarsGet (pChild);

    if (pPruDevInfo == NULL)
        return NULL;

    return &pPruDevInfo->vxbFdtDev;
    }


/*******************************************************************************
*
* pruicssResFree - free resource allocated by calling vxbResourceAlloc
*
* This routine replaces the child's device resource free method to free the
* resource.
*
* RETURNS: OK, ERROR when free fails or parent is NULL
*
* ERRNO: N/A
*/
    
LOCAL STATUS pruicssResFree
    (
    VXB_DEV_ID     pDev,
    VXB_DEV_ID     pChild,
    VXB_RESOURCE * pRes
    )
    {
    if ((pChild == NULL) || (pRes == NULL))
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* pruicssResAlloc - allocate resource from device resource list
*
* This routine replaces the child's device resource allocate method to get the
* resource with given ID and type.
*
* RETURNS: resource pointer or NULL if parent is NULL
*
* ERRNO: N/A
*/

LOCAL VXB_RESOURCE * pruicssResAlloc
    (
    VXB_DEV_ID pDev,
    VXB_DEV_ID pChild,
    UINT32     id
    )
    {
    VXB_PRUICSS_INFO * pPruDevInfo;
    VXB_RESOURCE * vxbRes;
    
    pPruDevInfo = (VXB_PRUICSS_INFO *) vxbDevIvarsGet (pChild);
    if (pPruDevInfo == NULL)
        {
        return NULL;
        }

    vxbRes = vxbResourceFind (&pPruDevInfo->vxbResList, id);

    if (vxbRes == NULL)
        {
        return NULL;
        }

    if (((VXB_RES_TYPE(vxbRes->id) == VXB_RES_MEMORY) ||
         (VXB_RES_TYPE(vxbRes->id) == VXB_RES_IO)) &&
         (vxbRegMap (vxbRes) == OK))
        {
        return vxbRes;
        }
    else if ((VXB_RES_TYPE (vxbRes->id) == VXB_RES_IRQ) &&
             (vxbIntMap (vxbRes) == OK))
        {
        return vxbRes;
        }
    else
        {
        return NULL;
        }
    }


/******************************************************************************
 *
 * pruicssStringLookup - find string in a list and return index.
 *
 * This function find string in a list and return index of matched string, for
 * example, here is an dts node:
 *
 *    pruicss1: pruicss1@0x54400000
 *        {
 *        compatible          = "ti,pruss";
 *        #address-cells      = <1>;
 *        #size-cells         = <1>;
 *        reg                 = <0x54400000 0x2000>,
 *                              <0x54402000 0x2000>,
 *                              <0x54410000 0x8000>,
 *                              <0x54426000 0x2000>,
 *                              <0x5442e000 0x31c>,
 *                              <0x54432000 0x58>;
 *        reg-names           = "dram0",
 *                              "dram1",
 *                              "shdram1",
 *                              "cfg",
 *                              "iep",
 *                              "mii_rt";         
 *        ranges;
 *        }
 *
 * pruicssStringLookup(pFdtDev->offset, "reg-names", "dram0"    ); will return 0;
 * pruicssStringLookup(pFdtDev->offset, "reg-names", "dram1"    ); will return 1;
 * pruicssStringLookup(pFdtDev->offset, "reg-names", "shdram1"  ); will return 2;
 * pruicssStringLookup(pFdtDev->offset, "reg-names", "cfg"      ); will return 3;
 * pruicssStringLookup(pFdtDev->offset, "reg-names", "iep"      ); will return 4;
 * pruicssStringLookup(pFdtDev->offset, "reg-names", "mii_rt"   ); will return 5;
 *
 * RETURNS: 
 *   0: for first string in list
 *   1: for second string in list
 *   2: for third string in list
 *   n: for n'TH string in list
 *(-1): when failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS pruicssStringLookup
    (
    int     offset,
    char        * propName,
    char        * string
    )
    {
    char  * pPropStr;
    int     propLen, strLen;
    int     ix = 0;

    if (propName == NULL || string == NULL)
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, "parameter error.\n");
        return ERROR;
        }
    
    pPropStr = (char*)vxFdtPropGet (offset, propName, &propLen);
    if (pPropStr == NULL)
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, 
        				"search %s error.\n",propName);
        return ERROR;
        }

    /* 
     * the returned property is collection of NULL end strings like this:
     * 'b' 'l' 'u' 'e' '0' 'r' 'e' 'd' '0'
     */

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
    PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
    				"not found %s error.\n",propName);
    return ERROR;    
    }


/*******************************************************************************
*
* pruicssRegbaseSetup - setup register base address of clock
*
* This routine will search the FDT to setup register base address for a clock.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
LOCAL STATUS pruicssRegbaseSetup
    (
    PRUICSS_CTRL      * pPruicssCtrl,
    VXB_DEV_ID      pDev
    )
    {
    int                regIndex     = 0xFF;
    VXB_FDT_DEV      * pFdtDev      = NULL;
    VXB_RESOURCE     * pRes         = NULL;
    VXB_RESOURCE_ADR * pResAdr      = NULL;
    uint32_t           strLen       = 0;
    char             * pruicss      = "pruicss1";

    if (pPruicssCtrl == NULL)
        {
    	PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,"parameter error\n");
        return ERROR;
        }

    pFdtDev = vxbFdtDevGet(pDev);
    
    /* get register base address of "dram0" if it exists */
    {
    regIndex = pruicssStringLookup(pFdtDev->offset, "reg-names", "dram0");
    if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pPruicssCtrl->pDev, VXB_RES_MEMORY, regIndex);
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
            		"PRU ESC->(%s) dram0 pRes error(dram0)\n",pFdtDev->name);
            return ERROR;
            }
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
            		"PRU ESC->(%s) dram0  pResAdr is NULL(dram0)\n",
					pFdtDev->name);
            return ERROR;
            }
    
        /* save the resource info */
        pPruicssCtrl->pDram0Handle  = pResAdr->pHandle;
        pPruicssCtrl->pDram0Regbase = (void*)pResAdr->virtual;
        
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        		"PRU ESC(%s) dram0 Register P=0x%08x,V=0x%08x, regIndex=%d\n",
				pFdtDev->name,(UINT32)pResAdr->start,
				(UINT32)pResAdr->virtAddr, regIndex);
        }
    else
        {
    	PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
    			"Invalid Reg lookup of dram0 (%s), regIndex=%d\n",
				pFdtDev->name, regIndex);
        }
    }

    /* get register base address of "dram1" if it exists */    
    {
    regIndex = pruicssStringLookup(pFdtDev->offset, "reg-names", "dram1");
    
    if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pPruicssCtrl->pDev, VXB_RES_MEMORY, regIndex);
        
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
            		"PRU ESC->(%s) dram1  pRes error(dram1)\n",pFdtDev->name);
            return ERROR;
            }
        
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        
        if (pResAdr == NULL)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
            		"PRU ESC->(%s) dram1  pResAdr is NULL(dram1)\n",
					pFdtDev->name);
            return ERROR;
            }
    
        /* save the resource info */
        pPruicssCtrl->pDram1Handle  = pResAdr->pHandle;
        pPruicssCtrl->pDram1Regbase = (void*)pResAdr->virtual;
        
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
					"PRU ESC(%s) dram1 Register P=0x%08x,"
					"V=0x%08x, regIndex=%d\n",
					pFdtDev->name,(UINT32)pResAdr->start,
					(UINT32)pResAdr->virtAddr, regIndex);
        }
    else
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        			"Invalid Reg lookup of dram1 (%s), regIndex=%d\n",
					pFdtDev->name, regIndex);
        }
    }         

    /* get register base address of "shdram1" if it exists */
    pFdtDev = vxbFdtDevGet(pDev);
    
    strLen = strlen(pFdtDev->name);
    
    if (strncmp(pFdtDev->name, pruicss, strLen) == 0)
        {
        regIndex = pruicssStringLookup(pFdtDev->offset,
        				"reg-names", "shdram1");
        if (regIndex >= 0)
            {
            pRes = vxbResourceAlloc(pPruicssCtrl->pDev, VXB_RES_MEMORY,
            					regIndex);
            
            if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
                {
                PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
                		"PRU ESC->(%s) sdrm1  pRes error(shdram1)\n",
						pFdtDev->name);
                return ERROR;
                }
            
            pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
            
            if (pResAdr == NULL)
                {
                PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
                		"PRU ESC->(%s) shdram1 pResAdr is NULL(shdram1)\n",
						pFdtDev->name);
                return ERROR;
                }
    
            /* save the resource info */
            pPruicssCtrl->pSDram1Handle  = pResAdr->pHandle;
            pPruicssCtrl->pSDram1Regbase = (void*)pResAdr->virtual;
            
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            			"PRU ESC(%s) sdrm1 Register P=0x%08x,V=0x%08x,"
            			" regIndex=%d\n",
            			pFdtDev->name,(UINT32)pResAdr->start,
						(UINT32)pResAdr->virtAddr, regIndex);
            }
        else
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            			"Invalid Reg lookup of shrdram1 (%s), regIndex=%d\n",
						pFdtDev->name, regIndex);
            }
    }     

        /* get register base address of "cfg" if it exists */    
    {
    regIndex = pruicssStringLookup(pFdtDev->offset, "reg-names", "cfg");
    if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pPruicssCtrl->pDev, VXB_RES_MEMORY, regIndex);
        
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            			"PRU ESC->(%s) cfg pRes error(cfg)\n",
						pFdtDev->name);
            return ERROR;
            }
        
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        
        if (pResAdr == NULL)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            			"PRU ESC->(%s) cfg pResAdr is NULL(cfg)\n",
						pFdtDev->name);
            return ERROR;
            }
    
        /* save the resource info */
        pPruicssCtrl->pCfgHandle    = pResAdr->pHandle;
        pPruicssCtrl->pCfgRegbase   = (void*)pResAdr->virtual;
        
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        				"PRU ESC(%s) cfg Register P=0x%08x,V=0x%08x,"
        				" regIndex=%d\n",
						pFdtDev->name,(UINT32)pResAdr->start,
						(UINT32)pResAdr->virtAddr, regIndex);
        }
    else
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        				"Invalid Reg lookup of cfg (%s), regIndex=%d\n",
						pFdtDev->name, regIndex);
        }
    }
        

    /* get register base address of "iep" if it exists */
    {
    regIndex = pruicssStringLookup(pFdtDev->offset, "reg-names", "iep");
    if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pPruicssCtrl->pDev, VXB_RES_MEMORY, regIndex);
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            				"PRU ESC->(%s) iep pRes error(iep)\n",
            				pFdtDev->name);
            return ERROR;
            }
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            				"PRU ESC->(%s) iep pResAdr is NULL(iep)\n",
            				pFdtDev->name);
            return ERROR;
            }
    
        /* save the resource info */        
        pPruicssCtrl->pIepHandle    = pResAdr->pHandle;
        pPruicssCtrl->pIepRegbase   = (void*)pResAdr->virtual;
        
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        					"PRU ESC(%s) iep Register "
        					"P=0x%08x,V=0x%08x, regIndex=%d\n",
        					pFdtDev->name, (UINT32)pResAdr->start,
							(UINT32)pResAdr->virtAddr, regIndex);

        if(strncmp(pFdtDev->name, "pruicss1", strlen(pFdtDev->name)) == 0)
            {       
            /* Enable WD */
            if (pruicssEnWatchDog(pPruicssCtrl, pDev) != OK)
                {
                PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
                			"Unable to enable PRUICSS Watch Dog\n");
                }

            /* Enable DGIO */
            if (pruicssEnDGIO(pPruicssCtrl, pDev) != OK)
                {
                PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
                			"Unable to enable PRUICSS DGIO\n");
                }

            }
        
        }
    else
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        					"Invalid Reg lookup of iep (%s), regIndex=%d\n",
        					pFdtDev->name, regIndex);
        }
    }

    /* get register base address of "mii_rt" if it exists */
    {
    regIndex = pruicssStringLookup(pFdtDev->offset, "reg-names", "mii_rt");
    if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pPruicssCtrl->pDev, VXB_RES_MEMORY, regIndex);
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            				"PRU ESC->(%s) pRes error(mii_rt)\n",
            				pFdtDev->name);
            return ERROR;
            }
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            				"PRU ESC->(%s) pResAdr is NULL(mii_rt)\n",
            				pFdtDev->name);
            return ERROR;
            }
    
        /* save the resource info */        
        pPruicssCtrl->pMiiRtHandle  = pResAdr->pHandle;
        pPruicssCtrl->pMiiRtRegbase = (void*)pResAdr->virtual;
        
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        					"PRU ESC(%s) mii_rt Register"
        					" P=0x%08x,V=0x%08x, regIndex=%d\n",
        					pFdtDev->name, (UINT32)pResAdr->start,
							(UINT32)pResAdr->virtAddr, regIndex);
        }
        else
        {
        PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
        					"Invalid Reg lookup of mii_rt"
        					" (%s), regIndex=%d\n",
        					pFdtDev->name, regIndex);
        }
    }    
    
    return OK;
    }


/*******************************************************************************
*
* pruicssEnWatchDog - Enable PRUICSS Watch Dog
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
LOCAL STATUS pruicssEnWatchDog
    (
    PRUICSS_CTRL      * pPruicssEnwCtrl,
    VXB_DEV_ID          pDev
    )
    {

    uint32_t      mode              = (1 << 4);
    uint8_t    * iepRegBaseAddr    = (uint8_t *)pPruicssEnwCtrl->pIepRegbase;

    vxbWrite32(pPruicssEnwCtrl->pIepHandle
              ,(UINT32 *)(iepRegBaseAddr + PRUICSS_IEP_DIGIO_CTRL)
              ,mode);
    PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
    					"\n\n\t\t\tWatchDog of PRUICSS is enabled!\n");        

    return OK;
    }


/*******************************************************************************
*
* pruicssEnDGIO - Enable PRUICSS DGIO
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
LOCAL STATUS pruicssEnDGIO
    (
    PRUICSS_CTRL      * pPruicssCtrl,
    VXB_DEV_ID          pDev
    )
    {

    uint32_t      mode              = 0x0;
    uint8_t    * iepRegBaseAddr    = (uint8_t *)pPruicssCtrl->pIepRegbase;
    
  mode =  vxbRead32(pPruicssCtrl->pIepHandle
              ,(UINT32 *)(iepRegBaseAddr + PRUICSS_IEP_DIGIO_EXP));
  
  mode = mode | 0x1;
  
  vxbWrite32(pPruicssCtrl->pIepHandle
                ,(UINT32 *)(iepRegBaseAddr + PRUICSS_IEP_DIGIO_EXP),mode);
    
    PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, 
    					"\n\t\t\tDGIO of PRUICSS is enabled!\n");        

    return OK;
    }


/*******************************************************************************
*
* pruicssCmdIntfWrite - Initialize the Command interface pointer
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/

LOCAL STATUS pruicssCmdIntfWrite
    (uint16_t           val
    ,volatile uint16_t *ptr)
    {
    PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
    		"\n\t\t\tInitializing the PRUICSS CMD Interface pointer!\n");
    *ptr = val;

    return OK;
    }


/*******************************************************************************
*
* pruicssRegPermInit - Set up the Register permission initialization
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/

LOCAL STATUS pruicssRegPermInit
    (VXB_DEV_ID pDev)
    {
    uint32_t regIndex;
    volatile t_register_properties *pRegPerm = 
                (volatile t_register_properties *)pPruicssCtrl->pDram1Regbase;
    PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
    		"\n\t\t\tInitializing the PRUICSS REG permissions!\n\n\n");
#if 0
    pRegPerm->reg_properties[0x0]= TIESC_PERM_READ_ONLY;//Type
    pRegPerm->reg_properties[0x1]= TIESC_PERM_READ_ONLY;//Revision
    pRegPerm->reg_properties[0x2]= TIESC_PERM_READ_ONLY;//Build
    pRegPerm->reg_properties[0x3]= TIESC_PERM_READ_ONLY;//Build
    pRegPerm->reg_properties[0x4]= TIESC_PERM_READ_ONLY;//FMMUs
    pRegPerm->reg_properties[0x5]= TIESC_PERM_READ_ONLY;//SMs
    pRegPerm->reg_properties[0x6]= TIESC_PERM_READ_ONLY;//RAM size
    pRegPerm->reg_properties[0x7]= TIESC_PERM_READ_ONLY;//Port descriptor
    pRegPerm->reg_properties[0x8]= TIESC_PERM_READ_ONLY;//ESC features
    pRegPerm->reg_properties[0x9]= TIESC_PERM_READ_ONLY;//ESC features
#else
    for (regIndex=0; regIndex < 10; regIndex++)
        {
        pRegPerm->reg_properties[regIndex]= TIESC_PERM_READ_ONLY;
        }
#endif
    pRegPerm->reg_properties[0x10]= TIESC_PERM_RW;//Configured Station address
    pRegPerm->reg_properties[0x11]= TIESC_PERM_RW;//Configured Station address
    pRegPerm->reg_properties[0x12]= TIESC_PERM_READ_ONLY;//Configured Station alias
    pRegPerm->reg_properties[0x13]= TIESC_PERM_READ_ONLY;//Configured Station alias
    pRegPerm->reg_properties[0x100]= TIESC_PERM_RW;//DL control
    pRegPerm->reg_properties[0x101]= TIESC_PERM_RW;//DL control
    pRegPerm->reg_properties[0x102]= TIESC_PERM_READ_ONLY;//DL control
    pRegPerm->reg_properties[0x103]= TIESC_PERM_RW;//DL control
    pRegPerm->reg_properties[0x108]= TIESC_PERM_RW;//Physical RW offset
    pRegPerm->reg_properties[0x109]= TIESC_PERM_RW;//Physical RW offset
    pRegPerm->reg_properties[0x110]= TIESC_PERM_READ_ONLY;//ESC DL status
    pRegPerm->reg_properties[0x111]= TIESC_PERM_READ_ONLY;//ESC DL status
    pRegPerm->reg_properties[0x120]= TIESC_PERM_RW;//AL control
    pRegPerm->reg_properties[0x121]= TIESC_PERM_RW;//AL control
    pRegPerm->reg_properties[0x130]= TIESC_PERM_READ_ONLY;//ESC AL status
    pRegPerm->reg_properties[0x131]= TIESC_PERM_READ_ONLY;//ESC AL status
    pRegPerm->reg_properties[0x134]= TIESC_PERM_READ_ONLY;//ESC AL status code
    pRegPerm->reg_properties[0x135]= TIESC_PERM_READ_ONLY;//ESC AL status code
    pRegPerm->reg_properties[0x140]= TIESC_PERM_READ_ONLY;//ESC PDI control
    pRegPerm->reg_properties[0x141]= TIESC_PERM_READ_ONLY;//ESC configuration
    pRegPerm->reg_properties[0x150]= TIESC_PERM_READ_ONLY;//Onchip configuration
    pRegPerm->reg_properties[0x151]= TIESC_PERM_READ_ONLY;//SYNC/LATCH PDI configuration
    pRegPerm->reg_properties[0x152]= TIESC_PERM_READ_ONLY;//Onchip extended configuration
    pRegPerm->reg_properties[0x153]= TIESC_PERM_READ_ONLY;//Onchip extended configuration
    pRegPerm->reg_properties[0x200]= TIESC_PERM_RW;//ECAT event mask
    pRegPerm->reg_properties[0x201]= TIESC_PERM_RW;//ECAT event mask
    pRegPerm->reg_properties[0x204]= TIESC_PERM_READ_ONLY;//AL event mask
    pRegPerm->reg_properties[0x205]= TIESC_PERM_READ_ONLY;//AL event mask
    pRegPerm->reg_properties[0x206]= TIESC_PERM_READ_ONLY;//AL event mask
    pRegPerm->reg_properties[0x207]= TIESC_PERM_READ_ONLY;//AL event mask
    pRegPerm->reg_properties[0x210]= TIESC_PERM_READ_ONLY;//ECAT event request
    pRegPerm->reg_properties[0x211]= TIESC_PERM_READ_ONLY;//ECAT event request
    pRegPerm->reg_properties[0x220]= TIESC_PERM_READ_ONLY;//AL event request
    pRegPerm->reg_properties[0x221]= TIESC_PERM_READ_ONLY;//AL event request
    pRegPerm->reg_properties[0x222]= TIESC_PERM_READ_ONLY;//AL event request
    pRegPerm->reg_properties[0x223]= TIESC_PERM_READ_ONLY;//AL event request
    pRegPerm->reg_properties[0x300]= TIESC_PERM_RW;//Invalid frame counter Port0
    pRegPerm->reg_properties[0x301]= TIESC_PERM_RW;//RX_ERR counter Port0
    pRegPerm->reg_properties[0x302]= TIESC_PERM_RW;//Invalid frame counter Port1
    pRegPerm->reg_properties[0x303]= TIESC_PERM_RW;//RX_ERR counter Port1
    pRegPerm->reg_properties[0x304]= TIESC_PERM_READ_ONLY;//Invalid frame counter Port2
    pRegPerm->reg_properties[0x305]= TIESC_PERM_READ_ONLY;//RX_ERR counter Port2
    pRegPerm->reg_properties[0x306]= TIESC_PERM_READ_ONLY;//Invalid frame counter Port3
    pRegPerm->reg_properties[0x307]= TIESC_PERM_READ_ONLY;//RX_ERR counter Port3
    pRegPerm->reg_properties[0x308]= TIESC_PERM_RW;//Forwarded Error Port0
    pRegPerm->reg_properties[0x309]= TIESC_PERM_RW;//Forwarded Error Port1
    pRegPerm->reg_properties[0x30A]= TIESC_PERM_READ_ONLY;//Forwarded Error Port2
    pRegPerm->reg_properties[0x30B]= TIESC_PERM_READ_ONLY;//Forwarded Error Port3
    pRegPerm->reg_properties[0x30C]= TIESC_PERM_RW;//ECAT processing unit counter
    pRegPerm->reg_properties[0x310]= TIESC_PERM_RW;//Link lost counter Port0
    pRegPerm->reg_properties[0x311]= TIESC_PERM_RW;//Link lost counter Port1
    pRegPerm->reg_properties[0x312]= TIESC_PERM_READ_ONLY;//Link lost counter Port2
    pRegPerm->reg_properties[0x313]= TIESC_PERM_READ_ONLY;//Link lost counter Port3
    pRegPerm->reg_properties[0x400]= TIESC_PERM_RW;//Watchdog divider
    pRegPerm->reg_properties[0x401]= TIESC_PERM_RW;//Watchdog divider
    pRegPerm->reg_properties[0x410]= TIESC_PERM_RW;//Watchdog time PDI
    pRegPerm->reg_properties[0x411]= TIESC_PERM_RW;//Watchdog time PDI
    pRegPerm->reg_properties[0x420]= TIESC_PERM_RW;//Watchdog time PD
    pRegPerm->reg_properties[0x421]= TIESC_PERM_RW;//Watchdog time PD
    pRegPerm->reg_properties[0x440]= TIESC_PERM_READ_ONLY;//Watchdog process data
    pRegPerm->reg_properties[0x441]= TIESC_PERM_READ_ONLY;//Watchdog process data
    pRegPerm->reg_properties[0x442]= TIESC_PERM_RW;//Watchdog counter PD
    pRegPerm->reg_properties[0x443]= TIESC_PERM_RW;//Watchdog counter PDI
    pRegPerm->reg_properties[0x500]= TIESC_PERM_RW;//EEPROM configuration
    pRegPerm->reg_properties[0x501]= TIESC_PERM_READ_ONLY;//EEPROM PDI access state
#if 0
    pRegPerm->reg_properties[0x502]= TIESC_PERM_RW;//EEPROM control/status
    pRegPerm->reg_properties[0x503]= TIESC_PERM_RW;//EEPROM control/status
    pRegPerm->reg_properties[0x504]= TIESC_PERM_RW;//EEPROM address
    pRegPerm->reg_properties[0x505]= TIESC_PERM_RW;//EEPROM address
    pRegPerm->reg_properties[0x506]= TIESC_PERM_RW;//EEPROM address
    pRegPerm->reg_properties[0x507]= TIESC_PERM_RW;//EEPROM address
    pRegPerm->reg_properties[0x508]= TIESC_PERM_RW;//EEPROM data0
    pRegPerm->reg_properties[0x509]= TIESC_PERM_RW;//EEPROM data0
#else
    for(regIndex=0; regIndex < 8; regIndex++)
        {
        pRegPerm->reg_properties[0x502+regIndex]= TIESC_PERM_RW;
        }
#endif
#if 0
    pRegPerm->reg_properties[0x50A]= TIESC_PERM_READ_ONLY;//EEPROM data1
    pRegPerm->reg_properties[0x50B]= TIESC_PERM_READ_ONLY;//EEPROM data1
    pRegPerm->reg_properties[0x50C]= TIESC_PERM_READ_ONLY;//EEPROM data2
    pRegPerm->reg_properties[0x50D]= TIESC_PERM_READ_ONLY;//EEPROM data2
    pRegPerm->reg_properties[0x50E]= TIESC_PERM_READ_ONLY;//EEPROM data3
    pRegPerm->reg_properties[0x50F]= TIESC_PERM_READ_ONLY;//EEPROM data3
#else
    for(regIndex=0; regIndex < 6; regIndex++)
        pRegPerm->reg_properties[0x50A+regIndex]= TIESC_PERM_READ_ONLY;
#endif
#if 0
    pRegPerm->reg_properties[0x510]= TIESC_PERM_RW;//MII management control and status
    pRegPerm->reg_properties[0x511]= TIESC_PERM_RW;//MII management control and status
    pRegPerm->reg_properties[0x512]= TIESC_PERM_RW;//MII management PHY address
    pRegPerm->reg_properties[0x513]= TIESC_PERM_RW;//MII management PHY register address
    pRegPerm->reg_properties[0x514]= TIESC_PERM_RW;//MII management PHY data
    pRegPerm->reg_properties[0x515]= TIESC_PERM_RW;//MII management PHY data
    pRegPerm->reg_properties[0x516]= TIESC_PERM_RW;//MII management ECAT access state
    pRegPerm->reg_properties[0x517]= TIESC_PERM_RW;//MII management PDI access state
#else
    for(regIndex=0; regIndex < 8; regIndex++)
        {
        pRegPerm->reg_properties[0x510+regIndex]= TIESC_PERM_RW;
        }
#endif
    for (regIndex=0; regIndex < 8; regIndex++) //8 FMMUs
    {
        memset((void*)&pRegPerm->reg_properties[0x600+regIndex*16],
        				TIESC_PERM_RW, 13);
        memset((void*)&pRegPerm->reg_properties[0x60D+regIndex*16],
        				TIESC_PERM_READ_ONLY, 3);//Mark reserved bytes read only
    }

    for (regIndex=0; regIndex < 8; regIndex++) //8 SMs
    {
        memset((void*)&pRegPerm->reg_properties[0x800+regIndex*8],
        				TIESC_PERM_RW, 5);
        memset((void*)&pRegPerm->reg_properties[0x805+regIndex*8],
        				TIESC_PERM_READ_ONLY, 1);
        memset((void*)&pRegPerm->reg_properties[0x806+regIndex*8],
        				TIESC_PERM_RW, 1);
        memset((void*)&pRegPerm->reg_properties[0x807+regIndex*8],
        				TIESC_PERM_READ_ONLY, 1);
    }
#if 0
    pRegPerm->reg_properties[0x900]= TIESC_PERM_RW;//Receive Time Port0
    pRegPerm->reg_properties[0x901]= TIESC_PERM_RW;//Receive Time Port0
    pRegPerm->reg_properties[0x902]= TIESC_PERM_RW;//Receive Time Port0
    pRegPerm->reg_properties[0x903]= TIESC_PERM_RW;//Receive Time Port0
#else
    for (regIndex=0; regIndex < 4; regIndex++)
        pRegPerm->reg_properties[0x900+regIndex]= TIESC_PERM_RW;
#endif

#if 0
    pRegPerm->reg_properties[0x904]= TIESC_PERM_READ_ONLY;//Receive Time Port1
    pRegPerm->reg_properties[0x905]= TIESC_PERM_READ_ONLY;//Receive Time Port1
    pRegPerm->reg_properties[0x906]= TIESC_PERM_READ_ONLY;//Receive Time Port1
    pRegPerm->reg_properties[0x907]= TIESC_PERM_READ_ONLY;//Receive Time Port1
    pRegPerm->reg_properties[0x908]= TIESC_PERM_READ_ONLY;//Receive Time Port2
    pRegPerm->reg_properties[0x909]= TIESC_PERM_READ_ONLY;//Receive Time Port2
    pRegPerm->reg_properties[0x90A]= TIESC_PERM_READ_ONLY;//Receive Time Port2
    pRegPerm->reg_properties[0x90B]= TIESC_PERM_READ_ONLY;//Receive Time Port2
    pRegPerm->reg_properties[0x90C]= TIESC_PERM_READ_ONLY;//Receive Time Port3
    pRegPerm->reg_properties[0x90D]= TIESC_PERM_READ_ONLY;//Receive Time Port3
    pRegPerm->reg_properties[0x90E]= TIESC_PERM_READ_ONLY;//Receive Time Port3
    pRegPerm->reg_properties[0x90F]= TIESC_PERM_READ_ONLY;//Receive Time Port3
#else
    for (regIndex=0; regIndex < 12; regIndex++)
        pRegPerm->reg_properties[0x904+regIndex]= TIESC_PERM_READ_ONLY;
#endif
#if 0
    pRegPerm->reg_properties[0x910]= TIESC_PERM_RW;//System Time
    pRegPerm->reg_properties[0x911]= TIESC_PERM_RW;//System Time
    pRegPerm->reg_properties[0x912]= TIESC_PERM_RW;//System Time
    pRegPerm->reg_properties[0x913]= TIESC_PERM_RW;//System Time
    pRegPerm->reg_properties[0x914]= TIESC_PERM_READ_ONLY;//System Time
    pRegPerm->reg_properties[0x915]= TIESC_PERM_READ_ONLY;//System Time
    pRegPerm->reg_properties[0x916]= TIESC_PERM_READ_ONLY;//System Time
    pRegPerm->reg_properties[0x917]= TIESC_PERM_READ_ONLY;//System Time
#else
    for (regIndex=0; regIndex < 4; regIndex++)
        pRegPerm->reg_properties[0x910+regIndex]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 4; regIndex++)
        pRegPerm->reg_properties[0x914+regIndex]= TIESC_PERM_READ_ONLY;
#endif

#if 0
    pRegPerm->reg_properties[0x918]= TIESC_PERM_READ_ONLY;//Receive Time ECAT processing
    pRegPerm->reg_properties[0x919]= TIESC_PERM_READ_ONLY;//Receive Time ECAT processing
    pRegPerm->reg_properties[0x91A]= TIESC_PERM_READ_ONLY;//Receive Time ECAT processing
    pRegPerm->reg_properties[0x91B]= TIESC_PERM_READ_ONLY;//Receive Time ECAT processing
#else
    for (regIndex=0; regIndex < 8; regIndex++)
        pRegPerm->reg_properties[0x918+regIndex]= TIESC_PERM_READ_ONLY;
#endif

#if 0
    pRegPerm->reg_properties[0x920]= TIESC_PERM_RW;//System Time Offset
    pRegPerm->reg_properties[0x921]= TIESC_PERM_RW;//System Time Offset
    pRegPerm->reg_properties[0x922]= TIESC_PERM_RW;//System Time Offset
    pRegPerm->reg_properties[0x923]= TIESC_PERM_RW;//System Time Offset
    pRegPerm->reg_properties[0x924]= TIESC_PERM_RW;//System Time Offset
    pRegPerm->reg_properties[0x925]= TIESC_PERM_RW;//System Time Offset
    pRegPerm->reg_properties[0x926]= TIESC_PERM_RW;//System Time Offset
    pRegPerm->reg_properties[0x927]= TIESC_PERM_RW;//System Time Offset

    pRegPerm->reg_properties[0x928]= TIESC_PERM_RW;//System Time Delay
    pRegPerm->reg_properties[0x929]= TIESC_PERM_RW;//System Time Delay
    pRegPerm->reg_properties[0x92A]= TIESC_PERM_RW;//System Time Delay
    pRegPerm->reg_properties[0x92B]= TIESC_PERM_RW;//System Time Delay
#else
    for (regIndex=0; regIndex < 12; regIndex++)
        pRegPerm->reg_properties[0x920+regIndex]= TIESC_PERM_RW;
#endif
    pRegPerm->reg_properties[0x92C]= TIESC_PERM_READ_ONLY;//System Time Difference
    pRegPerm->reg_properties[0x92D]= TIESC_PERM_READ_ONLY;//System Time Difference
    pRegPerm->reg_properties[0x92E]= TIESC_PERM_READ_ONLY;//System Time Difference
    pRegPerm->reg_properties[0x92F]= TIESC_PERM_READ_ONLY;//System Time Difference

    pRegPerm->reg_properties[0x930]= TIESC_PERM_RW;//Speed counter Start
    pRegPerm->reg_properties[0x931]= TIESC_PERM_RW;//Speed counter Start

    pRegPerm->reg_properties[0x932]= TIESC_PERM_READ_ONLY;//Speed counter Diff
    pRegPerm->reg_properties[0x933]= TIESC_PERM_READ_ONLY;//Speed counter Diff

    pRegPerm->reg_properties[0x934]= TIESC_PERM_RW;//System Time Difference Filter Depth
    pRegPerm->reg_properties[0x935]= TIESC_PERM_RW;//Speed counter Filter Depth

    pRegPerm->reg_properties[0x981]= TIESC_PERM_RW;//Sync Activation


    pRegPerm->reg_properties[0x982]= TIESC_PERM_READ_ONLY;//Pulse length of Sync Signals
    pRegPerm->reg_properties[0x983]= TIESC_PERM_READ_ONLY;//Pulse length of Sync Signals
    pRegPerm->reg_properties[0x984]= TIESC_PERM_READ_ONLY;//SYNC Activation status
    pRegPerm->reg_properties[0x98E]= TIESC_PERM_READ_ONLY;//SYNC0 status
    pRegPerm->reg_properties[0x98F]= TIESC_PERM_READ_ONLY;//SYNC1 status

#if 0
    pRegPerm->reg_properties[0x990]= TIESC_PERM_RW;//Start Time Cyclic Operation
    pRegPerm->reg_properties[0x991]= TIESC_PERM_RW;//Start Time Cyclic Operation
    pRegPerm->reg_properties[0x992]= TIESC_PERM_RW;//Start Time Cyclic Operation
    pRegPerm->reg_properties[0x993]= TIESC_PERM_RW;//Start Time Cyclic Operation
    pRegPerm->reg_properties[0x994]= TIESC_PERM_RW;//Start Time Cyclic Operation
    pRegPerm->reg_properties[0x995]= TIESC_PERM_RW;//Start Time Cyclic Operation
    pRegPerm->reg_properties[0x996]= TIESC_PERM_RW;//Start Time Cyclic Operation
    pRegPerm->reg_properties[0x997]= TIESC_PERM_RW;//Start Time Cyclic Operation
#else
    for (regIndex=0; regIndex < 8; regIndex++)
        pRegPerm->reg_properties[0x990+regIndex]= TIESC_PERM_RW;
#endif
#if 0
    pRegPerm->reg_properties[0x998]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
    pRegPerm->reg_properties[0x999]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
    pRegPerm->reg_properties[0x99A]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
    pRegPerm->reg_properties[0x99B]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
    pRegPerm->reg_properties[0x99C]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
    pRegPerm->reg_properties[0x99D]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
    pRegPerm->reg_properties[0x99E]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
    pRegPerm->reg_properties[0x99F]= TIESC_PERM_READ_ONLY;//Next SYNC1 pulse
#else
    for (regIndex=0; regIndex < 8; regIndex++)
        pRegPerm->reg_properties[0x998+regIndex]= TIESC_PERM_READ_ONLY;
#endif
#if 0
    pRegPerm->reg_properties[0x9A0]= TIESC_PERM_RW;//SYNC0 Cycle Time
    pRegPerm->reg_properties[0x9A1]= TIESC_PERM_RW;//SYNC0 Cycle Time
    pRegPerm->reg_properties[0x9A2]= TIESC_PERM_RW;//SYNC0 Cycle Time
    pRegPerm->reg_properties[0x9A3]= TIESC_PERM_RW;//SYNC0 Cycle Time

    pRegPerm->reg_properties[0x9A4]= TIESC_PERM_RW;//SYNC1 Cycle Time
    pRegPerm->reg_properties[0x9A5]= TIESC_PERM_RW;//SYNC1 Cycle Time
    pRegPerm->reg_properties[0x9A6]= TIESC_PERM_RW;//SYNC1 Cycle Time
    pRegPerm->reg_properties[0x9A7]= TIESC_PERM_RW;//SYNC1 Cycle Time

    pRegPerm->reg_properties[0x9A8]= TIESC_PERM_RW;//Latch0 Control
    pRegPerm->reg_properties[0x9A9]= TIESC_PERM_RW;//Latch1 Control
#else
    for (regIndex=0; regIndex < 10; regIndex++)
        pRegPerm->reg_properties[0x9A0+regIndex]= TIESC_PERM_RW;
#endif
#if 0
    pRegPerm->reg_properties[0x9AE]= TIESC_PERM_READ_ONLY;//LATCH0 status
    pRegPerm->reg_properties[0x9AF]= TIESC_PERM_READ_ONLY;//LATCH1 status

    pRegPerm->reg_properties[0x9B0]= TIESC_PERM_READ_ONLY;//Latch0 Time Positive Edge
    pRegPerm->reg_properties[0x9B1]= TIESC_PERM_READ_ONLY;//Latch0 Time Positive Edge
    pRegPerm->reg_properties[0x9B2]= TIESC_PERM_READ_ONLY;//Latch0 Time Positive Edge
    pRegPerm->reg_properties[0x9B3]= TIESC_PERM_READ_ONLY;//Latch0 Time Positive Edge

    pRegPerm->reg_properties[0x9B8]= TIESC_PERM_READ_ONLY;//Latch0 Time Negative Edge
    pRegPerm->reg_properties[0x9B9]= TIESC_PERM_READ_ONLY;//Latch0 Time Negative Edge
    pRegPerm->reg_properties[0x9BA]= TIESC_PERM_READ_ONLY;//Latch0 Time Negative Edge
    pRegPerm->reg_properties[0x9BB]= TIESC_PERM_READ_ONLY;//Latch0 Time Negative Edge

    pRegPerm->reg_properties[0x9C0]= TIESC_PERM_READ_ONLY;//Latch1 Time Positive Edge
    pRegPerm->reg_properties[0x9C1]= TIESC_PERM_READ_ONLY;//Latch1 Time Positive Edge
    pRegPerm->reg_properties[0x9C2]= TIESC_PERM_READ_ONLY;//Latch1 Time Positive Edge
    pRegPerm->reg_properties[0x9C3]= TIESC_PERM_READ_ONLY;//Latch1 Time Positive Edge

    pRegPerm->reg_properties[0x9C8]= TIESC_PERM_READ_ONLY;//Latch1 Time Negative Edge
    pRegPerm->reg_properties[0x9C9]= TIESC_PERM_READ_ONLY;//Latch1 Time Negative Edge
    pRegPerm->reg_properties[0x9CA]= TIESC_PERM_READ_ONLY;//Latch1 Time Negative Edge
    pRegPerm->reg_properties[0x9CB]= TIESC_PERM_READ_ONLY;//Latch1 Time Negative Edge
#else
    for (regIndex=0; regIndex < 34; regIndex++)
        pRegPerm->reg_properties[regIndex+0x9AE]= TIESC_PERM_READ_ONLY;
#endif
    //TI vendor specific registers
    for (regIndex=0; regIndex < 16; regIndex++)
        pRegPerm->reg_properties[0xE00+regIndex]= TIESC_PERM_READ_ONLY;

#if 0
    pRegPerm->reg_properties[0xE10]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE11]= TIESC_PERM_RW;

    pRegPerm->reg_properties[0xE12]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE13]= TIESC_PERM_RW;

    pRegPerm->reg_properties[0xE14]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE15]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE16]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE17]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE18]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE19]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE1A]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE1B]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE1C]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE1D]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE1E]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE1F]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE20]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE21]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE22]= TIESC_PERM_RW;
    pRegPerm->reg_properties[0xE23]= TIESC_PERM_RW;
#else
    //TI vendor specific registers
    for (regIndex=0; regIndex < 20; regIndex++)
        pRegPerm->reg_properties[0xE10+regIndex]= TIESC_PERM_RW;
#endif

    pRegPerm->reg_properties[0xEE0]= TIESC_PERM_RW;//APP_RELOAD_FLAG_REG

    //For debug only
#if PRUSS_END_DEBUG
    //memset ((void*)&pRegPerm->reg_properties[0x14], TIESC_PERM_RW, 32);
    //memset ((void*)&pRegPerm->reg_properties[0xE24], TIESC_PERM_RW, 12);
#endif
    
#ifdef ENABLE_PDI_REG_PERMISSIONS
    memset(&pdi_reg_perm_array[0], 3, 4096);

    for (regIndex=0; regIndex < 10; regIndex++)
        pdi_reg_perm_array[regIndex]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x10]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x12]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 4; regIndex++)
        pdi_reg_perm_array[regIndex+0x100]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x108]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x110]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x120]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x130]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x134]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x140]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 4; regIndex++)
        pdi_reg_perm_array[regIndex+0x150]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x200]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 4; regIndex++)
        pdi_reg_perm_array[regIndex+0x204]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x210]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 4; regIndex++)
        pdi_reg_perm_array[regIndex+0x220]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 13; regIndex++)
        pdi_reg_perm_array[regIndex+0x300]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 4; regIndex++)
        pdi_reg_perm_array[regIndex+0x310]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x400]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x410]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x420]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 4; regIndex++)
        pdi_reg_perm_array[regIndex+0x440]= TIESC_PERM_READ_ONLY;

    pdi_reg_perm_array[0x500]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 15; regIndex++)
        pdi_reg_perm_array[regIndex+0x501]= TIESC_PERM_RW;
    pdi_reg_perm_array[0x510]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 6; regIndex++)
        pdi_reg_perm_array[regIndex+0x511]= TIESC_PERM_RW;
    pdi_reg_perm_array[0x516]= TIESC_PERM_READ_ONLY;
    pdi_reg_perm_array[0x517]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 8; regIndex++) //8 FMMUs
    {
        memset((void*)&pdi_reg_perm_array[0x600+regIndex*16],
        				TIESC_PERM_READ_ONLY, 16);
    }

    for (regIndex=0; regIndex < 8; regIndex++) //8 SMs
    {
        memset((void*)&pdi_reg_perm_array[0x800+regIndex*8],
        				TIESC_PERM_READ_ONLY, 7);
        memset((void*)&pdi_reg_perm_array[0x807+regIndex*8], 
        				TIESC_PERM_RW, 1);
    }

#ifndef SYSTEM_TIME_PDI_CONTROLLED
    for (regIndex=0; regIndex < 27; regIndex++)
        pdi_reg_perm_array[regIndex+0x900]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 22; regIndex++)
        pdi_reg_perm_array[regIndex+0x920]= TIESC_PERM_READ_ONLY;
#else
    for (regIndex=0; regIndex < 16; regIndex++)
        pdi_reg_perm_array[regIndex+0x900]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 8; regIndex++)
        pdi_reg_perm_array[regIndex+0x910]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 8; regIndex++)
        pdi_reg_perm_array[regIndex+0x918]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 12; regIndex++)
        pdi_reg_perm_array[regIndex+0x920]= TIESC_PERM_RW;
    for (regIndex=0; regIndex < 4; regIndex++)
        pdi_reg_perm_array[regIndex+0x92c]= TIESC_PERM_READ_ONLY;
    pdi_reg_perm_array[0x930]= TIESC_PERM_RW;
    pdi_reg_perm_array[0x931]= TIESC_PERM_RW;
    pdi_reg_perm_array[0x932]= TIESC_PERM_READ_ONLY;
    pdi_reg_perm_array[0x933]= TIESC_PERM_READ_ONLY;
    pdi_reg_perm_array[0x934]= TIESC_PERM_RW;
    pdi_reg_perm_array[0x935]= TIESC_PERM_RW;
#endif

    pdi_reg_perm_array[0x981]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x982]= TIESC_PERM_RW;

    pdi_reg_perm_array[0x984]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 2; regIndex++)
        pdi_reg_perm_array[regIndex+0x98E]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 26; regIndex++)
        pdi_reg_perm_array[regIndex+0x990]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 34; regIndex++)
        pdi_reg_perm_array[regIndex+0x9AE]= TIESC_PERM_READ_ONLY;
    //TI vendor specific registers
    for (regIndex=0; regIndex < 8; regIndex++)
        pdi_reg_perm_array[regIndex+0xE00]= TIESC_PERM_READ_ONLY;
    for (regIndex=0; regIndex < 28; regIndex++)
        pdi_reg_perm_array[regIndex+0xE08]= TIESC_PERM_RW;
#endif

#if PRUSS_END_DEBUG
    for (regIndex=0; regIndex < (0xE08 + 28); regIndex++)
            {
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            		"DRAM1 @%x  %x\n",
					regIndex,pRegPerm->reg_properties[regIndex]);
            }
#endif
    
    return OK;
    }


/*******************************************************************************
*
* pruSharedMemTransfer - For PRU Test application, read from the shared memory
*                           Fetch information from the shared memory
*                           - Type of Slave Controller
*                           - Revision ID
*                           - Firmware Build Version
*                           - Write/Read to/from the shared memory
*
* RETURNS: Type of Slave Controller or Revision ID
*           or Firmware Build Version or Write/Read value to/from the shared memory
*
* ERRNO: N/A
*/
STATUS pruSharedMemTransfer
    (
     VXB_DEV_ID pDev,
     int num
    )
    {
    PRUICSS_CTRL  		* pDrvCtrl;
    uint32_t        	  val;
    uint32_t 			* address;
    
    if (pDev == NULL)
        {
        return ERROR;
        }
    
    pDrvCtrl = (PRUICSS_CTRL*)vxbDevSoftcGet (pDev);
    if (pDrvCtrl == NULL)
        {
        return ERROR;
        }

    /* Will read 4 byte at a time */
    
    address = (uint32_t*)pDrvCtrl->pSDram1Regbase;
    
    switch(num)
        {
        case CASE_ESC_TYPE :	
            	val = vxbRead32(pDrvCtrl->pSDram1Handle,address);
            	PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            		"\n\t\t\t Address = Type of Slave Controller : 0x%x "
            		"TI ESC: 0x90\n",
                     ((val >> 24) & 0xFF));
            	break;
                
        case CASE_ESC_REVISION :	
        		val = vxbRead32(pDrvCtrl->pSDram1Handle,address);
        		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, 
        		"\t\t\tRevision ID    : 0x%x  AM335X: 0x1, AM437X: 0x2\n",
                (((uint16_t)val)>>8)&0xFF);
                break;
        
        case CASE_ESC_BUILD_VERSION :	
        		val = vxbRead32(pDrvCtrl->pSDram1Handle,address);
        		PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO, 
        		"\t\t\tFirmware Build Version : 0x%x  AM335x/AM437x : "
        		"0x3B3\n",
                (((uint32_t)val)>>16)&0xFFFF);
                break;
                    
        case CASE_ESC_AL_CONTROL :	
        		val = vxbRead8(pDrvCtrl->pSDram1Handle,(UINT8 *)ESC_AL_CONTROL_OFFSET);
                PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
                				"the al control register"
                			" val is : 0x%x \n",
                val ); 
                break; 
                                                     
                                            
        default:
            val = vxbRead32(pDrvCtrl->pSDram1Handle,address + AL_STATUS);
            
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,
            		"\n\nWRITE to the shared RAM Address: 0x%x, Value:0x%x "
            		"value = %x\n"
                     , address + AL_STATUS, num,val);

            vxbWrite32(pDrvCtrl->pSDram1Handle,address + AL_STATUS, num);
            
            val = vxbRead32(pDrvCtrl->pSDram1Handle,address + AL_STATUS);
            
            PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_INFO,  
            		"\nREAD from the shared RAM Address: 0x%08x, Value:0x%x \n"
                    , address + AL_STATUS, num);

            break;
                
        }
        return val; 
    }


/*******************************************************************************
*
* vxbPruRead - Read the value from the command line of PRU app and
*               pass it to the pruSharedMemTransfer function
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbPruRead
    (
    VXB_DEV_ID          pDev,
    UINT32              offset,
    UINT8 *             buf,
    UINT32              length,
    UINT32 *            realByte
    )
    {
    int     val;
    
    if (pDev == NULL)
        return ERROR;

    val =  pruSharedMemTransfer (pDev, buf[0]);
     
    *realByte = val;
    return OK;
    
    }


/*******************************************************************************
*
* vxbPruWrite - Write the value from the command line of PRU app and
*               pass it to the pruSharedMemTransfer function
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbPruWrite
    (
    VXB_DEV_ID pDev,
    UINT32     addr,
    UINT8 *    pDataBuf,
    UINT32     length,
    UINT32*    realByte
    )
    {
    
    if (pDev == NULL)
        return ERROR;

    pruSharedMemTransfer (pDev, pDataBuf[0]);

    *realByte = length;
    return OK;
    
    }


/*******************************************************************************
*
* ReadByte_Dram_Iep - Read byte from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/

STATUS  ReadByte_Dram_Iep
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t op
    )
	{
	uint8_t val;
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
		    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
	
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			val = vxbRead8(pDrvCtrl->pDram0Handle,((Revaddress + offset)));
			break;
			
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			val = vxbRead8(pDrvCtrl->pDram1Handle,((Revaddress + offset)));
			break;
			
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			val = vxbRead8(pDrvCtrl->pIepHandle,((Revaddress + offset)));
			break;
		
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}
		        
	return val;
	}


/*******************************************************************************
*
* ReadWord_Dram_Iep - Read two bytes(a word) from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  ReadWord_Dram_Iep
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t op
    )
	{
	uint16_t val;
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
		    
		    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
		        
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			val = vxbRead16(pDrvCtrl->pDram0Handle,
					((UINT16 *)(Revaddress + offset)));
			break;
		
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			val = vxbRead16(pDrvCtrl->pDram1Handle,
					((UINT16 *)(Revaddress + offset)));
			break;
		
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			val = vxbRead16(pDrvCtrl->pIepHandle,
					((UINT16 *)(Revaddress + offset)));
			break;
		
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}
	return val;
	}


/*******************************************************************************
*
* ReadDword_Dram_Iep - Read four byte (dword)  from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  ReadDword_Dram_Iep
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t op
    )
	{
	uint32_t val;
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
		    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
		
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			val = vxbRead32(pDrvCtrl->pDram0Handle,
					((UINT32 *)(Revaddress + offset)));
			break;
			
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			val = vxbRead32(pDrvCtrl->pDram1Handle,
					((UINT32 *)(Revaddress + offset)));
			break;
			
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			val = vxbRead32(pDrvCtrl->pIepHandle,
					((UINT32 *)(Revaddress + offset)));
			break;
			
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}
	return val;
	}


/*******************************************************************************
*
* WriteByte_Dram_Iep - Write byte to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS WriteByte_Dram_Iep
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t data, uint8_t op
    )
	{
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
		    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
		
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			vxbWrite8(pDrvCtrl->pDram0Handle,((Revaddress + offset)),data);
			break;
			
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			vxbWrite8(pDrvCtrl->pDram1Handle,((Revaddress + offset)),data);
			break;
			
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			vxbWrite8(pDrvCtrl->pIepHandle,((Revaddress + offset)),data);
			break;
			
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}
	return OK;
	}

/*******************************************************************************
*
* WriteWord_Dram_Iep - Write two byte(word) to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  WriteWord_Dram_Iep
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint16_t data, uint8_t op
    )
	{
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
		    
		    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
	
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			vxbWrite16(pDrvCtrl->pDram0Handle,((UINT16 *)(Revaddress + offset)),data);
			break;
		
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			vxbWrite16(pDrvCtrl->pDram1Handle,((UINT16 *)(Revaddress + offset)),data);
			break;
		
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			vxbWrite16(pDrvCtrl->pIepHandle,((UINT16 *)(Revaddress + offset)),data);
			break;
		
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}
	return OK;
	}


/*******************************************************************************
*
* WriteDword_Dram_Iep - Write four byte(dword) to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  WriteDword_Dram_Iep
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint32_t data, uint8_t op
    )
	{
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
		    
		    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
		        
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			vxbWrite32(pDrvCtrl->pDram0Handle,((UINT32 *)(Revaddress + offset)),data);
			break;
		
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			vxbWrite32(pDrvCtrl->pDram1Handle,((UINT32 *)(Revaddress + offset)),data);
			break;
		
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			vxbWrite32(pDrvCtrl->pIepHandle,((UINT32 *)(Revaddress + offset)),data);
			break;
		
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}
	return OK;
	}


/*******************************************************************************
*
* ReadBlock_Dram_Iep -Read a block from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS ReadBlock_Dram_Iep
(
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op
    )
	{
	uint32_t i;
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
			    
			    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			
			for(i = 0; i < len; i++)
				{
				*(pData + i) = vxbRead8(pDrvCtrl->pDram0Handle,
						((UINT8 *)(Revaddress + offset + i)));
				}
			
			break;
		
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			
			for(i = 0; i < len; i++)
				{
				*(pData + i) = vxbRead8(pDrvCtrl->pDram1Handle,
						((UINT8 *)(Revaddress + offset + i)));
				}
			
			break;
		
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			
			for(i = 0; i < len; i++)
				{
				*(pData + i) = vxbRead8(pDrvCtrl->pIepHandle,
						((UINT8 *)(Revaddress + offset + i)));
				}
			
			break ;
		
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}	
	return OK;
	}


/*******************************************************************************
*
* WriteBlock_Dram_Iep - Write a block to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS WriteBlock_Dram_Iep
	(
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op
    )
	{
	uint32_t i;
	PRUICSS_CTRL *pDrvCtrl = NULL;
	uint8_t * Revaddress;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
			    
	pDrvCtrl = (PRUICSS_CTRL *)vxbDevSoftcGet (pDev);
	
	if (pDrvCtrl == NULL)
		{
		return ERROR;
		}
	
	switch(op)
		{
		case DRAM0:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram0Regbase;
			for(i = 0; i < len; i++)
				{
				vxbWrite8(pDrvCtrl->pDram0Handle,
						((UINT8 *)(Revaddress + offset + i)),(*(pData+i)));
				}
			break;
			
		case DRAM1:
			Revaddress   = (uint8_t *)pDrvCtrl->pDram1Regbase;
			for(i = 0; i < len; i++)
				{
				vxbWrite8(pDrvCtrl->pDram1Handle,
						((UINT8 *)(Revaddress + offset + i)),(*(pData+i)));
				}
			break;
			
		case IEP:
			Revaddress   = (uint8_t *)pDrvCtrl->pIepRegbase;
			for(i = 0; i < len; i++)
				{
				vxbWrite8(pDrvCtrl->pIepHandle,
						((UINT8 *)(Revaddress + offset + i)),(*(pData+i)));
				}
			break ;
			
		default:
			PRUICSS_END_DBG_MSG ( PRUICSS_END_DBG_ERR,
					"Invalid Case\n");
			return INVALID_CASE;
		}	
	return OK;
	}
