/* sampleUserCgi.h - sample user defined CGI program prototypes */

/*
 * Copyright (c) 1996/97, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,29aug12, r-w   standardization
01b,18may04, rws   renamed from user_cgi.h to sampleUserCgi.h
01a,03jun97, mb    initial creation
*/

/*
INCLUDE FILES: httpLib.h
*/


/* includes */
#include "httpLib.h"


/* CGI program prototypes */
short cgiPrintEnv ( long argc,char **argv,HTTP_REQ_ID reqId );

