
ifndef __HEADER_ACCESS_IMAGE_JPEG_INCLUDED
__HEADER_ACCESS_IMAGE_JPEG_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_IMAGE_JPEG = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/IMAGE_JPEG),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), IMAGE_JPEG))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/IMAGE_JPEG
endif
endif

endif

endif

