# Automatically generated file: do not edit

##########################
# IPNET_PTP Section
##########################


ifdef _WRS_CONFIG_IPNET_PTP
VSBL_IPNET_PTP_SRC = 
VSBL_IPNET_PTP_DEPEND = 
ifdef _WRS_CONFIG_IPNET_1_1_1_2_PTP_1_0_1_1
VSBL_IPNET_PTP_SRC += IPNET_PTP
VSBL_IPNET_PTP_DEPEND += END_LIB
VSBL_IPNET_PTP_DEPEND += IPNET_COREIP
VSBL_IPNET_PTP_DEPEND += END
IPNET_PTP_FASTBUILD = YES
VSBL_IPNET_PTP_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.2/app/ptp-1.0.1.1
VSBL_IPNET_PTP_VERSION = IPNET_1_1_1_2_PTP_1_0_1_1
endif
ifdef _WRS_CONFIG_IPNET_1_1_1_3_PTP_1_0_1_1
VSBL_IPNET_PTP_SRC += IPNET_PTP
VSBL_IPNET_PTP_DEPEND += END_LIB
VSBL_IPNET_PTP_DEPEND += IPNET_COREIP
VSBL_IPNET_PTP_DEPEND += END
IPNET_PTP_FASTBUILD = YES
VSBL_IPNET_PTP_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/ptp-1.0.1.1
VSBL_IPNET_PTP_VERSION = IPNET_1_1_1_3_PTP_1_0_1_1
endif
endif

