/* usb2KbdMap.h - Generation 2 USB keyboard mapper */

/*
 * Copyright (c) 2008-2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
24sep14,lan  Support extended key (V7CON-187)
29apr10,pad  Moved extern C statement after include statements.
13jan10,ghs  vxWorks 6.9 LP64 adapting
17apr09,adh  keyboard mapping changes
05Nov08,adh  written
*/

#ifndef __INCusb2KbdMaph
#define __INCusb2KbdMaph

/* includes */
#include <vxWorks.h>
#include <usb2Kbd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USB2_KBD_VALID_SCANCODE_LIMIT    0x3
#define USB2_KBD_BOOT_KEY_COUNT         (0x06)    /* size of scan codes */
#define USB2_KBD_ASCII_KEY_COUNT        (USB2_KBD_BOOT_KEY_COUNT * 2)    /* size of ASCII code */


/* These are the different modifier keys possibles as of HID spec */

#define USB2_KBD_NO_MODIFIER            (0x00)
#define USB2_KBD_LEFT_CTRL              (0x01)
#define USB2_KBD_LEFT_SHIFT             (0x02)
#define USB2_KBD_LEFT_ALT               (0x04)
#define USB2_KBD_LEFT_GUI               (0x08)
#define USB2_KBD_RIGHT_CTRL             (0x10)
#define USB2_KBD_RIGHT_SHIFT            (0x20)
#define USB2_KBD_RIGHT_ALT              (0x40)
#define USB2_KBD_RIGHT_GUI              (0x80)
#define USB2_KBD_FILTER_KEY             (0xFF)

/*
 * These are our key codes to define when one of the Caps, num, scr lock keys
 * has been pressed
 */

#define USB2_KBD_CAPS_KEY               0xF8
#define USB2_KBD_NUM_KEY                0xF9
#define USB2_KBD_SCR_KEY                0xFA
#define USB2_KBD_COMPOSE_KEY            0xFB
#define USB2_KBD_KANA_KEY               0xFC
#define USB2_KBD_CUSTOM_1_KEY           0xFD
#define USB2_KBD_CUSTOM_2_KEY           0xFE
#define USB2_KBD_CUSTOM_3_KEY           0xFF

#define USB2_KBD_LED_KEY_LIMIT          0xF8

#define USB2_KBD_MAX_MODIFIERS_PER_MAP   2
#define USB2_KBD_MAX_SCANCODES_PER_MAP  0x64

/* This defines a keyboard report */

typedef struct usb2_kbd_report
    {
    UINT8   modifiers;                                /* modifier keys */
    UINT8   reserved;                                 /* reserved */
    UINT8   scanCodes [USB2_KBD_BOOT_KEY_COUNT];      /* scan codes */
    } __attribute__ ((packed)) USB2_KBD_REPORT;

typedef struct usb2_kbd_modifier_map
    {
    UINT8   pModifiers[USB2_KBD_MAX_MODIFIERS_PER_MAP]; /* 0 terminated array */
    UINT8   pCharMap[USB2_KBD_MAX_SCANCODES_PER_MAP];   /* Char array */
    } USB2_KBD_MODIFIER_MAP;

typedef struct usb2_kbd_filter
    {
    UINT8   filterScanCode;     /* 0 terminated array */
    UINT8   modifierOffMask;
    UINT8   modifierOffIndex;
    UINT8   filterKeyCode;
    } USB2_KBD_FILTER;

typedef struct usb2_kbd_filter_map
    {
    USB2_KBD_FILTER filter;
    UINT8           pCharMap[USB2_KBD_MAX_SCANCODES_PER_MAP];  /* Char array */
    } USB2_KBD_FILTER_MAP;

typedef struct usb2_kbd_extender_map
    {
    UINT8   pModifiers[USB2_KBD_MAX_MODIFIERS_PER_MAP]; /* 0 terminated array */
    UINT8   pCharMap[USB2_KBD_MAX_SCANCODES_PER_MAP];   /* Char array */
    } USB2_KBD_EXTENDER_MAP;

typedef struct usb2_kbd_map
    {
    USB2_KBD_MODIFIER_MAP *     pModifierMap;
    USB2_KBD_FILTER_MAP   *     pFilterMap;
    USB2_KBD_EXTENDER_MAP *     pExtenderMap;
    UINT32                      maxKeyMaps;
    UINT32                      maxFilters;
    UINT32                      maxExtenders;
    UINT16                      countryCode;
    } USB2_KBD_MAP;

typedef struct usb2_map_handle
    {
    USB2_KBD_MAP *      pMap;
    UINT8               filterKeyMask;
    } USB2_KBD_MAP_HANDLE;

/* forward declarations */

extern ssize_t usb2KbdReport2Ascii
    (
    USB2_KBD_MAP_HANDLE *   pMapHandle, /* Char Mapping used */
    USB2_KBD_REPORT *       pReport,    /* Report used, scancodes transformed */
    UCHAR *                 pAscii      /* ascii array to be filled */
    );

extern STATUS usb2KbdMapInit();

STATUS usb2KbdMapAdd
    (
    USB2_KBD_MAP * pMap                /* Keyboard mapping to add to the list */
    );

USB2_KBD_MAP * usb2CntryCodeToMap
    (
    UINT16 countryCode                 /* language Id to get the Map */
    );

/* Default keyboard mapping used */

extern USB2_KBD_MAP usb2Kbd_Map_US;

#ifdef __cplusplus
}
#endif

#endif  /* __INCusb2KbdMap */
