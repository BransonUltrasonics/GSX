# Makefile - makefile for /src/hwif
#
# Copyright (c) Branson
#
# modification history
# --------------------
# TBD
#
# DESCRIPTION
# This file contains the makefile rules for building the vxBus library

ifdef _WRS_CONFIG_FDT
DOC_FILES	+= vxbeMmcLib.c
OBJS_COMMON += vxbeMmcLib.o
endif
