/* httpRpmPost.c - Evaluate forms posted to the server */

/* Copyright (c) 2001-2008,2010, 2012-2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------- 
09oct16,r_w  webserver does not replace spaces (V7MAN-216)
25aug16,r_w  replace "%A0" with a space (V7MAN-210)
28may14,r_w  fix compile warnings(V7MAN-78)
05mar14,r_w  disable form post feature in webserver standalone mode. defect VXW6-80639 and VXW6-1806
10sep13,r_w  make REGISTRY_ReaderEnter and REGISTRY_ReaderExit are handled symmetrically for defect WIND00433888
08apr13,r_w  fix static analysis issues for defect WIND00411995
18apr12,r_w  remove dependence of retrofit.h
             Requirement: WIND00299859
02feb10,h_y   support for LP64 
27feb09,y_z  Add codes to process error which may be happened during java 
             script authentication
01apr08,q_z  Fix defect WIND00113013
07mar07,ps   fixes for RTP builds. defect WIND00044864 (add fcn prototypes)
22feb07,ps   apigen reformatting
13feb07,msa  Fix comments to remove APIgen errors.
24jan07,ps   rewrite httpRpmPost socket read loop. Defect  WIND00070628,
             fix WIND00086310
25apr05,???  Update JS Auth #define ordering
13sep04,dkg  Modify httpRpmPost() to support JavaScript Authentication 
             feature
19jul04,vmk  fix 88639, Truncated excessive objects in evaluatePost.
22aug03,jws  fix compiler warning
29may03,adb  memory management in evaluatePost and strcpy before strcat
04feb03,jws  fix compiler warnings (SPR84193)
06nov01,jc   initial creation.
*/

/*
DESCRIPTION

This module implements a POST handler for HTML forms.  It only
supports content encoded as 'application/x-www-form-urlencoded' (the default
content encoding method for forms).  This RPM must be used in combination
with another RPM (such as httpRpmWmbPost) to do something useful with
the form elements that it stores in the environment.

INCLUDE FILES:

*/


#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "httpLib.h"
#include "httpRpms.h"
#include "httpReq.h"
#include "private/httpInt.h"
#include "httpCfg.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER
/* constants */
#define kSpecialMagicMarkup     "CheckBoxManager"

extern void REGISTRY_ReaderEnter(void);
extern void REGISTRY_ReaderExit(void);
extern void EscapeCgiArgument( sbyte *pCgiString);

#ifdef __JAVASCRIPT_DIGEST_ENABLED__

#include "httpAuth.h"

#define kCookieEntityLen  32 + kMaxNonceLen 
static STATUS wmJSLogonResp(WMB_SESSION_T *p_envVar);

#endif


/* locals */
typedef struct token
{
    unsigned long      MagicMarkupNameLen;             /* length of the MagicMarkup to assign */
    sbyte              *pMagicMarkupName;              /* pointer to the MagicMarkup string */

    Length             assignMagicMarkupValueLen;      /* length of the value to assign */
    sbyte              *pAssignMagicMarkupValue;       /* pointer to the value string */

    sbyte              *pDataStream;                   /* ptr to where we left off last time */
    unsigned long      dataStreamLen;                  /* the number of bytes remaining in the */
                                                /* data stream to be processed */
} token;

enum memPoolModes_t
    {
    kMemTotalAvailable =  0,
    kMemContigAvailable
    };


LOCAL STATUS evaluatePost
    (
    HTTP_REQ_ID          reqId,
    sbyte                *pDataStream,
    unsigned long        dataStreamLen
    );

/*******************************************************************************
*
* httpPostConfInit - initializes the RCB post handler
*
* RETURNS: HTTP_OK
*/

HTTP_STATUS httpPostConfInit (void)
    {
    return HTTP_OK;
    }

/*******************************************************************************
*
* httpRpmPost - process form elements processed to the server
*
* RETURNS: RPM_DONE if post was successful, RPM_ERROR otherwise.
*/

short httpRpmPost
    (
    HTTP_REQ_ID reqId   /* Handle of the active request, needed for all */
                        /* http - API function calls                    */
    )
    {
    unsigned long          entitySize;
    unsigned long          memAvail;
    MEM_POOL_ID            mpPool;
    const sbyte *          contentEnc = NULL;
    sbyte *                pEntity = NULL;

    long                   bytesRead;
    unsigned long          bytesRemaining;
    unsigned long          chunkSize;
    sbyte *                tmpBuf;
	
    
#ifdef __JAVASCRIPT_DIGEST_ENABLED__
    STATUS                 status;
    WMB_SESSION_T          *pSession = NULL;
#endif

    contentEnc = httpMimeHdrGet (reqId, HDR_IN, "Content-Type");
    if (strcmp(contentEnc, "application/x-www-form-urlencoded"))
        return RPM_OK; /* some other RPM may deal with it */

    entitySize = strtoul(httpMimeHdrGet(reqId, HDR_IN, "Content-Length"),NULL,0);

    if (0 == entitySize)
        {
        (void)(*phttpPostReply)(reqId, HTTP_OK);
        return RPM_DONE;    /* empty POST, done */
        }

    mpPool = httpReqMemPartIdGet(reqId);

    memAvail = memPoolAvail(mpPool, kMemContigAvailable);
    if (entitySize > memAvail)
        {   
        httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
        (void) httpError (reqId, "httpRpmPost: increase HTTP_MAX_REQ_PART_SIZE");
        reqId->fPersistent = HTTP_FALSE; /* close connection */
        return RPM_DONE;
        }


    /* allocate memory for the post entity (the CGI stream) */
    if (NULL == (pEntity = (sbyte *) memPoolAlloc(mpPool, entitySize)))
        {
        httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
        (void) httpError (reqId, "httpRpmPost: unable to allocate mem for entity. Increase HTTP_MAX_REQ_PART_SIZE");
        reqId->fPersistent = HTTP_FALSE;
        return RPM_DONE; 
        }

    /* Transfer from socket to buffer */
    tmpBuf = pEntity;
    bytesRemaining = entitySize;
    while (bytesRemaining > 0)
        {
        if (httpMaxTempBufSize < bytesRemaining)
            chunkSize = httpMaxTempBufSize;
        else
            chunkSize = bytesRemaining;
        bytesRead = httpBlockRead (reqId, tmpBuf, chunkSize);
        if (bytesRead < 0)
            return (HTTP_ERROR);
        bytesRemaining -= bytesRead;
        tmpBuf += bytesRead;
        }

    if (OK != evaluatePost(reqId, pEntity, entitySize))
        {
        httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
        (void) httpError (reqId, "httpRpmPost: unable to evaluate POST data. Increase HTTP_MAX_REQ_PART_SIZE");
        reqId->fPersistent = HTTP_FALSE;
        return RPM_DONE; 
        }
    
#ifdef __JAVASCRIPT_DIGEST_ENABLED__

    if (OK != wmbSessionCreate ( httpGetWmbComp(), 0, 0, &pSession ))
        {
        wmLogPrintf ("unable to create WMB session\n");
        return (short)wmError (ERROR_GENERAL, "unable to create WMB session");
        }

    /* store this request in this session */

    wmbSessionStateSet (pSession, (void *) reqId);

    /* configure session type */

    wmbSessionTypeSet (pSession, WMB_TYPE_WEB_K);

 
 /* this is a custom response necessary if JavaScript authentication is enabled.
 * If the user has just logged in, a cookie must be generated and transmitted to
 * the browser (for authentication of subsequent HTTP requests).  -LC */


    REGISTRY_ReaderEnter();
    status = wmJSLogonResp  ( pSession );
 /*If status is OK, httprpmwmbparse return RPM_DONE , then we return RPM_DONE.
 *  If status is RPM_ERROR, httpRpmWmbParse return RPM_ERROR, then we return RPM_ERROR.
 *  If status is ERROR_GENERAL , then it is not a java script digest Post request return RPM_OK.
 */
    if ( OK == status)
        {
        REGISTRY_ReaderExit();
        (void) wmbSessionDestroy ( pSession );
	    return RPM_DONE;
        }  
    else if(RPM_ERROR == status)
       {
    	REGISTRY_ReaderExit();
    	(void) wmbSessionDestroy ( pSession );
		reqId->fPersistent = HTTP_FALSE; /* close connection */
    	return RPM_ERROR;
       }
    REGISTRY_ReaderExit();    
    (void) wmbSessionDestroy ( pSession );
#endif


    return RPM_OK; /* not RPM_DONE because some other RPM must finish its job */

    }


#ifdef UNDEFINED
/*-----------------------------------------------------------------------*/
/****************************************************************************
*
* ParseLeftValue - 
*
* NOMANUAL
*
*/

LOCAL void ParseLeftValue
    (
    sbyte *pMagicMarkup,
    sbyte *pArgs
    )
{
    while (isalnum((sbyte4)*pMagicMarkup))
        pMagicMarkup++;

    if (NULL != pArgs)
        strcpy(pArgs, pMagicMarkup);

    *pMagicMarkup = '\0';
}
#endif


/*-----------------------------------------------------------------------*/
/****************************************************************************
*
* EscapeCgiArgument - 
*
* NOMANUAL
*
*/

extern void EscapeCgiArgument
    (
    sbyte *pCgiString
    )
{
    sbyte   Char, digit;
    sbyte   *pSlideChars;

    if (NULL != pCgiString)
        while ('\0' != *pCgiString)
        {
            /* replace '+' with a space */
            if ('+' == *pCgiString)
            {
                *pCgiString = ' ';
                pCgiString++;
                continue;
            }

            if ('%' == *pCgiString)
            {
                digit = pCgiString[1];

                /* converting hex string to value */
                if (('0' <= digit) && ('9' >= digit))
                    Char  = (sbyte)((digit - '0') << 4);
                else if (('A' <= digit) && ('F' >= digit))
                    Char  = (sbyte)(((digit - 'A') + 10) << 4);
                else if (('a' <= digit) && ('f' >= digit))
                    Char  = (sbyte)(((digit - 'a') + 10) << 4);
                else
                {
                    /* skip past bogus '%' and continue escaping... */
                    pCgiString++;
                    continue;
                }

                digit = pCgiString[2];

                if (('0' <= digit) && ('9' >= digit))
                    Char  = Char | (sbyte)(digit - '0');
                else if (('A' <= digit) && ('F' >= digit))
                    Char  = Char | (sbyte)((digit - 'A') + 10);
                else if (('a' <= digit) && ('f' >= digit))
                    Char  = Char | (sbyte)((digit - 'a') + 10);
                else
                {
                    pCgiString++;
                    continue;
                }

                *pCgiString = Char;
                
                /* replace "%A0" with a space */
                /*
                if (('A' == pCgiString[1]) && ('0' == pCgiString[2]))
                    *pCgiString = ' ';
                */

                pSlideChars = ++pCgiString;

                do
                    *pSlideChars = pSlideChars[2];
                while ('\0' != *pSlideChars++);

                continue;

            } /* escaping '%' */

            pCgiString++;

        } /* while */

} /* EscapeCgiArgument */



/*-----------------------------------------------------------------------*/
/****************************************************************************
*
* ParseCgiStream - 
*
* NOMANUAL
*
*/

LOCAL STATUS ParseCgiStream
    (
    token *p_tokPassBack
    )
{
    p_tokPassBack->pMagicMarkupName = p_tokPassBack->pDataStream;
    p_tokPassBack->MagicMarkupNameLen = 0;

    /* get the lvalue */
    while ((('=' != *p_tokPassBack->pDataStream)) && (0 < p_tokPassBack->dataStreamLen))
    {
        p_tokPassBack->MagicMarkupNameLen++;
        p_tokPassBack->pDataStream++;
        p_tokPassBack->dataStreamLen--;
    }

    if (p_tokPassBack->dataStreamLen == 0)
        return ERROR_POST_NO_MORE_MAGICMARKUPS;

    /* skip the '=' */
    p_tokPassBack->dataStreamLen--;
    p_tokPassBack->pAssignMagicMarkupValue = (++p_tokPassBack->pDataStream);
    p_tokPassBack->assignMagicMarkupValueLen = 0;

    /* get the rvalue */
    while (('&' != *p_tokPassBack->pDataStream) && (0 < p_tokPassBack->dataStreamLen))
    {
        p_tokPassBack->assignMagicMarkupValueLen++;
        p_tokPassBack->pDataStream++;
        p_tokPassBack->dataStreamLen--;
    }

    /* adjust pointers for next cgi value */
    if (0 < p_tokPassBack->dataStreamLen)
    {
        p_tokPassBack->pDataStream++;
        p_tokPassBack->dataStreamLen--;
    }

    return OK;

}  /* ParseCgiStream */



/*******************************************************************************
*
* evaluatePost - parse CGI stream and put it into the environment
*
* NOMANUAL
*
* RETURNS: OK or an error
*/

LOCAL STATUS evaluatePost
    (
    HTTP_REQ_ID   reqId,
    sbyte         *pDataStream,
    unsigned long  dataStreamLen
    )
    {
    token   tokData;
    sbyte   *pMagicMarkup, *pVal, *pAppendVal, *pOldVal, *pArgs;
    BOOL    flagMagicMarkup = OK;
    MEM_POOL_ID memPool = httpReqMemPartIdGet (reqId);

    if  ((NULL == pDataStream) || (0 >= dataStreamLen))
        {
        return ERROR_GENERAL_NO_DATA;
        }

    if  (NULL == (pArgs = (sbyte *)
                          memPoolAlloc
                          (memPool,
                           httpMaxUrlSize)))
        {
        return ERROR_MEMMGR_NO_MEMORY;
        }

    tokData.pDataStream   = pDataStream;
    tokData.dataStreamLen = dataStreamLen;

    while ((0 < tokData.dataStreamLen) && (OK == flagMagicMarkup))
        {
        if  (OK != ParseCgiStream(&tokData))
            break;

        if  (NULL == (pMagicMarkup = (sbyte *)
                                     memPoolAlloc
                                     (memPool,
                                      tokData.MagicMarkupNameLen + 1)))
            {
            (void) memPoolFree (memPool, pArgs);
            return ERROR_MEMMGR_NO_MEMORY;
            }

        if  (NULL == (pVal = (sbyte *)
                             memPoolAlloc
                             (memPool,
                              tokData.assignMagicMarkupValueLen + 1)))
            {
           (void) memPoolFree (memPool, pMagicMarkup);
           (void) memPoolFree (memPool, pArgs);
            return ERROR_MEMMGR_NO_MEMORY;
            }

        STRNCPY(pMagicMarkup,
                tokData.pMagicMarkupName,
                tokData.MagicMarkupNameLen);
        STRNCPY(pVal,
                tokData.pAssignMagicMarkupValue,
                tokData.assignMagicMarkupValueLen);

        pMagicMarkup[tokData.MagicMarkupNameLen] = '\0';
        pVal[tokData.assignMagicMarkupValueLen]  = '\0';

        EscapeCgiArgument(pMagicMarkup);
        EscapeCgiArgument(pVal);

        if  (NULL == (pOldVal = httpGetEnv(reqId, pMagicMarkup)))
            {
                httpSetEnv(reqId, pMagicMarkup, pVal);
            }
        else
            {

            /* duplicate name in posted data:  append values */

            if  (NULL == (pAppendVal = (sbyte *)
                                       memPoolAlloc
                                       (memPool,
                                        strlen(pVal) + strlen(pOldVal) + 2)))
                     /* add one byte for the null and one for the comma */
                {
                (void)memPoolFree (memPool, pVal);
                (void)memPoolFree (memPool, pMagicMarkup);
                (void)memPoolFree (memPool, pArgs);
                return ERROR_MEMMGR_NO_MEMORY;
                }

            /* coverity[secure_coding] */
            strcpy (pAppendVal, pOldVal);
            /* coverity[secure_coding] */
            strcat (pAppendVal, ",");
            /* coverity[secure_coding] */
            strcat (pAppendVal, pVal);

            if (NULL == httpSetEnv(reqId, pMagicMarkup, pAppendVal))
               {
               #ifdef __WM_DEBUG__
               printf("evaluatePost setEnv [%s] Failed: Increase HTTP_MAX_REQ_PART_SIZE\n",pMagicMarkup );
               #endif /* __WM_DEBUG__ */
               flagMagicMarkup = ERROR;
               }

            (void)memPoolFree (memPool, pAppendVal);
            }

        (void)memPoolFree (memPool, pVal);
        (void)memPoolFree (memPool, pMagicMarkup);

        } /* while */

    (void)memPoolFree (memPool, pArgs);
    return OK;

    }   /* evaluatePost */

#if 0 
/*******************************************************************************
*
* reportRPMError - return RPM_ERROR and display error message
*
* NOMANUAL
*
* RETURNS: RPM_ERROR
*/

LOCAL short reportRPMError
    (
    sbyte * pErrMsg
    )
    {
    logError (pErrMsg);
    wmError (ERROR_GENERAL, pErrMsg);
    return RPM_ERROR;
    }
#endif


#ifdef __JAVASCRIPT_DIGEST_ENABLED__

/****************************************************************************
*
* wmJSLogonResp - 
*
* NOMANUAL
*
*/

static STATUS wmJSLogonResp
    (
    	WMB_SESSION_T *p_envVar
    )
    {
    sbyte cookieEntity[kCookieEntityLen];

    if (OK != CgiLookup(p_envVar, "nonce", cookieEntity, kMaxNonceLen))
        return ERROR_GENERAL;
    else
        {
        HTTP_REQ_ID reqId;               
        reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar);
        if ( httpRpmWmbParse (reqId) < 0 )
            return RPM_ERROR;  
        }
 
   return OK;
   }

#endif /* __JAVASCRIPT_DIGEST_ENABLED__ */
#endif
