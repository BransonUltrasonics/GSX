
ifndef __HEADER_ACCESS_FSAPI_INCLUDED
__HEADER_ACCESS_FSAPI_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_FSAPI = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/FSAPI),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), FSAPI))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/FSAPI
endif
endif

endif

endif

