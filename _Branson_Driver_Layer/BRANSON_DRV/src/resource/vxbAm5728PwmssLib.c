/* vxbPwmssLib.c - vxBus PWM Subsystem function interfaces file */

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

LOCAL struct vxbPwmssHandle pwmsshandle[6];

/*****************************************************************************
*
* Am5728pwmssRegister - register routine to VxBus pwmss subsystem.
*
* This function register pwmss set/get routine to VxBus subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void Am5728pwmssRegister
    (
    VXB_DEV_ID pDev,
    VXB_PWMSS_FUNC * pwmss,
    char    name[32]
    )
    {
    char    * pwmss0  = "pwmss0";
    char    * pwmss2  = "pwmss2";
    char    * pwmss3  = "pwmss3";
    char    * pwmss4  = "pwmss4";
    int       strLen    = strlen(name);
    
	if(strncmp(name, pwmss0, strLen) == 0)
		{
		pwmsshandle[0].pPwmssInst = pDev;
		pwmsshandle[0].pPwmssFunc = pwmss;
		}
	if(strncmp(name, pwmss2, strLen) == 0)
		{
		pwmsshandle[2].pPwmssInst = pDev;
		pwmsshandle[2].pPwmssFunc = pwmss;
		}
	if(strncmp(name, pwmss3, strLen) == 0)
		{
		pwmsshandle[3].pPwmssInst = pDev;
		pwmsshandle[3].pPwmssFunc = pwmss;
		}
	if(strncmp(name, pwmss4, strLen) == 0)
		{
		pwmsshandle[4].pPwmssInst = pDev;
		pwmsshandle[4].pPwmssFunc = pwmss;
		}
    }

/*******************************************************************************
*
* vxbAm5728PwmssEnable - enable the Pwmss module
*
* This routine enable ehrpwm module in pwm subsystem 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728PwmssEnable
    (
    int pwmss,
	int module
    )
    {
    if (pwmsshandle[pwmss].pPwmssInst == NULL)
		return ERROR;
	
	return pwmsshandle[pwmss].pPwmssFunc->Am5728ehrpwmClkConfigEnable(pwmsshandle[pwmss].pPwmssInst,module);
	
    }

/*******************************************************************************
*
* vxbAm5728PwmssDisable - disable the Pwmss module
*
* This routine enable ehrpwm module in pwm subsystem
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

STATUS vxbAm5728PwmssDisable
    (
	int pwmss,
	int module
    )
    {
    
	if (pwmsshandle[pwmss].pPwmssInst == NULL)
		return ERROR;
	
	return pwmsshandle[pwmss].pPwmssFunc->Am5728ehrpwmClkConfigDisable(pwmsshandle[pwmss].pPwmssInst,module);
	
    }
