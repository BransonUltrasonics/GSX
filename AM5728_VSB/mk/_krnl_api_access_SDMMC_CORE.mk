
ifndef __HEADER_ACCESS_SDMMC_CORE_INCLUDED
__HEADER_ACCESS_SDMMC_CORE_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_SDMMC_CORE = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_CORE),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), SDMMC_CORE))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_CORE
endif
endif

endif

endif

