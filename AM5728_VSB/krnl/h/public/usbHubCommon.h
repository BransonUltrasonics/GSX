/* usbHubCommon.h - Macro definitions used in USB Hub class driver */

/*
 * Copyright (c) 2003,2005,2008-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003,2005,2008-2014 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
01y,15jun14,j_x  Modified the type for uNumberOfHubEvents (VXW6-80778)
01x,03sep13,wyy  Force to define USB_HUB_CREATE_BUSM_TASK_FOR_EACH_BUS and 
                 USB_HUB_CREATE_BUS_INSTANCE_FOR_EACH_HUB (WIND00432706)
01w,03may13,wyy  Remove compiler warning (WIND00356717)
01v,26sep12,w_x  Correct the way to calculate Route String (WIND00378361)
01u,19sep12,w_x  Delay device removal during device mounting reset (WIND00373115)
01t,17sep12,w_x  Use Multiple BusM mode as default configuration
01s,24aug12,w_x  Fix task schedule issue in single BusM mode (WIND00370558)
01r,19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
01q,13dec11,m_y  Modify according to code check result (WIND00319317)
01p,24jun11,ghs  Add defines of hub descriptor sequence flags (WIND00268645)
01o,26jul10,m_y  Remove uEnableChangeCount from structure usb_hub_port_info
                 (WIND00173382)
01n,05jul10,ghs  Fix conflict symbol defines(WIND00221550)
01m,29jun10,w_x  Add more port reset delay and clean hub logging (WIND00216628)
01l,24may10,m_y  Add usb hub enable status macro for usb robustness (WIND00183499)
01k,27apr10,j_x  Hard code replacement (WIND00183874)
01j,27jan10,y_l  Add device reset flag (WIND00151882)
01i,08jan10,y_l  Add hub current status buffer changed flag (WIND00191906)
01h,10nov09,w_x  Prevent endless enable change loop for quirky hubs that set
                 both reset and enable change at reset complete (WIND00189783)
01g,13jan10,ghs  vxWorks 6.9 LP64 adapting
01f,30sep09,w_x  Add uNumErrors for USB_HUB_INFO (WIND00183403)
01e,17jul09,w_x  Fix crash in reset callback (WIND00172353)
01d,17dec08,w_x  Added mutex to protect hub status data ;
                 Use seperate copy of hub status to avoid potential corruption;
                 Use DMA/cache safe buffer for hub status ;
                 Redefine USB_HUB_STATE_TYPE due to confilct with strucure
                 USB_HUB_STATES (WIND00148225)
01c,11dec08,s_z  Correct the comment of wPortChange(WIND00116506)
01b,18aug05,ami  Fix for enumerating hubs with class code 0x101
01a,30may03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This file contains the macro definitions that are used through
out the USB Hub Class Driver.
*/


/*
 INTERNAL
 ******************************************************************************
 * Filename         : usbHubCommon.h
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
 *
 * Description      : This contains the macro definitions that are used through
 *                    out the USB Hub Class Driver.
 *
 ******************************************************************************/

#ifndef __usbHubCommonh
#define __usbHubCommonh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <usb.h>
#include <usbOsal.h>
#include <usbHst.h>

#define  USB_HUB_CREATE_BUS_INSTANCE_FOR_EACH_HUB
#define  USB_HUB_CREATE_BUSM_TASK_FOR_EACH_BUS

/*
 * The following macros shall be always defined. Because this feature, using 
 * one task to support all host controllers, is still under development.
 */

#ifndef USB_HUB_CREATE_BUS_INSTANCE_FOR_EACH_HUB
#warning "USB_HUB_CREATE_BUS_INSTANCE_FOR_EACH_HUB is forced to be defined"
#define  USB_HUB_CREATE_BUS_INSTANCE_FOR_EACH_HUB
#endif

#ifndef USB_HUB_CREATE_BUSM_TASK_FOR_EACH_BUS
#warning "USB_HUB_CREATE_BUSM_TASK_FOR_EACH_BUS is forced to be defined"
#define  USB_HUB_CREATE_BUSM_TASK_FOR_EACH_BUS
#endif

#ifndef USB_HUB_CREATE_BUSM_TASK_FOR_EACH_BUS
extern OS_THREAD_ID BusManagerThreadID;
#endif

/* defines */

#define    USB_HUB_VERSION_NUMBER             "Devel_0.01"
#define    USB_HUB_VERSION_DATE               "4th April 2003"

/* Priority of the Hub's Threads */
#define    USB_HUB_THREAD_PRIORITY            (100)

/* The default configuration Index for a Hub */
#define    USB_DEF_ACTIVE_HUB_CONFIG_INDEX    ((UINT8)0x00)

/* Maximum retries for connection attempt */
#define    USB_HUB_CONFIG_RETRY_MAX           ((UINT8)0x03)

/* Maximum retries for port disable at reset change situations */
#define     USB_HUB_ENABLE_CHANGE_MAX         ((UINT8)0x03)

/* Maximum retries for hub interrupt pipe error condition */
#define    USB_HUB_ERROR_COUNT_MAX            ((UINT8)0x5)

/* Maximum frame number 11 bits are allowed for this field */
#define    USB_HUB_MAXIMUM_FRAME_NUMBER       ((UINT16) 2048)

/* The Debounce Time intreval */
#define    USB_HUB_PORT_DEBOUNCE_TIME         ((UINT16) 100)

/* Define for the Maximum power per root hub */
#define    USB_MAXIMUM_POWER_PER_ROOT_HUB     ((UINT8)0x5)

/* Port number not found */
#define    USB_PORT_NUMBER_NOT_FOUND          ((UINT8)255)

/* The Maximum depth that the USB Tree can grow in USB1.1 */
#define    HUB_MAX_ALLOWED_DEPTH_USB1_1       ((UINT8)5)


/* The Maximum depth that the USB Tree can grow in USB2.0 */
#define    HUB_MAX_ALLOWED_DEPTH_USB2_0       ((UINT8)7)


/* The default endpoint */
#define    USB_HUB_DEFAULT_ENDPOINT           ((UINT8)0x0)

/* Flag for indicating a port as not a High Speed port */
#define    USB_NOT_HIGH_SPEED_HUB_PORT        ((UINT8)0xFF)

/* Defines for the Usb Version of Hub */
#define   USB_HUB_VERSION_0200                ((UINT16)0x0200)
#define   USB_HUB_VERSION_0110                ((UINT16)0x0110)
#define   USB_HUB_VERSION_0100                ((UINT16)0x0100)
#define   USB_HUB_VERSION_0101                ((UINT16)0x0101)

/* Defines Status bits USB2.0 spec- table 11-7 (wValue) */
#define    USB_HUB_LOCAL_POWER                ((UINT8)0X0)
#define    USB_HUB_OVER_CURRENT               ((UINT8)0X1)
#define    USB_PORT_CONNECTION                ((UINT8)0X0)
#define    USB_PORT_ENABLE                    ((UINT8)0X1)
#define    USB_PORT_SUSPEND                   ((UINT8)0X2)
#define    USB_PORT_OVER_CURRENT              ((UINT8)0X3)
#define    USB_PORT_RESET                     ((UINT8)0X4)
#define    USB_PORT_POWER                     ((UINT8)0X8)
#define    USB_PORT_LOW_SPEED                 ((UINT8)0X9)
#define    USB_C_PORT_CONNECTION              ((UINT8)0X10)
#define    USB_C_PORT_ENABLE                  ((UINT8)0X11)
#define    USB_C_PORT_SUSPEND                 ((UINT8)0X12)
#define    USB_C_PORT_OVER_CURRENT            ((UINT8)0X13)
#define    USB_C_PORT_RESET                   ((UINT8)0X14)

/* defines for the bit value of the wHubStatus */
#define    USB_HUB_LOCAL_POWER_VALUE          ((UINT16)0X1)
#define    USB_HUB_OVER_CURRENT_VALUE         ((UINT16)0X2)

/* defines for the bit value of the wHubStatusChange */
#define    USB_C_HUB_LOCAL_POWER_VALUE         ((UINT16)0X1)
#define    USB_C_HUB_OVER_CURRENT_VALUE        ((UINT16)0X2)

/* defines for the bit value of the wPortStatus */
#define    USB_PORT_CONNECTION_VALUE          ((UINT16)0X0001)
#define    USB_PORT_ENABLE_VALUE              ((UINT16)0X0002)
#define    USB_PORT_SUSPEND_VALUE             ((UINT16)0X0004)
#define    USB_PORT_OVER_CURRENT_VALUE        ((UINT16)0X0008)
#define    USB_PORT_RESET_VALUE               ((UINT16)0X0010)
#define    USB_PORT_POWER_VALUE               ((UINT16)0X0100)
#define    USB_PORT_SPEED_VALUE               ((UINT16)0X0600)

/* defines for the bit value of the wPortChange */
#define    USB_C_PORT_CONNECTION_VALUE        ((UINT16)0X0001)
#define    USB_C_PORT_ENABLE_VALUE            ((UINT16)0X0002)
#define    USB_C_PORT_SUSPEND_VALUE           ((UINT16)0X0004)
#define    USB_C_PORT_OVER_CURRENT_VALUE      ((UINT16)0X0008)
#define    USB_C_PORT_RESET_VALUE             ((UINT16)0X0010)

/* Defines for the Features USB2.0 spec- table 9-4 (bRequest) */
#define    USB_CLEAR_FEATURE                  ((UINT8) 0x01)
#define    USB_SET_FEATURE                    ((UINT8) 0x03)
#define    USB_GET_DESCRIPTOR                 ((UINT8) 0x06)
#define    USB_SET_CONFIGURATION              ((UINT8) 0x09)
#define    USB_GET_STATUS                     ((UINT8) 0x00)
#define    USB_GET_HUB_DESCRIPTOR             ((UINT8) 0xA0)


/* Defines for Hub Class Request */
#define    USB_CLEAR_TT_REQUEST               ((UINT8) 0x08)
#define    USB_RESET_TT                       ((UINT8) 0x09)

/* Defines for the bmRequest Field */
#define    USB_HUB_TARGET_GET                 ((UINT8) 0xA0)
#define    USB_HUB_TARGET_SET                 ((UINT8) 0x20)
#define    USB_PORT_TARGET_GET                ((UINT8) 0xA3)
#define    USB_PORT_TARGET_SET                ((UINT8) 0x23)
#define    USB_DEVICE_TARGET_GET              ((UINT8) 0x80)

/* Defines for the Descriptor Types */
#define    USB_HUB_DESCRIPTOR                 ((UINT8) 0x29)
#define    USB_CONFIGURATION_DESCRIPTOR       ((UINT8) 0x02)

/* Defines for the hub states */
#define    USB_HUB_STATE_TYPE                     UINT8
#define    USB_HUB_NO_STATE                   ((USB_HUB_STATE_TYPE) 0x00)
#define    USB_MARKED_FOR_DELETION            ((USB_HUB_STATE_TYPE) 0x01)
#define    USB_HUB_DEBOUNCE_PENDING           ((USB_HUB_STATE_TYPE) 0x02)
#define    USB_HUB_RESET_PENDING              ((USB_HUB_STATE_TYPE) 0x03)
#define    USB_HUB_RESET_COMPLETION_PENDING   ((USB_HUB_STATE_TYPE) 0x04)
#define    USB_HUB_RESET_COMPLETED            ((USB_HUB_STATE_TYPE) 0x05)
#define    USB_HUB_PORT_DEFAULT               ((USB_HUB_STATE_TYPE) 0x06)
#define    USB_HUB_PORT_ADDRESSED             ((USB_HUB_STATE_TYPE) 0x07)
#define    USB_HUB_PORT_CONFIGURED            ((USB_HUB_STATE_TYPE) 0x08)

/* Defines the bit mask of  multiple TT */
#define HUB_MASK_MULTIPLE_TT                  0x80

/* Defines the bit mask of  multiple TT port number */
#define HUB_MASK_MULTIPLE_TT_PORT             0x7F

/* Defines the sub class code for hub class devices */
#define HUB_SUB_CLASS_CODE                    0x00

/* Defines protocol code for hub class devices */
#define HUB_PROTOCOL_CODE                     0x00

/* Defines the length of hub device descriptor */
#define HUB_LENGTH_DEVICE_DESC                18

/* Defines the length of hub config descriptor */
#define HUB_LENGTH_CONFIG_DESC                8

/* Defines the bDeviceProtocol of multiple TT hub*/
#define HUB_PROTOCOL_MULTIPLE_TT              2

/* Defines the size of hub port status */
#define HUB_SIZE_PORT_STATUS                  4

/* Defines the valid range of protocol */
#define HUB_VALID_RANGE_PROTOCOL              3

/* Defines the valid max packet size */
#define HUB_MAX_PACKET_SIZE                   64

/* Defines the number of configures */
#define HUB_NUMBER_CONFIG                     1

/* Defines the number of interfaces */
#define HUB_NUMBER_INTERFACE                  1

/* Defines the interface number */
#define HUB_INTERFACE_NUMBER                  0

/* Defines the valid range of bAlternateSetting */
#define HUB_VALID_RANGE_ALTERNATE             2

/* Defines the number of endpoints */
#define HUB_NUMBER_ENDPOINT                   1

#define USB_HUB_TT_TT(wHubCharacteristics) \
        ((wHubCharacteristics >> 5) & 0x3)

/* Defines sequence flags for hub descriptors */
#define USB_HUB_INTERFACE_DEFAULT_DESC_SEQ    (1 << 4)
#define USB_HUB_ENDPOINT_DESC_SEQ             (2 << 8)
#define USB_HUB_INTERFACE_ALTERNATE_DESC_SEQ  (4 << 12)

/*******************************************************************************
 * Macro Name  : HUB_DESC_bNbrPorts
 * Description : This returns the number of ports that a specific hub contains
 *               by parsing the hub  descriptor.
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : UINT8
 ******************************************************************************/

#define USB_HUB_DESC_bNbrPorts(pHubDescriptor)                                 \
     ((UINT8*) (pHubDescriptor))[2]


/*******************************************************************************
 * Macro Name  : HUB_DESC_wHubCharacteristics
 * Description : This parses the hub descriptor and returns the
 *               wHubCharacteristic.
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : UINT16
 ******************************************************************************/

#define USB_HUB_DESC_wHubCharacteristics(pHubDescriptor)                       \
        ((((UINT8*) (pHubDescriptor))[4]<<8)     |                            \
        (( (UINT8*) (pHubDescriptor))[3]))


/*******************************************************************************
 * Macro Name  : HUB_DESC_bPwrOn2PwrGood
 * Description : This parses the hub descriptor and returns the bPwrOn2PwrGood.
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : UINT8
 ******************************************************************************/

#define USB_HUB_DESC_bPwrOn2PwrGood(pHubDescriptor)                            \
        (((UINT8*) (pHubDescriptor))[5])


/*******************************************************************************
 * Macro Name  : HUB_DESC_bHubContrCurrent
 * Description : This parses the hub descriptor and returns the bHubContrCurrent.
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : UINT8
 ******************************************************************************/

#define USB_HUB_DESC_bHubContrCurrent(pHubDescriptor)                          \
        (((UINT8*) (pHubDescriptor))[6])


/*******************************************************************************
 * Macro Name  : HUB_DESC_GET_NUMPORT_OFFSET
 * Description : This parses the hub descriptor and returns the offset required
 *               for data of the DeviceRemovableMap and PortPWrCtrlMask fields
 *               as per the byte granularity.
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 *               uOffset                 OUT  the offset for the number of ports
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_DESC_GET_NUMPORT_OFFSET(pHubDescriptor,uOffset)                \
{                                                                              \
    UINT8 uNbrOfPorts = HUB_DESC_bNbrPorts(pHubDescriptor);                    \
    /* give the 0th Bit Padding */                                             \
    uNbrOfPorts++;                                                             \
    /* Implement the byte granurality */                                       \
    uOffset = USB_HUB_BYTE_GRANULARITY(uNbrOfPorts);                           \
}


/*******************************************************************************
 * Macro Name  : HUB_DESC_DevRemovableMapStart
 * Description : This parses the hub descriptor and returns the pointer to the
 *               start of the DeviceRemovable bit map
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : PUINT8
 ******************************************************************************/

#define USB_HUB_DESC_DevRemovableMapStart(pHubDescriptor)                      \
        ((UINT8*)(((ULONG)(pHubDescriptor))+7 ))


/*******************************************************************************
 * Macro Name  : HUB_DESC_PortPwrCtrlMaskStart
 * Description : This parses the hub descriptor and returns the pointer to the
 *               start of the PortPwrCtrlMask bit map
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : PUINT8
 ******************************************************************************/

#define USB_HUB_DESC_PortPwrCtrlMaskStart(pHubDescriptor,pPortPwrCtrlMask)     \
{                                                                              \
    UINT8 uOffset = 0;                                                         \
    HUB_DESC_GET_NUMPORT_OFFSET (pHubDescriptor,uOffset);                      \
    pPortPwrCtrlMask    =  ((UINT8*)( ( (ULONG) (pHubDescriptor) )            \
                                         +7+uOffset ) );                       \
}


/*******************************************************************************
 * Macro Name  : HUB_DESC_IS_GANG_POWERED
 * Description : This parses the hub descriptor and returns if the device is
 *               Gang powered or not.
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_DESC_IS_GANG_POWERED(pHubDescriptor)                           \
    (0==(UINT8)(HUB_DESC_wHubCharacteristics(pHubDescriptor)&0x1)?             \
           FALSE : TRUE)


/*******************************************************************************
 * Macro Name  : HUB_CONFIG_wTotalLength
 * Description : This parses the config descriptor and returns the total length
 *               of the descriptor
 * Parameters  : pConfigDescriptor        IN   pointer to the config descriptor.
 * Return Type : UINT16
 ******************************************************************************/

#define USB_HUB_CONFIG_wTotalLength(pConfigDescriptor)                         \
    (((pConfigDescriptor)[2]&0xff) | ((pConfigDescriptor)[3]&0xff)<<8)


/*******************************************************************************
 * Macro Name  : HUB_DESC_IS_COMPOUND_DEVICE
 * Description : This parses the hub descriptor and returns if the hub is part
 *               of a compound device or not.
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_DESC_IS_COMPOUND_DEVICE(pHubDescriptor)                        \
    (0==(UINT8)(USB_HUB_DESC_wHubCharacteristics(pHubDescriptor)&0x4)?         \
           FALSE : TRUE)


/*******************************************************************************
 * Macro Name  : HUB_DESC_POWER_PROTECTION_SCHEME
 * Description : This parses the hub descriptor and returns the power protection
 *               scheme used - if this field is 0 this is Global protection
 *               scheme, if 1, it has per port reporting. If >1 then it has no
 *               power current protection
 * Parameters  : pHubDescriptor          IN   pointer to the Hub descriptor.
 * Return Type : UINT8
 ******************************************************************************/

#define USB_HUB_DESC_POWER_PROTECTION_SCHEME(pHubDescriptor)                   \
    ( (USB_HUB_DESC_wHubCharacteristics(pHubDescriptor)&0x18)>>3 )


/*******************************************************************************
 * Macro Name  : HUB_IS_HUB_EVENT
 * Description : This parses the Event for a given Status and returns if any
 *               hub event has occurred.
 * Parameters  : uStatus                IN   Change status bitmap of the hub.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_EVENT(uStatus)                                              \
    ( ( 0==(((UINT8*)(uStatus))[0]&0x01))?FALSE:TRUE)

/*******************************************************************************
 * Macro Name  : HUB_IS_PORT_EVENT
 * Description : This parses the Event for a given Status and returns if any
 *               event has occurred in the port specified.
 * Parameters  : pStatus                IN   Change status bitmap of the hub.
 *               uPortNumber            IN   The port number of the change
 *               bResult                OUT  The result of the check
 * Return Type : None.
 ******************************************************************************/

#define USB_HUB_IS_PORT_EVENT(pStatus, uPortNumber)                            \
     ( (0 != ((pStatus)[(uPortNumber)/8]&((UINT8)0x1<<((uPortNumber)%8))) )?   \
            TRUE : FALSE )


/*******************************************************************************
 * Macro Name  : HUB_IS_CONNECTION_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               connect change event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_CONNECTION_CHANGE(uStatus)                                  \
    ((  0==((uStatus)&USB_C_PORT_CONNECTION_VALUE))? FALSE : TRUE)


/*******************************************************************************
 * Macro Name  : HUB_IS_ENABLE_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               enable change event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_ENABLE_CHANGE(uStatus)                                      \
    ((  0==((uStatus)&USB_C_PORT_ENABLE_VALUE))? FALSE : TRUE)

/*******************************************************************************
 * Macro Name  : USB_HUB_IS_PORT_ENABLED
 * Description : This parses the event for a given status and returns if status
 *               is enabled.
 * Parameters  : uStatus                IN   Status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_PORT_ENABLED(uStatus)                                      \
    ((  0==((uStatus)&USB_PORT_ENABLE_VALUE))? FALSE : TRUE)

/*******************************************************************************
 * Macro Name  : HUB_IS_SUSPEND_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               suspend change event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_SUSPEND_CHANGE(uStatus)                                     \
    ((  0==((uStatus)&USB_C_PORT_SUSPEND_VALUE))? FALSE : TRUE)


/*******************************************************************************
 * Macro Name  : HUB_IS_RESET_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               reset change event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_RESET_CHANGE(uStatus)                                       \
    ((  0==((uStatus)&USB_C_PORT_RESET_VALUE))? FALSE : TRUE)


/*******************************************************************************
 * Macro Name  : HUB_IS_OVER_CURRENT_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               over current event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_OVER_CURRENT_CHANGE(uStatus)                                \
    ((  0==((uStatus)&USB_C_PORT_OVER_CURRENT_VALUE))? FALSE:TRUE)

/*******************************************************************************
 * Macro Name  : USB_HUB_IS_LINK_STATE_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               link state change event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_LINK_STATE_CHANGE(uStatus)                                  \
    ((0 == ((uStatus) & USB3_HUB_C_PORT_LINK_STATE)) ? FALSE : TRUE)

/*******************************************************************************
 * Macro Name  : USB_HUB_IS_BH_RESET_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               BH reset change event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_BH_RESET_CHANGE(uStatus)                                    \
    ((0 == ((uStatus) & USB3_HUB_C_BH_PORT_RESET)) ? FALSE : TRUE)

/*******************************************************************************
 * Macro Name  : USB_HUB_IS_CONFIG_ERROR_CHANGE
 * Description : This parses the event for a given status and returns if any
 *               config error change event has occurred.
 * Parameters  : uStatus                IN   Change status of the port.
 * Return Type : BOOLEAN
 ******************************************************************************/

#define USB_HUB_IS_CONFIG_ERROR_CHANGE(uStatus)                                \
    ((0 == ((uStatus) & USB3_HUB_C_PORT_CONFIG_ERROR)) ? FALSE : TRUE)

/*******************************************************************************
 * Macro Name  : HUB_DEVICE_SPEED
 * Description : This detects the speed of the device from the port status
 * Parameters  : uStatus                IN   status of the port.
 * Return Type : UINT8
 ******************************************************************************/

#define USB_HUB_DEVICE_SPEED(uStatus)                                          \
        ((UINT8)((((uStatus) & USB_PORT_SPEED_VALUE) >> 9) & 0xFF))

/*******************************************************************************
 * Macro Name  : HUB_SET_HUB_FEATURE
 * Description : This will submit a blocking call to set a hub feature.
 * Parameters  : pHub                  IN   The Hub pointer.
 *               bResult               OUT  The result will be stored here
 *               Feature               IN   The feature on the hub to be set
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_SET_HUB_FEATURE(pHub,bResult,Feature)                          \
{                                                                              \
    bResult=usbHubSubmitControlRequest(pHub,USB_HUB_TARGET_SET,                \
    USB_SET_FEATURE,Feature ,0);                                               \
}


/*******************************************************************************
 * Macro Name  : HUB_CLEAR_HUB_FEATURE
 * Description : This will submit a blocking call to clear a hub feature.
 * Parameters  : pHub                  IN   The Hub pointer.
 *               bResult               OUT  The result will be stored here
 *               Feature               IN   The feature on the hub to be set
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_CLEAR_HUB_FEATURE(pHub,bResult,Feature)                        \
{                                                                              \
    bResult=usbHubSubmitControlRequest(pHub, USB_HUB_TARGET_SET,               \
    USB_CLEAR_FEATURE,Feature ,0);                                             \
}


/*******************************************************************************
 * Macro Name  : HUB_SET_PORT_FEATURE
 * Description : This will submit a blocking call to set a port feature.
 *               Converts the port number
 * Parameters  : pHub                  IN   The Hub pointer.
 *               uPortIndex            IN   Port index +1 = port number
 *               Feature               IN   The feature on the hub to be set
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_SET_PORT_FEATURE(pHub,uPortIndex,Feature)                      \
        usbHubSubmitControlRequest(pHub,                                       \
                                 USB_PORT_TARGET_SET,                          \
                                 USB_SET_FEATURE,                              \
                                 Feature,                                      \
                                 (UINT16)(uPortIndex + 1))


/*******************************************************************************
 * Macro Name  : USB_HUB_SET_PORT_LINK_STATE
 * Description : This will submit a blocking call to set port link state.
 * Parameters  : pHub                  IN   The Hub pointer.
 *               uPortIndex            IN   Port index +1 = port number
 *               LinkState             IN   The link state on port to be set
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_SET_PORT_LINK_STATE(pHub,uPortIndex,LinkState)                 \
        usbHubSubmitControlRequest(pHub,                                       \
                                 USB_PORT_TARGET_SET,                          \
                                 USB_SET_FEATURE,                              \
                                 USB3_HUB_FSEL_PORT_LINK_STATE,                \
                                 (UINT16)(((LinkState) << 8) |((uPortIndex) + 1)))

/*******************************************************************************
 * Macro Name  : HUB_CLEAR_PORT_FEATURE
 * Description : This will submit a blocking call to clear a port feature.
 * Parameters  : pHub                  IN   The Hub pointer.
 *               uPortIndex            IN   Port index +1 = port number
 *               Feature               IN   The feature on the hub to be set
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_CLEAR_PORT_FEATURE(pHub,uPortIndex,Feature)                    \
        usbHubSubmitControlRequest(pHub,                                       \
                                 USB_PORT_TARGET_SET,                          \
                                 USB_CLEAR_FEATURE,                            \
                                 Feature,                                      \
                                 (UINT16)(uPortIndex + 1))


/*******************************************************************************
 * Macro Name  : HUB_GET_HUB_STATUS
 * Description : This will submit a blocking call to get a hub status.
 * Parameters  : pHub                  IN      The Hub pointer.
 *               pBuffer               IN OUT  The buffer to which the status is
 *                                             copied
 *               pBufferLength         IN OUT  The Length of the Buffer
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_GET_STATUS(pHub,pBuffer,pBufferLength)                         \
        usbHubGetStatus(pHub, 0, USB_HUB_TARGET_GET,pBuffer,pBufferLength)


/*******************************************************************************
 * Macro Name  : HUB_GET_PORT_STATUS
 * Description : This will submit a blocking call to get a port status.
 * Parameters  : pHub                  IN      The Hub pointer.
 *               uPortIndex            IN      Port index +1 = port number
 *               pBuffer               IN OUT  The buffer to which the status is
 *                                             copied
 *               pBufferLength         IN OUT  The Length of the Buffer
 * Return Type : None
 ******************************************************************************/

#define USB_HUB_GET_PORT_STATUS(pHub,uPortIndex,pBuffer,pBufferLength)         \
        usbHubGetPortStatus(pHub, (UINT8)(uPortIndex+1),USB_PORT_TARGET_GET,pBuffer,pBufferLength)


/*******************************************************************************
 * Macro Name  : HUB_TIME_DIFF
 * Description : This finds the difference of time between the current
 *               frame number and a different frame number.
 * Parameters  : uCurrentFrame         IN   This is the current frame number.
 *               uLastFrame            IN   This is the last frame number.
 * Return Type : UINT16
 ******************************************************************************/

#define USB_HUB_TIME_DIFF(uCurrentFrame,uLastFrame)                            \
    ( ((uLastFrame)>(uCurrentFrame))?                                          \
    (USB_HUB_MAXIMUM_FRAME_NUMBER - (uLastFrame)+uCurrentFrame ):              \
    (uCurrentFrame-(uLastFrame)) )


/*******************************************************************************
 * Macro Name  : HUB_BYTE_GRANULARITY
 * Description : This converts the parameter into nearest largest byte count
 *               (byte granularity form).
 * Parameters  : uNumberOfBits         IN   This is the value that has to be
 *                                          converted to byte granularity form.
 * Return Type : UINT8
 ******************************************************************************/

#define USB_HUB_BYTE_GRANULARITY(uNumberOfBits)                                \
    ((((UINT8)(uNumberOfBits)%8)>0)?                                          \
     (((uNumberOfBits)/8)+1): ((uNumberOfBits)/8) )


/*******************************************************************************
 * Macro Name  : MARK_FOR_DELETE_PORT
 * Description : This marks the port for deletion. This takes care of reseting
 *               the global bus state if the device connected to the port is in
 *               default state
 * Parameters  : pHub            IN   pointer to the Hub
 *               pPort           IN   pointer to the port on the hub
 * Return Type : None.
 ******************************************************************************/

#define USB_MARK_FOR_DELETE_PORT(pHub,pPort)                                   \
    {                                                                          \
    /* Check if we have the bus information */                                 \
    if ((NULL != (pHub)->pBus) &                                               \
        (USB_HUB_PORT_CONFIGURED > (pPort)->StateOfPort) &                     \
        (USB_HUB_RESET_PENDING < (pPort)->StateOfPort))                        \
        {                                                                      \
        /* Set the bus state as no device is being configured */               \
        (pHub)->pBus->bDeviceBeingConfigured = FALSE;                          \
        } /* End of if (NULL !=.. */                                           \
                                                                               \
    (pPort)->StateOfPort = USB_MARKED_FOR_DELETION;                            \
                                                                               \
    } /* End of if ( (HUB_PORT_CONFIGURED > pPort->StateOfPort .. */

/*******************************************************************************
 * Macro Name  : VALIDATE_DESCRIPTOR_SEQUENCE
 * Description : This validates the config descriptor
 *               Only valid descriptors Sequence is
 *               1) 0x24210 (Endpoint,AlternateInterface,
 *                           Endpoint,DefaultInterface,,Config)
 *               2) 0x00210 (Endpoint,DefaultInterface,Config)
 * Parameters  : uDescriptorSequence    IN   Value showing the descriptor
 *                                           sequence
 * Return Type : BOOLEAN
 ******************************************************************************/
#define USB_VALIDATE_DESCRIPTOR_SEQUENCE(uDescriptorSequence)                  \
    (( 0 == ((uDescriptorSequence == 0x24210) ||                               \
        (uDescriptorSequence == 0x00210 ) ) ) ? FALSE : TRUE)

/* This Data Structure stores information about the hub  descriptor. */
typedef struct usb_hub_descriptor_info
{
    UINT8   bNbrPorts;           /* The Number of Ports                       */
    UINT16  wHubCharacteristics; /* Characteristics of the hub                */
    UINT8   bPwrOn2PwrGood;      /* Time to wait before the Power is stable   */
    UINT8   bHubContrCurrent;    /* Power Requiement by the hub electronics   */
} OS_STRUCT_PACKED USB_HUB_DESCRIPTOR_INFO, * pUSB_HUB_DESCRIPTOR_INFO;

/*
 * The following have been pre defined here as a method to handle cyclic
 * dependency
 */
typedef struct usb_hub_info       * pUSB_HUB_INFO;
typedef struct usb_hub_bus_info   * pUSB_HUB_BUS_INFO;
typedef struct usb_hub_port_info  * pUSB_HUB_PORT_INFO;

/*
 * This data structure holds the information about a port of a hub as to what
 * device is connected to that port. The port status is also stored in this
 * structure.
 */
typedef struct usb_hub_port_info
{
    UINT32               uDeviceHandle;   /* Identification of the device */
    UINT32               uRouteString;    /* Route String from root port */
    BOOLEAN              bDebouncePeriod; /* States if the port is in debounce*/
    BOOLEAN              bOldConnectStatus; /* previous connect status        */
    UINT16               uConnectFrame;   /* The frame num when last connected*/
    UINT8                uConnectRetry;   /* number of times connect retried  */
    USBHST_URB           portStatusURB;   /* Port Status URB                  */
    USBHST_SETUP_PACKET  portStatusSetup; /* Port status setup packet         */
    SEM_ID               hPortStatusSem;  /* Semaphore for status completion  */
    pUSB_HUB_INFO        pParentHub;      /* pointer to the parent hub        */
    UINT8                uPortIndex;      /* index of port in the parent hub  */
    pUSB_HUB_INFO        pHub;            /* pointer to the hub if this is hub*/
    USB_HUB_STATE_TYPE   StateOfPort;     /* State of this port               */
    UINT8                uPortSpeed;      /* Port Speed */
    BOOLEAN              bDelayedRemove;  /* During reset, we can not remove it */
    BOOLEAN              bResetCheckDevice; /* When a device is plugged in,
                                            * after reset need to create a
                                            * device handler; But when a class
                                            * driver send a device reset request,
                                            * the device handler has been created
                                            * alreay, no need to create it again.
                                            */
} USB_HUB_PORT_INFO;

/*
 * This data structure holds the information about the BUS that is present and
 * this forms the starting point for the entire topology of the bus.
 */
typedef struct usb_hub_bus_info
{
    UINT8         uBusHandle;             /* Identification of a Bus          */
    OS_THREAD_ID  BusManagerThreadID;     /* Thread ID of the Bus manager     */
    atomic_t      uNumberOfHubEvents;     /* Number of Hub events             */
    BOOLEAN       bDeviceBeingConfigured; /* Is any device being configured   */
    UINT8         uNumberOfConfigRetries; /* Number of configuration retries  */
    UINT32        uDeviceHandle;          /* The root hub device handle       */
    UINT8         uBusSpeed;              /* Speed of the bus                 */
    pUSB_HUB_INFO pRootHubInfo;           /* The pointer to root hub structure*/
    USBHST_URB    deviceResetURB;         /* The device reset URB of this bus */
    USBHST_SETUP_PACKET deviceResetSetup; /* device reset setup packet        */
    SEM_ID        hDeviceResetDoneSem;    /* Semaphore for reset completion   */
    UINT32        uResetCheckCount;       /* Counter for reset check retries  */
    struct usb_hub_bus_info *pNextBus;    /* The pointer to the next bus      */
} USB_HUB_BUS_INFO;

/*
 * This data structure holds the information about a hub and provides the
 * linking info about the devices connected to this hub.
 */
/*
 * NOTE1: The status change bit map obtained in response to the status change
 * interrupt IN request.
 */
/*
 * NOTE2: The pointer to the URB structure that is used for the polling of the
 * interrupt endpoint for status change bit map
 */

struct usb_hub_info
    {
    NODE                 node;
    UINT32               uDeviceHandle;  /* Identification of the hub device  */
    UINT8 *              pCopiedStatus;  /* Copied Status buffer              */
    UINT8 *              pCurrentStatus; /* Current Status buffer             */
    BOOLEAN              bCurrentStatusChanged; /* Current Status buffer changed */
    SEM_ID               hubMutex;       /* Mutex used to protect status      */
    USB_HUB_BUS_INFO *   pBus;           /* Pointer to the bus structure      */
    UINT8                uPowerPerPort;  /* power that can be supplied to port*/
    /* The Endpoint Address where the Interrupt Pipe resides.                 */
    UINT8                uInterruptEndpointNumber;
    USB_HUB_DESCRIPTOR_INFO  HubDescriptor;  /* fields of the hub descriptor  */
    USBHST_URB           StatusChangeURB;/* NOTE2:                            */
    BOOLEAN              bURBSubmitted;  /* Denotes if the URB Was submitted  */
    UINT32               uNumErrors;     /* Error counting                    */
    UINT8                uCurrentTier;   /* USB Tier of this hub              */
    UINT8                uHubTTInfo;     /* describing the TT organization    */
    UINT8                uPortIndex;     /* Index of port in the parent hub   */
    struct usb_hub_info *pParentHub;     /* Parent hub                        */
    UINT32               uTotalStatusLength; /* Total size of status to request */
    UINT32               uActualStatusLength; /* Actual returned size of status */
    USB_HUB_STATUS *     pHubStatus;     /* Hub Status buffer */
    USB_HUB_STATE_TYPE   StateOfHub;     /* State of this Hub              */
    USB_HUB_PORT_INFO*   pParentPort;    /* The parent port that owns this hub */
    USB_HUB_PORT_INFO*   pPortList[1];   /* List of pointers to ports - must be last */
    };

extern STATUS usbHubAddEvent
    (
    pUSB_HUB_INFO   pHub
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __HUB_COMMON_H__ */

/**************************** End of file Hub_Common.h ************************/
