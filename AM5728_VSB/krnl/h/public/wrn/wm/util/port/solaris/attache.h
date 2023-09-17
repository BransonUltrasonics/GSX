/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/attache.h,v 1.3 2003/01/15 14:00:27 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1994-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 *****************************************************************************/

/*
 * $Log: attache.h,v $
 * Revision 1.3  2003/01/15 14:00:27  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 15:58:29  tneale
 * Updated for newest file layout
 *
 * Revision 1.1.1.1  2001/11/05 17:47:15  tneale
 * Tornado shuffle
 *
 * Revision 1.11  2001/06/25 19:43:11  markadam
 * Remove the gcc only inline code for MEMCMP. This forces the compiler to
 * use the definition in glue.h, which aliases a call to etc_memcmp.
 *
 * Revision 1.10  2001/01/19 22:23:14  paul
 * Update copyright.
 *
 * Revision 1.9  2000/11/16 21:29:39  paul
 * Moved glue_set_deferred_io_handler to sockport.h.
 *
 * Revision 1.8  2000/10/16 19:21:47  paul
 * Restore sockets and mempool code.
 *
 * Revision 1.7  2000/03/17 00:10:53  meister
 * Update copyright message
 *
 * Revision 1.6  2000/03/15 19:30:35  paul
 * Whoops, we actually needed one of those revs.
 *
 * Revision 1.5  2000/03/13 21:21:56  paul
 * Removed some code that we are no longer working on.
 *
 * Revision 1.4  1999/11/03 19:26:41  paul
 * Add attache/h/packet.h include for glue function with packet parameter
 *
 * Revision 1.3  1999/02/18 04:41:28  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 1.2.6.1  1998/10/22 21:09:57  wes
 * Change glue_set_deferred_io_handler prototype; add it to other ports.
 * (oops, missed this the other day).
 *
 * Revision 1.2  1998/07/29 20:54:46  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* Previous revision history when this was snark/port/attache.h
 * 
 * Revision 2.2  1996/10/16  16:49:55  sar
 * Added ifndefs around the memory and string routines
 * to keep compilers happy.
 *
 * Revision 2.1  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.6  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PREAMBLE with #ifdef, not #if.
 *
 * Revision 1.5  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.4  1994/12/20  22:13:46  sra
 * Fix signature for crufty glue_pkt_handle() declaration.
 *
 * Revision 1.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.2  1994/09/04  06:13:38  sra
 * Clean up antique type names and install macros.
 *
 * Revision 1.1  1994/01/09  23:57:29  sra
 * Add support for NIT under SunOS 4.1.2.
 *
 */

#ifdef CONFIG_PREAMBLE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>

#define	MAX_PACKETS			50
#define	IP_MAX_PKTSIZE			1536

#define TCP_MAXCONN			128

#ifndef MEMCPY
#define MEMCPY(x, y, z)			memcpy(x, y, z)
#endif

#ifndef MEMSET
#define MEMSET(x, y, z)			memset(x, y, z)
#endif

#ifndef STRCMP
#define STRCMP(x, y)			strcmp(x, y)
#endif

#ifndef STRCPY
#define STRCPY(x, y)			strcpy(x, y)
#endif

#ifndef STRLEN
#define STRLEN(x)			strlen(x)
#endif

/*
 * Fun with our new run-time debugging facility.
 * This is still experimental, and defaults to uninstalled.
 */
#if defined(INSTALL_SNARK_BUG_CONF) && INSTALL_SNARK_BUG_CONF
#include <wrn/wm/demo/bug_conf.h>
#endif

/*
 * Backwards compatability with old braindamage can be a real pain.
 * Since this port file is strictly for debugging, we set a couple
 * of configuration parameters to more reasonable values here.
 */

/* Please do install a retransmission timeout handler */
#define	TCP_DEFAULT_MAX_RETRANSMISSIONS	0

/* Please buffer a reasonable number of ahead-of-sequence segments */
#define TCP_AHEAD_Q_MAX_DEFAULT	25

#else /* CONFIG_PREAMBLE */

#include <wrn/wm/common/install.h>
#include <wrn/wm/attache/packet.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef htons
#undef htons
#endif

#ifdef htonl
#undef htonl
#endif

extern unsigned short htons (unsigned short);
extern unsigned long  htonl (unsigned long);

#define	GLUE_ALLOC(x)			((void *) malloc(x))
#define GLUE_FREE(x)			free((char *) (x))
#define	GLUE_CKSUM(x, y)		glue_cksum((bits16_t *)(x), (y))
#define	GLUE_SWAP16(x)			((bits16_t) htons((unsigned short) (x)))
#define	GLUE_SWAP32(x)			((bits32_t) htonl((unsigned long) (x)))
#define	GLUE_CALLIN(x, y)		glue_callin(x, y)

extern int glue_intlock (int);
extern bits16_t glue_cksum (bits16_t *, unsigned);
extern void glue_callin (bits32_t, void (*)(void));

#ifndef GLUE_NOW
#define	GLUE_NOW()			glue_now()
extern bits32_t glue_now (void);
#endif

/* define the GLUE_SNTP_SET_TIME macro if using SNTP Client */
#if INSTALL_ATTACHE_MULTICAST && INSTALL_ATTACHE_SNTP_CLIENT
#ifndef GLUE_SNTP_SET_TIME
#define GLUE_SNTP_SET_TIME(p, s, f)	glue_set_time((p),(s),(f))
extern void glue_set_time(packet *, bits32_t, bits32_t);
#endif
#endif

/* Define PRINTF for debugging and printing code 
#ifndef PRINTF
#define PRINTF printf
#endif
*/

#ifdef ATTACHE_IPSEC_H

#define	GLUE_IPSEC_SA_EXPIRED(W,X,Y,Z) \
        glue_ipsec_sa_expired(W,X,Y,Z)
extern void glue_ipsec_sa_expired (ipsec_sa_dir_t, ipaddr_t *, 
                                   bits32_t, bits8_t);

#define GLUE_IPSEC_SND_PKT_DENIED(W,X,Y,Z)  \
        glue_ipsec_snd_pkt_denied((W),(X),(Y),(Z))

extern void glue_ipsec_snd_pkt_denied(packet *, ipaddr_t *, ipaddr_t *, int);


#define GLUE_IPSEC_RCV_PKT_BADSPI(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_badspi((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_badspi(packet *, ipaddr_t *, ipaddr_t *, 
                                      bits32_t, bits8_t);

#define GLUE_IPSEC_RCV_PKT_NOTPARTY(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_notparty((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_notparty(packet *, ipaddr_t *, ipaddr_t *,
                                        bits32_t, bits8_t);


#define GLUE_IPSEC_RCV_TUNPKT_NOTIP(U,V,W,X,Y,Z) \
        glue_ipsec_rcv_tunpkt_notip((U),(V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_tunpkt_notip(packet *, packet *, ipaddr_t *, 
                                        ipaddr_t *, bits32_t, bits8_t);

#define GLUE_IPSEC_RCV_TUNPKT_NOTPARTY(S,T,U,V,W,X,Y,Z) \
        glue_ipsec_rcv_tunpkt_notparty((S),(T),(U),(V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_tunpkt_notparty(packet *, ipaddr_t *, ipaddr_t *, 
                                           packet *, ipaddr_t *, ipaddr_t *, 
                                           bits32_t, bits8_t);

#define GLUE_IPSEC_RCV_SA_NOTUNNEL(V,W,X,Y,Z) \
        glue_ipsec_rcv_sa_notunnel((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_sa_notunnel(packet *, ipaddr_t *, ipaddr_t *,
                                       bits32_t, bits8_t);


#define GLUE_IPSEC_RCV_PKT_BADAH(V,W,X,Y) \
        glue_ipsec_rcv_pkt_badah((V),(W),(X),(Y))

extern void glue_ipsec_rcv_pkt_badah(packet *, ipaddr_t *, ipaddr_t *, 
                                     bits32_t);

#define GLUE_IPSEC_RCV_PKT_DENIED(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_denied((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_denied(packet *, ipaddr_t *, ipaddr_t *,
                                      ipsec_policy_flags_t, bits16_t);


#endif /* ATTACHE_IPSEC */

#endif /* CONFIG_PREAMBLE */
