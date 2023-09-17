/*
 * vxbTiEqep.c
 */

/*
 * Copyright - To be Done
 */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <hwif/vxBus.h>
#include <taskLib.h>
#include <sysLib.h>
#include <tickLib.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ioLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <vxbEqepLib.h>
#include <vxbEqep.h>

/* defines */
//#define EQEP_DBG_ON
#ifdef  EQEP_DBG_ON

#define EQEP_DBG_IRQ         0x00000001
#define EQEP_DBG_INFO        0x00000002
#define EQEP_DBG_ERR         0x00000004
#define EQEP_DBG_ALL         0xffffffff
#define EQEP_DBG_OFF         0x00000000

LOCAL UINT32 eqepDbgMask = EQEP_DBG_ALL;

#define EQEP_DBG(mask, ...)                              \
	    do                                                                  \
	        {                                                               \
	        if ((eqepDbgMask & mask) || (mask == EQEP_DBG_ALL))               \
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
#define EQEP_DBG(...)
#endif  /* EQEP_DBG_ON */

//Global EQEP Object

LOCAL EQEPGEN *    pEqepObj;

    
LOCAL STATUS    am437xEqepProbe (VXB_DEV_ID);
LOCAL STATUS    am437xEqepAttach (VXB_DEV_ID);
LOCAL STATUS	readeQEP(VXB_DEV_ID pDev);
LOCAL STATUS	eqepInitialize(VXB_DEV_ID pDev);
LOCAL STATUS	eqepGetMaxCount(VXB_DEV_ID pDev);
LOCAL STATUS	eqepSetMaxCount(VXB_DEV_ID pDev, int max);
LOCAL UINT32	eqepGetPosCount(VXB_DEV_ID pDev);
LOCAL STATUS	eqepSetInitCount(VXB_DEV_ID pDev, int val);
LOCAL STATUS	eqepGetInitCount(VXB_DEV_ID pDev);
LOCAL STATUS	eqepGetDirection(VXB_DEV_ID pDev);
LOCAL void eqepIrqHandler(void *arg);

LOCAL VXB_DRV_METHOD am437xeqepMethods[] = {
    { VXB_DEVMETHOD_CALL (vxbDevProbe),  (FUNCPTR)am437xEqepProbe},
    { VXB_DEVMETHOD_CALL (vxbDevAttach), (FUNCPTR)am437xEqepAttach},
	VXB_DEVMETHOD_END
};

VXB_DRV vxbOfAm437xeqepDrv =
    {
    { NULL } ,
	"ti,am4377-eqep",                 /* Name */
    "TI AM437x EPWM Driver",         /* Description */
	VXB_BUSID_FDT,                      /* Class */
    0,                                  /* Flags */
    0,                                  /* Reference count */
	(VXB_DRV_METHOD*)&am437xeqepMethods  /* Method table */
    };

VXB_DRV_DEF (vxbOfAm437xeqepDrv)

LOCAL const VXB_FDT_DEV_MATCH_ENTRY am437xeqepMatch[] =
    {
        {
        "ti,am437x-eqep", /* compatible */
        (void *)NULL
        },
        {} /* Empty terminated list */
    };


/*************************************************************************
 * 
 * am437xeqepProbe : This function finds and compare the compatible 
 * field from the dts file.
 * 
 * RETURN : if found then success , else fail.
 */

LOCAL STATUS am437xEqepProbe
    (
    struct vxbDev * pDev
    )
    {
	int ret = 0;
	ret = vxbFdtDevMatch (pDev, am437xeqepMatch, NULL);
	return ret;
    }


/***************************************************************************
 * 
 * am437xeqepAttach : This function allocates driver structure, 
 * registers and map it to  system.It always does the basic hardware and 
 * software initialization needed for driver and hardware functioning.
 * 
 * RETURN :  attach success , else fail.
 */

LOCAL STATUS am437xEqepAttach
    (
    VXB_DEV_ID  pDev
    )
	{
	VXB_RESOURCE_ADR *      pResAdr = NULL;
	VXB_RESOURCE *          pRes    = NULL;
	VXB_FDT_DEV *       	pFdtDev;
	VXB_DEV_ID          	pCur;
	VXB_PWMSS_EQEP_FUNC * 		eqep;
	int                		offset;
	void             * 		pFdtValue;
	void 			* 		res;
	STATUS                  ret     = ERROR;
	int 					status = 0;
	int 					result, len;
	int 					register_value;
	int       				strLen   ;
	if (pDev == NULL)
		{
		return ERROR;
		}
	
	pFdtDev = vxbFdtDevGet (pDev);
	if (pFdtDev == NULL)
		{
		return ERROR;
		}
	
	 strLen = strlen(pFdtDev->name);
	/* create controller driver context structure for core */
	
	res = (void *)vxFdtPropGet(pFdtDev->offset, "reg", &len);
	
	if(res != NULL)
		register_value = vxFdt32ToCpu (*(int *) res);
	
	/* Allocate Driver Memory */
	
	pEqepObj = (EQEPGEN *) vxbMemAlloc (sizeof (EQEPGEN));
	
	if (pEqepObj == NULL)
		goto attach_error;
		
	vxbDevSoftcSet (pDev, pEqepObj);
	   
	/* save instance ID */
	
	pEqepObj->pDev = pDev;
	
	/* Allocate resource */
	
	pRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 0);
		
	if (pRes == NULL)
		goto attach_error;
	
	pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
	
	if (pResAdr == NULL)
		goto attach_error;

	pEqepObj->regHandle = pResAdr->pHandle;
	pEqepObj->regBase =  (void *)pResAdr->virtAddr;
	
	pEqepObj->intRes = vxbResourceAlloc (pDev, VXB_RES_IRQ, 0);
	
	if (vxbPinMuxEnable (pDev) != OK){
			goto attach_error;
	}
	
	/* Allocate and register library function for i/o subsytem */
	eqep = (VXB_PWMSS_EQEP_FUNC *) vxbMemAlloc (sizeof (VXB_PWMSS_EQEP_FUNC));
	
	if (eqep == NULL)
		goto attach_error;
	
	eqep->eqepGet = readeQEP;
	eqep->eqepGetMaxCount = eqepGetMaxCount;
	eqep->eqepSetMaxCount = eqepSetMaxCount;
	eqep->eqepGetInitCount = eqepGetInitCount;
	eqep->eqepSetInitCount = eqepSetInitCount;
	eqep->eqepGetPosCount = eqepGetPosCount;
	eqep->eqepGetDirection = eqepGetDirection;
	eqepRegister(pDev,eqep,pFdtDev->name);

	/* enable and connect ISR */
	
	ret = vxbIntConnect (pDev, pEqepObj->intRes, 
					(VOIDFUNCPTR)eqepIrqHandler,
								NULL);		

	if (ret != OK)
		{
		goto attach_error;
		}
	
	ret = vxbIntEnable (pDev, pEqepObj->intRes);
	
	if (ret != OK)
		{
		(void)vxbIntDisconnect (pDev, pEqepObj->intRes);
		return ERROR;
		}

	eqepInitialize(pDev);
	return OK;
	
attach_error:
	if (pEqepObj != NULL)
		vxbMemFree (pEqepObj);


	if (pEqepObj->intRes != NULL)
		vxbResourceFree (pDev, pEqepObj->intRes);
	
	if (pEqepObj->regBase != NULL)
		vxbResourceFree (pDev, pEqepObj->regBase);
	
	if (eqep != NULL)
		vxbMemFree (eqep);
	return ERROR;
	}

/*******************************************************************************
*
* eqepIrqHandler - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL void eqepIrqHandler
	(
	void *arg
	)
	{
	EQEP_DBG(EQEP_DBG_INFO,"%s %s %d\n",__FUNCTION__,__FILE__,__LINE__);
	}

/*******************************************************************************
*
* eqepIrqHandler - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS	eqepInitialize
	(
	VXB_DEV_ID pDev
	)
	{
	UINT16 status = 0;
	UINT32 value = 0;
	UINT64 period = 0;
	UINT32 mode = 0;

    /* decoder control settings */
	status = CSR_READ_2(pDev, QDECCTL);
	status = status & ~QSRC0 & ~QSRC1;
	status = status & ~QAP;
	status = status & ~QBP;
	status = status & ~QIP;
	status = status & ~QSP;
	status = status | SWAP;
	CSR_WRITE_2(pDev, QDECCTL,status);

	CSR_WRITE_4(pDev, QPOSINIT,0);

	CSR_WRITE_4(pDev, QPOSMAX, 0xffffffff);
	
	CSR_WRITE_4(pDev, PWMSS_QPOSCNT,0);

//	period = 1000000000;
//	CSR_WRITE_2(pDev, QUPRD,(uint32_t)(period & 0x00000000FFFFFFFF));
//	
//    status = CSR_READ_2(pDev, QEPCTL);
//    status = status | PHEN | IEL0 | SWI | UTE | QCLM;
    status = 0x1098;
    CSR_WRITE_2(pDev,  QEPCTL,status);

	return OK;
	}

/*******************************************************************************
*
* eqepIrqHandler - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS	readeQEP(VXB_DEV_ID pDev)
{
	UINT32 mode;
	EQEPGEN *    pDrvCtrl;
	VXB_FDT_DEV *       	pFdtDev;

	pFdtDev = vxbFdtDevGet (pDev);
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	mode = CSR_READ_4(pDev,PWMSS_QPOSCNT);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QPOSCNT %x\n", mode);
	
	mode = CSR_READ_2(pDev,PWMSS_QEPSTS);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QEPSTS %x\n", mode);
	
	mode = CSR_READ_2(pDev,PWMSS_QDECCTL);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QDECCTL %x\n", mode);
	
	mode = CSR_READ_4(pDev, PWMSS_QREVID);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QREVID  %x\n", mode);
	
	mode = CSR_READ_4(pDev, PWMSS_QPOSILAT);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QPOSILAT  %x\n", mode);
	
	mode = CSR_READ_2(pDev, PWMSS_QFLG);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QFLG  %x\n", mode);
	
	mode = 0;
	CSR_WRITE_2(pDev,  PWMSS_QCLR, mode);
	
	mode = CSR_READ_4(pDev, PWMSS_QPOSMAX);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QPOSMAX  %x\n", mode);
	return OK;
}

/*******************************************************************************
*
* eqepGetPosCount - read Position counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL UINT32	eqepGetPosCount
	(
	VXB_DEV_ID pDev
	)
	{
	
	EQEPGEN *    	pDrvCtrl;
	VXB_FDT_DEV *   pFdtDev;
	UINT32 			mode;

	pFdtDev = vxbFdtDevGet (pDev);
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	mode = CSR_READ_4(pDev,PWMSS_QPOSCNT);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QPOSCNT  %x\n", mode);
	
	return mode;
	}

/*******************************************************************************
*
* eqepSetMaxCount - set Maximum counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
//TODO Rename - functionality changed to set counter instead of max!
LOCAL STATUS	eqepSetMaxCount
	(
	VXB_DEV_ID pDev, 
	int max
	)
	{
	EQEPGEN *    	pDrvCtrl;
	VXB_FDT_DEV *   pFdtDev;
	UINT32 			mode;

	pFdtDev = vxbFdtDevGet (pDev);
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	CSR_WRITE_4(pDev,PWMSS_QPOSCNT,max);
	
	return OK;
	}

/*******************************************************************************
*
* eqepGetMaxCount - get Maximum counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS	eqepGetMaxCount
	(
	VXB_DEV_ID pDev
	)
	{
	EQEPGEN *    	pDrvCtrl;
	VXB_FDT_DEV *   pFdtDev;
	UINT32 			mode;

	pFdtDev = vxbFdtDevGet (pDev);
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	mode = CSR_READ_4(pDev, PWMSS_QPOSMAX);
	EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QPOSMAX  %x\n", mode);
	
	return mode;
	}

/*******************************************************************************
*
* eqepSetInitCount - set Initial counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS	eqepSetInitCount
	(
	VXB_DEV_ID pDev, 
	int val
	)
	{
	EQEPGEN *    	pDrvCtrl;
	VXB_FDT_DEV *   pFdtDev;
	UINT32 			mode;

	pFdtDev = vxbFdtDevGet (pDev);
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	CSR_WRITE_4(pDev, QPOSINIT,val);
	
	return OK;
	}

/*******************************************************************************
*
* eqepGetInitCount - get Initial counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS	eqepGetInitCount
	(
	VXB_DEV_ID pDev
	)
	{
	EQEPGEN *    	pDrvCtrl;
	VXB_FDT_DEV *   pFdtDev;
	UINT32 			mode;

	pFdtDev = vxbFdtDevGet (pDev);
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	mode = CSR_READ_4(pDev, QPOSINIT);
	EQEP_DBG(EQEP_DBG_INFO,"QPOSINIT  %x\n", mode);
	
	return mode;
	}

/*******************************************************************************
*
* eqepGetDirection - get Initial counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS	eqepGetDirection
	(
	VXB_DEV_ID pDev
	)
	{
	EQEPGEN *    	pDrvCtrl;
	VXB_FDT_DEV *   pFdtDev;
	UINT32 			mode;

	pFdtDev = vxbFdtDevGet (pDev);
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	mode = CSR_READ_2(pDev,PWMSS_QEPSTS);
	mode = mode >> 5;
	mode &=0x1;
	EQEP_DBG(EQEP_DBG_INFO,"Direction %s \n",mode ? "clockwise" : "anti-clockwise");
	
	return mode;
	}
