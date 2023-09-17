/*  wm.h - main header for WindManage */

/* Copyright 2003-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
02d,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
02c,26jul09,m_z  add support to vxworks secure key database
                 (WIND00113818)
01b,17may06,ps  add string.h and strings.h include files. CQ WIND00044862
01a,16feb06,ps  changes for webcli on linux
01e,25apr05,???  Add __WM_PDU_BRIDGE_SNMP_STACK__ to also define ENVOY_STACK_K
01d,22jan04,jws  get rid of #warning when COMMON_IS_COMPONENT is defined
01c,12jan04,jws  minor componentization changes
01b,02jan04,jws  start componentization (COMMON_IS_COMPONENT)
01a,23sep03,jws  First attempt at adding coding standards related text.
                 Remove assert.h include; add include relative paths.
*/

/*
 * DESCRIPTION:
 *
 * To be added...
 */

#ifndef __INCwmh
#define __INCwmh

/* system include files */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

/* The project include file or installation dependent definitions. */

#ifndef COMMON_IS_COMPONENT

/* PSERVITA MUNGE */
#ifdef __UNIX__
# include "vxDefs.h"
#endif
/* PSERVITA MUNGE */

#include "commonCfg.h"
#else

#define __VXWORKS_OS__

#undef __WM_INVALIDATE__         /* Never defined!!  Lose it? */
#undef __DECOMPRESSION_ENABLED__ /* Never defined!!  Lose it? */
#undef __CONSOLE_LOG__           /* Never defined!!  Lose it? */
#undef MAX_EXTENDED_TYPES        /* Never defined!!  Lose it? */
#undef __ALLOW_ENUM_ORDINALS__   /* Never defined!!  Lose it? */
#undef __USE_MEMORY_BLOCKS__     /* Never defined!!  Lose it? */

#undef __ENABLE_IPC__            /* Never defined!!  Lose it? */
#undef __ENABLE_SHARED_MEMORY__  /* Only defined if __ENABLE_IPC__ defined */

/*#undef MUD_CONNECTION_VERIFICATION_IS_REQUIRED */ /* Never defined!! Lose it? */

#undef __WM_NO_LOCKS__           /* Never defined!!  Lose it? */

#undef __ANSI_FILE_MANAGER_ENABLED__ /* project enhancement */
#undef __ENABLE_WMB_USERS__          /* project enhancement */
#undef __ENABLE_CUSTOM_STRUCT__      /* project enhancement */


/*
 * __OS_MALLOC_PROVIDED__ controls whether we use a pre-allocated
 * memory pool for our malloc() type memory allocations, or use the
 * OS malloc().  The name __OS_MALLOC_PROVIDED__ is a bit misleading.
 */

#define __OS_MALLOC_PROVIDED__
#define kInitialMemoryAllocation 128000

#define __USE_LOCAL_ANSI_LIB__

#undef __ENABLE_THREAD_POOL__    /* can be defined, but never is!! */
#undef __WM_MAX_THREADS__        /* defined only in wmLib.c */

#undef EXTERNAL_START_FUNCTION   /* never defined!! Ref'd in wmLib.c */
#undef POST_WM_INIT_FUNCTION     /* never defined!! Ref'd in wmLib.c */
#undef __WM_DIRTY__              /* never defined!! Ref'd in memPoolLib.c */
#undef __WMB_TM_EXPLICIT_TID__   /* never defined!! Ref'd in wmbTransactionCore.c */
#undef __ENABLE_SNMP_RESOURCE_REGISTER__ /* never defined!! Ref'd in wmbSNMPLib.c */
#undef __ENABLE_LAN_IP_FILTER__  /* can be defined, but never is!! */
#undef __WMB_VIEW_ACCESS_MODEL__ /* can be defined, but never is!! */

#define __RLI_ACCESS_LEVEL_MASK__   0x0ffff

/* These next three defines have to be examined. */

#define __SNMP_API_ENABLED__
#define __WM_SNMP_STACK__
#undef  __WM_MASTER_SUBAGENT_SNMP_STACK__


/*
 * Next four are ifdef'd, then defined, but never defined external
 * to that module!!
 */

#undef WM_DELIM_META_K            /* wmbObjectLib.c */
#undef WM_DELIM_INSTANCE_K
#undef WM_PIPESIZE_K              /* wmbLib.c */
#undef WMB_LISTENER_SUFFIX_K      /* wmbComponentLib.c */


#define __DISABLE_STRUCTURES__    /* should be defined, but usually is not in wm_options.h?? */

#undef __DATABASE_USE_BTREE__     /* ever define? */

/* Signed Quantities */
typedef char sbyte;
typedef short sbyte2;
typedef int sbyte4;
#ifdef _WRS_CONFIG_LP64
typedef long sbyte8;
#endif

/* Unsigned Quantities */
typedef unsigned char ubyte;
typedef unsigned short ubyte2;
typedef unsigned int ubyte4;
#ifdef _WRS_CONFIG_LP64
typedef unsigned long ubyte8;
#endif

typedef ubyte2 Access;

#undef  HTTP_SHOW      /* defined by WMIT; compiles show routines */

/* === not used anymore except by per-project files === */

#define __WM_EVENT_MANAGER__

/* === */
#endif /* COMMON_IS_COMPONENT */


/* ====== Begin installation independent definitions ====== */

/* prevents envoy glue.h included by snmp.h from redefining STRXXX functions */

#define COMMON_GLUE_H

#if defined(__ENABLE_IPC__) && ! defined(__ENABLE_SHARED_MEMORY__)
#define __ENABLE_SHARED_MEMORY__
#endif

#if defined(__BSD_OS__) || defined(__SOLARIS_OS__) || defined(__POSIX_OS__)
#define __UNIX__
#endif

/* many flavors of epilogue snmp stack: we just need generic flag */

#if (defined(__WM_SNMP_STACK__) || defined(__WM_MASTER_SUBAGENT_SNMP_STACK__) || defined(__WM_PDU_BRIDGE_SNMP_STACK__))
#define ENVOY_STACK_K
#endif

/* mibway now uses the backplane for OID lookup */

/*
#undef __RLI_MIB_TRANSLATION__
#undef __MIBWAY_USE_TABLE_DATATYPE__
*/

/* patched legacy code can look for this define to know how to run */

#define __WINDMANAGE__

#ifdef WM_SECURE_KEY_STORAGE
#define WM_WEBSERVER_SECURE_KEY_ID  "wm_webserver_private_key"
#endif

/* lots of WindManage header files */

#include "wmData.h"
#include "wmStdLib.h"
#include "wmos.h"
#include "wmnet.h"
#include "memPoolLib.h"
#include "wmLib.h"
#include "wmMemBlockLib.h"
#include "wmAccessLib.h"
#include "wmBufferLib.h"
#include "wmConvertLib.h"
#include "wmErrors.h"
#include "wmFileLib.h"
#include "wmHashLib.h"
#include "wmOidLib.h"
#include "wmInstanceLib.h"
#include "wmLinkListLib.h"
#include "wmLogLib.h"

#ifdef __ENABLE_WMB_USERS__
#include "wmUserLib.h"
#endif

#endif /* __INCwmh */
