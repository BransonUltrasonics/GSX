/* httpIo.c -  Abstract all HTTP socket functions to user. */

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
28may14,r_w  fix compile warnings(V7MAN-78)
21may14,r_w  fix httpBlockread hang issue for defect VXW6-80811           
29oct13,r_w  fix web server hangs issue for defect WIND00432564           
08apr13,r_w  fix static analysis issues for defect WIND00411995                  
01nov11,r_w  HTTP header timeout support(WIND00307889)                    
17dec10,m_z  part of APIs changed                                         
             CQID: Fix WIND00247096                                       
02feb10,h_y  support for LP64                                             
17apr09,y_z  add error process after call httpprintf to avoid             
             error is ignored                                             
28aug08,q_z  change the some function return declared  (WIND00033655)     
12aug08,q_z  fix httpBlockRead does not send up error. (WIND00129649)     
18apr08,q_z  adding timeout if no CRLF received on time. (WIND00120499)   
21aug07,ps   check mudPeek return status earlier in httpLineRead(). Defect
             WIND00083459, fix WIND00101404.                              
22feb07,ps   apigen reformatting                                          
13feb07,msa  fix comments to remove APIgen errors.                        
13jan04,adb  changed mudTcp function name prefixes to mud                 
16dec03,adb  corrected MUD over WMNET layer violation                     
18aug03,jws  remove HTTP_BOOL                                             
02may03,jws  eliminate assert()'s                                         
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.           
03apr98,ck   support for persistent connections (OSAL API changes).       
15may97,mb   osal introduced.                                             
05sep96,ck   written.
*/

/*
DESCRIPTION
This module should isolate the user from all http socket specific tasks.

INCLUDE FILES:

    httpLib.h
    private/httpTcp.h
    private/httpReq.h

*/

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "httpLib.h"
#include "httpTcp.h"
#include "httpReq.h"
#include "mudLibP.h"

#ifdef INCLUDE_OPENSSL_SUPPORT
#include "mudOverOpenSSL.h"
#endif


/* prototypes */
LOCAL HTTP_STATUS httpSendChunk (HTTP_REQ * pReq, char * pchBuffer,
                                 unsigned long ulSize);

/*******************************************************************************
*
* httpStringPut - Output a string to an HTTP connection socket.
*
* This routine is called with the request handle of an active HTTP connection
* and sends a single (zero-terminated) string to the output socket.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpStringPut
    (
    HTTP_REQ * pReq,   /* Handle of the active connection */
    char * szString    /* zero terminated string          */
    )
    {
    unsigned long ulLength;

    if (!pReq->fProduceOutput)   /* no output for head request */
        return (HTTP_OK);

    ulLength = strlen (szString);
    return (httpBlockPut (pReq, szString, ulLength));
    }


/*******************************************************************************
*
* httpBlockPut - Output a block of specified length to the HTTP connection socket.
*
* This routine is called with the request handle of an active HTTP connection
* and sends a block of length <ulSize> to the output socket.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpBlockPut
    (
    HTTP_REQ * pReq,       /* Handle of the active connection    */
    char * pchBuffer,      /* Buffer with data to send to socket */
    unsigned long ulSize   /* Number of bytes to write to socket */
    )
    {
    if (!pReq->fProduceOutput)   /* no output for head request */
        return (HTTP_OK);


    if (pReq->fChunked)
        return httpSendChunk(pReq, pchBuffer, ulSize);


    if (0 == ulSize) return (HTTP_OK);

    if (OK != mudWrite(pReq->ioHandle, (char *) pchBuffer, ulSize))
        {
        wmError(SYS_ERROR_SOCKET_GENERAL, "unable to write to socket");
        return HTTP_ERROR;
        }
    return (HTTP_OK);
    } /* httpBlockPut */

/*******************************************************************************
*
* httpSendChunk - Output a chunk to the HTTP connection socket in chunked mode.
*
* This routine is called with the request handle of an active HTTP connection
* and sends a chunk of length <ulSize> to the output socket.
*
* The chunk has the format specified in RFC2616 sec.3.6.1.
* Calling the function with pData==NULL and len==0 will send a chunk of
* size zero, which signals the end of the chunked body.
*
* NOMANUAL
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

LOCAL HTTP_STATUS httpSendChunk
    (
    HTTP_REQ * pReq,       /* Handle of the active connection    */
    char * pchBuffer,      /* Buffer with data to send to socket */
    unsigned long ulSize   /* Number of bytes to write to socket */
    )
    {
    if (HTTP_ERROR == httpPrintf (pReq, "%x\r\n", ulSize))/* chunk-size CRLF */
        {
            wmError(SYS_ERROR_SOCKET_GENERAL, "unable to write to socket");
            return HTTP_ERROR;
        }            
    if (ulSize && pchBuffer)                     
        {
        if (OK != mudWrite(pReq->ioHandle, (char *) pchBuffer, ulSize))
            {                              /* chunk-data   */
            wmError(SYS_ERROR_SOCKET_GENERAL, "unable to write to socket");
            return HTTP_ERROR;
            }
        }
    if (HTTP_ERROR == httpPrintf (pReq, "\r\n"))                      /* CRLF            */
        {
        wmError(SYS_ERROR_SOCKET_GENERAL, "unable to write to socket");
        return HTTP_ERROR;
        }
    return (HTTP_OK);
    } /* httpSendChunk */

/*******************************************************************************
*
* httpLineRead - Read a single line from an HTTP connection socket.
*
* This routine reads a line from the socket,
* which is usually needed when using the
* POST or PUT method.  When the line exceeds <ulMaxSize>, it is truncated
* and a NUL is appended. Otherwise, the line is read up
* to the next CR/LF pair, which is discarded and replaced by a NUL
* character.
*
* RETURNS: Number of bytes read.
*/

long httpLineRead
    (
    HTTP_REQ * pReq,       /* Handle of the active connection    */
    char * szBuffer,       /* Buffer to store the read string    */
    size_t bufSize  /* Maximum size of read line          */
    )
    {
    size_t      lRead     = 0;
    size_t      iActRead  = 0;
    BOOL        fCRLFRead = FALSE;
    size_t      lOffset   = 0;
    STATUS      status    = ERROR;
    sbyte4      timetoWait = 0;
    ubyte4      retryCount = 0;
    HTTP_GEN_LIST_ENTRY_ID  lstEntry;
#ifndef _WRS_CONFIG_LP64
    ubyte4      iCurrentReadLineSecs = 0;
#else
    ubyte8      iCurrentReadLineSecs = 0;
#endif
        
    lstEntry = httpGenListFind (pReq->argListId, "END_OF_HEADERS");
    
    while ((lRead < (bufSize - 1)) && !fCRLFRead)
        {
        /*
         * The function mudPeek does not timeout.
         * If needed, add call to select() [mudSocketGroupWait()] here.
         */
        OS_FDSET_T  readFds;
        MUD_CONN_ID pConn = pReq->ioHandle;

        #ifdef INCLUDE_OPENSSL_SUPPORT
        if ((NULL == pConn->pMudConnSSLState) || (0 >= SSL_pending(pConn->pMudConnSSLState->pSSL))) {
        #endif /* INCLUDE_OPENSSL_SUPPORT */

        /* timeout if no CRLF received on time */
        (void) wmnetSocketGroupClear (&readFds);

        status = wmnetSocketGroupAddTo (pConn->sock, &readFds);
        if (OK != status)
            {
            pConn->status = MUD_CONN_SOCKET_ERROR;
            wmError(SYS_ERROR_SOCKET_GENERAL, "unable to add socket");
            return HTTP_ERROR;
            }
        
        if (lstEntry == NULL)
            {
        	 iCurrentReadLineSecs = wmosTimeGet();
        	 
        	 timetoWait = (sbyte4)(httpHeaderTimeout - (iCurrentReadLineSecs - pConn->iHttpStartReadSecs));
        	 
        	 if (timetoWait > 0)
        	     {
        		 status = wmnetSocketGroupWait(&(readFds), timetoWait);
        	     }
        	 else
        	     {
        		 pConn->status = MUD_CONN_SOCKET_TIMEOUT;
        		 wmError(SYS_ERROR_SOCKET_GENERAL, "socket timeout when http read line.");
        		 return HTTP_ERROR;         		 
        	     }
            }
        else
            {
        	status = wmnetSocketGroupWait(&(readFds), httpSockTimeout);	
            }
        
        if (ERROR_GENERAL_TIMEOUT == status)
            {
            pConn->status = MUD_CONN_SOCKET_TIMEOUT;
            wmError(SYS_ERROR_SOCKET_GENERAL, "socket timeout when http read line.");
            return HTTP_ERROR;
            }

        if (OK != status)
            {
            pConn->status = MUD_CONN_SOCKET_ERROR;
            wmError(SYS_ERROR_SOCKET_GENERAL, "socket failure on timeout");
            return HTTP_ERROR;
            }

        #ifdef INCLUDE_OPENSSL_SUPPORT
        }
        #endif /* INCLUDE_OPENSSL_SUPPORT */

        status = mudPeek (pReq->ioHandle,
                          &(szBuffer[lRead]),
                          bufSize - lRead,
#ifndef _WRS_CONFIG_LP64
                          (ubyte4*)&iActRead);
#else
                          (ubyte8*)&iActRead);
#endif

        if (OK != status)
            {
            if (retryCount++ < 15)
            	continue;
            pConn->status = MUD_CONN_SOCKET_ERROR;
            wmError(SYS_ERROR_SOCKET_GENERAL, "unable to peek at socket");
            return (-1);
            }

        if (0 == iActRead) break;                    /* End of file */

        /* Check for CRLF.  The last position of the buffer is used
           to avoid dividing CRLFs.  If a CR appears at the end of
           the buffer, it will be left in the socket for the following
           read */
        for (lOffset = lRead; lOffset < lRead + iActRead; lOffset++)
            {
            if (szBuffer[lOffset] == (char)10)  /* test for LF */
                {
                if (szBuffer[lOffset - 1] == (char)13) /* test for CR */
                    {
                    fCRLFRead = TRUE;          /* CRLF always terminates */
                    break;                     /* a line */
                    }
                }
            }

        if  (fCRLFRead)
            {
            /* if so read up to it */
            (void)mudRead (pReq->ioHandle,
                     &(szBuffer[lRead]),
                     lOffset - lRead + 1,
#ifndef _WRS_CONFIG_LP64
                     (ubyte4*)&iActRead);
#else
                     (ubyte8*)&iActRead);
#endif

            /* and replace CR by NULL */
            szBuffer[lOffset - 1] = (char)0;
            }
        else if (iActRead > bufSize - lRead - 1)
            {
            /* scanned chunk doesn't fit */
            (void)mudRead (pReq->ioHandle,
                     &(szBuffer[lRead]),
                     bufSize-lRead-1,
#ifndef _WRS_CONFIG_LP64
                     (ubyte4*)&iActRead);
#else
                     (ubyte8*)&iActRead);
#endif
            }
        else
            {
            /* scanned chunk fits */
            (void)mudRead (pReq->ioHandle,
                     &(szBuffer[lRead]),
                     iActRead,
#ifndef _WRS_CONFIG_LP64
                     (ubyte4*)&iActRead);
#else
                     (ubyte8*)&iActRead);
#endif
            }

        lRead+=iActRead;
        }

    if  (!fCRLFRead)
        {
        /* terminate the string */
        szBuffer[lRead++] = (char)0;
        }

    return((long)lRead);
    } /* httpLineRead */

/*******************************************************************************
*
* httpBlockRead - Read a block of data from the HTTP connection socket.
*
* This routine reads a block of data from the I/O socket (mainly used for
* POST method).
*
* RETURNS: The number of bytes read.
*/
long httpBlockRead
    (
    HTTP_REQ *    pReq,          /* Handle of the active connection    */
    char *        pchBuffer,     /* Buffer to store the read block     */
    size_t ulMaxSize      /* Maximum size of block              */
    )
    {
    size_t lRead;
    STATUS  status;
    STATUS  ret;

    OS_FDSET_T  readFds;
    MUD_CONN_ID pConn = pReq->ioHandle;

    #ifdef INCLUDE_OPENSSL_SUPPORT
    if ((NULL == pConn->pMudConnSSLState) || (0 >= SSL_pending(pConn->pMudConnSSLState->pSSL))) {
    #endif /* INCLUDE_OPENSSL_SUPPORT */

    /* timeout if no data received on time */
    (void) wmnetSocketGroupClear (&readFds);

    status = wmnetSocketGroupAddTo (pConn->sock, &readFds);
    if (OK != status)
        {
        pConn->status = MUD_CONN_SOCKET_ERROR;
        wmError(SYS_ERROR_SOCKET_GENERAL, "unable to add socket");
        return HTTP_ERROR;
        }

    status = wmnetSocketGroupWait(&(readFds), httpSockTimeout);

    if (ERROR_GENERAL_TIMEOUT == status)
        {
        pConn->status = MUD_CONN_SOCKET_TIMEOUT;
        wmError(SYS_ERROR_SOCKET_GENERAL, "socket timeout");
        return HTTP_ERROR;
        }

    if (OK != status)
        {
        pConn->status = MUD_CONN_SOCKET_ERROR;
        wmError(SYS_ERROR_SOCKET_GENERAL, "socket failure on timeout");
        return HTTP_ERROR;
        }

    #ifdef INCLUDE_OPENSSL_SUPPORT
    }
    #endif /* INCLUDE_OPENSSL_SUPPORT */

#ifndef _WRS_CONFIG_LP64
    ret = mudRead(pReq->ioHandle, pchBuffer, ulMaxSize, (ubyte4*)&lRead);
#else
    ret = mudRead(pReq->ioHandle, pchBuffer, ulMaxSize, (ubyte8*)&lRead);
#endif
    return (( ret == ERROR ) ? (long)ret : (long)lRead);

    } /* httpBlockRead */


/*******************************************************************************
*
* httpPrintf - Output a formatted string to the HTTP connection socket.
*
* This routine is called with the request handle of an active HTTP connection
* and sends a formatted string to the connection socket.
* It is identical to printf() in terms of the
* usage of parameters and format strings.
*
* RETURNS: The number of characters, or HTTP_ERROR.
*/

sbyte4 httpPrintf
    (
    HTTP_REQ * pReq,     /* Handle of the active connection   */
    char *     szFormat, /* Format string, printf like syntax */
    ...                  /* Arguments to format               */
    )
    {
    va_list vaList;
    STATUS  status;
    sbyte4  pBytesPrinted = 0;

    if (!pReq->fProduceOutput)   /* no output for head request */
        {
        return pBytesPrinted;
        }

    va_start (vaList, szFormat);
    status = mudSocketVprintf(pReq->ioHandle, szFormat, vaList, &pBytesPrinted);
    va_end (vaList);

    if (OK != status)
        {
        pBytesPrinted = 0;
        return HTTP_ERROR;
        }

    return (sbyte4)pBytesPrinted;
    }


/*******************************************************************************
*
* httpClientIPGet - Get the IP address of the connected client.
*
* This routine gets the IP address of the client connected for the current
* request and stores it in the specified string.  The required buffer size
* is at least 40 bytes, including the terminating NUL.
*
* This routine handles both IPV4 and IPV6 addresses.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpClientIPGet
    (
    HTTP_REQ * pReq,         /* Handle of the request to examine */
    char *     szIPString    /* Buffer for the text IP addr    */
    )
    {
    MUD_IP_ADDR ipAddr;

    if (pReq == NULL)
        return (HTTP_STATUS)(wmError(ERROR_GENERAL,"Bad HTTP_REQ"));

    if (OK != mudClientIpAddrGet(pReq->ioHandle, &ipAddr))
        return (HTTP_STATUS)(wmError(ERROR_GENERAL,"unable to get client socket address"));

    switch(ipAddr.len)
        {
        case 4:     /* IPV4 */

            (void)snprintf(szIPString, httpMaxUrlSize -1, "%u.%u.%u.%u",
                    ipAddr.ipAddrBytes[0], ipAddr.ipAddrBytes[1],
                    ipAddr.ipAddrBytes[2], ipAddr.ipAddrBytes[3]);
            break;
        case 16:    /* IPV6 */
            (void)snprintf(szIPString, httpMaxUrlSize-1, "%x:%x:%x:%x:%x:%x:%x:%x",
                    (ipAddr.ipAddrBytes[0] << 8) + ipAddr.ipAddrBytes[1],
                    (ipAddr.ipAddrBytes[2] << 8) + ipAddr.ipAddrBytes[3],
                    (ipAddr.ipAddrBytes[4] << 8) + ipAddr.ipAddrBytes[5],
                    (ipAddr.ipAddrBytes[6] << 8) + ipAddr.ipAddrBytes[7],
                    (ipAddr.ipAddrBytes[8] << 8) + ipAddr.ipAddrBytes[9],
                    (ipAddr.ipAddrBytes[10] << 8) + ipAddr.ipAddrBytes[11],
                    (ipAddr.ipAddrBytes[12] << 8) + ipAddr.ipAddrBytes[13],
                    (ipAddr.ipAddrBytes[14] << 8) + ipAddr.ipAddrBytes[15]);

            break;
        default:
            return (HTTP_STATUS)(wmError(ERROR_GENERAL,"unknown IP address type"));
            /*break;*/
        }

    return OK;
    } /* httpClientIPGet */


/*******************************************************************************
*
* httpStdIoStoreDefault - Store the current STDIO as the default value.
*
* This routine stores the current STDIO as the default value in the request
* structure for subsequent use with httpStdIoSetToDefault().
*
* NOTE: This routine is necessary only for CGI emulation.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpStdIoStoreDefault
    (
    HTTP_REQ * pReq         /* Handle of the current request */
    )
    {
#if 0
    return (httpOsalStdIoStoreDefault (pReq->ioHandleId));
#endif
    return (HTTP_STATUS)(wmError(ERROR_GENERAL,"function deprecated"));
    }


/*******************************************************************************
*
* httpStdIoSetToDefault - Restore the current STDIO to the default value.
*
* This routine restores the current STDIO to the default value in the
* request structure defined by httpStdIoStoreDefault().
*
* NOTE: This routine is necessary only for CGI emulation.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpStdIoSetToDefault
    (
    HTTP_REQ * pReq         /* Handle of the current request */
    )
    {
#if 0
    return (httpOsalStdIoSetToDefault (pReq->ioHandleId));
#endif
    return (HTTP_STATUS)(wmError(ERROR_GENERAL,"function deprecated"));
    }


/*******************************************************************************
*
* httpStdIoSetToSocket - Redirect current STDIO to the socket.
*
* This routine redirects the current STDIO to the socket.
*
* NOTE: routine is necessary only for CGI emulation.
*       This function is deprecated.
*
* RETURNS: HTTP_OK or HTTP_ERROR.
*/

HTTP_STATUS httpStdIoSetToSocket
    (
    HTTP_REQ * pReq         /* Handle of the current request */
    )
    {
#if 0
    return (httpOsalStdIoSetToSocket (pReq->ioHandleId));
#endif
    return (HTTP_STATUS)(wmError(ERROR_GENERAL,"function deprecated"));
    }


/*******************************************************************************
*
* httpTimeoutSet - Set the timeout for the current connection.
*
* This routine sets the timeout for the current connection,
* in seconds.
*
* RETURNS: HTTP_OK.
*/

HTTP_STATUS httpTimeoutSet
    (
    HTTP_REQ *  pReq,   /* Request Handle */
    long  lTimeout      /* Timeout in seconds */
    )
    {
#if 0
    return (httpOsalSocketTimeoutSet (pReq->ioHandleId, lTimeout));
#endif
    return (HTTP_STATUS)(wmError(ERROR_GENERAL,"function deprecated"));
    }
