/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/bsd44/thdport.h,v 1.1.1.1 2001/11/05 17:47:08 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1998-1999 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: thdport.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:08  tneale
 * Tornado shuffle
 *
 * Revision 1.9  2001/03/22 16:54:16  paul
 * Removed GLUE_COND_TIMEDWAIT().
 *
 * Revision 1.8  2001/03/20 17:11:15  paul
 * Removed unused timeout macros.
 *
 * Revision 1.7  2001/01/19 22:22:56  paul
 * Update copyright.
 *
 * Revision 1.6  2000/10/16 19:21:45  paul
 * Restore sockets and mempool code.
 *
 * Revision 1.4  2000/03/17 00:08:58  meister
 * Update copyright message
 *
 * Revision 1.3  1999/11/08 19:16:06  paul
 * Added copyright.
 *
 */

/* [clearcase]
modification history
-------------------
*/


#include <pthread.h>

/* !!! checking return codes here? */

#define GLUE_MUTEX_PRIO_MAX 100
#define GLUE_MUTEX_PRIO_HIGH 80
#define GLUE_MUTEX_PRIO_MID  40
#define GLUE_MUTEX_PRIO_LOW  20
#define GLUE_MUTEX_PRIO_MIN  10

typedef pthread_mutex_t glue_mutex_t;

typedef pthread_cond_t glue_cond_t;

typedef pthread_t glue_thread_t;

#define GLUE_MUTEX_INIT(lock, prio) pthread_mutex_init(lock, NULL)
#define GLUE_MUTEX_INIT_MAGIC(lock) pthread_mutex_init(lock, NULL)
#define GLUE_MUTEX_LOCK(lock, prio) pthread_mutex_lock(lock)
#define GLUE_MUTEX_UNLOCK(lock, prio) pthread_mutex_unlock(lock)
#define GLUE_MUTEX_TRYLOCK(lock, prio) pthread_mutex_trylock(lock)
#define GLUE_MUTEX_DESTROY(lock) pthread_mutex_destroy(lock)

#define GLUE_COND_INIT(cond, mutex) pthread_cond_init(cond, NULL)
#define GLUE_COND_WAIT(cond, mutex) pthread_cond_wait(cond, mutex)
#define GLUE_COND_WAKEUP(cond, mutex) pthread_cond_signal(cond)
#define GLUE_COND_BROADCAST(cond, mutex) pthread_cond_broadcast(cond)
#define GLUE_COND_DESTROY(cond) pthread_cond_destroy(cond)

#define GLUE_THD_SELF() pthread_self()
#define GLUE_THD_YIELD() pthread_yield()

#define GLUE_THD_CREATE(thread, func, arg) pthread_create(thread, NULL, func, arg)
#define GLUE_THD_WAIT(thread) pthread_join(thread, NULL)
#define GLUE_THD_EXIT() pthread_exit(NULL)

#define GLUE_THREAD_INIT() pthread_init()

pthread_key_t debug_state_key;
     
#define GLUE_THD_COOKIE_INIT(freertn) pthread_key_create (&debug_state_key, freertn);

#define GLUE_THD_COOKIE_GET(y) *(y) = pthread_getspecific(debug_state_key)
#define GLUE_THD_COOKIE_SET(x) pthread_setspecific(debug_state_key, x)

