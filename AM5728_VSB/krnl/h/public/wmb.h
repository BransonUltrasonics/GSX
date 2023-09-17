/* wmb.h -  backplane header */

/*
 * Copyright (c) 2004-2005, 2010, 2012-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,31jan13,r_w  disable portfolio feature
01e,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01d,06dec12,shi  the original type should be struct  _WMB_BACKPLANE_T *.
                 (WIND00391996)
01c,30mar10,f_f  VxWorks 64bit support (WIND00193649)
01b,12oct05,wdz  '#ifdef __cplusplus' statements added
                 for SPR#98444
01a,19feb04,jws  add mod history and copyright, SPR92723
*/

#ifndef __INCwmbh
#define __INCwmbh
#ifdef __cplusplus
#include "wmos.h"
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
typedef struct  wmbResourceList         WMB_RESOURCE_LIST_T;
typedef struct  wmbResourceReg          WMB_RESOURCE_REG_T;
#else
typedef struct _WMB_RESOURCE_LIST_T     WMB_RESOURCE_LIST_T;
typedef struct _WMB_RESOURCE_REG_T      WMB_RESOURCE_REG_T;
#endif /* __cplusplus */

typedef struct _WMB_SESSION_T           WMB_SESSION_T;
typedef struct _WMB_TRANSACTION_T       WMB_TRANSACTION_T;

typedef VOIDFUNCPTR                     WMB_RES_DEREG_T;

typedef struct  WMB_OBJECT_T *          WMB_OBJ_T;

#ifdef __WM_DEBUG__
typedef struct _WMB_TRANSACTION_T *     WMB_TID_T;
typedef struct  WMB_COMPONENT_T *       WMB_CID_T;
typedef struct  _WMB_BACKPLANE_T *      WMB_ID_T;
#else
typedef void *  WMB_TID_T;
typedef void *  WMB_CID_T;
typedef void *  WMB_ID_T;
#endif

typedef int     WMB_HASH_HINT_T;
typedef int     WMB_ROLES_T;
typedef void    WMB_RES_DEREG_CTL_T;
typedef int     WMB_RESOURCE_ID_T;
typedef int     WMB_ARCHIVE_TYPE_T;
typedef int     WMB_CMD_T;

/* backwards compat for now */

typedef WMB_CMD_T COMMAND_T;

/* 1.1.3    Transaction Processing Structures */

typedef enum WMB_COMMANDS
    {
    CMD_INVALID_K,
    CMD_GET_K,
    CMD_NVMSET_K,
    CMD_SET_K,
    CMD_GET_NEXT_K,
    CMD_TEST_K,
    CMD_COMMIT_K,
    CMD_UNDO_K,
    CMD_COMPLETE_K,
    CMD_COPY_K,
    CMD_ACCESS_READ_K,
    CMD_ACCESS_WRITE_K,
    CMD_EXEC_K
    } CMD_T;

typedef STATUS (*WMB_HANDLER_T)(WMB_TID_T tid, WMB_OBJ_T obj, COMMAND_T cmd);

typedef struct WMB_ENTRY_T
    {
    sbyte *         pName;          /* rapidmark name           */
    sbyte4          localId;        /* offset for handlers      */
    void *          pData;          /* pointer to data storage  */
    DATATYPE_T      type;           /* data type                */
    sbyte *         pValid;         /* data validation string   */
    size_t          size;           /* size of storage needed   */
    ubyte4          accessRead;     /* read access settings     */
    ubyte4          accessWrite;    /* write access settings    */
    ubyte4          flags;          /* record flags             */
    WMB_HANDLER_T   handler;        /* data access handler      */
    struct WMB_TABLE_T *   table;   /* table this belongs to    */
    struct WMB_ENTRY_T *   sibling; /* next element in table    */
    }
    WMB_ENTRY_T;

/* generic IPC struct */

typedef struct MSG_IPC_T
    {
    sbyte4     msgCmd;     /* msg command, e.g., event, resolve, etc */
    sbyte4     objCmd;     /* object command, e.g., get/set, etc     */
#if 0
    OS_MUTEX_T mutex;      /* ipc semaphore                          */
#endif
    OS_SEM_T   sem;        /* ipc semaphore                          */
    OS_MSGQ_T  qid;        /* requestor's queue                      */
    WMB_TID_T  tid;        /* requesting tid                         */
    WMB_OBJ_T  obj;        /* requesting obj                         */
    WMB_CID_T  remoteCid;  /* remote component                       */
    WMB_TID_T  remoteTid;  /* remote tid                             */
    WMB_OBJ_T  remoteObj;  /* remote obj                             */
    sbyte4     name;       /* offset to shared name buf              */
    sbyte4     value;      /* offset to shared value buf             */
    sbyte4     status;     /* command result status                  */
    }
    MSG_IPC_T;

typedef struct WMB_TABLE_T
    {
    sbyte *     	pName;		/* name of the table */
    sbyte4      	indexCount;	/* number of indexes for this table */
    sbyte * *   	pIndexNames;/* table of index names */
    sbyte4      	memberCount;/* number of members in this table */
    WMB_ENTRY_T *	firstEnum;  /* pointer to first WM member of table */
    }
    WMB_TABLE_T;

#ifdef __cplusplus
typedef struct wmbTableList
#else
typedef struct _WMB_TABLE_LIST_T
#endif /* __cplusplus */
    {
    WMB_TABLE_T *   pRows;
    sbyte4          used;
    sbyte4          max;
    struct wmbTableList * pNext;
    }
    WMB_TABLE_LIST_T;


#include "memPoolLib.h"
#include "wmbConfig.h"
#include "wmbCommon.h"
#include "wmbCommonHandlers.h"
#include "wmbLockMgrLib.h"
#include "wmbDBLib.h"
#include "wmbObjectLib.h"
#include "wmbLib.h"
#include "wmbDataLib.h"
#include "wmXmlParser.h"

#include "wmbVACMAccessLib.h"
#include "wmbBackplaneLib.h"
#include "wmbComponentLib.h"
#include "wmbErrors.h"
#include "wmbEventLib.h"
#include "wmbSessionLib.h"
#include "wmbTransactionLib.h"
#include "wmbTableLib.h"
#if 0 /* disable portfolio feature */
#include "wmbPortfolioLib.h"
#endif
#include "wmCompareLib.h"

#ifdef __SNMP_API_ENABLED__
#include "wmbMibwayLib.h"
#endif

/*
 * Tables used to use two phases: get next on index, then appy instance exactly.
 * This behaviour is to be superceded by a single get next on the entire row.
 */

#ifndef __WMB_TABLE_SINGLE_PHASE__
#define __WMB_TABLE_TWO_PHASE__
#endif

#ifndef CUSTOM_INIT
#define CUSTOM_INIT(x)
#endif

#ifndef WMB_QUIT_K
#define WMB_QUIT_K  " Quit"
#endif

#define isQuit(buffer)          (0 == STRNCMP((sbyte *) buffer, WMB_QUIT_K, sizeof(WMB_QUIT_K)))
#define wmbQuit(buffer, len)    ((len == sizeof(WMB_QUIT_K)) && isQuit(buffer))

/* main entry point to WindManage */

STATUS wmMain(sbyte * pBackplane);

/*
 * 7.1.3.2  Resource Registration
 *
 * The resource registration structure is passed as an argument to the
 * resource registration function.  It contains a list of the resources to
 * register and a function to call to de-register this set of resources.
 * The de-registration function will be called if the backplane shuts down
 * or if the component is de-registered without previously de-registering
 * its resources.
 */

typedef VOIDFUNCPTR WMB_RESOURCE_HANDLER_T;

#ifdef __cplusplus
typedef struct wmbResourceList
#else
typedef struct _WMB_RESOURCE_LIST_S
#endif /* __cplusplus */
    {
    WMB_RESOURCE_LIST_T     *pNext;         /* Next entry in this resource list */
    WM_BUFFER_T              resName;       /* Name of the Resource*/
    ubyte4                  flags;          /* Flags for this object */
    WMB_TYPE_T              type;           /* Type of this object, when known */
    WMB_RESOURCE_HANDLER_T  *pResHndlr;     /* Handler to call for resource access */
    } _WMB_RESOURCE_LIST_T;

#ifdef __cplusplus
typedef struct wmbResourceReg
#else
typedef struct WMB_RESOURCE_REG_S
#endif /* __cplusplus */
    {
    void *  whatever;
    } _WMB_RESOURCE_REG_T;

/************************* BLADE STARTUP PROTOTYPES *******************/

/* actual "cli_start" function generated by WMIT */

/*
#ifndef __RCC_ENABLED__
#define cli_start()     OK
#endif

#ifndef __HTTP_ENABLED__
#define httpd()         OK
#endif
*/
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INCwmbh */

