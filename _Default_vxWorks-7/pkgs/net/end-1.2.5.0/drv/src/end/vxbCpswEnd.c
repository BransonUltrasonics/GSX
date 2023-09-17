/* vxbCpswEnd.c - TI 3 port gigabit switch VxBus END driver */

/*
 * Copyright (c) 2011-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
09nov16,mca  silenced a 'declared but never referenced' warning
19nov15,yjw  add am57xx support (US64527)
06aug15,m_w  initlialize the ifPollInterval in EIOCGPOLLCONF case. (V7PRO-2249)
23oct14,m_w  fixed polling statistics error. (V7PRO-1409)
08jul14,m_w  add support CPSW on TI AM437x.(US40646)
             update to fetch out mac address from on-chip register CTRL_MAC_ID
             with high priority than local-mac-address(DTS).
27aug14,sye  fixed checked return issues. (US44214)
10jun14,c_t  add support for Beaglebone Black (US40510)
06may14,l_z  remove vxbMiiMethod.h header declare. (US33490)
05may14,rjq  Decouple END with COREIP. (V7NET-105)
18sep13,c_t  adapt to vxbus gen2
01apr13,c_l  Remove build warnings. (WIND00409411)
23oct12,my_  workaround for am335x errata 1.0.9 (WIND00384090)
19oct12,my_  fix wdb system mode failure (WIND00383259)
24sep12,my_  fix coverity warnings (WIND00378296)
13sep12,my_  add documentation about promisc mode
             add default port vlan definition
             correct the documentation about regBase4 and
             regBase5; re-implement dual emac support by using
             vlan table rather than putting ALE into
             bypass mode thus accord with the manual; add
             basic promisc mode support; improve error output
             in cpswEndErrHandle()
             improve cpswAleEntryShow() to show more
             information; change the prototype of
             cpswAleAddMultiCast() and cpswAleAddUniCast()
02jul12,my_  modify cpswEndStop() to give rx/tx handler a
             chance to finish during reboot (WIND00357187)
20may12,my_  add support for am335x
19apr12,clx  add "\ce" to fix error with documentation generation
             (WIND00345783)
19apr12,fao  fix apigen build error. (WIND00346008)
07mar12,my_  fix a typo which causes second gmac not working
             under certain conditions (WIND00337783).
             fix rx misqueue handling
01aug11,my_  written
*/

/*
DESCRIPTION
This module implements an END driver for the TI 3 port gigabit switch
(CPSW) network interface. The TI CPSW is a layer 2 ethernet switch.
The TI CPSW combines 2 10/100/1000 MAC ports and a DMA/CPU port.

However this driver doesn't support CPSW operating in switch mode.
Instead this driver configures the CPSW to operate in dual EMAC mode
by disabling address learning on the 2 GMAC ports and constructing
the MAC address look-up table manually during intialization.

To VxBus, there will only be one device instance, however
to support two devices, we need two END objects, as well as two private
device contexts. To implement this, the pDrvCtrl structure attached to
the VxBus device instance for a dual link device is actually an array of
two pDrvCtrls. Note that this means that it's not possible to unload
a single network interface on a dual link device: if the VxBus device
instance is deleted, that will cause both interfaces to be removed.

This driver supports promisc mode by setting ALE (Address Lookup Engine)
into bypass mode. In bypass mode, all packets received will be forwarded
only to host port and ingress checking is ignored. Because there are two
ethernet ports inside the CPSW, so there are two END objects showing up
when calling ifconfig shell command and because ALE is a shared resouce
so if one port is put into promisc mode then the other will also be put
into promisc mode automatically, but interface capability flags don't change
automactically. For example, calling:
\cs
ifconfig "cpsw0 inet promisc"
\ce
will put both cpsw0 and cpsw1 into promisc mode, but only cpsw0's
capability flags will show:

\cs
UP RUNNING SIMPLEX BROADCAST MULTICAST PROMISC ALLMULTI
\ce

cpsw1's capability flags will not be changed, the following command
must be called for cpsw1 to show up correctly:

\cs
ifconfig "cpsw1 inet promisc"
\ce

This same is true when removing promisc mode.

BOARD LAYOUT
The network interfaces are internal to the CPU. All configurations
are jumperless. See target.ref for connector locations.

DEVICE TREE BINDING
This driver is bound to device tree, and the device tree node must specify
below parameters:

\is

\i <reg>
This parameter specifies the register base address and length of this module.

\i <interrupt-parent>
This parameter specifies the offset of interrupt controller.

\i <interrupts>
This parameter specifies the interrupt number of this module.

\i <compatible>
This parameter specifies the name of the CPSW controller driver. It must be
"ti,cpsw-ma" for the master device, and "ti,cpsw-sl" for the slave port.

\ie

Following parameter is master device only:

\is

\i <cpsw-ver>
This parameter specifies the CPSW version. 1 for AM38XX, 0 for AM335x.

\i <cpsw-dulemac>
1 for dul-emac mode, or 0 for switch mode. This driver only support dul-emac
currently.

\i <cpsw-port0-offset>
cpsw port control register offset. Made it as 0x108/0x208 for compatible with
AM38xx/AM35xx.

\i <cpsw-stats-offset>
statistics register offset.

\i <cpsw-cpdma-offset>
cpdma register offset.

\i <cpsw-ale-offset>
ALE register offset.

\i <cpsw-mdio-offset>
MDIO register offset

\i <cpsw-wr-offset>
CPSW_WR offset.

\ie

Following parameter is slave device only:

\is

\i <cpsw-portid>
port 0 is host port, so this parameter must be 1 or 2.

\i <cpsw-sl>
SL register offset

\i <cpsw-port>
PORT control register offset.

\ie

\cs

This routine queries the device tree to provide the ethernet address
for a given MAC.

\ce

Note: This driver only supports dual emac mode, switch mode is not supported.

RESTRICTIONS

SEE ALSO: VxBus, miiBus, ifLib
*/

#include <vxWorks.h>
#include <string.h>
#include <intLib.h>
#include <stdio.h>
#include <string.h>
#include <netLib.h>
#include <netBufLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <wdLib.h>
#include <vmLib.h>
#include <etherMultiLib.h>
#include <end.h>
#define END_MACROS
#include <endLib.h>
#include <endMedia.h>
#include <cacheLib.h>
#include <spinLockLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <miiBus.h>
#include "vxbCpswEnd.h"

/* defines */

/* debug macro */

#undef CPSW_DEBUG
#ifndef CPSW_DEBUG
#include <private/kwriteLibP.h>         /* _func_kprintf */

#define DBG_OFF             0x00000000
#define DBG_WARN            0x00000001
#define DBG_ERR             0x00000002
#define DBG_INFO            0x00000004
#define DBG_ALL             0xffffffff
LOCAL UINT32 dbgMask = DBG_ALL;

#undef DBG_MSG

#define DBG_MSG(mask,...)                                      \
do                                                             \
{                                                              \
    if ((dbgMask & (mask)) || ((mask) == DBG_ALL))             \
        {                                                      \
        if (_func_kprintf != NULL)                             \
            {                                                  \
            (* _func_kprintf)("%s,%d, ",__FUNCTION__,__LINE__);\
            (* _func_kprintf)(__VA_ARGS__);                    \
            }                                                  \
        }                                                      \
}while (0)
#else
#define DBG_MSG(...)
#endif  /* CPSW_DEBUG */

/* typedef */

typedef struct CpswSlaveDevInfo {
	struct vxbFdtDev vxbFdtDev;
    VXB_RESOURCE_LIST vxbResList;
} CPSW_SL_DEV_INFO;

LOCAL UINT32 cpdmaHDPoffset = 0;

/* import functions */

IMPORT FUNCPTR   _func_m2PollStatsIfPoll;
IMPORT STATUS    sysNetMacNVRamAddrGet (char *, int, UINT8 *, int);
IMPORT void      vxbUsDelay            (int);

/* driver routines */

LOCAL STATUS     cpswMaProbe           (VXB_DEV_ID);
LOCAL STATUS     cpswMaAttach          (VXB_DEV_ID);
LOCAL VXB_FDT_DEV  * slaveDevInfo      (VXB_DEV_ID, VXB_DEV_ID);
LOCAL VXB_RESOURCE * cpswMaResAlloc    (VXB_DEV_ID, VXB_DEV_ID, UINT32);
LOCAL STATUS     cpswAttachSlave       (VXB_DEV_ID);

/* host functions */

LOCAL void       cpswEndErrHandle  (CPSW_HOST_CTRL *);
LOCAL STATUS     cpswHostPortInit  (VXB_DEV_ID);
LOCAL STATUS     cpswGmacPortInit  (CPSW_MAC_CTRL *);
LOCAL void       cpswEndInt        (CPSW_HOST_CTRL *);
LOCAL STATUS     cpswSoftReset     (VXB_DEV_ID, UINT32, UINT32);
LOCAL void       cpswIntHandle     (void *);

/* CPDMA routines */

LOCAL STATUS     cpdmaInit         (VXB_DEV_ID);
LOCAL STATUS     cpdmaChanRelease  (VXB_DEV_ID, BOOL, int);
LOCAL STATUS     cpdmaChanRequest  (VXB_DEV_ID, BOOL, int);

/* ALE routines */

LOCAL int    cpswAleAddUniCast     (VXB_DEV_ID, UCHAR *, int, int, int, int);
LOCAL void   cpswAleRead           (VXB_DEV_ID, CPSW_ALE_TBL * , UINT32);
LOCAL void   cpswAleWrite          (VXB_DEV_ID, CPSW_ALE_TBL * , UINT32);
LOCAL int    cpswAleFind           (VXB_DEV_ID);
LOCAL int    cpswAleMatch          (VXB_DEV_ID, UCHAR *);
LOCAL int    cpswAleMatchVlan      (VXB_DEV_ID, int);
LOCAL int    cpswAleAddVlan        (VXB_DEV_ID, int, int, int, int, int);
LOCAL int    cpswAleAddMultiCast   (VXB_DEV_ID, UCHAR*, int, int, int, int);
#ifndef _WRS_CONFIG_CERT
LOCAL void   cpswAleEntryShow      (CPSW_ALE_TBL *);
LOCAL void   cpswAleDump           (VXB_DEV_ID);
#endif

/* driver utility functions */

LOCAL VXB_DRV_METHOD cpswMaMethods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),     (FUNCPTR)cpswMaProbe     },
    { VXB_DEVMETHOD_CALL(vxbDevAttach),    (FUNCPTR)cpswMaAttach    },
    { VXB_DEVMETHOD_CALL(vxbFdtDevGet),    (FUNCPTR)slaveDevInfo    },
    { VXB_DEVMETHOD_CALL(vxbResourceAlloc),(FUNCPTR)cpswMaResAlloc  },
    { 0, 0 }
};

VXB_DRV  cpswMaDrv =
    {
    { NULL } ,
    "TI CPSW Master",           /* drvName */
    "TI CPSW master device",    /* Description */
    VXB_BUSID_FDT,              /* Class */
    0,                          /* Flags */
    0,                          /* Reference count */
    cpswMaMethods               /* Method table */
    };

VXB_DRV_DEF(cpswMaDrv)

/* register our own bus */

BUSTYPE_DEF(vxbCpswBus, "CPSW internal bus type");
#define VXB_BUSID_CPSW   BUSTYPE_ID(vxbCpswBus) /* CPSW internal BUS */

LOCAL VXB_FDT_DEV_MATCH_ENTRY cpswMaMatch[] =
    {
        {
        "ti,cpsw-ma",  /* compatible */
        NULL,
        },
        {} /* Empty terminated list */
    };

/* slave */

#include "vxbCpswSlEnd.c"


/*********************************************************************
*
* cpswMaProbe - check whether device and driver go together
*
* This routine probe a device with a device driver.
*
* RETURNS: OK if the device and driver match up, ERROR otherwise
*
* ERROR: N/A
*/

LOCAL STATUS cpswMaProbe
    (
    VXB_DEV_ID pDev
    )
    {
    return vxbFdtDevMatch (pDev, cpswMaMatch, NULL);
    }

/*********************************************************************
*
* cpswMaAttach - Initilize the cpsw master device.
*
* This routine initilize the cpsw master device.
*
* RETURNS: OK or ERROR if failed.
*
* ERROR: N/A
*/

LOCAL STATUS cpswMaAttach
    (
    VXB_DEV_ID pDev
    )
    {
    CPSW_HOST_CTRL * pHost;
    VXB_FDT_DEV *pFdtDev = (VXB_FDT_DEV *)vxbDevIvarsGet(pDev);
    int proplen = 0;
    UINT32 *prop;
    VXB_RESOURCE_ADR * pResAdr = NULL;
    VXB_RESOURCE * pRes;

    pHost = vxbMemAlloc (sizeof (CPSW_HOST_CTRL));
    if (pHost == NULL)
        {
        DBG_MSG (DBG_ERR,"vxbMemAlloc failed\n");
        return ERROR;
        }

    pHost->pDev    = pDev;
    vxbDevSoftcSet(pDev, pHost);

    /* get sub-module register offsets */

    pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);
    if(pRes == NULL)
        return ERROR;

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;

    if (pResAdr == NULL)
        return ERROR;

    pHost->regBase     = (void*)pResAdr->virtual;
    pHost->handle      = pResAdr->pHandle;

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-ver", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"cpsw version missed\n");
        goto failed;
        }
    pHost->chipVersion = vxFdt32ToCpu(*prop);

    if (pHost->chipVersion == TI814X_DEV_ID)
        {
        cpdmaHDPoffset = 0x100;
        }
    else if (pHost->chipVersion == TIAM335X_DEV_ID
          || pHost->chipVersion == TIAM437X_DEV_ID
          || pHost->chipVersion == TIAM57XX_DEV_ID)
        {
        cpdmaHDPoffset = 0x200;
        }
    else
        {
        DBG_MSG (DBG_ERR,"SoC is not supported\n");
        goto failed;
        }

    /*
     * workround for CPSW on 335x, use dmtimer5/dmtimer6 as interrupt reporter
     * to CPU when we get C0_TX_PEND and C0_RX_PEND Interrupts.
     * Advisory 1.0.9 of am335x silicon errata 2.1:
     * C0_TX_PEND and C0_RX_PEND Interrupts Not Connected to ARM Cortex-A8.
     */

    if (pHost->chipVersion == TIAM335X_DEV_ID)
        {
        pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 1);
        if (pRes != NULL)
            {
            pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;

            if (pResAdr == NULL)
                return ERROR;
            pHost->dm5Base = (void*)pResAdr->virtual;
            }

        pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 2);
        if (pRes != NULL)
            {
             pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;

            if (pResAdr == NULL)
                return ERROR;
            pHost->dm6Base = (void*)pResAdr->virtual;
            }
        }

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset,
                                  "cpsw-port0-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-port0-offset property found\n");
        goto failed;
        }
    pHost->portOffset  = vxFdt32ToCpu(*prop);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-stats-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-stats-offset property found\n");
        goto failed;
        }
    pHost->statsOffset = vxFdt32ToCpu(*prop);

    prop = (UINT32 *)vxFdtPropGet (pFdtDev->offset,
                                   "cpsw-cpdma-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-cpdma-offset property found\n");
        goto failed;
        }
    pHost->cpdmaOffset = vxFdt32ToCpu(*prop);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-ale-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-ale-offset property found\n");
        goto failed;
        }
    pHost->aleOffset   = vxFdt32ToCpu(*prop);

    prop = (UINT32 *)vxFdtPropGet (pFdtDev->offset,
                                   "cpsw-mdio-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-mdio-offset property found\n");
        goto failed;
        }
    pHost->mdioOffset  = vxFdt32ToCpu(*prop);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-wr-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-wr-offset property found\n");
        goto failed;
        }
    pHost->wrOffset    = vxFdt32ToCpu(*prop);

    /* is it possible to add more port?? */

    pHost->portnr = 0;

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-dulemac", &proplen);
    if (prop == NULL || vxFdt32ToCpu(*prop) != 1)
        {
        DBG_MSG (DBG_ERR,"Currently only dual emac mode is supported. \
                 Please set dualEmac to 1 in device tree\n");

        goto failed;
        }
    else
        {
        pHost->nrMacPorts = 2;
        pHost->dualEmac = TRUE;
        }

    SPIN_LOCK_ISR_INIT (&pHost->aleLock, 0);

    pHost->cpswHostSem =
        semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
    if (pHost->cpswHostSem == NULL)
        {
        DBG_MSG (DBG_ERR,"Can not create semaphore\n");

        goto failed;
        }

    /* enable control registers & pinmux */

    (void) vxbPinMuxEnable (pDev);
    (void) vxbClkEnableAll (pDev);

    /* initialize host(cpu) port */

    if (cpswHostPortInit (pDev) != OK)
        {
        DBG_MSG (DBG_ERR,"cpswHostPortInit() return error\n");
        goto failed;
        }

    /* initialize CPDMA */

    if (cpdmaInit (pDev) != OK)
        {
        DBG_MSG (DBG_ERR,"cpdmaInit() return error\n");
        goto failed;
        }

    /* here we call slave's attach function. */

    if (cpswAttachSlave (pDev) == ERROR)
        {
        DBG_MSG (DBG_ERR,"cpswAttachSlave() return error\n");
        goto failed;
        }

    (void)vxAtomicSet (&pHost->cpswIntPending, FALSE);

    /* attach our ISR(s) */

    pRes = vxbResourceAlloc (pDev, VXB_RES_IRQ, 0);
    if (pRes == NULL)
        {
        DBG_MSG (DBG_ERR,"vxbResourceAlloc failed for ISR 0\n");
        goto failed;
        }

    if (vxbIntConnect (pDev, pRes, cpswEndInt, pHost) != OK)
        {
        DBG_MSG (DBG_ERR,"can not connect ISR 0\n");
        goto failed;
        }
    if (vxbIntEnable  (pDev, pRes) != OK)
        {
        DBG_MSG (DBG_ERR,"can not enable ISR 0\n");
        goto failed;
        }

    pRes = vxbResourceAlloc (pDev, VXB_RES_IRQ, 1);
    if (pRes == NULL)
        {
        DBG_MSG (DBG_ERR,"vxbResourceAlloc failed for ISR 1\n");
        goto failed;
        }

    if (vxbIntConnect (pDev, pRes, cpswEndInt, pHost) != OK)
        {
        DBG_MSG (DBG_ERR,"can not connect ISR 1\n");
        goto failed;
        }
    if (vxbIntEnable  (pDev, pRes) != OK)
        {
        DBG_MSG (DBG_ERR,"can not enable ISR 1\n");
        goto failed;
        }

    pRes = vxbResourceAlloc (pDev, VXB_RES_IRQ, 2);
    if (pRes == NULL)
        {
        DBG_MSG (DBG_ERR,"vxbResourceAlloc failed for ISR 2\n");
        goto failed;
        }

    if (vxbIntConnect (pDev, pRes, cpswEndInt, pHost) != OK)
        {
        DBG_MSG (DBG_ERR,"can not connect ISR 2\n");
        goto failed;
        }
    if (vxbIntEnable  (pDev, pRes) != OK)
        {
        DBG_MSG (DBG_ERR,"can not enable ISR 2\n");
        goto failed;
        }
    return OK;

failed:

    if (pHost->cpswHostSem)
        (void) semDelete (pHost->cpswHostSem);

    vxbMemFree (pHost);
    vxbDevSoftcSet (pDev, NULL);

    return ERROR;
    }

/*******************************************************************************
*
* slaveDevInfo - get the FDT child device information
*
* This routine gets the FDT child device information
*
* \NOMANUAL
*
* RETURNS: the device information pointer
*
* ERRNO: N/A
*/

LOCAL VXB_FDT_DEV *  slaveDevInfo
    (
    VXB_DEV_ID pDev,
    VXB_DEV_ID pChild
    )
    {
    CPSW_SL_DEV_INFO * pSlDevInfo;

    if (pChild == NULL)
        return NULL;

    pSlDevInfo = vxbDevIvarsGet(pChild);

    if (pSlDevInfo == NULL)
        return NULL;

    return &pSlDevInfo->vxbFdtDev;
    }


/*******************************************************************************
*
* cpswMaResAlloc - vxbus alloc interface
*
* This routine will be used by child of this VXB_DEV to find the resource value.
*
* RETURNS: point of resource when success, NULL for others.
*
* ERRNO: N/A
*/

LOCAL VXB_RESOURCE * cpswMaResAlloc
    (
    VXB_DEV_ID pDev,
    VXB_DEV_ID pChild,
    UINT32 id
    )
    {
    CPSW_SL_DEV_INFO * pDevInfo;
    VXB_RESOURCE     * vxbRes;

    pDevInfo = (CPSW_SL_DEV_INFO *)vxbDevIvarsGet(pChild);

    if (pDevInfo == NULL)
        return NULL;

    vxbRes = vxbResourceFind(&pDevInfo->vxbResList, id);

    if (vxbRes == NULL)
        return NULL;

    if (((VXB_RES_TYPE(vxbRes->id) == VXB_RES_MEMORY) ||
         (VXB_RES_TYPE(vxbRes->id) == VXB_RES_IO)) &&
        (vxbRegMap (vxbRes) == OK))
        {
        return vxbRes;
        }
    else if ((VXB_RES_TYPE(vxbRes->id) == VXB_RES_IRQ) &&
        (vxbIntMap (vxbRes) == OK))
        return vxbRes;
    else
        {
        return NULL;
        }
    }


/*******************************************************************************
*
* cpswAttachSlave - Attach internal slave device
*
* This function implements the VxBus gen2 bus attach routine for an CPSW
* slave device instance.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS cpswAttachSlave
    (
    VXB_DEV_ID  pDev
    )
    {
    int         offset;
    VXB_FDT_DEV * pFdtDev;
    CPSW_SL_DEV_INFO * pSlaveDevInfo;
    VXB_FDT_DEV * pNewFdtDev;
    VXB_DEV_ID  pCur = NULL;

    DEBUG_MSG("AttachSlave enter\r\n", 0, 1, 2, 3, 4, 5);

    pFdtDev = vxbFdtDevGet(pDev);

    if (pFdtDev == NULL)
        {
        DEBUG_MSG ("cpswAttachSlave: pFdtDev is NULL!\r\n", 0, 1, 2, 3, 4, 5);
        return ERROR;
        }
    offset = pFdtDev->offset;

    DEBUG_MSG ("AttachSlave start to search new device from offset %d\r\n",
               offset, 1, 2, 3, 4, 5);

    for (offset = VX_FDT_CHILD(offset); offset > 0;
         offset = VX_FDT_PEER(offset))
        {
        pCur = NULL;

        if (vxFdtIsEnabled(offset) == FALSE)
            continue;

        if (vxbDevCreate (&pCur) != OK)
            continue;

        pSlaveDevInfo = (CPSW_SL_DEV_INFO *)vxbMemAlloc(
                                        sizeof(*pSlaveDevInfo));
        if (pSlaveDevInfo == NULL)
            {
            (void)vxbDevDestroy(pCur);
            continue;
            }

        pNewFdtDev = &pSlaveDevInfo->vxbFdtDev;

        pNewFdtDev->offset = offset;

        /* Get the device basic infomation  */

        vxbFdtDevSetup(offset, pNewFdtDev);
        vxbDevNameSet(pCur, pNewFdtDev->name, FALSE);

        /* Assign the bus internal variable and type  */

        vxbDevIvarsSet(pCur, (void *)pSlaveDevInfo);
        vxbDevClassSet(pCur, VXB_BUSID_CPSW);

        /* Get the device register and interrupt infomation  */

        if (vxbResourceInit(&pSlaveDevInfo->vxbResList) != OK)
            {
            (void)vxbDevDestroy(pCur);
            vxbMemFree (pSlaveDevInfo);
            continue;
            }

        /* Fetched out resource info from DTS for this devide */

        if (vxbFdtRegGet(&pSlaveDevInfo->vxbResList, pNewFdtDev) != OK)
            {
            vxbMemFree(pSlaveDevInfo);
            (void)vxbDevDestroy(pCur);
            continue;
            }

        (void)vxbDevAdd(pDev, pCur);
        }

    DEBUG_MSG("AttachSlave end of search device\r\n", 0, 1, 2, 3, 4, 5);

    return OK;
    }

/*******************************************************************************
*
* cpswHostPortInit - initialize CPSW host port
*
* This routine initializes the CPSW host port. It performs a soft reset, clears &
* enables the ALE module (NOTE: ALE must be enabled otherwise all packets will be
* dropped), and puts port in forward state. MDIO module is enabled in this routine
* (NOTE: if soft reset fails, the routine won't proceed. An error message will be
* print out if debug mode is enabled)
*
* RETURNS: OK or ERROR if reset timeout
*
* ERRNO: N/A
*/

LOCAL STATUS cpswHostPortInit
    (
    VXB_DEV_ID pDev
    )
    {
    CPSW_HOST_CTRL * pHost = vxbDevSoftcGet(pDev);
    UINT32 base, val;

    if (cpswSoftReset (pDev, CPSW_SOFT_RESET, CPSW_TIMEOUT_VAL) != OK)
        {
        DBG_MSG (DBG_ERR,"host port soft reset timeout\n");
        return ERROR;
        }

    base = pHost->aleOffset;

    val = CPSW_ALE_EN_TABLE | CPSW_ALE_CLR_TABLE;
    if (pHost->dualEmac == TRUE)
        val |= CPSW_ALE_VLAN_AWARE;

    CSR_WRITE_4 (pDev, base + CPSW_ALE_CONTROL, val);
    CSR_WRITE_4 (pDev, base + CPSW_ALE_PORTCTL(pHost->portnr),
                    CPSW_ALE_PORT_FW | CPSW_ALE_CTL_NO_LEARN);

    /* enable statistics for all ports */

    CSR_WRITE_4 (pDev, CPSW_STAT_PORT_EN, 7);

    base = pHost->portOffset;
    CSR_WRITE_4 (pDev, base + CPSW_TX_PRI_MAP, CPSW_TX_PRI_MAP_DFTL);
    CSR_WRITE_4 (pDev, base + CPSW_CPDMA_TX_PRI_MAP, CPDMA_TX_PRI_MAP_DFTL);

    if (pHost->dualEmac == TRUE)
        {
        /* set tx fifo into dual emac mode */

        val = CSR_READ_4 (pDev, base + CPSW_TX_IN_CTL);
        val = val & ~(CPSW_TX_IN_SEL_MSK << CPSW_TX_IN_SEL_SHIFT);
        CSR_WRITE_4 (pDev, base + CPSW_TX_IN_CTL,
                val | (CPSW_TX_FIFO_DUAL_EMAC << CPSW_TX_IN_SEL_SHIFT));
        }

    base = pHost->mdioOffset;
    CSR_WRITE_4 (pDev, base + CPSW_MDIO_CONTROL,
                    CPSW_MDIO_EN | CPSW_MDIO_CLK_DIV);

    return OK;
    }

/*******************************************************************************
*
* cpdmaChanRequest - request a DMA channel
*
* This routine allocates a DMA resource for a given request. The requested
* channel number is specified through the parameter "idx". If "rx" is TRUE, then
* it is asssumed the allocation is for rx channel otherwise it is assumed for
* tx channle. DMA is shared resource, so this routine must be called with
* semaphore taken.
*
* RETURNS: OK or ERROR if allocation fails.
*
* ERRNO: N/A
*/

LOCAL STATUS cpdmaChanRequest
    (
    VXB_DEV_ID pDev,
    BOOL rx,
    int idx
    )
    {
    CPSW_HOST_CTRL * pHost = vxbDevSoftcGet(pDev);
    unsigned char * chan = rx == TRUE ? pHost->cpswRxDmaChans \
               : pHost->cpswTxDmaChans;

    if (0 <= idx && idx < NR_DMA_CHANS)
        {
        if (!chan[idx])
            {
            chan[idx] = 1;
            return OK;
            }
        }

    return ERROR;
    }

/*******************************************************************************
*
* cpdmaChanRelease - release a DMA channel
*
* This routine releases a DMA resource for a given request. The requested
* channel number is specified through the parameter "idx". If "rx" is TRUE, then
* it is asssumed the release operation is for rx channel otherwise it is assumed
* for tx channle. If a already-free channel is given for release, this routine
* also returns OK. DMA is shared resource, so this routine must be called with
* semaphore taken.
*
* RETURNS: OK or ERROR if release operation fails.
*
* ERRNO: N/A
*/

LOCAL STATUS cpdmaChanRelease
    (
    VXB_DEV_ID pDev,
    BOOL rx,
    int idx
    )
    {
    CPSW_HOST_CTRL * pHost = vxbDevSoftcGet(pDev);
    unsigned char * chan = rx == TRUE ? pHost->cpswRxDmaChans \
               : pHost->cpswTxDmaChans;

    if (0 <= idx && idx < NR_DMA_CHANS)
        {
        if (!chan[idx])
            {
            return OK;
            }
        else
            {
            chan[idx] = 0;
            return OK;
            }
        }
    return ERROR;
    }

/*******************************************************************************
*
* cpdmaInit - initialize CPDMA module
*
* This routine initializes the CPDMA module inside the NIC. This routine will
* perform a soft reset, clear all the rx/tx header decriptor pointer/completion
* header pointer, disable interrupts and set the CPDMA control register to
* default value:
*
*     frames containing overrun error are filtered
*     DMA writes offset/len field in the descritpor
*     DMA clears the owership bit
*     All 8 DMA queue uses the round-robin scheduling algorithm
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL STATUS cpdmaInit
    (
    VXB_DEV_ID pDev
    )
    {
    int i;
    CPSW_HOST_CTRL * pHost = vxbDevSoftcGet(pDev);

    /* reset the cpdma module */

    if (cpswSoftReset (pDev, pHost->cpdmaOffset + CPDMA_SOFT_RESET,
                            CPSW_TIMEOUT_VAL) != OK)
        {
        DBG_MSG (DBG_ERR,"dma port soft reset timeout\n");
        return ERROR;
        }

    /* initialize dma queue header pointers */

    for (i = 0; i < NR_DMA_CHANS; i++)
        {
        CPDMA_REG_WRITE (pDev, CPDMA_TX_HDP(i)    , 0);
        CPDMA_REG_WRITE (pDev, CPDMA_RX_HDP(i)    , 0);
        CPDMA_REG_WRITE (pDev, CPDMA_TX_CP(i)     , 0);
        CPDMA_REG_WRITE (pDev, CPDMA_RX_CP(i)     , 0);
        CPDMA_REG_WRITE (pDev, CPDMA_RX_FREEBUF(i), 0);
        }

    CPDMA_REG_WRITE (pDev, CPDMA_DMACONTROL    , CPDMA_CONTROL_DEFAULT);

    /* diable interrupts */

    CPDMA_REG_WRITE (pDev, CPDMA_RX_BUFOFFSET  , 0x0);
    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_CLR, 0xffff);
    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_CLR, 0xffff);
    CPDMA_REG_WRITE (pDev, CPDMA_INTMASK_CLR   , 0x3);

    return OK;
    }

/*******************************************************************************
*
* cpswSoftReset - perform soft reset on a given module
*
* This routine performs soft reset on a given module.
*
* RETURNS: OK or ERROR if timeout
*
* ERRNO: N/A
*/

LOCAL STATUS cpswSoftReset
    (
    VXB_DEV_ID pDev,
    UINT32 reg,
    UINT32 timeout
    )
    {
    UINT32 i = 0;
    UINT32 ret;

    CSR_WRITE_4 (pDev, reg, 0x1);
    do
        {
        ret = CSR_READ_4 (pDev, reg);
        } while ((ret & 0x1) && (i++ < timeout));

    if (i >= timeout)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* cpswAleRead - read an ALE entry
*
* This routine reads an ALE entry from the given index
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswAleRead
    (
    VXB_DEV_ID pDev,
    CPSW_ALE_TBL * tbl,
    UINT32 entry
    )
    {
    CPSW_HOST_CTRL * pHost = vxbDevSoftcGet(pDev);

    SPIN_LOCK_ISR_TAKE (&pHost->aleLock);

    CSR_WRITE_4 (pDev, pHost->aleOffset + CPSW_ALE_TBLCTL,
                 entry & CPSW_ALE_ENTRY_IDX_MASK);

    tbl->word0 = CSR_READ_4 (pDev, pHost->aleOffset + CPSW_ALE_WORD0);
    tbl->word1 = CSR_READ_4 (pDev, pHost->aleOffset + CPSW_ALE_WORD1);
    tbl->word2 = CSR_READ_4 (pDev, pHost->aleOffset + CPSW_ALE_WORD2) & 0xf;

    SPIN_LOCK_ISR_GIVE (&pHost->aleLock);
    }

/*******************************************************************************
*
* cpswAleWrite - write an ALE entry
*
* This routine writes data to an ALE entry with the given index
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswAleWrite
    (
    VXB_DEV_ID pDev,
    CPSW_ALE_TBL * tbl,
    UINT32 entry
    )
    {
    CPSW_HOST_CTRL * pHost = vxbDevSoftcGet(pDev);

    SPIN_LOCK_ISR_TAKE (&pHost->aleLock);

    CSR_WRITE_4 (pDev, pHost->aleOffset + CPSW_ALE_WORD0, tbl->word0);
    CSR_WRITE_4 (pDev, pHost->aleOffset + CPSW_ALE_WORD1, tbl->word1);
    CSR_WRITE_4 (pDev, pHost->aleOffset + CPSW_ALE_WORD2, tbl->word2 & 0xf);

    CSR_WRITE_4 (pDev, pHost->aleOffset + CPSW_ALE_TBLCTL,
                 (entry & CPSW_ALE_ENTRY_IDX_MASK) | CPSW_ALE_WRITE);

    SPIN_LOCK_ISR_GIVE (&pHost->aleLock);
    }

/*******************************************************************************
*
* cpswAleFind - find an emtpy ALE entry
*
* This routine finds an emtpy ALE entry.
*
* RETURNS: empty entry index or -1 if ALE is full
*
* ERRNO: N/A
*/

LOCAL int cpswAleFind
    (
    VXB_DEV_ID pDev
    )
    {
    int i;
    CPSW_ALE_TBL tbl;

    for (i = 0; i < CPSW_ALE_ENTRY_NR; i++)
        {
        cpswAleRead (pDev, &tbl, i);
        if ((tbl.word1 & CPSW_ALE_ENTRY_MASK) == 0x0)
            return i;
        }

    return -1;
    }

/*******************************************************************************
*
* cpswAleMatch - find the entry with the given MAC address
*
* This routine finds the entry with the given MAC address
*
* RETURNS: entry found or -1 if the MAC address has not entered into ALE yet.
*
* ERRNO: N/A
*/

LOCAL int cpswAleMatch
    (
    VXB_DEV_ID pDev,
    unsigned char * mac
    )
    {
    int i;
    CPSW_ALE_TBL t;
    unsigned int word0, word1;

    word0 = (mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | (mac[5]);
    word1 = (mac[0] << 8) | (mac[1]);

    for (i = 0; i < CPSW_ALE_ENTRY_NR; i++)
        {
        cpswAleRead (pDev, &t, i);
        if ((t.word0 == word0) && ((t.word1 & 0xffff) == word1))
            return i;
        }
    return -1;
    }

#ifndef _WRS_CONFIG_CERT
/*******************************************************************************
*
* cpswAleEntryShow - show an ALE entry
*
* This routine outputs verbose message of an ALE entry
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswAleEntryShow
    (
    CPSW_ALE_TBL * tbl
    )
    {
    unsigned char * macaddr;
    unsigned char * macaddr1;
    int block = 0, secure = 0, port = 0, portmask = 0;
    int fwstate = 0, super = 0;
    int unicasttype = 0;
    int vmem = 0, umf = 0, rmf = 0, fue = 0;
    char * str = NULL;

    static char * unicast [] =
        {
        "u/na",
        "u/a/nt",
        "oui",
        "u/a/t"
        };

    macaddr = (unsigned char *)&tbl->word0;
    macaddr1 = (unsigned char *)&tbl->word1;
    int vid = (tbl->word1 >> 16) & 0xfff;
    int type = (tbl->word1 >> 28) & 0x3;
    int multicast = tbl->word1 & 0x100;

    if (type == 0x0)
        return;

    if (type == 1)
        {
        if (multicast)
            str = "multicast";
        else
            str = "unicast";
        }
    else if (type == 0x3)
        {
        if (multicast)
            str = "multi/vlan";
        else
            str = "uni/vlan";
        }

    if (type == 0x2)
        {
        str = "vlan";
        fue = (tbl->word0 >> 24) & 0x7;
        rmf = (tbl->word0 >> 16) & 0x7;
        umf = (tbl->word0 >> 8) & 0x7;
        vmem = tbl->word0  & 0x7;
        }
    else
        {
        if (!multicast)
            {
            unicasttype = (tbl->word1 >> 30) & 0x3;
            secure = (tbl->word2 & 0x1);
            block = (tbl->word2 & 0x2) >> 1;
            port = (tbl->word2 >> 0x2) & 0x3;
            }
        else
            {
            fwstate = (tbl->word1 >> 30) & 0x3;
            super = (tbl->word2 >> 1) & 0x1;
            portmask = (tbl->word2 >> 2) & 0x7;
            }
        }

    if (type == 0x3 || type == 0x1)
        {

        /* multicast entry */

        if (multicast)
            {
            (void)printf ("%-20s%-20s%-20s%-20s%-20s%-20s\n", "mac",
                          "ent type", "state", "super", "portmask", "vid");

            (void)printf ("---------------------------------------------"
                          "---------------------------------------------"
                          "-----------------------------------\n");

            (void)printf ("%02x:%02x:%02x:%02x:%02x:%02x   %-20s%"
                          "-20d%-20d%-20d%-20d\n\n", macaddr1[1], macaddr1[0],
                          macaddr[3], macaddr[2], macaddr[1],macaddr[0],
                          str, fwstate, super, portmask, vid);
            }

        /* unicast entry */

        else
            {
            (void)printf ("%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n",
                          "mac", "ent type", "type", "secure",
                          "block", "port", "vid");

            (void)printf ("---------------------------------------------"
                          "---------------------------------------------"
                          "-----------------------------------\n");

            (void)printf ("%02x:%02x:%02x:%02x:%02x:%02x   %-20s%"
                          "-20s%-20d%-20d%-20d%-20d\n\n", macaddr1[1],
                          macaddr1[0], macaddr[3], macaddr[2], macaddr[1],
                          macaddr[0], str, unicast[unicasttype], secure,
                          block, port, vid);
            }

        /* vlan entry */

        }
    else if (type == 0x2)
        {
        (void)printf ("%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n", "mac",
                      "ent type", "fue", "rmf", "umf", "vmem", "vid");
        (void)printf ("---------------------------------------------"
                      "---------------------------------------------"
                      "-----------------------------------\n");

        (void)printf ("%-20s%-20s%-20d%-20d%-20d%-20d%-20d\n\n", "N/A",
                      str, fue, rmf, umf, vmem, vid);
        }
    }

/*******************************************************************************
*
* cpswAleDump - show all the ALE entries
*
* This routine outputs verbose message of all the ALE entries
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswAleDump
    (
    VXB_DEV_ID pInst
    )
    {
    int i;
    CPSW_ALE_TBL tbl;

    if (pInst)
        {
        for (i = 0; i < CPSW_ALE_ENTRY_NR; i++)
            {
            cpswAleRead (pInst, &tbl, i);
            cpswAleEntryShow (&tbl);
            }
        }
    }
#endif /* !_WRS_CONFIG_CERT */

/*******************************************************************************
*
* cpswAleAddMultiCast - add a multicast MAC address into ALE table
*
* This routine adds a multicast MAC address into ALE table. First, this routine
* will try to find a match. If the MAC address is already in the table, it
* overwrites the entry content with new values. If it can not find a match, a empty
* entry will be allocated and the MAC address will be added.
*
* RETURNS: 0 on success, ENOMEM if can not find a emtpy entry
*
* ERRNO: N/A
*/

LOCAL int cpswAleAddMultiCast
    (
    VXB_DEV_ID   pDev,
    unsigned char * mac,
    int             vid,
    int             fwstate,
    int             super,
    int             portmask
    )
    {
    unsigned int word0, word1;
    CPSW_ALE_TBL tbl;
    int entry;

    word0 = (mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | (mac[5]);
    word1 = (mac[0] << 8) | (mac[1]);

    entry = cpswAleMatch (pDev, mac);
    if (entry < 0)
        {
        entry = cpswAleFind (pDev);
        if (entry < 0)
            return ENOMEM;
        }

    tbl.word0 = word0;

    /* multicast with vlan */

    if (vid > 0)
        tbl.word1 = word1 | ((vid & 0xfff) << 16) |
                    ((fwstate & 0x3) << 30) | (0x3 << 28);
    else
        tbl.word1 = word1 | ((fwstate & 0x3) << 30) | (0x1 << 28);

    tbl.word2 = ((super & 0x1) << 1) | ((portmask & 0x7) << 2);

    cpswAleWrite (pDev, &tbl, entry);

    return 0;
    }

/*******************************************************************************
*
* cpswAleAddUniCast - add a unicast MAC address into ALE table
*
* This routine adds a unicast MAC address into ALE table. First, this routine
* will try to find a match. If the MAC address is already in the table, it overwrites
* that entry content with new values. If it can not find a match,
* a empty entry will be allocated and the MAC address will be added.
*
* RETURNS: 0 on success or ENOMEM if can not find a emtpy entry
*
* ERRNO: N/A
*/

LOCAL int cpswAleAddUniCast
    (
    VXB_DEV_ID   pDev,
    unsigned char * mac,
    int             vid,
    int             secure,
    int             block,
    int             port
    )
    {
    unsigned int word0, word1;
    CPSW_ALE_TBL tbl;
    int entry;

    word0 = (mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | (mac[5]);
    word1 = (mac[0] << 8) | (mac[1]);

    tbl.word0 = word0;
    tbl.word1 = word1;
    tbl.word2 = 0x0;

    entry = cpswAleMatch (pDev, mac);
    if (entry < 0)
        {
        entry = cpswAleFind (pDev);
        if (entry < 0)
            return ENOMEM;
        }

    tbl.word0 = word0;

    /* unicast with vlan*/

    if (vid > 0)
        tbl.word1 = word1 | (0x3 << 28) |
                    CPSW_ALE_UNICAST_AGEABLE_NOT |
                    ((vid & 0xfff) << 16);
    else
        tbl.word1 = word1 | (0x1 << 28) |
                    CPSW_ALE_UNICAST_AGEABLE_NOT;

    tbl.word2 = ((block & 0x1) << 1) | (secure & 0x1) |
                ((port & 0x3) << 2);

    cpswAleWrite (pDev, &tbl, entry);

    return 0;
    }

/*******************************************************************************
*
* cpswAleMatchVlan - find the ALE entry with the given vid
*
* This routine finds the ALE entry index with the given vid
*
* RETURNS: ALE entry index on success or -1 if entry not found
*
* ERRNO: N/A
*/

LOCAL int cpswAleMatchVlan
    (
    VXB_DEV_ID pDev,
    int vid
    )
    {
    int i;
    CPSW_ALE_TBL t;

    for (i = 0; i < CPSW_ALE_ENTRY_NR; i++)
        {
        cpswAleRead (pDev, &t, i);
        if (((t.word1 >> 28) & 0x3) == 0x2)
            {
            if (((t.word1 >> 16 ) & 0xfff)  == vid)
                return i;
            }
        }
    return -1;
    }

/*******************************************************************************
*
* cpswAleAddVlan - add a vlan entry into ALE table
*
* This routine adds a vlan entry into ALE table
*
* RETURNS: 0 on success,  ENOMEM if can not find a emtpy entry or
*          EEXIST if the given vlan is alreay in ALE table
*
* ERRNO: N/A
*/

LOCAL int cpswAleAddVlan
    (
    VXB_DEV_ID pDev,
    int vlanMemberList,
    int unregFloodMask,
    int regFloodMask,
    int forceUntaggedEgress,
    int vid
    )
    {
    CPSW_ALE_TBL t;
    int entry;

    if (vid <= 0 || vid >= 4095)
        return EINVAL;

    entry = cpswAleMatchVlan (pDev, vid);
    if (entry < 0)
        {
        entry = cpswAleFind (pDev);
        if (entry < 0)
            return ENOMEM;
        }

    t.word0 = ((vlanMemberList & 0x7))      |
              ((unregFloodMask & 0x7) << 8) |
              ((regFloodMask & 0x7) << 16)  |
              ((forceUntaggedEgress & 0x7) << 24);
    t.word1 = ((vid & 0xfff) << 16) | (0x2 << 28);
    t.word2 = 0;
    cpswAleWrite (pDev, &t, entry);

    return 0;
    }



/*******************************************************************************
*
* cpswEndErrHandle - handle CPDMA channel errors
*
* This routine is scheduled by the ISR to run in the context of tNetTask
* whenever a CPDMA error interrupt is received. This routine will reset all the
* descriptors to their default value.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndErrHandle
    (
    CPSW_HOST_CTRL *    pHost
    )
    {
    int                 i, cnt;
    VXB_DEV_ID          pDev = pHost->pDev;
    CPSW_DESC *         desc;
    CPSW_MAC_CTRL *     pMacCtrl;
    M_BLK_ID            pMblk;
#ifdef CPSW_DEBUG
    UINT32              val, txErr, rxErr, rxChan, txChan;

    LOCAL char * txChanErrMsg [] =
        {
        "NO error",
        "SOP errpr",
        "Ownership bit not set in SOP buffer",
        "Zero next buffer descriptor pointer without EOP",
        "Zero buffer pointer",
        "Zero buffer length",
        "packet length error"
        };

    LOCAL char * rxChanErrMsg [] =
        {
        "NO error",
        "reserved",
        "Ownership bit not set in input buffer",
        "reserved",
        "Zero buffer pointer",
        "Zero buffer on non-SOP descriptor",
        "SOP buffer length not greater than offset"
        };

    /* get error code */

    val    = CPDMA_REG_READ (pDev, CPDMA_DMASTATUS);
    rxErr  = CPDMA_RX_HOST_ERR_CODE (val);
    txErr  = CPDMA_TX_HOST_ERR_CODE (val);
    rxChan = CPDMA_RX_HOST_ERR_CHAN (val);
    txChan = CPDMA_TX_HOST_ERR_CHAN (val);

    /* log the error message */

    DBG_MSG (DBG_INFO,"rx chan = %d\nrx err code = %d (%s)\n" \
                      "tx chan = %d\ntx err code = %d (%s)\n" \
                      "reset hardware\n",
                      rxChan, rxErr, (_Vx_usr_arg_t)rxChanErrMsg [rxErr],
                      txChan, txErr, (_Vx_usr_arg_t)txChanErrMsg [txErr]);
#endif /* CPSW_DEBUG */

    /* dma errors require hardware reset to recover */

    if (cpswHostPortInit (pDev) != OK)
        return;

    if (pHost->ports[0])
        {
        if (cpswGmacPortInit (pHost->ports[0]) != OK)
            return;
        }

    if (pHost->ports[1])
        {
        if (cpswGmacPortInit (pHost->ports[1]) != OK)
            return;
        }

    if (cpdmaInit (pDev) != OK)
        return;

    for (i = 0; i < pHost->nrMacPorts; i++)
        {
        pMacCtrl = pHost->ports[i];

        for (cnt = 0; cnt < CPSW_DESC_CNT; cnt++)
            {
            pMacCtrl->cpswTxDescMem[cnt].link   = 0;
            pMacCtrl->cpswTxDescMem[cnt].buf    = 0;
            pMacCtrl->cpswTxDescMem[cnt].len    = 0;
            pMacCtrl->cpswTxDescMem[cnt].flags  = 0;

            if (pMacCtrl->cpswTxblk[pMacCtrl->cpswTxQHead])
                {
                endPoolTupleFree (pMacCtrl->cpswTxblk[pMacCtrl->cpswTxQHead]);
                pMacCtrl->cpswTxblk[pMacCtrl->cpswTxQHead] = NULL;
                }
            }
        pMacCtrl->cpswTxQHead = CPSW_TXQ_INVALID;

        for (cnt = 0; cnt < CPSW_DESC_CNT; cnt++)
            {
            pMblk = pMacCtrl->cpswRxblk[cnt];
            desc = pMacCtrl->cpswRxDescMem + cnt;
            desc->buf = (UINT32)cpswVirToPhy (mtod(pMblk, UINT32));
            desc->len    = CPSW_MTU;
            desc->offset = 0;
            desc->flags  = CPSW_OWNERSHIP;
            }

        pMacCtrl->cpswRxIdx = 0;
        pMacCtrl->cpswTxFree = CPSW_DESC_CNT;
        pMacCtrl->cpswTxstall = FALSE;

        /* link all the RX descriptors, here we need physical address */

        desc = (CPSW_DESC *)(pMacCtrl->cpswRxDescMem[0].phys);
        for (cnt  = 0; cnt < CPSW_DESC_CNT - 1; cnt++)
            pMacCtrl->cpswRxDescMem[cnt].link = (UINT32)(desc + (cnt + 1));
        pMacCtrl->cpswRxDescTail = &pMacCtrl->cpswRxDescMem [CPSW_DESC_CNT - 1];

        /* reset the rx queue head */

        CPDMA_REG_WRITE (pDev, CPDMA_RX_FREEBUF(pMacCtrl->cpswRxDmaChan),
                         CPSW_DESC_CNT);
        CPDMA_REG_WRITE (pDev, CPDMA_RX_HDP(pMacCtrl->cpswRxDmaChan),
                         pMacCtrl->cpswRxDescMem[0].phys);

        /* enable interrupts */

        CPDMA_RX_INT_EN (pDev, pMacCtrl->cpswRxDmaChan);
        CPDMA_TX_INT_EN (pDev, pMacCtrl->cpswTxDmaChan);
        CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_TX_EN, 0xff);
        CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_TX_EN, 0xff);
        CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_MISC_EN, 0x1f);
        }

    CPDMA_HOSTERR_INT_EN (pDev);
    CPDMA_TX_EN (pDev);
    CPDMA_RX_EN (pDev);

    return;
    }

/*******************************************************************************
*
* cpswEndInt - handle device interrupts
*
* This function is invoked whenever the device's interrupt line is asserted
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndInt
    (
    CPSW_HOST_CTRL * pHost
    )
    {
    VXB_DEV_ID pDev = pHost->pDev;

    /* disable interrupts */

    CPDMA_HOSTERR_INT_DIS (pDev);
    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_CLR, 0xff);
    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_CLR, 0xff);
    CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_TX_EN, 0x0);
    CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_RX_EN, 0x0);
    CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_MISC_EN, 0x0);

    /* ack the interrupts */

    CPDMA_REG_WRITE (pDev, CPDMA_DMA_EOI, 0x3);
    CPDMA_REG_WRITE (pDev, CPDMA_DMA_EOI, 0x2);
    CPDMA_REG_WRITE (pDev, CPDMA_DMA_EOI, 0x1);
    if (pHost->dm5Base != NULL && pHost->dm6Base != NULL)
        {
        vxbWrite32 (CPSW_HANDLE(pDev),
                    (UINT32*)((char*)(pHost->dm5Base) + GPTIMER_IRQ_STATUS),
                    TCAR_IF_FLAG);

        vxbWrite32 (CPSW_HANDLE(pDev),
                    (UINT32*)((char*)(pHost->dm6Base) + GPTIMER_IRQ_STATUS),
                    TCAR_IF_FLAG);
        }

    if (vxAtomicCas (&pHost->cpswIntPending, FALSE, TRUE) == TRUE)
        {
        /* post the job */

        (void)jobQueuePost (pHost->cpswJobQueue, &pHost->cpswQJob);
        }
    }

/*******************************************************************************
*
* cpswIntHandle - handle network events
*
* This routine handles network events (send, receive, err)
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswIntHandle
    (
    void * pArg
    )
    {
    int i;
    QJOB * job = pArg;
    CPSW_HOST_CTRL * pHost = member_to_object (job, CPSW_HOST_CTRL, cpswQJob);
    VXB_DEV_ID pDev = pHost->pDev;
    UINT32 txSts, rxSts, errSts, rxErr, txErr;

    /* read interrupt status */

    errSts = CPDMA_REG_READ (pDev, CPDMA_DMASTATUS);
    rxSts  = CPDMA_REG_READ (pDev, CPDMA_RX_INTSTAT_RAW) & 0xff;
    txSts  = CPDMA_REG_READ (pDev, CPDMA_TX_INTSTAT_RAW) & 0xff;

    /* handle error first */

    rxErr = CPDMA_RX_HOST_ERR_CODE(errSts);
    txErr = CPDMA_TX_HOST_ERR_CODE(errSts);
    if (rxErr || txErr)
        {
        cpswEndErrHandle (pHost);
        }

    /* looking for rx events */

    for (i = 0; i < NR_DMA_CHANS; i++)
        {
        if (rxSts & (0x1 << i))
            {
            if (i == pHost->ports[0]->cpswRxDmaChan)
                cpswEndRxHandle (pHost->ports[0]);
            else if (pHost->ports[1] && (pHost->ports[1]->cpswRxDmaChan == i))
                cpswEndRxHandle (pHost->ports[1]);
            }
        }

    /* looking for tx events */

    for (i = 0; i < NR_DMA_CHANS; i++)
        {
        if (txSts & (0x1 << i))
            {
            if (i == pHost->ports[0]->cpswTxDmaChan)
                cpswEndTxHandle (pHost->ports[0]);
            else if (pHost->ports[1] && (pHost->ports[1]->cpswTxDmaChan == i))
                cpswEndTxHandle (pHost->ports[1]);
            }
        }

    errSts = CPDMA_REG_READ (pDev, CPDMA_DMASTATUS);
    rxSts  = CPDMA_REG_READ (pDev, CPDMA_RX_INTSTAT_RAW) & 0xff;
    txSts  = CPDMA_REG_READ (pDev, CPDMA_TX_INTSTAT_RAW) & 0xff;

    if ((errSts & ~0x80000000) || rxSts || txSts)
        {
        (void)jobQueuePost (pHost->cpswJobQueue, &pHost->cpswQJob);
        return;
        }

    (void)vxAtomicSet (&pHost->cpswIntPending, FALSE);

    CPDMA_HOSTERR_INT_EN (pDev);
    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_SET, 0xff);
    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_SET, 0xff);
    CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_TX_EN, 0xff);
    CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_RX_EN, 0xff);
    CSR_WRITE_4 (pDev, pHost->wrOffset + CPSW_WR_C0_MISC_EN, 0x1f);

    return;
    }
