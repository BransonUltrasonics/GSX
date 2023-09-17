# Automatically generated file: do not edit

##########################
# LOADER Section
##########################


ifdef _WRS_CONFIG_LOADER
VSBL_LOADER_SRC = 
VSBL_LOADER_DEPEND = 
ifdef _WRS_CONFIG_LOADER_1_1_2_0
ifdef _WRS_CONFIG_LOADER_VXTEST
VSBL_LOADER_SRC += LOADER_VXTEST
endif
VSBL_LOADER_SRC += LOADER
VSBL_LOADER_DEPEND += OSTOOLS
VSBL_LOADER_DEPEND += CORE_KERNEL
LOADER_FASTBUILD = YES
VSBL_LOADER_PATH = $(WIND_BASE)/pkgs/os/utils/loader-1.1.2.0
VSBL_LOADER_VERSION = LOADER_1_1_2_0
endif
endif

