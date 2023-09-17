/* sampleSmtpApp.c - Sample application to initiate Email transfer */

/*
 * Copyright (c) 1984-2003, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,29aug12,r_w  standardization
*/

/*
DESCRIPTION
Routine 'smtpTest' available in this file can be used to send the email to
specified recepient through given mail server. Macros given below are passed
as required arguments to SMTP MTA. Resource file to be sent can be passed as
argument to 'smtpTest'. If it is null, it uses default resource file.

MAIL_SERVER     - SMTP MAIL Server
DOMAIN_NAME     - Server Domain name
MAIL_FROM       - Mail From Address
NUM_RECEPIENTS  - Number of Recipients
RCPT_TO         - RCPT TO, address
RESOURCE        - Resource to mail

*/

/* includes */

#include "vxWorks.h"
#include <string.h>
#include "wmw_httpconf.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER

#ifdef WEBCLI_ON_LINUX
# include "wm.h"
# include "wmos.h"
#else
# include "taskLib.h"
#endif

#include "wm_options.h"
#include "smtp/smtpLib.h"
#include "sampleDevice.h"

#ifdef WEBCLI_INCLUDE_SMTP
/* define */

#define GATEWAY_ADDR    "128.224.162.1"     /* Gateway to reach SMTP Server */
#define MAIL_SERVER     "147.11.1.11"      /* SMTP MAIL Server */
#define DOMAIN_NAME     "wrs.com"           /* Server Domain name */
#define MAIL_FROM       "phil.servita@windriver.com" /* Mail From Address */
#define NUM_RECEPIENTS  1                   /* Number of Recipients */
#define RCPT_TO         "ruiqiang.wu@windriver.com"/* RCPT TO, address */
                                            /* Resource to mail */
#ifdef WEBCLI_ON_LINUX
#define RESOURCE        "/romfs/htdocs/emailalert.txt"
#else
#define RESOURCE        "/romfs/htdocs/emailalert.txt"
#endif

#define NUM_TASKS       7          /* Maximum number of test routine tasks */
#define TEST_TASK_PRI   101        /* Priority of each test task */
#define TEST_TASK_STACK 10000      /* Stack size for each test task */

#define ERR_VAL        "#ERROR"
#define CHECK_WM(X)    strstr ((X), ERR_VAL)
#define CHECK_OFF(X)    strstr ((X), "OFF")


/* Forward declarations */

/* test program to Initiates Mail transfer to test SMTP MTA. */
STATUS smtpTest (char *resource, char *mailServer, char *domainName,
                                 char *mailFrom, char *sendTo);
/* spawns multiple tasks to test SMTP MTA */
STATUS spawnTests (int numTasks, char *resource, char *mailServer,
                          char *domainName, char *mailFrom, char *sendTo);
/* collect email data and invoke SMTP_SendMail */
STATUS smtpSendAlert(char *resource);

/*******************************************************************************
*
* smtpTest - Initiates Mail transfer to test SMTP MTA.
*
* This routine initiates the SMTP mail transfer by invoking SMTP_SendMail with
* required arguments.
*
* RETURNS: OK, or ERROR if failed to send the Mail.
*/


STATUS smtpTest
    (
    char *resource,     /* Resource to mail */
    char *mailServer,   /* SMTP MAIL Server */
    char *domainName,   /* Server Domain name */
    char *mailFrom,     /* Mail From Address */
    char *sendTo        /* RCPT TO, address */
    )
    {
    Smtp_MailData pMailData;

    /* create SMTP Mail data structure */
    if (mailServer == NULL)
        strcpy(pMailData.mailServer, MAIL_SERVER);
    else
        {
        if (strlen(mailServer) < kNumDomainChars)        
            strcpy(pMailData.mailServer, mailServer);
        }

    if (domainName == NULL)
        strcpy(pMailData.domainName, DOMAIN_NAME);
    else
        {
        if (strlen(domainName) < kNumDomainChars)   
            strcpy(pMailData.domainName, domainName);
        }

    if (mailFrom == NULL)
        strcpy(pMailData.mailFrom, MAIL_FROM);
    else
        {
        if (strlen(mailFrom) < kNumMailChars)
            strcpy(pMailData.mailFrom, mailFrom);
        }

    pMailData.numRecipients = NUM_RECEPIENTS;

    if (sendTo == NULL)
        strcpy(pMailData.rcptTo[0], RCPT_TO);
    else
        {
        if (strlen(sendTo) < kNumMailChars)        
            strcpy(pMailData.rcptTo[0], sendTo);
        }

    if (resource == NULL)
        strcpy(pMailData.ResourceToMail, RESOURCE);
    else
        {
        if (strlen(resource) < kNumResourceChars)
            strcpy(pMailData.ResourceToMail, resource);
        }

    /* update Mail Data WindMarks */

    strcpy(x_mailFrom, pMailData.mailFrom);
    strcpy(x_mailTo, pMailData.rcptTo[0]);
    strcpy(x_domain, pMailData.domainName);
    strcpy(x_mailServer, pMailData.mailServer);

    /* update duplicate WindMarks too */
/*
    strcpy(x_mailFrom, x_mailfrom);
    strcpy(x_mailTo, x_mailto);
*/
    /* trigger SMTP Mail alert */
    return (SMTP_SendMail (&pMailData));
    }

#ifndef WEBCLI_ON_LINUX

/*******************************************************************************
*
* spawnTests - spawns multiple tasks of 'smtpTest' to test SMTP MTA.
*
* This routine spawns multiple tasks with entry point 'smtpTest', to test the
* SMTP FSM for re-entrancy of code.
*
* RETURNS: OK, or ERROR if failed to spawn tasks
*/

STATUS spawnTests
    (
    int numTasks,       /* Maximum number of test routine tasks */
    char *resource,     /* Resource to mail */
    char *mailServer,   /* SMTP MAIL Server */
    char *domainName,   /* Server Domain name */
    char *mailFrom,     /* Mail From Address */
    char *sendTo        /* RCPT TO, address */
    )
    {
    int i;
    char tName[20];
    OS_THREAD_T tid;

    if (!numTasks) numTasks = NUM_TASKS;

    if (numTasks > NUM_TASKS)
        numTasks = NUM_TASKS;

    for (i=0; i<numTasks; i++)  {
        sprintf(tName, "test%d", ((i+1) % (NUM_TASKS + 1)));
        tid = taskSpawn (tName, TEST_TASK_PRI, VX_PRIVATE_ENV, TEST_TASK_STACK,
                  smtpTest, (size_t)resource, (size_t)mailServer, (size_t)domainName,
       (size_t)mailFrom, (size_t)sendTo, 6, 7, 8, 9, 10);
#ifndef _WRS_CONFIG_LP64
        if (ERROR == (STATUS) tid)
#else
        if (TASK_ID_ERROR == tid)
#endif
            return ERROR;
    }
    return OK;
}

#endif /* WEBCLI_ON_LINUX */


/*******************************************************************************
*
* smtpSendAlert - Collect email data from WindMarks and invoke SMTP_SendMail
*
* This routine extracts the Mailing information from different configured
* WindMarks and invokes the SMTP_SendMail, to send the email alert.
*
* RETURNS: OK, or ERROR if failed to send emailalert.
*/


STATUS smtpSendAlert
    (
    char *resource        /* path of emailalert resource file */
    )
    {
    Smtp_MailData pMailData;
    int numRecepients = 0, len = 0, i;
    char *ptr;

    /* Initialise the SMTP Mail Data using the WindMark values */
    if (strlen(x_mailServer) < kNumDomainChars)
        strcpy(pMailData.mailServer, x_mailServer);
    
    if (strlen(x_domain) < kNumDomainChars)
        strcpy(pMailData.domainName, x_domain);

    if (strlen(x_mailFrom) < kNumMailChars)
        strcpy(pMailData.mailFrom, x_mailFrom);

    /* validate  current WindMark values */
    if (CHECK_WM(x_mailFrom) || CHECK_OFF(x_sendNow) || CHECK_WM(x_mailTo) ||
        CHECK_WM(x_domain) || CHECK_WM(x_emailTextInfo) ||
        CHECK_WM(x_mailServer) || CHECK_WM(x_subjectHeading))
            return ERROR;

    /* Extracting Address List */
    ptr = x_mailTo;

    /* Skip Initial commas */
    while ((len++ < MAX_WINDMARK_LENGTH) && (*ptr == ',') && (*ptr != '\0'))
        ptr++;
    len = 0;

    while (len < MAX_WINDMARK_LENGTH)
    {
        int ix = 0;
        numRecepients++;
        while ((*ptr != '\0') && (*ptr != ',') && (len++ < MAX_WINDMARK_LENGTH))
            pMailData.rcptTo[numRecepients - 1][ix++] = *ptr++;

        pMailData.rcptTo[numRecepients - 1][ix] = '\0';
        if (*ptr == '\0') break;
        while ((len++ < MAX_WINDMARK_LENGTH) && (*ptr == ',') && (*ptr != '\0'))
            ptr++;
        if (*ptr == '\0') break;
    }
    pMailData.numRecipients = (sbyte)numRecepients;

    for (i=0; i < numRecepients; i++)
        if ((strstr(pMailData.rcptTo[i], "@") == NULL) ||
                    (strstr(pMailData.rcptTo[i], ".") == NULL))

        {
            strcpy(x_mailTo, "#ERROR - Invalid Mail address List");
            return ERROR;
        }

    if (resource == NULL)
        strcpy(pMailData.ResourceToMail, RESOURCE);
    else
        {
        if (strlen(resource) < kNumResourceChars)
            strcpy(pMailData.ResourceToMail, resource);
        }

    /* UpDate the duplicate WindMarks */
/*    strcpy(x_emailBody, x_emailTextInfo);
    strcpy(x_subjectHeading, x_subjectheading);
    strcpy(x_mailFrom, x_mailfrom);
    strcpy(x_mailTo, x_mailto);
*/
    /* Trigger SMTP Email Alert */
    return (SMTP_SendMail (&pMailData));
    }
#endif
#endif

