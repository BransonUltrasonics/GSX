
ifndef __HEADER_ACCESS_BDM_INCLUDED
__HEADER_ACCESS_BDM_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_BDM = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/BDM),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), BDM))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/BDM
endif
endif

endif

endif

