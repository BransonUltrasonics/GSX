
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

# LIB_BASE_NAMES = AioPx

 

 
_SHARED_LIBOBJS_common_AioPx_user_shared_top_CORE_IO_src_diab_aio = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objAioPx/aioSysDrv.sho  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objAioPx/ioQLib.sho  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objAioPx/aioPxShow.sho  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objAioPx/aioPxLib.sho

_usrsharcmdfile_common_AioPx : $(_SHARED_LIBOBJS_common_AioPx_user_shared_top_CORE_IO_src_diab_aio)

_SHARED_LIBOBJS_common_AioPx += $(_SHARED_LIBOBJS_common_AioPx_user_shared_top_CORE_IO_src_diab_aio)

__OBJS_TO_CLEAN_CORE_IO += $(_SHARED_LIBOBJS_common_AioPx_user_shared_top_CORE_IO_src_diab_aio)

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so : $(_SHARED_LIBOBJS_common_AioPx_user_shared_top_CORE_IO_src_diab_aio)

ifndef _DONEBASE_USRSH_LIB_common_AioPx

_DONEBASE_USRSH_LIB_common_AioPx = TRUE

# LIBBASES += AioPx



usrshlib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin/libAioPx.so.1

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin/libAioPx.so.1 : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin

__DIR_TARGETS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin/libAioPx.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so $@

# XXX AioPx
# false
# true


 
usrshlib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/gnu/bin/libAioPx.so.1

__DIR_TARGETS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/gnu/bin C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/gnu C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/gnu/bin/libAioPx.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/gnu/bin
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so $@

clean _layer_clean_CORE_IO : _clean_USRSH_LIB_BASE_common_AioPx_SL_INSTALL_DIR_other

_clean_USRSH_LIB_BASE_common_AioPx_SL_INSTALL_DIR_other :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/gnu/bin/libAioPx.so.1



C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common 

arcmdfiles : _usrsharcmdfile_common_AioPx

_usrsharcmdfile_common_AioPx :
	$(file >$@,$(sort $(_SHARED_LIBOBJS_common_AioPx)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so : | usrstlib
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dplus -tARMCORTEXA9MV:rtp7 -Y I,+C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/public/c++03:C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/public:C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/../../include/diab -Xansi -WDVSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB  -W:c++:.CPP  -Xfp-fast  -XO -Xforce-declarations -ei5387,5388,5849,1824    -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=diab -D_VX_TOOL=diab -DARMEL -DARMEL -DINET -DINET6   -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/public -I.  -Xpic -Xshared -Xdynamic -soname=libAioPx.so.1 -o  $@ -Wl,@_usrsharcmdfile_common_AioPx  -LC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/diab/PIC -LC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/PIC  


clean _layer_clean_CORE_IO : _clean_USRSH_LIB_BASE_common_AioPx

_clean_USRSH_LIB_BASE_common_AioPx :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libAioPx.so

endif


 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
