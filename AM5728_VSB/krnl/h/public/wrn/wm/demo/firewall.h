/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/firewall.h,v 1.2 2001/11/07 14:27:44 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * Rudimentary firewall built on top of Attache.
 * This is a work in progress, I need the basic functionality to solve
 * a short term problem but the code will need more work to make
 * it as general as it should be.
 */

/*
 * $Log: firewall.h,v $
 * Revision 1.2  2001/11/07 14:27:44  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:32  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:23:34  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:12:12  meister
 * Update copyright message
 *
 * Revision 1.1  1998/09/06 08:11:07  sra
 * Packet filtering hooks in Attache, rudimentary firewall
 * implementation in Snark.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef SNARK_FIREWALL_H
#define SNARK_FIREWALL_H

#include <wrn/wm/attache/config.h>
#include <wrn/wm/attache/packet.h>

extern int  snark_firewall_inbound_filter (packet *);
extern int  snark_firewall_forward_filter (packet *);
extern int  snark_firewall_outbound_filter(packet *);

extern void snark_firewall_init(void);

#if INSTALL_SNARK_FIREWALL_INBOUND
#define GLUE_IP_RCV_PKT_FILTER(_P_)  snark_firewall_inbound_filter(_P_)
#endif

#if INSTALL_SNARK_FIREWALL_FORWARD
#define GLUE_IP_FWD_PKT_FILTER(_P_)  snark_firewall_forward_filter(_P_)
#endif

#if INSTALL_SNARK_FIREWALL_OUTBOUND
#define GLUE_IP_SEND_PKT_FILTER(_P_) snark_firewall_outbound_filter(_P_)
#endif

#endif /* SNARK_FIREWALL_H */
