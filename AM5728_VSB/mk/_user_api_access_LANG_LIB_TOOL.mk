
ifndef __HEADER_ACCESS_LANG_LIB_TOOL_INCLUDED
__HEADER_ACCESS_LANG_LIB_TOOL_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_LANG_LIB_TOOL = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/LANG_LIB_TOOL),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), LANG_LIB_TOOL))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/LANG_LIB_TOOL
endif
endif

endif

endif

