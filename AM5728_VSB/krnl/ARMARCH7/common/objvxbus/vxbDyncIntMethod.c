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

#include <vxWorks.h>
#include <hwif/vxBus.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/int/vxbDyncIntLib.h>
#include <hwif/methods/vxbDyncIntMethod.h>

VXB_DEVMETHOD_DEF(vxbIntAlloc, "VxBus interrupt allocate routine");

int VXB_INT_ALLOC (VXB_DEV_ID pDev, int count,  VXB_DYNC_INT_ENTRY * pVxbDyncIntEntry)
    {
    vxbIntAlloc_t * func = (vxbIntAlloc_t *) vxbDevMethodFind (pDev, VXB_DEVMETHOD_CALL(vxbIntAlloc));

    if (func == NULL)
        return (int)ERROR;

    return func (pDev, count, pVxbDyncIntEntry);
    }


VXB_DEVMETHOD_DEF(vxbIntFree, "VxBus interrupt free routine");

void VXB_INT_FREE (VXB_DEV_ID pDev, int count,  VXB_DYNC_INT_ENTRY * pVxbDyncIntEntry)
    {
    vxbIntFree_t * func = (vxbIntFree_t *) vxbDevMethodFind (pDev, VXB_DEVMETHOD_CALL(vxbIntFree));

    if (func == NULL)
        return;

    func (pDev, count, pVxbDyncIntEntry);
    }


