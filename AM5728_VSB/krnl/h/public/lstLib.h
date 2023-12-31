/* lstLib.h - doubly linked list library header */

/*
* Copyright (c) 1983-1984, 1986, 1990-1993, 2001, 2006,
*               2009-2010, 2014 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
09sep14,clt  vx7-cert clean up
28apr10,pad  Moved extern C statement after include statements.
13mar09,jpb  Updated for LP64 support.
24jul06,jpb  Removal of duplicate definition of NODE and LIST.
26jan06,mil  Updated for POSIX namespace conformance (P2).
19sep01,pcm  added lstLibInit () (SPR 20698)
02apr93,edm  ifdef'd out non-ASMLANGUAGE portions
22sep92,rrr  added support for c++
04jul92,jcf  cleaned up.
26may92,rrr  the tree shuffle
04oct91,rrr  passed through the ansification filter
             -changed VOID to void
             -changed copyright notice
23oct90,shl  included "vxWorks.h" so include file order isn't crucial.
05oct90,shl  added ANSI function prototypes.
             made #endif ANSI style.
             added copyright notice.
10aug90,dnw  added declaration of lstInsert().
07aug90,shl  added IMPORT type to function declarations.
21may86,llk  added forward declaration of lstNStep.
03jun84,dnw  changed list.{head,tail} to list.node.
             added declarations of lst{First,Last,Next,Previous}.
27jan84,ecs  added inclusion test.
15mar83,dnw  changed name from lstlb to lstLib
*/

#ifndef __INClstLibh
#define __INClstLibh

#ifndef _ASMLANGUAGE

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)
#include <vxWorks.h>
#endif  /* _POSIX_xxx_SOURCE */

#ifdef __cplusplus
extern "C" {
#endif

/* type definitions */

#if defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) || \
    defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)
#undef next
#undef previous
#endif

typedef struct _Vx_node         /* Node of a linked list. */
    {
    struct _Vx_node* next;      /* Points at the next node in the list */
    struct _Vx_node* previous;  /* Points at the previous node in the list */
    } _Vx_NODE;

/* HIDDEN */

#if defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) || \
    defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)
#undef node
#undef count
#endif

typedef struct                  /* Header for a linked list. */
    {
    _Vx_NODE node;              /* Header list node */
    int count;                  /* Number of nodes in list */
    } _Vx_LIST;

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)
typedef _Vx_NODE NODE;
typedef _Vx_LIST LIST;
#endif

/* END_HIDDEN */

/* function declarations */

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE) && \
    !defined(_POSIX_AEP_RT_CONTROLLER_C_SOURCE)

extern NODE*    lstFirst (LIST* pList);
extern NODE*    lstGet (LIST* pList);
extern NODE*    lstLast (LIST* pList);
extern NODE*    lstPrevious (NODE* pNode);
extern NODE*    lstNext (NODE* pNode);
extern int      lstCount (LIST* pList);
extern int      lstFind (LIST* pList, NODE* pNode);
extern void     lstAdd (LIST* pList, NODE* pNode);
extern void     lstDelete (LIST* pList, NODE* pNode);
extern void     lstInit (LIST* pList);
extern void     lstInsert (LIST* pList, NODE* pPrev, NODE* pNode);

typedef void    (*LST_FREE_RTN) (void*);
extern void     lstLibInit (void);
extern NODE*    lstNStep (NODE* pNode, int nStep);
extern NODE*    lstNth (LIST* pList, int nodenum);
extern void     lstConcat (LIST* pDstList, LIST* pAddList);
extern void     lstExtract (LIST* pSrcList, NODE* pStartNode, NODE* pEndNode,
                            LIST* pDstList);
extern void     lstFree (LIST* pList);
#endif  /* _POSIX_xxx_SOURCE */

#ifdef __cplusplus
    }
#endif

#endif /* ~ _ASMLANGUAGE */

#endif /* __INClstLibh */
