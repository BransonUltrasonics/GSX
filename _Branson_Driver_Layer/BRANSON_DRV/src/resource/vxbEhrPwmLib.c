/* vxbEhrPwmLib.c - vxBus ehrpwm function interfaces file */
/*
 * Copyright - To be Done
 */

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <vxbEhrPwmLib.h>
#ifdef _WRS_CONFIG_FS_DOSFS
#include <dosFsLib.h>
#endif /* _WRS_CONFIG_FS_DOSFS */

/* locals */

LOCAL struct vxbEhrPwmHandle ehrpwmHandle[6];

/*****************************************************************************
*
* ehrpwmRegister - register ehrpwm set/get routine to VxBus ehrpwm subsystem.
*
* This function register ehrpwm set/get routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void ehrpwmRegister
    (
    VXB_DEV_ID pDev,
    VXB_EHRPWM_FUNC * ehrpwm,
	char    name[32]
    )
    {
    char    * pwmss0  = "pwmss0";
    char    * ehrpwm0  = "ehrpwm0";
    char    * ehrpwm3  = "ehrpwm3";
    char    * ehrpwm4  = "ehrpwm4";
	int       strLen    = strlen(name);
	
	if(strncmp(name, ehrpwm0, strLen) == 0)
		{
		ehrpwmHandle[0].pEhrPwmInst = pDev;
		ehrpwmHandle[0].pEhrPwmFunc = ehrpwm;
		}
	if(strncmp(name, ehrpwm3, strLen) == 0)
		{
		ehrpwmHandle[3].pEhrPwmInst = pDev;
		ehrpwmHandle[3].pEhrPwmFunc = ehrpwm;
		}
	if(strncmp(name, ehrpwm4, strLen) == 0)
		{
		ehrpwmHandle[4].pEhrPwmInst = pDev;
		ehrpwmHandle[4].pEhrPwmFunc = ehrpwm;
		}
    }

/*******************************************************************************
*
* vxbEhrPwmSetTbClk - set the divisor of the time base clock
*
* This routine takes the divisor for System Clock and finally sets 
* Time Base Clock Frequency(TBCLK)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbEhrPwmSetTbClk
    (
	uint32_t tb_freq,
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	/* Time-base Clock Prescale Bits.
	 * These bits determine part of the time-base clock prescale value. 
	 */
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbClk(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, tb_freq);

	}


/*******************************************************************************
*
* vxbEhrPwmGetTbClk - get the value of the Time Base Clock Frequency(TBCLK)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbEhrPwmGetTbClk
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbClk(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbEhrPwmSetTbPWMCfg - set the value of the current PWM Counter Direction [CTRMODE] 
* 						& Maximum Period Counter Value(PWM Frequency[TBPRD]).
* 						
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetTbPWMCfg
    (
	uint32_t freq,		
	uint32_t dir,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbPWMCfg(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, freq, dir);

    }

/*******************************************************************************
*
* vxbEhrPwmGetTbPWMCfg - get the value of both PWM Counter Direction[CTRMODE]
* 						& Maximum Period Counter Value(PWM Frequency[TBPRD]).
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetTbPWMCfg
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbPWMCfg(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }

#if 0
/*******************************************************************************
*
* vxbEhrPwmSetTbPWMFreq - set the value of both PWM Counter Direction[CTRMODE]
* 						& Maximum Period Counter Value(PWM Frequency[TBPRD]).
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetTbPWMFreq
    (
	uint32_t pwm_freq,
	uint32_t dir
    )
    {
    if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
        return ERROR;

    vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
    
    return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbPWMFreq(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, 
    		pwm_freq, dir);
    }
#endif

#if 0
/*******************************************************************************
*
* vxbEhrPwmGetTbPWMFreq - get the value of the current PWM Frequency
*
* This routine expects an tm structure as input, which it will
* populate with the ehrpwm's current date and time. This routine will callback the
* ehrpwmGet() routine in ehrpwm device driver.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbEhrPwmGetTbPWMFreq
    (
    void
    )
    {
    if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
        return ERROR;
/*
 * These bits determine the period of the time-base counter.
 * This sets the PWM frequency.
 */
    vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
    
    return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbPWMFreq(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
    }
#endif

#if 0
/*******************************************************************************
*
* vxbEhrPwmSetTbPWMDir - set the value of the current PWM Counter Direction[CTRMODE]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetTbPWMDir
    (
	uint32_t dir
    )
    {
    if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
        return ERROR;

    vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
    
    return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbPWMDir(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, dir);
    }
#endif

/*******************************************************************************
*
* vxbEhrPwmGetTbPWMDir - get the value of the current PWM Counter Direction[CTRMODE]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetTbPWMDir
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbPWMDir(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbEhrPwmSetCMPA - set the value for Comparator A (CMPA)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetCMPA
    (
    uint32_t  ehrpwmTime,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbCOMPAConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, ehrpwmTime);

    }


/*******************************************************************************
*
* vxbEhrPwmGetCMPA - get the value for Comparator A (CMPA)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetCMPA
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbCOMPAConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbEhrPwmSetCMPB - set the value for Comparator B (CMPB)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetCMPB
    (
    uint32_t  ehrpwmTime,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbCOMPBConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, ehrpwmTime);

    }


/*******************************************************************************
*
* vxbEhrPwmGetCMPB - get the value for Comparator B (CMPB)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetCMPB
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbCOMPBConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbEhrPwmSetTbAQConfA - set the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0A
*
* Set at what point it will take action and what will be the action 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetTbAQConfA
    (
    uint32_t action,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbAQConfA(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, action);

    }


/*******************************************************************************
*
* vxbEhrPwmGetTbAQConfA - get the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0A
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

/* Set at what point it will take action and what will be the action */
STATUS vxbEhrPwmGetTbAQConfA
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbAQConfA(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbEhrPwmSetTbAQConfB - set the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0B
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetTbAQConfB
    (
    uint32_t action,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTbAQConfB(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, action);

    }


/*******************************************************************************
*
* vxbEhrPwmGetTbAQConfB - get the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0B
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetTbAQConfB
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbAQConfB(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbEhrPwmGetTbPRD - get the value of the  Maximum Period Counter 
* 						  Value(PWM Frequency[TBPRD])
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetTbPRD 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetPRD(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}


/*******************************************************************************
*
* vxbEhrPwmGetTbCounter - get the value of the Time Base Counter [TBCNT]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetTbCounter 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetCounter(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}


/*******************************************************************************
*
* vxbEhrPwmGetTbCounter - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetTbStatus 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTbStatus(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}

/*******************************************************************************
*
* vxbEhrPwmGetTbCounter - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmRegisterISR 
	(
	EHRPWMDRV_IRQ_HANDLER irqHandler,
	void* ehrpwmHandlerArgs,
	UINT8	ehrpwm_inst,
	int period,int mode
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmPWMRegisterISR(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,
			irqHandler, ehrpwmHandlerArgs, period,mode);
	}

/*******************************************************************************
*
* vxbEhrPwmGetIntStatus - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetIntStatus 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetIntStatus(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}

/*******************************************************************************
*
* vxbEhrPwmEnableInt - Enable interrupt generateion from PWM
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmEnableInt 
	(
	UINT8	ehrpwm_inst,
	int period, 
	int mode
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmEnableInt(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,period,mode);

}

/*******************************************************************************
*
* vxbEhrPwmSetEmulation - set the These bits select the behavior of the ePWM 
* time-base counter during emulation suspend events.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbEhrPwmSetEmulation 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetEmulation(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}

/*******************************************************************************
*
* vxbEhrPwmSetAQSFRC - set Software Force & also set Action when One-Time 
* Software Force A & B Is invoked
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbEhrPwmSetAQSFRC 
	(
	UINT32 val,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetAQSFRC(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,val);
	
	}

/*******************************************************************************
*
* vxbEhrPwmGetAQSFRC - get AQSFRC register 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbEhrPwmGetAQSFRC 
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetAQSFRC(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}


/*******************************************************************************
*
* vxbEhrPwmSetTBCTL - set Time-Base Control Register register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbEhrPwmSetTBCTL 
	(
	UINT32 val,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,val);
	
	}

/*******************************************************************************
*
* vxbEhrPwmGetTBCTL - get Time-Base Control Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetTBCTL 
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}

/*******************************************************************************
*
* vxbEhrPwmSetIntEvt - ePWM Interrupt (EPWMx_INT) Selection Options
*
* This routine set at what event the interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetIntEvt
	(
	UINT32 mode,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetIntEvtTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,mode);
	}

/*******************************************************************************
*
* vxbEhrPwmSetIntPrd - ePWM Interrupt (EPWMx_INT) Period Options
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

STATUS vxbEhrPwmSetIntPrd
	(
	UINT32 mode,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetIntPrdTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,mode);
	}

/*******************************************************************************
*
* vxbEhrPwmGetIntEvt - get ePWM Interrupt (EPWMx_INT) Selection Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetIntEvt
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetIntEvtTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}

/*******************************************************************************
*
* vxbEhrPwmGetIntPrd - get ePWM Interrupt (EPWMx_INT) Period Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmGetIntPrd
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmGetIntPrdTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}

/*******************************************************************************
*
* vxbEhrPwmDisableInt - disable ePWM Interrupt 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmDisableInt (UINT8	ehrpwm_inst)
{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmDisableInt(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	
}

/*******************************************************************************
*
* vxbEhrPwmSetTbCounter - set the value of the Time Base Counter [TBCNT]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbEhrPwmSetTbCounter 
	(
	UINT16  counter,
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbPwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->ehrpwmSetCounter(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,counter);
	}

