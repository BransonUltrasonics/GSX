/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/sunos/decorum.h,v 1.3 2003/01/15 14:00:28 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1996-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: decorum.h,v $
 * Revision 1.3  2003/01/15 14:00:28  josh
 * directory structure shifting
 *
 * Revision 1.2  2002/02/26 18:22:55  josh
 * updating paths in port files
 *
 * Revision 1.1.1.1  2001/11/05 17:47:16  tneale
 * Tornado shuffle
 *
 * Revision 2.5  2001/01/19 22:23:17  paul
 * Update copyright.
 *
 * Revision 2.4  2000/03/17 00:11:00  meister
 * Update copyright message
 *
 * Revision 2.3  1998/02/25 04:57:08  sra
 * Update copyrights.
 *
 * Revision 2.2  1998/02/16 21:33:44  josh
 * decorum port file now uses epilogue_char_t where appropriate
 *
 * Revision 2.1  1997/09/17 18:37:41  josh
 * include malloc.h so malloc() and free() will be defined
 *
 * Revision 2.0  1997/09/11 15:08:18  josh
 * updating version numbers for release
 *
 * Revision 1.2  1997/09/08 14:55:05  josh
 * a few constants that were left out...
 *
 * Revision 1.1  1997/08/11 18:50:28  josh
 * added ETC_STDIO_WRITE
 *
 * Revision 1.0  1997/08/08 22:11:22  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.9  1997/07/18 20:56:41  josh
 * modifications for the recently adapted HTTPD.  Nothing big,
 * just added a couple of macros and changed <strings.h> to
 * <string.h>
 *
 * Revision 0.8  1997/05/17 23:08:52  sra
 * Clean up some SunOS/gcc warnings.
 *
 * Revision 0.7  1997/03/20 06:57:52  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.6  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.5  1996/10/29 23:02:34  sar
 * added some includes to keep compilers happy
 *
 * Revision 0.4  1996/10/28  17:49:14  sar
 * added use of decorum_size_t
 *
 * Revision 0.3  1996/10/27  16:54:09  lowell
 * ETC_STDIO_SIZE
 *
 * Revision 0.2  1996/10/26  23:22:27  lowell
 * DECORUM_SENDBUF_LEN and ETC_STDIO_READ
 *
 * Revision 0.1  1996/10/23  00:14:56  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



#if (!defined(decorum_inc))
#define decorum_inc

#include <wrn/wm/common/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#ifndef DECORUM_ALLOC
#define	DECORUM_ALLOC(x) ((void *) malloc(x))
#endif
#ifndef DECORUM_FREE
#define DECORUM_FREE(x) (free((char *) (x)))
#endif

/* This first should be the same as SNMP_MAX_PACKET_SIZE in envoy.h */
#define DECORUM_MAX_PACKET_SIZE		(1400)
#define DECORUM_DEFAULT_RECEIVE_BUFLEN	(512)

/* needed for OIDC_T definition */
#if INSTALL_DECORUM_PROXY && (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

/* Some input routines, these are provided but may be replaced with
   more efficient implentations if desired */
extern sbits16_t decorum_parse_sbits32   (epilogue_char_t *, 
					  epilogue_char_t **,
					  sbits32_t *);
extern sbits16_t decorum_parse_bits32    (epilogue_char_t *, 
					  epilogue_char_t **,
					  bits32_t  *);
extern sbits16_t decorum_parse_hexstring (epilogue_char_t *, 
					  epilogue_char_t *,
					  decorum_size_t);
#define ETC_STRING_GET_SBITS32   decorum_parse_sbits32
#define ETC_STRING_GET_BITS32    decorum_parse_bits32
#define ETC_STRING_GET_HEXSTRING decorum_parse_hexstring

/* a routine to output a number to a byte string, used to create
   file/stream names */
#define ETC_STDIO_SPRINTF(buf, num, str) sprintf(buf, "%lu%s", num, str)

/* Define the structure that will be used for holding output
   until we are done */
typedef FILE etc_stdio_t;

/* Macros for opening, closing and removing a stream */
#define ETC_STDIO_MODE_READ    "r"
#define ETC_STDIO_MODE_WRITE   "w+"

#define ETC_STDIO_OPEN(fname, how) fopen(fname, how)
#define ETC_STDIO_CLOSE(outstream) fclose(outstream)
#define ETC_STDIO_REMOVE(fname) unlink(fname)
#define ETC_STDIO_READ(instream, size, buf)				\
              fread((buf), 1, (size), (instream))
#define ETC_STDIO_WRITE(outstream, size, buf)				\
              fwrite((buf), 1, (size), (outstream))
extern bits32_t file_size_32(char * fname);
#define ETC_STDIO_SIZE(fname) (file_size_32(fname))

/* Define the base output routines */
extern sbits16_t decorum_put_string     (etc_stdio_t *, epilogue_char_t *,
					 decorum_size_t);
extern sbits16_t decorum_put_sbits32    (etc_stdio_t *, sbits32_t);
extern sbits16_t decorum_put_bits32     (etc_stdio_t *, bits32_t);
extern sbits16_t decorum_put_hex_string (etc_stdio_t *, bits8_t *,
					 decorum_size_t);
#define ETC_STDIO_PUT_STRING     decorum_put_string
#define ETC_STDIO_PUT_SBITS32    decorum_put_sbits32
#define ETC_STDIO_PUT_BITS32     decorum_put_bits32
#define ETC_STDIO_PUT_HEX_STRING decorum_put_hex_string

#if INSTALL_DECORUM_PROXY
/* Define a more complex output routine, this may use the
   base routines (as the example code does) or may be more direct */
extern sbits16_t decorum_put_oid    (etc_stdio_t *, OIDC_T *, decorum_size_t);
#define ETC_STDIO_PUT_OID           decorum_put_oid
#endif /* INSTALL_DECORUM_PROXY */

/* Define a string comparison routine */
#define ETC_STRING_STRCASECMP strcasecmp
extern int strcasecmp();

/* Define a string length routine */
#define ETC_STRING_STRLEN strlen

/* Define a routine that gets the current time. */
#define DECORUM_GET_RAWTIME(timeval_p) unix_glue_getime ((timeval_p));
/* Define a routine that figgers out how big a buffer to use for a chunk 
 * of outgoing data. */
#define DECORUM_SENDBUF_LEN(http_sb) (512)

#endif /* #if (!defined(decorum_inc)) */
