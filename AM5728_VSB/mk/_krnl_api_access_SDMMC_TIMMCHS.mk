
ifndef __HEADER_ACCESS_SDMMC_TIMMCHS_INCLUDED
__HEADER_ACCESS_SDMMC_TIMMCHS_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_SDMMC_TIMMCHS = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_TIMMCHS),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), SDMMC_TIMMCHS))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_TIMMCHS
endif
endif

endif

endif

