/* httpRpmAuth.c - shows your authentication and grading for debugging */

/*
 * Copyright (c) 1996-2008,2010,2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
26sep14,r_w  fix static CHECKED_RETURN issue.
2feb10,h_y   support for LP64 
04mar09,m_z  change GID content display code
                 (WIND00157654)
25jun08,q_z  deleted an old tutorial reference.(WIND00032220)
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
15may97,mb   osal introduced.
10oct96,mb   documentation extended.
26sep96,mb   initial creation.
*/

/*
DESCRIPTION
This RPM is intended to verify and debug problems with access control.
Please be aware of some security relevant facts if you display
authorization information with this RPM:
    - the authorization string is base64 encoded - but this encryption
      is NOT secure, everyone is able to decode this authorization string
      and will know the username and password!
    - proxys could store this information containing your authorization

Besides this please note that some browsers remember the URLs for which a
server requested authorization and do not resend the authorization if the
user requests some completely other URLs.

INCLUDE FILES: http/httpLib.h
               http/httpRpms.h

NOMANUAL
*/


#include "httpLib.h"
#include "httpRpms.h"
#include "httpCfg.h"

#ifdef INCLUDE_HTTP_SHOWAUTH
/*******************************************************************************
*
* httpRpmAuth - display user authentication information for debugging
*
* This is the authentication RPM routine. It is
* is useful for identifying and debugging problems with access control.
*
* NOTE
* Be aware of the following items related to security, if you display
* authorization information with this RPM:
* the authorization string is base64 encoded, but this encryption
* is not secure, so anyone could decode this authorization string
* and will know the username and password; and
* proxies could store this information containing the authorization.
*
* In addition, note that some browsers store the URLs for which a
* server requests authorization, and do not resend the authorization if the
* user requests other URLs.
*
* RETURNS: RPM_DONE if output was successful, RPM_ERROR otherwise.
*/

short httpRpmAuth
    (
    HTTP_REQ_ID reqId        /* Handle of the active request */
    )
    {
    char * szTmp;
    HTTP_GEN_LIST_ENTRY_ID gidEntry = NULL; 

    httpStatusSet (reqId, HTTP_OK);       /* Set the HTTP status code     */
    (void) httpHeaderGenerate (reqId);           /* Send HTTP header             */

    (void) httpStringPut (reqId,"<HTML><TITLE>Access Verification</TITLE><BODY><CENTER><H1>ACCESS VERIFICATION</H1></CENTER>\n");
    (void)httpPrintf (reqId,"<LI>Your authorization (base64 encoded): <B><I>%s</B></I>\n<LI>Location: <B><I>%s</B></I>\n",
        httpAuthorizationGet (reqId,HDR_IN),
        httpLocationGet (reqId));
        while (NULL != httpGidEntryGet(reqId, &gidEntry))
            {
            szTmp = httpGidEntryDataGet(reqId, gidEntry);
            (void)httpPrintf (reqId,"<LI>GID: <B><I>%s</B></I>\n", szTmp);
            }
        (void)httpPrintf (reqId,"<p>");
	

    (void) httpStringPut (reqId,"<HR><H3>NOTE:</H3>Don\'t worry if the authorization line is empty and the GID is &quot;(null)&quot;. "
           "This could happen if you didn\'t invoke this RPM inside a protected URL tree. "
           "Some browsers like Netscape remember for which URL subtrees a server requested authentication "
           "and do NOT send an authorization string if they assume that this URL is unprotected.<P>\n");
    (void) httpStringPut (reqId,"</BODY>\n");
    return (RPM_DONE);
    }
#endif

