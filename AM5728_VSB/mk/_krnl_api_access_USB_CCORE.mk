
ifndef __HEADER_ACCESS_USB_CCORE_INCLUDED
__HEADER_ACCESS_USB_CCORE_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_USB_CCORE = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/USB_CCORE),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), USB_CCORE))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/USB_CCORE
endif
endif

endif

endif

