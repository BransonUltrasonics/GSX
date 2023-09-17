/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/nvutils.h,v 1.3 2002/05/23 18:51:11 josh Exp $ */

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
 * $Log: nvutils.h,v $
 * Revision 1.3  2002/05/23 18:51:11  josh
 * code modifications to support the entity MIB
 *
 * Revision 1.2  2001/11/07 14:27:44  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:36  tneale
 * Tornado shuffle
 *
 * Revision 7.16.4.3  2001/09/18 19:07:55  josh
 * base-level support for proxy and coexistence nvram routines
 *
 * Revision 7.16.4.2  2001/07/20 20:36:58  josh
 * nvutils support for community objects
 *
 * Revision 7.16.4.1  2001/06/28 18:24:11  josh
 * Seeding the Cormorant branch with the code from the old
 * cormorant branch
 *
 * Revision 7.16  2001/04/13 19:52:04  josh
 * overwriting the trunk version of this file with the one from
 * the kingfisher branch
 *
 * Revision 7.15  2001/01/19 22:23:35  paul
 * Update copyright.
 *
 * Revision 7.14  2000/06/09 15:00:28  josh
 * updated for new proxy code and new installation options
 *
 * Revision 7.13.4.1  2001/04/13 19:50:33  josh
 * preserving special proxy-related changes to this file
 * on the cormorant branch
 *
 * Revision 7.13.2.3  2001/03/12 22:09:28  tneale
 * Updated copyright
 *
 * Revision 7.13.2.2  2001/01/19 21:38:28  josh
 * a new mechanism for hardcoding traps with arbitrary varbind lists
 * into an agent.  useful for testing purposes
 *
 * Revision 7.13.2.1  2000/09/20 19:05:40  josh
 * bringing branch into line with root tree
 *
 * Revision 7.13  2000/03/17 00:12:12  meister
 * Update copyright message
 *
 * Revision 7.12  1999/10/19 15:16:31  josh
 * adding support for new sections: target_addr, target_params, notify,
 * notify_filter_profile, and notify_filter.
 *
 * Revision 7.11  1998/09/16 19:56:46  sar
 * Added some user exits for the v3 method routines to use in the binary code
 *
 * Revision 7.10  1998/06/28 00:21:58  sar
 * Corrected type signature for nv_view_add_mod
 *
 * Revision 7.9  1998/06/23 20:46:18  sar
 * Update the calling arguments for the config routine
 *
 * Revision 7.8  1998/06/16 05:34:53  sar
 * clean up some type info
 * return the 1445 routines to their previous state
 * return the proper errors for v3 routines
 *
 * Revision 7.7  1998/06/09 22:14:08  sar
 * Added TV3CONTEXT and TV3CNAME
 *
 * Revision 7.6  1998/06/07 02:10:34  sar
 * Move the global configuration arguments here and renamed them
 * (NV_Config_Static et al).
 *
 * Revision 7.5  1998/05/23 19:11:33  sar
 * Modified the view routines to include rfc2275 and added other
 * routines to deal with v3 mibs (access, groups, users, engineids etc).
 *
 * Revision 7.4  1998/02/25 04:57:59  sra
 * Update copyrights.
 *
 * Revision 7.3  1997/03/20 06:53:51  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.1  1997/01/08 01:58:11  sar
 * Removed no_pp stuff and updated copyrights
 *
 * Revision 7.0  1996/03/18  20:19:20  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.2  1996/01/10  16:41:40  sar
 * Usec updates - added a new set of options to the config file
 * for use with the usec agentid, access, context and user info.
 *
 * Revision 6.1  1995/10/20  22:39:59  sar
 * Cut nvutils down to only have the routines for dealing with views.
 *
 * Revision 6.0  1995/05/31  21:50:18  sra
 * Release 6.0.
 *
 * Revision 5.2  1994/11/01  17:54:49  sar
 * Added a flag to the mod list entry to allow us to determine if the
 * entry was being deleted and therefore shouldn't be written out.
 *
 * Revision 5.1  1994/09/23  19:29:50  sar
 * Removed the old non volatile scheme and added a new one that is hopefully
 * easier to use and allows for some new options such as a read only
 * flag to prohibit creating a new config file.  The config stuff will
 * only be included if needed.
 * In snmptalk and snmpint added a next function.
 *
 * Revision 5.0  1994/05/16  15:57:40  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  17:14:56  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 1.4  1993/06/02  23:23:10  dab
 * since this file gets included by other things, it needs to include
 * envoy files with <envoy/h/file.h> not just <file.h>
 *
 * Revision 1.3  1993/05/31  19:07:48  sar
 * changed to use a text rather than binary format for non volatile
 * storage, this allows me to use an editor to change the config files.
 *
 * Revision 1.2  1993/05/13  22:00:33  sar
 * changed stat to statflg to avoid conflicts elsewhere.
 *
 * Revision 1.1  1993/05/13  19:08:06  sar
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


/*****************************************************************************
 *****************************************************************************

 Note well: some of these definitions are used in the pSOS examples as part of
 the api.  Don't make changes without understanding which parts are in use.

 *****************************************************************************
 ****************************************************************************/

#include <wrn/wm/snmp/engine/asn1conf.h>
#include <wrn/wm/snmp/engine/asn1.h>
#include <wrn/wm/snmp/engine/buffer.h>
#include <wrn/wm/snmp/engine/snmpdefs.h>
#include <stdio.h>

#if !defined(nvutils_inc)
#define nvutils_inc

#define NV_VIEW_MAXNAMELEN 32

typedef struct TB_S 
        {
        int   token_tag;        /* Tag for this token */
        char *token_string;     /* String representation for this token */
        } TB_T;

typedef struct MODS_S
        {
        struct MODS_S   *next;  /* The next entry in the linked list */
        PTR_T            data;  /* The structure for the entity */
	PTR_T            newdata; /* new info for the entry */
        INT_32_T         flags; /* Flags to describe what to do with
                                   this entity.  Currently only
                                   destroy is defined */
#if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
	bits8_t		name[NV_VIEW_MAXNAMELEN];  /* the name of the view */
	ALENGTH_T	namelen;	           /* length of the name */
#endif
        } MODS_T;

#define NV_MOD_DESTROY 1

#define TVERSION 1

#define TCOMMENT  1
#define TPREAMBLE 2
#define TVIEW     3
#define TMYAGENT  4
#define TAGENT    5
#define TACCESS   6
#define TCONTEXT  7
#define TUSER     8
#define TV3MYENGINE  9
#define TV3ENGINE   10
#define TV3USER     11
#define TV3GROUP    12
#define TV3ACCESS   13
#define TV3CONTEXT  14
#define TV3TADDR    15
#define TV3TPARM    16
#define TV3NTFY     17
#define TV3NPROF    18
#define TV3NFILT    19
#define TV3TRAP     20
#define TV3PROXY    21
#define TCOMMUNITY  22
#define TENTLOGIC   23

#define TVID             1
#define TVSTATUS         2
#define TVSTORAGE        3
#define TVMASK           4
#define TVTYPE           5

#define TAGID            1
#define TAGBOOTS         2

#define TACID            1
#define TACUNAME         2
#define TACCNAME         3
#define TACQOS           4
#define TACPRIV          5
#define TACRIND          6
#define TACWIND          7

#define TCID             1
#define TCCNAME          2
#define TCFLAGS          3

#define TUID             1
#define TUUNAME          2
#define TUAUTHPROT       3
#define TUAUTHKEY        4

#define TENGID           1
#define TENGBOOTS        2
#define TENGTADDR        3

#define TV3UID		1
#define TV3UUNAME	2
#define TV3UAUTHPROT	3
#define TV3UAUTHKEY	4
#define TV3UPRIVPROT	5
#define TV3UPRIVKEY	6
#define TV3USTATUS	7
#define TV3USTORAGE	8

#define TV3GUSER	1
#define TV3GMODEL	2
#define TV3GGROUP	3
#define TV3GSTATUS	4
#define TV3GSTORAGE	5

#define TV3ACGROUP	 1
#define TV3ACPREFIX	 2
#define TV3ACMATCH	 3
#define TV3ACMODEL	 4
#define TV3ACLEVEL	 5
#define TV3ACREAD	 6
#define TV3ACWRITE	 7
#define TV3ACNOTIFY	 8
#define TV3ACSTATUS	 9
#define TV3ACSTORAGE	10

#define TV3CNAME         1

#define TV3TADDRNAME     1
#define TV3TADDRTDOM     2
#define TV3TADDRTADDR    3
#define TV3TADDRTMASK    4
#define TV3TADDRTMOUT    5
#define TV3TADDRRETRY    6
#define TV3TADDRTLIST    7
#define TV3TADDRTMMS     8
#define TV3TADDRPARMS    9
#define TV3TADDRSTORAGE 10
#define TV3TADDRSTATUS  11

#define TV3TPARMNAME     1
#define TV3TPARMMPMOD    2
#define TV3TPARMSECMOD   3
#define TV3TPARMSECNAM   4
#define TV3TPARMSECLVL   5
#define TV3TPARMSTORAGE  6
#define TV3TPARMSTATUS   7

#define TV3NTFYNAME      1
#define TV3NTFYTAG       2
#define TV3NTFYTYPE      3
#define TV3NTFYSTORAGE   4
#define TV3NTFYSTATUS    5

#define TV3NPROFPARM     1
#define TV3NPROFNAME     2
#define TV3NPROFSTORAGE  3
#define TV3NPROFSTATUS   4

#define TV3NFILTPNAME    1
#define TV3NFILTSTREE    2
#define TV3NFILTMASK     3
#define TV3NFILTTYPE     4
#define TV3NFILTSTORAGE  5
#define TV3NFILTSTATUS   6

#define TV3TRAPNAME      1
#define TV3TRAPOID       2
#define TV3TRAPTYPE      3
#define TV3TRAPVALUE     4
#define TV3TRAPCOUNT     5

#define TV3PROXYNAME     1
#define TV3PROXYTYPE     2
#define TV3PROXYCONID    3
#define TV3PROXYCONNAME  4
#define TV3PROXYPARAMSIN 5
#define TV3PROXYSINGLE   6
#define TV3PROXYMULTIPLE 7
#define TV3PROXYSTORAGE  8
#define TV3PROXYSTATUS   9

#define TCOMMINDEX       1
#define TCOMMNAME        2
#define TCOMMSECNAME     3
#define TCOMMCONID       4
#define TCOMMCONNAME     5
#define TCOMMTAG         6
#define TCOMMSTORAGE     7
#define TCOMMSTATUS      8

#define TENTLOGICINDEX     1
#define TENTLOGICDESCR     2
#define TENTLOGICTYPE      3
#define TENTLOGICCOMMUNITY 4
#define TENTLOGICTADDRESS  5
#define TENTLOGICTDOMAIN   6
#define TENTLOGICCONID     7
#define TENTLOGICCONNAME   8

extern int  SNMP_NV_Config              (char *);
extern void SNMP_NV_Clean               (void);
extern int  SNMP_NV_Write               (int);
extern void SNMP_NV_Doit                (void);

struct SNMP_PKT_S;
struct VB_S;

#if (INSTALL_ENVOY_SNMP_V3_TARGET) && (INSTALL_ENVOY_SNMP_V3_NOTIFY)
typedef struct SNMPV3_TRAP_S
    {
    struct SNMPV3_TRAP_S *next;
    EBUFFER_T             trap_name;
    VBL_T                *vblp;
    } SNMPV3_TRAP_T;

extern SNMPV3_TRAP_T *trap_root;
#endif

#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
struct VIEWLEAF_S;
extern int  SNMP_NV_View_Add_Mod        (struct VIEWLEAF_S *, INT_32_T);
#endif

#if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
struct VIEWLEAF_S;
extern sbits32_t  SNMP_NV_View_2275_Add_Mod   (struct VIEWLEAF_S *,
					       struct VIEWLEAF_S *,
					       sbits32_t);
extern sbits32_t snmp_view_2275_test    (struct SNMP_PKT_S *, struct VB_S *,
					 struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern void      snmp_view_2275_set     (struct SNMP_PKT_S *, struct VB_S *,
					 struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern void      snmp_view_2275_backout (struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern int       snmp_view_2275_undo    (struct SNMP_PKT_S *, struct VB_S *,
					 struct VIEWLEAF_S *,
					 struct VIEWLEAF_S *);
extern void      snmp_view_2275_finished (void);

#endif

#if (INSTALL_ENVOY_SNMP_VERSION_3)
struct SNMP_GROUP_S;
extern sbits32_t  SNMP_NV_V3_Group_Add_Mod    (struct SNMP_GROUP_S *,
					       struct SNMP_GROUP_S *,
					       sbits32_t);
extern sbits32_t  snmp_v3_group_test     (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_GROUP_S *,
					  struct SNMP_GROUP_S *);
extern void       snmp_v3_group_set      (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_GROUP_S *,
					  struct SNMP_GROUP_S *);
extern void       snmp_v3_group_backout  (struct SNMP_GROUP_S *,
					  struct SNMP_GROUP_S *);
extern int        snmp_v3_group_undo     (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_GROUP_S *,
					  struct SNMP_GROUP_S *);
extern void       snmp_v3_group_finished (void);


struct SNMP_ACCESS_S;
extern sbits32_t  SNMP_NV_V3_Access_Add_Mod   (struct SNMP_ACCESS_S *,
					       struct SNMP_ACCESS_S *,
					       sbits32_t);
extern sbits32_t  snmp_v3_access_test     (struct SNMP_PKT_S *, struct VB_S *,
				           struct SNMP_ACCESS_S *,
					   struct SNMP_ACCESS_S *);
extern void       snmp_v3_access_set      (struct SNMP_PKT_S *, struct VB_S *,
					   struct SNMP_ACCESS_S *,
					   struct SNMP_ACCESS_S *);
extern void       snmp_v3_access_backout  (struct SNMP_ACCESS_S *,
					   struct SNMP_ACCESS_S *);
extern int        snmp_v3_access_undo     (struct SNMP_PKT_S *, struct VB_S *,
					   struct SNMP_ACCESS_S *,
					   struct SNMP_ACCESS_S *);
extern void       snmp_v3_access_finished (void);


struct SNMP_USER_S;
extern sbits32_t  SNMP_NV_V3_User_Add_Mod     (struct SNMP_USER_S *,
					       struct SNMP_USER_S *,
					       sbits32_t);
extern sbits32_t  snmp_v3_user_test     (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_USER_S *,
					 struct SNMP_USER_S *);
extern void       snmp_v3_user_set      (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_USER_S *,
					 struct SNMP_USER_S *);
extern void       snmp_v3_user_backout  (struct SNMP_USER_S *,
					 struct SNMP_USER_S *);
extern int        snmp_v3_user_undo     (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_USER_S *,
					 struct SNMP_USER_S *);
extern void       snmp_v3_user_finished (void);

#if (INSTALL_ENVOY_SNMP_V3_TARGET)

struct SNMP_TARGET_ADDR_S;
extern sbits32_t  SNMP_NV_V3_TAddr_Add_Mod     (struct SNMP_TARGET_ADDR_S *,
						struct SNMP_TARGET_ADDR_S *,
						sbits32_t);
extern sbits32_t  snmp_v3_taddr_test     (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_TARGET_ADDR_S *,
					  struct SNMP_TARGET_ADDR_S *);
extern void       snmp_v3_taddr_set      (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_TARGET_ADDR_S *,
					 struct SNMP_TARGET_ADDR_S *);
extern void       snmp_v3_taddr_backout  (struct SNMP_TARGET_ADDR_S *,
					 struct SNMP_TARGET_ADDR_S *);
extern int        snmp_v3_taddr_undo     (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_TARGET_ADDR_S *,
					 struct SNMP_TARGET_ADDR_S *);
extern void       snmp_v3_taddr_finished (void);

struct SNMP_TARGET_PARAMS_S;
extern sbits32_t  SNMP_NV_V3_TParam_Add_Mod     (struct SNMP_TARGET_PARAMS_S *,
						 struct SNMP_TARGET_PARAMS_S *,
						 sbits32_t);
extern sbits32_t  snmp_v3_tparam_test    (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_TARGET_PARAMS_S *,
					  struct SNMP_TARGET_PARAMS_S *);
extern void       snmp_v3_tparam_set    (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_TARGET_PARAMS_S *,
					 struct SNMP_TARGET_PARAMS_S *);
extern void       snmp_v3_tparam_backout  (struct SNMP_TARGET_PARAMS_S *,
					   struct SNMP_TARGET_PARAMS_S *);
extern int        snmp_v3_tparam_undo     (struct SNMP_PKT_S *, struct VB_S *,
					   struct SNMP_TARGET_PARAMS_S *,
					   struct SNMP_TARGET_PARAMS_S *);
extern void       snmp_v3_tparam_finished (void);

#if (INSTALL_ENVOY_SNMP_V3_NOTIFY)

struct SNMP_NOTIFY_S;
extern sbits32_t  SNMP_NV_V3_Notify_Add_Mod     (struct SNMP_NOTIFY_S *,
						 struct SNMP_NOTIFY_S *,
						 sbits32_t);
extern sbits32_t  snmp_v3_notify_test    (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_NOTIFY_S *,
					  struct SNMP_NOTIFY_S *);
extern void       snmp_v3_notify_set    (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_NOTIFY_S *,
					 struct SNMP_NOTIFY_S *);
extern void       snmp_v3_notify_backout  (struct SNMP_NOTIFY_S *,
					   struct SNMP_NOTIFY_S *);
extern int        snmp_v3_notify_undo     (struct SNMP_PKT_S *, struct VB_S *,
					   struct SNMP_NOTIFY_S *,
					   struct SNMP_NOTIFY_S *);
extern void       snmp_v3_notify_finished (void);

struct SNMP_NOTIFY_FILTER_PROFILE_S;
extern sbits32_t  SNMP_NV_V3_NProf_Add_Mod     (struct SNMP_NOTIFY_FILTER_PROFILE_S *,
						struct SNMP_NOTIFY_FILTER_PROFILE_S *,
						sbits32_t);
extern sbits32_t  snmp_v3_nprof_test    (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_NOTIFY_FILTER_PROFILE_S *,
					 struct SNMP_NOTIFY_FILTER_PROFILE_S *);
extern void       snmp_v3_nprof_set    (struct SNMP_PKT_S *, struct VB_S *,
					struct SNMP_NOTIFY_FILTER_PROFILE_S *,
					struct SNMP_NOTIFY_FILTER_PROFILE_S *);
extern void       snmp_v3_nprof_backout  (struct SNMP_NOTIFY_FILTER_PROFILE_S *,
					  struct SNMP_NOTIFY_FILTER_PROFILE_S *);
extern int        snmp_v3_nprof_undo     (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_NOTIFY_FILTER_PROFILE_S *,
					  struct SNMP_NOTIFY_FILTER_PROFILE_S *);
extern void       snmp_v3_nprof_finished (void);

struct SNMP_NOTIFY_FILTER_S;
extern sbits32_t  SNMP_NV_V3_NFilt_Add_Mod     (struct SNMP_NOTIFY_FILTER_S *,
						struct SNMP_NOTIFY_FILTER_S *,
						sbits32_t);
extern sbits32_t  snmp_v3_nfilt_test    (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_NOTIFY_FILTER_S *,
					 struct SNMP_NOTIFY_FILTER_S *);
extern void       snmp_v3_nfilt_set    (struct SNMP_PKT_S *, struct VB_S *,
					struct SNMP_NOTIFY_FILTER_S *,
					struct SNMP_NOTIFY_FILTER_S *);
extern void       snmp_v3_nfilt_backout  (struct SNMP_NOTIFY_FILTER_S *,
					  struct SNMP_NOTIFY_FILTER_S *);
extern int        snmp_v3_nfilt_undo     (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_NOTIFY_FILTER_S *,
					  struct SNMP_NOTIFY_FILTER_S *);
extern void       snmp_v3_nfilt_finished (void);

#endif /* #if (INSTALL_ENVOY_SNMP_V3_NOTIFY) */

#if (INSTALL_ENVOY_SNMP_V3_PROXY)

struct SNMP_PROXY_S;
extern sbits32_t  SNMP_NV_V3_Proxy_Add_Mod     (struct SNMP_PROXY_S *,
						struct SNMP_PROXY_S *,
						sbits32_t);
extern sbits32_t  snmp_v3_proxy_test    (struct SNMP_PKT_S *, struct VB_S *,
					 struct SNMP_PROXY_S *,
					 struct SNMP_PROXY_S *);
extern void       snmp_v3_proxy_set    (struct SNMP_PKT_S *, struct VB_S *,
					struct SNMP_PROXY_S *,
					struct SNMP_PROXY_S *);
extern void       snmp_v3_proxy_backout  (struct SNMP_PROXY_S *,
					  struct SNMP_PROXY_S *);
extern int        snmp_v3_proxy_undo     (struct SNMP_PKT_S *, struct VB_S *,
					  struct SNMP_PROXY_S *,
					  struct SNMP_PROXY_S *);
extern void       snmp_v3_proxy_finished (void);

#endif /* #if (INSTALL_ENVOY_SNMP_V3_PROXY)  */
#if (INSTALL_ENVOY_SNMP_COEXISTENCE)

struct SNMP_COMMUNITY_S;
extern sbits32_t  SNMP_NV_Community_Add_Mod (struct SNMP_COMMUNITY_S *,
                                             struct SNMP_COMMUNITY_S *,
                                             sbits32_t);
extern sbits32_t  snmp_community_test (struct SNMP_PKT_S *, struct VB_S *,
                                       struct SNMP_COMMUNITY_S *,
                                       struct SNMP_COMMUNITY_S *);
extern void       snmp_community_set    (struct SNMP_PKT_S *, struct VB_S *,
                                         struct SNMP_COMMUNITY_S *,
                                         struct SNMP_COMMUNITY_S *);
extern void       snmp_community_backout  (struct SNMP_COMMUNITY_S *,
                                           struct SNMP_COMMUNITY_S *);
extern int        snmp_community_undo (struct SNMP_PKT_S *, struct VB_S *,
                                       struct SNMP_COMMUNITY_S *,
                                       struct SNMP_COMMUNITY_S *);
extern void       snmp_community_finished (void);


#endif /* #if (INSTALL_ENVOY_SNMP_COEXISTENCE */
#endif /* #if (INSTALL_ENVOY_SNMP_V3_TARGET) */
#endif /* #if (INSTALL_ENVOY_SNMP_VERSION_3) */

#define MAX_NVF 128
extern char NV_Config_Static[];
extern char NV_Old_Config_Static[];
extern char NV_Temp_Config_Static[];

extern char *NV_Config_File;
extern char *NV_Old_Config_File;
extern char *NV_Temp_Config_File;

extern int NV_Read_Only;
extern int NV_Views_From_Config;

#endif
