
ifndef __HEADER_ACCESS_LANG_LIB_LIBC_INCLUDED
__HEADER_ACCESS_LANG_LIB_LIBC_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_LANG_LIB_LIBC = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/LANG_LIB_LIBC),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), LANG_LIB_LIBC))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/LANG_LIB_LIBC
endif
endif

endif

endif

