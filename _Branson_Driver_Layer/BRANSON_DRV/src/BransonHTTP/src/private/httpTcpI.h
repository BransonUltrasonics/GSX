/* httpTcpI.h - HTTP server TCP initialization header */

/* Copyright (c) 1998-1999, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01c,09sep99,csr  merging for zlib
01b,09jun99,km   merge of Wind Web Server 2.0 code.
01a,15apr98,ck   written.
*/

#ifndef __INChttpTcpIh
#define __INChttpTcpIh

/* includes */

#include "httpType.h"

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */

HTTP_STATUS httpOsalSocketInit_persist (unsigned short usSocketNumber, 
                                        unsigned long ulAcceptIP, 
                                        unsigned short usConcurrentSockets,
                                        MEM_POOL_ID mpMemPool);

HTTP_STATUS httpOsalSocketInit_persistSsl (unsigned short usSocketNumber, 
                                           unsigned long ulAcceptIP, 
                                           unsigned short usConcurrentSockets,
                                           MEM_POOL_ID mpMemPool);

HTTP_STATUS httpOsalSocketInit_light (unsigned short usSocketNumber, 
                                      unsigned long ulAcceptIP, 
                                      MEM_POOL_ID mpMemPool);

HTTP_STATUS httpOsalSocketInit_lightSsl (unsigned short usSocketNumber, 
                                         unsigned long ulAcceptIP, 
                                         MEM_POOL_ID mpMemPool);

#ifdef __cplusplus
}
#endif

#endif /* __INChttpTcpIh */

