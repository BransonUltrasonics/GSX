/* smtpLib.c - SMTP MTA finite state machine */

/*
 * Copyright (c) 2003-2008, 2010, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
05mar14,r_w  disable SMTP feature in webserver standalone mode. defect VXW6-80639 and VXW6-1806
09jul10,m_z  remove compile warning
17jul08,q_z  clear the warning of gnu compiler
23feb07,msa  Correct the apigen comments so they match the actual
             routines.
16jan04,jws  remove per project dependencies
13oct03,jws  fix SMTP_SendMail() error returns, SPR 91107
08oct03,jws  fix SPR's 91005, 91006, 91032 (pretty much duplicates)
             also, many coding standards and misc fixes
15aug03,adb  wm_options.h instead of rc_options.h
14aug03,adb  added to source control
*/

/*
DESCRIPTION
This file contains the set of APIs for SMTP mail transfer agent. SMTP client
works on a simple finite state machine. The API SMTP_SendMail() is invoked
with required parameters to send an email alert. SMTP establishes a TCP
connection with given SMTP server and sends the email data over it.

INCLUDE FILES: smtpLib.h

SEE ALSO: httpFileProcess.c
*/

/*
 *  This was a part of the OpenControl SDK source code library.
 *
 *  Copyright (C) 1998 Rapid Logic, Inc.
 *  All rights reserved.
 *
 *
 * *****************  Version 13  *****************
 * User: Pstuart      Date: 2/23/01    Time: 5:11p
 * Updated in $/Rapid Logic/Code Line/rli_code/wcontrol
 * use macro for env access, use ints for array indices
 *
 * *****************  Version 12  *****************
 * User: Epeterson    Date: 4/25/00    Time: 5:29p
 * Updated in $/Rapid Logic/Code Line/rli_code/wcontrol
 * Include history and enable auto archiving feature from VSS
*/

/* includes */

#ifndef COMMON_IS_COMPONENT
#include "commonCfg.h"
#endif
#include "httpCfg.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER

#include "httpFileProcess.h"
#include "smtp/smtpLib.h"

#ifdef __SNMP_API_ENABLED__
#include "rcm_mibway.h"
#endif

#ifdef WEBCLI_INCLUDE_SMTP
/* defines */

/* Different states in SMTP MTA FSM */

enum smtpState
    {
    kSMTP_OPEN = 0,
    kSENT_HELO,
    kSENT_MAIL,
    kSENT_RCPT,
    kSENT_LAST_RCPT,
    kSENT_DATA,
    kXMITTED_DATA,
    kSENT_QUIT,
    kFINISHED,
    kABORT
    };

/* Possible events in SMTP MTA FSM */

enum smtpEvent
    {
    kOpen = 0,
    k100 = 0,
    k200,
    k300,
    k400,
    k500,
    kBadEvent
    };

#define kMaxRespBuffSize    256    /* Max size of buffer to collect response */
#define kSmtpPort            25    /* SMTP port number */
#define kNumStates            8    /* no. of states in SMTP FSM */
#define kNumEvents            6    /* no. of events in SMTP FSM */


/* typedefs */

/* structure for keeping track of mail session */

typedef struct
    {
    WMB_SESSION_T * pEnv;

    enum smtpState  currState;
    enum smtpEvent  currEvent;

    ubyte           rcptToCount;
    ubyte           retryCount;

    BOOL            isValidRecipient;
    BOOL            runAutomaton;
    } MAILCONTROLBLOCK;


typedef struct Smtp_StateEntry
    {
    STATUS (*p_funcAction)(MAILCONTROLBLOCK *  pMCB,
                           Smtp_MailData *     pMailData);

    void (*p_funcNextState)(MAILCONTROLBLOCK * pMCB,
                            Smtp_MailData *    pMailData);
    } Smtp_StateEntry;



LOCAL Smtp_StateEntry *    mSmtpStateTable[kNumStates];

LOCAL BOOL                 mSmtpInitialized = FALSE;

/* forward declarations */

LOCAL STATUS SMTP_CleanupMailControlBlock(MAILCONTROLBLOCK * pMCB);


/***************************************************************************
*
* SMTP_SendSimpleMessage - send message to SMTP server.
*
* This routine sends the messages in <pMesg> to the SMTP server through socket
* handle <sock>
*
* RETURNS: OK, or ERROR if failed to send the message.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendSimpleMessage
    (
    OS_SOCKET_T sock,    /* SMTP server socket */
    char * pMesg         /* SMTP message */
    )
    {
    STATUS    status;

    status = wmnetTcpWrite(sock, pMesg, strlen(pMesg));

    if (OK == status)
        {
        status = wmnetTcpWrite(sock, kCRLF, kCRLFSize);
        }

    return status;
    }


/***************************************************************************
*
* SMTP_SendMessageWithParams - send message with parameters to SMTP server.
*
* This routine sends the messages in <pMesg> followed by parameters <pParam>
* to the SMTP server through socket handle <sock>
*
* RETURNS: OK, or ERROR if failed to send the message.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendMessageWithParams
    (
    OS_SOCKET_T sock,   /* socket fd for TCP connection with SMTP Server */
    char *      pMesg,  /* SMTP Message */
    char *      pParam  /* parameters to the message */
    )
    {
    STATUS    status;

    status = wmnetTcpWrite(sock, pMesg, strlen(pMesg));

    if (status == OK)
        {
        status = wmnetTcpWrite(sock, pParam, strlen(pParam));

        if (status == OK)
            {
            status = wmnetTcpWrite(sock, kCRLF, kCRLFSize);
            }
        }

    return status;
    }



/***************************************************************************
*
* SMTP_SendHelo - send SMTP 'HELO:' command to SMTP server.
*
* This routine sends the SMTP 'HELO:' command with argument 'domainName'
* to the SMTP server.
*
* RETURNS: OK, or ERROR if failed to send the command.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendHelo
    (
    MAILCONTROLBLOCK * pMCB,     /* SMTP Mail control block struct */
    Smtp_MailData *         pMailData /* SMTP Mail data structure */
    )
    {
    return SMTP_SendMessageWithParams( pMCB->pEnv->sock,
                                       "HELO ", pMailData->domainName );
    }



/***************************************************************************
*
* SMTP_SendMailFrom - send SMTP 'MAIL FROM:' command to SMTP server.
*
* This routine sends the SMTP 'MAIL FROM:' command with argument 'MailFrom
* email address' to the SMTP server.
*
* RETURNS: OK, or ERROR if failed to send the command.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendMailFrom
    (
    MAILCONTROLBLOCK  *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData *pMailData        /* SMTP Mail data structure */
    )
    {
    return SMTP_SendMessageWithParams( pMCB->pEnv->sock,
                                   "MAIL FROM: ", pMailData->mailFrom );
    }



/***************************************************************************
*
* SMTP_SendRcptTo - send SMTP 'RCPT TO:' command to SMTP server.
*
* This routine sends the SMTP 'RCPT TO:' command  to the SMTP server, with
* 'recipient email address' as argument.
*
* RETURNS: OK, or ERROR if failed to send the command.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendRcptTo
    (
    MAILCONTROLBLOCK   *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData *pMailData         /* SMTP Mail data structure */
    )
    {
    char *      pRecipient;

    pRecipient = pMailData->rcptTo[pMCB->rcptToCount++];

    return SMTP_SendMessageWithParams(  pMCB->pEnv->sock,
                                        "RCPT TO: ", pRecipient );
    }


/***************************************************************************
*
* SMTP_SendData - send message through 'DATA' command to SMTP server.
*
* This routine sends the message contents through 'DATA' command to SMTP
* server
*
* RETURNS: OK, or ERROR if failed to send the message.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendData
    (
    MAILCONTROLBLOCK   *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData *pMailData         /* SMTP Mail data structure */
    )
    {
    return SMTP_SendSimpleMessage( pMCB->pEnv->sock, "DATA");
    }



/***************************************************************************
*
* SMTP_ValidateAndSendData - Validates recipient and send 'DATA' out to Server.
*
* This routine first validates the recipient and if recipient is valid, then
* it sends out the 'DATA' to SMTP Server.
*
* RETURNS: OK, or ERROR if recipient is invalid or fail to send the 'DATA'
*
* NOMANUAL
*/

LOCAL STATUS SMTP_ValidateAndSendData
    (
    MAILCONTROLBLOCK   *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData *pMailData         /* SMTP Mail data structure */
    )
    {
    /*
     * If there is a valid recipient, then send "DATA" out.  Otherwise,
     * close down the Automaton--there's no reason for continuing.
     */

    if ( pMCB->isValidRecipient )
        {
        return SMTP_SendData(pMCB, pMailData);
        }
    else
        {
        pMCB->runAutomaton = FALSE;
        return OK;
        }
    }


/***************************************************************************
*
* SMTP_SendResource - Send SMTP resource file contents to SMTP server.
*
* This routine invokes the calls to retrieve the resource file <Resource> and
* Parse its conents for WindMarks and then send the contents to SMTP server
* over the TCP socket <sock>
*
* RETURNS: OK, or ERROR if failed to send the resource file contents.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendResource
    (
    OS_SOCKET_T sock,         /* sock fd for TCP connection with SMTP Server */
    WMB_SESSION_T *p_envVar,  /* SMTP environment session structure */
    char *Resource            /* path of resource file */
    )
    {
    generic_fd_t  f;

    if (OK != httpFileOpen(Resource, "rb", &f))
        return wmError(ERROR_GENERAL_NO_DATA,NULL);

    /* send out the resource file */

    (void)fileSend(p_envVar, &f);

    /* close the resource file */

    (void) httpFileClose(&f);

    return OK;
    }


/***************************************************************************
*
* SMTP_XmitData - Sends the resource file and terminates the SMTP session.
*
* This routine invokes 'SMTP_SendResource' API to send the parsed resource data
* followed by <CRLF><dot><CRLF> characters to indicate end of message.
*
* RETURNS: OK, or ERROR if failed to deliver the message to server.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_XmitData
    (
    MAILCONTROLBLOCK   *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData *pMailData         /* SMTP Mail data structure */
    )
    {
    STATUS    status;

    status = SMTP_SendResource( pMCB->pEnv->sock,
                                pMCB->pEnv, pMailData->ResourceToMail );
    if (status != OK)
        return status;

    /* Close out connection by sending <CRLF>.<CRLF> */

    status = SMTP_SendSimpleMessage( pMCB->pEnv->sock, "\x0D\x0A." );

    return status;
    }



/***************************************************************************
*
* SMTP_SendQuit - Send 'QUIT' command to server to terminate SMTP session.
*
* This routine sends out 'QUIT' command to SMTP server.
*
* RETURNS: OK, or ERROR if failed to send out the command.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_SendQuit
    (
    MAILCONTROLBLOCK  *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData *pMailData        /* SMTP Mail data structure */
    )
    {
    return SMTP_SendSimpleMessage( pMCB->pEnv->sock, "QUIT");
    }


/***************************************************************************
*
* SMTP_AbortConnection - Mark connection aborted for this session.
*
* This routine marks the connection abort for the SMTP session.
*
* RETURNS: ERROR_SMTP_ABORT.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_AbortConnection
    (
    MAILCONTROLBLOCK *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData *pMailData       /* SMTP Mail data structure */
    )
    {
    pMCB->runAutomaton = FALSE;
    return wmError(ERROR_SMTP_ABORT,NULL);
    }


/***************************************************************************
*
* SMTP_CloseDown - Mark connection close down for this session.
*
* This routine marks the connection 'closed' for the SMTP session.
*
* RETURNS: OK.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_CloseDown
    (
    MAILCONTROLBLOCK   *pMCB,   /* SMTP Mail control block structure */
    Smtp_MailData  *pMailData        /* SMTP Mail data structure */
    )
    {
    pMCB->runAutomaton = FALSE;
    return OK;
    }


/***************************************************************************
*
* SMTP_AbortNextState - Moves SMTP FSM to Connection 'ABORT' state.
*
* This routine moves the SMTP FSM to connection 'ABORT' state. It sets the
* current state to 'ABORT' state.
*
*
* NOMANUAL
*/

LOCAL void SMTP_AbortNextState
    (
    MAILCONTROLBLOCK   *pMCB,        /* control block structure */
    Smtp_MailData           *pMailData    /* data structure */
    )
    {
    pMCB->currState = kABORT;
    return;
    }



/***************************************************************************
*
* SMTP_FinishedNextState - Moves SMTP FSM to Connection 'FINISHED' state.
*
* This routine moves the SMTP FSM to connection 'FINISHED' state. It sets the
* current state to 'FINISHED' state.
*
*
* NOMANUAL
*/

LOCAL void SMTP_FinishedNextState
    (
    MAILCONTROLBLOCK   *pMCB,         /* control block structure */
    Smtp_MailData           *pMailData     /* data structure */
    )
    {
    pMCB->currState = kFINISHED;
    return;
    }



/***************************************************************************
*
* SMTP_RcptToNextState - Moves SMTP FSM to 'SENT_RCPT' or 'SENT_LAST_RCPT' state
*
* This routine moves the SMTP FSM to 'SENT_RCPT' or 'SENT_LAST_RCPT' state
* based on number of recipients being sent through 'RCPT FROM:' command.
* State is set to 'SENT_LAST_RCPT' after last recipient address is sent.
*
*
* NOMANUAL
*/

LOCAL void SMTP_RcptToNextState
    (
    MAILCONTROLBLOCK   *pMCB,       /* control block structure */
    Smtp_MailData           *pMailData   /* data structure */
    )
    {
    enum smtpState  nextState;

    nextState       = ( pMCB->rcptToCount < pMailData->numRecipients ) ?
                                            kSENT_RCPT : kSENT_LAST_RCPT;
    pMCB->currState = nextState;
    return;
    }



/***************************************************************************
*
* SMTP_StandardNextState - Moves SMTP FSM to next standart state.
*
* This routine moves the SMTP FSM to next standart state. It just increments
* the current state to point to next state.
*
*
* NOMANUAL
*/

LOCAL void SMTP_StandardNextState
    (
    MAILCONTROLBLOCK   *pMCB,        /* control block structure */
    Smtp_MailData           *pMailData    /* data structure */
    )
    {
    (pMCB->currState)++;
    return;
    }



/***************************************************************************
*
* SMTP_ValidateAndSendDataNextState - Moves SMTP FSM to next state from SMTP_ValidateAndSendData.
*
* This routine is invoked for 'SMTP_ValidateAndSendData'. If recipient is valid
* this routine marks the FSM to 'SENT_DATA' state else it moves the connection
* to 'ABORT' state.
*
*
* NOMANUAL
*/

LOCAL void SMTP_ValidateAndSendDataNextState
    (
    MAILCONTROLBLOCK   *pMCB,       /* control block structure */
    Smtp_MailData           *pMailData   /* data structure */
    )
    {
    if (pMCB->isValidRecipient)
        {
        pMCB->currState = kSENT_DATA;
        }
    else
        {
        pMCB->currState = kABORT;
        }
    }



/***************************************************************************
*
* SMTP_GetEvent - Retrieves the 'event' out of SMTP Server's response.
*
* This routine checks the first character in SMTP Server's response packet
* and extracts the specific event out of it to initialize it as current event.
*
*
* NOMANUAL
*/

LOCAL void SMTP_GetEvent
    (
    sbyte *pRespBuff,              /* buffer to receive the event */
    Length numRead,                /* number of bytes to read */
    MAILCONTROLBLOCK  *pMCB   /* SMTP Mail control block structure */
    )
    {
    sbyte           code;
    enum smtpEvent  event;

    code = pRespBuff[0];

    switch (code)
        {
        case '1':
            event = k100;
            break;
        case '2':
            event = k200;
            break;
        case '3':
            event = k300;
            break;
        case '4':
            event = k400;
            break;
        case '5':
            event = k500;
            break;
        default:
            event = kBadEvent;
        }

    pMCB->currEvent = event;

    }



/***************************************************************************
*
* SMTP_ProcessEvent - Marks the availability of valid recipient.
*
* This routine serve to check whether or not we have a valid recipient and
* sets the 'isValidRecepient' flag to TRUE if valid.
*
*
* NOMANUAL
*/

LOCAL void SMTP_ProcessEvent
    (
    MAILCONTROLBLOCK    *pMCB   /* control block structure */
    )
    {
    /*
     * Eventually, this function can be an entry point for sophisticated
     * processing based on a high granularity of event characteristics.
     * Retry counters could be set, other options could be executed, etc.
     * In the immediate term, this function will merely serve to check
     * whether or not we've have a valid recipient.
     */

    if ((kSENT_RCPT == pMCB->currState)  &&
        (k200 == pMCB->currEvent      ))
        {
        pMCB->isValidRecipient = TRUE;
        }
    else if ((kSENT_LAST_RCPT == pMCB->currState)  &&
             (k200 == pMCB->currEvent           ))
        {
        pMCB->isValidRecipient = TRUE;
        }

    return;
    }



/***************************************************************************
*
* SMTP_InitMailControlBlock - Initilaises SMTP mail control block structure.
*
* This routine sets the initial values for SMTP Mail control block structure.
*
* RETURNS: OK, or ERROR if failed to initialise.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_InitMailControlBlock
    (
    MAILCONTROLBLOCK *pMCB,  /* control block structure */
    OS_SOCKET_T sock              /* TCP connection with SMTP Server */
    )
    {
    WMB_SESSION_T     *pEnv;
    STATUS        status;
    WMB_CID_T   cid;

#if 0
    status = ENVIRONMENT_Construct(sock, &pEnv);
#else /* 0 */
    if (wmbBackplaneComponentGet(wmbBackplaneGet(), &cid) != OK)
        logInfo("Error wmbBackplaneComponentGet\n");
    status = wmbSessionCreate(cid, sock, 0, &pEnv);
#endif /* 0 */

    if( OK != status )
        return status;

    pEnv->pBladeData = NULL;
    wmbSessionTypeSet (pEnv, WMB_TYPE_SMTP_K);
    pEnv->fileIncludeLevel = 0;

    pMCB->pEnv              = pEnv;
    pMCB->currState         = kSMTP_OPEN;
    pMCB->currEvent         = kOpen;
    pMCB->isValidRecipient  = FALSE;
    pMCB->runAutomaton      = TRUE;


#ifdef __SNMP_API_ENABLED__
    status = OCSNMP_ConstructSnmpEnviron((WMB_SESSION_T_WMM *)pEnv);
    if( OK != status )
    {
        (void) SMTP_CleanupMailControlBlock(pMCB);
        return status;
    }
#endif

    return status;
    }



/***************************************************************************
*
* SMTP_CleanupMailControlBlock - Clean up the memory allocated for environment.
*
* This routine frees the memory allocated for WMB_SESSION_T structure.
*
* RETURNS: OK
*
* NOMANUAL
*/

LOCAL STATUS SMTP_CleanupMailControlBlock
    (
    MAILCONTROLBLOCK *pMCB    /* control block structure */
    )
    {
    WMB_SESSION_T *p_envTemp;

    p_envTemp = pMCB->pEnv;

    (void) wmbSessionDestroy(*(&p_envTemp));
    return OK;

    }


/***************************************************************************
*
* SMTP_Init - Initialises the SMTP state table for FSM.
*
* This routine allocates the memory for state table and initialises the table
* with routines for each state and event involved in SMTP FSM. This routine
* must be invoked to initialize the FSM before using SMTP_SendMail() to send
* the email alert.
*
* RETURNS: OK or ERROR if fail to allocate memory for state table.
*
* SEE ALSO: smtpParser
*/

STATUS SMTP_Init(void)
    {
    sbyte4 i, j;

    for (i = 0; i < kNumStates; i++)
        {
        mSmtpStateTable[i] =
            (Smtp_StateEntry*) wmCalloc(kNumEvents,sizeof(Smtp_StateEntry));

        if (NULL == mSmtpStateTable[i])
            return wmError(ERROR_MEMMGR_NO_MEMORY,NULL);
        }

    /* Default Initialization */

    for ( i = 0; i < kNumStates; i++ )
        for ( j = 0; j < kNumEvents; j++)
        {
            mSmtpStateTable[i][j].p_funcAction    = SMTP_AbortConnection;
            mSmtpStateTable[i][j].p_funcNextState = SMTP_AbortNextState;
        }

    /* Specific SMTP Initialization */
    mSmtpStateTable[kSMTP_OPEN][k200].p_funcAction      = SMTP_SendHelo;
    mSmtpStateTable[kSMTP_OPEN][k200].p_funcNextState   =
                                                        SMTP_StandardNextState;

    mSmtpStateTable[kSENT_HELO][k200].p_funcAction      = SMTP_SendMailFrom;
    mSmtpStateTable[kSENT_HELO][k200].p_funcNextState   =
                                                        SMTP_StandardNextState;

    mSmtpStateTable[kSENT_MAIL][k200].p_funcAction      = SMTP_SendRcptTo;
    mSmtpStateTable[kSENT_MAIL][k200].p_funcNextState   = SMTP_RcptToNextState;

    mSmtpStateTable[kSENT_RCPT][k200].p_funcAction      = SMTP_SendRcptTo;
    mSmtpStateTable[kSENT_RCPT][k200].p_funcNextState   = SMTP_RcptToNextState;

    mSmtpStateTable[kSENT_RCPT][k500].p_funcAction      = SMTP_SendRcptTo;
    mSmtpStateTable[kSENT_RCPT][k500].p_funcNextState   = SMTP_RcptToNextState;

    mSmtpStateTable[kSENT_LAST_RCPT][k200].p_funcAction     = SMTP_SendData;
    mSmtpStateTable[kSENT_LAST_RCPT][k200].p_funcNextState  =
                                                        SMTP_StandardNextState;

    mSmtpStateTable[kSENT_LAST_RCPT][k500].p_funcAction     =
                                                      SMTP_ValidateAndSendData;
    mSmtpStateTable[kSENT_LAST_RCPT][k500].p_funcNextState  =
                                             SMTP_ValidateAndSendDataNextState;

    mSmtpStateTable[kSENT_DATA][k300].p_funcAction          = SMTP_XmitData;
    mSmtpStateTable[kSENT_DATA][k300].p_funcNextState       =
                                                        SMTP_StandardNextState;

    mSmtpStateTable[kXMITTED_DATA][k200].p_funcAction       = SMTP_SendQuit;
    mSmtpStateTable[kXMITTED_DATA][k200].p_funcNextState    =
                                                        SMTP_StandardNextState;

    mSmtpStateTable[kSENT_QUIT][k200].p_funcAction          = SMTP_CloseDown;
    mSmtpStateTable[kSENT_QUIT][k200].p_funcNextState       =
                                                        SMTP_FinishedNextState;

    mSmtpInitialized = TRUE;

    return OK;
    }


/***************************************************************************
*
* SMTP_RetrieveDataFromSocket - Reads the data received on socket, from server.
*
* This routine reads the data on TCP socket connected to server. Retrieved
* data is returned in <pBuf> and number of bytes read is returned in <pNumRead>
*
* RETURNS: OK or ERROR if fail to read.
*
* NOMANUAL
*/

LOCAL STATUS SMTP_RetrieveDataFromSocket
    (
    MAILCONTROLBLOCK *pMCB,   /* control block structure */
    sbyte *pBuf,                   /* pointer to receive buffer */
    sbyte4 bufSize,                /* buffer size */
    sbyte4 *pNumRead               /* number of bytes to read */
    )
    {
    ubyte2      numCrLfInRow;
    sbyte4      numReadSoFar, i;
#ifndef _WRS_CONFIG_LP64
    sbyte4      numRead;
#else
    sbyte8      numRead;
#endif

    *pNumRead    = 0;
    numCrLfInRow = 0;
    numRead      = 0;
    numReadSoFar = 0;

    while ((2 > numCrLfInRow) && (bufSize > numReadSoFar))
        {
#ifndef _WRS_CONFIG_LP64
        (void)wmnetTcpRead(pMCB->pEnv->sock, pBuf, (ubyte4)bufSize, &numRead);
#else
        (void)wmnetTcpRead(pMCB->pEnv->sock, pBuf, (ubyte8)bufSize, &numRead);
#endif

        if (0 > numRead)
            return (STATUS)numRead;

        if (0 == numRead)
            return SYS_ERROR_UNEXPECTED_END;

        for (i=0; i<numRead; i++)
            {
            if ((kCR == *pBuf) || (kLF == *pBuf))
                numCrLfInRow++;
            else
                numCrLfInRow = 0;

            pBuf++;
            numReadSoFar++;
            }
        }

    if ( 2 == numCrLfInRow )
    {
        *pNumRead = numReadSoFar;
        return OK;
    }
    else
        return SYS_ERROR_UNEXPECTED_END;
    }


/***************************************************************************
*
* SMTP_SendMail - Basic API to send the email through SMTP.
*
* This routine gets the 'domainName', 'mailServer', 'mailTo', 'mailFrom' and
* other required arguments through <pMailData>. It initialises the mail control
* block structure and FSM state table. It establishes the TCP connection with
* SMTP Server and starts the FSM by sending HELO command to the server. After
* resource file is sent to server, it cleans up the session and terminates the
* connection. smtpInit() must be called to initialize FSM before using this
* routine.
*
* RETURNS: OK or ERROR if failed to send the email.
*
* SEE ALSO: smtpParser
*/

STATUS SMTP_SendMail
    (
    Smtp_MailData *   pMailData   /* SMTP Mail data structure */
    )
    {
    OS_SOCKET_T             sock;
    enum smtpState          state;
    enum smtpEvent          event;
    STATUS   status;
    STATUS  (*pAction)(MAILCONTROLBLOCK *, Smtp_MailData *);
    void    (*pNextState)(MAILCONTROLBLOCK *, Smtp_MailData *);

    sbyte4   numRead = 0;
    MAILCONTROLBLOCK * pMCB = NULL;
    sbyte *  pRespBuff = NULL;


    if (!mSmtpInitialized)  return wmError(ERROR_SMTP_NOT_INIT,NULL);

    status = wmnetTcpConnect(&sock, pMailData->mailServer, kSmtpPort);
    if (status != OK)  return status;

    pMCB = wmCalloc(1,sizeof(MAILCONTROLBLOCK));
    if (pMCB == NULL)
        {
        (void) wmnetSocketClose(sock);
        return wmError(ERROR_MEMMGR_NO_MEMORY,NULL);
        }

    pRespBuff = wmCalloc(kMaxRespBuffSize, sizeof(sbyte));
    if (pRespBuff == NULL)
        {
        wmFree(pMCB);
        (void) wmnetSocketClose(sock);
        return wmError(ERROR_MEMMGR_NO_MEMORY,NULL);
        }

    status = SMTP_InitMailControlBlock( pMCB, sock );
    if (status != OK)
        {
        wmFree(pRespBuff);
        wmFree(pMCB);
        (void) wmnetSocketClose(sock);
        return status;
        }

    /* Execute the Mail Automaton */

    while ( pMCB->runAutomaton )
        {
        status = SMTP_RetrieveDataFromSocket(pMCB, pRespBuff,
                                             (sbyte4) kMaxRespBuffSize,
                                             &numRead);
        if (status != OK)  break;

        SMTP_GetEvent( pRespBuff, (Length)numRead, pMCB );
        SMTP_ProcessEvent( pMCB );

        /* Set up temporary variables (for ease in reading) */

        event       = pMCB->currEvent;
        state       = pMCB->currState;
        pAction     = mSmtpStateTable[state][event].p_funcAction;
        pNextState  = mSmtpStateTable[state][event].p_funcNextState;

        /* Perform the action and get the next state */

        status = pAction( pMCB, pMailData );

        if (status != OK)  break;

        pNextState( pMCB, pMailData );
        }

    state = pMCB->currState;   /* save before clean-up */

    wmFree(pRespBuff);
    (void) SMTP_CleanupMailControlBlock(pMCB);
    wmFree(pMCB);
    (void) wmnetSocketClose(sock);

    return (state == kFINISHED)  ?  OK : wmError(ERROR_SMTP_GENERAL,NULL);


    }

#endif /* WEBCLI_INCLUDE_SMTP */
#endif

