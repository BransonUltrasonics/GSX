/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/vxworks/envoy.h,v 1.6 2002/05/23 19:42:41 josh Exp $ */

/*
 * Copyright (C) 1999-2005, 2014 Wind River Systems, Inc.
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
 ****************************************************************************/

/*
modification history
--------------------
30may14,bbz  resolve snmp engine's dependence on snmp agent(V7MAN-82)
02may05,job  vxWorks-specific definition of SNMP_DH_RAND moves here
19apr05,job  update copyright notices
06oct04,job  draw processor alignment from vxWorks header files
26nov03,ps  copyright update
05nov03,ps  merge from latest on ickthwa_devel_branch.nh
19apr98,ann changed ENVOY_SNMP_INIT_LOCK macro
04jun96,rjc changed #if defined to #if
01may96,rjc Added copyright header
*/

/* envoy/port/sunos/envoy.h
 *    configuration for Envoy on VxWorks
 */

/*
 * $Date: 2002/05/23 19:42:41 $
 * $Revision: 1.6 $
 * $Author: josh $
 *
 * $Log: envoy.h,v $
 * Revision 1.6  2002/05/23 19:42:41  josh
 * updates to support the entity MIB
 *
 * Revision 1.5  2002/04/05 13:45:18  andre
 * Add ENVOY_AX_GET_CONNID user-exit.
 *
 * Revision 1.4  2002/04/04 15:23:15  andre
 * Clean up; change enum params to int.
 *
 * Revision 1.2  2001/11/07 14:26:53  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.7.4.3  2001/10/29 21:42:50  josh
 * adding a spot for the proxy forwarding routine
 *
 * Revision 1.7.4.2  2001/09/24 16:44:39  josh
 * add a prototype for a function
 *
 * Revision 1.7.4.1  2001/08/24 15:35:20  josh
 * add nonvolatile access definitions for proxy and community tables
 *
 * Revision 1.7  2001/04/13 18:09:25  josh
 * merging in changes from kingfisher branch
 *
 * Revision 1.6  2000/07/26 19:39:31  josh
 * fixing a broken comment
 *
 * Revision 1.5  2000/06/09 14:57:35  josh
 * updating header files to correspond with new installation options and
 * new code
 *
 * Revision 1.4.2.5  2001/03/12 22:09:03  tneale
 * Updated copyright
 *
 * Revision 1.4.2.4  2000/12/07 22:09:56  tneale
 * Added definition for ENVOY_AX_SA_MIB_ROOT_NODE
 *
 * Revision 1.4.2.3  2000/10/26 21:36:00  josh
 * fixing a few bugs in the vxWorks AgentX master agent
 *
 * Revision 1.4.2.2  2000/10/18 20:05:41  josh
 * added a definition for ENVOY_SNMP_FREE_LOCK_TAG() and removed
 * responsibility for cleaning continue lock from
 * snmpHookPrivRelease()
 *
 * Revision 1.4.2.1  2000/10/13 21:33:16  josh
 * AgentX updates
 *
 * Revision 1.4  2000/03/08 20:01:13  josh
 * SNMPv3 support for the vxWorks port of Envoy
 *
 * Revision 1.3  2000/03/07 21:23:37  josh
 * overwriting old envoy.h withnew one corresponding to new demo
 *
 * Revision 7.0  1996/03/18  20:25:29  sar
 * Updated rev to 7.0 and copyright ot 96
 *
 * Revision 6.5  1996/02/27  00:03:03  sar
 * Updated partytime to be usectime
 *
 * Revision 6.4  1995/11/14  22:37:59  sar
 * If proxy is installed release_private is proxy_release_private
 *
 * Revision 6.3  1995/11/10  23:59:08  sar
 * stopped using the pre & post set user exits for the nvview stuff
 *
 * Revision 6.2  1995/11/06  20:05:22  sar
 * Removed validate_community's dependency on install v1
 * removed validate_address & validate_vesion
 *
 * Revision 6.1  1995/10/20  23:15:55  sar
 * remvoed party, context and acl macros
 *
 * Revision 6.0  1995/05/31  22:00:33  sra
 * Release 6.0.
 *
 * Revision 5.6  1995/05/22  19:39:24  sar
 * Modified the lock code to add some new macros.
 *
 * Revision 5.5  1995/05/02  23:39:31  sar
 * Added the envoy timer macros.
 *
 * Revision 5.4  1995/05/01  21:58:15  sar
 * Put the example lock macros in an ifdef block
 *
 * Revision 5.3  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PREAMBLE with #ifdef, not #if.
 *
 * Revision 5.2  1994/11/01  17:59:04  sar
 * *_add_mod routines now take a flag as well as an data block.
 *
 * Revision 5.1  1994/09/29  20:39:39  sar
 * added macros for a simple locking scheme to test the compiliation of
 * our code
 * Modified the macros for use with the non volatile storage stuff to
 * use the new (as of fall 94) scheme.
 *
 * Revision 5.0  1994/05/16  16:47:41  sar 
 * Updated revision to 5.0 and copyright to include 1994 
 * 
 * Revision 4.3  1994/05/12  17:24:05  sar 
 * moved the include for nvutils.h to make it available for everybody 
 * that needs it. 
 * 
 * Revision 4.2  1994/05/10  20:21:45  sar 
 * moved view defines aroudn so they are available in other than v2 settings.
 *
 * Revision 4.1  1994/05/09  21:06:41  sar
 * Added define for SNMP_RESETSYSUPTIME to be nothing.
 *
 * Revision 4.0  1993/06/24  17:44:26  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 1.1  1993/06/09  19:33:45  dab
 * Initial revision
 *
 * 
 */

/* [clearcase]
modification history
-------------------
01a,03jun09,y_t add the trace type(input or output) macro
*/



#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <vxWorks.h>

#ifdef INSTALL_ON_BIG_ENDIAN
#undef INSTALL_ON_BIG_ENDIAN
#endif
    
#ifdef INSTALL_ON_LITTLE_ENDIAN
#undef INSTALL_ON_LITTLE_ENDIAN
#endif
    
#if (_BYTE_ORDER == _BIG_ENDIAN)
#define INSTALL_ON_BIG_ENDIAN 1
#define INSTALL_ON_LITTLE_ENDIAN 0
#elif (_BYTE_ORDER == _LITTLE_ENDIAN)
#define INSTALL_ON_LITTLE_ENDIAN 1
#define INSTALL_ON_BIT_ENDIAN 0
#endif
    
#ifdef INSTALL_ENVOY_SNMP_LOCK
#include <semLib.h>
#endif /* INSTALL_ENVOY_SNMP_LOCK */


#ifdef CONFIG_PREAMBLE

/*****************************************************************
 *
 * System Includes:
 *
 *     The SNMP uses memcpy(), memset(), and strlen().  The appropriate
 * include file should be put here to see that these routines are
 * properly declared.  If no such include file exists on the system
 * then declare memcpy() and memset() here directly.  The SNMP also
 * uses min() and max().  Include the system's definition or use the
 * macros supplied below.
 *****************************************************************/
#include <string.h>



/*****************************************************************
 * Memory allocation routines.  The SNMP needs to allocate memory to
 * decode the packet into and also to store the response data for GET's
 * and GET-NEXT's.
 *****************************************************************/

/*----------------------------------------------------------------
 * Make sure that the routines used by the SNMP_memory_alloc() and
 * SNMP_memory_free() macros are declared.  If system memory routines
 * are used (e.g. malloc() and free()) then include the appropriate
 * system header file here.
 ----------------------------------------------------------------*/
extern  void * snmpdMemoryAlloc (size_t);
extern  void   snmpdMemoryFree (void *);

#define SNMP_memory_alloc(p)     snmpdMemoryAlloc (p)
#define SNMP_memory_free(p)      snmpdMemoryFree (p)




/* Define the maximum packet size this implementation will accept.      */
/* There is no hard upper limit.  SNMP_MAX_PACKET_SIZE should not be    */
/* reduced below the value of SNMP_MIN_PACKET_SIZE.                     */

/* xxxxxxxxxxxxxxxxxxxxxxxxxxx*/

#define SNMP_MAX_PACKET_SIZE            (8*1024)
#define SNMP_MIN_PACKET_SIZE            484 /* shouldn't change */

/* Say what is the maximum number of components of an object identifer  */
/* which we can handle, including those in the instance.                */
#define MAX_OID_COUNT                   32

#if (INSTALL_ENVOY_SNMP_LOCK)
typedef SEM_ID ENVOY_LOCK_T;
#endif 


#define ENVOY_SA_SET_TIMEOUT 100000

#else   /* CONFIG_PREAMBLE */

/*****************************************************************
 *
 * The following macro definitions allow access to the user configurable
 * part of the SNMP code.
 *
 *****************************************************************/

/*****************************************************************
 * Include any declarations needed for the routines specified below
 * by the macros.  Don't put <snmp.h> here because this file is
 * included by that one.
 *****************************************************************/

/*----------------------------------------------------------------
 SNMP_validate_community -- Check an operation against the community name.
         
Parameters:
        SNMP_PKT_T *pkt         The received packet (decoded format)
        SNMPADDR_T *src         Source of the packet
        SNMPADDR_T *dst         Destination of the packet (most likely
                                the address of the machine on which this
                                code is running.)

Returns:
        This procedure should return 0 if happy with the community and
        1 if not.

        This routine should fill in the packet structure with the mib
        view mask associated with the community string.
        This routine may hang additional data onto the "private" field of
        the packet structure.  The user will be given the opportinity to
        release that memory via SNMP_release_private().

 ----------------------------------------------------------------*/

extern int snmpIoCommunityValidate __((SNMP_PKT_T *, SNMPADDR_T *, \
                                                SNMPADDR_T *));
typedef int (*P_SNMPIOCOMMUNITYVALIDATE)(SNMP_PKT_T *, SNMPADDR_T *, \
                                                SNMPADDR_T *);
extern P_SNMPIOCOMMUNITYVALIDATE p_snmpIoCommunityValidate;
#define SNMP_validate_community(pkt, src, dst) \
    (*p_snmpIoCommunityValidate) (pkt, src, dst)
 
#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)

#if 0   /* not supported for vxWorks agent */

#include <envoy/utils/nvutils.h>

#define SNMP_VIEW_CREATE_TEST(PKT, NEW)         SNMP_NV_View_Add_Mod(NEW, 0)
#define SNMP_VIEW_UPDATE_TEST(PKT, OLD, NEW)    SNMP_NV_View_Add_Mod(NEW, 0)
#define SNMP_VIEW_DESTROY_TEST(PKT, OLD)        SNMP_NV_View_Add_Mod(OLD, 1)
#define SNMP_VIEW_BACKOUT_HOOK(NEW)             SNMP_NV_Clean()
#define SNMP_VIEW_CREATE_UNDO(NEW)              (SNMP_NV_Clean(), 0)
#define SNMP_VIEW_UPDATE_UNDO(OLD, NEW)         (SNMP_NV_Clean(), 0)
#define SNMP_VIEW_DESTROY_UNDO(OLD)             (SNMP_NV_Clean(), 0)
#define SNMP_VIEW_DESTROY_BACKOUT(OLD)          SNMP_NV_Clean()
#define SNMP_VIEW_FINISHED                      SNMP_NV_Doit()


#endif  /* 0 (not supported for vxWorks agent) */

#endif /* version2, master agent, v2 views */

#if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
#include <wrn/wm/demo/nvutils.h>

typedef sbits32_t (*P_SNMP_VIEW_2275_TEST)(struct SNMP_PKT_S *, struct VB_S *,
					 struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern P_SNMP_VIEW_2275_TEST p_snmp_view_2275_test;
#define SNMP_VIEW_2275_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			(*p_snmp_view_2275_test)(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			(*p_snmp_view_2275_test)(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			(*p_snmp_view_2275_test)(PKT, VBP, CUR, NEW)


typedef void (*P_SNMP_VIEW_2275_SET)(struct SNMP_PKT_S *, struct VB_S *,
					 struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern P_SNMP_VIEW_2275_SET p_snmp_view_2275_set;
#define SNMP_VIEW_2275_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			(*p_snmp_view_2275_set)(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			(*p_snmp_view_2275_set)(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			(*p_snmp_view_2275_set)(PKT, VBP, CUR, NEW)


typedef void      (*P_SNMP_VIEW_2275_BACKOUT) (struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern P_SNMP_VIEW_2275_BACKOUT p_snmp_view_2275_backout;
#define SNMP_VIEW_2275_DESTROY_BACKOUT(CUR, NEW)	 \
			(*p_snmp_view_2275_backout)(CUR, NEW)
#define SNMP_VIEW_2275_CREATE_BACKOUT(CUR, NEW)		 \
			(*p_snmp_view_2275_backout)(CUR, NEW)
#define SNMP_VIEW_2275_UPDATE_BACKOUT(CUR, NEW)		 \
			(*p_snmp_view_2275_backout)(CUR, NEW)


typedef int (*P_SNMP_VIEW_2275_UNDO)(struct SNMP_PKT_S *, struct VB_S *,
					 struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern P_SNMP_VIEW_2275_UNDO p_snmp_view_2275_undo;
#define SNMP_VIEW_2275_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			(*p_snmp_view_2275_undo)(PKT, VBP, CUR, SAV)
#define SNMP_VIEW_2275_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			(*p_snmp_view_2275_undo)(PKT, VBP, CUR, SAV)
#define SNMP_VIEW_2275_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			(*p_snmp_view_2275_undo)(PKT, VBP, CUR, SAV)


typedef void      (*P_SNMP_VIEW_2275_FINISHED) (void);
extern P_SNMP_VIEW_2275_FINISHED p_snmp_view_2275_finished;
#define SNMP_VIEW_2275_FINISHED() (*p_snmp_view_2275_finished)()

#endif /* #if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS) */


/*****************************************************************
 If version 3 is installed we may need the following macros.

 ENVOY_TIME - returns a clock with a granularity of seconds
  PARAMETERS: none
  returns:    bits32_t

 SNMP_validate_address
  This routine gives the user a chance to examine the address
  information as well as most of the packet itself before a v3
  packet is processed.  The routine must attach the address
  information to the packet.  It may attach a non-default mib
  tree if desired.  Note: in some error cases the packet will
  not be decoded when this routine is called.

  Parameters:
	SNMP_PKT_T *pkt		The received packet (decoded format)
	SNMPADDR_T *src		Source of the packet
	SNMPADDR_T *dst		Destination of the packet (most likely
				the address of the machine on which this
				code is running.)
  returns:
	0 - ok
	1 - failure, drop the packet


 Lastly are are a series of macros that will connect the method
 routines we supplied for the v3 tables to your non-volatile
 storage areas.  You should consult the manuals for more information
 about these macros.
*****************************************************************/
#if INSTALL_ENVOY_SNMP_VERSION_3
#include <wrn/wm/demo/nvutils.h>

extern bits32_t snmpSecsGet(void);
#define ENVOY_TIME() snmpSecsGet()


extern int snmpIoAddressValidate __((SNMP_PKT_T *, SNMPADDR_T *, \
                                                SNMPADDR_T *));
#define SNMP_validate_address(pkt, src, dst) \
    snmpIoAddressValidate (pkt, src, dst)
#if INSTALL_ENVOY_SNMP_V3_NOTIFY

extern int envoy_taddress_to_snmpaddr(SNMPADDR_T *, OBJ_ID_T *, EBUFFER_T *);
extern int envoy_snmpaddr_to_taddress(SNMPADDR_T *, OBJ_ID_T *, EBUFFER_T *);

#define ENVOY_TADDRESS_TO_SNMPADDR(A, D, T) \
        envoy_taddress_to_snmpaddr(A, D, T)


#define ENVOY_SNMPADDR_TO_TADDRESS(A, D, T) \
        envoy_snmpaddr_to_taddress(A, D, T)
#endif

#define SNMP_V3_GROUP_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_group_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_GROUP_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_group_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_GROUP_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_group_backout(CUR, NEW)
#define SNMP_V3_GROUP_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_group_backout(CUR, NEW)
#define SNMP_V3_GROUP_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_group_backout(CUR, NEW)

#define SNMP_V3_GROUP_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_group_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_GROUP_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_group_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_GROUP_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_group_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_GROUP_FINISHED() snmp_v3_group_finished()


#define SNMP_V3_ACCESS_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_access_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_ACCESS_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_access_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_ACCESS_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_access_backout(CUR, NEW)
#define SNMP_V3_ACCESS_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_access_backout(CUR, NEW)
#define SNMP_V3_ACCESS_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_access_backout(CUR, NEW)

#define SNMP_V3_ACCESS_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_access_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_ACCESS_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_access_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_ACCESS_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_access_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_ACCESS_FINISHED() snmp_v3_access_finished()


#define SNMP_V3_USER_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_user_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_USER_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_user_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_USER_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_user_backout(CUR, NEW)
#define SNMP_V3_USER_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_user_backout(CUR, NEW)
#define SNMP_V3_USER_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_user_backout(CUR, NEW)

#define SNMP_V3_USER_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_user_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_USER_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_user_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_USER_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_user_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_USER_FINISHED() snmp_v3_user_finished()

#define SNMP_V3_TADDR_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_taddr_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_TADDR_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_taddr_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_TADDR_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_taddr_backout(CUR, NEW)
#define SNMP_V3_TADDR_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_taddr_backout(CUR, NEW)
#define SNMP_V3_TADDR_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_taddr_backout(CUR, NEW)

#define SNMP_V3_TADDR_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_taddr_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TADDR_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_taddr_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TADDR_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_taddr_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_TADDR_FINISHED() snmp_v3_taddr_finished()

#define SNMP_V3_TPARAM_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_tparam_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_TPARAM_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_tparam_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_TPARAM_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_tparam_backout(CUR, NEW)
#define SNMP_V3_TPARAM_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_tparam_backout(CUR, NEW)
#define SNMP_V3_TPARAM_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_tparam_backout(CUR, NEW)

#define SNMP_V3_TPARAM_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_tparam_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TPARAM_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_tparam_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TPARAM_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_tparam_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_TPARAM_FINISHED() snmp_v3_tparam_finished()

#define SNMP_V3_NOTIFY_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_notify_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_NOTIFY_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_notify_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_NOTIFY_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_notify_backout(CUR, NEW)
#define SNMP_V3_NOTIFY_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_notify_backout(CUR, NEW)
#define SNMP_V3_NOTIFY_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_notify_backout(CUR, NEW)

#define SNMP_V3_NOTIFY_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_notify_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NOTIFY_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_notify_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NOTIFY_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_notify_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_NOTIFY_FINISHED() snmp_v3_notify_finished()

#define SNMP_V3_NPROF_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nprof_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_NPROF_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nprof_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_NPROF_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_nprof_backout(CUR, NEW)
#define SNMP_V3_NPROF_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nprof_backout(CUR, NEW)
#define SNMP_V3_NPROF_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nprof_backout(CUR, NEW)

#define SNMP_V3_NPROF_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nprof_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NPROF_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nprof_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NPROF_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nprof_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_NPROF_FINISHED() snmp_v3_nprof_finished()

#define SNMP_V3_NFILT_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nfilt_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_NFILT_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nfilt_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_NFILT_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_nfilt_backout(CUR, NEW)
#define SNMP_V3_NFILT_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nfilt_backout(CUR, NEW)
#define SNMP_V3_NFILT_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nfilt_backout(CUR, NEW)

#define SNMP_V3_NFILT_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nfilt_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NFILT_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nfilt_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NFILT_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nfilt_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_NFILT_FINISHED() snmp_v3_nfilt_finished()

#define SNMP_V3_PROXY_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_proxy_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_PROXY_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_proxy_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_PROXY_DESTROY_BACKOUT(CUR, NEW)                         \
                        snmp_v3_proxy_backout(CUR, NEW)
#define SNMP_V3_PROXY_CREATE_BACKOUT(CUR, NEW)	                        \
                        snmp_v3_proxy_backout(CUR, NEW)
#define SNMP_V3_PROXY_UPDATE_BACKOUT(CUR, NEW)	                        \
                        snmp_v3_proxy_backout(CUR, NEW)

#define SNMP_V3_PROXY_CREATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_v3_proxy_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_PROXY_UPDATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_v3_proxy_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_PROXY_DESTROY_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_v3_proxy_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_PROXY_FINISHED()      snmp_v3_proxy_finished()

#define SNMP_V3_COMM_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_community_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_community_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_community_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_COMM_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_community_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_community_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_community_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_COMM_DESTROY_BACKOUT(CUR, NEW)                         \
                        snmp_community_backout(CUR, NEW)
#define SNMP_V3_COMM_CREATE_BACKOUT(CUR, NEW)	                        \
                        snmp_community_backout(CUR, NEW)
#define SNMP_V3_COMM_UPDATE_BACKOUT(CUR, NEW)	                        \
                        snmp_community_backout(CUR, NEW)

#define SNMP_V3_COMM_CREATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_community_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_COMM_UPDATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_community_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_COMM_DESTROY_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_community_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_COMM_FINISHED()      snmp_community_finished()

#if INSTALL_ENVOY_SNMP_V3_PROXY
void snmpSendProxyPkt __((SNMPADDR_T *, SNMPADDR_T *,
                          PTR_T, ALENGTH_T));

#define ENVOY_SNMP_SEND_PROXY(FOR, LOC, PKT, SIZ) \
    snmpSendProxyPkt((FOR), (LOC), (PKT), (SIZ))
#endif /* #if INSTALL_ENVOY_SNMP_V3_PROXY */

#if INSTALL_SNMP_V3_DIFFIE_HELLMAN
#define SNMP_DH_RAND() snmpDHRand()
#endif

#endif /* #if INSTALL_ENVOY_SNMP_VERSION_3 */

#if (INSTALL_ENVOY_EPI_MASTER)
#define SNMP_RESETSYSUPTIME
#endif

#if (INSTALL_ENVOY_AGENTX_MASTER) || (INSTALL_ENVOY_ENTITY_MIB)
extern bits32_t envoyGetSysUpTime();

#define ENVOY_GET_SYSUPTIME(CON) envoyGetSysUpTime(CON)
#endif /* #if (INSTALL_ENVOY_AGENTX_MASTER) || (INSTALL_ENVOY_ENTITY_MIB) */

/* Routines required by agentx */
#if INSTALL_ENVOY_AGENTX_MASTER

/* This placeholders eliminate need to include axapi.h and */
/* the attendant duplicate definitions that can occur.     */

struct AX_CONNENT_S;
struct AX_CONN_S;

/* Master agent routines and macros */
extern bits32_t envoyAxConnEntry (bits32_t axConnID, int matchFlag, struct AX_CONNENT_S *pConnEntry);
extern bits32_t envoyAxGetConnID (struct AX_CONN_S *pCE);

#define ENVOY_AX_CONN_ENTRY(INDEX, MATCH_TYPE, P_CONNENT) envoyAxConnEntry(INDEX, MATCH_TYPE, P_CONNENT)
#define ENVOY_AX_GET_CONNID(CONN_ENTRY) envoyAxGetConnID(CONN_ENTRY)

#endif /* #if INSTALL_ENVOY_AGENTX_MASTER */

#if INSTALL_ENVOY_AGENTX_SUB
/* then we do the subagent agentx macros */

extern int envoy_ax_sa_mib_root_node __((PTR_T, SNMP_PKT_T *));

#define ENVOY_AX_SA_MIB_ROOT_NODE(CKE, PKT) envoy_ax_sa_mib_root_node(CKE, PKT)
#define ENVOY_AX_SA_TIMEOUT 1000

#endif /* #if INSTALL_ENVOY_AGENTX_SUB */


/*----------------------------------------------------------------
 SNMP_release_private -- Get rid of a any private data attached to the packet
                    structure.  May be left undefined in which case nothing
                    is done with the private field when the SNMP packet
                    structure is freed.

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/

extern  void snmpHookPrivRelease __((SNMP_PKT_T*));
typedef  void (*P_SNMPHOOKPRIVRELEASE) (SNMP_PKT_T*);
extern P_SNMPHOOKPRIVRELEASE p_snmpHookPrivRelease;
#define SNMP_release_private(pkt)  (*p_snmpHookPrivRelease)(pkt)
 


#if (INSTALL_ENVOY_SNMP_PROXY)
extern void proxy_release_private __((SNMP_PKT_T *));
#define SNMP_release_private(pkt) proxy_release_private(pkt)
#endif /* (INSTALL_ENVOY_SNMP_PROXY) */



/*----------------------------------------------------------------
 SNMP_user_get_encode_buffer -- Allows the user the opportunity to get the
                        buffer that the SNMP packet will be encoded into.
                        If not used then leave undefined.

 Parameters:
        SNMP_PKT_T      *pkt    The packet being encoded.
        unsigned int    need    The amount of storage needed.
        EBUFFER_T       *ebuffp The ebuffer to fill in with the actual
                                buffer.

 Returns:
        0   successful
        1   not successful
 ----------------------------------------------------------------*/
/* #define SNMP_user_get_encode_buffer(pkt, need, ebuffp) 0 */


/*----------------------------------------------------------------
 SNMP_trace_input
 SNMP_trace_output
                -- for implementing packet tracing through the
                   SNMP code.  May not be left undefined but may
                   instead be defined as nothing.

 Parameters:
        SNMP_PKT_T *pkt         The packet being traced

 Returns: Nothing

 ----------------------------------------------------------------*/

extern void snmpTrace(SNMP_PKT_T *,int);

#define SNMP_TRACE_INPUT(pkt)   snmpTrace (pkt,INPUT)
#define SNMP_TRACE_OUTPUT(pkt)  snmpTrace (pkt,OUTPUT)

/*****************************************************************
 * The next series of macros are hooks into the processing of SET PDUs.
 * For most uses you should be able to leave these undefined.
 *****************************************************************/

/*----------------------------------------------------------------
 SNMP_validate_set_pdu -- Perform a global validation of a SET PDU.
         
 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns:
         -1 If the PDU is bad and should be rejected with a GEN_ERR.
          0 If the PDU is good and normal handling should proceed.
         +1 If the PDU is good and this routine has performed all of
            the set operations internally.
 ----------------------------------------------------------------*/
extern int snmpHookSetPduValidate   __((SNMP_PKT_T*));
typedef int (*P_SNMPHOOKSETPDUVALIDATE)(SNMP_PKT_T*);
extern P_SNMPHOOKSETPDUVALIDATE p_snmpHookSetPduValidate;
#define SNMP_validate_set_pdu(pkt) (*p_snmpHookSetPduValidate)(pkt)
  

/*----------------------------------------------------------------
 SNMP_user_pre_set -- Perform a global validation of a SET PDU after all of the
                 test procedures have been called and given the "go ahead".
         
 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns:
         -1 If the PDU is bad and should be rejected with a GEN_ERR.
          0 If the PDU is good and normal handling should proceed.
         +1 If the PDU is good and this routine has performed all of
            the set operations internally.
 ----------------------------------------------------------------*/
extern  int snmpHookPreSet   __((SNMP_PKT_T*));
typedef int (*P_SNMPHOOKPRESET) (SNMP_PKT_T*);
extern P_SNMPHOOKPRESET p_snmpHookPreSet;
#define SNMP_user_pre_set(pkt) (*p_snmpHookPreSet)(pkt)



/*----------------------------------------------------------------
 SNMP_user_post_set -- Perform any final activities after all of the set
                  procedures have been called.

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/


extern  int snmpHookPostSet   __((SNMP_PKT_T*));
typedef  int (*P_SNMPHOOKPOSTSET)   (SNMP_PKT_T*);
extern P_SNMPHOOKPOSTSET p_snmpHookPostSet;
#define SNMP_user_post_set   (*p_snmpHookPostSet)

/*************************************************************************************
* set failed routine
*/

extern int snmpHookSetFailed   __((SNMP_PKT_T*));
typedef int (*P_SNMPHOOKSETFAILED) (SNMP_PKT_T*);
extern P_SNMPHOOKSETFAILED p_snmpHookSetFailed;
#define SNMP_user_set_failed           (*p_snmpHookSetFailed)




#if (INSTALL_ENVOY_SNMP_LOCK)
 
#define ENVOY_SNMP_LOCK(name)                ENVOY_LOCK_T name 
#define ENVOY_SNMP_LOCK_EXTERN(name)         extern  ENVOY_LOCK_T name
 
#define ENVOY_SNMP_INIT_LOCK(name)           (name) = semBCreate (SEM_Q_PRIORITY, SEM_FULL)

#define ENVOY_SNMP_FREE_LOCK_TAG(name, tag)  ((void)semDelete ((name)))

#define ENVOY_SNMP_GET_READ_LOCK(name)       (semTake (name, WAIT_FOREVER))
#define ENVOY_SNMP_GET_WRITE_LOCK(name)      (semTake (name, WAIT_FOREVER))
#define ENVOY_SNMP_RELEASE_READ_LOCK(name)   ((void)semGive (name))
#define ENVOY_SNMP_RELEASE_WRITE_LOCK(name)  ((void)semGive (name))

#endif /* (INSTALL_ENVOY_SNMP_LOCK) */


typedef void (*P_ENVOY_CALL_TIMER)(bits32_t, void (*)(void));
extern P_ENVOY_CALL_TIMER p_envoy_call_timer;
extern bits32_t envoy_now        __((void));

#define ENVOY_CALL_TIMER(T, H)  (*p_envoy_call_timer)(T, H)
#define ENVOY_NOW()             envoy_now()

/* Definitions for use with the Virtual Stack features.
 * SNMP_VIRTUAL_STACK_NUM specifies the number of virtual stacks for whcih we
 * should add community and context information.
 * SNMP_SET_VIRTUAL_STACK specifies the function to call to actually change
 * the virtual stack we wish to use
 * NAMING_SCOPE_PREFIX specifies the string to be prepended to the stack number
 * for use within a community string or context name.
 * ENT_LOGICAL_DESCR specifies the string to be prepended to the description in
 * the entity table
 * ENT_LOGICAL_TADDRESS specifies the address to be used in the entity table
 */

#if INSTALL_SNMP_VXWORKS_VIRTUAL_STACK

#define SNMP_VIRTUAL_STACK_NUM 2
#define SNMP_SET_VIRTUAL_STACK  virtualStackNumTaskIdSet

#define NAMING_SCOPE_PREFIX "VS"

#if INSTALL_SNMP_VXWORKS_VS_ENTITY
#define ENT_LOGICAL_DESCR	"Virtual Stack #"
#define ENT_LOGICAL_TADDRESS	"192.168.0.52"
#endif /* INSTALL_SNMP_VXWORKS_VS_ENTITY */


/* Routines for use with the dynamic naming features.  This feature allows
 * community strings or context names to be dynamically configured instead
 * of being statically built at compile time.  This feature is useful with
 * the virtual stack option to allow new community strings or context names
 * to be added (removed) as a virtual is created (destroyed).
 */

extern void defaultCommunityStringsRegister __((bits32_t));

#define DEFAULT_COMMUNITIES_REGISTER(vsNum) \
    defaultCommunityStringsRegister (vsNum)

extern void vsCommunityStringsRegister __((int));

#define VS_COMMUNITY_REGISTER(vsNum) \
    vsCommunityStringsRegister (vsNum)


#if INSTALL_ENVOY_SNMP_VERSION_3

extern void defaultContextNamesRegister __((bits32_t));

#define DEFAULT_CONTEXTS_REGISTER(vsNum) \
    defaultContextNamesRegister (vsNum)

extern void vsContextNameRegister __((int));

#define VS_CONTEXT_REGISTER(vsNum) \
    vsContextNameRegister (vsNum)

#endif /* INSTALL_ENVOY_SNMP_VERSION_3 */

#endif /* INSTALL_SNMP_VXWORKS_VIRTUAL_STACK */


#define snmpdGroupByGetprocAndInstance group_by_getproc_and_instance  
#define snmpdVbRowLooseExtract         vb_extract_row_loose           
#define snmpdVbRowExtract              vb_extract_row                 
#define snmpdContinue                  SNMP_Continue_function         
#define SNMP_user_set_failed           (*p_snmpHookSetFailed)



#endif /* CONFIG_PREAMBLE */

#ifdef __cplusplus
}
#endif  /* __cplusplus */
