/* $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbglue.h,v 1.2 2003/01/15 14:00:24 josh Exp $
 * Byte manipulation glue macros
 */

/****************************************************************************
 *
 *  *** Restricted Rights Legend ***
 *
 *  The programs and information contained herein are licensed only
 *  pursuant to a license agreement that contains use, reverse
 *  engineering, disclosure, and other restrictions; accordingly, it
 *  is "Unpublished--all rights reserved under the applicable
 *  copyright laws".
 *
 *  Use duplication, or disclosure by the Government is subject to
 *  restrictions as set forth in subparagraph (c)(1)(ii) of the Rights
 *  in Technical Data and Computer Licensed Programs clause of DFARS
 *  52.227 7013.
 *
 *  Copyright 2000-2001 Wind River Systems, Inc.
 *  Copyright 1995-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 *
 *  *** Government Use ***
 *
 *  The Licensed Programs and their documentation were developed at
 *  private expense and no part of them is in the public domain.
 *
 *  The Licensed Programs are "Restricted Computer Software" as that
 *  term is defined in Clause 52.227-19 of the Federal Acquisition
 *  Regulations (FAR) and are "Commercial Computer Software" as that
 *  term is defined in Subpart 227.401 of the Department of Defense
 *  Federal Acquisition Regulation Supplement (DFARS).
 *
 *  (i) If the licensed Programs are supplied to the Department of
 *      Defense (DoD), the Licensed Programs are classified as
 *      "Commercial Computer Software" and the Government is acquiring
 *      only "restricted rights" in the Licensed Programs and their
 *      documentation as that term is defined in Clause 52.227
 *      7013(c)(1) of the DFARS, and
 *
 *  (ii) If the Licensed Programs are supplied to any unit or agency
 *      of the United States Government other than DoD, the
 *      Government's rights in the Licensed Programs and their
 *      documentation will be as defined in Clause 52.227-19(c)(2) of
 *      the FAR.
 ****************************************************************************/

/*
 * $Log: ldbglue.h,v $
 * Revision 1.2  2003/01/15 14:00:24  josh
 * directory structure shifting
 *
 * Revision 1.1.1.1  2001/11/05 17:47:20  tneale
 * Tornado shuffle
 *
 * Revision 1.12  2001/01/23 01:42:59  paul
 * Reworked 'set' macros to avoid evaluating arguments multiple times.
 *
 * Revision 1.11  2001/01/19 22:22:33  paul
 * Update copyright.
 *
 * Revision 1.10  2000/03/17 00:16:06  meister
 * Update copyright message
 *
 * Revision 1.9  1998/06/08 18:00:42  sra
 * Get rid of some lingering <types.h> references.
 *
 * Revision 1.8  1998/02/25 04:54:04  sra
 * Update copyrights.
 *
 * Revision 1.7  1997/09/26 20:23:39  alan
 * Be certain about install.h and types.h
 *
 * Revision 1.6  1997/06/04 05:46:28  alan
 * Fix RCS keywords.
 *
 * Revision 1.5  1997/06/03 23:02:48  alan
 * Fix RCS keywords.
 *
 * Revision 1.4  1997/05/16 03:47:17  alan
 * Add extra casts for bogus compilers.
 *
 * Revision 1.3  1997/05/14 06:30:56  alan
 * Multiple inclusion check.
 *
 * Revision 1.2  1997/04/22 00:18:34  alan
 * Fix copyright.
 *
 * Revision 1.1  1997/04/19 03:56:16  alan
 * Layout glue macros.
 *
 * Revision 1.2  1997/01/24 22:21:42  alan
 * Add extra parens for bogus compilers.
 *
 * Revision 1.1  1995/08/19  06:35:30  alan
 * Add `glue.h' -- generic byte manipulation glue macros
 */

#ifndef EPILOGUE_LAYOUT_LDBGLUE_H
#define EPILOGUE_LAYOUT_LDBGLUE_H

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif

#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif

#if (INSTALL_OEM_LAYOUT_GLUE_MACROS + INSTALL_UNALIGNED_BIGENDIAN_LAYOUT_GLUE_MACROS + INSTALL_TYPECHECKING_LAYOUT_GLUE_MACROS) > 1
#error "You can only ask for -one- kind of layout glue macros."
#endif

#if INSTALL_OEM_LAYOUT_GLUE_MACROS

#include <ldbglue.h>

#elif INSTALL_UNALIGNED_BIGENDIAN_LAYOUT_GLUE_MACROS

#error "Need to write unaligned bigendian layout glue macros."

#elif INSTALL_TYPECHECKING_LAYOUT_GLUE_MACROS

#error "Need to write typechecking layout glue macros."

#else /* INSTALL_(generic)_LAYOUT_GLUE_MACROS */

/* Here are the completely generic layout glue macros.
 * These should work on -any- hardware.
 */

#define GLUE_CAST_PTR(p) ((bits8_t *) (p))
#define GLUE_CAST08(v) ((ldb_int8_t) (v))
#define GLUE_CAST16(v) ((ldb_int16_t) (v))
#define GLUE_CAST32(v) ((ldb_int32_t) (v))

/* ---- Generic Get ---- */

/* Seemingly redundant parentheses are needed for some buggy compilers
 * (irix 5.2).  Leave them alone!
 */

#define GLUE_GB08(p) ((ldb_int8_t)((p)[0]))

#define GLUE_GB16(p) (  ((ldb_int16_t)((p)[0]) <<  8)			\
		      | ((ldb_int16_t)((p)[1])      ))

#define GLUE_GB32(p) (  ((ldb_int32_t)((p)[0]) << 24)			\
		      | ((ldb_int32_t)((p)[1]) << 16)			\
		      | ((ldb_int32_t)((p)[2]) <<  8)			\
		      | ((ldb_int32_t)((p)[3])      ))

#define GLUE_GB32LS24(p) (  ((ldb_int32_t)((p)[1]) << 16)		\
			  | ((ldb_int32_t)((p)[2]) <<  8)		\
			  | ((ldb_int32_t)((p)[3])      ))

#define GLUE_GB32MS24(p) (  ((ldb_int32_t)((p)[0]) << 24)		\
			  | ((ldb_int32_t)((p)[1]) << 16)		\
			  | ((ldb_int32_t)((p)[2]) <<  8))

#define GLUE_GL08(p) ((ldb_int8_t)((p)[0]))

#define GLUE_GL16(p) (  ((ldb_int16_t)((p)[1]) <<  8)			\
		      | ((ldb_int16_t)((p)[0])      ))

#define GLUE_GL32(p) (  ((ldb_int32_t)((p)[3]) << 24)			\
		      | ((ldb_int32_t)((p)[2]) << 16)			\
		      | ((ldb_int32_t)((p)[1]) <<  8)			\
		      | ((ldb_int32_t)((p)[0])      ))

#define GLUE_GL32LS24(p) (  ((ldb_int32_t)((p)[2]) << 16)		\
			  | ((ldb_int32_t)((p)[1]) <<  8)		\
			  | ((ldb_int32_t)((p)[0])      ))

#define GLUE_GL32MS24(p) (  ((ldb_int32_t)((p)[3]) << 24)		\
			  | ((ldb_int32_t)((p)[2]) << 16)		\
			  | ((ldb_int32_t)((p)[1]) <<  8))

/* ---- Generic Set ---- */

/* Seemingly unnecessary casts to bits8_t are needed for some
 * buggy compilers (GCC 2.6.3).  Leave them alone!
 */

#define GLUE_SB08(p, v) ((p)[0] = ((bits8_t) (v)))

#define GLUE_SB16(p, v) do {						\
			  bits16_t _v_ = (bits16_t)(v);			\
			  (p)[0] = ((bits8_t) ((_v_) >> 8));		\
			  (p)[1] = ((bits8_t) ((_v_)     ));		\
			} while (0)

#define GLUE_SB32(p, v) do {						\
			  bits32_t _v_ = (bits32_t)(v);			\
			  (p)[0] = ((bits8_t) ((_v_) >> 24));		\
			  (p)[1] = ((bits8_t) ((_v_) >> 16));		\
			  (p)[2] = ((bits8_t) ((_v_) >>  8));		\
			  (p)[3] = ((bits8_t) ((_v_)      ));		\
			} while (0)

#define GLUE_SB32LS24(p, v) do { 					\
			      bits32_t _v_ = (bits32_t)(v);		\
			      (p)[1] = ((bits8_t) ((_v_) >> 16));	\
			      (p)[2] = ((bits8_t) ((_v_) >>  8));	\
			      (p)[3] = ((bits8_t) ((_v_)      ));	\
			    } while (0)

#define GLUE_SB32MS24(p, v) do { 					\
			      bits32_t _v_ = (bits32_t)(v);		\
			      (p)[0] = ((bits8_t) ((v) >> 24));		\
			      (p)[1] = ((bits8_t) ((v) >> 16));		\
			      (p)[2] = ((bits8_t) ((v) >>  8));		\
			    } while (0)

#define GLUE_SL08(p, v) ((p)[0] = ((bits8_t) (v)))

#define GLUE_SL16(p, v) do {						\
			  bits16_t _v_ = (bits16_t)(v);			\
			  (p)[1] = ((bits8_t) ((_v_) >> 8));		\
			  (p)[0] = ((bits8_t) ((_v_)     ));		\
			} while (0)

#define GLUE_SL32(p, v) do {						\
			  bits32_t _v_ = (bits32_t)(v);			\
			  (p)[3] = ((bits8_t) ((_v_) >> 24));		\
			  (p)[2] = ((bits8_t) ((_v_) >> 16));		\
			  (p)[1] = ((bits8_t) ((_v_) >>  8));		\
			  (p)[0] = ((bits8_t) ((_v_)      ));		\
			} while (0)

#define GLUE_SL32LS24(p, v) do {					\
			      bits32_t _v_ = (bits32_t)(v);		\
			      (p)[2] = ((bits8_t) ((_v_) >> 16));	\
			      (p)[1] = ((bits8_t) ((_v_) >>  8));	\
			      (p)[0] = ((bits8_t) ((_v_)      ));	\
			    } while (0)

#define GLUE_SL32MS24(p, v) do {					\
			      bits32_t _v_ = (bits32_t)(v);		\
			      (p)[3] = ((bits8_t) ((_v_) >> 24));	\
			      (p)[2] = ((bits8_t) ((_v_) >> 16));	\
			      (p)[1] = ((bits8_t) ((_v_) >>  8));	\
			    } while (0)

/* ---- Generic Opaque values ---- */

/* Since the GLUE_OPAQUEnn macros are defined to take bigendian arguments,
 * the easiest implementation of opaque values is to just define
 * them to be the same as bigendian values!
 */

#define GLUE_GU08 GLUE_GB08
#define GLUE_GU16 GLUE_GB16
#define GLUE_GU32 GLUE_GB32
#define GLUE_GU32LS24 GLUE_GB32LS24
#define GLUE_GU32MS24 GLUE_GB32MS24

#define GLUE_SU08 GLUE_SB08
#define GLUE_SU16 GLUE_SB16
#define GLUE_SU32 GLUE_SB32
#define GLUE_SU32LS24 GLUE_SB32LS24
#define GLUE_SU32MS24 GLUE_SB32MS24

#define GLUE_OPAQUE08(v) (v)
#define GLUE_OPAQUE16(v) (v)
#define GLUE_OPAQUE32(v) (v)

#endif /* INSTALL_(whatever)_LAYOUT_GLUE_MACROS */

#endif /* EPILOGUE_LAYOUT_LDBGLUE_H */
