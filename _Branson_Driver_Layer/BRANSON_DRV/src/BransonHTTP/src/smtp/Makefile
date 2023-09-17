# Makefile - makefile for http/src/smtp
#
# Copyright (c) 2003-2007, 2012-2014 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 06nov14,r_w  fix doc build error V7MAN-111
# 22apr13,r_w  modified for static analysis tool prevent.pl
# 04jun12,r_w  Suppport for _WRS_CONFIG_PRECOMP_RESTRICTED(requirement WIND00350129)
# 19jun07,tpw  Use WRVX_COMPBASE vs. WIND_COMPONENTS correctly.
# 13feb07,msa  Make changes to go with Phil's APIgen changes
# 16may05,tpw  Update documentation build settings.
# 15jan04,jws  toward componentization
# 03oct03,jws  copied from rpm and modified
#
# DESCRIPTION
# This file contains the makefile rules for building the http/smtp library
#

LIB_BASE_NAME = wmhttp

ifeq ($(SPACE),)
  SPACE = kernel
endif

override DOCS_ROOT = $(WIND_DOCS)/com.windriver.ide.doc.vxworks_apis_7/vxworks_webcli_api_reference_7/$(VSBL_NAME)

DOC_FILES= smtpLib.c

SRC_FILES= $(DOC_FILES)

# include defs.library to get the VSB definitions
ifeq ($(SPACE),user)
include $(WIND_USR_MK)/defs.default.mk
else
include $(WIND_KRNL_MK)/defs.library.mk
endif

ifndef __DEFS_VSBVARS_MK_INCLUDED
include $(WIND_KRNL_MK)/defs.vsbvars.mk
endif
-include $(VSB_MAKE_CONFIG_FILE) 

OBJS= $(SRC_FILES:%c=%o)


# support for the _WRS_CONFIG_PRECOMP_RESTRICTED flag
ifdef _WRS_CONFIG_PRECOMP_RESTRICTED
OBJS_PRECOMP=
else
OBJS+=
endif

#
# PRE_INCLUDE ensures that these directories get searched before the
# standard, which is what we want if WM_BASE is not under WIND_BASE.
#

PRE_INCLUDE=   -I..

# The user might set EXTRA_DEFINE on the command line, so append to it.
override EXTRA_DEFINE += -UCOMMON_IS_COMPONENT


ifeq ($(SPACE),user)
include $(WIND_USR_MK)/rules.library.mk
else
include $(WIND_KRNL_MK)/rules.library.mk
endif

