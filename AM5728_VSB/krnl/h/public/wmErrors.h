/* wmErrors.h - error codes for WindManage */

/* Copyright (c) 2004, 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01g,18sep12,r_w  modify error msg for requirement WIND00347266
01f,01nov11,r_w  HTTP header timeout support(WIND00307889)
01e,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01d,12may04,vmk  added error message strings
01c,23mar04,vmk  added error code SYS_ERROR_SOCKET_EWOULDBLOCK.
01b,01mar04,dkg  added error code ERROR_GENERAL_NO_ERROR
01a,20feb04,jws  added copyright and mod history
*/


#ifndef __INCwmErrorsh
#define __INCwmErrorsh

/* General purpose errors. */

#define ERROR_GENERAL                           -100
#define ERROR_GENERAL_NO_DATA                   ( ERROR_GENERAL - 1  )
#define ERROR_GENERAL_NOT_FOUND                 ( ERROR_GENERAL - 2  )
#define ERROR_GENERAL_ACCESS_DENIED             ( ERROR_GENERAL - 3  )
#define ERROR_GENERAL_NOT_EQUAL                 ( ERROR_GENERAL - 4  )
#define ERROR_GENERAL_ILLEGAL_VALUE             ( ERROR_GENERAL - 5  )
#define ERROR_GENERAL_CREATE_TASK            	( ERROR_GENERAL - 6  )
#define ERROR_GENERAL_NULL_POINTER            	( ERROR_GENERAL - 7  )
#define ERROR_GENERAL_DATA_AMBIG                ( ERROR_GENERAL - 8  )
#define ERROR_GENERAL_FILE_NOT_FOUND            ( ERROR_GENERAL - 9  )
#define ERROR_GENERAL_BUFFER_OVERRUN            ( ERROR_GENERAL - 10 )
#define ERROR_GENERAL_BAD_NAME                  ( ERROR_GENERAL - 11 )
#define ERROR_GENERAL_INVALID_RAPIDMARK         ERROR_GENERAL_BAD_NAME
#define ERROR_GENERAL_OUT_OF_RANGE              ( ERROR_GENERAL - 12 )
#define ERROR_GENERAL_INVALID_PATH              ( ERROR_GENERAL - 13 )
#define ERROR_GENERAL_TIMEOUT                   ( ERROR_GENERAL - 14 )
#define ERROR_GENERAL_ABANDONED                 ( ERROR_GENERAL - 15 )
#define ERROR_GENERAL_NO_NEXT                   ( ERROR_GENERAL - 16 )
#define ERROR_GENERAL_COMPONENT_NOT_FOUND       ( ERROR_GENERAL - 20 )
#define ERROR_GENERAL_READ_ONLY                 ( ERROR_GENERAL - 21 )
#define ERROR_GENERAL_NOT_LOCAL                 ( ERROR_GENERAL - 22 )
#define ERROR_GENERAL_INVALID_USER              ( ERROR_GENERAL - 23 )
#define ERROR_GENERAL_NO_ACCESS                 ERROR_GENERAL_ACCESS_DENIED
#define ERROR_GENERAL_INCONSISTENT_VALUE        ERROR_GENERAL_OUT_OF_RANGE
#define ERROR_GENERAL_INCONSISTENT_NAME         ERROR_GENERAL_BAD_NAME
#define ERROR_GENERAL_WRONG_VALUE               ERROR_GENERAL_ILLEGAL_VALUE
#define ERROR_GENERAL_NO_CREATION               ( ERROR_GENERAL - 24 )
#define ERROR_GENERAL_COMMIT_FAILED             ( ERROR_GENERAL - 25 )
#define ERROR_GENERAL_UNDO_FAILED               ( ERROR_GENERAL - 26 )
#define ERROR_GENERAL_WRONG_LENGTH              ( ERROR_GENERAL - 28 )
#define ERROR_GENERAL_NO_SUCH_INSTANCE          ( ERROR_GENERAL - 29 )
#define	ERROR_GENERAL_TOO_BIG                   ERROR_GENERAL_BUFFER_OVERRUN
#define	ERROR_GENERAL_NO_SUCH_NAME              ( ERROR_GENERAL - 31 )
#define	ERROR_GENERAL_BAD_VALUE	                ( ERROR_GENERAL - 32 )
#define ERROR_GENERAL_WRONG_TYPE	            ( ERROR_GENERAL - 33 )
#define ERROR_GENERAL_WRONG_ENCODING            ( ERROR_GENERAL - 34 )
#define ERROR_GENERAL_AUTHORIZATION_ERROR       ( ERROR_GENERAL - 35 )
#define ERROR_GENERAL_NOT_WRITABLE              ( ERROR_GENERAL - 36 )
#define ERROR_GENERAL_NONEXTINSTANCE            ( ERROR_GENERAL - 37 )
#define ERROR_GENERAL_QUIT                      ( ERROR_GENERAL - 38 )
#define ERROR_GENERAL_UNINITIALIZED             ( ERROR_GENERAL - 39 )
#define ERROR_GENERAL_OVERWRITE                 ( ERROR_GENERAL - 40 )
#define ERROR_GENERAL_INVALID                   ( ERROR_GENERAL - 41 )
#define ERROR_GENERAL_INVALID_TYPE              ( ERROR_GENERAL - 42 )
#define ERROR_GENERAL_MAXED                     ( ERROR_GENERAL - 43 )
#define ERROR_GENERAL_NOT_HANDLED               ( ERROR_GENERAL - 44 )
#define ERROR_HTTP_HEADER_TIMEOUT               ( ERROR_GENERAL - 45 )
#define ERROR_GENERAL_NO_ERROR                  ( ERROR_GENERAL - 46 )

/* Errors returned by the Post Handler */

#define ERROR_POST_GENERAL                      -200
#define ERROR_POST_NO_MORE_MAGICMARKUPS         ( ERROR_POST_GENERAL - 1 )

/* Errors returned by the Get Handler */

#define ERROR_GET_GENERAL                       -300
#define ERROR_TX_ENG_BAD_MAGICMARKUP            ( ERROR_GET_GENERAL - 1 )

/* Errors returned by the datatype conversion routines */

#define ERROR_CONVERSION_GENERAL                -400
#define ERROR_CONVERSION_INCORRECT_TYPE         ( ERROR_CONVERSION_GENERAL - 1 )
#define ERROR_CONVERSION_OVERFLOW               ( ERROR_CONVERSION_GENERAL - 2 )
#define ERROR_CONVERSION_UNDERFLOW              ( ERROR_CONVERSION_GENERAL - 3 )
#define ERROR_CONVERSION_TOO_LONG               ( ERROR_CONVERSION_GENERAL - 4 )

/* Errors returned by the memory management system.*/

#define ERROR_MEMMGR_GENERAL                    -500
#define ERROR_MEMMGR_BAD_MEMSIZE                ( ERROR_MEMMGR_GENERAL - 1 )
#define ERROR_MEMMGR_INITIALIZATION             ( ERROR_MEMMGR_GENERAL - 2 )
#define ERROR_MEMMGR_NO_MEMORY                  ( ERROR_MEMMGR_GENERAL - 3 )
#define ERROR_MEMMGR_BAD_POINTER                ( ERROR_MEMMGR_GENERAL - 4 )
#define ERROR_MEMMGR_BAD_FREE                   ( ERROR_MEMMGR_GENERAL - 5 )
#define ERROR_MEMMGR_MEMORY_CORRUPTION          ( ERROR_MEMMGR_GENERAL - 6 )
#define ERROR_MEMMGR_INVALID_LENGTH             ( ERROR_MEMMGR_GENERAL - 7 )

/* Errors returned by the decompression system.*/

#define ERROR_DECOMP_GENERAL                    -600
#define ERROR_DECOMP_BAD_PKZIP_FILE             ( ERROR_DECOMP_GENERAL - 1 )
#define ERROR_DECOMP_BAD_FIRST_ENTRY            ( ERROR_DECOMP_GENERAL - 2 )
#define ERROR_DECOMP_GZIP_FILE_NOT_DEFLATED     ( ERROR_DECOMP_GENERAL - 3 )
#define ERROR_DECOMP_MULTIPART_GZIP_FILES       ( ERROR_DECOMP_GENERAL - 4 )
#define ERROR_DECOMP_INVALID_FILE_FORMAT        ( ERROR_DECOMP_GENERAL - 5 )
#define ERROR_DECOMP_FORMAT_VIOLATION           ( ERROR_DECOMP_GENERAL - 6 )
#define ERROR_DECOMP_LENGTH_MISMATCH            ( ERROR_DECOMP_GENERAL - 7 )
#define ERROR_DECOMP_CRC_MISMATCH               ( ERROR_DECOMP_GENERAL - 8 )
#define ERROR_DECOMP_DATA_LENGTH                ( ERROR_DECOMP_GENERAL - 9 )

/* Errors returned by the e-mail system.*/

#define ERROR_SMTP_GENERAL                      -700
#define ERROR_SMTP_NOT_INIT                     ( ERROR_SMTP_GENERAL - 1 )
#define ERROR_SMTP_ABORT                        ( ERROR_SMTP_GENERAL - 2 )

/* Errors returned by the pre-existing system.*/

#define SYS_ERROR_GENERAL                       -1000
#define SYS_ERROR_NO_MEMORY                     ( SYS_ERROR_GENERAL - 1 )
#define SYS_ERROR_UNEXPECTED_END                ( SYS_ERROR_GENERAL - 2 )

#define SYS_ERROR_MUTEX_GENERAL                 -1100
#define SYS_ERROR_MUTEX_CREATE                  ( SYS_ERROR_MUTEX_GENERAL - 1 )
#define SYS_ERROR_MUTEX_WAIT                    ( SYS_ERROR_MUTEX_GENERAL - 2 )
#define SYS_ERROR_MUTEX_RELEASE                 ( SYS_ERROR_MUTEX_GENERAL - 3 )

#define SYS_ERROR_SOCKET_GENERAL                -1200
#define SYS_ERROR_SOCKET_CREATE                 ( SYS_ERROR_SOCKET_GENERAL - 1 )
#define SYS_ERROR_SOCKET_BIND                   ( SYS_ERROR_SOCKET_GENERAL - 2 )
#define SYS_ERROR_SOCKET_THREAD                 ( SYS_ERROR_SOCKET_GENERAL - 3 )
#define SYS_ERROR_SOCKET_LISTEN                 ( SYS_ERROR_SOCKET_GENERAL - 4 )
#define SYS_ERROR_SOCKET_ACCEPT                 ( SYS_ERROR_SOCKET_GENERAL - 5 )
#define SYS_ERROR_SOCKET_CREATE_TASK            ( SYS_ERROR_SOCKET_GENERAL - 6 )
#define SYS_ERROR_SOCKET_DELETE                 ( SYS_ERROR_SOCKET_GENERAL - 7 )
#define SYS_ERROR_SOCKET_SHARE                  ( SYS_ERROR_SOCKET_GENERAL - 8 )
#define SYS_ERROR_SOCKET_START                  ( SYS_ERROR_SOCKET_GENERAL - 9 )
#define SYS_ERROR_SOCKET_CONNECT                ( SYS_ERROR_SOCKET_GENERAL - 10 )
#define SYS_ERROR_SOCKET_DISCONNECTED           ( SYS_ERROR_SOCKET_GENERAL - 12 )
#define SYS_ERROR_SOCKET_EWOULDBLOCK            ( SYS_ERROR_SOCKET_GENERAL - 13 )

#ifdef __RLI_SSL_ENABLED__

/* errors returned by SSL */

#define ERROR_SSL_GENERAL                       -1300
#define ERROR_SSL_NO_SLOTS                      ( ERROR_SSL_GENERAL - 1 )
#define ERROR_SSL_CTX                           ( ERROR_SSL_GENERAL - 2 )
#define ERROR_SSL_SERVER_CERT                   ( ERROR_SSL_GENERAL - 3 )
#define ERROR_SSL_SERVER_CA_PATH                ( ERROR_SSL_GENERAL - 4 )

#endif

/* Error Message Strings */

/* General purpose errors. */
#define ErrMsg_ERROR_GENERAL_NO_DATA               "Data not available" 
#define ErrMsg_ERROR_GENERAL_NOT_FOUND             "Not Found"
#define ErrMsg_ERROR_GENERAL_ACCESS_DENIED         "Access Denied"
#define ErrMsg_ERROR_GENERAL_NOT_EQUAL             "Not Equal"
#define ErrMsg_ERROR_GENERAL_ILLEGAL_VALUE         "Illegal Value"
#define ErrMsg_ERROR_GENERAL_CREATE_TASK           "Task Create Error"
#define ErrMsg_ERROR_GENERAL_NULL_POINTER          "Null Pointer"
#define ErrMsg_ERROR_GENERAL_DATA_AMBIG            "Ambiguous data"
#define ErrMsg_ERROR_GENERAL_FILE_NOT_FOUND        "File Not Found"
#define ErrMsg_ERROR_GENERAL_BUFFER_OVERRUN        "Buffer Overrun"
#define ErrMsg_ERROR_GENERAL_BAD_NAME              "Bad Name"
#define ErrMsg_ERROR_GENERAL_INVALID_RAPIDMARK     "Invalid WindMark"
#define ErrMsg_ERROR_GENERAL_OUT_OF_RANGE          "Out Of Range"
#define ErrMsg_ERROR_GENERAL_INVALID_PATH          "Invalid Path"
#define ErrMsg_ERROR_GENERAL_TIMEOUT               "TimeOut"
#define ErrMsg_ERROR_GENERAL_ABANDONED             "Abondoned"
#define ErrMsg_ERROR_GENERAL_NO_NEXT               "No Next"
#define ErrMsg_ERROR_GENERAL_COMPONENT_NOT_FOUND   "Component Not Found"
#define ErrMsg_ERROR_GENERAL_READ_ONLY             "Read Only"
#define ErrMsg_ERROR_GENERAL_NOT_LOCAL             "Not Local"
#define ErrMsg_ERROR_GENERAL_INVALID_USER          "Invalid User"
#define ErrMsg_ERROR_GENERAL_NO_ACCESS             "No access"
#define ErrMsg_ERROR_GENERAL_INCONSISTENT_VALUE    "Inconsistent Value"
#define ErrMsg_ERROR_GENERAL_INCONSISTENT_NAME     "Inconsistent Name"
#define ErrMsg_ERROR_GENERAL_WRONG_VALUE           "Wrong Value"
#define ErrMsg_ERROR_GENERAL_NO_CREATION           "No Creation"
#define ErrMsg_ERROR_GENERAL_COMMIT_FAILED         "Commit Failed"
#define ErrMsg_ERROR_GENERAL_UNDO_FAILED           "Undo Failed"
#define ErrMsg_ERROR_GENERAL_WRONG_LENGTH          "Wrong Length"
#define ErrMsg_ERROR_GENERAL_NO_SUCH_INSTANCE      "No Instance"
#define ErrMsg_ERROR_GENERAL_TOO_BIG               "Too Big"
#define ErrMsg_ERROR_GENERAL_NO_SUCH_NAME          "No Such Name"
#define ErrMsg_ERROR_GENERAL_BAD_VALUE             "Bad Value"
#define ErrMsg_ERROR_GENERAL_WRONG_TYPE            "Wrong Type"
#define ErrMsg_ERROR_GENERAL_WRONG_ENCODING        "Wrong Encoding"
#define ErrMsg_ERROR_GENERAL_AUTHORIZATION_ERROR   "Authorisation Error"
#define ErrMsg_ERROR_GENERAL_NOT_WRITABLE          "Not Writable"
#define ErrMsg_ERROR_GENERAL_NONEXTINSTANCE        "Not Existence"
#define ErrMsg_ERROR_GENERAL_QUIT                  "Quit"
#define ErrMsg_ERROR_GENERAL_UNINITIALIZED         "Uninitialised"
#define ErrMsg_ERROR_GENERAL_OVERWRITE             "Overwrite"
#define ErrMsg_ERROR_GENERAL_INVALID               "Invalid"
#define ErrMsg_ERROR_GENERAL_INVALID_TYPE          "Invalid Type"
#define ErrMsg_ERROR_GENERAL_MAXED                 "Maxed"
#define ErrMsg_ERROR_GENERAL_NOT_HANDLED           "Not Handled"
#define ErrMsg_ERROR_GENERAL_NO_ERROR              "No Error"
#define ErrMsg_ERROR_HTTP_HEADER_TIMEOUT           "Http Header Timeout"

 /* Errors returned by the Post Handler */
#define ErrMsg_ERROR_POST_NO_MORE_MAGICMARKUPS     "No More MagicMarkups"

 /* Errors returned by the Get Handler */
#define ErrMsg_ERROR_TX_ENG_BAD_MAGICMARKUP        "Tx Engine Bad MagicMarkup"

 /* Errors returned by the datatype conversion routines */
#define ErrMsg_ERROR_CONVERSION_INCORRECT_TYPE     "Conversion Incorrect Type"
#define ErrMsg_ERROR_CONVERSION_OVERFLOW           "Conversion Overflow"
#define ErrMsg_ERROR_CONVERSION_UNDERFLOW          "Conversion Underflow"
#define ErrMsg_ERROR_CONVERSION_TOO_LONG           "Conversion Too Long"

 /* Errors returned by the memory management system.*/
#define ErrMsg_ERROR_MEMMGR_BAD_MEMSIZE            "Bad Memory Size"
#define ErrMsg_ERROR_MEMMGR_INITIALIZATION         "Memory Manager Initialisation error"
#define ErrMsg_ERROR_MEMMGR_NO_MEMORY              "Memory Manager No Memory"
#define ErrMsg_ERROR_MEMMGR_BAD_POINTER            "Memory Manager bad Pointer"
#define ErrMsg_ERROR_MEMMGR_BAD_FREE               "Memory Manager Bad Free"
#define ErrMsg_ERROR_MEMMGR_MEMORY_CORRUPTION      "Memory Manager Memory Corruption"
#define ErrMsg_ERROR_MEMMGR_INVALID_LENGTH         "Memory Manager Invalid Length"

 /* Errors returned by the decompression system.*/
#define ErrMsg_ERROR_DECOMP_BAD_PKZIP_FILE         "Decomp Bad pkzip file"
#define ErrMsg_ERROR_DECOMP_BAD_FIRST_ENTRY        "Decomp Bad First Entry"
#define ErrMsg_ERROR_DECOMP_GZIP_FILE_NOT_DEFLATED "Decomp Gzip File Not Deflated"
#define ErrMsg_ERROR_DECOMP_MULTIPART_GZIP_FILES   "Decomp MultiPart Gzip Files"
#define ErrMsg_ERROR_DECOMP_INVALID_FILE_FORMAT    "Decomp Invalid File Format"
#define ErrMsg_ERROR_DECOMP_FORMAT_VIOLATION       "Decomp Format Violation"
#define ErrMsg_ERROR_DECOMP_LENGTH_MISMATCH        "Decomp Length Mismatch"
#define ErrMsg_ERROR_DECOMP_CRC_MISMATCH           "Decomp Crc Mismatch"
#define ErrMsg_ERROR_DECOMP_DATA_LENGTH            "Decomp Data Length"

 /* Errors returned by the e-mail system.*/
#define ErrMsg_ERROR_SMTP_NOT_INIT                 "SMTP Not Init"
#define ErrMsg_ERROR_SMTP_ABORT                    "SMTP Abort"

/* Errors returned by the pre-existing system.*/
#define ErrMsg_SYS_ERROR_NO_MEMORY                  "System Error: No Memory"
#define ErrMsg_SYS_ERROR_UNEXPECTED_END             "System Error: Unexpected End"
#define ErrMsg_SYS_ERROR_MUTEX_CREATE               "System Error: Mutex Create"
#define ErrMsg_SYS_ERROR_MUTEX_WAIT                 "System Error: Mutex Wait"
#define ErrMsg_SYS_ERROR_MUTEX_RELEASE              "System Error: Mutex Release"
#define ErrMsg_SYS_ERROR_SOCKET_CREATE              "System Error: Socket Create"
#define ErrMsg_SYS_ERROR_SOCKET_BIND                "System Error: Socket Bind"
#define ErrMsg_SYS_ERROR_SOCKET_THREAD              "System Error: Socket Thread"
#define ErrMsg_SYS_ERROR_SOCKET_LISTEN              "System Error: Socket Listen"
#define ErrMsg_SYS_ERROR_SOCKET_ACCEPT              "System Error: Socket Accept"
#define ErrMsg_SYS_ERROR_SOCKET_CREATE_TASK         "System Error: Socket Create Task"
#define ErrMsg_SYS_ERROR_SOCKET_DELETE              "System Error: Socket delete"
#define ErrMsg_SYS_ERROR_SOCKET_SHARE               "System Error: Socket Share"
#define ErrMsg_SYS_ERROR_SOCKET_START               "System Error: Socket Start"
#define ErrMsg_SYS_ERROR_SOCKET_CONNECT             "System Error: Socket Connect"
#define ErrMsg_SYS_ERROR_SOCKET_DISCONNECTED        "System Error: Socket Disconnected"
#define ErrMsg_SYS_ERROR_SOCKET_EWOULDBLOCK         "System Error: Socket EWOULD Block"

#ifdef __RLI_SSL_ENABLED__

#define ErrMsg_ERROR_SSL_NO_SLOTS                   "SSL No Slots"
#define ErrMsg_ERROR_SSL_CTX                        "SSL CTX"
#define ErrMsg_ERROR_SSL_SERVER_CERT                "SSL Server certification"
#define ErrMsg_ERROR_SSL_SERVER_CA_PATH             "SSL Server CA Path"

#endif

#endif /* ! __INCwmErrorsh */
