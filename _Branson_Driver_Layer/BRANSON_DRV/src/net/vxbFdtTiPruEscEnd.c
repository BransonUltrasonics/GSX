/*
 * PRU-subsystem EtherCAT Slave Controller driver for AM437x IDK
 *	This is parent node for the PRU-ICSS.
 *	This function configures:
 *	1. PRCM (Assert and Deassert Reset)
 *	2. Enable OCP Master Access
 *	3. Enable all the pins and clock.
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
#include <subsys/int/vxbIntLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>

#include <subsys/timer/vxbTimerLib.h>
#include <time.h>
#include <private/timeP.h>

/* Driver Header FIle */
#include "vxbFdtTiPruEscEnd.h"

/* PRU Constants */
#include <PRUESC.h>


/* Driver utility functions */
LOCAL VXB_DRV_METHOD pruEscMethods[] =
    {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),      (FUNCPTR)pruEscProbe        },
    { VXB_DEVMETHOD_CALL(vxbDevAttach),     (FUNCPTR)pruEscAttach       },
    { VXB_DEVMETHOD_CALL(vxbFdtDevGet),     (FUNCPTR)PruEscFdtDevGet    },
    { VXB_DEVMETHOD_CALL(vxbResourceFree),  (FUNCPTR)PruEscResFree      },
    { VXB_DEVMETHOD_CALL(vxbResourceAlloc), (FUNCPTR)PruEscResAlloc     },
    { 0, 0 }
    };

/* VxBus driver definition */
VXB_DRV  pruEscDrv =
    {
    { NULL } ,
    "pruEsc",               /* drvName */
    "PRU ESC e-CAT",        /* Description */
    VXB_BUSID_FDT,          /* Class */
    0,                      /* Flags */
    0,                      /* Reference count */
    pruEscMethods           /* Method table */
    };
VXB_DRV_DEF(pruEscDrv)

/* struct used for matching a device */
LOCAL VXB_FDT_DEV_MATCH_ENTRY pruEscMatch[] =
    {
        {
        "ti,am437x-pruEsc-Controller",  /* compatible */
        NULL,                           /* no configuration */
        },
        {}                              /* Empty terminated list */
    };


/******************************************************************************
*
* pruEscProbe - probe for device presence at specific address
*
* This routine probes for device presence at specific address.
*
* RETURNS: OK if probe passes.  ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS pruEscProbe
    (
    VXB_DEV_ID  pDev
    )
    {
    return vxbFdtDevMatch (pDev, pruEscMatch, NULL);
    }


/******************************************************************************
*
* pruEscAttach - attach PRU ESC device
*
* This is the PRU EtherCAT Slave Controller initialization routine.
*
* RETURNS: OK or ERROR when initialize failed
*
* ERRNO: N/A
*/
LOCAL STATUS pruEscAttach
    (
    VXB_DEV_ID  pDev
    )
    {
	VXB_FDT_DEV         * pFdtDev       = NULL;
    VXB_RESOURCE_ADR    * pResAdr       = NULL;
    VXB_RESOURCE        * pRes          = NULL;
    int                   result, len   = 0;
    int                   regIndex      = 0;
    uint32_t              retVal        = 0;
    
    pPruEscCtrl = vxbMemAlloc (sizeof (PRU_ESC_CTRL));
    if (pPruEscCtrl == NULL)
        {
        DEBUG_MSG ("%s: vxbMemAlloc failed\n", __FUNCTION__);
        return ERROR;
        }

    pFdtDev = vxbFdtDevGet (pDev);
    if (pFdtDev == NULL)
        {
        DEBUG_MSG ("%s: pFdtDev failed\n", __FUNCTION__);
        vxbMemFree (pPruEscCtrl);
        return ERROR;
        }
            
    result = vxFdtPropGet (pFdtDev->offset, "reg", &len);

    /* create connect between pSwCtrl and it's pDev */    
    vxbDevSoftcSet(pDev, pPruEscCtrl);
    pPruEscCtrl->pDev = pDev;

    pRes = vxbResourceAlloc(pPruEscCtrl->pDev, VXB_RES_MEMORY, regIndex);
    if (pRes == NULL)
        {
        DEBUG_MSG ("pRes is NULL\n");
        goto FAIL;
        }

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    if (pResAdr == NULL)
        {
        DEBUG_MSG ("pResAdr is NULL\n");
        goto FAIL;
        }

    pPruEscCtrl->pClkHandle = pResAdr->pHandle;
    pPruEscCtrl->pClkRegbase= (void *)pResAdr->virtual;

    /*TODO: Initialize HW Spinlock */
    if (pruEscHwSpinLockInit(pPruEscCtrl, pDev) != OK)
        {
        DEBUG_MSG("Unable to initialize HW Spinlock\n");
        goto FAIL;
        }

   /* NOTE:
    *   1.Assert and then clear reset.
    *   2.Enable PRUSS CLK.  
    */
    if (pruEscConfigPRCM(pPruEscCtrl) != OK)
        {
        DEBUG_MSG("Unable to configure PRCM\n");
        goto FAIL;
        }

    if (vxbClkEnableAll (pDev) != OK)
        {
        kprintf("ClkEnableAll error\n");
        }

    {
    pRes = vxbResourceAlloc(pPruEscCtrl->pDev, VXB_RES_MEMORY, 1);
        if(pRes == NULL)
            {
            DEBUG_MSG ("pRes is NULL\n");
            goto FAIL;
            }
    
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            DEBUG_MSG ("pResAdr is NULL\n");
            goto FAIL;
            }

        pPruEscCtrl->pCtrlHandle  = pResAdr->pHandle;
        pPruEscCtrl->pCtrlRegbase = (void *)pResAdr->virtual;


    /* Enable OCP Master access */
    if (pruEscOCPMasterAccess(pPruEscCtrl, pDev) != OK)
        {
        DEBUG_MSG("Unable to enable OCP Master access was NULL\n");
        goto FAIL;
        }

    (void) vxbPinMuxEnable (pDev);
    
    /* Creating child nodes */
    if (pruEscChildCreate (pDev, pFdtDev) == ERROR)
        {
        DEBUG_MSG ("pruEscChildCreate() return error\n");
        goto FAIL;
        }
    
    return OK;
        
FAIL:

    if (pRes != NULL)
        {
        (void) vxbResourceFree (pDev, pRes);
        }    
    if (pPruEscCtrl != NULL)
        {
        vxbMemFree (pPruEscCtrl);
        }
    return ERROR;
    }
}



/*******************************************************************************
*
* pruEscChildCreate - create the pruicss devices
*
* This function implements the VxBus gen2 bus attach routine for pruicss
* device instance.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/    
LOCAL STATUS pruEscChildCreate
    (
    VXB_DEV_ID      pDev,
    VXB_FDT_DEV   * pParFdtDev
    )
    {
    VXB_FDT_DEV        * pFdtDev            = NULL;
    VXB_PRU_ESC_INFO   * pPruEscDevInfo     = NULL;
    VXB_DEV_ID           pCur               = NULL;
    int                  offset             = pParFdtDev->offset;

    for (offset = VX_FDT_CHILD(offset); 
         offset > 0;
         offset = VX_FDT_PEER(offset)
        )
    {
    pCur = NULL;

    if (vxbDevCreate (&pCur) != OK)
        continue;

    pPruEscDevInfo = (VXB_PRU_ESC_INFO *)vxbMemAlloc(sizeof(* pPruEscDevInfo));
    if (pPruEscDevInfo == NULL)
        {
        DEBUG_MSG ("Mem alloc failed\n");
        (void) vxbDevDestroy(pCur);
        continue;
        }

    pFdtDev = &pPruEscDevInfo->vxbFdtDev;

    vxbFdtDevSetup(offset, pFdtDev);
    vxbDevNameSet(pCur, pFdtDev->name, FALSE);

    /* Get the device register and interrupt infomation  */

    if (vxbResourceInit(&pPruEscDevInfo->vxbResList) != OK)
        {
        DEBUG_MSG ("ERROR: vxbResourceInit\n");
        (void) vxbDevDestroy (pCur);
        vxbMemFree (pPruEscDevInfo);
        continue;
        }

    if(vxbFdtRegGet(&pPruEscDevInfo->vxbResList, pFdtDev) == ERROR)
        {
        DEBUG_MSG ("ERROR: vxbFdtRegGet\n");
        vxbFdtResFree(&pPruEscDevInfo->vxbResList);
        vxbMemFree(pPruEscDevInfo);
        (void) vxbDevDestroy(pCur);
        continue;
        }

    if (vxbFdtIntGet(&pPruEscDevInfo->vxbResList, pFdtDev) == ERROR)
        {
        DEBUG_MSG ("ERROR: vxbFdtIntGet\n");
        vxbFdtResFree(&pPruEscDevInfo->vxbResList);
        vxbMemFree(pPruEscDevInfo);
        (void) vxbDevDestroy(pCur);
        continue;
        }

    /* Assign the bus internal variable and type  */

    vxbDevIvarsSet(pCur, (void *) pPruEscDevInfo);
    vxbDevClassSet(pCur, VXB_BUSID_FDT);

    /* Add the child devices(PRUICSS 0/1) to PRU ESC(Parent node) */
    vxbDevAdd(pDev, pCur);
    }


    return OK;
    }


/*********************************************************************************
*
* PruEscFdtDevGet - get the FDT bus device information
*
* This routine gets the FDT bus device information for the child nodes of PRU ESC.
*
* The routine vxbFdtDevGet (pDev); would be replaced with the method
* PruEscFdtDevGet (pDev); in the child node.
*
* RETURNS: device information pointer, or NULL if parent is NULL
*
* ERRNO: N/A
*/ 

LOCAL VXB_FDT_DEV * PruEscFdtDevGet
    (
    VXB_DEV_ID      pDev,
    VXB_DEV_ID      pChild
    )
    {
    VXB_PRU_ESC_INFO * pPruDevInfo;
    if (pChild == NULL)
        return NULL;

    pPruDevInfo = (VXB_PRU_ESC_INFO *) vxbDevIvarsGet (pChild);

    if (pPruDevInfo == NULL)
        return NULL;

    return &pPruDevInfo->vxbFdtDev;
    }


/*******************************************************************************
*
* PruEscResFree - free resource allocated by calling vxbResourceAlloc
*
* This routine replaces the child's device resource free method to free the
* resource.
*
* RETURNS: OK, ERROR when free fails or parent is NULL
*
* ERRNO: N/A
*/

LOCAL STATUS PruEscResFree
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
* PruEscResAlloc - allocate resource from device resource list
*
* This routine replaces the child's device resource allocate method to get the
* resource with given ID and type.
*
* RETURNS: resource pointer or NULL if parent is NULL
*
* ERRNO: N/A
*/

LOCAL VXB_RESOURCE * PruEscResAlloc
    (
    VXB_DEV_ID      pDev,
    VXB_DEV_ID      pChild,
    uint32_t        id
    )
    {
    VXB_PRU_ESC_INFO    * pPruDevInfo;
    VXB_RESOURCE        * vxbRes;
    
    pPruDevInfo = (VXB_PRU_ESC_INFO *) vxbDevIvarsGet (pChild);
    if (pPruDevInfo == NULL)
        {
        return NULL;
        }

    vxbRes = vxbResourceFind (&pPruDevInfo->vxbResList, id);

    if (vxbRes == NULL)
        {
        return NULL;
        }

    if (((VXB_RES_TYPE(vxbRes->id) == VXB_RES_MEMORY)
          || (VXB_RES_TYPE(vxbRes->id) == VXB_RES_IO))
          && (vxbRegMap (vxbRes) == OK)
        )
        {
        return vxbRes;
        }
    else if ((VXB_RES_TYPE (vxbRes->id) == VXB_RES_IRQ)
              && (vxbIntMap (vxbRes) == OK)
            )
        {
        return vxbRes;
        }
    else
        {
        return NULL;
        }
    }


/*******************************************************************************
*
* pruEscOCPMasterAccess - Enables PRU ESC OnChip Master Access.
*
* Enabling Master Access lets the Host processor talk to PRU.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS pruEscOCPMasterAccess
    (
    PRU_ESC_CTRL        * pPruEscCtrl,
    VXB_DEV_ID            pDev
    )
    {

    if (pPruEscCtrl == NULL || pDev == NULL)
        {
        DEBUG_MSG("parameter error, NULL\n");
        return ERROR;
        }

    DEBUG_MSG("\n\t\t\tOCP Master Access of PRUICSS is enabled!\n\n");

    uint8_t *   pBase               = (uint8_t*)pPruEscCtrl->pCtrlRegbase;
    uint32_t    mask, shift, retVal = 0;
                mask                = PRUICSS_CFG_SYSCFG_STANDBY_INIT_MASK;
                shift               = PRUICSS_CFG_SYSCFG_STANDBY_INIT_SHIFT;
                
    retVal = vxbRead32(pPruEscCtrl->pCtrlHandle,(UINT32 *)(pBase + PRUICSS_CFG_SYSCFG));
    
    retVal |= (0x1 << shift);
    
    vxbWrite32(pPruEscCtrl->pCtrlHandle,(UINT32 *)(pBase + PRUICSS_CFG_SYSCFG), retVal);
	
    return OK;
    }


/**********************************************************************************
*
* pruEscHwSpinLockInit - Initializes Hardware SpinLock for the peripheral Registers
*                        of Clock Module.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS pruEscHwSpinLockInit
    (
    PRU_ESC_CTRL        * pPruEscCtrl,
    VXB_DEV_ID            pDev
    )
    {

    if (pPruEscCtrl == NULL || pDev == NULL)
        {
        DEBUG_MSG("parameter error, NULL\n");
        return ERROR;
        }

    DEBUG_MSG("\n\n\t\t\tHW Spinlock of PRUICSS is Intialized!\n");

    /* To be done. Clock Module for Peripheral Register is required.
     * Entry to be made in the Device tree.
     */
    #if 0
    vxbWrite32(pPruEscCtrl->pClkHandle,(uint32_t*)(0x44df8d28), 2);
    uint32_t regVal;
    for (regVal=0; regVal < 8; regVal++)
    {
        vxbWrite32(pPruEscCtrl->pClkHandle,
        		(uint32_t*)((0x44df8d28) + (regVal <<2)), 0);
    }
    #endif

    return OK;
    }


/************************************************************************************
*
* pruEscConfigPRCM - Configures Power, Reset and Clock Management modules of PRU ESC
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS    pruEscConfigPRCM
    (
    PRU_ESC_CTRL    * pPruEscCtrl
    )
    {
    uint32_t    retVal  = 0;

    DEBUG_MSG("\n\t\t\tPRCM of PRUICSS is configured!\n");
    
    retVal  = vxbRead32(pPruEscCtrl->pClkHandle,(uint32_t*)pPruEscCtrl->pClkRegbase);
    retVal |= PRCM_ASRT_RST_CTRL;
    retVal &= PRCM_CLR_RST_CTRL;

    vxbWrite32(pPruEscCtrl->pClkHandle,(uint32_t*)pPruEscCtrl->pClkRegbase, retVal);
    
    retVal = vxbRead32(pPruEscCtrl->pClkHandle,(uint32_t*)pPruEscCtrl->pClkRegbase);

    return OK;
    }

