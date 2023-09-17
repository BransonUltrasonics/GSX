/* httpMimeAcceptEncoding.c - Parsing functions for MIME header */

/* Copyright 1999-2003,2010,2014 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
27jan10,h_y  support for LP64
31oct03,adb  handling encoding different than x-gzip or gzip (SRR 90771)
31aug99,va   created.
*/
/*
DESCRIPTION
This module contains HTTP_REQ access functions useful for MIME
<Accept-Encoding:> determination.

INCLUDE FILES:
	private/httpReq.h
	httpLib.h
	httpSem.h

*/

/*includes*/
#include <string.h>

#include "httpLib.h"
#include "httpReq.h"
#include "httpSem.h"

HTTP_STATUS httpMimeAcceptEncoding(HTTP_REQ_ID reqId,char *      szLine );
/*******************************************************************************
*
* httpMimeAcceptEncoding - Parse request header for MIME <Accept-Encoding:>.
*
* This routine is called by the MIME parser to extract
* the <Accept-Encoding> header from the client's HTTP request.
* The MIME parser calls httpMimeAcceptEncoding() only if <Accept-Encoding:>
* is found in a request.
*
*
* NOTE: Do not modify <szLine>.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeAcceptEncoding
    (
    HTTP_REQ_ID reqId,   /* Handle of the active request */
    char *      szLine   /* Line to parse                */
    )
    {
    char * szType;
    char * szNext;
    int encXGzip =0;

#ifdef DEBUG_INFO_MAE
    fprintf(stderr, "httpMimeAcceptEncoding: szLine %s \n", szLine);
    fflush(stderr);
#endif
    szType = strchr (szLine, ' ');
    if  (szType == NULL)
        {
        szType = szLine;
        httpSetEnv (reqId , "ACCEPT_ENCODING" , szType);
#ifdef DEBUG_INFO_MAE
        fprintf(stderr, "httpMimeAcceptEncoding: Accept encoding- None %s \n", szType);
        fflush(stderr);
#endif
	(void) httpMimeHdrSet (reqId, HDR_IN, "Content-Encoding", "");
	return HTTP_OK;
	}
    else
        szType++;
    szNext = szType;

    httpSetEnv (reqId , "ACCEPT_ENCODING" , szNext);

#ifdef DEBUG_INFO_MAE
    fprintf(stderr,"httpMimeAcceptEncoding: Accept encoding supported %s\n",szType);
    fflush(stderr);
#endif

    if  (szNext != NULL)
        {
        while (*szNext != '\0')
               /* checking for end of string */
            {


            if  (httpStrNCaseCmp(szNext,"x-gzip", 6) ==0)
                {
                (void) httpMimeHdrSet (reqId, HDR_IN, "Content-Encoding","x-gzip");
                szNext = strchr (szType, ',');
                encXGzip = 1;
                                
                if  (szNext == NULL)
                    {
                    break;
                    }
                }


            if  (httpStrNCaseCmp(szNext,"gzip", 4) ==0)
                {
                (void) httpMimeHdrSet (reqId, HDR_IN, "Content-Encoding", "gzip");
#ifdef DEBUG_INFO_MAE
                fprintf(stderr, "httpMimeAcceptEncoding: Accept encoding-gzip \n");
                fflush(stderr);
#endif
                return HTTP_OK;
                }

            szNext++;
            }
        }

#ifdef DEBUG_INFO_MAE
    if  (encXGzip ==1)
        {
        fprintf(stderr, "httpMimeAcceptEncoding: Accept encoding x-gzip \n");
        fflush(stderr);
	}
    else
	{
        fprintf(stderr, "httpMimeAcceptEncoding: Accept encoding unknown %s\n",szType);
        fflush(stderr);
	}
#endif
    if  (encXGzip ==0)
        (void) httpMimeHdrSet (reqId, HDR_IN, "Content-Encoding", "");

    logInfoMsg ("MIME Accept-Encoding: >%s< detected",szType);

    return HTTP_OK;
    }
