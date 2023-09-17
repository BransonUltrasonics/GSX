/* wmbConfig.h - configuration file for RCB library */

/* Copyright 2001 Wind River Systems, Inc. */

#ifndef __INCwmbConfigh
#define __INCwmbConfigh

/*
modification history
--------------------
00b,08aug01,pns written.
*/

/*
 * DESCRIPTION:
 *
 *   wmbLib.h contains the definitions for the constants that define
 *   system behavior. Most of these should exist in rc_options.h
 *
 */

/* system default definitions */

#ifndef WMB_MAX_PRODUCERS_K
#define WMB_MAX_PRODUCERS_K         12
#endif

#ifndef WMB_DB_ROWS_K
#define WMB_DB_ROWS_K               512
#endif

#ifndef WMB_NAME_K
#define WMB_NAME_K                  "publicWMB"
#endif

#ifndef WMB_UDP_PORT_K
#define WMB_UDP_PORT_K              1999
#endif

#ifndef UDP_BUFSIZE
#define UDP_BUFSIZE                 256
#endif

#ifndef RM_BUFFER_SIZE
#define RM_BUFFER_SIZE              128
#endif

#ifndef WMB_PRODUCER_NAME_MAX_K
#define WMB_PRODUCER_NAME_MAX_K     32
#endif


#ifndef MAGIC_WMB_HEADER_K
#define MAGIC_WMB_HEADER_K          0xfeedbabe
#endif

#ifndef WMB_LISTENER_PORT_K
#define WMB_LISTENER_PORT_K         1999
#endif

#ifndef WMB_BACKPLANE_NAME_MAX_K
#define WMB_BACKPLANE_NAME_MAX_K    32
#endif

#ifndef WMB_PRODUCER_NAME_SIZE_K
#define WMB_PRODUCER_NAME_SIZE_K    32
#endif

#ifndef WMB_SHARED_DATA_NAME_K
#define WMB_SHARED_DATA_NAME_K      "wmbSharedData"
#endif

#ifndef WMB_SHARED_DATA_SIZE_K
#define WMB_SHARED_DATA_SIZE_K      10000
#endif

#ifndef WMB_OBJECT_POOL_K
#define WMB_OBJECT_POOL_K           32
#endif

#ifndef WMB_BUFFER_POOL_K
#define WMB_BUFFER_POOL_K           32
#endif

#ifndef WMB_QUEUE_SIZE_K
#define WMB_QUEUE_SIZE_K            16
#endif

#ifndef WMB_SESSION_VARS_K
#define WMB_SESSION_VARS_K          128
#endif

#ifndef WMB_BACKPLANE_MEMORY_K
#define WMB_BACKPLANE_MEMORY_K      256000
#endif

/* not used? */
#ifndef DEFAULT_LOCAL_ROWS_K
#define DEFAULT_LOCAL_ROWS_K        32
#endif

#ifndef DEFAULT_LOCAL_SIZE_K
#define DEFAULT_LOCAL_SIZE_K        32
#endif


#endif /* __INCwmbConfigh */
