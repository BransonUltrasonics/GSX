
ifndef __HEADER_ACCESS_FS_VDFS_INCLUDED
__HEADER_ACCESS_FS_VDFS_INCLUDED = TRUE





__SUBSCRIBE_ACCESS_FS_VDFS = 



ifneq ($(wildcard $(VSB_USER_PROTECTED_INCLUDE_DIR)/FS_VDFS),)

ifneq ($(VSBL_NAME),)
ifeq ($(VSBL_NAME),$(filter $(VSBL_NAME), FS_VDFS))
VSBL_LUH += $(OPTION_INCLUDE_DIR)$(VSB_USER_PROTECTED_INCLUDE_DIR)/FS_VDFS
endif
endif

endif

endif

