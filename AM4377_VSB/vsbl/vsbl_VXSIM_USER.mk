# Automatically generated file: do not edit

##########################
# VXSIM_USER Section
##########################


ifdef _WRS_CONFIG_VXSIM_USER
VSBL_VXSIM_USER_SRC = 
VSBL_VXSIM_USER_DEPEND = 
ifdef _WRS_CONFIG_VXSIM_1_0_1_1_USER_1_0_0_0
VSBL_VXSIM_USER_SRC += VXSIM_USER
VXSIM_USER_FASTBUILD = YES
VSBL_VXSIM_USER_PATH = $(WIND_BASE)/pkgs/os/arch/simulator-1.0.1.1/user
VSBL_VXSIM_USER_VERSION = VXSIM_1_0_1_1_USER_1_0_0_0
endif
ifdef _WRS_CONFIG_VXSIM_1_0_7_8_USER_1_0_2_2
VSBL_VXSIM_USER_SRC += VXSIM_USER
ifdef _WRS_CONFIG_COMPAT69&
endif
ifndef _WRS_CONFIG_LIBDL_7X
VSBL_VXSIM_USER_DEPEND += LIBDL_7X
endif
VXSIM_USER_FASTBUILD = YES
VSBL_VXSIM_USER_PATH = $(WIND_BASE)/pkgs/os/arch/simulator-1.0.7.8/user
VSBL_VXSIM_USER_VERSION = VXSIM_1_0_7_8_USER_1_0_2_2
endif
endif

