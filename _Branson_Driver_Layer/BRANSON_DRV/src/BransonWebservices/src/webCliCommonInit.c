/* webCliCommonInit.c */

/* Copyright (c) 2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
23may14,r_w  create file
*/

#include "wm_options.h"
#include "commonCfg.h"

extern int sysClkRateGet(void);
void webcli_common_init(void)
{
  global_kWmosThreadCreateOptions = kWmosThreadCreateOptions;
  global_kHwTicksPerSecond = kHwTicksPerSecond;
}
