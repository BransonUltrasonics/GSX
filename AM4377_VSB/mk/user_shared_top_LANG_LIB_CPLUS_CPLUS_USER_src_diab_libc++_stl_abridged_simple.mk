
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
#	LIBDIRBASE = $(LIBDIRBASE_$(LIB_VARIANT))






ifeq (,)



endif


# DEP_OBJS = (DEP_OBJS)

# LIB_BASE_NAMES = stlabr

 
_STATIC_LIBOBJS_common_stlabr_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_abridged_simple = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/fiopen.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/gccex.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/gcctinfo.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/iomanip.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/iostream.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/limits.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/locale0.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/locale.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/string.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/strstrea.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/throw.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/wiostrea.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/nomemory.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/xfpostox.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/xlocinfo.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/xstart.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/wlocale.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/xlocale.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/ios.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/raisehan.o C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr/xgetglob.o

_usrstarcmdfile_common_stlabr : $(_STATIC_LIBOBJS_common_stlabr_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_abridged_simple)

_STATIC_LIBOBJS_common_stlabr += $(_STATIC_LIBOBJS_common_stlabr_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_abridged_simple)

__OBJS_TO_CLEAN_LANG_LIB_CPLUS_CPLUS_USER += $(_STATIC_LIBOBJS_common_stlabr_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_abridged_simple)

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libstlabr.a : $(_STATIC_LIBOBJS_common_stlabr_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_abridged_simple)

ifndef _DONEBASE_USRST_LIB_common_stlabr

_DONEBASE_USRST_LIB_common_stlabr = TRUE

# LIBBASES += stlabr

# C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common:
#	mkdir -p $@

# C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objstlabr :
#	mkdir -p $@

usrstlib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libstlabr.a

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libstlabr.a : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common

__STATIC_BUILT_LIBS += common_stlabr

__STLIB_common_stlabr = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libstlabr.a

arcmdfiles : _usrstarcmdfile_common_stlabr

_usrstarcmdfile_common_stlabr :
	$(file >$@,$(sort $(_STATIC_LIBOBJS_common_stlabr)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libstlabr.a : _usrstarcmdfile_common_stlabr | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @_usrstarcmdfile_common_stlabr

clean  _layer_clean_LANG_LIB_CPLUS_CPLUS_USER : _clean_USRST_LIB_BASE_common_stlabr

_clean_USRST_LIB_BASE_common_stlabr :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libstlabr.a

endif


 

 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
