/* wmmwmbErrors.h - error codes for WindManage */

/* Copyright 2013 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,24jul13,r_w  create file.
*/

#ifndef __INCwmmwmbErrorsh
#define __INCwmmwmbErrorsh

/* Errors returned by the Transaction Management system. */

#define ERROR_TM_GENERAL                        -1400
#define ERROR_TM_TID_NOT_FOUND                  ( ERROR_TM_GENERAL - 1 )
#define ERROR_TM_NOT_INITIALIZED                ( ERROR_TM_GENERAL - 2 )
#define ERROR_TM_BAD_STATE                      ( ERROR_TM_GENERAL - 3 )
#define ERROR_TM_BAD_INPUT                      ( ERROR_TM_GENERAL - 4 )
#define ERROR_TM_HANDLER_NOT_FOUND              ( ERROR_TM_GENERAL - 5 )
#define ERROR_TM_BAD_CMD                        ( ERROR_TM_GENERAL - 6 )
#define ERROR_TM_NOT_HANDLED                    ( ERROR_TM_GENERAL - 7 )
#define ERROR_TM_INVALID_TRANSACTION            ( ERROR_TM_GENERAL - 8 )
#define ERROR_TM_INVALID_LOCAL_ID               ( ERROR_TM_GENERAL - 9 )
#define ERROR_TM_DEADLOCKED                     ( ERROR_TM_GENERAL - 10 )

/* Database errors */

#define ERROR_DB                                -1500
#define ERROR_DB_DUPLICATE_ENTRY                ( ERROR_DB - 1)
#define ERROR_DB_NO_DATABASE                    ( ERROR_DB - 2)

/* Errors returned by the Lock Management system. */

#define ERROR_LM_GENERAL                        -1600
#define ERROR_LM_ENTRY_NOT_FOUND                ( ERROR_LM_GENERAL - 1 )
#define ERROR_LM_LOCK_NOT_FOUND                 ( ERROR_LM_GENERAL - 2 )
#define ERROR_LM_LOCK_NOT_GRANTED               ( ERROR_LM_GENERAL - 3 )
#define ERROR_LM_INTENT_MODE_FAILED             ( ERROR_LM_GENERAL - 4 )
#define ERROR_LM_TABLE_NOT_FOUND                ( ERROR_LM_GENERAL - 5 )

/* Table errors */

#define ERROR_TABLE_GENERAL                     -1700
#define ERROR_TABLE_END_OF_TABLE                ( ERROR_TABLE_GENERAL - 1 )
#define ERROR_TABLE_INVALID_COLUMN              ( ERROR_TABLE_GENERAL - 2 )
#define ERROR_TABLE_COLUMN_BAD_SYNC             ( ERROR_TABLE_GENERAL - 3 )


/* Gerneral backplane errors */

#define ERROR_WMB_GENERAL                       -1800
#define ERROR_WMB_BACKPLANE_INVALID             ( ERROR_WMB_GENERAL - 1 )
#define ERROR_WMB_SESSION_INVALID               ( ERROR_WMB_GENERAL - 2 )
#define ERROR_WMB_OBJECT_INVALID                ( ERROR_WMB_GENERAL - 3 )
#define ERROR_WMB_COMPONENT_INVALID             ( ERROR_WMB_GENERAL - 4 )
#define ERROR_GENERAL_SESSION_NOT_FOUND         ( ERROR_WMB_SESSION_INVALID )
#define ERROR_GENERAL_OBJECT_NOT_FOUND          ( ERROR_WMB_OBJECT_INVALID )
#define ERROR_GENERAL_RESOURCE_UNAVAILABLE      ( ERROR_GENERAL_OBJECT_NOT_FOUND )


/* Error Message Strings */

/* Errors returned by the Transaction Management system. */

#define ErrMsg_ERROR_TM_TID_NOT_FOUND           "Transaction Id Not Found"
#define ErrMsg_ERROR_TM_NOT_INITIALIZED         "Transaction Not Initialised"
#define ErrMsg_ERROR_TM_BAD_STATE               "Transaction Bad State"
#define ErrMsg_ERROR_TM_BAD_INPUT               "Transaction Bad Input"
#define ErrMsg_ERROR_TM_HANDLER_NOT_FOUND       "Transaction Handler Not Found"
#define ErrMsg_ERROR_TM_BAD_CMD                 "Transaction Bad Command"
#define ErrMsg_ERROR_TM_NOT_HANDLED             "Transaction  Not Handled"
#define ErrMsg_ERROR_TM_INVALID_TRANSACTION     "Transaction Invalid"
#define ErrMsg_ERROR_TM_INVALID_LOCAL_ID        "Transaction Invalid Local Id"
#define ErrMsg_ERROR_TM_DEADLOCKED              "Transaction DeadLocked"

/* Database errors */

#define ErrMsg_ERROR_DB_DUPLICATE_ENTRY         "Database Duplicate Entry"
#define ErrMsg_ERROR_DB_NO_DATABASE             "No Database"

/* Errors returned by the Lock Management system. */

#define ErrMsg_ERROR_LM_ENTRY_NOT_FOUND         "Lock Entry Not Found"
#define ErrMsg_ERROR_LM_LOCK_NOT_FOUND          "Lock Not Found"
#define ErrMsg_ERROR_LM_LOCK_NOT_GRANTED        "Lock Not Granted"
#define ErrMsg_ERROR_LM_INTENT_MODE_FAILED      "Lock Intent Mode Failed"
#define ErrMsg_ERROR_LM_TABLE_NOT_FOUND         "Lock Table Not Found"

/* Table errors */

#define ErrMsg_ERROR_TABLE_END_OF_TABLE         "End of Table"
#define ErrMsg_ERROR_TABLE_INVALID_COLUMN       "Table Invalid Column"
#define ErrMsg_ERROR_TABLE_COLUMN_BAD_SYNC      "Table Column Bad Sync"


/* Gerneral backplane errors */

#define ErrMsg_ERROR_WMB_BACKPLANE_INVALID      "WMB Backplane Invalid"
#define ErrMsg_ERROR_WMB_SESSION_INVALID        "WMB Session Invalid"
#define ErrMsg_ERROR_WMB_OBJECT_INVALID         "Invalid WindMark Object"
#define ErrMsg_ERROR_WMB_COMPONENT_INVALID      "WMB Component Invalid"
#define ErrMsg_ERROR_GENERAL_SESSION_NOT_FOUND  "Session Not Found"
#define ErrMsg_ERROR_GENERAL_OBJECT_NOT_FOUND   "WindMark Object Not Found"
#define ErrMsg_ERROR_GENERAL_RESOURCE_UNAVAILABLE "Resource Unavailable"


#define kNoChange       0
#define kChange         1
#define wmbSessionIdGetWmm(pX)             (pX)->id

typedef struct _WMB_SESSION_T_WMM
    {
#ifndef __NO_MAGIC__
    ubyte4              magic;          /* data integrity */
#endif
    OS_SOCKET_T         sock;
    ubyte4              id;             /* unique id of this session */

    WMNET_IP_ADDR       wmnet_IpAddr;

    void *   pComponent;     /* component, e.g., CLI, WEB */
    void *              pInfo;          /* pointer to consumer specific state */
    void *              table;
    void *           tid;
    WM_ACCESS_T         access;         /* read/write security credentials */
#if defined __WMB_VIEW_ACCESS_MODEL__
    WMB_SESSION_VIEW_T  viewList;       /* List of views    */
#endif
    ubyte4              flags;          /* flags associated with session */
    void *        pPool;
#if 0
    sbyte *             variables[SESSION_MAX_ENV];
    Access              UserLevel;      /* TEMP!!! */
#endif
    void *              pSnmpData;      /* TEMP!!!! need different scheme */
#ifdef ENVOY_STACK_K
    void*               pIntSetSnmpPkt;
#endif
    void *          pDir;           /* session directory */
    sbyte4              type;           /* session type, e.g., CLI, WEB */
    sbyte4              fileIncludeLevel; /* recursive level for fileInclude windMark */
    void *              pBladeData;     /* cookie reserved for the current consumer blade        */
#ifdef __ENABLE_CUSTOM_STRUCT__
    void *              pCustomData;    /* custom pointer to bu used by the clients bo backplane */
#endif


#ifdef __ENABLE_RCB_SUPPORT__
	RC_ENVIRONMENT_T    pRcEnvironment; /* This structure supplies support for RC3x migration */
#endif
    sbyte4                clientIndex;
    sbyte                 clientAddr[64];
    } _WMB_SESSION_T_WMM;

typedef struct _WMB_SESSION_T_WMM           WMB_SESSION_T_WMM;

#define wmbSessionWmm(x)   ((WMB_SESSION_T_WMM *) x)

#endif /* ! __INCwmmwmbErrorsh */
