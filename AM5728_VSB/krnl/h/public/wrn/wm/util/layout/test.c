/* $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/test.c,v 1.1.1.1 2001/11/05 17:47:20 tneale Exp $ */

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
 *  Copyright 1997 Epilogue Technology Corporation.
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
 * Test the .h files generated from .ldb files.
 */

/*
 * $Log: test.c,v $
 * Revision 1.1.1.1  2001/11/05 17:47:20  tneale
 * Tornado shuffle
 *
 * Revision 1.10  2001/01/19 22:22:34  paul
 * Update copyright.
 *
 * Revision 1.9  2000/03/17 00:16:06  meister
 * Update copyright message
 *
 * Revision 1.8  1998/11/23 20:54:36  wes
 * Add layout-macro description for DHCP packets.
 *
 * Revision 1.7  1998/04/09 21:49:12  wes
 * Make netime use layout macros
 *
 * Revision 1.6  1998/04/08 17:01:13  wes
 * Add TFTP layout macros
 *
 * Revision 1.5  1998/02/25 04:54:15  sra
 * Update copyrights.
 *
 * Revision 1.4  1997/11/18 03:18:35  sra
 * Brand new PAP and CHAP code for PPP.
 *
 * Revision 1.3  1997/10/30 19:29:20  sra
 * Silence a silly warning.
 *
 * Revision 1.2  1997/09/26 20:37:47  alan
 * Don't include types.h and ldbglue.h -- let other files pull
 * them in...
 *
 * Revision 1.1  1997/09/10 07:42:37  alan
 * I've been using this file myself for a while -- others may find it useful
 * in the future.
 *
 */

/* Include -all- of the automatically generated .h files.
 * This catches bugs like duplicated names,
 * and perhaps (in some environments) names that are too long.
 * We explicitly do -not- include ldbglue.h, types.h, etc. here
 * because we want to make sure that the .h files do it right
 * themselves.
 */

#include "agentx.h"
#include "apple.h"
#include "arp.h"
#include "bgp.h"
#include "chaos.h"
#include "dhcp.h"
#include "dns.h"
#include "ethernet.h"
#include "ip.h"
#include "ip6.h"
#include "netime.h"
#include "ppp.h"
#include "rip.h"
#include "secure.h"
#include "tftp.h"

/* Now some test code.
 * Currently we are only interested in whether this code compiles.
 * Someday, to do a complete test you may have to actually -run-
 * the executable.
 */

int
main(int argc, char **argv)
{
  bits8_t buffer[1000];

  /* Once upon a time, this generated some code that caused certain recent
   * versions of GCC (2.6.3 at least) to issue an (incorrect) warning.
   * There is a comment in ldbglue.h about this.
   * Make sure that never happens again.
   */
  SET_ETHERNET_HEADER_TYPE(buffer, ETHERNET_TYPE_is_IPV6);

  return 0;
}
