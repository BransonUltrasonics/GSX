/* retrofit.c  -- map WM to RC format for backward compatiblity */

/* Copyright 2003, 2010, 2014 Wind River Systems, Inc. */

/*
modification history
--------------------
04may14,f_f  fix static overrun(US35919)
17mar10,lan  VxWorks 64-bit support.(WIND00193649)
12oct05,wdz  '#ifdef __cplusplus' statement added
             for SPR#98444
26aug03,dlr  fixed default buffer size in wmbRcbXxx routines
06aug03,dlr  written
*/

#ifndef __INCretrofith
#define __INCretrofith
#ifdef __cplusplus
IMPORT "C" {
#endif

#ifdef __cplusplus 
#include "wmb.h"
#endif /* __cplusplus */

/* defines */

/* Enable any and all RCB Backwards Support Code */
#define __ENABLE_RCB_SUPPORT__

/* fix this value here, since the HTTP define is not globally available */
#define kRcbMacroRepeatNestMax	4

/* RapidMark delimiters ADVISORY CODE ONLY*/
#if 0
	#define RM_PAIR(a, b)               ((a * 0x100) + b)
	#ifdef __RL_FIXED_MM_DELIM__
	#  ifdef  __BRACKET_DELIMS__
	#    define kMagicMarkupStart       RM_PAIR('[', '[')
	#    define kMagicMarkupEnd         RM_PAIR(']', ']')
	#  else  /* default delimitators */
	#    define kMagicMarkupStart       RM_PAIR('$', '%')
	#    define kMagicMarkupEnd         RM_PAIR('#', '$')
	#  endif
	#else
	#  define kMagicMarkupStart         RM_PAIR(kMagicMarkupStartChar0, kMagicMarkupStartChar1)
	#  define kMagicMarkupEnd           RM_PAIR(kMagicMarkupEndChar0, kMagicMarkupEndChar1)
	#  define kMagicMarkupStartLen		2
	#  define kMagicMarkupEndLen		2
	#endif /* __RL_FIXED_MM_DELIM__ */

	#define kSizeofMagicMarkupStart     sizeof(sbyte2)
	#define kSizeofMagicMarkupEnd       sizeof(sbyte2)

	#ifdef __MACRO_REPEAT_NEST__
	#define kMaxMagicMarkupEndLen                   (40 + 40)
	#endif
#endif /* RapidMark delimiters ADVISORY CODE ONLY*/



/* rc_environ.c */

#define SESSION_CONSUMER_GET_M  wmbSessionStateGet
#define SESSION_CONSUMER_SET_M  wmbSessionStateSet

#define SYS_ERROR_SOCKET_TIMEOUT        ERROR_GENERAL_TIMEOUT

#define cliMudGet(pEnv)                 (CLIENV(pEnv)->mud)
#define cliMudSet(pEnv, X)              (CLIENV(pEnv)->mud = X)
#define cliSockGet(pEnv)                (CLIENV(pEnv)->sock)
#define cliSockSet(pEnv, X)             (CLIENV(pEnv)->sock = X)

#define cli_env WMB_SESSION_T


#define OS_SPECIFIC_MUTEX   OS_SEM_T
#define RLSTATUS            STATUS
#define RL_STATIC           LOCAL
#define list                WM_LIST_T
#define RC_MALLOC           wmMalloc
#define RC_FREE             wmFree
#define RC_CALLOC           wmCalloc

#ifndef	__BOOLEAN_DEFINED__
#define Boolean             BOOL
#endif

#define MEMORY_ALLOC_M(x)           ((x *) wmCalloc(sizeof(x), 1))
#define MemMgr_GetMemoryUsage()     memPoolUsed(wmMemoryGet())
#define MEMMGR_Debug                wmMemoryDebug
#define MEMMGR_DebugStdout          wmMemoryDebugStdout

/* ENVIRONMENT_GetEnvironment constants */
#define kenv_REQUEST_METHOD     0
#define kenv_DOCUMENT_NAME      1
#define kenv_ACCEPT             2
#define kenv_AUTHORIZATION      3
#define kenv_COOKIE             4
#define kenv_CONTENT_LENGTH     5
#define kenv_CONTENT_TYPE       6
#define kenv_CONNECTION         7
#define kenv_HOST               8
#define kenv_LANGUAGE           9
#define kenv_PRAGMA             10
#define kenv_REFERER            11
#define kenv_USER_AGENT         12
#define kenv_QUERY_STRING       13
#define kenv_HTTP_VERSION       14

#define ENVIRONMENT_GetBladeType            wmbSessionTypeGet
#define ENVIRONMENT_Construct(sock, env)    wmbSessionCreate(wmcComponentGet(), sock, 0, env)
#define ENVIRONMENT_Destruct(env)           wmbSessionDestroy(*(env))

#define MFILE_GetCwdPtr                     wmbSessionCwdGet
#define MFILE_GetDirHandle                  wmbSessionDirGet
/* from RC3.x */

/* constants */

#define kenv_REQUEST_METHOD     0
#define kenv_DOCUMENT_NAME      1
#define kenv_ACCEPT             2
#define kenv_AUTHORIZATION      3
#define kenv_COOKIE             4
#define kenv_CONTENT_LENGTH     5
#define kenv_CONTENT_TYPE       6
#define kenv_CONNECTION         7
#define kenv_HOST               8
#define kenv_LANGUAGE           9
#define kenv_PRAGMA             10
#define kenv_REFERER            11
#define kenv_USER_AGENT         12
#define kenv_QUERY_STRING       13
#define kenv_HTTP_VERSION       14

/* used for WSA */

#define kenv_REMOTE_ADDR		15
#define kenv_REMOTE_PORT		16
#define kenv_SERVER_SOFTWARE	17
#define kenv_SCRIPT_NAME		18

/* mibway */

#define MIBSRCH_FindByName          wmmOidStringByName
#define MIBSRCH_GetTypeByName       wmmTypeByName


/* rc_linklist */

#define List_Construct              wmLinkListConstruct
#define List_Size                   wmLinkListSize
#define List_GetNextObject          wmLinkListGetNextObject
#define List_Destruct               wmLinkListDestruct
#define pList_FindObject            wmLinkListFindObject
#define List_AddObject              wmLinkListAddObject
#define List_RemoveObject           wmLinkListRemoveObject
#define List_SelectiveRemoveObject  wmLinkListSelectiveRemoveObject
#define List_Destruct_Param         wmLinkListParamDestruct
#define List_GetFirstObject         wmLinkListGetFirstObject

void * BinSrch_FindObj (void * pArrayStart, sbyte4 numEntries,
                        ubyte4 sizeOfEntry, void * pObject,
                        CompareType   (*funcCompare)(void *pObject1,void *pObject2) );

/* rc_stdlib.h */

#define RC_MIN          MIN_M
#define RC_MAX          MAX_M
#ifndef ISWSPACE
#define ISWSPACE(c)		ISSPACE(c)
#endif

/* renamed to match posix vers */

#define ISALPHANUMERIC  ISALNUM

/* rc_database.h */

#define kDTinvalid      DATATYPE_INVALID_K
#define kDTmacro        DATATYPE_MACRO_K
#define kDTstring       DATATYPE_STRING_K
#define kDTipaddress    DATATYPE_IPV4_K
#define kDTinteger      DATATYPE_INTEGER_K
#define kDTchar         DATATYPE_CHAR_K
#define kDTshort        DATATYPE_SHORT_K
#define kDTlong         DATATYPE_LONG_K
#define kDTuinteger     DATATYPE_UINTEGER_K
#define kDTuchar        DATATYPE_UCHAR_K
#define kDTushort       DATATYPE_USHORT_K
#define kDTulong        DATATYPE_ULONG_K
#define kDTvoid         DATATYPE_VOID_K
#define kDTmacaddr      DATATYPE_MACADDR_K
#define kDTenum         DATATYPE_ENUM_K
#define kDTlist         DATATYPE_LIST_K
#define kDTabsolute     DATATYPE_ABSOLUTE_K
#define kDTnull         DATATYPE_NULL_K
#define kDTaccess       DATATYPE_ACCESS_K


/* rc_access.c */

#define ACCESS_ValidIpAddress       accessIpValid
#define RC_ACCESS_Allowed(x,y)      wmAccessReadOkay(y,x)

/* rc_ansifs.c */

#define  ANSIFS_Init				wmFileInit
#define  ANSIFS_ReadFile(x,y,z)	    wmFileReadFile(MFILE_GetDirHandle(x),y,z)
#define  ANSIFS_RetrieveFile		wmFileRetrieveFile
#define  ANSIFS_ReleaseFile			wmFileReleaseFile
#define  ANSIFS_RetrieveResource	wmFileRetrieveResource
#define  ANSIFS_ReleaseResource		wmFileReleaseResource

#define  ANSIFS_Chdir(x,y)		    wmFileChdir(MFILE_GetDirHandle(x), y)
#define  ANSIFS_OpenDir(x,y)		wmFileOpenDir(MFILE_GetDirHandle(x), y)
#define  ANSIFS_NextDir(x,y,z)		wmFileNextDir(MFILE_GetDirHandle(x), y, z)
#define  ANSIFS_CloseDir(x)			wmFileCloseDir(MFILE_GetDirHandle(x))

#define kANSIFS_DIRECTORY           FS_DIRECTORY_K
#define kANSIFS_READ_ONLY           FS_READ_ONLY_K

/* for session directories */
#ifndef kDIRECTORY_BUFFER_SIZE
#define kDIRECTORY_BUFFER_SIZE      DIRECTORY_BUFFER_SIZE_K
#endif

/* rc_environ.c */

#define environment             	WMB_SESSION_T

#define MMISC_EnvBladeGet(pEnv)     (pEnv)->type
#define MMISC_EnvBladeSet(pEnv,x)   (pEnv)->type = x

enum	BladeType
{
	kBTunknown,
	kBTwc,
	kBTocp,
	kBTcli,
    kBTwws          /* Wind Web Server */
};

/* rc_cache.c */

#define Cache_Construct             cacheConstruct
#define Cache_FlushTempObjects(x)

/* rc_database.c */

#define DB_CreateEntry              databaseCreateEntry
#define DB_CreateStrucDesc          databaseCreateStrucDesc

#define DB_ReadData(s, n, v, i)     wmbDataStringGet(s, n, i, 0, v, 0, NULL)
#define DB_WriteData(s, n, v, i)    wmbDataStringSet(s, n, i, 0, v, STRLEN(v))
#define DB_ElementType(n, t)        wmbDataType(n, ((DATATYPE_T *) (t)))

#define DB_QueryValue(s, n, v)      OK

typedef DATATYPE_T  DataType;

/* RC Indexing support definitions */
typedef struct RC_INDEXED_TABLE_S       /* support for indexed RC stucture based tables */
{	short nUniqueId;
    void*   (*p_funcStrucLoc )(WMB_SESSION_T *, UniqId);       /* return pointer to the object */
    void*   (*p_funcIndexNext)(WMB_SESSION_T *, void *, int);  /* only if an indexable element */
} RC_INDEXED_TABLE_T;

typedef struct RC_INDEXED_ELEMENT_S     /* (basename:  idx) */
{
    void*       		pDynamicLocation;		/* address of current structure */
    long         		Index;
    STATUS      		StateOfIndexedResource; /* has it been updated */
    RC_INDEXED_TABLE_T*	p_indexedTable;         /* object structure descriptor */
} RC_INDEXED_ELEMENT_T;


/* WMB to RC handlers support */
/* rcb1 handlers */

typedef void   (* WMB_RCB_READ  )(WMB_SESSION_T *, void * pBuffer, void * pData, char *pArg);
typedef STATUS (* WMB_RCB_WRITE )(WMB_SESSION_T *, void * pBuffer, void * pData);
typedef STATUS (* WMB_RCB_WRITE4)(WMB_SESSION_T *, void * pBuffer, void * pData, char *pArg);
typedef STATUS (* WMB_RCB_VALID )(WMB_SESSION_T *, void * pValue);

/* Shared RC CLI functions */

typedef WM_DIR_T    DIR_HANDLE;

#define RCC_DB_GetRootNode          	wmcRootNodeGet

#define RCC_USER_ValidateUser(x,y,z)    wmUserValidate(y,z)
#define RCC_USER_AddUser(x,y,z)         wmUserAdd(y,z)
#define RCC_USER_DeleteUser(x,y)        wmUserDelete(y)
#define RCC_USER_ChangePassword(x,y,z)  wmUserChangePassword(y,z)
#define RCC_USER_ChangeAccess(x,y,z)    wmUserChangeAccess(y,z)
#define RCC_USER_ShowUsers(x)           wmUserShowUsers()

#define RCC_TELNETD_Kill            	wmcDeregisterSelf
#define RCC_TELNETD_Done            	cliDone

/* rcc_rcb.c */

#define RCC_RCB_ValueGet(s,r,a,o,b,l)         wmbDataStringGet(s, r, a, 0, o, b,l)
#define RCC_RCB_ReadValueFromRCB(s,r,a,o,b,l) wmbDataStringGet(s, r, a, 0, o, b,l)
#define RCC_RCB_WriteValueToRCB(s, n, v, i)   wmbDataStringSet(s, n, v, 0, i, STRLEN(i))

/* for temp projects */
#define wmbUserValidate wmUserValidate


/*==============================================================*/

#if 0	/* RC CLI CODE - ADVISORY ONLY. Use as needed. */

	/* rcc_structs.h */

	#define cli_info                CLI_SESSION_T

	/* rcc_db.c */

	#define RCC_DB_DestroyEnvironment   cliSessionDestroy
	#define RCC_DB_Process_CLI          cliCommandProcess

	/* rcc_hist.c */

	#define RCC_HIST_InitHistInfo       cliHistoryInit
	#define RCC_HIST_AddHistLine        cliHistoryLineAdd
	#define RCC_HIST_ExecHistory        cliHistoryExec

	/* rcc_ext.c */

	#define RCC_EXT_Write           consoleWrite
	#define RCC_EXT_WriteStr        consoleWriteStr
	#define RCC_EXT_WriteStrLine    consoleWriteStrLine
	#define RCC_EXT_InsertText      consoleTextInsert
	#define RCC_EXT_PrintString     consolePrintString
	#define RCC_EXT_InitOutput      consoleOutputInit
	#define RCC_EXT_ResetOutput     consoleOutputReset
	#define RCC_EXT_GetPriorChar    consoleGetPriorChar
	#define RCC_EXT_FreeOutput      consoleOutputDestroy

	/* rcc_task.c */

	#define RCC_TASK_PrintPrompt    cliPromptPrint
	#define RCC_TASK_Cleanup        cliSessionDestroy

	/* rcc_util.c */

	#define RCC_UTIL_UpdatePrompt   cliPromptUpdate
	#define RCC_UTIL_Init           cliSessionInit


	/* remap constants */
	#define kRCC_FIXED_PORT             CLI_TELNET_PORT_K
	#define kRCC_MAX_CLI_TASK           CLI_MAX_SESSIONS_K
	#define	kRCC_THREAD_STACK_SIZE	    CLI_THREAD_STACK_SIZE_K
	#define	kRCC_SERVER_PRIO            CLI_SERVER_PRIORITY_K
	#define	kRCC_TIMEOUT                CLI_SESSION_TIMEOUT_SECS_K
	#define	kRCC_MAX_LOGIN_ATTEMPTS     CLI_MAX_LOGIN_ATTEMPTS_K
	#define	kRCC_MAX_LOGIN_LEN	        CLI_MAX_LOGIN_LEN_K
	#define	kRCC_MAX_PASSWORD_LEN       CLI_MAX_PASSWORD_LEN_K
	#define	kRCC_LOGIN_PROMPT	        CLI_LOGIN_PROMPT_K
	#define	kRCC_PASSWORD_PROMPT	    CLI_PASSWORD_PROMPT
	#define	kRCC_HISTORY_BUFFER_SIZE	CLI_HISTORY_BUFFER_SIZE_K
	#define	kRCC_DEFAULT_PROMPT         CLI_DEFAULT_PROMPT_K
	#define	kRCC_MODE_DEPTH             CLI_MODE_DEPTH_K
	#define	kRCC_MAX_TOKENS             CLI_MAX_TOKENS_K
	#define	kRCC_ALIAS_BUFFER_SIZE      CLI_ALIAS_BUFFER_SIZE_K
	#define	kRCC_ALIAS_COUNT            CLI_ALIAS_COUNT_K
	#define	kRCC_MAX_PROMPT_LEN         CLI_MAX_PROMPT_LEN_K
	#define	kRCC_MAX_CMD_LEN            CLI_COMMAND_BUFFER_LEN_K
	#define	kRCC_MAX_PARAMS             CLI_MAX_PARAMS_K
	#define	kOUTPUT_BUFFER_SIZE         CLI_OUTPUT_BUFFER_SIZE_K
	#ifdef CLI_CONSOLE_ENABLED_K
	#define	__RCC_CONSOLE_ENABLED__     CLI_CONSOLE_ENABLED_K
	#endif


	/* rcc_telnetd.c */

	#define RCC_TELNETD                 cliMain
	#define RCC_TELNETD_AddSession      daemonSessionAdd
#define RCC_TELNETD_NewChannel(x)   daemonChannelGet(NULL,x)

#endif /* RC CLI CODE - ADVISORY ONLY */


/*==============================================================*/

/* rc_sock.h */

/*
#define kThreadCreate       THREAD_STATUS_CREATE_K
#define kThreadIdle         THREAD_STATUS_IDLE_K
#define kThreadSnmpSync     THREAD_STATUS_SNMP_SNYC_K
#define kThreadWorking      THREAD_STATUS_WORKING_K
#define kThreadFinished     THREAD_STATUS_FINSISHED_K
#define kThreadDead         THREAD_STATUS_DEAD_K
*/
#define kThreadCreate       1
#define kThreadIdle         2
#define kThreadSnmpSync     3
#define kThreadWorking      4
#define kThreadFinished     5
#define kThreadDead         6

#define SOCKET_Initialize           wmnetInitialize
#define SOCKET_OpenServer           wmnetTcpServerOpen
#define SOCKET_Accept               wmnetTcpAccept
#define OS_SPECIFIC_SOCKET_HANDLE   OS_SOCKET_T
#define OS_SPECIFIC_DGRAM_HANDLE    OS_SOCKET_T

typedef struct UdpParams
{
    sbyte *pRecPacket;
    sbyte *pSendPacket;
   #ifdef _WRS_CONFIG_LP64
	  ubyte8 clientAddr;
    ubyte8 sendPacketLength;
    ubyte8 recvPacketLength;
   #else
    ubyte4 clientAddr;
    ubyte4 sendPacketLength;
    ubyte4 recvPacketLength;
   #endif
    ubyte2 clientPort;

} UdpParams;


/*==============================================================*/

#if 0  /* UDP: ADVISORY CODE ONLY */

	STATUS wmnetUdpSendTo         (OS_SOCKET_T sock, ubyte4 addr, ubyte2 port,
								   sbyte * pBuffer, ubyte4 bufSize);

	IMPORT RLSTATUS SOCKET_UdpSendTo(OS_SPECIFIC_SOCKET_HANDLE sock, UdpParams *pParams);

	IMPORT RLSTATUS SOCKET_UdpBindFixed
									(OS_SPECIFIC_SOCKET_HANDLE *sock, ubyte4 port);

	STATUS wmnetUdpBind           (OS_SOCKET_T * pSock, ubyte2 port);


	IMPORT RLSTATUS SOCKET_UdpReceive(OS_SPECIFIC_SOCKET_HANDLE soc, UdpParams *pParams);

	STATUS wmnetUdpRcvFrom        (OS_SOCKET_T sock, ubyte4 * pAddr, ubyte2 port,
								   sbyte * pBuffer, ubyte4 bufSize,
								   sbyte4 * pBytesRead);

#endif /* UDP CODE */

/*==============================================================*/


#define SOCKET_UdpBindFixed(s, p)   wmnetUdpBind(s, (ubyte2) p)
#define SOCKET_UdpSendTo(s, p)      wmnetUdpSendTo(s, p->clientAddr, p->clientPort, \
                                                   p->pSendPacket, p->sendPacketLength)

#define SOCKET_UdpReceive(s, p)     wmnetUdpRcvFrom(s, &p->clientAddr, p->clientPort, \
                                                    p->pRecPacket, p->recvPacketLength, \
                                                    &dummy_int)


/* rc_os_spec.h */

/* different degrees of errors */
#define kDebugError                 0
#define kWarningError               1
#define kAssertError                2
#define kSevereError                3
#define kUnrecoverableError         4

#define OS_SPECIFIC_SLEEP_TIME          wmosTimeSleep
#define OS_SPECIFIC_CREATE_THREAD       wmosThreadCreate
#define OS_SPECIFIC_GET_SECS            wmosTimeGet
#define OS_SPECIFIC_MUTEX_CREATE(x)     wmosSemCreate(x, 1, 0)
#define OS_SPECIFIC_MUTEX_RELEASE(x)    wmosSemGive(x)
#define OS_SPECIFIC_MUTEX_WAIT(x)       wmosSemTake(x, WAIT_FOREVER_K)
#define OS_SPECIFIC_SOCKET_READ(x,y,z)  wmnetTcpRead(x,y,z, (ubyte4 *) &dummy_int)
#define OS_SPECIFIC_SOCKET_WRITE        wmnetTcpWrite
#define OS_SPECIFIC_SOCKET_CLOSE        wmnetSocketClose
#define OS_SPECIFIC_SOCKET_ACCEPT       wmnetSocketAccept
#define OS_SPECIFIC_SOCKET_DATA_AVAILABLE(sock, timeout)   OK

#define OS_SPECIFIC_GET_ADDR(x)         0

#define SOCKET_Close                    wmnetSocketClose


/* rcm_snmp.c */


/* rc_convert.c */

#define CONVERT_GetEnumCount    wmConvertGetEnumCount
#define RC_CONVERT_Suffix       wmConvertSuffix
#define CONVERT_TypeToInfo      wmConvertTypeToInfo
#define CONVERT_GetDTProtoType  wmConvertGetDTProtoType
#define CONVERT_GetDTName       wmConvertDataTypeName
#define CONVERT_Valid           wmConvertValid
#define CONVERT_Validate        wmbValidate
#define CONVERT_EnumStr         wmConvertEnumStr
#define CONVERT_ListToStr       wmConvertListToStr
#define CONVERT_StrTypeTo       wmConvertStrTypeTo
#define CONVERT_AccessToStr(x,y,z)  wmAccessToStr(x, y)

/*
 * IT IS STONGLY ADVISED TO REPLACE THE USAGE OF THESE MACROS WITH THE PROPER
 * wmConvertStrTo and wmConvertStrTo, USING THE CORRECT BUFFER SIZE VALUES
 */
#define CONVERT_StrTo(s, d, t)  wmConvertStrTo(s, d, sizeof(*(d)), t)
#define CONVERT_ToStr(d,s,t)    wmConvertToStr(d, s, kMagicMarkupBufferSize, t)

/* Some string lengths to assist wmConvertToStr */
#define CONVERT_STRSIZ_IPV4		16
#define CONVERT_STRSIZ_INT		16
#define CONVERT_STRSIZ_SHORT	 6
#define CONVERT_STRSIZ_PHYS		20

/* rc_msghdlr.c */

/* Possible Message Types */
#define kInternalMessage        0
#define kCustomerMessage        1


/* retrofit.c IMPORT functions */

IMPORT void OS_SPECIFIC_LOG_ERROR
	(
	ubyte4 ErrorLevel, 	/* error level of this message */
	sbyte *ErrorMessage	/* error string */
	);

IMPORT int ENVIRONMENT_IsPostValid
	(
	WMB_SESSION_T *pSession
	);

IMPORT void ENVIRONMENT_SetPostInvalid
	(
	WMB_SESSION_T *pSession	/* The WMB session == RC Environment structure */
	);

IMPORT void ENVIRONMENT_SetPostValid
	(
	WMB_SESSION_T *pSession	/* The WMB session == RC Environment structure */
	);


/* Wrapper routines for RapidMark handler functions */

IMPORT STATUS wmbRcbGet
    (
    WMB_TID_T       tid,				/* transaction id */
    WMB_OBJ_T       obj,				/* first object in transaction */
    WMB_RCB_READ    func,				/* the RC READ call being wrapped */
	RC_INDEXED_TABLE_T* p_indexedTable	/* structure indexing information */
	);

IMPORT STATUS wmbRcbSet
    (
    WMB_TID_T       tid,				/* transaction id */
    WMB_OBJ_T       obj,				/* first object in transaction */
    WMB_RCB_WRITE   func,				/* the RC READ call being wrapped */
	RC_INDEXED_TABLE_T* p_indexedTable	/* structure indexing information */
    );

IMPORT STATUS wmbRcbSet4
    (
    WMB_TID_T       tid,				/* transaction id */
    WMB_OBJ_T       obj,				/* first object in transaction */
    WMB_RCB_WRITE4  func,				/* the RC READ call being wrapped */
	RC_INDEXED_TABLE_T* p_indexedTable	/* structure indexing information */
    );

IMPORT STATUS wmbRcbValid
    (
    WMB_TID_T       tid,				/* transaction id */
    WMB_OBJ_T       obj,				/* first object in transaction */
    WMB_RCB_VALID   func,				/* the RC READ call being wrapped */
	RC_INDEXED_TABLE_T* p_indexedTable	/* structure indexing information */
    );


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INCretrofith */

/* eof retrofit.h */
