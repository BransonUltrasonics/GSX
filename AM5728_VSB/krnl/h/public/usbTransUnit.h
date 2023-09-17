/* usbTransUnit.h - Translation Unit definitions */

/*
 * Copyright (c) 2005-2008, 2010, 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
    04jan14,j_x  clear unnecessary header files
01k,09sep13,j_x  Rename the "LINK" structure to avoid namespace pollution
01j,15jul10,ghs  Add magic number for USBTU_PIPE_INFO(WIND00216670)
01i,28jun10,w_x  Avoid dynamical alloc/free for IRP context (WIND00216670)
01h,29apr10,pad  Moved extern C statement after include statements.
01g,15mar10,j_x  Changed for USB debug (WIND00184542)
01f,13jan10,ghs  vxWorks 6.9 LP64 adapting
01e,11jul08,w_x  WIND00113466 and WIND00121282 merge changes
01d,04jun08,w_x  Removed msgQ mechanism for device dynamic event handling;
                 added pDriver for USBTU_DEVICE structure. (WIND00121282 fix)
01c,02feb07,ami  Path of usbHst.h changed
01b,03jan06,ami  New structure created (SPR #116302 Fix)
01a,06jan05,ami  added irpMutext in USBTU_PIPE_INFO structure. (SPR #102674)
*/

/*
DESCRIPTION
This file defines constants and structures related to the Translation
Unit.
*/

#ifndef __INCusbTransUnith
#define __INCusbTransUnith


/* includes */

#include <usbPlatform.h>
#include <usb.h>               /* general USB definitions */
#include <usbLib.h>            /* USB utility functions */
#include <usbdLib.h>           /* USBD interface */
#include <ossLib.h>            /* os specific services */
#include <usbQueueLib.h>
#include <usbListLib.h>

#include <msgQLib.h>
#include <taskLib.h>
#include <semLib.h>
#include <fioLib.h>
#include <string.h>

#include <usbHst.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* defines */

/* String length definition */

#define USBTU_NAME_LEN    32    /* Maximum length for name */

/* Wind View Event ID's */

#define USBTU_WV_CLIENT_INIT           23000   /* Client Initialization event ID */
#define USBTU_WV_CLIENT_TRANSFER       23001   /* Client Data Transfer event ID */
#define USBTU_WV_CLIENT_STD            23002   /* Client Standard Requests event ID */
#define USBTU_WV_USBD_INIT             23003   /* USBD Initialization/Shutdown event ID */

/* Wind View Event Filter */

#define USBTU_WV_FILTER       0x00000001

/* Wind View Event Log Message Size */

#define USBTU_WV_LOGSIZE      80

/* Usbtu pipe info magic number */

#define USBTU_PIPE_INFO_MAGIC_NUM      0x12345678

/* Typedefs */

/*
 *  USBTU_EVENTCODE
 */

typedef enum usbtu_eventcode
    {
    ADDDEVICE,            /* device attachment event code */
    REMOVEDEVICE,         /* device removal event code */
    SUSPENDDEVICE,        /* device suspend event code */
    RESUMEDEVICE,         /* device resume event code */
    IRPCOMPLETE           /* IRP completion event code */
    } USBTU_EVENTCODE;

/*
 * USBTU_DEVICE_DRIVER
 */

typedef struct usbtu_device_driver
    {
    UINT16 class;                         /* device class code */
    UINT16 subclass;                      /* device subclass code */
    UINT16 protocol;                      /* device protocol code */
    INT8  clientName[USBTU_NAME_LEN];    /* client name          */
    USBD_ATTACH_CALLBACK  attachCallback;  /* attach/detach callback */
    USBD_MNGMT_CALLBACK   mngmtCallback;   /* managemnt callback */
    pVOID                 mngmtCallbackParam; /* management callback param */
    MSG_Q_ID              msgQid;             /* message queue id for client */
    MSG_Q_ID              msgQidIrpComplete;   /* message queue id for IrpComplete */
    THREAD_HANDLE         threadHandle;       /* thread handle  */
    THREAD_HANDLE         irpThreadHandle;       /* thread handle  */
    pUSBHST_DEVICE_DRIVER pDriverData;        /* pointer to driver data */
    USB_LINK              tuDriverLink;       /* pointer to next in list */
    } USBTU_DEVICE_DRIVER, *pUSBTU_DEVICE_DRIVER;

/*
 * USBTU_NODE_INFO
 */

typedef struct usbtu_node_info
    {
    UINT32        hDevice;       /* handle to the device */
    UINT16        configuration; /* device configuration */
    UINT16        interface;     /* device interface */
    MSG_Q_ID      msgQid;        /* message queue id of client */
    USB_LINK      devLink;       /* link */
    pUSBTU_DEVICE_DRIVER pDriver; /* dirver for this device */
    } USBTU_NODE_INFO, *pUSBTU_NODE_INFO;

/*
 * USBTU_PIPE_INFO
 */

typedef struct usbtu_pipe_info
    {
    UINT32         hDevice;           /* handle to the device */
    UINT8          endpointAddress;   /* endpointaddress of pipe */
    UINT16         transferType;      /* transfer type on pipe */
    LIST_HEAD      irpList;           /* list of irp pending on pipe */
    UINT8          markedForDeletion; /* indicates if pipe is marked for deletion */
    UINT32	       uMaxPacketSize;    /* Maximum packet size for the endpoint */
    UINT32         bandwidth;
    MUTEX_HANDLE   irpMutex;	      /* for synchronisation */
    pUSB_IRP       pPipeDeletionIrp;  /* terminating IRP to delete the pipe safely */
    UINT32         uMagic;            /* magic number */
    } USBTU_PIPE_INFO, *pUSBTU_PIPE_INFO;

/*
 * USBTU_CLIENTMSG
 */

typedef struct usbtu_clientmsg
    {
    USBTU_EVENTCODE    eventCode;       /* event code */
    pUSB_IRP           pIrp;            /* pointer to IRP structure */
    UINT32             hDevice;         /* handle to device */
    UINT16             interface;       /* device interface */
    UINT16             class;           /* device class code */
    UINT16             subclass;        /* device subclass code */
    UINT16             protocol;        /* device protocol code */
    }USBTU_CLIENTMSG, *pUSBTU_CLIENTMSG;

/*
 * USBTU_TUMSG
 */

typedef struct usbtu_tumsg
    {
    USBTU_EVENTCODE     eventCode;      /* event code */
    UINT32              hDevice;        /* handle to device */
    UINT16              interface;      /* device interface */
    pVOID*              ppDriverData;   /* pointer to driver data */
    } USBTU_TUMSG, *pUSBTU_TUMSG;

/*
 * USBTU_DEVICE
 */

typedef struct usbtu_device
    {
    MSG_Q_ID           msgId;           /* message queue id of the client - not used now */
    pUSBTU_DEVICE_DRIVER pDriver;       /* dirver for this device */
    UINT16             class;           /* device class code */
    UINT16             subclass;        /* device subclass code */
    UINT16             protocol;        /* device protocol code */
    } USBTU_DEVICE, *pUSBTU_DEVICE;

/* global variable declarations */

extern UINT32 usbtuInitWvFilter;
extern LIST_HEAD     usbtuClientList;   /* list of clients registered with USBD */
extern LIST_HEAD     usbtuDevList;      /* list of devices attached to USB */
extern MUTEX_HANDLE  usbtuMutex;        /* mutex of Translation Unit */

#ifdef	__cplusplus
}
#endif

#endif	/* __INCusbTransUnith */

/* End of file. */


