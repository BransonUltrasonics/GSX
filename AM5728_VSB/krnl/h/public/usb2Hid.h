/* usb2Hid.h - USB HID Specific headerfile */

/*
 * Copyright (c) 2008-2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,29jul13,j_x  Add report descriptor getting support 
01d,29apr10,pad  Moved extern C statement after include statements.
01c,27apr10,j_x  Changed for usbGen2Show (WIND00184395)
01b,17apr09,w_x  usb2HidDescriptorGet API change
01a,05Nov08,adh  written

*/
#ifndef __INCusb2Hidh
#define __INCusb2Hidh

/* includes */
#include <vxWorks.h>
#include <usb2Helper.h>
#include "usbhid.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Defines*/
#define USB2_HID_CLASS                          0x03 /* HID class (spec)  */
#define USBHST_HID_DESC_TYPE                    0x21 /* HID descriptor type*/

/* IOCTL codes (see module definition) */
#define USB2_HID_SET_IDLE                       0x0a
#define USB2_HID_GET_IDLE                       0x02
#define USB2_HID_IDLE_MIN                       0x0100
#define USB2_HID_IDLE_INFINITE                  0x0000
#define USB2_HID_IDLE_1020                      0xFF00
#define USB2_HID_IDLE_MIN_MS                    4
#define USB2_HID_IDLE_MAX_MS                    1020
#define USB2_HID_IDLE_INFINITE_MS               0xffffffff
#define USB2_HID_IDLE_STEP_MS                   4



#define USB2_HID_GET_PROTO                      0x03
#define USB2_HID_SET_PROTO                      0x0b
#define USB2_HID_PROTO_BOOT                     0x00
#define USB2_HID_PROTO_REPORT                   0x01

#define USB2_HID_SET_REPORT                     0x9
#define USB2_HID_GET_REPORT                     0x1
#define USB2_HID_REPORT_INPUT_TYPE              0x0100
#define USB2_HID_REPORT_OUTPUT_TYPE             0x0200
#define USB2_HID_REPORT_FEATURE_TYPE            0x0300

#define USB2_IOCTL_HID_IDLE_TIME_GET            0x7001
#define USB2_IOCTL_HID_IDLE_TIME_SET            0x7002
#define USB2_IOCTL_HID_PROTO_GET                0x7003
#define USB2_IOCTL_HID_PROTO_SET                0x7004
#define USB2_IOCTL_HID_REPORT_GET               0x7005
#define USB2_IOCTL_HID_REPORT_SET               0x7006

/* typedefs */
typedef struct usb2_hid_idle_time
    {
        UCHAR duration;
        UCHAR reportID;
    } __attribute__ ((packed)) USB2_HID_IDLE_TIME;

/* Structure to hold the HID Descriptor */
typedef struct usbhst_hid_descriptor
    {
    UINT8  bLength;              /* Descriptor length in bytes                */
    UINT8  bDescriptorType;      /* HID Descriptor type                       */
    UINT16 bcdHID;               /* HID Class Specification release           */
    UINT8  bCountryCode;         /* country code                              */
    UINT8  bNumDescriptors;      /* number of class descriptors               */
    UINT8  bClsDescriptorType;   /* type of class descriptor                  */
    UINT16 wDescriptorLength;    /* total size of the Report descriptor       */
    } __attribute__((packed)) USBHST_HID_DESCRIPTOR;

/* forward declarations */

extern USBHST_STATUS usb2HidIdleTimeSet
    (
    USB2_CLASS_DEVICE * pDevice,        /* usb2 class device */
    UINT16              idleTime,       /* idle time as of HID spec */
    int                 timeout         /* time to wait in ms before timeout */
    );

extern USBHST_STATUS usb2HidIdleTimeGet
    (
    USB2_CLASS_DEVICE * pDevice,        /* usb2 class device */
    UINT8 *             pIdleTime,      /* ptr to idle time returned */
    int                 timeout         /* time to wait in ms before timeout */
    );

extern USBHST_STATUS usb2HidProtoSet
    (
    USB2_CLASS_DEVICE * pDevice,        /* usb2 class device */
    UCHAR               value,          /* protocol value as of HID spec */
    int                 timeout         /* time in ms before timeout error */
    );

extern USBHST_STATUS usb2HidReportGet
    (
    USB2_CLASS_DEVICE * pDevice,        /* usb2 class device */
    UINT16              reportType,     /* report ID AND TYPE */
    pUINT8              pReport,        /* report value */
    UINT16 *            pActLen,        /* length of report */
    int                 timeout         /* time in ms before timeout error */
    );

extern USBHST_STATUS usb2HidReportSet
    (
    USB2_CLASS_DEVICE * pDevice,        /* usb2 class device */
    UINT16              reportType,     /* report ID AND TYPE */
    pUINT8              pReport,        /* report value */
    UINT16 *            pActLen,        /* length of report */
    int                 timeout         /* time in ms before timeout error */
    );

extern USBHST_STATUS usb2HidProtoGet
    (
    USB2_CLASS_DEVICE * pDevice,        /* usb2 class device */
    UINT8 *             pProtoValue,    /* pointer to protocol value */
    int                 timeout         /* time in ms before timeout error */
    );
extern USBHST_HID_DESCRIPTOR * usb2HidDescriptorGet
    (
    pUSBD_DEVICE_INFO   pDeviceInfo,    /* Device Information      */
    UINT16              uInterfaceNum,  /* Interface number        */
    UINT16              uAltIndex       /* Alternate setting index */
    );

extern UINT16 usb2HidCalculateIdleTimeFromMs
    (
    UINT32 idleTimeMs
    );

extern STATUS usb2HIDReportDescDump 
    (    
    unsigned char * pBuffer, 
    unsigned int size
    );


#ifdef __cplusplus
}
#endif
#endif  /* __INCusb2Hidh */
