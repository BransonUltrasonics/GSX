/*
 * Copyright (c) 2006-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
#ifndef IPNET_ICMP6_H
#define IPNET_ICMP6_H

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 2                    CONFIGURATION
 ****************************************************************************
 */

#include "ipnet_config.h"


/*
 ****************************************************************************
 * 3                    INCLUDE FILES
 ****************************************************************************
 */

#include <ipcom_type.h>
#include <ipcom_cstyle.h>

#include "ipnet_ip6.h"
#include "ipnet_route.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

/* Router Solicitation is disabled on a link if the 'inet6_rs_retrans_count'
   member of Ipnet_netif is set to this value */
#define IPNET_INET6_RS_RETRANS_COUNT_DISABLED  0x00
/* The retransmission count will be read from sysvar if the
   'inet6_rs_retrans_count' member of Ipnet_netif is set to this value */
#define IPNET_INET6_RS_RETRANS_COUNT_NEED_INIT 0xff


/*
 ****************************************************************************
 * 5                    TYPES
 ****************************************************************************
 */

struct Ipnet_icmp6_redirect_param
{
    /* The IP address of the destination which is redirected to the target. */
    IP_CONST struct Ip_in6_addr    *dst_addr;

    /* The neighbor entry for the better next-hop */
    IP_CONST Ipnet_neigh           *next_hop;
};

struct Ipnet_icmp6_neighbor_sol_param
{
    /* The IP address of the target of the solicitation. It MUST NOT be a multicast address. */
    IP_CONST struct Ip_in6_addr    *target_addr;
};

struct Ipnet_icmp6_neighbor_adv_param
{
    IP_CONST struct Ip_in6_addr *target_addr;
    /* Neighbor entry matching the ND request */
    IP_CONST Ipnet_neigh        *neigh;
    /* Set to 0 if this is a host */
    Ip_u8                        is_router;
    /* Set to 0 if the solicated flag should not be set */
    Ip_u8                        solicated;
    /* Set to 0 if the override flag should not be set */
    Ip_u8                        override;
};

struct Ipnet_icmp6_mld_param
{
    /* The multicast address to create a report for. */
    Ipnet_ip6_addr_entry *addr_entry;
};

typedef struct Ipnet_icmp6_param_struct
{
    /*
     * The interface to use if the packet is sent to a scoped
     * (link-local) address
     */
    Ip_u32          ifindex;

    /* The packet that caused the ICMP response */
    Ipcom_pkt      *recv_pkt;

    /* The virtual router to use when sending the ICMPv6 packet */
    Ip_u16          vr;

    /* The the ICMPv6 message type */
    Ip_u8           type;

    /* The value to use in the code field */
    Ip_u8           code;

    /* If this ICMPv6 message is subject to throttling */
    Ip_bool         rate_limit;

    /*
     * The address to use as source address, can be IP_NULL (IPNET
     * will then use the link-local address of the outgoing interface)
     */
    IP_CONST struct Ip_in6_addr  *from;

    /*
     * The address to which the ICMP message should be sent, must not
     * be IP_NULL
     */
    IP_CONST struct Ip_in6_addr  *to;

	/* VRRP ID of this packet */
	Ip_u8           vrid;

    union
    {
        struct Ipnet_icmp6_redirect_param       redirect;
        struct Ipnet_icmp6_neighbor_sol_param   neigbor_sol;
        struct Ipnet_icmp6_neighbor_adv_param   neighbor_adv;
        Ip_u32                                  pkt_to_big_mtu;
        Ip_u32                                  param_prob_pointer;
        struct Ipnet_icmp6_mld_param            mld;
    } data;
}
Ipnet_icmp6_param;


/*
 ****************************************************************************
 * 6                    FUNCTIONS
 ****************************************************************************
 */

IP_GLOBAL Ip_u16 ipnet_icmp6_checksum(IP_CONST struct Ip_in6_addr *src,
                                      IP_CONST struct Ip_in6_addr *dst,
                                      Ip_u16 proto,
                                      Ip_u16 *data,
                                      Ip_u16 bytes);
IP_GLOBAL void ipnet_icmp6_param_init(Ipnet_icmp6_param *icmp6_param,
                                      Ipcom_pkt *pkt);
IP_GLOBAL void ipnet_icmp6_neighbor_cache_init(Ipnet_route_entry *rt);
IP_GLOBAL void ipnet_icmp6_input(Ipnet_dst_cache *dst, Ipcom_pkt *pkt);
IP_GLOBAL void ipnet_icmp6_nd_probe(Ipnet_neigh *neigh, Ip_bool unicast_probe);
IP_GLOBAL int  ipnet_icmp6_send_dad(Ipnet_ip6_addr_entry *addr_entry);
IP_GLOBAL void ipnet_icmp6_send_addr_unreachable(Ipcom_pkt *pkt);
IP_GLOBAL void ipnet_icmp6_send_port_unreachable(Ipcom_pkt *pkt);
IP_GLOBAL void ipnet_icmp6_send_dst_unreach(Ipcom_pkt *failing_pkt, int error_code);
IP_GLOBAL void ipnet_icmp6_send_router_sol_msg(Ipnet_netif *netif);
IP_GLOBAL int  ipnet_icmp6_send_router_adv(Ipnet_netif *netif);
IP_GLOBAL int  ipnet_icmp6_send(Ipnet_icmp6_param *icmp6_param);
IP_GLOBAL Ipnet_pkt_nd_opt_hdr* ipnet_icmp6_find_opts(Ipcom_pkt *pkt, int *tot_opt_len);
IP_GLOBAL Ip_bool ipnet_icmp6_validate_options_len(Ipnet_pkt_nd_opt_hdr *first_opt, int tot_opt_len);
IP_GLOBAL Ip_bool ipnet_icmp6_option_present(Ipcom_pkt *pkt, Ip_u8 opt);
IP_GLOBAL Ipnet_pkt_nd_opt_hdr *ipnet_icmp6_get_option(Ipnet_pkt_nd_opt_hdr *first_opt,
                                                       int tot_opt_len,
                                                       Ip_u16 type,
                                                       int nr);

#ifdef IPNET_USE_SOURCE_SPECIFIC_MCAST
IP_GLOBAL void ipnet_mldv2_report_change(Ipnet_netif *netif, struct Ip_in6_addr *group);
#endif /* IPNET_USE_SOURCE_SPECIFIC_MCAST */
IP_GLOBAL void ipnet_mld_report_filter_change(Ipnet_ip6_addr_entry *maddr);
IP_GLOBAL void ipnet_mld_done(Ipnet_ip6_addr_entry *maddr);

IP_GLOBAL void ipnet_icmp6_nd_delete(Ipnet_neigh *neigh);
IP_GLOBAL Ip_bool ipnet_icmp6_default_router_change_rtl(struct Ipnet_netif_struct *netif,
                                                        IP_CONST struct Ip_in6_addr *gw_addr,
                                                        Ip_bool adjust_lifetime,
                                                        Ip_u32 lifetime_sec,
                                                        Ip_bool adjust_preference,
                                                        Ip_bool preferred);
IP_GLOBAL Ipnet_route_entry *ipnet_icmp6_prefix_get_rtl(Ipnet_netif *netif,
                                                        IP_CONST struct Ip_in6_addr *prefix,
                                                        Ip_u8 prefix_len);
IP_GLOBAL void ipnet_icmp6_init(void);

#ifdef __cplusplus
}
#endif

#endif

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
