/* pl310Access.c - PL310 L2 cache registers access configlette */

/*
 * Copyright (c) 2015-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
21oct16,yya  refactored secure mode interface (F6283)
07apr16,gls  added function pointers for register access routines
10nov15,cfm  writen (US69494)
*/

/*
DESCRIPTION
This module provides vxPl310ClearDisable, vxPl310CtrlWrite, vxPl310AuxCtrlWrite
and vxPl310LatCtrlWrite routines.
*/

/* includes */

#include <vxWorks.h>

/* defines */

/* forward declarations */

IMPORT void vxPl310ClearDisable (UINT32 addr, UINT32 ass, 
                          UINT32 ctrl);

IMPORT void vxPl310CtrlWrite (UINT32 addr, UINT32 value);

IMPORT void vxPl310AuxCtrlWrite (UINT32 addr,UINT32 value);

IMPORT void vxPl310LatCtrlWrite (UINT32 tagLat, 
                          UINT32 datLat,
                          UINT32 tagLatCtrl, 
                          UINT32 datLatCtrl);

/* externs */

IMPORT void (* _func_vxPl310ClearDisable) (UINT32 addr, UINT32 ass, 
                                           UINT32 ctrl);

IMPORT void (* _func_vxPl310CtrlWrite) (UINT32 addr, UINT32 value);

IMPORT void (* _func_vxPl310AuxCtrlWrite) (UINT32 addr,UINT32 value);

IMPORT void (* _func_vxPl310LatCtrlWrite) (UINT32 tagLat, 
                                           UINT32 datLat,
                                           UINT32 tagLatCtrl, 
                                           UINT32 datLatCtrl);

/******************************************************************************
*
* vxPl310AccessInit - Initialize L2 cache access routine function pointers
*
* This routine initializes function pointers for accessing P310 cache control
* registers.
*
* RETURNS: NA
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxPl310AccessInit (void)
    {
    _func_vxPl310ClearDisable = vxPl310ClearDisable;
    _func_vxPl310CtrlWrite = vxPl310CtrlWrite;
    _func_vxPl310AuxCtrlWrite = vxPl310AuxCtrlWrite;
    _func_vxPl310LatCtrlWrite = vxPl310LatCtrlWrite;
    }

