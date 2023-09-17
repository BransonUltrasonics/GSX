/* wmbErrors.h - error codes for WindManage */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,12may12,vmk  added error message strings.
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbErrorsh
#define __INCwmbErrorsh


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


#endif /* ! __INCwmbErrorsh */
