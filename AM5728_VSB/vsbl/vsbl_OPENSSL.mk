# Automatically generated file: do not edit

##########################
# OPENSSL Section
##########################


ifdef _WRS_CONFIG_OPENSSL
VSBL_OPENSSL_SRC = 
VSBL_OPENSSL_DEPEND = 
ifdef _WRS_CONFIG_OPENSSL_1_1_1_0
VSBL_OPENSSL_SRC += OPENSSL
VSBL_OPENSSL_DEPEND += HASH
VSBL_OPENSSL_DEPEND += HASH
VSBL_OPENSSL_DEPEND += SEC_CRYPTO
OPENSSL_FASTBUILD = YES
VSBL_OPENSSL_PATH = $(WIND_BASE)/pkgs/security/openSSL-1.1.1.0
VSBL_OPENSSL_VERSION = OPENSSL_1_1_1_0
endif
endif

