# Makefile - makefile for user code
#
# Copyright (c) 2014, 2018 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 19jun18,lan  Add wm_filesys.c for little endian, add wm_filesys_be.c 
#              for big endian. 
# 28mar14,r_w  created
#
# DESCRIPTION
# This file contains the makefile rules for building the webdemo library
#
#

LIB_BASE_NAME = webclidemo

include $(WIND_KRNL_MK)/defs.vsbvars.mk

SRC_FILES = rcc_handler.c sampleHttpUserFuncs.c sampleJSUser.c sampleLiveCtrlApp.c sampleDynamicTable.c \
            sampleSmtpApp.c sampleRamDisk.c sampleUserCgi.c sampleDevice.c sampleCheckbox.c sampleUserRpm.c \
            wmw_httpconf.c webCliCommonInit.c webCliHttpInit.c webCliCliInit.c \
            wmc_webclidemo.c wmb_webclidemo.c 

ifeq ($(_WRS_CONFIG_ENDIAN), little)
SRC_FILES += wm_filesys.c
else
SRC_FILES += wm_filesys_be.c
endif

OBJS = $(SRC_FILES:%c=%o)

ifeq ($(SPACE),user)
include $(WIND_USR_MK)/rules.library.mk
else
include $(WIND_KRNL_MK)/rules.library.mk
endif

