
#
# This file is automatically generated by mk/usr/defs.fastlibgen.mk.
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
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

# LIB_BASE_NAMES = stlstd

 
_STATIC_LIBOBJS_common_stlstd_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_simple = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/fiopen.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/gccex.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/gcctinfo.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/iomanip.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/iostream.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/limits.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/locale0.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/locale.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/string.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/strstrea.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/throw.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/wiostrea.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/nomemory.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/xfpostox.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/xlocinfo.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/xstart.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/wlocale.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/xlocale.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/ios.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/raisehan.o C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd/xgetglob.o

_usrstarcmdfile_common_stlstd : $(_STATIC_LIBOBJS_common_stlstd_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_simple)

_STATIC_LIBOBJS_common_stlstd += $(_STATIC_LIBOBJS_common_stlstd_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_simple)

__OBJS_TO_CLEAN_LANG_LIB_CPLUS_CPLUS_USER += $(_STATIC_LIBOBJS_common_stlstd_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_simple)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libstlstd.a : $(_STATIC_LIBOBJS_common_stlstd_user_shared_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_simple)

ifndef _DONEBASE_USRST_LIB_common_stlstd

_DONEBASE_USRST_LIB_common_stlstd = TRUE

# LIBBASES += stlstd

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common:
#	mkdir -p $@

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objstlstd :
#	mkdir -p $@

usrstlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libstlstd.a

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libstlstd.a : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common

__STATIC_BUILT_LIBS += common_stlstd

__STLIB_common_stlstd = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libstlstd.a

arcmdfiles : _usrstarcmdfile_common_stlstd

_usrstarcmdfile_common_stlstd :
	$(file >$@,$(sort $(_STATIC_LIBOBJS_common_stlstd)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libstlstd.a : _usrstarcmdfile_common_stlstd | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @_usrstarcmdfile_common_stlstd

clean  _layer_clean_LANG_LIB_CPLUS_CPLUS_USER : _clean_USRST_LIB_BASE_common_stlstd

_clean_USRST_LIB_BASE_common_stlstd :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libstlstd.a

endif


 

 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 