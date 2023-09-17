/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/agentx.h,v 1.9 2002/05/30 17:24:54 andre Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * $Log: agentx.h,v $
 * Revision 1.9  2002/05/30 17:24:54  andre
 * Add placeholder definition for AX_RESPONSE_PREPROCESS
 *
 * Revision 1.8  2002/04/09 14:18:35  tneale
 * REGLIST changes to handle list-based registration
 *
 * Revision 1.7  2002/03/28 14:37:07  tneale
 * Removed externs of old mib variables
 *
 * Revision 1.6  2002/03/20 20:20:41  tneale
 * Fixed multiple definition of ENVOY_AX_PARSE_ERROR
 *
 * Revision 1.5  2002/03/19 21:47:01  tneale
 * Changed Agentx protocol error codes per RFC2741
 *
 * Revision 1.4  2002/03/18 16:05:43  tneale
 * Added cmonnect_id member to Session structure
 *
 * Revision 1.3  2002/03/12 15:54:39  tneale
 * Added structure definition for the registration list
 *
 * Revision 1.2  2001/11/06 21:35:00  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.6.4.1  2001/08/17 20:38:43  meister
 * Added prototypes for some agentx helper functions which were previously
 * declared statically. Added prototypes for agentx and v3 dynamic config init
 * functions.
 *
 * Revision 9.6  2001/01/19 22:22:02  paul
 * Update copyright.
 *
 * Revision 9.5  2000/03/17 19:48:27  meister
 * Update copyright notice
 *
 * Revision 9.4  2000/03/09 20:58:32  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.3  2000/03/09 16:53:16  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.2  1999/12/23 14:47:26  josh
 * cleaning up code per Shawn's comments -- removing extraneous
 * comments and doing proxy check before notify check
 *
 * Revision 9.1  1999/03/30 15:20:13  josh
 * feature to allow customer-driven context conversion into AgentX
 *
 * Revision 9.0  1998/10/16 22:10:03  sar
 * Update version stamp to match release
 *
 * Revision 8.2  1998/08/12 04:52:15  sar
 * revamp the initialization routines to limit the amount of
 * code that might get pulled in.
 *
 * Revision 8.1  1998/02/25 04:50:19  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:19  sar
 * Updated revision to 8.0
 *
 * Revision 7.4  1997/10/30 04:23:16  sar
 * Update type information mostly convert bits16_ts to ALENGTH_Ts
 * also add MAX_ALENGTH
 *
 * Revision 7.3  1997/10/29 04:11:40  sar
 * Cleaned up some type mismatches to keep compilers happy
 *
 * Revision 7.2  1997/10/21 02:53:37  sar
 * Added defines for transport types and index allocate and deallocate macros
 * and removed leaf decrement function
 *
 * Revision 7.1  1997/10/16 00:25:16  sar
 * Primarily this set of modifications is for the agentx protocol.
 * Most of the work was in the agentx.h and ax_mth.h files.  Most
 * other files (snmpdefs.h, vbdefs.h and mib.h) received updates
 * to their structures and snmp got some new lock macros.
 *
 * One other change of note was the modification of continue
 * processing.  Previously we called a specific continue routine
 * directly, now we store a pointer to the corret continue
 * function in the snmp packet structure and call that.  We include
 * macros for backwards compatibility.
 *
 */

/* [clearcase]
modification history
-------------------
01b,19apr05,job  update copyright notices
01a,04aug04,job  dropped connection handling
*/


#if (!defined(envoy_agentx_inc))
#define envoy_agentx_inc

#ifdef __cplusplus
extern"C" {
#endif

#include <wrn/wm/snmp/engine/asn1conf.h>
#include <wrn/wm/snmp/engine/asn1.h>
#include <wrn/wm/snmp/engine/buffer.h>
#include <wrn/wm/snmp/engine/vbdefs.h>
#include <wrn/wm/snmp/engine/etimer.h>
#include <wrn/wm/snmp/engine/tree.h>

/*extern struct ENVOY_AX_PKT_S;
extern struct SNMP_PKT_S;*/

typedef void ENVOY_AX_ADMIN_T (ptr_t, ptr_t);
typedef int  ENVOY_AX_SEND_T  (ptr_t, ptr_t, ptr_t, ALENGTH_T);
typedef void ENVOY_AX_ERROR_T (ptr_t, int);
typedef void ENVOY_AX_ADD_T   (ptr_t);

#define ENVOY_AX_MASTER_PORT 705

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

/* Define a chunk block for agentx, this is used by the byte_handler
   code to save and build a packet when it arrives in chunks rather
   than all at once */
typedef struct ENVOY_AX_CHUNK_S
	{
	EBUFFER_T chunk;
        } ENVOY_AX_CHUNK_T;

/* Define a state block for the sub agent, primarily this will
   be useful if you want to have multiple sub agents in the same
   process space */
typedef struct ENVOY_AX_SA_SB_S
	{
	struct ENVOY_AX_PKT_S *set_list; /* list of outstanding set requests */
	struct ENVOY_AX_PKT_S *gnb_list; /* list of deferred gnb requests */
	bits32_t               readers;	 /* number of outstanding gnb reqs */

	ENVOY_TIMER_T          ager;     /* timer block for aging pkts */
	ENVOY_TIMER_T          defer;    /* timer block for deferring pkts */
	bits32_t               flags;    /* currently only if the timer has
					    been set */
#if INSTALL_ENVOY_SNMP_LOCK
	ENVOY_LOCK_T    list_lock;	/* lock for manipulating this struct
					   and the structs on the two lists */
	ENVOY_LOCK_T    coarse_lock;	/* lock for manipulating the mib db */
#endif
	} ENVOY_AX_SA_SB_T;

/* Define the agentx structure */
typedef struct ENVOY_AX_PKT_S
        {
        bits8_t   version;		/* The agentx protocol version */
        bits8_t   type;			/* The agentx pdu type, see below */
        bits8_t   flags;		/* Agentx flags, also see below */
        bits32_t  session_id;		/* session id between mast & sub */
	bits32_t  transaction_id;	/* groups agentx pkts to snmp pkts */
	bits32_t  packet_id;		/* associates response pkts to reqs */

	EBUFFER_T context;		/* context, optional, see flags */

	ENVOY_AX_SEND_T       *send;    /* io completion routine */
	ENVOY_AX_ERROR_T      *error;   /* error completion routine */
	ptr_t                  cookie;  /* cookie for comp routines */

	struct SNMP_PKT_S     *pktp;	/* snmp packet we've turned this agentx
					   request into for processing */
	ENVOY_AX_SA_SB_T      *sasb;	/* pointer to the state block for
					   this sub agent */
	struct ENVOY_AX_PKT_S *next;	/* next pointer for lists */

	bits32_t               ts;      /* timestamp for aging pkts */

	bits32_t  sysuptime;		/* these 3 are only used in */
	bits16_t  error_stat;		/* responses */
	bits16_t  error_index;

	union {				/* pdu data, one of the following */

	    bits8_t reason;		/* enum for reason for closing */

	    struct {			/* used for opens, add caps and */
	        bits8_t   timeout;	/* remove caps */
		bits8_t   prefix;
	        OBJ_ID_T  sub_id;
		EBUFFER_T descr;
	        } open_data;		

	    struct {			/* used for (un)registers */
	        bits8_t   timeout;
		bits8_t   priority;
		bits8_t   range;
		OBJ_ID_T  region;
		OIDC_T    bound;
	        } reg_data;

	    struct {			/* gets, nexts, bulks, test sets, */
	        bits16_t   non_reps;	/* responses, notifies and index */
		bits16_t   max_reps;	/* (de)allocates */
	        VBL_T      vbl_str;
		bits8_t    cur_phase;	/* current phase of set processing */
		                        /* same as vb flags */

		bits32_t            next_pid;    /* info for the next phase */
		bits8_t             next_phase;  /* of set processing, what */
		ENVOY_AX_SEND_T    *next_send;   /* to do, and any comp info */
		ENVOY_AX_ERROR_T   *next_error;  
		ptr_t              *next_cookie; 
	        } proc_data;
	    }     data;
        } ENVOY_AX_PKT_T;

/* AgentX pdu types */
#define ENVOY_AX_OPEN              1
#define ENVOY_AX_CLOSE             2
#define ENVOY_AX_REGISTER          3
#define ENVOY_AX_UNREGISTER        4
#define ENVOY_AX_GET               5
#define ENVOY_AX_NEXT              6
#define ENVOY_AX_BULK              7
#define ENVOY_AX_TEST              8
#define ENVOY_AX_COMMIT            9
#define ENVOY_AX_UNDO             10
#define ENVOY_AX_CLEANUP          11
#define ENVOY_AX_NOTIFY           12
#define ENVOY_AX_PING             13
#define ENVOY_AX_INDEX_ALLOCATE   14
#define ENVOY_AX_INDEX_DEALLOCATE 15
#define ENVOY_AX_ADD_AC           16
#define ENVOY_AX_REMOVE_AC        17
#define ENVOY_AX_RESPONSE         18

/* AgentX flag bits */
#define ENVOY_AX_BIT_INSTANCE      1
#define ENVOY_AX_BIT_NEW_INDEX     2
#define ENVOY_AX_BIT_ANY_INDEX     4
#define ENVOY_AX_BIT_NDC           8	/* non-default context bit, when set
					   optional context field is used */
#define ENVOY_AX_BIT_BYTE_ORDER   16	/* when set, network byte order used */

/* reasons for closing a connection */
#define ENVOY_AX_CLOSE_OTHER    1
#define ENVOY_AX_CLOSE_PARSE    2
#define ENVOY_AX_CLOSE_PROTO    3
#define ENVOY_AX_CLOSE_TIMEOUTS 4
#define ENVOY_AX_CLOSE_SHUTDOWN 5
#define ENVOY_AX_CLOSE_MANAGER  6

/* AgentX codes for the next phase of sets */
#define ENVOY_AX_NEXT_CLEANUP_START 1
#define ENVOY_AX_NEXT_UNDO_START    2

/* AgentX Protocol error codes */
#define ENVOY_AX_OPEN_FAILED             256
#define ENVOY_AX_NOT_OPEN                257
#define ENVOY_AX_INDEX_WRONG_TYPE        258
#define ENVOY_AX_INDEX_ALREADY_ALLOCATED 259
#define ENVOY_AX_INDEX_NONE_AVAILABLE    260
#define ENVOY_AX_INDEX_NOT_ALLOCATED     261
#define ENVOY_AX_UNSUPPORTED_CONTEXT     262
#define ENVOY_AX_DUPLICATE_REGISTRATION  263
#define ENVOY_AX_UNKNOWN_REGISTRATION    264
#define ENVOY_AX_UNKNOWN_AGENT_CAPS      265
#define ENVOY_AX_PROTOCOL_PARSE_ERROR    266
#define ENVOY_AX_REQUEST_DENIED          267
#define ENVOY_AX_PROCESSING_ERROR        268

#define ENVOY_AX_OTHER			 0xffff

/* AgentX error codes */
#define ENVOY_AX_ALLOCATION_FAILURE 1
#define ENVOY_AX_BAD_VALUE          2
#define ENVOY_AX_TOO_SMALL	    3
#define ENVOY_AX_BAD_SIZE           4
#define ENVOY_AX_WRONG_VERSION      5
#define ENVOY_AX_PARSE_ERROR        6
#define ENVOY_AX_BAD_OPERATION      7
#define ENVOY_AX_SESS_LOSS          8
#define ENVOY_AX_TOO_BIG            9

/* AgentX packet access macros */
#define ax_pkt_get_context(PKTP) (&(PKTP)->context)

/* AgentX version stamps */
#define ENVOY_AX_VERSION_1 1
#define ENVOY_AX_VERSION_MAXIMUM 1

/* AgentX session admin status codes */
#define ENVOY_AX_SESSION_UP   1
#define ENVOY_AX_SESSION_DOWN 2

/* globals variables for subagents */
#ifndef ENVOY_AX_SA_TIMEOUT
#define ENVOY_AX_SA_TIMEOUT 1000
#endif
extern bits32_t envoy_ax_sa_timeout;

extern void envoy_ax_sa_init   (void);


extern void            envoy_ax_pkt_init     (ENVOY_AX_PKT_T *);
extern ENVOY_AX_PKT_T *envoy_ax_pkt_allocate (void);
extern ALENGTH_T       envoy_ax_pkt_size     (ENVOY_AX_PKT_T *, VB_T *);
extern void            envoy_ax_pkt_clean    (ENVOY_AX_PKT_T *);
extern void            envoy_ax_pkt_free     (ENVOY_AX_PKT_T *);

extern int	       envoy_ax_pkt_encode  (ENVOY_AX_PKT_T *, VB_T *,
					     EBUFFER_T *, ALENGTH_T);
extern ENVOY_AX_PKT_T *envoy_ax_pkt_decode  (bits8_t *, ALENGTH_T, int *);



extern ENVOY_AX_PKT_T *envoy_ax_pkt_create_all(int *,    bits8_t,    bits8_t,
					       bits8_t,  bits32_t,   bits32_t,
					       bits8_t,  bits32_t,   bits8_t *,
					       int,      OBJ_ID_T *, bits8_t,
					       bits8_t,  bits8_t,    OIDC_T,
					       bits32_t, bits8_t *,  int,
					       bits8_t);

extern int envoy_ax_pkt_create_open  (EBUFFER_T *, bits8_t,    bits8_t,
				      bits32_t,    bits8_t,    bits32_t,
				      bits8_t *,   OBJ_ID_T *);

extern int envoy_ax_pkt_create_close (EBUFFER_T *, bits8_t,  bits8_t,
				      bits32_t,    bits32_t, bits8_t);

extern int envoy_ax_pkt_create_registrations(EBUFFER_T *, bits8_t,  bits8_t,
					     bits8_t,     bits32_t, bits32_t,
					     bits8_t,     bits8_t,  bits8_t,
					     OBJ_ID_T *,  OIDC_T,   bits32_t,
					     bits8_t *);

extern int envoy_ax_pkt_create_ping  (EBUFFER_T *, bits8_t,  bits8_t,
				      bits32_t,    bits32_t, bits32_t,
				      bits8_t *);

extern int envoy_ax_pkt_create_agent_caps(EBUFFER_T *, bits8_t,    bits8_t,
					  bits8_t,     bits32_t,   bits32_t,
					  bits32_t,    bits8_t *,  bits32_t,
					  bits8_t *,   OBJ_ID_T *);

extern ENVOY_AX_PKT_T *envoy_ax_pkt_create_variables(int *,    bits8_t,
						     bits8_t,  bits8_t,
						     bits32_t, bits32_t,
						     bits32_t, bits8_t *,
						     int,      int);

extern int envoy_ax_bind_integer   (ENVOY_AX_PKT_T *, int, int, OIDC_T *,
				    sbits32_t);
extern int envoy_ax_bind_ip_address(ENVOY_AX_PKT_T *, int, int, OIDC_T *,
				    bits8_t *);
extern int envoy_ax_bind_null      (ENVOY_AX_PKT_T *, int, int, OIDC_T *);
extern int envoy_ax_bind_object_id (ENVOY_AX_PKT_T *, int, int, OIDC_T *,
				    int, OIDC_T *);
extern int envoy_ax_bind_string    (ENVOY_AX_PKT_T *, int, int, OIDC_T *,
				    bits8_t, int, bits8_t *, int);
extern int envoy_ax_bind_uint_64   (ENVOY_AX_PKT_T *, int, int, OIDC_T *,
				    bits8_t, bits32_t, bits32_t);
extern int envoy_ax_bind_uint      (ENVOY_AX_PKT_T *, int, int, OIDC_T *,
				    bits8_t, bits32_t);

extern void envoy_ax_sa_handler(bits8_t *, ALENGTH_T, ENVOY_AX_SA_SB_T *,
				ENVOY_AX_ADMIN_T *, ENVOY_AX_SEND_T *,
				ENVOY_AX_ERROR_T *, ptr_t);

extern int _snmp_find_next_helper_ax(MIBNODE_T *, int, 
                                     OIDC_T*, OBJ_ID_T *, int, VB_T *, 
                                     SNMP_PKT_T *, int);

extern int _snmp_find_next_bound(MIBNODE_T *, VB_T *);

#if INSTALL_COMMON_DYNAMIC_COMP_CONFIG
extern void ax_dyncfg_init(void);
#endif

extern ENVOY_AX_SA_SB_T *envoy_ax_sa_state_create(void);
extern void              envoy_ax_sa_state_init  (ENVOY_AX_SA_SB_T *);
extern int               envoy_ax_sa_session_loss(ENVOY_AX_SA_SB_T *, ptr_t,
						  int, bits32_t);

typedef struct ENVOY_AX_SESSION_S
	{
	struct ENVOY_AX_SESSION_S *next;/* next session in the list */
	bits32_t	session_id;	/* id for this session */
	bits32_t	connection_id;  /* id for this connection */
	bits8_t		version;  	/* protocol version for this session */
	bits8_t         timeout;        /* default timeout period */
	bits8_t         flags;		/* flags, currently byte order */

	ENVOY_AX_SEND_T  *send;         /* routine to send to sub agent */
	ENVOY_AX_ERROR_T *error;        /* routine to free the cookie */
	ptr_t             cookie;       /* cookie for use with send rtn */

	OBJ_ID_T	sub_id;         /* tag for sub agent */
	EBUFFER_T       descr;          /* string for sub agent */
	bits32_t        timestamp;      /* when this session started */
        bits32_t        timeout_count;  /* number of consecutive timeouts */
        } ENVOY_AX_SESSION_T;

typedef struct ENVOY_AX_MA_REG_S
	{
	bits32_t	reg_index;	/* index for use in agentx mib */
	bits32_t	context_id;     /* index of context for this object */
	bits8_t         priority;       /* priority for this object */
	MIBLEAF_T      *leaf;           /* leaf for use in the mib tree
					   also contains session, priority
					   and timeout information */
	struct ENVOY_AX_MA_REG_S *next;
	struct ENVOY_AX_MA_REG_S *mib_next;
	} ENVOY_AX_MA_REG_T;

typedef struct ENVOY_AX_MA_REGLIST_S
	{
	struct ENVOY_AX_MA_REGLIST_S *next; /* next registration in the list */
	bits32_t	reg_index;	   /* index for use in agentx mib */
	bits32_t	session_id;	   /* id for this session */
	bits32_t	connection_id;     /* id for this connection */
	bits8_t		range_id;	   /* subid which has range, if any */
	OIDC_T 		bound;		   /* upper bound of range, if any */
	bits8_t		flags;	   	   /* non-zero if this is an instance */
	MIBLEAF_T      *mibleaf;           /* MIB leaf that has more info on this reg. */
	struct ENVOY_AX_MA_REGLIST_S *match; /* For list of registrations w/same OID */
	bits32_t        context_id;        /* id for this context */ 
	bits8_t         priority;          /* priority of this registration */
	OBJ_ID_T        start_oid;         /* registered subtree, or start if range */
	} ENVOY_AX_MA_REGLIST_T;

typedef struct ENVOY_AX_CONTEXT_S
	{
	bits32_t	id;	        /* this context's index in the list */
	struct ENVOY_AX_CONTEXT_S *next; /* ptr to next element in list */
	EBUFFER_T       context;	/* string for the context */
	MIBNODE_T       *mibroot;	/* mib root node the user gave us */
	TREENODE_T      *indexroot;     /* root for possible indexes */
        } ENVOY_AX_CONTEXT_T;

/* control structure for outstanding agentx requests */
typedef struct AX_CB_S
        {
        struct AX_CB_S *next;
	bits32_t        reqid;
        SNMP_PKT_T     *pktp;
        VB_T           *vbp;
	int             count;
        } AX_CB_T;

/* global variables used by an agentx master to keep state */
#ifndef ENVOY_AX_DEFAULT_TIMEOUT
#define ENVOY_AX_DEFAULT_TIMEOUT 1
#endif

#ifndef SNMP_AX_TIMEOUT_COUNT
#define SNMP_AX_TIMEOUT_COUNT 3
#endif
    
/* General sub-agent response packet pre-processing. */
#ifndef AX_RESPONSE_PREPROCESS
#define AX_RESPONSE_PREPROCESS(PKT)
#endif

extern bits32_t agentxDefaultTimeout;

extern AX_CB_T    *first_axcb;
extern bits32_t    ax_reqid;

extern bits32_t    ax_registration_id;
extern TREENODE_T *registration_root;

extern bits32_t    agentxRegisterDuplicate;

extern bits32_t            envoy_ax_session_id;
extern ENVOY_AX_SESSION_T *envoy_ax_session_list;
extern ENVOY_AX_MA_REGLIST_T *envoy_ax_registration_list;

extern bits32_t            context_list_id;
extern ENVOY_AX_CONTEXT_T *context_list;

extern sbits32_t   agentxMasterAgentXVer;

extern void envoy_ax_ma_init   (void);

extern void envoy_ax_ma_handler(bits8_t *, ALENGTH_T, ENVOY_AX_SA_SB_T *,
				ENVOY_AX_ADMIN_T *, ENVOY_AX_SEND_T *,
				ENVOY_AX_ERROR_T *, ptr_t);

extern int  envoy_ax_ma_cleanup_sessions(ptr_t, bits8_t);

typedef void ENVOY_AX_HANDLER_T (bits8_t *, ALENGTH_T, ENVOY_AX_SA_SB_T *,
				 ENVOY_AX_ADMIN_T *, ENVOY_AX_SEND_T *,
				 ENVOY_AX_ERROR_T *, ptr_t);

extern void envoy_ax_chunk_init   (ENVOY_AX_CHUNK_T *);
extern int  envoy_ax_chunk_clean  (ENVOY_AX_CHUNK_T *);
extern int  envoy_ax_chunk_handler(bits8_t *, ALENGTH_T, ENVOY_AX_HANDLER_T *,
				   ENVOY_AX_CHUNK_T *, ENVOY_AX_SA_SB_T *,
				   ENVOY_AX_ADMIN_T *, ENVOY_AX_SEND_T *,
				   ENVOY_AX_ERROR_T *, ENVOY_AX_ADD_T *,
				   ptr_t);

#if (!defined(ENVOY_AX_COOKIE_CMP))
#define ENVOY_AX_COOKIE_CMP(HAND, COOKIE) (HAND == COOKIE)
#endif

typedef struct ENVOY_AX_INDEX_STR_S
	{
	ALENGTH_T  length;
	bits8_t   *buffer;
        } ENVOY_AX_INDEX_STR_T;

typedef struct ENVOY_AX_INDEX_DATA_S
	{
	struct ENVOY_AX_INDEX_DATA_S *next;
	bits32_t                      session_id; /* 0 for no session */
	union {
		OBJ_ID_T              objid;
		bits32_t              bits32;
		ENVOY_AX_INDEX_STR_T  string;
		bits8_t               ipaddr[4];
	      } data;
	} ENVOY_AX_INDEX_DATA_T;

typedef struct ENVOY_AX_INDEX_HDR_S
	{
	bits8_t                type;
	ENVOY_AX_INDEX_DATA_T *data;
	} ENVOY_AX_INDEX_HDR_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

extern ENVOY_AX_SESSION_T *ax_session_find_index(bits32_t, int *);
extern void                ax_session_release   (ENVOY_AX_SESSION_T *);
extern ENVOY_AX_SESSION_T *ax_session_next      (bits32_t, int *);
extern int                 envoy_ax_ma_cleanup_session_mth(bits32_t, bits8_t);

extern ENVOY_AX_CONTEXT_T *ax_context_index(bits32_t);
extern ENVOY_AX_CONTEXT_T *ax_context_next (ENVOY_AX_CONTEXT_T *);

extern ENVOY_AX_MA_REG_T  *find_reg_leaf(OBJ_ID_T *, bits8_t, bits32_t);

extern sbits32_t Find_Leaf_From_Root(MIBNODE_T *, OBJ_ID_T *, MIBLEAF_T **);
extern int  AX_Add_Leaf_From_Root(MIBNODE_T *, OBJ_ID_T *, MIBLEAF_T *,
				  MIBLEAF_T **);
extern MIBLEAF_T * AX_Remove_Leaf_From_Root(MIBNODE_T *, OBJ_ID_T *);

#define ENVOY_AX_TRANSPORT_TYPE_UNIX  1
#define ENVOY_AX_TRANSPORT_TYPE_TCP   2
#define ENVOY_AX_TRANSPORT_TYPE_UDP   3
#define ENVOY_AX_TRANSPORT_TYPE_MEM   4
#define ENVOY_AX_TRANSPORT_TYPE_OTHER 5

extern void  ax_ma_index_allocate  (ENVOY_AX_PKT_T *, ENVOY_AX_PKT_T *,
				    ENVOY_AX_CONTEXT_T *);
extern void  ax_ma_index_deallocate(ENVOY_AX_PKT_T *, ENVOY_AX_PKT_T *,
				    ENVOY_AX_CONTEXT_T *);
extern void  ax_ma_cleanup_indexes (bits32_t);

#if INSTALL_ENVOY_AGENTX_INDEX
#define AX_MA_RTN_IND_ALLOC(AX, RP, CON)   ax_ma_index_allocate(AX, RP, CON)
#define AX_MA_RTN_IND_DEALLOC(AX, RP, CON) ax_ma_index_deallocate(AX, RP, CON)
#else
#define AX_MA_RTN_IND_ALLOC(AX, RP, CON)   ((RP)->error_stat = GEN_ERR)
#define AX_MA_RTN_IND_DEALLOC(AX, RP, CON) ((RP)->error_stat = GEN_ERR)
#endif

#ifdef __cplusplus
}
#endif

#endif /* envoy_agentx_inc */
