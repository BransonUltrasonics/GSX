/* httpDate.h - HTTP server system date and time header */
 
/* Copyright (c) 1998-1999, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01b,22jun99,km   merge of Wind Web Server 2.0 code.
01a,08jul98,ck   written.
*/

#ifndef __INChttpDateh
#define __INChttpDateh

/* includes */

#include "httpLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */

HTTP_STATUS httpOsalDateToString (char * const  szDate,  unsigned short usMaxlen, unsigned long ulSeconds);
HTTP_STATUS httpOsalDateGet (char * const szDate);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpDateh */

