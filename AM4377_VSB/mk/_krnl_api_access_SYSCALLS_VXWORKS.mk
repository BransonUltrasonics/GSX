
ifndef __HEADER_ACCESS_SYSCALLS_VXWORKS_INCLUDED
__HEADER_ACCESS_SYSCALLS_VXWORKS_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_SYSCALLS_VXWORKS = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SYSCALLS_VXWORKS),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), SYSCALLS_VXWORKS))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/SYSCALLS_VXWORKS
endif
endif

endif

endif

