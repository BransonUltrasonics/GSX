/* usrAdjustDtb.c - adjust the dtb address */

/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20jan15,yjw  fix GNU warning (V7PRO-1664)
30oct14,cfm  created (V7PRO-1430)
*/

#include <vxWorks.h>
#include <prjParams.h>

IMPORT void * gpDtbInit;
IMPORT VIRT_ADDR vxMmuEarlyRegMap (PHYS_ADDR physAdrs, ULONG length);

/*****************************************************************************
*
* usrAdjustDtb - adjust the dtb address
*
* When using non-standalone dtb, the dtb is using physcial address, we should
* convert it to virtual address that can be used during early MMU stage.
*
* RETURNS: N/A
*
* \NOMANUAL
*/

LOCAL void usrAdjustDtb (void)
    {
#ifndef INCLUDE_STANDALONE_DTB
#ifdef _WRS_CONFIG_LP64	
    gpDtbInit = (void *)vxMmuEarlyRegMap ((PHYS_ADDR)gpDtbInit, DTB_MAX_LEN);
#else  /* !_WRS_CONFIG_LP64 */

    /* type-cast for clean warning */

    gpDtbInit = (void *)vxMmuEarlyRegMap ((PHYS_ADDR)((UINT32)gpDtbInit), DTB_MAX_LEN);
#endif /* _WRS_CONFIG_LP64 */
#endif /* !INCLUDE_STANDALONE_DTB */
    }
