# Automatically generated file: do not edit

##########################
# SDMMC Section
##########################


ifdef _WRS_CONFIG_SDMMC
VSBL_SDMMC_SRC = 
VSBL_SDMMC_DEPEND = 
ifdef _WRS_CONFIG_SDMMC_1_0_1_3
ifdef _WRS_CONFIG_SDMMC_CORE
VSBL_SDMMC_SRC += SDMMC_CORE
endif
ifdef _WRS_CONFIG_SDMMC_DEVICE
VSBL_SDMMC_SRC += SDMMC_DEVICE
endif
ifdef _WRS_CONFIG_SDMMC_STORAGE
VSBL_SDMMC_SRC += SDMMC_STORAGE
endif
ifdef _WRS_CONFIG_SDMMC_HOST
VSBL_SDMMC_SRC += SDMMC_HOST
endif
ifdef _WRS_CONFIG_SDMMC_SDHC
VSBL_SDMMC_SRC += SDMMC_SDHC
endif
ifdef _WRS_CONFIG_SDMMC_TIMMCHS
VSBL_SDMMC_SRC += SDMMC_TIMMCHS
endif
VSBL_SDMMC_SRC += SDMMC
SDMMC_FASTBUILD = YES
VSBL_SDMMC_PATH = $(WIND_BASE)/pkgs/connectivity/sdmmc-1.0.1.3
VSBL_SDMMC_VERSION = SDMMC_1_0_1_3
endif
endif

