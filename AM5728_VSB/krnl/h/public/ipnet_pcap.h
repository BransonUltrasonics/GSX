/*
 * Copyright (c) 2006-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
#ifndef IPNET_PCAP_H
#define IPNET_PCAP_H

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

#include <ipcom_cstyle.h>
#include <ipcom_sock.h>
#include <ipcom_type.h>

#include "ipnet_netif.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

#define IPNET_PCAP_OP_START  1 /* Start a capture session */
#define IPNET_PCAP_OP_STOP   2 /* Stop a capture session */

/* Types determine how/where the capture packets are stored */
#define IPNET_PCAP_TYPE_FILE 1 /* Store on local filesystem */
#define IPNET_PCAP_TYPE_NET  2 /* Send to node on the network */

#define IPNET_PCAP_FNAMSIZ   255

/*
 ****************************************************************************
 * 5                    TYPES
 ****************************************************************************
 */

/* Is pointed to by the "ip_ifr_opt" member of Ip_mreq structure */
typedef struct Ipnet_pcap_ioctl_struct
{
    int op;              /* One of IPNET_PCAP_OP_xxx constants */
    int type;            /* One of IPNET_PCAP_TYPE_xxx constants */
    union {
        char filename[IPNET_PCAP_FNAMSIZ]; /* Filename for IPNET_PCAP_TYPE_FILE */
        union Ip_sockaddr_union dst; /* Destination address for IPNET_PCAP_TYPE_NET */
    } d;
}
Ipnet_pcap_ioctl_t;

/*
 ****************************************************************************
 * 6                    FUNCTIONS
 ****************************************************************************
 */

IP_GLOBAL int
ipnet_pcap_ioctl(Ipnet_netif *netif, Ip_u32 command, void *data);

IP_PUBLIC int
ipnet_cmd_pcap(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
