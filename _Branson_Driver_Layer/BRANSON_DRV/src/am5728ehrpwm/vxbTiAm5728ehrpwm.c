/*
 * vxbTiAm5728ehrpwm.c
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
#include <vxbEpwm.h>
#include <ehrpwm.h>
#include <vxb_pwmss_epwm.h>
#include <vxbAm5728ehrpwmLib.h>

/* defines */
//#define EHRPWM_DBG_ON
#ifdef  EHRPWM_DBG_ON

#define EHRPWM_DBG_IRQ         0x00000001
#define EHRPWM_DBG_INFO        0x00000002
#define EHRPWM_DBG_ERR         0x00000004
#define EHRPWM_DBG_ALL         0xffffffff
#define EHRPWM_DBG_OFF         0x00000000

//LOCAL UINT32 ehrpwmDbgMask = EHRPWM_DBG_ALL;
LOCAL UINT32 ehrpwmDbgMask = EHRPWM_DBG_OFF;

#define EHRPWM_DBG(mask, ...)                              \
	    do                                                                  \
	        {                                                               \
	        if ((ehrpwmDbgMask & mask) || (mask == EHRPWM_DBG_ALL))               \
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
#define EHRPWM_DBG(...)
#endif  /* EHRPWM_DBG_ON */

//Global EHRPWM Object

LOCAL EHRPWMGEN *    pEhrpwmObj;
EHRPWMDRV_IRQ_STRUCT pIrqHandler[6];
//EHRPWMDRV_IRQ_STRUCT pIrqHandler[2];
char    * am5728ehrpwm0  = "am5728ehrpwm0";
char    * am5728ehrpwm2  = "am5728ehrpwm2";
char    * am5728ehrpwm4  = "am5728ehrpwm4";
    
LOCAL STATUS    am572xehrpwmProbe (VXB_DEV_ID);
LOCAL STATUS    am572xehrpwmAttach (VXB_DEV_ID);
LOCAL STATUS Am5728ehrpwmTbTimeBaseClkCfg(VXB_DEV_ID pDev, UINT32 tb);
//#if(0)
LOCAL STATUS Am5728ehrpwmTbPwmDirCfg(VXB_DEV_ID pDev,UINT32 dir);
LOCAL STATUS Am5728ehrpwmTbPwmFreqCfg(VXB_DEV_ID pDev,UINT32 freq,UINT32 dir);
LOCAL STATUS Am5728ehrpwmTbPwmCfg(VXB_DEV_ID pDev,UINT32 freq,UINT32 dir);
//#endif
LOCAL STATUS Am5728ehrpwmTbSetEmulationMode(VXB_DEV_ID pDev);
//#if(0)
LOCAL STATUS Am5728ehrpwmCtrCompAConf(VXB_DEV_ID pDev,UINT32 cmpa);
LOCAL STATUS Am5728ehrpwmCtrCompBConf(VXB_DEV_ID pDev,UINT32 cmpb);
LOCAL STATUS Am5728ehrpwmActionQualifierConfA(VXB_DEV_ID pDev,UINT32 action);
LOCAL STATUS Am5728ehrpwmGetActionQualifierConfA(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmActionQualifierConfB(VXB_DEV_ID pDev,UINT32 action);
LOCAL STATUS Am5728ehrpwmGetActionQualifierConfB(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmGetCtrCompAConf(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmGetCtrCompBConf(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmGetCounterTbCTL(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmGetCounter(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmSetCounter(VXB_DEV_ID pDev,UINT16 counter);
LOCAL STATUS Am5728ehrpwmGetPRDCounter(VXB_DEV_ID pDev);
//#endif
LOCAL STATUS Am5728ehrpwmGetTbSts(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmGetTbPwmCfg(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmGetTbPwmDirCfg(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmEnablePWMInt(VXB_DEV_ID pDev,int period, int mode);
LOCAL STATUS Am5728ehrpwmIntConnect(VXB_DEV_ID pDev,EHRPWMDRV_IRQ_HANDLER irqHandler,void* ehrpwmHandlerArgs,int period,int mode);
LOCAL STATUS Am5728ehrpwmIntStatus(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmDisablePWMInt(VXB_DEV_ID pDev);
LOCAL void Am5728ehrpwmIrqHandler0(void * arg);
LOCAL void Am5728ehrpwmIrqHandler2(void * arg);
LOCAL void Am5728ehrpwmIrqHandler4(void * arg);
//#if(0)
LOCAL STATUS Am5728ehrpwmSetAQSFRC(VXB_DEV_ID pDev,UINT32  action);
LOCAL STATUS Am5728ehrpwmGetAQSFRC(VXB_DEV_ID pDev);
//#endif
LOCAL STATUS Am5728ehrpwmSetTBCTL(VXB_DEV_ID pDev,UINT32  action);
LOCAL STATUS Am5728ehrpwmGetTBCTL(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmPWMIntPeriodSelect(VXB_DEV_ID pDev,UINT32 mode);
LOCAL STATUS Am5728ehrpwmPWMIntEventSelect(VXB_DEV_ID pDev,UINT32 mode);
LOCAL STATUS Am5728ehrpwmPWMGetIntPeriodSelect(VXB_DEV_ID pDev);
LOCAL STATUS Am5728ehrpwmPWMGetIntEventSelect(VXB_DEV_ID pDev);

LOCAL VXB_DRV_METHOD am572xehrpwmMethods[] = {
    { VXB_DEVMETHOD_CALL (vxbDevProbe),  (FUNCPTR)am572xehrpwmProbe},
    { VXB_DEVMETHOD_CALL (vxbDevAttach), (FUNCPTR)am572xehrpwmAttach},
	VXB_DEVMETHOD_END
};

VXB_DRV vxbOfAm572xehrpwmDrv =
    {
    { NULL } ,
	"ti,am5728-ehrpwm",                 /* Name */
    "TI AM572x EPWM Driver",         /* Description */
	VXB_BUSID_FDT,                      /* Class */
    0,                                  /* Flags */
    0,                                  /* Reference count */
	(VXB_DRV_METHOD*)&am572xehrpwmMethods  /* Method table */
    };

VXB_DRV_DEF (vxbOfAm572xehrpwmDrv)

LOCAL const VXB_FDT_DEV_MATCH_ENTRY am572xehrpwmMatch[] =
    {
        {
        "ti,am5728-ehrpwm", /* compatible */
        (void *)NULL
        },
        {} /* Empty terminated list */
    };


/*************************************************************************
 * 
 * am572xehrpwmProbe : This function finds and compare the compatible 
 * field from the dts file.
 * 
 * RETURN : if found then success , else fail.
 */

LOCAL STATUS am572xehrpwmProbe
    (
    struct vxbDev * pDev
    )
    {
	int ret = OK;
	ret = vxbFdtDevMatch (pDev, am572xehrpwmMatch, NULL);
	return ret;
    }


/***************************************************************************
 * 
 * am572xehrpwmAttach : This function allocates driver structure, 
 * registers and map it to  system.It always does the basic hardware and 
 * software initialization needed for driver and hardware functioning.
 * 
 * RETURN :  attach success , else fail.
 */

LOCAL STATUS am572xehrpwmAttach
    (
    VXB_DEV_ID  pDev
    )
	{
	VXB_RESOURCE_ADR *      pResAdr = NULL;
	VXB_RESOURCE *          pRes    = NULL;
	VXB_FDT_DEV *       	pFdtDev = NULL;
	VXB_DEV_ID          	pCur;
	VXB_EHRPWM_FUNC * 		ehrpwm  = NULL;
	int                		offset;
	void* 		            pFdtValue = NULL;
	void* 		            res       = NULL;
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
	
	pEhrpwmObj = (EHRPWMGEN *) vxbMemAlloc (sizeof (EHRPWMGEN));
	
	if (pEhrpwmObj == NULL)
		goto attach_error;
		
	vxbDevSoftcSet (pDev, pEhrpwmObj);
	   
	/* save instance ID */
	
	pEhrpwmObj->pDev = pDev;
	
	/* Allocate resource */
	
	pRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 0);
		
	if (pRes == NULL)
		goto attach_error;
	
	pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
	
	if (pResAdr == NULL)
		goto attach_error;

	pEhrpwmObj->regHandle = pResAdr->pHandle;
	pEhrpwmObj->regBase =  (void *)pResAdr->virtAddr;
	
	pEhrpwmObj->intRes = vxbResourceAlloc (pDev, VXB_RES_IRQ, 0);
	
	/* Configure the pin mux functionality */ 
	/* Pinmux will also be used here to check Register 664h, CTRL_PWMSS in Control Module, 
	 * for enabling TimeBase for PWMSSx instance*/
	if (vxbPinMuxEnable (pDev) != OK){
			goto attach_error;
	}
	/* Allocate and register library function for i/o subsytem */
	ehrpwm = (VXB_EHRPWM_FUNC *) vxbMemAlloc (sizeof (VXB_EHRPWM_FUNC));
	
	if (ehrpwm == NULL)
		goto attach_error;
	
	ehrpwm->Am5728ehrpwmSetTbClk = Am5728ehrpwmTbTimeBaseClkCfg;
//#if(0)
	ehrpwm->Am5728ehrpwmSetTbPWMFreq = Am5728ehrpwmTbPwmFreqCfg;
	ehrpwm->Am5728ehrpwmSetTbPWMDir = Am5728ehrpwmTbPwmDirCfg;
	ehrpwm->Am5728ehrpwmSetTbPWMCfg = Am5728ehrpwmTbPwmCfg;
	ehrpwm->Am5728ehrpwmSetTbCOMPAConf = Am5728ehrpwmCtrCompAConf;
	ehrpwm->Am5728ehrpwmSetTbCOMPBConf = Am5728ehrpwmCtrCompBConf;
	ehrpwm->Am5728ehrpwmSetTbAQConfA = Am5728ehrpwmActionQualifierConfA;
	ehrpwm->Am5728ehrpwmSetTbAQConfB = Am5728ehrpwmActionQualifierConfB;
//#endif
	ehrpwm->Am5728ehrpwmGetTbPWMCfg = Am5728ehrpwmGetTbPwmCfg;
//#if(0)
	ehrpwm->Am5728ehrpwmGetTbAQConfA = Am5728ehrpwmGetActionQualifierConfA;
	ehrpwm->Am5728ehrpwmGetTbAQConfB = Am5728ehrpwmGetActionQualifierConfB;
	ehrpwm->Am5728ehrpwmGetTbCOMPAConf = Am5728ehrpwmGetCtrCompAConf;
	ehrpwm->Am5728ehrpwmGetTbCOMPBConf = Am5728ehrpwmGetCtrCompBConf;
	ehrpwm->Am5728ehrpwmGetTbClk = Am5728ehrpwmGetCounterTbCTL;
	ehrpwm->Am5728ehrpwmGetCounter = Am5728ehrpwmGetCounter;
	ehrpwm->Am5728ehrpwmSetCounter = Am5728ehrpwmSetCounter;
	ehrpwm->Am5728ehrpwmGetPRD = Am5728ehrpwmGetPRDCounter;
//#endif
	ehrpwm->Am5728ehrpwmGetTbStatus = Am5728ehrpwmGetTbSts;
	ehrpwm->Am5728ehrpwmGetTbPWMDir = Am5728ehrpwmGetTbPwmDirCfg;
	ehrpwm->Am5728ehrpwmPWMRegisterISR = Am5728ehrpwmIntConnect;
	ehrpwm->Am5728ehrpwmGetIntStatus = Am5728ehrpwmIntStatus;
	ehrpwm->Am5728ehrpwmEnableInt = Am5728ehrpwmEnablePWMInt;
	ehrpwm->Am5728ehrpwmDisableInt = Am5728ehrpwmDisablePWMInt;
	ehrpwm->Am5728ehrpwmSetEmulation = Am5728ehrpwmTbSetEmulationMode;
//#if(0)
	ehrpwm->Am5728ehrpwmSetAQSFRC = Am5728ehrpwmSetAQSFRC;
	ehrpwm->Am5728ehrpwmGetAQSFRC = Am5728ehrpwmGetAQSFRC;
//#endif
	ehrpwm->Am5728ehrpwmSetTBCTL = Am5728ehrpwmSetTBCTL;
	ehrpwm->Am5728ehrpwmGetTBCTL = Am5728ehrpwmGetTBCTL;
	ehrpwm->Am5728ehrpwmSetIntEvtTBCTL = Am5728ehrpwmPWMIntEventSelect;
	ehrpwm->Am5728ehrpwmSetIntPrdTBCTL = Am5728ehrpwmPWMIntPeriodSelect;
	ehrpwm->Am5728ehrpwmGetIntEvtTBCTL = Am5728ehrpwmPWMGetIntEventSelect;
	ehrpwm->Am5728ehrpwmGetIntPrdTBCTL = Am5728ehrpwmPWMGetIntPeriodSelect;
	Am5728ehrpwmRegister(pDev,ehrpwm,pFdtDev->name);
	
	/* enable and connect ISR */
		
	if (pEhrpwmObj->intRes != NULL)
		{
		EHRPWM_DBG(EHRPWM_DBG_INFO,"\n");
		if(strncmp(pFdtDev->name, am5728ehrpwm0, strLen) == 0)
			{
			ret = vxbIntConnect (pDev, pEhrpwmObj->intRes, 
							(VOIDFUNCPTR)Am5728ehrpwmIrqHandler0,
										NULL);
			}
		if(strncmp(pFdtDev->name, am5728ehrpwm2, strLen) == 0)
			{
			ret = vxbIntConnect (pDev, pEhrpwmObj->intRes, 
							(VOIDFUNCPTR)Am5728ehrpwmIrqHandler2,
										NULL);
			}
		if(strncmp(pFdtDev->name, am5728ehrpwm4, strLen) == 0)
			{
			ret = vxbIntConnect (pDev, pEhrpwmObj->intRes, 
							(VOIDFUNCPTR)Am5728ehrpwmIrqHandler4,
										NULL);
			}
		
		
		if (ret != OK)
			{
			goto attach_error;
			}
		
		ret = vxbIntEnable (pDev, pEhrpwmObj->intRes);
		
		if (ret != OK)
			{
			(void)vxbIntDisconnect (pDev, pEhrpwmObj->intRes);
			return ERROR;
			}
		}
	
	return OK;
	
attach_error:
	if (pEhrpwmObj != NULL)
		vxbMemFree (pEhrpwmObj);


	if (pEhrpwmObj->intRes != NULL)
		vxbResourceFree (pDev, pEhrpwmObj->intRes);
	
	if (pEhrpwmObj->regBase != NULL)
		vxbResourceFree (pDev, pEhrpwmObj->regBase);
	
	if (ehrpwm != NULL)
		vxbMemFree (ehrpwm);
	return ERROR;
	}


/*******************************************************************************
*
* Am5728ehrpwmTbTimeBaseClkCfg - set the value of the current time base clock
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS Am5728ehrpwmTbTimeBaseClkCfg
	(
	VXB_DEV_ID pDev,
	UINT32 clkDiv
	)
	{
	/* Configure Time base clock */
	EHRPWMGEN *    pDrvCtrl;
	UINT32 hspClkDiv = 0U;
	UINT32 lspClkDiv = 0U;
	UINT32 lspClkDivSetting = 0U;
	UINT32 regVal = 0U;
	UINT32 DivVal = 1U;
	
	if (pDev == NULL)
		return ERROR;
	
	if (clkDiv > EPWM_TBCTL_HSPCLKDIV_14)
	    {
		/* High speed Time-base clock pre scale value */
		hspClkDiv = PWMSS_EPWM_TBCTL_HSPCLKDIV_DIV_14;
		/* Time-base clock pre scale value */
		lspClkDiv = clkDiv/EPWM_TBCTL_HSPCLKDIV_14;

		
		while(lspClkDiv > 1U)
		{
		   lspClkDiv = lspClkDiv >> 1U;
		   lspClkDivSetting++;
		}
		DivVal = hspClkDiv * lspClkDivSetting;
		if(DivVal != 0)
		{
			pEhrpwmObj->tbclk = SOC_EHRPWM_2_MODULE_FREQ / DivVal;
		}
		else
			{
				pEhrpwmObj->tbclk = SOC_EHRPWM_2_MODULE_FREQ;
			}
	    }
	else
		{
		/* High speed Time-base clock pre scale value */
		hspClkDiv = clkDiv / 2U;
		/* divide by 1 */
		lspClkDivSetting = PWMSS_EPWM_TBCTL_HSPCLKDIV_DIV_1;
		DivVal = hspClkDiv * lspClkDivSetting;
			if(DivVal != 0)
			{
				pEhrpwmObj->tbclk = 
					(SOC_EHRPWM_2_MODULE_FREQ / DivVal);
			}
			else
			{
				pEhrpwmObj->tbclk = SOC_EHRPWM_2_MODULE_FREQ;
			}
		}
	
	regVal = CSR_READ_2(pDev,PWMSS_EPWM_TBCTL);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CLKDIV, lspClkDivSetting);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_HSPCLKDIV, hspClkDiv);
	CSR_WRITE_2(pDev,PWMSS_EPWM_TBCTL, (UINT16)regVal);
	
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmTbPwmFreqCfg - set the value of the current PWM Frequency
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS Am5728ehrpwmTbPwmFreqCfg
	(
	VXB_DEV_ID pDev,
	UINT32 tbPeriodCount,
	UINT32 dir
	)
	{
	/* Configure PWM time base counter frequency 
	 * NOTE: Only to be used when directio is up or down.Cannot be used for both
	 * For Up-Down both mode use function Am5728ehrpwmTbPwmCfg
	 */
	
	UINT32 regVal = 0U;
	
	if (pDev == NULL)
		return ERROR;
	
	if(dir == EPWM_TB_COUNTER_DIR_UP_DOWN)
		regVal = (UINT16)(tbPeriodCount - 1U) / 2U;
	else
		regVal = (UINT16)(tbPeriodCount - 1U);
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBPRD, (UINT16)regVal);
	
	return OK;
    }

/*******************************************************************************
*
* Am5728ehrpwmTbPwmDirCfg - set the value of the current PWM Counter Direction
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmTbPwmDirCfg
	(
	VXB_DEV_ID pDev,
	UINT32 counterDir
	)
	{
	/* Configure PWM time base counter direction */
	UINT32 regVal = 0U;
	
	if (pDev == NULL)
		return ERROR;
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_PRDLD, EPWM_SHADOW_REG_CTRL_ENABLE);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE, counterDir);
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)regVal);
	
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmTbPwmCfg - get the value of the current PWM Counter Direction
* 						& Period
*
* Configure PWM time base counter direction and period 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmTbPwmCfg
	(
	VXB_DEV_ID pDev,
	UINT32 tbPeriodCount,
	UINT32 counterDir
	)
	{
	UINT32 regVal = 0U;

	if (pDev == NULL)
		return ERROR;
	
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_PRDLD, EPWM_SHADOW_REG_CTRL_ENABLE);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE, counterDir);
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)regVal);
	
	regVal = (UINT16)(tbPeriodCount);
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBPRD, (UINT16)regVal);
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmTbPwmCfg - get the value of the current PWM Counter Direction
* 						& Frequency
*
* Configure PWM time base counter direction and frequency 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetTbPwmCfg
	(
	VXB_DEV_ID pDev
	)
	{
	UINT32 regVal = 0U;

	if (pDev == NULL)
		return ERROR;
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Direction : %x\n",
			CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE));
	EHRPWM_DBG(EHRPWM_DBG_INFO,"PWM Period: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_TBPRD));
	return OK;
	}

/*******************************************************************************
*
* m5728ehrpwmGetTbPwmDirCfg - get the value of the current PWM Counter Direction
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*
*/

STATUS Am5728ehrpwmGetTbPwmDirCfg
	(
	VXB_DEV_ID pDev
	)
	{
	UINT32 regVal = 0U;

	if (pDev == NULL)
		return ERROR;
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Direction : %x\n",
			CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE));
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmTbSetEmulationMode - set the emulation mode
*
*
* These bits select the behavior of the ePWM time-base counter
* during emulation suspend events.
* Emulation debug events can be set up in the Debug Subsystem.
* 0h (R/W) = Stop after the next time-base counter increment or
* decrement
* 1h (R/W) = Stop when counter completes a whole cycle. (a) Upcount
* mode: stop when the time-base counter = period (TBCNT =
* TBPRD). (b) Down-count mode: stop when the time-base counter =
* 0000 (TBCNT = 0000h). (c) Up-down-count mode: stop when the
* time-base counter = 0000 (TBCNT = 0000h).
* 2h (R/W) = Free run
* 3h (R/W) = Free run
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmTbSetEmulationMode(VXB_DEV_ID pDev)
	{
	UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);

	if (pDev == NULL)
		return ERROR;
	
	regVal &= (~PWMSS_EPWM_TBCTL_FREE_SOFT_MASK);
	regVal |= (PWMSS_EPWM_TBCTL_FREE_SOFT_STOP_AFTER_CYCLE << PWMSS_EPWM_TBCTL_FREE_SOFT_SHIFT) & PWMSS_EPWM_TBCTL_FREE_SOFT_MASK;
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)regVal);
	return OK;
	}

/******************************************************************************
*
* Am5728ehrpwmCtrCompAConf - set the CMPA value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmCtrCompAConf
	(
	VXB_DEV_ID pDev,
	UINT32   cmpa
	)
	{
	/* Counter Comparator A configuration */
	UINT32 regVal =
	CSR_READ_2(pDev, PWMSS_EPWM_CMPCTL);

	if (pDev == NULL)
		return ERROR;
	
	/*  LOADAMODE:
	 *  Active Counter-Compare A (CMPA) Load From Shadow Select
		Mode.
		This bit has no effect in immediate mode (CMPCTL[SHDWAMODE]
		= EPWM_SHADOW_REG_CTRL_DISABLE).
	 *  0h (R/W) = Load on CTR = 0: Time-base counter equal to zero
		(TBCNT = 0000h)
		1h (R/W) = Load on CTR = PRD: Time-base counter equal to period
		(TBCNT = TBPRD)
		2h (R/W) = Load on either CTR = 0 or CTR = PRD
		3h (R/W) = Freeze (no loads possible)
	 */
	/*  SHADOWMODE:
	 *  Counter-compare A (CMPA) Register Operating Mode.
		0h (R/W) = Shadow mode. Operates as a double buffer. All writes
		via the CPU access the shadow register.
		1h (R/W) = Immediate mode. Only the active compare register is
		used. All writes and reads directly access the active register for
		immediate compare action
	 */
	
	CSR_SET_FIELD(regVal,
	PWMSS_EPWM_CMPCTL_SHDWAMODE, EPWM_SHADOW_REG_CTRL_ENABLE);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_CMPCTL_LOADAMODE,
			EPWM_CC_CMP_LOAD_MODE_CNT_EQ_PRD);
	CSR_WRITE_2(pDev,PWMSS_EPWM_CMPCTL,	(UINT16)regVal);
	
	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPA);
	regVal &= (~PWMSS_EPWM_CMPA_MASK);
	regVal |= ( cmpa << PWMSS_EPWM_CMPA_SHIFT) & PWMSS_EPWM_CMPA_MASK;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_CMPA, (UINT16)regVal);
	return OK;
	}


/******************************************************************************
*
* Am5728ehrpwmGetCtrCompAConf - print the value for CMPA
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetCtrCompAConf
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Comparator A: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_CMPA));
	return OK;
	}


/******************************************************************************
*
* Am5728ehrpwmCtrCompBConf - set the CMPB value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmCtrCompBConf
	(
	VXB_DEV_ID pDev,
	UINT32   cmpb
	)
	{
	/* Counter Comparator B configuration */
	UINT32 regVal =
	CSR_READ_2(pDev, PWMSS_EPWM_CMPCTL);

	if (pDev == NULL)
		return ERROR;
	
	/*  LOADAMODE:
		Active Counter-Compare B (CMPB) Load From Shadow Select
		Mode.
		This bit has no effect in immediate mode (CMPCTL[SHDWBMODE]
		= 1).
		0h (R/W) = Load on CTR = 0: Time-base counter equal to zero
		(TBCNT = 0000h)
		1h (R/W) = Load on CTR = PRD: Time-base counter equal to period
		(TBCNT = TBPRD)
		2h (R/W) = Load on either CTR = 0 or CTR = PRD
		3h (R/W) = Freeze (no loads possible)
	 */
	/*  SHADOWMODE:
	 *  Counter-compare B (CMPB) Register Operating Mode.
		0h (R/W) = Shadow mode. Operates as a double buffer. All writes
		via the CPU access the shadow register.
		1h (R/W) = Immediate mode. Only the active compare B register is
		used. All writes and reads directly access the active register for
		immediate compare action.
	 */
	
	CSR_SET_FIELD(regVal,
	PWMSS_EPWM_CMPCTL_SHDWBMODE, EPWM_SHADOW_REG_CTRL_ENABLE);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_CMPCTL_LOADBMODE,
			EPWM_CC_CMP_LOAD_MODE_CNT_EQ_PRD);
	
	CSR_WRITE_2(pDev,PWMSS_EPWM_CMPCTL,	(UINT16)regVal);
	
	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPB);
	regVal &= (~PWMSS_EPWM_CMPB_MASK);
	regVal |= ( cmpb << PWMSS_EPWM_CMPB_SHIFT) & PWMSS_EPWM_CMPB_MASK;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_CMPB, (UINT16)regVal);
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmGetCtrCompBConf - print the value for CMPB
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetCtrCompBConf
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Comparator B: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_CMPB));
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmActionQualifierConfA - set the action qualifier for ehrpwm0A
* 
* This function sets the actions when the counter reaches 0, TBPRD, CMPA when 
* counter is incrementing, CMPA when counter is decrementing , CMPB when 
* counter is incrementing & CMPB when counter is decrementing for EHRPWM0A.
* They each occupy 2 bits and following values can be set for each of them.  
* Possible values:0h (R/W) = Do nothing (action disabled)
*	1h (R/W) = Clear - force EPWMxA output low.
*	2h (R/W) = Set - force EPWMxA output high.
*	3h (R/W) = Toggle EPWMxA output - low output signal will be forced
* It is a 16 bits register . The order is 
* x x CBD  CBU CAD CAU PRD ZRO	
*	Ex. To toggle when counter reaches CMPB when counter is decrementing,
*	the value to set is  0x0c00
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmActionQualifierConfA
	(
	VXB_DEV_ID pDev,
	UINT32  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	/*TODO: Configure Action Qualifier - toggle if any case happens*/
	CSR_WRITE_2(pDev, PWMSS_EPWM_AQCTLA, (UINT16)action);
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmActionQualifierConfA - print the action qualifier for ehrpwm0A
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetActionQualifierConfA
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Action Qualifier A: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_AQCTLA));
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmActionQualifierConfB - set the action qualifier for ehrpwm0B
* 
* This function sets the actions when the counter reaches 0, TBPRD, CMPA when 
* counter is incrementing, CMPA when counter is decrementing , CMPB when 
* counter is incrementing & CMPB when counter is decrementing for EHRPWM0B.
* They each occupy 2 bits and following values can be set for each of them.  
* Possible values:0h (R/W) = Do nothing (action disabled)
*	1h (R/W) = Clear - force EPWMxB output low.
*	2h (R/W) = Set - force EPWMxB output high.
*	3h (R/W) = Toggle EPWMxB output - low output signal will be forced
* It is a 16 bits register . The order is 
* x x CBD  CBU CAD CAU PRD ZRO	
*	Ex. To toggle when counter reaches CMPA when counter is decrementing,
*	the value to set is  0x00c0
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmActionQualifierConfB
	(
	VXB_DEV_ID pDev,
	UINT32  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	/*TODO: Configure Action Qualifier - toggle if any case happens */
	CSR_WRITE_2(pDev, PWMSS_EPWM_AQCTLB, (UINT16)action);
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmActionQualifierConfB - print the action qualifier for ehrpwm0B
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetActionQualifierConfB
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Action Qualifier B: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_AQCTLB));
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmGetCounterTbCTL - print the COUTER MODE(direction of counter)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetCounterTbCTL
	(
	VXB_DEV_ID pDev
	)
	{
	UINT32 regVal;

	if (pDev == NULL)
		return ERROR;
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Direction of Counter : %x \n",
			CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE));
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmGetCounter - print the TBCNT
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetCounter
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Current Counter: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_TBCNT));
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmSetCounter - set the TBCNT
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmSetCounter
	(
	VXB_DEV_ID pDev,
	UINT16 counter
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCNT,counter);
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmGetPRDCounter - print the TBPRD
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetPRDCounter
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"PWM PRD: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_TBPRD));
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmGetTbSts - print the time base status register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetTbSts
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Time Base Status: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_TBSTS));
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmTbEnablePWMInt - Enables PWM Interrupt generation and set at which event 
* 						to generate interrupt.
*
* This routine enables the PWM interrupt generation and set at what event the 
* interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS Am5728ehrpwmEnablePWMInt
	(
	VXB_DEV_ID pDev,
	int period,
	int mode
	)
	{
	UINT32 regVal;
	EHRPWMGEN *    	pDrvCtrl;
	int 			ret;
	
	if (pDev == NULL)
		return ERROR;
	
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	ret = vxbIntEnable (pDev, pDrvCtrl->intRes);
	
	if (ret != OK)
		{
		(void)vxbIntDisconnect (pDev, pDrvCtrl->intRes);
		return ERROR;
		}
	
	/* Generate an interrupt on the first event INTCNT  */
	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);
	switch (period)
	{
	case ETPS_INTPRD_DISABLE :
		// Disable the interrupt event counter
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_DISABLE);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
		
	case ETPS_INTPRD_GEN_FIRST_EVT:
		// Generate an interrupt on the first event INTCNT = 01 (first event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_FIRST_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
		
	case ETPS_INTPRD_GEN_SECOND_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,0 (second	event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_SECOND_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
		
	case ETPS_INTPRD_GEN_THIRD_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,1 (third event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_THIRD_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
	
	}
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
	
	/* Enable ePWM Interrupt (EPWMx_INT) Generation
	 * & Enable event time-base counter equal to zero
	 */
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETSEL);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTEN, PWMSS_EPWM_ETSEL_INTEN_ENABLE);
	switch (mode)
	{
	case TBCNT_EQ_ZERO:
		// Enable event time-base counter equal to zero
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_TBPRD:
		// Enable event time-base counter equal to TIME PERIOD
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_PERIOD);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPA:
		// Enable event time-base counter equal to cmpa while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPA:
		// Enable event time-base counter equal to cmpa while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPB:
		// Enable event time-base counter equal to cmpb while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPB:
		// Enable event time-base counter equal to cmpb while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
	}
	//CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmTbDisablePWMInt - Disable PWM interrupt
*
* This routine disables the PWM interrupt generation
*  
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
LOCAL STATUS Am5728ehrpwmDisablePWMInt
	(
	VXB_DEV_ID pDev
	)
	{
	EHRPWMGEN *    	pDrvCtrl;
	UINT32 		regVal;
	int 			ret;
	
	
	if (pDev == NULL)
		return ERROR;
	
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	/* Generate an interrupt on the first event INTCNT  */
	regVal = 0;
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
	
	/* Enable ePWM Interrupt (EPWMx_INT) Generation
	 * & Enable event time-base counter equal to zero
	 */
	regVal = 0;
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
	
	CSR_WRITE_2(pDev,PWMSS_EPWM_TBCTL, (UINT16)0);
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCNT,0);
	
    ret = vxbIntDisable (pDev, pDrvCtrl->intRes);
	
	
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmIntConnect - Registers the user application ISR
*
* This routine stores the function pointer from the user application to the 
* global function pointer object in the driver and gets called when interrupt
* is triggered.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmIntConnect
	(
	VXB_DEV_ID pDev,
	EHRPWMDRV_IRQ_HANDLER irqHandler,
	void* ehrpwmHandlerArgs,
	int period,
	int mode
	)
	{
	EHRPWMGEN *    pDrvCtrl;
	VXB_FDT_DEV *       	pFdtDev;
	int strLen;
    if (pDev == NULL || irqHandler == NULL)
        {
        return ERROR;
        }
   
    pFdtDev = vxbFdtDevGet (pDev);
	if (pFdtDev == NULL)
		{
		return ERROR;
		}

	strLen = strlen(pFdtDev->name);
    /* Assign the user ISR */
    if(strncmp(pFdtDev->name, am5728ehrpwm0, strLen) == 0)
		{
    	pIrqHandler[0].func = irqHandler;
    	pIrqHandler[0].inst = pDev;
		}
	if(strncmp(pFdtDev->name, am5728ehrpwm2, strLen) == 0)
		{
		pIrqHandler[2].func = irqHandler;
		pIrqHandler[2].inst = pDev;
		}
	if(strncmp(pFdtDev->name, am5728ehrpwm4, strLen) == 0)
		{
		pIrqHandler[4].func = irqHandler;
		pIrqHandler[4].inst = pDev;
		}
    
    
 
    /* Enable PWM Interrupts */
   // ehrpwmEnablePWMInt(pDev,ETPS_INTPRD_GEN_FIRST_EVT,TBCNT_EQ_ZERO);
	 Am5728ehrpwmEnablePWMInt(pDev,period,mode);

    return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmIntStatus - print the Interrupt status register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmIntStatus
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Enable ePWM Interrupt (EPWMx_INT) Generation %x\n",
			CSR_READ_2(pDev, PWMSS_EPWM_ETSEL));
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Event-Trigger Prescale Register %x\n",
			CSR_READ_2(pDev, PWMSS_EPWM_ETPS));
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Event-Trigger Flag Register %x\n",
			CSR_READ_2(pDev, PWMSS_EPWM_ETFLG));
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Event-Trigger Clear Register %x\n",
			CSR_READ_2(pDev, PWMSS_EPWM_ETCLR));
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Event-Trigger Force Register %x\n",
			CSR_READ_2(pDev, PWMSS_EPWM_ETFRC));
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmIrqHandler0 - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL void Am5728ehrpwmIrqHandler0
	(
	void *arg
	)
	{
	CSR_WRITE_2(pIrqHandler[0].inst,PWMSS_EPWM_ETCLR,1);
	(*pIrqHandler[0].func)();
	}

/*******************************************************************************
*
* Am5728ehrpwmIrqHandler2 - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL void Am5728ehrpwmIrqHandler2
	(
	void *arg
	)
	{
	CSR_WRITE_2(pIrqHandler[2].inst,PWMSS_EPWM_ETCLR,1);
	(*pIrqHandler[2].func)();
	}

/*******************************************************************************
*
* Am5728ehrpwmIrqHandler4 - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL void Am5728ehrpwmIrqHandler4
	(
	void *arg
	)
	{
	CSR_WRITE_2(pIrqHandler[4].inst,PWMSS_EPWM_ETCLR,1);
	(*pIrqHandler[4].func)();
	}

/*******************************************************************************
*
* Am5728ehrpwmSetAQSFRC - set Software Force & also set Action when One-Time 
* Software Force A & B Is invoked
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmSetAQSFRC
	(
	VXB_DEV_ID pDev,
	UINT32  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	/*TODO: Configure Action Qualifier - toggle if any case happens*/
	CSR_WRITE_2(pDev, PWMSS_EPWM_AQSFRC, (UINT16)action);
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmGetAQSFRC - print the AQSFRC register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetAQSFRC
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"AQSFRC A: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_AQSFRC));
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmGetTBCTL - print the Time-Base Control Register register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmGetTBCTL
	(
	VXB_DEV_ID pDev
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"TBCTL: %x \n",
			CSR_READ_2(pDev, PWMSS_EPWM_TBCTL));
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmSetTBCTL - Set the Time-Base Control Register register bits
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728ehrpwmSetTBCTL
	(
	VXB_DEV_ID pDev,
	UINT32  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)action);
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmPWMIntEventSelect - ePWM Interrupt (EPWMx_INT) Selection Options
*
* This routine set at what event the interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS Am5728ehrpwmPWMIntEventSelect
	(
	VXB_DEV_ID pDev,
	UINT32 mode
	)
	{
	UINT32 regVal;

	if (pDev == NULL)
		return ERROR;

	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETSEL);
	switch (mode)
	{
	case TBCNT_EQ_ZERO:
		// Enable event time-base counter equal to zero
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_TBPRD:
		// Enable event time-base counter equal to TIME PERIOD
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_PERIOD);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPA:
		// Enable event time-base counter equal to cmpa while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPA:
		// Enable event time-base counter equal to cmpa while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPB:
		// Enable event time-base counter equal to cmpb while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPB:
		// Enable event time-base counter equal to cmpb while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
		break;
	}
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmPWMIntPeriodSelect - ePWM Interrupt (EPWMx_INT) Selection Options
*
* These bits determine how many selected ETSEL[INTSEL] events
* need to occur before an interrupt is generated.
* To be generated, the interrupt must be enabled (ETSEL[INT] = 1).
* If the interrupt status flag is set from a previous interrupt
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS Am5728ehrpwmPWMIntPeriodSelect
	(
	VXB_DEV_ID pDev,
	UINT32 mode
	)
	{
	UINT32 regVal;

	if (pDev == NULL)
		return ERROR;

	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);
	
	switch (mode)
	{
	case ETPS_INTPRD_DISABLE :
		// Disable the interrupt event counter
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_DISABLE);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
		
	case ETPS_INTPRD_GEN_FIRST_EVT:
		// Generate an interrupt on the first event INTCNT = 01 (first event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_FIRST_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
		
	case ETPS_INTPRD_GEN_SECOND_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,0 (second	event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_SECOND_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
		
	case ETPS_INTPRD_GEN_THIRD_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,1 (third event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_THIRD_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
		break;
	
	}
	return OK;
	}


/*******************************************************************************
*
* Am5728ehrpwmPWMGetIntEventSelect - print ePWM Interrupt (EPWMx_INT) Selection Register
*
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS Am5728ehrpwmPWMGetIntEventSelect
	(
	VXB_DEV_ID pDev
	)
	{
	UINT32 regVal;

	if (pDev == NULL)
		return ERROR;

	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETSEL);
		
	regVal = CSR_GET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL);
	
	EHRPWM_DBG(EHRPWM_DBG_INFO,"PWMSS_EPWM_ETSEL_INTSEL: %x \n",
				regVal);
	return OK;
	}

/*******************************************************************************
*
* Am5728ehrpwmPWMGetIntPeriodSelect - print ePWM Interrupt (EPWMx_INT) Selection Reg
*
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS Am5728ehrpwmPWMGetIntPeriodSelect
	(
	VXB_DEV_ID pDev
	)
	{
	UINT32 regVal;

	if (pDev == NULL)
		return ERROR;

	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);

	regVal = CSR_GET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD);
	EHRPWM_DBG(EHRPWM_DBG_INFO,"PWMSS_EPWM_ETPS_INTPRD: %x \n",
					regVal);
	return OK;
	}
