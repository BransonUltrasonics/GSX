/* usrUsbDebug.c - USB debug definitions */

/*
 * Copyright (c) 2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
09Oct14,ljg  Add USB Pch Udc Tcd support
14may14,m_y  Modify USB_TRACK_MHDRCHCD to USB_TRACK_MHDRC
29dec13,s_z  Add USB PHY debug support
01sep13,j_x  Add touch screen
25sep12,j_x  Add XHCD debug support (WIND00378698)
16may12,s_z  Add the PLX USB 3.0 TCD driver debug support (WIND00306135)
16aug11,j_x  Unify name of MHCD to MHDRCHCD (WIND00288821)
22apr11,ljg  Add USBTGT printer and USBTGT keyboard emulators debug support
17mar11,m_y  Add USB target MSC function driver debug support
12feb11,s_z  Add USB target management level debug support
             Add USB target RNDIS function driver debug support
30jan11,ghs  Add target serial emulator support
13jan11,w_x  Initial USB OTG support changes (WIND00211541)
16mar11,j_x  Add multiple inclusion guard symbol (WIND00247788)
09jul10,m_y  Add synopsys hcd debug
10jun10,ghs  Add usb serial debug
05feb10,j_x  written for requirement (WIND00184542)
*/

/*
DESCRIPTION

This file contains the definitions of debugging macros and global variables

INCLUDE FILES: usb/usbOsalDebug.h
*/

#ifndef __INCusrUsbDebugc
#define __INCusrUsbDebugc

/* includes */

#include <usbOsalDebug.h>

/* defines */

#ifndef USB_TRACK_XHCD
#define USB_TRACK_XHCD            0
#endif /* XHCD debug information */

#ifndef USB_TRACK_MHDRC
#define USB_TRACK_MHDRC            0
#endif /* MHDRC debug information */

#ifndef USB_TRACK_EHCD
#define USB_TRACK_EHCD            0
#endif /* EHCD debug information */

#ifndef USB_TRACK_OHCD
#define USB_TRACK_OHCD            0
#endif /* OHCD debug information */

#ifndef USB_TRACK_UHCD
#define USB_TRACK_UHCD            0
#endif /* UHCD debug information */

#ifndef USB_TRACK_SYNOPSYSHCD
#define USB_TRACK_SYNOPSYSHCD     0
#endif /* UHCD debug information */

#ifndef USB_TRACK_USBD
#define USB_TRACK_USBD            0
#endif /* USBD debug information */

#ifndef USB_TRACK_TU
#define USB_TRACK_TU              0
#endif /* TU debug information */

#ifndef USB_TRACK_HUB
#define USB_TRACK_HUB             0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN1_BLK
#define USB_TRACK_GEN1_BLK        0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN1_CBI
#define USB_TRACK_GEN1_CBI        0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN1_KBD
#define USB_TRACK_GEN1_KBD        0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN1_MSE
#define USB_TRACK_GEN1_MSE        0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN1_PRN
#define USB_TRACK_GEN1_PRN        0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN1_END
#define USB_TRACK_GEN1_END        0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN1_SPK
#define USB_TRACK_GEN1_SPK        0
#endif /* HUB debug information */

#ifndef USB_TRACK_GEN2_HLP
#define USB_TRACK_GEN2_HLP        0
#endif /* debug information */

#ifndef USB_TRACK_GEN2_MSC
#define USB_TRACK_GEN2_MSC        0
#endif /* debug information */

#ifndef USB_TRACK_GEN2_MSE
#define USB_TRACK_GEN2_MSE        0
#endif /* debug information */

#ifndef USB_TRACK_GEN2_KBD
#define USB_TRACK_GEN2_KBD        0
#endif /* debug information */

#ifndef USB_TRACK_TOUCH_SCREEN
#define USB_TRACK_TOUCH_SCREEN    0
#endif /* debug information */

#ifndef USB_TRACK_GEN2_PRN
#define USB_TRACK_GEN2_PRN        0
#endif /* debug information */

#ifndef USB_TRACK_GEN2_END
#define USB_TRACK_GEN2_END        0
#endif /* debug information */

#ifndef USB_TRACK_GEN2_SER
#define USB_TRACK_GEN2_SER        0
#endif /* debug information */

#ifndef USB_TRACK_OTG
#define USB_TRACK_OTG             0
#endif /* debug information */

#ifndef USB_TRACK_TGT
#define USB_TRACK_TGT             0
#endif /* debug information */

#ifndef USB_TRACK_TGT_END
#define USB_TRACK_TGT_END         0
#endif

#ifndef USB_TRACK_TGT_RNDIS
#define USB_TRACK_TGT_RNDIS       0
#endif /* debug information */

#ifndef USB_TRACK_TGT_SER
#define USB_TRACK_TGT_SER         0
#endif /* debug information */

#ifndef USB_TRACK_TGT_MSC
#define USB_TRACK_TGT_MSC         0
#endif /* debug information */

#ifndef USB_TRACK_TGT_PRN
#define USB_TRACK_TGT_PRN         0
#endif /* debug information */

#ifndef USB_TRACK_TGT_KBD
#define USB_TRACK_TGT_KBD         0
#endif /* debug information */

#ifndef USB_TRACK_PLXTCD
#define USB_TRACK_PLXTCD          0
#endif /* USB_TRACK_PLX */

#ifndef USB_TRACK_PCHUDCTCD
#define USB_TRACK_PCHUDCTCD       0
#endif /* debug information */

#ifndef USB_TRACK_PHY
#define USB_TRACK_PHY             0
#endif /* USB_TRACK_PHY */

/* globals */

UINT32  usbXhcdDebug = USB_TRACK_XHCD;
UINT32  usbMhdrcDebug = USB_TRACK_MHDRC;
UINT32  usbEhcdDebug = USB_TRACK_EHCD;
UINT32  usbOhcdDebug = USB_TRACK_OHCD;
UINT32  usbUhcdDebug = USB_TRACK_UHCD;
UINT32  usbShcdDebug = USB_TRACK_SYNOPSYSHCD;
UINT32  usbUsbdDebug = USB_TRACK_USBD;
UINT32  usbTuDebug   = USB_TRACK_TU;
UINT32  usbHubDebug  = USB_TRACK_HUB;
UINT32  usb1BlkDebug = USB_TRACK_GEN1_BLK;
UINT32  usb1CbiDebug = USB_TRACK_GEN1_CBI;
UINT32  usb1KbdDebug = USB_TRACK_GEN1_KBD;
UINT32  usb1MseDebug = USB_TRACK_GEN1_MSE;
UINT32  usb1PrnDebug = USB_TRACK_GEN1_PRN;
UINT32  usb1EndDebug = USB_TRACK_GEN1_END;
UINT32  usb1SpkDebug = USB_TRACK_GEN1_SPK;
UINT32  usb2HlpDebug = USB_TRACK_GEN2_HLP;
UINT32  usb2MscDebug = USB_TRACK_GEN2_MSC;
UINT32  usb2MseDebug = USB_TRACK_GEN2_MSE;
UINT32  usb2KbdDebug = USB_TRACK_GEN2_KBD;
UINT32  usb2TSDebug  = USB_TRACK_TOUCH_SCREEN;
UINT32  usb2PrnDebug = USB_TRACK_GEN2_PRN;
UINT32  usb2EndDebug = USB_TRACK_GEN2_END;
UINT32  usb2SerDebug = USB_TRACK_GEN2_SER;
UINT32  usbOtgDebug = USB_TRACK_OTG;
UINT32  usbTgtDebug = USB_TRACK_TGT;
UINT32  usbTgtRndisDebug = USB_TRACK_TGT_RNDIS;
UINT32  usbTgtEndDebug   = USB_TRACK_TGT_END;
UINT32  usbTgtSerDebug = USB_TRACK_TGT_SER;
UINT32  usbTgtMscDebug = USB_TRACK_TGT_MSC;
UINT32  usbTgtPrnDebug = USB_TRACK_TGT_PRN;
UINT32  usbTgtKbdDebug = USB_TRACK_TGT_KBD;
UINT32  usbPlxDebug = USB_TRACK_PLXTCD;
UINT32  usbPchUdcDebug = USB_TRACK_PCHUDCTCD;
UINT32  usbPhyDebug = USB_TRACK_PHY;

#endif /* __INCusrUsbDebugc */
