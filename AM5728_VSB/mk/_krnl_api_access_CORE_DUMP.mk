
ifndef __HEADER_ACCESS_CORE_DUMP_INCLUDED
__HEADER_ACCESS_CORE_DUMP_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_CORE_DUMP = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/CORE_DUMP),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), CORE_DUMP))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/CORE_DUMP
endif
endif

endif

endif

