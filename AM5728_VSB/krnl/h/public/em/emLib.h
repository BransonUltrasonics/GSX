/* emLib.h - Event Manager Component Methods. */

/* Copyright 1984-2002 Wind River Systems, Inc. */

#include "copyright_wrs.h"

/*
modification history
--------------------
01b,16feb06,ps  changes for webcli on linux
01a,11mar02,gdj  written.

*/

#ifndef __INCemLibh
#define __INCemLibh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */ 

#define __WEBCLI_OSCONF_NEED_MSGQLIB__
#define __WEBCLI_OSCONF_NEED_SEMLIB__
#include <webcli_osconf.h>

/* status codes */

/* 
 * The module number is chosen arbitrarily.  The em and its sub-components use
 * the module numbers 751 to 754.  It is advised that if another application
 * is using the same module number, either this or the other application's
 * module number be changed such that all the errno module numbers are unique
 * within the built image. 
 */
#define M_emLib (751 << 16) /* module number for event manager */

/* problem with remote request */
#define S_emLib_REMOTE_ENGINE_PROCESS_ERROR (M_emLib | 1)

/* similarly named producer/listener is already registered */
#define S_emLib_DUPLICATE_REGISTRATION (M_emLib | 2)

/* tried to register one more producer (or listener, or listeners/event, or
 * producers/event) than configuration allows */
#define S_emLib_ENTITY_LIST_OVERFLOW (M_emLib | 3)

/* specified id could not be found (events, producers, listeners) */
#define S_emLib_ENTITY_NOT_FOUND (M_emLib | 4)

/* if the request type does not have a define handler function */
#define S_emLib_INVALID_REQUEST_TYPE (M_emLib | 5)

/* an attempt was made to start an engine which was not loaded or initialized */
#define S_emLib_ENGINE_NOT_INSTALLED (M_emLib | 6)

/* defines */

#define EM_REGION "EM_EVENT_MANAGER_REGION"
#define EM_MGR_NAME_MAXLEN 128
#define MAX_EVENT_MANAGER_NAME_LENGTH EM_MGR_NAME_MAXLEN

/* typedefs */

/* listener priorities */
typedef enum emPriority_t
    {
    EM_PRIORITY_NORMAL,
    EM_PRIORITY_HIGH
    } EM_PRIORITY;

/* listener notification methods */
typedef enum emNotifyMethod_t
    {
    EM_NOTIFY_FUNC,
    EM_NOTIFY_Q
    } EM_NOTIFY_METHOD;

/*
 * the EM_ID is actually a pointer to the EM's base structure, while freeing it
 * would not stop an event manager from working, it would still be
 * unadvisable.
 */
typedef struct emEventManager * EM_ID;  /* EM_EVENT_MANAGER * */

/*
 * the following structure is ripc sensitive.  If the duplex q structure in
 * ripc is redefined in some way, that changed should be reflected here as
 * well.  The duplication is necessary to avoid inter-component (source and
 * header) dependencies at the customer level.
 */
typedef struct emDuplexQ
    {
    SEM_ID marshalSem;
    MSG_Q_ID requestQ;
    MSG_Q_ID responseQ;
    } EM_RIPC_DUPLEX_Q;

/*
 * Although this is a public structure, the contents should be considered
 * private, for use only by the event manager
 */
typedef struct emListenerContext
    {
    uint32_t entityId;         /* the EM-assigned id of the listener */
    EM_RIPC_DUPLEX_Q duplexQ;  /* the duplex queue to send request to the EM */
                               /*through */
    EM_ID eventManager;        /* listener's event manager */
    void * emHomePd;           /* event manager's PD (actual type PD_ID, use */
                               /* void * to avoid include path issues) */
    } EM_ENTITY_CONTEXT, EM_LISTENER_CONTEXT;

/*
 * Although this is a public structure, the contents should be considered
 * private, for use only by the event manager
 */
typedef struct emProducerContext
    {
    uint32_t entityId;         /* the EM-assigned id of the producer */
    EM_RIPC_DUPLEX_Q duplexQ;  /* the duplex queue to send request to the EM */
                               /* through */
    EM_ID eventManager;        /* producer's event manager */
    void * emHomePd;           /* event manager's PD (actual type PD_ID, use */
                               /* void * to avoid include path issues) */
    MSG_Q_ID acceptorQ;        /* the message queue to submit events on */  
    } EM_PRODUCER_CONTEXT;

/*
 * the contents of this structure may be modified directly, or through a
 * convenience method, emEventManagerConfigInit()
 */
typedef struct emManagerConfig
    {
    char * pManagerName;                /* the manager's name */
    uint32_t maxNumEvents;              /* max number of unique events the */
                                        /* manager will service */
    uint32_t maxEventPayloadSize;       /* maximum payload an event can carry */
                                        /* to be accepted for dispatching */
    uint32_t maxListenersPerEvent;      /* maximum number of listeners that */
                                        /* can listen to a specific event type */
    uint32_t maxProducersPerEvent;      /* maximum number of producers that */
                                        /* can listen to a specific event type */
    uint32_t maxNumListeners;           /* maximum number of listeners that */
                                        /* can register with the event manager */
    uint32_t maxNumProducers;           /* maximum number of producers that*/
                                        /* can register with the event manager */ 
    uint32_t namepoolSize;              /* total number of bytes for */
                                        /* name storage */
    uint32_t maxNumUniqueNames;         /* maximum number of unique */
                                        /* names that will register with the */
                                        /* EM (listeners,events,producers) */
    uint32_t maxEntityNameSize;         /* maximum number of bytes a */
                                        /* listener/producer/event name can */
                                        /* have (\0 not included) */
    uint32_t maxBacklogSubmittedEvents; /* how many events the EM can accept */
                                        /* without dispatching or losing any */
    uint32_t eventEnginePriority;       /* task priority for the event 
                                           dispatching engine */
    uint32_t remoteEnginePriority;      /* task priority for remote execution */
                                        /* engine (registration/querying) */
    BOOL strictChecking;                /* allow only registered producers */
                                        /* to dispatch registered events */
} EM_CONFIG_DATA;


typedef struct emEventData
    {
    uint32_t eventTypeId;               /* type id of the event */
    uint32_t producerId;                /* id of the producer that submitted */
                                        /* the event (assigned by EM) */
    uint32_t eventSubtype;              /* user defined uint32_t field to */
                                        /* qualify events of the same type */
    uint32_t payloadSize;               /* the number of bytes following */
                                        /* this structure that are the extra */
                                        /* payload of the event */
    uint32_t sequenceNum;               /* internal, for tracking of events */
                                        /* (assigned by EM) */
    } EM_EVENT_DATA;

/* function declarartions */

STATUS emLibInit(int maxManagers);

EM_ID emEventManagerCreate(EM_CONFIG_DATA* pEMConfig);

STATUS emEventManagerDestroy(EM_ID emId);

STATUS emEventManagerConfigInit(EM_CONFIG_DATA * pEMConfig,
                                char * pManagerName, uint32_t maxNumEvents,
                                uint32_t maxEventSize,
                                uint32_t maxListenersPerEvent,
                                uint32_t maxProducersPerEvent, 
                                uint32_t maxNumListeners,
                                uint32_t maxNumProducers,
                                uint32_t eventEnginePriority,
                                uint32_t remoteEnginePriority);

STATUS emProducerRegister(char * pProducerName, char * pManagerName,
                          EM_PRODUCER_CONTEXT * pProducerContext);

STATUS emProducerUnregister(EM_PRODUCER_CONTEXT * pProducerContext);

STATUS emProducerEventRegister(const EM_PRODUCER_CONTEXT * pProducerContext,
                               char * pEventName, uint32_t * pEventTypeId);

STATUS emProducerEventUnregister(const EM_PRODUCER_CONTEXT * pProducerContext,
                                 uint32_t eventTypeId);

STATUS emEventSubmit(EM_PRODUCER_CONTEXT * pProducerContext,
                     EM_EVENT_DATA * pEvent);

STATUS emListenerRegister(char * pListenerName, char * pManagerName,
                          EM_LISTENER_CONTEXT * pListenerContext);

STATUS emListenerUnregister(EM_LISTENER_CONTEXT * pListenerContext);

STATUS emListenerEventRegister(EM_LISTENER_CONTEXT * pListenerContext,
                               char * pEventName, EM_PRIORITY priority,
                               EM_NOTIFY_METHOD notifyMethod, void * notifier,
                               uint32_t * pEventTypeId);

STATUS emListenerEventUnregister(EM_LISTENER_CONTEXT * pListenerContext,
                                 uint32_t eventTypeId);

void emRegLibInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __INCemLibh */
