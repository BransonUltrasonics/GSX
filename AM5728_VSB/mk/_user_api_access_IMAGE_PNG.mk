
ifndef __HEADER_ACCESS_IMAGE_PNG_INCLUDED
__HEADER_ACCESS_IMAGE_PNG_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_IMAGE_PNG = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/IMAGE_PNG),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), IMAGE_PNG))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/IMAGE_PNG
endif
endif

endif

endif

