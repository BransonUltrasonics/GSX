/* httpRpmWmbParser.c - Parse HTML files and get WindMark values from Wmb */

/*
 * Copyright (c) 2001-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
09oct16,r_w  webserver does not replace spaces (V7MAN-216)
13jun16,r_w  replace space with "&nbsp;" (V7MAN-185)
06jun16,r_w  fix http memory issue in httpRpmWmbParse() (V7MAN-189)
17feb16,r_w  fix compiler and static analysis warnings.
26sep14,r_w  fix static CHECKED_RETURN issue.
27aug14,r_w  close file decription for defect VXW6-83291
28may14,r_w  fix compile warnings(V7MAN-78)
05may14,r_w  fix static analysis issues(US35919)
21feb13,r_w  fix static analysis issue(defect WIND00404092)
08aug12,r_w  eliminate warnings for "'markFind' defined but not used"
                 Req WIND00347266
17feb12,shi  add header file httpCfg.h to this file to fix 
                 kJavaScriptLogonPage is undefined issue.
                 CQID: WIND00332818
17may12,h_l  replace NO_STRING with NULL_STRING (WIND00032212)
16may12,r_w  eliminate buiding warnings
                 defect WIND00349929
17feb12,shi  add header file httpCfg.h to this file to fix 
                 kJavaScriptLogonPage is undefined issue.
                 CQID: WIND00332818
07jun11,m_z  add etag/if-modified-since process WIN00203773
                 CQID: WIND00203773
30mar11,r_w  fix defect WIND00257291 make API reference readable
17dec10,m_z  part of APIs changed
                 CQID: Fix WIND00247096
07sep10,m_z  set apart normal windmark analyse from SMTP type, in which 
             sock is used instead of req structure.
             CQID: Fix WIND00231909
30jul10,m_z  fix page display break when fake wind mark in page
             CQID: Fix WIND00225833
02feb10,h_y  support for LP64 
17apr09,y_z  add error process when http's some print routine return error
30jun08,q_z  fix using REPEAT macro in pages sent via POST (WIND00039488)
23May08,q_z  fix create table error for bad instances (WIND00040581)
14jan08,ps   fix macro cleanup in markParserDrvr()
05sep07,ten  Add include to allow building for linux target
22feb07,ps   apigen reformatting
13feb07,msa  Fix comments to remove APIgen errors.
26jul06,ps   update for IPV6
16feb06,ps   changes for webcli on linux
25apr05,dkg  Update JS Auth #define ordering
13sep04,dkg  Modify httpRpmWmbParse() and markParserDrvr() to support
             JavaScript Authentication feature
15sep04,dkg  corrected httpWindMarkParse() to fix SPR 101452
27may04,dlr  Destroy created session if early exit in markParserDrvr()
30apr04,dkg  Modify markParserDrvr() to fix SPR 96036
27apr04,adb  replaced all buffer size constants with kMagicMarkupBufferSize
15mar04,adb  trivialized httpWmbParserConfigId by using instead File System
12mar04,adb  using httpFsRootGet
08mar04,adb  added httpWmbParserConfigurationShow
03mar04,dkg  Check status before using the wmErrNo() in markParserDrvr()
25feb04,dkg  Modify markParserDrvr() for evaluating the IF macro
             statement correctly to fix SPR 91625
25feb04,jws  remove per project dependencies (again!)
12feb04,vmk  SPR 91236; Error handling for invalid windmarks.
06feb04,dkg  Merge fileSendChunkProcess() from httpFileProcess.c and
             markParser() from httpRpmWmbParser.c into new function
             httpWindMarkParse() with reference to SPR 91304
03feb04,gsr  support for inline instantiation SPR#91008
29jan04,dkg  Modify Zero value handling in markParser() to fix
             SPR 91623
15jan04,jws  remove per project dependencies
14jan04,gsr  cleanup routine for SPR 93009
14oct03,jws  Modify ERROR_GENERAL_NO_DATA handling in markParser()
             to fix SPR 90576.
16jun03,adb  initialize tempBuf in markParser, SPR - 82954
05jun03,jws  Remove debug printf().
04jun03,jws  Deal with ERROR_GENERAL_NO_DATA errors in markParser().
             Needed because of wmConvertLib changes, SPR88612, etc.
17mar03,tf   Initialize tempBuf in markParser() before using it.
06mar03,adb  &nbsp will substitute NULL string only within tables
06nov01,jc   initial creation.
*/

/*
DESCRIPTION

INCLUDE FILES:

*/


#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "commonCfg.h"
#include "httpCfg.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER

#ifdef WEBCLI_ON_LINUX
# include <errno.h>
#endif

#undef DEBUG_ERROR

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpShow.h"

#ifdef __JAVASCRIPT_DIGEST_ENABLED__
#include "httpAuth.h"
#endif

#ifdef HTTP_ETAG_ENABLE
#include "httpMimeEtag.h"
#endif


/* externals - will move to a header file */

IMPORT char wmVarMagicMarkupStartChar0;
IMPORT char wmVarMagicMarkupStartChar1;
IMPORT char wmVarMagicMarkupEndChar0;
IMPORT char wmVarMagicMarkupEndChar1;


IMPORT void httpExpandInstance ( WMB_SESSION_T *   pSession, char*, char *);
#ifdef HTTP_ETAG_ENABLE
LOCAL STATUS markFind ( HTTP_REQ_ID   reqId, generic_fd_t  * pF, unsigned long fileSize);
#endif

/* defines */

enum WM_STATE {WM_INVALID, WM_BEGIN, WM_START, WM_MID, WM_END, WM_FINAL};


/* local variables */

LOCAL HTTP_GEN_LIST_ID httpWmbParserConfigId; /* list of URL-path mappings */


/* local functions */


/***************************************************************************
*
* httpWmbParserConfInit - initializes the RCB parser
*
* This function creates the list of mappings between URLs and paths.
*
* RETURNS: List id of the list or
*          NULL if errors ocurred
*/

HTTP_GEN_LIST_ID httpWmbParserConfInit
    (
    unsigned short maxPaths   /* max number of entries (URL-path mappings) */
    )
    {

    return (httpWmbParserConfigId =
                      httpGenListCreate (httpGlobalMemPartIdGet(),
                                         maxPaths , HTTP_TRUE ) );
    }

/***************************************************************************
*
* httpWmbParserConfCleanup - Cleanup routine for httpWmbParserConfInit()
*
* RETURNS: HTTP_OK
*/
HTTP_STATUS httpWmbParserConfCleanup
    (
    void
    )
    {
    if  (NULL != httpWmbParserConfigId)
        {
        httpGenListDestroy (httpWmbParserConfigId);

        httpWmbParserConfigId = NULL;
        }

    return (HTTP_OK);

    } /* httpWmbParserConfCleanup */

/***************************************************************************
*
* httpWmbParserConfAdd - add an entry to the WMB parser RPM configuration list
*
* This routine adds the specified entry to the WMB parser RPM list.
*
* NOTE: The string is not copied.  Only a pointer to its location is
* stored.
*
* RETURNS: HTTP_GEN_LIST_ENTRY_ID of the new configuration entry, or
* NULL if the new entry could not be added.
*
* SEE ALSO: httpWmbParserConfInit
*/

HTTP_GEN_LIST_ENTRY_ID httpWmbParserConfAdd
    (
    char * pUrl,         /* Incoming URL request          */
    char * pFsPath       /* Corresponding filesystem path */
    )
    {
    return (httpFsConfAdd (pUrl, pFsPath));

    /* the current implementation trivializes httpWmbParserConfigId */
    }

/***************************************************************************
*
* httpRpmWmbParse - read a file, replace WindMarks and send it to browser
*
* This is the Wmb parser RPM routine.
* It reads a file, finds all the WindMarks in it, gets their current value
* from the Wind Manage Backplane, and writes the output to the network.
* To minimize memory usage, the file is sent using chunked transfer encoding
* The file can be read from anywhere in the file system (hard disk, NFS,
* memory file system, and so on).
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*/

short httpRpmWmbParse
    (
    HTTP_REQ_ID reqId   /* Handle of the active request, needed for all */
                        /* http - API function calls                    */
    )
    {
    generic_fd_t           f;
    sbyte *                szBuffer;
    sbyte *                cpTransBuffer;
    unsigned long          ulMaxTransBufSize;
    MEM_POOL_ID            mpPool;
    ssize_t                lFileSize=0;

#ifdef __JAVASCRIPT_DIGEST_ENABLED__

    sbyte *                szBufferAuth   = NULL;
    WM_ACCESS_T            *userAccessLevel    = NULL;
    Access                 readAccess;
    Access                 writeAccess;
    Access                 userReadAccess;
    MUD_SOCKET             sock;

#endif

    /*
     * If this is not text/html, leave it to other RPM ( File System RPM ?).
     * For the next call to work, the MIME guessing RPM needs to be enabled.
     */

    if (strcmp("text/html", httpMimeHdrGet (reqId, HDR_OUT, "Content-Type")))
        return RPM_OK;

    szBuffer = httpBufferGet (reqId);

    if  (HTTP_OK != httpFsRootGet (reqId,
                                   szBuffer,
                                   httpMaxTempBufSize))

        {
        httpStatusSet( reqId, HTTP_NOT_FOUND );
        (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_FILE_NOT_FOUND));
        return (RPM_ERROR);
        }

#ifdef __JAVASCRIPT_DIGEST_ENABLED__

    szBufferAuth = ( sbyte * )  wmMalloc ( STRLEN ( szBuffer ) +
                                           STRLEN ( kJavaScriptLogonPage ) + 1);
    if (NULL == szBufferAuth)
	    return (RPM_ERROR);	
											   
    MEMSET ( szBufferAuth, 0, ( STRLEN ( szBuffer ) +
                                STRLEN ( kJavaScriptLogonPage ) + 1 ));
    STRCPY ( szBufferAuth, szBuffer );

#endif

    strncat (szBuffer,
             httpRpmDataGet (reqId),
             httpMaxTempBufSize - strlen (szBuffer) - 1);

    if ( OK != wmbSessionCreate ( httpGetWmbComp(), 0, 0, &( reqId->pSession )))
        {
        wmLogPrintf ( "unable to create WMB session\n" );
		#ifdef __JAVASCRIPT_DIGEST_ENABLED__
		if ( NULL != szBufferAuth )
            {
            wmFree ( szBufferAuth );
            szBufferAuth = NULL;
            }
		#endif
        return (short)wmError ( ERROR_GENERAL, "unable to create WMB session" );
        }

    /* store this request in this session */

    wmbSessionStateSet ( reqId->pSession, (void *) reqId );

    /* configure session type */

    wmbSessionTypeSet ( reqId->pSession, WMB_TYPE_WEB_K );

#ifdef __JAVASCRIPT_DIGEST_ENABLED__


    if ( OK != mudConnSocketGet ( reqId->ioHandle, &sock ))
        {
        (void) wmbSessionDestroy ( reqId->pSession );
        if ( NULL != szBufferAuth )
            {
            wmFree ( szBufferAuth );
            szBufferAuth = NULL;
            }
  return (short)wmError (ERROR_GENERAL, "unable to get sock ID");
        }

    if ( OK != wmnetClientIpAddrGet ( sock, &(reqId->pSession->wmnet_IpAddr )))
        {
        (void) wmbSessionDestroy ( reqId->pSession );
        if ( NULL != szBufferAuth )
            {
            wmFree ( szBufferAuth );
            szBufferAuth = NULL;
            }
        return (short)wmError (ERROR_GENERAL, "unable to get address from socket");
        }

    userAccessLevel = wmAUTHCheckAuthentication ( reqId->pSession );

    if ( NULL != userAccessLevel )
        {
        reqId->pSession->access = *userAccessLevel;
        userReadAccess = wmAccessReadGet ( *userAccessLevel );
        }
    else
        userReadAccess = 0;

    (void) httpFileAccessGet ( szBuffer, &readAccess, &writeAccess);

    if (! wmAccessReadOkay ( userReadAccess, readAccess ))
        {
        if ( NULL != httpFileRegHandle ( szBuffer ))
            {
            sbyte*  pCookie = wmAUTHGetClientCookie ( reqId->pSession->clientIndex );
            (void) httpMimeOutCookieSet(reqId,"COOKIE", pCookie, "/", NULL, NULL, 9999, FALSE );
            STRCAT ( szBufferAuth, kJavaScriptLogonPage );
      STRCPY ( szBuffer, szBufferAuth );
            }
         }

    if ( NULL != szBufferAuth )
        {
        wmFree ( szBufferAuth );
        szBufferAuth = NULL;
        }
#endif  /*__JAVASCRIPT_DIGEST_ENABLED__*/

#ifdef DEBUG_INFO
    fprintf(stderr, "httpRpmWmbParse: request is %s\n", szBuffer);
    fflush(stderr);
#endif

    if (OK != httpFileOpen(szBuffer, "rb", &f))
        {
        httpStatusSet( reqId, HTTP_NOT_FOUND );
        (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_FILE_NOT_FOUND));

#ifdef DEBUG_INFO
        httpPrintf (reqId, "<H2>Could not open the file <TT>'%s'</TT></H2>\n",
                    szBuffer);
#endif
        (void) wmbSessionDestroy(reqId->pSession);
        return RPM_ERROR;
        }

    if (0 >= (lFileSize = httpFileSize(&f)))
        {
        (void) httpFileClose(&f);
        (void) wmbSessionDestroy(reqId->pSession);
        return RPM_ERROR;
        }

    /*
     * After next call httpBlockPut will send chunks, not plain blocks.
     * If req is HTTP/1.0, this has no effect.
     */

    httpChunkedEncodingOn (reqId);

    mpPool = httpReqMemPartIdGet (reqId);

    ulMaxTransBufSize = (unsigned long)(lFileSize +1);
    cpTransBuffer = httpConservativeAlloc (mpPool, (size_t *)&ulMaxTransBufSize);

    if (cpTransBuffer == NULL)
        {
        logError ("httpRpmWmbParse: out of request-pool memory");
        httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
        (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_ENTITY_TOO_LARGE));
        (void) httpFileClose(&f);
        (void) wmbSessionDestroy ( reqId->pSession );
        return (RPM_ERROR);
        }
    else
        {
#ifdef DEBUG_INFO
        fprintf (stderr, "httpRpmWmbParse: using transfer buffer size %d\n",
                 ulMaxTransBufSize);
        fflush(stderr);
#endif
        }

    logInfoMsg ("httpRpmFs: opening file %s...\n",szBuffer);

#ifdef HTTP_ETAG_ENABLE
    sbyte *                szTime;
    unsigned long          rc = 0;
    /* Get http file modification time */
    szTime = memPoolAlloc (mpPool, 33);
    if (szTime != NULL)
        {
        bzero ((char*)szTime, 33);
        rc = (unsigned long) httpFileTimeGet (&f, szBuffer, szTime);
        if ((OK == rc) && (0 != *szTime))
            {
            (void) httpMimeHdrSet (reqId, HDR_OUT, "Last-Modified", szTime);
            logInfoMsg ("Setting Last-Modified: %s\n", szTime);
            }
       (void) memPoolFree (mpPool, szTime);
        }
    else
        {
        logInfo ("httpRpmFs: out of request-pool memory");
        }


    char *szOutEtag = NULL; 
    const char *szInEtag  = NULL;
    const char *szInIMS   = NULL;
    struct tm biInIMS, *biOutIMS;
    HTTP_ETAG_BINARY       biOutEtag; 
    HTTP_ETAG_BINARY       biInEtag; 
    int biInDay = 0;
    int biInSec = 0;
    int biOutDay = 0;
    int biOutSec = 0;
    memset(&biInIMS, 0, sizeof(biInIMS));

    if (OK != markFind(reqId, &f, (unsigned long)lFileSize)) 
        {
        szOutEtag = memPoolAlloc (mpPool, HTTP_ETAG_MAX_LEN);
        if (szOutEtag != NULL)
            {
            biOutEtag.size = (unsigned long)lFileSize;
            if ((OK ==  httpEtagFileTestAndSet(szBuffer, &biOutEtag.file))
                && (OK == httpFileBiTimeGet (&f, szBuffer, &biOutEtag.time))
                )
                {
                (void) httpEtagValueSet(&biOutEtag, szOutEtag);
                (void) httpMimeHdrSet (reqId, HDR_OUT, "Etag", szOutEtag);
    
    
                szInEtag = httpMimeHdrGet (reqId, HDR_IN, "If-None-Match");
                if (HTTP_ERROR != httpEtagValueGet(szInEtag, &biInEtag)
                    && (biInEtag.file == biOutEtag.file)
                    && (biInEtag.size == biOutEtag.size)
                    && (biInEtag.time == biOutEtag.time)
                    )
                    {
                    httpChunkedEncodingOff (reqId);
                    httpStatusSet (reqId, HTTP_NOT_MODIFIED);
                    httpNotModifiedResp (reqId);

                    (void) memPoolFree (mpPool, szOutEtag);
                    (void) httpFileClose (&f);
                    (void) memPoolFree (mpPool, cpTransBuffer);
                    wmbSessionDestroy ( reqId->pSession );
 
                    return RPM_DONE;
                    }
                
                szInIMS = httpMimeHdrGet (reqId, HDR_IN, "If-Modified-Since");
                if (szInIMS != NULL 
                    && *szInIMS != '\0'
                    && HTTP_ERROR !=  httpIfModifiedSinceValueGet(szInIMS, &biInIMS))
                    {
                    biInDay = (biInIMS.tm_year << 9) | (biInIMS.tm_mon << 5) | biInIMS.tm_mday;
                    biInSec = (biInIMS.tm_hour << 12) | (biInIMS.tm_min << 6) | biInIMS.tm_sec;
                    biOutIMS = gmtime(&biOutEtag.time); 
                    if(NULL != biOutIMS)
                    {
                      biOutDay =((biOutIMS->tm_year+1900) << 9) | (biOutIMS->tm_mon << 5) | (biOutIMS->tm_mday);
                      biOutSec = (biOutIMS->tm_hour << 12) | (biOutIMS->tm_min << 6) | biOutIMS->tm_sec;
                    }
                    if ( (biInDay > biOutDay) 
                        ||((biInDay == biOutDay) && (biInSec >= biOutSec+60))
                        )
                        {
                        httpChunkedEncodingOff (reqId);
                        httpStatusSet (reqId, HTTP_NOT_MODIFIED);
                        (void) httpNotModifiedResp (reqId);

                        (void) memPoolFree (mpPool, szOutEtag);
                        (void) httpFileClose (&f);
                        (void) memPoolFree (mpPool, cpTransBuffer);
                        (void) wmbSessionDestroy ( reqId->pSession );
 
                        return RPM_DONE;
                        }
                    }
                 }
            else
                (void) httpEtagFileRemove(szBuffer);
    
            (void) memPoolFree (mpPool, szOutEtag);
            }
        else
            {
            logInfo ("httpRpmFs: out of request-pool memory");
            }
        }
#endif


    /* if we are here, we have a non-empty open file waiting to be read */

    httpStatusSet (reqId, HTTP_OK);

    /*
     * If a Content-Type was registered for the file, use it.
     * Else, leave the one guessed by the Mime RPM.
     */

    if (httpFileContentTypeGet(&f))
        (void) httpMimeHdrSet(reqId, HDR_OUT, "Content-Type",
                              httpFileContentTypeGet(&f));

    /*
     * !! if HTTP/1.0, here we need to guess Content-Length header.
     * Chunked only for HTTP/1.1 !!
     */

    /* process the macros and windMarks in the open file */

    if (OK != markParserDrvr (NULL, NULL, reqId, &f, (unsigned long)lFileSize, cpTransBuffer,
                              (ulMaxTransBufSize-1)))
        {
        httpStatusSet (reqId, HTTP_INTERNAL_ERROR);
        (void) httpHeaderGenerate (reqId);
        (void)httpBlockPut (reqId, NULL, 0); /* terminate chunked encoding */
        (void) httpFileClose(&f);
        (void) memPoolFree (mpPool, cpTransBuffer);
        (void) wmbSessionDestroy ( reqId->pSession );
        return RPM_ERROR;
        }

    (void) httpFileClose (&f);
    (void) memPoolFree (mpPool, cpTransBuffer);
    (void) wmbSessionDestroy ( reqId->pSession );
    return RPM_DONE;
    }

/**************************************************************************
*
* markParserDrvr - evaluates the windMarks and macros embedded in test
*
* NOMANUAL
*
* RETURNS: OK or an error
*/

STATUS markParserDrvr
    (
    STATUS (*writeFctPtr)(void *, sbyte *, ubyte4), /* output routine, expected to be httpBlockPut or wmnetTcpWrite */
    void *        writeArg,                         /* socket fd or reqId structure */
    HTTP_REQ_ID   reqId,
    generic_fd_t  * pF,
    unsigned long fileSize,
    sbyte *       workBuffer,
    unsigned long bufferSize
    )
    {
    STATUS         status           = OK;
    unsigned long  charsParsed      = 0;
    ubyte4         filePos          = 0;
    ubyte4         bytesRead        = 0;
    ubyte4         bytesToRead      = 0;
    sbyte *        pExitMark        = NULL;
    sbyte *        newWorkBuffer    = NULL;
    sbyte *        endTablePos      = NULL;
    unsigned long  skip             = 0;
    unsigned long  count            = 0;
    MACRO_STACK_ID stack            = NULL;
    MACRO_STACK_ID FirstifStack     = NULL;
    MACRO_STACK_ID FirstrepeatStack = NULL;
    HTTP_BOOL      ClearCharBool    = HTTP_FALSE;

    (void) httpFileSeek(pF, 0);
    (void) httpHeaderGenerate (reqId);

    if (OK != httpMacroInit(reqId->pSession, kMacroRepeatNestMax))
        {
        (void)wmError (ERROR_GENERAL, "unable to initialize WMB macro module");
        return ERROR;
        }
    if ((reqId->repeatStack != NULL) && (reqId->ifStack != NULL) && (reqId->pSession->fileIncludeLevel == 0))
        {
        FirstifStack     = reqId->ifStack;
        FirstrepeatStack = reqId->repeatStack;
        }

    do
        {
        /* don't read more than what the buffer can hold */
        bytesToRead = (ubyte4)((bufferSize > (fileSize - filePos)) ?
                          (fileSize-filePos ) : (bufferSize));

        if (0 == bytesToRead) break;
        if (OK != httpFileRead(pF, workBuffer,
                               (ubyte4)bytesToRead, &bytesRead))
            {
            (void)wmError (ERROR_GENERAL, "failed reading file");
            break;
            }

        if (ClearCharBool == HTTP_TRUE)
            {
            endTablePos = workBuffer;
            for(count=0; count<bytesRead; count++)
                {
                if (0 != memcmp(endTablePos,"endTable",8))
                    {
                    *endTablePos=' ';
                    endTablePos++;
                    continue;
                    }
                wmbSessionTableSet(reqId->pSession,NULL);
                ClearCharBool=HTTP_FALSE;
                break;
                }
            if (count<bytesRead)
                {
                for(; count<bytesRead; count++)
                    {
                    if (0 != memcmp(endTablePos,"-->",3))
                        {
                        *endTablePos=' ';
                        endTablePos++;
                        continue;
                        }
                    break;
                    }
                }

            if (count == bytesRead)
                {
                filePos += bytesRead;
                httpFileSeek(pF, filePos);
                continue;
                }
            }

        stack = reqId->ifStack ;
		if (NULL == stack)
		    break;
        if (stack->count != 0 && reqId->fProduceOutput != TRUE)
            {
            workBuffer[bytesRead] = '\0';
            if ((newWorkBuffer = strstr ( workBuffer, "$%IF" )) == NULL)
                {
                /* read next buffer if don't find '$%IF' to stop skiping file
                   contents */
                filePos += bytesRead;
                continue;
                }
            else
                {
                skip = (unsigned long) (newWorkBuffer  - workBuffer);
                filePos = (ubyte4)(filePos + skip);
                bytesRead = (ubyte4)(bytesRead - skip);
                }
            }

        if (skip != 0)
            {
            skip = 0;
            if (OK != (status = httpWindMarkParse (writeFctPtr, writeArg, reqId->pSession,
                                        newWorkBuffer,  bytesRead,
                                        &charsParsed, &pExitMark)))
                break;  /* !! deal with this error !!*/
            }
        else
            {
            if (OK != (status = httpWindMarkParse (writeFctPtr, writeArg, reqId->pSession,
                                        workBuffer,  bytesRead,
                                        &charsParsed, &pExitMark)))
                break;  /* !! deal with this error !!*/
            }

        filePos = (ubyte4)(filePos + charsParsed);

        /* check if markParser exited because of a mark that needs evaluation */

        if (NULL != (pExitMark))
           (void) httpMacroExecute (reqId->pSession, pExitMark, &filePos);

        if ((NULL != pExitMark) && (strstr(pExitMark,"createTable")) && (reqId->fWmbTable != HTTP_TRUE))
            {
            ClearCharBool=HTTP_TRUE;
            endTablePos = workBuffer;
            for(count=0;count<bytesRead;count++)
                {
                if (0!=memcmp(endTablePos,"endTable",8))
                    {
                    *endTablePos=' ';
                    endTablePos++;
                    continue;
                    }
                wmbSessionTableSet(reqId->pSession,NULL);
                ClearCharBool=HTTP_FALSE;
                break;
                }
            if (count<bytesRead)
                {
                for(;count<bytesRead;count++)
                    {
                    if (0!=memcmp(endTablePos,"-->",3))
                        {
                        *endTablePos=' ';
                        endTablePos++;
                        continue;
                        }
                    break;
                    }
                }
            filePos = (ubyte4)(filePos + (unsigned long)endTablePos - (unsigned long)workBuffer - charsParsed);
            }

        /* adjust file pointer to re-read unprocessed characters */
        if (charsParsed != bytesRead)  
            (void) httpFileSeek(pF, filePos);

#ifdef DEBUG_INFO
        fprintf (stderr, "httpRpmFs: have read %d, next read %d\n",
                 filePos, bytesToRead);
        fflush(stderr);
#endif
        if ((reqId->repeatStack == NULL) && (reqId->ifStack == NULL) && (reqId->pSession->fileIncludeLevel == 0))
            {
            reqId->ifStack     = FirstifStack;
            reqId->repeatStack = FirstrepeatStack;
            }
        } while (bytesRead);

    (void) httpMacroCleanup(reqId->pSession);
    if (reqId->pSession->fileIncludeLevel == 0)
        {
        if (HTTP_OK != httpBlockPut (reqId, NULL, 0)) /* needed to terminate chunked encoding */
            {
	        return HTTP_ERROR;
            }
        }
    return status;
    }

#ifdef HTTP_ETAG_ENABLE
/**************************************************************************
*
* markFind - find windMarks existence
*
* NOMANUAL
*
* RETURNS: OK or an error
*/

LOCAL STATUS markFind
    (
    HTTP_REQ_ID   reqId,
    generic_fd_t  * pF,
    unsigned long fileSize
    )
    {
    STATUS          status          = ERROR;
    unsigned int    bytesRead       = 0;
    enum WM_STATE   flag            = WM_INVALID;
    char * pBuf                     = NULL;
    char readBuffer[kMagicMarkupBufferSize+1];
    readBuffer[kMagicMarkupBufferSize] = '\0';
 
    (void) httpFileSeek(pF, 0);
    while (fileSize > 0)
        {
        if (OK != httpFileRead(pF, readBuffer, kMagicMarkupBufferSize, &bytesRead))
            {
            (void)wmError (ERROR_GENERAL, "failed reading file");
            break;
            }
        readBuffer[bytesRead] = '\0';
        pBuf = readBuffer;

        while (('\0' != *pBuf) && (WM_FINAL != flag))
            {
            switch (flag)
                {
                case WM_BEGIN:
                    if (wmVarMagicMarkupStartChar1 == *pBuf)
                        flag = WM_START;
                    else
                        flag = WM_INVALID;
                    break;
                case WM_START:
                    if (*pBuf == ' ')
                        flag = WM_START;
                    else
                        flag = WM_MID;
                    break;
                case WM_MID:
                    if (wmVarMagicMarkupEndChar0 == *pBuf)
                        flag = WM_END;
                    break;
                case WM_END:
                    if (wmVarMagicMarkupEndChar1 == *pBuf)
                        flag = WM_FINAL;
                    else
                        flag = WM_MID;
                    break;
                default:
                    if (wmVarMagicMarkupStartChar0 == *pBuf)
                        flag = WM_BEGIN;
                    break;
                }
            pBuf++;
            }

        if (WM_FINAL == flag)
            {
            status = OK;
            break;
            }

        fileSize -= bytesRead;
        }

   return status;
    }
    
#endif

/**************************************************************************
*
* replaceSpace - replace space with "&nbsp;" Acording to: 
*                http://www.w3schools.com/html/html_entities.asp
* If the windmark has more than one blank space then the all the other spaces
* are removed by the browser
*
* NOMANUAL
*
* RETURNS: HTTP_OK or an HTTP_ERROR
*/
/*
LOCAL STATUS replaceSpace
    (sbyte *  srcBuf,
     size_t   len
    )
    {
    sbyte * tempBuf = NULL;
    sbyte * tempBuf1 = NULL;
    sbyte * tempBuf2 = NULL;
    size_t  tempLen = 0;

    if (NULL == srcBuf)
        return HTTP_ERROR;
    
    if ((tempBuf = wmosMalloc (kMagicMarkupBufferSize)) == NULL)
        {
        logInfo("Error replaceSpace: Failed to allocate memory buffer\n");
        return (HTTP_ERROR);
        }
    MEMSET (tempBuf, '\0', kMagicMarkupBufferSize);

    tempBuf1 = srcBuf;
    tempBuf2 = tempBuf;
    while((*srcBuf)&&(len > 0)&&(tempLen < (kMagicMarkupBufferSize -1)))
      {
      if ((*srcBuf) != ' ')
        {
         *tempBuf = *srcBuf;
         srcBuf++;
         tempBuf++;
         len--;
         tempLen++;
        }
      else
        {
        if((tempLen + 6) < kMagicMarkupBufferSize)
          {
           MEMCPY(tempBuf, "&nbsp;",6);
           srcBuf++;
           tempBuf += 6;
           len--;
           tempLen += 6;        
           }
         else
           {
            *tempBuf = ' ';
             srcBuf++;
             tempBuf++;
             len--;
             tempLen++;
           }
        }
      }
      *tempBuf = '\0';

      MEMCPY (tempBuf1, tempBuf2,kMagicMarkupBufferSize);      

      wmosFree (tempBuf2);

      return HTTP_OK;
    }
*/

/**************************************************************************
*
* httpWindMarkParse - output a list of objects from windmarks embedded in text
*
* if httpTxEngine finds a 'special' WM (a WM that needs to be interpreted
* by this RPM instead of by the backplane), it will exit.  In this last case
* ppExitMark returns NULL
*
* RETURNS: OK, or ERROR invalid windmarks
*          pUsed, equal to srcLen except on special exit conditions signaled
*          by pExitMark
*/

STATUS httpWindMarkParse
    (
    STATUS (*writeFctPtr)(void *, sbyte *, ubyte4), /* output routine, expected to be httpBlockPut or wmnetTcpWrite */
    void *            writeArg,  /* socket fd or reqId structure */
    WMB_SESSION_T *   pSession,  /* the current session  environment struct */
    sbyte *           pSrc,      /* data read from resource file */
    unsigned long     srcLen,    /* number of bytes read from file */
    unsigned long *   pUsed,     /* returns number of characters parsed */
    sbyte **          ppExitMark /* signal  occurrence of special WindMark */
    )
    {

    STATUS   status      = OK;
    sbyte *  pBuf        = NULL;
    sbyte *  pMark       = NULL;
    sbyte *  pHead       = NULL;
    unsigned long        markLength  = 0;
    unsigned long        preCount    = 0;
    unsigned long        used        = 0;
    sbyte *  pInstance   = NULL;
    unsigned long        instanceLen = 0;
    HTTP_REQ_ID reqId    = NULL;
    enum WM_STATE flag   = WM_INVALID;
    sbyte *  pErrMsg     = NULL;
    sbyte4   errorNum    = 0;

    sbyte *  pInlineInstance = NULL;
    sbyte    inlineMark[kMagicMarkupBufferSize + 1]; /* should be max instance length ??*/
    /* XXX - consider getting next buffers off the stack */

    sbyte *  pWindMarkName = NULL;
    sbyte *  tempBuf       = NULL;


    if ((NULL == pSrc) || (NULL == pSession) || (NULL == pUsed))
        return ERROR;

    reqId = (HTTP_REQ_ID) wmbSessionStateGet (pSession);
    if ((pSession->type != WMB_TYPE_SMTP_K) && (reqId == NULL))
        {
        return ERROR;
        }

    if ((pWindMarkName = wmosMalloc (kMagicMarkupBufferSize + 1)) == NULL)
        {
        logInfo("Error httpWindMarkParse: Failed to allocate memory buffer\n");
        return (ERROR);
        }

    if ((tempBuf = wmosMalloc (kMagicMarkupBufferSize)) == NULL)
        {
        logInfo("Error httpWindMarkParse: Failed to allocate memory buffer\n");
        wmosFree (pWindMarkName);
        return (ERROR);
        }

    tempBuf[0] = '\0'; /* initialize tempBuf to the empty string */

    *pUsed = 0;
    (*ppExitMark) = 0;

    pBuf = pHead = pSrc;
    while ('\0' != *pBuf)
        {
        flag       = WM_INVALID;
        preCount   = 0;
        markLength = 0;

        /* We have to reset pInstance to NULL */

        pInstance   = NULL;
        memset (tempBuf, 0, kMagicMarkupBufferSize);

        while (('\0' != *pBuf) && (WM_FINAL != flag) && (used < srcLen))
            {
            switch (flag)
                {
                case WM_BEGIN:
                    if (wmVarMagicMarkupStartChar1 == *pBuf)
                        flag = WM_START;
                    else
                        {
                        preCount += 2;
                        flag = WM_INVALID;
                        }
                    break;
                case WM_START:
                    if (*pBuf == ' ')
                        flag = WM_START;
                    else
                        flag = WM_MID;
                    pMark = pBuf;
                    break;
                case WM_MID:
                    markLength++;
                    if (wmVarMagicMarkupEndChar0 == *pBuf)
                        flag = WM_END;
                    break;
                case WM_END:
                    if (wmVarMagicMarkupEndChar1 == *pBuf)
                        {
                        sbyte * bBuf = pBuf - 2;
                        while ((*bBuf == ' ')  && (*bBuf-- != wmVarMagicMarkupStartChar1))
                            markLength--;
                        flag = WM_FINAL;
                        }
                    else
                        flag = WM_MID;
                    break;
                default:
                    if (wmVarMagicMarkupStartChar0 == *pBuf)
                        flag = WM_BEGIN;
                    else
                        preCount++;
                    break;
                }
            used++;
            pBuf++;
            }


        /* flush buffer prior to WM or end of text */

        if (pSession->type != WMB_TYPE_SMTP_K)
            {
            if (preCount)
	          { 
		       if (HTTP_OK!=httpBlockPut (reqId, pHead, preCount))
		            {
                        wmosFree (pWindMarkName);
                        wmosFree (tempBuf);
                        return HTTP_ERROR;
                    }
               } 
            }
#ifdef WEBCLI_INCLUDE_SMTP
        else
            {
            if (preCount)
	           { 
                 if (OK!=(*writeFctPtr)(writeArg, pHead, (ubyte4)preCount))
		            {
                        wmosFree (pWindMarkName);
                        wmosFree (tempBuf);
                        return HTTP_ERROR;
                    }
               }
            }
#endif

        /* if in any of these states, truncated WindMarks => Correct 'used' */
        if       (WM_BEGIN == flag) used -= 1;
        else if  (WM_START == flag) used -= 2;
        else if  ((WM_MID == flag) || (WM_END == flag)) used -= (markLength+3);

  /*
   * SPR#91236 fix; if parsed byte (used) is empty,
   * return error status;incorrect Wind Markup characters
   */
  if (0 == used)
      {
      status = ERROR;

      /*
      * ToDo: Add err string "Invalid WindMarks in HTML page" in
            * httpSysErrorTab[] and send this err string to web client
          */
      break;
      }

        /* no windMark? */

        if (WM_FINAL != flag)
            break;

        /* get windMark data */

        markLength = min(markLength, kMagicMarkupBufferSize);
        STRNCPY(pWindMarkName, pMark, markLength);
        pWindMarkName[markLength] = '\0';

        if (NULL != (pInlineInstance = strstr(pWindMarkName,"{[")))
           {
            /* expand the instances */
             httpExpandInstance (pSession, pInlineInstance, inlineMark);
             if (*inlineMark == '\0')
                break;
             STRCPY (pInlineInstance ,inlineMark);
             pInlineInstance[STRLEN(inlineMark)] = '\0';
             markLength = STRLEN(pWindMarkName);
           }
        else if (NULL != (pInlineInstance = strstr(pWindMarkName,"{(")))
            {
            /* expand the instances */
             httpExpandInstance (pSession, pInlineInstance, inlineMark);
             if (*inlineMark == '\0')
                break;
             STRCPY (pInlineInstance ,inlineMark);
             pInlineInstance[STRLEN(inlineMark)] = '\0';
             markLength = STRLEN(pWindMarkName);
            }


        /* fast check to know if this WindMark has parameters */

        if ( ')' == pWindMarkName[markLength-1])
            {

            /* check if it is a macro implemented by the Web server... */

            if (httpIsMacro(pWindMarkName))
                {
                (*ppExitMark) = pMark; /* pointer to WM in input stream */
                *(pBuf-2) = 0;          /* terminate WM in input stream */

                break;
                }
            else  /* ...or else pass the parameters as instance info */
                {
                STRTOK_REENTRANT (pWindMarkName, "(", &pInstance);
                if (NULL != pInstance)
                    {
                    pWindMarkName[markLength-1] = '\0';  /* kill closing parens */
                    instanceLen = strlen (pInstance) + 1;
                    }
                else          /* bad WM format: let the backplane error this */
                    instanceLen = 0;
                }
            }

        if ((pSession->type != WMB_TYPE_SMTP_K) &&(reqId != NULL)&&(reqId->fWmbTable) && (NULL == pInstance))
            {
            status = wmbTableGet(pSession, pWindMarkName, tempBuf,kMagicMarkupBufferSize );

#ifdef DEBUG_INFO
            logInfo    ("wmbTableGet called. list of args follow");
            logInfoMsg ("  pSesion = %0x", pSession);
            logInfoMsg ("  pWindMarkName = %s", pWindMarkName);
            logInfoMsg ("  tempBuf (result) = %s", tempBuf);
            logInfoMsg ("  sizeOfBuf = %d", kMagicMarkupBufferSize );
#endif
            }
        else
            {
            status = wmbDataStringGet (pSession, pWindMarkName, pInstance,
                                       (sbyte4)instanceLen, tempBuf, kMagicMarkupBufferSize, NULL);

#ifdef DEBUG_INFO
            logInfo    ("wmbDataStringGet called. list of args follow");
            logInfoMsg ("  pSesion = %0x", pSession);
            logInfoMsg ("  pWindMarkName = %s", pWindMarkName);
            logInfoMsg ("  pInstance = %s", (NULL == pInstance) ? "NULL" : pInstance);
            logInfoMsg ("  instanceLength = %d", instanceLen);
            logInfoMsg ("  tempBuf (result) = %s", tempBuf);
            logInfoMsg ("  sizeOfBuf = %d", kMagicMarkupBufferSize );
            logInfo    ("  pWritten = NULL");
#endif
            }

        /*
         * XXX Dealing with missing data must be looked at.
         *
         * The following if() has been modified to fix SPR 90576
         */

        if (status != OK)
            {
            errorNum = wmErrNo();
            }
        else
            {
            errorNum = ERROR_GENERAL_NO_ERROR;
            }

        if (status == OK || ERROR_GENERAL_NO_DATA        == errorNum
                         || ERROR_GENERAL_BUFFER_OVERRUN == errorNum)
            {
            /*
             * Although, status might be ERROR, no data is not considered
             * to be an error.  If a windmark is returned with no text,
             * we (might) need filler.
             */

            if (NULL_STRING(tempBuf))
                {
                status = OK;

                if  ((pSession->type != WMB_TYPE_SMTP_K) && (reqId) && (HTTP_TRUE == reqId->fWmbTable))
                    {
                    /* only within a table */
	        
		     if (HTTP_OK!=httpBlockPut (reqId, "&nbsp;", 6))
		        {
                            wmosFree (pWindMarkName);
                            wmosFree (tempBuf);
                            return HTTP_ERROR;
                        }
               

                    }
                else
                    {
                    /*
                     * policy change: we chose not to send the empty string
                     *
                     * httpBlockPut (reqId, "", 1);
                     */
                    }
                }
            else
                {
                /*
                 * do sanity check if the table creation failed and still returned
                 * string "0"  for integers
                 */

                 if (0 == STRCMP (tempBuf,"0") && ERROR_GENERAL_NO_DATA == errorNum)
                     {
                     /*
                      * policy change: we chose not to send the empty string
                      *
                      * httpBlockPut (reqId, "", 1);
                      */
                     }
                 else
                     {
                     /* normal case; just put the string */
                     if (pSession->type != WMB_TYPE_SMTP_K)
	                 { 
/*                        
	                   if (HTTP_OK!= replaceSpace(tempBuf, (size_t)strlen(tempBuf)))
                           {
                            wmosFree (pWindMarkName);
                            wmosFree (tempBuf);
                            return HTTP_ERROR;
                           }	              
*/                           
			           if (HTTP_OK!=httpBlockPut (reqId, tempBuf, strlen(tempBuf)))
		                   {
                               wmosFree (pWindMarkName);
                               wmosFree (tempBuf);
                               return HTTP_ERROR;
                           }
                      }
#ifdef WEBCLI_INCLUDE_SMTP                     
		             else
        			 {
        			   if (OK != (*writeFctPtr)(writeArg, tempBuf, (ubyte4)strlen(tempBuf)))
        			       {
                               wmosFree (pWindMarkName);
                               wmosFree (tempBuf);
                               return HTTP_ERROR;
        			       }
        			 }
#endif                     
                     }
                }
            }
        else  /* handling of "real" errors */
            {
            /*
             * httpWindMarkGetFailed() is supplied by the user and must
             * not require pErrMsg to be freed.  It is typically the
             * "scratch" buffer which is part of reqId.
             */

            if (pSession->type != WMB_TYPE_SMTP_K)
	        { 
                pErrMsg = (*phttpWindMarkGetFailed) (reqId, pWindMarkName, (unsigned short)(errno));
		    if (HTTP_OK!=httpBlockPut (reqId, pErrMsg, ((pErrMsg) ? strlen (pErrMsg):0)))
		        {
                        wmosFree (pWindMarkName);
                        wmosFree (tempBuf);
                        return HTTP_ERROR;
			}
                }

            status = OK; /* error notified; clean status */
            }

        /* skip over WM markup */

        pHead       = pBuf;
        }

    *pUsed = used;
    wmosFree (pWindMarkName);
    wmosFree (tempBuf);
    return status;

  }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpWmbParserConfigurationShow - displays HTTP WMB parser Configuration list
*
* The WMB parser configuration list is indexed by regular expression encoding
* URL prefixes and each entry's data is the corresponding file system
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpWmbParserConfigurationShow
    (
    void
    )
    {
    httpListShow (httpWmbParserConfigId,
                  "httpWmbParserEnabledUrls",
                  FALSE,    /* entry id is a string */
                  FALSE,    /* entry data is a string */
                  NULL      /* left margin is not required */
                 );

    return;
    } /* httpWmbParserConfigurationShow */
#endif /* HTTP_SHOW */
#endif
