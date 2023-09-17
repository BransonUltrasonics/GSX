# Makefile - makefile for http/src
#
# Copyright (c) 2002-2007, 2012-2014 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 06nov14,r_w  fix doc build error V7MAN-111
# 18oct13,jpb  Removed include of VSB config file
# 22apr13,r_w  modified for static analysis tool prevent.pl
# 04jun12,r_w  Suppport for _WRS_CONFIG_PRECOMP_RESTRICTED(requirement WIND00350129)
# 19jun07,tpw  Use WRVX_COMPBASE vs. WIND_COMPONENTS correctly.
# 02apr07,msa  Remove files which produce no apigen documentation from the
#              DOC_FILES lists
# 29mar07,msa  Remove files that are EXCLUDEd by .resource files.
# 23feb07,msa  Update This so it generates docs for all the files needed
#              to match the 4.5 API doc.
# 12feb07,msa  Add Phil's APIgen changes
# 16may05,tpw  Update documentation build settings.
# 13jan04,jws  moving toward WM component
# 06feb03,tf   Added the rpm directory.
# 05feb03,tf   Fixed Makefile for T2.2 directory structures.
# 01nov02,kw   created
#
# DESCRIPTION
# This file contains the makefile rules for building the wmw library
#

LIB_BASE_NAME = wmhttp

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))

ifeq ($(SPACE),)
  SPACE = kernel
endif

HTTP_INCLUDE_DIRS += 	-Iprivate

WRN_DEFINITIONS= -D__VxWORKS__ -D__BIG_ENUMS__ -DWRN_PD_CODE

ifeq ($(SPACE),user)
include $(WIND_USR_MK)/defs.default.mk
else
include $(WIND_KRNL_MK)/defs.library.mk
endif

HTTP_DEFINITIONS=

# HTTP definitions for turning components on and off ...
# HTTP definitions for supoorted features in vxWorks...

#HTTP definitions for unsupported feaures which are disabled by default...
HTTP_DEFINITIONS+= -UCOMMON_IS_COMPONENT
HTTP_DEFINITIONS+= -UWEBCLI_ON_LINUX


#HTTP definitions for defining the capabilities of the OS
HTTP_DEFINITIONS+= -DHAVE_CONFIG_H

#SDA defaults to disabled on some architectures and breaks perfectly functioning builds
SDA_DISABLE=TRUE

override DOCS_ROOT = $(WIND_DOCS)/com.windriver.ide.doc.vxworks_apis_7/vxworks_webcli_api_reference_7/$(VSBL_NAME)

DOC_FILES =  httpIsMatching.c httpStrdup.c httpStrcpy.c httpStringFuncs.c \
             httpStrCaseCmp.c httpBase64Util.c httpDate.c \
             httpErrorHandling.c httpAuthUtils.c httpGenericListUtil.c \
             httpIo.c httpLineEscape.c httpLineUnescape.c httpFileRegistry.c \
             httpLib.c httpMem.c httpMimeAcceptEncoding.c \
             httpMimeAcceptLanguage.c httpMimeConnection.c \
             httpMimeContentEncodingAfn.c httpMimeContentLength.c \
             httpMimeContentLengthAfn.c httpMimeContentType.c \
             httpMimeContentTypeAfn.c httpMimeCookie.c httpMimeDateAfn.c \
             httpMimeHdrAccessFns.c httpMimeHost.c httpMimeKeepAlive.c \
             httpMimeParse.c httpMimePersist.c \
             httpMimePersistentConnectionAfn.c httpMimeUserAgent.c \
             httpMultipart.c httpOutput.c httpParseUtils.c \
             httpRequestTranslation.c httpRequestUtil.c httpRpmDispatch.c \
             httpSysRpmConfUtil.c \
             httpWmbHandlers.c httpMemGzio.c httpFileProcess.c \
             httpFileMgr.c httpWmbExpandInstance.c httpLCLib.c

ENCRYPTION_FILES = httpIo.c httpRequestTranslation.c
UNENCRYPTION_FILES = httpIsMatching.c httpStrdup.c httpStrcpy.c httpStringFuncs.c \
             httpStrCaseCmp.c httpBase64Util.c httpDate.c \
             httpErrorHandling.c httpAuthUtils.c httpGenericListUtil.c \
             httpLineEscape.c httpLineUnescape.c httpFileRegistry.c \
             httpLib.c httpMem.c httpMimeAcceptEncoding.c \
             httpMimeAcceptLanguage.c httpMimeConnection.c \
             httpMimeContentEncodingAfn.c httpMimeContentLength.c \
             httpMimeContentLengthAfn.c httpMimeContentType.c \
             httpMimeContentTypeAfn.c httpMimeCookie.c httpMimeDateAfn.c \
             httpMimeHdrAccessFns.c httpMimeHost.c httpMimeKeepAlive.c \
             httpMimeParse.c httpMimePersist.c \
             httpMimePersistentConnectionAfn.c httpMimeUserAgent.c \
             httpMultipart.c httpOutput.c httpParseUtils.c \
             httpRequestUtil.c httpRpmDispatch.c \
             httpSysRpmConfUtil.c \
             httpWmbHandlers.c httpMemGzio.c httpFileProcess.c \
             httpFileMgr.c httpWmbExpandInstance.c httpLCLib.c \
	     httpglob.c httpLCIO.c httpMimeAccept.c \
	     httpMimeEtag.c httpMimeIfMatch.c httpMimeIfModifiedSince.c \
	     httpMimeIfNoneMatch.c httpMimeIfUnmodifiedSince.c httpNvFileMgr.c \
	     httpOsalCom.c httpOsalSem.c httpOsalTime.c httpOsalUtil.c \
	     httpPortfolio.c httpResults.c httpSaxC.c httpShow.c httpWmbFileIncludeHandler.c \
	     httpWmbMacro.c httpXmlParser.c httpAuth.c httpNonce.c

SRC_FILES= $(ENCRYPTION_FILES) $(UNENCRYPTION_FILES)

# include defs.library to get the VSB definitions
#include $(WIND_KRNL_MK)/defs.library.mk
ifndef __DEFS_VSBVARS_MK_INCLUDED
include $(WIND_KRNL_MK)/defs.vsbvars.mk
endif
-include $(VSB_MAKE_CONFIG_FILE) 

OBJS= $(SRC_FILES:%c=%o)
ENCRYPTION_OBJS= $(ENCRYPTION_FILES:%c=%o)

# support for the _WRS_CONFIG_PRECOMP_RESTRICTED flag
ifdef _WRS_CONFIG_PRECOMP_RESTRICTED
OBJS_PRECOMP=$(ENCRYPTION_OBJS)
else
OBJS+=
endif

# The user might set EXTRA_DEFINE on the command line, so append to it.

ifeq ($(DEBUG),YES)
override EXTRA_DEFINE += -g -O0 -D__VxDEBUG__ -D__WRN_DEF_DEBUG__ $(WRN_DEFINITIONS) $(HTTP_DEFINITIONS) $(HTTP_INCLUDE_DIRS)
else
override EXTRA_DEFINE += $(WRN_DEFINITIONS) $(HTTP_DEFINITIONS) $(HTTP_INCLUDE_DIRS)
endif

ifeq ($(SPACE),user)
include $(WIND_USR_MK)/rules.library.mk
else
include $(WIND_KRNL_MK)/rules.library.mk
endif

