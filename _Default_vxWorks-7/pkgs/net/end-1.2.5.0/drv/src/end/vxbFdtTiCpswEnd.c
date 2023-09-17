/* vxbFdtTiCpswEnd.c - TI 3 port gigabit switch VxBus END driver */

/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
09nov16,mca  silenced a 'declared but never referenced' warning.
04apr16,dlk  EIOCGFLAGS argument is an INT32 pointer (V7PRO-2981).
02apr16,dlk  Use END_LOGX() for logging (V7NET-535).
28mar16,y_f  corrected return value for adding/deleting multicast addresses
             (V7PRO-2922)
05jan16,m_w  written. (V7PRO-2729)
*/

/*
DESCRIPTION
This driver is re-written based on vxbCpswEnd.c/vxbCpswSlEnd.c and
it's component name is DRV_END_FDT_TI_CPSW. The old component
DRV_VXBEND_TI_CPSW is kept for compatibility with the bsp which
include the DRV_VXBEND_TI_CPSW. The old driver use only one interrupt
handle cpswEndInt() to process all the tx/rx/misc interrupt of all
ports, that is not a very efficient way, for example the tx interrupt
will be disabled when process the rx interrupt.
This driver(DRV_END_FDT_TI_CPSW) use different interrupt handle for
different interrupt to let the driver running in a high efficient
status. DRV_VXBEND_TI_CPSW is deprecated and should be replaced with
DRV_END_FDT_TI_CPSW.

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

\i <compatible>
This parameter specifies the name of the CPSW controller driver. It must be
"ti,cpsw-switch-controller" for the switch controller device, and
"ti,cpsw-port" for the slave port.

\ie

Following parameter is switch controller device only:

\is

\i <cpsw-port0-offset>
cpsw port control register offset.

\i <cpsw-stats-offset>
statistics register offset.

\i <cpsw-cpdma-offset>
cpdma register offset.

\i <cpsw-cpdmaHdp-offset>
cpdma HDP register offset.

\i <cpsw-ale-offset>
ALE register offset.

\i <cpsw-mdio-offset>
MDIO register offset

\i <cpsw-wr-offset>
CPSW_WR offset.

\ie

Following parameter is port device only:

\is

\i <interrupt-parent>
This parameter specifies the offset of interrupt controller.

\i <interrupts>
This parameter specifies the interrupt number of this module.

\i <cpsw-port-index>
port 0 is host port, so this parameter must be 1 or 2.

\i <cpsw-gmac-offset>
Ethernet gmac offset.

\i <cpsw-port-offset>
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
#include "vxbFdtTiCpswEnd.h"

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
	struct vxbFdtDev  vxbFdtDev;
    VXB_RESOURCE_LIST vxbResList;
} CPSW_PORT_DEV_INFO;

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

/* common resource which is shared by all port */

LOCAL CPSW_SW_CTRL * pSwCtrl = NULL;

/* import functions */

IMPORT FUNCPTR      _func_m2PollStatsIfPoll;
IMPORT STATUS       sysNetMacNVRamAddrGet (char *, INT32, UINT8 *, INT32);
IMPORT void         vxbUsDelay            (INT32);

/* switch controller routines */

LOCAL STATUS        cpswSwCtrlProbe     (VXB_DEV_ID);
LOCAL STATUS        cpswSwCtrlAttach    (VXB_DEV_ID);
LOCAL VXB_FDT_DEV * cpswPortDevInfo     (VXB_DEV_ID, VXB_DEV_ID);
LOCAL VXB_RESOURCE* cpswPortResAlloc    (VXB_DEV_ID, VXB_DEV_ID, UINT32);
LOCAL STATUS        cpswPortsCreate     (VXB_DEV_ID);

LOCAL STATUS        cpswHostPortInit    (VXB_DEV_ID);
LOCAL STATUS        cpswGmacPortInit    (CPSW_DRV_CTRL *);
LOCAL STATUS        cpswSoftReset       (VXB_DEV_ID, UINT32, UINT32);

/* DMA routines */

LOCAL STATUS        cpswDmaInit         (VXB_DEV_ID);
LOCAL STATUS        cpswDmaChanRequest  (CPSW_SW_CTRL *, BOOL, INT32);
LOCAL STATUS        cpswDmaChanRelease  (CPSW_SW_CTRL *, BOOL, INT32);

/* ALE routines */

LOCAL INT32         cpswAleAddUniCast   (VXB_DEV_ID, UCHAR *,
                                         INT32, INT32, INT32, INT32);
LOCAL void          cpswAleRead         (VXB_DEV_ID, CPSW_ALE_TBL * , UINT32);
LOCAL void          cpswAleWrite        (VXB_DEV_ID, CPSW_ALE_TBL * , UINT32);
LOCAL INT32         cpswAleFind         (VXB_DEV_ID);
LOCAL INT32         cpswAleMatch        (VXB_DEV_ID, UCHAR *);
LOCAL INT32         cpswAleMatchVlan    (VXB_DEV_ID, INT32);
LOCAL INT32         cpswAleAddVlan      (VXB_DEV_ID, INT32, INT32, INT32, INT32, INT32);
LOCAL INT32         cpswAleAddMultiCast (VXB_DEV_ID,
                                         UCHAR*, INT32, INT32, INT32, INT32);
#ifndef _WRS_CONFIG_CERT
LOCAL void          cpswAleEntryShow    (CPSW_ALE_TBL *);
LOCAL void          cpswAleDump         (VXB_DEV_ID);
#endif

/* END functions */

LOCAL END_OBJ *     cpswEndLoad         (char *, void *);
LOCAL STATUS        cpswEndUnload       (END_OBJ *);
LOCAL INT32         cpswEndIoctl        (END_OBJ *, INT32, caddr_t);
LOCAL STATUS        cpswEndMCastAddrAdd (END_OBJ *, char *);
LOCAL STATUS        cpswEndMCastAddrDel (END_OBJ *, char *);
LOCAL STATUS        cpswEndMCastAddrGet (END_OBJ *, MULTI_TABLE *);
LOCAL STATUS        cpswEndStart        (END_OBJ *);
LOCAL STATUS        cpswEndStop         (END_OBJ *);
LOCAL INT32         cpswEndEncap        (CPSW_DRV_CTRL *, M_BLK_ID);
LOCAL INT32         cpswEndSend         (END_OBJ *, M_BLK_ID);
LOCAL STATUS        cpswEndPollSend     (END_OBJ *, M_BLK_ID);
LOCAL INT32         cpswEndPollReceive  (END_OBJ *, M_BLK_ID);
LOCAL void          cpswEndRxHandle     (void *);
LOCAL void          cpswEndTxHandle     (void *);
LOCAL void          cpswEndMiscHandle   (void *);
LOCAL void          cpswEndTxInt        (CPSW_SW_CTRL *);
LOCAL void          cpswEndRxInt        (CPSW_SW_CTRL *);
LOCAL void          cpswEndMiscInt      (CPSW_SW_CTRL *);
LOCAL STATUS        cpswPhyRead         (VXB_DEV_ID, UINT8, UINT8, UINT16 *);
LOCAL STATUS        cpswPhyWrite        (VXB_DEV_ID, UINT8, UINT8, UINT16);
LOCAL STATUS        cpswLinkUpdate      (VXB_DEV_ID);
LOCAL void          cpswMuxConnect      (VXB_DEV_ID, void *);
LOCAL STATUS        cpswPortProbe       (VXB_DEV_ID);
LOCAL STATUS        cpswPortAttach      (VXB_DEV_ID);

/* TOOL routines */

LOCAL PHYS_ADDR     cpswVirToPhy        (VIRT_ADDR);

LOCAL NET_FUNCS cpswNetFuncs =
    {
    cpswEndStart,             /* start func. */
    cpswEndStop,              /* stop func. */
    cpswEndUnload,            /* unload func. */
    cpswEndIoctl,             /* ioctl func. */
    cpswEndSend,              /* send func. */
    cpswEndMCastAddrAdd,      /* multicast add func. */
    cpswEndMCastAddrDel,      /* multicast delete func. */
    cpswEndMCastAddrGet,      /* multicast get fun. */
    cpswEndPollSend,          /* cpswPolling send func. */
    cpswEndPollReceive,       /* cpswPolling receive func. */
    endEtherAddressForm,      /* put address info into a NET_BUFFER */
    endEtherPacketDataGet,    /* get pointer to data in NET_BUFFER */
    endEtherPacketAddrGet     /* Get packet addresses */
    };

/* driver utility functions */

LOCAL VXB_DRV_METHOD cpswSwCtrlMethods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),     (FUNCPTR)cpswSwCtrlProbe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach),    (FUNCPTR)cpswSwCtrlAttach},
    { VXB_DEVMETHOD_CALL(vxbFdtDevGet),    (FUNCPTR)cpswPortDevInfo },
    { VXB_DEVMETHOD_CALL(vxbResourceAlloc),(FUNCPTR)cpswPortResAlloc},
    { 0, 0 }
};

/* driver utility functions */

LOCAL VXB_DRV_METHOD cpswPortMethods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),    (FUNCPTR)cpswPortProbe    },
    { VXB_DEVMETHOD_CALL(vxbDevAttach),   (FUNCPTR)cpswPortAttach   },
    { VXB_DEVMETHOD_CALL(miiRead),        (FUNCPTR)cpswPhyRead      },
    { VXB_DEVMETHOD_CALL(miiWrite),       (FUNCPTR)cpswPhyWrite     },
    { VXB_DEVMETHOD_CALL(miiMediaUpdate), (FUNCPTR)cpswLinkUpdate   },
    { 0, 0 }
};

VXB_DRV  cpswSwCtrlDrv =
    {
    { NULL } ,
    "cpsw switch controller",   /* drvName */
    "cpsw switch controller",   /* Description */
    VXB_BUSID_FDT,              /* Class */
    0,                          /* Flags */
    0,                          /* Reference count */
    cpswSwCtrlMethods           /* Method table */
    };

VXB_DRV  cpswPortDrv =
    {
    { NULL } ,
    "cpsw port",        /* drvName */
    "cpsw port",        /* Description */
    VXB_BUSID_FDT,      /* Class */
    0,                  /* Flags */
    0,                  /* Reference count */
    cpswPortMethods     /* Method table */
    };

VXB_DRV_DEF(cpswSwCtrlDrv)
VXB_DRV_DEF(cpswPortDrv)

LOCAL VXB_FDT_DEV_MATCH_ENTRY cpswSwCtrlMatch[] =
    {
        {
        "ti,cpsw-switch-controller",  /* compatible */
        NULL,
        },
        {} /* Empty terminated list */
    };

LOCAL VXB_FDT_DEV_MATCH_ENTRY cpswPortMatch[] =
    {
        {
        "ti,cpsw-port",  /* compatible */
        NULL,
        },
        {} /* Empty terminated list */
    };

/*********************************************************************
*
* cpswSwCtrlProbe - check whether device and driver go together
*
* This routine probe a device with a device driver.
*
* RETURNS: OK if the device and driver match up, ERROR otherwise
*
* ERROR: N/A
*/

LOCAL STATUS cpswSwCtrlProbe
    (
    VXB_DEV_ID pDev
    )
    {
    return vxbFdtDevMatch (pDev, cpswSwCtrlMatch, NULL);
    }

/*********************************************************************
*
* cpswSwCtrlAttach - Initilize the cpsw switch controller device.
*
* This routine initilize the cpsw switch controller device.
*
* RETURNS: OK or ERROR if failed.
*
* ERROR: N/A
*/

LOCAL STATUS cpswSwCtrlAttach
    (
    VXB_DEV_ID pDev
    )
    {
    VXB_FDT_DEV      * pFdtDev = (VXB_FDT_DEV *)vxbDevIvarsGet(pDev);
    int                proplen = 0;
    UINT32           * prop;
    VXB_RESOURCE_ADR * pResAdr = NULL;
    VXB_RESOURCE     * pRes;

    pSwCtrl = vxbMemAlloc (sizeof (CPSW_SW_CTRL));
    if (pSwCtrl == NULL)
        {
        DBG_MSG (DBG_ERR,"vxbMemAlloc failed\n");
        return ERROR;
        }

    /* create connect between pSwCtrl and it's pDev */

    pSwCtrl->pDev = pDev;
    vxbDevSoftcSet(pDev, pSwCtrl);

    /* init register offsets */

    pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);
    if(pRes == NULL)
        {
        DBG_MSG (DBG_ERR,"pRes is NULL\n");
        goto failed;
        }

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    if (pResAdr == NULL)
        {
        DBG_MSG (DBG_ERR,"pResAdr is NULL\n");
        goto failed;
        }

    pSwCtrl->regBase = (void*)pResAdr->virtual;
    pSwCtrl->handle  = pResAdr->pHandle;

    /*
     * workround for CPSW on 335x, use dmtimer5/dmtimer6 as interrupt reporter
     * to CPU when we get C0_TX_PEND and C0_RX_PEND Interrupts.
     * Advisory 1.0.9 of am335x silicon errata 2.1:
     * C0_TX_PEND and C0_RX_PEND Interrupts Not Connected to ARM Cortex-A8.
     */
    pSwCtrl->dm5Base = NULL;
    pSwCtrl->dm6Base = NULL;
    pSwCtrl->errataNeedFix = FALSE;

    pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 1);
    if (pRes != NULL)
        {
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr != NULL)
            pSwCtrl->dm5Base = (void*)pResAdr->virtual;
        }

    pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 2);
    if (pRes != NULL)
        {
         pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;

        if (pResAdr != NULL)
            pSwCtrl->dm6Base = (void*)pResAdr->virtual;
        }

    if (pSwCtrl->dm5Base != NULL && pSwCtrl->dm6Base != NULL)
        {
        pSwCtrl->errataNeedFix = TRUE;
        }

    DBG_MSG (DBG_INFO,"dm5Base = 0x%08x, dm6Base = 0x%08x\n",
             pSwCtrl->dm5Base, pSwCtrl->dm6Base);
    DBG_MSG (DBG_INFO,"errataNeedFix = %d\n",
             pSwCtrl->errataNeedFix);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset,
                                  "cpsw-port0-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-port0-offset property found\n");
        goto failed;
        }
    pSwCtrl->portOffset  = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"cpsw-port0-offset = 0x%08x\n",
             pSwCtrl->portOffset);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-stats-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-stats-offset property found\n");
        goto failed;
        }
    pSwCtrl->statsOffset = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"cpsw-stats-offset = 0x%08x\n",
             pSwCtrl->statsOffset);

    prop = (UINT32 *)vxFdtPropGet (pFdtDev->offset,
                                   "cpsw-cpdma-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-cpdma-offset property found\n");
        goto failed;
        }
    pSwCtrl->cpdmaOffset = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"cpsw-cpdma-offset = 0x%08x\n",
             pSwCtrl->cpdmaOffset);

    prop = (UINT32 *)vxFdtPropGet (pFdtDev->offset,
                                   "cpsw-hdp-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-hdp-offset property found\n");
        goto failed;
        }
    pSwCtrl->cpdmaHdpOffset = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"cpsw-hdp-offset = 0x%08x\n",
             pSwCtrl->cpdmaHdpOffset);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-ale-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-ale-offset property found\n");
        goto failed;
        }
    pSwCtrl->aleOffset   = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"cpsw-ale-offset = 0x%08x\n",
             pSwCtrl->aleOffset);

    prop = (UINT32 *)vxFdtPropGet (pFdtDev->offset,
                                   "cpsw-mdio-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-mdio-offset property found\n");
        goto failed;
        }
    pSwCtrl->mdioOffset  = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"cpsw-mdio-offset = 0x%08x\n",
             pSwCtrl->mdioOffset);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-wr-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-wr-offset property found\n");
        goto failed;
        }
    pSwCtrl->wrOffset    = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"cpsw-wr-offset = 0x%08x\n",
             pSwCtrl->wrOffset);

    /*
     * set work mode to independent port mode,
     * in fact we only support this mode
     */

    pSwCtrl->workMode = CPSW_MODE_INDEPENDENT_PORT;

    /* set host port(which is connect to cpu) index to 0 */

    pSwCtrl->hostPortIndex = 0;

    pSwCtrl->cpswDevSem = semMCreate (SEM_Q_PRIORITY  |
                                      SEM_DELETE_SAFE |
                                      SEM_INVERSION_SAFE);
    if (pSwCtrl->cpswDevSem == NULL)
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

    if (cpswDmaInit (pDev) != OK)
        {
        DBG_MSG (DBG_ERR,"cpswDmaInit() return error\n");
        goto failed;
        }

    /* here we call slave port attach function. */

    if (cpswPortsCreate (pDev) == ERROR)
        {
        DBG_MSG (DBG_ERR,"cpswPortsCreate() return error\n");
        goto failed;
        }

    return OK;

failed:

    if (pSwCtrl != NULL)
        {
        if (pSwCtrl->cpswDevSem)
            (void) semDelete (pSwCtrl->cpswDevSem);

        vxbMemFree (pSwCtrl);
        pSwCtrl = NULL;
        }

    vxbDevSoftcSet (pDev, NULL);

    return ERROR;
    }

/*******************************************************************************
*
* cpswPortDevInfo - get the FDT child device information
*
* This routine gets the FDT child device information
*
* \NOMANUAL
*
* RETURNS: the device information pointer
*
* ERRNO: N/A
*/

LOCAL VXB_FDT_DEV *  cpswPortDevInfo
    (
    VXB_DEV_ID pDev,
    VXB_DEV_ID pChild
    )
    {
    CPSW_PORT_DEV_INFO * pSlDevInfo;

    if (pChild == NULL)
        return NULL;

    pSlDevInfo = vxbDevIvarsGet(pChild);

    if (pSlDevInfo == NULL)
        return NULL;

    return &pSlDevInfo->vxbFdtDev;
    }


/*******************************************************************************
*
* cpswPortResAlloc - vxbus alloc interface
*
* This routine will be used by child of this VXB_DEV to find the resource value.
*
* RETURNS: point of resource when success, NULL for others.
*
* ERRNO: N/A
*/

LOCAL VXB_RESOURCE * cpswPortResAlloc
    (
    VXB_DEV_ID pDev,
    VXB_DEV_ID pChild,
    UINT32     id
    )
    {
    CPSW_PORT_DEV_INFO * pDevInfo;
    VXB_RESOURCE       * pVxbRes;

    pDevInfo = (CPSW_PORT_DEV_INFO *)vxbDevIvarsGet(pChild);

    if (pDevInfo == NULL)
        return NULL;

    pVxbRes = vxbResourceFind(&pDevInfo->vxbResList, id);

    if (pVxbRes == NULL)
        return NULL;

    if (((VXB_RES_TYPE(pVxbRes->id) == VXB_RES_MEMORY) ||
         (VXB_RES_TYPE(pVxbRes->id) == VXB_RES_IO)) &&
        (vxbRegMap (pVxbRes) == OK))
        {
        return pVxbRes;
        }
    else if ((VXB_RES_TYPE(pVxbRes->id) == VXB_RES_IRQ) &&
        (vxbIntMap (pVxbRes) == OK))
        {
        return pVxbRes;
        }
    else
        {
        return NULL;
        }
    }

/*******************************************************************************
*
* cpswPortsCreate - create the port device
*
* This function implements the VxBus gen2 bus attach routine for CPSW port
* device instance.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS cpswPortsCreate
    (
    VXB_DEV_ID  pDev
    )
    {
    VXB_FDT_DEV        * pFdtDev;
    CPSW_PORT_DEV_INFO * pPortDevInfo;
    VXB_FDT_DEV        * pNewFdtDev;
    VXB_DEV_ID           pCur = NULL;
    INT32                offset;

    pFdtDev = vxbFdtDevGet(pDev);

    if (pFdtDev == NULL)
        {
        DBG_MSG (DBG_ERR, "pFdtDev is NULL\n");
        return ERROR;
        }
    offset = pFdtDev->offset;

    for (offset = VX_FDT_CHILD(offset); offset > 0;
         offset = VX_FDT_PEER(offset))
        {
        pCur = NULL;

        if (vxFdtIsEnabled(offset) == FALSE)
            continue;

        if (vxbDevCreate (&pCur) != OK)
            continue;

        pPortDevInfo = (CPSW_PORT_DEV_INFO *)
                        vxbMemAlloc(sizeof(*pPortDevInfo));
        if (pPortDevInfo == NULL)
            {
            (void)vxbDevDestroy(pCur);
            continue;
            }

        pNewFdtDev = &pPortDevInfo->vxbFdtDev;

        pNewFdtDev->offset = offset;

        /* get the device basic infomation  */

        vxbFdtDevSetup(offset, pNewFdtDev);
        vxbDevNameSet(pCur, pNewFdtDev->name, FALSE);

        /* assign the bus internal variable and type  */

        vxbDevIvarsSet(pCur, (void *)pPortDevInfo);
        vxbDevClassSet(pCur, VXB_BUSID_FDT);

        /* get the device register and interrupt infomation  */

        if (vxbResourceInit(&pPortDevInfo->vxbResList) != OK)
            {
            (void)vxbDevDestroy(pCur);
            vxbMemFree (pPortDevInfo);
            continue;
            }

        /* fetched out reg space info from DTS for this devide */

        if (vxbFdtRegGet(&pPortDevInfo->vxbResList, pNewFdtDev) != OK)
            {
            vxbMemFree(pPortDevInfo);
            (void)vxbDevDestroy(pCur);
            continue;
            }

        /* fetched out interrupt info from DTS for this devide */

        if (vxbFdtIntGet(&pPortDevInfo->vxbResList, pNewFdtDev) != OK)
            {
            vxbMemFree(pPortDevInfo);
            (void)vxbDevDestroy(pCur);
            continue;
            }

        (void)vxbDevAdd(pDev, pCur);
        }

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
    CPSW_SW_CTRL * pSwCtrl = vxbDevSoftcGet(pDev);
    UINT32         base, val;

    if (cpswSoftReset (pDev, CPSW_SOFT_RESET, CPSW_TIMEOUT_VAL) != OK)
        {
        DBG_MSG (DBG_ERR,"host port soft reset timeout\n");
        return ERROR;
        }

    base = pSwCtrl->aleOffset;

    val = CPSW_ALE_EN_TABLE | CPSW_ALE_CLR_TABLE;

    /* let ALE enter vlan mode */

    if (pSwCtrl->workMode == CPSW_MODE_INDEPENDENT_PORT)
        {
        val |= CPSW_ALE_VLAN_AWARE;
        }

    CSR_WRITE_4 (pDev, base + CPSW_ALE_CONTROL, val);
    CSR_WRITE_4 (pDev, base + CPSW_ALE_PORTCTL(pSwCtrl->hostPortIndex),
                 CPSW_ALE_PORT_FW | CPSW_ALE_CTL_NO_LEARN);

    /* enable statistics for all ports */

    CSR_WRITE_4 (pDev, CPSW_STAT_PORT_EN, 7);

    base = pSwCtrl->portOffset;
    CSR_WRITE_4 (pDev, base + CPSW_TX_PRI_MAP, CPSW_TX_PRI_MAP_DFTL);
    CSR_WRITE_4 (pDev, base + CPSW_CPDMA_TX_PRI_MAP, CPDMA_TX_PRI_MAP_DFTL);

    if (pSwCtrl->workMode == CPSW_MODE_INDEPENDENT_PORT)
        {
        /* set tx fifo into dual emac mode */

        val = CSR_READ_4 (pDev, base + CPSW_TX_IN_CTL);
        val = val & ~(CPSW_TX_IN_SEL_MSK << CPSW_TX_IN_SEL_SHIFT);
        CSR_WRITE_4 (pDev, base + CPSW_TX_IN_CTL,
                     val | (CPSW_TX_FIFO_DUAL_EMAC << CPSW_TX_IN_SEL_SHIFT));
        }

    base = pSwCtrl->mdioOffset;
    CSR_WRITE_4 (pDev, base + CPSW_MDIO_CONTROL,
                 CPSW_MDIO_EN | CPSW_MDIO_CLK_DIV);

    return OK;
    }

/*******************************************************************************
*
* cpswDmaChanRequest - request a DMA channel
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

LOCAL STATUS cpswDmaChanRequest
    (
    CPSW_SW_CTRL  * pSwCtrl,
    BOOL            rx,
    INT32             chanNum
    )
    {
    UINT8 * pChanArray;

    (void)semTake(pSwCtrl->cpswDevSem, WAIT_FOREVER);

    if(rx == TRUE)
        pChanArray = pSwCtrl->cpswRxDmaChans;
    else
        pChanArray = pSwCtrl->cpswTxDmaChans;

    if (0 <= chanNum && chanNum < NR_DMA_CHANS)
        {
        if (!pChanArray[chanNum])
            {
            pChanArray[chanNum] = 1;

            (void)semGive(pSwCtrl->cpswDevSem);
            return OK;
            }
        }

    (void)semGive(pSwCtrl->cpswDevSem);
    return ERROR;
    }

/*******************************************************************************
*
* cpswDmaChanRelease - release a DMA channel
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

LOCAL STATUS cpswDmaChanRelease
    (
    CPSW_SW_CTRL  * pSwCtrl,
    BOOL            rx,
    INT32             chanNum
    )
    {
    UINT8 * pChanArray;

    (void)semTake(pSwCtrl->cpswDevSem, WAIT_FOREVER);

    if(rx == TRUE)
        pChanArray = pSwCtrl->cpswRxDmaChans;
    else
        pChanArray = pSwCtrl->cpswTxDmaChans;

    if (0 <= chanNum && chanNum < NR_DMA_CHANS)
        {
        if (!pChanArray[chanNum])
            {
            (void)semGive(pSwCtrl->cpswDevSem);
            return OK;
            }
        else
            {
            pChanArray[chanNum] = 0;
            (void)semGive(pSwCtrl->cpswDevSem);
            return OK;
            }
        }

    (void)semGive(pSwCtrl->cpswDevSem);
    return ERROR;
    }

/*******************************************************************************
*
* cpswDmaInit - initialize CPDMA module
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

LOCAL STATUS cpswDmaInit
    (
    VXB_DEV_ID pDev
    )
    {
    INT32            i;
    CPSW_SW_CTRL * pSwCtrl = vxbDevSoftcGet(pDev);

    /* reset the cpdma module */

    if (cpswSoftReset (pDev, pSwCtrl->cpdmaOffset + CPDMA_SOFT_RESET,
                       CPSW_TIMEOUT_VAL) != OK)
        {
        DBG_MSG (DBG_ERR,"dma port soft reset timeout\n");
        return ERROR;
        }

    /* initialize dma queue header pointers */

    for (i = 0; i < NR_DMA_CHANS; i++)
        {
        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset + CPDMA_TX_HDP(i), 0);
        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset + CPDMA_RX_HDP(i), 0);
        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset + CPDMA_TX_CP(i),  0);
        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset + CPDMA_RX_CP(i),  0);

        CPDMA_REG_WRITE (pDev, CPDMA_RX_FREEBUF(i), 0);
        }

    CPDMA_REG_WRITE (pDev, CPDMA_DMACONTROL, CPDMA_CONTROL_DEFAULT);

    /* diable interrupts */

    CPDMA_REG_WRITE (pDev, CPDMA_RX_BUFOFFSET  , 0x0);
    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_CLR, 0xffff);
    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_CLR, 0xffff);
    CPDMA_REG_WRITE (pDev, CPDMA_INTMASK_CLR   , 0x3);

#ifdef CPSW_DBG
    val = CPDMA_REG_READ (pDev, CPDMA_RX_INTSTAT_MASKED);
    if (val & 0xffff)
        {
        DBG_MSG("rx mask(0x%08x) is not zero\n",val);
        }
    val = CPDMA_REG_READ (pDev, CPDMA_TX_INTSTAT_MASKED);
    if (val & 0xffff)
        {
        DBG_MSG("tx mask(0x%08x) is not zero\n",val);
        }
#endif /* CPSW_DBG */

    /* enable global tx rx DMA */

    CPDMA_TX_EN (pDev);
    CPDMA_RX_EN (pDev);

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
    VXB_DEV_ID     pDev,
    CPSW_ALE_TBL * pTbl,
    UINT32         entry
    )
    {
    CPSW_SW_CTRL * pSwCtrl = vxbDevSoftcGet(pDev);

    CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_TBLCTL,
                 entry & CPSW_ALE_ENTRY_IDX_MASK);

    pTbl->word0 = CSR_READ_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_WORD0);
    pTbl->word1 = CSR_READ_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_WORD1);
    pTbl->word2 = CSR_READ_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_WORD2) & 0xf;
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
    VXB_DEV_ID     pDev,
    CPSW_ALE_TBL * pTbl,
    UINT32         entry
    )
    {
    CPSW_SW_CTRL * pSwCtrl = vxbDevSoftcGet(pDev);

    CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_WORD0, pTbl->word0);
    CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_WORD1, pTbl->word1);
    CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_WORD2, pTbl->word2 & 0xf);

    CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_TBLCTL,
                 (entry & CPSW_ALE_ENTRY_IDX_MASK) | CPSW_ALE_WRITE);
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

LOCAL INT32 cpswAleFind
    (
    VXB_DEV_ID pDev
    )
    {
    int          i;
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

LOCAL INT32 cpswAleMatch
    (
    VXB_DEV_ID      pDev,
    UINT8 * mac
    )
    {
    int          i;
    CPSW_ALE_TBL t;
    UINT32       word0, word1;

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
    UINT8 * mac;
    UINT8 * macaddr1;
    INT32   block = 0, secure = 0, port = 0, portmask = 0;
    INT32   fwstate = 0, super = 0;
    INT32   unicasttype = 0;
    INT32   vmem = 0, umf = 0, rmf = 0, fue = 0;

    char  * str = NULL;

    static char * unicast [] =
        {
        "u/na",
        "u/a/nt",
        "oui",
        "u/a/t"
        };

    mac = (UINT8 *)&tbl->word0;
    macaddr1 = (UINT8 *)&tbl->word1;
    INT32 vid = (tbl->word1 >> 16) & 0xfff;
    INT32 type = (tbl->word1 >> 28) & 0x3;
    INT32 multicast = tbl->word1 & 0x100;

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
                          mac[3], mac[2], mac[1],mac[0],
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
                          macaddr1[0], mac[3], mac[2], mac[1],
                          mac[0], str, unicast[unicasttype], secure,
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
    INT32 i;
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

LOCAL INT32 cpswAleAddMultiCast
    (
    VXB_DEV_ID   pDev,
    UINT8      * pMac,
    INT32        vid,
    INT32        fwstate,
    INT32        super,
    INT32        portmask
    )
    {
    UINT32       word0, word1;
    CPSW_ALE_TBL tbl;
    INT32        entry;

    word0 = (pMac[2] << 24) | (pMac[3] << 16) | (pMac[4] << 8) | (pMac[5]);
    word1 = (pMac[0] << 8) | (pMac[1]);

    entry = cpswAleMatch (pDev, pMac);
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

LOCAL INT32 cpswAleAddUniCast
    (
    VXB_DEV_ID   pDev,
    UINT8      * pMac,
    INT32        vid,
    INT32        secure,
    INT32        block,
    INT32        port
    )
    {
    UINT32       word0, word1;
    CPSW_ALE_TBL tbl;
    INT32        entry;

    word0 = (pMac[2] << 24) | (pMac[3] << 16) | (pMac[4] << 8) | (pMac[5]);
    word1 = (pMac[0] << 8) | (pMac[1]);

    tbl.word0 = word0;
    tbl.word1 = word1;
    tbl.word2 = 0x0;

    entry = cpswAleMatch (pDev, pMac);
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

LOCAL INT32 cpswAleMatchVlan
    (
    VXB_DEV_ID pDev,
    INT32 vid
    )
    {
    INT32        i;
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

LOCAL INT32 cpswAleAddVlan
    (
    VXB_DEV_ID pDev,
    INT32      vlanMemberList,
    INT32      unregFloodMask,
    INT32      regFloodMask,
    INT32      forceUntaggedEgress,
    INT32      vid
    )
    {
    CPSW_ALE_TBL t;
    INT32 entry;

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
* cpswSlSetMiiDev - mii device setting function
*
* This function sets the mii device  for an end device.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL STATUS cpswSlSetMiiDev
   (
   VXB_DEV_ID pDev,
   VXB_DEV_ID miiDev,
   INT32      phyAddr
   )
   {
   CPSW_DRV_CTRL * pDrvCtrl = (CPSW_DRV_CTRL *)vxbDevSoftcGet(pDev);
   MII_DRV_CTRL  * pMiiDrvCtrl;

   pDrvCtrl->cpswMiiDev = miiDev;

   pMiiDrvCtrl = (MII_DRV_CTRL *)vxbDevSoftcGet(miiDev);
   pMiiDrvCtrl->pEndInst = pDev;

   miiBusDevInstConnect(pDev);

   return OK;
   }

/*******************************************************************************
*
* cpswPortProbe - check whether device and driver go together
*
* This routine probe a device with a device driver.
*
* RETURNS: OK if the device and driver match up, ERROR otherwise
*
* ERRNO
*/

LOCAL STATUS cpswPortProbe
    (
    VXB_DEV_ID pDev
    )
    {
    return vxbFdtDevMatch (pDev, cpswPortMatch, NULL);
    }

/*******************************************************************************
*
* cpswGmacPortInit - initialize CPSW GMAC port
*
* This routine initializes the CPSW GMAC port. It performs a soft reset, sets up
* neccessary reigsters, and finally get MAC address from BSP. (NOTE: if soft
* reset fails, the routine won't proceed. An error message will be print out if
* debug mode is enabled)
*
* RETURNS: OK or ERROR if reset timeout
*
* ERRNO: N/A
*/

LOCAL STATUS cpswGmacPortInit
    (
    CPSW_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32         lo, hi;
    VXB_DEV_ID     pDev = pSwCtrl->pDev;
    UINT32         base;

   /*
    * put MAC port into forward state and disable learning on this port,
    * because we are operating in dual emac mode, not switch mode, so
    * we add our MAC address manually.
    */

    base = pSwCtrl->aleOffset;
    CSR_WRITE_4 (pDev, base + CPSW_ALE_PORTCTL(pDrvCtrl->portIndex),
                 CPSW_ALE_PORT_FW | CPSW_ALE_CTL_NO_LEARN);

    base = pDrvCtrl->portOffset;
    CSR_WRITE_4 (pDev, base + CPSW_TX_PRI_MAP, CPSW_TX_PRI_MAP_DFTL);

    /*
     +-------+-------+-------+-------+-------+-------+
     | byte5 | byte4 | byte3 | byte2 | byte1 | byte0 |
     +-------+-------+-------+-------+-------+-------+
     */

    lo = pDrvCtrl->mac[5] << 8   | pDrvCtrl->mac[4];
    hi = pDrvCtrl->mac[3] << 24  | pDrvCtrl->mac[2] << 16 |
         pDrvCtrl->mac[1] << 8   | pDrvCtrl->mac[0];

    CSR_WRITE_4 (pDev, pDrvCtrl->portOffset + CPSW_SL_SA_L0, lo);
    CSR_WRITE_4 (pDev, pDrvCtrl->portOffset + CPSW_SL_SA_HI, hi);

    /* reset the gmac module */

    base = pDrvCtrl->gmacOffset;
    if (cpswSoftReset (pDev, base + CPSW_SL_MAC_SOFT_RESET,
        CPSW_TIMEOUT_VAL) != OK)
        {
        DBG_MSG (DBG_ERR,"gmac port soft reset timeout\n");
        return ERROR;
        }

    CSR_WRITE_4 (pDev, base + CPSW_SL_RX_PRI_MAP, CPDMA_TX_PRI_MAP_DFTL);
    CSR_WRITE_4 (pDev, base + CPSW_SL_RX_MAXLEN , CPSW_MTU);
    CSR_WRITE_4 (pDev, base + CPSW_SL_MAC_CTL   , CPSW_SL_MAC_CTL_DEF);

    if (pSwCtrl->workMode == CPSW_MODE_INDEPENDENT_PORT)
        {
        CSR_WRITE_4 (pDev, pDrvCtrl->portOffset + CPSW_PORT_VLAN,
                     pDrvCtrl->portVlan);

        (void)cpswAleAddVlan(pDev, (0x1 << pSwCtrl->hostPortIndex) |
                                   (0x1 << pDrvCtrl->portIndex),
                             0, 0, 0, pDrvCtrl->portVlan);

        (void)cpswAleAddUniCast (pDev, pDrvCtrl->mac,
                                 pDrvCtrl->portVlan,
                                 0, 0, pSwCtrl->hostPortIndex);
        }

    return OK;
    }

/*******************************************************************************
*
* cpswPortAttach - attach a vxbus device
*
* This is the cpswSl initialization routine.
*
* RETURNS: OK, or ERROR if initialization failed.
*
* ERRNO: N/A
*/


LOCAL STATUS cpswPortAttach
    (
    VXB_DEV_ID pDev
    )
    {
    CPSW_DRV_CTRL    * pDrvCtrl;
    VXB_FDT_DEV      * pFdtDev;
    UINT32           * prop;
    VXB_RESOURCE     * pMacLoRes = NULL;
    VXB_RESOURCE     * pMacHiRes = NULL;
    VXB_RESOURCE_ADR * pResAdrLo = NULL;
    VXB_RESOURCE_ADR * pResAdrHi = NULL;
    VXB_RESOURCE     * pRes;
    INT32              i, cnt, proplen = 0;
    UINT32             macVal;

    if (pDev == NULL)
        {
        DBG_MSG (DBG_ERR,"parameter error\n");
        return ERROR;
        }

    pFdtDev = vxbFdtDevGet(pDev);
    if (pFdtDev == NULL)
        {
        DBG_MSG (DBG_ERR,"no fdtDev info\n");
        return ERROR;
        }

    pDrvCtrl = vxbMemAlloc (sizeof (CPSW_DRV_CTRL));
    if (pDrvCtrl == NULL)
        {
        DBG_MSG (DBG_ERR,"not enough memory\n");
        goto portAttachFail;
        }

    /* create connect between pDrvCtrl and pDev */

    vxbDevSoftcSet(pDev, pDrvCtrl);
    pDrvCtrl->pDev = pDev;

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset, "cpsw-port-index", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"No cpsw-port-index property found\n");
        goto portAttachFail;
        }

    i  = vxFdt32ToCpu(*prop);

    /*
     * verify the portIndex , the host port(connect to CPU) is 0,
     * so the other port should start from 1 and not beyond NR_MAC_PORTS
     */

    if (i < 1 || i > NR_MAC_PORTS)
        {
        DBG_MSG (DBG_ERR,"wrong portIndex\n");
        goto portAttachFail;
        }
    pSwCtrl->port[i-1] = pDrvCtrl;
    pDrvCtrl->pSwCtrl  = pSwCtrl;

    /* get gmac resources */

    pDrvCtrl->portIndex     = i;
    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset,
                                  "cpsw-gmac-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"%s%d did't find cpsw-gmac-offset property\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        goto portAttachFail;
        }
    pDrvCtrl->gmacOffset = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"%s%d gmacOffset = 0x%08x\n",
             CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, pDrvCtrl->gmacOffset);

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset,
                                  "cpsw-port-offset", &proplen);
    if (prop == NULL)
        {
        DBG_MSG (DBG_ERR,"%s%d did't find cpsw-port-offset property\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        goto portAttachFail;
        }
    pDrvCtrl->portOffset = vxFdt32ToCpu(*prop);
    DBG_MSG (DBG_INFO,"%s%d portOffset = 0x%08x\n",
             CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, pDrvCtrl->portOffset);

    /* set the default value of macSaveWay*/

    pDrvCtrl->macSaveWay = 0;

    prop = (UINT32 *)vxFdtPropGet(pFdtDev->offset,
                                  "cpsw-mac-save-way", &proplen);
    if (prop != NULL)
        {
        DBG_MSG (DBG_ERR,"%s%d find cpsw-mac-save-way property\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        pDrvCtrl->macSaveWay = vxFdt32ToCpu(*prop);
        }
    DBG_MSG (DBG_INFO,"%s%d macSaveWay = 0x%08x\n",
             CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, pDrvCtrl->macSaveWay);

    /*
     * Retrieve the station address.
     * The mac address will be fetched out from CTRL_MAC_ID with higher priority
     * than local-mac-address(DTS), each board has an unique address for each
     * mac port which was saved in CTRL_MAC_ID, if this area was not supplied
     * or it does not exist, the mac address will be fetched out from
     * local-mac-address(DTS) area. Please be careful to use same
     * local-mac-address(DTS) value with more than one boards in same LAN,
     * because the same MAC address value with multiple boards in same LAN will
     * lead ARP confict.
     * When the CTRL_MAC_ID area and local-mac-address was supplied
     * simultaneously, CTRL_MAC_ID WIN which means mac address will be fetched
     * out from CTRL_MAC_ID in stead of local-mac-address(DTS).
     */

    pMacLoRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);
    pMacHiRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 1);
    if (pMacLoRes == NULL ||
        pMacHiRes == NULL ||
        VXB_RES_TYPE(pMacLoRes->id) != VXB_RES_MEMORY ||
        VXB_RES_TYPE(pMacHiRes->id) != VXB_RES_MEMORY
        )
        {
        /*
         * fetched out mac address from local-mac-address when CTRL_MAC_ID
         * was not exist.
         */
        prop = (UINT32*)vxFdtPropGet(pFdtDev->offset, "local-mac-address",
                                     &proplen);
        if (prop == NULL)
            {
            DBG_MSG (DBG_ERR,"%s%d did't find local-mac-address property\n",
                     CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
            goto portAttachFail;
            }
        memcpy(pDrvCtrl->mac, (char *)prop, ETHER_ADDR_LEN);
        }
    else /* fetched out mac address from CTRL_MAC_ID*/
        {
        pResAdrLo = (VXB_RESOURCE_ADR *)pMacLoRes->pRes;
        pResAdrHi = (VXB_RESOURCE_ADR *)pMacHiRes->pRes;

        if (pDrvCtrl->macSaveWay == 1)
            {
			macVal = *(UINT32*)pResAdrHi->virtual;
            pDrvCtrl->mac[0] = (UINT8)((macVal & 0x00FF0000)>>16);
            pDrvCtrl->mac[1] = (UINT8)((macVal & 0x0000FF00)>>8);
            pDrvCtrl->mac[2] = (UINT8)((macVal & 0x000000FF));

            macVal = *(UINT32*)pResAdrLo->virtual;
            pDrvCtrl->mac[3] = (UINT8)((macVal & 0x00FF0000)>>16);
            pDrvCtrl->mac[4] = (UINT8)((macVal & 0x0000FF00)>>8);
            pDrvCtrl->mac[5] = (UINT8)((macVal & 0x000000FF));
			}
        else
            {
            /*
             +-------+-------+-------+-------+-------+-------+
             | byte5 | byte4 | byte3 | byte2 | byte1 | byte0 |
             +-------+-------+-------+-------+-------+-------+
             */

            /* read byte0 ~ byte1 from CTRL_MAC_ID_LO */

            macVal = *(UINT32*)pResAdrLo->virtual;
            pDrvCtrl->mac[4] = (UINT8)((macVal & 0x000000FF)>>0);
            pDrvCtrl->mac[5] = (UINT8)((macVal & 0x0000FF00)>>8);

            /* read byte2 ~ byte5 from CTRL_MAC_ID_HI */

            macVal = *(UINT32*)pResAdrHi->virtual;
            pDrvCtrl->mac[0] = (UINT8)((macVal & 0x000000FF)>>0);
            pDrvCtrl->mac[1] = (UINT8)((macVal & 0x0000FF00)>>8);
            pDrvCtrl->mac[2] = (UINT8)((macVal & 0x00FF0000)>>16);
            pDrvCtrl->mac[3] = (UINT8)((macVal & 0xFF000000)>>24);
            }
        }

    /* connect our ISR, ISR will be enabled in cpswEndStart() */

    for (i = 0; i < 3; i++)
        {
        pRes = vxbResourceAlloc (pDev, VXB_RES_IRQ, i);
        if (pRes == NULL)
            {
            DBG_MSG (DBG_ERR,"%s%d vxbResourceAlloc failed for ISR %d\n",
                     CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, i);
            goto portAttachFail;
            }

        /*
         * save the interrupt resource which is used to
         * enable/disconnect/disable for other functions
         */

        pDrvCtrl->intRes[i] = pRes;

        switch (i)
            {
            case 0:
                (void)vxbIntConnect (pDev, pRes, cpswEndRxInt, pSwCtrl);
                break;
            case 1:
                (void)vxbIntConnect (pDev, pRes, cpswEndTxInt, pSwCtrl);
                break;
            case 2:
                (void)vxbIntConnect (pDev, pRes, cpswEndMiscInt, pSwCtrl);
                break;
            default:
                break;
            }
        }

    /* allocate desc resource */

    pDrvCtrl->cpswTxDescMem = cacheDmaMalloc(sizeof(CPSW_DESC) *
                                             CPSW_DESC_CNT);
    if (pDrvCtrl->cpswTxDescMem == NULL)
        {
        DBG_MSG (DBG_ERR,"%s%d can not create tx desc\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        goto portAttachFail;
        }
    memset (pDrvCtrl->cpswTxDescMem, 0, (sizeof (CPSW_DESC)) * CPSW_DESC_CNT);

    /* translate virtual address into physical address */

    for (cnt = 0; cnt < CPSW_DESC_CNT; cnt++)
        {
        pDrvCtrl->cpswTxDescMem[cnt].phys =
        (UINT32) cpswVirToPhy ((VIRT_ADDR)(&pDrvCtrl->cpswTxDescMem[cnt]));
        }

    pDrvCtrl->cpswRxDescMem = cacheDmaMalloc(sizeof(CPSW_DESC) *
                                             CPSW_DESC_CNT);
    if (!pDrvCtrl->cpswRxDescMem)
        {
        DBG_MSG (DBG_ERR,"%s%d can not create rx desc\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        goto portAttachFail;
        }
    memset (pDrvCtrl->cpswRxDescMem, 0, (sizeof (CPSW_DESC)) * CPSW_DESC_CNT);

    /* translate virtual address into physical address */

    for (cnt = 0; cnt < CPSW_DESC_CNT; cnt++)
        {
        pDrvCtrl->cpswRxDescMem[cnt].phys =
        (UINT32) cpswVirToPhy ((VIRT_ADDR)(&pDrvCtrl->cpswRxDescMem[cnt]));
        }

    /*
    * prepare vlan value which will be programmed,
    * use the portIndex as vlan ID
    */

    pDrvCtrl->portVlan = CPSW_PORT_VLAN_PROG(pDrvCtrl->portIndex);

    /* search the phy from DTS and probe/attach the phy to system */

    if (phyAttach (pDev) != OK)
        {
        /* did't find phy node from DTS or did't match */

        DBG_MSG (DBG_ERR,"%s%d phyAttach failed\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

        goto portAttachFail;
        }

    /*
     * search the phy instance from mii system and make relation
     * ship between ethernet port and phy.
     */

    if (miiFind (pDev, cpswSlSetMiiDev) != OK)
        {
        DBG_MSG (DBG_ERR,"%s%d miiFind failed\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

        goto portAttachFail;
        }

    /* initialize mac port(s) */

    if (cpswGmacPortInit (pDrvCtrl) != OK)
        {
        DBG_MSG (DBG_ERR,"%s%d cpswGmacPortInit failed\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

        goto portAttachFail;
        }

    cpswMuxConnect (pDev, NULL);

    return OK;

portAttachFail:

    if (pDrvCtrl)
        {
        if (pDrvCtrl->cpswTxDescMem)
            (void)cacheDmaFree (pDrvCtrl->cpswTxDescMem);
        if (pDrvCtrl->cpswRxDescMem)
            (void)cacheDmaFree (pDrvCtrl->cpswRxDescMem);

        vxbMemFree (pDrvCtrl);

        vxbDevSoftcSet(pDev, NULL);
        }

    return ERROR;
    }

/*******************************************************************************
*
* cpswPhyRead - miiBus miiRead method
*
* This function implements an miiRead() method that allows PHYs
* on the miiBus to access our MII management registers.
*
* RETURNS: ERROR if invalid PHY addr, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS cpswPhyRead
    (
    VXB_DEV_ID pPhyParent,
    UINT8      phyAddr,
    UINT8      regAddr,
    UINT16   * pDataVal
    )
    {
    UINT32          ret;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    UINT32          index = 0;

    if (phyAddr >= 32)
        {
        return ERROR;
        }

    do
        {
        ret = CSR_READ_4 (pDev, pSwCtrl->mdioOffset + CPSW_USERACCESSn(index));
        } while (ret & CPSW_MDIO_GO);

    ret = phyAddr << CPSW_PHY_ADDR_SHIFT |
          regAddr << CPSW_REG_ADDR_SHIFT |
          CPSW_MDIO_GO;

    CSR_WRITE_4 (pDev, pSwCtrl->mdioOffset + CPSW_USERACCESSn(index), ret);

    do
        {
        ret = CSR_READ_4 (pDev, pSwCtrl->mdioOffset + CPSW_USERACCESSn(index));
        } while (ret & CPSW_MDIO_GO);

    *pDataVal = ret & 0xffff;

    return OK;
    }

/*******************************************************************************
*
* cpswPhyWrite - miiBus miiWrite method
*
* This function implements an miiWrite() method that allows PHYs
* on the miiBus to access our MII management registers. This routine
* works in much the same way as cpswPhyRead(), using the shortcut
* PHY management registers to make it look like there's a single
* PHY at MII address 0.
*
* RETURNS: ERROR if invalid PHY addr, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS cpswPhyWrite
    (
    VXB_DEV_ID pPhyParent,
    UINT8      phyAddr,
    UINT8      regAddr,
    UINT16     dataVal
    )
    {
    UINT32          ret;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    UINT32          index = 0;

    if (phyAddr >= 32)
        {
        return ERROR;
        }

    do
        {
        ret = CSR_READ_4 (pDev, pSwCtrl->mdioOffset + CPSW_USERACCESSn(index));
        } while (ret & CPSW_MDIO_GO);

    ret = phyAddr << CPSW_PHY_ADDR_SHIFT |
          regAddr << CPSW_REG_ADDR_SHIFT |
          CPSW_MDIO_GO | CPSW_MDIO_WRITE | dataVal;

    CSR_WRITE_4 (pDev, pSwCtrl->mdioOffset + CPSW_USERACCESSn(index), ret);

    do
        {
        ret = CSR_READ_4 (pDev, pSwCtrl->mdioOffset + CPSW_USERACCESSn(index));
        } while (ret & CPSW_MDIO_GO);

    return OK;
    }

/*****************************************************************************
*
* cpswLinkUpdate - miiBus miiLinkUpdate method
*
* This function implements an miiLinkUpdate() method that allows
* miiBus to notify us of link state changes. This routine will be
* invoked by the miiMonitor task when it detects a change in link
* status. Normally, the miiMonitor task checks for link events every
* two seconds.
*
* Once we determine the new link state, we will announce the change
* to any bound protocols via muxError(). We also update the ifSpeed
* fields in the MIB2 structures so that SNMP queries can detect the
* correct link speed.
*
* RETURNS: ERROR if obtaining the new media setting fails, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS cpswLinkUpdate
    (
    VXB_DEV_ID      pPhyParent
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl;
    VXB_DEV_ID      pDev;
    UINT32          oldStatus;
    UINT32          val;

    if (vxbDevSoftcGet(pPhyParent) == NULL)
        return ERROR;

    pDrvCtrl = (CPSW_DRV_CTRL*) vxbDevSoftcGet(pPhyParent);
    pDev     = pSwCtrl->pDev;

    if (pDrvCtrl->cpswMiiDev== NULL)
        return ERROR;

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);

    oldStatus = pDrvCtrl->cpswCurStatus;

    if (miiBusModeGet(pDrvCtrl->cpswMiiDev,
        &pDrvCtrl->cpswCurMedia, &pDrvCtrl->cpswCurStatus) == ERROR)
        {
        (void)semGive(pSwCtrl->cpswDevSem);
        return ERROR;
        }

    if (!(pDrvCtrl->cpswEndObj.flags & IFF_UP))
        {
        (void)semGive(pSwCtrl->cpswDevSem);
        return OK;
        }

    val = CSR_READ_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL);
    switch(IFM_SUBTYPE(pDrvCtrl->cpswCurMedia))
        {
        case(IFM_1000_T):
        case(IFM_1000_SX):
            val &= ~CPSW_EXT_EN;
            CSR_WRITE_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL,
                         GAMC_CTL_GIG | val);

            pDrvCtrl->cpswEndObj.mib2Tbl.ifSpeed = 1000000000;
            break;
        case(IFM_100_TX):
            val &= ~CPSW_EXT_EN;
            CSR_WRITE_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL,
                    val & ~GAMC_CTL_GIG);

            pDrvCtrl->cpswEndObj.mib2Tbl.ifSpeed = 100000000;
            break;
        case(IFM_10_T):
            val |= CPSW_EXT_EN;
            CSR_WRITE_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL,
                         val & ~GAMC_CTL_GIG);

            pDrvCtrl->cpswEndObj.mib2Tbl.ifSpeed = 10000000;
            break;
        default:
            pDrvCtrl->cpswEndObj.mib2Tbl.ifSpeed = 0;
            break;
        }

    val = CSR_READ_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL);
    if ((pDrvCtrl->cpswCurMedia & IFM_GMASK) == IFM_FDX)
        CSR_WRITE_4 (pDev, pDrvCtrl->gmacOffset + \
                     CPSW_SL_MAC_CTL, val | GMAC_CTL_FULLDUPLEX);
    else
        CSR_WRITE_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL,
                     val &~ GMAC_CTL_FULLDUPLEX);

    if (pDrvCtrl->cpswEndObj.pMib2Tbl != NULL)
        pDrvCtrl->cpswEndObj.pMib2Tbl->m2Data.mibIfTbl.ifSpeed =
            pDrvCtrl->cpswEndObj.mib2Tbl.ifSpeed;

    if (!(pDrvCtrl->cpswEndObj.flags & IFF_UP))
        {
        (void)semGive (pSwCtrl->cpswDevSem);
        return (OK);
        }

    /* If status went from down to up, announce link up. */

    if (pDrvCtrl->cpswCurStatus & IFM_ACTIVE && !(oldStatus & IFM_ACTIVE))
        {
        DBG_MSG (DBG_INFO,"LINK UP - %s%d, Speed: %d %s\n",
                 pDrvCtrl->cpswEndObj.devObject.name,
                 pDrvCtrl->cpswEndObj.devObject.unit,
                 pDrvCtrl->cpswEndObj.mib2Tbl.ifSpeed/1000000,
                 (UINT32) ((pDrvCtrl->cpswCurMedia & IFM_FDX) ? "FDX":"HDX"));

        (void)jobQueueStdPost (pDrvCtrl->cpswJobQueue,
                               NET_TASK_QJOB_PRI, muxLinkUpNotify,
                               &pDrvCtrl->cpswEndObj, NULL, NULL, NULL, NULL);
        }

    /* If status went from up to down, announce link down. */

    if (!(pDrvCtrl->cpswCurStatus & IFM_ACTIVE) && oldStatus & IFM_ACTIVE)
        {
        DBG_MSG (DBG_INFO,"LINK DOWN - %s%d, Speed: %d %s\n",
                 pDrvCtrl->cpswEndObj.devObject.name,
                 pDrvCtrl->cpswEndObj.devObject.unit,
                 pDrvCtrl->cpswEndObj.mib2Tbl.ifSpeed/1000000,
                 (UINT32) ((pDrvCtrl->cpswCurMedia & IFM_FDX) ? "FDX":"HDX"));

        (void)jobQueueStdPost (pDrvCtrl->cpswJobQueue,
                               NET_TASK_QJOB_PRI, muxLinkDownNotify,
                               &pDrvCtrl->cpswEndObj, NULL, NULL, NULL, NULL);
        }

    (void)semGive(pSwCtrl->cpswDevSem);

    return (OK);
    }

/*******************************************************************************
*
* cpswMuxConnect - muxConnect method handler
*
* This function handles muxConnect() events, which may be triggered
* manually or(more likely) by the bootstrap code. Most VxBus
* initialization occurs before the MUX has been fully initialized,
* so the usual muxDevLoad()/muxDevStart() sequence must be defered
* until the networking subsystem is ready. This routine will ultimately
* trigger a call to cpswEndLoad() to create the END interface instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswMuxConnect
    (
    VXB_DEV_ID pDev,
    void     * unused
    )
    {
    CPSW_DRV_CTRL  * pDrvCtrl;
    int              index;

    pDrvCtrl = (CPSW_DRV_CTRL*) vxbDevSoftcGet(pDev);
    index    = pDrvCtrl->portIndex - 1;

    pDrvCtrl->cookie = muxDevLoad (index,
                                   cpswEndLoad, "", TRUE, pDev);

    if (pDrvCtrl->cookie)
        {
        (void)muxDevStart (pDrvCtrl->cookie);
        }

    if (_func_m2PollStatsIfPoll != NULL &&
        pDrvCtrl->cpswStatsConf.ifWatchdog == NULL)
        {
        (void)endPollStatsInit (pDrvCtrl->cookie,
                                _func_m2PollStatsIfPoll);
        }
    }

/*******************************************************************************
*
* cpswEndLoad - END driver entry point
*
* This routine initializes the END interface instance associated
* with this device. In traditional END drivers, this function is
* the only public interface, and it's typically invoked by a BSP
* driver configuration stub. With VxBus, the BSP stub code is no
* longer needed, and this function is now invoked automatically
* whenever this driver's muxConnect() method is called.
*
* For older END drivers, the load string would contain various
* configuration parameters, but with VxBus this use is deprecated.
* The load string should just be an empty string. The second
* argument should be a pointer to the VxBus device instance
* associated with this device. Like older END drivers, this routine
* will still return the device name if the init string is empty,
* since this behavior is still expected by the MUX. The MUX will
* invoke this function twice: once to obtain the device name,
* and then again to create the actual END_OBJ instance.
*
* When this function is called the second time, it will initialize
* the END object, perform MIB2 setup, allocate a buffer pool, and
* initialize the supported END capabilities. The only special
* capability we support is VLAN_MTU, since we can receive slightly
* larger than normal frames.
*
* RETURNS: An END object pointer, or NULL on error, or 0 and the name
* of the device if the <loadStr> was empty.
*
* ERRNO: N/A
*/

LOCAL END_OBJ * cpswEndLoad
    (
    char * loadStr,
    void * pArg
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl;
    VXB_DEV_ID      pDev;
    int             unit;

    if (loadStr == NULL)
        return NULL;

    if (loadStr[0] == 0)
        {
        bcopy (CPSW_PORT_NAME, loadStr, sizeof (CPSW_PORT_NAME));
        return NULL;
        }

    pDev = pArg;
    pDrvCtrl = (CPSW_DRV_CTRL*) vxbDevSoftcGet(pDev);

    /* get our instance number from load string */

    unit = atoi (loadStr);

    if (END_OBJ_INIT (&pDrvCtrl->cpswEndObj, NULL, CPSW_PORT_NAME,
        unit, &cpswNetFuncs, "CPSW END") != OK)
        {
        return NULL;
        }

    if (endM2Init (&pDrvCtrl->cpswEndObj, M2_ifType_ethernet_csmacd,
                   pDrvCtrl->mac, ETHER_ADDR_LEN, ETHERMTU, 100000000,
                   IFF_NOTRAILERS | IFF_SIMPLEX | IFF_MULTICAST | IFF_BROADCAST)
                   == ERROR)
        {
        return NULL;
        }

    pDrvCtrl->mtu = CPSW_MTU;

    if (endPoolCreate ((CPSW_DESC_CNT * 3),
        &pDrvCtrl->cpswEndObj.pNetPool) != OK)
        {
        return NULL;
        }
    pDrvCtrl->cpswPollbuf = endPoolTupleGet (pDrvCtrl->cpswEndObj.pNetPool);

    /* Set up cpswPolling stats. */

    pDrvCtrl->cpswStatsConf.ifEndObj = &pDrvCtrl->cpswEndObj;
    pDrvCtrl->cpswStatsConf.ifWatchdog = NULL;
    pDrvCtrl->cpswStatsConf.ifValidCounters = (
                        END_IFINMULTICASTPKTS_VALID |
                        END_IFINBROADCASTPKTS_VALID |
                        END_IFINOCTETS_VALID |
                        END_IFINERRORS_VALID |
                        END_IFINDISCARDS_VALID |
                        END_IFOUTMULTICASTPKTS_VALID |
                        END_IFOUTBROADCASTPKTS_VALID |
                        END_IFOUTOCTETS_VALID |
                        END_IFOUTERRORS_VALID);

    /* Set up capabilities. */

    pDrvCtrl->cpswCaps.cap_available = IFCAP_VLAN_MTU;
    pDrvCtrl->cpswCaps.cap_enabled   = IFCAP_VLAN_MTU;

    return (&pDrvCtrl->cpswEndObj);
    }

/*******************************************************************************
*
* cpswEndUnload - unload END driver instance
*
* This routine undoes the effects of cpswEndLoad(). The END object
* is destroyed, our network pool is released, the endM2 structures
* are released, and the cpswPolling stats watchdog is terminated.
*
* Note that the END interface instance can't be unloaded if the
* device is still running. The device must be stopped with muxDevStop()
* first.
*
* RETURNS: ERROR if device is still in the IFF_UP state, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndUnload
    (
    END_OBJ * pEnd
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl;

    if (pEnd->flags & IFF_UP)
        return ERROR;

    pDrvCtrl = (CPSW_DRV_CTRL *)pEnd;

    /* free the poll mode temp buffer */

    (void) endPoolTupleFree (pDrvCtrl->cpswPollbuf);

    /* release our buffer pool */

    (void) endPoolDestroy (pDrvCtrl->cpswEndObj.pNetPool);

    /* terminate stats polling */

    (void) wdDelete (pDrvCtrl->cpswStatsConf.ifWatchdog);

    (void) endM2Free (&pDrvCtrl->cpswEndObj);

    END_OBJECT_UNLOAD (&pDrvCtrl->cpswEndObj);

    /* prevent freeing of pDrvCtrl */

    return (EALREADY);
    }

/*****************************************************************************
 *
 * cpswEndHashTblPopulate - set the Multicast table entry
 *
 * This function programs the CPSW controller's multicast table entry
 * to receive frames sent to the multicast groups specified.
 * If VLAN ID is greater than zero then VLAN LLDP/Multicast is added.
 * If the interface is in IFF_ALLMULTI or IFF_PROMISC mode, the filter
 * will be programmed to receive all multicast packets by setting to the
 * bypass mode.The packet with a matching multicast destination address
 * set to be a supervisory packet.
 *
 * RETURNS: N/A
 *
 * ERRNO: N/A
 */

LOCAL void cpswEndHashTblPopulate
    (
    CPSW_DRV_CTRL  * pDrvCtrl
    )
    {
    INT32 i, type, multicast, rt;
    CPSW_SW_CTRL * pSwCtrl = pDrvCtrl->pSwCtrl;
    VXB_DEV_ID       pDev = pSwCtrl->pDev;
    ETHER_MULTI *    mCastNode = NULL;
    CPSW_ALE_TBL     tbl;
    UINT32           aleCtl;

    /* If flags & (IFF_ALLMULTI | IFF_PROMISC) is true, set to bypass mode. */

    if (pDrvCtrl->cpswEndObj.flags & (IFF_ALLMULTI|IFF_PROMISC))
        {
        aleCtl = CSR_READ_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_CONTROL);
        aleCtl |= CPSW_ALE_BYPASS;
        CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_CONTROL, aleCtl);
        return;
        }

    /* First, clear out all multicast ALEs. */

    for (i = 0; i < CPSW_ALE_ENTRY_NR; i++)
        {
        cpswAleRead (pDev, &tbl, i);
        type = (tbl.word1 >> 28) & 0x3;
        multicast = tbl.word1 & 0x100;

        if (multicast &&
            ((type == CPSW_ALE_TYPE_ADDR) || (type == CPSW_ALE_TYPE_VLAN_ADDR)))
            {
            memset ((char *)&tbl, 0, sizeof(tbl));
            cpswAleWrite (pDev, &tbl, i);
            }
        }

    /* Otherwise, add multicast ALEs */

    for (mCastNode =
        (ETHER_MULTI *) lstFirst (&pDrvCtrl->cpswEndObj.multiList);
         mCastNode != NULL;
         mCastNode = (ETHER_MULTI *) lstNext (&mCastNode->node))
        {
        rt = cpswAleAddMultiCast (pDev, (UINT8 *)mCastNode->addr,
                                  pDrvCtrl->portVlan, CPSW_ALE_MCAST_FWD_2,
                                  CPSW_ALE_MCAST_SUPER, 1 << pSwCtrl->hostPortIndex);
        if (rt != 0)
            break;
        }
    }

/*******************************************************************************
*
* cpswEndMCastAddrAdd - add a multicast address for the device
*
* This routine adds a multicast address to whatever the driver
* is already listening for. It then resets the address filter.
*
* RETURNS: OK, or ERROR when 1) the parameter is not a valid multicast address
* and 2) there is no enough resource for etherMultiAdd()
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndMCastAddrAdd
    (
    END_OBJ * pEnd,
    char    * pAddr
    )
    {
    INT32           retVal;
    CPSW_DRV_CTRL * pDrvCtrl =(CPSW_DRV_CTRL *)pEnd;

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);

    if ((pDrvCtrl->cpswEndObj.flags & IFF_UP) == 0)
        {
        (void)semGive (pSwCtrl->cpswDevSem);
        return OK;
        }

    retVal = etherMultiAdd (&pEnd->multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pEnd->nMulti++;
        cpswEndHashTblPopulate (pDrvCtrl);
        }

    (void)semGive (pSwCtrl->cpswDevSem);

    if ((retVal == ENETRESET) || (retVal == OK))
        {
        return OK;
        }
    else
        {
        return ERROR;
        }
    }

/*******************************************************************************
*
* cpswEndMCastAddrDel - delete a multicast address for the device
*
* This routine removes a multicast address from whatever the driver
* is listening for. It then resets the address filter.
*
* RETURNS: OK, or ERROR when the multicast address does not exist.
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndMCastAddrDel
    (
    END_OBJ * pEnd,
    char    * pAddr
    )
    {
    INT32           retVal;
    CPSW_DRV_CTRL * pDrvCtrl =(CPSW_DRV_CTRL *) pEnd;

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);

    if ((pDrvCtrl->cpswEndObj.flags & IFF_UP) == 0)
        {
        (void)semGive (pSwCtrl->cpswDevSem);
        return OK;
        }

    retVal = etherMultiDel (&pEnd->multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pEnd->nMulti--;
        cpswEndHashTblPopulate (pDrvCtrl);
        }

    (void)semGive (pSwCtrl->cpswDevSem);

    if ((retVal == ENETRESET) || (retVal == OK))
        {
        return OK;
        }
    else
        {
        return ERROR;
        }
    }

/*******************************************************************************
*
* cpswEndMCastAddrGet - get the multicast address list for the device
*
* This routine gets the multicast list of whatever the driver
* is already listening for.
*
* RETURNS: OK, always.
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndMCastAddrGet
    (
    END_OBJ     * pEnd,
    MULTI_TABLE * pTable
    )
    {
    INT32 retVal;
    CPSW_DRV_CTRL * pDrvCtrl =(CPSW_DRV_CTRL *) pEnd;

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);

    if ((pDrvCtrl->cpswEndObj.flags & IFF_UP) == 0)
        {
        (void)semGive (pSwCtrl->cpswDevSem);
        return OK;
        }

    retVal = etherMultiGet (&pEnd->multiList, pTable);

    if (retVal == ENETRESET)
        {
        pEnd->nMulti++;
        }

    (void)semGive (pSwCtrl->cpswDevSem);

    return OK;
    }

/*******************************************************************************
*
* cpswEndStatsDump - return polled statistics counts
*
* This routine is automatically invoked periodically by the polled
* statistics watchdog.
*
* RETURNS: always OK
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndStatsDump
    (
    CPSW_DRV_CTRL * pDrvCtrl
    )
    {
    int              i;
    VXB_DEV_ID       pDev = pSwCtrl->pDev;
    UINT32         * temp = (UINT32 *)&pSwCtrl->cpswStat;
    END_IFCOUNTERS * pEndStatsCounters;

    /*
     * hardware statistic counters are write-to-decrement,
     * after a read, we write the value read to clear
     * the counters
     */

    for (i = 0; i < sizeof (CPSW_STAT) / sizeof (UINT32); i++)
        {
        *temp = CSR_READ_4 (pDev, pSwCtrl->statsOffset + i*4);
        CSR_WRITE_4 (pDev, pSwCtrl->statsOffset + i*4, *temp);
        temp++;
        }

    pEndStatsCounters = &pDrvCtrl->cpswStatsCounters;

    pEndStatsCounters->ifInOctets = pSwCtrl->cpswStat.rxoctets;
    pEndStatsCounters->ifInMulticastPkts = pSwCtrl->cpswStat.rxmulticast;
    pEndStatsCounters->ifInBroadcastPkts = pSwCtrl->cpswStat.rxbroadcast;
    pEndStatsCounters->ifInErrors = pSwCtrl->cpswStat.rxpause +
                                    pSwCtrl->cpswStat.rxcrcerros +
                                    pSwCtrl->cpswStat.rxalignmenterrors +
                                    pSwCtrl->cpswStat.rxoversized +
                                    pSwCtrl->cpswStat.rxjabber +
                                    pSwCtrl->cpswStat.rxundersized;
    pEndStatsCounters->ifInDiscards = pDrvCtrl->cpswInDropped;
    pEndStatsCounters->ifOutOctets = pSwCtrl->cpswStat.txoctets;
    pEndStatsCounters->ifOutMulticastPkts = pSwCtrl->cpswStat.txmulticast;
    pEndStatsCounters->ifOutBroadcastPkts = pSwCtrl->cpswStat.txbroadcast;
    pEndStatsCounters->ifOutErrors = pSwCtrl->cpswStat.txpause +
                                     pSwCtrl->cpswStat.txdefered +
                                     pSwCtrl->cpswStat.txcollision +
                                     pSwCtrl->cpswStat.txexceesive +
                                     pSwCtrl->cpswStat.txsinglecol+
                                     pSwCtrl->cpswStat.txmulticol +
                                     pSwCtrl->cpswStat.txlatecol +
                                     pSwCtrl->cpswStat.txunderrun;

    return OK;
    }

/*******************************************************************************
*
* cpswEndIoctl - the driver I/O control routine
*
* This function processes ioctl requests supplied via the muxIoctl()
* routine. In addition to the normal boilerplate END ioctls, this
* driver supports the IFMEDIA ioctls, END capabilities ioctls, and
* polled stats ioctls.
*
* RETURNS: A command specific response, usually OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL INT32 cpswEndIoctl
    (
    END_OBJ * pEnd,
    INT32     cmd,
    caddr_t   data
    )
    {
    CPSW_DRV_CTRL    * pDrvCtrl = (CPSW_DRV_CTRL *)pEnd;
    VXB_DEV_ID         pDev;
    END_MEDIALIST    * mediaList;
    END_CAPABILITIES * hwCaps;
    END_MEDIA        * pMedia;
    END_RCVJOBQ_INFO * qinfo;
    UINT32             nQs;
    UINT32             portIdx;
    INT32              value;
    INT32              error = OK;

    pSwCtrl = pDrvCtrl->pSwCtrl;
    pDev    = pSwCtrl->pDev;

    if (cmd != EIOCPOLLSTART && cmd != EIOCPOLLSTOP)
        (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);

    switch (cmd)
        {
        case EIOCSADDR:
            if (data == NULL)
                error = EINVAL;
            else
                {
                INT32 flags;
                bcopy ((char *)data, (char *)pDrvCtrl->mac, ETHER_ADDR_LEN);
                bcopy ((char *)data,
                    (char *)pEnd->mib2Tbl.ifPhysAddress.phyAddress,
                    ETHER_ADDR_LEN);
                if (pEnd->pMib2Tbl != NULL)
                    bcopy ((char *)data,
                    (char *)pEnd->pMib2Tbl->m2Data.mibIfTbl.ifPhysAddress.phyAddress,
                    ETHER_ADDR_LEN);

                flags = END_FLAGS_GET (pEnd);
                if ((flags & IFF_PROMISC) == 0)
                    {
                    (void)cpswAleAddVlan(pDev,
                            (0x1 << pSwCtrl->hostPortIndex) | (0x1 << pDrvCtrl->portIndex),
                            0, 0, 0, pDrvCtrl->portVlan);
                    (void)cpswAleAddUniCast (pDev, pDrvCtrl->mac,
                            pDrvCtrl->portVlan, 0, 0, pSwCtrl->hostPortIndex);
                    }
                }
            break;

        case EIOCGADDR:
            if (data == NULL)
                error = EINVAL;
            else
                bcopy ((char *)pDrvCtrl->mac, (char *)data, ETHER_ADDR_LEN);
            break;

        case EIOCSFLAGS:
            {
            INT32 oldFlags;
            INT32 newFlags;
            oldFlags = END_FLAGS_GET (pEnd);
            value = (INT32)(long)data;
            if (value < 0)
                {
                value = -value;
                value--;
                END_FLAGS_CLR (pEnd, value);
                }
            else
                {
                END_FLAGS_SET (pEnd, value);
                }

            newFlags = END_FLAGS_GET (pEnd);

            if (!(newFlags & IFF_PROMISC) && (oldFlags & IFF_PROMISC))
                {
                UINT32 tmp = CPSW_ALE_EN_TABLE | CPSW_ALE_CLR_TABLE;
                if (pSwCtrl->workMode == CPSW_MODE_INDEPENDENT_PORT)
                    tmp |= CPSW_ALE_VLAN_AWARE;
                CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_CONTROL, tmp);

                for(portIdx = 0; portIdx < NR_MAC_PORTS; portIdx++)
                    {
                    if (pSwCtrl->port[portIdx] != NULL)
                        {
                        (void)cpswAleAddVlan (pDev,
                                              (0x1 << pSwCtrl->hostPortIndex) |
                                              (0x1 << pSwCtrl->port[portIdx]->portIndex),
                                              0, 0, 0, pSwCtrl->port[portIdx]->portVlan);
                        (void)cpswAleAddUniCast (pDev, pSwCtrl->port[portIdx]->mac,
                                                 pSwCtrl->port[portIdx]->portVlan,
                                                 0, 0, pSwCtrl->hostPortIndex);
                        }
                    }
                }
            if ((newFlags & IFF_PROMISC) && !(oldFlags & IFF_PROMISC))
                {
                CSR_WRITE_4 (pDev, pSwCtrl->aleOffset + CPSW_ALE_CONTROL,
                             CPSW_ALE_EN_TABLE | CPSW_ALE_CLR_TABLE |
                             CPSW_ALE_BYPASS);
                }
            }

            break;

        case EIOCGFLAGS:
            if (data == NULL)
                error = EINVAL;
            else
                *(INT32 *)data = END_FLAGS_GET(pEnd);

            break;

        case EIOCMULTIADD:
            error = cpswEndMCastAddrAdd (pEnd,(char *) data);
            break;

        case EIOCMULTIDEL:
            error = cpswEndMCastAddrDel (pEnd,(char *) data);
            break;

        case EIOCMULTIGET:
            error = cpswEndMCastAddrGet (pEnd,(MULTI_TABLE *) data);
            break;

        case EIOCPOLLSTART:
            /* set poll flag */

            pDrvCtrl->cpswPolling = TRUE;

            /* stop tx rx dma channel interrupt */

            /* stop tx channel interrupt */

            CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_CLR,
                             BIT(pDrvCtrl->cpswTxDmaChan));
            CSR_CLR_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_TX_EN,
                        BIT(pDrvCtrl->cpswTxDmaChan));

            /* stop rx channel interrupt */

            CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_CLR,
                             BIT(pDrvCtrl->cpswRxDmaChan));
            CSR_CLR_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_RX_EN,
                        BIT(pDrvCtrl->cpswRxDmaChan));

            /*
             * We may have been asked to enter polled mode while there are
             * transmissions pending. This is a problem, because the polled
             * transmit routine expects that the TX ring will be empty when
             * it's called. In order to guarantee this, we have to drain
             * the TX ring here. We could also just plain reset and
             * reinitialize the transmitter, but this is faster.
             */

            while (pDrvCtrl->cpswTxFree < CPSW_DESC_CNT)
                {
                M_BLK_ID pMblk;
                volatile CPSW_DESC * desc;

                desc = &pDrvCtrl->cpswTxDescMem[pDrvCtrl->cpswTxQHead];

                 if (desc->flags & CPSW_EOQ)
                    {
                    CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                                 CPDMA_TX_CP(pDrvCtrl->cpswTxDmaChan), desc->phys);

                    if (desc->link)
                        {
                        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                                     CPDMA_TX_HDP(pDrvCtrl->cpswTxDmaChan), desc->link);
                        }
                    }

                 while (desc->flags & CPSW_OWNERSHIP)
                     ;

                pMblk = pDrvCtrl->cpswTxblk[pDrvCtrl->cpswTxQHead];

                if (pMblk != NULL)
                    {
                    endPoolTupleFree (pMblk);
                    pDrvCtrl->cpswTxblk[pDrvCtrl->cpswTxQHead] = NULL;
                    }

                pDrvCtrl->cpswTxFree++;
                CPSW_DESC_INC (pDrvCtrl->cpswTxQHead, CPSW_DESC_CNT);
                }
            break;

        case EIOCPOLLSTOP:

            pDrvCtrl->cpswPolling = FALSE;

            /* enable relative tx channel interrupts */

            CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_SET,
                             BIT(pDrvCtrl->cpswTxDmaChan));
            CSR_SET_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_TX_EN,
                        BIT(pDrvCtrl->cpswTxDmaChan));

            /* enable relative rx channel interrupts */

            CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_SET,
                             BIT(pDrvCtrl->cpswRxDmaChan));
            CSR_SET_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_RX_EN,
                        BIT(pDrvCtrl->cpswRxDmaChan));

            break;

        case EIOCGMIB2233:
        case EIOCGMIB2:
            error = endM2Ioctl (&pDrvCtrl->cpswEndObj, cmd, data);
            break;

        case EIOCGPOLLCONF:
            if (data == NULL)
                error = EINVAL;
            else
                {
                pDrvCtrl->cpswStatsConf.ifPollInterval = sysClkRateGet();
                *((END_IFDRVCONF **)data) = &pDrvCtrl->cpswStatsConf;
                }
            break;

        case EIOCGPOLLSTATS:
            if (data == NULL)
                error = EINVAL;
            else
                {
                error = cpswEndStatsDump (pDrvCtrl);
                if (error == OK)
                    *((END_IFCOUNTERS **)data) = &pDrvCtrl->cpswStatsCounters;
                }
            break;

        case EIOCGMEDIALIST:
            if (data == NULL)
                {
                error = EINVAL;
                break;
                }
            if (pDrvCtrl->cpswMediaList->endMediaListLen == 0)
                {
                error = ENOTSUP;
                break;
                }

            mediaList =(END_MEDIALIST *)data;
            if (mediaList->endMediaListLen <
                pDrvCtrl->cpswMediaList->endMediaListLen)
                {
                mediaList->endMediaListLen =
                    pDrvCtrl->cpswMediaList->endMediaListLen;
                error = ENOSPC;
                break;
                }

            bcopy ((char *)pDrvCtrl->cpswMediaList, (char *)mediaList,
                  sizeof (END_MEDIALIST) + (sizeof (UINT32) *
                  pDrvCtrl->cpswMediaList->endMediaListLen));
            break;

        case EIOCGIFMEDIA:
            if (data == NULL)
                error = EINVAL;
            else
                {
                pMedia =(END_MEDIA *)data;
                pMedia->endMediaActive = pDrvCtrl->cpswCurMedia;
                pMedia->endMediaStatus = pDrvCtrl->cpswCurStatus;
                }
            break;

        case EIOCSIFMEDIA:
            if (data == NULL)
                error = EINVAL;
            else
                {
                pMedia =(END_MEDIA *)data;
                (void)miiBusModeSet (pDrvCtrl->cpswMiiDev,
                                     pMedia->endMediaActive);
                (void)cpswLinkUpdate (pDrvCtrl->cpswMiiDev);
                error = OK;
                }
            break;

        case EIOCGIFCAP:
            hwCaps =(END_CAPABILITIES *)data;
            if (hwCaps == NULL)
                {
                error = EINVAL;
                break;
                }
            hwCaps->cap_available = pDrvCtrl->cpswCaps.cap_available;
            hwCaps->cap_enabled = pDrvCtrl->cpswCaps.cap_enabled;
            break;

        case EIOCSIFCAP:
            error = ENOTSUP;
            break;

        case EIOCGIFMTU:
            if (data == NULL)
                error = EINVAL;
            else
                *(INT32 *)data = pEnd->mib2Tbl.ifMtu;
            break;

        case EIOCSIFMTU:
            value =(INT32)data;
            if (value <= 0 || value > pDrvCtrl->mtu)
                {
                error = EINVAL;
                break;
                }
            pEnd->mib2Tbl.ifMtu = value;
            if (pEnd->pMib2Tbl != NULL)
                pEnd->pMib2Tbl->m2Data.mibIfTbl.ifMtu = value;
            break;

        case EIOCGRCVJOBQ:
            if (data == NULL)
                {
                error = EINVAL;
                break;
                }

            qinfo = (END_RCVJOBQ_INFO *)data;
            nQs = qinfo->numRcvJobQs;
            qinfo->numRcvJobQs = 1;
            if (nQs < 1)
                error = ENOSPC;
            else
                qinfo->qIds[0] = pDrvCtrl->cpswJobQueue;
            break;

        default:
            error = EINVAL;
            break;
        }

    if (cmd != EIOCPOLLSTART && cmd != EIOCPOLLSTOP)
        (void)semGive (pSwCtrl->cpswDevSem);

    return (error);
    }

/*******************************************************************************
*
* cpswEndStart - start the device
*
* This function resets the device to put it into a known state and
* then configures it for RX and TX operation. The RX and TX configuration
* registers are initialized, and the address of the RX DMA window is
* loaded into the device. Interrupts are then enabled, and the initial
* link state is configured.
*
* Note that this routine also checks to see if an alternate jobQueue
* has been specified via the vxbParam subsystem. This allows the driver
* to divert its work to an alternate processing task, such as may be
* done with TIPC. This means that the jobQueue can be changed while
* the system is running, but the device must be stopped and restarted
* for the change to take effect.
*
* RETURNS: ERROR if device initialization failed, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndStart
    (
    END_OBJ * pEnd
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl = (CPSW_DRV_CTRL *)pEnd;
    CPSW_DESC     * desc;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    VXB_DEV_ID      pDevEnd = pDrvCtrl->pDev;
    UINT32          val;
    M_BLK_ID        pMblk;
    int             i;
    STATUS          ret = OK;

    (void)semTake(pSwCtrl->cpswDevSem, WAIT_FOREVER);
    (void)END_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    if (pEnd->flags & IFF_UP)
        {
        ret = ERROR;
        DBG_MSG (DBG_ERR, "end already startup\n");

        goto out;
        }

    /* we use the (port index number - 1)  as the channel number */

    pDrvCtrl->cpswRxDmaChan = pDrvCtrl->portIndex - 1;
    pDrvCtrl->cpswTxDmaChan = pDrvCtrl->portIndex - 1;

    /* request for rx dma channel,  */

    if (cpswDmaChanRequest (pSwCtrl, TRUE, pDrvCtrl->cpswRxDmaChan) != OK)
        {
        ret = ERROR;
        DBG_MSG(DBG_ERR,"%s%d: alloc rx dma chan[%d] failed\n",
                CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, pDrvCtrl->cpswRxDmaChan);
        goto failed;
        }

    /* request for tx dma channel */

    if (cpswDmaChanRequest (pSwCtrl, FALSE, pDrvCtrl->cpswTxDmaChan) != OK)
        {
        ret = ERROR;
        DBG_MSG(DBG_ERR,"%s%d: alloc tx dma chan[%d] failed\n",
                CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, pDrvCtrl->cpswRxDmaChan);
        goto failed;
        }

    DBG_MSG (DBG_INFO,"%s%d tx dma channel number: %d\n",
             CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, pDrvCtrl->cpswTxDmaChan);
    DBG_MSG (DBG_INFO,"%s%d rx dma channel number: %d\n",
             CPSW_PORT_NAME, pDrvCtrl->portIndex - 1, pDrvCtrl->cpswRxDmaChan);

    /*
     * setup channel dma priority mapping for independent port mode,
     * let all priority[0~3] goto same channel.
     *
     * CPSW_PORT_P0_CPDMA_RX_CH_MAP Register Field Descriptions
     *
     * Bit      Description
     * -----    -----------------------------------------------------
     * 30-28    Port 2 Priority 3 packets go to this CPDMA Rx Channel
     * 26-24    Port 2 Priority 2 packets go to this CPDMA Rx Channel
     * 22-20    Port 2 Priority 1 packets go to this CPDMA Rx Channel
     * 18-16    Port 2 Priority 0 packets go to this CPDMA Rx Channel
     * 14-12    Port 1 Priority 3 packets go to this CPDMA Rx Channel
     * 10-8     Port 1 Priority 2 packets go to this CPDMA Rx Channel
     * 6-4      Port 1 Priority 1 packets go to this CPDMA Rx Channel
     * 2-0      Port 1 Priority 0 packets go to this CPDMA Rx Channel
     */

    val = CSR_READ_4 (pDev, pSwCtrl->portOffset + CPDMA_RX_CH_MAP);

    /* for port 1 */
    if ((pDrvCtrl->portIndex - 1) == 0)
        {
        val &= 0xffff0000; /* do not only bit 16~31 */
        val |= ((pDrvCtrl->cpswRxDmaChan << 0) |
                (pDrvCtrl->cpswRxDmaChan << 4) |
                (pDrvCtrl->cpswRxDmaChan << 8) |
                (pDrvCtrl->cpswRxDmaChan << 12));
        }
    /* for port 2 */
    else
        {
        val &= 0x0000ffff; /* do not only bit 0~15 */
        val |= ((pDrvCtrl->cpswRxDmaChan << 16)|
                (pDrvCtrl->cpswRxDmaChan << 20)|
                (pDrvCtrl->cpswRxDmaChan << 24)|
                (pDrvCtrl->cpswRxDmaChan << 28));
        }

    CSR_WRITE_4 (pDev, pSwCtrl->portOffset + CPDMA_RX_CH_MAP, val);

    (void) vxAtomicSet (&pDrvCtrl->cpswTxIntPend,   FALSE);
    (void) vxAtomicSet (&pDrvCtrl->cpswRxIntPend,   FALSE);
    (void) vxAtomicSet (&pDrvCtrl->cpswMiscIntPend, FALSE);

    /* set the default job queue to netJobQueueId */

    pDrvCtrl->cpswJobQueue = netJobQueueId;

    /* setup the job queue fucntion */

    QJOB_SET_PRI(&pDrvCtrl->cpswTxQJob, NET_TASK_QJOB_PRI);
    pDrvCtrl->cpswTxQJob.func = (QJOB_FUNC)cpswEndTxHandle;

    QJOB_SET_PRI(&pDrvCtrl->cpswRxQJob, NET_TASK_QJOB_PRI);
    pDrvCtrl->cpswRxQJob.func = (QJOB_FUNC)cpswEndRxHandle;

    QJOB_SET_PRI(&pDrvCtrl->cpswMiscQJob, NET_TASK_QJOB_PRI);
    pDrvCtrl->cpswMiscQJob.func = (QJOB_FUNC)cpswEndMiscHandle;

    /* set up rx mblks */

    for (i = 0; i < CPSW_DESC_CNT; i++)
        {
        pMblk = endPoolTupleGet (pDrvCtrl->cpswEndObj.pNetPool);
        if (!pMblk)
            {
            ret = ERROR;
            DBG_MSG (DBG_ERR,"%s%d alloc end truple failed\n",
                     CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

            goto failed;
            }

        (void)cacheInvalidate (DATA_CACHE, pMblk->m_data, pMblk->m_len);

        pDrvCtrl->cpswRxblk[i] = pMblk;

        desc = pDrvCtrl->cpswRxDescMem + i;

        desc->buf    = (UINT32) cpswVirToPhy (mtod(pMblk, UINT32));
        desc->len    = CPSW_MTU;
        desc->offset = 0;
        desc->flags  = CPSW_OWNERSHIP;
        }

    /* link all the RX descriptors, here we need physical address */

    desc = (CPSW_DESC *)pDrvCtrl->cpswRxDescMem[0].phys;
    for (i = 0; i < CPSW_DESC_CNT - 1; i++)
        {
        pDrvCtrl->cpswRxDescMem[i].link = (UINT32)(desc + (i + 1));
        }

    pDrvCtrl->cpswRxDescTail = &pDrvCtrl->cpswRxDescMem [CPSW_DESC_CNT - 1];

    CPDMA_REG_WRITE (pDev, CPDMA_RX_FREEBUF(pDrvCtrl->cpswRxDmaChan),
                     CPSW_DESC_CNT);

    CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                 CPDMA_RX_HDP(pDrvCtrl->cpswRxDmaChan),
                 pDrvCtrl->cpswRxDescMem[0].phys);

    /* setup desc index */

    pDrvCtrl->cpswTxQHead = CPSW_TXQ_INVALID;
    pDrvCtrl->cpswTxFree  = CPSW_DESC_CNT;
    pDrvCtrl->cpswTxstall = FALSE;
    pDrvCtrl->cpswTxIdx   = 0;
    pDrvCtrl->cpswRxIdx   = 0;

    /* enable the GMII */

    val = CSR_READ_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL);
    val |= CPSW_GMII_EN;
    CSR_WRITE_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL, val);

    /* enable relative tx channel interrupts */

    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_SET,
                     BIT(pDrvCtrl->cpswTxDmaChan));
    CSR_SET_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_TX_EN,
                BIT(pDrvCtrl->cpswTxDmaChan));

    /* enable relative rx channel interrupts */

    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_SET,
                     BIT(pDrvCtrl->cpswRxDmaChan));
    CSR_SET_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_RX_EN,
                BIT(pDrvCtrl->cpswRxDmaChan));

    /* enable misc interrupts */

    CPDMA_REG_WRITE (pDev, CPDMA_INTMASK_SET, 0x2);
    CSR_SET_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_MISC_EN, 0x1f);

    /* tell the stack that we are on-line */

    pDrvCtrl->cpswCurMedia = IFM_ETHER | IFM_NONE;
    pDrvCtrl->cpswCurStatus = IFM_AVALID;

    (void)miiBusModeSet (pDrvCtrl->cpswMiiDev, IFM_ETHER | IFM_AUTO);

    END_FLAGS_SET (pEnd, (IFF_UP | IFF_RUNNING));

    /* enable interrupts */

    (void) vxbIntEnable (pDevEnd, pDrvCtrl->intRes[0]);
    (void) vxbIntEnable (pDevEnd, pDrvCtrl->intRes[1]);
    (void) vxbIntEnable (pDevEnd, pDrvCtrl->intRes[2]);

    goto out;

failed:
    if (pDrvCtrl->cpswRxDmaChan >= 0)
        (void)cpswDmaChanRelease (pSwCtrl, TRUE, pDrvCtrl->cpswRxDmaChan);
    if (pDrvCtrl->cpswTxDmaChan >= 0)
        (void)cpswDmaChanRelease (pSwCtrl, FALSE, pDrvCtrl->cpswTxDmaChan);

    for (i = 0; i < CPSW_DESC_CNT; i++)
        {
        if (pDrvCtrl->cpswRxblk[i])
            endPoolTupleFree (pDrvCtrl->cpswRxblk[i]);

        pDrvCtrl->cpswRxblk[i] = NULL;
        }

    DBG_MSG(DBG_ERR,"%s%d: cpswEndStart() failed\n",
            CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

out:
    (void)END_TX_SEM_GIVE (pEnd);
    (void)semGive (pSwCtrl->cpswDevSem);

    DBG_MSG(DBG_INFO,"%s%d: cpswEndStart() ok\n",
            CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

    return ret;
    }

/*******************************************************************************
*
* cpswEndStop - stop the device
*
* This function undoes the effects of cpswEndStart(). The device is shut
* down and all resources are released. Note that the shutdown process
* pauses to wait for all pending RX, TX and link event jobs that may have
* been initiated by the interrupt handler to complete. This is done
* to prevent tNetTask from accessing any data that might be released by
* this routine.
*
* RETURNS: ERROR if device shutdown failed, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndStop
    (
    END_OBJ * pEnd
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl = (CPSW_DRV_CTRL *)pEnd;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    VXB_DEV_ID      pDevEnd = pDrvCtrl->pDev;
    UINT32          val;
    int             i;

    DBG_MSG(DBG_INFO, "%s%d: cpswEndStop is called\n",
            CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);

    if ((pEnd->flags & IFF_UP) == 0)
        {
        (void)semGive (pSwCtrl->cpswDevSem);
        return OK;
        }

    /* clear IFF_UP | IFF_RUNNING */

    END_FLAGS_CLR (pEnd,(IFF_UP | IFF_RUNNING));

    /* disable the GMII */

    val = CSR_READ_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL);
    val &= ~CPSW_GMII_EN;
    CSR_WRITE_4 (pDev, pDrvCtrl->gmacOffset + CPSW_SL_MAC_CTL, val);

    /* stop tx channel interrupt */

    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_CLR,
                     BIT(pDrvCtrl->cpswTxDmaChan));
    CSR_CLR_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_TX_EN,
                BIT(pDrvCtrl->cpswTxDmaChan));

    /* stop rx channel interrupt */

    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_CLR,
                     BIT(pDrvCtrl->cpswRxDmaChan));
    CSR_CLR_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_RX_EN,
                BIT(pDrvCtrl->cpswRxDmaChan));

    /* disable interrupts */

    (void) vxbIntDisable (pDevEnd, pDrvCtrl->intRes[0]);
    (void) vxbIntDisable (pDevEnd, pDrvCtrl->intRes[1]);
    (void) vxbIntDisable (pDevEnd, pDrvCtrl->intRes[2]);

    /* release rx mblk resources */

    for (i = 0; i < CPSW_DESC_CNT; i++)
        {
        if (pDrvCtrl->cpswRxblk[i] != NULL)
            {
            endPoolTupleFree (pDrvCtrl->cpswRxblk[i]);
            pDrvCtrl->cpswRxblk[i] = NULL;
            }
        }

    /*
     * Flush the recycle cache to shake loose any of our mBlks that may be
     * stored there.
     */

    endMcacheFlush ();

    (void) END_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    for (i = 0; i < CPSW_DESC_CNT; i++)
        {
        if (pDrvCtrl->cpswTxblk [i] != NULL)
            {
            endPoolTupleFree (pDrvCtrl->cpswTxblk [i]);
            pDrvCtrl->cpswTxblk [i] = NULL;
            }
        }

    (void) END_TX_SEM_GIVE (pEnd);

    /* release DMA channel resource */

    (void) cpswDmaChanRelease (pSwCtrl, TRUE,  pDrvCtrl->cpswRxDmaChan);
    (void) cpswDmaChanRelease (pSwCtrl, FALSE, pDrvCtrl->cpswTxDmaChan);

    (void) semGive (pSwCtrl->cpswDevSem);

    DBG_MSG(DBG_INFO, "%s%d: cpswEndStop done\n",
            CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

    return OK;
    }

/*******************************************************************************
*
* cpswEndTxInt - interrupt handle for tx done
*
* This routine is the interrupt handle for tx done.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndTxInt
    (
    CPSW_SW_CTRL * pSwCtrl
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    UINT32          txStat;
    UINT32          txChannel;

    /* disable all tx interrupt first */

    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_CLR, 0xff);
    CSR_WRITE_4(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_TX_EN, 0);

    /* ack the tx interrupts by write 0x2 to CPDMA_EOI_VECTOR */

    CPDMA_REG_WRITE (pDev, CPDMA_DMA_EOI, EOI_VEC_TX_DONE);

    /* ERRATA Advisory 1.0.9 of am335x */

    if (pSwCtrl->errataNeedFix == TRUE)
        {
        vxbWrite32 (CPSW_HANDLE(pDev),
                    (UINT32*)((char*)(pSwCtrl->dm6Base) + GPTIMER_IRQ_STATUS),
                    TCAR_IF_FLAG);
        }

    /*
     * read the TX_STAT bit address location to determine which
     * channel(s) caused the interrupt
     */

    txStat = CPDMA_REG_READ (pDev, CPDMA_TX_INTSTAT_RAW) & 0xff;

    /* search looking for tx events */

    for (txChannel = 0; txChannel < NR_MAC_PORTS; txChannel++)
        {
        if (txStat & BIT(txChannel))
            {
            pDrvCtrl = pSwCtrl->port[txChannel];

            if ((pDrvCtrl != NULL)
                && (vxAtomicCas (&pDrvCtrl->cpswTxIntPend, FALSE, TRUE)
                    == TRUE))
                {
                /* post the tx done job to task */

                (void) jobQueuePost (pDrvCtrl->cpswJobQueue,
                                     &pDrvCtrl->cpswTxQJob);
                }
            }
        }

    /* enable the tx interrupt except the one which has just occured */

    CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_SET, (~txStat)&0xff);
    CSR_SET_BIT (pDev, pSwCtrl->wrOffset + CPSW_WR_C0_TX_EN,
                (~txStat)&0xff);
    }

/*******************************************************************************
*
* cpswEndRxInt - interrupt handle for rx done
*
* This routine is the interrupt handle for rx done.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndRxInt
    (
    CPSW_SW_CTRL * pSwCtrl
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    UINT32          rxStat;
    UINT32          rxChannel;

    /* disable all rx interrupt first */

    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_CLR, 0xff);
    CSR_WRITE_4(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_RX_EN, 0);

    /* ack the rx interrupts by write 0x1 to CPDMA_EOI_VECTOR */

    CPDMA_REG_WRITE (pDev, CPDMA_DMA_EOI, EOI_VEC_RX_DONE);

    /* ERRATA Advisory 1.0.9 of am335x */

    if (pSwCtrl->errataNeedFix == TRUE)
        {
        vxbWrite32 (CPSW_HANDLE(pDev),
                    (UINT32*)((char*)(pSwCtrl->dm5Base) + GPTIMER_IRQ_STATUS),
                    TCAR_IF_FLAG);
        }

    /*
     * read the RX_STAT bit address location to determine which
     * channel(s) caused the interrupt
     */

    rxStat = CPDMA_REG_READ (pDev, CPDMA_RX_INTSTAT_RAW) & 0xff;

    /* search looking for rx events */

    for (rxChannel = 0; rxChannel < NR_MAC_PORTS; rxChannel++)
        {
        if (rxStat & BIT(rxChannel))
            {
            pDrvCtrl = pSwCtrl->port[rxChannel];

            if ((pDrvCtrl != NULL) &&
                (vxAtomicCas (&pDrvCtrl->cpswRxIntPend, FALSE, TRUE)
                 == TRUE))
                {
                /* post the rx done job to task */

                (void) jobQueuePost (pDrvCtrl->cpswJobQueue,
                                     &pDrvCtrl->cpswRxQJob);
                }
            }
        }

    /* enable the rx interrupt except the one which has just occured */

    CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_SET, (~rxStat)&0xff);
    CSR_SET_BIT (pDev, pSwCtrl->wrOffset + CPSW_WR_C0_RX_EN,
                 (~rxStat)&0xff);
    }

/*******************************************************************************
*
* cpswEndMiscInt - interrupt handle for misc interrupt
*
* This routine is the interrupt handle for misc interrupt.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndMiscInt
    (
    CPSW_SW_CTRL * pSwCtrl
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    UINT32          dmaStatus, rxErr, txErr;
    UINT32          rxChan, txChan, errChanNum;

    /* ack the interrupt */

    CPDMA_REG_WRITE (pDev, CPDMA_DMA_EOI, EOI_VEC_MISC);

    /* disable msic interrupt */

    CSR_WRITE_4(pDev, CPSW_DMA_BAR(pDev) + CPDMA_INTMASK_CLR, 0x2);

    /* get error code */

    dmaStatus = CPDMA_REG_READ (pDev, CPDMA_DMASTATUS);
    rxErr     = CPDMA_RX_HOST_ERR_CODE(dmaStatus);
    txErr     = CPDMA_TX_HOST_ERR_CODE(dmaStatus);
    rxChan    = CPDMA_RX_HOST_ERR_CHAN(dmaStatus);
    txChan    = CPDMA_TX_HOST_ERR_CHAN(dmaStatus);

    if (rxErr || txErr)
        {
        errChanNum = rxChan;
        pDrvCtrl = pSwCtrl->port[errChanNum];
        if(pDrvCtrl != NULL)
            {
            /* log the error message */
            END_LOG2 ("%s%d: dma status error\n",
                      CPSW_PORT_NAME, (pDrvCtrl->portIndex - 1));
            END_LOG6 ("rx chan = %d rx err code = %d (%s)\n"
                      "tx chan = %d tx err code = %d (%s)\n",
                      rxChan, rxErr, rxChanErrMsg[rxErr],
                      txChan, txErr, txChanErrMsg[txErr]);

            if (vxAtomicCas (&pDrvCtrl->cpswMiscIntPend, FALSE, TRUE) == TRUE)
                {
                /* post the job to task */

                (void) jobQueuePost (pDrvCtrl->cpswJobQueue,
                                     &pDrvCtrl->cpswMiscQJob);
                }
            }
        }
    }

/*******************************************************************************
*
* cpswEndRxHandle - process received frames
*
* This function is scheduled by the ISR to run in the context of tNetTask
* whenever an RX interrupt is received. It processes packets from the
* RX window and encapsulates them into mBlk tuples which are handed up
* to the MUX.
*
* There may be several packets waiting in the window to be processed.
* We take care not to process too many packets in a single run through
* this function so as not to monopolize tNetTask and starve out other
* jobs waiting in the jobQueue. If we detect that there's still more
* packets waiting to be processed, we queue ourselves up for another
* round of processing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndRxHandle
    (
    void * pArg
    )
    {
    QJOB          * job = pArg;
    CPSW_DRV_CTRL * pDrvCtrl = member_to_object (job,
                               CPSW_DRV_CTRL, cpswRxQJob);
    CPSW_SW_CTRL  * pSwCtrl = pDrvCtrl->pSwCtrl;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    UINT32          inPktLen;
    CPSW_DESC     * pDesc;
    M_BLK_ID        pMblk;
    M_BLK_ID        newblk;
    UINT32          val;
    INT32           loopCounter = CPSW_DESC_CNT;
    INT32           status;

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);

    val = CSR_READ_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                      CPDMA_RX_CP(pDrvCtrl->cpswRxDmaChan));
    CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                 CPDMA_RX_CP(pDrvCtrl->cpswRxDmaChan), val);

    while (loopCounter--)
        {
        pMblk = NULL;
        pDesc = &pDrvCtrl->cpswRxDescMem[pDrvCtrl->cpswRxIdx];
        if (pDesc->flags & CPSW_OWNERSHIP)
            {
            break;
            }

        inPktLen = pDesc->flags & CPSW_PKT_LEN_MASK;
        status   = pDesc->flags & CPSW_EOQ;

        if ((pDesc->flags & CPSW_PKT_ERROR) || !inPktLen)
            {
            DBG_MSG(DBG_ERR, "%s%d receive wrong packet\n",
                    CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

            pDesc->len    = CPSW_MTU;
            pDesc->offset = 0;
            pDesc->flags  = CPSW_OWNERSHIP;

            pDrvCtrl->cpswInDropped++;

            CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                         CPDMA_RX_CP(pDrvCtrl->cpswRxDmaChan),  pDesc->phys);

            CPSW_DESC_INC (pDrvCtrl->cpswRxIdx, CPSW_DESC_CNT);

            goto addToTail;
            }

        newblk = endPoolTupleGet (pDrvCtrl->cpswEndObj.pNetPool);
        if (!newblk)
            {
            DBG_MSG(DBG_ERR, "%s%d endPoolTupleGet() return NULL\n",
                    CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);

            pDrvCtrl->cpswInDropped++;
            pDrvCtrl->cpswLastError.errCode = END_ERR_NO_BUF;
            muxError (&pDrvCtrl->cpswEndObj, &pDrvCtrl->cpswLastError);

            pDesc->len    = CPSW_MTU;
            pDesc->offset = 0;
            pDesc->flags  = CPSW_OWNERSHIP;

            CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                         CPDMA_RX_CP(pDrvCtrl->cpswRxDmaChan),  pDesc->phys);

            CPSW_DESC_INC (pDrvCtrl->cpswRxIdx, CPSW_DESC_CNT);

            goto addToTail;
            }

        pMblk  = pDrvCtrl->cpswRxblk[pDrvCtrl->cpswRxIdx];
        pDrvCtrl->cpswRxblk[pDrvCtrl->cpswRxIdx] = newblk;
        newblk->m_next = NULL;

        /* there is a reason to cacheInvalidate the buffer before DMA push
         * fresh data into it instead of after DMA push data into it.
         * consider a case like this:
         *
         * - a fresh pNewMblk as been allocated and it will be assigned to an
         *   RX DMA descriptor so that the MAC can use it.
         * - prior to allocating this fresh buffer, it was previously used by
         *   the TCP/IP stack, and the stack did some writes/stores to it.
         * - these writes/stores are now written to the cache, but have not
         *   been flushed to RAM yet.
         * - assign this new fresh pNewMblk to RX DMA descriptor.
         * - the CPU is busy doing other things, and it only has a limited
         *   about of cache.
         * - before the packet arrived into that fresh pNewMblk buffer, there
         *   is a long time for CPU.
         * - during this time, the CPU needs to cache other data, but it all
         *   the cache lines are in use, the CPU decides that since the RX
         *   buffer hasn't been touched in a long time, it can evict it from
         *   the cache so that it can re-use its cache lines, since there are
         *   writes pending, eviction means the CPU must flush the pending
         *   writes to RAM also.
         * - before the CPU flush the data from cache into RAM, a new ethernet
         *   frame arrives, and the MAC DMAs it to the RX buffer in RAM.
         * - then the CPU flushes the modified (and now stale) writes to the
         *   RX buffer from the cache to RAM.
         *   So this will lead a wrong packet in RAM.
         *
         * The solution is: before DMA push data into buffer, flush the relative
         * cache into RAM first.
         * cacheInvalidate() is called to make two result, one is described as
         * above-mentioned, the other is when CPU read the data from this
         * buffer, load it from RAM into cache again.
         */

        (void)cacheInvalidate (DATA_CACHE, newblk->m_data, newblk->m_len);

        if (pDesc->flags & CPSW_PASS_CRC)
            pMblk->m_len = pMblk->m_pkthdr.len = inPktLen - ETHER_CRC_LEN;
        else
            pMblk->m_len = pMblk->m_pkthdr.len = inPktLen;
        pMblk->m_flags = M_PKTHDR | M_EXT;

        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                     CPDMA_RX_CP(pDrvCtrl->cpswRxDmaChan),  pDesc->phys);

        /*
         * translate virtual address into physical address
         * and save into pDesc->buf
         */

        pDesc->buf    = (UINT32) cpswVirToPhy (mtod(newblk, UINT32));
        pDesc->len    = CPSW_MTU;
        pDesc->offset = 0;

        /* transmit the owner of this desc from CPU to CPSW hardware */

        pDesc->flags  = CPSW_OWNERSHIP;

        CPSW_DESC_INC (pDrvCtrl->cpswRxIdx, CPSW_DESC_CNT);

addToTail:
        pDesc->link = 0;
        pDrvCtrl->cpswRxDescTail->link = pDesc->phys;
        pDrvCtrl->cpswRxDescTail = pDesc;

        pDesc = &pDrvCtrl->cpswRxDescMem[pDrvCtrl->cpswRxIdx];
        if (status)
            {
            CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                         CPDMA_RX_HDP(pDrvCtrl->cpswRxDmaChan),
                         pDesc->phys);
            }

        if (pMblk)
            {
            END_CACHE_INVAL_POST_DMA (pMblk->m_data, pMblk->m_len);

            /* push the packet up to tcp/ip stack */

            END_RCV_RTN_CALL (&pDrvCtrl->cpswEndObj, pMblk);
            }
        }

    (void) semGive (pSwCtrl->cpswDevSem);

    (void) vxAtomicSet (&pDrvCtrl->cpswRxIntPend, FALSE);

    /*
     * enable the rx channel interrupt which is relative
     * with current pDrvCtrl again
     */

    if (pDrvCtrl->cpswPolling == FALSE)
        {
        CPDMA_REG_WRITE (pDev, CPDMA_RX_INTMASK_SET,
                         BIT(pDrvCtrl->cpswRxDmaChan));

        CSR_SET_BIT(pDev, pSwCtrl->wrOffset + CPSW_WR_C0_RX_EN,
                    BIT(pDrvCtrl->cpswRxDmaChan));
        }
    }

/*******************************************************************************
*
* cpswEndTxHandle - process TX completion events
*
* This function is scheduled by the ISR to run in the context of tNetTask
* whenever an TX interrupt is received. It runs through all of the
* TX register pairs and checks the TX status to see how many have
* completed. For each completed transmission, the associated TX mBlk
* is released, and the outbound packet stats are updated.
*
* If the transmitter has stalled, this routine will also call muxTxRestart()
* to drain any packets that may be waiting in the protocol send queues,
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndTxHandle
    (
    void * pArg
    )
    {
    QJOB          * job = pArg;
    CPSW_DRV_CTRL * pDrvCtrl = member_to_object (job,
                               CPSW_DRV_CTRL, cpswTxQJob);
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    BOOL            restart = FALSE;
    CPSW_DESC     * pDesc;
    M_BLK_ID        pMblk;

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);
    (void)END_TX_SEM_TAKE (&pDrvCtrl->cpswEndObj, WAIT_FOREVER);

    if (pDrvCtrl->cpswTxQHead == CPSW_TXQ_INVALID)
        {
        goto out;
        }

    while (pDrvCtrl->cpswTxFree < CPSW_DESC_CNT)
        {
        pDesc  = pDrvCtrl->cpswTxDescMem + pDrvCtrl->cpswTxQHead;
        if (pDesc->flags & CPSW_OWNERSHIP)
            {
            break;
            }

        if ((pDesc->flags & CPSW_EOQ) && (pDesc->link))
            {
            CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                         CPDMA_TX_HDP(pDrvCtrl->cpswTxDmaChan), pDesc->link);
            }

        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                     CPDMA_TX_CP(pDrvCtrl->cpswTxDmaChan), pDesc->phys);

        pMblk = pDrvCtrl->cpswTxblk[pDrvCtrl->cpswTxQHead];
        if (pMblk)
            {
            endPoolTupleFree (pMblk);
            pDrvCtrl->cpswTxblk[pDrvCtrl->cpswTxQHead] = NULL;
            }
        pDrvCtrl->cpswTxFree++;
        CPSW_DESC_INC (pDrvCtrl->cpswTxQHead, CPSW_DESC_CNT);
        }
    out:
    if (pDrvCtrl->cpswTxstall == TRUE)
        {
        pDrvCtrl->cpswTxstall = FALSE;
        restart = TRUE;
        }

    (void)END_TX_SEM_GIVE (&pDrvCtrl->cpswEndObj);
    (void)semGive (pSwCtrl->cpswDevSem);

    if (restart == TRUE)
        muxTxRestart (pDrvCtrl);

    (void)vxAtomicSet (&pDrvCtrl->cpswTxIntPend, FALSE);

    /*
     * enable the tx channel interrupt which is relative
     * with current pDrvCtrl again
     */

    if (pDrvCtrl->cpswPolling == FALSE)
        {
        CPDMA_REG_WRITE (pDev, CPDMA_TX_INTMASK_SET,
                         BIT(pDrvCtrl->cpswTxDmaChan));

        CSR_SET_BIT (pDev, pSwCtrl->wrOffset + CPSW_WR_C0_TX_EN,
                    BIT(pDrvCtrl->cpswTxDmaChan));
        }
    }

/*******************************************************************************
*
* cpswEndMiscHandle - reset hardware when need
*
* This routine reset hardware when need.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void cpswEndMiscHandle
    (
    void * pArg
    )
    {
    QJOB          * job = pArg;
    CPSW_DRV_CTRL * pDrvCtrl = member_to_object (job,
                               CPSW_DRV_CTRL, cpswMiscQJob);
    VXB_DEV_ID      pDev = pSwCtrl->pDev;

    /* reset hardware */

    if (muxDevStop (pDrvCtrl->cookie) != OK)
        {
        END_LOG2 ("%s%d muxDevStop return ERROR\n", CPSW_PORT_NAME,
                  (pDrvCtrl->portIndex - 1));
        return;
        }

    if (muxDevStart (pDrvCtrl->cookie) != OK)
        {
        END_LOG2 ("%s%d muxDevStart return ERROR\n", CPSW_PORT_NAME,
                  (pDrvCtrl->portIndex - 1));
        return;
        }

    /* enable int again */

    CSR_WRITE_4(pDev, CPSW_DMA_BAR(pDev) + CPDMA_INTMASK_SET, 0x2);
    }

/*******************************************************************************
*
* cpswEndEncap - encpswCapsulate an outbound packet in the TX ring
*
* This function transmits the packet specified in <pMblk>.
*
* RETURNS: EAGAIN if ring is full, otherwise OK.
*
* ERRNO: N/A
*/

LOCAL INT32 cpswEndEncap
    (
    CPSW_DRV_CTRL * pDrvCtrl,
    M_BLK_ID        pMblk
    )
    {
    VXB_DEV_ID  pDev = pSwCtrl->pDev;
    CPSW_DESC * pDesc = NULL;
    CPSW_DESC * pHead = NULL;
    CPSW_DESC * pPrev = NULL;
    UINT32      headIndex;
    M_BLK_ID    pCurr;

    if (pDrvCtrl->cpswTxFree == 0)
        {
        DBG_MSG (DBG_INFO, "%s%d not enough tx descriptor\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        return EAGAIN;
        }

    pDrvCtrl->cpswTxblk[pDrvCtrl->cpswTxIdx] = pMblk;
    headIndex = pDrvCtrl->cpswTxIdx;
    pHead = &pDrvCtrl->cpswTxDescMem[headIndex];

    /* setup descriptors for all the segments */

    for (pCurr = pMblk; pCurr != NULL; pCurr = pCurr->m_next)
        {
        pDesc = &pDrvCtrl->cpswTxDescMem[pDrvCtrl->cpswTxIdx];

        /* translate virtual address into physical address */

        pDesc->buf    = (UINT32)cpswVirToPhy (mtod(pCurr, UINT32));
        pDesc->offset = 0;

        /*
         * According to ethernet standard, minimum frame size is 64 bytes,
         * including packet data and FCS(frame check sequence).
         * When packet size + FCS(4bytes) is smaller than minimum frame size,
         * some padding bytes need to be added.
         * Here insure packet size is equal to or larger than 60 bytes.
         */

        pDesc->len = pCurr->m_len < CPSW_MIN_PKT_PADDING ? \
                                   CPSW_MIN_PKT_PADDING: pCurr->m_len;
        pDesc->flags = CPSW_SOP | CPSW_EOP | CPSW_OWNERSHIP | CPSW_TO_PORT_EN |
            (pDrvCtrl->portIndex << CPSW_TO_PORT_SHIFT) | pDesc->len ;

        if (pPrev)
            pPrev->link = pDesc->phys;

        /* make sure data is coherent */

        (void)cacheFlush (DATA_CACHE, pCurr->m_data, pCurr->m_len);

        pPrev = pDesc;
        CPSW_DESC_INC (pDrvCtrl->cpswTxIdx, CPSW_DESC_CNT);
        pDrvCtrl->cpswTxFree--;
        }

    pPrev->link = 0;

    /* if current tx queue is empty, trigger a new transfer */

    if (pDrvCtrl->cpswTxQHead == CPSW_TXQ_INVALID)
        {
        pDrvCtrl->cpswTxQHead = headIndex;

        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                     CPDMA_TX_HDP(pDrvCtrl->cpswTxDmaChan), pHead->phys);
        }

    /* else add the packet to current queue also detect mis queue event */

    else
        {
        pPrev = &pDrvCtrl->cpswTxDescMem[headIndex == 0 ? \
                 CPSW_DESC_CNT - 1 : headIndex - 1];
        pPrev->link = pHead->phys;

        if ((pPrev->flags & (CPSW_OWNERSHIP | CPSW_EOQ)) == CPSW_EOQ)
            {
            pPrev->flags &= ~CPSW_EOQ;

            CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                         CPDMA_TX_HDP(pDrvCtrl->cpswTxDmaChan), pHead->phys);
            }
        }

    return OK;
    }

/*******************************************************************************
*
* cpswEndSend - transmit a packet
*
* This function transmits the packet specified in <pMblk>.
*
* RETURNS: OK, ERROR, or END_ERR_BLOCK.
*
* ERRNO: N/A
*/

LOCAL INT32 cpswEndSend
    (
    END_OBJ *   pEnd,
    M_BLK_ID    pMblk
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl;
    UINT32          rval;
    M_BLK_ID        pTmp;

    pDrvCtrl =(CPSW_DRV_CTRL *)pEnd;

    (void)semTake (pSwCtrl->cpswDevSem, WAIT_FOREVER);
    (void)END_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    if (pDrvCtrl->cpswPolling == TRUE)
        {
        endPoolTupleFree (pMblk);
        (void)END_TX_SEM_GIVE (pEnd);
        (void)semGive (pSwCtrl->cpswDevSem);

        DBG_MSG (DBG_ERR,"%s%d cpswEndSend() is called in polling mode\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        return ERROR;
        }

    if (pMblk->m_next != NULL)
        {

        if ((pTmp = endPoolTupleGet (pDrvCtrl->cpswEndObj.pNetPool)) == NULL)
            goto blocked;
        pTmp->m_len = pTmp->m_pkthdr.len =
            netMblkToBufCopy (pMblk, mtod(pTmp, char *), NULL);
        pTmp->m_flags = pMblk->m_flags;

        rval = cpswEndEncap(pDrvCtrl, pTmp);
        if (rval == OK)
            endPoolTupleFree(pMblk);
        else
            endPoolTupleFree(pTmp);
        }
    else
        {
        rval = cpswEndEncap (pDrvCtrl, pMblk);
        }

    if (rval != OK)
        {
        goto blocked;
        }

    (void)END_TX_SEM_GIVE(pEnd);
    (void)semGive (pSwCtrl->cpswDevSem);

    return (OK);

blocked:
    pDrvCtrl->cpswTxstall = TRUE;

    (void)END_TX_SEM_GIVE(pEnd);
    (void)semGive (pSwCtrl->cpswDevSem);

    return (END_ERR_BLOCK);
    }

/*******************************************************************************
*
* cpswEndPollSend - polled mode transmit routine
*
* This function is similar to the cpswEndSend() routine shown above, except
* it performs transmissions synchronously with interrupts disabled. After
* the transmission is initiated, the routine will poll the state of the
* TX status register associated with the current slot until transmission
* completed.
*
* RETURNS: OK, EAGAIN, or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS cpswEndPollSend
    (
    END_OBJ * pEnd,
    M_BLK_ID  pMblk
    )
    {
    M_BLK_ID        pTemp;
    UINT32          val;
    CPSW_DRV_CTRL * pDrvCtrl = (CPSW_DRV_CTRL *)pEnd;
    CPSW_DESC     * desc;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    int             i;

    if (pDrvCtrl->cpswPolling == FALSE)
        return ERROR;

    pTemp = pDrvCtrl->cpswPollbuf;
    pTemp->m_len = pTemp->m_pkthdr.len =
         netMblkToBufCopy (pMblk, mtod(pTemp, char *), NULL);
    pTemp->m_pkthdr.csum_flags = pMblk->m_pkthdr.csum_flags;
    pTemp->m_pkthdr.csum_data  = pMblk->m_pkthdr.csum_data;
    pTemp->m_pkthdr.vlan       = pMblk->m_pkthdr.vlan;

    if (cpswEndEncap (pDrvCtrl, pTemp) != OK)
        {
        return EAGAIN;
        }

    for (i = 0; i < CPSW_TIMEOUT_VAL; i++)
        {
        val = CSR_READ_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                          CPDMA_TX_HDP(pDrvCtrl->cpswTxDmaChan));
        if (val == 0)
            break;
        }

    if (i >= CPSW_TIMEOUT_VAL)
        {
        DBG_MSG (DBG_ERR, "%s%d cpswEndPollSend timeout\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        return ERROR;
        }

    desc  = &pDrvCtrl->cpswTxDescMem[pDrvCtrl->cpswTxQHead];
    pTemp = pDrvCtrl->cpswTxblk[pDrvCtrl->cpswTxQHead];
    if (pTemp)
        {
        pDrvCtrl->cpswTxblk[pDrvCtrl->cpswTxQHead] = NULL;
        }
    pDrvCtrl->cpswTxFree++;

    CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                 CPDMA_TX_CP(pDrvCtrl->cpswTxDmaChan), desc->phys);

    CPSW_DESC_INC (pDrvCtrl->cpswTxQHead, CPSW_DESC_CNT);

    return OK;
    }

/*******************************************************************************
*
* cpswEndPollReceive - polled mode receive routine
*
* This function receives a packet in polled mode, with interrupts disabled.
* It's similar in operation to the cpswEndRxHandle() routine, except it
* doesn't process more than one packet at a time and does not load out
* buffers. Instead, the caller supplied an mBlk tuple into which this
* function will place the received packet.
*
* RETURNS: OK, or ERROR if operation failed.
*
* ERRNO: N/A
*/

LOCAL INT32 cpswEndPollReceive
    (
    END_OBJ * pEnd,
    M_BLK_ID  pMblk
    )
    {
    CPSW_DRV_CTRL * pDrvCtrl = (CPSW_DRV_CTRL *)pEnd;
    VXB_DEV_ID      pDev = pSwCtrl->pDev;
    CPSW_DESC     * pDesc;
    M_BLK_ID        pPkt;
    UINT32          inPktLen;
    STATUS          ret = OK;
    UINT32          val;
    UINT32          status;

    if (pDrvCtrl->cpswPolling == FALSE)
        {
        return ERROR;
        }

    if (!(pMblk->m_flags & M_EXT))
        {
        DBG_MSG (DBG_ERR, "%s%d cpswEndPollReceive pMblk has no M_EXT\n",
                 CPSW_PORT_NAME, pDrvCtrl->portIndex - 1);
        return ERROR;
        }

    pDesc = &pDrvCtrl->cpswRxDescMem[pDrvCtrl->cpswRxIdx];
    if ((pDesc->flags & CPSW_OWNERSHIP) != 0x0)
        {
        return (EAGAIN);
        }

    val = CSR_READ_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                      CPDMA_RX_CP(pDrvCtrl->cpswRxDmaChan));

    inPktLen = pDesc->flags & CPSW_PKT_LEN_MASK;
    status   = pDesc->flags & CPSW_EOQ;
    if (!(pDesc->flags & CPSW_PKT_ERROR) && inPktLen)
        {
        pPkt = pDrvCtrl->cpswRxblk[pDrvCtrl->cpswRxIdx];

        (void)cacheInvalidate (DATA_CACHE, pPkt->m_data, pPkt->m_len);

        if (pDesc->flags & CPSW_PASS_CRC)
            pMblk->m_len = pMblk->m_pkthdr.len = inPktLen - ETHER_CRC_LEN;
        else
            pMblk->m_len = pMblk->m_pkthdr.len = inPktLen;

        pMblk->m_flags  |= M_PKTHDR;
        bcopy (mtod(pPkt, char *),  mtod(pMblk, char *), pMblk->m_len);
        }
    else
        {
        ret = ERROR;
        }

    CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                 CPDMA_RX_CP(pDrvCtrl->cpswRxDmaChan), val);

    pDesc->len = CPSW_MTU;
    pDesc->offset = 0;
    pDesc->flags = CPSW_OWNERSHIP;

    CPSW_DESC_INC (pDrvCtrl->cpswRxIdx, CPSW_DESC_CNT);

    pDesc->link = 0;
    pDrvCtrl->cpswRxDescTail->link = pDesc->phys;
    pDrvCtrl->cpswRxDescTail = pDesc;
    pDesc = &pDrvCtrl->cpswRxDescMem[pDrvCtrl->cpswRxIdx];
    if (status)
        {
        CSR_WRITE_4 (pDev, pSwCtrl->cpdmaHdpOffset +
                     CPDMA_RX_HDP(pDrvCtrl->cpswRxDmaChan),
                     pDesc->phys);
        }

    return ret;
    }

/*****************************************************************************
*
* cpswVirToPhy - convert virtual address to physical address
*
* The routine converts virtual address to physical address.
*
* RETURNS: physical address
*
* ERRNO: N/A
*/

LOCAL PHYS_ADDR cpswVirToPhy
    (
    VIRT_ADDR virtAdrs
    )
    {
#ifdef _WRS_CONFIG_UNSUPPORTS_MMU
    return (PHYS_ADDR)virtAdrs;
#else
    PHYS_ADDR physAddr;
    (void)vmTranslate(NULL, virtAdrs, &physAddr);
    return physAddr;
#endif
    }
