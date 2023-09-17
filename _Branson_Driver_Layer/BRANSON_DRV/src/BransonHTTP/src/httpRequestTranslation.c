/* httpRequestTranslation.c - request translation and processing */

/*
 * Copyright (c) 1996-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
11jun15,r_w  fix static analysis issue V7MAN-154.
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
01nov11,r_w  HTTP header timeout support(WIND00307889)
19sep10,m_z  allow special char in parameter content in http get request
             CQID: Fix WIND00233571
26jan10,h_y  support for LP64
23sep09,y_z  add macro WEBCLI_STANDALONE_WEBSERVER in the httpParseArgs for 
             the compile of the standalone webserver
07jul08,q_z  fix GET method pass windmark argument within URL(WIND00126453)
01apr08,q_z  Fix defect WIND00113013
21aug07,ps   pass correct error codes back when mudPeek returns an error
             condition. Defect WIND00083459, fix WIND00101404
31jan04,adb  multiport functionality necessitated changes
30sep03,jws  remove assert()'s
04feb00,csr  fixing SPR #30001 (calling httpGetEnv with null parameters
             throws an exception)
18jan00,va   cleaning up debug info message
22dec99,va   added httpPostReqParse for POST handling
06dec99,csr  fixing to move call of httpLineUnescape to httpParseArgs
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
09jun99,km   introduced httpUri to handle "http" or "https" connections.
20jan99,mb   httpGetEnv() uses case insensitive keys now.
08dec98,mb   bugfix: httpReqLineParse() shifted reqId->szAbsPath.
13may97,mb   osal introduced.
11oct96,mb   documentation extended.
11oct96,mb   bugfix (URL unescaping fixed).
08oct96,wk   adapted for vxWorks 5.2 on UNIX host and m68k target.
17sep96,ck   argument parsing.
07sep96,ck   mime header processing.
03sep96,mb   initial creation.
*/

/*
DESCRIPTION
In this file the central processing of incoming HTTP 1.0/1.1 requests
is done, but especially functions related to the HTTP protocol are also
contained in here.

INCLUDE FILES: httpLib.h
               private/httpReq.h
               private/httpTcp.h
               private/httpInt.h

NOMANUAL
*/

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpReq.h"
#include "httpTcp.h"
#include "private/httpInt.h"
#include "mudLibP.h"
#include "httpCfg.h"

/* globals */
HTTP_STATUS httpReqLineParse(HTTP_REQ_ID  reqId,char * szReqLine);
#define httpMaxProtocols 2

const char * const httpProtocolStrings[httpMaxProtocols] =
    {
    "HTTP/1.0",  /* The position is crucial & magic! do not insert, only add! */
    "HTTP/1.1"
    };


/* locals */

/*
 * is is essential to the dispatcher to preserve the order
 * of HTTP_M_XXX and the method strings !
 * the value of HTTP_M_XXX must corespond to its position in this table !
 */
const METHODS httpMethods [HTTP_M_MAX] =
    {
    { HTTP_M_UNKNOWN, "UNKNOWN" },
    { HTTP_M_OPTIONS, "OPTIONS" },  /* Not yet implemented! */
    { HTTP_M_GET,     "GET"     },
    { HTTP_M_HEAD,    "HEAD"    },
    { HTTP_M_POST,    "POST"    },
    { HTTP_M_PUT,     "PUT"     },  /* Not yet implemented! */
    { HTTP_M_DELETE,  "DELETE"  },  /* Not yet implemented! */
    { HTTP_M_TRACE,   "TRACE"   }   /* Not yet implemented! */
    };


/*******************************************************************************
*
* httpGetEnv - get an HTTP environment variable by name
*
* This routine gets an HTTP environment value by name.  It can
* be used for CGI programs.
*
* RETURNS: A pointer to value string, NULL otherwise.
*/

char * httpGetEnv
    (
    HTTP_REQ_ID reqId,    /* Handle of the active request */
    char *      szName    /* Name of the variable         */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID argEntryId;

    if ((szName == NULL) || (reqId == NULL))
        return NULL;

    if ( (argEntryId = httpGenListCaseFind (reqId->argListId,szName)) != NULL)
        {
        logInfoMsg ( "httpGetEnv: variable %s found",szName );
        logInfoMsg ( "httpGetEnv:             value = >%s<",httpGenListDataGet ( argEntryId ));
        return ( httpGenListDataGet ( argEntryId ) );
        }
    logInfoMsg ( "httpGetEnv: %s NOT found",szName );
    return (NULL);
    }


/*******************************************************************************
*
* httpSetEnv - set an HTTP environment variable
*
* This routine sets an HTTP environment variable.  This routine can
* used for CGI programs.  Storage is accomplished within the request memory
* pool with httpStrdup(). All memory is therefore de-allocated and erased after
* the request is processed.
*
* RETURNS: List entry ID of the new set variable, or
* NULL if the variable list is full or httpStrdup() failed.
*/

HTTP_GEN_LIST_ENTRY_ID httpSetEnv
    (
    HTTP_REQ_ID reqId,    /* handle of the active request */
    char *      szName,   /* name of the variable         */
    char *      szValue   /* value of the variable        */
    )
    {
    char *                 szId;
    char *                 szData;
    HTTP_GEN_LIST_ENTRY_ID argEntryId;
    MEM_POOL_ID            reqMemId = httpReqMemPartIdGet (reqId);

    if (szName == NULL)  return NULL;
    if (szName[strlen(szName)-2] == '.'
    &&((szName[strlen(szName)-1] == 'x')||(szName[strlen(szName)-1] == 'y')))
        return NULL;

    if ( (argEntryId = httpGenListFind (reqId->argListId,szName)) != NULL)
        {
        logInfoMsg ( "httpSetEnv: replacing variable %s",szName );
        szId = httpGenListIdGet ( argEntryId );
        szData = httpGenListDataGet ( argEntryId );
        httpGenListRemove ( reqId->argListId , argEntryId );
        (void)memPoolFree ( reqMemId , szId );
        (void)memPoolFree ( reqMemId , szData );
        }

    szId   = httpStrdup ( reqMemId , szName );
    szData = httpStrdup ( reqMemId , szValue );

    if ( (szId!=NULL) && (szData!=NULL) )
        {
        logInfoMsg ( "httpSetEnv: setting %s",szName );
        if (( argEntryId = httpGenListDataAppend ( reqId->argListId , szId , szData )) != NULL)
            {
            logInfoMsg ( "httpSetEnv: value = >%s< set successfully",szValue );
            return argEntryId;
            }
        }
    logInfoMsg ( "httpSetEnv: failed setting %s!",szName );
    return (NULL);
    }


/*******************************************************************************
*
* httpParseArgs - separate and store args
*
* Args are mentioned in GET requests like:
* GET /mypath/somfile.html?arg1=17&switch&arg2=hey
* These args are split up in several strings with "&" as separator and
* stored in the request structure.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if arg processing succeeded, HTTP_ERROR otherwise
*/

static HTTP_STATUS httpParseArgs
    (
    HTTP_REQ_ID reqId,
    char *      szArgStart
    )
    {
    char *      szParamPtr;
    char *      szParamValue;
#ifndef WEBCLI_STANDALONE_WEBSERVER
    HTTP_BOOL   bSessionCreated = HTTP_FALSE;

    /* szArgStart needs to be unescaped here */
#endif
    logInfoMsg ( "httpParseArgs: %s",szArgStart );
    httpSetEnv ( reqId , "QUERY_STRING"    , szArgStart );
#ifndef WEBCLI_STANDALONE_WEBSERVER
    if ((!bSessionCreated) && (NULL != httpGetWmbComp()) && (NULL == reqId->pSession))
        {
        if ( OK == wmbSessionCreate ( httpGetWmbComp(), 0, 0, &( reqId->pSession )))
            {
            /* store this request in this session */
            wmbSessionStateSet ( reqId->pSession, (void *) reqId );
            /* configure session type */
            wmbSessionTypeSet ( reqId->pSession, WMB_TYPE_WEB_K );
            bSessionCreated = HTTP_TRUE;
            }
        }
#endif
    szParamPtr = NULL;
    while ((szParamValue = httpOsalStringToken (szArgStart, "&", &szParamPtr)) != NULL)
        {
        char * szToken = NULL;
        char * szName;
        char * szValue;

        szName = httpOsalStringToken (szParamValue, "=", &szToken);
        szValue = httpOsalStringToken (NULL, "=", &szToken);
        (void) httpLineUnescape(szName);
        (void) httpLineUnescape(szValue);

        if (szName != NULL && szValue != NULL && strlen (szName) > 0)
            {
            httpSetEnv (reqId, szName, szValue);
#ifndef WEBCLI_STANDALONE_WEBSERVER
            if (bSessionCreated)
                {
                (void)wmbDataStringSet(reqId->pSession,szName,NULL,0,szValue,strlen(szValue));
                }
#endif
            }

        szArgStart = NULL;
        }
#ifndef WEBCLI_STANDALONE_WEBSERVER
    if (bSessionCreated)
        {
        (void) wmbSessionDestroy ( reqId->pSession );
        reqId->pSession = NULL;
        bSessionCreated = HTTP_FALSE;
        }
#endif
    return HTTP_OK;
    }


/*******************************************************************************
*
* httpReqLineParse - parse HTML request line in a buffer
*
* This function parses the complete HTML request line inside a buffer by
* dividing the line into several parts.
* This is the reason why a request MUST NOT be longer than the buffer, or
* some chars will be cut off!
* This buffer size is defined by httpMaxUrlLen.
*
* IMPORTANT: The parts remain inside the buffer, so DO NOT OVERWRITE them! !
*
* NOMANUAL
*
* RETURNS: A pointer to a token or NULL.
*/

HTTP_STATUS httpReqLineParse
    (
    HTTP_REQ_ID  reqId,
    char *       szReqLine
    )
    {
    char            buf[10];
    char *          szPos;
    char *          szPathInfo;
    char *          szTmp = NULL;
    unsigned short  i;
    ubyte2          port;

    if (szReqLine == NULL)  return HTTP_ERROR;

    logInfo ( szReqLine );

    /* try to get method */
    if ((szPos = httpOsalStringToken (szReqLine," ",&szTmp)) == NULL )
        {
        reqId->sStatus = HTTP_BAD_REQUEST; /* there is no method ? */
        return (HTTP_ERROR);
        }

    reqId->sMethod = HTTP_M_UNKNOWN;       /* Default :) */
    for (i=1; i<HTTP_M_MAX; i++)
        if (httpStrNCaseCmp (szPos, httpMethods[i].szName,
                  strlen (httpMethods[i].szName)) == 0)
            {
            reqId->sMethod = (short) i;
            break;
            }

    if (reqId->sMethod == HTTP_M_UNKNOWN)
        {
        reqId->sStatus = HTTP_BAD_REQUEST;
        return (HTTP_ERROR);
        }

    /* try to get HTTP/1.1 URI */
    if ((szPos = httpOsalStringToken (NULL," ",&szTmp)) == NULL )
        {
        reqId->sStatus = HTTP_BAD_REQUEST;
        return (HTTP_ERROR);
        }
    if (strstr (szPos,(char*)httpUri) == szPos)
        {
        /* strip off HTTP 1.1 conform http[s]:// */
        szPos+=strlen((char*)httpUri);
        }

    /* hostname:port should be processed here if neccessary, ignored at this time */

    if ((szPathInfo = strchr (szPos,'/')) == NULL )
        {
        /* reuse the already parsed part of the buffer as string buffer for "/" */

        /* TO BE REMOVED:
           The below commented operation replaces absolute path with "/" (server root)
           Acc. to  rfc 2616, sec. 5.1.2 , this operation should be done at the
           http client while sending the request or by a proxy server.
         */
        /*strcpy (szPathInfo=szReqLine,"/");*/

        /* send a bad request because of malformed syntax requested by client */
        reqId->sStatus = HTTP_BAD_REQUEST;
        return (HTTP_ERROR);
        }
    else
        {
        if ((szPos = strchr (szPathInfo, '?')) != NULL)
            {
            *szPos++ = '\0';

            /* there seem to be some arguments inside the URL     */
            /* unescape the request line ( replace %xx and + ...) */

            /* this needs to be done in parseArgs separately for each environment variable
               httpLineUnescape (szPos);
              */
            (void) httpParseArgs (reqId, szPos);
            }
        }

    /* Try to determine the requested Protocol (HTTP/1.1 or HTTP/1.0) */

    if ((szPos = httpOsalStringToken (NULL," ",&szTmp)) == NULL )
        {
        reqId->sStatus = HTTP_BAD_REQUEST;
        return (HTTP_ERROR);
        }

    for (i=0; i < httpMaxProtocols; i++)
        if (httpStrNCaseCmp (szPos, httpProtocolStrings[i],
                     strlen (httpProtocolStrings[i])) == 0)
            {
	        reqId->sProtocol = (short) i;
            break;        
            }

    if(i >= httpMaxProtocols)
        {
         reqId->sStatus = HTTP_BAD_REQUEST;
         return (HTTP_ERROR);    	
        }

    /* set persistent connections as default for HTTP>1.1 */

    if (reqId->sProtocol >= 1)                       /* If Protocol >= HTTP/1.1 (magic position!) */
        {
        reqId->fPersistent = httpPersistentConnectionsAllowed;
        }
    else
        reqId->fPersistent = HTTP_FALSE;

    httpSetEnv ( reqId , "SERVER_SOFTWARE" , (char*)httpServerIdentification );
    httpSetEnv ( reqId , "SERVER_NAME"     , (char*)httpServerName );
    httpSetEnv ( reqId , "SERVER_PROTOCOL" , (char*)httpProtocolStrings[reqId->sProtocol] );

    if  (OK != mudConnPortGet (reqId->ioHandle, &port))
        {
        return (HTTP_ERROR);
        }

    (void)snprintf (buf, 9, "%d", port);

    httpSetEnv ( reqId , "SERVER_PORT"     , buf );

    httpSetEnv ( reqId , "REQUEST_METHOD"  , (char*)httpMethods[reqId->sMethod].szName );
    httpSetEnv ( reqId , "PATH_INFO"       , szPathInfo );

    httpStrCpy(reqId->szAbsPath,szPathInfo);

    return (HTTP_OK);
    }


/*******************************************************************************
*
* httpReqTrans - read HTTP requests from io and do an early processing
*
* At this point we assume that reqId->szAbsPath point to
* different preallocated buffers with size httpMaxUrlLen
* The request is read into a buffer line, and this is the reason why
* a request MUST NOT be longer than the buffer, or some chars will be cut off!
* This buffer size is defined by httpMaxUrlLen.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if request was read without error, HTTP_ERROR otherwise
*/

HTTP_STATUS httpReqTrans
    (
    HTTP_REQ_ID reqId
    )
    {
    char * szBuffer;
    long   lRead;

    if ((reqId == NULL) || (reqId->szAbsPath == NULL) ||
        (reqId->MimeHdrListId[HDR_IN] == NULL) ||
        (reqId->MimeHdrListId[HDR_OUT] == NULL))
        {
        return HTTP_ERROR;
        }

    /* set the clients IP address */

    if (OK != httpClientIPGet (reqId, reqId->szAbsPath))
        {
        (void) httpError (reqId,httpErrStringGet (HTTP_ERROR_TRANSFER_INTERRUPTED));
        return (HTTP_ERROR);
        }
    httpSetEnv ( reqId , "REMOTE_ADDR" , reqId->szAbsPath );

    /* Process request-line - see http 1.1 draft 07 p.30 */
    
    lRead = httpLineRead (reqId,reqId->szAbsPath,httpMaxUrlSize);
    if (-1 == lRead)
       {
        reqId->sStatus = HTTP_REQ_TIME_OUT;
        /* In the HTTP1.1, it added the code "408 request timeout" in case the http request
         * timeout, but it will not send the "408" http response as customer demand.
         */
        if ((MUD_CONN_SOCKET_ERROR != reqId->ioHandle->status)
            && (MUD_CONN_SOCKET_TIMEOUT != reqId->ioHandle->status))
            {
            (void) httpError (reqId,httpErrStringGet (HTTP_ERROR_TRANSFER_INTERRUPTED));
            }
        return (HTTP_ERROR);
        }

    /*
     * Check whether the request line was longer than our buffer,
     * which will lead to truncated arguments; do not accept such
     * requests!
     */

    if ( (unsigned long) lRead == httpMaxUrlSize )
        {
        reqId->sStatus = HTTP_ENTITY_TOO_LARGE;
        (void) httpError (reqId,httpErrStringGet (HTTP_ERROR_ENTITY_TOO_LARGE));
        return (HTTP_ERROR);
        }

    /*
     * paranoia: terminate request line string, although this should
     * be done by httpReadLine()
     */

    reqId->szAbsPath[lRead] = 0;
    if (httpReqLineParse (reqId,reqId->szAbsPath) == HTTP_ERROR )
        {
        (void) httpError (reqId,httpErrStringGet (HTTP_ERROR_WRONG_URL));
        return (HTTP_ERROR);
        }

    /*
     * Process MIME header, (fill out MimeHdrIn, assume default
     * to MimeHdrOut
     */

    szBuffer = httpBufferGet (reqId);

    do
        {
        szBuffer[0] = 0;
        /* PSERVITA */
        lRead = httpLineRead (reqId, szBuffer, httpMaxTempBufSize);
        if (-1 == lRead)
            {
            reqId->sStatus = HTTP_REQ_TIME_OUT;
            /* In the HTTP1.1, it added the code "408 request timeout" in case the http request
             * timeout, but it will not send the "408" http response as customer demand.
             */
            if ((MUD_CONN_SOCKET_ERROR != reqId->ioHandle->status)
                && (MUD_CONN_SOCKET_TIMEOUT != reqId->ioHandle->status))
                {
                (void) httpError (reqId,httpErrStringGet (HTTP_ERROR_TRANSFER_INTERRUPTED));
                }
            return (HTTP_ERROR);
            }


        (void) httpLineUnescape (szBuffer);
        if (szBuffer[0]!=0)
            httpMimeParse (reqId, szBuffer);

        /*
         * ...do something useful with the rest of the HTTP
         * header here...
         */

        } while (szBuffer[0]!=0);

    /* mark end of headers to separate RPM added environment variables */

    httpSetEnv(reqId, "END_OF_HEADERS", "");
    return (HTTP_OK);
    }
