/* ip.h - ip head file */

/*
 * Copyright (c) 2001-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ip.h	8.2 (Berkeley) 6/1/94
 * $FreeBSD: src/sys/netinet/ip.h,v 1.25 2003/10/29 15:07:03 ume Exp $
 * $FreeBSD: src/sys/netinet/ip.h,v 1.17 1999/12/22 19:13:20 shin Exp $
 */

/*
modification history
--------------------
01l,09jan12,h_x  Move extern "C" after last header file include.
01k,19apr05,rp   merged from comp_wn_ipv6_mld_interim-dev
01j,15feb05,vvv  include in_systm.h only in kernel
01i,07feb05,vvv  _KERNEL cleanup
01h,14jan05,vvv  osdep.h cleanup
01g,20nov03,niq  Remove copyright_wrs.h file inclusion
01f,04nov03,rlm  Ran batch header path update for header re-org.
01e,03nov03,rlm  Removed wrn/coreip/ prefix from #includes for header re-org.
01d,07aug03,niq  Merging from Accordion label ACCORDION_BASE6_MERGE_BASELINE
01c,09may03,vvv  included in.h and in_systm.h
01b,09sep02,hsh  add c++ protection
01a,23aug01,ann  ported to clarinet from freeBSD 4.3 version 1.17
 */

#ifndef _NETINET_IP_H_
#define _NETINET_IP_H_

#include <netinet/in.h>

#ifdef _WRS_KERNEL
#include <netinet/in_systm.h>
#endif

#ifdef __cplusplus
 extern "C" {
#endif

/*
 * Definitions for internet protocol version 4.
 * Per RFC 791, September 1981.
 */
#define	IPVERSION	4

/*
 * Structure of an internet header, naked of options.
 */
struct ip {
#ifdef _IP_VHL
	u_char	ip_vhl;			/* version << 4 | header length >> 2 */
#else
#if _BYTE_ORDER == _LITTLE_ENDIAN
	u_int	ip_hl:4,		/* header length */
		ip_v:4;			/* version */
#endif
#if _BYTE_ORDER == _BIG_ENDIAN
	u_int	ip_v:4,			/* version */
		ip_hl:4;		/* header length */
#endif
#endif /* not _IP_VHL */
	u_char	ip_tos;			/* type of service */
	u_short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	u_short	ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
};

#ifdef _IP_VHL
#define	IP_MAKE_VHL(v, hl)	((v) << 4 | (hl))
#define	IP_VHL_HL(vhl)		((vhl) & 0x0f)
#define	IP_VHL_V(vhl)		((vhl) >> 4)
#define	IP_VHL_BORING		0x45
#endif

#define	IP_MAXPACKET	65535		/* maximum packet size */

/*
 * Definitions for IP type of service (ip_tos)
 */
#define	IPTOS_LOWDELAY		0x10
#define	IPTOS_THROUGHPUT	0x08
#define	IPTOS_RELIABILITY	0x04
#define	IPTOS_MINCOST		0x02
#if 1
/* ECN RFC3168 obsoletes RFC2481, and these will be deprecated soon. */
/* ECN bits proposed by Sally Floyd */
#define	IPTOS_CE		0x01	/* congestion experienced */
#define	IPTOS_ECT		0x02	/* ECN-capable transport */
#endif

/*
 * Definitions for IP precedence (also in ip_tos) (hopefully unused)
 */
#define	IPTOS_PREC_NETCONTROL		0xe0
#define	IPTOS_PREC_INTERNETCONTROL	0xc0
#define	IPTOS_PREC_CRITIC_ECP		0xa0
#define	IPTOS_PREC_FLASHOVERRIDE	0x80
#define	IPTOS_PREC_FLASH		0x60
#define	IPTOS_PREC_IMMEDIATE		0x40
#define	IPTOS_PREC_PRIORITY		0x20
#define	IPTOS_PREC_ROUTINE		0x00

/*
 * ECN (Explicit Congestion Notification) codepoints in RFC3168
 * mapped to the lower 2 bits of the TOS field.
 */
#define	IPTOS_ECN_NOTECT	0x00	/* not-ECT */
#define	IPTOS_ECN_ECT1		0x01	/* ECN-capable transport (1) */
#define	IPTOS_ECN_ECT0		0x02	/* ECN-capable transport (0) */
#define	IPTOS_ECN_CE		0x03	/* congestion experienced */
#define	IPTOS_ECN_MASK		0x03	/* ECN field mask */

/*
 * Definitions for options.
 */
#define	IPOPT_COPIED(o)		((o)&0x80)
#define	IPOPT_CLASS(o)		((o)&0x60)
#define	IPOPT_NUMBER(o)		((o)&0x1f)

#define	IPOPT_CONTROL		0x00
#define	IPOPT_RESERVED1		0x20
#define	IPOPT_DEBMEAS		0x40
#define	IPOPT_RESERVED2		0x60

#define	IPOPT_EOL		0		/* end of option list */
#define	IPOPT_NOP		1		/* no operation */

#define	IPOPT_RR		7		/* record packet route */
#define	IPOPT_TS		68		/* timestamp */
#define	IPOPT_SECURITY		130		/* provide s,c,h,tcc */
#define	IPOPT_LSRR		131		/* loose source route */
#define	IPOPT_ESO		133		/* extended security */
#define	IPOPT_CIPSO		134		/* commerical security */
#define	IPOPT_SATID		136		/* satnet id */
#define	IPOPT_SSRR		137		/* strict source route */
#define	IPOPT_RA		148		/* router alert */

/*
 * Offsets to fields in options other than EOL and NOP.
 */
#define	IPOPT_OPTVAL		0		/* option ID */
#define	IPOPT_OLEN		1		/* option length */
#define IPOPT_OFFSET		2		/* offset within option */
#define	IPOPT_MINOFF		4		/* min value of above */

/*
 * Time stamp option structure.
 */
struct	ip_timestamp {
	u_char	ipt_code;		/* IPOPT_TS */
	u_char	ipt_len;		/* size of structure (variable) */
	u_char	ipt_ptr;		/* index of current entry */
#if _BYTE_ORDER == _LITTLE_ENDIAN
	u_int	ipt_flg:4,		/* flags, see below */
		ipt_oflw:4;		/* overflow counter */
#endif
#if _BYTE_ORDER == _BIG_ENDIAN
	u_int	ipt_oflw:4,		/* overflow counter */
		ipt_flg:4;		/* flags, see below */
#endif
	union ipt_timestamp {
		u_long	ipt_time[1];
		struct	ipt_ta {
			struct in_addr ipt_addr;
			u_long ipt_time;
		} ipt_ta[1];
	} ipt_timestamp;
};

/* flag bits for ipt_flg */
#define	IPOPT_TS_TSONLY		0		/* timestamps only */
#define	IPOPT_TS_TSANDADDR	1		/* timestamps and addresses */
#define	IPOPT_TS_PRESPEC	3		/* specified modules only */

/* bits for security (not byte swapped) */
#define	IPOPT_SECUR_UNCLASS	0x0000
#define	IPOPT_SECUR_CONFID	0xf135
#define	IPOPT_SECUR_EFTO	0x789a
#define	IPOPT_SECUR_MMMM	0xbc4d
#define	IPOPT_SECUR_RESTR	0xaf13
#define	IPOPT_SECUR_SECRET	0xd788
#define	IPOPT_SECUR_TOPSECRET	0x6bc5

/*
 * Internet implementation parameters.
 */
#define	MAXTTL		255		/* maximum time to live (seconds) */
#define	IPDEFTTL	64		/* default ttl, from RFC 1340 */
#define	IPFRAGTTL	60		/* time to live for frags, slowhz */
#define	IPTTLDEC	1		/* subtracted when forwarding */

#define	IP_MSS		576		/* default maximum segment size */

#ifdef __cplusplus
}
#endif

#endif
