
ifndef __HEADER_ACCESS_IMAGE_INCLUDED
__HEADER_ACCESS_IMAGE_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_IMAGE = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/IMAGE),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), IMAGE))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/IMAGE
endif
endif

endif

endif

