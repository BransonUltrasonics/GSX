/*
 * vxbTiehrpwm.c
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
#include <vxbEhrPwmLib.h>

/* defines */
#define EHRPWM_DBG_ON
#ifdef  EHRPWM_DBG_ON

#define EHRPWM_DBG_IRQ         0x00000001
#define EHRPWM_DBG_INFO        0x00000002
#define EHRPWM_DBG_ERR         0x00000004
#define EHRPWM_DBG_ALL         0xffffffff
#define EHRPWM_DBG_OFF         0x00000000

LOCAL UINT32 ehrpwmDbgMask = EHRPWM_DBG_ALL;

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
char    * ehrpwm0  = "ehrpwm0";
char    * ehrpwm3  = "ehrpwm3";
char    * ehrpwm4  = "ehrpwm4";
    
LOCAL STATUS    am437xehrpwmProbe (VXB_DEV_ID);
LOCAL STATUS    am437xehrpwmAttach (VXB_DEV_ID);
LOCAL STATUS ehrpwmTbTimeBaseClkCfg(VXB_DEV_ID pDev, uint32_t tb);
LOCAL STATUS ehrpwmTbPwmDirCfg(VXB_DEV_ID pDev,uint32_t dir);
LOCAL STATUS ehrpwmTbPwmFreqCfg(VXB_DEV_ID pDev,uint32_t freq,uint32_t dir);
LOCAL STATUS ehrpwmTbPwmCfg(VXB_DEV_ID pDev,uint32_t freq,uint32_t dir);
LOCAL STATUS ehrpwmTbSetEmulationMode(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmCtrCompAConf(VXB_DEV_ID pDev,uint32_t cmpa);
LOCAL STATUS ehrpwmCtrCompBConf(VXB_DEV_ID pDev,uint32_t cmpb);
LOCAL STATUS ehrpwmActionQualifierConfA(VXB_DEV_ID pDev,uint32_t action);
LOCAL STATUS ehrpwmGetActionQualifierConfA(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmActionQualifierConfB(VXB_DEV_ID pDev,uint32_t action);
LOCAL STATUS ehrpwmGetActionQualifierConfB(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmGetCtrCompAConf(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmGetCtrCompBConf(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmGetCounterTbCTL(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmGetCounter(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmSetCounter(VXB_DEV_ID pDev,uint16_t counter);
LOCAL STATUS ehrpwmGetPRDCounter(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmGetTbSts(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmGetTbPwmCfg(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmEnablePWMInt(VXB_DEV_ID pDev,int period, int mode);
LOCAL STATUS ehrpwmIntConnect(VXB_DEV_ID pDev,EHRPWMDRV_IRQ_HANDLER irqHandler,void* ehrpwmHandlerArgs,int period,int mode);
LOCAL STATUS ehrpwmIntStatus(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmDisablePWMInt(VXB_DEV_ID pDev);
LOCAL void ehrpwmIrqHandler0(void * arg);
LOCAL void ehrpwmIrqHandler3(void * arg);
LOCAL void ehrpwmIrqHandler4(void * arg);
LOCAL STATUS ehrpwmSetAQSFRC(VXB_DEV_ID pDev,uint32_t  action);
LOCAL STATUS ehrpwmGetAQSFRC(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmSetTBCTL(VXB_DEV_ID pDev,uint32_t  action);
LOCAL STATUS ehrpwmGetTBCTL(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmPWMIntPeriodSelect(VXB_DEV_ID pDev,uint32_t mode);
LOCAL STATUS ehrpwmPWMIntEventSelect(VXB_DEV_ID pDev,uint32_t mode);
LOCAL STATUS ehrpwmPWMGetIntPeriodSelect(VXB_DEV_ID pDev);
LOCAL STATUS ehrpwmPWMGetIntEventSelect(VXB_DEV_ID pDev);

LOCAL VXB_DRV_METHOD am437xehrpwmMethods[] = {
    { VXB_DEVMETHOD_CALL (vxbDevProbe),  (FUNCPTR)am437xehrpwmProbe},
    { VXB_DEVMETHOD_CALL (vxbDevAttach), (FUNCPTR)am437xehrpwmAttach},
	VXB_DEVMETHOD_END
};

VXB_DRV vxbOfAm437xehrpwmDrv =
    {
    { NULL } ,
	"ti,am4377-ehrpwm",                 /* Name */
    "TI AM437x EPWM Driver",         /* Description */
	VXB_BUSID_FDT,                      /* Class */
    0,                                  /* Flags */
    0,                                  /* Reference count */
	(VXB_DRV_METHOD*)&am437xehrpwmMethods  /* Method table */
    };

VXB_DRV_DEF (vxbOfAm437xehrpwmDrv)

LOCAL const VXB_FDT_DEV_MATCH_ENTRY am437xehrpwmMatch[] =
    {
        {
        "ti,am4377-ehrpwm", /* compatible */
        (void *)NULL
        },
        {} /* Empty terminated list */
    };


/*************************************************************************
 * 
 * am437xehrpwmProbe : This function finds and compare the compatible 
 * field from the dts file.
 * 
 * RETURN : if found then success , else fail.
 */

LOCAL STATUS am437xehrpwmProbe
    (
    struct vxbDev * pDev
    )
    {
	int ret = 0;
	ret = vxbFdtDevMatch (pDev, am437xehrpwmMatch, NULL);
	return ret;
    }


/***************************************************************************
 * 
 * am437xehrpwmAttach : This function allocates driver structure, 
 * registers and map it to  system.It always does the basic hardware and 
 * software initialization needed for driver and hardware functioning.
 * 
 * RETURN :  attach success , else fail.
 */

LOCAL STATUS am437xehrpwmAttach
    (
    VXB_DEV_ID  pDev
    )
	{
	VXB_RESOURCE_ADR *      pResAdr = NULL;
	VXB_RESOURCE *          pRes    = NULL;
	VXB_FDT_DEV *       	pFdtDev;
	VXB_DEV_ID          	pCur;
	VXB_EHRPWM_FUNC * 		ehrpwm;
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
	
	ehrpwm->ehrpwmSetTbClk = ehrpwmTbTimeBaseClkCfg;
	ehrpwm->ehrpwmSetTbPWMFreq = ehrpwmTbPwmFreqCfg;
	ehrpwm->ehrpwmSetTbPWMDir = ehrpwmTbPwmDirCfg;
	ehrpwm->ehrpwmSetTbPWMCfg = ehrpwmTbPwmCfg;
	ehrpwm->ehrpwmSetTbCOMPAConf = ehrpwmCtrCompAConf;
	ehrpwm->ehrpwmSetTbCOMPBConf = ehrpwmCtrCompBConf;
	ehrpwm->ehrpwmSetTbAQConfA = ehrpwmActionQualifierConfA;
	ehrpwm->ehrpwmSetTbAQConfB = ehrpwmActionQualifierConfB;
	ehrpwm->ehrpwmGetTbPWMCfg = ehrpwmGetTbPwmCfg;
	ehrpwm->ehrpwmGetTbAQConfA = ehrpwmGetActionQualifierConfA;
	ehrpwm->ehrpwmGetTbAQConfB = ehrpwmGetActionQualifierConfB;
	ehrpwm->ehrpwmGetTbCOMPAConf = ehrpwmGetCtrCompAConf;
	ehrpwm->ehrpwmGetTbCOMPBConf = ehrpwmGetCtrCompBConf;
	ehrpwm->ehrpwmGetTbClk = ehrpwmGetCounterTbCTL;
	ehrpwm->ehrpwmGetCounter = ehrpwmGetCounter;
	ehrpwm->ehrpwmSetCounter = ehrpwmSetCounter;
	ehrpwm->ehrpwmGetPRD = ehrpwmGetPRDCounter;
	ehrpwm->ehrpwmGetTbStatus = ehrpwmGetTbSts;
	ehrpwm->ehrpwmGetTbPWMDir = ehrpwmGetCounterTbCTL;
	ehrpwm->ehrpwmPWMRegisterISR = ehrpwmIntConnect;
	ehrpwm->ehrpwmGetIntStatus = ehrpwmIntStatus;
	ehrpwm->ehrpwmEnableInt = ehrpwmEnablePWMInt;
	ehrpwm->ehrpwmDisableInt = ehrpwmDisablePWMInt;
	ehrpwm->ehrpwmSetEmulation = ehrpwmTbSetEmulationMode;
	ehrpwm->ehrpwmSetAQSFRC = ehrpwmSetAQSFRC;
	ehrpwm->ehrpwmGetAQSFRC = ehrpwmGetAQSFRC;
	ehrpwm->ehrpwmSetTBCTL = ehrpwmSetTBCTL;
	ehrpwm->ehrpwmGetTBCTL = ehrpwmGetTBCTL;
	ehrpwm->ehrpwmSetIntEvtTBCTL = ehrpwmPWMIntEventSelect;
	ehrpwm->ehrpwmSetIntPrdTBCTL = ehrpwmPWMIntPeriodSelect;
	ehrpwm->ehrpwmGetIntEvtTBCTL = ehrpwmPWMGetIntEventSelect;
	ehrpwm->ehrpwmGetIntPrdTBCTL = ehrpwmPWMGetIntPeriodSelect;
	ehrpwmRegister(pDev,ehrpwm,pFdtDev->name);
	
	/* enable and connect ISR */
		
	if (pEhrpwmObj->intRes != NULL)
		{
		EHRPWM_DBG(EHRPWM_DBG_INFO,"\n");
		if(strncmp(pFdtDev->name, ehrpwm0, strLen) == 0)
			{
			ret = vxbIntConnect (pDev, pEhrpwmObj->intRes, 
							(VOIDFUNCPTR)ehrpwmIrqHandler0,
										NULL);
			}
		if(strncmp(pFdtDev->name, ehrpwm3, strLen) == 0)
			{
			ret = vxbIntConnect (pDev, pEhrpwmObj->intRes, 
							(VOIDFUNCPTR)ehrpwmIrqHandler3,
										NULL);
			}
		if(strncmp(pFdtDev->name, ehrpwm4, strLen) == 0)
			{
			ret = vxbIntConnect (pDev, pEhrpwmObj->intRes, 
							(VOIDFUNCPTR)ehrpwmIrqHandler4,
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
* ehrpwmTbTimeBaseClkCfg - set the value of the current time base clock
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS ehrpwmTbTimeBaseClkCfg
	(
	VXB_DEV_ID pDev,
	uint32_t clkDiv
	)
	{
	/* Configure Time base clock */
	EHRPWMGEN *    pDrvCtrl;
	uint32_t hspClkDiv = 0U;
	uint32_t lspClkDiv = 0U;
	uint32_t lspClkDivSetting = 0U;
	uint32_t regVal = 0U;
	uint32_t DivVal = 1U;

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
	CSR_WRITE_2(pDev,PWMSS_EPWM_TBCTL, (uint16_t)regVal);
	
	return OK;
	}

/*******************************************************************************
*
* ehrpwmTbPwmFreqCfg - set the value of the current PWM Frequency
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS ehrpwmTbPwmFreqCfg
	(
	VXB_DEV_ID pDev,
	uint32_t tbPeriodCount,
	uint32_t dir
	)
	{
	/* Configure PWM time base counter frequency 
	 * NOTE: Only to be used when directio is up or down.Cannot be used for both
	 * For Up-Down both mode use function ehrpwmTbPwmCfg
	 */
	
	uint32_t regVal = 0U;
	
	if (pDev == NULL)
		return ERROR;
	
	if(dir == EPWM_TB_COUNTER_DIR_UP_DOWN)
		regVal = (uint16_t)(tbPeriodCount - 1U) / 2U;
	else
		regVal = (uint16_t)(tbPeriodCount - 1U);
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBPRD, (uint16_t)regVal);
	
	return OK;
    }

/*******************************************************************************
*
* ehrpwmTbPwmDirCfg - set the value of the current PWM Counter Direction
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmTbPwmDirCfg
	(
	VXB_DEV_ID pDev,
	uint32_t counterDir
	)
	{
	/* Configure PWM time base counter direction */
	uint32_t regVal = 0U;
	
	if (pDev == NULL)
		return ERROR;
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_PRDLD, EPWM_SHADOW_REG_CTRL_ENABLE);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE, counterDir);
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (uint16_t)regVal);
	
	return OK;
	}

/*******************************************************************************
*
* ehrpwmTbPwmCfg - get the value of the current PWM Counter Direction
* 						& Period
*
* Configure PWM time base counter direction and period 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmTbPwmCfg
	(
	VXB_DEV_ID pDev,
	uint32_t tbPeriodCount,
	uint32_t counterDir
	)
	{
	uint32_t regVal = 0U;

	if (pDev == NULL)
		return ERROR;
	
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_PRDLD, EPWM_SHADOW_REG_CTRL_ENABLE);
	CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE, counterDir);
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (uint16_t)regVal);
	
	regVal = (uint16_t)(tbPeriodCount);
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBPRD, (uint16_t)regVal);
	return OK;
	}

/*******************************************************************************
*
* ehrpwmTbPwmCfg - get the value of the current PWM Counter Direction
* 						& Frequency
*
* Configure PWM time base counter direction and frequency 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetTbPwmCfg
	(
	VXB_DEV_ID pDev
	)
	{
	uint32_t regVal = 0U;

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
* ehrpwmTbSetEmulationMode - set the emulation mode
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

STATUS ehrpwmTbSetEmulationMode(VXB_DEV_ID pDev)
	{
	uint32_t regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);

	if (pDev == NULL)
		return ERROR;
	
	regVal &= (~PWMSS_EPWM_TBCTL_FREE_SOFT_MASK);
	regVal |= (PWMSS_EPWM_TBCTL_FREE_SOFT_STOP_AFTER_CYCLE << PWMSS_EPWM_TBCTL_FREE_SOFT_SHIFT) & PWMSS_EPWM_TBCTL_FREE_SOFT_MASK;
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (uint16_t)regVal);
	return OK;
	}

/******************************************************************************
*
* ehrpwmCtrCompAConf - set the CMPA value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmCtrCompAConf
	(
	VXB_DEV_ID pDev,
	uint32_t   cmpa
	)
	{
	/* Counter Comparator A configuration */
	uint32_t regVal =
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
	CSR_WRITE_2(pDev,PWMSS_EPWM_CMPCTL,	(uint16_t)regVal);
	
	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPA);
	regVal &= (~PWMSS_EPWM_CMPA_MASK);
	regVal |= ( cmpa << PWMSS_EPWM_CMPA_SHIFT) & PWMSS_EPWM_CMPA_MASK;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_CMPA, (uint16_t)regVal);
	return OK;
	}


/******************************************************************************
*
* ehrpwmGetCtrCompAConf - print the value for CMPA
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetCtrCompAConf
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
* ehrpwmCtrCompBConf - set the CMPB value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmCtrCompBConf
	(
	VXB_DEV_ID pDev,
	uint32_t   cmpb
	)
	{
	/* Counter Comparator B configuration */
	uint32_t regVal =
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
	
	CSR_WRITE_2(pDev,PWMSS_EPWM_CMPCTL,	(uint16_t)regVal);
	
	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPB);
	regVal &= (~PWMSS_EPWM_CMPB_MASK);
	regVal |= ( cmpb << PWMSS_EPWM_CMPB_SHIFT) & PWMSS_EPWM_CMPB_MASK;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_CMPB, (uint16_t)regVal);
	return OK;
	}


/*******************************************************************************
*
* ehrpwmGetCtrCompBConf - print the value for CMPB
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetCtrCompBConf
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
* ehrpwmActionQualifierConfA - set the action qualifier for ehrpwm0A
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

STATUS ehrpwmActionQualifierConfA
	(
	VXB_DEV_ID pDev,
	uint32_t  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	/*TODO: Configure Action Qualifier - toggle if any case happens*/
	CSR_WRITE_2(pDev, PWMSS_EPWM_AQCTLA, (uint16_t)action);
	return OK;
	}


/*******************************************************************************
*
* ehrpwmActionQualifierConfA - print the action qualifier for ehrpwm0A
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetActionQualifierConfA
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
* ehrpwmActionQualifierConfB - set the action qualifier for ehrpwm0B
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

STATUS ehrpwmActionQualifierConfB
	(
	VXB_DEV_ID pDev,
	uint32_t  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	/*TODO: Configure Action Qualifier - toggle if any case happens */
	CSR_WRITE_2(pDev, PWMSS_EPWM_AQCTLB, (uint16_t)action);
	return OK;
	}


/*******************************************************************************
*
* ehrpwmActionQualifierConfB - print the action qualifier for ehrpwm0B
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetActionQualifierConfB
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
* ehrpwmGetCounterTbCTL - print the COUTER MODE(direction of counter)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetCounterTbCTL
	(
	VXB_DEV_ID pDev
	)
	{
	uint32_t regVal;

	if (pDev == NULL)
		return ERROR;
	
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
	EHRPWM_DBG(EHRPWM_DBG_INFO,"Direction of Counter : %x \n",
			CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE));
	return OK;
	}

/*******************************************************************************
*
* ehrpwmGetCounter - print the TBCNT
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetCounter
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
* ehrpwmSetCounter - set the TBCNT
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmSetCounter
	(
	VXB_DEV_ID pDev,
	uint16_t counter
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCNT,counter);
	return OK;
	}

/*******************************************************************************
*
* ehrpwmGetPRDCounter - print the TBPRD
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetPRDCounter
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
* ehrpwmGetTbSts - print the time base status register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetTbSts
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
* ehrpwmTbEnablePWMInt - Enables PWM Interrupt generation and set at which event 
* 						to generate interrupt.
*
* This routine enables the PWM interrupt generation and set at what event the 
* interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS ehrpwmEnablePWMInt
	(
	VXB_DEV_ID pDev,
	int period,
	int mode
	)
	{
	uint32_t regVal;
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
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
		
	case ETPS_INTPRD_GEN_FIRST_EVT:
		// Generate an interrupt on the first event INTCNT = 01 (first event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_FIRST_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
		
	case ETPS_INTPRD_GEN_SECOND_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,0 (second	event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_SECOND_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
		
	case ETPS_INTPRD_GEN_THIRD_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,1 (third event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_THIRD_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
	
	}
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
	
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
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_TBPRD:
		// Enable event time-base counter equal to TIME PERIOD
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_PERIOD);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPA:
		// Enable event time-base counter equal to cmpa while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPA:
		// Enable event time-base counter equal to cmpa while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPB:
		// Enable event time-base counter equal to cmpb while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPB:
		// Enable event time-base counter equal to cmpb while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
	}
	//CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
	return OK;
	}

/*******************************************************************************
*
* ehrpwmTbDisablePWMInt - Disable PWM interrupt
*
* This routine disables the PWM interrupt generation
*  
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
LOCAL STATUS ehrpwmDisablePWMInt
	(
	VXB_DEV_ID pDev
	)
	{
	EHRPWMGEN *    	pDrvCtrl;
	uint32_t 		regVal;
	int 			ret;
	
	
	if (pDev == NULL)
		return ERROR;
	
	pDrvCtrl = vxbDevSoftcGet (pDev);
	
	/* Generate an interrupt on the first event INTCNT  */
	regVal = 0;
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
	
	/* Enable ePWM Interrupt (EPWMx_INT) Generation
	 * & Enable event time-base counter equal to zero
	 */
	regVal = 0;
	CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
	
	CSR_WRITE_2(pDev,PWMSS_EPWM_TBCTL, (uint16_t)0);
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCNT,0);
	
    ret = vxbIntDisable (pDev, pDrvCtrl->intRes);
	
	
	return OK;
	}

/*******************************************************************************
*
* ehrpwmIntConnect - Registers the user application ISR
*
* This routine stores the function pointer from the user application to the 
* global function pointer object in the driver and gets called when interrupt
* is triggered.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmIntConnect
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
    if(strncmp(pFdtDev->name, ehrpwm0, strLen) == 0)
		{
    	pIrqHandler[0].func = irqHandler;
    	pIrqHandler[0].inst = pDev;
		}
	if(strncmp(pFdtDev->name, ehrpwm3, strLen) == 0)
		{
		pIrqHandler[3].func = irqHandler;
		pIrqHandler[3].inst = pDev;
		}
	if(strncmp(pFdtDev->name, ehrpwm4, strLen) == 0)
		{
		pIrqHandler[4].func = irqHandler;
		pIrqHandler[4].inst = pDev;
		}
    
    
 
    /* Enable PWM Interrupts */
   // ehrpwmEnablePWMInt(pDev,ETPS_INTPRD_GEN_FIRST_EVT,TBCNT_EQ_ZERO);
	 ehrpwmEnablePWMInt(pDev,period,mode);

    return OK;
	}


/*******************************************************************************
*
* ehrpwmIntStatus - print the Interrupt status register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmIntStatus
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
* ehrpwmIrqHandler0 - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL void ehrpwmIrqHandler0
	(
	void *arg
	)
	{
	CSR_WRITE_2(pIrqHandler[0].inst,PWMSS_EPWM_ETCLR,1);
	(*pIrqHandler[0].func)();
	}

/*******************************************************************************
*
* ehrpwmIrqHandler3 - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL void ehrpwmIrqHandler3
	(
	void *arg
	)
	{
	CSR_WRITE_2(pIrqHandler[3].inst,PWMSS_EPWM_ETCLR,1);
	(*pIrqHandler[3].func)();
	}

/*******************************************************************************
*
* ehrpwmIrqHandler4 - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL void ehrpwmIrqHandler4
	(
	void *arg
	)
	{
	CSR_WRITE_2(pIrqHandler[4].inst,PWMSS_EPWM_ETCLR,1);
	(*pIrqHandler[4].func)();
	}
/*******************************************************************************
*
* ehrpwmSetAQSFRC - set Software Force & also set Action when One-Time 
* Software Force A & B Is invoked
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmSetAQSFRC
	(
	VXB_DEV_ID pDev,
	uint32_t  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	/*TODO: Configure Action Qualifier - toggle if any case happens*/
	CSR_WRITE_2(pDev, PWMSS_EPWM_AQSFRC, (uint16_t)action);
	return OK;
	}


/*******************************************************************************
*
* ehrpwmGetAQSFRC - print the AQSFRC register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetAQSFRC
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
* ehrpwmGetTBCTL - print the Time-Base Control Register register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmGetTBCTL
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
* ehrpwmSetTBCTL - Set the Time-Base Control Register register bits
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS ehrpwmSetTBCTL
	(
	VXB_DEV_ID pDev,
	uint32_t  action
	)
	{

	if (pDev == NULL)
		return ERROR;
	
	CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (uint16_t)action);
	return OK;
	}

/*******************************************************************************
*
* ehrpwmPWMIntEventSelect - ePWM Interrupt (EPWMx_INT) Selection Options
*
* This routine set at what event the interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS ehrpwmPWMIntEventSelect
	(
	VXB_DEV_ID pDev,
	uint32_t mode
	)
	{
	uint32_t regVal;

	if (pDev == NULL)
		return ERROR;

	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETSEL);
	switch (mode)
	{
	case TBCNT_EQ_ZERO:
		// Enable event time-base counter equal to zero
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_TBPRD:
		// Enable event time-base counter equal to TIME PERIOD
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_PERIOD);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPA:
		// Enable event time-base counter equal to cmpa while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPA:
		// Enable event time-base counter equal to cmpa while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_INC_CMPB:
		// Enable event time-base counter equal to cmpb while incrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_INCR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
		
	case TBCNT_EQ_DEC_CMPB:
		// Enable event time-base counter equal to cmpb while decrementing
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_DECR);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (uint16_t)regVal);
		break;
	}
	return OK;
	}

/*******************************************************************************
*
* ehrpwmPWMIntPeriodSelect - ePWM Interrupt (EPWMx_INT) Selection Options
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

LOCAL STATUS ehrpwmPWMIntPeriodSelect
	(
	VXB_DEV_ID pDev,
	uint32_t mode
	)
	{
	uint32_t regVal;

	if (pDev == NULL)
		return ERROR;

	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);
	
	switch (mode)
	{
	case ETPS_INTPRD_DISABLE :
		// Disable the interrupt event counter
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_DISABLE);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
		
	case ETPS_INTPRD_GEN_FIRST_EVT:
		// Generate an interrupt on the first event INTCNT = 01 (first event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_FIRST_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
		
	case ETPS_INTPRD_GEN_SECOND_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,0 (second	event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_SECOND_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
		
	case ETPS_INTPRD_GEN_THIRD_EVT:
		// Generate interrupt on ETPS[INTCNT] = 1,1 (third event)
		CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_THIRD_EVT);
		CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (uint16_t)regVal);
		break;
	
	}
	return OK;
	}


/*******************************************************************************
*
* ehrpwmPWMGetIntEventSelect - print ePWM Interrupt (EPWMx_INT) Selection Register
*
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS ehrpwmPWMGetIntEventSelect
	(
	VXB_DEV_ID pDev
	)
	{
	uint32_t regVal;

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
* ehrpwmPWMGetIntPeriodSelect - print ePWM Interrupt (EPWMx_INT) Selection Reg
*
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS ehrpwmPWMGetIntPeriodSelect
	(
	VXB_DEV_ID pDev
	)
	{
	uint32_t regVal;

	if (pDev == NULL)
		return ERROR;

	regVal = 0;
	regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);

	regVal = CSR_GET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD);
	EHRPWM_DBG(EHRPWM_DBG_INFO,"PWMSS_EPWM_ETPS_INTPRD: %x \n",
					regVal);
	return OK;
	}
