/* wmInstanceLib.h */

/* Copyright 2004, 2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01a,04may04,dlr  derived from wmOidLib.h, adapted to string Instances
*/

#ifndef __INCwmInstanceLibh
#define __INCwmInstanceLibh

typedef WM_BUFFER_T * WM_INSTANCE_T;

#define wmInstanceBad    wmBufferBad

#ifdef __cplusplus
extern "C" {
#endif

STATUS      	wmInstanceAppend     (WM_INSTANCE_T pDestInst, WM_INSTANCE_T pAddInst);
int         	wmInstanceCmp        (WM_INSTANCE_T first, WM_INSTANCE_T second);
BOOL        	wmInstanceEqual      (WM_INSTANCE_T first, WM_INSTANCE_T second);
#ifndef _WRS_CONFIG_LP64
WM_INSTANCE_T   wmInstanceFromString (sbyte * pString, int max);
#else
WM_INSTANCE_T   wmInstanceFromString (sbyte * pString, long max);
#endif
#ifndef _WRS_CONFIG_LP64
int         	wmInstanceLen        (WM_INSTANCE_T inst);
#else
long            wmInstanceLen        (WM_INSTANCE_T inst);
#endif
void        	wmInstancePrint      (WM_INSTANCE_T inst, sbyte * pMsg);
STATUS      	wmInstanceStringSet  (WM_INSTANCE_T inst, sbyte * pString, BOOL append);
#ifndef _WRS_CONFIG_LP64
STATUS      	wmInstanceStringGet  (WM_INSTANCE_T inst, sbyte * pString, sbyte4 strLen);
#else
STATUS      	wmInstanceStringGet  (WM_INSTANCE_T inst, sbyte * pString, sbyte8 strLen);
#endif
WM_INSTANCE_T   wmInstanceSuffix     (WM_INSTANCE_T pBase, WM_INSTANCE_T pFull);
#ifndef _WRS_CONFIG_LP64
STATUS      	wmInstanceTruncate   (WM_INSTANCE_T inst, sbyte4 length);
#else
STATUS      	wmInstanceTruncate   (WM_INSTANCE_T inst, sbyte8 length);
#endif
STATUS      	wmInstanceValid      (WM_INSTANCE_T inst);

STATUS 			wmBufferInstSuffix   (WM_BUFFER_T *pRoot, WM_BUFFER_T* pFull, WM_BUFFER_T *pSuffix);

#if 0
BOOL        	wmInstanceScaler     (WM_INSTANCE_T inst);
void        	wmInstanceScalerSet  (WM_INSTANCE_T inst);
#endif

#ifdef __WM_DEBUG__
void        	wmInstanceDebug      (WM_INSTANCE_T inst, sbyte * pMsg);
void        	wmInstanceDebugEnable(BOOL enable);
#else
#define     	wmInstanceDebug(x,y)
#define     	wmInstanceDebugEnable(x)
#endif /* __WM_DEBUG__ */

#ifdef __cplusplus
}
#endif

#endif /* ! __INCwmInstanceLibh */
