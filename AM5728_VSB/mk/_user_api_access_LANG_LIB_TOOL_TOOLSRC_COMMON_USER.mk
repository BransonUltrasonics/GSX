
ifndef __HEADER_ACCESS_LANG_LIB_TOOL_TOOLSRC_COMMON_USER_INCLUDED
__HEADER_ACCESS_LANG_LIB_TOOL_TOOLSRC_COMMON_USER_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_LANG_LIB_TOOL_TOOLSRC_COMMON_USER = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/LANG_LIB_TOOL_TOOLSRC_COMMON_USER),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), LANG_LIB_TOOL_TOOLSRC_COMMON_USER))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/LANG_LIB_TOOL_TOOLSRC_COMMON_USER
endif
endif

endif

endif

