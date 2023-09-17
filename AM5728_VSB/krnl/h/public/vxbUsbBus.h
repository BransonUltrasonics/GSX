/* vxbUsbBus.h - USB Bus VxBus Interface Header File */

/*
 * Copyright (C) 2013 Wind River Systems, Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* 
modification history
--------------------
04nov13,s_z  created
*/

#ifndef __INCvxbUsbBush
#define __INCvxbUsbBush

#include <vxWorks.h>
#include <hwif/vxBus.h>
#include <hwif/util/vxbResourceLib.h>

#ifdef __cplusplus
extern "C" {
#endif

BUSTYPE_DECL(vxbUsbBus);
BUSTYPE_DECL(vxbUsbRootHub);

#define VXB_BUSID_USB                BUSTYPE_ID(vxbUsbBus)
#define VXB_BUSID_USB_ROOT_HUB       BUSTYPE_ID(vxbUsbRootHub)

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbUsbBush */





