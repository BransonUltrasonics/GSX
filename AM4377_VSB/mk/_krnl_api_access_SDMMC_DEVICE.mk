
ifndef __HEADER_ACCESS_SDMMC_DEVICE_INCLUDED
__HEADER_ACCESS_SDMMC_DEVICE_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_SDMMC_DEVICE = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_DEVICE),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), SDMMC_DEVICE))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SDMMC_DEVICE
endif
endif

endif

endif

