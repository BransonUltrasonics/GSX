
ifndef __HEADER_ACCESS_OPENSSL_INCLUDED
__HEADER_ACCESS_OPENSSL_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_OPENSSL = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/OPENSSL),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), OPENSSL))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/OPENSSL
endif
endif

endif

endif
