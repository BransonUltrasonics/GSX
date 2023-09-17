/* commonCfg.h */

/* Copyright(c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01nov16,r_w  add kMagicMarkupBufferSize config option (V7MAN-186)
19nov14,f_f  using the memory management flag in vsb config if not defining HV
             (V7MAN-122) 
29sep14,mmi  HYP-9809,US45391: Disable dynamic memory allocation for vx7-virt
04aug14,f_f  make some options configurable in VSB (US15622)
24jun14,md   only enable the SNMP API if SNMP is included in the VSB
23may14,r_w  written
*/

#ifndef __COMMON_CFG_H_HEADER__
#define __COMMON_CFG_H_HEADER__

#include <vsbConfig.h>
#include <vxWorks.h>

#undef   __WM_INET6__
#undef __ENABLE_WMB_USERS__
#undef  INCLUDE_OPENSSL_SUPPORT
#undef  HTTP_SSL_PORT
#undef  HTTP_SSL_CA_FILE
#undef  HTTP_SSL_CA_PATH
#undef  HTTP_SSL_CERTIFICATE_FILE
#undef  HTTP_SSL_CIPHER_LIST
#undef  HTTP_SSL_PRIVATE_KEY_FILE
#undef  HTTP_SSL_V2
#undef  HTTP_SSL_V3
#undef  HTTP_TLS_V1
#undef  MD5_ENABLED_COMMON
#undef  INCLUDE_HTTP_CGI_COMMON
#undef kMagicMarkupBufferSize
#undef kMagicMarkupStartChar0
#undef kMagicMarkupStartChar1
#undef kMagicMarkupEndChar0
#undef kMagicMarkupEndChar1
#undef __ENABLE_THREAD_POOL__
#undef __WM_MAX_THREADS__
#undef __ENABLE_LAN_IP_FILTER__
#undef RLI_CUSTOM_DYNAMIC_IP_FILTER_ENABLED
#undef RLI_CUSTOM_DYNAMIC_IP_CHECK
#undef __SNMP_API_ENABLED__
#undef __WM_SNMP_STACK__
#undef __WM_MASTER_SUBAGENT_SNMP_STACK__
#undef __WM_PDU_BRIDGE_SNMP_STACK__
#undef __DISABLE_STRUCTURES__
#undef __WM_EVENT_MANAGER__
#undef __RLI_ACCESS_LEVEL_MASK__
#undef __RLI_ACCESS_OPTION_MASK__
#undef __RLI_ACCESS_LEVEL_SHIFT__
#undef EMPTY
#undef __WMB_VIEW_ACCESS_MODEL__
#undef WMB_NAME_K
#undef WMB_PARENT_NAME_K       
#undef WMB_PIPE_ENABLE_K       
#undef WMB_PORT_ENABLE_K       
#undef WMB_TIMEOUT_K           
#undef WMB_UDP_PORT_K          
#undef UDP_BUFSIZE             
#undef RM_BUFFER_SIZE          
#undef WMB_PRODUCER_NAME_MAX_K 
#undef MAGIC_WMB_HEADER_K      
#undef WMB_LISTENER_PORT_K     
#undef WMB_BACKPLANE_NAME_MAX_K
#undef WMB_PRODUCER_NAME_SIZE_K
#undef WMB_SHARED_DATA_NAME_K  
#undef WMB_SHARED_DATA_SIZE_K  
#undef WMB_OBJECT_POOL_K       
#undef WMB_BUFFER_POOL_K       
#undef WMB_SESSION_VARS_K      
#undef WMB_BACKPLANE_MEMORY_K  
#undef DEFAULT_LOCAL_ROWS_K    
#undef DEFAULT_LOCAL_SIZE_K    
#undef WMB_OFFSET
#undef __ENABLE_MEMMGR_DEBUG__

/* WEBCLI menu */
#ifndef _WRS_CONFIG_WEBCLI_GENERAL_SYS_THREAD_OPT
#define kWmosThreadCreateOptions 0
#else
#define kWmosThreadCreateOptions _WRS_CONFIG_WEBCLI_GENERAL_SYS_THREAD_OPT
#endif

#ifndef _WRS_CONFIG_WEBCLI_GENERAL_SYS_THREAD_MULTI
#define __SINGLE_THREADED_SERVER_ENABLED__
#undef __MULTI_THREADED_SERVER_ENABLED__
#else
#undef __SINGLE_THREADED_SERVER_ENABLED__
#define __MULTI_THREADED_SERVER_ENABLED__
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_SYS_USE_ANSI
#define __USE_LOCAL_ANSI_LIB__
#else
#undef __USE_LOCAL_ANSI_LIB__
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_SYS_MEM_HOST
#define __OS_MALLOC_PROVIDED__
#else
#undef __OS_MALLOC_PROVIDED__
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_SYS_MEM_ALLOC
#define kInitialMemoryAllocation _WRS_CONFIG_WEBCLI_GENERAL_SYS_MEM_ALLOC
#else
#define kInitialMemoryAllocation  256000
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_DEBUG_WM
#define __WM_DEBUG__
#else
#undef __WM_DEBUG__
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_DEBUG_RCC
#define __RCC_DEBUG__
#else
#undef __RCC_DEBUG__
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_DEBUG_HTTP
#define HTTP_DEBUG
#else
#undef HTTP_DEBUG
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_DEBUG_HTTP_SHOW
#define HTTP_SHOW
#else
#undef HTTP_SHOW
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_DEBUG_INFO
#define DEBUG_INFO
#else
#undef DEBUG_INFO
#endif

#ifdef _WRS_CONFIG_WEBCLI_GENERAL_DEBUG_WM_ERROR
#define __WM_ERROR_SHOW__
#else
#undef __WM_ERROR_SHOW__
#endif

#ifndef _WRS_CONFIG_WEBCLI_GENERAL_FILE_MGR
#undef __ANSI_FILE_MANAGER_ENABLED__
#else
#define __ANSI_FILE_MANAGER_ENABLED__
#endif

#ifndef _WRS_CONFIG_WEBCLI_GENERAL_SESSION_DIR
#undef __ENABLE_SESSION_DIRECTORIES__
#else
#define __ENABLE_SESSION_DIRECTORIES__
#endif

#ifndef _WRS_CONFIG_WEBCLI_GENERAL_DIR_PATH
#define kDIR_DEFAULT_PATH "/host/"
#else
#define kDIR_DEFAULT_PATH _WRS_CONFIG_WEBCLI_GENERAL_DIR_PATH
#endif

#ifndef _WRS_CONFIG_WEBCLI_GENERAL_DIR_LEN
#define kDIRECTORY_BUFFER_SIZE 256
#else
#define kDIRECTORY_BUFFER_SIZE _WRS_CONFIG_WEBCLI_GENERAL_DIR_LEN
#endif

/* WEBCLI menuend */

#ifndef _WRS_CONFIG_WEBCLI_WM_INET6
#undef   __WM_INET6__
#else
#define  __WM_INET6__
#endif

#ifndef _WRS_CONFIG_WEBCLI_ENABLE_WMB_USERS
#undef __ENABLE_WMB_USERS__
#else
#define __ENABLE_WMB_USERS__ 
#endif

/* SSL configuration */

/* Enable SSL Support Code */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_OPENSSL_SUPPORT
#undef  INCLUDE_OPENSSL_SUPPORT
#else
#define  INCLUDE_OPENSSL_SUPPORT
#endif

/* SSL port number */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_PORT
#define  HTTP_SSL_PORT 443
#else
#define  HTTP_SSL_PORT _WRS_CONFIG_WEBCLI_HTTP_SSL_PORT
#endif

/* SSL Certificate Authority File */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_CA_FILE
#define  HTTP_SSL_CA_FILE ""
#else
#define  HTTP_SSL_CA_FILE _WRS_CONFIG_WEBCLI_HTTP_SSL_CA_FILE
#endif

/* SSL Certificate Authority File Path */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_CA_PATH
#define  HTTP_SSL_CA_PATH ""
#else
#define  HTTP_SSL_CA_PATH _WRS_CONFIG_WEBCLI_HTTP_SSL_CA_PATH
#endif

/* SSL Certificate file */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_CERTIFICATE_FILE
#define  HTTP_SSL_CERTIFICATE_FILE "/romfs/htdocs/server.pem"
#else
#define  HTTP_SSL_CERTIFICATE_FILE _WRS_CONFIG_WEBCLI_HTTP_SSL_CERTIFICATE_FILE
#endif

/* SSL Cipher     */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_CIPHER_LIST
#define  HTTP_SSL_CIPHER_LIST "DEFAULT"
#else
#define  HTTP_SSL_CIPHER_LIST _WRS_CONFIG_WEBCLI_HTTP_SSL_CIPHER_LIST
#endif

/* SSL Private Key */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_PRIVATE_KEY_FILE
#define  HTTP_SSL_PRIVATE_KEY_FILE "/romfs/htdocs/server.pem"
#else
#define  HTTP_SSL_PRIVATE_KEY_FILE _WRS_CONFIG_WEBCLI_HTTP_SSL_PRIVATE_KEY_FILE
#endif

/* Enable ssl v2  */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_V2
#undef  HTTP_SSL_V2
#else
#define  HTTP_SSL_V2
#endif

/* Enable ssl v3  */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_SSL_V3
#undef  HTTP_SSL_V3
#else
#define  HTTP_SSL_V3
#endif

/* Enable tls v1  */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_TLS_V1
#undef  HTTP_TLS_V1
#else
#define  HTTP_TLS_V1
#endif

#ifndef _WRS_CONFIG_WEBCLI_MD5_ENABLED_COMMON
#undef MD5_ENABLED_COMMON
#else
#define MD5_ENABLED_COMMON 
#endif

#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_CGI_COMMON
#undef INCLUDE_HTTP_CGI_COMMON
#else
#define  INCLUDE_HTTP_CGI_COMMON
#endif

#define __VXWORKS_OS__
#undef __VXWORKS_AE_OS__
#undef __BSD_OS__
#undef __WIN32_OS__
#undef __POSIX_OS__

/* Compiler Specific Type Definitions */
/* Signed Quantities */
typedef char sbyte;
typedef short sbyte2;
typedef int sbyte4;
typedef signed long sbyte8;

/* Unsigned Quantities */
typedef unsigned char ubyte;
typedef unsigned short ubyte2;
typedef unsigned int ubyte4;
typedef unsigned long ubyte8;

#ifndef _WRS_CONFIG_WEBCLI_SUBSTITUTION_BUFFERSIZE
#define kMagicMarkupBufferSize 1000
#else
#define kMagicMarkupBufferSize _WRS_CONFIG_WEBCLI_SUBSTITUTION_BUFFERSIZE
#endif

/* WindMark delimiters */
#define kMagicMarkupStartChar0 '$'
#define kMagicMarkupStartChar1 '%'
#define kMagicMarkupEndChar0 '#'
#define kMagicMarkupEndChar1 '$'

/* Thread Pool Flags  */
#undef __ENABLE_THREAD_POOL__
#undef __WM_MAX_THREADS__

/* HYP-9809: executing a template from a script will not work without this change */

#ifdef _WRS_CONFIG_HYPERVISOR
    #undef __OS_MALLOC_PROVIDED__
    #define kInitialMemoryAllocation 256000
#endif


#undef __ENABLE_LAN_IP_FILTER__
#undef RLI_CUSTOM_DYNAMIC_IP_FILTER_ENABLED
#undef RLI_CUSTOM_DYNAMIC_IP_CHECK

#if defined(_WRS_CONFIG_SNMP)
#define __SNMP_API_ENABLED__
#define __WM_SNMP_STACK__
#endif
#undef __WM_MASTER_SUBAGENT_SNMP_STACK__
#undef __WM_PDU_BRIDGE_SNMP_STACK__

#undef __DISABLE_STRUCTURES__
#undef  __WM_EVENT_MANAGER__

#define __RLI_ACCESS_LEVEL_MASK__   0x000ff
#define __RLI_ACCESS_OPTION_MASK__  0x0ff00
#define __RLI_ACCESS_LEVEL_SHIFT__  8
#define EMPTY 16
typedef ubyte2 Access;
#undef __WMB_VIEW_ACCESS_MODEL__

/* System Default Definitions */
#define WMB_NAME_K                  "publicWMB"
#define WMB_PARENT_NAME_K           ""
#undef  WMB_PIPE_ENABLE_K           
#undef  WMB_PORT_ENABLE_K           
#define WMB_TIMEOUT_K               100
#define WMB_UDP_PORT_K              1999
#define UDP_BUFSIZE                 256
#define RM_BUFFER_SIZE              128
#define WMB_PRODUCER_NAME_MAX_K     32
#define MAGIC_WMB_HEADER_K          0xfeedbabe
#define WMB_LISTENER_PORT_K         1999
#define WMB_BACKPLANE_NAME_MAX_K    32
#define WMB_PRODUCER_NAME_SIZE_K    32
#define WMB_SHARED_DATA_NAME_K      "wmbSharedData"
#define WMB_SHARED_DATA_SIZE_K      10000
#define WMB_OBJECT_POOL_K           32
#define WMB_BUFFER_POOL_K           32
#define WMB_SESSION_VARS_K          128
#define WMB_BACKPLANE_MEMORY_K      256000
#define DEFAULT_LOCAL_ROWS_K        32
#define DEFAULT_LOCAL_SIZE_K        32

#define WMB_OFFSET(STRUCTURE, FIELD)    (void *)&(((STRUCTURE *) 0x0)->FIELD)


#undef __ENABLE_MEMMGR_DEBUG__

extern int global_kWmosThreadCreateOptions;
extern int global_kHwTicksPerSecond;

#endif /* __COMMON_CFG_H_HEADER__ */

