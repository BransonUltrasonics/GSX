
ifndef __HEADER_ACCESS_PSL_INCLUDED
__HEADER_ACCESS_PSL_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_PSL = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/PSL),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), PSL))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/PSL
endif
endif

endif

endif

