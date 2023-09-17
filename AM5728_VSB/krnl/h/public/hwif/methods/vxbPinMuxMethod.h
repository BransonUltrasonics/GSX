/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
 *  DO NOT MODIFY THIS FILE MANUALLY
 *
 *  This file is automatically generated from
 *  the method file (extended with *.m).
 *
 */

#ifndef _INCvxbPinMuxMethodh
#define _INCvxbPinMuxMethodh

VXB_DEVMETHOD_DECL(vxbPinMuxEnable)
typedef STATUS vxbPinMuxEnable_t (VXB_DEV_ID pDev, INT32 offset);
STATUS VXB_PIN_MUX_ENABLE (VXB_DEV_ID pDev, INT32 offset);

VXB_DEVMETHOD_DECL(vxbPinMuxDisable)
typedef STATUS vxbPinMuxDisable_t (VXB_DEV_ID pDev, INT32 offset);
STATUS VXB_PIN_MUX_DISABLE (VXB_DEV_ID pDev, INT32 offset);

VXB_DEVMETHOD_DECL(vxbPinMuxShow)
typedef void vxbPinMuxShow_t (VXB_DEV_ID pDev, INT32 verbose);
void VXB_PIN_MUX_SHOW (VXB_DEV_ID pDev, INT32 verbose);

#endif /* _INCvxbPinMuxMethodh */
