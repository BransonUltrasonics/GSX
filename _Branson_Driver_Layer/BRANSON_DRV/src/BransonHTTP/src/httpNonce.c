/* httpNonce.c - JS support routines */

/* Copyright(c) 2005-2007, 2010, 2012 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01e,18apr12,r_w  remove dependence of retrofit.h
                 Requirement: WIND00299859
01d,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01c,24feb07,msa  Correct the apigen comments so they match the actual routines
                 and correct any apigen errors that may occur.
01b,25apr05,???  Update JS Auth #define ordering
01a,17sep04,dkg  ported from RC3.3 to WM4.4 for JavaScript Authentication
*/

/* See if Digest is enabled globally */
#include "commonCfg.h"
#include "httpCfg.h"

#ifdef __JAVASCRIPT_DIGEST_ENABLED__

#include "httpLib.h"
#include "httpAuth.h"
#include "httpNonce.h"


STATUS wmbGetNonceHandler
    (
    WMB_TID_T   tid,        /* transaction id */
    WMB_OBJ_T   obj,        /* the WindMark object */
    COMMAND_T   cmd         /* handler command */
    )
    {
    sbyte *pHtmlOutputBuf;
    STATUS status;
    WMB_SESSION_T * pSession = wmbTransactionSessionGet(tid);

    pHtmlOutputBuf = wmAUTHGetSentNonce(pSession->clientIndex);
    status = wmbObjectImport(obj,pHtmlOutputBuf,32,DATATYPE_STRING_K);

    return status;
    }


#endif

