# Automatically generated file: do not edit

##########################
# IPNET_IPSEC Section
##########################


ifdef _WRS_CONFIG_IPNET_IPSEC
VSBL_IPNET_IPSEC_SRC = 
VSBL_IPNET_IPSEC_DEPEND = 
ifdef _WRS_CONFIG_IPNET_1_1_1_2_IPSECIKE_1_0_1_10_IPSEC_1_0_0_9
VSBL_IPNET_IPSEC_SRC += IPNET_IPSEC
VSBL_IPNET_IPSEC_DEPEND += IPNET_COREIP
VSBL_IPNET_IPSEC_DEPEND += SEC_CRYPTO
VSBL_IPNET_IPSEC_DEPEND += SEC_HASH
VSBL_IPNET_IPSEC_DEPEND += OPENSSL
IPNET_IPSEC_FASTBUILD = YES
IPNET_IPSEC_FRIEND = GDOI IPNET_COREIP CRYPTOMISC_IPHWCRYPTO
VSBL_IPNET_IPSEC_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.2/ipsecike-1.0.1.10/ipsec
VSBL_IPNET_IPSEC_VERSION = IPNET_1_1_1_2_IPSECIKE_1_0_1_10_IPSEC_1_0_0_9
endif
ifdef _WRS_CONFIG_IPNET_1_1_1_3_IPSECIKE_1_0_1_10_IPSEC_1_0_0_9
VSBL_IPNET_IPSEC_SRC += IPNET_IPSEC
VSBL_IPNET_IPSEC_DEPEND += IPNET_COREIP
VSBL_IPNET_IPSEC_DEPEND += SEC_CRYPTO
VSBL_IPNET_IPSEC_DEPEND += SEC_HASH
VSBL_IPNET_IPSEC_DEPEND += OPENSSL
IPNET_IPSEC_FASTBUILD = YES
IPNET_IPSEC_FRIEND = GDOI IPNET_COREIP CRYPTOMISC_IPHWCRYPTO
VSBL_IPNET_IPSEC_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/ipsecike-1.0.1.10/ipsec
VSBL_IPNET_IPSEC_VERSION = IPNET_1_1_1_3_IPSECIKE_1_0_1_10_IPSEC_1_0_0_9
endif
endif

