# rules.bsp.mk - make rules
#
# Copyright 2015-2016 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 18aug16,cfm  replace LOAD_ADDR with LOCAL_MEM_PHYS_ADRS (V7PRO-3207)
# 03aug16,jnl  modified rules to fix bootapp fails to load the vxWorks image
#              (V7PRO-3188)
# 12nov15,yjw  created (US64529)

# DESCRIPTION
# This file contains rules for building VxWorks for the TI Sitara
# boards. This board features the Texas Instruments Sitara processor
# with an ARM Cortex-A15 core.
#

# INTERNAL
# This file should only contain rules specific to the BSP.  Definitions
# specific to this BSP should be placed in the defs file (defs.bsp.mk)
#

# adjust lma for all sections.

__WRVX_BSP_POST_BUILD_RULE += @                                    \
        $(CP) $@ $@_swap ;                                         \
        $(OBJCPY) --change-section-lma .*+0x$(LOCAL_MEM_PHYS_ADRS) \
                  --adjust-start 0x$(LOCAL_MEM_PHYS_ADRS)          \
                  --no-change-warnings $@ > /dev/null 2>&1 ;
