# Automatically generated file: do not edit

##########################
# IPNET_RIP Section
##########################


ifdef _WRS_CONFIG_IPNET_RIP
VSBL_IPNET_RIP_SRC = 
VSBL_IPNET_RIP_DEPEND = 
ifdef _WRS_CONFIG_IPNET_1_1_1_2_ROUTEPROTO_1_0_1_5_RIP_1_0_0_5
VSBL_IPNET_RIP_SRC += IPNET_RIP
VSBL_IPNET_RIP_DEPEND += IPNET_COREIP
VSBL_IPNET_RIP_DEPEND += SEC_HASH
IPNET_RIP_FASTBUILD = YES
VSBL_IPNET_RIP_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.2/routeproto-1.0.1.5/rip
VSBL_IPNET_RIP_VERSION = IPNET_1_1_1_2_ROUTEPROTO_1_0_1_5_RIP_1_0_0_5
endif
ifdef _WRS_CONFIG_IPNET_1_1_1_3_ROUTEPROTO_1_0_1_5_RIP_1_0_0_5
VSBL_IPNET_RIP_SRC += IPNET_RIP
VSBL_IPNET_RIP_DEPEND += IPNET_COREIP
VSBL_IPNET_RIP_DEPEND += SEC_HASH
IPNET_RIP_FASTBUILD = YES
VSBL_IPNET_RIP_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/routeproto-1.0.1.5/rip
VSBL_IPNET_RIP_VERSION = IPNET_1_1_1_3_ROUTEPROTO_1_0_1_5_RIP_1_0_0_5
endif
endif
