
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
#	LIBDIRBASE = $(TOOL_COMMON_DIR)$(LIB_DIR_TAG)






ifeq (,)



endif


# DEP_OBJS = (DEP_OBJS)

# LIB_BASE_NAMES = debug

 

 
_SHARED_LIBOBJS_common_debug_user_shared_top_OSTOOLS_user_src_diab_debug = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objdebug/rtpDbgCmdLib.sho

_usrsharcmdfile_common_debug : $(_SHARED_LIBOBJS_common_debug_user_shared_top_OSTOOLS_user_src_diab_debug)

_SHARED_LIBOBJS_common_debug += $(_SHARED_LIBOBJS_common_debug_user_shared_top_OSTOOLS_user_src_diab_debug)

__OBJS_TO_CLEAN_OSTOOLS += $(_SHARED_LIBOBJS_common_debug_user_shared_top_OSTOOLS_user_src_diab_debug)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so : $(_SHARED_LIBOBJS_common_debug_user_shared_top_OSTOOLS_user_src_diab_debug)

ifndef _DONEBASE_USRSH_LIB_common_debug

_DONEBASE_USRSH_LIB_common_debug = TRUE

# LIBBASES += debug



usrshlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libdebug.so.1

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libdebug.so.1 : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin

__DIR_TARGETS += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libdebug.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so $@

# XXX debug
# true
# true


 
usrshlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/gnu/bin/libdebug.so.1

__DIR_TARGETS += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/gnu/bin C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/gnu C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/gnu/bin/libdebug.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/gnu/bin
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so $@

clean _layer_clean_OSTOOLS : _clean_USRSH_LIB_BASE_common_debug_SL_INSTALL_DIR_other

_clean_USRSH_LIB_BASE_common_debug_SL_INSTALL_DIR_other :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/gnu/bin/libdebug.so.1



C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common 

arcmdfiles : _usrsharcmdfile_common_debug

_usrsharcmdfile_common_debug :
	$(file >$@,$(sort $(_SHARED_LIBOBJS_common_debug)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so : | usrstlib
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dplus -tARMCORTEXA15MV:rtp7 -Y I,+C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public/c++03:C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public:C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/../../include/diab -Xansi -WDVSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB  -W:c++:.CPP  -Xfp-fast  -XO -Xforce-declarations -ei5387,5388,5849,1824    -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=diab -D_VX_TOOL=diab -DARMEL -DARMEL -DINET -DINET6   -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public -I.  -Xpic -Xshared -Xdynamic -soname=libdebug.so.1 -o  $@ -Wl,@_usrsharcmdfile_common_debug  -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/diab/PIC -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC  


clean _layer_clean_OSTOOLS : _clean_USRSH_LIB_BASE_common_debug

_clean_USRSH_LIB_BASE_common_debug :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libdebug.so

endif


 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
