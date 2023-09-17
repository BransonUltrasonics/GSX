/* usbd.h - USBD functional interface definition */

/*
 * Copyright (c) 2003, 2006, 2008-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003, 2006, 2008-2015 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
10dec15,whu  Protecte Usb control pipe against concurrent access triggers
             data abort. (V7CON-341)
05sep14,bbz  cleanup warnings of no check of the return value(V7CON-184)
28may14,wyy  Add deviceInfoMutex to protect class driver from multi-removing
             (WIND00446805)
16jan14,s_z  remove compiler warning
04nov13,s_z  make USB work both on new VxBus model and VxBus legacy model
04jul13,ljg  Change VOID parameter to void for compiler issue (WIND00423631)
28apr13,ghs  Add support of USB message (WIND00411952)
07nov12,s_z  Remove compiler CHECKED_RETURN warning (WIND00378821)
17oct12,w_x  Fix compiler warnings (WIND00370525)
19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
06jul11,ghs  Add new match function for device ID matching (WIND00183500)
13jan11,w_x  Initial USB OTG support changes (WIND00211541)
08sep10,ghs  Add MHCD and SYNOPSYSHCD count to the max hcd count.
21jul10,m_y  Add macro USBD_CLEANUP_ENDPOINT_INFO to do the clean action of
             the structure usbd_endpoint_info
04mar10,m_y  Add urbList into structure usbd_endpoint_info to record the URBs
             submitted to this endpoint.(WIND00200272)
11may10,m_y  Modify macro USBD_DECREMENT_FUNCTIONAL_DEVICE_COUNT,
             change OSS_ASSERT to "if" judgement (WIND00196406)
29apr10,pad  Moved extern C statement after include statements.
27apr10,j_x  Hard code replacement (WIND00183874)
13jan10,ghs  vxWorks 6.9 LP64 adapting
11sep09,ghs  Redefine max bus and hcd counts(WIND00152418)
24apr09,w_x  Moved USBD_NOTIFY_ALL from usbd.c to usbd.h(WIND00163712)
12mar09,w_x  Moved LOCAL prototype from usbd.h to usbd.c to remove warning
             (WIND00160883)
14feb09,w_x  Added root hub handle in USBD_BUS_INFO (WIND00152849)
13jan09,w_x  Added uParentPortIndex/uParentTier in device info (WIND00151472)
23dec08,w_x  Added uBCDUSB in device info structure (WIND00149529)
17dec08,w_x  Added "bAnnounced" flag in device/interface info (WIND00148490)
20oct08,w_x  Increase USBD_MAX_HCD_NUM and USBD_MAX_BUS_NUM
14aug08,w_x  Add mutex to sync up pipe delete/re-create (WIND00130877)
11jul08,w_x  USB Composite device support under vxBus (WIND00127515)
07oct06,ami  Changes for USB-vxBus porting
30may03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This file contains the data structures for usbd module.
*/

/*
INTERNAL
 *******************************************************************************
 * Filename         : Usbd.h
 *
 * Copyright        :
 *
 * THE COPYRIGHT IN THE CONTENTS OF THIS SOFTWARE VEST WITH WIPRO
 * LIMITED A COMPANY INCORPORATED UNDER THE LAWS OF INDIA AND HAVING
 * ITS REGISTERED OFFICE AT DODDAKANNELLI SARJAPUR ROAD  BANGALORE
 * 560 035. DISTRIBUTION OR COPYING OF THIS SOFTWARE BY
 * ANY INDIVIDUAL OR ENTITY OTHER THAN THE ADDRESSEE IS STRICTLY
 * PROHIBITED AND MAY INCUR LEGAL LIABILITY. IF YOU ARE NOT THE
 * ADDRESSEE PLEASE NOTIFY US IMMEDIATELY BY PHONE OR BY RETURN EMAIL.
 * THE ADDRESSEE IS ADVISED TO MAINTAIN THE PROPRIETARY INTERESTS OF
 * THIS COPYRIGHT AS PER APPLICABLE LAWS.
 *
 * Description      : This file contains the data structures for usbd module.
 *
 ******************************************************************************/

#ifndef __USBD_H__
#define __USBD_H__

/* Include Files */
#include <usbOsal.h>
#include <usbHst.h>
#include <objLib.h>
#include <usbHcdInstr.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define USBD_MAX_HCD_NUM  ((USB_MAX_UHCI_COUNT) + (USB_MAX_OHCI_COUNT) \
                          + (USB_MAX_EHCI_COUNT) + (USB_MAX_MHCI_COUNT) \
                          + (USB_MAX_SYNOPSYSHCI_COUNT)) /* the max
                                                          * host controllers
                                                          */
#define USBD_MAX_BUS_NUM  ((USBD_MAX_HCD_NUM) * 2) /* the max bus in all HCDs */

#define USBD_ADDRESS_MAP_SIZE  (128/(8*sizeof(UINT8))) /* max # of devices */
#define USBD_INVALID_CRC_VALUE    0  /* invalid CRC value for configuration */
#define USBD_PARSE_AND_CREATE  0x01  /* used as flag to parse and create pipe */
#define USBD_PARSE_ONLY        0x02  /* used as flag to parse only */
#define CRC16_MASK           0xFFFF  /* CRC16 info */
#define CRC16_INITIAL_HOLD   0xFFFF  /* CRC16 info */
#define CRC16_GENERATOR_POLY 0x8005  /* 1000 0000 0000 0101b */

#define USBD_NOTIFY_ALL      0xFFFF  /* Device/Class match flag wildcard */

/* Defines the maximum device address */
#define USBD_MAX_DEVICE_ADDRESS                127

/* Defines the offset of bux index in device or hub handle */
#define USBD_OFFSET_BUS_INDEX                  8

/* Defines the offset of invalid byte in device handle */
#define USBD_OFFSET_INVALID                    16

/* Defines the bit position within a byte */
#define USBD_BIT_POSITION                      1

/* Defines the byte and bit position in the device address map */
#define USBD_POSITION_ADDRESS_MAP              8

/* Defines the transfer type bit map in the bmAttributes */
#define USBD_TRANSFER_TYPE                     0x03

/* Defines the max packet size is 8 */
#define USBD_MAX_PACKET_SIZE_8                 8

/* Defines the max packet size is 16 */
#define USBD_MAX_PACKET_SIZE_16                16

/* Defines the max packet size is 32 */
#define USBD_MAX_PACKET_SIZE_32                32

/* Defines the max packet size is 64 */
#define USBD_MAX_PACKET_SIZE_64                64

/* Defines the max packet size is 512 */
#define USBD_MAX_PACKET_SIZE_512               512

/* Defines the max packet size is 1023 */
#define USBD_MAX_PACKET_SIZE_1023              1023

/* Defines the max packet size is 1024 */
#define USBD_MAX_PACKET_SIZE_1024              1024

/* Defines the range of bInterval */

/* For full-/high-speed isochronous endpoints,
 * this value must be in the range from 1 to 16
 */

#define USBD_MIN_BINTERVAL                    1
#define USBD_MAX_BINTERVAL                    16

/* Defines the length of endpoint descriptor */
#define USBD_LENGTH_END_DESC                  7

/* Defines the length of device descriptor */
#define USBD_LENGTH_DEV_DESC                  18

/* Defines the length of config descriptor */
#define USBD_LENGTH_CONFIG_DESC               9

/* Defines the USB2.0 SPEC version */
#define USBD_USB_SPEC_20                      0x0200

/* Defines the 16-bit reversed value for the hold value */
#define USBD_HOLD_VALUE_16                    16

/* Defines the byte reversed value for the hold value */
#define USBD_HOLD_VALUE_8                     8

/* Defines transfer direction of bmRequestType */
#define USBD_DEVICE_TO_HOST                   0x80
#define USBD_HOST_TO_DEVICE                   0x00

/* Defines the offset of descriptor type in wValue */
#define USBD_OFFSET_DES_TYPE                  8

/* Defines the request status size is 2 */
#define USBD_STATUS_SIZE_2                    2

/* Defines the request status size is 1 */
#define USBD_STATUS_SIZE_1                    1

/* Defines the request status size is 0 */
#define USBD_STATUS_SIZE_0                    0

/***************************************************************************
*
* USBD_INCREMENT_FUNCTIONAL_DEVICE_COUNT - Increment the functional
* device count
*
* This Macro is used to increment a functional device count on the
* specified bus <uBusIndex>.
*
* RETURNS: N/A
*/
#define USBD_INCREMENT_FUNCTIONAL_DEVICE_COUNT(uBusIndex)                   \
    {                                                                       \
    /* Lock the bus before modifying functional count */                    \
    (void)OS_WAIT_FOR_EVENT((gUSBBusInfoList[(uBusIndex)].busOperationLock),\
                       OS_WAIT_INFINITE);                                   \
                                                                            \
    /* Increment the functional count of the bus */                         \
    gUSBBusInfoList[(uBusIndex)].uFunctionalCount++;                        \
                                                                            \
    /* Release the lock */                                                  \
    (void)OS_RELEASE_EVENT(gUSBBusInfoList[(uBusIndex)].busOperationLock);        \
    }

/***************************************************************************
*
* USBD_DECREMENT_FUNCTIONAL_DEVICE_COUNT - decrement a functional device
* count
*
* This Macro is used to decrement a functional device count on the
* specified bus <uBusIndex>.
*
* RETURNS: N/A
*/
#define USBD_DECREMENT_FUNCTIONAL_DEVICE_COUNT(uBusIndex)                   \
    {                                                                       \
    /* Lock the bus before modifying functional count */                    \
    (void)OS_WAIT_FOR_EVENT((gUSBBusInfoList[(uBusIndex)].busOperationLock),\
                      OS_WAIT_INFINITE);                                    \
    /* Just to check the count is not zero */                               \
    if (gUSBBusInfoList[(uBusIndex)].uFunctionalCount != 0)                 \
    /* Decrement the functional count of the bus */                         \
    gUSBBusInfoList[(uBusIndex)].uFunctionalCount--;                        \
                                                                            \
    /* Release the lock */                                                  \
    (void)OS_RELEASE_EVENT((gUSBBusInfoList[(uBusIndex)].busOperationLock));      \
    }

/***************************************************************************
*
* USBD_CLEANUP_ENDPOINT_INFO - do clean action of the USBD_ENDPOINT_INFO
*
* This Macro is used to do clean action of the USBD_ENDPOINT_INFO.
*
* RETURNS: N/A
*/

#define USBD_CLEANUP_ENDPOINT_INFO(pEndpointInfo)               \
    {                                                           \
    NODE *pNode = NULL;                                         \
    NODE *pNextNode = NULL;                                     \
    (void)semTake(&(pEndpointInfo->urbListSem),WAIT_FOREVER);   \
    pNode = lstFirst(&(pEndpointInfo->urbList));                \
    while(pNode != NULL)                                        \
        {                                                       \
        pNextNode = lstNext(pNode);                             \
        lstDelete(&(pEndpointInfo->urbList), pNode);            \
        pNode = pNextNode;                                      \
        }                                                       \
    semTerminate(&(pEndpointInfo->urbListSem));                 \
    }

/* Macro definition of USB message code */
#define USBMSG_CODE(SysOrUser, ModuleType, Module, Msg)     \
    (((((UINT32)SysOrUser) & 0xFF) << 24) | ((((UINT32)ModuleType) & 0xF) << 20) \
    | ((((UINT32)Module) & 0xF) << 16) | ((((UINT32)Msg) & 0xFFFF) << 0))

/*
 * Bit Table of Message Code
 *
 * All 0 or All 1 is reserved for any sub values
 *
 * [31-24]  USB System Message or User Message
 *          [0x00]~[0x20] - Unused, Reserved
 *          [0x21]        - USB Message
 *          [0x22]~[0x80] - Reserved
 *          [0x81]        - User Message
 *          [0x82]~[0xFF] - Reserved
 *
 * [23-20]  Module Type (for USB only)
 *          [0x0] - Unused, Reserved
 *          [0x1] - HCD Message
 *          [0x2] - TCD Message
 *          [0x3] - OTG Message
 *          [0x4] - USBD Message
 *          [0x5] - Host Class Driver Message
 *          [0x6] - Target Function Driver Message
 *          [0x7] - Tools and Other Type Message
 *          [0x8~0xF] - Unused, Reserved
 *
 * [19-16]  Modules
 *          Defined by every modules, value [0x0] is reserved.
 *          For GEN2 Host Class Driver Define:
 *              1 - legacy (GEN1 Reserved) 2 - Hub, 3 - Helper, 4- Keyboard,
 *              5 - Mouse, 6 - Storage, 7 - Network, 8 - Printer, 9 - Serial,
 *              10 - PHDC
 *
 * [15-0]   Message Values
 *          Defined by every modules, value [0x0] is reserved.
 *          For GEN2 Host Class Driver Define:
 *              1 - New Device Ready, 2 - Device Removed.
 *
 */

/* USB_MSG_CODE - SysOrUser */

#define USBMSG_USBSYS                               0x21
#define USBMSG_USBUSER                              0x81

/* USB_MSG_CODE - ModuleType */

#define USBMSG_HCD                                  0x1
#define USBMSG_TCD                                  0x2
#define USBMSG_OCD                                  0x3
#define USBMSG_USBD                                 0x4
#define USBMSG_CLASS                                0x5
#define USBMSG_FUNC                                 0x6
#define USBMSG_TOOL                                 0x7

/* flags value to add callback */
#define USBMSG_FLAG_CALLBACK_ASYN                   (1 << 0)

/********************** STRUCTURES DEFINITION *********************************/

/*
 * This structure is used to maintain the list of Class Drivers registered
 * with USBD
 */
typedef struct usbd_device_driver_list
    {
    pUSBHST_DEVICE_DRIVER             pDriver;      /* pointer to driver */
    struct usbd_device_driver_list *  pNextDriver;  /* pointer to next driver */
    } USBD_DEVICE_DRIVER_LIST, *pUSBD_DEVICE_DRIVER_LIST;

/* Structure to hold the endpoint information */
typedef struct usbd_endpoint_info
    {
    UINT8  uEndpointAddress; /* Address of the end Point */
    ULONG  hPipe;            /* Ptr to pipe corresponding to the endpoint */
    LIST   urbList;          /* List of the urb submited to this endpoint */
    SEMAPHORE urbListSem;    /* semphore of the urb list */
    } USBD_ENDPOINT_INFO, *pUSBD_ENDPOINT_INFO;

/*
 * This structure is used to store the information about an interface required
 * by a USB device.
 */
typedef struct usbd_interface_info
    {
    UINT8      uInterfaceNum;      /* Interface number */
    UINT8      uAlternateSetting;  /* Alt settting value for this interface */
    UINT8      uNumEndpoints;      /* # of end points in the interface */
    UINT8      uInterfaceClass;    /* Class code for the interface */
    UINT8      uInterfaceSubClass; /* Sub class ocde for the interface */
    UINT8      uInterfaceProtocol; /* protocol code  for the interface */
    pUSBHST_DEVICE_DRIVER pDriver; /* Ptr to the DEVICE_DRIVER struct */
    void *     pDriverData;        /* Driver data specific to interface */
    pUSBD_ENDPOINT_INFO pEndpointInformation; /* Ptr to end point info */
    VXB_DEVICE_ID       pInterfaceDev;        /* Pointer to struct pDev */
    BOOLEAN             bAnnounced;/* Is the VXB_DEVICE_ID bAnnounced */
    OS_EVENT_ID         pipeLock;  /* Lock to protect pipe modification */
    } USBD_INTERFACE_INFO, *pUSBD_INTERFACE_INFO;

/* This structure is used to store the information about a USB device */
typedef struct usbd_device_info
    {
    UINT32    hDevice;             /* Handle to the device */
    UINT8     uDeviceSpeed;        /* Speed of the device */
    UINT8     uDeviceAddress;      /* Device addr on particular USB bus */
    UINT8     uBusIndex;           /* Index of the bus in global bus list */
    UINT16    uHighSpeedHubInfo;   /* Nearest high speed hub and port number
                                    * info; used for split transfer to full/low
                                    * speed devices. high byte holds high speed
                                    * address, low byte is port number
                                    */
    UINT8     uParentPortIndex;    /* The port index where this device is attached */
    UINT8     uParentTier;         /* The tier of the parent hub of this device */
    ULONG     hDefaultPipe;        /* Ptr to pipe for end point 0 */
    UINT16    uBCDUSB;             /* bcdUSB, USB version device conforms to */
    UINT8     uDeviceClass;        /* Class code of the device */
    UINT8     uDeviceSubClass;     /* Sub class code of the device */
    UINT8     uDeviceProtocol;     /* Protocol code of the device */
    UINT16    uVendorID;           /* Vendor Id of the device */
    UINT16    uDeviceID;           /* Device Id for device */
    UINT16    uBCDDevice;          /* USB device release version binary coded decimal */
    UINT8     uMaxPacketSize0;     /* Max Packet Size for default control endpoint */
    pUSBHST_DEVICE_DRIVER pDriver; /* Store the driver data */
    void *    pDriverData;         /* Driver data specific to interface */
    UINT8     uInterfaceNumber;    /* Interface # */
    UINT8     uInterfaceCount;     /* # of interfaces supported */
    UCHAR *   pBOS;                /* Ptr to Binary Device Object Store */
    UCHAR *   pCurrentConfiguration; /* Ptr to hold config of the device */
    UINT8     uNumConfigurations;    /* # of possible configurations */
    UINT16    uConfigCRCValue;       /* CRC Value */
    pUSBD_INTERFACE_INFO pInterfacesInfo; /* Info about interfaces */
    struct usbd_device_info *pNextUSBDeviceInfo; /* Ptr to next USB devinfo */
    VXB_DEVICE_ID       pDev;       /* Pointer to struct pDev */
#ifndef _WRS_CONFIG_VXBUS_LEGACY    
    VXB_DEVICE_ID       pParentDev; /* Pointer to parent struct pDev */
#endif    
    BOOLEAN   bAnnounced;           /* Is the VXB_DEVICE_ID bAnnounced */
    BOOLEAN   bIsComposite;         /* Is this a composite device */
    UCHAR *   pOtgDescr;            /* Ptr to OTG descriptor buffer */
    UINT16    uOtgDescrOffset;      /* Offset of OTG descr in config descr */
    MUTEX_HANDLE deviceInfoMutex;   /* protect deviceInfo from concurrent modification */ 
    MUTEX_HANDLE ctrlPipeMutex;     /* protect control pipe*/
    } USBD_DEVICE_INFO, *pUSBD_DEVICE_INFO;

#ifndef _WRS_CONFIG_VXBUS_LEGACY   
typedef struct usbd_func_info
    {
    void *               pFuncSpecData;
    VXB_DEVICE_ID        pDev; 
    pUSBD_DEVICE_INFO    pDeviceInfo;
    pUSBD_INTERFACE_INFO pFirstInterfaceInfo;  
    UINT32               uFlags;
    UINT8                uFirstInferface;
    UINT8                uInterfaceCount; 
    void *               pExtData;
    } USBD_FUNC_INFO, *pUSBD_FUNC_INFO;
#endif    
/* Since the root hub is a virtual device, we can use any non-zero addresses */

#define USB_ROOT_HUB_ADDRESS USBD_MAX_DEVICE_ADDRESS

/*
 * To store the information about a bus and also to maintain a list of USB
 * devices connected on the specific bus.
 */
typedef struct usbd_bus_info
    {
    UINT8  uDeviceAddressMap[USBD_ADDRESS_MAP_SIZE]; /*
                                                      * Device address map
                                                      * of devs on bus.
                                                      */
    UINT8   uRelativeBusIndex;  /* rellative bus index in the host controller */
    UINT8   hcdIndex;           /* Host controller driver index */
    ULONG   hDefaultPipe;       /* Handle to the default pipe */
    UINT32  uFunctionalCount;   /* Count of functional interfaces/devices */
    OS_EVENT_ID busOperationLock;  /* Lock used while accessing list */
    pUSBD_DEVICE_INFO   pUSBDeviceInfoList; /* info about devices connected */
    UINT32  hRootHub;           /* Root hub device handle */
    UINT8   uRootHubAddress;    /* Root hub device address */
    UINT8   uBusSpeed;          /* Root hub bus speed */
    VXB_DEVICE_ID       pDev;   /* Host Controller VxBus Instance */
    } USBD_BUS_INFO, *pUSBD_BUS_INFO;

typedef void *(* usbmsg_callback)(UINT32 msgCode, void * pModule,
                                  void * param, void * userData);

typedef usbmsg_callback USBMSG_CALLBACK;

/*******************************************************************************
 * Function Name    : USBD_TRANSLATE_DEVICE_HANDLE
 * Description      : This function is used to translate device handle
 *                    to USB device information.       .
 * Parameters       : uHandle        IN  Device handle to be translated.
 *                    pDeviceInfo    OUT Pointer to hold the translated device
 *                                       information.
 * Return Type      : None
 ******************************************************************************/

void usbdTranslateDeviceHandle
    (
    UINT32             uHandle,
    pUSBD_DEVICE_INFO  *pDeviceInfo
    );


pUSBHST_DEVICE_ID usbdMatchDriverId
    (
    pUSBHST_DEVICE_ID pDevId,
    pUSBHST_DEVICE_ID pDrvDevId
    );

void * usbMsgCallbackAdd
    (
    UINT32 msgCode,
    USBMSG_CALLBACK callback,
    void * userData,
    UINT32 flags
    );

STATUS usbMsgCallbackDel
    (
    void * handle
    );

void * usbMsgPost
    (
    UINT32 msgCode,
    void * pModule,
    void * param
    );

STATUS usbMsgLibInit
    (
    int maxCallback,
    int maxPendingMsg
    );

STATUS usbMsgAsynLibInit
    (
    void
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __USBD_H__ */
/**************************** End of file Usbd.h ******************************/

