
ifndef __HEADER_ACCESS_USB_HOST_INCLUDED
__HEADER_ACCESS_USB_HOST_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_USB_HOST = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/USB_HOST),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), USB_HOST))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/USB_HOST
endif
endif

endif

endif

