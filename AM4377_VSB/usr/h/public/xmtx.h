/* xmtx.h internal header */
#ifndef _XMTX
#define _XMTX
#include <yvals.h>
#include <stdlib.h>

_C_LIB_DECL

typedef void *_Rmtx;

 #if _IS_WRS	/* compiler test */

 #ifdef __RTP__
  #define _IS_WRS_RTP	1
/*
 * In the RTP world, the __diab_alloc_mutex function returns a pointer
 * to a mutex. Since the data structure is accessed through an opaque
 * void * pointer, we cannot pass an address of a mutex variable - say mutx
 * and * expect the compiler to be able to do the following structure copy:
 * mutx = *(__diab_alloc_mutex())
 * Therefore, changing the prototype of the function for that initializes
 * mutexes.
 */
void _Mtxinit(_Rmtx **);

 #else /* __RTP__ */
void _Mtxinit(_Rmtx *);
 #endif /* __RTP__ */

void _Mtxdst(_Rmtx *);
void _Mtxlock(_Rmtx *);
void _Mtxunlock(_Rmtx *);

 #else /* _IS_WRS */
void _Mtxinit(_Rmtx *);
void _Mtxdst(_Rmtx *);
void _Mtxlock(_Rmtx *);
void _Mtxunlock(_Rmtx *);
 #endif /* _IS_WRS */

 #if !_MULTI_THREAD
  #define _Mtxinit(mtx)
  #define _Mtxdst(mtx)
  #define _Mtxlock(mtx)
  #define _Mtxunlock(mtx)

typedef char _Once_t;

  #define _Once(cntrl, func)	if (*(cntrl) == 0) (func)(), *(cntrl) = 2
  #define _ONCE_T_INIT	0

 #else /* !_MULTI_THREAD */

 #if _WIN32_C_LIB
typedef long _Once_t;

  #ifdef _M_CEE_PURE
void __clrcall _Once(_Once_t *, void (__cdecl *)(void));

  #else /* _M_CEE_PURE */
void _Once(_Once_t *, void (__cdecl *)(void));
  #endif /* _M_CEE_PURE */

  #define _ONCE_T_INIT	0

 #elif _IS_WRS_RTP
  #undef _IS_WRS_RTP

typedef unsigned long __diab_atomic_level;
typedef _Rmtx __diab_mutex;

extern __diab_atomic_level __diab_atomic_enter(void);
extern void __diab_atomic_restore(__diab_atomic_level);
extern __diab_mutex __diab_alloc_mutex(void);
extern void __diab_free_mutex(__diab_mutex);
extern void __diab_lock_mutex(__diab_mutex);
extern void __diab_unlock_mutex(__diab_mutex);
#if !defined(_NO_WINDRIVER_MODIFICATIONS)
 extern int __diab_trylock_mutex(__diab_mutex);
#endif /* _NO_WINDRIVER_MODIFICATIONS */
extern void taskDelay(int);

typedef int _Once_t;
void _Once(_Once_t *, void (*)(void));
  #define _ONCE_T_INIT	0

#define _Mtxdst(x)	__diab_free_mutex(x)
#define _Mtxlock(x)	__diab_lock_mutex(x)
#define _Mtxunlock(x)	__diab_unlock_mutex(x)
#if !defined(_NO_WINDRIVER_MODIFICATIONS)
 #define _Mtxtrylock(x)   __diab_trylock_mutex(x)
#endif /* _NO_WINDRIVER_MODIFICATIONS */

 #elif _HAS_POSIX_C_LIB
  #include <setjmp.h>
  #include <time.h>

 #if 0x570 <= __SUNPRO_CC && !defined(_RESTRICT_KYWD)	/* compiler test */
  #define _RESTRICT_KYWD restrict
 #endif /* 0x570 <= __SUNPRO_CC etc. */

  #include <pthread.h>

typedef pthread_once_t _Once_t;

  #define _Once(cntrl, func)	pthread_once(cntrl, func)
  #define _ONCE_T_INIT	PTHREAD_ONCE_INIT

 #else /* library type */
  #error unknown library type
 #endif /* library type */

 #endif /* !_MULTI_THREAD */

_END_C_LIB_DECL
#endif /* _XMTX */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:1278 */
