
#
# This file is automatically generated by mk/usr/defs.fastlibgen.mk.
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
#	CPU : ARMARCH7
#	TOOL : gnu
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = common
#	LIBDIRBASE = $(LIBDIRBASE_$(LIB_VARIANT))






ifeq (,)



endif


# DEP_OBJS = (DEP_OBJS)

# LIB_BASE_NAMES = c

 

 
_SHARED_LIBOBJS_common_c_user_shared_top_LANG_LIB_TOOL_TOOLSRC_GNU_USER_src_gnu_libc_pic = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objc/unwind-c.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objc/unwind-arm.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objc/libunwind.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objc/unwind-dw2-fde.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objc/pr-support.sho C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objc/emutls.sho

_usrsharcmdfile_common_c : $(_SHARED_LIBOBJS_common_c_user_shared_top_LANG_LIB_TOOL_TOOLSRC_GNU_USER_src_gnu_libc_pic)

_SHARED_LIBOBJS_common_c += $(_SHARED_LIBOBJS_common_c_user_shared_top_LANG_LIB_TOOL_TOOLSRC_GNU_USER_src_gnu_libc_pic)

__OBJS_TO_CLEAN_LANG_LIB_TOOL_TOOLSRC_GNU_USER += $(_SHARED_LIBOBJS_common_c_user_shared_top_LANG_LIB_TOOL_TOOLSRC_GNU_USER_src_gnu_libc_pic)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libc.so : $(_SHARED_LIBOBJS_common_c_user_shared_top_LANG_LIB_TOOL_TOOLSRC_GNU_USER_src_gnu_libc_pic)

ifndef _DONEBASE_USRSH_LIB_common_c

_DONEBASE_USRSH_LIB_common_c = TRUE

# LIBBASES += c



usrshlib : /libc.so.1

/libc.so.1 : | 

__DIR_TARGETS +=   

/libc.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libc.so
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libc.so $@

# XXX c
# 
# 



C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libc.so : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common 

arcmdfiles : _usrsharcmdfile_common_c

_usrsharcmdfile_common_c :
	$(file >$@,$(sort $(_SHARED_LIBOBJS_common_c)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libc.so : | usrstlib
	C:/WindRiverSR500/compilers/gnu-4.8.1.9/x86-win32/bin/c++arm -t7 -mfpu=vfp -mfloat-abi=softfp -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -std=c99 -fasm  -O2  -fstrength-reduce -fno-builtin -Wall     -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -DARMEL -DARMEL    -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h -isystemC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h -isystemC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/system -isystemC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public -I.  -fpic -D__SO_PICABILINUX__  -shared -Wl,-soname,libc.so.1 -o  $@ -Wl,@_usrsharcmdfile_common_c  -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/gnu/PIC -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC  


clean _layer_clean_LANG_LIB_TOOL_TOOLSRC_GNU_USER : _clean_USRSH_LIB_BASE_common_c

_clean_USRSH_LIB_BASE_common_c :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libc.so

endif


 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
