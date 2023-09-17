/* printenv.c - prints the complete set of server environment vars */

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
05nov12, r_w  fix output '%' issue
12mar12, r_w  add puts() demo and add return Link
02jun97, mb   adapted to new CGI concept
02oct96, mb   initial creation
*/


/* includes */
#include "stdio.h"
#include "wmw_httpconf.h"
#include "httpLib.h"

#ifdef INCLUDE_HTTP_CGI
static void outputString(char * pChar)
{
	char   tempBuf[256];
	char   *ptempBuf = tempBuf;
	
	while('\0' != *pChar)
	    {
		if ('%' == *pChar)
			*ptempBuf++ = *pChar;	
		*ptempBuf++ = *pChar++;
	    }
	*ptempBuf = '\0';
	printf(tempBuf);
	
}

short cgiPrintEnv ( long argc,char ** argv,HTTP_REQ_ID reqId )
    {
    char   tempBuf[256];
    
    /* write the HTTP status line and type information to the network */
    printf ("%s 200 OK\r\n", httpGetEnv(reqId,"SERVER_PROTOCOL"));
   
    /* printf output non-format string demo */
    printf("Transfer-Encoding: chunked\r\n");
    
    /* puts() will appends a new-line character(\n) to the output */
    (void) puts("Content-Type: text/html\r\n\r");
    
    sprintf (tempBuf, "<HTML><HEAD><TITLE>CGI Test</TITLE></HEAD><BODY><H1>This is a CGI test</H1>\n");
    printf("%x\r\n",strlen(tempBuf));
	outputString(tempBuf);

    sprintf (tempBuf, "Environment:<P>\n<HR>" );
    printf("\r\n%x\r\n",strlen(tempBuf));
	outputString(tempBuf);
	
    sprintf (tempBuf, "<LI>REQUEST_METHOD=%s<LI>QUERY_STRING=%s\n<LI>PATH_INFO=%s\n<HR>",
	     httpGetEnv(reqId,"REQUEST_METHOD"),
	     httpGetEnv(reqId,"QUERY_STRING"),
	     httpGetEnv(reqId,"PATH_INFO"));
    printf("\r\n%x\r\n",strlen(tempBuf));
	outputString(tempBuf);
    
    sprintf ( tempBuf, "<LI>SERVER_SOFTWARE=%s<LI>SERVER_NAME=%s<LI>SERVER_PROTOCOL=%s<LI>SERVER_PORT=%s<HR>\n",
	     httpGetEnv(reqId,"SERVER_SOFTWARE"),
	     httpGetEnv(reqId,"SERVER_NAME"),
	     httpGetEnv(reqId,"SERVER_PROTOCOL"),
	     httpGetEnv(reqId,"SERVER_PORT"));
    printf("\r\n%x\r\n",strlen(tempBuf));
    outputString(tempBuf);
    
    sprintf ( tempBuf, "<LI>GATEWAY_INTERFACE=%s<LI>SCRIPT_NAME=%s<HR>\n",
	     httpGetEnv(reqId,"GATEWAY_INTERFACE"),
	     httpGetEnv(reqId,"SCRIPT_NAME"));
    printf("\r\n%x\r\n",strlen(tempBuf));
    outputString(tempBuf);
    
    sprintf ( tempBuf, "<LI>REMOTE_HOST=%s<LI>REMOTE_ADDR=%s<LI>AUTH_TYPE=%s<LI>REMOTE_USER=%s<LI>REMOTE_IDENT=%s<HR>\n",
	     httpGetEnv(reqId,"REMOTE_HOST"),
	     httpGetEnv(reqId,"REMOTE_ADDR"),
	     httpGetEnv(reqId,"AUTH_TYPE"),
	     httpGetEnv(reqId,"REMOTE_USR"),
	     httpGetEnv(reqId,"REMOTE_IDENT"));
    printf("\r\n%x\r\n",strlen(tempBuf));
    outputString(tempBuf);
    
    sprintf ( tempBuf, "<LI>CONTENT_TYPE=%s<LI>CONTENT_LENGTH=%s<LI>CONTENT_ENCODING=%s<LI>HTTP_ACCEPT=%s<HR>\n",
	     httpGetEnv(reqId,"CONTENT_TYPE"),
	     httpGetEnv(reqId,"CONTENT_LENGTH"),
	     httpGetEnv(reqId,"CONTENT_ENCODING"),
	     httpGetEnv(reqId,"HTTP_ACCEPT"));
    printf("\r\n%x\r\n",strlen(tempBuf));
    outputString(tempBuf);
    
    sprintf (tempBuf, "<p>Return to <a href=\"../fs/index.html\">Tutorial Index</a>.</p>");
    printf("\r\n%x\r\n",strlen(tempBuf));
    outputString(tempBuf);
       
    /* puts() will appends a new-line character(\n) to the output */
    (void) puts("\r\n0\r\n\r");
    
    return (0);
    }
#endif /* INCLUDE_HTTP_CGI */
