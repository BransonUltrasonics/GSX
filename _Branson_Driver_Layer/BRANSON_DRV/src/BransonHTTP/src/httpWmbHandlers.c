/* httpWmbHandlers.c - Wind River Web Backplane handlers */

/* Copyright 2002-2007,2010,2016 Wind River Systems, Inc. */

/*
modification history
-------------------- 
17feb16,r_w  fix compiler and static analysis warnings.
02feb10,h_y  support for LP64 
15jan04,jws  add reference to global variable
23oct02,jc   written
*/

/*
DESCRIPTION
This module contains useful handlers to deal with HTML forms.

*/

/* includes */
#include "wm.h"
#include <string.h>
#include "httpCfg.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"


/* externals - will move to header file */

STATUS selHandler(WMB_TID_T   tid,WMB_OBJ_T   selObj,COMMAND_T   cmd );
STATUS checkedHandler(WMB_TID_T   tid, WMB_OBJ_T   chkObj, COMMAND_T   cmd );
STATUS jsManglerHandler(WMB_TID_T   tid,WMB_OBJ_T   jsObj,COMMAND_T   cmd);

IMPORT ubyte4 wmVarMagicMarkupBufferSize;

/***************************************************************************
*
* selHandler - SELECTED handler to be used in SELECT form elements
*
* This function expects to find two parameters in the object instance
* field:  a WindMark name, and a value.  If the value matches at least
* one of the WindMark values, the returned value is "SELECTED"
* Otherwise it's an empty string.
*
* This handler will work correctly with "multiple selects", that
* is, selects where more than one option is selected.
*
* NOTE: This handler only supports GET requests.
*
*
* RETURNS: ERROR or OK.
*/

STATUS selHandler
    (
    WMB_TID_T   tid,        /* transaction id  */
    WMB_OBJ_T   selObj,     /* the SELECTED WM */
    COMMAND_T   cmd         /* handler command */
    )
    {
    sbyte *         pSelectedPars;
    sbyte *         pSelectName;
    sbyte *         pSelectValue;
#if 0
    sbyte           buffer[kMagicMarkupBufferSize] = "SELECTED";
#else
    sbyte *         pBuffer;
#endif
    WMB_SESSION_T * pSession;
    STATUS          status;
    WM_BUFFER_T *   pInstance;

    pSession = wmbTransactionSessionGet(tid);

    if (CMD_GET_K != cmd)
        return wmError(ERROR_GENERAL_READ_ONLY, "write attempt on a read only WindMark");

    if (TRUE == wmbObjectBad(selObj))
        return wmError(ERROR_GENERAL, "invalid WindMark");

    /* get the parameters of SELECTED */

    if (NULL == (pSelectedPars = wmbObjectInstancePtrGet(selObj)))
        return ERROR;

    /* parse the parameters */
    STRTOK_REENTRANT (pSelectedPars, ",", &pSelectValue);

    pSelectName = pSelectedPars;

    if ((NULL == pSelectValue) || (NULL == pSelectName))
        return ERROR;

    /*
     we should now have the name of the Select WindMark in pSelectName
     and the value we have to compare with in pSelectValue
     */

    pBuffer = wmMalloc(wmVarMagicMarkupBufferSize);

    if (pBuffer == NULL)  return ERROR;

    /* Need to get the instance info if this is a table object */

    if ((pInstance = wmbTableInstanceSfxGet (pSession, pSelectName)) != NULL)
        status = wmbDataStringGet (pSession, pSelectName,
                                   wmBufferDataGet(pInstance),
                                   (sbyte4)wmBufferUsedGet(pInstance), pBuffer,
                                   wmVarMagicMarkupBufferSize, NULL);
    else
        status = wmbDataStringGet (pSession, pSelectName, NULL, 0, pBuffer,
                                   wmVarMagicMarkupBufferSize, NULL);

#ifdef __WM_DEBUG__
    printf ("%s is %s (%s)\n", pSelectName, pBuffer, pSelectValue);
#endif

    if (OK != status)
        {
        wmFree(pBuffer);
        return ERROR;
        }

    if (strcmp (pBuffer, pSelectValue) == 0)
        strcpy (pBuffer, "SELECTED");
    else
        strcpy (pBuffer, " ");

    /* load result into SELECTED object's value buffer */

    wmbObjectImport (selObj, pBuffer, strlen(pBuffer), DATATYPE_STRING_K);
    wmFree(pBuffer);

    return OK;
    }

/***************************************************************************
*
* checkedHandler - CHECKED handler to be used in CHECKBOX form elements
*
* This function expects to find two parameters in the object instance
* field:  a WindMark name, and a value.  If the value matches at least
* one of the WindMark values, the returned value is "CHECKED"
* Otherwise it's an empty string.
*
* NOTE: This handler only supports GET requests.
*
* RETURNS: ERROR or OK.
*/

STATUS checkedHandler
    (
    WMB_TID_T   tid,        /* transaction id  */
    WMB_OBJ_T   chkObj,     /* the CHECKED WM */
    COMMAND_T   cmd         /* handler command */
    )
    {
    sbyte *         pCheckedPars;
    sbyte *         pChkBoxName;
    sbyte *         pChkBoxValue;
#if 0
    sbyte           buffer[kMagicMarkupBufferSize] = "CHECKED";
#else
    sbyte *         pBuffer;
#endif
    WMB_SESSION_T * pSession;
    STATUS          status;
    WM_BUFFER_T *   pInstance;

    pSession = wmbTransactionSessionGet(tid);

    if (CMD_GET_K != cmd)
        return wmError(ERROR_GENERAL_READ_ONLY, "write attempt on a read only WindMark");

    if (TRUE == wmbObjectBad(chkObj))
        return wmError(ERROR_GENERAL, "invalid WindMark");

    /* get the parameters of Checked */

    if (NULL == (pCheckedPars = wmbObjectInstancePtrGet(chkObj)))
        return ERROR;

    /* parse the parameters */
    STRTOK_REENTRANT (pCheckedPars, ",", &pChkBoxValue);

    pChkBoxName = pCheckedPars;

    if ((NULL == pChkBoxValue) || (NULL == pChkBoxName))
        return ERROR;

    /*
     we should now have the name of the checkbox WindMark in pChkBoxName
     and the value we have to compare with in pChkBoxValue
     */

    pBuffer = wmMalloc(wmVarMagicMarkupBufferSize);

    if (pBuffer == NULL)  return ERROR;

    /* Need to get the instance info if this is a table object */

    if ((pInstance = wmbTableInstanceSfxGet (pSession, pChkBoxName)) != NULL)
        status = wmbDataStringGet (pSession, pChkBoxName,
                                   wmBufferDataGet(pInstance),
                                   (sbyte4)wmBufferUsedGet(pInstance),
                                   pBuffer, wmVarMagicMarkupBufferSize, NULL);
    else
        status = wmbDataStringGet (pSession, pChkBoxName, NULL, 0, pBuffer,
                                   wmVarMagicMarkupBufferSize, NULL);

    if (OK != status)
        {
        wmFree(pBuffer);
        return ERROR;
        }

    if (0 != strcmp (pBuffer, pChkBoxValue))
        strcpy (pBuffer, " ");
    else
        strcpy (pBuffer, "CHECKED");

    /* load result into CHECKED object's value buffer */

    wmbObjectImport (chkObj, pBuffer, strlen(pBuffer), DATATYPE_STRING_K);
    wmFree(pBuffer);

    return OK;
    }



/***************************************************************************
*
* jsManglerHandler - mangle WindMark names so they can be used by JavaScript
*
* JavaScript cannot work with form elements that contain colons and/or
* dots in their names, although those are valid WindMark names.  In order to
* solve this, this function can be used to produce valid JS names that
* will be converted into standard WindMarks by an RPM (httpRpmJsPost).
*
* This handler expects the WindMark to be called with 3 parameters:  the
* pre-colon prefix, the WindMark name and the instance suffix.  If the first
* character in the instance suffix is a decimal digit, it will be interpreted
* as a constant dotted OID string.
* Otherwise it will be assumed that it is a WindMark that evaluates into a
* dotted OID string (such as getInstance).
*
* The first and third parameters are optional, but the comma separator is
* expected even if those arguments are missing (i.e. js(,myWindMark,) or
* js(,myOtherWindMark,getInstance))
*
* To use this handler, first the user must link it to a WindMark that in this
* example we'll call 'js()'.
*
* Then, if the user wants to name a form element with a WindMark value with
* colons and dots (such as rfc1234:myNose.1.2.3.4) he would use this WindMark
* in the following way in his HTML page:
*
* <... name=$%js(rfc1234,myNose,1.2.3.4)#$ value=$%rfc1234:myNose.1.2.3.4#$>
*
* After the server processes the page, this is what it would look like on the
* client side:
*
* <... name=__jsr_rfc1234_myNose_1_2_3_4 value=[whatever the value is] >
*
* Now the JavaScript code has a perfectly valid element name to work on.
* Forms containing this sort of WindMarks should be processed by httpRpmJsPost
* who demangles the name back into it's original format.
*
* NOTE: This handler only supports GET requests.
*
* RETURNS: ERROR or OK.
*/

STATUS jsManglerHandler
    (
    WMB_TID_T   tid,        /* transaction id  */
    WMB_OBJ_T   jsObj,      /* the js WM       */
    COMMAND_T   cmd         /* handler command */
    )
    {
    sbyte *         pTempBuf;
    sbyte *         pWmName;
    sbyte *         pWmPrefix;
    sbyte *         pWmSuffix;
    sbyte *         pArgs = NULL;
#if 0
    sbyte           buffer[kMagicMarkupBufferSize] = "__js_";
#else
    sbyte *         pBuffer;
#endif
    WMB_SESSION_T * pSession;
    sbyte4          used;

    pSession = wmbTransactionSessionGet(tid);

    if (CMD_GET_K != cmd)
        return wmError(ERROR_GENERAL_READ_ONLY, "write attempt on a read only WindMark");

    if (TRUE == wmbObjectBad(jsObj))
        return wmError(ERROR_GENERAL, "invalid WindMark");

    /* get the parameters of js */

    if (NULL == (pTempBuf = wmbObjectInstancePtrGet(jsObj)))
        return ERROR;

    /* parse the parameters */

    if (NULL == (pWmPrefix = STRTOK_REENTRANT (pTempBuf, ",", &pWmSuffix)))
        return ERROR;

    pBuffer = wmMalloc(wmVarMagicMarkupBufferSize);

    if (pBuffer == NULL)  return ERROR;

    strcpy(pBuffer,"__js_");

    if (',' != *pTempBuf)   /* a prefix was provided */
        {
        strcat (pBuffer, pWmPrefix);
        strcat (pBuffer, "_");
        if (NULL == (pWmName = STRTOK_REENTRANT (NULL, ",", &pWmSuffix)))
            {
            wmFree(pBuffer);
            return ERROR;
            }
        }
    else
        pWmName = pWmPrefix;

    strcat (pBuffer, pWmName);

    if (0 == *pWmSuffix)       /* if no suffix, done */
        {
        wmbObjectImport (jsObj, pBuffer, strlen(pBuffer), DATATYPE_STRING_K);
        wmFree(pBuffer);
        return OK;
        }

    strcat (pBuffer, "_");
    used = (sbyte4)strlen (pBuffer);
    pTempBuf = pBuffer + used;   /* save beginning of suffix pointer */

    /* if suffix is a windmark, get it's value */

    if (!isdigit (*pWmSuffix))
        {

        /* pWmName will now point to the third arg WindMark's name */

        pWmName = STRTOK_REENTRANT (pWmSuffix, "(", &pWmPrefix);

        if (NULL != pWmPrefix) /* reused pWmPrefix to hold strtok state */
            pArgs = STRTOK_REENTRANT (NULL, ")", &pWmPrefix);

		/* reset pArgs if NULL at this point */
        if (NULL == pArgs)
        	pArgs = "";

        (void)wmbDataStringGet (pSession, pWmName, pArgs, (sbyte4)(strlen(pArgs) + 1),
                                   pTempBuf, wmVarMagicMarkupBufferSize - used,
                                   NULL);

        /* if windMark failed, httpWindMarkGetFailed will have copied the
           default error message on pTempBuf.  If this is not appropriate
           you may change that here

        if (OK != status)
            {
            strcpy (pTempBuf, "x.x.x.x");
            }
        */
        }
    else
        strcpy (pTempBuf, pWmSuffix);

    /* buffer now is '__js_prefix_wmname_1.2.3.4.5' : replace '.'s with '_'s */

    while (NULL != (pTempBuf = strchr(pTempBuf, '.')))
        (*pTempBuf) = '_';

    /* load result into object's value buffer */

    wmbObjectImport (jsObj, pBuffer, strlen(pBuffer), DATATYPE_STRING_K);
    wmFree(pBuffer);

    return OK;
    }
#endif

