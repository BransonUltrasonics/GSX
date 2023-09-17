# Automatically generated file: do not edit

##########################
# OPTEE_DEMO Section
##########################


ifdef _WRS_CONFIG_OPTEE_DEMO
VSBL_OPTEE_DEMO_SRC = 
VSBL_OPTEE_DEMO_DEPEND = 
ifdef _WRS_CONFIG_OPTEE_1_0_0_1_DEMO_1_0_0_0
VSBL_OPTEE_DEMO_SRC += OPTEE_DEMO
VSBL_OPTEE_DEMO_DEPEND += OPTEE_LIB
OPTEE_DEMO_FASTBUILD = YES
VSBL_OPTEE_DEMO_PATH = $(WIND_BASE)/pkgs/security/optee_client_api-1.0.0.1/demo
VSBL_OPTEE_DEMO_VERSION = OPTEE_1_0_0_1_DEMO_1_0_0_0
endif
endif

