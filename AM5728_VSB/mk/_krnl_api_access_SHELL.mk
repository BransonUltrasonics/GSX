
ifndef __HEADER_ACCESS_SHELL_INCLUDED
__HEADER_ACCESS_SHELL_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_SHELL = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SHELL),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), SHELL))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SHELL
endif
endif

endif

endif

