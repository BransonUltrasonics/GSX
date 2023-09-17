/* usbHst.h - USB Host Stack functional interface definition */

/*
 * Copyright (c) 2003, 2006, 2009-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003, 2006, 2009-2015 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
01w,29dec15,whu  Add declaration of usbMsgPost 
01v,16jul14,wyy  Unify APIs of USB message to support error report (VXW6-16596)
01u,04nov13,s_z  make USB work both on new VxBus model and VxBus legacy model
01t,03sep13,x_z  fixed hwif header file path.
01s,01sep13,j_x  Add touch screen
01r,09sep13,j_x  Rename the "LINK" structure to avoid namespace pollution
01q,28apr13,ghs  Add support of USB message (WIND00411952)
01p,12sep12,w_x  Keep root hub driver for backward compatibility (WIND00375595)
01o,26aug12,w_x  Add USBHST_URB_DATA_IN() for convenience (WIND00372215)
01n,19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
01m,16jan13,s_z  Add usbHstClassVendorRequest routine (WIND00392584)
01l,06jul11,ghs  Improve match function and structure (WIND00183500)
01k,24may10,m_y  Add usbHstWaitUrbComplete for usb robustness (WIND00183499)
01j,19may10,j_x  Include "lstLib.h"
01i,18may10,w_x  USB transition to vxbDmaBufLib supporting 64 bit (WIND00193267)
01h,29apr10,pad  Moved extern C statement after include statements.
01g,04mar10,m_y  Modify the structure usbhst_urb and macros to fill urb
                 structure.(WIND00200272)
01f,27jan10,y_l  Add hub device reset check fuction to gUsbdFunctionList
                 (WIND00151882)
01e,13jan10,ghs  vxWorks 6.9 LP64 adapting
01d,25may09,s_z  Add the vendor devices supported list item in structure
                 usbhst_device_driver(WIND00163729)
01c,13jan09,w_x  Added uParentPortIndex/uParentTier in device info (WIND00151472)
01b,07oct06,ami  Changes for USB-vxBus porting
01a,02jun03,nrv  Changing the code to WRS standards
*/

/*
DESCRIPTION

This file contains the application interface to the USB2 Host Stack.

*/

/*
INTERNAL
 *******************************************************************************
 * Filename         : Usbhst.h
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
 * Description      : This file contains the data structures for usbd and other
 *                    modules.
\ *
 ******************************************************************************/

#ifndef __USBHST_H__
#define __USBHST_H__


#include "usbOsal.h"

#include <cacheLib.h>
#include <lstLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************** MACRO DEFINITION **************************************/

/* Device class for vendor specific*/
#define USBHST_VENDOR_SPECIFIC                  0xFF

/* Hub class code*/
#define USBHST_HUB_CLASS                        0x09

/* Hub class code */
#define USBHST_HUB_PROTOCOL_MTT                 0x02

/* Timeout value for URB requests */
#define USBHST_URB_TIMEOUT                      \
        OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(30000)

/* To hold the URB transfer flags (BEGIN) */

/* Flag to specify that a short transfer is acceptable */
#define USBHST_SHORT_TRANSFER_OK                0x01

/*
 * Flag to specify that the isochronous transfer should be started as
 * soon as possible.
 */
#define USBHST_START_ISOCHRONOUS_TRANSFER_ASAP  0x02

/* To hold the URB transfer flags (END) */

/*
 * Default configuration index is used with USBHST_SetConfiguration ()
 * to set the device into address state.
 */
#define USBHST_DEFAULT_CONFIGURATION_INDEX      0xFF

/* Default interface value */
#define USBHST_DEFAULT_INTERFACE_VALUE          0x00

/************* Three different speeds - Start *********************************/

/* Full speed mode */
#define USBHST_FULL_SPEED                       ((UINT8)0x00)

/* Low speed mode */
#define USBHST_LOW_SPEED                        ((UINT8)0x01)

/* High Speed mode */
#define USBHST_HIGH_SPEED                       ((UINT8)0x02)

/* Super Speed mode */
#define USBHST_SUPER_SPEED                      ((UINT8)0x03)

/************* Three different speeds - End ***********************************/

/******************* To hold the USB Transfer Types (BEGIN) *******************/

/* Value to specific an USB Control Transfer */
#define USBHST_CONTROL_TRANSFER                 0x00

/* Value to specific an USB Isochronous Transfer */
#define USBHST_ISOCHRONOUS_TRANSFER             0x01

/* Value to specific an USB Bulk Transfer */
#define USBHST_BULK_TRANSFER                    0x02

/* Value to specific an USB Interrupt Transfer */
#define USBHST_INTERRUPT_TRANSFER               0x03

/******************** To hold the USB Transfer Types (END) ********************/

/************* Different types of Feature Selector - Start ********************/

/* Endpoint halt for the endpoints */
#define USBHST_FEATURE_ENDPOINT_HALT            ((UINT16)0x00)

/* Remote wakeup for the device */
#define USBHST_FEATURE_DEVICE_REMOTE_WAKEUP     ((UINT16)0x01)

/* Test mode feature for the endpoints */
#define USBHST_FEATURE_TEST_MODE                ((UINT16)0x02)

/************** Different types of Feature Selector - End *********************/


/************** Different test selectors values - Start ***********************/

/* Test selector for TEST_J */
#define USBHST_TEST_J                ((UINT8)0x01)

/* Test selector for TEST_K */
#define USBHST_TEST_K                ((UINT8)0x02)

/* Test selector for TEST_SE0_NAK */
#define USBHST_TEST_SE0_NAK          ((UINT8)0x03)

/* Test selector for TEST_PACKET */
#define USBHST_TEST_PACKET           ((UINT8)0x04)

/* Test selector for TEST_FORCE_ENABLE */
#define USBHST_ TEST_FORCE_ENABLE    ((UINT8)0x05)

/************** Different test selectors values  - End ************************/

/************** Different recipient types  - Start ****************************/

/* Recipient is device */
#define USBHST_RECIPIENT_DEVICE                 0x00

/* Recipient is interface */
#define USBHST_RECIPIENT_INTERFACE              0x01

/* Recipient is endpoint */
#define USBHST_RECIPIENT_ENDPOINT               0x02

/* Other recipients */
#define USBHST_RECIPIENT_OTHER                  0x03

/* recipient mask */
#define USBHST_RECIPIENT_MASK                   0x03

/************** Different recipient types  - End ******************************/

/************** Different descriptor types  - Start ***************************/

/* Defines device descriptor */
#define USBHST_DEVICE_DESC                  ((UINT8) 0x01)

/* Defines configuration descriptor */
#define USBHST_CONFIG_DESC                  ((UINT8) 0x02)

/* Defines string descriptor */
#define USBHST_STRING_DESC                  ((UINT8) 0x03)

/* Defines interface descriptor */
#define USBHST_INTERFACE_DESC               ((UINT8) 0x04)

/* Defines endpoint descriptor */
#define USBHST_ENDPOINT_DESC                ((UINT8) 0x05)

/* defines the device qualifier */
#define USBHST_DEVICE_QUALIFIER_DESC        ((UINT8) 0x06)

/* defines the other speed configuration descriptor */
#define USBHST_OTHER_SPEED_CONFIG_DESC      ((UINT8) 0x07)

/* defines the interface power descriptor */
#define USBHST_INTERFACE_POWER_DESC         ((UINT8) 0x08)

/* defines the OTG descriptor */
#define USBHST_OTG_DESC                     ((UINT8) 0x09)

/* defines the DEBUG descriptor */
#define USBHST_DEBUG_DESC                   ((UINT8) 0x0A)

/* defines the IAD (INTERFACE ASSOCIATION) descriptor */
#define USBHST_IAD_DESC                     ((UINT8) 0x0B)

/* defines the BOS (Binary Device Object Store) descriptor */
#define USBHST_BOS_DESC                     ((UINT8) 0x0F)

/* defines the DEVICE CAPABILITY descriptor */
#define USBHST_DEV_CAP_DESC                 ((UINT8) 0x10)

/* defines the SUPERSPEED_USB_ENDPOINT_COMPANION descriptor */
#define USBHST_SS_USB_EP_COMPANION_DESC     ((UINT8) 0x30)

/************** Different descriptor types  - End *****************************/

/************** Different Standard Request codes - Start **********************/

#define USBHST_REQ_GET_STATUS               ((UINT8) 0x00)
#define USBHST_REQ_CLEAR_FEATURE            ((UINT8) 0x01)
#define USBHST_REQ_SET_FEATURE              ((UINT8) 0x03)
#define USBHST_REQ_SET_ADDRESS              ((UINT8) 0x05)
#define USBHST_REQ_GET_DESCRIPTOR           ((UINT8) 0x06)
#define USBHST_REQ_SET_DESCRIPTOR           ((UINT8) 0x07)
#define USBHST_REQ_GET_CONFIGURATION        ((UINT8) 0x08)
#define USBHST_REQ_SET_CONFIGURATION        ((UINT8) 0x09)
#define USBHST_REQ_GET_INTERFACE            ((UINT8) 0x0A)
#define USBHST_REQ_SET_INTERFACE            ((UINT8) 0x0B)
#define USBHST_REQ_SYNCH_FRAME              ((UINT8) 0x0C)
#define USBHST_REQ_SET_SEL                  ((UINT8) 0x30)
#define USBHST_REQ_SET_ISOCH_DELAY          ((UINT8) 0x31)

/************** Different Standard Request codes - End ************************/

/************** Different USB status codes - Start ****************************/

#define USBHST_STATUS                       INT8
#define USBHST_SUCCESS                      ((INT8)0)
#define USBHST_TRANSFER_COMPLETED           ((INT8)1)
#define USBHST_FAILURE                      ((INT8)-1)
#define USBHST_MEMORY_NOT_ALLOCATED         ((INT8)-2)
#define USBHST_INSUFFICIENT_BANDWIDTH       ((INT8)-3)
#define USBHST_INSUFFICIENT_MEMORY          ((INT8)-4)
#define USBHST_INSUFFICIENT_RESOURCE        ((INT8)-5)
#define USBHST_INVALID_REQUEST              ((INT8)-6)
#define USBHST_INVALID_PARAMETER            ((INT8)-7)
#define USBHST_STALL_ERROR                  ((INT8)-8)
#define USBHST_DEVICE_NOT_RESPONDING_ERROR  ((INT8)-9)
#define USBHST_DATA_OVERRUN_ERROR           ((INT8)-10)
#define USBHST_DATA_UNDERRUN_ERROR          ((INT8)-11)
#define USBHST_BUFFER_OVERRUN_ERROR         ((INT8)-12)
#define USBHST_BUFFER_UNDERRUN_ERROR        ((INT8)-13)
#define USBHST_TRANSFER_CANCELLED           ((INT8)-14)
#define USBHST_TIMEOUT                      ((INT8)-15)
#define USBHST_BAD_START_OF_FRAME           ((INT8)-16)

/************** Different USB status codes - End ******************************/

/************** USB message code, host class driver definition - Start ************/

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
 * [19-16]  Sub-module Type 
 *          Defined by every modules, value [0x0] is reserved.
 *          For GEN2 Host Class Driver Define (module Type: [0x5]):
 *              1 - legacy (GEN1 Reserved) 2 - Hub, 3 - Helper, 4- Keyboard,
 *              5 - Mouse, 6 - Storage, 7 - Network, 8 - Printer, 9 - Serial,
 *              10 - PHDC,
 *          HCD Defination (module Type: [0x1]):
 *              1 - USBD, 2 - EHCD, 3 - UHCD, 4 - OHCD, 5 - SynopsysHCD, 
 *              6 - MhdrcHCD, 7 - XHCD
 *
 * [15-0]   Message Values
 *          Defined by every modules, value [0x0] is reserved.
 *          For GEN2 Host Class Driver Define:
 *              1 - New Device Ready, 2 - Device Removed.
 *              3 - Unrecoveriable ERROR
 *          HCD message difination:
 *              1 - New Device Ready, 2 - Device Removed.
 *              3 - Unrecoveriable ERROR
 *
 */

/* USB_MSG_CODE field - SysOrUser, bits from 31 to 24  */

#define USBMSG_USBSYS                               0x21
#define USBMSG_USBUSER                              0x81

/* USB_MSG_CODE field - ModuleType, bits from 23 to 20 */

#define USBMSG_HCD                                  0x1
#define USBMSG_TCD                                  0x2
#define USBMSG_OCD                                  0x3
#define USBMSG_USBD                                 0x4
#define USBMSG_CLASS                                0x5
#define USBMSG_FUNC                                 0x6
#define USBMSG_TOOL                                 0x7

/* USB_MSG_CODE field - Sub-module of USBMSG_CLASS, bits from 19 to 16 */

#define USBMSG_HST_LEGACY                           0x1
#define USBMSG_HST_HUB                              0x2
#define USBMSG_HST_HLP                              0x3
#define USBMSG_HST_KBD                              0x4
#define USBMSG_HST_MSE                              0x5
#define USBMSG_HST_MSC                              0x6
#define USBMSG_HST_END                              0x7
#define USBMSG_HST_PRN                              0x8
#define USBMSG_HST_SER                              0x9
#define USBMSG_HST_PHDC                             0xA
#define USBMSG_HST_TS                               0xB

/* USB_MSG_CODE field - Sub-module of USBMSG_HCD, bits from 19 to 16 */

#define USBMSG_HCD_USBD                             0x1
#define USBMSG_HCD_EHCD                             0x2
#define USBMSG_HCD_UHCD                             0x3
#define USBMSG_HCD_OHCD                             0x4
#define USBMSG_HCD_SHCD                             0x5
#define USBMSG_HCD_MHCD                             0x6
#define USBMSG_HCD_XHCD                             0x7

/* USB_MSG_CODE field - Message Values of USBMSG_CLASS, bits from 15 to 0 */

#define GEN2_DEVREADY                               0x1
#define GEN2_DEVREMOVE                              0x2
#define GEN2_DEVATTACH                              0x3
#define GEN2_DEVDETACH                              0x4
#define GEN2_LUNMOUNTED                             0x10
#define GEN2_LUNUNMOUNT                             0x11
#define GEN2_ENDBIND                                0x20
#define GEN2_ENDUNBIND                              0x21
#define GEN2_ERROR                                  0x30

/* USB_MSG_CODE field - Message Values of USBMSG_HCD, bits from 15 to 0 */

#define HCD_READY                                   0x1
#define HCD_REMOVE                                  0x2
#define HCD_ERROR                                   0x3


/* Macro definition of USB message code */

#define USBMSG_CODE(SysOrUser, ModuleType, Module, Msg)     \
    (((((UINT32)SysOrUser) & 0xFF) << 24) | ((((UINT32)ModuleType) & 0xF) << 20) \
    | ((((UINT32)Module) & 0xF) << 16) | ((((UINT32)Msg) & 0xFFFF) << 0))

#define USBMSG_HST(Mod,Msg)                         \
    USBMSG_CODE(USBMSG_USBSYS, USBMSG_CLASS, Mod, Msg)

#define USBMSG_HC(Mod,Msg)                         \
    USBMSG_CODE(USBMSG_USBSYS, USBMSG_HCD, Mod, Msg)


/* USB_MSG_CODE for all class driver */

#define USBMSG_GEN2_KBD_DEVREADY    USBMSG_HST(USBMSG_HST_KBD, GEN2_DEVREADY)
#define USBMSG_GEN2_MSE_DEVREADY    USBMSG_HST(USBMSG_HST_MSE, GEN2_DEVREADY)
#define USBMSG_GEN2_PRN_DEVREADY    USBMSG_HST(USBMSG_HST_PRN, GEN2_DEVREADY)
#define USBMSG_GEN2_SER_DEVREADY    USBMSG_HST(USBMSG_HST_SER, GEN2_DEVREADY)
#define USBMSG_GEN2_PHDC_DEVREADY   USBMSG_HST(USBMSG_HST_PHDC, GEN2_DEVREADY)

#define USBMSG_GEN2_MSE_DEVREMOVED  USBMSG_HST(USBMSG_HST_MSE, GEN2_DEVREMOVE)
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

#define USBMSG_GEN2_KBD_ERROR       USBMSG_HST(USBMSG_HST_KBD, GEN2_ERROR)
#define USBMSG_GEN2_MSE_ERROR       USBMSG_HST(USBMSG_HST_MSE, GEN2_ERROR)
#define USBMSG_GEN2_PRN_ERROR       USBMSG_HST(USBMSG_HST_PRN, GEN2_ERROR)
#define USBMSG_GEN2_SER_ERROR       USBMSG_HST(USBMSG_HST_SER, GEN2_ERROR)
#define USBMSG_GEN2_MSC_ERROR       USBMSG_HST(USBMSG_HST_MSC, GEN2_ERROR)
#define USBMSG_GEN2_END_ERROR       USBMSG_HST(USBMSG_HST_END, GEN2_ERROR)

/* USB_MSG_CODE for all host controller driver */

#define USBMSG_HCD_EHCD_READY       USBMSG_HC(USBMSG_HCD_EHCD, HCD_READY)
#define USBMSG_HCD_OHCD_READY       USBMSG_HC(USBMSG_HCD_OHCD, HCD_READY)
#define USBMSG_HCD_UHCD_READY       USBMSG_HC(USBMSG_HCD_UHCD, HCD_READY)
#define USBMSG_HCD_SHCD_READY       USBMSG_HC(USBMSG_HCD_SHCD, HCD_READY)
#define USBMSG_HCD_MHCD_READY       USBMSG_HC(USBMSG_HCD_MHCD, HCD_READY)
#define USBMSG_HCD_XHCD_READY       USBMSG_HC(USBMSG_HCD_XHCD, HCD_READY)

#define USBMSG_HCD_EHCD_REMOVE      USBMSG_HC(USBMSG_HCD_EHCD, HCD_REMOVE)
#define USBMSG_HCD_OHCD_REMOVE      USBMSG_HC(USBMSG_HCD_OHCD, HCD_REMOVE)
#define USBMSG_HCD_UHCD_REMOVE      USBMSG_HC(USBMSG_HCD_UHCD, HCD_REMOVE)
#define USBMSG_HCD_SHCD_REMOVE      USBMSG_HC(USBMSG_HCD_SHCD, HCD_REMOVE)
#define USBMSG_HCD_MHCD_REMOVE      USBMSG_HC(USBMSG_HCD_MHCD, HCD_REMOVE)
#define USBMSG_HCD_XHCD_REMOVE      USBMSG_HC(USBMSG_HCD_XHCD, HCD_REMOVE)

#define USBMSG_HCD_USBD_ERROR       USBMSG_HC(USBMSG_HCD_USBD, HCD_ERROR)
#define USBMSG_HCD_EHCD_ERROR       USBMSG_HC(USBMSG_HCD_EHCD, HCD_ERROR)
#define USBMSG_HCD_OHCD_ERROR       USBMSG_HC(USBMSG_HCD_OHCD, HCD_ERROR)
#define USBMSG_HCD_UHCD_ERROR       USBMSG_HC(USBMSG_HCD_UHCD, HCD_ERROR)
#define USBMSG_HCD_SHCD_ERROR       USBMSG_HC(USBMSG_HCD_SHCD, HCD_ERROR)
#define USBMSG_HCD_MHCD_ERROR       USBMSG_HC(USBMSG_HCD_MHCD, HCD_ERROR)
#define USBMSG_HCD_XHCD_ERROR       USBMSG_HC(USBMSG_HCD_XHCD, HCD_ERROR)

/* flags value to add callback */

#define USBMSG_FLAG_CALLBACK_ASYN                   (1 << 0)

/************** USB message code, host class driver definition - End ************/

/*******************************************************************************
 * Macro Name  : USBHST_FILL_SETUP_PACKET
 * Description : Initialize the SETUP_PACKET structure with the requisite
 *               parameter.
 * Parameters  : pSetup         OUT Setup packet to be filled.
 *               uRequestType   IN  Bitmap for request characteristics.
 *               uRequest       IN  Standard request code.
 *               uValue         IN  Word size value field for standard request.
 *               uIndex         IN  Word size index field for standard request.
 *               uSize          IN  Number of bytes to transfer if there is
 *                                  data stage.
 * Return Type : None
 ******************************************************************************/
#define USBHST_FILL_SETUP_PACKET(pSetup,                                     \
                                 uRequestType,                               \
                                 uRequest,                                   \
                                 uValue,                                     \
                                 uIndex,                                     \
                                 uSize)                                      \
{                                                                            \
    /* Store bitmap for request characteristics in the setup packet */       \
    (pSetup)->bmRequestType   = (UINT8)(uRequestType);                       \
                                                                             \
    /* Store the request in the setup packet */                              \
    (pSetup)->bRequest        = (UINT8)(uRequest);                           \
                                                                             \
    /* Store word size value field for request in the setup packet */        \
    (pSetup)->wValue          = OS_UINT16_CPU_TO_LE((UINT16)(uValue));       \
                                                                             \
    /* Store word size index field for request  in the setup packet */       \
    (pSetup)->wIndex          = OS_UINT16_CPU_TO_LE((UINT16)(uIndex));       \
                                                                             \
    /* Store number of bytes to transfer if there is data stage */           \
    (pSetup)->wLength         = OS_UINT16_CPU_TO_LE((UINT16)(uSize));        \
}

/*******************************************************************************
 * Macro Name  : USBHST_FILL_CONTROL_URB
 * Description : Initialize the URB with requisite parameters for control
 *               transfer.
 * Parameters  : Urb                   OUT Pointer to URB.
 *               Device                IN  Handle to the Device.
 *               EndPointAddress       IN  End Point Address for which URB is
 *                                          issued.
 *               TransferBuffer        OUT Pointer to the buffer passed as
 *                                          parameter.
 *               TransferLength        IN  Length of the data transfer
 *                                          requested.
 *               TransferFlags         IN  Flags for data transfer.
 *               SetupPacket           IN  Pointer to setup packet.
 *               Callback              IN  Pointer to the callback function.
 *               Context               IN  Class driver specific URB information
 *               Status                OUT Status of the request.
 * Return Type : None
 ******************************************************************************/
#define USBHST_FILL_CONTROL_URB(Urb,                                       \
                                Device,                                    \
                                EndPointAddress,                           \
                                TransferBuffer,                            \
                                TransferLength,                            \
                                TransferFlags,                             \
                                SetupPacket,                               \
                                Callback,                                  \
                                Context,                                   \
                                Status)                                    \
{                                                                          \
    /* Store the handle to the Device  */                                  \
    (Urb)->hDevice               = (Device);                               \
                                                                           \
    /* Store the end Point Address for which Urb is issued */              \
    (Urb)->uEndPointAddress      = (EndPointAddress);                      \
                                                                           \
    /* Store pointer to the buffer passed as parameter */                  \
    (Urb)->pTransferBuffer       = (TransferBuffer);                       \
                                                                           \
    /* Store length of the data transfer requested */                      \
    (Urb)->uTransferLength       = (TransferLength);                       \
                                                                           \
    /* Store type of the data transfer requested */                        \
    (Urb)->uTransferType       = (USBHST_CONTROL_TRANSFER);                \
                                                                           \
    /* Store flags for data transfer */                                    \
    (Urb)->uTransferFlags        = (TransferFlags);                        \
                                                                           \
    /* Store pointer to setup packet */                                    \
    (Urb)->pTransferSpecificData = (SetupPacket);                          \
                                                                           \
    /* Store the pointer to the callback function */                       \
    (Urb)->pfUserCallback            = (Callback);                         \
                                                                           \
    /* Store the pointer of the common callback function */                \
    (Urb)->pfCallback                  = (usbHstCommonCallback);           \
                                                                           \
    /* Store Class driver specific URB information */                      \
    (Urb)->pContext              = (Context);                              \
                                                                           \
    /* Store the status of the request */                                  \
    (Urb)->nStatus               = (Status);                               \
}

/*******************************************************************************
 * Macro Name  : USBHST_FILL_BULK_URB
 * Description : Initialize the URB with requisite parameters for bulk transfer.
 * Parameters  : Urb                   OUT Pointer to URB
 *               Device                IN  Handle to the Device
 *               EndPointAddress       IN  End Point Address for which URB is
 *                                          issued.
 *               TransferBuffer        OUT Pointer to the buffer passed as
 *                                          parameter.
 *               TransferLength        IN  Length of the data transfer
 *                                          requested.
 *               TransferFlags         IN  Flags for data transfer.
 *               Callback              IN  Pointer to the callback function.
 *               Context               IN  Class driver specific URB information
 *               Status                OUT Status of the request.
 * Return Type : None
 ******************************************************************************/
#define USBHST_FILL_BULK_URB(Urb,                                          \
                             Device,                                       \
                             EndPointAddress,                              \
                             TransferBuffer,                               \
                             TransferLength,                               \
                             TransferFlags,                                \
                             Callback,                                     \
                             Context,                                      \
                             Status)                                       \
{                                                                          \
    /* Store the handle to the Device  */                                  \
    (Urb)->hDevice               = (Device);                               \
                                                                           \
    /* Store the end Point Address for which URB is issued */              \
    (Urb)->uEndPointAddress      = (EndPointAddress);                      \
                                                                           \
    /* Store the end Point Address for which URB is issued */              \
    (Urb)->uStreamId             = (0);                                    \
                                                                           \
    /* Store pointer to the buffer passed as parameter */                  \
    (Urb)->pTransferBuffer       = (TransferBuffer);                       \
                                                                           \
    /* Store length of the data transfer requested */                      \
    (Urb)->uTransferLength       = (TransferLength);                       \
                                                                           \
    /* Store type of the data transfer requested */                        \
    (Urb)->uTransferType       = (USBHST_BULK_TRANSFER);                   \
                                                                           \
    /* Store flags for data transfer */                                    \
    (Urb)->uTransferFlags        = (TransferFlags);                        \
                                                                           \
    /* Store pointer to setup packet */                                    \
    (Urb)->pTransferSpecificData = NULL;                                   \
                                                                           \
    /* Store the pointer to the callback function */                       \
    (Urb)->pfUserCallback            = (Callback);                         \
                                                                           \
    /* Store the pointer of the common callback function */                \
    (Urb)->pfCallback                  = (usbHstCommonCallback);           \
                                                                           \
    /* Store Class driver specific URB information */                      \
    (Urb)->pContext              = (Context);                              \
                                                                           \
    /* Store the status of the request */                                  \
    (Urb)->nStatus               = (Status);                               \
}

/*******************************************************************************
* Macro Name  : USBHST_FILL_BULK_STREAM_URB
* Description : Initialize the URB with requisite parameters for bulk transfer.
* Parameters  : Urb                   OUT Pointer to URB
*               Device                IN  Handle to the Device
*               EndPointAddress       IN  End Point Address for which URB is
*                                          issued.
*               StreamId              IN  Stream ID for this Transfer (USB3)
*               TransferBuffer        OUT Pointer to the buffer passed as
*                                          parameter.
*               TransferLength        IN  Length of the data transfer
*                                          requested.
*               TransferFlags         IN  Flags for data transfer.
*               Callback              IN  Pointer to the callback function.
*               Context               IN  Class driver specific URB information
*               Status                OUT Status of the request.
* Return Type : None
******************************************************************************/
#define USBHST_FILL_BULK_STREAM_URB(Urb,                                   \
                             Device,                                       \
                             EndPointAddress,                              \
                             StreamId,                                     \
                             TransferBuffer,                               \
                             TransferLength,                               \
                             TransferFlags,                                \
                             Callback,                                     \
                             Context,                                      \
                             Status)                                       \
{                                                                          \
    /* Store the handle to the Device  */                                  \
    (Urb)->hDevice               = (Device);                               \
                                                                           \
    /* Store the end Point Address for which URB is issued */              \
    (Urb)->uEndPointAddress      = (EndPointAddress);                      \
                                                                           \
    /* Store the end Point Address for which URB is issued */              \
    (Urb)->uStreamId             = (StreamId);                             \
                                                                           \
    /* Store pointer to the buffer passed as parameter */                  \
    (Urb)->pTransferBuffer       = (TransferBuffer);                       \
                                                                           \
    /* Store length of the data transfer requested */                      \
    (Urb)->uTransferLength       = (TransferLength);                       \
                                                                           \
    /* Store type of the data transfer requested */                        \
    (Urb)->uTransferType       = (USBHST_BULK_TRANSFER);                   \
                                                                           \
    /* Store flags for data transfer */                                    \
    (Urb)->uTransferFlags        = (TransferFlags);                        \
                                                                           \
    /* Store pointer to setup packet */                                    \
    (Urb)->pTransferSpecificData = NULL;                                   \
                                                                           \
    /* Store the pointer to the callback function */                       \
    (Urb)->pfUserCallback            = (Callback);                         \
                                                                           \
    /* Store the pointer of the common callback function */                \
    (Urb)->pfCallback                  = (usbHstCommonCallback);           \
                                                                           \
    /* Store Class driver specific URB information */                      \
    (Urb)->pContext              = (Context);                              \
                                                                           \
    /* Store the status of the request */                                  \
    (Urb)->nStatus               = (Status);                               \
}

/*******************************************************************************
 * Macro Name  : USBHST_FILL_INTERRUPT_URB
 * Description : Initialize the URB with requisite parameters for interrupt
 *               transfer.
 * Parameters  : Urb                   OUT Pointer to URB
 *               Device                IN  Handle to the Device
 *               EndPointAddress       IN  End Point Address for which URB is
 *                                          issued.
 *               TransferBuffer        OUT Pointer to the buffer passed as
 *                                          parameter.
 *               TransferLength        IN  Length of the data transfer
 *                                          requested.
 *               TransferFlags         IN  Flags for data transfer.
 *               Callback              IN  Pointer to the callback function.
 *               Context               IN  Class driver specific URB information
 *               Status                OUT Status of the request.
 * Return Type : None
*******************************************************************************/
#define USBHST_FILL_INTERRUPT_URB(Urb,                                     \
                                  Device,                                  \
                                  EndPointAddress,                         \
                                  TransferBuffer,                          \
                                  TransferLength,                          \
                                  TransferFlags,                           \
                                  Callback,                                \
                                  Context,                                 \
                                  Status)                                  \
{                                                                          \
    /* Store the handle to the Device  */                                  \
    (Urb)->hDevice               = (Device);                               \
                                                                           \
    /* Store the end Point Address for which URB is issued */              \
    (Urb)->uEndPointAddress      = (EndPointAddress);                      \
                                                                           \
    /* Store pointer to the buffer passed as parameter */                  \
    (Urb)->pTransferBuffer       = (TransferBuffer);                       \
                                                                           \
    /* Store length of the data transfer requested */                      \
    (Urb)->uTransferLength       = (TransferLength);                       \
                                                                           \
    /* Store type of the data transfer requested */                        \
    (Urb)->uTransferType       = (USBHST_INTERRUPT_TRANSFER);              \
                                                                           \
    /* Store flags for data transfer */                                    \
    (Urb)->uTransferFlags        = (TransferFlags);                        \
                                                                           \
    /* Store pointer to setup packet */                                    \
    (Urb)->pTransferSpecificData = NULL;                                   \
                                                                           \
    /* Store the pointer to the callback function */                       \
    (Urb)->pfUserCallback            = (Callback);                         \
                                                                           \
    /* Store the pointer of the common callback function */                \
    (Urb)->pfCallback                  = (usbHstCommonCallback);           \
                                                                           \
    /* Store Class driver specific (Urb) information */                    \
    (Urb)->pContext              = (Context);                              \
                                                                           \
    /* Store the status of the request */                                  \
    (Urb)->nStatus               = (Status);                               \
}

/*******************************************************************************
 * Macro Name  : USBHST_FILL_ISOCHRONOUS_URB
 * Description : Initialize the URB with requisite parameters for isochronous
 *               transfer.
 *               Device                IN  Handle to the Device
 *               EndPointAddress       IN  End Point Address for which URB is
 *                                         issued.
 *               TransferBuffer        OUT Pointer to the buffer passed as
 *                                         parameter.
 *               TransferLength        IN  Length of the data transfer
 *                                         requested.
 *               TransferFlags         IN  Flags for data transfer.
 *               StartFrame            IN  Start of Frame information.
 *               NumberOfPackets       IN  Number of packets.
 *               Callback              IN  Pointer to the callback function.
 *               Context               IN  Class driver specific URB information
 *               Status                OUT Status of the request.
 * Return Type : None
 ******************************************************************************/
#define USBHST_FILL_ISOCHRONOUS_URB(Urb,                                   \
                                    Device,                                \
                                    EndPointAddress,                       \
                                    TransferBuffer,                        \
                                    TransferLength,                        \
                                    TransferFlags,                         \
                                    StartFrame,                            \
                                    NumberOfPackets,                       \
                                    IsoPacketDesc,                         \
                                    Callback,                              \
                                    Context,                               \
                                    Status)                                \
{                                                                          \
    /* Store the handle to the Device  */                                  \
    (Urb)->hDevice               = (Device);                               \
                                                                           \
    /* Store the end Point Address for which URB is issued */              \
    (Urb)->uEndPointAddress      = (EndPointAddress);                      \
                                                                           \
    /* Store pointer to the buffer passed as parameter */                  \
    (Urb)->pTransferBuffer       = (TransferBuffer);                       \
                                                                           \
    /* Store length of the data transfer requested */                      \
    (Urb)->uTransferLength       = (TransferLength);                       \
                                                                           \
    /* Store type of the data transfer requested */                        \
    (Urb)->uTransferType       = (USBHST_ISOCHRONOUS_TRANSFER);            \
                                                                           \
    /* Store flags for data transfer */                                    \
    (Urb)->uTransferFlags        = (TransferFlags);                        \
                                                                           \
    /* Store the start of Frame information */                             \
    (Urb)->uStartFrame           = (StartFrame);                           \
                                                                           \
    /* Store the number of packets */                                      \
    (Urb)->uNumberOfPackets      = (NumberOfPackets);                      \
                                                                           \
    /* Store pointer to setup packet */                                    \
    (Urb)->pTransferSpecificData = (IsoPacketDesc);                        \
                                                                           \
    /* Store the pointer to the callback function */                       \
    (Urb)->pfUserCallback            = (Callback);                         \
                                                                           \
    /* Store the pointer of the common callback function */                \
    (Urb)->pfCallback                  = (usbHstCommonCallback);           \
                                                                           \
    /* Store Class driver specific URB information */                      \
    (Urb)->pContext              = (Context);                              \
                                                                           \
    /* Store the status of the request */                                  \
    (Urb)->nStatus               = (Status);                               \
}

/* Check if the URB data transfer is IN direction */

#define USBHST_URB_DATA_IN(pURB) \
    ((((pURB)->uEndPointAddress & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_IN) ||\
     (((pURB)->uTransferType == USB_ATTR_CONTROL) && \
      ((((pUSBHST_SETUP_PACKET)((pURB)->pTransferSpecificData))->bmRequestType \
          & USB_RT_DIRECTION_MASK) == USB_RT_DEV_TO_HOST)))

/* Header used to identify the descriptor type and length */
typedef struct  usbhst_descriptor_header
    {
    UINT8       uLength;         /* Size of this descriptor in bytes         */
    UINT8       uDescriptorType; /* Descriptor Type                          */
    } __attribute__((packed)) USBHST_DESCRIPTOR_HEADER,
      *pUSBHST_DESCRIPTOR_HEADER;


/* Structure to hold the Standard Device Descriptor */
typedef struct usbhst_device_descriptor
    {
    UINT8  bLength;             /* Size of this descriptor                   */
    UINT8  bDescriptorType;     /* DEVICE Descriptor Type                    */
    UINT16 bcdUSB;              /* BCD USB Spec Release Number               */
    UINT8  bDeviceClass;        /* Class Class code                          */
    UINT8  bDeviceSubClass;     /* SubClass Subclass code                    */
    UINT8  bDeviceProtocol;     /* Protocol code                             */
    UINT8  bMaxPacketSize;      /* Max packet size for endpoint zero         */
    UINT16 idVendor;            /* Vendor ID of this interface               */
    UINT16 idProduct;           /* Product ID of this interface              */
    UINT16 bcdDevice;           /* BCD Device release number                 */
    UINT8  iManufacturer;       /* Manufacturer string descriptor index      */
    UINT8  iProduct;            /* Product string descriptor index           */
    UINT8  iSerialNumber;       /* Dev serial number string descriptor index */
    UINT8  bNumConfigurations;  /* Num of possible configurations            */
    }__attribute__((packed)) USBHST_DEVICE_DESCRIPTOR,
    *pUSBHST_DEVICE_DESCRIPTOR;

/* Structure to hold the device qualifier */
typedef struct usbhst_device_qualifier
    {
    UINT8  bLength;             /* Size of this descriptor in bytes          */
    UINT8  bDescriptorType;     /* Interface Descriptor Type                 */
    UINT16 bcdUSB;              /* BCD USB Specification release number      */
    UINT8  bDeviceClass;        /* Device class code                         */
    UINT8  bDeviceSubClass;     /* Device subClass code                      */
    UINT8  bDeviceProtocol;     /* Protocol code                             */
    UINT8  bMaxPacketSize;      /* Max packet size for endpoint zero         */
    UINT8  bNumConfigurations;  /* Num of possible configurations            */
    UINT8  bReserved;           /* reserved for future use                   */
    }__attribute__((packed)) USBHST_DEVICE_QUALIFIER,
    *pUSBHST_DEVICE_QUALIFIER;


/* Structure to hold Standard Configuration Descriptor */
typedef struct usbhst_config_descriptor
    {
    UINT8  bLength;             /* Size of this descriptor in bytes          */
    UINT8  bDescriptorType;     /* CONFIGURATION Descriptor Type             */
    UINT16 wTotalLength;        /* Configuration data length                 */
    UINT8  bNumInterfaces;      /* Number of interfaces supported            */
    UINT8  bConfigurationValue; /* SetConfiguration() function argument      */
    UINT8  iConfiguration;      /* Configuration string descriptor           */
    UINT8  bmAttributes;        /* Bitmap configuration characteristics      */
    UINT8  MaxPower; /* Max pwr consumption (mA) of operational device on bus*/
    }__attribute__((packed)) USBHST_CONFIG_DESCRIPTOR,
    *pUSBHST_CONFIG_DESCRIPTOR;

/* Structure to hold Standard Interface Descriptor */
typedef struct usbhst_interface_descriptor
    {
    UINT8  bLength;             /* Size of this descriptor in bytes          */
    UINT8  bDescriptorType;     /* Interface Descriptor Type                 */
    UINT8  bInterfaceNumber;    /* Zero-based interface number               */
    UINT8  bAlternateSetting;   /* Alternate setting for interface           */
    UINT8  bNumEndpoints;       /* Num of endpoints (excluding endpoint zero)*/
    UINT8  bInterfaceClass;     /* Class code of this interface              */
    UINT8  bInterfaceSubClass;  /* Subclass code of this interface           */
    UINT8  bInterfaceProtocol;  /* Interface protocol code                   */
    UINT8  iInterface;          /* Interface string descriptor               */
    }__attribute__((packed)) USBHST_INTERFACE_DESCRIPTOR,
    *pUSBHST_INTERFACE_DESCRIPTOR;

/* Structure to hold Standard Endpoint Descriptor */
typedef struct usbhst_endpoint_descriptor
    {
    UINT8  bLength;             /* Descriptor length in bytes                */
    UINT8  bDescriptorType;     /* Endpoint Descriptor type                  */
    UINT8  bEndpointAddress;    /* Endpoint address on the USB device        */
    UINT8  bmAttributes;        /* Endpoint transfer, sych & usage types attr*/
    UINT16 wMaxPacketSize;      /* Endpoint max packet size                  */
    UINT8  bInterval;           /* Polling interval for endpoint data xfer   */
    }__attribute__((packed)) USBHST_ENDPOINT_DESCRIPTOR,
    *pUSBHST_ENDPOINT_DESCRIPTOR;

/* Structure to hold Standard Endpoint COMPANION Descriptor */
typedef struct usbhst_endpoint_companion_descriptor
    {
    UINT8  bLength;             /* Descriptor length in bytes                */
    UINT8  bDescriptorType;     /* Endpoint Descriptor type                  */
    UINT8  bMaxBurst;           /* Max # packets the EP can TX/RX in a burst */
    UINT8  bmAttributes;        /* Endpoint MaxStreams (Bulk)/Mult(ISOCH)    */
    UINT16 wBytesPerInterval;   /* Total # bytes this EP will XFER every SI  */
    }__attribute__((packed)) USBHST_ENDPOINT_COMPANION_DESCRIPTOR,
    *pUSBHST_ENDPOINT_COMPANION_DESCRIPTOR;

#define USBHST_SS_EP_COMPANION_ATTR_MULT_MASK (0x3)
#define USBHST_SS_EP_COMPANION_ATTR_MAX_STREAMS_MASK (0x1F)

/* Structure to hold the String Descriptor */
typedef struct usbhst_string_descriptor
    {
    UINT8  bLength;              /* Descriptor length in bytes                */
    UINT8  bDescriptorType;      /* Endpoint Descriptor type                  */
    UINT16 wUnicodeString[1];    /* UNICODE encoded string                    */
    }__attribute__((packed)) USBHST_STRING_DESCRIPTOR,
    *pUSBHST_STRING_DESCRIPTOR;

/* Notify type of device */
#define USBD_MATCH_NOTIFY_AS_INTERFACE                  (1 << 0)
#define USBD_MATCH_NOTIFY_AS_DEVICE                     (1 << 1)

/* matching rules */
#define USBD_MATCH_ID_VENDORID                          (1 << 0)
#define USBD_MATCH_ID_PRODUCTID                         (1 << 1)
#define USBD_MATCH_ID_BCDDEVICE                         (1 << 2)
#define USBD_MATCH_ID_DEVICE_CLASS                      (1 << 3)
#define USBD_MATCH_ID_DEVICE_SUBCLASS                   (1 << 4)
#define USBD_MATCH_ID_DEVICE_PROTOCOL                   (1 << 5)
#define USBD_MATCH_ID_INTERFACE_CLASS                   (1 << 6)
#define USBD_MATCH_ID_INTERFACE_SUBCLASS                (1 << 7)
#define USBD_MATCH_ID_INTERFACE_PROTOCOL                (1 << 8)

/* Structure of the devices information can be used by device list */

typedef struct usbhst_device_id
    {
    USB_LINK    link;           /* link list header                           */

    UINT32  uNotifyType;        /* notify type for this device                */

    UINT16  uVendorID;          /* Vendor Id of the device                    */
    UINT16  uProductID;         /* Product Id for device                      */

    UINT16  uDriverBCDLow;      /* used for driver to store minimum BCD value */
    UINT16  uDriverBCDHigh;     /* used for driver to store maximum BCD value */
    UINT16  uBCDDevice;         /* device release version binary coded decimal*/

    UINT8   uDeviceClass;       /* Class code of the device                   */
    UINT8   uDeviceSubClass;    /* Sub class code of the device               */
    UINT8   uDeviceProtocol;    /* Protocol code of the device                */

    UINT8   bInterfaceClass;    /* Class code of this interface               */
    UINT8   bInterfaceSubClass; /* Subclass code of this interface            */
    UINT8   bInterfaceProtocol; /* Interface protocol code                    */

    UINT32  uMatchFlag;         /* Match flags for device info                */

    VOID *  pUserData;          /* Private data for driver, usbd do not use   */
    } USBHST_DEVICE_ID, *pUSBHST_DEVICE_ID;

/*
 * This structure is used to store the pointers to entry points and class
 * driver information
 */
typedef struct usbhst_device_driver
    {
#ifdef _WRS_CONFIG_VXBUS_LEGACY
    struct  vxbDevRegInfo vxbDriverInfo;  /* to hold DRIVER_REGISTRATION */
                                             /* information */
#else
    char   drvName[MAX_DRV_NAME_LEN+1];
#endif /* _WRS_CONFIG_VXBUS_LEGACY */
    BOOL    bFlagVendorSpecific;  /* Vendor Specific or class specific flag  */
    UINT16  uVendorIDorClass;     /* Vendor ID (if vendor) or Class Code     */
    UINT16  uProductIDorSubClass; /* Dev ID (if vendor) or SubClass Code     */
    UINT16  uBCDUSBorProtocol;    /* DevRel num (if vendor) or Protocol code */

    VOID *  pDeviceList;          /* Pointer to list of supported devices */
    /*
     * Function registered as to be called when a matching interface/device
     * is connected
     */
    USBHST_STATUS  (*addDevice)   (UINT32    hDevice,
                                   UINT8     uInterfaceNumber,
				   UINT8     uSpeed,
                                   void      **pDriverData);
    /*
     * Function registered as to be called when a matching interface/device
     * is disconnected
     */
    void          (*removeDevice)  (UINT32    hDevice,
                                    void *    pDriverData);
    /*
     * Function registered as to be called when a matching interface/device
     *  is suspended
     */
    void          (*suspendDevice) (UINT32    hDevice,
                                    void *     pDriverData);

    /*
     * Function registered as to be called when a matching interface/device
     * is resumed
     */
    void          (*resumeDevice)  (UINT32    hDevice,
                                    void *     pDriverData);
    } USBHST_DEVICE_DRIVER, *pUSBHST_DEVICE_DRIVER;

/* Setup packet structure  */
typedef struct usbhst_setup_packet
    {
    UINT8  bmRequestType;        /* Bitmap for request characteristics       */
    UINT8  bRequest;             /* Specific request                         */
    UINT16 wValue;               /* Word size value field for specific req   */
    UINT16 wIndex;               /* Word size index field for specific req   */
    UINT16 wLength;              /* Num of bytes to transfer if data stage   */
    }__attribute__((packed)) USBHST_SETUP_PACKET,
     *pUSBHST_SETUP_PACKET;

/*
 * This structure is used to store the information for isochronous
 * transfer required by USB request Block
 */
typedef struct usbhst_iso_packet_desc
    {
    UINT32        uLength;       /* Length of data                           */
    UINT32        uOffset;       /* Offset in the pTransferBuffer for data   */
    USBHST_STATUS nStatus;       /* Per frame status.                        */
    } USBHST_ISO_PACKET_DESC, *pUSBHST_ISO_PACKET_DESC;

typedef struct usbhst_urb USBHST_URB, *pUSBHST_URB;
typedef USBHST_STATUS (*pURB_CALLBACK_t)(pUSBHST_URB pURB);

/*
 * This structure is used to store the information required to fill a
 * USB Request Block
 */

struct usbhst_urb
    {
    UINT32          hDevice;           /* Device handle                      */
    UINT8           uEndPointAddress;  /* EndPoint addr for which URB issued */
    UINT8           uTransferType;     /* Transfer type for which URB issued */
    UINT32          uStreamId;         /* Stream ID for this Transfer (USB3) */
    UCHAR *         pTransferBuffer;   /* Ptr to the buffer passed as param  */
    UINT32          uTransferLength;   /* Len of the data transfer requested
                                        * and updated to the actual transfered */
    UINT32          uTransferFlags;    /* Flags for data transfer            */
    UINT16          uStartFrame;       /* Start of Frame information         */
    UINT32          uNumberOfPackets;  /* Number of packets                  */

    /*
     * Pointer to setup packet in case of control transfers
     * Pointer to isochronous packet descriptor in case of isochronous transfer.
     * NULL in case of bulk and interrupt transfers
     */

    void *           pTransferSpecificData;

    /*
     * Pointer to the HCD specific information; Normally a HCD will have a
     * HCD specific data structure associated with a URB when it is submitted,
     * which maintains HCD private information. USBD and class drivers are not
     * allowed to modify this pointer while the URB has been submitted.
     *
     * This can be used to retrive the HCD specific information for a URB
     * without any complicated searching, thus would bring some peroformance
     * improvment if used properly by the HCD.
     */

    void *           pHcdSpecific;

    pURB_CALLBACK_t  pfCallback;        /* Pointer to the callback function   */
    pURB_CALLBACK_t  pfUserCallback;    /* pointer to the user register
                                         * callback funciton
                                         */
    void *           pContext;          /* Class driver specific URB info     */
    USBHST_STATUS    nStatus;           /* Request status                     */
    NODE             urbListNode;       /* List node for endpoint URB list    */
    };

#define LIST_NODE_TO_URB(pNode)       \
       ((USBHST_URB *) ((char *) (pNode) -      \
                        OFFSET(USBHST_URB, urbListNode)))

/* This structure is used to store the device control parameters */

typedef struct usbhst_device_control_info
    {
    UINT8   uDeviceAddress;   /* Device address */
    UINT8   uDeviceSpeed;     /* Device speed */
    UINT8   uRootHubPort;     /* Device Root Bug Port */
    UINT8   uTTHubAddress;    /* High Speed (TT) Hub Address */
    UINT8   uTTHubPort;       /* High Speed (TT) Hub Port */
    UINT8   uTTHubNumPorts;   /* High Speed (TT) Hub Port Count */
    UINT8   uMTT;             /* Multi-TT (MTT) */
    UINT8   uTTT;             /* TT Think Time (TTT) */
    UINT32  uRouteString;     /* Route String */
    UINT32  uControlCode;     /* Control code that is to be operated */
    ULONG   uControlParam;    /* Control parameter */
    USBHST_STATUS   nStatus;  /* Control status */
    }USBHST_DEVICE_CONTROL_INFO, *pUSBHST_DEVICE_CONTROL_INFO;

/* This structure is used to store the pipe control parameters */

typedef struct usbhst_pipe_control_info
    {
    UINT8   uEndPointAddress; /* Endpoint address of the pipe */
    UINT32  uControlCode;     /* Control code that is to be operated */
    ULONG   uControlParam;    /* Control parameter */
    USBHST_STATUS   nStatus;  /* Control status */
    }USBHST_PIPE_CONTROL_INFO, *pUSBHST_PIPE_CONTROL_INFO;

#define USBHST_CMD_PIPE_RESET         0   /* Reset the pipe */
#define USBHST_CMD_TRANSFER_SETUP     1   /* Setup transfer */
#define USBHST_CMD_RESERVE_DEVICE     2   /* Reserve device */
#define USBHST_CMD_RELEASE_DEVICE     3   /* Release device */
#define USBHST_CMD_HS_HUB_TT_MODE     4   /* High-Speed Hub TT mode */

/* This structure is used to store the trasnfer setup information */

typedef struct usb_transfer_setup_info
    {
    size_t   uMaxTransferSize; /* Maxium possible transfer size of this pipe */
    size_t   uMaxNumReqests;   /* Maxium possible transfer requests at a time */
    UINT32   uFlags;           /* Any possible flags */
    }USB_TRANSFER_SETUP_INFO, *pUSB_TRANSFER_SETUP_INFO;

/*
 * This structure is used to store the pointers to USBD entry points that
 * are visible to only Hub Class Driver.
 */
typedef struct usbhst_usbd_to_hub_function_list
    {
    /*
     * Pointer to function, which will be called by Hub Class Driver to
     * create a new USB device
     */
    USBHST_STATUS   (*newDevice)(UINT8   uSpeed,
                                 UINT8   uUsbBusIndex,
                                 UINT32  uHighSpeedHub,
                                 UINT8   uHighSpeedHubPortNumber,
                                 UINT8   uParentTier,
                                 UINT8   uParentPortIndex,
                                 UINT32  uRouteString,
                                 UINT32 *pDeviceHandle);

    /*
     * Pointer to function, which will be called by Hub Class Driver to
     * configure a new USB device
     */
    USBHST_STATUS   (*configureDevice)(UINT32 hDevice);

    /*
     * Pointer to function, which will be called by Hub Class Driver to
     * remove a  USB device
     */
    void            (*removeDevice)(UINT32 hDevice);

    /*
     * Pointer to function, which will be called by Hub Class Driver to
     * suspend a USB device
     */
    void            (*suspendDevice)(UINT32 hDevice);

    /*
     * Pointer to function, which will be called by Hub Class Driver to
     * resume a USB device
     */
    void            (*resumeDevice)(UINT32 hDevice);

    /*
     * Pointer to function, which will be called by Hub Class Driver upon
     * completion of Clear TT Request
     */

    USBHST_STATUS   (*clearTTComplete)(USBHST_STATUS nStatus,
                                       UINT32        hHighSpeedHub,
                                       void *        pContext);
    /*
     * Pointer to function, which will be called by Hub Class Driver upon
     * completion of Reset TT Request
     */
    USBHST_STATUS   (*resetTTComplete)(USBHST_STATUS nStatus,
                                       UINT32        uHighSpeedHub,
                                       void *        pContext);

    /*
     * Pointer to function, which will be called by Hub Class Driver, When a
     * upper driver call usbHubResetDevice to reset itself. After the reset
     * the hub driver shouldn't just re-new a device because the device handle
     * has been created already, it should check device's configuration and
     * re-configurate the device.
     */
    USBHST_STATUS   (*resetDeviceCheck)(UINT32 uDeviceHandle,
                                        UINT8   uSpeed,
                                        UINT8   uBusIndex);

    } USBHST_USBD_TO_HUB_FUNCTION_LIST, *pUSBHST_USBD_TO_HUB_FUNCTION_LIST;

/*
 * This structure is used to store the pointers to USBD entry points that
 * are visible only to Enhanced Host Controller(EHCI) Driver.
 */
typedef struct usbHstUsbdToHcdFunctionList
{
    /*
     * Pointer to function, which will be called by Host Controller Driver to
     * submit clear TT request
     */
    USBHST_STATUS  (*clearTTRequest)(UINT32 hHCDriver,
                                     UINT8  uRelBusIndex,
                                     UINT8  uHighSpeedHubAddress,
                                     UINT8  uHighSpeedPortNumber,
                                     UINT16 wValue,
                                     void * pContext);

    /*
     * Pointer to function, which will be called by Host Controller Driver to
     * submit reset TT request
     */
    USBHST_STATUS  (*resetTTRequest)(UINT32 hHCDriver,
                                     UINT8  uRelBusIndex,
                                     UINT8  uHighSpeedHubAddress,
                                     UINT8  uHighSpeedPortNumber,
                                     void * pContext);

} USBHST_USBD_TO_HCD_FUNCTION_LIST, *pUSBHST_USBD_TO_HCD_FUNCTION_LIST;


/* This structure contains the hub class driver function pointers */
typedef struct usbhst_hub_function_list
    {
    /* Function pointer to suspend a device */
    USBHST_STATUS (*selectiveSuspendDevice)(UINT32 uDeviceHandle);

    /* Function pointer to resume a device */
    USBHST_STATUS (*selectiveResumeDevice) (UINT32 uDeviceHandle);

    /* Function pointer to add a root hub */
    USBHST_STATUS (*addRootHub)   (UINT32 uDeviceHandle,
				   UINT8 uBusHandle,
				   UINT8 uSpeed);

    /* Function pointer to remove a root hub */
    void  (*removeRootHub)(UINT8 uBusHandle);

    /*
     * Function pointer to check whether the power required for a device can be
     * supported.
     */
    USBHST_STATUS (*checkForPower)(UINT32 uDeviceHandle,
                                   UINT8 uPowerRequirement);

    /* Function pointer to reset a device */
    USBHST_STATUS (*resetDevice)   (UINT32 uDeviceHandle,
                                    INT32 timeoutMS);


    /* Function pointer to submit clear TT request */
    USBHST_STATUS (*clearTT)(UINT32 hHighSpeedHub,
                             UINT8  uPort,
                             UINT16 uValue,
                             void *  pContext);

    /* Function pointer to submit reset TT request */
    USBHST_STATUS (*resetTT)(UINT32 hHighSpeedHub,
                             UINT8  uPort,
                             void *  pContext);


    } USBHST_HUB_FUNCTION_LIST, *pUSBHST_HUB_FUNCTION_LIST;

typedef union usbHstFunctionList
{

    USBHST_USBD_TO_HUB_FUNCTION_LIST    UsbdToHubFunctionList;
    USBHST_HUB_FUNCTION_LIST             HubFunctionList;

}USBHST_FUNCTION_LIST, *pUSBHST_FUNCTION_LIST;

/* This structure contains the HC driver function pointers */
typedef struct usbhst_hc_driver
    {
    /* Number of buses for this host controller driver */
    UINT8       uNumberOfBus;

    /* Number of hosts for this host controller driver */
    UINT8       uNumberOfHost;

    /* Bus Id */

    UINT32      busID;

    /* vxBus specific pointer to hold vxbBusTypeInfo type information */

    struct vxbBusTypeInfo        * pUsbHcdBusType;

    /* HCD specific information */

    VOID *      pHcdSpecific[USB_MAX_HOSTS_PER_HCD];

    /* Function pointer to control the device characteristics */
    USBHST_STATUS   (*deviceControl)   (UINT8          uBusIndex,
                                        pUSBHST_DEVICE_CONTROL_INFO    pCtrlInfo);

    /* Function pointer to get the frame number */
    USBHST_STATUS   (*getFrameNumber)  (UINT8   uBusIndex,
                                        UINT16 *puFrameNumber);

    /* Function pointer to set the bit rate */
    USBHST_STATUS   (*setBitRate)      (UINT8   uBusIndex,
                                        BOOL    bIncrement,
                                        UINT32 *puCurrentFrameWidth);

    /* Function pointer to check if required bandwidth is avilable */
    USBHST_STATUS   (*isBandwidthAvailable)
                                           (UINT8   uBusIndex,
                                            UINT8   uDeviceAddress,
                                            UINT8   uDeviceSpeed,
                                            UCHAR  *pCurrentDescriptor,
                                            UCHAR  *pNewDescriptor);

    /* Function pointer to create a pipe */
    USBHST_STATUS   (*createPipe)      (UINT8   uBusIndex,
                                        UINT8   uDeviceAddress,
                                        UINT8   uDeviceSpeed,
                                        UCHAR  *pEndPointDescriptor,
                                        UINT16  uHighSpeedHubInfo,
                                        ULONG  *puPipeHandle);

    /* Function pointer to modify the default pipe */
    USBHST_STATUS   (*modifyDefaultPipe)
                                           (UINT8   uBusIndex,
                                            ULONG   uDefaultPipeHandle,
                                            UINT8   uDeviceSpeed,
                                            UINT8   uMaxPacketSize,
                                            UINT16   uHighSpeedHubInfo);

    /* Function pointer to delete a pipe */
    USBHST_STATUS   (*deletePipe)      (UINT8   uBusIndex,
                                        ULONG   uPipeHandle);

    /* Function pointer to control the pipe characteristics */
    USBHST_STATUS   (*pipeControl)     (UINT8          uBusIndex,
                                        ULONG          uPipeHandle,
                                        pUSBHST_PIPE_CONTROL_INFO    pCtrlInfo);

    /* Function pointer to check if there is any request pending on the pipe */
    USBHST_STATUS   (*isRequestPending)(UINT8   uBusIndex,
                                        ULONG   uPipeHandle);

    /* Function pointer to submit USB request  */
    USBHST_STATUS   (*submitURB)       (UINT8          uBusIndex,
                                        ULONG          uPipeHandle,
                                        pUSBHST_URB    pURB);

    /* Function pointer to cancel USB request */
    USBHST_STATUS   (*cancelURB)       (UINT8          uBusIndex,
                                        ULONG          uPipeHandle,
                                        pUSBHST_URB    pURB);

    /* Function pointer to submit the status of the clear TT request */
    USBHST_STATUS (*clearTTRequestComplete)(UINT8         uRelativeBusIndex,
                                            void *         pContext,
                                            USBHST_STATUS nStatus);
    /* Function pointer to submit the status of the reset TT request */
    USBHST_STATUS (*resetTTRequestComplete)(UINT8         uRelativeBusIndex,
                                            void *         pContext,
                                            USBHST_STATUS nStatus);

    } USBHST_HC_DRIVER, *pUSBHST_HC_DRIVER;

#ifndef _WRS_CONFIG_VXBUS_LEGACY
typedef struct vxbDev * VXB_DEVICE_ID;
#endif

/**************************** FUNCTIONS DECLARATION ***************************/

/*******************************************************************************
 * Function Name    : USBHST_RegisterDriver
 * Description      : This function registers the class driver with the USB
 *                    Host Stack. This function also associates the devices that
 *                    can be supported by this class driver.
 * Parameters       : pDeviceDriverInfo IN      Information about the class
 *                                              driver being registered.
 *                    pContext          IN OUT  For Non-Hub Class Drivers:
 *                                              Not Interpreted.
 *                                              For Hub Class Drivers:
 *                                              IN: Pointer to structure
 *                                              containing function pointers
 *                                              required by USBD from Hub Driver
 *                                              OUT: Pointer to structure
 *                                              containing function pointers
 *                                              required by Hub Driver from USBD
 * Return Type      : USBHST_INVALID_PARAMETER if Parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY Memory could not be allocated
 *                    USBHST_FAILURE if Driver is already registered
 *                    USBHST_SUCCESS if Class driver is registered successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstDriverRegister
    (
    pUSBHST_DEVICE_DRIVER pDeviceDriverInfo,
    void                 **pContext,
    char                  * pDrvName
    );

/*******************************************************************************
 * Function Name    : USBHST_DeregisterDriver
 * Description      : This function deregisters the class driver from the USB
 *                    Host Stack. The class driver is unloaded for all the
 *                    supported devices
 * Parameters       : pDeviceDriverInfo IN  Information about the class driver
 *                                          being deregistered
 * Return Type      : USBHST_INVALID_PARAMETER if Parameters are not valid.
 *                    USBHST_FAILURE if Driver is not found or if it is a hub
 *                    class driver and there are some functional devices present
 *                    USBHST_SUCCESS if class driver deregistered successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstDriverDeregister
    (
    pUSBHST_DEVICE_DRIVER pDeviceDriverInfo
    );

/*******************************************************************************
 * Function Name    : USBHST_RegisterHCD
 * Description      : This function is used to register a Host Controller
 *                    Driver with the USB Host Stack.
 * Parameters       : pHCDriver     IN  Information about the Host Controller
 *                                      Driver being registered.
 *                    phHCDriver    OUT Parameter to hold the handle for HC
 *                                      Driver.
 *                    void * pContext
 *                    UNIT32 busID   IN Used by vxBus to identify the bus type.
 * Return Type      : USBHST_INVALID_PARAMETER if Parameters are not valid.
 *                    USBHST_INSUFFICIENT_RESOURCE if Resources are not
 *                     sufficient.
 *                    USBHST_SUCCESS if Host Controller Driver is registered
 *                    successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstHCDRegister
    (
    pUSBHST_HC_DRIVER   pHCDriver,
    UINT32              *phHCDriver,
    void *              pContext,
    UINT32              busId
    );

/*******************************************************************************
 * Function Name    : USBHST_DeregisterHCD
 * Description      : This function is used to deregister a Host Controller
 *                    Driver from the USB Host Stack.
 * Parameters       : hHCDriver    IN  Handle for HC Driver being deregistered.
 * Return Type      : USBHST_INVALID_PARAMETER if Parameters are not valid.
 *                    USBHST_FAILURE if bus count is not zero
 *                    USBHST_SUCCESS if Host Controller Driver is deregistered
 *                    successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstHCDDeregister
    (
    UINT32 hHCDriver
    );

/*******************************************************************************
 * Function Name    : USBHST_RegisterBus
 * Description      : This function registers an USB Bus corresponding to the
 *                    host controller.
 * Parameters       : hHCDriver     IN  Handle for HC Driver requesting
 *                                      registration for its bus.
 *                    uSpeed        IN  Speed of the USB Bus
 *                    hDefaultPipe  IN  Handle to the default pipe for the USB
 *                                      Bus
                      pDev          IN  struct vxbDev *
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_INSUFFICIENT_RESOURCES if Resources are not
 *                    sufficient for this operation.
 *                    USBHST_FAILURE   USB Bus is already registered.
 *                    USBHST_SUCCESS   USB Bus is registered successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstBusRegister
    (
    UINT32    hHCDriver,
    UINT8     uSpeed,
    ULONG               hDefaultPipe,
    VXB_DEVICE_ID       pDev
    );

/*******************************************************************************
 * Function Name    : USBHST_DeregisterBus
 * Description      : This function deregisters an USB Bus corresponding to the
 *                    host controller.
 * Parameters       : hHCDriver         IN  Handle for HC Driver requesting
 *                                          deregistration for its bus.
 *                    uBusIndex         IN  Relative Index of the Bus being
 *                                          deregistered.
 *                    hDefaultPipe      IN  Handle to the default pipe for the
 *                                          USB Bus
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_FAILURE when Attempt to deregister the USB Bus
 *                    while there are functional devices on it.
 *                    USBHST_SUCCESS if USB Bus is deregistered successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstBusDeregister
    (
    UINT32  hHCDriver,
    UINT32  uBusIndex,
    ULONG   hDefaultPipe
    );

extern USBHST_STATUS usbHstCtlrAdd
    (
    UINT32         hHCDriver,       /* Host Controller Driver handle */
    VXB_DEVICE_ID  pDev             /* struct vxbDev for Host Ctlr   */
    );

extern USBHST_STATUS usbHstCtlrRemove
    (
    UINT32         hHCDriver,       /* Host Controller Driver handle */
    VXB_DEVICE_ID  pDev             /* struct vxbDev for Host Ctlr   */
    );

extern USBHST_STATUS usbHstBusAdd
    (
    UINT32         hHCDriver,       /* Host Controller Driver handle */
    VXB_DEVICE_ID  pDev,            /* struct vxbDev for host ctrlr  */
    UINT8          uBusSpeed,       /* USB Bus speed                 */
    ULONG          hDefaultPipe     /* Default pipe handle           */
    );

extern USBHST_STATUS usbHstBusRemove
    (
    UINT32         hHCDriver,       /* Host Controller Driver handle */
    VXB_DEVICE_ID  pDev,            /* struct vxbDev for host ctrlr  */
    UINT8          uBusSpeed,       /* USB Bus speed                 */
    ULONG          hDefaultPipe     /* Default pipe handle           */
    );

/*******************************************************************************
 * Function Name    : USBHST_SelectiveSuspend
 * Description      : This function is called by the class drivers to
 *                    selectively suspend its device.
 * Parameters       : hDevice   IN  Handle to the device to be suspended
 *                    selectively.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS   Hub driver is informed successfully about
 *                    selective suspend of the device.
 ******************************************************************************/
extern USBHST_STATUS usbHstSelectiveSuspend
    (
    UINT32 hDevice
    );

/*******************************************************************************
 * Function Name    : USBHST_SelectiveResume
 * Description      : This function is called by the class drivers to
 *                    selectively resume its device.
 * Parameters       : hDevice   IN  Handle to the device to be resumed
 *                                  selectively.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS   Hub driver is informed successfully about
 *                    selective resume of the device.
 ******************************************************************************/
extern USBHST_STATUS usbHstSelectiveResume
    (
    UINT32 hDevice
    );

/*******************************************************************************
 * Function Name    : USBHST_ResetDevice
 * Description      : This function is called by the class drivers to
 *                    reset its device.
 * Parameters       : hDevice   IN  Handle to the device to be reset.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS   Hub driver is called successfully
 *                       to reset the device.
 ******************************************************************************/
extern USBHST_STATUS usbHstResetDevice
    (
    UINT32 hDevice
    );

/*******************************************************************************
 * Function Name    : usbHstResetDeviceTimeout
 * Description      : This function is called by the class drivers to
 *                    reset its device with timeout wait time.
 * Parameters       : hDevice   IN  Handle to the device to be reset.
                      timeoutMS reset wait timeout value.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS   Hub driver is called successfully
 *                       to reset the device.
 ******************************************************************************/
extern USBHST_STATUS usbHstResetDeviceTimeout
    (
    UINT32 hDevice,
    INT32  timeoutMS
    );
/*******************************************************************************
* usbHstPipePrepare - prepare a pipe for data transfers
*
* This routine is used to prepare a pipe for data transfers, by specifying the
* maximum possible transfer request size and the maximum number of simultaneous
* requests for one pipe. Based on the maximum possible transfer request size,
* the HCD can create the underlying DMA TAG; Based on the maximum number of
* possible transfer requests at a time, the HCD can create enough DMA MAPs
* to accommodate all simultaneous pending transfer requests.
*
* RETURNS: USBHST_SUCCESS, or other USBHST_STATUS values if fails.
*
* ERRNO: N/A
*/

USBHST_STATUS usbHstPipePrepare
    (
    UINT32                      hDevice,      /* USB device handle */
    UINT8                       uEpAddr,      /* USB endpoint number */
    pUSB_TRANSFER_SETUP_INFO    pSetupInfo    /* Pipe transfer setup info */
    );

/*******************************************************************************
 * Function Name    : USBHST_GetDescriptor
 * Description      : This function is used to issue GET_DESCRIPTOR USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.
 *                    uDescType     IN  Specify the type of descriptor.
 *                    uDescIndex    IN  Index of configuration or string
 *                                      descriptor.
 *                    swLangID      IN  Language ID in case of string descriptor
 *                    uSize         IN  Number of bytes of descriptor to fetch.
 *                    pBuffer       OUT Pointer to buffer to hold the fetched
 *                                      data.
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if data for the descriptor is
 *                    successfully fetched
 ******************************************************************************/
extern USBHST_STATUS usbHstGetDescriptor
    (
    UINT32     hDevice,
    UINT8      uDescType,
    UINT8      uDescIndex,
    UINT16     swLangID,
    UINT32    *pSize,
    UCHAR     *pBuffer
    );

/*******************************************************************************
 * Function Name    : USBHST_GetStatus
 * Description      : This function is used to issue GetStatus USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.
 *                    uRecipient    IN  Specify the desired recipient.
 *                    uIndex        IN  Specify the index of recipient info list
 *                    pBuffer       OUT Pointer to buffer to hold the fetched
 *                                      data.
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if Status of the requested recipient is
 *                    fetched successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstGetStatus
    (
    UINT32  hDevice,
    UINT8   uRecipient,
    UINT16  uIndex,
    UCHAR  *pBuffer
    );

/*******************************************************************************
 * Function Name    : USBHST_ClearFeature
 * Description      : This function is used to issue ClearFeature USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.
 *                    uRecipient    IN  Specify the desired recipient.
 *                    uIndex        IN  Specify the index of recipient info list
 *                    uFeature      IN  Specify the feature selector
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if Requested feature is cleared
 *                    successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstClearFeature
    (
    UINT32   hDevice,
    UINT8    uRecipient,
    UINT16   uIndex,
    UINT16   uFeature
    );

/*******************************************************************************
 * Function Name    : USBHST_GetConfiguration
 * Description      : This function is used to issue GetConfiguration USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.
 *                    pBuffer       OUT Pointer to buffer to hold the fetched
 *                                      data.
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if current configuration is successfully
 *                    fetched
 ******************************************************************************/
extern USBHST_STATUS usbHstGetConfiguration
    (
    UINT32   hDevice,
    UCHAR   *pBuffer
    );

/*******************************************************************************
 * Function Name    : USBHST_GetInterface
 * Description      : This function is used to issue GetInterface USB
 *                    Standard Request.
 * Parameters       : hDevice          IN  Handle to USB Device.
 *                    uInterfaceNumber IN  Specify the interface number.
 *                    pBuffer          OUT Pointer to buffer to hold the fetched
 *                                         data.
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if Selected alternate setting for a
 *                    requested interface is fetched successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstGetInterface
    (
    UINT32       hDevice,
    UINT16       uInterfaceNumber,
    UCHAR       *pBuffer
    );

/*******************************************************************************
 * Function Name    : USBHST_SetConfiguration
 * Description      : This function is used to issue SetConfiguration USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.
 *                    uIndex        IN  Specify the Configuration index.
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_INSUFFICIENT_BANDWIDTH if Insufficient bandwidth
 *                    for new configuration set up.
 *                    USBHST_INSUFFICIENT_RESOURCES Insufficient power
 *                    resources from the hub to support new configuration.
 *                    USBHST_SUCCESS if Configuration is set successfully..
 ******************************************************************************/
extern USBHST_STATUS usbHstSetConfiguration
    (
    UINT32 hDevice,
    UINT16 uIndex
    );

/*******************************************************************************
 * Function Name    : USBHST_SetFeature
 * Description      : This function is used to issue SetFeature USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.
 *                    uRecipient    IN  Specify the desired recipient.
 *                    uIndex        IN  Specify the index of recipient info list
 *                    uFeature      IN  Specify the feature selector
 *                    uTestSlector  IN  Specifiy the test selector in case the
 *                                      uFeature is USBHST_TEST_MODE
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if Requested feature is set successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstSetFeature
    (
    UINT32     hDevice,
    UINT8      uRecipient,
    UINT16     uIndex,
    UINT16     uFeature,
    UINT8      uTestSelector
    );

/*******************************************************************************
 * Function Name    : USBHST_SetDescriptor
 * Description      : This function is used to issue SetDescriptor USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.
 *                    uDescType     IN  Type of descriptor to set.
 *                    uDescIndex    IN  Parameter to hold the device handle.
 *                    swLangID      IN  Language ID in case of string descriptor
 *                    uSize         IN  Number of bytes of descriptor to fetch.
 *                    pBuffer       OUT Buffer Pointer containing data.
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if the descriptor is set successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstSetDescriptor
    (
    UINT32  hDevice,
    UINT8   uDescType,
    UINT8   uDescIndex,
    UINT16  swLangID,
    UCHAR  *pBuffer,
    UINT32  uSize
    );

/*******************************************************************************
 * Function Name    : USBHST_SetInterface
 * Description      : This function is used to issue SetInterface USB
 *                    Standard Request.
 * Parameters       : hDevice       IN  Handle to USB Device.                .
 *                    uIndex        IN  Specify the interface number.
 *                    uAltIndex     IN  Specify the alternate interface
 *                                      setting value.
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if the interface is set successfully
 ******************************************************************************/
extern USBHST_STATUS usbHstSetInterface
    (
    UINT32  hDevice,
    UINT16  uIndex,
    UINT16  uAltIndex
    );

/*******************************************************************************
 * Function Name    : USBHST_SetSynchFrame
 * Description      : This function is used to issue SetSynchFrame USB
 *                    Standard Request.
 * Parameters       : hDevice           IN  Handle to USB Device.
 *                    uEndPointNumber   IN  Specify the endpoint number
 *                    pBuffer           OUT Pointer to buffer to hold the
 *                                          synchronization frame number
 * Return Type      : USBHST_INVALID_PARAMETERS if parameters are not valid.
 *                    USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    USBHST_TIMEOUT if request submitted to Host controller
 *                    driver is timed out.
 *                    USBHST_SUCCESS if Synchronization frame for the requested
 *                    end point is set and reported successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstSetSynchFrame
    (
    UINT32     hDevice,
    UINT16     uEndPointNumber,
    UCHAR     *pBuffer
    );

/*******************************************************************************
 * Function Name    : USBHST_SubmitURB.
 * Description      : This function is used to submit the USB request to HCD.
 * Parameters       : pURB  IN  USB request block structure.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS   URB is successfully submitted to the Host
 *                    Controller Driver.
 ******************************************************************************/
extern USBHST_STATUS usbHstURBSubmit
    (
    USBHST_URB *pURB
    );

/*******************************************************************************
 * Function Name    : USBHST_CancelURB.
 * Description      : This function is used to cancel the request submitted to
 *                    HCD.
 * Parameters       : pURB  IN  USB request block structure.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS   Host controller driver is requested
 *                    successfully to cancel the URB.
 ******************************************************************************/
extern USBHST_STATUS usbHstURBCancel
    (
    USBHST_URB *pURB
    );

/*******************************************************************************
 * Function Name    : usbHst_WaitUrbComplete.
 * Description      : This function is used to wait the URB to be completed.
 * Parameters       : pURB    IN  USB request block structure.
 *                    EventId IN  semphore of the URB to wait.
 *                    timeout IN  time out value to wait the urb complete
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_STATUS             URB complete status
 ******************************************************************************/
extern USBHST_STATUS usbHstWaitUrbComplete
    (
    pUSBHST_URB pURB,
    SEM_ID      EventId,
    int         timeout
    );

/*******************************************************************************
 * Function Name    : USBHST_SetBitRate
 * Description      : This function is used to set the bit rate of specified bus
 * Parameters       : hDevice              IN   Handle to  device requesting
 *                                              this operation.
 *                    bIncrement           IN   Boolean Flag:
 *                                              0:Decrement the bit rate.
 *                                              1:Increment the bit rate.
 *                    pCurrentFrameWidth   OUT  Pointer to hold the current bit
 *                                              rate.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS  Bit rate is set successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstSetBitRate
    (
    UINT32   hDevice,
    BOOL     bIncrement,
    UINT32  *pCurrentFrameWidth
    );

/*******************************************************************************
 * Function Name    : USBHST_GetFrameNumber
 * Description      : This function is used to get the current frame number
 *                    on specified bus.
 * Parameters       : hDevice       IN  Handle to the device requesting this
 *                                      operation.
 *                    pFrameNumber  OUT Pointer to hold the frame number.
 * Return Type      : USBHST_INVALID_PARAMETERS if Parameters are not valid.
 *                    USBHST_SUCCESS if Frame number is fetched successfully.
 ******************************************************************************/
extern USBHST_STATUS usbHstGetFrameNumber
    (
    UINT32  hDevice,
    UINT16 *pFrameNumber
    );

/*******************************************************************************
 * Function Name    : USBD_Init
 * Description      : This function initializes the global variables.
 * Parameters       : None
 * Return Type      : a) USBHST_FAILURE if USBD could not be initialized.
 *                    b) USBHST_SUCCESS if all the global variables are
 *                       properly initialized.
 ******************************************************************************/
extern USBHST_STATUS usbdInit(void);

/*******************************************************************************
 * Function Name    : USBD_Exit
 * Description      : This function does the clean up.
 * Parameters       : None
 * Return Type      : a) USBHST_FAILURE if bus count is not zero.
 *                    b) USBHST_SUCCESS when USBD successfully exited.
 ******************************************************************************/
extern USBHST_STATUS usbdExit(void);

/*******************************************************************************
 * Function Name    : usbHstCommonCallback
 * Description      : This function does the call back action of the urb.
 * Parameters       : None
 * Return Type      : a) USBHST_FAILURE if parameter is not valid.
 *                    b) USBHST_SUCCESS if callback function successfully exited.
 ******************************************************************************/
extern USBHST_STATUS usbHstCommonCallback
    (
    pUSBHST_URB pUrb
    );

/*******************************************************************************
 * Function Name    : usbHstClassVendorRequest
 * Description      : This function is used to issue class/vendor request.
 * Parameters       : hDevice        IN  Handle to the device requesting this
 *                                       operation.
 *                    bmRequestType  IN  Specify request Type of setup packet.
 *                    bRequest       IN  Specify request code of setup packet.
 *                    wValue         IN  Specify wValue of setup packet.
 *                    wIndex         IN  Specify wIndex of setup packet.
 *                    pwLength       IN  Specify expected transfer length.
 *                                   OUT Return actual transfer length.
 *                    pBuffer        IN  Specify data buffer address.
 * Return Type      : a) USBHST_INVALID_PARAMETERS if parameter is not valid.
 *                    b) USBHST_INSUFFICIENT_MEMORY on insufficient resource.
 *                    c) USBHST_TIMEOUT if request submitted to Host controller
 *                       driver is timed out.
 *                    d) USBHST_FAILURE if transfer is unsuccessfully.
 *                    e) USBHST_SUCCESS if callback function successfully exited.
 ******************************************************************************/
extern USBHST_STATUS usbHstClassVendorRequest
    (
    UINT32    hDevice,
    UINT8     bmRequestType,
    UINT8     bRequest,
    UINT16    wValue,
    UINT16    wIndex,
    UINT32  * pwLength,
    UCHAR   * pBuffer
    );

/*******************************************************************************
 * Function Name    : usbMsgPost
 * Description      : Post a message to report current status or error
 * Parameters       : msgCode        IN  Specify USB_MSG_CODE
 *                    pModule        IN  parameter for module
 *                    param          IN  parameter
 * Return Type      : Always return NULL
 ******************************************************************************/
extern void * usbMsgPost
    (
    UINT32 msgCode,
    void * pModule,
    void * param
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __USBHST_H__*/

/**************************** End of file Usbhst.h ****************************/

