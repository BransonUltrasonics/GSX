# Automatically generated file: do not edit

##########################
# WEBCLI_HTTP Section
##########################


ifdef _WRS_CONFIG_WEBCLI_HTTP
VSBL_WEBCLI_HTTP_SRC = 
VSBL_WEBCLI_HTTP_DEPEND = 
ifdef _WRS_CONFIG_WEBCLI_1_0_0_3_HTTP_1_0_1_6
VSBL_WEBCLI_HTTP_SRC += WEBCLI_HTTP
VSBL_WEBCLI_HTTP_DEPEND += FS_DOSFS
VSBL_WEBCLI_HTTP_DEPEND += ZLIB
VSBL_WEBCLI_HTTP_DEPEND += WEBCLI_COMMON
WEBCLI_HTTP_FASTBUILD = YES
VSBL_WEBCLI_HTTP_PATH = $(WIND_BASE)/pkgs/app/webcli-1.0.0.3/http-1.0.1.6
VSBL_WEBCLI_HTTP_VERSION = WEBCLI_1_0_0_3_HTTP_1_0_1_6
endif
endif

