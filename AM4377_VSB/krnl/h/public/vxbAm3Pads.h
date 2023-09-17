/* vxbAm3Pads.h - TI Pin Contiguration header file */

/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
29sep16,y_y  written.
*/

/* includes */

#include <vxWorks.h>
#include <subsys/pinmux/vxbPinMuxLib.h>

typedef struct am3PadsDrvCtrl
    {
    VXB_DEV_ID          pInst;
    VXB_PINMUX_CTRL *   pPmxCtrl;        
    void *              pRegbase;
    void *              pHandle;
    } AM3_PADS_DRVCTRL;
