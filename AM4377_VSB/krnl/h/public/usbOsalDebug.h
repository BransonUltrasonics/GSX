/* usbOsalDebug.h - USB OS Abstraction Layer Debug Definitions */

/*
 * Copyright (c) 2003, 2010-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003, 2010-2015 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
02apr16,dlk  Use END_LOGX() for logging (V7NET-535).
03nov15,j_x  Add VSB debug option (US68396)
09Oct14,ljg  Add USB Pch Udc Tcd support
29dec13,s_z  add USB PHY debug support
04nov13,s_z  make USB work both on new VxBus model and VxBus legacy model
15oct13,j_x  Disable USB Debug Message by default (WIND00436489)
01sep13,j_x  Add touch screen
06jan13,s_z  Redefine debug level using bitmap (WIND00371346)
18may12,s_z  Add support for PLX USB 3.0 TCD support (WIND00188662) 
19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
12mar12,ljg  Modify debug messages (WIND00334460)
22apr11,ljg  Add USBTGT printer and USBTGT keyboard emulators debug support
13feb11,s_z  Initial USB network function driver debug support 
29jan11,ghs  Add USB target serial emulator debug
13jan11,w_x  Initial USB OTG support changes (WIND00211541)
26sep10,ghs  Disable debug messages for default configuration (WIND00234470)
07sep10,ghs  Defined MHCD debug components
08jul10,m_y  Add synopsys hcd debug
10jun10,ghs  Add usb serial debug
27apr10,j_x  Error debug adaption
08apr10,jpb  Added header file for logLib.h.
03mar10,j_x  Changed for USB debug (WIND00184542)
17mar03,ssh  written
*/

/*
DESCRIPTION

This file contains the definitions the debugging macros for the OS Abstraction
Layer
*/

/*
INTERNAL
 *******************************************************************************
 * Filename         : OSAL_DEBUG.h
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
 * Description      : OS Abstraction layer debugging support. This file contains
 *                    the macros to enable or disable the debugging support on
 *                    a module basis.
 *
 *
 ******************************************************************************/

#ifndef __INCusbOsalDebugh
#define __INCusbOsalDebugh

#ifdef _WRS_CONFIG_VXBUS_LEGACY
#include <hwif/vxbus/vxBus.h>
#else
#include <hwif/vxBus.h>
#include "vxbUsbBus.h"
#endif
#include <vsbConfig.h>

/*
 * Dynamic debug
 */

#define USB_NONE_DEBUG                 0   /* Show NONE msgs       */
#define USB_INFO_DEBUG                 (0x01 << 0)   /* Show INFO msgs      */
#define USB_ERROR_DEBUG                (0x01 << 1)   /* Show ERROR msgs      */
#define USB_WARNING_DEBUG              (0x01 << 2)   /* Show WARNING msgs    */
#define USB_NORMAL_DEBUG               (0x01 << 3)   /* Show NORMAL msgs     */
#define USB_VERBOSE_DEBUG              (0x01 << 4)   /* Show VERBOSE msgs    */

#ifdef _WRS_CONFIG_USB_DEBUG_ENABLE

#include <logLib.h>		/* _func_logMsg */

#define USB_DBG_INT2SYM(x)      #x
#define USB_DBG_INT2STR(x)      USB_DBG_INT2SYM(x)
#define USB_DBG_LOCATION        __FILE__"["USB_DBG_INT2STR(__LINE__)"]: "

#define USB_ERR(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    do {                                                         \
        if (((class##Debug) & USB_ERROR_DEBUG) &&                \
            (_func_logMsg != NULL))                              \
            {                                                     \
            _func_logMsg(                                         \
                    "ERROR: "#class" - "USB_DBG_LOCATION fmt,     \
                    (_Vx_usr_arg_t)(arg1),                      \
                    (_Vx_usr_arg_t)(arg2),                      \
                    (_Vx_usr_arg_t)(arg3),                      \
                    (_Vx_usr_arg_t)(arg4),                      \
                    (_Vx_usr_arg_t)(arg5),                      \
                    (_Vx_usr_arg_t)(arg6));                     \
            }                                                   \
    } while (FALSE)

#define USB_WARN(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                         \
        if (((class##Debug) & USB_WARNING_DEBUG) &&              \
            (_func_logMsg != NULL))                              \
            {                                                    \
            _func_logMsg("WARN: "#class" - "USB_DBG_LOCATION fmt,                 \
                    (_Vx_usr_arg_t)(arg1),                       \
                    (_Vx_usr_arg_t)(arg2),                       \
                    (_Vx_usr_arg_t)(arg3),                       \
                    (_Vx_usr_arg_t)(arg4),                       \
                    (_Vx_usr_arg_t)(arg5),                       \
                    (_Vx_usr_arg_t)(arg6));                      \
            }                                                    \
    } while (FALSE)

#define USB_DBG(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                        \
        if (((class##Debug) & USB_NORMAL_DEBUG) &&             \
            (_func_logMsg != NULL))                             \
            {                                                   \
            _func_logMsg("DBG: "#class" - "USB_DBG_LOCATION fmt,                 \
                    (_Vx_usr_arg_t)(arg1),                      \
                    (_Vx_usr_arg_t)(arg2),                      \
                    (_Vx_usr_arg_t)(arg3),                      \
                    (_Vx_usr_arg_t)(arg4),                      \
                    (_Vx_usr_arg_t)(arg5),                      \
                    (_Vx_usr_arg_t)(arg6));                     \
            }                                                   \
    } while (FALSE)

#define USB_VDBG(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)\
    do {                                                        \
        if (((class##Debug) & USB_VERBOSE_DEBUG) &&             \
            (_func_logMsg != NULL))                             \
            {                                                   \
            _func_logMsg("VDBG: "#class" - "USB_DBG_LOCATION fmt,                \
                    (_Vx_usr_arg_t)(arg1),                      \
                    (_Vx_usr_arg_t)(arg2),                      \
                    (_Vx_usr_arg_t)(arg3),                      \
                    (_Vx_usr_arg_t)(arg4),                      \
                    (_Vx_usr_arg_t)(arg5),                      \
                    (_Vx_usr_arg_t)(arg6));                     \
            }                                                   \
    } while (FALSE)

#define USB_INFO(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                         \
        if (((class##Debug) & USB_INFO_DEBUG) &&                 \
            (_func_logMsg != NULL))                              \
            {                                                    \
            _func_logMsg("INFO: "#class" - "USB_DBG_LOCATION fmt,                 \
                    (_Vx_usr_arg_t)(arg1),                       \
                    (_Vx_usr_arg_t)(arg2),                       \
                    (_Vx_usr_arg_t)(arg3),                       \
                    (_Vx_usr_arg_t)(arg4),                       \
                    (_Vx_usr_arg_t)(arg5),                       \
                    (_Vx_usr_arg_t)(arg6));                      \
            }                                                    \
    } while (FALSE)

/* No-wait versions using NOWAIT_LOGX */

#define USB_ERR_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                           \
        if ((class##Debug) & USB_ERROR_DEBUG)                      \
            NOWAIT_LOG6 ("ERROR: "#class" - "USB_DBG_LOCATION fmt, \
                      arg1, arg2, arg3, arg4, arg5, arg6);         \
    } while (FALSE)

#define USB_WARN_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                          \
        if ((class##Debug) & USB_WARNING_DEBUG)                   \
            NOWAIT_LOG6 ("WARN: "#class" - "USB_DBG_LOCATION fmt, \
                      arg1, arg2, arg3, arg4, arg5, arg6);        \
    } while (FALSE)

#define USB_DBG_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                         \
        if ((class##Debug) & USB_NORMAL_DEBUG)                   \
            NOWAIT_LOG6 ("DBG: "#class" - "USB_DBG_LOCATION fmt, \
                      arg1, arg2, arg3, arg4, arg5, arg6);       \
    } while (FALSE)

#define USB_VDBG_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                          \
        if ((class##Debug) & USB_VERBOSE_DEBUG)                   \
            NOWAIT_LOG6 ("VDBG: "#class" - "USB_DBG_LOCATION fmt, \
                      arg1, arg2, arg3, arg4, arg5, arg6);        \
    } while (FALSE)

#define USB_INFO_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    do {                                                          \
        if ((class##Debug) & USB_INFO_DEBUG)                      \
            NOWAIT_LOG6 ("INFO: "#class" - "USB_DBG_LOCATION fmt, \
                      arg1, arg2, arg3, arg4, arg5, arg6);        \
    } while (FALSE)

#else

#define USB_ERR(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_DBG(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_VDBG(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_INFO(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_WARN(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_ERR_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_DBG_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_VDBG_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_INFO_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define USB_WARN_NW(class, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#endif

/* XHCD DEBUG */

#define USB_XHCD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbXhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_XHCD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbXhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_XHCD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbXhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_XHCD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbXhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_XHCD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbXhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* EHCD DEBUG */

#define USB_EHCD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbEhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_EHCD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbEhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_EHCD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbEhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_EHCD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbEhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_EHCD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbEhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* OHCD DEBUG */

#define USB_OHCD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbOhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_OHCD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbOhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_OHCD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbOhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_OHCD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbOhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_OHCD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbOhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* UHCD DEBUG */

#define USB_UHCD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbUhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_UHCD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbUhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_UHCD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbUhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_UHCD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbUhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_UHCD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbUhcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* SYNOPSYSHCD DEBUG */

#define USB_SHCD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbShcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_SHCD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbShcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_SHCD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbShcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_SHCD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbShcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_SHCD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbShcd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* MHDRC DEBUG */

#define USB_MHDRC_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbMhdrc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_MHDRC_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbMhdrc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_MHDRC_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbMhdrc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_MHDRC_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbMhdrc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_MHDRC_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbMhdrc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)


/* USBD DEBUG */

#define USB_USBD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbUsbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_USBD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbUsbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_USBD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbUsbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_USBD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbUsbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_USBD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbUsbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* TU DEBUG */

#define USB_TU_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbTu, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TU_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbTu, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TU_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbTu, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TU_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbTu, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TU_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbTu, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* HUB DEBUG */

#define USB_HUB_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbHub, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_HUB_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbHub, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_HUB_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbHub, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_HUB_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbHub, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_HUB_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbHub, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* OTG DEBUG */

#define USBOTG_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbOtg, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBOTG_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbOtg, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBOTG_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbOtg, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBOTG_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbOtg, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBOTG_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbOtg, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN1 BULK DEBUG */

#define USB1_BLK_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb1Blk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_BLK_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb1Blk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_BLK_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb1Blk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_BLK_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb1Blk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_BLK_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb1Blk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN1 CBI DEBUG */

#define USB1_CBI_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb1Cbi, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_CBI_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb1Cbi, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_CBI_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb1Cbi, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_CBI_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb1Cbi, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_CBI_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb1Cbi, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN1 KEYBOARD DEBUG */

#define USB1_KBD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb1Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_KBD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb1Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_KBD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb1Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_KBD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb1Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_KBD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb1Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN1 MOUSE DEBUG */

#define USB1_MSE_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb1Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_MSE_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb1Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_MSE_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb1Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_MSE_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb1Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_MSE_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb1Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN1 PRINTER DEBUG */

#define USB1_PRN_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb1Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_PRN_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb1Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_PRN_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb1Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_PRN_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb1Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_PRN_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb1Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN1 PEGASUS DEBUG */

#define USB1_END_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb1End, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_END_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb1End, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_END_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb1End, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_END_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb1End, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_END_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb1End, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN1 SPEAKER DEBUG */

#define USB1_SPK_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb1Spk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_SPK_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb1Spk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_SPK_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb1Spk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_SPK_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb1Spk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB1_SPK_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb1Spk, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN2 HELPER DEBUG */

#define USB2_HLP_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb2Hlp, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_HLP_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb2Hlp, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_HLP_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb2Hlp, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_HLP_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb2Hlp, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_HLP_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb2Hlp, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN2 MASS STORAGE DEBUG */

#define USB2_MSC_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb2Msc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSC_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb2Msc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSC_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb2Msc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSC_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb2Msc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSC_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb2Msc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN2 MOUSE DEBUG */

#define USB2_MSE_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb2Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSE_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb2Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSE_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb2Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSE_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb2Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_MSE_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb2Mse, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN2 KEYBOARD DEBUG */

#define USB2_KBD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb2Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_KBD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb2Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_KBD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb2Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_KBD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb2Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_KBD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb2Kbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* TOUCH SCREEN DEBUG */

#define USB2_TS_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb2TS, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_TS_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb2TS, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_TS_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb2TS, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_TS_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb2TS, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_TS_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb2TS, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
    
/* GEN2 PRINTER DEBUG */

#define USB2_PRN_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb2Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_PRN_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb2Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_PRN_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb2Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_PRN_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb2Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_PRN_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb2Prn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN2 END DEBUG */
/*
 * Use no-wait versions, since otherwise if the network task blocks
 * in logMsg() and logging goes to any file descriptors that are
 * network-dependent, the system can deadlock. Note that not all
 * uses of these macros occur in a network task, but some do.
 */
#define USB2_END_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR_NW(usb2End, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_END_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG_NW(usb2End, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_END_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG_NW(usb2End,fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_END_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO_NW(usb2End,fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_END_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN_NW(usb2End,fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* GEN2 Serial DEBUG */

#define USB2_SER_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usb2Ser, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_SER_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usb2Ser, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_SER_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usb2Ser, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_SER_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usb2Ser, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB2_SER_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usb2Ser, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB TGT DEBUG */

#define USB_TGT_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbTgt, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TGT_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbTgt, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TGT_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbTgt, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TGT_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbTgt, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_TGT_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbTgt, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB TGT RNDIS DEBUG */

#define USBTGT_RNDIS_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbTgtRndis, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_RNDIS_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbTgtRndis, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_RNDIS_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbTgtRndis, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_RNDIS_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbTgtRndis, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_RNDIS_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbTgtRndis, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB TGT MSC DEBUG */

#define USBTGT_MSC_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbTgtMsc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_MSC_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbTgtMsc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_MSC_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbTgtMsc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_MSC_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbTgtMsc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_MSC_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbTgtMsc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB TGT END Module Debug */

/*
 * Use no-wait versions, since otherwise if the network task blocks
 * in logMsg() and logging goes to any file descriptors that are
 * network-dependent, the system can deadlock. Note that not all
 * uses of these macros occur in a network task, but some do.
 */
#define USBTGT_END_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR_NW(usbTgtEnd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_END_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG_NW(usbTgtEnd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_END_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG_NW(usbTgtEnd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_END_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO_NW(usbTgtEnd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_END_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN_NW(usbTgtEnd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB TGT SER DEBUG */

#define USBTGT_SER_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbTgtSer, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_SER_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbTgtSer, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_SER_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbTgtSer, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_SER_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbTgtSer, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_SER_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbTgtSer, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB TGT PRN DEBUG */

#define USBTGT_PRN_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbTgtPrn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_PRN_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbTgtPrn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_PRN_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbTgtPrn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_PRN_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbTgtPrn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_PRN_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbTgtPrn, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB TGT KBD DEBUG */

#define USBTGT_KBD_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbTgtKbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_KBD_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbTgtKbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_KBD_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbTgtKbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_KBD_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbTgtKbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USBTGT_KBD_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbTgtKbd, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB PLX TCD DEBUG */

#define USB_PLX_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbPlx, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PLX_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbPlx, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PLX_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbPlx, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PLX_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbPlx, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PLX_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbPlx, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PCHUDC_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbPchUdc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PCHUDC_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbPchUdc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PCHUDC_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbPchUdc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PCHUDC_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbPchUdc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PCHUDC_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbPchUdc, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

/* USB PHY DEBUG */

#define USB_PHY_ERR(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_ERR(usbPhy, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PHY_DBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)   \
    USB_DBG(usbPhy, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PHY_VDBG(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_VDBG(usbPhy, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PHY_INFO(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_INFO(usbPhy, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define USB_PHY_WARN(fmt, arg1, arg2, arg3, arg4, arg5, arg6)  \
    USB_WARN(usbPhy, fmt, arg1, arg2, arg3, arg4, arg5, arg6)


/* IMPORTS */

IMPORT UINT32  usbXhcdDebug;
IMPORT UINT32  usbEhcdDebug;
IMPORT UINT32  usbOhcdDebug;
IMPORT UINT32  usbUhcdDebug;
IMPORT UINT32  usbMhdrcDebug;
IMPORT UINT32  usbShcdDebug;
IMPORT UINT32  usbUsbdDebug;
IMPORT UINT32  usbTuDebug;
IMPORT UINT32  usbHubDebug;
IMPORT UINT32  usb1BlkDebug;
IMPORT UINT32  usb1CbiDebug;
IMPORT UINT32  usb1KbdDebug;
IMPORT UINT32  usb1MseDebug;
IMPORT UINT32  usb1PrnDebug;
IMPORT UINT32  usb1EndDebug;
IMPORT UINT32  usb1SpkDebug;
IMPORT UINT32  usb2HlpDebug;
IMPORT UINT32  usb2MscDebug;
IMPORT UINT32  usb2MseDebug;
IMPORT UINT32  usb2KbdDebug;
IMPORT UINT32  usb2TSDebug;
IMPORT UINT32  usb2PrnDebug;
IMPORT UINT32  usb2EndDebug;
IMPORT UINT32  usb2SerDebug;
IMPORT UINT32  usbOtgDebug;
IMPORT UINT32  usbTgtDebug;
IMPORT UINT32  usbTgtRndisDebug;
IMPORT UINT32  usbTgtMscDebug;
IMPORT UINT32  usbTgtEndDebug;
IMPORT UINT32  usbTgtSerDebug;
IMPORT UINT32  usbTgtPrnDebug;
IMPORT UINT32  usbTgtKbdDebug;
IMPORT UINT32  usbPlxDebug;
IMPORT UINT32  usbPchUdcDebug;
IMPORT UINT32  usbPhyDebug;

#endif /* __INCusbOsalDebugh */

