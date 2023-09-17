/* usbLib.h - USB utility functions */

/*
 * Copyright (c) 1999-2000, 2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1999-2000, 2010, 2014 Wind River Systems, Inc.

   This software includes software licensed to Wind River Systems, Inc.
   by Wipro, Ltd. Wind River licensees may use this software according
   to the terms of their Wind River license agreement(s) applicable to
   this software.
*/

/*
Modification history
--------------------
    16jan14,j_x  Code clean for TU
01f,29apr10,pad  Moved extern C statement after include statements.
01e,13jan10,ghs  vxWorks 6.9 LP64 adapting
01d,26jan00,rcb  Modify usbRecurringTime() to accept <bandwidth> instead
                 of <bytesPerFrame>.
                 Add usbDescrCopy32() and usbDescrStrCopy32().
01c,17jan99,rcb  Add usbConfigDescrGet() function.
01b,23nov99,rcb  Add usbRecurringTime() function.
01a,16jul99,rcb  First.
*/

#ifndef __INCusbLibh
#define __INCusbLibh


/* includes */
#include "usbdLib.h" 

#ifdef  __cplusplus
extern "C" {
#endif


/* function prototypes */

UINT32 usbTransferTime
    (
    UINT16 transferType,    /* transfer type */
    UINT16 direction,       /* transfer direction */
    UINT16 speed,           /* speed of pipe */
    UINT32 bytes,           /* number of bytes for packet to be calc'd */
    UINT32 hostDelay,       /* host controller delay per packet */
    UINT32 hostHubLsSetup   /* host controller time for low-speed setup */
    );

UINT32 usbRecurringTime
    (
    UINT16 transferType,    /* transfer type */
    UINT16 direction,       /* transfer direction */
    UINT16 speed,           /* speed of pipe */
    UINT16 packetSize,      /* max packet size for endpoint */
    UINT32 bandwidth,       /* number of bytes to transfer per second */
    UINT32 hostDelay,       /* host controller delay per packet */
    UINT32 hostHubLsSetup   /* host controller time for low-speed setup */
    );

pVOID usbDescrParseSkip
    (
    pUINT8 *ppBfr,          /* buffer to parse */
    pUINT16 pBfrLen,        /* length of buffer to parse */
    UINT8 descriptorType    /* type of descriptor being sought */
    );

pVOID usbDescrParse
    (
    pUINT8 pBfr,            /* buffer to parse */
    UINT16 bfrLen,          /* length of buffer to parse */
    UINT8 descriptorType    /* type of descriptor being sought */
    );

#ifdef  __cplusplus
}
#endif

#endif  /* __INCusbLibh */

/* End of file. */

