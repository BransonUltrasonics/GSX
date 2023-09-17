# Makefile - makefile for vxbTiAm335xSpi
#
# Copyright (c) 2012 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 28nov12,mpc  created
#
# DESCRIPTION
# This file contains the makefile macro values for the
# vxbTiAm335xSpi driver
#

ifdef _WRS_CONFIG_FDT
DOC_FILES     += vxbTiAm437xSpi.c
OBJS_ARMARCH7 += vxbTiAm437xSpi.o
endif
