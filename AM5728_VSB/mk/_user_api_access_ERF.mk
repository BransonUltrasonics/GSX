
ifndef __HEADER_ACCESS_ERF_INCLUDED
__HEADER_ACCESS_ERF_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_ERF = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/ERF),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), ERF))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/ERF
endif
endif

endif

endif

