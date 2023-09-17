/* armMonitorStub.c - system monitor stub */

/*
 * Copyright (c) 2015-2016 Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
29jul16,g_x  added non-mmu support (F6411)
07sep15,yya  created for warm boot.(US63670)
*/

/* includes */

#include <vxWorks.h>
#include <prjParams.h>

/* imports */

IMPORT void * gpDtbInit;
#ifndef _WRS_CONFIG_MMULESS_KERNEL
IMPORT VIRT_ADDR vxMmuEarlyRegMap (PHYS_ADDR physAdrs, ULONG length);
#endif /* _WRS_CONFIG_MMULESS_KERNEL */

#ifdef INCLUDE_WARM_BOOT
IMPORT void warmBootRestore (ULONG offset);
IMPORT STATUS warmBootCheck (ULONG offset);
IMPORT void warmBootBackup (ULONG adjust);
#endif /* INCLUDE_WARM_BOOT */

/*****************************************************************************
*
* armMonitorStub - reboot monitor stub function in usrInit()
*
* in code boot, this function saves data section to backup area; if warm reboot,
* this function restores the saved data.
*
* RETURNS: N/A
*
* \NOMANUAL
*/

void armMonitorStub
    (
    UINT32 startType
    )
    {
#ifdef INCLUDE_WARM_BOOT
    if (startType == BOOT_COLD)
        {
        warmBootBackup(0);
        }
    else
        {
#ifndef _WRS_CONFIG_MMULESS_KERNEL

        /*
         * DTB address from warm reboot is in physical address, map it when
         * INCLUDE_STANDALONE_DTB option is selected, because the OS assumes
         * that the standalone (actually meaning "built-in") DTB is in virtual
         * address.
         */

#ifdef INCLUDE_STANDALONE_DTB
#ifdef _WRS_CONFIG_LP64	    

        gpDtbInit = (void *)vxMmuEarlyRegMap ((PHYS_ADDR)gpDtbInit, DTB_MAX_LEN);

#else /* !_WRS_CONFIG_LP64 */

	/* type-cast for clean warning */

        gpDtbInit = (void *)vxMmuEarlyRegMap ((PHYS_ADDR)((UINT32)gpDtbInit), DTB_MAX_LEN);
#endif /* _WRS_CONFIG_LP64 */

#endif /* INCLUDE_STANDALONE_DTB */

#endif /* !_WRS_CONFIG_MMULESS_KERNEL */	

        void * temp = gpDtbInit;
        warmBootRestore(0);
        gpDtbInit = temp;
        }

#endif /* INCLUDE_WARM_BOOT */
    }

/*****************************************************************************
*
* armMonitorRebootCheck - check the integrity of the warm boot data
*
* this function checks whether the text section and the saved data is intact.
*
* RETURNS: OK if code and data CRC is correct; otherwise ERROR.
*
* \NOMANUAL
*/

STATUS armMonitorRebootCheck (void)
    {
#ifdef INCLUDE_WARM_BOOT
    return warmBootCheck(0);
#else
    return OK;
#endif
    }

/*****************************************************************************
*
* armMonitorStackGet - get the stack used in reboot monitor
*
* This function returns an address which will be momentarily used in the reboot
* monitor. As an requirement when calling the C function usrInit(), the stack
* must be initialized. we deliberately avoid inherit the stack of the task
* that requests the reboot because if the reboot is trigger by a data abort due
* to invalid stack, then inherit that stack is double fault forever.
* Here we assume that there is some space available under RAM_LOW_ADRS and
* can be used temporarily as stack space.
* TODO: introduce a BSP rewritable MACRO, such as USER_INIT_STACK_ADRS, so that
* BSP can specify more convenient address to be used as temporary stack, such
* as SRAM on some systems.
*
* RETURNS: temporary stack address
*
* \NOMANUAL
*/

char * armMonitorStackGet (void)
    {
    return (char *)(RAM_LOW_ADRS);
    }
