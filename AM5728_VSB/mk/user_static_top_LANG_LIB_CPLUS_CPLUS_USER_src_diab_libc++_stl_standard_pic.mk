
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
#	LIBDIRBASE = common/PIC
#	LIBDIRBASE = $(LIBDIRBASE_$(LIB_VARIANT))






ifeq (,)



endif


# DEP_OBJS = (DEP_OBJS)

# LIB_BASE_NAMES = stlstd

 
_STATIC_LIBOBJS_common_PIC_stlstd_user_static_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_pic = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/fiopen.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/gccex.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/gcctinfo.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/iomanip.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/iostream.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/limits.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/locale0.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/locale.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/string.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/strstrea.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/throw.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/wiostrea.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/nomemory.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/xfpostox.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/xlocinfo.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/xstart.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/wlocale.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/xlocale.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/ios.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/raisehan.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd/xgetglob.sho

_usrstarcmdfile_common_PIC_stlstd : $(_STATIC_LIBOBJS_common_PIC_stlstd_user_static_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_pic)

_STATIC_LIBOBJS_common_PIC_stlstd += $(_STATIC_LIBOBJS_common_PIC_stlstd_user_static_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_pic)

__OBJS_TO_CLEAN_LANG_LIB_CPLUS_CPLUS_USER += $(_STATIC_LIBOBJS_common_PIC_stlstd_user_static_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_pic)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/libstlstd.a : $(_STATIC_LIBOBJS_common_PIC_stlstd_user_static_top_LANG_LIB_CPLUS_CPLUS_USER_src_diab_libc++_stl_standard_pic)

ifndef _DONEBASE_USRST_LIB_common_PIC_stlstd

_DONEBASE_USRST_LIB_common_PIC_stlstd = TRUE

# LIBBASES += stlstd

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC:
#	mkdir -p $@

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/objstlstd :
#	mkdir -p $@

usrstlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/libstlstd.a

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/libstlstd.a : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC

__STATIC_BUILT_LIBS += common_PIC_stlstd

__STLIB_common_PIC_stlstd = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/libstlstd.a

arcmdfiles : _usrstarcmdfile_common_PIC_stlstd

_usrstarcmdfile_common_PIC_stlstd :
	$(file >$@,$(sort $(_STATIC_LIBOBJS_common_PIC_stlstd)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/libstlstd.a : _usrstarcmdfile_common_PIC_stlstd | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @_usrstarcmdfile_common_PIC_stlstd

clean  _layer_clean_LANG_LIB_CPLUS_CPLUS_USER : _clean_USRST_LIB_BASE_common_PIC_stlstd

_clean_USRST_LIB_BASE_common_PIC_stlstd :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC/libstlstd.a

endif


 

 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
