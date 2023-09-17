/* httpMimeParse.c - Mime header parser */

/*
 * Copyright (c) 1996-2004, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03b,10jul12,r_w  NULL pointer process to prevent exception
                 defect: WIND00358669
03a,27jan10,h_y  support for LP64
02i,14jan04,gsr  cleanup routine for SPR 93009
02h,07nov03,adb  WRS coding conventions modifications
02g,31oct03,adb  added httpMimeParserConfigurationShow routine
02f,30sep03,jws  remove assert()'s
02e,18aug03,jws  remove HTTP_BOOL
02d,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02c,14apr98,mb   API change for httpGenListIterator() integrated.
02b,05apr98,ck   minor bugfix (debugging info).
02a,29may97,mb   generic lists introduced; httpMimeParseFnConfInit() and
                 httpMimeParseFnConfAdd() moved here.
01d,21may97,mb   MimeDispatchTab renamed to httpMimeDispatchTab.
01c,13may97,mb   osal introduced.
01b,30sep96,ck   seperated from mime_par.c (scalability).
01a,15jun96,ck   initial creation.
*/

/*
DESCRIPTION
This module contains the MIME header processing facilities.
If this feature is enabled in the httpconf.h configuration file,
custom and system MIME header processing functions may be registered
at this module and can parse and store any MIME header information.

INCLUDE FILES: httpLib private/httpReq.h httpUtil.h httpShow.h
*/

/* includes */

#include <string.h>

#include "httpLib.h"
#include "httpReq.h"
#include "httpUtil.h"
#include "httpShow.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_MIME_GUESSTYPE
/* locals */

LOCAL unsigned short    httpMimeParseFnMax;
LOCAL HTTP_GEN_LIST_ID  httpMimeParseConfigId;

HTTP_GEN_LIST_ID httpMimeParseFnConfInit( unsigned short sMaxEntries );
HTTP_STATUS httpMimeParseFnConfCleanup(void);
HTTP_GEN_LIST_ENTRY_ID httpMimeParseFnConfAdd(char * szMimeType,HTTP_MIME_TOKEN_PARSER pMimeParser );
HTTP_STATUS httpMimeParse(HTTP_REQ_ID reqId,char *  szLine);


/***************************************************************************
*
* httpMimeParseFnConfInit - initialize the sizes of the MIME parsing tables
*
* This routine initializes the MIME parse function configuration table.
*
* RETURNS: List ID of the MIME parsing function list.
*
* SEE ALSO: httpMimeParseFnConfAdd()
*/

HTTP_GEN_LIST_ID httpMimeParseFnConfInit
    (
    unsigned short sMaxEntries /* Maximum number of entries in predefined table */
    )
    {
    httpMimeParseFnMax = sMaxEntries;
    return ( httpMimeParseConfigId = httpGenListCreate ( httpGlobalMemPartIdGet() , httpMimeParseFnMax , HTTP_TRUE) );
    }

/***************************************************************************
*
* httpMimeParseFnConfCleanup - Cleanup routine for httpMimeParseFnConfInit()
*
* RETURNS: HTTP_OK always
*
* SEE ALSO: httpMimeParseFnConfInit()
*/

HTTP_STATUS httpMimeParseFnConfCleanup
    (
    void
    )
    {
    if  (NULL != httpMimeParseConfigId)
        {
        httpGenListDestroy (httpMimeParseConfigId);
        }

    httpMimeParseConfigId = NULL;    

    return (HTTP_OK);

    } /* httpMimeParseFnConfCleanup */

/***************************************************************************
*
* httpMimeParseFnConfAdd - install a MIME header processing function
*
* This routine adds the specified MIME header processing routine to the
* configuration list of the MIME parsing system RPM.
*
* NOTE: The string is not copied. Only a pointer to its location is stored.
*
* RETURNS: The entry ID of the MIME header processing function list.
*
* SEE ALSO: httpMimeParseFnConfInit()
*/

HTTP_GEN_LIST_ENTRY_ID httpMimeParseFnConfAdd
    (
    char *                 szMimeType,  /* Mime Tag                       */
    HTTP_MIME_TOKEN_PARSER pMimeParser  /* Corresponding function pointer */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID entryId;

    if ((entryId = httpGenListFuncAppend ( httpMimeParseConfigId , szMimeType , (HTTP_FUNCPTR)pMimeParser )) == NULL )
    {
    logError ( "httpMimeParseFnConfAdd: list full!");
    }
    return entryId;
    }


/***************************************************************************
*
* httpMimeParse - parse MIME line in a buffer
*
* This routine parses the MIME line in a buffer.
*
* NOTE: Do not modify <szLine>.
*
* NOMANUAL
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpMimeParse
    (
    HTTP_REQ_ID reqId,
    char *      szLine
    )
    {
    char *                 szPos;
    char *                 szTmp = NULL;
    BOOL                   fTokenProcessed = FALSE;
    HTTP_GEN_LIST_ENTRY_ID entryId = NULL;
    HTTP_MIME_TOKEN_PARSER pFn;

    if ((reqId == NULL) || (szLine == NULL))  return HTTP_ERROR;

    if ( httpMimeParseFnMax == 0 )
    return HTTP_OK;   /* if mime header processing is not configured */

    /* try to get MIME token */
    if ((szPos = httpOsalStringToken (szLine,":",&szTmp)) == NULL )
        {
        logInfo ("httpMimeParse: wrong MIME header format!");
        return (HTTP_ERROR);
        }
    
    if (szTmp == NULL)
        {
        logInfo ("httpMimeParse: wrong MIME header format!");
        return (HTTP_ERROR);
        }    	
    	
#if 0
    printf ( "MIME-trace: %s:%s\n" , szPos , szTmp);
#endif

    while ((httpGenListIterator ( httpMimeParseConfigId , &entryId )  != NULL)
           && (!fTokenProcessed))
        {
        if (httpStrNCaseCmp (httpGenListIdGet (entryId) , szPos,
                             strlen(httpGenListIdGet (entryId))) == 0)
            {
            if (NULL !=
               (pFn = (HTTP_MIME_TOKEN_PARSER)httpGenListFuncGet ( entryId )))
                {
                logInfoMsg ( "calling MIME handler for header %s..." , szPos );
                fTokenProcessed =
                    (HTTP_OK == pFn(reqId, szTmp) ? FALSE : TRUE);
                }
            }
        }
    return ((signed short) fTokenProcessed);
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpMimeParserConfigurationShow - displays HTTP MIME Parser Configuration
*
* It displays the contents of the private list that matches MIME header
* HTTP protocol level string and internally resolvable executable
* service function pointer.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpMimeParserConfigurationShow
    (
    void
    )
    {
    httpListShow (httpMimeParseConfigId, 
                  "httpMimeHeaderMimeFunctionMatchList", 
                  FALSE, /* entry id is a string */
                  TRUE,  /* entry data is a pointer */
                  "");

    return;
    } /* httpMimeParserConfigurationShow */
#endif /* HTTP_SHOW */
#endif

