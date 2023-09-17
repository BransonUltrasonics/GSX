/* usrUsbInit.c - configlette and initialization of the USB stack */

/*
 * Copyright (c) 2000, 2003-2007, 2009-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
05aug16,j_x  add SYNOPSYSHCI support (US66050)
14aug15,j_x  cleanup VIP build warnings
03sep14,wyy  Init Keyboard and Mouse driver before network driver (V7CON-179)
19may14,m_y  add mhdrc legacy initialization (V7CON-127)
15may14,j_x  add otg stack initialization (V7CON-126)
14may14,m_y  Modify for mhdrc hcd driver (US22050 & US38618)
16apr14,j_x  Add O/U/XHCI vxBus GEN2 support (US22049 US22048 US25159)
17apr14,s_z  Add USB target stack support on VxBus new mode and
             legacy mode (US23307 and US34223), move ossLib init
             to usrUsbInit.c
21mar14,j_x  Enable O/U HCI related functions (VXW7-2566, 2780)
05mar14,j_x  Remove unreleased USB components for vx7 Core Profile
             (VXW7-2428)
16jan14,j_x  Decouple this file from USBD
29dec13,s_z  make USB component enable on both new VxBus model and vxBus
             legacy model, move INCLUDE_USB from usb host cdf to this file
18oct13,j_x  remove usb GEN1 components (WIND00439658)
04sep13,j_x  Add USB touch screen
09sep13,j_x  Add usbPeriphWvFilter
06may13,s_z  Remove compiler warning (WIND00356717)
28apr13,ghs  Add support of USB message (WIND00411952)
19jul12,w_x  Add support for USB 3.0 host (WIND00188662)
07jul11,ghs  Add usrUsbDebug.c to this file (WIND00286556)
17mar11,j_x  Pick off USB configlette files from usrDepend.c
16mar11,j_x  Add multiple inclusion guard symbol (WIND00247788)
14jan11,w_x  Initial USB OTG support changes (WIND00211541)
14jan11,ghs  Add GEN2 printer driver to class driver initialization
             routine (WIND00250645)
07jan11,ghs  Clean up compile warnings (WIND00247082)
14dec10,ghs  Change the usage of the reboot hook APIs to specific
             rebootHookAdd/rebootHookDelete (WIND00240804)
07dec10,ghs  Combine USB HCD initialization to one group (WIND00245236)
06sep10,j_x  Fix usb reboot hang issue (WIND00205038)
05jul10,m_y  Modify for coding convention
24may10,m_y  Add USB_WAIT_TIME_OUT_VALE define (WIND00183499)
04mar10,m_y  Add USB_CHECK_RESUBMIT_URB_FLAG define.(WIND00200272)
15mar10,j_x  Changed for USB debug (WIND00184542)
13jan10,ghs  vxWorks 6.9 LP64 adapting
09sep09,adh  Add USB_MEM_PART_SIZE define (WIND00142070)
21aug09,ghs  Add USB_HUB_POLLING_INTERVAL define(WIND00160843)
11jul07,jrp  Removing stdout announcements
20mar07,sup  remove reference to the target/h/usb2 directory
06oct06,ami  Changes for USB-vxBus porting
20apr06,ami  typo in description of the file (SPR#120283)
26jul05,ami  Function for deinitialization of stack added (SPR #110691)
24jan05,hch  Removed #if 0 code
20jan05,hch  Corrected the usage of _WRS_VXWORKS_MAJOR macro
26oct04,ami  Debug Messages Changes
15oct04,ami  Apigen Changes
11oct04,ami  Apigen Changes
06oct04,ami  SPR #94684 Fix
04oct04,tcr  update WindView logging
30aug04,pdg  ARM target support
16aug04,pdg  Fix for long booting time of OHCI and EHCI Host controllers
             in pentium
02jun04,hch  Remove compiler warning for usb2EvLog.
06apr04,cfc  Fix compiler warnings
29oct03,cfc  USB2 Merge
26sep03,cfc  Fix compiler warnings
23sep03,cfc  Support for additional usb2 windview logging
21sep03,cfc  Fix compiler warning
18sep03,cfc  remove erroneous '\' char
16sep03,cfc  Add WindView user event stub
08sep03,cfc  usb Host Stack will initialize the Hub and usbd layers
27aug03,cfc  Call HCD PCI setup routines
11aug03,cfc  Support for USB2 Host Stack
07aug03,cfc  support for usb2.0
27feb00,wef  removed unecessary printfs, fixed return codes
23aug00,wef  Created
*/

/*
DESCRIPTION

This configlette initializes a USB stack. This process is done at
boot and is usually followed by the attachment of a USB controller.
The configlette usrUsbHcdXXXXInit.c handles the respective EHCI, OHCI
or UHCI host controller attachment.
*/

#ifndef __INCusrUsbInitc
#define __INCusrUsbInitc


/* includes */

#include <usbdLib.h>
#include <usbHst.h>
#ifdef INCLUDE_USBD
#include <usbHubInitialization.h>
#endif
#include <usbOtg.h>

#if defined(INCLUDE_BOOT_APP)
#include <private/bootUsbLoad.h>
#endif
#ifdef INCLUDE_WINDVIEW
#include <wvLib.h>
#endif
#include <string.h>

/* debug level define */

#include <usrUsbDebug.c>

/* Configlettes */

#ifdef INCLUDE_USB_SHOW
    #define INCLUDE_USB
    #include <usrUsbShow.c>
#endif /* INCLUDE_USBSHOW */

/* INIT components */

#ifdef INCLUDE_USB_INIT
    #define INCLUDE_USB
#endif /* INCLUDE_USB_INIT */
#ifdef INCLUDE_USB_GEN2_KEYBOARD_INIT
    #define INCLUDE_USB_GEN2_KEYBOARD
#endif /* INCLUDE_USB_GEN2_KEYBOARD_INIT */
#ifdef INCLUDE_USB_GEN2_MOUSE_INIT
    #define INCLUDE_USB_GEN2_MOUSE
#endif /* INCLUDE_USB_GEN2_MOUSE_INIT */
#ifdef INCLUDE_USB_TOUCH_SCREEN_INIT
    #define INCLUDE_USB_TOUCH_SCREEN
#endif /* INCLUDE_USB_TOUCH_SCREEN_INIT */
#ifdef INCLUDE_USB_GEN2_PRINTER_INIT
    #define INCLUDE_USB_GEN2_PRINTER
#endif /* INCLUDE_USB_GEN2_PRINTER_INIT */
#ifdef INCLUDE_USB_GEN2_STORAGE_INIT
    #define INCLUDE_USB_GEN2_STORAGE
#endif /* INCLUDE_USB_GEN2_STORAGE_INIT */
#ifdef INCLUDE_USB_GEN2_END_INIT
    #include <usrUsb2EndInit.c>
#endif /* INCLUDE_USB_GEN2_END_INIT */
#ifdef INCLUDE_USB_GEN2_SERIAL_INIT
    #include <usrUsb2SerialInit.c>
#endif /* INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT */

/* Non-INIT Components */

#ifdef INCLUDE_USB_GEN2_MOUSE
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2MseInit.c>
#endif /* INCLUDE_USB_GEN2_MOUSE */
#ifdef INCLUDE_USB_TOUCH_SCREEN
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsbTSInit.c>
#endif /* INCLUDE_USB_TOUCH_SCREEN */
#ifdef INCLUDE_USB_GEN2_KEYBOARD
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2KbdInit.c>
#endif /* INCLUDE_USB_GEN2_KEYBOARD */
#ifdef INCLUDE_USB_GEN2_PRINTER
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2PrnInit.c>
#endif /* INCLUDE_USB_GEN2_PRINTER */
#ifdef INCLUDE_USB_GEN2_STORAGE
    #define INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2MscInit.c>
#endif /* INCLUDE_USB_GEN2_STORAGE */
#ifdef INCLUDE_USB_GEN2_HELPER
    #include <usrUsb2HelperInit.c>
#endif /* INCLUDE_USB_GEN2_HELPER */

#ifdef INCLUDE_USB_OTG
#include <usrUsbOtgInit.c>
#endif
/* defines */

#define USB2_WV_LOGSIZE          80       /* Wind View Event Log Message Size */

#ifndef USB_HUB_POLLING_INTERVAL
#define USB_HUB_POLLING_INTERVAL 2        /* Default polling interval */
#endif

#ifndef USB_MEM_PART_SIZE
#define USB_MEM_PART_SIZE        0x200000 /* Default Memory Partition Size */
#endif

#ifndef USB_CHECK_RESUBMIT_URB_FLAG
#define USB_CHECK_RESUBMIT_URB_FLAG  FALSE
#endif

#ifndef USB_WAIT_TIME_OUT_VALUE
#define USB_WAIT_TIME_OUT_VALUE  (5000)   /* Default wait time out value */
#endif

#define USB_WAIT_TIME_OUT_MIN_VALUE (5000)

/* globals */

#if defined(INCLUDE_BOOT_APP)

#ifdef INCLUDE_UHCI_INIT
extern int usbUhcdDisableHC(int startType);
#endif

#ifdef INCLUDE_OHCI_INIT
extern int usbOhciDisable(int startType);
#endif

#ifdef INCLUDE_EHCI_INIT
extern int usbEhcdDisableHC(int startType);
#endif

void bootUsbSupportInit(void);

void usbCtlrQuiescent (int startType);

#endif /* defined(INCLUDE_BOOT_APP) */

#ifdef _WRS_CONFIG_VXBUS_LEGACY
#ifdef INCLUDE_EHCI
extern VOID usbEhcdInstantiate();
#endif


#ifdef INCLUDE_OHCI
extern VOID usbOhciInstantiate();
#endif

#ifdef INCLUDE_UHCI
extern VOID usbUhcdInstantiate();
#endif
#endif

#ifdef INCLUDE_EHCI_INIT
#ifdef _WRS_CONFIG_VXBUS_LEGACY
extern VOID vxbUsbEhciRegister();
#endif
extern STATUS usbEhcdInit();
extern STATUS usbEhcdExit();
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
#ifdef _WRS_CONFIG_VXBUS_LEGACY
extern VOID vxbUsbSynopsysHciRegister();
#endif
extern STATUS usbSynopsysHcdInit (void);
extern STATUS usbSynopsysHcdExit (void);
#endif

#ifdef INCLUDE_OHCI_INIT
#ifdef _WRS_CONFIG_VXBUS_LEGACY
extern VOID vxbUsbOhciRegister();
#endif
extern STATUS usbOhcdInit();
extern STATUS usbOhcdExit();
#endif

#ifdef INCLUDE_UHCI_INIT
#ifdef _WRS_CONFIG_VXBUS_LEGACY
extern VOID vxbUsbUhciRegister();
#endif
extern STATUS usbUhcdInit();
extern STATUS usbUhcdExit();
#endif



#ifdef INCLUDE_USB_XHCI_HCD_INIT
#ifdef _WRS_CONFIG_VXBUS_LEGACY
extern VOID vxbUsbXhciRegister();
#endif
extern STATUS usbXhcdInit();
extern STATUS usbXhcdExit();
#endif
#ifdef INCLUDE_USB_MHDRC_HCD_INIT
#ifdef _WRS_CONFIG_VXBUS_LEGACY
extern STATUS vxbUsbMhdrcHcdRegister (void);
#endif
extern STATUS usbMhdrcHcdInit (void);
extern STATUS usbMhdrcHcdExit (void);
#endif

extern STATUS usbMsgLibInit (int maxCallback, int maxPendingMsg);

#ifndef _WRS_CONFIG_VXBUS_LEGACY
void usbHstClassInit (void);
#endif

extern STATUS ossInitialize(void);
extern STATUS ossShutdown(void);

/* locals */

LOCAL BOOL usbdInitByKernel = FALSE;

#ifdef INCLUDE_WINDVIEW
    LOCAL char usb2EvLog[USB2_WV_LOGSIZE];/* buffer for event logging */
#endif

LOCAL UINT32 usrUsbHubPollingInterval = USB_HUB_POLLING_INTERVAL;
LOCAL BOOL   usrUsbReSubmitUrbCheckFlag = USB_CHECK_RESUBMIT_URB_FLAG;
LOCAL int    usrUsbWaitTimeOutValue = USB_WAIT_TIME_OUT_VALUE;

UINT32     usb2WvFilter = 0;                  /* user controllable filter */
UINT32     usbPeriphWvFilter = 0x0;           /* user controllable filter */

/* USB OTG ERF events */

usbOtgEventRaisePrototype usbOtgEventRaiseFunc = NULL;
usbOtgEventDataGetPrototype usbOtgEventDataGetFunc = NULL;
usbOtgEventDataPutPrototype usbOtgEventDataPutFunc = NULL;

#ifdef INCLUDE_USBD

/*****************************************************************************
*
* usbdTranslationInit - initialize the USBD 1.1 Translation Layer
*
* This function initializes the USBD 1.1 backwards compatibility layer that
* allows class drivers written with for the USB1.1 Host Stack to operate with
* the USB2.0 Host Stack
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/


STATUS usbdTranslationInit( void )
    {

    UINT16 verStatus;
    UINT16 usbdVersion;
    char   usbdMfg [USBD_NAME_LEN+1];


    if (usbdInitByKernel)
        if (usbdInitialize() != OK)
            {
            printf(" Failed to initialize USBD \n");
            return ERROR;
            }

    if ((verStatus = usbdVersionGet (&usbdVersion, usbdMfg)) != OK)
        {
        printf ("usbdVersionGet() failed..returned %d\n", verStatus);
        return ERROR;
        }

#ifdef DEBUG_ANNOUNCE_USB
    printf("USBD  %s %i Initialized\n", usbdMfg, usbdVersion);
#endif
    return OK;
    }


/*****************************************************************************
*
* usrUsbHubInit - initialize the USB Hub Driver
*
* This function initializes the USB2.0 Hub Driver.
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/

STATUS usrUsbHubInit(void)
    {
    if (usbdInitByKernel)
        {
        if (0 == usbHubInit())
            {
#ifdef DEBUG_ANNOUNCE_USB
            printf("USB Hub Driver Initialized \n");
#endif
            return OK;
            }
        else
            {
            printf("USB Hub Driver Failed to Initialize\n");
            return ERROR;
            }
        }
    return ERROR;
    }

#endif /* INCLUDE_USBD */

/*****************************************************************************
*
* usbHcdInit - initialize USB host controller
*
* This routine initializes USB host controller
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbHcdInit(void)
    {
#ifdef INCLUDE_EHCI_INIT
    usbEhcdInit();
#endif
#ifdef INCLUDE_OHCI_INIT
    usbOhcdInit();
#endif

#ifdef INCLUDE_UHCI_INIT
    usbUhcdInit();
#endif
#ifdef INCLUDE_USB_XHCI_HCD_INIT
    usbXhcdInit();
#endif

#ifdef INCLUDE_USB_MHDRC_HCD_INIT
    usbMhdrcHcdInit ();
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
    usbSynopsysHcdInit ();
#endif
    }

/*****************************************************************************
*
* usbInit - initialize the USB2 stack
*
* This function initializes the USB2 Stack
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/

STATUS usbInit (void)
    {

    if (!usbdInitByKernel)
        {
        /* Add usb boot hook */

#if defined(INCLUDE_BOOT_APP)
        bootUsbSupportInit();
#endif
        /* Init OssLib */

        ossInitialize ();

        /* Set the default memory partition size */

        ossPartSizeSet(USB_MEM_PART_SIZE);

#ifdef INCLUDE_USBD

        /* Initialize message library */

        usbMsgLibInit(0, SEM_RW_MAX_CONCURRENT_READERS);

        /* Initialize the USB2 Host Stack */

        if (usbdInit() == OK)
            {
            usbdInitByKernel = TRUE;
            if ((usrUsbHubInit() != OK) ||
                (usbdTranslationInit() != OK))
                {
                return ERROR;
                }
            }

#endif /* INCLUDE_USBD */

        usbHcdInit();

        }

    return OK;
    }

/*****************************************************************************
*
* usbHcdExit - un - initialize USB host controller
*
* This function un - initializes USB host controller
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: N/A
*/

STATUS usbHcdExit(void)
    {
#ifdef INCLUDE_EHCI_INIT
    if (usbEhcdExit() != OK)
        {
        printf("Error exiting EHCI driver\n");
        return ERROR;
        }
#endif
#ifdef INCLUDE_OHCI_INIT
    if (usbOhcdExit() != OK)
        {
        printf("Error exiting OHCI driver\n");
        return ERROR;
        }
#endif

#ifdef INCLUDE_UHCI_INIT
    if (usbUhcdExit() != OK)
        {
        printf("Error exiting UHCI driver\n");
        return ERROR;
        }
#endif

#ifdef INCLUDE_USB_XHCI_HCD_INIT
    if (usbXhcdExit() != OK)
        {
        printf("Error exiting xHCI HCD driver\n");
        return ERROR;
        }
#endif

#ifdef INCLUDE_USB_MHDRC_HCD_INIT
    if (usbMhdrcHcdExit() != OK)
        {
        printf("Error exiting MHDRC HCD driver\n");
        return ERROR;
        }
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
    if (usbSynopsysHcdExit() != OK)
        {
        printf("Error exiting SYNOPSYSHCI HCD driver\n");
        return ERROR;
        }
#endif
    return OK;
    }

/*****************************************************************************
*
* usbExit - un - initialize the USB2 stack
*
* This function un - initializes the USB2 Stack
*
* RETURNS: OK if sucessful or ERROR if failure
*
* ERRNO: none
*/

STATUS usbExit (void)
    {

    if (usbdInitByKernel)
        {

        /* Exit HCD before exit hub and usbd */

        if (usbHcdExit() != OK)
            return ERROR;

#ifdef INCLUDE_USBD
        /* un - intialize the hub class driver */

        if (usbHubExit() != 0)
            {
            return ERROR;
            }

        /* Un - initialize the translation unit */

        if (usbdShutdown() == ERROR)
            {
            return ERROR;
            }
#endif /* INCLUDE_USBD */

        /* ossShutdown */

        ossShutdown();

        /* Set usbdInitByKernel to false */

        usbdInitByKernel = FALSE;
        }
    else
        {
        return ERROR;
        }

    return OK;
    }

/*****************************************************************************
*
* usbHcdInstantiate - instantiate the USB Host Controller Driver
*
* This routine instantiates the USB Host Controller Driver and allows
* the controller driver to be included with the vxWorks image and
* not be registered with vxBus. Devices will remain orphan devices
* until the Host Controller Init routine is called
*
* The routine call Host Controller's instantiate routine, itself does nothing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbHcdInstantiate (void)
    {
#ifdef _WRS_CONFIG_VXBUS_LEGACY
#ifdef INCLUDE_EHCI
    usbEhcdInstantiate();
#endif

#ifdef INCLUDE_OHCI
    usbOhciInstantiate();
#endif

#ifdef INCLUDE_UHCI
    usbUhcdInstantiate();
#endif

#endif
    return;
    }


#ifdef _WRS_CONFIG_VXBUS_LEGACY
/*******************************************************************************
*
* vxbUsbHciRegister - register host controller driver with vxBus
*
* This routine registers the host controller driver with vxBus. The registration is
* done for both PCI and Local bus type by calling the routine vxbDevRegister().
*
* Once the driver is registered, this routine also registers the
* Host Controller Root hub as bus-controller type with vxBus
*
* This routine call Host Controller's register routine, itself does nothing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/


void vxbUsbHciRegister (void)
    {
#ifdef INCLUDE_EHCI_INIT
    vxbUsbEhciRegister();
#endif

#ifdef INCLUDE_OHCI_INIT
    vxbUsbOhciRegister();
#endif

#ifdef INCLUDE_UHCI_INIT
    vxbUsbUhciRegister();
#endif
#ifdef INCLUDE_USB_XHCI_HCD_INIT
    vxbUsbXhciRegister();
#endif

#ifdef INCLUDE_USB_MHDRC_HCD_INIT
    vxbUsbMhdrcHcdRegister();
#endif

#ifdef INCLUDE_SYNOPSYSHCI_INIT
    vxbUsbSynopsysHciRegister();
#endif

    return;
    }
#endif

/*******************************************************************************
*
* usbHstClassInit - initialize USB host class driver
*
* This routine initializes USB host class driver. This routine call each class
* driver's initialization that included by configuration.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usbHstClassInit (void)
    {
#ifdef INCLUDE_USB_GEN2_HELPER
    usrUsb2HelperInit();
#endif

#ifdef INCLUDE_USB_GEN2_SERIAL_INIT
    usrUsb2SerialInit();
#endif

#ifdef INCLUDE_USB_GEN2_END_INIT
    usrUsb2EndInit();
#endif

#ifdef INCLUDE_USB_TOUCH_SCREEN_INIT
    usrUsbTSInit();
#endif

#ifdef INCLUDE_USB_GEN2_PRINTER_INIT
    usrUsb2PrnInit();
#endif
    }

/*****************************************************************************
*
* usb2WindViewLog - Log USB2 Host Stack WindView events
*
* The USB2 Host Stack is instrumented to provide additional WindView User
* log events for function entries/exits tracing within the Host Controllers.
* Logging is based whether or not usbHcdWvFilter has been initialized with
* the appropriate filter mask. If WindView is not enabled and the correct
* filter mask is set, then the WindView user event message will be logged
* to the standard output.
*
* \ss
*        USB_EHCD_WV_FILTER                0x00000001
*        USB_OHCD_WV_FILTER                0x00000002
*        USB_UHCD_WV_FILTER                0x00000004
*        USB_USBD_WV_FILTER                0x00000008
*        USB_HUB_WV_FILTER                0x0000000F
* \se
*
* RETURNS: N/A
*
* ERRNO: none
*/

void usb2WindViewLog
    (
    UINT32 evId,
    char *buffer,
    UINT32 mask
    )
    {

    if ((usb2WvFilter & mask) == mask)
        {
#ifdef INCLUDE_WINDVIEW
  #if (defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
        int     length;

        strncpy(usb2EvLog, buffer, USB2_WV_LOGSIZE);
        usb2EvLog [USB2_WV_LOGSIZE - 1] = '\0';
        length = (int)strlen (usb2EvLog);
        EVT_STR_N(evId, 0, 0, 0, 0, 0, 0, 0,
                  length, usb2EvLog);
  #else /* _WRS_VXWORKS_MAJOR */
        strncpy(usb2EvLog, buffer, USB2_WV_LOGSIZE);
        usb2EvLog [USB2_WV_LOGSIZE - 1] = '\0';
        wvEvent (evId, usb2EvLog, strlen (usb2EvLog));
  #endif /* _WRS_VXWORKS_MAJOR */
#else /* INCLUDE_WINDVIEW */
  #ifdef INCLUDE_LOGGING
        logMsg("USB2 wvEvent: %s\n",(long)buffer,0,0,0,0,0);
  #endif
#endif /* INCLUDE_WINDVIEW */
        }
    }


/*****************************************************************************
*
* usbHubPollingIntervalGet - Get usb hub polling interval
*
* This routine return usb hub polling interval. This value can be configured
* by user to modify the define of USB_HUB_POLLING_INTERVAL.
*
* RETURNS : Value of hub polling interval by milliseconds.
*
* ERRNO: none
*/

UINT32 usbHubPollingIntervalGet(void)
    {
    return usrUsbHubPollingInterval;
    }


/*****************************************************************************
*
* usbHubPollingIntervalSet - Set usb hub polling interval
*
* This routine is used to set usb hub polling interval. User can also modify
* USB_HUB_POLLING_INTERVAL to change this value.
*
* RETURNS : N/A
*
* ERRNO: none
*/

void usbHubPollingIntervalSet(UINT32 interval)
    {
    usrUsbHubPollingInterval = interval;
    return;
    }

/*****************************************************************************
*
* usrUsbResubmitUrbCheckEnabled - get the resubmit urb check flag
*
* This routine is used to get the resubmit urb check flag.
*
* RETURNS : Value of resubmit urb check flag.
*
* ERRNO: none
*/

BOOL usrUsbResubmitUrbCheckEnabled
    (
    void
    )
    {
    return usrUsbReSubmitUrbCheckFlag;
    }

/*****************************************************************************
*
* usrUsbWaitTimeOutValueGet - Get usb wait time out value
*
* This routine return usb wait time out value. This value can be configured
* by user to modify the define of USB_WAIT_TIME_OUT_VALUE.
*
* RETURNS : Value of usb wait time out by milliseconds.
*
* ERRNO: none
*/

int usrUsbWaitTimeOutValueGet(void)
    {
    /*
     * If user set this value smaller than "5000", still return "5000"
     * as this is the short time limit.
     */

    if (usrUsbWaitTimeOutValue == WAIT_FOREVER)
        return WAIT_FOREVER;
    if (usrUsbWaitTimeOutValue < USB_WAIT_TIME_OUT_MIN_VALUE)
        return OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(USB_WAIT_TIME_OUT_MIN_VALUE);
    else
        return OS_CONVERT_MILLISECONDS_TO_WAIT_VALUE(usrUsbWaitTimeOutValue);
    }

#if defined(INCLUDE_BOOT_APP)

/*******************************************************************************
*
* bootUsbSupportInit - initialize boot loader USB support
*
* This routine initializes the boot loader application USB support.
* It adds a routine to the boot shell to reset all the usb host controllers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void bootUsbSupportInit(void)
    {
    /* Setup the function pointer to stop all USB controllers */

    bootUsbStopAllFunc = usbCtlrQuiescent;

    return;
    }


/*******************************************************************************
*
* usbCtlrQuiescent - reset and put all USB controllers in quiescent mode
*
* This routine will reset and put all USB controllers in quiescent mode.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void usbCtlrQuiescent(int startType)
    {
#ifdef INCLUDE_UHCI_INIT
    usbUhcdDisableHC(startType);
#endif

#ifdef INCLUDE_OHCI_INIT
    usbOhciDisable(startType);
#endif

#ifdef INCLUDE_EHCI_INIT
    usbEhcdDisableHC(startType);
#endif

   /*
    * If other USB contorllers (either host or device controllers)
    * are to be included in bootrom, then apropriate code can be
    * added here to reset and put these controllers into known state.
    */

    return;
    }

#endif /* defined(INCLUDE_BOOT_APP) */

#endif /* __INCusrUsbInitc */
