
ifndef __HEADER_ACCESS_SDMMC_SDHC_INCLUDED
__HEADER_ACCESS_SDMMC_SDHC_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_SDMMC_SDHC = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_SDHC),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), SDMMC_SDHC))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_SDHC
endif
endif

endif

endif

