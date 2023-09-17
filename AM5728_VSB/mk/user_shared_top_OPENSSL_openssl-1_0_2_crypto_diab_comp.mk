
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

# LIB_BASE_NAMES = OPENSSL

 

 
_SHARED_LIBOBJS_common_OPENSSL_user_shared_top_OPENSSL_openssl-1_0_2_crypto_diab_comp = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/comp_err.sho  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/comp_lib.sho  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/c_rle.sho  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/c_zlib.sho

_usrsharcmdfile_common_OPENSSL : $(_SHARED_LIBOBJS_common_OPENSSL_user_shared_top_OPENSSL_openssl-1_0_2_crypto_diab_comp)

_SHARED_LIBOBJS_common_OPENSSL += $(_SHARED_LIBOBJS_common_OPENSSL_user_shared_top_OPENSSL_openssl-1_0_2_crypto_diab_comp)

__OBJS_TO_CLEAN_OPENSSL += $(_SHARED_LIBOBJS_common_OPENSSL_user_shared_top_OPENSSL_openssl-1_0_2_crypto_diab_comp)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.so : $(_SHARED_LIBOBJS_common_OPENSSL_user_shared_top_OPENSSL_openssl-1_0_2_crypto_diab_comp)

ifndef _DONEBASE_USRSH_LIB_common_OPENSSL

_DONEBASE_USRSH_LIB_common_OPENSSL = TRUE

# LIBBASES += OPENSSL



usrshlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libOPENSSL.so.1

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libOPENSSL.so.1 : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin

__DIR_TARGETS += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libOPENSSL.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.so
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.so $@

# XXX OPENSSL
# false
# 



C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.so : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common 

arcmdfiles : _usrsharcmdfile_common_OPENSSL

_usrsharcmdfile_common_OPENSSL :
	$(file >$@,$(sort $(_SHARED_LIBOBJS_common_OPENSSL)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.so : | usrstlib
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dplus -tARMCORTEXA15MV:rtp7 -Y I,+C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public/c++03:C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public:C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/../../include/diab -Xansi -WDVSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB  -W:c++:.CPP  -Xfp-fast  -XO -w    -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=diab -D_VX_TOOL=diab -DARMEL -DARMEL -DOPENSSL_NO_INLINE_ASM -DOPENSSL_NO_ASM   -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public -I.  -Xpic -Xshared -Xdynamic -soname=libOPENSSL.so.1 -o  $@ -Wl,@_usrsharcmdfile_common_OPENSSL  -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/diab/PIC -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC  


clean _layer_clean_OPENSSL : _clean_USRSH_LIB_BASE_common_OPENSSL

_clean_USRSH_LIB_BASE_common_OPENSSL :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.so

endif


 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
