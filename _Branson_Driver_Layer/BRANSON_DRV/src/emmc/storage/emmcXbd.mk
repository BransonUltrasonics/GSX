# Makefile - makefile for usb
#
# modification history
# --------------------
# 27mar14,e_d   create (US29286)
#
# DESCRIPTION
# This file contains the makefile rules for building the usb library
#
#

ifdef _WRS_CONFIG_FDT
DOC_FILES	+= emmcXbd.c
OBJS_COMMON += emmcXbd.o
endif
