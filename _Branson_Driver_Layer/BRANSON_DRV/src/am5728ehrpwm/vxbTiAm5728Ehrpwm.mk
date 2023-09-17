# Makefile - makefile for Epwm

# DESCRIPTION
#
# This file contains the makefile macro values for the
# vxbTiEpwm driver
#

ifdef _WRS_CONFIG_FDT
DOC_FILES     += vxbTiAm5728ehrpwm.c
OBJS_ARMARCH7M += vxbTiAm5728ehrpwm.o
OBJS_ARMARCH7 += vxbTiAm5728ehrpwm.o
endif
