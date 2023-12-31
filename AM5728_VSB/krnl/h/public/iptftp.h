/* iptftp.h - Public API of Wind River TFTP */

/*
 * Copyright (c) 2006-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
#ifndef IPTFTP_H
#define IPTFTP_H

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 * TFTP public header file.
 */

/*
DESCRIPTION
This library contains the APIs provided by the WindRiver TFTP.
*/

/*
 ****************************************************************************
 * 2                    CONFIGURATION
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 3                    INCLUDE FILES
 ****************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

/*
 *===========================================================================
 *                         release
 *===========================================================================
 */

#define IPTFTP_RELEASE 60500


/* TFTP Client error codes */
#define IPTFTP_SUCCESS                  0
#define IPTFTP_ERR_SERVER_ADDR          1
#define IPTFTP_ERR_MEMORY_FAIL          2
#define IPTFTP_ERR_FILE_OPEN            3
#define IPTFTP_ERR_FILE_READ            4
#define IPTFTP_ERR_FILE_WRITE           5
#define IPTFTP_ERR_TRANSFER_TIMEOUT     6
#define IPTFTP_ERR_SOCKET_FAIL          7
#define IPTFTP_ERR_ILLEGAL_MESSAGE      8
#define IPTFTP_ERR_ERROR_MESSAGE        9


/*
 ****************************************************************************
 * 5                    TYPES
 ****************************************************************************
 */


/*
 ****************************************************************************
 * 6                    FUNCTIONS
 ****************************************************************************
 */

/*******************************************************************************
*
* iptftp_client_put - transfer a file from the target to a TFTP server
*
* This routine transfers a file from the target to a TFTP server.
*
* Parameters:
* \is
* \i <server>
* A pointer to a string  with either the Internet address or the hostname of the
* TFTP server.
* \i <srcfile>
* A pointer to a string with the source-file name.
* \i <dstfile>
* A pointer to a string with the destination-file name.
* \i <binary>
* A boolean that specifies binary (octet) or 'netascii' mode.
* \i <verbose>
* A boolean that specifies whether the routine prints status information to the
* standard output.
* \i <port>
* An int that specifies server tftp port other than the default value. If 0 use default. 
* \ie
*
* RETURNS: 'IPTFTP_SUCCESS' or an error code.
*
* ERRNO:
*/
IP_PUBLIC Ip_err iptftp_client_put
(
    const char *server,
    const char *srcfile,
    const char *dstfile,
    Ip_bool binary,
    Ip_bool verbose,
    int port
);


/*******************************************************************************
*
* iptftp_client_get - transfer a file to the target from a TFTP server
*
* This routine transfers a file to the target from a TFTP server.
*
* Parameters:
* \is
* \i <server>
* A pointer to a string containing either the Internet address or the hostname
* of the TFTP server.
* \i <srcfile>
* A pointer to a string with the source-file name.
* \i <dstfile>
* A pointer to a string with the destination-file name.
* \i <binary>
* A boolean that specifies binary (octet) or 'netascii' mode.
* \i <verbose>
* A boolean that specifies whether the routine prints status information to the
* standard output.
* \i <port>
* An int that specifies server tftp port other than the default value. If 0 use default. 
* \ie
*
* RETURNS: 'IPTFTP_SUCCESS' or an error code.
*
* ERRNO:
*/
IP_PUBLIC Ip_err iptftp_client_get
(
    const char *server,
    const char *srcfile,
    const char *dstfile,
    Ip_bool binary,
    Ip_bool verbose,
    int port
);


#ifdef __cplusplus
}
#endif

#endif

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
