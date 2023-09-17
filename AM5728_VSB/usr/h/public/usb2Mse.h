/* usb2Mse.h - Generation 2 USB mouse driver */

/*
 * Copyright (c) 2008, 2010, 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
07jul14,j_x  add RTP support (V7CON-161)
25sep13,j_x  add wheel support
29apr10,pad  Moved extern C statement after include statements.
23apr09,w_x  coding convention changes
17apr07,w_x  added HID mouse report definitions
05nov08,adh  written
*/

#ifndef __INCusb2Mseh
#define __INCusb2Mseh

/* includes */

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* USB mouse protocol codes */

#define USB2_MSE_PROTOCOL               0x02 /* mouse protocol (HID spec) */

/* HID mouse report definitions */

#define USB2_MSE_BUTTON_1               0x01
#define USB2_MSE_BUTTON_2               0x02
#define USB2_MSE_BUTTON_3               0x04

/* typedefs */

typedef struct usb2_mse_report
    {   
    UINT8 buttonState;              /* Button State */
    INT8 xMove;                     /* signed x-displacement */
    INT8 yMove;                     /* signed y-displacement */
    INT8 wheel;                     /* whell value */
    } __attribute__((packed)) USB2_MSE_REPORT;

/* forward declarations */

extern void usb2MseDeinit
    (
    void
    );

extern STATUS usb2MseInit
    (
    char *  pName /* Default base name that will be used, or NULL if no names */
    );

#ifdef __cplusplus
}
#endif
#endif  /* __INCusb2Mseh */
