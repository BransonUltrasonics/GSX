
ifndef __HEADER_ACCESS_AUDIO_LIB_INCLUDED
__HEADER_ACCESS_AUDIO_LIB_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_AUDIO_LIB = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/AUDIO_LIB),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), AUDIO_LIB))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/AUDIO_LIB
endif
endif

endif

endif

