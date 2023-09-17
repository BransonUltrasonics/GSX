/* httpErrorHandling.c - Provide debugging output functions */

/* Copyright(c) 1996-2010, 2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
02feb10,h_y  support for LP64 
09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
13may97,mb   osal introduced.
03sep96,ck   initial creation.
*/

/*
DESCRIPTION
This module provides information and error message logging
for the HTTP server.

INCLUDE FILES:
	httpLib.h
	httpTask.h

*/


#include "errno.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "httpLib.h"
#include "httpTask.h"
#include "httpCfg.h"


/*******************************************************************************
*
* httpLogErrorFunction - Log an error message to standard error output.
*
* This routine can be used when writing new code for the server to provide
* verbose error output for the developer.  When the source is
* compiled with	DEBUG_ERROR defined, the string will be printed using
* 'perror( )'.
* When DEBUG_ERROR is not defined (typically in the
* memory-critical version), the string will not be compiled (saving memory).
*
* RETURNS: N/A
*
*/

void httpLogErrorFunction
    (
    char * szMessage /* String to print with perror */
    )
    {
    /* fprintf ( stderr,"ERROR(%#8x): ", (unsigned int) httpOsalTaskIdSelf () ); */
    perror (szMessage);
    }


/*******************************************************************************
*
* httpLogErrorMsgFunction - Log an error message with a string param to stdout.
*
*
* This routine can be used when writing new code for the server to provide
* verbose output to the developer.  When the source is compiled with
* DEBUG_INFO defined, the string will be printed using 'fprintf( )' to stderr.
* When DEBUG_INFO is not defined (typically in the release version), the
* string will not be compiled (saving memory).
*
* Currently this routine works like this:
*\cs
*     printf (szMessage, szArg);
*\ce
*
* RETURNS: N/A
*
*/

void httpLogErrorMsgFunction
    (
    char * szMessage, /* string that may contain a printf format specifier */
    ...               /* printf argument */
    )
    {
    va_list vaList;

    va_start (vaList, szMessage);
    /* fprintf ( stderr,"ERROR(%#8x): ", (unsigned int) httpOsalTaskIdSelf () ); */
    (void) vfprintf ( stderr, szMessage, vaList);
    perror ( "" );

    va_end (vaList);
    }


/*******************************************************************************
*
* httpLogInfoFunction - Log an information message to standard output.
*
* This routine can be used when writing new code for the server to provide
* verbose output for the developer.  When the source is compiled with
* DEBUG_INFO defined, the string will be printed using 'fprintf( )' to standard
* error.
* When DEBUG_INFO is not defined (typically in the release version), the
* string will not be compiled (saving memory).
*
* RETURNS: N/A
*
*/

void httpLogInfoFunction
    (
    char * szMessage
    )
    {
    (void)fprintf (stderr,"INFO(): %s\n", szMessage);
    }


/*******************************************************************************
*
* httpLogInfoMsgFunction - Log an info message with a string param to stdout.
*
* This routine can be used when writing new code for the server to provide
* verbose output for the developer.  When the source is compiled with
* DEBUG_INFO defined, the string will be printed using 'fprintf( )' to standard
* error.
* When DEBUG_INFO is defined (typically in the release version), the
* string will not be compiled (saving memory).
*
* Currently this routine works like this:
* \cs
*     printf (szMessage, szArg);
* \ce
*
*
* RETURNS: N/A
*
*/

void httpLogInfoMsgFunction
    (
    char * szMessage, /* string that may contain a printf format specifier */
    ...               /* printf argument */
    )
    {
    va_list vaList;

    va_start (vaList, szMessage);
    (void)fprintf ( stderr,"INFO(): ");
    (void)vfprintf ( stderr, szMessage, vaList);
    (void)fprintf ( stderr,"\n" );

    va_end (vaList);
    }

/* WMB Error Message Structure */
typedef struct wmb_error_msg
    {
    sbyte4    errnoVal;
    char *    szErrMsg;
    } WMB_ERR_MSG;


/* WMB Error Message Table */
static WMB_ERR_MSG wmbErrorMsg[] =
{
    /* General purpose errors. */
    { ERROR_GENERAL_NO_DATA,               ErrMsg_ERROR_GENERAL_NO_DATA              },
    { ERROR_GENERAL_NOT_FOUND,             ErrMsg_ERROR_GENERAL_NOT_FOUND            },
    { ERROR_GENERAL_ACCESS_DENIED,         ErrMsg_ERROR_GENERAL_ACCESS_DENIED        },
    { ERROR_GENERAL_NOT_EQUAL,             ErrMsg_ERROR_GENERAL_NOT_EQUAL            },
    { ERROR_GENERAL_ILLEGAL_VALUE,         ErrMsg_ERROR_GENERAL_ILLEGAL_VALUE        },
    { ERROR_GENERAL_CREATE_TASK,           ErrMsg_ERROR_GENERAL_CREATE_TASK          },
    { ERROR_GENERAL_NULL_POINTER,          ErrMsg_ERROR_GENERAL_NULL_POINTER         },
    { ERROR_GENERAL_DATA_AMBIG,            ErrMsg_ERROR_GENERAL_DATA_AMBIG           },
    { ERROR_GENERAL_FILE_NOT_FOUND,        ErrMsg_ERROR_GENERAL_FILE_NOT_FOUND       },
    { ERROR_GENERAL_BUFFER_OVERRUN,        ErrMsg_ERROR_GENERAL_BUFFER_OVERRUN       },
    { ERROR_GENERAL_BAD_NAME,              ErrMsg_ERROR_GENERAL_BAD_NAME             },
    { ERROR_GENERAL_INVALID_RAPIDMARK,     ErrMsg_ERROR_GENERAL_INVALID_RAPIDMARK    },
    { ERROR_GENERAL_OUT_OF_RANGE,          ErrMsg_ERROR_GENERAL_OUT_OF_RANGE         },
    { ERROR_GENERAL_INVALID_PATH,          ErrMsg_ERROR_GENERAL_INVALID_PATH         },
    { ERROR_GENERAL_TIMEOUT,               ErrMsg_ERROR_GENERAL_TIMEOUT              },
    { ERROR_GENERAL_ABANDONED,             ErrMsg_ERROR_GENERAL_ABANDONED            },
    { ERROR_GENERAL_NO_NEXT,               ErrMsg_ERROR_GENERAL_NO_NEXT              },
    { ERROR_GENERAL_COMPONENT_NOT_FOUND,   ErrMsg_ERROR_GENERAL_COMPONENT_NOT_FOUND  },
    { ERROR_GENERAL_READ_ONLY,             ErrMsg_ERROR_GENERAL_READ_ONLY            },
    { ERROR_GENERAL_NOT_LOCAL,             ErrMsg_ERROR_GENERAL_NOT_LOCAL            },
    { ERROR_GENERAL_INVALID_USER,          ErrMsg_ERROR_GENERAL_INVALID_USER         },
    { ERROR_GENERAL_NO_ACCESS,             ErrMsg_ERROR_GENERAL_NO_ACCESS            },
    { ERROR_GENERAL_INCONSISTENT_VALUE,    ErrMsg_ERROR_GENERAL_INCONSISTENT_VALUE   },
    { ERROR_GENERAL_INCONSISTENT_NAME,     ErrMsg_ERROR_GENERAL_INCONSISTENT_NAME    },
    { ERROR_GENERAL_WRONG_VALUE,           ErrMsg_ERROR_GENERAL_WRONG_VALUE          },
    { ERROR_GENERAL_WRONG_VALUE,           ErrMsg_ERROR_GENERAL_WRONG_VALUE          },
    { ERROR_GENERAL_COMMIT_FAILED,         ErrMsg_ERROR_GENERAL_COMMIT_FAILED        },
    { ERROR_GENERAL_UNDO_FAILED,           ErrMsg_ERROR_GENERAL_UNDO_FAILED          },
    { ERROR_GENERAL_WRONG_LENGTH,          ErrMsg_ERROR_GENERAL_WRONG_LENGTH         },
    { ERROR_GENERAL_NO_SUCH_INSTANCE,      ErrMsg_ERROR_GENERAL_NO_SUCH_INSTANCE     },
    { ERROR_GENERAL_TOO_BIG,               ErrMsg_ERROR_GENERAL_TOO_BIG              },
    { ERROR_GENERAL_NO_SUCH_NAME,          ErrMsg_ERROR_GENERAL_NO_SUCH_NAME         },
    { ERROR_GENERAL_BAD_VALUE,             ErrMsg_ERROR_GENERAL_BAD_VALUE            },
    { ERROR_GENERAL_WRONG_TYPE,            ErrMsg_ERROR_GENERAL_WRONG_TYPE           },
    { ERROR_GENERAL_WRONG_ENCODING,        ErrMsg_ERROR_GENERAL_WRONG_ENCODING       },
    { ERROR_GENERAL_AUTHORIZATION_ERROR,   ErrMsg_ERROR_GENERAL_AUTHORIZATION_ERROR  },
    { ERROR_GENERAL_NOT_WRITABLE,          ErrMsg_ERROR_GENERAL_NOT_WRITABLE         },
    { ERROR_GENERAL_NONEXTINSTANCE,        ErrMsg_ERROR_GENERAL_NONEXTINSTANCE       },
    { ERROR_GENERAL_QUIT,                  ErrMsg_ERROR_GENERAL_QUIT                 },
    { ERROR_GENERAL_UNINITIALIZED,         ErrMsg_ERROR_GENERAL_UNINITIALIZED        },
    { ERROR_GENERAL_OVERWRITE,             ErrMsg_ERROR_GENERAL_OVERWRITE            },
    { ERROR_GENERAL_INVALID,               ErrMsg_ERROR_GENERAL_INVALID              },
    { ERROR_GENERAL_INVALID_TYPE,          ErrMsg_ERROR_GENERAL_INVALID_TYPE         },
    { ERROR_GENERAL_MAXED,                 ErrMsg_ERROR_GENERAL_MAXED                },
    { ERROR_GENERAL_NOT_HANDLED,           ErrMsg_ERROR_GENERAL_NOT_HANDLED          },
    { ERROR_GENERAL_NO_ERROR,              ErrMsg_ERROR_GENERAL_NO_ERROR             },

    /* Errors returned by the Post Handler */
    { ERROR_POST_NO_MORE_MAGICMARKUPS,     ErrMsg_ERROR_POST_NO_MORE_MAGICMARKUPS    },

    /* Errors returned by the Get Handler */
    { ERROR_TX_ENG_BAD_MAGICMARKUP,        ErrMsg_ERROR_TX_ENG_BAD_MAGICMARKUP       },

    /* Errors returned by the datatype conversion routines */
    { ERROR_CONVERSION_INCORRECT_TYPE,     ErrMsg_ERROR_CONVERSION_INCORRECT_TYPE    },
    { ERROR_CONVERSION_OVERFLOW,           ErrMsg_ERROR_CONVERSION_OVERFLOW          },
    { ERROR_CONVERSION_UNDERFLOW,          ErrMsg_ERROR_CONVERSION_UNDERFLOW         },
    { ERROR_CONVERSION_TOO_LONG,           ErrMsg_ERROR_CONVERSION_TOO_LONG          },

    /* Errors returned by the memory management system.*/
    { ERROR_MEMMGR_BAD_MEMSIZE,            ErrMsg_ERROR_MEMMGR_BAD_MEMSIZE           },
    { ERROR_MEMMGR_INITIALIZATION,         ErrMsg_ERROR_MEMMGR_INITIALIZATION        },
    { ERROR_MEMMGR_NO_MEMORY,              ErrMsg_ERROR_MEMMGR_NO_MEMORY             },
    { ERROR_MEMMGR_BAD_POINTER,            ErrMsg_ERROR_MEMMGR_BAD_POINTER           },
    { ERROR_MEMMGR_BAD_FREE,               ErrMsg_ERROR_MEMMGR_BAD_FREE              },
    { ERROR_MEMMGR_MEMORY_CORRUPTION,      ErrMsg_ERROR_MEMMGR_MEMORY_CORRUPTION     },
    { ERROR_MEMMGR_INVALID_LENGTH,         ErrMsg_ERROR_MEMMGR_INVALID_LENGTH        },

    /* Errors returned by the decompression system.*/
    { ERROR_DECOMP_BAD_PKZIP_FILE,         ErrMsg_ERROR_DECOMP_BAD_PKZIP_FILE        },
    { ERROR_DECOMP_BAD_FIRST_ENTRY,        ErrMsg_ERROR_DECOMP_BAD_FIRST_ENTRY       },
    { ERROR_DECOMP_GZIP_FILE_NOT_DEFLATED, ErrMsg_ERROR_DECOMP_GZIP_FILE_NOT_DEFLATED},
    { ERROR_DECOMP_MULTIPART_GZIP_FILES,   ErrMsg_ERROR_DECOMP_MULTIPART_GZIP_FILES  },
    { ERROR_DECOMP_INVALID_FILE_FORMAT,    ErrMsg_ERROR_DECOMP_INVALID_FILE_FORMAT   },
    { ERROR_DECOMP_FORMAT_VIOLATION,       ErrMsg_ERROR_DECOMP_FORMAT_VIOLATION      },
    { ERROR_DECOMP_LENGTH_MISMATCH,        ErrMsg_ERROR_DECOMP_LENGTH_MISMATCH       },
    { ERROR_DECOMP_CRC_MISMATCH,           ErrMsg_ERROR_DECOMP_CRC_MISMATCH          },
    { ERROR_DECOMP_DATA_LENGTH,            ErrMsg_ERROR_DECOMP_DATA_LENGTH           },

#ifdef WEBCLI_INCLUDE_SMTP
    /* Errors returned by the e-mail system.*/
    { ERROR_SMTP_NOT_INIT,                 ErrMsg_ERROR_DECOMP_DATA_LENGTH           },
    { ERROR_SMTP_ABORT,                    ErrMsg_ERROR_SMTP_ABORT                   },
#endif
    /* Errors returned by the pre-existing system.*/
   { SYS_ERROR_NO_MEMORY,                  ErrMsg_SYS_ERROR_NO_MEMORY                },
   { SYS_ERROR_UNEXPECTED_END,             ErrMsg_SYS_ERROR_UNEXPECTED_END           },
   { SYS_ERROR_MUTEX_CREATE,               ErrMsg_SYS_ERROR_MUTEX_CREATE             },
   { SYS_ERROR_MUTEX_WAIT,                 ErrMsg_SYS_ERROR_MUTEX_WAIT               },
   { SYS_ERROR_MUTEX_RELEASE,              ErrMsg_SYS_ERROR_MUTEX_RELEASE            },
   { SYS_ERROR_SOCKET_CREATE,              ErrMsg_SYS_ERROR_SOCKET_CREATE            },
   { SYS_ERROR_SOCKET_BIND,                ErrMsg_SYS_ERROR_SOCKET_BIND              },
   { SYS_ERROR_SOCKET_THREAD,              ErrMsg_SYS_ERROR_SOCKET_THREAD            },
   { SYS_ERROR_SOCKET_LISTEN,              ErrMsg_SYS_ERROR_SOCKET_LISTEN            },
   { SYS_ERROR_SOCKET_ACCEPT,              ErrMsg_SYS_ERROR_SOCKET_ACCEPT            },
   { SYS_ERROR_SOCKET_CREATE_TASK,         ErrMsg_SYS_ERROR_SOCKET_CREATE_TASK       },
   { SYS_ERROR_SOCKET_DELETE,              ErrMsg_SYS_ERROR_SOCKET_DELETE            },
   { SYS_ERROR_SOCKET_SHARE,               ErrMsg_SYS_ERROR_SOCKET_SHARE             },
   { SYS_ERROR_SOCKET_START,               ErrMsg_SYS_ERROR_SOCKET_START             },
   { SYS_ERROR_SOCKET_CONNECT,             ErrMsg_SYS_ERROR_SOCKET_CONNECT           },
   { SYS_ERROR_SOCKET_DISCONNECTED,        ErrMsg_SYS_ERROR_SOCKET_DISCONNECTED      },
   { SYS_ERROR_SOCKET_EWOULDBLOCK,         ErrMsg_SYS_ERROR_SOCKET_EWOULDBLOCK       },

#ifdef __RLI_SSL_ENABLED__

   { ERROR_SSL_NO_SLOTS,                   ErrMsg_ERROR_SSL_NO_SLOTS                 },
   { ERROR_SSL_CTX,                        ErrMsg_ERROR_SSL_CTX                      },
   { ERROR_SSL_SERVER_CERT,                ErrMsg_ERROR_SSL_SERVER_CERT              },
   { ERROR_SSL_SERVER_CA_PATH,             ErrMsg_ERROR_SSL_SERVER_CA_PATH           },

#endif

#ifndef WEBCLI_STANDALONE_WEBSERVER
    { ERROR_TM_TID_NOT_FOUND,              ErrMsg_ERROR_TM_TID_NOT_FOUND             },
    { ERROR_TM_NOT_INITIALIZED,            ErrMsg_ERROR_TM_NOT_INITIALIZED           },
    { ERROR_TM_BAD_STATE,                  ErrMsg_ERROR_TM_BAD_STATE                 },
    { ERROR_TM_BAD_INPUT,                  ErrMsg_ERROR_TM_BAD_INPUT                 },
    { ERROR_TM_HANDLER_NOT_FOUND,          ErrMsg_ERROR_TM_HANDLER_NOT_FOUND         },
    { ERROR_TM_BAD_CMD,                    ErrMsg_ERROR_TM_BAD_CMD                   },
    { ERROR_TM_NOT_HANDLED,                ErrMsg_ERROR_TM_NOT_HANDLED               },
    { ERROR_TM_INVALID_TRANSACTION,        ErrMsg_ERROR_TM_INVALID_TRANSACTION       },
    { ERROR_TM_INVALID_LOCAL_ID,           ErrMsg_ERROR_TM_INVALID_LOCAL_ID          },
    { ERROR_TM_DEADLOCKED,                 ErrMsg_ERROR_TM_DEADLOCKED                },

    /* Database errors */
    { ERROR_DB_DUPLICATE_ENTRY,            ErrMsg_ERROR_DB_DUPLICATE_ENTRY           },
    { ERROR_DB_NO_DATABASE,                ErrMsg_ERROR_DB_NO_DATABASE               },

    /* Errors returned by the Lock Management system. */
    { ERROR_LM_ENTRY_NOT_FOUND,            ErrMsg_ERROR_LM_ENTRY_NOT_FOUND           },
    { ERROR_LM_LOCK_NOT_FOUND,             ErrMsg_ERROR_LM_LOCK_NOT_FOUND            },
    { ERROR_LM_LOCK_NOT_GRANTED,           ErrMsg_ERROR_LM_LOCK_NOT_GRANTED          },
    { ERROR_LM_INTENT_MODE_FAILED,         ErrMsg_ERROR_LM_INTENT_MODE_FAILED        },
    { ERROR_LM_TABLE_NOT_FOUND,            ErrMsg_ERROR_LM_TABLE_NOT_FOUND           },

    /* Table errors */
    { ERROR_TABLE_END_OF_TABLE,            ErrMsg_ERROR_TABLE_END_OF_TABLE           },
    { ERROR_TABLE_INVALID_COLUMN,          ErrMsg_ERROR_TABLE_INVALID_COLUMN         },
    { ERROR_TABLE_COLUMN_BAD_SYNC,         ErrMsg_ERROR_TABLE_COLUMN_BAD_SYNC        },


   /* Gerneral backplane errors */
    { ERROR_WMB_BACKPLANE_INVALID,         ErrMsg_ERROR_WMB_BACKPLANE_INVALID         },
    { ERROR_WMB_SESSION_INVALID,           ErrMsg_ERROR_WMB_SESSION_INVALID           },
    { ERROR_WMB_OBJECT_INVALID,            ErrMsg_ERROR_WMB_OBJECT_INVALID            },
    { ERROR_WMB_COMPONENT_INVALID,         ErrMsg_ERROR_WMB_COMPONENT_INVALID         },
    { ERROR_GENERAL_SESSION_NOT_FOUND,     ErrMsg_ERROR_GENERAL_SESSION_NOT_FOUND     },
    { ERROR_GENERAL_OBJECT_NOT_FOUND,      ErrMsg_ERROR_GENERAL_OBJECT_NOT_FOUND      },
    { ERROR_GENERAL_RESOURCE_UNAVAILABLE,  ErrMsg_ERROR_GENERAL_RESOURCE_UNAVAILABLE  },
#endif
};


/**************************************************************************
*
* wmGetErrMsg - Get Error Message String wmb Error Message Table
*
* This function is used to retrieve error message string corresponding to
* Errno value
*
* RETURNS: Error Message String
*
*
* NOMANUAL
*/

char * wmGetErrMsg
    (
    sbyte4 errnoVal
    )
    {
    ubyte2 i;

    for (i = 0; i < (sizeof(wmbErrorMsg)/sizeof(WMB_ERR_MSG)); i++)
        {
        if (wmbErrorMsg[i].errnoVal == errnoVal)
            return wmbErrorMsg[i].szErrMsg;
        }

    return "WindMark Operation Failed";
    }


