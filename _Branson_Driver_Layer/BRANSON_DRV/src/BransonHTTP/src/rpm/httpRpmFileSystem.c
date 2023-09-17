/* httpRpmFileSystem.c - Filesystem RPM */

/* Copyright (c) 1996/97, 1999-2004, 2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
05o,26sep14,r_w  fix static CHECKED_RETURN issue.
05n,27aug14,r_w  close file decription for defect VXW6-83291
05m,08apr13,r_w  fix static analysis issues for defect WIND00411995
05l,16may12,r_w  eliminate buiding warnings
                 defect WIND00349929
05k,29jul11,m_z  remove harmful session destroy for not modified response
05j,07jun11,m_z  add etag/if-modified-since process WIND00203773
                 CQID: WIND00203773
05i,17dec10,m_z  part of APIs changed
                 CQID: Fix WIND00247096
05h,02feb10,h_y  support for LP64  
05g,26mar10,m_z  delete unused HTTP_MAX_REQLINE_SIZE definition
                 CQID: Fix WIND00205202
05f,26apr04,vmk  uncommented and modified http get modification time code.
05e,12mar04,adb  introduced httpFileSystemRootGet 
05d,05mar04,adb  introduced httpSecureFsConfAdd
05c,14jan04,gsr  cleanup routine for SPR 93009
05b,10nov03,adb  WRS coding conventions modifications
05a,31oct03,adb  added httpFileSystemsShow routine
04h,26jan00,csr  fixing SPR 29994 (floating point operation causes 'Device Not
                 Found' exception on targets with no floating point support)
04g,24jan00,va   fixed accept encoding (added httpNullByte comparison)
04f,05oct99,va   file read made consistent for all filesystems
04e,30sep99,va   buffer allocation bug fixed
04d,27aug99,va   added mechanism for setting content encoding.
04c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
04b,06apr98,ck   buffer allocated from memory pool (usually bigger, thus faster)
04a,06apr98,ck   support for persistent connections added.
03b,15may97,mb   osal introduced.
03a,27feb97,mb   generic lists are used now instead of array tables.
02c,28sep96,mb   configuration structure moved to httpconfig.c.
02b,28sep96,mb   bug fix - remove leading '/' from filenames.
02a,02sep96,mb   beta test.
01c,15aug96,mb   bug fixed (close).
01b,12aug96,mb   Debug output added.
01a,29jul96,mb   initial creation.
*/

/*
DESCRIPTION

INCLUDE FILES: httpLib.h
               memPoolLib.h
               httpList.h
               httpRpms.h
               private/httpInt.h
               httpShow.h
*/

/* includes */

#include <string.h>
#include <stdio.h>

#include "commonCfg.h"
#include "httpCfg.h"
#include "httpLib.h"
#include "httpRpms.h"
#include "private/httpInt.h"
#include "httpShow.h"
#ifdef HTTP_ETAG_ENABLE
#include "httpMimeEtag.h"
#endif

#undef   DEBUG_INFO_RPMFS /* local to this module */

/* locals */

LOCAL HTTP_GEN_LIST_ID httpFsConfigId HTTP_PREDEFINE (NULL);
LOCAL HTTP_GEN_LIST_ID httpSecureFsConfigId HTTP_PREDEFINE (NULL);

/***************************************************************************
*
* httpFsConfInit - initialize the sizes of the file system configuration list
*
* This routine initializes the internal boundaries needed for the
* subsequent calls of all the other API functions to access the
* file system configuration tables.
*
* RETURNS: HTTP_OK or HTTP_ERROR
*
* SEE ALSO: httpFsConfAdd 
*           httpSecureFsConfAdd
*/

HTTP_STATUS httpFsConfInit
    (
    unsigned short sTabMax /* Maximum number of entries in predefined tables */
    )
    {
    if  (NULL == 
         (httpFsConfigId = httpGenListCreate (httpGlobalMemPartIdGet(), 
                                              sTabMax, 
                                              HTTP_TRUE)))
        {
        return (HTTP_ERROR);
        }

    if  (NULL == 
         (httpSecureFsConfigId = httpGenListCreate (httpGlobalMemPartIdGet(), 
                                                    sTabMax, 
                                                    HTTP_TRUE)))
        {
        httpFsConfCleanup ();

        return (HTTP_ERROR);
        }
                                   
    return (HTTP_OK);

    } /* httpFsConfInit */

/***************************************************************************
*
* httpFsConfCleanup - Cleanup routine for httpFsConfInit
*
* RETURNS: HTTP_OK always 
*
* SEE ALSO: httpFsConfInit()
*/

HTTP_STATUS httpFsConfCleanup
    (
    void
    )
    {
    if  (NULL != httpFsConfigId)
        {
        httpGenListDestroy (httpFsConfigId);
        }

    httpFsConfigId = NULL;

    if  (NULL != httpSecureFsConfigId)
        {
        httpGenListDestroy (httpSecureFsConfigId);
        }

    httpSecureFsConfigId = NULL;

    return (HTTP_OK);

    } /* httpFsConfCleanup */

/***************************************************************************
*
* httpFsConfAdd - add an entry to the file-system RPM list
*
* This routine adds the specified entry to the file-system RPM list.
*
* NOTE: The string is not copied.  Only a pointer to its location is
* stored.
*
* RETURNS: HTTP_GEN_LIST_ENTRY_ID of the new configuration entry, or
* NULL if the new entry could not be added.
*
* SEE ALSO: httpFsConfInit()
*/

HTTP_GEN_LIST_ENTRY_ID httpFsConfAdd
    (
    char * szUrl,         /* Incoming URL request          */
    char * szFsRoot       /* Corresponding filesystem path */
    )
    {
    return (httpGenListDataAppend (httpFsConfigId, szUrl, szFsRoot));
    }

/***************************************************************************
*
* httpSecureFsConfAdd - add an entry to the secure file-system RPM list
*
* This routine adds the specified entry to the secyre file-system RPM list.
*
* NOTE: The string is not copied.  Only a pointer to its location is
* stored.
*
* RETURNS: HTTP_GEN_LIST_ENTRY_ID of the new configuration entry, or
* NULL if the new entry could not be added.
*
* SEE ALSO: httpFsConfInit()
*/

HTTP_GEN_LIST_ENTRY_ID httpSecureFsConfAdd
    (
    char * szUrl,         /* Incoming URL request          */
    char * szFsRoot       /* Corresponding filesystem path */
    )
    {
    return (httpGenListDataAppend (httpSecureFsConfigId, szUrl, szFsRoot));
    }

/***************************************************************************
*
* httpFsRootGet - get the corresponding file system prefix
*
* If successful it retrieves the corresponding file system and copies it
* to the provided buffer if it fits in it.
*
* RETURNS: OK or ERROR
*/

HTTP_STATUS httpFsRootGet
    (
    HTTP_REQ_ID     reqId,
    char *          pRoot,
    size_t   bufferSize
    )
    {
    HTTP_GEN_LIST_ID        auxListId = NULL;
    HTTP_GEN_LIST_ENTRY_ID  configId =  NULL;
    char *                  pData =     NULL;

    auxListId = (mudConnSSLVerify (reqId->ioHandle)) ? 
                httpSecureFsConfigId : 
                httpFsConfigId;

    if  (NULL == (configId = httpGenListFind (auxListId , 
                                              httpRpmIdentGet (reqId))))
        {
        return (HTTP_ERROR);
        }

    if  ((NULL == (pData = httpGenListDataGet (configId))) ||
         (bufferSize < strlen (pData) + 1))
        {
        return HTTP_ERROR;
        }

    strcpy (pRoot, pData); /* we have already compared buffersize and strlen */

    return (HTTP_OK);

    } /* httpFsRootGet */

/***************************************************************************
*
* httpRpmFs - read a file and send it to the client
*
* This is the file-system RPM routine.
* It provides the usual functionality one expects from an HTTP
* server.  It reads a file and writes it to the network.
* The file can be read from anywhere in the file system (hard disk, NFS,
* memory file system, and so on).
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*/

short httpRpmFs
    (
    HTTP_REQ_ID reqId   /* Handle of the active request, needed for all */
                        /* http - API function calls                    */
    )
    {
    generic_fd_t           f;
    unsigned int           lCount;
    char *                 szBuffer;
    sbyte *                cpTransBuffer;
    unsigned long          ulMaxTransBufSize;
    MEM_POOL_ID            mpPool;
    /*sbyte *                contentEnc = NULL;*/
    ssize_t                lFileSize=0;
    unsigned long          lReadCount=0;
    unsigned int          bytesRead;
    
    szBuffer = httpBufferGet (reqId);
    /*
    if (httpServerUncompress == HTTP_FALSE)
        contentEnc = httpMimeContentEncodingGet ( reqId, HDR_IN);
    */
    if  (HTTP_OK != httpFsRootGet (reqId, 
                                   szBuffer,
                                   httpMaxTempBufSize))

        {
        httpStatusSet( reqId, HTTP_NOT_FOUND );
        (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_FILE_NOT_FOUND));
        return (RPM_ERROR);
        }

    strncat (szBuffer,
             httpRpmDataGet (reqId),
             httpMaxTempBufSize - strlen (szBuffer) - 1);

    /* determine filesize and set content length (important for persist. connections!) */
#ifdef DEBUG_INFO_RPMFS
    fprintf(stderr, "httpRpmFs: request is %s\n", szBuffer);
    fflush(stderr);
#endif

    if (OK != httpFileOpen(szBuffer, "rb", &f))
        {
        httpStatusSet( reqId, HTTP_NOT_FOUND );
        (void) httpError( reqId, httpErrStringGet (HTTP_ERROR_FILE_NOT_FOUND));
#ifdef DEBUG_INFO_RPMFS
        httpPrintf (reqId, "<P><HR><H2>Could not open the file <TT>'%s'</TT></H2><HR></P>\n", szBuffer);
#endif
        return RPM_ERROR;
        }
    if (0 >= (lFileSize = httpFileSize(&f)))
        {
        (void) httpFileClose(&f);
        return RPM_OK;
        }
    (void) httpMimeContentLengthSet (reqId, HDR_OUT, (long) lFileSize);

    mpPool = httpReqMemPartIdGet (reqId);

 
#ifdef DEBUG_INFO_RPMFS
    fprintf(stderr, "httpRpmFs: setting size to %ld\n", lFileSize);
    fflush(stderr);
#endif

#ifdef HTTP_ETAG_ENABLE
    unsigned long          rc = 0;
    sbyte *                szTime;
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
        (void)memPoolFree (mpPool, szTime);
        }
    else
        {
        logInfo ("httpRpmFs: out of request-pool memory");
        }


    char *szOutEtag = NULL; 
    const char *szInEtag;
    const char *szInIMS ;
    struct tm biInIMS, *biOutIMS;
    HTTP_ETAG_BINARY       biOutEtag; 
    HTTP_ETAG_BINARY       biInEtag; 
    int biInDay = 0;
    int biInSec = 0;
    int biOutDay = 0;
    int biOutSec = 0;

    memset(&biInIMS, 0, sizeof(biInIMS));

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
            if (HTTP_ERROR != httpEtagValueGet(szInEtag, &biInEtag))
                {
                if (biInEtag.file == biOutEtag.file 
                    && biInEtag.size == biOutEtag.size
                    && biInEtag.time == biOutEtag.time)
                    {
                    httpStatusSet (reqId, HTTP_NOT_MODIFIED);
                    httpNotModifiedResp(reqId);

                    (void)memPoolFree (mpPool, szOutEtag);
                    (void)httpFileClose (&f);
 
                    return RPM_DONE;
                    }
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
#endif


    ulMaxTransBufSize = (unsigned long)lFileSize;
    cpTransBuffer = httpConservativeAlloc (mpPool, (size_t *)&ulMaxTransBufSize);

     /* don't read more than the allotted */

    lCount = (unsigned int)((ulMaxTransBufSize > lFileSize) ? lFileSize : ulMaxTransBufSize);

    if (cpTransBuffer == NULL)
        {
        logError ("httpRpmFs: out of request-pool memory");
        httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
        httpError( reqId, httpErrStringGet (HTTP_ERROR_ENTITY_TOO_LARGE));
        (void) httpFileClose(&f);
        return (RPM_ERROR);
        }
    else
        {
#ifdef DEBUG_INFO_RPMFS
        fprintf (stderr, "httpRpmFs: using transfer buffer size %ld\n", ulMaxTransBufSize);
        fflush(stderr);
#endif
        }

    logInfoMsg ("httpRpmFs: opening file %s...\n",szBuffer);

    /* if we are here, we have a non-empty open file waiting to be read */
    httpStatusSet (reqId, HTTP_OK);
    /* if a Content-Type was registered for the file, use it.  Else, leave the one
       guessed by the Mime RPM */
    if (httpFileContentTypeGet(&f))
        (void) httpMimeHdrSet(reqId, HDR_OUT, "Content-Type", httpFileContentTypeGet(&f));
    (void) httpHeaderGenerate (reqId);
    do
        {
        if (OK != httpFileRead(&f, cpTransBuffer, lCount, &bytesRead))
            {
            logInfo("httpRpmFs: Transfer Interrupted!\n");
            httpStatusSet (reqId, HTTP_SERV_UNAVAILABLE);
            (void) httpError (reqId,httpErrStringGet(HTTP_ERROR_TRANSFER_INTERRUPTED));
            (void) memPoolFree (mpPool, cpTransBuffer);
            (void) httpFileClose(&f);
            return RPM_ERROR;
            }
        lReadCount += bytesRead;
        if (HTTP_ERROR == httpBlockPut (reqId, cpTransBuffer, bytesRead))
            break;

        lCount = (unsigned int)((ulMaxTransBufSize > (lFileSize - lReadCount)) ? (lFileSize-lReadCount ) :
             ulMaxTransBufSize);
#ifdef DEBUG_INFO_RPMFS
        fprintf (stderr, "httpRpmFs: have read %ld, next read %ld\n", lReadCount, lCount);
        fflush(stderr);
#endif
        if (0 == lCount) break;

        } while (bytesRead);

    (void) httpFileClose (&f);
    (void) memPoolFree (mpPool, cpTransBuffer);

    return RPM_DONE;
    }

#ifdef HTTP_SHOW
/***************************************************************************
*
* httpFileSystemsShow - displays HTTP File Systems
*
* The file systems configuration list is indexed by URL prefixes and each 
* entry's data is the corresponding internally resolvable directory path.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpFileSystemsShow
    (
    void
    )
    {
    httpListShow (httpFsConfigId, 
                  "httpFileSystems", 
                  FALSE,    /* entry id is a string */
                  FALSE,    /* entry data is a string */
                  NULL      /* left margin is not required */
                 );

    return;
    } /* httpFileSystemsShow */

/***************************************************************************
*
* httpSecureFileSystemsShow - displays HTTP Secure File Systems
*
* The secure file systems configuration list is indexed by URL prefixes and 
* each entry's data is the corresponding internally resolvable directory path.
*
* RETURNS: NA
*
* NOMANUAL
*/
void httpSecureFileSystemsShow
    (
    void
    )
    {
    httpListShow (httpSecureFsConfigId, 
                  "httpSecureFileSystems", 
                  FALSE,    /* entry id is a string */
                  FALSE,    /* entry data is a string */
                  NULL      /* left margin is not required */
                 );

    return;
    } /* httpSecureFileSystemsShow */
#endif /* HTTP_SHOW */
