/* usb2Kbd.h - Generation 2 USB keyboard driver */

/*
 * Copyright (c) 2008-2010, 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20aug14,bbz  cleanup warnings of no check of the return value (US44214)
01f,02may13,ghs  Modify attach to shell work mode (WIND00415711)
01e,29apr10,pad  Moved extern C statement after include statements.
01d,13jan10,ghs  vxWorks 6.9 LP64 adapting
01c,09sep09,y_l  Add usb2KbdRcvCallback for bootAppShell (WIND00130193)
01b,17apr09,adh  added feature to attach keyboard to shell
01a,05Nov08,adh  written

*/
#ifndef __INCusb2Kbdh
#define __INCusb2Kbdh

/* includes */
#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* USB keyboard protocol codes */

#define USB2_KBD_PROTOCOL                       0x01    /* keyboard protocol */

#define USB2_KBD_KEY_LEFT_CTRL_MODIFIER         0x01
#define USB2_KBD_KEY_LEFT_SHIFT_MODIFIER        0x02
#define USB2_KBD_KEY_LEFT_ALT_MODIFIER          0x04
#define USB2_KBD_KEY_LEFT_GUI_MODIFIER          0x08
#define USB2_KBD_KEY_RIGHT_CTRL_MODIFIER        0x10
#define USB2_KBD_KEY_RIGHT_SHIFT_MODIFIER       0x20
#define USB2_KBD_KEY_RIGHT_ALT_MODIFIER         0x40
#define USB2_KBD_KEY_RIGHT_GUI_MODIFIER         0x80

/* HID keyboard LED definitions for output report */

#define USB2_KBD_LED_NUM_LOCK_REPORT            0x01
#define USB2_KBD_LED_CAPS_LOCK_REPORT           0x02
#define USB2_KBD_LED_SCROLL_LOCK_REPORT         0x04
#define USB2_KBD_LED_COMPOSE_REPORT             0x08
#define USB2_KBD_LED_KANA_REPORT                0x10

/* Typedefs */

/* forward declarations */
extern void usb2KbdDeinit
    (
    void
    );

extern STATUS usb2KbdInit
    (
    char *                pName,            /* Default drv base name or NULL */
    UINT32                idleRate,         /* typematic rate in ms */
    UINT16                countryCode       /* default country code */
    );

extern BOOL usb2KbdIsRedirectTty
    (
    void
    );

#ifdef __cplusplus
}
#endif
#endif  /* __INCusb2Kbdh */
