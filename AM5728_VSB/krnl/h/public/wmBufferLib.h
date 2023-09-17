/* wmBufferLib.h - header file for WindManagerlibrary */

/* Copyright 2000-2011 Wind River Systems, Inc. */

/*
modification history
--------------------
01f,28mar11,r_w fix defect WIND00257014
                to make some declares of functions comply with definitions
01e,17dec10,m_z part of APIs changed
                CQID: Fix WIND00247096
01d,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
12oct05,wdz      '#ifdef __cplusplus' statement added
                 for SPR#98444
01b,20feb04,jws  copyright added
01a,15jan02,pns  written.
*/


/*
 * DESCRIPTION:
 */

#ifndef __INCwmBufferLibh
#define __INCwmBufferLibh

typedef struct WM_BUFFER_S
    {
    void *     data;       /* points to a data buffer */
#ifdef __cplusplus
#ifndef _WRS_CONFIG_LP64
    sbyte4     Used;       /* length of data currently stored in the buffer */
    sbyte4     Max;        /* length of entire buffer */
#else
    sbyte8     Used;       /* length of data currently stored in the buffer */
    sbyte8     Max;        /* length of entire buffer */
#endif
#else
#ifndef _WRS_CONFIG_LP64
    ubyte4     Used;       /* length of data currently stored in the buffer */
    ubyte4     Max;        /* length of entire buffer */
#else
    ubyte8     Used;       /* length of data currently stored in the buffer */
    ubyte8     Max;        /* length of entire buffer */
#endif
#endif /* __cplusplus */
    ubyte      Native;     /* native data type */
    ubyte      Local;      /* local data type */
    ubyte4     flags;      /* flags, including static/dynamic flag */
    void *     pState;     /* information required to free buffer, if dynamic */
#ifndef __NO_MAGIC__
    ubyte4     magic;      /* normally first member but this
                              also mimics 3.x oid structs */
#endif
    }
    WM_BUFFER_T;

#define WM_BUFFER_MAGIC_K   0xabbababe
#define wmBufferBless(x)    wmMagicBless(x, WM_BUFFER_MAGIC_K)
#define wmBufferBad(x)      wmMagicBad(x, WM_BUFFER_MAGIC_K)

#define wmBuffer(x)         ((WM_BUFFER_T *) x)

/* access macros */

#define wmBufferMaxGet(x)       (wmBufferBad(x) ? 0 : wmBuffer(x)->Max)
#define wmBufferUsedGet(x)      (wmBufferBad(x) ? 0 : wmBuffer(x)->Used)
#define wmBufferTypeGet(x)      (wmBufferBad(x) ? 0  : wmBuffer(x)->Native)
#define wmBufferLocalGet(x)     (wmBufferBad(x) ? 0  : wmBuffer(x)->Local)


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ENABLE_SHARED_MEMORY__
#define wmBufferDataGet(buf)    wmDataGet(wmBufferShared(buf), buf, buf->data)
#else
void *wmBufferDataGet(WM_BUFFER_T  * buf);
#endif
STATUS  wmBufferAppend (WM_BUFFER_T * pBuf, void * pData, size_t len);
WM_BUFFER_T * wmBufferCreate (void * pool, size_t size, void * pData,
                              DATATYPE_T dataType, BOOL copy);
STATUS  wmBufferCopy    (WM_BUFFER_T * pDest, WM_BUFFER_T * pSrc);
STATUS wmBufferAssign ( WM_BUFFER_T * pBuf, void * pData, size_t len, ubyte type);
WM_BUFFER_T * wmBufferDuplicate (WM_BUFFER_T * pSrc);
STATUS  wmBufferExport  (WM_BUFFER_T * pBuf, void * pDest, 
                         size_t destSize, ubyte4 destType, size_t * pWritten);                         
STATUS  wmBufferDelete  (WM_BUFFER_T * pBuf);
void    wmBufferInit    (WM_BUFFER_T *   pBuf, ubyte4 flags);

STATUS  wmBufferSuffix  (WM_BUFFER_T * pBase, WM_BUFFER_T * pFull,
                         WM_BUFFER_T * pSuffix);

/*
BOOL    wmBufferIsOid   (WM_BUFFER_T * pBuf);
void    wmBufferMakeOid (WM_BUFFER_T * pBuf, BOOL isOid);
*/

BOOL    wmBufferShared  (WM_BUFFER_T * pBuf);

/*
void    wmBufferSharedSet (WM_BUFFER_T * pBuf, BOOL shared);
*/

STATUS  wmBufferStringSize (WM_BUFFER_T * pBuf, size_t * pSize);
STATUS  wmBufferUpdate  (WM_BUFFER_T * pBuf, void * pData, size_t len,
                        DATATYPE_T dataType, BOOL copy);
void wmBufferReset      (WM_BUFFER_T * pBuf);
void wmBufferUsedSet(WM_BUFFER_T * pBuf, size_t size);
void wmBufferTypeSet(WM_BUFFER_T * pBuf, DATATYPE_T type);
void wmBufferLocalSet(WM_BUFFER_T * pBuf, DATATYPE_T type);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCwmBufferLibh */

