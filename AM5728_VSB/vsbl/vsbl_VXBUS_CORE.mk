# Automatically generated file: do not edit

##########################
# VXBUS_CORE Section
##########################


ifdef _WRS_CONFIG_VXBUS_CORE
VSBL_VXBUS_CORE_SRC = 
VSBL_VXBUS_CORE_DEPEND = 
ifdef _WRS_CONFIG_VXBUS_1_0_5_0_CORE_1_0_8_1
ifdef _WRS_CONFIG_VXBUS_VXTEST
VSBL_VXBUS_CORE_SRC += VXBUS_VXTEST
endif
VSBL_VXBUS_CORE_SRC += VXBUS_CORE
ifndef _WRS_CONFIG_CERT
VSBL_VXBUS_CORE_DEPEND += ERF
endif
VSBL_VXBUS_CORE_DEPEND += VXBUS
VXBUS_CORE_FASTBUILD = YES
VSBL_VXBUS_CORE_PATH = $(WIND_BASE)/pkgs/os/drv/vxbus-1.0.5.0/core-1.0.8.1
VSBL_VXBUS_CORE_VERSION = VXBUS_1_0_5_0_CORE_1_0_8_1
endif
endif

