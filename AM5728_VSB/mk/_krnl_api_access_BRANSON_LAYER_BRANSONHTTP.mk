
ifndef __HEADER_ACCESS_BRANSON_LAYER_BRANSONHTTP_INCLUDED
__HEADER_ACCESS_BRANSON_LAYER_BRANSONHTTP_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_BRANSON_LAYER_BRANSONHTTP = 



ifneq ($(wildcard $(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/BRANSON_LAYER_BRANSONHTTP),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), BRANSON_LAYER_BRANSONHTTP))
VSBL_LKH += $(OPTION_INCLUDE_DIR)$(VSB_KERNEL_PROTECTED_INCLUDE_DIR)/BRANSON_LAYER_BRANSONHTTP
endif
endif

endif

endif

