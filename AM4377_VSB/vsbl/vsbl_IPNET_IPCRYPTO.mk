# Automatically generated file: do not edit

##########################
# IPNET_IPCRYPTO Section
##########################


ifdef _WRS_CONFIG_IPNET_IPCRYPTO
VSBL_IPNET_IPCRYPTO_SRC = 
VSBL_IPNET_IPCRYPTO_DEPEND = 
ifdef _WRS_CONFIG_IPNET_1_1_1_2_CRYPTO_1_0_0_9_IPCRYPTO_1_0_0_6
VSBL_IPNET_IPCRYPTO_SRC += IPNET_IPCRYPTO
VSBL_IPNET_IPCRYPTO_DEPEND += IPNET_COREIP
VSBL_IPNET_IPCRYPTO_DEPEND += OPENSSL
IPNET_IPCRYPTO_FASTBUILD = YES
VSBL_IPNET_IPCRYPTO_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.2/crypto-1.0.0.9/ipcrypto
VSBL_IPNET_IPCRYPTO_VERSION = IPNET_1_1_1_2_CRYPTO_1_0_0_9_IPCRYPTO_1_0_0_6
endif
ifdef _WRS_CONFIG_IPNET_1_1_1_3_CRYPTO_1_0_0_9_IPCRYPTO_1_0_0_6
VSBL_IPNET_IPCRYPTO_SRC += IPNET_IPCRYPTO
VSBL_IPNET_IPCRYPTO_DEPEND += IPNET_COREIP
VSBL_IPNET_IPCRYPTO_DEPEND += OPENSSL
IPNET_IPCRYPTO_FASTBUILD = YES
VSBL_IPNET_IPCRYPTO_PATH = $(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/crypto-1.0.0.9/ipcrypto
VSBL_IPNET_IPCRYPTO_VERSION = IPNET_1_1_1_3_CRYPTO_1_0_0_9_IPCRYPTO_1_0_0_6
endif
endif

