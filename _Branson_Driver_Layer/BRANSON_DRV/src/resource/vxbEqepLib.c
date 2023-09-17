/* vxbEqepLib.c - vxBus EQEP function interfaces file */

/*
 * Copyright : To Be Done.
 */
/*
DESCRIPTION

This file implements the vxbus EQEP functionalities. All this functions
are used to operate the EQEP device.


*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <vxbEqepLib.h>
#ifdef _WRS_CONFIG_FS_DOSFS
#include <dosFsLib.h>
#endif /* _WRS_CONFIG_FS_DOSFS */

#define VXB_EQEP_DEBUG

/* locals */

LOCAL struct vxbEqepHandle eqepHandle[3];


/*****************************************************************************
*
* eqepRegister - register eqep set/get routine to VxBus eqep subsystem.
*
* This function register eqep set/get routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void eqepRegister
    (
    VXB_DEV_ID pDev,
    VXB_PWMSS_EQEP_FUNC * eqep,
    char    name[32]
    )
    {
    char    * eqep0  = "eqep0";
    char    * eqep2  = "eqep2";
    int       strLen    = strlen(name);
    
	if(strncmp(name, eqep0, strLen) == 0)
		{
    eqepHandle[0].pInst = pDev;
    eqepHandle[0].pEqepFunc = eqep;
    }
	
	if(strncmp(name, eqep2, strLen) == 0)
		{
    eqepHandle[2].pInst = pDev;
    eqepHandle[2].pEqepFunc = eqep;
    }
    }

/*******************************************************************************
*
* vxbEqepGet - get the value of the current EQEP 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbEqepGet
    (
    int eqep
    )
    {
    if (eqepHandle[eqep].pInst == NULL)
        return ERROR;
   
    return eqepHandle[eqep].pEqepFunc->eqepGet(eqepHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepGetMaxCount - get the maximum counter value
*
* RETURNS: maximum counter value.
*
* ERRNO: N/A
*/

STATUS vxbEqepGetMaxCount
    (
    int eqep
    )
    {
    if (eqepHandle[eqep].pInst == NULL)
        return ERROR;
   
    return eqepHandle[eqep].pEqepFunc->eqepGetMaxCount(eqepHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepSetMaxCount - set the maximum counter value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbEqepSetMaxCount
    (
    int eqep,
	int count
    )
    {
    if (eqepHandle[eqep].pInst == NULL)
        return ERROR;
   
    return eqepHandle[eqep].pEqepFunc->eqepSetMaxCount(eqepHandle[eqep].pInst,count);
    }

/*******************************************************************************
*
* vxbEqepGetPosCount - get the current position counter value
*
* RETURNS: get current counter.
*
* ERRNO: N/A
*/

INT32 vxbEqepGetPosCount
    (
    int eqep
    )
    {
    if (eqepHandle[eqep].pInst == NULL)
        return ERROR;
   
    return eqepHandle[eqep].pEqepFunc->eqepGetPosCount(eqepHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepGetInitCount - get the initial counter value
*
* RETURNS: initial counter value.
*
* ERRNO: N/A
*/

STATUS vxbEqepGetInitCount
    (
    int eqep
    )
    {
    if (eqepHandle[eqep].pInst == NULL)
        return ERROR;
   
    return eqepHandle[eqep].pEqepFunc->eqepGetInitCount(eqepHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepSetInitCount - set the initial counter value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbEqepSetInitCount
    (
    int eqep,
	int count
    )
    {
    if (eqepHandle[eqep].pInst == NULL)
        return ERROR;
   
    return eqepHandle[eqep].pEqepFunc->eqepSetInitCount(eqepHandle[eqep].pInst,count);
    }

/*******************************************************************************
*
* vxbEqepGetDirection - get the initial counter value
*
* RETURNS: direction value
*
* ERRNO: N/A
*/

STATUS vxbEqepGetDirection
    (
    int eqep
    )
    {
    if (eqepHandle[eqep].pInst == NULL)
        return ERROR;
   
    return eqepHandle[eqep].pEqepFunc->eqepGetDirection(eqepHandle[eqep].pInst);
    }
