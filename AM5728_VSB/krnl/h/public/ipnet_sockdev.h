/*
 * Copyright (c) 2006-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
#ifndef IPNET_SOCKDEV_H
#define IPNET_SOCKDEV_H

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 * Socket to network device.
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

#include "ipnet_config.h"

#include "ipnet_h.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

#define IPNET_SOCKDEV_IF_NAME_BASE     "sd"


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

IP_PUBLIC int ipnet_sockdev_if_init(Ipnet_netif *netif);


#ifdef __cplusplus
}
#endif

#endif

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
