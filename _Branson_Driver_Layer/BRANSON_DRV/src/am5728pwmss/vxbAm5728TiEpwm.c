/*
 * vxbTiEpwm.c
 *
 *  Created on: Apr 24, 2017
 *      Author: EE207024
 */

/*
 * Copyright - To be Done
 */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtSpiLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <tickLib.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ioLib.h>
#include <hwif/buslib/vxbFdtI2cLib.h>
#include "types.h"
#include "vxbEpwm.h"
#include "ehrpwm.h"
#include <vxbAm5728EhrPwmLib.h>

/* defines */
/* #define PWMSS_DBG_ON*/
#ifdef  PWMSS_DBG_ON

#define PWMSS_DBG_IRQ         0x00000001
#define PWMSS_DBG_INFO        0x00000002
#define PWMSS_DBG_ERR         0x00000004
#define PWMSS_DBG_ALL         0xffffffff
#define PWMSS_DBG_OFF         0x00000000

LOCAL UINT32 pwmssDbgMask = PWMSS_DBG_ALL;

#define PWMSS_DBG(mask, ...)                              \
	    do                                                                  \
	        {                                                               \
	        if ((pwmssDbgMask & mask) || (mask == PWMSS_DBG_ALL))               \
	            {                                                           \
	            if (_func_kprintf != NULL)                                  \
	                {                                                       \
	                (* _func_kprintf)("%s %s %d\n",__FUNCTION__,__FILE__,__LINE__);                         \
	                (* _func_kprintf)(__VA_ARGS__);                         \
	                }                                                       \
	            }                                                           \
	        }                                                               \
	    while ((FALSE))
#else
#define PWMSS_DBG(...)
#endif  /* PWMSS_DBG_ON */



LOCAL STATUS    am572xEpwmProbe (VXB_DEV_ID);
LOCAL STATUS    am572xEpwmAttach (VXB_DEV_ID);
STATUS			am572xPwmssClockDisable(VXB_DEV_ID, int);
STATUS 			am572xPwmssClockEnable(VXB_DEV_ID, int);
LOCAL VXB_FDT_DEV *     am572xPwmssFdtDevGet (VXB_DEV_ID,
											VXB_DEV_ID
										   );
LOCAL STATUS            am572xPwmssResFree (VXB_DEV_ID,
											VXB_DEV_ID,
											VXB_RESOURCE *
											);
LOCAL VXB_RESOURCE *    am572xPwmssResAlloc (VXB_DEV_ID,
											  VXB_DEV_ID,
											  uint32_t
											);


LOCAL VXB_DRV_METHOD am572xEpwmMethods[] = {
    { VXB_DEVMETHOD_CALL (vxbDevProbe),  (FUNCPTR)am572xEpwmProbe},
    { VXB_DEVMETHOD_CALL (vxbDevAttach), (FUNCPTR)am572xEpwmAttach},
	{ VXB_DEVMETHOD_CALL(vxbFdtDevGet),     (FUNCPTR)am572xPwmssFdtDevGet    },
	{ VXB_DEVMETHOD_CALL(vxbResourceFree),  (FUNCPTR)am572xPwmssResFree      },
	{ VXB_DEVMETHOD_CALL(vxbResourceAlloc), (FUNCPTR)am572xPwmssResAlloc     },	
  	VXB_DEVMETHOD_END
};

VXB_DRV vxbOfam572xEpwmDrv =
    {
    { NULL } ,
	"ti,am5728-pwmss",                 /* Name */
    "TI am572x EPWM Driver",         /* Description */
	VXB_BUSID_FDT,                      /* Class */
    0,                                  /* Flags */
    0,                                  /* Reference count */
	(VXB_DRV_METHOD*)&am572xEpwmMethods  /* Method table */
    };

VXB_DRV_DEF (vxbOfam572xEpwmDrv)

LOCAL const VXB_FDT_DEV_MATCH_ENTRY am572xEpwmMatch[] =
    {
        {
        "ti,am4377-pwmss", /* compatible */
        (void *)NULL
        },
        {} /* Empty terminated list */
    };


/*********************************************************************************
 * 
 * am572xEpwmProbe : This function finds and compare the compatible field from 
 * the dts file.
 * 
 * if found then success , else fail.
 */

LOCAL STATUS am572xEpwmProbe
    (
    struct vxbDev * pDev
    )
    {
	int ret = 0;
	ret = vxbFdtDevMatch (pDev, am572xEpwmMatch, NULL);
	return ret;
    }


/*********************************************************************************
*
* am572xPwmssFdtDevGet - get the FDT bus device information
*
* This routine gets the FDT bus device information for the child nodes of PRU ESC.
*
* The routine vxbFdtDevGet (pDev); would be replaced with the method
* am572xPwmssFdtDevGet (pDev); in the child node.
*
* RETURNS: device information pointer, or NULL if parent is NULL
*
* ERRNO: N/A
*/ 

LOCAL VXB_FDT_DEV * am572xPwmssFdtDevGet
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
* am572xPwmssResFree - free resource allocated by calling vxbResourceAlloc
*
* This routine replaces the child's device resource free method to free the
* resource.
*
* RETURNS: OK, ERROR when free fails or parent is NULL
*
* ERRNO: N/A
*/

LOCAL STATUS am572xPwmssResFree
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
* am572xPwmssResAlloc - allocate resource from device resource list
*
* This routine replaces the child's device resource allocate method to get the
* resource with given ID and type.
*
* RETURNS: resource pointer or NULL if parent is NULL
*
* ERRNO: N/A
*/
LOCAL VXB_RESOURCE * am572xPwmssResAlloc
    (
    VXB_DEV_ID      pDev,
    VXB_DEV_ID      pChild,
    uint32_t        id
    )
    {
    VXB_PRU_ESC_INFO    * pPruDevInfo;
    VXB_RESOURCE        * vxbRes;
    VXB_RESOURCE_ADR    * vxbAdrRes;

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
* pwmChildCreate - create the pwmss devices
*
* This function implements the VxBus gen2 bus attach routine for pwmss
* device instance.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/    
LOCAL STATUS pwmChildCreate
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
    	PWMSS_DBG(PWMSS_DBG_ERR,"Mem alloc failed\n");
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
    	PWMSS_DBG(PWMSS_DBG_ERR,"ERROR: vxbFdtRegGet\n");
        vxbFdtResFree(&pPruEscDevInfo->vxbResList);
        vxbMemFree(pPruEscDevInfo);
        (void) vxbDevDestroy(pCur);
        continue;
        }

    if (vxbFdtIntGet(&pPruEscDevInfo->vxbResList, pFdtDev) == ERROR)
        {
    	PWMSS_DBG(PWMSS_DBG_ERR,"ERROR: vxbFdtIntGet\n");
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


/**
 * This function allocates driver structure, registers and map it to
 * system.It always does the basic hardware and software initialization 
 * needed for driver and hardware functioning. 
 */
LOCAL STATUS am572xEpwmAttach
    (
    VXB_DEV_ID  pDev
    )
    {
	PWMGEN *    			pEpwmObj;
	VXB_RESOURCE_ADR *      pResAdr = NULL;
	VXB_RESOURCE *          pRes    = NULL;
	VXB_RESOURCE_ADR *      pPRCMResAdr = NULL;
	VXB_RESOURCE *          pPRCMRes    = NULL;
	STATUS                  ret     = ERROR;
	VXB_FDT_DEV *       	pFdtDev;
	VXB_DEV_ID          	pCur;
	VXB_PWMSS_FUNC * 		pwmss;
	void             * 		pFdtValue;
	void 			* 		res;
	int 					status = 0;
	int 					result, len;
	int                		offset;
	int 					register_value;
	
	if (pDev == NULL)
		{
		return ERROR;
		}
	
	pFdtDev = vxbFdtDevGet (pDev);
	if (pFdtDev == NULL)
		{
		return ERROR;
		}

	/* create controller driver context structure for core */
	res = (void *)vxFdtPropGet(pFdtDev->offset, "reg", &len);

	if(res != NULL)
		register_value = vxFdt32ToCpu (*(int *) res);
		

	pEpwmObj = (PWMGEN *) vxbMemAlloc (sizeof (PWMGEN));
	if (pEpwmObj == NULL){
		PWMSS_DBG(PWMSS_DBG_INFO,"%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
		goto attach_error;
	}

	/* Configure the pin mux functionality */ 
	/* Pinmux will also be used here to check Register 664h, CTRL_PWMSS in Control Module, 
	 * for enabling TimeBase for PWMSSx instance*/
	/*if (vxbPinMuxEnable (pDev) != OK){
		goto attach_error;
	}*/
	
	/* save pDrvCtrl in VXB_DEVICE structure */
	
	vxbDevSoftcSet (pDev, pEpwmObj);
	
	/* save instance ID */
	
	pEpwmObj->pDev = pDev;
	pRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 0);
	
	if (pRes == NULL){
		goto attach_error;
	}
	
	pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
	
	if (pResAdr == NULL)
		goto attach_error;
	
	pEpwmObj->regHandle = pResAdr->pHandle;
	pEpwmObj->regBase =  (void *)pResAdr->virtAddr;
	
	//# Enable Clocks for PWMSS
	(void) vxbClkEnableAll (pDev);
	
	PWMSS_DBG(PWMSS_DBG_INFO,"REeg 1 address EPWM: %x\n",(UINT32)pEpwmObj->regBase);
	PWMSS_DBG(PWMSS_DBG_INFO,"CSR_READ_4(pDev, 0) = %x\n", CSR_READ_4(pDev, 0x0));
	PWMSS_DBG(PWMSS_DBG_INFO,"CSR_READ_4(pDev, 4) = %x\n", CSR_READ_4(pDev, 0x4));
	PWMSS_DBG(PWMSS_DBG_INFO,"CSR_READ_4(pDev, 8) = %x\n", CSR_READ_4(pDev, 0x8));
	PWMSS_DBG(PWMSS_DBG_INFO,"CSR_READ_4(pDev, C) = %x\n", CSR_READ_4(pDev, 0xC));
	PWMSS_DBG(PWMSS_DBG_INFO,"REeg 1: %x\n",vxbRead32(pEpwmObj->regHandle,(UINT32*)pEpwmObj->regBase));
	PWMSS_DBG(PWMSS_DBG_INFO,"REeg 2: %x\n",vxbRead32(pEpwmObj->regHandle,(UINT32*)pEpwmObj->regBase+0x1));
	PWMSS_DBG(PWMSS_DBG_INFO,"REeg 3: %x\n",vxbRead32(pEpwmObj->regHandle,(UINT32*)pEpwmObj->regBase+0x2));
	PWMSS_DBG(PWMSS_DBG_INFO,"REeg 4: %x\n",vxbRead32(pEpwmObj->regHandle,(UINT32*)pEpwmObj->regBase+0x3));
	PWMSS_DBG(PWMSS_DBG_INFO,"CLKCONFIG Register after ehrpwm clock enabled: %x\n", CSR_READ_4(pDev, 0x8));     
	PWMSS_DBG(PWMSS_DBG_INFO,"CLKCONFIG Register after ehrpwm clock disbled: %x\n",CSR_READ_4(pDev, 0x8));

	/* Creating child nodes */
	if (pwmChildCreate (pDev, pFdtDev) == ERROR)
		{
		PWMSS_DBG(PWMSS_DBG_ERR,"pwmChildCreate() return error\n");
		goto attach_error;
		}
	
	/* Library Register */
	pwmss = (VXB_PWMSS_FUNC *) vxbMemAlloc (sizeof (VXB_PWMSS_FUNC));
	pwmss->Am5728ehrpwmClkConfigDisable	= am572xPwmssClockDisable;
	pwmss->Am5728ehrpwmClkConfigEnable 	= am572xPwmssClockEnable;
	Am5728pwmssRegister(pDev,pwmss,pFdtDev->name);
	
	am572xPwmssClockDisable(pDev,EHRPWM);
	
	return OK;

attach_error:
		if (pRes != NULL)
			(void) vxbResourceFree (pDev, pRes);
		(void)vxbClkDisableAll (pDev);
		(void)vxbPinMuxDisable (pDev);
		vxbMemFree (pEpwmObj);
		return ERROR;
}


/*******************************************************************************
*
* am572xPwmssClockEnable - This functions enables clock for PWMSS subsystem
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS am572xPwmssClockEnable
    (
    VXB_DEV_ID pDev,
	int module
    )
    {
	PWMGEN *    pEpwmObj; 
	uint32_t 	regVal;
	
	if (pDev == NULL)
		return ERROR;

	regVal = CSR_READ_4(pDev, 0x8);
	
	switch (module)
	{
	case EHRPWM:
		/* Enable ePwm */
		regVal |= EPWMCLK_EN;
		regVal &= ~(EPWMCLKSTOP_REQ);
		CSR_WRITE_4(pDev, REG_CLKCONFIG, regVal);
		break;
	
	case EQEP:
		/* Enable eQep */
		regVal |= EQEPCLK_EN;
		CSR_WRITE_4(pDev, REG_CLKCONFIG,regVal);
		break;

	case ECAP:
		/* Enable eCap */
		regVal |= ECAPCLK_EN;
		CSR_WRITE_4(pDev, REG_CLKCONFIG,regVal);
		break;

	default:
		PWMSS_DBG(PWMSS_DBG_ERR,"Invalid Module\n");
		break;
	}
	return OK;
    }



/******************************************************************************
*
* am572xPwmssClockDisable - This functions disables clock for PWMSS subsystem
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS am572xPwmssClockDisable
	(
	VXB_DEV_ID pDev,
	int module
	)
	{
	PWMGEN *    pEpwmObj; 
	uint32_t 	regVal;
	
	if (pDev == NULL)
		return ERROR;
	
	regVal = CSR_READ_4(pDev, REG_CLKCONFIG);
	
	switch (module)
	{
	case EHRPWM:
		/* Disable ePwm */
		regVal |= EPWMCLKSTOP_REQ;
		regVal &= ~(EPWMCLK_EN);
		CSR_WRITE_4(pDev, REG_CLKCONFIG, regVal);
		break;
	
	case EQEP:
		/* Disable eQep */
		regVal |= EQEPCLKSTOP_REQ;
		CSR_WRITE_4(pDev, REG_CLKCONFIG,regVal);
		break;

	case ECAP:
		/* Disable eCap */
		regVal |= ECAPCLKSTOP_REQ;
		CSR_WRITE_4(pDev, REG_CLKCONFIG,regVal);
		break;

	default:
		PWMSS_DBG(PWMSS_DBG_ERR,"Invalid Module\n");
		break;
	}
	return OK;
	}

