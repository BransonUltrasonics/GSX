
#
# This file is automatically generated by mk/usr/defs.fastlibgen.mk.
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = common
#	LIBDIRBASE = $(TOOL_COMMON_DIR)$(LIB_DIR_TAG)






ifeq (,)



endif


# DEP_OBJS = (DEP_OBJS)

# LIB_BASE_NAMES = net dsi

 
_STATIC_LIBOBJS_common_net_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab = 

_usrstarcmdfile_common_net : $(_STATIC_LIBOBJS_common_net_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

_STATIC_LIBOBJS_common_net += $(_STATIC_LIBOBJS_common_net_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

__OBJS_TO_CLEAN_LANG_LIB_TOOL_TOOLSRC_DIAB_USER += $(_STATIC_LIBOBJS_common_net_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libnet.a : $(_STATIC_LIBOBJS_common_net_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

ifndef _DONEBASE_USRST_LIB_common_net

_DONEBASE_USRST_LIB_common_net = TRUE

# LIBBASES += net

# C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common:
#	mkdir -p $@

# C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/obj :
#	mkdir -p $@

usrstlib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libnet.a

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libnet.a : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common

__STATIC_BUILT_LIBS += common_net

__STLIB_common_net = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libnet.a

arcmdfiles : _usrstarcmdfile_common_net

_usrstarcmdfile_common_net :
	$(file >$@,$(sort $(_STATIC_LIBOBJS_common_net)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libnet.a : _usrstarcmdfile_common_net | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @_usrstarcmdfile_common_net

clean  _layer_clean_LANG_LIB_TOOL_TOOLSRC_DIAB_USER : _clean_USRST_LIB_BASE_common_net

_clean_USRST_LIB_BASE_common_net :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libnet.a

endif
  
_STATIC_LIBOBJS_common_dsi_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab = 

_usrstarcmdfile_common_dsi : $(_STATIC_LIBOBJS_common_dsi_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

_STATIC_LIBOBJS_common_dsi += $(_STATIC_LIBOBJS_common_dsi_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

__OBJS_TO_CLEAN_LANG_LIB_TOOL_TOOLSRC_DIAB_USER += $(_STATIC_LIBOBJS_common_dsi_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libdsi.a : $(_STATIC_LIBOBJS_common_dsi_user_shared_top_LANG_LIB_TOOL_TOOLSRC_DIAB_USER_libs_diab)

ifndef _DONEBASE_USRST_LIB_common_dsi

_DONEBASE_USRST_LIB_common_dsi = TRUE

# LIBBASES += dsi

# C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common:
#	mkdir -p $@

# C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/obj :
#	mkdir -p $@

usrstlib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libdsi.a

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libdsi.a : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common

__STATIC_BUILT_LIBS += common_dsi

__STLIB_common_dsi = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libdsi.a

arcmdfiles : _usrstarcmdfile_common_dsi

_usrstarcmdfile_common_dsi :
	$(file >$@,$(sort $(_STATIC_LIBOBJS_common_dsi)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libdsi.a : _usrstarcmdfile_common_dsi | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @_usrstarcmdfile_common_dsi

clean  _layer_clean_LANG_LIB_TOOL_TOOLSRC_DIAB_USER : _clean_USRST_LIB_BASE_common_dsi

_clean_USRST_LIB_BASE_common_dsi :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libdsi.a

endif

 
   

   

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
