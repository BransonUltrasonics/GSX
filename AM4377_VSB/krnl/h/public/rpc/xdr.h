/* xdr.h, External Data Representation Serialization Routines. */

/*
 * Copyright (c) 1984-2005, 2006-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
/*      @(#)xdr.h 1.1 86/02/03 SMI      */

/*
 * xdr.h, External Data Representation Serialization Routines.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

/*
modification history
--------------------
01w,14sep12,yjl  Fix WIND00376255, dosFs to be compatible with Windows FAT32
                 naming rules for file/directory name length
01v,29apr10,pad  Moved extern C statement after include statements.
01u,15jan10,y_t Add LP64 support.
01t,17may05,vvv  fixed integer overflow (SPR #108521)
01s,15apr05,vvv  changed prototype for xdrrec_create() (SPR #105654)
01r,24nov03,ann  added support for long long (64 bit)
01q,27jul99,elg  Add XDR_PUTWORDS and XDR_PUTLONGS.
01p,01mar99,spm  added a C++ compatible XDR structure (SPR #24837)
01o,29jan96,mem  replaced test of CPU_FAMILY with test of _BYTE_ORDER
01n,09jun93,hdn  added support for I80X86
01n,17oct94,ism  fixed prototype of xdr_u_char() (SPR#2675)
01m,22sep92,rrr  added support for c++
01l,07sep92,smb  added include of rpctypes.h
01k,26may92,rrr  the tree shuffle
01j,26may92,ajm  fixed #define of IXDR_GET_LONG for all compilers (see 01g)
          updated copyright
01i,04oct91,rrr  passed through the ansification filter
          -fixed broken prototype
          -fixed #else and #endif
          -changed copyright notice
01h,18sep91,wmd  use defined(HOST_MIPS) || defined(HOST_DEC) instead of
         CPU=R3000 for the conditional.
01g,05aug91,ajm  changed #define of IXDR_GET_LONG to be K+R compliant
                 for mips compiler (++ can't be applied to an expresion)
01f,05aug91,del  IXDR_GET_LONG is ixdr_get_long function for I960
         problem w/htonl macro expansion.
01e,24oct90,shl  commented out redundant function declarations.
01d,05oct90,shl  added ANSI function prototypes.
                 added copyright notice.
01c,19apr90,hjb  modified IXDR_GET_LONG, IXDR_PUT_LONG, IXDR_PUT_BOOL,
         IXDR_PUT_ENUM, IXDR_PUT_U_LONG, IXDR_PUT_SHORT,
         IXDR_PUT_U_SHORT.
01b,26oct89,hjb  upgraded to release 4.0
*/

#ifndef __INCxdrh
#define __INCxdrh

#include "rpctypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * XDR provides a conventional way for converting between C data
 * types and an external bit-string representation.  Library supplied
 * routines provide for the conversion on built-in C data types.  These
 * routines and utility routines defined here are used to help implement
 * a type encode/decode routine for each user-defined type.
 *
 * Each data type provides a single procedure which takes two arguments:
 *
 *  bool_t
 *  xdrproc(xdrs, argresp)
 *      XDR *xdrs;
 *      <type> *argresp;
 *
 * xdrs is an instance of a XDR handle, to which or from which the data
 * type is to be converted.  argresp is a pointer to the structure to be
 * converted.  The XDR handle contains an operation field which indicates
 * which of the operations (ENCODE, DECODE * or FREE) is to be performed.
 *
 * XDR_DECODE may allocate space if the pointer argresp is null.  This
 * data can be freed with the XDR_FREE operation.
 *
 * We write only one procedure per data type to make it easy
 * to keep the encode and decode procedures for a data type consistent.
 * In many cases the same code performs all operations on a user defined type,
 * because all the hard work is done in the component type routines.
 * decode as a series of calls on the nested data types.
 */

/*
 * Xdr operations.  XDR_ENCODE causes the type to be encoded into the
 * stream.  XDR_DECODE causes the type to be extracted from the stream.
 * XDR_FREE can be used to release the space allocated by an XDR_DECODE
 * request.
 */
enum xdr_op {
    XDR_ENCODE=0,
    XDR_DECODE=1,
    XDR_FREE=2
};

#ifdef _WRS_CONFIG_DOSFS_NAME_LENGTH_COMPAT
#define USE_FILE_PATHLENGTH
#endif 
/*
 * This is the number of bytes per unit of external data.
 */
#define BYTES_PER_XDR_UNIT  (4)
#define RNDUP(x) \
    ((((x) + BYTES_PER_XDR_UNIT - 1) / BYTES_PER_XDR_UNIT) * \
    BYTES_PER_XDR_UNIT)     /* 4.0 */

/*
 * A xdrproc_t exists for each data type which is to be encoded or decoded.
 *
 * The second argument to the xdrproc_t is a pointer to an opaque pointer.
 * The opaque pointer generally points to a structure of the data type
 * to be decoded.  If this pointer is 0, then the type routines should
 * allocate dynamic storage of the appropriate size and return it.
 * bool_t   (*xdrproc_t)(XDR *, caddr_t *);
 */
typedef bool_t (*xdrproc_t)();

/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the paticular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular impelementation.
 */

#ifdef __cplusplus

typedef struct {
    enum xdr_op x_op;       /* operation; fast additional param */
    struct xdr_ops {
                    /* get a long long from underlying stream */
        bool_t  (*x_getlonglong)(...);
                    /* put a long long to underlying stream */
        bool_t  (*x_putlonglong) (...);
                    /* get a long from underlying stream */
        bool_t  (*x_getlong)(...);
                    /* put a long to underlying stream */
        bool_t  (*x_putlong) (...);
                    /* get some bytes from underlying stream */
        bool_t  (*x_getbytes) (...);
                    /* put some bytes to underlying stream */
        bool_t  (*x_putbytes) (...);
                    /* put some words to underlying stream */
        bool_t  (*x_putwords) (...);
                    /* put some longs to underlying stream */
        bool_t  (*x_putlongs) (...);
                    /* returns bytes from beginning of stream*/
        u_int   (*x_getpostn) (...);
                    /* reposition the stream */
        bool_t  (*x_setpostn) (...);
                    /* buf quick ptr to buffered data */
        int *   (*x_inline) (...);
                    /* free private elements of this stream */
                void (*x_destroy) (...);
    } *x_ops;
    caddr_t     x_public;   /* users' data */
    caddr_t     x_private;  /* pointer to private data */
    caddr_t     x_base;     /* private used for position info */
    u_int       x_handy;    /* extra private word */
} XDR;

#else

typedef struct {
    enum xdr_op x_op;       /* operation; fast additional param */
    struct xdr_ops {
        bool_t  (*x_getlonglong)(); /* get a long long from underlying stream */
        bool_t  (*x_putlonglong)(); /* put a long long to */
        bool_t  (*x_getlong)(); /* get a long from underlying stream */
        bool_t  (*x_putlong)(); /* put a long to " */
        bool_t  (*x_getbytes)();/* get some bytes from " */
        bool_t  (*x_putbytes)();/* put some bytes to " */
        bool_t  (*x_putwords)();/* put some words to " */
        bool_t  (*x_putlongs)();/* put some longs to " */
        u_int   (*x_getpostn)();/* returns bytes off from beginning */
        bool_t  (*x_setpostn)();/* lets you reposition the stream */
        int *   (*x_inline)();  /* buf quick ptr to buffered data */
        void    (*x_destroy)(); /* free privates of this xdr_stream */
    } *x_ops;
    caddr_t     x_public;   /* users' data */
    caddr_t     x_private;  /* pointer to private data */
    caddr_t     x_base;     /* private used for position info */
    u_int       x_handy;    /* extra private word */
} XDR;

#endif  /* __cplusplus */

/*
 * Operations defined on a XDR handle
 *
 * XDR      *xdrs;
 * long     *longp;
 * caddr_t   addr;
 * u_int     len;
 * u_int     pos;
 */
#define XDR_GETLONG(xdrs, longp)            \
    (*(xdrs)->x_ops->x_getlong)(xdrs, longp)
#define xdr_getlong(xdrs, longp)            \
    (*(xdrs)->x_ops->x_getlong)(xdrs, longp)

#define XDR_PUTLONG(xdrs, longp)            \
    (*(xdrs)->x_ops->x_putlong)(xdrs, longp)
#define xdr_putlong(xdrs, longp)            \
    (*(xdrs)->x_ops->x_putlong)(xdrs, longp)

#define XDR_GETLONGLONG(xdrs, longlongp)            \
    (*(xdrs)->x_ops->x_getlonglong)(xdrs, longlongp)
#define xdr_getlonglong(xdrs, longp)            \
    (*(xdrs)->x_ops->x_getlonglong)(xdrs, longlongp)

#define XDR_PUTLONGLONG(xdrs, longlongp)            \
    (*(xdrs)->x_ops->x_putlonglong)(xdrs, longlongp)
#define xdr_putlonglong(xdrs, longlongp)            \
    (*(xdrs)->x_ops->x_putlonglong)(xdrs, longlongp)

#define XDR_GETBYTES(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)
#define xdr_getbytes(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)

#define XDR_PUTBYTES(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)
#define xdr_putbytes(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)

#define XDR_PUTWORDS(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_putwords)(xdrs, addr, len)
#define xdr_putwords(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_putwords)(xdrs, addr, len)

#define XDR_PUTLONGS(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_putlongs)(xdrs, addr, len)
#define xdr_putlongs(xdrs, addr, len)           \
    (*(xdrs)->x_ops->x_putlongs)(xdrs, addr, len)

#define XDR_GETPOS(xdrs)                \
    (*(xdrs)->x_ops->x_getpostn)(xdrs)
#define xdr_getpos(xdrs)                \
    (*(xdrs)->x_ops->x_getpostn)(xdrs)

#define XDR_SETPOS(xdrs, pos)               \
    (*(xdrs)->x_ops->x_setpostn)(xdrs, pos)
#define xdr_setpos(xdrs, pos)               \
    (*(xdrs)->x_ops->x_setpostn)(xdrs, pos)

#define XDR_INLINE(xdrs, len)               \
    (*(xdrs)->x_ops->x_inline)(xdrs, len)
#define xdr_inline(xdrs, len)               \
    (*(xdrs)->x_ops->x_inline)(xdrs, len)

#define XDR_DESTROY(xdrs)               \
    if ((xdrs)->x_ops->x_destroy)           \
        (*(xdrs)->x_ops->x_destroy)(xdrs)
#define xdr_destroy(xdrs)               \
    if ((xdrs)->x_ops->x_destroy)           \
        (*(xdrs)->x_ops->x_destroy)(xdrs)

/*
 * Support struct for discriminated unions.
 * You create an array of xdrdiscrim structures, terminated with
 * a entry with a null procedure pointer.  The xdr_union routine gets
 * the discriminant value and then searches the array of structures
 * for a matching value.  If a match is found the associated xdr routine
 * is called to handle that part of the union.  If there is
 * no match, then a default routine may be called.
 * If there is no match and no default routine it is an error.
 */
#define NULL_xdrproc_t ((xdrproc_t)0)
struct xdr_discrim {
    int value;
    xdrproc_t proc;
};

/*
 * In-line routines for fast encode/decode of primitve data types.
 * Caveat emptor: these use single memory cycles to get the
 * data from the underlying buffer, and will fail to operate
 * properly if the data is not aligned.  The standard way to use these
 * is to say:
 *  if ((buf = XDR_INLINE(xdrs, count)) == NULL)
 *      return (FALSE);
 *  <<< macro calls >>>
 * where ``count'' is the number of bytes of data occupied
 * by the primitive data types.
 *
 * N.B. and frozen for all time: each data type here uses 4 bytes
 * of external representation.
 */
#ifndef _BYTE_ORDER
#error _BYTE_ORDER must be defined.
#endif
#ifndef _BIG_ENDIAN
#error _BIG_ENDIAN must be defined.
#endif

#if     (_BYTE_ORDER == _BIG_ENDIAN)
#define IXDR_GET_LONG(buf)              ntohl((u_int) *(buf)++)
#else
#define IXDR_GET_LONG(buf)              ixdr_get_long ((u_int **)&buf)
#endif  /* (_BYTE_ORDER == _BIG_ENDIAN) */

#define IXDR_PUT_LONG(buf, v)       (*buf++ = htonl((u_int) (v)))

#define IXDR_GET_BOOL(buf)      ((bool_t)IXDR_GET_LONG(buf))
#define IXDR_GET_ENUM(buf, t)       ((t)IXDR_GET_LONG(buf))
#define IXDR_GET_U_LONG(buf)        ((u_int)IXDR_GET_LONG(buf))
#define IXDR_GET_SHORT(buf)     ((short)IXDR_GET_LONG(buf))
#define IXDR_GET_U_SHORT(buf)       ((u_short)IXDR_GET_LONG(buf))

#define IXDR_PUT_BOOL(buf, v)       IXDR_PUT_LONG((buf), (v))
#define IXDR_PUT_ENUM(buf, v)       IXDR_PUT_LONG((buf), (v))
#define IXDR_PUT_U_LONG(buf, v)     IXDR_PUT_LONG((buf), (v))
#define IXDR_PUT_SHORT(buf, v)      IXDR_PUT_LONG((buf), (v))
#define IXDR_PUT_U_SHORT(buf, v)    IXDR_PUT_LONG((buf), (v))

/*
 * Common opaque bytes objects used by many rpc protocols
 * declared here due to commonality.
 */

#ifdef USE_FILE_PATHLENGTH
#define MAX_NETOBJ_SZ   1036
#else
#define MAX_NETOBJ_SZ   1024
#endif

struct netobj
    {
    u_int   n_len;
    char    *n_bytes;
    };

typedef struct netobj netobj;

/*
 * These are the public routines for the various implementations of
 * xdr streams.
 */

/* extern void   xdrmem_create(); */    /* XDR using memory buffers */
/* extern void   xdrstdio_create(); */  /* XDR using stdio library */
/* extern void   xdrrec_create(); */    /* XDR pseudo records for tcp */
/* extern bool_t xdrrec_endofrecord(); */   /* make end of xdr record */
/* extern bool_t xdrrec_skiprecord(); */   /* move to begining of next record */
/* extern bool_t xdrrec_eof(); */   /* true iff no more input */

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

extern    void         xdr_free (xdrproc_t proc, char *objp);
extern    bool_t       xdr_void (void);
extern    bool_t       xdr_int (XDR *xdrs, int *ip);
extern    bool_t       xdr_u_int (XDR *xdrs, u_int *up);
extern    bool_t       xdr_longlong_t(XDR * xdrs,long long * lp);
extern    bool_t       xdr_u_longlong_t(XDR *xdrs, unsigned long long *ulp);
extern    bool_t       xdr_long (XDR *xdrs, int *lp);
extern    bool_t       xdr_u_long (XDR *xdrs, u_int *ulp);
extern    bool_t       xdr_short (XDR *xdrs, short *sp);
extern    bool_t       xdr_u_short (XDR *xdrs, u_short *usp);
extern    bool_t       xdr_char (XDR *xdrs, char *cp);
extern    bool_t       xdr_u_char (XDR *xdrs, u_char *ucp);
extern    bool_t       xdr_bool (XDR *xdrs, bool_t *bp);
extern    bool_t       xdr_enum (XDR *xdrs, enum_t *ep);
extern    bool_t       xdr_opaque (XDR *xdrs, caddr_t cp, u_int cnt);
extern    bool_t       xdr_bytes (XDR *xdrs, char **cpp, u_int *sizep,
                  u_int maxsize);
extern    bool_t       xdr_netobj (XDR *xdrs, struct netobj *np);
extern    bool_t       xdr_union (XDR *xdrs, enum_t *dscmp, caddr_t unp,
                 struct xdr_discrim *choices, xdrproc_t dfault);
extern    bool_t       xdr_string (XDR *xdrs, char **cpp, u_int maxsize);
extern    bool_t       xdr_wrapstring (XDR *xdrs, char **cpp);
extern    bool_t       xdr_array (XDR *xdrs, caddr_t *addrp, u_int *sizep,
                 u_int maxsize, u_int elsize, xdrproc_t elproc);
extern    bool_t       xdr_vector (XDR *xdrs, char *basep, u_int nelem,
                   u_int elemsize, xdrproc_t xdr_elem);
extern    void         xdr_floatInclude (void);
extern    bool_t       xdr_float (XDR *xdrs, float *fp);
extern    bool_t       xdr_double (XDR *xdrs, double *dp);
extern    void         xdrmem_create (XDR *xdrs, caddr_t addr, u_int size,
                      enum xdr_op op);
extern    void *       xdrrec_create (XDR *xdrs, u_int sendsize, u_int recvsize,
                   caddr_t tcp_handle, int (*readit)(), int (*writeit)());
extern    bool_t       xdrrec_skiprecord (XDR *xdrs);
extern    bool_t       xdrrec_eof (XDR *xdrs);
extern    bool_t       xdrrec_endofrecord (XDR *xdrs, bool_t sendnow);
extern    bool_t       xdr_reference (XDR *xdrs, caddr_t *pp, u_int size,
                      xdrproc_t proc);
extern    bool_t       xdr_pointer (XDR *xdrs, char **objpp, u_int obj_size,
                    xdrproc_t xdr_obj);
extern    u_int        ixdr_get_long (u_int ** buf);

#else

extern    void         xdr_free ();
extern    bool_t       xdr_void ();
extern    bool_t       xdr_int ();
extern    bool_t       xdr_u_int ();
extern    bool_t       xdr_long ();
extern    bool_t       xdr_u_long ();
extern    bool_t       xdr_short ();
extern    bool_t       xdr_u_short ();
extern    bool_t       xdr_char ();
extern    bool_t       xdr_u_char ();
extern    bool_t       xdr_bool ();
extern    bool_t       xdr_enum ();
extern    bool_t       xdr_opaque ();
extern    bool_t       xdr_bytes ();
extern    bool_t       xdr_netobj ();
extern    bool_t       xdr_union ();
extern    bool_t       xdr_string ();
extern    bool_t       xdr_wrapstring ();
extern    bool_t       xdr_array ();
extern    bool_t       xdr_vector ();
extern    bool_t       xdr_float ();
extern    bool_t       xdr_double ();
extern    void *       xdrmem_create ();
extern    void         xdrrec_create ();
extern    bool_t       xdrrec_skiprecord ();
extern    bool_t       xdrrec_eof ();
extern    bool_t       xdrrec_endofrecord ();
extern    bool_t       xdr_reference ();
extern    bool_t       xdr_pointer ();

#endif  /* __STDC__ */

#ifdef __cplusplus
}
#endif

#endif /* __INCxdrh */
