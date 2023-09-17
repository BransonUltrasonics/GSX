# Makefile - makefile for http/src/rpm
#
# Copyright (c) 2002-2007, 2013-2014 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 06nov14,r_w  fix doc build error V7MAN-111
# 22apr13,r_w  modified for static analysis tool prevent.pl
# 19jun07,tpw  Use WRVX_COMPBASE vs. WIND_COMPONENTS correctly.
# 10apr07,msa  Another apigen removal of a non-shipping file.
# 24feb07,msa  Update This so it generates docs for all the files needed
#                  to match the 4.5 API doc.
# 13feb07,msa  Make changes to go with Phil's APIgen changes
# 16may05,tpw  Update documentation build settings.
# 14jan04,jws  Towards archive
# 06feb03,tf   Fixed Makefile for T2.2 directory structures.
# 01nov02,kw   created
#
# DESCRIPTION
# This file contains the makefile rules for building the http/rpm library
#

LIB_BASE_NAME = wmhttp

ifeq ($(SPACE),)
  SPACE = kernel
endif

override DOCS_ROOT = $(WIND_DOCS)/com.windriver.ide.doc.vxworks_apis_7/vxworks_webcli_api_reference_7/$(VSBL_NAME)

DOC_FILES= httpProcessPostData.c httpRpmAccessControl.c httpRpmAlias.c httpRpmAuth.c httpRpmCgi.c \
           httpRpmFileSystem.c httpRpmIpCheck.c httpRpmLog.c httpRpmMime.c \
           httpRpmPost.c httpRpmUpload.c httpRpmWmbParser.c httpRpmWmbPost.c \
           httpRpmPassword.c httpRpmLiveControl.c

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

PRE_INCLUDE=  -I..

# The user might set EXTRA_DEFINE on the command line, so append to it.
override EXTRA_DEFINE += -UCOMMON_IS_COMPONENT


ifeq ($(SPACE),user)
include $(WIND_USR_MK)/rules.library.mk
else
include $(WIND_KRNL_MK)/rules.library.mk
endif

