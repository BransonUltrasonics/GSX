# Makefile - makefile for Epwm

# DESCRIPTION
#
# This file contains the makefile macro values for the
# vxbTiEpwm driver
#

ifdef _WRS_CONFIG_FDT
DOC_FILES     += vxbTiEpwm.c
OBJS_ARMARCH7M += vxbTiEpwm.o
OBJS_ARMARCH7 += vxbTiEpwm.o
endif
