/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/dsmemcfg.h,v 1.2 2001/11/07 14:27:44 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: dsmemcfg.h,v $
 * Revision 1.2  2001/11/07 14:27:44  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:32  tneale
 * Tornado shuffle
 *
 * Revision 2.4.4.1  2001/10/31 20:04:57  paul
 * Removed unneeded udptable.h
 *
 * Revision 2.4  2001/01/19 22:23:34  paul
 * Update copyright.
 *
 * Revision 2.3  2000/03/17 00:12:12  meister
 * Update copyright message
 *
 * Revision 2.2  1999/04/05 23:03:41  qli
 * updated to use the new memory pool identifier names
 *
 * Revision 2.1  1999/02/18 04:41:31  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 1.1.2.1  1998/12/02 20:47:35  qli
 * data structure memory pools configuration options
 * should be included by sys_conf.h file since it is application specific in
 * the case of pSOS
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef _NC_DSMEMCFG_H
#define _NC_DSMEMCFG_H

#include <wrn/wm/attache/config.h>
#include <wrn/wm/attache/arp.h>
#include <wrn/wm/attache/ip_rte.h>
#include <wrn/wm/attache/ip4_rd.h>
#include <wrn/wm/attache/tcp.h>
#include <wrn/wm/attache/udp.h>

#define NC_IF_ARP_INSTANCE                 5
#define NC_IF_ARP_ENTRIES                  5   /* each entry contains (MAX_ARP_ENTRIES * size) */
#define NC_IF_ARP_IPADDRESS                20
 
#define NC_RTE_TABLE_DISPATCH              20
#define NC_RTE_TABLE_HEAD                  20
#define NC_RTE_TABLE_AVLNODE               20
#define NC_RTE_TABLE_ROUTEHEAD             20
#define NC_RTE_TABLE_RTENTRY_NHGATEWAY     20   /* each entry is (route_entry_t + nh_gateway_t) */
#define NC_RTE_TABLE_FIXEDEXT              20
#define NC_RTE_TABLE_ADDREXT               20
#define NC_RTE_TABLE_OWNER                 20
 
#define NC_RTE_DISC_DISPATCH               20
#define NC_RTE_DISC_HEAD                   20
#define NC_IF_RD_ROUTERCONF                20
#define NC_IF_RD_HOSTCONF                  20
#define NC_RTE_DISC_AVLNODE                20
#define NC_RTE_DISC_RDCENTRY               20
#define NC_RTE_DISC_NHBLOCK                20
 
#define NC_SNMP_TMP_ENTRY                  20
#define NC_SNMP_ROUTEEXT                   20
#define NC_SNMP_RTENTRY_NHGATEWAY          20
 
#define NC_TCP_TCB                         10
 
#define NC_UDP_TABENT                      10


/*
 * size of each configured data structure
 */
#define SZ_NC_IF_ARP_INSTANCE              sizeof(struct arp_instance)
#define SZ_NC_IF_ARP_ENTRIES               (sizeof(struct arp_entry) + MAX_HW_ADDR_LENGTH + MAX_ARP_MISC_SIZE) * MAX_ARP_ENTRIES 
#define SZ_NC_IF_ARP_IPADDRESS             sizeof(struct arp_ip_addr)
 
#define SZ_NC_RTE_TABLE_DISPATCH           sizeof(avl_dispatch_t)
#define SZ_NC_RTE_TABLE_HEAD               sizeof(avl_head_t)
#define SZ_NC_RTE_TABLE_AVLNODE            sizeof(avl_node_t)
#define SZ_NC_RTE_TABLE_ROUTEHEAD          sizeof(route_head_t)
#define SZ_NC_RTE_TABLE_RTENTRY_NHGATEWAY  sizeof(route_entry_t) + sizeof(nh_gateway_t)
#define SZ_NC_RTE_TABLE_FIXEDEXT           sizeof(fixed_route_ext_t)
#define SZ_NC_RTE_TABLE_ADDREXT            sizeof(addr_route_ext_t)
#define SZ_NC_RTE_TABLE_OWNER              sizeof(route_owner_t)
 
#define SZ_NC_RTE_DISC_DISPATCH            sizeof(avl_dispatch_t)
#define SZ_NC_RTE_DISC_HEAD                sizeof(avl_head_t)
#if INSTALL_ATTACHE_IP_FORWARDING
#define SZ_NC_IF_RD_ROUTERCONF             sizeof(rd_router_conf_t)
#else
#define SZ_NC_IF_RD_ROUTERCONF             0
#endif
#if ((!INSTALL_ATTACHE_IP_FORWARDING) || INSTALL_ATTACHE_KEEP_RD_CACHE_ON_ROUTER)
#define SZ_NC_IF_RD_HOSTCONF               sizeof(rd_host_conf_t)
#else
#define SZ_NC_IF_RD_HOSTCONF               0
#endif
#define SZ_NC_RTE_DISC_AVLNODE             sizeof(avl_node_t)
#define SZ_NC_RTE_DISC_RDCENTRY            sizeof(rdc_entry_t)
#define SZ_NC_RTE_DISC_NHBLOCK             sizeof(nh_block_t)

extern tmp_route_entry_t;
#define SZ_NC_SNMP_TMP_ENTRY               sizeof(tmp_route_entry_t) /* snmpcidr.c */

#define SZ_NC_SNMP_ROUTEEXT                sizeof(route_ext_t)
#define SZ_NC_SNMP_RTENTRY_NHGATEWAY       sizeof(route_entry_t) + sizeof(nh_gateway_t)
 
#define SZ_NC_TCP_TCB                      sizeof(struct tcb)
 
#define SZ_NC_UDP_TABENT                   sizeof(udp_handle_t)


#endif
