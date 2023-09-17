/* vxbMdioLib.c - vxBus MDIO function interfaces file */

/*
DESCRIPTION

This file implements the vxbus MDIO functionalities. All this functions
are used to operate the MDIO device.

*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <vxbMdioLib.h>

/* locals */

LOCAL struct vxbMdioHandle mdioHandle;

/*****************************************************************************
*
* mdio_Register - register MDIO set/get routine to VxBus mdio subsystem.
*
* This function register MDIO routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void mdio_Register
    (
    VXB_DEV_ID pDev,
    VXB_MDIO_FUNC *mdio
    )
    {
	mdioHandle.pInst = pDev;
	mdioHandle.pMdioFunc = mdio;
    }

/*******************************************************************************
*
* Mdio_Disable_Logs - Disable Logs for pru mdio device
*
* This routine disable logs pru mdio device 
*
* \NOMANUAL
*
* ERRNO: N/A
*/

STATUS Mdio_Disable_Logs  (void)
    {
    if (mdioHandle.pInst == NULL)
        return ERROR;
    return mdioHandle.pMdioFunc->mdioDisable(mdioHandle.pInst);
    }

/*******************************************************************************
*
* Mdio_Enable_Logs - Enable Logs for pru mdio device
*
* This routine enable logs pru mdio device 
*
* \NOMANUAL
*
* ERRNO: N/A
*/

STATUS Mdio_Enable_Logs  (void )
    {
    if (mdioHandle.pInst == NULL)
        return ERROR;
    return mdioHandle.pMdioFunc->mdioEnable(mdioHandle.pInst);
    }

/*******************************************************************************
*
* Mdio_Print_Reg - Print Register
*
* This routine print register 
*
* \NOMANUAL
*
* ERRNO: N/A
*/

STATUS Mdio_Print_Reg (void)
	{
    if (mdioHandle.pInst == NULL)
        return ERROR;
    return mdioHandle.pMdioFunc->mdioPrintReg(mdioHandle.pInst);
    }
