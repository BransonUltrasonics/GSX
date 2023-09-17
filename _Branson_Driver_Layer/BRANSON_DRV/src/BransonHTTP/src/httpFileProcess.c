/* httpFileProcess.c - Parse resource file routines */

/*
 * Copyright (c) 2003-2008,2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
19jul10,h_y  modified for WIND00223629
02feb10,h_y  support for LP64 
30jun08,q_z  fix using REPEAT macro in pages sent via POST (WIND00039488)
31jan08,ps   back out last changes; they introduce problems elsewhere...
25jan08,ps   add macro processing to fileSendProcess()
05sep07,ten  Add include to allow building for linux target
13feb07,msa  Fix comments to remove APIgen errors.
27apr04,adb  replaced all buffer size constants with kMagicMarkupBufferSize
06feb04,dkg  Removed fileSendChunkProcess() with reference to SPR 91304
30jan04,dkg  Modify ERROR_GENERAL_NO_DATA handling in fileSendChunkProcess()
             to fix SPR 91304
16jan04,jws  remove per project dependencies
15aug03,adb  wm_options.h instead of rc_options.h
15aug03,adb  initialized status to OK in fileSendChunkProcess
14aug03,adb  added to source control
*/

/*
DESCRIPTION
This library provides the basic APIs used by SMTP MTA, to retrieve the email
alert file and parse it for WindMarks before sending the parsed data to SMTP
Server. Also the global API fileSend() can be called from HTTP session to
send out the parsed file to browser. Files can be recursively included within
the files upto maximum level defined in MAX_FILE_INCLUDE_LEVEL. Value of this
macro depends on the stack size of task which is executing the fileSend().
Enough stack must be allocated to handle the required level of recursive file
inclusions.

INCLUDE FILES: httpFileProcess
*/

/* includes */


/* #include "wm_options.h" */

#include "wm.h"
#include <webcli_osconf.h>

#include "httpFileProcess.h"
#include "httpRpms.h"
#include "httpCfg.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER
/* externals - will move to header file */

IMPORT char wmVarMagicMarkupStartChar0;
IMPORT char wmVarMagicMarkupStartChar1;
IMPORT char wmVarMagicMarkupEndChar0;
IMPORT char wmVarMagicMarkupEndChar1;

ubyte        ufileSendWithReqId = 0;
HTTP_REQ_ID  pHttpfileSendReqId = NULL;

/*******************************************************************************
*
* fileSend - check session type & assign routines to parse & send resource file.
*
* This routine gets the size of resource file and allocates the storage buffer
* for reading the contents of the file. It is called from SMTP_SendResource() or
* from 'Include File' WindMark scalar handler. Based on session type, it assigns
* appropriate ouput routines to be used by fileSendProcess(). For SMTP Session
* it assigns wmnetTcpWrite() as output routine and server socket fd as argument.
* For HTTP Session, it assigns httpBlockPut() as output routine and pointer to
* reqId structure as its argument. It calls fileSendProcess() to read and parse
* the contents of file for evaluating WindMarks. After fileSendProcess returns,
* it frees the allocated memory and returns.The arguments passed to this routine
* are, pointer to WMB session structure 'pSession' for current session & generic
* file descriptor f to access the resource file.
*
* RETURNS: OK or ERROR
*
* SEE ALSO: smtpLib
*/

STATUS fileSend
    (
    WMB_SESSION_T *pSession, /* environment structure for the current session */
    generic_fd_t *f     /* generic file descriptor to access resource file */
    )
    {
    ssize_t  lFileSize =  0;
    sbyte * workBuffer = NULL;
    void *  reqId =      NULL;

    STATUS (*writeFctPtr)(void *, sbyte *, ubyte4);
    void * writeArg = NULL;

    /* check validity of session */
    if (pSession == NULL)
        {
        logInfo("Error fileSend: Invlaid Session Id\n");
        return (ERROR);
        }

    /* recursive usage of 'fileSend' to include sub files, depends on
     * the stack size of this task. So limit the maximum level of
     * sub-file inclusions.
     */
    if (++pSession->fileIncludeLevel >= MAX_FILE_INCLUDE_LEVEL)
        {
        logInfo("Error fileSend: crosses max file include level\n");
        return (ERROR);
        }

    /* Get the size of resource file */
    if (0 >= (lFileSize = (ssize_t)httpFileSize(f)))
        {
        logInfo("Error fileSend: failed to get file size\n");
        return (ERROR);
        }

    reqId = wmbSessionStateGet (pSession);

#ifdef WEBCLI_INCLUDE_SMTP
    /* check whether it is HTTP or SMTP Session and call the
     * appropriate routines with required arguments for parsing resource file
     */
    if (wmbSessionTypeGet(pSession) == WMB_TYPE_SMTP_K)
        {
        writeFctPtr = (STATUS (*)(void *, sbyte *, ubyte4)) wmnetTcpWrite;
        writeArg = (void *)(long)pSession->sock;
        }
    else   /* Its an HTTP Session */
#endif        
        {

        if (reqId == NULL)
            {
            logInfo("Error fileSend: Invalid reqId in HTTP Session\n");
            return (ERROR);
            }

        writeFctPtr = (STATUS (*)(void *, sbyte *, ubyte4)) httpBlockPut;
        writeArg = reqId;
        }

    /* allocate working memory buffer for storing while reading from file */

    if ((workBuffer = wmosMalloc (MAX_READ_BYTES + 1)) == NULL)
        {
        logInfo("Error fileSend: Failed to allocate memory buffer\n");

        return (ERROR);
        }

    /* read the data from file to parse it for WindMarks */
    if(lFileSize > 0)
      (void)fileSendProcess (writeFctPtr,
                     writeArg,
                     pSession,
                     f,
                     (size_t)lFileSize,
                     workBuffer,
                     MAX_READ_BYTES);

    /* free allocated memory before return */
    wmosFree (workBuffer);

    if (pSession->fileIncludeLevel)
        pSession->fileIncludeLevel--;

    return (OK);

    } /* fileSend */

/*******************************************************************************
*
* fileSendProcess - read the file & pass the data to fileSendChunkProcess
*
* This routine reads the resource file through generic file descriptor
* pFileDescriptor, and pass the data to fileSendChunkProcess(), to parse the
* WindMarks.It gets called from fileSend() with following arguments:writeFctPtr:
* a pointer to function httpBlockPut() or wmnetTcpWrite(), writeArg: Argument to
* writeFctpPtr(), holds sock fd for SMTP Session or reqId for HTTP session,
* pSession: pointer to WMB Session structure for the current session
* pFileDescriptor: a generic file descriptor to acess the resource file,
* fileSize: size of resource file, auxBuffer: allocated buffer to read the file,
* and auxBufferSize: size of the auxbuffer.
*
* RETURNS: OK or ERROR
*
* SEE ALSO: smtpLib
*/

STATUS fileSendProcess
    (
    STATUS (*writeFctPtr)(void *, sbyte *, ubyte4), /* output routine, expected to be httpBlockPut or wmnetTcpWrite */
    void *          writeArg,        /* socket fd or reqId structure */
    WMB_SESSION_T * pSession,        /* current session  environment struct */
    generic_fd_t *  pFileDescriptor, /* generic file descriptor of resource */
    size_t          fileSize,        /* size of resource file */
    sbyte *         auxBuffer,       /* allocated buffer to read the file */
    size_t          auxBufferSize    /* size of auxillary buffer */
    )
    {
    WMB_SESSION_T * wmbSession    = NULL;
    size_t          charsParsed   = 0;
    size_t          filePos       = 0;
    size_t          bytesRead     = 0;
    size_t          bytesToRead   = 0;
    sbyte *         pExitMark     = NULL;
    HTTP_REQ_ID     reqId         = NULL;
    WMB_SESSION_T * reqIdSession  = NULL;

    if (OK != wmbSessionCreate (httpGetWmbComp(), (int)(long)writeArg, (ubyte4)0, &wmbSession))
        {
        wmLogPrintf ("unable to create WMB session\n");
        return ERROR;
        }

    logInfoMsg ("wmbSessionCreate called successfully (sessionId = %0x)", wmbSession);

    /* store this request in this session */
    wmbSessionStateSet (wmbSession, (void *) writeArg);

    wmbSessionTypeSet (wmbSession, wmbSessionTypeGet(pSession));
    wmbSession->fileIncludeLevel = pSession->fileIncludeLevel;

    if (pSession->type != WMB_TYPE_SMTP_K)
        {
        if ((reqId = (HTTP_REQ_ID) wmbSessionStateGet (pSession)) != NULL)
            {
            /* process the macros and windMarks in the send file */
            if (OK != markParserDrvr (NULL, NULL, reqId, pFileDescriptor, fileSize, auxBuffer,
                                      auxBufferSize))
                {
                wmError (ERROR_GENERAL, "failed process macros and windMarks in send http file");
                httpStatusSet (reqId, HTTP_INTERNAL_ERROR);
                (void)httpHeaderGenerate (reqId);
                (void)httpBlockPut (reqId, NULL, 0); /* terminate chunked encoding */
                (void) wmbSessionDestroy ( reqId->pSession );
                }
            }
        }
#ifdef WEBCLI_INCLUDE_SMTP
    else
        {
        /* macros support in the send file when session type is
         * WMB_TYPE_SMTP_K and with a http req structure
         */
        if ((ufileSendWithReqId == 1) && (pHttpfileSendReqId != NULL))
            {
            /* store this request in this session */
            reqId           = pHttpfileSendReqId;
            reqIdSession    = reqId->pSession;
            reqId->pSession = wmbSession;
            wmbSessionStateSet (wmbSession, (void *)reqId);

            /* process the macros and windMarks in the send file */
            if (OK != markParserDrvr (writeFctPtr, writeArg, reqId, pFileDescriptor, fileSize, auxBuffer,
                                      auxBufferSize))
                {
                wmError (ERROR_GENERAL, "failed process macros and windMarks in send smtp file");
                httpStatusSet (reqId, HTTP_INTERNAL_ERROR);
                (void)httpHeaderGenerate (reqId);
                (void)httpBlockPut (reqId, NULL, 0); /* terminate chunked encoding */
                (void) wmbSessionDestroy ( reqIdSession );
                }
            reqId->pSession = reqIdSession;
            (void) wmbSessionDestroy(wmbSession);
            return (OK);
            }

        /* No macros support in the send file when session type is WMB_TYPE_SMTP_K*/
        do
            {
            /* don't read more than what the buffer can hold */
            bytesToRead = (auxBufferSize > (fileSize - filePos)) ?
                              (fileSize-filePos ) : (auxBufferSize);

            if (0 == bytesToRead) break;

            /* Read from file only atmost MAX_READ_BYTES at a time */
            if (OK != httpFileRead(pFileDescriptor, auxBuffer,
                                   (ubyte4)bytesToRead, (ubyte4 *)&bytesRead))
                {
                wmError (ERROR_GENERAL, "failed reading file");
                break;
                }

            logInfoMsg("httpRpmWmbParse: parsing %d chars\n", bytesRead);

            /*
             * Call httpWindMarkParse to process file contents, replace WindMarks
             * and send it to SMTP Server
             */
            if (OK != httpWindMarkParse (writeFctPtr, writeArg, wmbSession,
                                         auxBuffer,  (unsigned long)bytesRead,
                                         (unsigned long *)&charsParsed, &pExitMark))
                {
                wmError (ERROR_GENERAL, "failed parsing WindMarks in send file");
                break;
                }

            filePos += charsParsed;

            /* adjust file pointer to re-read unprocessed characters */
            if (charsParsed != bytesRead) httpFileSeek(pFileDescriptor, (ubyte4)filePos);

#ifdef DEBUG_INFO
            fprintf (stderr, "httpRpmFs: have read %d, next read %d\n",
                     filePos, bytesToRead);
            fflush(stderr);
#endif
            } while (bytesRead);
        }
#endif /* WEBCLI_INCLUDE_SMTP */

    (void) wmbSessionDestroy(wmbSession);

    return (OK);
    } /* fileSendProcess */
#endif

