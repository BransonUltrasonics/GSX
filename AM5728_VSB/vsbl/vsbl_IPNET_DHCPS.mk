# Automatically generated file: do not edit

##########################
# IPNET_DHCPS Section
##########################


ifdef _WRS_CONFIG_IPNET_DHCPS
VSBL_IPNET_DHCPS_SRC = 
VSBL_IPNET_DHCPS_DEPEND = 
ifdef _WRS_CONFIG_IPNET_1_1_1_2_DHCPS_1_0_0_10
VSBL_IPNET_DHCPS_SRC += IPNET_DHCPS
VSBL_IPNET_DHCPS_DEPEND += IPNET_COREIP
VSBL_IPNET_DHCPS_DEPEND += SEC_HASH
IPNET_DHCPS_FASTBUILD = YES
VSBL_IPNET_DHCPS_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.2/app/dhcps-1.0.0.10
VSBL_IPNET_DHCPS_VERSION = IPNET_1_1_1_2_DHCPS_1_0_0_10
endif
ifdef _WRS_CONFIG_IPNET_1_1_1_3_DHCPS_1_0_0_10
VSBL_IPNET_DHCPS_SRC += IPNET_DHCPS
VSBL_IPNET_DHCPS_DEPEND += IPNET_COREIP
VSBL_IPNET_DHCPS_DEPEND += SEC_HASH
IPNET_DHCPS_FASTBUILD = YES
VSBL_IPNET_DHCPS_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/dhcps-1.0.0.10
VSBL_IPNET_DHCPS_VERSION = IPNET_1_1_1_3_DHCPS_1_0_0_10
endif
endif
