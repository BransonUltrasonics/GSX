/* httpInit.c */

/* Copyright (c) 2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
22may14,r_w  create file
*/

#include "wmw_httpconf.h"
#include "httpCfg.h"

extern void httpWindMarkSetFailed (void * reqId, char * windMark,unsigned short errnoVal);
extern char *httpWindMarkGetFailed (void * reqId,char * windMark,unsigned short errnoVal);
extern void  httpPostReply (void * reqId, short status);

#ifdef __JAVASCRIPT_DIGEST_ENABLED__ 
extern void* wmGetAuthenticationLevelPassword (void *envVar,char *pUserName,char *pPassword);
extern void  wmStoreUserData(char *chBuffer);
#endif

void webcli_http_init(void)
{
  /* basic configuration */
  httpServerName = HTTP_SERVER_NAME;
  global_HTTP_BASIC_REALM = HTTP_BASIC_REALM;
  httpServerIdentification = HTTP_SERVER_IDENTIFICATION;
  httpUri = HTTP_URI;

  /* socket configuration */
  httpPort = HTTP_PORT;
  httpMaxSockBuff = HTTP_MAX_SOCK_BUFF;
  httpSockTimeout = HTTP_SOCK_TIMEOUT;
  httpHeaderTimeout = HTTP_HEADER_TIMEOUT;
  httpMaxConnections = HTTP_MAX_CONNECTIONS;
  httpMaxPorts = HTTP_MAX_PORTS;

  /* memory configuration */
  httpGlobalMemPartSize = HTTP_MAX_GLOBAL_MEM_PART_SIZE;       
  httpReqMemPartSize = HTTP_MAX_REQ_PART_SIZE;          
  httpMaxUrlSize = HTTP_MAX_REQLINE_SIZE;
  httpMaxTempBufSize = HTTP_MAX_REQLINE_SIZE;           
  httpMaxArguments = HTTP_MAX_ARGUMENTS;            
  httpMaxMimeLines = HTTP_MIME_MAXLINES;            
  httpMaxSendCookies = HTTP_MAX_SENDCOOKIES;          
  httpMaxRecCookies = HTTP_MAX_RECCOOKIES;           

  /* task configuration */
  httpPriority = HTTP_PROCESS_PRIORITY;      
  httpMaxProcesses = HTTP_MAX_PROCESSES;  
  httpdTaskStackSize = HTTP_TASK_STACK_SIZE;

  /* resource configuration */
  global_HTTP_SYSRPM_CONF_MAX = HTTP_SYSRPM_CONF_MAX;          /* max # of system RPMs */
  global_HTTP_RPM_CONF_MAX = HTTP_RPM_CONF_MAX;             /* max # of user RPMs per HTTP method */
  global_HTTP_RPM_DISP_MAX = HTTP_RPM_DISP_MAX;             /* max # of supported HTTP methods */
  global_HTTP_MIMEPARSE_CONF_MAX = HTTP_MIMEPARSE_CONF_MAX;       /* max # of MIME parsing functions */
  global_HTTP_ALIAS_CONF_MAX = HTTP_ALIAS_CONF_MAX;           /* max # of alias definitions */
  global_HTTP_LOCATION_CONF_MAX = HTTP_LOCATION_CONF_MAX;        /* max # of location groups */
  global_HTTP_LOCATION_NAME_SIZE = HTTP_LOCATION_NAME_SIZE;       /* max size of a location group string */
  global_HTTP_IP_PER_LOCATION_MAX = HTTP_IP_PER_LOCATION_MAX;      /* max # of IP patterns per location group */
  global_HTTP_IP_PATTERN_SIZE = HTTP_IP_PATTERN_SIZE;          /* max size of an IP pattern */
  global_HTTP_GID_CONF_MAX = HTTP_GID_CONF_MAX;             /* max # of user groups */
  global_HTTP_GID_NAME_SIZE = HTTP_GID_NAME_SIZE;            /* max size of a user group string */
  global_HTTP_USER_PER_GID = HTTP_USER_PER_GID;             /* max # of users per user group */
  global_HTTP_USERNAME_SIZE = HTTP_USERNAME_SIZE;            /* max size of a user name */
  global_HTTP_PASSWORD_SIZE = HTTP_PASSWORD_SIZE;            /* max size of a user password */
  global_HTTP_CTRL_CONF_MAX = HTTP_CTRL_CONF_MAX;            /* max # of URL paths for access control */
  global_HTTP_MEMBERS_PER_CTRL = HTTP_MEMBERS_PER_CTRL;         /* max # of access control clauses per URL path */
  global_HTTP_CTRL_URL_SIZE = HTTP_CTRL_URL_SIZE;            /* max size of an access control statement */
  global_HTTP_CGI_FN_CONF_MAX = HTTP_CGI_FN_CONF_MAX;          /* max # of CGI routines */
  global_HTTP_UPLOAD_CONF_MAX = HTTP_UPLOAD_CONF_MAX;          /* max # of path definitions for file upload */
  global_HTTP_FS_CONF_MAX = HTTP_FS_CONF_MAX;              /* max # of file system mounting points */
  global_HTTP_WMB_PATHS_MAX = HTTP_WMB_PATHS_MAX;            /* max # of WMB file system mounting points */
  global_HTTP_FILE_REGISTRY_MAX = HTTP_FILE_REGISTRY_MAX;        /* max # of special files registered with the server */
  kMacroRepeatNestMax = HTTP_MACRO_REPEAT_NEST_MAX;                  /* max # of nested depth for Repeat Macro */
  kUniqueSignature = HTTP_UNIQUE_SIGNATURE;
  #ifdef HTTP_DEBUG
  globle_HTTP_DEBUG = 1;  
  #else
  globle_HTTP_DEBUG = 0;  
  #endif

  phttpWindMarkSetFailed = httpWindMarkSetFailed;
  phttpWindMarkGetFailed = httpWindMarkGetFailed;
  phttpPostReply = httpPostReply;
  #ifdef __JAVASCRIPT_DIGEST_ENABLED__ 
  pwmGetAuthenticationLevelPassword = wmGetAuthenticationLevelPassword;
  pwmStoreUserData = wmStoreUserData;
  #endif
}
