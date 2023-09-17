# Makefile - makefile for Epwm

# DESCRIPTION
#
# This file contains the makefile macro values for the
# vxbAm5728TiEpwm driver
#

ifdef _WRS_CONFIG_FDT
DOC_FILES     += vxbAm5728TiEpwm.c
OBJS_ARMARCH7M += vxbAm5728TiEpwm.o
OBJS_ARMARCH7 += vxbAm5728TiEpwm.o
endif
