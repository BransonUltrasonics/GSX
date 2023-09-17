/* sampleDevice.c - sample data implementation for tutorial */

/* Copyright (c) 2004, 2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06nov14,r_w  change /tgsvr to /hostfs
14mar04,rws  updated for 4.3 tutorial changes
*/

/*
DESCRIPTION
This file contains the local tutorial variables associated with
each windMark. Initial values of each WindMark is set here.
*/


/* includes */
#include "wmw_httpconf.h"
#include "wm.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"
#endif
#include "sampleDevice.h"

/* defines */

/* typedefs */

/* globals */


/* Setting initial values of example non-snmp tutorial WindMarks */
sbyte   x_sysDescr[32] = "Embedded Target";
sbyte   x_sysName[32] = "Wind River Tutorial System";
sbyte   x_sysContact[32] = "Wind River Support";
sbyte4  x_referenceInteger = 1525;
sbyte   showHelp[8] = "no";
sbyte4  g_access = 0;



/* Setting initial values of SMTP tutorial WindMarks */
/* These windMarks are associated with following HTML forms:
1. emailalert.txt
2. smtpVerification.htm
3. smtpMailSend.htm
*/


 /* domain Name */
sbyte   x_domain[MAX_WINDMARK_LENGTH] = "Configurable domain name";

/* Email body */
sbyte   x_emailTextInfo[MAX_WINDMARK_LENGTH] = "Default Email contents";

/* Tagus - SMTP Email Server */
sbyte   x_mailServer[MAX_WINDMARK_LENGTH] = "SMTP server Address";

/* SendNow windMark */
extern char   x_sendNow[MAX_WINDMARK_LENGTH];

/* error WindMark */
sbyte   x_ERROR[MAX_WINDMARK_LENGTH] = "0";

/* Email body */
sbyte   x_emailBody[MAX_WINDMARK_LENGTH] = "Email contents";

/* mail from windMark */
sbyte   x_mailFrom[MAX_WINDMARK_LENGTH] = "sender@domain.com";

/* mail from windMark */
sbyte   x_mailTo[MAX_WINDMARK_LENGTH] = "recipient@domain.com";

/* subject heading */
sbyte   x_subjectHeading[MAX_WINDMARK_LENGTH] = "SMTP Test";

/* Mail Delivery status */
sbyte x_status[MAX_WINDMARK_LENGTH] = "OK";

/* FileInclude windmark Handler */
sbyte x_fileInclude[MAX_WINDMARK_LENGTH] = "/hostfs/ws/firstFile.html";

/* eof sampleDevice.c */
