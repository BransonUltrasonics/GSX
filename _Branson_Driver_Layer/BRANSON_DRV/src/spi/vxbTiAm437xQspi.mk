# Makefile - makefile for vxbTiAm572xQspi.c

# DESCRIPTION
#
# This file contains the makefile macro values for the
# vxbTiQspi driver
#

#OBJS_ARMARCH7M += vxbTiQspi.o

#OBJS_ARMARCH7 += vxbTiQspi.o


ifdef _WRS_CONFIG_FDT

ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

DOC_FILES	+= vxbTiAm437xQspi.c
OBJS_COMMON += vxbTiAm437xQspi.o
endif
