
ifndef __HEADER_ACCESS_CORE_KERNEL_INCLUDED
__HEADER_ACCESS_CORE_KERNEL_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_CORE_KERNEL = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/CORE_KERNEL),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), CORE_KERNEL))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/CORE_KERNEL
endif
endif

endif

endif

