# Automatically generated file: do not edit

##########################
# GDOI Section
##########################


ifdef _WRS_CONFIG_GDOI
VSBL_GDOI_SRC = 
VSBL_GDOI_DEPEND = 
ifdef _WRS_CONFIG_GDOI_1_0_0_2
VSBL_GDOI_SRC += GDOI
VSBL_GDOI_DEPEND += IPNET_IPSECIKE
VSBL_GDOI_DEPEND += OPENSSL
GDOI_FASTBUILD = YES
GDOI_FRIEND = IPNET_IKE IPNET_IPSEC
VSBL_GDOI_PATH = $(WIND_BASE)/pkgs/security/gdoi-1.0.0.2
VSBL_GDOI_VERSION = GDOI_1_0_0_2
endif
endif

