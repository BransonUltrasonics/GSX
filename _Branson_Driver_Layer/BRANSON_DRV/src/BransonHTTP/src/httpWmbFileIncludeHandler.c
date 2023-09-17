/* httpWmbFileIncludeHandler.c - WMB handler for the fileInclude WindMark */

/*
 * Copyright (c) 2003-2007,2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------- 
17feb16,r_w  fix compiler and static analysis warnings.
05may14,r_w  fix static analysis issues(US35919)
02feb10,h_y  support for LP64 
23feb07,msa  Correct the apigen comments so they match the actual
             routines.
20may04,adb  testing wmbSessionType
19may04,adb  using relative paths
18aug03,adb  added to source control
*/

/*
DESCRIPTION
This module contains the WMB handler for the fileInclude WindMark

INCLUDE FILES: wm.h wmb.h httpLib.h httpFileProcess.h
*/

/* includes */

#include "wm.h"
#include "httpCfg.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"
#include <string.h>
#include "httpLib.h"
#include "httpReq.h"
#include "httpFileProcess.h"

/* defines */

/* typedefs */

/* globals */

/***************************************************************************
*
* wmbFileIncludeHandler - WMB handler for the fileInclude WindMark
*
* The fileInclude WindMark takes the file name as a parameter and it always
* returns the empty string as value. However, as a side effect of the windMark 
* evaluation, if the input file exists it is transmitted to the HTTP client.
*
* This function, wmbFileIncludeHandler, can be the fileInclude WindMark handler.
*
* NOTE: This handler only supports GET requests.
*
* RETURNS: ERROR or OK.
*
* NOMANUAL
*/

STATUS wmbFileIncludeHandler
    (
    WMB_TID_T   tid,        /* transaction id */
    WMB_OBJ_T   obj,        /* the WindMark object */
    COMMAND_T   cmd         /* handler command */
    )
    {
    WMB_SESSION_T * pSession =            NULL;

    sbyte *         pFileName =           NULL;

    WM_BUFFER_T *   pBuffer =             NULL;
    sbyte *         pBufferData =         NULL;

    /*sbyte           tempInstanceHolder[100];*/

    generic_fd_t    fileDescriptor;

    HTTP_REQ_ID     reqId =               NULL;

    char *          pChar =               NULL;;

    /* tempInstanceHolder[0] = 0;*/

    if  (NULL == (pSession = wmbTransactionSessionGet(tid)))
        {
        return (wmError (ERROR_GENERAL, 
                         "wmbFileIncludeHandler: invalid session"));
        }

    if  (CMD_GET_K != cmd)
        {
        return (wmError (ERROR_GENERAL_READ_ONLY, 
                         "wmbFileIncludeHandler: getColumnar is read only"));
        }

    while (wmbObjectGood (obj))
        {
        if  (NULL == (pFileName = wmbObjectInstancePtrGet(obj)))
            {
            return (wmError (ERROR_GENERAL_NO_DATA, 
                             "wmbFileIncludeHandler: file name is missing"));
            }

        if  (wmbSessionTypeGet(pSession) == WMB_TYPE_WEB_K)
            {
            if  (NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet (pSession)))
                {
                return (wmError (ERROR_GENERAL_NO_DATA,
                                "wmbFileIncludeHandler: invalid reqId"));
                }

            pChar = httpBufferGet (reqId);

            if  (HTTP_OK != httpFsRootGet (reqId, 
                                           pChar,
                                           httpMaxTempBufSize))

                {
                return (wmError (ERROR_GENERAL,
                                "wmbFileIncludeHandler: httpFsRootGet failed"));
                }

            strncat (pChar,
                     pFileName,
                     httpMaxTempBufSize - strlen (pChar) - 1);
            }
        else
            {
            reqId = NULL;

            pChar = pFileName;
            }

        if  (OK != httpFileOpen (pChar, "rb", &fileDescriptor))
            {
            return (wmError (ERROR_GENERAL_NO_DATA,
                            "wmbFileIncludeHandler: unable to open file"));
            }

        if  (OK != fileSend (pSession, &fileDescriptor))
            {
            if  (OK != httpFileClose (&fileDescriptor))
                {
                return (wmError (ERROR_GENERAL_NO_DATA,
                                "wmbFileIncludeHandler: send AND close error"));
                }   

            return (wmError (ERROR_GENERAL_NO_DATA,
                            "wmbFileIncludeHandler: fileSend failed"));
             
            }

        if  (OK != httpFileClose (&fileDescriptor))
            {
            return (wmError (ERROR_GENERAL_NO_DATA,
                            "wmbFileIncludeHandler: unable to close file"));
            }

        pBuffer = wmbObjectValueGet (obj);
        pBufferData = wmBufferDataGet (pBuffer);
        if(NULL != pBufferData)
          pBufferData[0] = 0;

        obj = wmbObjectNextGet (obj);
        }

    return OK;

    } /* wmbFileIncludeHandler */
#endif

