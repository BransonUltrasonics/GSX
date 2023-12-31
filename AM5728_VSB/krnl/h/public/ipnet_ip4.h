/*
 * Copyright (c) 2006-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
#ifndef IPNET_IP4_H
#define IPNET_IP4_H

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 * Internal IPv4 header file.
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

#include <ipcom_cstyle.h>
#include <ipcom_hash.h>
#include <ipcom_list.h>
#include <ipcom_observer.h>
#include <ipcom_os.h>
#include <ipcom_set.h>
#include <ipcom_slab.h>
#include <ipcom_type.h>

#include "ipnet.h"
#include "ipnet_dst_cache.h"
#include "ipnet_mcast.h"
#include "ipnet_neigh.h"
#include "ipnet_route.h"
#include "ipnet_timeout.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

#ifdef IPNET_USE_RFC1256

/* RFC constants for RFC1256 */
#define IPNET_RFC1256_ROUTER_MAX_INITIAL_ADVERT_INTERVAL       (16 * 1000)
#define IPNET_RFC1256_ROUTER_MAX_INITIAL_ADVERTISEMENTS        (3)
#define IPNET_RFC1256_ROUTER_MAX_RESPONSE_DELAY                (2  * 1000)

#define IPNET_RFC1256_HOST_MAX_SOLICITATION_DELAY              (1  * 1000)
#define IPNET_RFC1256_HOST_SOLICITATION_INTERVAL               (3  * 1000)
#define IPNET_RFC1256_HOST_MAX_SOLICITATIONS                   (3)

/* Type of RFC1256 function enabled on this interface */
#define IPNET_RFC1256_MODE_NONE                                0
#define IPNET_RFC1256_MODE_ROUTER                              1
#define IPNET_RFC1256_MODE_HOST                                2

#define IPNET_RFC1256_GENERIC_STATE_NONE                       0
#define IPNET_RFC1256_GENERIC_STATE_INITIALIZING               1
#define IPNET_RFC1256_GENERIC_STATE_SHUTDOWN                   2

#define IPNET_RFC1256_GENERIC_STATE_END                        2

/* Current host state */
#define IPNET_RFC1256_HOST_STATE_SOLICIT                       (IPNET_RFC1256_GENERIC_STATE_END + 1)
#define IPNET_RFC1256_HOST_STATE_IDLE                          (IPNET_RFC1256_GENERIC_STATE_END + 2)

/* Current router state */
#define IPNET_RFC1256_ROUTER_STATE_BROADCASTING                (IPNET_RFC1256_GENERIC_STATE_END + 1)
#define IPNET_RFC1256_ROUTER_STATE_ADVERTISING                 (IPNET_RFC1256_GENERIC_STATE_END + 2)

/* Current router flags */
#define IPNET_RFC1256_FLAG_ROUTER_MULTICAST                    0x01
#define IPNET_RFC1256_FLAG_ROUTER_PENDING                      0x02

/* Current host flags */
#define IPNET_RFC1256_FLAG_HOST_MULTICAST                      0x01

/* PREFERENCE */
#define IPNET_RFC1256_SIGNED_MIN                               0x80000000
#define IPNET_RFC1256_SIGNED_TO_HOPCOUNT(p)                    ~((Ip_u32)(p) - IPNET_RFC1256_SIGNED_MIN)

#endif

/*
 ****************************************************************************
 * 5                    TYPES
 ****************************************************************************
 */

/*
 *===========================================================================
 *                         forward declarations
 *===========================================================================
 */
struct Ipnet_data_struct;
struct Ipnet_dst_cache_struct;
struct Ipnet_netif_struct;
struct Ipnet_peer_info_struct;
struct Ipnet_socket_struct;
union Ipnet_frame_union;

/*
 *===========================================================================
 *                         Ipnet_pkt_ip
 *===========================================================================
 */
#include "ipcom_align16.h"
typedef IP_PACKED1 struct Ipnet_pkt_ip_struct
{
    Ip_u8    v_hl;      /* 4bit version and 4bit header length. */
    Ip_u8    tos;       /* 8bit type of service. */
    Ip_u16   len;       /* Total length. */
    Ip_u16   id;        /* Identification. */
    Ip_u16   off;       /* Fragment offset field. */
    Ip_u8    ttl;       /* 8bit time-to-live. */
    Ip_u8    p;         /* 8bit IP protocol. */
    Ip_u16   sum;       /* Internet Checksum. */
    Ip_u16   src[2];    /* Source IP address. */
    Ip_u16   dst[2];    /* Destination IP address. */
}
IP_PACKED2 Ipnet_pkt_ip;

#define IPNET_IP_HDR_SIZE   (20)   /* The default/minimum IPv4 header is 20 octets long */

#define IPNET_IP4_GET_VERSION(ip)        ((ip)->v_hl >> 4)
#define IPNET_IP4_GET_HDR_LEN(ip)        ((ip)->v_hl & 0x0F)
#define IPNET_IP4_GET_HDR_OCTET_LEN(ip)  (IPNET_IP4_GET_HDR_LEN(ip) << 2)
#define IPNET_IP4_GET_OPTS_OCTET_LEN(ip) (IPNET_IP4_GET_HDR_OCTET_LEN(ip) - IPNET_IP_HDR_SIZE)
#define IPNET_IP4_GET_IPADDR(ip)         IP_GET_32ON16(ip)
#define IPNET_IP4_SET_IPADDR(ip1,ip2)    IP_SET_32ON16(ip1,ip2)
#define IPNET_IP4_IS_LINK_LOCAL(paddr)   ((IP_GET_NTOHL(paddr) & 0xffff0000) == 0xa9fe0000)

/* IP header offset field. */
#define IPNET_ISFRAG(off)  ((off) & (IPNET_OFF_MF | IPNET_OFF_MASK))
#ifdef IP_LITTLE_ENDIAN
#define IPNET_OFF_MASK   0xff1f
#define IPNET_OFF_MF     0x0020
#define IPNET_OFF_DF     0x0040
#else
#define IPNET_OFF_MASK   0x1fff
#define IPNET_OFF_MF     0x2000
#define IPNET_OFF_DF     0x4000
#endif


typedef IP_PACKED1 struct Ipnet_pkt_ip_opt_struct
{
    Ip_u8   flag_class_num;     /* 1 bit copied flag, 2 bits class, 5 bits option number */
    Ip_u8   len;                /* Option length in octets included the header */
    Ip_u8   data[2];            /* Option data */
}
IP_PACKED2 Ipnet_pkt_ip_opt;


typedef IP_PACKED1 struct Ipnet_pkt_ip_opt_timestamp_struct
{
    Ip_u8   flag_class_num;     /* IP_IPOPT_TIMESTAMP */
    Ip_u8   len;                /* Option length in octets included the header */
    Ip_u8   pointer;            /* the number of octets from the beginning of this option to the
                                   end of timestamps (i.e., the beginning of space for next
                                   timestamp).  It is set to one, an odd number, when no more
                                   space remains in the header for timestamps */
    Ip_u8   overflow_flags;     /* The number of IP modules that cannot register timestamps
                                   due to lack of space. */
    Ip_u8   timestamp[1];       /* Timestamp area */
}
IP_PACKED2 Ipnet_pkt_ip_opt_timestamp;
#include "ipcom_align16.h"

#define IPNET_OPT_TS_GET_FLAGS(ts_opt) ((ts_opt)->overflow_flags & 0xf)
#define IPNET_OPT_TS_GET_OVERFLOW_COUNT(ts_opt) ((ts_opt)->overflow_flags >> 4)
#define IPNET_OPT_TS_INC_OVERFLOW_COUNT(ts_opt) ((ts_opt)->overflow_flags = (Ip_u8) ((ts_opt)->overflow_flags + 0x10))


/*
 *===========================================================================
 *                         Ipnet_pkt_icmp
 *===========================================================================
 */
 #include "ipcom_align16.h"
typedef IP_PACKED1 struct Ipnet_pkt_icmp_struct
{
    Ip_u8    type;
    Ip_u8    code;
    Ip_u16   cksum;       /* Internet Checksum. */

    union Ipnet_pkt_icmp_data_union
    {
        struct Ipnet_pkt_icmp_data_echo_struct
        {
            Ip_u16    id;
            Ip_u16    seq;
            Ip_u16    data[2];
        } echo;

        struct Ipnet_pkt_icmp_data_redirect_struct
        {
            Ip_u16    gateway_addr[2];
            Ip_u16    ip[10];
            Ip_u16    transport_hdr[4];
        } redirect;

        struct Ipnet_pkt_icmp_data_failing_pkt_struct
        {
            Ip_u16    next_hop_mtu[2];  /* Only if code NEED_FRAG, unused otherwise */
            Ip_u16    ip[10];
            Ip_u16    transport_hdr[4];
        } failing_pkt;

        struct Ipnet_pkt_icmp_timestamp_struct
        {
            Ip_u16    id;
            Ip_u16    seq;
            Ip_u16    originate[2];
            Ip_u16    receive[2];
            Ip_u16    transmit[2];
        } timestamp;

        struct Ipnet_pkt_igmp_struct
        {
            Ip_u16    multicast_addr[2];
            Ip_u8     s_qrv;
            Ip_u8     qqic;
            Ip_u16    number_of_sources;
            Ip_u16    source_addr[1];
        } igmp;

        struct Ipnet_pkt_igmp_param_struct
        {
            Ip_u8     pointer;
            Ip_u8     unused[3];
        } param;

        struct Ipnet_pkt_icmp_advert_struct
        {
            Ip_u8     num_addrs;
            Ip_u8     addr_entry_size;
            Ip_u16    lifetime;
            Ip_u32    advert[1];
        } advertise;

        struct Ipnet_pkt_icmp_data_addrmask_struct
        {
            Ip_u16    id;
            Ip_u16    seq;
            Ip_u32    mask;
        } addrmask;
    }
    data;
}
IP_PACKED2 Ipnet_pkt_icmp;


/*
 *===========================================================================
 *                         Ipnet_pkt_icmp
 *===========================================================================
 */
typedef IP_PACKED1 struct Ipnet_pkt_igmpv3_group_record_struct
{
    Ip_u8  record_type;
    Ip_u8  aux_data_len;
    Ip_u16 number_of_sources;
    Ip_u32 multicast_addr;
    Ip_u32 source_addr[1];
}
IP_PACKED2 Ipnet_pkt_igmpv3_group_record;

typedef IP_PACKED1 struct Ipnet_pkt_igmpv3_report_struct
{
    Ip_u8  type;
    Ip_u8  reserved1;
    Ip_u16 checksum;
    Ip_u16 reserved2;
    Ip_u16 number_of_group_records;
    Ipnet_pkt_igmpv3_group_record group_records[1];
}
IP_PACKED2 Ipnet_pkt_igmpv3_report;
#include "ipcom_align16.h"

#define IPNET_IGMPV3_REPORT_MIN_SIZE 16

#define IPNET_IGMPV1_QUERY_SIZE     8
#define IPNET_IGMPV2_QUERY_SIZE     IPNET_IGMPV1_QUERY_SIZE
#define IPNET_IGMPV3_MIN_QUERY_SIZE 12


/*
 * Represents a IPv4 multicast address joined by this socket.
 */
typedef struct Ipnet_ip4_sock_mcast_data_struct
{
    struct Ipnet_ip4_sock_mcast_data_struct *next;
    struct Ipnet_netif_struct               *netif;
    struct Ip_in_addr                        maddr;
#ifdef IPNET_USE_SOURCE_SPECIFIC_MCAST
    Ipcom_set                               *sources;
    struct Ipnet_netif_struct               *local_netif;
#endif
}
Ipnet_ip4_sock_mcast_data;


/*
 * IP options to add to each sent datagram
 */
struct Ipnet_ip4_sock_opts
{
    int               len;      /* Total length of all options */
    Ip_u8             opts[40]; /* The options */
};

/*
 * IPv4 specific socket information.
 */
typedef struct Ipnet_ip4_socket_struct
{
    Ip_u32   saddr_n;            /* source IP address. */
    Ip_u32   daddr_n;            /* destination IP address. */

    Ip_u32   multicast_if;       /* Interface to use for outgoing packets (set by socket option) */
    Ip_u16   flags;              /* Flags that only applies to IPv4 (IPNET_SOCKET_FLAG_IP4_xxx) */
    Ip_u8    type_of_service;    /* Type of service (set by socket option) */
    unsigned dont_frag:1;        /* Set the DF on each sent IPv4
                                  * datagram */
    unsigned no_local_frag:1;    /* Set to reject local transmissions
                                  * that cannot fit the MTU of the
                                  * egress interface  */

    /*
     * Multicast addresses joined on this socket
     */
    Ipnet_ip4_sock_mcast_data  *multicast_addr_list;

    struct Ipnet_ip4_sock_opts *opts;         /* IP options to add to the datagram or IP_NULL */
    struct Ip_icmp_filter      *icmp_filter;  /* Pointer to the ICMP filter
                                            (!= IP_NULL only for sockets where proto=ICMP) */
}
Ipnet_ip4_socket;

#define IPNET_SOCKET_FLAG_IP4_ALLOW_BROADCAST   0x01  /* Packets can be sent to the IPv4 broadcast address */
#define IPNET_SOCKET_FLAG_IP4_HDRINCL           0x02  /* The user supplies the IP header */
#define IPNET_SOCKET_FLAG_IP4_BYPASS_FLOW       0x04  /* This socket should bypass IPSEC */
#define IPNET_SOCKET_FLAG_IP4_RECVTTL           0x08  /* Returns the TTL from the IP header as ancillary data */
#define IPNET_SOCKET_FLAG_IP4_RECVTOS           0x10  /* Returns the TOS from the IP header as ancillary data */
#define IPNET_SOCKET_FLAG_IP4_RECVDSTADDR       0x20  /* Return the IP destination address as ancillary data */

#if defined(IPNET_USE_RFC5227) || defined(IPNET_USE_RFC3927)
/*
 *===========================================================================
 *                         Ipnet_address_conflict_detect
 *===========================================================================
 */
typedef struct Ipnet_address_conflict_detect_struct
{
    /* Address Conflict Detection according to RFC3927 or RFC5227 */
    Ip_u8             state;        /* IPNET_IP4_ACD_STATE_xxx */
    Ip_u8             num;          /* Number of probes or announcement left */
    Ipnet_timeout    *tmo;          /* Timeout structure for IPv4 address */
}
Ipnet_address_conflict_detect;
#endif /* #if defined(IPNET_USE_RFC5227) || defined(IPNET_USE_RFC3927) */


/*
 *===========================================================================
 *                    Address entry
 *===========================================================================
 */
/* IPv4 address entry */
typedef struct Ipnet_ip4_addr_entry_struct
{
    /* THIS MEMBER MUST BE FIRST! List of socket(s) that are bound to
       this address */
    struct Ipnet_socket_struct         *socket_list;

    /* THIS MEMBER MUST BE SECOND! Next address in list */
    struct Ipnet_ip4_addr_entry_struct *next;

    /* Next entry that has the same address (but different interface) */
    struct Ipnet_ip4_addr_entry_struct *next_dup_addr;

    /* Next address in list */
    struct Ipnet_ip4_addr_entry_struct *global_next;

    /* The interface this address is assigned to */
    struct Ipnet_netif_struct          *netif;

    /* The neighbor entry for this address. != IP_NULL on unicast addresses */
    Ipnet_neigh                        *neigh;

    /* != IP_NULL if there is a timer running for this address,
        currently only used for tentative addresses */
    struct Ipnet_timeout_struct        *tmo;
    /* Number of references to this address */
    Ip_u16               refcnt;
    /* IPNET_IP4_ADDR_FLAG_xxx flags */
    Ip_u16               flags;
    /* One of the IPNET_ADDR_TYPE_xxx constants */
    enum Ipnet_addr_type type;
    /*
     * Must observe the state of the neighbor entry that is used to
     * response to ARP requests on this address
     */
    Ipcom_observer       neigh_observer;

#ifdef IPCOM_USE_INET6
    Ip_u32  ipv6_mapped_space[3];
#endif /*IPCOM_USE_INET6 */
    Ip_u32  ipaddr_n;
    Ip_u32  netmask_n;

    /* NOTE! This field will only be present if the mode is IPNET_ADDR_TYPE_MULTICAST */
    Ipnet_mcast_addr mcast;

#if defined(IPNET_USE_RFC5227) || defined(IPNET_USE_RFC3927)
    /* Address Conflict Detection according to RFC3927 or RFC5227 */
    struct Ipnet_address_conflict_detect_struct acd;
#endif
}
Ipnet_ip4_addr_entry;

#define IPNET_IP4_ADDR_FLAG_RW_MASK      0xFF80 /* Mask to filter away read only flags */

/* Read only flags */
#define IPNET_IP4_ADDR_FLAG_LOOPBACK_RT  0x1    /* A loopback route was added for this address */
#define IPNET_IP4_ADDR_FLAG_NETWORK_RT   0x2    /* A network route was added for this address */
#define IPNET_IP4_ADDR_FLAG_HOST_RT      0x4    /* A host route was added for this address */
#define IPNET_IP4_ADDR_FLAG_MCAST        0x8    /* This address was used to join a link multicast group */
#define IPNET_IP4_ADDR_FLAG_NETBRD       0x10   /* A (sub)network broadcast address was added together with this address */
#define IPNET_IP4_ADDR_FLAG_NEWADDR_DONE 0x20   /* A new address message has been delivered for this address */
#define IPNET_IP4_ADDR_FLAG_UPDATE_DONE  0x40   /* This address has been processed in an all address update */
#define IPNET_IP4_ADDR_FLAG_PREFERRED    0x80   /* This is a preferred global address */

/* Read/write flags, can be specified in the ipnet_ip4_add_addr2() call */
#define IPNET_IP4_ADDR_FLAG_AUTOMATIC    0x0100 /* This address has been added automatically */
#define IPNET_IP4_ADDR_FLAG_PRIMARY      0x0200 /* Primary IPv4 address. IPNET does not make any
                                                   distingtions between IPv4 addresses, but this flag
                                                   is required to keep track of which address to
                                                   set/replace, return or delete by IP_SIOCSIFADDR,
                                                   IP_SIOCGIFADDR and IP_SIOCDIFADDR. */
#define IPNET_IP4_ADDR_FLAG_HOMEADDRESS  0x0400 /* Home address */
#define IPNET_IP4_ADDR_FLAG_LINK_LOCAL   0x0800 /* RFC 3927 link local IPv4 address */
#define IPNET_IP4_ADDR_FLAG_TENTATIVE    0x1000 /* Address in tentative state. E.g. during ARP probing for duplicate address detection
                                                   This means it will not be available for any sockets or visible in ifconfig */


/*
 *===========================================================================
 *                         Ipnet_icmp_param
 *===========================================================================
 */
typedef struct Ipnet_icmp_param_struct
{
    /* The packet that caused the ICMP response */
    Ipcom_pkt     *recv_pkt;
    /* The the ICMP message type */
    int            type;
    /* The value to use in the code field */
    int            code;

    /* Address the ICMP message should be sent to */
    struct Ip_in_addr to;

    /*
     * Address the ICMP message should use as source address or
     * INADDR_ANY to use the source selection rules to determine the
     * source
     */
    struct Ip_in_addr from;

    /* TTL, if specified */
    Ip_u8          ttl;

    /*
     * TOS to use, unless the ICMP specification requires a specific
     * TOS value
     */
    Ip_u8          tos;

    /* Virtual router index */
    Ip_u16         vr;

    /* Egress interface or 0 if any interface can be used */
    unsigned ifindex;

    union {
        /* Redirect msg: the on link address to use instead of the address of this host */
        Ip_u8                       param_pointer;
        IP_CONST struct Ip_in_addr *gateway_addr;
        Ip_u32                      max_path_mtu;
        Ipnet_ip4_addr_entry       *igmp_addr_entry;
    } data;
}
Ipnet_icmp_param;


/*
 * Ipv4 data, shared by all stack instances.  All data is read-only
 * unless all but the primary stack instance are suspended.
 */
typedef struct Ipnet_ip4_shared_data_struct
{
    /*
     * Maps unicast or multicast address, interface and virtual route
     * tab index to the Ipnet_ip4_addr_entry of the matching local
     * address entry
     */
    Ipcom_hash   *addrs;

    /*
     * Same as above except for that two addresses on different
     * interfaceses will occupy the same slot
     */
    Ipcom_hash   *addrs_ignore_if;

    /*
     * Slab cache for IPv4 addresses
     */
    Ipcom_memory_cache *addr_slab;

    /*
     * Home addresses; to be assigned prior to any other addresses
     */
    Ipnet_ip4_addr_entry *globals;

#ifdef IPNET_USE_TUNNEL
    /*
     * Hash table of available tunnel interfaces over IPv4
     */
    Ipcom_hash   *tunnel_ifs;
#endif /* IPNET_USE_TUNNEL */
}
Ipnet_ip4_shared_data;


/*
 ****************************************************************************
 * 6                    FUNCTIONS
 ****************************************************************************
 */

IP_INLINE struct Ipnet_netif_struct *
ipnet_ip4_addr_to_netif(IP_CONST Ipnet_ip4_addr_entry *addr)
{
    return addr->netif;
}

/*
 *===========================================================================
 *                    ipnet_ip4_pseudo_header_checksum_update
 *===========================================================================
 * Description: Calculates the internet checksum on IPv4 pseudo header.
 * Parameters:  src - The source address.
 *              dst - The destination address.
 *              proto - The protcol ID of the upper layer protocol.
 *              len - The number of octets in the payload for which this is
 *              pseudo header.
 * Returns:     The accumulated 32 checksum including the pseudo header.
 *
 */
IP_INLINE Ip_u32
ipnet_ip4_pseudo_header_checksum_update(IP_CONST struct Ip_in_addr *src,
                                        IP_CONST struct Ip_in_addr *dst,
                                        Ip_u16 proto, Ip_u16 len)
{
    Ip_u32   sum;

    sum  = ((Ip_u16 *) src)[0];
    sum += ((Ip_u16 *) src)[1];
    sum += ((Ip_u16 *) dst)[0];
    sum += ((Ip_u16 *) dst)[1];
    sum += ip_htons(len);
    sum += ip_htons(proto);

    return sum;
}


IP_GLOBAL int  ipnet_ip4_init_once(void);

IP_GLOBAL int ipnet_ip4_insert_addr_cache(Ipnet_ip4_addr_entry *addr);
IP_GLOBAL void ipnet_ip4_remove_addr_cache(Ipnet_ip4_addr_entry *addr);
IP_GLOBAL int ipnet_ip4_change_addr_mask(Ipnet_ip4_addr_entry *addr, Ip_u32 new_netmask_n);
IP_GLOBAL int ipnet_ip4_add_addr(struct Ipnet_netif_struct *netif, Ip_u32 ipaddr_n, Ip_u32 netmask_n,
                                 Ip_bool primary, enum Ipnet_addr_type addr_type);
IP_GLOBAL Ip_u32 ipnet_ip4_netif_get_subbrd(struct Ipnet_netif_struct *netif);

IP_GLOBAL int ipnet_ip4_add_addr2(struct Ipnet_netif_struct *netif, Ip_u32 ipaddr_n, Ip_u32 netmask_n,
                                  Ip_u16 flags, enum Ipnet_addr_type addr_type);
IP_GLOBAL int ipnet_ip4_remove_addr(struct Ipnet_netif_struct *netif, Ip_u32 ipaddr_n);

IP_GLOBAL enum Ipnet_addr_type ipnet_ip4_get_addr_type(Ip_u32 ipaddr_n,
                                                       Ip_u16 vr,
                                                       struct Ipnet_netif_struct *netif);
IP_GLOBAL enum Ipnet_addr_type ipnet_ip4_get_addr_type2(Ip_u32 ipaddr_n, struct Ipnet_netif_struct *netif);
IP_GLOBAL Ipnet_ip4_addr_entry *ipnet_ip4_get_addr_entry(Ip_u32 ipaddr_n, Ip_u16 vr,
                                                         struct Ipnet_netif_struct *netif);

IP_FASTTEXT IP_GLOBAL int ipnet_ip4_input(struct Ipnet_netif_struct *netif, Ipcom_pkt *pkt);
IP_FASTTEXT IP_GLOBAL int ipnet_ip4_rx(Ipnet_dst_cache *dst, Ipcom_pkt *pkt, Ipnet_pkt_ip *iphdr);

IP_GLOBAL void ipnet_ip4_dst_unreachable(Ipcom_pkt *pkt, int err);
IP_GLOBAL void ipnet_ip4_dst_unreachable_filter(Ipcom_pkt *pkt);

IP_GLOBAL int  ipnet_ip4_sendto(struct Ipnet_socket_struct *sock,
                                IP_CONST struct Ip_msghdr *msg,
                                Ipcom_pkt *pkt);
IP_GLOBAL int ipnet_ip4_tx(struct Ipnet_data_struct *net,
                           Ip_u8 tos,
                           Ip_u8 ttl,
                           Ip_u8 ip_proto,
                           IP_CONST Ip_u32 *src,
                           IP_CONST Ip_u32 *dst,
                           Ip_u32 ifindex,
                           Ipcom_pkt *pkt);
IP_GLOBAL int ipnet_ip4_pkt_with_iphdr_tx(struct Ipnet_data_struct *net,
                                          Ipcom_pkt *pkt,
                                          void *rt_lookup_key);
IP_GLOBAL void ipnet_ip4_transport_rx(Ipnet_dst_cache *dst, Ipcom_pkt *pkt, Ip_u8 proto);

#ifdef IPTCP
IP_GLOBAL Ip_u32 ipnet_ip4_get_mss(struct Ipnet_socket_struct *sock,
                                   Ip_bool *is_link_local);
#endif
IP_GLOBAL IP_CONST struct Ip_in_addr * ipnet_ip4_get_src_addr(Ip_u16 vr,
                                                              IP_CONST struct Ip_in_addr *dst_addr,
                                                              Ipnet_route_entry *rt,
                                                              struct Ipnet_netif_struct *netif);
IP_GLOBAL IP_CONST struct Ip_in_addr * ipnet_ip4_get_src_addr_ext(Ip_u16 vr,
                                                                  IP_CONST struct Ip_in_addr *dst_addr,
                                                                  Ipnet_route_entry *rt,
                                                                  struct Ipnet_netif_struct *netif,
                                                                  Ip_src_addr_filter         filter,
                                                                  void *                     filter_arg);

IP_GLOBAL Ipnet_dst_cache * ipnet_ip4_dst_cache_get_tx(struct Ipnet_data_struct *net,
                                                       Ip_u16 vr,
                                                       const struct Ip_in_addr *to,
                                                       const struct Ip_in_addr *from,
                                                       Ip_u8 tos,
                                                       unsigned egress_ifindex,
                                                       unsigned ingress_ifindex);

IP_GLOBAL int ipnet_ip4_dst_cache_rx_ctor(Ipnet_dst_cache *dst,
                                          Ipnet_route_entry *rt);

IP_GLOBAL void ipnet_ip4_flow_spec_from_pkt(Ipnet_flow_spec *flow_spec,
                                            const Ipcom_pkt *pkt,
                                            const Ipnet_pkt_ip *ip_hdr,
                                            Ip_bool is_ingress);
IP_GLOBAL int ipnet_ip4_flow_spec_from_info(Ipnet_flow_spec *flow_spec,
                                            const void        *src,
                                            const void        *dst);


IP_GLOBAL int ipnet_ip4_add_route_table(Ip_u16 vr, Ip_u32 table);
IP_GLOBAL int ipnet_ip4_configure_route_table(Ip_u16 vr, Ip_u32 table);

IP_GLOBAL struct Ipnet_netif_struct * ipnet_sockopt_ip4_addr_to_netif(Ip_u16 vr, struct Ip_in_addr *in_addr);
IP_GLOBAL Ip_bool ipnet_ip4_is_part_of_same_pkt(Ipcom_pkt *frag1, Ipcom_pkt *frag2);

IP_GLOBAL void ipnet_ip4_kioevent(struct Ipnet_netif_struct *netif, int command);

IP_GLOBAL Ipnet_ip4_sock_mcast_data **ipnet_sock_ip4_get_mcast_data(struct Ipnet_socket_struct *sock,
                                                                    struct Ipnet_netif_struct *netif,
                                                                    IP_CONST struct Ip_in_addr *maddr);
IP_GLOBAL int ipnet_sock_ip4_mcast_delete_data(struct Ipnet_socket_struct *sock,
                                               struct Ipnet_netif_struct *netif,
                                               struct Ip_in_addr *maddr);
IP_GLOBAL struct Ipnet_socket_struct * ipnet_sock_ip4_lookup(Ip_u16 vr, int proto,
                                                             IP_CONST Ip_u32 *local_addr_ptr, Ip_u16 local_port,
                                                             IP_CONST Ip_u32 *peer_addr_ptr, Ip_u16 peer_port);


IP_GLOBAL void ipnet_ip4_fragment_timeout(Ipcom_pkt *frag_head);


#ifdef IPNET_USE_SOURCE_SPECIFIC_MCAST
IP_GLOBAL void
ipnet_igmpv3_report_change(struct Ipnet_netif_struct *netif, struct Ip_in_addr *group);
#endif

#ifdef IPCOM_USE_ETHERNET
IP_GLOBAL void ipnet_arp_request(struct Ipnet_netif_struct *netif, Ip_u32 pending_pkt_srcaddr_n,
                                 Ip_u32 dstaddr_n, int gratuitous, Ip_u8 *src_mac_arg, Ip_u8 *dst_mac_arg);
IP_GLOBAL int ipnet_arp_ioctl(struct Ipnet_socket_struct *sock, unsigned long reqest, struct Ip_arpreq *req);
#endif

#ifdef IPNET_USE_MCAST_ROUTING
IP_GLOBAL void ipnet_ip4_mcast_set_assert(struct Ipnet_socket_struct *sock, Ip_bool val);
IP_GLOBAL int ipnet_ip4_mcast_add_vif(struct Ipnet_socket_struct *sock, struct Ip_vifctl *vifctl);
IP_GLOBAL int ipnet_ip4_mcast_del_vif(struct Ipnet_socket_struct *sock, Ip_vifi_t vifi);
IP_GLOBAL int ipnet_ip4_mcast_add_mfc(struct Ipnet_socket_struct *sock, struct Ip_mfcctl *mfcctl);
IP_GLOBAL int ipnet_ip4_mcast_del_mfc(struct Ipnet_socket_struct *sock, struct Ip_mfcctl *mfcctl);
IP_GLOBAL int ipnet_ip4_mcast_mrt_done(struct Ipnet_socket_struct *sock);
IP_GLOBAL int ipnet_ip4_mcast_getsockopt(struct Ipnet_socket_struct *sock,
                                         int optname,
                                         void *optval,
                                         Ip_socklen_t *optlen);
IP_GLOBAL int ipnet_ip4_mcast_setsockopt(struct Ipnet_socket_struct *sock,
                                         int optname,
                                         IP_CONST void *optval,
                                         Ip_socklen_t optlen);
IP_GLOBAL void ipnet_ip4_mcast_input(struct Ipnet_netif_struct *netif, Ipcom_pkt *pkt);
IP_GLOBAL int ipnet_ip4_mcast_ioctl(struct Ipnet_socket_struct *sock,
                                    unsigned long request,
                                    void *data);
IP_GLOBAL void ipnet_ip4_mcast_tick(void);

IP_GLOBAL void ipnet_pim_ip4_rx(Ipnet_dst_cache *dst, Ipcom_pkt *pkt);
#endif /* IPNET_USE_MCAST_ROUTING */

#ifdef IPNET_USE_NAT
IP_GLOBAL Ip_s32 ipnet_nat_getsockopt(struct Ipnet_socket_struct *sock, int optname, void *optval,
                           Ip_socklen_t *optlen);

IP_GLOBAL Ip_s32 ipnet_nat_setsockopt(struct Ipnet_socket_struct *sock, int optname, IP_CONST void *optval,
                           Ip_socklen_t optlen);
#endif


#if defined(IPNET_USE_RFC3927) || defined(IPNET_USE_RFC5227)
IP_GLOBAL void ipnet_ip4_acd_conflict(Ipnet_ip4_addr_entry *addr);
#endif /* IPNET_USE_RFC3927 || IPNET_USE_RFC5227 */

#ifdef IPNET_USE_RFC1256
IP_GLOBAL Ip_bool ipnet_ip4_rfc1256_mode_update(Ip_u16                      vr,
                                                struct Ipnet_netif_struct   *netif);
#endif

IP_GLOBAL void ipnet_icmp4_param_init(Ipnet_icmp_param *icmp_param,
                                      Ipcom_pkt *pkt);
IP_GLOBAL int ipnet_icmp4_send(Ipnet_icmp_param *icmp_param, Ip_bool is_igmp);
IP_GLOBAL void  ipnet_icmp4_send_host_unreachable(Ipcom_pkt *pkt);
IP_GLOBAL void ipnet_icmp4_send_port_unreachable(Ipcom_pkt *pkt);

IP_INLINE Ip_size_t
ipnet_ip4_get_hdr_len(Ipnet_pkt_ip *ip_hdr)
{
    return (Ip_size_t) (ip_hdr->v_hl & 0xF);
}

IP_INLINE Ip_size_t
ipnet_ip4_get_hdr_octet_len(Ipnet_pkt_ip *ip_hdr)
{
    return ipnet_ip4_get_hdr_len(ip_hdr) << 2u;
}

IP_INLINE Ip_size_t
ipnet_ip4_get_opts_octet_len(Ipnet_pkt_ip *ip_hdr)
{
    return ipnet_ip4_get_hdr_octet_len(ip_hdr) - IPNET_IP_HDR_SIZE;
}

#ifdef __cplusplus
}
#endif

#endif


/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
