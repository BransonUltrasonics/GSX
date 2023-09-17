# defs.bsp.mk - make variable definitions
#
# Copyright 2014 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01jul14,swu created (US40646)

# DESCRIPTION
# This file contains definitions for building VxWorks for the TI Sitara
# boards. This board features the Texas Instruments Sitara processor
# with an ARM Cortex-A9 core.
#

# INTERNAL
# This file should only contain definitions specific to the BSP.  Any rules
# specific to this BSP should be placed in the rules file (rules.bsp.mk)
#

VFPVARIANT      = VFPV3

LOAD_ADDR       = 0x80000000

# build binary target

vxWorks.bin:    vxWorks
		- @ $(RM) $@
		$(EXTRACT_BIN) vxWorks_swap $@
