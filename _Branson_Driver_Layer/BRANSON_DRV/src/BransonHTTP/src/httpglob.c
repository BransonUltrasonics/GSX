/* httpglob.c - initialization and global variables */

/*
 * Copyright (c) 1997-2008, 2010-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
18apr16,r_w  fix mismatched prototype issue (V7MAN-182)
26sep14,r_w  fix static CHECKED_RETURN issue.
02jul14,r_w  support Etag on NVM file system US32703
12sep13,f_f  ipv6 support improvement
26mar13,r_w  chang unsigned long to int for defect WIND00409393
20feb13,r_w  initialize pointer variable
17sep12,r_w  correct for thread priority.
             Defect: WIND00375444
01nov11,r_w  HTTP header timeout support(WIND00307889)
07jun11,m_z  add etag/if-modified-since header process WIND00203773
             CQID: WIND00203773
20dec10,r_w  change "ACCESS-LANGUAGE" to "ACCEPT-LANGUAGE"(WIND00247143)
02feb10,h_y  support for LP64
28aug08,q_z  removed the some const declared  (WIND00032048)
12jun08,q_z  clear LC Poll Threads when server shutdown(WIND00106722)
22feb07,ps   apigen reformatting
12jul06,cws  Added JS to Mimetypes so Windmarks will be parsed in Javascript
03mar04,adb  WMIT backwards compatibility edits
26feb04,adb  introduced httpMudTasksDelete and httpMudInfrastructureDelete
17feb04,adb  enforced httpShutdown MUD layering dependency protection
12feb04,adb  modified httpInit and httpShutdown
23jan04,dkg  added "text/css" file type in MIME type suffix guess list
             (httpMimeSuffixTable) for SPR 91624.
30jan04,dlr  remove INCLUDE_HTTP_COOKIE_SUPPORT, make operation automatic
22jan04,jws  declare rc_filearray, if necessary
15jan04,jws  add componentization globals
14jan04,gsr  cleanup routines for SPR 93009
18aug03,jws  removed HTTP_BOOL
26oct01,cd   deleting httpPostFileUploadPath
22dec99,va   adding httpPostFileUploadPath
09dec99,csr  adding a comment for INCLUDE_HTTP_SERVER_UNCOMPRESS
09dec99,csr  adding HTTP_DEBUG: initialization debugging flag
01dec99,csr  fixing SPR# 29355 (httpglob assigns httpMaxSendCookies and
             httpMaxRecCookies unconditionally)
01dec99,csr  modifying comment regarding adding a system RPM
29sep99,csr  removing defined(INCLUDE_HTTP_FS) for httpSystemMemFsMaxFiles
14sep99,csr  adding httpSystemMemFsMaxFiles to global variable list
30aug99,km   removed references to virtual filesystem.
27aug99,csr  added compression code.
06aug99,km   added new MIME types.
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
06apr98,ck   variables for persistent connection handling added.
21may97,mb   separated from wmw_httpconf.c
*/

/*
DESCRIPTION
In this file global vars and initialization functions take place that must be
compiled to configure the HTTP server.

INCLUDE FILES:
    httpLib.h
    httpRpms.h
    httpMime.h
    httpWmb.h
    wmw_httpconf.h

NOMANUAL

*/

/* includes */

#include "httpLib.h"
#include "httpRpms.h"
#include "httpMime.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "httpWmb.h"
#endif
#ifdef INCLUDE_HTTP_LIVE_CONTROL
#include "httpLCLib.h"
#endif
#ifdef HTTP_ETAG_ENABLE
#include "httpMimeEtag.h"
#endif
#ifdef __JAVASCRIPT_DIGEST_ENABLED__
#include "wmMD5.h"
#include "httpAuth.h"
#endif
#ifdef WEBCLI_INCLUDE_SMTP
#include "smtp/smtpLib.h"
#endif

/* WMIT backwards compatibility definitions */


/* global constants */
const BOOL httpVarBasicMsgs = FALSE;


#ifdef __ENABLE_RCB_SUPPORT__

/* WMB to RC handlers support */

extern STATUS RC_MACRO_INIT(WMB_SESSION_T * pSession);
extern STATUS RC_MACRO_CLEANUP(WMB_SESSION_T * pSession);
extern STATUS RC_MACRO_PUSH(WMB_SESSION_T * pSession,int bPushStatus);
extern STATUS RC_MACRO_DONE_REPEAT(WMB_SESSION_T * pSession,int bPushStatus);

STATUS (*pRC_MACRO_INIT)(WMB_SESSION_T *) = RC_MACRO_INIT;
STATUS (*pRC_MACRO_CLEANUP)(WMB_SESSION_T *) = RC_MACRO_CLEANUP;
STATUS (*pRC_MACRO_PUSH)(WMB_SESSION_T *,int) = RC_MACRO_PUSH;
STATUS (*pRC_MACRO_DONE_REPEAT)(WMB_SESSION_T *,int) = RC_MACRO_DONE_REPEAT;

#else

#ifndef WEBCLI_STANDALONE_WEBSERVER
STATUS (*pRC_MACRO_INIT)(WMB_SESSION_T *) = NULL;
STATUS (*pRC_MACRO_CLEANUP)(WMB_SESSION_T *) = NULL;
STATUS (*pRC_MACRO_PUSH)(WMB_SESSION_T *,int) = NULL;
STATUS (*pRC_MACRO_DONE_REPEAT)(WMB_SESSION_T *,int) = NULL;
#endif

#endif

#if defined(__NO_ROMBUILD_IMAGE__)

void   (*pFILEMGR_Construct)(void) = NULL;
void * (*pFILEMGR_RetrieveFile)(int *, void *) = NULL;
void   (*pFILEMGR_ReleaseFile)(void *) = NULL;
void   (*pFILEMGR_DeConstruct)(void) = NULL;

STATUS (*pMemGzRead)(memGzFile_t *,sbyte *,ubyte4 *) = NULL;
STATUS (*pMemGzOpen)(sbyte *,memGzFile_t **,ubyte4 *) = NULL;
STATUS (*pMemGzClose)(memGzFile_t *) = NULL;
STATUS (*pMemGzSeek)(memGzFile_t *,ubyte4) = NULL;

const sbyte * rc_filearray[] = { NULL, NULL};

#else

#include "httpNvFileMgr.h"

void FILEMGR_DeConstruct(void);   /* should be in httpNvFileMgr.h */

void   (*pFILEMGR_Construct)(void) = FILEMGR_Construct;
void * (*pFILEMGR_RetrieveFile)(int *, void *) = FILEMGR_RetrieveFile;
void   (*pFILEMGR_ReleaseFile)(void *) = FILEMGR_ReleaseFile;
void   (*pFILEMGR_DeConstruct)(void) = FILEMGR_DeConstruct;

STATUS (*pMemGzRead)(memGzFile_t *,sbyte *,ubyte4 *) = memGzRead;
STATUS (*pMemGzOpen)(sbyte *,memGzFile_t **,ubyte4 *) = memGzOpen;
STATUS (*pMemGzClose)(memGzFile_t *) = memGzClose;
STATUS (*pMemGzSeek)(memGzFile_t *,ubyte4) = memGzSeek;

#endif

/* user implemented functions */
void   (*phttpWindMarkSetFailed) (void * reqId,char * windMark,unsigned short errnoVal) = NULL;
char * (*phttpWindMarkGetFailed) (void * reqId,char * windMark,unsigned short errnoVal) = NULL;
void   (*phttpPostReply) (void * reqId, short status) = NULL;

unsigned short globle_HTTP_DEBUG = 1;
extern  unsigned short httpSSLPort;
extern  int            httpMaxSockBuff;
extern  unsigned short httpHeaderTimeout;       /* HTTP header timeout support */
unsigned long  httpGlobalMemPartSize = 12096;
unsigned long  httpReqMemPartSize = 12096; /* #bytes for memory */                                                                  /* pool per task     */
unsigned short httpMaxArguments   = 96;
unsigned long  httpMaxUrlSize     = 386;  /* #bytes for buffer to store the HTTP request line*/
unsigned long  httpMaxTempBufSize = 386;  /* #bytes for a temporary buffer  */
unsigned short httpMaxMimeLines   = 96;     /* Max. length of MIME header       */
unsigned short httpMaxSendCookies = 10;   /* Max. number of cookies to send   */
unsigned short httpMaxRecCookies  = 10;    /* Max. number of cookies to receive*/
unsigned short httpPort           = 80;        /* #TCP socket port  */
unsigned short httpMaxPorts       = 4;         /* max open ports    */
unsigned short httpMaxConnections = 20;        /* max open sockets  */
unsigned short httpSockTimeout    = 400;       /* socket timeout    */
unsigned short httpPriority       = 200;       /* task priority     */
unsigned short httpMaxProcesses   = 20;        /* #daemon tasks     */
long           httpdTaskStackSize = 15000;    /* #bytes for stack per server process*/
char         * httpServerName   = "www.target.own";        /* the server name or IP */
char         * httpServerIdentification = "WindRiver-WebServer/4.9";
char         * global_HTTP_BASIC_REALM = "EnchantedWorld";
short global_HTTP_SYSRPM_CONF_MAX = 5;
unsigned short global_HTTP_RPM_CONF_MAX = 11;
unsigned short global_HTTP_RPM_DISP_MAX = 2;
unsigned short global_HTTP_MIMEPARSE_CONF_MAX = 10;
unsigned short global_HTTP_ALIAS_CONF_MAX = 11;
unsigned short global_HTTP_LOCATION_CONF_MAX = 4;
unsigned short  kMacroRepeatNestMax = 8;

unsigned short global_HTTP_LOCATION_NAME_SIZE = 11;
unsigned short global_HTTP_IP_PER_LOCATION_MAX = 3;
unsigned short global_HTTP_IP_PATTERN_SIZE = 11;    
unsigned short global_HTTP_GID_CONF_MAX = 3;       
unsigned short global_HTTP_GID_NAME_SIZE = 11;      
unsigned short global_HTTP_USER_PER_GID = 3;       
unsigned short global_HTTP_USERNAME_SIZE = 7;      
unsigned short global_HTTP_PASSWORD_SIZE = 7;      
unsigned short global_HTTP_CTRL_CONF_MAX = 3;      
unsigned short global_HTTP_MEMBERS_PER_CTRL = 3;   
unsigned short global_HTTP_CTRL_URL_SIZE = 8;      
unsigned short global_HTTP_CGI_FN_CONF_MAX = 1;    
unsigned short global_HTTP_UPLOAD_CONF_MAX = 1;    
unsigned short global_HTTP_FS_CONF_MAX = 5;        
unsigned short global_HTTP_WMB_PATHS_MAX = 3;      
unsigned short global_HTTP_FILE_REGISTRY_MAX = 33; 

#if 0
#ifdef INCLUDE_HTTP_SERVER_UNCOMPRESS  /* server uncompression enabled? */
BOOL                 httpServerUncompress = TRUE;    /* enable server http    */
                                                     /* content uncompression */
#else
BOOL                 httpServerUncompress = FALSE;   /* disable server http   */
                                                     /* content uncompression */
#endif /* INCLUDE_HTTP_SERVER_UNCOMPRESS */
#endif

BOOL                 httpPersistentConnectionsAllowed = TRUE;
char                 * httpUri          = "http://";               /* the http URI */

char           * kUniqueSignature = "OWOWOWOWOWOWOWOW";
const long           kSectorSize        = HTTP_SECTOR_SIZE;
const long           kNVRamSize         = HTTP_NVRAM_SIZE;

#ifdef INCLUDE_OPENSSL_SUPPORT
 char           httpSSLCertificateFile[] = HTTP_SSL_CERTIFICATE_FILE;
 char           httpSSLPrivateKeyFile[]  = HTTP_SSL_PRIVATE_KEY_FILE;
 char           httpSSLCipherList[]      = HTTP_SSL_CIPHER_LIST;
 char           httpSSLCAFile[]          = HTTP_SSL_CA_FILE;
 char           httpSSLCAPath[]          = HTTP_SSL_CA_PATH;
#endif

#if defined(INCLUDE_HTTP_SYSTEMMEMFS)
unsigned long httpSystemMemFsMaxFiles = 0;/* the maximum number of files for systemFs, defined by pagepack */
#endif

/* standard HTTP status codes, you MAY change the order or add new items */
const HTTP_STATUS_MSG httpStatusMessages [] =
    {
    { HTTP_CONTINUE ,        "Continue"                      },
    { HTTP_REQ_OK,           "OK"                            },
    { HTTP_MULT_CHOICE,      "Multiple Choices"              },
    { HTTP_BAD_REQUEST,      "Bad Request"                   },
    { HTTP_NOT_IMPLEMENTED,  "Not Implemented"               },
#if defined (HTTP_MESSAGES_FULL)
    { HTTP_SWITCH_PROT,      "Switching Protocols"           },
    { HTTP_CREATED,          "Created"                       },
    { HTTP_ACCEPTED,         "Accepted"                      },
    { HTTP_NON_AUTH_INF,     "Non-Authoritative Information" },
    { HTTP_NO_CONTENT,       "No Content"                    },
    { HTTP_RESET_CONTENT,    "Reset Content"                 },
    { HTTP_PART_CONTENT,     "Partial Content"               },
    { HTTP_MOVED_PERM,       "Moved Permanently"             },
    { HTTP_MOVED_TEMP,       "Moved Temporarily"             },
    { HTTP_SEE_OTHER,        "See Other"                     },
    { HTTP_NOT_MODIFIED,     "Not Modified"                  },
    { HTTP_USE_PROXY,        "Use Proxy"                     },
    { HTTP_UNAUTHORIZED,     "Unauthorized"                  },
    { HTTP_PAYMENT_REQ,      "Payment Required"              },
    { HTTP_FORBIDDEN,        "Forbidden"                     },
    { HTTP_NOT_FOUND,        "Not Found"                     },
    { HTTP_METHOD_NA,        "Method Not Allowed"            },
    { HTTP_NOT_ACCEPTED,     "Not Acceptable"                },
    { HTTP_PROXY_AUTH_REQ,   "Proxy Authentication Required" },
    { HTTP_REQ_TIME_OUT,     "Request Time-out"              },
    { HTTP_CONFLICT,         "Conflict"                      },
    { HTTP_GONE,             "Gone"                          },
    { HTTP_LENGTH_REQIRED,   "Length Required"               },
    { HTTP_PRECOND_FAIL,     "Precondition Failed"           },
    { HTTP_ENTITY_TOO_LARGE, "Request Entity Too Large"      },
    { HTTP_URI_TOO_LARGE,    "Request-URI Too Large"         },
    { HTTP_UNSUP_MEDIA_TYPE, "Unsupported Media Type"        },
    { HTTP_INTERNAL_ERROR,   "Internal Server Error"         },
    { HTTP_BAD_GATEWAY,      "Bad Gateway"                   },
    { HTTP_SERV_UNAVAILABLE, "Service Unavailable"           },
    { HTTP_GATEWAY_TIMEOUT,  "Gateway Time-out"              },
    { HTTP_VERSION_NOT_SUPP, "HTTP Version not supported"    },
#endif /* HTTP_MESSAGES_FULL */
    { 0,                     "N/A"                           }
    };


#ifdef INCLUDE_HTTP_MIME_GUESSTYPE
static const HTTP_MIME_SUFFIX_GUESS httpMimeSuffixTable [] =
    {
     /* File suffix, expected client application */
    { "TXT",   "text/plain"               },
    { "C",     "text/plain"               },
    { "JAVA",  "text/plain"               },
    { "HTML",  "text/html"                },
    { "HTM",   "text/html"                },
    { "CSS",   "text/css"                 },
    { "SGML",  "text/x-sgml"              },
    { "SGM",   "text/x-sgml"              },
    { "TSV",   "text/tab-separated-values"},
    { "DOC",   "application/msword"       },
    { "PPT",   "application/powerpoint"   },
    { "PDF",   "application/pdf"          },
    { "CLASS", "application/octet-stream" },
    { "BIN",   "application/octet-stream" },
    { "PS",    "application/postscript"   },
    { "EPS",   "application/postscript"   },
    { "TCL",   "application/x-tcl"        },
    { "TAR",   "application/x-tar"        },
    { "TGZ",   "application/x-gtar"       },
    { "Z",     "application/x-compress"   },
    { "GZ",    "application/x-gzip"       },
    { "ZIP",   "application/zip"          },
    { "GIF",   "image/gif"                },
    { "JPEG",  "image/jpeg"               },
    { "JPG",   "image/jpeg"               },
    { "JPE",   "image/jpeg"               },
    { "PBM",   "image/x-portable-bitmap"  },
    { "PPM",   "image/x-portable-pixmap"  },
    { "XBM",   "image/x-xbitmap"          },
    { "XPM",   "image/x-xpixmap"          },
    { "AU",    "audio/basic"              },
    { "SND",   "audio/basic"              },
    { "WAV",   "audio/x-wav"              },
    { "RA",    "audio/x-realaudio"        },
    { "RAM",   "audio/x-pn-realaudio"     },
    { "RPM",   "audio/x-pn-realaudio-plugin"},
    { "MPGA",  "audio/mpeg"               },
    { "MP2",   "audio/mpeg"               },
    { "MP3",   "audio/mpeg"               },
    { "AIF",   "audio/x-aiff"             },
    { "AIFF",  "audio/x-aiff"             },
    { "AIFC",  "audio/x-aiff"             },
    { "MPEG",  "video/mpeg"               },
    { "MPG",   "video/mpeg"               },
    { "MPE",   "video/mpeg"               },
    { "QT",    "video/quicktime"          },
    { "MOV",   "video/quicktime"          },
    { "AVI",   "video/x-msvideo"          },
    { "WRL",   "x-world/x-vrml"           },
    { "VRML",  "x-world/x-vrml"           },
    { "JS",    "text/html"                },
    { NULL,     NULL                      }
    };
#endif

#ifdef INCLUDE_HTTP_WMB_INTERFACE
    sbyte                httpWmbName[64] = "publicWMB";
#endif

HTTP_GEN_LIST_ID         httpIpConfId = NULL;
HTTP_GEN_LIST_ID         httpPwdConfId = NULL;
HTTP_GEN_LIST_ID         httpCtrlConfId = NULL;


/* default user group of the password RPM */
#ifdef INCLUDE_HTTP_PWD_CHECKING
const char * const httpDefGidString = NULL;       /* must be NULL if rpmPass is used ! */
#else
const char * const httpDefGidString = "Anybody";
#endif

/* default location group of the IP RPM */
#ifdef INCLUDE_HTTP_IP_CHECKING
const char * const httpDefLocationString = NULL;
#else
const char * const httpDefLocationString = "Everywhere"; /* provide some location group if rpmIpchk is not used */
#endif


/*
 * configure the system RPMs to be used
 * system RPMs are invoked for each request, so this is just a list
 * of function pointers
 *
 * Usually, you will only access this table through the
 * function httpSysRpmConfAdd. Nevertheless, the structure
 * can be statically preinitialized to save the overhead
 * of an API function call, but this is NOT RECOMMENDED
 *
 * NOTE: httpSystemRpms MUST be finished by a NULL entry
 *       even an empty table HAS TO contain a NULL entry!
 */
HTTP_RPM httpSystemRpms[10];

OS_THREAD_T *  httpServerTaskIds;

/* functions */


/****************************************************************************
*
* httpInit - initialization routine of the http server
*
* This routine is called once at start-up from the server task.
* It initializes the memory pool management, filesystem and symbol table if
* needed.
*
* NOMANUAL
*
* RETURNS: HTTP_OK on success,
*          HTTP_ERROR otherwise
*/
HTTP_STATUS httpInit ()
    {
if  (OK != mudLibInit ())
    {
    if (globle_HTTP_DEBUG)
      perror ("httpInit: mudLibInit failed");
    
    return HTTP_ERROR;
    }

if  (HTTP_OK != httpMudCreate ())
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: unable to initialize HTTP server");

    return HTTP_ERROR;
    }

    /* initialize the global vars */
    httpSystemRpms[0] = NULL;

#ifdef INCLUDE_HTTP_UPLOAD
if  (HTTP_OK != httpUploadConfInit (global_HTTP_UPLOAD_CONF_MAX))
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: Upload RPM initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: Upload RPM successfully initialized.\n");

#endif /* INCLUDE_HTTP_UPLOAD */

#ifdef INCLUDE_HTTP_CGI
if  (NULL == httpCgiFnConfInit (global_HTTP_CGI_FN_CONF_MAX))
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: CGI initialization failed");
    
    return HTTP_ERROR;    
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: CGI successfully initialized.\n");

#endif /* INCLUDE_HTTP_CGI */

#ifdef INCLUDE_HTTP_FS
/*
 * initialize the file registry
 */
if  (HTTP_OK != httpFileRegInit((ubyte2)global_HTTP_FILE_REGISTRY_MAX))
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: System File Registry initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: System File Registry successfully initialized.\n");


/*
 * initialize the server file manager
 */
if  (OK != httpFileMngrInit())
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: System File Manager initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: System File Manager successfully initialized.\n");


/*
 * initialize the file system RPM
 */
if  (HTTP_OK != httpFsConfInit (global_HTTP_FS_CONF_MAX))
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: File System RPM initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: File System RPM successfully initialized.\n");

#endif /* INCLUDE_HTTP_FS */


if  (httpRpmConfInit (global_HTTP_RPM_DISP_MAX, global_HTTP_RPM_CONF_MAX) == NULL)
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: RPM dispatcher initialization failed");
    
    return HTTP_ERROR;
    }

if  (httpSysRpmConfInit (global_HTTP_SYSRPM_CONF_MAX) == HTTP_ERROR)
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: RPM dispatcher (system RPMs) initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: RPM dispatcher successfully initialized.\n");



/*
 * configure the system RPMs to be used
 * system RPMs are invoked for each request, so you do not have to
 * specify any URLs or methods.
 *
 * NOTE: The order in which you add them will be the order, in which
 *       they will be invoked. Thus, the position can be relevant
 *       for certain RPMs (e.g. first determine the usergroup, THEN
 *       do some access controll)
 */

/* log all accesses to the server to file */
#ifdef INCLUDE_HTTP_LOG
if  (httpLogConfInit (HTTP_LOG_FILE) == HTTP_ERROR)
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: Log initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: Log successfully initialized.\n");

#endif

/*
 * configuration of the aliasing table for the alias rpm
 * Syntax:   "requested path in url", "path to be substituted"
 * remark: only one asterisk in the requested path may occur, which
 *         in turn will be replaced by the second string.
 * Example: "* /" -> "* /index.html" will convert "/pub/" to "/pub/index.html"
 *       (the ^ above spaces are just for the compiler ... normaly,
 *        there should be no spaces inbetween * and /!!!)
 */
#ifdef INCLUDE_HTTP_ALIASING
if  (NULL == httpAliasConfInit (global_HTTP_ALIAS_CONF_MAX))
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: Alias initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: Alias successfully initialized.\n");

#endif


/* enable support for MIME content type guessing RPM */
#ifdef INCLUDE_HTTP_MIME_GUESSTYPE
if  (httpMimeConfInit ((HTTP_MIME_SUFFIX_GUESS*)httpMimeSuffixTable) == HTTP_ERROR)
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: MIME type guessing RPM initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: MIME type guessing RPM successfully initialized.\n");

#endif /* INCLUDE_HTTP_MIME_GUESSTYPE */

/* configuration of Rapid Control Backplane
 *
 * Start the backplane and register the server as a component
 *
 */
 #ifndef WEBCLI_STANDALONE_WEBSERVER
#ifdef INCLUDE_HTTP_WMB_INTERFACE
if  (NULL == httpGetWmbComp ())
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: HTTP server failed to register with WindManage Backplane");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: HTTP server connected successfully to WindManage Backplane.\n");

    httpWmbParserConfInit (global_HTTP_WMB_PATHS_MAX);
    httpWmbPostConfInit ();
#endif
#endif

#ifdef INCLUDE_HTTP_POST
if  (OK != httpPostConfInit())
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: Post RPM initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: Post RPM successfully initialized.\n");

#endif

/*
 * configuration of MIME header processing
 *
 * Syntax:  ("Mime Header Field", processing function pointer)
 */
#ifndef INCLUDE_HTTP_MIME_HDR_PROC
    httpMimeParseFnConfInit (0); /* MIME parsing MUST be initialized, even if unused! */
#else
if  (httpMimeParseFnConfInit (global_HTTP_MIMEPARSE_CONF_MAX) != NULL)
    {
    #ifdef HTTP_ETAG_ENABLE
    httpMimeParseFnConfAdd ("If-Modified-Since",   httpMimeIfModifiedSince);
    httpMimeParseFnConfAdd ("IF-None-Match",   httpMimeIfNoneMatch);
    httpMimeParseFnConfAdd ("Etag",   httpMimeInEtag);
    #endif

    httpMimeParseFnConfAdd ("CONTENT-TYPE",   httpMimeContentType);
    httpMimeParseFnConfAdd ("ACCEPT-ENCODING",httpMimeAcceptEncoding);
    httpMimeParseFnConfAdd ("CONTENT-LENGTH", httpMimeContentLength);
    httpMimeParseFnConfAdd ("HOST",           httpMimeHost);

    #ifdef INCLUDE_HTTP_PWD_CHECKING
    httpMimeParseFnConfAdd ("AUTHORIZATION",  httpMimeAuthorization);
    #endif

    #ifdef INCLUDE_HTTP_LANGUAGE_NEGOTIATION
    httpMimeParseFnConfAdd ("ACCEPT-LANGUAGE",httpMimeAcceptLanguage);
    #endif

    #ifdef INCLUDE_HTTP_PERSISTENT_CONNECTION
    httpMimeParseFnConfAdd ("CONNECTION",     httpMimeConnection);
    httpMimeParseFnConfAdd ("KEEP-ALIVE",     httpMimeKeepAlive);
    httpMimeParseFnConfAdd ("PERSIST",        httpMimePersist);
    #endif

    if  ((httpMaxSendCookies > 0) || (httpMaxRecCookies > 0))
        httpMimeParseFnConfAdd ("COOKIE",     httpMimeReadCookie);
    }
#endif  /* INCLUDE_HTTP_MIME_HDR_PROC */


#ifdef INCLUDE_HTTP_IP_CHECKING
    /* enable IP checking support */
if  ((httpIpConfId = httpIpConfInit (global_HTTP_LOCATION_CONF_MAX,
            global_HTTP_IP_PER_LOCATION_MAX,
            global_HTTP_IP_PATTERN_SIZE,
            global_HTTP_LOCATION_NAME_SIZE)) == NULL)
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: http IP checking initialization failed");
    
    return HTTP_ERROR;
    }

if (globle_HTTP_DEBUG)
    printf ("httpInit: http IP checking successfully initialized.\n");

#endif


#ifdef INCLUDE_HTTP_PWD_CHECKING
    /* enable password checking */
if  ((httpPwdConfId = httpPwdConfInit (global_HTTP_GID_CONF_MAX,
             global_HTTP_USER_PER_GID,
             global_HTTP_GID_NAME_SIZE,
             /* 08/30/02 sld - Correct 'credential' size
                See httpPwdConfInit() in httpRpmPassword.c
                and httpBase64encode() in httpBase64Util.c
              */
             (unsigned short)((global_HTTP_USERNAME_SIZE+global_HTTP_PASSWORD_SIZE+1)*4/3+4),
             global_HTTP_BASIC_REALM)) == NULL)
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: http Password Checking initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: http Password Checking successfully initialized.\n");

#endif


#ifdef INCLUDE_HTTP_ACCESS_CTRL
    /* enable access control by password and IP address */
if  ((httpCtrlConfId = httpCtrlConfInit (global_HTTP_CTRL_CONF_MAX,global_HTTP_MEMBERS_PER_CTRL,global_HTTP_CTRL_URL_SIZE,global_HTTP_GID_NAME_SIZE,global_HTTP_LOCATION_NAME_SIZE)) == NULL)
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: http Access Control initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: http Access Control successfully initialized.\n");

#endif

#ifdef HTTP_ETAG_ENABLE
#ifndef HTTP_MAX_ETAG_FILES
#define HTTP_MAX_ETAG_FILES 50
#endif
if  (HTTP_ERROR == httpEtagInit(HTTP_MAX_ETAG_FILES))
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: http etag initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: http etag successfully initialized.\n");

#endif

#ifdef __JAVASCRIPT_DIGEST_ENABLED__
if  (OK > wmMD5INIT())
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: wmMD5 initialization failed");
    
    return HTTP_ERROR;
    }

if  (OK > wmAUTHConstruct())
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: wmAUTHConstruct initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: wmMD5 and wmAUTHConstruct successfully initialized.\n");

#endif

#ifdef WEBCLI_INCLUDE_SMTP
if  (OK > SMTP_Init())
    {
    if (globle_HTTP_DEBUG)
    perror ("httpInit: SMTP initialization failed");
    
    return HTTP_ERROR;
    }
if (globle_HTTP_DEBUG)
    printf ("httpInit: SMTP successfully initialized.\n");

#endif

    return HTTP_OK;
    }

/****************************************************************************
*
* httpCleanup - clean routine of the http server
*
* This routine is called once at the end of the server task.
* It cleans  memory pool management, filesystem and symbol table if exist
*
* NOMANUAL
*
* RETURNS: HTTP_OK on success and HTTP_ERROR otherwise
*/
HTTP_STATUS httpCleanup (void)
    {
    /* layering dependency */

    if  (NULL == httpMud)
        {
        return (HTTP_ERROR);
        }

    (void) httpRpmConfCleanup ();

#ifdef INCLUDE_HTTP_UPLOAD
    (void) httpUploadConfCleanup ();
#endif

#ifdef INCLUDE_HTTP_CGI
    (void) httpCgiFnConfCleanup ();
#endif

#ifdef INCLUDE_HTTP_FS
    (void) httpFileRegCleanup ();
    (void) httpFileMngrCleanup ();
    (void) httpFsConfCleanup ();
#endif

#ifdef INCLUDE_HTTP_ALIASING
    (void) httpAliasConfCleanup ();
#endif

#ifdef INCLUDE_HTTP_MIME_GUESSTYPE
    (void) httpMimeParseFnConfCleanup ();
#endif

#ifndef WEBCLI_STANDALONE_WEBSERVER
#ifdef INCLUDE_HTTP_WMB_INTERFACE
    (void) httpWmbParserConfCleanup ();
#endif
#endif

#ifdef INCLUDE_HTTP_PWD_CHECKING
    (void) httpPwdConfCleanup ();
#endif

#ifdef INCLUDE_HTTP_IP_CHECKING
    (void) httpIpConfCleanup ();
#endif

#ifdef INCLUDE_HTTP_ACCESS_CTRL
    (void) httpCtrlConfCleanup ();
#endif

#ifdef INCLUDE_HTTP_LIVE_CONTROL
    httpLCPollPushShutdown ();
#endif

#ifdef HTTP_ETAG_ENABLE
    (void) httpEtagDestroy();
#endif
     return HTTP_OK;

    } /* httpCleanup */

/****************************************************************************
*
* httpShutdown - HTTP shutdown routine
*
* It destroys the underlying MUD and cleans-up
*
* NOMANUAL
*
* RETURNS: HTTP_OK on success and HTTP_ERROR otherwise
*/
HTTP_STATUS httpShutdown
    (
    void
    )
    {
    if  (OK != httpMudTasksDelete ())
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpShutdown: httpMudTasksDelete failed\n", 0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    if  (HTTP_OK != httpCleanup ())
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpShutdown: httpCleanup failed\n", 0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    if  (HTTP_OK != httpMudInfrastructureDelete ())
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpShutdown: httpMudDestroy failed\n", 0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    if  (HTTP_OK != mudLibShutdown ())
        {
#ifdef _WRS_KERNEL
        (void)logMsg ("httpShutdown: mudLibShutdown failed\n", 0, 0, 0, 0, 0, 0);
#endif

        return (HTTP_ERROR);
        }

    return (HTTP_OK);

    } /* httpShutdown */

