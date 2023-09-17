/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/decorum.h,v 1.3 2003/01/15 14:00:26 josh Exp $ */

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
 * Revision 1.3  2003/01/15 14:00:26  josh
 * directory structure shifting
 *
 * Revision 1.2  2002/02/26 18:22:53  josh
 * updating paths in port files
 *
 * Revision 1.1.1.1  2001/11/05 17:47:12  tneale
 * Tornado shuffle
 *
 * Revision 2.7  2001/01/19 22:23:03  paul
 * Update copyright.
 *
 * Revision 2.6  2000/03/17 00:10:27  meister
 * Update copyright message
 *
 * Revision 2.5  1998/03/12 20:59:46  josh
 * added string.h to prevent some compiler warnings
 *
 * Revision 2.4  1998/02/25 04:56:57  sra
 * Update copyrights.
 *
 * Revision 2.3  1998/02/16 21:33:43  josh
 * decorum port file now uses epilogue_char_t where appropriate
 *
 * Revision 2.2  1997/11/04 00:23:34  sar
 * added memory.h for some compilers to get rid of some warnings.
 *
 * Revision 2.1  1997/09/15 22:32:59  josh
 * include stdlib.h so we know how malloc() and free() are defined
 *
 * Revision 2.0  1997/09/11 15:08:17  josh
 * updating version numbers for release
 *
 * Revision 1.0  1997/08/08 22:11:19  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.8  1997/08/05 19:23:36  josh
 * updated for Decorum 2.0 -- new stuff added
 *
 * Revision 0.7  1997/03/20 06:57:48  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.6  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.5  1997/02/12 19:17:12  lowell
 * refer to DECORUM_GET_RAWTIME
 *
 * Revision 0.4  1996/10/28 17:49:00  sar
 * Added use of decorum_size_t
 *
 * Revision 0.3  1996/10/27  16:54:09  lowell
 * ETC_STDIO_SIZE
 *
 * Revision 0.2  1996/10/26  23:03:18  lowell
 * DECORUM_SENDBUF_LEN and ETC_STDIO_READ
 *
 * Revision 0.1  1996/10/23  00:12:55  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



#if (!defined(decorum_inc))
#define decorum_inc

/* zortech, watcom and intel 960 don't have a memory.h so away it goes */
#if (!defined(INSTALL_on_ztc) && !defined(INSTALL_on_watcom) && !defined(INSTALL_on_i960))
#include <memory.h>
#endif /* INSTALL_on_ztc */

#include <wrn/wm/common/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

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
#define ETC_STDIO_REMOVE(fname) remove(fname)
#define ETC_STDIO_READ(instream, size, buf)                             \
              fread((buf), 1, (size), (instream))
#define ETC_STDIO_WRITE(outstream, size, buf)				\
              fwrite((buf), 1, (size), (outstream))
extern bits32_t file_size_32(char * fname);
#define ETC_STDIO_SIZE(fname) (file_size_32(fname))
#if 0
extern void file_last_modified_time(char *, struct decorum_rawtime *);
#define ETC_STDIO_LAST_MODIFIED_TIME(fname, tvp) \
  (file_last_modified_time((fname), (tvp)))
#endif

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
#define ETC_STRING_STRCASECMP stricmp

/* Define a string length routine */
#define ETC_STRING_STRLEN strlen

/* Define a routine that gets the current time. */
/* This is compiler dependent on MS-DOS. */
#if 0
#define DECORUM_GET_RAWTIME(timeval_p) msdos_glue_getime ((timeval_p));
#endif
/* Define a routine that figgers out how big a buffer to use for a chunk
 * of outgoing data. */
#define DECORUM_SENDBUF_LEN(http_sb) (256)

#endif /* #if (!defined(decorum_inc)) */
