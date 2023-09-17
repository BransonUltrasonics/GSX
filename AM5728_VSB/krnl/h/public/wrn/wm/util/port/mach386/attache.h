/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/mach386/attache.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: attache.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:12  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:23:01  paul
 * Update copyright.
 *
 * Revision 1.6  2000/03/17 00:10:19  meister
 * Update copyright message
 *
 * Revision 1.5  1998/03/12 18:23:43  meister
 * GLUE_IPSEC_RCV_SA_NOTUNNEL()
 *
 * Revision 1.4  1998/03/11 21:11:12  meister
 * Added some parameters to some IPSEC glue functions
 *
 * Revision 1.3  1998/02/27 22:30:09  meister
 * Added IPSEC glue function defs
 *
 * Revision 1.2  1998/02/25 04:56:52  sra
 * Update copyrights.
 *
 * Revision 1.1  1998/02/18 22:34:58  sra
 * Add ip_loopback(), eradicate glue_pkt_handle().
 *
 * Revision 1.0  1997/08/08 22:11:16  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:58:06  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/28 20:30:07  sar
 * Initial version
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* Previous revision history from mach386/attache.h
 * with release update messages removed
 *
 * Revision 2.4  1994/12/20  21:19:11  lowell
 * worked around problems between Mach's memory.h and
 * the builtin versions provided by GCC...
 *
 * Revision 2.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 2.2  1994/09/04  06:13:38  sra
 * Clean up antique type names and install macros.
 *
 * Revision 1.6  1993/08/03  23:40:54  dab
 * CONFIG_PREAMBLE is either defined or not, so use #ifdef
 *
 * Revision 1.5  1993/08/02  21:58:54  dab
 * moved MEM* and STR* to the first section
 *
 * Revision 1.4  1993/04/21  15:30:10  dab
 * Default configuration moved to <attache/h/config.h>
 *
 * Revision 1.2  1993/02/28  17:25:09  dab
 * If set SGRP to 1 instead of just on then matches what happens in
 * envoy/h/asn1conf.h.
 *
 */

#ifdef CONFIG_PREAMBLE

/* Things which MAY be defined in here:
 *
 * MAX_PACKETS
 * STATIC_PACKETS
 * MAX_ARP_ENTRIES
 * ARP_EXPIRATION
 * ARP_TIMER_FREQUENCY
 * ARP_TIMEOUT
 * PRINTF
 * DEFAULT_IP_TTL
 * MAX_REASM
 * IP_REASM_TIMEOUT
 * TFTP_TIMEOUT
 */

/* Define MAX_PACKETS in order to preallocate packets using glue_alloc().
 * MAX_PACKETS value is the number of packets to preallocate.
 *
 * Define STATIC_PACKETS to have MAX_PACKETS packet buffers allocated
 * from a static array. STATIC_PACKETS should be the packet size
 * to allocate.
 */
#define	MAX_PACKETS	20
/*#define	STATIC_PACKETS	1536 */

/* Define IP_MAX_PKTSIZE to be the maximum reassembly buffer size. 
 * This isn't the largest packet that can be reassembled because some of the
 * buffer space is taken up with other things.
 * Also, ptcp_init() will preallocate packets of this size at startup
 */
#ifdef STATIC_PACKETS
#define IP_MAX_PKTSIZE STATIC_PACKETS
#else
#define	IP_MAX_PKTSIZE	1536
#endif

/* Declarations for various standard C functions needed by Attache.
 * These later may be declared by just including system include files
 * or the functions may be explcitly declared here.  The standard C
 * functions needed by Attache are:
 *
 * memcpy memcmp memset strcmp strcpy strlen
 * 
 */

/* The standard C functions provided by Mach 386 are declared with
 * char* instead of void*.  Rather than using Mach's declarations,
 * we'll use gcc's builtin functions where they're available:
 * memset() isn't a builtin, so we'll just define it here, gcc-style. */
void * memset(void *, int, size_t);


#include <strings.h>

/* If you're using Attache along with Envoy, these macros will be
 * needed in envoy.h as well.  Make sure they are identical so the
 * C-preprocessor doesn't complain about them being redefined. */
#define MEMCPY(dst, src, n) memcpy((char *)(dst), (char *)(src), n)
#define MEMCMP(s1, s2, n) memcmp((char *)(s1), (char *)(s2), n)
#define MEMSET(dst, val, n) memset((char *)(dst), val, n)


#else /* CONFIG_PREAMBLE */


/* glue routines to interface to the outside world.
 */
int glue_intlock (int);	/* lock interrupts, return old state */

/* generic memory allocation routines
 */
#define	GLUE_ALLOC(len)	malloc(len)
#define GLUE_FREE(mem)	free(mem)

/* Define this one if you want your own packet allocation routine. Both
 * GLUE_PKTALLOC() and GLUE_PKTFREE() must be defined or undefined.
 */
/* #define	GLUE_PKTALLOC(len)	gettapkt(len)	*/
/* #define	GLUE_PKTFREE(p)		givepkt(p)	*/

/* Define GLUE_PKTPRIVATE_FREE(x) to be a routine to free the pkt_private
 * value, if you need to do that.
 */
/* #define	GLUE_PKTPRIVATE_FREE(x)	priv_free(x)	*/

/* Define GLUE_CKSUM to be a function that computes the IP checksum
 * We provide a C routine in mach386/glue.c
 */
#define	GLUE_CKSUM(buf, word_count)	cksum((bits16_t *)(buf), (word_count))

/* Define GLUE_SWAP16 and GLUE_SWAP32 iff your machine is little endian and
 * thus SWAP16 and SWAP32 are defined. These functions are provided in
 * mach386/glue.c
 */
#ifdef	SWAP16
#define	GLUE_SWAP16(x)	swap16(x)
#endif /* SWAP16 */

#ifdef	SWAP32
#define	GLUE_SWAP32(x)	swap32(x)
#endif /* SWAP32 */

/* GLUE time functions
 *
 *	GLUE_NOW() returns a 32 bit number in milliseconds. It's used as a
 * relative, not absolute, number. Just keep it to whatever resolution is
 * convenient, but make sure it is a monotonically increasing number.
 */
#define	GLUE_NOW()	glue_now()

/*	GLUE_CALLIN() schedules a function to be called in ms milliseconds.
 * It only has to remember one function; when it is called while a call is
 * already scheduled, the previous settings can be forgotten.
 * It should guarantee that when the function is called, GLUE_NOW() will
 * be at least ms greater than it was when GLUE_CALLIN() was called (ie:
 * make sure you call later rather than earlier)
 *	Also, be sure that from whatever context handler is called, it's
 * safe to make calls into the protocol stack, for instance, into udp_sendto()
 * or similar functions.
 */
#define	GLUE_CALLIN(ms, handler)	callin(ms, handler)

/* Declare all the previously used glue functions */
bits16_t cksum (bits16_t *, unsigned);
bits16_t swap16 (bits16_t);
bits32_t swap32 (bits32_t);
bits32_t glue_now (void);
void callin (bits32_t, void (*)(void));
void *malloc (unsigned);
void free (void *);


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







