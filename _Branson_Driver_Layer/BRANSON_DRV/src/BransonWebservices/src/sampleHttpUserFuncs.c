/* sampleHttpUserFuncs.c - User implemented functions for Wind River Web Server */

/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
21aug12,r_w  create file
*/

/*
DESCRIPTION
The functions in this module are called by the Wind River Web server, but should
be implemented by the user.
*/

#include "wmw_httpconf.h"
#include "wm.h"
#include "httpLib.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "smtp/smtpLib.h"
#endif
#include "httpFileProcess.h"
#include "httpReq.h"


/* SMTP Verification forms to be displayed after sending emailalert */
#define FULL_DEMO_SMTP_RESOURCE_LOCATION "/romfs/htdocs/"
#define SMTP_SEND_SUCCEEDED  FULL_DEMO_SMTP_RESOURCE_LOCATION "smtpSendSucc.html"
#define SMTP_SEND_FAILED     FULL_DEMO_SMTP_RESOURCE_LOCATION "smtpSendFail.html"
#define SMTP_SEND_ALERT      FULL_DEMO_SMTP_RESOURCE_LOCATION "emailalert.txt"

/* SendNow windMark */
#define MAX_WINDMARK_LENGTH     256 /* Maximum length of any WindMark Value */
char   x_sendNow[MAX_WINDMARK_LENGTH] = "OFF";

/*******************************************************************************
*
* httpWindMarkSetFailed - called when a WindMark write attempt has failed
*
* By default Wind Mark set attempts that fail remain in the request environment,
* so the user can retrieve them later, when httpPostReply is called.
* This function is called right after each failed set attempt so the user can
* store more information about the error in the environment, such as the errno or
* a custom error message.  The information stored at this point can be retrieved
* by httpPostReply by calling httpWmbErrorIterator
*
* This function should be implemented by the user
*
* RETURNS: HTTP_OK
*/

void httpWindMarkSetFailed
    (
    HTTP_REQ_ID reqId,
    sbyte *     windMark,
    ubyte4      errnoVal
    )
    {
    sbyte *     errorMsg;
    MEM_POOL_ID reqMemPool = httpReqMemPartIdGet(reqId);

    logInfoMsg ("httpWindMarkSetFailed: input = %s\n", windMark);

    /* this memory will be freed by the server after request is processed */
    if  (NULL == (errorMsg = memPoolAlloc (reqMemPool, 128)))
        {
        logInfoMsg ("unable to allocate error message memory for %s", windMark);
        return;
        }

    /* create the custom error message based on the returned errno */
    sprintf(errorMsg, "WindMark(%s) set attempt failed. Errno: %u(%s)", windMark, errnoVal,wmGetErrMsg((sbyte4)errnoVal));
    
    /* store it */
    if (NULL == httpWmbErrorStore(reqId, windMark, errorMsg))
        logInfoMsg ("unable to store error message for %s", windMark);
    return;
    }

/*******************************************************************************
*
* httpWindMarkGetFailed - called when the value of a WindMark could not be read
*
* This function is provided so the user can configure the way the server behaves
* when WindMark read errors occur.  On some applications it may be better to
* just ignore the WindMark silently.  In those cases the function should just
* return a pointer to an empty string.
*
* In other cases, like the one shown in this example, an error message is
* printed on the HTML page.  The messages shows the WindMark that could not
* be read along with the errno code returned by the backplane.
*
* The user should implement this function.
*
* RETURNS:
*/

sbyte * httpWindMarkGetFailed
    (
    HTTP_REQ_ID reqId,
    sbyte *     windMark,
    ubyte4      errnoVal
    )
    {
    sbyte * errorMsg = httpBufferGet(reqId);

    /* build a string that will replace the WindMark that failed */
    sprintf(errorMsg, "WindMark(%s) read attempt failed. Errno: %u(%s)", windMark, errnoVal,wmGetErrMsg((sbyte4)errnoVal));

    return errorMsg;
    }

#ifndef WEBCLI_STANDALONE_WEBSERVER

/*******************************************************************************
*
* httpPostReply - display output to the browser after completing an HTTP post
*
* This is an example post reply. You must modify this routine to
* customize both the functionality (behavior) and the visual representation
* of the HTTP post response.
*
*
* RETURNS: HTTP_OK
*/

void httpPostReply
    (
    HTTP_REQ_ID reqId,
    HTTP_STATUS status /* choose OK or ERROR message to be sent to the browser */
    )
    {
    sbyte* failedWM;
    sbyte* errorMsg;
    HTTP_GEN_LIST_ENTRY_ID errEntry = NULL;
    generic_fd_t  f;
    sbyte4 fileStatus;
    WMB_SESSION_T *  pSession = NULL;

    if (0 == strcmp ("ON", x_sendNow) && 0 == strcmp (reqId->szRpmData, "smtp.html"))
    /* user has checked the SMTP alert send now box and this is the SMPT page */
    {

	    if (OK != wmbSessionCreate (httpGetWmbComp(), 0, 0, &pSession))
	    {
	        wmLogPrintf ("unable to create WMB session\n");
	        return;
	    }
	    /* store this request in this session */
	    wmbSessionStateSet (pSession, (void *) reqId);

	    if (HTTP_OK == status)
	        {
	        (void) httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
	        httpStatusSet (reqId, HTTP_OK);       /* Set the HTTP status code     */
	        (void) httpHeaderGenerate (reqId);           /* Send HTTP header             */

        /* send SMTP Mail Alert */
	        status = (HTTP_STATUS)smtpSendAlert(SMTP_SEND_ALERT);

	        if (status == OK)
	            fileStatus = (sbyte4)httpFileOpen(SMTP_SEND_SUCCEEDED, "rb", &f);
	        else
	            fileStatus = (sbyte4)httpFileOpen(SMTP_SEND_FAILED, "rb", &f);

	        if (fileStatus == ERROR)
	        {

	            (void) httpPrintf(reqId, "</HTML></BODY>\n");
	            (void) wmbSessionDestroy(pSession);
	            return;
	        }
	        /* send the appropriate SMTP fail/success file resource  */
	        (void) fileSend (pSession, &f);

	        /* close the file resource */
	        (void) httpFileClose(&f);

		    /* reset x_sendNow to OFF so message is not sent on subsequent reqeusts */
			strcpy(x_sendNow, "OFF");

	        (void) wmbSessionDestroy(pSession);
	        return;

	        }
	    else
	        {
	        (void) httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
	        httpStatusSet (reqId, HTTP_NOT_ACCEPTED);  /* Set the HTTP status code */
	        (void) httpHeaderGenerate (reqId);                /* Send HTTP header         */

	        if (OK != httpFileOpen(SMTP_SEND_FAILED, "rb", &f))
	        {

	            (void) httpPrintf(reqId, "</HTML></BODY>\n");
	            (void) wmbSessionDestroy(pSession);
	            return;     /* not able to find resource */
	        }

	       /* display SMTP_SEND_FAIL resource file  */
	        (void) fileSend (pSession, &f);

	        /* close the  file */
	        (void) httpFileClose(&f);

	        (void) wmbSessionDestroy(pSession);
	        }
	}

    else  /* no SMTP alert */
    {
 	   if (HTTP_OK == status)
 	   {

 	       /* if there are post errors, display them */
	 	   	if (TRUE == httpWmbErrorsExist(reqId))
		    {
		        (void) httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
		        httpStatusSet (reqId, HTTP_OK);       /* Set the HTTP status code     */
		        (void) httpHeaderGenerate (reqId);           /* Send HTTP header             */

	   			(void) httpPrintf(reqId, "<HTML><BODY>\n");
	   			(void) httpPrintf(reqId, "<CENTER><H1>Post received</H1></CENTER>");
				(void) httpPrintf(reqId, "<HR><H3>The following WMB access errors were encountered:</H3>");
				while (NULL != (errEntry = httpWmbErrorIterator(reqId, errEntry, &failedWM, &errorMsg)))
			          {
			           httpPrintf (reqId, "<B>%s</B> <FONT COLOR=\"red\">%s</FONT><BR>", failedWM, errorMsg);
			          }
			    (void) httpPrintf(reqId, "</HTML></BODY>\n");
				return;
			}

			/*
			 * if not SMTP and no post errors exist, resend the page from
			 * which the post request was made and return.
			 */
			reqId->sMethod = HTTP_M_GET;
			(void) httpRpmWmbParse(reqId);
#if 0
		    httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
				    httpStatusSet (reqId, HTTP_OK);  /* Set the HTTP status code */
				    httpHeaderGenerate (reqId);                /* Send HTTP header         */

				    httpPrintf(reqId, "<HTML><BODY>\n");
				    httpPrintf(reqId, "<HR><H2>HTTP Request was accepted!</H2>\n");
				    httpPrintf(reqId, "The transmitted file was stored on the server.\n");
				    httpPrintf(reqId, "</HTML></BODY>\n");
#endif 				    
  		}

		else   /* http server errors exist, so transmit the error response */
    	{
		    (void) httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
		    httpStatusSet (reqId, HTTP_NOT_ACCEPTED);  /* Set the HTTP status code */
		    (void) httpHeaderGenerate (reqId);                /* Send HTTP header         */

		    (void) httpPrintf(reqId, "<HTML><BODY>\n");
		    (void) httpPrintf(reqId, "<HR><H2>HTTP Request not accepted!</H2>\n");
		    (void) httpPrintf(reqId, "The transmitted file was not stored on the server.\n");
		    (void) httpPrintf(reqId, "</HTML></BODY>\n");
		}
    return;
	}
}
#endif
/* eof sampleHttpSvrFuncs.c */

