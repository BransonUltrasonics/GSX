/* sysLib.c - TI Sitara Cortex-A9 board system-dependent library */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15mar16,r_w  add INCLUDE_DEBUG_KPUTS for BOARD_DESC_FLAG_DBG. (V7COR-3896)
01jul14,swu  created (US40646)
*/

/*
DESCRIPTION
This library provides board-specific routines for Sitara Cortex-A9 
series processor.

INCLUDE FILES:

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/* includes */

#include <vxWorks.h>
#include <boardLib.h>
#include "prjParams.h"
#include <am4xxx.h>

#ifdef INCLUDE_SHOW_ROUTINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ioLib.h>
#include <vmLibCommon.h>
#include <pmapLib.h>
#include <vxFdtLib.h>

/* defines */

#define SCM_DEVICE_ID_OFFSET            0x600
#define SCM_DEVICE_FEATURE_OFFSET       0x604

#define AM437X_DEVID                    0xB98C
#define AM437X_BOARD_MAGIC              0xEE3355AA

LOCAL void boardInfo (void);
IMPORT void cpuArmVerShow (void);

#endif /* INCLUDE_SHOW_ROUTINES */

LOCAL BOARD_FUNC_TBL sitaraFuncTbl = {
    /* .earlyInit  = */ am4xxxEarlyInit,
    /* .init       = */ am4xxxInit,
    /* .reset      = */ am4xxxReset,
    /* .model      = */ am4xxxModel,
    /* .usDelay    = */ am4xxxUsDelay,
#ifdef INCLUDE_SHOW_ROUTINES
    /* .infoShow   = */ boardInfo
#else
    /* .infoShow   = */ NULL
#endif /* INCLUDE_SHOW_ROUTINES */
};

LOCAL BOARD_DESC ti_am437x_idk_evm =
    {
    /* .uVer     = */ BOARD_DESC_VER_2_0,
    /* .pCompat  = */ "ti,sitara-ctxa9",
#if defined(INCLUDE_DEBUG_KPRINTF) || defined(INCLUDE_DEBUG_KPUTS)
    /* .uFlag    = */ BOARD_DESC_FLAG (BOARD_DESC_FLAG_DBG, 0),
#else
    /* .uFlag    = */ BOARD_DESC_FLAG (0, 0),
#endif
    /* .probe    = */ am4xxxProbe,
    /* .pFuncTbl = */ &sitaraFuncTbl
    };

BOARD_DEF (ti_am437x_idk_evm)

#ifdef INCLUDE_SHOW_ROUTINES

/*******************************************************************************
*
* socInfoShow - print SOC information
*
* This routine prints SOC information.
*
* RETURNS: N/A
* ERRNO: N/A
*/

LOCAL void socInfoShow(void)
    {
    void * virtAddr;
    UINT32 phyaddr;
    int    offset;
    UINT32 *prop;
    UINT32 devId;
    UINT32 devFeature;
    UINT32 partNo, rev;

    offset = vxFdtPathOffset("/soc/scm");
    if (offset <= 0)
        {
        printf("ERROR: no scm unit present in this board\n");
        return;
        }
    prop = (UINT32*)vxFdtPropGet (offset, "reg", NULL);
    phyaddr = vxFdt32ToCpu (*prop);
    
    virtAddr = pmapGlobalMap ((PHYS_ADDR) phyaddr,(size_t) 0x1000, 
        MMU_ATTR_SUP_RW | MMU_ATTR_CACHE_OFF | MMU_ATTR_CACHE_GUARDED);
    
    if (virtAddr == PMAP_FAILED)
        {
        printf ("pmapGlobalMap returned ERROR.\n");
        return;
        }
    devId = *(UINT32*)((char*)virtAddr + SCM_DEVICE_ID_OFFSET);
    devFeature = *(UINT32*)((char*)virtAddr + SCM_DEVICE_FEATURE_OFFSET);

    partNo = (devId>>12) & 0xFFFF;
    rev = (devId >> 28) & 0xF;

    printf ("CPU:");
    
    if (partNo == AM437X_DEVID)
        {
        switch (devFeature)
            {
            case 0x22FF60FF:
                printf ("AM437x ");
                break;
            default:
                printf ("unknown SOC ");
                break;
            }
        printf ("Rev ");
        switch (rev)
            {
            case 0:
                printf("1.0");
                break;
            case 1:
                printf("2.0");
                break;
            case 2:
                printf("2.1");
                break;
            default:
                printf("0x%x", rev);
                break;
            }    
        }
    printf(", ");  
    return;
    }

/*******************************************************************************
*
* boardInfo - print board information
*
* This routine prints board information.
*
* RETURNS: N/A
* ERRNO: N/A
*
*/

LOCAL void boardInfo (void)
    {
    char * devName = "/eeprom/0";
    UINT32 length = 16;
    char   readbuf[32] = {0};
    char * pBuf = readbuf;
    int    fd = 0;
    UINT32 tmp;
    
    /* open the EEPROM */

    if ((fd = open(devName, O_RDONLY, 0)) == ERROR)
        {
        printf("eepromIoRead: open the EEPROM failed.\n");
        return;
        }
    
    /* read the data */

    if (read(fd, pBuf, length) == ERROR)
        {
        printf("eepromIoRead: read the EEPROM failed.\n");
        goto Exit;
        }

    tmp = *(UINT32*)pBuf;
    printf ("TI ");
    if (tmp == AM437X_BOARD_MAGIC)
        {
        pBuf += 4;
        if (strncmp(pBuf, "AM43__GP", 8) == 0)
            {
            printf("AM437x GP EVM ");
            }
        else
            {
            printf("Unknown Board ");
            }
        pBuf += 12;
        *pBuf = 0; /* make sure only show 4 byte version */
        pBuf -= 4;
        printf("Rev %s, ",pBuf);
        }
    else
        {
        printf("Unknown Board (unknown revision), ");
        }

    socInfoShow();

    cpuArmVerShow();
Exit:
    close(fd); 
    return;
    }
#endif /* INCLUDE_SHOW_ROUTINES */
