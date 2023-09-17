/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/asn1conf.h,v 1.3 2003/01/15 13:58:56 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * $Log: asn1conf.h,v $
 * Revision 1.3  2003/01/15 13:58:56  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/06 21:50:40  josh
 * second (and hopefully final) pass of new path hacking
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.9.4.3  2001/09/21 19:14:28  josh
 * an extra check:  can't install coexistence if neither SNMPv1
 * nor SNMPv2 are installed
 *
 * Revision 9.9.4.2  2001/08/24 14:32:10  josh
 * a variety of option checks to make sure we don't have conflicting
 * options installed
 *
 * Revision 9.9.4.1  2001/06/28 18:22:56  josh
 * Seeding the Cormorant branch with the code from the old
 * cormorant branch
 *
 * Revision 9.9  2001/04/11 20:58:32  josh
 * we are overwriting the trunk's copy of this file with
 * the file from the kingfisher branch
 *
 * Revision 9.8  2001/01/19 22:22:02  paul
 * Update copyright.
 *
 * Revision 9.7  2000/07/10 15:55:13  mrf
 * Add configuration for Cygnus NT environment with associated changes.
 *
 * Revision 9.6  2000/06/09 14:54:12  josh
 * modifications due to new installation options, new proxy code
 * moved some definitions from sendntfy.h into v3_trgt.h so they'd
 * be built with the target code
 *
 * Revision 9.5.4.1  2001/04/11 20:48:29  josh
 * moving cormorant-specific changes off to the cormorant
 * branch
 *
 * Revision 9.5.2.2  2001/03/12 22:07:50  tneale
 * Updated copyright
 *
 * Revision 9.5.2.1  2000/09/21 21:14:52  josh
 * bringing branch include files in line with root
 *
 * Revision 9.5  2000/03/17 19:48:28  meister
 * Update copyright notice
 *
 * Revision 9.4  2000/03/09 20:58:32  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.3  2000/03/09 16:54:09  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.2  1999/06/01 15:46:14  josh
 * Moving some installation option checking.
 *
 * Revision 9.1  1999/04/09 20:36:59  josh
 * Obsoleting ENVOY_SNMP_VERSON_USEC and ENVOY_40_VB_FREE_COMPAT
 *
 * Revision 9.0  1998/10/16 22:10:04  sar
 * Update version stamp to match release
 *
 * Revision 8.8  1998/08/04 02:02:17  sar
 * Added some macros for use with longer term storage, if the user
 * hasn't defined them they default to the standard ones.
 *
 * Revision 8.7  1998/06/23 20:28:23  sar
 * Added requirement for view2275 if v3 is installed
 *
 * Revision 8.6  1998/06/09 21:34:56  sar
 * Added MEMCMP_NULLOK
 *
 * Revision 8.5  1998/06/08 19:42:10  sar
 * Create a define to tell us if we need the stats group in general
 *
 * Revision 8.4  1998/05/29 03:53:54  sar
 * Add and use defines for v2 types and v2 protocol pieces.  This allows
 * us to include the types and pieces for other versions, such as v3,
 * without having to touch all of the seperate files that use the
 * types or pieces.
 *
 * Revision 8.3  1998/05/23 19:55:38  sar
 * As part of adding rfc2275 made a switch to mesh 2275 and 1445.
 *
 * Revision 8.2  1998/02/25 15:21:23  sra
 * Finish moving types.h, bug.h, and bugdef.h to common/h/.
 *
 * Revision 8.1  1998/02/25 04:50:22  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:20  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/10/30 04:23:16  sar
 * Update type information mostly convert bits16_ts to ALENGTH_Ts
 * also add MAX_ALENGTH
 *
 * Revision 7.4  1997/10/22 03:07:28  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.3  1997/08/21 17:23:44  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 7.2  1997/03/20 06:47:59  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 */

/* [clearcase]
modification history
-------------------
01b,26jul08,y_t  fix the incompatible for STL
01a,19apr05,job  update copyright notices
*/


#if !defined(asn1conf_inc)
#define asn1conf_inc

#ifdef __cplusplus
extern"C" {
#endif

#if !defined(EPILOGUE_INSTALL_H)
#include <wrn/wm/common/install.h>
#endif

/* Get the standard types and the install defines */
#if !defined(EPILOGUE_TYPES_H)
#include <wrn/wm/common/types.h>
#endif

/* !!! These are here for backwards compatibility only while we're switching
 * over to the new configuration system. */
#if (INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2)
#define SGRP 1
#endif

/* This is to provide compatibility for windnet users because the
 * windnet version of envoy had all struct declarations bracketted 
 * with an alignment pragma based on this test.  We will follow
 * suit but put the test in a single place (here). */
#if INSTALL_ENVOY_ALIGN_STRUCTURES
#define WINDNET_STRUCT_ALIGN
#endif

/*****************************************************************
 * Portable types definitions for backwards compatibility.  These
 * are now defined in terms of things defined by <common/h/types.h> and
 * therefore <oemtypes.h> so any changes should be made in
 * <oemtypes.h>.
 *****************************************************************/
	/* various sizes of integers */
typedef sbits32_t	INT_32_T;
typedef bits32_t	UINT_32_T;
typedef sbits16_t	INT_16_T;
typedef bits16_t 	UINT_16_T;
typedef bits8_t		UINT_8_T;
typedef bits8_t		OCTET_T ;
	/* A generic pointer type.  Use void * if your compiler can hack
	 * it, otherwise char * is probably the right thing. */
typedef ptr_t		PTR_T;
	/* SNMP ASN.1 tags and lengths can always fit into 16 bits...	*/
typedef bits16_t 	ATVALUE_T;	/* ASN.1 type values		*/
typedef bits16_t 	ALENGTH_T;	/* ASN.1 contents field	lengths	*/
	/* this is used as both a maximum and a mask */
#define MAX_ALENGTH     0xFFFF

#define OCTETS_PER_INT32    (sizeof(INT_32_T)/sizeof(OCTET_T))
#define OCTETS_PER_INT16    (sizeof(INT_16_T)/sizeof(OCTET_T))


/*
 * get the common system configuration
 */
#include <wrn/wm/common/config.h>

/*
 * get the system configuration for envoy
 */
#define CONFIG_PREAMBLE 1
#include <envoy.h>
#undef CONFIG_PREAMBLE


/* Long lived dynamic storage.  If the user doesn't define
   them explicitly we use the shorter term ones */
#ifndef SNMP_memory_alloc_lt
#define SNMP_memory_alloc_lt(need) SNMP_memory_alloc(need)
#endif

#ifndef SNMP_memory_free_lt
#define SNMP_memory_free_lt(buf) SNMP_memory_free(buf)
#endif

/*****************************************************************
 * Set the size of object identifier components.  32 bits is recommended
 * but 16 bits may work and may save a little space.
 * This may be overridden in <oemtypes.h> by defining __TYPES_HAVE_OIDC_T_
 *****************************************************************/
#ifndef __TYPES_HAVE_OIDC_T_
typedef	bits32_t	OIDC_T;
#endif

/* min and max macros if not provided by the system */
#ifndef NOMINMAX 
#if (!defined(max))
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#if (!defined(min))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#endif



/* These are the standard C functions that we use in Envoy.  If the
 * customer's compiler supplies these correctly then nothing more need be
 * done.  If not, then <envoy.h> can override.  The most common case is
 * that the system provides them but with the wrong type arguments (char *
 * instead of void *).  The macros can then fix this with casts. */
#ifndef MEMCPY
#define MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif
#ifndef MEMSET
#define MEMSET(dst, val, n) memset(dst, val, n)
#endif
#ifndef MEMCMP
#define MEMCMP(s1, s2, n) memcmp(s1, s2, n)
#endif
#ifndef STRLEN
#define STRLEN(s) strlen(s)
#endif

#ifndef MEMCMP_NULLOK
#define MEMCMP_NULLOK(s1, s2, n) (n ? MEMCMP(s1, s2, n) : 0)
#endif

/* Error checking, Look for inconsistencies or necessary configuration
   that hasn't been done */

/* Do we need any of the stats.  For v1 & v2 there is a seperate
   installation option for the stats groups, for v3 the stats
   are required for reports so installing v3 installs the groups */
#if (INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2 || \
     INSTALL_ENVOY_SNMP_VERSION_3)
#define ENVOY_NEED_STATS 1
#else
#define ENVOY_NEED_STATS 0
#endif

/* See if we need either v2 types or protocols, we do this
   to avoid needing to add new install option whenever a new
   version is defined */
#if (INSTALL_ENVOY_SNMP_USE_V2_TYPES || INSTALL_ENVOY_SNMP_VERSION_2 || \
     INSTALL_ENVOY_SNMP_VERSION_3)
#define ENVOY_USE_V2_TYPES 1
#else
#define ENVOY_USE_V2_TYPES 0
#endif

#if (INSTALL_ENVOY_SNMP_VERSION_2 || INSTALL_ENVOY_SNMP_VERSION_3)
#define ENVOY_USE_V2_PROTOS 1
#else
#define ENVOY_USE_V2_PROTOS 0
#endif

/* If version 3 is installed we must have 2275 views */
#if ((INSTALL_ENVOY_SNMP_VERSION_3) && !(INSTALL_ENVOY_SNMP_RFC2275_VIEWS))
#error If you are installing version 3 you must also install rfc2275
#error views using _ENVOY_SNMP_RFC2275_VIEWS.
#endif

/* check that only one of the view schemes is turned on, 
   if one is turned on turn on the flex view option for use
   elsewhere */
#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS && INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
#error Must not have both the rfc1445 and rfc2275 view schemes installed.
#error You should modify the configuration file and remove either the
#error ENVOY_SNMP_RFC1445_VIEWS or ENVOY_SNMP_RFC2275_VIEWS option
#endif

#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS || INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
#define INSTALL_ENVOY_SNMP_DYNAMIC_VIEWS 1
#else
#define INSTALL_ENVOY_SNMP_DYNAMIC_VIEWS 0
#endif

/* Check that that the lock & serialization options are consistent */
#if (INSTALL_ENVOY_SNMP_LOCK && INSTALL_ENVOY_SNMP_SERIALIZE)
#error Must not have both the lock code and the serialization code installed.
#error You should modify the configuration file and remove either
#error the ENVOY_SNMP_LOCK or the ENVOY_SNMP_SERIALIZE option
#endif

#if (INSTALL_ENVOY_CONTINUE_REENTRANT && !INSTALL_ENVOY_SNMP_LOCK)
#error Must have locks installed to use reentrant continues.
#error You should modify the configuration file and install
#error the ENVOY_SNMP_LOCK option.
#endif

/* check that the options for the agentx scheme are consistent */
#if ((INSTALL_ENVOY_AGENTX_MASTER || INSTALL_ENVOY_AGENTX_SUB) && \
     !(INSTALL_ENVOY_AGENTX))
#error You must install ENVOY_AGENTX with ENVOY_AGENTX_MASTER
#error and ENVOY_AGENTX_SUB
#endif


#if (INSTALL_ENVOY_AGENTX_MASTER)
#if (INSTALL_ENVOY_MAIN_AGENT && !(INSTALL_ENVOY_SNMP_DYNAMIC_VIEWS))
#error You must install ENVOY_SNMP_RFC1445_VIEWS or
#error ENVOY_SNMP_RFC2275_VIEWS for use with a master agent
#endif

#if (!INSTALL_ENVOY_SNMP_LOCK && !INSTALL_ENVOY_SNMP_SERIALIZE)
#error Must have either the locking scheme or the serialization scheme
#error installed.  You should modify the configuration file and install
#error either the ENVOY_SNMP_LOCK or ENVOY_SNMP_SERIALIZE option
#endif
#endif

/* Check that the various SNMPv3 components are consistent */
#if ((INSTALL_ENVOY_SNMP_V3_TARGET) && !(INSTALL_ENVOY_SNMP_VERSION_3))
#error The target code (ENVOY_SNMP_V3_TARGET) requires SNMPv3 support.
#error Either uninstall ENVOY_SNMP_V3_TARGET or install ENVOY_SNMP_VERSION_3.
#endif

#if !(INSTALL_ENVOY_SNMP_VERSION_1 || INSTALL_ENVOY_SNMP_VERSION_2) && \
      INSTALL_ENVOY_SNMP_COEXISTENCE
#error If coexistence is installed, either SNMPv1 or SNMPv2 must also
#error be installed.
#endif

#if ((INSTALL_ENVOY_SNMP_V3_NOTIFY || INSTALL_ENVOY_SNMP_V3_PROXY || \
      INSTALL_ENVOY_SNMP_COEXISTENCE) && !(INSTALL_ENVOY_SNMP_V3_TARGET))
#error The use of notifies, proxies, or coexistence requires that 
#error ENVOY_SNMP_V3_TARGET be installed.
#endif

#if ((INSTALL_ENVOY_SNMP_V3_PROXY) && (INSTALL_ENVOY_SNMP_PROXY))
#error ENVOY_SNMP_V3_PROXY and ENVOY_SNMP_PROXY cannot both be installed.
#endif

#if ((INSTALL_ENVOY_SNMP_V3_PROXY) || (INSTALL_ENVOY_SNMP_PROXY))
#define INSTALL_ENVOY_USE_SNMP_PROXY 1
#else
#define INSTALL_ENVOY_USE_SNMP_PROXY 0
#endif

/* If we don't have a main agent we shouldn't be counting stats */
#if ((INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2) && \
     !(INSTALL_ENVOY_MAIN_AGENT))
#error Must not have the stats groups installed if building only a 
#error sub agent.  You should modify the configuration file
#error for the makmak program and either uninstall the SNMP_GROUP
#error options, add the MASTER agent option or remove the SUB agent option
#endif

#if INSTALL_ENVOY_SNMP_VERSION_USEC
#error Envoy no longer supports USEC.  Please uninstall 
#error ENVOY_SNMP_VERSION_USEC and recompile.
#endif

#if INSTALL_ENVOY_40_VB_FREE_COMPAT
#error ENVOY_40_VB_FREE_COMPAT is no longer a valid option.
#error Please uninstall it and recompile.
#endif

#if (INSTALL_SNMP_VXWORKS_VIRTUAL_STACK && \
     INSTALL_SNMP_VXWORKS_VS_ENTITY && \
     !(INSTALL_ENVOY_ENTITY_MIB))
#error You must install ENVOY_ENTITY_MIB for use with SNMP_VXWORKS_VS_ENTITY
#endif
 
#ifdef __cplusplus
}
#endif

#endif /* asn1conf_inc */
