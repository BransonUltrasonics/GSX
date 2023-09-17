/* wmbEventLib.h - WMB Event Manager interface definitions */

/* Copyright (c) 2002-2004, Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      'extern "C"' added for SPR#98444
01b,19jan04,jws  remove __WM_EVENT_MANAGER__ ifdefs
01a,15aug02,bk   written
*/

#ifndef __INCwmbEventLibh
#define __INCwmbEventLibh

#include "em/emLib.h"
#include "em/emEventEngineLib.h"


/* WMB_EVENT_T. This is the payload structure for the EM_EVENT_DATA structure. It contains
 * additional useful information for the listeners of WMB events
 */

typedef enum
    {
    WMB_EVENT_TYPE_CONTROL = 0,
    WMB_EVENT_TYPE_PORTFOLIO
    }WMB_EVENT_TYPE_T;

#define WMB_MAX_EVENT_TYPES_K    2

/* WMB Event Manager name   */

#define WMB_EM_NAME_K   "WMB Event Manager"

/* WMB Generated Event names    */

#define WMB_EVENT_TYPE_CONTROL_NAME_K   "WMB Event"
#define WMB_EVENT_TYPE_PORTFOLIO_NAME_K "WMB Portfolio Event"

typedef enum
    {
    WMB_EVENT_SEV_INFO = 0,
    WMB_EVENT_SEV_NOTICE,
    WMB_EVENT_SEV_WARNING,
    WMB_EVENT_SEV_ALARM,
    WMB_EVENT_SEV_DEBUG
    }WMB_EVENT_SEVERITY_T;

typedef struct _WMB_EVENT_T
    {
    WMB_CID_T               producer;           /* Producer of the event    */
    ubyte4                  eventTime;          /* Time that the event occurred (sysUpTime) */
    WMB_EVENT_TYPE_T        eventType;          /* What is the category of event    */
    WMB_EVENT_SEVERITY_T    eventSeverity;      /* Is this an error or information  */
    ubyte4                  eventID;            /* for each category, event numbering  */\
    ubyte4                  eventInfo[4];       /* 16 bytes of additional, context info */
    }WMB_EVENT_T;

typedef struct WMB_EM_EVENT_S
    {
    EM_EVENT_DATA   emEvent;
    WMB_EVENT_T     wmbEvent;
    }WMB_EM_EVENT_T;

/* Functions    */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

STATUS wmbEventManagerStart(void);

void wmbEventSubmit
    (
    WMB_CID_T           producer,       /* [in] Event Producer      */
    WMB_EVENT_TYPE_T    eventType,      /* [in] WMB event type      */
    ubyte4              eventId,        /* [in] Internal ID         */
    WMB_EVENT_SEVERITY_T severity,       /* [in] Event severity      */
    ubyte4              infoSize,       /* [in] Event info size     */
    ubyte4              *eventInfo      /* [in] Pointer to the additional info */
    );

STATUS wmbEventManagerStop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCwmbEventLibh */
