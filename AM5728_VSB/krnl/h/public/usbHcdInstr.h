/* usbHcdInstr.h - WindView Instrumentation definitions for HCD module */

/* Copyright 2004 Wind River Systems, Inc. 

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according  
   to the terms of their Wind River license agreement(s) applicable to 
   this software.
*/

/*
modification history
--------------------
01b,22sep04,tcr  move event ids from user-event range
01a,03jun03,nld Initial Version
*/

/*
DESCRIPTION

This file contains the definitions for the WindView Instrumentation for all 
three HCD modules i.e. EHCD, OHCD and UHCD.
*/

#ifndef __INCusbHcdInstrh
#define __INCusbHcdInstrh

#ifdef	__cplusplus
extern "C" {
#endif

/* defines */

/* Event IDs for EHCD module */
#define USB_EHCI_WV_INIT_EXIT		10150
#define USB_EHCI_WV_EVENT_HANDLER	10151
#define USB_EHCI_WV_PORT_ROUTE          10152
#define USB_EHCI_WV_TRANSFER		10153

/* Event IDs for OHCD module */
#define USB_OHCI_WV_INIT_EXIT		10160
#define USB_OHCI_WV_EVENT_HANDLER	10161
#define USB_OHCI_WV_TRANSFER		10162

/* Event IDs for UHCD module */
#define USB_UHCI_WV_INIT_EXIT		10170
#define USB_UHCI_WV_EVENT_HANDLER	10171
#define USB_UHCI_WV_TRANSFER		10172

/* Event IDs for USBD module */
#define USB_USBD_WV_INIT_EXIT		10180
#define USB_USBD_WV_REGISTER      	10181
#define USB_USBD_WV_DEVICE		10182
#define USB_USBD_WV_REQUEST		10183


/* Event IDs for HUB module */
#define USB_HUB_WV_INIT_EXIT		10190
#define USB_HUB_WV_EVENT_HANDLER	10191
#define USB_HUB_WV_DEVICE  	        10192
#define USB_HUB_WV_TRANSFER		10193

    
/* Event filters */
#define	USB_EHCD_WV_FILTER	      	0x00000001
#define	USB_OHCD_WV_FILTER	        0x00000002
#define	USB_UHCD_WV_FILTER	        0x00000004
#define	USB_USBD_WV_FILTER	        0x00000008
#define	USB_HUB_WV_FILTER	        0x0000000F

extern void usb2WindViewLog (UINT32, char *, UINT32);

/* Macro to log the instrumentation event */
#define USB_HCD_LOG_EVENT(eventId, eventBuf, USB2_FILTER)					\
         usb2WindViewLog(eventId, eventBuf, USB2_FILTER);

#define USB_USBD_LOG_EVENT(eventId, eventBuf, USB2_FILTER)					\
         usb2WindViewLog(eventId, eventBuf, USB2_FILTER);

#define USB_HUB_LOG_EVENT(eventId, eventBuf, USB2_FILTER)					\
         usb2WindViewLog(eventId, eventBuf, USB2_FILTER);

#ifdef	__cplusplus
}
#endif

#endif	/* __INCusbHcdInstrh */

/* End of file usbHcdInstr.h */

