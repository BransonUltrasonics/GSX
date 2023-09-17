/* httpLCLib.c - common Live Control functionality */

/*
 * Copyright (c) 1996-2008, 2010-2014 Wind River Systems, Inc.
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
08apr13,r_w  fix static analysis issues for defect WIND00411995
20jul12,r_w  eliminate memory leak issue. defect WIND00359749
13jan11,m_z  verify input parameter validation
             CQID: Fix WIND00250448
07jan11,m_z  correct compiling error on linux
             CQID: Fix WIND00249657
17dec10,m_z  part of APIs changed
             CQID: Fix WIND00247096
28jul10,m_z  eliminate compile warning
19jul10,h_y  modified for WIND00223629
16dec08,y_z  set bLCPollTimerThreadEnable to TRUE before create
             thread LCPollMedium and LCPollHigh for race condition problem
12jun08,q_z  clear LC Poll Threads when server shutdown(WIND00106722)
10jan08,ps   Use usleep()-based LiveControl polling task scheme for both
             vxWorks and Linux.
01oct07,msa  Add check to ensure only one httpLCPollTimerThread is running
             at any given time. Defect WIND00103869.
05sep07,ten  Add include to allow building for linux target
01aug07,ps   conditionally compile certain declarations for Linux (Defect
             WIND00097236)
22feb07,ps   apigen reformatting
13feb07,msa  Fix comments to remove APIgen errors.
20jul06,ps   SPR116676 add httpLCErrorHandler();
             fixes unnecessary LC timeouts.
20jul06,ps   rework posix timer creation on vxWorks
20jul06,ps   remove pollpush timer semaphores on Linux; use usleep()
17may06,ps   add sysLib.h include file. CQ WIND000044862
03may06,ps   ifdef out some debugging printfs
16feb06,ps   changes for webcli on linux
19oct05,ps   fixes related to SPR112566
26may04,dlr  Fix endian cast of boolean type for BE targets.
14may04,dlr  Adapt to instance rules of wmbDataXxx, fix for GetNext on Scalars, Fetch Binaries
04jan04,dlr  Added support for GET and GETNEXT cmnd, variable instances
22dec03,dlr  wrap LC code inside #ifdef INCLUDE_HTTP_LIVE_CONTROL
20oct03,dlr  Port to WM Web 4.3
08apr98,mb   integration using generic lists.
02jul97,mb   adaption to new HTTP server concept.
26nov96,mb   written.
*/

/*
DESCRIPTION
This package provides remote controling of devices by the Live Control technology

Live Control Structures:
------------------------
Maintains list of registered LC clients and subscription entries.

Client List       = List of LC Clients
         Index    = Assigned cookie value from WMW
         Data     = Assigned Socket ID from OS

Subscribe List    = List of Entries
         Index    = WindMark Names
         Data     = Pointer to Entry's Client List

Entry Client List = List of Clients for this WindMark
         Index    = Client's Cookie Names
         Data     = Client's ID (pointer to Client structure)


Client List:   c1=[ "Cookie1" ]  c2=[ "Cookie2" ]  c3=[ "Cookie3" ]
                  [           ]---->[           ]---->[     0     ]
                  [ SocketId1 ]     [ SocketId2 ]     [ SocketId3 ]


Subcribe List: s1=[ "Entry1"  ]  s2=[ "Entry2"  ]  s3=[ "Entry3"  ]
                  [           ]---->[           ]---->[     0     ]
                  [ ClientList]     [ ClientList]     [ ClientList]
                    |                 |                 |
                    v                 v                 v
                  [ "Cookie1" ]     [ "Cookie3" ]     [ "Cookie3" ]
                  [           ]     [     0     ]     [     0     ]
                  [ClientID=c1]     [ClientId=c3]     [ClientId=c3]
                        |
                        v
                  [ "Cookie3" ]
                  [     0     ]
                  [ClientID=c3]


Live Control Polling Structures:
--------------------------------
Active polling that compares current values with new values (whether a local WindMark
or a remote SNMP object) using WMB transactions, and notifies Clients of changes.

[No   Priority List (Handler Push) ]
     List 0:      [Transaction]
                  [ ObjectLst ]---->[ Empty ]

     List 1:      [Transaction]
                  [ ObjectLst ]---->[ Empty ]

[High Priority List (~ 1/3 second) ]
     List 0:      [Transaction]
                  [ ObjectLst ]---->[ Empty ]

     List 1:      [Transaction]
                  [ ObjectLst ]---->[ Empty ]

[Med  Priority List (~ 1 second) = Default ]
     List 0:      [Transaction]     [ "Entry1" ]     [ "Entry2" ]     [ "Entry3" ]
                  [ ObjectLst ]---->[          ]---->[          ]---->[          ]
                                    [ value    ]     [ value    ]     [ value    ]
                                    [ type     ]     [ type     ]     [ type     ]

     List 1:      [Transaction]     ["Entry1"  ]     [ "Entry2" ]     [ "Entry3" ]
                  [ ObjectLst ]---->[          ]---->[          ]---->[          ]
                                    [ value    ]     [ value    ]     [ value    ]
                                    [ type     ]     [ type     ]     [ type     ]



Live Control Debugging:
------------------------
HTTP_LC_DEBUG                 : LC activity messages compiled in and sent to console
HTTP_LC_REG_STRINGS_ONLY      : Have all entry data pass as test strings over LC
HTTP_LC_POLLING_SHOWBLOCKING  : Show the lock messages for polling passes (this will
                                scroll the console, lose displayed information)


INCLUDE FILES: private/httpReq.h
               httpLib.h
               httpLCLib.h

NOMANUAL
*/

#include "string.h"

#include "commonCfg.h"

#ifdef WEBCLI_ON_LINUX
/* stuff */
#else
# include "timers.h"
# include "errnoLib.h"
# include <sysLib.h>
#endif

#include "httpLib.h"
#include "httpReq.h"
#include "httpCfg.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "httpLCLib.h"

/* Only compile if feature is enabled (and existing) in project */
#ifdef INCLUDE_HTTP_LIVE_CONTROL               /* include Live Control Java processing */

/* LOCAL vars */
LOCAL ubyte4            usMaxClients;          /* Maximum number of clients */
LOCAL ubyte4            ulMaxSymbols;          /* Maximum number of entries */
LOCAL HTTP_GEN_LIST_ID  clientListId;          /* List of clients           */
LOCAL HTTP_GEN_LIST_ID  subscribeListId;       /* List of subscribes entries */
LOCAL OS_SEM_T          semLockLC     = NULL;  /* Master  LC lock */
LOCAL OS_SEM_T          semSyncLCPoll = NULL;  /* Watchdog sync semaphore for polling */
LOCAL OS_SEM_T          semCloseLC    = NULL;  /* Watchdog sync semaphore for socket close queue */

ubyte4 httpRpmLCHighPoll              = 20;    /* default of about 1/3 second, in ticks */
ubyte4 httpRpmLCMediumPoll            = 60;    /* default of about 1   second, in ticks */

WMB_SESSION_T * pLCSession            = NULL;  /* WMB Session used by LC thread */

LOCAL BOOL bLCPollTimerThreadEnable   = FALSE; /* Flag to hold LC polling in non-stop loop */

/* forward defines */
HTTP_STATUS       httpLCDisplay(void);
HTTP_STATUS       httpLCWindMarkType(char *szName,DATATYPE_T *nType);
HTTP_STATUS       httpLCWindMarkIsWmbTable(char *szName, BOOL *bIsWmbTable, BOOL  *bIsSNMPTable);

LOCAL HTTP_STATUS httpLCPollPushStartUp(ubyte4  flags);
void              httpLCPollPushShutdown(void);
LOCAL HTTP_STATUS httpLCPollRemoveWM(char *szName);
LOCAL HTTP_STATUS httpLCPollAddWM(sbyte *szName, ubyte4 nPriority);

MUD_CONN_ACTION   httpLCErrorHandler (MUD_CONN_ID pConn);
BOOL httpLCWindMarkInstanceUpdate( char * szOldName, char *  szNewName);
HTTP_STATUS httpLCFetchValue(char *   szName, char * szNameBf, void * szValue, size_t nValueMax,  size_t * pValueSize, ubyte4 nCmnd);

HTTP_STATUS httpLCPollMoveWM( char *  szName, ubyte4  nNewPriority );
void webcli_select_usleep (long usec);   
void httpLCPthreadPollPushUnblocker( void * cookie );
void httpLCPollTimerThread (void);
extern STATUS errnoSet (int error);


/* local defines */
#define LCPOLLDISPLAY_BUF 0x01  /* Display internal buffer pointers */
#define LCPOLLDISPLAY_VAL 0x02  /* Display buffer values */
#define LCPOLLDISPLAY_LMT 0x04  /* Limit display list (e.g. if broken) */
#define LCPOLLDISPLAY_LCK 0x10  /* Don't use the locks (if tasks are paused) */

/* Polling structures and defines */
typedef struct LC_POLLPUSH_T
{
  WMB_TID_T pTID[2];
  ubyte4    nActiveTID;
  ubyte4    nPollTime;
  BOOL      bReady;
  ubyte4    flags;
} LC_POLLPUSH_T;

HTTP_STATUS httpLCPollPushTest(LC_POLLPUSH_T * pPollPush);

LC_POLLPUSH_T *timer_pollpush [3];
int test_timer_debug = 0;

LC_POLLPUSH_T * pPollPushNone;    /* No polling list */
LC_POLLPUSH_T * pPollPushMedium;  /* Medium time priority list */
LC_POLLPUSH_T * pPollPushHigh;    /* High   time priority list */

#define nOtherTID(n) ((n+1) & 0x01) /* toggle between the before/after transaction lists */

extern STATUS errnoSet (int error);

/* forward polling defines */
LOCAL HTTP_STATUS httpLCPollStartTimer(LC_POLLPUSH_T *  pPollPush);

/*******************************************************************************
*
* httpLCInit - initialize the Live Control package
*
* This routine must be called prior to any other function within this package.
* httpLCInit() will create an administrative list to handle up to usClients
* Live Control clients and up to ulSymbols different subscribed WindMarks at
* the same time.
*
* RETURNS: HTTP_OK if initialization succeeded,
*          HTTP_ERROR otherwise.
*/

HTTP_STATUS httpLCInit
    (
    ubyte4  usClients, /* max # of clients using the server        */
    ubyte4  ulSymbols  /* max # of different subscribable entries  */
    )
    {
    HTTP_STATUS status = HTTP_OK;

    /* Init semaphore for Live Control */
    if ( OK != wmosSemMCreate ( &semLockLC, HTTP_SEM_Q_PRIORITY ))
        return HTTP_ERROR;

    /* Init semaphore for Socket Close Queue */
    (void)wmosSemBinaryCreate (&semCloseLC, SEM_Q_FIFO);
    if (NULL == semCloseLC)
        return HTTP_ERROR;

    /* Get a session for the LC notify data lookups */
    status = (HTTP_STATUS)wmbSessionCreate(httpGetWmbComp(), 0, 0, &pLCSession);
    if (OK != status)
        return HTTP_ERROR;

    /* Create the core LC lists */
    usMaxClients = usClients;
    ulMaxSymbols = ulSymbols;
    if ( NULL == ( subscribeListId = httpGenListCreate ( httpGlobalMemPartIdGet() , (ubyte2)ulSymbols , TRUE ) ))
        return HTTP_ERROR;

    if ( NULL == ( clientListId    = httpGenListCreate ( httpGlobalMemPartIdGet() , (ubyte2)usClients , TRUE ) ))
        return HTTP_ERROR;

    /* Init the Polling loop(s) */
    (void) httpLCPollPushStartUp(0);

    return HTTP_OK;
    }

/*******************************************************************************
*
* httpLCLock - locks the Live Control subscription list
*
* This routine uses a mutual exclusion semaphore to lock accesses to the
* Live Control subscription list.
*
* ATTENTION: This is a "global lock in the universe" to all accesses that might
*            be done to the Live Control package!
*
* NOMANUAL
*
* RETURNS: HTTP_OK if locking was successful, or
*          HTTP_ERROR otherwise.
*/

HTTP_STATUS httpLCLock (void)
    {
#ifdef HTTP_LC_DEBUG
    printf("httpLCLock(task=%lx)\n",taskIdSelf());
#endif

    return (HTTP_STATUS)( (OK != wmosSemTake (semLockLC, WAIT_FOREVER_K)) ? HTTP_ERROR : HTTP_OK);
    }

/*******************************************************************************
*
* httpLCUnlock - unlocks the Live Control subscription list
*
* This routine uses a mutual exclusion semaphore to unlock access to the
* Live Control subscription list.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if unlocking was successful,
*          HTTP_ERROR otherwise.
*/

HTTP_STATUS httpLCUnlock (void)
    {
#ifdef HTTP_LC_DEBUG
    printf("httpLCUnLock(task=%lx)\n",taskIdSelf());
#endif

    return (HTTP_STATUS)( (OK != wmosSemGive (semLockLC)) ? HTTP_ERROR : HTTP_OK);
    }

/*******************************************************************************
*
* httpLCSubscribeListRemove - clean up empty subscriber list entries
*
* This routine clean up empty subscriber list entries. This is an internal
* routine.
*
* NOMANUAL
*
* RETURNS: N/A
*/

LOCAL void httpLCSubscribeListRemove
    (
    HTTP_GEN_LIST_ENTRY_ID subsEntryId  /* Subscribe ID */
    )
    {
    HTTP_GEN_LIST_ID clients;
    char * szName;

    if ( subsEntryId != NULL )
        {
        if ( ( clients = httpGenListDataGet ( subsEntryId ) ) != NULL )
            {
            if ( httpGenListItemsGet ( clients ) == 0 )
                {
                /* First remove the orphaned WindMark from the LC Polling list */
                (void) httpLCPollRemoveWM(subsEntryId->szId);

                /* now delete the subscription entry for the WindMark, and free Entry name */
                szName = httpGenListIdGet( subsEntryId );
                httpGenListRemove ( subscribeListId , subsEntryId );
               (void) memPoolFree ( httpGlobalMemPartIdGet() , szName );

                /* Finally, destroy the cilent list */
                httpGenListDestroy ( clients );
                }
            }
        }
    }

/*******************************************************************************
*
* httpLCSubscribeAccess - Does this user have access to this WindMark?
*
* This routine checks to see if (1) the user has access to this WindMark, and
* (2) if this WindMark is readable. This routine needs reqID to get to
* the Session structure which contains the user's access rights
*
* Write access is tested by default when Live Control is used to set values
*
* NOMANUAL
*
* RETURN: TRUE if the WindMark user has access, or
*         FALSE otherwise.
*/

BOOL httpLCSubscribeAccess
    (
    HTTP_REQ_ID const      reqId, /* Handle of the active connection    */
    char *                 szName /* WindMark name       */
    )
    {
    WMB_OBJ_T   obj;      /* Temp object record to test WindMark name */
    sbyte       *pInstance;
    sbyte4      nInstanceLength;
    STATUS      status;

    if (NULL == httpWmbSessionGet(reqId))
        {
#ifdef HTTP_LC_DEBUG
        printf("httpLCSubscribeAccess: No Session Record for this request.\n",szName);
#endif
        return TRUE;
        }

    /* preset the instance */
    pInstance   = NULL;
    nInstanceLength = 0;

    /* Create an object records for the WindMark, with no TID */
    if (OK != wmbObjectCreate(NULL, szName, pInstance, nInstanceLength, &obj))
        {
#ifdef HTTP_LC_DEBUG
        printf("httpLCSubscribeAccess: Could not create object for %s.\n",szName);
#endif
        wmError (0, "httpLCSubscribeAccess: Could not create object.");
        return FALSE;
        }

    /* Test this WindMark Object against the session access rights */
    status = wmbObjectPermission (httpWmbSessionGet(reqId), obj, CMD_GET_K);

    /* Delete the temp object record */
    wmbObjectDelete(obj);

    return ( (OK == status) ? TRUE:FALSE);
    }

/*******************************************************************************
*
* httpLCSubscribe - subscribe a WindMark for Java Live Control
*
* This routine subscribes a WindMark for Java Live Control. Subscribed WindMarks
* get registered at the Live Control subsystem of the HTTP server. If for any
* reason the server recognizes that a subscribed WindMark has been changed,
* all clients which registered this WindMark will receive a notification.
*
* NOMANUAL
* RETURN: TRUE if the WindMark was added successfully, or
*         FALSE otherwise.
*/

BOOL httpLCSubscribe
    (
    MUD_CONN_ID             ioHandle, /* Handle of the active connection */
    char *                  szName,   /* WindMark name       */
    ubyte4                  nPriority,/* priority indicates which polling list */
    HTTP_GEN_LIST_ENTRY_ID  clientId, /* subscribing client */
    ubyte4                  *status   /* subscription status flag */
    )
    {
    HTTP_GEN_LIST_ID        clients;
    HTTP_GEN_LIST_ENTRY_ID  subsEntryId = NULL;
   /* MUD_CONN_ID             subReqId;*/

    *status = LC_SUBSCRIBE_OK;

    /* Is WindMark already subscribed? */
    if ( ( subsEntryId = httpGenListFind ( subscribeListId ,szName ) ) == NULL )
        {
        /* WindMark is not subscribed yet, so create a new client list for subscriptions */
        if ( ( clients = httpGenListCreate ( httpGlobalMemPartIdGet() , (ubyte2)usMaxClients , TRUE )) != NULL )
            {
            if ( NULL == (subsEntryId = httpGenListDataAppend(subscribeListId,
                                                              httpStrdup(httpGlobalMemPartIdGet(),
                                                              szName),
                                                              clients)))
                {
                httpGenListDestroy ( clients );
                logErrorMsg ( "httpLCSubscribe: tried to subscribe %s - too many clients!" , szName );
                *status = LC_SUBSCRIBE_ERROR;
                return FALSE;
                }
            }
        }
    else
        {
        clients = httpGenListDataGet ( subsEntryId );
        }

    /* Append this client to entry */
    /*subReqId = (MUD_CONN_ID) httpGenListDataGet ( clientId );*/
    if ( httpGenListFind(clients,httpGenListIdGet ( clientId )) == NULL )
        {
        subsEntryId = httpGenListDataAppend ( clients , httpGenListIdGet ( clientId ) , clientId );
        }
    else
        {
        /* this multiple subscription is expected for GET and GETNEXT (but not SUBSCRIBE) */
        /* logErrorMsg ( "httpLCSubscribe: WindMark %s subscribed multiple times!" , szName ); */
        *status = LC_SUBSCRIBE_DUP;
        }

    /* add to polling */
    if ( NULL != subsEntryId )
        {
        (void) httpLCPollAddWM(szName, nPriority);
        }
    else
        {
        *status = LC_SUBSCRIBE_ERROR;
        }

    return ( (*status != LC_SUBSCRIBE_ERROR) ? TRUE:FALSE  );
    }

/*******************************************************************************
*
* httpLCUnsubscribe - unsubscribe a WindMark from Java Live Control system
*
* This routine unsubscribes a WindMark from Java Live Control system. WindMarks
* subscribed using httpLCSubscribe() must be unsubscribed if no further
* notifications should be done by the Live Control system. If the network
* connection to the remote client breaks down, all WindMarks subscribed by the
* cut off client must be unsubscribed, too.
*
* NOMANUAL
*
* RETURN: TRUE if the WindMark+Client was unsubscribed, or
*         FALSE otherwise.
*/

BOOL httpLCUnsubscribe
    (
    char *                 szName,   /* WindMark name to remove */
    HTTP_GEN_LIST_ENTRY_ID clientId  /* client id             */
    )
    {
    HTTP_GEN_LIST_ID       clListId;
    HTTP_GEN_LIST_ENTRY_ID subsEntryId,clientEntryId;
    ubyte4                 fSuccess = FALSE;

    if ( ( subsEntryId = httpGenListFind ( subscribeListId , szName  ) ) != NULL )
        {
        clListId = httpGenListDataGet ( subsEntryId );
        if ( ( clientEntryId = httpGenListFind ( clListId , httpGenListIdGet ( clientId ) ) ) != NULL )
            {
            httpGenListRemove ( clListId , clientEntryId );
            /* finally reap the whole client sub list if empty */
            httpLCSubscribeListRemove( subsEntryId );
            fSuccess = TRUE;
            }
        else
            {
            logInfoMsg ( "httpLCUnsubscribe: Client %lx for WindMark %s was not found!", szName );
            }
        }
    else
        {
        logInfoMsg ( "httpLCUnsubscribe: WindMark %s was not found!", szName );
        }

    return (BOOL)( fSuccess );
    }

/*******************************************************************************
*
* httpLCWindMarkInstanceUpdate - update a registered WindMark's instance field
*
* A GetNext operation will presumably change the instance part of the registered
* WindMark name. This routine will replace the old subscription record (but
* preserve the client list), and it will replace the polling sturctures.
*
* NOMANUAL
*
* RETURN: TRUE if the WindMark was added updated, or
*         FALSE otherwise.
*/

BOOL httpLCWindMarkInstanceUpdate
    (
    char *          szOldName,   /* WindMark name w/ old instance */
    char *          szNewName    /* WindMark name w/ new instance */
    )
    {
    HTTP_GEN_LIST_ID        clients;
    HTTP_GEN_LIST_ENTRY_ID  subsEntryId;
    char *                  szName;

    /* Is old name WindMark subscribed? */
    if (NULL != ( subsEntryId = httpGenListFind ( subscribeListId ,szOldName ) ))
        {
        /* preserve client list */
        clients = httpGenListDataGet ( subsEntryId );

        /* remove old entry */
        szName = httpGenListIdGet( subsEntryId );
        httpGenListRemove ( subscribeListId , subsEntryId );
        (void) memPoolFree ( httpGlobalMemPartIdGet() , szName );

        /* update subscription entry */
        if ( NULL == httpGenListDataAppend(subscribeListId,
                                                          httpStrdup(httpGlobalMemPartIdGet(),
                                                          szNewName),
                                                          clients))
            {
            httpGenListDestroy ( clients );
            logErrorMsg ( "httpLCWindMarkInstanceUpdate: tried to subscribe %s - too many clients!" , szNewName );
            return FALSE;
            }

        /* Remove the old WindMark name from the LC Polling list */
        httpLCPollRemoveWM(szOldName);
        (void) httpLCPollAddWM(szNewName, kPollPushNone);
        return TRUE;
        }

    return FALSE;
    }

/*******************************************************************************
*
* httpLCClientFindByHandle - find registered Live Control client by io handle
*
* This routine finds a registered Live Control client by IO handle.
* Live Control clients must be registered at the Live Control system in order
* to store a MUD_CONN_ID for server event submissions.
* Therefore the client is stored within a global list of clients.
* If the network connection breaks down, the client must be unregistered
* using httpLCClientRemove() to release Live Control system resources.
* httpLCClientFindByHandle() helps to retrieve the MUD_CONN_ID for a
* previously registered client.
*
* NOMANUAL
*
* RETURN: HTTP_GEN_LIST_ENTRY_ID the client has been found in, or
*         NULL otherwise.
*
* SEE ALSO: httpLCClientRemove(), httpLCClientAdd(), httpLCClientFindByCookie()
*/

LOCAL HTTP_GEN_LIST_ENTRY_ID httpLCClientFindByHandle
    (
    MUD_CONN_ID   ioHandle  /* Handle of the active connection */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID clientId;
    HTTP_GEN_LIST_ENTRY_ID currentId = NULL;

    while ( ( clientId = httpGenListIterator ( clientListId , &currentId ) ) != NULL )
        if ( httpGenListDataGet ( clientId ) == (void *) ioHandle)
            break;
    return clientId;
    }


/*******************************************************************************
*
* httpLCClientFindByCookie - find registered Live Control client by ID string
*
* This routine finds a registered Live Control client by ID string.
* Live Control clients must be registered at the Live Control system in order
* to store a MUD_CONN_ID for server event submissions.
* Therefore the client is stored within a global list of clients.
* In order to be able to send response over channels other that the requesting
* one, channels must be identifiable. This can be reached using cookies.
* httpLCClientFindByCookie() helps to retrieve the MUD_CONN_ID for a
* previously registered client identifiable by cookie szCookie.
*
* NOMANUAL
*
* RETURN: HTTP_GEN_LIST_ENTRY_ID the client has been found in, or
*         NULL otherwise.
*
* SEE ALSO: httpLCClientRemove(), httpLCClientAdd(), httpLCClientFindByHandle()
*/

HTTP_GEN_LIST_ENTRY_ID httpLCClientFindByCookie
    (
    char * szCookie
    )
    {
    HTTP_GEN_LIST_ENTRY_ID clientId;

    clientId = httpGenListFind ( clientListId , szCookie );
    return clientId;
    }


/*******************************************************************************
*
* httpLCClientAdd - register a new client for Live Control communication
*
* This routine registers a new client for Live Control communication.
* Live Control clients must be registered at the Live Control system in order
* to store a MUD_CONN_ID for server event submissions.
* In addidion the client is stored within a global list of clients.
* If the network connection breaks down, the client must be unregistered
* using httpLCClientRemove() to release Live Control system resources.
*
* NOMANUAL
*
* RETURN: Client List ID  if the Client was unsubscribed successfully, or
*         NULL otherwise.
*
* SEE ALSO: httpLCClientRemove()
*/

HTTP_GEN_LIST_ENTRY_ID httpLCClientAdd
    (
    char *      szClient, /* name of client (= cookie name ) */
    HTTP_REQ_ID reqId   /* output channel */
    )
    {
    MUD_CONN_ID       ioHandle = reqId->ioHandle; /* Handle of the active connection */

    /* Set the Socket close callback */
    if (MUD_CONN_OK != mudConnectionHandlersSet(ioHandle,           /* MUD connection */
                                                NULL,               /* Handles incoming data */
                                                httpLCErrorHandler, /* Handles errors */
                                                (MUD_CALLBACK) httpLCSocketClosed, /* Handles closing */
                                                (void *) ioHandle   /* connCloseCallback argument */
                                                ))
    {
    logInfoMsg ( "can not add LiveControl client %s to mudConnectionHandlersSet socket close hook!",szClient);
    return NULL;
    }

    return httpGenListDataAppend ( clientListId , httpStrdup(httpGlobalMemPartIdGet(),szClient) , (void *) ioHandle );
    }


/*******************************************************************************
*
* httpLCClientRemove - unregister some Live Control client
*
* This routine unregisters a Live Control client. Live Control clients must
* be unregistered at the Live Control system in order to release Live Control
* system resources. Normally httpLCClientAdd() does provide this functionality
* by installing httpLCClientRemove() as call back function, which is executed
* by httoOsalSocketClose().
*
* NOMANUAL
*
* RETURN: N/A
*
* SEE ALSO: httpLCClientAdd(), httpLCClientFind()
*
*/

void httpLCClientRemove
    (
    MUD_CONN_ID   ioHandle  /* Handle of the active connection */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID iteratorId     = NULL;
    HTTP_GEN_LIST_ENTRY_ID iteratorIdPrev = NULL;
    HTTP_GEN_LIST_ENTRY_ID subsEntryId,clientId;
    char *                 szName;

    logInfoMsg ( "httpLCClientRemove() called. with hndl %#lx",  ioHandle);
    if ( ( clientId = httpLCClientFindByHandle ( ioHandle ) ) != NULL )
        {
        /* remove all WindMark subscriptions of specified client from subscription list */
        while ( ( subsEntryId = httpGenListIterator ( subscribeListId , &iteratorId ) ) != NULL )
            {
            szName = httpGenListIdGet ( subsEntryId );

            /* if entry was removed, restart table search since current record was just NULLed */
            if (TRUE == httpLCUnsubscribe ( szName , clientId ))
                iteratorId     = iteratorIdPrev;
            else
                iteratorIdPrev = iteratorId;
            }
        /* remove client entry from global list of clients */
        szName = httpGenListIdGet( clientId );
        httpGenListRemove ( clientListId , clientId );
        (void)memPoolFree ( httpGlobalMemPartIdGet() , szName );
        logInfoMsg ( "httpLCClientRemove: buried client %#lx.",ioHandle );
        }

   }

#ifndef _LC_NEW_SOCKET_CLOSE_

LOCAL int nHandlesToClose = 0;

/*******************************************************************************
*
* httpLCSocketClosed - Mark this Client of a (presumably closed) IO channel for deletion
*
* The Client entry is labeled for deletion by setting it's respective socket ID to NULL.
* This routine will then trigger the LC polling handler to delete the Client. This is
* done so that (1) this call does not block when used as a call-back from a socket close
* and the LC semaphore is in use, and (2) this can be invoked from inside the LC semaphore
* when a socket write fails, without dead-locking on the same LC semaphore or changing
* structure while this current transaction is in progress (via httpDoLCSocketClosed).
*
* NOMANUAL
*
* RETURN: HTTP_OK if the IO handler was found and removed, or
*         HTTP_ERROR otherwise.
*/

HTTP_STATUS httpDoLCSocketClosed
    (
      MUD_CONN_ID   ioHandle  /* Handle of the closed/dead connection */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID clientId;

    /* Use this from within LC */

    /* Find the respective Client ID for this handler */
    if (NULL == (clientId = httpLCClientFindByHandle(ioHandle)))
        return HTTP_ERROR;

    /* Mark this client for deletion, by NULLing the Client's handle */
    clientId->listItem.pData = (void *) NULL; /* httpGenListDataSet(clientId,NULL); */
    nHandlesToClose++;

    /* trigger the Polling thread to asynchronously remove the respective client et. al. */
    if (semSyncLCPoll != NULL )
        (void)wmosSemGive ( semSyncLCPoll );

    return(HTTP_OK);
    }

/* Used for the External API */

/************************************************************
*
* httpLCSocketClosed -
*
* NOMANUAL
*
*/

HTTP_STATUS httpLCSocketClosed
    (
      MUD_CONN_ID   ioHandle  /* Handle of the closed/dead connection */
    )
    {
    HTTP_STATUS status = HTTP_OK;

    (void) httpLCLock();
    status = httpDoLCSocketClosed(ioHandle);
    (void) httpLCUnlock();
    return status;
    }

#else

#define CLOSESOCKETARRAY_MAX 16
LOCAL MUD_CONN_ID closeSocketArray[CLOSESOCKETARRAY_MAX];
LOCAL int begPtr = 0;
LOCAL int endPtr = 0;


/************************************************************
*
* httpLCSocketClosed -
*
* NOMANUAL
*
*/

HTTP_STATUS httpLCSocketClosed
    (
    MUD_CONN_ID   ioHandle  /* Handle of the closed/dead connection */
    )
    {
    int endOrgPtr;

    if (OK != wmosSemTake (semCloseLC, WAIT_FOREVER_K))
        {
        return (HTTP_ERROR);
        }

    endOrgPtr = endPtr;
    endPtr    = (endPtr+1) % CLOSESOCKETARRAY_MAX;
    if (endPtr == begPtr)
        {
        wmosSemGive (semCloseLC);
        endPtr = endOrgPtr;
        return(HTTP_OK);
        }

    closeSocketArray[endPtr] = ioHandle;

    /* trigger the Polling thread to asynchronously remove the respective client et. al. */
    /*
    if (semSyncLCPoll != NULL )
        wmosSemGive ( semSyncLCPoll );
    */

    wmosSemGive (semCloseLC);
    return(HTTP_OK);
    }

HTTP_STATUS httpDoLCSocketClosed(MUD_CONN_ID ioHandle) {return (httpLCSocketClosed(ioHandle)); };


/************************************************************
*
* httpLCSocketCloseGet -
*
* NOMANUAL
*
*/

LOCAL MUD_CONN_ID httpLCSocketCloseGet()
    {
    MUD_CONN_ID   retIoHandle;  /* Handle of the closed/dead connection */

    if (OK != wmosSemTake (semCloseLC, WAIT_FOREVER_K))
        {
        return (NULL);
        }

    if (endPtr == begPtr)
        {
        wmosSemGive (semCloseLC);
        return (NULL);
        }

    retIoHandle = closeSocketArray[begPtr];
    begPtr      = (begPtr+1) % CLOSESOCKETARRAY_MAX;

    wmosSemGive (semCloseLC);

    return (retIoHandle);
    }

#endif

/*******************************************************************************
*
* httpLCFetchValue - fetch value of a WindMark, GET/GETNEXT, Scalar/Table Object
*
* This routine will fetch a context-less WindMark name. It will determine if it
* member of an SNMP table (and thus require the instance in binary OID form),
* a WindMark table member, or a simple SNMP/WindMark scalar.
*
* It will work for both native and (perhaps mapped) string types.
*
* Instances are extracted and restored to the string WindMark name.
*
* In addtion to a basic GET request, it can process a GETNEXT updating the
* returned instance information as appropriate. An End-Of-Table is indicated
* by the instance not changing.
*
* NOTE: processing GETNEXT on a SNMP table object requires using the wmbTableLib API
*
* NOMANUAL
*
* RETURNS: HTTP_OK if the object has been fetched successfully, or
*          HTTP_ERROR if illegal name or instance.
*/

HTTP_STATUS httpLCFetchValue
    (
    char *   szName,     /* Original WindMark name                */
    char *   szNameBf,   /* Local    WindMark name                */
    void *   szValue,    /* WindMark value buffer                 */
    size_t   nValueMax,  /* WindMark value buffer max size        */
    size_t * pValueSize, /* size of new value                     */
    ubyte4   nCmnd       /* HTTP_LC_PACKET_GET | HTTP_LC_PACKET_GETNEXT */
    )
    {
    /* Local value fetch */
    sbyte      *pInstance,*pTemp;
    sbyte4     instanceLength;
    sbyte4     instanceMaxLength;
    char       instanceSep;
    BOOL       bIsTableObject  = FALSE;
    BOOL       bIsSNMPObject   = FALSE;
    BOOL       bIsRootInstance = FALSE;
    DATATYPE_T wmType          = DATATYPE_STRING_K;
    STATUS     status          = OK;

    char *     pStrInstance    = NULL;
    sbyte4     nStrInstanceLen = 0;
    sbyte4     nStrInstanceMax = 0;

    /* If this is a table WindMark with no instance, attach appropriate */
    (void) httpLCWindMarkIsWmbTable(szNameBf, &bIsTableObject, &bIsSNMPObject);
    if (bIsTableObject && (NULL == strchr (szNameBf, '(')) && (NULL == strchr (szNameBf, '.')))
        {
        bIsRootInstance = TRUE;
        strcat(szNameBf,(bIsSNMPObject) ? ".":"()");
        }

    /* SNMP Scalars cannot accept GetNext */
    if ((HTTP_LC_PACKET_GETNEXT == nCmnd) && bIsSNMPObject && !bIsTableObject)
        {
        logInfoMsg("httpLCFetchValue: GetNext of Scalar '%s'",szNameBf);
        strcpy(szValue,"<NoNextOnScalar>");
        *pValueSize = (ubyte4)strlen(szValue)+1;

        wmError (ERROR_GENERAL_NO_NEXT, "httpLCFetchValue: GetNext on Scalar");
        return ERROR;
        }

    /* Extract any instance/argument field */
    pInstance         = NULL;
    /* extract any instance/argument from WindMark name */
    if (NULL != (pInstance = strchr (szNameBf, '(')))
        {
        *pInstance++ = '\0';

        /* remove closing paren */
        if (NULL !=(pTemp = strrchr (pInstance, ')')))
          *pTemp = '\0';

        instanceLength = (sbyte4)strlen(pInstance)+1;
        instanceSep    = '(';
        }
    else if (NULL != (pInstance = strchr (szNameBf, '.')))
        {
        *pInstance++      = '\0';
        instanceLength    = (sbyte4)strlen(pInstance)+1;
        instanceMaxLength = kMagicMarkupBufferSize - (sbyte4)strlen(szNameBf) - 1;
        instanceSep       = '.';

        if (HTTP_LC_PACKET_GETNEXT == nCmnd)
            {

            /* The pInstance string OID buffer. Swap in binary version for transaction. */
            pStrInstance    = pInstance;
            nStrInstanceLen   = instanceLength;
            nStrInstanceMax   = instanceMaxLength;

            instanceMaxLength = 128*4;
            pInstance         = wmMalloc((unsigned long)instanceMaxLength);

            if (NULL != pInstance)
                {
                if ((0  == nStrInstanceLen) ||
                    (OK != wmConvertStrToOid(pStrInstance, (ubyte4 *) pInstance, instanceMaxLength, &instanceLength)) )
                    {
                    instanceLength = 0;
                    }
                }
            else
                {
                instanceMaxLength = 0;
                instanceLength    = 0;
                }

            if (OK != wmbTableCreate(pLCSession, szNameBf, (0 != instanceLength) ? pStrInstance:NULL, szNameBf))
                {
                }
            }
        }
    else
        {
        /* the buffer after the name is available for a new instance */
        pInstance         = &szNameBf[strlen(szNameBf) + 1];
        memset(pInstance,0,4);
        instanceLength    = 0;
        instanceSep       = '\0';
        }
    instanceMaxLength = kMagicMarkupBufferSize - (sbyte4)strlen(szNameBf) - 1;

    /* If its WindMark type maps to STRING, then force the value WMB lookup */
    /*  to string, so that non-LC types will be accessed and most importantly */
    /*  passed to LC as strings */
    (void)httpLCWindMarkType(szNameBf,&wmType);
#ifdef HTTP_LC_REG_STRINGS_ONLY
    wmType = DATATYPE_STRING_K;
#endif
    if (HTTP_SYM_TYPE_STRING_ID != httpWMtoLCType(wmType))
        {
        if ((HTTP_LC_PACKET_GETNEXT == nCmnd) && bIsSNMPObject)
            {
            /* GETNEXT for bIsSNMPObject objects are managed by wmbTableLib */
            void *table = wmbSessionTableGet(pLCSession);

            /* If Root Instance, then we have the first row data already */
            if (!bIsRootInstance)
                status = wmbTableRowNext(table);

            if (OK == status)
                {
                WM_BUFFER_T * oidSfx;

                /* Extract the GETNEXT string value !Use second half of value buffer! */
                (void)wmbTableGet(pLCSession,szNameBf, (((char*)szValue)+(nValueMax/2)) ,nValueMax/2);
                *pValueSize = (ubyte4)strlen( (((char*)szValue)+(nValueMax/2)) )+1;
                /* Convert to native type */
#ifndef _WRS_CONFIG_LP64
                (void) wmConvert( (((char*)szValue)+(nValueMax/2)) ,(sbyte4)*pValueSize,DATATYPE_STRING_K,szValue,(sbyte4)nValueMax,wmType,(sbyte4*) pValueSize);
#else
                (void) wmConvert( (((char*)szValue)+(nValueMax/2)) ,(long)*pValueSize,DATATYPE_STRING_K,szValue,(long)nValueMax,wmType,(long*) pValueSize);
#endif

                /* Extract the new GETNEXT instance */
                oidSfx = wmbTableInstanceSfxGet (pLCSession, szNameBf);
                instanceLength = (sbyte4)wmBufferUsedGet(oidSfx);
                if(NULL != wmBufferDataGet(oidSfx))
                    memcpy(pInstance,wmBufferDataGet(oidSfx),(size_t)instanceLength);
                }
            else
                {
                if (wmErrNo()!=ERROR_TABLE_END_OF_TABLE)
                    {
                    errnoSet(ERROR_TABLE_END_OF_TABLE);
                    printf("[[[ SHOULD BE END OF TABLE ]]]");
                    }
                }
            }
        else if (HTTP_LC_PACKET_GETNEXT == nCmnd)
            status = wmbDataNativeGetNext (pLCSession, szNameBf,pInstance,
                                           &instanceLength, instanceMaxLength,
                                           DATATYPE_STRING_K,szValue, (long)nValueMax,
                                           (ssize_t *)pValueSize);
        else
            status = wmbDataNativeGet (pLCSession, szNameBf,pInstance,instanceLength,
                                       szValue, (long)nValueMax,(ssize_t *)pValueSize);
        if (OK != status)
            {
            /* Handler Errors here */
            logInfoMsg("httpLCFetchValue: wmbDataNativeGet(%s) ERROR! =========\n",szNameBf);
            memset(szValue,0,4);
            *pValueSize = 4;
            }
        }
    else
        {
        if ((HTTP_LC_PACKET_GETNEXT == nCmnd) && bIsSNMPObject)
            {
            /* GETNEXT for bIsSNMPObject objects are managed by wmbTableLib */
            void *table = wmbSessionTableGet(pLCSession);
            /* If Root Instance, then we have the first row data already */
            if (!bIsRootInstance)
                status = wmbTableRowNext(table);

            if (OK == status)
                {
                WM_BUFFER_T * oidSfx;

                /* Extract the GETNEXT string value */
                (void)wmbTableGet(pLCSession,szNameBf,szValue,nValueMax);
                *pValueSize = (ubyte4)strlen(szValue)+1;

                /* Extract the new GETNEXT instance */
                oidSfx = wmbTableInstanceSfxGet (pLCSession, szNameBf);
                instanceLength = (sbyte4)wmBufferUsedGet(oidSfx);
                if (NULL != wmBufferDataGet(oidSfx))
                  memcpy(pInstance,wmBufferDataGet(oidSfx),(size_t)instanceLength);
                }
            else
                {
                if (wmErrNo()!=ERROR_TABLE_END_OF_TABLE)
                    {
                    errnoSet(ERROR_TABLE_END_OF_TABLE);
#ifdef HTTP_LC_DEBUG
                    printf("[[[ SHOULD BE END OF TABLE ]]]\n");
#endif
                    }
                }
            }
        else if (HTTP_LC_PACKET_GETNEXT == nCmnd)
            status = wmbDataStringGetNext (pLCSession, szNameBf,pInstance,&instanceLength,
                                           instanceMaxLength,DATATYPE_STRING_K,szValue,
                                           (long)nValueMax,(ssize_t *) pValueSize);
        else
            status = wmbDataStringGet (pLCSession, szNameBf,pInstance, instanceLength,
                                       szValue, kMagicMarkupBufferSize,(ssize_t*) pValueSize);

        if ((OK == status) && (wmErrNo()==ERROR_TABLE_END_OF_TABLE))
            {
            status = ERROR;
#ifdef HTTP_LC_DEBUG
            printf("[[[EOT TO ERRORNO]]]\n");
#endif
            }

        if (OK != status)
            {
            /* Handler Errors here */
            logInfoMsg("httpLCFetchValue: wmbDataStringGet(%s) ERROR! =========\n",szNameBf);
            strcpy(szValue,(wmErrNo()==ERROR_TABLE_END_OF_TABLE) ? "<EndTable>":"<NoValue>");
            *pValueSize = (ubyte4)strlen(szValue) + 1;
            }

        /* insure actual string length from wmbDataStringGet !!! */
        if ((strlen(szValue)+1) != *pValueSize)
            {
            *pValueSize = (ubyte4)strlen(szValue) + 1;
            }
        }

    /* Put any binary OID back to string OID */
    if (NULL != pStrInstance)
        {
        char *temp;

        if ((NULL == pInstance) || (0 == instanceLength) ||
            (OK != wmConvertOidToString((ubyte4 *) pInstance,instanceLength,pStrInstance,
                                         nStrInstanceMax,&nStrInstanceLen)))
            {
            *pStrInstance = '\0';
            }

        /* swap, so that pInstance is string OID ptr, and pStrInstance is temp buffer */
        temp            = pInstance;
        pInstance       = pStrInstance;
        pStrInstance    = temp;
        instanceLength  = nStrInstanceLen; /* we do not care about nStrInstanceLen after this */
        }

    /* The new GETNEXT instance needs some propagation */
    if (HTTP_LC_PACKET_GETNEXT == nCmnd)
        {
        /* remember that pInstance is one (delim) character past end of name */
        if ('\0' == instanceSep)
            {
            /* Force an instance delim if this base base instance (i.e. no instance) */
            instanceSep = (char)((bIsSNMPObject) ? '.':'(');
            }

        pInstance--;
        *pInstance = instanceSep;
        if ('(' == instanceSep)
            strcat(pInstance,")");


        /* Replace references to the old name to the new name (and instance) */
        (void) httpLCWindMarkInstanceUpdate(szName,szNameBf);

        /* UPDATE INPUT NAME BUFFER WITH NEW NAME*/
        strcpy(szName,szNameBf);

        /* Release wmbTable structue for this object? */
        if (bIsSNMPObject)
            {
            (void)wmbTableDestroy(pLCSession);
            }
        }
    else
        {
        /* remember that pInstance (if any) is one (delim) character past end of name */
        pInstance--;
        *pInstance = instanceSep;
        if ('(' == instanceSep)
            strcat(pInstance,")");
        }

    /* if pStrInstance buffer at large, free it */
    if (NULL != pStrInstance)
        wmFree(pStrInstance);

    return( (HTTP_STATUS)((OK == status) ? HTTP_OK:HTTP_ERROR));
    }

/*******************************************************************************
*
* httpLCNotify - notify all Live Control clients subscribed to a WindMark
*
* This internal routine notifies all Live Control clients which are subscribed to a
* WindMark. httpLCNotify() sends a WindMark value to all subscribed clients.
* Before doing so, the WindMark is updated using its get function if its
* new native value isn't passed to this routine.
*
* NOTE: If the WindMark was subscribed with an argument or instance, then the
* name MUST appear exactly the same, with the same argument or instance format,
* otherwise the LC Client will not recognize it.
*
* NOTE: ALL NON-LC TYPES MUST BE PASSED AS STRING VALUES. See httpWMtoLCType() for the
* mappings.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if all clients have been notified successfully, or
*          HTTP_ERROR if one or more clients could not be notified.
*/

HTTP_STATUS httpLCNotify
    (
    char *  szName,     /* WindMark name to push                 */
    void *  szValue,    /* WindMark value (NULL if not provided) */
    size_t  nValueSize, /* size of value  (0    if not provided) */
    ubyte4  nCmnd       /* HTTP_LC_PACKET_GET | HTTP_LC_PACKET_GETNEXT */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID subsEntryId, clientEntryId;
    HTTP_GEN_LIST_ID       clListId;
    HTTP_GEN_LIST_ENTRY_ID iteratorId = NULL;
    HTTP_STATUS            status = HTTP_OK;

    /* Local value fetch */
    char       *szBuffer;
    char       *szNameBf;
    DATATYPE_T wmType = DATATYPE_STRING_K;
    ubyte4     nType,i;
    ubyte2     nShort,*pShort;
    ubyte4     nLong ,*pLong;

    /* Got to have a name to work with */
    if ( szName == NULL )
        {
        return HTTP_ERROR;
        }

    /* Create temp buffers : two pieces out of one */
    if (NULL == (szBuffer = wmMalloc(2*kMagicMarkupBufferSize)))
        {
        return HTTP_ERROR;
        }
    szNameBf = &szBuffer[kMagicMarkupBufferSize];

    /* Make safe local copy of name */
    strcpy(szNameBf,szName);

    /* Fetch the current value of the WindMark, if not provided */
    if (NULL == szValue)
        {
        szValue    = szBuffer;
        nValueSize = 0;

        status = httpLCFetchValue(szName,szNameBf,szValue,kMagicMarkupBufferSize,&nValueSize,nCmnd);
        }

    /* Convert Native to Network ordering as needed */
    (void)httpLCWindMarkType(szNameBf,&wmType);
#ifdef HTTP_LC_REG_STRINGS_ONLY
    wmType = DATATYPE_STRING_K;
#endif
    nType = (ubyte4)httpWMtoLCType(wmType);
    switch (nType)
        {
        case HTTP_SYM_TYPE_BOOL_ID        :
            /* insure one byte LC boolean form */
            switch (nValueSize)
                {
                case 1 : break;
                case 2 : nShort = *((ubyte2 *) szValue);
                    *(char *) szValue = (char)((nShort) ? 1:0);
                    break;
                case 4 : nLong  = *((ubyte4  *) szValue);
                    *(char *) szValue = (char)((nLong ) ? 1:0);
                    break;
                }
            nValueSize = 1;
            break;

        case HTTP_SYM_TYPE_UCHAR_ID       :
        case HTTP_SYM_TYPE_CHAR_ID        :
            nValueSize = 1;
            break;

        case HTTP_SYM_TYPE_SHORT_ID       :
        case HTTP_SYM_TYPE_USHORT_ID      :
            /* convert to network order (HighEndian) Short from host Short */
            /* use szBuffer since szValue may be the calling routine's data */
            nValueSize = 2;
            pShort     = (ubyte2 *) szValue;
            nShort     = *pShort;
            for (i=0;i<nValueSize;i++,nShort >>= 8)
                szBuffer[nValueSize-1-i] = (char)(nShort & 0x0ff);
            szValue = szBuffer;
            break;

        case HTTP_SYM_TYPE_LONG_ID        :
        case HTTP_SYM_TYPE_INT_ID         :
        case HTTP_SYM_TYPE_ULONG_ID       :
        case HTTP_SYM_TYPE_UINT_ID        :
            /* convert to network order (HighEndian) long from host long */
            /* use szBuffer since szValue may be the calling routine's data */
            nValueSize = 4;
            pLong      = (ubyte4 *) szValue;
            nLong      = *pLong;
            for (i=0;i<nValueSize;i++,nLong >>= 8)
                szBuffer[nValueSize-1-i] = (char)(nLong & 0x0ff);
            szValue = szBuffer;
            break;

        case HTTP_SYM_TYPE_FLOAT_ID       : /* these are forced to string type */
        case HTTP_SYM_TYPE_DOUBLE_ID      :

        case HTTP_SYM_TYPE_STRING_ID      :
        case HTTP_SYM_TYPE_GENLIST_ID     :
        default               :
            break;
        }

    /* Push WindMark to all registered clients */
    if ( ( subsEntryId = httpGenListFind ( subscribeListId , szNameBf ) ) != NULL )
        {
        clListId = httpGenListDataGet ( subsEntryId );

        while ( ( clientEntryId = httpGenListIterator ( clListId , &iteratorId ) ) != NULL )
            {
            logInfoMsg ( "httpLCNotify: notify to WindMark %s.",szNameBf );
            /* httpLCClientNotify using httpLCNotify's locking */
            if (HTTP_OK != httpLCClientNotify((MUD_CONN_ID)httpGenListDataGet(httpGenListDataGet(clientEntryId)),
                                              szNameBf, szValue, nValueSize ))
                {
                status = HTTP_ERROR;
                }
            }
        }

    /* free the temp buffer(s) */
    wmFree(szBuffer);

    return status;
    }

/*******************************************************************************
*
* httpLCNotifyChange - directly notify Live Control of a WindMark value change
*
* This public routine is used in the user's handler code to directly push a value change to
* Live Control clients. It calls httpLCNotify() after asserting the locking.
*
* It is specifically intended for calling from the respective WinDMark handlers.
*
* NOTE: If the WindMark was subscribed with an argument or instance, then the
* name MUST appear exactly the same, with the same argument or instance format,
* otherwise the LC Client will not recognize it.
*
* NOTE: ALL NON-LC TYPES MUST BE PASSED AS STRING VALUES. See httpWMtoLCType() for the
* mappings.
*
* RETURNS: HTTP_OK if all clients have been notified successfully, or
*          HTTP_ERROR if one or more clients could not be notified.
*/

HTTP_STATUS httpLCNotifyChange
    (
    char *  szName,   /* WindMark's name to push */
    void *  szValue,  /* WindMark's STRING value (NULL if not provided) */
    ubyte4  nValueSize  /* size  of   string       (0    if not provided) */
    )
    {
    HTTP_STATUS status = HTTP_OK;

    (void) httpLCLock();
    status = httpLCNotify(szName, szValue, nValueSize, HTTP_LC_PACKET_GET);
    (void) httpLCUnlock();

    return status;
    }

void httpLCPollPushDisplay (ubyte4 nDisplayFlags);

/*******************************************************************************
*
* httpLCDisplay - Display the Live Control Structures
*
*
* RETURN: void
*/

HTTP_STATUS httpLCDisplay
    (
    void
    )
    {
    HTTP_GEN_LIST_ENTRY_ID subsEntryId, clientEntryId;
    HTTP_GEN_LIST_ID       clListId;
    HTTP_GEN_LIST_ENTRY_ID iteratorId = NULL;
    HTTP_GEN_LIST_ENTRY_ID iteratorId2= NULL;

    /* Are we initialized? */
    if (NULL == semLockLC)
        {
        printf("Live Control not yet initialized\n");
        return HTTP_OK;
        }

    /* no outside changes to the lists please */
    (void) httpLCLock();

    printf("=== Client List (0x%08lx): ====\n", (unsigned long)clientListId);
    while (NULL != ( clientEntryId = httpGenListIterator ( clientListId , &iteratorId ) ))
        {
        printf("Client:     '%20s' = 0x%08lx\n", clientEntryId->szId, (unsigned long)(clientEntryId));
        }

    iteratorId = NULL;
    printf("=== Subscriber List (0x%08lx): ====\n", (unsigned long)subscribeListId);
    while (NULL != ( subsEntryId = httpGenListIterator ( subscribeListId , &iteratorId ) ))
        {
        printf("Symbol:     '%-20s' = 0x%08lx\n", subsEntryId->szId, (unsigned long)subsEntryId);

        iteratorId2 = NULL;
        clListId = httpGenListDataGet ( subsEntryId );
        while (NULL != ( clientEntryId = httpGenListIterator ( clListId , &iteratorId2 ) ))
            {
            printf("    Client: '%20s' = 0x%08lx\n", clientEntryId->szId, (unsigned long)clientEntryId);
            }

        }
    printf("\n");
    (void) httpLCUnlock();

    /* Also display the polling transaction state */
    httpLCPollPushDisplay(LCPOLLDISPLAY_BUF);

    return(HTTP_OK);
    }


/*

 Live Control Poll functions for proactively detecting value changes


 The LC polling has three lists of different timing priority:
     None   : No polling. New values are explicited push with httpLCNotifyChange()
     Medium : Polling is done at about every 1   second
     High   : Polling is done at about every 1/3 second

 Basic fucntionality:

 Each Polling structure maintains two transaction. Each list contains object records
 for all the member WindMarks. Every time the polling timer triggers, the active
 transaction list is sent to the WMB, the values compared, and any differences are
 notified to the LC Clients. The active transaction indicator is toggled, and the respective
 timer re-started.
 The object record data values start out empty, so the first possible notify will not occur
 until the second polling event for any new entries.

 Polling is triggered via a watch dog timer and a syncronization semaphore.

*/


/*******************************************************************************
*
* httpLCPollPushDisplay - Display values in PollPush list;
*
* Display the before/after transaction lists of a polling table
*
* The display options are:
*    LCPOLLDISPLAY_BUF : display the transaction structure object pointers
*    LCPOLLDISPLAY_VAL : display the transaction values
*    LCPOLLDISPLAY_LMT : limit the display (is case pointers are corrupted)
*
* NOMANUAL
*
* RETURN: void
*/

LOCAL void httpLCPollPushDisplayList
    (
    LC_POLLPUSH_T *pPollPush,   /* Poll List */
    ubyte4        nDisplayFlags /* Display options */
    )
    {
    WMB_OBJ_T     obj;
    ubyte4        i,limit;
    WM_BUFFER_T   *pValue ;
    sbyte         *pBuffer;
    ulong_t       valueLen;

    /* How many entries to show */
    if (0 != (nDisplayFlags & LCPOLLDISPLAY_LMT))
        limit = 10; else limit = 100;

    /* loop through all objects in the group linked list */
    obj = wmbTransactionHeadGet(pPollPush->pTID[0]);
    printf("%s [0]%s",(0 == pPollPush->nActiveTID) ? "->" :"  ",
           (0 != (nDisplayFlags & LCPOLLDISPLAY_VAL)) ? ":\n":": ");
    while (wmbObjectGood(obj) && (limit--))
        {
        if (0 != (nDisplayFlags & LCPOLLDISPLAY_BUF))
            printf("<'%s'@%lx,%lx,%lx>",wmbObjectNamePtrGet(obj), (unsigned long)obj, (unsigned long)obj->pNext, (unsigned long)obj->pGroup);

        if (0 != (nDisplayFlags & LCPOLLDISPLAY_VAL))
            {
            printf("<'%-14s'=",wmbObjectNamePtrGet(obj));
            if (NULL != (pValue = wmbObjectValueGet(obj)))
                {
                valueLen  = wmBufferUsedGet(pValue);
                pBuffer   = wmBufferDataGet(pValue);
                for (i=0;(i<10) && (i<valueLen);i++) printf("%02x ",pBuffer[i] & 0x0ff);
                }
            printf(">\n");
            }

        /* Step to the next objects */
        obj = wmbObjectListNextGet(obj);
        }
    if (0 != (nDisplayFlags & LCPOLLDISPLAY_BUF))
        printf("\n");

    /* How many entries to show */
    if (0 != (nDisplayFlags & LCPOLLDISPLAY_LMT))
        limit = 10; else limit = 100;

    obj = wmbTransactionHeadGet(pPollPush->pTID[1]);
    printf("%s [1]%s",(1 == pPollPush->nActiveTID) ? "->" :"  ",
           (0 != (nDisplayFlags & LCPOLLDISPLAY_VAL)) ? ":\n":": ");
    while (wmbObjectGood(obj) && (limit--))
        {
        if (0 != (nDisplayFlags & LCPOLLDISPLAY_BUF))
            printf("<'%s'@%lx,%lx,%lx>",wmbObjectNamePtrGet(obj), (unsigned long)obj, (unsigned long)obj->pNext, (unsigned long)obj->pGroup);

        if (0 != (nDisplayFlags & LCPOLLDISPLAY_VAL))
            {
            printf("<'%-14s'=",wmbObjectNamePtrGet(obj));
            if (NULL != (pValue = wmbObjectValueGet(obj)))
                {
                valueLen  = wmBufferUsedGet(pValue);
                pBuffer   = wmBufferDataGet(pValue);
                for (i=0;(i<10) && (i<valueLen);i++) printf("%02x ",pBuffer[i] & 0x0ff);
                }
            printf(">\n");
            }

        /* Step to the next objects */
        obj = wmbObjectListNextGet(obj);
        }
    if (0 != (nDisplayFlags & LCPOLLDISPLAY_BUF))
        printf("\n");

    }

/*******************************************************************************
*
* httpLCPollPushDisplay - Display values in PollPush list;
*
* Display the state of the polling structures.
*
* NOTE: Set the option bit "LCPOLLDISPLAY_LCK" if this is called from inside the LC lock.
*
* RETURN: void
*/

void httpLCPollPushDisplay
    (
    ubyte4  nDisplayFlags     /* display options */
    )
    {
    /* Are we initialized? */
    if (NULL == semLockLC)
        {
        printf("Live Control not yet initialized\n");
        return;
        }

    /* by default, show the buffer info */
    if (0 == nDisplayFlags)
        nDisplayFlags = LCPOLLDISPLAY_BUF;

    /* no outside changes to the transaction list please */
    if (0 == (nDisplayFlags & LCPOLLDISPLAY_LCK))
        (void) httpLCLock();

    printf("None List:\n");
    httpLCPollPushDisplayList(pPollPushNone  ,nDisplayFlags);
    printf("Med  List:\n");
    httpLCPollPushDisplayList(pPollPushMedium,nDisplayFlags);
    printf("High List:\n");
    httpLCPollPushDisplayList(pPollPushHigh  ,nDisplayFlags);

    /* no outside changes to the transaction list please */
    if (0 == (nDisplayFlags & LCPOLLDISPLAY_LCK))
        (void) httpLCUnlock ();
    }

/*******************************************************************************
*
* httpLCPollPushTest - Fetch and compare new values in PollPush list; Notify the Clients
* if there are changes.
*
* NOMANUAL
*
* RETURNS: HTTP_OK    if successful, or
*          HTTP_ERROR if a trasaction fails, or the transaction lists become out-of-sync
*/

HTTP_STATUS httpLCPollPushTest
    (
    LC_POLLPUSH_T * pPollPush /* the Polling Loop to process */
    )
    {
    WMB_OBJ_T     obj0;
    WMB_OBJ_T     obj1;

    WM_BUFFER_T   *pValue0 ;
    sbyte         *pBuffer0;
    ulong_t       valueLen0;
    WM_BUFFER_T   *pValue1 ;
    sbyte         *pBuffer1;
    ulong_t       valueLen1;
    HTTP_STATUS   status = HTTP_OK;

    /* no outside changes to the transaction list please */
#ifdef  HTTP_LC_POLLING_SHOWBLOCKING
    httpLCLock();
#else
    wmosSemTake ( semLockLC, WAIT_FOREVER_K );
#endif

    /* Perform the transaction on the active list */
    if (wmbTransactionProcess(pPollPush->pTID[pPollPush->nActiveTID], CMD_GET_K) != OK)
        {
            wmError (0, "httpLCPollPushTest: failed to process transaction\n");
            return HTTP_ERROR;
        }

    /* loop through all objects in the group linked list */
    obj0 = wmbTransactionHeadGet(pPollPush->pTID[0]);
    obj1 = wmbTransactionHeadGet(pPollPush->pTID[1]);

    for (; wmbObjectGood(obj0) && wmbObjectGood(obj1);
         obj0 = wmbObjectListNextGet(obj0),obj1 = wmbObjectListNextGet(obj1))
        {
        if ((NULL != wmbObjectNamePtrGet(obj0))&&
            (NULL != wmbObjectNamePtrGet(obj1))&&
             (0 != strcmp(wmbObjectNamePtrGet(obj0),wmbObjectNamePtrGet(obj1)))
            )
            {
#ifdef HTTP_LC_DEBUG
            printf("httpLCPollPushTest: Lists out of sync (%s != %s) ERROR.\n",wmbObjectNamePtrGet(obj0),wmbObjectNamePtrGet(obj1));
#endif
            wmError (0, "httpLCPollPushTest: Lists out of sync\n");
            return HTTP_ERROR;
            }

        /* fetch value 0 */
        if ( (pValue0 = wmbObjectValueGet(obj0)) == NULL )
            {
            /* newly added WindMark entry: catch it next time */
            continue;
            }
        valueLen0 = wmBufferUsedGet(pValue0);
        pBuffer0  = wmBufferDataGet(pValue0);

        /* insure actual string length from wmbObjectValueGet !!! */
        if (DATATYPE_STRING_K == wmbObjectTypeLocalGet(obj0))
            {
            if(NULL != pBuffer0)
                valueLen0 = strlen(pBuffer0);
            else
                valueLen0 = 0;
            }

        /* fetch value 1 */
        if ( (pValue1 = wmbObjectValueGet(obj1)) == NULL )
            {
            /* newly added WindMark entry: catch it next time */
            continue;
            }
        valueLen1 = wmBufferUsedGet(pValue1);
        pBuffer1  = wmBufferDataGet(pValue1);

        /* insure actual string length from wmbObjectValueGet !!! */
        if ((DATATYPE_STRING_K == wmbObjectTypeLocalGet(obj1))&&(NULL != pBuffer1))
            {
            valueLen1 = strlen(pBuffer1);
            }

        /* Compare the values */
        if ((valueLen0 != valueLen1) || (0 != memcmp(pBuffer0, pBuffer1, valueLen0)))
            {
#ifdef HTTP_LC_DEBUG
            int q;
            printf(">>>>httpLCPollPushTest: Change Detected! (obj%d=%s):",
              pPollPush->nActiveTID,wmbObjectNamePtrGet(obj1));
            for (q=0;q<4;q++) printf("<%02x>",pBuffer0[q] & 0x0ff);
            printf("|");
            for (q=0;q<4;q++) printf("<%02x>",pBuffer1[q] & 0x0ff);
            printf("\n");
#endif

            /* If this is a binary/octet type, defer the lookup to the handler */
            /*  to allow string conversion */
            switch (wmbObjectTypeLocalGet(obj0))
                {
                case DATATYPE_OCTETS_K :
                case DATATYPE_OID_K   :
                case DATATYPE_MACADDR_K :
                case DATATYPE_ENUM_K :
                case DATATYPE_LIST_K :
                case DATATYPE_ACCESS_K :
                case DATATYPE_FUNCPTR_K :
                case DATATYPE_VOIDFUNCPTR_K :
                case DATATYPE_TIME_K :
                case DATATYPE_TICKS_K :
                case DATATYPE_COUNTER64_K :
                case DATATYPE_ROW_STATUS_K :
                case DATATYPE_OPAQUE_K :
                    pBuffer0 = NULL; valueLen0 = 0;
                    pBuffer1 = NULL; valueLen1 = 0;
                    break;
                }

            /* Notify clients of the new entry value !!! */
            if (0 == pPollPush->nActiveTID)
                status = httpLCNotify(wmbObjectNamePtrGet(obj0),pBuffer0, (ubyte4)valueLen0,HTTP_LC_PACKET_GET);
            else
                status = httpLCNotify(wmbObjectNamePtrGet(obj1),pBuffer1, (ubyte4)valueLen1,HTTP_LC_PACKET_GET);
            }

        }

    /* Switch the transaction banks */
    pPollPush->nActiveTID = nOtherTID(pPollPush->nActiveTID);

    /* no outside changes to the transaction list please */
#ifdef HTTP_LC_POLLING_SHOWBLOCKING /* Be quiet on the debugging consol */
    httpLCUnlock();
#else
    (void)wmosSemGive ( semLockLC );
#endif

    return status;
    }

/*******************************************************************************
*
* httpLCPollFindWMList - Find WindMark object one of the existing lists
*
* NOMANUAL
*
* RETURNS: WindMark's Object pointer if found as a member of this Polling list, or
*          NULL if not found in the list
*/

LOCAL WMB_OBJ_T  httpLCPollFindWMList
    (
    char          *szName,     /* WindMark name */
    LC_POLLPUSH_T *pPollPush,  /* The Polling list to search */
    ubyte4        nSelectTID,  /* The transaction list to search */
    ubyte4        nNameLength  /* length of root name (w/o instance) */
    )
    {
    WMB_OBJ_T     obj;

    /* loop through all objects in the group linked list */
    obj = wmbTransactionHeadGet(pPollPush->pTID[nSelectTID]);
    while (wmbObjectGood(obj))
        {
        /* find the matching name, ignore instance */
        if ((NULL != wmbObjectNamePtrGet(obj)) &&
            (0 == strncmp(szName,wmbObjectNamePtrGet(obj),nNameLength)) &&
            (nNameLength == strlen(wmbObjectNamePtrGet(obj))))
            return obj;

        obj = wmbObjectListNextGet(obj);
        }

    return NULL;
    }

/*******************************************************************************
*
* httpLCPollFindWM - Find WindMark Object one of the existing lists
*
* NOMANUAL
*
* RETURNS: Polling List where WindMark was found, or
*          NULL if not found in any polling list
*/

LOCAL LC_POLLPUSH_T *  httpLCPollFindWM
    (
    char *  szName,   /* WindMark name */
    ubyte4  nNameLength /* length of root name (w/o instance) */
    )
    {

    /* Look in each List for an object of this name */
    if (NULL != httpLCPollFindWMList(szName,pPollPushNone  ,0,nNameLength))
        return pPollPushNone;

    if (NULL != httpLCPollFindWMList(szName,pPollPushMedium,0,nNameLength))
        return pPollPushMedium;

    if (NULL != httpLCPollFindWMList(szName,pPollPushHigh  ,0,nNameLength))
        return pPollPushHigh;

    return NULL;
    }

/*******************************************************************************
*
* httpLCPollAddWM - Fetch and add WindMark Object to Polling Transaction
*
*
* NOMANUAL
*
* RETURNS: HTTP_OK if successful, or
*          HTTP_ERROR if WMB Object could not be created
*/

LOCAL HTTP_STATUS httpLCPollAddWM
    (
    sbyte*         szName,       /* WindMark name */
    ubyte4  nPriority     /* priority indicates which polling list */
    )
    {
    LC_POLLPUSH_T *pPollPush;
    WMB_OBJ_T     obj0;
    WMB_OBJ_T     obj1;
    sbyte         *pInstance,*pTemp;
    sbyte4        nInstanceLength;
    char          szNameBf[kMagicMarkupBufferSize];

    /* Make local copy of name */
    strncpy(szNameBf,szName,kMagicMarkupBufferSize);
    szNameBf[kMagicMarkupBufferSize-1] = '\0';
    szName = szNameBf;

    /* Extract any instance/argument field */
    pInstance       = "";
    nInstanceLength = 0;
    /* extract any instance/argument from WindMark name */
    if (NULL != (pInstance = strchr (szName, '(')))
        {
        *pInstance++ = '\0';

        /* remove closing paren, using szValue as temp pointer */
        if (NULL !=(pTemp = strrchr (pInstance, ')')))
            *pTemp = '\0';

        nInstanceLength = (sbyte4)strlen(pInstance);
        }
    else if (NULL != (pInstance = strchr (szName, '.')))
        {
        *pInstance++    = '\0';
        nInstanceLength = (sbyte4)strlen(pInstance);
        }

    /* is this WindMark already registered on one of the lists? */
    if (NULL != httpLCPollFindWM(szName, (ubyte4)strlen(szName)))
        {
#ifdef HTTP_LC_DEBUG
        printf("httpLCPollAddWM: Name '%s' is already added\n",szName);
#endif
        return HTTP_OK;
        }

    switch (nPriority)
        {
        case kPollPushNone   : pPollPush = pPollPushNone; break;
        case kPollPushHigh   : pPollPush = pPollPushHigh; break;
        case kPollPushMedium :
        default              : pPollPush = pPollPushMedium; break;
        }

    /* Create the two object records for the WindMark */
    if (OK != wmbObjectCreate(pPollPush->pTID[0], szName,pInstance, nInstanceLength, &obj0))
        {
#ifdef HTTP_LC_DEBUG
        printf("Could not add WM to PollPush0.\n");
#endif
        wmError (0, "Could not add WM to PollPush0.");
        return HTTP_ERROR;
        }

    if (OK != wmbObjectCreate(pPollPush->pTID[1], szName,pInstance, nInstanceLength, &obj1))
        {
#ifdef HTTP_LC_DEBUG
        printf("Could not add WM to PollPush1.\n");
#endif
        wmError (0, "Could not add WM to PollPush1.");
        return HTTP_ERROR;
        }

    /* If its WindMark type maps to STRING, then force the polling WMB lookups */
    /*  to string, so that non-LC types will be polled and most importantly    */
    /*  passed to LC as strings */
    if (HTTP_SYM_TYPE_STRING_ID == httpWMtoLCType(wmbObjectTypeNativeGet(obj0)))
        {
        wmbObjectTypeLocalSet(obj0,DATATYPE_STRING_K);
        wmbObjectTypeLocalSet(obj1,DATATYPE_STRING_K);
        }


#ifdef HTTP_LC_DEBUG
    printf("httpLCPollAddWM(%s) is successful\n",szName);
    /* httpLCPollPushDisplay(LCPOLLDISPLAY_BUF|LCPOLLDISPLAY_LCK); */
#endif

    return HTTP_OK;
    }


/*******************************************************************************
*
* httpLCPollMoveWM - Move WindMark Object from existing list to specified list
*
*
* NOMANUAL
*
* RETURNS: HTTP_OK if successful, or
*          HTTP_ERROR if WindMark entry could not be found
*/

HTTP_STATUS httpLCPollMoveWM
    (
    char *  szName,       /* name of WindMark */
    ubyte4  nNewPriority    /* priority indicates the new polling list */
    )
    {
    LC_POLLPUSH_T *pOldPollPush;
    LC_POLLPUSH_T *pNewPollPush;
    WMB_OBJ_T     obj0;
    WMB_OBJ_T     obj1;
    ubyte4 nameLength;
    char          *pInstance;

    /* Extract the WindMark name length without any instance/argument field */
    if (NULL != (pInstance = strchr (szName, '(')))
        nameLength = (ubyte4)(pInstance - szName);
    else if (NULL != (pInstance = strchr (szName, '.')))
        nameLength = (ubyte4)(pInstance - szName);
    else
        nameLength = (ubyte4)strlen(szName);


    /* Find the old Poll List, if any */
    if ( NULL == (pOldPollPush = httpLCPollFindWM(szName, nameLength)) )
        {
        wmError (0, "Could not find WM in Poll lists.");
        return HTTP_ERROR;
        }

    obj0 = httpLCPollFindWMList(szName, pOldPollPush, 0, nameLength);
    obj1 = httpLCPollFindWMList(szName, pOldPollPush, 1, nameLength);

    /* Get the new Poll List */
    switch (nNewPriority)
        {
        case kPollPushNone   : pNewPollPush = pPollPushNone; break;
        case kPollPushHigh   : pNewPollPush = pPollPushHigh; break;
        case kPollPushMedium :
        default              : pNewPollPush = pPollPushMedium; break;
        }

    /* Cut the objects from the original transaction lists */
    (void) wmbTransactionObjectRemove(pOldPollPush->pTID[0],obj0);
    (void) wmbTransactionObjectRemove(pOldPollPush->pTID[1],obj1);

    /* Add the objects to the new transaction lists */
    if (NULL != obj0)
        (void) wmbTransactionObjectAdd(pNewPollPush->pTID[0],obj0);
    if (NULL != obj1)
        (void) wmbTransactionObjectAdd(pNewPollPush->pTID[1],obj1);

#ifdef HTTP_LC_DEBUG
    printf("httpLCPollMoveWM(%s) is successful\n",szName);
#endif

    return HTTP_OK;
    }

/*******************************************************************************
*
* httpLCPollRemoveWM - Remove registered WindMarks Object from its Polling List
*
*
* NOMANUAL
*
* RETURNS: HTTP_OK if successful, or
*          HTTP_ERROR if WindMark could not be found
*/

LOCAL HTTP_STATUS httpLCPollRemoveWM
    (
    char *  szName      /* WindMark name */
    )
    {
    LC_POLLPUSH_T *pPollPush;
    WMB_OBJ_T     obj;
    ubyte4        nameLength;
    char          *pInstance;

    /* Extract the WindMark name length without any instance/argument field */
    if (NULL != (pInstance = strchr (szName, '(')))
        nameLength = (ubyte4)(pInstance - szName);
    else if (NULL != (pInstance = strchr (szName, '.')))
        nameLength = (ubyte4)(pInstance - szName);
    else
        nameLength = (ubyte4)strlen(szName);

    /* Find the old Poll List, if any */
    if ( NULL == (pPollPush = httpLCPollFindWM(szName, nameLength)) )
        {
        return HTTP_ERROR;
        }

    /* Delete the object from the A list */
    obj = httpLCPollFindWMList(szName, pPollPush, 0, nameLength);
    (void) wmbTransactionObjectRemove(pPollPush->pTID[0],obj);
    wmbObjectDelete(obj);

    /* Delete the object from the B list */
    obj = httpLCPollFindWMList(szName, pPollPush, 1, nameLength);
    (void) wmbTransactionObjectRemove(pPollPush->pTID[1],obj);
    wmbObjectDelete(obj);

#ifdef HTTP_LC_DEBUG
    printf("httpLCPollRemoveWM(%s) is successful\n",szName);
    httpLCPollPushDisplay(LCPOLLDISPLAY_BUF|LCPOLLDISPLAY_LCK);
#endif

    return HTTP_OK;
    }

/*******************************************************************************
*
* httpLCWindMarkType - Get the data type of registered WindMark
*
* NOMANUAL
*
* RETURNS: HTTP_OK if successful, or
*          HTTP_ERROR if WindMark could not be found
*/

HTTP_STATUS httpLCWindMarkType
    (
    char       *szName, /* WindMark name */
    DATATYPE_T *nType   /* Pointer to type variable to set */
    )
    {
    LC_POLLPUSH_T *pPollPush;
    WMB_OBJ_T     obj;
    ubyte4        nameLength;
    char          *pInstance;

    /* Extract the WindMark name length without any instance/argument field */
    if (NULL != (pInstance = strchr (szName, '(')))
        nameLength = (ubyte4)(pInstance - szName);
    else if (NULL != (pInstance = strchr (szName, '.')))
        nameLength = (ubyte4)(pInstance - szName);
    else
        nameLength = (ubyte4)strlen(szName);

    *nType = 0;

    /* Find the old Poll List, if any */
    if ( NULL == (pPollPush = httpLCPollFindWM(szName, nameLength)) )
        {
        wmError (0, "Could not find WM in Poll lists.");
        return HTTP_ERROR;
        }

    /* Find the object record from the A list */
    if (NULL == (obj = httpLCPollFindWMList(szName, pPollPush, 0, nameLength)))
        {
        wmError (0, "Could not find WM in Poll lists.");
        return HTTP_ERROR;
        }

    *nType = wmbObjectTypeNativeGet(obj);
    return HTTP_OK;
    }

/*******************************************************************************
*
* httpLCWindMarkIsWmbTable - Set flag if registered WindMark is controled by wmbTableLib
*                          - i.e. DB_FLAG_OID_COOKIE_K flag is set
*
* NOMANUAL
*
* RETURNS: HTTP_OK if successful, or
*          HTTP_ERROR if WindMark could not be found
*/

HTTP_STATUS httpLCWindMarkIsWmbTable
    (
    char  * szName,     /* WindMark name */
    BOOL  * bIsWmbTable,/* Pointer to type variable to set */
    BOOL  * bIsOidTable /* Pointer to type variable to set */
    )
    {
    LC_POLLPUSH_T *pPollPush;
    WMB_OBJ_T     obj;
    ubyte4        nameLength;
    char          *pInstance;

    /* Extract the WindMark name length without any instance/argument field */
    if (NULL != (pInstance = strchr (szName, '(')))
        nameLength = (ubyte4)(pInstance - szName);
    else if (NULL != (pInstance = strchr (szName, '.')))
        nameLength = (ubyte4)(pInstance - szName);
    else
        nameLength = (ubyte4)strlen(szName);

    *bIsWmbTable = FALSE;
    *bIsOidTable = FALSE;

    /* Find the old Poll List, if any */
    if ( NULL == (pPollPush = httpLCPollFindWM(szName, nameLength)) )
        {
        wmError (0, "Could not find WM in Poll lists.");
        return HTTP_ERROR;
        }

    /* Find the object record from the A list */
    if (NULL == (obj = httpLCPollFindWMList(szName, pPollPush, 0, nameLength)))
        {
        wmError (0, "Could not find WM in Poll lists.");
        return HTTP_ERROR;
        }

    *bIsWmbTable = (wmbEntry(wmbObjectEntryGet(obj))->table) != NULL;
    *bIsOidTable = (wmbDBEntryFlagGet(wmbObjectEntryGet(obj),DB_FLAG_OID_COOKIE_K));

    return HTTP_OK;
    }


/***************************************************************************
*
* httpLCErrorHandler - the main error handler of the server
*
* slightly different error handler for LiveControl connections;
*
* MUD_CONN_SOCKET_TIMEOUT, (from a select timeout) is not an error here.
* Keep the connection in this case.
*
* NOMANUAL
*
* RETURNS: CONN_CLOSE or CONN_KEEP
*/

MUD_CONN_ACTION httpLCErrorHandler
    (
    MUD_CONN_ID pConn
    )
    {
    MUD_CONN_STATUS mudConnStatus = MUD_CONN_INVALID;

#ifdef HTTP_LC_DEBUG
    printf ("*** httpLCErrorHandler() called ***\n");
#endif

    if  (OK != mudConnStatusGet (pConn, &mudConnStatus))
        {
        return (CONN_CLOSE);
        }

    switch (mudConnStatus)
        {
        case MUD_CONN_SYSTEM_LIMITS_EXCEEDED:
            /* !! Here send server busy message on connection !! */
            logInfo ("Maximum number of connections reached");
            return CONN_CLOSE;
            /*break;*/
        case MUD_CONN_CLOSED_BY_CLIENT:
            logInfo ("Connection closed by browser.  Closing server side too.");
            return CONN_CLOSE;
           /* break;*/
        case MUD_CONN_REQUESTS_PERMISSION_TO_CLOSE:
            logInfoMsg ("Dennied REQUEST_TO_CLOSE MUD_CONN %p\n", (int)pConn);
            return CONN_KEEP;
            /*break;*/
        case MUD_CONN_SOCKET_TIMEOUT:
#ifdef HTTP_LC_DEBUG
            printf ("*** httpLCErrorHandler() MUD_CONN_SOCKET_TIMEOUT, return CONN_KEEP ***\n");
#endif
            return CONN_KEEP;
            /*break;*/
        default:
            logInfoMsg ("Connection status number %d", mudConnStatus);
            return CONN_CLOSE;
           /* break;*/
        }
    }

#ifndef WEBCLI_ON_LINUX
void webcli_select_usleep (long usec)
    {
    struct timeval tm;

    tm.tv_sec = 0;
    tm.tv_usec = usec;

    select (0, NULL, NULL, NULL, &tm);
    }
#endif



/*******************************************************************************
*
* httpLCPthreadPollPushUblocker - spin on the timer semaphore for a given
*                                 PollPush structure. When we unblock,
*                                 mark the pollpush strict as ready, then
*                                 signal the counting semaphore for the
*                                 main LiveControl polling task.
*
*                                 A new Pthread is created to execute
*                                 this function.
*
* Args: a pointer to the pollpush structure.
*
* NOMANUAL
*
* RETURNS: void
*
*/

void httpLCPthreadPollPushUnblocker
    (
    void * cookie
    )
    {
    LC_POLLPUSH_T *pPollPush = (LC_POLLPUSH_T *) cookie;
    unsigned long sleeptime = 0;

#ifdef HTTP_LC_DEBUG
    printf ("pollpush thread started\n");
#endif

    while (bLCPollTimerThreadEnable)
        {
        if (pPollPush != NULL)
        	sleeptime = (unsigned long)(1000000L / 60) * pPollPush->nPollTime;
#ifdef WEBCLI_ON_LINUX
        usleep (sleeptime);
#else
        webcli_select_usleep ((long)sleeptime);
#endif

#ifdef HTTP_LC_DEBUG
        printf ("pollpush %ul unblocked\n", (unsigned long) pPollPush);
#endif

        if (pPollPush != NULL)
            {
            /* mark the pollpush as ready */
            pPollPush->bReady = TRUE;

            /* unblock the livecontrol push thread */
            if (semSyncLCPoll != NULL )
                (void)wmosSemGive ( semSyncLCPoll );
            }
        }
    }

/*******************************************************************************
*
* httpLCPollStartTimer - Trigger the watchdog timer for given PollPush
*
*
* NOMANUAL
*
* RETURNS: HTTP_OK if timer started successfully, or
*          HTTP_ERROR if timer failed or was not initialized
*/

LOCAL HTTP_STATUS httpLCPollStartTimer
    (
    LC_POLLPUSH_T * pPollPush /* Polling structure */
    )
    {
    return HTTP_OK;
    }

/*******************************************************************************
*
* httpLCPollStopTimer - Stop the watchdog timer for given PollPush
*
*
* NOMANUAL
*
* RETURNS: HTTP_OK if successful, or
*          HTTP_ERROR if watchdog timer failed or was not initialized
*/

LOCAL HTTP_STATUS httpLCPollStopTimer
    (
    LC_POLLPUSH_T * pPollPush /* Polling structure */
    )
    {
    return HTTP_OK;
    }

/*******************************************************************************
*
* httpLCPollTimerThread - The main thread for the LC Polling. This routine
* blocks on the wathdog timer handler, processes any work, and then restarts
* the respective timer.
*
* NOMANUAL
*
* RETURNS: void
*
*/

void httpLCPollTimerThread (void)
    {

#ifdef HTTP_LC_DEBUG
    printf("httpLCPollTimerThread is starting\n");
#endif

    /* Init semaphore for Polling */
    (void)wmosSemCreate (&semSyncLCPoll, 0, SEM_TYPICAL_K);

    if (NULL == semSyncLCPoll)
        {
        wmError (0, "httpLCPollTimerThread: No timer Mutex\n");
        return;
        }

    /* Start the timers */
    (void) httpLCPollStartTimer(pPollPushMedium);
    (void) httpLCPollStartTimer(pPollPushHigh  );

    /* loop until LC shutdown */
    while (bLCPollTimerThreadEnable)
        {
#ifdef  HTTP_LC_DEBUG
#ifdef  HTTP_LC_POLLING_SHOWBLOCKING
        printf("httpLCPollTimerThread is   blocked\n");
#endif
#endif
        /* wait for a timer to unblock the semaphore */
        wmosSemTake (semSyncLCPoll, WAIT_FOREVER_K);

#ifdef HTTP_LC_DEBUG
        printf("httpLCPollTimerThread is unblocked\n");
#endif

        if (!bLCPollTimerThreadEnable)
            break;

#ifdef  HTTP_LC_DEBUG
#ifdef  HTTP_LC_POLLING_SHOWBLOCKING
        printf("httpLCPollTimerThread is unblocked\n");
#endif
#endif


#ifndef _LC_NEW_SOCKET_CLOSE_
        /* Do we have connections to clean up? */
        if (0 < nHandlesToClose)
            {
            (void) httpLCLock();
            /* Remove the first marked Client */
            httpLCClientRemove( (MUD_CONN_ID) NULL );
            nHandlesToClose--;
            (void) httpLCUnlock();
            continue;
            }

#else
        /* Do we have connections to clean up? */
        while (NULL != httpLCSocketCloseGet())
            {
            httpLCLock();
            httpLCClientRemove( (MUD_CONN_ID) NULL );
            httpLCUnlock();
            }
#endif

        /* Find the timer that expired */
        if (pPollPushHigh->bReady)
            {
            (void) httpLCPollPushTest(pPollPushHigh);
            pPollPushHigh->bReady = FALSE;

            /* restart the timer */
            (void) httpLCPollStartTimer(pPollPushHigh);

            continue;
            }

        if (pPollPushMedium->bReady)
            {
            (void) httpLCPollPushTest(pPollPushMedium);
            pPollPushMedium->bReady = FALSE;

            /* restart the timer */
            (void) httpLCPollStartTimer(pPollPushMedium);
            continue;
            }
        }

    /* Destroy the semaphore here (the timers are already stopped) */
	if (semSyncLCPoll != NULL)
        {
		(void)wmosSemDestroy(semSyncLCPoll);
        semSyncLCPoll = NULL;
	    }

#ifdef HTTP_LC_DEBUG
    printf("httpLCPollTimerThread has exited\n");
#endif
    }

/*******************************************************************************
*
* httpLCPollPushInit - Initialize a specific LC Polling structure
*
* NOMANUAL
*
* RETURNS: HTTP_OK if initialization successfully, or
*          HTTP_ERROR if this poll struct could not be initialized
*/

LOCAL HTTP_STATUS httpLCPollPushInit
    (
    ubyte4        nPollTime,    /* polling time, in clock ticks */
    LC_POLLPUSH_T **ppPollPush, /* double pointer to Polling structure for alloc'ing */
    ubyte4        flags         /* Init flags (reserved) */
    )
    {
    OS_THREAD_T tid;

    if  (NULL == (*ppPollPush = memPoolAlloc ( httpGlobalMemPartIdGet() , sizeof(LC_POLLPUSH_T))) )
        {
        wmError (0, "Could not create LCPollPush.");
        return HTTP_ERROR;
        }

    memset(*ppPollPush,0,sizeof(LC_POLLPUSH_T));
    (*ppPollPush)->nPollTime  = nPollTime;
    (*ppPollPush)->nActiveTID = 0;
    (*ppPollPush)->bReady   = FALSE;

    /* Setup Polling transactions */
    if  (HTTP_OK != wmbTransactionCreate(pLCSession,0,WMB_TM_NOUNDO, &((*ppPollPush)->pTID[0])) )
        {
        wmError (0, "Could not create LCPollPush Transaction.");
        return HTTP_ERROR;
        }

    if  (HTTP_OK != wmbTransactionCreate(pLCSession,0,WMB_TM_NOUNDO, &((*ppPollPush)->pTID[1])) )
        {
        wmError (0, "Could not create LCPollPush Transaction.");
        return HTTP_ERROR;
        }

    if  (0 == nPollTime)
        {
        /*
         *  we're done ; no polling thread needs to be started for
         *  this pollpush
         */
        return HTTP_OK;
        }

    /* fire off a new PollPushUnblocker Pthread to handle this PollPush */
    if  (httpRpmLCMediumPoll == nPollTime)
        {
        if  (OK != wmosThreadCreate (httpLCPthreadPollPushUnblocker,
                                     "LCPollMedium",  /* no name */
                                     (void *) (*ppPollPush),
                                     0,   /* no relative prio */
                                     0,   /* no desired stacksize */
                                     &tid))
            {
            wmError (0, "Could not create LCPollPushUnblocker pthread");
            return HTTP_OK;  /* lets let the rest of the server run */
            }
        }

    if  (httpRpmLCHighPoll == nPollTime)
        {
        if  (OK != wmosThreadCreate (httpLCPthreadPollPushUnblocker,
                                     "LCPollHigh",  /* no name */
                                     (void *) (*ppPollPush),
                                     0,   /* no relative prio */
                                     0,   /* no desired stacksize */
                                     &tid))
            {
            wmError (0, "Could not create LCPollPushUnblocker pthread");
            return HTTP_OK;  /* lets let the rest of the server run */
            }
        }

    return HTTP_OK;
    }

/*******************************************************************************
*
* httpLCPollPushStartUp - Initialize the LC Polling
*
*
* NOMANUAL
*
* RETURNS: HTTP_OK if initialization successfully, or
*          HTTP_ERROR if one or more poll structs could not be initialized
*/

LOCAL HTTP_STATUS httpLCPollPushStartUp
    (
    ubyte4        flags   /* Init flags (reserved) */
    )
    {
    OS_THREAD_T tid;

    if (TRUE != bLCPollTimerThreadEnable)
        {
        /* start the polling task */
        bLCPollTimerThreadEnable = TRUE;

        if (HTTP_OK != httpLCPollPushInit(0,&pPollPushNone,flags))
            return HTTP_ERROR;

        if (HTTP_OK != httpLCPollPushInit(httpRpmLCMediumPoll,&pPollPushMedium,flags))
            return HTTP_ERROR;

        if (HTTP_OK != httpLCPollPushInit(httpRpmLCHighPoll,&pPollPushHigh,flags))
            return HTTP_ERROR;

        (void)wmosThreadCreate (httpLCPollTimerThread,
                          "LCPollTimer",
                          NULL,
                          0,
                          0,
                          &tid);
        }

#ifdef HTTP_LC_DEBUG
    printf("httpLCPollPushStartUp is successful\n");
#endif

  return HTTP_OK;
  }

/*******************************************************************************
*
* httpLCPollPushDestroy - Halt and Free a specific LC Polling structures
*
*
* NOMANUAL
*
* RETURNS: void
*
*/

LOCAL void httpLCPollPushDestroy
    (
    LC_POLLPUSH_T * pPollPush /* Polling structure */
    )
    {

    if (NULL != pPollPush)
    {
        /* Free Polling transactions */
        if (NULL != pPollPush->pTID[0])
            (void) wmbTransactionEnd(pPollPush->pTID[0]);
        if (NULL != pPollPush->pTID[1])
            (void) wmbTransactionEnd(pPollPush->pTID[1]);

        /* return to initial state */
        pPollPush = NULL;
    }

#ifdef HTTP_LC_DEBUG
    printf("httpLCPollPushDestory successful.\n");
#endif
    }

/*******************************************************************************
*
* httpLCPollPushShutdown - Halt and Free the LC Polling structures
*
*
* NOMANUAL
*
* RETURNS: void
*
*/

void httpLCPollPushShutdown(void)
    {
    /* Stop the timers */
    (void) httpLCPollStopTimer(pPollPushMedium);
    (void) httpLCPollStopTimer(pPollPushHigh  );

    /* Stop the polling thread */
    bLCPollTimerThreadEnable = FALSE;
    if (semSyncLCPoll != NULL )
        (void)wmosSemGive ( semSyncLCPoll );

#if defined(__VXWORKS_OS__) || defined(__VXWORKS_AE_OS__)
    (void)taskDelay (2 * (int) httpRpmLCMediumPoll / 60);
#elif defined(__POSIX_OS__)
    sleep(2 * (int) httpRpmLCMediumPoll / 60);
#endif


    httpLCPollPushDestroy(pPollPushNone  );
    httpLCPollPushDestroy(pPollPushMedium);
    httpLCPollPushDestroy(pPollPushHigh  );
    
    (void) wmbSessionDestroy(pLCSession);
    
    if (semLockLC != NULL)
        {
        (void)wmosSemDestroy(semLockLC);
        semLockLC = NULL;
        }
    if (semSyncLCPoll != NULL)
        {
        (void)wmosSemDestroy(semSyncLCPoll);
        semSyncLCPoll = NULL;
        }
    if (semCloseLC != NULL)
        {
        (void)wmosSemDestroy(semCloseLC);
        semCloseLC = NULL;
        }
    if ((subscribeListId != NULL)&&(subscribeListId->semLock != NULL))
        {
    	(void)wmosSemDestroy(subscribeListId->semLock);
    	subscribeListId->semLock = NULL;
        }
    if ((clientListId != NULL)&&(clientListId->semLock != NULL))
        {
        (void)wmosSemDestroy(clientListId->semLock);
        clientListId->semLock = NULL;
        }    
    }

#endif /* INCLUDE_HTTP_LIVE_CONTROL  : include Live Control Java processing */
#endif

