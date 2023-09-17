/* sampleDevice.h - sample data implementation for tutorial */

/* Copyright (c) 2004-2012 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,09aug12,r_w  create file
*/

/*
DESCRIPTION
each of the following is the underlying device
variable represented by a WindMark. These declarations
and the corresponding initializations in sampleDevice.c
represent the target device data you wish to manage.
*/


#ifndef __INCsampleDeviceh
#define __INCsampleDeviceh


#define REMOTE_TABLE_SIZE   8

extern sbyte   x_sysName[32];
extern sbyte   x_sysContact[32];
extern sbyte   x_sysDescr[32];
extern sbyte4  x_referenceInteger;
extern ubyte4  x_rem_addr[REMOTE_TABLE_SIZE];
extern ubyte4  x_rem_port[REMOTE_TABLE_SIZE];
extern sbyte   showHelp[8];
extern sbyte4   g_access;




typedef struct portInfo
    {
    ubyte4  ip;
    ubyte   mac[6];
    }
    portInfo;


/* SMTP variable declarations.These variables are represented by
   WindMarks used in the SMTP example.
*/

#define MAX_WINDMARK_LENGTH     256 /* Maximum length of any WindMark Value */


extern sbyte   x_domain[MAX_WINDMARK_LENGTH];          /* 'SMTP domain' name */
extern sbyte   x_emailTextInfo[MAX_WINDMARK_LENGTH]; /* 'Email Text Info */
extern sbyte   x_mailServer[MAX_WINDMARK_LENGTH];/* SMTP Mail Server address */
extern sbyte   x_subjectHeading[MAX_WINDMARK_LENGTH];/* mail subject heading */
extern sbyte   x_sendNow[MAX_WINDMARK_LENGTH];   /* checkBox to send email */
extern sbyte   x_ERROR[MAX_WINDMARK_LENGTH];   /* ERROR windMark */

extern sbyte   x_emailBody[MAX_WINDMARK_LENGTH];  /* email body contents */
extern sbyte   x_mailFrom[MAX_WINDMARK_LENGTH];   /* 'mail from' Address */
extern sbyte   x_mailTo[MAX_WINDMARK_LENGTH];   /* 'mail to' address */

extern sbyte   x_status[MAX_WINDMARK_LENGTH];   /* Mail Delivery status */
extern sbyte   x_fileInclude[MAX_WINDMARK_LENGTH]; /* FileInclude windmark Handler */

#endif /* __INCsampleDeviceh */



/* eof sampleDevice.h */
