/* httpWmbMacro.c - Evaluate WMB macros */

/*
 * Copyright (c) 2003-2008 2010 2012-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
18apr16,r_w  fix mismatched prototype issue (V7MAN-182)
17feb16,r_w  fix compiler and static analysis warnings.
26sep14,r_w  fix static CHECKED_RETURN issue. 
05may14,r_w  fix static analysis issues(US35919)
08apr13,r_w  fix static analysis issues for defect (WIND00411995)
01mar12,r_w  eliminate compile warning for defect (WIND00335205)
28jul10,m_z  eliminate compile warning
02feb10,h_y  support for LP64  
30jun08,q_z  fix using REPEAT macro in pages sent via POST (WIND00039488)
14jan08,ps   allow underscores in windmarks used in IF and REPEAT macros.
05sep07,ten  Add include to allow building for linux target
23feb07,msa  Correct the apigen comments so they match the actual routines.
16feb06,ps   changes for webcli on linux
22apr05,dlr  Update from source, 4/22/05
25oct04,dlr  SPR #102900:remove debugging statement from check-in
22oct04,dlr  SPR #102900:Allow dynamic length to IF WindMark parameter values
19oct04,dlr  SPR #102700:Fixed IF parameter test to allow literal negative numbers
25feb04,dkg  Move struct stack_entry_tag and struct macro_stack_tag into
             httpWmbMacro.h to fix SPR 91625
16jan04,jws  remove per project dependencies
24sep03,jws  fix compiler warnings, SPR 90843
04aug03,dlr  added backwards support for RCB handlers - indexing counters
22jul03,adb  forcing parsing of "." and balanced "(" and ")" by parseVar
03jul03,jws  Change if(ISALNUM() == TRUE) to if(ALNUM())
07feb03,jws  fix compiler warning
07dic01,jc   initial creation.
*/

/*
DESCRIPTION

This module processes WMB html macros.

INCLUDE FILES:
    <string.h>
    <ctype.h>
    <stdio.h>
    <stdlib.h>

    "httpLib.h"
    "private/httpReq.h"

*/

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "commonCfg.h"
#include "httpCfg.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER

#ifdef WEBCLI_ON_LINUX
# include <errno.h>
#endif

#define DEBUG_ERROR

#include "httpLib.h"
#include "httpReq.h"

/* externals - will move to a header file */

IMPORT STATUS (*pRC_MACRO_INIT)(WMB_SESSION_T *);
IMPORT STATUS (*pRC_MACRO_CLEANUP)(WMB_SESSION_T *);
IMPORT STATUS (*pRC_MACRO_PUSH)(WMB_SESSION_T *,int);
IMPORT STATUS (*pRC_MACRO_DONE_REPEAT)(WMB_SESSION_T *,int);

IMPORT long  wmVarMagicMarkupBufferSize;


#define kMaxRepeatArgLen                     64

/* typedefs */

typedef enum macro_token_tag
    {
    MACRO_VAR_NONE   = 0x0000,  MACRO_VAR_NOHINT = 0x0000,
    MACRO_VAR_VARMSK = 0x0100,  MACRO_VAR_INT    = 0x0101,
    MACRO_VAR_STR    = 0x0102,  MACRO_VAR_STXMSK = 0x0200,
    MACRO_VAR_BEGIN  = 0x0201,  MACRO_VAR_ELSE   = 0x0202,
    MACRO_VAR_END    = 0x0203,  MACRO_VAR_COMMA  = 0x0204,
    MACRO_VAR_LPAREN = 0x0205,  MACRO_VAR_RPAREN = 0x0206,
    MACRO_VAR_EQUMSK = 0x0400,  MACRO_VAR_EQUEQU = 0x0401,
    MACRO_VAR_EQU    = 0x0402,  MACRO_VAR_LT     = 0x0403,
    MACRO_VAR_LTE    = 0x0404,  MACRO_VAR_GT     = 0x0405,
    MACRO_VAR_GTE    = 0x0406,  MACRO_VAR_NE     = 0x0407
    } MACRO_TOKEN;

typedef enum macro_type_tag
     {
     INVALID_MACRO = 0,
     REPEAT_MACRO ,
     IF_MACRO ,
     END_ROW_MACRO,
     CREATE_TABLE_MACRO,
     END_TABLE_MACRO
     } MACRO_TYPE;

typedef struct macro_map_tag
    {
    sbyte *    identifier;
    MACRO_TYPE type;
    } MACRO_MAP;


typedef struct macro_var_tag
	{
    MACRO_TOKEN    var_type;
    int            var_length;
    int            var_int;
	sbyte          *var_str;
    sbyte          lcl_str[kMaxRepeatArgLen + 1];	/* default buffer for var_str */
	} MACRO_VAR;


/* locals */

MACRO_MAP httpMacros[] =
    { { "REPEAT",      REPEAT_MACRO},
      { "IF",          IF_MACRO},
      { "endRow",      END_ROW_MACRO},
      { "createTable", CREATE_TABLE_MACRO},
      { "endTable",    END_TABLE_MACRO},
      { NULL ,         INVALID_MACRO}
    };


/* prototypes */

LOCAL STATUS stackPush (MACRO_STACK_ID stack, ubyte4 startAddr,
                           ubyte4 remaining);
LOCAL STATUS stackPop (MACRO_STACK_ID stack, ubyte4 * startAddr,
                          ubyte4 * remaining);
LOCAL BOOL stackDoneRepeat (MACRO_STACK_ID stack, ubyte4 * loopStartAddr);

LOCAL STATUS parseVar (WMB_SESSION_T * pSession, sbyte *pArg,
                       MACRO_VAR * pVar,int hint);

LOCAL STATUS parseRepeatArgs(WMB_SESSION_T * pSession, sbyte *pArgs,
                  MACRO_TOKEN *beginEnd, int *Start, int *End);

LOCAL ubyte4 stackStartAddrGet (MACRO_STACK_ID stack);

LOCAL ubyte4 stackRemainingGet (MACRO_STACK_ID stack);
LOCAL STATUS stackRemainingSet (MACRO_STACK_ID stack, ubyte4);
LOCAL STATUS parseIfArgs (WMB_SESSION_T * pSession, sbyte *pArgs,
                          MACRO_TOKEN * beginEndElse, BOOL * result);
LOCAL STATUS parseCreateTableArgs (WMB_SESSION_T * pSession, sbyte *pArgs,
                                   sbyte ** pTableName);
LOCAL STATUS parseEndArgs (MACRO_TYPE     rowOrTable, WMB_SESSION_T * pSession,
                           sbyte * pArgs);


/**************************************************************************
*
* lclHttpVarInit - Initialize a MACRO variable structure.
*
* NOMANUAL
*/

LOCAL void lclHttpVarInit
	(
    MACRO_VAR *pVar		/* the var object */
    )
    {
    pVar->var_type   = MACRO_VAR_NONE;
    pVar->var_length = 0;
    pVar->var_int    = 0;
    pVar->var_str    = pVar->lcl_str;	/* by default, point to the local buffer */
    pVar->var_str[0] = '\0';
	}

LOCAL void lclHttpVarFree
	(
    MACRO_VAR *pVar		/* the var object */
    )
    {
    pVar->var_type   = 0;

	/* free any temp alloc'ed buffer */
	if (pVar->var_str != pVar->lcl_str)
		wmFree(pVar->var_str);
	}

LOCAL STATUS lclHttpVarSetBuffer
	(
    MACRO_VAR     *pVar,		/* the var object */
    char          *sVal,		/* source string */
    size_t        nLength		/* buffer length (without string terminator) */
    )
    {
	STATUS status = OK;

	/* free any previous alloc'ed buffer */
	if (pVar->var_str != pVar->lcl_str)
		{
		wmFree(pVar->var_str);
		pVar->var_str = pVar->lcl_str;
		}

	if (kMaxRepeatArgLen <= nLength)
		{
		/* alloc a temp buffer to be freed later */
	    if (NULL == (pVar->var_str = (char *) wmMalloc((ubyte4)nLength+1)))
	    	{
			pVar->var_str = pVar->lcl_str;
			nLength       = kMaxRepeatArgLen;
			status        = ERROR_GENERAL;
			}
		}

	STRNCPY(pVar->var_str,sVal,nLength);
	pVar->var_str[nLength] = '\0';

	return status;
	}

LOCAL STATUS lclHttpVarSetString
	(
    MACRO_VAR *pVar,	/* the var object */
    char      *sVal		/* source string */
    )
    {
	return (lclHttpVarSetBuffer(pVar, sVal, strlen(sVal)));
	}


/**************************************************************************
*
* httpIsMacro - True if the WindMark is HTTP specific.
*
* This routine checks if the called WindMark is a macro. It returns a
* non-zero value if it is a macro, and a non-zero value otherwise.
*
* INTERNAL: The non-zero value returned is one of the enumerated
* MACRO_TYPES. It is not exported since it is only used by this module.
*
* RETURNS: non-zero if <pMark> is recognized as a macro
*          0 otherwise
*
* NOMANUAL
*/

sbyte4 httpIsMacro
    (
    sbyte * pMark   /* WindMark Pointer */
    )
    {
    sbyte4 i = -1;
    sbyte * pStartPos;

    if (NULL == pMark) return INVALID_MACRO;
    while (httpMacros[++i].identifier)
       {
       pStartPos = strstr(pMark, httpMacros[i].identifier);
       if ((NULL != pStartPos) && (pStartPos == pMark))
           return httpMacros[i].type;
       }
    return INVALID_MACRO;
    }

/**************************************************************************
*
* httpMacroInit - basic stack customized for Repeat loop tracking
*
* This routine initializes a basic stack customized for Repeat loop
* tracking.
*
* RETURNS: NULL or MACRO_STACK_ID
*
* NOMANUAL
*/
STATUS httpMacroInit
    (
    WMB_SESSION_T * pSession, /* Backplane session pointer. */
    ubyte4 size          /* Maximum level of nesting. See wm_options.h */
    )
    {
    HTTP_REQ_ID reqId;

    if ((NULL == pSession) ||
        (NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(pSession))))
        return wmError(ERROR_GENERAL, "invalid parameters");

    if (pSession->fileIncludeLevel > 1 )
        return OK;

    reqId->repeatStack = NULL;
    reqId->ifStack = NULL;

    /* create REPEAT stack */
    reqId->repeatStack = (MACRO_STACK_ID) memPoolAlloc(reqId->memPool, sizeof(MACRO_STACK));

    if (NULL == reqId->repeatStack)
        return wmError(ERROR_GENERAL, "could not alloc REPEAT stack");

    reqId->repeatStack->entry = memPoolAlloc(reqId->memPool, size * sizeof(STACK_ENTRY));

    if (NULL == reqId->repeatStack->entry)
        {
       (void) memPoolFree(reqId->memPool, reqId->repeatStack);
        reqId->repeatStack = NULL;
        return wmError(ERROR_GENERAL, "could not alloc REPEAT stack");
        };
    reqId->repeatStack->size = size;
    reqId->repeatStack->count = 0;

    /* create IF stack */
    reqId->ifStack = (MACRO_STACK_ID) memPoolAlloc(reqId->memPool, sizeof(MACRO_STACK));

    if (NULL == reqId->ifStack)
    	{
    	(void)memPoolFree(reqId->memPool, reqId->repeatStack->entry);
    	(void)memPoolFree(reqId->memPool, reqId->repeatStack);
        reqId->repeatStack = NULL;
        return wmError(ERROR_GENERAL, "could not alloc IF stack");
    	}

    reqId->ifStack->entry = memPoolAlloc(reqId->memPool, size * sizeof(STACK_ENTRY));

    if (NULL == reqId->ifStack->entry)
        {
    	(void)memPoolFree(reqId->memPool, reqId->repeatStack->entry);
    	(void)memPoolFree(reqId->memPool, reqId->repeatStack);
        (void)memPoolFree(reqId->memPool, reqId->ifStack);
        reqId->repeatStack = NULL;
        reqId->ifStack = NULL;
        return wmError(ERROR_GENERAL, "could not alloc IF stack");
        };
    reqId->ifStack->size = size;
    reqId->ifStack->count = 0;

    if (pRC_MACRO_INIT)    /* Init Emulation of RC33 indexing support */
        pRC_MACRO_INIT(pSession);

    return OK;
    }

/**************************************************************************
*
* httpMacroCleanup - Destroy repeat stack.
*
* Destroy repeat stack.
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/
STATUS httpMacroCleanup
    (
    WMB_SESSION_T * pSession    /* Backplane session pointer. */
    )
    {
    STATUS s = OK;
    HTTP_REQ_ID reqId;

    if ((NULL == pSession) ||
        (NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(pSession))))
        return wmError (ERROR_GENERAL, "called httpMacroInit() yet?");

    if (pSession->fileIncludeLevel > 1 )
        return OK;

    if  (NULL != reqId->repeatStack)
        {
        if  (NULL != reqId->repeatStack->entry)
            {
            s += memPoolFree(reqId->memPool , (void *) reqId->repeatStack->entry);
            reqId->repeatStack->entry = NULL;
            }
        s += memPoolFree(reqId->memPool, (void *) reqId->repeatStack);
        reqId->repeatStack = NULL;
        }

    if  (NULL != reqId->ifStack)
        {
        if  (NULL != reqId->ifStack->entry)
            {
            s += memPoolFree(reqId->memPool , (void *) reqId->ifStack->entry);
            reqId->ifStack->entry = NULL;
            }
        s += memPoolFree(reqId->memPool, (void *) reqId->ifStack);
        reqId->ifStack = NULL;
        }

    if (pRC_MACRO_CLEANUP)   /* Emulation of RC33 indexing support */
	    pRC_MACRO_CLEANUP(pSession);

    if  (OK != s)
        wmError(ERROR_GENERAL, "could not free REPEAT or IF stack");

    return s;
    }

/**************************************************************************
*
* httpMacroExecute - Execute a macro found in a HTML file stream.
*
* RETURNS: OK or ERROR
*          pFilePos is modified if the macro requires it
*          pProduceOutput is returned by ref
*
* NOMANUAL
*/
STATUS httpMacroExecute
    (
    WMB_SESSION_T * pSession,   /* Backplane session pointer. */
    sbyte *         pMacro,     /* Macro Pointer. */
    ubyte4 *        pFilePos
    )
    {
    sbyte4 arg1 = 0;
    sbyte4 arg2 = 0;
    MACRO_TOKEN beginEndElse=MACRO_VAR_NONE;
    HTTP_REQ_ID reqId = wmbSessionStateGet(pSession);
    MACRO_STACK_ID stack;
    sbyte * pTableName;
    sbyte * pArgs = strstr(pMacro, "(");
    STATUS s;

    if (NULL == pArgs)
        return wmError(ERROR_GENERAL, "unable to locate macro parameters");

    switch (httpIsMacro (pMacro))
        {
        case REPEAT_MACRO:
            stack = reqId->repeatStack;
            (void) parseRepeatArgs(pSession, pArgs, &beginEndElse, &arg1, &arg2);
            if (MACRO_VAR_BEGIN == beginEndElse)
                {
                if ((arg1 > 0) && (HTTP_TRUE == reqId->fWmbTable))
                    (void) wmbTableSkipAhead (pSession, arg1);
                s = stackPush(stack, * pFilePos, (ubyte4)(arg2 - arg1));

            if (pRC_MACRO_PUSH)    /* Emulate RC33 indexing support */
				pRC_MACRO_PUSH(pSession,s);

                }
            else if (MACRO_VAR_END == beginEndElse)
            	{
                s = stackDoneRepeat (stack, pFilePos);

            if (pRC_MACRO_DONE_REPEAT)    /* Emulate RC33 indexing support */
				pRC_MACRO_DONE_REPEAT(pSession,s);

				}
            break;
        case IF_MACRO:
            stack = reqId->ifStack;
            (void) parseIfArgs(pSession, pArgs, &beginEndElse, &arg1);
            if (MACRO_VAR_BEGIN == beginEndElse)
                {
                /* save state of fProduceOutput and result for this branch */
                (void) stackPush(stack, (ubyte4) reqId->fProduceOutput, (ubyte4)arg1);
                if (arg1 && stackStartAddrGet(stack)) httpOutputEnable(reqId);
                else httpOutputDisable(reqId);
                }
            else if (MACRO_VAR_ELSE == beginEndElse)
                {
                /* next line ensures that only one branch per IF executes */
                if (stackRemainingGet(stack)) arg1 = FALSE;
                if (arg1 && stackStartAddrGet(stack))
                    { /* enable output, unless parent IF disabled it */
                    httpOutputEnable(reqId);
                    (void) stackRemainingSet(stack, (ubyte4)arg1);
                    }
                else httpOutputDisable(reqId);
                }
            else if (MACRO_VAR_END == beginEndElse)
                {
                /* restore fProduceOutput */
                (void) stackPop(stack, (ubyte4*) &arg1, (ubyte4*) &arg2);
        /* arg2 ignored */
                reqId->fProduceOutput = (sbyte) arg1;
                }
            break;
        case CREATE_TABLE_MACRO:
            if (OK == parseCreateTableArgs(pSession, pArgs, &pTableName))
                reqId->fWmbTable = HTTP_TRUE;
            break;
        case END_TABLE_MACRO:
            if (OK == parseEndArgs(END_TABLE_MACRO, pSession, pArgs))
                reqId->fWmbTable = HTTP_FALSE;
            break;
        case END_ROW_MACRO:
            (void) parseEndArgs(END_ROW_MACRO, pSession, pArgs);
            break;
        default:
            break;
        }
    return OK;
    }

/* local functions */

/**************************************************************************
*
* stackPush - basic stack push
*
* The arguments are interpreted differently by REPEAT and IF.
*
* In REPEAT evaluations, 'startAddr' is used to hold the file position
* of the beginning of the loop, and 'remaining' holds the number of
* iterations pending.
*
* During IF evaluations, 'startAddr' is not used and 'remaining' is used
* to track if any of the IF branches has evaluated to true yet.
*
* RETURNS: OK or error
*
* NOMANUAL
*/
LOCAL STATUS stackPush
    (
    MACRO_STACK_ID  stack,
    ubyte4          startAddr,
    ubyte4          remaining
    )
    {
    if (stack->size == stack->count)
        return ERROR_GENERAL_BUFFER_OVERRUN;
    stack->entry[stack->count].startAddr = startAddr;
    stack->entry[stack->count++].remaining = remaining;
    return OK;
    }

/**************************************************************************
*
* stackPop - basic stack pop
*
* RETURNS: OK or error
*          If OK, the data is returned by reference
*
* NOMANUAL
*/
LOCAL STATUS stackPop
    (
    MACRO_STACK_ID stack,
    ubyte4 *        startAddr,
    ubyte4 *        remaining
    )
    {
    if (0 == stack->count)
        return ERROR_GENERAL_BUFFER_OVERRUN;
    (*remaining) = stack->entry[--stack->count].remaining;
    (*startAddr) = stack->entry[stack->count].startAddr;
    return OK;
    }

/**************************************************************************
*
* stackDoneRepeat - conditional pop customized for REPEAT
*
* This function decrements the remaining loops for the last entry in the
* stack.  If the result is 0, the function pops the entry and returns TRUE
* and startAddr is left unchanged.
* Otherwise it returns FALSE and startAddr is set to the starting
* address for that entry.
*
* RETURNS: TRUE if finished looping or FALSE otherwise
*          startAddr returned by ref only for FALSE result
*
* NOMANUAL
*/
LOCAL BOOL stackDoneRepeat
    (
    MACRO_STACK_ID stack,
    ubyte4 *        startAddr
    )
    {

    ubyte4 dummy1, dummy2;

    if (0 < stack->entry[stack->count-1].remaining)
        --stack->entry[stack->count-1].remaining;

    if (0 == stack->entry[stack->count-1].remaining)
        {
        (void) stackPop(stack, &dummy1, &dummy2);
        return TRUE;
        }
    (*startAddr) = stack->entry[stack->count-1].startAddr;
    return FALSE;
    }


/**************************************************************************
*
* stackStartAddrGet - get 'last in' entry without popping
*
* This routine returns the value of the 'startAddr' field in the last
* entry in the stack.
*
* RETURNS: the value of 'remaining'
*
* NOMANUAL
*/
LOCAL ubyte4 stackStartAddrGet
    (
    MACRO_STACK_ID stack
    )
    {
    return stack->entry[stack->count-1].startAddr;
    }

/**************************************************************************
*
* stackRemainingGet - get 'last in' entry without popping
*
* This routine returns the value of the 'remaining' field in the last
* entry in the stack.
*
* RETURNS: the value of 'remaining'
*
* NOMANUAL
*/
LOCAL ubyte4 stackRemainingGet
    (
    MACRO_STACK_ID stack
    )
    {
    return stack->entry[stack->count-1].remaining;
    }

/**************************************************************************
*
* stackRemainingSet - set the value of 'remaining' without popping
*
* This routine sets the value of the 'remaining' field in the last
* entry in the stack.
*
* RETURNS: the value of 'remaining'
*
* NOMANUAL
*/
LOCAL STATUS stackRemainingSet
    (
    MACRO_STACK_ID stack,
    ubyte4         newRemaining
    )
    {
    if (stack->count > 0)
        stack->entry[stack->count-1].remaining = newRemaining;
    return OK;
    }

/**************************************************************************
*
* parseVar - examine macro tokens
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/
LOCAL STATUS parseVar
    (
    WMB_SESSION_T * pSession,
    sbyte *pArg,
    MACRO_VAR *pVar,
    int hint
    )
    {
    sbyte       *pIntoArgMessage;
    sbyte       ArgValue[kMaxRepeatArgLen + 1];
    ubyte4      Index, MessageLen, BytesProcessed;
    sbyte4      Val;
    HTTP_REQ_ID reqId = (HTTP_REQ_ID) wmbSessionStateGet(pSession);
	char *		pVarStart;
	int   		nVarLength;

    int openParen = 0;

    ArgValue[0] = '\0';

	/* Initialize the var object */
	lclHttpVarInit(pVar);

    MessageLen      = (ubyte4)STRLEN(pArg);
    pIntoArgMessage = pArg;
    BytesProcessed  = 0;
    Index           = 0;

    if (0 == MessageLen) return ERROR_GENERAL;

    /* Skip White Space */
    while ((' ' == *pIntoArgMessage) || ('\t' == *pIntoArgMessage))
    {
        pIntoArgMessage++;
        pVar->var_length++;
    }

    /* Parse Syntax and Constants */
    if         (0 == STRNICMP("BEGIN",pIntoArgMessage,5))
        {pVar->var_type = MACRO_VAR_BEGIN; pVar->var_length+=5;}
    else if (0 == STRNICMP("END"  ,pIntoArgMessage,3))
        {pVar->var_type = MACRO_VAR_END  ; pVar->var_length+=3;}
    else if (0 == STRNICMP("ELSE" ,pIntoArgMessage,4))
        {pVar->var_type = MACRO_VAR_ELSE ; pVar->var_length+=4;}
    else if (0 == STRNICMP(","    ,pIntoArgMessage,1))
        {pVar->var_type = MACRO_VAR_COMMA; pVar->var_length+=1;}
    else if (0 == STRNICMP("("    ,pIntoArgMessage,1))
        {pVar->var_type = MACRO_VAR_LPAREN;pVar->var_length+=1;}
    else if (0 == STRNICMP(")"    ,pIntoArgMessage,1))
        {pVar->var_type = MACRO_VAR_RPAREN;pVar->var_length+=1;}
    else if (0 == STRNICMP("=="   ,pIntoArgMessage,2))
        {pVar->var_type = MACRO_VAR_EQUEQU;pVar->var_length+=2;}
    else if (0 == STRNICMP("="    ,pIntoArgMessage,1))
        {pVar->var_type = MACRO_VAR_EQU;   pVar->var_length+=1;}
    else if (0 == STRNICMP("<>"   ,pIntoArgMessage,2))
        {pVar->var_type = MACRO_VAR_NE;    pVar->var_length+=2;}
    else if (0 == STRNICMP("<="   ,pIntoArgMessage,2))
        {pVar->var_type = MACRO_VAR_LTE;   pVar->var_length+=2;}
    else if (0 == STRNICMP("<"    ,pIntoArgMessage,1))
        {pVar->var_type = MACRO_VAR_LT;    pVar->var_length+=1;}
    else if (0 == STRNICMP(">="   ,pIntoArgMessage,2))
        {pVar->var_type = MACRO_VAR_GTE;   pVar->var_length+=2;}
    else if (0 == STRNICMP(">"    ,pIntoArgMessage,1))
        {pVar->var_type = MACRO_VAR_GT;    pVar->var_length+=1;}
    else if (0 == STRNICMP("\""   ,pIntoArgMessage,1))
        {
		/* Extract the literal string token */
        pVar->var_type  = MACRO_VAR_STR;
		pVarStart       = pIntoArgMessage++;
		nVarLength      = 1;
        Index           = 0;
        while ((kMaxRepeatArgLen > Index) && ('\0' != *pIntoArgMessage   ) &&
           ('\"' != *pIntoArgMessage ) )
            {
            nVarLength++;
            pIntoArgMessage++;
            Index++;
            }
        if ('\"' == *pIntoArgMessage ) nVarLength++;

		(void) lclHttpVarSetBuffer(pVar, pVarStart, nVarLength);
		pVar->var_length += nVarLength;
       }
    if (pVar->var_type != MACRO_VAR_NONE)
        {
        /* We found a token, so return */
        return OK;
        }
    else if ((MACRO_VAR_STXMSK|MACRO_VAR_EQUMSK) & hint)
        {
        /* It was hinted that we were supposed to read a Syntax or Equ-op token */
        return ERROR_GENERAL;
        }
    openParen = 0;

    while ((kMaxRepeatArgLen > Index)           &&
           (MessageLen > BytesProcessed)        &&
           (ISALNUM ((int)*pIntoArgMessage) ||
            ('_' == *pIntoArgMessage)       ||
            ('*' == *pIntoArgMessage)       ||
            ('/' == *pIntoArgMessage)       ||
            ('-' == *pIntoArgMessage)       ||
            (':' == *pIntoArgMessage)       ||
            ('.' == *pIntoArgMessage)       ||
            ('(' == *pIntoArgMessage)       ||
            ((',' == *pIntoArgMessage)  &&
             (openParen > 0))               ||
            ((')'== *pIntoArgMessage)   &&
             (openParen > 0))               ||
            (' ' == *pIntoArgMessage)       ||
            ('\t' == *pIntoArgMessage)))
        {
        /*
         * Note that we do not support windmarks with string
         * arguments that may include white space characters.
         *
         * Crossing line boundaries is not supported either.
         */

        if  ((' ' == *pIntoArgMessage) || ('\t' == *pIntoArgMessage))
            {
            BytesProcessed++;
            pIntoArgMessage++;
            pVar->var_length++;

            continue;
            }

        /* special care is taken of left and right parentheses */

        if  ('(' == *pIntoArgMessage)
            {
            openParen++;
            }

        if  (')' == *pIntoArgMessage)
            {
            openParen--;

            if  (openParen < 0)
                {
                return (ERROR_GENERAL);
                }

            if  (0 == openParen)
                {
                ArgValue[Index] = '\0';
                Index++;
                BytesProcessed++;
                pIntoArgMessage++;
                pVar->var_length++;

                break;
                /* not elegant but it works: process last ')' and break */
                }
            }

        /* processing rest of accepted characters */

        ArgValue[Index] = *pIntoArgMessage;
        Index++;
        BytesProcessed++;
        pIntoArgMessage++;
        pVar->var_length++;
        }

    ArgValue[Index] = '\0';

    if ((MessageLen == BytesProcessed) || (kMaxRepeatArgLen == Index))
        return ERROR_GENERAL;

    /* Parse Numbers and Variables, using hint if provided */
    if (MACRO_VAR_STR == hint)
        {
        pVar->var_type = MACRO_VAR_STR;
		lclHttpVarSetString(pVar,ArgValue);
        }
    else if ((ISDIGIT((int)*ArgValue) || ('-'==*ArgValue)) &&
             (OK == wmConvertStrTo(ArgValue, &Val, sizeof(Val), DATATYPE_INTEGER_K)))
        {
		/* Since wmConvertStrTo does not fail silently, we pre-checked that the first
		   char is a number, to avoid false Error notices when the arg is a WindMark name */
        pVar->var_type = MACRO_VAR_INT;
        pVar->var_int  = Val;
        }
    else
        {
        sbyte*  pSubBuffer;
        sbyte   MagicMarkup[kMaxMagicMarkupIdLen + 1];
        sbyte   MagicMarkupArgs[kMaxRepeatArgLen + 1];
        Counter DupIndex0 = 0;
        Counter DupIndex1 = 0;
        STATUS  errCode;


        if (NULL == (pSubBuffer = memPoolAlloc(reqId->memPool,
                     wmVarMagicMarkupBufferSize)))
            {
            logError("parseVar: out of mem -- increase request part size");
            return ERROR_CONVERSION_GENERAL;
            }

        /* copy MagicMarkup label */

        while ((DupIndex0 < (kMaxRepeatArgLen + 1))&&
		       ('\0' != ArgValue[DupIndex0]) &&
               (DupIndex0 < kMaxMagicMarkupIdLen) &&
                 (    (ISALNUM((int)ArgValue[DupIndex0]))
	           || ('_' == (int)ArgValue[DupIndex0])
                 )
              )
            {
            MagicMarkup[DupIndex0] = ArgValue[DupIndex0];
            DupIndex0++;
            }

        MagicMarkup[DupIndex0] = '\0';

        if  ((DupIndex0 < (kMaxRepeatArgLen + 1))&&('\0' != ArgValue[DupIndex0]))
            {
            DupIndex0++;
            /* not elegant but it works: kill expected first '.' or '(' */
            }

        /* copy args */

        while ((DupIndex0 < (kMaxRepeatArgLen + 1))&&('\0' != ArgValue[DupIndex0]) && (DupIndex1 < kMaxRepeatArgLen))
            {
            MagicMarkupArgs[DupIndex1] = ArgValue[DupIndex0];
            DupIndex0++;
            DupIndex1++;
            }

        MagicMarkupArgs[DupIndex1] = '\0';


        if (NULL != wmbSessionTableGet(pSession))
            {
            errCode = wmbTableGet(pSession, MagicMarkup, pSubBuffer, wmVarMagicMarkupBufferSize );
            }
        else
            {
			errCode = wmbDataStringGet (pSession,
										MagicMarkup,
										MagicMarkupArgs,
										(sbyte4)strlen (MagicMarkupArgs),
										pSubBuffer,
										wmVarMagicMarkupBufferSize,
										NULL);
            }




        if (OK==errCode)
            {
		    if ((ISDIGIT((int)*pSubBuffer) || ('-'==*pSubBuffer)) &&
                (OK == (errCode = wmConvertStrTo(pSubBuffer, &Val, sizeof(Val), DATATYPE_INTEGER_K))))
                {
                pVar->var_type = MACRO_VAR_INT;
                pVar->var_int  = Val;
                }
            else
                {
                pVar->var_type = MACRO_VAR_STR;
				(void) lclHttpVarSetString(pVar,pSubBuffer);
                errCode = OK;
                }
            }
        else
            {
            /* Allow strings that do not resolve to Rapidmarks to be    */
            /*  returned as string constants (i.e. for SNMP table name) */
            pVar->var_type = MACRO_VAR_STR;
			(void) lclHttpVarSetString(pVar,MagicMarkup);
            errCode = OK;
            }


        (void)memPoolFree(reqId->memPool, pSubBuffer);

        if (OK != errCode)
            return ERROR_CONVERSION_GENERAL;
        }

    return OK;

    }   /* parseVar */


/**************************************************************************
*
* parseRepeatArgs - parse arguments of REPEAT macro
*
* RETURNS: OK or ERROR
*          pBeginEnd is set to either MACRO_VAR_BEGIN or MACRO_VAR_END
*          Start, End set to the REPEAT second and third arguments
*
* NOMANUAL
*/

LOCAL STATUS parseRepeatArgs
    (
    WMB_SESSION_T * pSession,
    sbyte *pArgs,
    MACRO_TOKEN * pBeginEnd,
    int *Start,
    int *End
    )
    {
	STATUS status = OK;
    MACRO_VAR Var1,Var2;
    int Index;

    /*======= Check for (BEGIN,start,end) or (SNMP table,start,end) ==========*/
    Index        = 0;

	/* Initialize the var parameter structures */
	lclHttpVarInit(&Var1);
	lclHttpVarInit(&Var2);

    /* Get Opening Paren */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
        (MACRO_VAR_LPAREN != Var1.var_type                                  ))
        {
        status = ERROR_GENERAL;
        goto exitParseRepeatArgs;
		}
    Index += Var1.var_length;

    /* Get BEGIN (or flag for SNMP table search) */
    (void)parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT);

    if  (MACRO_VAR_END != Var1.var_type)
        {
        Index += Var1.var_length;
        /* Anything other than END is considered BEGIN to maintain support for
           SNMP style REPEAT( where the first argument is the name of a table
         */
        (* pBeginEnd) = MACRO_VAR_BEGIN;

        /* Get Comma */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
            (MACRO_VAR_COMMA != Var1.var_type                                 ))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}
        Index += Var1.var_length;

        /* Get First Variable */
        if (OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_VARMSK))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}
        Index += Var1.var_length;

        /* Get Next Comma */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_STXMSK)) ||
            (MACRO_VAR_COMMA != Var2.var_type                                 ))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}
        Index += Var2.var_length;

        /* Get Second Variable */
        if (OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_VARMSK))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}
        Index += Var2.var_length;

        /* Insure that both rapidmarks are integers */
        if ((Var1.var_type != MACRO_VAR_INT) ||
            (Var2.var_type != MACRO_VAR_INT)  )
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}

        *Start = Var1.var_int;
        *End   = Var2.var_int;

        /* Get Closing Paren */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_STXMSK)) ||
            (MACRO_VAR_RPAREN != Var2.var_type                              ))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}

		status = OK;
        }
    else
        {
        /*======= Check (END) ==========*/
        (* pBeginEnd) = MACRO_VAR_END;

        Index = 0;

        /* Get Opening Paren */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
            (MACRO_VAR_LPAREN != Var1.var_type                              ))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}
        Index += Var1.var_length;

        /* Get END */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
            (MACRO_VAR_END != Var1.var_type                                 ))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}
        Index += Var1.var_length;

        /* Get Closing Paren */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_STXMSK)) ||
            (MACRO_VAR_RPAREN != Var2.var_type                              ))
			{
			status = ERROR_GENERAL;
			goto exitParseRepeatArgs;
			}

        /*======= We are done! ========*/
		status = OK;
        }

exitParseRepeatArgs:

	/* Free the var parameters */
    lclHttpVarFree(&Var1);
    lclHttpVarFree(&Var2);

    return status;
    }

/**************************************************************************
*
* parseIfArgs - parse arguments of IF macro
*
* RETURNS: OK or ERROR
*          pBeginEndElse is set to MACRO_VAR _BEGIN, _ELSE or _END
*          result result is set to TRUE if the logical statement is true
*
* NOMANUAL
*/

LOCAL STATUS parseIfArgs
    (
    WMB_SESSION_T * pSession,
    sbyte *pArgs,
    MACRO_TOKEN * pBeginEndElse,
    BOOL * result
    )
    {
	STATUS    status = OK;
    ubyte4    Index,equ_op;
    MACRO_VAR Var1,Var2;

	/* Initialize the var parameter structures */
	lclHttpVarInit(&Var1);
	lclHttpVarInit(&Var2);

    Index   = 0;
    *result = FALSE;
    equ_op  = MACRO_VAR_NONE;

    /* Get Opening Paren */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
        (MACRO_VAR_LPAREN != Var1.var_type                                  ) )
		{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}
    Index += Var1.var_length;

    /* Get BEGIN or ELSE */
    if (OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK))
		{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}
    Index += Var1.var_length;

    if (MACRO_VAR_BEGIN == Var1.var_type)
        (*pBeginEndElse) = MACRO_VAR_BEGIN;

    else if (MACRO_VAR_END   == Var1.var_type)
        {
        (*pBeginEndElse) = MACRO_VAR_END;
        *result = FALSE;

		goto exitParseIfArgs;
        }
    else if (MACRO_VAR_ELSE  == Var1.var_type)
        {
        (*pBeginEndElse) = MACRO_VAR_ELSE;
        /* If RIGHT PAREN, then this ELSE is always TRUE */
        if (OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT))
			{
			status = ERROR_GENERAL;
			goto exitParseIfArgs;
			}

        if (MACRO_VAR_RPAREN == Var1.var_type)
            {
            *result = TRUE;
			goto exitParseIfArgs;
            }
        }
    else
    	{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}

    /* Get Comma */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
        (MACRO_VAR_COMMA != Var1.var_type                                   ) )
    	{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}
    Index += Var1.var_length;

    /* Get First Variable */
    if (OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_VARMSK))
    	{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}
    Index += Var1.var_length;

    /* Get Equal Op */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_EQUMSK)) ||
        ((MACRO_VAR_EQUMSK & Var2.var_type) == 0                            ) )
    	{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}
    Index += Var2.var_length;
    equ_op = Var2.var_type;

    /* Get Second Variable */
    if (OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_VARMSK))
    	{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}
    Index += Var2.var_length;

    /* Compute Equ-OP */
    if (Var1.var_type != Var2.var_type)
    	{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}
    if (Var1.var_type == MACRO_VAR_INT) switch (equ_op)
        {
        case MACRO_VAR_EQU    :
        case MACRO_VAR_EQUEQU :
            *result = (Var1.var_int == Var2.var_int); break;
        case MACRO_VAR_NE     :
            *result = (Var1.var_int != Var2.var_int); break;
        case MACRO_VAR_LT     :
            *result = (Var1.var_int <  Var2.var_int); break;
        case MACRO_VAR_LTE    :
            *result = (Var1.var_int <= Var2.var_int); break;
        case MACRO_VAR_GT     :
            *result = (Var1.var_int >  Var2.var_int); break;
        case MACRO_VAR_GTE    :
            *result = (Var1.var_int >= Var2.var_int); break;
        }
    if (Var1.var_type == MACRO_VAR_STR) switch (equ_op)
        {
        case MACRO_VAR_EQU    :
            *result = (STRICMP(Var1.var_str,Var2.var_str)==0); break;
        case MACRO_VAR_EQUEQU :
            *result = (STRCMP( Var1.var_str,Var2.var_str)==0); break;
        case MACRO_VAR_NE     :
            *result = (STRICMP(Var1.var_str,Var2.var_str)!=0); break;
        case MACRO_VAR_LT     :
            *result = (strcmp( Var1.var_str,Var2.var_str)< 0); break;
        case MACRO_VAR_LTE    :
            *result = (STRCMP( Var1.var_str,Var2.var_str)<=0); break;
        case MACRO_VAR_GT     :
            *result = (STRCMP( Var1.var_str,Var2.var_str)> 0); break;
        case MACRO_VAR_GTE    :
            *result = (strcmp( Var1.var_str,Var2.var_str)>=0); break;
        }

    /* Get Closing Paren */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_STXMSK)) ||
        (MACRO_VAR_RPAREN != Var2.var_type                                  ) )
    	{
		status = ERROR_GENERAL;
		goto exitParseIfArgs;
		}


exitParseIfArgs:

	/* Free the var parameters *
* NOMANUAL
*/
    lclHttpVarFree(&Var1);
    lclHttpVarFree(&Var2);

    return status;
}

/**************************************************************************
*
* parseCreateTableArgs - parse arguments of CreateTable macro
*
* RETURNS: OK or ERROR
*          pTableName points to the null terminated indexColumn argument
*                     in pArgs (yes, pArgs is modified!)
*
*/

LOCAL STATUS parseCreateTableArgs
    (
    WMB_SESSION_T * pSession,
    sbyte *         pArgs,
    sbyte **        pTableName
    )
    {
	STATUS status = OK;
    enum argLen_tag {kCreateTableIncColsLength = 1024};
    MACRO_VAR Var1, Var2, Var3, indexVar;
    ubyte4 skipAhead;
    int Index, IndexFilter, IndexCols;
    sbyte incColumns[kCreateTableIncColsLength] = "";

	/* Initialize the var parameter structures */
	lclHttpVarInit(&Var1);
	lclHttpVarInit(&Var2);
	lclHttpVarInit(&Var3);
	lclHttpVarInit(&indexVar);

    /*=== Check for
     (pIndexColumn, length, firstInstance, skipAhead, filter, filterType, includedColumns)
     Do 2 passes, one for wmbCreateTable and wmbSkipAhead and one for wmbTableFilter
     ==*/
    Index = 0;

    /* First pass */
    /* Get Opening Paren */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
        (MACRO_VAR_LPAREN != Var1.var_type                                       ) )
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var1.var_length;

    /* Get pIndexColumn */
    if ((OK != parseVar(pSession,&pArgs[Index],&indexVar,MACRO_VAR_STR)) ||
        (MACRO_VAR_STR != indexVar.var_type                                        ) )
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    (*pTableName) = &pArgs[Index]; /* return pointer to pIndexColumn */
    Index += indexVar.var_length;

    /* Get Comma */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT)) ||
        (MACRO_VAR_COMMA != Var1.var_type                                        ) )
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    pArgs[Index] = 0;             /* chop pArgs after pIndexColumn */
    Index += Var1.var_length;

    /* Get length (ignore) */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT)))
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var1.var_length;

    /* Get Comma */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT)) ||
        (MACRO_VAR_COMMA != Var1.var_type                                        ) )
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var1.var_length;

    /* get first instance string and store in Var2 */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_STR)))
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var2.var_length;
    /* If prev token is not comma, first instance is NULL, and expect a comma now */
    if (MACRO_VAR_COMMA != Var2.var_type)
        {
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_COMMA)) ||
             (MACRO_VAR_COMMA != Var1.var_type                                   ) )
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var1.var_length;
        }
    /* get skip ahead integer */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT)))
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var1.var_length;
    /* If prev token is not comma, save skip ahead and expect a comma now */
    if (MACRO_VAR_COMMA != Var1.var_type)
        {
        skipAhead = Var1.var_int;
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_COMMA)) ||
             (MACRO_VAR_COMMA != Var1.var_type                                   ) )
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var1.var_length;
        }
    else skipAhead = 0;

    /* save IndexFilter for second pass */
    IndexFilter = Index;

    /* get filter string (discard this time) */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT)))
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var1.var_length;
    /* If prev token is not comma, expect a comma now */
    if (MACRO_VAR_COMMA != Var1.var_type)
        {
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_COMMA)) ||
             (MACRO_VAR_COMMA != Var1.var_type                                   ) )
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var1.var_length;
        }

    /* get filter mode (discard this time) */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STR)))
    	{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var1.var_length;
    /* If prev token is not comma, save skip ahead and expect a comma now */
    if (MACRO_VAR_COMMA != Var1.var_type)
        {
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_COMMA)) ||
             (MACRO_VAR_COMMA != Var1.var_type                                   ) )
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var1.var_length;
        }

    IndexCols = Index;
    do  /* read a variable number of included columns and store in incColumns */
        {
        /* get one of the included columns and store in Var3 */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var3,MACRO_VAR_STR)))
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var3.var_length;
        /* prevent incColumns buffer overrun */
        if (kCreateTableIncColsLength < (Index - IndexCols))
            {
            logError("run out of internal buffer.  increase kCreateTableIncColsLength");
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
            }
        strcat(incColumns, Var3.var_str);
        /* get comma or right parens */
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_NOHINT)))
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var1.var_length;
        /* If prev token is not comma or a closing parens, fail */
        if (MACRO_VAR_COMMA == Var1.var_type)
            strcat(incColumns, ",");
        else if (MACRO_VAR_RPAREN != Var1.var_type)
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        } while (MACRO_VAR_COMMA == Var1.var_type);

    /* Time to make the wmb calls for the first pass */
    if (OK != wmbTableCreate(pSession, indexVar.var_str,
                             Var2.var_str, incColumns))
        {
        logErrorMsg ("wmbTableCreate returned error %0x", errno);
        wmError(ERROR_GENERAL, "wmbTableCreate() failed");
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
        }

    logInfo ("wmbTableCreate called.  arg list follows:");
    logInfoMsg ("  pSession = %0x", pSession);
    logInfoMsg ("  pIndexColumn = %s", indexVar.var_str);
    logInfoMsg ("  pFirstInstance = %s",  Var2.var_str);
    logInfoMsg ("  pSelectedColumns = %s", incColumns);

    if (0 != skipAhead)
        {
        wmbTableSkipAhead(pSession, skipAhead);
        logInfo ("wmbTableSkipAhead called.  arg list follows:");
        logInfoMsg ("  pSession = %0x", pSession);
        logInfoMsg ("  skipAhead = %d", skipAhead);
        }

    /* === Second pass, filter arguments === */
    Index = IndexFilter;
    /* first check if the next token is a comma, and if not try to get a string
       this first check is needed because we must provide a hint for the filter
       string or it may be converted to an integer  */
    if (OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_COMMA))
        if (OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_STR))
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
    Index += Var2.var_length;
    /* If prev token is not comma, expect a comma now */
    if (MACRO_VAR_COMMA != Var2.var_type)
        {
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_COMMA)) ||
             (MACRO_VAR_COMMA != Var1.var_type                                   ) )
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var1.var_length;
        }

    /* get filter mode */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var3,MACRO_VAR_NOHINT)))
		{
		status = ERROR_GENERAL;
		goto exitParseCreateTableArgs;
		}
    Index += Var3.var_length;
    /* If prev token is not comma, expect a comma now */
    if (MACRO_VAR_COMMA != Var3.var_type)
        {
        if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_COMMA)) ||
             (MACRO_VAR_COMMA != Var1.var_type                                   ) )
			{
			status = ERROR_GENERAL;
			goto exitParseCreateTableArgs;
			}
        Index += Var1.var_length;
        }

    /* empty filters sent as a NULL */
    wmbTableFilter (pSession, Var2.var_str, Var3.var_str);

    logInfo ("wmbTableFilter called.  arg list follows:");
    logInfoMsg ("  pSession = %0x", pSession);
    logInfoMsg ("  pFilterString = %s", Var2.var_str);
    logInfoMsg ("  pFilterMode = %s",  Var3.var_str);


exitParseCreateTableArgs:

	/* Free the var parameters */
    lclHttpVarFree(&Var1);
    lclHttpVarFree(&Var2);
    lclHttpVarFree(&Var3);
    lclHttpVarFree(&indexVar);

    return status;
    }

/**************************************************************************
*
* parseEnd - parse args of endTable and endRow macros and issue wmb calls
*
* RETURNS: OK or ERROR
*
*
* NOMANUAL
*/

LOCAL STATUS parseEndArgs
    (
    MACRO_TYPE     rowOrTable,
    WMB_SESSION_T * pSession,
    sbyte *         pArgs
    )
    {
	STATUS status = OK;
    MACRO_VAR Var1, Var2;
    int Index;
    STATUS s;
    HTTP_REQ_ID reqId;

    /*=== Check for (pIndexColumn) ==*/

	/* Initialize the var parameter structures */
	lclHttpVarInit(&Var1);
	lclHttpVarInit(&Var2);

    Index = 0;

    /* Get Opening Paren */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_STXMSK)) ||
        (MACRO_VAR_LPAREN != Var1.var_type                                 ))
		{
		status = ERROR_GENERAL;
		goto exitParseEndArgs;
		}
    Index += Var1.var_length;

    /* Get pIndexColumn */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var2,MACRO_VAR_STR))    ||
        (MACRO_VAR_STR != Var2.var_type                                    ))
		{
		status = ERROR_GENERAL;
		goto exitParseEndArgs;
		}

    Index += Var2.var_length;

    /* Get Right parenthesis */
    if ((OK != parseVar(pSession,&pArgs[Index],&Var1,MACRO_VAR_RPAREN)) ||
        (MACRO_VAR_RPAREN != Var1.var_type                                 ))
		{
		status = ERROR_GENERAL;
		goto exitParseEndArgs;
		}
    Index += Var1.var_length;

    if (rowOrTable == END_TABLE_MACRO)
        {
        s = wmbTableDestroy (pSession);
        if (OK != s) logErrorMsg ("wmbTableDestroy  failed (errno=%0x)", errno);
        }
    else if (rowOrTable == END_ROW_MACRO)
        {
        s = wmbTableEndRow (pSession);
        if (OK != s) /* last row. stop REPEAT */
            if   (NULL != (reqId = (HTTP_REQ_ID) wmbSessionStateGet(pSession)))
                (void) stackRemainingSet(reqId->repeatStack, 1);
        }
    else
		{
		status = ERROR_GENERAL;
		goto exitParseEndArgs;
		}


exitParseEndArgs:

	/* Free the var parameters */
    lclHttpVarFree(&Var1);
    lclHttpVarFree(&Var2);

    return status;
    }

#endif

