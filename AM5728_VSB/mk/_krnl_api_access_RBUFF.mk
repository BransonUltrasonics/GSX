
ifndef __HEADER_ACCESS_RBUFF_INCLUDED
__HEADER_ACCESS_RBUFF_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_RBUFF = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/RBUFF),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), RBUFF))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/RBUFF
endif
endif

endif

endif

