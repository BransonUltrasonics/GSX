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
#include <vxbAm5728EhrPwmLib.h>
#ifdef _WRS_CONFIG_FS_DOSFS
#include <dosFsLib.h>
#endif /* _WRS_CONFIG_FS_DOSFS */

/* locals */

LOCAL struct vxbEhrPwmHandle ehrpwmHandle[6];

/*****************************************************************************
*
* Am5728ehrpwmRegister - register ehrpwm set/get routine to VxBus ehrpwm subsystem.
*
* This function register ehrpwm set/get routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void Am5728ehrpwmRegister
    (
    VXB_DEV_ID pDev,
    VXB_EHRPWM_FUNC * ehrpwm,
	char    name[32]
    )
    {
    char    * pwmss0  = "pwmss0";
    char    * am5728ehrpwm0  = "am5728ehrpwm0";
    char    * ehrpwm2  = "ehrpwm2";
    char    * am5728ehrpwm4  = "am5728ehrpwm4";
	int       strLen    = strlen(name);
	
	if(strncmp(name, am5728ehrpwm0, strLen) == 0)
		{
		ehrpwmHandle[0].pEhrPwmInst = pDev;
		ehrpwmHandle[0].pEhrPwmFunc = ehrpwm;
		}
	if(strncmp(name, ehrpwm2, strLen) == 0)
		{
		ehrpwmHandle[2].pEhrPwmInst = pDev;
		ehrpwmHandle[2].pEhrPwmFunc = ehrpwm;
		}
	if(strncmp(name, am5728ehrpwm4, strLen) == 0)
		{
		ehrpwmHandle[4].pEhrPwmInst = pDev;
		ehrpwmHandle[4].pEhrPwmFunc = ehrpwm;
		}
    }

/*******************************************************************************
*
* vxbAm5728EhrPwmSetTbClk - set the divisor of the time base clock
*
* This routine takes the divisor for System Clock and finally sets 
* Time Base Clock Frequency(TBCLK)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbAm5728EhrPwmSetTbClk
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
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbClk(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, tb_freq);

	}


/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbClk - get the value of the Time Base Clock Frequency(TBCLK)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbAm5728EhrPwmGetTbClk
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbClk(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmSetTbPWMCfg - set the value of the current PWM Counter Direction [CTRMODE] 
* 						& Maximum Period Counter Value(PWM Frequency[TBPRD]).
* 						
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetTbPWMCfg
    (
	uint32_t freq,		
	uint32_t dir,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbPWMCfg(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, freq, dir);

    }

/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbPWMCfg - get the value of both PWM Counter Direction[CTRMODE]
* 						& Maximum Period Counter Value(PWM Frequency[TBPRD]).
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetTbPWMCfg
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbPWMCfg(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }

#if 0
/*******************************************************************************
*
* vxbAm5728EhrPwmSetTbPWMFreq - set the value of both PWM Counter Direction[CTRMODE]
* 						& Maximum Period Counter Value(PWM Frequency[TBPRD]).
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS Am5728vxbEhrPwmSetTbPWMFreq
    (
	uint32_t pwm_freq,
	uint32_t dir
    )
    {
    if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
        return ERROR;

    vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
    
    return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbPWMFreq(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, 
    		pwm_freq, dir);
    }
#endif

#if 0
/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbPWMFreq - get the value of the current PWM Frequency
*
* This routine expects an tm structure as input, which it will
* populate with the ehrpwm's current date and time. This routine will callback the
* ehrpwmGet() routine in ehrpwm device driver.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbAm5728EhrPwmGetTbPWMFreq
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
    vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
    
    return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbPWMFreq(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
    }
#endif

#if 0
/*******************************************************************************
*
* vxbAm5728EhrPwmSetTbPWMDir - set the value of the current PWM Counter Direction[CTRMODE]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetTbPWMDir
    (
	uint32_t dir
    )
    {
    if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
        return ERROR;

    vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
    
    return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbPWMDir(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, dir);
    }
#endif

/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbPWMDir - get the value of the current PWM Counter Direction[CTRMODE]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetTbPWMDir
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbPWMDir(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmSetCMPA - set the value for Comparator A (CMPA)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetCMPA
    (
    uint32_t  ehrpwmTime,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbCOMPAConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, ehrpwmTime);

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

STATUS vxbAm5728EhrPwmGetCMPA
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbCOMPAConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmSetCMPB - set the value for Comparator B (CMPB)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetCMPB
    (
    uint32_t  ehrpwmTime,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbCOMPBConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, ehrpwmTime);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmGetCMPB - get the value for Comparator B (CMPB)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetCMPB
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbCOMPBConf(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmSetTbAQConfA - set the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0A
*
* Set at what point it will take action and what will be the action 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetTbAQConfA
    (
    uint32_t action,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbAQConfA(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, action);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbAQConfA - get the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0A
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

/* Set at what point it will take action and what will be the action */
STATUS vxbAm5728EhrPwmGetTbAQConfA
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbAQConfA(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmSetTbAQConfB - set the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0B
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetTbAQConfB
    (
    uint32_t action,
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTbAQConfB(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst, action);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbAQConfB - get the actions to take when TBCNT reach 0, TBPRD, CMPA
* 						and CMPB for eHRPWM0B
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetTbAQConfB
    (
	UINT8	ehrpwm_inst
    )
    {
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbAQConfB(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

    }


/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbPRD - get the value of the  Maximum Period Counter 
* 						  Value(PWM Frequency[TBPRD])
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetTbPRD 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetPRD(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}


/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbCounter - get the value of the Time Base Counter [TBCNT]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetTbCounter 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetCounter(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}


/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbCounter - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetTbStatus 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTbStatus(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}

/*******************************************************************************
*
* vxbAm5728EhrPwmGetTbCounter - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmRegisterISR 
	(
	EHRPWMDRV_IRQ_HANDLER irqHandler,
	void* ehrpwmHandlerArgs,
	UINT8	ehrpwm_inst,
	int period,int mode
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmPWMRegisterISR(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,
			irqHandler, ehrpwmHandlerArgs, period,mode);
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmGetIntStatus - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetIntStatus 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetIntStatus(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}

/*******************************************************************************
*
* vxbAm5728EhrPwmEnableInt - Enable interrupt generateion from PWM
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmEnableInt 
	(
	UINT8	ehrpwm_inst,
	int period, 
	int mode
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmEnableInt(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,period,mode);

}

/*******************************************************************************
*
* vxbAm5728EhrPwmSetEmulation - set the These bits select the behavior of the ePWM 
* time-base counter during emulation suspend events.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbAm5728EhrPwmSetEmulation 
	(
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetEmulation(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);

	}

/*******************************************************************************
*
* vxbAm5728EhrPwmSetAQSFRC - set Software Force & also set Action when One-Time 
* Software Force A & B Is invoked
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbAm5728EhrPwmSetAQSFRC 
	(
	UINT32 val,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetAQSFRC(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,val);
	
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmGetAQSFRC - get AQSFRC register 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbAm5728EhrPwmGetAQSFRC 
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetAQSFRC(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}


/*******************************************************************************
*
* vxbAm5728EhrPwmSetTBCTL - set Time-Base Control Register register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/


STATUS vxbAm5728EhrPwmSetTBCTL 
	(
	UINT32 val,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,val);
	
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmGetTBCTL - get Time-Base Control Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetTBCTL 
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;

	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmSetIntEvt - ePWM Interrupt (EPWMx_INT) Selection Options
*
* This routine set at what event the interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetIntEvt
	(
	UINT32 mode,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetIntEvtTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,mode);
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmSetIntPrd - ePWM Interrupt (EPWMx_INT) Period Options
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

STATUS vxbAm5728EhrPwmSetIntPrd
	(
	UINT32 mode,
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetIntPrdTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,mode);
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmGetIntEvt - get ePWM Interrupt (EPWMx_INT) Selection Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetIntEvt
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetIntEvtTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmGetIntPrd - get ePWM Interrupt (EPWMx_INT) Period Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmGetIntPrd
	(
	UINT8	ehrpwm_inst
	)
	{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmGetIntPrdTBCTL(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	}

/*******************************************************************************
*
* vxbAm5728EhrPwmDisableInt - disable ePWM Interrupt 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmDisableInt (UINT8	ehrpwm_inst)
{
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmDisableInt(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst);
	
}

/*******************************************************************************
*
* vxbAm5728EhrPwmSetTbCounter - set the value of the Time Base Counter [TBCNT]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728EhrPwmSetTbCounter 
	(
	UINT16  counter,
	UINT8	ehrpwm_inst
	)
	{
	
	if (ehrpwmHandle[ehrpwm_inst].pEhrPwmInst == NULL)
		return ERROR;
	
	vxbAm5728PwmssEnable(ehrpwm_inst,EHRPWM);//If PwmSS Ehrpwm not enabled ,driver will crash
	
	return ehrpwmHandle[ehrpwm_inst].pEhrPwmFunc->Am5728ehrpwmSetCounter(ehrpwmHandle[ehrpwm_inst].pEhrPwmInst,counter);
	}

