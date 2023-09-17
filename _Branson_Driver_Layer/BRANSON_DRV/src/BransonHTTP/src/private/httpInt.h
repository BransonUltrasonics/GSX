/* httpInt.h - HTTP server internal interfaces header */

/* Copyright (c) 1996-1999, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01d,09sep99,csr  merging for zlib
01c,09jun99,km   merge of Wind Web Server 2.0 code.
01b,08oct96,wk   adapted for vxWorks 5.2 on UNIX host and m68k target.
01a,03sep96,ck   written.
*/

#ifndef __INChttpInth
#define __INChttpInth

/* includes */

#include "httpType.h"
#include "httpReq.h"
#include "memPoolLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

typedef struct methods
    {
    const short  mNumber;
    const char * const szName;
    } METHODS;

/* globals */

extern const HTTP_STATUS_MSG   httpStatusMessages[];
extern const METHODS           httpMethods [];
extern const char * const      httpProtocolStrings[];
extern const char              httpNullByte;

/* function declarations */

HTTP_STATUS       httpMimeParse (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS       httpDispatcher (HTTP_REQ_ID reqId);
HTTP_STATUS       httpReqTrans (HTTP_REQ_ID reqId);
char *            httpStatus (short);      /* give the string to a specified error nr */
HTTP_STATUS       httpGenericPtrPtrTableAppend (void ** ppTable, short sMaxTableLength, void * szOne, void * szTwo);
MEM_POOL_ID  	  httpReqMemPartCreate (unsigned long ulSize);
HTTP_REQ_ID 	  httpReqStructInit (MEM_POOL_ID memId);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpInth */

