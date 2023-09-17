/* usb.h - Basic USB (Universal Serial Bus) definitions */

/*
 * Copyright (c) 1999-2002, 2004-2005, 2010-2013, 2016-2017 
 * Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
12jan17,whu  Add USB_SPEED_UNKNOWN definition (vx7-US89247)
30may16,whu  Remove old clearcase versioning 
28apr16,whu  Add USB_CLASS_CDC definition (V7CON-389)
04nov13,s_z  Add USB_CLASS_VENDOR definition
09sep13,j_x  Rename the "LINK" structure to avoid namespace pollution
19oct12,w_x  Unify some USB3 hub macro names
03sep12,s_z  Fix BOS buffer using issue (WIND00374269)
18may12,s_z  Add support for USB 3.0 target (WIND00326012)
19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
30feb11,s_z  Add the definition of USB_CLASS_MISC, USB_SUBCLASS_COMMON
             and USB_PROTOCOL_IAD
             Add some endpoint descriptor attributes definition                 
23feb11,s_z  Add the "pContex" and "erpFlags" field to USB_ERP
             Add the defination of USB_ERP_FLAGS
             Add some description about "targPtr", "tcdPtr" and "usrPtr" 
             to use USB_ERP
23feb11,s_z  Add the defination of IAD descriptor structure 
17feb11,m_y  Add the defination for USB_ENDPOINT_STS_UNHALT
13jan11,w_x  Initial USB OTG support changes (WIND00211541)
28jun10,w_x  Avoid dynamical alloc/free for IRP context (WIND00216670)
29apr10,pad  Moved extern C statement after include statements.
03mar10,ghs  Fix LP64 adapting(WIND00201187)
13jan10,ghs  vxWorks 6.9 LP64 adapting
31jan05,pdg  Corrected USB_ERP structure
19jan05,h_k  added pragma pack to USB_DESCR_TYPLEN and USB_HID_DESCR and
             added USB_HID_DESCR_LEN. (SPR #104230)
08sep04,ami  New macros added
16jul04,???  Backward compatibility with the USB 1.1 Peripheral stack
03may04,???  Addition of macros for USB 2.0
02apr04,???  USB 2.0 modifications
28mar02,h_k  removed #if (CPU==SH7700)&&(_BYTE_ORDER==_BIG_ENDIAN) and set
             USB_MAX_DESCR_LEN 255 for SH7700 (SPR #73896).
16nov01,wef  Pack ENDPOINT_DESCRIPTOR structure.
08nov01,wef  add provisions for SH - pack structures and conditionally make
             USB_MAX_DESCR_LEN a multiple of 4
18sep01,wef  merge from wrs.tor2_0.usb1_1-f for veloce
25jul01,rcb  fixed spr #69287
26jan00,rcb  Add "dataBlockSize" field to USB_IRP to give application
             greater control over isochronous transfer sizes.
             Change USB_BFR_LIST "bfrLen" and "actLen" fields to UINT32.
12jan00,rcb  Add definition for USB_CLASS_AUDIO.
             Add typedef for USB_DESCR_HDR_SUBHDR.
07sep99,rcb  Add definition for USB_TIME_RESUME.
03jun99,rcb  First.
*/

/*
DESCRIPTION

This file defines constants and structures related to the USB (Universal
Serial Bus).

NOTE: The USB specification defines that multi-byte fields will be stored
in little-endian format.
*/

#ifndef __INCusbh
#define __INCusbh


/* includes */

#include "usbListLib.h"
#include "usbHandleLib.h"
#include "usbHst.h"

#ifdef  __cplusplus
extern "C" {
#endif


/* Defined by the USB */

/* defines */

#define USB_MAX_DEVICES         127

#define USB_MAX_TOPOLOGY_DEPTH  6

#define USB_MAX_DESCR_LEN       255

/* USB class codes */

#define USB_CLASS_AUDIO         0x01
#define USB_CLASS_CDC           0x02
#define USB_CLASS_HID           0x03
#define USB_CLASS_PRINTER       0x07
#define USB_CLASS_HUB           0x09
#define USB_CLASS_MISC          0xEF
#define USB_CLASS_VENDOR        0xFF

/* USB subclass code */

#define USB_SUBCLASS_COMMON     0x02

/* USB protocol code */

#define USB_PROTOCOL_IAD        0x01


/* USB requestType (bmRequestType) bit fields */

#define USB_RT_DIRECTION_MASK   0x80
#define USB_RT_HOST_TO_DEV      0x00
#define USB_RT_DEV_TO_HOST      0x80

#define USB_RT_CATEGORY_MASK    0x60
#define USB_RT_STANDARD         0x00
#define USB_RT_CLASS            0x20
#define USB_RT_VENDOR           0x40

#define USB_RT_RECIPIENT_MASK   0x03
#define USB_RT_DEVICE           0x00
#define USB_RT_INTERFACE        0x01
#define USB_RT_ENDPOINT         0x02
#define USB_RT_OTHER            0x03

/* USB requests (bRequest) */

#define USB_REQ_GET_STATUS          0
#define USB_REQ_CLEAR_FEATURE       1
#define USB_REQ_GET_STATE           2
#define USB_REQ_SET_FEATURE         3
#define USB_REQ_SET_ADDRESS         5
#define USB_REQ_GET_DESCRIPTOR      6
#define USB_REQ_SET_DESCRIPTOR      7
#define USB_REQ_GET_CONFIGURATION   8
#define USB_REQ_SET_CONFIGURATION   9
#define USB_REQ_GET_INTERFACE       10
#define USB_REQ_SET_INTERFACE       11
#define USB_REQ_GET_SYNCH_FRAME     12
#define USB_REQ_SET_SEL             48
#define USB_REQ_SET_ISOCH_DELAY     49

/* USB descriptor types */

#define USB_DESCR_DEVICE                    0x01
#define USB_DESCR_CONFIGURATION             0x02
#define USB_DESCR_STRING                    0x03
#define USB_DESCR_INTERFACE                 0x04
#define USB_DESCR_ENDPOINT                  0x05
#define USB_DESCR_DEVICE_QUALIFIER          0x06
#define USB_DESCR_OTHER_SPEED_CONFIGURATION 0x07
#define USB_DESCR_INTERFACE_POWER           0x08
#define USB_DESCR_OTG                       0x09
#define USB_DESCR_DEBUG                     0x0A
#define USB_DESCR_IAD                       0x0B
#define USB_DESCR_BOS                       0x0F
#define USB_DESCR_DEVICE_CAPABILITY         0x10
#define USB_DESCR_SS_ENDPOINT_COMPANION     0x30
#define USB_DESCR_HID                       0x21
#define USB_DESCR_REPORT                    0x22    /* HID report descriptor */
#define USB_DESCR_PHYSICAL                  0x23    /* HID physical descriptor */
#define USB_DESCR_UAS_PIPE_USAGE            0x24    /* UAS Pipe Usage Descriptor */
#define USB_DESCR_HUB                       0x29
#define USB_DESCR_USB3_HUB                  0x2A
#define USB_DESCR_CLASS_CS_INTERFACE        0x24
#define USB_DESCR_CLASS_CS_ENDPOINT         0x25
#define USB_DESCR_SUBCLASS_UNION            0x06

/* configuration descriptor attributes values */

#define USB_ATTR_SELF_POWERED               0x40
#define USB_ATTR_REMOTE_WAKEUP              0x20

/* Max power available from a hub port */

#define USB_POWER_SELF_POWERED              500     /* 500 mA */
#define USB_POWER_BUS_POWERED               100     /* 100 mA */

/* power requirements as reported in configuration descriptors */

#define USB_POWER_MA_PER_UNIT               2       /* 2 mA per unit */

/* endpoint descriptor attributes values */

#define USB_ATTR_EPTYPE_OFFSET              0x00
#define USB_ATTR_EPTYPE_MASK                0x03
#define USB_ATTR_CONTROL                    0x00
#define USB_ATTR_ISOCH                      0x01
#define USB_ATTR_BULK                       0x02
#define USB_ATTR_INTERRUPT                  0x03

/* Endpoint descriptor attributes synchronization types */

#define USB_ATTR_EP_SYNCH_TYPE_OFFSET       (0x02) 
#define USB_ATTR_EP_SYNCH_TYPE_MASK         (0x03) 
#define USB_ATTR_EP_SYNCH_TYPE_NOSYNCH      (0x00) 
#define USB_ATTR_EP_SYNCH_TYPE_ASYNCH       (0x01) 
#define USB_ATTR_EP_SYNCH_TYPE_ADAPTIVE     (0x02) 
#define USB_ATTR_EP_SYNCH_TYPE_SYNCH        (0x03) 

/* Endpoint descriptor attributes usage types */

#define USB_ATTR_EP_USAGE_TYPE_OFFSET       (0x04) 
#define USB_ATTR_EP_USAGE_TYPE_MASK         (0x03) 
#define USB_ATTR_EP_USAGE_TYPE_DATA         (0x00) 
#define USB_ATTR_EP_USAGE_TYPE_FEEDBACK     (0x01) 
#define USB_ATTR_EP_USAGE_TYPE_IMFEEDBACK   (0x02) 
#define USB_ATTR_EP_USAGE_TYPE_RESERVED     (0x03) 

/* Standard USB feature selectors */

#define USB_FSEL_DEV_ENDPOINT_HALT      0
#define USB_FSEL_DEV_REMOTE_WAKEUP      1
#define USB_FSEL_DEV_TEST_MODE          2
#define USB_FSEL_DEV_B_HNP_ENABLE       3
#define USB_FSEL_DEV_A_HNP_SUPPORT      4
#define USB_FSEL_DEV_A_ALT_HNP_SUPPORT  5
#define USB_FSEL_DEV_FUNCTION_SUSPEND   0
#define USB_FSEL_DEV_U1_ENABLE          48
#define USB_FSEL_DEV_U2_ENABLE          49
#define USB_FSEL_DEV_LTM_ENABLE         50
#define USB_FSEL_DEV_B3_NTF_HOST_REL    51
#define USB_FSEL_DEV_B3_RSP_ENABLE      52


/* Test selector values for the TEST_MODE feature */

#define USB_TEST_MODE_J_STATE               0x01    /* Test_J */
#define USB_TEST_MODE_K_STATE               0x02    /* Test_K */
#define USB_TEST_MODE_SE0_ACK               0x03    /* Test_SE0 */
#define USB_TEST_MODE_TEST_PACKET           0x04    /* Test Packet */
#define USB_TEST_MODE_TEST_FORCE_ENABLE     0x05    /* Test Force Enable */


/* Defines the bit mask of multiple TT */
#define USB_HUB_MASK_MULTIPLE_TT              0x80

/* Defines the bit mask of TT port number (mask off the MTT bit) */
#define USB_HUB_MASK_MULTIPLE_TT_PORT         0x7F

/* Hub feature selections */

#define USB_HUB_FSEL_C_HUB_LOCAL_POWER  0
#define USB_HUB_FSEL_C_HUB_OVER_CURRENT 1
#define USB_HUB_FSEL_PORT_CONNECTION    0
#define USB_HUB_FSEL_PORT_ENABLE        1
#define USB_HUB_FSEL_PORT_SUSPEND       2
#define USB_HUB_FSEL_PORT_OVER_CURRENT  3
#define USB_HUB_FSEL_PORT_RESET         4
#define USB_HUB_FSEL_PORT_POWER         8
#define USB_HUB_FSEL_PORT_LOW_SPEED     9
#define USB_HUB_FSEL_C_PORT_CONNECTION  16
#define USB_HUB_FSEL_C_PORT_ENABLE      17
#define USB_HUB_FSEL_C_PORT_SUSPEND     18
#define USB_HUB_FSEL_C_PORT_OVER_CURRENT 19
#define USB_HUB_FSEL_C_PORT_RESET       20

/* See USB3 Table 10-8. Hub Class Feature Selectors */
#define USB3_HUB_FSEL_PORT_LINK_STATE       5
#define USB3_HUB_FSEL_PORT_U1_TIMEOUT       23
#define USB3_HUB_FSEL_PORT_U2_TIMEOUT       24
#define USB3_HUB_FSEL_C_PORT_LINK_STATE     25
#define USB3_HUB_FSEL_C_PORT_CONFIG_ERROR   26
#define USB3_HUB_FSEL_PORT_REMOTE_WAKE_MASK 27
#define USB3_HUB_FSEL_BH_PORT_RESET         28
#define USB3_HUB_FSEL_C_BH_PORT_RESET       29
#define USB3_HUB_FSEL_FORCE_LINKPM_ACCEPT   30

/* USB Hub Class Specific Requests */

#define USB_HUB_REQ_GET_STATUS          0
#define USB_HUB_REQ_CLEAR_FEATURE       1
#define USB_HUB_REQ_GET_STATE           2
#define USB_HUB_REQ_SET_FEATURE         3
#define USB_HUB_REQ_GET_DESCRIPTOR      6
#define USB_HUB_REQ_SET_DESCRIPTOR      7
#define USB_HUB_REQ_CLEAR_TT_BUFFER     8
#define USB_HUB_REQ_RESET_TT            9
#define USB_HUB_REQ_GET_TT_STATE        10
#define USB_HUB_REQ_STOP_TT             11
#define USB3_HUB_REQ_SET_HUB_DEPTH      12
#define USB3_HUB_REQ_GET_PORT_ERR_COUNT 13

/* endpoint direction mask */

#define USB_ENDPOINT_DIR_MASK           0x80
#define USB_ENDPOINT_OUT                0x00
#define USB_ENDPOINT_IN                 0x80

/* endpoint max transfer size related mask */

#define USB_ENDPOINT_MAX_PACKET_SIZE_MASK    (0x7FF)
#define USB_ENDPOINT_NUM_TRANSACTIONS_MASK   (0x1800)
#define USB_ENDPOINT_NUM_TRANSACTIONS_OFFSET (11)

/* standard endpoints */

#define USB_MAX_ENDPOINT_NUM            15
#define USB_ENDPOINT_MASK               0xf

/* Max packet size related macros */
#define USB_MAX_PACKET_SIZE_MASK                0x7ff
#define USB_NUMBER_OF_TRANSACTIONS_MASK         0x1800
#define USB_NUMBER_OF_TRANSACTIONS_BITPOSITION  11

#define USB_ENDPOINT_DEFAULT_CONTROL        0

/* hub characteristics */

#define USB_HUB_GANGED_POWER                0x0000
#define USB_HUB_INDIVIDUAL_POWER            0x0001

#define USB_HUB_NOT_COMPOUND                0x0000
#define USB_HUB_COMPOUND                    0x0004

#define USB_HUB_GLOBAL_OVERCURRENT          0x0000
#define USB_HUB_INDIVIDUAL_OVERCURRENT      0x0008

/* standard device status */

#define USB_DEV_STS_LOCAL_POWER             0x0001
#define USB_DEV_STS_REMOTE_WAKEUP           0x0002
#define USB_DEV_STS_U1_ENABLE               0x0004  
#define USB_DEV_STS_U2_ENABLE               0x0008  
#define USB_DEV_STS_LMT_ENABLE              0x0010  

/* standard endpoint status */

#define USB_ENDPOINT_STS_HALT               0x0001
#define USB_ENDPOINT_STS_UNHALT             0x0000

/* hub status & change */

#define USB_HUB_STS_LOCAL_POWER         0x0001
#define USB_HUB_STS_OVER_CURRENT        0x0002

#define USB_HUB_C_LOCAL_POWER           0x0001
#define USB_HUB_C_OVER_CURRENT          0x0002

/* hub port status & change */

#define USB_HUB_STS_PORT_CONNECTION     0x0001
#define USB_HUB_STS_PORT_ENABLE         0x0002
#define USB_HUB_STS_PORT_SUSPEND        0x0004
#define USB_HUB_STS_PORT_OVER_CURRENT   0x0008
#define USB_HUB_STS_PORT_RESET          0x0010
#define USB_HUB_STS_PORT_POWER          0x0100
#define USB_HUB_STS_PORT_LOW_SPEED      0x0200
#define USB_HUB_STS_PORT_HIGH_SPEED     0x0400
#define USB_HUB_STS_PORT_STATUS_TEST    0x0800
#define USB_HUB_STS_PORT_INDICATOR      0x1000

#define USB_HUB_C_PORT_CONNECTION       0x0001
#define USB_HUB_C_PORT_ENABLE           0x0002
#define USB_HUB_C_PORT_SUSPEND          0x0004
#define USB_HUB_C_PORT_OVER_CURRENT     0x0008
#define USB_HUB_C_PORT_RESET            0x0010

/* See USB3 Table 10-11. Port Status Field, wPortStatus */

#define USB3_HUB_STS_PORT_PLS_MASK       (0xF << 5)
#define USB3_HUB_STS_PORT_PLS_GET(uSts)  (((uSts) >> 5) & 0xF)
#define USB3_HUB_STS_PORT_PLS(uPls)      (((uPls) & 0xF) << 5)
/* USB3 PORT_POWER is bit 9 while USB2 is bit 8 */
#define USB3_HUB_STS_PORT_POWER          (1 << 9)
#define USB3_HUB_STS_PORT_SPEED_MASK     (0x7 << 10)
#define USB3_HUB_STS_PORT_SPEED_GET(uSts)(((uSts) >> 10) & 0x7)
#define USB3_HUB_STS_PORT_SPEED(uSpeed)  (((uSpeed) & 0x7) << 10)

/* See USB3 Table 10-12. Port Change Field, wPortChange */

#define USB3_HUB_C_BH_PORT_RESET         (1 << 5)
#define USB3_HUB_C_PORT_LINK_STATE       (1 << 6)
#define USB3_HUB_C_PORT_CONFIG_ERROR     (1 << 7)

/* Values for Port Link State (PORT_LINK_STATE) of USB3 ports */

#define USB3_PORT_PLS_U0                (0)
#define USB3_PORT_PLS_U1                (1)
#define USB3_PORT_PLS_U2                (2)
#define USB3_PORT_PLS_U3                (3)
#define USB3_PORT_PLS_SS_Disabled       (4)
#define USB3_PORT_PLS_Rx_Detect         (5)
#define USB3_PORT_PLS_SS_Inactive       (6)
#define USB3_PORT_PLS_Polling           (7)
#define USB3_PORT_PLS_Recovery          (8)
#define USB3_PORT_PLS_Hot_Reset         (9)
#define USB3_PORT_PLS_Compliance        (10)
#define USB3_PORT_PLS_Loopback          (11)

/* Values for (PORT_SPEED) of USB3 ports */

#define USB3_PORT_SPEED_5Gbps           (0)


/* USB 2.0 Table 11-17. Hub Class Feature Selectors */

#define USB_HUB_PORT_FEATURE_CONNECTION      (0x00)
#define USB_HUB_PORT_FEATURE_ENABLE          (0x01)
#define USB_HUB_PORT_FEATURE_SUSPEND         (0x02)
#define USB_HUB_PORT_FEATURE_OVER_CURRENT    (0x03)
#define USB_HUB_PORT_FEATURE_RESET           (0x04)
#define USB_HUB_PORT_FEATURE_POWER           (0x08)
#define USB_HUB_PORT_FEATURE_LOWSPEED        (0x09)
#define USB_HUB_PORT_FEATURE_HIGHSPEED       (0x0A) /* 10 */
#define USB_HUB_PORT_FEATURE_C_CONNECTION    (0x10) /* 16 */
#define USB_HUB_PORT_FEATURE_C_ENABLE        (0x11) /* 17 */
#define USB_HUB_PORT_FEATURE_C_SUSPEND       (0x12) /* 18 */
#define USB_HUB_PORT_FEATURE_C_OVER_CURRENT  (0x13) /* 19 */
#define USB_HUB_PORT_FEATURE_C_RESET         (0x14) /* 20 */
#define USB_HUB_PORT_FEATURE_TEST            (0x15) /* 21 */
#define USB_HUB_PORT_FEATURE_INDICATOR       (0x16) /* 22 */

/* See USB3 Table 10-15. Downstream Port Remote Wake Mask Encoding */

#define USB3_HUB_PORT_Conn_RWEnable        (1 << 0)
#define USB3_HUB_PORT_Disconn_RWEnable     (1 << 1)
#define USB3_HUB_PORT_OC_RWEnable          (1 << 2)

/* hub status endpoint */

#define USB_HUB_ENDPOINT_STS_HUB        0x01
#define USB_HUB_ENDPOINT_STS_PORT0      0x02

/* USB timing */

#define USB_TIME_POWER_ON           100     /* 100 msec */
#define USB_TIME_RESET              50      /* 50 msec */
#define USB_TIME_RESET_RECOVERY     10      /* 10 msec */
#define USB_TIME_SET_ADDRESS        2       /* 2 msec */
#define USB_TIME_REQUEST            5000    /* 5 seconds */
#define USB_TIME_RESUME             20      /* 20 msec */

/* USB packet identifiers */

#define USB_PID_SETUP               0x2d
#define USB_PID_OUT                 0xe1
#define USB_PID_IN                  0x69

/* packet size range */

#define USB_MIN_CTRL_PACKET_SIZE        8   /* min for control pipe */
#define USB_MAX_CTRL_PACKET_SIZE        64  /* max for control pipe */
#define USB_MAX_SS_CTRL_PACKET_SIZE     512  /* max for control pipe */
#define USB_MAX_HIGH_SPEED_BULK_SIZE    512 /* Maximum size for a high speed
                                             * bulk pipe
                                             */

#define USB_MAX_SUPER_SPEED_BULK_SIZE   0x400
#define USB_MAX_FULL_SPEED_BULK_SIZE    0x40

#define USB_MAX_BULK_OUT_NAK_RATE       255 /* Maximum NAK rate for a bulk
                                             * OUT pipe
                                             */

/* maximum bandwidth (expressed as nanoseconds per frame) which may be
 * allocated for various purposes.
 */

#define USB_LIMIT_ISOCH_INT     ((UINT32) 900000L)  /* 900 usec */
#define USB_LIMIT_ALL           ((UINT32) 999000L)  /* 999 usec */

#define USB_SETUP_PACKET_SIZE   (0x08)
#define USB_HUB_STATUS_SIZE     (0x04)

/* Typedefs */

/*
 * USB_SETUP
 */

typedef struct usb_setup
    {
    UINT8 requestType;          /* bmRequestType */
    UINT8 request;              /* bRequest */
    UINT16 value;               /* wValue */
    UINT16 index;               /* wIndex */
    UINT16 length;              /* wLength */
    } USB_SETUP, *pUSB_SETUP;

/*
 * USB_DESCR_HDR
 *
 * header common to all USB descriptors
 */

typedef struct usb_descr_hdr
    {
    UINT8 length;               /* bLength */
    UINT8 descriptorType;       /* bDescriptorType */
    } USB_DESCR_HDR, *pUSB_DESCR_HDR;

#define USB_DESCR_HDR_LEN       2

/*
 * USB_DESCR_HDR_SUBHDR
 *
 * Some classes of USB devices (e.g., audio) use descriptors with a
 * header and a subheader.
 */

typedef struct usb_descr_hdr_subhdr
    {
    UINT8 length;                   /* bLength */
    UINT8 descriptorType;           /* bDescriptorType */
    UINT8 descriptorSubType;        /* bDescriptorSubType */
    } USB_DESCR_HDR_SUBHDR, *pUSB_DESCR_HDR_SUBHDR;

/*
 * USB_DEVICE_DESCR
 */

typedef struct usb_device_descr
    {
    UINT8 length;                   /* bLength */
    UINT8 descriptorType;           /* bDescriptorType */
    UINT16 bcdUsb;                  /* bcdUSB - USB release in BCD */
    UINT8 deviceClass;              /* bDeviceClass */
    UINT8 deviceSubClass;           /* bDeviceSubClass */
    UINT8 deviceProtocol;           /* bDeviceProtocol */
    UINT8 maxPacketSize0;           /* bMaxPacketSize0 */
    UINT16 vendor;                  /* idVendor */
    UINT16 product;                 /* idProduct */
    UINT16 bcdDevice;               /* bcdDevice - dev release in BCD */
    UINT8 manufacturerIndex;        /* iManufacturer */
    UINT8 productIndex;             /* iProduct */
    UINT8 serialNumberIndex;        /* iSerialNumber */
    UINT8 numConfigurations;        /* bNumConfigurations */
    } WRS_PACK_ALIGN(4) USB_DEVICE_DESCR, *pUSB_DEVICE_DESCR;

#define USB_DEVICE_DESCR_LEN        18

/* Device Qualifier Descriptor */

typedef struct usb_device_qualifier_descr
    {
    UINT8 length;               /* bLength */
    UINT8 descriptorType;       /* bDescriptorType */
    UINT16 bcdUsb;              /* bcdUSB - USB release in BCD */
    UINT8 deviceClass;          /* bDeviceClass */
    UINT8 deviceSubClass;       /* bDeviceSubClass */
    UINT8 deviceProtocol;       /* bDeviceProtocol */
    UINT8 maxPacketSize0;       /* bMaxPacketSize0 */
    UINT8 numConfigurations;    /* bNumConfigurations */
    UINT8 reserved;             /* reserved */
    } WRS_PACK_ALIGN(4) USB_DEVICE_QUALIFIER_DESCR, *pUSB_DEVICE_QUALIFIER_DESCR;

#define USB_DEVICE_QUALIFIER_DESCR_LEN  10
/*
 * USB_CONFIG_DESCR
 */

typedef struct usb_config_descr
    {
    UINT8 length;               /* bLength */
    UINT8 descriptorType;       /* bDescriptorType */
    UINT16 totalLength;         /* wTotalLength */
    UINT8 numInterfaces;        /* bNumInterfaces */
    UINT8 configurationValue;   /* bConfigurationValue */
    UINT8 configurationIndex;   /* iConfiguration */
    UINT8 attributes;           /* bmAttributes */
    UINT8 maxPower;             /* MaxPower */
    } WRS_PACK_ALIGN(4) USB_CONFIG_DESCR, *pUSB_CONFIG_DESCR;

#define USB_CONFIG_DESCR_LEN            9

/*
 * USB_INTERFACE_DESCR
 */

typedef struct usb_interface_descr
    {
    UINT8 length;                   /* bLength */
    UINT8 descriptorType;           /* bDescriptorType */
    UINT8 interfaceNumber;          /* bInterfaceNumber */
    UINT8 alternateSetting;         /* bAlternateSetting */
    UINT8 numEndpoints;             /* bNumEndpoints */
    UINT8 interfaceClass;           /* bInterfaceClass */
    UINT8 interfaceSubClass;        /* bInterfaceSubClass */
    UINT8 interfaceProtocol;        /* bInterfaceProtocol */
    UINT8 interfaceIndex;           /* iInterface */
    } USB_INTERFACE_DESCR, *pUSB_INTERFACE_DESCR;

#define USB_INTERFACE_DESCR_LEN     9

/*
 * USB_ENDPOINT_DESCR
 */

typedef struct usb_endpoint_descr
    {
    UINT8 length;                   /* bLength */
    UINT8 descriptorType;           /* bDescriptorType */
    UINT8 endpointAddress;          /* bEndpointAddress */
    UINT8 attributes;               /* bmAttributes */
    UINT16 maxPacketSize;           /* wMaxPacketSize */
    UINT8 interval;                 /* bInterval */
    } WRS_PACK_ALIGN(1) USB_ENDPOINT_DESCR, *pUSB_ENDPOINT_DESCR;

#define USB_ENDPOINT_DESCR_LEN      7

/*
 * USB_ENDPOINT_COMPANION_DESCR
 */

typedef struct usb_endpoint_companion_descr
    {
    UINT8  bLength;             /* Descriptor length in bytes                */
    UINT8  bDescriptorType;     /* Endpoint Descriptor type                  */
    UINT8  bMaxBurst;           /* Max # packets the EP can TX/RX in a burst */
    UINT8  bmAttributes;        /* Endpoint MaxStreams (Bulk)/Mult(ISOCH)    */
    UINT16 wBytesPerInterval;   /* Total # bytes this EP will XFER every SI  */
    } WRS_PACK_ALIGN(1) USB_ENDPOINT_COMPANION_DESCR, *pUSB_ENDPOINT_COMPANION_DESCR;

#define USB_ENDPOINT_COMP_DESCR_LEN      6
#define USB_SS_EP_MAX_STREAMS(bmAttributes) (1 << ((bmAttributes) & 0x1F))
#define USB_SS_EP_MULT(bmAttributes) ((bmAttributes) & 0x03)

/*
 * USB_LANGUAGE_DESCR
 */

typedef struct usb_language_descr
    {
    UINT8 length;               /* bLength */
    UINT8 descriptorType;       /* bDescriptorType */
    UINT16 langId [1];          /* wLANGID[] - variable len */
    } USB_LANGUAGE_DESCR, *pUSB_LANGUAGE_DESCR;

/*
 * USB_STRING_DESCR
 */

typedef struct usb_string_descr
    {
    UINT8 length;               /* bLength */
    UINT8 descriptorType;       /* bDescriptorType */
    UINT8 string [1];           /* bString - variable len */
    } USB_STRING_DESCR, *pUSB_STRING_DESCR;

/*
 * USB_HID_DESCR
 */

typedef struct usb_descr_typlen
    {
    UINT8 type;                 /* bDescriptorType */
    UINT16 length;              /* wDescriptorLength */
    } WRS_PACK_ALIGN(1) USB_DESCR_TYPLEN;

typedef struct usb_hid_descr
    {
    UINT8 length;                       /* bLength */
    UINT8 descriptorType;               /* bDescriptorType */
    UINT16 bcdHid;                      /* bcdHID */
    UINT8 countryCode;                  /* bCountryCode */
    UINT8 numDescriptors;               /* bNumDescriptors */
    USB_DESCR_TYPLEN descriptor [1];    /* (variable length) */
    } WRS_PACK_ALIGN(1) USB_HID_DESCR, *pUSB_HID_DESCR;

#define USB_HID_DESCR_LEN   9           /* size of USB_HID_DESCR */

/*
 * USB_HUB_DESCR
 */

typedef struct usb_hub_descr
    {
    UINT8 length;                   /* bLength */
    UINT8 descriptorType;           /* bDescriptorType */
    UINT8 nbrPorts;                 /* bNbrPorts */
    UINT16 hubCharacteristics;      /* wHubCharacteristics */
    UINT8 pwrOn2PwrGood;            /* bPwrOn2PwrGood */
    UINT8 hubContrCurrent;          /* bHubContrCurrent */
    UINT8 deviceRemovable [1];      /* DeviceRemovable - variable len */
    UINT8 portPwrCtrlMask [1];      /* portPwrCtrlMask - variable len */
    } WRS_PACK_ALIGN(4) USB_HUB_DESCR, *pUSB_HUB_DESCR;

#define USB_HUB_DESCR_LEN   9

/*
 * USB_IAD_DESCR
 */

typedef struct usb_iad_descr
    {
    UINT8  length;                   /* bLength */
    UINT8  descriptorType;           /* bDescriptorType */
    UINT8  bFirstInterface ;         /* bFirstInterface  */
    UINT8  bInterfaceCount ;         /* bInterfaceCount */
    UINT8  bFunctionClass ;          /* bFunctionClass */
    UINT8  bFunctionSubClass;        /* bFunctionSubClass */
    UINT8  bFunctionProtocol;        /* bFunctionProtocol */
    UINT8  iFunction;                /* iFunction */
    } WRS_PACK_ALIGN(1) USB_IAD_DESCR, *pUSB_IAD_DESCR;

#define USB_IAD_DESCR_LEN   8

/*
 * USB_STANDARD_STATUS
 */

typedef struct usb_standard_status
    {
    UINT16 status;                  /* status word */
    } USB_STANDARD_STATUS, *pUSB_STANDARD_STATUS;

/*
 * This Data Structure stores information about the hub Status Change
 * information.
 */
typedef struct usb_hub_status
    {
    UINT16  wHubStatus;          /* Contains the status of the hub            */
    UINT16  wHubChange;          /* Contains the change status of the hub     */
    } USB_HUB_STATUS,* pUSB_HUB_STATUS;

/*
 * This Data Structure stores information about the Port status change
 * information.
 */
typedef struct usb_hub_port_status
    {
    UINT16  wPortStatus;                  /* The port status information      */
    UINT16  wPortChange;                  /* The port status change.          */
    } USB_HUB_PORT_STATUS, * pUSB_HUB_PORT_STATUS;

typedef struct usb_ss_hub_descr
    {
    UINT8   bDescLength;            /* Number of bytes in this descriptor (12) */
    UINT8   bDescriptorType;        /* Descriptor Type (2AH for SuperSpeed) */
    UINT8   bNbrPorts;              /* Number of downstream facing ports */
    UINT16  wHubCharacteristics;    /* Hub Characteristics */
    UINT8   bPwrOn2PwrGood;         /* Power on to powr good time (in 2ms) */
    UINT8   bHubContrCurrent;       /* Maximum current requirements */
    UINT8   bHubHdrDecLat;          /* Hub Packet Header Decode Latency */
    UINT16  wHubDelay;              /* Delay in ns forwarding packets */
    UINT16  wDeviceRemovable;       /* Indicates if a port has a removable 
                                     * device attached */
    } WRS_PACK_ALIGN(1) USB_SS_HUB_DESCR, * pUSB_SS_HUB_DESCR;

/* See USB3 Table 9-11. BOS Descriptor */

typedef struct usb_bos_descr
    {
    UINT8   bLength;                /* Size of descriptor */
    UINT8   bDescriptorType;        /* BOS Descriptor type */
    UINT16  wTotalLength;           /* Length of this all of its sub descrs */
    UINT8   bNumDeviceCaps;         /* The number of separate device 
                                     * capability descriptors in the BOS */
    } WRS_PACK_ALIGN(1) USB_BOS_DESCR, * pUSB_BOS_DESCR;

#define USB_BOS_DESCR_LEN  (5)
#define USB_BOS_DESCR_SIZE (5)

/* See USB3 Table 9-13. Device Capability Type Codes */

#define USB_DEV_CAP_TYPE_WIRELESS_USB      (0x01)
#define USB_DEV_CAP_TYPE_USB2_EXT          (0x02)
#define USB_DEV_CAP_TYPE_SUPERSPEED_USB    (0x03)
#define USB_DEV_CAP_TYPE_CONTAINER_ID      (0x04)

typedef struct usb_dev_cap_hdr
    {
    UINT8   bLength;                /* Size of descriptor */
    UINT8   bDescriptorType;        /* Descriptor type: DEVICE CAPABILITY Type */
    UINT8   bDevCapabilityType;     /* Device Capability Type Code */
    }USB_DEV_CAP_HDR, * pUSB_DEV_CAP_HDR;

/* See USB3 Table 9-14. USB 2.0 Extension Descriptor */

typedef struct usb_usb2_ext_descr
    {
    UINT8   bLength;                /* Size of descriptor */
    UINT8   bDescriptorType;        /* Descriptor type: DEVICE CAPABILITY Type */
    UINT8   bDevCapabilityType;     /* Device Capability Type Code */
    UINT8   bmAttributes;           /* Bitmap encoding of supported device 
                                     * level features */
    } WRS_PACK_ALIGN(1) USB_USB2_EXT_DESCR, * pUSB_USB2_EXT_DESCR;

#define USB_20EXT_DESCR_LEN      7

/* USB_USB2_EXT_DESCR::bmAttributes bit masks */

#define USB_USB2_EXT_ATTR_LPM        (1 << 1)

/* See USB3 Table 9-15. SuperSpeed Device Capabilities Descriptor */

typedef struct usb_ss_dev_cap_descr
    {
    UINT8   bLength;                /* Size of descriptor */
    UINT8   bDescriptorType;        /* Descriptor type: DEVICE CAPABILITY Type */
    UINT8   bDevCapabilityType;     /* Device Capability Type Code */
    UINT8   bmAttributes;           /* Bitmap encoding of supported device 
                                     * level features */
    UINT16  wSpeedsSupported;       /* Speed supported by this device when 
                                     * operating in SuperSpeed mode */  
    UINT8   bFunctionalitySupport;  /* The lowest speed at which all the 
                                     * functionality is supported */  
    UINT8   bU1DevExitLat;          /* U1 Device Exit Latency */  
    UINT16  wU2DevExitLat;          /* U2 Device Exit Latency */                                 
    } WRS_PACK_ALIGN(1) USB_SS_DEV_CAP_DESCR, * pUSB_SS_DEV_CAP_DESCR;

#define USB_SS_DEV_CAP_DESCR_LEN      10
/* USB_SS_DEV_CAP_DESCR::bmAttributes bit masks */

#define USB_SS_DEV_CAP_ATTR_LPM        (1 << 1)

/* USB_SS_DEV_CAP_DESCR::wSpeedsSupported bit masks */

#define USB_SS_DEV_CAP_SPD_LS          (1 << 0)
#define USB_SS_DEV_CAP_SPD_FS          (1 << 1)
#define USB_SS_DEV_CAP_SPD_HS          (1 << 2)
#define USB_SS_DEV_CAP_SPD_SS_5Gbps    (1 << 3)

/* See USB3 Table 9-16. Container ID Descriptor */

typedef struct usb_container_id_descr
    {
    UINT8   bLength;                /* Size of descriptor */
    UINT8   bDescriptorType;        /* Descriptor type: DEVICE CAPABILITY Type */
    UINT8   bDevCapabilityType;     /* Device Capability Type Code */
    UINT8   bReserved;              /* Reserved and shall be set to zero */
    UINT8   xContainerID[16];       /* UUID: 128-bit number that is unique 
                                     * to a device instance */
    } WRS_PACK_ALIGN(1) USB_CONTAINER_ID_DESCR, * pUSB_CONTAINER_ID_DESCR;

#define USB_CONTAINER_ID_DESCR_LEN      20

/* See USB3 9.4.12 Set SEL */

typedef struct usb_exit_latency
    {
    UINT8   uU1SEL;          /* Time in us for U1 System Exit Latency */
    UINT8   uU1PEL;          /* Time in us for U1 Device to Host Exit Latency */
    UINT16  uU2SEL;          /* Time in us for U2 System Exit Latency */
    UINT16  uU2PEL;          /* Time in us for U1 Device to Host Exit Latency */
    } WRS_PACK_ALIGN(1) USB_EXIT_LATENCY, * pUSB_EXIT_LATENCY;

/* Implementation-specific host definitions */

/* defines */

/* node types */

#define USB_NODETYPE_NONE       0
#define USB_NODETYPE_HUB        1
#define USB_NODETYPE_DEVICE     2

/* node speeds */

#define USB_SPEED_FULL          0   /* 12 mbit device */
#define USB_SPEED_LOW           1   /* low speed device (1.5 mbit) */
#define USB_SPEED_HIGH          2   /* 480M bps */
#define USB_SPEED_SUPER         3   /* 5.0G bps */
#define USB_SPEED_UNKNOWN       0xff

/* standard endpoints */

#define USB_ENDPOINT_CONTROL    0

/* transfer (pipe) types */

#define USB_XFRTYPE_CONTROL     1
#define USB_XFRTYPE_ISOCH       2
#define USB_XFRTYPE_INTERRUPT   3
#define USB_XFRTYPE_BULK        4

/* direction indicators for IRPs */

#define USB_DIR_OUT             1
#define USB_DIR_IN              2
#define USB_DIR_INOUT           3

/* data toggle specifiers for USB_IRP */

#define USB_DATA0               0
#define USB_DATA1               1

/* flags for IRPs */

#define USB_FLAG_SHORT_OK       0x00
#define USB_FLAG_SHORT_FAIL     0x01
#define USB_FLAG_ISO_ASAP       0x02

/* IRP timeouts */

#define USB_TIMEOUT_DEFAULT     5000        /* 5 seconds */
#define USB_TIMEOUT_NONE        0xffffffff  /* no timeout */

/* typedefs */

/*
 * IRP_CALLBACK
 */

typedef VOID (*IRP_CALLBACK) (pVOID pIrp);

/*
 * USBTU_IRPCONTEXT - save the internal states of Translation Unit
 */

typedef struct usbtu_irpcontext
    {
    UINT32           urbIndex;           /* index into array urbPtr */
    UINT32           urbCompleteCount;   /* URB completion count    */
    MSG_Q_ID         msgQid;             /* message queue id of client */
    pVOID            pPipeInfo;          /* ptr to pipe info structure */
    }USBTU_IRPCONTEXT, *pUSBTU_IRPCONTEXT;

/*
 * USB_BFR_LIST - used only for host class drivers
 */

typedef struct usb_bfr_list
    {
    USBHST_URB  urb;            /* URB for this buffer */
    UINT16 pid;                 /* Specifies packet type as USB_PID_xxxx */
    pUINT8 pBfr;                /* Pointer to bfr */
    UINT32 bfrLen;              /* Length of buffer */
    UINT32 actLen;              /* actual length transferred */
    } USB_BFR_LIST, *pUSB_BFR_LIST;

/*
 * USB_IRP
 *
 * NOTE: There are certain requirements on the layout of buffers described
 * in the bfrList[].
 *
 * For control transfers, the first bfrList [] entry must be the Setup packet.
 * If there is a data stage, the bfrList [] entry for the data stage should
 * follow.  Finally, a zero-length bfrList [] entry must follow which serves
 * as a place-holder for the status stage.
 *
 * For isochronous, interrupt, and bulk transfers there may be one or more
 * bfrList[] entries.
 *
 * If there is more than one bfrList[] entry for an isochronous, interrupt,
 * or bulk transfers or more than two bfrList [] entries for control
 * transfers, then each bfrList[].bfrLen (except the last) must be an exact
 * multiple of the maxPacketSize.  The HCD and underlying hardware will
 * make no attempt to gather (during OUT) or scatter (during IN) a single
 * USB packet across multiple bfrList[] entries.
 */

typedef struct usb_irp
    {
    USB_LINK usbdLink;          /* Link field used internally by USBD */
    pVOID usbdPtr;              /* Ptr field for use by USBD */
    USB_LINK hcdLink;           /* Link field used internally by USB HCD */
    pVOID hcdPtr;               /* Ptr field for use by USB HCD */
    pVOID userPtr;              /* Ptr field for use by client */
    USBTU_IRPCONTEXT tuContext; /* IRP TU context */
    UINT16 irpLen;              /* Total length of IRP structure */
    int result;                 /* IRP completion result: S_usbHcdLib_xxxx */
    IRP_CALLBACK usbdCallback;  /* USBD completion callback routine */
    IRP_CALLBACK userCallback;  /* client's completion callback routine */
    UINT16 dataToggle;          /* IRP should start with DATA0/DATA1. */
    UINT16 flags;               /* Defines other IRP processing options */
    UINT32 timeout;             /* IRP timeout in milliseconds */
    UINT16 startFrame;          /* Start frame for isoch transfer */
    UINT16 dataBlockSize;       /* Data granularity for isoch transfer */
    UINT32 transferLen;         /* Total length of data to be transferred */
    UINT16 bfrCount;            /* Indicates count of buffers in BfrList */
    USB_BFR_LIST bfrList [1];
    } USB_IRP, *pUSB_IRP;

/* Implementation-specific target definitions */

/*
 * ERP_CALLBACK
 */

typedef VOID (*ERP_CALLBACK) (pVOID pErp);

/*
 * USB_ERP_BFR_LIST - used only for target applications
 */

typedef struct usb_erp_bfr_list
    {
    UINT16 pid;                 /* Specifies packet type as USB_PID_xxxx */
    UINT16 uStreamId;           /* Stream ID for USB 3.0 Bulk transaction */
    pUINT8 pBfr;                /* Pointer to bfr */
    UINT32 bfrLen;              /* Length of buffer */
    UINT32 actLen;              /* actual length transferred */
    } USB_ERP_BFR_LIST, *pUSB_ERP_BFR_LIST;

/* The ERP control flags */

typedef enum usb_erp_flags
    {
    USB_ERP_FLAG_NORMAL      = (0),           /* Nothing to do, treat as normal */
    USB_ERP_FLAG_ZLP_NEEDED  = (0x01 << 0x00),/* ZLP needed after the data tranaction finished */
    USB_ERP_FLAG_FAST_RETURN = (0x01 << 0x01) /* Fast return, do not wait to get all data required */
    }USB_ERP_FLAGS;

/*
 * USB_ERP
 *
 * The USB_ERP is similar to the USB_IRP, except that it applies to data transfers
 * as viewed from the perspective of a USB device - as opposed to the USB_IRP, which
 * is used by the USB host.
 *
 * Each USB_ERP (USB Endpoint Request Packet) describes a transfer through a device
 * endpoint.  As with USB_IRPs, the USB_ERP carries a bfrList[] which is an array
 * of USB_BFR_LIST structures - each describing a block of data to be transferred.
 *
 * There are certain limitations on the bfrList[].  If the first entry in a bfrList[]
 * has a PID of USB_PID_SETUP, then there may be only a single bfrList[] entry.
 *
 * bfrList[] entries must be all IN or all OUT.  A single ERP cannot describe
 * transfers in both directions.
 *
 * <'targPtr'> is used by usbTargLib, which we called target management level
 * <'tcdPtr'> is used by USB TCD driver
 * <'userPtr'> is used by client, or USB target function driver
 *
 * <'pContext'> is used by the current transaction, to record the current contex
 * <'erpFlags'> is the flags of the current transaction. Such as 
 * the flag to submit one zero-length-packet(ZLP) after the transaction 
 * finished, which will be used when the length of the data is multiple of max
 * packet size.
 * .
 */

typedef struct usb_erp
    {
    USB_LINK   targLink;       /* Link field used internally by usbTargLib */
    pVOID  targPtr;            /* Ptr field for use by usbTargLib */
    USB_LINK   tcdLink;        /* Link field used internally by USB TCD */
    pVOID  tcdPtr;             /* Ptr field for use by USB TCD */
    pVOID  userPtr;            /* Ptr field for use by client */
    UINT16 erpLen;             /* Total length of ERP structure */
    UINT16 uStreamId;          /* Stream Id for BULK stream transaction */
    int    result;             /* ERP completion result: S_usbTcdLib_xxxx */
    ERP_CALLBACK targCallback; /* usbTargLib completion callback routine */
    ERP_CALLBACK userCallback; /* client's completion callback routine */
    pVOID  pPipeHandle;        /* Pipe handle */
    UINT16 transferType;       /* Type of ERP: control, bulk, etc. */
    UINT16 dataToggle;         /* ERP should start with DATA0/DATA1. */
    UINT16 bfrCount;           /* Indicates count of buffers in BfrList */
    UINT16 endpointId;         /* device endpoint */
                               /* Added for complaince with the old stack */
    pVOID  pContext;           /* The contex of the current transation */
    USB_ERP_FLAGS     erpFlags;/* The flags of the current transaction */
    USB_ERP_BFR_LIST  bfrList [1];
    } USB_ERP, *pUSB_ERP;


#ifdef  __cplusplus
}
#endif

#endif  /* __INCusbh */

/* End of file. */

