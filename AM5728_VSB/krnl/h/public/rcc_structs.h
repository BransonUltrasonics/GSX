/* rcc_structs.h */

/*
 * Copyright (c) 2002-2008, 2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17jun14,pca  added the isScriptRoot attribute to paramList nodes (V7MAN-98)
27may14,r_w  Eliminate dependency for V7MAN-62
19apr12,lan  add the variable specialcharflags (REQ:WIND00303792)
14sep11,m_z  merge WIND00238506 fix 
12jan10,m_z  reworking WIND00195306, ajust code for match and expand token
                 CQID: Fix WIND00195306
17dec08,m_z  enhance full order parameter check
                 (WIND00142732) 
07nov08,m_z  differentiate node locating when expand token and when execute
                 (WIND00140886)
13may08,m_z  fix "enter" key doesn't work on some telnet clients
                 (WIND00118285)
30apr08,m_z  fix for subnode "no" command handler invoke err(WIND00120787)
21mar07,msa  Fix defect 63849
07feb05,gsr  increase TOKEN_BLOCK_SIZE to 128 for SPR#106096
20feb04,jws  added copyright and mod history.
*/

#ifndef __RCC_STRUCTS_HEADER__
#define __RCC_STRUCTS_HEADER__

typedef ubyte4  BitMask;
typedef ubyte   optType;
typedef ubyte   optAction;
typedef optType SubOption;
typedef sbyte4  cliChar;
typedef ubyte4  paramID;

/* just in case no ordering flag at all */

#ifndef __RCC_PARAMETER_ORDER_NONE__
#ifndef __RCC_PARAMETER_ORDER_UNNAMED__
#ifndef __RCC_PARAMETER_ORDER_FULL__
#define __RCC_PARAMETER_ORDER_NONE__
#endif
#endif
#endif

/* cli version 3 used bitmasks for unordered handlers -- no more */

#if defined(__RCC_PARAMETER_ORDER_NONE__) && ! defined(__WINDMANAGE__)
#define __CLI_BITMASK__
#endif

#ifdef	__RCC_PARAMETER_ORDER_NONE__
# define kRCC_DEFAULT_ORDER     kRCC_FLAG_ORDER_NONE
# define PARAMETER_DEFAULT      DB_IsParameterOrderNone
#endif

#ifdef	__RCC_PARAMETER_ORDER_UNNAMED__
# define kRCC_DEFAULT_ORDER     kRCC_FLAG_ORDER_SOME
# define PARAMETER_DEFAULT      DB_IsParameterOrderUnnamed
#endif

#ifdef	__RCC_PARAMETER_ORDER_FULL__
# define kRCC_DEFAULT_ORDER     kRCC_FLAG_ORDER_FULL
# define PARAMETER_DEFAULT      DB_IsParameterOrderFull
#endif


#ifndef TOKEN_BLOCK_SIZE
#define TOKEN_BLOCK_SIZE    128
#endif

#ifndef PARAM_BLOCK_SIZE
#define PARAM_BLOCK_SIZE    TOKEN_BLOCK_SIZE
#endif

/* if aliasing implicitly disabled */

#if ( (kRCC_ALIAS_COUNT == 0) || (kRCC_ALIAS_BUFFER_SIZE == 0))
#define __NO_ALIASING__
#endif

typedef enum KEY_STATE
{
    KEY_STATE_INVALID,
    KEY_STATE_DATA,
    KEY_STATE_ESC,
    KEY_STATE_CURSOR,
    KEY_STATE_CONTINUE
} KEY_STATE;

/* for telnet negotiation */

typedef struct optionFlags
{
    optType    option;
    optAction  flag;
} optionFlags;

typedef enum TelState
{
    TS_Invalid,
    TS_No,
    TS_WantNo,
    TS_WantYes,
    TS_Yes
} TelState;

enum QueueState
{
    QUEUE_None,
    QUEUE_Empty,
    QUEUE_Opposite
};

typedef struct OptionState
{
    sbyte   name;
    ubyte   count;
    ubyte   optState;
    ubyte   queueState;
} OptionState;

typedef struct PairState
{
    optType     option;
    sbyte       desired;
    OptionState host;
    OptionState client;
} PairState;

/*-----------------------------------------------------------------------*/

typedef STATUS WriteHandle (WMB_SESSION_T *pEnv, sbyte *pBuf, sbyte4 BufSize);
typedef STATUS ReadHandle  (WMB_SESSION_T *pEnv, cliChar *charIn);
typedef STATUS SessionInit (WMB_SESSION_T *pEnv);



/* possible types of tokenNodes */
typedef enum tokenType
    {
    kTT_INVALID,
    kTT_NO,
    kTT_NODE,
    kTT_KEYWORD,
    kTT_ABSOLUTE,
    kTT_DATA,
    kTT_ARRAY_START,
    kTT_ARRAY,
    kTT_BAD_PARAM,
    kTT_END_COMMAND,
    kTT_STDIO_PIPE,
    kTT_STDIO_OUT,
    kTT_STDIO_IN,
    kTT_ASSIGN,
    kTT_OPERATOR,
    kTT_RVALUE,
    kTT_OPERAND,
    kTT_IPC,            /* receives next commands output as a parameter */
    kTT_ARG,            /* function argument */
    kTT_FILE            /* file used by stdio */
    } tokenType;

/* saved fragments for variable reassignment */

typedef struct textfrag
{
    struct textfrag *pNext;
    sbyte           *pString;
    sbyte4           used;
} textfrag;


/*-----------------------------------------------------------------------*/

/* storage for parsed input line  */

typedef struct tokenNode
{
    sbyte      *pStart;
    EditType    length;
    EditType    offset;
    void       *pData;
    ubyte4      flags;
    sbyte4      group;
    tokenType   type;
} tokenNode;


typedef struct tokenBlock_tag
{
    struct tokenBlock_tag   *pNext;
    tokenNode                tokens[TOKEN_BLOCK_SIZE];
} tokenBlock;

typedef struct tokenTable
{
    sbyte4              currentToken;       /* index to token               */
    sbyte4              numTokens;          /* total # tokens on line       */
    sbyte4              fork;               /* namespace collision index    */
    struct paramList   *pParamList;
    struct cmdNode     *pCurrentNode;
    sbyte4              offset;             /* offset into token block      */
    tokenBlock         *pBlock;             /* pool of tokens               */
    sbyte               buffer[kRCC_MAX_CMD_LEN + 1];
} tokenTable;

typedef STATUS (*HandlerFunction)(WMB_SESSION_T *pCliEnv,
                                 struct paramList *pParamList,
                                 sbyte *pOutputBuf);


/*-----------------------------------------------------------------------*/

/* structures generated by the RCIT */


typedef struct paramEntry
{
    paramID id;
    sbyte4  flags;
} paramEntry;


typedef struct oHandler
{
    sbyte4           flags;
    HandlerFunction  pHandlerFunc;
    sbyte4           paramCount;
    paramEntry      *pParams;
} oHandler;

typedef struct uHandler
{
    sbyte4           flags;
    HandlerFunction  pHandlerFunc;
    BitMask          reqdParamMask;
    BitMask          optlParamMask;
} uHandler;


#ifdef __CLI_BITMASK__
typedef uHandler handlerDefn;
#else
typedef oHandler handlerDefn;
#endif

typedef STATUS IsParameter(WMB_SESSION_T * pCliEnv, BOOL exact);

typedef struct paramDefn
{
    sbyte      *pKeyword;
    DATATYPE_T  type;
    paramID     id;
    ubyte4      flags;      /* general purpose...*/
    DTTypeInfo *pParamInfo;
} paramDefn;


/* used for tracking candidates for proper handler */

typedef struct handlerList
{
    struct handlerList *    pNext;      /* next in list             */
    handlerDefn *           pHandler;   /* possibly valid handler   */
    paramDefn *             pParam;     /* matching parameter       */
    sbyte4                  offset;     /* current param entry      */
    sbyte4                  index;      /* array offset             */
} handlerList;


typedef struct cmdNode
{
    sbyte          *pKeyword;
    sbyte          *pHelp;
    void           *pHelpHandler;
    ubyte4          flags;
    sbyte          *pPrompt;
    Access          accessLvl;
    sbyte2          numChildren;
    struct cmdNode *pChildren;
    sbyte2          numParams;
    paramDefn      *pParams;
    sbyte2          numHandlers;
    handlerDefn    *pHandlers;
} cmdNode;

/*-----------------------------------------------------------------------*/

typedef struct CmdHistBuff
{
    ubyte4   flags;
    sbyte4   bufSize;
    sbyte *  histCmd;
    struct CmdHistBuff * pPrev;
    struct CmdHistBuff * pNext;
} CmdHistBuff;

#define kRCC_HIST_RING   0x0001
#define kRCC_HIST_MODAL  0x0002

/*-----------------------------------------------------------------------*/

typedef struct HistInfo
{
    ubyte4  flags;
    sbyte4  bufferIndex;            /* points to [iNumCmds]              */
    sbyte4  iMaxHistCmds;	        /* max num hist cmds                 */
    sbyte4  iCurHistCmd;	        /* current hist cmd                  */
    sbyte4  iNumCmds;               /* total number of commands issued   */
    sbyte  *tempHist;               /* current command when scrolling    */
    CmdHistBuff *pFirst;
    CmdHistBuff *pLast;
    CmdHistBuff *pCurrent;
} HistInfo;


typedef struct paramDescr
{
    sbyte     *pValue;
    ubyte4     flags;
    sbyte4     position;
    sbyte2     arraySize;         /* number array elements        */
    sbyte2     arrayStart;        /* offset to first token        */
    struct paramDefn *pParamDefn;
} paramDescr;

/* parameter storage */

typedef struct paramBlock_tag
{
    struct paramBlock_tag   *pNext;
    paramDescr              params[PARAM_BLOCK_SIZE];
} paramBlock;



typedef struct paramList
{
    struct paramList   *pPrev;          /* previous entry in stack      */
    struct paramList   *pNext;          /* next in stack                */
    struct paramList   *pExec;          /* next to execute              */
    handlerDefn        *pHandler;       /* exec handler for this data   */
    cmdNode            *pCmdNode;       /* command node to execute      */
    cmdNode            *pTree;          /* current command tree         */
    sbyte4	            modeDepth;      /* how many nodes there are     */
    ubyte4              flags;	        /* if "no" token is given       */
    paramBlock         *pBlock;         /* starting block of params     */
    sbyte4              start;          /* offset into param block      */
    sbyte4              numParams;      /* number of parameters         */
    sbyte4              skippedParams;  /* Num skipped in full ordering */
    sbyte4              arguments;      /* count of tokens in func arg  */
    tokenTable         *pTokens;        /* list of tokens in command    */
    HistInfo           *pHistory;       /* modal history support        */
    CmdHistBuff        *pHead;          /* loop begining                */
    CmdHistBuff        *pLoop;          /* loop through history here    */
    sbyte              *pFileOut;       /* name of stdout file          */
    handlerList        *pPossible;      /* possibly valid handlers      */
    sbyte4              possibleCount;  /* count of possible handlers   */
    BOOL                isScriptRoot;   /* flag that this node is the root
                                           of a new parameter environment */
} paramList;

/*-----------------------------------------------------------------------*/

typedef struct CmdEditInfo
{
    EditType  lineLength;     /* character count in line buffer     */
    EditType  cursorPos;      /* offset of cursor into buffer       */
    EditType  termX;          /* horizontal pos of cursor           */
    EditType  termY;          /* vertical pos of cursor             */
    EditType  startCol;       /* track before exec handler          */
    EditType  startRow;       /* track before exec handler          */
    EditType  scroll;         /* scrolling long horiz line offset   */
    EditType  maxLines;       /* number of lines the cmd spans      */
    sbyte4    bufferSize;
    ubyte4    flags;
    sbyte    *pInputBuffer;
} CmdEditInfo;

/*-----------------------------------------------------------------------*/

typedef struct CmdAlias
{
    sbyte   *pName;
    sbyte   *pText;
} CmdAlias;

/*-----------------------------------------------------------------------*/

#ifndef __NO_ALIASING__

typedef struct AliasTable
{
    sbyte      *pAliasCmd;
    sbyte4      numEntries;
    sbyte4      bufferUsed;
    CmdAlias    alias[kRCC_ALIAS_COUNT];
    sbyte       data[kRCC_ALIAS_BUFFER_SIZE];
} AliasTable;

#endif

/*-----------------------------------------------------------------------*/

typedef struct LineOut
{
    ubyte4  flags;      /* output settings                      */
    sbyte4  indent;     /* indent from left                     */
    sbyte4  height;     /* "screen" height                      */
    sbyte4  width;      /* "screen" width                       */
    sbyte4  maxSize;    /* maximum buffer size                  */
    sbyte4  length;     /* length of buffer contents            */
    sbyte4  offset;     /* offset to text not yet printed       */
    sbyte4  stop;       /* don't print text after this point    */
    sbyte4  lineCount;  /* number of lines printed this time    */
    sbyte  *pBuffer;    /* output buffer                        */
} LineOut;

typedef struct helpInfo_tag
{
    BitMask flags;
    sbyte4  width;
    sbyte4  leader;
    sbyte * pTitle;
    sbyte * pPrefix;
    sbyte * pNode;
    sbyte * pDelimiter;
    sbyte * pQuote;
    sbyte * pUnquote;
} helpInfo;

/*-----------------------------------------------------------------------*/

typedef struct cliVar
    {
    struct cliVar * pNext;
    sbyte * pName;
    sbyte * pValue;
    sbyte4  valueSize;
    } cliVar;

/*-----------------------------------------------------------------------*/

#ifndef __DISABLE_CLI_INTERRUPT__

#define COLLECT_CHAR_MAX  128

#define READ_ESC_TASK_PRI  110

#ifndef _WRS_CONFIG_LP64
#define READ_ESC_TASK_STACK  2000
#else
#define READ_ESC_TASK_STACK  4000
#endif

#define EXEC_HANDLER_TASK_PRI 110

typedef enum intExecTypes_s
{
    INT_EXEC_BASIC,
    INT_EXEC_QUEUE,
    INT_EXEC_SCRIPT
} intExecTypes;

typedef struct intExecFuncArgs_s
{
    handlerDefn *pHandler;

    struct paramList *pParamList;
    sbyte *pOutputBuf;

    STATUS execRet;
    intExecTypes intType;
} intExecFuncArgs;

typedef struct cliIntExec_s
{
    sbyte4 sessionId;
    ubyte4 scriptNum; /* avoid spawning multiple tasks in scripting mode */

    OS_SEM_T semExec;

    OS_MUTEX_T semDelTask;

    OS_THREAD_T tidRunHandler;
    OS_THREAD_T tidReadEsc;

    intExecFuncArgs funcArgs;

    sbyte *collectBuf; /* the characters tReadEsc read other than ctrl+c ctrl+x */
    sbyte4 collectLen;
} cliIntExec;

#endif /* #ifndef __DISABLE_CLI_INTERRUPT__ */

/*-----------------------------------------------------------------------*/

typedef struct cli_info_tag
{
    sbyte4              ptty;           /* pseudo terminal id */
    ubyte4              flags;
    ubyte4              termflags;
    helpInfo            help;
    sbyte4              modeDepth;
    cmdNode            *pCurrentRoot;
    cmdNode            *pCmdNode;       /* currently evaluated/executing node */
    paramList          *pParamRoot;     /* root/head of param list */
    paramList          *pParamCurrent;  /* current/ "active" plist */
    paramList          *pParamExec;     /* param list currently executing */
    paramBlock         *pParamBlock;
    WMB_SESSION_T      *pEnvironment;
    #ifndef _WRS_CONFIG_LP64
    ubyte4              startTime;
    ubyte4              inputTime;
    #else
    ubyte8              startTime;
    ubyte8              inputTime;
    #endif
    sbyte               login[kRCC_MAX_LOGIN_LEN];
    sbyte               prompt[kRCC_MAX_PROMPT_LEN + 1];
    sbyte               promptTail[kRCC_MAX_PROMPT_TAIL_LEN];
    Length              promptLength;
    CmdEditInfo         cmdEditInfo;
#ifndef __NO_ALIASING__
    AliasTable          aliasTable;
#endif
    sbyte4              errorTextLen;
    sbyte               errorText[kRCC_ERROR_TEXT_SIZE + 1];
    sbyte              *pErrorMsg;
    sbyte              *matches[kRCC_MATCH_LIST_SIZE];
    PairState           optHandled[kRCC_MAX_OPT_HANDLED];
    optType             subOption;
    ubyte4              recvState;
    ubyte4              typeConn;
    sbyte4              timeOut;            /* seconds till session times out */
    WriteHandle        *pCliWriteHandle;
    ReadHandle         *pCliReadHandle;
    sbyte4              optBufferIndex;
    sbyte               optBuffer[kRCC_OPT_BUF_SIZE];
    sbyte               terminalName[kRCC_TERM_TYPE_SIZE];
    sbyte4              termType;
    EditType            screenWidth;
    EditType            screenHeight;
    sbyte4              errorPos;
    LineOut             output;
    int                 fd;
    sbyte4              keyState;
    IsParameter        *pfParamCheck;
    void               *mud;
    void               *pCustom;  /* customer defined */
#ifndef __DISABLE_CLI_INTERRUPT__
    cliIntExec         *pIntExec;
#endif
    cliVar             *pVars;
    textfrag           *pReader;
    sbyte4              readOffset;
    textfrag           *pFragments;
    sbyte4              fragmentTotal;
    sbyte4              sibling;        /* current command tree in use */
    FILE               *pStdOut;
    sbyte               preChar;
    sbyte               auxState;
    sbyte4              fOrderOffset;
    ubyte4             specialcharflags;
} cli_info;

/*-----------------------------------------------------------------------*/

typedef void  (*HelpHandler)(WMB_SESSION_T *, cmdNode *, paramDefn *);

/*-----------------------------------------------------------------------*/

/* node/handler/param flags */

#define kRCC_COMMAND_NO             0x00000001  /* can take no form             */
#define kRCC_COMMAND_MODE           0x00000002  /* intermediate mode allowed    */
#define kRCC_COMMAND_GLOBAL         0x00000004  /* global command               */
#define kRCC_COMMAND_META           0x00000008  /* used by help command         */
#define kRCC_COMMAND_NO_EXEC        0x00000010  /* don't execute                */
#define kRCC_COMMAND_FALLTHROUGH    0x00000020  /* try next node                */
#define kRCC_COMMAND_QUEUE          0x00000040  /* queue same handler execution */
#define kRCC_COMMAND_IGNORE         0x00000080  /* ignore extra parameters      */
#define kRCC_COMMAND_NO_CHAIN       0x00000100  /* execute only first node      */
#define kRCC_COMMAND_NO_TO_ALL      0x00000200  /* "no" applies to subnodes     */
#define kRCC_COMMAND_SHOW_NEXT      0x00000400  /* just show next help entry    */
#define kRCC_COMMAND_EXACT_NODE     0x00000800  /* sub nodes must match exactly */
#define kRCC_COMMAND_EXACT_PARAM    0x00001000  /* parames must match exactly   */
#define kRCC_COMMAND_STOP_HERE      0x00002000  /* don't go past this node      */
#define kRCC_COMMAND_BIND_PARAM     0x00004000  /* if parameters treat as leaf  */
#define kRCC_COMMAND_LEAF_ONLY      0x00008000  /* only execute leaf node       */
#define kRCC_COMMAND_OPERATOR         0x00010000  /* command is preceded by value */
#define kRCC_COMMAND_FUNCTION       0x00020000  /* command is a function        */
#define kRCC_COMMAND_               0x00040000  /*     */
#define kRCC_COMMAND_NO_LOOKUP      0x00080000  /* no automatic var lookup      */
#define kRCC_COMMAND_CUSTOM1        0x01000000  /* custom placeholder           */
#define kRCC_COMMAND_CUSTOM2        0x02000000  /* custom placeholder           */
#define kRCC_COMMAND_CUSTOM3        0x04000000  /* custom placeholder           */

#define kRCC_COMMAND_NOLINK         kRCC_COMMAND_NO_CHAIN
#define kRCC_COMMAND_LEAF_NODE      kRCC_COMMAND_LEAF_ONLY

#define kRCC_PARAMETER_OPTIONAL     0x0000  /* parameter is optional    */
#define kRCC_PARAMETER_REQUIRED     0x0001  /* parameter is required    */
#define kRCC_PARAMETER_NOKEYWORD    0x0002  /* parameter has no keyword */
#define kRCC_PARAMETER_ARRAY        0x0004  /* parameter is an array    */
#define kRCC_PARAMETER_IS_USED      0x0008  /* parameter used by handler*/
#define kRCC_PARAMETER_DEFAULT      0x0010  /* parameter used by handler*/
#define kRCC_PARAMETER_MARK         0x0020  /* parameter is a windmark  */
#define kRCC_PARAMETER_IPC          0x0040  /* parameter is ipc data    */

#define kRCC_HANDLER_NO         0x0001  /* for no form                  */

#define kRCC_AUXSTATE_EXPAND_TOKEN  0x01
#define kRCC_AUXSTATE_BANG_COMMAND  0x02

#define kPRINT_NO_ALLOC         0x0001  /* external buffer - don't free */

#define CLIENV(pEnv)        ((cli_info *) wmbSessionStateGet(pEnv))

#define WM_IS_SET(x, y, z)     ((x->z & y) == y)
#define WM_IS_CLEAR(x, y, z)   (! WM_IS_SET(x, y, z))
#define CLEAR_IT(x, y, z)   (x->z &= ~y)
#define SET_IT(x, y, z)     (x->z |=  y)

#ifndef __WINDMANAGE__

#define FLAG_SET(x, y)      WM_IS_SET(x, y, flags)
#define FLAG_CLEAR(x, y)    (! FLAG_SET(x, y))
#define CLEAR_FLAG(x, y)    CLEAR_IT(x, y, flags)
#define SET_FLAG(x, y)      SET_IT(x, y, flags)

#endif /* __WINDMANAGE__ */

#define TERM_SET(x, y)      WM_IS_SET(CLIENV(x), y, termflags)
#define TERM_CLEAR(x, y)    (! TERM_SET(x, y))
#define CLEAR_TERM(x, y)    CLEAR_IT(CLIENV(x), y, termflags)
#define SET_TERM(x, y)      SET_IT(CLIENV(x), y, termflags)

#define TOKEN_SET(x, y)      WM_IS_SET(x, y, pParamList->flags)
#define TOKEN_CLEAR(x, y)    (! FLAG_SET(x->pParamList, y))
#define CLEAR_TOKEN(x, y)    CLEAR_IT(x, y, pParamList->flags)
#define SET_TOKEN(x, y)      SET_IT(x, y, pParamList->flags)
#define SET_PRE_TOKEN(x, y)  SET_IT(x, y, pParamList->pPrev->flags)

#define HELP_SET(x, y)      WM_IS_SET(CLIENV(x), y, help.flags)
#define HELP_CLEAR(x, y)    (! HELP_SET(x, y))
#define CLEAR_HELP(x, y)    CLEAR_IT(CLIENV(x), y, help.flags)
#define SET_HELP(x, y)      SET_IT(CLIENV(x), y, help.flags)

#define SET_AUXSTATE(pEnv, flag)            SET_IT(CLIENV(pEnv), flag, auxState)
#define AUXSTATE_SET(pEnv, flag)            WM_IS_SET(CLIENV(pEnv), flag, auxState)
#define CLEAR_AUXSTATE(pEnv, flag)          CLEAR_IT(CLIENV(pEnv), flag, auxState)
#define AUXSTATE_CLEAR(pEnv, flag)          WM_IS_CLEAR(CLIENV(pEnv), flag, auxState)

#define IS_NO_COMMAND(x)   (FLAG_SET(x, kRCC_PFLAG_IS_NO_CMD))

#define kNoHandler	kRCC_HANDLER_NO

/*-----------------------------------------------------------------------*/


/* Set and check system "features" */
#define RCC_EnableFeature(pEnv, x)              SET_FLAG(CLIENV(pEnv), x)
#define RCC_DisableFeature(pEnv, x)             CLEAR_FLAG(CLIENV(pEnv), x)
#define RCC_IsEnabled(pEnv, x)                  FLAG_SET(CLIENV(pEnv), x)
#define RCC_NotEnabled(pEnv, x)                 FLAG_CLEAR(CLIENV(pEnv), x)

#define RCC_HELP_GET_M(pEnv, x)                 FLAG_SET(CLIENV(pEnv), x)
#define RCC_NotEnabled(pEnv, x)                 FLAG_CLEAR(CLIENV(pEnv), x)

/* helper access method/macros for edit session data */
#define MPRIN_OutputPtr(pEnv)                   (&(ENVOUT(pEnv)))
#define MPRIN_GetOutputBuffer(pEnv)             (ENVOUT(pEnv).pBuffer)
#define MPRIN_SetOutputBuffer(pEnv, x)          (ENVOUT(pEnv).pBuffer = x)
#define MPRIN_GetOutputBufferSize(pEnv)         (ENVOUT(pEnv).length)
#define MPRIN_GetOutputBufferMax(pEnv)          (ENVOUT(pEnv).maxSize)
#define MPRIN_SetOutputBufferMax(pEnv, x)       (ENVOUT(pEnv).maxSize = x)
#define MPRIN_AppendToOutput(pEnv, x)           (ENVOUT(pEnv).pBuffer[ENVOUT(pEnv).length++] = x)
#define MPRIN_BufferFull(pEnv)                  (ENVOUT(pEnv).length >= ENVOUT(pEnv).maxSize)

#define MPRIN_OutputLength(x)   FLAG_SET(x, kRCC_PRINT_MORE) ? x->stop - x->offset + 1 : pOutput->length - pOutput->offset;

#define ENVOUT(pEnv)                            (CLIENV(pEnv)->output)

#define MMISC_GetHelpPtr(pEnv)                 &(CLIENV(pEnv)->help)
#define MMISC_GetHelpInfo(pEnv)                 (CLIENV(pEnv)->help)
#define MMISC_SetHelpInfo(pEnv, X)              (CLIENV(pEnv)->help = X)
#define MCONN_GetTimeOut(pEnv)                  (CLIENV(pEnv)->timeOut)
#define MCONN_SetTimeOut(pEnv, X)               (CLIENV(pEnv)->timeOut = X)
#define MCONN_GetSubOption(pEnv)                (CLIENV(pEnv)->subOption)
#define MCONN_SetSubOption(pEnv, X)             (CLIENV(pEnv)->subOption = X)
#define MCONN_OptBufferPtr(pEnv)     (sbyte *) &(CLIENV(pEnv)->optBuffer)
#define MCONN_GetOptBufferIndex(pEnv)           (CLIENV(pEnv)->optBufferIndex)
#define MCONN_SetOptBufferIndex(pEnv, X)        (CLIENV(pEnv)->optBufferIndex = X)
#define MCONN_GetRecvState(pEnv)                (CLIENV(pEnv)->recvState)
#define MCONN_SetRecvState(pEnv, X)             (CLIENV(pEnv)->recvState = X)
#define MCONN_GetTermName(pEnv)                 (CLIENV(pEnv)->terminalName)
#define MCONN_SetTermName(pEnv, x)              STRNCPY(MCONN_GetTermName(pEnv), x, kRCC_TERM_TYPE_SIZE)
#define MCONN_GetTermType(pEnv)                 (CLIENV(pEnv)->termType)
#define MCONN_SetTermType(pEnv, x)              (CLIENV(pEnv)->termType = x)
#define MCHAN_Env(pChan)                        ((environment *)(pChan->env))
#define MCHAN_CliEnv(pChan)                     (MCHAN_Env(pChan)->pCli)
#define MMISC_GetOptHandled(pEnv)               (CLIENV(pEnv)->optHandled)
#define MSCRN_GetWidth(pEnv)                    (CLIENV(pEnv)->screenWidth)
#define MSCRN_GetHeight(pEnv)                   (CLIENV(pEnv)->screenHeight)
#define MSCRN_SetWidth(pEnv,  x)                (CLIENV(pEnv)->screenWidth  = x)
#define MSCRN_SetHeight(pEnv, y)                (CLIENV(pEnv)->screenHeight = y)
#define MEDIT_GetXPos(pEnv)                     (CLIENV(pEnv)->cmdEditInfo.termX)
#define MEDIT_SetXPos(pEnv, x)                  (CLIENV(pEnv)->cmdEditInfo.termX = x)
#define MEDIT_GetYPos(pEnv)                     (CLIENV(pEnv)->cmdEditInfo.termY)
#define MEDIT_SetYPos(pEnv, x)                  (CLIENV(pEnv)->cmdEditInfo.termY = x)
#define MEDIT_GetLength(pEnv)                   (CLIENV(pEnv)->cmdEditInfo.lineLength)
#define MEDIT_SetLength(pEnv, x)                (CLIENV(pEnv)->cmdEditInfo.lineLength = x)
#define MEDIT_GetScroll(pEnv)                   (CLIENV(pEnv)->cmdEditInfo.scroll)
#define MEDIT_SetScroll(pEnv, x)                (CLIENV(pEnv)->cmdEditInfo.scroll = x)
#define MEDIT_GetCursor(pEnv)                   (CLIENV(pEnv)->cmdEditInfo.cursorPos)
#define MEDIT_SetCursor(pEnv, y)                (CLIENV(pEnv)->cmdEditInfo.cursorPos = y)
#define MEDIT_GetBufPtr(pEnv)                   (CLIENV(pEnv)->cmdEditInfo.pInputBuffer)
#define MEDIT_SetBufPtr(pEnv, x)                (CLIENV(pEnv)->cmdEditInfo.pInputBuffer = x)
#define MEDIT_GetBufSize(pEnv)                  (CLIENV(pEnv)->cmdEditInfo.bufferSize)
#define MEDIT_SetBufSize(pEnv, x)               (CLIENV(pEnv)->cmdEditInfo.bufferSize = x)
#define MEDIT_Prompt(pEnv)                      (CLIENV(pEnv)->prompt)
#define MEDIT_GetPromptLen(pEnv)                (CLIENV(pEnv)->promptLength)
#define MEDIT_SetPromptLen(pEnv, x)             (CLIENV(pEnv)->promptLength = x)
#define MEDIT_PromptTail(pEnv)                  (CLIENV(pEnv)->promptTail)
#define MEDIT_GetIndent(pEnv)                   (ENVOUT(pEnv).indent)
#define MEDIT_SetIndent(pEnv, x)                (ENVOUT(pEnv).indent = x)
#define MEDIT_EditInfoPtr(pEnv)               (&(CLIENV(pEnv)->cmdEditInfo))
#define MHIST_TempBuf(hist)                     (hist->tempHist)
#define MCONN_GetWriteHandle(pEnv)              (CLIENV(pEnv)->pCliWriteHandle)
#define MCONN_GetReadHandle(pEnv)               (CLIENV(pEnv)->pCliReadHandle)
#define MCONN_SetWriteHandle(pEnv, X)           (CLIENV(pEnv)->pCliWriteHandle = X)
#define MCONN_SetReadHandle(pEnv, X)            (CLIENV(pEnv)->pCliReadHandle = X)
#define MCONN_GetConnType(pEnv)                 (CLIENV(pEnv)->typeConn)
#define MCONN_SetConnType(pEnv, X)              (CLIENV(pEnv)->typeConn = X)
#define MCONN_GetSock(pEnv)                     (pEnv->sock)
#define MCONN_GetInputTime(pEnv)                (CLIENV(pEnv)->inputTime)
#define MCONN_SetInputTime(pEnv, X)             (CLIENV(pEnv)->inputTime = X)
#define MCONN_GetStartTime(pEnv)                (CLIENV(pEnv)->startTime)
#define MCONN_SetStartTime(pEnv, X)             (CLIENV(pEnv)->startTime = X)
#define MCONN_GetKeyState(pEnv)                 (CLIENV(pEnv)->keyState)
#define MCONN_SetKeyState(pEnv, X)              (CLIENV(pEnv)->keyState = X)
#define MCONN_GetPtty(pEnv)                     (CLIENV(pEnv)->ptty)
#define MCONN_SetPtty(pEnv, X)                  (CLIENV(pEnv)->ptty = X)
#define MCONN_GetPreChar(pEnv)                  (CLIENV(pEnv)->preChar)
#define MCONN_SetPreChar(pEnv, X)               (CLIENV(pEnv)->preChar = X)
#define MMISC_MatchList(pEnv)                   (CLIENV(pEnv)->matches)
#define MMISC_GetEnv(pEnv)                      (CLIENV(pEnv)->pEnvironment)
#define MMISC_SetEnv(pEnv, X)                   (CLIENV(pEnv)->pEnvironment = X)
#define MMISC_GetCustom(pEnv)                   (CLIENV(pEnv)->pCustom)
#define MMISC_SetCustom(pEnv, X)                (CLIENV(pEnv)->pCustom = X)
#define MMISC_GetFD(pEnv)                       (CLIENV(pEnv)->fd)
#define MMISC_SetFD(pEnv, X)                    (CLIENV(pEnv)->fd = X)
#define MMISC_GetStdout(pEnv)                   (CLIENV(pEnv)->pStdOut)
#define MMISC_SetStdout(pEnv, X)                (CLIENV(pEnv)->pStdOut = X)
#ifndef __DISABLE_CLI_INTERRUPT__
#define MMISC_GetIntExec(pEnv)                  (CLIENV(pEnv)->pIntExec)
#define MMISC_SetIntExec(pEnv, X)               (CLIENV(pEnv)->pIntExec = X)
#endif
#define MMISC_GetVar(pEnv)                      (CLIENV(pEnv)->pVars)
#define MMISC_SetVar(pEnv, X)                   (CLIENV(pEnv)->pVars = X)
#define MMISC_GetCliNode(pEnv)                  (CLIENV(pEnv)->pCliNode)
#define MMISC_SetCliNode(pEnv, X)               (CLIENV(pEnv)->pCliNode = X)
#define MMISC_GetRootParam(pEnv)                (CLIENV(pEnv)->pParamRoot)
#define MMISC_SetRootParam(pEnv,pList)          (CLIENV(pEnv)->pParamRoot = pList)
#define MMISC_SetCurrParam(pEnv, pList)         (CLIENV(pEnv)->pParamCurrent = pList)
#define MMISC_GetExecParam(pEnv)                (CLIENV(pEnv)->pParamExec)
#define MMISC_SetExecParam(pEnv,pList)          (CLIENV(pEnv)->pParamExec = pList)
#define MMISC_GetModeDepth(pEnv)                (CLIENV(pEnv)->modeDepth)
#define MMISC_SetModeDepth(pEnv, depth)         (CLIENV(pEnv)->modeDepth = depth)
#define MMISC_GetParamCheck(pEnv)               (CLIENV(pEnv)->pfParamCheck)
#define MMISC_SetParamCheck(pEnv, pc)           (CLIENV(pEnv)->pfParamCheck = pc)
/*
#define MMISC_GetAccess(pEnv)                   (pEnv->UserLevel)
#define MMISC_SetAccess(pEnv, access)           (pEnv->UserLevel = access)
*/
#define MMISC_GetCurrRoot(pEnv)                 (CLIENV(pEnv)->pCurrentRoot)
#define MMISC_SetCurrRoot(pEnv, pNode)          (CLIENV(pEnv)->pCurrentRoot = pNode)
#define MMISC_PrevRootPtr(pEnv)                 (CLIENV(pEnv)->pPrevRoot)
#define MMISC_GetLogin(pEnv)                    (CLIENV(pEnv)->login)
#define MMISC_SetLogin(pEnv, x)                  STRNCPY(CLIENV(pEnv)->login, x, kRCC_MAX_LOGIN_LEN)
#define MMISC_AliasCmd(pEnv)                    (CLIENV(pEnv)->aliasTable.pAliasCmd)
#define MMISC_AliasPtr(pEnv)                   &(CLIENV(pEnv)->aliasTable)
#define MMISC_GetErrorPos(pEnv)                 (CLIENV(pEnv)->errorPos)
#define MMISC_SetErrorPos(pEnv, x)              (CLIENV(pEnv)->errorPos = x)
#define MMISC_GetErrorText(pEnv)                (CLIENV(pEnv)->errorText)
#define MMISC_SetErrorText(pEnv, msg)            STRNCPY(CLIENV(pEnv)->errorText, msg, kRCC_ERROR_TEXT_SIZE)
#define MMISC_GetErrorLen(pEnv)                 (CLIENV(pEnv)->errorTextLen)
#define MMISC_SetErrorLen(pEnv, x)              (CLIENV(pEnv)->errorTextLen = x)
#define MMISC_ValidGlobal(node, global)         (FLAG_SET(node, kRCC_COMMAND_GLOBAL) == global)
#define MMISC_GetNodePrompt(x)                  (NULL != x ? x->pPrompt : NULL)
#define MMISC_GetCmdNode(pEnv)                  (CLIENV(pEnv)->pCmdNode)
#define MMISC_SetCmdNode(pEnv, pNode)           (CLIENV(pEnv)->pCmdNode = pNode)
#define MMISC_OutputPtr(pEnv)                   (&(ENVOUT(pEnv)))
#define MMISC_OutputBuffer(pEnv)                (ENVOUT(pEnv).pBuffer)
#define MPARM_HasKeyword(x)                     (!(FLAG_SET(x, kRCC_PARAMETER_NOKEYWORD) | NULL_STRING(x->pKeyword)))
#define MEDIT_CopyFromInput(pEnv, dest)         STRCPY(dest, MEDIT_GetBufPtr(pEnv))
#define MMISC_GetSibling(pEnv)                  (CLIENV(pEnv)->sibling)
#define MMISC_SetSibling(pEnv, x)               (CLIENV(pEnv)->sibling = x)
#define MMISC_GetFullOrderOffset(pEnv)          (CLIENV(pEnv)->fOrderOffset)
#define MMISC_SetFullOrderOffset(pEnv, x)       (CLIENV(pEnv)->fOrderOffset = x)
#define MMISC_ResetFullOrderOffset(pEnv)        (CLIENV(pEnv)->fOrderOffset = 0)


#define envCwd(pEnv)   CLIENV(pEnv)->cwd

/* var reassignment */

#define MMISC_FragmentTotalGet(pEnv)            (CLIENV(pEnv)->fragmentTotal)
#define MMISC_FragmentTotalSet(pEnv, y)         (CLIENV(pEnv)->fragmentTotal = y)
#define MMISC_FragmentGet(pEnv)                 (CLIENV(pEnv)->pFragments)
#define MMISC_FragmentSet(pEnv, y)              (CLIENV(pEnv)->pFragments = y)
#define MMISC_FragmentReaderGet(pEnv)           (CLIENV(pEnv)->pReader)
#define MMISC_FragmentReaderSet(pEnv, y)        (CLIENV(pEnv)->pReader = y)
#define MMISC_FragmentOffsetGet(pEnv)           (CLIENV(pEnv)->readOffset)
#define MMISC_FragmentOffsetSet(pEnv, y)        (CLIENV(pEnv)->readOffset = y)

/* for temporary backwards compatability */

#define kRCC_FLAG_MORE      kRCC_TERM_MORE
#define kRCC_FLAG_HARDWRAP  kRCC_TERM_HARDWRAP
#define MMISC_Login         MMISC_GetLogin
#define MCONN_TermType      MCONN_GetTermName
#define MMISC_ExecNode      MMISC_GetCmdNode

#define SPCHAR_SET(x, y)      WM_IS_SET(CLIENV(x), y, specialcharflags)
#define SPCHAR_CLEAR(x, y)    (! SPCHAR_SET(x, y))
#define CLEAR_SPCHAR(x, y)    CLEAR_IT(CLIENV(x), y, specialcharflags)
#define SET_SPCHAR(x, y)      SET_IT(CLIENV(x), y, specialcharflags)


/*
#define MCONN_SetTermType   MCONN_SetTermName
#define MEDIT_BufPtr        MEDIT_GetBufPtr
#define MMISC_ExecNode      MMISC_GetCmdNode
#define MMISC_GetExecNode   MMISC_GetCmdNode
#define MMISC_SetExecNode   MMISC_SetCmdNode
*/
#endif /* ifndef __RCC_STRUCTS_HEADER__ */
