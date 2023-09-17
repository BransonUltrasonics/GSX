/* sampleUserRpm.c - Sample custom user RPM code           */

/*
 * Copyright (c) 1996/97, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
11may11,r_w    Add static limitid for variate counter.
18may04,rws    renamed from user_rpm.c to sampleUserRpm.c
02sep96,pk     beta test
19aug96,pk     documentation reworked
12aug96,ck     initial creation
*/

/*
INCLUDE FILES
              httpLib.h
*/

/* includes */

#include "httpLib.h"
#include "httpRpms.h"
#include "sampleUserRpm.h"


static short counter HTTP_PREDEFINE (0);

/****************************************************************************
*
* httpRpmUserExample - simple "hello world" rpm
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise
*/

short httpRpmUserExample
    (
    HTTP_REQ_ID reqId      /* Handle of the active request, needed for all */
                         /* http - API function calls                    */
    )
    {

    /*
     * Do all neccessary actions to determine if the request can be
     * handled correctly.
     */

    httpStatusSet (reqId, HTTP_OK);       /* Set the HTTP status code     */

    /*
     * Set the content type of the outgoing document.
     * For valid types look at the file httpglob.c or for full reference
     * consider the HTTP specification.
     */
/*
    httpMimeContentTypeSet (reqId, HDR_OUT, "text/html")
*/

    (void) httpHeaderGenerate (reqId);           /* Send HTTP header             */

    /*
     * Here, you can send the HTML body, using the API function
     * httpPrintf (handle, format string, parameters . . . );
     */

    (void) httpPrintf (reqId,"<HTML><BODY>\n");
    (void) httpPrintf (reqId,"<H1>Hello world.</H1>\n");
    (void) httpPrintf (reqId,"This is the output of a sample RPM... you can do whatever you like here!</p>\n");
    (void) httpPrintf (reqId,"The httpPrintf acts like any normal printf; it ");
    (void) httpPrintf (reqId,"takes format strings and so on</p>\n");
    (void) httpPrintf (reqId,"<p><hr><p>This page has been accessed %d times.\n", counter);

#ifdef _DEBUG
    printf ("you can of course also write to the console.... User RPM was called %d times!\n", counter);
#endif
    /* and do anything application specific! */

    counter = (short)(counter + 1);

    (void) httpPrintf (reqId,"</BODY></HTML>\n");

    return (RPM_DONE);
    }

#ifndef WEBCLI_STANDALONE_WEBSERVER
/****************************************************************************
*
* httpRpmUserExample1 - simple custom rpm 
* 
* This sample custom rpm is used to capture the windmark using POST method
* Normally, httpRpmPost() and httpRpmWmbPost() are used to do this.
* 
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise
*/

#include "httpReq.h"

extern sbyte4  g_access;
extern void httpPostReply    (
    HTTP_REQ_ID reqId,
    HTTP_STATUS status 
    );

short httpRpmUserExample1
    (
    HTTP_REQ_ID reqId    
    )
{
    unsigned long          entitySize;
    unsigned long          memAvail;
    long                   bytesRead;
    unsigned long          bytesRemaining;
    unsigned long          chunkSize;
    MEM_POOL_ID            mpPool;
    char *                 pEntity = NULL;
    char *                 tmpBuf =NULL;
    char *                 paddr  = NULL;
    char *                 paddr1 = NULL;
    char                   username[32]= "";
    char                   password[32]= "";
    
    
	   printf("\r\n httpRpmUserExample1 was called.\r\n");
	   
	   entitySize = strtoul(httpMimeHdrGet(reqId, HDR_IN, "Content-Length"),NULL,0);
	   if (0 == entitySize)
	        {
	        httpPostReply(reqId, HTTP_OK);
	        return RPM_DONE;    /* empty POST, done */
	        }

	    mpPool = httpReqMemPartIdGet(reqId);

	    memAvail = memPoolAvail(mpPool, 1);
	    if (entitySize > memAvail)
	        {   
	        httpStatusSet (reqId, HTTP_ENTITY_TOO_LARGE);
	        (void) httpError (reqId, "httpRpmPost: increase HTTP_MAX_REQ_PART_SIZE");
	        reqId->fPersistent = HTTP_FALSE; /* close connection */
	        return RPM_DONE;
	        }

	    /* allocate memory for the post entity */
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
	        bytesRead = (unsigned long)httpBlockRead (reqId, tmpBuf, chunkSize);
	        if (bytesRead < 0)
	            return (HTTP_ERROR);
	        bytesRemaining -= bytesRead;
	        tmpBuf += bytesRead;
	        }
	pEntity[entitySize]= '\0';  /* pEntity have already stored the post value */
	
	printf("\r\n captured value:(%s).\r\n",pEntity);
	
	/*Now you can process these post value: "name1=value1&name2=value2&name3=value3..."*/
	paddr = strstr(pEntity,"&");
	pEntity[paddr-pEntity] = '\0';
	
	/* username=xxx */
    if (strlen((char *)(pEntity+9)) < 32)
 	    strcpy(username, pEntity+9);  /* <input type="text" name="username" value=""><br> */
	
	paddr1 = strstr(paddr+1,"&");
	pEntity[paddr1-pEntity] = '\0';
	
	/* password=xxx */
    if (strlen((char *)(paddr+10)) < 32)
	    strcpy(password, paddr+10);  /* <input type="text" name="password" value=""><br> */
	
    /* <input type="text" name="access" value="$%access#$"><br> ,access is a windmark*/
	g_access = atoi(paddr1+8);
	printf("\r\n username:(%s). password:(%s). access:(%d).\r\n",username,password,g_access);
	
	/* simple authentication */
	if(strcmp(username, "admin")|| strcmp(password, "admin"))
		{
		/* fail,return original page */
		reqId->sMethod = HTTP_M_GET;
		(void) httpRpmWmbParse(reqId);
		}
	else
		{/* success,return index.html*/
		reqId->sMethod = HTTP_M_GET;
		reqId->szRpmData = "index.html";
		(void) httpRpmWmbParse(reqId);		
		}
	(void)memPoolFree(mpPool, pEntity);
	
	return (RPM_DONE);
	
}
#endif

