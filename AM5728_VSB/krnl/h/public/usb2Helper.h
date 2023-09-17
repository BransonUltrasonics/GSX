/* usb2Helper.h - Generation 2 USB Helper for class drivers*/

/*
 * Copyright (c) 2008-2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,16jul14,wyy  Unify APIs of USB message to support error report (VXW6-16596)
01n,01sep13,j_x  Add touch screen
01m,28apr13,ghs  Add support of USB message (WIND00411952)
01l,19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
01k,29apr10,pad  Moved extern C statement after include statements.
01j,25nov09,w_x  Back port GEN2 class drivers for old VxWorks (WIND00192167)
01i,23mar10,j_x  Changed for USB debug (WIND00184542)
01h,09feb10,rlp  Added lstLib header file.
01g,13jan10,ghs  vxWorks 6.9 LP64 adapting
01f,03nov09,w_x  Fix URB callback and control URB cancel deadlock (WIND00189102)
01e,29oct09,ghs  Add new debug level define(WIND00188349)
01d,27aug09,w_x  Unify log mechanism and variable naming with some compiler
                 warning removal in USB GEN2 class drivers (WIND00178936)
01c,05may09,s_z  Remove usb2_class_node structure to this file
01b,24apr09,w_x  unified variable naming
01a,05nov08,adh  written
*/

#ifndef __INCusb2Helperh
#define __INCusb2Helperh


/* includes */

#include <vxWorks.h>
#include <semLib.h>
#include <msgQLib.h>
#include <lstLib.h>

/* To be used across various vxWorks versions */

#ifndef _WRS_VXWORKS_MAJOR
#define _WRS_VXWORKS_MAJOR 5   /* vxWorks 5.x has no _WRS_VXWORKS_MAJOR       */
#ifndef RUNTIME_VERSION
#define _WRS_VXWORKS_MINOR 4   /* vxWorks 5.4 defines VXWORKS_VERSION instead */
#else
#define _WRS_VXWORKS_MINOR 5   /* vxWorks 5.5 starts to have RUNTIME_VERSION  */
#endif
#endif

#include <usb.h>

/*
 * vxWorks 6.6 and above changed the USB stack directory tree
 */

#if  (_WRS_VXWORKS_MAJOR > 6) || \
     ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 6))	 
#include <usbHst.h>
#include <usbd.h>
#else
#include <usbHst.h>
#include <usbd.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif


/* defines */
#ifndef USB2_MAX_DEVICE_NAME_SIZE
#define USB2_MAX_DEVICE_NAME_SIZE       256
#endif

/* Those defines are needed when doing the class specific callbacks */
#define USB2_RT_HOST_TO_DEV             0x00
#define USB2_RT_DEV_TO_HOST             0x80

#define USB2_RT_STANDARD                0x00
#define USB2_RT_CLASS                   0x20
#define USB2_RT_VENDOR                  0x40

#define USB2_RT_DEVICE                  0x00
#define USB2_RT_INTERFACE               0x01
#define USB2_RT_ENDPOINT                0x02
#define USB2_RT_OTHER                   0x03

#ifndef USBD_NOTIFY_ALL
#define USBD_NOTIFY_ALL                 0xffff
#endif

#define USB2_ISOC_ENDP_SYNC_MASK        0x0C            /* Endpoint sync mask */
#define USB2_ISOC_ENDP_SYNC_NO_SYNC     0x00            /* No Synchonisation */
#define USB2_ISOC_ENDP_SYNC_ASYNC       0x04            /* Asynchronous */
#define USB2_ISOC_ENDP_SYNC_ADAPT       0x08            /* Adaptive */
#define USB2_ISOC_ENDP_SYNC_SYNC        0x0C            /* Synchronous*/
#define USB2_ISOC_ENDP_SYNC_ALL         0xFF            /* Match for all */

#define USB2_ISOC_ENDP_USAGE_MASK       0x30
#define USB2_ISOC_ENDP_USAGE_DATA       0x00            /* Data Endpoint */
#define USB2_ISOC_ENDP_USAGE_FEEDBACK   0x10            /* Feedback Endpoint */
#define USB2_ISOC_ENDP_USAGE_EXPLICT    0x20            /* Explicit Feedback
                                                         * Data Endpoint */

#define USB2_ISOC_ENDP_USAGE_RESERVE    0x30            /* Reserved */
#define USB2_ISOC_ENDP_USAGE_ALL        0xFF            /* Match for all */
#define USB2_ATTR_EPTYPE_MASK           0x03
#define USB2_ATTR_CONTROL               0x00            /* Control Endpoint*/
#define USB2_ATTR_ISOCH                 0x01            /* Isoch Endpoint*/
#define USB2_ATTR_BULK                  0x02            /* Bulk Endpoint*/
#define USB2_ATTR_INTERRUPT             0x03            /* Interrupt Endpoint*/
#define USB2_ENDPOINT_DIR_MASK          0x80            /* Endpoint direction
                                                         * mask*/
#define USB2_ENDPOINT_OUT               0x00            /* OUT Endpoint */
#define USB2_ENDPOINT_IN                0x80            /* IN Endpoint */

/* Default timeout used by the class drivers (in ms) */

#define USB2_DEFAULT_TIMEOUT            5000

/* Default options for mutex creation */

#ifndef USB2_MUTEX_CREATION_OPTS
#define USB2_MUTEX_CREATION_OPTS        (SEM_Q_PRIORITY |       \
                                        SEM_INVERSION_SAFE |    \
                                        SEM_DELETE_SAFE)
#endif

/* Big/little endian convertors, needed for some class drivers */

#if (_BYTE_ORDER == _BIG_ENDIAN)
/* This is if the Host CPU is BIG ENDIAN */

#define USB2_FROM_LITTLEW(w)            (MSB((w)) | (LSB((w)) << 8))
#define USB2_FROM_LITTLEL(l)            (LONGSWAP((l)))
#define USB2_FROM_BIGW(w)               (w)
#define USB2_FROM_BIGL(l)               (l)
#else
/* This is if the Host CPU is Little ENDIAN */

#define USB2_FROM_LITTLEW(w)            (w)
#define USB2_FROM_LITTLEL(l)            (l)
#define USB2_FROM_BIGW(w)               (MSB((w)) | (LSB((w)) << 8))
#define USB2_FROM_BIGL(l)               (LONGSWAP((l)))
#endif

#define USB2_TO_LITTLEW(w)              USB2_FROM_LITTLEW((w))
#define USB2_TO_LITTLEL(l)              USB2_FROM_LITTLEL((l))
#define USB2_TO_BIGW(w)                 USB2_FROM_BIGW((w))
#define USB2_TO_BIGL(w)                 USB2_FROM_BIGL((l))

/* This is to define USB host class driver message code */

#define GEN2_DEVREADY                   0x1
#define GEN2_DEVREMOVE                  0x2
#define GEN2_DEVATTACH                  0x3
#define GEN2_DEVDETACH                  0x4
#define GEN2_LUNMOUNTED                 0x10
#define GEN2_LUNUNMOUNT                 0x11
#define GEN2_ENDBIND                    0x20
#define GEN2_ENDUNBIND                  0x21

/* Class code and USBMSG_HST() is defined in usbHst.h */

#define USBMSG_GEN2_KBD_DEVREADY    USBMSG_HST(USBMSG_HST_KBD, GEN2_DEVREADY)
#define USBMSG_GEN2_MSE_DEVREADY    USBMSG_HST(USBMSG_HST_MSE, GEN2_DEVREADY)
#define USBMSG_GEN2_TS_DEVREADY     USBMSG_HST(USBMSG_HST_TS,  GEN2_DEVREADY)
#define USBMSG_GEN2_PRN_DEVREADY    USBMSG_HST(USBMSG_HST_PRN, GEN2_DEVREADY)
#define USBMSG_GEN2_SER_DEVREADY    USBMSG_HST(USBMSG_HST_SER, GEN2_DEVREADY)
#define USBMSG_GEN2_PHDC_DEVREADY   USBMSG_HST(USBMSG_HST_PHDC, GEN2_DEVREADY)

#define USBMSG_GEN2_MSE_DEVREMOVED  USBMSG_HST(USBMSG_HST_MSE, GEN2_DEVREMOVE)
#define USBMSG_GEN2_TS_DEVREMOVED   USBMSG_HST(USBMSG_HST_TS,  GEN2_DEVREMOVE)
#define USBMSG_GEN2_KBD_DEVREMOVED  USBMSG_HST(USBMSG_HST_KBD, GEN2_DEVREMOVE)
#define USBMSG_GEN2_PRN_DEVREMOVED  USBMSG_HST(USBMSG_HST_PRN, GEN2_DEVREMOVE)
#define USBMSG_GEN2_SER_DEVREMOVED  USBMSG_HST(USBMSG_HST_SER, GEN2_DEVREMOVE)
#define USBMSG_GEN2_PHDC_DEVREMOVED USBMSG_HST(USBMSG_HST_PHDC, GEN2_DEVREMOVE)

#define USBMSG_GEN2_MSC_ATTACH      USBMSG_HST(USBMSG_HST_MSC, GEN2_DEVATTACH)
#define USBMSG_GEN2_END_ATTACH      USBMSG_HST(USBMSG_HST_END, GEN2_DEVATTACH)

#define USBMSG_GEN2_MSC_DETACH      USBMSG_HST(USBMSG_HST_MSC, GEN2_DEVDETACH)
#define USBMSG_GEN2_END_DETACH      USBMSG_HST(USBMSG_HST_END, GEN2_DEVDETACH)

#define USBMSG_GEN2_MSC_LUNMOUNTED  USBMSG_HST(USBMSG_HST_MSC, GEN2_LUNMOUNTED)
#define USBMSG_GEN2_MSC_LUNUNMOUNT  USBMSG_HST(USBMSG_HST_MSC, GEN2_LUNUNMOUNT)

#define USBMSG_GEN2_END_BIND        USBMSG_HST(USBMSG_HST_END, GEN2_ENDBIND)
#define USBMSG_GEN2_END_UNBIND      USBMSG_HST(USBMSG_HST_END, GEN2_ENDUNBIND)

/* typedefs */

typedef struct usb2_class_driver
    {
    USBHST_DEVICE_DRIVER        hstDriver;           /* Pointer to struct
                                                      * that registers to the
                                                      * USBD host stack
                                                      */

    SEM_ID                      hDriverMutex;        /* Driver Mutex */

    LIST *                      pDeviceList;         /* Device list */

    char *                      pDefaultDriverName;  /* Default driver
                                                      * basename
                                                      */
    } USB2_CLASS_DRIVER;

/* forward stuct declaration */

struct usb2_class_node;

typedef struct usb2_class_device
    {
    struct usb2_class_node *    pNode;                /* Node info for the lists
                                                       * that holds this device
                                                       */

    USB2_CLASS_DRIVER *         pDriver;              /* pointer to driver for
                                                       *  which uses this device
                                                       */

    SEM_ID                      hDeviceMutex;         /* Device Mutex */

    UINT32                      hDevice;              /* Device handler used by
                                                       * the usbd
                                                       */

    UINT8                       bConfigurationIndex;  /* ConfigurationIndex
                                                       * set by this device
                                                       * instance
                                                       */

    UINT8                       bInterfaceNumber;     /* Interface number
                                                       * set by this device
                                                       * instance
                                                       */

    UINT8                       bAlternateSetting;    /* Alternate Setting
                                                       * set by this device
                                                       * instance
                                                       */

    USBHST_SETUP_PACKET *       pSetupPacket;         /* Setup packet that can
                                                       * be used by this device
                                                       * instance on the control
                                                       * pipe
                                                       */

    USBHST_URB *                pClassSpecificUrb;    /* Class Specific or
                                                       * Vendor Specific URB
                                                       * buffer that can be
                                                       * used by this device
                                                       * instance
                                                       */

    SEM_ID                      hClassSpecificSem;    /* Signaling semaphore
                                                       * That can be used
                                                       * by this device
                                                       * instance
                                                       */

    SEM_ID                      hSetupMutex;          /* Protect possible
                                                       * concurrent access
                                                       * with other pipes
                                                       */

    BOOL                        bDeviceRemoving;      /* Flag to set/check
                                                       * to see if the device
                                                       * is beeing removed
                                                       * avoids unecessary IO
                                                       */

    UINT32                      uDeviceNameMaxSize;   /* max size of the name
                                                       * buffer
                                                       */

    char *                      pDeviceName;          /* Device name buffer
                                                       * may be used for the
                                                       * IO device naming
                                                       */
    } USB2_CLASS_DEVICE;

typedef struct usb2_class_node
    {
    NODE                        node;                 /* node will be used
                                                       * for the linked lists
                                                       */

    USB2_CLASS_DEVICE *         pClassDevice;         /* class device generic
                                                       * info
                                                       */

    UINT32                      uDeviceIndex;         /* Device index
                                                       * in the list
                                                       */
    } USB2_CLASS_NODE;

/* File descriptor structure created when opening a file descriptor */

typedef struct usb2_file_desc
    {
    UINT32  fdType;      /* type of access (READ / WRITE / RDWR) */
    void *  pDevice;     /* Device that is used by this filedescriptor */
    } USB2_FILE_DESC;

/* forward declarations */

extern USBHST_INTERFACE_DESCRIPTOR * usb2FindInterface
    (
    pUSBD_DEVICE_INFO   pDeviceInfo,        /* Device Information      */
    UINT16              uInterfaceNum,      /* Interface number        */
    UINT16              uAltIndex           /* Alternate setting index */
    );

extern USBHST_ENDPOINT_DESCRIPTOR * usb2FindEndpoint
    (
    UINT8 *     pInterfaceStart,            /* Ptr to interface buffer */
    UINT8       uTransferType,              /* Transfer type           */
    UINT8       uTransferDir,               /* Transfer direction      */
    UINT8       uSyncType,                  /* Synchronisation Type (Iso Mode) */
    UINT8       uUsageType,                 /* Usage Type (Iso Mode) */
    UINT8       uFirstEndpointTypeIndex     /* Endpoint index in the interface */
    );

extern VOID * usb2FindEndpointHelperDescr
    (
    UINT8 * pEndpointStart,             /* Ptr to endpoint buffer in config */
    UINT8   uHelperDescrType            /* Heler Descriptor type           */
    );

extern USB2_CLASS_DEVICE * usb2ClassDeviceCreate
    (
    USB2_CLASS_DRIVER * pDriver,
    UINT32              hDevice,
    UINT8               configurationIndex,
    UINT8               bInterfaceNumber,
    UINT8               uAlternateSetting
    );

extern void usb2ClassDeviceDelete
    (
    USB2_CLASS_DEVICE *  pUsb2ClassDevice
    );

extern USB2_CLASS_DRIVER * usb2ClassDriverCreate
    (
    char * pDriverDefaultName
    );

extern void usb2ClassDriverDelete
    (
    USB2_CLASS_DRIVER * pDriver
    );

extern int usb2ClassDeviceIsValid
    (
    USB2_CLASS_DRIVER * pDriver,
    USB2_CLASS_DEVICE * pClassDevice
    );

extern BOOL usb2DriverIsEmpty
    (
    USB2_CLASS_DRIVER * pDriver
    );

extern STATUS usb2TakeDriver
    (
    USB2_CLASS_DRIVER * pDriver
    );

extern STATUS usb2ReleaseDriver
    (
    USB2_CLASS_DRIVER * pDriver
    );

extern STATUS usb2TakeDevice
    (
    USB2_CLASS_DEVICE * pClassDevice
    );

extern STATUS usb2ReleaseDevice
    (
    USB2_CLASS_DEVICE * pClassDevice
    );

extern USBHST_STATUS usb2VendorClassSpecific
    (
    USB2_CLASS_DEVICE * pClassDevice,   /* USB2_CLASS_DEVICE structure */
    UINT8               requestType,    /* bmrequestType parameter as per spec*/
    UINT8               request,        /* brequest parameter as per spec */
    UINT16              value,          /* value as per spec*/
    UINT16              index,          /* index value as per spec*/
    UINT16 *            pActLen,        /* n bytes supposed to send */
    pUINT8              pBfr,           /* Buffer to store / send data to */
    int                 timeoutMs       /* Timeout in ms, or WAIT_FOREVER */
    );

extern void usb2ClassDeviceAddCallbackSet
    (
    FUNCPTR callback
    );

extern void usb2ClassDeviceDelCallbackSet
    (
    FUNCPTR callback
    );

extern STATUS usb2DeviceDeleteMsgPost
    (
    USB2_CLASS_DEVICE*  pClassDevice,           /* Usb2 Device structure */
    FUNCPTR             pCallback,              /* Delete Callback function */
    void *              pDevice                 /* Device to delete */
    );

extern STATUS usb2DeviceDeleteHelperInit
    (
    int maxParallelDevices,  /* Maximum amount of devices in parallel handle */
    int helperTaskPrio,      /* Delete helper Task priority */
    int helperTaskLatencySec /* Latency of task if one delete did not succeed */
    );

extern void usb2DeviceDeleteHelperDeinit
    (
    void
    );

/*******************************************************************************
 * Function Name    : USBD_TRANSLATE_DEVICE_HANDLE
 * Description      : This function is used to translate device handle
 *                    to USB device information.       .
 * Parameters       : uHandle        IN  Device handle to be translated.
 *                    pDeviceInfo    OUT Pointer to hold the translated device
 *                                       information.
 * Return Type      : None
 ******************************************************************************/
extern void usbdTranslateDeviceHandle
          (
          UINT32             uHandle,
          pUSBD_DEVICE_INFO  *pDeviceInfo
          );

#ifdef __cplusplus
}
#endif
#endif  /* __INCusb2Helperh */

