
ifndef __HEADER_ACCESS_USB_PHY_INCLUDED
__HEADER_ACCESS_USB_PHY_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_USB_PHY = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/USB_PHY),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), USB_PHY))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/USB_PHY
endif
endif

endif

endif
