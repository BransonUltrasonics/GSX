/* httpOutput.c - output generating functions */

/*
 * Copyright (c) 1996-2008,2010-2011, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
26sep14,r_w  fix static CHECKED_RETURN issue.
20may14,f_f  fix apigen issue (V7MAN-53)
05may14,r_w  fix static overrun(US35919)
08apr13,r_w  fix static overrun for defect WIND00411995
07jun11,m_z  add etag/if-modified-since support WIND203773
             CQID: WIND00203773
30mar11,r_w  fix defect WIND00257291 make API reference readable
02feb10,h_y  support for LP64 
28aug08,q_z  change the some function return declared  (WIND00033655)
28mar08,q_z  Fix defect WIND00118541
24feb07,msa  Correct the apigen comments so they match the actual routines
             and correct any apigen errors that may occur.
09jan07,ps   only generate www-authenticate header if needed in 401
             response. (defect WIND00084319)
06apr06,ps   copy input error string to safely writable buffer. SPR 119949
07jan04,adb  modified httpError errno reporting message
01dec03,adb  added httpOutputScreen () and httpOutputValidate ()
24nov03,adb  Cross Site Scripting Mitigation
07aug03,jws  CONST and other minor fix-ups
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
29jun98,ck   added requested url to error-page.
14apr98,mb   API change for httpGenListIterator() integrated.
06apr98,ck   Generic List interface for MIME header introduced.
13may97,mb   osal introduced.
20mar97,mb   httpOutputEnable() and httpOutputDisable added.
30sep96,ck   reviewed for scalability.
11sep96,mb   customizable error strings added.
24jun96,mb   initial creation.
*/

/*
DESCRIPTION
This module provides output generating functions.

INCLUDE FILES: httpLib.h, httpUtil.h

*/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "httpLib.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpCfg.h"

/* locals */

/* this table was created using httpXSSFilterTableCreate */

LOCAL BOOL httpXSSFilterTable[] =
    {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

/***************************************************************************
*
* httpStatusLine - send an HTTP status line to a client
*
* This routine prints the status line directly to the socket connection.
* To modify the defaults, you must call the
* appropriate API routines before calling this routine.
*
* RETURNS: HTTP_OK if output was successful, HTTP_ERROR if not.
*/

HTTP_STATUS httpStatusLine
    (
    HTTP_REQ_ID  reqId
    )
    {
    BOOL fTmp;

    fTmp = reqId->fProduceOutput;
    reqId->fProduceOutput = TRUE;

    /* XXX - Error processing needed here. */

    if (reqId->sProtocol >= 2)  return HTTP_ERROR;

#if 0
    if (httpPrintf (reqId,"%s %d %s\r\n",
            httpProtocolStrings [reqId->sProtocol],
            reqId->sStatus,
            httpStatus (reqId->sStatus)) == HTTP_ERROR)
#endif

    if (httpPrintf (reqId,"HTTP/1.1 %d %s\r\n",
            reqId->sStatus,
            httpStatus (reqId->sStatus)) == HTTP_ERROR)

        {
        /* hmm, what should we do here ?? */
        reqId->fProduceOutput = fTmp;
        return (HTTP_ERROR);
        }
    reqId->fProduceOutput = fTmp;
    return (HTTP_OK);
    } /* httpStatusLine */

/***************************************************************************
*
* httpOutputEnable - enable output routines
*
* This routine enables output routines such as httpPrintf(), so they can
* write data to the network.
* This routine is available for implementing custom responses, particularly
* custom response MIME header sections.
* httpHeaderGenerate() enables output production automatically, so
* httpOutputEnable() is not needed.
*
* RETURNS: N/A
*
* SEE ALSO: httpHeaderGenerate(), httpOutputDisable()
*/

void httpOutputEnable
    (
    HTTP_REQ_ID  reqId
    )
    {
    reqId->fProduceOutput = TRUE;
    } /* httpOutputEnable */

/**************************************************************************
*
* httpOutputDisable - disable output routines
*
* This routine prevents output routines such as httpPrintf() from
* writing data to the network.
* This routine is available for implementing custom responses, particularly
* custom response MIME header sections.
* httpOutputDisable() is useful for supporting the HTTP HEAD method, as each RPM
* may disable the output routines after writing the HTTP header.
* httpHeaderGenerate() leaves the output routines disabled if the current
* HTTP method is HEAD.
*
* RETURNS: N/A
*
* SEE ALSO: httpHeaderGenerate(), httpOutputEnable()
*/

void httpOutputDisable
    (
    HTTP_REQ_ID  reqId
    )
    {
    reqId->fProduceOutput = FALSE;
    } /* httpOutputDisable */

/**************************************************************************
*
* httpChunkedEncodingOn - enable chunked transfer encoding
*
* After this routine is called, subsequent calls to httpBlockPut will use
* chunked transfer encoding to send data to the network.
*
* This applies only to the current request being handled, and has no effect
* for HTTP/1.0
*
* RETURNS: N/A
*
* SEE ALSO: httpHeaderGenerate(), httpOutputEnable()
*/

void httpChunkedEncodingOn
    (
    HTTP_REQ_ID  reqId
    )
    {
    if (0 < reqId->sProtocol) /* protocol = HTTP/1.[reqId->sProtocol] */
        {
        (void) httpMimeHdrSet (reqId, HDR_OUT, "Transfer-Encoding", "chunked");
        reqId->fChunked = TRUE;
        }
    } /* httpOutputDisable */


/**************************************************************************
*
* httpChunkedEncodingOff - disable chunked transfer encoding
*
* This routine resets httpBlockPut back to clear encoding, undoing the effect
* of httpChunkedEncodingOn.
*
* RETURNS: N/A
*
* SEE ALSO: httpHeaderGenerate(), httpOutputEnable()
*/

void httpChunkedEncodingOff
    (
    HTTP_REQ_ID  reqId
    )
    {
    reqId->fChunked = FALSE;
    } /* httpChunkedEncodingOff */

/**************************************************************************
*
* httpErrStringGet - get standard system error description string
*
* This routine returns a pointer to a standard system error description,
* which must not be changed or overwritten.
* Developers may add their own RPM-specific standard messages with indices
* above HTTP_MAX_SYS_HTTP_ERRORS in the table `httpSysErrorTab'
* (in httpconfig.c).
*
* RETURNS: A pointer to a string, NULL if <usIndex> was out of range.
*/

char * httpErrStringGet
    (
    unsigned short usIndex
    )
    {
    if (usIndex < httpMaxErrors)
        {
        return ((char *)(httpSysErrorTab[usIndex]));
        }
    return (NULL);
    } /* httpErrStringGet */

/**************************************************************************
*
* httpError - write an error message to a socket
*
* This routine generates an HTML error message, including a complete header.
*
* Before calling this routine, set error type by calling httpStatusSet().
*
* RETURNS: if successful HTTP_OK else the intermediately reported status
*
* SEE ALSO: httpStatusSet ()
*/

HTTP_STATUS httpError
    (
    HTTP_REQ_ID  reqId,  /* Handle of the active request */
    char *       szMsg   /* Additional info */
    )
    {
    HTTP_STATUS rs = HTTP_ERROR;
    BOOL        fTmp;

    char copybuf [256];
    if  (NULL != szMsg)
        {
        strncpy (copybuf, szMsg, 255);
        copybuf [255] = '\0';
        }
    else
        copybuf [0] = '\0';    

    fTmp = reqId->fProduceOutput;
    reqId->fProduceOutput = TRUE;
    reqId->fPersistent    = FALSE; /* Emergency mode! Drop fancy features! */

    if  (0 == reqId->sStatus)
        /* someone called error, without setting the errorcode */
        {
        reqId->sStatus = HTTP_INTERNAL_ERROR;
        }

    if  (reqId->sStatus >= 300)
        /* only status codes >= 300 are real errors in HTTP/1.1 */
        {
        if  (!reqId->fHdrWritten)
            {
            if  (HTTP_OK !=
                 (rs = httpMimeContentTypeSet (reqId, HDR_OUT,"text/html")))
                {
                goto EXIT_LABEL;
                }

            /* produce some MIME-header */

            if  (!httpHeaderGenerate (reqId))
                {
                rs = HTTP_ERROR;

                goto EXIT_LABEL;
                }

            if  (NULL != httpErrorHeader)
                {
                if  (HTTP_ERROR == (rs = (signed short) httpPrintf (reqId, (char *) httpErrorHeader)))
                    {
                    goto EXIT_LABEL;
                    }
                }
            }
        else
            {
            if  (HTTP_ERROR == (rs = (signed short) httpPrintf (reqId, "<H2>Transfer interrupted!</H2><HR>\r\n")))
                {
                goto EXIT_LABEL;
                }
            }

        /* print HTTP error first */

        if  (HTTP_ERROR == (rs = (signed short) httpPrintf (reqId, "<H2>Server Error: %d %s</H2><HR>\r\n",
                          reqId->sStatus, httpStatus (reqId->sStatus))))
            {
            goto EXIT_LABEL;
            }

#ifdef DEBUG_ERROR
        /* if a system error occured, print it too */

        if  (errno > 0)
            {
            if  (HTTP_ERROR == (rs = (signed short) httpPrintf (reqId, "<H2>errno is 0x%x: %s </H2><HR>\r\n",
                             errno, reqId->szBuffer)))
                {
                goto EXIT_LABEL;
                }
            }
#endif /* DEBUG_ERROR */

        /* ... write user definable error message ... */

     /* if  (copybuf != NULL)
            {*/
            (void) httpOutputFilter (copybuf);

            /* we do not check filter routine's return value */

            if  (HTTP_ERROR == (rs = (signed short) httpPrintf (reqId, "<H2>%s</H2><HR>\r\n", copybuf)))
                {
                goto EXIT_LABEL;
                }
           /* }*/

        if  (reqId->szAbsPath != NULL)
            {
            (void) httpOutputFilter (reqId->szAbsPath);

            /* we do not check filter routine's return value */

            if  (HTTP_ERROR == (rs = (signed short) httpPrintf (reqId, "<H2>%s</H2><HR>\r\n", reqId->szAbsPath)))
                {
                goto EXIT_LABEL;
                }
            }

        if  (NULL != httpErrorFooter)
            {
            if  (HTTP_ERROR == (rs = (signed short) httpPrintf (reqId, (char *) httpErrorFooter)))
                {
                goto EXIT_LABEL;
                }
            }
        }
    else
        {
        logInfoMsg ("httpError was called, but status = %d", reqId->sStatus);
        }

    rs = HTTP_OK;

EXIT_LABEL:

    reqId->fProduceOutput = fTmp;

    return (rs);

    } /* httpError */

#ifdef HTTP_ETAG_ENABLE
/**************************************************************************
*
* httpNotModifiedResp - output a simple headers string 
*
* This routine sends a header to the socket. It is called with the request
* handle of an active HTTP connection when 304 status.
*
* RETURNS: TRUE or FALSE 
*/
BOOL httpNotModifiedResp
    (
    HTTP_REQ_ID  reqId     /* Handle of the active connection   */
    )
    {
    char *                 szEntity;
    const char *           szValue;
    HTTP_GEN_LIST_ENTRY_ID glMimeEntityId = NULL;

    if (reqId->fHdrWritten)  /* header has been written before! */
        {
        return (reqId->fProduceOutput);
        }

    reqId->fProduceOutput = TRUE;      /* enable output generation */

    /* Status Line, see HTTP1.1, p. 38 */

    if (reqId->sStatus == 0)     /* if nothing else was stated, presume OK */
        reqId->sStatus = HTTP_REQ_OK;

    (void) httpStatusLine (reqId);

    /* Server Info */
    (void)httpPrintf (reqId, "Server: %s\r\n",httpServerIdentification);

    /* Etag & Last-Modified */
    while (httpGenListIterator (reqId->MimeHdrListId[HDR_OUT],
                                &glMimeEntityId) != NULL)
        {
        szEntity = httpGenListIdGet   (glMimeEntityId);


        if ((NULL == szEntity)
            ||
            (
            (strcmp (szEntity, "Etag") != 0)
            && (strcmp (szEntity, "Last-Modified") != 0)
            )
            )
           continue;

        szValue  = httpGenListDataGet (glMimeEntityId);
        httpPrintf (reqId, "%s: %s\r\n", szEntity, szValue);
        }

    /* Date */
    char szTime[32] = {0};
    time_t now = time(0);
    size_t size = 0L;
    if(NULL != localtime(&now))
    {
        size = strftime (szTime, 
             	     32,
             	     "%a,%d %b %Y %H:%M:%S %Z", 
             	     localtime(&now));
    }

     if (size != 0L)
        (void)httpPrintf(reqId, "Date: %s\r\n", szTime);

    /* Cache-Control*/
/*     httpPrintf(reqId, "Cache-Control : max-age=0\r\n"); */


    (void)httpPrintf (reqId,"\r\n");     /* end of header */

    reqId->fHdrWritten = TRUE;

    if (reqId->sMethod == HTTP_M_HEAD)
        reqId->fProduceOutput = FALSE;  /* disable output generation */

    return (reqId->fProduceOutput);
    } /* httpHeaderGenerate */

#endif /* HTTP_ETAG_ENABLE */

/**************************************************************************
*
* httpHeaderGenerate - output a formatted header string to a connection socket
*
* This routine sends a header to the socket. It is called with the request
* handle of an active HTTP connection.
*
* NOTE:
* This routine  must be called before any other output that should be
* displayed by the browser, or httpOutputEnable() must be called and
* the HTTP protocol header created.
* Anything sent before this call is interpreted as part of the header
* (and will disrupt the protocol).
*
* Before calling can this routine, any information that should be
* included in the MIME header should be procured with the appropriate
* access routines.
*
* RETURNS: TRUE or FALSE depending on whether a body has been produced.
*/

BOOL httpHeaderGenerate
    (
    HTTP_REQ_ID  reqId     /* Handle of the active connection   */
    )
    {
    char *                 szEntity;
    const char *           szValue;
    HTTP_GEN_LIST_ENTRY_ID glMimeEntityId = NULL;
    HTTP_GEN_LIST_ENTRY_ID glCookieEntityId = NULL;

    if (reqId->fHdrWritten)  /* header has been written before! */
        {
        return (reqId->fProduceOutput);
        }

    reqId->fProduceOutput = TRUE;      /* enable output generation */

    /* Status Line, see HTTP1.1, p. 38 */

    if (reqId->sStatus == 0)     /* if nothing else was stated, presume OK */
        reqId->sStatus = HTTP_REQ_OK;

    (void) httpStatusLine (reqId);

    /* Generate the default header lines */

    (void)httpPrintf (reqId, "Server: %s\r\n",httpServerIdentification);

    szValue = httpMimeContentTypeGet (reqId, HDR_OUT);

    if (szValue[0] == 0)       /* presume html as default ;+b */
        (void)httpMimeContentTypeSet (reqId, HDR_OUT, "text/html");


    /* Print Persistent Connection header */

    szValue = httpMimeHdrGet (reqId, HDR_OUT, "Content-Length");
    (void)httpPrintf (reqId, "Connection: ");

    if ((reqId->fPersistent) &&
        ((szValue[0]!=0) || (reqId->fChunked)))
        {
        (void)httpPrintf (reqId, "Keep-Alive\r\nKeep-Alive:\r\nPersist:\r\n");
        }
    else
        {
        (void)httpPrintf (reqId, "close\r\n");
        reqId->fPersistent = FALSE;
        reqId->fChunked    = FALSE;
        }

    /* Any cookies set from the application */

    while (reqId->sendCookieListId != NULL &&
           httpGenListIterator (reqId->sendCookieListId,
                                &glCookieEntityId) != NULL)
    {
    szEntity = httpGenListIdGet   (glCookieEntityId);
    szValue  = httpGenListDataGet (glCookieEntityId);
    (void)httpPrintf (reqId, "Set-Cookie: %s=%s\r\n", szEntity, szValue);
    }


    /*
     * Print the MIME header fields taken from generic list
     * (including the user specific ones)
     */

    while (httpGenListIterator (reqId->MimeHdrListId[HDR_OUT],
                                &glMimeEntityId) != NULL)
        {
        szEntity = httpGenListIdGet   (glMimeEntityId);

        /*
         * skip www-authenticate header if we are not a 401
         * response. (fix for defect WIND00084319)
         */

        if (    (HTTP_UNAUTHORIZED != reqId->sStatus)
             && (NULL != szEntity) 
             && (strcmp (szEntity, "WWW-Authenticate") == 0)
           )
           {
           continue;
           }

        szValue  = httpGenListDataGet (glMimeEntityId);
        (void)httpPrintf (reqId, "%s: %s\r\n", szEntity, szValue);
        }

    (void)httpPrintf (reqId,"\r\n");     /* end of header */

    reqId->fHdrWritten = TRUE;

    if (reqId->sMethod == HTTP_M_HEAD)
        reqId->fProduceOutput = FALSE;  /* disable output generation */

    return (reqId->fProduceOutput);
    } /* httpHeaderGenerate */

/**************************************************************************
*
* httpXSSFilterTableCreate - create Cross Site Scripting filter table
*
* This routine prints a boolean table identifying special characters
* in an HTTP output message. It is assumed that the character set encoding
* follows ISO 8859-1. The set of special characters chosen is an extension
* of the one suggested in the CERT directive "Understanding Malicious
* Content Mitigation for Web Developers".
*
* RETURNS: NA
*/
void httpXSSFilterTableCreate
    (
    void
    )
    {
    int i = 0;
    int j = 0;

    printf ("BOOL httpXSSFilterTable[] = ");
    printf ("\n%4s{", "");

    /*
     * It is assumed that the character set encoding follows ISO 8859-1.
     * Therefore the total number of characters is 256.
     * We will print 16 lines with 16 table entries per line
     */

    for (i = 0; i < 16; i++)
        {
        printf ("\n%4s", "");

        for (j = 16*i; j < 16*(i+1); j++)
            {
            if  (('<'  == j) || /* 60 == 0x3c */
                 ('>'  == j) || /* 62 == 0x3e */
                 ('\'' == j) || /* 39 == 0x27 */
                 ('"'  == j) || /* 34 == 0x22 */
                 ('`'  == j) || /* 96 == 0x60 */
                 ('$'  == j) || /* 36 == 0x24 */
                 ('#'  == j) || /* 35 == 0x23 */
                 ('%'  == j) || /* 37 == 0x25 */
                 (';'  == j) || /* 59 == 0x3b */
                 ('('  == j) || /* 40 == 0x28 */
                 (')'  == j) || /* 41 == 0x29 */
                 ('&'  == j) || /* 38 == 0x26 */
                 ('+'  == j) || /* 43 == 0x2b */
                 ('-'  == j))   /* 45 == 0x2d */
                {
                printf ("%d", 1);
                }
            else
                {
                printf ("%d", 0);
                }

            /* comma should not follow the last entry */

            if  (j < 16*16-1)
                {
                printf (", ");
                }
            }
        }

    printf ("\n%4s};\n", "");

    return;

    } /* httpXSSFilterTableCreate */

/**************************************************************************
*
* httpOutputFilter - truncate output if it contains a special character
*
* The CERT Advisory CA-2000-02, titled "Malicious HTML Tags Embedded
* in Client Web Requests", warns HTTP clients of what is commonly referred
* as Cross Site Scripting. To protect Wind Manage Web Server clients we
* conservatively truncate untrusted output messages at a special character.
*
* This routine should be called to filterany message or message segment that
* is not totally created by the server before transmission. For the default
* server configuration it is sufficient to call it only when we report an
* HTTP error but it is the responsibility of the designers and implementors
* of any custom RPM to decide whether to filter output or not.
*
* It is assumed that it will always be executed successfully possibly
* overwriting a non-protected memory location storing untrusted data.
*
* RETURNS: TRUE if a special character was encountered resulting in output
* truncation and FALSE otherwise.
*
* SEE ALSO: httpOutputScreen (), httpOutputValidate ()
*
* INTERNAL:
*
* A variety of alternative implementations all of them based on the
* premise of first identifying special characters and then truncating
* or modifying or labeling untrusted output can be leveraged.
*
* It is assumed that the input is a valid null-terminating string. Else,
* unauthorized access exception unintended memory overwrite may occur.
*/
BOOL httpOutputFilter
    (
    char * pOutput
    )
    {
    while ('\0' != *pOutput)
        {
        if  (httpXSSFilterTable[(int)(*pOutput)])
            {
            *pOutput = '\0';

            return (TRUE);

           /* break;*/
            }

        pOutput++;
        }

    return (FALSE);

    } /* httpOutputFilter */

/**************************************************************************
*
* httpOutputScreen - verify that argument is free of special characters
*
* The CERT Advisory CA-2000-02, titled "Malicious HTML Tags Embedded
* in Client Web Requests", warns HTTP clients of what is commonly referred
* as Cross Site Scripting. To protect Wind Manage Web Server clients we
* offer this tool to Wind Manage Web Server RPM designers and implementors.
* Screen untrusted prospective HTTP output and acting conservatively submit
* it only if it contains no special characters.
*
* RETURNS: FALSE if a special character was encountered and TRUE otherwise.
*
* SEE ALSO: httpOutputFilter (), httpOutputValidate ()
*
* INTERNAL:
*
* It is assumed that the input is a valid null-terminating string.
*/
BOOL httpOutputScreen
    (
    char * pOutput
    )
    {
    while ('\0' != *pOutput)
        {
        if  (httpXSSFilterTable[(int)(*pOutput)])
            {
            return (FALSE);
           /* break;*/
            }

        pOutput++;
        }

    return (TRUE);

    } /* httpOutputScreen */

/**************************************************************************
*
* httpOutputValidate - verify length and absense of special characters
*
* The CERT Advisory CA-2000-02, titled "Malicious HTML Tags Embedded
* in Client Web Requests", warns HTTP clients of what is commonly referred
* as Cross Site Scripting. To protect Wind Manage Web Server clients we
* offer this tool to Wind Manage Web Server RPM designers and implementors.
*
* Screen untrusted prospective HTTP output and acting conservatively submit
* it only if it contains no special characters and it is a properly NULL
* terminated string of size smaller than <maxOutputSize> characters.
*
* RETURNS: FALSE if the input is not a properly NULL terminated string of
* appropriate size or if a special character was encountered else TRUE.
*
* SEE ALSO: httpOutputFilter (), httpOutputScreen ()
*/
BOOL httpOutputValidate
    (
    char *       pOutput,
    unsigned int maxOutputSize
    )
    {
    unsigned int i = 0;

    if  (NULL == pOutput)
        {
        return (FALSE);
        }

    for (i = 0; i < maxOutputSize; i++)
        {
        if  ('\0' == *pOutput)
            {
            return (TRUE);
            /*break;*/
            }

        if  (httpXSSFilterTable[(int)(*pOutput)])
            {
            return (FALSE);
            /*break;*/
            }

        pOutput++;
        }

    return (FALSE);

    } /* httpOutputValidate */
