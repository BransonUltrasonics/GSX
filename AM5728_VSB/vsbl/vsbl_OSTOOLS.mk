# Automatically generated file: do not edit

##########################
# OSTOOLS Section
##########################


ifdef _WRS_CONFIG_OSTOOLS
VSBL_OSTOOLS_SRC = 
VSBL_OSTOOLS_DEPEND = 
ifdef _WRS_CONFIG_OSTOOLS_1_0_2_7
ifdef _WRS_CONFIG_OSTOOLS_VXTEST
VSBL_OSTOOLS_SRC += OSTOOLS_VXTEST
endif
VSBL_OSTOOLS_SRC += OSTOOLS
VSBL_OSTOOLS_DEPEND += CORE_KERNEL
VSBL_OSTOOLS_DEPEND += CORE_KERNEL
OSTOOLS_FASTBUILD = YES
VSBL_OSTOOLS_PATH = $(WIND_BASE)/pkgs/os/utils/ostools-1.0.2.7
VSBL_OSTOOLS_VERSION = OSTOOLS_1_0_2_7
endif
endif

