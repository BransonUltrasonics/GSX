/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/attache.h,v 1.2 2002/02/26 18:22:53 josh Exp $ */

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
 * Revision 1.2  2002/02/26 18:22:53  josh
 * updating paths in port files
 *
 * Revision 1.1.1.1  2001/11/05 17:47:12  tneale
 * Tornado shuffle
 *
 * Revision 1.12  2001/01/19 22:23:03  paul
 * Update copyright.
 *
 * Revision 1.11  2000/03/17 00:10:25  meister
 * Update copyright message
 *
 * Revision 1.10  1999/04/28 23:53:25  sra
 * Clean up various minor installation option glitches.
 *
 * Revision 1.9  1998/09/06 08:11:06  sra
 * Packet filtering hooks in Attache, rudimentary firewall
 * implementation in Snark.
 *
 * Revision 1.8  1998/03/12 18:23:42  meister
 * GLUE_IPSEC_RCV_SA_NOTUNNEL()
 *
 * Revision 1.7  1998/03/11 21:08:28  meister
 * Added extra parameters to a couple of IPSEC glue macros
 *
 * Revision 1.6  1998/03/03 15:26:16  mrf
 * Comment out PRINTF declarations.
 *
 * Revision 1.5  1998/02/28 01:40:00  meister
 * Added stdio and a PRINTF
 *
 * Revision 1.4  1998/02/27 22:31:01  meister
 * Added ipsec glue functions
 *
 * Revision 1.3  1998/02/25 04:56:56  sra
 * Update copyrights.
 *
 * Revision 1.2  1998/02/18 22:34:59  sra
 * Add ip_loopback(), eradicate glue_pkt_handle().
 *
 * Revision 1.1  1997/09/15 15:48:27  sra
 * GLUE_NOW() migration version skew.
 *
 * Revision 1.0  1997/08/08 22:11:19  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.5  1997/03/20 06:57:47  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.4  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.3  1997/01/08 01:08:19  sar
 * as part of changing snmptalk to use the snark suite we can remove
 * the ifdefs for snark in this file.
 *
 * Revision 0.2  1996/10/26  00:43:57  sar
 * put the #else for config_preamble in the right place
 *
 * Revision 0.1  1996/10/23  00:12:55  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* previous history from attache/port/msdos/attache.h
 *
 * Revision 3.1  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 3.0  1995/05/10  22:37:24  sra
 * Release 3.0.
 *
 * Revision 2.4  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 2.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 2.2  1994/09/04  06:13:38  sra
 * Clean up antique type names and install macros.
 *
 * Revision 2.1  1993/08/06  22:03:43  sra
 * Release 2.0.
 *
 * Revision 1.8  1993/08/03  23:19:38  dab
 * CONFIG_PREAMBLE is either defined or not, so use ifdef
 *
 * Revision 1.7  1993/08/02  21:59:13  dab
 * moved MEM* and STR* to the first section
 *
 * Revision 1.6  1993/05/20  20:22:21  sar
 * Removed memory.h if the compiler is watcom or intel 960
 *
 * Revision 1.5  1993/05/17  20:11:55  sar
 * Added an ifdef for zortech to remove the include of memory.h and
 * to add the define of NEEDS_STR{DUP ICMP NICMP} as they weren't
 * available.
 *
 * Revision 1.4  1993/04/21  15:30:59  dab
 * Default configuration moved to <attache/h/config.h>
 *
 * Revision 1.3  1993/02/28  18:28:38  dab
 * Changed "#define SGRP" to "#define SGRP 1" so it matches envoy/h/asn1conf.h
 * so we don't get warnings.
 * Added include of <memory.h> and <string.h>.
 *
 * Revision 1.2  1993/02/27  17:31:59  dab
 * Merged in stuff from glue.h
 *
 * Revision 1.1  1993/02/18  21:50:33  dab
 * Initial revision
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

#define	TCP_DEFAULT_MAX_RETRANSMISSIONS	0

#else /* CONFIG_PREAMBLE */

/* Declarations for various standard C functions needed by Attache.
 * These later may be declared by just including system include files
 * or the functions may be explcitly declared here.  The standard C
 * functions needed by Attache are:
 *
 * memcpy memcmp memset strcmp strcpy strlen
 * 
 */
/* microsoft and borland have a memory.h so include it */
#if defined(INSTALL_on_microsoft) || defined(INSTALL_on_bcc)
#include <memory.h>
#endif

#include <string.h>
#include <stdio.h>

/* glue routines to interface to the outside world. */

/* lock interrupts, return old state */
extern int      glue_intlock (int);

/* generic memory allocation routines  */
#define	GLUE_ALLOC(len)	((void *) malloc(len))
#define GLUE_FREE(mem)	free((char *)(mem))

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
 */
#define	GLUE_CKSUM(x, y)		glue_cksum((bits16_t *)(x), (y))

/* Define GLUE_SWAP16 and GLUE_SWAP32 iff your machine is little endian and
 * thus SWAP16 and SWAP32 are defined. Supplied are definitions for PC-DOS.
 */
#define	GLUE_SWAP16(x)	glue_swap16(x)
#define	GLUE_SWAP32(x)  glue_swap32(x)

/* GLUE time functions
 *
 *	GLUE_NOW() returns a 32 bit number in milliseconds. It's used as a
 * relative, not absolute, number. Just keep it to whatever resolution is
 * convenient, but make sure it is a monotonically increasing number.
 */
#ifndef GLUE_NOW
#define	GLUE_NOW()	glue_now()
#endif

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
#define	GLUE_CALLIN(x, y)		glue_callin(x, y)

/* here we include files that define functions (like malloc(), for instance)
 * that may be glue routines or declare the functions directly
 */
#include <stdlib.h>

extern bits32_t glue_now (void);

extern bits16_t glue_cksum   (bits16_t *, unsigned);
extern void     glue_callin  (bits32_t, void (*)(void));
extern bits16_t glue_swap16  (bits16_t);
extern bits32_t glue_swap32  (bits32_t);

#ifndef MEMCPY
#define MEMCPY(x, y, z)			memcpy(x, y, z)
#endif
#ifndef MEMCMP
#define MEMCMP(x, y, z)			memcmp(x, y, z)
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

/* Define PRINTF to build debugging/printing code
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

#define GLUE_IPSEC_RCV_SA_NOTUNNEL(V,W,X,Y,Z) \
        glue_ipsec_rcv_sa_notunnel((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_sa_notunnel(packet *, ipaddr_t *, ipaddr_t *,
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

#define GLUE_IPSEC_RCV_PKT_BADAH(V,W,X,Y) \
        glue_ipsec_rcv_pkt_badah((V),(W),(X),(Y))

extern void glue_ipsec_rcv_pkt_badah(packet *, ipaddr_t *, ipaddr_t *, 
                                     bits32_t);

#define GLUE_IPSEC_RCV_PKT_DENIED(V,W,X,Y,Z) \
        glue_ipsec_rcv_pkt_denied((V),(W),(X),(Y),(Z))

extern void glue_ipsec_rcv_pkt_denied(packet *, ipaddr_t *, ipaddr_t *,
                                      ipsec_policy_flags_t, bits16_t);


#endif /* ATTACHE_IPSEC */

#if defined(INSTALL_SNARK_FIREWALL) && INSTALL_SNARK_FIREWALL
#include <wrn/wm/demo/firewall.h>
#endif

#endif /* CONFIG_PREAMBLE */
