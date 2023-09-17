/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/sockport.h,v 1.1.1.1 2001/11/05 17:47:16 tneale Exp $ */

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
 * $Log: sockport.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:16  tneale
 * Tornado shuffle
 *
 * Revision 1.12  2001/05/29 19:13:51  masashi
 * Include errno.h if needed.
 *
 * Revision 1.11  2001/05/17 19:10:32  paul
 * Added SOCKTABLE_MIN_FD.
 *
 * Revision 1.10  2001/01/19 22:23:16  paul
 * Update copyright.
 *
 * Revision 1.9  2000/11/16 21:30:05  paul
 * Moved glue_set_deferred_io_handler from attache.h.
 * Removed stuff that is now defaulted in attache/sockets/sockintl.h.
 *
 * Revision 1.8  2000/10/16 19:21:47  paul
 * Restore sockets and mempool code.
 *
 * Revision 1.6  2000/03/17 00:10:54  meister
 * Update copyright message
 *
 * Revision 1.5  1999/05/14 20:31:20  paul
 * Moved send[to]/recv[from] wrappers from sockets.c to socktop.c.  Now
 * the mainline sockets code only has entry points for sendmsg/recvmsg.
 *
 * Revision 1.4  1999/05/12 19:03:18  paul
 * Added socktable_iterate.
 *
 * Revision 1.3  1999/03/04 17:08:35  paul
 * added sockets_shutdown, socktable_dump
 *
 * Revision 1.2  1999/02/18 04:41:29  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 1.1.4.8  1999/02/05 20:17:45  paul
 * Changed DEFAULT_* to SOCK_DFLT_*, moved fallback definitions to sockintl.h.
 * Changed SETERROR to SOCK_SET_ERRNO to reduce possibility of name
 * collision with customer code.
 *
 * Revision 1.1.4.7  1999/02/05 18:34:18  paul
 * Updated to match the bsd44 version.
 *
 */

/* [clearcase]
modification history
-------------------
*/



#ifndef _SOCKPORT_H
#define _SOCKPORT_H

#if !INSTALL_ATTACHE_SOCKETS_DEFINE_ERRORS
#include <errno.h>
#endif

extern int errno;		/* needs to be per-thread/per-task */
#define SOCK_SET_ERRNO(x) (errno = (x))

void glue_set_deferred_io_handler (void (*first)(void), void (*handler)(void));
#define GLUE_SET_DEFERRED_IO_HANDLER(x,y) glue_set_deferred_io_handler(x,y)

/* Other things you can set here:
 *
 * If you are using the built-in socket table code:
 *   SOCKTABLE_MIN_FD
 *   SOCKTABLE_MAX_FDS
 *   SOCKTABLE_HASH_BUCKETS
 *
 * If you are supplying your own socket table code:
 *   SOCKTABLE_INIT
 *   SOCKTABLE_LOOKUP
 *   SOCKTABLE_ADD
 *   SOCKTABLE_DEL
 *   SOCKTABLE_ITERATE
 *   SOCKTABLE_DUMP
 *
 * If you want to insert your own system-call type interface between
 * the user socket library and the real sockets code: 
 *   SOCK_SOCKETS_INIT
 *   SOCK_SOCKETS_SHUTDOWN
 *   SOCK_SOCKET
 *   SOCK_BIND
 *   SOCK_LISTEN
 *   SOCK_ACCEPT
 *   SOCK_CONNECT
 *   SOCK_GETSOCKNAME
 *   SOCK_GETPEERNAME
 *   SOCK_SHUTDOWN
 *   SOCK_CLOSE
 *   SOCK_SENDMSG
 *   SOCK_RECVMSG
 *   SOCK_SELECT
 *   SOCK_SETSOCKOPT
 *   SOCK_GETSOCKOPT
 *   SOCK_IOCTL
 *
 * Default parameters for sending and receiving:
 *   SOCK_DFLT_UDP_SNDBUF
 *   SOCK_DFLT_TCP_SNDBUF
 *   SOCK_DFLT_RAW_SNDBUF
 *   SOCK_DFLT_UDP_RCVBUF
 *   SOCK_DFLT_TCP_RCVBUF
 *   SOCK_DFLT_RAW_RCVBUF
 *   SOCK_DFLT_SNDLOWAT
 *   SOCK_DFLT_RCVLOWAT
 */

#endif /* _SOCKPORT_H */
