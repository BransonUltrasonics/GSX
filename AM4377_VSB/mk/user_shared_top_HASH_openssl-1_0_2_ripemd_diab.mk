
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

# LIB_BASE_NAMES = HASH

 

 
_SHARED_LIBOBJS_common_HASH_user_shared_top_HASH_openssl-1_0_2_ripemd_diab = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objHASH/rmd_dgst.sho  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/objHASH/rmd_one.sho

_usrsharcmdfile_common_HASH : $(_SHARED_LIBOBJS_common_HASH_user_shared_top_HASH_openssl-1_0_2_ripemd_diab)

_SHARED_LIBOBJS_common_HASH += $(_SHARED_LIBOBJS_common_HASH_user_shared_top_HASH_openssl-1_0_2_ripemd_diab)

__OBJS_TO_CLEAN_HASH += $(_SHARED_LIBOBJS_common_HASH_user_shared_top_HASH_openssl-1_0_2_ripemd_diab)

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libHASH.so : $(_SHARED_LIBOBJS_common_HASH_user_shared_top_HASH_openssl-1_0_2_ripemd_diab)

ifndef _DONEBASE_USRSH_LIB_common_HASH

_DONEBASE_USRSH_LIB_common_HASH = TRUE

# LIBBASES += HASH



usrshlib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin/libHASH.so.1

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin/libHASH.so.1 : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin

__DIR_TARGETS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/root/diab/bin/libHASH.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libHASH.so
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libHASH.so $@

# XXX HASH
# false
# 



C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libHASH.so : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common 

arcmdfiles : _usrsharcmdfile_common_HASH

_usrsharcmdfile_common_HASH :
	$(file >$@,$(sort $(_SHARED_LIBOBJS_common_HASH)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libHASH.so : | usrstlib
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dplus -tARMCORTEXA9MV:rtp7 -Y I,+C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/public/c++03:C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/public:C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/../../include/diab -Xansi -WDVSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB  -W:c++:.CPP  -Xfp-fast  -XO -w    -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=diab -D_VX_TOOL=diab -DARMEL -DARMEL -DOPENSSL_NO_INLINE_ASM -DOPENSSL_NO_ASM   -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h/public -I.  -Xpic -Xshared -Xdynamic -soname=libHASH.so.1 -o  $@ -Wl,@_usrsharcmdfile_common_HASH  -LC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/diab/PIC -LC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/PIC  


clean _layer_clean_HASH : _clean_USRSH_LIB_BASE_common_HASH

_clean_USRSH_LIB_BASE_common_HASH :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common/libHASH.so

endif


 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
