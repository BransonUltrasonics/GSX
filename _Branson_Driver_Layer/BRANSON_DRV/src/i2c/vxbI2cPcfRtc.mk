# Makefile - makefile for vxbI2cPcfRtc.c 
#
# Copyright (c) 2014 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01jan14,mpc  written (US22546)
#
# DESCRIPTION
# This file contains the makefile macro values for the
# I2C RTC driver
#

ifdef _WRS_CONFIG_FDT

ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

DOC_FILES	+= vxbI2cPcfRtc.c
OBJS_COMMON     += vxbI2cPcfRtc.o

endif
