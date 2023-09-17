/* vxbUsbPhyLib.h -  VxWorks FDT USB PHY Library Defines */

/*
 * Copyright (c) 2013, 2014, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
12jan17,whu  add phyConnected and phyDisconnected (vx7-US89247)
09apr14,m_y  add phy read write routine
23feb14,s_z  coding convention for device tree property
16jan14,s_z  code clean up
30dec13,s_z  created
*/

#ifndef __INCvxbUsbPhyLibh
#define __INCvxbUsbPhyLibh

#include <string.h>
#include <usb.h>
#include <hwif/vxBus.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define USB_PHY_STR_SIZE_MAX  16
#define TI_SITARA             1
#define FSL_IMX               2
#define FSL_VYBRID            3
#define FSL_ARM_QORIQ         4

/* typedefs */

typedef enum usbPhyType
    {
    USB_PHY_TYPE_UTMI = 0,        /* UTMI */
    USB_PHY_TYPE_UTMI_WIDE = 1,   /* UTMI WIDE */
    USB_PHY_TYPE_ULPI = 2,        /* ULPI */
    USB_PHY_TYPE_SERIAL = 3       /* SERIAL */
    }USB_PHY_TYPE;

typedef enum usbPhyMode
    {
    USB_PHY_MODE_HOST = 0,     /* Host */
    USB_PHY_MODE_TARGET = 1,   /* Target */
    USB_PHY_MODE_OTG = 2       /* OTG */
    }USB_PHY_MODE;

typedef enum usbPhyIoctlCmd
    {
    USB_PHY_SRP_REQUEST    = 0, /* Issue SRP signal */
    USB_PHY_HNP_REQUEST    = 1, /* Issue HNP signal */
    USB_PHY_ID_STATE_GET   = 2, /* Get ID pin state, A-device or B-device */
    USB_PHY_VBUS_STATE_GET = 3, /* Get Vbus pin state */
    USB_PHY_LINE_STATE_GET = 4, /* Get D+ D- line state, such as SE0? */
    USB_PHY_VBUS_CHARGE    = 5, /* Charge VBUS or not */
    USB_PHY_VBUS_DISCHARGE = 6, /* Discharge VBUS or not */
    USB_PHY_VBUS_DRIVE     = 7, /* Drive Vbus or not */
    USB_PHY_MODE_SET       = 8, /* Force set phy mode as host, target,or otg */
    USB_PHY_DP_PULLUP      = 9  /* Pull up D+ or not */
    }USB_PHY_IOCTL_CMD;

/* typedefs */

typedef struct usbPhyDev
    {
    SL_NODE      devNode;           /* Always the first */
    UINT32       uFlags;            /* Extern flags */
    VXB_DEV_ID   pDev;              /* VxBus Dev ID of USB PHY Ctlr */
    void *       pDevCtlr;          /* Specific controller data */ 
    USB_PHY_TYPE phyType;           /* Phy type */

    /* Read the phy register offset, optional */
    
    UINT32       (*phyRead)(struct usbPhyDev *, UINT32);

    /* Write value to the phy register offset, optional  */

    STATUS       (*phyWrite)(struct usbPhyDev *, UINT32, UINT32);

    /* 
     * Enable the phy to be usable. Such as functional the clock,
     * pinmux and etc used by the phy controller.
     * optional 
     */
     
    STATUS       (*phyEnable)(struct usbPhyDev *);

    /* 
     * Disable the phy to release resource used by phy controller 
     * sush as the clock, GPIO, PinMux and etc.
     * To use the phy controller later, phyEnable interface should be 
     * called again.
     * optional 
     */
     
    STATUS       (*phyDisable)(struct usbPhyDev *);

    /*
     * Issue Ioctl command, such as:
     * - get working mode (host, otg, target)
     * - issue SRP
     * - and etc.
     * optional 
     */

    STATUS       (*phyIoct)(struct usbPhyDev *, USB_PHY_IOCTL_CMD, void *); 

    /* notify phy connect status connected */

    VOID         (*phyConnected)(struct usbPhyDev *, UINT32);

    /* notify phy connect status disconnected */

    VOID         (*phyDisconnected)(struct usbPhyDev *, UINT32);

    void * pExtData; /* Extend data */
    } USB_PHY_DEV, *pUSB_PHY_DEV;

/* externs */

extern UINT32 g_usbPlatformFlag;

/* Routines for USB PHY Controllers Drivers */

IMPORT STATUS vxbUsbPhyDelete
    (
    USB_PHY_DEV * pPhyDev
    );
IMPORT STATUS vxbUsbPhyAdd
    (
    USB_PHY_DEV * pPhyDev
    );

/* Routines for USB Host/Target/OTG Controller Drivers */

IMPORT USB_PHY_TYPE vxbUsbPhyTypeGet
    (
    void * pPhy
    );
IMPORT STATUS vxbUsbPhyDisable
    (
    void * pPhy
    );
IMPORT STATUS vxbUsbPhyEnable
    (
    void * pPhy
    );
UINT32 vxbUsbPhyRead
    (
    struct usbPhyDev * pPhyDev,
    UINT32             offset
    );
STATUS vxbUsbPhyWrite
    (
    struct usbPhyDev * pPhyDev,
    UINT32             offset,
    UINT32             val
    );
#ifdef  __cplusplus
}
#endif

#endif /* __INCvxbUsbPhyLibh */


